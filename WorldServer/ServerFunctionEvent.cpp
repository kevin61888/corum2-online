#include "Stdafx.h"
#include "ServerFunctionEvent.h"
#include "EventDungeonManager.h"
#include "Notice.h"
#include "ServerMessage.h"


void __stdcall CreateEventJudgment(DWORD dwVal)
{
	dwVal;
	
	START_FUNCTION_PROFILE()

	if(NULL != g_pEventDungeonManager)
	{
		g_pEventDungeonManager->ProcessRelationWithCreate();
	}

	END_FUNCTION_PROFILE();
}


BOOL QueryEventDungeonProperties()
{
	EVENT_DUNGEON_PROPERTY* pEventDungeonProperties = new EVENT_DUNGEON_PROPERTY[MAX_EVENT_DUNGEON_KIND];

	int iCount	= 0;
	int i		= 0;

	if(NULL != pEventDungeonProperties)
	{
		iCount = g_pDb->OpenRecord("Select [DungeonNumber], [EventType], [EnterLevelMin], [EnterLevelMax], [OpenTime], [ReadyOpenTime], [OpenTimeUpTime], [FirstCloseTime], [CloseTime], [MaxEnterNo], [PositionType] From EventDungeonProperty Order by EventType",
			pEventDungeonProperties,MAX_EVENT_DUNGEON_KIND,(BYTE)GAME_DB);

		if(NULL != g_pEventDungeonManager->m_pPropertyList)
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
				
				g_pEventDungeonManager->m_pPropertyList->AddTail(pProperty);			
			}
		}

		delete [] pEventDungeonProperties;
		pEventDungeonProperties = NULL;		
	} 

	return TRUE;
}


void UnloadEventDungeonProperties(void)
{
	if (NULL != g_pEventDungeonManager)
	{
		g_pEventDungeonManager->Clear();

		delete g_pEventDungeonManager;
		g_pEventDungeonManager = NULL;
	}
}


bool UnloadNoticeMessage(void)
{
	if (NULL != g_pNoticeMessage)
	{
		delete[] g_pNoticeMessage;
		g_pNoticeMessage = NULL;
	}

	g_dwNoticeMessageCount = 0;

	return true;
}


bool LoadNoticeMessage(void)
{
	try
	{
		// Initialize require global variables.		
		UnloadNoticeMessage();			
		
		// Load Resource from database.
		NOTICE_MESSAGE aNoticeMessage[MAX_NOTICE_COUNT];
		
		int iCount = 0;
		iCount = g_pDb->OpenRecord("Select [No], [Important], [Type], [Message], [Show] From NOTICE_MESSAGE",
									aNoticeMessage,
									MAX_NOTICE_COUNT,
									(BYTE)TOTAL_DB);
		
		if (0 < iCount)
		{
			// Alloc require memory and copy notice message data to the buffer.			
			g_dwNoticeMessageCount = static_cast<DWORD>(iCount);
			
			// Exception process.
			g_pNoticeMessage = new NOTICE_MESSAGE[g_dwNoticeMessageCount];
			if (NULL == g_pNoticeMessage)
			{
				throw false;
			}

			memset(g_pNoticeMessage, 0, sizeof(NOTICE_MESSAGE) * g_dwNoticeMessageCount);

			// Actually copy data.
			memcpy(g_pNoticeMessage, aNoticeMessage, sizeof(NOTICE_MESSAGE) * g_dwNoticeMessageCount);

			throw true;			
		}
		else
		{
			// Load failure process.			
			g_dwNoticeMessageCount = 0;			
			throw false;
		}
	}
	catch (bool bResult)
	{
		return bResult;
	}	
}


void UnLoadServerMessage(void)
{
	if (NULL != g_pServerMessage)
	{
		delete[] g_pServerMessage;
		g_pServerMessage = NULL;
	}

	g_dwServerMessageCount = 0;	
}


bool LoadServerMessage(void)
{
	try
	{
		UnLoadServerMessage();

		SERVER_MESSAGE	aServerMessage[MAX_SERVER_MESSAGE_COUNT];

		int iCount = 0;
		iCount = g_pDb->OpenRecord("Select [Id], [Message] From ServerMessage ORDER by [Id]",
									aServerMessage,
									MAX_SERVER_MESSAGE_COUNT,
									(BYTE)TOTAL_DB);		
		if (0 < iCount)
		{
			// Alloc require memory and copy notice message data to the buffer.
			g_dwServerMessageCount = static_cast<DWORD>(iCount);
			
			// Exception process.
			g_pServerMessage = new SERVER_MESSAGE[g_dwServerMessageCount];
			if (NULL == g_pServerMessage)
			{
				throw false;
			}

			memset(g_pServerMessage, 0, sizeof(SERVER_MESSAGE) * g_dwServerMessageCount);

			// Actually copy data.
			memcpy(g_pServerMessage, aServerMessage, sizeof(SERVER_MESSAGE) * g_dwServerMessageCount);

			throw true;			
		}
		else
		{
			// Load failure process.
			g_dwServerMessageCount = 0;			
			throw false;
		}
	}
	catch (bool bResult)
	{
		return bResult;
	}
}


//----------------------------------------------------------------------------
// Name : GetServerMessage
// Description :
//----------------------------------------------------------------------------
char* GetServerMessage(int iCount)
{
	if (NULL == g_pServerMessage)
	{
		return NULL;
	}

	if (0 > iCount || g_dwServerMessageCount <= (unsigned long int)iCount)
	{
		return NULL;
	}

#ifdef _DEBUG
	if (MAX_SERVER_MESSAGE_LENGTH <= strlen(g_pServerMessage[iCount].szMessage))
	{
		Log(LOG_NORMAL, "array overwrite at GetServerMessage Function");
		return NULL;
	}
#endif

	return g_pServerMessage[iCount].szMessage;
}





