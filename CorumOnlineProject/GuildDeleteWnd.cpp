//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "GuildDeleteWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "CodeFun.h"
#include "NetworkClient.h"
#include "InitGame.h"

CGuildDeleteWnd* CGuildDeleteWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGuildDeleteWnd::CGuildDeleteWnd()
{ 
	
	m_dwUserIndex	= 0;	
	m_bChk			= FALSE;
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
}

CGuildDeleteWnd::~CGuildDeleteWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGuildDeleteWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_GUILDDELETE_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILDDELETE_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDDELETE_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDDELETE_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDDELETE_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDDELETE_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDDELETE_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDDELETE_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDDELETE_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildDeleteWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildDeleteWnd::SetOrder()
{
}

void CGuildDeleteWnd::RenderText()
{	
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE202].szMessage);	// "경고"

	int nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+25+nSize*7, m_fPosZ+6, m_fPosZ+18, GetStartOrder()+1);
			
	if(m_byType==1 || m_byType==2)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE203].szMessage); // "탈퇴(해체) 하시겠습니까?"
	else if(m_byType==3)
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE204].szMessage, m_szName); // "%s 님을 추방하시겠습니까?"
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+52, GetStartOrder()+1);
}

void CGuildDeleteWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		if(m_bChk)
		{			
			m_bChk	= FALSE;
		}
		m_dwUserIndex = 0;
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDDELETE_WND, TRUE);
		SetRender(SPR_OBJ_GUILDDELETE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDDELETE_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDDELETE_CANCEL1, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildDeleteWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDDELETE_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDDELETE_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDDELETE_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDDELETE_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDDELETE_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDDELETE_CANCEL3, FALSE);
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
				SetRender(SPR_OBJ_GUILDDELETE_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDDELETE_CLOSE2, TRUE);
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
				if(m_byType==1)
				{
					CTWS_GUILD_DELETE pPacket;
					memset(pPacket.szGuildName, 0, sizeof(pPacket.szGuildName));

					pPacket.dwGuildId = g_pMainPlayer->m_dwGuildId;
					__lstrcpyn(pPacket.szGuildName, g_pMainPlayer->m_szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				else if(m_byType==2)
				{
					CTWS_GUILD_USERDEL pPacket;
					memset(pPacket.szCharacterName, 0, sizeof(pPacket.szCharacterName));
					__lstrcpyn(pPacket.szCharacterName, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					pPacket.byUserType	= 1;
					pPacket.dwGuildId	= g_pMainPlayer->m_dwGuildId;	
					pPacket.dwUserIndex	= m_dwUserIndex;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				else if(m_byType==3)
				{
					CTWS_GUILD_USERDEL pPacket;
					memset(pPacket.szCharacterName, 0, sizeof(pPacket.szCharacterName));
					__lstrcpyn(pPacket.szCharacterName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					pPacket.byUserType	= 1;
					pPacket.dwGuildId	= g_pMainPlayer->m_dwGuildId;	
					pPacket.dwUserIndex	= m_dwUserIndex;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				m_bBtn[0] = TRUE;
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUILDDELETE_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDDELETE_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDDELETE_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}			
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_GUILDDELETE_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDDELETE_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDDELETE_OK3, FALSE);
				m_bBtn[0] = TRUE;
			}		
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetActive(FALSE);
				m_bBtn[1] = TRUE;
			}			
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDDELETE_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDDELETE_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDDELETE_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_GUILDDELETE_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDDELETE_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDDELETE_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}			
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDDELETE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDDELETE_CLOSE2, FALSE);		
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