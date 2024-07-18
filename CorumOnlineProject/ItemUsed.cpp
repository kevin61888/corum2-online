#include "../CommonServer/CommonHeader.h"
#include "ItemUsed.h"
#include "ItemTradeShopWnd.h"
#include "StoreWnd.h"
#include "DSMiniMap.h"
#include "OkWnd.h"
#include "CDungeonInfoWnd.h"
#include "Chat.h"
#include "InitGame.h"
#include "message.h"
#include "User.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "DungeonTable.h"
#include "GuildWnd.h"
#include "GroupWnd.h"
#include "InitItemWnd.h"
#include "GameDefine.h"
#include "ItemNative.h"


DWORD g_dwMagicArrayTick = 0;


BOOL ItemUsedZodianInsurance(CItem* pItem, BYTE byZipcode, BYTE byType)
{
	int	nValue	= pItem->GetID() / ITEM_DISTRIBUTE;

	if(nValue==ITEM_ZODIAC_INDEX)
	{						
		// 조디악카드(천칭) : 아이템 보험 
		if(pItem->GetID() == __ITEM_INSURANCE_INDEX__)
		{
			if(!g_pMainPlayer->m_bMatching)
			{
				if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PORTAL_MOVING)
				{
					if(pItem->m_Item_Zodiac.bInvIndex>=1)
					{
						// "이미 사용중 입니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE848].szMessage, 0xffff2cff); 
						return TRUE;
					}

					for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Small[i].GetID()==pItem->GetID())
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_Item_Zodiac.bInvIndex >= 1)
							{
								// "이미 사용중 입니다."
								DisplayMessageAdd(g_Message[ETC_MESSAGE848].szMessage, 0xffff2cff); 
								return TRUE;
							}
						}										
					}

					CTDS_ITEM_PICKUP pPacket;
					pPacket.bSectionNum	= 1;
					pPacket.i64ItemID	= 0;
					SetItemPacket(&pPacket, 100, byZipcode, 0, 0, 0);
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
					
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL,byZipcode,0xff);
					}

					g_bRButton	= FALSE;
					return TRUE;	
				}								
			}
		}		
	}

	return FALSE;
}


