#include "DungeonMsg_jjw6263.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "message.h"
#include "CodeFun.h"
#include "CorumResource.h"
#include "DungeonMsg.h"
#include "EventDungeonWnd.h"
#include "ItemMakingWnd.h"
#include "ItemMakingSubWnd.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "DungeonSiegeInfoWnd.h"
#include "EffectProcess.h"
#include "DungeonTable.h"
#include "EventNPCSubWnd.h"
#include "EventDungeonProcess.h"
#include "ItemMoveManager.h"
#include "QuantityWnd.h"
#include "ItemNative.h"


void InitPacketProcDungeonJJW6263()
{
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMUPGRADE_SUCCESS]					= CmdUpgradeItemSuccess;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMUPGRADE_FAIL]						= CmdUpgradeItemFail;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMUPGRADE_FAIL_INSURANCE]				= CmdUpgradeItemFailInsurance; 				
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_UPGRADE_CANCEL]							= CmdUpgradeCancel;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_UPGRADE_CANCEL_RETURN_INV]				= CmdUpgradeItem_ReturnInv;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMMAKING_SUCCESS]						= CmdMakingItemSuccess;				
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMMAKING_FAIL]						= CmdMakingItemFail;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEMMAKING_CANCEL]						= CmdMakingItemCancel;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_MAKING_CANCEL_RETURN_INV]				= CmdMakingItem_ReturnInv;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUARDIANSUPPLIES_CANCAL_RETURN_INV]		= CmdGuardianSupplies_ReturnInv;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLEAR]					= CmdEventDungeonClear;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLEAR_PRESENT]			= CmdEventDungeonClearPresent;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_LAST_LAYER]				= CmdEventDungeonLastLayer;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_FAILED]					= CmdEventDungeonFailed;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_CHANGE_LAYER_RESULT]				= CmdEnentDungeonChangeLayer;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_ALIVE_MONSTER_COUNT]				= CmdEventAliveMonsterCount;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_EXCEPTION_RESULT]					= CmdEventExceptionResult;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_ENTER]					= CmdEventDungeonEnter;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_OPEN_INFO]				= CmdEventDungeonOpenInfo;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_CLEAR_CONDITION]					= CmdEventClearCondition;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_ALIVE_USER_COUNT]					= CmdEventAliveUserCount;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_LAYER_CLEAR]				= CmdEventDungeonLayerClear;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLOSE]					= CmdEventClose;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_FIRST_TIMEOUT]			= CmdEventDungeonFirstTimeOut;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_TRAINING_DISPLAY]			= CmdEventDungeonTraninigDisPlay;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER]	= CmdEventDungeonTraninigOneLayerClayer;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_SURVIVAL_DISPLAY]			= CmdEventDungeonSurvivalDisPlay;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLOSE_DISPLAY]			= CmdEventDungeonCloseDisPlay;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_JOINUSER_COUNT]			= CmdEventDungeonJoinUserCount;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_EVENT_DUNGEON_CLEARUSER_SEND]			= CmdEventDungeonClearUserSend;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEM_MOVE]								= CmdItemMove;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEM_USED_NATIVE]						= CmdItemUsedNative;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_SWITCH_WEAPON]							= CmdSwitchWeapon;

#ifdef DEVELOP_MODE	
	if( IsUltra() )
	{
		PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ITEM_PICKUP]						= NULL;
		g_ItemMoveManager.SetNewItemMoveMode(TRUE);
	}		
#endif
}


BOOL IsUltra()
{
	WORD	wVersionRequested;
	WSADATA wsaData;
	int		err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
	{
		return FALSE;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 )
	{
		WSACleanup();
		return FALSE; 
	}

	char name[255] = {0,};
	char szip[16] = {0,};
	struct hostent* hostinfo;

	if( gethostname ( name, sizeof(name)) == 0 )
	{
		if((hostinfo = gethostbyname(name)) != NULL)
		{
			for(int i=0; hostinfo->h_addr_list[i] != NULL; i++)
			{
				strcpy(szip, inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[i]));             
			}
		}
	}

	if( !strcmp(szip,"222.106.120.35")	||
		!strcmp(szip,"222.106.120.16")	||
		!strcmp(szip,"222.106.120.13")	||
		!strcmp(szip,"222.106.120.12"))
	{
		WSACleanup();
//		return TRUE;
		return FALSE;
	}

	WSACleanup();

	return FALSE;
}


//----------------------------------------------------------------------------
// 아이템 이동 
//----------------------------------------------------------------------------
void CmdItemMove( char* pMsg, DWORD dwLen )
{
	g_ItemMoveManager.Process(pMsg,dwLen);
	g_ItemMoveManager.Initialize();

	CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
	pQuantityWnd->m_bReq = FALSE;
}


