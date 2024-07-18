#pragma once


#include "stdafx.h"
#include "../CommonServer/AbyssHash64.h"
#include "../CommonServer/CommonHeader.h"
#include "ItemMove.h"
#include "OnlyList.h"
#include "Define.h"
#include "GameDefine.h"
#include "ObjectDefine.h"


const DWORD WM_SOCKET_EVENT			=	WM_USER + 0x7000;


struct DESC_BASENETWORK
{
	WORD		wSockEventWinMsgID;
    void		(*ReceivedMsg)(DWORD dwInex,char* pMsg,DWORD dwLen);
	void		(*OnDisconnect)(DWORD dwInex);
	void		(*OnConnect)(DWORD dwInex);
};

struct SMSG
{
	char		szMessage[0xff];
	DWORD		sdwColor;
	int			snSize;
};

struct	CHARMOVEINFO
{
	BYTE		byNum;
	BYTE		byResult;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
};

struct	CHARMOVEINFOEIDT
{
	CHARMOVEINFO	sCharMoveInfo;

	char			szName[MAX_CHARACTER_NAME_LENGTH];
};

// 대결 정보 
typedef	struct match_mark_t
{
	int		iRecords;	// 전적
	int		iWin;		// 승리
	int		iLose;		// 패배
} MATCH_MARK, * LPMATCH_MARK;

enum ENSKILLINFO
{
	SKILLINFO_NONE	= 0,
	SKILLINFO_LEVEL	= 1,
	SKILLINFO_MIN	= 2,
	SKILLINFO_MAX	= 3,
	SKILLINFO_SP	= 4,
	SKILLINFO_PBT	= 5,
	SKILLINFO_DUR	= 6,
	SKILLINFO_CPS	= 7,
};


typedef struct __tag_guild_display__
{
	char	szRankName[MAX_GUILD_NAME_LENGTH];
	char	szClassName[8];
	char	szLevName[10];

	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char	szNickName[MAX_GUILD_NAME_LENGTH];
	
	BOOL	bNickName;

}GUILD_DISPLAY, *LP_GUILD_DISPLAY;

typedef struct __tag_special_effect__
{
	GXOBJECT_HANDLE	hHandle;

}SPECIAL_EFFECT, *LP_SPECIAL_EFFECT;

typedef struct __tag_sScript__
{
	ENCOMMAND	enCommand;

	char		szId[MAX_ID_LENGTH];
	char		szMessage[__MAX_INPUT_BUFFER_SIZE__];
	
	char		szParam[5][__MAX_INPUT_BUFFER_SIZE__];
	
}SSCRIPT, *LPSSCRIPT;

typedef struct __tag_sMessageRQ__
{
	char		szId[MAX_ID_LENGTH];	
	char		szMessage[__MAX_INPUT_BUFFER_SIZE__];
	char		szTime[__MAX_INPUT_BUFFER_SIZE__];

}SMESSAGERQ, *LPMESSAGERQ;

typedef struct GUILD_OFFLINE_USER
{
	DWORD	dwUserIndex;

	char	szName[MAX_CHARACTER_NAME_LENGTH];
	
	WORD	wClass;
	DWORD	dwLevel;
	BYTE	byRank;

	char	szNickName[MAX_CHARACTER_NAME_LENGTH];

}GUILD_OFFLINE_USER, *LP_GUILD_OFFLINE_USER;

struct SWSCHATMSG
{
	int			snSize;
	char		szDungeon[__MAX_INPUT_BUFFER_SIZE__];
	DWORD		sdwChatColor;
};

typedef struct __tag_mouse_interface__
{
	POINT		spMousePos;
	BOOL		sbLeft;
	BOOL		sbRight;

}SINTERFACEMOUSE, *LPINTERFACEMOUSE;

typedef	struct __tag_map_data__
{
	float		sfMaxXLenght;
	float		sfMaxZLenght;

}SMAPDATA, *LPMAPDATA;

struct SDGCHATMSG
{
	int		sTimeIndex;
	int		snSize;
	char	szDungeon[__MAX_INPUT_BUFFER_SIZE__];
	
	DWORD	sdwColor;
	BOOL	sbActive;
};

typedef struct __tag_skill_table__
{
	BYTE	bSKillLevel;
	DWORD	dwSKillId;

}SSKILL_TABLE, *LPSKILL_TABLE;