BOOL ItemUsedZodiac(CItem* pItem, BYTE byZipcode, BYTE byType)
{	
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	CStoreWnd*			pStoreWnd			= CStoreWnd::GetInstance();
	CMiniMapWnd*		pMiniMapWnd			= CMiniMapWnd::GetInstance();
	CDungeonInfoWnd*	pDungeonInfoWnd		= CDungeonInfoWnd::GetInstance();
	COkWnd*				pOkWnd				= COkWnd::GetInstance();

	BOOL				bPortalChk			= FALSE;
	BOOL				bOnwerChk			= FALSE;

	if(pItem->GetID() == __ITEM_INSURANCE_INDEX__)
	{
		// 조디악카드(천칭) : 아이템 보험 
		return FALSE;
	}


	int nValue = pItem->GetID() / ITEM_DISTRIBUTE;

	if(nValue == ITEM_ZODIAC_INDEX)
	{
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			
		}
		else
		{
			if(g_pMainPlayer->m_MouseItem.GetID()!=0)
			{
				// "아이템 픽업중에는 포탈을 할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE1254].szMessage, 0xffff0000);
				return TRUE;
			}
		}
		
		if(pStoreWnd->GetActive())
		{
			// "상점이용 중에는 다른 행동을 하실 수 없습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1007].szMessage, 0xffff2cff); 
			return TRUE;
		}

		if(pItemTradeShopWnd->GetActive())
		{
			// "교환중에는 사용할수 없습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE179].szMessage, 0xffff2cff); 
			return TRUE;
		}
		
		if(TRUE == g_pMainPlayer->m_bInEventDungeon)
		{
			// "이벤트 던전내에서 조디악 카드 사용은 금지되어 있습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE500].szMessage, 0xffff2cff); 
			return TRUE;
		}

		if(pItem->m_wItemID==__ITEM_PORTAL_INDEX__)							
		{			
			if(!g_pMainPlayer->m_bMatching)
			{
				// 포탈에 좌표 입력할때 //
				if(pItem->m_Item_Zodiac.bType==0)
				{		
					
					if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PORTAL_MOVING)
					{
						if(!g_pThisDungeon->IsDungeonOwner(g_pMainPlayer))
						{
#if IS_TAIWAN_LOCALIZING()								
							if(g_pThisDungeon->m_dwID > 10)
#else
							if(g_pThisDungeon->m_dwID >= 2000 && g_pThisDungeon->m_dwID < 4000)
#endif
							{
								if(pMiniMapWnd->m_byMapLayer==0)
								{
									bPortalChk = TRUE;
								}
								else
								{
									//"이곳은 조디악카드에 위치를 기억시킬 수 없습니다."
									DisplayMessageAdd(g_Message[ETC_MESSAGE987].szMessage, 0xffff0000);
								}
							}
							else
							{
								// "이곳은 조디악카드에 위치를 기억시킬 수 없습니다."
								DisplayMessageAdd(g_Message[ETC_MESSAGE987].szMessage, 0xffff0000);
							}
						}
						else
						{
							bPortalChk = TRUE;
						}

						if(bPortalChk)
						{				
							if(byType==ZODIAC_USE_TYPE_INVENTORY)
							{
								pOkWnd->SetActive(TRUE);
								pOkWnd->m_bZipCode	= byZipcode;	
								pOkWnd->m_bType		= __OKWND_TYPE_POTAL_SAVE;
							}	
							else if(byType==ZODIAC_USE_TYPE_BELT)
							{
								CTDS_ITEM_PICKUP pPacket;
								pPacket.bSectionNum	= 1;
								pPacket.i64ItemID	= 0;
								SetItemPacket(&pPacket, 55, byZipcode, 0, 0, 0);
								g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
								
								if(g_ItemMoveManager.GetNewItemMoveMode())
								{
									g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_BELT,byZipcode,0xff);
								}
							}
						}

						return TRUE;
					}															
					else
					{
						// "포탈 이동 중에는 사용할 수 없습니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE502].szMessage, 0xFFFF2CFF); 
					}
				}
				else if(pItem->m_Item_Zodiac.bType==1)
				{										
					if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PORTAL_MOVING)
					{
						POSITION_ pos = pDungeonInfoWnd->m_pPageList->GetHeadPosition();

						while(pos)
						{
							DUNGEON_DATA_EX* pDungeon = 
								(DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);

							if(pDungeon)
							{
								if(pDungeon->IsDungeonOwner(g_pMainPlayer))
								{													
									if(pItem->m_Item_Zodiac.wMapId==g_pThisDungeon->m_dwID)
									{
										if(pItem->m_Item_Zodiac.bLayer==pMiniMapWnd->m_byMapLayer)
										{
											// "같은 지역에서는 이동을 할수가 없습니다."
											DisplayMessageAdd(g_Message[ETC_MESSAGE503].szMessage, 0xFFFF2CFF); 
											break;
										}															
									}

									bOnwerChk = TRUE;
									break;
								}
							}
						}

						if(!bOnwerChk)
						{
#if IS_TAIWAN_LOCALIZING()	
							if (pItem->m_Item_Zodiac.wMapId > 10)
#else
							if (pItem->m_Item_Zodiac.wMapId>=2000 && pItem->m_Item_Zodiac.wMapId<4000)
#endif
							{
								if(pItem->m_Item_Zodiac.bLayer==0)
								{
									bPortalChk = TRUE;
								}
								else
								{
									//"저장된 지역으로는 현재 이동이 불가능합니다."
									DisplayMessageAdd(g_Message[ETC_MESSAGE988].szMessage, 0xffff0000);
								}
							}
							else
							{
								// "저장된 지역으로는 현재 이동이 불가능합니다."
								DisplayMessageAdd(g_Message[ETC_MESSAGE988].szMessage, 0xffff0000);
							}
						}
						else
						{
							bPortalChk = TRUE;						
						}

						if(bPortalChk)
						{
							if(byType==ZODIAC_USE_TYPE_INVENTORY)
							{
								pOkWnd->m_dwDungeonID	= pItem->m_Item_Zodiac.wMapId;							
								pOkWnd->m_bZipCode		= byZipcode;

								if(	g_pThisDungeon->m_dwID != pItem->m_Item_Zodiac.wMapId	&&
									pItem->m_Item_Zodiac.wMapId > 4000						&&
									pItem->m_Item_Zodiac.wMapId < 5000)
								{
									pOkWnd->m_bType	= __OKWND_TYPE_POTAL_DUNGEON_USE| __OKWND_TYPE_POTAL_ENTRANCE;
								}
								else
								{
									pOkWnd->m_bType	= __OKWND_TYPE_POTAL_DUNGEON_USE;
								}

								pOkWnd->SetActive(TRUE);
								SendPacketRequestDungeonInfo(pItem->m_Item_Zodiac.wMapId);							
							}					
							else if(byType==ZODIAC_USE_TYPE_BELT)
							{

								for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Small[i].GetID()==__ITEM_PORTALUSED_INDEX__)
									{
										if(	(pMiniMapWnd->m_wMapId != g_pMainPlayer->m_pInv_Small[byZipcode].m_Item_Zodiac.wMapId) ||
											(pMiniMapWnd->m_byMapLayer!=g_pMainPlayer->m_pInv_Small[byZipcode].m_Item_Zodiac.bLayer))
										{
											CTDS_ITEM_PICKUP pPacket;
											pPacket.bSectionNum	= 1;
											pPacket.i64ItemID	= 0;
											SetItemPacket(&pPacket, 54, byZipcode, i, 0, 0);
											g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

											if(g_ItemMoveManager.GetNewItemMoveMode())
											{
												g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL,byZipcode,i);
											}
											break;

										}
										else
										{
											// "같은 지역에서는 이동을 할수가 없습니다."
											DisplayMessageAdd(g_Message[ETC_MESSAGE503].szMessage, 0xFFFF2CFF); 
										}
									}
								}
							}
						}
						
						return TRUE;
					}
					else
					{
						// "포탈 이동 중에는 사용할 수 없습니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE502].szMessage, 0xFFFF2CFF); 
					}
				}
			}
			else
			{
				// "대결 중에는 사용할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF); 
			}
		}
		// 마을 귀환 스크롤 추가 2005.02.14 김영대 
		else if (pItem->GetID()>= __ITEM_PORTAL_VILL1__ && pItem->GetID() <= __ITEM_PORTAL_VILL6__)
		{
			if(!g_pMainPlayer->m_bMatching)
			{
				if(pItem->m_Item_Zodiac.bType==1)
				{
					if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PORTAL_MOVING)
					{
						// 마을 귀환 스크롤 추가 2005.02.14 김영대 
						WORD wItemID = pItem->GetID();
						if(wItemID >= __ITEM_PORTAL_VILL1__ && wItemID <= __ITEM_PORTAL_VILL6__)
						{							
							if(byType==ZODIAC_USE_TYPE_INVENTORY)
							{								
								pOkWnd->m_bZipCode	= byZipcode;	
								pOkWnd->m_bType		= __OKWND_TYPE_POTAL_VILLAGE_USE;
								pOkWnd->SetActive(TRUE);
							}
							else if(byType==ZODIAC_USE_TYPE_BELT)
							{
								CTDS_ITEM_PICKUP pPacket;
								pPacket.bSectionNum	= 1;
								pPacket.i64ItemID	= 0;
								SetItemPacket(&pPacket, 53, byZipcode, 0, 0, 0);
								g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
								
								if(g_ItemMoveManager.GetNewItemMoveMode())
								{
									g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_BELT,byZipcode,0xff);
								}
							}

							return TRUE;
						}						
					}
					else
					{
						// "포탈 이동 중에는 사용할 수 없습니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE502].szMessage, 0xFFFF2CFF); 
					}
				}								
			}	
			else
			{
				// "대결 중에는 사용할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF); 
			}
		}		
		else
		{
			// "사용할 수 있는 아이템이 없습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE791].szMessage, 0xFFFF2CFF); 
		}

		return TRUE;
	}

	return FALSE;
}


