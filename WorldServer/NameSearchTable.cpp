#include "stdafx.h"
#include "NameSearchTable.h"
#include "WorldUser.h"


CNameSearchTable::CNameSearchTable(DWORD dwNum)
{
	m_pNameBlockPool = CreateStaticMemoryPool();	
	InitializeStaticMemoryPool(m_pNameBlockPool, sizeof(NameBlock), 5000, 5000);

	m_dwMaxBucketNum = dwNum;

	m_ppInfoTable = new NameBlock*[ dwNum ];		
	memset(m_ppInfoTable, 0, sizeof(NameBlock*)*dwNum);

	m_dwNum = 0;
}

BOOL CNameSearchTable::Add(CWorldUser* pUser)
{
	if(!pUser)	
	{
		Log(LOG_IMPORTANT, "NULL Pointer Entered At CNameSearchTable::Add(CWorldUser* pUser)!");
		return FALSE;
	}

	NameBlock *pBlock = (NameBlock*)LALAlloc(m_pNameBlockPool);

	if(!pBlock)
	{	
		Log(LOG_IMPORTANT, "!! Warning Can't Alloc Memory CNameSearchTable::Add(CWorldUser* pUser)!");
		return FALSE;
	}
	pBlock->pUser = pUser;

	// 귓말 버그 수정 대소문자 버그 수정 소문자로 변환
	// 2005.01.30 김영대
	char  szTmp[MAX_CHARACTER_NAME_LENGTH ];
	memset(szTmp,0x00,MAX_CHARACTER_NAME_LENGTH );
	strncpy(szTmp,pUser->m_szCharacterName,MAX_CHARACTER_NAME_LENGTH );
	_strlwr(szTmp);


	DWORD dwIndex = BuildHashKey(szTmp) % m_dwMaxBucketNum ;
	//DWORD dwIndex = *((DWORD*)(szTmp)) % m_dwMaxBucketNum;		
	m_dwNum++;

	if (!m_ppInfoTable[dwIndex])	//m_ppInfoTable[dwIndex] 에 내용이 없으면 괄호 안으로.
	{
		m_ppInfoTable[dwIndex] = pBlock;
		pBlock->pNextData = NULL;
		pBlock->pPrevData = NULL;
		return TRUE;
	}

	NameBlock* cur = m_ppInfoTable[dwIndex];
	NameBlock* prv = NULL;

	while (cur)
	{
		prv = cur;
		cur = cur->pNextData;
	}
	
	cur = prv->pNextData = pBlock;
	cur->pPrevData = prv;
	cur->pNextData = NULL;

	return TRUE;
}

CWorldUser* CNameSearchTable::GetUserInfo(const char* szCharacterName)
{
	// 귓말 버그 수정 대소문자 버그 수정 Hash Key 키 구하 루틴 변경
	// 2005.01.30 김영대
	/*
	DWORD dwIndex = *((DWORD*)szCharacterName) % m_dwMaxBucketNum;

//	int k = strlen(szCharacterName);
	NameBlock* cur = m_ppInfoTable[dwIndex];

	

	while (cur)
	{
		// sung-han 2005-01-14 로그인한 캐릭터에게 보낸 귓말이 메모로 저장되는 버그 수정(영문 대소문자 구별 안하게)
		//--------------------------------------------------------------------------
		//if(CompareStr(szCharacterName, cur->pUser->m_szCharacterName))
		//	return cur->pUser;
		if(CompareStrWithoutCase(szCharacterName, cur->pUser->m_szCharacterName))
			return cur->pUser;
		//--------------------------------------------------------------------------

		cur = cur->pNextData;
	}

	*/

	// 귓말 버그 수정 대소문자 버그 수정 소문자로 변환후 한번더 검색한다.
	// 2005.01.30 김영대
	char  szTmp[MAX_CHARACTER_NAME_LENGTH ];
	memset(szTmp,0x00,MAX_CHARACTER_NAME_LENGTH );
	strncpy(szTmp,szCharacterName,MAX_CHARACTER_NAME_LENGTH );
	_strlwr(szTmp);
	//dwIndex = *((DWORD*)szTmp) % m_dwMaxBucketNum;
	DWORD dwIndex = BuildHashKey(szTmp) % m_dwMaxBucketNum ;
	NameBlock*  cur = m_ppInfoTable[dwIndex];
	while (cur)
	{
		// sung-han 2005-01-14 로그인한 캐릭터에게 보낸 귓말이 메모로 저장되는 버그 수정(영문 대소문자 구별 안하게)
		//--------------------------------------------------------------------------
		//if(CompareStr(szCharacterName, cur->pUser->m_szCharacterName))
		//	return cur->pUser;
		if(CompareStrWithoutCase(szCharacterName, cur->pUser->m_szCharacterName))
			return cur->pUser;
		//--------------------------------------------------------------------------

		cur = cur->pNextData;
	}

	return NULL;
}

