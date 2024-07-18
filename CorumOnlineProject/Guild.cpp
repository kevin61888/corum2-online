//==============================================================//
// Code by Jang.									2003.08.01	//
//==============================================================//
#include "Guild.h"
#include "GameControl.h"
#include "../CommonServer/CommonHeader.h"
#include "User.h"
#include "Message.h"
#include "HashTable.h"
#include "Interface.h"
#include "GroupWnd.h"
#include "GuildWnd.h"
#include "GuildRankWnd.h"
#include "GuildJoinWnd.h"
#include "Chat.h"
#include "GlobalSoundHandles.h"
#include "GroupWnd.h"
#include "IMEEdit.h"
#include "GuildRequestWnd.h"
#include "InputManager.h"
#include "GuildPortalWnd.h"
#include "GuildWarWnd.h"
#include "RankNpcWnd.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonTable.h"
#include "CodeFun.h"
#include "GuildExWnd.h"
#include "ItemNative.h"


//==============================================================//
// 길드 마스터 권한 이양.										//
//==============================================================//
void CmdGuildUserGuildMaster(char* pMsg, DWORD dwLen)
{
	WSTC_USER_GUILDMASTER* pPacket = (WSTC_USER_GUILDMASTER*)pMsg;

	char szInfo[0xff] = {0,};

	if(g_pMainPlayer->m_dwUserIndex==pPacket->dwMasterUserIndex || g_pMainPlayer->m_dwUserIndex==pPacket->dwUserIndex)
	{
		if(g_pMainPlayer->m_dwUserIndex==pPacket->dwMasterUserIndex)
		{
			g_pMainPlayer->m_byRank	= pPacket->byMasterRank;

			LP_GUILD_USER lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwUserIndex);

			if(lpGuildUser)		
				lpGuildUser->byRank = pPacket->byRank;

			LP_GUILD_OFFLINE_USER lpGuildOfflineUser = g_pGuildOfflineHash->GetData(pPacket->dwUserIndex);

			if(lpGuildOfflineUser)
				lpGuildOfflineUser->byRank = pPacket->byRank;
			
			// "%s 님이 길드 마스터 권한이 이양 하셨습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE802].szMessage, pPacket->szName); 
			DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
		}
		else
		{
			g_pMainPlayer->m_byRank	= pPacket->byRank;

			LP_GUILD_USER lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwMasterUserIndex);

			if(lpGuildUser)		
				lpGuildUser->byRank = pPacket->byMasterRank;

			LP_GUILD_OFFLINE_USER lpGuildOfflineUser = g_pGuildOfflineHash->GetData(pPacket->dwMasterUserIndex);

			if(lpGuildOfflineUser)
				lpGuildOfflineUser->byRank = pPacket->byMasterRank;
			
			// "%s 님에게 길드 마스터 권한이 이양 되었습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE803].szMessage, pPacket->szMasterName); 
			DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
		}
	}
	else
	{
		LP_GUILD_USER lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwMasterUserIndex);

		if(lpGuildUser)		
			lpGuildUser->byRank = pPacket->byMasterRank;

		LP_GUILD_OFFLINE_USER lpGuildOfflineUser = g_pGuildOfflineHash->GetData(pPacket->dwMasterUserIndex);

		if(lpGuildOfflineUser)
			lpGuildOfflineUser->byRank = pPacket->byMasterRank;		
	
		lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwUserIndex);

		if(lpGuildUser)		
			lpGuildUser->byRank = pPacket->byRank;

		lpGuildOfflineUser = g_pGuildOfflineHash->GetData(pPacket->dwUserIndex);

		if(lpGuildOfflineUser)
			lpGuildOfflineUser->byRank = pPacket->byRank;
	
		// "%s 님에게 길드 마스터 권한이 이양 되었습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE803].szMessage, pPacket->szMasterName); 
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}

	CGroupWnd* pGorupWnd = CGroupWnd::GetInstance();
	
	if(pGorupWnd->GetActive())
		pGorupWnd->SetStatus(GROUP_GUILD_DEFAULT);
}


//==============================================================//
// 길드마스터 탈퇴로 인한 권한 이양.							//
//==============================================================//
void CmdGuildUserMaster(char* pMsg, DWORD dwLen)
{
	WSTC_USER_MASTER* pPacket = (WSTC_USER_MASTER*)pMsg;

	char szInfo[0xff] = {0,};
		
	g_pGuildInfo.byCount		-= 1;		
	g_pGuildInfo.byMember[0]	= pPacket->byMember[0];
	g_pGuildInfo.byMember[1]	= pPacket->byMember[1];
	g_pGuildInfo.byMember[2]	= pPacket->byMember[2];
	g_pGuildInfo.byMember[3]	= pPacket->byMember[3];
		
	LP_GUILD_USER lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwUserIndex);

	if(lpGuildUser)
	{
		g_pGuildUserHash->Delete(lpGuildUser, lpGuildUser->dwUserIndex, TRUE);
		memset(lpGuildUser, 0, sizeof(lpGuildUser));
	}

	if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)
		wsprintf(szInfo, g_Message[ETC_MESSAGE199].szMessage, pPacket->szCharacterName); // "%s 님이 클랜에서 탈퇴하였습니다."
	else if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)
		wsprintf(szInfo, g_Message[ETC_MESSAGE200].szMessage, pPacket->szCharacterName); // "%s 님이 길드에서 탈퇴하였습니다."

	DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);

	lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwMasterUserIndex);

	if(lpGuildUser)
	{		
		lpGuildUser->byRank	= 1;

		if(pPacket->byType==0)			// "%s %s 님의 캐릭터 삭제로 인해 %s 님에게 길드 마스터 권한이 이양 되었습니다."			
			wsprintf(szInfo, g_Message[ETC_MESSAGE804].szMessage, g_pGuildInfo.szRankName[0], pPacket->szCharacterName, lpGuildUser->szCharacterName);
		else if(pPacket->byType==1)		// "%s %s 님의 길드 탈퇴로 인해 %s 님에게 길드 마스터 권한이 이양 되었습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE805].szMessage, g_pGuildInfo.szRankName[0], pPacket->szCharacterName, lpGuildUser->szCharacterName);

		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}
	else if(g_pMainPlayer->m_dwUserIndex==pPacket->dwMasterUserIndex)
	{
		g_pMainPlayer->m_byRank = 1;
		lstrcpy(szInfo, g_Message[ETC_MESSAGE806].szMessage); // "길드 마스터 권한이 이양 되었습니다."
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}

	CGroupWnd* pGorupWnd = CGroupWnd::GetInstance();
	
	if(pGorupWnd->GetActive())
		pGorupWnd->SetStatus(GROUP_GUILD_DEFAULT);
}


//==============================================================//
// 등급 변경 실패.												//
//==============================================================//
void CmdGuildRankFail(char* pMsg, DWORD dwLen)
{
//	WSTC_RANK_FAIL* pPacket = (WSTC_RANK_FAIL*)pMsg;

	char szInfo[0xff] = {0,};
	lstrcpy(szInfo, g_Message[ETC_MESSAGE171].szMessage); // "해당등급의 정원이 찼습니다."
	DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
}


//==============================================================//
// 길드 레벨 설정.												//
//==============================================================//
void CmdGuildLevel(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_LEVEL* pPacket = (WSTC_GUILD_LEVEL*)pMsg;

	if(pPacket->dwGuildId==g_pMainPlayer->m_dwGuildId)
	{		
		LP_GUILD_USER lpGuildNode = g_pGuildUserHash->GetData( pPacket->dwUserIndex );

		if(lpGuildNode)
		{
			lpGuildNode->dwLev	= pPacket->dwLevel;			
			return;
		}
	}
}


//==============================================================//
// 길드 레벨업.													//
//==============================================================//
void CmdGuildExp(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_EXP* pPacket = (WSTC_GUILD_EXP*)pMsg;

	if(pPacket->byType==1)
	{
		char szInfo[0xff] = {0,};
		
		// 레벨업 //
		if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)		
			lstrcpy(szInfo, g_Message[ETC_MESSAGE172].szMessage); // "클랜이 레벨업 되었습니다."
		else		
			lstrcpy(szInfo, g_Message[ETC_MESSAGE173].szMessage); // "길드가 레벨업 되었습니다."

		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
	}
	g_pGuildInfo.dwExp	= pPacket->dwExp;
	g_pGuildInfo.wLevel	= pPacket->wLevel;

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	
	pGroupWnd->m_byGuildLevel = pPacket->byIndex;
	
	if(pGroupWnd->GetActive())
		pGroupWnd->OpenTab();	
}


