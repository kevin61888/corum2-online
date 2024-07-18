#include "stdafx.h"
#include "recvmsg.h"
#include "MsgProc.h"
#include "OwnServer.h"
#include "DungeonTable.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonLayer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "Monster.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "AcceptTable.h"
#include "MsgProcForServer.h"
#include "MsgProcForServerGuild.h"
#include "MsgProcForServerParty.h"
#include "MsgProcForServerPortal.h"
#include "RecordSet.h"
#include "DBProc.h"
#include "MsgProc_for_jjw6263.h"
#include "MsgProc_for_minbobo.h"
#include "MsgProc_for_Yak.h"
#include "MsgProc_for_Y2jin.h"
#include "MonsterManager.h"
#include "GameSystemNative.h"
#include "EventDungeonManager.h"
#include "MsgProcForServerEvent.h"
#include "../CommonServer/CommonHeader.h"


void QueryTypePresentCountCheck(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)pResult->pData;
	if(!pUser) return;
	
	DSTC_ITEM_PICKUP	PickupItem;
	memset( &PickupItem, 0, sizeof( DSTC_ITEM_PICKUP ) );
	PickupItem.bStatus	= UPDATE_GAME_PLAY;
	PickupItem.bHeader	= Protocol_DSTC::CMD_ITEM_PICKUP;

	if(pResult->nError<0)
	{
		Log(LOG_IMPORTANT, "QueryPresentCountCheck Failed! (Name:%s)", pUser->m_szName );
		//에러 메세지 
		PickupItem.bEqiupCode2 = 254;		//쿼리에러!
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		return;
	}

	BYTE byPackCnt=0;
	CItem cItem;
	memset(&cItem, 0, sizeof(cItem));
	
	DB_PRESENT_COUNT_RESULT Present;
	memcpy(&Present, pResult->Query.select.pResult, pResult->Query.select.dwRowSize);

//	if(Present.dwResult == 2)	//최대 공급량 초과 //아직은 절대 풀지마세여
//	{
//		PickupItem.bEqiupCode2 = 0xff;		//꽝이요!
//		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
//		return;		
//	}
	if(Present.dwResult == 255)				//해당하는 아이템 없음 
	{
		Log(LOG_IMPORTANT, "No Present Item! (Item_id:%d, Item_quantity:%d)", Present.wItemID, Present.dwItemQuantity );
		return;
	}

//	int nValue = Present.wItemID / ITEM_DISTRIBUTE;
	if(GetItemType(Present.wItemID)==ITEM_TYPE_SMALL)
	{
		//쿼리 결과가 오는 사이에 그자리에 다른 아이템이 들어온 경우 
		if(pUser->m_pInv_Small[ Present.dwEmptyPos ].GetID() != 0)
		{
			//새로운 빈자리를 찾는다. 
			int nResult = GetUserEmptyInventoryPos(pUser, TRUE);

			if(nResult < 0)	//빈자리가 없으면 꽝으로 처리 
			{
				PickupItem.bEqiupCode2 = 0xff;		//꽝이요!
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
				Log(LOG_IMPORTANT, "Cannot find empty inventory position!");
				return;
			}
			else
			{
				Present.dwEmptyPos = nResult;
			}
		}

		//Item 넣어주고 
#ifdef JAPAN_LOCALIZING
		CreateItem(&cItem, (WORD)Present.wItemID, (BYTE)Present.dwItemQuantity, Store_Flag_Out_Store, TRUE);
#else
		CreateItem(&cItem, (WORD)Present.wItemID, (BYTE)Present.dwItemQuantity);
#endif

		byPackCnt = (BYTE)Insert_SmallInvItem(pUser, &cItem, (BYTE)Present.dwEmptyPos, (BYTE)Present.dwItemQuantity, TRUE, FALSE);//byPackCnt는 Inventory에 꽉차서 못넣은 갯수를 반환한다. 
	}

	if(GetItemType(Present.wItemID)==ITEM_TYPE_LARGE)
	{
		//쿼리 결과가 오는 사이에 그자리에 다른 아이템이 들어온 경우 
		if(pUser->m_pInv_Large[ Present.dwEmptyPos ].GetID() != 0)
		{
			//새로운 빈자리를 찾는다. 
			int nResult = GetUserEmptyInventoryPos(pUser, FALSE);

			if(nResult < 0)	//빈자리가 없으면 꽝으로 처리 
			{
				PickupItem.bEqiupCode2 = 0xff;		//꽝이요!
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
				return;
			}
			else
			{
				Present.dwEmptyPos = nResult;
			}
		}

		//Item 넣어주고 
#ifdef JAPAN_LOCALIZING
		CreateItem(&cItem, (WORD)Present.wItemID, (BYTE)Present.dwItemQuantity, Store_Flag_Out_Store, TRUE);
#else
		CreateItem(&cItem, (WORD)Present.wItemID, (BYTE)Present.dwItemQuantity);
#endif

		memcpy(&pUser->m_pInv_Large[ Present.dwEmptyPos ], &cItem, sizeof(CItem));
		byPackCnt = 0;
	}

	//클라이언트에게 아이템 생성된 정보를 알려줌 
	memcpy(&PickupItem.Item, &cItem, sizeof(CItem));
	PickupItem.Item.SetQuantity((BYTE)(Present.dwItemQuantity - byPackCnt));
	PickupItem.bInv			= 78;
	PickupItem.bZipCode		= (BYTE)Present.dwEmptyPos;
	PickupItem.dwId			= Present.wItemID;				
	
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&PickupItem, PickupItem.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION );
}


void QueryTypeUserCharacter(DBRECEIVEDATA* pResult)
{
	int		nCount	= pResult->Query.select.dwRowCount;
	CUser*	pUser	= (CUser*)g_pNet->GetUserInfo( (DWORD)pResult->pData );

	if(pUser)
	{					
		for(int i = 0; i < nCount; i++)
		{
			memcpy(&pUser->m_pUserCharacterInfo[i]
				, pResult->Query.select.pResult+(i*sizeof(DB_USER_CHARACTERINFO)), sizeof(DB_USER_CHARACTERINFO));					
		}

		pUser->m_bCount = (BYTE)nCount;				
	}
	else
	{
		Log( LOG_NORMAL, "pUser is NULL In ReceivedFromDB::DB_USER_CHARACTER " );
	}
}


void QueryTypeBankUserInfo(DBRECEIVEDATA* pResult)
{
	DSTC_BANK_INFO	pBankInfo;
	CUser* pUser = (CUser*)g_pNet->GetUserInfo( (DWORD)pResult->pData );

	if (pUser)
	{						
		DB_BANK_USER*	pBankUser	= (DB_BANK_USER*)pResult->Query.select.pResult;
		
		if(pBankUser)
		{
			CDungeon* pDungeon = pUser->GetCurDungeon();
			if (!pDungeon->PermissionJoinDungeon(pUser))
				return;

			pUser->m_dwBankMoney	= pBankUser->dwMoney;
			pBankInfo.dwMoney		= pBankUser->dwMoney;

			memcpy(pUser->m_pBank_Large, pBankUser->sBankItemLarge, sizeof(pUser->m_pBank_Large));
			memcpy(pUser->m_pBank_Small, pBankUser->sBankItemSamll, sizeof(pUser->m_pBank_Small));	
			
			for(int i = 0; i < MAX_BANK_SMALL_POOL; i++)
			{
				if(pBankUser->sBankItemSamll[i].GetID() != 0)
				{
					pBankInfo.Items[pBankInfo.bSmallItemCount].index = BYTE(i);
					memcpy(&pBankInfo.Items[pBankInfo.bSmallItemCount++].cItem
						, &pBankUser->sBankItemSamll[i], sizeof(CItem));
				}							
			}

			for(i = 0; i < MAX_BANK_LARGE_POOL; i++)
			{
				if(pBankUser->sBankItemLarge[i].GetID() != 0)
				{
					pBankInfo.Items[pBankInfo.bSmallItemCount + pBankInfo.bLargeItemCount].index = BYTE(i);
					memcpy(&pBankInfo.Items[pBankInfo.bSmallItemCount + pBankInfo.bLargeItemCount++].cItem, 
						&pBankUser->sBankItemLarge[i], sizeof(CItem));
				}
			}
			
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pBankInfo, pBankInfo.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);					
		}	
		else
		{
			char szLog[0xff]={0,};
			wsprintf("Name = %s, Char_Index = %u (BankInfo Fail)", pUser->m_szName, pUser->GetID());
			Log(LOG_NORMAL, szLog);
		}
	}
	else
	{
		Log( LOG_NORMAL, "pUser is NULL In ReceivedFromDB::DB_BANK_USER_INFO " );
	}
}


