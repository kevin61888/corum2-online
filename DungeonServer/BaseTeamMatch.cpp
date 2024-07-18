#include ".\baseteammatch.h"

#include "recvmsg.h"
#include "MsgProc.h"
#include "OwnServer.h"
#include "DungeonTable.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonLayer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "Monster.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "AcceptTable.h"
#include "MsgProcForServer.h"
#include "MsgProcForServerGuild.h"
#include "MsgProcForServerParty.h"
#include "MsgProcForServerPortal.h"
#include "RecordSet.h"
#include "DBProc.h"
#include "MonsterManager.h"
#include "GameSystemNative.h"
#include "EventDungeonManager.h"
#include "MsgProcForServerEvent.h"
#include "MsgProc_for_jjw6263.h"
#include "GameSystemNative.h"
#include "RivalGuildWar.h"

void CBaseTeamMatch::Process()
{
	if( IsEmpty() )
	{
		return;
	}

	if( IsRun() )
	{
		if( ( m_dwTeamMatchEndTime <= timeGetTime() ) )
		{
			OnEnd();//끝.
			Clear();

			Send_DSTWS_STADIUM_STATE_MODIFY(this);
			Send_DSTWS_STADIUM_STATE_END(this, true);
		} // if( m_dwTeamMatchEndTime <= timeGetTime() )
	} // if( IsRun() )
	
	if( IsPrepare() )
	{
		if( m_dwTeamMatchBeginTime <= timeGetTime() )
		{//게임 시작 할 때.
			if( true != m_bIsAutoMatch )//오토매치가 아닌데 
			{
				if(!OnProcess() )
				{
					State(TMS_EMPTY);
					Send_DSTWS_STADIUM_STATE_MODIFY(this);
					Send_DSTWS_STADIUM_STATE_END(this, false);
					return;
				}
			}

			State(TMS_RUN);
			Send_DSTWS_STADIUM_STATE_MODIFY(this);
		} // if( m_dwTeamMatchBeginTime <= timeGetTime() )

		static DWORD dwPrevTime = timeGetTime();
		const DWORD dwNowTime = timeGetTime();
		
		if(dwPrevTime + 1000 < dwNowTime)//시간 동기화 안되서 보내는 겁니다. 지우지 마십시요
		{
			dwPrevTime = dwNowTime;
			
			if(!m_mapMatchData.empty())
			{
				MATCH_DATA &rMD = (*m_mapMatchData.begin()).second;
				Send_DSTWS_STADIUM_TEAM_POINT(this, rMD.team_index, rMD.point );//
			}
		}
	}
}

void __DisconnectUserInTeamMatch(CUser* pUser)
{
	CDungeonLayer *pLayer = pUser->GetCurDungeonLayer();

	if( pLayer 
	&&	pLayer->m_pTeamMatch)
	{
		pLayer->m_pTeamMatch->UnRegTeamMatchPlayer( pUser->Name() );
	}
}

void MakeStadiumPos(CBaseTeamMatch *pTeamMatch, STADIUM_DATA &rSD)
{
	for( int i = 0; MAX_MATCH_TEAM > i; ++i)
	{
		rSD.aPos[i].x = pTeamMatch->GetPos(i,false).x;//옵저버
		rSD.aPos[i].y = pTeamMatch->GetPos(i,false).y;
	}
}

void CBaseTeamMatch::State(const BYTE byState)
{
	m_byState = byState;
}

DWORD CBaseTeamMatch::GetRemainBeginTime()
{
	const DWORD dwNow = timeGetTime();
	if(dwNow <= m_dwTeamMatchBeginTime)
	{
		const DWORD dwRemainTime = m_dwTeamMatchBeginTime - dwNow;
		return dwRemainTime/1000;
	} // if(dwNow <= m_dwTeamMatchEndTime)

	return 0;
}

DWORD CBaseTeamMatch::GetRemainEndTime()
{
	const DWORD dwNow = timeGetTime();
	if(dwNow <= m_dwTeamMatchEndTime)
	{
		const DWORD dwRemainTime = m_dwTeamMatchEndTime - dwNow;
		return dwRemainTime/1000;
	} // if(dwNow <= m_dwTeamMatchEndTime)

	return 0;
}

void CBaseTeamMatch::Prepare(const FLAG_TYPE match_type, const FLAG_TYPE match_level,  const COUNT_TYPE player_count, const COUNT_TYPE team_count, const GUILD_WAR_TIME match_time, const bool bIsAutoMatch)
{
	const DWORD dwNow = timeGetTime();

	State(TMS_PREPARE);

	IsAutoMatch( bIsAutoMatch);

	// Remark
#if defined(GUILDMATCH_TEST)
	match_time = 1;
#endif
	
	if( bIsAutoMatch )
	{
		m_dwTeamMatchBeginTime = ( 25 )*1000 + dwNow;//대기시간 임시로 10초
		m_dwTeamMatchEndTime = m_dwTeamMatchBeginTime + (match_time*1000*60);//600000;// 임시로 한판에 30초 
	} // if( bIsAutoMatch )
	else
	{
		m_dwTeamMatchBeginTime = ( 180 )*1000 + dwNow;//대기시간 3분
		m_dwTeamMatchEndTime = m_dwTeamMatchBeginTime + (match_time*1000*60);//600000;// 임시로 한판에 20초 
	}

	MatchLevel(match_level);
	PlayerCount( player_count);
	TeamCount(team_count);
	MatchTime(match_time);

	m_mapMatchData.clear();
	
	for( size_t x = 0 ; MAX_MATCH_TEAM >= x ; ++x)//0번 팀 부터 셋팅 -> 옵저버,  1 2 3 팀 
	{
		MATCH_DATA MD;
		MD.player_count = player_count;
		MD.point =0;
		MD.team_index = static_cast< FLAG_TYPE >(x);
		m_mapMatchData.Insert(x, MD);
	}
	Send_DSTWS_STADIUM_STATE_MODIFY(this);
}

