//==================================================//
// Code by Jang.									//
//==================================================//
#pragma once

// 모든 인터페이스 사용하는 스프라이트는 이곳에 Define
// 하고 사용해야 한다. 

#include <windows.h>

#define SPR_TYPE_ALL		0
#define SPR_TYPE_DUNGEON	1

void InterfaceSprLoad(BYTE byType);
void InterfaceSprRemove(BYTE byType);

#define SPR_INTERFACE_DF_WND1									1		// .\Data\UI\df_window1.tif
#define SPR_INTERFACE_DF_WND2									2		// .\Data\UI\df_window2.tif
#define SPR_INTERFACE_BANK_WND3									3		// .\Data\UI\itembank2.tif
#define SPR_INTERFACE_BANK_CHK1									4		// .\Data\UI\item_menu1.tif
#define SPR_INTERFACE_BANK_CHK2									5		// .\Data\UI\item_menu2.tif
#define SPR_INTERFACE_BANK_BAR									6		// .\Data\UI\itembank1.tif
#define SPR_INTERFACE_CLOSE1									7		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_CLOSE2									8		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_SC_BG										9		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_SC										10		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_MONEY1									11		// .\Data\UI\menu_2.tga
#define SPR_INTERFACE_MONEY2									12		// .\Data\UI\menu_2.tga
#define SPR_INTERFACE_MONEY3									13		// .\Data\UI\menu_2.tga
#define SPR_INTERFACE_DF_WND3									14		// .\Data\UI\df_window3.tif
#define SPR_INTERFACE_DUNGEONINFOEX_WND1						15		// .\Data\UI\dungeon_1.tif
#define SPR_INTERFACE_DUNGEONINFOEX_WND2						16		// .\Data\UI\dungeon_2.tif
#define SPR_INTERFACE_DUNGEONINFOEX_WND3						17		// .\Data\UI\dungeon_3.tif
#define SPR_INTERFACE_DUNGEONINFOEX_WND4						18		// .\Data\UI\dungeon_4.tif
#define SPR_INTERFACE_DUNGEONINFOEX_STATUS1						19		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_STATUS2						20		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_STATUS3						21		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_INFORMATION1				22		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_INFORMATION2				23		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_INFORMATION3				24		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_PAGENEXT1									25		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_PAGENEXT2									26		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_PAGENEXT3									27		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_PAGEPRE1									28		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_PAGEPRE2									29		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_PAGEPRE3									30		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_PAGEBOX									31		// .\Data\UI\menu_1.tga	
#define SPR_INTERFACE_DUNGEONINFOEX_PRODUCTION1					32		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_PRODUCTION2					33		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_PRODUCTION3					34		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_SELECTBAR					35		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_DUNGEONINFOEX_MANAGEMENT1					36		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_DUNGEONINFOEX_MANAGEMENT2					37		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_DUNGEONINFOEX_MANAGEMENT3					38		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_DUNGEONMANAGER_WND						39		// .\Data\UI\dungeon_manage1.tif
#define BUTTON_INTERFACE_DUNGEONMANAGERMENTMODE_DEFENSE1		40		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_DUNGEONMANAGERMENTMODE_DEFENSE3		41		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_DUNGEONMANAGERMENTMODE_LEVELUP1		42		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_DUNGEONMANAGERMENTMODE_LEVELUP3		43		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_DUNGEONMANAGERMENTMODE_PRODUCTION1		44		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_DUNGEONMANAGERMENTMODE_PRODUCTION3		45		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_ACQUISITION1							46		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_ACQUISITION2							47		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_ACQUISITION3							48		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_DUNGEONMANAGER_MOF1						49		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_DUNGEONMANAGER_MOF2						50		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_DUNGEONMANAGER_MOF3						51		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_OK2_WND									52		// .\Data\UI\okwindow2.tif
#define SPR_INTERFACE_OK1										53		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_OK2										54		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_OK3										55		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_CHAR_WINDOWS1								56		// .\Data\UI\Chrwindow1.tif
#define SPR_INTERFACE_CHAR_WINDOWS2								57		// .\Data\UI\Chrwindow2.tif
#define BUTTON_INTERFACE_UP1									58		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_UP2									59		// .\Data\UI\menu_1.tga					
#define SPR_INTERFACE_CHATLIST_WND1								60		// .\Data\UI\Chatlist1.tif
#define SPR_INTERFACE_CHATLIST_WND2								61		// .\Data\UI\Chatlist2.tif
#define SPR_INTERFACE_CHATLIST_WND3								62		// .\Data\UI\Chatlist3.tif
#define SPR_INTERFACE_CHATLIST_UPDOWN1							63		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_CHATLIST_UPDOWN2							64		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_CHATLIST_ALL1								65		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_ALL2								66		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_ALL3								67		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_PARTY1							68		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_PARTY2							69		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_PARTY3							70		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_GUILD1							71		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_GUILD2							72		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_GUILD3							73		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_WHISPER1							74		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_WHISPER2							75		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_WHISPER3							76		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_FRIEND1							77		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_FRIEND2							78		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_FRIEND3							79		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHATLIST_SC1								80		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_CHATLIST_SC2								81		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_CHATLIST_SC3								82		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_CHATLIST_SC4								83		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_CHAT_WND									84		// .\Data\UI\chat_window.tif
#define SPR_INTERFACE_CHAT_CHAT1								85		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHAT_CHAT2								86		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHAT_MESSAGE1								87		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHAT_MESSAGE2								88		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHAT_LIST1								89		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHAT_LIST2								90		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_CHAT_LIST3								91		// .\Data\UI\7
#define SPR_INTERFACE_DF_WND4									92		// .\Data\UI\df_window4.tif
#define SPR_INTERFACE_DF_WND5									93		// .\Data\UI\df_window5.tif
#define SPR_INTERFACE_DF_WND6									94		// .\Data\UI\df_window6.tif
#define SPR_INTERFACE_DFCMD										95		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_OKWND1									96		// .\Data\UI\okwindow.tif
#define	SPR_INTERFACE_CANCEL1									97		// .\Data\UI\menu_3.tga
#define	SPR_INTERFACE_CANCEL2									98		// .\Data\UI\menu_3.tga
#define	SPR_INTERFACE_CANCEL3									99		// .\Data\UI\menu_3.tga
#define	SPR_INTERFACE_PARTY_RED									100		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_PARTY_BLUE								101		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_PARTY_WHITE								102		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_PARTY_GREEN								103		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_PARTY_BORA								104		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_PARTY_YELLOW								105		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_PARTY_ORANGE								106		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_PARTY_PINK								107		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_MINIMAP									108		// .\Data\UI\minimap.tif
#define	SPR_INTERFACE_DSMAP_USER								109		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_MAP										110
#define	SPR_INTERFACE_DF7										111		// .\Data\UI\df_window7.tif
#define	SPR_INTERFACE_DF8										112		// .\Data\UI\df_window8.tif
#define	SPR_INTERFACE_DF9										113		// .\Data\UI\df_window9.tif
#define	BUTTON_INTERFACE_GAME_EXIT1								114		// .\Data\UI\menu_2.tga
#define	BUTTON_INTERFACE_GAME_EXIT2								115		// .\Data\UI\menu_2.tga
#define	BUTTON_INTERFACE_GAME_EXIT3								116		// .\Data\UI\menu_2.tga
#define	BUTTON_INTERFACE_CHAR_SELECT1							117		// .\Data\UI\menu_2.tga
#define	BUTTON_INTERFACE_CHAR_SELECT2							118		// .\Data\UI\menu_2.tga
#define	BUTTON_INTERFACE_CHAR_SELECT3							119		// .\Data\UI\menu_2.tga
#define	BUTTON_INTERFACE_GAME_RETURN1							120		// .\Data\UI\menu_2.tga
#define	BUTTON_INTERFACE_GAME_RETURN2							121		// .\Data\UI\menu_2.tga
#define	BUTTON_INTERFACE_GAME_RETURN3							122		// .\Data\UI\menu_2.tga
#define	SPR_INTERFACE_OPTIONWND									123		// .\Data\UI\optionwindow.tif
#define	SPR_INTERFACE_OPTION_CHECK								124		// .\Data\UI\checkv1.tif
#define	BUTTON_INTERFACE_OPTION_EXIT1							125		// .\Data\UI\menu_3.tga
#define	BUTTON_INTERFACE_OPTION_EXIT2							126		// .\Data\UI\menu_3.tga
#define	BUTTON_INTERFACE_OPTION_EXIT3							127		// .\Data\UI\menu_3.tga
#define	SPR_INTERFACE_OPTION_VOL								128		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_GUILDEDIT_BAR								129		// .\Data\UI\guile_edit1.tif
#define	SPR_INTERFACE_GROUPWND									130		// .\Data\UI\group_window.tif
#define	SPR_INTERFACE_GROUPWND_PARTY1							131		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_GROUPWND_PARTY2							132		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_GROUPWND_FRIEND1							133		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_GROUPWND_FRIEND2							134		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_GROUPWND_GUILDWAR1						135		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_GROUPWND_GUILDWAR2						136		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_GROUPWND_GUILD1							137		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_GROUPWND_GUILD2							138		// .\Data\UI\menu_4.tif
#define	SPR_INTERFACE_GROUPWND_SELECT							139		// .\Data\UI\menu_4.tif	  	
#define SPR_INTERFACE_GROUPWND_ADD1								140		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_ADD2								141		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_ADD3								142		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_DELETE1							143		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_DELETE2							144		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_DELETE3							145		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_INFO1							146		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_INFO2							147		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_INFO3							148		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_OUT1								149		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_OUT2								150		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_OUT3								151		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_MSG1								152		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_MSG2								153		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_MSG3								154		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GUILD_ONOFF1								155		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GUILD_ONOFF2								156		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GUILD_ONOFF3								157		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GUILD_INFO1								158		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GUILD_INFO2								159		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GUILD_INFO3								160		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GUILD_LEVEL								161		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_GROUPWND_GUILDWARADD1						162		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUPWND_GUILDWARADD2						163		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUPWND_GUILDWARADD3						164		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUPWND_GUILDWAREXIT1					165		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUPWND_GUILDWAREXIT2					166		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUPWND_GUILDWAREXIT3					167		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUPWND_STATUS							168		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUP_MASTER1								169		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUP_MASTER2								170		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GROUP_MASTER3								171		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GUILDWAR_GUILD							172		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_GUILD_MARK								173		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_HELP_NEXT1								174		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_HELP_NEXT2								175		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_HELP_NEXT3								176		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_HELP_HP									177		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_SP									178		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_COOL									179		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_EXP									180		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_ITEM									181		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_CHAR									182		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_SKILL								183		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_GROUP								184		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_DUNGEON								185		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_CHAT									186		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_SHOP									187		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_MATCH								188		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_REST									189		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_TRADE								190		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_PK									191		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_OPTION								192		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_SLOT									193		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_SUPPLISE								194		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_INTERFACE1							195		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_UP1									196		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_UP2									197		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_INTERFACE2							198		// .\Data\UI\help01.tga
#define SPR_INTERFACE_HELP_INTERFACE3							199		// .\Data\UI\help01.tga
#define SPR_INTERFACE_ITEMSHOPWND								200		// .\Data\UI\itemshopwnd.tif
#define BUTTON_INTERFACE_MAKE1									201		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_MAKE2									202		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_MAKE3									203		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_STR1									204		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_STR2									205		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_STR3									206		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_TRADE1									207		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_TRADE2									208		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_TRADE3									209		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_INFO1									210		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_INFO2									211		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_INFO3									212		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_BANK_BTN1									213		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_BANK_BTN2									214		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_BANK_BTN3									215		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_ITEMTRADESHOP_WND							216		// .\Data\UI\TradeWindow.tif
#define SPR_INTERFACE_ITEMTRADESHOP_TRADE1						217		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_ITEMTRADESHOP_TRADE2						218		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_ITEMTRADESHOP_TRADE3						219		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_ITEMTRADESHOP_TRADE4						220		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_ITEMTRADESHOP_OK0							221		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_ITEMUPGRADEWND							222		// .\Data\UI\strength_window.tif
#define SPR_INTERFACE_ITEMUPGRADE_HELPWND						223		// .\Data\UI\window_help.tif
#define BUTTON_INTERFACE_ITEMUPGRADE1							224		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_ITEMUPGRADE2							225		// .\Data\UI\menu_3.tga
#define BUTTON_INTERFACE_ITEMUPGRADE3							226		// .\Data\UI\menu_3.tga
#define SPR_INTERFACE_ITEMWINDOW								227		// .\Data\UI\itemwindow.tif
#define SPR_INTERFACE_ITEMWINDOW2								228		// .\Data\UI\itemwindow1.tif
#define SPR_INTERFACE_ITEM_CHK1									229		// .\Data\UI\item_menu1.tif
#define SPR_INTERFACE_ITEM_CHK2									230		// .\Data\UI\item_menu2.tif
#define SPR_INTERFACE_ITEMWND_GUARDIAN_OFF1						231		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_ITEMWND_GUARDIAN_OFF2						232		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_ITEMWND_GUARDIAN_ON1						233		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_ITEMWND_GUARDIAN_ON2						234		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_PLAYER_SHOP_WND							235		// .\Data\UI\playershop_a.tif
#define BUTTON_INTERFACE_PLAYERSHOP_OPEN1						236		// .\Data\UI\menu_4.tga
#define BUTTON_INTERFACE_PLAYERSHOP_OPEN2						237		// .\Data\UI\menu_4.tga
#define BUTTON_INTERFACE_PLAYERSHOP_OPEN3						238		// .\Data\UI\menu_4.tga
#define BUTTON_INTERFACE_PLAYERSHOP_CLOSE1						239		// .\Data\UI\menu_4.tga
#define BUTTON_INTERFACE_PLAYERSHOP_CLOSE2						240		// .\Data\UI\menu_4.tga
#define BUTTON_INTERFACE_PLAYERSHOP_CLOSE3						241		// .\Data\UI\menu_4.tga
#define BUTTON_INTERFACE_PLAYERSHOPWND_CANCEL1					242		// .\Data\UI\menu_4.tga
#define BUTTON_INTERFACE_PLAYERSHOPWND_CANCEL2					243		// .\Data\UI\menu_4.tga
#define BUTTON_INTERFACE_PLAYERSHOPWND_CANCEL3					244		// .\Data\UI\menu_4.tga
#define SPR_INTERFACE_QUANTITY_WND								245		// .\Data\UI\quantitywindow.tif
#define SPR_INTERFACE_RANKNPC_RANK1								246		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_RANKNPC_RANK2								247		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_RANKNPC_RANK3								248		// .\Data\UI\menu_5.tga
#define SPR_INTERFACE_UI_LEFT									249		// .\Data\UI\interface_Left.tif
#define SPR_INTERFACE_UI_RIGHT									250		// .\Data\UI\interface_Right.tif
#define SPR_INTERFACE_EN2										251		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_EN1										252		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_EXP										253		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_MANA2										254		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_MANA1										255		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_CAST										256		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_ITEM1									257		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_CHR1									258		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_SKILL1									259		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_GUILD1									260		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_PARTY1									261		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_CHAT1									262		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_ITEM2									263		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_CHR2									264		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_SKILL2									265		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_GUILD2									266		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_PARTY2									267		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_CHAT2									268		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_SYSTEM1								269		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_SYSTEM2								270		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_PKON1									271		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_PKON2									272		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_PKOFF1									273		// .\Data\UI\menu_1.tga
#define BUTTON_INTERFACE_PKOFF2									274		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_MONSTERHP_BAR1							275		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_MONSTERHP_BAR2							276		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_MONSTER_GAGE1								277		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_MONSTER_GAGE2								278		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_USERHP_BAR1								279		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_USERHP_BAR2								280		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_USER_GAGE1								281		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_USER_GAGE2								282		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_GUARDIAN_WND1								283		// .\Data\UI\guardian_gauge.tif
#define SPR_INTERFACE_GUARDIAN_EN								284		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_TRADE1									285		// .\Data\UI\menu_1.tga		
#define SPR_INTERFACE_UI_TRADE2									286		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_TRADE3									287		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_SHOP1									288		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_SHOP2									289		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_SHOP3									290		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_REST1									291		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_REST2									292		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_REST3									293		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_MATCH1									294		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_MATCH2									295		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_MATCH3									296		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_UI_QUICKSLOT_INABLE						297		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_WND1								298		// .\Data\UI\skillwindow.tif
#define SPR_INTERFACE_SKILL_WND2								299		// .\Data\UI\skillwindow_a.tif
#define SPR_INTERFACE_SKILL_MANA1								300		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_MANA2								301		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_MANA3								302		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_AURA1								303		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_AURA2								304		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_AURA3								305		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_DIVINE1								306		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_DIVINE2								307		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_DIVINE3								308		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_SUMMON1								309		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_SUMMON2								310		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_SUMMON3								311		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_CHAKRA1								312		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_CHAKRA2								313		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_CHAKRA3								314		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_SKILL_UP1									315		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_SKILL_UP2									316		// .\Data\UI\menu_1.tga
#define SPR_INTERFACE_GROUPWND_PARTYBAR							317		// .\Data\UI\menu_4.tif
#define SPR_INTERFACE_GROUPWND_PARTYGAGE						318		// .\Data\UI\menu_1.tga
#define	SPR_INTERFACE_OKWND2									319		// .\Data\UI\okwindow2.tif
#define BUTTON_INTERFACE_GAME_WORLD1							320		// .\Data\UI\menu_2.tga
#define BUTTON_INTERFACE_GAME_WORLD2							321		// .\Data\UI\menu_2.tga
#define BUTTON_INTERFACE_GAME_WORLD3							322		// .\Data\UI\menu_2.tga

