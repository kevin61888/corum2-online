#include "ItemNative.h"
#include "stdafx.h"
#include "Define.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "GameControl.h"
#include "HashTable.h"
#include "EffectCommand.h"
#include "EffectProcess.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "Npc.h"
#include "DungeonTable.h"
#include "WorldProcess.h"
#include "Interface.h"
#include "UserInterface.h"
#include "Script.h"
#include "Filter.h"
#include "ItemWnd.h"
#include "CharWnd.h"
#include "GameMenuWnd.h"
#include "SkillWnd.h"
#include "ChatWnd.h"
#include "CommandWnd.h"
#include "Struct.h"
#include "SkillWnd.h"
#include "ItemShopWnd.h"
#include "DSMiniMap.h"
#include "QuantityWnd.h"
#include "ExitWnd.h"
#include "StoreWnd.h"
#include "cdungeonmanagerwnd.h"
#include "cdungeoninfownd.h"
#include "BankWnd.h"
#include "GuildWnd.h"
#include "GuildExWnd.h"
#include "ChatWnd.h"
#include "GuildJoinWnd.h"
#include "GuildRankWnd.h"
#include "GMTool.h"
#include "GuildEditWnd.h"
#include "PartyJoinWnd.h"
#include "ComboBox.h"
#include "NoticeWnd.h"
#include "HelpWnd.h"
#include "ErrorWnd.h"
#include "ItemTradeWnd.h"
#include "ItemTradeShopWnd.h"
#include "message.h"
#include "OkWnd.h"
#include "GroupWnd.h"
#include "npcdungeoninfownd.h"
#include "playershopwnd.h"
#include "PartyPortalWnd.h"
#include "RequestMatchWnd.h"
#include "CodeFun.h"
#include "Party.h"
#include "Trade.h"
#include "Guild.h"
#include "Chat.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "DungeonSiegeInfoWnd.h"
#include "GameNoticeWnd.h"
#include "InitItemWnd.h"
#include "IMEEdit.h"
#include "Messenger.h"
#include "Shop.h"
#include "InputManager.h"
#include "WorldWnd.h"
#include "CharDieWnd.h"
#include "ChatBackground.h"
#include "PathFinding\SearchModule_Astar.h"
#include "NetworkClient.h"
#include "Map.h"
#include "Effect.h"
#include "DamageNumber.h"
#include "ChrInfo.h"
#include "RankWnd.h"
#include "RankNpcWnd.h"
#include "EventNPCWnd.h"
#include "EventNPCSubWnd.h"
#include "special_effect.h"
#include "PartyEntryWnd.h"
#include "PartyBoardWnd.h"
#include "ItemUsed.h"
#include "ItemMakingWnd.h"
#include "EventDungeonProcess.h"
#include "TopDungeonHelpWnd.h"
#include "MinboboWork.h"


#define ITEM_SKILL_BOOK_START	20519
#define ITEM_SKILL_BOOK_END		20578


extern BYTE g_bKeyItem_Large[MAX_INV_LARGE_POOL];


void ItemNativeOnLButtonUpProcess()
{
	CChatWnd*			pChatWnd			= CChatWnd::GetInstance();
	CGuildExWnd*		pGuildExWnd			= CGuildExWnd::GetInstance();
	CStoreWnd*			pStoreWnd			= CStoreWnd::GetInstance();	
	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	CQuantityWnd*		pQuantityWnd		= CQuantityWnd::GetInstance();	
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	CPlayerShopWnd*		pPlayerShopWnd		= CPlayerShopWnd::GetInstance();

	BOOL				bNewItemMove		= FALSE;
	
	if(!g_Mouse.bLDown)
	{
		if(pStoreWnd->m_bSupplies==TRUE)
		{
			if(pItemWnd->GetActive())
			{
				//##########################
				// 아이템 라쥐 스몰 지역  
				//##########################
				if(	g_Mouse.MousePos.x>=pItemWnd->m_fPosX+6 &&
					g_Mouse.MousePos.x<=pItemWnd->m_fPosX+244 &&
					g_Mouse.MousePos.y>=pItemWnd->m_fPosZ+278 &&
					g_Mouse.MousePos.y<=pItemWnd->m_fPosZ+482)
				{
					if(pItemTradeShopWnd->GetActive()==TRUE)
						return;

					if(ItemCollision(ITEM_WND)==TRUE)
					{
						if(pStoreWnd->m_wSuppliesId==0)
							return;

						//#################################################################################						
						// PlayerShop
						//#################################################################################						
						if(pPlayerShopWnd->m_bChkMouse == 1)
						{
							if(!pPlayerShopWnd->m_pOwnerPlayerShop)
							{
								pStoreWnd->m_wSuppliesId	= 0;
								pPlayerShopWnd->m_bChkMouse = 0;
								return;
							}

							int nValue = pStoreWnd->m_wSuppliesId/ITEM_DISTRIBUTE;

							if(GetType(nValue)==ITEM_LARGE)
							{
								int nPosX		= (g_Mouse.MousePos.x-(int)pItemWnd->m_fPosX-6)/34;
								int nPosY		= (g_Mouse.MousePos.y-(int)pItemWnd->m_fPosZ-278)/68;
								int nItemIndex	= nPosY*7+nPosX;

								if(nItemIndex>=MAX_INV_LARGE_POOL)
									nItemIndex = 0;

								CTDS_ITEM_PICKUP ItemPickup;	
								ItemPickup.bSectionNum	= 1;
								ItemPickup.i64ItemID	= 0;
								ItemPickup.bInv			= 72;
								ItemPickup.dwMoney		= pPlayerShopWnd->m_pOwnerPlayerShop->m_dwUserIndex;
								ItemPickup.dwId			= pStoreWnd->m_wSuppliesId;
								ItemPickup.bZipCode		= pPlayerShopWnd->m_bZipCode;						
								ItemPickup.dwShop		= pPlayerShopWnd->m_sPlayerShopInfo.m_dwCustomSellPrice[pPlayerShopWnd->m_bZipCode];
								g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

								if(g_ItemMoveManager.GetNewItemMoveMode())
								{
									g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_LARGE
										, nItemIndex
										, 1
										, 1
										, pPlayerShopWnd->m_sPlayerShopInfo.m_dwCustomSellPrice[pPlayerShopWnd->m_bZipCode]
										, pStoreWnd->m_wSuppliesId);
									bNewItemMove = TRUE;
								}
							}
							else if(GetType(nValue)==ITEM_SMALL)
							{
								int nPosX		= (g_Mouse.MousePos.x-(int)pItemWnd->m_fPosX-6)/34;
								int nPosY		= (g_Mouse.MousePos.y-(int)pItemWnd->m_fPosZ-278)/34;
								int nItemIndex	= nPosY*7+nPosX;

								if(nItemIndex>=MAX_INV_SMALL_POOL)
								{
									pQuantityWnd->m_byZipCode = 0;
								}
								else
								{
									pQuantityWnd->m_byZipCode = nItemIndex;
								}

								if(g_pGVDungeon->bChatMode)
								{
									ChatModeOff(INPUT_BUFFER_19, 0, TRUE);									
								}

								g_pInputManager->ClearInput(INPUT_BUFFER_19);
								
								if(pGuildExWnd->GetActive()==TRUE)
								{
									pGuildExWnd->SetActive(FALSE);				
								}

								if(pChatWnd->GetActive()==TRUE)								
								{
									pChatWnd->SetActive(FALSE);								
								}
								
								// 창 열기 //
								pQuantityWnd->OpenWnd(__QUANTITY_PLAYERSHOPSELLCNT__);
								
								RECT rt;
								rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
								rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
								rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
								rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
								int nOrder	= __ORDER_INTERFACE_START__ + pQuantityWnd->m_byOrder*10+2;						
								
								g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
								g_pInputManager->SetFocusInput(INPUT_BUFFER_5);								
							}
						}
						//#################################################################################						
						// Npc Shop
						//#################################################################################	
						else
						{
							int nValue = pStoreWnd->m_wSuppliesId/ITEM_DISTRIBUTE;
						
							if(GetType(nValue)==ITEM_SMALL)
							{						
								int nPosX		= (g_Mouse.MousePos.x-(int)pItemWnd->m_fPosX-6)/34;
								int nPosY		= (g_Mouse.MousePos.y-(int)pItemWnd->m_fPosZ-278)/34;
								int nItemIndex	= nPosY*7+nPosX;

								CBaseItem* lpItemTable = g_pItemTableHash->GetData(pStoreWnd->m_wSuppliesId);
				
								if(lpItemTable)
								{
									int nMaxQuantity	= 1;
									nMaxQuantity		= lpItemTable->GetMaxQuantity();							

									if(nMaxQuantity==1)
									{
										CTDS_ITEM_PICKUP ItemPickup;	
										ItemPickup.bSectionNum	= 1;
										ItemPickup.i64ItemID	= 0;
										ItemPickup.bInv			= 19;
										ItemPickup.dwMoney		= 1;
										ItemPickup.dwId			= pStoreWnd->m_wSuppliesId;
										ItemPickup.bZipCode		= nItemIndex;
										g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

										if(g_ItemMoveManager.GetNewItemMoveMode())
										{
											g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL,nItemIndex,1,1);
											bNewItemMove = TRUE;
										}
									}
									else
									{							
										if(g_pGVDungeon->bChatMode)
											ChatModeOff(INPUT_BUFFER_19, 0, TRUE);

										g_pInputManager->ClearInput(INPUT_BUFFER_19);
										
										if(pGuildExWnd->GetActive()==TRUE)
											pGuildExWnd->SetActive(FALSE);				

										if(pChatWnd->GetActive()==TRUE)								
											pChatWnd->SetActive(FALSE);								
										
										// 창 열기 //
										pQuantityWnd->OpenWnd(__QUANTITY_BUY__);						

										RECT rt;
										rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
										rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
										rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
										rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
										int nOrder	= pQuantityWnd->GetStartOrder()+2;						

										g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
										g_pInputManager->SetFocusInput(INPUT_BUFFER_5);

										if(nItemIndex>=MAX_INV_SMALL_POOL)
										{
											pQuantityWnd->m_byZipCode = 0;
										}
										else
										{
											pQuantityWnd->m_byZipCode = nItemIndex;
										}
										
										if(g_ItemMoveManager.GetNewItemMoveMode())
										{
											bNewItemMove = TRUE;
										}
									}
								}
							}
							else if(GetType(nValue)==ITEM_LARGE)
							{
								CTDS_ITEM_PICKUP ItemPickup;	
								ItemPickup.bSectionNum	= 1;
								ItemPickup.i64ItemID	= 0;

								if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)
									ItemPickup.bInv = 35;
								else if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
									ItemPickup.bInv = 37;
								else if(nValue==ITEM_BAG_INDEX)
									ItemPickup.bInv = 85;

								int nPosX		= (g_Mouse.MousePos.x-(int)pItemWnd->m_fPosX-6)/34;
								int nPosY		= (g_Mouse.MousePos.y-(int)pItemWnd->m_fPosZ-278)/68;
								int nItemIndex	= nPosY*7+nPosX;

								if(nItemIndex>=MAX_INV_LARGE_POOL)
									nItemIndex = 0;
								
								ItemPickup.dwId			= pStoreWnd->m_wSuppliesId;
								ItemPickup.bZipCode		= nItemIndex;
								g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

								if(g_ItemMoveManager.GetNewItemMoveMode())
								{
									g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_LARGE,nItemIndex,1,1);
									bNewItemMove = TRUE;
								}
							}
						}						
					}									
				}
			}

			// 마우스 처리 //
			pItemWnd->m_wItemId[__ITEM_MOUSE_START__] = 0;
			pStoreWnd->m_bSupplies		= FALSE;
			pPlayerShopWnd->m_bChkMouse	= 0;
		}

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			if(g_ItemMoveManager.IsItemPick() && !bNewItemMove)
			{
				g_pMainPlayer->m_nItemSelect = 0;
				ItemChk();				
			}
		}
		else
		{
			if(g_pMainPlayer->m_MouseItem.m_wItemID!=0)
			{
				g_pMainPlayer->m_nItemSelect = 0;						
				ItemChk();
			}		
		}
	}
}


