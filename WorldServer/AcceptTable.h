#ifndef __ACCEPT_TABLE_H__
#define __ACCEPT_TABLE_H__


#pragma once


#include "../4DyuchiGXGFunc/global.h"
#include "OnlyList.h"
#include "WorldUser.h"


struct ACCEPT_USER
{
	DWORD			dwSerialCode;		//Agent의 시리얼 코드 
	DWORD			dwChar_Index;		//선택한 Character의 DB상의 Char_Index
	DWORD			dwAcceptedTick;		//Accept 된 시점의 TickCount
	POSITION_		AcceptedListPos;	//LinkList Position

	BILLING_INFO	BillingInfo;		//결제 정보 

	BYTE			bBillingType;		//결제 타입 
	DWORD			dwRemainTime;		//남은 시간 
	DBTIMESTAMP		BillingEday;		//결제 끝나는시각 
	DWORD			dwConnection;
	DWORD			dwPropID;
	BYTE			bEventFlag;
	ACCEPT_USER*	pPrevData;
	ACCEPT_USER*	pNextData;
};


class CAcceptTable
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwUserNum;			
	ACCEPT_USER**			m_ppInfoTable;
	STMPOOL_HANDLE			m_pAcceptPool;	

	void					RemoveAcceptTable();

public:	

	COnlyList				*m_pAcceptedList;
	
	ACCEPT_USER*			AllocNewAccept();
	ACCEPT_USER*			GetAcceptInfo(DWORD dwChar_Index);
	BOOL					Add(ACCEPT_USER* pUser);
	void					Remove(ACCEPT_USER* pUser);
	DWORD					GetCount()	{	return m_dwUserNum;	}

	CAcceptTable(DWORD dwBucketNum, DWORD dwPoolSize);
	~CAcceptTable();
};


extern CAcceptTable*	g_pAcceptTable;

#endif 