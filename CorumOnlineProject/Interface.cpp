//==================================================//
// Code by Jang.						2002.12.9	//
//==================================================//
#include "InitGame.h"
#include "Map.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "Ini.h"
#include "Interface.h"
#include "Menu.h"
#include "ItemWnd.h"
#include "CharWnd.h"
#include "GuardianWnd.h"
#include "SkillWnd.h"
#include "GameMenuWnd.h"
#include "DSMiniMap.h"
#include "CommandWnd.h"
#include "ChatWnd.h"
#include "ChatListWnd.h"
#include "UserInterface.h"
#include "ItemShopWnd.h"
#include "NpcDungeonInfoWnd.h"
#include "QuantityWnd.h"
#include "ExitWnd.h"
#include "StoreWnd.h"
#include "cdungeonmanagerwnd.h"
#include "cdungeoninfownd.h"
#include "BankWnd.h"
#include "GuildWnd.h"
#include "GuildExWnd.h"
#include "GuildJoinWnd.h"
#include "GuildRankWnd.h"
#include "GuildEditWnd.h"
#include "PartyJoinWnd.h"
#include "DefaultWnd.h"
#include "HelpWnd.h"
#include "ErrorWnd.h"
#include "ItemTradeWnd.h"
#include "ItemTradeShopWnd.h"
#include "Interface.h"
#include "OkWnd.h"
#include "GroupWnd.h"
#include "playershopwnd.h"
#include "PartyPortalWnd.h"
#include "RequestMatchWnd.h"
#include "DungeonSiegeInfoWnd.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "ItemUpgradeHelpWnd.h"
#include "EventDungeonWnd.h"
#include "NoticeWnd.h"
#include "PartyRequestWnd.h"
#include "InitItemWnd.h"
#include "GuildRequestWnd.h"
#include "MessengerWnd.h"
#include "GuildDeleteWnd.h"
#include "GameDefaultWnd.h"
#include "GameNoticeWnd.h"
#include "CharMoveWnd.h"
#include "message.h"
#include "DungeonTable.h"
#include "MessengerDeleteWnd.h"
#include "GuildMasterWnd.h"
#include "WorldWnd.h"
#include "CharDieWnd.h"
#include "GuildPortalWnd.h"
#include "GuildWarWnd.h"
#include "RankNpcWnd.h"
#include "RankWnd.h"
#include "EventNPCWnd.h"
#include "EventNPCSubWnd.h"
#include "DungeonProcess.h"
#include "CharMoveRtWnd.h"
#include "CharMoveEditWnd.h"
#include "PartyBoardWnd.h"
#include "PartyEntryWnd.h"
#include "PartyHelpWnd.h"
#include "PartyBoardInfoWnd.h"
#include "BlockWnd.h"
#include "ItemMakingWnd.h"
#include "ItemMakingSubWnd.h"
#include "ItemMakingHelpWnd.h"
#include "CodeFun.h"
#include "TopDungeonHelpWnd.h"
#include "LoginWnd.h"
#include "GuildwarRequest.h"
#include "PartyMatchingWnd.h"
#include <time.h>
#include "Chat.h"
#include "PartyMatchingWnd.h"
#include "GuildWarFinalSettingWnd.h"
#include "GuildWarStatusWnd.h"
#include "GuildWarInvite.h"


#define BAG_ITEM_NUM_FRIST	3600
#define BAG_ITEM_NUM_LAST	3700



CInterface* CInterface::c_pThis = NULL;

//==================================================//
// Construction/Destruction.						//
//==================================================//
CInterface::CInterface()
{
	m_byIndex		= 0;
	m_byOrder		= 0;	
	m_bClick		= FALSE;	
	m_bActiveChk	= FALSE;
	m_bIconChk		= FALSE;
	m_bSoundChk		= FALSE;

//	LoadInterfaceComponentInfoCDB("InterfaceComponentInfo.cdb");
}

CInterface::~CInterface()
{
	
}

//==================================================//
// Class Member Function.							//
//==================================================//
BOOL CInterface::LoadInterfaceComponentInfoCDB(char* pFileName)
{
	INTERFACE_COMPONENT_INFO sInterfaceComponentInfo[MAX_INTERFACE_COMPONENT_INFO];
	
	int nTotalSize = DecodeCDBData(GetFile(pFileName, DATA_TYPE_MANAGER), sInterfaceComponentInfo);
	int nMaxNode = nTotalSize / sizeof(INTERFACE_COMPONENT_INFO);	
	
	// 키 설정 데이터 //
	for(int i = 0; i < nMaxNode; i++)
	{
		if( (BANK_WND	== sInterfaceComponentInfo[i].wInterfaceID && __SERVER_TYPE_VILL__ != m_byServer) ||
			(STORE_WND	== sInterfaceComponentInfo[i].wInterfaceID && __SERVER_TYPE_VILL__ != m_byServer))
		{
			continue;
		}
		if(sInterfaceComponentInfo[i].wInterfaceID >= EXIT_WND)
			continue;
		
		switch(sInterfaceComponentInfo[i].byResourceType)
		{
		case INTERFACE_COMPONENT_INFO_RESOUCETYPE_SPRITE:
			m_pMenu[sInterfaceComponentInfo[i].wInterfaceID]->InsertData(
					sInterfaceComponentInfo[i].byCallBackID,
					sInterfaceComponentInfo[i].wResourceID,
					sInterfaceComponentInfo[i].wLeft,
					sInterfaceComponentInfo[i].wTop,
					sInterfaceComponentInfo[i].fScaleX,
					sInterfaceComponentInfo[i].fScaleY,
					sInterfaceComponentInfo[i].wOrder,
					FALSE,
					sInterfaceComponentInfo[i].byPos
			);
			break;
		case INTERFACE_COMPONENT_INFO_RESOUCETYPE_COLLITION_BOX:
			m_pMenu[sInterfaceComponentInfo[i].wInterfaceID]->InsertCheckInterface(
					sInterfaceComponentInfo[i].wLeft, 
					sInterfaceComponentInfo[i].wTop,
					sInterfaceComponentInfo[i].wRight, 
					sInterfaceComponentInfo[i].wBottom,
					sInterfaceComponentInfo[i].byCallBackID, 
					(CHECKTYPE)sInterfaceComponentInfo[i].wResourceID,
					0,
					sInterfaceComponentInfo[i].wOrder
			);
			break;
		case INTERFACE_COMPONENT_INFO_RESOUCETYPE_MESSAGE:
			break;
		default:
			asm_int3();
		}
		
	}

	return TRUE;
}


BOOL CInterface::LoadInterfaceFrameInfoCDB(char* pFileName)
{
	INTERFACE_FRAME_INFO sInterfaceFrameInfo[MAX_INTERFACE_FRAME_INFO];
	
	int nTotalSize = DecodeCDBData(GetFile(pFileName, DATA_TYPE_MANAGER), sInterfaceFrameInfo);
	int nMaxNode = nTotalSize / sizeof(INTERFACE_FRAME_INFO);	
	
	for(int i = 0; i < nMaxNode; i++)
	{
		m_pMenu[sInterfaceFrameInfo[i].wFrameID]->CreateMenu(
			sInterfaceFrameInfo[i].szFrameName
			, sInterfaceFrameInfo[i].wWidth
			, sInterfaceFrameInfo[i].wHeight
			, sInterfaceFrameInfo[i].byType
			, sInterfaceFrameInfo[i].wIndex
			, sInterfaceFrameInfo[i].byActive
			, sInterfaceFrameInfo[i].byScroll);		
	}

	return TRUE;
}


