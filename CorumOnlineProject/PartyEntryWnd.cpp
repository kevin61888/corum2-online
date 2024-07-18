//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "PartyEntryWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "InputManager.h"
#include "InitGame.h"
#include "DungeonProcess.h"
#include "ChatWnd.h"
#include "ChatBackground.h"
#include "CodeFun.h"

CPartyEntryWnd* CPartyEntryWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CPartyEntryWnd::CPartyEntryWnd()
{ 		
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
	m_byEntryType	= PARTYENTRY_INPUT;
}

CPartyEntryWnd::~CPartyEntryWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CPartyEntryWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_PARTYENTRY_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_PARTYENTRY_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 6.0f, 0);
	InsertData(SPR_OBJ_PARTYENTRY_WND3, SPR_INTERFACE_DF_WND3, 0, 128, 1.0f, 1.0f, 0);	
	InsertData(SPR_OBJ_PARTYENTRY_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYENTRY_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYENTRY_OK1, SPR_INTERFACE_OK1, 121, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYENTRY_OK2, SPR_INTERFACE_OK2, 121, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYENTRY_OK3, SPR_INTERFACE_OK3, 121, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYENTRY_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYENTRY_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYENTRY_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 115, 1.0f, 1.0f, 2);	
				
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(96, 115, 160, 131, 4, CHECK_BTN);
	InsertCheckInterface(176, 115, 240, 131, 5, CHECK_BTN);
	InsertCheckInterface(20, 60, 256, 74, 6, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CPartyEntryWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CPartyEntryWnd::SetOrder()
{
	RECT rt;
	rt.left		= long(m_fPosX+20);
	rt.right	= long(m_fPosX+256);
	rt.top		= long(m_fPosZ+60);
	rt.bottom	= long(m_fPosZ+74);				
	g_pInputManager->InitializeInput(INPUT_BUFFER_PARTY_ENTRY, FONT_SS3D_GAME, rt, 0xffffff00, GetStartOrder()+3, FALSE, FALSE);
}

void CPartyEntryWnd::RenderText()
{	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1149].szMessage); // "파티등록"
	
	int nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+85, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

	if(m_byEntryType==PARTYENTRY_INPUT)
	{
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE1150].szMessage); // "파티 설명을 쓰세요. (16자 이하)"
		
		nSize = lstrlen(m_szInfo);
		RenderFont(m_szInfo, m_fPosX+20, m_fPosX+255, m_fPosZ+37, m_fPosZ+51, GetStartOrder()+1);
	
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE1151].szMessage); // "파티를 NPC에 등록 하시겠습니까?"
		
		nSize = lstrlen(m_szInfo);
		RenderFont(m_szInfo, m_fPosX+20, m_fPosX+255, m_fPosZ+87, m_fPosZ+101, GetStartOrder()+1);
	}
	else if(m_byEntryType==PARTYENTRY_RESULT)
	{
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE1152].szMessage); // "리스트에 등록 되었습니다."
	
		nSize = lstrlen(m_szInfo);
		RenderFont(m_szInfo, m_fPosX+20, m_fPosX+255, m_fPosZ+37, m_fPosZ+51, GetStartOrder()+1);		
	}
}

void CPartyEntryWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{	
		m_bSoundChk	= FALSE;
		ShowSpriteAll();

		if(m_byEntryType==PARTYENTRY_INPUT)
		{
			IMESetEdit(0);
			ChatModeOff(INPUT_BUFFER_PARTY_ENTRY);		
		}
			
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_PARTYENTRY_WND1, TRUE);
		SetRender(SPR_OBJ_PARTYENTRY_WND2, TRUE);
		SetRender(SPR_OBJ_PARTYENTRY_WND3, TRUE);
		SetRender(SPR_OBJ_PARTYENTRY_CLOSE1, TRUE);

		if(m_byEntryType==PARTYENTRY_INPUT)
		{
			SetRender(SPR_OBJ_PARTYENTRY_OK1, TRUE);
			SetRender(SPR_OBJ_PARTYENTRY_CANCEL1, TRUE);

			if(!g_pGVDungeon->bChatMode)
			{
#ifdef _USE_IME
				POINT pChatPoint;
				pChatPoint.x	= long(m_fPosX+20);
				pChatPoint.y	= long(m_fPosZ+60);
				GET_IMEEDIT()->SetEditIndex(1);
				GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_PARTYENTRY_TEXT);
				ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
#endif
				RECT rt;
				rt.left		= long(m_fPosX+20);
				rt.right	= long(m_fPosX+256);
				rt.top		= long(m_fPosZ+60);
				rt.bottom	= long(m_fPosZ+74);				
				g_pInputManager->InitializeInput(INPUT_BUFFER_PARTY_ENTRY, FONT_SS3D_GAME, rt, 0xffffff00, GetStartOrder()+3);				
				ChatModeOn(INPUT_BUFFER_PARTY_ENTRY, FALSE);
			}
			else
			{				
#ifdef _USE_IME
				POINT pChatPoint;
				pChatPoint.x	= long(m_fPosX+20);
				pChatPoint.y	= long(m_fPosZ+60);
				GET_IMEEDIT()->SetEditIndex(1);
				GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_PARTYENTRY_TEXT);
				ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
