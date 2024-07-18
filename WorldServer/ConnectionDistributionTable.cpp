#include "stdafx.h"
#include "ConnectionDistributionTable.h"
#include "WorldUserTable.h"
#include "WorldUser.h"
#include "linkedlist.h"
#include "recvmsg.h"


CConnectionDistributionTable * g_pCDTable = NULL;


CConnectionDistributionTable::CConnectionDistributionTable(DWORD dwMaxBucketNum, DWORD dwMaxConnectionPerBucket, DWORD dwAllocListNumPerOverFlow)
{
	memset(this, 0, sizeof(CConnectionDistributionTable));
	
	m_dwMaxBucketNum = dwMaxBucketNum;
	m_dwMaxConnectionPerBucket = dwMaxConnectionPerBucket;
	m_dwAllocListNumPerOverflow = dwAllocListNumPerOverFlow;
	m_ppBucketList = NULL;
	m_ppBucketList = new COnlyList*[ dwMaxBucketNum ];		
	memset(m_ppBucketList, 0, sizeof(COnlyList*)*dwMaxBucketNum);

	AllocAddedList();
}


CConnectionDistributionTable::~CConnectionDistributionTable()
{
	for(DWORD i = 0; i < m_dwCurBucketListCount; i++)
	{
		if(m_ppBucketList[i])
		{
			m_ppBucketList[i]->RemoveAll();
			delete m_ppBucketList[i];
			m_ppBucketList[i] = NULL;
		}
	}

	if (m_ppBucketList)
	{
		delete [] m_ppBucketList;
		m_ppBucketList = NULL;
	}
}


DWORD CConnectionDistributionTable::AllocAddedList()
{
	DWORD dwNewCurBucketID = 0xffffffff;

	for(DWORD i = 0; i < m_dwAllocListNumPerOverflow; i++)
	{
		m_ppBucketList[ m_dwCurBucketListCount + i ] = new COnlyList(m_dwMaxConnectionPerBucket);
		
		if(i == 0)
		{
			// 새로운 CurBucketID로 셋팅후 리턴  
			dwNewCurBucketID = m_dwCurBucketListCount;		
		}
	}

	m_dwCurBucketListCount += m_dwAllocListNumPerOverflow;

	return dwNewCurBucketID;
}

DWORD CConnectionDistributionTable::SetCurBucketList(DWORD dwCurID)
{

	if(dwCurID+1 >= m_dwCurBucketListCount)
		return 0xffffffff;

	for(DWORD i = dwCurID+1; i < m_dwCurBucketListCount; i++)
	{
		if(DWORD(m_ppBucketList[i]->GetCount()) < m_dwMaxConnectionPerBucket)
			return i;
	}

	return 0xffffffff;
}


BOOL CConnectionDistributionTable::Add(CWorldUser* pUser)
{
	if(!pUser)	return FALSE;

	pUser->m_dwCDTableBucketID = m_dwCurBucketID;
	pUser->m_ListPosCDTable = m_ppBucketList[ m_dwCurBucketID ]->AddTail( pUser );
	m_dwConnectionNum++;
		
	if( DWORD(m_ppBucketList[ m_dwCurBucketID ]->GetCount()) == m_dwMaxConnectionPerBucket )
	{
		// 현재 생성해놓은 리스트는 모두 꽉찬 상태인 경우 
		// 새로운 추가 List를 생성하고 새로운 CurBucketID로 셋팅한다. 
		DWORD dwIndex = SetCurBucketList(m_dwCurBucketID);

		if(dwIndex == 0xffffffff)	
		{
			m_dwCurBucketID = AllocAddedList();
		}
		else	 
		{
			// 아닌경우 기존의 빈 리스트로 셋팅
			m_dwCurBucketID = dwIndex;
		}
	}
	
	return TRUE;
}


DWORD CConnectionDistributionTable::SendKeepAlivePacket()
{
	ECHO_KEEP_ALIVE packet;

	POSITION_ pos = m_ppBucketList[ m_dwNextCheckBucketListID ]->GetHeadPosition();

	while(pos)
	{
		CWorldUser* pUser = (CWorldUser*)m_ppBucketList[ m_dwNextCheckBucketListID ]->GetNext(pos);

		if(!pUser)
		{
			Log(LOG_IMPORTANT, "pUser is NULL!! in CConnectionDistributionTable::SendKeepAlivePacket()");
			continue;
		}
		
		if(pUser->m_bIsMapLoading)	continue;	// 맵로딩 중이면 체크 대상에서 제외시킨다.
		if(pUser->m_bIsMonitor)		continue;	// 모니터링 유저는 체크대상에서 제외시킨다. 

		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		pUser->m_bKeepAliveCheckFlag	= 1;
		pUser->m_bCheckSended			= 1;	// 체크 패킷을 보낸 유저임을 확인 
		pUser->m_dwCheckSendedTick		= g_dwCurTick;
	}

	return m_dwCurBucketID;
}

void CConnectionDistributionTable::CheckKeepAlive()
{
	POSITION_ pos = m_ppBucketList[ m_dwNextCheckBucketListID ]->GetHeadPosition();

	while(pos)
	{
		CWorldUser* pUser = (CWorldUser*)m_ppBucketList[ m_dwNextCheckBucketListID ]->GetNext(pos);

		if(!pUser)
		{
			Log(LOG_IMPORTANT, "pUser is NULL!! in CConnectionDistributionTable::CheckKeepAlive()");
			continue;
		}

		// 체크 패킷을 보낸 유저만
		if(pUser->m_bCheckSended)	 
		{
			// 맵로딩이 아닌유저중 응답 메세지를 못받은 유저이면 끊어버려라 
			if(pUser->m_bKeepAliveCheckFlag && !pUser->m_bIsMapLoading)	
			{
				DWORD dwTick			= g_dwCurTick - pUser->m_dwCheckSendedTick;
				DWORD dwConnectionIndex = pUser->m_dwConnectionIndex;

				OnDisconnectUser(dwConnectionIndex);
				
				char szIp[32] = {0,};
				WORD wPort = 0;

				g_pNet->GetUserAddress(pUser->m_dwConnectionIndex, szIp, &wPort);
				szIp[15]='\0';

				g_pNet->SetUserInfo(dwConnectionIndex, NULL);
				g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);

				Log(LOG_NORMAL
					, "Unanswered User Disconnected! (Name:%s, IP:%s, Gap:%d)"
					, pUser->m_szCharacterName
					, szIp
					, dwTick );
			}

			pUser->m_bCheckSended = 0;
		}
	}
}


void CConnectionDistributionTable::SetNextList()
{
	m_dwNextCheckBucketListID++;
	
	if(!m_ppBucketList[m_dwNextCheckBucketListID])
	{
		m_dwNextCheckBucketListID = 0;
	}
}


BOOL CConnectionDistributionTable::Remove(CWorldUser* pUser)
{
	if(m_ppBucketList[ pUser->m_dwCDTableBucketID ] && pUser->m_ListPosCDTable)
	{
		m_ppBucketList[ pUser->m_dwCDTableBucketID ]->RemoveAt(pUser->m_ListPosCDTable);
		
		if(pUser->m_dwCDTableBucketID < m_dwCurBucketID)
			m_dwCurBucketID = pUser->m_dwCDTableBucketID;
		
		pUser->m_ListPosCDTable = NULL;
		pUser->m_dwCDTableBucketID = 0xffffffff;
		m_dwConnectionNum--;

		return TRUE;		
	}

	return FALSE;
}


