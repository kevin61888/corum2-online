#include "stdafx.h"
#include "NTOS.h"
#include "WorldUser.h"


CNetworkTimeObserverServer * g_pNtos = NULL;


CNetworkTimeObserverServer::CNetworkTimeObserverServer()
{
	Init();
}

CNetworkTimeObserverServer::~CNetworkTimeObserverServer()
{
}

void CNetworkTimeObserverServer::Init()
{
	m_dwStatus = 0;
	m_dwTimerNum = 0;
	m_dwMaxOverTick = 0;
	m_dwInterval = 0;
}

void CNetworkTimeObserverServer::DebugRemainInfo()
{
	char szBuf[256];

	if( !(m_dwStatus & NTOS_RUN) )
		return;

	wsprintf( szBuf, " <<  NTOS - Remained Timer Num :  %d  >> \n", m_dwTimerNum);
	OutputDebugString( szBuf);
}

void CNetworkTimeObserverServer::Initialize()
{
	DebugRemainInfo();
	Init();
	m_dwStatus = NTOS_INIT;
}

BOOL CNetworkTimeObserverServer::GetTimerNum( DWORD* pdwTimerNum)
{
	if( !(m_dwStatus & NTOS_INIT) )
		return FALSE;

	*pdwTimerNum = m_dwTimerNum;

	return TRUE;
}

BOOL CNetworkTimeObserverServer::GetMaxOverTick( DWORD* pdwMaxOverTick)
{
	if( !(m_dwStatus & NTOS_INIT) )
	{
		*pdwMaxOverTick = 0;
		return FALSE;
	}

	*pdwMaxOverTick = m_dwMaxOverTick;

	return TRUE;
}

BOOL CNetworkTimeObserverServer::SetMaxOverTick( DWORD dwMaxOverTick)
{
	if( !(m_dwStatus & NTOS_INIT) )
		return FALSE;

	m_dwMaxOverTick = dwMaxOverTick;

	return TRUE;
}

BOOL CNetworkTimeObserverServer::GetInterval( DWORD* pdwInterval)
{
	if( !(m_dwStatus & NTOS_INIT) )
	{
		*pdwInterval = 0;
		return FALSE;
	}

	*pdwInterval = m_dwInterval;

	return TRUE;
}

BOOL CNetworkTimeObserverServer::SetInterval( DWORD dwInterval)
{
	if( !(m_dwStatus & NTOS_INIT) )
		return FALSE;

	m_dwInterval = dwInterval;

	return TRUE;
}

BOOL CNetworkTimeObserverServer::Begin()
{
	if( m_dwStatus != NTOS_INIT )
		return FALSE;

	m_dwStatus |= NTOS_RUN;
	
	return TRUE;
}

BOOL CNetworkTimeObserverServer::End()
{
	if( !(m_dwStatus & NTOS_RUN) )
		return FALSE;

	DebugRemainInfo();
	m_dwTimerNum = 0;

	m_dwStatus &= (~NTOS_RUN);

	return TRUE;
}

BOOL CNetworkTimeObserverServer::RemoveTimer( NTOS_INFO* pNtos_info)
{
	if( !(m_dwStatus & NTOS_RUN)  ||  !(pNtos_info->bStatus & NTOS_INIT) )
		return FALSE;

	m_dwTimerNum--;

	return TRUE;
}

BOOL CNetworkTimeObserverServer::AddTimer( NTOS_INFO* pNtos_info)
{
	if( !(m_dwStatus & NTOS_RUN) )
		return FALSE;

	pNtos_info->bStatus = NTOS_INIT;
	m_dwTimerNum++;

	return TRUE;
}

BOOL CNetworkTimeObserverServer::Sync( NTOS_INFO* pNtos_info)
{
	if( !(m_dwStatus & NTOS_RUN)  ||  !(pNtos_info->bStatus & NTOS_INIT) )
		return FALSE;

	pNtos_info->dwSync = timeGetTime();
	
	return TRUE;
}