typedef struct __tag_skill_level__
{
	BYTE		sbSkill_Lev[MAX_SKILL];

}SSKILL_LEV, LPSKILL_LEV;

typedef struct __tag_key_config__
{
	int		snKey[MAX_KEY];						// 설정된 키.
	char	szKeyInfo[MAX_KEY][128];

}SKEY_CONFIG, *LPKEY_CONFIG;

typedef struct __tag_dg_chat__
{
	int		nSize;
	char	szChat[64];

}SDGCHAT, *LPDGCHAT;

typedef struct __tag_key_info__
{
	int		nIndex;

	char	szInfo[128];
	
}SKEYINFO, *LP_KEY_INFO;

typedef struct __tag_item_chktype__
{
	int		nItemIndex;
	BYTE	byItemZipCode;

}ITEM_CHKTYPE, *LP_ITEM_CHKTYPE;

typedef struct __tag_guild_info__
{
	DWORD		dwGuildId;

	char		szGuildName[MAX_GUILD_NAME_LENGTH];

	char		szRankName[4][MAX_GUILD_NAME_LENGTH];

	BYTE		byType;
	BYTE		byCount;
		
	
	WORD		wLevel;
	DWORD		dwExp;
	DWORD		dwPoint;	
	
	BYTE		byMember[4];
	
}GUILD_INFO, *LP_GUILD_INFO;

typedef struct __tag_guild_war__
{
	DWORD		dwGuildId;
	WORD		wPoint1;
	WORD		wPoint2;
	
	time_t		sTime;

	char		szGuildName[MAX_GUILD_NAME_LENGTH];

}SGUILD_WAR, *LP_GUILD_WAR;

typedef struct __tag_party_board__
{
	DWORD		dwUserIndex;
	DWORD		dwPartyId;
	WORD		wDungeonID;

	char		szName[MAX_GUILD_NAME_LENGTH];
	char		szMemo[32];

}SPARTY_BOARD, *LP_PARTY_BOARD;

typedef struct __tag_messenger_user__
{
	DWORD	dwUserIndex;

	char	szName[MAX_GUILD_NAME_LENGTH];

	BYTE	byType;
	
	WORD	wClass;
	
	BYTE	byStatus;

}MESSENGER_USER, *LP_MESSENGER_USER;

typedef struct __tag_guild_data__
{
	DWORD	dwGuildId;

	char	szGuildName[MAX_GUILD_NAME_LENGTH];

}GUILD_DATA, *LP_GUILD_DATA;

typedef struct __tag_guild_user__
{
	char	szCharacterName[MAX_GUILD_NAME_LENGTH];
	char	szNickName[MAX_GUILD_NAME_LENGTH];

	BYTE	byRank;

	DWORD	dwLev;
	WORD	wClass;

	DWORD	dwGuildId;

	DWORD	dwUserIndex;

}GUILD_USER, *LP_GUILD_USER;

struct MAP_TILE;
typedef struct __tag_party__
{		
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	
	//--  For Dungeon
	WORD	wClass;
	DWORD	dwUserIndex;
	DWORD	dwResourceId;
	BYTE	byLeader;
	WORD	wLevel;	
	WORD	wHp;
	WORD	wMaxHp;
	//--

	//--   For World
	BYTE				m_bStatus;
	BYTE				m_byItemType;		
	VECTOR3				m_v3CurPos;
	VECTOR3				m_v3Direction;
	MAP_TILE*			m_pDestTile;
	MAP_TILE*			m_pCurTile;
	OBJECT_HANDLE_DESC	m_hPlayer;			// 케릭터 바디 핸들.
	OBJECT_HANDLE_DESC	m_hPlayerHead;		// 머리 핸들 
	OBJECT_HANDLE_DESC	m_hPlayerRHand;		// 오른손 무기 핸들 
	OBJECT_HANDLE_DESC	m_hPlayerLHand;		// 왼손 무기 핸들 
	OBJECT_HANDLE_DESC	m_hPlayerHelmet;	// 헬멧 
	//--

	BYTE	GetStatus()	{	return m_bStatus; }
	void	SetStatus(BYTE bStatus)		{	m_bStatus = bStatus;	}

	
}PARTY_USER, *LP_PARTY_USER;

