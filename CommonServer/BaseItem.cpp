#include "StdAfx.h"
#include "BaseItem.h"
#include "ItemManagerDefine.h"


WORD CBaseItem::GetID()
{
	return wID;
}


BYTE CBaseItem::GetClass()
{
	if(dwCode_ID & ITEM_KIND_WEAPON)
		return BaseItem_Weapon.bClassID;
	else if(dwCode_ID & ITEM_KIND_AROMR)
		return BaseItem_Armor.bClassID;
	else if (dwCode_ID & ITEM_KIND_RIDE) 
		return NULL;
	else 
		return 0xff;
}


BYTE CBaseItem::GetLandItemType()
{
	return bRandItem;
}


WORD CBaseItem::GetLastTime()
{
	if(dwCode_ID & ITEM_KIND_CONSUMABLE)
		return BaseItem_Consumable.wLast_Time;
	else
		return NULL;
}


DWORD CBaseItem::GetSellPrice()
{
	if(dwCode_ID & ITEM_KIND_WEAPON)
		return BaseItem_Weapon.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_AROMR)
		return BaseItem_Armor.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_BAG)
		return BaseItem_Bag.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_GUARDIAN)
		return BaseItem_Guardian.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_ZODIAC)
		return BaseItem_Zodiac.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_RIDE)
		return BaseItem_Ride.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_SPECIAL)
		return BaseItem_Special.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_MAGICARRAY)
		return BaseItem_MagicArray.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_MATERIALS)
		return BaseItem_Materials.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_MIXUPGRADE)
		return BaseItem_MixUpgrade.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_MAGICFIELDARRAY)
		return BaseItem_MagicFieldArray.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_SUPPLIES)
		return BaseItem_Supplies.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_CONSUMABLE)
		return BaseItem_Consumable.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_UPGRADE)
		return BaseItem_UpGrade.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_LIQUID)
		return BaseItem_LiQuid.dwSellPrice;
	else if(dwCode_ID & ITEM_KIND_EDITION)
		return BaseItem_Edition.dwSellPrice;
	else 
		return NULL;	
}


DWORD CBaseItem::GetPrice()
{
	if(dwCode_ID & ITEM_KIND_WEAPON)
		return BaseItem_Weapon.dwPrice;
	else if(dwCode_ID & ITEM_KIND_AROMR)
		return BaseItem_Armor.dwPrice;
	else if(dwCode_ID & ITEM_KIND_BAG)
		return BaseItem_Bag.dwPrice;
	else if(dwCode_ID & ITEM_KIND_GUARDIAN)
		return BaseItem_Guardian.dwPrice;
	else if(dwCode_ID & ITEM_KIND_ZODIAC)
		return BaseItem_Zodiac.dwPrice;
	else if(dwCode_ID & ITEM_KIND_RIDE)
		return BaseItem_Ride.dwPrice;
	else if(dwCode_ID & ITEM_KIND_SPECIAL)
		return BaseItem_Special.dwPrice;
	else if(dwCode_ID & ITEM_KIND_MAGICARRAY)
		return BaseItem_MagicArray.dwPrice;
	else if(dwCode_ID & ITEM_KIND_MATERIALS)
		return BaseItem_Materials.dwPrice;
	else if(dwCode_ID & ITEM_KIND_MIXUPGRADE)
		return BaseItem_MixUpgrade.dwPrice;
	else if(dwCode_ID & ITEM_KIND_MAGICFIELDARRAY)
		return BaseItem_MagicFieldArray.dwPrice;
	else if(dwCode_ID & ITEM_KIND_SUPPLIES)
		return BaseItem_Supplies.dwPrice;
	else if(dwCode_ID & ITEM_KIND_CONSUMABLE)
		return BaseItem_Consumable.dwPrice;
	else if(dwCode_ID & ITEM_KIND_UPGRADE)
		return BaseItem_UpGrade.dwPrice;
	else if(dwCode_ID & ITEM_KIND_LIQUID)
		return BaseItem_LiQuid.dwPrice;
	else if(dwCode_ID & ITEM_KIND_EDITION)
		return BaseItem_Edition.dwPrice;
	else 
		return NULL;		
}


BYTE CBaseItem::GetMaxQuantity()
{	
	if(dwCode_ID & ITEM_KIND_SUPPLIES)
		return BaseItem_Supplies.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_ZODIAC)
		return BaseItem_Zodiac.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_MAGICARRAY)
		return BaseItem_MagicArray.bQuantityCnt;		
	else if(dwCode_ID & ITEM_KIND_MAGICFIELDARRAY)
		return BaseItem_MagicFieldArray.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_UPGRADE)
		return BaseItem_UpGrade.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_LIQUID)
		return BaseItem_LiQuid.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_EDITION)
		return BaseItem_Edition.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_SPECIAL)
		return BaseItem_Special.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_MATERIALS)
		return BaseItem_Materials.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_CONSUMABLE)
		return BaseItem_Consumable.bQuantityCnt;
	else if(dwCode_ID & ITEM_KIND_MIXUPGRADE)
		return BaseItem_MixUpgrade.bQuantityCnt;
	else 
		return 1;	
}


