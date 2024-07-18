#include "stdafx.h"
#include "recvmsg.h"
#include "User.h"
#include "OwnServer.h"
#include "WorldServerTable.h"
#include "UserTable.h"
#include "CPSSOLib.h"
#include "MsgProc.h"
#include "MsgProcForServer.h"
#include "DBProc.h"
#include "DBProcItemMall.h"
#include "ThrowQuery.h"
#include "ChinaBillingProc.h"
#include "LicensingProc.h"


extern BOOL			g_bIsServerRunning;
extern COwnServer*	g_pThis;
extern MACHINENAME	g_MachineName;


void	(*PacketProc[ MAX_UPDATE_GAME ][ MAX_PACKET_NUM ])(CUser* pUser ,char* pMsg,DWORD dwLength);


void InitializePacketProc()
{
	memset(PacketProc, 0, sizeof(PacketProc));

	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_LOGIN ]							= CmdLoginOverlapped;		
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_REQUEST_SERVER_ALIVE ]				= CmdRequestServerAlive;
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_CMTOOL_NOTICE]						= CmdCMToolNotice;
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_IS_ALIVE_USER]						= CmdIsAliveUser;
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_CM_KICK_USER]						= CmdCMKickUser;
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_CHECK_MONITORING ]					= CmdCheckMonitoring;
	PacketProc[ UPDATE_GAME_LOGIN ][ Protocol_Login::CMD_LOGIN_NETMARBLE]					= CmdLoginOverlapped;	

	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_CHARACTER_SELECT ]		= CmdCharacterSelect;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_CREATE_NEW_CHARACTER ]	= CmdCreateNewCharacter;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_DELETE_CHARACTER ]		= CmdDeleteCharacter;
	PacketProc[ UPDATE_GAME_CHAR_SELECT ][ Protocol_CharSelect::CMD_CHECK_GM ]			    = CmdCheckGM;			
}


void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
	if(!g_pThis->IsUserAcceptAllowed())
	{
		g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
	}
}


void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	dwConnectionIndex;
}

void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	if(!g_bIsServerRunning)	return;

#ifdef USE_LICENSING_SERVER
	if(g_pThis->GetLicensingServerConnectionIndex() == dwConnectionIndex)
	{
		OnDisconnectLicensingServer();
		return;
	}
#endif

	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 : 050104 hwoarang
	{
		if(g_pThis->GetBillingServerConnectionIndex() == dwConnectionIndex)
		{
			OnDisconnectBillingServer();
			return;
		}
	}
	
	WORLD_SERVER_DATA* pServer = (WORLD_SERVER_DATA*)g_pNet->GetServerInfo(dwConnectionIndex);

	if(pServer)
	{
		Log(LOG_FATAL, "WorldServer Disconnected! (WorldMapID=%d)", pServer->dwWorldMapID);
		pServer->dwServerStatus = SERVER_STATUS_NOT_CONNECTED;
		pServer->dwConnectionIndex = 0;
		g_pNet->SetServerInfo(dwConnectionIndex, NULL);
		
		if( ! IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE 아니면 : 050104 Hwoarang
		{
			Log(LOG_IMPORTANT,"OnDisconnectServer() : SetEvent(g_pThis->m_hKeyEvent[7])");
			SetEvent(g_pThis->m_hKeyEvent[7]);	
		}
		
	}
	else
	{
		char szIP[16]={0,};	WORD wPort=0;
		g_pNet->GetServerAddress(dwConnectionIndex, szIP, &wPort);

		Log(LOG_NORMAL, "Unattached Server Disconnected! (IP=%s)", szIP);
	}
}