#define	SPR_INTERFACE_PARTYBOARD_ENTRY1							323		// .\Data\UI\menu_5.tga
#define	SPR_INTERFACE_PARTYBOARD_ENTRY2							324		// .\Data\UI\menu_5.tga
#define	SPR_INTERFACE_PARTYBOARD_ENTRY3							325		// .\Data\UI\menu_5.tga
#define	SPR_INTERFACE_PARTYBOARD_SEA1							326		// .\Data\UI\menu_5.tga
#define	SPR_INTERFACE_PARTYBOARD_SEA2							327		// .\Data\UI\menu_5.tga
#define	SPR_INTERFACE_PARTYBOARD_SEA3							328		// .\Data\UI\menu_5.tga
#define	SPR_INTERFACE_PARTYBOARD_HELP1							329		// .\Data\UI\menu_5.tga
#define	SPR_INTERFACE_PARTYBOARD_HELP2							330		// .\Data\UI\menu_5.tga
#define	SPR_INTERFACE_PARTYBOARD_HELP3							331		// .\Data\UI\menu_5.tga

#define	SPR_INTERFACE_PARTYHELP_CREATE1							332		// .\Data\UI\menu_6.tga
#define	SPR_INTERFACE_PARTYHELP_CREATE2							333		// .\Data\UI\menu_6.tga
#define	SPR_INTERFACE_PARTYHELP_CREATE3							334		// .\Data\UI\menu_6.tga
#define	SPR_INTERFACE_PARTYHELP_DESTROY1						335		// .\Data\UI\menu_6.tga
#define	SPR_INTERFACE_PARTYHELP_DESTROY2						336		// .\Data\UI\menu_6.tga
#define	SPR_INTERFACE_PARTYHELP_DESTROY3						337		// .\Data\UI\menu_6.tga