BOOL CInterface::InitInterface(BYTE byServer)
{
	int		nPosX = 0, nPosY = 0;
	char	szInfo[0xff] = {0,};
		
	for(int i = 0; i < CUR_INTERFACE; i++)
	{
		m_nSize[i*2]	= 0;
		m_nSize[i*2+1]	= 0;
		memset(m_szName[i], 0, sizeof(m_szName[i]));
	}
												
	// Interface 정의 //	
	m_pMenu[COMMAND_WND]			= CCommandWnd::GetInstance();
	m_pMenu[INTERFACE_USER]			= CUserInterface::GetInstance();
	m_pMenu[ITEM_WND]				= CItemWnd::GetInstance();
	m_pMenu[CHAR_WND]				= CCharWnd::GetInstance();
	m_pMenu[SKILL_WND]				= CSkillWnd::GetInstance();
	m_pMenu[GAMEMENU_WND]			= CGameMenuWnd::GetInstance();		
	m_pMenu[MINIMAP_WND]			= CMiniMapWnd::GetInstance();
	m_pMenu[CHAT_WND]				= CChatWnd::GetInstance();		
	m_pMenu[CHATLIST_WND]			= CChatListWnd::GetInstance();
	m_pMenu[ITEMSHOP_WND]			= CItemShopWnd::GetInstance();
	m_pMenu[NPCDUNGEON_INFO_WND]	= CNpcDungeonInfoWnd::GetInstance();
	m_pMenu[QUANTITY_WND]			= CQuantityWnd::GetInstance();
	m_pMenu[EXIT_WND]				= CExitWnd::GetInstance();
	m_pMenu[STORE_WND]				= CStoreWnd::GetInstance();
	m_pMenu[DUNGEONMANAGER_WND]		= CDungeonManagerWnd::GetInstance();
	m_pMenu[DUNGEONINFO_WND]		= CDungeonInfoWnd::GetInstance();
	m_pMenu[BANK_WND]				= CBankWnd::GetInstance();
	m_pMenu[GUILD_WND]				= CGuildWnd::GetInstance();
	m_pMenu[GUILDEX_WND]			= CGuildExWnd::GetInstance();
	m_pMenu[GUILDJOIN_WND]			= CGuildJoinWnd::GetInstance();
	m_pMenu[GUILDRANK_WND]			= CGuildRankWnd::GetInstance();
	m_pMenu[GUILDEDIT_WND]			= CGuildEditWnd::GetInstance();
	m_pMenu[PARTYJOIN_WND]			= CPartyJoinWnd::GetInstance();
	m_pMenu[DEFAULT_WND]			= CDefaultWnd::GetInstance();
	m_pMenu[HELP_WND]				= CHelpWnd::GetInstance();
	m_pMenu[ERROR_WND]				= CErrorWnd::GetInstance();
	m_pMenu[ITEMTRADE_WND]			= CItemTradeWnd::GetInstance();
	m_pMenu[ITEMTRADESHOP_WND]		= CItemTradeShopWnd::GetInstance();
	m_pMenu[OK_WND]					= COkWnd::GetInstance();
	m_pMenu[GROUP_WND]				= CGroupWnd::GetInstance();
	m_pMenu[PLAYERSHOP_WND]			= CPlayerShopWnd::GetInstance();
	m_pMenu[PARTYPORTAL_WND]		= CPartyPortalWnd::GetInstance();
	m_pMenu[MATCH_WND]				= CRequestMatchWnd::GetInstance();
	m_pMenu[DUNGEONSIEGEINFO_WND]	= CDungeonSiegeInfoWnd::GetInstance();
	m_pMenu[ITEMUPGRADE_WND]		= CItemUpgradeWnd::GetInstance();
	m_pMenu[EVENTDUNGEON_WND]		= CEventDungeonWnd::GetInstance();
	m_pMenu[NOTICE_WND]				= CNoticeWnd::GetInstance();
	m_pMenu[PARTYREQUEST_WND]		= CPartyRequestWnd::GetInstance();
	m_pMenu[INITITEM_WND]			= CInitItemWnd::GetInstance();
	m_pMenu[GUILDREQUEST_WND]		= CGuildRequestWnd::GetInstance();
	m_pMenu[MESSENGER_WND]			= CMessengerWnd::GetInstance();
	m_pMenu[GUILDDELETE_WND]		= CGuildDeleteWnd::GetInstance();
	m_pMenu[GAME_NOTICE_WND]		= CGameNoticeMessageWnd::GetInstance();
	m_pMenu[MESSENGERDELETE_WND]	= CMessengerDeleteWnd::GetInstance();
	m_pMenu[GUILDMASTER_WND]		= CGuildMasterWnd::GetInstance();
	m_pMenu[WORLD_WND]				= CWorldWnd::GetInstance();
	m_pMenu[CHARDIE_WND]			= CCharDieWnd::GetInstance();
	m_pMenu[GUILDPORTAL_WND]		= CGuildPortalWnd::GetInstance();
	m_pMenu[GUILDWAR_WND]			= CGuildWarWnd::GetInstance();
	m_pMenu[RANKNPC_WND]			= CRankNpcWnd::GetInstance();
	m_pMenu[RANK_WND]				= CRankWnd::GetInstance();
	m_pMenu[EVENT_NPC_WND]			= CEventNPCWnd::GetInstance();
	m_pMenu[EVENT_NPC_SUB_WND]		= CEventNPCSubWnd::GetInstance();
	m_pMenu[ITEMUPGRADE_SUB_WND]	= CItemUpgradeSubWnd::GetInstance();
	m_pMenu[ITEMUPGRADE_HELP_WND]	= CItemUpgradeHelpWnd::GetInstance();
	m_pMenu[CHAR_MOVE_WND]			= CCharMoveWnd::GetInstance();
	m_pMenu[CHAR_MOVERT_WND]		= CCharMoveRtWnd::GetInstance();
	m_pMenu[CHAR_MOVEEDIT_WND]		= CCharMoveEditWnd::GetInstance();
	m_pMenu[PARTY_BOARD_WND]		= CPartyBoardWnd::GetInstance();
	m_pMenu[PARTY_ENTRY_WND]		= CPartyEntryWnd::GetInstance();
	m_pMenu[PARTY_HELP_WND]			= CPartyHelpWnd::GetInstance();
	m_pMenu[PARTY_BOARDINFO_WND]	= CPartyBoardInfoWnd::GetInstance();
	m_pMenu[BLOCK_WND]				= CBlockWnd::GetInstance();
	m_pMenu[GAMEDEFAULT_WND]		= CGameDefaultWnd::GetInstance();
	m_pMenu[ITEMMAKING_WND]			= CItemMakingWnd::GetInstance();
	m_pMenu[ITEMMAKINGSUB_WND]		= CItemMakingSubWnd::GetInstance();
	m_pMenu[ITEMMAKINGHELP_WND]		= CItemMakingHelpWnd::GetInstance();
	m_pMenu[TOPDUNGEONHELP_WND]		= CTopDungeonHelpWnd::GetInstance();
	m_pMenu[GUARDIAN_WND]			= CGuardianWnd::GetInstance();
	m_pMenu[PARTYMATCHING_WND]		= CPartyMatchingWnd::GetInstance();
	m_pMenu[GUILDWAR_REQUEST_WND]	= CGuildWarRequest::GetInstance();
	// 전병렬->
	m_pMenu[GUILDWAR_FINALSETTING_WND] = CGuildWarFinalSettingWnd::GetInstance();		
	m_pMenu[GUILDWAR_INVITE_WND] = CGuildWarInvite::GetInstance();
	// Menu 생성 //
	m_pMenu[EXIT_WND]->CreateMenu("EXIT", EXITWND_SIZE_X, EXITWND_SIZE_Y, __MENU_TYPE_WORLD__, EXIT_WND);
	LoadInterfaceFrameInfoCDB("InterfaceFrameInfo.cdb");
	/*
	m_pMenu[COMMAND_WND]->CreateMenu("COMMAND", COMMANDWND_SIZE_X, COMMANDWND_SIZE_Y, __MENU_TYPE_WORLD__, COMMAND_WND);
	m_pMenu[INTERFACE_USER]->CreateMenu("INTERFACE", 0, 0, __MENU_TYPE_DEFAULT__, INTERFACE_USER, TRUE);
	m_pMenu[ITEM_WND]->CreateMenu("ITEM", ITEMWND_SIZE_X, ITEMWND_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEM_WND);
	m_pMenu[CHAR_WND]->CreateMenu("CHAR", CHARWND_SIZE_X, CHARWND_SIZE_Y, __MENU_TYPE_DEFAULT__, CHAR_WND);
	m_pMenu[SKILL_WND]->CreateMenu("SKILL", SKILLWND_SIZE_X, SKILLWND_SIZE_Y, __MENU_TYPE_DEFAULT__, SKILL_WND);
	m_pMenu[GAMEMENU_WND]->CreateMenu("GAMEMENU", GAMEMENUWND_SIZE_X, GAMEMENUWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GAMEMENU_WND);
	m_pMenu[MINIMAP_WND]->CreateMenu("MINIMAP", DSMINIMAP_SIZE_X, DSMINIMAP_SIZE_Y, __MENU_TYPE_DEFAULT__, MINIMAP_WND, TRUE);
	m_pMenu[CHAT_WND]->CreateMenu("CHAT", CHATWND_SIZE_X, CHATWND_SIZE_Y, __MENU_TYPE_DEFAULT__, CHAT_WND);
	m_pMenu[CHATLIST_WND]->CreateMenu("CHATLIST", CHATLISTWND_SIZE_X, CHATLISTWND_SIZE_Y, __MENU_TYPE_DEFAULT__, CHATLIST_WND);
	m_pMenu[ITEMSHOP_WND]->CreateMenu("ITEMSHOP", ITEMSHOPWND_SIZE_X, ITEMSHOPWND_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEMSHOP_WND);
	m_pMenu[NPCDUNGEON_INFO_WND]->CreateMenu("NPCDUNGEONINFO", NPCDUNGEONINFOWND_SIZE_X, NPCDUNGEONINFOWND_SIZE_Y, __MENU_TYPE_DEFAULT__, NPCDUNGEON_INFO_WND);
	m_pMenu[QUANTITY_WND]->CreateMenu("QUANTITY", QUANTITYWND_SIZE_X, QUANTITYWND_SIZE_Y, __MENU_TYPE_CHARACTER__, QUANTITY_WND);
	m_pMenu[STORE_WND]->CreateMenu("STORE", STOREWND_SIZE_X, STOREWND_SIZE_Y, __MENU_TYPE_DEFAULT__, STORE_WND);
	m_pMenu[DUNGEONMANAGER_WND]->CreateMenu("DUNGEONMANAGER", DUNGEONMANAGER_SIZE_X, DUNGEONMANAGER_SIZE_Y, __MENU_TYPE_DEFAULT__, DUNGEONMANAGER_WND);
	m_pMenu[DUNGEONINFO_WND]->CreateMenu("DUNGEONINFO", DUNGEONINFOWND_SIZE_X, DUNGEONINFOWND_SIZE_Y, __MENU_TYPE_WORLD__, DUNGEONINFO_WND);
	m_pMenu[BANK_WND]->CreateMenu("BANK", BANKWND_SIZE_X, BANKWND_SIZE_Y, __MENU_TYPE_DEFAULT__, BANK_WND);
	m_pMenu[GUILD_WND]->CreateMenu("GUILD", GUILDWND_SIZE_X, GUILDWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILD_WND);
	m_pMenu[GUILDEX_WND]->CreateMenu("GUILDEX", GUILDEXWND_SIZE_X, GUILDEXWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDEX_WND);
	m_pMenu[GUILDJOIN_WND]->CreateMenu("GUILDJOIN", GUILDJOINWND_SIZE_X, GUILDJOINWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDJOIN_WND);
	m_pMenu[GUILDRANK_WND]->CreateMenu("GUILDRANK", GUILDRANKWND_SIZE_X, GUILDRANKWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDRANK_WND);
	m_pMenu[GUILDEDIT_WND]->CreateMenu("GUILDEDIT", GUILDEDITWND_SIZE_X, GUILDEDITWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDEDIT_WND);
	m_pMenu[PARTYJOIN_WND]->CreateMenu("PRATYJOIN", PARTYJOINWND_SIZE_X, PARTYJOINWND_SIZE_Y, __MENU_TYPE_DEFAULT__, PARTYJOIN_WND);
	m_pMenu[DEFAULT_WND]->CreateMenu("DEFAULT", DEFAULTWND_SIZE_X, DEFAULTWND_SIZE_Y, __MENU_TYPE_CHARACTER__, DEFAULT_WND);
	m_pMenu[HELP_WND]->CreateMenu("HELP", HELPWND_SIZE_X, HELPWND_SIZE_Y, __MENU_TYPE_WORLD__, HELP_WND);
	m_pMenu[ERROR_WND]->CreateMenu("ERROR", ERRORWND_SIZE_X, ERRORWND_SIZE_Y, __MENU_TYPE_CHARACTER__, ERROR_WND);
	m_pMenu[ITEMTRADE_WND]->CreateMenu("ITEMTRADE", ITEMTRADEWND_SIZE_X, ITEMTRADEWND_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEMTRADE_WND);
	m_pMenu[ITEMTRADESHOP_WND]->CreateMenu("ITEMTRADESHOP", ITEMTRADESHOPWND_SIZE_X, ITEMTRADESHOPWND_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEMTRADESHOP_WND);
	m_pMenu[OK_WND]->CreateMenu("OK", OKWND_SIZE_X, OKWND_SIZE_Y, __MENU_TYPE_WORLD__, OK_WND);
	m_pMenu[GROUP_WND]->CreateMenu("GROUP", GROUPWND_SIZE_X, GROUPWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GROUP_WND);
	m_pMenu[PLAYERSHOP_WND]->CreateMenu("PLAYERSHOP", PLAYERSHOPWND_SIZE_X, PLAYERSHOPWND_SIZE_Y, __MENU_TYPE_DEFAULT__, PLAYERSHOP_WND);
	m_pMenu[PARTYPORTAL_WND]->CreateMenu("PARTYPORTAL", PARTYPORTALWND_SIZE_X, PARTYPORTALWND_SIZE_X, __MENU_TYPE_DEFAULT__, PARTYPORTAL_WND);
	m_pMenu[MATCH_WND]->CreateMenu("MATCH", MATCHWND_SIZE_X, MATCHWND_SIZE_Y, __MENU_TYPE_DEFAULT__, MATCH_WND);
	m_pMenu[DUNGEONSIEGEINFO_WND]->CreateMenu("DUNGEONSIEGEINFO", DUNGEONSIEGEINFOWND_SIZE_X, DUNGEONSIEGEINFOWND_SIZE_Y, __MENU_TYPE_DEFAULT__, DUNGEONSIEGEINFO_WND);
	m_pMenu[ITEMUPGRADE_WND]->CreateMenu("ITEMUPGRADE", ITEMUPGRADE_SIZE_X, ITEMUPGRADE_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEMUPGRADE_WND);
	m_pMenu[EVENTDUNGEON_WND]->CreateMenu("EVENTDUNGEON", EVENTDUNGEONWND_SIZE_X, EVENTDUNGEONWND_SIZE_Y, __MENU_TYPE_DEFAULT__, EVENTDUNGEON_WND);
	m_pMenu[NOTICE_WND]->CreateMenu("NOTICE", NOTICEWND_SIZE_X, NOTICEWND_SIZE_Y, __MENU_TYPE_DEFAULT__, NOTICE_WND);
	m_pMenu[PARTYREQUEST_WND]->CreateMenu("PARTYREQUEST", PARTYREQUESTWND_SIZE_X, PARTYREQUESTWND_SIZE_Y, __MENU_TYPE_DEFAULT__, PARTYREQUEST_WND);
	m_pMenu[INITITEM_WND]->CreateMenu("INITITEM", INITITEMWND_SIZE_X, INITITEMWND_SIZE_Y, __MENU_TYPE_DEFAULT__, INITITEM_WND);
	m_pMenu[GUILDREQUEST_WND]->CreateMenu("GUILDREQUEST", GUILDREQUESTWND_SIZE_X, GUILDREQUESTWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDREQUEST_WND);
	m_pMenu[MESSENGER_WND]->CreateMenu("MESSENGER", MESSENGERWND_SIZE_X, MESSENGERWND_SIZE_Y, __MENU_TYPE_DEFAULT__, MESSENGER_WND);
	m_pMenu[GUILDDELETE_WND]->CreateMenu("GUILDDELETE", GUILDDELETEWND_SIZE_X, GUILDDELETEWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDDELETE_WND);
	m_pMenu[GAME_NOTICE_WND]->CreateMenu("GAMENOTICE", GAMENOTICEWND_SIZE_X, GAMENOTICEWND_SIZE_Y, __MENU_TYPE_WORLD__, GAME_NOTICE_WND);
	m_pMenu[MESSENGERDELETE_WND]->CreateMenu("MESSENGERDELETE", MESSENGERDELETEWND_SIZE_X, MESSENGERDELETEWND_SIZE_Y, __MENU_TYPE_DEFAULT__, MESSENGERDELETE_WND);
	m_pMenu[GUILDMASTER_WND]->CreateMenu("GUILDMASTER", GUILDMASTERWND_SIZE_X, GUILDMASTERWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDMASTER_WND);
	m_pMenu[WORLD_WND]->CreateMenu("WORLD", WORLDWND_SIZE_X, WORLDWND_SIZE_Y, __MENU_TYPE_CHARACTER__, WORLD_WND);
	m_pMenu[CHARDIE_WND]->CreateMenu("CHARDIE", CHARDIEWND_SIZE_X, CHARDIEWND_SIZE_Y, __MENU_TYPE_DEFAULT__, CHARDIE_WND);
	m_pMenu[GUILDPORTAL_WND]->CreateMenu("GUILDPORTAL", GUILDPORTALWND_SIZE_X, GUILDPORTALWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDPORTAL_WND);	
	m_pMenu[GUILDWAR_WND]->CreateMenu("GUILDWAR", GUILDWARWND_SIZE_X, GUILDWARWND_SIZE_Y, __MENU_TYPE_DEFAULT__, GUILDWAR_WND);	
	m_pMenu[RANKNPC_WND]->CreateMenu("RANKNPC", RANKNPCWND_SIZE_X, RANKNPCWND_SIZE_Y, __MENU_TYPE_DEFAULT__, RANKNPC_WND);
	m_pMenu[RANK_WND]->CreateMenu("RANK", RANKWND_SIZE_X, RANKWND_SIZE_Y, __MENU_TYPE_DEFAULT__, RANK_WND);
	m_pMenu[EVENT_NPC_WND]->CreateMenu("EVENTNPC", EVENTNPCWND_SIZE_X, EVENTNPCWND_SIZE_Y, __MENU_TYPE_DEFAULT__, EVENT_NPC_WND);
	m_pMenu[EVENT_NPC_SUB_WND]->CreateMenu("EVENTNPCSUB", EVENTNPCSUBWND_SIZE_X, EVENTNPCSUBWND_SIZE_Y, __MENU_TYPE_DEFAULT__, EVENT_NPC_SUB_WND);
	m_pMenu[ITEMUPGRADE_SUB_WND]->CreateMenu("ITEMUPGRADESUB", OKWND_SIZE_X, OKWND_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEMUPGRADE_SUB_WND);
	m_pMenu[ITEMUPGRADE_HELP_WND]->CreateMenu("ITEMUPGRADEHELP", NOTICEWND_SIZE_X, NOTICEWND_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEMUPGRADE_HELP_WND);
	m_pMenu[CHAR_MOVE_WND]->CreateMenu("CHARMOVE", CHARMOVEWND_SIZE_X, CHARMOVEWND_SIZE_Y, __MENU_TYPE_CHARACTER__, CHAR_MOVE_WND);
	m_pMenu[CHAR_MOVERT_WND]->CreateMenu("CHARMOVERT", CHARMOVERTWND_SIZE_X, CHARMOVERTWND_SIZE_Y, __MENU_TYPE_CHARACTER__, CHAR_MOVERT_WND);
	m_pMenu[CHAR_MOVEEDIT_WND]->CreateMenu("CHARMOVEEDIT", CHARMOVEEDITWND_SIZE_X, CHARMOVEEDITWND_SIZE_Y, __MENU_TYPE_CHARACTER__, CHAR_MOVEEDIT_WND);
	m_pMenu[PARTY_BOARD_WND]->CreateMenu("PARTYBOARD", PARTYBOARDWND_SIZE_X, PARTYBOARDWND_SIZE_Y, __MENU_TYPE_DEFAULT__, PARTY_BOARD_WND);
	m_pMenu[PARTY_ENTRY_WND]->CreateMenu("PARTYENTRY", PARTYENTRYWND_SIZE_X, PARTYENTRYWND_SIZE_Y, __MENU_TYPE_DEFAULT__, PARTY_ENTRY_WND);
	m_pMenu[PARTY_HELP_WND]->CreateMenu("PARTYHELP", PARTYHELPWND_SIZE_X, PARTYHELPWND_SIZE_Y, __MENU_TYPE_DEFAULT__, PARTY_HELP_WND);
	m_pMenu[PARTY_BOARDINFO_WND]->CreateMenu("PARTYBOARDINFO", PARTYBOARDINFO_WND_SIZE_X, PARTYBOARDINFO_WND_SIZE_Y, __MENU_TYPE_DEFAULT__, PARTY_BOARDINFO_WND);
	m_pMenu[BLOCK_WND]->CreateMenu("BLOCK", BLOCKWND_SIZE_X, BLOCKWND_SIZE_Y, __MENU_TYPE_CHARACTER__, BLOCK_WND);
	m_pMenu[GAMEDEFAULT_WND]->CreateMenu("GAMEDEFAULT", GAMEDEFAULTWND_SIZE_X, GAMEDEFAULTWND_SIZE_Y, __MENU_TYPE_CHARACTER__, GAMEDEFAULT_WND);
	m_pMenu[ITEMMAKING_WND]->CreateMenu("ITEMMAKING",ITEMUPGRADE_SIZE_X, ITEMUPGRADE_SIZE_Y, __MENU_TYPE_DEFAULT__,ITEMMAKING_WND);
	m_pMenu[ITEMMAKINGSUB_WND]->CreateMenu("ITEMMAKINGSUB", OKWND_SIZE_X, OKWND_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEMMAKINGSUB_WND);
	m_pMenu[ITEMMAKINGHELP_WND]->CreateMenu("ITEMMAKINGHELP", NOTICEWND_SIZE_X, NOTICEWND_SIZE_Y, __MENU_TYPE_DEFAULT__, ITEMMAKINGHELP_WND);
	m_pMenu[TOPDUNGEONHELP_WND]->CreateMenu("TOPDUNGEONHELP", TOPDUNGEONHELPWND_SIZE_X, TOPDUNGEONHELPWND_SIZE_Y, __MENU_TYPE_DEFAULT__, TOPDUNGEONHELP_WND);
	*/
	m_byServer	= byServer;	

	if(byServer==__MENU_TYPE_WORLD__ || byServer==__MENU_TYPE_CHARACTER__)
	{
		wsprintf(szInfo, "%s\\%s", g_szBasePath, "Interface.ini");		
		
		for(int i = 0; i < CUR_INTERFACE; i++)
		{
			if(i==INTERFACE_USER)
				continue;

			strcpy(m_szName[i], m_pMenu[i]->m_szMenu);
			nPosX = GetPPI(m_szName[i], "POSX", -1, szInfo);
			nPosY = GetPPI(m_szName[i], "POSY", -1, szInfo);
			
			if(nPosX==0 && nPosY==0)			
				m_pMenu[i]->SetPos(512 - m_pMenu[i]->m_fMaxSizeWidth/2, 384 - m_pMenu[i]->m_fMaxSizeHeight/2);
			else
				m_pMenu[i]->SetPos((float)nPosX, (float)nPosY);
		}
	}

	// 생성자에서 로드하도록 한다... 젠장

	// 누가 생성자에서 로드하게 만들었어? 잡히면 뒤져
	
	// sung-han 2005-01-18 파티창이 오류로 투명하게 나타나는 부분 수정
	// .cdb 파일 여러번 불러서 초기화가 제대로 안되는 부분이 문제
	//---------------------------------------------
	for( i=0; i<CUR_INTERFACE ; i++ )
	{
		m_pMenu[i]->RemoveAllData();
	}
	//---------------------------------------------

	LoadInterfaceComponentInfoCDB("InterfaceComponentInfo.cdb");

	// Ini 파일 로드 //	
	g_bInitSoundEnalbe = FALSE;// 사운드 효과음 죽이기
	for(i = 0; i < CUR_INTERFACE; i++ )
	{
		if(m_pMenu[i]->m_byType!=__MENU_TYPE_NONE__)
		{
			if(byServer>=m_pMenu[i]->m_byType)
			{
				m_bInit	= m_pMenu[i]->Init();		

				if(!m_bInit)
					return FALSE;

				m_pMenu[i]->SetActive(m_pMenu[i]->m_bFirst);
			}
			m_nSize[i*2]	= int(m_pMenu[i]->m_fMaxSizeWidth);
			m_nSize[i*2+1]	= int(m_pMenu[i]->m_fMaxSizeHeight);			
		}
	}
	g_bInitSoundEnalbe = TRUE;// 사운드 효과음 살리기
	
	m_byIndex = CUR_INTERFACE;

	for(i = 0; i < m_byIndex; i++)
		m_pMenu[i]->SetOrderIndex(i);	
	
	OptionConfig(FALSE);	
		
	if(byServer==1 || byServer==2)
	{	
		CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();
		g_pExecutive->GetGeometry()->SetShadowFlag(pGameMenuWnd->m_bShadowFlag);
		pGameMenuWnd->SetEffect();
	}	
	
	m_bSoundChk	= TRUE;
	return TRUE;
}


