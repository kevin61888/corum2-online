#include "stdafx.h"
#include "ServerFunction.h"
#include "ServerTable.h"
#include "WorldUserTable.h"
#include "recvmsg.h"
#include "GuildHash.h"
#include "PartyHash.h"
#include "MessengerHash.h"
#include "AcceptTable.h"
#include "DungeonTable.h"
#include "EventDungeonManager.h"
#include "Map.h"
#include "ConnectionDistributionTable.h"
#include <afxdisp.h>
#include "GuildHash.h"
#include "Guild.h"
#include "GlobalDefine.h"
#include "SchoolGuildDungeon.h"
#include "GuildMatcher.h"
#include "GuildMatchHandler.h"
#include "StadiumMgr.h"

#include "../Commonserver/RivalGuildWarPacket.h"

#include "PartyMatcher.h"

extern void Recv_CTWS_TM_INSERT(CWorldUser* pUser, char* pMsg, DWORD dwLength);
extern void Recv_CTWS_TM_REMOVE(CWorldUser* pUser);

const int MAX_RANDOM_TEAM_COUNT = 2;

CGuildMatcher	g_GuildMatcher;
CStadiumMgr< CStadium >	 g_StadiumMgr;

void __stdcall WorldMatchProc(DWORD dwVal)
{
	g_GuildMatcher.HandleEvent(); // 길드전 안일어나니까 우선 주석
	g_PartyMatcher.Process(20); // 파티 자동 매칭 : 최덕석 2005.2.15

	static SYSTEMTIME PrevTime;

	SYSTEMTIME NowTime;
	GetLocalTime(&NowTime);

	//9시 10시 11시 에 메세지 보내기 ( 길드전 참여 하라고 ) 
	if( (PrevTime.wHour == 20 || PrevTime.wHour == 21 || PrevTime.wHour == 22 ) //9 ~ 11 시 에 방송 하라고
	&&	(NowTime.wHour == PrevTime.wHour+1 ) )	
	{	
		GUILD_WAR_BBS	packet;
		g_pUserTable->BroadCast((char*)&packet, packet.GetPacketSize() );
	}
	PrevTime = NowTime;
}


CWorldUser* GetCanMatchGuildLeader(const DWORD dwGuildID, const BYTE bLimitRank)//: 050202 hwoarang 길마 외에 2등급까지 받는다
{
	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData( dwGuildID );		// 신청한 길드

	ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();
	
	CWorldUser*	pMasterUser	= NULL;

	while ( pGuildUserPos ) 
	{
		pMasterUser = pGuildUserPos->pData;

		if ( pMasterUser ) 
		{
			DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pMasterUser->m_dwDungeonID);//던전이 있는놈이어야지

			if( pMasterUser->m_byRank && pMasterUser->m_byRank <= bLimitRank ) 
			{
				if( pDungeon &&  !g_StadiumMgr.GetUserStadium( pMasterUser->m_szCharacterName ) )
				{
					Log ( LOG_NORMAL, "IsThereGuildMaster() Pointer : %p", pMasterUser );
					return pMasterUser;
				}
			}
		}
		else
		{
			return NULL;
		}
		pGuildUserPos = pGuildUserPos->pNext;
	}

	return NULL;
}

void BroadCastGuildUser(const DWORD dwGuildID, char* pPacket, const DWORD dwLen)//: 050202 hwoarang 길마 외에 2등급까지 받는다
{
	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData( dwGuildID );		// 신청한 길드

	ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();
	
	CWorldUser*	pGuildUser	= NULL;

	while ( pGuildUserPos ) 
	{
		pGuildUser = pGuildUserPos->pData;

		if( pGuildUser ) 
		{
			DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo( (WORD)pGuildUser->m_dwDungeonID );//던전이 있는놈이어야지

			if( pDungeon )//경기장이 아니어야 한다.
			{
				g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, pPacket, dwLen, FLAG_SEND_NOT_ENCRYPTION);
			}
		}
		else
		{
			return;
		}
		pGuildUserPos = pGuildUserPos->pNext;
	}
	return;
}

FLAG_TYPE GetRandomMatchType()
{
	const FLAG_TYPE aMatchType[] = 
		{
			M_T_DEAH_MATCH,	
			M_T_KING_OF_THE_HILL,	
			M_T_CAPTURE_THE_FLAG,	
			M_T_LEADER_KILL, 
			M_T_KILLING_FIELD
		};

	const size_t array_count = sizeof(aMatchType)/sizeof(FLAG_TYPE);

	const int iIndex = rand()%array_count;//0 되면 곤란

	return aMatchType[iIndex];
}

COUNT_TYPE GetRandomBasePlayerCount()
{
	const COUNT_TYPE base_array_count = sizeof( g_aBasePlayerCount )/sizeof(COUNT_TYPE);//0 되면 곤란

	const int iIndex = rand()%base_array_count;

	return g_aMatchTeamCount[iIndex];
}


