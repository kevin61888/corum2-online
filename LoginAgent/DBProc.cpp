#include "stdafx.h"
#include "DBProc.h"
#include "OwnServer.h"
#include "WorldServerTable.h"
#include "UserTable.h"
#include "recvmsg.h"
#include "ThrowQuery.h"
#include "User.h"
#include "PacketEncrypt.h"
#include "ServerFunction.h"
#include "../CommonServer/CommonHeader.h"
#include "ChinaBillingProc.h"

extern IOleDBThread*	g_pDb;
extern CTime			g_RecentCheckTime;
extern LPBASEITEM_HASH	g_pBaseItemHash;
extern MACHINENAME		g_MachineName;

void QueryTypeLoginBilling(DBRECEIVEDATA* pResult);

void QueryTypeLogin(DBRECEIVEDATA* pResult)
{
	if( IS_ABLE_SERVICE_TYPE(ST_CHINA_BILLING) )//중국빌링 이면 : 050104 hwoarang
	{
		QueryTyprLoginChinaBilling(pResult);
	}
	else if( IS_ABLE_SERVICE_TYPE(ST_BILLING) )
	{
		QueryTypeLoginBilling(pResult);
	}
	else
	{
		QueryTypeLoginFree(pResult);
	}
}


void QueryTypeLoginFree(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_IMPORTANT, "pUser is NULL!! in QueryTypeLogin()");
		return;
	}

	DWORD dwResult = *(DWORD*)(pResult->Query.select.pResult);

	switch(dwResult)
	{
	case 111: 
	case 0:		//Login OK	//billing check 구간 아님 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			}

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			ZeroMemory(&LoginResult,sizeof(LoginResult));//재원 추가
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
			
			//2003년 10월 28일 이벤트용 추천 Event 코딩 
			pUser->m_bEventNum = EVENT_NUM_RECOM_ADD_ITEM;
		
			//유저 테이블에 유저 추가
			pUser->m_dwStatus = USER_STATUS_LOGIN_SUCCESS;
			pUser->SetPropID(atoi(LoginResult.szPropID));

			pUser->m_ID_RegDate = LoginResult.id_regdate;//050323 재원 

			//만약 유저해쉬에 해당 유저정보가 남아있는데 로그인처리 가 또 요청된 거라면
			//이전 유저해쉬 정보를 삭제 시켜주는 처리를 하자 
			PrevUserProcess( pUser->GetPropID() );

			g_pUserTable->Add(pUser);

			if(dwResult == 111)
			{
				ThrowBlockTitleQuery(pUser, FALSE);
			}

			// 아이템몰에서 받을 아이템이 있는지 쿼리 
			ThrowItemMallQuery(pUser);
		}
		break;

	case 1:	//SQL DB 오류
	case 2:	//없는 아이디
	case 3: //비밀번호 틀림
		{
			SendToUserLoginFailPacket( pUser, (BYTE)dwResult, NULL );
			
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case 4:	//게임 접속중인 아이디	//billing check 구간 아님 
		{
			DWORD dwPropID = 0;

			// DB에서 Logout 처리 	
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			}

			LOGIN_RESULT_WITH_PROPID LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
			
			int nUserServerNum = atoi(g_pThis->GetServerSetCode());
			
			if(LoginResult.dwPrevUserServerSet != (DWORD)nUserServerNum)
			{
				SendToUserLoginFailPacket( pUser, (BYTE)LOGIN_FAIL_OVERLAPPED_ID_ANOTHER_SERVER
					, LoginResult.dwPrevUserServerSet );

				// 유저 Release 처리 
				pUser->m_dwStatus = 0;
				memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
				g_pUserTable->FreeUser(pUser);	// Alloc 했던 유저 메모리 풀에서 해제 							
				g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
				pUser = NULL;

				return;
			}
			else
			{
				SendToUserLoginFailPacket( pUser, (BYTE)LOGIN_FAIL_OVERLAPPED_ID, NULL );
			}

			dwPropID = atoi(LoginResult.szPropID);
			
			CUser* pPrevUser = g_pUserTable->GetUserInfo(dwPropID);

			if(pPrevUser)
			{
				if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	
				{
					SendToUserLoginFailPacket( pPrevUser, (BYTE)LOGIN_FAIL_OVERLAPPED_ID, NULL );
				}
					
				// 다른 월드나 던전 서버에 유저가 있다면 모두 종료시켜 버려라 
				ASTWS_KICK_USER	KickPacket;
				KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
				g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
				
				pPrevUser->m_dwStatus = 0;
				memset(pPrevUser->m_szLoginID, 0, MAX_ID_LENGTH);
				g_pNet->SetUserInfo(pPrevUser->GetConnectionIndex(), NULL);				
				//유저 테이블에서 이 유저 삭제 Free도 함 
				g_pUserTable->Remove(pPrevUser);		
				pPrevUser = NULL;
			}

			ThrowLogoutQuery(pUser);
			
			// 유저 Release 처리 
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);		
		}
		break;

	case 5:		//Block 중인 아이디 //billing check 구간 아님 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				break;
			} // if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
	
			if( IS_ABLE_NATION(NC_JAPAN))
			{
				if(LoginResult.dwCoupon == 1) // 일본 block! 블럭 사유 읽어와야한다.
				{
					ThrowBlockTitleQuery(pUser, TRUE);
					break;
				} // if(LoginResult.dwCoupon == 1) // 일본 block! 블럭 사유 읽어와야한다.
			} // if( IS_ABLE_NATION(NC_JAPAN))
		
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_USER_BLOCK, NULL );
	
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);				
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case 6:		//14세미만 임시가입자로 부모 동의 처리 후 게임가능합니다. //billing check 구간 아님 
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_14_OVER, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;
		
	case 11:	//미리 캐릭 생성 대상자가 아닙니다. 
		{
			SendToUserLoginFailPacket( pUser, 11, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

		
	case 12:	//애들은가라~ 만 18세 이상 서버 
		{
			SendToUserLoginFailPacket( pUser, 12, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case 13:	//실명 인증을 안한 유저 
		{
			SendToUserLoginFailPacket( pUser, 13, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case LOGIN_FAIL_NOT_BETA_TESTER:	// 일본, 베타테스터 아님 
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_NOT_BETA_TESTER, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	case LOGIN_FAIL_EJECT_USER_JAPAN:	//일본, 강퇴자 
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_EJECT_USER_JAPAN, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);			
		}
		break;

	case LOGIN_FAIL_SECESSION_USER:		//일본, 탈퇴한 회원
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_SECESSION_USER, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);		
		}
		break;
	
	default:
		{
			SendToUserLoginFailPacket( pUser, LOGIN_FAIL_ETC, NULL );

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;
	}	
}


void QueryTypeLoginBilling(DBRECEIVEDATA* pResult)
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
	case 0:	//돈을 내시오! 
		{
			WSTC_LOGIN_FAIL	packet;
			packet.bResult = (BYTE)dwResult;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;
		
	case 111:	//Login OK with Warning		//Added by KBS 040309
	case 1:		//Login OK
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			}

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
			
			pUser->m_bEventNum = EVENT_NUM_RECOM_ADD_ITEM;
			
			//유저 테이블에 유저 추가
			pUser->m_dwStatus = USER_STATUS_LOGIN_SUCCESS;
			pUser->SetPropID(atoi(LoginResult.szPropID));
			pUser->m_BillingEday = LoginResult.eday;
			pUser->m_ID_RegDate = LoginResult.id_regdate;//050323 재원 
			pUser->m_dwRemainTime = LoginResult.dwRemainTime;
			pUser->m_bBillingType = (BYTE)LoginResult.dwRestrictEx;

			//만약 유저해쉬에 해당 유저정보가 남아있는데 로그인처리 가 또 요청된 거라면 
			//이전 유저해쉬 정보를 삭제 시켜주는 처리를 하자 
			PrevUserProcess( pUser->GetPropID() );

			g_pUserTable->Add(pUser);

			// 일본 경고 유저의 경고 타이틀 획득!
			if(dwResult == 111)
			{
				ThrowBlockTitleQuery(pUser, FALSE);
			}
			
			// Item Mall에 선물 들어온게 있는지 체크 billing check 구간 
			ThrowItemMallQuery(pUser);
		}
		break;

	case 7:	//SQL DB 오류
	case 2:	//없는 아이디
	case 3: //비밀번호 틀림
		{
			WSTC_LOGIN_FAIL	packet;
			packet.bResult = (BYTE)dwResult;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);						 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;


	case 4:		//게임 접속중인 아이디 
		{
			DWORD dwPropID = 0;
			
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
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
				g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
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
	
			//서버에서 먼저 있던 유저의 정보를 찾아서 
			dwPropID = atoi(LoginResult.szPropID); // added by deepdark... 맞는거냐??
			CUser* pPrevUser = g_pUserTable->GetUserInfo(dwPropID);

			if(pPrevUser)
			{
				if(pPrevUser->m_dwCurWorldServerPos == SERVER_POS_AGENT)	//Agent에 접속되어 있는 유저 
				{
					WSTC_LOGIN_FAIL	packet;
					packet.bResult = (BYTE)LOGIN_FAIL_OVERLAPPED_ID;
					SendToUser(pPrevUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);					
				}
				
				// 다른 월드나 던전 서버에 유저가 있다면 모두 종료시켜 버려라 
				ASTWS_KICK_USER	KickPacket;
				KickPacket.dwCharIndex = pPrevUser->m_dwChar_Index;
				g_pServerTable->BroadCastWorldServer((char*)&KickPacket, KickPacket.GetPacketSize());
				
				// 유저 테이블에서 이 유저 삭제 Free도 함 
				pPrevUser->m_dwStatus = 0;
				g_pNet->SetUserInfo(pPrevUser->GetConnectionIndex(), NULL);				
				g_pUserTable->Remove(pPrevUser);		
				pPrevUser = NULL;
			}
			else
			{
				Log(LOG_NORMAL, "Login overlapped but prev userinfo is not exist!");
			}

			//DB에서 이전유저의 로그아웃 처리 하고 
			dwPropID = atoi(LoginResult.szPropID);
			
			ThrowLogoutQuery(pUser);

			//유저 Release 처리 
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
			pUser = NULL;

		}
		break;

	case 5:		//Block 중인 아이디 
		{
			if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )
			{
				Log(LOG_IMPORTANT, "Query size is overflow!");
				return;
			} // if(pResult->Query.select.dwRowSize > sizeof(LOGIN_RESULT_WITH_PROPID) )

			LOGIN_RESULT_WITH_PROPID	LoginResult;
			memcpy(&LoginResult, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

			if( IS_ABLE_NATION(NC_JAPAN) )
			{
				if(LoginResult.dwCoupon == 1)
				{
					ThrowBlockTitleQuery(pUser, TRUE);
					break;
				} // if(LoginResult.dwCoupon == 1)
			} // if( IS_ABLE_NATION(NC_JAPAN))

			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_USER_BLOCK;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
			//--
				
		}
		break;

	case 6:		//14세미만 임시가입자로 부모 동의 처리 후 게임가능합니다.	//billing check 구간 
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_14_OVER;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);

		}
		break;
		
		//미리 캐릭 생성 대상자가 아닙니다.  //billing check 구간 
	case 11:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 11;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

		//애들은가라~ 만 18세 이상 서버		//billing check 구간 
	case 12:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 12;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

	//실명 인증을 안한 유저 //billing check 구간 
	case 13:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = 13;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

		//탈퇴한 회원 //billing check 구간 
	case 50:
		{
			WSTC_LOGIN_FAIL	packet;
			packet.bResult = (BYTE)dwResult;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
			
		}
		break;

		//일본, 베타테스터 아님 //billing check 구간 
	case 14:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_NOT_BETA_TESTER;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;

		//일본, 강퇴자 //billing check 구간 
	case 15:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_EJECT_USER_JAPAN;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);			
		}
		break;

		//일본, 탈퇴한 회원//billing check 구간 
	case 16:
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_SECESSION_USER;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
			
			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);		
		}
		break;
	
	default://billing check 구간 
		{
			WSTC_LOGIN_FAIL packet;
			packet.bResult = LOGIN_FAIL_ETC;
			SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->m_dwStatus = 0;
			memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);

			g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
			g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		}
		break;
	}	
}


