#if !defined(AFX_ITEMMOVEMANAGER_H__188803AB_8DE9_49DE_B51B_8E7A09E0878B__INCLUDED_)
#define AFX_ITEMMOVEMANAGER_H__188803AB_8DE9_49DE_B51B_8E7A09E0878B__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "ItemMove.h"
#include "DungeonMsg_jjw6263.h"


class CItem;
struct DSTC_ITEM_MOVE;


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

	CItemMoveManager							();
	virtual ~CItemMoveManager					();

	void			SetLButtonDownItem			(BYTE bSrc = 0xff,BYTE bSrcIndex = 0xff,WORD wMouseItemID = 0,WORD wStoreItemID = 0,DWORD dwUnitID = 0);
	void			SetLButtonUpItem			(BYTE bDest = 0xff,BYTE bDestIndex = 0xff,BYTE bSectionNum = 1,BYTE bQuantity = 1,DWORD dwMoney = 0,WORD wPlayerShopItemID = 0);
	void			Process						(char* pMsg,DWORD dwLen);
	WORD			GetMouseItemID				();
	BOOL			IsItemPick					();
	void			Initialize					();
	BYTE			GetNativeSrc				();
	BYTE			GetNativeSrcIndex			();

	void			SetNewItemMoveMode			(BOOL bNewItemMoveMode);
	BOOL			GetNewItemMoveMode			();

	BOOL			IsSrcToDestCorrect			(ITEM_NATIVE eItemNativeSrc,ITEM_NATIVE eItemNativeDest);
	BOOL			IsDestNativeToCorrectType	(ITEM_NATIVE eItemNativeDest,BYTE bySlotIndex,const CItem* const pItemNativeSrc);
	void			TileToInvPickupItem			(ITEM* pItem);

private:

	BYTE			m_bSrc;
	BYTE			m_bSrcIndex;
	BYTE			m_bDest;
	BYTE			m_bDestIndex;
	WORD			m_wStoreItemID;	
	WORD			m_wMouseItemID;
	BYTE			m_bSectionNum;	
	BYTE			m_bQuantity;
	DWORD			m_dwUnitID;
	BYTE			m_bSlotPool[ITEM_NATIVE_PLAYER_END-ITEM_NATIVE_PLAYER_START];
	
	BOOL			m_bNewItemMoveMode;
	
private:

	void			(CItemMoveManager::*ItemMoveProc[MAX_SRC][MAX_DEST])	(DSTC_ITEM_MOVE* pItemMove);
	void			KarzMoveProcess				(DSTC_ITEM_MOVE* pItemMove);
	void			DefaultSlotChangeProcess	(DSTC_ITEM_MOVE* pItemMove);
	void			AnotherPlayerShopUserBuyItem(DSTC_ITEM_MOVE* pItemMove);
	void			DropAndPickupItemProcess	(DSTC_ITEM_MOVE* pItemMove);
	const CItem*	GetItemNativePointer		(DSTC_ITEM_MOVE* const pItemMove,ITEM_NATIVE_INFO* pItemNativeInfo,BYTE bIsSrcDest); 
	BOOL			IsSameNative				(ITEM_NATIVE eSlotIDSrc,ITEM_NATIVE eSlotIDDest);
	BOOL			TypeLargeMoveItemNative		(DSTC_ITEM_MOVE* const pItemMove,const CItem* pItemSrc,const CItem* pItemDest,ITEM_NATIVE_INFO* ItemNativeInfo_Src,ITEM_NATIVE_INFO* ItemNativeInfo_Dest,BYTE byQuantity);
	BOOL			TypeSmallMoveItemNative		(DSTC_ITEM_MOVE* const pItemMove,const CItem* pItemSrc,const CItem* pItemDest,ITEM_NATIVE_INFO* ItemNativeInfo_Src,ITEM_NATIVE_INFO* ItemNativeInfo_Dest,BYTE byQuantity);
	void			AnotherPlayerShopUserBuyItemOverlapItem(DSTC_ITEM_MOVE* pItemMove,const CItem* pItemSrc,const CItem* pItemDest,ITEM_NATIVE_INFO* ItemNativeInfo_Dest,BYTE byQuantity);
	void			InitItemMoveProc			();
	BOOL			IsInitialize				();
	void			Hyena						();
	void			DisplayErrorCode			(DSTC_ITEM_MOVE* pItemMove);
};


#endif 