void QueryTypeUserPlayerShop(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo( (DWORD)pResult->pData );

	if(pUser)
	{
		if(!pUser->GetCurDungeon())
			return;

		BOOL bChk = FALSE;

		PLAYERSHOP_DB	sPlayerShopDB;
		DSTC_SHOP_ITEM	pPacket;
			
		memcpy(&sPlayerShopDB, pResult->Query.select.pResult, pResult->Query.select.dwRowSize );					
		memcpy(pUser->m_sPlayerShop.cItem, sPlayerShopDB.cItem, sizeof(CItem)*MAX_PLAYER_SHOP_INV);

		for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
			if(pUser->m_sPlayerShop.cItem[i].GetID()!=0)
			{
				for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
				{								
					if(pUser->m_sPlayerShop.cItem[i].GetSerial()->i64Serial ==
						pUser->m_pInv_Large[j].GetSerial()->i64Serial)
					{
						memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
						bChk = TRUE;
						continue;
					}
				}
				
				for(j = 0; j < MAX_INV_SMALL_POOL; j++)
				{								
					if(pUser->m_sPlayerShop.cItem[i].GetSerial()->i64Serial == 
						pUser->m_pInv_Small[j].GetSerial()->i64Serial)
					{
						memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
						bChk = TRUE;
						continue;
					}
				}
				
				for(j = 0; j < MAX_EQUIP_POOL; j++)
				{								
					if(pUser->m_sPlayerShop.cItem[i].GetSerial()->i64Serial ==
						pUser->m_pEquip[j].GetSerial()->i64Serial)
					{
						memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
						bChk = TRUE;
						continue;
					}
				}
			}						
		}					

		if(pUser->GetCurDungeon()->GetDungeonKind() == SERVER_TYPE_VILLAGE)
		{
			for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
			{
				if(pUser->m_sPlayerShop.cItem[i].GetID()!=0)
				{
					for(int j = 0; j < MAX_BANK_LARGE_POOL; j++)
					{								
						if(pUser->m_sPlayerShop.cItem[i].GetSerial()->i64Serial == 
							pUser->m_pBank_Large[j].GetSerial()->i64Serial)
						{
							memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
							bChk = TRUE;
							continue;
						}
					}
					
					for(j = 0; j < MAX_BANK_SMALL_POOL; j++)
					{								
						if(pUser->m_sPlayerShop.cItem[i].GetSerial()->i64Serial ==
							pUser->m_pBank_Small[j].GetSerial()->i64Serial)
						{
							memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
							bChk = TRUE;
							continue;
						}
					}
				}						
			}
		}
		
		if(bChk)
		{
			PlayerShopLogAll(pUser);
		}

		for(i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
			if(pUser->m_sPlayerShop.cItem[i].GetID() != 0)
			{
				pPacket.Nodes[pPacket.bItemCount].index = BYTE(i);
				memcpy(&pPacket.Nodes[pPacket.bItemCount++].cItem
					, &pUser->m_sPlayerShop.cItem[i]
					, sizeof(CItem) );
			}
		}
		
		NetSendToUser(pUser->m_dwConnectionIndex
			, (char*)&pPacket, pPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		Log( LOG_NORMAL, "pUser is NULL In ReceivedFromDB::DB_USER_PLAYERSHOP " );
	}
}


void QueryTypeUUIDDungeonJoin(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo( (DWORD)pResult->pData );

	if (pUser)
	{						
		if(pResult->Query.select.dwRowCount == 0)
		{
			Log(LOG_IMPORTANT, "Query Failed! (DB_UUID_DUNGEON_JOIN)");
			if(pUser->pBufForPortal)
			{
				LALFree(g_pPortalBufferPool, pUser->pBufForPortal);
				pUser->pBufForPortal = NULL;
				//__asm int 3
			}
			return;
		}
		
		pUser->SetQueryUserInfo(pResult->Query.select.pResult);
		
		if(pUser->GetCurDungeon()->GetDungeonKind()==SERVER_TYPE_VILLAGE)
		{
			// 은행 메세지 //
			char szSQL[1024]={0,};
			wsprintf(szSQL, "select * From BankInfo where [ID] = '%s'", pUser->m_szId);
			g_pDb->THOpenRecord(szSQL, DB_BANK_USER_INFO, (void*)pUser->m_dwConnectionIndex, 1, GAME_DB);						
		}
		else
		{
 			CDungeon* pDungeon = pUser->GetCurDungeon();
			if (!pDungeon->PermissionJoinDungeon(pUser))
				return;
		}										
		
		// 계정안에 Character 길드, 클랜 정보 					
		char szSQL[0xff]={0,};

		wsprintf(szSQL, "Select Type, [Name] From CharacterInfo Where [ID] = '%s'", pUser->m_szId);
		g_pDb->THOpenRecord(szSQL, DB_USER_CHARACTER, (void*)pUser->m_dwConnectionIndex, 4, GAME_DB);

		if( IsUltra() )
		{
			
		}
		else
		{
			// 노점 데이터 쿼리 
			wsprintf(szSQL, "Select * From PlayerShop Where [Char_Index] = %u", pUser->GetID());
			g_pDb->THOpenRecord(szSQL, DB_USER_PLAYERSHOP, (void*)pUser->m_dwConnectionIndex, 1, GAME_DB);
		}						
	}
	else
	{
		Log( LOG_NORMAL, "pUser is NULL In ReceivedFromDB::DB_UUID_DUNGEON_JOIN " );
	}
}