BYTE CBaseItem::GetGrade()
{
	if(dwCode_ID & ITEM_KIND_WEAPON)	
		return BaseItem_Weapon.bClassGrade;
	else if(dwCode_ID & ITEM_KIND_AROMR)
		return BaseItem_Armor.bClassGrade;
	else if(dwCode_ID & ITEM_KIND_ZODIAC)
		return BaseItem_Zodiac.bGrade;
	else if(dwCode_ID & ITEM_KIND_RIDE)
		return BaseItem_Ride.bGrade;
	else if(dwCode_ID & ITEM_KIND_SPECIAL)
		return BaseItem_Special.bClassGrade;
	else if(dwCode_ID & ITEM_KIND_GUARDIAN)
		return BaseItem_Guardian.bClassGrade;
	else if(dwCode_ID & ITEM_KIND_MAGICARRAY)
		return BaseItem_MagicArray.bGrade;
	else if(dwCode_ID & ITEM_KIND_MATERIALS)
		return BaseItem_Materials.bGrade;
	else if(dwCode_ID & ITEM_KIND_MIXUPGRADE)
		return BaseItem_MixUpgrade.bGrade;		
	else 
		return 0;	
}


BYTE CBaseItem::GetWeaponKind()
{
	if(dwCode_ID & ITEM_KIND_WEAPON)
		return BaseItem_Weapon.bWeaponKind;	
	else if(dwCode_ID & ITEM_KIND_AROMR)
		return BaseItem_Armor.bArmorKind;
	else
		return 0xff;
}


BYTE CBaseItem::GetWeaponHand()
{
	if(dwCode_ID & ITEM_KIND_WEAPON)
		return BaseItem_Weapon.bHand;
	else
		return NULL;
}


WORD CBaseItem::GetSetID()
{
	if(dwCode_ID & ITEM_KIND_WEAPON)
		return BaseItem_Weapon.wSetID;
	else if(dwCode_ID & ITEM_KIND_AROMR)
		return BaseItem_Armor.wSetID;
	else
		return NULL;	
}


WORD CBaseItem::GetWeight()
{
	if(dwCode_ID & ITEM_KIND_WEAPON)
		return BaseItem_Weapon.wWeight;
	else if(dwCode_ID & ITEM_KIND_AROMR)
		return BaseItem_Armor.wWeight;
	else if(dwCode_ID & ITEM_KIND_BAG)
		return NULL;
	else if(dwCode_ID & ITEM_KIND_GUARDIAN)
		return BaseItem_Guardian.wWeight;
	else if(dwCode_ID & ITEM_KIND_RIDE)
		return BaseItem_Ride.wWeight;
	else if(dwCode_ID & ITEM_KIND_MIXUPGRADE)
		return BaseItem_MixUpgrade.wWeight;
	else if(dwCode_ID & ITEM_KIND_UPGRADE)
		return BaseItem_UpGrade.wWeight;
	else if(dwCode_ID & ITEM_KIND_LIQUID)
		return BaseItem_LiQuid.wWeight;
	else if(dwCode_ID & ITEM_KIND_EDITION)
		return BaseItem_Edition.wWeight;
	else if(dwCode_ID & ITEM_KIND_ZODIAC)
		return BaseItem_Zodiac.wWeight;
	else if(dwCode_ID & ITEM_KIND_SUPPLIES)
		return BaseItem_Supplies.wWeight;
	else if(dwCode_ID & ITEM_KIND_SPECIAL)
		return BaseItem_Special.wWeight;
	else if(dwCode_ID & ITEM_KIND_MATERIALS)
		return BaseItem_Materials.wWeight;
	else if(dwCode_ID & ITEM_KIND_MAGICARRAY)
		return BaseItem_MagicArray.wWeight;
	else if(dwCode_ID & ITEM_KIND_MAGICFIELDARRAY)
		return BaseItem_MagicFieldArray.wWeight;
	else if(dwCode_ID & ITEM_KIND_CONSUMABLE)
		return BaseItem_Consumable.wWeight;	
	else 
		return NULL;
}


BYTE CBaseItem::GetOverlap()
{
	if(dwCode_ID & ITEM_KIND_MIXUPGRADE)
		return BaseItem_MixUpgrade.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_UPGRADE)
		return BaseItem_UpGrade.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_LIQUID)
		return BaseItem_LiQuid.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_EDITION)
		return BaseItem_Edition.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_ZODIAC)
		return BaseItem_Zodiac.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_SUPPLIES)
		return BaseItem_Supplies.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_SPECIAL)
		return BaseItem_Special.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_MATERIALS)
		return BaseItem_Materials.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_MAGICARRAY)
		return BaseItem_MagicArray.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_MAGICFIELDARRAY)
		return BaseItem_MagicFieldArray.bOverCnt;
	else if(dwCode_ID & ITEM_KIND_CONSUMABLE)
		return BaseItem_Consumable.bOverCnt;
	else 
		return NULL;
}

BYTE CBaseItem::GetMovable()
{
	return bMovable;
}