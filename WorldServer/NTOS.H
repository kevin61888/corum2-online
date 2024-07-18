#ifndef __NETWORK_TIME_OBSERVER_SERVER_H__
#define __NETWORK_TIME_OBSERVER_SERVER_H__


#pragma once


#define MAX_OVERTICK	5	//	( 5 Percent )


class	CWorldUser;
struct	NTOS_INFO;


class CNetworkTimeObserverServer
{
	DWORD		m_dwStatus;
	DWORD		m_dwTimerNum;

	DWORD		m_dwMaxOverTick;
	DWORD		m_dwInterval;

	void		Init();
	void		DebugRemainInfo();

public:

	CNetworkTimeObserverServer();
	~CNetworkTimeObserverServer();

	void		Initialize();
	BOOL		GetTimerNum( DWORD* pdwTimerNum);
	BOOL		GetMaxOverTick( DWORD* pdwMaxOverTick);
	BOOL		SetMaxOverTick( DWORD dwMaxOverTick);
	BOOL		GetInterval( DWORD* pdwInterval);
	BOOL		SetInterval( DWORD dwInterval);
	BOOL		Begin();
	BOOL		End();
	BOOL		AddTimer( NTOS_INFO* pNtos_info);
	BOOL		RemoveTimer( NTOS_INFO* pNtos_info);
	BOOL		Sync( NTOS_INFO* pNtos_info);
	BOOL		Ack_Check( NTOS_INFO* pNtos_info, DWORD dwTime, int* piOver);
};


extern CNetworkTimeObserverServer* g_pNtos;


void TimerSync( CWorldUser* pUser);
void CmdTimerReady( CWorldUser* pUser, char* pMsg, DWORD dwLength );
void CmdTimerAck( CWorldUser* pUser, char* pMsg, DWORD dwLength );


#endif 


