#include "stdafx.h"
#include "ChinaBillingProc.h"
#include "OwnServer.h"
#include "../CommonServer/Log.h"
#include "User.h"
#include "UserTable.h"
#include "DBProc.h"
#include "ThrowQuery.h"
#include "../CommonServer/LoginPacket.h"
#include "../CommonServer/AgentServerPacket.h"
#include "WorldServerTable.h"
#include "recvmsg.h"
#include "ServerFunction.h"
#include "PacketEncrypt.h"
#include "User.h"




// function pointers for china billing message process.
void	(*g_BillPacketProc[ MAX_BILLING_PACKET_TYPE ])(char* pMsg, DWORD dwLength);


void InitializeProcFunction()
{
	ZeroMemory(g_BillPacketProc, sizeof(g_BillPacketProc));

	g_BillPacketProc[china_billing_packet_login_result]	= OnBillingMsgLoginResult;
	g_BillPacketProc[china_billing_packet_kickout]		= OnBillingMsgKickOut;
	g_BillPacketProc[china_billing_packet_server_clear]	= OnBillingMsgClear;
}

void ConnectToBillingServer()
{
	if(g_pThis->IsBillingServerConnected())
	{
		Log(LOG_JUST_DISPLAY, "@ Already connected with billing server!");
		return;
	}

	if(!g_pNet->ConnectToServerWithServerSide(	g_pThis->GetIPForBillingServer(), 
												g_pThis->GetPortForBillingServer(), 
												SuccessToConnectBillingServer, 
												FailToConnectBillingServer, NULL))
	{
		Log(LOG_IMPORTANT, "@ Failed To Connect Billing Server ...");
	}
}

void DisconnectBillingServer()
{
	g_pThis->SetBillingServerClosing(TRUE);

	// 연결을 끊기 전에 clear 메시지 송신한다.
	SendChinaBillingClear();
}

void __stdcall FailToConnectBillingServer(void* pExt)
{
	Log(LOG_IMPORTANT, "@ Failed To Connect Billing Server! (IP:%s, Port:%d)", 
		g_pThis->GetIPForBillingServer(), g_pThis->GetPortForBillingServer() );

	// 재연결 시도 중이었다면...
	if(g_pThis->IsTryToConnectBillingServerContinuous())
	{
		g_pNet->ResumeTimer(CHINA_BILLING_CONNECT_TIMER_INDEX);
	}
}

void __stdcall SuccessToConnectBillingServer(DWORD dwConnectionIndex, void* pExt)
{
	Log(LOG_JUST_DISPLAY, "@ Billing Server Connected Successfully!");
	
	g_pThis->SetBillingServerConnectionIndex(dwConnectionIndex);
	g_pThis->SetBillingServerConnected(TRUE);

	// 재연결 시도 중이었다면...
	if(g_pThis->IsTryToConnectBillingServerContinuous())
	{
		g_pNet->PauseTimer(CHINA_BILLING_CONNECT_TIMER_INDEX);
		g_pThis->SetTryToConnectBillingServer(FALSE);
		
		// 현재 접속하여 게임 중인 모든 유저의 로그인 정보를 다시 보낸다!
		SendChinaBillingLoginAll();
	}

	// 연결 성공했으므로, clear 메시지 보내서 알린다.
	SendChinaBillingClear();
}

void __stdcall OnDisconnectBillingServer()
{	
	// 연결을 종료하길 원해서 종료한 경우... 
	if(	g_pThis->IsBillingServerConnected() && 
		g_pThis->IsBillingServerConnectionClosing() )
	{
		g_pThis->SetBillingServerConnected(FALSE);
		g_pThis->SetBillingServerConnectionIndex(0);
		g_pThis->SetBillingServerClosing(FALSE);

		Log(LOG_IMPORTANT, "@ Connection With Billing Server Successfully Closed~");
		
		// 이제 서버를 종료시킨다!!
		TerminateServer();

		return;
	}

	// 연결이 끝겼다.... 우리는 일단 접속 시도하고,
	// 차후에도 연결이 실패하면 일정 주기마다 연결이 될 때까지 연결을 시도한다.
	Log(LOG_IMPORTANT, "@ Connection with Billing Server Closed!!");
	g_pThis->SetBillingServerConnected(FALSE);

	// 재연결임을 세팅하고.. 연결 시도한다.
	g_pThis->SetTryToConnectBillingServer(TRUE);	

	ConnectToBillingServer();
}

