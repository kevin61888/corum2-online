#include "stdafx.h"
#include "ntnetwork.h"
#include "NTAes.h"
#include "network_guid.h"


CNTNetWork::CNTNetWork(void)
{

}

CNTNetWork::~CNTNetWork(void)
{

}


BOOL	CNTNetWork::CreateNetwork(DESC_NETWORK* desc,DWORD dwUserAcceptInterval,DWORD dwServerAcceptInterval)
{
	return m_pNetwork->CreateNetwork(desc,dwUserAcceptInterval,dwServerAcceptInterval);
}

void 	CNTNetWork::BreakMainThread() 
{
	m_pNetwork->BreakMainThread() ;
}

void	CNTNetWork::ResumeMainThread() 
{
	m_pNetwork->ResumeMainThread() ;
}

void	CNTNetWork::SetUserInfo(DWORD dwConnectionIndex,void* user)
{
	m_pNetwork->SetUserInfo(dwConnectionIndex,user);
}

void *	CNTNetWork::GetUserInfo(DWORD dwConnectionIndex)
{
	return m_pNetwork->GetUserInfo(dwConnectionIndex);
}

void	CNTNetWork::SetServerInfo(DWORD dwConnectionIndex,void* server)
{
	m_pNetwork->SetServerInfo(dwConnectionIndex,server);
}

void*	CNTNetWork::GetServerInfo(DWORD dwConnectionIndex)
{
	return m_pNetwork->GetServerInfo(dwConnectionIndex);
}

sockaddr_in* CNTNetWork::GetServerAddress(DWORD dwConnectionIndex)
{
	return m_pNetwork->GetServerAddress(dwConnectionIndex);
}

sockaddr_in* CNTNetWork::GetUserAddress(DWORD dwConnectionIndex)
{
	return m_pNetwork->GetUserAddress(dwConnectionIndex);
}

BOOL	CNTNetWork::GetServerAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort)
{
	return m_pNetwork->GetServerAddress(dwConnectionIndex,pIP,pwPort);
}

BOOL	CNTNetWork::GetUserAddress(DWORD dwConnectionIndex,char* pIP,WORD* pwPort)
{
	return m_pNetwork->GetUserAddress(dwConnectionIndex,pIP,pwPort);
}

BOOL	CNTNetWork::SendToServer(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag,BOOL bEnCrypt)
{

	return m_pNetwork->SendToServer(dwConnectionIndex,msg,length,flag);
/*
	if ( FALSE == bEnCrypt) 
		return m_pNetwork->SendToServer(dwConnectionIndex,msg,length,flag);

	int nPacketSize = 0;
	int nSize = 0;

	if ( length +SIZE_PACKET < BLOCK_LENGTH )
		nSize = BLOCK_LENGTH;
	else
		nSize = length  + SIZE_PACKET;

	//int nCheckCount = GetBlockCount(nSize);
	int nCheckCount = GetBlockCount(length);

	char * szTmp = new char[nCheckCount * BLOCK_LENGTH + SIZE_PACKET];
	memset(szTmp,0x00,nCheckCount * BLOCK_LENGTH + SIZE_PACKET);

	nPacketSize  = MakeEncPaket(msg,length,szTmp,nCheckCount);

	BOOL bRet = m_pNetwork->SendToServer(dwConnectionIndex,szTmp,nPacketSize ,flag);

	delete[] szTmp;

	return bRet;
	*/
}

BOOL	CNTNetWork::SendToUser(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag,BOOL bEnCrypt)
{
	return m_pNetwork->SendToUser(dwConnectionIndex,msg,length,flag);
/*
	if ( FALSE == bEnCrypt ) 
		return m_pNetwork->SendToUser(dwConnectionIndex,msg,length,flag);

	int nPacketSize = 0;
	int nSize = 0;

	if ( length +SIZE_PACKET < BLOCK_LENGTH )
		nSize = BLOCK_LENGTH;
	else
		nSize = length  + SIZE_PACKET;


	//int nCheckCount = GetBlockCount(nSize);
	int nCheckCount = GetBlockCount(length);

	char * szTmp = new char [nCheckCount * BLOCK_LENGTH + SIZE_PACKET ];
	memset(szTmp,0x00,nCheckCount * BLOCK_LENGTH + SIZE_PACKET);

	nPacketSize  = MakeEncPaket(msg,length,szTmp,nCheckCount);

	BOOL bRet = m_pNetwork->SendToUser(dwConnectionIndex,szTmp,nPacketSize,flag);

	delete[] szTmp;

	return bRet;
	*/
}

void	CNTNetWork::CompulsiveDisconnectServer(DWORD dwConnectionIndex)
{
	m_pNetwork->CompulsiveDisconnectServer(dwConnectionIndex);
}

void	CNTNetWork::CompulsiveDisconnectUser(DWORD dwConnectionIndex)
{
	m_pNetwork->CompulsiveDisconnectUser(dwConnectionIndex);
}