void QueryTypeCreateGuardian(DBRECEIVEDATA* pResult)
{
	LPGUARDIAN_CREATION_INFO pInfo = (LPGUARDIAN_CREATION_INFO)pResult->pData;
					
	// Error처리 해주고.
	if( pResult->nError < 0 ) 
	{
		pInfo->pUser->m_pInv_Guardian[ pInfo->bZipCode ].m_Item_Guardian.bCreate =
			GUARDIAN_CREATE_STATUS_NONE;
		delete pInfo;
		return;
	}

	MY_DB_NUMERIC Result;
	memcpy( &Result, pResult->Query.select.pResult, sizeof( MY_DB_NUMERIC ) );
	
	if( pInfo->bUserGuardian )
	{
		pInfo->pUser->m_pInv_Guardian[ pInfo->bZipCode ].m_Item_Guardian.bCreate	= GUARDIAN_CREATE_STATUS_ALREADYCALL;	// 생성됨을 표시.
		pInfo->pUser->m_pInv_Guardian[pInfo->bZipCode].m_Item_Guardian.dwID			= DWORD(Result.least);

		// 이제 생성된 가디언을 소환한다.
		char szSQL[0xff]={0,};
		wsprintf( szSQL, "select * from GuardianInfo where ID=%d", pInfo->pUser->m_pInv_Guardian[pInfo->bZipCode].m_Item_Guardian.dwID );
		g_pDb->THOpenRecord( szSQL, DB_UUID_GUARDIAN_INFO, pInfo, 1, GAME_DB );

		SendToUpdate_GuardianItem(pInfo->pUser->GetConnectionIndex(), &pInfo->pUser->m_pInv_Guardian[pInfo->bZipCode]);
	}
	else
	{
		pInfo->pItem->m_Item_Guardian.bCreate	= GUARDIAN_CREATE_STATUS_ALREADYCALL;	// 생성됨을 표시.
		pInfo->pItem->m_Item_Guardian.dwID		= DWORD(Result.least);

		// 이제 생성된 가디언을 소환한다.
		char szSQL[0xff]={0,};
		wsprintf( szSQL, "select * from GuardianInfo where ID=%d", pInfo->pItem->m_Item_Guardian.dwID );
		g_pDb->THOpenRecord( szSQL, DB_UUID_GUARDIAN_INFO, pInfo, 1, GAME_DB );
	}
}


void QueryTypeUUIDGuardianInfo(DBRECEIVEDATA* pResult)
{
	LPGUARDIAN_INFO	pGuardian = (LPGUARDIAN_INFO)pResult->Query.select.pResult;
					
	LPGUARDIAN_CREATION_INFO pInfo = (LPGUARDIAN_CREATION_INFO)pResult->pData;
	 
	if( pResult->Query.select.dwRowCount == 0 )
	{
		delete pInfo;
		return;
	}

	if( pInfo->pUser )			
	{
		// 개인 가디언 이다.
		if( pInfo->pUser->GetCurDungeon() )
		{
			if(pInfo->pUser->m_pGuardian[0])
			{
				delete pInfo;
				return;
			}

			if(	pInfo->pUser->GetUnitStatus()		== UNIT_STATUS_DEAD ||
				pInfo->pUser->GetCurLayerIndex()	== INVALID_DUNGEON_LAYER)
			{
				delete pInfo;
				return;
			}

			pInfo->pUser->GetCurDungeonLayer()->CallGuardian(pInfo->pUser
				, pInfo->bZipCode
				, pGuardian );
		}
	}
	else if( pInfo->pDungeon )	
	{
		// 던젼 수호 가디언이겠지...
		// 최하층 레이어에 수호 가디언을 생성한다.
		pInfo->pDungeon->GetDungeonLayer(BYTE(pInfo->pDungeon->GetTotalLayer() - 1))->CallDungeonGuardian( pGuardian );
	}
	
	delete pInfo;
}


void QueryTypeChangeDungeonOwner(DBRECEIVEDATA* pResult)
{
	DB_CHANGE_DUNGEON* pChangeDungeon = (DB_CHANGE_DUNGEON*)pResult->pData;
	CDungeon* pDungeon = (CDungeon*)pChangeDungeon->pDungeon;

	pDungeon->GetDungeonDataEx()->SetOwner(pChangeDungeon->dwUserIndex, pChangeDungeon->dwUserGuildId, pChangeDungeon->szLoginID, pChangeDungeon->szName);
	pDungeon->RestartSiege();
}