void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{
	if(!g_bIsServerRunning)	return;
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	if(pUser)
	{
		// 모니터링 툴의 로그아웃 처리.
		if( TRUE == pUser->m_bIsMonitor )	
		{
			LogoutMonitoringTool(pUser);
			return;
		}
		
		// 일반 유저의 로그아웃 처리. 아직 월드에 로그인 되지 않은 상태 
		if(pUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)		
		{
			if(pUser->m_dwStatus != 0)	
			{
				// 로그인 프로시저에서 성공한 유저만 
				ThrowLogoutQuery(pUser);

				Log(LOG_JUST_DISPLAY, "User Logout...  (ID:%s, Name:%s)", pUser->m_szLoginID, pUser->GetSelectedCharacter()->szName);
				g_pUserTable->Remove(pUser);
			}						
		}
		else
		{			
			Log(LOG_JUST_DISPLAY, "User Connection Freed! (ID:%s,Name:%s,CurWorldMapID:%d)", pUser->m_szLoginID, pUser->GetSelectedCharacter()->szName, pUser->m_dwCurWorldServerPos);
		}
	}
	else
	{
		Log(LOG_JUST_DISPLAY, "Unlogin User Disconnected...");
	}
	
	g_pNet->SetUserInfo(dwConnectionIndex, NULL);
}


void LogoutMonitoringTool(CUser* pUser)
{
	ThrowLogoutQuery(pUser);

	g_pUserTable->RemoveMonitoringTool(pUser->GetConnectionIndex());
	g_pUserTable->Remove(pUser);

	Log(LOG_JUST_DISPLAY, "Monitoring tool logout..");	
}


void __stdcall OnRecvFromServerTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	if(!g_bIsServerRunning)	return;

#ifdef USE_LICENSING_SERVER
	if(g_pThis->GetLicensingServerConnectionIndex() == dwConnectionIndex)
	{
		OnRecvFromLicensingServerTCP(pMsg, dwLength);
		return;
	}
#endif

	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 : 050104 hwoarang
	{
		if(g_pThis->GetBillingServerConnectionIndex() == dwConnectionIndex)
		{
			OnRecvFromBillingServerTCP(pMsg, dwLength);
			return;
		} // if(g_pThis->GetBillingServerConnectionIndex() == dwConnectionIndex)
	} // if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 : 050104 hwoarang

	int nSize = 0;
	/*
	memcpy(&nSize,pMsg,SIZE_PACKET);

	if ( ! g_pNet->isCheckSize( nSize , dwLength - SIZE_PACKET) )
	{
		Log(LOG_IMPORTANT, "Illegal Packet or Hacked Packet !, %d , %d ",nSize , dwLength);
		return ;
	}

	char * szPacket =  new char[dwLength+1];
	memset(szPacket,0x00,dwLength +1);
	g_pNet->MakeDecPaket(pMsg + SIZE_PACKET,(dwLength -SIZE_PACKET) ,szPacket);
	*/

	char * szPacket  = pMsg;
	nSize  = 	dwLength;

	WORD wHeader;
	memcpy(&wHeader, szPacket, 2);	

	WORLD_SERVER_DATA* pServer = (WORLD_SERVER_DATA*)g_pNet->GetServerInfo(dwConnectionIndex);
	//Login 하지 않은 서버로부터의 패킷 처리 
	if(!pServer)
	{
		if(wHeader == Protocol_Agent_Server::ACMD_SERVER_ATTACH)
			CmdServerAttach(dwConnectionIndex, szPacket, nSize);
		else
		{
			char szIp[ 16 ]={0,}; WORD wPort=0;
			g_pNet->GetServerAddress(dwConnectionIndex, szIp, &wPort);
			Log(LOG_IMPORTANT, "Unknown packet received by ServerSide!(%s)", szIp);
		}
	}

	switch(wHeader)
	{
		case Protocol_Agent_Server::ACMD_USER_ACCEPT_ALLOWED:		AcmdUserAcceptAllowed(pServer, szPacket, nSize);			break;
		case Protocol_Agent_Server::ACMD_USER_ACCEPT_FAILED:		AcmdUserAcceptFailed(pServer, szPacket, nSize);			break;
		case Protocol_Agent_Server::ACMD_USER_WORLD_LOGIN_SUCCESS:	AcmdUserWorldLoginSuccess(pServer, szPacket, nSize);		break;
		case Protocol_Agent_Server::ACMD_WORLD_USER_DISCONNECT:		AcmdWorldUserDisconnect(pServer, szPacket, nSize);		break;
		case Protocol_Agent_Server::ACMD_SERVER_ALIVE_INFO:			AcmdServerAliveInfo(pServer, szPacket, nSize);			break;
		case Protocol_Agent_Server::ACMD_REQUEST_CHINA_BILLING:		AcmdRequestChinaBilling(pServer, szPacket, nSize);		break;
		case Protocol_Agent_Server::ACMD_REQUEST_CHINA_BILLINGINFO:	AcmdRequestChinaBillingInfo(pServer, szPacket, nSize);	break;
	}

	//delete[] szPacket;
}


