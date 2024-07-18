#include "stdafx.h"
#include "LicensingProc.h"
#include "OwnServer.h"
#include "User.h"
#include "UserTable.h"
#include "WorldServerTable.h"
#include "LicensingPacket.h"
#include "ServerFunction.h"
#include "ThrowQuery.h"
#include "recvmsg.h"


DWORD				g_dwLastHeartbeatTick = 0;
extern MACHINENAME	g_MachineName;


void	(*g_LicensingPacketProc[ MAX_LICENSING_PACKET_TYPE ])(char* pMsg, DWORD dwLength);


void InitializeProcFunctionForLicensing()
{
	ZeroMemory(g_LicensingPacketProc, sizeof(g_LicensingPacketProc));

	g_LicensingPacketProc[Proto_Licensing_Heartbeat]			= OnLicensingHeartbeat;
	g_LicensingPacketProc[Proto_Licensing_AuthenticaionResult]	= OnLicensingAuthenticationResult;
	g_LicensingPacketProc[Proto_Licensing_Command]				= OnLicensingCommand;
}

BOOL ConnectToLicensingServer()
{
	if(g_pThis->IsLicensingServerConnected())
	{
		Log(LOG_IMPORTANT, "@ Already connected with Licensing server!");
		return FALSE;
	}

	Log(LOG_IMPORTANT, "@ NOW Try to connect to Licensing server!");
	if(!g_pNet->ConnectToServerWithServerSide(	LICENSING_SERVER_IP, 
												LICENSING_SERVER_PORT, 
												SuccessToConnectLicensingServer, 
												FailToConnectLicensingServer, NULL))
	{
		Log(LOG_IMPORTANT, "@ Failed To Connect Licensing Server ...");
		return FALSE;
	}

	return TRUE;
}

void DisconnectLicensingServer()
{
	g_pThis->SetAuthenticated(FALSE);
	g_pThis->SetLicensingServerClosing(TRUE);

	// 연결을 종료하길 원해서 종료한 경우... 
	if(	g_pThis->IsLicensingServerConnected() && 
		g_pThis->IsLicensingServerConnectionClosing() )
	{
		// 연결을 끝낸다.
		g_pNet->CompulsiveDisconnectServer(g_pThis->GetLicensingServerConnectionIndex());

		g_pThis->SetLicensingServerConnected(FALSE);
		g_pThis->SetLicensingServerConnectionIndex(0);
		g_pThis->SetLicensingServerClosing(FALSE);

		Log(LOG_IMPORTANT, "@ Connection With Licensing Server Successfully Closed~");
	}
}

void __stdcall FailToConnectLicensingServer(void* pExt)
{
	pExt;

	Log(LOG_IMPORTANT, "@ Failed To Connect License Server!");

	// 재연결 시도 중이었다면...
	if(g_pThis->IsTryToConnectLicensingServerContinuous())
	{
		g_pNet->ResumeTimer(LICENSING_CONNECT_TIMER_INDEX);
	}	
}

void __stdcall SuccessToConnectLicensingServer(DWORD dwConnectionIndex, void* pExt)
{
	pExt;

	Log(LOG_IMPORTANT, "@ Licensing Server Connected Successfully!");
	
	g_pThis->SetLicensingServerConnectionIndex(dwConnectionIndex);
	g_pThis->SetLicensingServerConnected(TRUE);

	// 재연결 시도 중이었다면...
	if(g_pThis->IsTryToConnectLicensingServerContinuous())
	{
		g_pNet->PauseTimer(LICENSING_CONNECT_TIMER_INDEX);
		g_pThis->SetTryToConnectLicensingServer(FALSE);
	}

	// 연결 성공했으므로, 인증 요청 메시지 보낸다.
	SendLicensingAuthentication();
}