void __stdcall OnRecvFromBillingServerTCP(char* pMsg, DWORD dwLength)
{
	// Validation of packet type!
	if(	pMsg[0] >= MAX_BILLING_PACKET_TYPE || NULL == g_BillPacketProc[pMsg[0]] )
	{
		Log(LOG_IMPORTANT, "Invalid Billing Packet Received(Type: %d)", pMsg[0]);
		return;	
	}

	Log(LOG_IMPORTANT, "<BILLING INFO>Recv Packet Type : %d", (BYTE)*pMsg);
	// reset keep alive check timer

	// proc 실행!!
	(*g_BillPacketProc[pMsg[0]])(pMsg, dwLength);
}

void __stdcall OnChinaBillingKeepAlive(DWORD dwEventIndex)
{
	dwEventIndex;

	// Keep Alive 용 더미 패킷을 전송한다.
	SendChinaBillingDummy();
}

void __stdcall OnChinaBillingTryConnect(DWORD dwEventIndex)
{
	dwEventIndex;

	// 재연결 시도할 시간이 됐다. 다시 시도하라.
	ConnectToBillingServer();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void OnBillingMsgClear(char* pMsg, DWORD dwLength)
{
	Proto_China_Billing_Server_Clear* pPacket = (Proto_China_Billing_Server_Clear*)pMsg;

	// 제대로 된 월드인지 확인!
	if(pPacket->btGameWorld != atoi(g_pThis->GetServerSetCode())) return;

	// 연결 종료 과정이 아니면 무시한다.
	if(!g_pThis->IsBillingServerConnectionClosing()) return;
	
		// 연결을 종료하길 원해서 종료한 경우... 
	if(	g_pThis->IsBillingServerConnected() && 
		g_pThis->IsBillingServerConnectionClosing() )
	{
		// 연결을 끝낸다.
		g_pNet->CompulsiveDisconnectServer(g_pThis->GetBillingServerConnectionIndex());

		g_pThis->SetBillingServerConnected(FALSE);
		g_pThis->SetBillingServerConnectionIndex(0);
		g_pThis->SetBillingServerClosing(FALSE);

		Log(LOG_IMPORTANT, "@ Connection With Billing Server Successfully Closed~");

		// 이제 서버를 종료시킨다!!
		TerminateServer();
	}
}

void OnBillingMsgLoginResult(char* pMsg, DWORD dwLength)
{
	Proto_China_Billing_Login_Result* pPacket = (Proto_China_Billing_Login_Result*)pMsg;
	
	
	//////////////////////////////////////////////////////////////////////////
	// Log
	Log(LOG_IMPORTANT, "In OnBillingMsgLoginResult");
	//--


	// 제대로 된 월드인지 확인!
	if(pPacket->GetServerSetID() != atoi(g_pThis->GetServerSetCode())) return;

	char szUserName[0xff]={0,};
	lstrcpyn(szUserName, pPacket->GetUserName(), MAX_ID_LENGTH);
	DWORD dwUserIndex	= pPacket->GetUserIndex();	

	// 유저 테이블에서 이 유저 객체 획득.
	CUser* pUser		= g_pUserTable->GetUserInfo(dwUserIndex);

	// 일단 해당 유저가 해제된 상태면 ...
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "pUser is NULL!! in OnBillingMsgLoginResult()");

		// 로그인 성공한 경우에는 다시 로그아웃 메시지를 날려줘여.. 이때 ip는 모르니까 0으로!!
		switch(pPacket->GetLoginResult())
		{
		case china_billing_login_success :
		case china_billing_login_unexpected1 :
		case china_billing_login_unexpected2 :
			SendChinaBillingLogout(dwUserIndex, szUserName, 0);
			break;
		}

		return;
	}

	// 결과에 따른 로그인 처리!!
	switch(pPacket->GetLoginResult())
	{
		// 일단 로그인 성공시킨다.
	case china_billing_login_success :
	case china_billing_login_unexpected1 :
	case china_billing_login_unexpected2 :
		{
			// 유저에게 남은 시간/기간 정보 패킷 전송
			WSTC_CHINA_BILL_REMAIN_TIME Packet;
			
			Packet.SetTimeType(pPacket->GetTimeType());
			Packet.SetTime(pPacket->GetTime());

			SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			// 아이템몰 관련 쿼리하면서 계속 진행.
			
			if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
			{
				// China Biling Info Set //
				pUser->m_sChinaBillingData.dwChinaBillingPoint		= Packet.dwTimeValue;
				pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_approval;

				LPWorldServerData pServer = g_pServerTable->GetServer( 1 );
				
				RS_CharacterLogInfo_With_Equip * pSelectedCharacter = pUser->GetSelectedCharacter();

				if(pServer && pSelectedCharacter)
				{
					Log(LOG_IMPORTANT, "<BILLING INFO> %s(%3d), Login OK!!", 
						pSelectedCharacter->szName, pSelectedCharacter->dwLevel);

					SendChinaBillingInfo(
						pUser,	
						pPacket->GetUserIndex(), 
						Billing_Info_Procedure_Type_NONE);		// normal proc type						

				}
			}
		}
		break;

		// 로그인 실패!
	case china_billing_login_no_user :
	case china_billing_login_over_capacity :
	case china_billing_login_timeout :
	case china_billing_login_canceled :
	case china_billing_login_etc :
	case china_billing_login_time_expired :
		{
			if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
			{
				// China Biling Info Set //
				pUser->m_sChinaBillingData.dwChinaBillingPoint		= 0;
				pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_limit;

				// 유저에게 남은 시간/기간 정보 패킷 전송
				WSTC_CHINA_BILL_REMAIN_TIME Packet;
				
				Packet.SetTimeType(pPacket->GetTimeType());
				Packet.SetTime(pPacket->GetTime());

				SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	

				LPWorldServerData pServer = g_pServerTable->GetServer( 1 );
				RS_CharacterLogInfo_With_Equip * pSelectedCharacter = pUser->GetSelectedCharacter();

				if(pServer && pSelectedCharacter)
				{
						SendChinaBillingInfo(pUser, pPacket->GetUserIndex(), Billing_Info_Procedure_Type_Normal);	// normal
						Log(LOG_IMPORTANT, "<BILLING INFO> %s(%3d), Login Fail!!", 
							pSelectedCharacter->szName, pSelectedCharacter->dwLevel);
				}
			}
			if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 : 050104 hwoarang
			{
				// 유저에게 로그인 실패 - 로그인 실패 메시지 송신.
				WSTC_LOGIN_FAIL	packet;
				packet.bResult = (BYTE)(0xf0 | pPacket->GetLoginResult());
				SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			

				OnDisconnectUser(pUser->m_dwConnectionIndex);
				g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
				g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
			}
		}break;
	}
}

