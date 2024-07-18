#include "stdafx.h"
#include "InitGame.h"
#include "RivalGuildWar.h"
#include "NetworkClient.h"
#include "GroupWnd.h"
#include "GuildWarRequest.h"
#include "message.h"
#include "Chat.h"
#include "GuildWarInvite.h"
#include "GuildWarStatusWnd.h"

void Cmd_CTWS_GuildWarPacket(TEAM_MATCH_PACKET TeamMatch, WORD wType)
{
	TEAM_MATCH_PACKET TMPacket;

	CGroupWnd*	pGroupWnd = CGroupWnd::GetInstance();

	TMPacket.wType = wType;

	switch(wType)
	{
	case CTWS_TM_INSERT:
	case CTWS_TM_REMOVE:
		{
			memcpy(&TMPacket.Body.Condtion, &TeamMatch.Body.Condtion, sizeof(TeamMatch.Body.Condtion));
		}
		break;

	case CTWS_TM_LADDER_LIST:
		{
			TMPacket.Body.PageNum.iPageNumber = TeamMatch.Body.PageNum.iPageNumber - 1;
			if(TMPacket.Body.PageNum.iPageNumber < 0)
			{
				TMPacket.Body.PageNum.iPageNumber = 0;
				pGroupWnd->m_nGuildWarRankListPage = 1;
			}
		}
		break;
	case CTWS_TM_GUILD_WAR_LIST:
		{
			TMPacket.Body.PageNum.iPageNumber = TeamMatch.Body.PageNum.iPageNumber;
		}
		break;

	case CTWS_TM_GUILD_WAR_REQUEST:
		{
			memcpy(&TMPacket.Body.GuildWarRequest, &TeamMatch.Body.GuildWarRequest, sizeof(TeamMatch.Body.GuildWarRequest));
		}
		break;

	case CTWS_TM_GUILD_WAR_OBSERVE:
		{
			TMPacket.Body.ObserveIndex.iObserveIndex = TeamMatch.Body.ObserveIndex.iObserveIndex;
			g_pMainPlayer->m_wDungeonID		= TMPacket.Body.ObserveIndex.wDungeonID = TeamMatch.Body.ObserveIndex.wDungeonID;
			g_pMainPlayer->m_byLayerIndex	= TMPacket.Body.ObserveIndex.byLayerIndex = TeamMatch.Body.ObserveIndex.byLayerIndex;
			g_pMainPlayer->m_match_type		= TMPacket.Body.ObserveIndex.match_type = TeamMatch.Body.ObserveIndex.match_type;
		}
	case WSTC_TM_GUILD_WAR_INFO:
		{//길드전 끝났다~
		}break;
	}

	pGroupWnd->m_bGuildWarPacketSendEnable = TRUE;//나중에 빼자(서버하고 연결이 되면)
	
	g_pNet->SendMsg((char*)&TMPacket, TMPacket.GetPacketSize(), SERVER_INDEX_WORLD);
}

