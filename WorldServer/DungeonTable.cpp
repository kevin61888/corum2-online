#include "stdafx.h"
#include "DungeonTable.h"

CDungeonTable * g_pDungeonTable = NULL;

CDungeonTable::CDungeonTable(DWORD dwNum)
{
	m_dwMaxBucketNum = dwNum; 

	m_ppInfoTable = NULL;
	m_ppInfoTable = new DUNGEON_DATA_EX*[dwNum];		
	memset(m_ppInfoTable, 0, sizeof(DUNGEON_DATA_EX*) * dwNum);

	m_pDungeonList		= new COnlyList(MAX_DUNGEON_NUM_PER_SERVERSET);		
	m_pDungeonVillList	= new COnlyList(MAX_DUNGEON_NUM_PER_SERVERSET);		

	m_dwNum = 0;
}

CDungeonTable::~CDungeonTable()
{	
	RemoveDungeonTable();

	if(m_pDungeonList)
	{
		delete m_pDungeonList;
		m_pDungeonList = NULL;
	}	

	if(m_pDungeonVillList)
	{
		delete m_pDungeonVillList;
		m_pDungeonVillList = NULL;
	}

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}
}

DUNGEON_DATA_EX* CDungeonTable::AllocNewDungeon(WORD wID)
{
	if(GetDungeonInfo(wID))	//던전 ID 중복 
		__asm int 3

	DUNGEON_DATA_EX* pDungeon = new DUNGEON_DATA_EX;
		
	pDungeon->SetDungeonType(wID);
	pDungeon->m_dwID = (WORD)wID;
	
	return pDungeon;
}

BOOL CDungeonTable::Add(DUNGEON_DATA_EX* pDungeon)
{
	if (!pDungeon)	
	{
		return FALSE;
	}

	//if(pDungeon->dwID > 4000 && pDungeon->pServer)	//점령던전 이면  
	if (pDungeon->IsConquer())	//점령던전 이면			//개발용..  실제는 윗줄로 갱신 
	{
		pDungeon->m_pListPos = m_pDungeonList->AddTail(pDungeon);
	}
	else
	{
		pDungeon->m_pListPos = m_pDungeonVillList->AddTail(pDungeon);		
	}

	DWORD dwIndex = pDungeon->m_dwID % m_dwMaxBucketNum;	
	m_dwNum++;

	if (!m_ppInfoTable[dwIndex])	//m_ppInfoTable[dwIndex] 에 내용이 없으면 괄호 안으로.
	{
		m_ppInfoTable[dwIndex] = pDungeon;
		pDungeon->m_pNextData = NULL;
		pDungeon->m_pPrevData = NULL;
		return TRUE;
	}

	DUNGEON_DATA_EX* cur = m_ppInfoTable[dwIndex];
	DUNGEON_DATA_EX* prv = NULL;

	while (cur)
	{
		prv = cur;
		cur = cur->m_pNextData;
	}
	
	cur = prv->m_pNextData = pDungeon;
	cur->m_pPrevData = prv;
	cur->m_pNextData = NULL;

	return TRUE;
}

DUNGEON_DATA_EX* CDungeonTable::GetDungeonInfo(DWORD wID)
{
	DWORD dwIndex = wID%m_dwMaxBucketNum;

	DUNGEON_DATA_EX* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if ((WORD)cur->m_dwID == wID)
		{
			return cur;
		}
		cur = cur->m_pNextData;
	}

	return NULL;
}

void CDungeonTable::Remove(DUNGEON_DATA_EX* pInfo)
{
	DWORD dwIndex = pInfo->m_dwID%m_dwMaxBucketNum;
	
	if(pInfo->IsConquer() )	//점령던전 이면 
	{
		m_pDungeonList->RemoveAt(pInfo->m_pListPos);
	}	
	else
	{
		m_pDungeonVillList->RemoveAt(pInfo->m_pListPos);		
	}

	DUNGEON_DATA_EX* cur = m_ppInfoTable[dwIndex];
	DUNGEON_DATA_EX* next = NULL;
	DUNGEON_DATA_EX* prv = NULL;
	
	while (cur)
	{
		prv = cur->m_pPrevData;
		next = cur->m_pNextData;

		if (cur->m_dwID == pInfo->m_dwID)
		{
			if (!prv)	
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->m_pNextData = next;
			
			if (next)
				next->m_pPrevData = prv;

			if(cur)	
				delete cur;
						
			cur = NULL;
			m_dwNum--;
			return;
		}

		cur = cur->m_pNextData;
	}
}

void CDungeonTable::RemoveDungeonTable()
{
	m_pDungeonList->RemoveAll();
	m_pDungeonVillList->RemoveAll();
	
	DUNGEON_DATA_EX* cur = NULL;
	DUNGEON_DATA_EX* next = NULL;

	for (DWORD i = 0; i < m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];

		while (cur)
		{
			next = cur->m_pNextData;
			delete cur;
			cur = next;
		}
		
		m_ppInfoTable[i] = NULL;
	}
}