void __stdcall OnRecvFromUserTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	BYTE bStatus = 0;
	BYTE bHeader = 0;

	try
	{
		if(!g_bIsServerRunning)	return;

		try
		{
			if(!dwLength)	
			{
				g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
				return;
			}
		}
		catch(...)
		{
			throw "Exception from g_pNet->CompulsiveDiscon";
		}

		int nSize = 0;
		/*
		memcpy(&nSize,pMsg,SIZE_PACKET);
		if ( ! g_pNet->isCheckSize( nSize , dwLength - SIZE_PACKET) )
		{
			Log(LOG_IMPORTANT, "Illegal Packet or Hacked Packet !, %d , %d " , nSize , dwLength);
			return ;
		}

		char * szPacket =  new char[dwLength];
		memset(szPacket,0x00,dwLength);
		g_pNet->MakeDecPaket(pMsg + SIZE_PACKET,(dwLength - SIZE_PACKET),szPacket);
		*/

		char * szPacket  = pMsg;
		nSize  = 	dwLength;

		bStatus = *((BYTE*)(szPacket));
		bHeader = *((BYTE*)(szPacket+1));
		CUser *pUser = NULL;
		
		try
		{
			 pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		}
		catch(...)
		{
			throw "Exception from g_pNet->GetUserInfo";
		}
		//Login 하지 않은 유저로부터의 패킷 처리 
		if(!pUser)
		{
			bool bLoginCheck = false;

			if( IS_ABLE_SERVICE_TYPE( ST_NETMARBLE ) )
			{
				if(bStatus == UPDATE_GAME_LOGIN && bHeader == Protocol_Login::CMD_LOGIN_NETMARBLE)
				{
					bLoginCheck = true;
				}
			}
			else
			{
				if(bStatus == UPDATE_GAME_LOGIN && bHeader == Protocol_Login::CMD_LOGIN)
				{
					bLoginCheck = true;
				}
			}

			if(bLoginCheck)
			{
	#ifdef __PACKET_ENCRYPTION
				if(dwLength > 2560 || ((dwLength-3) % 8)) // 최대를 512*5로 잡는다. 이걸 넘을리는 없다!!
				{
					try
					{
						char szIP[32]={0,};
						WORD wPort =0;
						
						g_pNet->GetUserAddress(dwConnectionIndex, szIP, &wPort);

						Log(LOG_IMPORTANT, "It might be Hacked Packet!, Length: %u, IP: %s", dwLength, szIP);
						//delete[] szPacket;
						return;
					}
					catch(...)
					{
						throw "Exception from g_pNet->GetUserAddress";
					}
				}

				try
				{
					char cDecrpptBuffer[4096] = {0,};
					WORD wSize = 0;
					cDecrpptBuffer[0] = pMsg[0];
					cDecrpptBuffer[1] = pMsg[1];

					DecryptReceivedMsgWithBaseKey(szPacket, nSize, cDecrpptBuffer, &wSize);
					if(wSize <= 2)
					{
						Log(LOG_IMPORTANT, "Illegal Decrypted Packet Size!");
						//delete[] szPacket;
						return;
					}
				}
				catch(...)
				{
					throw "Exception from DecryptReceivedMsgWithBaseKey";
				}
				
				try
				{
					CmdLogin(dwConnectionIndex, cDecrpptBuffer, wSize);
				}
				catch(...)
				{
					throw "Exception from CmdLogin";
				}
		#else
				try
				{
					CmdLogin(dwConnectionIndex, szPacket, nSize);
				}
				catch(...)
				{
					throw "Exception from CmdLogin";
				}
		#endif
				

			}
			else
			{
				try
				{
					char szIp[ 16 ]={0,}; WORD wPort=0;
					g_pNet->GetUserAddress(dwConnectionIndex, szIp, &wPort);
					Log(LOG_IMPORTANT, "Unknown packet received!(%s)", szIp);
					g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
				}
				catch(...)
				{
					throw "Exception from g_pNet->GetUserAddress";
				}
			}
			//delete[] szPacket;
			return;
		}
		
		if(PacketProc[ bStatus ][ bHeader ])
		{
	#ifdef __PACKET_ENCRYPTION
			// 최대를 512*5로 잡는다. 이걸 넘을리는 없다!! 패킷 길이는 항상 8n+3!!
			if(dwLength > 2560 || ((dwLength-3) % 8)) 
			{
				try
				{
					char szIP[32]	= {0,};
					WORD wPort		= 0;
					g_pNet->GetUserAddress(dwConnectionIndex, szIP, &wPort);
					Log(LOG_IMPORTANT, "It might be Hacked Packet!, Length: %u, IP: %s", dwLength, szIP);
					//delete[] szPacket;
					return;
				}
				catch(...)
				{
					throw "Exception from g_pNet->GetUserAddress";
				}
			}

			try
			{
				CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
			}
			catch(...)
			{
				throw "Exception from g_pNet->GetUserInfo";
			}

			try
			{

				char cDecrpptBuffer[4096]	= {0,};
				WORD wSize					= 0;
				cDecrpptBuffer[0]			= pMsg[0];
				cDecrpptBuffer[1]			= pMsg[1];

				DecryptReceivedMsg(pUser, szPacket, nSize, cDecrpptBuffer, &wSize);
			}
			catch(...)
			{
				throw "Exception from DecryptReceivedMsg";
			}

			if(wSize <= 2)
			{
				Log(LOG_IMPORTANT, "Illegal Decrypted Packet Size!");
				//delete[] szPacket;
				return;
			}

			try
			{
				(*PacketProc[ bStatus ][ bHeader ])(pUser, cDecrpptBuffer, wSize);
			}
			catch(...)
			{
				throw "Exception from PacketProc";
			}
	#else
			try
			{
				(*PacketProc[ bStatus ][ bHeader ])(pUser, szPacket, nSize);
			}
			catch(...)
			{
				throw "Exception from PacketProc";
			}

	#endif

		}
		else
		{
			try
			{
				char szIp[ 16 ]={0,}; WORD wPort=0;
				g_pNet->GetUserAddress(dwConnectionIndex, szIp, &wPort);
				Log(LOG_IMPORTANT, "Unknown packet received!(ID:%s, IP:%s)", pUser->m_szLoginID, szIp);
				g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
				//delete[] szPacket;
				return;
			}	
			catch(...)
			{
				throw "Exception from g_pNet->GetUserInfo";
			}
		}
		//delete[] szPacket;
	}
	catch(char *msg)
	{
		Log(LOG_IMPORTANT, "%s, %d, %d", msg, bStatus, bHeader);
	}
	catch(...)
	{
		Log(LOG_IMPORTANT, "Unhandled Exception!!");
	}
}