//==============================================================//
// Item 이동 함수.												//
//==============================================================//
void ItemChk()
{
	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	CBankWnd*			pBankWnd			= CBankWnd::GetInstance();
	CQuantityWnd*		pQuantityWnd		= CQuantityWnd::GetInstance();
	CStoreWnd*			pStoreWnd			= CStoreWnd::GetInstance();
	CGuildExWnd*		pGuildExWnd			= CGuildExWnd::GetInstance();
	CDungeonManagerWnd* pDungeonManagerWnd	= CDungeonManagerWnd::GetInstance();
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	BOOL				bItemActive			= FALSE;
	BOOL				bItemLand			= FALSE;
	int					nValue				= FALSE;
	CTDS_ITEM_PICKUP	ItemPickup;
	
	ItemPickup.bSectionNum	= 1;
	ItemPickup.i64ItemID	= 0;

	if( pQuantityWnd->GetActive() )			return;
	if( pQuantityWnd->m_bReq )				return;
	
	if(g_ItemMoveManager.GetNewItemMoveMode())
	{
		WORD wMouseID = g_ItemMoveManager.GetMouseItemID();
		if(!wMouseID)
			return;
		nValue =  wMouseID / ITEM_DISTRIBUTE;	
	}	
	else
	{
		nValue = g_pMainPlayer->m_MouseItem.GetID() /ITEM_DISTRIBUTE;
	}
	
	// Item Wnd //
	if(pItemWnd->GetActive()==TRUE)
	{
		//##########################
		// 아이템 라쥐 스몰 지역  
		//##########################
		if(g_Mouse.MousePos.x>=pItemWnd->m_fPosX+6 && g_Mouse.MousePos.x<=pItemWnd->m_fPosX+244 &&
			g_Mouse.MousePos.y>=pItemWnd->m_fPosZ+278 && g_Mouse.MousePos.y<=pItemWnd->m_fPosZ+482)
		{			
			bItemLand = FALSE;

			if(pQuantityWnd->m_bReq==TRUE)
				return;

			// Item 좌표 체크 //
			if(ItemCollision(ITEM_WND)==FALSE)
			{
				if(InsertItem(&ItemPickup))
					return;

				bItemLand = TRUE;
			}

			if(!bItemLand)
			{
				if(!pQuantityWnd->GetActive())
				{
					if(GetType(nValue)==ITEM_LARGE)
					{					
						int nPosX = (g_Mouse.MousePos.x-pItemWnd->m_fPosX-6)/34;
						int nPosY = (g_Mouse.MousePos.y-pItemWnd->m_fPosZ-278)/68;
						int nItemIndex = nPosY*7+nPosX;

						if(nItemIndex>=MAX_INV_LARGE_POOL)
						{
							for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
								{
									bItemActive	= TRUE;
									SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
									g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
									return;
								}
							}

							SetItemPacket(&ItemPickup, 10, 0, 0, 0, 0);
							ItemPickup.fItemX	= g_Mouse.v3Mouse.x;
							ItemPickup.fItemY	= g_Mouse.v3Mouse.y;
							ItemPickup.fItemZ	= g_Mouse.v3Mouse.z;
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
							return;
						}
						
						if(g_ItemMoveManager.GetNewItemMoveMode())
							g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_LARGE,nItemIndex,1,1);
				
						if(g_pMainPlayer->m_pInv_Large[nItemIndex].m_wItemID==0)
						{
							bItemActive	= TRUE;
							SetItemPacket(&ItemPickup, 6, nItemIndex, 0, 0, 0);							
						}
						else
						{
							//  비워있는 아이템 없으면 바닥에 버리고 있으면 넣는다 //
							for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
								{
									bItemActive	= TRUE;
									SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
									break;
								}
							}

							if(bItemActive==FALSE)
							{
								if(CBankWnd::GetInstance()->GetActive())
								{
									for(int i = 0; i < MAX_BANK_LARGE_POOL; i++)
									{
										if(g_pMainPlayer->m_pBank_Large[i].m_wItemID==0)
										{
											bItemActive	= TRUE;
											SetItemPacket(&ItemPickup, 24, i, 0, 0, 0);

											// "인벤토리에 공간이 부족합니다."
											DisplayMessageAdd(g_Message[ETC_MESSAGE90].szMessage, 0xFFFFC309); 
											// "아이템을 창고로 이동합니다."
											DisplayMessageAdd(g_Message[ETC_MESSAGE252].szMessage, 0xFFFFC309); 
											break;
										}
									}		
								}
								
								if(bItemActive==FALSE)
								{
									// case 10:									
									SetItemPacket(&ItemPickup, 10, 0, 0, 0, 0);
									bItemActive	= TRUE;
								}								
							}
						}						
					}															
					else if(GetType(nValue)==ITEM_SMALL)
					{	
						CChatWnd* pChatWnd = CChatWnd::GetInstance();

						//== 아이템 인베토리에 넣기(소비아이템) ==//
						int nPosX = (g_Mouse.MousePos.x-pItemWnd->m_fPosX-6)/34;
						int nPosY = (g_Mouse.MousePos.y-pItemWnd->m_fPosZ-278)/34;
						int nItemIndex = nPosY*7+nPosX;

						if(nItemIndex>MAX_INV_SMALL_POOL)
						{
							for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
									g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
									return;
								}
							}			
							
							if(g_pThisDungeon->IsVillage()==TRUE)
							{
								for(int j = 0; j < MAX_BANK_SMALL_POOL; j++)
								{
									if(g_pMainPlayer->m_pBank_Small[j].m_wItemID==0)
									{
										ItemPickup.bInv		= 25;
										ItemPickup.bZipCode	= j;
										ItemPickup.dwMoney	= g_pMainPlayer->m_MouseItem.GetQuantity();
										pQuantityWnd->m_bReq = TRUE;																				
									}
								}
							}
							return;
						}
						else
						{
							pQuantityWnd->m_byZipCode = nItemIndex;						
						}
						
						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							ITEM_NATIVE_INFO ItemNativeInfo;
							ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
							ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

							if(ItemNativeInfo.eSlotID == ITEM_NATIVE_BANK_SMALL)
							{
								CItem cItem;
								memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));

								if(cItem.GetQuantity() > 1 )
								{
									if(g_pGVDungeon->bChatMode)
										ChatModeOff(INPUT_BUFFER_19);

									if(pGuildExWnd->GetActive()==TRUE)
										pGuildExWnd->SetActive(FALSE);

									if(pChatWnd->GetActive()==TRUE)
										pChatWnd->SetActive(FALSE);
							
									g_pInputManager->ClearInput(INPUT_BUFFER_19);

									pQuantityWnd->OpenWnd(__QUANTITY_FROMBANKTOINV__);
								}
								else
								{
									BYTE byCnt = cItem.GetQuantity();
									g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL,nItemIndex,1,byCnt);	
									pQuantityWnd->m_bReq = TRUE;
								}
							}
							else
							{
								CItem cItem;
								memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));
								BYTE byCnt = cItem.GetQuantity();
								g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL,nItemIndex,1,byCnt);	
								pQuantityWnd->m_bReq = TRUE;
							}
						}

						// 아이템칸이 비워 있으면 삽입 //
						if(g_pMainPlayer->m_pInv_Small[nItemIndex].m_wItemID==0)
						{
							bItemActive			= TRUE;
							ItemPickup.bZipCode	= nItemIndex;
							ItemPickup.bInv		= 14;
						}
						else
						{					
							// 아이템이 있으면 처리 //
							for(int i = nItemIndex; i < MAX_INV_SMALL_POOL; i++)
							{
								// 아이템에서 있는 칸을 찾는다 //
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
								{
//									LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
									CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

									if(lpItemTable)
									{
										if(g_pMainPlayer->m_pInv_Small[i].GetQuantity()<lpItemTable->GetMaxQuantity())
										{
											SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
											bItemActive	= TRUE;
											break;
										}
									}
								}
							}

							if(!bItemActive)
							{
								for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
								{								
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
									{									
										SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
										bItemActive = TRUE;
										break;
									}
								}
							}	
							
							if(!bItemActive)
							{					
								// 수량 창 //
								if(g_pThisDungeon->IsVillage()==TRUE)
								{
									for(int j = 0; j < MAX_BANK_SMALL_POOL; j++)
									{
										if(g_pMainPlayer->m_pBank_Small[j].m_wItemID==0)
										{
											ItemPickup.bInv		= 25;
											ItemPickup.bZipCode	= j;
											ItemPickup.dwMoney	= g_pMainPlayer->m_MouseItem.GetQuantity();
											pQuantityWnd->m_bReq = TRUE;																				
											g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
											return;
										}
									}
								}
								bItemActive = TRUE;
								SetItemPacket(&ItemPickup, 4, 0, 0, 0, 0);
							}
						}										
					}
					
					if(bItemActive==TRUE)
					{
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
						return;
					}					
				}
			}
		}
		//##########################
		// 가디언 지역  
		//##########################
		else if(g_Mouse.MousePos.x>=pItemWnd->m_fPosX+2 && g_Mouse.MousePos.x<=pItemWnd->m_fPosX+34 &&
				g_Mouse.MousePos.y>=pItemWnd->m_fPosZ+528 && g_Mouse.MousePos.y<=pItemWnd->m_fPosZ+592)
		{
			bItemLand = FALSE;

			if(pQuantityWnd->m_bReq==TRUE)
				return;

			// Item 좌표 체크 //
			if(ItemCollision(ITEM_WND)==FALSE)
			{
				if(InsertItem(&ItemPickup))
					return;

				bItemLand = TRUE;
			}

			if(bItemLand==FALSE)
			{
				if(!pQuantityWnd->GetActive())
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_GUARDIAN,0,1,1,0,0);
						return;
					}

					int nValue = g_pMainPlayer->m_MouseItem.m_wItemID /ITEM_DISTRIBUTE;

					if(GetType(nValue)==ITEM_LARGE)
					{
						int nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;

						if(nVal==ITEM_GUARDIAN_INDEX)
						{
							ItemPickup.bInv		= 38;
							ItemPickup.bZipCode	= 0;
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							return;
						}
						else
						{
							for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
								{
									bItemActive	= TRUE;
									SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
									g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
									return;
								}
							}
						}
					}
					else if(GetType(nValue)==ITEM_SMALL)
					{
						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
								g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								return;
							}
						}			
						
						if(g_pThisDungeon->IsVillage()==TRUE)
						{
							for(int j = 0; j < MAX_BANK_SMALL_POOL; j++)
							{
								if(g_pMainPlayer->m_pBank_Small[j].m_wItemID==0)
								{
									ItemPickup.bInv		= 25;
									ItemPickup.bZipCode	= j;
									ItemPickup.dwMoney	= g_pMainPlayer->m_MouseItem.GetQuantity();
									pQuantityWnd->m_bReq = TRUE;																				
								}
							}
						}
					}
				}				
			}
		}
		//##########################
		// 장착 지역  
		//##########################
		else if(g_Mouse.MousePos.x>=pItemWnd->m_fPosX+6 && g_Mouse.MousePos.x<=pItemWnd->m_fPosX+244 &&
				g_Mouse.MousePos.y>=pItemWnd->m_fPosZ+40 && g_Mouse.MousePos.y<=pItemWnd->m_fPosZ+248)
		{			
			bItemLand = FALSE;

			if(pQuantityWnd->m_bReq==TRUE)
				return;

			// Item 좌표 체크 //
			if(ItemCollision(ITEM_WND)==FALSE)
			{
				if(InsertItem(&ItemPickup))
					return;

				bItemLand = TRUE;
			}

			if(bItemLand==FALSE)
			{
				if(!pQuantityWnd->GetActive())
				{
					if(nValue<=__MAX_ITEM_LARGE_INDEX__)
					{
						ItemMouseUp();
						return;
					}
					else if(GetType(nValue)==ITEM_SMALL)
					{								
						// 아이템이 있으면 처리 //
						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							// 아이템에서 있는 칸을 찾는다 //
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<100)
								{
									SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
									bItemActive	= TRUE;
									break;
								}
							}
						}
						
						if(!bItemActive)
						{
							for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
							{								
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{									
									SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
									bItemActive = TRUE;
									break;
								}
							}
						}
						
						if(!bItemActive)
						{					
							// 수량 창 //
							bItemActive = TRUE;
							SetItemPacket(&ItemPickup, 4, 0, 0, 0, 0);
						}
		
						if(bItemActive==TRUE)
						{
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
							return;
						}						
					}
				}				
			}
		}
		//##########################
		// 나머지 지역  
		//##########################
		else if(g_Mouse.MousePos.x>=pItemWnd->m_fPosX &&
				g_Mouse.MousePos.x<=pItemWnd->m_fPosX+CItemWnd::GetInstance()->m_fMaxSizeWidth	&&
				g_Mouse.MousePos.y>=pItemWnd->m_fPosZ &&
				g_Mouse.MousePos.y<=pItemWnd->m_fPosZ+CItemWnd::GetInstance()->m_fMaxSizeHeight)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.Initialize();
			}

			if(pQuantityWnd->m_bReq==TRUE)
				return;
			
			if(InsertItem(&ItemPickup))
				return;			
		}
	}

	// Item Trade Wnd //
	if(pItemTradeShopWnd->GetActive()==TRUE)
	{
		if(	g_Mouse.MousePos.x>=pItemTradeShopWnd->m_fPosX+7	&&
			g_Mouse.MousePos.x<=pItemTradeShopWnd->m_fPosX+252 	&&
			g_Mouse.MousePos.y>=pItemTradeShopWnd->m_fPosZ+52 	&&
			g_Mouse.MousePos.y<=pItemTradeShopWnd->m_fPosZ+186)
		{
			if(pQuantityWnd->GetActive()==TRUE)
				return;

			int nValue = 0;

			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(g_ItemMoveManager.GetMouseItemID() == 0)
					return;

				nValue = g_ItemMoveManager.GetMouseItemID() / ITEM_DISTRIBUTE; 
			}
			else
			{
				nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;
			}
			
			int nPosX	= (g_Mouse.MousePos.x-(pItemTradeShopWnd->m_fPosX+7))/35;
			int nPosY	= (g_Mouse.MousePos.y-(pItemTradeShopWnd->m_fPosZ+52))/67;
			int nIndex	= nPosY*7+nPosX;

			if(GetType(nValue)==ITEM_LARGE)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_pMainPlayer->m_pItemTrade[nIndex].GetID()==0)
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADE,nIndex,1,1,0,0);
						return;
					}
					else
					{
						for(int i = 0; i < MAX_TRADE; i++)
						{
							if(g_pMainPlayer->m_pItemTrade[i].GetID()==0)
							{
								g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADE,nIndex,1,1,0,0);
								return;
							}
						}
					}
				}
				else
				{
					if(g_pMainPlayer->m_MouseItem.GetID()!=0)
					{				
						// Large Item //				
						if(g_pMainPlayer->m_pItemTrade[nIndex].GetID()==0)
						{
							// Process //

//							sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 거래...
							CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
							if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
							{
								SetItemPacket(&ItemPickup, 46, nIndex, 0, 0, 0);
								g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								return;
							}
							else
							{
								InvReCallItem(ITEM_LARGE);
								DisplayMessageAdd(g_Message[ETC_MESSAGE1417].szMessage, TEXT_COLOR_YELLOW); // "거래할 수 없는 아이템입니다."
								return;
							}
						}
						else
						{
							for(int i = 0; i < MAX_TRADE; i++)
							{
								if(g_pMainPlayer->m_pItemTrade[i].GetID()==0)
								{
									// Process //

//									sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 거래...
									CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
									if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
									{
										SetItemPacket(&ItemPickup, 46, i, 0, 0, 0);
										g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
										return;
									}
									else
									{
										InvReCallItem(ITEM_LARGE);
										DisplayMessageAdd(g_Message[ETC_MESSAGE1417].szMessage, TEXT_COLOR_YELLOW); // "거래할 수 없는 아이템입니다."
										return;
									}
								}
							}
						}
					}
				}
			}
			else
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					ITEM_NATIVE_INFO ItemNativeInfo;
					ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
					ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

					if(ItemNativeInfo.eSlotID == ITEM_NATIVE_INV_SMALL)
					{
						CItem cItem;
						memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));

						if(pQuantityWnd->GetActive() == FALSE)
						{
							if(cItem.GetQuantity() > 1 )
							{
								pQuantityWnd->OpenWnd(__QUANTITY_TRADE__);
								pQuantityWnd->m_byZipCode = nIndex;	
								return;
							}
							else
							{
								BYTE byCnt = cItem.GetQuantity();
								g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADE,nIndex,1,byCnt);	
								pQuantityWnd->m_bReq = TRUE;
								return;
							}
						}
						else
						{
							pQuantityWnd->SetActive(FALSE);
						}
					}						
				}
				else
				{
					if(g_pMainPlayer->m_MouseItem.GetID()!=0)				
					{
						CChatWnd* pChatWnd = CChatWnd::GetInstance();

						if(g_pGVDungeon->bChatMode)						
							ChatModeOff(INPUT_BUFFER_19);

						if(pChatWnd->GetActive()==TRUE)					
							pChatWnd->SetActive(FALSE);						
						
						g_pInputManager->ClearInput(INPUT_BUFFER_19);

						// Quantity Wnd //
						if(pQuantityWnd->GetActive() == FALSE)
						{
							if(g_pMainPlayer->m_MouseItem.GetQuantity() > 1)
							{
//								sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 거래...
								CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
								if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
								{
									pQuantityWnd->OpenWnd(__QUANTITY_TRADE__);
									pQuantityWnd->m_byZipCode = nIndex;
								}
								else
								{
									InvReCallItem(ITEM_SMALL);
									DisplayMessageAdd(g_Message[ETC_MESSAGE1417].szMessage, TEXT_COLOR_YELLOW); // "거래할 수 없는 아이템입니다."
								}
							}
							else
							{
//								sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 거래...
								CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
								if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
								{
									CTDS_ITEM_PICKUP ItemPickup;	
									ItemPickup.bSectionNum	= 1;
									ItemPickup.i64ItemID	= 0;
									ItemPickup.dwMoney		= g_pMainPlayer->m_MouseItem.GetQuantity();	
									
									SetItemPacket(&ItemPickup, 47, nIndex, 0, 0, 0);					
									g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

									pQuantityWnd->m_bReq = TRUE;
								}
								else
								{
									InvReCallItem(ITEM_SMALL);
									DisplayMessageAdd(g_Message[ETC_MESSAGE1417].szMessage, TEXT_COLOR_YELLOW); // "거래할 수 없는 아이템입니다."
								}
							}						
						}
						else
						{
							pQuantityWnd->SetActive(FALSE);
						}
						
						RECT rt;
						rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
						rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
						rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
						rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
						int nOrder	= pQuantityWnd->GetStartOrder()+2;						

						g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
						g_pInputManager->SetFocusInput(INPUT_BUFFER_5);															
					}
				}
			}
		}
		else if(g_Mouse.MousePos.x>=pItemTradeShopWnd->m_fPosX	&&
				g_Mouse.MousePos.x<=pItemTradeShopWnd->m_fPosX+CItemTradeShopWnd::GetInstance()->m_fMaxSizeWidth &&
				g_Mouse.MousePos.y>=pItemTradeShopWnd->m_fPosZ &&
				g_Mouse.MousePos.y<=pItemTradeShopWnd->m_fPosZ+CItemTradeShopWnd::GetInstance()->m_fMaxSizeHeight)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.Initialize();
			}
			else
			{
				if(InsertItem(&ItemPickup))
					return;
			}			
		}
		else
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.Initialize();
			}
		}
	}

	// Store Wnd //
	if(pStoreWnd->GetActive()==TRUE)
	{	
		if(	g_Mouse.MousePos.x>=pStoreWnd->m_fPosX &&
			g_Mouse.MousePos.x<=pStoreWnd->m_fPosX+CStoreWnd::GetInstance()->m_fMaxSizeWidth &&
			g_Mouse.MousePos.y>=pStoreWnd->m_fPosZ &&
			g_Mouse.MousePos.y<=pStoreWnd->m_fPosZ+CStoreWnd::GetInstance()->m_fMaxSizeHeight)
		{			
			if(pItemTradeShopWnd->GetActive()==TRUE)
			{
				if(GetType(nValue)==ITEM_SMALL)				
				{		
					// 아이템이 있으면 처리 //
					for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						// 아이템에서 있는 칸을 찾는다 //
						if(g_pMainPlayer->m_pInv_Small[i].m_wItemID == g_pMainPlayer->m_MouseItem.m_wItemID)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<100)
							{
								SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
								g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								return;
							}
						}
					}

					for(i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
						{
							SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							return;
						}
					}
				}
				else
				{
					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							return;
						}
					}
				}
				return;
			}

			if(pQuantityWnd->m_bReq==TRUE)
				return;
			
			bItemLand = FALSE;

			// Item 좌표 체크 //
			if(ItemCollision(STORE_WND)==FALSE)
			{
				if(InsertItem(&ItemPickup))
					return;

				bItemLand = TRUE;
			}
			
			if(bItemLand==FALSE)
			{				
				// Item 팔때.
				if(GetType(nValue)==ITEM_SMALL)
				{
					CChatWnd*	pChatWnd	= CChatWnd::GetInstance();

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_INV_SMALL)
						{						
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_ItemMoveManager.GetMouseItemID());

							if(!lpItemTable)
								return;

							int nMaxQuantity	= 1;
							nMaxQuantity		= lpItemTable->GetMaxQuantity();

							if(nMaxQuantity==1)
							{
								g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADERMONSTER_INV_SMALL, 0, 1, 1);
								pQuantityWnd->m_bReq = TRUE;
								return;
							}

							if(g_pGVDungeon->bChatMode)
								ChatModeOff(INPUT_BUFFER_19, 0, TRUE);
						
							g_pInputManager->ClearInput(INPUT_BUFFER_19);
							
							if(pGuildExWnd->GetActive()==TRUE)
								pGuildExWnd->SetActive(FALSE);

							if(pChatWnd->GetActive()==TRUE)
								pChatWnd->SetActive(FALSE);					
												
							// 창이 열린다 //
							pQuantityWnd->OpenWnd(__QUANTITY_STORE__);					

							RECT rt;
							rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
							rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
							rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
							rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
							int nOrder	= pQuantityWnd->GetStartOrder()+2;						

							g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
							g_pInputManager->SetFocusInput(INPUT_BUFFER_5);										

							return;
						}
					}
					else
					{
						CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());

						if(!lpItemTable)
							return;

						int nMaxQuantity = 1;

						nMaxQuantity = lpItemTable->GetMaxQuantity();					

						if(nMaxQuantity==1 || g_pMainPlayer->m_MouseItem.GetQuantity()==1)
						{
							ItemPickup.bInv		= 18;
							ItemPickup.bZipCode	= 0;
							ItemPickup.dwMoney	= g_pMainPlayer->m_MouseItem.GetQuantity();
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							pQuantityWnd->m_bReq = TRUE;

							return;
						}					

						if(g_pGVDungeon->bChatMode)
							ChatModeOff(INPUT_BUFFER_19, 0, TRUE);
					
						g_pInputManager->ClearInput(INPUT_BUFFER_19);
						
						if(pGuildExWnd->GetActive()==TRUE)
							pGuildExWnd->SetActive(FALSE);

						if(pChatWnd->GetActive()==TRUE)
							pChatWnd->SetActive(FALSE);					
											
						// 창이 열린다 //
						pQuantityWnd->OpenWnd(__QUANTITY_STORE__);					

						RECT rt;
						rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
						rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
						rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
						rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
						int nOrder	= pQuantityWnd->GetStartOrder()+2;						

						g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
						g_pInputManager->SetFocusInput(INPUT_BUFFER_5);										

						return;
					}					
				}
				else if((nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)	||
						(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)		||
						(nValue==ITEM_GUARDIAN_INDEX) ||
						(nValue==ITEM_BAG_INDEX))
				{
					/*if(pStoreWnd->m_byStoreType==STORE_TYPE_SUSPZO)
					{
						SetItemPacket(&ItemPickup, 10, 0, 0, 0, 0);
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						return;
					}*/					
					
					if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)
					{
						ItemPickup.bInv	= 34;
					}
					else if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
					{
						ItemPickup.bInv	= 36;
					}
					else if(nValue==ITEM_GUARDIAN_INDEX)
					{
						ItemPickup.bInv = 83;
					}
					else if(nValue==ITEM_BAG_INDEX)
					{
						ItemPickup.bInv = 84;
					}

					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADERMONSTER_INV_LARGE, 0, 1, 1);
					}
				}				
			}
		}
		else
		{	
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.Initialize();
			}
			
			if(g_pMainPlayer->m_MouseItem.GetType()==ITEM_LARGE)
			{
				for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
					{
						SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						return;
					}
				}
			}
			else if(g_pMainPlayer->m_MouseItem.GetType()==ITEM_SMALL)
			{			
				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
					{
						SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						return;
					}
				}
			}			
		}
	}

	// Bank Wnd //
	if(pBankWnd->GetActive()==TRUE)
	{		
		if(	g_Mouse.MousePos.x>=pBankWnd->m_fPosX+4 &&
			g_Mouse.MousePos.x<=pBankWnd->m_fPosX+242 &&
			g_Mouse.MousePos.y>pBankWnd->m_fPosZ+50 &&
			g_Mouse.MousePos.y<pBankWnd->m_fPosZ+400)		
		{	
			bItemLand = FALSE;

			if(pQuantityWnd->m_bReq==TRUE)
				return;
			
			// Item 좌표 체크 //
			if(ItemCollision(BANK_WND)==FALSE)
			{
				if(InsertItem(&ItemPickup))
					return;

				bItemLand = TRUE;
			}

			if(bItemLand==FALSE)
			{
				if(!pQuantityWnd->GetActive())
				{	
					if(GetType(nValue)==ITEM_SMALL)					
					{
						CChatWnd* pChatWnd = CChatWnd::GetInstance();

						int nPosX		= (g_Mouse.MousePos.x-(int)pBankWnd->m_fPosX-4)/34;
						int nPosY		= (g_Mouse.MousePos.y-(int)pBankWnd->m_fPosZ-50)/34;
						int nItemIndex	= nPosY*7+nPosX;

						if(nItemIndex>=MAX_BANK_SMALL_POOL)
						{
							for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
									g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
									return;
								}
							}														
							return;
						}
						else
						{
							pQuantityWnd->m_byZipCode = nItemIndex;						
						}

						if(g_pGVDungeon->bChatMode)
							ChatModeOff(INPUT_BUFFER_19);
						
						if(pGuildExWnd->GetActive()==TRUE)
							pGuildExWnd->SetActive(FALSE);

						if(pChatWnd->GetActive()==TRUE)
							pChatWnd->SetActive(FALSE);
						
						g_pInputManager->ClearInput(INPUT_BUFFER_19);

						if(pQuantityWnd->GetActive()==FALSE)
						{
							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								ITEM_NATIVE_INFO ItemNativeInfo;
								ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
								ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

								if( !g_ItemMoveManager.IsSrcToDestCorrect(ItemNativeInfo.eSlotID,ITEM_NATIVE_BANK_SMALL) )
								{
									return;
								}

								CItem cItem;
								memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));

								BYTE byCnt = cItem.GetQuantity();

								if(cItem.GetQuantity() > 1)
								{
									if(ItemNativeInfo.eSlotID == ITEM_NATIVE_BANK_SMALL)
									{
										g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_BANK_SMALL,nItemIndex,1,byCnt);
										pQuantityWnd->m_bReq = TRUE;
									}
									else
									{
										pQuantityWnd->OpenWnd(__QUANTITY_BANKINSERT__);	
									}
								}
								else
								{
									g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_BANK_SMALL,nItemIndex,1,byCnt);
									pQuantityWnd->m_bReq = TRUE;
								}
							}
							else
							{
								if(g_pMainPlayer->m_MouseItem.GetQuantity()>1)
								{
//									sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 창고...
									CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
									if(lpItemTalbe && lpItemTalbe->GetMovable() != 2)
									{
										if(g_bSmallBankChk)
										{
											CTDS_ITEM_PICKUP ItemPickup;	
											ItemPickup.bSectionNum	= 1;
											ItemPickup.i64ItemID	= 0;
											ItemPickup.bInv			= 25;
											ItemPickup.dwMoney		= g_pMainPlayer->m_MouseItem.GetQuantity();
											ItemPickup.bZipCode		= nItemIndex;				
											g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
											pQuantityWnd->m_bReq = TRUE;
										}
										else
										{
											pQuantityWnd->OpenWnd(__QUANTITY_BANKINSERT__);
										}
									}
									else
									{
										InvReCallItem(ITEM_SMALL);
										DisplayMessageAdd(g_Message[ETC_MESSAGE1418].szMessage, TEXT_COLOR_YELLOW); // "이동이 불가능한 아이템입니다."
									}
								}
								else
								{
									//sif(g_pMainPlayer->m_pBank_Small[nItemIndex].m_wItemID==0)

//									sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 창고...
									CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
									if(lpItemTalbe && lpItemTalbe->GetMovable() != 2)
									{
										CTDS_ITEM_PICKUP ItemPickup;	
										ItemPickup.bSectionNum	= 1;
										ItemPickup.i64ItemID	= 0;
										ItemPickup.bInv			= 25;
										ItemPickup.dwMoney		= g_pMainPlayer->m_MouseItem.GetQuantity();
										ItemPickup.bZipCode		= nItemIndex;				
										g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
										pQuantityWnd->m_bReq = TRUE;
									}
									else
									{
										InvReCallItem(ITEM_SMALL);
										DisplayMessageAdd(g_Message[ETC_MESSAGE1418].szMessage, TEXT_COLOR_YELLOW); // "이동이 불가능한 아이템입니다."
									}
								}
							}														
						}
						else
						{
							pQuantityWnd->SetActive(FALSE);
						}

						RECT rt;
						rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
						rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
						rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
						rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
						int nOrder	= pQuantityWnd->GetStartOrder()+2;						

						g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
						g_pInputManager->SetFocusInput(INPUT_BUFFER_5);
												
						// 재조정 //	
						return;
					}					
					else if(GetType(nValue)==ITEM_LARGE)
					{
//						if(g_pMainPlayer->m_MouseItem.GetID()!=0)
//						{
							// Item 검사 //
							int nItemChk	= pBankWnd->m_nCurScrollPos*7;
							int nPosX		= (g_Mouse.MousePos.x-pBankWnd->m_fPosX-4)/34;
							int nPosY		= (g_Mouse.MousePos.y-pBankWnd->m_fPosZ-50)/68;
							int nIndex		= (nItemChk+nPosX)+(nPosY*7);

							if(nIndex>=MAX_BANK_LARGE_POOL || nItemChk+35<=nIndex)
							{
								for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
									{
										SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
										g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
										return;
									}
								}
								
								SetItemPacket(&ItemPickup, 10, 0, 0, 0, 0);
								ItemPickup.fItemX	= g_Mouse.v3Mouse.x;
								ItemPickup.fItemY	= g_Mouse.v3Mouse.y;
								ItemPickup.fItemZ	= g_Mouse.v3Mouse.z;
								g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
								return;
							}

							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_BANK_LARGE,nIndex,1,1);
							}

