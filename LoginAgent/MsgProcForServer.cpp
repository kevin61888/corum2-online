#include "stdafx.h"
#include "MsgProcForServer.h"
#include "ThrowQuery.h"
#include "UserTable.h"
#include "PacketEncrypt.h"
#include "ChinaBillingProc.h"
#include "User.h"


// 해당 클라이언트(모니터링 툴)에게 서버들 정보를 보내줌.
void AcmdServerAliveInfo(WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	dwLength;pServer;
	
	WSTAS_SERVER_ALIVE_INFO*	pRecievePacket = (WSTAS_SERVER_ALIVE_INFO*)pMsg;
	ASTC_SERVER_ALIVE_INFO		SendPacket;

	SendPacket.wServerType			= pRecievePacket->wServerType;
	SendPacket.dwServerID			= pRecievePacket->dwServerID;
	SendPacket.wConnectedUserCount	= pRecievePacket->wConnectedUserCount;
	SendPacket.dwExpGainRate		= pRecievePacket->dwExpGainRate;
	SendPacket.dwMagicItemDropRate	= pRecievePacket->dwMagicItemDropRate;

	g_pUserTable->BroadCastToMonitoringTool((char*)&SendPacket, SendPacket.GetPacketSize());
}


// 월드에서 로그인 서버와 커넥트 성공했다.
void CmdServerAttach(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	dwLength;

	WSTAS_SERVER_ATTACH *pPacket = (WSTAS_SERVER_ATTACH*)pMsg;
	
	LPWorldServerData pServer = g_pServerTable->GetServer( pPacket->dwWorldServerID );

	if(!pServer)
	{
		Log(LOG_IMPORTANT, "Server That has Invalid ID Try to Login!(ID=%d)", pPacket->dwWorldServerID);
	
		//등록되지 않은 아이디의 월드번호가 들어온 경우 
		ASTWS_SERVER_ATTACH_RESULT	FailPacket;
		FailPacket.bErrorCode = 1;	
		g_pNet->SendToServer(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
	
	char szIP[ MAX_IP_LENGTH ]={0,};	WORD wPort=0;
	g_pNet->GetServerAddress(dwConnectionIndex, szIP, &wPort);

	// DB에 등록된 서버 IP와 다른 경우 
	if(__strcmp(pServer->szIPForServer, szIP))		
	{
		Log(LOG_IMPORTANT, "Invalid WorldServer IP Address Connected! Check WorldServerTable!(%s)", szIP);

		//DB에 등록되어있지않은 IP의 월드서버가 Attach를 시도한 경우 
		ASTWS_SERVER_ATTACH_RESULT	FailPacket;
		FailPacket.bErrorCode = 2;
		g_pNet->SendToServer(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if(pServer->dwServerStatus == SERVER_STATUS_CONNECTED)
	{
		Log(LOG_IMPORTANT, "WorldServer Already Connected! (WorldMapID:%d)", pServer->dwWorldMapID);

		// DB에 등록되어있지않은 IP의 월드서버가 Attach를 시도한 경우 
		ASTWS_SERVER_ATTACH_RESULT	FailPacket;
		FailPacket.bErrorCode = 3;
		g_pNet->SendToServer(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
		
	pServer->dwConnectionIndex = dwConnectionIndex;
	pServer->dwServerStatus = SERVER_STATUS_CONNECTED;
	g_pNet->SetServerInfo(dwConnectionIndex, pServer);
	
	// Attach Success!
	ASTWS_SERVER_ATTACH_RESULT	SuccessPacket;
	SuccessPacket.bErrorCode = 0;
#ifdef ADULT_MODE
	SuccessPacket.bAdultMode = 1;
#else
	SuccessPacket.bAdultMode = 0;
#endif
	g_pNet->SendToServer(dwConnectionIndex, (char*)&SuccessPacket, SuccessPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	Log(LOG_JUST_DISPLAY, "@ WorldServer Attached! (WorldMapID=%d, IP=%s)", pServer->dwWorldMapID, szIP);
}


// 월드에서 유저를 받아드릴 준비가 되었다.
void AcmdUserAcceptAllowed(WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	dwLength;

	WSTAS_USER_ACCEPT_ALLOWED *pPacket = (WSTAS_USER_ACCEPT_ALLOWED*)pMsg;

	CUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwPropID);

	if(!pUser) return;

	ASTC_CONNECT_WORLD_SERVER	Packet;
	Packet.dwPropID		= pUser->GetPropID();
	Packet.dwChar_Index = pUser->m_dwChar_Index;
	Packet.dwIp			= pServer->dwIPForUser;
	Packet.wPort		= pServer->wPortForUser;
	Packet.dwSerialCode = pUser->m_dwSerialCode;
	Packet.bEventFlag	= pUser->m_bEventNum;
	
	SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	pUser->m_dwStatus = USER_STATUS_AWAITING_WORLD_CONNECT; 
}


// 월드에서 유저를 받아드릴 준비가 안됬다.
void AcmdUserAcceptFailed(WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	dwLength;pServer;

	WSTAS_USER_ACCEPT_FAILED *pPacket = (WSTAS_USER_ACCEPT_FAILED*)pMsg;

	CUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwPropID);

	if(!pUser) return;

	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
	{
		SendChinaBillingLogout(pUser->GetPropID(), pUser->GetUserID(), pUser->GetIP());
	}

	ASTC_CHARACTER_SELECT_FAIL FailPacket;
	FailPacket.bErrorCode = 5;
	SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


// 월드에 로그인 잘되었다.
void AcmdUserWorldLoginSuccess(WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	dwLength;

	WSTAS_USER_WORLD_LOGIN_SUCCESS *pPacket = (WSTAS_USER_WORLD_LOGIN_SUCCESS*)pMsg;

	CUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwPropID);

	if(pUser)
	{
		pUser->m_dwCurWorldServerPos	= pServer->dwWorldMapID;
		pUser->m_dwStatus				= USER_STATUS_GAME_STARTED; 
		pUser->m_dwLastBillingCheckTick = pUser->m_dwBillingStartTick = timeGetTime();		//게임 시작한 시점의 TickCount
	
		// CM툴이면 접속을 끊지 않는다.
		if( FALSE == pUser->m_bIsMonitor ) 
		{
			g_pNet->CompulsiveDisconnectUser(pUser->GetConnectionIndex());
		}
		
		if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
		{
			// 그래 여기야~~ 이제 이놈이 빌링 처리 할놈인지 
			if(pUser->GetSelectedCharacter()->dwLevel < 10)
			{
				// 10 미만의 캐릭터는 꽁짜다.
				pUser->m_sChinaBillingData.dwChinaBillingPoint		= 0;
				pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_limit;
			}
		}
	}
	else
	{
		Log(LOG_IMPORTANT, "Userinfo is not found at AcmdUserWorldLoginSuccess() Function!");
	}
}


// 월드에서 유저가 접속을 끈었다. 로그인에 알려준다.
void AcmdWorldUserDisconnect(WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	dwLength;pServer;

	WSTAS_WORLD_USER_DISCONNECT	*pPacket = (WSTAS_WORLD_USER_DISCONNECT*)pMsg;

	CUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwPropID);

	if(!pUser)				return;

	// CMTool이면 모든 로그아웃 처리를 OnDisconnectUser에서 처리한다. 
	if(pUser->m_bIsMonitor)	return;		

	ThrowLogoutQuery(pUser);
	
	Log(LOG_JUST_DISPLAY, "World User Logouted...  (ID:%s, Name:%s)", pUser->m_szLoginID, pUser->GetSelectedCharacter()->szName);
	g_pUserTable->Remove(pUser);
}


void AcmdRequestChinaBilling(WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
	{
		WSTAS_REQUEST_CHINA_BILLING* pPacket = (WSTAS_REQUEST_CHINA_BILLING*)pMsg;

		Log(LOG_IMPORTANT, "Recv In AcmdRequestChinaBilling");
		
		CUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwPropID);

		if(!pUser)	return;
		
		// World Server Billing Info //
		if (china_billing_info_approval != pUser->m_sChinaBillingData.eChinaBillingUserData)
		{
			// 로그인이 제대로 안된상태이다. 
			SendChinaBillingInfo(pUser, pPacket->dwUserIndex, Billing_Info_Procedure_Type_Normal);
		}
		else
		{
			// 그냥 정보만 보내라.
			SendChinaBillingInfo(pUser, pPacket->dwUserIndex, Billing_Info_Procedure_Type_NONE);
		}
	}
}

void AcmdRequestChinaBillingInfo(WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
	{
		WSTAS_REQUEST_CHINA_BILLINGINFO* pPacket = (WSTAS_REQUEST_CHINA_BILLINGINFO*)pMsg;

		Log(LOG_IMPORTANT, "Recv In AcmdRequestChinaBillingInfo");
		
		CUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwPropID);

		if(!pUser)	return;

		// 일단 레벨 업해서 빌링서버에 재 접속 하라는 메시지를 받은이상 월드로 잠깐 나와라..

		SendChinaBillingInfo(pUser, pPacket->dwUserIndex, Billing_Info_Procedure_Type_Level_Limit);		// Over 10 level

		if (china_billing_info_approval !=pUser->m_sChinaBillingData.eChinaBillingUserData )
		{
			SendChinaBillingLogin(pUser->GetPropID(), pUser->GetUserID(), pUser->GetIP());
		}		
	}
}
