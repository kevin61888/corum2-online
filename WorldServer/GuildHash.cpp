//======================================================//
// Code by Jang.							2003.04.04	//
// Object : Guild.										//
//======================================================//
#include "stdafx.h"
#include "GuildHash.h"
#include "OnlyList.h"
#include "WorldUser.h"

// 각 언어별 길드 랭크 기본값을 세팅.. ㅠㅠ

CGuildTableHash*		g_pGuildTableHash = NULL;
CGroupInfoTableHash*	g_pGroupInfoTableHash = NULL; 
CGuildWarHash*			g_pGuildWarHash = NULL;
COnlyList*				g_pRankList = NULL;

COnlyList*				g_pGuildWarTimeList = NULL;

STMPOOL_HANDLE			g_pGuildWarTimePool = NULL;

CDungeonListHash*		g_pDungeonListHash = NULL;

STMPOOL_HANDLE			g_pDungeonListPool = NULL;
STMPOOL_HANDLE			g_pDungeonNodePool = NULL;

STMPOOL_HANDLE			g_pGuildTablePool = NULL;
STMPOOL_HANDLE			g_pGroupTablePool = NULL;
STMPOOL_HANDLE			g_pGuildOfflinePool = NULL;
STMPOOL_HANDLE			g_pGuildWarPool = NULL;
STMPOOL_HANDLE			g_pRankListPool = NULL;	


void CDungeonListHash::DeleteData(void* pData)
{
	LP_DUNGEON_NODE pDungeonNode = (LP_DUNGEON_NODE)pData;
	
	if(!pDungeonNode)
		return;	
	
	memset(pDungeonNode, 0, sizeof(SDUNGEON_NODE));
	LALFree(g_pDungeonNodePool, pDungeonNode);	
	pDungeonNode = NULL;			
}

void CGuildOfflineHash::DeleteData(void* pData)
{	
	LP_GUILD_OFFLINE_USER pGuildOfflineTable = (LP_GUILD_OFFLINE_USER)pData;
	
	if(!pGuildOfflineTable)
		return;	
	
	memset(pGuildOfflineTable, 0, sizeof(GUILD_OFFLINE_USER));
	LALFree(g_pGuildOfflinePool, pGuildOfflineTable);	
	pGuildOfflineTable = NULL;
}

void CGuildUserTableHash::DeleteData(void* pData)
{
	pData;
}

void CGuildTableHash::DeleteData(void* pData)
{
	LP_GUILD_TABLE pGuildTable = (LP_GUILD_TABLE)pData;

	if(pGuildTable->pGuildUserHash)
	{
		pGuildTable->pGuildUserHash->Destroy(TRUE);
		delete pGuildTable->pGuildUserHash;
		pGuildTable->pGuildUserHash = NULL;
	}

	if(pGuildTable->pGuildOfflineHash)
	{
		pGuildTable->pGuildOfflineHash->Destroy(TRUE);
		delete pGuildTable->pGuildOfflineHash;
		pGuildTable->pGuildOfflineHash = NULL;
	}
	
	if(!pGuildTable)
		return;	
	
	LALFree(g_pGuildTablePool, pGuildTable);	
}

void CGroupInfoTableHash::DeleteData(void* pData)
{
	LP_GROUPINFO_TABLE pGroupInfoTable = (LP_GROUPINFO_TABLE)pData;
	
	if(!pGroupInfoTable)
		return;	
	
	memset(pGroupInfoTable, 0, sizeof(SGROUPINFO_TABLE));
	LALFree(g_pGroupTablePool, pGroupInfoTable);	
	pGroupInfoTable = NULL;
}


void CGuildWarHash::DeleteData(void* pData)
{
	LP_GUILD_LIST pGuildList = (LP_GUILD_LIST)pData;
	
	if(!pGuildList)
		return;	
	
	memset(pGuildList, 0, sizeof(SGUILD_LIST));
	LALFree(g_pGuildWarPool, pGuildList);	
	pGuildList = NULL;
}
//======================================================//
// End.													//
//======================================================//