void CInterface::SetWnd(BYTE bWnd)
{	
	if(GetGameStatus()==UPDATE_GAME_PLAY || GetGameStatus()==UPDATE_GAME_WORLD)
	{
		CUserInterface*		pUserInterface		= CUserInterface::GetInstance();		
											
		pUserInterface->m_byOrderCheck = bWnd;

		for(int i = 0; i < CUR_INTERFACE; i++)
		{
			if(m_pMenu[pUserInterface->GetChkOrderIndex()]->GetOrderIndex()<m_pMenu[i]->GetOrderIndex())
			{
				m_pMenu[i]->SetOrderIndex(m_pMenu[i]->GetOrderIndex()-1);
			}
		}
		
		m_pMenu[pUserInterface->GetChkOrderIndex()]->SetOrderIndex(m_byIndex-1);

		for(i = 0; i < CUR_INTERFACE; i++)
		{
			if(m_pMenu[i]->m_bInit==TRUE)
			{
				m_pMenu[i]->SetOrder();
			}
		}

		m_bSetWnd = TRUE;
	}	
}


BOOL CInterface::GetSetWnd()
{
	return !m_bSetWnd;
}


int CInterface::InterfaceCheck()
{
	SetIconChk(FALSE);

	m_bSetWnd = FALSE;
	
	int nIndex	= m_byIndex-1;	

	for(int i = 0; i < m_byIndex; i++)
	{		
		for(int j = 0; j < m_byIndex; j++)
		{
			if ( m_pMenu[j]->m_bInit != TRUE )				continue;
			if ( m_pMenu[j]->GetActive() != TRUE )			continue;
			if ( m_pMenu[j]->GetOrderIndex() != nIndex )	continue;
			if ( m_pMenu[j]->CheckInterface() != TRUE )		continue;

			if(j!=1)
			{
				for(int k = 0; k < m_byIndex; k++)
				{
					if(m_pMenu[j]->GetOrderIndex()<m_pMenu[k]->GetOrderIndex())
					{
						m_pMenu[k]->SetOrderIndex(m_pMenu[k]->GetOrderIndex()-1);
					}
				}

				m_pMenu[j]->SetOrderIndex(m_byIndex-1);

				for(k = 0; k < m_byIndex; k++)
				{
					if(m_pMenu[k]->GetActive()==TRUE)
					{
						m_pMenu[k]->SetOrder();
					}
				}
				
				return j;
			}
			else
			{
				CUserInterface* pUserInterface = CUserInterface::GetInstance();

				for(int k = 0; k < m_byIndex; k++)
				{
					if(m_pMenu[pUserInterface->GetChkOrderIndex()]->GetOrderIndex()<m_pMenu[k]->GetOrderIndex())
					{
						m_pMenu[k]->SetOrderIndex(m_pMenu[k]->GetOrderIndex()-1);
					}
				}

				m_pMenu[pUserInterface->GetChkOrderIndex()]->SetOrderIndex(m_byIndex-1);
				
				for(k = 0; k < m_byIndex; k++)
				{
					if(m_pMenu[k]->GetActive()==TRUE)
					{
						m_pMenu[k]->SetOrder();
					}
				}
				
				return pUserInterface->GetChkOrderIndex();
			}						
		}

		nIndex--;
	} 

	return -1;
}

