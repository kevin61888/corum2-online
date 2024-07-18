#include "stdafx.h"
#include "ItemMoveManager.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "User.h"
#include "OwnServer.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "Map.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "MonsterManager.h"
#include "TraderMonster.h"
#include "ItemNativeManager.h"
#include "../CommonServer/CommonHeader.h"


CItemMoveManager::CItemMoveManager()
{
	memset(CItemMoveManager::ItemMoveProc,0,sizeof(CItemMoveManager::ItemMoveProc));
	memset(m_bSlotPool,0,sizeof(m_bSlotPool));
	
	m_bSlotPool[ITEM_NATIVE_EQUIP]			= MAX_EQUIP_POOL;
	m_bSlotPool[ITEM_NATIVE_INV_LARGE]		= MAX_INV_LARGE_POOL;
	m_bSlotPool[ITEM_NATIVE_INV_SMALL]		= MAX_INV_SMALL_POOL;
	m_bSlotPool[ITEM_NATIVE_INV_GUARDIAN]	= MAX_INV_GUARDIAN_POOL - MAX_INV_GUARDIAN_POOL;
	m_bSlotPool[ITEM_NATIVE_BELT]			= MAX_BELT_POOL;
	m_bSlotPool[ITEM_NATIVE_BANK_LARGE]		= MAX_BANK_LARGE_POOL;
	m_bSlotPool[ITEM_NATIVE_BANK_SMALL]		= MAX_BANK_SMALL_POOL;
	m_bSlotPool[ITEM_NATIVE_TRADE]			= MAX_UPGRADE_POOL;
	m_bSlotPool[ITEM_NATIVE_UPGRADE]		= MAX_UPGRADE_POOL;
	m_bSlotPool[ITEM_NATIVE_MAKING]			= MAX_MAKING_POOL;
	m_bSlotPool[ITEM_NATIVE_PLAYERSHOP]		= MAX_PLAYERSHOP_POOL;
	m_bSlotPool[ITEM_NATIVE_GUARDIAN]		= MAX_GUARDIAN_POOL - MAX_GUARDIAN_POOL;

	InitItemMoveProc();
}


CItemMoveManager::~CItemMoveManager()
{
	
}


void CItemMoveManager::InitItemMoveProc()
{
	ItemMoveProc[ITEM_NATIVE_INV_LARGE][ITEM_NATIVE_TRADE]		= LinkSlotChangeInvToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_INV_LARGE][ITEM_NATIVE_UPGRADE]	= LinkSlotChangeInvToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_INV_LARGE][ITEM_NATIVE_MAKING]		= LinkSlotChangeInvToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_INV_LARGE][ITEM_NATIVE_PLAYERSHOP] = LinkSlotChangeInvToLinkProcess;
	
	ItemMoveProc[ITEM_NATIVE_INV_SMALL][ITEM_NATIVE_TRADE]		= LinkSlotChangeInvToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_INV_SMALL][ITEM_NATIVE_UPGRADE]	= LinkSlotChangeInvToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_INV_SMALL][ITEM_NATIVE_MAKING]		= LinkSlotChangeInvToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_INV_SMALL][ITEM_NATIVE_PLAYERSHOP] = LinkSlotChangeInvToLinkProcess;
	
	ItemMoveProc[ITEM_NATIVE_INV_LARGE][ITEM_NATIVE_TILE]		= DropAndPickupItemProcess;
	ItemMoveProc[ITEM_NATIVE_INV_SMALL][ITEM_NATIVE_TILE]		= DropAndPickupItemProcess;
	ItemMoveProc[ITEM_NATIVE_TILE][ITEM_NATIVE_INV_LARGE]		= DropAndPickupItemProcess;
	ItemMoveProc[ITEM_NATIVE_TILE][ITEM_NATIVE_INV_SMALL]		= DropAndPickupItemProcess;
	ItemMoveProc[ITEM_NATIVE_TILE][ITEM_NATIVE_BELT]			= DropAndPickupItemProcess;
	ItemMoveProc[ITEM_NATIVE_INV_KARZ][ITEM_NATIVE_TILE]		= DropAndPickupItemProcess;

	ItemMoveProc[ITEM_NATIVE_INV_KARZ][ITEM_NATIVE_BANK_KARZ]	= KarzMoveProcess;
	ItemMoveProc[ITEM_NATIVE_BANK_KARZ][ITEM_NATIVE_INV_KARZ]	= KarzMoveProcess;

	ItemMoveProc[ITEM_NATIVE_UPGRADE][ITEM_NATIVE_UPGRADE]		= LinkSlotChangeLinkToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_MAKING][ITEM_NATIVE_MAKING]		= LinkSlotChangeLinkToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_PLAYERSHOP][ITEM_NATIVE_PLAYERSHOP]= LinkSlotChangeLinkToLinkProcess;
	ItemMoveProc[ITEM_NATIVE_TRADE][ITEM_NATIVE_TRADE]			= LinkSlotChangeLinkToLinkProcess;

	ItemMoveProc[ITEM_NATIVE_UPGRADE][ITEM_NATIVE_INV_LARGE]	= LinkSlotChangeLinkToInvProcess;
	ItemMoveProc[ITEM_NATIVE_UPGRADE][ITEM_NATIVE_INV_SMALL]	= LinkSlotChangeLinkToInvProcess;	
	ItemMoveProc[ITEM_NATIVE_PLAYERSHOP][ITEM_NATIVE_INV_LARGE] = LinkSlotChangeLinkToInvProcess;
	ItemMoveProc[ITEM_NATIVE_PLAYERSHOP][ITEM_NATIVE_INV_SMALL] = LinkSlotChangeLinkToInvProcess;
	ItemMoveProc[ITEM_NATIVE_TRADE][ITEM_NATIVE_INV_LARGE]		= LinkSlotChangeLinkToInvProcess;
	ItemMoveProc[ITEM_NATIVE_TRADE][ITEM_NATIVE_INV_SMALL]		= LinkSlotChangeLinkToInvProcess;
	ItemMoveProc[ITEM_NATIVE_MAKING][ITEM_NATIVE_INV_LARGE]		= LinkSlotChangeLinkToInvProcess;
	ItemMoveProc[ITEM_NATIVE_MAKING][ITEM_NATIVE_INV_SMALL]		= LinkSlotChangeLinkToInvProcess;	
}


BOOL CItemMoveManager::IsAvailability(CUser* pUser
									  ,CDungeonLayer** pLayer
									  ,CTDS_ITEM_MOVE* const pItemMove)
{
	if(!pUser)	return FALSE;
	if(!pUser->GetCurDungeon())	return FALSE;
	if( pUser->GetUnitStatus() == UNIT_STATUS_DEAD )	return FALSE;
	if( pUser->GetUnitStatus() == UNIT_STATUS_CASTING )	return FALSE;
	
	*pLayer =  pUser->GetCurDungeonLayer();

	if ( NULL == *pLayer )							return FALSE;
	if ( CheckShop(pUser) )							return FALSE;
	if ( pUser->m_byRest == 1 )						return FALSE;
	if ( pItemMove->bQuantity == 0 )				return FALSE;		
	if ( pUser->m_bPortalMoving == 1 )				return FALSE;
	if ( pItemMove->bSectionNum == 0 )				return FALSE;
	if ( pItemMove->bSrc >= MAX_SRC)				return FALSE;
	if ( pItemMove->bDest >= MAX_DEST)				return FALSE;
	
	return TRUE;
}


BOOL CItemMoveManager::IsSameNative(ITEM_NATIVE eSlotIDSrc,ITEM_NATIVE eSlotIDDest)
{
	if(eSlotIDSrc == eSlotIDDest)
		return TRUE;
	else
		return FALSE;	
}


