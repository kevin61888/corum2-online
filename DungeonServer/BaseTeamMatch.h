#pragma once
#include "stdafx.h"
#include "user.h"
#include "../Commonserver/HS_LIST.h"
#include "../Commonserver/HS_HASH.h"
#include "../Commonserver/RivalGuildWarPacket.h"

class CBaseTeamMatch;

extern void Send_DSTWS_STADIUM_STATE_END( CBaseTeamMatch* pTeamMatch, const bool bIsSuccess);
extern void Send_DSTWS_STADIUM_STATE_MODIFY( CBaseTeamMatch* pTeamMatch );
extern void Send_DSTWS_STADIUM_TEAM_POINT(CBaseTeamMatch* pTeamMatch, const size_t team_index, const size_t point);//팀 점수 보내기

class CBaseTeamMatch
{
public:
	CBaseTeamMatch(const WORD wDungeonID, const BYTE byLayerIndex)
		:m_wDungeonID(wDungeonID), m_byLayerIndex(byLayerIndex)
	{
		Clear();
	}
	virtual ~CBaseTeamMatch(void)
	{
	}

	typedef struct tagTeamInfo
	{
		tagTeamInfo()
		{
			guild_war_flag = 0;
			team_index = 0;
		}

		FLAG_TYPE guild_war_flag;
		COUNT_TYPE team_index;
	}TEAM_INFO;

	typedef struct tagMatchData
	{
		tagMatchData()
		{
			player_count = 0;
			team_index = 0;
		}

		void IncPlayer()
		{
			++player_count;
		}

		void DecPlayer()
		{
			if(0 != player_count)
			{
				--player_count;
			}
		}
	
		void IncPoint( const size_t get_point = 1)
		{
			point += get_point;
		}

		void DecPoint( const size_t lose_point = 1)
		{
			if(point >=lose_point)
			{
				point -= lose_point;
			}
			else
			{
				point = 0;
			}
		}

		FLAG_TYPE team_index;
		COUNT_TYPE player_count;
		COUNT_TYPE point;//점수
	}MATCH_DATA;

	typedef CHS_HASH< size_t, MATCH_DATA > COUNT_HASH;//팀번호, 팀 인원수
	typedef COUNT_HASH::iterator COUNT_ITOR;
public:

	 void Clear()
	 {
		State(TMS_EMPTY);

		m_dwTeamMatchBeginTime	= 0;	// 대전 시작 시간
		m_dwTeamMatchEndTime	= 0;	// 대전 종료 시간
		
		m_mapMatchData.clear();
		m_mapPlayer.clear();
	 }

	virtual void OnEnd() = 0;
	virtual bool OnProcess() = 0;
	virtual bool OnDie(CUnit *pCaster, CUnit *pTarget) = 0;
	virtual bool OnAttack(const CUnit *pCaster, const CUnit *pTarget) = 0;
	virtual bool OnMove(CUnit *pCaster) = 0;
	
	virtual FLAG_TYPE GetMatchType() = 0;

	virtual FLAG_TYPE GetTeamMatchFlag(const CUnit *pCaster) 
	{
		TEAM_INFO *pTI = m_mapPlayer.Find( pCaster->Name() );
		if( pTI )
		{
			return pTI->guild_war_flag;
		}
		return G_W_F_NONE;
	}

	virtual COUNT_TYPE GetTeamIndex(const CUnit *pCaster) 
	{
		TEAM_INFO *pTI = m_mapPlayer.Find( pCaster->Name() );
		if( pTI )
		{
			return pTI->team_index;
		}
		return 0;
	}
	
	virtual void State(const BYTE byState);

	virtual BYTE State(){return m_byState;}

