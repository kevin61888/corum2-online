#include "stdafx.h"
#include "worlduser.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonTable.h"
#include "GuildHash.h"
#include "../commonserver/CommonDungeon.h"

extern CGuildWarHash*		g_pGuildWarHash;


void CWorldUser::SetCurDungeon(WORD wDungeonID)
{
	if(wDungeonID == 0)
	{
		m_dwPrevDungeonId = m_dwDungeonID;
	}

	if( !((5008 <= wDungeonID) && (6000 > wDungeonID))//: hwoarang 050220 
	&&	DUNGEON_TYPE_MATCH_STATIDUM != DungeonIDtoType( wDungeonID ) 	)//길드전 맵이 아니면
	{
		m_dwDungeonID = wDungeonID;
	}

	if( DUNGEON_TYPE_TUNNEL == DungeonIDtoType( wDungeonID ) )//통로던전이면
	{
		m_bDestWorldSpot = wDungeonID%2==0;
	}
	else
	{
		m_bDestWorldSpot = 0;
	}
}

void CWorldUser::GuildIdChange()
{
	DUNGEON_DATA_EX* cur = NULL;
	POSITION_ pos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();
	while (pos)
	{
		cur = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pDungeonList->GetNext(pos);

		if (cur->m_dwOwnerIndex == m_dwUserIndex)
		{
			// 이놈 던전이네. 이놈던전에게 길드 갱신해줘라.
			cur->SetOwner(m_dwUserIndex, m_dwGuildId, m_szID, m_szCharacterName);
			cur->SendRefreshOwnerGuild();
		}		
	}
}

void CWorldUser::GuildIdPoint()
{
	char szQuery[0xff]={0,};

	LP_GUILD_LIST	lpGuildListTable	= g_pGuildWarHash->GetData(m_dwGuildId);	
	LP_GUILD_TABLE	lpGuildTable		= g_pGuildTableHash->GetData(m_dwGuildId);
		
	if(!lpGuildListTable)
		return;	
	if(!lpGuildTable)
		return;	

	for(int i = 0; i < MAX_GUILD_WAR; i++)
	{
		if(lpGuildListTable->sGuildNode[i].dwGuildId!=0)
		{
			LP_GUILD_LIST	lpGuildListTableDst	= g_pGuildWarHash->GetData(lpGuildListTable->sGuildNode[i].dwGuildId);
			LP_GUILD_TABLE	lpGuildTableDst		= g_pGuildTableHash->GetData(lpGuildListTable->sGuildNode[i].dwGuildId);

			if(!lpGuildListTableDst)
				continue;	
			if(!lpGuildTableDst)
				continue;
		
			for(int j = 0; j < MAX_GUILD_WAR; j++)
			{
				if(lpGuildListTableDst->sGuildNode[j].dwGuildId==lpGuildListTable->dwGuildId)
				{
					lpGuildListTable->sGuildNode[i].wPoint2		+= 3;
					lpGuildListTableDst->sGuildNode[j].wPoint1	+= 3;

					WSTC_GUILDWAR_POINT pPointPacket;
					pPointPacket.dwGuildId	= lpGuildListTable->sGuildNode[i].dwGuildId;
					pPointPacket.wPoint1	= lpGuildListTable->sGuildNode[i].wPoint1;
					pPointPacket.wPoint2	= lpGuildListTable->sGuildNode[i].wPoint2;
					pPointPacket.byType		= 7;												

					ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();

					while(pGuildUserPos)
					{
						CWorldUser* pGuildUser = pGuildUserPos->pData;

						if(pGuildUser)
							g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPointPacket, pPointPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

						pGuildUserPos = pGuildUserPos->pNext;
					}
					
					pPointPacket.dwGuildId	= lpGuildListTableDst->sGuildNode[j].dwGuildId;
					pPointPacket.wPoint1	= lpGuildListTableDst->sGuildNode[j].wPoint1;
					pPointPacket.wPoint2	= lpGuildListTableDst->sGuildNode[j].wPoint2;			
					pPointPacket.byType		= 6;

					pGuildUserPos = lpGuildTableDst->pGuildUserHash->GetHead();

					while(pGuildUserPos)
					{
						CWorldUser* pGuildUser = pGuildUserPos->pData;

						if(pGuildUser)
							g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPointPacket, pPointPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
						
						pGuildUserPos = pGuildUserPos->pNext;
					}

					// 디비 업데이트 //
					DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);

					if(!pBinding)
					{
						Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
						return;
					}

					GUILD_NODE sGuildNode[MAX_GUILD_WAR];	
					memset(szQuery, 0, sizeof(szQuery));
					memset(sGuildNode, 0, sizeof(sGuildNode));
					memcpy(sGuildNode, lpGuildListTableDst->sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR);

					pBinding[0].obValue		= 0;
					pBinding[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
					pBinding[0].wType		= DBTYPE_BYTES;					
					
					sprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTableDst->sGuild.dwGuildId);
					g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR, 1, FALSE, 0, NULL, (BYTE)GAME_DB);
				}
			}						
		}
	}

	// 디비 업데이트 //
	DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);

	if(!pBinding)
	{
		Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
		return;
	}

	GUILD_NODE sGuildNode[MAX_GUILD_WAR];	
	memset(szQuery, 0, sizeof(szQuery));
	memset(sGuildNode, 0, sizeof(sGuildNode));
	memcpy(sGuildNode, lpGuildListTable->sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR);

	pBinding[0].obValue		= 0;
	pBinding[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
	pBinding[0].wType		= DBTYPE_BYTES;					
	
	sprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTable->sGuild.dwGuildId);
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR, 1, FALSE, 0, NULL, (BYTE)GAME_DB);			
}

#include "GuildMatcher.h"
extern CGuildMatcher g_GuildMatcher;

bool CWorldUser::IsHaveGMC()
{
	if( M_T_NONE == m_GMC.First().MatchType() )
	{
		return false;
	}
	return true;
}

void CWorldUser::ClearGuildWarData()
{
//	CPlayerInfo Player( this );
//	g_GuildMatcher.Remove( m_GMC, Player);
	m_GMC.Clear();
	m_byLayerIndex = 0;//길드전시에만 갱신 됩니다. 평상시에는 사용 하지 마세요
	m_dwGuildWarFlag = G_W_F_NONE;//G_W_F_xxxx
	m_team_index = 0;//
	 //Call 된 애들은 Master가 지우기 전까지는 Stadium 에서 Remove 되지 않는다! 
}