#define	SPR_INTERFACE_PARTYHELP_WND								338		// .\Data\UI\party_reg.tga
#define	BUTTON_INTERFACE_HELP_END1		                		339   	// .\Data\UI\menu_6.tga
#define	BUTTON_INTERFACE_HELP_END2            					340   	// .\Data\UI\menu_6.tga
#define	BUTTON_INTERFACE_HELP_END3                				341   	// .\Data\UI\menu_6.tga

#define SPR_CHAR_TAB											342
#define SPR_CHAR_GUARDIAN_LIFE									343
#define SPR_GUARDIAN_STATUS_WND1								344		// .\Data\UI\Guar_Window_1.TIF
/*
#define SPR_LOGIN_BACKGROUND									344
#define SPR_LOGIN_SERVERSELECT_WINDOW							345
#define SPR_CS_BLACK_LINE										346
#define SPR_LOGIN_LOGIN_WINDOW									347
#define BUTTON_LOGIN_OK_NORMAL									348
#define BUTTON_LOGIN_OK_OVER									349
#define BUTTON_LOGIN_OK_DOWN									350
#define BUTTON_LOGIN_CANCEL_NORMAL								351
#define BUTTON_LOGIN_CANCEL_OVER								352
#define BUTTON_LOGIN_CANCEL_DOWN								353
#define BUTTON_LOGIN_EXIT_NORMAL								354
#define BUTTON_LOGIN_EXIT_OVER									355
#define BUTTON_LOGIN_EXIT_DOWN									356
*/

