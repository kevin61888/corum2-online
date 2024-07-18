#include "stdafx.h"
#include "linkedlist.h"
#include "User.h"
#include "UserTable.h"
#include "PacketEncrypt.h"

CUserTable	*g_pUserTable = NULL;	


CUserTable::CUserTable(DWORD dwNum)
{
	m_dwMaxBucketNum = dwNum;

	m_ppInfoTable = NULL;
	m_ppInfoTable = new CUser*[ dwNum ];		
	memset(m_ppInfoTable, 0, sizeof(CUser*)*dwNum);
	
	m_pUserPool = NULL;
	m_pUserPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pUserPool, sizeof(CUser), 5000, 5000);	
	m_dwNum = 0;	
	
	for (int i = 0; i < MAX_MONITORING_COUNT; i++)
	{
		a_dwMonitoringList[i] = 0xffffffff;
	}

	m_pUserList = new COnlyList(5000);
}


CUserTable::~CUserTable()
{
	RemoveUserTable();

	if(m_pUserList)
	{
		delete m_pUserList;
	}

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}

	if(m_pUserPool)
	{
		ReleaseStaticMemoryPool(m_pUserPool);
		m_pUserPool = NULL;
	}
}


CUser* CUserTable::AllocNewUser()
{
	CUser* pUser = (CUser*)LALAlloc(m_pUserPool);

	memset(pUser, 0, sizeof(CUser));
	pUser->m_bTryCreateCharSlot = 0xff;
	
	return pUser;
}


void CUserTable::FreeUser(CUser* pUser)
{
	LALFree(m_pUserPool, pUser);
}


BOOL CUserTable::Add(CUser* pUser)
{
	if(!pUser)	return FALSE;

	CUser* pAlreadyUser = GetUserInfo(pUser->GetPropID());
	if(pAlreadyUser)
	{
		Log(LOG_IMPORTANT, "User already added in usertable! (LoginID:%s)", pUser->m_szLoginID);
	}
			
	// 전체 유저 리스트에 추가 
	pUser->m_pListPos = m_pUserList->AddTail(pUser);

	DWORD dwIndex = pUser->GetPropID() % m_dwMaxBucketNum;		
	m_dwNum++;
	
	// m_ppInfoTable[dwIndex] 에 내용이 없으면 괄호 안으로.
	if (!m_ppInfoTable[dwIndex])	
	{
		m_ppInfoTable[dwIndex] = pUser;
		pUser->m_pNextData = NULL;
		pUser->m_pPrevData = NULL;
		return TRUE;
	}

	CUser* cur = m_ppInfoTable[dwIndex];
	CUser* prv = NULL;
	while (cur)
	{
		prv = cur;
		cur = cur->m_pNextData;
	}
	
	cur = prv->m_pNextData = pUser;
	cur->m_pPrevData = prv;
	cur->m_pNextData = NULL;

	return TRUE;
}


void CUserTable::Remove(DWORD dwConnectionIndex)
{
	CUser* pInfo = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pInfo)	return;

	Remove(pInfo);
}

void CUserTable::Remove(CUser* pInfo)
{
	if(!pInfo) return;

	DWORD dwIndex = pInfo->GetPropID()%m_dwMaxBucketNum;

	CUser* pAlreadyUser = GetUserInfo(pInfo->GetPropID());
	if(!pAlreadyUser)	 
	{
		// 없는데 지우라고 들어온 경우
		Log(LOG_IMPORTANT, "User already removed in usertable! (LoginID:%s)", pInfo->m_szLoginID);
		return;
	}
	
	if(pInfo->m_pListPos) 
	{
		m_pUserList->RemoveAt( pInfo->m_pListPos );	// 전체 유저리스트에서 삭제 
		pInfo->m_pListPos = NULL;
	}
	else
	{
		Log(LOG_IMPORTANT, "User list position is NULL!");
	}

	CUser* cur = m_ppInfoTable[dwIndex];
	CUser* next = NULL;
	CUser* prv = NULL;
	
	while (cur)
	{
		prv = cur->m_pPrevData;
		next = cur->m_pNextData;
		if (cur->GetPropID() == pInfo->GetPropID())
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->m_pNextData = next;
			
			if (next)
				next->m_pPrevData = prv;

			if(cur)	
			{
				if(cur->m_pItemMallBox)	delete [] cur->m_pItemMallBox;
				LALFree(m_pUserPool, cur);
			}
									
			cur = NULL;
			m_dwNum--;
			return;
		}

		cur = cur->m_pNextData;
	}
}

CUser* CUserTable::GetUserInfo(DWORD dwPropID)
{
	DWORD dwIndex = dwPropID%m_dwMaxBucketNum;

	CUser* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->GetPropID() == dwPropID)
		{
			return cur;
		}
		cur = cur->m_pNextData;
	}

	return NULL;
}


void CUserTable::RemoveUserTable()
{
	m_pUserList->RemoveAll();
	
	CUser* cur = NULL;
	CUser* next = NULL;
	
	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];
		while (cur)
		{
			next = cur->m_pNextData;

			if(cur->m_pItemMallBox)	delete [] cur->m_pItemMallBox;
			LALFree(m_pUserPool, cur);
			
			cur = next;
		}
		m_ppInfoTable[i] = NULL;
	}
}

void CUserTable::BroadCast(char* pPacket, DWORD dwLength)
{
	CUser* cur = NULL;
	CUser* next = NULL;
	
	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];
		
		while (cur)
		{
			next = cur->m_pNextData;
			SendToUser(cur, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
			cur = next;
		}
	}		
}

void CUserTable::BroadCastToMonitoringTool(char* pPacket, DWORD dwLength)
{
	for (int i = 0; i < MAX_MONITORING_COUNT; i++)
	{
		if (0xffffffff != a_dwMonitoringList[i])
		{
			CUser* pMonitoingUser = (CUser*)g_pNet->GetUserInfo(a_dwMonitoringList[i]);

			if(pMonitoingUser == NULL)
				continue;

			SendToUser(pMonitoingUser, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
		}		
	}	
}

bool CUserTable::AddMonitoringTool(DWORD dwConnectionIndex)
{
	for (int i = 0; i < MAX_MONITORING_COUNT; i++)
	{
		if (0xffffffff == a_dwMonitoringList[i])
		{
			a_dwMonitoringList[i] = dwConnectionIndex;
			break;
		}
	}
	
	return true;
}

bool CUserTable::RemoveMonitoringTool(DWORD dwConnectionIndex)
{
	for (int i = 0; i < MAX_MONITORING_COUNT; i++)
	{
		if (dwConnectionIndex == a_dwMonitoringList[i])
		{
			a_dwMonitoringList[i] = 0xffffffff;
			break;
		}
	}

	return true;
}

POSITION_ CUserTable::GetHeadPosition()
{
	return m_pUserList->GetHeadPosition();
}

CUser* CUserTable::GetNext(POSITION_& Position)
{
	return (CUser*)m_pUserList->GetNext(Position);
}