#include "stdafx.h"
#include "SchoolGuildDungeon.h"


// 일본판을 위한 비점령 던전의 학교 길드 이름 부여!

SCHOOL_GUILD_DUNGEON g_pGuildDungeon[50];
CSchoolGuildDungeonHash* g_pSchoolGuildDungeonTableHash = NULL;


void InitSchoolGuildDungeon()
{
	g_pSchoolGuildDungeonTableHash = new CSchoolGuildDungeonHash;
	g_pSchoolGuildDungeonTableHash->InitializeHashTable(50, 50);

	memset(g_pGuildDungeon, 0, sizeof(g_pGuildDungeon));
	
	int nCount = g_pDb->OpenRecord("Select * from GuildDungeonTable", g_pGuildDungeon, 50, (BYTE)GAME_DB);

	if(nCount<=0)
	{
		Log(LOG_IMPORTANT, "Fail to query GuildDungeonTable!");
		return;
	}

	for(int i = 0; i < nCount; i++)
	{
		g_pSchoolGuildDungeonTableHash->Insert(&g_pGuildDungeon[i], g_pGuildDungeon[i].dwDungeonID);
	}
}
