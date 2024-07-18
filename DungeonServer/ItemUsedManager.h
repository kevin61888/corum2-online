#if !defined(AFX_ITEMUSEDMANAGER_H__C997241A_7B3F_4E7E_A48D_2F04A0DB23B4__INCLUDED_)
#define AFX_ITEMUSEDMANAGER_H__C997241A_7B3F_4E7E_A48D_2F04A0DB23B4__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#define MAX_TYPE	10
#define MAX_VALUE	100


#include "ItemMove.h"


class CUser;
class CDungeonLayer;
class CItem;


enum 
{
	CANNOT_ENOUGH_LEVEL = 1,
	UNPACK_PRESENTBOX_FAIL,	
	QUERY_ERROR,
	SUPPLY_EXCEED,
	NOT_CONFORM_ITEM,
	UNIT_STATUS_IN_SIEGE,
	IN_VILLAGE,
	MAGICARRAY_TO_PARTY_OCCUPIABLE,
	CANNOT_FIND_HERO_ITEM,
	CANNOT_FIND_GUARDIANITEM,
	CANNOT_FIND_AQUARIUS,
	NOT_GUARDIAN_ITEM_TYPE_EGG,
	UN_SUMMON_GURADIAN,
	UN_SUMMON_GURADIANEGG,
	CANNOT_COORDINATESAVE,
	CANNOT_GOTO_COORDINATE,
	ITEM_LINKING,
	ALREADY_GUILD_JOIN,
	ALREADY_CLEN_JOIN,
	IS_NOT_PARTY,	
	IS_NOT_PARTY_LEADER,
	CUR_DUNGEON_EVENT_INLIVE,
	CANNOT_MYSELF_USED,
	USER_LAST_ITEM_CASTING,
	CANNOT_FIND_VALUE_PROC,
	CANNOT_FIND_NATIVE_VALUE,
};


class CItemUsedManager  
{

public:

	CItemUsedManager();
	virtual ~CItemUsedManager();

public:

	void	Process(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);

private:
	
	void	(CItemUsedManager::*ItemUsedTypeProc[MAX_TYPE])		(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, CTDS_ITEM_USED_NATIVE* const pItemUsed, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	(CItemUsedManager::*ItemUsedValueProc[MAX_VALUE])	(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* ItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	void	InitItemUsedTypeProc();
	void	InitItemUsedValueProc();
	void	InitItemMoveResultPacket(CTDS_ITEM_USED_NATIVE* const pItemUsed, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);

	BOOL	IsAvailability(CUser* pUser, CDungeonLayer** pLayer, CTDS_ITEM_USED_NATIVE* const pItemUsed);
	BOOL	FindItemUsedNativeValue(CTDS_ITEM_USED_NATIVE* const pItemUsed,BYTE* pbSrc, CUser* const pUser);

	void	SendToItemMoveResult(DWORD dwConnectionIndex, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	void	ItemMoveLogProcess();

	void	ItemUsedNativeTypeZodiac(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, CTDS_ITEM_USED_NATIVE* const pItemUsed, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	void	ItemUsedNativeTypeSupplies(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, CTDS_ITEM_USED_NATIVE* const pItemUsed, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	void	ItemUsedNativeTypeSpecial(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, CTDS_ITEM_USED_NATIVE* const pItemUsed, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	void	ItemUsedNativeTypeMagicarray(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, CTDS_ITEM_USED_NATIVE* const pItemUsed, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	void	ItemUsedNativeTypeConsumable(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, CTDS_ITEM_USED_NATIVE* const pItemUsed, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);

	BOOL	Value_ItemUsedNative_HP(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_MP(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_HP_Point(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_SP_HP_Point(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_HatchTimeShortening(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_LifeExtension(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	
	BOOL	Value_ItemUsedNative_CoordinateSave(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_Goto_Coordinate(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_TownPortalScroll(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_Insurance(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);

	BOOL	Value_ItemUsedNative_Guild_Clen_Make(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_Status_Skill_Init(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_Voucher_Of_Hero(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);

	BOOL	Value_ItemUsedNative_PersonalRecall(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_AllPartyRecall(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_Guild_Recall(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
	BOOL	Value_ItemUsedNative_MagicArrayToParty(DWORD dwConnectionIndex, CUser* const pUser, CDungeonLayer* const pLayer, const CItem* pItemNative, ITEM_NATIVE_INFO* pItemNativeInfo_Used, DSTC_ITEM_USED_NATIVE* const pItemUsedResult);
};

#endif 
