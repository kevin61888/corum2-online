#include "stdafx.h"
#include "ItemMoveManager.h"
#include "../CommonServer/CommonHeader.h"
#include "ItemMove.h"
#include "NetworkClient.h"
#include "GameControl.h"
#include "User.h"
#include "GameDefine.h"
#include "HashTable.h"
#include "InitGame.h"
#include "PlayerShopWnd.h"
#include "CDungeonManagerWnd.h"
#include "message.h"
#include "Chat.h"
#include "GameMenuWnd.h"
#include "ItemWnd.h"
#include "BankWnd.h"
#include "QuantityWnd.h"


CItemMoveManager::CItemMoveManager()
{
	memset(this,0,sizeof(CItemMoveManager));
	
	m_bSrc			= 0xff;
	m_bSrcIndex		= 0xff;
	m_bDest			= 0xff;
	m_bDestIndex	= 0xff;
	m_wStoreItemID	= 0;	
	m_wMouseItemID	= 0;
	m_bSectionNum	= 0;
	m_bQuantity		= 0;
	m_dwUnitID		= 0;

	m_bSlotPool[ITEM_NATIVE_EQUIP]			= MAX_EQUIP_POOL;
	m_bSlotPool[ITEM_NATIVE_INV_LARGE]		= MAX_INV_LARGE_POOL;
	m_bSlotPool[ITEM_NATIVE_INV_SMALL]		= MAX_INV_SMALL_POOL;
	m_bSlotPool[ITEM_NATIVE_INV_GUARDIAN]	= MAX_INV_GUARDIAN_POOL - MAX_INV_GUARDIAN_POOL;
	m_bSlotPool[ITEM_NATIVE_BELT]			= MAX_BELT_POOL;
	m_bSlotPool[ITEM_NATIVE_BANK_LARGE]		= MAX_BANK_LARGE_POOL;
	m_bSlotPool[ITEM_NATIVE_BANK_SMALL]		= MAX_BANK_SMALL_POOL;
	m_bSlotPool[ITEM_NATIVE_TRADE]			= MAX_UPGRADE_POOL;
	m_bSlotPool[ITEM_NATIVE_UPGRADE]		= MAX_UPGRADE_POOL;
	m_bSlotPool[ITEM_NATIVE_MAKING]			= MAX_MAKING_POOL;
	m_bSlotPool[ITEM_NATIVE_PLAYERSHOP]		= MAX_PLAYERSHOP_POOL;

	InitItemMoveProc();
}


CItemMoveManager::~CItemMoveManager()
{
		
}


void CItemMoveManager::InitItemMoveProc()
{
	ItemMoveProc[ITEM_NATIVE_INV_LARGE][ITEM_NATIVE_TILE]		= DropAndPickupItemProcess;
	ItemMoveProc[ITEM_NATIVE_INV_SMALL][ITEM_NATIVE_TILE]		= DropAndPickupItemProcess;

	ItemMoveProc[ITEM_NATIVE_TILE][ITEM_NATIVE_INV_LARGE]		= DropAndPickupItemProcess;
	ItemMoveProc[ITEM_NATIVE_TILE][ITEM_NATIVE_INV_SMALL]		= DropAndPickupItemProcess;
	ItemMoveProc[ITEM_NATIVE_TILE][ITEM_NATIVE_BELT]			= DropAndPickupItemProcess;

	ItemMoveProc[ITEM_NATIVE_INV_KARZ][ITEM_NATIVE_TILE]		= DropAndPickupItemProcess;
	
	ItemMoveProc[ITEM_NATIVE_INV_KARZ][ITEM_NATIVE_BANK_KARZ]	= KarzMoveProcess;
	ItemMoveProc[ITEM_NATIVE_BANK_KARZ][ITEM_NATIVE_INV_KARZ]	= KarzMoveProcess;
}


void CItemMoveManager::SetNewItemMoveMode(BOOL bNewItemMoveMode)
{
	m_bNewItemMoveMode = bNewItemMoveMode;
}


BOOL CItemMoveManager::GetNewItemMoveMode()
{
	return m_bNewItemMoveMode;
}


void CItemMoveManager::Initialize()
{
	m_bSrc			= 0xff;
	m_bSrcIndex		= 0xff;
	m_bDest			= 0xff;
	m_bDestIndex	= 0xff;
	m_wStoreItemID	= 0;	
	m_wMouseItemID	= 0;
	m_bSectionNum	= 0;
	m_bQuantity		= 0;
	m_dwUnitID		= 0;
}


BOOL CItemMoveManager::IsInitialize()
{
	if(	m_bSrc			== 0xff &&
		m_bSrcIndex		== 0xff &&
		m_bDest			== 0xff &&
		m_bDestIndex	== 0xff &&
		m_wStoreItemID	== 0	&&
		m_wMouseItemID	== 0	&&
		m_bSectionNum	== 0	&&
		m_bQuantity		== 0	&&
		m_dwUnitID		== 0		)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


void CItemMoveManager::SetLButtonDownItem(BYTE bSrc
										  , BYTE bSrcIndex
										  , WORD wMouseItemID
										  , WORD wStoreItemID
										  , DWORD dwUnitID)
{
	if( !IsInitialize() )
		return;

	m_bSrc			= bSrc;
	m_bSrcIndex		= bSrcIndex;
	m_wMouseItemID	= wMouseItemID;
	m_wStoreItemID	= wStoreItemID;
	m_dwUnitID		= dwUnitID;
	
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ITEMCLICK, g_v3InterfaceSoundPos, FALSE);
	
	if(bSrc == ITEM_NATIVE_INV_KARZ || bSrc == ITEM_NATIVE_BANK_KARZ)
	{

	}
	else
	{
		CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
		pQuantityWnd->SetActive(FALSE);
	}	
}


void CItemMoveManager::SetLButtonUpItem(BYTE bDest
										, BYTE bDestIndex
										, BYTE bSectionNum
										, BYTE bQuantity
										, DWORD dwMoney
										, WORD wPlayerShopItemID)
{
	CTDS_ITEM_MOVE packet;
	
	m_bDest						= bDest;
	m_bDestIndex				= bDestIndex;
	m_bSectionNum				= bSectionNum;
	m_bQuantity					= bQuantity;

	packet.bSrc					= m_bSrc;
	packet.bDest				= m_bDest;
	packet.bSrcIndex			= m_bSrcIndex;
	packet.bDestIndex			= m_bDestIndex;
	packet.bSectionNum			= m_bSectionNum;
	packet.bQuantity			= m_bQuantity;
	packet.dwUnitID				= m_dwUnitID;
	packet.dwMoney				= dwMoney;
	packet.wPlayerShopItemID	= wPlayerShopItemID;

	g_pNet->SendMsg((char*)&packet,packet.GetPacketSize(),SERVER_INDEX_ZONE);
}