void CInterface::SetUp(BOOL bUp)
{
	for(int i = 0; i < m_byIndex; i++)
	{
		if(m_pMenu[i]->GetActive())
		{
			m_pMenu[i]->SetUp(bUp);
		}
	}
}

void CInterface::Render()
{		
	if(m_bInit)
	{
		for(int i = 0; i < m_byIndex; i++)
		{	
			if(m_pMenu[i]->m_bInit)
			{
				if(m_pMenu[i]->GetActive())
				{
					m_pMenu[i]->Render();
					m_pMenu[i]->RenderText();
				}
				else
				{
					if(m_pMenu[i]->m_nIndex==GROUP_WND)
					{
						m_pMenu[i]->Render();
					}
				}
			}
		}
	}
}


void CInterface::Remove()
{
	char	szTemp[0xff] = {0,};
	char	szInfo[0xff] = {0,};	
	
	wsprintf(szInfo, "%s\\%s", g_szBasePath, "Interface.ini");

	for(int i = 0; i < CUR_INTERFACE; i++)
	{
		if(i==INTERFACE_USER)
			continue;

		wsprintf(szTemp, " %d", (int)m_pMenu[i]->m_fPosX);
		WritePPSting(m_pMenu[i]->m_szMenu, "POSX", szTemp, szInfo);
		wsprintf(szTemp, " %d", (int)m_pMenu[i]->m_fPosZ);
		WritePPSting(m_pMenu[i]->m_szMenu, "POSY", szTemp, szInfo);		
	}

	// Option Config //
	OptionConfig(TRUE);

	if(m_bInit)
	{	
		for(int i = 0; i < m_byIndex; i++)
		{
			if(m_byServer>=m_pMenu[i]->m_byType)
				m_pMenu[i]->Remove();			
		}
	}	
	
	m_bSoundChk	= FALSE;
}


BOOL CInterface::GetClick()
{
	for(int i = 0; i < m_byIndex; i++)
	{
		m_bClick = m_pMenu[i]->GetClick();

		if(m_bClick)
		{
			return m_bClick;
		}
	}

	return FALSE;
}


void CInterface::SetClick(BOOL bClick)
{
	for(int i = 0; i < m_byIndex; i++)
		m_pMenu[i]->SetClick(bClick);

	m_bClick = bClick;
}


void CInterface::SetMiniMapPos()
{			
	CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();	
	pMiniMapWnd->SetMinimapPos();	
}


BOOL CInterface::GetMouseIcon()
{	
	for(int i = 0; i < CUR_INTERFACE; i++)
	{	
		if(m_pMenu[i]->GetMouseIcon()==TRUE)
		{
			m_pMenu[i]->GetMouseIcon(FALSE);
			return TRUE;
		}	
	}

	return FALSE;
}


BOOL CInterface::GetIconChk()
{
	return m_bIconChk;
}


void CInterface::SetIconChk(BOOL bIconChk)
{
	m_bIconChk = bIconChk;
}


void CInterface::OptionConfig(BOOL bSave)
{
	char	szInfo[0xff] = {0,};
	char	szDefault[0xff] = {0,};
	char	szReturn[0xff] = {0,};
	char	szVal[5][0xff] = {0,};

	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();

	wsprintf(szInfo, "%s\\%s", g_szBasePath, "OptionConfig.ini");

	if(bSave)
	{	
		memset(szVal[0], 0, sizeof(szVal[0]));
		memset(szVal[1], 0, sizeof(szVal[1]));

		strcpy(szVal[0], " OFF");
		strcpy(szVal[1], " ON");

		// Bankground //
		WritePPSting("OPTION CONFIG", "BACKGROUND", szVal[pGameMenuWnd->m_bBGMFlag], szInfo);	
		// EffectSound //	
		WritePPSting("OPTION CONFIG", "EFFECTSOUND", szVal[pGameMenuWnd->m_bEffectFlag], szInfo);	
		// Shadow //
		WritePPSting("OPTION CONFIG", "SHADOW", szVal[pGameMenuWnd->m_bShadowFlag], szInfo);				
		// Text //
#if! IS_CHINA_LOCALIZING()
		WritePPSting("OPTION CONFIG", "TEXT", szVal[pGameMenuWnd->m_bTextFlag], szInfo);
#endif
		// System Message //.
		WritePPSting("OPTION CONFIG", "SYSTEMMESSAGE", szVal[pGameMenuWnd->m_bSystemMsgFlag], szInfo);
		// Zoom //
		memset(szVal[0], 0, sizeof(szVal[0]));
		wsprintf(szVal[0], "%u", g_nDGZoom);

		WritePPSting("OPTION CONFIG", "ZOOM", szVal[0], szInfo);
		// Effect //
		memset(szVal[0], 0, sizeof(szVal[0]));
		memset(szVal[1], 0, sizeof(szVal[1]));
		memset(szVal[2], 0, sizeof(szVal[2]));
		memset(szVal[3], 0, sizeof(szVal[3]));
		memset(szVal[4], 0, sizeof(szVal[4]));		

		strcpy(szVal[0], " OFF");
		strcpy(szVal[1], " LOW");
		strcpy(szVal[2], " MIDIUM");
		strcpy(szVal[3], " HIGH");
		strcpy(szVal[4], " MAX");

		WritePPSting("OPTION CONFIG", "EFFECT", szVal[pGameMenuWnd->m_byEffectIndex], szInfo);

		// sung-han 2005-04-01 옵션을 몇개 더 늘린다.--------------------------------------------------------------------------------
		memset(szVal[0], 0, sizeof(szVal[0]));
		memset(szVal[1], 0, sizeof(szVal[1]));
		strcpy(szVal[0], " OFF");
		strcpy(szVal[1], " ON");

		WritePPSting("OPTION CONFIG", "EMOTICON", szVal[pGameMenuWnd->m_bChatEmoticon], szInfo);// 자동 이모티콘
		WritePPSting("OPTION CONFIG", "WHISPER", szVal[pGameMenuWnd->m_bChatKindWhisper], szInfo);// 귓말을 채팅창에 보여주는지 여부...
		WritePPSting("OPTION CONFIG", "GUILD", szVal[pGameMenuWnd->m_bChatKindGuild], szInfo);// 길드...
		WritePPSting("OPTION CONFIG", "PARTY", szVal[pGameMenuWnd->m_bChatKindParty], szInfo);// 파티...
		WritePPSting("OPTION CONFIG", "FRIEND", szVal[pGameMenuWnd->m_bChatKindFriend], szInfo);// 친구...

		ZeroMemory(szVal, sizeof(szVal));
		strcpy(szVal[1], "FAST");
		strcpy(szVal[2], "MEDIUM");
		strcpy(szVal[3], "SLOW");
		WritePPSting("OPTION CONFIG", "TEXTSPEED", szVal[pGameMenuWnd->m_byChatRemainTime], szInfo);// 채팅창 텍스트 사라지는 속도

		ZeroMemory(szVal, sizeof(szVal));
		strcpy(szVal[1], "SHORT");
		strcpy(szVal[2], "MIDDLE");
		strcpy(szVal[3], "LONG");
		WritePPSting("OPTION CONFIG", "TEXTLINE", szVal[g_byChatLineCnt], szInfo);// 채팅창 라인수 1:5, 2:10, 3:15

		ZeroMemory(szVal, sizeof(szVal));
		strcpy(szVal[1], "NORMAL");
		strcpy(szVal[2], "WINDOW");
		strcpy(szVal[3], "ALL");
		WritePPSting("OPTION CONFIG", "NOTICE", szVal[pGameMenuWnd->m_byKindNotich], szInfo);// 공지 표시방법 선택
		//--------------------------------------------------------------------------------------------------------------------------------
	}
	else
	{
		// sung-han 2005-04-01 옵션을 몇개 더 늘린다.--------------------------------------------------------------------------------
		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "MEDIUM");
		GetPPS("OPTION CONFIG", "TEXTSPEED", szDefault, szReturn, 256, szInfo);
		if(__strcmp(szReturn, "FAST")==0)
			pGameMenuWnd->m_byChatRemainTime = 1;
		else if(__strcmp(szReturn, "MEDIUM")==0)
			pGameMenuWnd->m_byChatRemainTime = 2;
		else if(__strcmp(szReturn, "SLOW")==0)
			pGameMenuWnd->m_byChatRemainTime = 3;
		else
			pGameMenuWnd->m_byChatRemainTime = 2;


		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "MIDDLE");
		GetPPS("OPTION CONFIG", "TEXTLINE", szDefault, szReturn, 256, szInfo);
		if(__strcmp(szReturn, "SHORT")==0)
			g_byChatLineCnt = 1;
		else if(__strcmp(szReturn, "MIDDLE")==0)
			g_byChatLineCnt = 2;
		else if(__strcmp(szReturn, "LONG")==0)
			g_byChatLineCnt = 3;
		else
			g_byChatLineCnt = 2;


		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "WINDOW");
		GetPPS("OPTION CONFIG", "NOTICE", szDefault, szReturn, 256, szInfo);
		if(__strcmp(szReturn, "NORMAL")==0)
			pGameMenuWnd->m_byKindNotich = 1;
		else if(__strcmp(szReturn, "WINDOW")==0)
			pGameMenuWnd->m_byKindNotich = 2;
		else if(__strcmp(szReturn, "ALL")==0)
			pGameMenuWnd->m_byKindNotich = 3;
		else
			pGameMenuWnd->m_byKindNotich = 2;


		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "EMOTICON", szDefault, szReturn, 256, szInfo);
		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bChatEmoticon = TRUE;
		else if(__strcmp(szReturn, "OFF")==0)
			pGameMenuWnd->m_bChatEmoticon = FALSE;
		else
			pGameMenuWnd->m_bChatEmoticon = TRUE;


		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "WHISPER", szDefault, szReturn, 256, szInfo);
		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bChatKindWhisper = TRUE;
		else if(__strcmp(szReturn, "OFF")==0)
			pGameMenuWnd->m_bChatKindWhisper = FALSE;
		else
			pGameMenuWnd->m_bChatKindWhisper = TRUE;


		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "GUILD", szDefault, szReturn, 256, szInfo);
		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bChatKindGuild = TRUE;
		else if(__strcmp(szReturn, "OFF")==0)
			pGameMenuWnd->m_bChatKindGuild = FALSE;
		else
			pGameMenuWnd->m_bChatKindGuild = TRUE;


		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "PARTY", szDefault, szReturn, 256, szInfo);
		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bChatKindParty = TRUE;
		else if(__strcmp(szReturn, "OFF")==0)
			pGameMenuWnd->m_bChatKindParty = FALSE;
		else
			pGameMenuWnd->m_bChatKindParty = TRUE;


		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "FRIEND", szDefault, szReturn, 256, szInfo);
		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bChatKindFriend = TRUE;
		else if(__strcmp(szReturn, "OFF")==0)
			pGameMenuWnd->m_bChatKindFriend = FALSE;
		else
			pGameMenuWnd->m_bChatKindFriend = TRUE;
		//--------------------------------------------------------------------------------------------------------------------------------


		// Bankground //
		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "BACKGROUND", szDefault, szReturn, 256, szInfo);

		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bBGMFlag = TRUE;
		else if(__strcmp(szReturn, "OFF")==0)
			pGameMenuWnd->m_bBGMFlag = FALSE;
		else
			pGameMenuWnd->m_bBGMFlag = TRUE;

		// EffectSound //
		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "EFFECTSOUND", szDefault, szReturn, 256, szInfo);

		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bEffectFlag = TRUE;
		else if(__strcmp(szReturn, "OFF")==0)
			pGameMenuWnd->m_bEffectFlag = FALSE;
		else
			pGameMenuWnd->m_bEffectFlag = TRUE;					

		// Zoom //
		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "16");
		GetPPS("OPTION CONFIG", "ZOOM", szDefault, szReturn, 256, szInfo);

		g_nDGZoom = atoi(szReturn);		

		if(g_nDGZoom>__MAX_ZOOM_OUT__)
			g_nDGZoom = __MAX_ZOOM_OUT__;
			
		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "SHADOW", szDefault, szReturn, 256, szInfo);

		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bShadowFlag = TRUE;
		else
			pGameMenuWnd->m_bShadowFlag = FALSE;
	
		// Text //
		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "OFF");		