void Cmd_WSTC_RivalGuildWar( char* pMsg, DWORD dwLen )
{
	TEAM_MATCH_PACKET* pPacket = (TEAM_MATCH_PACKET*)pMsg;
	CGroupWnd*	pGroupWnd = CGroupWnd::GetInstance();
	CGuildWarRequest* pGuildWarRequest = CGuildWarRequest::GetInstance();

	switch(pPacket->wType)
	{
	case CTWS_TM_INSERT:
		{
			pGroupWnd->m_bGuildWarMatchEnable = TRUE;
		}
		break;

	case CTWS_TM_REMOVE:
		{
			pGroupWnd->m_bGuildWarMatchEnable = FALSE;
		}
		break;

	case CTWS_TM_LADDER_LIST:
		{
//			memcpy( pGroupWnd->m_LadderList, pPacket->Body.LadderList, sizeof(pPacket->Body.LadderList));
			memcpy( &pGroupWnd->m_pGuildWarRankListInfoPage, &pPacket->Body.m_GuildLadderInfoPage, sizeof(pPacket->Body.m_GuildLadderInfoPage));
			pGroupWnd->m_nGuildWarRankListMaxPage = pPacket->Body.m_GuildLadderInfoPage.m_uMaxPage;
//			pGroupWnd->m_nGuildWarRankListPage = pPacket->Body.m_GuildLadderInfoPage.m_uCurPage;
			pGroupWnd->m_nGuildWarRankListTotalCount = pPacket->Body.m_GuildLadderInfoPage.m_uUseCount;
		}
		break;

	case CTWS_TM_GUILD_WAR_LIST:
		{
			memcpy( &pGroupWnd->m_GuildWarList, &pPacket->Body.GuildWarList.aGuildWarData, sizeof(pPacket->Body.GuildWarList.aGuildWarData));
			pGroupWnd->m_nGuildWarListMaxPage = pPacket->Body.GuildWarList.total_page;
			pGroupWnd->m_nGuildWarListTotalCount = pPacket->Body.GuildWarList.count_per_page;
			pGroupWnd->m_nGuildWarListPage = pPacket->Body.GuildWarList.cur_page;
		}
		break;

	case CTWS_TM_GUILD_WAR_REQUEST:
		{
			switch(pPacket->Body.GuildWarRequest.Sequence)
			{			
			case GWRS_REQUEST:
			case GWRS_REREQUEST:
				{
					pGuildWarRequest->m_nGameForm = GetFormConvert(pPacket->Body.GuildWarRequest.MatchType);
					pGuildWarRequest->m_nGameTime = GetWarTime(pPacket->Body.GuildWarRequest.MatchTime);
					pGuildWarRequest->m_nGameStaff = GetWarNumber(pPacket->Body.GuildWarRequest.MatchNumber);
					lstrcpy( pGuildWarRequest->m_szRequestName, pPacket->Body.GuildWarRequest.szCasterName );
					//pGuildWarRequest->SetRankData(pPacket->Body.GuildWarRequest.LadderData);
					pGuildWarRequest->m_bReRequestEnable = TRUE;
					pGuildWarRequest->SetActive(TRUE);
				}
				break;

			case GWRS_NONE:
				{
					pGuildWarRequest->m_nGameForm = GetFormConvert(pPacket->Body.GuildWarRequest.MatchType);
					pGuildWarRequest->m_nGameTime = GetWarTime(pPacket->Body.GuildWarRequest.MatchTime);
					pGuildWarRequest->m_nGameStaff = GetWarNumber(pPacket->Body.GuildWarRequest.MatchNumber);
					lstrcpy( pGuildWarRequest->m_szRequestName, pPacket->Body.GuildWarRequest.szTargetName );
					//pGuildWarRequest->SetRankData(pPacket->Body.GuildWarRequest.LadderData);
					pGuildWarRequest->m_bReRequestEnable = FALSE;
					pGuildWarRequest->SetActive(TRUE);
				}
				break;

			case GWRS_APPLY:
				{
					pGuildWarRequest->m_bReRequestEnable = TRUE;
					DisplayMessageAdd(g_Message[ETC_MESSAGE1398].szMessage,TEXT_COLOR_YELLOW);// "길드전맵으로 이동합니다."
				}
				break;

			case GWRS_REJECT:
				{
					pGuildWarRequest->SetActive(FALSE);
					if(pPacket->Body.GuildWarRequest.MatchType == M_T_NONE)
					{
						DisplayMessageAdd(g_Message[ETC_MESSAGE1389].szMessage, TEXT_COLOR_YELLOW); // "신청하신 길드는 현재 도전을 받을 수 없습니다."
					}
					else
					{
						DisplayMessageAdd(g_Message[ETC_MESSAGE1400].szMessage, TEXT_COLOR_YELLOW); // "길드전 도전이 거절되었습니다."
					}
				}
				break;

			case GWRS_END:
				{
					pGuildWarRequest->SetActive(FALSE);
				}
				break;
			}
		}
		break;

	case CTWS_TM_GUILD_WAR_OBSERVE:
		{
		}
		break;

	case WSTC_TM_GUILD_WAR_INFO:
		{
			char szTmp[255] = {0,};
			int iIsUse[MAX_MATCH_TEAM] ={0,};
			int iIsUseCount = 0;

			GUILD_WAR_INFO &GWI = pPacket->Body.GuildWarInfo;

			BOOL bInitMatch = FALSE;
			for( int i=1; i<MAX_MATCH_TEAM ; i++)
			{
				if(GWI.a_bIsUse[i])
				{
					iIsUse[iIsUseCount] = i;
					iIsUseCount++;
				}

				if(g_pMainPlayer->m_dwGuildId == GWI.adwGuildID[i])
				{
					bInitMatch = TRUE;
				}
			}
			if(bInitMatch)
				pGroupWnd->InitGuildWarMatching();

			if(GWI.byState == TMS_PREPARE)//시작할때
			{	
//				if( GWI.adwGuildID[ iIsUse[0] ] )
				{
					::wsprintf(szTmp, g_Message[ETC_MESSAGE1346].szMessage, 
								GWI.aszGuildName[iIsUse[0]], 0, 
								GWI.aszGuildName[iIsUse[1]], 0 );// " '%s'길드(%d위)와 '%s'길드(%d위)의 Quantum Ares가 시작됩니다."
				}
//				else
//				{
//					::wsprintf(szTmp, g_Message[ETC_MESSAGE1346].szMessage, 
//								g_Message[ETC_MESSAGE1381], 0, 
//								g_Message[ETC_MESSAGE1382], 0 );// " '%s'길드(%d위)와 '%s'길드(%d위)의 Quantum Ares가 시작됩니다."
//				}
				
				DisplayMessageAdd(szTmp, 0xFFFFFF00);
			}
			else if(pPacket->Body.GuildWarInfo.byState == TMS_EMPTY)//끝날때
			{
				int iWinner = 0;
				int iLoser = 0;
				int iPoint = -1;
				for(int i=1; i<MAX_MATCH_TEAM; i++)
				{
					if(GWI.a_bIsUse[i])
					{
						if( iPoint < (int)GWI.a_point[i] )
						{
							iPoint = GWI.a_point[i];
							iWinner = i;
						}
					}
				}

				if( iWinner == iIsUse[0] )
				{
					iLoser = iIsUse[1];
				}
				else
				{
					iLoser = iIsUse[0];
				}

				if(GWI.a_point[iWinner] == GWI.a_point[iLoser])
				{
					wsprintf(szTmp, g_Message[ETC_MESSAGE1406].szMessage, GWI.aszGuildName[iWinner], 0, 
						GWI.aszGuildName[iLoser], 0 );// "[길드전결과] '%s'길드(%d위)와 '%s'길드(%d위)의 길드전이 무승부로 끝났습니다."

					if( CGuildWarStatusWnd::GetInstance() )
						CGuildWarStatusWnd::GetInstance()->Draw();
				}
				else
				{
#ifdef TAIWAN_LOCALIZING // 대만의 경우 주어동사의 위치가 우리나라와 달라서 위치가 변경 된다.
					//[公會戰結果]'%s'公會(第%d名) 以%d:%d戰勝'%s'公會(第%d名)
					wsprintf(szTmp, g_Message[ETC_MESSAGE1348].szMessage, GWI.aszGuildName[iWinner], 0,  
									GWI.a_point[iWinner], GWI.a_point[iLoser],
									GWI.aszGuildName[iLoser], 0);// "[길드전결과] '%s'길드(%d위)가 '%s'길드(%d위)에 %d:%d 로 승리했습니다."					
#else
					wsprintf(szTmp, g_Message[ETC_MESSAGE1348].szMessage, GWI.aszGuildName[iWinner], 0, GWI.aszGuildName[iLoser], 0, 
						GWI.a_point[iWinner], GWI.a_point[iLoser] );// "[길드전결과] '%s'길드(%d위)가 '%s'길드(%d위)에 %d:%d 로 승리했습니다."					
#endif

					if( CGuildWarStatusWnd::GetInstance() )
					{
						for( DWORD dwIter = 1 ; dwIter < MAX_MATCH_TEAM ; ++dwIter )
						{
							DWORD dwCurIndex = dwIter-1;

							if( GWI.a_bIsUse[dwIter] && g_pMainPlayer->m_team_index-1 == dwCurIndex )
							{
								if( iWinner == dwIter )
								{
									CGuildWarStatusWnd::GetInstance()->Win();
								}
								else
								{
									CGuildWarStatusWnd::GetInstance()->Lose();
								}
							}
						}						
					}						
				}			
				
				DisplayMessageAdd(szTmp, 0xFFFFFF00);
			}
		}
		break;
	}
	pGroupWnd->m_bGuildWarPacketSendEnable = TRUE;
}