//							sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 창고...
							CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
							BOOL bItemMovealbe = FALSE;
							if(lpItemTalbe && lpItemTalbe->GetMovable() != 2)
								bItemMovealbe = TRUE;
							if(g_pMainPlayer->m_pBank_Large[nIndex].m_wItemID==0 && bItemMovealbe)
							{
								SetItemPacket(&ItemPickup, 24, nIndex, 0, 0, 0);
								bItemActive	= TRUE;
							}
							else
							{
								// 아이템이 있으면 처리 //
								if(bItemMovealbe)
								{
									for(int i = 0; i < MAX_BANK_LARGE_POOL; i++)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pBank_Large[i].m_wItemID==0)
										{
											SetItemPacket(&ItemPickup, 24, i, 0, 0, 0);
											bItemActive	= TRUE;
											break;
										}
									}
								}
								else
								{
									bItemActive	= FALSE;
									DisplayMessageAdd(g_Message[ETC_MESSAGE1418].szMessage, TEXT_COLOR_YELLOW); // "이동이 불가능한 아이템입니다."
								}

								if(bItemActive==FALSE)
								{
									for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
										{		
											SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
											bItemActive	= TRUE;
											break;
										}
									}
									
									if(bItemActive==FALSE && bItemMovealbe)
									{
										bItemActive = TRUE;
										SetItemPacket(&ItemPickup, 10, 0, 0, 0, 0);
									}								
								}																
							}
//						}
					}
					else if(nValue==ITEM_DIVIDE_ID_ZODIAC)					
					{
						CChatWnd* pChatWnd = CChatWnd::GetInstance();

						if(g_pGVDungeon->bChatMode)
							ChatModeOff(INPUT_BUFFER_19);

						if(pGuildExWnd->GetActive()==TRUE)
							pGuildExWnd->SetActive(FALSE);
						
						if(pChatWnd->GetActive()==TRUE)
							pChatWnd->SetActive(FALSE);
						
						g_pInputManager->ClearInput(INPUT_BUFFER_19);

						if(pQuantityWnd->GetActive()==FALSE)
							pQuantityWnd->OpenWnd(__QUANTITY_BANKINSERT__);
						else
							pQuantityWnd->SetActive(FALSE);												

						int nPosX		= (g_Mouse.MousePos.x-(int)pBankWnd->m_fPosX-4)/34;
						int nPosY		= (g_Mouse.MousePos.y-(int)pBankWnd->m_fPosZ-50)/34;
						int nItemIndex	= nPosY*7+nPosX;

						if(nItemIndex>=MAX_BANK_SMALL_POOL)
							pQuantityWnd->m_byZipCode = 0;
						else
							pQuantityWnd->m_byZipCode = nItemIndex;						

						RECT rt;
						rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
						rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
						rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
						rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
						int nOrder	= pQuantityWnd->GetStartOrder()+2;						

						g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
						g_pInputManager->SetFocusInput(INPUT_BUFFER_5);												
						// 재조정 //	
						return;
					}	

					if(bItemActive==TRUE)
					{
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						return;
					}
				}
			}
		}
		else if(g_Mouse.MousePos.x>=pBankWnd->m_fPosX &&
				g_Mouse.MousePos.x<=pBankWnd->m_fPosX+CBankWnd::GetInstance()->m_fMaxSizeWidth &&
				g_Mouse.MousePos.y>pBankWnd->m_fPosZ &&
				g_Mouse.MousePos.y<pBankWnd->m_fPosZ+CBankWnd::GetInstance()->m_fMaxSizeHeight)
		{
			if(InsertItem(&ItemPickup))
				return;
		}
		else
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
			}
			else
			{
				if(g_pMainPlayer->m_MouseItem.GetType()==ITEM_LARGE)
				{
					for(int i = 0; i < MAX_BANK_LARGE_POOL; i++)
					{
						// 아이템에서 있는 칸을 찾는다 //
						if(g_pMainPlayer->m_pBank_Large[i].m_wItemID==0)
						{
							SetItemPacket(&ItemPickup, 24, i, 0, 0, 0);
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							bItemActive	= TRUE;
							
							// "아이템을 창고로 이동합니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE252].szMessage, 0xFFFFC309); 
							return;
						}
					}
				}	
				else if(g_pMainPlayer->m_MouseItem.GetType()==ITEM_SMALL)
				{
					CTDS_ITEM_PICKUP ItemPickup;	
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;
					ItemPickup.bInv			= 25;
					ItemPickup.dwMoney		= g_pMainPlayer->m_MouseItem.GetQuantity();
					ItemPickup.bZipCode		= 0;				
					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					pQuantityWnd->m_bReq = TRUE;
				}
			}			
		}
	}

	// 던전 운영 Wnd //
	if(pDungeonManagerWnd->GetActive())
	{
		if(pQuantityWnd->m_bReq==TRUE)
			return;
		if(pDungeonManagerWnd->m_pDungeon->m_bSiege)	return;
		
		bItemLand = FALSE;

		// 버릴때 수량창 나오기.
		if(ItemCollision(DUNGEONMANAGER_WND)==FALSE)
		{
			if(InsertItem(&ItemPickup))
					return;

			bItemLand = TRUE;
		}

		if(bItemLand==FALSE)
		{
			if(nValue==ITEM_MATERIALEND_INDEX)
			{
				switch(g_pMainPlayer->m_MouseItem.GetID())
				{
				case __ITEM_METERIALS_FIRE_PROPERTY_INDEX__:
				case __ITEM_METERIALS_ICE_PROPERTY_INDEX__:
				case __ITEM_METERIALS_LIGHT_PROPERTY_INDEX__:
					if(g_Mouse.MousePos.x>=pDungeonManagerWnd->m_fPosX+11 
						&& g_Mouse.MousePos.x<=pDungeonManagerWnd->m_fPosX+43 
						&& g_Mouse.MousePos.y>=pDungeonManagerWnd->m_fPosZ+173 
						&& g_Mouse.MousePos.y<=pDungeonManagerWnd->m_fPosZ+220)
					{
						//OpenQuantityWnd(__QUANTITY_DUNGEONMANAGEINSERT__);
						ItemPickup.dwMoney	= 1;					
						SetItemPacket(&ItemPickup, 63, 0, 0, 0, 0);	
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						return;
					}
				}
				
			}
			else if(nValue==ITEM_GUARDIAN_INDEX)
			{				
				if(g_Mouse.MousePos.x>=pDungeonManagerWnd->m_fPosX+136 
					&& g_Mouse.MousePos.x<=pDungeonManagerWnd->m_fPosX+168 
					&& g_Mouse.MousePos.y>=pDungeonManagerWnd->m_fPosZ+173 
					&& g_Mouse.MousePos.y<=pDungeonManagerWnd->m_fPosZ+250)		
				{
					SetItemPacket(&ItemPickup, 42, 0, 0, 0, 0);
					g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					return;
				}
			}
		}
	}

	// 아이템 업그레이드 //	
	if(ItemChkUpgrade(&ItemPickup)==TRUE)
		return;	

	// 아이템 제조 //
	if(ItemChkMaking(&ItemPickup)==TRUE)
		return;

	// 노점상 //
	if(ItemChkPlayerShop(&ItemPickup)==TRUE)
		return;
		
	// Belt //
	if(	g_Mouse.MousePos.x>=727 && g_Mouse.MousePos.x<=1024 &&
		g_Mouse.MousePos.y>=733 && g_Mouse.MousePos.y<=768)
	{					
		bItemLand = FALSE;

		// Belt //
		if(BeltCollision()==FALSE)
			bItemLand = TRUE;
			
		if(bItemLand==FALSE)
		{
			// 물약 //						
			int nPosX = (g_Mouse.MousePos.x-727)/37;

			if(nPosX<MAX_BELT_POOL)
			{
				int nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;
				
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					WORD wMouseID = g_ItemMoveManager.GetMouseItemID();
					if(!wMouseID)
						return;
					nValue =  wMouseID / ITEM_DISTRIBUTE;	
				}
												
				if(GetType(nValue)==ITEM_SMALL)
				{
					if(g_pMainPlayer->m_pBelt[nPosX].m_wItemID==0)
					{
						SetItemPacket(&ItemPickup, 3, nPosX, 0, 0, 0);
					}
					else
					{
						BOOL bChk = FALSE;

						if(g_pMainPlayer->m_pBelt[nPosX].m_Item_Supplies.bQuantity<100)
						{
							SetItemPacket(&ItemPickup, 3, nPosX, 0, 0, 0);
						}
						else
						{
							for(int i = 0; i < MAX_BELT_POOL; i++)
							{
								if(g_pMainPlayer->m_pBelt[i].m_wItemID==0)
								{
									SetItemPacket(&ItemPickup, 3, i, 0, 0, 0);
									bChk = TRUE;
									break;
								}
								else
								{
									if(g_pMainPlayer->m_pBelt[nPosX].m_Item_Supplies.bQuantity<100)
									{
										SetItemPacket(&ItemPickup, 3, nPosX, 0, 0, 0);
										bChk = TRUE;
										break;
									}
								}
							}

							for(i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
									g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
									return;
								}
							}

							// 물약 바닥에 버리기 //
							if(bChk==FALSE)
							{
								SetItemPacket(&ItemPickup, 4, 0, 0, 0, 0);
							}
						}
					}
					
					g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						ITEM_NATIVE_INFO ItemNativeInfo;
						ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
						ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

						CItem cItem;
						memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));
						BYTE byCnt = cItem.GetQuantity();
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_BELT,nPosX,1,byCnt);
					}

					return;
				}				
			}			
		}
	}

	// Guardian // 가디언 소환 했을때 나오는 창 
	if(	g_Mouse.MousePos.x>=93	&&
		g_Mouse.MousePos.x<=125 &&
		g_Mouse.MousePos.y>=76	&&
		g_Mouse.MousePos.y<=108)
	{
		if(CUserInterface::GetInstance()->m_bGuardian==TRUE)
		{
			bItemLand = FALSE;

			// Guardian //
			if(GuardianCollision()==FALSE)
				bItemLand = TRUE;

			if(bItemLand==FALSE)
			{
				if(nValue==ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX))
				{
					if(nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX)
					{
						if(g_pMainPlayer->m_MouseItem.GetID() < ITEM_SKILL_BOOK_START)
						{
							for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
									g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
									return;
								}
							}
						}
					}

					SetItemPacket(&ItemPickup, 40, 0, 0, 0, 0);
					g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						ITEM_NATIVE_INFO ItemNativeInfo;
						ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
						ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

						CItem cItem;
						memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));

						BYTE byCnt = cItem.GetQuantity();

						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_GUARDIAN,0,1,byCnt,0,0);
					}

					return;
				}
				else
				{
					for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
						{
							SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							return;
						}
					}
				}
			}			
		}
	}

	//###################################################
	// Drop Item
	//###################################################
	if(!pQuantityWnd->GetActive())
	{
		// 버릴때 수량창 나오기 //
		if(pItemTradeShopWnd->GetActive()==TRUE)
		{
			if(GetType(nValue)==ITEM_SMALL)
			{		
				// 아이템이 있으면 처리 //
				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					// 아이템에서 있는 칸을 찾는다 //
					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<100)
						{
							SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							return;
						}
					}
				}

				for(i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
					{
						SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						return;
					}
				}
			}
			else
			{
				for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
					{
						SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						return;
					}
				}
			}
		}
		else
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if( !g_ItemMoveManager.IsSrcToDestCorrect((ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc()
					,ITEM_NATIVE_TILE) )
				{
					return;
				}
			}

			if(GetType(nValue)==ITEM_SMALL)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_INV_SMALL)
					{
						CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_ItemMoveManager.GetMouseItemID());

						if(!lpItemTable)
							return;

						int nMaxQuantity	= 1;

						nMaxQuantity = lpItemTable->GetMaxQuantity();

						if(g_pMainPlayer->GetStatus()==UNIT_STATUS_DEAD)
						{
						
						}
						else
						{
							if(nMaxQuantity == 1)
							{
								g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TILE,0xff,1,1);
							}
							else
							{
								OpenQuantityWnd(__QUANTITY_THROW__);
								return;
							}
						}
					}
				}
				else
				{					
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());

					if(!lpItemTable)
						return;

					int nMaxQuantity	= 1;

					nMaxQuantity = lpItemTable->GetMaxQuantity();				

					if(g_pMainPlayer->GetStatus()==UNIT_STATUS_DEAD)
					{
						for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
								g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								return;
							}
						}
					}
					else
					{
//						sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 드롭...
						CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
						if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
						{
							if(nMaxQuantity==1 || g_pMainPlayer->m_MouseItem.GetQuantity()==1)
							{
								SetItemPacket(&ItemPickup, 28, 0, 0, 0, 0);
								ItemPickup.fItemX	= g_Mouse.v3Mouse.x;
								ItemPickup.fItemY	= g_Mouse.v3Mouse.y;
								ItemPickup.fItemZ	= g_Mouse.v3Mouse.z;
								ItemPickup.dwMoney	= 1;
								g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							}
							else
							{
								CQuantityWnd::GetInstance()->m_fItemX = g_Mouse.v3Mouse.x;
								CQuantityWnd::GetInstance()->m_fItemY = g_Mouse.v3Mouse.y;
								CQuantityWnd::GetInstance()->m_fItemZ = g_Mouse.v3Mouse.z;					
								OpenQuantityWnd(__QUANTITY_THROW__);
							}
						}
						else
						{
							InvReCallItem(ITEM_SMALL);
							DisplayMessageAdd(g_Message[ETC_MESSAGE1416].szMessage, TEXT_COLOR_YELLOW); // "버릴수 없는 아이템입니다."
						}

					}												
					/*if(nValue==ITEM_ZODIAC_INDEX)
					{
						SetItemPacket(&ItemPickup, 28, 0, 0, 0, 0);
						ItemPickup.dwMoney	= 1;
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					}
					else*/	
				}
			}
			else
			{					
				if(g_pMainPlayer->GetStatus()==UNIT_STATUS_DEAD)
				{
					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							return;
						}
					}
				}				
				else
				{
//					sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 드롭...
					CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
					if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
					{
						SetItemPacket(&ItemPickup, 10, 0, 0, 0, 0);
						ItemPickup.fItemX	= g_Mouse.v3Mouse.x;
						ItemPickup.fItemY	= g_Mouse.v3Mouse.y;
						ItemPickup.fItemZ	= g_Mouse.v3Mouse.z;
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );

						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TILE,0xff,1,1);
						}
					}
					else
					{
						InvReCallItem(ITEM_LARGE);
						DisplayMessageAdd(g_Message[ETC_MESSAGE1416].szMessage, TEXT_COLOR_YELLOW); // "버릴수 없는 아이템입니다."
					}
				}				
			}
		}
	}	
}