#if! IS_CHINA_LOCALIZING()
		GetPPS("OPTION CONFIG", "TEXT", szDefault, szReturn, 256, szInfo);
#endif

		if(__strcmp(szReturn, "ON")==0)
		{
			pGameMenuWnd->m_bTextFlag	= TRUE;
			g_dwFontFlag = SS3D_FONT;
		}
		else
		{
			pGameMenuWnd->m_bTextFlag	= FALSE;
			g_dwFontFlag = D3DX_FONT;
		}

		// System Message //
		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "ON");
		GetPPS("OPTION CONFIG", "SYSTEMMESSAGE", szDefault, szReturn, 256, szInfo);

		if(__strcmp(szReturn, "ON")==0)
			pGameMenuWnd->m_bSystemMsgFlag	= TRUE;
		else
			pGameMenuWnd->m_bSystemMsgFlag	= FALSE;

		// Effect //
		memset(szReturn, 0, sizeof(szReturn));
		memset(szDefault, 0, sizeof(szDefault));
		strcpy(szDefault, "MAX");
		GetPPS("OPTION CONFIG", "EFFECT", szDefault, szReturn, 256, szInfo);	

		if(__strcmp(szReturn, "MAX")==0)
			pGameMenuWnd->m_byEffectIndex= __EFFECT_OPTION__MAX__;
		else if(__strcmp(szReturn, "HIGH")==0)
			pGameMenuWnd->m_byEffectIndex= __EFFECT_OPTION__HIGH__;
		else if(__strcmp(szReturn, "MIDIUM")==0)
			pGameMenuWnd->m_byEffectIndex= __EFFECT_OPTION__MIDIUM__;
		else if(__strcmp(szReturn, "LOW")==0)
			pGameMenuWnd->m_byEffectIndex= __EFFECT_OPTION__LOW__;
		else if(__strcmp(szReturn, "OFF")==0)
			pGameMenuWnd->m_byEffectIndex= __EFFECT_OPTION__OFF__;			
				
		if(pGameMenuWnd->m_bBGMFlag)
		{
			PlayBGM(pGameMenuWnd->m_szLast_BGM_Name); // >>>>>인트로 화면과 캐릭 선택창에서 BGM 연결시킴 김영대 2003.02.22
		}
		else
		{
			StopBGM();
		}	
	}
}


void CInterface::ItemInfoRender_Weapon(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo
									   , DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize)
{
	int		nSize			= 0;
	char	szInfo[0xff]	= {0,};

	if( pSetItemInfo && dwItemSetID != 0 && dwItemSetID != 32767 )
	{
		strcpy( szItemInfo[*iInfoRow], pSetItemInfo->szItemName_Kor );
	}
	
	strcat( szItemInfo[*iInfoRow], "(" );
	strcat( szItemInfo[*iInfoRow], pItemInfo->szItemName_Kor );
	
	if(isEquip)
	{
		if( pSetItemInfo && dwItemSetID !=0 && dwItemSetID != 32767 )
		{
			char szTmp[50] = {0,};
			wsprintf( szTmp, " : %d/%d)", dwSetNum, pSetItemInfo->bFullSet );
			strcat( szItemInfo[*iInfoRow], szTmp );
		}
		else
		{
			strcat( szItemInfo[*iInfoRow], ")" );
		}
	}
	else 
	{
		strcat( szItemInfo[*iInfoRow], ")" );
	}

	if(pItem->m_bSlot.uUsedSlot > 0)
	{
		memset(szInfo, 0, sizeof(szInfo));
		wsprintf(szInfo, " + %u", pItem->m_bSlot.uUsedSlot);
		strcat(szItemInfo[*iInfoRow], szInfo);			
	}

	switch(pItemInfo->BaseItem_Weapon.byDropBox)
	{
		case 1:
			if( iOption != 0 )
				pdwTextColor[*iInfoRow] = TEXT_COLOR_BLUE;		
			break;
		case 2:
			pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;		
			break;
		case 3:
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			break;			
	}		

	if(pItem->m_bSlot.uUsedSlot > 0)
	{
		// 업그레이드 된 아이템은 노랑색으로...
		pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
	}		

	nSize = lstrlen(szItemInfo[*iInfoRow]);	

	if(*nMaxSize < nSize)
		*nMaxSize = nSize;		
	
	if(pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage );
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
		
		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}
	
	if( pItemInfo->BaseItem_Weapon.bClassID != 0 )
	{
		BOOL bClass = FALSE;

		if(pItemInfo->BaseItem_Weapon.bClassID & (BYTE)( 1 << (g_pMainPlayer->m_wClass-1) ))
			bClass = TRUE;

		if(bClass)
		{
			// "클래스착용 가능"
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE995].szMessage); 
			pdwTextColor[*iInfoRow] = 0xffffffff;
		}
		else
		{
			// "클래스착용 불가"
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE996].szMessage); 
			pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
			pdwTextColor[*iInfoRow-2] = TEXT_COLOR_RED;
		}			
		
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
					
		*iInfoRow += 1;
	}
	
	wsprintf( szItemInfo[*iInfoRow], "       " );

	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;
	
	// "Damage %d ~ %d"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE858].szMessage
		, (int)pItem->m_Item_Weapon.wMin_Damage, (int)pItem->m_Item_Weapon.wMax_Damage );

	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;
	
	// "Grade : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE997].szMessage
		, pItemInfo->BaseItem_Weapon.bItemGrade ); 

	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "스킬공격력 : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE998].szMessage, pItem->m_Item_Weapon.wMana_Damage);			
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "Slot : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE860].szMessage,  pItem->m_bSlot.uMaxSlot ); 
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;			

	// "무게 : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItemInfo->GetWeight()); 
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;
	
#ifdef __USE_STAT_LIMIT					
	if(g_pMainPlayer->m_dwLevel >= pItemInfo->BaseItem_Weapon.wMin_Level)
		pdwTextColor[*iInfoRow] = 0xffffffff;
	else
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;

	wsprintf(szItemInfo[*iInfoRow], "Level : %u", pItemInfo->BaseItem_Weapon.wMin_Level);
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	if(g_pMainPlayer->m_dwStr >= pItemInfo->BaseItem_Weapon.wMin_Str)
		pdwTextColor[*iInfoRow] = 0xffffffff;
	else
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;

	wsprintf(szItemInfo[*iInfoRow], "Str : %u", pItemInfo->BaseItem_Weapon.wMin_Str);
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;			
#endif
}


void CInterface::ItemInfoRender_Armor(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, LPSET_ITEM_INFO pSetItemInfo
									  , DWORD dwItemSetID, DWORD dwSetNum, DWORD* pdwTextColor, BOOL isEquip, int iOption, int* nMaxSize, BYTE byZipCode)
{
	int		nSize			= 0;
	char	szInfo[0xff]	= {0,};

	if( pSetItemInfo && dwItemSetID != 0 && dwItemSetID != 32767 )
	{
		wsprintf( szItemInfo[*iInfoRow], "%s", pSetItemInfo->szItemName_Kor );
	}
	
	strcat( szItemInfo[*iInfoRow], "(" );
	strcat( szItemInfo[*iInfoRow], pItemInfo->szItemName_Kor );

	if( isEquip )
	{
		if( pSetItemInfo && dwItemSetID !=0 && dwItemSetID != 32767 )
		{
			char szTmp[50] = {0,};
			wsprintf( szTmp, " : %d/%d)", dwSetNum, pSetItemInfo->bFullSet );
			strcat( szItemInfo[*iInfoRow], szTmp );
		}
		else
		{
			strcat( szItemInfo[*iInfoRow], ")" );
		}
	}
	else 
	{
		strcat( szItemInfo[*iInfoRow], ")" );
	}
	
	if(pItem->m_bSlot.uUsedSlot > 0)
	{
		memset(szInfo, 0, sizeof(szInfo));
		wsprintf(szInfo, " + %u", pItem->m_bSlot.uUsedSlot);
		strcat(szItemInfo[*iInfoRow], szInfo);			
	}

	nSize = lstrlen(szItemInfo[*iInfoRow]);	

	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	switch( pItemInfo->BaseItem_Armor.byDropBox)// : 050108 hwoarang
	{
	case 1:
		if( iOption != 0 )
			pdwTextColor[*iInfoRow] = TEXT_COLOR_BLUE;		
		break;
	case 2:
		pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;		
		break;
	case 3:
		pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
		break;			
	}
	
	if(pItem->m_bSlot.uUsedSlot>0)
	{
		// 업그레이드 된 아이템은 노랑색으로...
		pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
	}		

	if(isEquip)
	{
		switch(byZipCode)
		{
			case EQUIP_TYPE_AMULE1:
			case EQUIP_TYPE_AMULE3:
			case EQUIP_TYPE_LRING1:
			case EQUIP_TYPE_LRING2:
			case EQUIP_TYPE_LRING3:
				pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
				break;
		}
	}

	if(pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage );
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
		
		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}
	
	if( pItemInfo->BaseItem_Armor.bClassID != 0 )
	{
		BOOL bClass = FALSE;
		
		if(pItemInfo->BaseItem_Armor.bClassID & (BYTE)( 1 << (g_pMainPlayer->m_wClass - 1) ))
			bClass = TRUE;

		if(bClass)
		{
			// "클래스착용 가능"
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE995].szMessage); 
			pdwTextColor[*iInfoRow] = 0xffffffff;
		}
		else
		{
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE996].szMessage); 
			pdwTextColor[*iInfoRow]		= TEXT_COLOR_RED;
			pdwTextColor[*iInfoRow-2]	= TEXT_COLOR_RED;
		}			
		
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
					
		*iInfoRow += 1;
	}
	
	wsprintf( szItemInfo[*iInfoRow], "       " );

	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// MSG_ID : 104 ; Defense %d
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE104].szMessage, pItem->m_Item_Armor.wDefense );	
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "Grade : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE997].szMessage,  pItemInfo->BaseItem_Armor.bItemGrade ); 
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "Slot : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE860].szMessage,  pItem->m_bSlot.uMaxSlot );	
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	// "무게 : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItemInfo->GetWeight()); 
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

#ifdef __USE_STAT_LIMIT						
	if(g_pMainPlayer->m_dwLevel>=pItemInfo->BaseItem_Armor.wMin_Level)
		pdwTextColor[*iInfoRow] = 0xffffffff;
	else
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;

	wsprintf(szItemInfo[*iInfoRow], "Level : %u", pItemInfo->BaseItem_Armor.wMin_Level);

	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	if(g_pMainPlayer->m_dwStr>=pItemInfo->BaseItem_Armor.wMin_Str)
		pdwTextColor[*iInfoRow] = 0xffffffff;
	else
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;

	wsprintf(szItemInfo[*iInfoRow], "Str : %u", pItemInfo->BaseItem_Armor.wMin_Str);

	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;
#endif
}


