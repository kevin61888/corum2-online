#if !defined(AFX_ITEMMOVEMANAGER_H__D5EB40E9_E967_458B_AF92_AA6DF747AB20__INCLUDED_)
#define AFX_ITEMMOVEMANAGER_H__D5EB40E9_E967_458B_AF92_AA6DF747AB20__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "ItemMove.h"
#include "../CommonServer/CommonHeader.h"


class CUnit;
class CUser;
class CDungeonLayer;


#define MAX_SRC		123
#define MAX_DEST	123


enum
{
	GET_SRC_NATIVEPOINTER		= 1,
	GET_DEST_NATIVEPOINTER		= 2,
};


enum
{
	GET_NOT_AVAILABILITY		= 1,
	GET_CANNOT_NATIVEPOINTER,
	GET_CANNOT_NATIVELINKPOINTER,
	GET_CANNOT_BASEITEMPOINTER,
	GET_CANNOT_USERPOINTER,
	UNSALABLE_ITEM, 
	BE_NOT_ENOUGH_MONEY,
	BE_FULL_OF_MONEY,
	CREATE_ITEM_FAIL,
	QUITE_FULL_SLOT,
	MAKE_A_MISTAKE_WAY, 
	MAKE_A_MISTAKE_ITEM_KIND,
	MAKE_A_MISTAKE_SLOTINDEX,
	MAKE_A_MISTAKE_ITEMQUANTITY,
	MAKE_A_MISTAKE_MONEY,
	MOVE_ITEMNATIVE_FAIL,
	SET_SLOT_ITEMNATIVE_FAIL,
	REMOVE_ITEMNATIVE_FAIL,
	THIS_SLOT_ALREADY_ITEM, 
	EXCHANGE_ITEMNATIVE_FAIL, 
	PUTON_ITEMNATIVE_FAIL,
	THIS_SLOT_ALREADY_ITEM_LINK,
	DROP_ITEM_FAIL,
	PICKUP_ITEM_FAIL,
	MONEY_MOVE_FAIL,
	INV_TO_LINK_FAIL,
	LINK_TO_LINK_FAIL,
	LINK_TO_INV_FAIL,
	WORKING_LINK,
	ALREADY_DIFFERENT_WINDOW_LINK, 
	ANOTHER_PLAYERSHOP_CLOSE,
	ANOTHER_PLAYERSHOP_SELLITEM_CHANGE,
	ANOTHER_PLAYERSHOP_SELLITEM_CUSTOMSELLPRICE_CHANGE,
	ANOTHER_PLAYERSHOP_BUYITEM_FAIL,
	THIS_ITEM_IS_OCCUPIABLE,
};


class CItemMoveManager  
{

public:

	CItemMoveManager								();
	virtual			~CItemMoveManager				();
	
	void			Process							(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);	
	void			ItemMoveLogProcess				(CItem* pItem, BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller = NULL);
	void			KarzLogProcess					(BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller,DWORD dwMoney,CItem* pItem = NULL);

private:

	BOOL			IsAvailability					(CUser* pUser,CDungeonLayer** pLayer,CTDS_ITEM_MOVE* const pItemMove);
	BOOL			IsDestNativeToCorrectType		(CUser* const pUser,ITEM_NATIVE eItemNativeDest,BYTE bySlotIndex,const CItem* const pItemNativeSrc);
	BOOL			IsSrcToDestCorrect				(ITEM_NATIVE eItemNativeSrc,ITEM_NATIVE eItemNativeDest);
	BOOL			IsNativeToCorrectSlotIndex		(ITEM_NATIVE eItemNative,BYTE bySlotIndex);
	const CItem* 	GetItemNativePointer			(CUnit* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,ITEM_NATIVE_INFO* pItemNativeInfo,BYTE bIsSrcDest); 
	const CItem*	GetItemNativeLinkPointer		(CUnit* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,ITEM_NATIVE_INFO* pItemNativeInfo,BYTE bIsSrcDest);
	void			InitItemMoveProc				();
	void			InitItemMoveResultPacket		(CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);
	void			SendToItemMoveResult			(DWORD dwConnectionIndex,DSTC_ITEM_MOVE* const pItemMoveResult,BYTE bErrorCode);
	void			LinkSlotChangeInvToLinkProcess	(DWORD dwConnectionIndex,CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);
	void			LinkSlotChangeLinkToLinkProcess	(DWORD dwConnectionIndex,CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);
	void			LinkSlotChangeLinkToInvProcess	(DWORD dwConnectionIndex,CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);
	void			DefaultSlotChangeProcess		(DWORD dwConnectionIndex,CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);
	void			AnotherPlayerShopUserBuyItem	(DWORD dwConnectionIndex,CUser* const pBuyer,CUser* const pSeller,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);
	BOOL			TypeLargeMoveItemNative			(CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult,const CItem* pItemSrc,const CItem* pItemDest,ITEM_NATIVE_INFO* ItemNativeInfo_Src,ITEM_NATIVE_INFO* ItemNativeInfo_Dest,BYTE byQuantity,BYTE* byErrorCode);
	BOOL			TypeSmallMoveItemNative			(CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult,const CItem* pItemSrc,const CItem* pItemDest,ITEM_NATIVE_INFO* ItemNativeInfo_Src,ITEM_NATIVE_INFO* ItemNativeInfo_Dest,BYTE byQuantity,BYTE* byErrorCode);
	BOOL			TraderMonsterInvSmallAndInvSmall(CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult,const CItem* pItemSrc,const CItem* pItemDest,ITEM_NATIVE_INFO* ItemNativeInfo_Src,ITEM_NATIVE_INFO* ItemNativeInfo_Dest,BYTE byQuantity,BYTE* byErrorCode);
	BOOL			TraderMonsterInvLargeAndInvLarge(CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult,const CItem* pItemSrc,const CItem* pItemDest,ITEM_NATIVE_INFO* ItemNativeInfo_Src,ITEM_NATIVE_INFO* ItemNativeInfo_Dest,BYTE byQuantity,BYTE* byErrorCode);
	BOOL			IsSameNative					(ITEM_NATIVE eSlotIDSrc,ITEM_NATIVE eSlotIDDest);
	void			PutOnOffItemNativeResult		(CUser* const pUser,CDungeonLayer* const pLayer,const CItem* pItemSrc);
	void			DropAndPickupItemProcess		(DWORD dwConnectionIndex,CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);
	void			KarzMoveProcess					(DWORD dwConnectionIndex,CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);
	void			(CItemMoveManager::*ItemMoveProc[MAX_SRC][MAX_DEST])	(DWORD dwConnectionIndex,CUser* const pUser,CDungeonLayer* const pLayer,CTDS_ITEM_MOVE* const pItemMove,DSTC_ITEM_MOVE* const pItemMoveResult);

private:

	BYTE			m_bSlotPool[ITEM_NATIVE_PLAYER_END-ITEM_NATIVE_PLAYER_START];

	
};


#endif 
