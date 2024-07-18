#include "stdafx.h"
#include "ItemUsedManager.h"
#include "../CommonServer/CommonHeader.h"
#include "ItemMove.h"
#include "NetworkClient.h"
#include "GameControl.h"
#include "User.h"
#include "GameDefine.h"
#include "HashTable.h"
#include "InitGame.h"
#include "PlayerShopWnd.h"
#include "Chat.h"
#include "message.h"
#include "DungeonProcess.h"
#include "ItemWnd.h"
#include "UserInterface.h"
#include "GuildExWnd.h"
#include "ChatWnd.h"
#include "InputManager.h"


CItemUsedManager::CItemUsedManager()
{
	memset(CItemUsedManager::ItemUsedProc,NULL,sizeof(CItemUsedManager::ItemUsedProc));
	InitItemUsedProc();
}


CItemUsedManager::~CItemUsedManager()
{
	
}


void CItemUsedManager::SendItemUsedNativePacket(BYTE bSrc, BYTE bSrcIndex, BYTE bCommonIndex)
{
	CTDS_ITEM_USED_NATIVE packet;
	packet.bSrc				= bSrc;
	packet.bSrcIndex		= bSrcIndex;
	packet.bCommonIndex		= bCommonIndex; 
	
	g_pNet->SendMsg((char*)&packet,packet.GetPacketSize(),SERVER_INDEX_ZONE);
}


void CItemUsedManager::SendItemUsedNativePacket(BYTE bSrc, BYTE bSrcIndex, BYTE bCommonIndex
												, DWORD dwCommonId, DWORD dwPortalUserIndex)
{
	CTDS_ITEM_USED_NATIVE packet;
	packet.bSrc					= bSrc;
	packet.bSrcIndex			= bSrcIndex;
	packet.bCommonIndex			= bCommonIndex; 
	packet.dwCommonId			= dwCommonId;
	packet.dwPortalUserIndex	= dwPortalUserIndex;
	
	g_pNet->SendMsg((char*)&packet,packet.GetPacketSize(),SERVER_INDEX_ZONE);
}


void CItemUsedManager::Initialize()
{
	
}


void CItemUsedManager::Process(char* pMsg,DWORD dwLen)
{
	DSTC_ITEM_USED_NATIVE* pItemUsed = (DSTC_ITEM_USED_NATIVE*)pMsg;

	// 에러코드 처리
	if(pItemUsed->bErrorCode < 0xff)
	{
//		DisplayMessageAdd();
		return;
	}

	BYTE bSrc	= pItemUsed->bItemUsedNativeType;
	BYTE bDest	= pItemUsed->bItemUsedNativeValue;

	if(ItemUsedProc[bSrc][bDest])
	{
		(this->*ItemUsedProc[bSrc][bDest])(pItemUsed);		
	}
	else
	{

	}
}


