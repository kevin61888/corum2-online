//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "NpcDungeonInfoWnd.h"
#include "GameControl.h"
#include "Define.h"
#include "DungeonTable.h"
#include "DSMiniMap.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "initgame.h"
#include "CodeFun.h"


CNpcDungeonInfoWnd* CNpcDungeonInfoWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CNpcDungeonInfoWnd::CNpcDungeonInfoWnd()
{ 		
	m_bType	= 0;			
}

CNpcDungeonInfoWnd::~CNpcDungeonInfoWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CNpcDungeonInfoWnd::Init()
{	
		/*
	InsertData(SPR_OBJ_NPCDUNGEONINFO_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_NPCDUNGEONINFO_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_NPCDUNGEONINFO_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_NPCDUNGEONINFO_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NPCDUNGEONINFO_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CNpcDungeonInfoWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CNpcDungeonInfoWnd::SetOrder()
{
}

void CNpcDungeonInfoWnd::RenderText()
{
	CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();
	
	if(pMiniMapWnd->m_bTemp)
	{	
	//	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pMiniMapWnd->m_wMapId);

		// 마을 정보 //	
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE354].szMessage);	// MSG_ID : 354 ; 마을 정보
		RenderFont(m_szInfo, m_fPosX+25, m_fPosX+100, m_fPosZ+5, m_fPosZ+17, GetStartOrder()+1);
	}
}

void CNpcDungeonInfoWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;

	if(bActive==FALSE)
	{
		m_bSoundChk	= FALSE;
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_NPCDUNGEONINFO_WND1, TRUE);
		SetRender(SPR_OBJ_NPCDUNGEONINFO_WND2, TRUE);
		SetRender(SPR_OBJ_NPCDUNGEONINFO_WND3, TRUE);			
		SetRender(SPR_OBJ_NPCDUNGEONINFO_CLOSE1, TRUE);									

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CNpcDungeonInfoWnd::CheckInterface()
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
				SetRender(SPR_OBJ_NPCDUNGEONINFO_CLOSE1, FALSE);
				SetRender(SPR_OBJ_NPCDUNGEONINFO_CLOSE2, TRUE);
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
		SetRender(SPR_OBJ_NPCDUNGEONINFO_CLOSE1, TRUE);
		SetRender(SPR_OBJ_NPCDUNGEONINFO_CLOSE2, FALSE);		
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