BOOL ItemUsedSupplies(CItem* pItem, BYTE byZipcode, BYTE byType)
{
	WORD	wSumWeight		= g_pMainPlayer->GetSumWeight();
	WORD	wAllSumWeight	= g_pMainPlayer->GetAllSumWeight();
	float	fPerWeight		= ((float)wSumWeight/(float)wAllSumWeight)*100;
	int		nValue			= pItem->GetID() / ITEM_DISTRIBUTE;
	BOOL	bWeightChk		= FALSE;

	if ( nValue != ITEM_SUPPLIES_INDEX )
		return FALSE;
	
	if ( g_pMainPlayer->GetStatus() == UNIT_STATUS_PORTAL_MOVING )
	{
		// "포탈 이동 중에는 사용할 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE502].szMessage, 0xFFFF2CFF); 
		return TRUE;
	}

	if ( g_pMainPlayer->m_bMatching )
	{
		// "대결 중에는 사용할 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF); 
		return TRUE;
	}

	if ( byType == ZODIAC_USE_TYPE_INVENTORY )
	{		
		if ( !g_bBeltChk )
		{
			g_bBeltChk = FALSE;
			return FALSE;
		}

		g_bBeltChk = FALSE;
	}
	
	if ( fPerWeight >= WEIGTH_100PER_OVER )
	{
		time_t now;
	
		time(&now);
		localtime(&now);

		int nTime = (int)difftime(now, g_pMainPlayer->m_PotionTime);

		if (nTime < (int)(fPerWeight * 0.6) / 10 )		
		{
			//"포화무게한도에 도달하여 달리기가 안되며, 포션의 사용 딜레이가 증가합니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE986].szMessage, 0xffff0000);
			bWeightChk = TRUE;
			//bChk = TRUE;

			return TRUE;
		}
	}
	
	// sung-han 2005-03-07 중독이 되면 포션을 사용할수 없게 하기------------------------------------------------------
	if(g_pMainPlayer->m_nPoisonSec)
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE1415].szMessage, 0xffff0000);//중독이 되면 포션을 사용할 수 없습니다.
		return TRUE;
	}
	//----------------------------------------------------------------------------------------------------------------

	if( ( g_pMainPlayer->GetStatus() != UNIT_STATUS_CASTING ) && !bWeightChk )
	{
		// 물약 사용하기 //
		CTDS_ITEM_USED pPacket;
		pPacket.bZipCode = byZipcode;

		if ( byType == ZODIAC_USE_TYPE_BELT )
		{
			pPacket.bInv = 2;
		}
		else if ( byType == ZODIAC_USE_TYPE_INVENTORY )
		{
			pPacket.bInv = 1;
		}

		g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			BYTE bSrc = 0;
			if(byType == ZODIAC_USE_TYPE_INVENTORY)
				bSrc = ITEM_NATIVE_INV_SMALL;
			else if(byType == ZODIAC_USE_TYPE_BELT)
				bSrc = ITEM_NATIVE_BELT;

			g_ItemUsedManager.SendItemUsedNativePacket(bSrc,byZipcode,0xff);
		}

		time(&g_pMainPlayer->m_PotionTime);
		localtime(&g_pMainPlayer->m_PotionTime);

		return TRUE;
	}
	
	return FALSE;
}


