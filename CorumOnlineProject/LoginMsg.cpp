#include "stdafx.h"
#include "LoginProcess.h"
#include "InitGame.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "GameControl.h"
#include "SpriteManager.h"
#include "develop.h"
#include "ObjectManager.h"
#include "HashTable.h"
#include "../CommonServer/AbyssHash.h"
#include "GameDefaultWnd.h"
#include "CharSelectProcess.h"
#include "WorldProcess.h"
#include "DungeonProcess.h"
#include "GlobalSoundHandles.h"
#include "message.h"
#include "GameMenuWnd.h"
#include "ErrorWnd.h"
#include "IMEEdit.h"
#include "InputManager.h"
#include "NetworkClient.h"
#include "BlockWnd.h"
#include "LoginMsg.h"
#include "ChinaBillingRemainTime.h"
#include "PacketEncrypt.h"

void CmdLoginFail( char* pMsg, DWORD dwLen )
{
	WSTC_LOGIN_FAIL*	pPacket			= (WSTC_LOGIN_FAIL*)pMsg;

	// 중국 빌링 서버 관련 로그인 실패 처리!!
	if(pPacket->bResult >= 0xf0)
	{
		OnChinaBillingLoginFail(pPacket->bResult);
		return;
	}

	CGameDefaultWnd*	pGameDefaultWnd = CGameDefaultWnd::GetInstance();

	g_pGVLogin->bIsLoging = FALSE;

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
	{
		g_NetMarble.bLoging = FALSE;
	}

	//0: 비번틀림, 2: 없는아이디 , 3: 아이디사용중, 4: 서버 최대인원 초과 
	//1: 로그인 성공(사용안함)
	switch(pPacket->bResult)
	{
	case LOGIN_FAIL_NO_PAY:
		{
			//"고객님의 결제내역이 없습니다."	//
			//"현재 상용서비스 중입니다."		//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE832].szMessage, g_Message[ETC_MESSAGE833].szMessage, "", 402, 459, 2, TRUE);									
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}
		break;
	case LOGIN_FAIL_EJECT_USER:		//탈퇴한 회원 
		{
			// "탈퇴한 회원입니다."	//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE834].szMessage, "", "", 402, 459, 1, TRUE);								
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}			
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}
		break;	
	case LOGIN_FAIL_SQL_ERROR:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE592].szMessage, "", "", 402, 459, 1, TRUE); // "SQL 오류입니다."				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);			
		}
		break;
	case LOGIN_FAIL_INVALID_PASSWORD:	
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE318].szMessage, "", "", 402, 459, 1, TRUE);	// MSG_ID : 318 ; 비밀번호가 틀렸습니다.			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);
		}		
		break;
	case LOGIN_FAIL_NO_ID:	
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE317].szMessage, "", "", 402, 459, 1, TRUE); // MSG_ID : 317 ; 등록되지 않은 계정입니다.			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}			
		break;
	case LOGIN_FAIL_OVERLAPPED_ID_ANOTHER_SERVER:
		{
			char szServerSetName[32]	= {0,};
			char szMsg1[0xff]			= {0,};

			switch(pPacket->dwExData)
			{
			case 1:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE216].szMessage);	break; // "카이엔"
			case 2:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE217].szMessage);	break; // "이슈리아"
			case 3:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE218].szMessage);	break; // "자이피"
			case 4:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE219].szMessage);	break; // "아이슈마"
			case 5:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE220].szMessage);	break; // "아마테라스"
			case 6:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE221].szMessage);	break; // "아누비스"
			case 7:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE222].szMessage);	break; // "바하무트"
			case 8:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE223].szMessage);	break; // "베논"
			case 9:	lstrcpy(szServerSetName, g_Message[ETC_MESSAGE224].szMessage);	break; // "엘레아"
			}

			wsprintf(szMsg1, g_Message[ETC_MESSAGE277].szMessage, szServerSetName); // "아이디가 %s 서버에서 사용중 "

			// "로그인 실패"
			// "입니다. 해당서버로 로그인을 시도"
			// "하여 접속을 끊을 수 있습니다."
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE276].szMessage, szMsg1, g_Message[ETC_MESSAGE275].szMessage, g_Message[ETC_MESSAGE274].szMessage,  402, 459, 3, TRUE); 			
			pGameDefaultWnd->m_byType = 167;
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
		}
		break;
	case LOGIN_FAIL_OVERLAPPED_ID:	
		{
			if(GetGameStatus()==UPDATE_GAME_CHAR_SELECT)
			{				
				// "중복 로그인이 되었습니다." , "5초 후에 종료하겠습니다."

				CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();				
				pErrorWnd->SetError(g_Message[ETC_MESSAGE593].szMessage, g_Message[ETC_MESSAGE594].szMessage , "", 2);				
				pErrorWnd->SetActive(TRUE);
				pErrorWnd->m_byType		= 1;
				pErrorWnd->m_byTime		= 5;
				pErrorWnd->m_dwCurTime	= timeGetTime();
				pErrorWnd->m_dwPevTime	= timeGetTime();				
			}
			else
			{
				// "현재 사용중인 계정이거나 비정상종료로", "인해 접속이 남아있습니다.", "프로그램 종료후 재접속 해주세요."
				// MSG_ID : 325 ; 현재 사용중인 계정입니다.
				pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE595].szMessage, g_Message[ETC_MESSAGE596].szMessage, g_Message[ETC_MESSAGE597].szMessage, 402, 459, 3, TRUE);																			
				pGameDefaultWnd->SetActive();
				pGameDefaultWnd->m_bExit		= TRUE;
				pGameDefaultWnd->m_dwCurTime	= timeGetTime();
				pGameDefaultWnd->m_dwPevTime	= timeGetTime();
				LoginInterface(FALSE);

				if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
				{
					pGameDefaultWnd->m_bExit = TRUE;
					return;
				}
				g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
				g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
				g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);				
			}
		}		
		break;
	case LOGIN_FAIL_USER_OVERFLOW:	
		{
			// "선택하신 서버는 인원이 초과하였"	//
			//"습니다. 잠시후 다시 접속해 주시기",	//
			//"바랍니다.",							//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE326].szMessage, g_Message[ETC_MESSAGE598].szMessage, g_Message[ETC_MESSAGE328].szMessage, 402, 459, 3, TRUE);				 							
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}				
		break;				
	case LOGIN_FAIL_USER_NOT_TEST_ID:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE329].szMessage, g_Message[ETC_MESSAGE330].szMessage, "", 402, 459, 2, TRUE);	// MSG_ID : 329, 330			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}			
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
		}
		break;
	case LOGIN_FAIL_INVALID_VERSION:
		{

#if IS_JAPAN_LOCALIZING()
			// MSG_ID : 331 ; 이전버전의 클라이언트입니다., 332 ; 새버젼의 클라이언트로 갱신해주세요!
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage
				, g_Message[ETC_MESSAGE331].szMessage, g_Message[ETC_MESSAGE332].szMessage
				, g_Message[ETC_MESSAGE249].szMessage,402, 459, 2, TRUE);	