void ItemInfoRender()
{ 					
	if(g_pMainPlayer->m_dwItemInfoType==2)
	{
		if(g_pMainPlayer->m_MouseItem.GetID()==0)
		{
			CUserInterface* pUserInterface = CUserInterface::GetInstance();

			if(pUserInterface->PosCollision(g_Mouse.MousePos.x-5, g_Mouse.MousePos.x+5, g_Mouse.MousePos.y-5, g_Mouse.MousePos.y+5)==FALSE)				
				return;

			CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_InfoItem.GetID());

			if(lpItemTalbe)
			{				
				int		iOption = g_pMainPlayer->m_InfoItem.GetOptionCount();
				DWORD	dwColor = 0xffffffff;


				// 스몰포션 아이템 노란색 표시 수정 김영대 2005.02.06
				WORD wKind = GetItemKind(lpItemTalbe->GetID());
				//switch( lpItemTalbe->bCode_Type )
				switch( wKind )
				{
				case ITEM_KIND_WEAPON:
					{
						switch(lpItemTalbe->BaseItem_Weapon.byDropBox)
						{
						case 1:
							if( iOption > 0 )
								dwColor = TEXT_COLOR_BLUE;
							break;
						case 2:
							dwColor = TEXT_COLOR_GREEN;
							break;
						case 3:
							dwColor = TEXT_COLOR_YELLOW;
							break;
						}
					}
					break;
				case ITEM_KIND_AROMR:
					{
						switch(lpItemTalbe->BaseItem_Armor.byDropBox)
						{
						case 1:
							if( iOption > 0 )
								dwColor = TEXT_COLOR_BLUE;
							break;
						case 2:
							dwColor = TEXT_COLOR_GREEN;
							break;
						case 3:
							dwColor = TEXT_COLOR_YELLOW;
							break;
						}
					}
					break;
				}
		
				// Item이 있을 경우만 //
				int nSize = lstrlen(lpItemTalbe->szItemName_Kor);
				RenderFont(lpItemTalbe->szItemName_Kor, g_Mouse.MousePos.x + 18, g_Mouse.MousePos.x + 18+ nSize*7, g_Mouse.MousePos.y + 5, g_Mouse.MousePos.y + 19, __ORDER_ITEM_DESC__+1, dwColor);
				CInterface::GetInstance()->RenderInfoBox(g_Mouse.MousePos.x + 10, g_Mouse.MousePos.y, nSize*8+5, 19, __ORDER_ITEM_DESC__);
			}		
			else
			{
				if(g_pMainPlayer->m_InfoItem.GetID()==ITEM_MONEY_INDEX*ITEM_DISTRIBUTE)
				{
					char szInfo[0xff] = {0,};
					wsprintf(szInfo, g_Message[ETC_MESSAGE866].szMessage, g_pMainPlayer->m_InfoItem.m_Item_Money.dwMoney);//"%u Karz"

					// Item이 있을 경우만 //
					int nSize = lstrlen(szInfo);

					RenderFont(szInfo, g_Mouse.MousePos.x+18, g_Mouse.MousePos.x+18+nSize*8, g_Mouse.MousePos.y+5, g_Mouse.MousePos.y+19, __ORDER_ITEM_DESC__+1);
					CInterface::GetInstance()->RenderInfoBox(g_Mouse.MousePos.x + ITEMINFO_RENDER_POSITION, g_Mouse.MousePos.y, nSize*8+5, 19, __ORDER_ITEM_DESC__);
				}
			}			
		}
	}
	else if(g_Mouse.MousePos.x>=727 && g_Mouse.MousePos.x<=1024 && g_Mouse.MousePos.y>=733 && g_Mouse.MousePos.y<=768)
	{			
		if( g_pThisDungeon->IsStadium() && g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
			return;

		CItem*	pItemInfo;
		DWORD	pdwTextColor[50];
		
		CUserInterface* pUserInterface = CUserInterface::GetInstance();

		if(g_Mouse.bLDown==TRUE)
			return;
		else if(pUserInterface->PosCollision(727, 1024, 733, 768)==FALSE)	
			return;

		memset(pdwTextColor, 0xff, sizeof( DWORD ) * 50);	

		const int nPosX	= (g_Mouse.MousePos.x-727)/37;

		if( MAX_BELT_POOL > nPosX 
		&& 0 <= nPosX)//: 050110 hwoarang 
		{		
			if(g_pMainPlayer->m_pBelt[nPosX].m_wItemID==0)
				return;
			else if(g_pMainPlayer->m_pBelt[nPosX].GetType()!=ITEM_SMALL)
				return;
			if(nPosX>MAX_BELT_POOL)
				return;
			
			pItemInfo	= &g_pMainPlayer->m_pBelt[nPosX];
					
			CInterface::GetInstance()->ItemInfoRender(pItemInfo, FALSE);		
		}
	}	
}


void InputQuantity()
{
	CQuantityWnd* pQuantityWnd	= CQuantityWnd::GetInstance();

	DWORD		dwCnt	= 10;
	int			nIdex	= 0;
	__int64		nMoney	= _atoi64(pQuantityWnd->m_szMoney);	

	if(pQuantityWnd->m_bQuantityType==__QUANTITY_MONEY__)
	{
		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex	= 0;
		}
		else
		{
			if(g_pMainPlayer->m_dwMoney==0)
			{
				memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
				pQuantityWnd->m_byDrawIndex	= 0;
			}
			else
			{
				if(g_pMainPlayer->m_dwMoney>=nMoney)
				{											
					int nSize = lstrlen(pQuantityWnd->m_szMoney);

					_ui64toa(nMoney, pQuantityWnd->m_szMoney, 10);
					pQuantityWnd->m_byDrawIndex	= nSize;
				}
				else
					_i64toa(g_pMainPlayer->m_dwMoney, pQuantityWnd->m_szMoney, 10);	
			}			
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_TRADEMONEY__)
	{
		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex	= 0;
		}
		else
		{
			if(g_pMainPlayer->m_dwMoney==0)
			{
				memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
				pQuantityWnd->m_byDrawIndex	= 0;
			}
			else
			{
				if(g_pMainPlayer->m_dwMoney>=nMoney)
				{											
					int nSize = lstrlen(pQuantityWnd->m_szMoney);

					_ui64toa(nMoney, pQuantityWnd->m_szMoney, 10);
					pQuantityWnd->m_byDrawIndex	= nSize;
				}
				else
					_i64toa(g_pMainPlayer->m_dwMoney, pQuantityWnd->m_szMoney, 10);	
			}
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_PLAYERSHOPSELLCNT__)
	{
		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex = 0;			
		}
		else
		{
			CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
			
			int nCount	= pPlayerShopWnd->m_sPlayerShopInfo.cItem[pPlayerShopWnd->m_bZipCode].GetQuantity();

			if(nCount<nMoney)
			{							
				for(int i = 0; i < 5; i++)
				{
					if(nCount>=dwCnt)
					{
						nIdex++;
						dwCnt=dwCnt*10;
					}			
				}							
				_ui64toa(nCount, pQuantityWnd->m_szMoney, 10);
				pQuantityWnd->m_byDrawIndex	= nIdex+1;				
			}
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_BUY__)
	{
		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex = 0;			
		}
		else
		{
			CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();			
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(pStoreWnd->m_wSuppliesId);

			int nMaxQuantity	= 1;

			if(lpItemTable)
			{
				nMaxQuantity = lpItemTable->GetMaxQuantity();			
								
				int nCount	= nMaxQuantity;

				if(nCount<nMoney)
				{							
					for(int i = 0; i < 5; i++)
					{
						if(nCount>=dwCnt)
						{
							nIdex++;
							dwCnt=dwCnt*10;
						}			
					}							
					_ui64toa(nCount, pQuantityWnd->m_szMoney, 10);
					pQuantityWnd->m_byDrawIndex	= nIdex+1;				
				}
			}
		}		
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_THROW__)
	{
		int nCount = 0;

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			ITEM_NATIVE_INFO ItemNativeInfo;
			ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
			ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

			CItem cItem;
			memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));
			
			nCount = cItem.GetQuantity();
		}
		else
		{
			nCount	= g_pMainPlayer->m_MouseItem.GetQuantity();		
		}		

		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex		= 0;
			g_pInputManager->ClearInput(INPUT_BUFFER_5);
			
		}
		else if(nCount<nMoney)
		{							
			for(int i = 0; i < 5; i++)
			{
				if(nCount>=(int)dwCnt)
				{
					nIdex++;
					dwCnt=dwCnt*10;
				}
			}							
			_ui64toa(nCount, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nIdex+1;
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_BANKINSERT__)
	{
		int nCount = 0;

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			ITEM_NATIVE_INFO ItemNativeInfo;
			ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
			ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

			CItem cItem;
			memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));
			
			nCount = cItem.GetQuantity();
		}
		else
		{
			nCount = g_pMainPlayer->m_MouseItem.GetQuantity();
		}
		
		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex		= 0;
			g_pInputManager->ClearInput(INPUT_BUFFER_5);
		}
		else if(nCount<nMoney)
		{
			for(int i = 0; i < 5; i++)
			{
				if(nCount>=(int)dwCnt)
				{
					nIdex++;
					dwCnt=dwCnt*10;
				}			
			}							
			_i64toa(nCount, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nIdex+1;
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_STORE__)
	{
		int nCount = 0;

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			ITEM_NATIVE_INFO ItemNativeInfo;
			ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
			ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

			CItem cItem;
			memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));
			
			nCount = cItem.GetQuantity();
		}
		else
		{
			nCount = g_pMainPlayer->m_MouseItem.GetQuantity();
		}		

		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex		= 0;
			g_pInputManager->ClearInput(INPUT_BUFFER_5);
			
		}
		else if(nCount<nMoney)
		{							
			for(int i = 0; i < 5; i++)
			{
				if(nCount>=(int)dwCnt)
				{
					nIdex++;
					dwCnt=dwCnt*10;
				}			
			}							
			_ui64toa(nCount, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nIdex+1;
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_FROMBANKTOINV__)
	{
		int nCount = 0;
		ITEM_NATIVE_INFO ItemNativeInfo;
		ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
		ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

		CItem cItem;
		memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{			
			nCount = cItem.GetQuantity();
		}
		else
		{
			nCount = g_pMainPlayer->m_MouseItem.GetQuantity();
		}
		
		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex		= 0;
			g_pInputManager->ClearInput(INPUT_BUFFER_5);
		}
		else if(nCount<nMoney)
		{
			for(int i = 0; i < 5; i++)
			{
				if(nCount>=(int)dwCnt)
				{
					nIdex++;
					dwCnt=dwCnt*10;
				}			
			}							
			_i64toa(nCount, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nIdex+1;
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_BNAKMONEYINSERT__)
	{
		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex	= 0;
		}
		else
		{
			if(g_pMainPlayer->m_dwMoney<(DWORD)nMoney)
			{							
				for(int i = 0; i < 20; i++)
				{
					if(g_pMainPlayer->m_dwMoney>=dwCnt)
					{
						nIdex++;
						dwCnt=dwCnt*10;
					}			
				}
				if(g_pMainPlayer->m_dwMoney==0)
				{
					memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
					pQuantityWnd->m_byDrawIndex	= 0;					
				}
				else
				{
					_i64toa(g_pMainPlayer->m_dwMoney, pQuantityWnd->m_szMoney, 10);
					pQuantityWnd->m_byDrawIndex		= nIdex+1;					
				}						
			}
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_BNAKMONEYPOP__)
	{
		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex	= 0;
		}
		else
		{
			if(g_pMainPlayer->m_dwBankMoney<(DWORD)nMoney)
			{							
				for(int i = 0; i < 20; i++)
				{
					if(g_pMainPlayer->m_dwBankMoney>=dwCnt)
					{
						nIdex++;
						dwCnt=dwCnt*10;
					}			
				}
				if(g_pMainPlayer->m_dwBankMoney==0)
				{
					memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
					pQuantityWnd->m_byDrawIndex	= 0;					
				}
				else
				{
					_i64toa(g_pMainPlayer->m_dwBankMoney, pQuantityWnd->m_szMoney, 10);
					pQuantityWnd->m_byDrawIndex		= nIdex+1;					
				}						
			}
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_DUNGEONMANAGEINSERT__)
	{		
		int nCount	= g_pMainPlayer->m_MouseItem.GetQuantity();

		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex		= 0;
			g_pInputManager->ClearInput(INPUT_BUFFER_5);
		}
		else if(nCount<nMoney)
		{
			for(int i = 0; i < 5; i++)
			{
				if(nCount>=(int)dwCnt)
				{
					nIdex++;
					dwCnt=dwCnt*10;
				}			
			}							
			_i64toa(nCount, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nIdex+1;
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_TRADE__)
	{
		int nCount = 0;

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			ITEM_NATIVE_INFO ItemNativeInfo;
			ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
			ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

			CItem cItem;
			memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));

			nCount = cItem.GetQuantity();
		}
		else
		{
			nCount = g_pMainPlayer->m_MouseItem.GetQuantity();
		}

		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex		= 0;
			g_pInputManager->ClearInput(INPUT_BUFFER_5);
		}
		else if(nCount<nMoney)
		{
			for(int i = 0; i < 5; i++)
			{
				if(nCount>=(int)dwCnt)
				{
					nIdex++;
					dwCnt=dwCnt*10;
				}			
			}
			
			_i64toa(nCount, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nIdex+1;
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_PLAYERSHOP__)
	{				
		if(MAX_MONEY_PLAYERSHOP >= nMoney)
		{											
			int nSize = lstrlen(pQuantityWnd->m_szMoney);
			_i64toa(nMoney, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nSize;
		}
		else
		{
			_i64toa(MAX_MONEY_PLAYERSHOP, pQuantityWnd->m_szMoney, 10);					
		}
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_PLAYERSHOPCNT__)
	{	
		int nCount = 0;

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			ITEM_NATIVE_INFO ItemNativeInfo;
			ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
			ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

			CItem cItem;
			memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));

			nCount = cItem.GetQuantity();
		}
		else
		{
			nCount = g_pMainPlayer->m_MouseItem.GetQuantity();
		}

		if(nCount>=nMoney)
		{											
			int nSize = lstrlen(pQuantityWnd->m_szMoney);

			_i64toa(nMoney, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nSize;
		}
		else
			_i64toa(nCount, pQuantityWnd->m_szMoney, 10);					
	}
	else if(pQuantityWnd->m_bQuantityType==__QUANTITY_DUNGEON__)
	{
		int	nCount	= g_pThisDungeon->GetMaxEntrancePay();

		if(nMoney==0)
		{
			memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
			pQuantityWnd->m_byDrawIndex		= 0;
			g_pInputManager->ClearInput(INPUT_BUFFER_5);
			
		}
		else if(nCount<nMoney)
		{							
			for(int i = 0; i < 5; i++)
			{
				if(nCount>=(int)dwCnt)
				{
					nIdex++;
					dwCnt=dwCnt*10;
				}
			}							
			_ui64toa(nCount, pQuantityWnd->m_szMoney, 10);
			pQuantityWnd->m_byDrawIndex	= nIdex+1;
		}
	}

	// 좌표 설정 //		
	g_pInputManager->GetInputRect(INPUT_BUFFER_5).left = 
		(LONG)pQuantityWnd->m_fPosX + 124 - (pQuantityWnd->m_byDrawIndex * 6) - 7 - (pQuantityWnd->m_byDrawIndex / 3 + 1 ) * 6;
	g_pInputManager->GetInputRect(INPUT_BUFFER_5).right = (LONG)pQuantityWnd->m_fPosX + 124;
}


void SendPickupItem(ITEM* pItem, BOOL bActive, BOOL bType)
{
	BOOL				bIVActive = FALSE;
	CTDS_ITEM_PICKUP	ItemPickup;	

	if(!pItem)
		return;

	ItemPickup.i64ItemID	= pItem->Item.m_Serial.i64Serial;
	ItemPickup.bSectionNum	= pItem->bSectionNum;

	int nVal = pItem->Item.m_wItemID/ITEM_DISTRIBUTE;				
	
	// 인벤토리가 열여있지 않다고 가정한다.
	if(nVal==ITEM_MONEY_INDEX)
	{
		SetItemPacket(&ItemPickup, 1, 0, 0, 0, 0);					
	}
	else if(nVal<=__MAX_ITEM_LARGE_INDEX__)
	{
		SetItemPacket(&ItemPickup, 1, 0, 0, 0, 0);
	}
	else
	{
		SetItemPacket(&ItemPickup, 2, 0, 0, 0, 0);
	}

	if(ItemPickup.bInv==1)
	{
		if(nVal==ITEM_MONEY_INDEX)
		{
			// 돈일 경우.
			SetItemPacket(&ItemPickup, 1, 0, 0, 0, 0);
			bIVActive = TRUE;
		}
		else
		{
			// Item 일 경우.
			if(bActive==FALSE)
			{
				for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
					{
						if(bType==FALSE)
						{
							if(g_bKeyItem_Large[i]==0)
							{
								SetItemPacket(&ItemPickup, 1, i, 0, 0, 0);
								bIVActive			= TRUE;
								g_bKeyItem_Large[i] = 1;
								break;
							}
						}
						else
						{
							SetItemPacket(&ItemPickup, 1, i, 0, 0, 0);
							bIVActive = TRUE;
							break;
						}						
					}
				}
				
				if(bIVActive==FALSE)
				{
					// MSG_ID : 53 ; 장착아이템 인벤토리가 가득찼습니다
					DisplayMessageAdd(g_Message[ETC_MESSAGE53].szMessage, 0xFFFFC309);	
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
			}
			else if(bActive==TRUE)
			{
				SetItemPacket(&ItemPickup, 5, 0, 0, 0, 0);
				bIVActive = TRUE;
			}
		}
	}				
	else if(ItemPickup.bInv==2)
	{
		if(bActive==FALSE)
		{
			if(pItem->Item.m_wItemID/ITEM_DISTRIBUTE==ITEM_SUPPLIES_INDEX)
			{
				BOOL bChk = FALSE;

//				LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(pItem->Item.m_wItemID);
				CBaseItem* lpItemTable = g_pItemTableHash->GetData(pItem->Item.m_wItemID);
				
				if(!lpItemTable)
					return;
					
				int nMaxQuantity = lpItemTable->GetMaxQuantity();				
					
				for(int i = 0; i < MAX_BELT_POOL; i++)
				{
					if(g_pMainPlayer->m_pBelt[i].m_wItemID==0)
					{						
						SetItemPacket(&ItemPickup, 71, i, 0, 0, 0);
						bChk = TRUE;
						break;
					}
					else
					{
						if(g_pMainPlayer->m_pBelt[i].GetID()==pItem->Item.m_wItemID)
						{
							if(g_pMainPlayer->m_pBelt[i].m_Item_Supplies.bQuantity<nMaxQuantity)
							{
								SetItemPacket(&ItemPickup, 71, i, 0, 0, 0);
								bChk = TRUE;
								break;
							}
						}
					}									
				}
				
				if(bChk)
				{
					g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					return;
				}									
			}

			for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
			{
				// 칸에서 아이템이 있는지 찾는다 //
				if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==pItem->Item.m_wItemID)
				{					
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Small[i].m_wItemID);

					int nMaxQuantity	= 1;

					if(!lpItemTable)
						return;
					
					nMaxQuantity = lpItemTable->GetMaxQuantity();					

					if(g_pMainPlayer->m_pInv_Small[i].GetQuantity()<nMaxQuantity)
					{
						SetItemPacket(&ItemPickup, 2, i, 0, 0, 0);
						bIVActive = TRUE;
						break;
					}								
				}
			}

			if(!bIVActive)
			{
				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{								
					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
					{									
						SetItemPacket(&ItemPickup, 2, i, 0, 0, 0);
						bIVActive = TRUE;
						break;
					}
				}
			}

			if(bIVActive==FALSE)
			{
				// MSG_ID : 54 ; 소비아이템 인벤토리가 가득찼습니다
				DisplayMessageAdd(g_Message[ETC_MESSAGE54].szMessage, 0xFFFFC309);	
				_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
			}
		}
		else if(bActive==TRUE)
		{
			SetItemPacket(&ItemPickup, 12, 0, 0, 0, 0);
			bIVActive = TRUE;
		}
	}

	if(bIVActive==TRUE)
	{
		g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
	}
}


