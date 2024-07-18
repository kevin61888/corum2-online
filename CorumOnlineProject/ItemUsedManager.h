#if !defined(AFX_ITEMUSEDMANAGER_H__F8519F19_F285_4BCE_809F_0B85694AD7BC__INCLUDED_)
#define AFX_ITEMUSEDMANAGER_H__F8519F19_F285_4BCE_809F_0B85694AD7BC__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#define MAX_TYPE	10
#define MAX_VALUE	100


struct DSTC_ITEM_USED_NATIVE;


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
};


class CItemUsedManager  
{

public:

	CItemUsedManager();
	virtual ~CItemUsedManager();

public:

	void	SendItemUsedNativePacket(BYTE bSrc, BYTE bSrcIndex, BYTE bCommonIndex = 0xff);
	void	SendItemUsedNativePacket(BYTE bSrc, BYTE bSrcIndex, BYTE bCommonIndex, DWORD dwCommonId, DWORD dwPortalUserIndex);
	void	Initialize();
	void	Process(char* pMsg,DWORD dwLen);

private:

	void	(CItemUsedManager::*ItemUsedProc[MAX_TYPE][MAX_VALUE])	(DSTC_ITEM_USED_NATIVE* const pItemUsed);	
	void	InitItemUsedProc();	

	void	Value_ItemUsedNative_CoordinateSave(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_Goto_Coordinate(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_TownPortalScroll(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_Insurance(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_HP(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_MP(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_HP_Point(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_SP_HP_Point(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_HatchTimeShortening(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_Guild_Clen_Make(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_Status_Skill_Init(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_Voucher_Of_Hero(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_PersonalRecall(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_AllPartyRecall(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_Guild_Recall(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_MagicArrayToParty(DSTC_ITEM_USED_NATIVE* const pItemUsed);
	void	Value_ItemUsedNative_Consumable(DSTC_ITEM_USED_NATIVE* const pItemUsed);
};

#endif 
