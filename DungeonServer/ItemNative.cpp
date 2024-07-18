#include "StdAfx.h"
#include "ItemNative.h"
#include "ItemMove.h"
#include "OwnServer.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "Map.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "MonsterManager.h"
#include "Section.h"
#include "TraderMonster.h"
#include "MonsterTable.h"
#include "User.h"
#include "ItemMoveManager.h"
#include "GameSystemNative.h"
#include "ItemAttrLayer.h"
#include "../CommonServer/CommonHeader.h"


BOOL IsItemDungeonManager(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_DUNGEONMANAGER == eItemNative;
}


BOOL IsItemNativeTile(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_TILE == eItemNative;
}


BOOL IsItemNativePlayer(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_PLAYER_START < eItemNative && ITEM_NATIVE_PLAYER_END > eItemNative;
}


BOOL IsItemNativeTraderMonster(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_TRADERMONSTER_START < eItemNative && ITEM_NATIVE_TRADERMONSTER_END > eItemNative;
}


BOOL IsItemNativeInvLargeAndSmall(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_INV_LARGE == eItemNative || ITEM_NATIVE_INV_SMALL == eItemNative;
}


BOOL IsItemNativeKarz(ITEM_NATIVE eItemNative)
{
	return ITEM_NATIVE_KARZ_START < eItemNative && ITEM_NATIVE_KARZ_END > eItemNative;
}


BYTE GetItemNativeDungeoManagerBufferCount()
{
	return ITEM_NATIVE_DUNGEONMANAGER_END - ITEM_NATIVE_DUNGEONMANAGER_START;
}


BYTE GetItemNativePlayerBufferCount()
{
	return ITEM_NATIVE_PLAYER_END - ITEM_NATIVE_PLAYER_START;
}


BYTE GetItemNativeTileBufferCount()
{
	return ITEM_NATIVE_TILE_END - ITEM_NATIVE_TILE_START;
}


BYTE GetItemNativeTraderMonsterCount()
{
	return ITEM_NATIVE_TRADERMONSTER_END - ITEM_NATIVE_TRADERMONSTER_START;
}