BOOL ItemUsedConsumable(CItem* pItem, BYTE byZipcode, BYTE byType)
{
	int		nValue	= pItem->GetID() / ITEM_DISTRIBUTE;
	BYTE	byInv	= 0;

	if(nValue>=ITEM_CONSUMABLEST_INDEX && nValue<=ITEM_CONSUMABLEEND_INDEX)
	{
		if(pItem->m_Item_Consumable.bInvIndex>=1)
		{
			// "이미 사용중 입니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE848].szMessage, 0xffff2cff); 
			return TRUE;
		}
		
		if(byType == ZODIAC_USE_TYPE_BELT)
		{
			for(int i = 0; i < MAX_BELT_POOL; i++)
			{
				DWORD dwNewId = g_pMainPlayer->m_pBelt[i].GetID();
				DWORD dwOldId = pItem->GetID();

				if(dwNewId == dwOldId)
				{
					if(g_pMainPlayer->m_pBelt[i].m_Item_Consumable.bInvIndex>=1)
					{
						// "이미 사용중 입니다."					
						DisplayMessageAdd(g_Message[ETC_MESSAGE848].szMessage, 0xffff2cff); 
						return TRUE;
					}
				}

				if(IsSamePropertyConsumableItem(&g_pMainPlayer->m_pBelt[i], pItem))
				{
					if(g_pMainPlayer->m_pBelt[i].m_Item_Consumable.bInvIndex>=1)
					{
						// "같은효과의 아이템이 사용중입니다."					
						DisplayMessageAdd(g_Message[ETC_MESSAGE1431].szMessage, 0xffff2cff); 
						return TRUE;
					}
				}
			}

			byInv = 1;
		}
		else
		{
			for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Small[i].GetID() == pItem->GetID())
				{
					if(g_pMainPlayer->m_pInv_Small[i].m_Item_Consumable.bInvIndex>=1)
					{
						// "이미 사용중 입니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE848].szMessage, 0xffff2cff); 
						return TRUE;
					}
				}

				if(IsSamePropertyConsumableItem(&g_pMainPlayer->m_pInv_Small[i], pItem))
				{
					if(g_pMainPlayer->m_pInv_Small[i].m_Item_Consumable.bInvIndex>=1)
					{
						// "같은효과의 아이템이 사용중입니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE1431].szMessage, 0xffff2cff); 
						return TRUE;
					}
				}
			}			
		}
		
		CTDS_ITEM_PICKUP pPacket;
		pPacket.bSectionNum	= 1;
		pPacket.i64ItemID	= 0;			
		SetItemPacket(&pPacket, 74, byZipcode, byInv, 0, 0);
		g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
		
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			BYTE bSrc = 0;
			if(byType == ZODIAC_USE_TYPE_INVENTORY)
				bSrc = ITEM_NATIVE_INV_SMALL;
			else if(byType == ZODIAC_USE_TYPE_BELT)
				bSrc = ITEM_NATIVE_BELT;
			g_ItemUsedManager.SendItemUsedNativePacket(bSrc,byZipcode,0xff);
		}

		return TRUE;
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

	CBaseItem *pSrcBase = g_pItemTableHash->GetData(pSrc->GetID());
	CBaseItem *pDstBase = g_pItemTableHash->GetData(pDst->GetID());

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

