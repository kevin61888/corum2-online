#include "StdAfx.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "../CommonServer/CommonHeader.h"
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
#include "MsgProc_for_jjw6263.h"
#include "GameSystemNative.h"


void GuardianSupplies_Cancel(CUser* pUser)
{
	DSTC_GUARDIANSUPPLIES_CANCAL_RETURN_INV ReturnInvPacket;

	ReturnInvPacket.ItemNodeNative.eSlotID		= ITEM_NATIVE_GUARDIAN;
	ReturnInvPacket.ItemNodeNative.bySlotIndex	= 0; 

	const CItem*		pItemNative = NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo;

	ItemNativeInfo.eSlotID		= ITEM_NATIVE_GUARDIAN;
	ItemNativeInfo.bySlotIndex	= 0;
	ItemNativeInfo.dwOwnerID	= pUser->GetID();

	pItemNative = GetItemNative(pUser->GetCurDungeonLayer(), &ItemNativeInfo);

	if(pItemNative && pItemNative->GetID() != 0)
	{
		BYTE byCnt = pItemNative->GetQuantity();

		// 인벤 스몰에 빈슬롯을 찾아라  
		const CItem*		pItemNativeDest = NULL;
		ITEM_NATIVE_INFO	ItemNativeInfoSmall;

		ItemNativeInfoSmall.eSlotID		= ITEM_NATIVE_INV_SMALL;
		ItemNativeInfoSmall.dwOwnerID	= pUser->GetID();
		ItemNativeInfoSmall.bySlotIndex = 0;	

		pItemNativeDest = GetItemNative(pUser->GetCurDungeonLayer(), &ItemNativeInfoSmall);
		
		for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
		{
			ItemNativeInfoSmall.bySlotIndex = i;

			if(pItemNativeDest[i].GetID() == 0)
			{
				MoveItemNative(pUser->GetCurDungeonLayer(), &ItemNativeInfo, &ItemNativeInfoSmall, byCnt, NULL);
	
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
			else if(pItemNativeDest[i].GetID() == pItemNative->GetID())
			{
				BYTE byMaxQuantity	= pItemNativeDest[i].GetBaseItem()->GetMaxQuantity();
				BYTE byPutInCnt		= byMaxQuantity - pItemNativeDest[i].GetQuantity();

				if(byPutInCnt >= byCnt)
				{
					OverlapItemNative(pUser->GetCurDungeonLayer(),&ItemNativeInfo, &ItemNativeInfoSmall, byCnt, NULL);

					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
				else if(byPutInCnt != 0)
				{
					OverlapItemNative(pUser->GetCurDungeonLayer(),&ItemNativeInfo, &ItemNativeInfoSmall, byPutInCnt, NULL);
					byCnt -= byPutInCnt;
				}
			}
		}

		// 여기까지 왔으면 인벤 스몰에 다 못넣은거다. 벨트에 넣자.
		ItemNativeInfoSmall.eSlotID		= ITEM_NATIVE_BELT;
		ItemNativeInfoSmall.dwOwnerID	= pUser->GetID();
		ItemNativeInfoSmall.bySlotIndex = 0;	

		pItemNativeDest = GetItemNative(pUser->GetCurDungeonLayer(), &ItemNativeInfoSmall);

		for(i = 0; i < MAX_BELT_POOL; i++)
		{
			ItemNativeInfoSmall.bySlotIndex = i;

			if(pItemNativeDest[i].GetID() == 0)
			{
				MoveItemNative(pUser->GetCurDungeonLayer(), &ItemNativeInfo, &ItemNativeInfoSmall, byCnt, NULL);
	
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
			else if(pItemNativeDest[i].GetID() == pItemNative->GetID())
			{
				BYTE byMaxQuantity	= pItemNativeDest[i].GetBaseItem()->GetMaxQuantity();
				BYTE byPutInCnt		= byMaxQuantity - pItemNativeDest[i].GetQuantity();

				if(byPutInCnt >= byCnt)
				{
					OverlapItemNative(pUser->GetCurDungeonLayer(),&ItemNativeInfo, &ItemNativeInfoSmall, byCnt, NULL);

					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
				else if(byPutInCnt != 0)
				{
					OverlapItemNative(pUser->GetCurDungeonLayer(),&ItemNativeInfo, &ItemNativeInfoSmall, byPutInCnt, NULL);
					byCnt -= byPutInCnt;
				}
			}
		}

		// 여기까지 왔으면 벨트에 다 못넣은거다. 창고에 넣자.
		ItemNativeInfoSmall.eSlotID		= ITEM_NATIVE_BANK_SMALL;
		ItemNativeInfoSmall.dwOwnerID	= pUser->GetID();
		ItemNativeInfoSmall.bySlotIndex = 0;	

		pItemNativeDest = GetItemNative(pUser->GetCurDungeonLayer(), &ItemNativeInfoSmall);

		for(i = 0; i < MAX_BELT_POOL; i++)
		{
			ItemNativeInfoSmall.bySlotIndex = i;

			if(pItemNativeDest[i].GetID() == 0)
			{
				MoveItemNative(pUser->GetCurDungeonLayer(), &ItemNativeInfo, &ItemNativeInfoSmall, byCnt, NULL);
	
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
			else if(pItemNativeDest[i].GetID() == pItemNative->GetID())
			{
				BYTE byMaxQuantity	= pItemNativeDest[i].GetBaseItem()->GetMaxQuantity();
				BYTE byPutInCnt		= byMaxQuantity - pItemNativeDest[i].GetQuantity();

				if(byPutInCnt >= byCnt)
				{
					OverlapItemNative(pUser->GetCurDungeonLayer(),&ItemNativeInfo, &ItemNativeInfoSmall, byCnt, NULL);

					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
				else if(byPutInCnt != 0)
				{
					OverlapItemNative(pUser->GetCurDungeonLayer(),&ItemNativeInfo, &ItemNativeInfoSmall, byPutInCnt, NULL);
					byCnt -= byPutInCnt;
				}
			}
		}
	}	
}


//==============================================
// 제조창에 있던 아이템을 인벤으로 돌린다.
//==============================================
void Making_Cancel(CUser* pUser)
{
	DSTC_MAKING_CANCAL_RETURN_INV ReturnInvPacket;

	ITEM_NATIVE_INFO ItemNativeInfo_Link;
	ITEM_NATIVE_INFO ItemNativeInfo_Real;

	const CItem*			pItemNative			= NULL;
	const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

	ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_MAKING;
	ItemNativeInfo_Link.dwOwnerID	= pUser->GetID();

	ReturnInvPacket.bItemCount = 0;

	for(int i = 0; i < MAX_MAKING_POOL; i++)
	{
		ItemNativeInfo_Link.bySlotIndex	= i;
		pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo_Link);

		ItemNativeInfo_Real.eSlotID				= pItemNativeInfoLink->eSlotID;
		ItemNativeInfo_Real.bySlotIndex			= pItemNativeInfoLink->bySlotIndex;
		ItemNativeInfo_Real.dwOwnerID			= pItemNativeInfoLink->dwOwnerID;
		ItemNativeInfo_Real.byItemQuantityCnt	= pItemNativeInfoLink->byItemQuantityCnt; 

		pItemNative = GetItemNative(NULL,&ItemNativeInfo_Real);

		if(pItemNative && pItemNative->GetID() != 0)
		{
			// 클라이언트 처리
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].index		= i;
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].eSlotID		= ItemNativeInfo_Real.eSlotID;
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].bySlotIndex	= ItemNativeInfo_Real.bySlotIndex;
			ReturnInvPacket.bItemCount++;

			// 서버처리 
			RemoveItemNativeLink(&ItemNativeInfo_Link);
		}
	}

	pUser->RemoveAllVirtualItemNativeLink();

	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);
}


//==============================================
// 업그레이드창에 있던 아이템을 인벤으로 돌린다.
//==============================================
void UpGrade_Cancel(CUser* pUser)
{
	DSTC_UPGRADE_CANCAL_RETURN_INV ReturnInvPacket;

	ITEM_NATIVE_INFO ItemNativeInfo_Link;
	ITEM_NATIVE_INFO ItemNativeInfo_Real;

	const CItem*			pItemNative			= NULL;
	const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

	ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_UPGRADE;
	ItemNativeInfo_Link.dwOwnerID	= pUser->GetID();

	ReturnInvPacket.bItemCount = 0;

	for(int i = 0; i < MAX_UPGRADE_POOL; i++)
	{
		ItemNativeInfo_Link.bySlotIndex	= i;
		pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo_Link);

		ItemNativeInfo_Real.eSlotID				= pItemNativeInfoLink->eSlotID;
		ItemNativeInfo_Real.bySlotIndex			= pItemNativeInfoLink->bySlotIndex;
		ItemNativeInfo_Real.dwOwnerID			= pItemNativeInfoLink->dwOwnerID;
		ItemNativeInfo_Real.byItemQuantityCnt	= pItemNativeInfoLink->byItemQuantityCnt; 

		pItemNative = GetItemNative(NULL,&ItemNativeInfo_Real);

		if(pItemNative && pItemNative->GetID() != 0)
		{
			// 클라이언트 처리
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].index		= i;
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].eSlotID		= ItemNativeInfo_Real.eSlotID;
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].bySlotIndex	= ItemNativeInfo_Real.bySlotIndex;
			ReturnInvPacket.bItemCount++;

			// 서버처리 
			RemoveItemNativeLink(&ItemNativeInfo_Link);
		}
	}

	pUser->RemoveAllVirtualItemNativeLink();

	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);
}