//==============================================================//
// 길드 정보 변경.												//
//==============================================================//
void CmdGuildInfoRefresh(char* pMsg, DWORD dwLen)
{
	DSTC_GUILD_INFO* pPacket = (DSTC_GUILD_INFO*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(pUser)
	{		
		memset(pUser->m_szNickName, 0, sizeof(pUser->m_szNickName));
		__lstrcpyn(pUser->m_szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
		pUser->m_dwGuildId	= pPacket->dwGuildId;	
		pUser->DungeonOwnerEffect();
		
		if(pUser->m_dwGuildId!=0)
		{
			LP_GUILD_DATA lpGuildData = g_pGuildInfoHash->GetData(pUser->m_dwGuildId);

			if(!lpGuildData)
			{
				CTWS_GUILD_DATA_REQUEST pSendPacket;
				pSendPacket.dwGuildId	= pUser->m_dwGuildId;
				g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_WORLD);
			}
		}

		if(pUser==g_pMainPlayer)
		{
			if(pUser->m_dwGuildId==0)
			{
				if(g_pMainPlayer->m_byRank==1)
					g_pGuildOfflineHash->Destroy(TRUE);
	
				g_pGuildUserHash->Destroy(TRUE);

				CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

				if(pGroupWnd->GetActive())
					pGroupWnd->SetActive(FALSE);
			}
		}
	}
}


//==============================================================//
// 길드 인원수 변경.											//
//==============================================================//
void CmdGuildCount(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_COUNT* pPacket = (WSTC_GUILD_COUNT*)pMsg;

	g_pGuildInfo.byCount		= pPacket->byCount;
	g_pGuildInfo.byMember[0]	= pPacket->byMember[0];
	g_pGuildInfo.byMember[1]	= pPacket->byMember[1];
	g_pGuildInfo.byMember[2]	= pPacket->byMember[2];
	g_pGuildInfo.byMember[3]	= pPacket->byMember[3];
}


//==============================================================//
// 던전에서 다른 길드정보 요청.									//
//==============================================================//
void CmdGuildDataRequest(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_DATA_REQUEST* pPacket = (WSTC_GUILD_DATA_REQUEST*)pMsg;
	
	LP_GUILD_DATA lpGuildInfo = g_pGuildInfoHash->GetData(pPacket->dwGuildId);

	if(!lpGuildInfo)
	{
		LP_GUILD_DATA lpGuildDataInfo = (LP_GUILD_DATA)LALAlloc(g_pGuildDataPool);
		memset(lpGuildDataInfo, 0, sizeof(lpGuildDataInfo));
		lpGuildDataInfo->dwGuildId	= pPacket->dwGuildId;
		__lstrcpyn(lpGuildDataInfo->szGuildName, pPacket->szName, MAX_GUILD_NAME_REAL_LENGTH);
		g_pGuildInfoHash->Insert(lpGuildDataInfo, pPacket->dwGuildId);
	}
}


//==============================================================//
// 길드원이 요청한 유저 길드 가입 거절.							//
//==============================================================//
void CmdGuildUserRequestFail(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_USERREQUEST_FAIL* pPacket = (WSTC_GUILD_USERREQUEST_FAIL*)pMsg;

	char szInfo[0xff] = {0,};
	
	if(pPacket->byType==3)
		lstrcpy(szInfo, g_Message[ETC_MESSAGE174].szMessage); // "마스터 님께서 가입 요청을 거절하셨습니다."
	else
		return;
	
	DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
}


//==============================================================//
// 길드 마스터에게 길드원이 길드 가입 메세지 요청.				//
//==============================================================//
void CmdGuildRequest(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_REQUEST* pPacket = (WSTC_GUILD_REQUEST*)pMsg;	

	char szInfo[0xff] = {0,};

	CGuildRequestWnd* pGuildRequestWnd = CGuildRequestWnd::GetInstance();
	memset(pGuildRequestWnd->m_szName, 0, sizeof(pGuildRequestWnd->m_szName));
	memset(pGuildRequestWnd->m_szRequestName, 0, sizeof(pGuildRequestWnd->m_szRequestName));
	pGuildRequestWnd->m_bChk				= TRUE;
	pGuildRequestWnd->m_dwUserIndex			= pPacket->dwUserIndex;
	pGuildRequestWnd->m_dwRequestUserIndex	= pPacket->dwRequestUserIndex;
	__lstrcpyn(pGuildRequestWnd->m_szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH );
	__lstrcpyn(pGuildRequestWnd->m_szRequestName, pPacket->szRequestName, MAX_CHARACTER_NAME_REAL_LENGTH );	
	pGuildRequestWnd->SetActive();
		
	if(g_pGuildInfo.byType==__GCTYPE_CLEN__)
		wsprintf(szInfo, g_Message[ETC_MESSAGE175].szMessage, pPacket->szName); // "%s 님에게서 클랜 요청 메세지가 왔습니다."
	else
		wsprintf(szInfo, g_Message[ETC_MESSAGE176].szMessage, pPacket->szName); // "%s 님에게서 길드 요청 메세지가 왔습니다."

	DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);		
}


