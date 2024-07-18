//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "WorldWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "HashTable.h"
#include "CodeFun.h"


CWorldWnd* CWorldWnd::c_pThis = NULL;


//======================================================//
// Construction/Destrution.								//
//======================================================//
CWorldWnd::CWorldWnd()
{ 
	m_bPointer	= FALSE;
	m_bInfoType = 0;
	
	for(int i = 0; i < 7; i++)
		memset(szMessageLine[i], 0, sizeof(szMessageLine[i]));
}

CWorldWnd::~CWorldWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CWorldWnd::Init()
{	
/*
	InsertData(SPR_OBJ_WORLD_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_WORLD_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_WORLD_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_WORLD_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_WORLD_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CWorldWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CWorldWnd::SetOrder()
{
}

void CWorldWnd::RenderText()
{
	// 좌측 상단 좌표 //
	RECT rect;
	rect.top = (int)(m_fPosZ + 50);
	rect.left = (int)(m_fPosX + 15);
	rect.bottom = rect.top + 20;
	rect.right = rect.left + 400;

	DWORD dwColor=0xffffffff;
	
	for(int i=0; i<7; i++)
	{
		if(IsEmptyString(szMessageLine[i])) return;
		
		if(i==2 && !m_bInfoType)
			dwColor = 0xff00ffff;
		else
			dwColor = 0xffffffff;

		RenderFont(szMessageLine[i], rect.left, rect.right, rect.top, rect.bottom, GetStartOrder()+1, dwColor);		
		
		if(i==2 && !m_bInfoType)	
		{
			rect.top += 20;
			rect.bottom += 20;
		}
		rect.top += 20;
		rect.bottom += 20;
	}
}

void CWorldWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();
		//if ( GetGameStatus() != UPDATE_GAME_LOGIN)   // 로그인시 꽝하는 사운드 이펙트 안나오게 안한다. 김영대 2003.02.22
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_WORLD_WND1, TRUE);
		SetRender(SPR_OBJ_WORLD_WND2, TRUE);
		SetRender(SPR_OBJ_WORLD_WND3, TRUE);
		SetRender(SPR_OBJ_WORLD_CLOSE1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CWorldWnd::CheckInterface()
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
				SetRender(SPR_OBJ_WORLD_CLOSE1, FALSE);
				SetRender(SPR_OBJ_WORLD_CLOSE2, TRUE);
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
		SetRender(SPR_OBJ_WORLD_CLOSE1, TRUE);
		SetRender(SPR_OBJ_WORLD_CLOSE2, FALSE);		
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