//============================================
// 플레이어샵에 있던 아이템을 인벤으로 돌린다.
//============================================
void PlayerShop_Cancel(CUser* pUser)
{
	DSTC_PLAYERSHOPITEM_RETURN_INV ReturnInvPacket;

	ITEM_NATIVE_INFO ItemNativeInfo_Link;
	ITEM_NATIVE_INFO ItemNativeInfo_Real;

	const CItem*			pItemNative			= NULL;
	const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

	ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_PLAYERSHOP;
	ItemNativeInfo_Link.dwOwnerID	= pUser->GetID();

	ReturnInvPacket.bItemCount = 0;

	for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
	{
		ItemNativeInfo_Link.bySlotIndex	= i;
		pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo_Link);

		ItemNativeInfo_Real.eSlotID				= pItemNativeInfoLink->eSlotID;
		ItemNativeInfo_Real.bySlotIndex			= pItemNativeInfoLink->bySlotIndex;
		ItemNativeInfo_Real.dwOwnerID			= pItemNativeInfoLink->dwOwnerID;
		ItemNativeInfo_Real.byItemQuantityCnt	= pItemNativeInfoLink->byItemQuantityCnt; 

		pItemNative = GetItemNative(NULL,&ItemNativeInfo_Real);

		if(pItemNative && pItemNative->GetID() != 0)
		{
			// 클라이언트 처리
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].index		= i;
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].eSlotID		= ItemNativeInfo_Real.eSlotID;
			ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].bySlotIndex	= ItemNativeInfo_Real.bySlotIndex;
			ReturnInvPacket.bItemCount++;

			// 서버처리 
			RemoveItemNativeLink(&ItemNativeInfo_Link);
			pUser->RemovePlayerShopCustonSellPrice(NULL,ItemNativeInfo_Link.bySlotIndex);
		}
	}

	pUser->RemoveAllVirtualItemNativeLink();
	pUser->RemovePlayerShopTitle();
	
	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);
}


//==========================================
// 트레이드에 있던 아이템을 인벤으로 돌린다.
//==========================================
void Trade_Cancel(CUser* pUser)
{
	if( IsUltra() )
	{
		DSTC_TRADE_CANCEL_RETURN_INV ReturnInvPacket;
		ReturnInvPacket.byType = 0;

		ITEM_NATIVE_INFO ItemNativeInfo_Link;
		ITEM_NATIVE_INFO ItemNativeInfo_Real;

		const CItem*			pItemNative			= NULL;
		const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

		ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_TRADE;
		ItemNativeInfo_Link.dwOwnerID	= pUser->GetID();

		ReturnInvPacket.bItemCount = 0;

		for(int i = 0; i < MAX_TRADE_POOL; i++)
		{
			ItemNativeInfo_Link.bySlotIndex	= i;
			pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo_Link);

			ItemNativeInfo_Real.eSlotID				= pItemNativeInfoLink->eSlotID;
			ItemNativeInfo_Real.bySlotIndex			= pItemNativeInfoLink->bySlotIndex;
			ItemNativeInfo_Real.dwOwnerID			= pItemNativeInfoLink->dwOwnerID;
			ItemNativeInfo_Real.byItemQuantityCnt	= pItemNativeInfoLink->byItemQuantityCnt; 

			pItemNative = GetItemNative(NULL,&ItemNativeInfo_Real);

			if(pItemNative && pItemNative->GetID() != 0)
			{
				// 클라이언트 처리
				ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].index		= i;
				ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].eSlotID		= ItemNativeInfo_Real.eSlotID;
				ReturnInvPacket.ItemNodeNative[ReturnInvPacket.bItemCount].bySlotIndex	= ItemNativeInfo_Real.bySlotIndex;
				ReturnInvPacket.bItemCount++;

				// 서버처리 
				RemoveItemNativeLink(&ItemNativeInfo_Link);				
			}
		}

		pUser->RemoveAllVirtualItemNativeLink();

		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&ReturnInvPacket, ReturnInvPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		DSTC_TRADE_CANCEL pUserPacket;
		pUserPacket.byType = 0;

		for(int i = 0; i < MAX_TRADE_POOL; i++)
		{
			pUserPacket.bInvType[i]	= 0;

			if(pUser->m_pTradeItem[i].GetID() != 0)
			{
				pUserPacket.Nodes[pUserPacket.btItemCount].index = i;
				memcpy(&pUserPacket.Nodes[pUserPacket.btItemCount].cItem, &pUser->m_pTradeItem[i], sizeof(CItem));
				pUserPacket.btItemCount++;
			}
		}

		// Item Move //
		for(i = 0; i < MAX_TRADE_POOL; i++)
		{
			int nValue = pUser->m_pTradeItem[i].GetID();

			if(pUser->m_pTradeItem[i].GetID()!=0)
			{
				if(GetItemType(nValue)==ITEM_TYPE_SMALL)			
				{
					// 스몰 아이템 //
					Insert_SmallInvItem(pUser, &pUser->m_pTradeItem[i], 0, pUser->m_pTradeItem[i].GetQuantity(), TRUE);
				}
				else if(GetItemType(nValue)==ITEM_TYPE_LARGE)			
				{
					// 큰 아이템 //
					BOOL bChk = FALSE;

					for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
					{
						if(pUser->m_pInv_Large[j].GetID()==0)
						{
							memcpy(&pUser->m_pInv_Large[j], &pUser->m_pTradeItem[i], sizeof(CItem));
							bChk = TRUE;
							break;
						}
					}

					if(!bChk)
					{
						for(j = 0; j < MAX_BANK_LARGE_POOL; j++)
						{
							if(pUser->m_pBank_Large[j].GetID()==0)
							{
								memcpy(&pUser->m_pBank_Large[j], &pUser->m_pTradeItem[i], sizeof(CItem));
								
								for(int k = 0; k < MAX_TRADE_POOL; k++)
								{
									if(pUserPacket.Nodes[k].index==i)
									{
										pUserPacket.bInvType[k]	= 1;
										break;
									}
								}

								break;
							}
						}
					}				
				}
			}

			memset(&pUser->m_pTradeItem[i], 0, sizeof(CItem));
		}

		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pUserPacket, pUserPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);	
	}
}


BOOL Trade_SmallItem(CItem* pInvItem, CItem* pItem)
{
	if(!pInvItem)
		return FALSE;
	if(!pItem)
		return FALSE;

	BOOL	bChk		= FALSE;
//	int		nValue		= pItem->GetID()/ITEM_DISTRIBUTE;
	BYTE	byPackCnt	= pItem->GetQuantity();
		
	for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(pInvItem[i].GetID()==0)
		{
			memcpy(&pInvItem[i], pItem, sizeof(CItem));	
			pInvItem[i].SetQuantity(byPackCnt);
			bChk	= TRUE;
			break;
		}
		else if(pInvItem[i].GetID()==pItem->GetID())
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pInvItem[i].m_wItemID);

			if(!pBaseItem)
				return 0;