//==============================================================//
// 로그아웃일때 길드유저 오프라인 유저로 추가.					//
//==============================================================//
void CmdGuildoffLine(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_OFFLINEUSER* pPacket = (WSTC_GUILD_OFFLINEUSER*)pMsg;
	
	LP_GUILD_OFFLINE_USER lpGuildOffLineUser = (LP_GUILD_OFFLINE_USER)LALAlloc(g_pGuildOfflinePool);

	if(lpGuildOffLineUser)
	{
		memset(lpGuildOffLineUser,  0, sizeof(lpGuildOffLineUser));
		__lstrcpyn(lpGuildOffLineUser->szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		__lstrcpyn(lpGuildOffLineUser->szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
		lpGuildOffLineUser->byRank		= pPacket->byRank;
		lpGuildOffLineUser->dwLevel		= pPacket->dwLevel;
		lpGuildOffLineUser->dwUserIndex	= pPacket->dwUserIndex;
		lpGuildOffLineUser->wClass		= pPacket->wClass;
		g_pGuildOfflineHash->Insert(lpGuildOffLineUser, lpGuildOffLineUser->dwUserIndex);
		
		CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
		
		if(pGroupWnd->GetActive())
		{
			if(pGroupWnd->m_byGroupTab==GROUP_TAB_GUILD)
				pGroupWnd->SetPrevNext(__GUILD_PAGE_CURR__);
		}
	}	
}


//==============================================================//
// 길드 레벨 설정.												//
//==============================================================//
void CmdGuildUserLevel(char* pMsg, DWORD dwLen)
{
	WSTC_GUILDUSER_LEVEL* pPacket = (WSTC_GUILDUSER_LEVEL*)pMsg;

	LP_GUILD_USER lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwUserIndex);

	if(lpGuildUser)	
		lpGuildUser->dwLev	= pPacket->dwLevel;
}


//==============================================================//
// 로그인이나 길드유저 삭제시 오프라인 유저에서 삭제.			//
//==============================================================//
void CmdGuildoffLineDelete(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_OFFLINEUSER_DELETE* pPacket = (WSTC_GUILD_OFFLINEUSER_DELETE*)pMsg;
	
	LP_GUILD_OFFLINE_USER lpGuildOffLineUser = g_pGuildOfflineHash->GetData(pPacket->dwUserIndex);

	if(lpGuildOffLineUser)
	{
		g_pGuildOfflineHash->Delete(lpGuildOffLineUser, lpGuildOffLineUser->dwUserIndex);	
		
		CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
		
		if(pGroupWnd->GetActive()==TRUE)
			pGroupWnd->SetActive();					
	}
}


//==============================================================//
// 길드원 정보 보기.											//
//==============================================================//
void CmdGuildUserInfoRt(char* pMsg, DWORD dwLen)
{
	WSTC_GUILDUSER_INFO* pPacket = (WSTC_GUILDUSER_INFO*)pMsg;

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	
	if(pGroupWnd->GetActive()==TRUE)
	{
		memset(pGroupWnd->m_szName, 0, sizeof(pGroupWnd->m_szName));
		__lstrcpyn(pGroupWnd->m_szName, pPacket->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pGroupWnd->m_wDungeonID		= pPacket->wDungeonID;
		pGroupWnd->m_byLayerIndex		= pPacket->byLayerIndex;
		pGroupWnd->m_bInfo				= TRUE;
		pGroupWnd->m_dwPrevTime			= timeGetTime();
		pGroupWnd->m_dwCurTime			= timeGetTime();
	}
}


//==============================================================//
// 길드 생성시 정보 설정.										//
//==============================================================//
void CmdDgGuildUser(char* pMsg, DWORD dwLen)
{
	WSTC_DGGUILD_USER* pPacket = (WSTC_DGGUILD_USER*)pMsg;

	g_pGuildInfo.byType			= pPacket->byType;
	g_pGuildInfo.byCount		= pPacket->byCount;
	g_pGuildInfo.wLevel			= pPacket->wLevel;
	g_pMainPlayer->m_dwGuildId	= pPacket->dwGuildId;
	g_pMainPlayer->m_byType		= pPacket->byType;
	g_pMainPlayer->DungeonOwnerEffect();
	__lstrcpyn(g_pMainPlayer->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
	__lstrcpyn(g_pGuildInfo.szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);

	for( int iter =0 ; iter < 4 ; ++iter )
	{
		__lstrcpyn(g_pGuildInfo.szRankName[iter], pPacket->szRankName[iter], MAX_RANK_REAL_NAME);
	}
}


//==============================================================//
// 길드 생성시 유저 설정.										//
//==============================================================//
void CmdDgGuildUserInfo( char* pMsg, DWORD dwLen )
{
	WSTC_DGGUILD_USERINFO*	pPacket	= (WSTC_DGGUILD_USERINFO*)pMsg;		
			
	if(g_pMainPlayer->m_dwUserIndex==pPacket->dwUserIndex)
	{		
		g_pMainPlayer->m_byRank	= pPacket->byRank;
		__lstrcpyn(g_pMainPlayer->m_szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);			
	}

	if(g_pMainPlayer->m_byRank!=__GUILD_MASTER__)
	{
		LP_GUILD_USER pGuildUser = (LP_GUILD_USER)LALAlloc(g_pGuildPool);
		memset(pGuildUser, 0, sizeof(GUILD_USER));

		pGuildUser->byRank		= pPacket->byRank;	
		pGuildUser->dwLev		= pPacket->dwLev;
		pGuildUser->wClass		= pPacket->wClass;
		pGuildUser->dwGuildId	= pPacket->dwGuildId;
		pGuildUser->dwUserIndex	= pPacket->dwUserIndex;
		__lstrcpyn(pGuildUser->szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
		__lstrcpyn(pGuildUser->szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);			
		g_pGuildUserHash->Insert(pGuildUser, pGuildUser->dwUserIndex);		

		char szInfo[0xff] = {0,};
		// "[%s] %s님이 가입하셨습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE177].szMessage, g_pGuildInfo.szGuildName, pGuildUser->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}
}


//==============================================================//
// 길드 생성.													//
//==============================================================//
void CmdGuildCreate( char* pMsg, DWORD dwLen )
{
	char szInfo[0xff] = {0,};

	WSTC_GUILD_CREATE* pPacket = (WSTC_GUILD_CREATE*)pMsg;
						
	if(pPacket->bCreate==TRUE)
	{
		if(pPacket->bType==__GCTYPE_GUILD__)		// MSG_ID : 112 ; %s님의 길드가 성공적으로 등록되었습니다.
			wsprintf(szInfo, g_Message[ETC_MESSAGE112].szMessage, g_pMainPlayer->m_szName);	
		else										// MSG_ID : 113 ; %s님의 클랜이 성공적으로 등록되었습니다.
			wsprintf(szInfo, g_Message[ETC_MESSAGE113].szMessage, g_pMainPlayer->m_szName);

		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
					
		CGuildRankWnd* pGuildRankWnd = CGuildRankWnd::GetInstance();		
		pGuildRankWnd->SetActive(TRUE);						
		
		int nOrder = __ORDER_INTERFACE_START__ + pGuildRankWnd->m_byOrder*10+2;	

		RECT rt;
		rt.left = long(pGuildRankWnd->m_fPosX+58);		rt.right	= long(pGuildRankWnd->m_fPosX+120);
		rt.top	= long(pGuildRankWnd->m_fPosZ+45);		rt.bottom	= long(pGuildRankWnd->m_fPosZ+57);
		g_pInputManager->InitializeInput(INPUT_BUFFER_8, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
		rt.left = long(pGuildRankWnd->m_fPosX+58);		rt.right	= long(pGuildRankWnd->m_fPosX+120);
		rt.top	= long(pGuildRankWnd->m_fPosZ+60);		rt.bottom	= long(pGuildRankWnd->m_fPosZ+72);
		g_pInputManager->InitializeInput(INPUT_BUFFER_9, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
		rt.left = long(pGuildRankWnd->m_fPosX+58);		rt.right	= long(pGuildRankWnd->m_fPosX+120);
		rt.top	= long(pGuildRankWnd->m_fPosZ+75);		rt.bottom	= long(pGuildRankWnd->m_fPosZ+87);
		g_pInputManager->InitializeInput(INPUT_BUFFER_10, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
		rt.left = long(pGuildRankWnd->m_fPosX+58);		rt.right	= long(pGuildRankWnd->m_fPosX+120);
		rt.top	= long(pGuildRankWnd->m_fPosZ+90);		rt.bottom	= long(pGuildRankWnd->m_fPosZ+102);
		g_pInputManager->InitializeInput(INPUT_BUFFER_11, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
		
		g_pInputManager->SetFocusInput(INPUT_BUFFER_8);
		
		ChangeLanguage(TRUE);
		g_pGVDungeon->bChatMode = 1;

		// 길드 생성 아이템 제거 2005.01.25 김영대 
		//CGuildWnd * pGuildWnd = CGuildWnd::GetInstance();		
		//pGuildWnd->SendGuildItemUse(pPacket->bType);
		
#ifdef	_USE_IME 

		POINT pChatPoint;
		pChatPoint.x	= long(pGuildRankWnd->m_fPosX+58);
		pChatPoint.y	= long(pGuildRankWnd->m_fPosZ+45);
		GET_IMEEDIT()->SetEditIndex(1);
		GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_RANK_REAL_NAME, pGuildRankWnd->GetStartOrder()+2);

#endif		
	}		
	else
	{
		if(pPacket->bType==3)
			wsprintf(szInfo, g_Message[ETC_MESSAGE136].szMessage, g_pMainPlayer->m_szName);
		else if(pPacket->bType==__GCTYPE_GUILD__)
			lstrcpy(szInfo, g_Message[ETC_MESSAGE193].szMessage);	// "같은 이름의 길드가 있습니다."
		else		
			lstrcpy(szInfo, g_Message[ETC_MESSAGE194].szMessage);	// "같은 이름에 클랜이 있습니다."

		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
	}			
	
	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	
	if(pGroupWnd->GetActive()==TRUE)
	{
		pGroupWnd->OpenTab();
		pGroupWnd->SetActive();
		pGroupWnd->SetPosGroup();
		CInterface::GetInstance()->SetWnd(GROUP_WND);
	}
}


//==============================================================//
// 길드 삭제.													//
//==============================================================//
void CmdGuildDelete( char* pMsg, DWORD dwLen )
{
	char szInfo[0xff] = {0,};

//	WSTC_GUILD_DELETE* pPacket = (WSTC_GUILD_DELETE*)pMsg;

	if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)
		lstrcpy(szInfo, g_Message[ETC_MESSAGE195].szMessage);	// "클랜이 해체되었습니다."
	else
		lstrcpy(szInfo, g_Message[ETC_MESSAGE196].szMessage);	// "길드가 해체되었습니다."
	
	DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);

	if(GetGameStatus()==UPDATE_GAME_PLAY)
	{
		CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
		pGroupWnd->SetActive(FALSE);	
	}	

	// 오프라인 캐릭터 삭제 //
	if(g_pMainPlayer->m_byRank==1)
		g_pGuildOfflineHash->Destroy(TRUE);
	// 길드원 삭제 //
	g_pGuildUserHash->Destroy(TRUE);			
	// 정보 삭제 //
	memset(g_pMainPlayer->m_szGuildName, 0, sizeof(g_pMainPlayer->m_szGuildName));	
	memset(g_pMainPlayer->m_szNickName, 0, sizeof(g_pMainPlayer->m_szNickName));
	g_pMainPlayer->m_dwGuildId	= 0;
	g_pMainPlayer->m_byType		= 0;
	g_pMainPlayer->m_byRank		= 0;
	g_pMainPlayer->DungeonOwnerEffect();
}


//==============================================================//
// 길드 유저 추방, 탈퇴.										//
//==============================================================//
void CmdUserDelete( char* pMsg, DWORD dwLen )
{
	char				szInfo[0xff] = {0,};
	WSTC_USER_DELETE*	pPacket = (WSTC_USER_DELETE*)pMsg;
		
	if(pPacket->dwUserIndex==g_pMainPlayer->m_dwUserIndex)
	{
		if(pPacket->byChk == 1)
		{
			if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)			
				lstrcpy(szInfo, g_Message[ETC_MESSAGE779].szMessage); // "길드에서 추방당하였습니다."
			else if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)
				lstrcpy(szInfo, g_Message[ETC_MESSAGE788].szMessage); // "클랜에서 추방당하였습니다."
		}
		else
		{
			if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)
				lstrcpy(szInfo, g_Message[ETC_MESSAGE197].szMessage); // "길드에서 탈퇴하였습니다."
			else if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)
				lstrcpy(szInfo, g_Message[ETC_MESSAGE198].szMessage); // "클랜에서 탈퇴하였습니다."
		}
		
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);			

		// 자기 자신을 경우 //
		if(g_pMainPlayer->m_byRank==1)
			g_pGuildOfflineHash->Destroy(TRUE);
					
		memset(&g_pGuildInfo, 0, sizeof(g_pGuildInfo));
		g_pGuildUserHash->Destroy(TRUE);
		
		memset(g_pMainPlayer->m_szGuildName, 0, sizeof(g_pMainPlayer->m_szGuildName));
		memset(g_pMainPlayer->m_szNickName, 0, sizeof(g_pMainPlayer->m_szNickName));				
		g_pMainPlayer->m_dwGuildId	= 0;
		g_pMainPlayer->m_byRank		= 0;
		g_pMainPlayer->m_byType		= 0;	
		g_pMainPlayer->DungeonOwnerEffect();
	}
	else
	{
		g_pGuildInfo.byCount		-= 1;		
		g_pGuildInfo.byMember[0]	= pPacket->byMember[0];
		g_pGuildInfo.byMember[1]	= pPacket->byMember[1];
		g_pGuildInfo.byMember[2]	= pPacket->byMember[2];
		g_pGuildInfo.byMember[3]	= pPacket->byMember[3];
		
		LP_GUILD_USER lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwUserIndex);

		if(lpGuildUser)
		{
			g_pGuildUserHash->Delete(lpGuildUser, lpGuildUser->dwUserIndex, TRUE);
			memset(lpGuildUser, 0, sizeof(lpGuildUser));
		}

		if(pPacket->byChk == 1)
		{
			if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)
				wsprintf(szInfo, g_Message[ETC_MESSAGE787].szMessage, pPacket->szCharacterName); // "%s 님이 클랜에서 추방당했습니다."
			else if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)
				wsprintf(szInfo, g_Message[ETC_MESSAGE778].szMessage, pPacket->szCharacterName); // "%s 님이 길드에서 추방당했습니다."
		}
		else
		{
			if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)
				wsprintf(szInfo, g_Message[ETC_MESSAGE199].szMessage, pPacket->szCharacterName); // "%s 님이 클랜에서 탈퇴하였습니다."
			else if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)
				wsprintf(szInfo, g_Message[ETC_MESSAGE200].szMessage, pPacket->szCharacterName); // "%s 님이 길드에서 탈퇴하였습니다."
		}

		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);					
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive())
	{
		pGroupWnd->SetActive();
		pGroupWnd->SetActive(FALSE);	
	}	
}