/*
struct WORLD_PARTY_USER
{
	// GXOBJECT_HANDLE 
	OBJECT_HANDLE_DESC	m_hPlayer;			// 케릭터 바디 핸들.
	OBJECT_HANDLE_DESC	m_hPlayerHead;		// 머리 핸들 
	OBJECT_HANDLE_DESC	m_hPlayerRHand;		// 오른손 무기 핸들 
	OBJECT_HANDLE_DESC	m_hPlayerLHand;		// 왼손 무기 핸들 
	OBJECT_HANDLE_DESC	m_hPlayerHelmet;	// 헬멧 
	
	LP_PARTY_USER		pInfoForDungeon;	// 던전에서 쓰는 실제 Party User 구조체 
	
	char*   GetName()		{	return pInfoForDungeon->szName;			}
	DWORD	GetCharIndex()	{	return pInfoForDungeon->dwUserIndex;	}	
};
*/

typedef struct __tag_chat_msg__
{
	char	szText[__MAX_MESSGAE__];

	DWORD	dwColor;

}CHAT_MSG, *LP_CHAT_MSG;

typedef struct __tag_party_display__
{
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	char	szClassName[16];

}PARTY_DISPLAY, *LP_PARTY_DISPLAY;


typedef struct __tag_store_info__
{
	WORD	wItemId;

	char	szItemName[64];
	char	szPriceName[64];
	char	szOption[10][64];

	DWORD	dwNameColor;
	DWORD	dwColor[10];
	BYTE	byOptionCnt;

}SSTORE_INFO, *LP_STORE_INFO;


typedef struct __tag_skill_dp_info__
{
	BOOL		bActive;

	ENSKILLINFO	enSkillInfo[__MAX_SKILLINFO_VAL__];
	
	char		szInfo[0xff];

	BYTE		byIndex;

	WORD		wSkillLevel;

}SSKILL_DPINFO, *LP_SKILL_DPINFO;

#pragma pack(push, 1)

typedef struct __tag_item_option__
{
	WORD	wId;
	BYTE	byCount;
	BYTE	byDisplayFlag[4];	// 디스플레이 방식이 지정되있다.	by yak
	char	szOption[4][64];

}ITEM_OPTION, *LP_ITEM_OPTION;


struct ITEM
{
	OBJECT_HANDLE_DESC	hItem;
	VECTOR3				v3ItemPos;
	CItem				Item;
	BYTE				bSectionNum;
};

typedef struct __tag_level_exp__
{
	BYTE		byLevel;
	DWORD		dwExp;

}SLEVEL_EXP, *LP_LEVEL_EXP;

typedef struct __tag_item_store__
{
	WORD	wItemId;
	BYTE	byType;	
	WORD	wMapId;
	
}ITEM_STORE, *LP_ITEM_STORE;

typedef struct __tag_item_resource__
{
	WORD				wID;
	
	char				szIconFileName[40];
	char				szModFileName[40];

	BYTE				byIconStartIndex;
	BYTE				byIconCount;
	
	WORD				wModCount;

	BYTE				byResourceType;
	BYTE				byPreLoad;
	BYTE				byAnimation;

}SITEM_RESOURCE, *LP_ITEM_RESOURCE;

typedef struct __tag_skill_list__
{
	COnlyList*	pActiveList;
	COnlyList*	pPassiveList;
	COnlyList*	pOverDriveList;
	COnlyList*	pMasteryList;

}SSKILL_LIST, *LP_SKILL_LIST;

typedef struct __tag_skill_list_manager__
{
	SSKILL_LIST	pSkillList[5];

	BYTE byLeftSkillCnt;
	BYTE byRightSkillCnt;
	BYTE byLeftSkill[MAX_SKILL];
	BYTE byRightSkill[MAX_SKILL];	

	IDISpriteObject* pSpr[MAX_SKILL];

}SSKILL_LIST_MANAGER, *LP_SKILL_LIST_MANAGER;


typedef struct __tag_skill_resource__
{
	WORD	wId;
	
	BYTE	bySkillType;
	
	char	szFileName[20];

	WORD	wIndex;

	char	szFileNameAct[20];

	WORD	wIndexAct;

	BYTE	byType;

	BYTE	byTypeIndex;

	BYTE	byTypePos;

}SSKILL_RESOURCE, *LP_SKILL_RESOURCE;

