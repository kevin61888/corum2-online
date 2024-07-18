#include "Stdafx.h"
#include "AcceptTable.h"


CAcceptTable* g_pAcceptTable = NULL;


CAcceptTable::CAcceptTable(DWORD dwBucketNum, DWORD dwPoolSize)
{
	memset(this, 0, sizeof(CAcceptTable));

	m_pAcceptPool = CreateStaticMemoryPool();

	dwPoolSize = max( dwPoolSize, 2048 );	// 풀 사이즈는 최소 2048 로 잡는다

	InitializeStaticMemoryPool( m_pAcceptPool, sizeof(ACCEPT_USER), dwPoolSize, dwPoolSize );

	m_dwMaxBucketNum = dwBucketNum;

	m_ppInfoTable = NULL;
	m_ppInfoTable = new ACCEPT_USER*[ dwBucketNum ];		
	memset(m_ppInfoTable, 0, sizeof(ACCEPT_USER*) * dwBucketNum);

	m_pAcceptedList = new COnlyList( dwPoolSize );

	m_dwUserNum = 0;
}

CAcceptTable::~CAcceptTable()
{
	RemoveAcceptTable();

	if(m_pAcceptedList)
	{
		delete m_pAcceptedList;
		m_pAcceptedList = NULL;
	}

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

//New로 메모리 할당해서 생성..      일번 던전일 경우 
ACCEPT_USER* CAcceptTable::AllocNewAccept()
{
	ACCEPT_USER* pUser = (ACCEPT_USER*)LALAlloc( m_pAcceptPool );
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "Allocation Failed in CAcceptTable::AllocNewAccept() !!!");
		return NULL;
	}

	memset(pUser, 0, sizeof(ACCEPT_USER));

	return pUser;
}

BOOL CAcceptTable::Add(ACCEPT_USER* pUser)
{
	if(!pUser)	return FALSE;
	
	DWORD dwIndex = pUser->dwChar_Index % m_dwMaxBucketNum;		
	m_dwUserNum++;

	if (!m_ppInfoTable[dwIndex])	//m_ppInfoTable[dwIndex] 에 내용이 없으면 괄호 안으로.
	{
		m_ppInfoTable[dwIndex] = pUser;
		pUser->pNextData = NULL;
		pUser->pPrevData = NULL;
		return TRUE;
	}

	ACCEPT_USER* cur = m_ppInfoTable[dwIndex];
	ACCEPT_USER* prv = NULL;
	int nTemp = 0;

	while (cur)
	{
		nTemp++;
		prv = cur;
		cur = cur->pNextData;
	}
	
	cur = prv->pNextData = pUser;
	cur->pPrevData = prv;
	cur->pNextData = NULL;

	Log(LOG_JUST_DISPLAY, "AcceptTable BucketNum: %d, Level: %d", dwIndex, nTemp);

	pUser->AcceptedListPos = m_pAcceptedList->AddTail(pUser);		//LinkList에 추가 

	return TRUE;
}

ACCEPT_USER* CAcceptTable::GetAcceptInfo(DWORD dwChar_Index)
{
	DWORD dwIndex = dwChar_Index%m_dwMaxBucketNum;

	ACCEPT_USER* cur = m_ppInfoTable[ dwIndex ];

	while (cur)
	{
		if (cur->dwChar_Index == dwChar_Index)
		{
			return cur;
		}
		cur = cur->pNextData;
	}

	return NULL;
}

void CAcceptTable::Remove(ACCEPT_USER* pUser)
{
	m_pAcceptedList->RemoveAt( pUser->AcceptedListPos );	
	pUser->AcceptedListPos = NULL;

	DWORD dwIndex = pUser->dwChar_Index%m_dwMaxBucketNum;

	ACCEPT_USER* cur = m_ppInfoTable[dwIndex];
	ACCEPT_USER* next = NULL;
	ACCEPT_USER* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrevData;
		next = cur->pNextData;
		if (cur->dwChar_Index == pUser->dwChar_Index)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNextData = next;
			
			if (next)
				next->pPrevData = prv;

			if(cur)	
			{
				LALFree( m_pAcceptPool, (void*)cur );
			}
										
			cur = NULL;
			m_dwUserNum--;
			return;

		}
		cur = cur->pNextData;
	}
}

void CAcceptTable::RemoveAcceptTable()
{
	m_pAcceptedList->RemoveAll();

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