//==============================================================//
// Item 이동 함수.												//
//==============================================================//
void Insert_MakingItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{
	BOOL	bAppearChk	= FALSE;
	BYTE	byPack		= pItem->GetQuantity()-byPackCnt;

	CItemMakingWnd* pItemMakingWnd = CItemMakingWnd::GetInstance();
	
	if(pItemMakingWnd->m_pMakingItem[bZipCode].GetID()==0)
	{
		if(byPack!=0)
		{
			pItem->SetQuantity(pItem->GetQuantity()-1);
			Insert_SmallInvItem(pItem, 0, byPack);
		}
		memcpy(&pItemMakingWnd->m_pMakingItem[bZipCode], pItem, sizeof(CItem));
		pItemMakingWnd->m_pMakingItem[bZipCode].SetQuantity(byPackCnt);
	}
	else
	{
		for(int i = 1; i < 4; i++)
		{
			if(pItemMakingWnd->m_pMakingItem[bZipCode].GetID()==0)
			{
				memcpy(&pItemMakingWnd->m_pMakingItem[bZipCode], pItem, sizeof(CItem));
				pItemMakingWnd->m_pMakingItem[bZipCode].SetQuantity(byPackCnt);
				bAppearChk = TRUE;
				break;
			}
		}

		if(byPack!=0 || !bAppearChk)
			Insert_SmallInvItem(pItem, 0, byPack+byPackCnt);
	}	
}


void Insert_UpgradeItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{	
	BOOL	bAppearChk	= FALSE;
	BYTE	byPack		= pItem->GetQuantity()-byPackCnt;
	
	CItemUpgradeWnd* pItemUpgradeWnd = CItemUpgradeWnd::GetInstance();

	if(pItemUpgradeWnd->m_pUpgradeItem[bZipCode].GetID()==0)
	{
		if(byPack!=0)
		{
			pItem->SetQuantity(pItem->GetQuantity()-1);
			Insert_SmallInvItem(pItem, 0, byPack);
		}
		memcpy(&pItemUpgradeWnd->m_pUpgradeItem[bZipCode], pItem, sizeof(CItem));
		pItemUpgradeWnd->m_pUpgradeItem[bZipCode].SetQuantity(byPackCnt);
	}
	else
	{
		for(int i = 1; i < 4; i++)
		{
			if(pItemUpgradeWnd->m_pUpgradeItem[bZipCode].GetID()==0)
			{
				memcpy(&pItemUpgradeWnd->m_pUpgradeItem[bZipCode], pItem, sizeof(CItem));
				pItemUpgradeWnd->m_pUpgradeItem[bZipCode].SetQuantity(byPackCnt);
				bAppearChk = TRUE;
				break;
			}
		}

		if(byPack!=0 || !bAppearChk)
			Insert_SmallInvItem(pItem, 0, byPack+byPackCnt);
	}
}

void Insert_MagicFieldArrayItem(CItem* pItem, BYTE byPackCnt)
{	
	if(!pItem)
		return;

	BOOL	bAppearChk	= FALSE;
	BYTE	byPack		= pItem->GetQuantity()-byPackCnt;
		
	CDungeonManagerWnd* pDungeonManagerWnd = CDungeonManagerWnd::GetInstance();
		
	if(pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.GetID()==0)
	{
		if(byPack!=0)
			Insert_SmallInvItem(pItem, 0, byPack);

		memcpy(&pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem, pItem, sizeof(CItem));
		pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.SetQuantity(byPackCnt);
	}
	else
	{
		if(pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.GetID()==pItem->GetID())
		{
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.GetID());			

			if(lpItemTable)
			{			
				int nMaxQuantity	= 1;
				
				nMaxQuantity = lpItemTable->GetMaxQuantity();			

				BYTE byCnt = nMaxQuantity - pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.GetQuantity();

				if(byCnt>=byPackCnt)
				{
					BYTE bQuantity = pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.GetQuantity();
					pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.SetQuantity(bQuantity+byPackCnt);
					byPackCnt	= 0;
					bAppearChk = TRUE;
				}
				else if(byCnt!=0)
				{
					BYTE bQuantity = pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.GetQuantity();
					pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem.SetQuantity(bQuantity+byCnt);
					byPackCnt -= byCnt;
				}

				if(bAppearChk==FALSE || byPack!=0)
				{
					pItem->SetQuantity(byPackCnt+byPack);
					Insert_SmallInvItem(pItem, 0, byPackCnt+byPack);
				}
			}
		}
		else
		{
			pItem->SetQuantity(pItem->GetQuantity());
			Insert_SmallInvItem(pItem, 0, pItem->GetQuantity());						
		}
	}
}

void Insert_TradeItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{
	if(pItem->GetType()==ITEM_LARGE)
		return;

	{
		if(g_pMainPlayer->m_pItemTrade[bZipCode].GetID()==0)
			memcpy(&g_pMainPlayer->m_pItemTrade[bZipCode], pItem, sizeof(CItem));
		else
		{
			BOOL bChk = FALSE;

			if(g_pMainPlayer->m_pItemTrade[bZipCode].GetID()==pItem->GetID())			
			{
				CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pItemTrade[bZipCode].m_wItemID);

				if(!lpItemTable)
					return;

				int nMaxQuantity	= 1;

				nMaxQuantity = lpItemTable->GetMaxQuantity();				

				BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_pItemTrade[bZipCode].GetQuantity();

				if(byCnt>=byPackCnt)
				{
					BYTE bQuantity = g_pMainPlayer->m_pItemTrade[bZipCode].GetQuantity();
					g_pMainPlayer->m_pItemTrade[bZipCode].SetQuantity(bQuantity+byPackCnt);
					bChk = TRUE;
				}
				else if(byCnt!=0)
				{
					BYTE bQuantity = g_pMainPlayer->m_pItemTrade[bZipCode].GetQuantity();
					g_pMainPlayer->m_pItemTrade[bZipCode].SetQuantity(bQuantity+byCnt);
					byPackCnt -= byCnt;
				}
			}

			if(bChk==FALSE)
			{
				for(int i = 0; i < MAX_TRADE; i++)
				{
					if(g_pMainPlayer->m_pItemTrade[i].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pItemTrade[i], pItem, sizeof(CItem));
						g_pMainPlayer->m_pItemTrade[i].SetQuantity(byPackCnt);
						bChk = TRUE;
						break;
					}
					else if(g_pMainPlayer->m_pItemTrade[i].GetID()==pItem->GetID())
					{
						CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pItemTrade[i].m_wItemID);

						if(!lpItemTable)
							return;

						int nMaxQuantity	= 1;

						nMaxQuantity = lpItemTable->GetMaxQuantity();						

						BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_pItemTrade[i].GetQuantity();

						if(byCnt>=byPackCnt)
						{
							BYTE bQuantity = g_pMainPlayer->m_pItemTrade[i].GetQuantity();
							g_pMainPlayer->m_pItemTrade[i].SetQuantity(bQuantity+byPackCnt);
							bChk = TRUE;
							break;
						}
						else if(byCnt!=0)
						{
							BYTE bQuantity = g_pMainPlayer->m_pItemTrade[i].GetQuantity();
							g_pMainPlayer->m_pItemTrade[i].SetQuantity(bQuantity+byCnt);
							byPackCnt -= byCnt;
						}
					}
				}
				
				if(bChk==FALSE)
				{
					pItem->SetQuantity(byPackCnt);
					Insert_SmallInvItem(pItem, 0, byPackCnt);
				}
			}			
		}
	}
}

void Insert_PlayerShopItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{
	if(!pItem)
		return;

	if(g_pMainPlayer->m_sPlayerShop.cItem[bZipCode].GetID()==0)
		memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[bZipCode], pItem, sizeof(CItem));
	else
	{
		BOOL bChk = FALSE;

		if(g_pMainPlayer->m_sPlayerShop.cItem[bZipCode].GetID()==pItem->GetID())			
		{
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_sPlayerShop.cItem[bZipCode].m_wItemID);

			int nMaxQuantity	= 1;

			nMaxQuantity = lpItemTable->GetMaxQuantity();			

			BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_sPlayerShop.cItem[bZipCode].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = g_pMainPlayer->m_sPlayerShop.cItem[bZipCode].GetQuantity();
				g_pMainPlayer->m_sPlayerShop.cItem[bZipCode].SetQuantity(bQuantity+byPackCnt);
				bChk = TRUE;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = g_pMainPlayer->m_sPlayerShop.cItem[bZipCode].GetQuantity();
				g_pMainPlayer->m_sPlayerShop.cItem[bZipCode].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
			}
		}

		if(bChk==FALSE)
		{
			for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
			{
				if(g_pMainPlayer->m_sPlayerShop.cItem[i].GetID()==0)
				{
					memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[i], pItem, sizeof(CItem));
					g_pMainPlayer->m_sPlayerShop.cItem[i].SetQuantity(byPackCnt);
					bChk = TRUE;
					break;
				}
				else if(g_pMainPlayer->m_sPlayerShop.cItem[i].GetID()==pItem->GetID())
				{
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_sPlayerShop.cItem[i].m_wItemID);

					if(!lpItemTable)
						return;

					int nMaxQuantity	= 1;

					nMaxQuantity = lpItemTable->GetMaxQuantity();					

					BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_sPlayerShop.cItem[i].GetQuantity();

					if(byCnt>=byPackCnt)
					{
						BYTE bQuantity = g_pMainPlayer->m_sPlayerShop.cItem[i].GetQuantity();
						g_pMainPlayer->m_sPlayerShop.cItem[i].SetQuantity(bQuantity+byPackCnt);
						bChk = TRUE;
						break;
					}
					else if(byCnt!=0)
					{
						BYTE bQuantity = g_pMainPlayer->m_sPlayerShop.cItem[i].GetQuantity();
						g_pMainPlayer->m_sPlayerShop.cItem[i].SetQuantity(bQuantity+byCnt);
						byPackCnt -= byCnt;
					}
				}
			}
			
			if(bChk==FALSE)
			{
				pItem->SetQuantity(byPackCnt);
				Insert_SmallInvItem(pItem, 0, byPackCnt);
			}
		}			
	}
}

BYTE Insert_SmallBankItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{
	if(!pItem)
		return 0;
	
	if(pItem->GetType()==ITEM_LARGE)
		return 0;
	
	if(g_pMainPlayer->m_pBank_Small[bZipCode].GetID()==0)
		memcpy(&g_pMainPlayer->m_pBank_Small[bZipCode], pItem, sizeof(CItem));
	else
	{
		BOOL bChk = FALSE;

		if(g_pMainPlayer->m_pBank_Small[bZipCode].GetID()==pItem->GetID())			
		{
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pBank_Small[bZipCode].m_wItemID);

			if(!lpItemTable)
				return 0;

			int nMaxQuantity	= 1;

			nMaxQuantity	= lpItemTable->GetMaxQuantity();							

			BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_pBank_Small[bZipCode].GetQuantity();

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = g_pMainPlayer->m_pBank_Small[bZipCode].GetQuantity();
				g_pMainPlayer->m_pBank_Small[bZipCode].SetQuantity(bQuantity+byPackCnt);
				byPackCnt = 0;
				bChk = TRUE;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = g_pMainPlayer->m_pBank_Small[bZipCode].GetQuantity();
				g_pMainPlayer->m_pBank_Small[bZipCode].SetQuantity(bQuantity+byCnt);
				byPackCnt -= byCnt;
			}
		}

		if(bChk==FALSE)
		{
			for(int i = 0; i < MAX_BANK_SMALL_POOL; i++)
			{
				if(g_pMainPlayer->m_pBank_Small[i].GetID()==0)
				{
					memcpy(&g_pMainPlayer->m_pBank_Small[i], pItem, sizeof(CItem));
					g_pMainPlayer->m_pBank_Small[i].SetQuantity(byPackCnt);
					bChk = TRUE;
					byPackCnt = 0;
					break;
				}
				else if(g_pMainPlayer->m_pBank_Small[i].GetID()==pItem->GetID())
				{
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pBank_Small[i].m_wItemID);

					if(!lpItemTable)
						return 0;

					int nMaxQuantity	= 1;

					nMaxQuantity	= lpItemTable->GetMaxQuantity();

					BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_pBank_Small[i].GetQuantity();

					if(byCnt>=byPackCnt)
					{
						BYTE bQuantity = g_pMainPlayer->m_pBank_Small[i].GetQuantity();
						g_pMainPlayer->m_pBank_Small[i].SetQuantity(bQuantity+byPackCnt);
						bChk = TRUE;
						byPackCnt = 0;
						break;
					}
					else if(byCnt!=0)
					{
						BYTE bQuantity = g_pMainPlayer->m_pBank_Small[i].GetQuantity();
						g_pMainPlayer->m_pBank_Small[i].SetQuantity(bQuantity+byCnt);
						byPackCnt -= byCnt;
					}
				}
			}
			
			if(bChk==FALSE)
				byPackCnt = Insert_SmallInvItem(pItem, 0, byPackCnt);
		}			
	}
	return byPackCnt;
}

void Insert_SmallBeltItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bChk)
{
	if(!pItem)
		return;

	if(pItem->GetType()==ITEM_LARGE)
		return;
	
	{
		if(g_pMainPlayer->m_pBelt[bZipCode].GetID()==0)
			memcpy(&g_pMainPlayer->m_pBelt[bZipCode], pItem, sizeof(CItem));
		else
		{
			BOOL bChk = FALSE;

			if(g_pMainPlayer->m_pBelt[bZipCode].GetID()==pItem->GetID())			
			{
				CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pBelt[bZipCode].m_wItemID);

				if(!lpItemTable)
					return;

				int nMaxQuantity	= 1;

				nMaxQuantity = lpItemTable->GetMaxQuantity();				

				BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_pBelt[bZipCode].GetQuantity();

				if(byCnt>=byPackCnt)
				{
					BYTE bQuantity = g_pMainPlayer->m_pBelt[bZipCode].GetQuantity();
					g_pMainPlayer->m_pBelt[bZipCode].SetQuantity(bQuantity+byPackCnt);
					byPackCnt = 0;
					bChk = TRUE;				
				}
				else if(byCnt!=0)
				{
					BYTE bQuantity = g_pMainPlayer->m_pBelt[bZipCode].GetQuantity();
					g_pMainPlayer->m_pBelt[bZipCode].SetQuantity(bQuantity+byCnt);
					byPackCnt -= byCnt;
					pItem->SetQuantity(byPackCnt);
				}
			}

			if(bChk==FALSE)
			{
				for(int i = 0; i < MAX_BELT_POOL; i++)
				{
					if(g_pMainPlayer->m_pBelt[i].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pBelt[i], pItem, sizeof(CItem));
						g_pMainPlayer->m_pBelt[i].SetQuantity(byPackCnt);
						byPackCnt = 0;
						break;
					}
					else if(g_pMainPlayer->m_pBelt[i].GetID()==pItem->GetID())
					{
						CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pBelt[i].m_wItemID);

						if(!lpItemTable)
							return;
						
						int nMaxQuantity	= 1;

						nMaxQuantity = lpItemTable->GetMaxQuantity();
						
						BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_pBelt[i].GetQuantity();

						if(byCnt>=byPackCnt)
						{
							BYTE bQuantity = g_pMainPlayer->m_pBelt[i].GetQuantity();
							g_pMainPlayer->m_pBelt[i].SetQuantity(bQuantity+byPackCnt);
							byPackCnt = 0;							
							break;
						}
						else if(byCnt!=0)
						{
							BYTE bQuantity = g_pMainPlayer->m_pBelt[i].GetQuantity();
							g_pMainPlayer->m_pBelt[i].SetQuantity(bQuantity+byCnt);
							byPackCnt -= byCnt;
						}
					}
				}
				
				if(bChk==FALSE)				
				{
					if(byPackCnt!=0)
					{
						pItem->SetQuantity(byPackCnt);
						Insert_SmallInvItem(pItem, 0, byPackCnt);					
					}
				}
			}
		}
	}
}

BYTE Insert_SmallInvItem(CItem* pItem, BYTE bZipCode, BYTE byPackCnt)
{	
	if(!pItem)
		return 0;

	if(pItem->GetType()==ITEM_LARGE)
		return 0;
	
	{
		BOOL bChk = FALSE;

		if(g_pMainPlayer->m_pInv_Small[bZipCode].GetID()==0)
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[bZipCode], pItem, sizeof(CItem));
			byPackCnt = 0;
			bChk = TRUE;
		}
		else
		{ 
			if(g_pMainPlayer->m_pInv_Small[bZipCode].GetID()==pItem->GetID())			
			{
				CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Small[bZipCode].m_wItemID);

				if(!lpItemTable)
					return 0;

				int nMaxQuantity	= 1;

				nMaxQuantity = lpItemTable->GetMaxQuantity();				

				BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_pInv_Small[bZipCode].GetQuantity();

				if(byCnt>=byPackCnt)
				{
					BYTE bQuantity = g_pMainPlayer->m_pInv_Small[bZipCode].GetQuantity();
					g_pMainPlayer->m_pInv_Small[bZipCode].SetQuantity(bQuantity+byPackCnt);
					bChk = TRUE;
					byPackCnt = 0;
				}
				else if(byCnt!=0)
				{
					BYTE bQuantity = g_pMainPlayer->m_pInv_Small[bZipCode].GetQuantity();
					g_pMainPlayer->m_pInv_Small[bZipCode].SetQuantity(bQuantity+byCnt);
					byPackCnt -= byCnt;
				}
			}

			if(bChk==FALSE)
			{
				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Small[i].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pInv_Small[i], pItem, sizeof(CItem));						
						g_pMainPlayer->m_pInv_Small[i].SetQuantity(byPackCnt);
						bChk = TRUE;
						byPackCnt = 0;
						break;
					}
					else if(g_pMainPlayer->m_pInv_Small[i].GetID()==pItem->GetID())
					{
						CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Small[i].m_wItemID);

						if(!lpItemTable)
							return 0;

						int nMaxQuantity	= 1;

						nMaxQuantity = lpItemTable->GetMaxQuantity();						

						BYTE byCnt = nMaxQuantity - g_pMainPlayer->m_pInv_Small[i].GetQuantity();

						if(byCnt>=byPackCnt)
						{
							BYTE bQuantity = g_pMainPlayer->m_pInv_Small[i].GetQuantity();
							g_pMainPlayer->m_pInv_Small[i].SetQuantity(bQuantity+byPackCnt);							
							bChk = TRUE;
							byPackCnt = 0;
							break;
						}
						else if(byCnt!=0)
						{
							BYTE bQuantity = g_pMainPlayer->m_pInv_Small[i].GetQuantity();
							g_pMainPlayer->m_pInv_Small[i].SetQuantity(bQuantity+byCnt);
							byPackCnt -= byCnt;
						}
					}
				}							
				if(bChk==FALSE)			
					return byPackCnt;
			}			
		}
	}
	return byPackCnt;
}


//======================================================================//
// Item 관련(서버에서 받은 메세지 처리).								//
//======================================================================//
void SetPickupItemRing(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)		
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));		
	else if(pPacket->bInv==9)
	{		
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));			
	}
}

void SetPickupItemBoots(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)		
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));		
	else if(pPacket->bInv==9)
	{	
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));					
	}
}

void SetPickupItemAmule(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));		
	else if(pPacket->bInv==9)
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));				
	}
}

void SetPickupItemRide(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));		
	else if(pPacket->bInv==9)
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));				
	}
}

void SetPickupItemBelt(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));		
	else if(pPacket->bInv==9)
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));				
	}
}

void SetPickupItemArmor(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));						
	else if(pPacket->bInv==9)
	{		
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));					
	}
}

void SetPickupItemHelmet(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)	
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));						
	else if(pPacket->bInv==9)
	{		
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));	
	}
}


void SetPickupItemBag(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
	else if(pPacket->bInv==9)
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));						
	}
}


void SetPickupItemGlove(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
	else if(pPacket->bInv==9)
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk==TRUE)
			memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));		

		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));						
	}
}


