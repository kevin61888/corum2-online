#pragma once
#include "../commonserver/HS_HASH.h"
#include "../commonserver/HS_LIST.h"

#include "../commonserver/RivalGuildWarPacket.h"
#include "DungeonTable.h"
#include "DUNGEON_DATA_EX.h"
#include "../commonserver/ClassSupport.h"

#include "GuildHash.h"
#include "WorldUserTable.h"

#include "DebugLog.h"
extern CDebugLog g_GuildWarLog;

extern bool MoveToPrevDungeon( CWorldUser *pUser, const WORD wNowStadiumID, const bool bIsGMCClear);//기본적으로 플레이어다

typedef struct tagStadiumPlayer
{
//	tagStadiumPlayer()
//	{
//		team_index = 0;
//		dwGuildWarFlag = 0;
//	}

	tagStadiumPlayer(const FLAG_TYPE &r_match_type, const DUNGEON_MOVE_INFO &rDMI, const size_t &r_team_index, const DWORD &rdwGuildWarFlag, const FLAG_TYPE &r_team_type)
	{
		match_type		= r_match_type;
		team_index		= r_team_index;
		dwGuildWarFlag	= rdwGuildWarFlag;
		team_type		= r_team_type;
		DMI				= rDMI;
	}

	FLAG_TYPE	team_type;
	FLAG_TYPE	match_type;
	size_t		team_index;
	DWORD		dwGuildWarFlag;
	DUNGEON_MOVE_INFO DMI;
}STADIUM_PLAYER;

typedef CHS_HASH< string, STADIUM_PLAYER > PLAYER_HASH;
typedef PLAYER_HASH::iterator PLAYER_ITOR;

struct tagTeamMatchPointCalc
{
	DWORD	dwPoint;
	DWORD	dwGuildID;	

	__inline	bool	operator<( CONST tagTeamMatchPointCalc& rhs )
	{
		return dwPoint > rhs.dwPoint;
	}
};

typedef struct tagTeamData
{
	FLAG_TYPE	team_type;
	DWORD		dwGuildID;

	size_t		point;
	PLAYER_HASH mapPlayer;
}TEAM_DATA;