BOOL CItemMoveManager::IsSrcToDestCorrect(ITEM_NATIVE eItemNativeSrc,ITEM_NATIVE eItemNativeDest)
{
	switch(eItemNativeSrc)
	{
	case ITEM_NATIVE_EQUIP:
		if( eItemNativeDest == ITEM_NATIVE_EQUIP ||	eItemNativeDest == ITEM_NATIVE_INV_LARGE )
			return TRUE;
		break;
	case ITEM_NATIVE_INV_LARGE:
		if(	eItemNativeDest == ITEM_NATIVE_INV_LARGE	||	eItemNativeDest == ITEM_NATIVE_EQUIP		||
			eItemNativeDest == ITEM_NATIVE_INV_GUARDIAN ||	eItemNativeDest == ITEM_NATIVE_BANK_LARGE	||
			eItemNativeDest == ITEM_NATIVE_TRADE		||	eItemNativeDest == ITEM_NATIVE_UPGRADE		||
			eItemNativeDest == ITEM_NATIVE_PLAYERSHOP	||  eItemNativeDest == ITEM_NATIVE_TRADERMONSTER_INV_LARGE ||
			eItemNativeDest == ITEM_NATIVE_TILE			||	eItemNativeDest == ITEM_NATIVE_DUNGEONMANAGER)
		{
			return TRUE;			
		}
		break;
	case ITEM_NATIVE_INV_SMALL:
		if( eItemNativeDest == ITEM_NATIVE_INV_SMALL	||	eItemNativeDest == ITEM_NATIVE_BELT			||
			eItemNativeDest == ITEM_NATIVE_BANK_SMALL	||	eItemNativeDest == ITEM_NATIVE_TRADE		||
			eItemNativeDest == ITEM_NATIVE_UPGRADE		||	eItemNativeDest == ITEM_NATIVE_MAKING		||
			eItemNativeDest == ITEM_NATIVE_PLAYERSHOP   ||	eItemNativeDest == ITEM_NATIVE_TRADERMONSTER_INV_SMALL ||
			eItemNativeDest == ITEM_NATIVE_TILE			||  eItemNativeDest == ITEM_NATIVE_GUARDIAN		||
			eItemNativeDest == ITEM_NATIVE_DUNGEONMANAGER)
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_INV_GUARDIAN:
		if(	eItemNativeDest == ITEM_NATIVE_INV_GUARDIAN	||	eItemNativeDest == ITEM_NATIVE_INV_LARGE )
			return TRUE;
		break;
	case ITEM_NATIVE_BELT:
		if( eItemNativeDest == ITEM_NATIVE_BELT			||	eItemNativeDest == ITEM_NATIVE_INV_SMALL )
			return TRUE;
		break;
	case ITEM_NATIVE_BANK_LARGE:
		if( eItemNativeDest == ITEM_NATIVE_BANK_LARGE	||	eItemNativeDest == ITEM_NATIVE_INV_LARGE )
			return TRUE;
		break;
	case ITEM_NATIVE_BANK_SMALL:
		if( eItemNativeDest == ITEM_NATIVE_BANK_SMALL	||	eItemNativeDest == ITEM_NATIVE_INV_SMALL )
			return TRUE;
		break;
	case ITEM_NATIVE_TRADE:
		if( eItemNativeDest == ITEM_NATIVE_TRADE		||
			eItemNativeDest == ITEM_NATIVE_INV_LARGE	||
			eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_UPGRADE:
		if( eItemNativeDest == ITEM_NATIVE_UPGRADE		||
			eItemNativeDest == ITEM_NATIVE_INV_LARGE	||
			eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_MAKING:
		if( eItemNativeDest == ITEM_NATIVE_MAKING		||
			eItemNativeDest == ITEM_NATIVE_INV_LARGE	||
			eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_PLAYERSHOP:
		if( eItemNativeDest == ITEM_NATIVE_PLAYERSHOP	||
			eItemNativeDest == ITEM_NATIVE_INV_LARGE	||
			eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_DUNGEONMANAGER:
		if( eItemNativeDest == ITEM_NATIVE_INV_LARGE	||	eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_TRADERMONSTER_INV_LARGE:
		if( eItemNativeDest == ITEM_NATIVE_INV_LARGE )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_TRADERMONSTER_INV_SMALL:
		if( eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_TILE:
		if( eItemNativeDest == ITEM_NATIVE_INV_LARGE	||	eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_INV_KARZ:
		if( eItemNativeDest == ITEM_NATIVE_TILE			||	eItemNativeDest == ITEM_NATIVE_BANK_KARZ )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_GUARDIAN:
		if( eItemNativeDest == ITEM_NATIVE_GUARDIAN		||  eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
	default:
		return FALSE;
	}

	return FALSE;
}


BOOL CItemMoveManager::IsDestNativeToCorrectType(CUser* const pUser, ITEM_NATIVE eItemNativeDest, BYTE bySlotIndex, const CItem* const pItemNativeSrc)
{
	if( !IsItemNativePlayer(eItemNativeDest) && !IsItemNativeTraderMonster(eItemNativeDest) )
	{
		return FALSE;
	}

	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItemNativeSrc->GetID());

	if(NULL == pBaseItem)
	{
		return FALSE;
	}

	ITEM_TYPE eType		= GetItemType(pItemNativeSrc->GetID());
	DWORD wKind			= GetItemKind(pItemNativeSrc->GetID());
	WORD wItemID		= pItemNativeSrc->GetID(); 
	BYTE byWeaponKind	= pBaseItem->GetWeaponKind();	
		
	switch(eItemNativeDest)
	{
	case ITEM_NATIVE_EQUIP:
		if(	eType == ITEM_TYPE_LARGE &&	
			(wKind == ITEM_KIND_WEAPON || wKind == ITEM_KIND_AROMR || wKind == ITEM_KIND_RIDE || wKind == ITEM_KIND_BAG))
		{
			switch(bySlotIndex)
			{
			case EQUIP_TYPE_RHAND1:
			case EQUIP_TYPE_RHAND2:
				if(byWeaponKind >= ITEM_CLASS_SWARD && byWeaponKind <= ITEM_CLASS_STAFF)
					return TRUE;
				break;
			case EQUIP_TYPE_LHAND1:
			case EQUIP_TYPE_LHAND2:	
				if(byWeaponKind == ITEM_CLASS_SHIELD)
					return TRUE;
				else if((byWeaponKind >= ITEM_CLASS_KATAL && byWeaponKind <= ITEM_CLASS_SPEAR) ||
					byWeaponKind == ITEM_CLASS_STAFF)
				{
					return TRUE;
				}
				break;
			case EQUIP_TYPE_RRING1:
			case EQUIP_TYPE_RRING2:
			case EQUIP_TYPE_RRING3:
				if(byWeaponKind == ITEM_CLASS_RING)
					return TRUE;
				break;
			case EQUIP_TYPE_LRING1:
			case EQUIP_TYPE_LRING2:
			case EQUIP_TYPE_LRING3:
				break;
			case EQUIP_TYPE_BOOTS:
				if(byWeaponKind == ITEM_CLASS_BOOTS)
					return TRUE;
				break;
			case EQUIP_TYPE_AMULE1:	
				break;
			case EQUIP_TYPE_AMULE2:
				if(byWeaponKind == ITEM_CLASS_AMULET)
					return TRUE;
				break;
			case EQUIP_TYPE_AMULE3:
				break;
			case EQUIP_TYPE_BELT:
				if(byWeaponKind == ITEM_CLASS_BELT)
					return TRUE;
				break;
			case EQUIP_TYPE_MAIL:
				if(byWeaponKind == ITEM_CLASS_ARMOR)
					return TRUE;
				break;
			case EQUIP_TYPE_HELMET:
				if(byWeaponKind == ITEM_CLASS_HELMET)
					return TRUE;	
				break;
			case EQUIP_TYPE_RIDE:
				if(wKind == ITEM_KIND_RIDE)
					return TRUE;
				break;
			case EQUIP_TYPE_BAG:
				if(wKind == ITEM_KIND_BAG)
					return TRUE;
				break;			
			case EQUIP_TYPE_RGLOVE:
			case EQUIP_TYPE_LGLOVE:
				if(byWeaponKind == ITEM_CLASS_GLOVE)
					return TRUE;
				break;
			default:
				return FALSE;
			}
		}
		break;
	case ITEM_NATIVE_INV_LARGE:	
		if(eType == ITEM_TYPE_LARGE)
			return TRUE;
		break;
	case ITEM_NATIVE_INV_SMALL:	
		if(eType == ITEM_TYPE_SMALL)
			return TRUE;
		break;
	case ITEM_NATIVE_BANK_LARGE:
		if(eType == ITEM_TYPE_LARGE)
			return TRUE;
		break;
	case ITEM_NATIVE_BANK_SMALL:
		if(eType == ITEM_TYPE_SMALL)
			return TRUE;
		break;
	case ITEM_NATIVE_INV_GUARDIAN:
		if(eType == ITEM_TYPE_LARGE && wKind == ITEM_KIND_GUARDIAN)
		{
			if(bySlotIndex == 0)
			{
				return TRUE;
			}			
		}
		break;
	case ITEM_NATIVE_BELT:
		if(eType == ITEM_TYPE_SMALL && (wKind == ITEM_KIND_SUPPLIES || wKind == ITEM_KIND_ZODIAC))
			return TRUE;
		break;
	case ITEM_NATIVE_TRADE:
		if(eType == ITEM_TYPE_LARGE || eType == ITEM_TYPE_SMALL)
		{
			if( CheckTrade(pUser->m_nItemTradeSeriel) )
			{
				return TRUE;
			}			
		}
		break;
	case ITEM_NATIVE_UPGRADE:
		if(bySlotIndex == ITEM_UPGRADE_WEAPON) 
		{
			if(eType == ITEM_TYPE_LARGE &&	(wKind == ITEM_KIND_WEAPON || wKind == ITEM_KIND_AROMR))
			{
				ITEM_NATIVE_INFO ItemNativeInfo;
				memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
				const CItem*			pItemNative			= NULL;
				const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
			
				ItemNativeInfo.eSlotID		= eItemNativeDest;
				ItemNativeInfo.dwOwnerID	= pUser->GetID();
				ItemNativeInfo.bySlotIndex	= bySlotIndex;
				
				pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
				pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

				if(pItemNative && pItemNative->GetID() != 0)
				{
					// 기존슬롯에 업그레이드 하려고 올려논 아이템이 있다.
					return FALSE;
				}
				else if(byWeaponKind == ITEM_CLASS_RING || byWeaponKind == ITEM_CLASS_AMULET)
				{
					// 반지와 아뮬렛이면 업그레이드 안됨
					return FALSE;
				}
				else if(pItemNativeSrc->m_bSlot.uMaxSlot == 0)
				{
					// 아이템의 슬롯이 0이면 안됨
					return FALSE;
				}
				else if(wKind == ITEM_KIND_WEAPON	&& pBaseItem->BaseItem_Weapon.bItemGrade == 0 ||
						wKind == ITEM_KIND_AROMR && pBaseItem->BaseItem_Armor.bItemGrade == 0)
				{
					// 아이템의 그레이드가 0이면 안됨
					return FALSE;
				}
				
				for(BYTE i = 1; i < MAX_UPGRADE_POOL; i++ )
				{
					ITEM_NATIVE_INFO ItemNativeInfo;
					memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
					const CItem*			pItemNative			= NULL;
					const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
				
					ItemNativeInfo.eSlotID		= eItemNativeDest;
					ItemNativeInfo.dwOwnerID	= pUser->GetID();
					ItemNativeInfo.bySlotIndex	= i;
					
					pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
					pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

					if(NULL == pItemNative)
						continue;
					
					DWORD wKindAddition = GetItemKind(pItemNative->GetID());
					CBaseItem* pBaseItemAddition = pItemNative->GetBaseItem();
					if(NULL == pBaseItemAddition)
						return FALSE;

					if(wKindAddition == ITEM_KIND_UPGRADE)
					{
						if(wKind == ITEM_KIND_WEAPON)
						{
							if(pBaseItem->BaseItem_Weapon.bItemGrade != pBaseItemAddition->BaseItem_UpGrade.wW_Grade)
							{
								return FALSE;
							}								
						}
						else if(wKind == ITEM_KIND_AROMR)
						{
							if(pBaseItem->BaseItem_Armor.bItemGrade != pBaseItemAddition->BaseItem_UpGrade.wW_Grade)
							{
								return FALSE;
							}								
						}

						if(pBaseItemAddition->BaseItem_UpGrade.wFormula[byWeaponKind] == 0)
						{
							return FALSE;
						}
					}
					else if(wKindAddition == ITEM_KIND_LIQUID)
					{
						if(wKind == ITEM_KIND_WEAPON)
						{
							if(pBaseItem->BaseItem_Weapon.bItemGrade != pBaseItemAddition->BaseItem_LiQuid.wW_Grade)
							{
								return FALSE;
							}								
						}
						else if(wKind == ITEM_KIND_AROMR)
						{
							if(pBaseItem->BaseItem_Armor.bItemGrade != pBaseItemAddition->BaseItem_LiQuid.wW_Grade)
							{
								return FALSE;
							}								
						}
					}
					else if(wKindAddition == ITEM_KIND_EDITION)
					{
						if(pBaseItemAddition->BaseItem_Edition.wFormula[byWeaponKind] == 0)
						{
							return FALSE;
						}
					}					
				}

				return TRUE;
			}
		}
		else if(bySlotIndex == ITEM_UPGRADE_UPGRADE ||
				bySlotIndex == ITEM_UPGRADE_LIQUID	||
				bySlotIndex == ITEM_UPGRDAE_EDITION)
		{
			if(	eType == ITEM_TYPE_SMALL &&
				(wKind == ITEM_KIND_UPGRADE || wKind == ITEM_KIND_LIQUID || wKind == ITEM_KIND_EDITION))
			{
				ITEM_NATIVE_INFO ItemNativeInfo;
				memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
				const CItem*			pItemNative			= NULL;
				const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
			
				ItemNativeInfo.eSlotID		= eItemNativeDest;
				ItemNativeInfo.dwOwnerID	= pUser->GetID();
				ItemNativeInfo.bySlotIndex	= bySlotIndex;
				
				pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
				pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

				if(pItemNative && pItemNative->GetID() != 0)
				{
					// 기존슬롯에 올려논 업그레이드,리퀴드,에디션 아이템이 있다.
					return FALSE;
				}

				ItemNativeInfo.bySlotIndex	= ITEM_UPGRADE_WEAPON;
				pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
				pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

				// 강화할 아이템이 올려져 있으면 강화서,그레이드,에디션이 매칭되는지 
				if(pItemNative && pItemNative->GetID())
				{
					DWORD wKindWeapon = GetItemKind(pItemNative->GetID());
					CBaseItem* pBaseItemWeapon = pItemNative->GetBaseItem();

					if(NULL == pBaseItemWeapon)
						return FALSE;

					BYTE byWeaponKind = pBaseItemWeapon->GetWeaponKind();	

					if(wKind == ITEM_KIND_UPGRADE)
					{
						if(wKindWeapon == ITEM_KIND_WEAPON)
						{
							if(pBaseItemWeapon->BaseItem_Weapon.bItemGrade != pBaseItem->BaseItem_UpGrade.wW_Grade)
							{
								return FALSE;
							}
						}
						else if(wKindWeapon == ITEM_KIND_AROMR)
						{
							if(pBaseItemWeapon->BaseItem_Armor.bItemGrade != pBaseItem->BaseItem_UpGrade.wW_Grade)
							{
								return FALSE;
							}
						}

						if(pBaseItem->BaseItem_UpGrade.wFormula[byWeaponKind] == 0)
						{
							return FALSE;
						}
					}
					else if(wKind == ITEM_KIND_LIQUID)
					{
						if(wKindWeapon == ITEM_KIND_WEAPON)
						{
							if(pBaseItemWeapon->BaseItem_Weapon.bItemGrade != pBaseItem->BaseItem_LiQuid.wW_Grade)
							{
								return FALSE;
							}
						}
						else if(wKindWeapon == ITEM_KIND_AROMR)
						{
							if(pBaseItemWeapon->BaseItem_Armor.bItemGrade != pBaseItem->BaseItem_LiQuid.wW_Grade)
							{
								return FALSE;
							}
						}
					}
					else if(wKind == ITEM_KIND_EDITION)
					{
						if(pBaseItem->BaseItem_Edition.wFormula[byWeaponKind] == 0)
						{
							return FALSE;
						}
					}

					// 혹시 올린거 또 올렸어?
					for(BYTE i = 1; i < MAX_UPGRADE_POOL; i++)
					{
						ITEM_NATIVE_INFO ItemNativeInfo;
						memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
						const CItem*			pItemNative			= NULL;
						const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
					
						ItemNativeInfo.eSlotID		= eItemNativeDest;
						ItemNativeInfo.dwOwnerID	= pUser->GetID();
						ItemNativeInfo.bySlotIndex	= i;
						
						pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
						pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

						if(NULL == pItemNative)
							continue;

						DWORD wKindAddition = GetItemKind(pItemNative->GetID());
						CBaseItem* pBaseItemAddition = pItemNative->GetBaseItem();
						if(NULL == pBaseItemAddition)
							return FALSE;

						// 같은 종류가 올라가 있구만.니미
						if(wKind == wKindAddition)
						{
							return FALSE;
						}
					}
				}
				else
				{
					// 강화할 아이템은 올려려져 있지 않은데 강화서나 리퀴드가 올려져 있을 경우
					for(BYTE i = 1; i < MAX_UPGRADE_POOL; i++ )
					{
						ITEM_NATIVE_INFO ItemNativeInfo;
						memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
						const CItem*			pItemNative			= NULL;
						const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
					
						ItemNativeInfo.eSlotID		= eItemNativeDest;
						ItemNativeInfo.dwOwnerID	= pUser->GetID();
						ItemNativeInfo.bySlotIndex	= i;
						
						pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
						pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

						if(NULL == pItemNative)
							continue;

						DWORD wKindAddition = GetItemKind(pItemNative->GetID());
						CBaseItem* pBaseItemAddition = pItemNative->GetBaseItem();
						if(NULL == pBaseItemAddition)
							return FALSE;

						if(wKind == ITEM_KIND_UPGRADE && wKindAddition == ITEM_KIND_LIQUID)
						{
							if(pBaseItem->BaseItem_UpGrade.wW_Grade != pBaseItemAddition->BaseItem_LiQuid.wW_Grade)
							{
								return FALSE;
							}
						}
						else if(wKind == ITEM_KIND_LIQUID && wKindAddition == ITEM_KIND_UPGRADE)
						{
							if(pBaseItem->BaseItem_LiQuid.wW_Grade != pBaseItemAddition->BaseItem_UpGrade.wW_Grade)
							{
								return FALSE;
							}
						}

						// 같은 종류가 올라가 있구만.니미
						if(wKind == wKindAddition)
						{
							return FALSE;
						}
					}
				}

				return TRUE;
			}
		}
		break;
	case ITEM_NATIVE_MAKING:
		if(bySlotIndex == ITEM_MAKING_RESULT)
		{
			if(eType == ITEM_TYPE_LARGE &&
				(wKind == ITEM_KIND_WEAPON || wKind == ITEM_KIND_RIDE || wKind == ITEM_KIND_GUARDIAN))
			{
				return TRUE;
			}
		}
		else if(bySlotIndex == ITEM_MAKING_MATERIALS1 ||
				bySlotIndex == ITEM_MAKING_MATERIALS2 ||
				bySlotIndex == ITEM_MAKING_ZODIAC)
		{
			if(eType == ITEM_TYPE_SMALL)
			{
				if(wKind == ITEM_KIND_MATERIALS)
				{
					if(pBaseItem->BaseItem_Materials.byOrder == 0)
					{
						// 제조못하는 아이템 
						return FALSE;
					}

					ITEM_MAKING_INFO* pMakingInfoSrc = g_pItemMakingInfoHash->GetData(
						pBaseItem->BaseItem_Materials.byOrder);

					if(NULL == pMakingInfoSrc)
					{
						// 제조 정보가 없따.
						return FALSE;
					}

					// 기존에 제조창에 같은 메터리얼 이나 다른 메터리얼이 있어? 
					for(BYTE i = 1; i < MAX_MAKING_POOL; i++ )
					{
						ITEM_NATIVE_INFO ItemNativeInfo;
						memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
						const CItem*			pItemNative			= NULL;
						const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
											
						ItemNativeInfo.eSlotID		= eItemNativeDest;
						ItemNativeInfo.dwOwnerID	= pUser->GetID();
						ItemNativeInfo.bySlotIndex	= i;
						
						pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
						pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

						if(NULL == pItemNative)
							continue;

						if(wItemID == pItemNative->GetID())
						{
							// 같은 아이템이 이미 올라가 있구마잉 ㅡ.-;;
							return FALSE;
						}

						CBaseItem*			pBaseItemMaterials = pItemNative->GetBaseItem();
						ITEM_MAKING_INFO*	pMakingInfo = g_pItemMakingInfoHash->GetData(
							pBaseItemMaterials->BaseItem_Materials.byOrder);
						
						if(NULL == pMakingInfo)
							continue;

						if(pMakingInfoSrc->wID != pMakingInfo->wID)
						{
							// 제조 아이디가 달름 
							return FALSE;
						}
					}

					// 제조창에 제조에 성공한 라쥐 아이템이 있을경우
					ITEM_NATIVE_INFO ItemNativeInfo;
					memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
					const CItem*			pItemNative			= NULL;
					const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
										
					ItemNativeInfo.eSlotID		= eItemNativeDest;
					ItemNativeInfo.dwOwnerID	= pUser->GetID();
					ItemNativeInfo.bySlotIndex	= ITEM_MAKING_RESULT;
					
					pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
					pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

					if(NULL != pItemNative)
					{
						if(pItemNative->GetID())
						{
							// 제조에 성공한 아이템이 있잖아.인벤에 느!!
							return FALSE;
						}
					}

					return TRUE;
				}
				else if(wKind == ITEM_KIND_ZODIAC && pBaseItem->GetID() == __ITEM_MAKING_INDEX__)
				{
					// 기존에 제조창에 조디악 카드 (큰게)가 있어?
					for(BYTE i = 1; i < MAX_MAKING_POOL; i++ )
					{
						ITEM_NATIVE_INFO ItemNativeInfo;
						memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
						const CItem*			pItemNative			= NULL;
						const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
											
						ItemNativeInfo.eSlotID		= eItemNativeDest;
						ItemNativeInfo.dwOwnerID	= pUser->GetID();
						ItemNativeInfo.bySlotIndex	= i;
						
						pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
						pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

						if(NULL == pItemNative)
							continue;

						if(wItemID == pItemNative->GetID())
						{
							// 같은 아이템이 이미 올라가 있구마잉 ㅡ.-;;
							return FALSE;
						}
					}

					// 제조창에 제조에 성공한 라쥐 아이템이 있을경우
					ITEM_NATIVE_INFO ItemNativeInfo;
					memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
					const CItem*			pItemNative			= NULL;
					const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
										
					ItemNativeInfo.eSlotID		= eItemNativeDest;
					ItemNativeInfo.dwOwnerID	= pUser->GetID();
					ItemNativeInfo.bySlotIndex	= ITEM_MAKING_RESULT;
					
					pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
					pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

					if(NULL != pItemNative)
					{
						if(pItemNative->GetID())
						{
							// 제조에 성공한 아이템이 있잖아.인벤에 느!!
							return FALSE;
						}
					}

					return TRUE;
				}
			}
		}
		break;
	case ITEM_NATIVE_PLAYERSHOP:
		if(eType == ITEM_TYPE_LARGE || eType == ITEM_TYPE_SMALL)	
			return TRUE;
		break;
	case ITEM_NATIVE_DUNGEONMANAGER:
		if(eType == ITEM_TYPE_LARGE)
		{
			if(wKind == ITEM_KIND_GUARDIAN)
			{
				if(pBaseItem->BaseItem_Guardian.bType == GUARDIAN_ITEM_TYPE_ADULT)
				{
					if(bySlotIndex == ITEM_DUNGEONMANAGER_GUARDIAN)
					{
						return TRUE;
					}
				}				
			}
		}
		else if(eType == ITEM_TYPE_SMALL)
		{
			if(wKind == ITEM_KIND_MATERIALS)
			{
				if(bySlotIndex == ITEM_DUNGEONMANAGER_MATERIALS)
				{
					switch(wItemID)
					{
						case __ITEM_METERIALS_FIRE_PROPERTY_INDEX__	:	return TRUE;
						case __ITEM_METERIALS_ICE_PROPERTY_INDEX__	:	return TRUE;
						case __ITEM_METERIALS_LIGHT_PROPERTY_INDEX__:	return TRUE;
						default:	return FALSE;
					}			
				}				
			}
		}
		break;
	case ITEM_NATIVE_TRADERMONSTER_INV_LARGE:
	case ITEM_NATIVE_TRADERMONSTER_INV_SMALL:
		if(eType == ITEM_TYPE_LARGE || eType == ITEM_TYPE_SMALL)	
			return TRUE;
		break;
	case ITEM_NATIVE_GUARDIAN:
		if(eType == ITEM_TYPE_SMALL)
		{
			if( wKind == ITEM_KIND_SUPPLIES )
			{
				if(pBaseItem->BaseItem_Supplies.bType == 3)
				{
					// HP
					return TRUE;
				}
			}			
			else if( wKind == ITEM_KIND_CONSUMABLE )
			{
				return TRUE;
			}
		}
	default:
		return FALSE;
	}

	return FALSE;
}


BOOL CItemMoveManager::IsNativeToCorrectSlotIndex(ITEM_NATIVE eItemNative,BYTE bySlotIndex)
{
	if( IsItemNativePlayer(eItemNative) )
	{
		if( m_bSlotPool[eItemNative] > bySlotIndex )
		{
			return TRUE;
		}
		else if( eItemNative == ITEM_NATIVE_INV_GUARDIAN || eItemNative == ITEM_NATIVE_GUARDIAN )
		{
			if(bySlotIndex == 0)
			{
				return TRUE;
			}
		}
	}
	else if( IsItemNativeTraderMonster(eItemNative) )
	{
		if( ITEM_NATIVE_TRADERMONSTER_INV_LARGE == eItemNative )
		{	
			if( MAX_TRADERMONSTER_INV_LARGE > bySlotIndex )
			{
				return TRUE;
			}
		}
		else if( ITEM_NATIVE_TRADERMONSTER_INV_SMALL == eItemNative )
		{
			if( MAX_TRADERMONSTER_INV_SMALL > bySlotIndex )
			{
				return TRUE;
			}
		}
	}	

	return FALSE;
}


void CItemMoveManager::InitItemMoveResultPacket(CTDS_ITEM_MOVE* const pItemMove
												,DSTC_ITEM_MOVE* const pItemMoveResult)
{
	pItemMoveResult->bSrc		= pItemMove->bSrc;
	pItemMoveResult->bSrcIndex	= pItemMove->bSrcIndex;
	pItemMoveResult->bDest		= pItemMove->bDest;
	pItemMoveResult->bDestIndex = pItemMove->bDestIndex;
	pItemMoveResult->bQuantity	= pItemMove->bQuantity;
	pItemMoveResult->dwMoney	= pItemMove->dwMoney;
	pItemMoveResult->dwUnitID	= pItemMove->dwUnitID;
}


void CItemMoveManager::Process(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	BYTE bSrc						= 0;
	BYTE bDest						= 0;
	CUser* pUser					= NULL;
	CDungeonLayer* pLayer			= NULL;
	CTDS_ITEM_MOVE* pItemMove		= NULL;
	DSTC_ITEM_MOVE ItemMoveResult;
		
	pUser		= (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	pItemMove	= (CTDS_ITEM_MOVE*)pMsg;
	
	if(!IsAvailability(pUser,&pLayer,pItemMove))
	{
		SendToItemMoveResult(dwConnectionIndex,&ItemMoveResult,GET_NOT_AVAILABILITY);		
		return;
	}

	InitItemMoveResultPacket(pItemMove, &ItemMoveResult);

	bSrc	= pItemMove->bSrc;
	bDest	= pItemMove->bDest;

	if(ItemMoveProc[bSrc][bDest])
	{
		(this->*ItemMoveProc[bSrc][bDest])(dwConnectionIndex,pUser,pLayer,pItemMove,&ItemMoveResult);		
	}
	else
	{
		DefaultSlotChangeProcess(dwConnectionIndex,pUser,pLayer,pItemMove,&ItemMoveResult);			
	}

	if(ItemMoveResult.bErrorCode == 0xff)
	{
		CheckSpeed(pUser);
	}
}


BOOL CItemMoveManager::TraderMonsterInvLargeAndInvLarge(CUser* const pUser
														, CDungeonLayer* const pLayer
														, CTDS_ITEM_MOVE* const pItemMove
														, DSTC_ITEM_MOVE* const pItemMoveResult
														, const CItem* pItemSrc
														, const CItem* pItemDest
														, ITEM_NATIVE_INFO* ItemNativeInfo_Src
														, ITEM_NATIVE_INFO* ItemNativeInfo_Dest
														, BYTE byQuantity
														, BYTE* byErrorCode)
{
	BOOL bIsEmpty			= FALSE;
	BYTE byFindSlotIndex	= 0xff;
	
	if( ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_TRADERMONSTER_INV_LARGE && 
		ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_INV_LARGE )
	{
		DWORD dwPrice		= pItemSrc->GetBaseItem()->GetPrice();
		DWORD dwUserMoney	= pUser->GetMoney();
		
		if(dwUserMoney < dwPrice)
		{
			*byErrorCode = BE_NOT_ENOUGH_MONEY;
			return FALSE;
		}

		// 들어갈 자리에 아이템이 없으면 
		if(pItemDest->GetID() == 0)
		{
			if( SetSlotItemNative(pLayer, pItemSrc, ItemNativeInfo_Dest) )
			{
				memcpy(&pItemMoveResult->Item,pItemSrc,sizeof(CItem));

				CItem cItem;
				memcpy(&cItem,pItemSrc,sizeof(CItem));
				cItem.SetSerialNum(g_pThis->GetNewSerial());
				
				// 상점아이템에 새로운 시리얼을 넣는다.
				if( SetSlotItemNative(pLayer, &cItem, ItemNativeInfo_Src) )
				{
					pUser->SetMoney(dwUserMoney - dwPrice);
					ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_CREATE_BY_SHOP,	pLayer->GetParent()->GetID()
						, (char*)pUser->GetName());	
					return TRUE;
				}
				else
				{
					*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
					return FALSE;
				}
			}
			else
			{
				*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
				return FALSE;
			}
		}
		else
		{
			// 들어갈 자리에 아이템이 있으면 
			pItemDest = pItemDest - pItemMove->bDestIndex;

			for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
			{
				if(pItemDest[i].GetID() == 0)
				{
					bIsEmpty = TRUE;
					byFindSlotIndex = i;
					break;
				}
			}

			// Find Empty Slot
			if(bIsEmpty)
			{
				ItemNativeInfo_Dest->bySlotIndex = byFindSlotIndex;

				if( SetSlotItemNative(pLayer, pItemSrc, ItemNativeInfo_Dest) )
				{
					memcpy(&pItemMoveResult->Item,pItemSrc,sizeof(CItem));

					CItem cItem;
					memcpy(&cItem,pItemSrc,sizeof(CItem));
					cItem.SetSerialNum(g_pThis->GetNewSerial());

					if( SetSlotItemNative(pLayer, &cItem, ItemNativeInfo_Src) )
					{
						pUser->SetMoney(dwUserMoney - dwPrice);
						ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_CREATE_BY_SHOP, pLayer->GetParent()->GetID()
							, (char*)pUser->GetName());	
						return TRUE;
					}
					else
					{
						*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
						return FALSE;
					}
				}
				else
				{
					*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
					return FALSE;
				}
			}
			else 
			{
				*byErrorCode = QUITE_FULL_SLOT;
				return FALSE;
			}
		}		
	}
	else if(ItemNativeInfo_Src->eSlotID ==  ITEM_NATIVE_INV_LARGE &&
			ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_TRADERMONSTER_INV_LARGE)
	{
		DWORD dwSellPrice	= pItemSrc->GetBaseItem()->GetSellPrice();
		DWORD dwUserMoney	= pUser->GetMoney();

		if(dwUserMoney + dwSellPrice  > __MAX_MONEY__)
		{
			*byErrorCode = BE_FULL_OF_MONEY;
			return FALSE;
		}

		ItemMoveLogProcess((CItem*)pItemSrc, ITEM_LOG_DELETE_BY_SHOP, pLayer->GetParent()->GetID()
			, (char*)pUser->GetName());	

		RemoveItemNative(ItemNativeInfo_Src);
		pUser->SetMoney(dwUserMoney + dwSellPrice);		
	}

	return TRUE;
}


BOOL CItemMoveManager::TraderMonsterInvSmallAndInvSmall(CUser* const pUser
														, CDungeonLayer* const pLayer
														, CTDS_ITEM_MOVE* const pItemMove
														, DSTC_ITEM_MOVE* const pItemMoveResult
														, const CItem* pItemSrc
														, const CItem* pItemDest
														, ITEM_NATIVE_INFO* ItemNativeInfo_Src
														, ITEM_NATIVE_INFO* ItemNativeInfo_Dest
														, BYTE byQuantity
														, BYTE* byErrorCode)
{
	BYTE		byTempQuantity	= byQuantity;
	CBaseItem*	pBaseItemDest	= NULL;
	
	if( ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_TRADERMONSTER_INV_SMALL &&
		ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_INV_SMALL )
	{
		DWORD dwPrice		= pItemSrc->GetBaseItem()->GetPrice();
		DWORD dwUserMoney	= pUser->GetMoney();

		if(dwUserMoney < (dwPrice * byQuantity) )
		{
			*byErrorCode = BE_NOT_ENOUGH_MONEY;
			return FALSE;
		}

		CItem cBuyItem;
		memcpy(&cBuyItem, pItemSrc, sizeof(CItem));
		cBuyItem.SetQuantity(byQuantity);

		memcpy(&pItemMoveResult->Item, &cBuyItem, sizeof(CItem));

		// 들어갈 자리에 아이템이 없으면
		if(pItemDest->GetID() == 0)
		{
			if( SetSlotItemNative(pLayer, &cBuyItem, ItemNativeInfo_Dest) )
			{
				CItem cItem;
				memcpy(&cItem,pItemSrc,sizeof(CItem));
				cItem.SetSerialNum(g_pThis->GetNewSerial());

				// 상점아이템에 새로운 시리얼을 넣는다.
				if( SetSlotItemNative(pLayer, &cItem, ItemNativeInfo_Src) )
				{
					pUser->SetMoney( dwUserMoney - (dwPrice * byQuantity) );
					ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_CREATE_BY_SHOP,	pLayer->GetParent()->GetID()
						, (char*)pUser->GetName());	
					return TRUE;
				}
				else
				{
					*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
					return FALSE;
				}
			}
			else
			{
				*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
				return FALSE;
			}
		}
		else
		{
			// 들어갈 자리에 아이템이 있으면
			pBaseItemDest = pItemDest->GetBaseItem();

			if(NULL == pBaseItemDest)
			{
				*byErrorCode = GET_CANNOT_BASEITEMPOINTER;
				return FALSE;	
			}	
			
			// 들어갈 자리에 아이템이 있고 소스아이템과 같은 아이디면 수량이 겹칠수 있는지 알아본다. 
			if(pItemSrc->GetID() == pItemDest->GetID())
			{
				BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
				BYTE byCnt			= byMaxQuantity - pItemDest->GetQuantity();

				if(byCnt >= byQuantity)
				{
					CItem cDestItem;
					memcpy(&cDestItem, pItemDest, sizeof(CItem));
					cDestItem.SetQuantity( cDestItem.GetQuantity() + byQuantity );

					if( SetSlotItemNative(pLayer, &cDestItem, ItemNativeInfo_Dest) )
					{
						dwUserMoney	= pUser->GetMoney();
						pUser->SetMoney( dwUserMoney - (dwPrice * byQuantity) );

						ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_CREATE_BY_SHOP,	pLayer->GetParent()->GetID()
							, (char*)pUser->GetName());	
						return TRUE;
					}
					else
					{
						*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
						return FALSE;
					}
				}
				else if(byCnt != 0)
				{
					CItem cDestItem;
					memcpy(&cDestItem, pItemDest, sizeof(CItem));
					cDestItem.SetQuantity( cDestItem.GetQuantity() + byCnt );

					SetSlotItemNative(pLayer, &cDestItem, ItemNativeInfo_Dest);

					byQuantity -= byCnt;

					dwUserMoney	= pUser->GetMoney();
					pUser->SetMoney( dwUserMoney - (dwPrice * byCnt) );
				}
			}

			// 여기까지 왔으면 사야할 cnt가 남은거다. 맨처음 슬롯부터 찾는다.
			pItemDest = pItemDest - pItemMove->bDestIndex;

			for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
			{
				if(pItemDest[i].GetID() == 0)
				{
					cBuyItem.SetQuantity(byQuantity);
					ItemNativeInfo_Dest->bySlotIndex = i;

					if( SetSlotItemNative(pLayer, &cBuyItem, ItemNativeInfo_Dest) )
					{
						CItem cItem;
						memcpy(&cItem,pItemSrc,sizeof(CItem));
						cItem.SetSerialNum(g_pThis->GetNewSerial());

						// 상점아이템에 새로운 시리얼을 넣는다.
						if( SetSlotItemNative(pLayer, &cItem, ItemNativeInfo_Src) )
						{
							dwUserMoney	= pUser->GetMoney();
							pUser->SetMoney( dwUserMoney - (dwPrice * byQuantity) );

							ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_CREATE_BY_SHOP,	pLayer->GetParent()->GetID()
								, (char*)pUser->GetName());	
							return TRUE;
						}
						else
						{
							*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
							return FALSE;
						}
					}
					else
					{
						*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
						return FALSE;
					}
				}
				else
				{
					// 들어갈 자리에 아이템이 있으면
					pBaseItemDest = pItemDest[i].GetBaseItem();

					if(NULL == pBaseItemDest)
					{
						*byErrorCode = GET_CANNOT_BASEITEMPOINTER;
						return FALSE;	
					}	
					
					// 들어갈 자리에 아이템이 있고 소스아이템과 같은 아이디면 수량이 겹칠수 있는지 알아본다. 
					if(pItemSrc->GetID() == pItemDest[i].GetID())
					{
						BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
						BYTE byCnt			= byMaxQuantity - pItemDest[i].GetQuantity();

						if(byCnt >= byQuantity)
						{
							CItem cDestItem;
							memcpy(&cDestItem, &pItemDest[i], sizeof(CItem));
							cDestItem.SetQuantity( cDestItem.GetQuantity() + byQuantity );

							ItemNativeInfo_Dest->bySlotIndex = i;

							if( SetSlotItemNative(pLayer, &cDestItem, ItemNativeInfo_Dest) )
							{
								dwUserMoney	= pUser->GetMoney();
								pUser->SetMoney( dwUserMoney - (dwPrice * byQuantity) );

								ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_CREATE_BY_SHOP
									, pLayer->GetParent()->GetID(), (char*)pUser->GetName());	
								return TRUE;
							}
							else
							{
								*byErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
								return FALSE;
							}
						}
						else if(byCnt != 0)
						{
							CItem cDestItem;
							memcpy(&cDestItem, &pItemDest[i], sizeof(CItem));
							cDestItem.SetQuantity( cDestItem.GetQuantity() + byCnt );

							ItemNativeInfo_Dest->bySlotIndex = i;

							SetSlotItemNative(pLayer, &cDestItem, ItemNativeInfo_Dest);

							byQuantity -= byCnt;

							dwUserMoney	= pUser->GetMoney();
							pUser->SetMoney( dwUserMoney - (dwPrice * byCnt) );
						}
					}
				}
			}
		}

		if( byTempQuantity == byQuantity )
		{
			*byErrorCode = QUITE_FULL_SLOT;
			return FALSE;
		}		
		else if( byQuantity > 0 )
		{
			BYTE byRealCnt = byTempQuantity - byQuantity;
			pItemMoveResult->Item.SetQuantity(byRealCnt);
			pItemMoveResult->bQuantity = byRealCnt;

			ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_CREATE_BY_SHOP, pLayer->GetParent()->GetID()
				, (char*)pUser->GetName());
		}
		else if( byQuantity == 0 )
		{
			// 이동하려는 수량이 다 이동했다.
			pItemMoveResult->bQuantity = byTempQuantity;
		}
	}
	else if(ItemNativeInfo_Src->eSlotID ==  ITEM_NATIVE_INV_SMALL &&
			ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_TRADERMONSTER_INV_SMALL)
	{
		DWORD dwSellPrice	= pItemSrc->GetBaseItem()->GetSellPrice() * byQuantity;
		DWORD dwUserMoney	= pUser->GetMoney();

		if(dwUserMoney + dwSellPrice > __MAX_MONEY__)
		{
			*byErrorCode = BE_FULL_OF_MONEY;
			return FALSE;
		}

		CItem cItemSrc;
		memcpy(&cItemSrc, pItemSrc, sizeof(CItem));
		BYTE byCnt = cItemSrc.GetQuantity();

		if(byCnt == byQuantity)
		{
			RemoveItemNative(ItemNativeInfo_Src);
		}
		else if( byCnt > byQuantity )
		{
			cItemSrc.SetQuantity(byCnt - byQuantity);
			SetSlotItemNative(pLayer, &cItemSrc, ItemNativeInfo_Src);
		}

		pUser->SetMoney(dwUserMoney + dwSellPrice);	
		
		cItemSrc.SetQuantity(byQuantity);
		ItemMoveLogProcess(&cItemSrc, ITEM_LOG_DELETE_BY_SHOP, pLayer->GetParent()->GetID()
			, (char*)pUser->GetName());
	}

	return TRUE;
}


BOOL CItemMoveManager::TypeLargeMoveItemNative(CUser* const pUser
											   , CDungeonLayer* const pLayer
											   , CTDS_ITEM_MOVE* const pItemMove
											   , DSTC_ITEM_MOVE* const pItemMoveResult
											   , const CItem* pItemSrc
											   , const CItem* pItemDest
											   , ITEM_NATIVE_INFO* ItemNativeInfo_Src
											   , ITEM_NATIVE_INFO* ItemNativeInfo_Dest
											   , BYTE byQuantity
											   , BYTE* byErrorCode)
{
	BOOL bIsEmpty			= FALSE;
	BOOL bSuccess			= FALSE;
	BYTE byFindSlotIndex	= 0xff;
	
	if( IsSameNative(ItemNativeInfo_Src->eSlotID,ItemNativeInfo_Dest->eSlotID) )
	{
		if(	ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_EQUIP &&	ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_EQUIP )
		{
			bSuccess = PutOnItemNative(pLayer, ItemNativeInfo_Src, ItemNativeInfo_Dest);
			return bSuccess;
		}

		if( pUser )
		{
			if( pUser->GetItemNativeType((ITEM_NATIVE_TYPE)pItemMove->bSrc) == ITEM_NATIVE_TYPE_REAL )
			{
				if( ExchangeItemNative(pLayer, ItemNativeInfo_Src, ItemNativeInfo_Dest) )
				{
					return TRUE;
				}		
				else
				{
					*byErrorCode = EXCHANGE_ITEMNATIVE_FAIL;
					return FALSE;
				}		
			}
		}	
	}
	else if(ItemNativeInfo_Src->eSlotID	== ITEM_NATIVE_INV_LARGE &&	ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_EQUIP )
	{
		bSuccess = PutOnItemNative(pLayer, ItemNativeInfo_Src, ItemNativeInfo_Dest);	

		if(bSuccess)
		{
			return TRUE;
		}
		else
		{
			*byErrorCode = PUTON_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}	
	else if(IsItemNativeTraderMonster(ItemNativeInfo_Src->eSlotID) || 
			IsItemNativeTraderMonster(ItemNativeInfo_Dest->eSlotID))
	{
		BOOL bResult = TraderMonsterInvLargeAndInvLarge(pUser, pLayer, pItemMove, pItemMoveResult
			, pItemSrc, pItemDest, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity, byErrorCode);
		return bResult;
	}
	else if(ItemNativeInfo_Src->eSlotID	== ITEM_NATIVE_INV_LARGE &&
			ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_DUNGEONMANAGER )
	{
		if( pItemDest->GetID() == 0 )
		{
			CItem cItem;
			memcpy(&cItem, pItemDest, sizeof(CItem));
			pUser->GetCurDungeon()->GetDungeonDataEx()->SetGuardianItem(&cItem);
			return TRUE;
		}
		else
		{
			*byErrorCode = THIS_SLOT_ALREADY_ITEM;
			return FALSE;
		}
	}

	// <<<<<<< default Type Large Move ItemNative >>>>>>> //
	
	// 들어갈 자리에 아이템이 없으면 
	if(pItemDest->GetID() == 0)
	{
		if( MoveItemNative(pLayer, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity, pItemMoveResult) )
		{
			if(	ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_DUNGEONMANAGER &&
				ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_INV_LARGE)
			{
				CItem cItem;
				memset(&cItem, NULL, sizeof(CItem));
				pUser->GetCurDungeon()->GetDungeonDataEx()->SetGuardianItem(&cItem);
			}

			return TRUE;
		}
		else
		{
			*byErrorCode = MOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else
	{
		// 들어갈 자리에 아이템이 있으면 
		pItemDest = pItemDest - pItemMove->bDestIndex;

		for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
		{
			if(pItemDest[i].GetID() == 0)
			{
				bIsEmpty = TRUE;
				byFindSlotIndex = i;
				break;
			}
		}

		// Find Empty Slot
		if(bIsEmpty)
		{
			ItemNativeInfo_Dest->bySlotIndex = byFindSlotIndex;
						
			if( MoveItemNative(pLayer,ItemNativeInfo_Src,ItemNativeInfo_Dest,byQuantity,pItemMoveResult) )
			{
				pItemMoveResult->bDestIndex = byFindSlotIndex;

				if(	ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_DUNGEONMANAGER &&
					ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_INV_LARGE)
				{
					CItem cItem;
					memset(&cItem, NULL, sizeof(CItem));
					pUser->GetCurDungeon()->GetDungeonDataEx()->SetGuardianItem(&cItem);
				}

				return TRUE;
			}
			else
			{
				*byErrorCode = MOVE_ITEMNATIVE_FAIL;
				return FALSE;
			}
		}
		else
		{
			*byErrorCode = QUITE_FULL_SLOT;
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CItemMoveManager::TypeSmallMoveItemNative(CUser* const pUser
											   , CDungeonLayer* const pLayer
											   , CTDS_ITEM_MOVE* const pItemMove
											   , DSTC_ITEM_MOVE* const pItemMoveResult
											   , const CItem* pItemSrc
											   , const CItem* pItemDest
											   , ITEM_NATIVE_INFO* ItemNativeInfo_Src
											   , ITEM_NATIVE_INFO* ItemNativeInfo_Dest
											   , BYTE byQuantity
											   , BYTE* byErrorCode)
{
	BYTE		byTempQuantity			= byQuantity;
	CBaseItem*	pBaseItemSrc			= NULL;
	CBaseItem*	pBaseItemDest			= NULL;
	
	CItem cItem;
	memcpy(&cItem,pItemSrc,sizeof(CItem));
	pBaseItemSrc = cItem.GetBaseItem();

	DWORD wKind = GetItemKind(cItem.GetID());

	if(cItem.GetID() == __ITEM_INSURANCE_INDEX__)
	{
		// 아이템 보험 적용 되어있어?
		if( cItem.m_Item_Zodiac.bInvIndex >= 1 )
		{
			*byErrorCode = THIS_ITEM_IS_OCCUPIABLE;
			return FALSE;
		}
	}

	if(ITEM_LAND_TYPE_PLAYTIME == pBaseItemSrc->GetLandItemType())
	{
		switch(wKind)
		{
			case ITEM_KIND_CONSUMABLE:
			{
				if( cItem.m_Item_Consumable.bInvIndex >= 1 )
				{
					*byErrorCode = THIS_ITEM_IS_OCCUPIABLE;
					return FALSE;
				}		
			}
			break;
		}		
	}

	// 같은 창에서의 이동인지 본다.
	if( IsSameNative(ItemNativeInfo_Src->eSlotID,ItemNativeInfo_Dest->eSlotID) )
	{
		if( pUser )
		{
			if( pUser->GetItemNativeType((ITEM_NATIVE_TYPE)pItemMove->bSrc) == ITEM_NATIVE_TYPE_REAL )
			{
				if( ExchangeItemNative(pLayer, ItemNativeInfo_Src, ItemNativeInfo_Dest) )
				{
					return TRUE;
				}		
				else
				{
					*byErrorCode = EXCHANGE_ITEMNATIVE_FAIL;
					return FALSE;
				}					
			}		
		}		
	}
	else if(IsItemNativeTraderMonster(ItemNativeInfo_Src->eSlotID) ||
			IsItemNativeTraderMonster(ItemNativeInfo_Dest->eSlotID)		)
	{
		BOOL bResult = TraderMonsterInvSmallAndInvSmall(pUser, pLayer, pItemMove, pItemMoveResult
			, pItemSrc, pItemDest, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity, byErrorCode);
		return bResult;
	}
	else if(ItemNativeInfo_Src->eSlotID	== ITEM_NATIVE_INV_SMALL &&
			ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_DUNGEONMANAGER )
	{
		if( pItemDest->GetID() == 0 )
		{
			CItem cItem;
			memcpy(&cItem, pItemDest, sizeof(CItem));
			pUser->GetCurDungeon()->GetDungeonDataEx()->SetMagicFieldArray(&cItem);
			return TRUE;
		}
		else
		{
			*byErrorCode = THIS_SLOT_ALREADY_ITEM;
			return FALSE;
		}
	}
	
	// <<<<<<< default Type Small Move ItemNative >>>>>>> //

	// 들어갈 자리에 아이템이 없으면
	if(pItemDest->GetID() == 0)
	{
		if( MoveItemNative(pLayer, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity, pItemMoveResult) )
		{
			if(	ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_DUNGEONMANAGER &&
				ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_INV_SMALL)
			{
				CItem cItem;
				memset(&cItem, NULL, sizeof(CItem));
				pUser->GetCurDungeon()->GetDungeonDataEx()->SetMagicFieldArray(&cItem);
			}

			return TRUE;
		}
		else
		{
			*byErrorCode = MOVE_ITEMNATIVE_FAIL;
			return FALSE;
		}
	}
	else
	{	
		// 들어갈 자리에 아이템이 있으면
		pBaseItemDest = pItemDest->GetBaseItem();

		if(NULL == pBaseItemDest)
		{
			*byErrorCode = GET_CANNOT_BASEITEMPOINTER;
			return FALSE;	
		}

		// 들어갈 자리에 아이템이 있고 소스아이템과 같은 아이디면 수량이 겹칠수 있는지 알아본다. 
		if(pItemSrc->GetID() == pItemDest->GetID())
		{
			BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
			BYTE byCnt			= byMaxQuantity - pItemDest->GetQuantity();

			if(byCnt >= byQuantity)
			{
				if( OverlapItemNative(pLayer,ItemNativeInfo_Src,ItemNativeInfo_Dest,byQuantity,pItemMoveResult) )
				{
					return TRUE;
				}
				else
				{
					*byErrorCode = MOVE_ITEMNATIVE_FAIL;
					return FALSE;
				}										
			}
			else if(byCnt != 0)
			{
				OverlapItemNative(pLayer,ItemNativeInfo_Src,ItemNativeInfo_Dest,byCnt,pItemMoveResult);
				byQuantity -= byCnt;				
			}
		}

		// 여기까지 왔으면 이동해야할 cnt가 남은거다. 맨처음 슬롯부터 찾는다.
		pItemDest = pItemDest - pItemMove->bDestIndex;

		for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
		{
			if(pItemDest[i].GetID() == 0)
			{
				ItemNativeInfo_Dest->bySlotIndex = i;
				if( MoveItemNative(pLayer,ItemNativeInfo_Src,ItemNativeInfo_Dest,byQuantity,pItemMoveResult) )
				{
					if(	ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_DUNGEONMANAGER &&
						ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_INV_SMALL)
					{
						CItem cItem;
						memset(&cItem, NULL, sizeof(CItem));
						pUser->GetCurDungeon()->GetDungeonDataEx()->SetMagicFieldArray(&cItem);
					}

					return TRUE;
				}
				else
				{
					*byErrorCode = MOVE_ITEMNATIVE_FAIL;
					return FALSE;
				}
			}
			else
			{
				pBaseItemDest = g_pBaseItemHash->GetData(pItemDest[i].GetID());

				if(NULL == pBaseItemDest)
				{
					continue;
				}
				
				if(pItemSrc->GetID() == pItemDest[i].GetID())
				{
					BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
					BYTE byCnt			= byMaxQuantity - pItemDest[i].GetQuantity();

					if(byCnt >= byQuantity)
					{
						ItemNativeInfo_Dest->bySlotIndex = i;
						if( OverlapItemNative(pLayer, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity, pItemMoveResult) )
						{
							return TRUE;
						}
						else
						{
							*byErrorCode = MOVE_ITEMNATIVE_FAIL;
							return FALSE;
						}						
					}
					else if(byCnt != 0)
					{
						ItemNativeInfo_Dest->bySlotIndex = i;
						OverlapItemNative(pLayer, ItemNativeInfo_Src, ItemNativeInfo_Dest, byCnt, pItemMoveResult);
						byQuantity -= byCnt;								
					}
				}
			}
		}

		if( byTempQuantity == byQuantity )
		{
			*byErrorCode = QUITE_FULL_SLOT;
			return FALSE;
		}
		else if( byQuantity > 0 )
		{
			BYTE byRealCnt = byTempQuantity - byQuantity;
			pItemMoveResult->Item.SetQuantity(byRealCnt);
			pItemMoveResult->bQuantity = byRealCnt;
		}
		else if( byQuantity == 0 )
		{
			// 이동하려는 수량이 다 이동했다.
			pItemMoveResult->bQuantity = byTempQuantity;
		}
	}

	return TRUE;
}


//=========================================================================================
// 아이템과 카르츠 드랍,픽업
//=========================================================================================
void CItemMoveManager::DropAndPickupItemProcess(DWORD dwConnectionIndex
												, CUser* const pUser
												, CDungeonLayer* const pLayer
												, CTDS_ITEM_MOVE* const pItemMove
												, DSTC_ITEM_MOVE* const pItemMoveResult)
{
	BOOL				bSuccess			= FALSE;	
	BYTE				byErrorCode			= 0xff;
	BYTE				byQuantity			= NULL;
	CBaseItem*			pBaseItemSrc		= NULL;
	const CItem*		pItemSrc			= NULL;
	const CItem*		pItemDest			= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;

	// 링크 걸어놨잖아. 이동하지마 
	if( pUser->GetItemNativeLinkCount() )
	{
		byErrorCode = WORKING_LINK;

		ITEM_NATIVE eNative		= pUser->GetCurItemNativeLink();
		pItemMoveResult->bSrc	= (BYTE)eNative;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( !IsSrcToDestCorrect((ITEM_NATIVE)pItemMove->bSrc, (ITEM_NATIVE)pItemMove->bDest) )
	{
		byErrorCode = MAKE_A_MISTAKE_WAY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( (ITEM_NATIVE)pItemMove->bSrc == ITEM_NATIVE_INV_KARZ &&	(ITEM_NATIVE)pItemMove->bDest == ITEM_NATIVE_TILE )
	{
		ItemNativeInfo_Src.eSlotID		= (ITEM_NATIVE)pItemMove->bSrc;
		ItemNativeInfo_Src.dwOwnerID	= pUser->GetID();
		ItemNativeInfo_Src.bySlotIndex	= pItemMove->bSrcIndex;
		ItemNativeInfo_Src.dwMoney		= pItemMove->dwMoney;

		bSuccess = DropItem(pLayer, &ItemNativeInfo_Src, byQuantity, pItemMoveResult);

		if(!bSuccess)
		{
			byErrorCode = DROP_ITEM_FAIL;
		}
		else
		{
			KarzLogProcess(ITEM_LOG_KARZ_DROP, pLayer->GetParent()->GetID(), (char*)pUser->GetName(), NULL
				, pItemMove->dwMoney);
		}

		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	pItemSrc	= GetItemNativePointer(pUser, pLayer, pItemMove, &ItemNativeInfo_Src, GET_SRC_NATIVEPOINTER);
	pItemDest	= GetItemNativePointer(pUser, pLayer, pItemMove, &ItemNativeInfo_Dest, GET_DEST_NATIVEPOINTER);

	if(NULL == pItemSrc)
	{
		byErrorCode = GET_CANNOT_NATIVEPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}	

	ITEM_TYPE eType = GetItemType( pItemSrc->GetID() );

	// 돈아템은 베이스가 엄써..
	if(eType != ITEM_TYPE_MONEY)
	{
		pBaseItemSrc = g_pBaseItemHash->GetData(pItemSrc->GetID());

		if(NULL == pBaseItemSrc)
		{
			byErrorCode = GET_CANNOT_BASEITEMPOINTER;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}

		if( pItemSrc->GetQuantity() < pItemMove->bQuantity || pBaseItemSrc->GetMaxQuantity() < pItemMove->bQuantity )
		{
			byErrorCode = MAKE_A_MISTAKE_ITEMQUANTITY;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}
	}	

	byQuantity	= pItemSrc->GetQuantity();	
	byQuantity	= pItemMove->bQuantity;

	CItem cItemTemp;
	memcpy(&cItemTemp, pItemSrc, sizeof(CItem));
	
	if( IsItemNativeInvLargeAndSmall((ITEM_NATIVE)pItemMove->bSrc) )
	{
		// Drop Item
		bSuccess = DropItem(pLayer, &ItemNativeInfo_Src, byQuantity, pItemMoveResult);

		if(!bSuccess)
		{
			byErrorCode = DROP_ITEM_FAIL;
		}
		else
		{
			cItemTemp.SetQuantity(byQuantity);
			ItemMoveLogProcess(&cItemTemp, ITEM_LOG_DROP, pLayer->GetParent()->GetID(), (char*)pUser->GetName());
		}
	}
	else if( IsItemNativeTile((ITEM_NATIVE)pItemMove->bSrc) )
	{
		// Pickup Item
		bSuccess = PickupItem(pLayer, &ItemNativeInfo_Src, &ItemNativeInfo_Dest, pItemMoveResult);

		if(!bSuccess)
		{
			byErrorCode = PICKUP_ITEM_FAIL;
		}
		else
		{
			ITEM_TYPE eType = GetItemType(cItemTemp.GetID());

			if(ITEM_TYPE_MONEY == eType)
			{
				if( pUser->IsCurDungeonVillage() )
				{
					KarzLogProcess(ITEM_LOG_KARZ_PICKUP, pLayer->GetParent()->GetID(), (char*)pUser->GetName(), NULL
						, cItemTemp.m_Item_Money.dwMoney, NULL);
				}
				else
				{
					if(cItemTemp.m_Item_Money.dwMoney >= 10000)
					{
						KarzLogProcess(ITEM_LOG_KARZ_PICKUP, pLayer->GetParent()->GetID(), (char*)pUser->GetName(), NULL
							, cItemTemp.m_Item_Money.dwMoney, NULL);
					}
				}
			}
			else
			{
				cItemTemp.SetQuantity(byQuantity);
				ItemMoveLogProcess(&cItemTemp, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), (char*)pUser->GetName());
			}			
		}
	}

	SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
}


//=========================================================================================
// 인벤과 창고의 카르츠 이동 
//=========================================================================================
void CItemMoveManager::KarzMoveProcess(DWORD dwConnectionIndex
										, CUser* const pUser 
										, CDungeonLayer* const pLayer
										, CTDS_ITEM_MOVE* const pItemMove
										, DSTC_ITEM_MOVE* const pItemMoveResult)
{
	BYTE byErrorCode = 0xff;

	// 링크 걸어놨잖아. 이동하지마 
	if( pUser->GetItemNativeLinkCount() )
	{
		byErrorCode = WORKING_LINK;

		ITEM_NATIVE eNative		= pUser->GetCurItemNativeLink();
		pItemMoveResult->bSrc	= (BYTE)eNative;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);		
		return;
	}

	if(	(ITEM_NATIVE)pItemMove->bSrc == ITEM_NATIVE_INV_KARZ &&	(ITEM_NATIVE)pItemMove->bDest == ITEM_NATIVE_BANK_KARZ )
	{
		// 인벤에서 창고로 카르츠 이동 
		if( pItemMove->dwMoney <= pUser->GetMoney() )
		{
			__int64 nValueBankMon	= pUser->GetBankMoney();
			__int64 nValueMon		= pItemMove->dwMoney;

			if( nValueBankMon + nValueMon > __MAX_MONEY__ )
			{
				DWORD dwMoney = __MAX_MONEY__ - pUser->GetBankMoney();

				pUser->SetMoney( pUser->GetMoney() - dwMoney );
				pUser->SetBankMoney( pUser->GetBankMoney() + dwMoney );				
			}
			else
			{
				pUser->SetMoney( pUser->GetMoney() - pItemMove->dwMoney );
				pUser->SetBankMoney( pUser->GetBankMoney() + pItemMove->dwMoney );				
			}
			
			pItemMoveResult->dwMoney	= pUser->GetMoney();
			pItemMoveResult->i64ItemID	= pUser->GetBankMoney();

			KarzLogProcess(ITEM_LOG_KARZ_RECEIVE, pUser->GetCurDungeonID(), (char*)pUser->GetName(), NULL
				, pItemMove->dwMoney );
		}
		else
		{
			byErrorCode = MONEY_MOVE_FAIL;
		}
	}
	else if((ITEM_NATIVE)pItemMove->bSrc == ITEM_NATIVE_BANK_KARZ && (ITEM_NATIVE)pItemMove->bDest == ITEM_NATIVE_INV_KARZ)
	{
		// 창고에서 인벤으로 카르츠 이동		
		if( pItemMove->dwMoney <= pUser->GetBankMoney() )
		{
			__int64 nValueMon		= pUser->GetMoney();
			__int64 nValuePackMon	= pItemMove->dwMoney;

			if( nValueMon + nValuePackMon > __MAX_MONEY__ )
			{
				DWORD dwMoney = __MAX_MONEY__ - pUser->GetMoney();

				pUser->SetMoney( pUser->GetMoney() + dwMoney );
				pUser->SetBankMoney( pUser->GetBankMoney() - dwMoney );				
			}
			else
			{
				pUser->SetMoney( pUser->GetMoney() + pItemMove->dwMoney );
				pUser->SetBankMoney( pUser->GetBankMoney() - pItemMove->dwMoney );				
			}
			
			pItemMoveResult->dwMoney	= pUser->GetMoney();
			pItemMoveResult->i64ItemID	= pUser->GetBankMoney();

			KarzLogProcess(ITEM_LOG_KARZ_DRAWOUT, pUser->GetCurDungeonID(), (char*)pUser->GetName(), NULL
				, pItemMove->dwMoney );
		}
		else
		{
			byErrorCode = MONEY_MOVE_FAIL;
		}
	}

	SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
}


//=========================================================================================
// 기본적인 슬롯 이동 
//=========================================================================================
void CItemMoveManager::DefaultSlotChangeProcess(DWORD dwConnectionIndex
												, CUser* const pUser 
												, CDungeonLayer* const pLayer
												, CTDS_ITEM_MOVE* const pItemMove
												, DSTC_ITEM_MOVE* const pItemMoveResult)
{
	BOOL				bSuccess			= FALSE;	
	BYTE				byErrorCode			= 0xff;
	BYTE				byQuantity			= NULL;
	CBaseItem*			pBaseItemSrc		= NULL;
	const CItem*		pItemSrc			= NULL;
	const CItem*		pItemDest			= NULL;
	ITEM_TYPE			eType;
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;

	// 링크 걸어놨잖아. 이동하지마 
	if( pUser->GetItemNativeLinkCount() )
	{
		byErrorCode = WORKING_LINK;

		ITEM_NATIVE eNative		= pUser->GetCurItemNativeLink();
		pItemMoveResult->bSrc	= (BYTE)eNative;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( !IsSrcToDestCorrect((ITEM_NATIVE)pItemMove->bSrc, (ITEM_NATIVE)pItemMove->bDest) )
	{
		byErrorCode = MAKE_A_MISTAKE_WAY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( !IsNativeToCorrectSlotIndex((ITEM_NATIVE)pItemMove->bSrc,pItemMove->bSrcIndex) ||
		!IsNativeToCorrectSlotIndex((ITEM_NATIVE)pItemMove->bDest,pItemMove->bDestIndex)	)
	{
		byErrorCode = MAKE_A_MISTAKE_SLOTINDEX;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}
	
	pItemSrc	= GetItemNativePointer(pUser, pLayer, pItemMove, &ItemNativeInfo_Src, GET_SRC_NATIVEPOINTER);
	pItemDest	= GetItemNativePointer(pUser, pLayer, pItemMove, &ItemNativeInfo_Dest, GET_DEST_NATIVEPOINTER);

	if(NULL == pItemSrc)
	{
		byErrorCode = GET_CANNOT_NATIVEPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}
	else if(NULL == pItemDest)
	{
		if(	pItemMove->bDest != ITEM_NATIVE_TRADERMONSTER_INV_LARGE &&
			pItemMove->bDest != ITEM_NATIVE_TRADERMONSTER_INV_SMALL )
		{
			byErrorCode = GET_CANNOT_NATIVEPOINTER;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}		
	}
	
	pBaseItemSrc = g_pBaseItemHash->GetData(pItemSrc->GetID());

	if(NULL == pBaseItemSrc)
	{
		byErrorCode = GET_CANNOT_BASEITEMPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( !IsDestNativeToCorrectType(pUser, ItemNativeInfo_Dest.eSlotID, ItemNativeInfo_Dest.bySlotIndex, pItemSrc) )
	{
		byErrorCode = MAKE_A_MISTAKE_ITEM_KIND;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( pItemSrc->GetQuantity() < pItemMove->bQuantity || pBaseItemSrc->GetMaxQuantity() < pItemMove->bQuantity )
	{
		byErrorCode = MAKE_A_MISTAKE_ITEMQUANTITY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	eType		= GetItemType(pItemSrc->GetID());
	byQuantity	= pItemSrc->GetQuantity();	
	byQuantity	= pItemMove->bQuantity; 

	// 소스 아이템을 살짝 복사해 놓는다.
	CItem cItemSrcTemp;
	memcpy(&cItemSrcTemp, pItemSrc, sizeof(CItem));
	
	if(ITEM_TYPE_LARGE == eType)
	{
		bSuccess = TypeLargeMoveItemNative(pUser, pLayer, pItemMove, pItemMoveResult
			, pItemSrc, pItemDest, &ItemNativeInfo_Src, &ItemNativeInfo_Dest, byQuantity, &byErrorCode);
		
		if(bSuccess)
		{
			pItemMoveResult->bDestIndex = ItemNativeInfo_Dest.bySlotIndex;

			if(ItemNativeInfo_Src.eSlotID == ITEM_NATIVE_EQUIP || ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_EQUIP )
			{
				PutOnOffItemNativeResult(pUser, pLayer, &cItemSrcTemp);
			}
		}		
	}
	else if(ITEM_TYPE_SMALL == eType)
	{
		bSuccess = TypeSmallMoveItemNative(pUser, pLayer, pItemMove, pItemMoveResult
				, pItemSrc, pItemDest, &ItemNativeInfo_Src, &ItemNativeInfo_Dest, byQuantity, &byErrorCode );			
	}

	SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);	
}


//=========================================================================================
// 아이템 이동 로그를 남긴다.
//=========================================================================================
void CItemMoveManager::ItemMoveLogProcess(CItem* pItem, BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller)
{
	if(!g_bEnableItemLog)										return;
	if(!pItem)													return;
	if( pItem->GetID() == 0 )									return;
	if( pItem->GetID() == __ITEM_KARZ__ )						return;			
	if( g_pInvalidLogItemHash->bSameKey( pItem->GetID() ) )		return;

	ITEM_LOG Log;
	Log.bLogCode	= bLogCode;
	Log.bServerSet	= (BYTE)g_pThis->GetServerSetNum();
	Log.dwDungeonID	= wDungeonID;
	memcpy( &Log.Serial, &pItem->m_Serial, sizeof( ITEM_SERIAL ) );
	memcpy( &Log.Item, pItem, sizeof( CItem ) );
	__lstrcpyn( Log.szOwnerName, szOwner, MAX_CHARACTER_NAME_LENGTH );

	if( szSeller )
		__lstrcpyn( Log.szSellerName, szSeller, MAX_CHARACTER_NAME_LENGTH );
	
	char		szQuery[1024]={0,};
	int			nColumn			= 7;	
	int			col				= 0;
	DWORD		dwParamOffset	= 0;
	DBBINDING*	pBinding		= g_pLogDb->THCreateParamInfo(nColumn);	

	if(!pBinding)
	{
		::Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetItemLog Function()!");
		return;
	}

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Serial);
	pBinding[col++].wType	= DBTYPE_UI8;		
	dwParamOffset			+= sizeof(Log.Serial);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bLogCode);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(Log.bLogCode);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bServerSet);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(Log.bServerSet);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.dwDungeonID);
	pBinding[col++].wType	= DBTYPE_UI4;		
	dwParamOffset			+= sizeof(Log.dwDungeonID);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Item);
	pBinding[col++].wType	= DBTYPE_BYTES;		
	dwParamOffset			+= sizeof(Log.Item);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szOwnerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szOwnerName);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szSellerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szSellerName);

#ifdef DEV_MODE
	Log.bServerSet = (BYTE)g_pThis->GetDevModeServerSetNum();
#endif

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	wsprintf( szQuery, "Insert into ItemLog%d_%d%02d%02d (Serial, LogCode, ServerSet, DungeonID, Item, OwnerName, SellerName) values( ?, ?, ?, ?, ?, ?, ? )",
						g_pThis->GetServerSetNum(), CurTime.wYear, CurTime.wMonth, CurTime.wDay);

	g_pLogDb->THExecuteSQLByParam( szQuery, pBinding, &Log.Serial, sizeof(Log), nColumn, FALSE, 0, NULL, LOG_DB);		 
}


//=========================================================================================
// 카르츠 로그를 남긴다.
//=========================================================================================
void CItemMoveManager::KarzLogProcess(BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller,DWORD dwMoney
									  ,CItem* pItem)
{
	if(!g_bEnableItemLog)	return;
	if(dwMoney == NULL)		return;
	
	KARZ_LOG Log;	
	Log.bLogCode	= bLogCode;
	Log.bServerSet	= (BYTE)g_pThis->GetServerSetNum();
	Log.dwDungeonID	= wDungeonID;
	Log.dwMoney		= dwMoney;

	if(pItem != NULL)
	{
		memcpy( &Log.Serial, &pItem->m_Serial, sizeof( ITEM_SERIAL ) );
		memcpy( &Log.Item, pItem, sizeof( CItem ) );
	}
	
	__lstrcpyn( Log.szOwnerName, szOwner, MAX_CHARACTER_NAME_LENGTH );

	if( szSeller )
		__lstrcpyn( Log.szSellerName, szSeller, MAX_CHARACTER_NAME_LENGTH );

	char		szQuery[1024]	= {0,};
	int			nColumn			= 8;	
	int			col				= 0;
	DWORD		dwParamOffset	= 0;
	DBBINDING*	pBinding		= g_pLogDb->THCreateParamInfo(nColumn);	

	if(!pBinding)
	{
		::Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetItemLog Function()!");
		return;
	}

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Serial);
	pBinding[col++].wType	= DBTYPE_UI8;		
	dwParamOffset			+= sizeof(Log.Serial);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bLogCode);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(Log.bLogCode);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bServerSet);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(Log.bServerSet);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.dwDungeonID);
	pBinding[col++].wType	= DBTYPE_UI4;		
	dwParamOffset			+= sizeof(Log.dwDungeonID);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Item);
	pBinding[col++].wType	= DBTYPE_BYTES;		
	dwParamOffset			+= sizeof(Log.Item);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.dwMoney);
	pBinding[col++].wType	= DBTYPE_UI4;		
	dwParamOffset			+= sizeof(Log.dwMoney);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szOwnerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szOwnerName);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szSellerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szSellerName);
	