void SetPickupItemWeaponR(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)
	{				
		int nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;

		if((nVal>=2 && nVal<=5) || nVal==7)
		{
			// 투핸드 //
			memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));		

			if(pPacket->bEquipCode==EQUIP_TYPE_RHAND1)
			{
				if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID!=0)
				{
					BOOL bChk = FALSE;

					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							bChk = TRUE;
							break;
						}						
					}

					if(bChk==TRUE)
						memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1], sizeof(CItem));

					memset(&g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
				}
			}
			else if(pPacket->bEquipCode==EQUIP_TYPE_RHAND2)
			{
				if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID!=0)
				{
					BOOL bChk = FALSE;

					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							bChk = TRUE;
							break;
						}						
					}

					if(bChk==TRUE)
						memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2], sizeof(CItem));

					memset(&g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));
				}
			}
		}
		else
		{
			// 원핸드 //
			memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));		
		}				
	}
	else if(pPacket->bInv==9)
	{
		int nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;

		if((nVal>=2 && nVal<=5) || nVal==7)
		{
			// 투핸드 //
			BOOL bChk = FALSE;

			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					bChk = TRUE;
					break;
				}
			}

			if(bChk==TRUE)
			{
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));
				memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
			}
			else
			{
				memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
			}

			if(pPacket->bEquipCode==EQUIP_TYPE_RHAND1)
			{
				if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID!=0)
				{
					BOOL bChk = FALSE;

					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							bChk = TRUE;
							break;
						}						
					}

					if(bChk==TRUE)
						memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode2], &g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1], sizeof(CItem));

					memset(&g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
				}
			}
			else if(pPacket->bEquipCode==EQUIP_TYPE_RHAND2)
			{
				if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID!=0)
				{
					BOOL bChk = FALSE;

					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							bChk = TRUE;
							break;
						}						
					}

					if(bChk==TRUE)
						memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode2], &g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2], sizeof(CItem));

					memset(&g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));
				}
			}
		}
		else
		{
			BOOL bChk = FALSE;

			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					bChk = TRUE;
					break;
				}
			}

			if(bChk==TRUE)
			{
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));
				memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
			}
			else
			{
				memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
			}
		}		
	}
}


void SetPickupItemShield(DSTC_ITEM_PICKUP* pPacket)
{
	if(pPacket->bInv==8)
	{
		memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));		

		if(pPacket->bEquipCode==EQUIP_TYPE_LHAND1)
		{
			if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID!=0)
			{
				int nVal = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID/ITEM_DISTRIBUTE;

				if((nVal>=2 && nVal<=5) || nVal==7)
				{
					BOOL bChk = FALSE;

					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							bChk = TRUE;
							break;
						}						
					}

					if(bChk==TRUE)
						memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1], sizeof(CItem));

					memset(&g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
				}
			}
		}
		else if(pPacket->bEquipCode==EQUIP_TYPE_LHAND2)
		{
			if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID!=0)
			{
				int nVal = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID/ITEM_DISTRIBUTE;

				if((nVal>=2 && nVal<=5) || nVal==7)
				{
					BOOL bChk = FALSE;

					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
						{
							bChk = TRUE;
							break;
						}						
					}

					if(bChk==TRUE)
						memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2], sizeof(CItem));

					memset(&g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));
				}
			}
		}			
	}
	else if(pPacket->bInv==9)
	{		
		int nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;

		if(nVal==12)
		{
			// 투핸드 //
			BOOL bChk = FALSE;

			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					bChk = TRUE;
					break;
				}
			}

			if(bChk==TRUE)
			{
				memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode], &g_pMainPlayer->m_pEquip[pPacket->bEquipCode], sizeof(CItem));
				memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
			}
			else
			{
				memcpy(&g_pMainPlayer->m_pEquip[pPacket->bEquipCode], &g_pMainPlayer->m_MouseItem, sizeof(CItem));
			}

			if(pPacket->bEquipCode==EQUIP_TYPE_LHAND1)
			{
				if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID!=0)
				{
					int nVal2 = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID/ITEM_DISTRIBUTE;

					if((nVal2>=2 && nVal2<=5) || nVal2==7)
					{
						BOOL bChk2 = FALSE;

						for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
							{
								bChk2 = TRUE;
								break;
							}						
						}

						if(bChk2==TRUE)
							memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode2], &g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1], sizeof(CItem));

						memset(&g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
					}
				}
			}
			else if(pPacket->bEquipCode==EQUIP_TYPE_LHAND2)
			{
				if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID!=0)
				{
					int nVal2 = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID/ITEM_DISTRIBUTE;

					if((nVal2>=2 && nVal2<=5) || nVal2==7)
					{
						BOOL bChk2 = FALSE;

						for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
							{
								bChk2 = TRUE;
								break;
							}						
						}

						if(bChk2==TRUE)
							memcpy(&g_pMainPlayer->m_pInv_Large[pPacket->bZipCode2], &g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2], sizeof(CItem));

						memset(&g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));
					}
				}
			}
		}	
	}
}


//======================================================================//
// Item 관련(서버로 메세지 보낼때).										//
//======================================================================//
void ItemSetWeaponR(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{			
	if(nItemType>=0 && nItemType<=7)
	{	
		// 장착창에 아이템이 없을 경우 //
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		// 장착창에 아이템이 있을 경우 //
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}

void ItemSetWeaponShield(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	BOOL	bChk	= FALSE;
	BYTE	bType	= 0;	

	if(nItemType==12)
	{	
		// 장착창에 아이템이 없을 경우 //
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		// 장착창에 아이템이 있을 경우 //
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{		
		if(nItemIndex==EQUIP_TYPE_LHAND1)
			bType = EQUIP_TYPE_RHAND1;
		else if(nItemIndex==EQUIP_TYPE_LHAND2)
			bType = EQUIP_TYPE_RHAND2;						
		
		if((nItemType>=2 && nItemType<=5) || nItemType==7)
		{
			// 장착창에 아이템이 없을 경우 //
			if(g_pMainPlayer->m_pEquip[bType].m_wItemID==0)
				SetItemPacket(ItemPickup, 8, 0, bType, 0, 0);
			// 장착창에 아이템이 있을 경우 //
			else
				SetItemPacket(ItemPickup, 9, 0, bType, 0, 0);
		}
		else
		{
			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
					bChk = TRUE;
					break;
				}
			}
			
			if(bChk==FALSE)
				SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
		}		
	}
}

void ItemSetRing(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{		
	if(	nItemType==16 &&
		(nItemIndex==EQUIP_TYPE_RRING1 || nItemIndex==EQUIP_TYPE_RRING2 || nItemIndex==EQUIP_TYPE_RRING3))
	{
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		if(	nItemType==16 &&
			(nItemIndex==EQUIP_TYPE_LRING1 || nItemIndex==EQUIP_TYPE_LRING2 || nItemIndex==EQUIP_TYPE_LRING3))
		{
			// "이곳에 착용할 수 없는 아이템입니다"
			DisplayMessageAdd(g_Message[ETC_MESSAGE989].szMessage, 0xffff0000); 
		}

		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}	
}

void ItemSetBoots(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	if(nItemType==15)
	{
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}


void ItemSetAmule(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	if(nItemType==17 && nItemIndex==EQUIP_TYPE_AMULE2)
	{
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		if(	nItemType==17 &&
			(nItemIndex==EQUIP_TYPE_AMULE1 || nItemIndex==EQUIP_TYPE_AMULE3))
		{
			// "이곳에 착용할 수 없는 아이템입니다"
			DisplayMessageAdd(g_Message[ETC_MESSAGE989].szMessage, 0xffff0000); 
		}

		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}

void ItemSetBelt(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	if(nItemType==13)
	{
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}

void ItemSetArmor(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	if(nItemType==11)
	{

#ifndef		GM_TOOL

		if(g_pMainPlayer->m_MouseItem.m_wItemID==GM_ARMOR)
		{
			BOOL bChk = FALSE;

			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
					bChk = TRUE;
					break;
				}
			}

			if(bChk==FALSE)
				SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
		}
		else
		{
			if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
				SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
			else
				SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);	
		}
		
		

#else
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);		
#endif		
	}
	else
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}

void ItemSetHelmet(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	if(nItemType==10)
	{
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}

void ItemSetGlove(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	if(nItemType==14)
	{
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}

void ItemSetRide(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	if(nItemType==20)
	{
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}

void ItemSetMantle(CTDS_ITEM_PICKUP* ItemPickup, int nItemIndex, int nItemType)
{
	if(nItemType==18)
	{
		if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID==0)
			SetItemPacket(ItemPickup, 8, 0, nItemIndex, 0, 0);
		else
			SetItemPacket(ItemPickup, 9, 0, nItemIndex, 0, 0);
	}
	else
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
				bChk = TRUE;
				break;
			}
		}

		if(bChk==FALSE)
			SetItemPacket(ItemPickup, 10, i, 0, 0, 0);
	}
}

