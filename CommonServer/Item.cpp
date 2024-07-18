#include "StdAfx.h"
#include "BaseItem.h"
#include "Item.h"
#include "ItemManagerDefine.h"
#include "CommonHash.h"


extern LPBASEITEM_HASH g_pBaseItemHash;


WORD CItem::GetID()	const
{
	return m_wItemID;
}


void CItem::SetID(WORD wItemID)
{
	m_wItemID = wItemID;
}


void CItem::SetSerialNum(ITEM_SERIAL* pSerial)
{
	m_Serial = *pSerial;
}


void CItem::SetSerialNum(ITEM_SERIAL serial)
{
	m_Serial = serial;
}


const ITEM_SERIAL* CItem::GetSerial() const
{
	return &m_Serial;
}


CBaseItem* CItem::GetBaseItem() const
{
	return g_pBaseItemHash->GetData(GetID());
}


BOOL CItem::IsOverlapItem()
{
	return BOOL( GetBaseItem()->GetOverlap() );	
}


DWORD CItem::GetKind()
{
	return GetBaseItem()->dwCode_ID;
}


BYTE CItem::GetWeaponKind()
{
	return GetBaseItem()->GetWeaponKind();
}


ITEM_WEAPON_TYPE CItem::GetWeaponType()
{
	return ITEM_WEAPON_TYPE( GetBaseItem()->GetWeaponHand() );
}


BYTE CItem::GetType()
{
	return GetBaseItem()->bCode_Type;
}


BYTE CItem::GetQuantity() const
{	
	if(NULL == GetBaseItem())
		return NULL;

	DWORD dwCode_ID = GetBaseItem()->dwCode_ID;

	if(dwCode_ID & ITEM_KIND_SUPPLIES)
		return m_Item_Supplies.bQuantity;
	else if(dwCode_ID & ITEM_KIND_ZODIAC)
		return m_Item_Zodiac.bQuantity;
	else if(dwCode_ID & ITEM_KIND_MAGICARRAY)
		return m_Item_MagicArray.bQuantity;		
	else if(dwCode_ID & ITEM_KIND_MAGICFIELDARRAY)
		return m_Item_MagicField_Array.bQuantity;
	else if(dwCode_ID & ITEM_KIND_UPGRADE)
		return m_Item_UpGreade.bQuantity;
	else if(dwCode_ID & ITEM_KIND_LIQUID)
		return m_Item_LiQuid.bQuantity;
	else if(dwCode_ID & ITEM_KIND_EDITION)
		return m_Item_Edition.bQuantity;
	else if(dwCode_ID & ITEM_KIND_SPECIAL)
		return m_Item_Special.bQuantity;
//	else if(dwCode_ID & ITEM_KIND_MATERIALS)// : 050111 hwoarang
//		return m_Item_Materials.bQuantity;
	else if(dwCode_ID & ITEM_KIND_CONSUMABLE)
		return m_Item_Consumable.bQuantity;
	else if(dwCode_ID & ITEM_KIND_MIXUPGRADE)
		return m_Item_Mix_Upgrade.bQuantity;
	else 
		return 1;			
}


void CItem::SetQuantity(BYTE bQuantity)
{
	if(NULL == GetBaseItem())
		return;
	
	DWORD dwCode_ID = GetBaseItem()->dwCode_ID;

	if(dwCode_ID & ITEM_KIND_SUPPLIES)
		m_Item_Supplies.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_ZODIAC)
		m_Item_Zodiac.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_MAGICARRAY)
		m_Item_MagicArray.bQuantity = bQuantity;		
	else if(dwCode_ID & ITEM_KIND_MAGICFIELDARRAY)
		m_Item_MagicField_Array.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_UPGRADE)
		m_Item_UpGreade.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_LIQUID)
		m_Item_LiQuid.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_EDITION)
		m_Item_Edition.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_SPECIAL)
		m_Item_Special.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_MATERIALS)
		m_Item_Materials.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_CONSUMABLE)
		m_Item_Consumable.bQuantity = bQuantity;
	else if(dwCode_ID & ITEM_KIND_MIXUPGRADE)
		m_Item_Mix_Upgrade.bQuantity = bQuantity;
	else 
		return;		
}


int	CItem::GetOptionCount()
{
	int iOption = 0;

	while(wAttr[iOption].uCode != 0 && iOption < MAX_ITEM_ATTR)
	{
		iOption++;
	}

	return iOption;
}


void CItem::SetGuardianEggCompleteTime(DWORD dwCompleteTime)
{
	DWORD dwCode_ID = GetBaseItem()->dwCode_ID;

	if(dwCode_ID & ITEM_KIND_GUARDIAN)	
	{
		m_Item_Guardian.dwCompleteTime = dwCompleteTime;

		if(GetBaseItem()->BaseItem_Guardian.wCompleteTime < dwCompleteTime)
		{
			m_Item_Guardian.dwCompleteTime = GetBaseItem()->BaseItem_Guardian.wCompleteTime;
		}
	}	
}











