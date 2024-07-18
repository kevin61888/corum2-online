#pragma once


#include "CommonServer.h"


#pragma pack(push,1)


enum ITEM_WEAPON_TYPE;
class CBaseItem;

#pragma warning( disable: 4201 )

struct ITEM_SERIAL_STRUCT
{
	BYTE	bServerSet;
	BYTE	bServerSerialID;
	WORD	wRestartCount;
	DWORD	dwSerialNum;
};

struct ITEM_SERIAL
{
	union
	{
		ITEM_SERIAL_STRUCT	ItemSerialStruct;
		unsigned __int64 i64Serial;
	};
};
#pragma warning( default: 4201 )

struct ITEM_SLOT
{
	BYTE	uUsedSlot	:4;
	BYTE	uMaxSlot	:4;
};

struct ITEM_ATTR
{
	WORD	uCode	:6;			
	WORD	uValue	:10;		
};

struct ITEM_WEAPON
{
	unsigned __int64	wMin_Damage		: 8;
	unsigned __int64	wMax_Damage		: 8;
	unsigned __int64	wMana_Damage	: 10;
	unsigned __int64	i64Reserved		: 38;
	
	DWORD	dwReserved		: 32;
	BYTE	bReserved		: 8;
};

struct ITEM_ARMOR
{
	WORD	wDefense		: 10;		
	WORD	wReserved		: 6;
};

struct ITEM_RIDEOBJECT
{
	float	fWorldPos_x;		
	float	fWorldPos_z;
};

struct ITEM_GUARDIAN
{
	DWORD	dwEndLife;				// 가뎐 생명 끝나는 시기. 이값이전에 태어나야 해..
	DWORD	dwCompleteTime	: 32;	// 완성도
	DWORD	dwID			: 32;	// 디비에 생성된 가디언의 아이디.

	BYTE	bCreate			: 1;	// 가디언이 디비에 생성됐는가. 0이라면 아직 알인 상태인것이다.
	BYTE	bReserved		: 7;
};

struct ITEM_MONEY
{
	DWORD	dwMoney			: 32;		
};

struct ITEM_BAG
{
	WORD	wProbability	: 12;	
	WORD	bType			: 4;	
};

struct ITEM_ZODIAC
{
	WORD				wPosX			: 16;	
	WORD				wPosZ			: 16;

	BYTE				bQuantity		: 7;		
	BYTE				bType			: 1;

	unsigned __int64	wMapId			: 14;		
	unsigned __int64	bLayer			: 5;		
	unsigned __int64	bInvIndex		: 6;		
	unsigned __int64	bReserved		: 39;
}; 

struct ITEM_SPECIAL
{
	BYTE	bQuantity		: 4;
	BYTE	bReserved		: 4;
};

struct ITEM_CONSUMABLE
{
	DWORD	dwEndDate		: 32;		// 아이템 끝나는 시간을 기록한다.
	BYTE	bInvIndex		: 7;		// 사용했을때의 인벤위치 : 사용한 후에는 이동하지 못한다. base : 1부터	
	BYTE	bQuantity		: 1;	
};

struct ITEM_SUPPLIES
{
	BYTE	bQuantity		: 7;
	BYTE	bReserved		: 1;
};

struct ITEM_MAGICARRAY
{
	BYTE	bQuantity		: 4;
	BYTE	bReserved		: 4;
};

struct ITEM_MATERIALS
{
	BYTE	bQuantity		: 1;
	BYTE	bReserved		: 7;
};

struct ITEM_MIX_UPGRADE
{
	BYTE	bQuantity		: 7;
	BYTE	bReserved		: 1;
};

struct ITEM_MAGICFIELD_ARRAY
{
	BYTE	bQuantity		: 7;
	BYTE	bReserved		: 1;
};

struct ITEM_UPGREADE
{
	BYTE	bQuantity		: 1;
	BYTE	bReserved		: 7;
};

struct ITEM_LIQUID
{
	BYTE	bQuantity		: 1;
	BYTE	bReserved		: 7;
};

struct ITEM_EDITION
{
	BYTE	bQuantity		: 1;
	BYTE	bReserved		: 7;
};

struct ITEM_LAND
{
	DWORD	dwStartTime		: 32;

	union
	{
		DWORD	dwPlayTime		: 32;
		DWORD	dwServerTime	: 32;	
	};
	
	WORD	wBaseItemID		: 16;
};


class DECLSPECIFIER CItem
{
public:

	WORD			m_wItemID;				//<-- 2Byte
	ITEM_SERIAL		m_Serial;				//<-- 8Byte

	union						
	{
		ITEM_WEAPON				m_Item_Weapon;
		ITEM_ARMOR				m_Item_Armor;
		ITEM_SUPPLIES			m_Item_Supplies;
		ITEM_MONEY				m_Item_Money;		
		ITEM_RIDEOBJECT			m_Item_RideObject;		
		ITEM_SPECIAL			m_Item_Special;
		ITEM_ZODIAC				m_Item_Zodiac;
		ITEM_GUARDIAN			m_Item_Guardian;
		ITEM_MAGICARRAY			m_Item_MagicArray;
		ITEM_MATERIALS			m_Item_Materials;
		ITEM_MIX_UPGRADE		m_Item_Mix_Upgrade;
		ITEM_MAGICFIELD_ARRAY	m_Item_MagicField_Array;
		ITEM_CONSUMABLE			m_Item_Consumable;
		ITEM_UPGREADE			m_Item_UpGreade;
		ITEM_LIQUID				m_Item_LiQuid;
		ITEM_EDITION			m_Item_Edition;
		ITEM_BAG				m_Item_Bag;
		ITEM_LAND				m_Item_Land;
	};										//<-- 13 Byte
	
	union
	{
		ITEM_SLOT		m_bSlot;				//<-- 1 Byte
		BYTE			bLevelH;				//<-- for skill level, high bit
	};

	union
	{
		ITEM_ATTR		wAttr[8];				//<-- 16 Byte	
		struct _Guardian
		{
			WORD			wStat[5];				//<-- 16 Bytes for Guardian Stat : 최덕석 2005.03.28
			BYTE			bSkill[5];				//<-- 5 Bytes for Guardian Skill
			BYTE			bLevelL;				//<-- for skill level, low bit
		}Guardian;
	};

	WORD				GetID()	const;					
	void				SetID(WORD wItemID);			
	void				SetSerialNum(ITEM_SERIAL* pSerial);
	void				SetSerialNum(ITEM_SERIAL serial);
	const ITEM_SERIAL*	GetSerial()	const;				
	BYTE				GetQuantity() const;
	void				SetQuantity(BYTE bQuantity);
	int					GetOptionCount();
	CBaseItem*			GetBaseItem() const;
	void				SetGuardianEggCompleteTime(DWORD dwCompleteTime);
	DWORD				GetKind();
	BYTE				GetWeaponKind();
	ITEM_WEAPON_TYPE	GetWeaponType();
	BOOL				IsOverlapItem();
	BYTE				GetType();
};


#pragma pack(pop)