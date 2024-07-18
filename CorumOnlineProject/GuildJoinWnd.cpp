//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "GuildJoinWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "CodeFun.h"


CGuildJoinWnd* CGuildJoinWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGuildJoinWnd::CGuildJoinWnd()
{ 	
	m_bMsgChk	= FALSE;
	m_bBtn[0]	= FALSE;
	m_bBtn[1]	= FALSE;
}

CGuildJoinWnd::~CGuildJoinWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGuildJoinWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_GUILDJOIN_CREATEWND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILDJOIN_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDJOIN_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDJOIN_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDJOIN_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDJOIN_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDJOIN_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDJOIN_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDJOIN_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildJoinWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildJoinWnd::SetOrder()
{
}

void CGuildJoinWnd::RenderText()
{
	if(m_bGuildType==__GCTYPE_GUILD__)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE299].szMessage);		// MSG_ID : 299 ; 길드 가입  
	else
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE300].szMessage);		// MSG_ID : 300 ; 클랜 가입 

	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+85, m_fPosZ+6, m_fPosZ+18, GetStartOrder()+1);

	if(m_bGuildType==__GCTYPE_GUILD__)
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE301].szMessage, m_szGuildName);	// MSG_ID : 301 ; %s 길드에 가입 하시겠습니까?
	else 
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE302].szMessage, m_szGuildName);	// MSG_ID : 302 ; %s 클랜에 가입 하시겠습니까?

	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+250, m_fPosZ+50, m_fPosZ+64, GetStartOrder()+1);	
}

void CGuildJoinWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();

		if(m_bMsgChk)
		{
			// 길드 가입 취소 //
			CTWS_GUILD_JOIN pGuildJoinPacket;
			memset(pGuildJoinPacket.szGuildName, 0, sizeof(pGuildJoinPacket.szGuildName));
			pGuildJoinPacket.bGuildType		= m_bGuildType;
			pGuildJoinPacket.bResult		= FALSE;
			pGuildJoinPacket.dwUserIndex	= m_dwUserIndex;
			__lstrcpyn(pGuildJoinPacket.szGuildName, m_szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
			__lstrcpyn(pGuildJoinPacket.szCharacterName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			g_pNet->SendMsg((char*)&pGuildJoinPacket, pGuildJoinPacket.GetPacketSize(), SERVER_INDEX_WORLD);

			m_bMsgChk = FALSE;
		}
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDJOIN_CREATEWND, TRUE);
		SetRender(SPR_OBJ_GUILDJOIN_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDJOIN_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDJOIN_CLOSE1, TRUE);

		m_bMsgChk = TRUE;
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildJoinWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDJOIN_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDJOIN_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDJOIN_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDJOIN_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDJOIN_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDJOIN_CANCEL3, FALSE);
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
				SetRender(SPR_OBJ_GUILDJOIN_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDJOIN_CLOSE2, TRUE);
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
				// 길드 가입 //			
				CTWS_GUILD_JOIN pGuildJoinPacket;
				memset(pGuildJoinPacket.szGuildName, 0, sizeof(pGuildJoinPacket.szGuildName));
				pGuildJoinPacket.dwUserIndex	= m_dwUserIndex;
				pGuildJoinPacket.bGuildType		= m_bGuildType;
				pGuildJoinPacket.dwLev			= g_pMainPlayer->m_dwLevel;
				pGuildJoinPacket.wClass			= g_pMainPlayer->m_wClass;
				pGuildJoinPacket.bResult		= TRUE;				
				__lstrcpyn(pGuildJoinPacket.szGuildName, m_szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
				__lstrcpyn(pGuildJoinPacket.szCharacterName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				g_pNet->SendMsg((char*)&pGuildJoinPacket, pGuildJoinPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				m_bMsgChk = FALSE;				
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDJOIN_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDJOIN_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDJOIN_OK3, TRUE);				
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_GUILDJOIN_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDJOIN_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDJOIN_OK3, FALSE);
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
				SetRender(SPR_OBJ_GUILDJOIN_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDJOIN_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDJOIN_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{								
				SetRender(SPR_OBJ_GUILDJOIN_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDJOIN_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDJOIN_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDJOIN_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDJOIN_CLOSE2, FALSE);		
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
