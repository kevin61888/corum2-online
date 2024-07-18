//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "PartyJoinWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"



CPartyJoinWnd* CPartyJoinWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CPartyJoinWnd::CPartyJoinWnd()
{ 
	m_dwUserIndex	= 0;		
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
}

CPartyJoinWnd::~CPartyJoinWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CPartyJoinWnd::Init()
{	
/*
	InsertData(SPR_OBJ_PARTYJOIN_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_PARTYJOIN_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYJOIN_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYJOIN_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYJOIN_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYJOIN_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYJOIN_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYJOIN_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYJOIN_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CPartyJoinWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CPartyJoinWnd::SetOrder()
{
}

void CPartyJoinWnd::RenderText()
{
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE357].szMessage);		// MSG_ID : 357 ; 파티 가입
	
#if IS_JAPAN_LOCALIZING()
	
	int nRight	= (int)m_fPosX+100;

#else

	int nRight	= (int)m_fPosX+85;

#endif
	
	RenderFont(m_szInfo, (int)m_fPosX+25, nRight, (int)m_fPosZ+6, (int)m_fPosZ+18, GetStartOrder()+1);

	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE358].szMessage);	// MSG_ID : 358 ; 파티에 가입 하시겠습니까?
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+255, m_fPosZ+40, m_fPosZ+52, GetStartOrder()+1);	
}

void CPartyJoinWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		m_dwUserIndex	= 0;
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_PARTYJOIN_WND, TRUE);
		SetRender(SPR_OBJ_PARTYJOIN_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYJOIN_OK1, TRUE);
		SetRender(SPR_OBJ_PARTYJOIN_CANCEL1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CPartyJoinWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_PARTYJOIN_OK1, TRUE);
		SetRender(SPR_OBJ_PARTYJOIN_OK2, FALSE);
		SetRender(SPR_OBJ_PARTYJOIN_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_PARTYJOIN_CANCEL1, TRUE);
		SetRender(SPR_OBJ_PARTYJOIN_CANCEL2, FALSE);
		SetRender(SPR_OBJ_PARTYJOIN_CANCEL3, FALSE);
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
				CTWS_PARTY_JOIN pPacketSend;
				pPacketSend.bErrCode	= 2;
				pPacketSend.dwUserIndex	= m_dwUserIndex;
				__lstrcpyn(pPacketSend.szName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);

				SetActive(FALSE);	
				
				char szInfo[0xff] = {0,};
				wsprintf(szInfo, g_Message[ETC_MESSAGE359].szMessage, m_szName);	// MSG_ID : 359 ; %s 님의 파티 요청을 거절하셨습니다.
				DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_PARTYJOIN_CLOSE1, FALSE);
				SetRender(SPR_OBJ_PARTYJOIN_CLOSE2, TRUE);
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
				CTWS_PARTY_APPROVE pPacket;
				pPacket.dwUserIndex	= m_dwUserIndex;
				__lstrcpyn(pPacket.szName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_PARTYJOIN_OK1, FALSE);
				SetRender(SPR_OBJ_PARTYJOIN_OK2, FALSE);
				SetRender(SPR_OBJ_PARTYJOIN_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_PARTYJOIN_OK1, FALSE);
				SetRender(SPR_OBJ_PARTYJOIN_OK2, TRUE);
				SetRender(SPR_OBJ_PARTYJOIN_OK3, FALSE);				
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				// 이미 가입 메세지가 왔을 경우 //
				CTWS_PARTY_JOIN pPacketSend;
				pPacketSend.bErrCode	= 2;
				pPacketSend.dwUserIndex	= m_dwUserIndex;
				__lstrcpyn(pPacketSend.szName, m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				g_pNet->SendMsg((char*)&pPacketSend, pPacketSend.GetPacketSize(), SERVER_INDEX_WORLD);
				SetActive(FALSE);				
								
				char szInfo[0xff] = {0,};
				wsprintf(szInfo, g_Message[ETC_MESSAGE359].szMessage, m_szName);	// MSG_ID : 359 ; %s 님의 파티 요청을 거절하셨습니다.
				DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);

				return CInterface::GetInstance()->GetSetWnd();
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{						
				SetRender(SPR_OBJ_PARTYJOIN_CANCEL1, FALSE);
				SetRender(SPR_OBJ_PARTYJOIN_CANCEL2, FALSE);
				SetRender(SPR_OBJ_PARTYJOIN_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_PARTYJOIN_CANCEL1, FALSE);
				SetRender(SPR_OBJ_PARTYJOIN_CANCEL2, TRUE);
				SetRender(SPR_OBJ_PARTYJOIN_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_PARTYJOIN_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYJOIN_CLOSE2, FALSE);		
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