#else
			// MSG_ID : 331 ; 이전버전의 클라이언트입니다., 332 ; 새버젼의 클라이언트로 갱신해주세요!
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage
				, g_Message[ETC_MESSAGE331].szMessage, g_Message[ETC_MESSAGE332].szMessage, "",402, 459, 2, TRUE);	
#endif				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);			
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}		
		break;		
	case LOGIN_FAIL_NOW_TRY_TO_LOGIN:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE333].szMessage, g_Message[ETC_MESSAGE334].szMessage, "", 402, 459, 2);	// MSG_ID : 333 ; Now try to login..., 334 ; Please Wait
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;

	case LOGIN_FAIL_USER_BLOCK:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE335].szMessage, "", "",402, 459, 1, TRUE);	// MSG_ID : 334 ; 사용중지중인 계정입니다.			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;
		
			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}		
		break;		
	case LOGIN_FAIL_NEXT:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE337].szMessage, "", "",402, 459, 1, TRUE);	// MSG_ID : 337 ; 다음 PC방 오픈부터 접속가능합니다.			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_ETC:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE338].szMessage, g_Message[ETC_MESSAGE339].szMessage, "",402, 459, 2, TRUE);	//MSG_ID : 338 ; 로그인에 실패했습니다., 339 ; 잠시후 다시 시도해주세요!			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_CHARACTER_OVERLAPPED:
		{
			// "캐릭터 중복 오류입니다."	//
			// "운영자에게 문의하세요!"		//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE599].szMessage, g_Message[ETC_MESSAGE600].szMessage, "",402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_INVALID_CHARACTER_INDEX:
		{
			// "캐릭터 인덱스 오류입니다."	//
			//"운영자에게 문의하세요!"		//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE601].szMessage, g_Message[ETC_MESSAGE600].szMessage, "",402, 459, 3, TRUE);
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
			g_bExitChk = TRUE;

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_14_OVER:
		{
			// "14세미만 임시가입자로 부모"		//
			// "동의 처리 후 게임가능합니다."	//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE602].szMessage, g_Message[ETC_MESSAGE603].szMessage, "", 402, 459, 3, TRUE);				
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);
			
			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;		
	case LOGIN_FAIL_NOT_MIRI_USER:	//미리 생성 대상자 
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
				g_Message[ETC_MESSAGE604].szMessage, // "캐릭터를 미리 생성할 수 있는 "
				g_Message[ETC_MESSAGE605].szMessage, // "대상자가 아닙니다."
				"", 402, 459, 3, TRUE);	
			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;		
	case LOGIN_FAIL_18_OVER:	//18세 이상 유저 
		{
			// "이서버는 성인전용 서버로 "			//
			// "만18세 이상 유저만 사용가능합니다."	//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE606].szMessage, g_Message[ETC_MESSAGE607].szMessage, "", 402, 459, 2, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_NOT_REAL_NAME:
		{
			//"아직 실명인증을 받지 않으셨습니다.",			//
			//"홈페이지에서 실명인증 후 실행 가능합니다.",	//
			// "http://www.corumonline.co.kr",				//
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE608].szMessage, g_Message[ETC_MESSAGE609].szMessage, g_Message[ETC_MESSAGE610].szMessage, 402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_NOT_BETA_TESTER:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE926].szMessage, g_Message[ETC_MESSAGE927].szMessage, "", 402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_EJECT_USER_JAPAN:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE928].szMessage, g_Message[ETC_MESSAGE600].szMessage, "", 402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	case LOGIN_FAIL_SECESSION_USER:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE929].szMessage, g_Message[ETC_MESSAGE600].szMessage,  "", 402, 459, 3, TRUE);	
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);

			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		break;
	default:
		{
			// Show Error Code by minjin. 2004. 10. 29.
			char szTemp[1024] = {0,};
			wsprintf( szTemp, "[Error Code : %d]", pPacket->bResult );

			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, "Cannot login with some reason", szTemp, "", 402, 459, 3, TRUE);			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);


			// 비밀번호에 Focus 주고 깜빡이게 //
			g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_PASSWORD);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)							
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
	}

	// When Login is Failed..
	// Disconnect from Login Server
	// for Packet Encryption with deepdark
	// 2004. 08. 20.
	// Begin - minjin

	// If User Failed to Login, then
	// Server Side User Object Reset..
	// So, Key must be Reseted

