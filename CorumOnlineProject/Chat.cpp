//==============================================================//
// Code by Jang.									2003.08.01	//
//==============================================================//
#include "Chat.h"
#include "GameControl.h"
#include "../CommonServer/CommonHeader.h"
#include "User.h"
#include "Message.h"
#include "HashTable.h"
#include "Interface.h"
#include "GroupWnd.h"
#include "DungeonProcess.h"
#include "ChatListWnd.h"
#include "ChatWnd.h"
#include "GlobalSoundHandles.h"
#include "InitGame.h"
#include "ChatBackground.h"
#include "CodeFun.h"
#include "InputManager.h"
#include "WorldProcess.h"
#include "GameMenuWnd.h"

extern int			g_nDgIndex;
extern int			g_nDgCount;
extern SDGCHATMSG	g_sDgChatMsg[__MAX_QUEUE__];
//==============================================================//
// 던전에서 메세지 초기화.										//
//==============================================================//
void InitDungeonMessage()
{
	for(int i = 0; i < __MAX_QUEUE__; i++)
	{
		memset(g_sDgChatMsg[i].szDungeon, 0, sizeof(g_sDgChatMsg[i].szDungeon));		
		g_sDgChatMsg[i].sTimeIndex		= 0;
		g_sDgChatMsg[i].sdwColor		= 0xffffffff;
		g_sDgChatMsg[i].sbActive		= FALSE;
		g_sDgChatMsg[i].snSize			= 0;
	}
	
	for(i = 0; i < __MAX_CHAT_LIST_QUEUE__; i++)
	{
		memset(g_sDgChatListMsg[i].szMessage, 0, sizeof(g_sDgChatListMsg[i].szMessage));
		g_sDgChatListMsg[i].snSize		= 0;
		g_sDgChatListMsg[i].sdwColor	= 0xffffffff;
	}
	g_nDgChatListIndex	= 0;
	g_nDgIndex			= 0;
	g_nDgChatListStart	= 0;
	g_nDgChatListEnd	= 0;
}
//==============================================================//
// 던전에서 메세지 추가.										//
//==============================================================//
int DungeonMessageAdd(char* pszMessage, DWORD dwCol, BOOL bChkSystemMsg, BYTE byType)
{			
	ChatListAdd(pszMessage, dwCol, byType, bChkSystemMsg);	
	return 2;
}
//==============================================================//
// 던전에서 메세지 추가.										//
//==============================================================//
int MessageAdd(char* pszMessage, DWORD dwCol, BOOL bChkSystemMsg, int nSize)
{
	// Trash String Attached by [strncpy] function. So, [strncpy] change to [__lstrcpyn]
	// [strcpy] change to [lstrcpy]
	// 2004. 08. 23.
	// Begin - minjin

//	CChatListWnd* pChatListWnd = CChatListWnd::GetInstance();

	if(bChkSystemMsg)// && pChatListWnd->GetActive())
	{
		// 채팅 리스트 메세지 //
		if(g_nDgChatListIndex<__MAX_CHAT_LIST_QUEUE__)
		{
			g_sDgChatListMsg[g_nDgChatListIndex].sdwColor	= dwCol;
			
			// memory init -_- by deepdark.
		//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));
			
			if(nSize)
			{
//				strncat(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, "\0", 1);
				__lstrcpyn(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, pszMessage, nSize);
			}
			else
			{
//				::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//				strcpy(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, pszMessage);
				lstrcpy(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, pszMessage);
			}

			g_sDgChatListMsg[g_nDgChatListIndex].snSize = lstrlen(g_sDgChatListMsg[g_nDgChatListIndex].szMessage);
			g_nDgChatListIndex++;
			g_nDgChatListEnd++;
		}	
		else
		{
			if(g_nDgChatListStart==__MAX_CHAT_LIST_QUEUE__-1)
				g_nDgChatListStart = 0;
			else
				g_nDgChatListStart++;

			if(g_nDgChatListEnd>=__MAX_CHAT_LIST_QUEUE__-1)
				g_nDgChatListEnd = 0;
			else
				g_nDgChatListEnd++;

			g_sDgChatListMsg[g_nDgChatListEnd].sdwColor	= dwCol;

			// memory init -_- by deepdark.
		//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));

			if(nSize)
			{
//				strncat(g_sDgChatListMsg[g_nDgChatListEnd].szMessage, "\0", 1);
				__lstrcpyn(g_sDgChatListMsg[g_nDgChatListEnd].szMessage, pszMessage, nSize);
			}
			else
			{
//				::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//				strcpy(g_sDgChatListMsg[g_nDgChatListEnd].szMessage, pszMessage);
				lstrcpy(g_sDgChatListMsg[g_nDgChatListEnd].szMessage, pszMessage);
			}

			g_sDgChatListMsg[g_nDgChatListEnd].snSize = lstrlen(g_sDgChatListMsg[g_nDgChatListEnd].szMessage);
		}
		// Active == FALSE
		for(int i = g_nDgIndex; i < __MAX_QUEUE__; i++)
		{
			if(g_sDgChatMsg[i].sbActive==FALSE)
			{
				// memory init -_- by deepdark.
			//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));

				if(nSize)
				{
//					strncat(g_sDgChatMsg[i].szDungeon, "\0", 1);
					__lstrcpyn(g_sDgChatMsg[i].szDungeon, pszMessage, nSize);
				}
				else
				{
//					::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//					strcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
					lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
				}

				g_sDgChatMsg[i].sTimeIndex	= 0;		
				g_sDgChatMsg[i].sdwColor	= dwCol;
				g_sDgChatMsg[i].sbActive	= TRUE;
				g_sDgChatMsg[i].snSize		= lstrlen(pszMessage);
				
				if(g_nDgIndex<__MAX_QUEUE__-1)
					g_nDgIndex++;
				else
					g_nDgIndex = 0;

				if(g_nDgCount<__MAX_QUEUE__)
					g_nDgCount++;

				return 1;
			}		
		}
		for(i = 0; i < g_nDgIndex; i++)
		{
			if(g_sDgChatMsg[i].sbActive==FALSE)
			{
				// memory init -_- by deepdark.
			//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));

				if(nSize)
				{
//					strncat(g_sDgChatMsg[i].szDungeon, "\0", 1);
					__lstrcpyn(g_sDgChatMsg[i].szDungeon, pszMessage, nSize);
				}
				else
				{
//					::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//					lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
					lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
				}

				g_sDgChatMsg[i].sTimeIndex	= 0;		
				g_sDgChatMsg[i].sdwColor		= dwCol;
				g_sDgChatMsg[i].sbActive		= TRUE;
				g_sDgChatMsg[i].snSize			= lstrlen(pszMessage);
				
				if(g_nDgIndex<__MAX_QUEUE__-1)
					g_nDgIndex++;
				else
					g_nDgIndex = 0;

				if(g_nDgCount<__MAX_QUEUE__)
					g_nDgCount++;

				return 1;
			}
		}
		// Non Active == FALSE
		
		// memory init -_- by deepdark.
	//	ZeroMemory(g_sDgChatListMsg[g_nDgChatListIndex].szMessage, sizeof(g_sDgChatListMsg[g_nDgChatListIndex].szMessage));

		if(nSize)
		{
//			strncat(g_sDgChatMsg[g_nDgIndex].szDungeon, "\0", 1);
			__lstrcpyn(g_sDgChatMsg[g_nDgIndex].szDungeon, pszMessage, nSize);
		}
		else
		{
//			::MessageBox(g_hMainWnd, "nSize is ZERO", "ERROR", MB_OK);
//			lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
			lstrcpy(g_sDgChatMsg[i].szDungeon, pszMessage);
		}

		g_sDgChatMsg[g_nDgIndex].sTimeIndex	= 0;
		g_sDgChatMsg[g_nDgIndex].sdwColor	= dwCol;
		g_sDgChatMsg[g_nDgIndex].sbActive	= TRUE;
		g_sDgChatMsg[g_nDgIndex].snSize		= lstrlen(pszMessage);
		
		if(g_nDgIndex<__MAX_QUEUE__-1)
			g_nDgIndex++;
		else
			g_nDgIndex = 0;

		if(g_nDgCount<__MAX_QUEUE__)
			g_nDgCount++;
	}

	// End - minjin

	return 1;
}
//==============================================================//
// 던전에서 메세지 처리.										//
//==============================================================//
void ProcessMessgae()
{
	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();
	for(int i = 0; i < __MAX_QUEUE__; i++)
	{
		if(g_sDgChatMsg[i].sbActive==TRUE)
		{
			g_sDgChatMsg[i].sTimeIndex++;

			//if(g_sDgChatMsg[i].sTimeIndex>600)     //(정우창 2005.03.03) -  채팅큐(좌측하단에 채팅글 올라가는 부분) 시간 조정 부분 
			if(g_sDgChatMsg[i].sTimeIndex > pGameMenuWnd->m_byChatRemainTime * 510)// sung-han 2005-03-30 노멀채팅 표시 사라지는 시간
			{
				g_sDgChatMsg[i].sbActive = FALSE;
				g_nDgCount--;
			}
		}
	}
}
//==============================================================//
// 쪽지 보내기 실패했을 경우 처리.								//
//==============================================================//
void CmdDGFail(char* pMsg, DWORD dwLen)
{
//	char*	szTemp	= 0;
//	int		nSize	= 0;
	char	szInfo[0xff] = {0,};
	DWORD	dwColor = 0xffff9a63;	

 	WSTC_DSF_CHAT* pPacket = (WSTC_DSF_CHAT*)pMsg;

	if(pPacket->enCommand==COMMAND_WHISPER)
		wsprintf(szInfo, g_Message[ETC_MESSAGE94].szMessage, pPacket->szId);			// MSG_ID : 94 ; %s 캐릭터가 없습니다. 캐릭터 이름을 다시 확인해 주십시오.
	else if(pPacket->enCommand==COMMAND_MESSAGE)
	{
		if(pPacket->nFail==1)
		{			
			wsprintf(szInfo, g_Message[ETC_MESSAGE681].szMessage, pPacket->szId);	//"%s 캐릭터가 로그인 되어있지 않습니다.", 
			DisplayMessageAdd(szInfo, dwColor, TRUE, DEF_CHAT_TYPE_WHISPER);
			lstrcpy(szInfo, g_Message[ETC_MESSAGE682].szMessage);					//"쪽지를 저장하겠습니다.");			
		}
		else if(pPacket->nFail==0)
		{
			wsprintf(szInfo, g_Message[ETC_MESSAGE683].szMessage, pPacket->szId);	//"%s 캐릭터가 없어서 쪽지를 저장할 수 없습니다.", 			
			DisplayMessageAdd(szInfo, dwColor, TRUE, DEF_CHAT_TYPE_WHISPER);
			lstrcpy(szInfo, g_Message[ETC_MESSAGE684].szMessage);					// "캐릭터 이름을 다시 확인해 주십시오.");			 
		}
		else if(pPacket->nFail==2)
			wsprintf(szInfo, g_Message[ETC_MESSAGE426].szMessage, pPacket->szId);	// MSG_ID : 97 ; %s 캐릭터에 쪽지함이 다 차서 보낼수가 없습니다.
	}		
	DisplayMessageAdd(szInfo, dwColor, TRUE, DEF_CHAT_TYPE_WHISPER);	
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
}
//==============================================================//
// 귓속말 처리.													//
//==============================================================//
void CmdDGWhisper(char* pMsg, DWORD dwLen)
{	
	char szInfo[0xff] = {0,};
	
	WSTC_DSCHAT_WHISPER* pPacket = (WSTC_DSCHAT_WHISPER*)pMsg;

	if(pPacket->nSuccess==1)
	{		
		wsprintf(szInfo, g_Message[ETC_MESSAGE97].szMessage, pPacket->szId, pPacket->szMessage);	// MSG_ID : 97 ; 귓속말 %s : %s\n\n
	}
	else if(pPacket->nSuccess==2)
		wsprintf(szInfo, g_Message[ETC_MESSAGE98].szMessage, pPacket->szId);						// MSG_ID : 98 ; 귓속말을 %s님 전했습니다.\n\n

	DisplayMessageAdd(szInfo, 0xffff9a63, TRUE, DEF_CHAT_TYPE_WHISPER);		
}
//==============================================================//
// 쪽지 처리.													//
//==============================================================//
void CmdDGMessage(char* pMsg, DWORD dwLen)
{	
	char szInfo[0xff] = {0,};
	
	CTWS_DSCHAT_COMMAND* pPacket = (CTWS_DSCHAT_COMMAND*)pMsg;

	if(pPacket->nSuccess==1)
		wsprintf(szInfo, g_Message[ETC_MESSAGE99].szMessage, pPacket->szId, pPacket->szMessage);	// MSG_ID : 99 ; 쪽지 %s : %s\n\n
	else if(pPacket->nSuccess==2)
		wsprintf(szInfo, g_Message[ETC_MESSAGE100].szMessage, pPacket->szId);					// MSG_ID : 100 ; 쪽지를 %s님 전했습니다.\n\n

	DisplayMessageAdd(szInfo, 0xffff9a63, TRUE, DEF_CHAT_TYPE_WHISPER);		
}
//==============================================================//
// 던전에서 일반채팅 처리.										//
//==============================================================//
void CmdDGChatUser(char* pMsg, DWORD dwLen)
{
	// Modified by minjin. if statement -> switch-case statement // 2004. 10. 01.
	char szInfo[0xff] = {0,};
	WSTC_DGCHAT_USER* pPacket = (WSTC_DGCHAT_USER*)pMsg;			
	wsprintf( szInfo, "%s : %s", pPacket->szId, pPacket->szMessage );

	// 근데.. 여기 이 if 문이 멀 하는건가요..? ^^;;;
	if ( GetGameStatus() == UPDATE_GAME_WORLD )
	{
		switch ( pPacket->nIndex )
		{
		case 2:	DisplayMessageAdd( szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD );				break;
		case 3:	DisplayMessageAdd( szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY );				break;
		case 4:	DisplayMessageAdd( szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND );			break;
		case 5:	DisplayMessageAdd( pPacket->szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD );	break;
		}
/*		if(pPacket->nIndex==2)
			DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
		else if(pPacket->nIndex==3)
			DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
		else if(pPacket->nIndex==4)
			DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);
		else if(pPacket->nIndex==5)
			DisplayMessageAdd(pPacket->szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
//*/}
	else
	{
		switch ( pPacket->nIndex )
		{
		case 2:	DisplayMessageAdd( szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD );				break;
		case 3:	DisplayMessageAdd( szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY );				break;
		case 4:	DisplayMessageAdd( szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND );			break;
		case 5:	DisplayMessageAdd( pPacket->szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD );	break;
		}
/*		if(pPacket->nIndex==2)
			DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);			
		else if(pPacket->nIndex==3)
			DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);			
		else if(pPacket->nIndex==4)
			DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);			
		else if(pPacket->nIndex==5)	
			DisplayMessageAdd(pPacket->szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
//*/}
}
//==============================================================//
// 월드에서 일반채팅 메세지 처리.								//
//==============================================================//
void WorldMessageAdd(char* pszMessage, DWORD dwChatColor)
{	
	lstrcpy(g_sWordChatMsg[g_nMsgInx].szDungeon, pszMessage);
	g_sWordChatMsg[g_nMsgInx].sdwChatColor	= dwChatColor;
	g_sWordChatMsg[g_nMsgInx].snSize = lstrlen(pszMessage);

	if(g_nMsgInx<__MAX_WORD_QUEUE__-1)
		g_nMsgInx++;
	else
		g_nMsgInx = 0;
	
	if(g_nWordEnd<__MAX_WORD_QUEUE__-1)
	{		
		if(g_nWordIndex!=0)
			g_nWordEnd++;
	}
	else
		g_nWordEnd = 0;
		
	if(g_nWordIndex==__MAX_WORD_QUEUE__)
	{
		if(g_nWordStart<__MAX_WORD_QUEUE__-1)
			g_nWordStart++;
		else
			g_nWordStart = 0;
	}

	if(g_nWordIndex<__MAX_WORD_QUEUE__)
		g_nWordIndex++;	

	if(g_nWordIndex>__MAX_CHAT_INDEX__)
	{	
		if(g_nScrollPos<__MAX_CHAT_SCROLL__)
			g_nScrollPos++;

		g_nCurScrollPos = g_nScrollPos;
	}

	if(g_nScrollPos>0)
	{
		//SPR(SPR_CHAT_SC)->vPos.y = 730-16;
	}
}
//==============================================================//
// 메세지 리스트에 추가(모든 메세지는 DisplayMessageAdd 호출).	//
//==============================================================//
int DisplayMessageAdd(char* pszMessage, DWORD dwCol, BOOL bChk, BYTE byType)
{	
	if (GetGameStatus()==UPDATE_GAME_PLAY)
		return DungeonMessageAdd(pszMessage, dwCol, bChk, byType);
	else
		WorldMessageAdd(pszMessage, dwCol);

	return 0;
}
//==============================================================//
// 특정 채팅 메세지 리스트에서 삭제.							//
//==============================================================//
void RemoveChat(POSITION_ pos, BYTE byType)
{
	if(!pos)
		return;
	
	LP_CHAT_MSG	lpChatMsg = NULL;
	
	switch(byType)
	{
	case DEF_CHAT_TYPE_ALLCHAT:
		lpChatMsg	= (LP_CHAT_MSG)g_pAllChatList->GetNext(pos);	
		break;
	case DEF_CHAT_TYPE_WHISPER:
		lpChatMsg	= (LP_CHAT_MSG)g_pWhisperChatList->GetNext(pos);			
		break;
	case DEF_CHAT_TYPE_GUILD:
		lpChatMsg = (LP_CHAT_MSG)g_pGuildChatList->GetNext(pos);
		break;
	case DEF_CHAT_TYPE_PARTY:
		lpChatMsg = (LP_CHAT_MSG)g_pPartyChatList->GetNext(pos);
		break;
	case DEF_CHAT_TYPE_FRIEND:
		lpChatMsg = (LP_CHAT_MSG)g_pFriendChatList->GetNext(pos);
		break;		
	default:
		return;
	}

	if(lpChatMsg)
	{
		LALFree(g_pChatPool, lpChatMsg);
		lpChatMsg = NULL;
	}
}
//==============================================================//
// 특정 채팅 메세지 리스트에서 추가.							//
//==============================================================//
void AddChat(char* pszText, DWORD dwColor, BYTE byType, int nSize)
{
	LP_CHAT_MSG lpChatMsg = (LP_CHAT_MSG)LALAlloc(g_pChatPool);
	memset(lpChatMsg->szText, 0, sizeof(lpChatMsg->szText));

	if(nSize)
		__lstrcpyn(lpChatMsg->szText, pszText, nSize);
	else
		lstrcpy(lpChatMsg->szText, pszText);

	lpChatMsg->dwColor	= dwColor;

	switch(byType)
	{
	case DEF_CHAT_TYPE_ALLCHAT:
		g_pAllChatList->AddTail(lpChatMsg);
		break;
	case DEF_CHAT_TYPE_WHISPER:
		g_pWhisperChatList->AddTail(lpChatMsg);
		break;
	case DEF_CHAT_TYPE_GUILD:
		g_pGuildChatList->AddTail(lpChatMsg);		
		break;		
	case DEF_CHAT_TYPE_PARTY:
		g_pPartyChatList->AddTail(lpChatMsg);
		break;		
	case DEF_CHAT_TYPE_FRIEND:
		g_pFriendChatList->AddTail(lpChatMsg);		
		break;		
	default:
		{
			if(lpChatMsg)
			{
				LALFree(g_pChatPool, lpChatMsg);
				lpChatMsg = NULL;
			}
		}
		return;
	}	
	CChatListWnd::GetInstance()->SetScroll();
}
//==============================================================//
// 메세지 리스트가 다 차면 먼저 들어온 메세지 삭제.				//
//==============================================================//
void ChkMaxChaList(BYTE byType)
{
	switch(byType)
	{
	case DEF_CHAT_TYPE_ALLCHAT:		
		{
			if(g_pAllChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pAllChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pAllChatList->RemoveHead();
			}
		}
		break;
	case DEF_CHAT_TYPE_WHISPER:
		{
			if(g_pWhisperChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pWhisperChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pWhisperChatList->RemoveHead();
			}
		}		
		break;
	case DEF_CHAT_TYPE_GUILD:
		{
			if(g_pGuildChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pGuildChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pGuildChatList->RemoveHead();
			}
		}		
		break;		
	case DEF_CHAT_TYPE_PARTY:
		{
			if(g_pPartyChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pPartyChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pPartyChatList->RemoveHead();
			}
		}		
		break;		
	case DEF_CHAT_TYPE_FRIEND:
		{
			if(g_pFriendChatList->GetCount()==__MAX_MSG_CNT__)
			{
				POSITION_ pos = g_pFriendChatList->GetHeadPosition();	
				RemoveChat(pos, byType);
				g_pFriendChatList->RemoveHead();
			}
		}		
		break;		
	default:		
		return;
	}		
}
//==============================================================//
// 메세지 사이즈 조절해서 추가하는 부분.						//
//==============================================================//
void ChatListAdd(char* pszText, DWORD dwCol, BYTE byType, BOOL bChkSystemMsg)
{
	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();

	char*	szTemp = {0, };
	int		nSize	= 0;
	
	szTemp	= pszText;	
	nSize	= lstrlen(pszText);
	
	while(nSize>0)
	{
		if(nSize>MAX_CHATLIST_TEXT)
		{
			for(;;)
			{
				if(nSize > MAX_CHATLIST_TEXT)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+MAX_CHATLIST_TEXT, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev);
	
					ChkMaxChaList(byType);
					AddChat(szTemp, dwCol, byType, nSize);
					// sung-han 2005-03-30 노말 채팅창 필터링
					if( ( byType == DEF_CHAT_TYPE_ALLCHAT )
						|| ( pGameMenuWnd->m_bChatKindWhisper && byType == DEF_CHAT_TYPE_WHISPER )
						|| ( pGameMenuWnd->m_bChatKindGuild && byType == DEF_CHAT_TYPE_GUILD )
						|| ( pGameMenuWnd->m_bChatKindParty && byType == DEF_CHAT_TYPE_PARTY )
						|| ( pGameMenuWnd->m_bChatKindFriend && byType == DEF_CHAT_TYPE_FRIEND ) )
					{
						MessageAdd(szTemp, dwCol, bChkSystemMsg, nSize);
					}

					szTemp	= szPrev;
					nSize	= lstrlen(szTemp);
				}
				else	// 구한 문자열을 채팅 라인에 쓰고, 길이를 기록한다.
				{
					ChkMaxChaList(byType);
					AddChat(szTemp, dwCol, byType, nSize);
					// sung-han 2005-03-30 노말 채팅창 필터링
					if( ( byType == DEF_CHAT_TYPE_ALLCHAT )
						|| ( pGameMenuWnd->m_bChatKindWhisper && byType == DEF_CHAT_TYPE_WHISPER )
						|| ( pGameMenuWnd->m_bChatKindGuild && byType == DEF_CHAT_TYPE_GUILD )
						|| ( pGameMenuWnd->m_bChatKindParty && byType == DEF_CHAT_TYPE_PARTY )
						|| ( pGameMenuWnd->m_bChatKindFriend && byType == DEF_CHAT_TYPE_FRIEND ) )
					{
						MessageAdd(szTemp, dwCol, bChkSystemMsg, nSize);
					}
					nSize = 0;
					break;
				}
			}
		}
		else
		{
			ChkMaxChaList(byType);
			AddChat(szTemp, dwCol, byType);
			// sung-han 2005-03-30 노말 채팅창 필터링
			if( ( byType == DEF_CHAT_TYPE_ALLCHAT )
				|| ( pGameMenuWnd->m_bChatKindWhisper && byType == DEF_CHAT_TYPE_WHISPER )
				|| ( pGameMenuWnd->m_bChatKindGuild && byType == DEF_CHAT_TYPE_GUILD )
				|| ( pGameMenuWnd->m_bChatKindParty && byType == DEF_CHAT_TYPE_PARTY )
				|| ( pGameMenuWnd->m_bChatKindFriend && byType == DEF_CHAT_TYPE_FRIEND ) )
			{
				MessageAdd(szTemp, dwCol, bChkSystemMsg);
			}
			nSize = 0;
		}
	}
}