BOOL ItemUsedSpecial(CItem* pItem, BYTE byZipcode, BYTE byType)
{
	int					nValue				= pItem->GetID() / ITEM_DISTRIBUTE;
	COkWnd*				pOkWnd				= COkWnd::GetInstance();
	CStoreWnd*			pStoreWnd			= CStoreWnd::GetInstance();
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	CGuildWnd*			pGuildWnd			= CGuildWnd::GetInstance();
	CInitItemWnd*		pInitItemWnd		= CInitItemWnd::GetInstance();

	if(nValue>=ITEM_SPECIALST_INDEX && nValue<=ITEM_SPECIALEND_INDEX)
	{							
		if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PORTAL_MOVING)
		{
			if(g_bGuildChk==TRUE)
			{
				if(!g_pMainPlayer->m_bMatching)
				{
					WORD wItemId = pItem->m_wItemID;

					if(wItemId==__ITEM_GUILD_INDEX__ || wItemId==__ITEM_CLEN_INDEX__)
					{
						if(pStoreWnd->GetActive())
						{
							// "상점이용 중에는 다른 행동을 하실 수 없습니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE1007].szMessage, 0xffff0000); 
						}
						else if(pItemTradeShopWnd->GetActive())
						{
							// "교환중에는 다른 행동을 하실 수 없습니다."							
							DisplayMessageAdd(g_Message[ETC_MESSAGE1006].szMessage, 0xFFFFFF00); 
						}
						else
						{
							if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_SHOP)	
							{
								// "노점중에는 다른 행동을 하실 수 없습니다."
								DisplayMessageAdd(g_Message[ETC_MESSAGE1170].szMessage, 0xFFFFFF00);	
							}
							else
							{
								CTDS_GUILD_ITEM pPacket;				
								pPacket.bZipCode = byZipcode;

								if(pItem->m_wItemID==__ITEM_GUILD_INDEX__)
									pPacket.bGuildType = __GCTYPE_GUILD__;
								if(pItem->m_wItemID==__ITEM_CLEN_INDEX__)
									pPacket.bGuildType = __GCTYPE_CLEN__;
								
								pGuildWnd->m_bZipCode	= byZipcode;
								g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
							}
						}
					}

					if(	wItemId==__ITEM_STATUS_INT__ ||
						(wItemId>=__ITEM_STATUS_INTSTART__ && wItemId<=__ITEM_STATUS_INTEND__) ||
						wItemId==__ITEM_SKILL_INT__ ||
						(wItemId>=__ITEM_SKILL_INTSTART__ && wItemId<=__ITEM_SKILL_INTEND__))
					{			
						if(g_pMainPlayer->m_dwLevel<10)
						{
							// "레벨 10 이상부터 사용할 수 있습니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE789].szMessage, 0xffff2cff); 
							return TRUE;
						}
						
						pInitItemWnd->m_byIndex	= byZipcode;
						
						if(	wItemId==__ITEM_STATUS_INT__ ||
							(wItemId>=__ITEM_STATUS_INTSTART__ && wItemId<=__ITEM_STATUS_INTEND__))
						{
							pInitItemWnd->m_byType	= 1;
						}
						else if(wItemId==__ITEM_SKILL_INT__ ||
							(wItemId>=__ITEM_SKILL_INTSTART__ && wItemId<=__ITEM_SKILL_INTEND__))
						{
							pInitItemWnd->m_byType	= 2;
						}

						pInitItemWnd->SetActive();
						g_bGuildChk	= FALSE;
					}

					if( wItemId== __ITEM_HEROITEM_INDEX__ )	
					{
						// 선물상자 이벤트 2002.02.02 김영대
						//태극기 아이템(선물상자)
						//영웅의 증표 아이템(선물상자)
						pOkWnd->m_bZipCode	= byZipcode;	
						pOkWnd->m_bType		= __OKWND_TYPE_UNPACK_PRESENT_BOX;
						pOkWnd->m_dwDungeonID = 0;	//인벤에서 사용 						
						pOkWnd->SetActive(TRUE);
					}
				}
				else
				{
					// "대결 중에는 사용할 수 없습니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE501].szMessage, 0xFFFF2CFF); 
				}
			}
		}
		else
		{
			// "포탈 이동 중에는 사용할 수 없습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE502].szMessage, 0xFFFF2CFF); 
		}

		return TRUE;
	}	
	
	return FALSE;
}