typedef struct __tag_pre_load
{
	char	szFileName[50];

	BYTE	byType;				// 0번 Item, 1번 Effect, 2번 DamageNumber, 3번 Character, 4번 UI, 5번 NPC, 6번 Monster.

}PRE_LOAD, *LPPRE_LOAD;

typedef struct __tag_skill_resourceex__
{
	WORD	wId;
	
	BYTE	bySkillType;
	
	char	szFileName[20];

	WORD	wIndex;

	char	szFileNameAct[20];

	WORD	wIndexAct;

	BYTE	byType;

	BYTE	byTypeIndex;

	BYTE	byTypePos;	

	IDISpriteObject*	pSpr;
	
	IDISpriteObject*	pSprAct;

}SSKILL_RESOURCE_EX, *LP_SKILL_RESOURCE_EX;

typedef struct __tag_item_resourceex__
{
	WORD				wID;
	
	char				szIconFileName[40];
	char				szModFileName[40];

	BYTE				byIconStartIndex;
	BYTE				byIconCount;
	
	WORD				wModCount;

	BYTE				byResourceType;
	BYTE				byPreLoad;
	BYTE				byAnimation;

	IDISpriteObject*	pSpr;

}SITEM_RESOURCE_EX, *LP_ITEM_RESOURCE_EX;

typedef	struct __tag_help_info__
{
	WORD	wId;

	char	szText[64];

	WORD	wLeft;
	WORD	wTop;
	WORD	wLinkTextID;
	BYTE	byType;

}SHELP_INFO, *LP_HELP_INFO;




typedef struct message_t
{
	unsigned long	dwId;
	char			szMessage[255];
} MESSAGE, * LPMESSAGE;

typedef struct message_w_t
{
	unsigned long	dwId;
	char			szMessage[255*2];
} MESSAGE_W, * LPMESSAGE_W;

typedef struct emoticon_t
{
	unsigned long	dwId;
	char			szMessage[255];
//	int				iMod;
} EMOTICON, *LPEMOTICON;

struct BASE_CLASS_INFO
{
	int		iMax_Aura;
	int		iMax_Divine;
	int		iMax_Summon;
	int		iMax_Chakra;
	int		iMax_Magic;
};

typedef struct __tag_groupinfo__
{
	BYTE	byType;
	BYTE	byLevel;
	WORD	wMemeber[5];
	DWORD	dwExp;
	BYTE	byAura;
	BYTE	byMake_Size;

}SGROUP_INFO, *LP_GROUP_INFO;

typedef struct __tag_groupinfo_hash__
{
	SGROUP_INFO	sGroupInfo[2];

}SGROUPINFO_TABLE, *LP_GROUPINFO_TABLE;



struct PLAYERSHOPINFO
{
	char		szTitle[MAX_PLAYER_SHOP_TITLE];					// 노점상 제목	
	DWORD		m_dwCustomSellPrice[MAX_PLAYER_SHOP_INV];		// 노점상 만들때 팔 가격을 조정한다.
	
	CItem		cItem[MAX_PLAYER_SHOP_INV];						// 노점상 아이템목록
};

struct DUNGEONPRODUCTIONITEMMINMAX
{
	BYTE	byID;
	WORD	wItemIDMin;
	WORD	wItemIDMax;
	WORD	wItemIDDefault;
};

struct CPTable_Value
{
	WORD	wID;
	WORD	wValue;
};
struct CPTable	// 써버랑 클라이언트랑 구조체가 틀리다. 주의해라.
{
	WORD	wID;
	char	szKorName[50];
	char	szEngName[35];
	char	szDescription[128];
	BYTE	bClass;
	WORD	wRate;
	WORD	wAni1;
	WORD	wAni2;
	WORD	wSound1;
	WORD	wSound2;
	WORD	wApply_Time;
	BYTE	bPartyUse;
	CPTable_Value	sCPTable_Value[5];
};


extern CPTable	g_pCPTable[MAX_CP_TABLE];


struct NPC_TABLE
{
	DWORD dwID;
	char szName[ MAX_NPC_NAME_LENGTH ];
	DWORD dwType;
	char  szMsg[ 3 ][ MAX_NPC_MSG_LENGTH ];
};

extern NPC_TABLE g_NPCTable[MAX_NPC_NUM_PER_VILLAGE];


#pragma pack(pop)

//===========================================================================//