#ifdef _USE_IME

void EnableChatInput(BOOL bClear, DWORD dwIndex)
{
	POINT pChatPoint;
	pChatPoint.x	= 10;
	pChatPoint.y	= 635;

	if(bClear)	GET_IMEEDIT()->ClearImeEditString();
	if(dwIndex)	GET_IMEEDIT()->SetEditIndex(dwIndex);
	GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_BUUFER_SIZE__);

	ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
}
#endif

void ChatModeOn(DWORD dwInputId, BOOL bClear)
{
#ifndef _USE_IME
	ChangeLanguage(TRUE);
#endif	
	
	g_pGVDungeon->bChatMode = TRUE;	
	
	if(bClear)
		g_pInputManager->ClearInput(dwInputId);

	g_pInputManager->SetFocusInput(dwInputId);

}

void ChatModeOff(DWORD dwInputId, DWORD dwFocusId, BOOL bChk, BOOL bClear)
{
#ifndef _USE_IME
	ChangeLanguage(FALSE);
#endif	

	if(bChk)
	{
		g_nChatModeExt			= 0;
		g_pGVDungeon->bChatMode = FALSE;	
	}	

	if(bClear)
		g_pInputManager->ClearInput(dwInputId);
	
	g_pInputManager->SetFocusInput(dwFocusId);	
}

void IMESetEdit(DWORD dwIndex, BOOL bChk, BOOL bBack, BOOL bNative)
{
#ifdef _USE_IME

	if(GET_IMEEDIT()->IsActivated())
	{
		GET_IMEEDIT()->DeactivateIME();

		if(bChk)
			GET_IMEEDIT()->SetEditIndex(dwIndex);

		if(bBack)
			ShowChatEditBackground(FALSE, 0);

		if(bNative)
			GET_IMEEDIT()->SetNativeLanguageMode(FALSE);
	}

#endif
}
//==============================================================//
// End.															//
//==============================================================//