void CItemMoveManager::TileToInvPickupItem(ITEM* pItem)
{
	CTDS_ITEM_MOVE packet;

	packet.bSrc			= ITEM_NATIVE_TILE;
	packet.bSrcIndex	= 0;
	packet.bDest		= ITEM_NATIVE_INV_LARGE;
	packet.bDestIndex	= 0;
	packet.i64ItemID	= pItem->Item.m_Serial.i64Serial;
	packet.bSectionNum	= pItem->bSectionNum;
	packet.bQuantity	= 1;

	g_pNet->SendMsg((char*)&packet,packet.GetPacketSize(),SERVER_INDEX_ZONE);
}


BYTE CItemMoveManager::GetNativeSrc()
{
	return m_bSrc;
}


BYTE CItemMoveManager::GetNativeSrcIndex()
{
	return m_bSrcIndex;
}


WORD CItemMoveManager::GetMouseItemID()
{
//	LP_ITEM_TABLE pBaseItem = g_pItemTableHash->GetData(m_wMouseItemID);
	CBaseItem* pBaseItem = g_pItemTableHash->GetData(m_wMouseItemID);

	if(pBaseItem)
		return m_wMouseItemID;
	else
		return 0;
}


BOOL CItemMoveManager::IsItemPick()
{
	if(	IsItemNativePlayer((ITEM_NATIVE)m_bSrc)			||
		IsItemNativeTraderMonster((ITEM_NATIVE)m_bSrc)	||
		IsItemNativeTile((ITEM_NATIVE)m_bSrc)			||
		IsItemDungeonManager((ITEM_NATIVE)m_bSrc)		||
		GetMouseItemID() )
	{
		return TRUE;		
	}
	else
	{
		return FALSE;
	}
}


void CItemMoveManager::DisplayErrorCode(DSTC_ITEM_MOVE* pItemMove)
{
	switch(pItemMove->bErrorCode)
	{
		case ALREADY_DIFFERENT_WINDOW_LINK:
		case WORKING_LINK:
		{
			switch(pItemMove->bSrc)
			{
			case ITEM_NATIVE_PLAYERSHOP:	DisplayMessageAdd("노점 이용 중이어서 이동할 수 없습니다.",0xFFFF0000);		break;
			case ITEM_NATIVE_TRADE:			DisplayMessageAdd("트레이드 이용 중이어서 이동할 수 없습니다.",0xFFFF0000);	break;
			case ITEM_NATIVE_UPGRADE:		DisplayMessageAdd("업그레이드 이용 중이어서 이동할 수 없습니다.",0xFFFF0000);	break;
			case ITEM_NATIVE_MAKING:		DisplayMessageAdd("제조 이용 중이어서 이동할 수 없습니다.",0xFFFF0000);		break;
			}
		}
		break;	
		
		case MAKE_A_MISTAKE_ITEMQUANTITY:
		case MAKE_A_MISTAKE_ITEM_KIND:
		case GET_CANNOT_BASEITEMPOINTER:
		case GET_CANNOT_NATIVEPOINTER:
		case MAKE_A_MISTAKE_WAY:	
		case MAKE_A_MISTAKE_SLOTINDEX:		DisplayMessageAdd("아이템을 이동할 수 없습니다.",0xFFFF0000);				break;
		case PUTON_ITEMNATIVE_FAIL:			DisplayMessageAdd(g_Message[ETC_MESSAGE510].szMessage,0xFFFF0000);			break;
		case BE_NOT_ENOUGH_MONEY:			DisplayMessageAdd("Kartz가 부족합니다.",0xFFFF0000);						break;
		case QUITE_FULL_SLOT:				DisplayMessageAdd(g_Message[ETC_MESSAGE90].szMessage,0xFFFF0000);			break;
		case BE_FULL_OF_MONEY:				DisplayMessageAdd("Kartz가 초과하여 더 이상 팔 수 없습니다.",0xFFFF0000);		break;		
	}
}


void CItemMoveManager::Hyena()
{
	CItemWnd*		pItemWnd		= CItemWnd::GetInstance();
	CBankWnd*		pBankWnd		= CBankWnd::GetInstance();
	CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();

	if(pItemWnd->GetActive())
	{
		if(pItemWnd->m_byItemChk==0)
		{		
			pItemWnd->SetRender(SPR_OBJ_ITEM_CHK1, FALSE);
			pItemWnd->SetRender(SPR_OBJ_ITEM_CHK2, TRUE);
		}		
	}
	
	pItemWnd->SetLine(FALSE, 0);

	if(pItemWnd->GetActive()==TRUE)
	{
		pItemWnd->SetLine(TRUE, pItemWnd->m_byItemChk);	
	}
	
	if(pBankWnd->GetActive()==TRUE)	
	{	
		pBankWnd->SetRender(SPR_OBJ_BANK_CHK1, FALSE);
		pBankWnd->SetRender(SPR_OBJ_BANK_CHK2, FALSE);
		
		if(pBankWnd->m_byBankChk==0)
			pBankWnd->SetRender(SPR_OBJ_BANK_CHK1, TRUE);
		else
			pBankWnd->SetRender(SPR_OBJ_BANK_CHK2, TRUE);
	}	

	if(pQuantityWnd->GetActive()==FALSE)
	{
		pQuantityWnd->m_bReq = FALSE;
	}

	WORD wWeight = g_pMainPlayer->GetSumWeight();
	g_pMainPlayer->WeightProcess(TRUE, wWeight);	
	g_pMainPlayer->GetCheckUpgrade();
}


void CItemMoveManager::Process(char* pMsg,DWORD dwLen)
{
	DSTC_ITEM_MOVE* pItemMove = (DSTC_ITEM_MOVE*)pMsg;

	if(pItemMove->bErrorCode < 0xff)
	{
		// 에러코드 처리 
		DisplayErrorCode(pItemMove);
		return;
	}

	BYTE bSrc = pItemMove->bSrc;
	BYTE bDest = pItemMove->bDest;

	if(ItemMoveProc[bSrc][bDest])
	{
		(this->*ItemMoveProc[bSrc][bDest])(pItemMove);		
	}
	else
	{
		DefaultSlotChangeProcess(pItemMove);			
	}	

	Hyena();
}