//			int nValue			= pInvItem[i].m_wItemID/ITEM_DISTRIBUTE;
			int nMaxQuantity	= 1;

			nMaxQuantity = pBaseItem->GetMaxQuantity();
			
			BYTE byCnt = nMaxQuantity - pInvItem[i].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = pInvItem[i].GetQuantity();
				pInvItem[i].SetQuantity(bQuantity+byPackCnt);
				bChk	= TRUE;					
				break;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = pInvItem[i].GetQuantity();
				pInvItem[i].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
			}
		}
	}
	
	if(bChk==FALSE)
		return FALSE;		

	return TRUE;
}


BYTE Insert_PlayerShopItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{
	BYTE byResult	= 0xff;
	BOOL bAppearChk = FALSE;

//	int nValue = pItem->GetID()/ITEM_DISTRIBUTE;
	
	byResult = bZipCode;

	if(!pItem)
		return 0;

	if(pUser->m_sPlayerShop.cItem[bZipCode].GetID()==0)
		memcpy(&pUser->m_sPlayerShop.cItem[bZipCode], pItem, sizeof(CItem));	
	else
	{
		if(pUser->m_sPlayerShop.cItem[bZipCode].GetID()==pItem->GetID())			
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_sPlayerShop.cItem[bZipCode].m_wItemID);

			if(!pBaseItem)
				return 0;

//			int nValue			= pUser->m_sPlayerShop.cItem[bZipCode].m_wItemID/ITEM_DISTRIBUTE;
			int nMaxQuantity	= 1;
			
			nMaxQuantity = pBaseItem->GetMaxQuantity();			

			BYTE byCnt = nMaxQuantity - pUser->m_sPlayerShop.cItem[bZipCode].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = pUser->m_sPlayerShop.cItem[bZipCode].GetQuantity();
				pUser->m_sPlayerShop.cItem[bZipCode].SetQuantity(bQuantity+byPackCnt);
				bAppearChk = TRUE;				
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = pUser->m_sPlayerShop.cItem[bZipCode].GetQuantity();
				pUser->m_sPlayerShop.cItem[bZipCode].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
			}
		}

		if(bAppearChk==FALSE)
		{
			for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
			{
				if(pUser->m_sPlayerShop.cItem[i].GetID()==0)
				{
					memcpy(&pUser->m_sPlayerShop.cItem[i], pItem, sizeof(CItem));
					bAppearChk = TRUE;
					pUser->m_sPlayerShop.cItem[i].SetQuantity(byPackCnt);
					byPackCnt = 0;
					byResult = i;
					break;
				}
				else if(pUser->m_sPlayerShop.cItem[i].GetID()==pItem->GetID())
				{
					CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_sPlayerShop.cItem[i].m_wItemID);

					if(!pBaseItem)
						return 0;

//					int nValue			= pUser->m_sPlayerShop.cItem[i].m_wItemID/ITEM_DISTRIBUTE;
					int nMaxQuantity	= 1;

					nMaxQuantity = pBaseItem->GetMaxQuantity();					

					BYTE byCnt = nMaxQuantity - pUser->m_sPlayerShop.cItem[i].GetQuantity();

					if(byCnt>=byPackCnt)
					{
						BYTE bQuantity = pUser->m_sPlayerShop.cItem[i].GetQuantity();
						pUser->m_sPlayerShop.cItem[i].SetQuantity(bQuantity+byPackCnt);
						bAppearChk = TRUE;
						byResult = i;
						break;
					}
					else if(byCnt!=0)
					{
						BYTE bQuantity = pUser->m_sPlayerShop.cItem[i].GetQuantity();
						pUser->m_sPlayerShop.cItem[i].SetQuantity(bQuantity+byCnt);
						byPackCnt -= byCnt;
					}
				}
			}
			
			if(bAppearChk==FALSE)
			{
				pItem->SetQuantity(byPackCnt);
				Insert_SmallInvItem(pUser, pItem, 0, byPackCnt, ITEM_TYPE_SMALL);
			}
		}			
	}
	return byResult;
}


void Insert_TradeItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BYTE byNew)
{
	BOOL bAppearChk = FALSE;

	if(!pItem)
		return;

//	int nValue = pItem->GetID()/ITEM_DISTRIBUTE;

	if(GetItemType(pItem->GetID())==ITEM_TYPE_LARGE)
		return;

	if(pUser->m_pTradeItem[bZipCode].GetID()==0)
	{
		memcpy(&pUser->m_pTradeItem[bZipCode], pItem, sizeof(CItem));

		if(byNew)
			pUser->m_pTradeItem[bZipCode].SetSerialNum(g_pThis->GetNewSerial());
	}
	else
	{
		if(pUser->m_pTradeItem[bZipCode].GetID()==pItem->GetID())			
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pTradeItem[bZipCode].m_wItemID);

			if(!pBaseItem)
				return;
				
//			int nValue			= pUser->m_pTradeItem[bZipCode].m_wItemID/ITEM_DISTRIBUTE;
			int nMaxQuantity	= 1;

			nMaxQuantity = pBaseItem->GetMaxQuantity();
			
			BYTE byCnt = nMaxQuantity - pUser->m_pTradeItem[bZipCode].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = pUser->m_pTradeItem[bZipCode].GetQuantity();
				pUser->m_pTradeItem[bZipCode].SetQuantity(bQuantity+byPackCnt);
				bAppearChk = TRUE;				
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = pUser->m_pTradeItem[bZipCode].GetQuantity();
				pUser->m_pTradeItem[bZipCode].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
			}
		}

		if(bAppearChk==FALSE)
		{
			for(int i = 0; i < MAX_TRADE_POOL; i++)
			{
				if(pUser->m_pTradeItem[i].GetID()==0)
				{
					memcpy(&pUser->m_pTradeItem[i], pItem, sizeof(CItem));
					bAppearChk = TRUE;
					pUser->m_pTradeItem[i].SetQuantity(byPackCnt);
					byPackCnt = 0;

					if(byNew)
						pUser->m_pTradeItem[i].SetSerialNum(g_pThis->GetNewSerial());

					break;
				}
				else if(pUser->m_pTradeItem[i].GetID()==pItem->GetID())
				{
					CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pTradeItem[i].m_wItemID);

					if(!pBaseItem)
						return;

//					int nValue			= pUser->m_pTradeItem[i].m_wItemID/ITEM_DISTRIBUTE;
					int nMaxQuantity	= 1;

					nMaxQuantity = pBaseItem->GetMaxQuantity();						

					BYTE byCnt = nMaxQuantity - pUser->m_pTradeItem[i].GetQuantity();

					if(byCnt>=byPackCnt)
					{
						BYTE bQuantity = pUser->m_pTradeItem[i].GetQuantity();
						pUser->m_pTradeItem[i].SetQuantity(bQuantity+byPackCnt);
						bAppearChk = TRUE;
						break;
					}
					else if(byCnt!=0)
					{
						BYTE bQuantity = pUser->m_pTradeItem[i].GetQuantity();
						pUser->m_pTradeItem[i].SetQuantity(bQuantity+byCnt);
						byPackCnt -= byCnt;
					}
				}
			}
			
			if(bAppearChk==FALSE)
			{
				pItem->SetQuantity(byPackCnt);
				Insert_SmallInvItem(pUser, pItem, 0, byPackCnt, ITEM_TYPE_SMALL);
			}
		}
	}
}


