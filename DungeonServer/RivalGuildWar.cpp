#include "StdAfx.h"
#include "RivalGuildWar.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "MonsterTable.h"
#include "Monster.h"
#include "DungeonTable.h"
#include "OwnServer.h"
#include "recvmsg.h"

//길드전 : hwoarang 050205

void RECV_CMD_TEAM_MATCH(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{//월드로 부터 받는 오더
	WSTDS_TEAM_MATCH *pPacket = (WSTDS_TEAM_MATCH*)pMsg;

	CBaseTeamMatch *pTeamMatch = NULL;
	CDungeonLayer *pLayer	= NULL;
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo( pPacket->wDungeonID);		
	if(pDungeon)
	{
		pLayer = pDungeon->GetDungeonLayer( pPacket->byLayerIndex );
		if(	pLayer
		&&	pLayer->m_pTeamMatch)
		{
			pTeamMatch = pLayer->m_pTeamMatch;
		}
	}

	switch(pPacket->wType)
	{
	case CMD_WSTDS_PREPARE_MATCH:
		{
			WSTDS_PREPARE_TEAM_MATCH &rPTM = pPacket->Body.PTM;
			
			if(pLayer)
			{//레이어 있다.
				if( !pLayer->IsTeamMatchRun() )
				{//팀매치 시작 가능? //보고를 했던 서버이기 때문에 무조건 시작 가능 게다가 월드서버는 1개이기 때문에 동기화를 신경 쓸 필요도 없다.
					pLayer->PrepareTeamMatch(rPTM.match_type , rPTM.match_level, rPTM.player_count,  rPTM.team_count, rPTM.match_time, rPTM.dwExFlag?true:false );
				}
			} // if(pDungeon)
		}break;
	case CMD_WSTDS_REG_PLAYER:
		{
			WSTDS_REG_PLAYER &rRP = pPacket->Body.RP;
			
			if(pTeamMatch)
			{
				Log(LOG_NORMAL, "! ----- RegTeamMatchPlayer %s", rRP.szPlayerName);
				pTeamMatch->RegTeamMatchPlayer(rRP.szPlayerName, rRP.team_index , rRP.dwExFlag);
			}
		}break; // if(pDungeon)
	case CMD_WSTDS_UN_REG_PLAYER:
		{
			WSTDS_REG_PLAYER &rRP = pPacket->Body.RP;
			
			if(pTeamMatch)
			{
				Log(LOG_NORMAL, "! ----- UnRegTeamMatchPlayer %s", rRP.szPlayerName);
				pTeamMatch->UnRegTeamMatchPlayer(rRP.szPlayerName );

			}
		}break;
	case CMD_WSTDS_END_MATCH:
		{
			if(pTeamMatch)
			{//레이어 있다.
				pTeamMatch->Terminate();
			} // if(pDungeon)
		}break;
	}    
	return;
}