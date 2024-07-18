//==================================================//
// Code by Jang.						2002.12.9	//
// Object : Interface.								//
//==================================================//
#pragma once

#include "Struct.h"

extern VECTOR3	v3Pos;

class CMenu;

enum
{
	__SERVER_TYPE_CHARACTERSELECT__	= 1,
	__SERVER_TYPE_WORLD__			= 2,
	__SERVER_TYPE_DUNGEON__			= 3,
	__SERVER_TYPE_VILL__			= 4,
	__SERVER_TYPE_LOGIN__			= 5,
};


enum
{
	COMMAND_WND			=		0,
	INTERFACE_USER		=		1,
	ITEM_WND			=		2,
	CHAR_WND			=		3,
	SKILL_WND			=		4,
	GAMEMENU_WND		=		5,
	MINIMAP_WND			=		6,
	CHAT_WND			=		7,
	CHATLIST_WND		=		8,
	ITEMSHOP_WND		=		9,
	NPCDUNGEON_INFO_WND	=		10,
	QUANTITY_WND		=		11,
	STORE_WND			=		12,
	DUNGEONMANAGER_WND	=		13,
	DUNGEONINFO_WND		=		14,
	BANK_WND			=		15,
	GUILD_WND			=		16,
	GUILDEX_WND			=		17,
	GUILDJOIN_WND		=		18,
	GUILDRANK_WND		=		19,
	GUILDEDIT_WND		=		20,
	PARTYJOIN_WND		=		21,
	DEFAULT_WND			=		22,
	HELP_WND			=		23,
	ERROR_WND			=		24,
	ITEMTRADE_WND		=		25,
	ITEMTRADESHOP_WND	=		26,
	OK_WND				=		27,
	GROUP_WND			=		28,
	PLAYERSHOP_WND		=		29,
	PARTYPORTAL_WND		=		30,
	MATCH_WND			=		31,
	DUNGEONSIEGEINFO_WND=		32,
	ITEMUPGRADE_WND		=		33,
	EVENTDUNGEON_WND	=		34,
	NOTICE_WND			=		35,
	PARTYREQUEST_WND	=		36,
	INITITEM_WND		=		37,
	GUILDREQUEST_WND	=		38,
	MESSENGER_WND		=		39,
	GUILDDELETE_WND		=		40,
	GAME_NOTICE_WND		=		41,
	MESSENGERDELETE_WND	=		42,
	GUILDMASTER_WND		=		43,
	WORLD_WND			=		44,
	CHARDIE_WND			=		45,
	GUILDPORTAL_WND		=		46,
	GUILDWAR_WND		=		47,
	RANKNPC_WND			=		48,
	RANK_WND			=		49,
	EVENT_NPC_WND		=		50,
	EVENT_NPC_SUB_WND	=		51,
	ITEMUPGRADE_SUB_WND	=		52,
	ITEMUPGRADE_HELP_WND=		53,
	CHAR_MOVE_WND		=		54,
	CHAR_MOVERT_WND		=		55,
	CHAR_MOVEEDIT_WND	=		56,
	PARTY_BOARD_WND		=		57,
	PARTY_ENTRY_WND		=		58,
	PARTY_HELP_WND		=		59,
	PARTY_BOARDINFO_WND	=		60,
	BLOCK_WND			=		61,
	GAMEDEFAULT_WND		=		62,
	ITEMMAKING_WND		=		63,
	ITEMMAKINGSUB_WND	=		64,
	ITEMMAKINGHELP_WND	=		65,
	TOPDUNGEONHELP_WND	=		66,
	GUARDIAN_WND		=		67,	
	PARTYMATCHING_WND	=		68,
	GUILDWAR_REQUEST_WND=		69,
	GUILDWAR_FINALSETTING_WND = 70,
	GUILDWAR_INVITE_WND =		71,
	EXIT_WND,			
	CUR_INTERFACE
};

class CInterface
{
public:		
	
	BYTE	m_byServer;		
	BYTE	m_byIndex;
	BYTE	m_byOrder;
	
	BOOL	m_bIconChk;
	BOOL	m_bActiveChk;
	BOOL	m_bInit;
	BOOL	m_bClick;
	BOOL	m_bSoundChk;

	BOOL	m_bSetWnd;

	CMenu*	m_pMenu[CUR_INTERFACE];	

	int		m_nSize[CUR_INTERFACE*2];

	char	m_szName[CUR_INTERFACE][0xff];
	BOOL	LoadInterfaceComponentInfoCDB(char* pFileName);	
	BOOL	LoadInterfaceFrameInfoCDB(char* pFileName);

private:

	static CInterface* c_pThis;

public:

	static CInterface*	GetInstance()		{ if(!c_pThis) c_pThis = new CInterface; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	BOOL	GetClick();
	BOOL	InitInterface(BYTE byServer);	
	
	void	Render();
	void	Remove();
	void	SetUp(BOOL bUp);
	void	SetMiniMapPos();
	void	SetClick(BOOL bClick);
	
	int		InterfaceCheck();

	BOOL	GetMouseIcon();
	BOOL	GetIconChk();

	void	SetWnd(BYTE bWnd);
	void	SetIconChk(BOOL bIconChk);	
	void	OptionConfig(BOOL bSave);	
	void	ItemInfoRender(CItem* pItem, BOOL isEquip, BOOL bPrice = FALSE, DWORD dwPrice = 0, BYTE byZipCode = 0, BYTE byOption = 0);	
	void	RenderInfoBox(float fPosX, float fPosY, float fSizeX, float fSizeY, int nOrder);
	void	GetDueDayText(char* pszItemInfo, DWORD dwDueDay);
	void	GetLandTimeText(char* pszItemInfo, CItem* pItem);
	BOOL	GetSetWnd();
	
	void	ItemInfoRender_Weapon(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo, DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize);
	void	ItemInfoRender_Armor(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo, DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize, BYTE byZipCode);
	void	ItemInfoRender_Guardian(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
	void	ItemInfoRender_Supplies(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize );
	void	ItemInfoRender_Consumable(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
	void	ItemInfoRender_Zodiac(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize);
	void	ItemInfoRender_Default(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo,DWORD* pdwTextColor, int* nMaxSize);
	void	ItemInfoRender_Option(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize, int* iOption);
	void	ItemInfoRender_SetItemBonusOption(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize, BOOL isEquip, DWORD dwItemSetID, DWORD dwSetNum, LPSET_ITEM_INFO pSetItemInfo);
	
	CInterface();
	virtual ~CInterface();
};
