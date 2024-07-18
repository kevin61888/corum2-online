#include "stdafx.h"
#include "WorldUserTable.h"
#include "WorldUser.h"
#include "NameSearchTable.h"
#include "linkedlist.h"
#include "OwnServer.h"
#include "recvmsg.h"


CWorldUserTable* g_pUserTable = NULL;


CWorldUserTable::CWorldUserTable( DWORD dwBucketNum, DWORD dwPoolSize )
{
	m_dwMaxBucketNum = dwBucketNum;

	m_ppInfoTable = NULL;
	m_ppInfoTable = new CWorldUser*[ dwBucketNum ];		
	memset(m_ppInfoTable, 0, sizeof(CWorldUser*)*dwBucketNum);

	dwPoolSize	= max( dwPoolSize, 4096 );
	
	m_pUserPool = NULL;
	m_pUserPool = CreateStaticMemoryPool();

	InitializeStaticMemoryPool(m_pUserPool, sizeof(CWorldUser), dwPoolSize, dwPoolSize);

	m_dwUserNum = 0;

	// 혹시모를 여분 1024
	m_pNameSearch		= new CNameSearchTable(dwPoolSize + 1024);
	m_pUserListInWorld	= new COnlyList(dwPoolSize + 1024);
	m_pTotalUserList	= new COnlyList(dwPoolSize + 1024);		
}

CWorldUserTable::~CWorldUserTable()
{
	RemoveWorldUserTable();

	if(m_pUserListInWorld)
	{
		delete m_pUserListInWorld;
		m_pUserListInWorld = NULL;
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
		
	if(m_pNameSearch)
	{
		delete m_pNameSearch;
		m_pNameSearch = NULL;
	}

	if(m_pTotalUserList)
	{
		delete m_pTotalUserList;
		m_pTotalUserList = NULL;
	}
}

CWorldUser* CWorldUserTable::AllocNewUser()
{
	CWorldUser* pUser = (CWorldUser*)LALAlloc(m_pUserPool);
	memset(pUser, 0, sizeof(CWorldUser));

	pUser->m_bCommand		= TRUE;
	pUser->m_bParty			= TRUE;
	pUser->m_dwCurServerPos	= POS_SERVER_CONNECT;
	return pUser;
}

BOOL CWorldUserTable::Add(CWorldUser* pUser)
{
	if(!pUser)	return FALSE;

	CWorldUser* pAlreadyUser = GetUserInfo(pUser->m_dwUserIndex);
	if(pAlreadyUser)
	{
		Remove(pAlreadyUser);
		Log(LOG_FATAL, "User Overlapped!! (CWorldUserTable::Add() Function! Name:%s)", pUser->m_szCharacterName);
	}

	// 전체 유저 리스트에 추가 
	pUser->m_ListTotalUser = m_pTotalUserList->AddTail(pUser);
	
	pUser->m_pListRefreshFromDungeon	= new CDugneonRefreshList;
	pUser->m_pListOwnDungeon			= new COwnDugneonList;

	DWORD dwIndex = pUser->m_dwUserIndex % m_dwMaxBucketNum;		
	m_dwUserNum++;
	
	if (pUser->m_pListRefreshFromDungeon)
	{
		char szTemp[0xff]={0,};
		wsprintf(szTemp, "<yakolla> %x, %d :: new m_pListRefreshFromDungeon", pUser, pUser->m_dwConnectionIndex);
		Log(LOG_JUST_FILEWRITE, szTemp);
	}

	if (!m_ppInfoTable[dwIndex])	//m_ppInfoTable[dwIndex] 에 내용이 없으면 괄호 안으로.
	{
		m_ppInfoTable[dwIndex] = pUser;
		pUser->m_pNextData = NULL;
		pUser->m_pPrevData = NULL;
		return TRUE;
	}

	CWorldUser* cur = m_ppInfoTable[dwIndex];
	CWorldUser* prv = NULL;

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

CWorldUser* CWorldUserTable::GetUserInfo(DWORD dwUserIndex)
{
	DWORD dwIndex = dwUserIndex%m_dwMaxBucketNum;

	CWorldUser* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->m_dwUserIndex == dwUserIndex)
		{
			return cur;
		}
		cur = cur->m_pNextData;
	}

	return NULL;
}


CWorldUser* CWorldUserTable::GetUserInfoByName(const char* szCharacterName)
{
	return m_pNameSearch->GetUserInfo(szCharacterName);
}


