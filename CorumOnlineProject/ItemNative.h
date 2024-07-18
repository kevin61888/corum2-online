#pragma once


#include "StdAfx.h"


struct ITEM;
struct CTDS_ITEM_PICKUP;
struct DSTC_ITEM_PICKUP;
class CItem;
class CUser;


void ItemNativeOnLButtonUpProcess();
void ItemChk();
void ItemInfoRender();		
void InputQuantity();
void ItemMouseUp();			
void SendPickupItem(ITEM* pItem, BOOL bActive, BOOL bType = TRUE);
void SetItemPacket(CTDS_ITEM_PICKUP* pItemPacket, BYTE bInv, BYTE bZipCode, BYTE bEquipCode, BYTE bZipCode2, BYTE bEquipCode2);
void ItemLand(int nVal, CTDS_ITEM_PICKUP* ItemPickup);

void InvLargeInsert(CTDS_ITEM_PICKUP* pPacket);
void InvSmallInsert(CTDS_ITEM_PICKUP* pPacket, CItem* pItem);
void Insert_PlayerShopItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
void Insert_TradeItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
BYTE Insert_SmallBankItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
void Insert_SmallBeltItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bChk);
BYTE Insert_SmallInvItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
void Insert_MagicFieldArrayItem(CItem* pItem, BYTE byPackCnt);
void Insert_UpgradeItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
void Insert_MakingItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
BOOL InsertItem(CTDS_ITEM_PICKUP* ItemPickup);

void SetRHandChange(CUser* pUser, DWORD dwId);
void SetLHandChange(CUser* pUser, DWORD dwId);
void SetPickupItemRing(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemBoots(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemAmule(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemBelt(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemArmor(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemHelmet(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemGlove(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemRide(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemMantle(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemWeaponR(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemShield(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemRide(DSTC_ITEM_PICKUP* pPacket);
void SetPickupItemBag(DSTC_ITEM_PICKUP* pPacket);

void ItemSetRing(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetBoots(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetAmule(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetBelt(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetArmor(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetHelmet(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetGlove(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetRide(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetMantle(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetWeaponR(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);
void ItemSetWeaponShield(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType);

BOOL ItemChkUpgrade(CTDS_ITEM_PICKUP* pPacket);
BOOL ItemChkMaking(CTDS_ITEM_PICKUP* pPacket);
BOOL ItemChkPlayerShop(CTDS_ITEM_PICKUP* pPacket);

BOOL InvReCallItem(BYTE bItemKind);


