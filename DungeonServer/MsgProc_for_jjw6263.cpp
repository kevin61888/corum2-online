#include "MsgProc_for_jjw6263.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "MonsterTable.h"
#include "recvMsg.h"
#include "OwnServer.h"
#include "ItemMoveManager.h"
#include "ItemUsedManager.h"
#include "GameSystemNative.h"


void InitializePacketProcJJW6263()
{
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_EVENT_USER_PREV_DUNGEON_ID]		= CmdEventUserPrevDungeonID;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_EVENT_CLEAR_EFFECT_FINISH]		= CmdEventClearEffectFinish;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ITEMUPGRADE ]						= CmdItemUpgrade;		
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_UPGRADE_FAIL]						= CmdItemUpgradeFail;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ITEMMAKING ]						= CmdItemMaking;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_MAKING_CANCEL]					= CmdItemMakingCancel;
		
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ITEM_MOVE ]						= CmdItemMove; 
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ITEM_USED_NATIVE ]				= CmdItemUsedNative;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_SWITCH_WEAPON ]					= CmdSwitchWeapon;

#ifdef DEV_MODE
	if( IsUltra() )
	{
		PacketProc[UPDATE_GAME_PLAY][Protocol_CTS::CMD_ITEM_PICKUP]						= NULL;	
		PacketProc[UPDATE_GAME_PLAY][Protocol_CTS::CMD_ITEM_USED]						= NULL;
	}
#endif
}


BOOL IsUltra()
{
	WORD	wVersionRequested;
	WSADATA wsaData;
	int		err;

	wVersionRequested = MAKEWORD( 2, 2 );
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 )
	{
		return FALSE;
	}

	if ( LOBYTE( wsaData.wVersion ) != 2 ||	HIBYTE( wsaData.wVersion ) != 2 )
	{
		WSACleanup();
		return FALSE; 
	}

	char name[255] = {0,};
	char szip[16] = {0,};
	struct hostent* hostinfo;

	if( gethostname ( name, sizeof(name)) == 0 )
	{
		if((hostinfo = gethostbyname(name)) != NULL)
		{
			for(int i=0; hostinfo->h_addr_list[i] != NULL; i++)
			{
				strcpy(szip, inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[i]));             
			}
		}
	}
	
	if( !strcmp(szip,"222.106.120.35") || !strcmp(szip,"222.106.120.42"))
	{
		WSACleanup();
//		return TRUE;
		return FALSE;
	}

	WSACleanup();
	return FALSE;
}


void CmdItemMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	g_ItemMoveManager.Process(dwConnectionIndex,pMsg,dwLength);
}


void CmdItemUsedNative( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	g_ItemUsedManager.Process(dwConnectionIndex,pMsg,dwLength);
}


