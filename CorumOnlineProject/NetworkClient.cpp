#include "NetworkClient.h"
#include "LoginProcess.h"
#include "CharSelectProcess.h"
#include "WorldProcess.h"
#include "DungeonProcess.h"
#include "DungeonMsg.h"
#include "ErrorWnd.h"
#include "Interface.h"

#include "Chat.h"
#include "message.h"
#include "GameControl.h"
#include "User.h"
#include "InitGame.h"
#include "ClientTrafficLog.h"
#include "NetworkTimeObserver.h"
#include "PacketEncrypt.h"
#include "../CommonServer/NTAes.h"

DWORD	g_dwConnectIndex[MAX_SERVER_CONNECT];
void	(*PacketProc[ MAX_UPDATE_GAME ][ MAX_PACKET_NUM ])( char* pMsg, DWORD dwLen );

void InitPacketProc()
{
	memset(PacketProc, 0, sizeof(PacketProc));

	InitPacketProcLogin();
	InitPacketProcCharSelect();
	InitPacketProcWorld();
	InitPacketProcDungeon();
}

BOOL CNetworkClient::SendMsg( char* pszMsg, DWORD dwLength, BYTE bServerType )
{
#ifdef __TRAFFIC_LOG
	SendMsgLog(pszMsg, dwLength);
#endif

	return Send( g_dwConnectIndex[bServerType], pszMsg, dwLength );
}
/*
{
	BOOL bRet = Send( g_dwConnectIndex[bServerType], pszMsg, dwLength );
	return bRet;

#ifdef __TRAFFIC_LOG
	SendMsgLog(pszMsg, dwLength);
#endif

	int nPacketSize = 0;
	int nSize = 0;

	if ( dwLength +SIZE_PACKET < BLOCK_LENGTH )
		nSize = BLOCK_LENGTH;
	else
		nSize = dwLength  + SIZE_PACKET;

	//int nCheckCount = GetBlockCount(nSize);
	int nCheckCount = GetBlockCount(dwLength);
	char * szTmp = new char[nCheckCount * BLOCK_LENGTH + SIZE_PACKET];
	memset(szTmp,0x00,nCheckCount * BLOCK_LENGTH + SIZE_PACKET);

	nPacketSize  = MakeEncPaket(pszMsg,dwLength,szTmp,nCheckCount);

	BOOL bRet = Send( g_dwConnectIndex[bServerType], szTmp, nPacketSize );

	delete[] szTmp;

	return bRet;
}
*/

DWORD CNetworkClient::GetServerIndex( BYTE bServerType )
{
	if( bServerType >= MAX_SERVER_CONNECT ) return 0;
		
	return g_dwConnectIndex[bServerType];
}

DWORD CNetworkClient::ConnectServer( char* szIP, WORD wPort, BYTE bServerType )
{
	g_dwConnectIndex[bServerType] = ConnectToServer( szIP, wPort );
	return g_dwConnectIndex[bServerType];
}

void CNetworkClient::DisconnectServer( BYTE bServerType )
{
	if( !g_dwConnectIndex[bServerType] ) return;
	//CloseConnection( GetServerIndex(bServerType) );	
	CompulsiveDisconnect( GetServerIndex(bServerType) );

	g_dwConnectIndex[bServerType] = 0;	
}


int  CNetworkClient::GetBlockCount(int nPacketSize)
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