CWorldUser* CWorldUserTable::GetUserInfoByNameWithoutCase(const char* szCharacterName)
{
	return m_pNameSearch->GetUserInfoWithoutCase(szCharacterName);
}


void CWorldUserTable::Remove(DWORD dwConnectionIndex)
{
	CWorldUser* pInfo = (CWorldUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	Remove(pInfo);
}

void CWorldUserTable::Remove(CWorldUser* pInfo)
{
	//Name Hash Table에서 삭제 
	m_pNameSearch->Remove(pInfo->m_szCharacterName);	

	//전체 유저리스트에서 삭제 
	if(pInfo->m_ListTotalUser)
	{
		m_pTotalUserList->RemoveAt( pInfo->m_ListTotalUser );	
		pInfo->m_ListTotalUser = NULL;
	}

	DWORD dwIndex = pInfo->m_dwUserIndex%m_dwMaxBucketNum;

	CWorldUser* cur = m_ppInfoTable[dwIndex];
	CWorldUser* next = NULL;
	CWorldUser* prv = NULL;
	
	while (cur)
	{
		prv = cur->m_pPrevData;
		next = cur->m_pNextData;
		if (cur->m_dwUserIndex == pInfo->m_dwUserIndex)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->m_pNextData = next;
			
			if (next)
				next->m_pPrevData = prv;

			if(cur)	
			{
				RemoveUserResource(cur);				
			}
						
			cur = NULL;
			m_dwUserNum--;
			return;
		}

		cur = cur->m_pNextData;
	}
}


void CWorldUserTable::RemoveWorldUserTable()
{	
	m_pNameSearch->RemoveNameSearchTable();
	m_pTotalUserList->RemoveAll();

	CWorldUser* cur = NULL;
	CWorldUser* next = NULL;

	DWORD dwTotalUser = GetCount();
	DWORD dwCurNum = 0;

	for (DWORD i = 0; i < m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];

		while (cur)
		{
			next = cur->m_pNextData;

			dwCurNum++;
			SaveCharacterByBlockMode(cur);
			Log(LOG_JUST_DISPLAY, "Save User %d%%", (int)(((float)(dwCurNum/dwTotalUser*1.0))*100.0f) );
			RemoveUserResource(cur);
			
			cur = next;
		}

		m_ppInfoTable[i] = NULL;		
	}

	Log(LOG_JUST_DISPLAY, "Save User Complete!");
}

void CWorldUserTable::SendUserInWorld(char* pPacket, DWORD dwLength)
{
	POSITION_ pos = m_pUserListInWorld->GetHeadPosition();
	CWorldUser* pUser = NULL;

	while (pos)
	{
		pUser = (CWorldUser*)m_pUserListInWorld->GetNext(pos);
		if(!pUser)
		{
			Log(LOG_IMPORTANT, "pUser is NULL in CWorldUserTable::SendUserInWorld");
			continue;
		}

		g_pNet->SendToUser(pUser->m_dwConnectionIndex, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CWorldUserTable::BroadCast(char* pPacket, DWORD dwLength)
{		
	POSITION_ pos = m_pTotalUserList->GetHeadPosition();
	
	while(pos)
	{
		CWorldUser* pUser = (CWorldUser*)m_pTotalUserList->GetNext(pos);
		if(!pUser)
		{
			Log(LOG_IMPORTANT, "pUser is NULL in CWorldUserTable::BroadCast");
			continue;
		}

		g_pNet->SendToUser(pUser->m_dwConnectionIndex, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
	}
}

BOOL CWorldUserTable::AddCharacterToSearchTable(CWorldUser* pUser)
{
	// Name Hash Table에 추가 
	return m_pNameSearch->Add(pUser);			
}

void CWorldUserTable::RemoveUserResource(CWorldUser* pUser)
{
	if (pUser->m_pListRefreshFromDungeon)
	{
		pUser->m_pListRefreshFromDungeon->RemoveAll();
		delete pUser->m_pListRefreshFromDungeon;
		pUser->m_pListRefreshFromDungeon= NULL;
	}

	if (pUser->m_pListOwnDungeon)
	{
		pUser->m_pListOwnDungeon->RemoveAll();
		delete pUser->m_pListOwnDungeon;
		pUser->m_pListOwnDungeon = NULL;
	}

	LALFree(m_pUserPool, pUser);
}