	virtual void RegTeamMatchPlayer(const string &strName, const size_t team_index, const DWORD dwExFlag) 
	{
		TEAM_INFO TI;
		TI.team_index = team_index;
		TI.guild_war_flag = dwExFlag;
		
		if(!m_mapPlayer.Find( strName ) ) 
		{
			if( m_mapPlayer.Insert( strName, TI) )
			{
				MATCH_DATA *pMD = m_mapMatchData.Find( team_index );
				
				if(pMD)
				{
					pMD->IncPlayer();
					Send_DSTWS_STADIUM_TEAM_POINT(this, team_index, pMD->point );//유저 들어올때도 포인트 및 시간 갱신
				}
			} // if( m_mapPlayer.Insert( strName, TI) )
		}
	}

	virtual void UnRegTeamMatchPlayer( const string &strName ) 
	{
		if( m_mapPlayer.Remove( strName ) )
		{
			
		}
	}

	void Prepare(const FLAG_TYPE match_type, const FLAG_TYPE match_level,  const COUNT_TYPE player_count, const COUNT_TYPE team_count, const GUILD_WAR_TIME match_time, const bool bIsAutoMatch);
	
	void Terminate()
	{//준비모드에서만 종료
		if( IsPrepare() )
		{
			Clear();
			Send_DSTWS_STADIUM_STATE_END(this , false);
		} // if( IsPrepare() )
	}

	bool CanMove()
	{
		if( IsRun() )
		{
			return false;
		}
		return true;
	}

	bool IsEmpty()
	{
		if( TMS_EMPTY == State() )
		{
			return true;
		}
		return false;
	}

	bool IsPrepare()
	{
		if( TMS_PREPARE == State() )
		{
			return true;
		}
		return false;
	}

	bool IsRun()
	{
		if( TMS_RUN == State() )
		{
			return true;
		}
		return false;
	}

	void Process();

	WORD DungeonID()const{return m_wDungeonID;}
	BYTE LayerIndex()const {return m_byLayerIndex;}

	size_t GetPoint( const size_t team_index)
	{
		MATCH_DATA *pMD = m_mapMatchData.Find( team_index );
			
		if(pMD)
		{
			return pMD->point;
		}
		return 0;
	}
	
	virtual VECTOR2 GetPos( const size_t team_index, const bool bIsAddRandomPos ) = 0;
	
	DWORD GetRemainEndTime();
	DWORD GetRemainBeginTime();

	FLAG_TYPE MatchLevel() const {return m_match_level;}
	COUNT_TYPE PlayerCount() const {return m_player_count;}
	COUNT_TYPE TeamCount() const {return m_team_count;}
	GUILD_WAR_TIME MatchTime() const {return m_match_time;}

	bool IsAutoMatch( ) const{return m_bIsAutoMatch;}

	void MatchLevel(const FLAG_TYPE  match_level){m_match_level = match_level;}
	void PlayerCount(const COUNT_TYPE player_count){m_player_count = player_count;}
	void TeamCount(const COUNT_TYPE team_count){m_team_count = team_count;}
	void MatchTime(const GUILD_WAR_TIME match_time) {m_match_time = match_time;}

	void IsAutoMatch(const bool bIsAutoMatch) {m_bIsAutoMatch = bIsAutoMatch;}
	

protected:
	typedef CHS_HASH< string, TEAM_INFO >	PLAYER_HASH;
	typedef PLAYER_HASH::iterator			PLAYER_ITOR;
	PLAYER_HASH				m_mapPlayer;//유저명, 팀번호

	COUNT_HASH	m_mapMatchData;//팀번호, 유저 카운트

	//팀전용 매치 플래그
	BYTE		m_byState;//상태. 0 Empty, 1 Prepare, 2 RUN

//	BOOL		m_bIsTeamMatchState;				// 대전 : TRUE,  비대전 : FALSE 
	DWORD		m_dwTeamMatchBeginTime;				// 대전 시작 시간
	DWORD		m_dwTeamMatchEndTime;				// 대전 종료 시간

	const WORD		m_wDungeonID;
	const BYTE		m_byLayerIndex;

	FLAG_TYPE	m_match_level;
	COUNT_TYPE	m_player_count;
	COUNT_TYPE	m_team_count;
	GUILD_WAR_TIME m_match_time;
	bool		m_bIsAutoMatch;
};