int		CNTNetWork::GetServerMaxTransferRecvSize()
{
	return m_pNetwork->GetServerMaxTransferRecvSize();
}

int		CNTNetWork::GetServerMaxTransferSendSize()
{
	return m_pNetwork->GetServerMaxTransferSendSize();
}

int		CNTNetWork::GetUserMaxTransferRecvSize()
{
	return m_pNetwork->GetUserMaxTransferRecvSize();
}

int		CNTNetWork::GetUserMaxTransferSendSize()
{
	return m_pNetwork->GetUserMaxTransferSendSize();
}

void	CNTNetWork::BroadcastServer(char* pMsg,DWORD len,DWORD flag,BOOL bEnCrypt)
{

	m_pNetwork->BroadcastServer(pMsg,len,flag);
/*
	if ( FALSE == bEnCrypt)
	{
		m_pNetwork->BroadcastServer(pMsg,len,flag);
		return;
	}
	int nSize = 0;
	if ( len +SIZE_PACKET < BLOCK_LENGTH )
		nSize = BLOCK_LENGTH;
	else
		nSize = len  + SIZE_PACKET;

	//int nCheckCount = GetBlockCount(nSize);
	int nCheckCount = GetBlockCount(len);

	char * szTmp = new char[nCheckCount * BLOCK_LENGTH + SIZE_PACKET];
	memset(szTmp,0x00,nCheckCount * BLOCK_LENGTH + SIZE_PACKET );

	int nPacketSize  = MakeEncPaket(pMsg,len,szTmp,nCheckCount);

	m_pNetwork->BroadcastServer(szTmp,nPacketSize,flag);

	delete[] szTmp;
	*/
}

void	CNTNetWork::BroadcastUser(char* pMsg,DWORD len,DWORD flag,BOOL bEnCrypt)
{

	m_pNetwork->BroadcastUser(pMsg,len,flag);
/*
	if ( FALSE == bEnCrypt)
	{
		m_pNetwork->BroadcastUser(pMsg,len,flag);
		return;
	}

	int nSize = 0;
	if ( len +SIZE_PACKET < BLOCK_LENGTH )
		nSize = BLOCK_LENGTH;
	else
		nSize = len  + SIZE_PACKET;

	//int nCheckCount = GetBlockCount(nSize);
	int nCheckCount = GetBlockCount(len);

	char * szTmp = new char [nCheckCount * BLOCK_LENGTH + SIZE_PACKET ];
	memset(szTmp,0x00,nCheckCount * BLOCK_LENGTH + SIZE_PACKET );

	int nPacketSize  = MakeEncPaket(pMsg,len,szTmp,nCheckCount);

	m_pNetwork->BroadcastUser(szTmp,nPacketSize,flag);

	delete[] szTmp;
*/
}

DWORD	CNTNetWork::GetConnectedServerNum()
{
	return m_pNetwork->GetConnectedServerNum();
}

DWORD	CNTNetWork::GetConnectedUserNum()
{
	return m_pNetwork->GetConnectedUserNum();
}


WORD	CNTNetWork::GetBindedPortServerSide()
{
	return m_pNetwork->GetBindedPortServerSide();
}

WORD	CNTNetWork::GetBindedPortUserSide() 
{
	return m_pNetwork->GetBindedPortUserSide() ;
}


BOOL	CNTNetWork::ConnectToServerWithUserSide(char* szIP,WORD port,CONNECTSUCCESSFUNC pSucc,CONNECTFAILFUNC pConn ,void* pExt)
{
	return m_pNetwork->ConnectToServerWithUserSide(szIP,port,pSucc,pConn,pExt);
}


BOOL	CNTNetWork::ConnectToServerWithServerSide(char* szIP,WORD port,CONNECTSUCCESSFUNC pFunc ,CONNECTFAILFUNC pConn,void* pExt)
{
	return m_pNetwork->ConnectToServerWithServerSide(szIP,port,pFunc,pConn,pExt);
}


BOOL	CNTNetWork::StartServerWithUserSide(char* ip,WORD port)
{
	return m_pNetwork->StartServerWithUserSide(ip,port);
}


BOOL	CNTNetWork::StartServerWithServerSide(char* ip,WORD port)
{
	return m_pNetwork->StartServerWithServerSide(ip,port);
}

HANDLE	CNTNetWork::GetCustomEventHandle(DWORD index)
{
	return m_pNetwork->GetCustomEventHandle(index);
}

BOOL	CNTNetWork::PauseTimer(DWORD dwCustomEventIndex)
{
	return m_pNetwork->PauseTimer(dwCustomEventIndex);
}


