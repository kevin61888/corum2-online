#pragma once


#include "../CommonServer/AbyssHash64.h"
#include "User.h"
#include "Monster.h"		
#include "Npc.h"
#include "Struct.h"
#include "Menu.h"


typedef CAbyssHash64<ITEM>		ITEM_HASH, * LPITEM_HASH;
typedef CAbyssHash<CBaseItem>	BASEITEM_HASH,* LPBASEITEM_HASH;


class CUserHash : public CAbyssHash<CUser>
{
public:
	void DeleteData( void* pData );
};

class CMonsterHash : public CAbyssHash<CMonster>
{
public:
	void DeleteData( void* pData );
};

class CEffectHash : public CAbyssHash<EffectDesc>
{
public:
	void DeleteData( void* pData );
};

class CHelpInfoHash : public CAbyssHash<SHELP_INFO>
{
public:
	void DeleteData(void* pData);
};

class CItemResourceHash : public CAbyssHash<SITEM_RESOURCE_EX>
{
public:
	void DeleteData(void* pData);
};

class CItemStoreHash : public CAbyssHash<ITEM_STORE>
{
public:
	void DeleteData(void* pData);
};

class CItemOptionHash : public CAbyssHash<ITEM_OPTION>
{
public:
	void DeleteData(void* pData);
};

class CSetItemInfoHash : public CAbyssHash<SET_ITEM_INFO>
{
public:
	void DeleteData(void* pData);
};

class CItemMakingInfoHash : public CAbyssHash<ITEM_MAKING_INFO>
{
public:
	void DeleteData(void* pData);
};

class CPartyUserHash : public CAbyssHash<PARTY_USER>
{
public:
	void DeleteData( void* pData );
};

class CGroupInfoHash : public CAbyssHash<SGROUPINFO_TABLE>
{
public:
	void DeleteData( void* pData );
};

class CGuildUserHash : public CAbyssHash<GUILD_USER>
{
public:
	void DeleteData( void* pData );
};

class CGuildOffLineHash : public CAbyssHash<GUILD_OFFLINE_USER>
{
public:	
	void DeleteData( void* pData );
};

class CGuildInfoHash : public CAbyssHash<GUILD_DATA>
{
public:	
	void DeleteData( void* pData );
};

class CMessengerUserHash : public CAbyssHash<MESSENGER_USER>
{
public:	
	void DeleteData( void* pData );
};

class CGuildListHash : public CAbyssHash<SGUILD_WAR>
{
public:	
	void DeleteData( void* pData );
};

class CPartyBoardHash : public CAbyssHash<SPARTY_BOARD>
{
public:	
	void DeleteData( void* pData );
};