void QueryTypeUserPlayershop(DBRECEIVEDATA* pResult)
{
	PLAYERSHOP_LOG* lpPlayerShopLog = (PLAYERSHOP_LOG*)pResult->pData;
	PLAYERSHOP_LOG	pPlayerShopLog;

	if(!lpPlayerShopLog)
		return;		
	
	memcpy(&pPlayerShopLog, lpPlayerShopLog, sizeof(PLAYERSHOP_LOG));				
										
	if(g_pPlayerShopLogHash->GetData(pPlayerShopLog.dwKey)==NULL)				
		return;			
	
	g_pPlayerShopLogHash->Delete(lpPlayerShopLog, lpPlayerShopLog->dwKey, TRUE);
		
	CUser* pUser = g_pUserHash->GetData(pPlayerShopLog.dwUserIndex);

	if(!pUser)
	{
		// 에러 처리 //
		if(pResult->nError < 0)
		{
			return;
		}
		else
		{
			switch(pPlayerShopLog.bInv)
			{
			case 58:
				PlayerShopLog(pUser, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], 0, pPlayerShopLog.byZipCode, 0, 0, FALSE);
				return;
			case 60:
				PlayerShopLog(pUser, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], 0, pPlayerShopLog.byZipCode, 0, 0, FALSE);
				return;
			}
		}
		
		return;
	}

	if(pResult->nError < 0)
	{
		DSTC_ITEM_PICKUP pPacket;

		if(pUser->GetID()==22)
		{
			char szErr[0xff] = {0, };
			wsprintf(szErr, "PlayerShop DB Error : %u", pPlayerShopLog.bInv);
			Log(LOG_NORMAL, szErr);
		}

		switch(pPlayerShopLog.bInv)
		{
		case 57:
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_LARGE)
				{
					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].GetID()==0)
						{							
							pPacket.bInv		= 6;
							pPacket.bZipCode	= BYTE(i);
							memcpy(&pPacket.Item, &pPlayerShopLog.cItem, sizeof(CItem));
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
							NetSendToUser( pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
							return;
						}
					}

					if(pUser->GetCurDungeon())
					{
						if(pUser->GetCurDungeon()->GetDungeonDataEx()->IsVillage())
						{
							for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pBank_Large[i].GetID()==0)
								{
									pPacket.bInv		=	24;
									pPacket.bZipCode	=	BYTE(i);
									memcpy(&pUser->m_pBank_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
									memcpy(&pPacket.Item, &pUser->m_ItemMouse, sizeof(CItem));
									memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
									NetSendToUser( pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
									return;
								}
							}																		
						}
					}		
				}
				
				AppearItem(pUser, &pPlayerShopLog.cItem, pPlayerShopLog.cItem.GetQuantity(), 0, ITEM_TYPE_LARGE);
			}
			return;
		case 59:
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
				{
					memcpy(&pPacket.Item, &pUser->m_ItemMouse, sizeof(CItem));

					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

					pPacket.bInv		= 14;
					pPacket.bZipCode	= 0;																				
					NetSendToUser( pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );								
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				}
				return;
			}
			return;
		}
	}
	else
	{
		// 디비 로그 성공.

		DSTC_ITEM_PICKUP pPacket;

		switch(pPlayerShopLog.bInv)
		{
		case 57:	// 장비아이템 마우스에서 노점으로.
			{
				pUser->m_dwPlayserShop	= pPlayerShopLog.dwPrice;
				pPacket.bInv			= pPlayerShopLog.bInv;
				pPacket.bZipCode		= pPlayerShopLog.byZipCode;
				pPacket.bZipCode2		= 1;
				pPacket.dwMoney			= pPlayerShopLog.dwPrice;							
				pUser->m_sPlayerShop.dwCustomSellPrice[pPlayerShopLog.byZipCode] = pPlayerShopLog.dwPrice;
				memcpy(&pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], &pPlayerShopLog.cItem, sizeof(CItem));
				memcpy(&pPacket.Item, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));														
				NetSendToUser( pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

				if(pUser->GetID()==22)
				{
					char szErr[0xff] = {0, };
					wsprintf(szErr, "PlayerShop Item Move");
					Log(LOG_NORMAL, szErr);
				}
			}
			return;
		case 58:	// 장비아이템 노점에서 마우스로.
			{
				pUser->m_dwPlayserShop	= pUser->m_sPlayerShop.dwCustomSellPrice[pPlayerShopLog.byZipCode];							
				pPacket.bInv			= pPlayerShopLog.bInv;
				pPacket.bZipCode		= pPlayerShopLog.byZipCode;
				pPacket.dwMoney			= pPlayerShopLog.dwPrice;
				pUser->m_sPlayerShop.dwCustomSellPrice[pPlayerShopLog.byZipCode] = 0;
				memcpy(&pUser->m_ItemMouse, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
				memcpy(&pPacket.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], 0, sizeof(CItem));
				NetSendToUser( pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );														
			}
			return;
		case 59:	// 소비아이템 마우스에서 노점으로.
			{
				memcpy(&pPacket.Item, &pUser->m_ItemMouse, sizeof(CItem));

				if(pUser->m_ItemMouse.GetQuantity()>=pPlayerShopLog.dwCnt)
				{
					BYTE	byPacketCount	= BYTE(pPlayerShopLog.dwCnt);	
					BYTE	byOutCount		= BYTE(pUser->m_ItemMouse.GetQuantity()-pPlayerShopLog.dwCnt);
					BYTE	byResult		= 0;
										
					if(byPacketCount==pUser->m_ItemMouse.GetQuantity())
						byResult = Insert_PlayerShopItem(pUser, &pUser->m_ItemMouse, pPlayerShopLog.byZipCode, pUser->m_ItemMouse.GetQuantity());
					else
					{
						CItem cSmallItem;									
						memcpy(&cSmallItem, &pUser->m_ItemMouse, sizeof(CItem));

						if(pUser->m_ItemMouse.GetQuantity()>pPlayerShopLog.dwCnt)
						{										
							cSmallItem.SetQuantity(BYTE(pPlayerShopLog.dwCnt));
							cSmallItem.SetSerialNum(g_pThis->GetNewSerial());
						}
						pUser->m_ItemMouse.SetQuantity((BYTE)pPlayerShopLog.dwCnt);
						
						byResult = Insert_PlayerShopItem(pUser, &cSmallItem, pPlayerShopLog.byZipCode, BYTE(pPlayerShopLog.dwCnt));

						if(byOutCount!=0)
						{
							pUser->m_ItemMouse.SetQuantity(byOutCount);
							Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, byOutCount, TRUE);
						}
					}
					
					pUser->m_sPlayerShop.dwCustomSellPrice[byResult] = pPlayerShopLog.dwPrice;
					pUser->m_dwPlayserShop	= pPlayerShopLog.dwPrice;
					pPacket.bInv			= pPlayerShopLog.bInv;
					pPacket.dwId			= pPlayerShopLog.dwPrice;
					pPacket.dwMoney			= pPlayerShopLog.dwCnt;
					pPacket.bZipCode		= pPlayerShopLog.byZipCode;					
					pPacket.bZipCode2		= byResult;
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					NetSendToUser( pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
				}
			}
			return;
		case 60:	// 소비아이템 노점에서 마우스로.
			{
				pUser->m_dwPlayserShop	= pUser->m_sPlayerShop.dwCustomSellPrice[pPlayerShopLog.byZipCode];
				pPacket.bInv			= pPlayerShopLog.bInv;
				pPacket.bZipCode		= pPlayerShopLog.byZipCode;
				pPacket.dwMoney			= pPlayerShopLog.dwCnt;
				pPacket.dwId			= pPlayerShopLog.dwPrice;
				pUser->m_sPlayerShop.dwCustomSellPrice[pPlayerShopLog.byZipCode] = 0;

				memcpy(&pUser->m_ItemMouse, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
				memcpy(&pPacket.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], 0, sizeof(CItem));
				NetSendToUser( pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			}
			return;
		case 72:	// PlayerShop에서 살때(장비아템)
			{	
				if(!pUser->GetCurDungeon())
					return;

				CUser*			pPriceUser	= g_pUserHash->GetData(pPlayerShopLog.dwOwnUserIndex);
				CDungeonLayer*	pLayer		= pUser->GetCurDungeonLayer();

				if(!pPriceUser)
					return;
				
				if(!pLayer)
					return;							

				// 구매자에게 패킷 보내기//
				pPacket.bInv			= pPlayerShopLog.bInv;
				pPacket.bZipCode		= pPlayerShopLog.byOwnZipCode;
				pPacket.bZipCode2		= pPlayerShopLog.byOwnZipCode2;
				pPacket.dwMoney			= pPlayerShopLog.dwOwnPrice;
				pPriceUser->m_dwMoney	= pPlayerShopLog.dwOwnPrice;							
				memcpy(&pPacket.Item, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
				memcpy(&pPriceUser->m_pInv_Large[pPlayerShopLog.byOwnZipCode], &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
				NetSendToUser(pPriceUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

				// 판매자에게 패킷 보내기 //
				pUser->m_dwMoney += pPlayerShopLog.dwPrice;
				memset(&pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], 0, sizeof(CItem));
				pUser->m_sPlayerShop.dwCustomSellPrice[pPlayerShopLog.byZipCode] = 0;

				DSTC_PLAYERSHOP_SELL pSellPacket;
				pSellPacket.dwMoney		= pUser->m_dwMoney;
				pSellPacket.byZipCode	= pPlayerShopLog.byZipCode;
				memcpy(&pSellPacket.cItem, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSellPacket, pSellPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

				BOOL bSoldOut = TRUE;

				for(int j = 0; j < MAX_PLAYER_SHOP_INV; j++)
				{
					if(pUser->m_sPlayerShop.cItem[j].GetID()!=0)
					{
						bSoldOut = FALSE;
						return;
					}
				}
					
				if(bSoldOut)
				{									
					memset(pUser->m_sPlayerShop.szTitle, 0, sizeof(pUser->m_sPlayerShop.szTitle));
					pUser->m_sPlayerShop.m_bPlayerShopMode = FALSE;

					ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pUser->GetID());

					if(pItemShop)	
						pItemShop->byType = 0;

					SHOP_TILE* pNewShopTile = g_pShopTileHash->GetData(pUser->GetID());

					if(pNewShopTile)
					{
						g_pShopTileHash->Delete(pNewShopTile, pUser->GetID());
						memset(pNewShopTile, 0, sizeof(SHOP_TILE));
						pNewShopTile = NULL;
					}
					
					DSTC_PLAYERSHOP_CLOSE pClosePacket;
					pClosePacket.dwUserIndex = pUser->GetID();
					pLayer->BroadCastMsg( (char*)&pClosePacket, pClosePacket.GetPacketSize());

					// 판매자에게 보내기 //
					DSTC_PLAYERSHOP_SOLDOUT pSoldOutPacket;
					pSoldOutPacket.byType	= 1;									
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSoldOutPacket, pSoldOutPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}

				if(!pPriceUser->GetCurDungeon())
					return;
				if(!pPriceUser->GetCurDungeon()->GetDungeonDataEx())
					return;

				SetItemLog(&pPacket.Item, ITEM_LOG_PLAYERSHOP,WORD(pPriceUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID),pUser->m_szName,pPriceUser->m_szName);
				SetItemLogKarz(	ITEM_LOG_PLAYERSHOP,WORD(pPriceUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID),pUser->m_szName,pPriceUser->m_szName,pPlayerShopLog.dwPrice,&pPacket.Item);
			}
			return;
		case 73:	// PlayerShop에서 살때(소비아템)
			{
				if(!pUser->GetCurDungeon())
					return;

				CUser*			pPriceUser	= g_pUserHash->GetData(pPlayerShopLog.dwOwnUserIndex);
				CDungeonLayer*	pLayer		= pUser->GetCurDungeonLayer();

				if(!pPriceUser)
					return;							
				if(!pLayer)
					return;	
				
				CItem cItem[MAX_INV_SMALL_POOL];
				CItem cInsertItem;

				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
					memcpy(&cItem[i], &pPriceUser->m_pInv_Small[i], sizeof(CItem));
				
				memcpy(&cInsertItem, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
				cInsertItem.SetQuantity(pPlayerShopLog.byOwnZipCode2);

				if(pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode].GetQuantity()!=pPlayerShopLog.byOwnZipCode2)
					cInsertItem.SetSerialNum(g_pThis->GetNewSerial());

				if(Insert_SmallInvItem2(cItem, &cInsertItem, 0, cInsertItem.GetQuantity(), TRUE)==0)
				{
					for(i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						memcpy(&pPriceUser->m_pInv_Small[i], &cItem[i], sizeof(CItem));
					}

					// 구매자에게 패킷 보내기//
					pPacket.bInv			= pPlayerShopLog.bInv;
					pPacket.bZipCode		= 0;
					pPacket.bZipCode2		= pPlayerShopLog.byOwnZipCode;
					pPacket.bEquipCode		= BYTE(pUser->m_sPlayerShop.cItem[pPlayerShopLog.byOwnZipCode].GetQuantity()-pPlayerShopLog.byOwnZipCode2);
					pPriceUser->m_dwMoney	= pPlayerShopLog.dwOwnPrice;
					pPacket.dwMoney			= pPriceUser->m_dwMoney;
													
					memcpy(&pPacket.Item, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
					pPacket.Item.SetQuantity(pPlayerShopLog.byOwnZipCode2);
					NetSendToUser(pPriceUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

					// 판매자에게 패킷 보내기 //
					pUser->m_dwMoney += pPlayerShopLog.dwPrice;

					if(pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode].GetQuantity()==pPlayerShopLog.byOwnZipCode2)
					{
						memset(&pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], 0, sizeof(CItem));
						pUser->m_sPlayerShop.dwCustomSellPrice[pPlayerShopLog.byZipCode] = 0;
					}
					else
					{
						pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode].SetQuantity(
							BYTE(pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode].GetQuantity()-pPlayerShopLog.byOwnZipCode2));							
					}
					
					DSTC_PLAYERSHOP_SELL pSellPacket;
					pSellPacket.dwMoney		= pUser->m_dwMoney;
					pSellPacket.byZipCode	= pPlayerShopLog.byZipCode;
					memcpy(&pSellPacket.cItem, &pUser->m_sPlayerShop.cItem[pPlayerShopLog.byZipCode], sizeof(CItem));
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSellPacket, pSellPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

					BOOL bSoldOut = TRUE;

					for(int j = 0; j < MAX_PLAYER_SHOP_INV; j++)
					{
						if(pUser->m_sPlayerShop.cItem[j].GetID()!=0)
						{
							bSoldOut = FALSE;
							return;
						}
					}
					
					if(bSoldOut)
					{									
						memset(pUser->m_sPlayerShop.szTitle, 0, sizeof(pUser->m_sPlayerShop.szTitle));
						pUser->m_sPlayerShop.m_bPlayerShopMode = FALSE;

						ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pUser->GetID());

						if(pItemShop)	
							pItemShop->byType = 0;

						SHOP_TILE* pNewShopTile = g_pShopTileHash->GetData(pUser->GetID());

						if(pNewShopTile)
						{
							g_pShopTileHash->Delete(pNewShopTile, pUser->GetID());
							memset(pNewShopTile, 0, sizeof(SHOP_TILE));
							pNewShopTile = NULL;
						}

						CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();

						if(!pLayer)
							return;

						DSTC_PLAYERSHOP_CLOSE pClosePacket;
						pClosePacket.dwUserIndex = pUser->GetID();
						pLayer->BroadCastMsg( (char*)&pClosePacket, pClosePacket.GetPacketSize());

						// 판매자에게 보내기 //
						DSTC_PLAYERSHOP_SOLDOUT pSoldOutPacket;
						pSoldOutPacket.byType	= 1;									
						NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSoldOutPacket, pSoldOutPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}

					if(!pPriceUser->GetCurDungeon())
						return;
					if(!pPriceUser->GetCurDungeon()->GetDungeonDataEx())
						return;
					
					SetItemLog(&pPacket.Item,ITEM_LOG_PLAYERSHOP,WORD(pPriceUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID),pUser->m_szName,pPriceUser->m_szName);
					SetItemLogKarz( ITEM_LOG_PLAYERSHOP,WORD(pPriceUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID),pUser->m_szName,pPriceUser->m_szName,pPlayerShopLog.dwPrice,&pPacket.Item);
				}							
			}
			return;
		}										
	}	
}


