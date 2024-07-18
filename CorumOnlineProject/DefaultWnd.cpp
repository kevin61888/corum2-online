//======================================================//
// Code by Jang.							2002.5.27	//
//======================================================//
#include "DefaultWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "InitGame.h"

CDefaultWnd* CDefaultWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CDefaultWnd::CDefaultWnd()
{
	
}

CDefaultWnd::~CDefaultWnd()
{
	
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CDefaultWnd::Init()
{
	/*
	InsertData(SPR_OBJ_DEFAULT_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_DEFAULT_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DEFAULT_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DEFAULT_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DEFAULT_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DEFAULT_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DEFAULT_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DEFAULT_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DEFAULT_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 1);			

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
*/
	m_bInit = TRUE;

	return TRUE;
}

void CDefaultWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CDefaultWnd::SetOrder()
{
}

void CDefaultWnd::RenderText()
{
}

void CDefaultWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive==FALSE)
	{		
		m_bSoundChk	= FALSE;
		ShowSpriteAll();
		//if ( GetGameStatus() != UPDATE_GAME_LOGIN)   // 로그인시 꽝하는 사운드 이펙트 안나오게 안한다. 김영대 2003.02.22
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_DEFAULT_WND, TRUE);
		SetRender(SPR_OBJ_DEFAULT_CLOSE1, TRUE);	
		SetRender(SPR_OBJ_DEFAULT_OK1, TRUE);	
		SetRender(SPR_OBJ_DEFAULT_CANCEL1, TRUE);
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CDefaultWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

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
				SetRender(SPR_OBJ_DEFAULT_CLOSE1, FALSE);
				SetRender(SPR_OBJ_DEFAULT_CLOSE2, TRUE);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}			
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_DEFAULT_CLOSE1, TRUE);
		SetRender(SPR_OBJ_DEFAULT_CLOSE2, FALSE);		
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