void CInterface::ItemInfoRender_Guardian(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;

	wsprintf( szItemInfo[*iInfoRow], "%s", pItemInfo->szItemName_Kor );
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	if(pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage );
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
		
		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	// "무게 : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItemInfo->GetWeight()); 
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	if (GUARDIAN_ITEM_TYPE_EGG == pItemInfo->BaseItem_Guardian.bType)
	{
		if (CUserInterface::GetInstance()->m_bGuardian &&
			CUserInterface::GetInstance()->m_i64ActiveGuardianItemID == pItem->GetSerial()->i64Serial)
		{
			// "부화중입니다..."
			lstrcpy( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1250].szMessage); 
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}

		// "부화율 %d%%"
		wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1251].szMessage
			, DWORD((float)pItem->m_Item_Guardian.dwCompleteTime / pItemInfo->BaseItem_Guardian.wCompleteTime*100.f)); 
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;

		if (pItem->m_Item_Guardian.dwEndLife)
		{
			GetDueDayText(szItemInfo[*iInfoRow], pItem->m_Item_Guardian.dwEndLife);
							
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;
				
			*iInfoRow += 1;
		}
	}
	else if (GUARDIAN_ITEM_TYPE_ADULT == pItemInfo->BaseItem_Guardian.bType)
	{
		DWORD dwGuardianLevel = GetGuardianItemLevel(pItem->m_Item_Guardian.dwCompleteTime);

		wsprintf( szItemInfo[*iInfoRow], "Level %d", dwGuardianLevel);
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;

		// "경험치 %d/%d"
		wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1252].szMessage, pItem->m_Item_Guardian.dwCompleteTime 
			, GetExpTableOfLevel(OBJECT_TYPE_MONSTER, dwGuardianLevel+1)); 
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;

		if (pItem->m_Item_Guardian.dwEndLife)
		{
			GetDueDayText(szItemInfo[*iInfoRow], pItem->m_Item_Guardian.dwEndLife);
							
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;
				
			*iInfoRow += 1;
		}

		*iInfoRow += 1;

		pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
		wsprintf(szItemInfo[*iInfoRow], "EGO(%d) STR(%d) INT(%d) DEX(%d) VIT(%d)", pItem->Guardian.wStat[0], pItem->Guardian.wStat[1], pItem->Guardian.wStat[2], pItem->Guardian.wStat[3], pItem->Guardian.wStat[4]);
		*iInfoRow += 1;

		WORD wSkillLevel = pItem->bLevelH << 8 | pItem->Guardian.bLevelL;
		for(int i=0;i<MAX_GUARDIAN_USE_SKILL;++i)
		{
			BYTE bKind = pItem->Guardian.bSkill[i];
			BYTE bLevel = (7 & (wSkillLevel >> 3 * i)) * 5;
	
			if(bKind == 0)
				continue;

			if(bLevel == 0)
				bLevel = 1;

			pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
			wsprintf(szItemInfo[*iInfoRow], "%s Lv. %d", g_pEffectLayer->m_Effect[bKind].szName, bLevel);
			*iInfoRow += 1;
		}

		*iInfoRow += 1;
	}
	
	LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());
	
	if(lpItemOption)
	{
		for(int i = 0; i < lpItemOption->byCount; i++)
		{
			__lstrcpyn(szItemInfo[*iInfoRow], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if(*nMaxSize < nSize)
				*nMaxSize = nSize;
				
			*iInfoRow += 1;
		}
	}
}


void CInterface::ItemInfoRender_Supplies(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;
	
	wsprintf( szItemInfo[*iInfoRow], "%s", pItemInfo->szItemName_Kor );
	nSize = lstrlen(szItemInfo[*iInfoRow]);
		
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	if(pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage );
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
		
		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	if(pItemInfo->BaseItem_Supplies.bType == 7)
	{
		wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1419].szMessage
		, pItemInfo->BaseItem_Supplies.wMin_Lev);
	}
	else if(pItemInfo->BaseItem_Supplies.bType == 1 && pItemInfo->BaseItem_Supplies.bType == 2)
	{
		wsprintf(szItemInfo[*iInfoRow], "%d%% / %d%%", pItemInfo->BaseItem_Supplies.wMin
		, pItemInfo->BaseItem_Supplies.wMax);
	}
	else
	{
		wsprintf(szItemInfo[*iInfoRow], "%d / %d", pItemInfo->BaseItem_Supplies.wMin
		, pItemInfo->BaseItem_Supplies.wMax);
	}

	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;
		
	*iInfoRow += 1;

	// "무게 : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItemInfo->GetWeight()); 
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 1;

	LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());
	
	if(lpItemOption)
	{
		for(int i = 0; i < lpItemOption->byCount; i++)
		{
			__lstrcpyn(szItemInfo[*iInfoRow], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if(*nMaxSize < nSize)
				*nMaxSize = nSize;
				
			*iInfoRow += 1;
		}
	}
}


void CInterface::ItemInfoRender_Consumable(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;

	wsprintf( szItemInfo[*iInfoRow], "%s", pItemInfo->szItemName_Kor );						
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	if(pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage );
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
		
		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	// "무게 : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItemInfo->GetWeight()); 
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 2;
	
	if (pItem->m_Item_Consumable.bInvIndex)
	{
		// "사용중입니다."
		__lstrcpyn(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE844].szMessage, 0xff); 
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;

		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
			
		*iInfoRow += 1;
		
		if (pItemInfo->BaseItem_Consumable.wLast_Time)
		{
			if(ITEM_LAND_TYPE_SERVERTIME == pItemInfo->bCode_Type)// : 050108 hwoarang
				GetDueDayText(szItemInfo[*iInfoRow], pItem->m_Item_Consumable.dwEndDate);
			else if(ITEM_LAND_TYPE_PLAYTIME == pItemInfo->bCode_Type)
							
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_YELLOW;
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;
				
			*iInfoRow += 1;
		}
	}			
	
	LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());
	
	if(lpItemOption)
	{
		for(int i = 0; i < lpItemOption->byCount; i++)
		{
			__lstrcpyn(szItemInfo[*iInfoRow], g_pItemAttrLayer->GetItemDescription(pItem->GetID(), i) , 0xff);

			nSize = lstrlen(szItemInfo[*iInfoRow]);

			if(*nMaxSize < nSize)
				*nMaxSize = nSize;
				
			*iInfoRow += 1;
		}
	}
}


void CInterface::ItemInfoRender_Zodiac(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor, int* nMaxSize)
{
	int nSize = 0;
	
	strcpy( szItemInfo[*iInfoRow], pItemInfo->szItemName_Kor );
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	if(pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage );
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
		
		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	// "무게 : %u"
	wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage, pItemInfo->GetWeight()); 
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;
				
	*iInfoRow += 2;
	
	if(pItem->GetID()==__ITEM_INSURANCE_INDEX__)
	{
		if(pItem->m_Item_Zodiac.bInvIndex)
		{
			// 아이템 보험 사용중일때의 메시지출력
			// "사용중입니다."
			__lstrcpyn(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE844].szMessage, 0xff); 
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;			
		}
	}

	// 마을 귀환 스크롤 추가 2005.02.14 김영대 
	WORD wItemID = pItem->GetID();
	if( (wItemID == __ITEM_PORTAL_INDEX__ ) ||
		(wItemID >= __ITEM_PORTAL_VILL1__ && wItemID <= __ITEM_PORTAL_VILL6__)	)
	//if(	pItem->GetID()==__ITEM_PORTAL_INDEX__ || pItem->GetID()==__ITEM_PORTAL_VILL1__ ||
	//	pItem->GetID()==__ITEM_PORTAL_VILL2__ || pItem->GetID()==__ITEM_PORTAL_VILL3__ ||
	//	pItem->GetID()==__ITEM_PORTAL_VILL4__ || pItem->GetID()==__ITEM_PORTAL_VILL5__)
	{
		if(pItem->m_Item_Zodiac.bType==0)
		{
			// "기록된 좌표 없음"
			strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE462].szMessage); 
			nSize = lstrlen(szItemInfo[*iInfoRow]);
		
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;
				
			*iInfoRow += 1;
		}
		else
		{
			DUNGEON_DATA_EX* pDungeonInfo = g_pDungeonTable->GetDungeonInfo(pItem->m_Item_Zodiac.wMapId);
		
			if(pDungeonInfo)
			{
#if IS_JAPAN_LOCALIZING()
				wsprintf(szItemInfo[*iInfoRow]
					, lstrlen(pDungeonInfo->m_szSchoolName) ? pDungeonInfo->m_szSchoolName : pDungeonInfo->m_szDungeonName);
#else
				__lstrcpyn(szItemInfo[*iInfoRow], pDungeonInfo->m_szDungeonName, lstrlen(pDungeonInfo->m_szDungeonName));
#endif
				nSize = lstrlen(szItemInfo[*iInfoRow]);
			
				if(*nMaxSize < nSize)
					*nMaxSize = nSize;
					
				*iInfoRow += 1;
	
				if(pItem->m_Item_Zodiac.wMapId/1000!=0)
				{
					// 던젼 층수 //
					if(pItem->m_Item_Zodiac.bLayer==0)
					{
						// MSG_ID : 16 ; 지상 1층
						strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE16].szMessage);					
					}
					else
					{
						// MSG_ID : 17 ; 지하 %d층
						wsprintf(szItemInfo[*iInfoRow]
							, g_Message[ETC_MESSAGE17].szMessage, pItem->m_Item_Zodiac.bLayer);	
					}

					nSize = lstrlen(szItemInfo[*iInfoRow]);
			
					if(*nMaxSize < nSize)
						*nMaxSize = nSize;
						
					*iInfoRow += 1;
				}
				
				if(pItem->GetID()==__ITEM_PORTAL_INDEX__)
				{
					// "X : %u, Y : %u"
					wsprintf(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE867].szMessage, pItem->m_Item_Zodiac.wPosX, pItem->m_Item_Zodiac.wPosZ);
					nSize = lstrlen(szItemInfo[*iInfoRow]);
			
					if(*nMaxSize < nSize)
						*nMaxSize = nSize;
						
					*iInfoRow += 1;
				}					
			}
			else
			{
				// "사용할 수 없습니다."
				strcpy(szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE211].szMessage); 
				nSize = lstrlen(szItemInfo[*iInfoRow]);
			
				if(*nMaxSize < nSize)
					*nMaxSize = nSize;
					
				*iInfoRow += 1;
			}
		}
	}
	else
	{
		LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());

		if(lpItemOption)
		{
			for(int i = 0; i < lpItemOption->byCount; i++)
			{
				__lstrcpyn(szItemInfo[*iInfoRow], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

				nSize = lstrlen(szItemInfo[*iInfoRow]);
	
				if(*nMaxSize < nSize)
					*nMaxSize = nSize;
					
				*iInfoRow +=1;
			}
		}
	}
}


void CInterface::ItemInfoRender_Default(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor,int* nMaxSize)
{
	int nSize = 0;

	wsprintf( szItemInfo[*iInfoRow], "%s", pItemInfo->szItemName_Kor );			
	nSize = lstrlen(szItemInfo[*iInfoRow]);
	
	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	if(pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		*iInfoRow += 1;
		strcpy( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1428].szMessage );
		pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		nSize = lstrlen(szItemInfo[*iInfoRow]);	
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;
		
		*iInfoRow += 1;
	}
	else
	{
		*iInfoRow += 2;
	}

	// "무게 : %u"
	//wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItemInfo->GetWeight()); 
	if ( (pItem->m_wItemID >= BAG_ITEM_NUM_FRIST && pItem->m_wItemID <= BAG_ITEM_NUM_LAST) )
	{
		switch (pItem->m_Item_Bag.bType )
		{
		case ITEM_BAG_TYPE_DECREASE_SMALL_BAG :
			{
				wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1412].szMessage,  pItem->m_Item_Bag.wProbability); 
			}
			break;
		case ITEM_BAG_TYPE_INCREASE_TOTAL_BAG  :
			{
				wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1413].szMessage,  pItem->m_Item_Bag.wProbability); 
			}
			break;
		case ITEM_BAG_TYPE_INCREASE_TOTALPER_BAG  :
			{
				wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE1414].szMessage,  pItem->m_Item_Bag.wProbability); 
			}
			break;
		default :
			{
				wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItem->m_Item_Bag.wProbability); 
			}
		}
		nSize = lstrlen(szItemInfo[*iInfoRow]);
	}
	else
	{
		wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE999].szMessage,  pItem->m_Item_Bag.wProbability); 
		nSize = lstrlen(szItemInfo[*iInfoRow]);
	}

	//pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;

	if(*nMaxSize < nSize)
		*nMaxSize = nSize;

	*iInfoRow += 2;

	LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(pItem->GetID());

	if(lpItemOption)
	{
		for(int i = 0; i < lpItemOption->byCount; i++)
		{
			__lstrcpyn(szItemInfo[*iInfoRow], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

			nSize = lstrlen(szItemInfo[*iInfoRow]);
			//pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;
				
			*iInfoRow += 1;
		}
	}
	if ( (pItem->m_wItemID >= BAG_ITEM_NUM_FRIST && pItem->m_wItemID <= BAG_ITEM_NUM_LAST) )
	{
		wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE347].szMessage,  pItemInfo->BaseItem_Bag.wMin_Lev ); 
		if ( g_pMainPlayer->m_dwLevel < pItemInfo->BaseItem_Bag.wMin_Lev)	pdwTextColor[*iInfoRow] = TEXT_COLOR_RED;
		*iInfoRow += 1;
		nSize = lstrlen(szItemInfo[*iInfoRow]);
		if(*nMaxSize < nSize)	*nMaxSize = nSize;
	}

}


