//
//	Game Notice message display window.
//
//				2003/11/13
//				Trust Pak


#include "Menu.h"
#include "InitGame.h"
#include "Message.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "GameNoticeWnd.h"
#include "WorldProcess.h"
#include "CodeFun.h"
#include "Interface.h"

CGameNoticeMessageWnd* CGameNoticeMessageWnd::c_pThis = NULL;

//////////////////////////////////////////////////////////////////////////////
// Class Name : CGameNoticeMessageWnd
// Description :
//
//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::CGameNoticeMessageWnd
// Description : Constructor.
//----------------------------------------------------------------------------
CGameNoticeMessageWnd::CGameNoticeMessageWnd()
{
}

//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::~CGameNoticeMessageWnd
// Description : Destructor.
//----------------------------------------------------------------------------
CGameNoticeMessageWnd::~CGameNoticeMessageWnd()
{
}


//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::Init
// Description : 
//----------------------------------------------------------------------------
BOOL CGameNoticeMessageWnd::Init()
{	/*
	// defualt window sprites.	
	InsertData(SPR_OBJ_NOTICE_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_NOTICE_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 8.0f, 0);
	InsertData(SPR_OBJ_NOTICE_WND3, SPR_INTERFACE_DF_WND3, 0, 160, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_NOTICE_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NOTICE_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	*/
	m_bInit	= TRUE;		
	
	return TRUE;
}

//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::Render
// Description : 
//----------------------------------------------------------------------------
void CGameNoticeMessageWnd::RenderText()
{	
	int		nSize			= lstrlen(g_Message[ETC_MESSAGE415].szMessage);	
	char	szInfo[0xff]	= {0,};
	
	wsprintf(szInfo, g_Message[ETC_MESSAGE415].szMessage, lstrlen(g_Message[ETC_MESSAGE415].szMessage)); // "공지사항"
	RenderFont(szInfo, m_fPosX+25, m_fPosX+25+nSize*6, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);
	
	RenderTextEx();
}

//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::Remove
// Description : 
//----------------------------------------------------------------------------
void CGameNoticeMessageWnd::Remove()
{		
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;	
}

//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::SetOrder
// Description : 
//----------------------------------------------------------------------------
void CGameNoticeMessageWnd::SetOrder()
{	
}

//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::SetActive
// Description : 
//----------------------------------------------------------------------------
void CGameNoticeMessageWnd::SetActive(BOOL bActive)
{		
	m_bActive	= bActive;

	if(bActive == FALSE)
	{		
		m_bSoundChk	= FALSE;	
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{	
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		// Game notice message window has default position.				
		SetRender(SPR_OBJ_NOTICE_WND1, TRUE);
		SetRender(SPR_OBJ_NOTICE_WND2, TRUE);
		SetRender(SPR_OBJ_NOTICE_WND3, TRUE);
		SetRender(SPR_OBJ_NOTICE_CLOSE1, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}	
}

//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::CheckInterface
// Description : 
//----------------------------------------------------------------------------
int CGameNoticeMessageWnd::CheckInterface()
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
				SetRender(SPR_OBJ_NOTICE_CLOSE1, FALSE);
				SetRender(SPR_OBJ_NOTICE_CLOSE2, TRUE);
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
		SetRender(SPR_OBJ_NOTICE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_NOTICE_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		
	
	return 0;
}
//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::RenderText
// Description : 
//----------------------------------------------------------------------------
void CGameNoticeMessageWnd::RenderTextEx()
{
	// Display notice message.
	for(int i = 1; i < g_byNoticeCount; i++)
	{
	//	int nSize = (g_byLen[i -1] / 2) * 8;
		RenderFont(g_sNoticeMessage[i], this->GetPosX() + 15, this->GetPosX() + 15 + g_byLen[i] * 8, this->GetPosY() + 25 + ((i -1) * 18), this->GetPosY() + 49 + ((i -1) * 18), GetStartOrder()+1);
	}
}

//----------------------------------------------------------------------------
// Name : CGameNoticeMessageWnd::SetPosition
// Description :
//----------------------------------------------------------------------------
void CGameNoticeMessageWnd::SetPosition(float fX, float fZ)
{
	m_fPrevPosX = fX;
	m_fPrevPosZ = fZ;
	m_fPosX = fX;
	m_fPosZ = fZ;
}
//----------------------------------------------------------------------------