void OnBillingMsgKickOut(char* pMsg, DWORD dwLength)
{
	Proto_China_Billing_Kickout* pPacket = (Proto_China_Billing_Kickout*)pMsg;

	// 제대로 된 월드인지 확인!
	if(pPacket->GetServerSetID() != atoi(g_pThis->GetServerSetCode())) return;
	
	char szUserName[0xff]={0,};
	lstrcpyn(szUserName, pPacket->GetUserName(), MAX_ID_LENGTH);
	DWORD dwUserIndex	= pPacket->GetUserIndex();

	// 유저 테이블에서 이 유저 객체 획득.
	CUser* pUser = g_pUserTable->GetUserInfo(dwUserIndex);
	
	// 일단 해당 유저가 해제된 상태면 ...
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "pUser is NULL!! in OnBillingMsgKickOut()");

		// 무조건 로그아웃 메시지를 날려줘여.. 이때 ip는 모르니까 0으로!!
		SendChinaBillingLogout(dwUserIndex, szUserName, 0);

		return;
	}

	char szInfo[0xff] = {0, };
	Log(LOG_NORMAL, "OnBillingMsgKickOut");

	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 : 050104 hwoarang
	{
		// China Biling Info Set //
		pUser->m_sChinaBillingData.dwChinaBillingPoint		= 0;
		pUser->m_sChinaBillingData.eChinaBillingUserData	= china_billing_info_limit;

		if(pUser->m_dwCurWorldServerPos != SERVER_POS_AGENT)
		{
			char szInfo[0xff] = {0, };
			wsprintf(szInfo, "OnBillingMsgKickOut - ID : %s, UserIndex : %u", szUserName, dwUserIndex);
			Log(LOG_NORMAL, szInfo);

			//////////////////////////////////////////////////////////////////////////
			// 일단 로그오프 시킨다.
			ASTWS_KICK_USER	KickPacket;
			//다른 월드나 던전 서버에 유저가 있다면 모두 종료시켜 버려라 
			KickPacket.dwCharIndex = pUser->GetSelectedCharacter()->dwCharacterIndex;
			g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
			
			pUser->m_dwStatus = 0;

			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);

			//유저 테이블에서 이 유저 삭제 
			g_pUserTable->Remove(pUser);		//Free도 함 
			pUser = NULL;
			//--
		}
	}
	else
	{
		// 아직 월드에 로그인 되지 않은 상태 
		if(pUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)		
		{
			DWORD dwConnectionIndex = pUser->m_dwConnectionIndex;
			OnDisconnectUser(dwConnectionIndex);
			g_pNet->SetUserInfo(dwConnectionIndex, NULL);
			g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);

			SendChinaBillingLogout(dwUserIndex, szUserName, pUser->GetIP());
		}
		else
		{
			// 월드 서버에게 이 유저 로그아웃 시키라고 메시지 전송!
			ASTWS_KICK_USER Packet;

			Packet.dwCharIndex = pUser->m_dwChar_Index;
			Packet.btKickCode = pPacket->GetKickCode();

			g_pServerTable->BroadCastWorldServer((char*)&Packet, Packet.GetPacketSize());
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////

void SendToChinaBillingServer(char* szMsg, DWORD dwLen, BOOL bTimer)
{
	if(bTimer)
	{
		g_pNet->PauseTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);

		g_pNet->SendToServer(g_pThis->GetBillingServerConnectionIndex(), szMsg, dwLen, FLAG_SEND_NOT_ENCRYPTION);

		g_pNet->ResumeTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);
	}
	else
	{
		g_pNet->SendToServer(g_pThis->GetBillingServerConnectionIndex(), szMsg, dwLen, FLAG_SEND_NOT_ENCRYPTION);
	}

	//////////////////////////////////////////////////////////////////////////
	//	log
	Log(LOG_IMPORTANT, "<BILLING INFO>Send Packet Type : %d", (BYTE)*szMsg);
	// --
}