void CInterface::ItemInfoRender_Option(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor
									   , int* nMaxSize, int* iOption)
{
	int nSize = 0;
	
	if(*iOption != 0)
	{
		wsprintf( szItemInfo[*iInfoRow], "    " );
		nSize = lstrlen(szItemInfo[*iInfoRow]);
			
		if(*nMaxSize < nSize)
			*nMaxSize = nSize;

		*iInfoRow += 1;
		
		for( BYTE i = 0; i < MAX_ITEM_ATTR; i++ )
		{
			// 효과 수치가 0이면 무시!
			if(0 == pItem->wAttr[i].uValue) continue;

			__lstrcpyn(szItemInfo[*iInfoRow]
				, g_pItemAttrLayer->GetAttrOptionDescription(pItem->wAttr[i].uCode, pItem->wAttr[i].uValue ), 0xff);
			
			pdwTextColor[*iInfoRow] = TEXT_COLOR_BLUE;
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;
		}
	}
}


void CInterface::ItemInfoRender_SetItemBonusOption(char szItemInfo[100][255], int* iInfoRow, CItem* pItem, CBaseItem* pItemInfo, DWORD* pdwTextColor
												   , int* nMaxSize, BOOL isEquip, DWORD dwItemSetID, DWORD dwSetNum, LPSET_ITEM_INFO pSetItemInfo)
{
	int nSize = 0;

	if( isEquip && dwItemSetID )
	{
		int iLoop = 0;

		if( dwSetNum >= 2 )
		{
			// "추가 옵션"
			wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE589].szMessage ); 
			pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;
			*iInfoRow += 1;
			
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;

			iLoop = dwSetNum - 1;
			if( dwSetNum > 4 ) iLoop = 4;

			for( int i = 0; i < iLoop; i++ )
			{
				switch( pItemInfo->dwCode_ID )
				{
				case ITEM_KIND_WEAPON:
					{
						if( pItemInfo->BaseItem_Weapon.pPartOption[i].wKind == 0 )	continue;
						if( pItemInfo->BaseItem_Weapon.pPartOption[i].wValue == 0 )	continue; 
						
						__lstrcpyn( szItemInfo[*iInfoRow]
							, g_pItemAttrLayer->GetAttrOptionDescription(pItemInfo->BaseItem_Weapon.pPartOption[i].wKind
							, pItemInfo->BaseItem_Weapon.pPartOption[i].wValue), 0xff );

						pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
						nSize = lstrlen(szItemInfo[*iInfoRow]);
			
						if(*nMaxSize < nSize)
							*nMaxSize = nSize;

						*iInfoRow += 1;
					}
					break;

				case ITEM_KIND_AROMR:
					{
						if( pItemInfo->BaseItem_Armor.pPartOption[i].wKind == 0 ) continue;
						if( pItemInfo->BaseItem_Armor.pPartOption[i].wValue == 0 )
						{
							// 추가값이 0이어두 무시.. by deepdark.
							continue; 
						}
					
						__lstrcpyn( szItemInfo[*iInfoRow]
							, g_pItemAttrLayer->GetAttrOptionDescription(pItemInfo->BaseItem_Armor.pPartOption[i].wKind
							, pItemInfo->BaseItem_Armor.pPartOption[i].wValue) , 0xff);
						
						pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
						nSize = lstrlen(szItemInfo[*iInfoRow]);
			
						if(*nMaxSize < nSize)
							*nMaxSize = nSize;

						*iInfoRow += 1;
					}
					break;
				}
			}

			// Set Item Bonus 옵션.
			wsprintf( szItemInfo[*iInfoRow], "       " );
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;

			// "세트아이템 보너스 옵션"
			wsprintf( szItemInfo[*iInfoRow], g_Message[ETC_MESSAGE590].szMessage ); 
			pdwTextColor[*iInfoRow] = TEXT_COLOR_ORANGE;			
			nSize = lstrlen(szItemInfo[*iInfoRow]);
			
			if(*nMaxSize < nSize)
				*nMaxSize = nSize;

			*iInfoRow += 1;

			iLoop = dwSetNum - 1;
			if( dwSetNum > 5 ) iLoop = 5;

			for( i = 0; i < iLoop; i++ )
			{
				if( pSetItemInfo->pBonusOption[i][0].wKind != 0 && pSetItemInfo->pBonusOption[i][0].wValue != 0)
				{
					__lstrcpyn( szItemInfo[*iInfoRow]
						, g_pItemAttrLayer->GetAttrOptionDescription(pSetItemInfo->pBonusOption[i][0].wKind
						, pSetItemInfo->pBonusOption[i][0].wValue) , 0xff);

					pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
					nSize = lstrlen(szItemInfo[*iInfoRow]);
			
					if(*nMaxSize < nSize)
						*nMaxSize = nSize;
					
					*iInfoRow += 1;
				}

				if( pSetItemInfo->pBonusOption[i][1].wKind != 0 && pSetItemInfo->pBonusOption[i][0].wValue != 0)
				{
					__lstrcpyn( szItemInfo[*iInfoRow]
						, g_pItemAttrLayer->GetAttrOptionDescription(pSetItemInfo->pBonusOption[i][1].wKind
						, pSetItemInfo->pBonusOption[i][1].wValue), 0xff);
					
					pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
					nSize = lstrlen(szItemInfo[*iInfoRow]);
			
					if(*nMaxSize < nSize)
						*nMaxSize = nSize;

					*iInfoRow += 1;
				}
			}

			// Full Set Bonus 옵션.
			if( dwSetNum == pSetItemInfo->bFullSet )
			{
				for(i = 0; i < MAX_ITEM_ATTR; i++)
				{
					if( pSetItemInfo->pFullOption[i].wKind != 0 && pSetItemInfo->pFullOption[i].wValue != 0)
					{
						__lstrcpyn( szItemInfo[*iInfoRow]
							, g_pItemAttrLayer->GetAttrOptionDescription(pSetItemInfo->pFullOption[i].wKind
							, pSetItemInfo->pFullOption[i].wValue), 0xff );
						
						pdwTextColor[*iInfoRow] = TEXT_COLOR_GREEN;
						nSize = lstrlen(szItemInfo[*iInfoRow]);
			
						if(*nMaxSize < nSize)
							*nMaxSize = nSize;
						
						*iInfoRow += 1;
					}
				}
			}
		}
	}
}


