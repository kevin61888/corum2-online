#include "MsgProc_for_minbobo.h"
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
#include "GameSystemNative.h"


void InitializePacketProcMinbobo()
{
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_WORLD_MOVE]						= CmdWorldMove;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUILD_ITEM ]						= CmdGuildItem;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUILD_USERDATA]					= CmdGuildUserData;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUILD_INFO ]						= CmdGuildInfo;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUILD_JOININFO]					= CmdGuildJoinInfo;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_TRADE_REQUEST]					= CmdTradeRequest;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_TRADE_RESULT]						= CmdTradeResult;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_TRADE_CANCEL]						= CmdTradeCancel;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_TRADE_OK]							= CmdTradeOk;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_TRADE_TRADE]						= CmdTradeTrade;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PARTY_PORTAL ]					= CmdPartyPortal;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PLAYERSHOP_OPEN ]					= CmdPlayerShopOpen;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PLAYERSHOP_CLOSE ]				= CmdPlayerShopClose;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PARTY_PORTALUSED ]				= CmdPartyPortalUsed;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PARTY_PORTALFAIL ]				= CmdPartyPortalFail;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PLAYERSHOP_SHOWALL ]				= CmdPlayerShopShowAll;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PLAYER_REST]						= CmdPlayerRest;		
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUILD_PORTALUSED]					= CmdGuildPortalUsed;	
} 


