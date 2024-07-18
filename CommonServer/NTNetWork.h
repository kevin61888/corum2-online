#pragma once

#include "CommonServer.h"
#include "IOleDBThread.h"
#include "INetWork.h"


// 패킷 암호화 g_pNet 구성 변경 
// 2005.01.05 김영대

#define SIZE_PACKET 4
class   DECLSPECIFIER CNTNetWork
{
public:
#pragma pack(push,1)
	struct SEncPacket
	{
		DWORD dwTotal;
		char * pszPacket;
	};
#pragma pack(pop)
public:
	BOOL			CreateNetwork(DESC_NETWORK* desc,DWORD dwUserAcceptInterval,DWORD dwServerAcceptInterval) ;
	void 			BreakMainThread() ;
	void			ResumeMainThread() ;

	void			SetUserInfo(DWORD dwConnectionIndex,void* user) ;
	void*			GetUserInfo(DWORD dwConnectionIndex) ;

	void			SetServerInfo(DWORD dwConnectionIndex,void* server);
	void*			GetServerInfo(DWORD dwConnectionIndex);

	sockaddr_in*	GetServerAddress(DWORD dwConnectionIndex);
	sockaddr_in*	GetUserAddress(DWORD dwConnectionIndex);
	BOOL			GetServerAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort);
	BOOL			GetUserAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort);
	BOOL			SendToServer(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag,BOOL bEnCrypt = TRUE);
	BOOL			SendToUser(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag,BOOL bEnCrypt = TRUE);
	void			CompulsiveDisconnectServer(DWORD dwConnectionIndex);
	void			CompulsiveDisconnectUser(DWORD dwConnectionIndex);
	int			GetServerMaxTransferRecvSize();
	int			GetServerMaxTransferSendSize();
	int			GetUserMaxTransferRecvSize();
	int			GetUserMaxTransferSendSize();
	void			BroadcastServer(char* pMsg,DWORD len,DWORD flag,BOOL bEnCrypt = TRUE);
	void			BroadcastUser(char* pMsg,DWORD len,DWORD flag,BOOL bEnCrypt = TRUE);
	DWORD			GetConnectedServerNum();
	DWORD			GetConnectedUserNum();
	WORD			GetBindedPortServerSide();
	WORD			GetBindedPortUserSide();
	BOOL			ConnectToServerWithUserSide(char* szIP,WORD port,CONNECTSUCCESSFUNC pSucc,CONNECTFAILFUNC pConn ,void* pExt) ;
	BOOL			ConnectToServerWithServerSide(char* szIP,WORD port,CONNECTSUCCESSFUNC pFunc ,CONNECTFAILFUNC pConn,void* pExt);	
	BOOL			StartServerWithUserSide(char* ip,WORD port);
	BOOL			StartServerWithServerSide(char* ip,WORD port);
	HANDLE			GetCustomEventHandle(DWORD index) ;
	BOOL			PauseTimer(DWORD dwCustomEventIndex) ;
	BOOL			ResumeTimer(DWORD dwCustomEventIndex);
	/*현재 사용하지 않는 함수
	/*일단 빼기로 함 2004.01.06 김영대
	BOOL			SendToServer(DWORD dwConnectionIndex,WSABUF* pBuf,DWORD dwNum,DWORD flag);	
	BOOL			SendToUser(DWORD dwConnectionIndex,WSABUF* pBuf,DWORD dwNum,DWORD flag);	
	BOOL			SendToServer(DWORD dwConnectionIndex,PACKET_LIST* pList,DWORD flag);
	BOOL			SendToUser(DWORD dwConnectionIndex,PACKET_LIST* pList,DWORD flag);
	*/

public:
	BOOL			isCheckSize(int nNotEncSize, DWORD dwSize);
	int				GetBlockCount(int nPacketSize);
	int				MakeEncPaket( char * pszPacket, DWORD dwLength,  char * szReturn,int nPacketSize);
	int				MakeDecPaket(char *pszPacket,int nPacketSize, char * szReturn);
	void Release();
	HRESULT CreateInstance();
	CNTNetWork(void);
	~CNTNetWork(void);
protected:
	I4DyuchiNET * m_pNetwork;
};


extern CNTNetWork * g_pNet;