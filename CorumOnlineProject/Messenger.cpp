//==============================================================//
// Code by Jang.									2003.11.10	//
//==============================================================//
#include "Messenger.h"
#include "GameControl.h"
#include "User.h"
#include "Message.h"
#include "HashTable.h"
#include "Interface.h"
#include "MessengerWnd.h"
#include "Chat.h"
#include "GroupWnd.h"
//==============================================================//
// 메신저에 등록된 친구를 등록한다.								//
//==============================================================//
void CmdMessengerUser( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_USER* pPacket = (WSTC_MESSENGER_USER*)pMsg;

	LP_MESSENGER_USER lpMessengerUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);
	
	if(lpMessengerUser)
	{		
		memset(lpMessengerUser, 0, sizeof(MESSENGER_USER));
		lpMessengerUser->byType			= pPacket->byType;
		lpMessengerUser->dwUserIndex	= pPacket->dwUserIndex;
		lpMessengerUser->wClass			= pPacket->wClass;	
		lpMessengerUser->byStatus		= pPacket->byStatus;
		__lstrcpyn(lpMessengerUser->szName, pPacket->szName, MAX_CHARACTER_NAME_LENGTH);
		
		if(lpMessengerUser->byType==0)
			g_pMessengerOnLineHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);
		else
			g_pMessengerOffLineHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive()==TRUE)
	{
		if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
		{
			pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);
			pGroupWnd->SetStatus(GROUP_FRIEND_DEFAULT);
		}
		pGroupWnd->m_nPartyUserIndex = -1;		
		SPR(SPR_OBJ_GROUPWND_SELECT)->ShowSprite(FALSE);		
	}
}
//==============================================================//
// 메신저에 등록된 친구 로그인/로그아웃 설정.					//
//==============================================================//
void CmdMessengerUserInfo( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_USERINFO* pPacket = (WSTC_MESSENGER_USERINFO*)pMsg;

	LP_MESSENGER_USER lpMessengerUser		= NULL;
	LP_MESSENGER_USER lpMessengerCopyUser	= NULL;

	char szInfo[0xff] = {0,};	

	if(pPacket->byType==0)
	{
		lpMessengerUser = g_pMessengerOffLineHash->GetData(pPacket->dwUserIndex);

		if(lpMessengerUser)
		{
			lpMessengerCopyUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);
			
			memcpy(lpMessengerCopyUser, lpMessengerUser, sizeof(MESSENGER_USER));
			lpMessengerCopyUser->byType	= pPacket->byType;
			g_pMessengerOffLineHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex);
			g_pMessengerOnLineHash->Insert(lpMessengerCopyUser, lpMessengerCopyUser->dwUserIndex);			
			memset(lpMessengerUser, 0, sizeof(lpMessengerUser));
			
			wsprintf(szInfo, g_Message[ETC_MESSAGE796].szMessage, lpMessengerUser->szName); // "%s 님이 로그인 하셨습니다."
			DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
		}
	}
	else
	{
		lpMessengerUser = g_pMessengerOnLineHash->GetData(pPacket->dwUserIndex);

		if(lpMessengerUser)
		{
			lpMessengerCopyUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);

			memcpy(lpMessengerCopyUser, lpMessengerUser, sizeof(MESSENGER_USER));
			lpMessengerCopyUser->byType	= pPacket->byType;
			g_pMessengerOnLineHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex);
			g_pMessengerOffLineHash->Insert(lpMessengerCopyUser, lpMessengerCopyUser->dwUserIndex);
			memset(lpMessengerUser, 0, sizeof(lpMessengerUser));
			
			wsprintf(szInfo, g_Message[ETC_MESSAGE109].szMessage, lpMessengerUser->szName); // "%s 님이 로그 아웃 하셨습니다."
			DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
		}
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive()==TRUE)
	{
		if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
		{
			pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);
			pGroupWnd->SetStatus(GROUP_FRIEND_DEFAULT);
		}
		pGroupWnd->m_nPartyUserIndex = -1;
		pGroupWnd->SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);		
	}
}
//==============================================================//
// 메신저에 등록 실패했을 경우 에러 메세지.						//
//==============================================================//
void CmdMessengerUserFail( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_USERFAIL* pPacket = (WSTC_MESSENGER_USERFAIL*)pMsg;

	DWORD dwMsgId = 0;
	switch(pPacket->byFail)
	{
	case 0: dwMsgId = ETC_MESSAGE273; break; // "상대방이 없습니다."
	case 1: dwMsgId = ETC_MESSAGE272; break; // "상대방이 더 이상 친구를 등록할 수 없습니다."
	case 2: dwMsgId = ETC_MESSAGE163; break; // "더 이상 친구를 등록할 수 없습니다."
	case 3: dwMsgId = ETC_MESSAGE831; break; // "상대방에 친구 목록에 등록되어있습니다."
	}

	DisplayMessageAdd(g_Message[dwMsgId].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
}

//==============================================================//
// 메신저에 등록 실패했을 경우 에러 메세지.						//
//==============================================================//
void CmdMessengerUserAdd( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_USERADD* pPacket = (WSTC_MESSENGER_USERADD*)pMsg;
	
	CMessengerWnd* pMessengerWnd = CMessengerWnd::GetInstance();
	memset(pMessengerWnd->m_szName, 0, sizeof(pMessengerWnd->m_szName));	
	__lstrcpyn(pMessengerWnd->m_szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH );
	pMessengerWnd->m_bChk			= TRUE;
	pMessengerWnd->m_dwUserIndex	= pPacket->dwUserIndex;	
	pMessengerWnd->SetActive();
}
//==============================================================//
// 메신저에 등록 거부 메세지.									//
//==============================================================//
void CmdMessengerFail( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_FAIL* pPacket = (WSTC_MESSENGER_FAIL*)pMsg;

	char szInfo[0xff] = {0,};
	wsprintf(szInfo, g_Message[ETC_MESSAGE745].szMessage, pPacket->szName); // "%s 님이 친구 등록을 거부하셨습니다."
	DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
}
//==============================================================//
// 메신저에서 친구 삭제 처리.									//
//==============================================================//
void CmdFriendDelete( char* pMsg, DWORD dwLen )
{
	WSTS_FRIENDUSER_DELETE* pPacket = (WSTS_FRIENDUSER_DELETE*)pMsg;

	LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(pPacket->dwUserIndex);

	char szInfo[0xff] = {0,};

	if(lpMessengerUser)
	{		
		wsprintf(szInfo, g_Message[ETC_MESSAGE746].szMessage, lpMessengerUser->szName); // "%s 님이 친구 리스트에서 삭제되었습니다."
		DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);

		g_pMessengerOnLineHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex);
		memset(lpMessengerUser, 0, sizeof(lpMessengerUser));
		lpMessengerUser = NULL;
	}

	lpMessengerUser = g_pMessengerOffLineHash->GetData(pPacket->dwUserIndex);

	if(lpMessengerUser)
	{
		wsprintf(szInfo, g_Message[ETC_MESSAGE746].szMessage, lpMessengerUser->szName); // "%s 님이 친구 리스트에서 삭제되었습니다."
		DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
		
		g_pMessengerOffLineHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex);
		memset(lpMessengerUser, 0, sizeof(lpMessengerUser));
		lpMessengerUser = NULL;
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive()==TRUE)
	{
		if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
		{
			pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);
			pGroupWnd->SetStatus(GROUP_FRIEND_DEFAULT);
		}
		pGroupWnd->m_nPartyUserIndex = -1;
		pGroupWnd->SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
	}	
}
//==============================================================//
// 메신저에서 친구 등록 성공 처리.								//
//==============================================================//
void CmdMessengerSuccess( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_SUCCESS* pPacket = (WSTC_MESSENGER_SUCCESS*)pMsg;

	char szInfo[0xff] = {0,};
	wsprintf(szInfo, g_Message[ETC_MESSAGE748].szMessage, pPacket->szName); // "%s 님이 친구 리스트에 등록되었습니다."
	DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);

	LP_MESSENGER_USER lpMessengerUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);
	
	if(lpMessengerUser)
	{		
		
		memset(lpMessengerUser, 0, sizeof(MESSENGER_USER));
		lpMessengerUser->byType			= pPacket->byType;
		lpMessengerUser->dwUserIndex	= pPacket->dwUserIndex;
		lpMessengerUser->wClass			= pPacket->wClass;		
		__lstrcpyn(lpMessengerUser->szName, pPacket->szName, MAX_CHARACTER_NAME_LENGTH);
		
		LP_MESSENGER_USER lpDeleteUser  = g_pMessengerOffLineHash->GetData(lpMessengerUser->dwUserIndex);

		if(lpDeleteUser)
			g_pMessengerOffLineHash->Delete(lpDeleteUser, lpMessengerUser->dwUserIndex, TRUE);

		if(lpMessengerUser->byType==0)
			g_pMessengerOnLineHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);
		else
			g_pMessengerOffLineHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);
	}

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if(pGroupWnd->GetActive()==TRUE)
	{
		if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
		{
			pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);
			pGroupWnd->SetStatus(GROUP_FRIEND_DEFAULT);
		}
		pGroupWnd->m_nPartyUserIndex = -1;
		pGroupWnd->SetRender(SPR_OBJ_GROUPWND_SELECT, FALSE);
	}
}
//==============================================================//
// 메신저에서 친구 정보 보기.									//
//==============================================================//
void CmdFriendInfo( char* pMsg, DWORD dwLen )
{
	WSTC_FRIENDUSER_INFO* pPacket = (WSTC_FRIENDUSER_INFO*)pMsg;

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
// 메신저에서 친구 상태 변경.									//
//==============================================================//
void CmdFriendStaus( char* pMsg, DWORD dwLen )
{
	WSTC_MESSENGER_STATUS* pPacket = (WSTC_MESSENGER_STATUS*)pMsg;
	
	LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(pPacket->dwUserIndex);

	if(lpMessengerUser)
	{
		lpMessengerUser->byStatus	= pPacket->byType;

		CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

		if(pGroupWnd->GetActive()==TRUE)
		{
			if(pGroupWnd->m_byGroupTab==GROUP_TAB_FRIEND)
				pGroupWnd->SetPrevNext(__FRIEND_PAGE_CURR__);		
		}
	}	
}
//==============================================================//
// End.															//
//==============================================================//