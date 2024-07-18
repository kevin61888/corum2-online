#ifndef __NETWORK_TIME_OBSERVER_H__
#define __NETWORK_TIME_OBSERVER_H__


#pragma once


#include "stdafx.h"


#define NETWORK_TIME_OBSERVER_TIMER_ID		7
#define NETWORK_TIME_OBSERVER_INTERVAL		(1000 * 1)


#define INIT_TIMER_READY 0xFFFFFFFF


class CNetworkTimeObserver
{
	DWORD	m_dwTimerReady;


	void	Init()  { m_dwTimerReady = INIT_TIMER_READY; }

public:	
	CNetworkTimeObserver();
	~CNetworkTimeObserver();


	void	Begin();
	void	End();
	void	SetReady( DWORD dwTime);
	BOOL	CheckReady();
};

extern CNetworkTimeObserver* g_pNto;


////////////////////////////////////////////////////////////


BOOL Create_NTO();
void Delete_NTO();
void Begin_NTO();
void End_NTO();
void CmdTimerSync( char* pMsg, DWORD dwLen );
VOID CALLBACK NTO_TimerFunc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);


#endif // __NETWORK_TIME_OBSERVER_H__