BOOL	CNTNetWork::ResumeTimer(DWORD dwCustomEventIndex)
{
	return m_pNetwork->ResumeTimer(dwCustomEventIndex);
}
/*
BOOL	CNTNetWork::SendToServer(DWORD dwConnectionIndex,WSABUF* pBuf,DWORD dwNum,DWORD flag)
{
	return m_pNetwork->SendToServer(dwConnectionIndex,pBuf,dwNum,flag);
}

BOOL	CNTNetWork::SendToUser(DWORD dwConnectionIndex,WSABUF* pBuf,DWORD dwNum,DWORD flag) 
{
	return m_pNetwork->SendToUser(dwConnectionIndex,pBuf,dwNum,flag) ;
}

BOOL	CNTNetWork::SendToServer(DWORD dwConnectionIndex,PACKET_LIST* pList,DWORD flag)
{
	return m_pNetwork->SendToServer(dwConnectionIndex,pList,flag);
}

BOOL	CNTNetWork::SendToUser(DWORD dwConnectionIndex,PACKET_LIST* pList,DWORD flag)
{
	SEncPacket RetPacket;
	memset(&RetPacket,0x00,sizeof(RetPacket));

	MakeEncPaket(pList,sizeof(* pList),&RetPacket);

	return m_pNetwork->SendToUser(dwConnectionIndex,pList,flag);
}
*/

HRESULT CNTNetWork::CreateInstance()
{
	return CoCreateInstance(CLSID_4DyuchiNET, NULL, CLSCTX_INPROC_SERVER, IID_4DyuchiNET, (void**)&m_pNetwork);
} 

void CNTNetWork::Release()
{
	m_pNetwork->Release();
	m_pNetwork = NULL;
}

int  CNTNetWork::GetBlockCount(int nPacketSize)
{
	int nRet = 0;
	if ( nPacketSize <  BLOCK_LENGTH )
		nRet = 1;
	else
	{
		nRet = (int)(nPacketSize / BLOCK_LENGTH); 
		if (  0  < (nPacketSize % BLOCK_LENGTH) ) nRet ++; 	
	}
	return nRet;
}


int  CNTNetWork::MakeEncPaket(char * pszPacket, DWORD dwLength,  char * szReturn,int nPacketSize)
{

	int nSize = nPacketSize;

	char    szBuf[KEY_LENGTH + 1];
	char    szDataOut[BLOCK_LENGTH + 1];
	memset(szBuf,0x00,KEY_LENGTH + 1);
	
	CNTAes m_AES;

	memcpy(szReturn,&dwLength,SIZE_PACKET);

	LoadString(GetModuleHandle("CommonServer.dll"), 1,szBuf,KEY_LENGTH+1); // 府家胶 贸府茄促.
	//strcpy(szBuf,"abcdefghijklmnopqrstuvwxyz123456");
	m_AES.MakeKey(szBuf, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", KEY_LENGTH, BLOCK_LENGTH);

	
	for (int i = 0 ; i < nSize ; i ++)
	{
		memset(szDataOut,0x00,BLOCK_LENGTH + 1);
			
		m_AES.EncryptBlock(pszPacket+(i * BLOCK_LENGTH), szDataOut);
	
		memcpy(szReturn+(i * BLOCK_LENGTH +SIZE_PACKET),szDataOut,BLOCK_LENGTH);
	}

	return (nSize * BLOCK_LENGTH)+SIZE_PACKET;
}


int	CNTNetWork::MakeDecPaket(char *pszPacket,int nPacketSize, char * szReturn)
{

	int nSize = GetBlockCount(nPacketSize);

	char    szBuf[KEY_LENGTH + 1];
	char    szDataOut[BLOCK_LENGTH + 1];

	memset(szBuf,0x00,KEY_LENGTH + 1);

	CNTAes m_AES;
	
	LoadString(GetModuleHandle("CommonServer.dll"), 1,szBuf,KEY_LENGTH+1); // 府家胶 贸府茄促.
	//strcpy(szBuf,"abcdefghijklmnopqrstuvwxyz123456");
	m_AES.MakeKey(szBuf, "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0", KEY_LENGTH, BLOCK_LENGTH);


	for (int i = 0 ; i < nSize ; i ++)
	{
		memset(szDataOut,0x00,BLOCK_LENGTH +1 );
			
		m_AES.DecryptBlock(pszPacket +(i * BLOCK_LENGTH), szDataOut);
	
		memcpy(szReturn+(i * BLOCK_LENGTH),szDataOut,BLOCK_LENGTH);
	}


	return (nSize * BLOCK_LENGTH);
}

BOOL CNTNetWork::isCheckSize(int nNotEncSize, DWORD dwSize)
{
	int nPacketSize = nNotEncSize;

	if ( dwSize ==  nPacketSize) return TRUE;
	while( dwSize > nPacketSize) nPacketSize += BLOCK_LENGTH;

	int nTempSize = nPacketSize - dwSize;

	nTempSize = BLOCK_LENGTH - nTempSize;

	return ( dwSize == (nTempSize + nNotEncSize) );
}