void __stdcall OnDisconnectLicensingServer()
{
	// 연결을 종료하길 원해서 종료한 경우... 
	if(	g_pThis->IsLicensingServerConnected() && g_pThis->IsLicensingServerConnectionClosing() )
	{
		g_pThis->SetLicensingServerConnected(FALSE);
		g_pThis->SetLicensingServerConnectionIndex(0);
		g_pThis->SetLicensingServerClosing(FALSE);
		g_pThis->SetAuthenticated(FALSE);

		Log(LOG_IMPORTANT, "@ Connection With Licensing Server Successfully Closed~");
		
		// 이제 서버를 종료시킨다!!
		//TerminateServer();

		return;
	}

	// 연결이 끝겼다.... 우리는 일단 접속 시도하고,
	// 차후에도 연결이 실패하면 일정 주기마다 연결이 될 때까지 연결을 시도한다.
	Log(LOG_IMPORTANT, "@ Connection with Licensing Server Closed!!");
	g_pThis->SetLicensingServerConnected(FALSE);

	// 재연결임을 세팅하고.. 연결 시도한다.
	g_pThis->SetTryToConnectLicensingServer(TRUE);	

	ConnectToLicensingServer();	
}


void __stdcall OnRecvFromLicensingServerTCP(char* pMsg, DWORD dwLength)
{
	// Validation of packet type!
	if(	pMsg[0] >= MAX_LICENSING_PACKET_TYPE || NULL == g_LicensingPacketProc[pMsg[0]] )
	{
		Log(LOG_IMPORTANT, "Invalid Licensing Packet Received(Type: %d)", pMsg[0]);
		return;	
	}

	// proc 실행!!
	(*g_LicensingPacketProc[pMsg[0]])(pMsg, dwLength);	
}

void __stdcall OnLicensingHeartbeat(DWORD dwEventIndex)
{
	dwEventIndex;

	// 서버의 최종 응답 시간을 체크한다.
	if(0 != g_dwLastHeartbeatTick)
	{
		DWORD dwPassedTick = timeGetTime() - g_dwLastHeartbeatTick;

		// 5 주기동안 Heartbeat를 못받았다면!!!!
		if(dwPassedTick > LICENSING_HEARTBEAT_SEND_PERIOD * 5) // 5 min!!
		{
			Log(LOG_IMPORTANT, "OnLicensingHeartbeat() : DisconnectLicensingServer()");
			DisconnectLicensingServer();
			return;
		}
	}

	// HeartBeat 패킷을 전송한다.
	SendLicensingHeartbeat();
}

void __stdcall OnLicensingTryConnect(DWORD dwEventIndex)
{
	dwEventIndex;

	Log(LOG_IMPORTANT, "try connctto lic");
	// 재연결 시도할 시간이 됐다. 다시 시도하라.
	ConnectToLicensingServer();
}

void __stdcall OnLicensingSendInfo(DWORD dwEventIndex)
{
	dwEventIndex;

	if(!g_pUserTable) return;

	// 동접 정보 송신하라!!
	SendLicensingCurrentUser(g_pUserTable->GetCount());
}


void OnLicensingHeartbeat(char* pMsg, DWORD dwLength)
{
	Proto_Licensing_Packet_Heartbeat* pPacket = (Proto_Licensing_Packet_Heartbeat*)pMsg;
	if(pPacket->GetPacketSize() != dwLength) 	
	{
		IllegalSizePacketProc(pPacket->byPacketType, dwLength);
		return;
	}

	// 수신한 Tick를 저장한다.
	g_dwLastHeartbeatTick = timeGetTime();
}

void OnLicensingAuthenticationResult(char* pMsg, DWORD dwLength)
{
	Proto_Licensing_Packet_AuthenticaionResult* pPacket =
		(Proto_Licensing_Packet_AuthenticaionResult*)pMsg;

	if(pPacket->GetPacketSize() != dwLength) 	
	{
		IllegalSizePacketProc(pPacket->byPacketType, dwLength);
		return;
	}

	if(pPacket->byResult != 1)
	{
		Log(LOG_IMPORTANT,"OnLicensingAuthenticationResult()");
		TerminateServer();
	}
	else
	{
		// 인증 성공했다... 다음을 진행하도록 한다....
		g_pThis->SetAuthenticated(TRUE);

		// 주기적으로 동접 정보 보내도록 세팅.
		g_pNet->ResumeTimer(LICENSING_SEND_INFO_INDEX); 
	}
}