BOOL IsItemNativeSrcRemove(ITEM_NATIVE eItemNativeSrc,ITEM_NATIVE eItemNativeDest)
{
	if(	(eItemNativeSrc == ITEM_NATIVE_INV_LARGE	||	eItemNativeSrc == ITEM_NATIVE_INV_SMALL) &&  
		eItemNativeDest == ITEM_NATIVE_TRADE		||
		eItemNativeDest == ITEM_NATIVE_UPGRADE		||
		eItemNativeDest == ITEM_NATIVE_MAKING		||
		eItemNativeDest == ITEM_NATIVE_PLAYERSHOP )
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOL PickupItemNative(CDungeonLayer* pDungeonLayer
					  , const ITEM_NATIVE_INFO* pItemNativeInfo_Src
					  , ITEM_NATIVE_INFO* pItemNativeInfo_Dest
					  , DSTC_ITEM_MOVE* const pItemMoveResult)
{
	CItem cItemTile;
	memcpy(&cItemTile, GetItemNative(pDungeonLayer, pItemNativeInfo_Src), sizeof(CItem));
	
	ITEM_TYPE eItemType = GetItemType(cItemTile.GetID());
	BYTE byQuantity		= cItemTile.GetQuantity();

	if(ITEM_TYPE_LARGE == eItemType)
	{
		pItemNativeInfo_Dest->eSlotID = ITEM_NATIVE_INV_LARGE;
		pItemNativeInfo_Dest->bySlotIndex = 0;

		const CItem* pItemDest = GetItemNative(pDungeonLayer, pItemNativeInfo_Dest);
		BOOL bIsEmpty = FALSE; 

		for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(pItemDest[i].GetID() == 0)
			{
				bIsEmpty = TRUE;
				pItemNativeInfo_Dest->bySlotIndex = i;
				break;
			}
		}

		if(bIsEmpty)
		{
			if( SetSlotItemNative(pDungeonLayer, &cItemTile, pItemNativeInfo_Dest) )
			{
				memcpy(&pItemMoveResult->Item, &cItemTile, sizeof(CItem));
				pItemMoveResult->bDestIndex = pItemNativeInfo_Dest->bySlotIndex;
				return TRUE;
			}
			else
			{
				pItemMoveResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
				return FALSE;
			}
		}
		else
		{
			pItemMoveResult->bErrorCode = QUITE_FULL_SLOT;
			return FALSE;
		}
	}
	else if(ITEM_TYPE_SMALL == eItemType)
	{
		DWORD wKind = GetItemKind(cItemTile.GetID());

		if(wKind == ITEM_KIND_SUPPLIES)
		{
			pItemNativeInfo_Dest->eSlotID		= ITEM_NATIVE_BELT;
			pItemNativeInfo_Dest->bySlotIndex	= 0;

			const CItem* pItemDest = GetItemNative(pDungeonLayer, pItemNativeInfo_Dest);
			BOOL bIsEmpty = FALSE; 

			for(BYTE i = 0; i < MAX_BELT_POOL; i++)
			{
				if(cItemTile.GetID() == pItemDest[i].GetID())
				{
					BYTE byMaxQuantity	= pItemDest[i].GetBaseItem()->GetMaxQuantity();
					BYTE byCnt			= byMaxQuantity - pItemDest[i].GetQuantity();

					if(byCnt >= byQuantity)
					{
						bIsEmpty = TRUE;
						pItemNativeInfo_Dest->bySlotIndex = i;
						break;
					}
				}				
			}

			if(bIsEmpty)
			{
				CItem cItemDest;
				memcpy(&cItemDest, GetItemNative(pDungeonLayer,pItemNativeInfo_Dest), sizeof(CItem));
				cItemDest.SetQuantity(cItemDest.GetQuantity() + byQuantity);

				if( SetSlotItemNative(pDungeonLayer, &cItemDest, pItemNativeInfo_Dest) )
				{
					memcpy(&pItemMoveResult->Item, &cItemTile, sizeof(CItem));
					pItemMoveResult->bDest		= ITEM_NATIVE_BELT;
					pItemMoveResult->bDestIndex = pItemNativeInfo_Dest->bySlotIndex;
					return TRUE;
				}
				else
				{
					pItemMoveResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
					return FALSE;
				}
			}
			else
			{
				for(BYTE i = 0; i < MAX_BELT_POOL; i++)
				{
					if(pItemDest[i].GetID() == 0)
					{
						bIsEmpty = TRUE;
						pItemNativeInfo_Dest->bySlotIndex = i;
						break;
					}
				}

				if(bIsEmpty)
				{
					if( SetSlotItemNative(pDungeonLayer, &cItemTile, pItemNativeInfo_Dest) )
					{
						memcpy(&pItemMoveResult->Item, &cItemTile, sizeof(CItem));
						pItemMoveResult->bDest = ITEM_NATIVE_BELT;
						pItemMoveResult->bDestIndex = pItemNativeInfo_Dest->bySlotIndex;
						return TRUE;
					}
					else
					{
						pItemMoveResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
						return FALSE;
					}
				}
				else
				{
					pItemNativeInfo_Dest->eSlotID		= ITEM_NATIVE_INV_SMALL;
					pItemNativeInfo_Dest->bySlotIndex	= 0;
					pItemDest = GetItemNative(pDungeonLayer, pItemNativeInfo_Dest);

					for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						if(cItemTile.GetID() == pItemDest[i].GetID())
						{
							BYTE byMaxQuantity	= pItemDest[i].GetBaseItem()->GetMaxQuantity();
							BYTE byCnt			= byMaxQuantity - pItemDest[i].GetQuantity();

							if(byCnt >= byQuantity)
							{
								bIsEmpty = TRUE;
								pItemNativeInfo_Dest->bySlotIndex = i;
								break;
							}
						}
					}

					if(bIsEmpty)
					{
						CItem cItemDest;
						memcpy(&cItemDest, GetItemNative(pDungeonLayer,pItemNativeInfo_Dest), sizeof(CItem));
						cItemDest.SetQuantity(cItemDest.GetQuantity() + byQuantity);

						if( SetSlotItemNative(pDungeonLayer, &cItemDest, pItemNativeInfo_Dest) )
						{
							memcpy(&pItemMoveResult->Item, &cItemTile, sizeof(CItem));
							pItemMoveResult->bDest = ITEM_NATIVE_INV_SMALL;
							pItemMoveResult->bDestIndex = pItemNativeInfo_Dest->bySlotIndex;
							return TRUE;
						}
						else
						{
							pItemMoveResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
							return FALSE;
						}
					}
					else
					{
						for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(pItemDest[i].GetID() == 0)
							{
								bIsEmpty = TRUE;
								pItemNativeInfo_Dest->bySlotIndex = i;
								break;
							}
						}

						if(bIsEmpty)
						{
							if( SetSlotItemNative(pDungeonLayer, &cItemTile, pItemNativeInfo_Dest) )
							{
								memcpy(&pItemMoveResult->Item, &cItemTile, sizeof(CItem));
								pItemMoveResult->bDest = ITEM_NATIVE_INV_SMALL;
								pItemMoveResult->bDestIndex = pItemNativeInfo_Dest->bySlotIndex;
								return TRUE;
							}
							else
							{
								pItemMoveResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
								return FALSE;
							}
						}
						else
						{
							pItemMoveResult->bErrorCode = QUITE_FULL_SLOT;
							return FALSE;
						}
					}
				}
			}
		}
		else
		{
			pItemNativeInfo_Dest->eSlotID		= ITEM_NATIVE_INV_SMALL;
			pItemNativeInfo_Dest->bySlotIndex	= 0;

			const CItem* pItemDest = GetItemNative(pDungeonLayer, pItemNativeInfo_Dest);
			BOOL bIsEmpty = FALSE;

			for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
			{
				if(cItemTile.GetID() == pItemDest[i].GetID())
				{
					BYTE byMaxQuantity	= pItemDest[i].GetBaseItem()->GetMaxQuantity();
					BYTE byCnt			= byMaxQuantity - pItemDest[i].GetQuantity();

					if(byCnt >= byQuantity)
					{
						bIsEmpty = TRUE;
						pItemNativeInfo_Dest->bySlotIndex = i;
						break;
					}
				}
			}

			if(bIsEmpty)
			{
				CItem cItemDest;
				memcpy(&cItemDest, GetItemNative(pDungeonLayer,pItemNativeInfo_Dest), sizeof(CItem));
				cItemDest.SetQuantity(cItemDest.GetQuantity() + byQuantity);

				if( SetSlotItemNative(pDungeonLayer, &cItemDest, pItemNativeInfo_Dest) )
				{
					memcpy(&pItemMoveResult->Item, &cItemTile, sizeof(CItem));
					pItemMoveResult->bDest = ITEM_NATIVE_INV_SMALL;
					pItemMoveResult->bDestIndex = pItemNativeInfo_Dest->bySlotIndex;
					return TRUE;
				}
				else
				{
					pItemMoveResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
					return FALSE;
				}
			}
			else
			{
				for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(pItemDest[i].GetID() == 0)
					{
						bIsEmpty = TRUE;
						pItemNativeInfo_Dest->bySlotIndex = i;
						break;
					}
				}

				if(bIsEmpty)
				{
					if( SetSlotItemNative(pDungeonLayer, &cItemTile, pItemNativeInfo_Dest) )
					{
						memcpy(&pItemMoveResult->Item, &cItemTile, sizeof(CItem));
						pItemMoveResult->bDest		= ITEM_NATIVE_INV_SMALL;
						pItemMoveResult->bDestIndex = pItemNativeInfo_Dest->bySlotIndex;
						return TRUE;
					}
					else
					{
						pItemMoveResult->bErrorCode = SET_SLOT_ITEMNATIVE_FAIL;
						return FALSE;
					}
				}
				else
				{
					pItemMoveResult->bErrorCode = QUITE_FULL_SLOT;
					return FALSE;
				}
			}
		}			
	}
	else if(ITEM_TYPE_MONEY == eItemType)
	{
		CUser* pUser = g_pUserHash->GetData(pItemNativeInfo_Dest->dwOwnerID);
		if(!pUser)	
			return FALSE;

		DWORD dwUserMoney = pUser->GetMoney();
		DWORD dwTileMoney = cItemTile.m_Item_Money.dwMoney;

		if(dwUserMoney + dwTileMoney > __MAX_MONEY__)
		{
			pItemMoveResult->bErrorCode = BE_FULL_OF_MONEY;
			return FALSE;
		}

		pUser->SetMoney(dwUserMoney + dwTileMoney);
		memcpy(&pItemMoveResult->Item, &cItemTile, sizeof(CItem));
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL PickupItem(CDungeonLayer* pDungeonLayer
				, const ITEM_NATIVE_INFO* pItemNativeInfo_Src
				, ITEM_NATIVE_INFO* pItemNativeInfo_Dest
				, DSTC_ITEM_MOVE* const pItemMoveResult)
{
	if( IsItemNativeTile(pItemNativeInfo_Src->eSlotID) )
	{
		BOOL bSuccess = PickupItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest, pItemMoveResult);

		if(bSuccess)
		{
			CSection* pSection = pDungeonLayer->GetSection(pItemNativeInfo_Src->bySectionNum);
			if(pSection)
			{
				ITEM_SERVER* pItemDesc = pSection->m_pItemHash->GetData(pItemNativeInfo_Src->i64ItemID);			
				if(pItemDesc)
				{
					if( pDungeonLayer->RemoveItem(pItemDesc) )
					{
						DSTC_DISAPPEAR_ITEM	DisappearItem;
						DisappearItem.i64ItemID = pItemNativeInfo_Src->i64ItemID;
						pDungeonLayer->BroadCastSectionMsg( (char*)&DisappearItem, DisappearItem.GetPacketSize(), pItemNativeInfo_Src->bySectionNum );
					}					
				}
			}			
		}		
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


BOOL DropItem(CDungeonLayer* pDungeonLayer
			  , const ITEM_NATIVE_INFO* pItemNativeInfo_Src
			  , BYTE byQuantityCount
			  , DSTC_ITEM_MOVE* const pItemMoveResult)			  
{
	if( IsItemNativeInvLargeAndSmall(pItemNativeInfo_Src->eSlotID) )
	{
		CUser* pUser = g_pUserHash->GetData(pItemNativeInfo_Src->dwOwnerID);

		if(!pUser)
		{
			return FALSE;
		}
		
		CItem cItem;
		memcpy(&cItem, pUser->GetItem(pItemNativeInfo_Src->eSlotID, pItemNativeInfo_Src->bySlotIndex), sizeof(CItem));

		DWORD wKind = GetItemKind(cItem.GetID());

		if(wKind == ITEM_KIND_CONSUMABLE)
		{
			// 마일리지 아이템이면 현재 사용중인거인지 검사해야해..
			if( cItem.m_Item_Consumable.bInvIndex >= 1 )
			{
				return FALSE;
			}
		}

		if(cItem.GetID() == __ITEM_INSURANCE_INDEX__)
		{
			// 아이템 보험 적용 되어있어?
			if( cItem.m_Item_Zodiac.bInvIndex >= 1 )
			{
				return FALSE;
			}
		}

		BYTE byQuantityCnt		= cItem.GetQuantity();

#ifdef _DEBUG		
		CBaseItem* pBaseItem	= cItem.GetBaseItem();

		assert(byQuantityCnt);
		assert(byQuantityCnt >= byQuantityCount);
		assert(pBaseItem);
#endif

		if(cItem.IsOverlapItem())
		{
			CItem cAppearItem;
			memcpy(&cAppearItem,&cItem,sizeof(CItem));
			cAppearItem.SetQuantity(byQuantityCount);

			if( AppearItem(pUser, &cAppearItem, byQuantityCount, 0, TRUE,
				pUser->GetCurPosition()->x, 0, pUser->GetCurPosition()->y) )
			{
				if(byQuantityCnt > byQuantityCount)
				{
					cItem.SetSerialNum(g_pThis->GetNewSerial());
					cItem.SetQuantity(byQuantityCnt - byQuantityCount);
					
					if( SetSlotItemNative(pDungeonLayer, &cItem, pItemNativeInfo_Src) )
					{
						memcpy(&pItemMoveResult->Item,&cItem,sizeof(CItem));
						return TRUE;
					}
					else
					{
						return FALSE;
					}
				}
				else if(byQuantityCnt == byQuantityCount)
				{
					pUser->RemoveItem(pItemNativeInfo_Src->eSlotID, pItemNativeInfo_Src->bySlotIndex);
					return TRUE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			if( AppearItem(pUser, &cItem, byQuantityCount, 0, TRUE,
				pUser->GetCurPosition()->x, 0, pUser->GetCurPosition()->y) )
			{
				pUser->RemoveItem(pItemNativeInfo_Src->eSlotID, pItemNativeInfo_Src->bySlotIndex);
				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}		
	}
	else if( pItemNativeInfo_Src->eSlotID == ITEM_NATIVE_INV_KARZ )
	{
		CUser* pUser = g_pUserHash->GetData(pItemNativeInfo_Src->dwOwnerID);

		if(!pUser)
		{
			return FALSE;
		}

		DWORD dwUserMoney = pUser->GetMoney();
		DWORD dwDropMoney = pItemNativeInfo_Src->dwMoney;

		if(dwUserMoney < dwDropMoney)
		{
			return FALSE;
		}	
		
		if( AppearItem(pUser, NULL, byQuantityCount, dwDropMoney, TRUE,	pUser->GetCurPosition()->x, 0,
			pUser->GetCurPosition()->y) )
		{
			pItemMoveResult->dwMoney = dwDropMoney;
			pUser->SetMoney(dwUserMoney - dwDropMoney);
		}
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


//======================================================================================
// 링크와 링크 체인지 슬롯  
//======================================================================================
BOOL ExchangeItemNativeLink(const ITEM_NATIVE_INFO* pItemNativeInfo_Src, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	BOOL bSuccess = FALSE;

	ITEM_NATIVE_INFO ItemNativeInfoLink_Src;
	memcpy(&ItemNativeInfoLink_Src,GetItemNativeLink(pItemNativeInfo_Src),sizeof(ITEM_NATIVE_INFO));

	ITEM_NATIVE_INFO ItemNativeInfoLink_Dest;
	memcpy(&ItemNativeInfoLink_Dest,GetItemNativeLink(pItemNativeInfo_Dest),sizeof(ITEM_NATIVE_INFO));
	
	const CItem* pItemNativeSrc		= NULL;
	const CItem* pItemNativeDest	= NULL;
	
	pItemNativeSrc	= GetItemNative(NULL,&ItemNativeInfoLink_Src);
	pItemNativeDest = GetItemNative(NULL,&ItemNativeInfoLink_Dest);
	
	if(NULL == pItemNativeSrc && pItemNativeSrc->GetID() == 0)
	{
		return FALSE;
	}
	
	CUser* pUser = g_pUserHash->GetData(ItemNativeInfoLink_Src.dwOwnerID);

	if(pUser)
	{
		if(pItemNativeDest && pItemNativeDest->GetID() != 0)
		{
			bSuccess = SetSlotItemNativeLink(pItemNativeInfo_Dest->eSlotID
											,pItemNativeInfo_Dest->bySlotIndex
											,&ItemNativeInfoLink_Src);
			if(bSuccess)
			{
				if( pItemNativeInfo_Src->eSlotID == ITEM_NATIVE_PLAYERSHOP &&
					pItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_PLAYERSHOP)
				{
					DWORD dwGetCustomSellPrice = pUser->GetPlayerShopCustomSellPrice(pItemNativeInfo_Src->bySlotIndex);
					pUser->SetPlayerShopCustomSellPrice(dwGetCustomSellPrice, pItemNativeInfo_Dest->bySlotIndex);
				}
			}
			else
			{
				return FALSE;
			}

			bSuccess = SetSlotItemNativeLink(pItemNativeInfo_Src->eSlotID
											,pItemNativeInfo_Src->bySlotIndex
											,&ItemNativeInfoLink_Dest);
			if(bSuccess)
			{
				if( pItemNativeInfo_Src->eSlotID == ITEM_NATIVE_PLAYERSHOP &&
					pItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_PLAYERSHOP)
				{
					DWORD dwGetCustomSellPrice = pUser->GetPlayerShopCustomSellPrice(pItemNativeInfo_Dest->bySlotIndex);
					pUser->SetPlayerShopCustomSellPrice(dwGetCustomSellPrice, pItemNativeInfo_Src->bySlotIndex);
				}				
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			bSuccess = SetSlotItemNativeLink(pItemNativeInfo_Dest->eSlotID
											,pItemNativeInfo_Dest->bySlotIndex
											,&ItemNativeInfoLink_Src); 
			if(bSuccess)
			{
				if( pItemNativeInfo_Src->eSlotID == ITEM_NATIVE_PLAYERSHOP &&
					pItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_PLAYERSHOP)
				{
					DWORD dwGetCustomSellPrice = pUser->GetPlayerShopCustomSellPrice(pItemNativeInfo_Src->bySlotIndex);
					pUser->SetPlayerShopCustomSellPrice(dwGetCustomSellPrice, pItemNativeInfo_Dest->bySlotIndex);	
				}				
			}
			else
			{
				return FALSE;
			}

			bSuccess = RemoveItemNativeLink(pItemNativeInfo_Src);

			if(bSuccess)
			{
				if( pItemNativeInfo_Src->eSlotID == ITEM_NATIVE_PLAYERSHOP &&
					pItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_PLAYERSHOP)
				{
					pUser->RemovePlayerShopCustonSellPrice(NULL, pItemNativeInfo_Src->bySlotIndex);	
				}				
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		return FALSE;
	}

	return bSuccess;
}


//======================================================================================
// 장비 착용 
//======================================================================================
BOOL PutOnItemNative(CDungeonLayer* pDungeonLayer
					 , const ITEM_NATIVE_INFO* pItemNativeInfo_Src
					 , const ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	BOOL bSuccess = FALSE;

	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pDungeonLayer, pItemNativeInfo_Src), sizeof(CItem));
	assert(cItem_Src.GetID());
	
	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pDungeonLayer, pItemNativeInfo_Dest), sizeof(CItem));

	CUser* pUser = g_pUserHash->GetData(pItemNativeInfo_Src->dwOwnerID);
	if(NULL == pUser)
		return FALSE;
	
	if(pUser->GetUnitStatus() != UNIT_STATUS_NORMAL)
		return FALSE;
	
	// 착용 가능한지?
	if(pItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_EQUIP)
	{
		if( !pUser->CheckItem(&cItem_Src) )
		{
			return FALSE;
		}
	}
	
	BYTE byWeaponKind			= cItem_Src.GetBaseItem()->GetWeaponKind();
	BYTE byHandSlotIndex		= 0;
	BOOL IsDestItemExistence	= FALSE;

	if(pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_RHAND1)
		byHandSlotIndex = EQUIP_TYPE_LHAND1;
	else if(pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_RHAND2)
		byHandSlotIndex = EQUIP_TYPE_LHAND2;
	else if(pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_LHAND1)
		byHandSlotIndex = EQUIP_TYPE_RHAND1;
	else if(pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_LHAND2)
		byHandSlotIndex = EQUIP_TYPE_RHAND2;	

	// 들어갈 자리에 아이템이 있어?
	if(cItem_Dest.GetID())
		IsDestItemExistence = TRUE;
	else
		IsDestItemExistence = FALSE;
	
	// 무기류 
	if(byWeaponKind >= ITEM_CLASS_SWARD && byWeaponKind <= ITEM_CLASS_STAFF)
	{
		ITEM_WEAPON_TYPE eWeaponType = cItem_Src.GetWeaponType();

		// 투핸드 
		if( ITEM_WEAPON_TYPE_TWOHAND == eWeaponType )
		{
			if( pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_RHAND1 ||
				pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_RHAND2		)
			{
				CItem cItem_Hand;
				ITEM_NATIVE_INFO ItemNativeInfo_Hand;
				memcpy(&ItemNativeInfo_Hand,pItemNativeInfo_Dest,sizeof(ITEM_NATIVE_INFO));
				ItemNativeInfo_Hand.bySlotIndex = byHandSlotIndex;
				memcpy(&cItem_Hand, GetItemNative(pDungeonLayer, &ItemNativeInfo_Hand), sizeof(CItem));

				// LHand에 방패 아이템이 있으면 
				if( cItem_Hand.GetID() )
				{
					BOOL bIsEmpty = FALSE;
					
					const CItem* pItemInvLarge = NULL;
					ITEM_NATIVE_INFO ItemNativeInfo_Large;
					ItemNativeInfo_Large.eSlotID = ITEM_NATIVE_INV_LARGE;
					ItemNativeInfo_Large.dwOwnerID = pUser->GetID();
					ItemNativeInfo_Large.bySlotIndex = 0;
					ItemNativeInfo_Large.byItemQuantityCnt = 1;

					pItemInvLarge = GetItemNative(pDungeonLayer, &ItemNativeInfo_Large);						

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pItemInvLarge[i].GetID() == 0)
						{
							bIsEmpty = TRUE;
							ItemNativeInfo_Large.bySlotIndex = i;
							break;
						}
					}

					// Inv_Large에 빈슬롯이 있다.
					if(bIsEmpty)
					{
						// Equip에 있던 방패를 InvLarge로 
						bSuccess = MoveItemNative(pDungeonLayer, &ItemNativeInfo_Hand, &ItemNativeInfo_Large, cItem_Hand.GetQuantity(), NULL);														
						if(!bSuccess)
							return bSuccess;

						// InvLarge에있는 투핸드를 Equip로
						if(IsDestItemExistence)
							bSuccess = ExchangeItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest);
						else
							bSuccess = MoveItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity(), NULL);	
						return bSuccess;
					}
					else
					{
						// Inv_Large에 빈슬롯이 없다.
						return FALSE;
					}
				}
				else
				{
					// LHand에 방패 아이템이 없다. 
					if(IsDestItemExistence)
						bSuccess = ExchangeItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest);
					else
						bSuccess = MoveItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity(), NULL);	
					return bSuccess;
				}
			}				
		}
		else if( ITEM_WEAPON_TYPE_ONEHAND == eWeaponType )
		{
			// 원핸드 
			if(IsDestItemExistence)
				bSuccess = ExchangeItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest);
			else
				bSuccess = MoveItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity(), NULL);	
			return bSuccess;
		}
	}
	else if(byWeaponKind == ITEM_CLASS_SHIELD)
	{
		// 방패  
		if( pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_LHAND1 ||
			pItemNativeInfo_Dest->bySlotIndex == EQUIP_TYPE_LHAND2		)
		{
			CItem cItem_Hand;
			ITEM_NATIVE_INFO ItemNativeInfo_Hand;
			memcpy(&ItemNativeInfo_Hand,pItemNativeInfo_Dest,sizeof(ITEM_NATIVE_INFO));
			ItemNativeInfo_Hand.bySlotIndex = byHandSlotIndex;
			memcpy(&cItem_Hand, GetItemNative(pDungeonLayer, &ItemNativeInfo_Hand), sizeof(CItem));

			// RHand에 아이템이 있으면 
			if( cItem_Hand.GetID() )
			{
				ITEM_WEAPON_TYPE eWeaponType = cItem_Hand.GetWeaponType();
				
				// 투핸드 아이템 
				if( ITEM_WEAPON_TYPE_TWOHAND == eWeaponType )
				{
					BOOL bIsEmpty = FALSE;
					
					const CItem* pItemInvLarge = NULL;
					ITEM_NATIVE_INFO ItemNativeInfo_Large;
					ItemNativeInfo_Large.eSlotID = ITEM_NATIVE_INV_LARGE;
					ItemNativeInfo_Large.dwOwnerID = pUser->GetID();
					ItemNativeInfo_Large.bySlotIndex = 0;
					ItemNativeInfo_Large.byItemQuantityCnt = 1;

					pItemInvLarge = GetItemNative(pDungeonLayer, &ItemNativeInfo_Large);

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pItemInvLarge[i].GetID() == 0)
						{
							bIsEmpty = TRUE;
							ItemNativeInfo_Large.bySlotIndex = i;
							break;
						}
					}

					// Inv_Large에 빈슬롯이 있다.
					if(bIsEmpty)
					{
						// Equip에 있던 투핸드를 InvLarge로 
						bSuccess = MoveItemNative(pDungeonLayer, &ItemNativeInfo_Hand, &ItemNativeInfo_Large, cItem_Hand.GetQuantity(), NULL);														
						if(!bSuccess)
							return bSuccess;	

						// InvLarge에있는 방패를 Equip로 
						if(IsDestItemExistence)
							bSuccess = ExchangeItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest);
						else
							bSuccess = MoveItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity(), NULL);	
						return bSuccess;
					}
					else
					{
						// Inv_Large에 빈슬롯이 없다.
						return FALSE;
					}
				}
				else if( ITEM_WEAPON_TYPE_ONEHAND == eWeaponType )
				{
					// 원핸드 아이템
					// InvLarge에있는 방패를 Equip로 
					if(IsDestItemExistence)
						bSuccess = ExchangeItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest);
					else
						bSuccess = MoveItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity(), NULL);	
					return bSuccess;
				}
			}
			else
			{
				if(IsDestItemExistence)
					bSuccess = ExchangeItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest);
				else
					bSuccess = MoveItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity(), NULL);	
				return bSuccess;
			}
		}	
	}
	else
	{
		// 그외의 것들
		if(IsDestItemExistence)
			bSuccess = ExchangeItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest);
		else
			bSuccess = MoveItemNative(pDungeonLayer, pItemNativeInfo_Src, pItemNativeInfo_Dest, cItem_Src.GetQuantity(), NULL);	
		return bSuccess;
	}
	
	return bSuccess;
}