BOOL CItemMoveManager::IsSrcToDestCorrect(ITEM_NATIVE eItemNativeSrc,ITEM_NATIVE eItemNativeDest)
{
	switch(eItemNativeSrc)
	{
	case ITEM_NATIVE_EQUIP:
		if( eItemNativeDest == ITEM_NATIVE_EQUIP ||	eItemNativeDest == ITEM_NATIVE_INV_LARGE )
			return TRUE;
		break;
	case ITEM_NATIVE_INV_LARGE:
		if(	eItemNativeDest == ITEM_NATIVE_INV_LARGE	||	eItemNativeDest == ITEM_NATIVE_EQUIP		||
			eItemNativeDest == ITEM_NATIVE_INV_GUARDIAN ||	eItemNativeDest == ITEM_NATIVE_BANK_LARGE	||
			eItemNativeDest == ITEM_NATIVE_TRADE		||	eItemNativeDest == ITEM_NATIVE_UPGRADE		||
			eItemNativeDest == ITEM_NATIVE_PLAYERSHOP	||  eItemNativeDest == ITEM_NATIVE_TRADERMONSTER_INV_LARGE ||
			eItemNativeDest == ITEM_NATIVE_TILE			||	eItemNativeDest == ITEM_NATIVE_DUNGEONMANAGER)
		{
			return TRUE;			
		}
		break;
	case ITEM_NATIVE_INV_SMALL:
		if( eItemNativeDest == ITEM_NATIVE_INV_SMALL	||	eItemNativeDest == ITEM_NATIVE_BELT			||
			eItemNativeDest == ITEM_NATIVE_BANK_SMALL	||	eItemNativeDest == ITEM_NATIVE_TRADE		||
			eItemNativeDest == ITEM_NATIVE_UPGRADE		||	eItemNativeDest == ITEM_NATIVE_MAKING		||
			eItemNativeDest == ITEM_NATIVE_PLAYERSHOP   ||	eItemNativeDest == ITEM_NATIVE_TRADERMONSTER_INV_SMALL ||
			eItemNativeDest == ITEM_NATIVE_TILE			||  eItemNativeDest == ITEM_NATIVE_GUARDIAN		||
			eItemNativeDest == ITEM_NATIVE_DUNGEONMANAGER)
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_INV_GUARDIAN:
		if(	eItemNativeDest == ITEM_NATIVE_INV_GUARDIAN	||	eItemNativeDest == ITEM_NATIVE_INV_LARGE )
			return TRUE;
		break;
	case ITEM_NATIVE_BELT:
		if( eItemNativeDest == ITEM_NATIVE_BELT			||	eItemNativeDest == ITEM_NATIVE_INV_SMALL )
			return TRUE;
		break;
	case ITEM_NATIVE_BANK_LARGE:
		if( eItemNativeDest == ITEM_NATIVE_BANK_LARGE	||	eItemNativeDest == ITEM_NATIVE_INV_LARGE )
			return TRUE;
		break;
	case ITEM_NATIVE_BANK_SMALL:
		if( eItemNativeDest == ITEM_NATIVE_BANK_SMALL	||	eItemNativeDest == ITEM_NATIVE_INV_SMALL )
			return TRUE;
		break;
	case ITEM_NATIVE_TRADE:
		if( eItemNativeDest == ITEM_NATIVE_TRADE		||
			eItemNativeDest == ITEM_NATIVE_INV_LARGE	||
			eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_UPGRADE:
		if( eItemNativeDest == ITEM_NATIVE_UPGRADE		||
			eItemNativeDest == ITEM_NATIVE_INV_LARGE	||
			eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_MAKING:
		if( eItemNativeDest == ITEM_NATIVE_MAKING		||
			eItemNativeDest == ITEM_NATIVE_INV_LARGE	||
			eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_PLAYERSHOP:
		if( eItemNativeDest == ITEM_NATIVE_PLAYERSHOP	||
			eItemNativeDest == ITEM_NATIVE_INV_LARGE	||
			eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_DUNGEONMANAGER:
		if( eItemNativeDest == ITEM_NATIVE_INV_LARGE	||	eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_TRADERMONSTER_INV_LARGE:
		if( eItemNativeDest == ITEM_NATIVE_INV_LARGE )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_TRADERMONSTER_INV_SMALL:
		if( eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_TILE:
		if( eItemNativeDest == ITEM_NATIVE_INV_LARGE	||	eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_INV_KARZ:
		if( eItemNativeDest == ITEM_NATIVE_TILE			||	eItemNativeDest == ITEM_NATIVE_BANK_KARZ )
		{
			return TRUE;
		}
		break;
	case ITEM_NATIVE_GUARDIAN:
		if( eItemNativeDest == ITEM_NATIVE_GUARDIAN		||  eItemNativeDest == ITEM_NATIVE_INV_SMALL )
		{
			return TRUE;
		}
	default:
		return FALSE;
	}

	Initialize();

	CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
	pQuantityWnd->m_bReq = FALSE;

	DisplayMessageAdd("아이템을 이동할 수 없습니다",0xFFFF0000);

	return FALSE;
}


BOOL CItemMoveManager::IsDestNativeToCorrectType(ITEM_NATIVE eItemNativeDest,BYTE bySlotIndex,
												 const CItem* const pItemNativeSrc)
{
//	LP_ITEM_TABLE pItem = g_pItemTableHash->GetData(pItemNativeSrc->GetID());
	CBaseItem* pItem = g_pItemTableHash->GetData(pItemNativeSrc->GetID());

	if(NULL == pItem)
	{
		return FALSE;
	}

	ITEM_TYPE eType = GetItemType(pItemNativeSrc->GetID());
	WORD wKind = GetItemKind(pItemNativeSrc->GetID());
	BYTE byWeaponKind = pItem->GetWeaponKind();	
	
	switch(eItemNativeDest)
	{
	case ITEM_NATIVE_EQUIP:
		if(	eType == ITEM_TYPE_LARGE &&
			(wKind == ITEM_KIND_WEAPON || wKind == ITEM_KIND_AROMR || wKind == ITEM_KIND_RIDE))
		{
			switch(bySlotIndex)
			{
			case EQUIP_TYPE_RHAND1:
			case EQUIP_TYPE_RHAND2:
				if(byWeaponKind >= ITEM_CLASS_SWARD || byWeaponKind <= ITEM_CLASS_STAFF)
					return TRUE;
				break;
			case EQUIP_TYPE_LHAND1:
			case EQUIP_TYPE_LHAND2:	
				if(byWeaponKind == ITEM_CLASS_SHIELD)
					return TRUE;
				else if((byWeaponKind >= ITEM_CLASS_KATAL && byWeaponKind <= ITEM_CLASS_SPEAR) || byWeaponKind == ITEM_CLASS_STAFF)
					return TRUE;
				break;
			case EQUIP_TYPE_RRING1:
			case EQUIP_TYPE_RRING2:
			case EQUIP_TYPE_RRING3:
				if(byWeaponKind == ITEM_CLASS_RING)
					return TRUE;
				break;
			case EQUIP_TYPE_LRING1:
			case EQUIP_TYPE_LRING2:
			case EQUIP_TYPE_LRING3:
				break;
			case EQUIP_TYPE_BOOTS:
				if(byWeaponKind == ITEM_CLASS_BOOTS)
					return TRUE;
				break;
			case EQUIP_TYPE_AMULE1:	
				break;
			case EQUIP_TYPE_AMULE2:
				if(byWeaponKind == ITEM_CLASS_AMULET)
					return TRUE;
				break;
			case EQUIP_TYPE_AMULE3:
				break;
			case EQUIP_TYPE_BELT:
				if(byWeaponKind == ITEM_CLASS_BELT)
					return TRUE;
				break;
			case EQUIP_TYPE_ARMOR:
				if(byWeaponKind == ITEM_CLASS_ARMOR)
					return TRUE;
				break;
			case EQUIP_TYPE_HELMET:
				if(byWeaponKind == ITEM_CLASS_HELMET)
					return TRUE;	
				break;
			case EQUIP_TYPE_RIDE:
				if(wKind == ITEM_KIND_RIDE)
					return TRUE;
				break;
			case EQUIP_TYPE_BAG:				
				break;			
			case EQUIP_TYPE_RGLOVE:
			case EQUIP_TYPE_LGLOVE:
				if(byWeaponKind == ITEM_CLASS_GLOVE)
					return TRUE;
				break;
			}
		}
		break;
	case ITEM_NATIVE_INV_LARGE:	
		if(eType == ITEM_TYPE_LARGE)
			return TRUE;
		break;
	case ITEM_NATIVE_INV_SMALL:	
		if(eType == ITEM_TYPE_SMALL)
			return TRUE;
		break;
	case ITEM_NATIVE_BANK_LARGE:
		if(eType == ITEM_TYPE_LARGE)
			return TRUE;
		break;
	case ITEM_NATIVE_BANK_SMALL:
		if(eType == ITEM_TYPE_SMALL)
			return TRUE;
		break;
	case ITEM_NATIVE_INV_GUARDIAN:
		if(eType == ITEM_TYPE_LARGE && wKind == ITEM_KIND_GUARDIAN)
			return TRUE;
		break;
	case ITEM_NATIVE_BELT:
		if(eType == ITEM_TYPE_SMALL && (wKind == ITEM_KIND_SUPPLIES || wKind == ITEM_KIND_ZODIAC))
			return TRUE;
		break;
	case ITEM_NATIVE_TRADE:
		if(eType == ITEM_TYPE_LARGE || eType == ITEM_TYPE_SMALL)
			return TRUE;
		break;
	case ITEM_NATIVE_UPGRADE:
		if(bySlotIndex == ITEM_UPGRADE_WEAPON) 
		{
			if(eType == ITEM_TYPE_LARGE &&	(wKind == ITEM_KIND_WEAPON || wKind == ITEM_KIND_AROMR))
			{
				return TRUE;
			}
		}
		else if(bySlotIndex == ITEM_UPGRADE_UPGRADE ||
				bySlotIndex == ITEM_UPGRADE_LIQUID	||
				bySlotIndex == ITEM_UPGRDAE_EDITION)
		{
			if(	eType == ITEM_TYPE_SMALL &&
				(wKind == ITEM_KIND_UPGRADE || wKind == ITEM_KIND_LIQUID || wKind == ITEM_KIND_EDITION))
			{
				return TRUE;
			}
		}
		break;
	case ITEM_NATIVE_MAKING:
		if(bySlotIndex == ITEM_MAKING_RESULT)
		{
			if(eType == ITEM_TYPE_LARGE &&
				(wKind == ITEM_KIND_WEAPON || wKind == ITEM_KIND_RIDE || wKind == ITEM_KIND_GUARDIAN))
			{
				return TRUE;
			}
		}
		else if(bySlotIndex == ITEM_MAKING_MATERIALS1 ||
				bySlotIndex == ITEM_MAKING_MATERIALS2 ||
				bySlotIndex == ITEM_MAKING_ZODIAC)
		{
			if(eType == ITEM_TYPE_SMALL)
			{
				if(wKind == ITEM_KIND_MATERIALS)
					return TRUE;
				else if(wKind == ITEM_KIND_ZODIAC && pItem->GetID() == __ITEM_MAKING_INDEX__)
					return TRUE;
			}
		}
		break;
	case ITEM_NATIVE_PLAYERSHOP:
		if(eType == ITEM_TYPE_LARGE || eType == ITEM_TYPE_SMALL)	
			return TRUE;
		break;
	case ITEM_NATIVE_DUNGEONMANAGER:
		break;
	}

	return FALSE;
}


BOOL CItemMoveManager::IsSameNative(ITEM_NATIVE eSlotIDSrc,ITEM_NATIVE eSlotIDDest)
{
	if(eSlotIDSrc == eSlotIDDest)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


const CItem* CItemMoveManager::GetItemNativePointer(DSTC_ITEM_MOVE* const pItemMove,
													ITEM_NATIVE_INFO* pItemNativeInfo,
													BYTE bIsSrcDest) 
{
	const CItem* pItemNative = NULL;
	
	if(bIsSrcDest == GET_SRC_NATIVEPOINTER)
	{
		pItemNativeInfo->eSlotID = (ITEM_NATIVE)pItemMove->bSrc;

		if(::IsItemNativePlayer(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->dwOwnerID		= g_pMainPlayer->m_dwUserIndex;
			pItemNativeInfo->bySlotIndex	= pItemMove->bSrcIndex;
		}
		else if(::IsItemNativeTile(pItemNativeInfo->eSlotID))
		{
			pItemNative = &pItemMove->Item;
			return pItemNative;
		}
		else if(::IsItemNativeTraderMonster(pItemNativeInfo->eSlotID))
		{
			pItemNative = &pItemMove->Item;
			return pItemNative;
		}
		else if(::IsItemDungeonManager(pItemNativeInfo->eSlotID))
		{
			CDungeonManagerWnd* pDungeonManagerWnd = CDungeonManagerWnd::GetInstance();

			if(pItemNativeInfo->bySlotIndex)
				pItemNative = &pDungeonManagerWnd->m_pDungeon->m_cGuardianItem;
			else
				pItemNative = &pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem;

			return pItemNative;
		}
	}
	else if(bIsSrcDest == GET_DEST_NATIVEPOINTER)
	{
		pItemNativeInfo->eSlotID = (ITEM_NATIVE)pItemMove->bDest;

		if(::IsItemNativePlayer(pItemNativeInfo->eSlotID))
		{
			pItemNativeInfo->dwOwnerID		= g_pMainPlayer->m_dwUserIndex;
			pItemNativeInfo->bySlotIndex	= pItemMove->bDestIndex;
		}
		else if(::IsItemNativeTile(pItemNativeInfo->eSlotID))
		{
			return NULL;
		}
		else if(::IsItemNativeTraderMonster(pItemNativeInfo->eSlotID))
		{
			pItemNative = &pItemMove->Item;
			return pItemNative;
		}
		else if(::IsItemDungeonManager(pItemNativeInfo->eSlotID))
		{
			CDungeonManagerWnd* pDungeonManagerWnd = CDungeonManagerWnd::GetInstance();

			if(pItemNativeInfo->bySlotIndex)
				pItemNative = &pDungeonManagerWnd->m_pDungeon->m_cGuardianItem;
			else
				pItemNative = &pDungeonManagerWnd->m_pDungeon->m_cMagicFieldArrayItem;

			return pItemNative;
		}
	}

	pItemNative	= GetItemNative(pItemNativeInfo);
	
	return pItemNative;
}


BOOL CItemMoveManager::TypeLargeMoveItemNative(DSTC_ITEM_MOVE* const pItemMove
											   , const CItem* pItemSrc
											   , const CItem* pItemDest
											   , ITEM_NATIVE_INFO* ItemNativeInfo_Src
											   , ITEM_NATIVE_INFO* ItemNativeInfo_Dest
											   , BYTE byQuantity)
{
	BOOL bIsEmpty = FALSE;
	BYTE byFindSlotIndex = 0xff;
	
	// 같은 창에서의 이동인지 본다.
	if( IsSameNative(ItemNativeInfo_Src->eSlotID,ItemNativeInfo_Dest->eSlotID) )
	{
		// Equip <=> Equip 
		if(	ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_EQUIP &&	ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_EQUIP )
		{
			PutOnItemNative(ItemNativeInfo_Src, ItemNativeInfo_Dest);
			return TRUE;
		}

		ExchangeItemNative(ItemNativeInfo_Src, ItemNativeInfo_Dest);		
		return TRUE;
	}

	// Inv_Large => Equip 착용
	if(	ItemNativeInfo_Src->eSlotID	== ITEM_NATIVE_INV_LARGE && ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_EQUIP )
	{
		PutOnItemNative(ItemNativeInfo_Src, ItemNativeInfo_Dest);
		return TRUE;
	}
	
	// ITEM_NATIVE_TRADERMONSTER_INV_LARGE <=> ITEM_NATIVE_INV_LARGE  
	if( ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_TRADERMONSTER_INV_LARGE && 
		ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_INV_LARGE )
	{
		SetSlotItemNative(pItemSrc,ItemNativeInfo_Dest);
		CBaseItem* lpItemTable = g_pItemTableHash->GetData(pItemSrc->GetID());
		g_pMainPlayer->m_dwMoney -= lpItemTable->GetPrice();

		char szInfo[0xff] = {0,};
		// MSG_ID : 89 ; %s 아이템을 구매하였습니다.			
		wsprintf(szInfo, g_Message[ETC_MESSAGE89].szMessage, lpItemTable->szItemName_Kor);		
		DisplayMessageAdd(szInfo, 0xFFFF2CFF);

		return TRUE;
	}
	else if(ItemNativeInfo_Src->eSlotID ==  ITEM_NATIVE_INV_LARGE &&
			ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_TRADERMONSTER_INV_LARGE)
	{
		CBaseItem* lpItemTable = g_pItemTableHash->GetData(pItemSrc->GetID());		
		g_pMainPlayer->m_dwMoney += lpItemTable->GetSellPrice();
		RemoveItemNative(ItemNativeInfo_Src);

		// MSG_ID : 83 ; %u Karz를 획득했습니다.
		char szInfo[0xff] = {0,};
		wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, lpItemTable->GetSellPrice());
		DisplayMessageAdd(szInfo, 0xFFFFC309);
		return TRUE;
	}

	// 들어갈 자리에 아이템이 없으면 
	if(pItemDest->GetID() == 0)
	{
		MoveItemNative(pItemMove, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity);
	}
	else
	{
		// 들어갈 자리에 아이템이 있으면 
		pItemDest = pItemDest - pItemMove->bDestIndex;

		for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
		{
			if(pItemDest[i].GetID() == 0)
			{
				bIsEmpty = TRUE;
				byFindSlotIndex = i;
				break;
			}
		}

		// Find Empty Slot
		if(bIsEmpty)
		{
			ItemNativeInfo_Dest->bySlotIndex = byFindSlotIndex;
			MoveItemNative(pItemMove, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity);		
		}
		else
		{
			return FALSE;
		}	
	}

	return TRUE;
}


BOOL CItemMoveManager::TypeSmallMoveItemNative(DSTC_ITEM_MOVE* const pItemMove,
											   const CItem* pItemSrc,
											   const CItem* pItemDest,
											   ITEM_NATIVE_INFO* ItemNativeInfo_Src,
											   ITEM_NATIVE_INFO* ItemNativeInfo_Dest,
											   BYTE byQuantity)
{
	// 같은 창에서의 이동인지 본다.	
	if( IsSameNative(ItemNativeInfo_Src->eSlotID,ItemNativeInfo_Dest->eSlotID) )
	{
		ExchangeItemNative(ItemNativeInfo_Src, ItemNativeInfo_Dest);
		return TRUE;
	}

	// ITEM_NATIVE_TRADERMONSTER_INV_SMALL => ITEM_NATIVE_INV_SMALL
	if( ItemNativeInfo_Src->eSlotID == ITEM_NATIVE_TRADERMONSTER_INV_SMALL &&
		ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_INV_SMALL )
	{
		CBaseItem* lpItemTable = g_pItemTableHash->GetData(pItemSrc->GetID());

		// 들어갈 자리에 아이템이 없으면
		if(pItemDest->GetID() == 0)
		{
			CItem cBuyItem;
			memcpy(&cBuyItem, pItemSrc, sizeof(CItem));
			cBuyItem.SetQuantity(byQuantity);

			SetSlotItemNative(&cBuyItem, ItemNativeInfo_Dest);

			g_pMainPlayer->m_dwMoney -= (lpItemTable->GetPrice() * byQuantity);

			// MSG_ID : 85 ; "%s 아이템을 %u개를 구매하셨습니다.
			char szInfo[0xff] = {0,};
			wsprintf(szInfo, g_Message[ETC_MESSAGE85].szMessage, lpItemTable->szItemName_Kor, byQuantity);	
			DisplayMessageAdd(szInfo, 0xFFFF2CFF);

			return TRUE;
		}
		else
		{
			// 들어갈 자리에 아이템이 있으면 		
			CBaseItem* pBaseItemDest = g_pItemTableHash->GetData(pItemDest->GetID());
			
			if(NULL == pBaseItemDest)
			{
				
			}

			// 들어갈 자리에 아이템이 있고 소스아이템과 같은 아이디면 수량이 겹칠수 있는지 알아본다.
			if(pItemSrc->GetID() == pItemDest->GetID())
			{
				BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
				BYTE byCnt			= byMaxQuantity - pItemDest->GetQuantity();

				if(byCnt >= byQuantity)
				{
					CItem cDestItem;
					memcpy(&cDestItem, pItemDest, sizeof(CItem));
					cDestItem.SetQuantity( cDestItem.GetQuantity() + byQuantity );

					SetSlotItemNative(&cDestItem, ItemNativeInfo_Dest);

					g_pMainPlayer->m_dwMoney -= (lpItemTable->GetPrice() * byQuantity);

					// MSG_ID : 85 ; "%s 아이템을 %u개를 구매하셨습니다.
					char szInfo[0xff] = {0,};
					wsprintf(szInfo, g_Message[ETC_MESSAGE85].szMessage, lpItemTable->szItemName_Kor, byQuantity);	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);

					return TRUE;
				}
				else if(byCnt != 0)
				{
					CItem cDestItem;
					memcpy(&cDestItem, pItemDest, sizeof(CItem));
					cDestItem.SetQuantity( cDestItem.GetQuantity() + byCnt );

					SetSlotItemNative(&cDestItem, ItemNativeInfo_Dest);

					byQuantity -= byCnt;
					g_pMainPlayer->m_dwMoney -= (lpItemTable->GetPrice() * byCnt);
				}
			}

			// 여기까지 왔으면 사야할 cnt가 남은거다. 맨처음 슬롯부터 찾는다.
			pItemDest = pItemDest - pItemMove->bDestIndex;

			for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
			{
				if(pItemDest[i].GetID() == 0)
				{
					CItem cBuyItem;
					memcpy(&cBuyItem, pItemSrc, sizeof(CItem));
					cBuyItem.SetQuantity(byQuantity);

					ItemNativeInfo_Dest->bySlotIndex = i;
					SetSlotItemNative(&cBuyItem, ItemNativeInfo_Dest);

					g_pMainPlayer->m_dwMoney -= (lpItemTable->GetPrice() * byQuantity);

					// MSG_ID : 85 ; "%s 아이템을 %u개를 구매하셨습니다.
					char szInfo[0xff] = {0,};
					wsprintf(szInfo, g_Message[ETC_MESSAGE85].szMessage, lpItemTable->szItemName_Kor, byQuantity);	
					DisplayMessageAdd(szInfo, 0xFFFF2CFF);
					
					return TRUE;
				}
				else
				{
					pBaseItemDest = g_pItemTableHash->GetData(pItemDest[i].GetID());
					if(NULL == pBaseItemDest)
						continue;

					if(pItemSrc->GetID() == pItemDest[i].GetID())
					{
						BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
						BYTE byCnt			= byMaxQuantity - pItemDest[i].GetQuantity();

						if(byCnt >= byQuantity)
						{
							CItem cDestItem;
							memcpy(&cDestItem, &pItemDest[i], sizeof(CItem));
							cDestItem.SetQuantity( cDestItem.GetQuantity() + byQuantity );

							ItemNativeInfo_Dest->bySlotIndex = i;
							SetSlotItemNative(&cDestItem, ItemNativeInfo_Dest);

							g_pMainPlayer->m_dwMoney -= (lpItemTable->GetPrice() * byQuantity);

							// MSG_ID : 85 ; "%s 아이템을 %u개를 구매하셨습니다.
							char szInfo[0xff] = {0,};
							wsprintf(szInfo, g_Message[ETC_MESSAGE85].szMessage, lpItemTable->szItemName_Kor, byQuantity);	
							DisplayMessageAdd(szInfo, 0xFFFF2CFF);

							return TRUE;
						}
						else if(byCnt != 0)
						{
							CItem cDestItem;
							memcpy(&cDestItem, &pItemDest[i], sizeof(CItem));
							cDestItem.SetQuantity( cDestItem.GetQuantity() + byCnt );

							ItemNativeInfo_Dest->bySlotIndex = i;
							SetSlotItemNative(&cDestItem, ItemNativeInfo_Dest);

							byQuantity -= byCnt;
							g_pMainPlayer->m_dwMoney -= (lpItemTable->GetPrice() * byCnt);				
						}
					}
				}
			}
		}

		return TRUE;
	}
	else if(ItemNativeInfo_Src->eSlotID ==  ITEM_NATIVE_INV_SMALL &&
			ItemNativeInfo_Dest->eSlotID == ITEM_NATIVE_TRADERMONSTER_INV_SMALL)
	{
		CBaseItem* lpItemTable = g_pItemTableHash->GetData(pItemSrc->GetID());

		CItem cItemSrc;
		memcpy(&cItemSrc, pItemSrc, sizeof(CItem));
		BYTE byCnt = cItemSrc.GetQuantity();

		if(byCnt == byQuantity)
		{
			RemoveItemNative(ItemNativeInfo_Src);
		}
		else if( byCnt > byQuantity )
		{
			cItemSrc.SetQuantity(byCnt - byQuantity);
			SetSlotItemNative(&cItemSrc, ItemNativeInfo_Src);
		}

		g_pMainPlayer->m_dwMoney += ( lpItemTable->GetSellPrice() * byQuantity );

		// MSG_ID : 83 ; %u Karz를 획득했습니다.
		char szInfo[0xff] = {0,};
		wsprintf(szInfo, g_Message[ETC_MESSAGE83].szMessage, lpItemTable->GetSellPrice() * byQuantity);
		DisplayMessageAdd(szInfo, 0xFFFFC309);

		return TRUE;
	}

	// <<<<<<< default Type Small Move ItemNative >>>>>>> //

	// 들어갈 자리에 아이템이 없으면
	if(pItemDest->GetID() == 0)
	{
		MoveItemNative(pItemMove, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity);
	}
	else
	{
		// 들어갈 자리에 아이템이 있으면 		
		CBaseItem* pBaseItemDest = g_pItemTableHash->GetData(pItemDest->GetID());
		
		if(NULL == pBaseItemDest)
		{
			
		}

		if(pItemSrc->GetID() == pItemDest->GetID())
		{
			BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
			BYTE byCnt			= byMaxQuantity - pItemDest->GetQuantity();

			if(byCnt >= byQuantity)
			{
				OverlapItemNative(pItemMove, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity);
				return TRUE;
			}
			else if(byCnt != 0)
			{
				OverlapItemNative(pItemMove, ItemNativeInfo_Src, ItemNativeInfo_Dest, byCnt);
				byQuantity -= byCnt;				
			}
		}

		// 여기까지 왔으면 이동해야할 cnt가 남은거다. 맨처음 슬롯부터 찾는다.
		pItemDest = pItemDest - pItemMove->bDestIndex;

		for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
		{
			if(pItemDest[i].GetID() == 0)
			{
				ItemNativeInfo_Dest->bySlotIndex = i;
				MoveItemNative(pItemMove, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity);

				return TRUE;
			}
			else
			{
				pBaseItemDest = g_pItemTableHash->GetData(pItemDest[i].GetID());
				if(NULL == pBaseItemDest)
					continue;
				
				if(pItemSrc->GetID() == pItemDest[i].GetID())
				{
					BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
					BYTE byCnt			= byMaxQuantity - pItemDest[i].GetQuantity();

					if(byCnt >= byQuantity)
					{
						ItemNativeInfo_Dest->bySlotIndex = i;
						OverlapItemNative(pItemMove, ItemNativeInfo_Src, ItemNativeInfo_Dest, byQuantity);

						return TRUE;
					}
					else if(byCnt != 0)
					{
						ItemNativeInfo_Dest->bySlotIndex = i;
						OverlapItemNative(pItemMove, ItemNativeInfo_Src, ItemNativeInfo_Dest, byCnt);

						byQuantity -= byCnt;								
					}
				}
			}
		}		
	}

	return TRUE;
}


void CItemMoveManager::AnotherPlayerShopUserBuyItemOverlapItem(DSTC_ITEM_MOVE* pItemMove
															   , const CItem* pItemSrc
															   , const CItem* pItemDest
															   , ITEM_NATIVE_INFO* ItemNativeInfo_Dest
															   , BYTE byQuantity)
{
	// 들어갈 자리에 아이템이 없으면
	if(pItemDest->GetID() == 0)
	{
		CItem cBuyItem;
		memcpy(&cBuyItem, pItemSrc, sizeof(CItem));
		cBuyItem.SetQuantity(byQuantity);

		SetSlotItemNative(&cBuyItem, ItemNativeInfo_Dest);

		return;
	}
	else
	{
		// 들어갈 자리에 아이템이 있으면 		
		CBaseItem* pBaseItemDest = g_pItemTableHash->GetData(pItemDest->GetID());
		
		if(NULL == pBaseItemDest)
		{

		}

		// 들어갈 자리에 아이템이 있고 소스아이템과 같은 아이디면 수량이 겹칠수 있는지 알아본다.
		if(pItemSrc->GetID() == pItemDest->GetID())
		{
			BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
			BYTE byCnt			= byMaxQuantity - pItemDest->GetQuantity();

			if(byCnt >= byQuantity)
			{
				CItem cDestItem;
				memcpy(&cDestItem, pItemDest, sizeof(CItem));
				cDestItem.SetQuantity( cDestItem.GetQuantity() + byQuantity );

				SetSlotItemNative(&cDestItem, ItemNativeInfo_Dest);	
				
				return;
			}
			else if(byCnt != 0)
			{
				CItem cDestItem;
				memcpy(&cDestItem, pItemDest, sizeof(CItem));
				cDestItem.SetQuantity( cDestItem.GetQuantity() + byCnt );

				SetSlotItemNative(&cDestItem, ItemNativeInfo_Dest);

				byQuantity -= byCnt;					
			}
		}

		// 여기까지 왔으면 사야할 cnt가 남은거다. 맨처음 슬롯부터 찾는다.
		pItemDest = pItemDest - pItemMove->bDestIndex;

		for(BYTE i = 0; i < m_bSlotPool[pItemMove->bDest]; i++)
		{
			if(pItemDest[i].GetID() == 0)
			{
				CItem cBuyItem;
				memcpy(&cBuyItem, pItemSrc, sizeof(CItem));
				cBuyItem.SetQuantity(byQuantity);

				ItemNativeInfo_Dest->bySlotIndex = i;
				SetSlotItemNative(&cBuyItem, ItemNativeInfo_Dest);				
				
				return;
			}
			else
			{
				pBaseItemDest = g_pItemTableHash->GetData(pItemDest[i].GetID());
				if(NULL == pBaseItemDest)
					continue;

				if(pItemSrc->GetID() == pItemDest[i].GetID())
				{
					BYTE byMaxQuantity	= pBaseItemDest->GetMaxQuantity();
					BYTE byCnt			= byMaxQuantity - pItemDest[i].GetQuantity();

					if(byCnt >= byQuantity)
					{
						CItem cDestItem;
						memcpy(&cDestItem, &pItemDest[i], sizeof(CItem));
						cDestItem.SetQuantity( cDestItem.GetQuantity() + byQuantity );

						ItemNativeInfo_Dest->bySlotIndex = i;
						SetSlotItemNative(&cDestItem, ItemNativeInfo_Dest);

						return;
					}
					else if(byCnt != 0)
					{
						CItem cDestItem;
						memcpy(&cDestItem, &pItemDest[i], sizeof(CItem));
						cDestItem.SetQuantity( cDestItem.GetQuantity() + byCnt );

						ItemNativeInfo_Dest->bySlotIndex = i;
						SetSlotItemNative(&cDestItem, ItemNativeInfo_Dest);

						byQuantity -= byCnt;						
					}
				}
			}
		}
	}
}

//=========================================================================================
// 다른 유저의 플레이어샵에 있는 아템을 산다.. 
//=========================================================================================
void CItemMoveManager::AnotherPlayerShopUserBuyItem(DSTC_ITEM_MOVE* pItemMove)
{
	BYTE				byQuantity			= NULL;
	const CItem*		pItemSrc			= NULL;
	const CItem*		pItemDest			= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;

	pItemSrc	= &pItemMove->Item;
	pItemDest	= GetItemNativePointer(pItemMove,&ItemNativeInfo_Dest,GET_DEST_NATIVEPOINTER);

	byQuantity	= pItemMove->bQuantity;

	if( pItemMove->Item.IsOverlapItem() )
	{
		AnotherPlayerShopUserBuyItemOverlapItem(pItemMove, pItemSrc, pItemDest, &ItemNativeInfo_Dest, byQuantity);
		g_pMainPlayer->m_dwMoney = pItemMove->dwMoney;

		CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
		BYTE byCnt			= pPlayerShopWnd->m_sPlayerShopInfo.cItem[pItemMove->bSrcIndex].GetQuantity();
		BYTE byBuyRealCnt	= pItemMove->bQuantity; 

		if(byCnt == byBuyRealCnt)
		{
			memset(&pPlayerShopWnd->m_sPlayerShopInfo.cItem[pItemMove->bSrcIndex], 0, sizeof(CItem));
		}
		else if(byCnt > byBuyRealCnt)
		{
			pPlayerShopWnd->m_sPlayerShopInfo.cItem[pItemMove->bSrcIndex].SetQuantity(byCnt - byBuyRealCnt);
		}
	}
	else
	{
		SetSlotItemNative(pItemSrc,&ItemNativeInfo_Dest);
		g_pMainPlayer->m_dwMoney = pItemMove->dwMoney;

		CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
		memset(&pPlayerShopWnd->m_sPlayerShopInfo.cItem[pItemMove->bSrcIndex], 0, sizeof(CItem));
	}
}


void CItemMoveManager::DefaultSlotChangeProcess(DSTC_ITEM_MOVE* pItemMove)
{
	const CItem*		pItemSrc			= NULL;
	const CItem*		pItemDest			= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;

	// 다른 유저의 플레이어샵에 있는 아템을 산다.. 
	if(pItemMove->dwUnitID > 0)
	{
		CUser* pSeller = g_pUserHash->GetData(pItemMove->dwUnitID);
		if(pSeller)
		{
			AnotherPlayerShopUserBuyItem(pItemMove);
			return;
		}
	}

	pItemSrc	= GetItemNativePointer(pItemMove,&ItemNativeInfo_Src,GET_SRC_NATIVEPOINTER);
	pItemDest	= GetItemNativePointer(pItemMove,&ItemNativeInfo_Dest,GET_DEST_NATIVEPOINTER);	

	if(NULL == pItemSrc || NULL == pItemDest)
		return;

	CBaseItem* pItem = g_pItemTableHash->GetData(pItemSrc->GetID());

	if(NULL == pItem)
		return;

	// Virtual Slot Change	
	ITEM_NATIVE_TYPE eNativeTypeSrc		= GetItemNativeType(ItemNativeInfo_Src.eSlotID);
	ITEM_NATIVE_TYPE eNativeTypeDest	= GetItemNativeType(ItemNativeInfo_Dest.eSlotID);

	if(eNativeTypeSrc == ITEM_NATIVE_TYPE_VIRTUAL && eNativeTypeDest == ITEM_NATIVE_TYPE_VIRTUAL)
	{
		if(ItemNativeInfo_Src.eSlotID	== ITEM_NATIVE_PLAYERSHOP && ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_PLAYERSHOP)
		{
			DWORD dwSrcCustomSellPrice	= g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[ItemNativeInfo_Src.bySlotIndex];
			DWORD dwDestCustomSellPrice = g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[ItemNativeInfo_Dest.bySlotIndex];

			g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[ItemNativeInfo_Dest.bySlotIndex]	= dwSrcCustomSellPrice;
			g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[ItemNativeInfo_Src.bySlotIndex]	= dwDestCustomSellPrice;
			
			if(pItemDest && pItemDest->GetID() == 0)
			{
				g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[ItemNativeInfo_Src.bySlotIndex] = 0;
			}
		}
	}

	if(eNativeTypeSrc == ITEM_NATIVE_TYPE_VIRTUAL)
	{
		if((ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_INV_LARGE	||
			ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_INV_SMALL)	&&
			ItemNativeInfo_Src.eSlotID	== ITEM_NATIVE_PLAYERSHOP)
		{
			g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[ItemNativeInfo_Src.bySlotIndex] = pItemMove->dwMoney; 	
		}
	}
	else if(eNativeTypeDest == ITEM_NATIVE_TYPE_VIRTUAL)
	{
		if((ItemNativeInfo_Src.eSlotID	== ITEM_NATIVE_INV_LARGE ||
			ItemNativeInfo_Src.eSlotID	== ITEM_NATIVE_INV_SMALL) &&			 	
			ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_PLAYERSHOP)
		{
			g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[ItemNativeInfo_Dest.bySlotIndex] = pItemMove->dwMoney; 	
		}
	}
		
	ITEM_TYPE eType = GetItemType(pItemSrc->GetID());
	BYTE byQuantity = pItemSrc->GetQuantity();
	byQuantity		= pItemMove->bQuantity;

	CItemWnd* pItemWnd = CItemWnd::GetInstance();
	CBankWnd* pBankWnd = CBankWnd::GetInstance();
	
	if(ITEM_TYPE_LARGE == eType)
	{
		TypeLargeMoveItemNative(pItemMove,pItemSrc,pItemDest,&ItemNativeInfo_Src,&ItemNativeInfo_Dest,byQuantity);

		if(ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_INV_LARGE)
		{
			if(pItemWnd->m_byItemChk == TRUE)
				pItemWnd->m_byItemChk = FALSE;
		}
		else if(ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_BANK_LARGE)
		{
			if(pBankWnd->m_byBankChk == TRUE)
				pBankWnd->m_byBankChk = FALSE;			
		}
	}
	else if(ITEM_TYPE_SMALL == eType)
	{
		TypeSmallMoveItemNative(pItemMove,pItemSrc,pItemDest,&ItemNativeInfo_Src,&ItemNativeInfo_Dest,byQuantity);

		if(ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_INV_SMALL)
		{
			if(pItemWnd->m_byItemChk == FALSE)
				pItemWnd->m_byItemChk = TRUE;
		}
		else if(ItemNativeInfo_Dest.eSlotID == ITEM_NATIVE_BANK_SMALL)
		{
			if(pBankWnd->m_byBankChk == FALSE)
				pBankWnd->m_byBankChk = TRUE;			
		}
	}
	
	PlayItemSound(pItemSrc->GetID());
}


void CItemMoveManager::DropAndPickupItemProcess(DSTC_ITEM_MOVE* pItemMove)
{
	const CItem*		pItemSrc			= NULL;
	const CItem*		pItemDest			= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
	BYTE				byQuantity			= NULL;

	if( (ITEM_NATIVE)pItemMove->bSrc	== ITEM_NATIVE_INV_KARZ &&
		(ITEM_NATIVE)pItemMove->bDest	== ITEM_NATIVE_TILE )
	{
		ItemNativeInfo_Src.eSlotID		= (ITEM_NATIVE)pItemMove->bSrc;
		ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
		ItemNativeInfo_Src.bySlotIndex	= pItemMove->bSrcIndex;
		ItemNativeInfo_Src.dwMoney		= pItemMove->dwMoney;

		DropItem(pItemMove, &ItemNativeInfo_Src, byQuantity);		
		return;
	}

	pItemSrc	= GetItemNativePointer(pItemMove,&ItemNativeInfo_Src,GET_SRC_NATIVEPOINTER);
	pItemDest	= GetItemNativePointer(pItemMove,&ItemNativeInfo_Dest,GET_DEST_NATIVEPOINTER);

	if(NULL == pItemSrc || NULL == pItemDest)
	{
		
	}

	CBaseItem* pItem = g_pItemTableHash->GetData(pItemSrc->GetID());
	if(NULL == pItem)
	{
		
	}

	byQuantity = pItemSrc->GetQuantity();
	byQuantity = pItemMove->bQuantity;

	if( IsItemNativeInvLargeAndSmall((ITEM_NATIVE)pItemMove->bSrc) )
	{
		// Drop Item
		DropItem(pItemMove, &ItemNativeInfo_Src, byQuantity);
		PlayItemSound(pItemSrc->GetID());
	}
	else if( IsItemNativeTile((ITEM_NATIVE)pItemMove->bSrc) )
	{
		// Pickup Item
		PickupItem(pItemMove, &ItemNativeInfo_Src, &ItemNativeInfo_Dest);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_GETITEM, g_v3InterfaceSoundPos, FALSE);
	}
}


void CItemMoveManager::KarzMoveProcess(DSTC_ITEM_MOVE* pItemMove)
{
	g_pMainPlayer->m_dwMoney		= pItemMove->dwMoney;
	g_pMainPlayer->m_dwBankMoney	= (DWORD)pItemMove->i64ItemID; 	
}



