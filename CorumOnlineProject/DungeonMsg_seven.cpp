#include "DungeonMsg_seven.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "UserInterface.h"
#include "message.h"
#include "DungeonTable.h"
#include "Chat.h"
#include "CodeFun.h"
#include "CorumResource.h"
#include "DungeonMsg.h"
#include "RequestMatchWnd.h"
#include "InputManager.h"
#include "SevenWork.h"
#include "GroupWnd.h"
#include "RivalGuildWar.h"

void InitPacketProcDungeonSeven()
{
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_DUNGEON_EMOTICON ]				= CmdEmoticon;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_DUNGEON_REQUEST_MATCH_MAN2MAN]	= CmdDungeonRequestMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_DUNGEON_REPLY_MATCH_MAN2MAN]		= CmdDungeonReplyMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_READY_MATCH_MAN2MAN]				= CmdReadyMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_START_MATCH_MAN2MAN]				= CmdStartMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_END_MATCH_MAN2MAN ]				= CmdEndMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_DSTC::CMD_MATCH_USER_INFO ]				= CmdMatchUserInfo;	
}

void CmdEmoticon( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_EMOTICON * pEmoticon = (DSTC_DUNGEON_EMOTICON *)pMsg;
	char	szBuf[255] = {0,};

	CUser* pUser = g_pUserHash->GetData(pEmoticon->dwUserIndex);
	if (!pUser)	return;
	
	if (!pUser->m_hPlayerEmoticon.pHandle)
	{
		pUser->m_hPlayerEmoticon.pDesc				= AllocObjDesc();
		pUser->m_hPlayerEmoticon.pDesc->bType		= OBJECT_TYPE_EFFECT;
		pUser->m_hPlayerEmoticon.pDesc->ObjectFunc	= GXEmoticonProc;
		pUser->m_hPlayerEmoticon.pDesc->pInfo		= pUser;

		lstrcpy(szBuf, "e0202000.chr");
		pUser->m_hPlayerEmoticon.pHandle = 
			CreateHandleObject(GetFile(szBuf, DATA_TYPE_EFFECT), GXPlayerPROC, pUser->m_hPlayerEmoticon.pDesc, GXOBJECT_CREATE_TYPE_EFFECT);
	}

	GXSetPosition(pUser->m_hPlayerEmoticon.pHandle, &pUser->m_v3CurPos, FALSE, TRUE);
		
	g_pExecutive->SetAlphaFlag(pUser->m_hPlayerEmoticon.pHandle, 255);
	ShowObject(pUser->m_hPlayerEmoticon.pHandle);
	::SetAction(pUser->m_hPlayerEmoticon.pHandle, pEmoticon->bWhatEmoticon, ACTION_ONCE);
}


// ===================
// 상대방이 1:1 대결을 요청하였다.
// ===================
void CmdDungeonRequestMatchM2M(char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN	* pPacket = (DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN *) pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwRequestUserIndex);
	if (!pUser)	return;
	if (pUser->m_bMatching == TRUE)
	{
		// 대전중이다
		return;
	}

	// 신청자의 인덱스를 가지고 있어야 대답할때 잘 보낼수있다.
	CRequestMatchWnd * pRequestMatchWnd = CRequestMatchWnd::GetInstance();
	pRequestMatchWnd->m_dwRequestMatchUserIndex = pPacket->dwRequestUserIndex;

	__lstrcpyn(pRequestMatchWnd->m_szName, pUser->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
	pRequestMatchWnd->SetActive(TRUE);

	CUserInterface::GetInstance()->m_bMatch = FALSE;	// MATCH 버튼 풀어준다.
	CUserInterface::GetInstance()->ClearPVPFlag();	// PVP 관련 초기화!!!
}

