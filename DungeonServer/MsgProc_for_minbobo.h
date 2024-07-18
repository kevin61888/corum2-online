#pragma once

#include "StdAfx.h"


class CUser;
class ITEM_TRADE;


struct SDBTRADE
{
	char	szName1[MAX_CHARACTER_NAME_LENGTH];

	DWORD	dwMoeny1;
	CItem	cLargeItem1[MAX_INV_LARGE_POOL];
	CItem	cSmallItem1[MAX_INV_SMALL_POOL];
	
	char	szName2[MAX_CHARACTER_NAME_LENGTH];

	DWORD	dwMoeny2;
	CItem	cLargeItem2[MAX_INV_LARGE_POOL];
	CItem	cSmallItem2[MAX_INV_SMALL_POOL];
};


void InitializePacketProcMinbobo();


void CmdGuildInfo( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuildItem( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuildUserData( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuildJoinInfo( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdTradeRequest( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdTradeResult( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdTradeCancel( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdTradeOk( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdTradeTrade( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPartyPortal( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPlayerShopOpen( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPlayerShopClose( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPartyPortalUsed( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPlayerShopShowAll( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPartyPortalFail( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPlayerRest( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuildPortalUsed( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdWorldMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPartyPoralRecognition( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPartyLeader( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );


void RemoveTradeHash(CUser* pUser, CUser* pSendUser);
void AloneTradeCancel(CUser* pUser,ITEM_TRADE* pItemTrade);
void BothTradeCancel(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade, BYTE byUserReturnType, BYTE bySendUserReturnType);
BOOL IsDoNotExistLinkAndKarz_DoWork(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade);
BOOL IsTradeTradeKarz_DoWork(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade);
BOOL IsTradeTradeItemNativeLinkToItemNativeInv_DoWork(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade);
void TradeItemNative(CUser* pSrcUser, CUser* pDestUser, ITEM_TRADE* pItemTrade);
void TradeTrade(CUser* pUser, CUser* pSendUser, ITEM_TRADE* pItemTrade);