BOOL ItemUsedMagicArray(CItem* pItem, BYTE byZipcode, BYTE byType)
{
	CGroupWnd*	pGroupWnd		= CGroupWnd::GetInstance();
	char		szInfo[0xff]	= {0,};
	BOOL		bPotal			= TRUE;
	int			nValue			= pItem->GetID() / ITEM_DISTRIBUTE;
	
	if(nValue!=ITEM_MAGICARRAY_INDEX)
		return FALSE;

	if (DUNGEON_TYPE_EVENT == g_pThisDungeon->GetDungeonType())
	{
		// "이벤트 던전에서는 사용할 수 없습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE253].szMessage); 
		DisplayMessageAdd(szInfo, 0xFFFF2CFF);
		return TRUE;
	}

	DWORD dwMagicArrayTick = timeGetTime();

	if( (dwMagicArrayTick - g_dwMagicArrayTick) > 10000)
	{		
		if(pItem->GetID()==__ITEM_PARTY_PORTAL1__)
		{
			//  퍼스널 리콜.
			if(g_pMainPlayer->m_dwPartyId!=0)
			{
				if(g_pMainPlayer->m_bPartyLeader == 1)						
				{
					if(pGroupWnd->m_nPartyUserIndex > 0)
					{					
						ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();
						BYTE byUserIndex = 1;

						while(pNode)
						{
							LP_PARTY_USER sPartyNode = pNode->pData;

							if(pGroupWnd->m_nPartyUserIndex == byUserIndex)
							{																	
								if(bPotal)
								{
									g_dwMagicArrayTick = timeGetTime();

									// 메세지 보내기 //
									CTDS_PARTY_PORTAL pPacket;
									memset(pPacket.szName, 0, sizeof(pPacket.szName));
									__lstrcpyn(pPacket.szName, g_pMainPlayer->m_szName, sizeof(g_pMainPlayer->m_szName));
									
									pPacket.byZipCode			= byZipcode;
									pPacket.dwPartyId			= g_pMainPlayer->m_dwPartyId;
									pPacket.dwUserIndex			= g_pMainPlayer->m_dwUserIndex;
									pPacket.dwPartalUserIndex	= sPartyNode->dwUserIndex;
									g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

									if(g_ItemMoveManager.GetNewItemMoveMode())
									{
										g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL
											, byZipcode
											, 0xff
											, g_pMainPlayer->m_dwPartyId
											, sPartyNode->dwUserIndex);
									}

									g_bMagicChk	= FALSE;

									// "%s 님에게 메세지를 전달하였습니다."
									wsprintf(szInfo, g_Message[ETC_MESSAGE513].szMessage, sPartyNode->szName); 
									DisplayMessageAdd(szInfo, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY);
								}
								else
								{									
									// "공성전에는 포탈을 사용할 수 없습니다."
									wsprintf(szInfo, g_Message[ETC_MESSAGE514].szMessage); 
									DisplayMessageAdd(szInfo, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY);
								}

								return TRUE;
							}

							byUserIndex++;

							pNode = pNode->pNext;
						}
					}									
					else
					{
						if(pGroupWnd->m_nPartyUserIndex==-1)
						{
							// "파티원을 설정해 주십시오."
							DisplayMessageAdd(g_Message[ETC_MESSAGE515].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
						}
						else
						{
							// "자신에게 포탈을 사용할수 없습니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE516].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
						}
					}
				}
				else
				{
					// "파티리더가 아닙니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE517].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
				}
			}
			else
			{
				// "파티중이 아닙니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE518].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
			}
		}
		else if(pItem->GetID()==__ITEM_PARTY_PORTAL2__)
		{
			// 파티 전원 리콜.
			if(g_pMainPlayer->m_dwPartyId!=0)
			{
				if(g_pMainPlayer->m_bPartyLeader==1)						
				{
					ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

					while(pNode)
					{
						LP_PARTY_USER sPartyNode = pNode->pData;
						
						if(g_pMainPlayer->m_dwUserIndex != sPartyNode->dwUserIndex)
						{
							g_dwMagicArrayTick = timeGetTime();
							
							CTDS_ITEM_PICKUP pPacket;
							pPacket.bSectionNum	= 1;
							pPacket.i64ItemID	= 0;
							pPacket.bInv		= 62;
							pPacket.bZipCode	= byZipcode;
							g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL, byZipcode, 0xff);
							}

							break;
						}
						
						pNode = pNode->pNext;
					}	
					
					// "파티원에게 파티 포탈 메세지를 전달하였습니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE519].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
				}
				else
				{
					// "파티리더가 아닙니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE517].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
				}
			}
			else
			{
				// "파티중이 아닙니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE518].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
			}
		}
		else if(pItem->GetID()>=__ITEM_PARTY_LIFELINK1__ && pItem->GetID()<=__ITEM_PARTY_LIFELINK5__)
		{
			if(g_pThisDungeon->IsVillage())
			{				
				// "마을에서는 사용할 수 없습니다."
				wsprintf(szInfo, g_Message[ETC_MESSAGE810].szMessage); 
				DisplayMessageAdd(szInfo, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY);
				return TRUE;
			}

			// 라이프 링크.
			if(g_pMainPlayer->m_dwPartyId!=0)
			{
				if(g_pMainPlayer->m_bPartyLeader==1)						
				{									
					g_dwMagicArrayTick = timeGetTime();
					
					CTDS_ITEM_PICKUP pPacket;
					pPacket.bInv		= 61;
					pPacket.bSectionNum	= 1;
					pPacket.i64ItemID	= 0;
					pPacket.bZipCode	= byZipcode;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL, byZipcode, 0xff);
					}

					return TRUE;			
				}
				else
				{
					// "파티리더가 아닙니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE517].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
				}
			}
			else
			{
				// "파티중이 아닙니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE518].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
			}
		}
		else if(pItem->GetID()>=__ITEM_PARTY_AMBUSH1__ && pItem->GetID()<=__ITEM_PARTY_AMBUSH5__)
		{
			if(g_pThisDungeon->IsVillage())
			{				
				// "마을에서는 사용할 수 없습니다."
				wsprintf(szInfo, g_Message[ETC_MESSAGE810].szMessage); 
				DisplayMessageAdd(szInfo, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY);
				return TRUE;
			}

			// 엠부쉬.
			if(g_pMainPlayer->m_dwPartyId!=0)
			{
				if(g_pMainPlayer->m_bPartyLeader==1)						
				{
					g_dwMagicArrayTick = timeGetTime();

					CTDS_ITEM_PICKUP pPacket;
					pPacket.bInv		= 61;
					pPacket.bSectionNum	= 1;
					pPacket.i64ItemID	= 0;
					pPacket.bZipCode	= byZipcode;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL, byZipcode, 0xff);
					}
					
					return TRUE;
				}
				else
				{
					// "파티리더가 아닙니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE517].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
				}
			}
			else
			{
				// "파티중이 아닙니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE518].szMessage, 0xFFFF2CFF); 
			}
		}
		else if(pItem->GetID()>=__ITEM_PARTY_ST1__ && pItem->GetID()<=__ITEM_PARTY_ST5__)
		{
			if(g_pThisDungeon->IsVillage())
			{			
				// "마을에서는 사용할 수 없습니다."
				wsprintf(szInfo, g_Message[ETC_MESSAGE810].szMessage); 
				DisplayMessageAdd(szInfo, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY);
				return TRUE;
			}
			
			// 시너지 스틱 트레이닝.
			if(g_pMainPlayer->m_dwPartyId!=0)
			{
				if(g_pMainPlayer->m_bPartyLeader==1)						
				{
					g_dwMagicArrayTick = timeGetTime();
					
					CTDS_ITEM_PICKUP pPacket;
					pPacket.bInv		= 61;
					pPacket.bSectionNum	= 1;
					pPacket.i64ItemID	= 0;
					pPacket.bZipCode	= byZipcode;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL, byZipcode, 0xff);
					}

					return TRUE;								
				}
				else
				{
					// "파티리더가 아닙니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE517].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
				}
			}
			else
			{
				// "파티중이 아닙니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE518].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
			}
		}
		else
		{
			// "사용할 수 있는 아이템이 없습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE52].szMessage, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY); 
		}

		return TRUE;
	}
	else
	{		
		// "소환 진행중에는 다시 소환할수없습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE512].szMessage); 
		DisplayMessageAdd(szInfo, 0xFFFF2CFF, TRUE, DEF_CHAT_TYPE_PARTY);
		return TRUE;
	}

	return FALSE;
}


BOOL ItemUsedPresentBox(CItem* pItem, BYTE byZipcode, BYTE byType)
{
	int	nValue = pItem->GetID() / ITEM_DISTRIBUTE;

	if(nValue == ITEM_PRESENT_BOX_INDEX)
	{
		COkWnd* pOkWnd = COkWnd::GetInstance();		
		pOkWnd->m_bZipCode		= byZipcode;	
		pOkWnd->m_bType			= __OKWND_TYPE_UNPACK_PRESENT_BOX;
		pOkWnd->m_dwDungeonID	= 1;	//벨트에서 사용 
		pOkWnd->SetActive(TRUE);
		return TRUE;
	}

	return FALSE;
}