#ifdef DEV_MODE
	Log.bServerSet = (BYTE)g_pThis->GetDevModeServerSetNum();
#endif

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	wsprintf( szQuery, "Insert into KarzLog%d_%d%02d%02d (Serial, LogCode, ServerSet, DungeonID, Item, Money, OwnerName, SellerName) values( ?, ?, ?, ?, ?, ?, ?, ? )",
						g_pThis->GetServerSetNum(), CurTime.wYear, CurTime.wMonth, CurTime.wDay);

	g_pLogDb->THExecuteSQLByParam( szQuery, pBinding, &Log.Serial, sizeof(Log), nColumn, FALSE, 0, NULL, LOG_DB);	
}


//=========================================================================================
// 장착하거나 장착한걸 뱄을때 유저의 결과를 보내준다.
//=========================================================================================
void CItemMoveManager::PutOnOffItemNativeResult(CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemSrc)
{
	pUser->SendAllStatus();

	ITEM_NATIVE_INFO ItemNativeInfoMod;
	memset(&ItemNativeInfoMod,0,sizeof(ITEM_NATIVE_INFO));

	ItemNativeInfoMod.eSlotID		= ITEM_NATIVE_EQUIP;
	ItemNativeInfoMod.dwOwnerID		= pUser->GetID();
	ItemNativeInfoMod.bySlotIndex	= 0;

	BYTE byWeaponKind = pItemSrc->GetBaseItem()->GetWeaponKind();

	if( (byWeaponKind >= ITEM_CLASS_SWARD && byWeaponKind <= ITEM_CLASS_STAFF) || byWeaponKind == ITEM_CLASS_SHIELD )
	{
		const CItem* pItemRHand = NULL;
		const CItem* pItemLHand = NULL;

		if(pUser->m_bCurrentHand == 1)
		{
			ItemNativeInfoMod.bySlotIndex = EQUIP_TYPE_RHAND1;
			pItemRHand = GetItemNative(pLayer, &ItemNativeInfoMod);
			ItemNativeInfoMod.bySlotIndex = EQUIP_TYPE_LHAND1;
			pItemLHand = GetItemNative(pLayer, &ItemNativeInfoMod);

			ItemHandMod(pUser, pItemRHand->GetID(), pItemLHand->GetID());
		}
		else
		{
			ItemNativeInfoMod.bySlotIndex = EQUIP_TYPE_RHAND2;
			pItemRHand = GetItemNative(pLayer, &ItemNativeInfoMod);
			ItemNativeInfoMod.bySlotIndex = EQUIP_TYPE_LHAND2;
			pItemLHand = GetItemNative(pLayer, &ItemNativeInfoMod);

			ItemHandMod(pUser, pItemRHand->GetID(), pItemLHand->GetID());
		}
	}
	else if( byWeaponKind == ITEM_CLASS_ARMOR )
	{
		const CItem* pItemArmor = NULL; 
		ItemNativeInfoMod.bySlotIndex = EQUIP_TYPE_MAIL;
		pItemArmor = GetItemNative(pLayer, &ItemNativeInfoMod);
		ItemArmorMod(pUser, pItemArmor->GetID());
	}
	else if( byWeaponKind == ITEM_CLASS_HELMET )
	{
		const CItem* pItemHelmet = NULL;
		ItemNativeInfoMod.bySlotIndex = EQUIP_TYPE_HELMET;
		pItemHelmet = GetItemNative(pLayer, &ItemNativeInfoMod);
		ItemHelmetMod(pUser, pItemHelmet->GetID());
	}
	else
	{

	}
}


