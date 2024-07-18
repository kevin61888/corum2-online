#include "EventDungeonWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "InitGame.h"
#include "message.h"
#include "CodeFun.h"


CEventDungeonWnd* CEventDungeonWnd::c_pThis = NULL;


CEventDungeonWnd::CEventDungeonWnd()
{
	
}


CEventDungeonWnd::~CEventDungeonWnd()
{

}


BOOL CEventDungeonWnd::Init()
{	
/*
	InsertData(SPR_OBJ_EVENTDUNGEON_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_EVENTDUNGEON_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 8.0f, 0);
	InsertData(SPR_OBJ_EVENTDUNGEON_WND3, SPR_INTERFACE_DF_WND3, 0, 160, 1.0f, 1.0f, 0);

	InsertData(SPR_OBJ_EVENT_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_EVENT_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);

	InsertCheckInterface(0, 0, 242, 20, 2, CHECK_MOVEWND);
	InsertCheckInterface(242, 4, 256, 16, 3, CHECK_CLOSE);
*/
	m_bInit	= TRUE;	
	
	return TRUE;
}


void CEventDungeonWnd::Remove()
{	
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}


void CEventDungeonWnd::SetOrder()
{

}


void CEventDungeonWnd::RenderText()
{	
	int nPosX = int(m_fPosX+10);
	int nPosY = int(m_fPosZ+10+32);

	char szMessage[0xff] = {0,};
		
	if(EVENT_FAILED == m_iMessageType)
	{
		// "이벤트 던젼 클리어 실패했습니다. "		
		__lstrcpyn(szMessage, g_Message[ETC_MESSAGE557].szMessage, 0xfe); 
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
	}
	else if(EVENT_DUNGEON_SURVIVAL_FIRST_TIMEOUT == m_iMessageType)
	{		
		nPosY -= 17;
		
		// "생존 던전의 1차 이벤트가 끝났습니다. "
		__lstrcpyn(szMessage, g_Message[ETC_MESSAGE1056].szMessage, 0xfe); 
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;

		float ExpRatio = (float)m_bDungeonMonsterExpRatio;		
		ExpRatio = ExpRatio / 100.f;
		wsprintf(szMessage,g_Message[ETC_MESSAGE1258].szMessage,ExpRatio); 
		// "살아남은 분들 모두 %.1f배의 경험치를 얻게"
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;		

		// "되었습니다. 이후 2차 이벤트에서는 일정"
		__lstrcpyn(szMessage, g_Message[ETC_MESSAGE1058].szMessage, 0xfe); 
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;

		// "시간동안 던전안에서 P.K가 가능하게 됩니다."
		__lstrcpyn(szMessage,g_Message[ETC_MESSAGE1059].szMessage, 0xfe); 
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;

		// "2차 이벤트가 끝난 후 살아남은 최후의"
		__lstrcpyn(szMessage, g_Message[ETC_MESSAGE1060].szMessage, 0xfe); 
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;

		// "1인에게 상금이 주어집니다"
		__lstrcpyn(szMessage, g_Message[ETC_MESSAGE1061].szMessage, 0xfe); 
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
	}
	else
	{
		// "이벤트 던젼에 진입하였습니다."
		__lstrcpyn(szMessage, g_Message[ETC_MESSAGE554].szMessage, 0xfe);
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;

		// "모든 몬스터를 제거하고 살아남을 경우,"
		__lstrcpyn(szMessage, g_Message[ETC_MESSAGE555].szMessage, 0xfe); 
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;		

		// "특별한 선물이 주어집니다."
		__lstrcpyn(szMessage, g_Message[ETC_MESSAGE556].szMessage, 0xfe); 
		RenderFont(szMessage, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
	}	
}


void CEventDungeonWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		m_bSoundChk = FALSE;
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{	
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_EVENTDUNGEON_WND1, TRUE);
		SetRender(SPR_OBJ_EVENTDUNGEON_WND2, TRUE);
		SetRender(SPR_OBJ_EVENTDUNGEON_WND3, TRUE);
		
		SetRender(SPR_OBJ_EVENT_CLOSE1, TRUE);
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CEventDungeonWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtnChk==TRUE)
	{
		m_bBtnChk = FALSE;
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
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				m_bBtnChk = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				m_bBtnChk = TRUE;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	}	
	
	MouseUp();

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))		
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		
	
	return 0;
}