//----------------------------------------------------------------------------
// 아이템 사용 
//----------------------------------------------------------------------------
void CmdItemUsedNative( char* pMsg, DWORD dwLen )
{
	g_ItemUsedManager.Process(pMsg,dwLen);
}


//----------------------------------------------------------------------------
// SwitchWeapon : 'W'키 
//----------------------------------------------------------------------------
void CmdSwitchWeapon( char* pMsg, DWORD dwLen )
{
	DSTC_SWITCH_WEAPON* pPacket = (DSTC_SWITCH_WEAPON*)pMsg;
	g_pMainPlayer->m_bCurrnetHand = pPacket->bCurrnetHand;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CmdEnentDungeonChangeLayer( char* pMsg, DWORD dwLen )
{
	DSTC_EVENT_CHANGE_LAYER_RESULT* pPacket = (DSTC_EVENT_CHANGE_LAYER_RESULT*)pMsg;
	
	if (1 == pPacket->bMoveResult)
	{
		// "아직 살아있는 몬스터가 있습니다! 아래층으로 내려가실 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE521].szMessage, TEXT_COLOR_YELLOW); 
	}
	if (2 == pPacket->bMoveResult)
	{
		// "마법의 기운이 아직 강합니다. 마물들을 더 제거해야 합니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE755].szMessage, TEXT_COLOR_YELLOW); 
	}
	if (3 == pPacket->bMoveResult)
	{
		// "마법의 기운이 아직 강합니다. 희생자가 더 필요합니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE756].szMessage, TEXT_COLOR_YELLOW); 
	}
}


//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
void CmdEventAliveMonsterCount(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_ALIVE_MONSTER_COUNT* pPacket = (DSTC_EVENT_ALIVE_MONSTER_COUNT*)pMsg;	
	CDungeonSiegeInfoWnd* pDungeonSiegeInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	pDungeonSiegeInfoWnd->m_iTotalMonsterCount = (INT)pPacket->wAliveMonsterCount;	
	
	if(NULL != g_pThisDungeon)
	{	
		if(	DUNGEON_TYPE_EVENT == g_pThisDungeon->GetDungeonType() &&
			FALSE == g_bIsLastLayer &&
			g_pThisDungeon->m_byEventType == EVENT_DUNGEON_TRAINING )
		{		
			DisplayMessageAdd("마법의 기운이 약해졌습니다.아랫층으로 이동할수 있습니다.", TEXT_COLOR_YELLOW);		
		}
	}
	
	if (TRUE == g_bIsLastLayer)
	{
		g_bIsLastLayer = FALSE;
	}	
}


//----------------------------------------------------------------------------
// 이벤트 던젼 클리어
//----------------------------------------------------------------------------
void CmdEventDungeonClear(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_CLEAR*	pPacket = (DSTC_EVENT_DUNGEON_CLEAR*)pMsg;
	CUser*						pUser	= g_pUserHash->GetData( pPacket->dwUserIndex );

	EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(FILE(EFFECT_DUNGEONDEFENSESUCCESS_WORLD)
		, 1, __CHR_EFFECT_NONE__);

	pEffectDesc->dwTargetIndex[0] = pPacket->dwUserIndex;
	::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
	GXSetPosition(pEffectDesc->hEffect.pHandle,&pUser->m_v3CurPos,FALSE);
	pEffectDesc->hEffect.pDesc->ObjectFunc = EffectEventDungeonFunc;
	
	// "이벤트 던젼 클리어!!"
	DisplayMessageAdd(g_Message[ETC_MESSAGE523].szMessage, TEXT_COLOR_YELLOW);		
	InitEventRelationVariables();
}


void CmdEventDungeonClearPresent(char* pMsg, DWORD dwLen)
{
	
}


//----------------------------------------------------------------------------
// 수련던전의 마지막 층입니다.
//----------------------------------------------------------------------------
void CmdEventDungeonLastLayer(char* pMsg, DWORD dwLen)
{
	g_bIsLastLayer = TRUE;	
	DisplayMessageAdd(g_Message[ETC_MESSAGE1113].szMessage,TEXT_COLOR_YELLOW);
}


//----------------------------------------------------------------------------
// 이벤트 던젼 실패 메시지 출력.
//----------------------------------------------------------------------------
void CmdEventDungeonFailed(char* pMsg, DWORD dwLen)
{
	CEventDungeonWnd* pEventDungeonWnd = CEventDungeonWnd::GetInstance();
	
	pEventDungeonWnd->m_iMessageType = EVENT_FAILED;
	pEventDungeonWnd->SetActive();
	
	InitEventRelationVariables();
}


//----------------------------------------------------------------------------
// 
//----------------------------------------------------------------------------
void CmdEventExceptionResult(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_EXCEPTION_RESULT* pPacket = (DSTC_EVENT_EXCEPTION_RESULT*)pMsg;
	
	switch (pPacket->byExceptionCode)
	{
	case 0:			// 포탈 사용은 금지되어 있다.

		// "이벤트 던젼에서는 포탈기능을 이용할 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE524].szMessage, TEXT_COLOR_YELLOW);
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);		
		
		break;
		
	case 1:			// 조디악 카드 사용은 금지되어 있다.

		// "이벤트 던젼에서는 조디악 카드를 사용할 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE525].szMessage, TEXT_COLOR_YELLOW);
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		break;
		
	default:
		break;
	}
}