//=========================================================================================
// 인벤라쥐,스몰 => 링크 
//=========================================================================================
void CItemMoveManager::LinkSlotChangeInvToLinkProcess(DWORD dwConnectionIndex
													  , CUser* const pUser
													  , CDungeonLayer* const pLayer
													  , CTDS_ITEM_MOVE* const pItemMove
													  , DSTC_ITEM_MOVE* const pItemMoveResult)
{
	BYTE				byErrorCode			= 0xff;
	BOOL				bSuccess			= FALSE;
	CBaseItem*			pBaseItemSrc		= NULL;
	const CItem*		pItemSrc			= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
			
	memset(&ItemNativeInfo_Src, 0, sizeof(ITEM_NATIVE_INFO));
	memset(&ItemNativeInfo_Dest, 0, sizeof(ITEM_NATIVE_INFO));	

	if( !IsSrcToDestCorrect((ITEM_NATIVE)pItemMove->bSrc, (ITEM_NATIVE)pItemMove->bDest) )
	{
		byErrorCode = MAKE_A_MISTAKE_WAY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	// Inv_Large, Inv_Small	
	ItemNativeInfo_Src.eSlotID				= (ITEM_NATIVE)pItemMove->bSrc;
	ItemNativeInfo_Src.dwOwnerID			= pUser->GetID();
	ItemNativeInfo_Src.bySlotIndex			= pItemMove->bSrcIndex;
	ItemNativeInfo_Src.byItemQuantityCnt	= pItemMove->bQuantity;

	// Trade, Upgrade, Making, PlayerShop
	ItemNativeInfo_Dest.eSlotID				= (ITEM_NATIVE)pItemMove->bDest;
	ItemNativeInfo_Dest.dwOwnerID			= pUser->GetID();
	ItemNativeInfo_Dest.bySlotIndex			= pItemMove->bDestIndex;
	ItemNativeInfo_Dest.byItemQuantityCnt	= pItemMove->bQuantity;

	if( !pUser->IsCorrectItemNativeLink(ItemNativeInfo_Dest.eSlotID) )
	{
		byErrorCode = ALREADY_DIFFERENT_WINDOW_LINK;

		ITEM_NATIVE eNative		= pUser->GetCurItemNativeLink();
		pItemMoveResult->bSrc	= (BYTE)eNative;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	pItemSrc = GetItemNativePointer(pUser, pLayer, pItemMove, &ItemNativeInfo_Src, GET_SRC_NATIVEPOINTER);

	if(NULL == pItemSrc)
	{
		byErrorCode = GET_CANNOT_NATIVEPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	pBaseItemSrc = g_pBaseItemHash->GetData(pItemSrc->GetID());

	if(NULL == pBaseItemSrc)
	{
		byErrorCode = GET_CANNOT_BASEITEMPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( !IsDestNativeToCorrectType(pUser, ItemNativeInfo_Dest.eSlotID, ItemNativeInfo_Dest.bySlotIndex, pItemSrc) )
	{
		byErrorCode = MAKE_A_MISTAKE_ITEM_KIND;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( pItemSrc->GetQuantity() < pItemMove->bQuantity || pBaseItemSrc->GetMaxQuantity() < pItemMove->bQuantity )
	{
		byErrorCode = MAKE_A_MISTAKE_ITEMQUANTITY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	// 인벤의 아템이 링크 걸릴수 있는지 조사한당.. 
	if( pUser->IsUserVirtualItemNativeLink(pItemSrc, pItemMove->bQuantity) == FALSE )
	{
		byErrorCode = INV_TO_LINK_FAIL;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}
	
	if( IsItemNativeLink(&ItemNativeInfo_Dest) )
	{
		// 데스트에 이미 링크 걸려 있어
		if( ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_PLAYERSHOP )
		{
			if( pItemMove->dwMoney > MAX_MONEY_PLAYERSHOP || pItemMove->dwMoney == 0 )
			{
				byErrorCode = MAKE_A_MISTAKE_MONEY;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}

		// 비어있는 링크슬롯을 찾는다.
		BOOL bIsEmpty			= FALSE;
		BYTE byFindSlotIndex	= 0xff; 
		ITEM_NATIVE_INFO ItemNativeInfo;
		const ITEM_NATIVE_INFO* pItemNativeInfo = NULL;
		memcpy(&ItemNativeInfo,&ItemNativeInfo_Dest,sizeof(ITEM_NATIVE_INFO));
		ItemNativeInfo.bySlotIndex = 0;
		
		pItemNativeInfo = GetItemNativeLink(&ItemNativeInfo);

		for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
		{
			const CItem* pItemNative = GetItemNative(pLayer,&pItemNativeInfo[i]);
			if(NULL == pItemNative)
			{
				bIsEmpty = TRUE;
				byFindSlotIndex = i;
				break;
			}
		}

		// 빈 링크 슬롯 찾았떠.. 
		if(bIsEmpty)
		{
			ItemNativeInfo_Dest.bySlotIndex = byFindSlotIndex;
			pItemMoveResult->bDestIndex		= ItemNativeInfo_Dest.bySlotIndex;

			bSuccess = SetSlotItemNativeLink(ItemNativeInfo_Dest.eSlotID, ItemNativeInfo_Dest.bySlotIndex
				, &ItemNativeInfo_Src);

			if(bSuccess)
			{
				if( ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_PLAYERSHOP )
				{
					pUser->SetPlayerShopCustomSellPrice(pItemMove->dwMoney,byFindSlotIndex);
				}

				if( pUser->SetUserVirtualItemNativeLink(pItemSrc,pItemMove->bQuantity) )
				{
					SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
					return;
				}
				else
				{
					byErrorCode = INV_TO_LINK_FAIL;
					SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
					return;
				}					
			}
			else
			{
				byErrorCode = INV_TO_LINK_FAIL;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
		else
		{
			byErrorCode = QUITE_FULL_SLOT;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}
	}	
	else
	{
		// 바로 링크 걸어.. 
		if( ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_PLAYERSHOP )
		{
			if( pItemMove->dwMoney > MAX_MONEY_PLAYERSHOP || pItemMove->dwMoney == 0)
			{
				byErrorCode = MAKE_A_MISTAKE_MONEY;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}

		bSuccess = SetSlotItemNativeLink(ItemNativeInfo_Dest.eSlotID, ItemNativeInfo_Dest.bySlotIndex
			, &ItemNativeInfo_Src);

		if(bSuccess)
		{
			if( ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_PLAYERSHOP )
			{
				pUser->SetPlayerShopCustomSellPrice(pItemMove->dwMoney,ItemNativeInfo_Dest.bySlotIndex);
			}

			if( pUser->SetUserVirtualItemNativeLink(pItemSrc,pItemMove->bQuantity) )
			{
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
			else
			{
				byErrorCode = INV_TO_LINK_FAIL;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
		else
		{
			byErrorCode = INV_TO_LINK_FAIL;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}		
	}	
}


//=========================================================================================
// 링크 <=> 링크 
//=========================================================================================
void CItemMoveManager::LinkSlotChangeLinkToLinkProcess(DWORD dwConnectionIndex
													   ,CUser* const pUser
													   ,CDungeonLayer* const pLayer
													   ,CTDS_ITEM_MOVE* const pItemMove
													   ,DSTC_ITEM_MOVE* const pItemMoveResult)
{
	BYTE				byErrorCode			= 0xff;	
	BOOL				bSuccess			= FALSE;
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
	memset(&ItemNativeInfo_Src, 0, sizeof(ITEM_NATIVE_INFO));
	memset(&ItemNativeInfo_Dest, 0, sizeof(ITEM_NATIVE_INFO));

	if( !IsSrcToDestCorrect((ITEM_NATIVE)pItemMove->bSrc, (ITEM_NATIVE)pItemMove->bDest) )
	{
		byErrorCode = MAKE_A_MISTAKE_WAY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	// Trade, Upgrade, Making, PlayerShop
	ItemNativeInfo_Src.eSlotID				= (ITEM_NATIVE)pItemMove->bSrc;
	ItemNativeInfo_Src.dwOwnerID			= pUser->GetID();
	ItemNativeInfo_Src.bySlotIndex			= pItemMove->bSrcIndex;
	ItemNativeInfo_Src.byItemQuantityCnt	= pItemMove->bQuantity;

	// Trade, Upgrade, Making, PlayerShop
	ItemNativeInfo_Dest.eSlotID				= (ITEM_NATIVE)pItemMove->bDest;
	ItemNativeInfo_Dest.dwOwnerID			= pUser->GetID();
	ItemNativeInfo_Dest.bySlotIndex			= pItemMove->bDestIndex;
	ItemNativeInfo_Dest.byItemQuantityCnt	= pItemMove->bQuantity;

	if(	ItemNativeInfo_Src.eSlotID	== ITEM_NATIVE_UPGRADE &&
		ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_UPGRADE)
	{
		if(ItemNativeInfo_Src.bySlotIndex == ITEM_UPGRADE_WEAPON)
		{
			if(	ItemNativeInfo_Dest.bySlotIndex == ITEM_UPGRADE_UPGRADE ||
				ItemNativeInfo_Dest.bySlotIndex == ITEM_UPGRADE_LIQUID	||
				ItemNativeInfo_Dest.bySlotIndex == ITEM_UPGRDAE_EDITION )
			{
				byErrorCode = MAKE_A_MISTAKE_ITEM_KIND;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
		else if(ItemNativeInfo_Dest.bySlotIndex == ITEM_UPGRADE_UPGRADE ||
				ItemNativeInfo_Dest.bySlotIndex == ITEM_UPGRADE_LIQUID	||
				ItemNativeInfo_Dest.bySlotIndex == ITEM_UPGRDAE_EDITION )
		{
			if(ItemNativeInfo_Dest.bySlotIndex == ITEM_UPGRADE_WEAPON)
			{
				byErrorCode = MAKE_A_MISTAKE_ITEM_KIND;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
	}
	else if(ItemNativeInfo_Src.eSlotID	== ITEM_NATIVE_MAKING &&
			ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_MAKING)
	{
		if(ItemNativeInfo_Src.bySlotIndex == ITEM_MAKING_RESULT)
		{
			if( ItemNativeInfo_Dest.bySlotIndex == ITEM_MAKING_MATERIALS1 ||
				ItemNativeInfo_Dest.bySlotIndex == ITEM_MAKING_MATERIALS2 ||
				ItemNativeInfo_Dest.bySlotIndex == ITEM_MAKING_ZODIAC)
			{
				byErrorCode = MAKE_A_MISTAKE_ITEM_KIND;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
		else if(ItemNativeInfo_Dest.bySlotIndex == ITEM_MAKING_MATERIALS1 ||
				ItemNativeInfo_Dest.bySlotIndex == ITEM_MAKING_MATERIALS2 ||
				ItemNativeInfo_Dest.bySlotIndex == ITEM_MAKING_ZODIAC )
		{
			if(ItemNativeInfo_Dest.bySlotIndex == ITEM_MAKING_RESULT)
			{
				byErrorCode = MAKE_A_MISTAKE_ITEM_KIND;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
	}

	bSuccess = ExchangeItemNativeLink(&ItemNativeInfo_Src,&ItemNativeInfo_Dest);
	
	if(!bSuccess)
	{
		byErrorCode = LINK_TO_LINK_FAIL;
	}

	SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
}


//=========================================================================================
// 다른 유저의 플레이어샵에 있는 아템을 산다.. 
//=========================================================================================
void CItemMoveManager::AnotherPlayerShopUserBuyItem(DWORD dwConnectionIndex
													, CUser* const pBuyer	/* 아템 사는 유저 */ 
													, CUser* const pSeller  /* 아템 파는 유저 */ 
													, CDungeonLayer* const pLayer
													, CTDS_ITEM_MOVE* const pItemMove
													, DSTC_ITEM_MOVE* const pItemMoveResult)
{
	BYTE				byErrorCode				= 0xff;
	BYTE				byQuantity				= NULL;
	DWORD				dwCustomSellPrice		= NULL;
	DWORD				dwCustomSellPricePack	= NULL;
	BOOL				bSuccess				= FALSE;
	BOOL				bIsOverlap				= FALSE;
	__int64				nBuyerMoney				= 0;	
	__int64				nSellerMoney			= 0;
	__int64				nCustomSellPrice		= 0;	
	CBaseItem*			pBaseItemSrc			= NULL;
	const CItem*		pItemSrc				= NULL;
	const CItem*		pItemDest				= NULL;
	ITEM_SHOP*			pItemShop				= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
	
	pItemShop = g_pItemShopHash->GetData(pSeller->GetID());

	if( pItemShop->byType != 1 )
	{					
		byErrorCode = ANOTHER_PLAYERSHOP_CLOSE;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	pItemSrc	= GetItemNativeLinkPointer(pSeller, pLayer, pItemMove, &ItemNativeInfo_Src, GET_SRC_NATIVEPOINTER);
	pItemDest	= GetItemNativePointer(pBuyer, pLayer, pItemMove, &ItemNativeInfo_Dest, GET_DEST_NATIVEPOINTER);

	if(NULL == pItemSrc || NULL == pItemDest)
	{
		byErrorCode = GET_CANNOT_NATIVEPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode); 
		return;
	}

	pBaseItemSrc = g_pBaseItemHash->GetData(pItemSrc->GetID());

	if(NULL == pBaseItemSrc)
	{
		byErrorCode = GET_CANNOT_BASEITEMPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}
	
	if( !IsDestNativeToCorrectType(pBuyer, ItemNativeInfo_Dest.eSlotID, ItemNativeInfo_Dest.bySlotIndex, pItemSrc) )
	{
		byErrorCode = MAKE_A_MISTAKE_ITEM_KIND;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( pItemSrc->GetID() != pItemMove->wPlayerShopItemID )
	{
		byErrorCode = ANOTHER_PLAYERSHOP_SELLITEM_CHANGE;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;	
	}

	// 링크건 아템의 수량과 패킷의 아이템 수량을 검사한다.
	if( ItemNativeInfo_Src.byItemQuantityCnt < pItemMove->bQuantity )
	{
		byErrorCode = MAKE_A_MISTAKE_ITEMQUANTITY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	dwCustomSellPrice		= pSeller->GetPlayerShopCustomSellPrice(pItemMove->bSrcIndex);
	dwCustomSellPricePack	= pSeller->GetPlayerShopCustomSellPrice(pItemMove->bSrcIndex) * pItemMove->bQuantity;

	if( dwCustomSellPrice != pItemMove->dwMoney )
	{
		byErrorCode = ANOTHER_PLAYERSHOP_SELLITEM_CUSTOMSELLPRICE_CHANGE;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;	
	}

	nBuyerMoney			= (__int64)pBuyer->GetMoney();
	nSellerMoney		= (__int64)pSeller->GetMoney();
	nCustomSellPrice	= (__int64)dwCustomSellPricePack;
	
	if( (nBuyerMoney - nCustomSellPrice) < 0 )
	{
		byErrorCode = BE_NOT_ENOUGH_MONEY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	if( (nSellerMoney + nCustomSellPrice) > __MAX_MONEY__)
	{
		byErrorCode = BE_FULL_OF_MONEY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	byQuantity	= pItemSrc->GetQuantity();	
	byQuantity	= pItemMove->bQuantity;

	// 소스 아이템을 클라이언트로 보낼 패킷에 일단 복사해 놓는다.
	memcpy(&pItemMoveResult->Item, pItemSrc, sizeof(CItem));

	// 소스 아이템을 살짝 복사해 놓는다.
	CItem cItemSrcTemp;
	memcpy(&cItemSrcTemp, pItemSrc, sizeof(CItem));

	if( cItemSrcTemp.IsOverlapItem() )
		bIsOverlap = TRUE;
	else
		bIsOverlap = FALSE;

	if( bIsOverlap )
	{
		BOOL bIsFullBuy = FALSE; 
		ITEM_NATIVE_INFO ItemNativeInfo_Link;
		const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
		ITEM_NATIVE_INFO ItemNativeInfo_ReSet;
	
		ItemNativeInfo_Link.eSlotID		= (ITEM_NATIVE)pItemMove->bSrc;
		ItemNativeInfo_Link.dwOwnerID	= pSeller->GetID();
		ItemNativeInfo_Link.bySlotIndex	= pItemMove->bSrcIndex;
		
		pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo_Link);
		memcpy(&ItemNativeInfo_ReSet,pItemNativeInfoLink,sizeof(ITEM_NATIVE_INFO));
		
		bSuccess = TypeSmallMoveItemNative(NULL, pLayer, pItemMove, pItemMoveResult
			, pItemSrc, pItemDest, &ItemNativeInfo_Src, &ItemNativeInfo_Dest, byQuantity, &byErrorCode);
		
		if(bSuccess)
		{
			// 아템사는 유저가 실제구입한 수량
			BYTE byBuyRealCnt = pItemMoveResult->bQuantity;	
			
			if(pItemNativeInfoLink->byItemQuantityCnt == byBuyRealCnt)
			{
				// 팔려고 올려논 아이템을 다 샀다.(ex:스몰포션5개 중 5개 다 구입)
				RemoveItemNativeLink(&ItemNativeInfo_Link);				
				pSeller->RemovePlayerShopCustonSellPrice(NULL, ItemNativeInfo_Link.bySlotIndex);
				pSeller->SetUserVirtualItemNativeLinkSell(&cItemSrcTemp, byBuyRealCnt);

				bIsFullBuy = TRUE;
			}
			else if(pItemNativeInfoLink->byItemQuantityCnt > byBuyRealCnt)
			{
				// 팔려고 올려논 아이템중 몇개만 샀다.(ex:스몰포션5개 중 3개 구입)
				ItemNativeInfo_ReSet.byItemQuantityCnt -= byBuyRealCnt;
				SetSlotItemNativeLink(ItemNativeInfo_Link.eSlotID
					, ItemNativeInfo_Link.bySlotIndex
					, &ItemNativeInfo_ReSet);
				pSeller->SetUserVirtualItemNativeLinkSell(&cItemSrcTemp, byBuyRealCnt);	
				
				bIsFullBuy = FALSE;
			}

			// 실제로 구입하고 판 가격 
			nCustomSellPrice = (__int64)dwCustomSellPrice * byBuyRealCnt;

			// Buyer(산놈) 처리 
			pBuyer->SetMoney((DWORD)(nBuyerMoney - nCustomSellPrice));
			pItemMoveResult->dwMoney = pBuyer->GetMoney();
			pItemMoveResult->Item.SetQuantity(byBuyRealCnt);
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);

			ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_PLAYERSHOP_BUY, pLayer->GetParent()->GetID()
				, (char*)pBuyer->GetName(), (char*)pSeller->GetName());
			KarzLogProcess(ITEM_LOG_PLAYERSHOP_BUY_KARZ, pLayer->GetParent()->GetID()
				, (char*)pBuyer->GetName(), (char*)pSeller->GetName(), (DWORD)nCustomSellPrice);

			// Seller(판놈) 처리
			DSTC_PLAYERSHOP_SELL SellPacket;
			pSeller->SetMoney((DWORD)(nSellerMoney + nCustomSellPrice));
			SellPacket.dwMoney		= pSeller->GetMoney();
			SellPacket.byZipCode	= pItemMove->bSrcIndex;

			if(bIsFullBuy)
			{
				// 팔려고 올려논 아이템을 다 샀다.(ex:스몰포션5개 중 5개 다 구입)
				memset(&SellPacket.cItem,0,sizeof(CItem));
			}
			else 
			{
				// 팔려고 올려논 아이템중 몇개만 샀다.(ex:스몰포션5개 중 3개 구입)	
				memset(&SellPacket.cItem,0,sizeof(CItem));
				SellPacket.byBuyRealCnt = byBuyRealCnt;
			}

			NetSendToUser(pSeller->m_dwConnectionIndex, (char*)&SellPacket, SellPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);

			ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_PLAYERSHOP_SELL, pLayer->GetParent()->GetID()
				, (char*)pSeller->GetName(), (char*)pBuyer->GetName());
			KarzLogProcess(ITEM_LOG_PLAYERSHOP_SELL_KARZ, pLayer->GetParent()->GetID()
				, (char*)pSeller->GetName(), (char*)pBuyer->GetName(), (DWORD)nCustomSellPrice);
			
			DWORD dwVirtualLinkItemNativeHashCount = pSeller->GetItemNativeLinkCount();

			if(!dwVirtualLinkItemNativeHashCount)
			{
				// 다팔렸구나. 장사끝!
				pSeller->SetPlayerShopMode(FALSE);
				pSeller->RemovePlayerShopTitle();

				ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pSeller->GetID());
				if(pItemShop)	
					pItemShop->byType = 0;

				SHOP_TILE* pNewShopTile = g_pShopTileHash->GetData(pSeller->GetID());
				if(pNewShopTile)
				{
					g_pShopTileHash->Delete(pNewShopTile, pSeller->GetID());
					memset(pNewShopTile, 0, sizeof(SHOP_TILE));
					pNewShopTile = NULL;
				}

				DSTC_PLAYERSHOP_CLOSE pClosePacket;
				pClosePacket.dwUserIndex = pSeller->GetID();
				pLayer->BroadCastMsg( (char*)&pClosePacket, pClosePacket.GetPacketSize());
				
				DSTC_PLAYERSHOP_SOLDOUT pSoldOutPacket;
				pSoldOutPacket.byType	= 1;									
				NetSendToUser(pSeller->m_dwConnectionIndex, (char*)&pSoldOutPacket, pSoldOutPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
			}
		}
		else
		{
			byErrorCode = ANOTHER_PLAYERSHOP_BUYITEM_FAIL;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}
	}
	else
	{
		// Seller(판놈) 처리 
		ITEM_NATIVE_INFO ItemNativeInfo_Link;

		ItemNativeInfo_Link.eSlotID				= (ITEM_NATIVE)pItemMove->bSrc;
		ItemNativeInfo_Link.dwOwnerID			= pSeller->GetID();
		ItemNativeInfo_Link.bySlotIndex			= pItemMove->bSrcIndex;
		ItemNativeInfo_Link.byItemQuantityCnt	= pItemMove->bQuantity;

		if( RemoveItemNativeLink(&ItemNativeInfo_Link) )
		{
			pSeller->RemovePlayerShopCustonSellPrice(NULL, ItemNativeInfo_Link.bySlotIndex);

			if( pSeller->SetUserVirtualItemNativeLinkSell(pItemSrc, ItemNativeInfo_Link.byItemQuantityCnt) )
			{
				bSuccess = TypeLargeMoveItemNative(NULL, pLayer, pItemMove, pItemMoveResult
					, pItemSrc, pItemDest, &ItemNativeInfo_Src, &ItemNativeInfo_Dest, byQuantity, &byErrorCode);			
				
				if(bSuccess)
				{
					// Buyer(산놈) 처리 
					pBuyer->SetMoney((DWORD)(nBuyerMoney - nCustomSellPrice));
					pItemMoveResult->dwMoney = pBuyer->GetMoney();
					SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
					
					ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_PLAYERSHOP_BUY, pLayer->GetParent()->GetID()
						, (char*)pBuyer->GetName(), (char*)pSeller->GetName());
					KarzLogProcess(ITEM_LOG_PLAYERSHOP_BUY_KARZ, pLayer->GetParent()->GetID()
						, (char*)pBuyer->GetName(), (char*)pSeller->GetName(), (DWORD)nCustomSellPrice);

					// Seller(판놈) 처리 
					DSTC_PLAYERSHOP_SELL SellPacket;
					pSeller->SetMoney((DWORD)(nSellerMoney + nCustomSellPrice));
					SellPacket.dwMoney		= pSeller->GetMoney();
					SellPacket.byZipCode	= pItemMove->bSrcIndex;
					memset(&SellPacket.cItem,0,sizeof(CItem));
					NetSendToUser(pSeller->m_dwConnectionIndex, (char*)&SellPacket, SellPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);

					ItemMoveLogProcess(&pItemMoveResult->Item, ITEM_LOG_PLAYERSHOP_SELL, pLayer->GetParent()->GetID()
						, (char*)pSeller->GetName(), (char*)pBuyer->GetName());
					KarzLogProcess(ITEM_LOG_PLAYERSHOP_SELL_KARZ, pLayer->GetParent()->GetID()
						, (char*)pSeller->GetName(), (char*)pBuyer->GetName(), (DWORD)nCustomSellPrice);
					
					DWORD dwVirtualLinkItemNativeHashCount = pSeller->GetItemNativeLinkCount();

					if(!dwVirtualLinkItemNativeHashCount)
					{
						// 다팔렸구나. 장사끝!
						pSeller->SetPlayerShopMode(FALSE);
						pSeller->RemovePlayerShopTitle();

						ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pSeller->GetID());
						if(pItemShop)	
							pItemShop->byType = 0;

						SHOP_TILE* pNewShopTile = g_pShopTileHash->GetData(pSeller->GetID());
						if(pNewShopTile)
						{
							g_pShopTileHash->Delete(pNewShopTile, pSeller->GetID());
							memset(pNewShopTile, 0, sizeof(SHOP_TILE));
							pNewShopTile = NULL;
						}

						DSTC_PLAYERSHOP_CLOSE pClosePacket;
						pClosePacket.dwUserIndex = pSeller->GetID();
						pLayer->BroadCastMsg( (char*)&pClosePacket, pClosePacket.GetPacketSize());
						
						DSTC_PLAYERSHOP_SOLDOUT pSoldOutPacket;
						pSoldOutPacket.byType	= 1;									
						NetSendToUser(pSeller->m_dwConnectionIndex
							, (char*)&pSoldOutPacket
							, pSoldOutPacket.GetPacketSize()
							, FLAG_SEND_NOT_ENCRYPTION);
					}

					return;
				}
				else
				{
					byErrorCode = ANOTHER_PLAYERSHOP_BUYITEM_FAIL;
					SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
					return;
				}
			}
			else
			{
				byErrorCode = ANOTHER_PLAYERSHOP_BUYITEM_FAIL;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
		else
		{
			byErrorCode = ANOTHER_PLAYERSHOP_BUYITEM_FAIL;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}
	}		
}


//=========================================================================================
// 링크 => 인벤라쥐,스몰 
//=========================================================================================
void CItemMoveManager::LinkSlotChangeLinkToInvProcess(DWORD dwConnectionIndex
													  ,CUser* const pUser
													  ,CDungeonLayer* const pLayer
													  ,CTDS_ITEM_MOVE* const pItemMove
													  ,DSTC_ITEM_MOVE* const pItemMoveResult)
{
	BYTE				byErrorCode			= 0xff;
	BYTE				byQuantity			= NULL;
	BOOL				bSuccess			= FALSE;
	const CItem*		pItemSrc			= NULL;
	const CItem*		pItemDest			= NULL;
	CBaseItem*			pBaseItemSrc		= NULL;
	CUser*				pSeller				= NULL;
	ITEM_TYPE			eType;
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
	ITEM_NATIVE_INFO	ItemNativeInfo_Link;
	
	if( !IsSrcToDestCorrect((ITEM_NATIVE)pItemMove->bSrc,(ITEM_NATIVE)pItemMove->bDest) )
	{
		byErrorCode = MAKE_A_MISTAKE_WAY;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	pSeller = g_pUserHash->GetData(pItemMove->dwUnitID);

	// 다른유저의 플레이어샵에 있는 아이템을 사는거당..
	if(pSeller && pSeller != pUser)
	{
		AnotherPlayerShopUserBuyItem(dwConnectionIndex, pUser, pSeller, pLayer, pItemMove, pItemMoveResult);
		return;
	}

	pItemSrc	= GetItemNativeLinkPointer(pUser, pLayer, pItemMove, &ItemNativeInfo_Src, GET_SRC_NATIVEPOINTER);
	pItemDest	= GetItemNativePointer(pUser, pLayer, pItemMove, &ItemNativeInfo_Dest, GET_DEST_NATIVEPOINTER);
	
	if(NULL == pItemSrc)
	{   
		byErrorCode = GET_CANNOT_NATIVEPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode); 
		return;
	}
	
	pBaseItemSrc = g_pBaseItemHash->GetData(pItemSrc->GetID());

	if(NULL == pBaseItemSrc)
	{
		byErrorCode = GET_CANNOT_BASEITEMPOINTER;
		SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
		return;
	}

	ItemNativeInfo_Link.eSlotID				= (ITEM_NATIVE)pItemMove->bSrc;
	ItemNativeInfo_Link.dwOwnerID			= pUser->GetID();
	ItemNativeInfo_Link.bySlotIndex			= pItemMove->bSrcIndex;
	ItemNativeInfo_Link.byItemQuantityCnt	= pItemMove->bQuantity;

	eType		= GetItemType(pItemSrc->GetID());
	byQuantity	= pItemSrc->GetQuantity();
	byQuantity	= pItemMove->bQuantity; 

	if(pItemDest->GetID() == 0)
	{
		// 데스트에 아이템이 없따..
		if( RemoveItemNativeLink(&ItemNativeInfo_Link) )
		{
			if( ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_PLAYERSHOP )
			{
				pUser->RemovePlayerShopCustonSellPrice(NULL, ItemNativeInfo_Link.bySlotIndex);
			}			

			if( pUser->SetUserVirtualItemNativeLinkReturn(pItemSrc, ItemNativeInfo_Link.byItemQuantityCnt
				, SET_LINK_TO_INV_EMPTY_SLOT) )
			{
				bSuccess = MoveItemNative(pLayer, &ItemNativeInfo_Src, &ItemNativeInfo_Dest, byQuantity, pItemMoveResult);

				if(bSuccess)
				{
					SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
					return;	
				}
				else
				{
					byErrorCode = LINK_TO_INV_FAIL;
					SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
					return;
				}
			}
			else
			{
				byErrorCode = LINK_TO_INV_FAIL;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
		else
		{
			byErrorCode = LINK_TO_INV_FAIL;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}			
	}
	else
	{
		// 데스트에 아이템이 있따. 링크하기전 자리로 돌아가버려.. 
		// 어쩔수 없따.. 씨바 
		pItemMoveResult->bDestIndex = ItemNativeInfo_Src.bySlotIndex;

		if( RemoveItemNativeLink(&ItemNativeInfo_Link) )
		{
			if( ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_PLAYERSHOP )
			{
				pUser->RemovePlayerShopCustonSellPrice(NULL, ItemNativeInfo_Link.bySlotIndex);
			}
			
			if( pUser->SetUserVirtualItemNativeLinkReturn(pItemSrc, ItemNativeInfo_Link.byItemQuantityCnt
				, SET_LINK_TO_INV_NATIVE_SLOT) )
			{
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;					
			}
			else
			{
				byErrorCode = LINK_TO_INV_FAIL;
				SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
				return;
			}
		}
		else
		{
			byErrorCode = LINK_TO_INV_FAIL;
			SendToItemMoveResult(dwConnectionIndex, pItemMoveResult, byErrorCode);
			return;
		}		
	}	
}


const CItem* CItemMoveManager::GetItemNativeLinkPointer(CUnit* const pUser
														,CDungeonLayer* const pLayer
														,CTDS_ITEM_MOVE* const pItemMove
														,ITEM_NATIVE_INFO* pItemNativeInfo
														,BYTE bIsSrcDest)
{
	const CItem*			pItemNative			= NULL;
	const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
	ITEM_NATIVE_INFO		ItemNativeInfo;	

	if(bIsSrcDest == GET_SRC_NATIVEPOINTER)
	{
		ItemNativeInfo.eSlotID = (ITEM_NATIVE)pItemMove->bSrc;

		if(::IsItemNativePlayer(ItemNativeInfo.eSlotID))
		{
			ItemNativeInfo.dwOwnerID	= pUser->GetID();
			ItemNativeInfo.bySlotIndex	= pItemMove->bSrcIndex;
		}
		else 
		{
			ItemNativeInfo.bySectionNum = pItemMove->bSectionNum;
			ItemNativeInfo.i64ItemID	= pItemMove->i64ItemID;		
		}
	}
	else if(bIsSrcDest == GET_DEST_NATIVEPOINTER)
	{
		ItemNativeInfo.eSlotID = (ITEM_NATIVE)pItemMove->bDest;

		if(::IsItemNativePlayer(ItemNativeInfo.eSlotID))
		{
			ItemNativeInfo.dwOwnerID	= pUser->GetID();
			ItemNativeInfo.bySlotIndex	= pItemMove->bDestIndex;
		}
		else 
		{
			ItemNativeInfo.bySectionNum = pItemMove->bSectionNum;
			ItemNativeInfo.i64ItemID	= pItemMove->i64ItemID;		
		}
	}
	
	pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);

	pItemNativeInfo->eSlotID			= pItemNativeInfoLink->eSlotID;
	pItemNativeInfo->bySlotIndex		= pItemNativeInfoLink->bySlotIndex;
	pItemNativeInfo->dwOwnerID			= pItemNativeInfoLink->dwOwnerID;
	pItemNativeInfo->byItemQuantityCnt	= pItemNativeInfoLink->byItemQuantityCnt; 

	pItemNative = GetItemNative(pLayer,pItemNativeInfo);

	return pItemNative;
}


const CItem* CItemMoveManager::GetItemNativePointer(CUnit* const pUnit
													,CDungeonLayer* const pLayer
													,CTDS_ITEM_MOVE* const pItemMove
													,ITEM_NATIVE_INFO* pItemNativeInfo
													,BYTE bIsSrcDest) 
{
	const CItem* pItemNative = NULL;
	
	if(bIsSrcDest == GET_SRC_NATIVEPOINTER)
	{
		pItemNativeInfo->eSlotID = (ITEM_NATIVE)pItemMove->bSrc;

		if(::IsItemNativePlayer(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->dwOwnerID		= pUnit->GetID();
			pItemNativeInfo->bySlotIndex	= pItemMove->bSrcIndex;
		}
		else if(::IsItemNativeTile(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->bySectionNum	= pItemMove->bSectionNum;
			pItemNativeInfo->i64ItemID		= pItemMove->i64ItemID;	
			pItemNativeInfo->dwMoney		= pUnit->GetID();
		}
		else if(::IsItemNativeTraderMonster(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->dwOwnerID		= pItemMove->dwUnitID;
			pItemNativeInfo->bySlotIndex	= pItemMove->bSrcIndex;
		}
		else if(::IsItemNativeKarz(pItemNativeInfo->eSlotID))
		{
			
		}
		else if(::IsItemDungeonManager(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->dwOwnerID		= pUnit->GetID();
			pItemNativeInfo->bySlotIndex	= pItemMove->bSrcIndex;
		}
	}
	else if(bIsSrcDest == GET_DEST_NATIVEPOINTER)
	{
		pItemNativeInfo->eSlotID = (ITEM_NATIVE)pItemMove->bDest;

		if(::IsItemNativePlayer(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->dwOwnerID		= pUnit->GetID();
			pItemNativeInfo->bySlotIndex	= pItemMove->bDestIndex;
		}
		else if(IsItemNativeTile(pItemNativeInfo->eSlotID))
		{
			return NULL;
		}
		else if(::IsItemNativeTraderMonster(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->dwOwnerID		= pItemMove->dwUnitID;
			pItemNativeInfo->bySlotIndex	= pItemMove->bSrcIndex;
		}
		else if(::IsItemNativeKarz(pItemNativeInfo->eSlotID))
		{
			
		}
		else if(::IsItemDungeonManager(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->dwOwnerID		= pUnit->GetID();
			pItemNativeInfo->bySlotIndex	= pItemMove->bSrcIndex;
		}
	}

	pItemNative	= GetItemNative(pLayer,pItemNativeInfo);
	
	return pItemNative;
}


void CItemMoveManager::SendToItemMoveResult(DWORD dwConnectionIndex
											,DSTC_ITEM_MOVE* const pItemMoveResult,BYTE bErrorCode)
{
	pItemMoveResult->bErrorCode = bErrorCode;
	NetSendToUser( dwConnectionIndex, (char*)pItemMoveResult, pItemMoveResult->GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION );	
}