void CInterface::ItemInfoRender(CItem* pItem, BOOL isEquip, BOOL bPrice, DWORD dwPrice, BYTE byZipCode, BYTE byOption)
{
	char			szItemInfo[100][255]		= {0,};
	char			szUpgradeOption[10][255]	= {0,};

	int				iOption						= 0;
	int				iInfoRow					= 0;	
	int				nMaxSize					= 0;
	int				nSize						= 0;

	DWORD			dwSetNum					= 0;	
	DWORD			dwItemSetID					= 0;
	DWORD			dwEquipSetID				= 0;
	DWORD			dwSetID[10]					= {0,};
	DWORD			pdwTextColor[100]			= {0,};
	DWORD			dwUpgradeIndex				= 0;
	DWORD			dwUpgradeTextColor[10]		= {0,};

	CBaseItem*		pItemInfo					= NULL;
	LPSET_ITEM_INFO pSetItemInfo				= NULL;
	CBaseItem*		pItemTmp					= NULL;

	if(g_Mouse.bLDown == TRUE)
		return;

	for(int i = 0; i < 50; i++)
		memset(szItemInfo[i], 0, sizeof(szItemInfo[i]));
	memset(dwSetID, 0, sizeof(dwSetID));
	memset(pdwTextColor, 0xff, sizeof( DWORD ) * 50);	

	pItemInfo = g_pItemTableHash->GetData(pItem->m_wItemID);	
	if(NULL == pItemInfo)
		return;
	
	if(pItem->GetType() == ITEM_LARGE)
		dwItemSetID	= pItemInfo->GetSetID();

	if( dwItemSetID != 0 && dwItemSetID != 32767)
	{
		pSetItemInfo = g_pSetItemInfoHash->GetData(dwItemSetID);

		if(isEquip)
		{		
			BOOL isChk = FALSE;
			memset( dwSetID, 0, sizeof( DWORD ) * 10 );
		
			for(i = 0; i < MAX_EQUIP_POOL; i++ )
			{
				if(g_pMainPlayer->m_bCurrnetHand == 1)
				{
					if(i == EQUIP_TYPE_RHAND2 || i == EQUIP_TYPE_LHAND2)
						continue;
				}
				else if(g_pMainPlayer->m_bCurrnetHand == 2)
				{
					if(i == EQUIP_TYPE_RHAND1 || i == EQUIP_TYPE_LHAND1)
						continue;
				}				

				if( i == EQUIP_TYPE_AMULE1 || i == EQUIP_TYPE_AMULE3 || i == EQUIP_TYPE_LRING1 ||
					i == EQUIP_TYPE_LRING2 || i == EQUIP_TYPE_LRING3)
					continue;				
			
				if(g_pMainPlayer->CheckItem(&g_pMainPlayer->m_pEquip[i])==FALSE)
					continue;
					
				if( g_pMainPlayer->m_pEquip[i].m_wItemID == NULL )
					continue;

				pItemTmp		= g_pItemTableHash->GetData(g_pMainPlayer->m_pEquip[i].m_wItemID);
				dwEquipSetID	= pItemTmp->GetSetID();

				if( dwItemSetID != dwEquipSetID ) continue;

				isChk = FALSE;
				
				for(int j = 0; j < 10; j++)
				{
					if( dwSetID[j] == 0 ) break;
					if( dwSetID[j] == pItemTmp->wID )
					{
						isChk = TRUE;
						break;
					}
				}

				if( !isChk )
				{
					dwSetID[ dwSetNum ] = pItemTmp->wID;
					dwSetNum++;
				}
			}
		}
	}

	i = 0;

	while(i < MAX_ITEM_ATTR)
	{
		if(pItem->wAttr[i].uCode!=0)		
			iOption++;

		i++;
	}
	
	switch(pItemInfo->dwCode_ID)
	{
		case ITEM_KIND_WEAPON:	
			ItemInfoRender_Weapon(szItemInfo, &iInfoRow, pItem, pItemInfo, pSetItemInfo
				, dwItemSetID, dwSetNum, pdwTextColor, isEquip, iOption, &nMaxSize);	
			break;
		case ITEM_KIND_AROMR:
			ItemInfoRender_Armor(szItemInfo, &iInfoRow, pItem, pItemInfo, pSetItemInfo
				, dwItemSetID, dwSetNum, pdwTextColor, isEquip, iOption, &nMaxSize, byZipCode);
			break;
		case ITEM_KIND_GUARDIAN:
			ItemInfoRender_Guardian(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
			break;
		case ITEM_KIND_SUPPLIES:
			ItemInfoRender_Supplies(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
			break;
		case ITEM_KIND_CONSUMABLE:
			ItemInfoRender_Consumable(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
			break;
		case ITEM_KIND_ZODIAC:
			ItemInfoRender_Zodiac(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
			break;
		case ITEM_KIND_SPECIAL:			
		case ITEM_KIND_UPGRADE:		
		case ITEM_KIND_LIQUID:		
		case ITEM_KIND_EDITION:		
		case ITEM_KIND_MAGICFIELDARRAY:			
		case ITEM_KIND_MIXUPGRADE:		
		case ITEM_KIND_MATERIALS:		
		case ITEM_KIND_MAGICARRAY:				
		case ITEM_KIND_RIDE:		
		case ITEM_KIND_BAG:
			ItemInfoRender_Default(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize);
			break;
		default:
			return;
	}

	if(pItemInfo->GetMovable() != 0)// (귀속아이템) 표시
	{
		strcpy( szItemInfo[iInfoRow], g_Message[ETC_MESSAGE1429].szMessage );//본인계정 외 캐릭터에게는 양도가 금지됩니다.
		nSize = lstrlen(szItemInfo[iInfoRow]);	
		if(nMaxSize < nSize)
			nMaxSize = nSize;
		
		iInfoRow += 1;
	}
	
	if(pItemInfo->dwCode_ID != ITEM_KIND_GUARDIAN)
	{
		ItemInfoRender_Option(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize, &iOption);
	}

	ItemInfoRender_SetItemBonusOption(szItemInfo, &iInfoRow, pItem, pItemInfo, pdwTextColor, &nMaxSize, isEquip, dwItemSetID, dwSetNum, pSetItemInfo);

	if(isEquip && byOption)
	{
		BYTE byOptionValue[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		switch(byOption)
		{
		case 6:
			byOptionValue[0] = 31;	byOptionValue[1] = 10;	byOptionValue[2] = 12;	byOptionValue[3] = 5;	byOptionValue[4] = 14;	byOptionValue[5] = 5;
			byOptionValue[6] = 3;	byOptionValue[7] = 8;
			break;
		case 7:
			byOptionValue[0] = 31;	byOptionValue[1] = 15;	byOptionValue[2] = 12;	byOptionValue[3] = 7;	byOptionValue[4] = 14;	byOptionValue[5] = 7;
			byOptionValue[6] = 3;	byOptionValue[7] = 10;	byOptionValue[8] = 23;	byOptionValue[9] = 1;
			break;
		case 8:
			byOptionValue[0] = 31;	byOptionValue[1] = 25;	byOptionValue[2] = 12;	byOptionValue[3] = 10;	byOptionValue[4] = 14;	byOptionValue[5] = 10;
			byOptionValue[6] = 3;	byOptionValue[7] = 14;	byOptionValue[8] = 23;	byOptionValue[9] = 2;
			break;
		case 9:
			byOptionValue[0] = 31;	byOptionValue[1] = 40;	byOptionValue[2] = 12;	byOptionValue[3] = 15;	byOptionValue[4] = 14;	byOptionValue[5] = 15;
			byOptionValue[6] = 3;	byOptionValue[7] = 20;	byOptionValue[8] = 23;	byOptionValue[9] = 3;
			break;
		case 10:
			byOptionValue[0] = 31;	byOptionValue[1] = 60;	byOptionValue[2] = 12;	byOptionValue[3] = 25;	byOptionValue[4] = 14;	byOptionValue[5] = 25;
			byOptionValue[6] = 3;	byOptionValue[7] = 30;	byOptionValue[8] = 23;	byOptionValue[9] = 4;
			break;
		}
		
		for(int i = 0; i < 5; i++)
		{
			if(byOptionValue[i * 2] == 0)
				continue;

			__lstrcpyn(szUpgradeOption[dwUpgradeIndex]
				, g_pItemAttrLayer->GetAttrOptionDescription(byOptionValue[i*2], byOptionValue[i*2+1]), 0xff );

			dwUpgradeTextColor[dwUpgradeIndex] = TEXT_COLOR_GREEN;

			nSize = lstrlen(szUpgradeOption[dwUpgradeIndex]);

			if(nMaxSize<nSize)
				nMaxSize = nSize;

			dwUpgradeIndex++;
		}
	}
	
	CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();

	if(pStoreWnd->GetActive()==TRUE)
	{
		char		szInfo[0xff]	= {0,};
		char		szPrice[0xff]	= {0,};
		NUMBERFMT	nFmt			= { 0, 0, 3, ".", ",", 1 };		

		wsprintf(szInfo, "%u", pItemInfo->GetSellPrice());		
		::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szPrice, 15);

		// "%s Karz"
		wsprintf(szItemInfo[iInfoRow], g_Message[ETC_MESSAGE870].szMessage, szPrice); 

		if(pItemInfo->dwCode_ID!=ITEM_KIND_RIDE)
		{
			nSize = lstrlen(szItemInfo[iInfoRow]);
			
			if(nMaxSize<nSize)
				nMaxSize = nSize;

			iInfoRow++;
		}
	}

	int nOverXSize = 0;
	int nOverYSize = 0;
	int nInfoIndex = iInfoRow;

	if(bPrice)
		nInfoIndex+=2;

	if(isEquip && byOption)
		nInfoIndex+=8;

	if(g_Mouse.MousePos.y + 5 + (nInfoIndex * 18) + 14 > 768)
		nOverYSize = g_Mouse.MousePos.y + 5 + (nInfoIndex * 18) + 14 - 768;

	if((g_Mouse.MousePos.x + ITEMINFO_RENDER_POSITION)+(float)(nMaxSize*6.5f+10) > 1024)
		nOverXSize = (int)((g_Mouse.MousePos.x + ITEMINFO_RENDER_POSITION)+(float)(nMaxSize*6.5f+10) - 1024);

	if(bPrice)
	{
		iInfoRow++;

		char		szInfo[0xff]	= {0,};
		char		szPrice[0xff]	= {0,};
		NUMBERFMT	nFmt			= { 0, 0, 3, ".", ",", 1 };

		wsprintf(szInfo, "%u", dwPrice);
		::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szPrice, 15);
		
		if(GetType(pItem->GetID() / ITEM_DISTRIBUTE) == ITEM_SMALL)
		{
			// "1 개당 %s Karz"
			wsprintf(szItemInfo[iInfoRow], g_Message[ETC_MESSAGE812].szMessage, szPrice); 
		}
		else
		{
			// "%s Karz"
			wsprintf(szItemInfo[iInfoRow], g_Message[ETC_MESSAGE870].szMessage, szPrice); 
		}

		nSize = lstrlen(szItemInfo[iInfoRow]);
			
		if(nMaxSize<nSize)
			nMaxSize = nSize;

		iInfoRow++;
	}

	for( i = 0; i < iInfoRow; i++ )
	{
		if(__strcmp(szItemInfo[i], "") != 0)
		{
			nSize = lstrlen(szItemInfo[i]);	
			
			RenderFont(szItemInfo[i]
				, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION - nOverXSize
				, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION + nSize * 7 - nOverXSize
				, g_Mouse.MousePos.y + 5 + (i * 18)  - nOverYSize
				, g_Mouse.MousePos.y + 5 + (i * 18) + 14  - nOverYSize
				, __ORDER_ITEM_DESC__+ 3
				, pdwTextColor[i]);
		}
	}

	if(isEquip && byOption)
	{	
		switch(byOption)
		{
			case 6:		wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1132].szMessage);		break; // "+6 페오 마스터"
			case 7:		wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1133].szMessage);		break; // "+7 윈 마스터"
			case 8:		wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1134].szMessage);		break; // "+8 오셀 마스터"
			case 9:		wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1135].szMessage);		break; // "+9 제라 마스터"
			case 10:	wsprintf(szUpgradeOption[9], g_Message[ETC_MESSAGE1136].szMessage);		break; // "+10 베오르크 마스터"
		}
		
		nSize = lstrlen(szUpgradeOption[9]);		

		RenderFont(szUpgradeOption[9]
			, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION - nOverXSize
			, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION + nSize * 7 - nOverXSize
			, g_Mouse.MousePos.y + 5 + ((iInfoRow+1) * 18) - nOverYSize
			, g_Mouse.MousePos.y + 5 + ((iInfoRow+1) * 18) + 14 - nOverYSize
			, __ORDER_ITEM_DESC__+3
			, TEXT_COLOR_GREEN);

		if(nMaxSize<nSize)
			nMaxSize = nSize;
		
		iInfoRow += 2;
		
		for(i = iInfoRow + 1; i < iInfoRow + 6; i++)
		{
			if(__strcmp(szUpgradeOption[i-(iInfoRow+1)], "") != 0)
			{
				nSize = lstrlen(szUpgradeOption[i-(iInfoRow+1)]);

				RenderFont(szUpgradeOption[i-(iInfoRow+1)]
					, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION - nOverXSize
					, g_Mouse.MousePos.x + 10 + ITEMINFO_RENDER_POSITION + nSize * 7 - nOverXSize
					, g_Mouse.MousePos.y + 5 + (i * 18) - nOverYSize
					, g_Mouse.MousePos.y + 5 + (i * 18) + 14 - nOverYSize
					, __ORDER_ITEM_DESC__+ 3
					, dwUpgradeTextColor[i-(iInfoRow+1)]);				
			}
		}

		iInfoRow += 6;
	}

	RenderInfoBox(float(g_Mouse.MousePos.x + ITEMINFO_RENDER_POSITION) - nOverXSize
		, float(g_Mouse.MousePos.y-5) - nOverYSize
		, (float)(nMaxSize*6.5f+10)
		, (float)(iInfoRow*18+10)
		, __ORDER_ITEM_DESC__);
}


void CInterface::GetDueDayText(char* pszItemInfo, DWORD dwDueDay)
{
	struct tm *when;
	when = localtime((time_t*)&dwDueDay);
	
	if(when)//널 포인터 참조 수정 : 050103 hwoarang
	{
	// "유효기간 %04d/%02d/%02d %02d:%02d분까지입니다."
		wsprintf(pszItemInfo, g_Message[ETC_MESSAGE845].szMessage, when->tm_year+1900, when->tm_mon+1 ,when->tm_mday, when->tm_hour, when->tm_min);
	}
}


void CInterface::GetLandTimeText(char* pszItemInfo, CItem* pItem)
{
		
}


void CInterface::RenderInfoBox(float fPosX, float fPosY, float fSizeX, float fSizeY, int nOrder)
{
	VECTOR2 vScaling;
	VECTOR2	vPos;
	
	vPos.x		= fPosX;
	vPos.y		= fPosY;
	vScaling.x	= fSizeX/2.0f;
	vScaling.y	= fSizeY/2.0f;
	g_pRenderer->RenderSprite(CItemWnd::GetInstance()->m_pBoxSpr, &vScaling, 0.0f, &vPos, NULL, 0xffffffff, nOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
	
	vPos.x		= fPosX;
	vPos.y		= fPosY;
	vScaling.x	= fSizeX/2.0f;
	vScaling.y	= 1.0f;
	g_pRenderer->RenderSprite(CItemWnd::GetInstance()->m_pLineSpr, &vScaling, 0.0f, &vPos, NULL, 0xffffffff, nOrder+1, RENDER_TYPE_DISABLE_TEX_FILTERING);
	
	vPos.x		= fPosX;
	vPos.y		= fPosY+fSizeY;
	vScaling.x	= fSizeX/2.0f;
	vScaling.y	= 1.0f;
	g_pRenderer->RenderSprite(CItemWnd::GetInstance()->m_pLineSpr, &vScaling, 0.0f, &vPos, NULL, 0xffffffff, nOrder+1, RENDER_TYPE_DISABLE_TEX_FILTERING);

	vPos.x		= fPosX;
	vPos.y		= fPosY;
	vScaling.x	= 1.0f;
	vScaling.y	= fSizeY/2.0f;
	g_pRenderer->RenderSprite(CItemWnd::GetInstance()->m_pLineSpr, &vScaling, 0.0f, &vPos, NULL, 0xffffffff, nOrder+1, RENDER_TYPE_DISABLE_TEX_FILTERING);

	vPos.x		= fPosX+fSizeX;
	vPos.y		= fPosY;
	vScaling.x	= 1.0f;
	vScaling.y	= fSizeY/2.0f;
	g_pRenderer->RenderSprite(CItemWnd::GetInstance()->m_pLineSpr, &vScaling, 0.0f, &vPos, NULL, 0xffffffff, nOrder+1, RENDER_TYPE_DISABLE_TEX_FILTERING);
}