void ReceivedFromDBBill(DBRECEIVEDATA* pResult)
{
	switch(pResult->bQueryType)
	{
		case QUERY_TYPE_SELECT:			
		{
			if(!pResult->Query.select.pResult)
			{
				Log(LOG_IMPORTANT, "pResult->Query.select.pResult is NULL! (dwRowSize:%d)", pResult->Query.select.dwRowSize);
				return;
			}

			switch(pResult->dwQueryUID)
			{
				case QUERY_TYPE_BILLING_CHECK:		QueryTypeBillingCheck(pResult);		break;						
			}
		}
		break;
	}
}


void ReceivedFromDB(DBRECEIVEDATA* pResult)
{		
	if(!g_bIsServerRunning)	return;
	
	switch(pResult->bQueryType)
	{
		case QUERY_TYPE_SELECT:			// THOpenRecord 
		{
			if(!pResult->Query.select.pResult)
			{
				Log(LOG_IMPORTANT, "pResult->Query.select.pResult is NULL! (dwRowSize:%d)", pResult->Query.select.dwRowSize);
				return;
			}

			switch(pResult->dwQueryUID)
			{
				case QUERY_TYPE_LOGIN:						QueryTypeLogin(pResult);					break;
				case QUERY_TYPE_CHR_SELECT_INFO:			QueryTypeChrSelectInfo(pResult);			break;
				case QUERY_TYPE_CREATE_CHAR_INIT_BINARY:	QueryTypeCreateCharInitBinary(pResult);		break;
				case QUERY_TYPE_CREATE_NEW_CHAR:			QueryTypeCreateNewChar(pResult);			break;								
				case QUERY_TYPE_GM_CHECK:					QueryTypeGMCheck(pResult);					break;
				case QUERY_TYPE_CHECK_MONITOR:				QueryTypeCheckMonitoring(pResult);			break;
				case QUERY_TYPE_IS_ALIVE_USER:				QueryTypeIsAliveUser(pResult);				break;	
				case QUERY_TYPE_DELETE_CHARACTER:			QueryTypeDeleteCharacter(pResult);			break;
				case QUERY_TYPE_ITEMMALL_CHECK:				QueryTypeItemmallCheck(pResult);			break;
				case QUERY_TYPE_BANKINFO:					QueryTypeBankinfo(pResult);					break;
				case QUERY_TYPE_JUMIN_CHECK:				QueryTypeJuminCheck(pResult);				break;
				case QUERY_TYPE_BLOCK_TITLE:				QueryTypeBlockTitle(pResult);				break;
				case QUERY_TYPE_WARNING_TITLE:				QueryTypeWarningTitle(pResult);				break;
				case QUERY_TYPE_BANKITEM_CHECK:				QueryTypeBankItemCheck(pResult);			break;					
			}
		}
		break;
		
	case QUERY_TYPE_EXECUTE:		// THExecuteSQL
		{
			switch(pResult->dwQueryUID)
			{			
				case QUERY_TYPE_USER_LOGOUT:	
				{
					if(pResult->nError <= -1000)
					{
						DWORD dwPropID;
						memcpy(&dwPropID, &pResult->pData, 4);
						Log(LOG_FATAL, "Logout Process Failed! (PropID:%d)", dwPropID);
					}
				}
				break;			

				case QUERY_TYPE_LOGOUT_ALL:
				{
					if(pResult->nError >= 0)
					{
						Log(LOG_NORMAL, "Logout All User Successfully!");
					}
				}
				break;
			}			
		}
		break;

	case QUERY_TYPE_EXECUTE_BY_PARAM:	// THExecuteSQLByParam
		{
			switch(pResult->dwQueryUID)
			{
				case QUERY_TYPE_CREATE_CHAR_INIT_BINARY:	QueryTypeCreateCharInitBinary(pResult);	break;
				case QUERY_TYPE_BANK_UPDATE:				QueryTypeBankUpdate(pResult);			break;
			}
		}
		break;
	}	
}

void ReceivedFromDBLog(DBRECEIVEDATA* pResult)	//kjk(41123)
{
	if(!g_bIsServerRunning)	return;
}