void QueryTypeChrSelectInfo(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (!pUser)	return;
	
	if(pResult->nError < 0)
	{
		Log(LOG_IMPORTANT, "Query Error: QueryTypeChrSelectInfo()...  (User: %s)", pUser->m_szLoginID);
		return;
	}
	
	RS_CharacterLogInfo_With_Equip EnumChar[ 4 ];
	memset(EnumChar, 0, sizeof(EnumChar));

	if( pResult->Query.select.dwRowCount > 4 )
		__asm int 3;
	
	memcpy(EnumChar, pResult->Query.select.pResult, sizeof(RS_CharacterLogInfo_With_Equip) * pResult->Query.select.dwRowCount );
		
	WSTC_LOGIN_SUCCESS packet;
#ifdef ADULT_MODE
	packet.bAdultMode = TRUE;
#else
	packet.bAdultMode = FALSE;
#endif
	packet.bNum = (BYTE)pResult->Query.select.dwRowCount;
	
	for(int i = 0; i < packet.bNum; i++)
	{
		if(EnumChar[i].byInvItemCheck==0)
		{
			for(int j = 0; j < MAX_EQUIP; j++)
			{
				if(EnumChar[i].Equip[j].m_wItemID!=0)
				{
					WORD wOldId = EnumChar[i].Equip[j].m_wItemID;
					EnumChar[i].Equip[j].m_wItemID = (WORD)(( wOldId / 100 * 200 ) + wOldId % 100);
				}
			}			
		}
	}

	// ( sizeof(CItem)*MAX_EQUIP );		//Equip을 제외한 구조체 사이즈 
	int		nSize	= sizeof(RS_CharacterLogInfo_Without_Equip);
	WORD	wIndex	= 0;

	for(i = 0; i < packet.bNum; i++)
	{
		wIndex = EnumChar[i].wChrNum;

		//잘못된 CharacterIndex 일때 
		if(wIndex < 1 || wIndex > 4)	
		{
			WSTC_LOGIN_FAIL	FailPacket;	
			FailPacket.bResult = LOGIN_FAIL_INVALID_CHARACTER_INDEX;	
			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			memset(pUser->m_EnumChar, 0, sizeof(pUser->m_EnumChar));
			Log(LOG_IMPORTANT, "Character Overlapped! (login_id:%s)", pUser->m_szLoginID);
			return;
		}
		
		wIndex -= 1;	//배열 ZeroBase로 
		
		//dwStr값이 있으면 이미 이 CharacterNum에 캐릭터가 있는걸로 간주하자.
		if(pUser->m_EnumChar[ wIndex ].dwStr > 0)
		{
			WSTC_LOGIN_FAIL	FailPacket;	
			FailPacket.bResult = LOGIN_FAIL_CHARACTER_OVERLAPPED;	
			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			memset(pUser->m_EnumChar, 0, sizeof(pUser->m_EnumChar));
			Log(LOG_IMPORTANT, "Character Overlapped! (login_id:%s)", pUser->m_szLoginID);
			return;
		}

		// 유저 구조체에는 ChaNum 순서대로 저장하자!
		memcpy(&pUser->m_EnumChar[ wIndex ], &EnumChar[i], nSize);			
		memcpy(&packet.Character[i], &EnumChar[i], nSize );	
		memcpy(&pUser->m_EnumChar[ wIndex ].sCreateTime, &EnumChar[i].sCreateTime, sizeof(pUser->m_EnumChar[ wIndex ].sCreateTime));
		pUser->m_EnumChar[ wIndex ].byInvItemCheck = EnumChar[i].byInvItemCheck;
		
		BYTE byLHand = EQUIP_TYPE_LHAND1;
		BYTE byRHand = EQUIP_TYPE_RHAND1;
		
		if(EnumChar[i].dwGuildId!=0)
		{
			pUser->m_EnumChar[wIndex].dwGuildId = EnumChar[i].dwGuildId;
			pUser->m_EnumChar[wIndex].byRank	= EnumChar[i].byRank;
		}

		if(EnumChar[i].byCurrentHand==2)
		{
			byLHand = EQUIP_TYPE_LHAND2;		
			byRHand = EQUIP_TYPE_RHAND2;
		}	
		
		packet.Character[i].wLHandID	= EnumChar[i].Equip[ byLHand ].m_wItemID;			//왼손 무기
		packet.Character[i].wRHandID	= EnumChar[i].Equip[ byRHand ].m_wItemID;			//오른손 무기
		packet.Character[i].wHelmetID	= EnumChar[i].Equip[ EQUIP_TYPE_HELMET ].m_wItemID;	//헬멧
		packet.Character[i].wMailID		= EnumChar[i].Equip[ EQUIP_TYPE_ARMOR ].m_wItemID;	//갑옷
		packet.Character[i].wMantleID	= EnumChar[i].Equip[ EQUIP_TYPE_MANTLE ].m_wItemID;	//망또
	}

	SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	//Item 몰 처리가 있는 유저의 경우 
	if(pUser->m_bItemMallCount > 0)
	{
		ASTC_ITEMMALL_INFO MallPacket;
		BYTE byCharNum = 0;

		for(i = 0; i < 4; i++)
		{
			if(pUser->m_EnumChar[i].dwCharacterIndex==0)
				byCharNum++;
		}

		if(byCharNum==4)
			pUser->m_byItemMallType = 0xff;

		Log(LOG_NORMAL, "Client ItemMall : %u, %u", pUser->m_byItemMallType, pUser->m_bItemMallCount);
	
		if(pUser->m_byItemMallType==0xff)
		{
			MallPacket.bItemCount		= pUser->m_bItemMallCount;
			MallPacket.bSuccess			= FALSE;
			MallPacket.byItemMallType	= 0xff;
		}
		else if(pUser->m_wItemMallSupplyCount >= 150)
		{
			pUser->m_wItemMallSupplyCount	= 0;
			MallPacket.bItemCount			= pUser->m_bItemMallCount;
			MallPacket.bSuccess				= FALSE;
			MallPacket.byItemMallType		= 0;
		}
		else
		{
			MallPacket.bItemAllCount	= pUser->m_bItemMallCount;
			MallPacket.bItemCount		= (BYTE)pUser->m_wItemMallSupplyCount;
			MallPacket.bSuccess			= TRUE;
			MallPacket.byItemMallType	= 0;
		}
		
		SendToUser(pUser, (char*)&MallPacket, MallPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void QueryTypeCheckMonitoring(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (NULL == pUser)	return;
	
	g_pUserTable->AddMonitoringTool(pUser->GetConnectionIndex());

	Log(LOG_JUST_DISPLAY, "Monitoring Tool login.. (ID:%s, Name:%s)", pUser->m_szLoginID, pUser->GetSelectedCharacter()->szName);
	pUser->m_bIsMonitor = TRUE;	
}


void QueryTypeIsAliveUser(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if( pUser == NULL )		return;

	ASTC_IS_ALIVE_USER Packet;

	if(pResult->Query.select.dwRowCount == 0)
	{
		Packet.bIsAliveUser = 0;
		SendToUser( pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
	else
	{
		Packet.bIsAliveUser = 1;
		SendToUser( pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
}

void QueryTypeBillingCheck(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	if(pResult->nError < 0)
	{
		Log(LOG_IMPORTANT, "Billing Check Query Error!");
		return;
	}

	BILLING_CHECK_RESULT Result;
	memcpy(&Result, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

	if( IS_ABLE_SERVICE_TYPE(ST_DEVELOP) )//DEV_MODE : 050104 Hwoarang
	{
		char szTemp[1024]={0,};
		wsprintf(szTemp, "PropID:%d, BillingType:%d, eDay:%d-%d-%d %d:%d:%d, RemainTime:%d, Continue:%d", Result.dwPropID, Result.nBillingType, 
					Result.eday.year, Result.eday.month, Result.eday.day, Result.eday.hour, Result.eday.minute, Result.eday.second,
					Result.dwRemainTime, Result.dwContinue);

		OutputDebugString(szTemp);
		OutputDebugString("\n\n");
	}

	//결제 정보 없다. 
	if(!Result.dwContinue)
	{
		char szProc[0xff]={0,};
		wsprintf(szProc, "EXEC up_GameLogout '%s'", pUser->m_szLoginID );	
		g_pDb->THExecuteSQL(szProc, FALSE, 0, NULL, (BYTE)MEMBER_DB);	//DB에서 로그인 정보 삭제 
		
		ASTWS_KICK_USER packet;
		packet.dwCharIndex = pUser->m_dwChar_Index;
		g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());

		g_pUserTable->Remove(pUser);
		Log(LOG_NORMAL, "User Billing Expired! (ID:%s, Name:%s)", pUser->m_szLoginID, pUser->GetSelectedCharacter()->szName);
		return;
	}

	//결제 타입이 변경되었을때 
	if(pUser->m_bBillingType != Result.nBillingType)
	{
		ASTWS_CHANGE_BILLING_TYPE	ChangePacket;

		ChangePacket.bBillingType = (BYTE)Result.nBillingType;
		ChangePacket.dwChar_Index = pUser->m_dwChar_Index;
		ChangePacket.dwRemainTime = Result.dwRemainTime;
		ChangePacket.eDay = Result.eday;
		g_pServerTable->BroadCastWorldServer((char*)&ChangePacket, ChangePacket.GetPacketSize());

		pUser->m_dwBillingStartTick = timeGetTime();	//타입 변경에 따른 빌링시작시간 변경 
	}

	pUser->m_bBillingType = (BYTE)Result.nBillingType;
	pUser->m_BillingEday  = Result.eday;
	pUser->m_dwRemainTime = Result.dwRemainTime;
	
	switch(Result.nBillingType)
	{
	case USER_BILLING_FLAT_RATE:
		{
			if(Result.eday.year == 0)	
			{
				Log(LOG_IMPORTANT, "eday is NULL! (USER_BILLING_FLAT_RATE)");
				return;
			}

			CTime EndDay(Result.eday.year, Result.eday.month, Result.eday.day, Result.eday.hour, Result.eday.minute, Result.eday.second);
			CTimeSpan ts = EndDay - g_RecentCheckTime;

			if(ts.GetTotalMinutes() < 10)	//정액제 종료 일자가 10분 남았을때 
			{
				int nSecond = ts.GetTotalSeconds();

				if(nSecond >= 0)
				{
					//몇분 남았다 알려줘야 된다. 
					ASTWS_BILLING_SMALL_TIME	packet;
					packet.bBillingType = (BYTE)Result.nBillingType;
					packet.dwChar_Index = pUser->m_dwChar_Index;
					packet.dwSecond = nSecond;
					g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());
				}
				else
				{
					Log(LOG_NORMAL, "Clock Mismatch!");
				}
			}
		}
		break;
		
	case PCROOM_BILLING_FLAT_RATE:
		{
			if(Result.eday.year == 0)	
			{
				Log(LOG_IMPORTANT, "eday is NULL! (PCROOM_BILLING_FLAT_RATE)");
				return;
			}

			CTime EndDay(Result.eday.year, Result.eday.month, Result.eday.day, Result.eday.hour, Result.eday.minute, Result.eday.second);
			CTimeSpan ts = EndDay - g_RecentCheckTime;

			if(ts.GetTotalHours() < 48)		//정액제 종료 일자가 2일 전부터 알림 
			{
				int nSecond = ts.GetTotalSeconds();

				if(nSecond >= 0)
				{
					//몇분 남았다 알려줘야 된다. 
					ASTWS_BILLING_SMALL_TIME	packet;
					packet.bBillingType = (BYTE)Result.nBillingType;
					packet.dwChar_Index = pUser->m_dwChar_Index;
					packet.dwSecond = nSecond;
					g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());
				}
				else
				{
					Log(LOG_NORMAL, "Clock Mismatch!");
				}
			}
		}
		break;
	case PCROOM_BILLING_SOMEHOUR_FREE:
		{
			if( IS_ABLE_NATION(NC_TAIWAN) )
			{//타이완이고 빌링체크하면 다음으로 간다
				if( !IS_ABLE_SERVICE_TYPE(ST_BILLING) )//일반빌링 :050104 hwoarang
				{
					break;
				}
			} // if( IS_ABLE_NATION(NC_TAIWAN) )
			else
			{
				break;
			}
		}
	case USER_BILLING_1HOUR_FREE:
	case USER_BILLING_METER_RATE:
		{
			if(Result.dwRemainTime < 600)	//남은 시간이 10분 미만일때 
			{
				//몇분 남았다 알려줘야 된다. 
				ASTWS_BILLING_SMALL_TIME	packet;
				packet.bBillingType = (BYTE)Result.nBillingType;
				packet.dwChar_Index = pUser->m_dwChar_Index;
				packet.dwSecond = Result.dwRemainTime;
				g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());
			}
		}
		break;

	case PCROOM_BILLING_METER_RATE:
		{
			if(Result.dwRemainTime < 18000)	//남은 시간이 5시간 미만일때 
			{
				//몇분 남았다 알려줘야 된다. 
				ASTWS_BILLING_SMALL_TIME	packet;
				packet.bBillingType = (BYTE)Result.nBillingType;
				packet.dwChar_Index = pUser->m_dwChar_Index;
				packet.dwSecond = Result.dwRemainTime;
				g_pServerTable->BroadCastWorldServer((char*)&packet, packet.GetPacketSize());
			}
		}
		break;
	}
}


void QueryTypeBankItemCheck(DBRECEIVEDATA* pResult)
{
	char	szQuery[QUERY_STRING_SIZE]		= {0,};
	BYTE	bBankItemCheck[4]	= {0,};
	BOOL	bIsBankItemCheck	= FALSE;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	int nRet = pResult->Query.select.dwRowCount;

	if(nRet > 0)
	{
		memcpy(bBankItemCheck,pResult->Query.select.pResult,pResult->Query.select.dwRowCount);

		for(int i = 0; i < 4; i++)
		{
			if(bBankItemCheck[i] == 1)
			{
				bIsBankItemCheck = TRUE;
				break;
			}
		}		

		wsprintf(szQuery, "Update CharacterInfo Set InvItemCheck = %u, BankItemCheck = %u Where Name = '%s'", 1, bIsBankItemCheck, pUser->GetSelectedCharacter()->szName);
		g_pDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, (BYTE)GAME_DB);	
	}	
}


void QueryTypeCreateNewChar(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;

	if(pResult->nError<0)
	{
		WSTC_CREATE_CHARACTER_FAIL	FailPacket;
		pUser->m_bClass = 0;
		FailPacket.nResultValue = -100;
		memset(pUser->GetSelectedCharacter()->szName, 0, MAX_CHARACTER_NAME_LENGTH);	// 생성 실패이니 CharacterName 지우자 
		pUser->m_bSelectedChrIndex = 0;
		
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}

	CREATE_CHARACTER_RESULT Result;
	memcpy(&Result, pResult->Query.select.pResult , pResult->Query.select.dwRowSize);
			
	switch(Result.dwResult)
	{
		case 1:	//캐릭터 추가 성공 
		{
			pUser->m_EnumChar[ pUser->m_bSelectedChrIndex ].dwCharacterIndex = Result.dwChar_Index;

			char szQuery[QUERY_STRING_SIZE] = {0,};
						
			wsprintf(szQuery, "Select BankItemCheck From CharacterInfo Where ID = '%s'",pUser->m_szLoginID);
			g_pDb->THOpenRecord(szQuery,QUERY_TYPE_BANKITEM_CHECK,(void*)pUser->m_dwConnectionIndex,4,(BYTE)GAME_DB);
			
			//재원 050323 .
			pUser->m_dwCharCount = Result.dwCharCount; //만들어진 캐릭터 총 개수 갱신.			
			ThrowCreateCharInitBinaryQuery(pUser);
			

			ThrowInsertWebBoardQuery(pUser, pUser->GetSelectedCharacter()->szName, pUser->m_bClass);

			break;
		}
		break;

		case -1:	//이미 있는 캐릭터 
		{
			WSTC_CREATE_CHARACTER_FAIL	FailPacket;
			FailPacket.nResultValue = -1;

			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
			
			memset(pUser->m_EnumChar[ pUser->m_bTryCreateCharSlot ].szName, 0, MAX_CHARACTER_NAME_LENGTH);
			pUser->m_bTryCreateCharSlot = 0xff;			
		}
		break;

		case -2:	//자리 중복 
		{
			WSTC_CREATE_CHARACTER_FAIL	FailPacket;
			FailPacket.nResultValue = -2;

			SendToUser(pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

			memset(pUser->m_EnumChar[ pUser->m_bTryCreateCharSlot ].szName, 0, MAX_CHARACTER_NAME_LENGTH);
			pUser->m_bTryCreateCharSlot = 0xff;
		}
		break;
	}
}

void QueryTypeDeleteCharacter(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData); 
	if(!pUser) return;

	if(pResult->nError<0)
	{
		WSTC_DELETE_CHARACTER_RESULT	FailPacket;
		FailPacket.bSuccess = FALSE;
		FailPacket.nResultValue = -100;		//쿼리 에러 
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}

	DELETE_CHARACTER_RESULT DeleteResult;	memset(&DeleteResult, 0, sizeof(DELETE_CHARACTER_RESULT));
	memcpy(&DeleteResult, pResult->Query.select.pResult , pResult->Query.select.dwRowSize);

	switch(DeleteResult.dwResult)
	{
		case 2:		//모두다 지워진 경우 
		case 1:
		{
			for(int i = 0; i < 4; i++)
			{
				if(pUser->m_EnumChar[i].dwCharacterIndex!=0)
				{
					if(__strcmp(DeleteResult.szDeletedCharacterName, pUser->m_EnumChar[i].szName)==0)
					{
						memset(&pUser->m_EnumChar[i], 0, sizeof(pUser->m_EnumChar[i]));
						break;
					}
				}				
			}						

			WSTC_DELETE_CHARACTER_RESULT	SuccessPacket;
			SuccessPacket.bSuccess = TRUE;
			SuccessPacket.nResultValue = DeleteResult.dwResult;

			SendToUser(pUser, (char*)&SuccessPacket, SuccessPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

			ThrowDeleteWebBoardQuery(pUser, DeleteResult.szDeletedCharacterName);
		}
		break;

		case 4:		//잘못된 캐릭터 삭제 시도 
		{
			WSTC_DELETE_CHARACTER_RESULT	Packet;
			Packet.bSuccess = FALSE;
			Packet.nResultValue = DeleteResult.dwResult;

			SendToUser(pUser, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}
		break;
	}
}


void QueryTypeJuminCheck(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if(!pUser) return;	

	if(pResult->nError<0)
	{
		WSTC_DELETE_CHARACTER_RESULT	FailPacket;
		FailPacket.bSuccess = FALSE;
		FailPacket.nResultValue = -101;		//쿼리 에러 
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		Log(LOG_IMPORTANT, "QueryTypeJuminCheck Failed! (ID:%s)", pUser->m_szLoginID );
		return;
	}

	JUMIN_CHECK_RESULT result;
	memcpy(&result, pResult->Query.select.pResult, sizeof(JUMIN_CHECK_RESULT));

	if(result.dwResult == 1)	//주민번호 맞음 
	{	
		BYTE byIndex = 0;

		for(BYTE i = 0; i < 4; i++)
		{
			if(pUser->m_EnumChar[i].dwCharacterIndex!=0)
			{
				if(__strcmp(result.szCharacterName, pUser->m_EnumChar[i].szName)==0)
				{
					byIndex = i;
					break;
				}
			}
		}

		if(byIndex>=4)
		{
			WSTC_DELETE_CHARACTER_RESULT pPacket;
			pPacket.bSuccess		= FALSE;
			pPacket.nResultValue	= 11;
			SendToUser(pUser, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}
		else
		{					
			if(pUser->m_EnumChar[byIndex].sCreateTime.year==0 || pUser->m_EnumChar[byIndex].sCreateTime.month==0 || pUser->m_EnumChar[byIndex].sCreateTime.day==0)
			{
				ThrowDeleteCharQuery(pUser, result.szCharacterName);
			}
			else
			{
				CTime cCreateTime(pUser->m_EnumChar[byIndex].sCreateTime.year, pUser->m_EnumChar[byIndex].sCreateTime.month, pUser->m_EnumChar[byIndex].sCreateTime.day, pUser->m_EnumChar[byIndex].sCreateTime.hour, pUser->m_EnumChar[byIndex].sCreateTime.minute, pUser->m_EnumChar[byIndex].sCreateTime.second);
				CTime cCurrentTime = CTime::GetCurrentTime();

				CTimeSpan ts = cCurrentTime - cCreateTime;

				if(ts.GetTotalHours()>=24)
				{
					ThrowDeleteCharQuery(pUser, result.szCharacterName);
				}
				else
				{
					WSTC_DELETE_CHARACTER_RESULT pPacket;
					pPacket.bSuccess		= FALSE;
					pPacket.nResultValue	= 10;
					SendToUser(pUser, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
				}
			}					
		}
	}
	else	//주민번호 틀림 
	{
		WSTC_DELETE_CHARACTER_RESULT	FailPacket;
		FailPacket.bSuccess = FALSE;
		FailPacket.nResultValue = 5;		//주민등록번호 틀림 
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
}


void QueryTypeCreateCharInitBinary(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (!pUser)	return;
	
	if(pResult->nError >= 0)
	{
		WSTC_CREATE_CHARACTER_SUCCESS	packet;
		LPBASE_CLASS_INFO	pInfo = g_pUserTable->GetBaseClassInfo(pUser->m_bClass);

		packet.dwStr = pInfo->iSTR;
		packet.dwVit = pInfo->iVIT;
		packet.dwDex = pInfo->iDEX;
		packet.dwInt = pInfo->iINT;
		packet.dwEgo = pInfo->iEGO;
		SendToUser( pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
	else
	{
		pUser->m_bClass				= 0;
		pUser->m_bSelectedChrIndex	= 0;

		// 생성 실패이니 CharacterName 지우자 
		memset(pUser->GetSelectedCharacter()->szName, 0, MAX_CHARACTER_NAME_LENGTH);	

		WSTC_CREATE_CHARACTER_FAIL	FailPacket;
		SendToUser( pUser, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
	}
}


void QueryTypeGMCheck(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_NORMAL, "WorldUser Pointer is NULL in QueryTypeGMCheck!");
		return;
	}

	if(pResult->Query.select.dwRowCount == 0)
	{
		WSTC_INVALID_GM packet;
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
	
	// DB에 등록된 GM IP
	char szRegisteredIP[16]={0,};
	memcpy(szRegisteredIP, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);
	
	// 현재 접속한 Client의 IP
	char szIp[16]={0,};	WORD wPort=0;
	g_pNet->GetUserAddress(pUser->GetConnectionIndex(), szIp, &wPort);

	// 다른 경우 GM이 아닌걸로 판정 
	if(!__strcmp(szIp, szRegisteredIP))
	{
		WSTC_GM_LOGIN_SUCCESS packet;
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		WSTC_INVALID_GM packet;
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}	
}


void QueryTypeBlockTitle(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_NORMAL, "WorldUser Pointer is NULL in QueryTypeBlockTitle!");
		return;
	}

	// 경고나 제재상태이기는 하나 DB에 Block 사유가 없는 경우 
	if(pResult->Query.select.dwRowCount == 0)
	{
		WSTC_LOGIN_FAIL packet;
		packet.bResult = LOGIN_FAIL_USER_BLOCK;
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
		pUser->m_dwStatus = 0;
		memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
		g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
		g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
		return;
	}

	BLOCK_TITLE BlockTitle;
	memcpy(&BlockTitle, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

	if(BlockTitle.dwType == 2)	//로그인 제재 상태 
	{
		WSTC_BLOCK_ACCOUNT	packet;
		memcpy(packet.szReason, BlockTitle.szReason, strlen(BlockTitle.szReason) );
		packet.bLen = (BYTE)strlen(BlockTitle.szReason);
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}

	pUser->m_dwStatus = 0;
	memset(pUser->m_szLoginID, 0, MAX_ID_LENGTH);
				
	g_pUserTable->FreeUser(pUser);							//Alloc 했던 유저 메모리 풀에서 해제 
	g_pNet->SetUserInfo(pUser->GetConnectionIndex(), NULL);
}


void QueryTypeWarningTitle(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_NORMAL, "WorldUser Pointer is NULL in QueryTypeWarningTitle!");
		return;
	}

	if(pResult->Query.select.dwRowCount > 0)		//경고가 있는경우 
	{
		BLOCK_TITLE WarningTitle;
		memcpy(&WarningTitle, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

		WSTC_WARNING_TITLE	packet;
		memcpy(packet.szReason, WarningTitle.szReason, strlen(WarningTitle.szReason) );
		packet.bLen = (BYTE)strlen(WarningTitle.szReason);
		SendToUser(pUser, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}
