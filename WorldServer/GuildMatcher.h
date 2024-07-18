#pragma once

#include "../CommonServer/TeamMatcher.h"
#include ".\GuildMatchHandler.h"
#include ".\StadiumMgr.h"

extern CStadiumMgr< CStadium >		g_StadiumMgr;

class CGuildMatcher
	:	public CTeamMatcher< G_MC_DEFAULT , CGuildMatchHandler >
{
	typedef CCondition<  CMatchCondition , size_t >	G_MC_READY;//길드전 가능한 유저의 리스트들의 Key
	typedef map<  G_MC_READY , HANDLER_ARRAY >		READY_HASH;

public:
	CGuildMatcher(void){}
	virtual ~CGuildMatcher(void){}

	virtual bool HandleEvent()
	{	//Ready 상태의 handler를 뽑고
		//Condition 이 같은 handler를 vector로 정렬 //인원수는 들어갔음. G_MC_READY
		//인원수를 받으면? condition 에 넣어서 처리하나?
		//randomshuffle을 행하고 앞에서 부터 팀 숫자 만큼 전투를 붙임. 1번팀 2번팀 -->
		READY_HASH mapReady;
		mapReady.clear();
{
		HANDLER_HASH_ITOR hash_Itor = m_hash.begin();
		while( m_hash.end() != hash_Itor ) //ready 된 애들끼리 그룹화
		{
			T_HANDLER_FOR_HASH &pHandler	= (*hash_Itor).second;
			G_MC_DEFAULT	&r_match_cond	= const_cast< G_MC_DEFAULT& >((*hash_Itor).first);

			size_t minplayerCount;

			switch( r_match_cond.Second().TeamType() )
			{
			case T_T_GUILD:
				minplayerCount = g_min_player_count;
				break;			
			case T_T_S_GUILD:
				++hash_Itor;
				continue;			
			case T_T_RANDOM:
			default:
				minplayerCount = g_min_player_count_AutoMatch;
				break;
			}

			if( pHandler->IsReady( minplayerCount ) )//Ready이냐?
			{
				G_MC_READY ready_cond;
				
				ready_cond.First( r_match_cond.First() );
				ready_cond.Second( pHandler->ReadyPlayerCount() );

				READY_HASH::iterator ready_itor =  mapReady.find( ready_cond );//해당 컨디션이 있는지 찾고
				
				if( mapReady.end() != ready_itor )
				{//이미 HANDLER_ARRAY가 있다면 거기에 iterator를 추가한다.
					HANDLER_ARRAY& rvecHandler = (*ready_itor).second;
					
					READY_ELEMENT Ready(r_match_cond,pHandler) ;
					rvecHandler.push_back( Ready );
				}
				else
				{
					mapReady.insert( make_pair( ready_cond, HANDLER_ARRAY() ) );
					
					READY_HASH::iterator after_itor =  mapReady.find( ready_cond );
					if( mapReady.end() != after_itor )
					{
						HANDLER_ARRAY& rvecHandler = (*after_itor).second;

						READY_ELEMENT Ready(r_match_cond, pHandler) ;

						rvecHandler.push_back( Ready );
					}
					else
					{
						assert( NULL && "Can't Insert GuildMatcher ReadyMap");
					}
				}
			}
			++hash_Itor;
		} // while( m_hash.end() != Itor )
}//아래의 while과 통합 안됨 시도 하지 말 것.
{
		//map<  G_MC_READY , HANDLER_ARRAY >		READY_HASH; //게임 조건을 Key로 가지며 핸들러의 포인터의 배열을 가지는 맵
		READY_HASH::iterator ready_itor = mapReady.begin();//Ready된 
		
		while( mapReady.end()  != ready_itor )
		{//Ready 된 애들을 붙여줌
			G_MC_READY &ready_cond = const_cast<G_MC_READY&>( (*ready_itor).first );
			//현재 운동장이 몇개나 남았는지 확인 해야 한다.
	
			HANDLER_ARRAY& rvecHandler = (*ready_itor).second;//ready 상태의 handler들
			
			if( rvecHandler.size() >= MIN_MATCH_TEAM )// n:n 팀매치를 하기 위해서는 while문 카운트를 바꾸면 된다
			{//게임을 하기위한 최소한의 팀 숫자가 만족된다면
				CStadium *pSD = g_StadiumMgr.PreparedStadium(
					ready_cond.First().MatchType() ,	ready_cond.First().MatchLevel(),
					MIN_MATCH_TEAM,						ready_cond.Second(),
					GWT_TEN,							true );//준비된 운동장을 받는다
				if(!pSD)//해당 조건에 운동장 없으면
				{
					++ready_itor;
					continue;
				}

				random_shuffle( rvecHandler.begin(), rvecHandler.end() );//팀 섞기.
				
				//rvecHandler안에는 handler 들이 들어 있고 거기서 매치를 시킬 Handler만 떼어낸다.
				
				HANDLER_ARRAY vecAttached;//쌈을 붙은 팀을 가지는

				for(size_t i = 0; MIN_MATCH_TEAM > i; ++i)// n:n 팀매치를 하기 위해서는 for문 카운트를 바꾸면 된다
				{
					if( !rvecHandler.empty())
					{
						HANDLER_ARRAY::reverse_iterator rItor = rvecHandler.rbegin() ;
						vecAttached.push_back( (*rItor) );
						rvecHandler.pop_back();
					}
				} // for(size_t i = 0; MIN_MATCH_TEAM > i; ++i)

				//매칭 메세지를 보내자.
				HANDLER_ARRAY::iterator attach_itor = vecAttached.begin();

				vector< size_t > vec_team_index;
				vec_team_index.clear();

				size_t team_index_pos = 0;//경기자는 1팀 부터

				vec_team_index.push_back(1);//팀 인덱스. 지금은 4팀 밖에 없으니까. 이렇게 해놓는다.
				vec_team_index.push_back(2);
				vec_team_index.push_back(3);
				vec_team_index.push_back(4);

				random_shuffle( vec_team_index.begin(), vec_team_index.end() );

				while( vecAttached.end() != attach_itor )
				{
					READY_ELEMENT &ReadyElement = (*attach_itor);
					
					G_MC_DEFAULT &rCond			= ReadyElement.m_Cond;
					T_HANDLER_FOR_HASH pHandler = ReadyElement.m_pHandler;
					
					const size_t ready_count = ready_cond.Second();

					CMatcherResult::NotifyResult NR;

					NR.player_count = ready_count;

					NR.match_type		= ready_cond.First().MatchType();
					NR.pStadium			= pSD;
					NR.team_index		= vec_team_index.at(team_index_pos);
					NR.dwGuildWarFlag	= G_W_F_PLAYER;
					NR.team_type		= rCond.Second().TeamType();
//					NR.DMI.byLayerIndex = pSD->byLayerIndex;
//					NR.DMI.wDungeonID = pSD->dwDungeonID;
//					NR.DMI.wX = pSD->aPos[team_index].x;//위에서 MAX_MATCH_TEAM 만큼만 체크 해서 주기 때문에 index 범위 검사를 할 필요는 없다.
//					NR.DMI.wY = pSD->aPos[team_index].y;
					
					pHandler->OnMatchNotify( NR );//옮겨주고나서는 게임중인 애들 리스트로 넣어주자.
					++attach_itor;
					++team_index_pos;
				}
			}
			++ready_itor;
		} // while( m_MapReady.end()  != ready_itor )
}
		return false;
	}
	
	T_HANDLER* GetSTypeHandler(G_MC_DEFAULT &rGMC)
	{
		HANDLER_HASH_ITOR hash_Itor = m_hash.begin();
		while( m_hash.end() != hash_Itor ) //우리편 리스트 받기
		{
			const G_MC_DEFAULT &rFirst =  (*hash_Itor).first ;
			if( rFirst == rGMC)
			{
				return (*hash_Itor).second;
			}
			++hash_Itor;
		}
		return NULL;
	}
};

extern CGuildMatcher g_GuildMatcher;