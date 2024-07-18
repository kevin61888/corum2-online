//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "BlockWnd.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "HashTable.h"
#include "CodeFun.h"


CBlockWnd* CBlockWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CBlockWnd::CBlockWnd()
{ 
	m_bPointer	= FALSE;
	m_bInfoType = 0;
}

CBlockWnd::~CBlockWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CBlockWnd::Init()
{	
/*
	InsertData(SPR_OBJ_BLOCK_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_BLOCK_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_BLOCK_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_BLOCK_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_BLOCK_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CBlockWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CBlockWnd::SetOrder()
{
}

void CBlockWnd::SetActive(BOOL bActive)
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
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_BLOCK_WND1, TRUE);
		SetRender(SPR_OBJ_BLOCK_WND2, TRUE);
		SetRender(SPR_OBJ_BLOCK_WND3, TRUE);
		SetRender(SPR_OBJ_BLOCK_CLOSE1, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

void CBlockWnd::RenderText()
{
	// 좌측 상단 좌표 //	
	DWORD dwColor = 0xffffffff;
	
	for(int i=0; i<MAX_BLOCK_MESSAGE_LINE_COUNT; i++)
	{
		if(IsEmptyString(szMessageLine[i])) return;
		
		dwColor = (i==0) ? 0xffff0000 : 0xffffffff;
				
		int nTop	= (int)m_fPosZ + 50;
		int nBottom = (int)m_fPosZ + 70;

		RenderFont(szMessageLine[i], int(m_fPosZ + 50), int(m_fPosX + 415), nTop, nBottom, GetStartOrder()+1, dwColor);
		
		if(i==0)	
		{
			nTop	+= 20;
			nBottom += 20;
		}

		nTop	+= 20;
		nBottom += 20;
	}
}

int CBlockWnd::CheckInterface()
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
				SetRender(SPR_OBJ_BLOCK_CLOSE1, FALSE);
				SetRender(SPR_OBJ_BLOCK_CLOSE2, TRUE);
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
		SetRender(SPR_OBJ_BLOCK_CLOSE1, TRUE);
		SetRender(SPR_OBJ_BLOCK_CLOSE2, FALSE);		
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