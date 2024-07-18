#ifndef __PARTY_HASH_H__
#define __PARTY_HASH_H__

#pragma once

#include "OnlyList.h"
#include "../CommonServer/AbyssHash.h"


typedef struct __tag_party_table__
{		
	COnlyList*		pPartyUserList;
	
	DWORD			dwPartyId;
	BYTE			bCount;
} PARTY_TABLE, *LP_PARTY_TABLE;

typedef struct __tag_partyboard_table__
{
	DWORD			dwPartyId;
	DWORD			dwPartyUserLeader;

	char			szCharName[MAX_CHARACTER_NAME_LENGTH];
	
	char			szMemo[32];
	
	time_t			sTime;

} PARTYBOARD_TABLE, *LP_PARTYBOARD_TABLE;

class CPartyTableHash : public CAbyssHash<PARTY_TABLE>
{
public:
	void DeleteData( void* pData );
};

class CPartyBoardTableHash : public CAbyssHash<PARTYBOARD_TABLE>
{
public:
	void DeleteData( void* pData );
};

extern CPartyTableHash*			g_pPartyTableHash;
extern CPartyBoardTableHash*	g_pPartyBoardTableHash;

extern STMPOOL_HANDLE		g_pPartyTablePool;
extern STMPOOL_HANDLE		g_pPartyBoardTablePool;

#endif // __PARTY_HASH_H__