//======================================================================================
// 같은 창에서의 이동. ex)Inv Large => Inv Large
//======================================================================================
BOOL ExchangeItemNative(CDungeonLayer* pDungeonLayer
						, const ITEM_NATIVE_INFO* pItemNativeInfo_Src
						, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	BOOL bSuccess = FALSE;
	
	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pDungeonLayer, pItemNativeInfo_Src), sizeof(CItem));
	assert(cItem_Src.GetID());
	
	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pDungeonLayer, pItemNativeInfo_Dest), sizeof(CItem));

	if(cItem_Src.GetID() == cItem_Dest.GetID())
	{
		CBaseItem* pBaseItemDest = g_pBaseItemHash->GetData(cItem_Dest.GetID());
		assert(pBaseItemDest);		

		BYTE byMaxQuantity = pBaseItemDest->GetMaxQuantity();

		if(byMaxQuantity == 1)
		{
			bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Src, pItemNativeInfo_Dest);
			bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Dest, pItemNativeInfo_Src);
		}
		else
		{
			BYTE bySrcCnt = cItem_Src.GetQuantity();
			BYTE byDestCnt = cItem_Dest.GetQuantity();

			BYTE byPackInCnt = byMaxQuantity - byDestCnt;

			// 데스트 아이템 수량이 꽉 찼다. 기냥 자리만 바꾸자. 
			if(byPackInCnt == 0)
			{
				bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Src, pItemNativeInfo_Dest);
				bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Dest, pItemNativeInfo_Src);
				return bSuccess;
			}

			if(byPackInCnt >= bySrcCnt)
			{
				cItem_Dest.SetQuantity(byDestCnt + bySrcCnt);
				bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Dest, pItemNativeInfo_Dest);
				RemoveItemNative(pItemNativeInfo_Src);
			}
			else if(byPackInCnt != 0)
			{
				cItem_Dest.SetQuantity(byDestCnt + byPackInCnt);
				bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Dest, pItemNativeInfo_Dest);

				cItem_Src.SetQuantity(bySrcCnt - byPackInCnt);
				bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Src, pItemNativeInfo_Src);
			}
		}
	}
	else 
	{
		bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Src, pItemNativeInfo_Dest);
		bSuccess = SetSlotItemNative(pDungeonLayer, &cItem_Dest, pItemNativeInfo_Src);
	}

	return bSuccess;
}


