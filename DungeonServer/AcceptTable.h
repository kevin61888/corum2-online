#pragma once
#include "../4DyuchiGXGFunc/global.h"


struct ACCEPT_USER
{
	DWORD	dwDungeonID;
	DWORD	dwUserIndex;
	BOOL	bConnectToGameRoom;
	BYTE	bUserType;
	ACCEPT_USER*	pPrevData;
	ACCEPT_USER*	pNextData;
};


class CAcceptTable
{
	
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;			
	ACCEPT_USER**			m_ppInfoTable;

	STMPOOL_HANDLE			m_pAcceptPool;
	
	void					RemoveAcceptTable();

public:	
	
	ACCEPT_USER*			AllocNewAccept(DWORD dwDungeonID, DWORD dwUserIndex);
	ACCEPT_USER*			GetAcceptInfo(DWORD dwUserIndex);

	BOOL					Add(ACCEPT_USER* pUser);
	void					Remove(ACCEPT_USER* pUser);
	DWORD					GetCount()	{	return m_dwNum;	}

	CAcceptTable(DWORD dwNum);
	~CAcceptTable();
};


