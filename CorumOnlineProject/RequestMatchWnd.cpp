//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "RequestMatchWnd.h"
#include "GameControl.h"
#include "User.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "CodeFun.h"


CRequestMatchWnd* CRequestMatchWnd::c_pThis = NULL;

//======================================================//
// Construction/Desertion.								//
//======================================================//
CRequestMatchWnd::CRequestMatchWnd()
{
	m_dwRequestMatchEndTick = 0;
	m_dwRequestMatchUserIndex = 0;

	for(int i = 0; i < 2; i++)
		m_bBtnChk[i] = FALSE;
}

CRequestMatchWnd::~CRequestMatchWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CRequestMatchWnd::Init()
{
/*
	InsertData(SPR_OBJ_REQUESTMATCH_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_REQUESTMATCH_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_REQUESTMATCH_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_REQUESTMATCH_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_REQUESTMATCH_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_REQUESTMATCH_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_REQUESTMATCH_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_REQUESTMATCH_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_REQUESTMATCH_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CRequestMatchWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CRequestMatchWnd::SetOrder()
{
}

void CRequestMatchWnd::RenderText()
{	
	RenderFont(g_Message[ETC_MESSAGE636].szMessage, m_fPosX+25, m_fPosX+55, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);
	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE637].szMessage, m_szName); // "%s 님이 대결을 신청 하셨습니다. "
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

	RenderFont(g_Message[ETC_MESSAGE638].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+60, m_fPosZ+74, GetStartOrder()+1);
}

void CRequestMatchWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive==FALSE)
	{
		m_bSoundChk	= FALSE;		
		ShowSpriteAll();
		if ( m_dwRequestMatchUserIndex )
		{ //  	
			// 대전 거절  
			CTDS_REPLY_MATCH_MAN2MAN	packet;
			packet.bReplyCode = 2;
			packet.dwRequestUserIndex = m_dwRequestMatchUserIndex ;

			g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);

			CUserInterface::GetInstance()->m_bMatch	= FALSE;
			m_dwRequestMatchUserIndex = 0;			
		}
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{	
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_REQUESTMATCH_WND, TRUE);
		SetRender(SPR_OBJ_REQUESTMATCH_CLOSE1, TRUE);
		SetRender(SPR_OBJ_REQUESTMATCH_OK1, TRUE);
		SetRender(SPR_OBJ_REQUESTMATCH_CANCEL1, TRUE);		
		m_dwRequestMatchEndTick = g_dwCurTick + REQUEST_END_TICK; // 자동 거절 시간 세팅 

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CRequestMatchWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();		

	if(m_bBtnChk[0]==TRUE && nRt!=4)
	{
		SetRender(SPR_OBJ_REQUESTMATCH_OK1, TRUE);
		SetRender(SPR_OBJ_REQUESTMATCH_OK2, FALSE);
		SetRender(SPR_OBJ_REQUESTMATCH_OK3, FALSE);
		m_bBtnChk[0] = FALSE;
	}
	if(m_bBtnChk[1]==TRUE && nRt!=5)
	{
		SetRender(SPR_OBJ_REQUESTMATCH_CANCEL1, TRUE);
		SetRender(SPR_OBJ_REQUESTMATCH_CANCEL2, FALSE);
		SetRender(SPR_OBJ_REQUESTMATCH_CANCEL3, FALSE);
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
				CTDS_REPLY_MATCH_MAN2MAN	packet;
				packet.bReplyCode = 2;
				packet.dwRequestUserIndex = m_dwRequestMatchUserIndex ;
				g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);				
				m_dwRequestMatchUserIndex = 0;
				CUserInterface::GetInstance()->m_bMatch	= FALSE;

				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_REQUESTMATCH_CLOSE1, FALSE);
				SetRender(SPR_OBJ_REQUESTMATCH_CLOSE2, TRUE);
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
				if(!(g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_SHOP
					|| g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_REST))
				{					
					g_pMainPlayer->SetAction(MOTION_TYPE_WARSTAND, 0, ACTION_LOOP );		
					g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
					g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
					SendStopPacket();
					g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
				}
				
				// 대결을 허용  //
				CTDS_REPLY_MATCH_MAN2MAN packet;
				packet.bReplyCode = 1;
				packet.dwRequestUserIndex = m_dwRequestMatchUserIndex ;
				g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
				m_dwRequestMatchUserIndex = 0;

				CUserInterface::GetInstance()->m_bMatch	= FALSE;
				
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_REQUESTMATCH_OK1, FALSE);
				SetRender(SPR_OBJ_REQUESTMATCH_OK2, FALSE);
				SetRender(SPR_OBJ_REQUESTMATCH_OK3, TRUE);				
				m_bBtnChk[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_REQUESTMATCH_OK1, FALSE);
				SetRender(SPR_OBJ_REQUESTMATCH_OK2, TRUE);
				SetRender(SPR_OBJ_REQUESTMATCH_OK3, FALSE);				
				m_bBtnChk[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{						
			if(eMouseCheck==MOUSE_UP)
			{								
				SetActive(FALSE);				
				
				// 대전 거부 //	
				CTDS_REPLY_MATCH_MAN2MAN packet;		
				packet.bReplyCode		= 2;
				packet.dwRequestUserIndex = m_dwRequestMatchUserIndex ;				
				g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
				m_dwRequestMatchUserIndex = 0;

				CUserInterface::GetInstance()->m_bMatch	= FALSE;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{								
				SetRender(SPR_OBJ_REQUESTMATCH_CANCEL1, FALSE);
				SetRender(SPR_OBJ_REQUESTMATCH_CANCEL2, FALSE);
				SetRender(SPR_OBJ_REQUESTMATCH_CANCEL3, TRUE);					
				m_bBtnChk[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_REQUESTMATCH_CANCEL1, FALSE);
				SetRender(SPR_OBJ_REQUESTMATCH_CANCEL2, TRUE);
				SetRender(SPR_OBJ_REQUESTMATCH_CANCEL3, FALSE);
				m_bBtnChk[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_REQUESTMATCH_CLOSE1, TRUE);
		SetRender(SPR_OBJ_REQUESTMATCH_CLOSE2, FALSE);		
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