class CStadium
	: public STADIUM_DATA
{
public:
	CStadium(const STADIUM_DATA &rSD)
	{
//		PlayerHash().clear();
		TeamHash().clear();
		
		*this = rSD;//연산자 오버로딩 했기때문에 괜찮다.
	}
	~CStadium(){}

public:
	void operator = (const STADIUM_DATA &rSD)
	{
		wDungeonID			= rSD.wDungeonID;
		byLayerIndex		= rSD.byLayerIndex;
		match_type			= rSD.match_type;
		byState				= rSD.byState;

		match_level			= rSD.match_level;
		player_count		= rSD.player_count;
		team_count			= rSD.team_count;
		dwRemainBeginTime	= rSD.dwRemainBeginTime;
		dwRemainEndTime		= rSD.dwRemainEndTime;
		
		bIsAutoMatch		= rSD.bIsAutoMatch;
		::memcpy( &this->aPos[0], &rSD.aPos[0], sizeof(rSD.aPos) );
	}

	void operator = (const CStadium &rStadium)//대입시에 리스트는 지우지 않습니다.
	{
		wDungeonID			= rStadium.wDungeonID;
		byLayerIndex		= rStadium.byLayerIndex;
		match_type			= rStadium.match_type;
		byState				= rStadium.byState;

		match_level			= rStadium.match_level;
		player_count		= rStadium.player_count;
		team_count			= rStadium.team_count;
		dwRemainBeginTime	= rStadium.dwRemainBeginTime;
		dwRemainEndTime		= rStadium.dwRemainEndTime;

		bIsAutoMatch		= rStadium.bIsAutoMatch;
		::memcpy( &this->aPos[0], &rStadium.aPos[0], sizeof(rStadium.aPos) );
	}

	typedef CHS_HASH< size_t, TEAM_DATA >	TEAM_HASH;//팀 인덱스
	typedef TEAM_HASH::iterator	TEAM_ITOR;//사람이름에 기타 정보
//	typedef CHS_HASH< string , STADIUM_PLAYER > PLAYER_HASH;//사람이름에 기타 정보
//	typedef PLAYER_HASH::iterator PLAYER_ITOR;//사람이름에 기타 정보

	void InsertUser(CWorldUser *pUser, STADIUM_PLAYER S_P)
	{
		TEAM_DATA *pTeamData = TeamHash().Find( S_P.team_index );
		if(!pTeamData)
		{
			TEAM_DATA TD;
			TD.point = 0;

			TD.team_type = S_P.team_type;
			
			if( T_T_RANDOM == S_P.team_type )
			{	
				TD.dwGuildID = 0;
			}
			else
			{
				TD.dwGuildID = pUser->m_dwGuildId;
			}
		
			TeamHash().Insert(S_P.team_index, TD );
			pTeamData = TeamHash().Find( S_P.team_index );
			
			if(!pTeamData)
			{
				assert(NULL && "void InsertUser(CWorldUser *pUser, STADIUM_PLAYER S_P)" );
			}
		} // if(!pTeamData)

		STADIUM_PLAYER *pSP = pTeamData->mapPlayer.Find( pUser->m_szCharacterName );

		if(!pSP )
		{
			if( pTeamData->mapPlayer.size() < player_count )
			{
				pTeamData->mapPlayer.Insert( pUser->m_szCharacterName, S_P);
				Log(LOG_NORMAL, "@ ------- Insert Success User Name[%s] Now[%d], Max[%d]", pUser->m_szCharacterName, pTeamData->mapPlayer.size(), player_count);
			}
			else
			{
				Log(LOG_NORMAL, "@ ------- Can't Insert User Name[%s] Now[%d], Max[%d]", pUser->m_szCharacterName, pTeamData->mapPlayer.size(), player_count);
			}
		}
	}
	
	bool RemoveUser(CWorldUser *pTargetUser)
	{
		TEAM_ITOR itor = TeamHash().begin();
		while( TeamHash().end() != itor)
		{
			TEAM_DATA &rTD =  (*itor).second;

			PLAYER_ITOR player_itor = rTD.mapPlayer.begin();

			while( rTD.mapPlayer.end() != player_itor )
			{
				CWorldUser *pUser = g_pUserTable->GetUserInfoByName( (*player_itor).first.c_str() );		
				if(pTargetUser == pUser)
				{
					player_itor = rTD.mapPlayer.erase( player_itor ); 
					return true;
				}
				player_itor++;
			}
			++itor;
		} // while( Hash().end() != itor)

		return false;
	}
	
	void ClearUser()
	{
		TEAM_ITOR itor = TeamHash().begin();
		while( TeamHash().end() != itor)
		{
			TEAM_DATA &rTD =  (*itor).second;

			PLAYER_ITOR player_itor = rTD.mapPlayer.begin();

			while( rTD.mapPlayer.end() != player_itor )
			{
				CWorldUser *pUser = g_pUserTable->GetUserInfoByName( (*player_itor).first.c_str() );		
				if(pUser)
				{
					MoveToPrevDungeon( pUser, wDungeonID, true);
				}
				player_itor++;
			}
			++itor;
		} // while( Hash().end() != itor)
		TeamHash().clear();
	}

	STADIUM_PLAYER* GetUserStadium(const string &strName)
	{
		TEAM_ITOR itor = TeamHash().begin();
		while( TeamHash().end() != itor)
		{
			TEAM_DATA &rTD=  (*itor).second;
			
			STADIUM_PLAYER *pSP= rTD.mapPlayer.Find(strName);
			if(pSP)
			{
				return pSP;
			}
			++itor;
		} // while( TeamHash().end() != itor)
		return NULL;
	}

	void ModifyPoint(const size_t team_index, const size_t point, const BYTE byState)//, const DWORD dwGuildID)
	{
		TEAM_DATA* pTD = TeamHash().Find( team_index );
		
		if(pTD)
		{
			pTD->point = point;
		}
		else
		{
			TEAM_DATA TD;//이거 문제 있어 보이는데;
			TD.point = point;

			TeamHash().Insert( team_index, TD );
		}
		SendPointToUser(NULL);
	} // ifyPoint(const size_t team_index, const size_t point)//, const DWORD dwGuildID)

	void SendStateBroadCast()
	{
		TEAM_MATCH_PACKET packet;
		packet.wType = WSTC_TM_GUILD_WAR_INFO;
		GUILD_WAR_INFO &rWTGWI = packet.Body.GuildWarInfo;

		rWTGWI.byState = byState;
		rWTGWI.match_type = match_type;
		rWTGWI.match_level  = match_level;
		rWTGWI.team_count = team_count;
		rWTGWI.player_count = player_count;

		TEAM_ITOR team_itor = TeamHash().begin();
		
//		rGMG.dwSize = TeamHash().size();

		ZeroMemory( &rWTGWI.a_bIsUse[0], sizeof(rWTGWI.a_bIsUse) );

		vector< tagTeamMatchPointCalc > vecPointCalc;

		vecPointCalc.reserve( 8 );

		BOOL	bSenior = FALSE;
		
		while(team_itor != TeamHash().end() )
		{//점수 메이드
			TEAM_DATA &rTD = (*team_itor).second;

			const size_t team_index = (*team_itor).first;
			if(team_index && team_index < MAX_MATCH_TEAM)
			{
				rWTGWI.a_bIsUse[team_index] = true;
				rWTGWI.a_team_type[team_index] = rTD.team_type;

				if( T_T_RANDOM != rTD.team_type )//이름셋팅
				{
					if( T_T_S_GUILD == rTD.team_type && bSenior == FALSE )
						bSenior = TRUE;

					tagTeamMatchPointCalc	abc;
														
					abc.dwGuildID = rWTGWI.adwGuildID[team_index] = rTD.dwGuildID;
					abc.dwPoint = rTD.point;

					vecPointCalc.push_back( abc );

					LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData( rTD.dwGuildID );
					if(lpGuildTable)
					{
						strcpy( rWTGWI.aszGuildName[team_index], lpGuildTable->sGuild.szGuildName);
					}
				} // if(rTD.dwGuildID)
				else
				{
					rWTGWI.adwGuildID[team_index] = 0;
					sprintf( rWTGWI.aszGuildName[team_index], "RandomTeam%d", team_index);
				}
				rWTGWI.a_point[team_index] = rTD.point;
			} // if( team_index < MAX_MATCH_TEAM)
			++team_itor;
		} // while(team_itor != TeamHash().end() )

		std::sort( vecPointCalc.begin(), vecPointCalc.end() );

		// 두팀일때만 가능하다!!!
		if( vecPointCalc.size() >= 2 )
		{
			TCHAR	szQuery[0xff];

			if( bSenior )
			{
				if( vecPointCalc[0].dwPoint == vecPointCalc[1].dwPoint )
				{
					::wsprintf( szQuery, _T("EXEC SP_UPDATE_SENIOR_GUILDWAR_DRAW_RESULT %u, %u"), vecPointCalc[0].dwGuildID, vecPointCalc[1].dwGuildID );
				}
				else
				{
					::wsprintf( szQuery, _T("EXEC SP_UPDATE_SENIOR_GUILDWAR_WINLOSE_RESULT %u, %u"), vecPointCalc[0].dwGuildID, vecPointCalc[1].dwGuildID );
				}
			}
			else
			{
				if( vecPointCalc[0].dwPoint == vecPointCalc[1].dwPoint )
				{
					::wsprintf( szQuery, _T("EXEC SP_UPDATE_JUNIOR_GUILDWAR_DRAW_RESULT %u, %u"), vecPointCalc[0].dwGuildID, vecPointCalc[1].dwGuildID );
				}
				else
				{
					::wsprintf( szQuery, _T("EXEC SP_UPDATE_JUNIOR_GUILDWAR_WINLOSE_RESULT %u, %u"), vecPointCalc[0].dwGuildID, vecPointCalc[1].dwGuildID );
				}
			}

			g_pDb->ExecuteSQL( szQuery, (BYTE)GAME_DB );
		}

		rWTGWI.a_bIsUse[0] = false; //옵저버니까 안써.

		BroadCastToAllUser( (char*)&packet, packet.GetPacketSize());
	}

	// 길드의 레더정보를 유저에게 보내준다.
	void SendLadderInfoToUser(CWorldUser *pUser = NULL)//, const DWORD dwGuildID)
	{
		GUILD_MATCHBEGIN_SETTING packet;

		packet.eType = GMS_TYPE_LADDER_INFO;
		
		GUILD_MATCHBEGIN_LADDER_INFO &rGMLI = packet.Body.m_LadderInfo;


		// 길드전 준비시간을 셋팅
		rGMLI.dwRemainBeginTime		= dwRemainBeginTime;
		// 길드전 완료시간을 셋팅
		rGMLI.dwRemainEndTime		= dwRemainEndTime;

		Log(LOG_NORMAL, "@ ------- SendLadderInfoToUser Begin Time[%d] EndTime[%d]", dwRemainBeginTime, dwRemainEndTime);

		// 
		rGMLI.m_dwMaxMemberCount = team_count;
		rGMLI.m_dwEnemyWaitMemberCount = player_count;			// 상대길드 전체대기멤버 수
		rGMLI.m_dwEnemyWaitMemberAverageLevel = 0;	// 상대길드 전체대기멤버 평균레벨
///////////////
		TEAM_ITOR team_itor = TeamHash().begin();
		
//		packet.dwSize = TeamHash().size();

		ZeroMemory( &rGMLI.a_bIsUse[0], sizeof(rGMLI.a_bIsUse));//안쓰는 팀은 false 로 셋팅 해야 하니까 

		while(team_itor != TeamHash().end() )
		{//점수 메이드
			TEAM_DATA &rTD = (*team_itor).second;

			const size_t team_index = (*team_itor).first;

			if( team_index && team_index < MAX_MATCH_TEAM)
			{
				rGMLI.a_bIsUse[team_index] = true;				
				rGMLI.a_team_type[team_index] = rTD.team_type;

				// Random팀이 아닐경우.
				if( T_T_RANDOM != rTD.team_type )//이름셋팅
				{
					LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData( rTD.dwGuildID );
					if(lpGuildTable)
					{
						strcpy( rGMLI.m_szGuildName[team_index], lpGuildTable->sGuild.szGuildName);
					}
				} // if(rTD.dwGuildID)
				//
				else
				{
					rGMLI.m_dwLadderGrade[team_index] = 0;
					rGMLI.m_dwLadderPoint[team_index] = 0;
					sprintf( rGMLI.m_szGuildName[team_index], "RandomTeam%d", team_index);
				}
			} // if( team_index < MAX_MATCH_TEAM)
			++team_itor;
		} // while(team_itor != TeamHash().end() )

		rGMLI.a_bIsUse[0] = false; //옵저버니까 안써.
		if(!pUser)
		{
			BroadCastToPlayer( (char*)&packet, packet.GetPacketSize() ) ;
		}
		else
		{
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}

	void SendTeamInfoToUser(CWorldUser *pUser = NULL)//, const DWORD dwGuildID)
	{
		GUILD_MATCH_STATUS_RESULT packet;

		TEAM_ITOR team_itor = TeamHash().begin();
		
		packet.dwSize = TeamHash().size();

		ZeroMemory( &packet.a_bIsUse[0], sizeof(packet.a_bIsUse));//안쓰는 팀은 false 로 셋팅 해야 하니까 

		while(team_itor != TeamHash().end() )
		{//점수 메이드
			TEAM_DATA &rTD = (*team_itor).second;

			const size_t team_index = (*team_itor).first;
			if( team_index && team_index < MAX_MATCH_TEAM)
			{
				packet.dwRemainBeginTime = dwRemainBeginTime;
				packet.dwRemainEndTime = dwRemainEndTime;
				packet.a_bIsUse[team_index] = true;
				
				if( T_T_RANDOM != rTD.team_type )//이름셋팅
				{
					LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData( rTD.dwGuildID );
					if(lpGuildTable)
					{
						strcpy( packet.szName[team_index], lpGuildTable->sGuild.szGuildName);
					}
				} // if(rTD.dwGuildID)
				else
				{
					sprintf( packet.szName[team_index], "RandomTeam%d", team_index);
				}
				
				packet.a_point[team_index] = rTD.point;
			} // if( team_index < MAX_MATCH_TEAM)
			++team_itor;
		} // while(team_itor != TeamHash().end() )

		packet.a_bIsUse[0] = false; //옵저버니까 안써.
		if(!pUser)
		{
			BroadCastToPlayer( (char*)&packet, packet.GetPacketSize() ) ;
		}
		else
		{
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}

	void SendPointToUser(const BYTE State,CWorldUser *pUser = NULL)//, const DWORD dwGuildID)
	{
		GUILD_MATCH_STATUS packet;

		GUILD_MATCH_GUILDINFO &rGMG = packet.m_GuildInfo;

		ZeroMemory(&rGMG , sizeof(rGMG ));
		
		rGMG.byState = State;

		TEAM_ITOR team_itor = TeamHash().begin();
		
		rGMG.dwSize = TeamHash().size();

//		Log(LOG_NORMAL, "@ ------- SendPointToUser Begin Time[%d] EndTime[%d]", dwRemainBeginTime, dwReaminEndTime);

		while(team_itor != TeamHash().end() )
		{//점수 메이드
			TEAM_DATA &rTD = (*team_itor).second;

			const size_t team_index = (*team_itor).first;
			if( team_index < MAX_MATCH_TEAM)
			{
				rGMG.dwRemainBeginTime = dwRemainBeginTime;
				rGMG.dwRemainEndTime = dwRemainEndTime;
				rGMG.a_point[team_index] = rTD.point;
				rGMG.a_bIsUse[team_index] = true;
			} // if( team_index < MAX_MATCH_TEAM)
			++team_itor;
		} // while(team_itor != TeamHash().end() )

		rGMG.a_bIsUse[0] = false; //옵저버니까 안써.

		if(!pUser)
		{
			BroadCastToPlayer( (char*)&packet, packet.GetPacketSize() ) ;
		}
		else
		{
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}

	void BroadCastToPlayer( char *pPacket, const size_t length)
	{
		TEAM_ITOR team_itor = TeamHash().begin();
		while(team_itor != TeamHash().end() )
		{
			TEAM_DATA &rTD = (*team_itor).second;
			
			PLAYER_ITOR player_itor = rTD.mapPlayer.begin();
			while(rTD.mapPlayer.end() != player_itor )
			{	
				CWorldUser *pUser = g_pUserTable->GetUserInfoByName( (*player_itor).first.c_str() );		
				if(pUser)
				{
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, pPacket, length, FLAG_SEND_NOT_ENCRYPTION);
				}
				++player_itor;
			}
			++team_itor;
		} // while(team_itor = TeamHash().end() )
	}

	void BroadCastToAllUser(char *pPacket, const size_t length)
	{
		g_pUserTable->BroadCast( pPacket, length );
	}
//	PLAYER_HASH& PlayerHash(){return m_PlayerHash;}
	TEAM_HASH& TeamHash(){return m_TeamHash;}

	SYSTEMTIME	m_PrepareTime;//경기 시작 시간
protected:
//	PLAYER_HASH m_PlayerHash;
	TEAM_HASH	m_TeamHash;
};

template < typename T_ELEMENT >
class CStadiumMgr
{
public:
	CStadiumMgr(void){};
	~CStadiumMgr(void){};

	//STADIUM_DATA 여기에 CHS_LIST 붙여서 

	typedef CHS_LIST< T_ELEMENT > STADIUM_LIST;
	typedef CHS_HASH< FLAG_TYPE, STADIUM_LIST > STADIUM_HASH;//매치타입을 가진 맵
	
	typedef typename STADIUM_LIST::iterator LIST_ITOR;
	typedef typename STADIUM_HASH::iterator HASH_ITOR;

	void MakeGuildWarList(GUILD_WAR_DATA_LIST &rGWDL, const size_t cur_page)
	{
		::memset( &rGWDL, 0, sizeof(GUILD_WAR_DATA_LIST) );

		size_t index = 0;

		size_t element_count_per_page = 0;
		size_t total_element_count = 0;
		size_t total_page = 0;
		size_t end_page = 0;

		//요구하는 페이지 정보를 줘야된다.
		HASH_ITOR hash_itor = m_hash.begin();
		while(	(m_hash.end() != hash_itor) )
		{
			STADIUM_LIST &rList = (*hash_itor).second;

			LIST_ITOR list_itor = rList.begin();
			while( rList.end() != list_itor )
			{
				T_ELEMENT	&rElement = (*list_itor);

				if(TMS_RUN == rElement.byState)//경기중인 애들만
				{//경기중인 곳이 있는것.
					GUILD_WAR_DATA &rGWD = rGWDL.aGuildWarData[index++];
					
					rGWD.wDungeonID		= rElement.wDungeonID;
					rGWD.byLayerIndex	= rElement.byLayerIndex;
					rGWD.match_type		= rElement.match_type;
					rGWD.match_level	= rElement.match_level;
					rGWD.player_count	= rElement.player_count;

					for( size_t i = 1; MAX_MATCH_TEAM >i; i++ )//MAX_MATCH_TEAM rElement.team_count
					{//1번 팀 부터 시작
						GUILD_LADDER_DATA &rGLD = rGWD.aGLD[i];//길드 레더 데이터
						
						TEAM_DATA *pTeamData =rElement.TeamHash().Find( i );

						if(pTeamData)
						{
							MakeGuildLadderData( pTeamData->team_type, pTeamData->dwGuildID, i, rGLD );
						}
						else
						{
							rGLD.bIsUse = false;
						}
//						rElement.TeamHash();
//						MakeGuildLadderData( rGLD, TeamHash() );
					} // for( size_t i = 0; rElement.team_count >i; i++ )//MAX_MATCH_TEAM

					if( 0 == (total_element_count % MAX_LADDER_LIST) )//한페이지 지나면
					{
						++end_page;//마지막 페이지가 얼마였나
						++total_page;
						element_count_per_page = 0;
						index = 0;
//						if(total_page == cur_page)
//						{
//							bStop = true;
//							break;
//						}
					} // if( 0 == (total_element_count % MAX_LADDER_LIST) )//한페이지 지나면

					++total_element_count;//위치는 여기가 맞다.
					++element_count_per_page;
				} // if(TMS_RUN == rElement.byState)//경기중인 애들만
				++list_itor;
			} // while( rList.end() != list_itor )
			++hash_itor;
		} // while( m_hash.end() != hash_itor)

		rGWDL.total_page = total_page;
		rGWDL.cur_page = end_page;
		rGWDL.count_per_page = element_count_per_page;
	}

	// 길드의 레더데이터를 팀에다 셋팅한다.
	void MakeGuildLadderData(const FLAG_TYPE team_type, const DWORD dwGuildID, const COUNT_TYPE team_index, GUILD_LADDER_DATA &rGLD)
	{
		// 길드레더데이터를 사용한다.
		rGLD.bIsUse = true;

		// 팀타입이
		switch( team_type )
		{
		// 랜덤팀일 경우
		case T_T_RANDOM:	
			{
				// 이름은 없음.
				::sprintf( rGLD.szGuildName, "RandomTeam%d", team_index);

				// 점수라던지, 등수는 없다.
				rGLD.iGuildTotalCount = 0;				
				rGLD.iRank[0] = 0;
				rGLD.iRank[1] = 0;
				rGLD.iRank[2] = 0;
				rGLD.iTotalPoint = 0;
				rGLD.MonthPoint[0] = 0;
				rGLD.MonthPoint[1] = 0;
				rGLD.MonthPoint[2] = 0;
				rGLD.WeekPoint[0] = 0;
				rGLD.WeekPoint[1] = 0;
				rGLD.WeekPoint[2] = 0;
			}
			break;
		// 길드이거나, 길드연합일 경우
		case T_T_GUILD:
		case T_T_S_GUILD:
			{
				// 길드정보 테이블을 로드
				LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData( dwGuildID );		// 신청한 길드

				// 이름있다. 복사한다.
				::memcpy( 
					rGLD.szGuildName, 
					lpGuildTable->sGuild.szGuildName, 
					min(MAX_CHARACTER_NAME_LENGTH, MAX_GUILD_NAME_LENGTH) 
					);

				// 길드정보 테이블에서 현재 길드의 정보를 얻어온다.
			}
			break;
		}
	}

	size_t EmptyStadiumCount(const FLAG_TYPE match_type)
	{
		STADIUM_LIST *pList = m_hash.Find(match_type);
		size_t count = 0;
		if( pList )
		{
			STADIUM_LIST::iterator iter = pList->begin();
			while( pList->end() != iter )
			{
				STADIUM_DATA &rSD = (*iter);
				if( rSD.byState == TMS_EMPTY )
				{
					++count;
				}
				++iter;
			}
		}

		return count;
	}

	T_ELEMENT* PreparedStadium(	const FLAG_TYPE match_type, const FLAG_TYPE match_level, 
								const COUNT_TYPE team_count, const COUNT_TYPE player_count,
								const GUILD_WAR_TIME match_time, const bool bIsAutoMatch = true)
	{
		STADIUM_LIST *pList = m_hash.Find(match_type);
		if( pList )
		{
			STADIUM_LIST::iterator iter = pList->begin();
			while( pList->end() != iter )
			{
				T_ELEMENT &rElement = (*iter);
				if(rElement.byState == TMS_EMPTY)
				{
					rElement.byState = TMS_PREPARE;//던전에서 응답 받기 전에 다른데서 쓸 수 있으므로 플래그 셋팅

					rElement.match_level		= match_level;//얘네는 유동적으로 변하니까 prepare 할때마다 바뀐다.
					rElement.team_count			= team_count;
					rElement.player_count		= player_count;
					rElement.match_time			= match_time;
					rElement.bIsAutoMatch		= bIsAutoMatch;//오토매치냐?

					SendStartTeamMatch( rElement.wDungeonID, 
										rElement.byLayerIndex, 
										rElement.match_type, 
										
										rElement.match_level,
										rElement.player_count,
										rElement.team_count,
										rElement.match_time,
										bIsAutoMatch );//해당 맵에 경기 시작을 알린다.

					return &rElement;
				} // if(rSD.bCanUse)
				++iter;
			}
		}
		return NULL;
	}

	STADIUM_PLAYER* GetUserStadium(const string &strName)
	{
		HASH_ITOR hash_itor = m_hash.begin();
		while( m_hash.end() != hash_itor)
		{
			STADIUM_LIST &rList = (*hash_itor).second;

			LIST_ITOR list_itor = rList.begin();
			while( rList.end() != list_itor )
			{
				T_ELEMENT	&rElement = (*list_itor);

				STADIUM_PLAYER *pSP = rElement.GetUserStadium(strName);
				if( pSP )
				{
					return pSP;
				}
				++list_itor;
			} // while( rList.end() != list_itor )
			++hash_itor;
		}
		return NULL;
	}

	T_ELEMENT* GetStadium( const FLAG_TYPE match_type , const WORD wDungeonID, const BYTE byLayerIndex )
	{
		STADIUM_LIST *pList = m_hash.Find(match_type);
		if( pList )
		{
			STADIUM_LIST::iterator iter = pList->begin();
			while( pList->end() != iter )
			{
				T_ELEMENT &rElement = (*iter);
				if(	rElement.wDungeonID == wDungeonID
				&&	rElement.byLayerIndex == byLayerIndex)
				{
					return &rElement;
				} // if(rSD.bCanUse)
				++iter;
			}
		}
		return NULL;
	}

	T_ELEMENT* GetStadium( CWorldUser *pUser )
	{
		STADIUM_PLAYER *pSP =  GetUserStadium( pUser->m_szCharacterName );
		if(pSP)
		{
			CStadium *pStadium = GetStadium( pSP->match_type, pSP->DMI.wDungeonID, pSP->DMI.byLayerIndex);
			if(pStadium)
			{
				return pStadium;
			}
		}
		return NULL;
	}

	void SendStartTeamMatch(const WORD wDungeonID, const BYTE byLayerIndex, const FLAG_TYPE match_type, 
							const FLAG_TYPE	match_level, const COUNT_TYPE player_count, const COUNT_TYPE team_count,
							const GUILD_WAR_TIME match_time, const bool bIsDefault)
	{	// 던전 서버에게 보내줘라.
		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(wDungeonID);

		if( pDungeon  
		&&	pDungeon->m_pServer
		&&	pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
		{
			WSTDS_TEAM_MATCH packet;
			
			packet.wType = CMD_WSTDS_PREPARE_MATCH;
			
			packet.wDungeonID = wDungeonID;
			packet.byLayerIndex = byLayerIndex;
			packet.Body.PTM.match_type = match_type;

			packet.Body.PTM.match_level = match_level;
			packet.Body.PTM.player_count = player_count;
			
			packet.Body.PTM.team_count = team_count;
			
			packet.Body.PTM.dwExFlag = bIsDefault;
			packet.Body.PTM.match_time = match_time;

			g_pNet->SendToServer( pDungeon->m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		} // if(m_pServer && m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)			
	}

	void SendEndTeamMatch(const WORD wDungeonID, const BYTE byLayerIndex)
	{	// 던전 서버에게 보내줘라.
		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(wDungeonID);

		if( pDungeon  
		&&	pDungeon->m_pServer
		&&	pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
		{
			WSTDS_TEAM_MATCH packet;
			
			packet.wType = CMD_WSTDS_END_MATCH;
			
			packet.wDungeonID = wDungeonID;
			packet.byLayerIndex = byLayerIndex;
			g_pNet->SendToServer( pDungeon->m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		} // if(m_pServer && m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)					
	}

	void	LogGuildWar( T_ELEMENT* pElement , const bool IsSuccess)
	{
		if( NULL == pElement )
		{
			return;
		}

		string strLog;
	
		char szBuffer[MAX_PATH] = {0,};

		strLog += "\t";

		if( IsSuccess )
		{
			strLog += "SuccessGame\t";
		}
		else
		{
			strLog += "Canceled by User\t";
		}

		{
			strLog += "Starting Time:\t";

			::sprintf( szBuffer, "%d/%d/%d %d:%d:%d\t", pElement->m_PrepareTime.wYear, pElement->m_PrepareTime.wMonth, 
														pElement->m_PrepareTime.wDay, pElement->m_PrepareTime.wHour, 
														pElement->m_PrepareTime.wMinute, pElement->m_PrepareTime.wSecond);

			strLog += szBuffer;
			strLog += "Ending Time:\t";

			SYSTEMTIME	st;
			GetLocalTime( &st );
			::sprintf( szBuffer, "%d/%d/%d %d:%d:%d\t", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

			strLog += szBuffer;

			strLog += "MatchType:\t";

			switch( pElement->match_type )
			{
			case M_T_NONE:				{strLog += "None";}break;
			case M_T_DEAH_MATCH:		{strLog += "Death Match";}break;
			case M_T_KING_OF_THE_HILL:	{strLog += "King Of The Hill";}break;
			case M_T_CAPTURE_THE_FLAG:	{strLog += "Capture The Flag";}break;
			case M_T_LEADER_KILL:		{strLog += "Leader Kill";}break;
			case M_T_KILLING_FIELD:		{strLog += "Killing Filed";}break;
			case M_T_ALL:				{strLog += "All";}break;
			}

			strLog += "\t";

			strLog += "MatchLevel:\t";

			switch( pElement->match_level )
			{
			case M_L_NONE:		{strLog += "None";	 }break;
			case M_L_HIGH:		{strLog += "High";	 }break;
			case M_L_MIDDLE:	{strLog += "Middle"; }break;
			case M_L_LOW:		{strLog += "Low";	 }break;
			}

			strLog += "\t";

			strLog += "MatchMemberCount:\t";
			
			
			sprintf( szBuffer, "%d", pElement->player_count );
			
			strLog += szBuffer;

			strLog += "\t";

			T_ELEMENT::TEAM_HASH&	rHash = pElement->TeamHash();

			T_ELEMENT::TEAM_HASH::iterator	iter_begin = rHash.begin();
			T_ELEMENT::TEAM_HASH::const_iterator	iter_end = rHash.end();

			DWORD dwGuildCount = 0;

            while( iter_end != iter_begin )
			{
//				if( pTable )
				{
					switch( (*iter_begin).second.team_type)
					{
					case T_T_NONE:		{ strLog += "NONE"; }break;
					case T_T_GUILD:		{ strLog += "Guild"; }break;
					case T_T_S_GUILD:	{ strLog += "S_Guild"; }break;
					case T_T_RANDOM:	{ strLog += "RandomTeam"; }break;
					}

					::sprintf( szBuffer, "%d", dwGuildCount);
					
					strLog += szBuffer;
					strLog += "Name:\t";

					GUILD_TABLE* pTable = g_pGuildTableHash->GetData( (*iter_begin).second.dwGuildID );
					if( pTable )
					{
						strLog += pTable->sGuild.szGuildName;
					}
					else
					{
						strLog += "RandomTeam";
					}

					strLog += "\t";
					
					switch( (*iter_begin).second.team_type)
					{
					case T_T_NONE:		{ strLog += "NONE"; }break;
					case T_T_GUILD:		{ strLog += "Guild"; }break;
					case T_T_S_GUILD:	{ strLog += "S_Guild"; }break;
					case T_T_RANDOM:	{ strLog += "RandomTeam"; }break;
					}

					::sprintf( szBuffer, "%d", dwGuildCount);
					strLog += szBuffer;

					strLog += "Point:\t";
					
					::sprintf( szBuffer, "%d", (*iter_begin).second.point);
					strLog += szBuffer;

					strLog += "\t";
					
					++dwGuildCount;

					PLAYER_HASH::iterator	iter_player_begin = (*iter_begin).second.mapPlayer.begin();
					PLAYER_HASH::const_iterator	iter_player_end = (*iter_begin).second.mapPlayer.end();

					DWORD dwUserCount = 0;
					while( iter_player_begin != iter_player_end )
					{
						strLog += "User";
	
						::sprintf( szBuffer, "%d", dwUserCount);
						strLog += szBuffer;
	
						strLog += "Name:\t";
						strLog += (*iter_player_begin).first;
						strLog += "\t";

						++iter_player_begin;
						++dwUserCount;
					}
				}

				++iter_begin;
			}				


		} // if( file.is_open() )
		
		g_GuildWarLog.Log(LOG_LV1, strLog.c_str() );
	}

	void ModifyStadiumState(T_ELEMENT &rElement)
	{
		const FLAG_TYPE match_type = rElement.match_type;
		STADIUM_LIST *pList = m_hash.Find(match_type);
		if(!pList)
		{
			m_hash.insert( make_pair( match_type, STADIUM_LIST() ) );

			pList = m_hash.Find( match_type);
			if(!pList)
			{
				assert( NULL && "Can't Create StadiumList");
			}

		}

		T_ELEMENT *pElement = pList->Find(rElement);
		if( pElement )
		{
			(*pElement) = rElement;//있으면 상태 변경

			if( (*pElement).byState == TMS_PREPARE)
			{
				GetLocalTime( &(*pElement).m_PrepareTime );
			}
		}
		else
		{
			if( rElement.byState == TMS_PREPARE)
			{
				GetLocalTime( &rElement.m_PrepareTime );
			}
			pList->push_back(rElement);//없으면 PushBack
		}
	}
	
	void RemoveStadium(const WORD wDungeonID)
	{
		typedef CHS_LIST< T_ELEMENT > STADIUM_LIST;
		typedef CHS_HASH< FLAG_TYPE, STADIUM_LIST > STADIUM_HASH;//매치타입을 가진 맵
		
		STADIUM_HASH::iterator hash_itor = m_hash.begin();

		while( m_hash.end() != hash_itor)
		{
			STADIUM_LIST &rList = (*hash_itor).second;
			
			STADIUM_LIST::iterator list_itor = rList.begin();

			while( rList.end() != list_itor)
			{
				T_ELEMENT &rElement= (*list_itor);
				
				if(rElement.wDungeonID == wDungeonID)
				{
					Log(LOG_NORMAL, "@ ------- Stadium Removed ID=[%d]", wDungeonID);
					list_itor = rList.erase( list_itor );
				} // if(rSD.dwDungeonID == dwDungeonID)
				else
				{
					++list_itor;
				}
			} // while( rList.end() != list_itor)
			++hash_itor;
		}
	}
private:
	STADIUM_HASH m_hash;
};