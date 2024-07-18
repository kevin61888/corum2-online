#ifndef __GUILD_HASH_H__
#define __GUILD_HASH_H__

#pragma once

#include "GuildDef.h"
#include "../CommonServer/AbyssHash.h"
#include "WorldUser.h"

#pragma pack(push, 1)

// 각 길드의 기본정보를 가지는 구조체
typedef struct __tag_guild__
{
	// 길드의 지위를 나타냄
	enum	GUILD_RANK_COUNT
	{
		GUILD_RANK_MASTER		= 0,
		GUILD_RANK_SUB_MASTER	= 1,
		GUILD_RANK_HIGH			= 2,
		GUILD_RANK_NORMAL		= 3,
		GUILD_RANK_COUNT_MAX
	};
	DWORD	dwGuildId;	// 길드의 길드인덱싱 아이디

	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];	// 길드이름
	
	char	szRankName[GUILD_RANK_COUNT_MAX][MAX_CHARACTER_NAME_LENGTH];	// 각 지위에 맞는 명칭
	
	BYTE	byType;		// 길드의 타입? 뭔지는 아직 모름
	BYTE	byCount;	// 전체인원수
	
	WORD	wLevel;		// 길드레벨
	DWORD	dwExp;		// 길드경험치
	
	BYTE	byMember[GUILD_RANK_COUNT_MAX];	// 각 지위별 인원수

	DWORD	dwPoint;	// 길드의 점수
/*
	DWORD	m_dwWeekRate;	// 주간점수
	DWORD	m_dwWeekRank;	// 주간랭킹

	DWORD	m_dwMonthRate;	// 월간점수
	DWORD	m_dwMonthRank;	// 월간랭킹

	DWORD	m_dwTotalRate;	// 종합점수
	DWORD	m_dwTotalRank;	// 종합랭킹
*/
}GUILD, *LP_GUILD;

typedef struct __tag_guilduser__
{
	DWORD	dwCharIndex;
	DWORD	dwGuildId;

}GUILDUSER, *LP_GUILDUSER;

typedef struct __tag_guilduser_info__
{
	DWORD	dwCharIndex;
	DWORD	dwGuildId;
	BYTE	byRank;

}GUILDUSER_INFO, *LP_GUILDUSER_INFO;

typedef struct GUILD_OFFLINE_USER
{
	DWORD		dwUserIndex;

	BYTE		byRank;

	DWORD		dwLevel;
	
	WORD		wClass;

	char		szName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];
	
	DBTIMESTAMP sDate;

}GUILD_OFFLINE_USER, *LP_GUILD_OFFLINE_USER;

typedef struct __GUILDWAR_RANK__
{
	DWORD		dwPoint;
	DWORD		dwExp;
	DWORD		dwGuildId;

	char		szName[MAX_GUILD_NAME_LENGTH];

}GUILDWAR_RANK, *LP_GUILDWAR_RANK;

struct GUILD_NODE
{
	DWORD		dwGuildId;	
	
	time_t		sTime;

	WORD		wPoint1;
	WORD		wPoint2;
};

typedef struct GUILD_LIST
{	
	DWORD		dwGuildId;	
	GUILD_NODE	sGuildNode[MAX_GUILD_WAR];

}SGUILD_LIST, *LP_GUILD_LIST;

typedef struct SGUILD_LISTJAPAN
{	
	DWORD		dwGuildId;	
	GUILD_NODE	sGuildNode[8];

}SGUILD_LISTJAPAN, *LP_GUILD_LISTJAPAN;

typedef struct __tsg_dbrank__
{
	char	szRankName[GUILD::GUILD_RANK_COUNT_MAX][MAX_CHARACTER_NAME_LENGTH];

}GUILD_RANK, *LP_GUILD_RANK;

typedef struct __tag_groupinfo__
{
	BYTE	byType;
	BYTE	byLevel;
	WORD	wMemeber[5];
	DWORD	dwExp;
	BYTE	byAura;
	BYTE	byMake_Size;

}SGROUP_INFO, *LP_GROUP_INFO;

typedef	struct __tag_character_search__
{
	DWORD	dwUserIndex;

	char	szName[MAX_CHARACTER_NAME_LENGTH];

}SCHARACTER_SEARCH, *LP_CHARACTER_SEARCH;

typedef struct __tag_groupinfo_hash__
{
	SGROUP_INFO	sGroupInfo[2];

}SGROUPINFO_TABLE, *LP_GROUPINFO_TABLE;

typedef struct __tag_dungeon_node__
{
	DWORD	dwDungeonId;		// 던전 Id.
	BYTE	byType;				// 지역.

}SDUNGEON_NODE, *LP_DUNGEON_NODE;

typedef struct tagTime
{ 
	DWORD	dwLowId;
	DWORD	dwHiId;
}WAR_TIME;

typedef struct __tag_guild_war_time__
{
	union
	{
		WAR_TIME war_time;

		__int64	n64Id;
	};

	time_t	sTime;

}SGUILDWAR_TIME, *LP_GUILDWAR_TIME;

#pragma pack(pop)

class CDungeonListHash : public CAbyssHash<SDUNGEON_NODE>
{
public:
	
	void DeleteData( void* pData );
};

class CGuildOfflineHash : public CAbyssHash<GUILD_OFFLINE_USER>
{
public:
	
	void DeleteData( void* pData );
};


class CGuildUserTableHash : public CAbyssHash<CWorldUser>
{
public:
	
	void DeleteData( void* pData );
};

class CGuildWarHash : public CAbyssHash<GUILD_LIST>
{
public:
	
	void DeleteData( void* pData );
};

class CGroupInfoTableHash : public CAbyssHash<SGROUPINFO_TABLE>
{
public:
	
	void DeleteData( void* pData );
};

// 길드의 기본정보와, 유저관련 해쉬를 가지는 테이블
typedef struct __tag_guild_table__
{	
	GUILD					sGuild;				// 길드의 기본정보를 담는 구조체
	CGuildUserTableHash*	pGuildUserHash;		// 온라인? 또는 전체유저의 정보를 가지는 해쉬
	CGuildOfflineHash*		pGuildOfflineHash;	// 오프라인 유저의 해쉬
	
	BYTE					bLevelIndex;

	DWORD					dwPrevGuildExp;
	
}GUILD_TABLE, *LP_GUILD_TABLE;

class CGuildTableHash : public CAbyssHash<GUILD_TABLE>
{
public:
	
	void DeleteData( void* pData );
};


extern CGuildTableHash*		g_pGuildTableHash;
extern CGroupInfoTableHash* g_pGroupInfoTableHash;
extern CGuildWarHash*		g_pGuildWarHash;
extern CDungeonListHash*	g_pDungeonListHash;
extern COnlyList*			g_pRankList;


extern STMPOOL_HANDLE		g_pGuildTablePool;
extern STMPOOL_HANDLE		g_pGuildOfflinePool;
extern STMPOOL_HANDLE		g_pGroupTablePool;
extern STMPOOL_HANDLE		g_pGuildWarPool;
extern STMPOOL_HANDLE		g_pDungeonNodePool;
extern STMPOOL_HANDLE		g_pRankListPool;


extern char*				g_szDefaultGuildRank[4];


#endif 