FLAG_TYPE GetFormConvert(int nForm)
{
	FLAG_TYPE ftReturn = M_T_NONE;
	switch(nForm)
	{
	case 0:
		ftReturn = M_T_KILLING_FIELD;
		break;
	case 1:
		ftReturn = M_T_KILLING_FIELD;
		break;
	case 2:
		ftReturn = M_T_DEAH_MATCH;
		break;
	case 3:
		ftReturn = M_T_CAPTURE_THE_FLAG;
		break;
	case 4:
		ftReturn = M_T_LEADER_KILL;
		break;
	case 5:
		ftReturn = M_T_KING_OF_THE_HILL;
		break;
	}
	return ftReturn;
}

FLAG_TYPE GetTypeConvert(int nType)
{
	FLAG_TYPE ftReturn = M_T_NONE;
	switch(nType)
	{
	case 0:
		ftReturn = T_T_GUILD;
		break;
	case 1:
		ftReturn = T_T_RANDOM;
		break;
	case 2:
		ftReturn = T_T_S_GUILD;
		break;
	}
	return ftReturn;
}

GUILD_WAR_TIME GetWarTime(int nTime)
{
	GUILD_WAR_TIME rt= GWT_NONE;
	switch(nTime)
	{
	case 0:
		rt = GWT_TEN;
		break;
	case 1:
		rt = GWT_FIFTEEN;
		break;
	case 2:
		rt = GWT_EIGHTEEN;
		break;
	case 3:
		rt = GWT_TWENTY;
		break;
	}
	return rt;
}