void SendChinaBillingLogin(DWORD dwUserIndex, LPCSTR szUserId, DWORD dwIP, BOOL bTimer)
{
	if(!g_pThis->IsBillingServerConnected()) return;
	if(g_pThis->IsBillingServerConnectionClosing()) return;

	Proto_China_Billing_Login Packet;
	
	Packet.SetServerSetID(atoi(g_pThis->GetServerSetCode()));
	Packet.SetUserIndex(dwUserIndex);
	Packet.SetUserID(szUserId);	
	Packet.SetIP(dwIP);


	//////////////////////////////////////////////////////////////////////////
	// Log
	CUser * pUser = g_pUserTable->GetUserInfo(dwUserIndex);
	if (pUser)
	{
		RS_CharacterLogInfo_With_Equip* pSelectCharacter = pUser->GetSelectedCharacter();
		Log(LOG_IMPORTANT, "<BILLING INFO> TRY LOGIN %s(%d)...", 
		pSelectCharacter->szName, pSelectCharacter->dwLevel);
	}
	//--
	
	SendToChinaBillingServer((char*)&Packet, Packet.GetPacketSize(), bTimer);
}

void SendChinaBillingLogout(DWORD dwUserIndex, LPCSTR szUserId, DWORD dwIP)
{
	if(!g_pThis->IsBillingServerConnected()) return;
	if(g_pThis->IsBillingServerConnectionClosing()) return;

	Proto_China_Billing_Logout Packet;

	Packet.SetServerSetID(atoi(g_pThis->GetServerSetCode()));
	Packet.SetUserIndex(dwUserIndex);
	Packet.SetUserID(szUserId);
	Packet.SetIP(dwIP);

	//////////////////////////////////////////////////////////////////////////
	// Log
	CUser * pUser = g_pUserTable->GetUserInfo(dwUserIndex);
	if (pUser)
	{
		RS_CharacterLogInfo_With_Equip* pSelectCharacter = pUser->GetSelectedCharacter();
		Log(LOG_IMPORTANT, "<BILLING INFO> LOGOUT %S(%d)...", 
			pSelectCharacter->szName, pSelectCharacter->dwLevel);
	}
	//--

	SendToChinaBillingServer((char*)&Packet, Packet.GetPacketSize());	
}