// ===================
// 상대방의 대답이 유저에게 날라왔다. 
// ===================
void CmdDungeonReplyMatchM2M(char* pMsg, DWORD dwLen)
{
	DSTC_DUNGEON_REPLY_MATCH_MAN2MAN * pPacket = (DSTC_DUNGEON_REPLY_MATCH_MAN2MAN *) pMsg;
	char		szBuf[255] = {0,};
	CRequestMatchWnd * pRequestMatchWnd = CRequestMatchWnd::GetInstance();
	CGroupWnd*	pGroupWnd = CGroupWnd::GetInstance();
	
	switch(pPacket->bReplyCode)
	{
	case 0:	// 기타 문제 
		break;
	case 1: // 대결 
		g_pMainPlayer->SetAction(MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );		
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
		SendStopPacket();
		g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
		
		g_pMainPlayer->m_dwMatchRecords += 1;

		CUserInterface::GetInstance()->m_nPK = __PK_MODE__;
		g_pMainPlayer->m_bMatching = TRUE;	
		g_pMainPlayer->m_dwMatchUserIndex = pPacket->dwMatchUserIndex;

		if (g_pMainPlayer->m_dwUserIndex != pPacket->dwMatchUserIndex)
		{
			CUser * pMatchUser = (CUser *)g_pUserHash->GetData(pPacket->dwMatchUserIndex);
			if (pMatchUser)
			{
				pMatchUser->m_bMatching = TRUE;
			}
		}

		// sung-han 2005-02-15 pvp를 할경우 길드전매칭 취소를 보낸다--------------------------------
		if(pGroupWnd->m_bGuildWarMatchEnable)
		{
			pGroupWnd->m_bGuildWarMatchEnable = FALSE;
			pGroupWnd->m_bGuildWarPacketSendEnable = FALSE;
			TEAM_MATCH_PACKET TeamMatch;
			TeamMatch.Body.Condtion.iMatchType = M_T_NONE;
			TeamMatch.Body.Condtion.iMatchGuildNumber = 0;
			TeamMatch.Body.Condtion.iMatchGuildType = T_T_NONE;
			Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_REMOVE);
		}
		CUserInterface::GetInstance()->ClearPVPFlag();	// PVP 관련 초기화!!!
		//--------------------------------------------------------------------------------------------------
		goto Not_Init_Match;
	case 2: // 거부
		{
			CUser* pUser = g_pUserHash->GetData(pPacket->dwMatchUserIndex);
			if (!pUser)	return;

			wsprintf(szBuf, g_Message[ETC_MESSAGE531].szMessage, pUser->m_szName); // "%s 님이 대결을 거부 하셨습니다!"
			DisplayMessageAdd(szBuf, 0xFFFF0000);

			g_pMainPlayer->m_bMatching = FALSE;		
		}
		break;
	case 3:	//상대방이 대결중
		{
			CUser* pUser = g_pUserHash->GetData(pPacket->dwMatchUserIndex);
			if (!pUser)	return;

			wsprintf(szBuf, g_Message[ETC_MESSAGE532].szMessage, pUser->m_szName); // "%s 님은 대전중입니다!"
			DisplayMessageAdd(szBuf, 0xFFFF0000);
			
			goto Not_Init_Match;
		}
		break;
	case 4: // 접선중
		{
			// "%s 님은 대전 접선중입니다!"
			wsprintf(szBuf, g_Message[ETC_MESSAGE533].szMessage, g_pMainPlayer->m_szName); 
			DisplayMessageAdd(szBuf, 0xFFFF0000);			
		}
		break;
	case 5: // 대전장소가 꽉찼다.
		{
			// "대전장소가 꽉찼습니다. 잠시후에 대전을 신청하시기 바랍니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE534].szMessage, 0xFFFF0000); 

		}
		break;
	case 6:	// 교환중에 대결을 하려고한다.
		{
			// "교환중일 때는 대결을 하실 수 없습니다. 교환 완료후 계속하시기 바랍니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE535].szMessage, 0xFFFF0000); 
		}
		break;
	case 7:
		{
			//"노점중일 때는 대결을 하실 수 없습니다. 노점 종료후 계속하시기 바랍니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1052].szMessage, 0xFFFF0000);
		}
		break;
	case 8:
		{
			//"앉기중일 때는 대결을 하실 수 없습니다. 앉기 종료후 계속하시기 바랍니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1053].szMessage, 0xFFFF0000);
		}
		break;
	case 9:
		{
			//"상대방이 노점중일 때는 대결을 하실 수 없습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1054].szMessage, 0xFFFF0000);
		}
		break;
	case 12:
		{
			//"상대방이 앉기중일 때는 대결을 하실 수 없습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1055].szMessage, 0xFFFF0000);
		}
		break;

	case CUser::MATCH_STATUS_USER_REQUEST_AFTER_USER_GOTO_WORLD:
	default: // 정해지지 않은 에러
		{
			char	szMsg[255] = {0,};
			wsprintf(szMsg, g_Message[ETC_MESSAGE536].szMessage, pPacket->bReplyCode); // "일대일 대전 에러 코드 : %d"
			DisplayMessageAdd(szMsg, 0xFFFF0000);
		}
		break;
	}

	// 문제 발생시 초기화 
	pRequestMatchWnd->SetActive(FALSE);
	g_pMainPlayer->m_bMatching = FALSE;
	CRequestMatchWnd::GetInstance()->m_dwRequestMatchUserIndex = 0;

	if(pPacket->bReplyCode>=7 && pPacket->bReplyCode<=9 && pPacket->bReplyCode==12)
	{
		g_pMainPlayer->SetAction(MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );		
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
	}	