void ItemMouseUp()
{
	CItemWnd*			pItemWnd	= CItemWnd::GetInstance();
	CTDS_ITEM_PICKUP	ItemPickup;

	ItemPickup.bSectionNum	= 1;
	ItemPickup.i64ItemID	= 0;
		
	int nPosX		= (g_Mouse.MousePos.x-pItemWnd->m_fPosX-6) / 34;
	int nPosY		= (g_Mouse.MousePos.y-pItemWnd->m_fPosZ-44) / 68;
	int nItemIndex	= nPosY * 7 + nPosX;				
	int nItemType	= g_pMainPlayer->m_MouseItem.m_wItemID / ITEM_DISTRIBUTE;

	if(nItemIndex >= MAX_EQUIP_POOL)
	{
		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				return;
			}
		}
		return;
	}

	if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_NORMAL)
	{
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_EQUIP,nItemIndex,1,1);
		}
		else
		{
			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					return;
				}
			}
		}		
	}
	else
	{
		if(nItemType<=__MAX_ITEM_LARGE_INDEX__)
		{		
			switch(nItemIndex)
			{
			case EQUIP_TYPE_RHAND1:
			case EQUIP_TYPE_RHAND2:
				{
					ItemSetWeaponR(&ItemPickup, nItemIndex, nItemType);				
				}
				break;
			case EQUIP_TYPE_LHAND1:
			case EQUIP_TYPE_LHAND2:			
				{
					ItemSetWeaponShield(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			case EQUIP_TYPE_RRING1:
			case EQUIP_TYPE_RRING2:
			case EQUIP_TYPE_RRING3:
			case EQUIP_TYPE_LRING1:
			case EQUIP_TYPE_LRING2:
			case EQUIP_TYPE_LRING3:
				{				
					ItemSetRing(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			case EQUIP_TYPE_BOOTS:
				{
					ItemSetBoots(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			case EQUIP_TYPE_AMULE1:			
			case EQUIP_TYPE_AMULE2:
			case EQUIP_TYPE_AMULE3:
				{
					ItemSetAmule(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			case EQUIP_TYPE_BELT:
				{
					ItemSetBelt(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			case EQUIP_TYPE_ARMOR:
				{				
					ItemSetArmor(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			case EQUIP_TYPE_HELMET:
				{
					ItemSetHelmet(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			case EQUIP_TYPE_RIDE:
				{
					ItemSetRide(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			case EQUIP_TYPE_BAG:
				{
					ItemSetMantle(&ItemPickup, nItemIndex, nItemType);
				}
				break;			
			case EQUIP_TYPE_RGLOVE:
			case EQUIP_TYPE_LGLOVE:
				{
					ItemSetGlove(&ItemPickup, nItemIndex, nItemType);
				}
				break;
			}	
			
			// 인벤이 가득찬경우 장착창에 아이템 올릴경우 오류메시지 출력
			// 20050128 김영대 대만
#if IS_TAIWAN_LOCALIZING()
			BOOL bChk = FALSE;
			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					bChk = TRUE;
					break;
				}
			}

			if ( !bChk )
			{
				ItemPickup.bInv = 9;
			}
#endif

			g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
			

			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_EQUIP,nItemIndex,1,1);
			}
		}
	}
}

void SetItemPacket(CTDS_ITEM_PICKUP* pItemPacket, BYTE bInv, BYTE bZipCode, BYTE bEquipCode, BYTE bZipCode2, BYTE bEquipCode2)
{
	pItemPacket->bInv			= bInv;
	pItemPacket->bZipCode		= bZipCode;
	pItemPacket->bEquipCode		= bEquipCode;
	pItemPacket->bZipCode2		= bZipCode2;
	pItemPacket->bEqiupCode2	= bEquipCode2;
}

void ItemLand(int nVal, CTDS_ITEM_PICKUP* ItemPickup)
{
	if(nVal==50)
		SetItemPacket(ItemPickup, 4, 0, 0, 0, 0);
	else
		SetItemPacket(ItemPickup, 10, 0, 0, 0, 0);

	g_pNet->SendMsg( (char*)ItemPickup, ItemPickup->GetPacketSize(), SERVER_INDEX_ZONE );
}


BOOL InsertItem(CTDS_ITEM_PICKUP* ItemPickup)
{
	CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

	int nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;

	BOOL bItemActive = FALSE;

	ItemPickup->bSectionNum	= 1;
	ItemPickup->i64ItemID	= 0;

	if(!pQuantityWnd->GetActive())
	{			
		if(GetType(nValue)==ITEM_LARGE)
		{
			// 비워있는 아이템 없으면 바닥에 버리고 있으면 넣는다 //
			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					bItemActive	= TRUE;
					SetItemPacket(ItemPickup, 6, i, 0, 0, 0);
					break;
				}
			}
			if(bItemActive==FALSE)
			{
				if(g_pThisDungeon->IsVillage()==TRUE)
				{
					for(int j = 0; j < MAX_BANK_LARGE_POOL; j++)
					{
						if(g_pMainPlayer->m_pBank_Large[j].m_wItemID==0)
						{
							bItemActive	= TRUE;
							SetItemPacket(ItemPickup, 24, j, 0, 0, 0);
							// "인벤토리에 공간이 부족합니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE90].szMessage, 0xFFFFC309); 
							// "아이템을 창고로 이동합니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE252].szMessage, 0xFFFFC309); 
							break;
						}
					}
				}				
				SetItemPacket(ItemPickup, 10, 0, 0, 0, 0);
				bItemActive	= TRUE;
			}
		}										
		else if(GetType(nValue)==ITEM_SMALL)
		{												
			for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
			{								
				if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
				{									
					SetItemPacket(ItemPickup, 14, i, 0, 0, 0);
					bItemActive = TRUE;
					break;
				}
			}

			if(!bItemActive)
			{			
				if(g_pThisDungeon->IsVillage()==TRUE)
				{
					for(int j = 0; j < MAX_BANK_SMALL_POOL; j++)
					{
						if(g_pMainPlayer->m_pBank_Small[j].m_wItemID==0)
						{
							ItemPickup->bInv		= 25;
							ItemPickup->bZipCode	= j;
							ItemPickup->dwMoney		= g_pMainPlayer->m_MouseItem.GetQuantity();
							pQuantityWnd->m_bReq	= TRUE;
							bItemActive = TRUE;
							break;
						}
					}
				}

				if(!bItemActive)
				{
					// 수량 창 //
					bItemActive = TRUE;
					SetItemPacket(ItemPickup, 4, 0, 0, 0, 0);
				}				
			}
		}
				
		if(bItemActive==TRUE)
		{
			g_pNet->SendMsg((char*)ItemPickup, ItemPickup->GetPacketSize(), SERVER_INDEX_ZONE );
			return TRUE;
		}					
	}

	return FALSE;
}


void InvLargeInsert(CTDS_ITEM_PICKUP* pPacket)
{
	for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
	{
		if(g_pMainPlayer->m_pInv_Large[i].GetID()==0)
		{
			SetItemPacket(pPacket, 6, i, 0, 0, 0);
			g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE );
			return;
		}
	}

	SetItemPacket(pPacket, 10, 0, 0, 0, 0);
	g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE );

	return;
}


BOOL ItemChkMaking(CTDS_ITEM_PICKUP* pPacket)
{
	CQuantityWnd*		pQuantityWnd	= CQuantityWnd::GetInstance();
	CItemMakingWnd*		pItemMakingWnd	= CItemMakingWnd::GetInstance();
	int					nValueMouse		= 0;

	if(g_ItemMoveManager.GetNewItemMoveMode())
	{
		nValueMouse = g_ItemMoveManager.GetMouseItemID() / ITEM_DISTRIBUTE;
	}
	else
	{
		nValueMouse	= g_pMainPlayer->m_MouseItem.GetID() / ITEM_DISTRIBUTE;
	}

	if(pItemMakingWnd->GetActive())
	{
		if(pQuantityWnd->m_bReq==TRUE)
			return TRUE;

//		LP_ITEM_TABLE pBaseItemMouse = NULL;
		CBaseItem* pBaseItemMouse = NULL;

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			pBaseItemMouse = g_pItemTableHash->GetData(g_ItemMoveManager.GetMouseItemID()); 
		}
		else
		{
			pBaseItemMouse = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID()); 
		}

		// 버릴때 수량창 나오기.
		if(ItemCollision(ITEMMAKING_WND)==TRUE)
		{
			// 마우스에 있는 아이템이 라지 아이템 이면 마우스에서 인벤으로 
			if(GetType(nValueMouse)==ITEM_LARGE)
			{
				if( g_ItemMoveManager.GetNewItemMoveMode())
				{
					g_ItemMoveManager.Initialize();	
					return TRUE;
				}
				else
				{
					if(	g_Mouse.MousePos.x>=pItemMakingWnd->m_fPosX+142	&&
						g_Mouse.MousePos.x<=pItemMakingWnd->m_fPosX+253	&&
						g_Mouse.MousePos.y>=pItemMakingWnd->m_fPosZ+33	&&
						g_Mouse.MousePos.y<=pItemMakingWnd->m_fPosZ+65)
					{
						goto lb_ChkMakingFail;
					}
					else
					{
						goto lb_ChkMakingFail;
					}
				}				
			}
			// 마우스에 있는 아이템이 스몰 아이템이고 
			else if(GetType(nValueMouse)==ITEM_SMALL)
			{
				if( g_ItemMoveManager.GetNewItemMoveMode() )
				{
					if(	g_Mouse.MousePos.x>=pItemMakingWnd->m_fPosX+142	&&
						g_Mouse.MousePos.x<=pItemMakingWnd->m_fPosX+253	&&
						g_Mouse.MousePos.y>=pItemMakingWnd->m_fPosZ+33	&&
						g_Mouse.MousePos.y<=pItemMakingWnd->m_fPosZ+65)		
					{
						BYTE byZipCode = ( g_Mouse.MousePos.x - (pItemMakingWnd->m_fPosX+142) ) / 37;
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_MAKING,byZipCode+1,1,1,0,0);
						return TRUE;
					}
				}
				else
				{
					// 마우스에 있는 아이템이 메터리얼이면  			
					if(nValueMouse>=ITEM_MATERIALST_INDEX && nValueMouse<=ITEM_MATERIALEND_INDEX)					
					{
						if(pBaseItemMouse == NULL)
						{
							goto lb_ChkMakingFail;	
						}
						// 마우스에 있는 아이템이 메터리얼 이긴 한데 ItemMaking 오더가 0이면 제조 못하는 좆밥 아이템 
						else if(pBaseItemMouse->BaseItem_Materials.byOrder == 0)
						{
							goto lb_ChkMakingFail;
						}

						for(int i = 1; i < 4; i++)
						{
							if(pItemMakingWnd->m_pMakingItem[i].GetID() != 0)
							{
								int nValue = pItemMakingWnd->m_pMakingItem[i].GetID() / ITEM_DISTRIBUTE;

								// 현재 아이템 제조창 스몰 아이템 칸에 메터리얼이 있으면
								if(nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX)
								{
//									LP_ITEM_TABLE pBaseItem	= g_pItemTableHash->GetData(pItemMakingWnd->m_pMakingItem[i].GetID()); 
									CBaseItem* pBaseItem	= g_pItemTableHash->GetData(pItemMakingWnd->m_pMakingItem[i].GetID()); 
									ITEM_MAKING_INFO* pItemMakingInfo	= g_pItemMakingInfoHash->GetData(pBaseItem->BaseItem_Materials.byOrder);
									
									if(pBaseItemMouse->wID == pBaseItem->wID)
									{
										goto lb_ChkMakingFail;
									}
									else if(pBaseItemMouse->wID != pBaseItem->wID)
									{
										if(pBaseItemMouse->wID != pItemMakingInfo->wItem_id1)
										{
											if(pBaseItemMouse->wID != pItemMakingInfo->wItem_id2)
											{
												goto lb_ChkMakingFail;
											}										
										}									
									}
								}							
							}
						}
					}
					// 마우스에 있는 아이템이 조디악 카드 (큰게)이면 
					else if(nValueMouse == ITEM_ZODIAC_INDEX)
					{
						if(pBaseItemMouse->wID == __ITEM_MAKING_INDEX__)
						{
							for(int i = 1; i < 4; i++)
							{
								if(	pItemMakingWnd->m_pMakingItem[i].GetID() != 0 &&
									pItemMakingWnd->m_pMakingItem[i].GetID() == __ITEM_MAKING_INDEX__)
								{
									goto lb_ChkMakingFail;
								}
							}
						}
						else
						{
							goto lb_ChkMakingFail;
						}
					}
					else
					{
						goto lb_ChkMakingFail;
					}

					// 스몰아이템 업그레이드의 모든조건이 맞으면....
					if(	g_Mouse.MousePos.x>=pItemMakingWnd->m_fPosX+142	&&
						g_Mouse.MousePos.x<=pItemMakingWnd->m_fPosX+253	&&
						g_Mouse.MousePos.y>=pItemMakingWnd->m_fPosZ+33	&&
						g_Mouse.MousePos.y<=pItemMakingWnd->m_fPosZ+65)		
					{
						BYTE byZipCode = ( g_Mouse.MousePos.x - (pItemMakingWnd->m_fPosX+142) ) / 37;

						if(pItemMakingWnd->m_pMakingItem[byZipCode+1].GetID() != 0)
						{
							goto lb_ChkMakingFail;
						}
						else
						{
							SetItemPacket(pPacket, 79, byZipCode+1, 0, 0, 0);
							pPacket->dwMoney = 1;
							g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE );
							return TRUE;
						}
					}
				}				
			}
		}

lb_ChkMakingFail:

		// "제조 조건이 맞지 않습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1212].szMessage, 0xffffff00); 

		if(GetType(nValueMouse)==ITEM_SMALL)				
		{		
			for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
				{
					SetItemPacket(pPacket, 14, i, 0, 0, 0);
					g_pNet->SendMsg( (char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
					return TRUE;
				}
			}
		}
		else
		{
			for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
				{
					SetItemPacket(pPacket, 6, i, 0, 0, 0);
					g_pNet->SendMsg( (char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

BOOL ItemChkUpgrade(CTDS_ITEM_PICKUP* pPacket)
{
	CQuantityWnd*		pQuantityWnd	= CQuantityWnd::GetInstance();
	CItemUpgradeWnd*	pItemUpgradeWnd = CItemUpgradeWnd::GetInstance();
//	LP_ITEM_TABLE		pBaseItemWA		= NULL;
//	LP_ITEM_TABLE		pBaseItemUG		= NULL;
	CBaseItem*			pBaseItemWA		= NULL;
	CBaseItem*			pBaseItemUG		= NULL;

	if(pItemUpgradeWnd->GetActive())
	{
		if(pQuantityWnd->m_bReq==TRUE)
			return TRUE;

		int nValue = 0;
		CItem cItem;
		memset(&cItem, NULL, sizeof(CItem));

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			ITEM_NATIVE_INFO ItemNativeInfo;
			ItemNativeInfo.eSlotID = (ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc();
			ItemNativeInfo.bySlotIndex = g_ItemMoveManager.GetNativeSrcIndex();

			memcpy(&cItem, GetItemNative(&ItemNativeInfo), sizeof(CItem));

			nValue = cItem.GetID() / ITEM_DISTRIBUTE;
			pBaseItemWA = g_pItemTableHash->GetData(cItem.GetID()); 
		}
		else
		{
			nValue	= g_pMainPlayer->m_MouseItem.GetID() / ITEM_DISTRIBUTE;
			pBaseItemWA = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID()); 
		}

		if(!pBaseItemWA)		
		{
			char szInfo[0xff] = {0,};
			wsprintf(szInfo, g_Message[ETC_MESSAGE52].szMessage);	
			DisplayMessageAdd(szInfo, 0xffffff00);
			goto lb_ChkUpgradeFail;
		}

		// 버릴때 수량창 나오기.
		if(ItemCollision(ITEMUPGRADE_WND)==TRUE)
		{
			// 라지 아이템 이면
			if(GetType(nValue)==ITEM_LARGE)
			{
				// 무기구와 방어구 인지 
				if(	(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX) ||
					(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX))
				{
					// 반지와 아뮬렛이면 업그레이드 아예 안됨 
					if(nValue == ITEM_CLASS_RING || nValue == ITEM_CLASS_AMULET)
					{
						char szInfo[0xff] = {0,};
						wsprintf(szInfo,g_Message[ETC_MESSAGE1001].szMessage);	
						DisplayMessageAdd(szInfo, 0xffffff00);
						goto lb_ChkUpgradeFail;
					}

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						// 업그레이할 아이템의 슬롯이 0이면 업그레이드 안된다.
						if(cItem.m_bSlot.uMaxSlot == 0)
						{
							char szInfo[0xff] = {0,};
							wsprintf(szInfo,g_Message[ETC_MESSAGE1002].szMessage);	
							DisplayMessageAdd(szInfo, 0xffffff00);
							goto lb_ChkUpgradeFail;
						}
					}
					else
					{
						// 업그레이할 아이템의 슬롯이 0이면 업그레이드 안된다.
						if(g_pMainPlayer->m_MouseItem.m_bSlot.uMaxSlot == 0)
						{
							char szInfo[0xff] = {0,};
							wsprintf(szInfo,g_Message[ETC_MESSAGE1002].szMessage);	
							DisplayMessageAdd(szInfo, 0xffffff00);
							goto lb_ChkUpgradeFail;
						}
					}

					// 업그레이드할 아이템의 그레이드가 0인지 검사한다.0이면 업그레이드가 절대 안된다.
					if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
					{
						if( pBaseItemWA->BaseItem_Weapon.bItemGrade == 0 )
						{
							char szInfo[0xff] = {0,};
							wsprintf(szInfo,g_Message[ETC_MESSAGE1003].szMessage);	
							DisplayMessageAdd(szInfo, 0xffffff00);
							goto lb_ChkUpgradeFail;								
						}
					}
					else if(nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX)
					{
						if( pBaseItemWA->BaseItem_Armor.bItemGrade == 0 )
						{
							char szInfo[0xff] = {0,};
							wsprintf(szInfo,g_Message[ETC_MESSAGE1003].szMessage);	
							DisplayMessageAdd(szInfo, 0xffffff00);
							goto lb_ChkUpgradeFail;								
						}
					}

					// 강화창에 기존에 주문서와 리퀴드,에디션이 있는지 검사해서 있으면 그레이드 검사 
					for(int i = 1; i < 4; i++)
					{
						int nValueUG	= pItemUpgradeWnd->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE;
						pBaseItemUG		= g_pItemTableHash->GetData(pItemUpgradeWnd->m_pUpgradeItem[i].GetID());

						if( pBaseItemUG && nValueUG == ITEM_UPGRADE_INDEX )
						{
							// 무기류이면 
							if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
							{
								if( pBaseItemWA->BaseItem_Weapon.bItemGrade != pBaseItemUG->BaseItem_UpGrade.wW_Grade )
								{
									char szInfo[0xff] = {0,};
									wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
									DisplayMessageAdd(szInfo, 0xffffff00);
									goto lb_ChkUpgradeFail;
								}
								if( pBaseItemUG->BaseItem_UpGrade.wFormula[nValue] == 0 )
								{
									char szInfo[0xff] = {0,};
									wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
									DisplayMessageAdd(szInfo, 0xffffff00);
									goto lb_ChkUpgradeFail;
								}
							}
							// 방어구이면 
							else if( nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX )
							{
								if( pBaseItemWA->BaseItem_Armor.bItemGrade != pBaseItemUG->BaseItem_UpGrade.wW_Grade )
								{
									char szInfo[0xff] = {0,};
									wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
									DisplayMessageAdd(szInfo, 0xffffff00);
									goto lb_ChkUpgradeFail;
								}
								if( pBaseItemUG->BaseItem_UpGrade.wFormula[nValue] == 0 )
								{
									char szInfo[0xff] = {0,};
									wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
									DisplayMessageAdd(szInfo, 0xffffff00);
									goto lb_ChkUpgradeFail;
								}
							}
						}
						else if( pBaseItemUG && nValueUG == ITEM_LIQUID_INDEX )
						{
							// 무기류이면 
							if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
							{
								if( pBaseItemWA->BaseItem_Weapon.bItemGrade != pBaseItemUG->BaseItem_LiQuid.wW_Grade )
								{
									char szInfo[0xff] = {0,};
									wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
									DisplayMessageAdd(szInfo, 0xffffff00);
									goto lb_ChkUpgradeFail;
								}
							}
							// 방어구이면 
							else if( nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX )
							{
								if( pBaseItemWA->BaseItem_Armor.bItemGrade != pBaseItemUG->BaseItem_LiQuid.wW_Grade )
								{
									char szInfo[0xff] = {0,};
									wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
									DisplayMessageAdd(szInfo, 0xffffff00);
									goto lb_ChkUpgradeFail;
								}
							}
						}
						else if( pBaseItemUG && nValueUG == ITEM_EDITION_INDEX )
						{
							if( pBaseItemUG->BaseItem_Edition.wFormula[nValue] == 0 )
							{
								char szInfo[0xff] = {0,};
								wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
								DisplayMessageAdd(szInfo, 0xffffff00);
								goto lb_ChkUpgradeFail;
							}
						}
					}
						
					// 라지아이템 업그레이드의 모든조건이 맞으면....
					if(	g_Mouse.MousePos.x>=pItemUpgradeWnd->m_fPosX+12 &&
						g_Mouse.MousePos.x<=pItemUpgradeWnd->m_fPosX+44 &&
						g_Mouse.MousePos.y>=pItemUpgradeWnd->m_fPosZ+33 &&
						g_Mouse.MousePos.y<=pItemUpgradeWnd->m_fPosZ+97		)		
					{
						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_UPGRADE,0,1,1,0);
							return TRUE;
						}
						else
						{
							if(pItemUpgradeWnd->m_pUpgradeItem[0].GetID() == 0)
							{
								SetItemPacket(pPacket, 65, 0, 0, 0, 0);
								g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE );
								return TRUE;
							}
						}						
					}										
				}
			}
			// 스몰 아이템 
			else if(GetType(nValue)==ITEM_SMALL)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if( nValue == ITEM_UPGRADE_INDEX || nValue == ITEM_LIQUID_INDEX || nValue == ITEM_EDITION_INDEX )
					{
						// 강화할 아이템이 올려져 있으면 강화서와 그레이드,에디션이 매칭되는지 
						if( g_pMainPlayer->m_pUpgradeItem[0].GetID() != 0 )
						{
							int nValueWA	= g_pMainPlayer->m_pUpgradeItem[0].GetID() / ITEM_DISTRIBUTE;
							pBaseItemUG		= g_pItemTableHash->GetData(g_pMainPlayer->m_pUpgradeItem[0].GetID());

							if(!pBaseItemUG)
							{
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE52].szMessage);	
								DisplayMessageAdd(szInfo, 0xffffff00);
								goto lb_ChkUpgradeFail;
							}

							// 무기 강화 주문서 이면 
							if( nValue ==  ITEM_UPGRADE_INDEX )
							{
								// 무기구 
								if(nValueWA >= ITEM_WEAPONST_INDEX && nValueWA <= ITEM_WEAPONEND_INDEX)
								{
									// 그레이드가 맞는지 
									if( pBaseItemUG->BaseItem_Weapon.bItemGrade != pBaseItemWA->BaseItem_UpGrade.wW_Grade )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
									if( pBaseItemWA->BaseItem_UpGrade.wFormula[nValueWA] == 0 )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
								}
								// 방어구 
								else if(nValueWA >= ITEM_ARMORST_INDEX && nValueWA <= ITEM_ARMOREND_INDEX)
								{
									// 그레이드가 맞는지
									if( pBaseItemUG->BaseItem_Armor.bItemGrade != pBaseItemWA->BaseItem_UpGrade.wW_Grade )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
									if( pBaseItemWA->BaseItem_UpGrade.wFormula[nValueWA] == 0 )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
								}
							}
							
							// 리퀴드 이면 
							if( nValue == ITEM_LIQUID_INDEX )
							{
								// 무기구 
								if(nValueWA >= ITEM_WEAPONST_INDEX && nValueWA <= ITEM_WEAPONEND_INDEX)
								{
									// 그레이드가 맞는지 
									if( pBaseItemUG->BaseItem_Weapon.bItemGrade != pBaseItemWA->BaseItem_LiQuid.wW_Grade )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
								}
								// 방어구 
								else if(nValueWA >= ITEM_ARMORST_INDEX && nValueWA <= ITEM_ARMOREND_INDEX)
								{
									// 그레이드가 맞는지
									if( pBaseItemUG->BaseItem_Armor.bItemGrade != pBaseItemWA->BaseItem_LiQuid.wW_Grade )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
								}
							}

							// 에디션이면 
							if( nValue == ITEM_EDITION_INDEX )
							{
								if( pBaseItemWA->BaseItem_Edition.wFormula[nValueWA] == 0 )
								{
									char szInfo[0xff] = {0,};
									wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
									DisplayMessageAdd(szInfo, 0xffffff00);
									goto lb_ChkUpgradeFail;
								}
							}
						}
						// 강화할 아이템은 올려려져 있지 않은데 강화서나 리퀴드가 올려져 있을 경우 
						else 
						{
							for(int i = 1; i < 4; i++)
							{
								if( g_pMainPlayer->m_pUpgradeItem[i].GetID() != 0 )
								{
									int nValueUG	= g_pMainPlayer->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE;
									pBaseItemUG		= g_pItemTableHash->GetData(g_pMainPlayer->m_pUpgradeItem[i].GetID());
									
									if(!pBaseItemUG)
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo, g_Message[ETC_MESSAGE52].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}

									if( nValueUG == ITEM_UPGRADE_INDEX && nValue == ITEM_LIQUID_INDEX )
									{
										if( pBaseItemWA->BaseItem_LiQuid.wW_Grade != pBaseItemUG->BaseItem_UpGrade.wW_Grade )
										{
											char szInfo[0xff] = {0,};
											wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
											DisplayMessageAdd(szInfo, 0xffffff00);
											goto lb_ChkUpgradeFail;
										}
									}
									else if( nValueUG == ITEM_LIQUID_INDEX && nValue == ITEM_UPGRADE_INDEX )
									{
										if( pBaseItemWA->BaseItem_UpGrade.wW_Grade != pBaseItemUG->BaseItem_LiQuid.wW_Grade )
										{
											char szInfo[0xff] = {0,};
											wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
											DisplayMessageAdd(szInfo, 0xffffff00);
											goto lb_ChkUpgradeFail;
										}
									}
								}							
							}
						}
					}
				}
				else
				{
					if( nValue == ITEM_UPGRADE_INDEX || nValue == ITEM_LIQUID_INDEX || nValue == ITEM_EDITION_INDEX )
					{
						// 강화할 아이템이 올려져 있으면 강화서와 그레이드,에디션이 매칭되는지 
						if( pItemUpgradeWnd->m_pUpgradeItem[0].GetID() != 0 )
						{
							int nValueWA	= pItemUpgradeWnd->m_pUpgradeItem[0].GetID() / ITEM_DISTRIBUTE;
							pBaseItemUG		= g_pItemTableHash->GetData(pItemUpgradeWnd->m_pUpgradeItem[0].GetID());

							if(!pBaseItemUG)
							{
								char szInfo[0xff] = {0,};
								wsprintf(szInfo, g_Message[ETC_MESSAGE52].szMessage);	
								DisplayMessageAdd(szInfo, 0xffffff00);
								goto lb_ChkUpgradeFail;
							}

							// 무기 강화 주문서 이면 
							if( nValue ==  ITEM_UPGRADE_INDEX )
							{
								// 무기구 
								if(nValueWA >= ITEM_WEAPONST_INDEX && nValueWA <= ITEM_WEAPONEND_INDEX)
								{
									// 그레이드가 맞는지 
									if( pBaseItemUG->BaseItem_Weapon.bItemGrade != pBaseItemWA->BaseItem_UpGrade.wW_Grade )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
									if( pBaseItemWA->BaseItem_UpGrade.wFormula[nValueWA] == 0 )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
								}
								// 방어구 
								else if(nValueWA >= ITEM_ARMORST_INDEX && nValueWA <= ITEM_ARMOREND_INDEX)
								{
									// 그레이드가 맞는지
									if( pBaseItemUG->BaseItem_Armor.bItemGrade != pBaseItemWA->BaseItem_UpGrade.wW_Grade )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
									if( pBaseItemWA->BaseItem_UpGrade.wFormula[nValueWA] == 0 )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
								}
							}
							
							// 리퀴드 이면 
							if( nValue == ITEM_LIQUID_INDEX )
							{
								// 무기구 
								if(nValueWA >= ITEM_WEAPONST_INDEX && nValueWA <= ITEM_WEAPONEND_INDEX)
								{
									// 그레이드가 맞는지 
									if( pBaseItemUG->BaseItem_Weapon.bItemGrade != pBaseItemWA->BaseItem_LiQuid.wW_Grade )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
								}
								// 방어구 
								else if(nValueWA >= ITEM_ARMORST_INDEX && nValueWA <= ITEM_ARMOREND_INDEX)
								{
									// 그레이드가 맞는지
									if( pBaseItemUG->BaseItem_Armor.bItemGrade != pBaseItemWA->BaseItem_LiQuid.wW_Grade )
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}
								}
							}

							// 에디션이면 
							if( nValue == ITEM_EDITION_INDEX )
							{
								if( pBaseItemWA->BaseItem_Edition.wFormula[nValueWA] == 0 )
								{
									char szInfo[0xff] = {0,};
									wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
									DisplayMessageAdd(szInfo, 0xffffff00);
									goto lb_ChkUpgradeFail;
								}
							}
							
							for(int i = 1; i < 4; i++)
							{
								if( pItemUpgradeWnd->m_pUpgradeItem[i].GetID() != 0 )
								{
									int nValueUG	= pItemUpgradeWnd->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE;
									pBaseItemUG		= g_pItemTableHash->GetData(pItemUpgradeWnd->m_pUpgradeItem[i].GetID());
									
									if(!pBaseItemUG)
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo, g_Message[ETC_MESSAGE52].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}

									if(nValueUG == nValue)
									{
										DisplayMessageAdd(g_Message[ETC_MESSAGE1253].szMessage, 0xffffff00); // "같은 종류의 아이템이 이미 있습니다."
										goto lb_ChkUpgradeFail;
									}
								}
							}
						}
						// 강화할 아이템은 올려려져 있지 않은데 강화서나 리퀴드가 올려져 있을 경우 
						else 
						{
							for(int i = 1; i < 4; i++)
							{
								if( pItemUpgradeWnd->m_pUpgradeItem[i].GetID() != 0 )
								{
									int nValueUG	= pItemUpgradeWnd->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE;
									pBaseItemUG		= g_pItemTableHash->GetData(pItemUpgradeWnd->m_pUpgradeItem[i].GetID());
									
									if(!pBaseItemUG)
									{
										char szInfo[0xff] = {0,};
										wsprintf(szInfo, g_Message[ETC_MESSAGE52].szMessage);	
										DisplayMessageAdd(szInfo, 0xffffff00);
										goto lb_ChkUpgradeFail;
									}

									if( nValueUG == ITEM_UPGRADE_INDEX && nValue == ITEM_LIQUID_INDEX )
									{
										if( pBaseItemWA->BaseItem_LiQuid.wW_Grade != pBaseItemUG->BaseItem_UpGrade.wW_Grade )
										{
											char szInfo[0xff] = {0,};
											wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
											DisplayMessageAdd(szInfo, 0xffffff00);
											goto lb_ChkUpgradeFail;
										}
									}
									else if( nValueUG == ITEM_LIQUID_INDEX && nValue == ITEM_UPGRADE_INDEX )
									{
										if( pBaseItemWA->BaseItem_UpGrade.wW_Grade != pBaseItemUG->BaseItem_LiQuid.wW_Grade )
										{
											char szInfo[0xff] = {0,};
											wsprintf(szInfo,g_Message[ETC_MESSAGE1004].szMessage);	
											DisplayMessageAdd(szInfo, 0xffffff00);
											goto lb_ChkUpgradeFail;
										}
									}

									if(nValueUG == nValue)
									{
										DisplayMessageAdd(g_Message[ETC_MESSAGE1253].szMessage, 0xffffff00); // "같은 종류의 아이템이 이미 있습니다."
										goto lb_ChkUpgradeFail;
									}
								}							
							}
						}
					}
				}

				// 스몰아이템 업그레이드의 모든조건이 맞으면....
				if(	g_Mouse.MousePos.x>=pItemUpgradeWnd->m_fPosX+142	&&
					g_Mouse.MousePos.x<=pItemUpgradeWnd->m_fPosX+253	&&
					g_Mouse.MousePos.y>=pItemUpgradeWnd->m_fPosZ+33		&&
					g_Mouse.MousePos.y<=pItemUpgradeWnd->m_fPosZ+65			)		
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						BYTE byZipCode = ( g_Mouse.MousePos.x - (pItemUpgradeWnd->m_fPosX+142) ) / 37;						
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_UPGRADE,byZipCode+1,1,1,0);
						return TRUE;
					}
					else
					{
						BYTE byZipCode = ( g_Mouse.MousePos.x - (pItemUpgradeWnd->m_fPosX+142) ) / 37;						
						SetItemPacket(pPacket, 67, byZipCode+1, 0, 0, 0);
						pPacket->dwMoney = 1;
						g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE );
						return TRUE;
					}					
				}				
			}
		}

