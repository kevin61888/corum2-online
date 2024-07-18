#include "StdAfx.h"
#include "ItemManager.h"
#include "CommonHash.h"
#include "CommonPacket.h"
#include "CommonAccessory.h"
#include "ComInterface.h"


LPBASEITEM_HASH g_pBaseItemHash = NULL;


DWORD DECLSPECIFIER GetItemKind(WORD wItemID)
{
	if(__ITEM_KARZ__ == wItemID)
		return ITEM_KIND_MONEY;

	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(wItemID);

	if(pBaseItem)
		return pBaseItem->dwCode_ID;		
	else
		return NULL;
}


ITEM_TYPE DECLSPECIFIER GetItemType(WORD wItemID)
{
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(wItemID);

	if(pBaseItem)
	{
		return (ITEM_TYPE)pBaseItem->bCode_Type;	
	}
	else
	{
		if(__ITEM_KARZ__ == wItemID)
			return ITEM_TYPE_MONEY;
		else
			return ITEM_TYPE_NONE;			
	}
}


void DECLSPECIFIER SetCommonServerBaseItemHash(void* pBaseItemHash)
{
	g_pBaseItemHash = (LPBASEITEM_HASH)pBaseItemHash;	
}











