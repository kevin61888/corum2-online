#ifndef __NAME_SEARCH_TABLE_H__
#define __NAME_SEARCH_TABLE_H__

#pragma once

#include "stdafx.h"
#include "../4DyuchiGXGFunc/global.h"
#include "WorldUser.h"

struct NameBlock
{
	CWorldUser* pUser;
	
	NameBlock*	pNextData;
	NameBlock*	pPrevData;
};

class CNameSearchTable
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;		
	NameBlock**				m_ppInfoTable;
	
	STMPOOL_HANDLE			m_pNameBlockPool;
				
	
	BOOL					CompareStr(const char* szStr1, const char* szStr2);
	BOOL					CompareStrWithoutCase(const char* szStr1, const char* szStr2);
		
public:								  
	void					RemoveNameSearchTable();

	CWorldUser*				GetUserInfo(const char* szCharacterName);
	CWorldUser*				GetUserInfoWithoutCase(const char* szCharacterName);
	BOOL					Add(CWorldUser* pUser);
	void					Remove(const char* szCharacterName);

	DWORD					GetCount()	{	return m_dwNum;		}
	
	CNameSearchTable(DWORD dwNum);
	~CNameSearchTable();

	// 문자열을 해쉬 키로 변환한다
	inline UINT BuildHashKey(char * szSearchName);
};

#define MIN( x, y )					( (x>y)?y:x )

#endif // __NAME_SEARCH_TABLE_H__