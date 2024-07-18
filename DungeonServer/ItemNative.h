#pragma once


class CItem;
class CUser;
class CBaseItem;
class CDungeonLayer;
struct DSTC_ITEM_PICKUP;
struct DSTC_ITEM_MOVE;
struct ITEM_NATIVE_INFO;
struct ITEM_NATIVE_INFOEX;


enum ITEM_USED_NATIVE_TYPE
{
	ITEM_USED_NATIVE_TYPE_START			= 0,
	ITEM_USED_NATIVE_TYPE_ZODIAC		= 1,
	ITEM_USED_NATIVE_TYPE_SUPPLIES,
	ITEM_USED_NATIVE_TYPE_SPECIAL,
	ITEM_USED_NATIVE_TYPE_MAGICARRAY,
	ITEM_USED_NATIVE_TYPE_CONSUMABLE,
	ITEM_USED_NATIVE_TYPE_END,
};


enum ITEM_USED_NATIVE_VALUE
{
	ITEM_USED_NATIVE_VALUE_ZODIAC_START		= 0,
	ITEM_USED_NATIVE_VALUE_COORDINATE_SAVE	= 1,
	ITEM_USED_NATIVE_VALUE_GOTO_COORDINATE,
	ITEM_USED_NATIVE_VALUE_TOWNPORTALSCROLL,
	ITEM_USED_NATIVE_VALUE_INSURANCE,
	ITEM_USED_NATIVE_VALUE_ZODIAC_END,

	ITEM_USED_NATIVE_VALUE_SUPPLIES_START	= 10,
	ITEM_USED_NATIVE_VALUE_HP				= 11,
	ITEM_USED_NATIVE_VALUE_MP,
	ITEM_USED_NATIVE_VALUE_HP_POINT,
	ITEM_USED_NATIVE_VALUE_SP_HP_POINT,
	ITEM_USED_NATIVE_VALUE_HATCH_TIME_SHORTENING,
	ITEM_USED_NATIVE_VALUE_SUPPLIES_END,

	ITEM_USED_NATIVE_VALUE_SPECIAL_START	= 20,
	ITEM_USED_NATIVE_VALUE_GUILD_CLEN_MAKE	= 21,
	ITEM_USED_NATIVE_VALUE_STATUS_SKILL_INIT,
	ITEM_USED_NATIVE_VALUE_VOUCHER_OF_HERO,
	ITEM_USED_NATIVE_VALUE_SPECIAL_END,

	ITEM_USED_NATIVE_VALUE_MAGICARRAY_START	= 30,
	ITEM_USED_NATIVE_VALUE_PERSONAL_RECALL	= 31,
	ITEM_USED_NATIVE_VALUE_ALL_PARTY_RECALL,
	ITEM_USED_NATIVE_VALUE_GUILD_RECALL,
	ITEM_USED_NATIVE_VALUE_MAGICARRAY_TO_PARTY,
	ITEM_USED_NATIVE_VALUE_MAGICARRAY_END,

	ITEM_USED_NATIVE_VALUE_CONSUMABLE_START	= 40,
	ITEM_USED_NATIVE_VALUE_CONSUMABLE_END,
};


enum SET_LINKTOINV_TYPE
{
	SET_LINK_TO_INV_EMPTY_SLOT	= 1,
	SET_LINK_TO_INV_NATIVE_SLOT = 2,	
};


struct ITEM_NATIVE_INFO
{
	ITEM_NATIVE eSlotID;
	union
	{
		DWORD dwOwnerID;
		INT64 i64ItemID;
	};
	union
	{
		BYTE bySlotIndex;
		BYTE bySectionNum;
	};
	union 
	{
		BYTE byItemQuantityCnt;
		DWORD dwMoney;
	};
	
	ITEM_NATIVE_INFO()
	{
		memset( this, 0, sizeof(ITEM_NATIVE_INFO) );
	}
};


struct ITEM_NATIVE_VIRTUAL_INFO
{
	BOOL bIsLock;
	BYTE byVirtualCnt;
};


const CItem*			GetItemNative(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo);
const ITEM_NATIVE_INFO* GetItemNativeLink(const ITEM_NATIVE_INFO* pItemNativeInfo);
BOOL					SetSlotItemNative(CDungeonLayer* pDungeonLayer, const CItem* pItem_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest);
BOOL					SetSlotItemNativeLink(ITEM_NATIVE eFromItemNative, BYTE byFromSlotIndex,const ITEM_NATIVE_INFO* pToItemNativeLinkInfo);
BOOL					PutOnItemNative(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest);
BOOL					MoveItemNative(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest, BYTE byQuantityCount, DSTC_ITEM_MOVE* const pItemMoveResult = NULL);
BOOL					OverlapItemNative(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest, BYTE byQuantityCount, DSTC_ITEM_MOVE* const pItemMoveResult = NULL);
BOOL					ExchangeItemNative(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest);
BOOL					ExchangeItemNativeLink(const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest);
BOOL					RemoveItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo);
BOOL					RemoveItemNativeLink(const ITEM_NATIVE_INFO* pItemNativeInfo);
BOOL					DropItem(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, BYTE byQuantityCount, DSTC_ITEM_MOVE* const pItemMoveResult);
BOOL					PickupItem(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, ITEM_NATIVE_INFO* pItemNativeInfo_Dest,DSTC_ITEM_MOVE* const pItemMoveResult);
BOOL					PickupItemNative(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo_Src, ITEM_NATIVE_INFO* pItemNativeInfo_Dest, DSTC_ITEM_MOVE* const pItemMoveResult);


BOOL					IsItemNativeTile(ITEM_NATIVE eItemNative);
BOOL					IsItemNativePlayer(ITEM_NATIVE eItemNative);
BOOL					IsItemNativeTraderMonster(ITEM_NATIVE eItemNative);
BOOL					IsItemNativeSrcRemove(ITEM_NATIVE eItemNativeDest);
BOOL					IsItemNativeLink(const ITEM_NATIVE_INFO* pItemNativeInfo);
BOOL					IsItemNativeInvLargeAndSmall(ITEM_NATIVE eItemNative);
BOOL					IsItemNativeKarz(ITEM_NATIVE eItemNative);
BOOL					IsItemDungeonManager(ITEM_NATIVE eItemNative);
BYTE					GetItemNativePlayerBufferCount();
BYTE					GetItemNativeTileBufferCount();
BYTE					GetItemNativeTraderMonsterCount();

BOOL IsSamePropertyConsumableItem(CItem* pSrc, CItem* pDst);
