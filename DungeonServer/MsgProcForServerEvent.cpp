#include "stdafx.h"
#include "MsgProcForServerEvent.h"
#include "DungeonTable.h"
#include "Dungeon.h"
#include "OwnServer.h"
#include "DungeonLayer.h"
#include "DUNGEON_DATA_EX.h"
#include "ServerFunction.h"
#include "AcceptTable.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "recvmsg.h"
#include "DBProc.h"
#include "RecordSet.h"


//------------------------------------------------------------------------------
// 이벤트 던젼 얼렸다.
//------------------------------------------------------------------------------
void CmdEventDungeonOpenInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_OPEN_INFO* pPacket = (WSTDS_EVENT_DUNGEON_OPEN_INFO*)pMsg;	
	
	DSTC_EVENT_DUNGEON_OPEN_INFO SendPacket;
	
	memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
	__lstrcpyn(SendPacket.szDungeonName, pPacket->szDungeonName, MAX_DUNGEON_NAME_LENGTH);

	SendPacket.byEventState		= pPacket->byEventState; 
	SendPacket.wDungeonID		= pPacket->wDungeonID;
	SendPacket.dwEventEndTick	= pPacket->dwEventEndTick; 
	
	g_pDungeonTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());
	
	return;
}


//------------------------------------------------------------------------------
// 이벤트 던젼 닫혔다.
//------------------------------------------------------------------------------
void CmdEventDungeonCloseInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_CLOSE_INFO* pPacket = (WSTDS_EVENT_DUNGEON_CLOSE_INFO*)pMsg;

	DSTC_EVENT_DUNGEON_CLOSE SendPacket;
	memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
	__lstrcpyn(SendPacket.szDungeonName, pPacket->szDungeonName, MAX_DUNGEON_NAME_LENGTH);
	SendPacket.byEventState = pPacket->byEventState;
	SendPacket.wDungeonID	= pPacket->wDungeonID;
	g_pDungeonTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());
	
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(NULL == pDungeon)	return;

	if(pDungeon->GetEventDungeonProperty())
	{
		if( EVENT_DUNGEON_SURVIVAL == pDungeon->GetEventDungeonProperty()->byDungeonType )
		{
			if( EVENT_DUNGEON_CLOSE == pPacket->byEventState )
			{
				pDungeon->AllUserPKMode(FALSE);
			}			
		}
	}

	return;
}


//------------------------------------------------------------------------------
//	1차 이벤트 종료 되었을떄의 처리...
//------------------------------------------------------------------------------
void CmdEventDungeonFirstTimeOut(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT* pPacket = (WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT*)pMsg;
	
	DSTC_EVENT_DUNGEON_FIRST_TIMEOUT SendPacket;
	SendPacket.wDungeonID = pPacket->wDungeonID;
	memset(&SendPacket.szDungeonName, 0, MAX_DUNGEON_NAME_LENGTH);
	__lstrcpyn(SendPacket.szDungeonName, pPacket->szDungeonName, MAX_DUNGEON_NAME_LENGTH);
	
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)
	{
		return;
	}
	
	SendPacket.bMonsterExpRatio = pDungeon->GetEventDungeonProperty()->byMonsterExpRatio;
	
	pDungeon->EventFirstTimeUpProcess();
	pDungeon->BroadCast((char*)&SendPacket,SendPacket.GetPacketSize());	
	pDungeon->AllUserPKMode();
}


//------------------------------------------------------------------------------
//	이벤트 던전 입장객을 이벤트 던젼에 있는 모든 유져들에게 알려준다.
//------------------------------------------------------------------------------
void CmdEventDungeonJoinUserCount(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	WSTDS_EVENT_JOINUSER_COUNT* pPacket = (WSTDS_EVENT_JOINUSER_COUNT*)pMsg;

	DSTC_EVENT_DUNGEON_JOINUSER_COUNT SendPacket;
	SendPacket.wDungeonID		= pPacket->wDungeonID;
	SendPacket.wJoinUserCount	= pPacket->wJoinUserCount; 

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)
	{
		return;
	}

	pDungeon->BroadCast((char*)&SendPacket,SendPacket.GetPacketSize());	
}


//------------------------------------------------------------------------------
//	이벤트 던전 닫기전에 유저들에게 알린다.
//------------------------------------------------------------------------------
void CmdEventDungeonClose(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_CLOSE* pPacket = (WSTDS_EVENT_DUNGEON_CLOSE*)pMsg;

	DSTC_EVENT_DUNGEON_CLOSE_DISPLAY SendPacket;
	SendPacket.wDungeonID = pPacket->wDungeonID;

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)
	{
		return;
	}

	pDungeon->BroadCast((char*)&SendPacket,SendPacket.GetPacketSize());	
}


//------------------------------------------------------------------------------
//	이벤트 던전 재정비 한다. 
//------------------------------------------------------------------------------
void CmdEventDungeonInit(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_EVENT_DUNGEON_INIT* pPacket = (WSTDS_EVENT_DUNGEON_INIT*)pMsg;

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(NULL == pDungeon)	return;		

	if(FALSE == pPacket->bTimeUp)
	{
		// 이벤트 던젼 열었으나 한명도 안들어 왔을때 
		pDungeon->EventClearProcess();
	}
	else
	{
		pDungeon->EventTimeUpProcess();
		pDungeon->EventClearProcess();
	}	
}