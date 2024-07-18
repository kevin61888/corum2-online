#pragma once

#define MAX_MONSTER_FORMATION	300
#define MAX_MONSTER_KIND		10

#pragma pack(push, 1)

struct MONSTER_FORMATION
{
	BYTE	bType;								// Gamesystem Fixed?
	WORD	wX;
	WORD	wY;
	WORD	wResponse;
	DWORD	dwMonsterID;
};

struct LAYER_FORMATION
{
	DWORD				dwDungeon;				// Dungeon ID
	BYTE				bLayer;					// Layer
	MONSTER_FORMATION	pMonsterFormation[MAX_MONSTER_FORMATION];
	DWORD				pdwMonsterKind[MAX_MONSTER_KIND];
};

#pragma pack(pop)