void CItemUsedManager::InitItemUsedProc()
{
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_ZODIAC][ITEM_USED_NATIVE_VALUE_COORDINATE_SAVE]			= Value_ItemUsedNative_CoordinateSave;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_ZODIAC][ITEM_USED_NATIVE_VALUE_GOTO_COORDINATE]			= Value_ItemUsedNative_Goto_Coordinate;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_ZODIAC][ITEM_USED_NATIVE_VALUE_TOWNPORTALSCROLL]			= Value_ItemUsedNative_TownPortalScroll;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_ZODIAC][ITEM_USED_NATIVE_VALUE_INSURANCE]				= Value_ItemUsedNative_Insurance;

	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SUPPLIES][ITEM_USED_NATIVE_VALUE_HP]						= Value_ItemUsedNative_HP;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SUPPLIES][ITEM_USED_NATIVE_VALUE_MP]						= Value_ItemUsedNative_MP;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SUPPLIES][ITEM_USED_NATIVE_VALUE_HP_POINT]				= Value_ItemUsedNative_HP_Point;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SUPPLIES][ITEM_USED_NATIVE_VALUE_SP_HP_POINT]			= Value_ItemUsedNative_SP_HP_Point;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SUPPLIES][ITEM_USED_NATIVE_VALUE_HATCH_TIME_SHORTENING]	= Value_ItemUsedNative_HatchTimeShortening; 
	
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SPECIAL][ITEM_USED_NATIVE_VALUE_GUILD_CLEN_MAKE]			= Value_ItemUsedNative_Guild_Clen_Make;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SPECIAL][ITEM_USED_NATIVE_VALUE_STATUS_SKILL_INIT]		= Value_ItemUsedNative_Status_Skill_Init;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SPECIAL][ITEM_USED_NATIVE_VALUE_VOUCHER_OF_HERO]			= Value_ItemUsedNative_Voucher_Of_Hero;
	
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SPECIAL][ITEM_USED_NATIVE_VALUE_PERSONAL_RECALL]			= Value_ItemUsedNative_PersonalRecall;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SPECIAL][ITEM_USED_NATIVE_VALUE_ALL_PARTY_RECALL]		= Value_ItemUsedNative_AllPartyRecall;	
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SPECIAL][ITEM_USED_NATIVE_VALUE_GUILD_RECALL]			= Value_ItemUsedNative_MagicArrayToParty;
	ItemUsedProc[ITEM_USED_NATIVE_TYPE_SPECIAL][ITEM_USED_NATIVE_VALUE_MAGICARRAY_TO_PARTY]		= Value_ItemUsedNative_MagicArrayToParty;

	ItemUsedProc[ITEM_USED_NATIVE_TYPE_CONSUMABLE][ITEM_USED_NATIVE_VALUE_CONSUMABLE_START]		= Value_ItemUsedNative_Consumable;
}


//==============================
// 조디악 카드(쌍둥이) 좌표 저장 
//==============================
void CItemUsedManager::Value_ItemUsedNative_CoordinateSave(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	
	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);

	// "좌표가 저장되었습니다."
	DisplayMessageAdd(g_Message[ETC_MESSAGE511].szMessage, 0xFFFF2CFF); 
}


//=======================================
// 조디악 카드(쌍둥이) 저장된 좌표로 이동 
//=======================================
void CItemUsedManager::Value_ItemUsedNative_Goto_Coordinate(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)ITEM_NATIVE_INV_SMALL;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bCommonIndex;
	
	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;

	const CItem* pItemNative = GetItemNative(&ItemNativeInfo);

	WORD wToDungeonID = pItemNative->m_Item_Zodiac.wMapId;
	SendPacketRequestDungeonInfo(wToDungeonID);

	g_pMainPlayer->SetAction(36);
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;

	SendStopPacket();

	CTDS_PORTAL_MOVE packet;			
	packet.wPortalItemID    = pItemNative->GetID();
	packet.wToDungeonID		= pItemNative->m_Item_Zodiac.wMapId;
	packet.bLayerNo			= pItemNative->m_Item_Zodiac.bLayer;
	packet.v2Pos.x			= (float)pItemNative->m_Item_Zodiac.wPosX;
	packet.v2Pos.y			= (float)pItemNative->m_Item_Zodiac.wPosZ;

	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);

	g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
	
	CItemWnd* pWnd = CItemWnd::GetInstance();
	pWnd->SetActive(FALSE);
}


