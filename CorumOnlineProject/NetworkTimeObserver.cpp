#include "NetworkTimeObserver.h"


CNetworkTimeObserver* g_pNto = NULL;


CNetworkTimeObserver::CNetworkTimeObserver()
{
	Init();
}

CNetworkTimeObserver::~CNetworkTimeObserver()
{
}

void CNetworkTimeObserver::Begin()
{
	Init();
}

void CNetworkTimeObserver::End()
{
	Init();
}

void CNetworkTimeObserver::SetReady( DWORD dwTime)
{
	m_dwTimerReady = dwTime;
}

BOOL CNetworkTimeObserver::CheckReady()
{
	BOOL bReady;
	DWORD	dwTime;

	dwTime = timeGetTime();
	bReady = ( dwTime > m_dwTimerReady);

	if( bReady )
		m_dwTimerReady = INIT_TIMER_READY;

	return bReady;
}

////////////////////////////////////////////////////////////


#include "NetworkClient.h"
#include "InitGame.h"


BOOL Create_NTO()
{
	g_pNto = new CNetworkTimeObserver;
	return ((BOOL)g_pNto);
}

void Delete_NTO()
{
	if( g_pNto )
	{
		delete g_pNto;
		g_pNto = NULL;
	}
}

void Begin_NTO()
{
	g_pNto->Begin();
}

void End_NTO()
{
	g_pNto->End();
}

//DWORD Over = 0;
//DWORD Inc = 0;

void CmdTimerSync( char* pMsg, DWORD dwLen )
{	
	WSTC_TIMER_SYNC* pPacket = (WSTC_TIMER_SYNC*)pMsg;
	CTWS_TIMER_ACK TimerAck;
	DWORD	dwTime;

//	Inc += 100;
//	Over += Inc;
	dwTime = timeGetTime();
	TimerAck.dwTime = dwTime; // + Over;
	g_pNet->SendMsg( (char*) &TimerAck, TimerAck.GetPacketSize(), SERVER_INDEX_WORLD);
	
	g_pNto->SetReady( dwTime + (pPacket->wIntervalSec * 1000));
	SetTimer( g_hMainWnd, NETWORK_TIME_OBSERVER_TIMER_ID, NETWORK_TIME_OBSERVER_INTERVAL, NTO_TimerFunc);
}

VOID CALLBACK NTO_TimerFunc( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	if( idEvent != NETWORK_TIME_OBSERVER_TIMER_ID )		_asm int 3

	if( g_pNto->CheckReady() )
	{
		CTWS_TIMER_READY	TimerReady;

		g_pNet->SendMsg( (char*) &TimerReady, TimerReady.GetPacketSize(), SERVER_INDEX_WORLD);
		KillTimer( hwnd, NETWORK_TIME_OBSERVER_TIMER_ID);
	}
}