BYTE Insert_SmallBeltItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bNew)
{
	BOOL bAppearChk = FALSE;

	if(!pItem)
		return 0;

//	int nValue = pItem->GetID()/ITEM_DISTRIBUTE;
		
	if(GetItemType(pItem->GetID())==ITEM_TYPE_LARGE)
		return 0;
	
	if(pUser->m_pBelt[bZipCode].GetID()==0)
	{
		memcpy(&pUser->m_pBelt[bZipCode], pItem, sizeof(CItem));
	}
	else
	{
		if(pUser->m_pBelt[bZipCode].GetID()==pItem->GetID())			
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pBelt[bZipCode].m_wItemID);

			if(!pBaseItem)
				return 0;

//			int nValue			= pUser->m_pBelt[bZipCode].m_wItemID/ITEM_DISTRIBUTE;
			int nMaxQuantity	= 1;

			nMaxQuantity = pBaseItem->GetMaxQuantity();				

			BYTE byCnt = nMaxQuantity - pUser->m_pBelt[bZipCode].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = pUser->m_pBelt[bZipCode].GetQuantity();
				pUser->m_pBelt[bZipCode].SetQuantity(bQuantity+byPackCnt);
				bAppearChk = TRUE;				
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = pUser->m_pBelt[bZipCode].GetQuantity();
				pUser->m_pBelt[bZipCode].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
				pItem->SetQuantity(byPackCnt);
			}
		}

		if(bAppearChk==FALSE)
		{
			for(int i = 0; i < MAX_BELT_POOL; i++)
			{
				if(pUser->m_pBelt[i].GetID()==0)
				{
					memcpy(&pUser->m_pBelt[i], pItem, sizeof(CItem));
					bAppearChk = TRUE;
					pUser->m_pBelt[i].SetQuantity(byPackCnt);
					byPackCnt = 0;
					break;
				}
				else if(pUser->m_pBelt[i].GetID()==pItem->GetID())
				{
					CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pBelt[i].m_wItemID);

					if(!pBaseItem)
						return 0;

//					int nValue			= pUser->m_pBelt[i].m_wItemID/ITEM_DISTRIBUTE;
					int nMaxQuantity	= 1;

					nMaxQuantity = pBaseItem->GetMaxQuantity();						

					BYTE byCnt = nMaxQuantity - pUser->m_pBelt[i].GetQuantity();

					if(byCnt>=byPackCnt)
					{
						BYTE bQuantity = pUser->m_pBelt[i].GetQuantity();
						pUser->m_pBelt[i].SetQuantity(bQuantity+byPackCnt);
						bAppearChk = TRUE;
						break;
					}
					else if(byCnt!=0)
					{
						BYTE bQuantity = pUser->m_pBelt[i].GetQuantity();
						pUser->m_pBelt[i].SetQuantity(bQuantity+byCnt);
						byPackCnt -= byCnt;
					}
				}
			}
			
			if(bAppearChk==FALSE)
			{
				if(bNew)
					return byPackCnt;
				else
				{
					pItem->SetQuantity(byPackCnt);
					Insert_SmallInvItem(pUser, pItem, 0, byPackCnt, ITEM_TYPE_SMALL);					
				}
			}
		}			
	}
	return 0;
}


int Insert_SmallInvItem(CUser* pUser, const CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bNew, BOOL bChk)
{
	BOOL bAppearChk = FALSE;

	if(!pItem)
		return 0;

	if(GetItemType(pItem->GetID())==ITEM_TYPE_LARGE)
		return 0;

	if(pUser->m_pInv_Small[bZipCode].GetID()==0)
	{
		memcpy(&pUser->m_pInv_Small[bZipCode], pItem, sizeof(CItem));		
		byPackCnt = 0;
	}
	else
	{
		if(pUser->m_pInv_Small[bZipCode].GetID()==pItem->GetID())			
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pInv_Small[bZipCode].m_wItemID);

			if(!pBaseItem)
				return 0;

			int nMaxQuantity	= 1;

			nMaxQuantity = pBaseItem->GetMaxQuantity();				

			BYTE byCnt = nMaxQuantity - pUser->m_pInv_Small[bZipCode].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = pUser->m_pInv_Small[bZipCode].GetQuantity();
				pUser->m_pInv_Small[bZipCode].SetQuantity(bQuantity+byPackCnt);
				bAppearChk = TRUE;				
				byPackCnt = 0;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = pUser->m_pInv_Small[bZipCode].GetQuantity();
				pUser->m_pInv_Small[bZipCode].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
			}
		}

		if(bAppearChk==FALSE)
		{
			for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
			{
				if(pUser->m_pInv_Small[i].GetID()==0)
				{
					memcpy(&pUser->m_pInv_Small[i], pItem, sizeof(CItem));						
					pUser->m_pInv_Small[i].SetQuantity(byPackCnt);
					bAppearChk = TRUE;
					byPackCnt = 0;
					break;
				}
				else if(pUser->m_pInv_Small[i].GetID()==pItem->GetID())
				{
					CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pInv_Small[i].m_wItemID);

					if(!pBaseItem)
						return 0;

					int nMaxQuantity	= 1;

					nMaxQuantity = pBaseItem->GetMaxQuantity();						

					BYTE byCnt = nMaxQuantity - pUser->m_pInv_Small[i].GetQuantity();

					if(byCnt>=byPackCnt)
					{
						BYTE bQuantity = pUser->m_pInv_Small[i].GetQuantity();
						pUser->m_pInv_Small[i].SetQuantity(bQuantity+byPackCnt);
						bAppearChk = TRUE;
						byPackCnt = 0;
						break;
					}
					else if(byCnt!=0)
					{
						BYTE bQuantity = pUser->m_pInv_Small[i].GetQuantity();
						pUser->m_pInv_Small[i].SetQuantity(bQuantity+byCnt);
						byPackCnt -= byCnt;
					}
				}
			}
			
			if(bAppearChk==FALSE && bChk==TRUE)
			{
				if(bNew)
				{
					CItem cInsertItem;
					memcpy(&cInsertItem, pItem, sizeof(CItem));
					cInsertItem.SetQuantity(byPackCnt);
					cInsertItem.SetSerialNum(g_pThis->GetNewSerial());		
					
					AppearItem(pUser, &cInsertItem, byPackCnt, 0, ITEM_TYPE_SMALL);
				}
				else
				{
					return byPackCnt;
				}
			}
		}
	}
	
	return byPackCnt;
}


int Insert_SmallBankItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bChk)
{
	BOOL bAppearChk = FALSE;

	if(!pItem)
		return 0;

	if(GetItemType(pItem->GetID())==ITEM_TYPE_LARGE)
		return 0;
	
	if(pUser->m_pBank_Small[bZipCode].GetID()==0)
	{
		memcpy(&pUser->m_pBank_Small[bZipCode], pItem, sizeof(CItem));
		byPackCnt = 0;
	}
	else
	{
		if(pUser->m_pBank_Small[bZipCode].GetID()==pItem->GetID())			
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pBank_Small[bZipCode].m_wItemID);

			if(!pBaseItem)
				return 0;

			int nMaxQuantity	= 1;

			nMaxQuantity = pBaseItem->GetMaxQuantity();
			
			BYTE byCnt = nMaxQuantity - pUser->m_pBank_Small[bZipCode].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = pUser->m_pBank_Small[bZipCode].GetQuantity();
				pUser->m_pBank_Small[bZipCode].SetQuantity(bQuantity+byPackCnt);
				bAppearChk = TRUE;
				byPackCnt = 0;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = pUser->m_pBank_Small[bZipCode].GetQuantity();
				pUser->m_pBank_Small[bZipCode].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
			}
		}

		if(bAppearChk==FALSE)
		{
			for(int i = 0; i < MAX_BANK_SMALL_POOL; i++)
			{
				if(pUser->m_pBank_Small[i].GetID()==0)
				{
					memcpy(&pUser->m_pBank_Small[i], pItem, sizeof(CItem));
					bAppearChk = TRUE;
					pUser->m_pBank_Small[i].SetQuantity(byPackCnt);
					byPackCnt = 0;
					break;
				}
				else if(pUser->m_pBank_Small[i].GetID()==pItem->GetID())
				{
					CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pBank_Small[i].m_wItemID);

					if(!pBaseItem)
						return 0;

					int nMaxQuantity	= 1;

					nMaxQuantity = pBaseItem->GetMaxQuantity();						

					BYTE byCnt = nMaxQuantity - pUser->m_pBank_Small[i].GetQuantity();

					if(byCnt>=byPackCnt)
					{
						BYTE bQuantity = pUser->m_pBank_Small[i].GetQuantity();
						pUser->m_pBank_Small[i].SetQuantity(bQuantity+byPackCnt);
						bAppearChk = TRUE;
						byPackCnt = 0;
						break;
					}
					else if(byCnt!=0)
					{
						BYTE bQuantity = pUser->m_pBank_Small[i].GetQuantity();
						pUser->m_pBank_Small[i].SetQuantity(bQuantity+byCnt);
						byPackCnt -= byCnt;
					}
				}
			}
			
			if(bAppearChk==FALSE)
			{
				pItem->SetQuantity(byPackCnt);
				byPackCnt = Insert_SmallInvItem(pUser, pItem, 0, byPackCnt, bChk);					
			}
		}			
	}
	
	return byPackCnt;
}