// add by minjin. Ballon Chatting Window. 2004. 10. 02.
#define SPR_INTERFACE_BALOON_CHAT_WND_LTOP						345		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_CTOP						346		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_RTOP						347		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_LMID						348		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_CMID						349		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_RMID						350		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_LBOT						351		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_CBOT						352		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_RBOT						353		//	.\Data\UI\chat_bln.tif
#define SPR_INTERFACE_BALOON_CHAT_WND_TAIL						354		//	.\Data\UI\chat_bln.tif

#define SPR_GUARDIAN_STATUS_WND2								355		// .\Data\UI\Guar_Window_2.TIF

//파티 강화 시스템 (정우창 2005.01.25)
#define SPR_INTERFACE_GROUPWND_PARTYUSERINFO1					356     //  .\Data\UI\munu_4.tga  파티창 6번째 버튼 비활성화 (사용안함)
#define SPR_INTERFACE_GROUPWND_PARTYUSERINFO2					357		//	.\Data\UI\munu_4.tga  파티창 6번째 버튼 활성화 
#define SPR_INTERFACE_GROUPWND_PARTYUSERINFO3					358		//	.\Data\UI\munu_4.tga  파티창 6번째 버튼 버튼 클릭시 
#define SPR_INTERFACE_GROUPWND_EXPITEMCONDITION1				359		//	.\Data\UI\munu_5.tga  파티창 7번째 버튼 비활성화 (사용안함) 
#define SPR_INTERFACE_GROUPWND_EXPITEMCONDITION2				360		//	.\Data\UI\munu_5.tga  파티창 7번째 버튼 활성화 
#define SPR_INTERFACE_GROUPWND_EXPITEMCONDITION3				361		//	.\Data\UI\munu_5.tga  파티창 7번째 버튼 버튼 클릭시 
#define SPR_INTERFACE_GROUPWND_PMATCHCONDITION1					362		//	.\Data\UI\munu_6.tga  파티창 8번째 버튼 비활성화 (사용안함)
#define SPR_INTERFACE_GROUPWND_PMATCHCONDITION2					363		//	.\Data\UI\munu_6.tga  파티창 8번째 버튼 활성화	
#define SPR_INTERFACE_GROUPWND_PMATCHCONDITION3					364		//	.\Data\UI\munu_6.tga  파티창 8번째 버튼 버튼 클릭시  
#define SPR_INTERFACE_GROUPWND_PMATCHCONDITION_OK_OFF			365		//	.\Data\UI\munu_7.tga  매칭시스템설정 후 확인 버튼(비활성화) 
//#define SPR_INTERFACE_GROUPWND_PARTYFRIENDADD1					366		//	.\Data\UI\munu_7.tga  파티 시 친구추가 버튼(비활성화) 
#define SPR_INTERFACE_GROUPWND_CONDITIONLIST1					367		//	.\Data\UI\munu_7.tga  매칭시스템설정 리스트 박스 오른쪽
#define SPR_INTERFACE_GROUPWND_CONDITIONLIST2					368		//	.\Data\UI\munu_7.tga  매칭시스템설정 리스트 박스 가운데 
#define SPR_INTERFACE_GROUPWND_CONDITIONLIST3					369		//	.\Data\UI\munu_7.tga  매칭시스템설정 리스트 박스 버튼 활성화 
#define SPR_INTERFACE_GROUPWND_CONDITIONLIST4					370		//	.\Data\UI\munu_7.tga  매칭시스템설정 리스트 박스 버튼 클릭 
#define SPR_INTERFACE_GROUPWND_CONDITIONLIST5					371		//	.\Data\UI\munu_7.tga  매칭시스템설정 리스트 박스 버튼 비활성화 
#define SPR_INTERFACE_GROUPWND_CONDITIONLIST6					372		//	.\Data\UI\munu_7.tga  매칭시스템설정 리스트 박스 백판 	
#define SPR_INTERFACE_GROUPWND_PMATCHCONDITION_OK_ON			373		//	.\Data\UI\munu_7.tga  매칭시스템설정 후 확인 버튼(활성화) 
//#define SPR_INTERFACE_GROUPWND_PARTYFRIENDADD1					366		//	.\Data\UI\munu_7.tga  파티 시 친구추가 버튼(비활성화) 
//#define SPR_INTERFACE_GROUPWND_PARTYFRIENDADD1					366		//	.\Data\UI\munu_7.tga  파티 시 친구추가 버튼(비활성화) 

// 가디언 3차 : 최덕석 2005.03.29
#define SPR_INTERFACE_UI_GUARDIAN_STATUS						414		// .\Data\UI\menu_6.tga	가디언 스탯창 열기 버튼

//==================================================//
// End.												//
//==================================================//
