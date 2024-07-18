// GuildWarInvite.cpp: implementation of the CGuildWarInvite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildWarInvite.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "Party.h"
#include "GroupWnd.h"
#include "RivalGuildWar.h"

CGuildWarInvite* CGuildWarInvite::c_pThis = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildWarInvite::CGuildWarInvite()
{
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
}

CGuildWarInvite::~CGuildWarInvite()
{

}

BOOL CGuildWarInvite::Init()
{
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildWarInvite::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildWarInvite::SetOrder()
{
}

void CGuildWarInvite::RenderText()
{
//	char szTmp[50];
//	ZeroMemory(szTmp, sizeof(szTmp));
//	lstrcpy(szTmp, g_Message[ETC_MESSAGE1366].szMessage);
		
//	RenderFont(szTmp, (int)m_fPosX+25, (int)m_fPosX+85, (int)m_fPosZ+6, (int)m_fPosZ+18, GetStartOrder()+1);
	int nSize = strlen(g_Message[ETC_MESSAGE1392].szMessage);// "도전길드전이 벌어졌습니다. "
	RenderFont(g_Message[ETC_MESSAGE1392].szMessage, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+52, GetStartOrder()+1);	
	nSize = strlen(g_Message[ETC_MESSAGE1393].szMessage);// "길드전에 참여하시겠습니까? "
	RenderFont(g_Message[ETC_MESSAGE1393].szMessage, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+60, m_fPosZ+72, GetStartOrder()+1);	
}

void CGuildWarInvite::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDWAR_INVITE_WND, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_INVITE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_INVITE_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);


	}
}

int CGuildWarInvite::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDWAR_INVITE_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_INVITE_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDWAR_INVITE_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL3, FALSE);
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
			{
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUILDWAR_INVITE_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4://확인버튼
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

				if(pGroupWnd->m_bGuildWarMatchEnable)
				{
					pGroupWnd->m_bGuildWarPacketSendEnable = FALSE;
					TEAM_MATCH_PACKET TeamMatch;
					TeamMatch.Body.Condtion.iMatchType = M_T_NONE;
					TeamMatch.Body.Condtion.iMatchGuildNumber = 0;
					TeamMatch.Body.Condtion.iMatchGuildType = T_T_NONE;
					Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_REMOVE);
				}

				pGroupWnd->m_byGuildWarType = 2;
				pGroupWnd->m_bGuildWarMatchEnable = TRUE;
				pGroupWnd->m_bGuildWarPacketSendEnable = FALSE;
				TEAM_MATCH_PACKET TeamMatch;
				TeamMatch.Body.Condtion.iMatchType = GetFormConvert((int)pGroupWnd->m_bynGuildWarForm);
				TeamMatch.Body.Condtion.iMatchGuildNumber = GetWarNumber((int)pGroupWnd->m_byGuildWarNumber);
				TeamMatch.Body.Condtion.iMatchGuildType = GetTypeConvert((int)pGroupWnd->m_byGuildWarType);
				Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_INSERT);

				SetActive(FALSE);

				DisplayMessageAdd(g_Message[ETC_MESSAGE1399].szMessage, TEXT_COLOR_YELLOW);// "길드전초대를 수락하셨습니다."
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDWAR_INVITE_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_GUILDWAR_INVITE_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_OK3, FALSE);				
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5://취소버튼
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{						
				SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDWAR_INVITE_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDWAR_INVITE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_INVITE_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}