void SendToUpdate_GuardianItem(DWORD dwConnectionIndex, CItem* pGuardianItem)
{
	if( IsUltra() )
	{
		DSTC_UPDATE_GUARDIAN_ITEM packet;
		packet.bySlotIndex = 0;
		memcpy(&packet.cItem, pGuardianItem, sizeof(CItem));

		NetSendToUser( dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
	else
	{
		DSTC_ITEM_PICKUP	PickupItem;
		memset(&PickupItem, 0, sizeof(PickupItem));

		PickupItem.bInv		= 82;
		PickupItem.bStatus	= UPDATE_GAME_PLAY;
		PickupItem.bHeader	= Protocol_DSTC::CMD_ITEM_PICKUP;
		PickupItem.bZipCode = 0;
		memcpy(&PickupItem.Item, pGuardianItem, sizeof(CItem));

		NetSendToUser( dwConnectionIndex, (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
}


void Insert_MagicFieldArrayItem(CUser* pUser, CItem* pItem, BYTE byPackCnt)
{	
	if(!pItem)
		return;

	BOOL	bAppearChk	= FALSE;
//	int		nValue		= pItem->GetID()/ITEM_DISTRIBUTE;
	BYTE	byPack		= pItem->GetQuantity()-byPackCnt;
		
	if(0 == pUser->GetCurDungeon()->GetDungeonDataEx()->GetMagicFieldArrayItem()->GetID())
	{		
		if(byPack!=0)
		{
			pItem->SetQuantity(byPack);
			Insert_SmallInvItem(pUser, pItem, 0, byPack, ITEM_TYPE_SMALL);
		}
		pItem->SetQuantity(byPackCnt);
		pUser->GetCurDungeon()->GetDungeonDataEx()->SetMagicFieldArray(pItem);
	}
	else
	{
		if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetID()==pItem->GetID())
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.m_wItemID);

			if(!pBaseItem)
				return;

//			int nValue			= pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.m_wItemID/ITEM_DISTRIBUTE;
			int nMaxQuantity	= 1;
			
			nMaxQuantity = pBaseItem->GetMaxQuantity();			

			BYTE byCnt = nMaxQuantity - pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity();
				pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.SetQuantity(bQuantity+byPackCnt);
				pUser->GetCurDungeon()->GetDungeonDataEx()->SetMagicFieldArray(&pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem);
				byPackCnt = 0;
				bAppearChk = TRUE;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity();
				pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.SetQuantity(bQuantity+byCnt);
				pUser->GetCurDungeon()->GetDungeonDataEx()->SetMagicFieldArray(&pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem);
				byPackCnt -= byCnt;
			}

			if(bAppearChk==FALSE || byPack!=0)
			{
				pItem->SetQuantity(byPackCnt+byPack);
				Insert_SmallInvItem(pUser, pItem, 0, byPackCnt+byPack, ITEM_TYPE_SMALL);
			}
		}
		else
		{
			pItem->SetQuantity(byPackCnt+byPack);
			Insert_SmallInvItem(pUser, pItem, 0, pItem->GetQuantity(), ITEM_TYPE_SMALL);						
		}
	}
}


void Insert_MakingItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{
	if(!pItem)
		return;

	BOOL	bAppearChk	= FALSE;
//	int		nValue		= pItem->GetID()/ITEM_DISTRIBUTE;
	BYTE	byPack		= pItem->GetQuantity()-byPackCnt;
		
	if(pUser->m_pMakingItem[bZipCode].GetID()==0)
	{
		if(byPack!=0)
		{
			pItem->SetQuantity(pItem->GetQuantity()-1);
			Insert_SmallInvItem(pUser, pItem, 0, byPack, ITEM_TYPE_SMALL);
		}
		memcpy(&pUser->m_pMakingItem[bZipCode], pItem, sizeof(CItem));
		pUser->m_pMakingItem[bZipCode].SetQuantity(byPackCnt);
	}
	else
	{
		for(int i = 1; i < 4; i++)
		{
			if(pUser->m_pMakingItem[bZipCode].GetID()==0)
			{
				memcpy(&pUser->m_pMakingItem[bZipCode], pItem, sizeof(CItem));
				pUser->m_pMakingItem[bZipCode].SetQuantity(byPackCnt);
				bAppearChk = TRUE;
				break;
			}
		}

		if(byPack!=0 || !bAppearChk)
			Insert_SmallInvItem(pUser, pItem, 0, byPack+byPackCnt, ITEM_TYPE_SMALL);
	}
}

void Insert_UpgradeItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{	
	if(!pItem)
		return;

	BOOL	bAppearChk	= FALSE;
//	int		nValue		= pItem->GetID()/ITEM_DISTRIBUTE;
	BYTE	byPack		= pItem->GetQuantity()-byPackCnt;
		
	if(pUser->m_pUpgradeItem[bZipCode].GetID()==0)
	{
		if(byPack!=0)
		{
			pItem->SetQuantity(pItem->GetQuantity()-1);
			Insert_SmallInvItem(pUser, pItem, 0, byPack, ITEM_TYPE_SMALL);
		}
		memcpy(&pUser->m_pUpgradeItem[bZipCode], pItem, sizeof(CItem));
		pUser->m_pUpgradeItem[bZipCode].SetQuantity(byPackCnt);
	}
	else
	{
		for(int i = 1; i < 4; i++)
		{
			if(pUser->m_pUpgradeItem[bZipCode].GetID()==0)
			{
				memcpy(&pUser->m_pUpgradeItem[bZipCode], pItem, sizeof(CItem));
				pUser->m_pUpgradeItem[bZipCode].SetQuantity(byPackCnt);
				bAppearChk = TRUE;
				break;
			}
		}

		if(byPack!=0 || !bAppearChk)
			Insert_SmallInvItem(pUser, pItem, 0, byPack+byPackCnt, ITEM_TYPE_SMALL);
	}
}

int Insert_SmallInvItem2(CItem* pInv_Small, CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bChk)
{
	BOOL bAppearChk = FALSE;

	if(!pItem)
		return 1;

//	int nValue = pItem->GetID()/ITEM_DISTRIBUTE;
		
	if(pInv_Small[bZipCode].GetID()==0)
	{
		memcpy(&pInv_Small[bZipCode], pItem, sizeof(CItem));
		byPackCnt = 0;
	}
	else
	{
		if(pInv_Small[bZipCode].GetID() == pItem->GetID())			
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pInv_Small[bZipCode].m_wItemID);

			if(!pBaseItem)
				return 1;

		//	int nValue			= pInv_Small[bZipCode].m_wItemID/ITEM_DISTRIBUTE;
			int nMaxQuantity	= 1;

			nMaxQuantity = pBaseItem->GetMaxQuantity();				

			BYTE byCnt = nMaxQuantity - pInv_Small[bZipCode].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = pInv_Small[bZipCode].GetQuantity();
				pInv_Small[bZipCode].SetQuantity(bQuantity+byPackCnt);
				bAppearChk = TRUE;				
				byPackCnt = 0;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = pInv_Small[bZipCode].GetQuantity();
				pInv_Small[bZipCode].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
			}
		}

		if(bAppearChk==FALSE)
		{
			for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
			{
				if(pInv_Small[i].GetID()==0)
				{
					memcpy(&pInv_Small[i], pItem, sizeof(CItem));						
					pInv_Small[i].SetQuantity(byPackCnt);
					bAppearChk = TRUE;
					byPackCnt = 0;
					break;
				}
				else if(pInv_Small[i].GetID()==pItem->GetID())
				{
					CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pInv_Small[i].m_wItemID);

					if(!pBaseItem)
						return 1;

				//	int nValue			= pInv_Small[i].m_wItemID/ITEM_DISTRIBUTE;
					int nMaxQuantity	= 1;

					nMaxQuantity = pBaseItem->GetMaxQuantity();						

					BYTE byCnt = nMaxQuantity - pInv_Small[i].GetQuantity();

					if(byCnt>=byPackCnt)
					{
						BYTE bQuantity = pInv_Small[i].GetQuantity();
						pInv_Small[i].SetQuantity(bQuantity+byPackCnt);
						bAppearChk = TRUE;
						byPackCnt = 0;
						break;
					}
					else if(byCnt!=0)
					{
						BYTE bQuantity = pInv_Small[i].GetQuantity();
						pInv_Small[i].SetQuantity(bQuantity+byCnt);
						byPackCnt -= byCnt;
					}
				}
			}			
		}
	}
	
	return byPackCnt;
}

