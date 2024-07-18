//======================================================//
// Code by Yang.							2002.12.23	//
//======================================================//
#include "ItemTradeWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "User.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "CodeFun.h"

CItemTradeWnd* CItemTradeWnd::c_pThis = NULL;

//======================================================//
// Construction/Desertion.								//
//======================================================//
CItemTradeWnd::CItemTradeWnd()
{
	for(int i = 0; i < 2; i++)
		m_bBtnChk[i] = FALSE;
}

CItemTradeWnd::~CItemTradeWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CItemTradeWnd::Init()
{
	/*
	InsertData(SPR_OBJ_ITEMTRADE_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_ITEMTRADE_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMTRADE_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMTRADE_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADE_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADE_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADE_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADE_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADE_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CItemTradeWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CItemTradeWnd::SetOrder()
{
}

void CItemTradeWnd::RenderText()
{	
	RenderFont(g_Message[ETC_MESSAGE451].szMessage, m_fPosX+25, m_fPosX+55, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE581].szMessage, m_szName);	// "%s 님이 교환을 신청"
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

	RenderFont(g_Message[ETC_MESSAGE582].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+60, m_fPosZ+74, GetStartOrder()+1);
}

void CItemTradeWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive==FALSE)
	{
		m_bSoundChk	= FALSE;		
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_ITEMTRADE_WND, TRUE);
		SetRender(SPR_OBJ_ITEMTRADE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_ITEMTRADE_OK1, TRUE);
		SetRender(SPR_OBJ_ITEMTRADE_CANCEL1, TRUE);				

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CItemTradeWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtnChk[0]==TRUE && nRt!=4)
	{
		SetRender(SPR_OBJ_ITEMTRADE_OK1, TRUE);
		SetRender(SPR_OBJ_ITEMTRADE_OK2, FALSE);
		SetRender(SPR_OBJ_ITEMTRADE_OK3, FALSE);
		m_bBtnChk[0] = FALSE;
	}
	if(m_bBtnChk[1]==TRUE && nRt!=5)
	{
		SetRender(SPR_OBJ_ITEMTRADE_CANCEL1, TRUE);
		SetRender(SPR_OBJ_ITEMTRADE_CANCEL2, FALSE);
		SetRender(SPR_OBJ_ITEMTRADE_CANCEL3, FALSE);
		m_bBtnChk[1] = FALSE;
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
			{
				// 아이템 트래이드 거절 //
				CTDS_TRADE_RESULT pPacket;
				memset(pPacket.szCharacterName, 0, sizeof(pPacket.szCharacterName));
				
				__lstrcpyn(pPacket.szCharacterName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);

				pPacket.byCode		= 1;
				pPacket.dwUserIndex	= m_dwUserIndex;
				
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_ITEMTRADE_CLOSE1, FALSE);
				SetRender(SPR_OBJ_ITEMTRADE_CLOSE2, TRUE);
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
				m_bBtnChk[0] = TRUE;

				// 아이템 트래이드  //
				CTDS_TRADE_RESULT pPacket;
				memset(pPacket.szCharacterName, 0, sizeof(pPacket.szCharacterName));
				__lstrcpyn(pPacket.szCharacterName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);

				pPacket.byCode		= 0;
				pPacket.dwUserIndex	= m_dwUserIndex;				

				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);				

				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTONCLICK__);
				SetRender(SPR_OBJ_ITEMTRADE_OK1, FALSE);
				SetRender(SPR_OBJ_ITEMTRADE_OK2, FALSE);
				SetRender(SPR_OBJ_ITEMTRADE_OK3, TRUE);				
				m_bBtnChk[0] = TRUE;				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_ITEMTRADE_OK1, FALSE);
				SetRender(SPR_OBJ_ITEMTRADE_OK2, TRUE);
				SetRender(SPR_OBJ_ITEMTRADE_OK3, FALSE);				
				m_bBtnChk[0] = TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{				
				m_bBtnChk[1] = TRUE;
			
				// 아이템 트래이드 거절 //	
				CTDS_TRADE_RESULT pPacket;
				memset(pPacket.szCharacterName, 0, sizeof(pPacket.szCharacterName));
				__lstrcpyn(pPacket.szCharacterName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);

				pPacket.byCode		= 1;
				pPacket.dwUserIndex	= m_dwUserIndex;				

				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_ITEMTRADE_CANCEL1, FALSE);
				SetRender(SPR_OBJ_ITEMTRADE_CANCEL2, FALSE);
				SetRender(SPR_OBJ_ITEMTRADE_CANCEL3, TRUE);					
				m_bBtnChk[1]	= TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_ITEMTRADE_CANCEL1, FALSE);
				SetRender(SPR_OBJ_ITEMTRADE_CANCEL2, TRUE);
				SetRender(SPR_OBJ_ITEMTRADE_CANCEL3, FALSE);
				m_bBtnChk[1] = TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}	
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_ITEMTRADE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_ITEMTRADE_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
		return CInterface::GetInstance()->GetSetWnd();
	}

	if(IsReturn(nRt, eMouseCheck))
	{
		return CInterface::GetInstance()->GetSetWnd();		
	}

	return 0;
}
//======================================================//
// End.													//
//======================================================//