void CmdSwitchWeapon( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if (!pUser)	return;
	else if (!pUser->GetCurDungeon())	return;
	CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	DSTC_SWITCH_WEAPON packet;

	if(pUser->m_bCurrentHand==1)
	{
		// LEFT
		pUser->m_bCurrentHand	= 2;
		packet.bCurrnetHand		= 2;
	}
	else	
	{
		// RIGHT
		pUser->m_bCurrentHand	= 1;
		packet.bCurrnetHand		= 1;
	}					
	
	// 착용한 아이템을 계산하고 통보.
	pUser->SendAllStatus();
															
	// Mod 교체 메세지 //
	if(pUser->m_bCurrentHand==1)
	{
		ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID
			, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
	}
	else
	{
		ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID
			, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
	}

	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void CmdEventUserPrevDungeonID( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_EVENT_USER_PREV_DUNGEON_ID* pPacket = (CTDS_EVENT_USER_PREV_DUNGEON_ID*)pMsg;
	
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	if(!pUser)
		return;
	else if(!pUser->GetCurDungeon() )
		return;

	pUser->m_wRecentJoinDungeonID = pPacket->wPrevDungeonID;
}

void CmdEventClearEffectFinish( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_EVENT_CLEAR_EFFECT_FINISH* pPacket = (CTDS_EVENT_CLEAR_EFFECT_FINISH*)pMsg;
	pPacket;
	
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	if(!pUser)
		return;
	if(!pUser->GetCurDungeon())
		return;

	pUser->GoToWorld(pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD);	
}

void CmdItemMakingCancel( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_MAKING_CANCEL* pPacket = (CTDS_MAKING_CANCEL*)pMsg; 

	CUser* pUser = (CUser*)g_pUserHash->GetData(pPacket->dwUserIndex);

	if( IsUltra() )
	{
		Making_Cancel(pUser);
	}
	else
	{
		if(pUser)
		{
			for(int i = 0; i < 4; i++)
			{
				if(pUser->m_pMakingItem[i].GetID()!=0)
				{
					DSTC_MAKING_CANCEL pSendPacket;
					pSendPacket.bZipCode	= 255;
					pSendPacket.byType		= 255;
					
					for(int j = 0; j < 4; j++)
					{
						memset(&pSendPacket.pItem[j], 0, sizeof(CItem));
						
						if(pUser->m_pMakingItem[j].GetID() != 0)
							memcpy(&pSendPacket.pItem[j], &pUser->m_pMakingItem[j], sizeof(CItem));					
					}

					// Item Making //
					for(j = 1; j < 4; j++)
					{
						if(pUser->m_pMakingItem[j].GetID()!=0)
						{
							if(pUser->m_pMakingItem[j].GetQuantity()==0)						
								return;

							BYTE byCnt = Insert_SmallInvItem(pUser
								, &pUser->m_pMakingItem[j]
								, 0
								, pUser->m_pMakingItem[j].GetQuantity()
								, FALSE);
								
							if(byCnt==0)
								memset(&pUser->m_pMakingItem[j], 0, sizeof(CItem));
							else
								pUser->m_pMakingItem[j].SetQuantity(byCnt);
						}
					}

					for(j = 1; j < 4; j++)
					{
						if(pUser->m_pMakingItem[j].GetID()!=0)
						{
							if(pUser->m_pMakingItem[j].GetQuantity()==0)						
								return;

							BYTE byCnt = Insert_SmallBankItem(pUser
								, &pUser->m_pMakingItem[j]
								, 0
								, pUser->m_pMakingItem[j].GetQuantity()
								, FALSE);
								
							if(byCnt==0)
								memset(&pUser->m_pMakingItem[j], 0, sizeof(CItem));
							else
								pUser->m_pMakingItem[j].SetQuantity(byCnt);
						}
					}
					
					if(pUser->m_pMakingItem[0].GetID()!=0)
					{
						BOOL bChk = FALSE;

						for(j = 0; j < MAX_INV_LARGE_POOL; j++)
						{
							if(pUser->m_pInv_Large[j].GetID()==0 && bChk==FALSE)
							{
								memcpy(&pUser->m_pInv_Large[j], &pUser->m_pMakingItem[0], sizeof(CItem));
								memset(&pUser->m_pMakingItem[0], 0, sizeof(CItem));
								pSendPacket.bZipCode = j;
								pSendPacket.byType	 = 0;
								bChk = TRUE;
								break;
							}									
						}

						if(pUser->m_pMakingItem[0].GetID()!=0)
						{
							for(j = 0; j < MAX_BANK_LARGE_POOL; j++)
							{
								if(pUser->m_pBank_Large[j].GetID()==0 && bChk==FALSE)
								{
									memcpy(&pUser->m_pBank_Large[j], &pUser->m_pMakingItem[0], sizeof(CItem));
									memset(&pUser->m_pMakingItem[0], 0, sizeof(CItem));
									pSendPacket.bZipCode = j;
									pSendPacket.byType	 = 1;
									bChk = TRUE;
									break;
								}
							}
						}
					}

					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
			}		
		}
	}
}


void CmdItemUpgradeFail( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_UPGRADE_FAIL* pPacket = (CTDS_UPGRADE_FAIL*)pMsg;

	CUser* pUser = (CUser*)g_pUserHash->GetData(pPacket->dwUserIndex);

	if( IsUltra() )
	{
		UpGrade_Cancel(pUser);
	}
	else
	{
		if(pUser)
		{
			for(int i = 0; i < 4; i++)
			{
				if(pUser->m_pUpgradeItem[i].GetID()!=0)
				{
					DSTC_UPGRADE_FAIL pSendPacket;
					pSendPacket.bZipCode	= 255;
					pSendPacket.byType		= 255;
					
					for(int j = 0; j < 4; j++)
					{
						memset(&pSendPacket.pItem[j], 0, sizeof(CItem));
						
						if(pUser->m_pUpgradeItem[j].GetID()!=0)
							memcpy(&pSendPacket.pItem[j], &pUser->m_pUpgradeItem[j], sizeof(CItem));					
					}

					// Item Upgrade //
					for(j = 1; j < 4; j++)
					{
						if(pUser->m_pUpgradeItem[j].GetID()!=0)
						{
							if(pUser->m_pUpgradeItem[j].GetQuantity()==0)						
								return;

							BYTE byCnt = Insert_SmallInvItem(pUser
								, &pUser->m_pUpgradeItem[j]
								, 0
								, pUser->m_pUpgradeItem[j].GetQuantity()
								, FALSE);
								
							if(byCnt==0)
								memset(&pUser->m_pUpgradeItem[j], 0, sizeof(CItem));
							else
								pUser->m_pUpgradeItem[j].SetQuantity(byCnt);
						}
					}

					for(j = 1; j < 4; j++)
					{
						if(pUser->m_pUpgradeItem[j].GetID()!=0)
						{
							if(pUser->m_pUpgradeItem[j].GetQuantity()==0)						
								return;

							BYTE byCnt = Insert_SmallBankItem(pUser
								, &pUser->m_pUpgradeItem[j]
								, 0
								, pUser->m_pUpgradeItem[j].GetQuantity()
								, FALSE);
								
							if(byCnt==0)
								memset(&pUser->m_pUpgradeItem[j], 0, sizeof(CItem));
							else
								pUser->m_pUpgradeItem[j].SetQuantity(byCnt);
						}
					}
					
					if(pUser->m_pUpgradeItem[0].GetID()!=0)
					{
						BOOL bChk = FALSE;

						for(j = 0; j < MAX_INV_LARGE_POOL; j++)
						{
							if(pUser->m_pInv_Large[j].GetID()==0 && bChk==FALSE)
							{
								memcpy(&pUser->m_pInv_Large[j], &pUser->m_pUpgradeItem[0], sizeof(CItem));
								memset(&pUser->m_pUpgradeItem[0], 0, sizeof(CItem));
								pSendPacket.bZipCode = j;
								pSendPacket.byType	 = 0;
								bChk = TRUE;
								break;
							}									
						}

						if(pUser->m_pUpgradeItem[0].GetID()!=0)
						{
							for(j = 0; j < MAX_BANK_LARGE_POOL; j++)
							{
								if(pUser->m_pBank_Large[j].GetID()==0 && bChk==FALSE)
								{
									memcpy(&pUser->m_pBank_Large[j], &pUser->m_pUpgradeItem[0], sizeof(CItem));
									memset(&pUser->m_pUpgradeItem[0], 0, sizeof(CItem));
									pSendPacket.bZipCode = j;
									pSendPacket.byType	 = 1;
									bChk = TRUE;
									break;
								}
							}
						}
					}

					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
			}		
		}
	}
}

void CmdItemMaking( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if (!pUser)	return;
	else if (!pUser->GetCurDungeon())	return;
	CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	CTDS_ITEMMAKING* pPacket = (CTDS_ITEMMAKING*)pMsg;
	pPacket;

	// 종류별로 쑤셔넣자.
	CItem* pItem[4] = {NULL, };

	if( IsUltra() )
	{
		for(BYTE i = 1; i < MAX_MAKING_POOL; i++)
		{
			ITEM_NATIVE_INFO		ItemNativeInfo;
			const CItem*			pItemNative			= NULL;
			const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

			ItemNativeInfo.eSlotID		= ITEM_NATIVE_MAKING;
			ItemNativeInfo.dwOwnerID	= pUser->GetID();
			ItemNativeInfo.bySlotIndex	= i;

			pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
			pItemNative = GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);	

			if(NULL == pItemNative)
				continue;

			if(pItemNative->GetID() == 0)
				continue;

			DWORD wKind = GetItemKind(pItemNative->GetID());
			
			if(wKind == ITEM_KIND_MATERIALS)
			{
				if(NULL == pItem[ITEM_MAKING_MATERIALS1])
				{
					pItem[ITEM_MAKING_MATERIALS1] = (CItem*)pItemNative;
				}
				else
				{
					pItem[ITEM_MAKING_MATERIALS2] = (CItem*)pItemNative;
				}
			}
			else if(wKind == ITEM_KIND_ZODIAC)
			{
				pItem[ITEM_MAKING_ZODIAC] = (CItem*)pItemNative;
			}
		}

		if(	pItem[ITEM_MAKING_MATERIALS1]	== NULL ||
			pItem[ITEM_MAKING_MATERIALS2]	== NULL ||
			pItem[ITEM_MAKING_ZODIAC]		== NULL		)
		{
			return;
		}

		ITEM_NATIVE_INFO		ItemNativeInfo;
		const CItem*			pItemNative			= NULL;
		const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

		ItemNativeInfo.eSlotID		= ITEM_NATIVE_MAKING;
		ItemNativeInfo.dwOwnerID	= pUser->GetID();
		ItemNativeInfo.bySlotIndex	= ITEM_MAKING_RESULT;

		pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
		pItemNative			= GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);

		if(pItemNative && pItemNative->GetID() != 0 )
		{
			// 제조창에 제조에 성공한 라쥐 아이템이 있을경우
			return;
		}
		
		ItemNativeInfo.eSlotID	= ITEM_NATIVE_INV_LARGE;
		pItemNative				= GetItemNative(pUser->GetCurDungeonLayer(),&ItemNativeInfo);

		BOOL bIsEmpty = FALSE;

		for(i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			if(pItemNative[i].GetID() == 0)
			{
				bIsEmpty = TRUE;
				ItemNativeInfo.bySlotIndex = i;
				pItemNative = &pItemNative[i];
				break;
			}
		}

		if(FALSE == bIsEmpty)
		{
			// 빈슬롯이 없따. ㅡ.-;;
			return;
		}
		
		// 링크 걸어야지..
		if( !SetSlotItemNativeLink(ITEM_NATIVE_MAKING, ITEM_MAKING_RESULT, &ItemNativeInfo) )
		{
			return;
		}

		pItem[ITEM_MAKING_RESULT] = (CItem*)pItemNative;

		// 제조 
		BOOL bResult = ItemMaking(pUser,pItem);	
		
		if(TRUE == bResult)
		{
			// 제조 성공 
			DSTC_ITEMMAKING_SUCCESS packet;	
			memcpy(&packet.cItem, pItem[ITEM_MAKING_RESULT], sizeof(CItem));
			NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			pUser->SetUserVirtualItemNativeLink(pItemNative, pItemNative->GetQuantity());
		}
		else
		{
			ITEM_NATIVE_INFO ItemNativeInfo;
			ItemNativeInfo.eSlotID		= ITEM_NATIVE_MAKING;
			ItemNativeInfo.dwOwnerID	= pUser->GetID();
			ItemNativeInfo.bySlotIndex	= ITEM_MAKING_RESULT;
			RemoveItemNativeLink(&ItemNativeInfo);

			// 실패
			DSTC_ITEMMAKING_FAIL packet;
			NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}

		ItemNativeInfo.eSlotID		= ITEM_NATIVE_MAKING;
		ItemNativeInfo.dwOwnerID	= pUser->GetID();

		for(i = 1; i < MAX_MAKING_POOL; i++)
		{
			ItemNativeInfo.bySlotIndex	= i;
			pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
			pItemNative			= GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);	

			if(NULL == pItemNative)
				continue;

			if(pItemNative->GetID() == 0)
				continue;

			pUser->RemoveUserVirtualItemNativeLink(pItemNative);
			RemoveItemNative(pItemNativeInfoLink);
			RemoveItemNativeLink(&ItemNativeInfo);
		}
	}
	else
	{
		pItem[ITEM_MAKING_RESULT] = &pUser->m_pMakingItem[0];
		
		for(int i = 1; i < 4; ++i)
		{
			int nValue = pUser->m_pMakingItem[i].GetID() / ITEM_DISTRIBUTE;

			if(nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX)
			{
				if(pItem[ITEM_MAKING_MATERIALS1] == NULL)
				{
					pItem[ITEM_MAKING_MATERIALS1] = &pUser->m_pMakingItem[i];
				}
				else
				{
					pItem[ITEM_MAKING_MATERIALS1 + 1] = &pUser->m_pMakingItem[i];
				}
			}
			else if(pUser->m_pMakingItem[i].GetID() == __ITEM_MAKING_INDEX__)
			{
				pItem[ITEM_MAKING_ZODIAC] = &pUser->m_pMakingItem[i];
			}
		}

		// 메터리얼 두개와 조디악 카드(큰게)가 절대 꼭 있어야지만 제조를 할수 있따!!
		if(	pItem[ITEM_MAKING_MATERIALS1]	&&	pItem[ITEM_MAKING_MATERIALS2]	&&
			pItem[ITEM_MAKING_ZODIAC]		&&	pItem[ITEM_MAKING_RESULT]->GetID() == 0)
		{
			BOOL bResult = ItemMaking(pUser,pItem);

			if(TRUE == bResult)
			{
				// 제조 성공 
				DSTC_ITEMMAKING_SUCCESS packet;	
				memcpy(&packet.cItem, &pUser->m_pMakingItem[0], sizeof(CItem));
				NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
			else
			{
				// 실패
				DSTC_ITEMMAKING_FAIL packet;
				NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}

			memset(&pUser->m_pMakingItem[1], 0, sizeof(CItem));
			memset(&pUser->m_pMakingItem[2], 0, sizeof(CItem));
			memset(&pUser->m_pMakingItem[3], 0, sizeof(CItem));
		}
	}	
}

void CmdItemUpgrade( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	
	if (!pUser)						return;
	if (!pUser->GetCurDungeon())	return;
	CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer )					return;

	CTDS_ITEMUPGRADE* pPacket = (CTDS_ITEMUPGRADE*)pMsg;
	
	// 종류별로 쑤셔넣자.
	CItem* pItem[4] = {NULL, };
/*
	if( IsUltra() )
	{
		ITEM_NATIVE_INFO ItemNativeInfo;
		memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
		const CItem*			pItemNative			= NULL;
		const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

		ItemNativeInfo.eSlotID		= ITEM_NATIVE_UPGRADE;
		ItemNativeInfo.dwOwnerID	= pUser->GetID();
		
		for(BYTE i = 0; i < MAX_UPGRADE_POOL; i++)
		{
			ItemNativeInfo.bySlotIndex	= i;

			pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
			pItemNative			= GetItemNative(pUser->GetCurDungeonLayer(),pItemNativeInfoLink);	

			if(NULL == pItemNative)
				continue;

			if(pItemNative->GetID() == 0)
				continue;

			DWORD wKind = GetItemKind(pItemNative->GetID());

			switch(wKind)
			{
				case ITEM_KIND_WEAPON	:	pItem[ITEM_UPGRADE_WEAPON]	= (CItem*)pItemNative;	break;
				case ITEM_KIND_AROMR	:	pItem[ITEM_UPGRADE_WEAPON]	= (CItem*)pItemNative;	break;
				case ITEM_KIND_UPGRADE	:	pItem[ITEM_UPGRADE_UPGRADE] = (CItem*)pItemNative;	break; 
				case ITEM_KIND_LIQUID	:   pItem[ITEM_UPGRADE_LIQUID]	= (CItem*)pItemNative;	break; 
				case ITEM_KIND_EDITION	:	pItem[ITEM_UPGRDAE_EDITION]	= (CItem*)pItemNative;	break;
				default					:	break;		
			}
		}

		// 1.(무기류와방어구)
		// 2.강화서 
		// 3.리퀴드
		// 이 세가지는 꼭 있어야 돼!!!
		if(	pItem[ITEM_UPGRADE_WEAPON] && pItem[ITEM_UPGRADE_UPGRADE] && pItem[ITEM_UPGRADE_LIQUID])
		{
			BYTE bZipCode	= 0;	
			
#if defined JAPAN_LOCALIZING
			BOOL bResult	= ItemUpgrade_Localizing(pUser,pItem,&bZipCode);
#elif defined CHINA_LOCALIZING
			BOOL bResult	= ItemUpgrade_Localizing(pUser,pItem,&bZipCode);
#elif defined TAIWAN_LOCALIZING
			BOOL bResult	= ItemUpgrade_Localizing(pUser,pItem,&bZipCode);
#elif defined ENGLISH_LOCALIZING
			BOOL bResult	= ItemUpgrade_Localizing(pUser,pItem,&bZipCode);
#else
			BOOL bResult	= ItemUpgrade(pUser,pItem,&bZipCode);
#endif
			if (bResult > 0)
			{
				// 성공
				DSTC_ITEMUPGRADE_SUCCESS packet;
				packet.bResult = bResult;
				memcpy(&packet.cItem, pItem[ITEM_UPGRADE_WEAPON], sizeof(CItem));
				NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
			else if (bResult == 0)
			{
				// 실패
				DSTC_ITEMUPGRADE_FAIL packet;
				packet.bDelete = 0;

				ITEM_NATIVE_INFO ItemNativeInfo;
				const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

				ItemNativeInfo.eSlotID		= ITEM_NATIVE_UPGRADE;
				ItemNativeInfo.dwOwnerID	= pUser->GetID();
				ItemNativeInfo.bySlotIndex	= ITEM_UPGRADE_WEAPON;

				pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);

				RemoveItemNative(pItemNativeInfoLink);
				RemoveItemNativeLink(&ItemNativeInfo);
				
				NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
			else if (bResult == -1)
			{
				// 실패 하긴 했지만 보험 적용
				DSTC_ITEMUPGRADE_FAIL_INSURANCE packet;
				packet.bDelete = bZipCode;
				memcpy(&packet.cItem, pItem[ITEM_UPGRADE_WEAPON], sizeof(CItem));
				NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}

			ITEM_NATIVE_INFO		ItemNativeInfo;
			const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
			const CItem*			pItemNative			= NULL;

			ItemNativeInfo.eSlotID		= ITEM_NATIVE_UPGRADE;
			ItemNativeInfo.dwOwnerID	= pUser->GetID();

			for(BYTE i = 1; i < MAX_UPGRADE_POOL; i++)
			{
				ItemNativeInfo.bySlotIndex	= i;
				pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo);
				pItemNative			= GetItemNative(pUser->GetCurDungeonLayer(), pItemNativeInfoLink);	

				if(NULL == pItemNative)
					continue;

				if(pItemNative->GetID() == 0)
					continue;
				
				pUser->RemoveUserVirtualItemNativeLink(pItemNative);
				RemoveItemNative(pItemNativeInfoLink);
				RemoveItemNativeLink(&ItemNativeInfo);
			}
		}
	}
	else
	{*/
		for(int i = 0; i < 4; ++i)
		{
			if(!pUser->m_pUpgradeItem[i].GetID())
			{
				continue;
			}
			if(	GetItemKind(pUser->m_pUpgradeItem[i].GetID()) == ITEM_KIND_WEAPON ||
				GetItemKind(pUser->m_pUpgradeItem[i].GetID()) == ITEM_KIND_AROMR)
			{
				pItem[ITEM_UPGRADE_WEAPON] = &pUser->m_pUpgradeItem[i];
			}
			else if(pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE == ITEM_UPGRADE_INDEX)
			{
				pItem[ITEM_UPGRADE_UPGRADE] = &pUser->m_pUpgradeItem[i];
				SetItemCount(pItem[ITEM_UPGRADE_UPGRADE]->GetID(), pUser->GetCurDungeonID(), -1);
				SetItemLog( pItem[ITEM_UPGRADE_UPGRADE], ITEM_LOG_USED, pUser->GetCurDungeonID(), pUser->m_szName, NULL );
			}
			else if(pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE == ITEM_LIQUID_INDEX)
			{
				pItem[ITEM_UPGRADE_LIQUID] = &pUser->m_pUpgradeItem[i];
				SetItemCount(pItem[ITEM_UPGRADE_LIQUID]->GetID(), pUser->GetCurDungeonID(), -1);
				SetItemLog( pItem[ITEM_UPGRADE_LIQUID], ITEM_LOG_USED, pUser->GetCurDungeonID(), pUser->m_szName, NULL );
			}
			else if (pUser->m_pUpgradeItem[i].GetID()/ITEM_DISTRIBUTE == ITEM_EDITION_INDEX)
			{
				pItem[ITEM_UPGRDAE_EDITION] = &pUser->m_pUpgradeItem[i];
				SetItemCount(pItem[ITEM_UPGRDAE_EDITION]->GetID(), pUser->GetCurDungeonID(), -1);
				SetItemLog( pItem[ITEM_UPGRDAE_EDITION], ITEM_LOG_USED, pUser->GetCurDungeonID(), pUser->m_szName, NULL );
			}
		}

		// 1.(무기류와방어구)
		// 2.강화서 
		// 3.리퀴드
		// 이 세가지는 꼭 있어야 돼!!!
		if(	pItem[ITEM_UPGRADE_WEAPON] && pItem[ITEM_UPGRADE_UPGRADE] && pItem[ITEM_UPGRADE_LIQUID])
		{
			BYTE bZipCode	= 0;	
			
#if defined JAPAN_LOCALIZING
			BOOL bResult	= ItemUpgrade_Localizing(pUser,pItem,&bZipCode);
#elif defined CHINA_LOCALIZING
			BOOL bResult	= ItemUpgrade_Localizing(pUser,pItem,&bZipCode);
#elif defined TAIWAN_LOCALIZING
			BOOL bResult	= ItemUpgrade_Localizing(pUser,pItem,&bZipCode);
#elif defined ENGLISH_LOCALIZING
			BOOL bResult	= ItemUpgrade_Localizing(pUser,pItem,&bZipCode);
#else
			BOOL bResult	= ItemUpgrade(pUser,pItem,&bZipCode);
#endif

			if (bResult > 0)
			{
				// 성공 
				DSTC_ITEMUPGRADE_SUCCESS packet;
				packet.bResult = bResult;
				memcpy(&packet.cItem, &pUser->m_pUpgradeItem[0], sizeof(CItem));
				NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				SetItemCount(pUser->m_pUpgradeItem[0].GetID(), pUser->GetCurDungeonID(), pUser->m_pUpgradeItem[0].GetQuantity());
				SetItemLog( &pUser->m_pUpgradeItem[0], ITEM_LOG_UPGRADE, pUser->GetCurDungeonID(), pUser->m_szName, NULL );
			}
			else if (bResult == 0)
			{
				// 실패 
				SetItemCount(pUser->m_pUpgradeItem[0].GetID(), pUser->GetCurDungeonID(), pUser->m_pUpgradeItem[0].GetQuantity());
				SetItemLog( &pUser->m_pUpgradeItem[0], ITEM_LOG_UPGRADE_FAIL, pUser->GetCurDungeonID(), pUser->m_szName, NULL );

				DSTC_ITEMUPGRADE_FAIL packet;
				packet.bDelete = 0;
				
				memset(&pUser->m_pUpgradeItem[0], 0, sizeof(CItem));
				NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
			else if (bResult == -1)
			{
				// 실패 하긴 했지만 보험 적용 
				DSTC_ITEMUPGRADE_FAIL_INSURANCE packet;
				packet.bDelete = bZipCode;
				memcpy(&packet.cItem, &pUser->m_pUpgradeItem[0], sizeof(CItem));
				NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				SetItemCount(pUser->m_pUpgradeItem[0].GetID(), pUser->GetCurDungeonID(), pUser->m_pUpgradeItem[0].GetQuantity());
				SetItemLog( &pUser->m_pUpgradeItem[0], ITEM_LOG_UPGRADE_FAIL_INSURANCE, pUser->GetCurDungeonID(), pUser->m_szName, NULL );
			}

			memset(&pUser->m_pUpgradeItem[1], 0, sizeof(CItem));
			memset(&pUser->m_pUpgradeItem[2], 0, sizeof(CItem));
			memset(&pUser->m_pUpgradeItem[3], 0, sizeof(CItem));
		}
//	}	
}