void PlayerShopPriceLog(CUser* pUser, CItem* pItem, BYTE byInv, BYTE byIndex, DWORD dwPrice, DWORD dwCnt, BOOL bReturn, 
						DWORD dwOwnUserIndex, DWORD dwOwnPrice, BYTE byOwnZipcode, BYTE byOwnZipcode2)
{
	if(!pUser)
		return;
	
	DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);
	
	if(!pBinding)
	{
		Log(LOG_IMPORTANT, "ParameterPool is Full!!  in PlayerShopPriceLog Function()!");
		return;
	}

	pBinding[0].obValue		= 0;
	pBinding[0].cbMaxLen	= sizeof(CItem);
	pBinding[0].wType		= DBTYPE_BYTES;

	char szQuery[0xff]={0,};
	memset(szQuery, 0, sizeof(szQuery));

	PLAYERSHOP_LOG* pPlayerShopLog = (PLAYERSHOP_LOG*)LALAlloc(g_pPlayerShopLogPool);

	if(!pPlayerShopLog)
		__asm int 3

	memcpy(&pPlayerShopLog->cItem, pItem, sizeof(CItem));	
	pPlayerShopLog->dwUserIndex		= pUser->GetID();
	pPlayerShopLog->dwKey			= g_dwPlayerShopKey;
	pPlayerShopLog->dwPrice			= dwPrice;
	pPlayerShopLog->byZipCode		= byIndex;	
	pPlayerShopLog->bInv			= byInv;
	pPlayerShopLog->dwCnt			= dwCnt;
	pPlayerShopLog->dwOwnUserIndex	= dwOwnUserIndex;
	pPlayerShopLog->dwOwnPrice		= dwOwnPrice;
	pPlayerShopLog->byOwnZipCode	= byOwnZipcode;
	pPlayerShopLog->byOwnZipCode2	= byOwnZipcode2;
	g_pPlayerShopLogHash->Insert(pPlayerShopLog, g_dwPlayerShopKey);

	wsprintf(szQuery, "Update PlayerShop Set Item%u = ? where Char_Index = %u", byIndex+1, pUser->GetID());
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, pItem, sizeof(CItem), 1, bReturn, QUERY_TYPE_USER_PLAYERSHOP, pPlayerShopLog, GAME_DB);

	g_dwPlayerShopKey += 1;
}

void PlayerShopLog(CUser* pUser, CItem* pItem, BYTE byInv, BYTE byIndex, DWORD dwPrice, DWORD dwCnt, BOOL bReturn)
{
	if(!pUser)
		return;
	
	DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);

	if(!pBinding)
	{
		Log(LOG_IMPORTANT, "ParameterPool is Full!!  in PlayerShopLog Function()!");
		return;
	}

	pBinding[0].obValue		= 0;
	pBinding[0].cbMaxLen	= sizeof(CItem);
	pBinding[0].wType		= DBTYPE_BYTES;

	char szQuery[0xff]={0,};
	memset(szQuery, 0, sizeof(szQuery));

	PLAYERSHOP_LOG* pPlayerShopLog = (PLAYERSHOP_LOG*)LALAlloc(g_pPlayerShopLogPool);

	if(!pPlayerShopLog)
		__asm int 3

	memcpy(&pPlayerShopLog->cItem, pItem, sizeof(CItem));	
	pPlayerShopLog->dwUserIndex	= pUser->GetID();
	pPlayerShopLog->dwKey		= g_dwPlayerShopKey;
	pPlayerShopLog->dwPrice		= dwPrice;
	pPlayerShopLog->byZipCode	= byIndex;	
	pPlayerShopLog->bInv		= byInv;
	pPlayerShopLog->dwCnt		= dwCnt;
	g_pPlayerShopLogHash->Insert(pPlayerShopLog, g_dwPlayerShopKey);

	wsprintf(szQuery, "CZP_PLAYERSHOP_LOG%u %u, ?", byIndex+1, pUser->GetID());
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, pItem, sizeof(CItem), 1, bReturn, QUERY_TYPE_USER_PLAYERSHOP, pPlayerShopLog, GAME_DB);

	g_dwPlayerShopKey += 1;
}

void PlayerShopLogAll(CUser* pUser)
{
	if(!pUser)
		return;
	
	DBBINDING* pBinding = g_pDb->THCreateParamInfo(MAX_PLAYER_SHOP_INV);

	if(!pBinding)
	{
		Log(LOG_IMPORTANT, "ParameterPool is Full!!  in PlayerShopLogAll Function()!");
		return;
	}

	for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
	{
		pBinding[i].obValue		= sizeof(CItem)*i;
		pBinding[i].cbMaxLen	= sizeof(CItem);
		pBinding[i].wType		= DBTYPE_BYTES;
	}

	char szQuery[0xff]={0,};
	memset(szQuery, 0, sizeof(szQuery));	
	wsprintf(szQuery, "CZP_PLAYERSHOP_ALLLOG %u, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?", pUser->GetID());
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, pUser->m_sPlayerShop.cItem, sizeof(CItem)*MAX_PLAYER_SHOP_INV, MAX_PLAYER_SHOP_INV, FALSE, NULL, NULL, GAME_DB);	
}

void SendCopyItemPacket(CUser* pUser, CItem* pInsertItem, int nInvType)
{
	DSTC_COPY_ITEM_INSERTED	packet;
	packet.dwCopyItemID = pInsertItem->GetID();
	packet.dwInvType = (DWORD)nInvType;

	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	Log(LOG_IMPORTANT, "Copy Item Inserted! (Name:%s, ItemID:%d, nInvType:%d)", pUser->m_szName, pInsertItem->GetID(), nInvType );
}