//=======================================
// 마울 포탈 스크롤 
//=======================================
void CItemUsedManager::Value_ItemUsedNative_TownPortalScroll(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	const CItem* pItemNative = GetItemNative(&ItemNativeInfo);

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));

	BYTE byCnt = cItem.GetQuantity();

	if(byCnt > 1)
	{
		cItem.SetQuantity(byCnt - 1);
		SetSlotItemNative(&cItem, &ItemNativeInfo);
	}
	else
	{
		RemoveItemNative(&ItemNativeInfo);
	}

	WORD wToDungeonID = pItemUsed->cItem.m_Item_Zodiac.wMapId;	
					
	SendPacketRequestDungeonInfo(wToDungeonID);

	g_pMainPlayer->SetAction(36);
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;	

	SendStopPacket();

	CTDS_PORTAL_MOVE packet;	
	packet.wPortalItemID    = (WORD)cItem.GetID();
	packet.wToDungeonID		= cItem.m_Item_Zodiac.wMapId;					
	packet.bLayerNo			= cItem.m_Item_Zodiac.bLayer;
	packet.v2Pos.x			= (float)cItem.m_Item_Zodiac.wPosX;
	packet.v2Pos.y			= (float)cItem.m_Item_Zodiac.wPosZ;

	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);

	g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);

	CItemWnd* pWnd = CItemWnd::GetInstance();
	pWnd->SetActive(FALSE);
}


//=======================================
// 아이템 보험
//=======================================
void CItemUsedManager::Value_ItemUsedNative_Insurance(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	CItem cItem;
	memcpy(&cItem, &pItemUsed->cItem, sizeof(CItem));

	SetSlotItemNative(&cItem, &ItemNativeInfo);

	DisplayMessageAdd("아이템 보험이 적용되었습니다.", 0xFFFF2CFF);
}


//========================
// HP
//========================
void CItemUsedManager::Value_ItemUsedNative_HP(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);

	if(pItemUsed->bSrc == ITEM_NATIVE_GUARDIAN)
	{
		g_pMainPlayer->m_pGuardian[0]->m_dwHP = pItemUsed->dwCommonId; /*가뎐 HP*/

		CUserInterface* pUserInterface = CUserInterface::GetInstance();
		pUserInterface->SetGauge(g_pMainPlayer->m_pGuardian[0]->m_dwHP, g_pMainPlayer->m_pGuardian[0]->m_dwMaxHP);
	}
}


//========================
// MP
//========================
void CItemUsedManager::Value_ItemUsedNative_MP(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);
}


//========================
// HP Point
//========================
void CItemUsedManager::Value_ItemUsedNative_HP_Point(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);

	if(pItemUsed->bSrc == ITEM_NATIVE_GUARDIAN)
	{
		g_pMainPlayer->m_pGuardian[0]->m_dwHP = pItemUsed->dwCommonId; /*가뎐 HP*/

		CUserInterface* pUserInterface = CUserInterface::GetInstance();
		pUserInterface->SetGauge(g_pMainPlayer->m_pGuardian[0]->m_dwHP, g_pMainPlayer->m_pGuardian[0]->m_dwMaxHP);
	}
}


//========================
// SP,HP Point
//========================
void CItemUsedManager::Value_ItemUsedNative_SP_HP_Point(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);
}


//========================
// 가디언 부화 시간 단축 
//========================
void CItemUsedManager::Value_ItemUsedNative_HatchTimeShortening(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);
}


//=======================================
// 길드,클랜 생성 
//=======================================
void CItemUsedManager::Value_ItemUsedNative_Guild_Clen_Make(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	CGuildExWnd*		pGuildExWnd		= CGuildExWnd::GetInstance();	
	CChatWnd*			pChatWnd		= CChatWnd::GetInstance();
	ITEM_NATIVE_INFO	ItemNativeInfo;
	RECT				rt				= {0,};
	int					nOrder			= 0;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);

	// 생성 창 //								
	pGuildExWnd->SetActive(TRUE);
	pGuildExWnd->m_byChk = 1;

	if(pItemUsed->cItem.GetID() == __ITEM_GUILD_INDEX__)
		pGuildExWnd->m_byGuildType = __GCTYPE_GUILD__;			
	else if(pItemUsed->cItem.GetID() == __ITEM_CLEN_INDEX__)
		pGuildExWnd->m_byGuildType = __GCTYPE_CLEN__;

	rt.left		= (LONG)pGuildExWnd->m_fPosX+135;
	rt.right	= (LONG)pGuildExWnd->m_fPosX+200;
	rt.top		= (LONG)pGuildExWnd->m_fPosZ+70;
	rt.bottom	= (LONG)pGuildExWnd->m_fPosZ+82;
	nOrder		= __ORDER_INTERFACE_START__ + pGuildExWnd->m_byOrder * 10 + 2;

	if(TRUE == g_pGVDungeon->bChatMode)
	{
		g_pInputManager->ClearInput(INPUT_BUFFER_19);
		g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
		
		pChatWnd->SetActive(FALSE);				
	}

	g_pInputManager->InitializeInput(INPUT_BUFFER_7, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
	
	ChatModeOn(INPUT_BUFFER_7, FALSE);

#ifdef	_USE_IME 
	POINT pChatPoint;
	pChatPoint.x	= long(pGuildExWnd->m_fPosX + 135);
	pChatPoint.y	= long(pGuildExWnd->m_fPosZ + 70);
	GET_IMEEDIT()->SetEditIndex(1);
	GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_GUILD_NAME_REAL_LENGTH, pGuildExWnd->GetStartOrder()+2);			
