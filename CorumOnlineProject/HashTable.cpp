#include "stdafx.h"
#include "HashTable.h"
#include "User.h"
#include "Monster.h"		
#include "Npc.h"
#include "InitGame.h"
#include "struct.h"
#include "WorldProcess.h"

void CUserHash::DeleteData( void* pData )
{

}

void CMonsterHash::DeleteData( void* pData )
{
	
}

void CEffectHash::DeleteData( void* pData )
{	

}


void CGuildInfoHash::DeleteData( void* pData )
{	
	LP_GUILD_DATA pGuildInfoTable = (GUILD_DATA*)pData;

	if(!pGuildInfoTable)
		return;	
		
	LALFree(g_pGuildDataPool, pGuildInfoTable);
}


void CGroupInfoHash::DeleteData(void* pData)
{
	LP_GROUPINFO_TABLE pGroupInfoTable = (LP_GROUPINFO_TABLE)pData;

	if(!pGroupInfoTable)
		return;	
		
	LALFree(g_pGroupInfoPool, pGroupInfoTable);	
}

void CItemStoreHash::DeleteData(void* pData)
{
	LP_ITEM_STORE pItemStore = (LP_ITEM_STORE)pData;

	if(!pItemStore)
		return;	

	LALFree(g_pItemStorePool, pItemStore);	
}

void CHelpInfoHash::DeleteData(void* pData)
{
	LP_HELP_INFO pHelpInfoTable = (LP_HELP_INFO)pData;

	if(!pHelpInfoTable)
		return;	
		
	LALFree(g_pHelpInfoPool, pHelpInfoTable);	
}

void CItemResourceHash::DeleteData(void* pData)
{
	LP_ITEM_RESOURCE_EX pItemResource = (LP_ITEM_RESOURCE_EX)pData;

	if(!pItemResource)
		return;	
		
	if(pItemResource->pSpr)
	{
		pItemResource->pSpr->Release();
		pItemResource->pSpr = NULL;
	}

	LALFree(g_pItemResourcePool, pItemResource);	
	pItemResource = NULL;
}

void CItemOptionHash::DeleteData(void* pData)
{
	LP_ITEM_OPTION pItemOption = (LP_ITEM_OPTION)pData;

	if(!pItemOption)
		return;	
		
	LALFree(g_pItemOptionPool, pItemOption);	
}

void CSetItemInfoHash::DeleteData( void* pData )
{
	LPSET_ITEM_INFO pInfo = (LPSET_ITEM_INFO)pData;

	if( !pInfo ) return;

	delete pInfo;
}

void CItemMakingInfoHash::DeleteData( void* pData )
{
	LPITEM_MAKING_INFO pInfo = (LPITEM_MAKING_INFO)pData;

	if( !pInfo ) return;

	delete pInfo;
}


void CPartyUserHash::DeleteData( void* pData )
{
	LP_PARTY_USER pParty = (LP_PARTY_USER)pData;
	if(!pParty)	return;

	FreeWorldPartyUser( pParty );
	LALFree(g_pPartyPool, pParty);
}

void CGuildUserHash::DeleteData( void* pData )
{
	LP_GUILD_USER pGuildUser = (LP_GUILD_USER)pData;

	if(!pGuildUser)
		return;

	LALFree(g_pGuildPool, pGuildUser);
}

void CGuildOffLineHash::DeleteData( void* pData )
{
	LP_GUILD_OFFLINE_USER pGuildOfflineUser = (LP_GUILD_OFFLINE_USER)pData;

	if(!pGuildOfflineUser)
		return;

	LALFree(g_pGuildOfflinePool, pGuildOfflineUser);
}

void CMessengerUserHash::DeleteData( void* pData )
{
	LP_MESSENGER_USER pMessengerUser = (LP_MESSENGER_USER)pData;

	if(!pMessengerUser)
		return;

	LALFree(g_pMessengerUserPool, pMessengerUser);
}

void CGuildListHash::DeleteData( void* pData )
{
	LP_GUILD_WAR pGuildWar = (LP_GUILD_WAR)pData;

	if(!pGuildWar)
		return;

	LALFree(g_pGuildWarPool, pGuildWar);
}

void CPartyBoardHash::DeleteData( void* pData )
{
	LP_PARTY_BOARD lpPartyBoard = (LP_PARTY_BOARD)pData;

	if(!lpPartyBoard)
		return;

	LALFree(g_pPartyBoardPool, lpPartyBoard);
	memset(lpPartyBoard, 0, sizeof(SPARTY_BOARD));
	lpPartyBoard = NULL;
}