BOOL CNetworkTimeObserverServer::Ack_Check( NTOS_INFO* pNtos_info, DWORD dwTime, int* piOver)
{
	DWORD	dwAck;
	DWORD	dwHigh;
	DWORD	dwLow;
	float	fOver;


	*piOver = 0;

	if( !((m_dwStatus & pNtos_info->bStatus) & NTOS_RUN) )
		if( (m_dwStatus & NTOS_RUN)  &&  (pNtos_info->bStatus | NTOS_INIT) )
		{
			pNtos_info->bStatus |= NTOS_RUN;
			goto lb_sync;
		}
		else
			return FALSE;

	dwAck = timeGetTime();
	dwHigh = (dwAck - pNtos_info->dwSyncPrev);
	dwLow = (dwTime - pNtos_info->dwTime);
	fOver = ((float)dwLow / (float)dwHigh) * 100;
	*piOver = (int)fOver - 100;

lb_sync:
	pNtos_info->dwSyncPrev = pNtos_info->dwSync;
	pNtos_info->dwTime = dwTime;

	return TRUE;
}


#include "WorldUser.h"
#include "OwnServer.h"
#include "WorldUserTable.h"
#include "RecvMsg.h"

void DBLog_SpeedHackUser( CWorldUser* pUser, DWORD dwOver);
void KickSpeedHackUser( CWorldUser* pUser);


void TimerSync( CWorldUser* pUser)
{
	WSTC_TIMER_SYNC TimerSync;
	DWORD			dwInterval;

	g_pNtos->Sync( &pUser->m_NTOS);
	g_pNtos->GetInterval( &dwInterval);
	TimerSync.wIntervalSec = (WORD)dwInterval;
	g_pNet->SendToUser( pUser->m_dwConnectionIndex, (char*)&TimerSync, TimerSync.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void CmdTimerReady( CWorldUser* pUser, char* pMsg, DWORD dwLength )
{
	dwLength;pMsg;

	TimerSync( pUser);
}

void CmdTimerAck( CWorldUser* pUser, char* pMsg, DWORD dwLength )
{
	dwLength;

	CTWS_TIMER_ACK*	pPacket = (CTWS_TIMER_ACK*)pMsg;
	int		iOver;
	DWORD	dwMaxOver;
	BOOL	bR;

	bR = g_pNtos->Ack_Check( &pUser->m_NTOS, pPacket->dwTime, &iOver);
	if( !bR)	__asm int 3

	g_pNtos->GetMaxOverTick( &dwMaxOver);

	if( iOver > (int)dwMaxOver )
	{
		DBLog_SpeedHackUser( pUser, (DWORD) iOver);
		KickSpeedHackUser( pUser);
	}
}

void DBLog_SpeedHackUser(CWorldUser* pUser, DWORD dwOver)
{
	char szQuery[1024]={0,};
	BYTE bServerCode;

	bServerCode = (BYTE)atoi( g_pThis->GetServerSetCode());
	wsprintf( szQuery, "Insert into SpeedHackLog(Name, Serverset, Result) Values( '%s', %d, %d )", pUser->m_szCharacterName, bServerCode, dwOver);
	g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)TOTAL_DB);
}

void KickSpeedHackUser(CWorldUser* pUser)
{
	CWorldUser*	pKickUser = g_pUserTable->GetUserInfo( pUser->m_dwUserIndex);

	if( pKickUser )
	{
		Log(LOG_NORMAL, "SpeedHackUser Kicked! (Name:%s)", pKickUser->m_szCharacterName);
		pKickUser->m_bKicked = 1;	
		DWORD dwConnectionIndex = pKickUser->m_dwConnectionIndex;
		OnDisconnectUser( dwConnectionIndex);
		g_pNet->SetUserInfo( dwConnectionIndex, NULL);
		g_pNet->CompulsiveDisconnectUser( dwConnectionIndex);

		// 확실하게 Kick 해버리자. 
		WSTDS_KICK_USER	Kick;
		Kick.dwCharIndex = pKickUser->m_dwUserIndex;
		g_pServerTable->BroadCast( (char*)&Kick, Kick.GetPacketSize());
	}
}
