#include "ItemMakingHelpWnd.h"
#include "GameControl.h"
#include "User.h"
#include "QuantityWnd.h"
#include "dungeon_data_ex.h"
#include "hashtable.h"
#include "../CommonServer/CommonHeader.h"
#include "Chat.h"
#include "UserInterface.h"
#include "Message.h"
#include "DungeonTable.h"
#include "Interface.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "GlobalSoundHandles.h"
#include "CodeFun.h"


CItemMakingHelpWnd* CItemMakingHelpWnd::c_pThis = NULL;

CItemMakingHelpWnd::CItemMakingHelpWnd()
{

}


CItemMakingHelpWnd::~CItemMakingHelpWnd()
{

}


BOOL CItemMakingHelpWnd::Init()
{
	/*
	InsertData(SPR_OBJ_NOTICE_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_NOTICE_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_NOTICE_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_NOTICE_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NOTICE_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_HELP_NEXT1, SPR_INTERFACE_HELP_NEXT1, 84, 216, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_NEXT2, SPR_INTERFACE_HELP_NEXT2, 84, 216, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_HELP_NEXT3, SPR_INTERFACE_HELP_NEXT3, 84, 216, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(84, 216, 174, 233, 4, CHECK_BTN);
	*/
	m_bInit	= TRUE;
	
	memset(m_szHelpMessage, 0, 1024);
	
	return TRUE;
}


void CItemMakingHelpWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}


void CItemMakingHelpWnd::RenderText()
{	
	char	szInfo[0xff] = {0,};
	
	lstrcpy(szInfo, g_Message[ETC_MESSAGE314].szMessage);	// "µµ¿ò¸»"

	int nSize = lstrlen(szInfo);
	RenderFont(szInfo, m_fPosX+22, m_fPosX+22+nSize*6, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);

	RenderTextEx();
}

void CItemMakingHelpWnd::RenderTextEx()
{	
	if(m_bypage==0)
	{
		for(BYTE i = 160; i <= 170; i++)
		{
			InterfaceTextRender(i);
		}
	}
	else if(m_bypage == 1)
	{
		for(BYTE i = 171; i <= 171; i++)
		{
			InterfaceTextRender(i);
		}
	}	
}

void CItemMakingHelpWnd::SetOrder()
{
	
}


void CItemMakingHelpWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;
	
	if(bActive==FALSE)
	{		
		m_bSoundChk	= FALSE;	
		ShowSpriteAll();

		m_bypage = 0;
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{		
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		m_bypage = 0;
		
		SetRender(SPR_OBJ_NOTICE_WND1, TRUE);
		SetRender(SPR_OBJ_NOTICE_WND2, TRUE);
		SetRender(SPR_OBJ_NOTICE_WND3, TRUE);
		SetRender(SPR_OBJ_NOTICE_CLOSE1, TRUE);	
		SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);		
	}
}


int CItemMakingHelpWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;
	
	int nRt = GetChk();

	if(m_bBtn && nRt != 4)
	{
		SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
		SetRender(SPR_OBJ_HELP_NEXT2, FALSE);
		SetRender(SPR_OBJ_HELP_NEXT3, FALSE);
		m_bBtn = FALSE;
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
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_NOTICE_CLOSE1, FALSE);
				SetRender(SPR_OBJ_NOTICE_CLOSE2, TRUE);
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
				SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
				SetRender(SPR_OBJ_HELP_NEXT2, FALSE);
				SetRender(SPR_OBJ_HELP_NEXT3, FALSE);
				m_bypage++;
				if(m_bypage > 1)
					m_bypage = 0;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_HELP_NEXT1, FALSE);
				SetRender(SPR_OBJ_HELP_NEXT2, TRUE);
				SetRender(SPR_OBJ_HELP_NEXT3, FALSE);
				m_bBtn = TRUE;
			}
			else if(eMouseCheck == MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_HELP_NEXT1, FALSE);
				SetRender(SPR_OBJ_HELP_NEXT2, FALSE);
				SetRender(SPR_OBJ_HELP_NEXT3, TRUE);
				m_bBtn = TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_NOTICE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_NOTICE_CLOSE2, FALSE);		
	}
	
	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		
	
	return 0;
}


void CItemMakingHelpWnd::SetButtonStatus(DWORD dwSprID, DWORD dwStatus)
{
	switch(dwStatus)
	{
	case 0://nomal
		{
			SetRender(dwSprID, TRUE);
			SetRender(dwSprID+1, FALSE);
			SetRender(dwSprID+2, FALSE);
		}break;
	case 1://mouseover
		{
			SetRender(dwSprID, FALSE);
			SetRender(dwSprID+1, TRUE);
			SetRender(dwSprID+2, FALSE);
		}break;
	case 2://mousedown
		{
			SetRender(dwSprID, FALSE);
			SetRender(dwSprID+1, FALSE);
			SetRender(dwSprID+2, TRUE);
		}break;
	}
}