void SendChinaBillingClear()
{
	if(!g_pThis->IsBillingServerConnected()) return;

	Proto_China_Billing_Server_Clear Packet;

	Packet.SetServerSetID(atoi(g_pThis->GetServerSetCode()));
	
	SendToChinaBillingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendChinaBillingDummy()
{
	Proto_China_Billing_Dummy Packet;

	SendToChinaBillingServer((char*)&Packet, Packet.GetPacketSize());
}

void SendChinaBillingLoginAll()
{
	g_pNet->PauseTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);

	POSITION_ pos = g_pUserTable->GetHeadPosition();
	while(pos)
	{
		CUser* pUser = g_pUserTable->GetNext(pos);
		if(pUser) 
		{
			SendChinaBillingLogin(	pUser->GetPropID(), 
									pUser->GetUserID(), 
									pUser->GetIP(),
									FALSE);
		}
	}

	g_pNet->ResumeTimer(CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX);
}


void SendChinaBillingInfo(CUser * pUser, DWORD dwUserIndex, enum BILLING_INFO_PROCEDURE_TYPE eProcType)
{
	ASTWS_CHINA_BILLING_INFO pChinaBillingInfoPacket;
	pChinaBillingInfoPacket.byType			= pUser->m_sChinaBillingData.eChinaBillingUserData;
	pChinaBillingInfoPacket.dwPoint			= pUser->m_sChinaBillingData.dwChinaBillingPoint;
	pChinaBillingInfoPacket.dwUserIndex		= dwUserIndex;
	pChinaBillingInfoPacket.SetProcedureType(eProcType);


	//////////////////////////////////////////////////////////////////////////
	// Log
	RS_CharacterLogInfo_With_Equip * pSelectCharacter = pUser->GetSelectedCharacter();
	Log(LOG_IMPORTANT, "SendChinaBillingInfo() %S(%d) : PROC(%d), Point(%d)", 
		pSelectCharacter->szName, pSelectCharacter->dwLevel, 
		pUser->m_sChinaBillingData.eChinaBillingUserData, 
		pUser->m_sChinaBillingData.dwChinaBillingPoint);
	//--	


	g_pServerTable->BroadCastWorldServer((char*)&pChinaBillingInfoPacket, 
		pChinaBillingInfoPacket.GetPacketSize());

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void QueryTyprLoginChinaBilling(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser)
	{
		Log(LOG_IMPORTANT, "pUser is NULL!! in QueryTypeLogin()");
		return;
	}

	DWORD dwResult;
	memcpy(&dwResult, pResult->Query.select.pResult, sizeof(DWORD));

	switch(dwResult)
	{
	case 0:		//Login OK	//billing check 구간 아님 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				__asm int 3
				return;
			}

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

			// 빌링 서버와 정상적인 연결이 되어 있는 상태가 아니라면..
			// 유저의 로그인을 무조건 실패시킨다.
			if(!g_pThis->IsBillingServerConnected() ||
				g_pThis->IsBillingServerConnectionClosing())
			{
				// 실패 메시지 전송!!
				WSTC_LOGIN_FAIL	packet;
				packet.bResult = (BYTE)(0xf0); // !!!!! 이 경우 실패 코드에 성공 코드를 넣어 보낸다!!!! 
				SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				
				// 유저 릴리즈!
				pUser->m_dwStatus = 0;
				memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
				g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
				
				OnDisconnectUser(pUser->m_dwConnectionIndex);
				g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
				g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);				
			}
			
			//-------  2003년 10월 28일 이벤트용 추천 Event 코딩 -----------------
			//if(LoginResult.dwRestrictType == EVENT_NUM_RECOM_ADD_ITEM)
				pUser->m_bEventNum = EVENT_NUM_RECOM_ADD_ITEM;
			//--------------------------------------------------------------------
			
			//유저 테이블에 유저 추가
			pUser->m_dwStatus = USER_STATUS_LOGIN_SUCCESS;
			pUser->m_dwPropID = atoi(LoginResult.szPropID);

			//만약 유저해쉬에 해당 유저정보가 남아있는데 로그인처리 가 또 요청된 거라면 
			//이전 유저해쉬 정보를 삭제 시켜주는 처리를 하자 
						
			PrevUserProcess( pUser->m_dwPropID );

			// User 테이블에 추가...
			BOOL bChk = g_pUserTable->Add(pUser);				

			// 일단 여기에서 빌링 서버에게 이 유저가 접속함을 통보한다!!!!!
			// 이후의 처리는 빌링 서버로부터 로그인 결과를 받아 실행한다.
			// WSTAS_USER_WORLD_LOGIN_SUCCESS 패킷을 받아서 보낸다.
			// 월드에 들어가서 부터 빌링 시작.
			//SendChinaBillingLogin(pUser->GetPropID(), pUser->GetUserID(), pUser->GetIP());
			if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 Test : 050104 hwoarang
			{
				ThrowItemMallQuery(pUser);	
			}
		}
		break;
	case 1:	//SQL DB 오류
	case 2:	//없는 아이디
	case 3: //비밀번호 틀림
		{
			WSTC_LOGIN_FAIL	packet;
			packet.bResult = (BYTE)dwResult;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;

	case 4:		//게임 접속중인 아이디	//billing check 구간 아님 
		{
			DWORD dwPropID = 0;

			//DB에서 Logout 처리 	
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				__asm int 3
				return;
			}

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
			
			int nUserServerNum = atoi(g_pThis->GetServerSetCode());
			if(LoginResult.dwPrevUserServerSet != (DWORD)nUserServerNum)
			{
				//다른 서버로 중복 로그인이 되었다는 메세지를 보낸다. 
				WSTC_LOGIN_FAIL	AnotherPacket;
				AnotherPacket.bResult = (BYTE)LOGIN_FAIL_OVERLAPPED_ID_ANOTHER_SERVER;
				AnotherPacket.dwExData = LoginResult.dwPrevUserServerSet;
				SendToUser(pUser, (char*)&AnotherPacket, AnotherPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

				//유저 Release 처리 
				pUser->m_dwStatus = 0;
				memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
				g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
				
				OnDisconnectUser(pUser->m_dwConnectionIndex);
				g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
				g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);

				pUser = NULL;
				return;
			}
			else
			{
				//중복 로그인 되었다는 메세지 보낸다. 
				WSTC_LOGIN_FAIL	packet;
				packet.bResult = (BYTE)LOGIN_FAIL_OVERLAPPED_ID;
				SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}

			dwPropID = atoi(LoginResult.szPropID);
			
			CUser* pPrevUser = g_pUserTable->GetUserInfo(dwPropID);

			if(pPrevUser)
			{
				if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	//Agent에 접속되어 있는 유저 
				{
					WSTC_LOGIN_FAIL	packet;
					packet.bResult = (BYTE)LOGIN_FAIL_OVERLAPPED_ID;
					SendToUser(pPrevUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
					
				ASTWS_KICK_USER	KickPacket;
				//다른 월드나 던전 서버에 유저가 있다면 모두 종료시켜 버려라 
				KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
				g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
				
				pPrevUser->m_dwStatus = 0;

				OnDisconnectUser(pPrevUser->m_dwConnectionIndex);
				g_pNet->SetUserInfo(pPrevUser->m_dwConnectionIndex,NULL);
				g_pNet->CompulsiveDisconnectUser(pPrevUser->m_dwConnectionIndex);

				//유저 테이블에서 이 유저 삭제 
				g_pUserTable->Remove(pPrevUser);		//Free도 함 
				pPrevUser = NULL;
			}			

			ThrowLogoutQuery(pUser);			
			
			//유저 Release 처리 
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			//g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);

			pUser = NULL;

		}
		break;

	case 5:		//Block 중인 아이디 //billing check 구간 아님 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			} // if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
	
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_USER_BLOCK;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
			//g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
			//--
		}
		break;

	case 6:		//14세미만 임시가입자로 부모 동의 처리 후 게임가능합니다. //billing check 구간 아님 
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_14_OVER;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			//g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 

			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);			

		}
		break;
		
		//미리 캐릭 생성 대상자가 아닙니다. 
	case 11:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 11;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			//g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;

		//애들은가라~ 만 18세 이상 서버 
	case 12:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 12;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			//g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;

	//실명 인증을 안한 유저 
	case 13:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 13;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;

	default:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_ETC;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);

			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			
			OnDisconnectUser(pUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pUser->m_dwConnectionIndex,NULL);
			g_pNet->CompulsiveDisconnectUser(pUser->m_dwConnectionIndex);
		}
		break;
	}	
}