void OnLicensingCommand(char* pMsg, DWORD dwLength)
{
	Proto_Licensing_Packet_Command* pPacket = (Proto_Licensing_Packet_Command*)pMsg;
	if(pPacket->GetPacketSize() != dwLength) 	
	{
		IllegalSizePacketProc(pPacket->byPacketType, dwLength);
		return;
	}

	switch(pPacket->byCommand)
	{
	case Command_Send_Heartbeat:			// Heartbeat를 날리라고 명령.
		SendLicensingHeartbeat(); 
		break;

	case Command_Send_CurrentUser:			// 현재 유저 정보를 날리라고 명령.
		if(g_pUserTable) SendLicensingCurrentUser(g_pUserTable->GetCount());
		break;

	case Command_Send_BillingInformation:	// 빌링 정보를 날리라고 명령. 현재로선 아무런 행동 없다....
		break;

	case Command_Suicide:					// 죽으라고 명령.
		{
			Log(LOG_IMPORTANT,"OnLicensingCommand()");
			TerminateServer();
		}
		break;
	
	case Command_DropAllUsers:				// 모든 유저의 연결을 끊어 버리라고 명령.
		KickAllUser();
		break;
	}
}

void KickAllUser()
{
	if(!g_pUserTable) return;
	
	POSITION_ pos = g_pUserTable->GetHeadPosition();
	while(pos)
	{
		CUser* pPrevUser = (CUser*)g_pUserTable->GetNext(pos);		

		if(pPrevUser->m_dwChar_Index)
		{
			// 다른 월드나 던전 서버에 유저가 있다면 모두 종료시켜 버려라 
			ASTWS_KICK_USER	KickPacket;
			KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
			g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
		}
		
		// Agent에 접속되어 있는 유저 
		if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	
		{
			DWORD dwConnectionIndex = pPrevUser->GetConnectionIndex();
			OnDisconnectUser(dwConnectionIndex);
			g_pNet->SetUserInfo(dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);			
		}
		else
		{
			ThrowLogoutQuery(pPrevUser);

			// 유저 테이블에서 이 유저 삭제 
			g_pUserTable->Remove(pPrevUser); // Free도 함 
		}
	}
}

// send functions..
void SendToLicensingServer(char* szMsg, DWORD dwLen, BOOL bTimer)
{
	if(!g_pThis->IsLicensingServerConnected()) return;

	if(bTimer)
	{
		g_pNet->PauseTimer(LICENSING_HEARTBEAT_TIMER_INDEX);

		g_pNet->SendToServer(g_pThis->GetLicensingServerConnectionIndex(), szMsg, dwLen, FLAG_SEND_NOT_ENCRYPTION);

		g_pNet->ResumeTimer(LICENSING_HEARTBEAT_TIMER_INDEX);
	}
	else
	{
		g_pNet->SendToServer(g_pThis->GetLicensingServerConnectionIndex(), szMsg, dwLen, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void SendLicensingHeartbeat()
{
	if(!g_pThis->IsLicensingServerConnected()) return;
	if(g_pThis->IsLicensingServerConnectionClosing()) return;

	Proto_Licensing_Packet_Heartbeat Packet;

	SendToLicensingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendLicensingAuthentication()
{
	if(!g_pThis->IsLicensingServerConnected()) return;
	if(g_pThis->IsLicensingServerConnectionClosing()) return;

	Proto_Licensing_Packet_Authenticaion Packet;

	SendToLicensingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendLicensingCurrentUser(DWORD dwCCU)
{
	if(!g_pThis->IsLicensingServerConnected()) return;
	if(g_pThis->IsLicensingServerConnectionClosing()) return;

	Proto_Licensing_Packet_CurrentUser Packet;
	Packet.dwCurrentUserCount = dwCCU;

	SendToLicensingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendLicensingBillingInformation(char* pData, DWORD dwSize)
{
	if(!g_pThis->IsLicensingServerConnected()) return;
	if(g_pThis->IsLicensingServerConnectionClosing()) return;

	Proto_Licensing_Packet_BillingInformation Packet;

	if(dwSize > sizeof(Packet.BillingInformation)) return;
	{
	//	Log(LOG_IMPORTANT, "BillingInformation is larger than packet!! %u", dwSize);
		return;
	}

	CopyMemory(Packet.BillingInformation, pData, dwSize);

	SendToLicensingServer((char*)&Packet, Packet.GetPacketSize());
}

