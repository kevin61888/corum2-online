//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "GuildRankWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "Filter.h"
#include "message.h"
#include "Chat.h"
#include "InputManager.h"
#include "IMEEdit.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"


CGuildRankWnd* CGuildRankWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGuildRankWnd::CGuildRankWnd()
{ 	
	m_bBtn[0]	= FALSE;
	m_bBtn[1]	= FALSE;
}

CGuildRankWnd::~CGuildRankWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGuildRankWnd::Init()
{	
/*
	InsertData(SPR_OBJ_GUILDRANK_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILDRANK_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDRANK_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDRANK_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDRANK_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDRANK_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDRANK_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDRANK_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDRANK_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	InsertCheckInterface(58, 45, 253, 57, 6, CHECK_BTN);
	InsertCheckInterface(58, 60, 253, 72, 7, CHECK_BTN);
	InsertCheckInterface(58, 75, 253, 87, 8, CHECK_BTN);
	InsertCheckInterface(58, 90, 253, 102, 9, CHECK_BTN);			
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildRankWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildRankWnd::SetOrder()
{
	g_pInputManager->SetInputOrder(INPUT_BUFFER_8, GetStartOrder()+2);
	g_pInputManager->SetInputOrder(INPUT_BUFFER_9, GetStartOrder()+2);
	g_pInputManager->SetInputOrder(INPUT_BUFFER_10, GetStartOrder()+2);
	g_pInputManager->SetInputOrder(INPUT_BUFFER_11, GetStartOrder()+2);

}

void CGuildRankWnd::RenderText()
{
	if(g_pGuildInfo.byType==__GCTYPE_GUILD__)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE303].szMessage);			// MSG_ID : 303 ; 길드 등급 
	else if(g_pGuildInfo.byType==__GCTYPE_CLEN__)						// MSG_ID : 304 ; 클랜 등급 
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE304].szMessage);

	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+85, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE305].szMessage);				// MSG_ID : 305 ; 등급 명칭을 입력해주십시오.
	RenderFont(m_szInfo, m_fPosX+50, m_fPosX+220, m_fPosZ+28, m_fPosZ+42, GetStartOrder()+1);
		
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE306].szMessage);				// MSG_ID : 306 ; 1등급
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+60, m_fPosZ+45, m_fPosZ+57, GetStartOrder()+1);
	
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE307].szMessage);				// MSG_ID : 307 ; 2등급
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+60, m_fPosZ+60, m_fPosZ+72, GetStartOrder()+1);
	
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE308].szMessage);				// MSG_ID : 308 ; 3등급
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+60, m_fPosZ+75, m_fPosZ+87, GetStartOrder()+1);
	
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE309].szMessage);				// MSG_ID : 309 ; 4등급
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+60, m_fPosZ+90, m_fPosZ+102, GetStartOrder()+1);
}

void CGuildRankWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();

		ChatModeOff(INPUT_BUFFER_8);
		ChatModeOff(INPUT_BUFFER_9);
		ChatModeOff(INPUT_BUFFER_10);
		ChatModeOff(INPUT_BUFFER_11);
		IMESetEdit(0);

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDRANK_WND, TRUE);
		SetRender(SPR_OBJ_GUILDRANK_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDRANK_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDRANK_CLOSE1, TRUE);
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildRankWnd::CheckInterface()
{
		m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDRANK_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDRANK_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDRANK_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDRANK_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDRANK_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDRANK_CANCEL3, FALSE);
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
				SetRender(SPR_OBJ_GUILDRANK_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDRANK_CLOSE2, TRUE);
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
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_8);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_9);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_10);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_11);
				
#ifdef	_USE_IME
										
				switch(g_pInputManager->GetCurFocusInputID())
				{
				case INPUT_BUFFER_8:
					g_pInputManager->ClearInput(INPUT_BUFFER_8);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_8, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					
					break;
				case INPUT_BUFFER_9:					
					g_pInputManager->ClearInput(INPUT_BUFFER_9);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_9, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;
				case INPUT_BUFFER_10:					
					g_pInputManager->ClearInput(INPUT_BUFFER_10);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_10, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;
				case INPUT_BUFFER_11:
					g_pInputManager->ClearInput(INPUT_BUFFER_11);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_11, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;					
				}

#endif
				char	szSeps[]	= "',?";
				char	szTemp[256] = {0,};
				char*	pToken=0;
				char	szText[256] = {0,};

				for(int i = 8; i < 12; i++)
				{
					int nRt = ConvString(g_pInputManager->GetInputBuffer(i));

					if(nRt==1)
					{																		
						DisplayMessageAdd(g_Message[ETC_MESSAGE58].szMessage, 0xFFFF0000);	// MSG_ID : 58 ; 비속어나 욕설은 사용할 수 없습니다.
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
						
						g_pInputManager->ClearInput(INPUT_BUFFER_8);
						g_pInputManager->ClearInput(INPUT_BUFFER_9);
						g_pInputManager->ClearInput(INPUT_BUFFER_10);
						g_pInputManager->ClearInput(INPUT_BUFFER_11);
						
						return CInterface::GetInstance()->GetSetWnd();
					}

					nRt = NoConvString(g_pInputManager->GetInputBuffer(i));
			
					if(nRt==1)
					{				
						DisplayMessageAdd(g_Message[ETC_MESSAGE59].szMessage, 0xFFFF0000);		// MSG_ID : 59 ; 금지 단어입니다. 사용할 수 없습니다.
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
						
						g_pInputManager->ClearInput(INPUT_BUFFER_8);
						g_pInputManager->ClearInput(INPUT_BUFFER_9);
						g_pInputManager->ClearInput(INPUT_BUFFER_10);
						g_pInputManager->ClearInput(INPUT_BUFFER_11);
						return CInterface::GetInstance()->GetSetWnd();									
					}
					
					memset(szText, 0, sizeof(szText));
					__lstrcpyn(szText, g_pInputManager->GetInputBuffer(i), MAX_RANK_REAL_NAME);

					if(!IsEmptyString(szText))
					{
						pToken = strtok(szText, szSeps);

						if(pToken)
						{
							if(__strcmp(pToken, g_pInputManager->GetInputBuffer(i))!=0)
							{
								DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);	// MSG_ID : 57 ; 다음 문자를 사용할수가 없습니다(', ?).
								_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
								
								g_pInputManager->ClearInput(i);
								return CInterface::GetInstance()->GetSetWnd();
							}						
						}					
						else
						{
							DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);		// MSG_ID : 57 ; 다음 문자를 사용할수가 없습니다(', ?).
							_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
							
							g_pInputManager->ClearInput(i);
							return CInterface::GetInstance()->GetSetWnd();
						}
					}					
				}

				// 메세지 보내기 //
				CTWS_GUILD_RANK pPacket;
				memset(pPacket.szRankName[0], 0, sizeof(pPacket.szRankName[0]));
				memset(pPacket.szRankName[1], 0, sizeof(pPacket.szRankName[1]));
				memset(pPacket.szRankName[2], 0, sizeof(pPacket.szRankName[2]));
				memset(pPacket.szRankName[3], 0, sizeof(pPacket.szRankName[3]));
				memset(pPacket.szGuildName, 0, sizeof(pPacket.szGuildName));
				
				__lstrcpyn(pPacket.szGuildName, g_pGuildInfo.szGuildName, MAX_GUILD_NAME_REAL_LENGTH);								

				for(i = 8; i < 12; i++)
				{
					__lstrcpyn(szTemp, g_pInputManager->GetInputBuffer(i), MAX_RANK_REAL_NAME);

					pToken = strtok(szTemp, " ");

					if(pToken==NULL)	
					{
						switch(i)
						{
							case 8:
							case 9:
							case 10:
							case 11:	
								__lstrcpyn(pPacket.szRankName[i-8], g_Message[ETC_MESSAGE60+i-8].szMessage, MAX_RANK_REAL_NAME); break;		// MSG_ID : 63
								break;

						}
					}
					else
					{
						switch(i)
						{
							case 8:	
							case 9:	
							case 10:
							case 11:	
								__lstrcpyn(pPacket.szRankName[i-8], g_pInputManager->GetInputBuffer(i), MAX_RANK_REAL_NAME); break;
								break;
						}												
					}
				}

				for( int iter = 0 ; iter < 4 ; ++iter )
				{
					__lstrcpyn(g_pGuildInfo.szRankName[iter], pPacket.szRankName[iter], MAX_RANK_REAL_NAME);
				}
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				m_bClose = FALSE;
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDRANK_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDRANK_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDRANK_OK3, TRUE);	
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_GUILDRANK_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDRANK_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDRANK_OK3, FALSE);				
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
				SetRender(SPR_OBJ_GUILDRANK_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDRANK_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDRANK_CANCEL3, TRUE);				
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_GUILDRANK_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDRANK_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDRANK_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_8);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_9);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_10);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_11);
										
#ifdef	_USE_IME

				POINT pChatPoint;
				pChatPoint.x	= long(m_fPosX+58);
				pChatPoint.y	= long(m_fPosZ+45);
						
				switch(g_pInputManager->GetCurFocusInputID())
				{
				case INPUT_BUFFER_8:	
					g_pInputManager->ClearInput(INPUT_BUFFER_8);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_8, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					
					break;
				case INPUT_BUFFER_9:					
					g_pInputManager->ClearInput(INPUT_BUFFER_9);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_9, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					
					break;
				case INPUT_BUFFER_10:					
					g_pInputManager->ClearInput(INPUT_BUFFER_10);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_10, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					
					break;
				case INPUT_BUFFER_11:
					g_pInputManager->ClearInput(INPUT_BUFFER_11);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_11, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					
					break;					
				}
				IMESetEdit(0);				
				GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_8));
				GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_RANK_REAL_NAME, GetStartOrder()+2);
#endif
				g_pInputManager->SetFocusInput(INPUT_BUFFER_8);								
			}	
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 7:
		{			
			if(eMouseCheck==MOUSE_UP)
			{				
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_8);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_9);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_10);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_11);
									
#ifdef	_USE_IME

				POINT pChatPoint;
				pChatPoint.x	= long(m_fPosX+58);
				pChatPoint.y	= long(m_fPosZ+60);
						
				switch(g_pInputManager->GetCurFocusInputID())
				{
				case INPUT_BUFFER_8:	
					g_pInputManager->ClearInput(INPUT_BUFFER_8);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_8,(LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					 
					
					break;
				case INPUT_BUFFER_9:					
					g_pInputManager->ClearInput(INPUT_BUFFER_9);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_9,(LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					 
					break;
				case INPUT_BUFFER_10:					
					g_pInputManager->ClearInput(INPUT_BUFFER_10);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_10,(LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					 
					break;
				case INPUT_BUFFER_11:
					g_pInputManager->ClearInput(INPUT_BUFFER_11);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_11,(LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					 
					break;					
				}
				GET_IMEEDIT()->SetEditIndex(1);				
				GET_IMEEDIT()->DeactivateIME();					
				GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_9));
				GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_RANK_REAL_NAME, GetStartOrder()+2);			
#endif
				g_pInputManager->SetFocusInput(INPUT_BUFFER_9);				
			}		
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 8:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_DEFAULT__);				
				
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_8);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_9);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_10);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_11);
											
#ifdef	_USE_IME

				POINT pChatPoint;
				pChatPoint.x	= long(m_fPosX+58);
				pChatPoint.y	= long(m_fPosZ+75);
						
				switch(g_pInputManager->GetCurFocusInputID())
				{
				case INPUT_BUFFER_8:	
					g_pInputManager->ClearInput(INPUT_BUFFER_8);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_8, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					
					break;
				case INPUT_BUFFER_9:	
					g_pInputManager->ClearInput(INPUT_BUFFER_9);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_9, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;
				case INPUT_BUFFER_10:	
					g_pInputManager->ClearInput(INPUT_BUFFER_10);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_10, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;
				case INPUT_BUFFER_11:
					g_pInputManager->ClearInput(INPUT_BUFFER_11);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_11, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;					
				}
				GET_IMEEDIT()->SetEditIndex(1);				
				GET_IMEEDIT()->DeactivateIME();					
				GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_10));
				GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_RANK_REAL_NAME, GetStartOrder()+2);			
#endif
				g_pInputManager->SetFocusInput(INPUT_BUFFER_10);								
			}	
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 9:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_8);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_9);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_10);
				g_pInputManager->GetInputBuffer(INPUT_BUFFER_11);
				
#ifdef	_USE_IME

				POINT pChatPoint;
				pChatPoint.x	= long(m_fPosX+58);
				pChatPoint.y	= long(m_fPosZ+90);
						
				switch(g_pInputManager->GetCurFocusInputID())
				{
				case INPUT_BUFFER_8:	
					g_pInputManager->ClearInput(INPUT_BUFFER_8);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_8, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					
					break;
				case INPUT_BUFFER_9:	
					g_pInputManager->ClearInput(INPUT_BUFFER_9);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_9, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;
				case INPUT_BUFFER_10:	
					g_pInputManager->ClearInput(INPUT_BUFFER_10);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_10, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;
				case INPUT_BUFFER_11:
					g_pInputManager->ClearInput(INPUT_BUFFER_11);
					g_pInputManager->InsertCharacter(INPUT_BUFFER_11, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));					
					break;					
				}
				IMESetEdit(1);	
				GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_11));
				GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_RANK_REAL_NAME, GetStartOrder()+2);			
#endif
				g_pInputManager->SetFocusInput(INPUT_BUFFER_11);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDRANK_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDRANK_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
#ifdef	_USE_IME

		POINT pChatPoint = {0,};
		pChatPoint.x = long(m_fPosX+58);

		switch(g_pInputManager->GetCurFocusInputID())
		{
		case INPUT_BUFFER_8:																			
			pChatPoint.y = long(m_fPosZ+45);
			break;
		case INPUT_BUFFER_9:
			pChatPoint.y = long(m_fPosZ+60);
			break;
		case INPUT_BUFFER_10:
			pChatPoint.y = long(m_fPosZ+75);
			break;
		case INPUT_BUFFER_11:
			pChatPoint.y = long(m_fPosZ+90);
			break;
		}
		GET_IMEEDIT()->SetEditPos(pChatPoint.x, pChatPoint.y);

#endif

		int nOrder = GetStartOrder()+2;	

		RECT rt;

		rt.left = long(m_fPosX+58);		rt.right	= long(m_fPosX+120);
		rt.top	= long(m_fPosZ+45);		rt.bottom	= long(m_fPosZ+57);
		g_pInputManager->InitializeInput(INPUT_BUFFER_8, FONT_SS3D_GAME, rt, 0xffffffff, nOrder, FALSE, FALSE);
		rt.left = long(m_fPosX+58);		rt.right	= long(m_fPosX+120);
		rt.top	= long(m_fPosZ+60);		rt.bottom	= long(m_fPosZ+72);
		g_pInputManager->InitializeInput(INPUT_BUFFER_9, FONT_SS3D_GAME, rt, 0xffffffff, nOrder, FALSE, FALSE);
		rt.left = long(m_fPosX+58);		rt.right	= long(m_fPosX+120);
		rt.top	= long(m_fPosZ+75);		rt.bottom	= long(m_fPosZ+87);
		g_pInputManager->InitializeInput(INPUT_BUFFER_10, FONT_SS3D_GAME, rt, 0xffffffff, nOrder, FALSE, FALSE);
		rt.left = long(m_fPosX+58);		rt.right	= long(m_fPosX+120);
		rt.top	= long(m_fPosZ+90);		rt.bottom	= long(m_fPosZ+102);
		g_pInputManager->InitializeInput(INPUT_BUFFER_11, FONT_SS3D_GAME, rt, 0xffffffff, nOrder, FALSE, FALSE);

		return CInterface::GetInstance()->GetSetWnd();
	}
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}
			
void CGuildRankWnd::GuildMsg()
{
	char	szTemp[256] = {0,};
	char	szText[256] = {0,};
	char*	pToken=0;
	char	szSeps[]	= "',?";
	


	for(int i = 8; i < 12; i++)
	{
		int nRt = ConvString(g_pInputManager->GetInputBuffer(i));

		if(nRt==1)
		{					
			ErrMessage(g_Message[ETC_MESSAGE58].szMessage, 0xFFFF0000);
			g_pInputManager->ClearInput(INPUT_BUFFER_8);
			g_pInputManager->ClearInput(INPUT_BUFFER_9);
			g_pInputManager->ClearInput(INPUT_BUFFER_10);
			g_pInputManager->ClearInput(INPUT_BUFFER_11);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_11);
			return;
		}

		nRt = NoConvString(g_pInputManager->GetInputBuffer(i));

		if(nRt==1)
		{				
			ErrMessage(g_Message[ETC_MESSAGE59].szMessage, 0xFFFF0000);						
			g_pInputManager->ClearInput(INPUT_BUFFER_8);
			g_pInputManager->ClearInput(INPUT_BUFFER_9);
			g_pInputManager->ClearInput(INPUT_BUFFER_10);
			g_pInputManager->ClearInput(INPUT_BUFFER_11);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_11);												
			return;									
		}				
	}
	// 메세지 보내기 //
	CTWS_GUILD_RANK pPacket;
	for( int iter = 0 ; iter < 4 ; ++iter )
	{
		::memset(pPacket.szRankName[iter], 0, sizeof(pPacket.szRankName[iter]));
	}
	memset(pPacket.szGuildName, 0, sizeof(pPacket.szGuildName));				
	__lstrcpyn(pPacket.szGuildName, g_pGuildInfo.szGuildName, MAX_GUILD_NAME_REAL_LENGTH);

	for(i = 8; i < 12; i++)
	{
		__lstrcpyn(szText, g_pInputManager->GetInputBuffer(i), MAX_RANK_REAL_NAME);

		pToken = strtok(szText, szSeps);

		if(__strcmp(pToken, g_pInputManager->GetInputBuffer(i))!=0)
		{
			ErrMessage(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894);						
			g_pInputManager->ClearInput(i);
			return;
		}

		__lstrcpyn(szTemp, g_pInputManager->GetInputBuffer(i), MAX_RANK_REAL_NAME);

		pToken = strtok(szTemp, " ");

		if(pToken==NULL)	
		{
			switch(i)
			{
				case 8:	
				case 9:	
				case 10:
				case 11:	
					__lstrcpyn(pPacket.szRankName[i-8], g_Message[ETC_MESSAGE60+i-8].szMessage, MAX_RANK_REAL_NAME); // MSG_ID : 63 ; 일반회원 
					break;
					
			}
		}
		else
		{
			switch(i)
			{
				case 8:	
				case 9:	
				case 10:
				case 11:
					__lstrcpyn(pPacket.szRankName[i-8], g_pInputManager->GetInputBuffer(i), MAX_RANK_REAL_NAME); 
					break;
			}												
		}
	}
	for( int iter2 = 0 ; iter2 < 4 ; ++iter2 )
	{
		__lstrcpyn(g_pGuildInfo.szRankName[iter2], pPacket.szRankName[iter2], MAX_RANK_REAL_NAME);
	}
	g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	SetActive(FALSE);
}
//======================================================//
// End.													//
//======================================================//