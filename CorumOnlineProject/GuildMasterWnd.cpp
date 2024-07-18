//======================================================//
// Code by Jang.							2004.01.14	//
//======================================================//
#include "GuildMasterWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"

CGuildMasterWnd* CGuildMasterWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGuildMasterWnd::CGuildMasterWnd()
{ 
	m_dwUserIndex	= 0;
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
}

CGuildMasterWnd::~CGuildMasterWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGuildMasterWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_GUILDMASTER_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILDMASTER_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDMASTER_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDMASTER_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDMASTER_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDMASTER_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDMASTER_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDMASTER_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDMASTER_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildMasterWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildMasterWnd::SetOrder()
{
}

void CGuildMasterWnd::RenderText()
{
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE202].szMessage);	// "경고"

	int nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+25+nSize*7, m_fPosZ+6, m_fPosZ+18, GetStartOrder()+1);
			
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE807].szMessage, m_szName); // "%s 님에게 마스터 권한을"
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE808].szMessage); // "이양 하시겠습니까?"
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+60, m_fPosZ+74, GetStartOrder()+1);	
}

void CGuildMasterWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		m_dwUserIndex = 0;
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDMASTER_WND, TRUE);
		SetRender(SPR_OBJ_GUILDMASTER_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDMASTER_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDMASTER_CANCEL1, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildMasterWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDMASTER_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDMASTER_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDMASTER_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDMASTER_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDMASTER_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDMASTER_CANCEL3, FALSE);
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
				SetRender(SPR_OBJ_GUILDMASTER_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDMASTER_CLOSE2, TRUE);
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
				CTWS_USER_GUILDMASTER pPacket;
				pPacket.dwGuildId	= g_pMainPlayer->m_dwGuildId;
				pPacket.dwUserIndex	= m_dwUserIndex;
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);				
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUILDMASTER_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDMASTER_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDMASTER_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{								
				SetRender(SPR_OBJ_GUILDMASTER_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDMASTER_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDMASTER_OK3, FALSE);				
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
				SetRender(SPR_OBJ_GUILDMASTER_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDMASTER_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDMASTER_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_GUILDMASTER_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDMASTER_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDMASTER_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDMASTER_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDMASTER_CLOSE2, FALSE);		
	}

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