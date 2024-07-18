#include "Stdafx.h"
#include "AcceptTable.h"


CAcceptTable::CAcceptTable(DWORD dwNum)
{
	memset(this, 0, sizeof(CAcceptTable));

	m_pAcceptPool = CreateStaticMemoryPool();

	dwNum = max(1024, dwNum);

	InitializeStaticMemoryPool( m_pAcceptPool, sizeof(ACCEPT_USER), dwNum, dwNum );

	m_dwMaxBucketNum = dwNum;

	m_ppInfoTable = NULL;
	m_ppInfoTable = new ACCEPT_USER*[ dwNum ];		
	memset(m_ppInfoTable, 0, sizeof(ACCEPT_USER*)*dwNum);

	m_dwNum = 0;
}

ACCEPT_USER* CAcceptTable::AllocNewAccept(DWORD dwDungeonID, DWORD dwUserIndex)
{
	ACCEPT_USER* pUser = (ACCEPT_USER*)LALAlloc( m_pAcceptPool );
	memset(pUser, 0, sizeof(ACCEPT_USER));
	pUser->dwDungeonID = dwDungeonID;
	pUser->dwUserIndex = dwUserIndex;
		
	return pUser;
}

BOOL CAcceptTable::Add(ACCEPT_USER* pUser)
{
	if(!pUser)
		return FALSE;
	
	DWORD dwIndex = pUser->dwUserIndex % m_dwMaxBucketNum;		
	m_dwNum++;

	// m_ppInfoTable[dwIndex] 에 내용이 없으면 괄호 안으로.
	if (!m_ppInfoTable[dwIndex])	
	{
		m_ppInfoTable[dwIndex] = pUser;
		pUser->pNextData = NULL;
		pUser->pPrevData = NULL;
		return TRUE;
	}

	ACCEPT_USER* cur = m_ppInfoTable[dwIndex];
	ACCEPT_USER* prv = NULL;
	while (cur)
	{
		prv = cur;
		cur = cur->pNextData;
	}
	
	cur = prv->pNextData = pUser;
	cur->pPrevData = prv;
	cur->pNextData = NULL;

	return TRUE;
}

ACCEPT_USER* CAcceptTable::GetAcceptInfo(DWORD dwUserIndex)
{
	DWORD dwIndex = dwUserIndex % m_dwMaxBucketNum;
	ACCEPT_USER* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->dwUserIndex == dwUserIndex)
		{
			return cur;
		}

		cur = cur->pNextData;
	}

	return NULL;
}

void CAcceptTable::Remove(ACCEPT_USER* pUser)
{
	DWORD dwIndex = pUser->dwUserIndex % m_dwMaxBucketNum;

	ACCEPT_USER* cur = m_ppInfoTable[dwIndex];
	ACCEPT_USER* next = NULL;
	ACCEPT_USER* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrevData;
		next = cur->pNextData;

		if (cur->dwUserIndex == pUser->dwUserIndex)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNextData = next;
			
			if (next)
				next->pPrevData = prv;

			if(cur)	
				LALFree( m_pAcceptPool, (void*)cur );
										
			cur = NULL;
			m_dwNum--;
			return;
		}

		cur = cur->pNextData;
	}
}

CAcceptTable::~CAcceptTable()
{
	RemoveAcceptTable();

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}

	if(m_pAcceptPool)
	{
		ReleaseStaticMemoryPool( m_pAcceptPool );
		m_pAcceptPool = NULL;
	}
}

void CAcceptTable::RemoveAcceptTable()
{
	ACCEPT_USER* cur = NULL;
	ACCEPT_USER* next = NULL;

	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];
		while (cur)
		{
			next = cur->pNextData;
			LALFree( m_pAcceptPool, (void*)cur );
			cur = next;
		}

		m_ppInfoTable[i] = NULL;
	}
}