Not_Init_Match:
	return;
}

// ========
// 대결 준비 
// ========
void CmdReadyMatchM2M(char* pMsg, DWORD dwLen)
{
	DSTC_READY_MATCH_MAN2MAN * pPacket = (DSTC_READY_MATCH_MAN2MAN *)pMsg;
	char	szBuf[255] = {0,};

	CUser * pReadyUser = g_pUserHash->GetData(pPacket->dwReadyUserIndex);
	if ( !pReadyUser )
	{
		return;	
	}

	if( pReadyUser->m_dwUserIndex == g_pMainPlayer->m_dwUserIndex )
	{
		CGuildWarStatusWnd::GetInstance()->SetActive();
		CGuildWarStatusWnd::GetInstance()->SetMark_Ready();
	}
	
	wsprintf(szBuf, g_Message[ETC_MESSAGE537].szMessage, pReadyUser->m_szName); // "%s 님이 대결 준비가 되었습니다."
	DisplayMessageAdd(szBuf, 0xFF00FF1E);	
}

// ========
// 대결 시작
// ========
void CmdStartMatchM2M(char* pMsg, DWORD dwLen)
{
	DSTC_START_MATCH_MAN2MAN * pPacket = (DSTC_START_MATCH_MAN2MAN *)pMsg;

	CUser * pUser = (CUser *)g_pUserHash->GetData(pPacket->dwMatchUserIndex);
	
	CUserInterface * pUserInterface = CUserInterface::GetInstance();
	if (pUser)
	{
		pUserInterface->m_bMatchUserHPShowFlag = TRUE;
		pUserInterface->m_bMatchUserHPShowEnable = TRUE;
		pUserInterface->m_pMatchUser = pUser;
		g_pMainPlayer->m_dwMatchUserIndex = pUser->m_dwUserIndex;

		g_MatchInfo.SetDisplayMatchFlag(TRUE);
		g_MatchInfo.SetMatchTimeTick(pPacket->dwMatchTimeTick);

		pUserInterface->OpenUserBar(pUser->m_szName, 100, 100);	// 일단 꽉채운다.		
	}

	// PVP 유저 둘 모두 준비가 완료된다면
	// 움직일 수 있게 만들어 준다.
	// Start도 몇 초간 켜주고,
	CUserInterface::GetInstance()->IncPVPCount();	
	CUserInterface::GetInstance()->IncPVPCount();	

	CGuildWarStatusWnd::GetInstance()->SetMark_Start();

	SendPKMode(true);

	DisplayMessageAdd(g_Message[ETC_MESSAGE538].szMessage, 0xFF00FF1E); // "대결시작 GG!!"
}