GUILD_WAR_NUMBER GetWarNumber(int nNumber)
{
	GUILD_WAR_NUMBER rt= GWN_NONE;
	switch(nNumber)
	{
	case 0:
		rt = GWN_SIX;
		break;
	case 1:
		rt = GWN_NINE;
		break;
	case 2:
		rt = GWN_TWELVE;
		break;
	case 3:
		rt = GWN_FIFTEEN;
		break;
	case 4:
		rt = GWN_TWENTY;
		break;
	}
	return rt;
}

int GetFormConvert(FLAG_TYPE Form)
{
	int ftReturn = 0;
	switch(Form)
	{
	case M_T_ALL:
		ftReturn = 0;
		break;
	case M_T_KILLING_FIELD:
		ftReturn = 0;
		break;
	case M_T_DEAH_MATCH:
		ftReturn = 2;
		break;
	case M_T_CAPTURE_THE_FLAG:
		ftReturn = 3;
		break;
	case M_T_LEADER_KILL:
		ftReturn = 4;
		break;
	case M_T_KING_OF_THE_HILL:
		ftReturn = 5;
		break;
	}
	return ftReturn;
}


int GetWarTime(GUILD_WAR_TIME Time)
{
	int rt= 0;
	switch(Time)
	{
	case GWT_TEN:
		rt = 0;
		break;
	case GWT_FIFTEEN:
		rt = 1;
		break;
	case GWT_EIGHTEEN:
		rt = 2;
		break;
	case GWT_TWENTY:
		rt = 3;
		break;
	}
	return rt;
}

int GetWarNumber(GUILD_WAR_NUMBER Number)
{
	int rt= 0;
	switch(Number)
	{
	case GWN_SIX:
		rt = 0;
		break;
	case GWN_NINE:
		rt = 1;
		break;
	case GWN_TWELVE:
		rt = 2;
		break;
	case GWN_FIFTEEN:
		rt = 3;
		break;
	case GWN_TWENTY:
		rt = 4;
		break;
	}
	return rt;
}

void CmdGuildWarInvite( char* pMsg, DWORD dwLen )
{
	GUILD_WAR_INVITE_PACKET* pPacket = (GUILD_WAR_INVITE_PACKET*)pMsg;
	CGroupWnd*	pGroupWnd = CGroupWnd::GetInstance();

	if(!g_pThisDungeon->IsStadium() )//길드전 맵
	{
		if(!(pGroupWnd->m_bGuildWarMatchEnable && pGroupWnd->m_byGuildWarType == 2))
		{
			CGuildWarInvite::GetInstance()->SetActive(TRUE);
		}
	}
}

void CmdGuildWarBBS( char* pMsg, DWORD dwLen )
{// 길드전 시작 방송입니다.
	GUILD_WAR_BBS* pPacket = (GUILD_WAR_BBS*)pMsg;
	if(!g_pThisDungeon->IsStadium() )//길드전 맵
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE1403].szMessage,TEXT_COLOR_YELLOW);// "코룸길드전 Quantum Ares가 열리고 있습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1404].szMessage,TEXT_COLOR_YELLOW);// "참여하려면 오른쪽 하던 [그룹] 버튼을 클릭하고"
		DisplayMessageAdd(g_Message[ETC_MESSAGE1405].szMessage,TEXT_COLOR_YELLOW);// "길드전을 선택하세요"
	}
}