void QueryTypeUserUpdate(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo( (DWORD)pResult->pData );

	if(pResult->nError < 0)
	{
		if(pUser)
		{
			Log(LOG_IMPORTANT, "Fail to update user data(%s)", pUser->m_szName);
			
			if(pUser->pBufForPortal)
			{
				LALFree(g_pPortalBufferPool, pUser->pBufForPortal);
				pUser->pBufForPortal = NULL;
			}
		}
		else
		{
			Log(LOG_IMPORTANT, "Fail to update user data(NULL)");
		}
	}
	else
	{
		if(pUser && pUser->pBufForPortal)
		{
			PORTAL_BUCKET* pPortal = (PORTAL_BUCKET*)(pUser->pBufForPortal);
			
			//유저에게 포탈 이동하라는 Packet을 날림 		
			DSTC_PORTAL_MOVE_OUTSIDE packet;
			packet.dwConnectIP		= pPortal->dwConnectIP;
			packet.wPort			= pPortal->wPort;
			packet.wToDungeonID		= pPortal->wToDungeonID;
			packet.bLayerNo			= pPortal->bLayerNo;
			packet.v2Pos			= pPortal->v2Pos;
			NetSendToUser( pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			// 이벤트 던젼일 경우, 월드서버에게 입장객이 한명 늘었다고 알려준다.							
			if (NULL != pUser->GetCurDungeon())
			{
				CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pUser->GetCurDungeonID());							
				pDungeon->ReportUserEnterEventDungeonByPortal(packet.wToDungeonID);
			}							
			
			LALFree(g_pPortalBufferPool, pUser->pBufForPortal);
			pUser->pBufForPortal = NULL;
		}
	}
}


void QueryTypeUserBankUpdate(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo( (DWORD)pResult->pData );

	if(pResult->nError < 0)
	{
		if(pUser)
			Log(LOG_IMPORTANT, "Fail to update user bank data(%s)", pUser->m_szName);
		else
			Log(LOG_IMPORTANT, "Fail to update user bank data(NULL)");
	}
	else
	{
		Log(LOG_IMPORTANT, "update user bank data");
	}
}