//----------------------------------------------------------------------------
// 생존던전 1차 이벤트 종료 되었다.
//----------------------------------------------------------------------------
void CmdEventDungeonFirstTimeOut(char* pMsg, DWORD dwLen)
{
	// 생존던전 1차 이벤트 종료 메세지 출력 
	DSTC_EVENT_DUNGEON_FIRST_TIMEOUT* pPacket = (DSTC_EVENT_DUNGEON_FIRST_TIMEOUT*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;	

	CEventDungeonWnd* pEventDungeonWnd = CEventDungeonWnd::GetInstance();
	pEventDungeonWnd->m_iMessageType = EVENT_DUNGEON_SURVIVAL_FIRST_TIMEOUT;
	pEventDungeonWnd->m_bDungeonMonsterExpRatio = pPacket->bMonsterExpRatio;
	pEventDungeonWnd->SetActive();

	g_pMainPlayer->m_bInEvnetDungeonFirstTimeOut = TRUE;
	
	ListNode<CMonster>*	pMonsterNode	= g_pMonsterHash->GetHead();
	CMonster*			pMonster		= NULL;

	while( pMonsterNode )
	{
		pMonster = pMonsterNode->pData;

		if(pMonster)
		{
			if(pMonster->IsNormalMonster())
			{
				pMonster->SetAction( MON_MOTION_TYPE_STAND1, 0, ACTION_ONCE );
				pMonster->m_hMonster.pDesc->ObjectFunc		= MonsterKillFunc;
				pMonster->m_dwTemp[MONSTER_TEMP_PUSHING]	= 1;
			}
		}
		
		pMonsterNode = pMonsterNode->pNext;
	}
}


//----------------------------------------------------------------------------
// 이벤트 던젼(수련던전)에서 한층에서 잡아야 할 몹수와 잡은 몹수를 보내준다.   
//----------------------------------------------------------------------------
void CmdEventDungeonTraninigDisPlay(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_TRAINING_DISPLAY* pPacket = (DSTC_EVENT_DUNGEON_TRAINING_DISPLAY*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;

	CDungeonSiegeInfoWnd* pDungeonSiegeInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	pDungeonSiegeInfoWnd->m_dwTotalKillMonSterLayerCount	= pPacket->wTotalKillMonSterLayerCount;
	pDungeonSiegeInfoWnd->m_dwCurrentKillMonsterLayerCount	= pPacket->wCurrentKillMonsterLayerCount;
	pDungeonSiegeInfoWnd->m_dwPartyCurrentKillMonsterLayerCount = pPacket->wPartyCurrentKillMonsterLayerCount;
	pDungeonSiegeInfoWnd->m_bPartySameDungeonLayerUserCount = pPacket->bPartySameDungeonLayerUserCount;
	pDungeonSiegeInfoWnd->m_iType							= SIEGEINFOWND_TYPE_EVENT_TRAINING;
}


//----------------------------------------------------------------------------
// 이벤트 던젼(생존던전)에서 한층에서 잡은몹수와 총 유저수를 보내준다.   
//----------------------------------------------------------------------------
void CmdEventDungeonSurvivalDisPlay(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY* pPacket = (DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY*)pMsg;
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;
	
	CDungeonSiegeInfoWnd* pDungeonSiegeInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	pDungeonSiegeInfoWnd->m_dwCurrentMonterLayerCount	= pPacket->wCurrentMonterLayerCount;	/*던젼입장객수*/
	pDungeonSiegeInfoWnd->m_dwKillMonsterLayerCount		= pPacket->wKillMonsterLayerCount; 
	pDungeonSiegeInfoWnd->m_iType						= pPacket->byDisplayMode;
}


//----------------------------------------------------------------------------
// 이벤트 던젼이 종료 될꺼라는것을 알려준다.
//----------------------------------------------------------------------------
void CmdEventDungeonCloseDisPlay(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_CLOSE_DISPLAY* pPacket = (DSTC_EVENT_DUNGEON_CLOSE_DISPLAY*)pMsg;
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;	

	// "잠시후 이벤트 던젼 종료합니다."
	DisplayMessageAdd(g_Message[ETC_MESSAGE1117].szMessage,TEXT_COLOR_YELLOW);
}


//----------------------------------------------------------------------------
// 이벤트 던젼에 현재 입장객을 디스플레이 해준다.
//----------------------------------------------------------------------------
void CmdEventDungeonJoinUserCount(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_JOINUSER_COUNT* pPacket = (DSTC_EVENT_DUNGEON_JOINUSER_COUNT*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;	

	CDungeonSiegeInfoWnd* pDungeonSiegeInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	pDungeonSiegeInfoWnd->m_dwCurrentMonterLayerCount = pPacket->wJoinUserCount;	/*던젼입장객수*/
}


//----------------------------------------------------------------------------
// 이벤트 던젼에서 클리어한 유저를 알려준다.
//----------------------------------------------------------------------------
void CmdEventDungeonClearUserSend(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_CLEARUSER_SEND* pPacket = (DSTC_EVENT_DUNGEON_CLEARUSER_SEND*)pMsg;
	
	// 스트링만 출력해줌.
	char szOutMessage[0xff];
	memset(szOutMessage, 0, 0xff);	

	// "[%s]님이 이벤트 던젼 클리어 했습니다."
	wsprintf(szOutMessage, g_Message[ETC_MESSAGE1210].szMessage,pPacket->szCharName); 
	DisplayMessageAdd(szOutMessage,TEXT_COLOR_YELLOW);
}


//----------------------------------------------------------------------------
// 이벤트 던젼(수련던전)에서 한층에서 잡아야 할 몹수를 채웠다.다음층으로 이동 가능하다.
//----------------------------------------------------------------------------
void CmdEventDungeonTraninigOneLayerClayer(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER* pPacket = (DSTC_EVENT_DUNGEON_TRAINING_ONELAYER_CLAYER*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)
	{
		return;
	}

	// "마법의 기운이 약해졌습니다.아랫층으로 이동할수 있습니다."
	DisplayMessageAdd(g_Message[ETC_MESSAGE811].szMessage, TEXT_COLOR_YELLOW);	
}


//----------------------------------------------------------------------------
// 이벤트 던젼에 입장!! 
//----------------------------------------------------------------------------
void CmdEventDungeonEnter(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_ENTER* pPacket = (DSTC_EVENT_DUNGEON_ENTER*)pMsg;
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;	

	pDungeon->m_byEventType = pPacket->byEventType;
	
	CEventDungeonWnd*		pEventDungeonWnd		= CEventDungeonWnd::GetInstance();
	CDungeonSiegeInfoWnd*	pDungeonSiegeInfoWnd	= CDungeonSiegeInfoWnd::GetInstance();
	
	pEventDungeonWnd->m_iMessageType = EVENT_ENTER;
	pEventDungeonWnd->SetActive();
	
	pDungeonSiegeInfoWnd->m_iType = pDungeon->m_byEventType;
	pDungeonSiegeInfoWnd->SetActive();

	// 이벤트 던전 입장 했을때 어느마을에서 포탈을 탔는지 던전서버에 알려준다. 
	CTDS_EVENT_USER_PREV_DUNGEON_ID SendPacket;
	SendPacket.wPrevDungeonID = g_pMainPlayer->wPrevDungeonID;
	g_pNet->SendMsg((char*)&SendPacket,SendPacket.GetPacketSize(),SERVER_INDEX_ZONE);	
}


//----------------------------------------------------------------------------
// 이벤트 던젼오픈. 이벤트 엔피씨창에 열린 던젼 리스트 보여줘 
//----------------------------------------------------------------------------
void CmdEventDungeonOpenInfo(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_OPEN_INFO*	pPacket		= (DSTC_EVENT_DUNGEON_OPEN_INFO*)pMsg;
	DUNGEON_DATA_EX*				pDungeon	= g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(NULL == pDungeon)	return;			
	
	pDungeon->m_bEventState = pPacket->byEventState;

	if(EVENT_DUNGEON_OPEN == pDungeon->m_bEventState)
	{
		pDungeon->m_dwEventEndTick = pPacket->dwEventEndTick + g_dwCurTick;//남은 시간 셋팅 : 050108 hwoarang
	}

	char szOutMessage[0xff] = {0,};
	wsprintf(szOutMessage,"[%s](Lv.%d ~ Lv.%d) 이벤트 던젼이 오픈되었습니다."
		, pPacket->szDungeonName, pDungeon->m_byEventEnterMinLevel, pDungeon->m_byEventEnterMaxLevel);
	DisplayMessageAdd(szOutMessage, TEXT_COLOR_YELLOW);
	
	CEventNPCSubWnd* pEventNPCSubWnd = CEventNPCSubWnd::GetInstance();
	pEventNPCSubWnd->QueryCanMoveDungeon();		
}


//----------------------------------------------------------------------------
// 이벤트 던젼 입구가 닫히거나 종료  
//----------------------------------------------------------------------------
void CmdEventClose(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_CLOSE*	pPacket		= (DSTC_EVENT_DUNGEON_CLOSE*)pMsg;
	DUNGEON_DATA_EX*			pDungeon	= g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if(NULL == pDungeon)		return;

	pDungeon->m_bEventState = (BYTE)pPacket->byEventState;

	if(EVENT_DUNGEON_OPEN_TIME_UP == pDungeon->m_bEventState)
	{
		char szOutMessage[0xff] = {0,};
		wsprintf(szOutMessage,"[%s](Lv.%d ~ Lv.%d) 이벤트 던젼 입구가 닫혔습니다."
			, pPacket->szDungeonName, pDungeon->m_byEventEnterMinLevel, pDungeon->m_byEventEnterMaxLevel);
		DisplayMessageAdd(szOutMessage, TEXT_COLOR_YELLOW);		
	}

	if(EVENT_DUNGEON_CLOSE == pDungeon->m_bEventState)
	{
		pDungeon->m_dwEventEndTick = 0;

		char szOutMessage[0xff] = {0,};
		wsprintf(szOutMessage,"[%s](Lv.%d ~ Lv.%d) 이벤트 던젼이 종료되었습니다."
			, pPacket->szDungeonName, pDungeon->m_byEventEnterMinLevel, pDungeon->m_byEventEnterMaxLevel);
		DisplayMessageAdd(szOutMessage, TEXT_COLOR_YELLOW);				
	}	
	
	CEventNPCSubWnd* pEventNPCSubWnd = CEventNPCSubWnd::GetInstance();
	pEventNPCSubWnd->QueryCanMoveDungeon();	
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CmdEventClearCondition(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_CLEAR_CONDITION*	pPacket	= (DSTC_EVENT_CLEAR_CONDITION*)pMsg;
	
	g_iNeedUserNo			= pPacket->wNeedUserNumber;
	g_iNeedKillMonsterNo	= pPacket->wNeedToKillMonsterNumber;
	
	CDungeonSiegeInfoWnd* pEventDungeonInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	
	pEventDungeonInfoWnd->m_iNeedKillMonsterNumber		= g_iNeedKillMonsterNo;	
	pEventDungeonInfoWnd->m_iNeedUserNumber				= g_iNeedUserNo;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CmdEventAliveUserCount(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_ALIVE_USER_COUNT* pPacket = (DSTC_EVENT_ALIVE_USER_COUNT*)pMsg;
	
	g_iCurrentLayerUserNo		= pPacket->wAliveUserCount;
	g_iTotalUserCount			= pPacket->wTotalUserCount;
	
	CDungeonSiegeInfoWnd* pEventDungeonInfoWnd = CDungeonSiegeInfoWnd::GetInstance();
	
	pEventDungeonInfoWnd->m_iLayerTotalUserNumber   = g_iCurrentLayerUserNo;
	pEventDungeonInfoWnd->m_iDungeonTotalUserNumber = g_iTotalUserCount;
}


//----------------------------------------------------------------------------
//
//----------------------------------------------------------------------------
void CmdEventDungeonLayerClear(char* pMsg, DWORD dwLen)
{
	DSTC_EVENT_DUNGEON_LAYER_CLEAR* pPacket = (DSTC_EVENT_DUNGEON_LAYER_CLEAR*)pMsg;
	
	if (0 == pPacket->byType)	// 배틀로열 방식이라면,
	{
		g_dwChangeLayerRemainTime = (DWORD)pPacket->wRemainTime;
	}	
	else
	{
		g_dwChangeLayerRemainTime = 0xffffffff; // 0xffffffff 값은 카운팅 하지 않음을 표시.
	}
}


// ===================
// 아이템 제조 취소 
// ===================
void CmdMakingItemCancel( char* pMsg, DWORD dwLen )
{
	DSTC_MAKING_CANCEL* pPacket = (DSTC_MAKING_CANCEL*)pMsg;

	for(int i = 1; i < 4; i++)
	{
		if(pPacket->pItem[i].GetID()!=0)
		{
			BYTE byCnt = Insert_SmallInvItem(&pPacket->pItem[i], 0, pPacket->pItem[i].GetQuantity());

			if(byCnt==0)
				memset(&pPacket->pItem[i], 0, sizeof(CItem));
			else
				pPacket->pItem[i].SetQuantity(byCnt);
		}
	}

	for(i = 1; i < 4; i++)
	{
		if(pPacket->pItem[i].GetID()!=0)
		{
			BYTE byCnt = Insert_SmallBankItem(&pPacket->pItem[i], 0, pPacket->pItem[i].GetQuantity());

			if(byCnt==0)
				memset(&pPacket->pItem[i], 0, sizeof(CItem));
			else
				pPacket->pItem[i].SetQuantity(byCnt);
		}
	}
		
	if(pPacket->bZipCode!=255)
	{
		if(pPacket->byType==0)
		{
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->pItem[0], sizeof(CItem));	
			memset(&pPacket->pItem[0], 0, sizeof(CItem));
		}
		else
		{
			memcpy(&g_pMainPlayer->m_pBank_Large[pPacket->bZipCode], &pPacket->pItem[0], sizeof(CItem));	
			memset(&pPacket->pItem[0], 0, sizeof(CItem));
		}
	}

	CItemMakingWnd* pItemMakingWnd = CItemMakingWnd::GetInstance();
	
	for(i = 0; i < 4; i++)
		memcpy(&pItemMakingWnd->m_pMakingItem[i], &pPacket->pItem[0], sizeof(CItem));
		
	pItemMakingWnd->m_byProcess = 0;
	
	if(pPacket->bZipCode==255)
	{
		// "아이템제조가 취소 되었습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1211].szMessage, 0xFFFF2CFF); 
	}
	else if(pPacket->byType==0)
	{
		// "인벤토리로 이동합니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1114].szMessage, 0xFFFF2CFF); 
	}
	else 
	{
		// "인벤토리에 여유공간이 부족하여 창고로 이동합니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1115].szMessage, 0xFFFF2CFF); 
	}
}


void CmdGuardianSupplies_ReturnInv( char* pMsg, DWORD dwLen )
{
	DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV* pPacket = (DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV*)pMsg;
	
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
	const CItem*		pItemNativeSrc	= NULL;
	const CItem*		pItemNativeDest = NULL;

	ItemNativeInfo_Src.eSlotID		= pPacket->ItemNodeNative.eSlotID;
	ItemNativeInfo_Src.bySlotIndex	= pPacket->ItemNodeNative.bySlotIndex;
	ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	pItemNativeSrc = GetItemNative(&ItemNativeInfo_Src);

	ItemNativeInfo_Dest.eSlotID		= ITEM_NATIVE_INV_SMALL;
	ItemNativeInfo_Dest.bySlotIndex	= 0;
	ItemNativeInfo_Dest.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	pItemNativeDest = GetItemNative(&ItemNativeInfo_Dest);

	for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		ItemNativeInfo_Dest.bySlotIndex = i;	

		if(pItemNativeDest[i].GetID() == 0)
		{
			CItem cItem;
			memcpy(&cItem, pItemNativeSrc, sizeof(CItem));

			SetSlotItemNative(&cItem, &ItemNativeInfo_Dest);
			RemoveItemNative(&ItemNativeInfo_Src);
			return;
		}
		else if(pItemNativeDest[i].GetID() == pItemNativeSrc->GetID())
		{
//			LP_ITEM_TABLE pBaseItemDest = g_pItemTableHash->GetData(pItemNativeDest[i].GetID());
			CBaseItem* pBaseItemDest = g_pItemTableHash->GetData(pItemNativeDest[i].GetID());

			BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
			BYTE byPutInCnt		= BYTE(byMaxQuantity - pItemNativeDest[i].GetQuantity());
			BYTE byCnt			= pItemNativeSrc->GetQuantity();

			if(byPutInCnt >= byCnt)
			{
				CItem cItemDest;
				memcpy(&cItemDest, &pItemNativeDest[i], sizeof(CItem));
				cItemDest.SetQuantity(BYTE(cItemDest.GetQuantity() + byCnt));

				SetSlotItemNative(&cItemDest, &ItemNativeInfo_Dest);
				RemoveItemNative(&ItemNativeInfo_Src);
			}
			else if(byPutInCnt != 0)
			{
				CItem cItemSrc;
				cItemSrc.SetQuantity(BYTE(cItemSrc.GetQuantity() - byPutInCnt));
				SetSlotItemNative(&cItemSrc, &ItemNativeInfo_Src);

				CItem cItemDest;
				memcpy(&cItemDest, &pItemNativeDest[i], sizeof(CItem));
				cItemDest.SetQuantity(BYTE(cItemDest.GetQuantity() + byPutInCnt));
				SetSlotItemNative(&cItemDest, &ItemNativeInfo_Dest);
			}
		}
	}
}


void CmdMakingItem_ReturnInv( char* pMsg, DWORD dwLen )
{
	DSTC_MAKING_CANCAL_RETURN_INV* pPacket = (DSTC_MAKING_CANCAL_RETURN_INV*)pMsg;

	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
//	CBaseItem*			pBaseItemSrc		= NULL;
//	CBaseItem*			pBaseItemDest		= NULL;
//	const CItem*		pItemSrc			= NULL;
//	const CItem*		pItemDest			= NULL;
	
	ItemNativeInfo_Src.eSlotID		= ITEM_NATIVE_MAKING;
	ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
	
	ItemNativeInfo_Dest.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		ItemNativeInfo_Src.bySlotIndex = pPacket->ItemNodeNative[i].index;				
		
		ItemNativeInfo_Dest.eSlotID		= pPacket->ItemNodeNative[i].eSlotID;
		ItemNativeInfo_Dest.bySlotIndex = pPacket->ItemNodeNative[i].bySlotIndex;
		
		AllLinkSrcToDestNative(&ItemNativeInfo_Src,&ItemNativeInfo_Dest);
	}	

	// "아이템제조가 취소 되었습니다."
	DisplayMessageAdd(g_Message[ETC_MESSAGE1211].szMessage, 0xFFFF2CFF);
}


// ===================
// 업그레이드 취소 
// ===================
void CmdUpgradeCancel( char* pMsg, DWORD dwLen )
{	
	DSTC_UPGRADE_FAIL* pPacket = (DSTC_UPGRADE_FAIL*)pMsg;

	for(int i = 1; i < 4; i++)
	{
		if(pPacket->pItem[i].GetID()!=0)
		{
			BYTE byCnt = Insert_SmallInvItem(&pPacket->pItem[i], 0, pPacket->pItem[i].GetQuantity());

			if(byCnt==0)
				memset(&pPacket->pItem[i], 0, sizeof(CItem));
			else
				pPacket->pItem[i].SetQuantity(byCnt);
		}
	}

	for(i = 1; i < 4; i++)
	{
		if(pPacket->pItem[i].GetID()!=0)
		{
			BYTE byCnt = Insert_SmallBankItem(&pPacket->pItem[i], 0, pPacket->pItem[i].GetQuantity());

			if(byCnt==0)
				memset(&pPacket->pItem[i], 0, sizeof(CItem));
			else
				pPacket->pItem[i].SetQuantity(byCnt);
		}
	}
		
	if(pPacket->bZipCode!=255)
	{
		if(pPacket->byType==0)
		{
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &pPacket->pItem[0], sizeof(CItem));	
			memset(&pPacket->pItem[0], 0, sizeof(CItem));
		}
		else
		{
			memcpy(&g_pMainPlayer->m_pBank_Large[pPacket->bZipCode], &pPacket->pItem[0], sizeof(CItem));	
			memset(&pPacket->pItem[0], 0, sizeof(CItem));
		}
	}

	CItemUpgradeWnd* pItemUpgradeWnd = CItemUpgradeWnd::GetInstance();
	
	for(i = 0; i < 4; i++)
		memcpy(&pItemUpgradeWnd->m_pUpgradeItem[i], &pPacket->pItem[0], sizeof(CItem));
		
	pItemUpgradeWnd->m_bWaitingUpgrade = 0;
	
	if(pPacket->bZipCode==255)
	{
		// "아이템강화가 취소 되었습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE588].szMessage, 0xFFFF2CFF); 
	}
	else if(pPacket->byType==0)
	{
		// "인벤토리로 이동합니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1114].szMessage, 0xFFFF2CFF); 
	}
	else 
	{
		// "인벤토리에 여유공간이 부족하여 창고로 이동합니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1115].szMessage, 0xFFFF2CFF); 
	}
}


void CmdUpgradeItem_ReturnInv( char* pMsg, DWORD dwLen )
{
	DSTC_UPGRADE_CANCAL_RETURN_INV* pPacket = (DSTC_UPGRADE_CANCAL_RETURN_INV*)pMsg;

	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
//	CBaseItem*			pBaseItemSrc		= NULL;
//	CBaseItem*			pBaseItemDest		= NULL;
//	const CItem*		pItemSrc			= NULL;
//	const CItem*		pItemDest			= NULL;
	
	ItemNativeInfo_Src.eSlotID		= ITEM_NATIVE_UPGRADE;
	ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
	
	ItemNativeInfo_Dest.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		ItemNativeInfo_Src.bySlotIndex = pPacket->ItemNodeNative[i].index;				
		
		ItemNativeInfo_Dest.eSlotID		= pPacket->ItemNodeNative[i].eSlotID;
		ItemNativeInfo_Dest.bySlotIndex = pPacket->ItemNodeNative[i].bySlotIndex;
		
		AllLinkSrcToDestNative(&ItemNativeInfo_Src,&ItemNativeInfo_Dest);
	}
	
	// "아이템강화가 취소 되었습니다."
	DisplayMessageAdd(g_Message[ETC_MESSAGE588].szMessage, 0xFFFF2CFF); 
}


