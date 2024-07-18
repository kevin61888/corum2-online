#include "stdafx.h"
#include "dungeontable.h"
#include "linkedlist.h"
#include "Dungeon.h"
#include "DungeonLayer.h"


CDungeonTable::CDungeonTable(DWORD dwNum)
{
	memset(this, 0, sizeof(CDungeonTable));

	m_dwMaxBucketNum = dwNum;

	m_ppInfoTable = NULL;
	m_ppInfoTable = new CDungeon*[ dwNum ];		
	memset(m_ppInfoTable, 0, sizeof(CDungeon*)*dwNum);

	m_vfptr.pActiveSectionList	= new CActiveSectionList;
	m_vfptr.pMonsterList		= new CMonsterList;
	m_vfptr.pPcList				= new CPcList;
	m_vfptr.pDungeonList		= new CDungeonList;
	m_vfptr.pCreateItemList		= new CCreateItemList;
	
	m_pDungeonList				= new CDungeonList;
	m_dwNum						= 0;
}


CDungeonTable::~CDungeonTable()
{
	RemoveDungeonTable();

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}

	if(m_vfptr.pActiveSectionList)
		delete m_vfptr.pActiveSectionList;

	if(m_vfptr.pMonsterList)
		delete m_vfptr.pMonsterList;

	if(m_vfptr.pPcList)
		delete m_vfptr.pPcList;

	if(m_vfptr.pDungeonList)
		delete m_vfptr.pDungeonList;
	
	if(m_vfptr.pCreateItemList)
		delete m_vfptr.pCreateItemList;

	if (m_pDungeonList)
	{
		delete m_pDungeonList;
		m_pDungeonList = NULL;
	}
}

CDungeon* CDungeonTable::CreateNewDungeon(WORD wID)
{
	CDungeon* pDungeon = new CDungeon;
	
	for(int i = 0; i < DUNGEON_TEMP_MAX; ++i)
	{
		pDungeon->SetTemp((DUNGEON_TEMP)i, 0);
	}

	CDungeonList*	pDun	= m_vfptr.pDungeonList;
	CDungeonList*	pDunCur = m_pDungeonList;

	__asm
	{
		mov		esi,dword ptr[pDun]
		mov		edi,dword ptr[pDunCur]
		movsd
	}

	pDungeon->SetDungeonID(wID);
	
	return pDungeon;
}

BOOL CDungeonTable::Add(CDungeon* pDungeon)
{
	if(!pDungeon)	
	{
		return FALSE;
	}
	
	pDungeon->SetDungeonListPos( m_pDungeonList->AddTail(pDungeon) );			

	DWORD dwIndex = pDungeon->GetID() % m_dwMaxBucketNum;		
	m_dwNum++;

	if (!m_ppInfoTable[dwIndex])	
	{
		m_ppInfoTable[dwIndex] = pDungeon;
		pDungeon->SetNextDungeon(NULL);
		pDungeon->SetPrevDungeon(NULL);
		return TRUE;
	}

	CDungeon* cur = m_ppInfoTable[dwIndex];
	CDungeon* prv = NULL;

	while (cur)
	{
		prv = cur;
		cur = cur->GetNextDungeon();
	}
	
	prv->SetNextDungeon(pDungeon);
	cur = pDungeon;
	cur->SetPrevDungeon(prv);
	cur->SetNextDungeon(NULL);

	return TRUE;
}


CDungeon* CDungeonTable::GetDungeonInfo(WORD wID)
{
	DWORD dwIndex = wID % m_dwMaxBucketNum;

	CDungeon* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->GetID() == wID)
		{
			return cur;
		}

		cur = cur->GetNextDungeon();
	}

	return NULL;
}


void CDungeonTable::Remove(CDungeon* pDungeon)
{
	m_pDungeonList->RemoveAt( pDungeon->GetDungeonListPos() );	

	DWORD dwIndex = pDungeon->GetID() % m_dwMaxBucketNum;

	CDungeon* cur = m_ppInfoTable[dwIndex];
	CDungeon* next = NULL;
	CDungeon* prv = NULL;
	
	while (cur)
	{
		prv = cur->GetPrevDungeon();
		next = cur->GetNextDungeon();
		
		if (cur->GetID() == pDungeon->GetID())
		{
			if (!prv)	
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->SetNextDungeon(next);
			
			if (next)
				next->SetPrevDungeon(prv);				
										
			cur = NULL;
			m_dwNum--;
			return;
		}

		cur = cur->GetNextDungeon();
	}

	return;
}


void CDungeonTable::RemoveDungeonTable()
{
	m_pDungeonList->RemoveAll();

	CDungeon* cur	= NULL;
	CDungeon* next	= NULL;

	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];

		while (cur)
		{
			next = cur->GetNextDungeon();

			cur->Destroy();
			delete cur;
						
			cur = next;
		}

		m_ppInfoTable[i] = NULL;
	}
}


void CDungeonTable::BroadCast(char* pPacket, DWORD dwLength)
{
	POSITION_ pos = m_pDungeonList->GetHeadPosition();

	CDungeon *pDungeon = NULL;

	while(pos)
	{
		pDungeon = (CDungeon*)m_pDungeonList->GetNext(pos);	
		pDungeon->BroadCast(pPacket, dwLength);
	}
}


void CDungeonTable::ShowAllDungeonStatus()
{
	POSITION_ pos = m_pDungeonList->GetHeadPosition();

	CDungeon *pDungeon=0;
	char szTemp[ 0xff ]={0,};
	char* szType[4] = { "Non-Occupied Dungeon", "Occupied Dungeon", "Village", "Passway"};

	Log( LOG_JUST_DISPLAY, "													               " );
	Log( LOG_JUST_DISPLAY, "-------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "                Dungeon Server - All Dungeon Status                " );
	Log( LOG_JUST_DISPLAY, "-------------------------------------------------------------------" );
		
	while(pos)
	{
		pDungeon = (CDungeon*)m_pDungeonList->GetNext(pos);	
		
		wsprintf(szTemp, "[%s] Type: %s, TotalLayer: %d, TotalUser: %d", pDungeon->GetDungeonName(), 
				szType[ pDungeon->GetDungeonKind()-1 ], pDungeon->GetTotalLayer(), pDungeon->GetDungeonTotalUserCount() );
		Log(LOG_JUST_DISPLAY, szTemp);

		for(BYTE i = 0; i < pDungeon->GetTotalLayer(); i++)
		{
			if(i == 0)
				wsprintf(szTemp, "Lobby     : %d User Connected...", pDungeon->GetDungeonLayer(i)->GetUserCount());
			else
				wsprintf(szTemp, "Layer(%02d) : %d User Connected...", i, pDungeon->GetDungeonLayer(i)->GetUserCount());
			
			Log(LOG_JUST_DISPLAY, szTemp);
		}

		Log(LOG_JUST_DISPLAY, "	");
	}	
}