void QueryTypeTrade(DBRECEIVEDATA* pResult)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo( (DWORD)pResult->pData );
	if(NULL == pUser)
		return;

	ITEM_TRADE* pItemTrade = g_pItemTradeHash->GetData(pUser->m_nItemTradeSeriel);
	if(NULL == pItemTrade)
		return;

	int nUserIndex = 0;

	if(pItemTrade->TradeSerialStruct.dwHiUserIndex==pUser->GetID())		
		nUserIndex = pItemTrade->TradeSerialStruct.dwLowUserIndex;
	else
		nUserIndex = pItemTrade->TradeSerialStruct.dwHiUserIndex;

	CUser* pSendUser = g_pUserHash->GetData(nUserIndex);
	if(NULL == pSendUser)
		return;

	if(pResult->nError < 0)
	{
		Trade_Cancel(pUser);
		Trade_Cancel(pSendUser);
		g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
		pUser->m_nItemTradeSeriel		= 0;
		pSendUser->m_nItemTradeSeriel	= 0;
		pUser->m_byExTrade				= 0;
		pSendUser->m_byExTrade			= 0;
		
		// 실패 메세지 //
		DSTC_TRADE_RETURN pReturnPacket;
		pReturnPacket.byType = 7;					
		memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
		__lstrcpyn(pReturnPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		pReturnPacket.byType = 7;
		memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
		__lstrcpyn(pReturnPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
		NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);								
	}
	else
	{
		// Data Send //
		pUser->m_dwMoney		= pItemTrade->dwHiMoney;
		pSendUser->m_dwMoney	= pItemTrade->dwLowMoney;
		memcpy(pUser->m_pInv_Large, pItemTrade->pTradeLargeItem, sizeof(CItem)*MAX_INV_LARGE_POOL);
		memcpy(pUser->m_pInv_Small, pItemTrade->pTradeSmallItem, sizeof(CItem)*MAX_INV_SMALL_POOL);
		memcpy(pSendUser->m_pInv_Large, pItemTrade->pTradeLargeItem2, sizeof(CItem)*MAX_INV_LARGE_POOL);
		memcpy(pSendUser->m_pInv_Small, pItemTrade->pTradeSmallItem2, sizeof(CItem)*MAX_INV_SMALL_POOL);						
		
		// 패킷 보내기 //
		DSTC_TRADE_DATA pTradeDataPacket;
		pTradeDataPacket.dwMoeny = pUser->m_dwMoney;

		// modified by deepdark.
		for(int i = 0; i < MAX_TRADE_POOL; i++)
		{
			if(pSendUser->m_pTradeItem[i].GetID() != 0)
			{
				pTradeDataPacket.Nodes[pTradeDataPacket.btItemCount].index = BYTE(i);
				memcpy(&pTradeDataPacket.Nodes[pTradeDataPacket.btItemCount++].cItem,
					&pSendUser->m_pTradeItem[i], sizeof(CItem));
			}
		}

		//	memcpy(pTradeDataPacket.cItem, pSendUser->m_pTradeItem, sizeof(CItem)*MAX_TRADE);
		NetSendToUser(pUser->m_dwConnectionIndex
			, (char*)&pTradeDataPacket
			, pTradeDataPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);

		DSTC_TRADE_DATA pTradeDataPacket2;
		pTradeDataPacket2.dwMoeny = pSendUser->m_dwMoney;

		for( i = 0; i < MAX_TRADE_POOL; i++)
		{
			if(pUser->m_pTradeItem[i].GetID() != 0)
			{
				pTradeDataPacket2.Nodes[pTradeDataPacket2.btItemCount].index = BYTE(i);
				memcpy(&pTradeDataPacket2.Nodes[pTradeDataPacket2.btItemCount++].cItem,
					&pUser->m_pTradeItem[i], sizeof(CItem));
			}
		}

		//	memcpy(pTradeDataPacket.cItem, pUser->m_pTradeItem, sizeof(CItem)*MAX_TRADE);
		NetSendToUser(pSendUser->m_dwConnectionIndex
			, (char*)&pTradeDataPacket2
			, pTradeDataPacket2.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);

		// Hash에서 삭제 //
		g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
		pUser->m_nItemTradeSeriel		= 0;
		pSendUser->m_nItemTradeSeriel	= 0;	
		pUser->m_byExTrade				= 0;
		pSendUser->m_byExTrade			= 0;
		memset(pUser->m_pTradeItem, 0, sizeof(pSendUser->m_pTradeItem));
		memset(pSendUser->m_pTradeItem, 0, sizeof(pSendUser->m_pTradeItem));						
	}
}


