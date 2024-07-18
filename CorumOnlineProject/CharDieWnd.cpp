//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "CharDieWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "CodeFun.h"


CCharDieWnd* CCharDieWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CCharDieWnd::CCharDieWnd()
{
}

CCharDieWnd::~CCharDieWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CCharDieWnd::Init()
{	
/*
	InsertData(SPR_OBJ_CHARDIE_WND, SPR_INTERFACE_OK2_WND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHARDIE_OK1, SPR_INTERFACE_OK1, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHARDIE_OK2, SPR_INTERFACE_OK2, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHARDIE_OK3, SPR_INTERFACE_OK3, 189, 110, 1.0f, 1.0f, 1);
	
	InsertCheckInterface(0, 0, 242, 20, 2, CHECK_MOVEWND);
	InsertCheckInterface(189, 110, 253, 126, 3, CHECK_BTN);
	*/
	m_bInit	= TRUE;	

	return TRUE;
}

void CCharDieWnd::Remove()
{	
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CCharDieWnd::SetOrder()
{
}

void CCharDieWnd::RenderText()
{
	// 다시 시작합니다.
	RenderFont(g_Message[ETC_MESSAGE809].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);
}

void CCharDieWnd::SetActive(BOOL bActive)
{		
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		m_bSoundChk = FALSE;
		ShowSpriteAll();		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_CHARDIE_WND, TRUE);
		SetRender(SPR_OBJ_CHARDIE_OK1, TRUE);
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CCharDieWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtnChk==TRUE)
	{
		SetRender(SPR_OBJ_CHARDIE_OK1, TRUE);
		SetRender(SPR_OBJ_CHARDIE_OK2, FALSE);
		SetRender(SPR_OBJ_CHARDIE_OK3, FALSE);
		m_bBtnChk = FALSE;
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
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				CTDS_USER_DIE_OK packet;
				g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_CHARDIE_OK1, FALSE);
				SetRender(SPR_OBJ_CHARDIE_OK2, FALSE);
				SetRender(SPR_OBJ_CHARDIE_OK3, TRUE);
				m_bBtnChk = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_CHARDIE_OK1, FALSE);
				SetRender(SPR_OBJ_CHARDIE_OK2, TRUE);
				SetRender(SPR_OBJ_CHARDIE_OK3, FALSE);
				m_bBtnChk = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}	
		
	MouseUp();
	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}
//======================================================//
// End.													//
//======================================================//