void Send_DSTWS_STADIUM_TEAM_POINT(CBaseTeamMatch* pTeamMatch, const size_t team_index, const size_t point)//팀 점수 보내기
{
	DSTWS_STADIUM_STATE packet;	

	packet.wType = STADIUM_POINT;

	STADIUM_TEAM_POINT &rSTP = packet.Body.STP;

	rSTP.wDungeonID = pTeamMatch->DungeonID();
	rSTP.byLayerIndex = pTeamMatch->LayerIndex();
	rSTP.match_type	= pTeamMatch->GetMatchType();

	rSTP.team_index = team_index;
	rSTP.point = point;
	
	rSTP.dwRemainBeginTime = pTeamMatch->GetRemainBeginTime();
	rSTP.dwRemainEndTime = pTeamMatch->GetRemainEndTime();

	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void MakeStadiumStatePacket(DSTWS_STADIUM_STATE &rPacket, CBaseTeamMatch* pTeamMatch)
{
	STADIUM_DATA &rSD	= rPacket.Body.SD;
	
	rSD.wDungeonID		= pTeamMatch->DungeonID();
	rSD.byLayerIndex	= pTeamMatch->LayerIndex();
	rSD.match_type		= pTeamMatch->GetMatchType();
	
	rSD.byState			= pTeamMatch->State();
	rSD.match_level		= pTeamMatch->MatchLevel();
	rSD.player_count	= pTeamMatch->PlayerCount();
	rSD.team_count		= pTeamMatch->TeamCount();
	rSD.match_time		= pTeamMatch->MatchTime();

	rSD.dwRemainBeginTime	= pTeamMatch->GetRemainBeginTime();
	rSD.dwRemainEndTime		= pTeamMatch->GetRemainEndTime();

	rSD.bIsAutoMatch	= pTeamMatch->IsAutoMatch();

	MakeStadiumPos(pTeamMatch, rSD);
}

void Send_DSTWS_STADIUM_STATE_END( CBaseTeamMatch* pTeamMatch, const bool bIsSuccess)
{
	DSTWS_STADIUM_STATE packet;	
	packet.wType = STADIUM_END;

	MakeStadiumStatePacket(packet, pTeamMatch);

	packet.bIsSuccess = bIsSuccess;

	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void Send_DSTWS_STADIUM_STATE_MODIFY( CBaseTeamMatch* pTeamMatch )
{
	DSTWS_STADIUM_STATE packet;	
	packet.wType = STADIUM_MODIFY;

	MakeStadiumStatePacket(packet, pTeamMatch);

	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void DieOnGuildWar(CDungeonLayer *pLayer, CUser* pDieUser)
{
	const size_t team_index = pLayer->m_pTeamMatch->GetTeamIndex((CUser*)pDieUser);
	VECTOR2 pos = pLayer->m_pTeamMatch->GetPos(team_index, true);

	// 대상을 빠른 속도로 스팟 포인트로 보낸다
/*	DSTC_MOVE move;
	move.dwIndex			= pDieUser->GetID();
	move.bMoveType			= UNIT_STATUS_RUNNING;
	move.fSpeed				= 1000.f;
	move.v2MoveStartPos		= pos;
	move.v2MoveDirection	= *pDieUser->GetCurDirection();
	move.wDestX				= pos.x;
	move.wDestZ				= pos.y;
	pLayer->BroadCastMsg((char*)&move, move.GetPacketSize());
*/ 
//	pDieUser->SetCurPosition( &pos );


	DSTC_GUILDWAR_DIE_MSG GWDieMessage;
	GWDieMessage.dwUserIndex = pDieUser->GetID();
	lstrcpy(GWDieMessage.szName, pDieUser->GetName());
	pLayer->BroadCastMsg((char*)&GWDieMessage, GWDieMessage.GetPacketSize());

	UpdateUserForAI( pDieUser );

	pDieUser->RemoveAllDetachSkill();

	if(pDieUser->IsSummonGuardian(0))
	{
		pDieUser->UnSummonGuardian(0);
	}

	pDieUser->GetCurDungeon()->RemoveAllUserMonster( pDieUser );

	pDieUser->SetHP(pDieUser->GetMaxHP());
	pDieUser->SetSP(pDieUser->GetMaxSP());

	DSTC_DISAPPEAR	DisAppear;
	DisAppear.dwUserIndex = pDieUser->GetID();
	
	// 메시지를 보낸다.
	pLayer->BroadCastMsg((char*)&DisAppear, DisAppear.GetPacketSize());

	DSTC_STOP	stop;
	stop.dwUserIndex	= pDieUser->GetID();
	stop.v2StopPos		= pos;
	pLayer->BroadCastMsg( (char*)&stop, stop.GetPacketSize() );

	pDieUser->SetCurPosition(&pos);

	DSTC_APPEAR	AppearMe;	
	MakeAppearUser(&AppearMe, pDieUser, 0);
	pLayer->BroadCastMsg( (char*)&AppearMe, AppearMe.GetPacketSize() );

}