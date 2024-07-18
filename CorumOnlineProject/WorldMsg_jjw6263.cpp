#include "WorldMsg_jjw6263.h"
#include "message.h"
#include "Define.h"
#include "DUNGEON_DATA_EX.h"
#include "Chat.h"
#include "InitGame.h"
#include "DungeonTable.h"
#include "GameControl.h"
#include "WorldProcess.h"


BOOL g_bReceiveEventInfo	= FALSE;


void CmdEvnetDungeonEnterFailed(char* pMsg, DWORD dwLen)
{
	WSTC_EVENT_DUNGEON_ENTER_FAILED* pPacket = (WSTC_EVENT_DUNGEON_ENTER_FAILED*)pMsg;

	switch (pPacket->byErrorCode)
	{
	case 0:		
		//"해당 던젼에 입장이 가능한 레벨이 아닙니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE677].szMessage, 0xffffffff);
		break;
	case 1:
		//"파티의 리더만이 입장할 수 있는 던젼입니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE678].szMessage, 0xffffffff);
		break;
	default:
		break;
	};
}


void CmdEventStateChange(char* pMsg, DWORD dwLen)
{	
/*	WSTC_EVENT_DUNGEON_STATE_INFO* pPacket = (WSTC_EVENT_DUNGEON_STATE_INFO*)pMsg;
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(NULL == pDungeon)									return;
	if(DUNGEON_TYPE_EVENT != pDungeon->GetDungeonType())	return;
	
	pDungeon->m_bEventState	= (BYTE)pPacket->byEventState;

	if(EVENT_DUNGEON_READY == pDungeon->m_bEventState)
	{
		pDungeon->m_dwEndOpenReadyTime = pDungeon->m_dwEndOpenReadyTime + timeGetTime();
	}
	
	if(EVENT_DUNGEON_OPEN == pDungeon->m_bEventState)
	{
		pDungeon->m_dwEventStartTick = timeGetTime();
	}

	if(UPDATE_GAME_WORLD == GetGameStatus())
	{
		ShowEventDungeonModel(pDungeon);
	}	*/
}


void CmdEventDungeonInfo(char* pMsg, DWORD dwLen)
{	
	WSTC_EVENT_DUNGEON_INFO* pPacket = (WSTC_EVENT_DUNGEON_INFO*)pMsg;	

	DUNGEON_DATA_EX* pCheck = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(NULL != pCheck)		return;
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->AllocDungeonInfo();
	if(NULL == pDungeon)	return;
	
	pDungeon->m_dwID					= pPacket->wDungeonID;
	pDungeon->m_bEventState				= (BYTE)pPacket->byEventState;			
	pDungeon->m_byEventType				= pPacket->byEventType;
	pDungeon->m_wWorldMapID				= pPacket->byWorldMapID;
	pDungeon->m_dwEventOpenReadyTime	= pPacket->dwReadyOpenTime;
	pDungeon->m_byEventEnterMinLevel	= pPacket->byEnterMinLevel;
	pDungeon->m_byEventEnterMaxLevel	= pPacket->byEnterMaxLevel;
	pDungeon->m_dwEventEndTick			= pPacket->dwEventEndTick;

	if(EVENT_DUNGEON_READY == pDungeon->m_bEventState)
	{
		pDungeon->m_dwEndOpenReadyTime = pDungeon->m_dwEventOpenReadyTime + timeGetTime();		
	}

	memset(pDungeon->m_szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
	__lstrcpyn(pDungeon->m_szDungeonName, pPacket->szDungeonName, MAX_DUNGEON_NAME_LENGTH);

	g_pDungeonTable->AddDungeonInfo(pDungeon);

	if(EVENT_DUNGEON_OPEN == pDungeon->m_bEventState)
	{
		pDungeon->m_dwEventStartTick = timeGetTime();
	}
	
	ShowEventDungeonModel(pDungeon);
}