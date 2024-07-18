//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "CharMoveWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "PacketEncrypt.h"


CCharMoveWnd* CCharMoveWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CCharMoveWnd::CCharMoveWnd()
{ 	
	m_bBtn[0]	= FALSE;
	m_bBtn[1]	= FALSE;
	m_bPackChk	= FALSE;
}

CCharMoveWnd::~CCharMoveWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CCharMoveWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_CHARMOVE_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHARMOVE_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHARMOVE_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHARMOVE_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVE_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVE_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVE_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVE_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVE_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CCharMoveWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CCharMoveWnd::SetOrder()
{
}

void CCharMoveWnd::RenderText()
{
	char szInfo[0xff] = {0, };

	lstrcpy(szInfo, g_Message[ETC_MESSAGE1094].szMessage); // "알림 - 캐릭터 이전"
				
	int nSize = lstrlen(szInfo);
	
	RenderFont(szInfo, m_fPosX+15, m_fPosX+15+nSize*4, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);	
	
	for(int i = 0; i < m_byCnt; i++)
	{
		int nSize = lstrlen(m_szInfo[i]);
		
		RenderFont(m_szInfo[i], m_fPosX+15, m_fPosX+15+nSize*4, m_fPosZ+40+i*20, m_fPosZ+52+i*20, GetStartOrder()+1);			
	}	
}

void CCharMoveWnd::SetText(char* pszText1, char* pszText2, char* pszText3, BYTE byCnt)
{
	lstrcpy(m_szInfo[0], pszText1);
	lstrcpy(m_szInfo[1], pszText2);
	lstrcpy(m_szInfo[2], pszText3);
	m_byCnt = byCnt;
}

void CCharMoveWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{		
		if(g_bCharMoveChk && !m_bPackChk)
			g_bCharMoveChk = FALSE;
		
		m_bPackChk = FALSE;

		ShowSpriteAll();
		//_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_CHARMOVE_WND, TRUE);
		SetRender(SPR_OBJ_CHARMOVE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_CHARMOVE_OK1, TRUE);
		SetRender(SPR_OBJ_CHARMOVE_CANCEL1, TRUE);
		
		//_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CCharMoveWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_CHARMOVE_OK1, TRUE);
		SetRender(SPR_OBJ_CHARMOVE_OK2, FALSE);
		SetRender(SPR_OBJ_CHARMOVE_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_CHARMOVE_CANCEL1, TRUE);
		SetRender(SPR_OBJ_CHARMOVE_CANCEL2, FALSE);
		SetRender(SPR_OBJ_CHARMOVE_CANCEL3, FALSE);
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
				SetRender(SPR_OBJ_CHARMOVE_CLOSE1, FALSE);
				SetRender(SPR_OBJ_CHARMOVE_CLOSE2, TRUE);
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
				CTAS_CHR_MOVECHK pPacket;
				pPacket.byType	= 1;

				//	by minjin. 2004. 10. 25.
				SendMsgWithBlowfish((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_AGENT);
				//g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_AGENT );

				m_bPackChk	= TRUE;
				m_bBtn[0]	= TRUE;
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_CHARMOVE_OK1, FALSE);
				SetRender(SPR_OBJ_CHARMOVE_OK2, FALSE);
				SetRender(SPR_OBJ_CHARMOVE_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}			
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_CHARMOVE_OK1, FALSE);
				SetRender(SPR_OBJ_CHARMOVE_OK2, TRUE);
				SetRender(SPR_OBJ_CHARMOVE_OK3, FALSE);
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
				SetRender(SPR_OBJ_CHARMOVE_CANCEL1, FALSE);
				SetRender(SPR_OBJ_CHARMOVE_CANCEL2, FALSE);
				SetRender(SPR_OBJ_CHARMOVE_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_CHARMOVE_CANCEL1, FALSE);
				SetRender(SPR_OBJ_CHARMOVE_CANCEL2, TRUE);
				SetRender(SPR_OBJ_CHARMOVE_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}			
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_CHARMOVE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_CHARMOVE_CLOSE2, FALSE);		
	}

	MouseChk(nRt);	

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}
//======================================================//
// End.													//
//======================================================//