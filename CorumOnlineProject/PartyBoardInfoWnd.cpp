//======================================================//
// Code by Jang.							2004.6.14	//
//======================================================//
#include "PartyBoardInfoWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "PartyEntryWnd.h"


CPartyBoardInfoWnd* CPartyBoardInfoWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CPartyBoardInfoWnd::CPartyBoardInfoWnd()
{ 		
}

CPartyBoardInfoWnd::~CPartyBoardInfoWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CPartyBoardInfoWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_PARTYBOARDINFO_WND1, SPR_INTERFACE_DUNGEONINFOEX_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_PARTYBOARDINFO_WND2, SPR_INTERFACE_DUNGEONINFOEX_WND2, 0, 32, 1.0f, 55.0f, 0);
	InsertData(SPR_OBJ_PARTYBOARDINFO_WND3, SPR_INTERFACE_DUNGEONINFOEX_WND3, 0, 472, 1.0f, 1.0f, 0);	
	InsertData(SPR_OBJ_PARTYBOARDINFO_WND4, SPR_INTERFACE_DUNGEONINFOEX_WND4, 0, 472, 1.0f, 1.0f, 0);	
	InsertData(SPR_OBJ_PARTYBOARDINFO_CLOSE1, SPR_INTERFACE_CLOSE1, 407, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYBOARDINFO_CLOSE2, SPR_INTERFACE_CLOSE2, 407, 4, 1.0f, 1.0f, 1);
	

	InsertCheckInterface(408, 4, 488, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 408, 20, 3, CHECK_MOVEWND);	
		 */
	m_bInit	= TRUE;

	return TRUE;
}

void CPartyBoardInfoWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CPartyBoardInfoWnd::SetOrder()
{
}

void CPartyBoardInfoWnd::RenderText()
{
	/*int nOrder = __ORDER_INTERFACE_START__ + m_byOrder*10;

	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1153].szMessage); // "파티도움말"
	
	int nSize		= lstrlen(m_szInfo);

	m_rtPos.left	= (LONG)m_fPosX+25;
	m_rtPos.right	= (LONG)m_fPosX+85;
	m_rtPos.top		= (LONG)m_fPosZ+6;
	m_rtPos.bottom	= (LONG)m_fPosZ+20;		
	g_pGeometry->RenderFont(GetFont(), m_szInfo, nSize, &m_rtPos, 0xffffffff, CHAR_CODE_TYPE_ASCII, nOrder+1, 0);*/
}

void CPartyBoardInfoWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{	
		m_bSoundChk	= FALSE;
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_PARTYBOARDINFO_WND1, TRUE);
		SetRender(SPR_OBJ_PARTYBOARDINFO_WND2, TRUE);
		SetRender(SPR_OBJ_PARTYBOARDINFO_WND3, TRUE);
		SetRender(SPR_OBJ_PARTYBOARDINFO_WND4, TRUE);
		SetRender(SPR_OBJ_PARTYBOARDINFO_CLOSE1, TRUE);		
		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CPartyBoardInfoWnd::CheckInterface()
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
				SetRender(SPR_OBJ_PARTYBOARDINFO_CLOSE1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARDINFO_CLOSE2, TRUE);
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
		SetRender(SPR_OBJ_PARTYBOARDINFO_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYBOARDINFO_CLOSE2, FALSE);		
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