void CmdGuildRankRt( char* pMsg, DWORD dwLen )
{

}


//==============================================================//
// 길드 가입후 유저 정보 설정.									//
//==============================================================//
void CmdGuildJoinInfo( char* pMsg, DWORD dwLen ) 
{
	WSTC_GUILD_JOININFO*	pPacket = (WSTC_GUILD_JOININFO*)pMsg;
	CTDS_GUILD_JOININFO		pSendPacket;

	g_pMainPlayer->m_byRank		= pPacket->byRank;
	g_pMainPlayer->m_byType		= pPacket->byType;
	g_pMainPlayer->m_dwGuildId	= pPacket->dwGuildId;
	
	__lstrcpyn(g_pMainPlayer->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
	g_pMainPlayer->DungeonOwnerEffect();
}


//==============================================================//
// 길드 가입처리 결과 메세지.									//
//==============================================================//
void CmdGuildJoinResult( char* pMsg, DWORD dwLen )
{
	WSTC_GUILD_JOINRESULT*	pPacket = (WSTC_GUILD_JOINRESULT*)pMsg;
	char					szInfo[0xff] = {0,};

	if(pPacket->bResult==TRUE)
	{
		// 가입이 된 경우 //				
		wsprintf(szInfo, g_Message[ETC_MESSAGE134].szMessage, pPacket->szCharacterName);	// MSG_ID : 134 ; %s 님이 가입 권유를 받아들이셨습니다 
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}
	else
	{
		memset(szInfo, 0, sizeof(szInfo));

		if(pPacket->bErrorCode==0)
			wsprintf(szInfo, g_Message[ETC_MESSAGE135].szMessage, pPacket->szCharacterName);	// MSG_ID : 135 ; %s 님이 길드 가입 권유를 거절하셨습니다.
		else if(pPacket->bErrorCode==1)
			wsprintf(szInfo, g_Message[ETC_MESSAGE136].szMessage, pPacket->szCharacterName);	// MSG_ID : 136 ; %s 님이 이미 다른 길드에 가입 되어있습니다.
		else if(pPacket->bErrorCode==2)
			wsprintf(szInfo, g_Message[ETC_MESSAGE137].szMessage, pPacket->szCharacterName);	// MSG_ID : 137 ; %s 님이 이미 다른 클랜에 가입 되어있습니다.
		else if(pPacket->bErrorCode==3)
			wsprintf(szInfo, g_Message[ETC_MESSAGE138].szMessage, pPacket->szCharacterName); // MSG_ID : 138 ; %s 님이 이미 다른 길드에 가입 권유를 받으셨습니다.
		else if(pPacket->bErrorCode==4)
			wsprintf(szInfo, g_Message[ETC_MESSAGE139].szMessage, pPacket->szCharacterName); // MSG_ID : 139 ; %s 님이 이미 다른 클랜에 가입 권유를 받으셨습니다.

		// 가입을 거절한 경우 //
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
	}		
}


//==============================================================//
// 길드 가입권유를 받아서 처리.									//
//==============================================================//
void CmdGuildJoinMessage( char* pMsg, DWORD dwLen )
{
 	char					szInfo[0xff] = {0,};
	WSTC_GUILDJOIN_MESSAGE* pPacket = (WSTC_GUILDJOIN_MESSAGE*)pMsg;
	CGuildJoinWnd*			pGuildJoinWnd = CGuildJoinWnd::GetInstance();
		
	if(pGuildJoinWnd->GetActive()==FALSE)
	{
		if(pPacket->bType==__GCTYPE_GUILD__)
			wsprintf(szInfo, g_Message[ETC_MESSAGE140].szMessage, pPacket->szCharacterName, pPacket->szGuildName);	// MSG_ID : 140 ; %s 님이 %s 길드의 가입을 권유하셨습니다.
		else
			wsprintf(szInfo, g_Message[ETC_MESSAGE141].szMessage, pPacket->szCharacterName, pPacket->szGuildName);	// MSG_ID : 141 ; %s 님이 %s 클랜의 가입을 권유하셨습니다.

		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
		
		pGuildJoinWnd->m_bGuildType		=	pPacket->bType;
		pGuildJoinWnd->m_dwUserIndex	=	pPacket->dwUserIndex;
		__lstrcpyn(pGuildJoinWnd->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
		__lstrcpyn(pGuildJoinWnd->m_szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pGuildJoinWnd->SetActive(TRUE);
	}
	else
	{
		// 길드 가입 취소 //
		CTWS_GUILD_JOIN pGuildJoinPacket;
		memset(pGuildJoinPacket.szGuildName, 0, sizeof(pGuildJoinPacket.szGuildName));
		pGuildJoinPacket.bGuildType		= BYTE(pPacket->bType+2);
		pGuildJoinPacket.bResult		= FALSE;
		pGuildJoinPacket.dwUserIndex	= pPacket->dwUserIndex;
		__lstrcpyn(pGuildJoinPacket.szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
		__lstrcpyn(pGuildJoinPacket.szCharacterName, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		g_pNet->SendMsg((char*)&pGuildJoinPacket, pGuildJoinPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}
}


//==============================================================//
// 길드아이템 사용 결과.										//
//==============================================================//
void CmdGuildItem( char* pMsg, DWORD dwLen )
{
	DSTC_GUILD_ITEM* pGuildItem = (DSTC_GUILD_ITEM*)pMsg;

	if(pGuildItem->bResult==TRUE)
	{
		// Guild Wnd Open //
		CGuildWnd* pGuildWnd = CGuildWnd::GetInstance();						
		pGuildWnd->m_bGuildType  = pGuildItem->bGuildType;
		pGuildWnd->SetActive(TRUE);	
		CInterface::GetInstance()->SetWnd(GUILD_WND);
	}
	else
	{		
		if(pGuildItem->bZipCode==__GCTYPE_GUILD__)
		{
			// MSG_ID : 142 ; 다른 길드에 가입되어 있습니다.
			DisplayMessageAdd(g_Message[ETC_MESSAGE142].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		}
		else if(pGuildItem->bZipCode==__GCTYPE_CLEN__)
		{
			// MSG_ID : 143 ; 다른 클랜에 가입되어 있습니다.
			DisplayMessageAdd(g_Message[ETC_MESSAGE143].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		}
		else if(pGuildItem->bZipCode==3)
		{
			// MSG_ID : 144 ; 아이템이 없습니다.
			DisplayMessageAdd(g_Message[ETC_MESSAGE144].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		}
		else if(pGuildItem->bZipCode==4)
		{
			// MSG_ID : 145 ; 계정안에 다른 캐릭터가 클랜에 가입되어 있습니다.
			DisplayMessageAdd(g_Message[ETC_MESSAGE145].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		}
		else if(pGuildItem->bZipCode==5)
		{
			// "레벨 30 이상부터 클랜을 생성할 수 있습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE579].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD); 
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		}
		else if(pGuildItem->bZipCode==6)
		{
			// "레벨 10 이상부터 길드를 생성할 수 있습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE580].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD); 
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		}
	}
}


//==============================================================//
// 길드 등급, 칭호 설정.										//
//==============================================================//
void CmdGuildInfo(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_INFO*	pPacket = (WSTC_GUILD_INFO*)pMsg;
	char				szInfo[0xff] = {0,};

	if(pPacket->dwUserIndex==g_pMainPlayer->m_dwUserIndex)
	{
		__lstrcpyn(g_pMainPlayer->m_szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
		g_pMainPlayer->m_byRank = pPacket->bRank;		
	}
	else
	{
		LP_GUILD_USER lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwUserIndex);
	
		if(lpGuildUser)		
		{
			__lstrcpyn(lpGuildUser->szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
			lpGuildUser->byRank	= pPacket->bRank;
		}
		else
		{
			LP_GUILD_OFFLINE_USER lpGuildOfflineUser = g_pGuildOfflineHash->GetData(pPacket->dwUserIndex);

			if(lpGuildOfflineUser)
			{
				__lstrcpyn(lpGuildOfflineUser->szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
				lpGuildOfflineUser->byRank	= pPacket->bRank;
			}
		}				
	}
	
	if(pPacket->bMsgType==__GUILDINFO_MSG_RANK__)
	{
		wsprintf(szInfo, g_Message[ETC_MESSAGE411].szMessage, pPacket->szCharacterName, g_pGuildInfo.szRankName[pPacket->bRank]);
	}
	else if(pPacket->bMsgType==__GUILDINFO_MSG_NICKNAME__)
	{
		// MSG_ID : 412 ; %s 님의 칭호가 %s로 조정되었습니다.
		wsprintf(szInfo, g_Message[ETC_MESSAGE412].szMessage, pPacket->szCharacterName, pPacket->szNickName); 
	}
	else if(pPacket->bMsgType==__GUILDINFO_MSG_NICKNAMERANK__)
	{
		wsprintf(szInfo, g_Message[ETC_MESSAGE411].szMessage, pPacket->szCharacterName, g_pGuildInfo.szRankName[pPacket->bRank]);
		
		// MSG_ID : 412 ; %s 님의 칭호가 %s로 조정되었습니다.
		wsprintf(szInfo, g_Message[ETC_MESSAGE412].szMessage, pPacket->szCharacterName, pPacket->szNickName);	
	}
						
	DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
}


//==============================================================//
// 길드원이 접속하면 길드 전체에 알림.							//
//==============================================================//
void CmdGuildUserInfo(char* pMsg, DWORD dwLen)
{	
	WSTC_GUILD_USERINFO* pPacket = (WSTC_GUILD_USERINFO*)pMsg;	
				
	if(pPacket->bGuildCount==2)
		g_pGuildInfo.byCount += 1;
	
	if(g_pMainPlayer->m_dwUserIndex!=pPacket->dwUserIndex)
	{
		LP_GUILD_USER pGuildUser = (LP_GUILD_USER)LALAlloc(g_pGuildPool);
		memset(pGuildUser, 0, sizeof(GUILD_USER));		

		pGuildUser->dwLev		= pPacket->dwLev;
		pGuildUser->wClass		= pPacket->wClass;
		pGuildUser->byRank		= pPacket->byRank;
		pGuildUser->dwGuildId	= pPacket->dwGuildId;
		pGuildUser->dwLev		= pPacket->dwLev;
		pGuildUser->dwUserIndex	= pPacket->dwUserIndex;
		__lstrcpyn(pGuildUser->szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
		__lstrcpyn(pGuildUser->szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
		g_pGuildUserHash->Insert(pGuildUser, pGuildUser->dwUserIndex);
		
		char szInfo[0xff] = {0,};
		// "[%s] %s님이 접속하셨습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE201].szMessage, g_pGuildInfo.szGuildName, pGuildUser->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}		
}


//==============================================================//
// 길드원 삭제.													//
//==============================================================//
void CmdGuildUserInfoDel(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_USERINFODEL*	pPacket	= (WSTC_GUILD_USERINFODEL*)pMsg;		
	
	LP_GUILD_USER lpGuildUser = g_pGuildUserHash->GetData(pPacket->dwUserIndex);
	
	if(lpGuildUser)
	{
		g_pGuildUserHash->Delete(lpGuildUser, lpGuildUser->dwUserIndex, TRUE);
		memset(lpGuildUser, 0, sizeof(lpGuildUser));
	}
	
	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	
	if(pGroupWnd->GetActive()==TRUE)
		pGroupWnd->SetActive();
}


//==============================================================//
// 접속시 길드 정보 설정.										//
//==============================================================//
void CmdGuildUser(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_USER* pPacket = (WSTC_GUILD_USER*)pMsg;
	memset(g_pGuildInfo.szGuildName, 0, sizeof(g_pGuildInfo.szGuildName));

	for( int i = 0 ; i < 4 ; ++i )
	{
		::memset(g_pGuildInfo.szRankName[i], 0, sizeof(g_pGuildInfo.szRankName[i]));
	}
	memset(g_pMainPlayer->m_szGuildName, 0, sizeof(g_pMainPlayer->m_szGuildName));

	g_pGuildInfo.byType			= pPacket->byType;	
	g_pGuildInfo.byCount		= pPacket->byCount;
	g_pGuildInfo.dwGuildId		= pPacket->dwGuildId;	
	g_pGuildInfo.dwExp			= pPacket->dwExp;
	g_pGuildInfo.dwPoint		= pPacket->dwPoint;
	g_pGuildInfo.wLevel			= pPacket->wLevel;
	g_pGuildInfo.byMember[0]	= pPacket->byMember[0];
	g_pGuildInfo.byMember[1]	= pPacket->byMember[1];
	g_pGuildInfo.byMember[2]	= pPacket->byMember[2];
	g_pGuildInfo.byMember[3]	= pPacket->byMember[3];
	
	CGroupWnd::GetInstance()->m_byGuildLevel = pPacket->byLevelIndex;
			
	__lstrcpyn(g_pGuildInfo.szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);

	for( int iter = 0 ; iter < 4 ; ++iter )
	{
		__lstrcpyn(g_pGuildInfo.szRankName[iter], pPacket->szRankName[iter], MAX_RANK_REAL_NAME);
	}
	
	__lstrcpyn(g_pMainPlayer->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
}


//==============================================================//
// 길드 생성 실패시 유저에게 아이템 넣어줌.						//
//==============================================================//
void CmdGuildFail(char* pMsg, DWORD dwLen)
{
	DSTC_GUILD_FAIL* pPacket = (DSTC_GUILD_FAIL*)pMsg;

	Insert_SmallInvItem(&pPacket->cItem, 0, 1);	
}


//==============================================================//
// 길드전 리스트 정보.											//
//==============================================================//
void CmdGuildList(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_LIST* pPacket = (WSTC_GUILD_LIST*)pMsg;

	LP_GUILD_WAR lpGuildWar = (LP_GUILD_WAR)LALAlloc(g_pGuildWarPool);
	
	if(lpGuildWar)
	{
		time_t now;
		time(&now);		

		memset(lpGuildWar, 0, sizeof(lpGuildWar));
		__lstrcpyn(lpGuildWar->szGuildName, pPacket->szName, MAX_GUILD_NAME_REAL_LENGTH);
		lpGuildWar->wPoint1		= pPacket->wPoint1;
		lpGuildWar->wPoint2		= pPacket->wPoint2;
		lpGuildWar->dwGuildId	= pPacket->dwGuildId;
		lpGuildWar->sTime		= now-(pPacket->sCurTime-pPacket->sTime);		
		g_pGuildWarHash->Insert(lpGuildWar, lpGuildWar->dwGuildId);				
	}
}


//==============================================================//
// 길드관련 에러 메세지 처리.									//
//==============================================================//
void CmdGuildError(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_ERROR* pPacket = (WSTC_GUILD_ERROR*)pMsg;

	char szInfo[0xff] = {0,};

	if(pPacket->byError==1)
	{
		// "탈퇴했던 길드에 재가입하려면 탈퇴한 후 24 시간이 지나야 합니다"
		lstrcpy(szInfo, g_Message[ETC_MESSAGE868].szMessage);
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}
	else if(pPacket->byError==2)
	{	
		// "길드에 가입한 후 1주일간 탈퇴할 수 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE869].szMessage);
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}
	else if(pPacket->byError==3)
	{
	//	lstrcpy(szInfo, "");
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}
}


//==============================================================//
// 길드 포탈 메세지.											//
//==============================================================//
void CmdGuildPortal(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_PORTAL* pPacket = (WSTC_GUILD_PORTAL*)pMsg;

	CGuildPortalWnd* pGuildPortalWnd = CGuildPortalWnd::GetInstance();
	
	if(pGuildPortalWnd->GetActive()==FALSE)
	{
		if(pPacket->dwPartalUserIndex == g_pMainPlayer->m_dwUserIndex)
		{
			if(g_pMainPlayer->m_dwGuildId == pPacket->dwGuildId)
			{
				LP_GUILD_USER lpGuildUsr = g_pGuildUserHash->GetData(pPacket->dwUserIndex);
				
				if(lpGuildUsr)
				{
					if(GetGameStatus()==UPDATE_GAME_PLAY)
					{				
						pGuildPortalWnd->m_byIndex		= pPacket->byIndex;
						pGuildPortalWnd->m_byType		= pPacket->byType;
						pGuildPortalWnd->m_dwUserIndex	= pPacket->dwUserIndex;
						memset(pGuildPortalWnd->m_szName, 0, sizeof(pGuildPortalWnd->m_szName));
						__lstrcpyn(pGuildPortalWnd->m_szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);

						pGuildPortalWnd->SetActive();
						pGuildPortalWnd->m_bActiveChk = TRUE;
					}
				}

				return;
			}
		}
	}
}


//==============================================================//
// 길드 포탈 사용후 결과.										//
//==============================================================//
void CmdGuildPotalUsed(char* pMsg, DWORD dwLen)
{
	WSTC_GUILD_PORTALUSED* pPacket = (WSTC_GUILD_PORTALUSED*)pMsg;
	
	if(pPacket->dwResult==0)
	{
		// 거부 //
		char szInfo[0xff] = {0,};
		// "%s 님이 길드포탈을 거부하셨습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE880].szMessage, pPacket->szName); 
		DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
	}
	else if(pPacket->dwResult==1)
	{
		// 승인 //		
		if(pPacket->byType==1)
		{
			if(g_pMainPlayer->m_pInv_Small[pPacket->byIndex].GetID()!=__ITEM_GUILD_RECALL_INDEX__)
			{
				// 아이템이 없을 경우 //
				// "길드 포탈 아이템이 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE881].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD); 

				CTDS_GUILD_PORTALFAIL pSendPacket;
				pSendPacket.dwPartalUserIndex	= pPacket->dwPartalUserIndex;
				pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
				pSendPacket.byResult			= 1;				
				g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_ZONE );
			}
			else
			{
				CTDS_ITEM_PICKUP ItemPickup;
				ItemPickup.bSectionNum	= 1;
				ItemPickup.i64ItemID	= 0;
				ItemPickup.dwMoney		= pPacket->dwPartalUserIndex;				
				SetItemPacket(&ItemPickup, 76, pPacket->byIndex, 0, 0, 0);
				ItemPickup.bZipCode2	= pPacket->byType;
				g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
				return;
			}
		}
		else
		{
			CTDS_ITEM_PICKUP ItemPickup;
			ItemPickup.bSectionNum	= 1;
			ItemPickup.i64ItemID	= 0;
			ItemPickup.dwMoney		= pPacket->dwPartalUserIndex;			
			SetItemPacket(&ItemPickup, 76, pPacket->byIndex, 0, 0, 0);
			ItemPickup.bZipCode2	= pPacket->byType;
			g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
			return;
		}		
	}
}


//==============================================================//
// 길드 포탈 실패.												//
//==============================================================//
void CmdGuildPotalFail(char* pMsg, DWORD dwLen)
{
	DSTC_GUILD_PORTALFAIL* pPacket = (DSTC_GUILD_PORTALFAIL*)pMsg;

	char szInfo[0xff] = {0,};

	if(pPacket->byResult==1)
	{
		// 상대편이 취소 했을 경우 //
		// "%s 님이 길드포탈을 거부하셨습니다."		
		wsprintf(szInfo, g_Message[ETC_MESSAGE880].szMessage, pPacket->szName); 
	}
	else if(pPacket->byResult==2)
	{
		// 같은 던젼, 같은 층에 있을 경우 //
		// "같은 던젼, 같은 층에서는 포탈을 사용할수 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE621].szMessage); 
	}
	else if(pPacket->byResult==3)
	{
		// 아이템이 없을 경우 //
		// "%s 님이 아이템이 없습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE622].szMessage, pPacket->szName); 
	}
	else if(pPacket->byResult==4)
	{
		// 아이템이 없을 경우 //
		//"공성전에는 포탈을 사용할수 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE623].szMessage); 
	}
	else if(pPacket->byResult==5)
	{
		// 아이템이 없을 경우 //
		//"포탈에러로 사용할수 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE713].szMessage); 
	}
	else
	{
		return;
	}
	
	DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
}


//==============================================================//
// 길드 포탈 실패.												//
//==============================================================//
void CmdServerGuildPortalFail( char* pMsg, DWORD dwLen )
{
	WSTC_GUILD_PORTALFAIL* pPacket = (WSTC_GUILD_PORTALFAIL*)pMsg;

	char szInfo[0xff] = {0,};

	if(pPacket->byResult==1)
	{
		// 상대편이 취소 했을 경우 //
		wsprintf(szInfo, g_Message[ETC_MESSAGE880].szMessage, pPacket->szName); // "%s 님이 길드포탈을 거부하셨습니다."		
	}
	else if(pPacket->byResult==2)
	{
		// 같은 던젼, 같은 층에 있을 경우 //
		lstrcpy(szInfo, g_Message[ETC_MESSAGE621].szMessage); // "같은 던젼, 같은 층에서는 포탈을 사용할수 없습니다."
	}
	else if(pPacket->byResult==3)
	{
		// 아이템이 없을 경우 //
		wsprintf(szInfo, g_Message[ETC_MESSAGE622].szMessage, pPacket->szName); // "%s 님이 아이템이 없습니다."
	}
	else if(pPacket->byResult==4)
	{
		// 아이템이 없을 경우 //
		lstrcpy(szInfo, g_Message[ETC_MESSAGE623].szMessage); // "공성전에는 포탈을 사용할수 없습니다."
	}
	else
		return;

	DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_GUILD);
}


//==============================================================//
// 길드전 신청.													//
//==============================================================//
void CmdGuildWarMaster( char* pMsg, DWORD dwLen )
{
	WSTC_GUILDWAR_REQUEST* pPacket = (WSTC_GUILDWAR_REQUEST*)pMsg;
	
	CGuildWarWnd* pGuildWarWnd = CGuildWarWnd::GetInstance();
	
	if(pGuildWarWnd->GetActive()==FALSE)
	{
		pGuildWarWnd->SetActive();
		memset(pGuildWarWnd->m_szGuildName, 0, sizeof(pGuildWarWnd->m_szGuildName));
		__lstrcpyn(pGuildWarWnd->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
		pGuildWarWnd->m_dwGuildId	= pPacket->dwDstGuildId;
		pGuildWarWnd->m_byType		= 1;
		pGuildWarWnd->m_dwUserIndex	= pPacket->dwUserIndex;
	}	
}


//==============================================================//
// 길드전 조건이 안될때 에러 메세지.							//
//==============================================================//
void CmdGuildWarError( char* pMsg, DWORD dwLen )
{
	WSTC_GUILDWAR_ERRROR* pPacket = (WSTC_GUILDWAR_ERRROR*)pMsg;

	char szInfo[0xff] = {0,};

	switch(pPacket->byError)
	{
	case 0:
		// "상대 길드마스터가 게임에 접속해 있지 않습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE882].szMessage);
		break;
	case 1:
		// "상대 길드가 최대 8개의 다른 길드와 길드전 중입니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE883].szMessage);
		break;
	case 2:
		// "상대 길드 레벨 3이상이어야 길드전이 가능합니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE884].szMessage);
		break;
	case 3:
		// "상대 길드마스터를 제외한 길드원이 최소 1명 이상 존재해야 합니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE885].szMessage);
		break;
	case 4:
		// "%s 길드 마스터가 길드전 선포 신청을 거절하였습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE886].szMessage);
		break;
	case 5:
		// "길드 마스터가 아닙니다."
//		wsprintf(szInfo, g_Message[ETC_MESSAGE887].szMessage);
		lstrcpy(szInfo, g_Message[ETC_MESSAGE887].szMessage);
		break;
	case 6:
		// "길드가 최대 8개의 다른 길드와 길드전 중입니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE888].szMessage);
		break;
	case 7:
		// "길드 레벨 3이상이어야 길드전이 가능합니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE874].szMessage);
		break;
	case 8:
		// "길드마스터를 제외한 길드원이 최소 1명 이상 존재해야 합니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE873].szMessage);
		break;	
	case 9:		
		// "현재 길드전이 진행되고 있습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE875].szMessage);
		break;	
	case 10:
		// "같은 길드원끼리 길드전을 신청할 수 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE894].szMessage);
		break;
	case 11:		
		// "길드전 중 길드삭제가 불가능 합니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE906].szMessage);
		break;	
	case 12:
		// "현재 길드전 협상중입니다. 잠시 후에 다시 시도해주세요."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE907].szMessage);		
		break;
	case 13:
		// "길드전 승락시 3일(72시간)이후에 종료가 가능합니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE905].szMessage);
		break;
	default:
		return;
	}
	DisplayMessageAdd(szInfo, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
}


//==============================================================//
// 길드전 신청 결과.											//
//==============================================================//
void CmdGuildWarJoin( char* pMsg, DWORD dwLen )
{
	WSTC_GUILDWAR_JOIN* pPacket = (WSTC_GUILDWAR_JOIN*)pMsg;
	
	CGuildWarWnd* pGuildWarWnd = CGuildWarWnd::GetInstance();

	if(pGuildWarWnd->GetActive()==FALSE)
	{
		pGuildWarWnd->SetActive();
		memset(pGuildWarWnd->m_szGuildName, 0, sizeof(pGuildWarWnd->m_szGuildName));
		__lstrcpyn(pGuildWarWnd->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
		pGuildWarWnd->m_dwGuildId	= pPacket->dwGuildId;
		pGuildWarWnd->m_byType		= 2;
		pGuildWarWnd->m_dwUserIndex	= pPacket->dwUserIndex;
	}	
}


//==============================================================//
// 길드전 점수 관련. 											//
//==============================================================//
void CmdGuildWarPoint( char* pMsg, DWORD dwLen )
{
	WSTC_GUILDWAR_POINT* pPacket = (WSTC_GUILDWAR_POINT*)pMsg;

	LP_GUILD_WAR lpGuildWar = g_pGuildWarHash->GetData(pPacket->dwGuildId);

	if(lpGuildWar)
	{
		lpGuildWar->wPoint1	= pPacket->wPoint1;
		lpGuildWar->wPoint2	= pPacket->wPoint2;

		char szInfo[0xff] = {0,};

		switch(pPacket->byType)
		{
		case 0:
			// "%s 길드를 공격하여 %d포인트를 획득했습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE908].szMessage, lpGuildWar->szGuildName, 1);
			break;
		case 1:
			//"%s 길드로부터 공격당하여 %d포인트를 잃었습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE909].szMessage, lpGuildWar->szGuildName, 1);
			break;
		case 2:
			// "%s 길드를 공격하여 %d포인트를 획득했습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE908].szMessage, lpGuildWar->szGuildName, 3);
			break;
		case 3:
			//"%s 길드로부터 공격당하여 %d포인트를 잃었습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE909].szMessage, lpGuildWar->szGuildName, 3);
			break;
		case 4:
			// "%s 길드를 공격하여 %d포인트를 획득했습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE908].szMessage, lpGuildWar->szGuildName, 10);
			break;
		case 5:
			//"%s 길드로부터 공격당하여 %d포인트를 잃었습니다."
#if			IS_CHINA_LOCALIZING()

#else
			wsprintf(szInfo, g_Message[ETC_MESSAGE909].szMessage, lpGuildWar->szGuildName, 10);
#endif
			break;
		case 6:
			// "%s 길드 길드원이 가입하여 %d포인트를 획득했습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE910].szMessage, lpGuildWar->szGuildName, 3);
			break;
		case 7:
			//"%s 길드와 길드전중 길드원이 가입되어 %d 포인트를 잃었습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE911].szMessage, lpGuildWar->szGuildName, 3);
			break;
		case 8:
			//"%s 길드의 소유던젼을 점령하여 %d포인트를 획득했습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE912].szMessage, lpGuildWar->szGuildName, 100);
			break;
		case 9:
			//"%s 길드가 길드소유던젼을 점령하여 %d포인트를 잃었습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE913].szMessage, lpGuildWar->szGuildName, 100);
			break;
		default:
			return;
		}
		DisplayMessageAdd(szInfo, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
	}
}


//==============================================================//
// 길드전 종료.													//
//==============================================================//
void CmdGuildWarExit( char* pMsg, DWORD dwLen )
{
	WSTC_GUILDWAR_EXIT* pPacket = (WSTC_GUILDWAR_EXIT*)pMsg;	

	LP_GUILD_WAR pGuildWar = g_pGuildWarHash->GetData(pPacket->dwGuildId);

	if(pGuildWar)
	{
		char szInfo[0xff] = {0,};

		switch(pPacket->byType)
		{
		case 0:
			// "%s 길드와 %s 길드간 치뤄진 길드전은 %s 길드의 승리로 종료되었습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE914].szMessage, g_pMainPlayer->m_szGuildName, pGuildWar->szGuildName, g_pMainPlayer->m_szGuildName);
			break;
		case 1:
			// "%s 길드와 %s 길드간 치뤄진 길드전은 %s 길드의 승리로 종료되었습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE914].szMessage, g_pMainPlayer->m_szGuildName, pGuildWar->szGuildName, pGuildWar->szGuildName);
			break;
		case 2:
			// "%s 길드와 %s 길드간 치뤄진 길드전은 무승부로 종료되었습니다.
			wsprintf(szInfo, g_Message[ETC_MESSAGE915].szMessage, g_pMainPlayer->m_szGuildName, pGuildWar->szGuildName);
			break;
		default:
			return;
		}

		DisplayMessageAdd(szInfo, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
		
		g_pGuildInfo.dwPoint = pPacket->dwPoint;
		g_pGuildWarHash->Delete(pGuildWar, pGuildWar->dwGuildId, TRUE);
		memset(pGuildWar, 0, sizeof(pGuildWar));
		pGuildWar = NULL;	
	}
	
	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive())
		pGroupWnd->SetStatus(GROUP_GUILDWAR_DEFAULT);
}


//==============================================================//
// 길드전 순위 설정.											//
//==============================================================//
void CmdGuildWarRank( char* pMsg, DWORD dwLen )
{
	WSTC_GUILDWAR_RANK* pPacket = (WSTC_GUILDWAR_RANK*)pMsg;

	CRankNpcWnd* pRankNpcWnd = CRankNpcWnd::GetInstance();

	for(int i = 0; i < 4; i++)
	{
		pRankNpcWnd->m_dwGuildId[i]	= pPacket->dwGuildId[i];
		pRankNpcWnd->m_dwPoint[i]	= pPacket->dwPoint[i];
		memset(pRankNpcWnd->m_szGuildName[i], 0, sizeof(pRankNpcWnd->m_szGuildName[i]));
		__lstrcpyn(pRankNpcWnd->m_szGuildName[i], pPacket->szName[i], MAX_GUILD_NAME_REAL_LENGTH);
	}
	pRankNpcWnd->m_dwRank	= pPacket->dwRank;
	pRankNpcWnd->SetText();
	pRankNpcWnd->SetActive();
}


//==============================================================//
// 길드전 1위 길드 설정.										//
//==============================================================//
void CmdGuildUserRank( char* pMsg, DWORD dwLen )
{
//	DSTC_GUILD_RANK* pPacket = (DSTC_GUILD_RANK*)pMsg;
}


//==============================================================//
// 길드전 포인트 관련 메세지.									//
//==============================================================//
void CmdGuildWarMessage( char* pMsg, DWORD dwLen )
{
	DSTC_GUILD_WAR_MESSAGE* pPacket = (DSTC_GUILD_WAR_MESSAGE*)pMsg;

	char szInfo[0xff] = {0,};

	switch(pPacket->byType)
	{
	case 0:
		// "%s 길드가 %s 길드에 길드전을 선포했습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE1128].szMessage, pPacket->szGuildName, pPacket->szGuildNameDst);
		break;
	case 1:
		// "%s 길드가 %s 길드의 길드전 선포를 거부했습니다. %s 길드의 명성이 하락합니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE1129].szMessage, pPacket->szGuildName, pPacket->szGuildNameDst, pPacket->szGuildName);		
		break;	
	}
	DisplayMessageAdd(szInfo, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
}


//==============================================================//
// 길드전 포인트 관련 메세지.									//
//==============================================================//
void CmdGuildWarMessageGuild( char* pMsg, DWORD dwLen )
{
	WSTC_GUILDWAR_MESSAGE* pPacket = (WSTC_GUILDWAR_MESSAGE*)pMsg;

	char szInfo[0xff]	= {0,};	
	char szLayer[50]	= {0, };

	DUNGEON_DATA_EX* pDungeonData = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if(!pDungeonData)
		return;

	if(pPacket->byLayerIndex==0)	
		lstrcpy(szLayer, g_Message[ETC_MESSAGE16].szMessage);
	else
		wsprintf(szLayer, g_Message[ETC_MESSAGE17].szMessage, pPacket->byLayerIndex);
		
	switch(pPacket->byType)
	{
	case 2:
		// "%s 님이 %s 길드 %s 님에게 %s 던젼 %s 층에서 죽임을 당했습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE1130].szMessage, pPacket->szNameDst, pPacket->szGuildName, pPacket->szName, pDungeonData->m_szDungeonName, szLayer);
		break;
	case 3:
		// "%s 님이 %s 길드 %s 님에게 죽음을 선사했습니다. 길드전 점수가 상승됩니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE1131].szMessage, pPacket->szName, pPacket->szGuildName, pPacket->szNameDst);
		break;
	}

	DisplayMessageAdd(szInfo, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
}


void CmdGuildFlag( char* pMsg, DWORD dwLen )
{
	DSTC_GUILD_FLAG* pPacket = (DSTC_GUILD_FLAG*)pMsg;
	
	ListNode<CUser>* pUserNode =  g_pUserHash->GetHead();

	while(pUserNode)
	{
		CUser* pUser = (CUser*)pUserNode->pData;

		if(pUser)
		{
			if(pUser->m_dwGuildId==pPacket->dwPrevGuildId && pUser->m_dwGuildId!=0)
			{
				ItemDeleteObject(&pUser->m_hPlayerFlag.pHandle, &pUser->m_hPlayer.pHandle);
				ObjDeleteDesc(&pUser->m_hPlayerFlag.pDesc);	
			}
			
			if(pUser->m_dwGuildId==pPacket->dwGuildId  && pUser->m_dwGuildId!=0)
			{
				ItemDeleteObject(&pUser->m_hPlayerFlag.pHandle, &pUser->m_hPlayer.pHandle);				
				// sung-han 2005-02-14 길드전 혜택중 길드 깃발
				switch(pPacket->byGuildFlag)
				{
				case 1:
					{
						ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag.chr", DATA_TYPE_ITEM));				
					}
					break;
				case 2:
					{
						ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag2.chr", DATA_TYPE_ITEM));				
					}
					break;
				case 3:
					{
						ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag3.chr", DATA_TYPE_ITEM));				
					}
					break;
				case 4:
					{
						ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag4.chr", DATA_TYPE_ITEM));				
					}
					break;
				case 5:
					{
						ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag5.chr", DATA_TYPE_ITEM));				
					}
					break;
				}
			}
		}
		pUserNode = pUserNode->pNext;
	}
}

void CmdGuildUserFlag( char* pMsg, DWORD dwLen )
{
	DSTC_GUILDUSER_FLAG* pPacket = (DSTC_GUILDUSER_FLAG*)pMsg;
	
	CUser* pUser =  g_pUserHash->GetData(pPacket->dwUserIndex);
	
	if(pUser)
	{
		if(pPacket->byGuildFlag==0)
		{				
			ItemDeleteObject(&pUser->m_hPlayerFlag.pHandle, &pUser->m_hPlayer.pHandle);				
			ObjDeleteDesc(&pUser->m_hPlayerFlag.pDesc);	
		}		
		else if(pPacket->byGuildFlag)
		{
			ItemDeleteObject(&pUser->m_hPlayerFlag.pHandle, &pUser->m_hPlayer.pHandle);				
			// sung-han 2005-02-14 길드전 혜택중 길드 깃발
			switch(pPacket->byGuildFlag)
			{
			case 1:
				{
					ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag.chr", DATA_TYPE_ITEM));	
				}
				break;
			case 2:
				{
					ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag2.chr", DATA_TYPE_ITEM));	
				}
				break;
			case 3:
				{
					ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag3.chr", DATA_TYPE_ITEM));	
				}
				break;
			case 4:
				{
					ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag4.chr", DATA_TYPE_ITEM));	
				}
				break;
			case 5:
				{
					ItemAttach(0, &pUser->m_hPlayerFlag.pHandle, pUser->m_hPlayerFlag.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_FLAG, 0, 0, GetFile("GuildFlag5.chr", DATA_TYPE_ITEM));	
				}
				break;
			}
		}
	}
}

BOOL IsGuildCreate()
{
	CGuildExWnd* pGuildExWnd = CGuildExWnd::GetInstance();

	return pGuildExWnd->GetActive();
}
//==============================================================//
// End.															//
//==============================================================//