template< typename T_IN_TYPE >
FLAG_TYPE ConvertMatchType( const T_IN_TYPE InType )
{
	switch(InType)
	{
	case M_T_DEAH_MATCH:
	case M_T_KING_OF_THE_HILL:
	case M_T_CAPTURE_THE_FLAG:
	case M_T_LEADER_KILL:
	case M_T_KILLING_FIELD:
		{
			return static_cast<FLAG_TYPE>(InType);
		}break;

	case M_T_ALL:
		{
			return GetRandomMatchType();
		}break;
	} // switch(InType)

	
	return M_T_NONE;
}

FLAG_TYPE GetTeamType(const CWorldUser* pUser)
{
	if(!pUser)
	{
		return T_T_NONE;
	}

	if(0 != pUser->m_dwGuildId )
	{
		return T_T_GUILD;
	}

	return T_T_RANDOM;
}

FLAG_TYPE GetMatchLevel(const CWorldUser* pUser)
{
	if(!pUser)
	{
		return M_L_NONE;
	}

	if( pUser->m_dwLevel > 60 )
	{
		return M_L_HIGH;
	}

	return M_L_LOW;
}

//COUNT_TYPE GetTeamIndex(const CWorldUser* pUser)
//{
//	if( !pUser->m_dwGuildId )
//	{
//		return rand()%MAX_RANDOM_TEAM_INDEX;//길드 아이디가 없는 애들은 랜덤하게 팀 인덱스를 가짐
//	}
//	return pUser->m_dwGuildId;
//}

void MakeGMC(CWorldUser* pUser, const FLAG_TYPE match_type, const FLAG_TYPE team_type = T_T_NONE, const bool bDoInsert = true)
{
	const FLAG_TYPE	eMT = ConvertMatchType(match_type);//ALL타입은 타입쪼갬(버퍼링 되지 않는다는 의미
	FLAG_TYPE	eML	= GetMatchLevel(pUser);
	
	FLAG_TYPE	eTT	= GetTeamType(pUser);
	
	if(	T_T_S_GUILD == team_type 
	&& T_T_GUILD	== eTT )//길드가 있고
	{
		eTT = T_T_S_GUILD;
		eML = M_L_HIGH;//도전형 길드전은 무조건 매치 레벨이 High이다
	}
	
	if( T_T_RANDOM == team_type )
	{
		eTT = T_T_RANDOM;
	}

	if(	eMT == M_T_NONE
	||	eTT == T_T_NONE )
	{
		Log(LOG_ALL, "@Recv_CTWS_TM_INSERT Illigal Type!");
		return;
	}

	G_MC_DEFAULT GMD;

	CMatchCondition *pMC = const_cast<CMatchCondition*>(&GMD.First());
	CTeamInfo *pTI = const_cast<CTeamInfo*>(&GMD.Second());
	
	pMC->MatchType(eMT);
	pMC->MatchLevel(eML);//레벨 뽑기.

	pTI->TeamType(eTT);//팀타입 셋팅
//	pTI->TeamIndex( GetTeamIndex(pUser) );
	
//	pTI->TeamType(T_T_GUILD);//팀타입 셋팅

	const int randomteamindex = rand()%MAX_RANDOM_TEAM_COUNT;
	if( eTT == T_T_RANDOM )
	{
		pTI->TeamIndex( randomteamindex );
	}
	else
	{
		pTI->TeamIndex( pUser->m_dwGuildId );
	}
	
	pUser->m_GMC = GMD;//유저에 저장

	if( bDoInsert)
	{
		CPlayerInfo Player( pUser );
		g_GuildMatcher.Insert( GMD, Player );
		Log(LOG_ALL, "@Recv_CTWS_TM_INSERT name[%s] match_type[%d], match_level[%d], team_type[%d], team_index[%d]", pUser->m_szCharacterName, eMT, eML, eTT, pTI->TeamIndex() );
	}
}