#endif
}


//=======================================
// 스킬,스탯 초기화  
//=======================================
void CItemUsedManager::Value_ItemUsedNative_Status_Skill_Init(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);
}


//=======================================================
// 영웅의 증표((구)태극기) 
//=======================================================
void CItemUsedManager::Value_ItemUsedNative_Voucher_Of_Hero(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	const CItem*		pItemNative		= NULL;
	BYTE				byCnt			= NULL;
	ITEM_NATIVE_INFO	ItemNativeInfo;
	ITEM_NATIVE_INFO	ItemNativeInfo_Present;
	ITEM_TYPE			eType;
	
	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	pItemNative = GetItemNative(&ItemNativeInfo);

	CItem cItem;
	memcpy(&cItem, pItemNative, sizeof(CItem));

	// 영웅의 증표 아이템 지우기 
	byCnt = cItem.GetQuantity();

	if(byCnt > 1)
	{
		cItem.SetQuantity(byCnt - 1);
		SetSlotItemNative(&cItem, &ItemNativeInfo);
	}
	else
	{
		RemoveItemNative(&ItemNativeInfo);
	}

	eType = GetItemType(pItemUsed->cItem.GetID());

	// 선물 아이템 증정 
	if(eType == ITEM_TYPE_LARGE)
		ItemNativeInfo_Present.eSlotID = (ITEM_NATIVE)ITEM_NATIVE_INV_LARGE;
	else if(eType == ITEM_TYPE_SMALL)
		ItemNativeInfo_Present.eSlotID = (ITEM_NATIVE)ITEM_NATIVE_INV_SMALL;

	ItemNativeInfo.bySlotIndex	= pItemUsed->bCommonIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo_Present);
}


//=======================================================
// 퍼스널 리콜 
//=======================================================
void CItemUsedManager::Value_ItemUsedNative_PersonalRecall(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	
}


//=======================================================
// 파티 전원 리콜
//=======================================================
void CItemUsedManager::Value_ItemUsedNative_AllPartyRecall(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);	
}


//=======================================================
// 길드 리콜 
//=======================================================
void CItemUsedManager::Value_ItemUsedNative_Guild_Recall(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);	
}


//=======================================================
// 파티 마법진 사용 
//=======================================================
void CItemUsedManager::Value_ItemUsedNative_MagicArrayToParty(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);		
}


//============================================================================================
// 마일리지 아이템 사용 
//============================================================================================
void CItemUsedManager::Value_ItemUsedNative_Consumable(DSTC_ITEM_USED_NATIVE* const pItemUsed)
{
	ITEM_NATIVE_INFO ItemNativeInfo;

	ItemNativeInfo.eSlotID		= (ITEM_NATIVE)pItemUsed->bSrc;
	ItemNativeInfo.bySlotIndex	= pItemUsed->bSrcIndex;
	ItemNativeInfo.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;	

	SetSlotItemNative(&pItemUsed->cItem, &ItemNativeInfo);			
}








