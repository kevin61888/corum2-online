//======================================================//
// Code by Jang.							2002.1.7	//
//======================================================//
#include "CommandWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "InitGame.h"
#include "CodeFun.h"

CCommandWnd* CCommandWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CCommandWnd::CCommandWnd()
{ 
	m_nCurScrollPos	= 0;
	m_nScrollPos	= 0;
	m_byIndex		= 0;
	m_byMaxIndex	= 0;
	m_fSCTemp		= 0;
	m_byScrollChk	= 1;
	m_bMessageChk	= FALSE;
	m_bInit			= FALSE;
}

CCommandWnd::~CCommandWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CCommandWnd::Init()
{
/*
	InsertData(SPR_OBJ_COMMAND_WND1, SPR_INTERFACE_DF_WND4, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_COMMAND_WND2, SPR_INTERFACE_DF_WND5, 32, 0, 57.0f, 1.0f, 0);
	InsertData(SPR_OBJ_COMMAND_WND3, SPR_INTERFACE_DF_WND6, 488, 0, 1.0f, 1.0f, 0);		
	InsertData(SPR_OBJ_COMMAND_CLOSE1, SPR_INTERFACE_CLOSE1, 506, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_COMMAND_CLOSE2, SPR_INTERFACE_CLOSE2, 506, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_COMMAND_SC, SPR_INTERFACE_SC, 507, 32, 1.0f, 1.0f, 1);
	InsertData(SPR_INTERFACE_DFCMD, SPR_INTERFACE_DFCMD, 22, 1, 1.0f, 1.0f, 1);
		
	InsertCheckInterface(506, 4, 518, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 506, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(507, 32, 516, 242, 4, CHECK_SCROLL);
	*/
	SetScroll(10, 32, 242);
	
	m_byIndex		= 0;
	m_byMaxIndex	= 0;	
	m_nCurScrollPos	= 0;
	m_nScrollPos	= 0;	
	m_fSCTemp		= 0;
	m_byScrollChk	= 1;
	m_bInit			= TRUE;	

	return TRUE;
}

void CCommandWnd::Remove()
{
	if(m_bInit)
	{
		RemoveAllData();
		m_bInit			= FALSE;
		m_bActive		= FALSE;
		m_nCurScrollPos	= 0;
		m_nScrollPos	= 0;
		m_byIndex		= 0;
		m_byMaxIndex	= 0;		
		m_fSCTemp		= 0;
		m_byScrollChk	= 1;		
	}
}

void CCommandWnd::SetOrder()
{
}

void CCommandWnd::RenderText()
{	
	for(int i = m_nCurScrollPos; i < m_nCurScrollPos+10; i++)
	{
		if(!IsEmptyString(m_sMsg[i].szMessage))
			RenderFont(m_sMsg[i].szMessage, m_fPosX+10, m_fPosX+510, m_fPosZ+28+10+(i-m_nCurScrollPos)*20, m_fPosZ+28+10+(i-m_nCurScrollPos)*20+15, GetStartOrder()+1, m_sMsg[i].sdwColor);
	}
}

void CCommandWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive==FALSE)
	{
		if(m_bInit)
		{
			m_bSoundChk	= FALSE;
			ShowSpriteAll();			
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
		}
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_COMMAND_WND1, TRUE);
		SetRender(SPR_OBJ_COMMAND_WND2, TRUE);
		SetRender(SPR_OBJ_COMMAND_WND3, TRUE);
		SetRender(SPR_OBJ_COMMAND_CLOSE1, TRUE);
		SetRender(SPR_OBJ_COMMAND_CLOSE2, FALSE);
		SetRender(SPR_OBJ_COMMAND_SC, TRUE);
		SetRender(SPR_OBJ_DFCMD, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CCommandWnd::CheckInterface()
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
				SetRender(SPR_OBJ_COMMAND_CLOSE1, FALSE);
				SetRender(SPR_OBJ_COMMAND_CLOSE2, TRUE);
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

			if(eMouseCheck==MOUSE_DOWN)
			{
				if(GetCheckScroll())
					SetPosObjY(SPR_OBJ_COMMAND_SC, m_fScrollPos-m_fPosZ);
			}			
		}
		break;
	}			
		

	if(MouseScrollChk(nRt, 4))	
	{
		SetPosObjY(SPR_OBJ_COMMAND_SC, m_fScrollPos-m_fPosZ);			
		return CInterface::GetInstance()->GetSetWnd();
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_COMMAND_CLOSE1, TRUE);
		SetRender(SPR_OBJ_COMMAND_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}

void CCommandWnd::SetSize(int nCount)
{
	m_byMaxIndex = (BYTE)nCount;
	memset(m_sMsg, 0, sizeof(m_sMsg));	
}

BOOL CCommandWnd::MessageAdd(char* pszMessage, DWORD dwCol)
{
	if(m_byIndex<m_byMaxIndex)
	{
		lstrcpy(m_sMsg[m_byIndex].szMessage, pszMessage);
		m_sMsg[m_byIndex].sdwColor = dwCol;
		m_byIndex++;

		if(m_byIndex>10)
			m_nScrollPos++;

		m_nCurScrollPos = m_nScrollPos;
		SetPosObjY(SPR_OBJ_COMMAND_SC, 242-16);
		m_fSCTemp = 242-16-32;
		return TRUE;
	}
	return FALSE;
}
//======================================================//
// End.													//
//======================================================//
