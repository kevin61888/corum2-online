#ifndef __CONNECTION_DISTRIBUTION_TABLE_H__
#define __CONNECTION_DISTRIBUTION_TABLE_H__


#pragma once


#include "stdafx.h"
#include "OnlyList.h"
#include "../4DyuchiGXGFunc/global.h"
#include "WorldUser.h"


class CConnectionDistributionTable
{	
	DWORD					m_dwConnectionNum;				//현재 수용하고 있는 Connection 갯수 
	DWORD					m_dwMaxConnectionPerBucket;		//한 버켓이 가질수 있는 최대 Connection 갯수 
	DWORD					m_dwAllocListNumPerOverflow;	//한 버켓의 리스트가 꽉 찰때마다 Alloc할 List의 갯수  	
	DWORD					m_dwCurBucketListCount;			//현재 생성된 BucketList Count
	DWORD					m_dwCurBucketID;				//현재 가리키고 있는 Bucket 넘버 

public:								  

	DWORD					m_dwNextCheckBucketListID;		//다음번 KeepAlive Check해야할 List ID
	DWORD					m_dwMaxBucketNum;	
	COnlyList**				m_ppBucketList;

	DWORD AllocAddedList();
	DWORD SetCurBucketList(DWORD dwCurID);
	BOOL  Add(CWorldUser* pUser);
	BOOL  Remove(CWorldUser* pUser);
	DWORD SendKeepAlivePacket();
	void  CheckKeepAlive();
	void  SetNextList();
	
	CConnectionDistributionTable(DWORD dwMaxBucketNum, DWORD dwMaxConnectionPerBucket, DWORD dwAllocListNumPerOverFlow);
	~CConnectionDistributionTable();
};


extern CConnectionDistributionTable* g_pCDTable;


#endif 