int  CNetworkClient::MakeEncPaket(char * pszPacket, DWORD dwLength,  char * szReturn,int nPacketSize)
{

	int nSize = nPacketSize;

	char    szBuf[KEY_LENGTH + 1];
	char    szDataOut[BLOCK_LENGTH + 1];
	memset(szBuf,0x00,KEY_LENGTH + 1);
	
	CNTAes m_AES;

	memcpy(szReturn,&dwLength,SIZE_PACKET);

	LoadString(GetModuleHandle("CommonServer.dll"), 1,szBuf,KEY_LENGTH+1); // 리소스 처리한다.
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


int	CNetworkClient::MakeDecPaket(char *pszPacket,int nPacketSize, char * szReturn)
{

	int nSize = GetBlockCount(nPacketSize);

	char    szBuf[KEY_LENGTH + 1];
	char    szDataOut[BLOCK_LENGTH + 1];

	memset(szBuf,0x00,KEY_LENGTH + 1);

	CNTAes m_AES;
	
	LoadString(GetModuleHandle("CommonServer.dll"), 1,szBuf,KEY_LENGTH+1); // 리소스 처리한다.
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

BOOL CNetworkClient::isCheckSize(int nNotEncSize, DWORD dwSize)
{
	int nPacketSize = nNotEncSize;

	if ( dwSize ==  nPacketSize) return TRUE;
	while( dwSize > nPacketSize) nPacketSize += BLOCK_LENGTH;

	int nTempSize = nPacketSize - dwSize;

	nTempSize = BLOCK_LENGTH - nTempSize;

	return ( dwSize == (nTempSize + nNotEncSize) );
}


//-------------------------------------------------------------------------//

void OnConnect(DWORD dwIndex)
{

}

void OnDisConnect(DWORD dwIndex)
{
	if (g_pMainPlayer)
	{
		g_pMainPlayer->m_bMatching = 0;

	}

	if(g_pGVLogin)	g_pGVLogin->bIsLoging = FALSE;
	if(dwIndex == g_dwConnectIndex[SERVER_INDEX_WORLD])
	{
		//	deathrain - Add 2004. 7. 14
		End_NTO();
		//

		if(GetGameStatus()==UPDATE_GAME_PLAY)
		{
			CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();
			// "월드서버와의 접속이 끊어졌습니다.", "5초 후에 종료하겠습니다."
			pErrorWnd->SetError(g_Message[ETC_MESSAGE342].szMessage, g_Message[ETC_MESSAGE594].szMessage , "", 2);
			pErrorWnd->SetActive(TRUE);
			pErrorWnd->m_byType		= 1;
			pErrorWnd->m_byTime		= 5;
			pErrorWnd->m_dwCurTime	= timeGetTime();
			pErrorWnd->m_dwPevTime	= timeGetTime();
			CInterface::GetInstance()->SetWnd(ERROR_WND);			
		}
		else if(GetGameStatus()==UPDATE_GAME_WORLD)
		{
			CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();
			// "월드서버와의 접속이 끊어졌습니다.", "5초 후에 종료하겠습니다."
			pErrorWnd->SetError(g_Message[ETC_MESSAGE342].szMessage, g_Message[ETC_MESSAGE594].szMessage , "", 2);
			pErrorWnd->SetActive(TRUE);
			pErrorWnd->m_byType		= 1;
			pErrorWnd->m_byTime		= 5;
			pErrorWnd->m_dwCurTime	= timeGetTime();
			pErrorWnd->m_dwPevTime	= timeGetTime();
			CInterface::GetInstance()->SetWnd(ERROR_WND);
		}
		//MessageBox(g_hMainWnd, g_Message[ETC_MESSAGE342].szMessage, g_Message[ETC_MESSAGE343].szMessage, MB_OK);	// MSG_ID : 342 ; 월드서버와의 접속이 끊어졌습니다. ,343 ;Error
		g_dwConnectIndex[SERVER_INDEX_WORLD] = 0;		
	}
	else if(dwIndex == g_dwConnectIndex[SERVER_INDEX_ZONE])
	{
		if(GetGameStatus()==UPDATE_GAME_PLAY)
		{
			CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();
			//"던전서버와의 접속이 끊어졌습니다.",  "5초 후에 종료하겠습니다."
			pErrorWnd->SetError(g_Message[ETC_MESSAGE611].szMessage, g_Message[ETC_MESSAGE594].szMessage,  "", 2);
			pErrorWnd->SetActive(TRUE);
			pErrorWnd->m_byType		= 1;
			pErrorWnd->m_byTime		= 5;
			pErrorWnd->m_dwCurTime	= timeGetTime();
			pErrorWnd->m_dwPevTime	= timeGetTime();
			CInterface::GetInstance()->SetWnd(ERROR_WND);			
		}	
		else if(GetGameStatus()==UPDATE_GAME_WORLD)
		{
			CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();
			//"던전서버와의 접속이 끊어졌습니다.",  "5초 후에 종료하겠습니다."
			pErrorWnd->SetError(g_Message[ETC_MESSAGE611].szMessage, g_Message[ETC_MESSAGE594].szMessage,  "", 2);		
			pErrorWnd->SetActive(TRUE);
			pErrorWnd->m_byType		= 1;
			pErrorWnd->m_dwCurTime	= timeGetTime();
			pErrorWnd->m_dwPevTime	= timeGetTime();
			pErrorWnd->m_byTime		= 5;
			CInterface::GetInstance()->SetWnd(ERROR_WND);			
		}
		//MessageBox(NULL, "던전서버와의 접속이 끊어졌습니다.", "Error", MB_OK);
		g_dwConnectIndex[SERVER_INDEX_ZONE] = 0;
//		g_bIsRunning = FALSE;
	}
	else if(dwIndex == g_dwConnectIndex[SERVER_INDEX_AGENT])
	{
		g_dwConnectIndex[SERVER_INDEX_AGENT] = 0;
	//	g_bIsRunning = FALSE;
	}	
}

void ReceivedMsg (DWORD dwIndex, char* pMsg, DWORD dwLen)
{

	int nSize = 0;
/*
	memcpy(&nSize,pMsg,SIZE_PACKET);

	if ( ! g_pNet->isCheckSize( nSize , dwLen - SIZE_PACKET) )
	{
		char szMsg[0xff] = {0,};
		wsprintf(szMsg, "Illegal Packet or Hacked Packet !");
		MessageBox(g_hMainWnd, szMsg, NULL, NULL);
		return ;
	}

	char * szPacket =  new char[dwLen+1];
	memset(szPacket,0x00,dwLen +1);
	g_pNet->MakeDecPaket(pMsg + SIZE_PACKET,(dwLen -SIZE_PACKET) ,szPacket);
*/
	char * szPacket =   pMsg;
	nSize = dwLen;

#ifdef __TRAFFIC_LOG
	RecvMsgLog(szPacket, nSize);
#endif // __TRAFFIC_LOG

	BYTE bStatus = *((BYTE*)szPacket);
	BYTE bHeader = *((BYTE*)szPacket+1);

	if(PacketProc[ bStatus ][ bHeader ])
	{
		
#ifdef __PACKET_ENCRYPTION

		// 로그인 서버와 통신은 암호화.....
		// 월드에서 날라오는 패킷인데 UPDATE_GAME_CHAR_SELECT 인것이 있다.. -_-;;;
		if(	bStatus == UPDATE_GAME_LOGIN || 
			(bStatus == UPDATE_GAME_CHAR_SELECT && bHeader != Protocol_CharSelect::CMD_WORLD_USER_INFO))
		{
			char cEncryptedMsg[4096];
			WORD wSize = 0;

			DecryptReceivedMsgFromAgent(szPacket, nSize, cEncryptedMsg, &wSize);

			(*PacketProc[ bStatus ][ bHeader ])(cEncryptedMsg, wSize);
		}
		else
		{
			(*PacketProc[ bStatus ][ bHeader ])(szPacket, nSize);
		}

#else

//		LOGOUT( "bStatus : %d, bHeader : %d", bStatus, bHeader );
		(*PacketProc[ bStatus ][ bHeader ])(szPacket, nSize);

#endif // __PACKET_ENCRYPTION//		LOGOUT( "bStatus : %d, bHeader : %d", bStatus, bHeader );
		
	}
	else
	{
		char szMsg[0xff] = {0,};
		wsprintf(szMsg, "Unknown packet received!(bStatus:%d bHeader:%d)", bStatus, bHeader);
		MessageBox(g_hMainWnd, szMsg, NULL, NULL);
		//delete[] szPacket;
		return;
	}
	//delete[] szPacket;
}