void UpdateUserDBThread( CUser* pUser )
{
	if( !pUser )	
	{
		Log( LOG_IMPORTANT, "Not Exist user ");
		return;
	}

	if( pUser->GetClass() <= 0 	|| pUser->GetClass() > 5 || pUser->GetHeadModelNo() < 1000 )
	{
		Log( LOG_IMPORTANT, "Invalid Character (%s) !!!", pUser->GetName() );
		return;
	}
	
	char	szQuery[1024]	= {0,};
	int		nColumn			= 34;	
	int		col				= 0;
	DWORD	dwParamOffset	= 0;	

	DBBINDING* pBinding		= g_pDb->THCreateParamInfo((WORD)nColumn);
	if(!pBinding)
	{
		Log(LOG_IMPORTANT, "ParameterPool is Full!!  in UpdateUserDBThread Function()!");
		return;
	}
	
	UPDATE_USER_DATA UpdateData;
	
	UpdateData.m_dwChar_Index			= pUser->GetID();
	UpdateData.m_dwCurDungeonID			= (pUser->GetCurDungeon()) ? pUser->GetCurDungeonID() : 0;
	UpdateData.m_wClass					= pUser->GetClass();				
	UpdateData.m_wRace					= pUser->GetRace();
	UpdateData.m_wHP					= (WORD)pUser->GetHP();					
	UpdateData.m_wSP					= (WORD)pUser->GetSP();
	UpdateData.m_wPoint					= pUser->GetNotUseStatPoint();				
	UpdateData.m_wPointSkill			= pUser->GetLeftoverStatSkillPoint();	
	UpdateData.m_dwExp					= pUser->GetExp();
	UpdateData.m_dwLevel				= pUser->GetLevel();
	UpdateData.m_dwEgo					= pUser->GetEgo();
	UpdateData.m_dwStr					= pUser->GetStr();
	UpdateData.m_dwInt					= pUser->GetInt();
	UpdateData.m_dwDex					= pUser->GetDex();
	UpdateData.m_dwVit					= pUser->GetVit();
	UpdateData.m_bySelectedSkill[0]		= pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON);
	UpdateData.m_bySelectedSkill[1]		= pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON);

	memcpy(UpdateData.m_pEquip, pUser->GetItem(ITEM_NATIVE_EQUIP, 0), pUser->GetItemTotalSize(ITEM_NATIVE_EQUIP));
	memcpy(UpdateData.m_pInv_Large, pUser->GetItem(ITEM_NATIVE_INV_LARGE, 0), pUser->GetItemTotalSize(ITEM_NATIVE_INV_LARGE));
	memcpy(UpdateData.m_pInv_Small, pUser->GetItem(ITEM_NATIVE_INV_SMALL, 0), pUser->GetItemTotalSize(ITEM_NATIVE_INV_SMALL));
	memcpy(UpdateData.m_pBelt, pUser->GetItem(ITEM_NATIVE_BELT, 0), pUser->GetItemTotalSize(ITEM_NATIVE_BELT));
	memcpy(UpdateData.m_pwSkillLevel, pUser->GetSkillLevelPtr(), sizeof(UpdateData.m_pwSkillLevel));
	memcpy(UpdateData.m_pInv_Guardian, pUser->GetItem(ITEM_NATIVE_INV_GUARDIAN, 0), pUser->GetItemTotalSize(ITEM_NATIVE_INV_GUARDIAN));
	
	UpdateData.m_dwMoney				= pUser->GetMoney();
	UpdateData.m_wCriminalPropensity	= pUser->GetCriminalPropensity();	
	UpdateData.m_wClassRank				= pUser->GetClassRank();
	UpdateData.m_dwMatchRecords			= pUser->GetMatchDescInfo()->m_dwMatchRecords;
	UpdateData.m_dwMatchWin				= pUser->GetMatchDescInfo()->m_dwMatchWin;
	UpdateData.m_dwMatchLose			= pUser->GetMatchDescInfo()->m_dwMatchLose;
	UpdateData.m_bCurrentHand			= pUser->GetCurrentHand();	
	UpdateData.m_bPKCount				= pUser->GetPKDescInfo()->m_bPKCount;
	UpdateData.m_bPKRepeatCount			= pUser->GetPKDescInfo()->m_bPKRepeatCount;
	UpdateData.m_dwPKTime				= pUser->GetPKDescInfo()->m_dwPKTime;
	UpdateData.m_dwPKRepeatTime			= pUser->GetPKDescInfo()->m_dwPKRepeatTime;

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwChar_Index);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwChar_Index);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwCurDungeonID);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwCurDungeonID);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wClass);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wClass);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wRace);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wRace);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wHP);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wHP);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wSP);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wSP);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wPoint);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wPoint);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wPointSkill);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wPointSkill);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwExp );
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwExp);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwLevel);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwLevel);
		
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwEgo);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwEgo);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwStr);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwStr);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwInt);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwInt);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwDex);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwDex);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwVit);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwVit);
		
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]);
		
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pEquip);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pEquip);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pInv_Large);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pInv_Large);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pInv_Small);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pInv_Small);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pBelt);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pBelt);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pwSkillLevel);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pwSkillLevel);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pInv_Guardian);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pInv_Guardian);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwMoney);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwMoney);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wCriminalPropensity);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wCriminalPropensity);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wClassRank);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wClassRank);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bPKCount);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_bPKCount);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bPKRepeatCount);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_bPKRepeatCount);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwPKTime);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwPKTime);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwPKRepeatTime);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwPKRepeatTime);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwMatchRecords);
	pBinding[col++].wType	= DBTYPE_I4;
	dwParamOffset			+= sizeof(UpdateData.m_dwMatchRecords);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwMatchWin);
	pBinding[col++].wType	= DBTYPE_I4;
	dwParamOffset			+= sizeof(UpdateData.m_dwMatchWin);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwMatchLose);
	pBinding[col++].wType	= DBTYPE_I4;
	dwParamOffset			+= sizeof(UpdateData.m_dwMatchLose);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bCurrentHand);
	pBinding[col++].wType	= DBTYPE_BYTES;
	dwParamOffset			+= sizeof(UpdateData.m_bCurrentHand);

	wsprintf(szQuery, "CZP_USER_UPDATE ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?");
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, &UpdateData, sizeof(UPDATE_USER_DATA), (BYTE)nColumn
			, TRUE, QUERY_TYPE_USER_UPDATE, (void*)pUser->m_dwConnectionIndex, (BYTE)GAME_DB);

	if(g_pThis->GetServerType() == SERVER_TYPE_VILLAGE)// 김영대 팀장님 With 다 저장 : hwoarang 050220 
	{
		nColumn			= 3;
		col				= 0;
		dwParamOffset	= 0;
		
		DBBINDING* pBindingBank	= g_pDb->THCreateParamInfo((WORD)nColumn);
		if(!pBindingBank)
		{
			Log(LOG_IMPORTANT, "ParameterPool is Full!!  in UpdateUserDBThread(Bank) Function()!");
			return;
		}
	
		pBindingBank[col].obValue	=	dwParamOffset;
		pBindingBank[col].cbMaxLen	=	pUser->GetItemTotalSize(ITEM_NATIVE_BANK_LARGE);
		pBindingBank[col++].wType	=	DBTYPE_BYTES;					
		dwParamOffset				+=	pUser->GetItemTotalSize(ITEM_NATIVE_BANK_LARGE);

		pBindingBank[col].obValue	=	dwParamOffset;
		pBindingBank[col].cbMaxLen	=	pUser->GetItemTotalSize(ITEM_NATIVE_BANK_SMALL);
		pBindingBank[col++].wType	=	DBTYPE_BYTES;					
		dwParamOffset				+=	pUser->GetItemTotalSize(ITEM_NATIVE_BANK_SMALL);

		pBindingBank[col].obValue	=	dwParamOffset;
		pBindingBank[col].cbMaxLen	=	sizeof(pUser->GetBankMoney());
		pBindingBank[col++].wType	=	DBTYPE_I4;					
		dwParamOffset				+=	sizeof(DWORD);

		wsprintf(szQuery, "CZP_BANK_UPDATE '%s',?,?,?", pUser->GetLoginID());
		g_pDb->THExecuteSQLByParam(szQuery, pBindingBank, (void*)pUser->GetItem(ITEM_NATIVE_BANK_LARGE, 0)
			, dwParamOffset, (BYTE)nColumn, TRUE, QUERY_TYPE_USER_BANK_UPDATE
			, (void*)pUser->m_dwConnectionIndex, (BYTE)GAME_DB);

		Log(LOG_IMPORTANT, "THExecuteSQLByParam update user bank data");
	}
}

