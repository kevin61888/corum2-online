#pragma once


#include "../CommonServer/CommonHeader.h"


#pragma pack(push,1)
struct DB_USERID_DATA
{
	char szId[MAX_ID_LENGTH];
};

struct DB_BANK_USER
{
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	CItem	sBankItemLarge[MAX_BANK_LARGE_POOL];
	CItem	sBankItemSamll[MAX_BANK_SMALL_POOL];
	
	DWORD	dwMoney;
};

struct DB_JOIN_DUNGEON
{
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	WORD	wHeadID;
	WORD	wClassID;
};

struct DB_CHANGE_DUNGEON
{
	DWORD	dwUserIndex;
	DWORD	dwUserGuildId;
	DWORD	dwUserPartyId;
	char	szLoginID[MAX_CHARACTER_NAME_LENGTH];
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	
	void*	pDungeon;
};

struct DB_PRESENT_COUNT_RESULT
{
	DWORD	dwResult;
	WORD	wItemID;
	DWORD	dwItemQuantity;
	DWORD	dwEmptyPos;
	DWORD	dwRemovePos;
};

struct MY_DB_NUMERIC
{
	BYTE precision;
    BYTE scale;
    BYTE sign;
    unsigned __int64 least;
	unsigned __int64 most;
};
#pragma pack(pop)