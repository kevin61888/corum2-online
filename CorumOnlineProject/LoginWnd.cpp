// LoginWnd.cpp: implementation of the CLoginWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LoginWnd.h"
#include "SoundLib.h"
#include "DungeonProcess_Sound.h"
#include "GlobalSoundHandles.h"
#include "CodeFun.h"
#include "Interface.h"

CLoginWnd* CLoginWnd::c_pThis = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLoginWnd::CLoginWnd()
{

}

CLoginWnd::~CLoginWnd()
{

}

CLoginWnd::Init()
{
	/*
	g_pSprManager->CreateSprite(SPR_LOGIN_BACKGROUND, 0.0f, 0.0f, TRUE, 254); // resourceid 344
	g_pSprManager->CreateSprite(SPR_LOGIN_SERVERSELECT_WINDOW, 402, 380, FALSE);// resourceid 345
	V2_SPRITE* pSpr = g_pSprManager->CreateSprite(SPR_CS_BLACK_LINE, 0, 0, TRUE, 255 );// resourceid 346
	pSpr->v2Scaling.x = 8.0f;
	pSpr->v2Scaling.y = 6.0f;

	

	g_pSprManager->CreateSprite(SPR_LOGIN_LOGIN_WINDOW, 402, 459, FALSE);// resourceid 347

	g_pSprManager->CreateSprite(BUTTON_OK		, 523, 568, 0, 0, 64, 16, FALSE, 1);// resourceid 348
	g_pSprManager->CreateSprite(BUTTON_OK_OVER	, 523, 568, 0, 16, 64, 16, FALSE, 1);// resourceid 349
	g_pSprManager->CreateSprite(BUTTON_OK_DOWN  , 523, 568, 0, 32, 64, 16, FALSE, 1);// resourceid 350

	g_pSprManager->CreateSprite(BUTTON_CANCEL		, 588, 568, 64, 0, 64, 16, FALSE, 1);// resourceid 351
	g_pSprManager->CreateSprite(BUTTON_CANCEL_OVER	, 588, 568, 64, 16, 64, 16, FALSE, 1);// resourceid 352
	g_pSprManager->CreateSprite(BUTTON_CANCEL_DOWN  , 588, 568, 64, 32, 64, 16, FALSE, 1);// resourceid 353
	
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT1, 578, 618, 128, 0, 64, 16, FALSE, 4);// resourceid 354
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT2, 578, 618, 128, 16, 64, 16, FALSE, 4);// resourceid 355
	g_pSprManager->CreateSprite(SPR_LOGIN_EXIT3, 578, 618, 128, 32, 64, 16, FALSE, 4);	// resourceid 356
*/
	SetMousePointerType(__MOUSE_POINTER_DEFAULT__);

	m_bInit	= TRUE;
	return 0;
}

void CLoginWnd::SetOrder()
{
}

void CLoginWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}
void CLoginWnd::RenderText()
{
	
}

void CLoginWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CLoginWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	
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
		
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
		
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}