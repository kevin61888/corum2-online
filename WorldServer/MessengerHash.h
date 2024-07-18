#ifndef __MESSENGER_HASH_H__
#define __MESSENGER_HASH_H__

#pragma once

#include "MessengerDef.h"
#include "../CommonServer/AbyssHash.h"

#pragma pack(push, 1)
typedef struct __tag_messenger__
{
	DWORD	dwUserIndex;
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byType;	
	WORD	wClass;

}MESSENGER_USER, *LP_MESSENGER_USER;
#pragma pack(pop)

class CMessengerUserHash : public CAbyssHash<MESSENGER_USER>
{
public:
	
	void DeleteData( void* pData );
};


#pragma pack(push, 1)
typedef struct __tag_messenger_table__
{
	CMessengerUserHash* pMessengerUserHash;

}MESSENGER_TABLE, *LP_MESSENGER_TABLE;
#pragma pack(pop)

class CMessengerTableHash : public CAbyssHash<MESSENGER_TABLE>
{
public:
	
	void DeleteData( void* pData );
};



extern CMessengerTableHash*	g_pMessengerTableHash;
extern STMPOOL_HANDLE		g_pMessengerUserPool;
extern STMPOOL_HANDLE		g_pMessengerTablePool;


#endif