#ifdef __PACKET_ENCRYPTION
			ResetEncryptionKey();
#endif

	// End - minjin

}

void CmdLoginSuccess( char* pMsg, DWORD dwLen )
{	
	WSTC_LOGIN_SUCCESS* pPacket			= (WSTC_LOGIN_SUCCESS*)pMsg;
	CGameDefaultWnd*	pGameDefaultWnd = CGameDefaultWnd::GetInstance();	
	
	int		i		= 0;
	WORD	wIndex	= 0;
	
	g_bAdultMode = pPacket->bAdultMode;
	g_byCharacterAllCount	= pPacket->bNum;
	
	pGameDefaultWnd->SetActive(FALSE);
	
#ifndef DEVELOP_MODE
	g_pGVLogin->bIsLoging = FALSE;	
#endif

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
	{
		g_NetMarble.bLoging = FALSE;
	}

	V2_SPRITE* pSprOKOver = g_pSprManager->GetSprite(BUTTON_OK_OVER);

	for(i = 0; i < pPacket->bNum; i++)
	{
		if(IsEmptyString(pPacket->Character[i].szName))
			continue;

		wIndex = WORD(pPacket->Character[i].wChrNum - 1);

		if(wIndex > 3 || wIndex > 256)
		{			
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE322].szMessage, "", "", 402, 459, 1, TRUE);	// MSG_ID : 322 ; Invalid Character Index Received!			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);			
			PostQuitMessage(0);
	
			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
			{
				g_bIsRunning = FALSE;
				return;
			}
		}

		if(!IsEmptyString(g_pGVLogin->ChrSelectInfo[ wIndex ].ReceivedChrInfo.szName))
		{			
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE323].szMessage, "", "", 402, 459, 1, TRUE);	// MSG_ID : 323 ; Character Overlapped!			
			pGameDefaultWnd->SetActive();
			LoginInterface(FALSE);						
			PostQuitMessage(0);

			if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
			{
				pGameDefaultWnd->m_bExit = TRUE;
				return;
			}
		}
		memcpy(&g_pGVLogin->ChrSelectInfo[ wIndex ], &pPacket->Character[i], sizeof(RS_CharacterLogInfo) );		
		g_pGVLogin->ChrSelectInfo[ wIndex ].bByItemType = BYTE((pPacket->Character[i].wRHandID/ITEM_DISTRIBUTE) + 1);		
	}
	pSprOKOver->ShowSprite(FALSE);
	SetGameStatus( UPDATE_GAME_CHAR_SELECT );
	return;
}