// =========
// 대결 종료
// =========
void CmdEndMatchM2M(char* pMsg, DWORD dwLen)
{
	DSTC_END_MATCH_MAN2MAN * pPacket = (DSTC_END_MATCH_MAN2MAN *) pMsg;

	CUserInterface::GetInstance()->CloseUserBar();
	CUserInterface::GetInstance()->m_bMatchUserHPShowFlag = FALSE;
	CUserInterface::GetInstance()->m_bMatchUserHPShowEnable = FALSE;
	CUserInterface::GetInstance()->m_szMatchUserName[0] = '\0';
	CUserInterface::GetInstance()->m_pMatchUser = NULL;
	

	// 대결 정보를 지워준다.
	g_pInputManager->ClearInput(INPUT_BUFFER_13);
	g_pInputManager->ClearInput(INPUT_BUFFER_14);
	
	g_MatchInfo.SetDisplayMatchFlag(FALSE);
	g_MatchInfo.Init();

//	CUser * pWinner = NULL, * pLoser = NULL;
	char	szBuf[255] = {0,};
	if (pPacket->bJudgmentCode == 0)//
	{ 
		// KO		 
		wsprintf(szBuf, g_Message[ETC_MESSAGE539].szMessage, // // "%s 님과 %s 님의 대결에서 %s 님께서 KO승을 거뒀습니다.",
						pPacket->szWinnerName, pPacket->szLoserName, pPacket->szWinnerName);
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}
	else if (pPacket->bJudgmentCode == 1)
	{ 
		// 판정 
		wsprintf(szBuf, g_Message[ETC_MESSAGE540].szMessage, // "%s 님과 %s 님의 대결에서 %s 님께서 판정승을 거뒀습니다."
						pPacket->szWinnerName, pPacket->szLoserName, pPacket->szWinnerName);
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}
	else if (pPacket->bJudgmentCode == 2)
	{ 
		// 무승부 
		wsprintf(szBuf, g_Message[ETC_MESSAGE541].szMessage, // "%s 님과 %s 님의 대결에서 무승부 판정 났습니다."
						pPacket->szWinnerName, pPacket->szLoserName, pPacket->szWinnerName);
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}
	else if (pPacket->bJudgmentCode == 3)
	{
		// 비정상 종료 
		wsprintf(szBuf, g_Message[ETC_MESSAGE542].szMessage, // "%s 님과 %s 님의 대결에서 %s 님께서 승리하셨습니다.(비정상)"
			pPacket->szLoserName, pPacket->szWinnerName, pPacket->szWinnerName);
		DisplayMessageAdd(szBuf, 0xFF00FF1E);
	}

	if (!__strcmp(pPacket->szWinnerName, g_pMainPlayer->m_szName) )
	{ 
		// 대전 당사자가 승리 
		if (2 != pPacket->bJudgmentCode)
		{ 
			// 무승부가 아니다.
			g_pMainPlayer->m_dwMatchWin += 1;	
		}
	} 
	else
	if (!__strcmp(pPacket->szLoserName, g_pMainPlayer->m_szName) )	
	{ 
		// 대전 당사자가 패배
		if (2 != pPacket->bJudgmentCode)
		{ 
			// 무승부가 아니다.
			g_pMainPlayer->m_dwMatchLose += 1;	
		}
	}

	CGuildWarStatusWnd::GetInstance()->SetActive(FALSE);
}


void  CmdMatchUserInfo(char* pMsg, DWORD dwLen)
{
	CUserInterface * pUserInterface = CUserInterface::GetInstance();
	DSTC_MATCH_USER_INFO * pPacket = (DSTC_MATCH_USER_INFO *)pMsg;
	
	CUser * pUser = g_pUserHash->GetData(pPacket->dwMatchUserIndex);
	if (NULL != pUser)
	{
		if (pUserInterface->m_pMatchUser == pUser)
		{
//			pUserInterface->OpenUserBar(pUser->m_szName, pPacket->dwHP, pPacket->dwMaxHP);//구 작업버전 
//	sung-han 2004-12-30 : pvp 에서 캐릭터 에너지바와 엘레맨탈 에너지 바가 겹치는 부분 처리
//-----------------------------------------------------------------------------------------
			pUserInterface->m_fUserHp		= (float)pPacket->dwHP;
			pUserInterface->m_fUserMaxHp	= (float)pPacket->dwMaxHP;
//-----------------------------------------------------------------------------------------
			
		}
	}
}