void Recv_CMD_TEAM_MATCH(CWorldUser* pCaster, char* pMsg, DWORD dwLength)
{//유저로 부터 받았다.
	TEAM_MATCH_PACKET* pPacket = reinterpret_cast<TEAM_MATCH_PACKET*>(pMsg);
	
	const WORD wType = pPacket->wType;
	switch(wType)
	{
	case CTWS_TM_INSERT:
		{
			Recv_CTWS_TM_INSERT(pCaster, pMsg, dwLength);
		}
		break;
	case CTWS_TM_REMOVE:
		{
			Recv_CTWS_TM_REMOVE(pCaster);
		}
		break;
	case CTWS_TM_LADDER_LIST:
		{
			// 길드 레더 정보를 클라이언트로 전송한다.
			TCHAR	szQuery[0xFF];
			::wsprintf( szQuery, _T("EXEC SP_GET_GUILD_LADDER_INFO %u, %u"), pPacket->Body.m_GuildLadderInfoPage.m_uCurPage, pCaster->m_dwGuildId );
			g_pDb->THOpenRecord( szQuery, QUERY_TYPE_LADDER_INFO, reinterpret_cast< VOID* >(pCaster->m_dwConnectionIndex), 20, static_cast< BYTE>(GAME_DB) );
		}
		break;
	case CTWS_TM_GUILD_WAR_LIST:
		{//길드전 리스트 전송
			size_t cur_page = pPacket->Body.PageNum.iPageNumber;
//
			TEAM_MATCH_PACKET packet;
			packet.wType = CTWS_TM_GUILD_WAR_LIST;
			GUILD_WAR_DATA_LIST &rGWDL = packet.Body.GuildWarList;
			g_StadiumMgr.MakeGuildWarList(rGWDL, cur_page);
			g_pNet->SendToUser(pCaster->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}break;
	case CTWS_TM_GUILD_WAR_REQUEST:
		{
			pCaster->m_bGuildWarRequestApply = FALSE;
			CWorldUser* pTarget = g_pUserTable->GetUserInfoByName(pPacket->Body.GuildWarRequest.szTargetName);

			if(!pTarget)
			{
				break;
			}

			if( !pCaster->m_dwGuildId 
			||	!pTarget->m_dwGuildId )
			{//길드 ID 가 없거나
				break;
			}

			if( pCaster->m_dwGuildId == pTarget->m_dwGuildId )
			{//같은 길드라면
				break;
			}

			switch(pPacket->Body.GuildWarRequest.Sequence)
			{
			case GWRS_NONE:
				{

					CWorldUser* pGuildMaster = GetCanMatchGuildLeader( pTarget->m_dwGuildId, 2 );
					
					if( pTarget->m_byRank &&  pTarget->m_byRank <= 2 )
					{
						pGuildMaster = pTarget;
					}

					if(pGuildMaster)//길마 찾았다.
					{
						TEAM_MATCH_PACKET SendPacket;
						SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
						SendPacket.Body.GuildWarRequest.Sequence = GWRS_NONE;
						SendPacket.Body.GuildWarRequest.MatchType = M_T_NONE;
						SendPacket.Body.GuildWarRequest.MatchTime = GWT_TEN;
						SendPacket.Body.GuildWarRequest.MatchNumber = GWN_SIX;
						
						::strcpy( SendPacket.Body.GuildWarRequest.szTargetName, pGuildMaster->m_szCharacterName );

						//SendPacket.Body.GuildWarRequest.LadderData	// 상대방 길드의 래더 데이타를 넣는 부분
						ZeroMemory(&SendPacket.Body.GuildWarRequest.LadderData, sizeof(SendPacket.Body.GuildWarRequest.LadderData));
						g_pNet->SendToUser(pCaster->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						::Log(LOG_ALL, "GWRS_NONE GuildMaster GuildID[%d], MasterName[%s]", pTarget->m_dwGuildId, pGuildMaster->m_szCharacterName);
					}
					else
					{
						TEAM_MATCH_PACKET SendPacket;
						SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
						SendPacket.Body.GuildWarRequest.Sequence = GWRS_REJECT;
						SendPacket.Body.GuildWarRequest.MatchType = M_T_NONE;//임시로 쓰자.. 길마가 아니어서 리젝트될때
						SendPacket.Body.GuildWarRequest.MatchTime = GWT_TEN;
						SendPacket.Body.GuildWarRequest.MatchNumber = GWN_SIX;
						//SendPacket.Body.GuildWarRequest.LadderData	// 상대방 길드의 래더 데이타를 넣는 부분
						ZeroMemory(&SendPacket.Body.GuildWarRequest.LadderData, sizeof(SendPacket.Body.GuildWarRequest.LadderData));
						g_pNet->SendToUser(pCaster->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);


						::Log(LOG_ALL, "GWRS_NONE Can't Find GuildMaster GuildID[%d]", pTarget->m_dwGuildId);
					}
				}
				break;
			case GWRS_REQUEST:
			case GWRS_REREQUEST:
				{
					if( (pCaster->m_byRank == 1 || pCaster->m_byRank == 2)
					&&	(pTarget->m_byRank == 1 || pTarget->m_byRank == 2) )
					{
						memcpy(&pCaster->m_RequestGuildWarRequest, &pPacket->Body.GuildWarRequest, sizeof(pPacket->Body.GuildWarRequest));
						TEAM_MATCH_PACKET SendPacket;
						SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
						memcpy(&SendPacket.Body.GuildWarRequest, &pPacket->Body.GuildWarRequest, sizeof(pPacket->Body.GuildWarRequest));
						//SendPacket.Body.GuildWarRequest.LadderData //자기 길드의 래더 데이타 넣는 부분
						ZeroMemory(&SendPacket.Body.GuildWarRequest.LadderData, sizeof(SendPacket.Body.GuildWarRequest.LadderData));
						memcpy(&SendPacket.Body.GuildWarRequest.LadderData, &pPacket->Body.GuildWarRequest.LadderData, sizeof(pPacket->Body.GuildWarRequest.LadderData));
						g_pNet->SendToUser(pTarget->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
					else
					{
						Log(LOG_ALL, "GWRS_REQUEST Incorrect GuildRank  Caster[Rank%d], Target[Rank%d]", pCaster->m_byRank,  pTarget->m_byRank);
					}

				}
				break;
			case GWRS_APPLY:
				{
					if( (pCaster->m_byRank == 1 || pCaster->m_byRank == 2)
					&&	(pTarget->m_byRank == 1 || pTarget->m_byRank == 2) 
					&&	!g_StadiumMgr.GetUserStadium( pCaster->m_szCharacterName ) //경기장이 있는 놈이면 안된다 .
					&&	!g_StadiumMgr.GetUserStadium( pTarget->m_szCharacterName ) 
					)
					{
						GUILD_WAR_REQUEST &rGWR = pPacket->Body.GuildWarRequest;

						const FLAG_TYPE match_type	= rGWR.MatchType;
						const FLAG_TYPE match_level = M_L_NONE;
						const COUNT_TYPE max_player = rGWR.MatchNumber;
						const GUILD_WAR_TIME match_time = rGWR.MatchTime;

						size_t count = g_StadiumMgr.EmptyStadiumCount( match_type );
						if(	(0 < count )
						&&	(0 != pCaster->m_dwGuildId)
						&&	(0 != pTarget->m_dwGuildId)
						&&	(pCaster->m_dwGuildId != pTarget->m_dwGuildId) )
						{
							CStadium *pStadium = g_StadiumMgr.PreparedStadium( match_type, match_level, MIN_MATCH_TEAM, max_player, match_time, false);

							DUNGEON_MOVE_INFO DMI;

							DMI.wDungeonID = pStadium->wDungeonID;
							DMI.byLayerIndex = pStadium->byLayerIndex;
						
							MakeGMC(pCaster, match_type, T_T_S_GUILD, true);
							MakeGMC(pTarget, match_type, T_T_S_GUILD, true);
						
							
							size_t team_index_pos = 0;//경기자는 1팀 부터
							
							vector< size_t > vec_team_index;
							vec_team_index.clear();
							vec_team_index.push_back(1);//팀 인덱스. 지금은 4팀 밖에 없으니까. 이렇게 해놓는다.
							vec_team_index.push_back(2);
							vec_team_index.push_back(3);
							vec_team_index.push_back(4);
							random_shuffle( vec_team_index.begin(), vec_team_index.end() );
							
							{
							const size_t result_index = vec_team_index.at(team_index_pos);
							
							DMI.wX = static_cast< WORD >(pStadium->aPos[result_index].x);
							DMI.wY = static_cast< WORD >(pStadium->aPos[result_index].y);

							STADIUM_PLAYER S_P(match_type, DMI, result_index, G_W_F_MASTER, false);
							pStadium->InsertUser( pCaster, S_P );//경기장에 등록
							MoveToStadium( pCaster, DMI, result_index, G_W_F_MASTER);//얘는 마스터가 된다

							::Log(LOG_ALL, "GWRS_APPLY Name[%s] Type[%d] TeamIndex[%d] MovePos = %d %d", pCaster->m_szCharacterName, match_type, result_index, pStadium->aPos[result_index].x, pStadium->aPos[result_index].y);
							}
							++team_index_pos;
							{
							const size_t result_index = vec_team_index.at(team_index_pos);
							DMI.wX = static_cast< WORD >(pStadium->aPos[result_index].x);
							DMI.wY = static_cast< WORD >(pStadium->aPos[result_index].y);
							
							STADIUM_PLAYER S_P(match_type, DMI, result_index, G_W_F_MASTER, false);
							pStadium->InsertUser( pTarget, S_P );//경기장에 등록
							MoveToStadium( pTarget, DMI, result_index, G_W_F_MASTER);//얘는 마스터가 된다
							
							::Log(LOG_ALL, "GWRS_APPLY Name[%s] Type[%d] TeamIndex[%d] MovePos = %d %d", pTarget->m_szCharacterName, match_type, result_index, pStadium->aPos[result_index].x, pStadium->aPos[result_index].y);
							}
						}
					}
				}break;
			case GWRS_REJECT:
				{
					TEAM_MATCH_PACKET SendPacket;
					SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
					SendPacket.Body.GuildWarRequest.Sequence = GWRS_REJECT;
					SendPacket.Body.GuildWarRequest.MatchType = M_T_ALL;//임시로 쓰자.. 그냥 거절일때
					SendPacket.Body.GuildWarRequest.MatchTime = GWT_TEN;
					SendPacket.Body.GuildWarRequest.MatchNumber = GWN_SIX;
					//SendPacket.Body.GuildWarRequest.LadderData	// 상대방 길드의 래더 데이타를 넣는 부분
					ZeroMemory(&SendPacket.Body.GuildWarRequest.LadderData, sizeof(SendPacket.Body.GuildWarRequest.LadderData));
					g_pNet->SendToUser(pTarget->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				break;
			}

			if( pCaster->m_bGuildWarRequestApply && pTarget->m_bGuildWarRequestApply )
			{
				TEAM_MATCH_PACKET SendPacket;
				SendPacket.wType = CTWS_TM_GUILD_WAR_REQUEST;
				SendPacket.Body.GuildWarRequest.Sequence = GWRS_END;
				g_pNet->SendToUser(pCaster->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				g_pNet->SendToUser(pTarget->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				//길드전 신청
				//pCaster->m_RequestGuildWarRequest 에 해당 데이타가 들어있음
			}
		}
		break;
	case CTWS_TM_GUILD_WAR_OBSERVE:
		{	
//			int i = 0;
			GUILD_WAR_OBSERVE &rGWO = pPacket->Body.ObserveIndex;
			//관전모드
			
			CStadium *pStadium = g_StadiumMgr.GetStadium( rGWO.match_type, rGWO.wDungeonID, rGWO.byLayerIndex );
			if( pStadium )
			{
				if( TMS_RUN == pStadium->byState )
				{
					DUNGEON_MOVE_INFO DMI;
					DMI.wDungeonID		= rGWO.wDungeonID;
					DMI.byLayerIndex	= rGWO.byLayerIndex;
					DMI.wX				= static_cast< WORD >(pStadium->aPos[0].x);
					DMI.wY				= static_cast< WORD >(pStadium->aPos[0].y);

//					STADIUM_PLAYER S_P(rGWO.match_type, DMI, 0, G_W_F_OBSERVER, false);
//					pStadium->InsertUser( pCaster, S_P );//경기장에 등록
					//옵저버는 경기장에 등록 하면 안되는데.
					MoveToStadium( pCaster, DMI, 0, G_W_F_OBSERVER, false);
				}				
			} // if( pStadium )
			else
			{
				::Log(LOG_ALL, "CTWS_TM_GUILD_WAR_OBSERVE MatchType[%d] DungeonID[%d] LayerIndex[%d]", rGWO.match_type, rGWO.wDungeonID, rGWO.byLayerIndex);
			}
		}break;
	case CTWS_TM_GUILD_WAR_OBSERVE_CANCEL:
		{
			STADIUM_PLAYER *pSP = g_StadiumMgr.GetUserStadium( pCaster->m_szCharacterName );
			if(pSP)
			{
				MoveToPrevDungeon(pCaster, pSP->DMI.wDungeonID, false);
			}
		}break;
	}
}

// 전병렬
// 길드관룐 셋팅에 관한 정보가 요기에 다있다.
void CmdGuildMatchBeginSetting(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{// T_T_S_GUILD 일 경우
	GUILD_MATCHBEGIN_SETTING*	pPacket = reinterpret_cast< GUILD_MATCHBEGIN_SETTING* >(pMsg);

	GUILD_MATCHBEGIN_SETTING send_packet;

	switch( pPacket->eType )
	{
	case GMS_TYPE_REQUEST_MEMBER_LIST:
		{	// 멤버리스트 보내줘
			GUILD_MATCHBEGIN_MEMBER_LIST &rGMML = send_packet.Body.m_MemberList;
			ZeroMemory( &rGMML, sizeof(rGMML) );
			
			send_packet.eType = GMS_TYPE_MEMBER_LIST;

			CGuildMatchHandler *pHandler = g_GuildMatcher.GetSTypeHandler( pUser->m_GMC );

			if(pHandler)
			{
				const size_t total_size = pHandler->List().size();
				if( total_size )
				{//사이즈 있으면 있는대로 보낸다.
					CGuildMatchHandler::ELEMENT_LIST &rList = pHandler->List();
					CGuildMatchHandler::ELEMENT_LIST::iterator itor =  rList.begin();
					
					size_t temp_count = 0;
					while ( rList.end() != itor)
					{
						STADIUM_PLAYER *pSP = g_StadiumMgr.GetUserStadium( (*itor).Name() );
						rGMML.m_aMemberList[temp_count].byCalled = ((pSP)?1:0);

						rGMML.m_aMemberList[temp_count].byClass = (*itor).Class();
						strcpy( rGMML.m_aMemberList[temp_count].szName, (*itor).Name() );
						rGMML.m_aMemberList[temp_count].wLevel = static_cast< WORD >((*itor).LV());

						++temp_count;
						++itor;

						if(	MAX_MEMBER_LIST == temp_count
						||	rList.end() == itor)//마지막이면 보낸다
						{
							if( rList.end() == itor )
							{
								rGMML.m_wIsEnd = true;
							}
							else
							{
								rGMML.m_wIsEnd = false;
							}

							rGMML.m_wListSize = static_cast< WORD >(temp_count);

							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&send_packet, send_packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							temp_count = 0;//초기화
						}
					}
				} // if( total_size )
				else
				{//인원이 없다
					rGMML.m_wIsEnd = true;
					rGMML.m_wListSize = 0;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&send_packet, send_packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}break;
	case GMS_TYPE_REQUEST_LADDER_INFO:
		{
		// GUILD_MATCHBEGIN_LADDER_INFO	m_LadderInfo;
		// 레더 정보 보내줘.
			CStadium *pStadium= g_StadiumMgr.GetStadium( pUser );
			if(pStadium)
			{
				pStadium->SendLadderInfoToUser( pUser );
			}
		}break;
	case GMS_TYPE_REQUEST_USER_CALL:
		// GUILD_MATCHBEGIN_USER_CALL		m_UserCall;
		// 유저 소환해줘
		{
			char *szName = pPacket->Body.m_UserCall.szName;
			CWorldUser* pTargetUser = g_pUserTable->GetUserInfoByName( szName );

			CGuildMatchHandler *pHandler = g_GuildMatcher.GetSTypeHandler( pUser->m_GMC );
			if(pHandler)
			{
				CPlayerInfo Player(pTargetUser);
				CPlayerInfo *pPlayer = pHandler->List().Find(Player);
				if(pPlayer)//핸들러에 있는 유저
				{
					STADIUM_PLAYER *pSP =  g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName );//콜 한사람이 가진 스타디움
					
					STADIUM_PLAYER *pTargetSP =  g_StadiumMgr.GetUserStadium( pTargetUser->m_szCharacterName );//소환 당할 사람이 가진 스타디움

					if(pSP && !pTargetSP)//소환자는 경기장이 있어야 하고, 소환 될 사람은 경기장에 있어서는 안된다.
					{
						CStadium *pStadium = g_StadiumMgr.GetStadium( pUser->m_GMC.First().MatchType(), pSP->DMI.wDungeonID, pSP->DMI.byLayerIndex);
						
						if( pStadium )//경기장을 받아서
						{
							STADIUM_PLAYER S_P( pStadium->match_type, pSP->DMI, pSP->team_index, G_W_F_PLAYER, false);
							pStadium->InsertUser( pTargetUser, S_P );//경기장에 등록
							MoveToStadium(pTargetUser, pSP->DMI, pSP->team_index, G_W_F_PLAYER, false);//call 하는 애들은 지우지 않음
						}
					}
				}
			}
		}break;
	case GMS_TYPE_REQUEST_USER_KICK:
		// GUILD_MATCHBEGIN_USER_CALL		m_UserCall;
		// 유저 킥해줘
		{ //Call 된 애들은 Master가 지우기 전까지는 Stadium 에서 Remove 되지 않는다! 
			char *szName = pPacket->Body.m_UserCall.szName;
			CWorldUser* pTargetUser = g_pUserTable->GetUserInfoByName( szName );
			
			CGuildMatchHandler *pHandler = g_GuildMatcher.GetSTypeHandler( pUser->m_GMC );
			if(pHandler)
			{
				CPlayerInfo Player(pTargetUser);
				CPlayerInfo *pPlayer = pHandler->List().Find(Player);
				if(pPlayer)
				{
					STADIUM_PLAYER *pSP =  g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName );
					CStadium *pStadium = g_StadiumMgr.GetStadium( pUser->m_GMC.First().MatchType(),pSP->DMI.wDungeonID, pSP->DMI.byLayerIndex );
					if(pStadium)
					{
						pStadium->RemoveUser(pTargetUser);//call 하는 애들은 지우지 않음
						MoveToPrevDungeon( pTargetUser, pStadium->wDungeonID, false);
					}
				}
			}
		}break;
	case GMS_TYPE_REQUEST_SET_READY:
		{// 레디 하기.
		}break;
	case GMS_TYPE_REQUEST_RESET_READY:
		{// 레디 풀기.
		}break;
	case GMS_TYPE_REQUEST_CANCEL:
		{// 경기 취소.
			STADIUM_PLAYER *pPlayer = g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName );
			if(pPlayer)
			{
				g_StadiumMgr.SendEndTeamMatch(pPlayer->DMI.wDungeonID,pPlayer->DMI.byLayerIndex);
				g_StadiumMgr.LogGuildWar( g_StadiumMgr.GetStadium( pUser ) , false);
			}
		}break;
	case GMS_TYPE_REQUEST_INVITATION:
		{	// 작업필요
			GUILD_WAR_INVITE_PACKET packet;
			BroadCastGuildUser(pUser->m_dwGuildId, (char*)&packet, packet.GetPacketSize() );
		}break;

	} // switch( pPacket->eType )
}

void CmdGuildMatchStatusRequest(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	GUILD_MATCH_STATUS_REQUEST *pPacket = reinterpret_cast< GUILD_MATCH_STATUS_REQUEST* >(pMsg);

	CStadium *pStadium = NULL;
		 
	const FLAG_TYPE ret = pPacket->dwGuildWarFlag & G_W_F_CMP_BASE;
	switch(ret) 
	{
	case G_W_F_OBSERVER:
		{
			pStadium = g_StadiumMgr.GetStadium( pPacket->match_type, pPacket->wDungeonID, pPacket->byLayerIndex );
		}break;
	case G_W_F_PLAYER:
	case G_W_F_MASTER:
		{
			pStadium = g_StadiumMgr.GetStadium( pUser );
		}break;
	case G_W_F_NONE:
	default:
		{
			::Log(LOG_ALL, "CmdGuildMatchStatusRequest Incorrect GuildWarFlag[%d]",ret);
			return;
		}break;
	} // switch(pPacket->dwGuildWarFlag  & G_W_F_CMP_BASE)

	if( pStadium )
	{
		pStadium->SendTeamInfoToUser(pUser);
	}
}

void CmdGuildMatchCancelObserver(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{//던전 아이디

	if(! g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName ) )
	{
		GUILD_MATCH_CANCEL_OBSERVER *pPacket = reinterpret_cast< GUILD_MATCH_CANCEL_OBSERVER* >(pMsg);

		if( DUNGEON_TYPE_MATCH_STATIDUM  == DungeonIDtoType(pPacket->wDungeonID))
		{
			MoveToPrevDungeon( pUser, pPacket->wDungeonID, false);
		}
		else
		{
			::Log(LOG_ALL, "!! CmdGuildMatchCancelObserver There is Not Stadium DungeonID[%d]", pPacket->wDungeonID);
		}
	} // if(! g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName ) )
	else
	{
		::Log(LOG_ALL, "!! CmdGuildMatchCancelObserver User is Player");
	}
}

bool CheckHaveTeamMatch(CWorldUser* pUser)
{
	STADIUM_PLAYER *pSP = g_StadiumMgr.GetUserStadium( pUser->m_szCharacterName );
	if( pSP )//이미 게임 하던놈이었으면
	{// 맵이동 
		Log(LOG_ALL, "! CheckHaveTeamMatch User Have Game name[%s]", pUser);
		CStadium *pStadium = g_StadiumMgr.GetStadium( pUser );
		if(!pStadium->bIsAutoMatch)
		{// 선전포고형은 나갔다 오면 자동 등록 해준다.
			if( pUser->IsHaveGMC() )//뭔가 등록 되어 있다면 지우고
			{
				Recv_CTWS_TM_REMOVE(pUser);
			}
			MakeGMC(pUser, pSP->match_type, T_T_S_GUILD);//GMC를 만든다
		}

		MoveToStadium( pUser, pSP->DMI, pSP->team_index );
		return true;
	} // if( pSP )//이미 게임 하던놈이었으면
	return false;
}

void Recv_CTWS_TM_INSERT(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	TEAM_MATCH_PACKET* pPacket = reinterpret_cast<TEAM_MATCH_PACKET*>(pMsg);
	TEAM_MATCH_CONDITION &TMC = pPacket->Body.Condtion;

	if( DUNGEON_TYPE_MATCH_STATIDUM == DungeonIDtoType(pUser->m_dwDungeonID) )
	{//길드전 맵에 있으면 Insert 컨디션 변경 안됨
		Log(LOG_ALL, "@ pUser In Stadium [%s]!", pUser->m_szCharacterName );
		return;
	} // if( DUNGEON_TYPE_MATCH_STATIDUM == DungeonIDtoType(pUser->m_dwDungeonID) )

	if(CheckHaveTeamMatch(pUser))
	{
		return;
	}
	
	if( pUser->IsHaveGMC() )
	{//이미 들어 있음
		Log(LOG_ALL, "@ Already Regist AutoMatch System!");
		return;
	}

	MakeGMC(pUser, TMC.iMatchType ,TMC.iMatchGuildType, true);//기본이니까 팀 타입이 NONE
}

void Recv_CTWS_TM_REMOVE(CWorldUser* pUser)
{
	if( pUser->IsHaveGMC() )
	{
		Log(LOG_ALL, "! Recv_CTWS_TM_REMOVE name[%s]", pUser->m_szCharacterName);
		
		CPlayerInfo Player( pUser );
		g_GuildMatcher.Remove(pUser->m_GMC, Player);
		pUser->ClearGuildWarData();//빼니까 클리어
	}
}

void Recv_CMD_STADIUM_STATE(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{//던전서버로 부터 받는다.
	DSTWS_STADIUM_STATE* pPacket = (DSTWS_STADIUM_STATE*)pMsg;

	if( pPacket->GetPacketSize() != dwLength )
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	} // if( pPacket->GetPacketSize() != dwLength )

	switch( pPacket->wType )
	{
	case STADIUM_END:
		{//유저 정리.
			STADIUM_DATA &rSD = pPacket->Body.SD;
			CStadium *pStadium = g_StadiumMgr.GetStadium(rSD.match_type, rSD.wDungeonID, rSD.byLayerIndex);
			if(pStadium)
			{
				g_StadiumMgr.LogGuildWar
					( pStadium, pPacket->bIsSuccess );
				pStadium->SendStateBroadCast();
				pStadium->ClearUser();//Clear 먼저 하면 안됩니다
			}
		}//break; //Break 일부러 뺀겁니다.
	case STADIUM_MODIFY:
		{
			Log(LOG_ALL, "@------- Stadium State Modifyed! [%d] [%d:%d]", pPacket->Body.SD.byState, pPacket->Body.SD.aPos[0].x, pPacket->Body.SD.aPos[0].y);
		
			STADIUM_DATA &rSD = pPacket->Body.SD;

			CStadium S( rSD );
			g_StadiumMgr.ModifyStadiumState( S );

			CStadium *pStadium = g_StadiumMgr.GetStadium(rSD.match_type, rSD.wDungeonID, rSD.byLayerIndex);
			if(pStadium)
			{//상태 변경시 점수 보내기
				pStadium->SendPointToUser(rSD.byState, NULL);//모두에게
				if(rSD.byState == TMS_PREPARE)//경기 준비 모드
				{//경기시작 방송
					pStadium->SendStateBroadCast();
				}
			}
		}break;
	case STADIUM_POINT:
		{
			STADIUM_TEAM_POINT &rSTP = pPacket->Body.STP;
			
			CStadium *pStadium = g_StadiumMgr.GetStadium(rSTP.match_type, rSTP.wDungeonID, rSTP.byLayerIndex);
			if(pStadium)
			{
				pStadium->dwRemainBeginTime = rSTP.dwRemainBeginTime;//시간 갱신 
				pStadium->dwRemainEndTime	= rSTP.dwRemainEndTime;
				pStadium->ModifyPoint( rSTP.team_index, rSTP.point, pStadium->byState);
			}
			else
			{
				Log(LOG_ALL, "@------- Try Modify Point where No Stadium!   Warning!!!");
			}
		}break;
	}
	
}


void Send_WSTDS_TEAM_MATCH(CWorldUser *pUser, const DUNGEON_MOVE_INFO &rDMI , const size_t team_index, const DWORD dwGuildWarFlag)
{
	WSTDS_TEAM_MATCH TM_packet;
			
	TM_packet.wType = CMD_WSTDS_REG_PLAYER;
	
	TM_packet.wDungeonID	= rDMI.wDungeonID;
	TM_packet.byLayerIndex = rDMI.byLayerIndex;

	::strcpy( TM_packet.Body.RP.szPlayerName, pUser->m_szCharacterName);
	
	TM_packet.Body.RP.team_index =	team_index;
	TM_packet.Body.RP.dwExFlag = dwGuildWarFlag;
	
	DUNGEON_DATA_EX* pDungeonStadium = g_pDungeonTable->GetDungeonInfo((WORD)rDMI.wDungeonID);//이동할 곳 즉 경기장.
	if(pDungeonStadium)
	{
		g_pNet->SendToServer( pDungeonStadium->m_pServer->dwConnectionIndex, (char*)&TM_packet, TM_packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		Log(LOG_IMPORTANT, "There Is No Stadium(Name:%s)", pUser->m_szCharacterName);
	}
}

bool MoveToStadium( CWorldUser *pUser, const DUNGEON_MOVE_INFO &rDMI , const size_t team_index, const DWORD dwGuildWarFlag , const bool bIsGMCClear)//기본적으로 플레이어다
{//샵일때는 어떻게 하냐?;;
	if(pUser)
	{//유저 이동 
		WSTDS_USER_PORTAL packet;

		packet.byGMCommand = GM_Command_Dungeon_Portal;			// enum GM_COMMAND 참조 
		packet.dwTargetToUserIndex = 0;//갈 타겟
		
		DUNGEON_MOVE_INFO &rPacketDMI = packet.DMI;

		rPacketDMI	= rDMI;//Notify된 좌표 결과로 이동
		
		packet.dwUserIndex		= pUser->m_dwUserIndex;
		
		if( G_W_F_MASTER != (G_W_F_CMP_BASE & dwGuildWarFlag) )
		{//마스터가 아닐때만 지운다. 마스터는 GMC를 써야 하므로
			if( bIsGMCClear )
			{
				pUser->ClearGuildWarData();//이동 하므로 GMC클리어
			}
		}

		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);
		if(pDungeon)
		{//던전에 있는 유저면 //선수일 경우 추가로 던전에 정보를 쏴줘야 함.
			Send_WSTDS_TEAM_MATCH(pUser, rDMI, team_index, dwGuildWarFlag);//선수 등록

			const WORD x = static_cast< WORD >((rand()%800)-400);//좌표 랜덤
			const WORD y = static_cast< WORD >((rand()%800)-400);

			rPacketDMI.wX += x;
			rPacketDMI.wY += y;

			Log(LOG_IMPORTANT, "User Move To Dungeon (Name:%s) X[%d],Y[%d]", pUser->m_szCharacterName, rPacketDMI.wX ,rPacketDMI.wY);

			g_pNet->SendToServer( pDungeon->m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return true;
		} // if(pDungeon)
		else
		{//던전에 없고 월드에 있다.
			Log(LOG_IMPORTANT, "User don't have Dungeon (Name:%s)", pUser->m_szCharacterName);
		}
	} // if(pUser)

	return false;
}

bool MoveToPrevDungeon( CWorldUser *pUser, const WORD wNowStadiumID, const bool bIsGMCClear)//기본적으로 플레이어다
{//이전 던전으로 보냄
	if(pUser)//여기다 끝났다고 메세지 보내자.
	{//유저 이동 
		WSTDS_USER_PORTAL packet;

		packet.byGMCommand = GM_Command_Dungeon_Portal;			// enum GM_COMMAND 참조 
		packet.dwTargetToUserIndex = 0;//갈 타겟
		
		DUNGEON_MOVE_INFO &rPacketDMI = packet.DMI;

		packet.dwUserIndex		= pUser->m_dwUserIndex;

		if( bIsGMCClear )
		{
			CPlayerInfo Player( pUser );
			g_GuildMatcher.Remove( pUser->m_GMC, Player);
			pUser->ClearGuildWarData();//이동 하므로 GMC클리어
		}

		rPacketDMI.wDungeonID = static_cast< WORD >(pUser->m_dwPrevDungeonId);
		rPacketDMI.byLayerIndex = 0;
		rPacketDMI.wX = 0;
		rPacketDMI.wY = 0;

		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(wNowStadiumID);

		if(pDungeon)
		{//던전에 있는 유저면 //선수일 경우 추가로 던전에 정보를 쏴줘야 함.
			g_pNet->SendToServer( pDungeon->m_pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return true;
		} // if(pDungeon)
		else
		{//던전에 없고 월드에 있다.
			Log(LOG_IMPORTANT, "User don't have Dungeon (Name:%s)", pUser->m_szCharacterName);
		}
	} // if(pUser)
	return false;
}