BOOL IsCopyItem(CUser* pCheckUser, CItem* pInsertItem, int nInvType)
{
	if(!pInsertItem)
		return FALSE;

	int i = 0;

	switch(nInvType)
	{
	case COPY_CHECK_INV_SMALL:
		{
			for(i=0; i<MAX_INV_SMALL_POOL; i++)
			{
				if(pCheckUser->m_pInv_Small[i].GetID() == 0)	continue;
				if(pCheckUser->m_pInv_Small[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}
		}
		break;

	case COPY_CHECK_INV_LARGE:
		{
			for(i=0; i<MAX_INV_LARGE_POOL; i++)
			{
				if(pCheckUser->m_pInv_Large[i].GetID() == 0)	continue;
				if(pCheckUser->m_pInv_Large[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}
		}
		break;

	case COPY_CHECK_INV_ALL:
		{
			
			for(i=0; i<MAX_INV_SMALL_POOL; i++)
			{
				if(pCheckUser->m_pInv_Small[i].GetID() == 0)	continue;
				if(pCheckUser->m_pInv_Small[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

			for(i=0; i<MAX_INV_LARGE_POOL; i++)
			{
				if(pCheckUser->m_pInv_Large[i].GetID() == 0)	continue;
				if(pCheckUser->m_pInv_Large[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}
		}
		break;

	case COPY_CHECK_BANK_SMALL:
		{
			for(i=0; i<MAX_BANK_SMALL_POOL; i++)
			{
				if(pCheckUser->m_pBank_Small[i].GetID() == 0)	continue;
				if(pCheckUser->m_pBank_Small[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}
		}
		break;

	case COPY_CHECK_BANK_LARGE:
		{
			for(i=0; i<MAX_BANK_LARGE_POOL; i++)
			{
				if(pCheckUser->m_pBank_Large[i].GetID() == 0)	continue;
				if(pCheckUser->m_pBank_Large[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

		}
		break;

	case COPY_CHECK_BANK_ALL:
		{
			for(i=0; i<MAX_BANK_SMALL_POOL; i++)
			{
				if(pCheckUser->m_pBank_Small[i].GetID() == 0)	continue;
				if(pCheckUser->m_pBank_Small[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

			for(i=0; i<MAX_BANK_LARGE_POOL; i++)
			{
				if(pCheckUser->m_pBank_Large[i].GetID() == 0)	continue;
				if(pCheckUser->m_pBank_Large[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

		}
		break;

	case COPY_CHECK_EQUIP:
		{
			for(i=0; i<MAX_EQUIP_POOL; i++)
			{
				if(pCheckUser->m_pEquip[i].GetID() == 0)	continue;
				if(pCheckUser->m_pEquip[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}
		}
		break;

	case COPY_CHECK_BELT:
		{
			for(i=0; i<MAX_BELT_POOL; i++)
			{
				if(pCheckUser->m_pBelt[i].GetID() == 0)		continue;
				if(pCheckUser->m_pBelt[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}
		}
		break;

	case COPY_CHECK_ALL:
		{
			for(i=0; i<MAX_INV_SMALL_POOL; i++)
			{
				if(pCheckUser->m_pInv_Small[i].GetID() == 0)	continue;
				if(pCheckUser->m_pInv_Small[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

			for(i=0; i<MAX_INV_LARGE_POOL; i++)
			{
				if(pCheckUser->m_pInv_Large[i].GetID() == 0)	continue;
				if(pCheckUser->m_pInv_Large[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

			for(i=0; i<MAX_BANK_SMALL_POOL; i++)
			{
				if(pCheckUser->m_pBank_Small[i].GetID() == 0)	continue;
				if(pCheckUser->m_pBank_Small[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

			for(i=0; i<MAX_BANK_LARGE_POOL; i++)
			{
				if(pCheckUser->m_pBank_Large[i].GetID() == 0)	continue;
				if(pCheckUser->m_pBank_Large[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

			for(i=0; i<MAX_EQUIP_POOL; i++)
			{
				if(pCheckUser->m_pEquip[i].GetID() == 0)	continue;
				if(pCheckUser->m_pEquip[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}

			for(i=0; i<MAX_BELT_POOL; i++)
			{
				if(pCheckUser->m_pBelt[i].GetID() == 0)		continue;
				if(pCheckUser->m_pBelt[i].GetSerial()->i64Serial == pInsertItem->GetSerial()->i64Serial)
				{
					SendCopyItemPacket(pCheckUser, pInsertItem, nInvType);
					return TRUE;
				}
			}
		}
		break;
	}

	return FALSE;
}

void CoypPickupMsg(DSTC_ITEM_PICKUP* pPickupItem, CItem* pItem, BYTE byInv, BYTE byZipCode)
{
	pPickupItem->bInv		= byInv;
	pPickupItem->bZipCode	= byZipCode;
	memcpy(&pPickupItem->Item, pItem, sizeof(CItem));
}

BOOL IsCopyItem(CUser* pUser, DSTC_ITEM_PICKUP* pPickupItem, CItem* pItem, BYTE byInv, BYTE byType, BOOL bChk)
{
	if(IsCopyItem(pUser, pItem, byType)==TRUE)
	{
		pPickupItem->bInv		= byInv;
		pPickupItem->bZipCode	= 255;
		memset(&pPickupItem->Item, 0, sizeof(CItem));

		if(bChk)
			memset(pItem, 0, sizeof(CItem));

		return TRUE;
	}
	return FALSE;
}

BOOL SetIsCopyItem(CUser* pUser, DSTC_ITEM_PICKUP* pPickupItem, CItem* pItem, BYTE byInv, BYTE byType, BOOL bChk)
{
	switch(byType)
	{
	case DEF_COPYITEM_TYPE_LARGE:
		{
			if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
			{
				if(IsCopyItem(pUser, pPickupItem, pItem, byInv, COPY_CHECK_BANK_LARGE))
					return TRUE;
			}
			if(IsCopyItem(pUser, pPickupItem, pItem, byInv, COPY_CHECK_INV_LARGE))
				return TRUE;
			if(IsCopyItem(pUser, pPickupItem, pItem, byInv, COPY_CHECK_EQUIP))
				return TRUE;
		}
		break;
	case DEF_COPYITEM_TYPE_SMALL:
		{
			if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
			{
				if(IsCopyItem(pUser, pPickupItem, pItem, byInv, COPY_CHECK_BANK_SMALL))
					return TRUE;
			}
			if(IsCopyItem(pUser, pPickupItem, pItem, byInv, COPY_CHECK_INV_SMALL))
				return TRUE;
			if(IsCopyItem(pUser, pPickupItem, pItem, byInv, COPY_CHECK_BELT))
				return TRUE;
		}		
		break;
	}	
	return FALSE;
}			


void ItemHandMod(CUser*	pUser, DWORD dwEqCode1, DWORD dwEqCode2)
{
	DSTC_WEAPON_HAND	pPacket;
	CDungeonLayer*		pLayer;

	if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)
		return;
	else if(!pUser->GetCurDungeon())	return;

	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	pPacket.dwHand		= dwEqCode1;
	pPacket.dwHand2		= dwEqCode2;
	pPacket.dwUserIndex	= pUser->GetID();
	pLayer->BroadCastSectionMsg((char*)&pPacket, pPacket.GetPacketSize(), pUser->GetPrevSectionNum());
}

void ItemArmorMod(CUser* pUser, DWORD dwEqCode)
{
	DSTC_ARMOR_ITEM		pPacket;
	CDungeonLayer*		pLayer;

	if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)
		return;
	else if(!pUser->GetCurDungeon())	return;

	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	pPacket.dwArmor		= dwEqCode;
	pPacket.dwUserIndex	= pUser->GetID();
	pLayer->BroadCastSectionMsg((char*)&pPacket, pPacket.GetPacketSize(), pUser->GetPrevSectionNum());
}

void ItemHelmetMod(CUser* pUser, DWORD dwEqCode)
{
	DSTC_HELMET_ITEM	pPacket;
	CDungeonLayer*		pLayer;

	if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)
		return;

	else if(!pUser->GetCurDungeon())	return;
	
	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	pPacket.dwHelmet	= dwEqCode;
	pPacket.dwUserIndex	= pUser->GetID();
	pLayer->BroadCastSectionMsg((char*)&pPacket, pPacket.GetPacketSize(), pUser->GetPrevSectionNum());
}


BOOL CheckItem(CItem* pItem, ITEM_TYPE eType)
{	
	int nValue = pItem->GetID()/ITEM_DISTRIBUTE;

	if(pItem->GetID()==0)
		return FALSE;

	if(nValue<0)
		return FALSE;
		
	if(GetItemType(pItem->GetID())!=eType)
		return FALSE;

	return TRUE;
}


int GetItemIndex(CItem* pItem)
{
	return pItem->GetID() / ITEM_DISTRIBUTE;
}


BOOL SetPositionPotalItem(CItem* pItem, BYTE byLayerIndex, WORD wMapID, WORD wPosX, WORD wPosZ)
{
	pItem->m_Item_Zodiac.bType = 1;
	pItem->m_Item_Zodiac.bLayer	= byLayerIndex;
	pItem->m_Item_Zodiac.wMapId	= wMapID;
	pItem->m_Item_Zodiac.wPosX	= wPosX;
	pItem->m_Item_Zodiac.wPosZ	= wPosZ;
	
	return TRUE;
}


BOOL CheckShop(CUser* pUser)
{
	ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pUser->GetID());

	if(pItemShop)
	{
		if(pItemShop->byType == 1)
		{
			return TRUE;
		}
	}

	return FALSE;
}


void SetPortal(CItem* pItem)
{
	BYTE byIndex = pItem->GetID() - __ITEM_PORTAL_VILL1__;

	WORD wPortalPos[12] = {5533, 2807, 8922, 2697, 1057, 4440, 3500, 3300, 3300, 3100, 3300, 3100};
//	WORD wPortalPos[12] = {11300, 4800, 8300, 12000, 1057, 4440, 3500, 3300, 3300, 3100, 3300, 3100};//옛날 좌표


	// 마을 귀환 스크롤 추가 김영대 2004.02.14
	WORD wItemID = pItem->GetID();
	if (wItemID >= __ITEM_PORTAL_VILL1__  &&  wItemID <= __ITEM_PORTAL_VILL6__ )
	//if(pItem->GetID()>=__ITEM_PORTAL_VILL1__ && pItem->GetID()<=__ITEM_PORTAL_VILL5__)
	{
		pItem->m_Item_Zodiac.wPosX	= wPortalPos[byIndex*2];
		pItem->m_Item_Zodiac.wPosZ	= wPortalPos[byIndex*2+1];
	}
}


BOOL CheckStore(DWORD dwKey)
{
	ITEM_STORE*	pItemStore = g_pItemStoreHash->GetData(dwKey);

	if(!pItemStore)
		return FALSE;

	return TRUE;
}


BOOL CheckTrade(__int64 nKey)
{
	ITEM_TRADE* pItemTrade = g_pItemTradeHash->GetData(nKey);

	if(!pItemTrade)
		return FALSE;

	return TRUE;
}

void CheckSpeed(CUser* pUser)
{
	BYTE bySpeedIndex		= 0;
	BYTE bySpeedAftIndex	= 0;
	WORD wPerWeight			= pUser->GetAverageWeight();
	
	if(!pUser->GetCurDungeon())	return;	
	CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
	if(!pLayer)					return;		

	if(pUser->m_wPerWeight >= WEIGTH_100PER_OVER)
	{
		bySpeedIndex = 2;
	}
	else if(pUser->m_wPerWeight >= WEIGTH_80PER_OVER)
	{
		bySpeedIndex = 1;	
	}

	pUser->m_wPerWeight	= wPerWeight;		
	
	if(pUser->m_wPerWeight >= WEIGTH_100PER_OVER)
	{
		bySpeedAftIndex = 2;
	}
	else if(pUser->m_wPerWeight >= WEIGTH_80PER_OVER)
	{
		bySpeedAftIndex = 1;	
	}

	if(bySpeedAftIndex != bySpeedIndex)
	{
		float				fUserSpeed = 0;
		DSTC_USER_SPEED		pSpeedPacket;		

		if(bySpeedAftIndex >= 1)
		{
			fUserSpeed = pUser->GetMoveSpeed() / 3;
		}

		pSpeedPacket.dwUserIndex	= pUser->GetID();
		pSpeedPacket.fSpeed			= (pUser->GetMoveSpeed() - fUserSpeed) / 1000.f;		
		pLayer->BroadCastSectionMsg((char*)&pSpeedPacket, pSpeedPacket.GetPacketSize(), pUser->GetPrevSectionNum());
	}
}


CBaseItem* GetBaseItem(WORD wId)
{
	CBaseItem* pItem = g_pBaseItemHash->GetData(wId);			
	return pItem;	
}


void ConvertItem(CItem* pItem)
{
	WORD wOldID = pItem->GetID();
	WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
	pItem->SetID(wNewID);
}


void CheckItemSlot(CUser* pUser)
{
	if(g_pThis->GetServerType() != SERVER_TYPE_VILLAGE)
		return;
	
	BOOL bChk = FALSE;

	for(int i = 1; i < 4; i++)
	{
		if(pUser->m_pUpgradeItem[i].GetID()!=0)
		{
			if(GetItemType(pUser->m_pUpgradeItem[i].GetID())==ITEM_TYPE_SMALL)
			{
				Insert_SmallInvItem(pUser
					, &pUser->m_pUpgradeItem[i]
					, 0
					, pUser->m_pUpgradeItem[i].GetQuantity()
					, TRUE);
			}
		}
	}

	if(pUser->m_pUpgradeItem[0].GetID()!=0)
	{
		BOOL bChk = FALSE;

		for(i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(pUser->m_pInv_Large[i].GetID()==0 
				&& bChk==FALSE 
				&& GetItemType(pUser->m_pUpgradeItem[0].GetID())==ITEM_TYPE_LARGE)
			{
				memcpy(&pUser->m_pInv_Large[i], &pUser->m_pUpgradeItem[0], sizeof(CItem));
				bChk = TRUE;
			}									
		}
	}		
		
	for(i = 0; i < 4; i++)
	{
		memset(&pUser->m_pUpgradeItem[i], 0, sizeof(pUser->m_pUpgradeItem[i]));
	}
	
	for( i = 0; i < MAX_PLAYER_SHOP_INV; i++)
	{
		if(pUser->m_sPlayerShop.cItem[i].GetID()!=0)
		{
			if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_SMALL)					
			{
				int nCnt = Insert_SmallInvItem(pUser
								, &pUser->m_sPlayerShop.cItem[i]
								, 0
								, pUser->m_sPlayerShop.cItem[i].GetQuantity()
								, TRUE
								, FALSE);
				
				if(nCnt==0)
				{
					memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
					bChk = TRUE;
				}
				else
				{
					pUser->m_sPlayerShop.cItem[i].SetQuantity(nCnt);
					bChk = TRUE;
				}
			}
			else
			{
				for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
				{
					if(pUser->m_pInv_Large[j].GetID()==0)
					{
						memcpy(&pUser->m_pInv_Large[j], &pUser->m_sPlayerShop.cItem[i], sizeof(CItem));
						memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
						bChk = TRUE;
						break;											
					}									
				}
			}
		}

		pUser->m_sPlayerShop.dwCustomSellPrice[i] = 0;
	}

	for(i = 0; i < MAX_PLAYER_SHOP_INV; i++)
	{
		if(pUser->m_sPlayerShop.cItem[i].GetID()!=0)
		{
			if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_SMALL)
			{
				int nCnt = Insert_SmallBankItem(pUser
					, &pUser->m_sPlayerShop.cItem[i]
					, 0
					, pUser->m_sPlayerShop.cItem[i].GetQuantity()
					, FALSE);															

				if(nCnt==0)
				{
					memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
					bChk = TRUE;
				}
				else
				{
					pUser->m_sPlayerShop.cItem[i].SetQuantity(nCnt);
					bChk = TRUE;
				}
			}
			else if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_LARGE)
			{
				for(int j = 0; j < MAX_BANK_LARGE_POOL; j++)
				{
					if(pUser->m_pBank_Large[j].GetID()==0)
					{
						memcpy(&pUser->m_pBank_Large[j], &pUser->m_sPlayerShop.cItem[i], sizeof(CItem));									
						memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
						bChk = TRUE;
						break;
					}
				}
			}
		}																
	}
	
	if(bChk)
	{
		PlayerShopLogAll(pUser);
	}
}


BOOL CheckEquip(BYTE byValue, BYTE byEquipCode)
{	
#ifdef CHECK_EQUIP
	switch(byEquipCode)
	{
		case EQUIP_TYPE_RIDE:
			if(byValue==20)
				return TRUE;
		case EQUIP_TYPE_HELMET:
			if(byValue==10)
				return TRUE;
		case EQUIP_TYPE_MAIL:
			if(byValue==11)
				return TRUE;
		case EQUIP_TYPE_AMULE2:
			if(byValue==17)
				return TRUE;
		case EQUIP_TYPE_RRING1:
		case EQUIP_TYPE_RRING2:
		case EQUIP_TYPE_RRING3:
			if(byValue==16)
				return TRUE;
		case EQUIP_TYPE_BOOTS:
			if(byValue==15)
				return TRUE;
		case EQUIP_TYPE_BELT:
			if(byValue==13)
				return TRUE;
		case EQUIP_TYPE_RGLOVE:
		case EQUIP_TYPE_LGLOVE:
			if(byValue==14)
				return TRUE;
		case EQUIP_TYPE_BAG:
			if(byValue==18)
				return TRUE;
	}
	return FALSE;
#else
	return TRUE;
#endif
}





