#include "stdafx.h"
#include "OnlyList.h"
#include "EventDungeonManager.h"


CEventDungeonManager* g_pEventDungeonManager = NULL;


CEventDungeonManager::CEventDungeonManager()
{
	m_pEventDungeonList	= new COnlyList(MAX_EVENT_DUNGEON_NUMBER);	
	m_pPropertyList		= new CVoidDontDeleteList();
}


CEventDungeonManager::~CEventDungeonManager()
{	
	Clear();
}


// 이벤트 던젼 '속성'들을 DB로부터 불러온다.
BOOL CEventDungeonManager::QueryEventDungeonProperties()
{
	EVENT_DUNGEON_PROPERTY* pEventDungeonProperties = new EVENT_DUNGEON_PROPERTY[MAX_EVENT_DUNGEON_KIND];

	int iCount	= 0;
	int i		= 0;

	if(NULL != pEventDungeonProperties)
	{
		iCount = g_pDb->OpenRecord("Select [DungeonNumber], [EventType], [EnterLevelMin], [EnterLevelMax], [OpenTime], [ReadyOpenTime], [OpenTimeUpTime], [FirstCloseTime], [CloseTime], [MaxEnterNo], [PositionType] From EventDungeonProperty Order by EventType",
			pEventDungeonProperties,MAX_EVENT_DUNGEON_KIND,(BYTE)GAME_DB);

		if(NULL != m_pPropertyList)
		{	
			for(i = 0; i < iCount; i++)
			{
				EVENT_DUNGEON_PROPERTY* pProperty = new EVENT_DUNGEON_PROPERTY;
				memcpy(pProperty, pEventDungeonProperties + i, sizeof(EVENT_DUNGEON_PROPERTY));

				pProperty->dwFirstCloseTime	*= 1000;
				pProperty->dwCloseTime		*= 1000;
				pProperty->dwOpenTime		*= 1000;
				pProperty->dwOpenTimeUpTime	*= 1000;
				pProperty->dwReadyOpenTime	*= 1000;
				
				m_pPropertyList->AddTail(pProperty);			
			}
		}

		delete [] pEventDungeonProperties;
		pEventDungeonProperties = NULL;		
	} 

	return TRUE;
}


// 이벤트 던젼들의 리스트는 이벤트 던젼 메니져에 한번 더 등록하여 따로 처리 될 수 있게 한다.
BOOL CEventDungeonManager::AddEventDungeon(DUNGEON_DATA_EX* pDungeon)
{
	if(NULL == pDungeon)			return FALSE;
	if(NULL == m_pEventDungeonList)	return FALSE;
	
	pDungeon->m_pListPos				= m_pEventDungeonList->AddTail(pDungeon);
	pDungeon->m_dwEventLastActionTick	= g_dwCurTick;				

	return TRUE;
}


void CEventDungeonManager::Clear()
{
	ClearPropertyInfo();

	if(NULL != m_pPropertyList)
	{
		delete m_pPropertyList;
		m_pPropertyList = NULL;
	}

	if(NULL != m_pEventDungeonList)
	{
		delete m_pEventDungeonList;
		m_pEventDungeonList = NULL;
	}
}


BOOL CEventDungeonManager::ClearPropertyInfo()
{
	if(NULL == m_pPropertyList)		return FALSE;
	
	EVENT_DUNGEON_PROPERTY* pProperty	= NULL;
	int						iNodeNumber = 0;
	int						i			= 0;

	iNodeNumber = m_pPropertyList->GetCount();
	
	for (i = 0; i < iNodeNumber; i++)
	{
		pProperty = (EVENT_DUNGEON_PROPERTY*)m_pPropertyList->GetHead();

		if (NULL != pProperty)
		{
			delete pProperty;
			pProperty = NULL;
		}

		m_pPropertyList->RemoveHead();
	}	

	return TRUE;
}


EVENT_DUNGEON_PROPERTY* CEventDungeonManager::GetEventProperty(DWORD dwDungeonNumber) const
{
	if(NULL == m_pPropertyList)	return NULL;

	EVENT_DUNGEON_PROPERTY* pProperty = NULL;
	POSITION_ pos = m_pPropertyList->GetHeadPosition();

	while(pos)
	{
		pProperty = (EVENT_DUNGEON_PROPERTY*)m_pPropertyList->GetNext(pos);		

		if(dwDungeonNumber == (DWORD)pProperty->dwDungeonNumber)
		{
			return pProperty;
		}		
	}

	return NULL;	
}