void UpdateUserDB( CUser* pUser )
{
	if( !pUser ) return;
		
	char	szQuery[1024]	={0,};
	int		nColumn			= 34;	
	int		col				= 0;
	DWORD	dwParamOffset	= 0;
	
	DBBINDING* pBinding = g_pDb->CreateParamInfo((WORD)nColumn, (BYTE)GAME_DB);
	if(!pBinding)
	{
		Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
		return;
	}

	UPDATE_USER_DATA UpdateData;
	
	UpdateData.m_dwChar_Index		= pUser->GetID();
	UpdateData.m_dwCurDungeonID		= (pUser->GetCurDungeon()) ? pUser->GetCurDungeonID() : 0;
	UpdateData.m_wClass				= pUser->GetClass();				
	UpdateData.m_wRace				= pUser->GetRace();
	UpdateData.m_wHP				= (WORD)pUser->GetHP();					
	UpdateData.m_wSP				= (WORD)pUser->GetSP();
	UpdateData.m_wPoint				= pUser->GetNotUseStatPoint();
	UpdateData.m_wPointSkill		= pUser->GetLeftoverStatSkillPoint();	
	UpdateData.m_dwExp				= pUser->GetExp();
	UpdateData.m_dwLevel			= pUser->GetLevel();
	UpdateData.m_dwEgo				= pUser->GetEgo();
	UpdateData.m_dwStr				= pUser->GetStr();
	UpdateData.m_dwInt				= pUser->GetInt();
	UpdateData.m_dwDex				= pUser->GetDex();
	UpdateData.m_dwVit				= pUser->GetVit();
	UpdateData.m_bySelectedSkill[0] = pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON);
	UpdateData.m_bySelectedSkill[1] = pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON);

	memcpy(UpdateData.m_pEquip, pUser->GetItem(ITEM_NATIVE_EQUIP, 0), sizeof(UpdateData.m_pEquip));
	memcpy(UpdateData.m_pInv_Large, pUser->GetItem(ITEM_NATIVE_INV_LARGE, 0), sizeof(UpdateData.m_pInv_Large));
	memcpy(UpdateData.m_pInv_Small, pUser->GetItem(ITEM_NATIVE_INV_SMALL, 0), sizeof(UpdateData.m_pInv_Small));
	memcpy(UpdateData.m_pBelt, pUser->GetItem(ITEM_NATIVE_BELT, 0), sizeof(UpdateData.m_pBelt));
	memcpy(UpdateData.m_pwSkillLevel, pUser->GetSkillLevelPtr(), sizeof(UpdateData.m_pwSkillLevel));
	memcpy(UpdateData.m_pInv_Guardian, pUser->GetItem(ITEM_NATIVE_INV_GUARDIAN, 0), sizeof(UpdateData.m_pInv_Guardian));
	
	UpdateData.m_dwMoney				= pUser->GetMoney();
	UpdateData.m_wCriminalPropensity	= pUser->GetCriminalPropensity();	
	UpdateData.m_wClassRank				= pUser->GetClassRank();
	UpdateData.m_dwMatchRecords			= pUser->GetMatchDescInfo()->m_dwMatchRecords;
	UpdateData.m_dwMatchWin				= pUser->GetMatchDescInfo()->m_dwMatchWin;
	UpdateData.m_dwMatchLose			= pUser->GetMatchDescInfo()->m_dwMatchLose;
	UpdateData.m_bCurrentHand			= pUser->GetCurrentHand();	
	UpdateData.m_bPKCount				= pUser->GetPKDescInfo()->m_bPKCount;
	UpdateData.m_bPKRepeatCount			= pUser->GetPKDescInfo()->m_bPKRepeatCount;
	UpdateData.m_dwPKTime				= pUser->GetPKDescInfo()->m_dwPKTime;
	UpdateData.m_dwPKRepeatTime			= pUser->GetPKDescInfo()->m_dwPKRepeatTime;

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwChar_Index);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwChar_Index);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwCurDungeonID);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwCurDungeonID);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wClass);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wClass);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wRace);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wRace);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wHP);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wHP);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wSP);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wSP);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wPoint);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wPoint);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wPointSkill);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wPointSkill);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwExp );
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwExp);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwLevel);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwLevel);
		
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwEgo);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwEgo);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwStr);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwStr);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwInt);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwInt);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwDex);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwDex);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwVit);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwVit);
		
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]);
		
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pEquip);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pEquip);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pInv_Large);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pInv_Large);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pInv_Small);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pInv_Small);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pBelt);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pBelt);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pwSkillLevel);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pwSkillLevel);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_pInv_Guardian);
	pBinding[col++].wType	= DBTYPE_BYTES;					
	dwParamOffset			+= sizeof(UpdateData.m_pInv_Guardian);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwMoney);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwMoney);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wCriminalPropensity);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wCriminalPropensity);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_wClassRank);
	pBinding[col++].wType	= DBTYPE_I2;					
	dwParamOffset			+= sizeof(UpdateData.m_wClassRank);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bPKCount);
	pBinding[col++].wType	= DBTYPE_UI1;					
	dwParamOffset			+= sizeof(UpdateData.m_bPKCount);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bPKRepeatCount);
	pBinding[col++].wType	= DBTYPE_UI1;					
	dwParamOffset			+= sizeof(UpdateData.m_bPKRepeatCount);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwPKTime);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwPKTime);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwPKRepeatTime);
	pBinding[col++].wType	= DBTYPE_I4;					
	dwParamOffset			+= sizeof(UpdateData.m_dwPKRepeatTime);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwMatchRecords);
	pBinding[col++].wType	= DBTYPE_I4;
	dwParamOffset			+= sizeof(UpdateData.m_dwMatchRecords);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwMatchWin);
	pBinding[col++].wType	= DBTYPE_I4;
	dwParamOffset			+= sizeof(UpdateData.m_dwMatchWin);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_dwMatchLose);
	pBinding[col++].wType	= DBTYPE_I4;
	dwParamOffset			+= sizeof(UpdateData.m_dwMatchLose);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(UpdateData.m_bCurrentHand);
	pBinding[col++].wType	= DBTYPE_BYTES;
	dwParamOffset			+= sizeof(UpdateData.m_bCurrentHand);

	wsprintf(szQuery, "CZP_USER_UPDATE ?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?");
	g_pDb->ExecuteSQLByParam( szQuery, pBinding, &UpdateData, (BYTE)nColumn, (BYTE)GAME_DB );
	g_pDb->ReleaseParamInfo( pBinding, (BYTE)GAME_DB );		

	if(g_pThis->GetServerType() == SERVER_TYPE_VILLAGE)
	{
		nColumn			= 3;
		col				= 0;
		dwParamOffset	= 0;
		
		DBBINDING* pBindingBank	= g_pDb->CreateParamInfo((WORD)nColumn, (BYTE)GAME_DB);
		if(!pBindingBank)
		{
			Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
			return;
		}
		
		pBindingBank[col].obValue	=	dwParamOffset;
		pBindingBank[col].cbMaxLen	=	pUser->GetItemTotalSize(ITEM_NATIVE_BANK_LARGE);
		pBindingBank[col++].wType	=	DBTYPE_BYTES;					
		dwParamOffset				+=	pUser->GetItemTotalSize(ITEM_NATIVE_BANK_LARGE);

		pBindingBank[col].obValue	=	dwParamOffset;
		pBindingBank[col].cbMaxLen	=	pUser->GetItemTotalSize(ITEM_NATIVE_BANK_SMALL);
		pBindingBank[col++].wType	=	DBTYPE_BYTES;					
		dwParamOffset				+=	pUser->GetItemTotalSize(ITEM_NATIVE_BANK_SMALL);

		pBindingBank[col].obValue	=	dwParamOffset;
		pBindingBank[col].cbMaxLen	=	sizeof(DWORD);
		pBindingBank[col++].wType	=	DBTYPE_I4;					
		dwParamOffset				+=	sizeof(DWORD);

		wsprintf(szQuery, "CZP_BANK_UPDATE '%s', ?, ?, ?", pUser->GetLoginID());
		g_pDb->ExecuteSQLByParam(szQuery, pBindingBank, (void*)pUser->GetItem(ITEM_NATIVE_BANK_LARGE, 0)
			, (BYTE)nColumn, (BYTE)GAME_DB);
		g_pDb->ReleaseParamInfo( pBindingBank, (BYTE)GAME_DB );
	}
}

void UpdateAllUserDB()
{
	ListNode<CUser>* pUserNode = NULL;
		
	pUserNode = g_pUserHash->GetHead();
	
	while( pUserNode )
	{
		CUser* pUser = pUserNode->pData;
					
		UpdateUserDB( pUser );
		
		pUserNode = pUserNode->pNext;
	}
}

void UpdateGuardianDB( CMonster* pMonster )
{	
	if( pMonster->GetRace() != OBJECT_TYPE_GUARDIAN ) return;

	char		szQuery[1024]	= {0,};

	const MONSTER_SKILL * pMS = pMonster->GetMonsterSkillPtr();

	wsprintf(szQuery, "CZP_GUARDIAN_UPDATE %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d",
			pMonster->GetLevel(),
			pMonster->GetExp(),
			pMonster->GetHP(),
			pMonster->GetSP(),
			pMonster->GetEgo(),
			
			pMonster->GetStr() ,
			pMonster->GetInt() ,
			pMonster->GetDex() ,
			pMonster->GetVit() ,
			pMonster->GetNotUseStatPoint(),

			pMS[0].bySkillKind,
			pMS[0].bSkillLevel,

			pMS[1].bySkillKind,
			pMS[1].bSkillLevel,
			
			pMS[2].bySkillKind,
			pMS[2].bSkillLevel,
			
			pMS[3].bySkillKind,
			pMS[3].bSkillLevel,

			pMS[4].bySkillKind,
			pMS[4].bSkillLevel,

			pMonster->GetGuardianDBID() );

//	g_pDb->THExecuteSQLByParam(szQuery, pBinding, &UpdateGuardian, sizeof(UpdateGuardian), (BYTE)nColumn, FALSE);
	g_pDb->THExecuteSQL( szQuery, FALSE, 0, NULL, GAME_DB );
}