//======================================================================================
// Dest에 아이템이 없을때 사용
//======================================================================================
BOOL MoveItemNative(CDungeonLayer* pDungeonLayer
				, const ITEM_NATIVE_INFO* pItemNativeInfo_Src
				, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest
				, BYTE byQuantityCount
				, DSTC_ITEM_MOVE* const pItemMoveResult)
{
	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pDungeonLayer, pItemNativeInfo_Src), sizeof(CItem));
	CBaseItem* pBaseItem_Src = cItem_Src.GetBaseItem();
	
	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pDungeonLayer, pItemNativeInfo_Dest), sizeof(CItem));

	assert(pBaseItem_Src);
	assert(byQuantityCount);
	assert(cItem_Src.GetID());
	assert(cItem_Src.GetQuantity() >= byQuantityCount);	

	BYTE byMaxQuantity = pBaseItem_Src->GetMaxQuantity();

	// 나누어 지지 않는 아이템이다.
	if(byMaxQuantity == 1 && byQuantityCount == 1)
	{
		if( SetSlotItemNative(pDungeonLayer, &cItem_Src, pItemNativeInfo_Dest) )
		{
			RemoveItemNative(pItemNativeInfo_Src);
			return TRUE;
		}
		else
		{
			return FALSE;
		}		
	}
	else
	{
		// 나눠 진다.
		BYTE bCntSrc		= cItem_Src.GetQuantity();
		BYTE bPackCntSrc	= bCntSrc - byQuantityCount;
		cItem_Src.SetQuantity(bPackCntSrc);

		memcpy(&cItem_Dest,&cItem_Src,sizeof(CItem));
		cItem_Dest.SetQuantity(byQuantityCount);

		if( SetSlotItemNative(pDungeonLayer, &cItem_Src, pItemNativeInfo_Src) )
		{
			if( SetSlotItemNative(pDungeonLayer, &cItem_Dest, pItemNativeInfo_Dest) )
			{
				// 흐흥.. 소스 아이템수량 전체를 데스트로 줬구만.
				if(bPackCntSrc == 0)
				{
					RemoveItemNative(pItemNativeInfo_Src);						
				}
				else
				{
					// 흐흥.. 소스 아이템 수량이 남아 있구만. 데스트 아이템에 뉴 시리얼이다.
					cItem_Dest.SetSerialNum(g_pThis->GetNewSerial());
					SetSlotItemNative(pDungeonLayer, &cItem_Dest, pItemNativeInfo_Dest);
					memcpy(&pItemMoveResult->Item, &cItem_Dest, sizeof(CItem));
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}


//======================================================================================
// Dest에 아이템이 있고 소스 아이템과 데스트 아이템의 아이디가 같을때 수량이동 할때 사용 
//======================================================================================
BOOL OverlapItemNative(CDungeonLayer* pDungeonLayer
				, const ITEM_NATIVE_INFO* pItemNativeInfo_Src
				, const ITEM_NATIVE_INFO* pItemNativeInfo_Dest
				, BYTE byQuantityCount
				, DSTC_ITEM_MOVE* const pItemMoveResult)
{
	pItemMoveResult;

	CItem cItem_Src;
	memcpy(&cItem_Src, GetItemNative(pDungeonLayer, pItemNativeInfo_Src), sizeof(CItem));
	CBaseItem* pBaseItem_Src = cItem_Src.GetBaseItem();
	
	CItem cItem_Dest;
	memcpy(&cItem_Dest, GetItemNative(pDungeonLayer, pItemNativeInfo_Dest), sizeof(CItem));

#ifdef _DEBUG
	CBaseItem* pBaseItem_Dest = cItem_Dest.GetBaseItem();
#endif

	assert(pBaseItem_Src);
	assert(pBaseItem_Dest);
	assert(byQuantityCount);
	assert(cItem_Src.GetID());
	assert(cItem_Dest.GetID());
	assert(cItem_Src.GetID() == cItem_Dest.GetID());
	assert(cItem_Src.GetQuantity() >= byQuantityCount);	

	BYTE byMaxQuantity = pBaseItem_Src->GetMaxQuantity();

	if(byMaxQuantity > 1)
	{
		// 수량이 중복되는 아이템이다.
		BYTE bCntSrc		= cItem_Src.GetQuantity();
		BYTE bPackCntSrc	= bCntSrc - byQuantityCount;
		cItem_Src.SetQuantity(bPackCntSrc);

		BYTE bCntDest		= cItem_Dest.GetQuantity();
		BYTE bPackCntDest	= bCntDest + byQuantityCount;
		cItem_Dest.SetQuantity(bPackCntDest);

		if( SetSlotItemNative(pDungeonLayer, &cItem_Src, pItemNativeInfo_Src) )
		{
			if( SetSlotItemNative(pDungeonLayer, &cItem_Dest, pItemNativeInfo_Dest) )
			{
				if(bPackCntSrc == 0)
				{
					RemoveItemNative(pItemNativeInfo_Src);
				}

				return TRUE;	
			}
		}
	}
//	else
//	{
//		// 수량 중복 안되는디 뭐꼬  
//	}

	return FALSE;
}


//======================================================================================
// 아이템을 소스에서 테스트로 이동시킨다.
//======================================================================================
BOOL SetSlotItemNative(CDungeonLayer* pDungeonLayer
					   , const CItem* pItem_Src
					   , const ITEM_NATIVE_INFO* pItemNativeInfo_Dest)
{
	pDungeonLayer;

	if (IsItemNativePlayer(pItemNativeInfo_Dest->eSlotID))
	{
		CUser* pUser = g_pUserHash->GetData(pItemNativeInfo_Dest->dwOwnerID);
		if (!pUser)
			return FALSE;
				
		pUser->SetItem(pItemNativeInfo_Dest->eSlotID, pItemNativeInfo_Dest->bySlotIndex, pItem_Src);
	}
	else if (IsItemNativeTraderMonster(pItemNativeInfo_Dest->eSlotID))
	{
		CTraderMonster* pTraderMonster = (CTraderMonster*)g_pMonsterTable->GetMonster(pItemNativeInfo_Dest->dwOwnerID);
		if (!pTraderMonster)
			return FALSE;
		if (pTraderMonster->GetRace() != OBJECT_TYPE_TRADER_MONSTER)
			return FALSE;

		pTraderMonster->SetItem(pItemNativeInfo_Dest->eSlotID, pItemNativeInfo_Dest->bySlotIndex, pItem_Src);
	}

	return TRUE;
}


BOOL SetSlotItemNativeLink(ITEM_NATIVE eFromItemNative
						   , BYTE byFromSlotIndex
						   , const ITEM_NATIVE_INFO* pToItemNativeLinkInfo)
{
	if (IsItemNativePlayer(eFromItemNative))
	{
		CUser* pUser = g_pUserHash->GetData(pToItemNativeLinkInfo->dwOwnerID);
		if(!pUser)
			return FALSE;

		pUser->SetItemLink(eFromItemNative,byFromSlotIndex,pToItemNativeLinkInfo);
	}

	return TRUE;
}


BOOL RemoveItemNative(const ITEM_NATIVE_INFO* pItemNativeInfo)
{
	if (IsItemNativePlayer(pItemNativeInfo->eSlotID))
	{
		CUser* pUser = g_pUserHash->GetData(pItemNativeInfo->dwOwnerID);
		if (!pUser)	
			return FALSE;
		
		pUser->RemoveItem(pItemNativeInfo->eSlotID, pItemNativeInfo->bySlotIndex);

		return TRUE;
	}
	
	return FALSE;
}


BOOL RemoveItemNativeLink(const ITEM_NATIVE_INFO* pItemNativeInfo)
{
	if (IsItemNativePlayer(pItemNativeInfo->eSlotID))
	{
		CUser* pUser = g_pUserHash->GetData(pItemNativeInfo->dwOwnerID);
		if (!pUser)	return FALSE;

		pUser->RemoveItemLink(pItemNativeInfo->eSlotID,pItemNativeInfo->bySlotIndex);

		return TRUE;
	}

	return FALSE;
}


//======================================================================================
// 아이템 포인터를 얻어온다.
//======================================================================================
const CItem* GetItemNative(CDungeonLayer* pDungeonLayer, const ITEM_NATIVE_INFO* pItemNativeInfo)
{
	// 유저를 조사하면 된다.
	if (IsItemNativePlayer(pItemNativeInfo->eSlotID))
	{
		CUser* pUser = g_pUserHash->GetData(pItemNativeInfo->dwOwnerID);
		if (!pUser)	
			return NULL;
		
		return pUser->GetItem(pItemNativeInfo->eSlotID, pItemNativeInfo->bySlotIndex);
	}
	else if (IsItemNativeTile(pItemNativeInfo->eSlotID))
	{
		CSection*		pSection	= pDungeonLayer->GetSection(pItemNativeInfo->bySectionNum);
		ITEM_SERVER*	pItemDesc	= pSection->m_pItemHash->GetData(pItemNativeInfo->i64ItemID);
		
		if(pItemDesc)
		{
			if(pItemDesc->dwOwnerIndex)
			{
				// 아흐 짜증, dwMoney는 UserIndex 임, 소스 개판!! : 최덕석 2005.1.26
				CUser* pUser = g_pUserHash->GetData(pItemNativeInfo->dwMoney);

				if(pItemDesc->dwPartyIndex != 0 && pItemDesc->dwPartyIndex == pUser->GetPartyID())
				{
					// 파티플 아이템의 경우는 허가해줌
				}
				else if(pItemDesc->dwOwnerIndex != pUser->GetID())
				{
					// 소유자가 아니면 불허가
					return NULL;
				}
			}

			return &pItemDesc->Item;
		}
	}
	else if (IsItemNativeTraderMonster(pItemNativeInfo->eSlotID))
	{
		CTraderMonster* pTraderMonster = (CTraderMonster*)g_pMonsterTable->GetMonster(pItemNativeInfo->dwOwnerID);
		if (!pTraderMonster)
			return NULL;
		if (pTraderMonster->GetRace() != OBJECT_TYPE_TRADER_MONSTER)
			return NULL;

		return pTraderMonster->GetItem(pItemNativeInfo->eSlotID, pItemNativeInfo->bySlotIndex);
	}
	else if (IsItemDungeonManager(pItemNativeInfo->eSlotID))
	{
		CUser* pUser = g_pUserHash->GetData(pItemNativeInfo->dwOwnerID);
		if (!pUser)			return NULL;
		
		CDungeon* pDungeon = pUser->GetCurDungeon();
		if (!pDungeon)		return NULL;

		DUNGEON_DATA_EX* pDungeonDataEx = pDungeon->GetDungeonDataEx();
		if (pDungeonDataEx)	return NULL;

		BOOL bIsOwner = pDungeonDataEx->IsDungeonOwner(pUser);
		if (!bIsOwner)		return NULL;

		if(pItemNativeInfo->bySlotIndex)
			return pDungeonDataEx->GetGuardianItem();
		else
			return pDungeonDataEx->GetMagicFieldArrayItem();
	}
	
	return NULL;
}


const ITEM_NATIVE_INFO* GetItemNativeLink(const ITEM_NATIVE_INFO* pItemNativeInfo)
{
	CUser* pUser = g_pUserHash->GetData(pItemNativeInfo->dwOwnerID);
	if(!pUser) return NULL;

	return pUser->GetItemLink(pItemNativeInfo->eSlotID, pItemNativeInfo->bySlotIndex);
}

BOOL IsItemNativeLink(const ITEM_NATIVE_INFO* pItemNativeInfo)
{
	const CItem*			pItemNative			= NULL;
	const ITEM_NATIVE_INFO* pItemNativeInfoLink = GetItemNativeLink(pItemNativeInfo);

	if( ITEM_NATIVE_INV_LARGE == pItemNativeInfoLink->eSlotID ||
		ITEM_NATIVE_INV_SMALL == pItemNativeInfoLink->eSlotID )
	{
		pItemNative = GetItemNative(NULL,pItemNativeInfoLink);

		if(pItemNative && pItemNative->GetID() != 0)
		{
			return TRUE;
		}		
	}

	return FALSE;
}




BOOL IsSamePropertyConsumableItem(CItem* pSrc, CItem* pDst)
{
	int s = pSrc->GetID() / ITEM_DISTRIBUTE;
	int d = pDst->GetID() / ITEM_DISTRIBUTE;

	if(!(s>=ITEM_CONSUMABLEST_INDEX && s<=ITEM_CONSUMABLEEND_INDEX)
	|| !(d>=ITEM_CONSUMABLEST_INDEX && d<=ITEM_CONSUMABLEEND_INDEX))
		return FALSE;

	CBaseItem *pSrcBase = g_pBaseItemHash->GetData(pSrc->GetID());
	CBaseItem *pDstBase = g_pBaseItemHash->GetData(pDst->GetID());

	if((pSrcBase->wID >= 20201 && pSrcBase->wID <= 20219)
	&& (pDstBase->wID >= 20201 && pDstBase->wID <= 20219))
		return TRUE;

	return FALSE;

	/*
	
	for(s=0;s<MAX_BASEITEM_CONSUMABLE_ITEMATTR;s++)
	{
		WORD wSRCRCode = pSrcBase->BaseItem_Consumable.sItem_Attr[s].wAttr_Code;
		WORD wSRCRValue = pSrcBase->BaseItem_Consumable.sItem_Attr[s].wAttr_Value;
		WORD wSRCVCode = g_pItemAttrLayer->GetVirtualCode(wSRCRCode, wSRCRValue);
					
		if(wSRCVCode != 112)
			continue;
					
		for(d=0;d<MAX_BASEITEM_CONSUMABLE_ITEMATTR;d++)
		{
			WORD wDSTRCode = pDstBase->BaseItem_Consumable.sItem_Attr[d].wAttr_Code;
			WORD wDSTRValue = pDstBase->BaseItem_Consumable.sItem_Attr[d].wAttr_Value;
			WORD wDSTVCode = g_pItemAttrLayer->GetVirtualCode(wDSTRCode, wDSTRValue);
					
			if(wDSTVCode != 112)
				continue;

			if(wSRCVCode == wDSTVCode)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
	*/
}