// 20초마다 실행된다. 오픈시간이 되었는지 체크 해서 EVENT_DUNGEON_READY 상태로 만든다.
void CEventDungeonManager::ProcessRelationWithCreate()
{
	if(IS_ABLE_NATION(NC_JAPAN))
	{
		return ;
	}

	if(NULL == m_pEventDungeonList)		return;	

	POSITION_				pos			= m_pEventDungeonList->GetHeadPosition();
	DUNGEON_DATA_EX*		pDungeon	= NULL;
	EVENT_DUNGEON_PROPERTY*	pProperty	= NULL;

	while(pos)
	{
		pDungeon = (DUNGEON_DATA_EX*)m_pEventDungeonList->GetNext(pos);
		if(NULL == pDungeon)	continue;

		pProperty = GetEventProperty(pDungeon->m_dwID);
		if(NULL == pProperty)	return;

		if(EVENT_DUNGEON_CLOSE == pDungeon->m_bEventState)
		{
			if( (g_dwCurTick - pDungeon->m_dwEventLastActionTick) >= pProperty->dwOpenTime )// : 050108 duk9
			{
				if(pDungeon->m_pServer)
				{
					if(SERVER_STATUS_CONNECTED == pDungeon->m_pServer->dwServerStatus)
					{
						pDungeon->EventChangeState(EVENT_DUNGEON_READY, NULL);				
					}
				}				
			}
		}	
	}
}


// 0.5초마다 실행된다. 이벤트 던젼의 상태를 변화시킨다.
void CEventDungeonManager::Process()
{	
	POSITION_				pos					= m_pEventDungeonList->GetHeadPosition();
	DUNGEON_DATA_EX*		pDungeon			= NULL;
	EVENT_DUNGEON_PROPERTY*	pProperty			= NULL;
	DWORD					dwCurrentTick		= g_dwCurTick;
	DWORD					dwLastActionTick	= NULL; 
	
	while(pos)
	{		
		pDungeon = (DUNGEON_DATA_EX*)m_pEventDungeonList->GetNext(pos);
		if(NULL == pDungeon)	continue;

		pProperty = GetEventProperty(pDungeon->m_dwID);		
		if(NULL == pProperty)	continue;		
		
		if(DUNGEON_TYPE_EVENT == pDungeon->GetDungeonType())
		{
			dwLastActionTick = dwCurrentTick - pDungeon->GetEventLastActionTick();

			switch(pDungeon->m_bEventState)
			{
				case EVENT_DUNGEON_CLOSE:	break;
					
				case EVENT_DUNGEON_READY:
				{
					if(dwLastActionTick >= pProperty->dwReadyOpenTime)
					{
						pDungeon->EventChangeState(EVENT_DUNGEON_OPEN, dwCurrentTick);
//						pDungeon->SendEventInfo(pProperty->dwCloseTime + dwCurrentTick); //: 050108 hwoarang 
						pDungeon->SendEventInfo(pProperty->dwCloseTime);
					}					
				}
				break;
				
				case EVENT_DUNGEON_OPEN:
				{
					if(dwLastActionTick >= pProperty->dwOpenTimeUpTime)
					{
						pDungeon->EventChangeState(EVENT_DUNGEON_OPEN_TIME_UP, NULL);
						pDungeon->SendEventInfo(NULL);				
						pDungeon->DoNotExistEventDungeonEntered(dwCurrentTick, pProperty);									
					}					
				}
				break;	
				
				case EVENT_DUNGEON_OPEN_FULL:	break;

				case EVENT_DUNGEON_OPEN_TIME_UP:
				{
					if(pDungeon->DoNotExistEventDungeonEntered(dwCurrentTick, pProperty))
						break;
					
					if(EVENT_DUNGEON_SURVIVAL == pProperty->byDungeonType)
					{
						if(dwLastActionTick >= pProperty->dwFirstCloseTime)
						{
							pDungeon->EventChangeState(EVENT_DUNGEON_FIRST_CLOSE, NULL);
							pDungeon->SendEventInfoFirstClose();
							break;
						}						
					}

					if(dwLastActionTick >= pProperty->dwCloseTime - 10000)
					{
						pDungeon->SendEventInfoClose();
					}
					
					if(dwLastActionTick >= pProperty->dwCloseTime)
					{
						pDungeon->EventChangeState(EVENT_DUNGEON_CLOSE, dwCurrentTick);
						pDungeon->SendEventInfo(NULL);						
						pDungeon->ResetEventInfo(TRUE);	
					}				
				}
				break;

				case EVENT_DUNGEON_FIRST_CLOSE:
				{
					if(EVENT_DUNGEON_SURVIVAL == pProperty->byDungeonType)
					{
						if(pDungeon->DoNotExistEventDungeonEntered(dwCurrentTick, pProperty))
							break;
						
						if(dwLastActionTick >= pProperty->dwCloseTime - 10000)
						{
							pDungeon->SendEventInfoClose();
						}

						if(dwLastActionTick >= pProperty->dwCloseTime)
						{
							pDungeon->EventChangeState(EVENT_DUNGEON_CLOSE, dwCurrentTick);
							pDungeon->SendEventInfo(NULL);							
							pDungeon->ResetEventInfo(TRUE);	
						}						
					}					
				}
				break;
			}
		}
	}	
}








