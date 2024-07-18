#pragma once
#include "BaseNetworkInterface.h"
#include "Define.h"

#define		MAX_SERVER_CONNECT		4
#define		SERVER_INDEX_AGENT		0
#define		SERVER_INDEX_WORLD		1	// 월드 서버.	항상 연결을 유지해야 함으로 따로 가지고 있는다.
#define		SERVER_INDEX_ZONE		2	// 마을과 던젼중 연결된것의 인덱스. 하나만 들어갈수 있음으로.

// #define		MAX_PACKET_NUM			256 //GameStatus별 최대 Packet수 

#define WORLD_SERVER_CONNECT_PORT	13200	
#define LOGIN_AGENT_CONNECT_PORT	13100


//#define		WM_SOCKET_EVENT			=	WM_USER + 0x7000;

#define SIZE_PACKET 4

class CNetworkClient : public ISC_BaseNetwork
{

	// 맴버 변수 절대 추가 금지
public:
	DWORD	GetServerIndex( BYTE bServerType );
	
	BOOL	SendMsg( char* pszMsg, DWORD dwLength, BYTE bServerType );
	
	DWORD	ConnectServer(char* szIP, WORD wPort, BYTE bServerType );
	void	DisconnectServer( BYTE bServerType );
	int	    MakeEncPaket( char * pszPacket, DWORD dwLength,  char * szReturn,int nPacketSize);
	int	    MakeDecPaket(char *pszPacket,int nPacketSize, char * szReturn);
	int     GetBlockCount(int nPacketSize);
	BOOL	isCheckSize(int nNotEncSize, DWORD dwSize);
};

// Network 관련 함수
void OnConnect(DWORD dwIndex);
void OnDisConnect(DWORD dwIndex);
void ReceivedMsg(DWORD dwIndex, char* pMsg, DWORD dwLen);
void InitPacketProc();

extern CNetworkClient*	g_pNet;
extern DWORD	g_dwConnectIndex[MAX_SERVER_CONNECT];
extern void	(*PacketProc[ MAX_UPDATE_GAME ][ MAX_PACKET_NUM ])( char* pMsg, DWORD dwLen );