#endif
				RECT rt;
				rt.left		= long(m_fPosX+20);
				rt.right	= long(m_fPosX+256);
				rt.top		= long(m_fPosZ+60);
				rt.bottom	= long(m_fPosZ+74);				

				g_pInputManager->ClearInput(g_pInputManager->GetCurFocusInputID());
				g_pInputManager->InitializeInput(INPUT_BUFFER_PARTY_ENTRY, FONT_SS3D_GAME, rt, 0xffffff00, GetStartOrder()+3);				
				
				ChatModeOn(INPUT_BUFFER_PARTY_ENTRY, FALSE);
				
				g_nChatModeExt	= 1;
				
				CChatWnd* pChatWnd = CChatWnd::GetInstance();

				if(pChatWnd->GetActive())
					pChatWnd->SetActive(FALSE);				
			}
			SetPosObj(SPR_OBJ_PARTYENTRY_OK1, 121, 115);
			SetPosObj(SPR_OBJ_PARTYENTRY_OK2, 121, 115);
			SetPosObj(SPR_OBJ_PARTYENTRY_OK3, 121, 115);
			SetPosObj(SPR_OBJ_PARTYENTRY_CANCEL1, 189, 115);
			SetPosObj(SPR_OBJ_PARTYENTRY_CANCEL2, 189, 115);
			SetPosObj(SPR_OBJ_PARTYENTRY_CANCEL3, 189, 115);
		}
		else if(m_byEntryType==PARTYENTRY_RESULT)
		{
			SetRender(SPR_OBJ_PARTYENTRY_OK1, TRUE);

			SetPosObj(SPR_OBJ_PARTYENTRY_OK1, 189, 115);
			SetPosObj(SPR_OBJ_PARTYENTRY_OK2, 189, 115);
			SetPosObj(SPR_OBJ_PARTYENTRY_OK3, 189, 115);			
		}
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CPartyEntryWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4 && m_byEntryType==PARTYENTRY_INPUT)
	{
		SetRender(SPR_OBJ_PARTYENTRY_OK1, TRUE);
		SetRender(SPR_OBJ_PARTYENTRY_OK2, FALSE);
		SetRender(SPR_OBJ_PARTYENTRY_OK3, FALSE);
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5 && m_byEntryType==PARTYENTRY_INPUT)
	{
		SetRender(SPR_OBJ_PARTYENTRY_CANCEL1, TRUE);
		SetRender(SPR_OBJ_PARTYENTRY_CANCEL2, FALSE);
		SetRender(SPR_OBJ_PARTYENTRY_CANCEL3, FALSE);
		m_bBtn[1] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5 && m_byEntryType==PARTYENTRY_RESULT)
	{
		SetRender(SPR_OBJ_PARTYENTRY_OK1, TRUE);
		SetRender(SPR_OBJ_PARTYENTRY_OK2, FALSE);
		SetRender(SPR_OBJ_PARTYENTRY_OK3, FALSE);
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
				SetRender(SPR_OBJ_PARTYENTRY_CLOSE1, FALSE);
				SetRender(SPR_OBJ_PARTYENTRY_CLOSE2, TRUE);
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
				if(m_byEntryType==PARTYENTRY_INPUT)
				{
					if(g_pMainPlayer->m_bPartyLeader==1)
					{
						CTWS_PARTY_ENTRY pPacket;
						pPacket.dwPartyId	= g_pMainPlayer->m_dwPartyId;
						pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
						memset(pPacket.szPartyEntryMemo, 0, sizeof(pPacket.szPartyEntryMemo));
						__lstrcpyn(pPacket.szPartyEntryMemo, g_pInputManager->GetInputBuffer(INPUT_BUFFER_PARTY_ENTRY), MAX_PARTYENTRY_TEXT);
						g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
						SetActive(FALSE);
					}
					else
						DisplayMessageAdd(g_Message[ETC_MESSAGE1141].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "파티의 리더만이 등록할수 있습니다."					
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byEntryType==PARTYENTRY_INPUT)
				{
					SetRender(SPR_OBJ_PARTYENTRY_OK1, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_OK2, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_OK3, TRUE);
					m_bBtn[0] = TRUE;
				}				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byEntryType==PARTYENTRY_INPUT)
				{
					SetRender(SPR_OBJ_PARTYENTRY_OK1, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_OK2, TRUE);
					SetRender(SPR_OBJ_PARTYENTRY_OK3, FALSE);
					m_bBtn[0] = TRUE;
				}				
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byEntryType==PARTYENTRY_INPUT)
					SetActive(FALSE);
				else if(m_byEntryType==PARTYENTRY_RESULT)
					SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{	
				if(m_byEntryType==PARTYENTRY_INPUT)
				{
					SetRender(SPR_OBJ_PARTYENTRY_CANCEL1, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_CANCEL2, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_CANCEL3, TRUE);
					m_bBtn[1] = TRUE;
				}				
				else if(m_byEntryType==PARTYENTRY_RESULT)
				{
					SetRender(SPR_OBJ_PARTYENTRY_OK1, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_OK2, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_OK3, TRUE);
					m_bBtn[1] = TRUE;
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				if(m_byEntryType==PARTYENTRY_INPUT)
				{
					SetRender(SPR_OBJ_PARTYENTRY_CANCEL1, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_CANCEL2, TRUE);
					SetRender(SPR_OBJ_PARTYENTRY_CANCEL3, FALSE);
					m_bBtn[1] = TRUE;
				}				
				else if(m_byEntryType==PARTYENTRY_RESULT)				
				{
					SetRender(SPR_OBJ_PARTYENTRY_OK1, FALSE);
					SetRender(SPR_OBJ_PARTYENTRY_OK2, TRUE);
					SetRender(SPR_OBJ_PARTYENTRY_OK3, FALSE);
					m_bBtn[1] = TRUE;
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byEntryType==PARTYENTRY_INPUT)
				{
#ifdef _USE_IME
					POINT pChatPoint;
					pChatPoint.x	= long(m_fPosX+20);
					pChatPoint.y	= long(m_fPosZ+60);
					GET_IMEEDIT()->SetEditIndex(1);
					GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_PARTYENTRY_TEXT);
					ShowChatEditBackground(TRUE, GET_IMEEDIT()->GetOrder()-1);
#endif
					RECT rt;
					rt.left		= long(m_fPosX+20);
					rt.right	= long(m_fPosX+256);
					rt.top		= long(m_fPosZ+60);
					rt.bottom	= long(m_fPosZ+74);
					g_pInputManager->InitializeInput(INPUT_BUFFER_PARTY_ENTRY, FONT_SS3D_GAME, rt, 0xffffff00, GetStartOrder()+3);					
					
					ChatModeOn(INPUT_BUFFER_2, FALSE);
				}
			}			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
					
	if(MouseUp())
	{
		SetRender(SPR_OBJ_PARTYENTRY_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYENTRY_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
		RECT rt;
		rt.left		= long(m_fPosX+20);
		rt.right	= long(m_fPosX+256);
		rt.top		= long(m_fPosZ+60);
		rt.bottom	= long(m_fPosZ+74);				
		g_pInputManager->InitializeInput(INPUT_BUFFER_PARTY_ENTRY, FONT_SS3D_GAME, rt, 0xffffff00, GetStartOrder()+2, FALSE, FALSE);

		return CInterface::GetInstance()->GetSetWnd();
	}

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}
//======================================================//
// End.													//
//======================================================//
