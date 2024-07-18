//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "PartyRequestWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"

CPartyRequestWnd* CPartyRequestWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CPartyRequestWnd::CPartyRequestWnd()
{ 
	m_dwUserIndex			= 0;	
	m_dwRequestUserIndex	= 0;
	m_bChk					= FALSE;
	m_bBtn[0]				= FALSE;
	m_bBtn[1]				= FALSE;
}

CPartyRequestWnd::~CPartyRequestWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CPartyRequestWnd::Init()
{	
/*
	InsertData(SPR_OBJ_PARTYREQUEST_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_PARTYREQUEST_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYREQUEST_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYREQUEST_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYREQUEST_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYREQUEST_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYREQUEST_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYREQUEST_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYREQUEST_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CPartyRequestWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CPartyRequestWnd::SetOrder()
{
}

void CPartyRequestWnd::RenderText()
{
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE725].szMessage); // "파티요청"

	int nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+25+nSize*7, m_fPosZ+6, m_fPosZ+18, GetStartOrder()+1);
				
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE739].szMessage, m_szRequestName, m_szName); // "%s 님이 %s 님을"
		
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+52, GetStartOrder()+1);

	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE740].szMessage);	// "파티로 초대하길 희망합니다."
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+56, m_fPosZ+70, GetStartOrder()+1);
	
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE156].szMessage); // "동의하시겠습니까?"	
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+255, m_fPosZ+74, m_fPosZ+88, GetStartOrder()+1);
}

void CPartyRequestWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		if(m_bChk)
		{
			CTWS_PARTY_USERREQUEST_FAIL	pPacketSend;
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

		SetRender(SPR_OBJ_PARTYREQUEST_WND, TRUE);
		SetRender(SPR_OBJ_PARTYREQUEST_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYREQUEST_OK1, TRUE);
		SetRender(SPR_OBJ_PARTYREQUEST_CANCEL1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CPartyRequestWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_PARTYREQUEST_OK1, TRUE);
		SetRender(SPR_OBJ_PARTYREQUEST_OK2, FALSE);
		SetRender(SPR_OBJ_PARTYREQUEST_OK3, FALSE);
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_PARTYREQUEST_CANCEL1, TRUE);
		SetRender(SPR_OBJ_PARTYREQUEST_CANCEL1, FALSE);
		SetRender(SPR_OBJ_PARTYREQUEST_CANCEL1, FALSE);
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
				CTWS_PARTY_USERREQUEST_FAIL	pPacketSend;
				pPacketSend.byType		= 1;
				pPacketSend.dwUserIndex	= m_dwRequestUserIndex;			
				g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);						
				m_bChk = FALSE;
				SetActive(FALSE);	
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_PARTYREQUEST_CLOSE1, FALSE);
				SetRender(SPR_OBJ_PARTYREQUEST_CLOSE2, TRUE);
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
				CTWS_PARTY_JOIN pPacket;
				memset(pPacket.szName, 0, sizeof(pPacket.szName));
				__lstrcpyn(pPacket.szName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);			
				pPacket.bErrCode	= 0;
				pPacket.dwUserIndex	= m_dwUserIndex;
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				
				char szInfo[0xff] = {0,};
				wsprintf(szInfo, g_Message[ETC_MESSAGE387].szMessage, m_szName);	// %s 님에게 파티 메세지 보냈습니다.
				DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
				m_bChk = FALSE;
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_PARTYREQUEST_OK1, FALSE);
				SetRender(SPR_OBJ_PARTYREQUEST_OK2, FALSE);
				SetRender(SPR_OBJ_PARTYREQUEST_OK3, TRUE);				
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{								
				SetRender(SPR_OBJ_PARTYREQUEST_OK1, FALSE);
				SetRender(SPR_OBJ_PARTYREQUEST_OK2, TRUE);
				SetRender(SPR_OBJ_PARTYREQUEST_OK3, FALSE);
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				CTWS_PARTY_USERREQUEST_FAIL	pPacketSend;
				pPacketSend.byType		= 1;
				pPacketSend.dwUserIndex	= m_dwRequestUserIndex;			
				g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);
				m_bChk = FALSE;								
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{						
				SetRender(SPR_OBJ_PARTYREQUEST_CANCEL1, FALSE);
				SetRender(SPR_OBJ_PARTYREQUEST_CANCEL2, FALSE);
				SetRender(SPR_OBJ_PARTYREQUEST_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(!g_Mouse.bLDown && !m_bUpChk)
			{
				SetRender(SPR_OBJ_PARTYREQUEST_CANCEL1, FALSE);
				SetRender(SPR_OBJ_PARTYREQUEST_CANCEL2, TRUE);
				SetRender(SPR_OBJ_PARTYREQUEST_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_PARTYREQUEST_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYREQUEST_CLOSE2, FALSE);		
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