lb_ChkUpgradeFail:

		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			g_ItemMoveManager.Initialize();
			return TRUE;
		}
		else
		{
			if(GetType(nValue)==ITEM_SMALL)				
			{		
				// 아이템이 있으면 처리 //
				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					// 아이템에서 있는 칸을 찾는다 //
					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID == g_pMainPlayer->m_MouseItem.m_wItemID)
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<100)
						{
							SetItemPacket(pPacket, 14, i, 0, 0, 0);
							g_pNet->SendMsg( (char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
							return TRUE;
						}
					}
				}

				for(i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
					{
						SetItemPacket(pPacket, 14, i, 0, 0, 0);
						g_pNet->SendMsg( (char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
						return TRUE;
					}
				}
			}
			else
			{
				for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
					{
						SetItemPacket(pPacket, 6, i, 0, 0, 0);
						g_pNet->SendMsg( (char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL ItemChkPlayerShop(CTDS_ITEM_PICKUP* pPacket)
{
	CPlayerShopWnd* pPlayerShopWnd	= CPlayerShopWnd::GetInstance();
	CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();

	if(pPlayerShopWnd->GetActive())
	{
		if(pQuantityWnd->m_bReq==TRUE)
			return TRUE;
		if(pQuantityWnd->GetActive()==TRUE)
			return TRUE;

		// Item 좌표 체크 //
		if(ItemCollision(PLAYERSHOP_WND)==TRUE)			
		{
			// 다른 유저 노점창이야?
			if(pPlayerShopWnd->m_pOwnerPlayerShop != g_pMainPlayer)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					g_ItemMoveManager.Initialize();
					return TRUE;
				}

				if(g_pMainPlayer->m_MouseItem.GetID()!=0)
				{
					int nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;

					// 인벤토리에 넣기 //
					if(GetType(nValue)==ITEM_SMALL)	
					{
						InvSmallInsert(pPacket, &g_pMainPlayer->m_MouseItem);
					}
					else
					{
						InvLargeInsert(pPacket);						
					}

					return TRUE;
				}				
			}
			else
			{
				// 내 노점창이야?
				if(g_pMainPlayer->m_bPlayerShop)
				{
					// 노점 열었는데 왜 아이템을 올려놀라카냐?
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.Initialize();
						return TRUE;
					}
				}
			}
			
			if(	g_Mouse.MousePos.x > pPlayerShopWnd->m_fPosX+8		&&
				g_Mouse.MousePos.x < pPlayerShopWnd->m_fPosX+250	&&
				g_Mouse.MousePos.y > pPlayerShopWnd->m_fPosZ+68		&&
				g_Mouse.MousePos.y < pPlayerShopWnd->m_fPosZ+203)
			{
				if(pQuantityWnd->GetActive()==TRUE)
					return TRUE;

				int nValue = 0;

				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if( !g_ItemMoveManager.IsSrcToDestCorrect((ITEM_NATIVE)g_ItemMoveManager.GetNativeSrc()
						,ITEM_NATIVE_PLAYERSHOP) )
					{
						return TRUE;
					}
				}

				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					nValue = g_ItemMoveManager.GetMouseItemID()/ITEM_DISTRIBUTE; 
				}
				else
				{
					nValue	= g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;			
				}
			
				int nPosX	= (float)g_Mouse.MousePos.x-pPlayerShopWnd->m_fPosX-8;
				int nPosY	= (float)g_Mouse.MousePos.y-pPlayerShopWnd->m_fPosZ-68;

				if((nPosX%52<=32) && (nPosY%71<=64))
				{
					int nIndex = (nPosX/52)+(nPosY/71)*5;			
				
					if(nIndex >= MAX_PLAYER_SHOP_INV)
					{
						InvLargeInsert(pPacket);
						return TRUE;
					}
#ifdef _USE_IME
					if(GET_IMEEDIT()->IsActivated())
					{
						g_pInputManager->ClearInput(INPUT_BUFFER_2);
						g_pInputManager->InsertCharacter(INPUT_BUFFER_2, 
											(LPSTR)GET_IMEEDIT()->GetImeEditString(),
											lstrlen(GET_IMEEDIT()->GetImeEditString()));						
						GET_IMEEDIT()->DeactivateIME(FALSE);

#if IS_CHINA_LOCALIZING()
						if(CheckString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_2)))
						{
							g_pInputManager->ClearInput(INPUT_BUFFER_2);
							//	return TRUE;
						}
#endif
					}
#endif

					if(pPlayerShopWnd->GetInputMode()==1)
					{
						pPlayerShopWnd->SetInputMode(0, FALSE);
					}
					
					//pPlayerShopWnd->SetInputModeChk();

					if(GetType(nValue)==ITEM_LARGE)
					{
						if(g_pMainPlayer->m_sPlayerShop.cItem[nIndex].GetID()==0)
						{
							if(g_pMainPlayer->m_dwPlayserShop==0)
							{
								if( g_ItemMoveManager.GetNewItemMoveMode() )
								{
									if( g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_PLAYERSHOP )
									{
										g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_PLAYERSHOP,nIndex,1,1,0);
										return TRUE;
									}
								}

//								sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 노점...
								CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
								if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
								{
									CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
									pQuantityWnd->OpenWnd(__QUANTITY_PLAYERSHOP__);
									pQuantityWnd->m_byZipCode = nIndex;

									RECT rt;
									rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
									rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
									rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
									rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
									int nOrder	= pQuantityWnd->GetStartOrder()+2;	

									g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
									g_pInputManager->SetFocusInput(INPUT_BUFFER_5);								
								}
								else
								{
									InvReCallItem(ITEM_LARGE);
									DisplayMessageAdd(g_Message[ETC_MESSAGE1417].szMessage, TEXT_COLOR_YELLOW); // "거래할 수 없는 아이템입니다."
								}
							}
							else
							{								
								SetItemPacket(pPacket, 57, nIndex, 0, 0, 0);
								pPacket->dwMoney = g_pMainPlayer->m_dwPlayserShop;
								g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
							}	
							
							return TRUE;
						}
						else
						{
							for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
							{
								if(g_pMainPlayer->m_sPlayerShop.cItem[i].GetID()==0)
								{
									if(g_pMainPlayer->m_dwPlayserShop==0)
									{
										if( g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_PLAYERSHOP )
										{
											g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_PLAYERSHOP,nIndex,1,1,0);
											return TRUE;
										}

										// Process //
//										sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 노점...
										CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
										if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
										{
											CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
											pQuantityWnd->OpenWnd(__QUANTITY_PLAYERSHOP__);
											pQuantityWnd->m_byZipCode = i;
											
											RECT rt;
											rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
											rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
											rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
											rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
											int nOrder	= pQuantityWnd->GetStartOrder()+2;						

											g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
											g_pInputManager->SetFocusInput(INPUT_BUFFER_5);
										}
										else
										{
											InvReCallItem(ITEM_LARGE);
											DisplayMessageAdd(g_Message[ETC_MESSAGE1417].szMessage, TEXT_COLOR_YELLOW); // "거래할 수 없는 아이템입니다."
										}
									}
									else
									{
										SetItemPacket(pPacket, 57, i, 0, 0, 0);
										pPacket->dwMoney = g_pMainPlayer->m_dwPlayserShop;
										g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
									}

									return TRUE;
								}
							}
						}
					}
					else
					{
						if(g_pMainPlayer->m_sPlayerShop.cItem[nIndex].GetID()==0)
						{
							if(g_pMainPlayer->m_dwPlayserShop==0)
							{
								if( g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_PLAYERSHOP )
								{
									g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_PLAYERSHOP,nIndex,1,1,0);
									return TRUE;
								}
//								sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 노점...
								CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
								if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
								{
									CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
									pQuantityWnd->OpenWnd(__QUANTITY_PLAYERSHOP__);
									pQuantityWnd->m_byZipCode = nIndex;

									RECT rt;
									rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
									rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
									rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
									rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
									int nOrder	= pQuantityWnd->GetStartOrder()+2;

									g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
									g_pInputManager->SetFocusInput(INPUT_BUFFER_5);
								}
								else
								{
									InvReCallItem(ITEM_SMALL);
									DisplayMessageAdd(g_Message[ETC_MESSAGE1417].szMessage, TEXT_COLOR_YELLOW); // "거래할 수 없는 아이템입니다."
								}
							}
							else
							{	
								CPlayerShopWnd* pPlayerShopWnd	= CPlayerShopWnd::GetInstance();
								CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();

								CTDS_ITEM_PICKUP ItemPickup;	
								ItemPickup.bSectionNum	= 1;
								ItemPickup.i64ItemID	= 0;
								ItemPickup.bInv			= 59;
								ItemPickup.dwMoney		= g_pMainPlayer->m_MouseItem.GetQuantity();
								ItemPickup.bZipCode		= nIndex;
								ItemPickup.dwId			= pPlayerShopWnd->m_dwMoney;
								g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								pQuantityWnd->m_bReq	= TRUE;
							}

							return TRUE;
						}
						else
						{
							for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
							{
								if(g_pMainPlayer->m_sPlayerShop.cItem[i].GetID()==0)
								{
									if(g_pMainPlayer->m_dwPlayserShop==0)
									{
										if( g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_PLAYERSHOP )
										{
											g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_PLAYERSHOP,nIndex,1,1,0);
											return TRUE;
										}
										
										// Process //
//										sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 노점...
										CBaseItem* lpItemTalbe = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.GetID());
										if(lpItemTalbe && lpItemTalbe->GetMovable() == 0)
										{
											CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
											pQuantityWnd->OpenWnd(__QUANTITY_PLAYERSHOP__);
											pQuantityWnd->m_byZipCode = i;
											
											RECT rt;
											rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
											rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
											rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
											rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
											int nOrder	= pQuantityWnd->GetStartOrder()+2;						

											g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
											g_pInputManager->SetFocusInput(INPUT_BUFFER_5);
										}
										else
										{
											InvReCallItem(ITEM_SMALL);
											DisplayMessageAdd(g_Message[ETC_MESSAGE1417].szMessage, TEXT_COLOR_YELLOW); // "거래할 수 없는 아이템입니다."
										}
									}
									else
									{
										SetItemPacket(pPacket, 57, i, 0, 0, 0);
										pPacket->dwMoney = g_pMainPlayer->m_dwPlayserShop;
										g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
									}

									return TRUE;
								}
							}
						}
					}
				}
				else
				{
					if(g_pMainPlayer->m_MouseItem.GetID()!=0)
					{
						int nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;

						CTDS_ITEM_PICKUP ItemPickup;
						ItemPickup.bSectionNum	= 1;
						ItemPickup.i64ItemID	= 0;

						if(	CQuantityWnd::GetInstance()->m_bQuantityType==__QUANTITY_PLAYERSHOP__ ||
							CQuantityWnd::GetInstance()->m_bQuantityType==__QUANTITY_PLAYERSHOPCNT__)
						{
							// 인벤토리에 넣기 //
							if(GetType(nValue)==ITEM_SMALL)
							{
								InvSmallInsert(&ItemPickup, &g_pMainPlayer->m_MouseItem);
							}
							else
							{
								InvLargeInsert(&ItemPickup);
							}

							CQuantityWnd::GetInstance()->m_bReq = TRUE;
						}
					}
				}
			}					
			else if(g_Mouse.MousePos.x>pPlayerShopWnd->m_fPosX &&
					g_Mouse.MousePos.x<pPlayerShopWnd->m_fPosX+CPlayerShopWnd::GetInstance()->m_fMaxSizeWidth &&
					g_Mouse.MousePos.y>pPlayerShopWnd->m_fPosZ &&
					g_Mouse.MousePos.y<pPlayerShopWnd->m_fPosZ+CPlayerShopWnd::GetInstance()->m_fMaxSizeHeight)
			{
				if(g_pMainPlayer->m_MouseItem.GetID()!=0)
				{
					CTDS_ITEM_PICKUP ItemPickup;
					if(InsertItem(&ItemPickup))
						return TRUE;
				}				
			}
			else
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					g_ItemMoveManager.Initialize();
				}

				if(g_pMainPlayer->m_MouseItem.GetID()!=0)
				{
					int nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;

					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;
					
					// 인벤토리에 넣기 //
					if(GetType(nValue)==ITEM_SMALL)							
					{
						InvSmallInsert(&ItemPickup, &g_pMainPlayer->m_MouseItem);
					}
					else
					{
						InvLargeInsert(&ItemPickup);						
					}
				}

				return TRUE;
			}
		}			
		
		if(g_pMainPlayer->m_MouseItem.GetID()!=0)
		{
			int nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;

			CTDS_ITEM_PICKUP ItemPickup;
			ItemPickup.bSectionNum	= 1;
			ItemPickup.i64ItemID	= 0;
			
			// 인벤토리에 넣기 //
			if(GetType(nValue)==ITEM_SMALL)
			{
				InvSmallInsert(&ItemPickup, &g_pMainPlayer->m_MouseItem);
			}
			else
			{
				InvLargeInsert(&ItemPickup);						
			}

			return TRUE;
		}	
	}
	
	return FALSE;
}


void InvSmallInsert(CTDS_ITEM_PICKUP* pPacket, CItem* pItem)
{
	int nValue = pItem->GetID()/ITEM_DISTRIBUTE;
	
	if(GetType(nValue)==ITEM_SMALL)	
	{
		BOOL bChk = FALSE;

		for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
			{
				pPacket->bInv		= 14;
				pPacket->bZipCode	= i;
				bChk				= TRUE;
				break;
			}
			else
			{
				if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
				{
					if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<100)
					{
						pPacket->bInv		= 14;
						pPacket->bZipCode	= i;
						bChk			= TRUE;
						break;
					}
				}															
			}
		}
							
		if(bChk==FALSE)
			pPacket->bInv	= 4;
		
		g_pNet->SendMsg((char*)pPacket, pPacket->GetPacketSize(), SERVER_INDEX_ZONE);
	}
}


void SetRHandChange(CUser* pUser, DWORD dwId)
{	
	int nIndex = dwId/ITEM_DISTRIBUTE;

	// R Mod 교체 //	
	if(pUser->m_hPlayerRHand.pHandle)
	{
		ObjDeleteDesc(&pUser->m_hPlayerRHand.pDesc);
		ItemDeleteObject(&pUser->m_hPlayerRHand.pHandle, &pUser->m_hPlayer.pHandle);
	}	
	if(dwId!=0)	
		ItemAttach(dwId, &pUser->m_hPlayerRHand.pHandle, pUser->m_hPlayerRHand.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_RHAND);
		
	if(dwId==0)
	{
		pUser->m_byItemType	= 0;		
		pUser->SetAction(MOTION_TYPE_WARSTAND, g_pExecutive->GXOGetCurrentFrame( pUser->m_hPlayer.pHandle ), ACTION_LOOP );
		pUser->m_hPlayer.pDesc->ObjectFunc	= NULL;				
		pUser->SetStatus(UNIT_STATUS_NORMAL);
	}		
	else
	{
		pUser->m_byItemType	= nIndex+1;		
		pUser->SetAction(MOTION_TYPE_WARSTAND, g_pExecutive->GXOGetCurrentFrame( pUser->m_hPlayer.pHandle ), ACTION_LOOP );
		pUser->m_hPlayer.pDesc->ObjectFunc	= NULL;
		pUser->SetStatus(UNIT_STATUS_NORMAL);
	}
}

void SetLHandChange(CUser* pUser, DWORD dwId)
{
	// L Mod 교체 //
	if(pUser->m_hPlayerLHand.pHandle)
	{
		ObjDeleteDesc(&pUser->m_hPlayerLHand.pDesc);
		ItemDeleteObject(&pUser->m_hPlayerLHand.pHandle, &pUser->m_hPlayer.pHandle);
	}

	if(dwId!=0)
		ItemAttach(dwId, &pUser->m_hPlayerLHand.pHandle, pUser->m_hPlayerLHand.pDesc, &pUser->m_hPlayer.pHandle, ITEM_ATTACH_TYPE_LHAND);	
}

BOOL InvReCallItem(BYTE bItemKind) // 아이템을 아이템창으로 복귀 시키는 패킷을 서버로 전송하는 함수
{
	CTDS_ITEM_PICKUP	ItemPickup;
	
	ItemPickup.bSectionNum	= 1;
	ItemPickup.i64ItemID	= 0;

	int i=0;

	if(bItemKind == ITEM_LARGE)
	{
		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Large[i].m_wItemID==0)
			{
				SetItemPacket(&ItemPickup, 6, i, 0, 0, 0);
				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				return TRUE;
			}
		}
	}
	else if(bItemKind == ITEM_SMALL)
	{
		for(i = 0; i < MAX_INV_SMALL_POOL; i++)
		{
			if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
			{
				SetItemPacket(&ItemPickup, 14, i, 0, 0, 0);
				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				return TRUE;
			}
		}
	}

	return FALSE;
}