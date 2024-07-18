//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "PartyHelpWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "PartyEntryWnd.h"
#include "CodeFun.h"

CPartyHelpWnd* CPartyHelpWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CPartyHelpWnd::CPartyHelpWnd()
{ 		
}

CPartyHelpWnd::~CPartyHelpWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CPartyHelpWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_PARTYHELP_WND1, SPR_INTERFACE_PARTYHELP_WND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_PARTYHELP_WND2, SPR_INTERFACE_DUNGEONINFOEX_WND3, 0, 32, 1.0f, 55.0f, 0);
	InsertData(SPR_OBJ_PARTYHELP_WND3, SPR_INTERFACE_DUNGEONINFOEX_WND4, 0, 472, 1.0f, 1.0f, 0);	

	InsertData(SPR_OBJ_PARTYHELP_CLOSE1, SPR_INTERFACE_CLOSE1, 407, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYHELP_CLOSE2, SPR_INTERFACE_CLOSE2, 407, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYHELP_HELP1, SPR_INTERFACE_PARTYHELP_DESTROY2, 32, 260, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYHELP_HELP2, SPR_INTERFACE_PARTYHELP_CREATE2, 140, 420, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYHELP_HELP3, SPR_INTERFACE_PARTYHELP_DESTROY2, 280, 420, 1.0f, 1.0f, 2);			

	InsertCheckInterface(408, 4, 488, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 408, 20, 3, CHECK_MOVEWND);	
		 */
	m_bInit	= TRUE;

	return TRUE;
}

void CPartyHelpWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CPartyHelpWnd::SetOrder()
{
}

void CPartyHelpWnd::RenderText()
{	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1153].szMessage); // "파티도움말"
	
	int nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+85, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1154].szMessage); // "* 파티 등록 도움말"
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1155].szMessage); // "파티 등록 : 자신의 파티를 등록할 수 있는 곳입니다."
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+80, m_fPosZ+94, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1156].szMessage); // "파티 정보를 제공하여 파티 성격에 맞는 파티원을"
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+92, m_fPosX+92+nSize*7, m_fPosZ+100, m_fPosZ+114, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1157].szMessage); // "모집할 수 있습니다."
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+92, m_fPosX+92+nSize*7, m_fPosZ+120, m_fPosZ+134, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1158].szMessage); // "파티 보기 : 등록된 파티를 볼 수 있습니다."
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+160, m_fPosZ+174, GetStartOrder()+1);
	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1159].szMessage); // "자신에 맞는 파티를 찾아서 파티장에게 쪽지를 보"
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+92, m_fPosX+92+nSize*7, m_fPosZ+180, m_fPosZ+194, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1160].szMessage); // "내거나 파티 가입요청을 할 수 있습니다."
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+92, m_fPosX+92+nSize*7, m_fPosZ+200, m_fPosZ+214, GetStartOrder()+1);
	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1161].szMessage); // "등록 해제 : NPC에 등록된 파티 정보를 해제할 경우엔"
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+240, m_fPosZ+254, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1162].szMessage); // "우측 하단의 [GROUP]에서 [파티]를 클릭합니다."
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+92, m_fPosX+92+nSize*7, m_fPosZ+260, m_fPosZ+274, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1163].szMessage); // "이 아이템을 누르면 파티 등록이 해제 됩니다."
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+92, m_fPosX+92+nSize*7, m_fPosZ+280, m_fPosZ+294, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1164].szMessage); // "* 파티창에서 직접 등록"
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+320, m_fPosZ+334, GetStartOrder()+1);
	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1165].szMessage); // "파티 등록은 파티창에서 직접 할 수도 있습니다."
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+360, m_fPosZ+374, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1162].szMessage); // "우측 하단의 [GROUP] 에서 [파티]를 클릭합니다."
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+380, m_fPosZ+394, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1166].szMessage); // "파티를 등록하려면          해제를 원하시면"
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+420, m_fPosZ+434, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1167].szMessage); // "를 누르시면 됩니다."
	
	nSize = lstrlen(m_szInfo);	
	RenderFont(m_szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+440, m_fPosZ+454, GetStartOrder()+1);	
}

void CPartyHelpWnd::SetActive(BOOL bActive)
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

		SetRender(SPR_OBJ_PARTYHELP_WND1, TRUE);
		SetRender(SPR_OBJ_PARTYHELP_WND2, TRUE);
		SetRender(SPR_OBJ_PARTYHELP_WND3, TRUE);
		SetRender(SPR_OBJ_PARTYHELP_CLOSE1, TRUE);		
		SetRender(SPR_OBJ_PARTYHELP_HELP1, TRUE);		
		SetRender(SPR_OBJ_PARTYHELP_HELP2, TRUE);		
		SetRender(SPR_OBJ_PARTYHELP_HELP3, TRUE);

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CPartyHelpWnd::CheckInterface()
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
				SetRender(SPR_OBJ_PARTYHELP_CLOSE1, FALSE);
				SetRender(SPR_OBJ_PARTYHELP_CLOSE2, TRUE);
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
		SetRender(SPR_OBJ_PARTYHELP_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYHELP_CLOSE2, FALSE);		
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