void CmdLoginFailWithReason( char* pMsg, DWORD dwLen )
{
	WSTC_BLOCK_ACCOUNT*	pPacket = (WSTC_BLOCK_ACCOUNT*)pMsg;
	CBlockWnd*			pWnd	= CBlockWnd::GetInstance();

	g_pGVLogin->bIsLoging = FALSE;

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)	//제재상태일경우 
	{
		g_NetMarble.bLoging = FALSE;
	}

	pWnd->m_bInfoType = UPDATE_GAME_CHAR_SELECT;

	char*	szTemp				= 0;
	char	szReason[1024]		= {0,};	
	int		nMaxLineCharacter	= 34;

	memset(szReason, 0, 1024);
	memcpy(szReason, pPacket->szReason, pPacket->bLen);

	szTemp	= szReason;

	int nSize	= pPacket->bLen;
	int nCount	= 1;
	
	for(int i = 0; i < MAX_BLOCK_MESSAGE_LINE_COUNT; i++)
		memset(pWnd->szMessageLine[i], 0, 255);

	lstrcpy(pWnd->szMessageLine[0], g_Message[ETC_MESSAGE923].szMessage); // "<< 귀하는 제재중인 계정입니다. >>"

	while(nSize > 0)
	{
		if(nSize > nMaxLineCharacter)
		{
			for(;;)
			{
				if(nSize > nMaxLineCharacter)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+nMaxLineCharacter, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev);
					
					memset(pWnd->szMessageLine[nCount], 0, 255);
					__lstrcpyn(pWnd->szMessageLine[nCount], szTemp, nSize);
					
					szTemp = szPrev;
					nCount++;
					nSize = lstrlen(szTemp);
				}
				else
				{
					// 구한 문자열을 채팅 라인에 쓰고, 길이를 기록한다.
					lstrcpy(pWnd->szMessageLine[nCount], szTemp);
					nCount++;								
					nSize = 0;
					break;
				}
			}			
		}
		else
		{	
			memset(pWnd->szMessageLine[nCount], 0, nMaxLineCharacter);
			__lstrcpyn(pWnd->szMessageLine[nCount], szTemp, lstrlen(szTemp));	
			nCount++;
			nSize = 0;
		}		
	}
	
	pWnd->m_fPosX		= 402;
	pWnd->m_fPrevPosX	= pWnd->m_fPosX;
	pWnd->m_fPosZ		= 330;
	pWnd->m_fPrevPosZ	= pWnd->m_fPosZ;
	pWnd->SetActive();

	if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)			
		return;
	
	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
	g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);	
}

// added by deepdark. 로그인 서버에서 암호화 서버 키를 전송했다...
void CmdEncryptionKey(char* pMsg, DWORD dwLen)
{
	WSTC_ENCRYPTION_KEY* pPacket = (WSTC_ENCRYPTION_KEY*)pMsg;

	BYTE btEncryptionKey[ENCRYPT_KEY_LEN]={0,};
	CalculateKey(GetClientKey(), pPacket->byServerKey, btEncryptionKey);

/*
//	Output Key Strings For Packet Encryption
//	2004. 08. 20.
//	minjin with deepdark

	char szTemp[4096];
	LPBYTE szTempClient = GetClientKey();

	sprintf(szTemp, "[minjin with deepdark] Client Key : %d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n",
		szTempClient[0],	szTempClient[1],	szTempClient[2],	szTempClient[3],	szTempClient[4],
		szTempClient[5],	szTempClient[6],	szTempClient[7],	szTempClient[8],	szTempClient[9]);
	OutputDebugString(szTemp);

	sprintf(szTemp, "[minjin with deepdark] Server Key : %d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n",
		pPacket->byServerKey[0],	pPacket->byServerKey[1],	pPacket->byServerKey[2],	pPacket->byServerKey[3],	pPacket->byServerKey[4],
		pPacket->byServerKey[5],	pPacket->byServerKey[6],	pPacket->byServerKey[7],	pPacket->byServerKey[8],	pPacket->byServerKey[9]);
	OutputDebugString(szTemp);

	sprintf(szTemp, "[minjin with deepdark] Encrypted Key : %d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n",
		btEncryptionKey[0],		btEncryptionKey[1],		btEncryptionKey[2],		btEncryptionKey[3],
		btEncryptionKey[4],		btEncryptionKey[5],		btEncryptionKey[6],		btEncryptionKey[7],
		btEncryptionKey[8],		btEncryptionKey[9],		btEncryptionKey[10],	btEncryptionKey[11],
		btEncryptionKey[12],	btEncryptionKey[13],	btEncryptionKey[14],	btEncryptionKey[15]);
	OutputDebugString(szTemp);

//*/
	
	SetEncryptionKey(btEncryptionKey);
}