//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "GuildPortalWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "Chat.h"
#include "message.h"
#include "InitGame.h"
#include "CodeFun.h"

CGuildPortalWnd* CGuildPortalWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGuildPortalWnd::CGuildPortalWnd()
{ 
	m_dwUserIndex	= 0;	
	m_byResult		= 0;
	m_bActiveChk	= FALSE;	
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
}

CGuildPortalWnd::~CGuildPortalWnd()
{
	
}


//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGuildPortalWnd::Init()
{	
/*
	InsertData(SPR_OBJ_GUILDPORTAL_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILDPORTAL_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDPORTAL_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDPORTAL_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDPORTAL_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDPORTAL_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDPORTAL_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDPORTAL_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDPORTAL_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
*/
	m_bActiveChk = FALSE;
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildPortalWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildPortalWnd::SetOrder()
{
}

void CGuildPortalWnd::RenderText()
{
	int nRight = 0;

	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE612].szMessage); // "포탈"
	
#if IS_JAPAN_LOCALIZING()
	
	nRight	= (int)m_fPosX+85;

#else

	nRight	= (int)m_fPosX+55;

#endif
	
	RenderFont(m_szInfo, (int)m_fPosX+25, nRight, (int)m_fPosZ+6, (int)m_fPosZ+20, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE889].szMessage, m_szName); // "%s 님이 길드포탈을 요청을"
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);
	
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE629].szMessage); // "하셨습니다. 포탈을 하시겠습니까?"
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+250, m_fPosZ+60, m_fPosZ+74, GetStartOrder()+1);	
}

void CGuildPortalWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		if(m_bActiveChk)
		{			
			if(g_pMainPlayer->m_bMatching)
			{
				// "대결 중에는 사용할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF); 
			}

			m_bActiveChk = FALSE;
		}

		m_bSoundChk		= FALSE;
		m_dwUserIndex	= 0;
		m_byResult		= 0;

		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{	
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDPORTAL_WND, TRUE);
		SetRender(SPR_OBJ_GUILDPORTAL_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDPORTAL_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDPORTAL_CLOSE1, TRUE);							
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildPortalWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDPORTAL_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDPORTAL_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDPORTAL_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDPORTAL_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDPORTAL_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDPORTAL_CANCEL3, FALSE);
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
				SetRender(SPR_OBJ_GUILDPORTAL_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDPORTAL_CLOSE2, TRUE);
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
				m_byResult	= 1;
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUILDPORTAL_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDPORTAL_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDPORTAL_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_UP)
			{				
				SetRender(SPR_OBJ_GUILDPORTAL_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDPORTAL_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDPORTAL_OK3, FALSE);				
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
				SetRender(SPR_OBJ_GUILDPORTAL_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDPORTAL_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDPORTAL_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_UP)
			{					
				SetRender(SPR_OBJ_GUILDPORTAL_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDPORTAL_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDPORTAL_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDPORTAL_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDPORTAL_CLOSE2, FALSE);		
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
