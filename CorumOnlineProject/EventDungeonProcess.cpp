#include "EventDungeonProcess.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "message.h"
#include "CodeFun.h"
#include "CorumResource.h"
#include "DungeonMsg.h"
#include "EffectProcess.h"
#include "DungeonTable.h"
#include "DungeonSiegeInfoWnd.h"


INT			g_iCurrentLayerUserNo	 = 0;
INT			g_iNeedUserNo			 = 0;
INT			g_iNeedKillMonsterNo	 = 0;
INT			g_iCurrentKillMonsterNo  = 0;
INT			g_iTotalUserCount		 = 0;
INT			g_iNowEventDungeonKind	 = 0;
DWORD		g_dwChangeLayerRemainTime= 0;
BOOL		g_bIsLastLayer			 = FALSE;


//----------------------------------------------------------------------------
// Name : InitEventRelationVariables
// Description :
//----------------------------------------------------------------------------
void InitEventRelationVariables(void)
{
	g_iCurrentLayerUserNo = 0;
	g_iNeedUserNo = 0;
	g_iNeedKillMonsterNo = 0;
	g_iCurrentKillMonsterNo = 0;
	g_iTotalUserCount = 0;
	g_iNowEventDungeonKind = 0; 
	g_dwChangeLayerRemainTime = 0;
	g_bIsLastLayer = FALSE;
}


//----------------------------------------------------------------------------
// Name : ClearEventDungeonObjects
// Description :
//----------------------------------------------------------------------------
void ClearEventDungeonObjects(void)
{
/*	if (NULL != g_pMainPlayer->m_hEventClearObject)
	{
		DeleteHandleObject(g_pMainPlayer->m_hEventClearObject);
		g_pMainPlayer->m_hEventClearObject = NULL;
	}*/
}

//----------------------------------------------------------------------------
// Name : EventDungeonRender
// Description :
//----------------------------------------------------------------------------
void EventDungeonRender(void)
{/*
	//if (FALSE == g_pThisDungeon->IsEvent())
	if (DUNGEON_TYPE_EVENT != g_pThisDungeon->GetDungeonType())
	{
		return;
	}
	
	CDungeonSiegeInfoWnd* pEventDungeonInfoWnd = CDungeonSiegeInfoWnd::GetInstance();

	// 수련던젼 
	if(EVENT_DUNGEON_TYPE_1 == g_pThisDungeon->m_byEventType)
	{
		DWORD dwCurrentTick = timeGetTime();	
		
//		int iHour = g_pThisDungeon->GetRemainTime(dwCurrentTick, g_pThisDungeon->m_dwEventStartTick, TIME_HOUR);
//		int iMin  = g_pThisDungeon->GetRemainTime(dwCurrentTick, g_pThisDungeon->m_dwEventStartTick, TIME_MIN);
//		int iSec  = g_pThisDungeon->GetRemainTime(dwCurrentTick, g_pThisDungeon->m_dwEventStartTick, TIME_SEC);

		int iHour = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventStartTick, dwCurrentTick, TIME_HOUR);
		int iMin  = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventStartTick, dwCurrentTick, TIME_MIN);
		int iSec  = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventStartTick, dwCurrentTick, TIME_SEC);
		
		pEventDungeonInfoWnd->m_iHour	= iHour;
		pEventDungeonInfoWnd->m_iMin	= iMin;
		pEventDungeonInfoWnd->m_iSec	= iSec;	
		pEventDungeonInfoWnd->m_iType	= SIEGEINFOWND_TYPE_EVENT_TRAINING;
	}	
	// 마그슬레엄의 유물
	else if (EVENT_DUNGEON_TYPE_3 == g_pThisDungeon->m_byEventType)	
	{
		pEventDungeonInfoWnd->m_iNeedKillMonsterNumber		= g_iNeedKillMonsterNo;
		pEventDungeonInfoWnd->m_iCurrentKillMonsterCount	= g_iCurrentKillMonsterNo;
		pEventDungeonInfoWnd->m_iDungeonTotalUserNumber		= g_iTotalUserCount;
		pEventDungeonInfoWnd->m_iLayerTotalUserNumber		= g_iCurrentLayerUserNo;
		pEventDungeonInfoWnd->m_iNeedUserNumber				= g_iNeedUserNo;
		
		pEventDungeonInfoWnd->m_iType = SIEGEINFOWND_TYPE_EVENT_BATTLE;		
	}
	// 칼마르빈의 광산
	else if (EVENT_DUNGEON_TYPE_4 == g_pThisDungeon->m_byEventType)
	{
		pEventDungeonInfoWnd->m_iNeedKillMonsterNumber		= g_iNeedKillMonsterNo;
		pEventDungeonInfoWnd->m_iCurrentKillMonsterCount	= g_iCurrentKillMonsterNo;
		pEventDungeonInfoWnd->m_iDungeonTotalUserNumber		= g_iTotalUserCount;
		pEventDungeonInfoWnd->m_iLayerTotalUserNumber		= g_iCurrentLayerUserNo;
		pEventDungeonInfoWnd->m_iNeedUserNumber				= g_iNeedUserNo;
		
		pEventDungeonInfoWnd->m_iType = SIEGEINFOWND_TYPE_EVENT_KILLMON;
	}
	// Normal style.
	else 
	{
		DWORD dwCurrentTick = timeGetTime();	
		
//		int iHour = g_pThisDungeon->GetRemainTime(dwCurrentTick, g_pThisDungeon->m_dwEventStartTick, TIME_HOUR);
//		int iMin  = g_pThisDungeon->GetRemainTime(dwCurrentTick, g_pThisDungeon->m_dwEventStartTick, TIME_MIN);
//		int iSec  = g_pThisDungeon->GetRemainTime(dwCurrentTick, g_pThisDungeon->m_dwEventStartTick, TIME_SEC);

		int iHour = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventStartTick, dwCurrentTick, TIME_HOUR);
		int iMin  = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventStartTick, dwCurrentTick, TIME_MIN);
		int iSec  = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventStartTick, dwCurrentTick, TIME_SEC);
		
		pEventDungeonInfoWnd->m_iHour	= iHour;
		pEventDungeonInfoWnd->m_iMin	= iMin;
		pEventDungeonInfoWnd->m_iSec	= iSec;		
		pEventDungeonInfoWnd->m_iType	= SIEGEINFOWND_TYPE_EVENT_NORMAL;		
	}
	
	pEventDungeonInfoWnd->SetActive();*/
}