#include "stdafx.h"
#include "DungeonSiegeInfoWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "User.h"
#include "DSMiniMap.h"
#include "dungeontable.h"
#include "message.h"
#include "HashTable.h"
#include "DungeonProcess.h"
#include "Map.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "Interface.h"


CDungeonSiegeInfoWnd* CDungeonSiegeInfoWnd::c_pThis = NULL;


CDungeonSiegeInfoWnd::CDungeonSiegeInfoWnd()
{
	m_iTotalMonsterCount		= 0;
	m_iCurrenMonsterCount		= 0;
	m_iHour						= 0;
	m_iMin						= 0;
	m_iSec						= 0;
	m_dwCurrentMonterLayerCount = 0;
	m_dwKillMonsterLayerCount	= 0;
	m_iType						= SIEGEINFOWND_TYPE_SIEGE;	
}


CDungeonSiegeInfoWnd::~CDungeonSiegeInfoWnd()
{

}


BOOL CDungeonSiegeInfoWnd::Init()
{
/*
	InsertData(SPR_OBJ_DUNGEONSIEGEINFOWND1, SPR_INTERFACE_DF7, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_DUNGEONSIEGEINFOWND2, SPR_INTERFACE_DF8, 0, 32, 1.0f, 11.0f, 0);
	InsertData(SPR_OBJ_DUNGEONSIEGEINFOWND3, SPR_INTERFACE_DF9, 0, 120, 1.0f, 1.0f, 0);	

	InsertCheckInterface(0, 0, 242, 20, 2, CHECK_MOVEWND);	
		*/
	m_bInit = TRUE;

	return TRUE;
}


void CDungeonSiegeInfoWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}


void CDungeonSiegeInfoWnd::SetOrder()
{

}


void CDungeonSiegeInfoWnd::RenderText()
{	
	if(SIEGEINFOWND_TYPE_SIEGE == m_iType)
	{
		char	szInfo[0xff]	= {0,};		
		int		nPosX			= int(m_fPosX+10);
		int		nPosY			= int(m_fPosZ+10+32);
		
		// MSG_ID : 105 ; CP 정보: %d / %d
		wsprintf(szInfo, g_Message[ETC_MESSAGE105].szMessage, g_pMap->m_bCPRemainCount, g_pMap->m_pCPList->GetCount());	
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
		
		nPosY += 17;
					
		int nHour	= g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_HOUR);
		int nMinute = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_MIN);
		int nSec	= g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_SEC);

		// MSG_ID : 106 ; 공성 끝나는 시간: %02d:%02d:%02d		
		wsprintf(szInfo, g_Message[ETC_MESSAGE106].szMessage, nHour, nMinute, nSec);	
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
		
		nPosY += 17;

		// 유저의 공격 모드를 알려준다.		
		if(g_pMainPlayer->m_bAttackMode == ATTACK_MODE_DEFENSE)
		{
			// "전투 모드 : 방어자 입니다."
			lstrcpy(szInfo, g_Message[ETC_MESSAGE543].szMessage); 
		}
		else if(g_pMainPlayer->m_bAttackMode == ATTACK_MODE_OFFENSE)
		{
			// "전투 모드 : 공격자 입니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE544].szMessage, lstrlen(g_Message[ETC_MESSAGE544].szMessage)); 
		}
		else
		{
			// "전투 모드 : 보통 입니다."
			lstrcpy(szInfo, g_Message[ETC_MESSAGE545].szMessage); 
		}

		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		if(g_pMainPlayer->m_bCurLayer+1 < g_pThisDungeon->m_wLayerCount)
		{
			nPosY += 17;
											
			// "현재 층 몬스터 : %4d /%4d"
			wsprintf(szInfo, g_Message[ETC_MESSAGE546].szMessage, m_dwKillMonsterLayerCount, m_dwCurrentMonterLayerCount); 
			RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
		}
	}
	else if(SIEGEINFOWND_TYPE_EVENT_SURVIVAL == m_iType)
	{
		char	szInfo[0xff]	= {0,};		
		int		nPosX			= int(m_fPosX+10);
		int		nPosY			= int(m_fPosZ+10+32);		

		// "던전 전체 인원 : %02d"
		wsprintf(szInfo, g_Message[ETC_MESSAGE781].szMessage, m_dwCurrentMonterLayerCount); 
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;		

		// "죽인 몬스터 수 : %d"
		wsprintf(szInfo, g_Message[ETC_MESSAGE1213].szMessage, m_dwKillMonsterLayerCount); 
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
		
		nPosY += 17;

		int nHour	= g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventEndTick, g_dwCurTick, TIME_HOUR);
		int nMinute = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventEndTick, g_dwCurTick, TIME_MIN);
		int nSec	= g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventEndTick, g_dwCurTick, TIME_SEC);

		// "이벤트 던젼 끝나는 시간 : %02d:%02d:%02d"
		wsprintf(szInfo,g_Message[ETC_MESSAGE1120].szMessage,nHour, nMinute, nSec); 
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);		
	}	
	else if(SIEGEINFOWND_TYPE_EVENT_TRAINING == m_iType)
	{		
		char	szInfo[0xff] = {0,};		
		int		nPosX = int(m_fPosX+10);
		int		nPosY = int(m_fPosZ+10+32);		
				
		// "목표 몬스터 Kill 수 : %02d"
		wsprintf(szInfo, g_Message[ETC_MESSAGE784].szMessage, m_dwTotalKillMonSterLayerCount); 
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
		
		nPosY += 17;
		
		// "현재 몬스터 Kill 수 : %02d"
		wsprintf(szInfo,g_Message[ETC_MESSAGE785].szMessage,m_dwCurrentKillMonsterLayerCount);  
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
		
		nPosY += 17;

		// "현재 같은층 파티원 몬스터 Kill 수 : %02d"
		wsprintf(szInfo,g_Message[ETC_MESSAGE1248].szMessage,m_dwPartyCurrentKillMonsterLayerCount);  
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
				
		nPosY += 17;

		int nHour	= g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventEndTick, g_dwCurTick, TIME_HOUR);
		int nMinute = g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventEndTick, g_dwCurTick, TIME_MIN);
		int nSec	= g_pThisDungeon->GetRemainTime(g_pThisDungeon->m_dwEventEndTick, g_dwCurTick, TIME_SEC);

		// "이벤트 던젼 끝나는 시간 : %02d:%02d:%02d"
		wsprintf(szInfo,g_Message[ETC_MESSAGE1120].szMessage,nHour, nMinute, nSec); 
		RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);

		nPosY += 17;

		if(m_bPartySameDungeonLayerUserCount)
		{
			// "수련던젼 같은 층 파티원수 : %d"
			wsprintf(szInfo,g_Message[ETC_MESSAGE1249].szMessage,m_bPartySameDungeonLayerUserCount); 
			RenderFont(szInfo, nPosX, nPosX+240, nPosY, nPosY+15, GetStartOrder()+1);
		}		
	}
}


void CDungeonSiegeInfoWnd::SetActive(BOOL bActive)
{
	if(m_bActive==bActive)
		return;
	
	m_bActive = bActive;

	if(bActive == FALSE)
	{
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_DUNGEONSIEGEINFOWND1, TRUE);
		SetRender(SPR_OBJ_DUNGEONSIEGEINFOWND2, TRUE);
		SetRender(SPR_OBJ_DUNGEONSIEGEINFOWND3, TRUE);	
		CMiniMapWnd::GetInstance()->SetOrder();		
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}	
}


int CDungeonSiegeInfoWnd::CheckInterface()
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


void CDungeonSiegeInfoWnd::OnMove()
{
	
}