void CmdGuildPortalFail( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUILD_PORTALFAIL* pFailPacket = (CTDS_GUILD_PORTALFAIL*)pMsg;

	CUser*	pUser		= g_pUserHash->GetData(pFailPacket->dwUserIndex);
	CUser*	pSendUser	= g_pUserHash->GetData(pFailPacket->dwPartalUserIndex);

	if(pSendUser && pUser)
	{
		DSTC_GUILD_PORTALFAIL pSendPacket;
		memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
		__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
		pSendPacket.byResult = pFailPacket->byResult;
		NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CmdPartyPortalFail( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PARTY_PORTALFAIL* pFailPacket = (CTDS_PARTY_PORTALFAIL*)pMsg;

	CUser*	pUser		= g_pUserHash->GetData(pFailPacket->dwUserIndex);
	CUser*	pSendUser	= g_pUserHash->GetData(pFailPacket->dwPartalUserIndex);

	if(pSendUser && pUser)
	{
		DSTC_PARTY_PORTALFAIL pSendPacket;
		memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
		__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
		pSendPacket.byResult = pFailPacket->byResult;
		NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CmdGuildPortalUsed( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUILD_PORTALUSED* pPacket = (CTDS_GUILD_PORTALUSED*)pMsg;

	CUser* pGuildUser = g_pUserHash->GetData(pPacket->dwPartalUserIndex);

	if(pGuildUser)
	{
		if(CheckShop(pGuildUser))
			return;

		if(pGuildUser->m_byRest==1)
			return;
			
		WSTDS_GUILD_PORTALUSED pSendPacket;
		memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
		__lstrcpyn(pSendPacket.szName, pGuildUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
		pSendPacket.dwGuildId			= pPacket->dwGuildId;
		pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
		pSendPacket.dwPartalUserIndex	= pPacket->dwPartalUserIndex;
		pSendPacket.dwResult			= pPacket->byResult;
		pSendPacket.byIndex				= pPacket->byIndex;
		pSendPacket.byType				= pPacket->byType;

		g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);		
	}	
}


void CmdPartyPortalUsed( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PARTY_PORTALUSED* pPacket = (CTDS_PARTY_PORTALUSED*)pMsg;

	CUser* pPartalUser = g_pUserHash->GetData(pPacket->dwPartalUserIndex);

	if(pPartalUser)
	{
		if(CheckShop(pPartalUser))
			return;

		if(pPartalUser->m_byRest==1)
			return;
			
		WSTDS_PARTY_PORTALUSED pSendPacket;
		memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
		__lstrcpyn(pSendPacket.szName, pPartalUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
		pSendPacket.dwPartyId			= pPacket->dwPartyId;
		pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
		pSendPacket.dwPartalUserIndex	= pPacket->dwPartalUserIndex;
		pSendPacket.dwResult			= pPacket->byResult;
		pSendPacket.byIndex				= pPacket->byIndex;
		pSendPacket.byType				= pPacket->byType;
		g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);		
	}	
}


void CmdPartyPortal( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PARTY_PORTAL* pPacket = (CTDS_PARTY_PORTAL*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(pUser)
	{
		if( pUser->IsCurDungeonSiege() && pUser->GetAttackMode() != ATTACK_MODE_DEFENCE )
		{
			// 공성중일때는 방어자만 이동가능하다.
			DSTC_PARTY_PORTALFAIL pFailPacket;
			memset(pFailPacket.szName, 0, sizeof(pFailPacket.szName));
			__lstrcpyn(pFailPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pFailPacket.byResult = 4;
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

/*
		// Item Chk //
		if(pUser->m_pInv_Small[pPacket->byZipCode].GetID()==0)
		{
			return;
		}*/


		int nValue = pUser->m_pInv_Small[pPacket->byZipCode].GetID() / ITEM_DISTRIBUTE;
		
		if(nValue == ITEM_MAGICARRAY_INDEX)
		{
			if(pUser->m_pInv_Small[pPacket->byZipCode].GetID()==__ITEM_PARTY_PORTAL1__)
			{
				// 월드 서버로 보낸다 //
				WSTDS_PARTY_PORTAL pSendPacket;
				memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
				__lstrcpyn(pSendPacket.szName, pPacket->szName, MAX_CHARACTER_REAL_LENGTH);
				pSendPacket.dwPartyId			= pPacket->dwPartyId;
				pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
				pSendPacket.dwPartalUserIndex	= pPacket->dwPartalUserIndex;
				pSendPacket.byIndex				= pPacket->byZipCode;
				g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}							
		}
	}
}


void CmdPartyPoralRecognition( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PARTY_PORTAL_RECOGNITION* pPacket = (CTDS_PARTY_PORTAL_RECOGNITION*)pMsg;

	CUser* pUser		= (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	CUser* pSendUser	= g_pUserHash->GetData(pPacket->dwPortalUserIndex);
	
	if(NULL == pUser)	return;

	if(pSendUser)
	{
		if(!pSendUser->GetCurDungeon() || !pUser->GetCurDungeon())
		{
			DSTC_PARTY_PORTALFAIL pSendPacket;
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 5;
			NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 5;
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		if(pSendUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID == 
			pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID)
		{
			if(pSendUser->GetCurLayerIndex() == pUser->GetCurLayerIndex())
			{
				DSTC_PARTY_PORTALFAIL pSendPacket;
				memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
				__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				pSendPacket.byResult = 2;
				NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				
				memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
				__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				pSendPacket.byResult = 2;
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
		}

		if(pUser->IsCurDungeonEventInLive())
		{
			// 실패 //
			DSTC_PARTY_PORTALFAIL pSendPacket;
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 5;
			NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 5;
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}	

		ITEM_NATIVE_INFO ItemNativeInfoRecall;

		ItemNativeInfoRecall.eSlotID		= ITEM_NATIVE_INV_SMALL;
		ItemNativeInfoRecall.dwOwnerID		= pUser->GetID();
		ItemNativeInfoRecall.bySlotIndex	= pPacket->bySlotIndex;

		const CItem* pItemNative = GetItemNative(NULL, &ItemNativeInfoRecall);

		if(NULL == pItemNative)
		{
			DSTC_PARTY_PORTALFAIL pSendPacket;
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 3;
			NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 3;
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		WORD wItemID = pItemNative->GetID();			

		if(	pPacket->byRecallType == 0 || /*파티 전원 리콜*/ 
			wItemID==__ITEM_PARTY_PORTAL1__ || wItemID==__ITEM_PARTY_PORTAL2__)
		{
			// 퍼스널 리콜
			if(pPacket->byRecallType == 1)
			{
				CItem cItem;
				memcpy(&cItem, pItemNative, sizeof(CItem));
				
				BYTE byCnt = cItem.GetQuantity();

				if(wItemID==__ITEM_PARTY_PORTAL1__ )
				{
					if(byCnt > 1)
						SetSlotItemNative(NULL, &cItem, &ItemNativeInfoRecall);
					else
						RemoveItemNative(&ItemNativeInfoRecall);
				}

				// 클라이언트에 보내준다.
				DSTC_ITEM_USED_NATIVE SendUsedPacket;
				SendUsedPacket.bSrc					= ITEM_NATIVE_INV_SMALL;
				SendUsedPacket.bSrcIndex			= ItemNativeInfoRecall.bySlotIndex;
				SendUsedPacket.bItemUsedNativeType	= ITEM_USED_NATIVE_TYPE_MAGICARRAY;
				SendUsedPacket.bItemUsedNativeValue = ITEM_USED_NATIVE_VALUE_PERSONAL_RECALL;
				memcpy(&SendUsedPacket.cItem, pItemNative, sizeof(CItem));
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&SendUsedPacket, SendUsedPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION );
			}
			
			DSTC_PARTY_PORTALMOVE pSendPacket;
			pSendPacket.wDungeonID		= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
			pSendPacket.byLayerIndex	= pUser->GetCurLayerIndex();
			pSendPacket.dwPartyId		= pUser->m_dwPartyId;
			pSendPacket.fPosX			= pUser->GetCurPosition()->x;
			pSendPacket.fPosZ			= pUser->GetCurPosition()->y;						
			NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION );				
		}
		else
		{
			DSTC_PARTY_PORTALFAIL pSendPacket;
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 3;
			NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);

			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 3;
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
	}
	else
	{
		if(pUser->IsCurDungeonEventInLive())
		{
			// 실패 //
			DSTC_PARTY_PORTALFAIL pSendPacket;
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			pSendPacket.byResult = 5;
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		ITEM_NATIVE_INFO ItemNativeInfoRecall;

		ItemNativeInfoRecall.eSlotID		= ITEM_NATIVE_INV_SMALL;
		ItemNativeInfoRecall.dwOwnerID		= pUser->GetID();
		ItemNativeInfoRecall.bySlotIndex	= pPacket->bySlotIndex;

		const CItem* pItemNative = GetItemNative(NULL, &ItemNativeInfoRecall);

		if(NULL == pItemNative)
		{
			DSTC_PARTY_PORTALFAIL pSendPacket;
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 4;
			NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendPacket.byResult = 4;
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		WORD wItemID = pItemNative->GetID();

		if(	wItemID==__ITEM_PARTY_PORTAL1__ || wItemID==__ITEM_PARTY_PORTAL2__ )
		{
			// 퍼스널 리콜
			if(pPacket->byRecallType == 1)
			{
				CItem cItem;
				memcpy(&cItem, pItemNative, sizeof(CItem));
				
				BYTE byCnt = cItem.GetQuantity();

				if(wItemID==__ITEM_PARTY_PORTAL1__ )
				{
					if(byCnt > 1)
						SetSlotItemNative(NULL, &cItem, &ItemNativeInfoRecall);
					else
						RemoveItemNative(&ItemNativeInfoRecall);
				}

				// 클라이언트에 보내준다.
				DSTC_ITEM_USED_NATIVE SendUsedPacket;
				SendUsedPacket.bSrc					= ITEM_NATIVE_INV_SMALL;
				SendUsedPacket.bSrcIndex			= ItemNativeInfoRecall.bySlotIndex;
				SendUsedPacket.bItemUsedNativeType	= ITEM_USED_NATIVE_TYPE_MAGICARRAY;
				SendUsedPacket.bItemUsedNativeValue = ITEM_USED_NATIVE_VALUE_PERSONAL_RECALL;
				memcpy(&SendUsedPacket.cItem, pItemNative, sizeof(CItem));
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&SendUsedPacket, SendUsedPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION );
			}

			WSTDS_PARTY_PORTALMOVE pSendPacket;
			pSendPacket.wDungeonID			= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
			pSendPacket.byLayerIndex		= pUser->GetCurLayerIndex();
			pSendPacket.dwPartyId			= pUser->m_dwPartyId;
			pSendPacket.fPosX				= pUser->GetCurPosition()->x;
			pSendPacket.fPosZ				= pUser->GetCurPosition()->y;
			pSendPacket.dwUserIndex			= pUser->GetID();
			pSendPacket.dwPortalUserIndex	= pPacket->dwPortalUserIndex;
			g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);				
		}
		else
		{
			WSTDS_PARTY_PORTALFAIL pSendServerPacket;						
			memset(pSendServerPacket.szName, 0, sizeof(pSendServerPacket.szName));
			__lstrcpyn(pSendServerPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
			pSendServerPacket.byResult		= 3;
			pSendServerPacket.dwUserIndex	= pPacket->dwPortalUserIndex;
			g_pNet->SendToServer(WSINDEX, (char*)&pSendServerPacket, pSendServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
	}																												
}


void CmdPlayerRest( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PLAYER_REST* pPacket = (CTDS_PLAYER_REST*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(!pUser)
		return;
	if(pUser->GetCurLayerIndex()==INVALID_DUNGEON_LAYER)
		return;
	if(!pUser->GetCurDungeon())
		return;

	CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();
	
	if(!pLayer)
		return;
	
	if(pUser->m_byRest==pPacket->byType)
		return;
	
	pUser->SetRest(pPacket->byType);
}


void CmdWorldMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_WORLD_MOVE* pPacket = (CTDS_WORLD_MOVE*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(!pUser)
		return;
	if(!pUser->GetCurDungeon())
		return;
	
	if(pUser && pUser->m_bPortalMoving != 1)
		pUser->GoToWorld(pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD);
}


void CmdPlayerShopOpen( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PLAYERSHOP_OPEN*	pPacket = (CTDS_PLAYERSHOP_OPEN*)pMsg;
	DSTC_SHOP_FAIL			pSendPacket;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		
	if(!pUser)
		return;

	if(pUser->GetCurLayerIndex()==255)
		return;

	if (!pUser->GetCurDungeon())
		return;

	CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();
	
	if( !pLayer )
		return;

	if(pUser->m_sMatchDescInfo.m_bMatching)
	{
		pSendPacket.byType	= 22;
		NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		return;
	}
	
	if(pUser->m_nItemTradeSeriel!=0)
	{		
		if(CheckTrade(pUser->m_nItemTradeSeriel))
		{
			pSendPacket.byType	= 14;
			NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			return;
		}
	}
	
	if(pUser->m_bPortalMoving)
	{
		pSendPacket.byType	= 17;
		NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		return;
	}
	
	// 마을일 경우만 허용 //
	if(pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_VILLAGE)
	{
		pSendPacket.byType	= 1;
		NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		return;
	}

	MAP_TILE* pTile = pUser->GetCurMapTile();
	
	if(!pTile)
	{	
		pSendPacket.byType	= 2;
		NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		return;
	}
	
	ListNode<SHOP_TILE>* pTilePos = g_pShopTileHash->GetHead();

	while(pTilePos)
	{
		SHOP_TILE* pShopTile = pTilePos->pData;

		if(!pShopTile)
		{
			pTilePos = pTilePos->pNext;
			continue;
		}

		if(pTile->wIndex_X==pShopTile->wX && pTile->wIndex_Z==pShopTile->wY)
		{
			pSendPacket.byType	= 3;
			NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			return;
		}

		pTilePos = pTilePos->pNext;
	}

	ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pUser->GetID());

	if(!pItemShop)
	{
		pSendPacket.byType	= 4;
		NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		return;
	}

	if( IsUltra() )
	{
		DWORD dwVirtualLinkItemNativeHashCount = pUser->GetItemNativeLinkCount();
		if(dwVirtualLinkItemNativeHashCount < 1)
		{
			pSendPacket.byType	= 6;
			NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			return;
		}
	}
	else
	{
		BYTE byLarge = 0, bySmall = 0;
		BOOL bIsZeroCustomSellPrice = FALSE;

		for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
//			int nValue = pUser->m_sPlayerShop.cItem[i].GetID()/ITEM_DISTRIBUTE;

			if(pUser->m_sPlayerShop.cItem[i].GetID()==0)
				continue;
				
			if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_LARGE)
				byLarge++;		
			else if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_SMALL)
				bySmall++;
			
			if(pUser->m_sPlayerShop.dwCustomSellPrice[i] == NULL)
			{
				// 판매가격이 설정되어 있지 않다. 노점 못열어!!
				bIsZeroCustomSellPrice = TRUE;
				break;
			}
		}

		if(bIsZeroCustomSellPrice)
		{
			pSendPacket.byType = 20;
			NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			return;
		}

		if(byLarge==0 && bySmall==0)
		{
			pSendPacket.byType	= 6;
			NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			return;
		}

		BYTE byInvLarge = 0;

		for(i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
//			int nValue = pUser->m_pInv_Large[i].GetID()/ITEM_DISTRIBUTE;

			if(pUser->m_pInv_Large[i].GetID()==0)		
				byInvLarge++;
		}

		if(byLarge > byInvLarge)
		{
			pSendPacket.byType	= 5;
			NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			return;
		}

//		BYTE byInvSmall = 0;

		CItem cItem[MAX_INV_SMALL_POOL];

		BOOL bChk = TRUE;
		
		for(i = 0; i < MAX_INV_SMALL_POOL; i++)
			memcpy(&cItem[i], &pUser->m_pInv_Small[i], sizeof(CItem));

		for(i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
//			int nValue = pUser->m_sPlayerShop.cItem[i].GetID()/ITEM_DISTRIBUTE;

			if(pUser->m_sPlayerShop.cItem[i].GetID()==0)
				continue;

						
			if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID()) == ITEM_TYPE_SMALL)
			{
				if(Insert_SmallInvItem2(cItem
					, &pUser->m_sPlayerShop.cItem[i]
					, 0
					, pUser->m_sPlayerShop.cItem[i].GetQuantity()
					, TRUE) != 0 )
				{
					bChk = FALSE;
				}
			}
			// 아이템 로그 추가 김영대 2005.02.23
			SetItemLog( &pUser->m_sPlayerShop.cItem[i], ITEM_LOG_PLAYERSHOT_OPEN, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
	
		}
			
		if(!bChk)
		{
			pSendPacket.byType	= 5;
			NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			return;
		}
	}

	pItemShop->byType	= 1;

	SHOP_TILE* pNewShopTile = (SHOP_TILE*)LALAlloc(g_pShopTilePool);

	if(pNewShopTile)
	{
		pNewShopTile->wX	= pTile->wIndex_X;
		pNewShopTile->wY	= pTile->wIndex_Z;
		g_pShopTileHash->Insert(pNewShopTile, pUser->GetID());
	}

	if( IsUltra() )
	{
		pUser->SetPlayerShopMode(TRUE);
		pUser->SetPlayerShopTitle(pPacket->szTitle);
	}
	else
	{
		pUser->m_sPlayerShop.m_bPlayerShopMode = TRUE;
		lstrcpy(pUser->m_sPlayerShop.szTitle, pPacket->szTitle);
	}
	
	DSTC_PLAYERSHOP_OPEN packet;
	packet.dwUserIndex = pUser->GetID();
	memcpy(packet.szTitle, pPacket->szTitle, sizeof(pPacket->szTitle));
	pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), pUser );	

	pSendPacket.byType	= 0;
	NetSendToUser( dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
}


void CmdPlayerShopClose( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
//	CTDS_PLAYERSHOP_CLOSE* pPacket = (CTDS_PLAYERSHOP_CLOSE*)pMsg;
	
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		
	if(!pUser)
		return;
	if(pUser->GetCurLayerIndex()==255)
		return;
	if(!pUser->GetCurDungeon())
		return;

	CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();
	
	if(!pLayer)
		return;
	
	if( IsUltra() )
	{
		pUser->SetPlayerShopMode(FALSE);
		pUser->RemovePlayerShopTitle();
	}
	else
	{
		memset(pUser->m_sPlayerShop.szTitle, 0, sizeof(pUser->m_sPlayerShop.szTitle));
		pUser->m_sPlayerShop.m_bPlayerShopMode = FALSE;
	}
	
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

	if( IsUltra() )
	{
		PlayerShop_Cancel(pUser);		
	}
	else
	{
		DSTC_PLAYERSHOP_ITEM pSendPacket;

		// modified by deepdark.
		pSendPacket.bItemCount = 0;

		for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
			if(pUser->m_sPlayerShop.cItem[i].GetID() != 0)
			{
				pSendPacket.ItemNode[pSendPacket.bItemCount].index = BYTE(i);
				memcpy(&pSendPacket.ItemNode[pSendPacket.bItemCount].cItem, &pUser->m_sPlayerShop.cItem[i], sizeof(CItem)); 
				pSendPacket.bItemCount++;
			}
		}

		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		
		// 서버 처리 
		for(i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
//			int nValue = pUser->m_sPlayerShop.cItem[i].GetID()/ITEM_DISTRIBUTE;				

			if(pUser->m_sPlayerShop.cItem[i].GetID()!=0)
			{
				// 아이템 로그 추가 김영대 2005.02.23
				SetItemLog( &pUser->m_sPlayerShop.cItem[i], ITEM_LOG_PLAYERSHOT_CLOSE, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

				if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_SMALL)
				{
					int nCnt = Insert_SmallInvItem(pUser
						, &pUser->m_sPlayerShop.cItem[i]
						, 0
						, pUser->m_sPlayerShop.cItem[i].GetQuantity()
						, FALSE);					

					if(nCnt==0)
					{
						memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
					}
					else
					{
						pUser->m_sPlayerShop.cItem[i].SetQuantity((BYTE)nCnt);
					}
				}
				else if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_LARGE)
				{
					for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
					{
						if(pUser->m_pInv_Large[j].GetID()==0)
						{
							memcpy(&pUser->m_pInv_Large[j], &pUser->m_sPlayerShop.cItem[i], sizeof(CItem));
							memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
							break;
						}
					}
				}

			}
			
			pUser->m_sPlayerShop.dwCustomSellPrice[i] = 0;
		}
	}

	DSTC_PLAYERSHOP_CLOSE packet;
	packet.dwUserIndex = pUser->GetID();
	pLayer->BroadCastMsg( (char*)&packet, packet.GetPacketSize());

	// 디비 쿼리 //
	PlayerShopLogAll(pUser);
}


void CmdPlayerShopShowAll( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PLAYERSHOP_SHOWALL* pPacket = (CTDS_PLAYERSHOP_SHOWALL*)pMsg;
	
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		
	if(!pUser)
		return;

	if(pUser->GetCurLayerIndex()==INVALID_DUNGEON_LAYER)
		return;

	if(!pUser->GetCurDungeon())
		return;	

	CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();
	
	if(!pLayer)
		return;
	
	CUser* pTargetUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(!pTargetUser)
		return;

	if(pTargetUser->GetCurLayerIndex()==INVALID_DUNGEON_LAYER)
		return;

	if(!pTargetUser->GetCurDungeon())
		return;

	if (!pTargetUser->GetCurDungeonLayer())
		return;

	// modified by deepdark.		
	DSTC_PLAYERSHOP_SHOWALL SendPacket;

	SendPacket.dwUserIndex	= pTargetUser->GetID();
	// 일단 0으로 세팅.
	SendPacket.bItemCount	= 0; 

	memset(SendPacket.szTitle, 0, sizeof(SendPacket.szTitle));
	
	if( IsUltra() )
		__lstrcpyn(SendPacket.szTitle, pTargetUser->m_sPlayerShopLink.szTitle, MAX_PLAYER_SHOP_TITLE + 1);	
	else
		__lstrcpyn(SendPacket.szTitle, pTargetUser->m_sPlayerShop.szTitle, MAX_PLAYER_SHOP_TITLE);

	memset(&SendPacket.ItemNode, 0, sizeof(PLAYERSHOPINFO_NODE)*MAX_PLAYER_SHOP_INV);

	if( IsUltra() )
	{
		ITEM_NATIVE_INFO ItemNativeInfo;
		memset(&ItemNativeInfo,0,sizeof(ITEM_NATIVE_INFO));
		const ITEM_NATIVE_INFO* pItemNativeInfo = NULL;

		ItemNativeInfo.eSlotID		= ITEM_NATIVE_PLAYERSHOP;
		ItemNativeInfo.dwOwnerID	= pTargetUser->GetID();
		ItemNativeInfo.bySlotIndex	= 0;

		pItemNativeInfo = GetItemNativeLink(&ItemNativeInfo);		

		for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
			const CItem* pItemNative = GetItemNative(pLayer,&pItemNativeInfo[i]);
			if(NULL == pItemNative)
				continue;

			SendPacket.ItemNode[SendPacket.bItemCount].index = BYTE(i);
			SendPacket.ItemNode[SendPacket.bItemCount].m_dwCustomSellPrice =
				pTargetUser->GetPlayerShopCustomSellPrice(BYTE(i));

			CItem cItem;
			memcpy(&cItem,pItemNative,sizeof(CItem));

			if( cItem.IsOverlapItem() )
			{
				cItem.SetQuantity(pItemNativeInfo[i].byItemQuantityCnt);
			}
			
			memcpy( &SendPacket.ItemNode[SendPacket.bItemCount].cItem, &cItem, sizeof(CItem) );

			++SendPacket.bItemCount;
		}
	}
	else
	{
		for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
			if(pTargetUser->m_sPlayerShop.cItem[i].GetID()!=0)
			{
				SendPacket.ItemNode[SendPacket.bItemCount].index = BYTE(i);
				SendPacket.ItemNode[SendPacket.bItemCount].m_dwCustomSellPrice = 
					pTargetUser->m_sPlayerShop.dwCustomSellPrice[i];
				memcpy(&SendPacket.ItemNode[SendPacket.bItemCount].cItem, &pTargetUser->m_sPlayerShop.cItem[i], sizeof(CItem));
				
				++SendPacket.bItemCount;
			}
		}
	}

	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


//======================================================================//
// Guild.																//
//======================================================================//
void CmdGuildJoinInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	
}

void CmdGuildUserInfoDelete(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{

}

void CmdCharacterSearch(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_SEARCH pSendPacket;
	WSTDS_CHARACTER_SEARCH* pPacket = (WSTDS_CHARACTER_SEARCH*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwSearchUserIndex);

	memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
	__lstrcpyn(pSendPacket.szName, pPacket->szName, MAX_CHARACTER_REAL_LENGTH);
	pSendPacket.dwSearchUserIndex	= pPacket->dwSearchUserIndex;
	pSendPacket.dwUserIndex			= pPacket->dwUserIndex;

	if(pUser)
		pSendPacket.byMsg = 0;
	else
		pSendPacket.byMsg = 1;

	g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
}

void CmdGuildInfoMsg(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILDINFO_MSG* pPacket = (WSTDS_GUILDINFO_MSG*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	
	if(!pUser)
		return;

	if(pPacket->dwGuildId==pUser->m_dwGuildId)
	{
		memset(pUser->m_szNickName, 0, sizeof(pUser->m_szNickName));		
		__lstrcpyn(pUser->m_szNickName, pPacket->szNickName, MAX_NICK_REAL_LENGTH);		
		pUser->m_byRank	 = pPacket->byRank;					

		CDungeonLayer*	pLayer;

		if(!pUser->GetCurDungeon())
			return;

		pLayer = pUser->GetCurDungeonLayer();

		if(!pLayer)
			return;
	
		DSTC_GUILD_INFO pSendPacket;
		memset(pSendPacket.szNickName, 0, sizeof(pSendPacket.szNickName));
		__lstrcpyn(pSendPacket.szNickName, pUser->m_szNickName, MAX_NICK_REAL_LENGTH);
		pSendPacket.byRank		= pUser->m_byRank;
		pSendPacket.dwGuildId	= pUser->m_dwGuildId;
		pSendPacket.dwUserIndex	= pUser->GetID();		
		pLayer->BroadCastSectionMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), pUser->GetPrevSectionNum());
	}
}

void CmdGuildUserData(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTDS_GUILD_USERDATA* pGuildUserData = (CTDS_GUILD_USERDATA*)pMsg;		

	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if(!pUser)
		return;

	pUser->m_byGroupType		= pGuildUserData->byType;
	pUser->m_byRank		= pGuildUserData->byRank;
	pUser->SetGuildID(pGuildUserData->dwGuildId);
	__lstrcpyn(pUser->m_szGuildName, pGuildUserData->szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
	__lstrcpyn(pUser->m_szNickName, pGuildUserData->szNickName, MAX_NICK_REAL_LENGTH);			

	CDungeonLayer*	pLayer;

	if(!pUser->GetCurDungeon())
		return;

	pLayer = pUser->GetCurDungeonLayer();

	if(!pLayer)
		return;

	DSTC_GUILD_INFO pSendPacket;
	memset(pSendPacket.szNickName, 0, sizeof(pSendPacket.szNickName));
	__lstrcpyn(pSendPacket.szNickName, pUser->m_szNickName, MAX_NICK_REAL_LENGTH);
	pSendPacket.byRank		= pUser->m_byRank;
	pSendPacket.dwGuildId	= pUser->m_dwGuildId;
	pSendPacket.dwUserIndex	= pUser->GetID();		
	pLayer->BroadCastSectionMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), pUser->GetPrevSectionNum());
}

void CmdGuildItem(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{	
	CTDS_GUILD_ITEM*	pGuildItem = (CTDS_GUILD_ITEM*)pMsg;
	DSTC_GUILD_ITEM		pPacket;

	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if(!pUser)
		return;
				
	if(pUser->m_byGroupType==__GCTYPE_NONE__)
	{		
		// 아이템 인증 //	
		if(	pUser->m_pInv_Small[pGuildItem->bZipCode].m_wItemID==ITEM_GUILD_INDEX ||
			pUser->m_pInv_Small[pGuildItem->bZipCode].m_wItemID==ITEM_CLEN_INDEX)
		{
			if(pUser->m_pInv_Small[pGuildItem->bZipCode].m_wItemID==ITEM_CLEN_INDEX)
			{
				if(pUser->GetLevel() < 30)
				{
					pPacket.bResult		= FALSE;
					pPacket.bZipCode	= 5;					
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
			}
			else if(pUser->m_pInv_Small[pGuildItem->bZipCode].m_wItemID==ITEM_GUILD_INDEX)
			{
				if(pUser->GetLevel() < 10)
				{
					pPacket.bResult		= FALSE;
					pPacket.bZipCode	= 6;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);	
					return;
				}
			}
			
			if(pUser->m_pInv_Small[pGuildItem->bZipCode].GetQuantity()>0)
			{								
				BOOL				bChk = FALSE;
				DSTC_GUILD_ITEM		pPacket;

				// 체크 //
				for(int i = 0; i < pUser->m_bCount; i++)
				{
					if(__strcmp(pUser->m_szName, pUser->m_pUserCharacterInfo[i].szName)!=0)
					{
						if(pUser->m_pUserCharacterInfo[i].byType==__GCTYPE_CLEN__)
						{
							bChk = TRUE;
							break;
						}
					}
				}

				if(bChk==FALSE)
				{
					// 길드 생성 조건 //					
					pPacket.bResult		= TRUE;
					pPacket.bZipCode	= pGuildItem->bZipCode;
					pPacket.bGuildType	= pGuildItem->bGuildType;					
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
				}
				else
				{
					// 길드 생성 실패 //					
					pPacket.bResult		= FALSE;
					pPacket.bZipCode	= 4;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
				}

				return;
			}
		}
		
		// 아이템 인증 실패 //
		pPacket.bResult		= FALSE;
		pPacket.bZipCode	= 3;		
		NetSendToUser(dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		pPacket.bResult	= FALSE;

		if(pUser->m_byGroupType==__GCTYPE_GUILD__)
			pPacket.bZipCode = __GCTYPE_GUILD__;
		else
			pPacket.bZipCode = __GCTYPE_CLEN__;			
		
		NetSendToUser(dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CmdGuildInfo( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{	

}


void RemoveTradeHash(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade )
{
	g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
	memset(pItemTrade, 0, sizeof(ITEM_TRADE));

	pUser->m_nItemTradeSeriel		= 0;
	pSendUser->m_nItemTradeSeriel	= 0;	
	pUser->m_byExTrade				= 0;
	pSendUser->m_byExTrade			= 0;	
}


void AloneTradeCancel(CUser* pUser,ITEM_TRADE* pItemTrade)
{
	Trade_Cancel(pUser);

	g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
	memset(pItemTrade, 0, sizeof(ITEM_TRADE));

	pUser->m_nItemTradeSeriel	= 0;
	pUser->m_byExTrade			= 0;
}


void BothTradeCancel(CUser* pUser
					 , CUser* pSendUser
					 , ITEM_TRADE* pItemTrade
					 , BYTE byUserReturnType
					 , BYTE bySendUserReturnType)
{
	Trade_Cancel(pUser);
	Trade_Cancel(pSendUser);

	RemoveTradeHash(pUser, pSendUser, pItemTrade);

	// 실패 메세지 //
	DSTC_TRADE_RETURN pReturnPacket;
	pReturnPacket.byType = byUserReturnType;
	memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
	__lstrcpyn(pReturnPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);

	pReturnPacket.byType = bySendUserReturnType;
	memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
	__lstrcpyn(pReturnPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
	NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);
}


BOOL IsDoNotExistLinkAndKarz_DoWork(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade)
{
	__int64 nTradeMoney	= 0;
	DWORD	dwVirtualLinkItemNativeHashCount = 0;

	if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pUser->GetID())
		nTradeMoney	= pItemTrade->dwLowMoney;
	else
		nTradeMoney	= pItemTrade->dwHiMoney;

	if(nTradeMoney == 0)
	{
		dwVirtualLinkItemNativeHashCount = pUser->GetItemNativeLinkCount();

		if( dwVirtualLinkItemNativeHashCount == 0 )
		{
			nTradeMoney	= 0;

			if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pSendUser->GetID())
				nTradeMoney	= pItemTrade->dwLowMoney;
			else
				nTradeMoney	= pItemTrade->dwHiMoney;

			if(nTradeMoney == 0)
			{
				dwVirtualLinkItemNativeHashCount = pSendUser->GetItemNativeLinkCount();

				if(dwVirtualLinkItemNativeHashCount == 0)
				{
					// 트레이드 할께 둘다 없다.뭐하는겨?
					DSTC_TRADE_RETURN pReturnPacket;
					pReturnPacket.byType = 0;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}


BOOL IsTradeTradeKarz_DoWork(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade)
{
	__int64 nTradeMoney	= 0;
	__int64 nMoney		= pUser->m_dwMoney;

	if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pSendUser->GetID())
		nTradeMoney	= pItemTrade->dwHiMoney;
	else
		nTradeMoney	= pItemTrade->dwLowMoney;					

	if((nMoney + nTradeMoney) > __MAX_MONEY__ || nTradeMoney > pSendUser->m_dwMoney)
	{
		BothTradeCancel(pUser, pSendUser, pItemTrade, 1, 2);						
		return FALSE;
	}

	nMoney = pSendUser->m_dwMoney;

	if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pUser->GetID())
		nTradeMoney	= pItemTrade->dwHiMoney;
	else
		nTradeMoney	= pItemTrade->dwLowMoney;					

	if((nMoney + nTradeMoney) > __MAX_MONEY__ || nTradeMoney > pUser->m_dwMoney)
	{
		BothTradeCancel(pUser, pSendUser, pItemTrade, 2, 1);						
		return FALSE;						
	}

	return TRUE;
}


BOOL IsTradeTradeItemNativeLinkToItemNativeInv_DoWork(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade)
{
	BYTE byEmptyLargeSlotCount	= 0;
	BYTE byEmptySmallSlotCount	= 0;
	BYTE byLinkLargeItemCount	= 0;
	BYTE byLinkSmallItemCount	= 0;

	const CItem*			pItemNative			= NULL;
	const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
		
	ITEM_NATIVE_INFO ItemNativeInfo_Link;
	ITEM_NATIVE_INFO ItemNativeInfo_Real;
	ITEM_NATIVE_INFO ItemNativeInfoLarge;
	ITEM_NATIVE_INFO ItemNativeInfoSmall;

	ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_TRADE;
	ItemNativeInfo_Link.dwOwnerID	= pSendUser->GetID();						

	for(BYTE i = 0; i < MAX_TRADE_POOL; i++)
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
			if( ITEM_TYPE_LARGE == GetItemType(pItemNative->GetID()) )
				byLinkLargeItemCount++; 															
			else if( ITEM_TYPE_SMALL == GetItemType(pItemNative->GetID()) )	
				byLinkSmallItemCount++;
		}
	}

	ItemNativeInfoLarge.eSlotID		= ITEM_NATIVE_INV_LARGE;
	ItemNativeInfoLarge.dwOwnerID	= pUser->GetID();
	ItemNativeInfoLarge.bySlotIndex	= 0;
	
	pItemNative = GetItemNative(NULL, &ItemNativeInfoLarge);
	
	for(i = 0; i < MAX_INV_LARGE_POOL; i++)
	{
		if(pItemNative[i].GetID() == 0)
		{
			byEmptyLargeSlotCount++;
		}
		else
		{
			if( pSendUser->IsUserVirtualItemNativeLink(&pItemNative[i]) )
				byEmptyLargeSlotCount++;
		}
	}
	
	if(byEmptyLargeSlotCount < byLinkLargeItemCount)
	{
		BothTradeCancel(pUser, pSendUser, pItemTrade, 3, 4);
		return FALSE;	
	}
	
	ItemNativeInfoSmall.eSlotID		= ITEM_NATIVE_INV_SMALL;
	ItemNativeInfoSmall.dwOwnerID	= pUser->GetID();
	ItemNativeInfoSmall.bySlotIndex	= 0;
	
	pItemNative = GetItemNative(NULL, &ItemNativeInfoSmall);

	for(i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(pItemNative[i].GetID() == 0)
		{
			byEmptySmallSlotCount++;
		}
		else
		{
			if( pSendUser->IsUserVirtualItemNativeLink(&pItemNative[i]) )
				byEmptySmallSlotCount++;
		}
	}

	if(byEmptySmallSlotCount < byLinkSmallItemCount)
	{
		BothTradeCancel(pUser, pSendUser, pItemTrade, 5, 6);
		return FALSE;		
	}

	byEmptyLargeSlotCount	= 0;
	byEmptySmallSlotCount	= 0;
	byLinkLargeItemCount	= 0;
	byLinkSmallItemCount	= 0;

	ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_TRADE;
	ItemNativeInfo_Link.dwOwnerID	= pUser->GetID();						

	for(i = 0; i < MAX_TRADE_POOL; i++)
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
			if( ITEM_TYPE_LARGE == GetItemType(pItemNative->GetID()) )
				byLinkLargeItemCount++; 															
			else if( ITEM_TYPE_SMALL == GetItemType(pItemNative->GetID()) )	
				byLinkSmallItemCount++;
		}
	}

	ItemNativeInfoLarge.eSlotID		= ITEM_NATIVE_INV_LARGE;
	ItemNativeInfoLarge.dwOwnerID	= pSendUser->GetID();
	ItemNativeInfoLarge.bySlotIndex	= 0;
	
	pItemNative = GetItemNative(NULL, &ItemNativeInfoLarge);
	
	for(i = 0; i < MAX_INV_LARGE_POOL; i++)
	{
		if(pItemNative[i].GetID() == 0)
		{
			byEmptyLargeSlotCount++;
		}
		else
		{
			if( pSendUser->IsUserVirtualItemNativeLink(&pItemNative[i]) )
				byEmptyLargeSlotCount++;
		}
	}
	
	if(byEmptyLargeSlotCount < byLinkLargeItemCount)
	{
		BothTradeCancel(pUser, pSendUser, pItemTrade, 4, 3);
		return FALSE;	
	}
	
	ItemNativeInfoSmall.eSlotID		= ITEM_NATIVE_INV_SMALL;
	ItemNativeInfoSmall.dwOwnerID	= pSendUser->GetID();
	ItemNativeInfoSmall.bySlotIndex	= 0;
	
	pItemNative = GetItemNative(NULL, &ItemNativeInfoSmall);

	for(i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(pItemNative[i].GetID() == 0)
		{
			byEmptySmallSlotCount++;
		}
		else
		{
			if( pSendUser->IsUserVirtualItemNativeLink(&pItemNative[i]) )
				byEmptySmallSlotCount++;
		}
	}

	if(byEmptySmallSlotCount < byLinkSmallItemCount)
	{
		BothTradeCancel(pUser, pSendUser, pItemTrade, 6, 5);
		return FALSE;		
	}
	
	return TRUE;
}


void TradeItemNative(CUser* pSrcUser, CUser* pDestUser, ITEM_TRADE* pItemTrade)
{
	ITEM_NATIVE_INFO		ItemNativeInfo_Link;
	ITEM_NATIVE_INFO		ItemNativeInfo_Real;
	ITEM_NATIVE_INFO		ItemNativeInfoLarge;
	ITEM_NATIVE_INFO		ItemNativeInfoSmall;
	const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;
	const CItem*			pItemSrc			= NULL;
	const CItem*			pItemDest			= NULL;
	ITEM_TYPE				eType;
	DSTC_TRADE_DATA			TradeDataPacket;

	ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_TRADE;
	ItemNativeInfo_Link.dwOwnerID	= pSrcUser->GetID();

	for(BYTE i = 0; i < MAX_TRADE_POOL; i++)
	{
		ItemNativeInfo_Link.bySlotIndex	= i;
		pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo_Link);

		ItemNativeInfo_Real.eSlotID				= pItemNativeInfoLink->eSlotID;
		ItemNativeInfo_Real.bySlotIndex			= pItemNativeInfoLink->bySlotIndex;
		ItemNativeInfo_Real.dwOwnerID			= pItemNativeInfoLink->dwOwnerID;
		ItemNativeInfo_Real.byItemQuantityCnt	= pItemNativeInfoLink->byItemQuantityCnt; 

		pItemSrc = GetItemNative(NULL,&ItemNativeInfo_Real);

		if(pItemSrc && pItemSrc->GetID() != 0)
		{
			eType = GetItemType(pItemSrc->GetID());

			if(ITEM_TYPE_LARGE == eType)
			{
				memcpy(&TradeDataPacket.Nodes[TradeDataPacket.btItemCount++].cItem,pItemSrc, sizeof(CItem));
				RemoveItemNative(&ItemNativeInfo_Real);
				RemoveItemNativeLink(&ItemNativeInfo_Link);
			}
			else if(ITEM_TYPE_SMALL == eType)
			{
				memcpy(&TradeDataPacket.Nodes[TradeDataPacket.btItemCount++].cItem,pItemSrc, sizeof(CItem));			
				TradeDataPacket.Nodes[TradeDataPacket.btItemCount++].cItem.SetSerialNum(g_pThis->GetNewSerial());

				BYTE byRealCnt = pItemSrc->GetQuantity();
				BYTE byLinkCnt = ItemNativeInfo_Real.byItemQuantityCnt;
				TradeDataPacket.Nodes[TradeDataPacket.btItemCount++].cItem.SetQuantity(byLinkCnt);

				if(byRealCnt == byLinkCnt)
				{
					// 수량 다 링크 시켰쪄.
					RemoveItemNative(&ItemNativeInfo_Real);
				}
				else if(byRealCnt > byLinkCnt)
				{
					// 몇개만 링크 시켰쪄.
					CItem cItem;
					memcpy(&cItem, pItemSrc, sizeof(CItem));
					cItem.SetQuantity(BYTE(byRealCnt - byLinkCnt));

					SetSlotItemNative(NULL, &cItem, &ItemNativeInfo_Real);
				}

				RemoveItemNativeLink(&ItemNativeInfo_Link);
			}
		}
	}

	pSrcUser->RemoveAllVirtualItemNativeLink();

	for(i = 0; i < TradeDataPacket.btItemCount; i++)
	{
		eType = GetItemType(TradeDataPacket.Nodes[i].cItem.GetID());

		if(ITEM_TYPE_LARGE == eType)
		{
			ItemNativeInfoLarge.eSlotID		= ITEM_NATIVE_INV_LARGE;
			ItemNativeInfoLarge.dwOwnerID	= pDestUser->GetID();
			ItemNativeInfoLarge.bySlotIndex	= 0;

			pItemDest = GetItemNative(NULL,&ItemNativeInfoLarge);

			for(BYTE byLarge = 0; byLarge < MAX_INV_LARGE_POOL; byLarge++)
			{
				if( pItemDest[byLarge].GetID() == 0 )
				{
					ItemNativeInfoLarge.bySlotIndex = byLarge;
					
					SetSlotItemNative(NULL, &TradeDataPacket.Nodes[i].cItem, &ItemNativeInfoLarge);

					g_ItemMoveManager.ItemMoveLogProcess(&TradeDataPacket.Nodes[i].cItem, ITEM_LOG_TRADE
						, pDestUser->GetCurDungeonID(), (char*)pSrcUser->GetName(), (char*)pDestUser->GetName());

					TradeDataPacket.Nodes[i].index = byLarge;
					break;
				}
			}
		}
		else if(ITEM_TYPE_SMALL == eType)
		{
			ItemNativeInfoSmall.eSlotID		= ITEM_NATIVE_INV_SMALL;
			ItemNativeInfoSmall.dwOwnerID	= pDestUser->GetID();
			ItemNativeInfoSmall.bySlotIndex	= 0;

			pItemDest = GetItemNative(NULL,&ItemNativeInfoSmall);

			for(BYTE bySmall = 0; bySmall < MAX_INV_SMALL_POOL; bySmall++)
			{
				if( pItemDest[bySmall].GetID() == 0 )
				{
					ItemNativeInfoSmall.bySlotIndex = bySmall;

					SetSlotItemNative(NULL, &TradeDataPacket.Nodes[i].cItem, &ItemNativeInfoSmall);

					g_ItemMoveManager.ItemMoveLogProcess(&TradeDataPacket.Nodes[i].cItem, ITEM_LOG_TRADE
						, pDestUser->GetCurDungeonID(), (char*)pSrcUser->GetName(), (char*)pDestUser->GetName());

					TradeDataPacket.Nodes[i].index = bySmall;
					break;
				}	
			}
		}
	}

	DWORD dwMoney = 0; 

	// 카르츠 	
	if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pSrcUser->GetID())
	{
		dwMoney = pSrcUser->GetMoney();
		dwMoney = dwMoney - pItemTrade->dwHiMoney + pItemTrade->dwLowMoney;
		pSrcUser->SetMoney( dwMoney );

		TradeDataPacket.dwMoeny = dwMoney;

		if( pItemTrade->dwLowMoney > 0 )
		{	
			g_ItemMoveManager.KarzLogProcess(ITEM_LOG_TRADE_KARZ, pSrcUser->GetCurDungeonID()
				, (char*)pDestUser->GetName(), (char*)pSrcUser->GetName(), pItemTrade->dwLowMoney);
		}		
	}
	else
	{
		dwMoney = pDestUser->GetMoney();
		dwMoney = dwMoney - pItemTrade->dwLowMoney + pItemTrade->dwHiMoney;
		pDestUser->SetMoney( dwMoney );

		TradeDataPacket.dwMoeny = dwMoney;

		if( pItemTrade->dwHiMoney > 0 )
		{
			g_ItemMoveManager.KarzLogProcess(ITEM_LOG_TRADE_KARZ, pDestUser->GetCurDungeonID()
				, (char*)pSrcUser->GetName(), (char*)pDestUser->GetName(), pItemTrade->dwHiMoney);
		}		
	}

	NetSendToUser(pDestUser->m_dwConnectionIndex, (char*)&TradeDataPacket, TradeDataPacket.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);
}


//=====================================================================
// 실질적인 아이템,카르츠 트레이드 
//=====================================================================
void TradeTrade(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade)
{
	TradeItemNative(pUser, pSendUser, pItemTrade);
	TradeItemNative(pSendUser, pUser, pItemTrade);
	RemoveTradeHash(pUser, pSendUser, pItemTrade);
}


//======================================================================//
// 실질적인 트레이드 패킷 												//
//======================================================================//
void CmdTradeTrade(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CUser*				pUser	= (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
//	CTDS_TRADE_TRADE*	pPacket = (CTDS_TRADE_TRADE*)pMsg;
	DSTC_TRADE_TRADE	pSendPacket;

	ITEM_TRADE* pItemTrade = g_pItemTradeHash->GetData(pUser->m_nItemTradeSeriel);

	if(pItemTrade)	
	{		
		int nUserIndex = 0;
		
		if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pUser->GetID())	
			nUserIndex = pItemTrade->TradeSerialStruct.dwLowUserIndex;
		else
			nUserIndex = pItemTrade->TradeSerialStruct.dwHiUserIndex;
		
		CUser* pSendUser = g_pUserHash->GetData(nUserIndex);

		if(!pSendUser)
		{
			AloneTradeCancel(pUser, pItemTrade);			
			return;
		}
		else
		{
			if(pUser->m_nItemTradeSeriel != pSendUser->m_nItemTradeSeriel)
			{
				DSTC_TRADE_CANCELFAIL pSnedUserPacket;
				pSnedUserPacket.byType	= 1;
				memset(pSnedUserPacket.szCharacterName, 0, sizeof(pSnedUserPacket.szCharacterName));
				__lstrcpyn(pSnedUserPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSnedUserPacket, pSnedUserPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
		}

		// Process 
		if(pItemTrade->byHiUserRt!=2 || pItemTrade->byLowUserRt!=2)
		{			
			if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pUser->GetID())
				pItemTrade->byHiUserRt = 2;
			else
				pItemTrade->byLowUserRt = 2;
			
			if( IsUltra() )
			{
				// 양쪽 다 교환일 경우 
				if(pItemTrade->byHiUserRt == 2 && pItemTrade->byLowUserRt == 2)
				{
					// 아이템 교환이 가능한지 테스트 ,교환할 아이템과 돈이 있는지
					if( FALSE == IsDoNotExistLinkAndKarz_DoWork(pUser, pSendUser, pItemTrade) )
						return;

					// 돈을 체크 
					if( FALSE == IsTradeTradeKarz_DoWork(pUser, pSendUser, pItemTrade) )
						return;
						
					// 트레이드 아이템이 들어갈 슬롯을 체크
					if( FALSE == IsTradeTradeItemNativeLinkToItemNativeInv_DoWork(pUser, pSendUser, pItemTrade) )
						return;					

					// 여기까지왔으면 실제 아이템을 트레이드 해준다.
					TradeTrade(pUser, pSendUser, pItemTrade);
				}
				else
				{
					pSendPacket.byType	= 0;
					memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
					__lstrcpyn(pSendPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);

					pSendPacket.byType	= 1;
					memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
					__lstrcpyn(pSendPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
				}
			}
			else
			{
				// 양쪽 다 교환일 경우 
				if(pItemTrade->byHiUserRt == 2 && pItemTrade->byLowUserRt == 2)
				{
					// 아이템 교환이 가능한지 테스트 
					// 교환할 아이템과 돈이 있는지 
					__int64 nTradeMoney	= 0;
					
					if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pUser->GetID())
						nTradeMoney	= pItemTrade->dwLowMoney;
					else
						nTradeMoney	= pItemTrade->dwHiMoney;
					
					BOOL bChk = FALSE;
					
					if(nTradeMoney == 0)
					{
						for(int i = 0; i < MAX_TRADE_POOL; i++)
						{
							if(pUser->m_pTradeItem[i].GetID()!=0)
							{
								bChk = TRUE;
								break;							
							}
						}

						if(bChk == FALSE)
						{
							nTradeMoney	= 0;
					
							if(pItemTrade->TradeSerialStruct.dwHiUserIndex==pSendUser->GetID())
								nTradeMoney	= pItemTrade->dwLowMoney;
							else
								nTradeMoney	= pItemTrade->dwHiMoney;

							if(nTradeMoney == 0)
							{
								for(int i = 0; i < MAX_TRADE_POOL; i++)
								{
									if(pSendUser->m_pTradeItem[i].GetID()!=0)
									{
										bChk = TRUE;
										break;							
									}
								}

								if(bChk==FALSE)
								{
									// 실패 메세지 //
									DSTC_TRADE_RETURN pReturnPacket;
									pReturnPacket.byType = 0;
									NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									return;
								}
							}
						}
					}
					
					// 돈을 체크 //
					__int64 nMoney	= pUser->m_dwMoney;

					if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pSendUser->GetID())
						nTradeMoney	= pItemTrade->dwHiMoney;
					else
						nTradeMoney	= pItemTrade->dwLowMoney;					

					if((nMoney + nTradeMoney) > __MAX_MONEY__ || nTradeMoney > pSendUser->m_dwMoney)
					{
						Trade_Cancel(pUser);
						Trade_Cancel(pSendUser);

						g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
						memset(pItemTrade, 0, sizeof(ITEM_TRADE));

						pUser->m_nItemTradeSeriel		= 0;
						pSendUser->m_nItemTradeSeriel	= 0;
						pUser->m_byExTrade				= 0;
						pSendUser->m_byExTrade			= 0;
						
						// 실패 메세지 //
						DSTC_TRADE_RETURN pReturnPacket;
						pReturnPacket.byType = 1;
						memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
						__lstrcpyn(pReturnPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						pReturnPacket.byType = 2;
						memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
						__lstrcpyn(pReturnPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						return;
					}

					nMoney = pSendUser->m_dwMoney;

					if(pItemTrade->TradeSerialStruct.dwHiUserIndex==pUser->GetID())
						nTradeMoney	= pItemTrade->dwHiMoney;
					else
						nTradeMoney	= pItemTrade->dwLowMoney;					

					if((nMoney + nTradeMoney) > __MAX_MONEY__ || nTradeMoney > pUser->m_dwMoney)
					{
						Trade_Cancel(pUser);
						Trade_Cancel(pSendUser);
						g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
						memset(pItemTrade, 0, sizeof(ITEM_TRADE));
						pUser->m_nItemTradeSeriel		= 0;
						pSendUser->m_nItemTradeSeriel	= 0;
						pUser->m_byExTrade				= 0;
						pSendUser->m_byExTrade			= 0;
						// 실패 메세지 //					
						DSTC_TRADE_RETURN pReturnPacket;
						pReturnPacket.byType = 2;					
						memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
						__lstrcpyn(pReturnPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						pReturnPacket.byType = 1;
						memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
						__lstrcpyn(pReturnPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						return;
					}				
					
					// 아이템 체크 //				
					memcpy(pItemTrade->pTradeLargeItem, pUser->m_pInv_Large, sizeof(CItem)*MAX_INV_LARGE_POOL);
					memcpy(pItemTrade->pTradeSmallItem, pUser->m_pInv_Small, sizeof(CItem)*MAX_INV_SMALL_POOL);

					// 장비아이템 //
					for(int i = 0; i < MAX_TRADE_POOL; i++)
					{
						if(pSendUser->m_pTradeItem[i].GetID()!=0)
						{
//							int nValue = pSendUser->m_pTradeItem[i].GetID()/ITEM_DISTRIBUTE;

							if(GetItemType(pSendUser->m_pTradeItem[i].GetID())==ITEM_TYPE_LARGE)
							{
								BOOL bItemTradeChk = FALSE;

								for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
								{
									if(pItemTrade->pTradeLargeItem[j].GetID()==0)
									{
										memcpy(&pItemTrade->pTradeLargeItem[j], &pSendUser->m_pTradeItem[i], sizeof(CItem));
										bItemTradeChk = TRUE;
										break;
									}
								}

								if(bItemTradeChk==FALSE)
								{
									Trade_Cancel(pUser);
									Trade_Cancel(pSendUser);

									g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
									memset(pItemTrade, 0, sizeof(ITEM_TRADE));
									pUser->m_nItemTradeSeriel		= 0;
									pSendUser->m_nItemTradeSeriel	= 0;
									pUser->m_byExTrade				= 0;
									pSendUser->m_byExTrade			= 0;
									
									// 실패 메세지 //					
									DSTC_TRADE_RETURN pReturnPacket;
									pReturnPacket.byType = 3;					
									memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
									__lstrcpyn(pReturnPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

									pReturnPacket.byType = 4;
									memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
									__lstrcpyn(pReturnPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									return;
								}
								
							}
						}
					}
					
					// 소비 아이템 //
					for(i = 0; i < MAX_TRADE_POOL; i++)
					{
						if(pSendUser->m_pTradeItem[i].GetID()!=0)
						{
//							int nValue = pSendUser->m_pTradeItem[i].GetID()/ITEM_DISTRIBUTE;

							if(GetItemType(pSendUser->m_pTradeItem[i].GetID())==ITEM_TYPE_SMALL)
							{
								if(Trade_SmallItem(pItemTrade->pTradeSmallItem, &pSendUser->m_pTradeItem[i])==FALSE)
								{
									Trade_Cancel(pUser);
									Trade_Cancel(pSendUser);
									g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
									memset(pItemTrade, 0, sizeof(ITEM_TRADE));
									pUser->m_nItemTradeSeriel		= 0;
									pSendUser->m_nItemTradeSeriel	= 0;
									pUser->m_byExTrade				= 0;
									pSendUser->m_byExTrade			= 0;
									
									// 실패 메세지 //
									DSTC_TRADE_RETURN pReturnPacket;
									pReturnPacket.byType = 5;					
									memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
									__lstrcpyn(pReturnPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

									pReturnPacket.byType = 6;
									memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
									__lstrcpyn(pReturnPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									return;
								}
							}
						}
					}								

					memcpy(pItemTrade->pTradeLargeItem2, pSendUser->m_pInv_Large, sizeof(CItem)*MAX_INV_LARGE_POOL);
					memcpy(pItemTrade->pTradeSmallItem2, pSendUser->m_pInv_Small, sizeof(CItem)*MAX_INV_SMALL_POOL);

					// 장비아이템 //
					for(i = 0; i < MAX_TRADE_POOL; i++)
					{
						if(pUser->m_pTradeItem[i].GetID()!=0)
						{
//							int nValue = pUser->m_pTradeItem[i].GetID()/ITEM_DISTRIBUTE;

							if(GetItemType(pUser->m_pTradeItem[i].GetID())==ITEM_TYPE_LARGE)
							{
								BOOL bItemTradeChk = FALSE;

								for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
								{
									if(pItemTrade->pTradeLargeItem2[j].GetID()==0)
									{
										memcpy(&pItemTrade->pTradeLargeItem2[j], &pUser->m_pTradeItem[i], sizeof(CItem));
										bItemTradeChk = TRUE;
										break;
									}
								}

								if(bItemTradeChk==FALSE)
								{
									Trade_Cancel(pUser);
									Trade_Cancel(pSendUser);
									g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
									memset(pItemTrade, 0, sizeof(ITEM_TRADE));
									pUser->m_nItemTradeSeriel		= 0;
									pSendUser->m_nItemTradeSeriel	= 0;
									pUser->m_byExTrade				= 0;
									pSendUser->m_byExTrade			= 0;
									
									// 실패 메세지 //
									DSTC_TRADE_RETURN pReturnPacket;
									pReturnPacket.byType = 4;					
									memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
									__lstrcpyn(pReturnPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

									pReturnPacket.byType = 3;
									memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
									__lstrcpyn(pReturnPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									return;
								}
								
							}
						}
					}
					
					// 소비 아이템 //
					for(i = 0; i < MAX_TRADE_POOL; i++)
					{
						if(pUser->m_pTradeItem[i].GetID()!=0)
						{
//							int nValue = pUser->m_pTradeItem[i].GetID()/ITEM_DISTRIBUTE;

							if(GetItemType(pUser->m_pTradeItem[i].GetID())==ITEM_TYPE_SMALL)
							{
								if(Trade_SmallItem(pItemTrade->pTradeSmallItem2, &pUser->m_pTradeItem[i])==FALSE)
								{
									Trade_Cancel(pUser);
									Trade_Cancel(pSendUser);
									g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
									memset(pItemTrade, 0, sizeof(ITEM_TRADE));
									pUser->m_nItemTradeSeriel		= 0;
									pSendUser->m_nItemTradeSeriel	= 0;
									pUser->m_byExTrade				= 0;
									pSendUser->m_byExTrade			= 0;
									
									// 실패 메세지 //
									DSTC_TRADE_RETURN pReturnPacket;
									pReturnPacket.byType = 6;					
									memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
									__lstrcpyn(pReturnPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

									pReturnPacket.byType = 5;
									memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));
									__lstrcpyn(pReturnPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									return;
								}
							}
						}
					}
					
					// 검사 //
					BOOL bItemChk = FALSE;

					for( i=0; i<MAX_TRADE_POOL; i++ )
					{
						if(pUser->m_pTradeItem[i].m_wItemID!=0)
						{
							if(GetItemType(pUser->m_pTradeItem[i].GetID())==ITEM_TYPE_LARGE)
							{
								if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
								{
									if(IsCopyItem(pSendUser, &pUser->m_pTradeItem[i], COPY_CHECK_BANK_LARGE)==TRUE)									
									{	
										bItemChk = TRUE;
										break;
									}
								}

								if(IsCopyItem(pSendUser, &pUser->m_pTradeItem[i], COPY_CHECK_INV_LARGE)==TRUE)
								{
									bItemChk = TRUE;							
									break;
								}

								if(IsCopyItem(pSendUser, &pUser->m_pTradeItem[i], COPY_CHECK_EQUIP)==TRUE)
								{
									bItemChk = TRUE;							
									break;
								}
							}
							else
							{
								if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
								{
									if(IsCopyItem(pSendUser, &pUser->m_pTradeItem[i], COPY_CHECK_BANK_SMALL)==TRUE)
									{	
										bItemChk = TRUE;							
										break;
									}
								}

								if(IsCopyItem(pSendUser, &pUser->m_pTradeItem[i], COPY_CHECK_INV_SMALL)==TRUE)
								{		
									bItemChk = TRUE;							
									break;
								}

								if(IsCopyItem(pSendUser, &pUser->m_pTradeItem[i], COPY_CHECK_BELT)==TRUE)
								{
									bItemChk = TRUE;							
									break;
								}
							}
						}

						if(pSendUser->m_pTradeItem[i].m_wItemID!=0)
						{
							if(GetItemType(pSendUser->m_pTradeItem[i].GetID())==ITEM_TYPE_LARGE)
							{
								if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
								{
									if(IsCopyItem(pUser, &pSendUser->m_pTradeItem[i], COPY_CHECK_BANK_LARGE)==TRUE)									
									{	
										bItemChk = TRUE;
										break;
									}
								}

								if(IsCopyItem(pUser, &pSendUser->m_pTradeItem[i], COPY_CHECK_INV_LARGE)==TRUE)
								{
									bItemChk = TRUE;							
									break;
								}

								if(IsCopyItem(pUser, &pSendUser->m_pTradeItem[i], COPY_CHECK_EQUIP)==TRUE)
								{
									bItemChk = TRUE;							
									break;
								}
							}
							else
							{
								if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
								{
									if(IsCopyItem(pUser, &pSendUser->m_pTradeItem[i], COPY_CHECK_BANK_SMALL)==TRUE)
									{	
										bItemChk = TRUE;							
										break;
									}
								}

								if(IsCopyItem(pUser, &pSendUser->m_pTradeItem[i], COPY_CHECK_INV_SMALL)==TRUE)
								{		
									bItemChk = TRUE;							
									break;
								}

								if(IsCopyItem(pUser, &pSendUser->m_pTradeItem[i], COPY_CHECK_BELT)==TRUE)
								{
									bItemChk = TRUE;							
									break;
								}
							}
						}		
					}

					if(bItemChk)
					{
						Trade_Cancel(pUser);
						Trade_Cancel(pSendUser);

						g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
						memset(pItemTrade, 0, sizeof(ITEM_TRADE));
						pUser->m_nItemTradeSeriel		= 0;
						pSendUser->m_nItemTradeSeriel	= 0;
						pUser->m_byExTrade				= 0;
						pSendUser->m_byExTrade			= 0;
						
						// 실패 메세지 //
						DSTC_TRADE_RETURN pReturnPacket;
						pReturnPacket.byType = 8;					
						memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));					
						NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						pReturnPacket.byType = 8;
						memset(pReturnPacket.szCharacterName, 0, sizeof(pReturnPacket.szCharacterName));					
						NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pReturnPacket, pReturnPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						return;
					}

					for( i=0; i<MAX_TRADE_POOL; i++ )
					{
						if( pUser->m_pTradeItem[i].m_wItemID != 0 )
							SetItemLog( &pUser->m_pTradeItem[i], ITEM_LOG_EXCHANGE,
										(WORD)pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID, pUser->m_szName, pSendUser->m_szName );

						if( pSendUser->m_pTradeItem[i].m_wItemID != 0 )
							SetItemLog( &pSendUser->m_pTradeItem[i], ITEM_LOG_EXCHANGE,
										(WORD)pSendUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID, pSendUser->m_szName, pUser->m_szName );
					}

					// 카르츠 로그 
					DWORD dwMoney = 0;
					if(pItemTrade->TradeSerialStruct.dwHiUserIndex==pUser->GetID())
					{
						dwMoney = pItemTrade->dwHiMoney;
					}
					else
					{
						dwMoney = pItemTrade->dwLowMoney;
					}

					if(dwMoney > 0)
					{
						SetItemLogKarz(	ITEM_LOG_TRADE
							, (WORD)pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID
							, pUser->m_szName
							, pSendUser->m_szName,dwMoney);
					}
					
					if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pSendUser->GetID())
					{
						dwMoney = pItemTrade->dwHiMoney;	
					}
					else
					{
						dwMoney = pItemTrade->dwLowMoney;
					}
					
					if(dwMoney > 0)
					{
//						CItem* pItem = NULL;
						SetItemLogKarz(	ITEM_LOG_TRADE
							, (WORD)pSendUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID
							, pSendUser->m_szName,pUser->m_szName
							, dwMoney);
					}

					// DB 쿼리 //
					SDBTRADE sDBTrade;

					if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pUser->GetID())
					{
						sDBTrade.dwMoeny1		= pItemTrade->dwHiMoney;
						sDBTrade.dwMoeny2		= pItemTrade->dwLowMoney;
						sDBTrade.dwMoeny1		= (pUser->m_dwMoney - sDBTrade.dwMoeny1 + pItemTrade->dwLowMoney);
						sDBTrade.dwMoeny2		= (pSendUser->m_dwMoney - sDBTrade.dwMoeny2 + pItemTrade->dwHiMoney);
						pItemTrade->dwHiMoney	= sDBTrade.dwMoeny1;
						pItemTrade->dwLowMoney	= sDBTrade.dwMoeny2;
					}
					else
					{
						sDBTrade.dwMoeny1		= pItemTrade->dwLowMoney;
						sDBTrade.dwMoeny2		= pItemTrade->dwHiMoney;
						sDBTrade.dwMoeny1		= (pUser->m_dwMoney - sDBTrade.dwMoeny1 + pItemTrade->dwHiMoney);
						sDBTrade.dwMoeny2		= (pSendUser->m_dwMoney - sDBTrade.dwMoeny2 + pItemTrade->dwLowMoney);
						pItemTrade->dwHiMoney	= sDBTrade.dwMoeny1;
						pItemTrade->dwLowMoney	= sDBTrade.dwMoeny2;
					}	
					
					memset(sDBTrade.szName1, 0, sizeof(sDBTrade.szName1));
					memset(sDBTrade.szName2, 0, sizeof(sDBTrade.szName2));
					__lstrcpyn(sDBTrade.szName1, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					__lstrcpyn(sDBTrade.szName2, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);

					memcpy(sDBTrade.cLargeItem1, pItemTrade->pTradeLargeItem, sizeof(CItem)*MAX_INV_LARGE_POOL);
					memcpy(sDBTrade.cLargeItem2, pItemTrade->pTradeLargeItem2, sizeof(CItem)*MAX_INV_LARGE_POOL);
					memcpy(sDBTrade.cSmallItem1, pItemTrade->pTradeSmallItem, sizeof(CItem)*MAX_INV_SMALL_POOL);
					memcpy(sDBTrade.cSmallItem2, pItemTrade->pTradeSmallItem2, sizeof(CItem)*MAX_INV_SMALL_POOL);

					char	szQuery[1024]	= {0,};
					int		nColumn			= 8;	
					int		col				= 0;
					DWORD	dwParamOffset	= 0;
		
					DBBINDING* pBinding = g_pDb->THCreateParamInfo((WORD)nColumn);

					if(!pBinding)
					{
						Log(LOG_IMPORTANT, "ParameterPool is Full!!  in CmdTradeTrade Function()!");
						return;
					}

					pBinding[col].obValue = dwParamOffset;
					pBinding[col].cbMaxLen = sizeof(sDBTrade.szName1);
					pBinding[col++].wType = DBTYPE_STR;					
					dwParamOffset += sizeof(sDBTrade.szName1);

					pBinding[col].obValue = dwParamOffset;
					pBinding[col].cbMaxLen = sizeof(sDBTrade.dwMoeny1);
					pBinding[col++].wType = DBTYPE_I4;					
					dwParamOffset += sizeof(sDBTrade.dwMoeny1);
					
					pBinding[col].obValue = dwParamOffset;
					pBinding[col].cbMaxLen = sizeof(sDBTrade.cLargeItem1);
					pBinding[col++].wType = DBTYPE_BYTES;					
					dwParamOffset += sizeof(sDBTrade.cLargeItem1);
					
					pBinding[col].obValue = dwParamOffset;
					pBinding[col].cbMaxLen = sizeof(sDBTrade.cSmallItem1);
					pBinding[col++].wType = DBTYPE_BYTES;					
					dwParamOffset += sizeof(sDBTrade.cSmallItem1);
					
					pBinding[col].obValue = dwParamOffset;
					pBinding[col].cbMaxLen = sizeof(sDBTrade.szName2);
					pBinding[col++].wType = DBTYPE_STR;					
					dwParamOffset += sizeof(sDBTrade.szName2);
					
					pBinding[col].obValue = dwParamOffset;
					pBinding[col].cbMaxLen = sizeof(sDBTrade.dwMoeny2);
					pBinding[col++].wType = DBTYPE_I4;					
					dwParamOffset += sizeof(sDBTrade.dwMoeny2);

					pBinding[col].obValue = dwParamOffset;
					pBinding[col].cbMaxLen = sizeof(sDBTrade.cLargeItem2);
					pBinding[col++].wType = DBTYPE_BYTES;					
					dwParamOffset += sizeof(sDBTrade.cLargeItem2);
					
					pBinding[col].obValue = dwParamOffset;
					pBinding[col].cbMaxLen = sizeof(sDBTrade.cSmallItem2);
					pBinding[col++].wType = DBTYPE_BYTES;					
					dwParamOffset += sizeof(sDBTrade.cSmallItem2);
					
					wsprintf(szQuery, "CZP_TRADE ?, ?, ?, ?, ?, ?, ?, ?");
					g_pDb->THExecuteSQLByParam(szQuery
						, pBinding
						, &sDBTrade
						, sizeof(sDBTrade)
						, (BYTE)nColumn
						, TRUE
						, QUERY_TYPE_TRADE
						, (void*)pUser->m_dwConnectionIndex
						, GAME_DB);
				}
				else
				{
					// 메세지 보내기 //
					pSendPacket.byType	= 0;
					memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
					__lstrcpyn(pSendPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);

					pSendPacket.byType	= 1;
					memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
					__lstrcpyn(pSendPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
				}			
			}
		}
	}
}


//==========================================================================================
// 트레이드할 모든 준비가 끝났다. 트레이드할 아이템을 클라이언트에 보여준다.
//==========================================================================================
void CmdTradeOk(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTDS_TRADE_OK*	pPacket	= (CTDS_TRADE_OK*)pMsg;
	CUser*			pUser	= (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	int				nIndex	= 0;

	if(!pUser)
		return;

	ITEM_TRADE* pItemTrade = g_pItemTradeHash->GetData(pUser->m_nItemTradeSeriel);

	if(pItemTrade)	
	{
		if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pUser->GetID())
		{
			pItemTrade->byHiUserRt	= 1;
			pItemTrade->dwHiMoney	= pPacket->dwMoney;
			nIndex					= pItemTrade->TradeSerialStruct.dwLowUserIndex;
		}
		else
		{
			pItemTrade->byLowUserRt	= 1;
			pItemTrade->dwLowMoney	= pPacket->dwMoney;
			nIndex					= pItemTrade->TradeSerialStruct.dwHiUserIndex;
		}
			
		CUser* pSendUser = g_pUserHash->GetData(nIndex);

		if(!pSendUser)
		{
			if( IsUltra() )
			{
				AloneTradeCancel(pUser, pItemTrade);
				return;
			}
			else
			{
				DSTC_TRADE_CANCEL pUserPacket;
				pUserPacket.byType = 0;

				for(int i = 0; i < MAX_TRADE_POOL; i++)
				{
					pUserPacket.bInvType[i]		= 0;

					if(pUser->m_pTradeItem[i].GetID() != 0)
					{					
						pUserPacket.Nodes[pUserPacket.btItemCount].index	= BYTE(i);
						memcpy(&pUserPacket.Nodes[pUserPacket.btItemCount++].cItem, &pUser->m_pTradeItem[i], sizeof(CItem));
					}
				}

				// Item Move //
				for(i = 0; i < MAX_TRADE_POOL; i++)
				{
//					int nValue = pUser->m_pTradeItem[i].GetID();

					if(pUser->m_pTradeItem[i].GetID()!=0)
					{
						// 스몰 아이템 //
						if(GetItemType(pUser->m_pTradeItem[i].GetID())==ITEM_TYPE_SMALL)
						{
							Insert_SmallInvItem(pUser
								, &pUser->m_pTradeItem[i]
								, 0
								, pUser->m_pTradeItem[i].GetQuantity()
								, TRUE);
						}
						else
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

				g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
				memset(pItemTrade, 0, sizeof(ITEM_TRADE));

				pUser->m_nItemTradeSeriel	= 0;
				pUser->m_byExTrade			= 0;

				return;
			}			
		}
				
		if(pUser->m_nItemTradeSeriel != pSendUser->m_nItemTradeSeriel)
		{
			// 취소메세지 //
			DSTC_TRADE_CANCELFAIL pSnedUserPacket;

			pSnedUserPacket.byType	= 1;

			memset(pSnedUserPacket.szCharacterName, 0, sizeof(pSnedUserPacket.szCharacterName));
			__lstrcpyn(pSnedUserPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);

			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSnedUserPacket, pSnedUserPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);

			return;
		}

		// 있으면 //
		if(pItemTrade->byHiUserRt==1 && pItemTrade->byLowUserRt==1)
		{
			// 교환 활성화 //
			if(pPacket->dwMoney <= pUser->m_dwMoney)
			{
				// Item 보내기 //
				DSTC_TRADE_ITEM pUserDataPacket;
				
				// modified by deepdark.
				memset(pUserDataPacket.szCharacterName, 0, sizeof(pUserDataPacket.szCharacterName));
				__lstrcpyn(pUserDataPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				pUserDataPacket.dwMoney	= pPacket->dwMoney;
				pUserDataPacket.byType	= 2;

				if( IsUltra() )
				{
					ITEM_NATIVE_INFO ItemNativeInfo_Link;
					ITEM_NATIVE_INFO ItemNativeInfo_Real;

					const CItem*			pItemNative			= NULL;
					const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

					ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_TRADE;
					ItemNativeInfo_Link.dwOwnerID	= pUser->GetID();

					for(int i = 0; i < MAX_TRADE_POOL; i++)
					{
						ItemNativeInfo_Link.bySlotIndex	= BYTE(i);
						pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo_Link);

						ItemNativeInfo_Real.eSlotID				= pItemNativeInfoLink->eSlotID;
						ItemNativeInfo_Real.bySlotIndex			= pItemNativeInfoLink->bySlotIndex;
						ItemNativeInfo_Real.dwOwnerID			= pItemNativeInfoLink->dwOwnerID;
						ItemNativeInfo_Real.byItemQuantityCnt	= pItemNativeInfoLink->byItemQuantityCnt; 

						pItemNative = GetItemNative(NULL,&ItemNativeInfo_Real);	
						
						if(pItemNative && pItemNative->GetID() != 0)
						{
							CItem cItem;
							memcpy(&cItem, pItemNative, sizeof(CItem));

							if(cItem.IsOverlapItem())
							{
								cItem.SetQuantity(pItemNativeInfoLink->byItemQuantityCnt);
							}
							
							pUserDataPacket.Nodes[pUserDataPacket.bItemCount].index = BYTE(i);
							memcpy(&pUserDataPacket.Nodes[pUserDataPacket.bItemCount++].cItem
								, &cItem
								, sizeof(CItem));							
						}
					}
				}
				else
				{
					for(int i = 0; i < MAX_TRADE_POOL; i++)
					{
						if(pUser->m_pTradeItem[i].GetID() !=0)
						{
							pUserDataPacket.Nodes[pUserDataPacket.bItemCount].index = BYTE(i);
							memcpy(&pUserDataPacket.Nodes[pUserDataPacket.bItemCount++].cItem
								, &pUser->m_pTradeItem[i]
								, sizeof(CItem));
						}
					}
				}

				NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pUserDataPacket, pUserDataPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);

				DSTC_TRADE_OK pSendUserRtPacket;
				memset(pSendUserRtPacket.szCharacterName, 0, sizeof(pSendUserRtPacket.szCharacterName));
				__lstrcpyn(pSendUserRtPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				pSendUserRtPacket.byType = 2;

				NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendUserRtPacket, pSendUserRtPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);

				DSTC_TRADE_OK pUserRtPacket;
				memset(pUserRtPacket.szCharacterName, 0, sizeof(pUserRtPacket.szCharacterName));
				__lstrcpyn(pUserRtPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				pUserRtPacket.byType = 2;

				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pUserRtPacket, pUserRtPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
			}
		}
		else
		{
			if(pPacket->dwMoney <= pUser->m_dwMoney)
			{
				// Item 보내기 //
				DSTC_TRADE_ITEM pUserDataPacket;

				// modified by deepdark.
				memset(pUserDataPacket.szCharacterName, 0, sizeof(pUserDataPacket.szCharacterName));
				__lstrcpyn(pUserDataPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				pUserDataPacket.dwMoney	= pPacket->dwMoney;
				pUserDataPacket.byType	= 1;

				if( IsUltra() )
				{
					ITEM_NATIVE_INFO ItemNativeInfo_Link;
					ITEM_NATIVE_INFO ItemNativeInfo_Real;

					const CItem*			pItemNative			= NULL;
					const ITEM_NATIVE_INFO*	pItemNativeInfoLink = NULL;

					ItemNativeInfo_Link.eSlotID		= ITEM_NATIVE_TRADE;
					ItemNativeInfo_Link.dwOwnerID	= pUser->GetID();

					for(int i = 0; i < MAX_TRADE_POOL; i++)
					{
						ItemNativeInfo_Link.bySlotIndex	= BYTE(i);
						pItemNativeInfoLink = GetItemNativeLink(&ItemNativeInfo_Link);

						ItemNativeInfo_Real.eSlotID				= pItemNativeInfoLink->eSlotID;
						ItemNativeInfo_Real.bySlotIndex			= pItemNativeInfoLink->bySlotIndex;
						ItemNativeInfo_Real.dwOwnerID			= pItemNativeInfoLink->dwOwnerID;
						ItemNativeInfo_Real.byItemQuantityCnt	= pItemNativeInfoLink->byItemQuantityCnt; 

						pItemNative = GetItemNative(NULL,&ItemNativeInfo_Real);	
						
						if(pItemNative && pItemNative->GetID() != 0)
						{
							CItem cItem;
							memcpy(&cItem, pItemNative, sizeof(CItem));

							if(cItem.IsOverlapItem())
							{
								cItem.SetQuantity(pItemNativeInfoLink->byItemQuantityCnt);
							}
							
							pUserDataPacket.Nodes[pUserDataPacket.bItemCount].index = BYTE(i);
							memcpy(&pUserDataPacket.Nodes[pUserDataPacket.bItemCount++].cItem
								, &cItem
								, sizeof(CItem));							
						}
					}
				}
				else
				{
					for(int i = 0; i < MAX_TRADE_POOL; i++)
					{
						if(pUser->m_pTradeItem[i].GetID() !=0)
						{	
							pUserDataPacket.Nodes[pUserDataPacket.bItemCount].index = BYTE(i);
							memcpy(&pUserDataPacket.Nodes[pUserDataPacket.bItemCount++].cItem
								, &pUser->m_pTradeItem[i]
								, sizeof(CItem));
						}
					}
				}

				NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pUserDataPacket, pUserDataPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);

				DSTC_TRADE_OK pUserRtPacket;
				memset(pUserRtPacket.szCharacterName, 0, sizeof(pUserRtPacket.szCharacterName));
				__lstrcpyn(pUserRtPacket.szCharacterName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				pUserRtPacket.byType = 1;			
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pUserRtPacket, pUserRtPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				
				DSTC_TRADE_OK pSendUserRtPacket;
				memset(pSendUserRtPacket.szCharacterName, 0, sizeof(pSendUserRtPacket.szCharacterName));
				__lstrcpyn(pSendUserRtPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
				pSendUserRtPacket.byType = 3;			
				NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendUserRtPacket, pSendUserRtPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
			}
		}				
	}
}


//==========================================================================================
// 
//==========================================================================================
void CmdTradeCancel(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
//	CTDS_TRADE_CALCEL*	pPacket	= (CTDS_TRADE_CALCEL*)pMsg;
	CUser*				pUser	= (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	
	if(!pUser)
		return;	
		
	ITEM_TRADE* pItemTrade = g_pItemTradeHash->GetData(pUser->m_nItemTradeSeriel);

	if(pItemTrade)
	{
		DWORD dwIndex = 0;

		if(pUser)
		{
			if(pItemTrade->TradeSerialStruct.dwHiUserIndex==pUser->GetID())
				dwIndex = pItemTrade->TradeSerialStruct.dwLowUserIndex;
			else
				dwIndex = pItemTrade->TradeSerialStruct.dwHiUserIndex;						

			CUser* pSendUser = g_pUserHash->GetData(dwIndex);

			if( IsUltra() )
			{
				AloneTradeCancel(pUser, pItemTrade);
			}
			else
			{
				// 자기 자신 //
				DSTC_TRADE_CANCEL pUserPacket;
				pUserPacket.byType = 0;
				
				for(int i = 0; i < MAX_TRADE_POOL; i++)
				{
					pUserPacket.bInvType[i]		= 0;
					
					if(pUser->m_pTradeItem[i].GetID() != 0)
					{					
						pUserPacket.Nodes[pUserPacket.btItemCount].index = BYTE(i);
						memcpy(&pUserPacket.Nodes[pUserPacket.btItemCount++].cItem, &pUser->m_pTradeItem[i], sizeof(CItem));
					}
				}

				// Item Move //
				for(i = 0; i < MAX_TRADE_POOL; i++)
				{
//					int nValue = pUser->m_pTradeItem[i].GetID()/ITEM_DISTRIBUTE;

					if(pUser->m_pTradeItem[i].GetID()!=0)
					{
						if(GetItemType(pUser->m_pTradeItem[i].GetID())==ITEM_TYPE_SMALL)
						{
							// 스몰 아이템 //
							Insert_SmallInvItem(pUser
								, &pUser->m_pTradeItem[i]
								, 0
								, pUser->m_pTradeItem[i].GetQuantity()
								, TRUE);
						}
						else if(GetItemType(pUser->m_pTradeItem[i].GetID())==ITEM_TYPE_LARGE)
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
								
			if(pSendUser)
			{
				if(pSendUser->m_nItemTradeSeriel == 0)
				{
					g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
					memset(pItemTrade, 0, sizeof(ITEM_TRADE));
				}
				else
				{
					// 취소메세지 //
					DSTC_TRADE_CANCELFAIL pSnedUserPacket;
					pSnedUserPacket.byType	= 0;
					memset(pSnedUserPacket.szCharacterName, 0, sizeof(pSnedUserPacket.szCharacterName));
					__lstrcpyn(pSnedUserPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSnedUserPacket, pSnedUserPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
			
			if(!pSendUser)
			{
				g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
				memset(pItemTrade, 0, sizeof(ITEM_TRADE));
			}

			pUser->m_nItemTradeSeriel	= 0;
			pUser->m_byExTrade			= 0;
		}		
	}
}


//==========================================================================================
// 트레이드 신청한거에 대한 응답이 온다.  
//==========================================================================================
void CmdTradeResult(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSTC_TRADE_RESULT	pSendPacket;
	DSTC_SHOP_FAIL		pFailPacket;
	CTDS_TRADE_RESULT*	pPacket		= (CTDS_TRADE_RESULT*)pMsg;

	CUser*				pUser		= (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	CUser*				pDstUser	= g_pUserHash->GetData(pPacket->dwUserIndex);

	if(!pUser)
		return;

	if(!pDstUser)
		return;

	ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pUser->GetID());

	if(pItemShop)
	{
		if(pItemShop->byType == 1)
		{
			// 노점중에는 교환을 할수 없습니다.
			pFailPacket.byType	= 15;

			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION );
			NetSendToUser(pDstUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION );
			return;
		}
	}

	if(pUser->m_byRest == 1)
	{
		// "앉기중에는 교환을 할수 없습니다."
		pFailPacket.byType	= 18;

		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION );
		NetSendToUser(pDstUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION );
		return;
	}

	pItemShop = g_pItemShopHash->GetData(pDstUser->GetID());

	if(pItemShop)
	{
		if(pItemShop->byType==1)
		{
			// 노점중에는 교환을 할수 없습니다.
			pFailPacket.byType	= 15;

			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION );
			NetSendToUser(pDstUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION );
			return;
		}
	}

	if(pDstUser->m_byRest==1)
	{
		// "앉기중에는 교환을 할수 없습니다."
		pFailPacket.byType	= 18;

		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION );
		NetSendToUser(pDstUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION );
		return;
	}

	if(pPacket->byCode == 1 || pUser->m_nItemTradeSeriel != 0)
	{
		// 트레이드 거절.. 
		ITEM_TRADE* pItemTrade = g_pItemTradeHash->GetData(pDstUser->m_nItemTradeSeriel);

		if(pItemTrade)
		{
			g_pItemTradeHash->Delete(pItemTrade, pDstUser->m_nItemTradeSeriel, TRUE);
			memset(pItemTrade, 0, sizeof(ITEM_TRADE));
		}
		
		pUser->m_byExTrade				= 0;
		pDstUser->m_byExTrade			= 0;
		pUser->m_nItemTradeSeriel		= 0;
		pDstUser->m_nItemTradeSeriel	= 0;

		// 실패 //
		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);

		pSendPacket.byCode		= 1;
		pSendPacket.dwUserIndex	= pUser->GetID();		
		NetSendToUser(pDstUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
	else if(pPacket->byCode==0)
	{
		// 트레이드 신청 승락.. 
		ITEM_TRADE* pItemTrade = (ITEM_TRADE*)LALAlloc(g_pItemTradePool);
		memset(pItemTrade, 0, sizeof(ITEM_TRADE));
		
		if( pUser->GetID() > pDstUser->GetID() )
		{
			pItemTrade->TradeSerialStruct.dwHiUserIndex	= pUser->GetID();
			pItemTrade->TradeSerialStruct.dwLowUserIndex	= pDstUser->GetID();
		}
		else
		{
			pItemTrade->TradeSerialStruct.dwHiUserIndex	= pDstUser->GetID();
			pItemTrade->TradeSerialStruct.dwLowUserIndex	= pUser->GetID();
		}

		pUser->m_nItemTradeSeriel		= pItemTrade->nTradeSerial;
		pDstUser->m_nItemTradeSeriel	= pItemTrade->nTradeSerial;

		pItemTrade->byHiUserRt			= 0;
		pItemTrade->byLowUserRt			= 0;
		pItemTrade->dwHiMoney			= 0;
		pItemTrade->dwLowMoney			= 0;

		g_pItemTradeHash->Insert(pItemTrade, pItemTrade->nTradeSerial);		

		// 결과를 보낸다 //
		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pDstUser->m_szName, MAX_CHARACTER_REAL_LENGTH);

		pSendPacket.byCode		= 0;
		pSendPacket.dwUserIndex	= pDstUser->GetID();		
		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);

		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);

		pSendPacket.byCode		= 0;
		pSendPacket.dwUserIndex	= pUser->GetID();		
		NetSendToUser(pDstUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);		
	}
}


//==========================================================================================
// 트레이드 신청한다.  
//==========================================================================================
void CmdTradeRequest(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSTC_TRADE_REQUEST	pSendPacket;
	CTDS_TRADE_REQUEST* pPacket		= (CTDS_TRADE_REQUEST*)pMsg;
	
	CUser*				pUser		= (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);	
	CUser*				pSendUser	= g_pUserHash->GetData(pPacket->dwUserIndex);

	if(!pUser)
		return;
	
	if(pUser->m_sMatchDescInfo.m_bMatching)
		return;

	if(pSendUser)
	{
		if(pSendUser->m_sMatchDescInfo.m_bMatching)
		{
			return;
		}
	}

	memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
	
	if(pPacket->byCode == 0)
	{		
		if(!pSendUser)
		{
			// 트레이드 실패(로그아웃) 
			pUser->m_byExTrade		= 0;
			pSendPacket.byCode		= 1;
			pSendPacket.dwUserIndex	= pPacket->dwUserIndex;
			memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
			__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_REAL_LENGTH);

			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
		
		if(pUser->m_byExTrade == 1)
		{
			// 트레이드 신청을 이미 했다.
			pUser->m_byExTrade		= 0;
			pSendUser->m_byExTrade	= 0;
			
			pSendPacket.byCode		= 3;

			pSendPacket.dwUserIndex	= pPacket->dwUserIndex;
			memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
			__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_REAL_LENGTH);
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		if(pSendUser->m_byExTrade == 1)
		{
			// 트레이드 신청을 이미 받았다.
			pUser->m_byExTrade		= 0;
			pSendUser->m_byExTrade	= 0;			

			pSendPacket.byCode		= 2;

			pSendPacket.dwUserIndex	= pPacket->dwUserIndex;
			memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
			__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_REAL_LENGTH);
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		pUser->m_byExTrade		= 1;
		pSendUser->m_byExTrade	= 1;

		pSendPacket.byCode		= 0;
		pSendPacket.dwUserIndex	= pUser->GetID();
		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);

		NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
	}
	else if(pPacket->byCode == 1 || pSendUser->m_nItemTradeSeriel != 0)
	{
		if(!pSendUser)
			return;
		
		if(pSendUser->m_nItemTradeSeriel != 0)
		{
			ITEM_TRADE* pItemTrade = g_pItemTradeHash->GetData(pSendUser->m_nItemTradeSeriel);

			if(pItemTrade)
			{
				g_pItemTradeHash->Delete(pItemTrade, pSendUser->m_nItemTradeSeriel, TRUE);
				memset(pItemTrade, 0, sizeof(ITEM_TRADE));
			}

			pUser->m_nItemTradeSeriel		= 0;
			pSendUser->m_nItemTradeSeriel	= 0;
			pUser->m_byExTrade				= 0;
			pSendUser->m_byExTrade			= 0;
		}
		
		pSendPacket.byCode		= 2;

		pSendPacket.dwUserIndex	= pUser->GetID();
		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);

		NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(),
			FLAG_SEND_NOT_ENCRYPTION);

		return;
	}	
}