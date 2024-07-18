//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "GuildRequestWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "user.h"
#include "InitGame.h"
#include "CodeFun.h"

CGuildRequestWnd* CGuildRequestWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGuildRequestWnd::CGuildRequestWnd()
{ 
	m_dwUserIndex			= 0;	
	m_dwRequestUserIndex	= 0;
	m_bChk					= FALSE;
	m_bBtn[0]				= FALSE;
	m_bBtn[1]				= FALSE;
}

CGuildRequestWnd::~CGuildRequestWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGuildRequestWnd::Init()
{	
/*
	InsertData(SPR_OBJ_GUILDREQUEST_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILDREQUEST_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDREQUEST_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDREQUEST_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDREQUEST_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDREQUEST_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDREQUEST_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDREQUEST_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDREQUEST_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);	
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildRequestWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildRequestWnd::SetOrder()
{
}

void CGuildRequestWnd::RenderText()
{
	if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE207].szMessage);							// "클랜요청"
	else
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE208].szMessage);							// "길드요청"	

	int nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+25+nSize*7, m_fPosZ+6, m_fPosZ+18, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE739].szMessage, m_szRequestName, m_szName);	// "%s 님이 %s 님을"
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+52, GetStartOrder()+1);

	if(g_pMainPlayer->m_byType==__GCTYPE_CLEN__)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE209].szMessage);							// "클랜으로 초대하길 희망합니다."
	else
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE210].szMessage);							// "길드으로 초대하길 희망합니다."
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+56, m_fPosZ+70, GetStartOrder()+1);
	
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE156].szMessage);								// "동의하시겠습니까?"	
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+255, m_fPosZ+74, m_fPosZ+88, GetStartOrder()+1);	
}

void CGuildRequestWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		if(m_bChk)
		{
			CTWS_GUILD_USERREQUEST_FAIL	pPacketSend;
			pPacketSend.byType		= 1;
			pPacketSend.dwUserIndex	= m_dwRequestUserIndex;			
			g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);
			m_bChk	= FALSE;
		}
		m_dwUserIndex = 0;
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDREQUEST_WND, TRUE);
		SetRender(SPR_OBJ_GUILDREQUEST_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDREQUEST_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDREQUEST_CANCEL1, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildRequestWnd::CheckInterface()
{
m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDREQUEST_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDREQUEST_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDREQUEST_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDREQUEST_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDREQUEST_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDREQUEST_CANCEL3, FALSE);
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
			{
				CTWS_GUILD_USERREQUEST_FAIL	pPacketSend;
				pPacketSend.byType		= 1;
				pPacketSend.dwUserIndex	= m_dwRequestUserIndex;			
				g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);	
				m_bChk		= FALSE;
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUILDREQUEST_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDREQUEST_CLOSE2, TRUE);
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
				CTWS_GUILDJOIN_MESSAGE	pGuildJoin;
				memset(pGuildJoin.szGuildName, 0, sizeof(pGuildJoin.szGuildName));
				memset(pGuildJoin.szCharacterName, 0, sizeof(pGuildJoin.szCharacterName));
				pGuildJoin.dwUserIndex	= m_dwUserIndex;
				pGuildJoin.bType		= g_pMainPlayer->m_byType;
				__lstrcpyn(pGuildJoin.szGuildName, g_pMainPlayer->m_szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
				__lstrcpyn(pGuildJoin.szCharacterName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				g_pNet->SendMsg((char*)&pGuildJoin, pGuildJoin.GetPacketSize(), SERVER_INDEX_WORLD);
		
				char szInfo[0xff] = {0,};

				if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)
					wsprintf(szInfo, g_Message[ETC_MESSAGE388].szMessage, m_szName);	// MSG_ID : 388 ; %s 님에게 길드 가입 메세지를 보냈습니다.
				else
					wsprintf(szInfo, g_Message[ETC_MESSAGE389].szMessage, m_szName);	// MSG_ID : 389 ; %s 님에게 클랜 가입 메세지를 보냈습니다.

				DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);								
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDREQUEST_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDREQUEST_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDREQUEST_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_GUILDREQUEST_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDREQUEST_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDREQUEST_OK3, FALSE);
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				CTWS_GUILD_USERREQUEST_FAIL	pPacketSend;
				pPacketSend.byType		= 1;
				pPacketSend.dwUserIndex	= m_dwRequestUserIndex;			
				g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);
				m_bChk	= FALSE;								
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDREQUEST_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDREQUEST_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDREQUEST_CANCEL3, TRUE);	
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_GUILDREQUEST_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDREQUEST_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDREQUEST_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDREQUEST_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDREQUEST_CLOSE2, FALSE);		
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