CWorldUser* CNameSearchTable::GetUserInfoWithoutCase(const char* szCharacterName)
{
	//DWORD dwIndex = *((DWORD*)szCharacterName) % m_dwMaxBucketNum;
	char  szTmp[MAX_CHARACTER_NAME_LENGTH ];
	memset(szTmp,0x00,MAX_CHARACTER_NAME_LENGTH );
	strncpy(szTmp,szCharacterName,MAX_CHARACTER_NAME_LENGTH );
	_strlwr(szTmp);
	DWORD dwIndex = BuildHashKey(szTmp) % m_dwMaxBucketNum ;


//	int k = strlen(szCharacterName);
	NameBlock* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if(CompareStrWithoutCase(szCharacterName, cur->pUser->m_szCharacterName))
			return cur->pUser;

		cur = cur->pNextData;
	}
	return NULL;
}

void CNameSearchTable::Remove(const char* szCharacterName)
{	
	//DWORD dwIndex = *(DWORD*)szCharacterName%m_dwMaxBucketNum;
	char  szTmp[MAX_CHARACTER_NAME_LENGTH ];
	memset(szTmp,0x00,MAX_CHARACTER_NAME_LENGTH );
	strncpy(szTmp,szCharacterName,MAX_CHARACTER_NAME_LENGTH );
	_strlwr(szTmp);
	DWORD dwIndex = BuildHashKey(szTmp) % m_dwMaxBucketNum ;

	NameBlock* cur = m_ppInfoTable[dwIndex];
	NameBlock* next = NULL;
	NameBlock* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrevData;
		next = cur->pNextData;

		if (CompareStrWithoutCase(szCharacterName, cur->pUser->m_szCharacterName))
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNextData = next;
			
			if (next)
				next->pPrevData = prv;

			if(cur)	
				LALFree(m_pNameBlockPool, cur);
				//m_pNameBlockPool->Free((char*)cur);
							
			cur = NULL;
			m_dwNum--;

			return;

		}

		cur = cur->pNextData;
	}
}

CNameSearchTable::~CNameSearchTable()
{	
	RemoveNameSearchTable();

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}

	if(m_pNameBlockPool)
	{
		ReleaseStaticMemoryPool(m_pNameBlockPool);
		m_pNameBlockPool = NULL;
	}
}

void CNameSearchTable::RemoveNameSearchTable()
{		
	NameBlock* cur = NULL;
	NameBlock* next = NULL;

	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];

		while (cur)
		{
			next = cur->pNextData;
			LALFree(m_pNameBlockPool, cur);

			cur = next;
		}

		m_ppInfoTable[i] = NULL;
	}
}

BOOL CNameSearchTable::CompareStr(const char* szStr1, const char* szStr2)
{
	return (0 == __strcmp(szStr1, szStr2));
}

BOOL CNameSearchTable::CompareStrWithoutCase(const char* szStr1, const char* szStr2)
{
	return (0 == __stricmp(szStr1, szStr2));
}

inline UINT CNameSearchTable::BuildHashKey(char * szSearchName)
{
	UINT nHash = 0;
	if (szSearchName == NULL)
	{
//		assert(FALSE);
		return 0;
	}
	
	while (*szSearchName)
		nHash = (nHash<<5) + nHash + *szSearchName++;
	return nHash;
}
