//==========================================================//
// Code by Jang.								2003.05.03	//
// Object : Guild Edit.										//
//==========================================================//
#include "GuildEditWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "UserInterface.h"
#include "Interface.h"
#include "Filter.h"
#include "message.h"
#include "Chat.h"
#include "InputManager.h"
#include "IMEEdit.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"


CGuildEditWnd* CGuildEditWnd::c_pThis = NULL;

//==========================================================//
// Construction/Destruction.								//
//==========================================================//
CGuildEditWnd::CGuildEditWnd()
{
	m_dwUserIndex	= 0;
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
}

CGuildEditWnd::~CGuildEditWnd()
{
}
//==========================================================//
// Class Member Function.									//
//==========================================================//
BOOL CGuildEditWnd::Init()
{	
/*
	InsertData(SPR_OBJ_GUILDEDIT_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILDEDIT_BAR, SPR_INTERFACE_GUILDEDIT_BAR, 0, 25, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDEDIT_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDEDIT_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDEDIT_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEDIT_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEDIT_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEDIT_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEDIT_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEDIT_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEDIT_CHK, SPR_INTERFACE_OPTION_CHECK, 45, 53, 1.0f, 1.0f, 2);	

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	InsertCheckInterface(174, 41, 185, 52, 6, CHECK_BTN);
	InsertCheckInterface(174, 57, 185, 68, 7, CHECK_BTN);
	InsertCheckInterface(174, 73, 185, 84, 8, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildEditWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildEditWnd::SetData(char* pszName, char* pszNickName, BYTE bRank, DWORD dwUserIndex)
{
	char	szTemp[0xff] = {0,};
	char*	pToken=0;

	__lstrcpyn(m_szCharacterName, pszName, MAX_CHARACTER_NAME_REAL_LENGTH);	
	SetRender(SPR_OBJ_GUILDEDIT_CHK, TRUE);	
	SetRankChk(bRank);

	__lstrcpyn(szTemp, pszNickName, MAX_NICK_REAL_NAME);

	pToken = strtok(szTemp, " ");

	if(pToken!=NULL)
	{
		__lstrcpyn(m_szNickName, pszNickName, MAX_NICK_REAL_NAME);
		g_pInputManager->ClearInput(INPUT_BUFFER_12);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_12, pszNickName, lstrlen(pszNickName));
		
		
	}
	m_byUesrRank	= bRank;
	m_dwUserIndex	= dwUserIndex;
}

void CGuildEditWnd::SetRankChk(BYTE bRank)
{	
	if(bRank!=0)
		m_byRank = bRank;	

	SetPosObjY(SPR_OBJ_GUILDEDIT_CHK, float(25+((m_byRank-1)*16)));
	SetPosObjX(SPR_OBJ_GUILDEDIT_CHK, 174);	
}

void CGuildEditWnd::SetOrder()
{
	g_pInputManager->SetInputOrder(INPUT_BUFFER_12, GetStartOrder()+2);

#ifdef _USE_IME
	
	if(GetActive())
		GET_IMEEDIT()->SetOrder(GetStartOrder()+2);

#endif
}

void CGuildEditWnd::RenderText()
{	
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE285].szMessage);		// MSG_ID : 285 ; 회원정보
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+85, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

	// 이름 //
	int nSize = lstrlen(m_szCharacterName);
		
	if(nSize)
		RenderFont(m_szCharacterName, m_fPosX+32, m_fPosX+32+nSize*7, m_fPosZ+26, m_fPosZ+40, GetStartOrder()+2);	

	// 등급 //	
	if(!IsEmptyString(g_pGuildInfo.szRankName[0]))
	{
		nSize = lstrlen(g_pGuildInfo.szRankName[0]);
		RenderFont(g_pGuildInfo.szRankName[0], m_fPosX+190, m_fPosX+190+nSize*7, m_fPosZ+26, m_fPosZ+40, GetStartOrder()+1);			
	}	

	if(!IsEmptyString(g_pGuildInfo.szRankName[1]))
	{
		nSize = lstrlen(g_pGuildInfo.szRankName[1]);
		RenderFont(g_pGuildInfo.szRankName[1], m_fPosX+190, m_fPosX+190+nSize*7, m_fPosZ+42, m_fPosZ+56, GetStartOrder()+1);
	}

	if(!IsEmptyString(g_pGuildInfo.szRankName[2]))
	{
		nSize = lstrlen(g_pGuildInfo.szRankName[2]);
		RenderFont(g_pGuildInfo.szRankName[2], m_fPosX+190, m_fPosX+190+nSize*7, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);		
	}

	if(!IsEmptyString(g_pGuildInfo.szRankName[3]))
	{
		nSize = lstrlen(g_pGuildInfo.szRankName[3]);
		RenderFont(g_pGuildInfo.szRankName[3], m_fPosX+190, m_fPosX+190+nSize*7, m_fPosZ+72, m_fPosZ+86, GetStartOrder()+1);		
	}
}

void CGuildEditWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();
		
		ChatModeOff(INPUT_BUFFER_12);		
		IMESetEdit(0);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDEDIT_WND, TRUE);
		SetRender(SPR_OBJ_GUILDEDIT_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDEDIT_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDEDIT_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDEDIT_BAR, TRUE);																									
		g_nChatModeExt = 1;
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildEditWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDEDIT_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDEDIT_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDEDIT_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDEDIT_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDEDIT_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDEDIT_CANCEL3, FALSE);
		m_bBtn[1] = FALSE;
	}

	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);

	switch(nRt)
	{		
	case 1:
		{			
			SetMouseCheck(nRt, eMouseCheck);						
		}
		break;
	case 2:
		{			
			if(eMouseCheck==MOUSE_UP)
				SetActive(FALSE);				
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUILDEDIT_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDEDIT_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				if(g_pMainPlayer->m_byRank==__GUILD_MASTER__)
				{
#ifdef _USE_IME
					g_pInputManager->ClearInput(INPUT_BUFFER_12);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_12, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
					
#endif
					
					int nRt = ConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_12));

					if(nRt==1)
					{																		
						DisplayMessageAdd(g_Message[ETC_MESSAGE58].szMessage, 0xFFFF0000);	// MSG_ID : 58 ; 비속어나 욕설은 사용할 수 없습니다.
						g_pInputManager->ClearInput(INPUT_BUFFER_12);
						return CInterface::GetInstance()->GetSetWnd();
					}

					nRt = NoConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_12));
			
					if(nRt==1)
					{				
						DisplayMessageAdd(g_Message[ETC_MESSAGE59].szMessage, 0xFFFF0000);		// MSG_ID : 59 ; 금지 단어입니다. 사용할 수 없습니다.						
						g_pInputManager->ClearInput(INPUT_BUFFER_12);						
						return CInterface::GetInstance()->GetSetWnd();									
					}					
					
					char*	szToken=0;
					char	szText[256] = {0,};
					char	szSeps[]	= "',?";

					__lstrcpyn(szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_12), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_12))+1);

					szToken = strtok(szText, szSeps);

					if(szToken)
					{
						if(__strcmp(szToken, g_pInputManager->GetInputBuffer(INPUT_BUFFER_12))!=0)
						{
							DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; 다음 문자를 사용할수가 없습니다(', ?).
							g_pInputManager->ClearInput(INPUT_BUFFER_12);							
							return CInterface::GetInstance()->GetSetWnd();
						}
					}
#if !IS_KOREA_LOCALIZING() // 일본판에서는 칭호가 ""로 가능하길 원함. 추후 한국판도 적용??
					else
					{
						DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; 다음 문자를 사용할수가 없습니다(', ?).
						g_pInputManager->ClearInput(INPUT_BUFFER_12);
						return CInterface::GetInstance()->GetSetWnd();
					}
#endif
					CTWS_GUILD_INFO pPacket;
					memset(pPacket.szNickName, 0, sizeof(pPacket.szNickName));
					memset(pPacket.szCharacterName, 0, sizeof(pPacket.szCharacterName));
					
					if(__strcmp(m_szNickName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_12))==0)
					{
						if(m_byRank==m_byUesrRank)
						{
							SetActive(FALSE);							
							return CInterface::GetInstance()->GetSetWnd();
						}
						else
							pPacket.bMsgType = __GUILDINFO_MSG_RANK__;
					}
					else
					{
						if(m_byRank==m_byUesrRank)
							pPacket.bMsgType = __GUILDINFO_MSG_NICKNAME__;
						else
							pPacket.bMsgType = __GUILDINFO_MSG_NICKNAMERANK__;
					}					
					
					if(g_pMainPlayer->m_dwUserIndex==m_dwUserIndex)
					{
						if(m_byRank==g_pMainPlayer->m_byRank)
						{
							pPacket.bRank		= m_byRank;
							pPacket.dwUserIndex	= m_dwUserIndex;
							__lstrcpyn(pPacket.szCharacterName, m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							__lstrcpyn(pPacket.szNickName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_12), MAX_NICK_REAL_NAME);
							g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
						}
						else
						{
							char szInfo[0xff] = {0,};
//							wsprintf(szInfo, g_Message[ETC_MESSAGE205].szMessage); // "마스터는 등급을 변경할 수 없습니다."
							lstrcpy(szInfo, g_Message[ETC_MESSAGE205].szMessage); // "마스터는 등급을 변경할 수 없습니다."
							DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_GUILD);	
						}
					}
					else
					{
						pPacket.bRank		= m_byRank;
						pPacket.dwUserIndex	= m_dwUserIndex;
						__lstrcpyn(pPacket.szCharacterName, m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
						__lstrcpyn(pPacket.szNickName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_12), MAX_NICK_REAL_NAME);
						g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					}
					SetActive(FALSE);
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDEDIT_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDEDIT_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDEDIT_OK3, TRUE);								
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_GUILDEDIT_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDEDIT_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDEDIT_OK3, FALSE);				
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
				SetActive(FALSE);
			else if(eMouseCheck==MOUSE_DOWN)
			{								
				SetRender(SPR_OBJ_GUILDEDIT_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDEDIT_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDEDIT_CANCEL3, TRUE);	
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_GUILDEDIT_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDEDIT_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDEDIT_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{				
				if(m_byUesrRank!=__GUILD_MASTER__)
					SetRankChk(__GUILD_MASTER2__);
				else
				{
					DisplayMessageAdd(g_Message[ETC_MESSAGE284].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	// MSG_ID : 284 ; 등급을 변경할수 없습니다.
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 7:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				// 넷클루 요청으로 길드 등급 설정 오류 버그 수정
				// 2003.01.31 김영대   __GUILD_USER__ --> __GUILD_USER2__ -1

				if(m_byUesrRank!=__GUILD_MASTER__)
					SetRankChk(__GUILD_USER2__ -1 );
				else
				{
					DisplayMessageAdd(g_Message[ETC_MESSAGE284].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	// MSG_ID : 284 ; 등급을 변경할수 없습니다.
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
			}			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 8:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byUesrRank!=__GUILD_MASTER__)
					SetRankChk(__GUILD_USER2__);
				else
				{
					DisplayMessageAdd(g_Message[ETC_MESSAGE284].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	// MSG_ID : 284 ; 등급을 변경할수 없습니다.
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
			}			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 9 :
		{
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byUesrRank!=__GUILD_MASTER__)
					SetRankChk(__GUILD_USER2__);
				else
				{
					DisplayMessageAdd(g_Message[ETC_MESSAGE284].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	// MSG_ID : 284 ; 등급을 변경할수 없습니다.
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
			}			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
		
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDEDIT_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDEDIT_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
		RECT rt;
		rt.left = (LONG)m_fPosX+32;	rt.right = (LONG)m_fPosX+150;
		rt.top = (LONG)m_fPosZ+42;	rt.bottom = (LONG)m_fPosZ+54;
		g_pInputManager->InitializeInput(INPUT_BUFFER_12, FONT_SS3D_GAME, rt, 0xffffffff, 101, FALSE, FALSE);
			
#ifdef	_USE_IME

		POINT pChatPoint;
		pChatPoint.x	= long(m_fPosX+32);
		pChatPoint.y	= long(m_fPosZ+42);							
		GET_IMEEDIT()->SetEditPos(pChatPoint.x, pChatPoint.y);

#endif

		SetRankChk();

		return CInterface::GetInstance()->GetSetWnd();
	}

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

//==========================================================//
// End.														//
//==========================================================//

			