// ===================
// 업그레이드 실패  
// ===================
void CmdUpgradeItemFail( char* pMsg, DWORD dwLen )
{
//	DSTC_ITEMUPGRADE_FAIL* pPacket		= (DSTC_ITEMUPGRADE_FAIL*)pMsg;
	CItemUpgradeWnd* pItemUpgradeWnd	= CItemUpgradeWnd::GetInstance();
	CItemUpgradeSubWnd* pItemUpgradeSubWnd	= CItemUpgradeSubWnd::GetInstance(); 

	pItemUpgradeWnd->m_bWaitingUpgrade	= ITEM_UPGRADE_FAIL;
	pItemUpgradeSubWnd->m_byItemUpgradeProcess = ITEM_UPGRADE_PROCESS_FAIL;

	memset(&pItemUpgradeWnd->m_pUpgradeItem[0],0,sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[1],0,sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[2],0,sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[3],0,sizeof(CItem));
}


// ===================
// 업그레이드 실패하였지만 보험 처리  
// ===================
void CmdUpgradeItemFailInsurance( char* pMsg, DWORD dwLen )
{
	DSTC_ITEMUPGRADE_FAIL_INSURANCE* pPacket	= (DSTC_ITEMUPGRADE_FAIL_INSURANCE*)pMsg;
	CItemUpgradeWnd*	pItemUpgradeWnd			= CItemUpgradeWnd::GetInstance();
	CItemUpgradeSubWnd* pItemUpgradeSubWnd		= CItemUpgradeSubWnd::GetInstance();

	pItemUpgradeWnd->m_bWaitingUpgrade			= ITEM_UPGRADE_FAIL_INSURANCE;
	pItemUpgradeSubWnd->m_byItemUpgradeProcess	= ITEM_UPGRADE_PROCESS_FAIL_INSURANCE;
	
	memcpy(&pItemUpgradeWnd->m_pUpgradeItem[0], &pPacket->cItem, sizeof(pPacket->cItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[1], 0, sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[2], 0, sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[3], 0, sizeof(CItem));
	memset(&g_pMainPlayer->m_pInv_Small[pPacket->bDelete], 0, sizeof(CItem));
}


// ===================
// 업그레이드 성공  
// ===================
void CmdUpgradeItemSuccess( char* pMsg, DWORD dwLen )
{
	DSTC_ITEMUPGRADE_SUCCESS* pPacket		= (DSTC_ITEMUPGRADE_SUCCESS*)pMsg;
	CItemUpgradeWnd* pItemUpgradeWnd		= CItemUpgradeWnd::GetInstance();
	CItemUpgradeSubWnd* pItemUpgradeSubWnd	= CItemUpgradeSubWnd::GetInstance(); 
	
	pItemUpgradeWnd->m_bWaitingUpgrade	= pPacket->bResult;
	pItemUpgradeSubWnd->m_byItemUpgradeProcess = ITEM_UPGRADE_PROCESS_SUCCESS;
	
	memcpy(&pItemUpgradeWnd->m_pUpgradeItem[0], &pPacket->cItem, sizeof(pPacket->cItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[1], 0, sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[2], 0, sizeof(CItem));
	memset(&pItemUpgradeWnd->m_pUpgradeItem[3], 0, sizeof(CItem));
}


// ===================
// 아이템 제조 성공 
// ===================
void CmdMakingItemSuccess( char* pMsg, DWORD dwLen )
{
	DSTC_ITEMMAKING_SUCCESS* pPacket		= (DSTC_ITEMMAKING_SUCCESS*)pMsg;
	CItemMakingWnd* pItemMakingWnd			= CItemMakingWnd::GetInstance();
	CItemMakingSubWnd* pItemMakingSubWnd	= CItemMakingSubWnd::GetInstance(); 

	pItemMakingWnd->m_byProcess = ITEM_MAKING_PROCESS_RESULT_SUCCESS;
	pItemMakingSubWnd->m_byItemMakingProcess = ITEM_MAKING_PROCESS_SUCCESS;

	memcpy(&pItemMakingWnd->m_pMakingItem[0], &pPacket->cItem, sizeof(pPacket->cItem));
	memset(&pItemMakingWnd->m_pMakingItem[1], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[2], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[3], 0, sizeof(CItem));
}


// ===================
// 아이템 제조 실패 
// ===================
void CmdMakingItemFail( char* pMsg, DWORD dwLen )
{
//	DSTC_ITEMMAKING_FAIL* pPacket			= (DSTC_ITEMMAKING_FAIL*)pMsg;
	CItemMakingWnd* pItemMakingWnd			= CItemMakingWnd::GetInstance();
	CItemMakingSubWnd* pItemMakingSubWnd	= CItemMakingSubWnd::GetInstance();

	pItemMakingWnd->m_byProcess = ITEM_MAKING_PROCESS_RESULT_FAIL;
	pItemMakingSubWnd->m_byItemMakingProcess = ITEM_MAKING_PROCESS_FAIL;

	memset(&pItemMakingWnd->m_pMakingItem[0], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[1], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[2], 0, sizeof(CItem));
	memset(&pItemMakingWnd->m_pMakingItem[3], 0, sizeof(CItem));
}