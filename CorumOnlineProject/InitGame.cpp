#pragma once


#include "stdafx.h"
#include <Imm.h>
#ifdef	SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGXGFunc/global.h"
#else
#include "./../4DyuchiGXGFunc/global.h"
#endif
#include "define.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "effect.h"
#include "HashTable.h"
#include "ObjectManager.h"
#include "SpriteManager.h"
#include "../CommonServer/CommonHeader.h"
#include "MonsterManager.h"
#include "DungeonTable.h"
#include "IntroProcess.h"
#include "LoginProcess.h"
#include "CharSelectProcess.h"
#include "WorldProcess.h"
#include "DungeonProcess.h"
#include "Ini.h"
#include "Struct.h"
#include "GMTool.h"
#include "d3d8.h"
#include <dsound.h>
#include "message.h"
#include "DamageNumber.h"
#include "GlobalSoundHandles.h"
#include "MatchInfo.h"
#include "resource.h"
#include "InputManager.h"
#include "Version.h"
#include "PathFinding\SearchModule_Astar.h"
#include "IdentyGraphicCard.h"
#include "ChrInfo.h"
#include "GameControl.h"
#include "SoundControl.h"
#include "ListTable.h"
#include "GlobalVariableLogin.h"
#include "CodeFun.h"
#include "NetworkTimeObserver.h"
#include "DFXForm.h"
#include "ItemMoveManager.h"
#include "ItemUsedManager.h"
#include "InterfaceInstance.h"
#include "ItemAttrLayer.h"
#include "../CommonServer/CommonHeader.h"
#ifdef _USE_IME
#include "IMEEdit.h"
#endif 


#pragma comment(lib, ".\\CResNameResolver.lib")
#pragma comment(lib, ".\\SS3DGFunc.lib")
#pragma comment(lib, ".\\soundlib.lib")
#pragma comment(lib, ".\\CommonServer.lib")



//==================================================================================
// Global Variable : 여기다가 이쁘게 선언할것 딴데다가 하지마셈 
//==================================================================================
BYTE						g_byChatLineCnt			= 2;// sung-han 2005-03-30 노말채팅 라인수 1:5줄, 2:10줄, 3:15줄
BYTE						g_byCharMoveCnt			= 0;
BYTE						g_byCharMoveSuccess		= 0;
BYTE						g_byCharMoveFail		= 0;
BYTE						g_bExecuteType			= 0;
BYTE						g_byStatusMessenger		= 0;
BYTE						g_byChkMessenger		= 0;
BYTE						g_bGM					= 0;
BYTE						g_byCharacterAllCount	= 0;
#if IS_JAPAN_LOCALIZING()
BYTE						g_bVilType				= 1;
#else	
BYTE						g_bVilType				= 0;
#endif
char						g_szKeyOtherInfo[10][128];

DWORD						g_dwVersion				= 0;
DWORD						g_dwStatusTime			= 0;
DWORD						g_dwEventMapID			= 0;	
DWORD						g_dwFontFlag			= SS3D_FONT;
BOOL						g_bSmallBankChk			= FALSE;
BOOL						g_bCharMoveChk			= FALSE;
BOOL						g_bLoadingChk			= FALSE;
float						g_fBGMVolume			= 0.0f;
float						g_fEffectVolume			= 0.0f;
float						g_fDist;
VECTOR3						g_v3Temp;
WORD						g_wEffectLevel			= 0;	// 이펙트 단계 : 최덕석 2005.04.01

HINSTANCE					g_hInstance;
HWND						g_hMainWnd				= NULL;
HIMC						g_hImc;
HFONT						g_hFont;			
HMODULE						g_hExecutiveHandle		= NULL;
HMODULE						g_hBaseNetworkHandle	= NULL;
HCURSOR						g_hCursor[__MAX_MOUSE_POINTER__];

DUNGEONPRODUCTIONITEMMINMAX	g_DungeonProductionItemMinMax[DUNGEON_PROPERTY_MAX];
NPC_TABLE					g_NPCTable[MAX_NPC_NUM_PER_VILLAGE];
BASE_CLASS_INFO				g_sBaseClassInfo[5];
SSKILL_DPINFO				g_sSkillInfoDP[MAX_SKILL];
SLEVEL_EXP					g_sLevelExp[MAX_LEVEL+1];
SLEVEL_EXP					g_sGuardianLevelExp[MAX_LEVEL+1];
SSKILL_LIST_MANAGER			g_sSkillListManager;
GUILD_INFO					g_pGuildInfo;
A_STAR						g_PathFinder;
eGRAPHIC_CARD_OPTION		g_enGraphicCardOption;
DEVELOP_VARIABLE			g_Dev;
SKEY_CONFIG					g_sKeyConfig;

GXOBJECT_HANDLE				g_hHandle				= NULL;
GXOBJECT_HANDLE				g_hEffectHandle			= NULL;

I4DyuchiGXExecutive*		g_pExecutive			= NULL;
I4DyuchiGXGeometry*			g_pGeometry				= NULL;
I4DyuchiGXRenderer*			g_pRenderer				= NULL;

CNetworkClient*				g_pNet					= NULL;
IDIFontObject*				g_pFont[MAX_FONT_NUM];
CObjectManager*				g_pObjManager			= NULL;						
CSpriteManager*				g_pSprManager			= NULL;
CDungeonTable*				g_pDungeonTable			= NULL;	

CMessagePool				g_Message;
CMessagePool				g_CmdMessage;
CMessagePool				g_ConvMessage;
CMessagePool				g_NotConvMessage;
CMessagePool				g_Emoticon;

CResNameResolver			g_Res;
CResNameResolver			g_ResDefined;
CItemMoveManager			g_ItemMoveManager;
CItemUsedManager			g_ItemUsedManager;

STMPOOL_HANDLE				g_pObjDescPool			= NULL;
STMPOOL_HANDLE				g_pUserPool				= NULL;
STMPOOL_HANDLE				g_pMonsterPool			= NULL;
STMPOOL_HANDLE				g_pItemPool				= NULL;
STMPOOL_HANDLE				g_pItemStorePool		= NULL;
STMPOOL_HANDLE				g_pItemOptionPool		= NULL;
STMPOOL_HANDLE				g_pItemResourcePool		= NULL;
STMPOOL_HANDLE				g_pGroupInfoPool		= NULL;
STMPOOL_HANDLE				g_pMessengerUserPool	= NULL;
STMPOOL_HANDLE				g_pGuildWarPool			= NULL;
STMPOOL_HANDLE				g_pPartyBoardPool		= NULL;
STMPOOL_HANDLE				g_pGuildOfflinePool		= NULL;
STMPOOL_HANDLE				g_pGuildDataPool		= NULL;
STMPOOL_HANDLE				g_pHelpInfoPool			= NULL;
STMPOOL_HANDLE				g_pSkillReosurcePool	= NULL;
STMPOOL_HANDLE				g_pSoundEffectPool		= NULL;
STMPOOL_HANDLE				g_pInterfaceSprPool		= NULL;
STMPOOL_HANDLE				g_pGuildPool			= NULL;
STMPOOL_HANDLE				g_pPartyPool			= NULL;
STMPOOL_HANDLE				g_pChatPool				= NULL;
STMPOOL_HANDLE				g_pEffectPool			= NULL;

LPBASEITEM_HASH				g_pItemTableHash		= NULL;
CUserHash*					g_pUserHash				= NULL;
CMonsterHash*				g_pMonsterHash			= NULL;
CEffectHash*				g_pEffectHash			= NULL;
ITEM_HASH*					g_pItemHash				= NULL;
CPartyBoardHash*			g_pPartyBoardHash		= NULL;
CGuildListHash*				g_pGuildWarHash			= NULL;
CGuildInfoHash*				g_pGuildInfoHash		= NULL;
CGuildOffLineHash*			g_pGuildOfflineHash		= NULL;
CItemOptionHash*			g_pItemOptionHash		= NULL;
CItemStoreHash*				g_pItemStoreHash		= NULL;
CItemResourceHash*			g_pItemResourceHash		= NULL;
CHelpInfoHash*				g_pHelpInfoHash			= NULL;
CSetItemInfoHash*			g_pSetItemInfoHash		= NULL;
CItemMakingInfoHash*		g_pItemMakingInfoHash	= NULL;
CPartyUserHash*				g_pPartyUserHash		= NULL;
CGroupInfoHash*				g_pGroupInfoHash		= NULL;
CMessengerUserHash*			g_pMessengerOnLineHash	= NULL;
CMessengerUserHash*			g_pMessengerOffLineHash	= NULL;
CInterfaceSprHash*			g_pInterfaceSprHash		= NULL;
CGuildUserHash*				g_pGuildUserHash		= NULL;

COnlyList*					g_pAllChatList			= NULL;
COnlyList*					g_pPartyChatList		= NULL;
COnlyList*					g_pGuildChatList		= NULL;
COnlyList*					g_pWhisperChatList		= NULL;
COnlyList*					g_pFriendChatList		= NULL;

ChrInfoLayer*				g_pChrInfoUser			= NULL;
ChrInfoLayer*				g_pChrInfoMonster		= NULL;
ChrInfoLayer*				g_pChrInfoEffect		= NULL;
EffectLayer*				g_pEffectLayer			= NULL;
CMonsterManager*			g_pMonsterManager		= NULL;
CInputManager*				g_pInputManager			= NULL;
CItemAttrLayer*				g_pItemAttrLayer		= NULL;

char* g_pszKeyArray[0xff] = 
{
	"A",	"B",	"C",	"D",	"E",	"F",	"G",	"H",	"I",	"J",	"K",	"L",
	"M",	"N",	"O",	"P",	"Q",	"S",	"T",	"U",	"V",	"W",	"X",	"Y",
	"Z",	"0",	"1",	"2",	"3",	"4",	"5",	"6",	"7",	"8",	"9",	"F1",
	"F2",	"F3",	"F4",	"F5",	"F6",	"F7",	"F8",	"F9",	"F10",	"F11",	"F12",	";",
	"'",	"[",	"]",	"-",	"=",	"\\",	",",	".",	"/",	"SPACE"
};

int g_nKey[0xff] = 
{
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C,
	0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
	0x5A, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0xBA,
	0xDE, 0xDB, 0xDD, 0xBD, 0xBB, 0xDC, 0xBC, 0xBE, 0xBF, 0x20,
};

int g_nHanKey[0xff] = 
{
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C,
	0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
	0x5A, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0xBA,
	0xDE, 0xDB, 0xDD, 0xBD, 0xBB, 0xDC, 0xBC, 0xBE, 0xBF, 0x20,		
};

void	(*OnKeyDown[MAX_UPDATE_GAME])		(WPARAM wParam, LPARAM lParam);
void	(*OnKeyUp[MAX_UPDATE_GAME])			(WPARAM wParam, LPARAM lParam);
void	(*OnLButtonDown[MAX_UPDATE_GAME])	(WPARAM wParam, LPARAM lParam);
void	(*OnLButtonUp[MAX_UPDATE_GAME])		(WPARAM wParam, LPARAM lParam);
void	(*OnRButtonDown[MAX_UPDATE_GAME])	(WPARAM wParam, LPARAM lParam);
void	(*OnRButtonUp[MAX_UPDATE_GAME])		(WPARAM wParam, LPARAM lParam);
void	(*OnMouseMove[MAX_UPDATE_GAME])		(WPARAM wParam, LPARAM lParam);
void	(*OnMouseWheel[MAX_UPDATE_GAME])	(WPARAM wParam, LPARAM lParam);
void	(*OnTimerEvent[MAX_UPDATE_GAME])	(DWORD dwTimerIndex);
void	(*OnChar[MAX_UPDATE_GAME])			(BYTE bCh);
//==================================================================================
// Global Variable : 여기다가 이쁘게 선언할것 딴데다가 하지마셈 
//==================================================================================


DWORD GetEncryptedVersion(char* szSource)
{
	DFXform Xform;
	
	char szTemp[0xff]={0,};
	char szTemp2[0xff]={0,};
	UINT nSize = 0;

	Xform.XXDecode((LPBYTE)szSource, lstrlen(szSource)+1, (LPBYTE)szTemp, &nSize);
	Xform.UUDecode((LPBYTE)szTemp, nSize, (LPBYTE)szTemp2, &nSize);

	return (DWORD)atol(szTemp2);
}

void LoadBaseClassInfo()
{
	_CHECK_MEMORY();
	DecodeCDBData(GetFile("baseclassinfo.cdb", DATA_TYPE_MANAGER), g_sBaseClassInfo);
	_CHECK_MEMORY();
}

void LoadLevelExp()
{
	_CHECK_MEMORY();	
	DecodeCDBData(GetFile("Level.cdb", DATA_TYPE_MANAGER), &g_sLevelExp[1]);
	DecodeCDBData(GetFile("GuardianLevel.cdb", DATA_TYPE_MANAGER), &g_sGuardianLevelExp[1]);	
	_CHECK_MEMORY();
}

DWORD GetExpTableOfLevel(GAME_OBJECT_TYPE eObjectType, DWORD dwLevel)
{
	switch (eObjectType)
	{
	case OBJECT_TYPE_PLAYER:
		return g_sLevelExp[dwLevel].dwExp;
	case OBJECT_TYPE_MONSTER:
		return g_sGuardianLevelExp[dwLevel].dwExp;
	}

	return 0;
}


DWORD GetGuardianItemLevel(DWORD dwExp)
{	
	DWORD dwLow = 1, dwHigh = MAX_LEVEL;
	DWORD dwMidLevel = (dwHigh-dwLow)/2;
	DWORD dwMidExp;
	DWORD dwPrevExp;
	int cnt = 0;

	while(dwMidLevel)
	{
		dwMidExp = GetExpTableOfLevel(OBJECT_TYPE_MONSTER, dwMidLevel);
		dwPrevExp = GetExpTableOfLevel(OBJECT_TYPE_MONSTER, dwMidLevel-1);

		if(dwMidExp == 0)
			dwMidExp = -1;
		if(dwPrevExp == 0)
			dwPrevExp = -1;

		if (dwMidLevel == 1 || dwMidLevel == MAX_LEVEL)
			break;
		else if (dwExp < dwMidExp)
		{
			dwHigh = dwMidLevel;
			if (dwPrevExp <= dwExp)
			{
				--dwMidLevel;
				break;
			}

			dwMidLevel -= (dwHigh-dwLow)/2;
		}
		else if (dwMidExp < dwExp)
		{
			dwLow = dwMidLevel;
			if (dwPrevExp >= dwExp)
			{
				++dwMidLevel;
				break;
			}

			dwMidLevel += (dwHigh-dwLow)/2;
				
		}
		else
			break;
	
		if(cnt++ > 200)
			break;
	}

	return dwMidLevel;
}


BOOL LoadCDBMessage()
{
	int nTotalSize = 0;

	if(!g_Message.LoadTextResource(GetFile("message.cdb", DATA_TYPE_MANAGER)))						return FALSE;
	if(!g_ConvMessage.LoadTextResource(GetFile("filter_conv_message.cdb", DATA_TYPE_MANAGER)))		return FALSE;
	if(!g_NotConvMessage.LoadTextResource(GetFile("filter_notconv_message.cdb", DATA_TYPE_MANAGER)))return FALSE;
	if(!g_CmdMessage.LoadTextResource(GetFile("cmd_message.cdb", DATA_TYPE_MANAGER)))				return FALSE;
	if(!g_Emoticon.LoadTextResource(GetFile("emoticon.cdb", DATA_TYPE_MANAGER)))					return FALSE;

	nTotalSize = DecodeCDBData(GetFile("DungeonProductionItemMinMax.cdb", DATA_TYPE_MANAGER)
		, g_DungeonProductionItemMinMax);
	
	return TRUE;
}

void PreLoadResource()
{
	PreLoadDamageNumber();

	// Head //
	char szInfo[0xff] = {0,};
	for(int i = 1; i <= 22; i++)
	{							
		wsprintf(szInfo, "ph1010%02d.MOD", i);		
		g_pExecutive->PreLoadGXObject(GetFile(szInfo, DATA_TYPE_CHARACTER));
		wsprintf(szInfo, "ph2010%02d.MOD", i);		
		g_pExecutive->PreLoadGXObject(GetFile(szInfo, DATA_TYPE_CHARACTER));
	}

	// Shadow //
	for(i = 1; i <= 4; i++)
	{
		wsprintf(szInfo, "df_shadow%02d.MOD", i);		
		g_pExecutive->PreLoadGXObject(GetFile(szInfo, DATA_TYPE_CHARACTER));	
	}
	
	// 기본 몸 //
	for(i = 1; i <= 5; i++)
	{
		wsprintf(szInfo, "pm0%d000.chr", i);		
		g_pExecutive->PreLoadGXObject(GetFile(szInfo, DATA_TYPE_CHARACTER));	
	}
	
	g_pExecutive->PreLoadGXObject(GetFile("itembotary.chr", DATA_TYPE_ITEM));	
	g_pExecutive->PreLoadGXObject(GetFile("click_a.mod", DATA_TYPE_UI));
	g_pExecutive->PreLoadGXObject(GetFile("e0121000.chr", DATA_TYPE_EFFECT));
	g_pExecutive->PreLoadGXObject(GetFile("e0125000.chr", DATA_TYPE_EFFECT));
	g_pExecutive->PreLoadGXObject(GetFile("e0128000.chr", DATA_TYPE_EFFECT));
}


void InitializePool()
{
	g_pObjDescPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pObjDescPool, sizeof(OBJECT_DESC), 20000, 40000);

	g_pUserPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pUserPool, sizeof(CUser), MAX_DATA_FOR_USER_POOL, MAX_DATA_FOR_USER_POOL);

	g_pMonsterPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pMonsterPool, sizeof( CMonster ), 110, 110 );

	g_pEffectPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pEffectPool, sizeof( EffectDesc ), 1000, 1000 );
	
	g_pItemPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pItemPool, sizeof( ITEM ), 2000, 2000);

	g_pGuildPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pGuildPool, sizeof(GUILD_USER), 200, 200);	
	
	g_pMessengerUserPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pMessengerUserPool, sizeof(MESSENGER_USER), 80, 80);	

	g_pGuildWarPool	= CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pGuildWarPool, sizeof(SGUILD_WAR), 10, 10);		

	g_pPartyBoardPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pPartyBoardPool, sizeof(SPARTY_BOARD), 100, 1000);		

	g_pItemResourcePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pItemResourcePool, sizeof(SITEM_RESOURCE_EX), 2000, 2000);	
	
	g_pPartyPool		= CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pPartyPool, sizeof( PARTY_USER ), 100, 100 );

	g_pChatPool			= CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pChatPool, sizeof( CHAT_MSG), 2048, 2048 );
	
	g_pItemStorePool	= CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pItemStorePool, sizeof( ITEM_STORE ), 2000, 2000 );

	g_pItemOptionPool	= CreateStaticMemoryPool();
	InitializeStaticMemoryPool( g_pItemOptionPool, sizeof( ITEM_OPTION ), 700, 700 );
		
	g_pSkillReosurcePool	= CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pSkillReosurcePool, sizeof(SSKILL_RESOURCE_EX), 200, 200);
	
	g_pHelpInfoPool			= CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pHelpInfoPool, sizeof(SHELP_INFO), __MAX_INTERFACE_MESSAGE, __MAX_INTERFACE_MESSAGE);

	g_pGroupInfoPool		= CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pGroupInfoPool, sizeof(SGROUPINFO_TABLE), 12, 12);

	g_pGuildOfflinePool	= CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pGuildOfflinePool, sizeof(GUILD_OFFLINE_USER), 110, 110);
	
	g_pGuildDataPool	= CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pGuildDataPool, sizeof(GUILD_DATA), 500, 500);

	g_pInterfaceSprPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(g_pInterfaceSprPool, sizeof(SSPRITE_DATA), 1000, 1000);
}


void InitializeHash()
{
	g_pGuildInfoHash		= new CGuildInfoHash;		g_pGuildInfoHash->InitializeHashTable( 300, 300);
	g_pGuildOfflineHash		= new CGuildOffLineHash;	g_pGuildOfflineHash->InitializeHashTable( 100, 100);
	g_pItemOptionHash		= new CItemOptionHash;		g_pItemOptionHash->InitializeHashTable( 10000, 10000);
	g_pItemStoreHash		= new CItemStoreHash;		g_pItemStoreHash->InitializeHashTable( 10000, 10000);
	g_pUserHash				= new CUserHash;			g_pUserHash->InitializeHashTable( 500, 500 );
	g_pMonsterHash			= new CMonsterHash;			g_pMonsterHash->InitializeHashTable( 500, 500 );
	g_pEffectHash			= new CEffectHash;			g_pEffectHash->InitializeHashTable( 1000, 1000 );
	g_pItemHash				= new ITEM_HASH;			g_pItemHash->InitializeHashTable( 10000, 10000);	
	g_pItemTableHash		= new BASEITEM_HASH;		g_pItemTableHash->InitializeHashTable(10000, 10000);
	g_pSetItemInfoHash		= new CSetItemInfoHash;		g_pSetItemInfoHash->InitializeHashTable( 200, 200 );
	g_pItemMakingInfoHash	= new CItemMakingInfoHash;	g_pItemMakingInfoHash->InitializeHashTable( 200, 200 ); 
	g_pItemResourceHash		= new CItemResourceHash;	g_pItemResourceHash->InitializeHashTable(10000, 10000);
	g_pHelpInfoHash			= new CHelpInfoHash;		g_pHelpInfoHash->InitializeHashTable(__MAX_INTERFACE_MESSAGE, __MAX_INTERFACE_MESSAGE);
	g_pPartyUserHash		= new CPartyUserHash;		g_pPartyUserHash->InitializeHashTable(200, MAX_PARTY_USER*2);
	g_pGroupInfoHash		= new CGroupInfoHash;		g_pGroupInfoHash->InitializeHashTable(12, 12);
	g_pGuildUserHash		= new CGuildUserHash;		g_pGuildUserHash->InitializeHashTable(MAX_GUILD_USER, MAX_GUILD_USER);
	g_pMessengerOnLineHash	= new CMessengerUserHash;	g_pMessengerOnLineHash->InitializeHashTable(40, 40);	
	g_pMessengerOffLineHash	= new CMessengerUserHash;	g_pMessengerOffLineHash->InitializeHashTable(40, 40);		
	g_pGuildWarHash			= new CGuildListHash;		g_pGuildWarHash->InitializeHashTable(10, 10);
	g_pPartyBoardHash		= new CPartyBoardHash;		g_pPartyBoardHash->InitializeHashTable(100, 1000);
	g_pInterfaceSprHash		= new CInterfaceSprHash;	g_pInterfaceSprHash->InitializeHashTable(1000, 1000);
	g_pAllChatList			= new COnlyList(__MAX_MSG_CNT__, DeleteAllChatList);
	g_pPartyChatList		= new COnlyList(__MAX_MSG_CNT__, DeletePartyChatList);
	g_pGuildChatList		= new COnlyList(__MAX_MSG_CNT__, DeleteGuildChatList);
	g_pFriendChatList		= new COnlyList(__MAX_MSG_CNT__, DeleteFriendChatList);
	g_pWhisperChatList		= new COnlyList(__MAX_MSG_CNT__, DeleteWhisperChatList);	
	g_pChrInfoEffect		= new ChrInfoLayer;
	g_pChrInfoEffect->Init(MAX_EFFECT_KIND, 1, 1);
	g_pChrInfoMonster		= new ChrInfoLayer;
	g_pChrInfoMonster->Init(MAX_MONSTER_KIND, 15, 1);
	g_pChrInfoUser			= new ChrInfoLayer;
	g_pChrInfoUser->Init(MAX_CHAR_KIND, 50, 9);
	g_pItemAttrLayer		= new CItemAttrLayer;
	g_pInputManager			= new CInputManager;
#ifdef SOUND_NEW
	g_pSoundControl			= new CSoundControl;
#endif	
}


void LoadCursorDoIt()
{
	memset(g_hCursor, 0, sizeof(g_hCursor));	
	g_hCursor[ __MOUSE_POINTER_DEFAULT__	  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_COMMON_C));
	g_hCursor[ __MOUSE_POINTER_DEFAULTCLICK__ ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_COMMON));
	g_hCursor[ __MOUSE_POINTER_BUTTON__		  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_BUTTON_C));
	g_hCursor[ __MOUSE_POINTER_BUTTONCLICK__  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_BUTTON));
	g_hCursor[ __MOUSE_POINTER_ITEM__		  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ITEM_C));
	g_hCursor[ __MOUSE_POINTER_ITEMCLICK__	  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ITEM));
	g_hCursor[ __MOUSE_POINTER_ATTACK__		  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ATTACK_C));
	g_hCursor[ __MOUSE_POINTER_ATTACKCLICK__  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_ATTACK));
	g_hCursor[ __MOUSE_POINTER_NPC__		  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_DIALOG_C));
	g_hCursor[ __MOUSE_POINTER_NPCCLICK__	  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_DIALOG));
	g_hCursor[ __MOUSE_POINTER_GUARDIAN__	  ] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_TARGET_C));
	g_hCursor[ __MOUSE_POINTER_GUARDIANCLICK__] = LoadCursor(g_hInstance, MAKEINTRESOURCE(IDC_CURSOR_TARGET));
}


BOOL InitGame()
{
	g_enGraphicCardOption = SpecGraphicCard();

	if(g_enGraphicCardOption == VOODOO)
	{
		// MSG_ID : 444 ; 게임에서 지원하지 않는 그래픽 카드입니다.
		MessageBox(g_hMainWnd, g_Message[ETC_MESSAGE444].szMessage, "CorumOnlineProject", MB_OK);	
		return FALSE;
	}

	Create_NTO();
	
	GetCurrentDirectory( _MAX_PATH, g_szBasePath );
	GetCurrentDirectory( _MAX_PATH, g_szDataPath );
	lstrcat( g_szDataPath, "\\Data" );
	
#ifdef __PACKAGE_MODE__	
	GetCurrentDirectory( _MAX_PATH, g_szPackPath );
	lstrcat( g_szPackPath, "\\Data" );
#endif

	InitInterfaceInstances();

	g_dwVersion = GetEncryptedVersion(CURRENT_VERSION_STRING);

	g_Res.Init("CorumResource.erd", g_szBasePath);	
	g_ResDefined.Init("DefResource.erd", g_szBasePath);		
	SetRect( &g_rcScreenRect, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );

	InitFunctionPointer();
	InitPacketProc();
	if(!InitCOMObject())	return FALSE;
	if(!CreateFont())		return FALSE;
	
	g_pRenderer->InitializeRenderTarget(256, 1);
	
#ifdef _USE_IME
		GET_IMEEDIT()->InitializeIME(g_pExecutive, g_hMainWnd);	
		GET_IMEEDIT()->SetStatusWndPos(800, 600);			
#if IS_JAPAN_LOCALIZING()
		GET_IMEEDIT()->SetEditFonts(-12, SHIFTJIS_CHARSET);	
#elif IS_CHINA_LOCALIZING()
		GET_IMEEDIT()->SetEditFonts(-12, GB2312_CHARSET);
#elif IS_TAIWAN_LOCALIZING()
		GET_IMEEDIT()->SetEditFonts(-12, CHINESEBIG5_CHARSET);
#else
		GET_IMEEDIT()->SetEditFonts(-12, DEFAULT_CHARSET);
#endif
#endif	
	g_hImc	= ImmGetContext( g_hMainWnd );	
	
	BOOL	bCreated = CreateSoundLib( &g_pSoundLib );
	if( bCreated == FALSE)
	{
		OutputDebugString("Could not create sound library\n");
	}

	if (g_pSoundLib)
	{
		SOUND_ERROR_CODE sResult = g_pSoundLib->Init( 6000, 1000, "redist", FALSE, TRUE);
		if( sResult != SOUND_ERROR_NOERROR )
		{
			OutputDebugString("Sound Library Init Failed!\n");
			g_pSoundLib->Delete();
			g_pSoundLib = NULL;
		}
		else
		{
			g_pSoundLib->SetMaxVoiceCount(8);
		}
	}
	
	g_pGeometry->GetCameraDesc(&g_Camera.CameraDesc, 0);
	g_Camera.v3Angle.x =  g_Camera.CameraDesc.fXRot;
	g_Camera.v3Angle.y =  g_Camera.CameraDesc.fYRot;
	g_Camera.v3Angle.z =  g_Camera.CameraDesc.fZRot;

	SetListener(&g_Camera.v3Angle);
	LoadCursorDoIt();
	InitializePool();
	InitializeHash();	
	LoadSkillresourceTable();	
	LoadItemResourceTable();
	LoadBaseItemTable();
	LoadItemTable();	
	if(!LoadCDBMessage()) return FALSE;
	LoadNpcInfo();
	LoadBaseClassInfo();	
	LoadCPTable();	
	LoadLevelExp();
	PreLoadResource();

	g_pSprManager = new CSpriteManager(10000);
	g_pObjManager = new CObjectManager(100);
	
	InitializeEffect(TRUE);
		
	g_pDungeonTable		= new CDungeonTable(1000);		
	g_pMonsterManager	= new CMonsterManager;

	LoadPreLoadInfo();

	g_pExecutive->GetRenderer()->CreateEffectShaderPaletteFromFile(GetFile("special_effect.efs", DATA_TYPE_UI));

	int	nKey	= 0;
	int	nRt		= 0;

	for(int i = 0; i < MAX_KEY; i++)
		g_sKeyConfig.snKey[i] = -1;	
	
	char szInfo[0xff]	= {0,};
	char szReturn[0xff] = {0,};
	char szFile[0xff]	= {0,};
	
	char* szTemp[MAX_KEY] = 
	{
		"SKILL OPEN", "GROUP OPEN", "WEAPON SWITCH", "CHARACTER OPEN", "INVENTORY OPEN",
		"CHAT CLEAR", "OPTION OPEN", "AUTO CASTING TOGGLE1", "AUTO CASTING TOGGLE2",
		
		"ITEM BELT1", "ITEM BELT2", "ITEM BELT3", "ITEM BELT4", "ITEM BELT5", "ITEM BELT6", 
		"ITEM BELT7", "ITEM BELT8", "ITEM GUARDIAN",
		
		"SKILL1", "SKILL2", "SKILL3", "SKILL4", "SKILL5", "SKILL6", "SKILL7", "SKILL8",
		"SKILL9", "SKILL10", "SKILL11", "SKILL12", "SKILL13", "SKILL14", "SKILL15",  "SKILL16",

		"ITEM", "GUARDIAN OPEN"
	};

#if IS_TAIWAN_LOCALIZING()
	char* szDefault[MAX_KEY] =
	{
		"S", "P", "W", "A", "T", "L", "O", "F", "G", 
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", 
		"F10", "F11", "F12", "", "", "", "", "", "", "", "", "",
		"Z", "X", "C", "V", "SPACE", "G"
	};
#else
	char* szDefault[MAX_KEY] =
	{
		"S", "P", "W", "A", "T", 
		"L", "O", "F", "G", 
		
		"1", "2", "3", "4", "5", "6", 
		"7", "8", "9", 
		
		"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", 
		"F9", "F10", "F11", "F12", "Z", "X", "C", "V", 

		"SPACE", "G"
	};	
#endif	
	wsprintf(szFile, "%s\\%s", g_szBasePath, "KeyConfig.ini");
	
	for(i = 0; i < MAX_KEY; i++)
	{
		GetPPS("KEY CONFIG", szTemp[i], szDefault[i], szReturn, 256, szFile);	
		
		nKey	= KeySearch(szReturn);
		nRt		= KeyCheck(nKey);
		
		if(nKey==-1 || nRt==-1)
		{
			for(int j = 0; j < MAX_KEY; j++)
			{
				g_sKeyConfig.snKey[j] = KeySearch(szDefault[j]);
				wsprintf(szInfo, " %s", szDefault[j]);
				WritePPSting("KEY CONFIG", szTemp[j], szInfo, szFile);
			}
			break;
		}
		g_sKeyConfig.snKey[i]	= nKey;
	}	

	wsprintf(szInfo, "%s\\%s", g_szBasePath, "OptionConfig.ini");
	
	int nBGMVolume		= GetPPI("OPTION CONFIG", "BGMVOLUME", 50, szInfo);
	int nEffectVolume	= GetPPI("OPTION CONFIG", "EFFECTVOLUME", 50, szInfo);
	// sung-han 2005-04-01 채팅창의 라인수-----------------------------------------
//	char szTmpDefault[0xff] = {0,};
//	char szTmpReturn[0xff] = {0,};
//	memset(szTmpDefault, 0, sizeof(szTmpReturn));
//	memset(szTmpReturn, 0, sizeof(szTmpDefault));
//	strcpy(szTmpReturn, "MIDDLE");
//	GetPPS("OPTION CONFIG", "TEXTLINE", szTmpDefault, szTmpReturn, 256, szInfo);
//	if(__strcmp(szTmpDefault, "SHORT")==0)
//		g_byChatLineCnt = 1;
//	else if(__strcmp(szTmpDefault, "MIDDLE")==0)
//		g_byChatLineCnt = 2;
//	else if(__strcmp(szTmpDefault, "LONG")==0)
//		g_byChatLineCnt = 3;
//	else
//		g_byChatLineCnt = 2;
	// ----------------------------------------------------------------------------
	
	g_fBGMVolume	= (float)nBGMVolume/100.0f;;
	g_fEffectVolume = (float)nEffectVolume/100.0f;;
		
	LoadHelpInfo();
	LoadGroupInfo();
	InitDamageNumber();
	InterfaceSprLoad(0);

	return TRUE;
}


void ReleaseGame()
{
	Delete_NTO();

	(*ReleaseGameProcess[GetGameStatus()])();

	ReleaseMainPlayer();
	
	IMESetEdit(0);	

#ifdef SOUND_NEW
	if (g_pSoundLib)
	{
		if (g_pSoundControl)
		{
			delete g_pSoundControl;
			g_pSoundControl = NULL;
		}
	}
#endif
	
	if (g_pSoundLib)
	{
		StopBGM();
		g_SoundEffectBasketCase.ForceAllClean();
		Release_Sound_Resource();

		g_pSoundLib->Delete();
		g_pSoundLib = NULL;
	}

#ifdef GM_TOOL
	if(g_pGVGm)
	{
		delete g_pGVGm;
		g_pGVGm = NULL;
	}
#endif
	
	if (g_pInputManager)
	{
		delete g_pInputManager;
		g_pInputManager = NULL;
	}

	if (g_pItemAttrLayer)
	{
		delete g_pItemAttrLayer;
		g_pItemAttrLayer = NULL;
	}

	if(g_pChrInfoUser)
	{
		g_pChrInfoUser->Remove();
		delete  g_pChrInfoUser;
		g_pChrInfoUser = NULL;
	}
	
	if(g_pChrInfoMonster)
	{
		g_pChrInfoMonster->Remove();
		delete  g_pChrInfoMonster;
		g_pChrInfoMonster = NULL;
	}
			
	if(g_pChrInfoEffect)
	{
		g_pChrInfoEffect->Remove();
		delete  g_pChrInfoEffect;
		g_pChrInfoEffect = NULL;
	}
	
	for(int i=0; i<MAX_FONT_NUM; i++)
	{
		g_pFont[i]->Release();
		g_pFont[i] = NULL;	
	}

	if(g_pDungeonTable)
	{
		delete g_pDungeonTable;
		g_pDungeonTable = NULL;
	}
	
	if(g_pSprManager)
	{
		delete g_pSprManager;
		g_pSprManager = NULL;
	}

	if(g_pObjManager)
	{
		delete g_pObjManager;
		g_pObjManager = NULL;
	}
	
	if(g_pEffectLayer)
	{
		delete g_pEffectLayer;
		g_pEffectLayer = NULL;
	}

	InterfaceSprRemove(0);

	if(g_pGVLogin)
	{
		delete g_pGVLogin;
		g_pGVLogin = NULL;
	}

	for(i = 0; i < 5; i++)
	{
		POSITION_ pos = g_sSkillListManager.pSkillList[i].pActiveList->GetHeadPosition();	
		POSITION_ posTemp;
	
		while(pos)
		{
			posTemp = pos;

			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pActiveList->GetNext(pos);			
			g_sSkillListManager.pSkillList[i].pActiveList->RemoveAt(posTemp);
			LALFree(g_pSkillReosurcePool, lpSkillResourceNode);
		}

		delete g_sSkillListManager.pSkillList[i].pActiveList;
		g_sSkillListManager.pSkillList[i].pActiveList = NULL;

		pos = g_sSkillListManager.pSkillList[i].pPassiveList->GetHeadPosition();	
				
		while(pos)
		{
			posTemp = pos;

			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pPassiveList->GetNext(pos);			
			g_sSkillListManager.pSkillList[i].pPassiveList->RemoveAt(posTemp);
			LALFree(g_pSkillReosurcePool, lpSkillResourceNode);
		}

		delete g_sSkillListManager.pSkillList[i].pPassiveList;
		g_sSkillListManager.pSkillList[i].pPassiveList = NULL;

		pos = g_sSkillListManager.pSkillList[i].pOverDriveList->GetHeadPosition();	
				
		while(pos)
		{
			posTemp = pos;

			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pOverDriveList->GetNext(pos);			
			g_sSkillListManager.pSkillList[i].pOverDriveList->RemoveAt(posTemp);
			LALFree(g_pSkillReosurcePool, lpSkillResourceNode);
		}
		
		delete g_sSkillListManager.pSkillList[i].pOverDriveList;
		g_sSkillListManager.pSkillList[i].pOverDriveList = NULL;

		pos = g_sSkillListManager.pSkillList[i].pMasteryList->GetHeadPosition();	
				
		while(pos)
		{
			posTemp = pos;

			LP_SKILL_RESOURCE_EX lpSkillResourceNode = (LP_SKILL_RESOURCE_EX)g_sSkillListManager.pSkillList[i].pMasteryList->GetNext(pos);			
			g_sSkillListManager.pSkillList[i].pMasteryList->RemoveAt(posTemp);
			LALFree(g_pSkillReosurcePool, lpSkillResourceNode);
		}						

		delete g_sSkillListManager.pSkillList[i].pMasteryList;
		g_sSkillListManager.pSkillList[i].pMasteryList = NULL;
	}		

	if( g_pGuildUserHash )
	{
		g_pGuildUserHash->Destroy(TRUE);
		delete g_pGuildUserHash;
		g_pGuildUserHash = NULL;
	}	

	g_helpLoadingSprite.ReleaseSprites();	
			
	if(g_pAllChatList)
	{
		g_pAllChatList->RemoveAllData();		
		delete g_pAllChatList;
		g_pAllChatList = NULL;
	}

	if(g_pPartyChatList)
	{
		g_pPartyChatList->RemoveAllData();
		delete g_pPartyChatList;
		g_pPartyChatList = NULL;
	}
	
	if(g_pFriendChatList)
	{
		g_pFriendChatList->RemoveAllData();		
		delete g_pFriendChatList;
		g_pFriendChatList = NULL;
	}

	if(g_pGuildChatList)
	{
		g_pGuildChatList->RemoveAllData();
		delete g_pGuildChatList;
		g_pGuildChatList = NULL;
	}

	if(g_pWhisperChatList)
	{
		g_pWhisperChatList->RemoveAllData();
		delete g_pWhisperChatList;
		g_pWhisperChatList = NULL;
	}

	if( g_pUserHash )
	{
		g_pUserHash->Destroy();
		delete g_pUserHash;
		g_pUserHash = NULL;
	}
	
	if(g_pGroupInfoHash)
	{
		g_pGroupInfoHash->Destroy(TRUE);
		delete g_pGroupInfoHash;
		g_pGroupInfoHash = NULL;
	}
			
	if(g_pGuildOfflineHash)
	{
		g_pGuildOfflineHash->Destroy(TRUE);
		delete g_pGuildOfflineHash;
		g_pGuildOfflineHash = NULL;
	}
	
	if(g_pGuildInfoHash)
	{
		g_pGuildInfoHash->Destroy(TRUE);
		delete g_pGuildInfoHash;
		g_pGuildInfoHash = NULL;
	}

	if(g_pGuildDataPool)
	{
		ReleaseStaticMemoryPool(g_pGuildDataPool);
		g_pGuildDataPool = NULL;
	}

	if(g_pGuildOfflinePool)
	{
		ReleaseStaticMemoryPool(g_pGuildOfflinePool);
		g_pGuildOfflinePool = NULL;
	}

	if(g_pGroupInfoPool)
	{
		ReleaseStaticMemoryPool(g_pGroupInfoPool);
		g_pGroupInfoPool = NULL;
	}

	if(g_pUserPool)
	{
		ReleaseStaticMemoryPool(g_pUserPool);
		g_pUserPool = NULL;
	}

	if(g_pItemStoreHash)
	{
		g_pItemStoreHash->Destroy(TRUE);
		delete g_pItemStoreHash;
		g_pItemStoreHash = NULL;
	}

	if(g_pItemOptionHash)
	{
		g_pItemOptionHash->Destroy(TRUE);
		delete g_pItemOptionHash;
		g_pItemOptionHash = NULL;
	}

	if(g_pSkillReosurcePool)
	{
		ReleaseStaticMemoryPool(g_pSkillReosurcePool);
		g_pSkillReosurcePool = NULL;
	}
	
	if(g_pItemOptionPool)
	{
		ReleaseStaticMemoryPool(g_pItemOptionPool);
		g_pItemOptionPool = NULL;
	}
		
	if(g_pItemStorePool)
	{
		ReleaseStaticMemoryPool(g_pItemStorePool);
		g_pItemStorePool = NULL;
	}
	
	if(g_pItemTableHash)
	{
		g_pItemTableHash->Destroy(TRUE);
		delete g_pItemTableHash;
		g_pItemTableHash = NULL;
	}
	
	if(g_pSetItemInfoHash)
	{
		g_pSetItemInfoHash->Destroy(TRUE);
		delete g_pSetItemInfoHash;
		g_pSetItemInfoHash = NULL;
	}

	if(g_pItemMakingInfoHash)
	{
		g_pItemMakingInfoHash->Destroy(TRUE);
		delete g_pItemMakingInfoHash;
		g_pItemMakingInfoHash = NULL;
	}

	if(g_pPartyUserHash)
	{
		g_pPartyUserHash->Destroy(TRUE);
		delete g_pPartyUserHash;
		g_pPartyUserHash = NULL;
	}
	
	if(g_pMessengerOnLineHash)
	{
		g_pMessengerOnLineHash->Destroy(TRUE);
		delete g_pMessengerOnLineHash;
		g_pMessengerOnLineHash = NULL;
	}
	
	if(g_pMessengerOffLineHash)
	{
		g_pMessengerOffLineHash->Destroy(TRUE);
		delete g_pMessengerOffLineHash;
		g_pMessengerOffLineHash = NULL;
	}

	if(g_pMessengerUserPool)
	{
		ReleaseStaticMemoryPool(g_pMessengerUserPool);
		g_pMessengerUserPool = NULL;
	}

	if(g_pGuildWarHash)
	{
		g_pGuildWarHash->Destroy(TRUE);
		delete g_pGuildWarHash;
		g_pGuildWarHash = NULL;
	}

	if(g_pPartyBoardHash)
	{
		g_pPartyBoardHash->Destroy(TRUE);
		delete g_pPartyBoardHash;
		g_pPartyBoardHash = NULL;
	}
		
	if(g_pInterfaceSprHash)
	{
		g_pInterfaceSprHash->Destroy(TRUE);
		delete g_pInterfaceSprHash;
		g_pInterfaceSprHash = NULL;
	}

	if(g_pInterfaceSprPool)
	{
		ReleaseStaticMemoryPool( g_pInterfaceSprPool);
		g_pInterfaceSprPool = NULL;
	}

	if(g_pEffectPool)
	{
		ReleaseStaticMemoryPool(g_pEffectPool);
		g_pEffectPool = NULL;
	}	

	if(g_pHelpInfoHash)
	{
		g_pHelpInfoHash->Destroy(TRUE);
		delete g_pHelpInfoHash;
		g_pHelpInfoHash = NULL;
	}		

	if(g_pHelpInfoPool)
	{
		ReleaseStaticMemoryPool(g_pHelpInfoPool);
		g_pHelpInfoPool = NULL;
	}
		
	if(g_pItemResourceHash)
	{
		g_pItemResourceHash->Destroy(TRUE);
		delete g_pItemResourceHash;
		g_pItemResourceHash = NULL;
	}	

	if(g_pGuildWarPool)
	{
		ReleaseStaticMemoryPool(g_pGuildWarPool);
		g_pGuildWarPool = NULL;
	}	

	if(g_pPartyBoardPool)
	{
		ReleaseStaticMemoryPool(g_pPartyBoardPool);
		g_pPartyBoardPool = NULL;
	}	
	
	if(g_pItemResourcePool)
	{
		ReleaseStaticMemoryPool(g_pItemResourcePool);
		g_pItemResourcePool = NULL;
	}

	if( g_pItemHash )
	{
		RemoveAllItem();
		g_pItemHash->Destroy();
		delete g_pItemHash;
		g_pItemHash = NULL;
	}	
	
	if( g_pItemPool )
	{
		ReleaseStaticMemoryPool( g_pItemPool );
		g_pItemPool = NULL;
	}
	
	if( g_pGuildPool )
	{
		ReleaseStaticMemoryPool( g_pGuildPool );
		g_pGuildPool = NULL;
	}
	
	if( g_pPartyPool )
	{
		ReleaseStaticMemoryPool( g_pPartyPool );
		g_pPartyPool = NULL;
	}

	if( g_pChatPool )
	{
		ReleaseStaticMemoryPool( g_pChatPool );
		g_pPartyPool = NULL;
	}
	
	if( g_pEffectHash )
	{
		g_pEffectHash->Destroy();
		delete g_pEffectHash;
		g_pEffectHash = NULL;
	}

	if( g_pMonsterHash )
	{
		g_pMonsterHash->Destroy(TRUE);
		delete g_pMonsterHash;
		g_pMonsterHash = NULL;
	}

	if(g_pMonsterPool)
	{
		ReleaseStaticMemoryPool(g_pMonsterPool);
		g_pMonsterPool = NULL;
	}

	char	szVal[0xff] = {0,};
	char	szInfo[0xff] = {0,};
	
	int nBGMVolume		= int(g_fBGMVolume*100.0f);
	int nEffectVolume	= int(g_fEffectVolume*100.0f);

	wsprintf(szInfo, "%s\\%s", g_szBasePath, "OptionConfig.ini");

	memset(szVal, 0, sizeof(szVal));
	wsprintf(szVal, " %d", nBGMVolume);
	WritePPSting("OPTION CONFIG", "BGMVOLUME", szVal, szInfo);	

	memset(szVal, 0, sizeof(szVal));
	wsprintf(szVal, " %d", nEffectVolume);
	WritePPSting("OPTION CONFIG", "EFFECTVOLUME", szVal, szInfo);	

	ReleaseDamageNumber();
	
	if(g_pObjDescPool)
	{
		ReleaseStaticMemoryPool(g_pObjDescPool);
		g_pObjDescPool = NULL;
	}

	if(g_pExecutive)		
	{
		g_pExecutive->UnloadAllPreLoadedGXObject(UNLOAD_PRELOADED_RESOURCE_TYPE_ALL_PRELOADED);		

#ifdef _USE_IME
		GET_IMEEDIT()->DeleteSprite();
#endif
		g_pExecutive->Release();
		g_pExecutive = NULL;
		g_pGeometry = NULL;
		g_pRenderer = NULL;
	}

#ifdef _USE_IME	
	DESTROY_IMEEDIT();
#endif

	FreeLibrary( g_hExecutiveHandle );
	g_hExecutiveHandle = NULL;

	if(g_pNet)
	{
		g_pNet->Release();
		g_pNet = NULL;
	}

	FreeLibrary( g_hBaseNetworkHandle );
	g_hBaseNetworkHandle = NULL;

	g_Res.UnInit();
	g_ResDefined.UnInit();	
	ImmReleaseContext( g_hMainWnd, g_hImc );

	if( g_pSw )
	{
		delete g_pSw;
		g_pSw = NULL;
	}
	
	if( g_pMonsterManager )
	{		
		delete g_pMonsterManager;
		g_pMonsterManager = NULL;
	}

	DeleteObject((HFONT)g_hFont);
	
	DestroyInterfaceInstances();
}


BOOL ChangeCursor(int nCursorIndex)
{
	if(!g_hCursor[ nCursorIndex ] || nCursorIndex >= __MAX_MOUSE_POINTER__)	
	{
		asm_int3();
		return FALSE;
	}

	SetCursor( g_hCursor[ nCursorIndex ] );
	return TRUE;
}


BOOL IsCompressedTextureFormatOk()
{
	LPDIRECT3D8 lpD3D;

    if( NULL == ( lpD3D = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
	    return FALSE;
	}

	BOOL	bResult = FALSE;
	HRESULT hr;

	D3DDISPLAYMODE			d3ddm;
	if( FAILED( lpD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
	{
		lpD3D->Release();
		return FALSE;
	}

	hr = lpD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,d3ddm.Format,0,D3DRTYPE_TEXTURE,D3DFMT_DXT1);

	if (hr == D3D_OK)
	{
		bResult = TRUE;
	}

	lpD3D->Release();

	return bResult;
}

void InitFunctionPointer()
{
	Render[ RENDER_MODE_NORMAL ] = RenderNormal;
	Render[ RENDER_MODE_MULTIPLE ] = RenderMultiple;

	memset(AfterRender, 0, sizeof(AfterRender));
	memset(BeforeRender, 0, sizeof(BeforeRender));
	
	AfterRender[ UPDATE_GAME_INTRO ]				= (DWORD (__stdcall*)())&AfterRenderGameIntro;
	AfterRender[ UPDATE_GAME_LOGIN ]				= (DWORD (__stdcall*)())&AfterRenderGameLogin;
	AfterRender[ UPDATE_GAME_CHAR_SELECT ]			= (DWORD (__stdcall*)())&AfterRenderGameCharSelect;
	AfterRender[ UPDATE_GAME_WORLD ]				= (DWORD (__stdcall*)())&AfterRenderGameWorld;
	AfterRender[ UPDATE_GAME_PLAY ]					= (DWORD (__stdcall*)())&AfterRenderGameDungeon;

	BeforeRender[ UPDATE_GAME_LOGIN ]				= (DWORD (__stdcall*)())&BeforeRenderGameLogin;
	BeforeRender[ UPDATE_GAME_CHAR_SELECT ]			= (DWORD (__stdcall*)())&BeforeRenderGameCharSelect;
	BeforeRender[ UPDATE_GAME_WORLD ]				= (DWORD (__stdcall*)())&BeforeRenderGameWorld;
	BeforeRender[ UPDATE_GAME_PLAY ]				= (DWORD (__stdcall*)())&BeforeRenderGameDungeon;

	InitGameProcess[ UPDATE_GAME_INTRO ]			= InitGameIntro;
	InitGameProcess[ UPDATE_GAME_LOGIN ]			= InitGameLogin;
	InitGameProcess[ UPDATE_GAME_CHAR_SELECT ]		= InitGameCharSelect;
	InitGameProcess[ UPDATE_GAME_WORLD ]			= InitGameWorld;
	InitGameProcess[ UPDATE_GAME_PLAY ]				= InitGameDungeon;

	UpdateGameProcess[ UPDATE_GAME_INTRO ]			= UpdateGameIntro;
	UpdateGameProcess[ UPDATE_GAME_LOGIN ]			= UpdateGameLogin;
	UpdateGameProcess[ UPDATE_GAME_CHAR_SELECT ]	= UpdateGameCharSelect;
	UpdateGameProcess[ UPDATE_GAME_WORLD ]			= UpdateGameWorld;
	UpdateGameProcess[ UPDATE_GAME_PLAY ]			= UpdateGameDungeon;

	ReleaseGameProcess[ UPDATE_GAME_INTRO ]			= ReleaseGameIntro;
	ReleaseGameProcess[ UPDATE_GAME_LOGIN ]			= ReleaseGameLogin;
	ReleaseGameProcess[ UPDATE_GAME_CHAR_SELECT ]	= ReleaseGameCharSelect;
	ReleaseGameProcess[ UPDATE_GAME_WORLD ]			= ReleaseGameWorld;
	ReleaseGameProcess[ UPDATE_GAME_PLAY ]			= ReleaseGameDungeon;

	OnKeyDown[ UPDATE_GAME_LOGIN ]					= OnKeyDownLogin;
	OnKeyDown[ UPDATE_GAME_CHAR_SELECT ]			= OnKeyDownCharSelect;
	OnKeyDown[ UPDATE_GAME_WORLD ]					= OnKeyDownWorld;
	OnKeyDown[ UPDATE_GAME_PLAY ]					= OnKeyDownDungeon;
	
	OnKeyUp[ UPDATE_GAME_LOGIN ]					= OnKeyUpLogin;
	OnKeyUp[ UPDATE_GAME_CHAR_SELECT ]				= OnKeyUpCharSelect;
	OnKeyUp[ UPDATE_GAME_WORLD ]					= OnKeyUpWorld;
	OnKeyUp[ UPDATE_GAME_PLAY ]						= OnKeyUpDungeon;

	OnLButtonDown[ UPDATE_GAME_LOGIN ]				= OnLButtonDownLogin;
	OnLButtonDown[ UPDATE_GAME_CHAR_SELECT ]		= OnLButtonDownCharSelect;
	OnLButtonDown[ UPDATE_GAME_WORLD ]				= OnLButtonDownWorld;
	OnLButtonDown[ UPDATE_GAME_PLAY ]				= OnLButtonDownDungeon;

	OnLButtonUp[ UPDATE_GAME_LOGIN ]				= OnLButtonUpLogin;
	OnLButtonUp[ UPDATE_GAME_CHAR_SELECT ]			= OnLButtonUpCharSelect;
	OnLButtonUp[ UPDATE_GAME_WORLD ]				= OnLButtonUpWorld;
	OnLButtonUp[ UPDATE_GAME_PLAY ]					= OnLButtonUpDungeon;

	OnRButtonDown[ UPDATE_GAME_LOGIN ]				= OnRButtonDownLogin;
	OnRButtonDown[ UPDATE_GAME_CHAR_SELECT ]		= OnRButtonDownCharSelect;
	OnRButtonDown[ UPDATE_GAME_WORLD ]				= OnRButtonDownWorld;
	OnRButtonDown[ UPDATE_GAME_PLAY ]				= OnRButtonDownDungeon;

	OnRButtonUp[ UPDATE_GAME_LOGIN ]				= OnRButtonUpLogin;
	OnRButtonUp[ UPDATE_GAME_CHAR_SELECT ]			= OnRButtonUpCharSelect;
	OnRButtonUp[ UPDATE_GAME_WORLD ]				= OnRButtonUpWorld;
	OnRButtonUp[ UPDATE_GAME_PLAY ]					= OnRButtonUpDungeon;

	OnChar[ UPDATE_GAME_LOGIN ]						= OnCharLogin;
	OnChar[ UPDATE_GAME_CHAR_SELECT ]				= OnCharCharSelect;
	OnChar[ UPDATE_GAME_WORLD ]						= OnCharWorld;
	OnChar[ UPDATE_GAME_PLAY ]						= OnCharDungeon;

	OnMouseWheel[ UPDATE_GAME_LOGIN ]				= OnMouseWheelLogin;
	OnMouseWheel[ UPDATE_GAME_CHAR_SELECT ]			= OnMouseWheelCharSelect;
	OnMouseWheel[ UPDATE_GAME_WORLD ]				= OnMouseWheelWorld;
	OnMouseWheel[ UPDATE_GAME_PLAY ]				= OnMouseWheelDungeon;

	OnMouseMove[ UPDATE_GAME_LOGIN ]				= OnMouseMoveLogin;
	OnMouseMove[ UPDATE_GAME_CHAR_SELECT ]			= OnMouseMoveCharSelect;
	OnMouseMove[ UPDATE_GAME_WORLD ]				= OnMouseMoveWorld;
	OnMouseMove[ UPDATE_GAME_PLAY ]					= OnMouseMoveDungeon;

	OnTimerEvent[ UPDATE_GAME_LOGIN ]				= OnTimerEventLogin;
	OnTimerEvent[ UPDATE_GAME_CHAR_SELECT ]			= OnTimerEventCharSelect;
	OnTimerEvent[ UPDATE_GAME_WORLD ]				= OnTimerEventWorld;
	OnTimerEvent[ UPDATE_GAME_PLAY ]				= OnTimerEventDungeon;
}


BOOL InitCOMObject()
{
	CoInitialize(NULL);
	g_hExecutiveHandle = LoadLibrary("SS3DExecutiveForCorum.dll");
	CREATE_INSTANCE_FUNC pFunc = (CREATE_INSTANCE_FUNC)GetProcAddress(g_hExecutiveHandle,"DllCreateInstance");
	
	HRESULT hr = pFunc((void**)&g_pExecutive);
	if (hr != S_OK)
	{
		// MSG_ID : 445 ; Can not find CLSID_4DyuchiGXExecutive , 343 ; Error
		MessageBox(g_hMainWnd,g_Message[ETC_MESSAGE445].szMessage,g_Message[ETC_MESSAGE343].szMessage,MB_OK);	
		return FALSE;
	}
	
	DISPLAY_INFO dispInfo;
	dispInfo.dwWidth		= SCREEN_WIDTH;
	dispInfo.dwHeight		= SCREEN_HEIGHT;
	dispInfo.dwBPS			= 2;
	
#ifndef DEVELOP_MODE	
	char szValue[0xff]={0,};
	char szInfo[0xff]={0,};
	wsprintf(szInfo, "%s\\%s", g_szBasePath, "OptionConfig.ini");
	GetPPS("OPTION CONFIG", "WINDOWMODE", "OFF", szValue, 0xff, szInfo);
	
	g_Dev.bFullScreen = 
		BYTE((0 == __strnicmp(szValue, "ON", lstrlen(szValue))) ? 0 : 1);
#else
	g_Dev.bFullScreen =	0;
#endif	

	if(g_Dev.bFullScreen)
	{
		dispInfo.dispType		= FULLSCREEN_WITH_BLT;	
		dispInfo.dwRefreshRate	= 0;
	}
	else
	{
		dispInfo.dispType		= WINDOW_WITH_BLT;	
	}

#ifdef GM_TOOL
	dispInfo.dispType			= WINDOW_WITH_BLT;	
#endif	

#ifdef __PACKAGE_MODE__
	PACKFILE_NAME_TABLE sPackFileNameTable[256];
	int iPackCount = LoadPakList("Paklist.sin", sPackFileNameTable, 256);
	g_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll", 12000, 4096, 64, FILE_ACCESS_METHOD_FILE_OR_PACK,	sPackFileNameTable, iPackCount);
	g_pExecutive->InitializeWithoutRegistry("SS3DGeometryForCorum.dll", "SS3DRendererForCorum.dll", g_hMainWnd, &dispInfo, 10000, 1024, 512, 32, 32, NULL);
#else
	g_pExecutive->InitializeFileStorageWithoutRegistry("SS3DFileStorage.dll", 0, 4096, 0, FILE_ACCESS_METHOD_ONLY_FILE,	NULL, 0);	
	g_pExecutive->InitializeWithoutRegistry("SS3DGeometryForCorum.dll", "SS3DRendererForCorum.dll",	g_hMainWnd,	&dispInfo, 10000, 1024,	512, 32, 32, NULL);
#endif

	g_pGeometry = g_pExecutive->GetGeometry();
	g_pRenderer = g_pExecutive->GetRenderer();
	g_pExecutive->PreCreateLight("light.cfg",0);
	g_pExecutive->SetViewport(0);
	g_pRenderer->SetAlphaRefValue(0);

	DIRECTIONAL_LIGHT_DESC	dirLight;
	dirLight.v3Dir.x	= 1.0f;
	dirLight.v3Dir.y	= -1.0f;
	dirLight.v3Dir.z	= 1.0f;
	dirLight.dwSpecular = 0xffffffff;
	dirLight.dwDiffuse	= 0xffc8c8c8;
	dirLight.dwAmbient	= 0x00b4b4b4;

	g_pGeometry->SetDirectionalLight(&dirLight,0);
	g_pGeometry->EnableDirectionalLight(0);

	g_hBaseNetworkHandle = LoadLibrary("BaseNetwork.dll");

	DllGetClassObject_BaseNetwork	pNetFunc;
	pNetFunc = (DllGetClassObject_BaseNetwork)GetProcAddress(g_hBaseNetworkHandle,"DllGetClassObject");
	hr = pNetFunc(CLSID_SC_BASENETWORK_DLL, IID_SC_BASENETWORK_DLL, (void**)&g_pNet);
	
	if (S_OK != hr)
	{
		// MSG_ID : 445 ; Can not find CLSID_4DyuchiGXExecutive , 343 ; Error
		MessageBox(g_hMainWnd,"",g_Message[ETC_MESSAGE343].szMessage,MB_OK);	
		return FALSE;
	}
	
	// Network 초기화 
	DESC_BASENETWORK Desc;	memset(&Desc,0,sizeof(Desc));
	Desc.wSockEventWinMsgID = WM_SOCKET_EVENT;	
	Desc.OnConnect			= OnConnect;
	Desc.OnDisconnect		= OnDisConnect;
	Desc.ReceivedMsg		= ReceivedMsg;
	
	if (!g_pNet->InitNetwork( g_hMainWnd, MAX_SERVER_CONNECT, &Desc))
	{
		// MSG_ID : 447 ; Fail to init network! , 343 ; Error
		MessageBox(g_hMainWnd,g_Message[ETC_MESSAGE447].szMessage,g_Message[ETC_MESSAGE343].szMessage,MB_OK);	
		return FALSE;
	}
	
	return TRUE;
}


DWORD __stdcall DefaultErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr)
{
	char	txt[512] = {0,};
	memset(txt,0,512);
	return 0;
}


IDIFontObject* GetFont()
{
	if(GetGameStatus()==UPDATE_GAME_LOGIN)
	{
		if(g_dwFontFlag==D3DX_FONT)
			return g_pFont[FONT_D3D_LOGIN];
		else
			return g_pFont[FONT_SS3D_LOGIN];
	}
	else
	{
		if(g_dwFontFlag==D3DX_FONT)
			return g_pFont[FONT_D3D_GAME];
		else
			return g_pFont[FONT_SS3D_GAME];
	}
	return 0;
}


BOOL CreateFont()
{
	memset(g_pFont, 0, sizeof(g_pFont));
	
	LOGFONT LogFont;
	LogFont.lfHeight			= -12;
	LogFont.lfWidth				= 0;
	LogFont.lfEscapement		= 0;
	LogFont.lfOrientation		= 0;
	LogFont.lfWeight			= FW_BOLD;
	LogFont.lfItalic			= 0;
	LogFont.lfUnderline			= 0;
	LogFont.lfStrikeOut			= 0;

#if IS_JAPAN_LOCALIZING()
	LogFont.lfCharSet			= SHIFTJIS_CHARSET;
#elif IS_KOREA_LOCALIZING()
	LogFont.lfCharSet			= HANGUL_CHARSET;	
#elif IS_CHINA_LOCALIZING()
	LogFont.lfCharSet			= GB2312_CHARSET;
#elif IS_TAIWAN_LOCALIZING()
	LogFont.lfCharSet			= CHINESEBIG5_CHARSET;
#else
	LogFont.lfCharSet			= DEFAULT_CHARSET;
#endif
		
	LogFont.lfOutPrecision		= 0;
	LogFont.lfClipPrecision		= 0;
	LogFont.lfQuality			= PROOF_QUALITY;
	LogFont.lfPitchAndFamily	= 0;

#if IS_JAPAN_LOCALIZING()
	lstrcpy(LogFont.lfFaceName,"MS PGothic");
#elif IS_KOREA_LOCALIZING()
	lstrcpy(LogFont.lfFaceName,"굴림체");
#elif IS_CHINA_LOCALIZING()
	lstrcpy(LogFont.lfFaceName,"Mingliu");
#elif IS_TAIWAN_LOCALIZING()
	lstrcpy(LogFont.lfFaceName,"Mingliu");
#else
	lstrcpy(LogFont.lfFaceName,"Courier New");
#endif 

	DWORD dwFlag = D3DX_FONT;			

	g_pFont[FONT_D3D_LOGIN] = g_pRenderer->CreateFontObject(&LogFont, dwFlag);	
	
	if(!g_pFont[FONT_D3D_LOGIN])
		return FALSE;																

	dwFlag = SS3D_FONT;

	g_pFont[FONT_SS3D_LOGIN] = g_pRenderer->CreateFontObject(&LogFont, dwFlag);
	
	if(!g_pFont[FONT_SS3D_LOGIN])
		return FALSE;

	LogFont.lfHeight			= -12;
	LogFont.lfWidth				= 0;
	LogFont.lfEscapement		= 0;
	LogFont.lfOrientation		= 0;
	LogFont.lfWeight			= FW_NORMAL;
	LogFont.lfItalic			= 0;
	LogFont.lfUnderline			= 0;
	LogFont.lfStrikeOut			= 0;

#if IS_JAPAN_LOCALIZING()
	LogFont.lfCharSet			= SHIFTJIS_CHARSET;
#elif IS_KOREA_LOCALIZING()
	LogFont.lfCharSet			= HANGUL_CHARSET;	
#elif IS_CHINA_LOCALIZING()
	LogFont.lfCharSet			= GB2312_CHARSET;
#elif IS_TAIWAN_LOCALIZING()
	LogFont.lfCharSet			= CHINESEBIG5_CHARSET;
#else
	LogFont.lfCharSet			= DEFAULT_CHARSET;
#endif

	LogFont.lfOutPrecision		= 0;
	LogFont.lfClipPrecision		= 0;
	LogFont.lfQuality			=  PROOF_QUALITY;
	LogFont.lfPitchAndFamily	= 0;

// __deepdark	
#if IS_JAPAN_LOCALIZING()
	lstrcpy(LogFont.lfFaceName,"MS PGothic");
#elif IS_KOREA_LOCALIZING()
	lstrcpy(LogFont.lfFaceName,"굴림체");
#elif IS_CHINA_LOCALIZING()
	lstrcpy(LogFont.lfFaceName,"Mingliu");
#elif IS_TAIWAN_LOCALIZING()
	lstrcpy(LogFont.lfFaceName,"Mingliu");
#else
	lstrcpy(LogFont.lfFaceName,"Courier New");
#endif 
// __deepdark

	dwFlag = D3DX_FONT;
	
	g_pFont[FONT_D3D_GAME] = g_pRenderer->CreateFontObject(&LogFont, dwFlag);
	
	if(!g_pFont[FONT_D3D_GAME])
		return FALSE;	

	dwFlag = SS3D_FONT;

	g_pFont[FONT_SS3D_GAME] = g_pRenderer->CreateFontObject(&LogFont, dwFlag);
	
	if(!g_pFont[FONT_SS3D_GAME])
		return FALSE;		

	g_hFont = CreateFontIndirect(&LogFont);
	return TRUE;
}

HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	g_hInstance = hInstance; // Store instance handle in our global variable

	RECT rc;
	rc.left		= 0;
	rc.top		= 0;
	rc.right	= SCREEN_WIDTH;
	rc.bottom	= SCREEN_HEIGHT;
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

#ifndef DEVELOP_MODE

	g_hMainWnd = CreateWindow(WINDOW_CLASS_NAME, WINDOW_TITLE_NAME,	NULL, /*WS_OVERLAPPEDWINDOW*/
	  CW_USEDEFAULT, CW_USEDEFAULT,	rc.right - rc.left, rc.bottom - rc.top, 0L, NULL, hInstance, NULL);
#else

	g_hMainWnd = CreateWindow(WINDOW_CLASS_NAME, WINDOW_TITLE_NAME,	WS_OVERLAPPEDWINDOW, 
	  CW_USEDEFAULT, CW_USEDEFAULT,	rc.right - rc.left, rc.bottom - rc.top, 0L, NULL, hInstance, NULL);

#endif

	if (!g_hMainWnd)
	{
	  return FALSE;
	}

	ShowWindow(g_hMainWnd, nCmdShow);
	UpdateWindow(g_hMainWnd);

	return g_hMainWnd;
}

ATOM RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_ICON);
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_ICON);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= WINDOW_CLASS_NAME;

	return RegisterClassEx(&wcex);
}


void CentreWindow(HWND hwnd)
{
    RECT winrect, workrect;
    
    SystemParametersInfo(SPI_GETWORKAREA, 0, &workrect, 0);
    int workwidth = workrect.right -  workrect.left;
    int workheight = workrect.bottom - workrect.top;
    
    GetWindowRect(hwnd, &winrect);
    int winwidth = winrect.right - winrect.left;
    int winheight = winrect.bottom - winrect.top;
	
    winwidth = min(winwidth, workwidth);
    winheight = min(winheight, workheight);
	
    SetWindowPos(hwnd,HWND_TOP
		, workrect.left + (workwidth-winwidth) / 2,workrect.top + (workheight-winheight) / 2,winwidth
		, winheight,SWP_SHOWWINDOW);
    SetForegroundWindow(hwnd);
}

char* ReturnKey(int nKey)
{
	for(int i = 0; i < 58; i++)
	{
		if(g_nKey[i]==nKey)
			return g_pszKeyArray[i];		
	}
	return NULL;
}

int KeySearch(char* pszKey)
{
	char* szTemp;

	szTemp = _strupr(pszKey);

	for(int i = 0; i < 58; i++)
	{
		if(__strcmp(g_pszKeyArray[i], szTemp)==0)
			return g_nKey[i];		
	}			
	return -1;
}

char* KeySearch(int nKey)
{
	for(int i = 0; i < 58; i++)
	{
		if(g_nKey[i]==nKey)
			return g_pszKeyArray[i];			
	}
	return NULL;
}

int KeyCheck(int nKey)
{
	for(int i = 0; i < MAX_KEY; i++)
	{
		if(g_sKeyConfig.snKey[i]==nKey)
			return i;
	}
	return -1;
}

void LoadSkillresourceTable()
{
	for(int i = 0; i < 5; i++)
	{
		g_sSkillListManager.pSkillList[i].pActiveList		= new COnlyList(MAX_SKILL_NODE);
		g_sSkillListManager.pSkillList[i].pPassiveList		= new COnlyList(MAX_SKILL_NODE);
		g_sSkillListManager.pSkillList[i].pOverDriveList	= new COnlyList(MAX_SKILL_NODE);
		g_sSkillListManager.pSkillList[i].pMasteryList		= new COnlyList(MAX_SKILL_NODE);
	}
	
	SSKILL_RESOURCE			sSkillResource[1000];
	LP_SKILL_RESOURCE_EX	lpSkillResourceEx;

	int	nLeftIndex	= 1;
	int	nRightIndex	= 1;

	_CHECK_MEMORY();
	
	// Item Resource 
	int nTotalSize	= DecodeCDBData(GetFile("SkillResource.cdb", DATA_TYPE_MANAGER), sSkillResource);
	int nMaxNode	= nTotalSize / sizeof(SSKILL_RESOURCE);		

	_CHECK_MEMORY();
	g_sSkillListManager.byLeftSkill[0] = 0;
	g_sSkillListManager.byRightSkill[0] = 0;

	for(i = 0; i < nMaxNode; i++)
	{		
		lpSkillResourceEx = (LP_SKILL_RESOURCE_EX)LALAlloc(g_pSkillReosurcePool); 

		memset(lpSkillResourceEx->szFileName, 0, sizeof(lpSkillResourceEx->szFileName));
		memset(lpSkillResourceEx->szFileNameAct, 0, sizeof(lpSkillResourceEx->szFileNameAct));

		__lstrcpyn(lpSkillResourceEx->szFileName, sSkillResource[i].szFileName, 20);
		__lstrcpyn(lpSkillResourceEx->szFileNameAct, sSkillResource[i].szFileNameAct, 20);
		lpSkillResourceEx->bySkillType	= sSkillResource[i].bySkillType;
		lpSkillResourceEx->byType		= sSkillResource[i].byType;
		lpSkillResourceEx->byTypeIndex	= sSkillResource[i].byTypeIndex;
		lpSkillResourceEx->byTypePos	= sSkillResource[i].byTypePos;
		lpSkillResourceEx->wId			= sSkillResource[i].wId;
		lpSkillResourceEx->wIndex		= sSkillResource[i].wIndex;
		lpSkillResourceEx->wIndexAct	= sSkillResource[i].wIndexAct;		

		if(lpSkillResourceEx->byTypePos==2)
		{
			g_sSkillListManager.byLeftSkill[nLeftIndex] = BYTE(lpSkillResourceEx->wId);
			g_sSkillListManager.byRightSkill[nRightIndex] = BYTE(lpSkillResourceEx->wId);
			nLeftIndex++;
			nRightIndex++;			
		}
		else if(lpSkillResourceEx->byTypePos==1)
		{
			g_sSkillListManager.byRightSkill[nRightIndex] = BYTE(lpSkillResourceEx->wId);
			nRightIndex++;
		}

		switch(lpSkillResourceEx->byType)
		{
		case SKILL_TYPE_MASTERY:	// 마스터리.
			g_sSkillListManager.pSkillList[lpSkillResourceEx->bySkillType].pMasteryList->AddTail(lpSkillResourceEx);
			break;
		case SKILL_TYPE_ACTIVE:		// 액티브.
			g_sSkillListManager.pSkillList[lpSkillResourceEx->bySkillType].pActiveList->AddTail(lpSkillResourceEx);
			break;
		case SKILL_TYPE_OVERDIRVE:	// 오버드라이브.
			g_sSkillListManager.pSkillList[lpSkillResourceEx->bySkillType].pOverDriveList->AddTail(lpSkillResourceEx);
			break;
		case SKILL_TYPE_PASSIVE:	// 패시브.
			g_sSkillListManager.pSkillList[lpSkillResourceEx->bySkillType].pPassiveList->AddTail(lpSkillResourceEx);
			break;
		}
	}
	g_sSkillListManager.byLeftSkillCnt	= (BYTE)nLeftIndex;
	g_sSkillListManager.byRightSkillCnt	= (BYTE)nRightIndex;
}

void LoadItemResourceTable()
{
	SITEM_RESOURCE		sItemResource[2000];
	ITEM_STORE			sItemStore[2000];
	ITEM_OPTION			sItemOption[2000];	
	LP_ITEM_RESOURCE_EX	lpItemResource=0;	
	LP_ITEM_STORE		lpItemStore=0;
	LP_ITEM_OPTION		lpItemOption=0;
	
	_CHECK_MEMORY();

	// Item Resource //
	int nTotalSize = DecodeCDBData(GetFile("ItemResource.cdb", DATA_TYPE_MANAGER), sItemResource);
	int nMaxNode = nTotalSize / sizeof(SITEM_RESOURCE);	
	_CHECK_MEMORY();

	for(int i = 0; i < nMaxNode; i++)
	{
		lpItemResource = (LP_ITEM_RESOURCE_EX)LALAlloc(g_pItemResourcePool);

		if(lpItemResource == NULL)
			MessageBox(g_hMainWnd, "Item Resource Pool = NULL", "CorumOnline", MB_OK);

		memset(lpItemResource, 0, sizeof(SITEM_RESOURCE_EX));
		
		lpItemResource->byIconCount			= sItemResource[i].byIconCount;
		lpItemResource->byIconStartIndex	= sItemResource[i].byIconStartIndex;
		lpItemResource->byPreLoad			= sItemResource[i].byPreLoad;
		lpItemResource->byResourceType		= sItemResource[i].byResourceType;
		lpItemResource->wID					= sItemResource[i].wID;
		lpItemResource->wModCount			= sItemResource[i].wModCount;
		lpItemResource->byAnimation			= sItemResource[i].byAnimation;
		__lstrcpyn(lpItemResource->szIconFileName, sItemResource[i].szIconFileName, sizeof(sItemResource[i].szIconFileName));
		__lstrcpyn(lpItemResource->szModFileName, sItemResource[i].szModFileName, sizeof(sItemResource[i].szModFileName));						
		
		int	nWidth	= 32;
		int	nHeight	= 64;
		int	nPosX	= 0;
		int	nPosY	= 0;
		int nValue	= lpItemResource->wID/ITEM_DISTRIBUTE;

		if(nValue<=20)
		{
			nHeight	= 64;
			nPosX	= (lpItemResource->byIconStartIndex % 8) * 32;
			nPosY	= (lpItemResource->byIconStartIndex / 8) * 64;		
		}
		else
		{
			nHeight	= 32;
			nPosX	= (lpItemResource->byIconStartIndex % 8) * 32;
			nPosY	= (lpItemResource->byIconStartIndex / 8) * 32;
		}
				
		lpItemResource->pSpr = g_pRenderer->CreateSpriteObject(GetFile(lpItemResource->szIconFileName, DATA_TYPE_UI), nPosX, nPosY, nWidth, nHeight, 0);

		if(!lpItemResource->pSpr)
		{
			char szTemp[0xff] = {0,};
			wsprintf(szTemp, "file not found : %s", lpItemResource->szIconFileName);
			MessageBox(g_hMainWnd, szTemp, "CorumOnline Error", MB_OK);
		}
					
		g_pItemResourceHash->Insert(lpItemResource, lpItemResource->wID);		
	}

	_CHECK_MEMORY();
	
	// Item Store //
	nTotalSize = DecodeCDBData(GetFile("ItemStore.cdb", DATA_TYPE_MANAGER), sItemStore);
	nMaxNode = nTotalSize / sizeof(ITEM_STORE);	
	_CHECK_MEMORY();

	for(i = 0; i < nMaxNode; i++)
	{		
		lpItemStore = (LP_ITEM_STORE)LALAlloc(g_pItemStorePool);

		if(lpItemStore == NULL)
			MessageBox(g_hMainWnd, "Item Store Pool = NULL", "CorumOnline", MB_OK);

		memset(lpItemStore, 0, sizeof(ITEM_STORE));
		memcpy(lpItemStore, &sItemStore[i], sizeof(ITEM_STORE));

		g_pItemStoreHash->Insert(lpItemStore, i);		
	}

	_CHECK_MEMORY();
	
	// Item Option //
	nTotalSize = DecodeCDBData(GetFile("ItemOption.cdb", DATA_TYPE_MANAGER), sItemOption);
	nMaxNode = nTotalSize / sizeof(ITEM_OPTION);	
	_CHECK_MEMORY();

	for(i = 0; i < nMaxNode; i++)
	{		
		lpItemOption = (LP_ITEM_OPTION)LALAlloc(g_pItemOptionPool);

		if(lpItemOption==NULL)
			MessageBox(g_hMainWnd, "Item Option Pool = NULL", "CorumOnline", MB_OK);
		
		memset(lpItemOption, 0, sizeof(ITEM_OPTION));
		memcpy(lpItemOption, &sItemOption[i], sizeof(ITEM_OPTION));
		
		g_pItemOptionHash->Insert(lpItemOption, sItemOption[i].wId);		
	}
}

void LoadHelpInfo()
{
	SHELP_INFO		sInfo[__MAX_INTERFACE_MESSAGE];
	LP_HELP_INFO	lpHelpInfo=0;

	int nTotalSize = DecodeCDBData(GetFile("HelpInfo.cdb", DATA_TYPE_MANAGER), sInfo);
	int nMaxNode = nTotalSize / sizeof(SHELP_INFO);	
	
	// 키 설정 데이터 //
	for(int i = 0; i < nMaxNode; i++)
	{	
		lpHelpInfo = (LP_HELP_INFO)LALAlloc(g_pHelpInfoPool);
		memset(lpHelpInfo->szText, 0, sizeof(lpHelpInfo->szText));
		__lstrcpyn(lpHelpInfo->szText, sInfo[i].szText, lstrlen(sInfo[i].szText));
		lpHelpInfo->wId		= sInfo[i].wId;
		lpHelpInfo->wLeft	= sInfo[i].wLeft;
		lpHelpInfo->wTop	= sInfo[i].wTop;
		lpHelpInfo->wLinkTextID = sInfo[i].wLinkTextID;
		lpHelpInfo->byType = sInfo[i].byType;
		g_pHelpInfoHash->Insert(lpHelpInfo, lpHelpInfo->wId);	
	}
}

void LoadGroupInfo()
{
	SGROUP_INFO			sInfo[0xff];
	LP_GROUPINFO_TABLE	lpGroupInfo=0;
	LP_GROUPINFO_TABLE	lpGroupInfoTable=0;

	_CHECK_MEMORY();
	int nTotalSize = DecodeCDBData(GetFile("GroupInfo.cdb", DATA_TYPE_MANAGER), sInfo);
	int nMaxNode = nTotalSize / sizeof(SGROUP_INFO);	
	_CHECK_MEMORY();
	
	// 키 설정 데이터 //
	for(int i = 0; i < nMaxNode; i++)
	{	
		lpGroupInfo = g_pGroupInfoHash->GetData(sInfo[i].byLevel);

		if(lpGroupInfo)
		{
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].byAura		= sInfo[i].byAura;
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].byLevel		= sInfo[i].byLevel;
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].byMake_Size	= sInfo[i].byMake_Size;
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].byType		= sInfo[i].byType;
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].dwExp		= sInfo[i].dwExp;
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].wMemeber[0]	= sInfo[i].wMemeber[0];
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].wMemeber[1]	= sInfo[i].wMemeber[1];
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].wMemeber[2]	= sInfo[i].wMemeber[2];
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].wMemeber[3]	= sInfo[i].wMemeber[3];
			lpGroupInfo->sGroupInfo[sInfo[i].byType-1].wMemeber[4]	= sInfo[i].wMemeber[4];
		}
		else
		{
			lpGroupInfoTable = (LP_GROUPINFO_TABLE)LALAlloc(g_pGroupInfoPool);
			memset(lpGroupInfoTable, 0, sizeof(lpGroupInfoTable));						
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].byAura		= sInfo[i].byAura;
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].byLevel		= sInfo[i].byLevel;
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].byMake_Size	= sInfo[i].byMake_Size;
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].byType		= sInfo[i].byType;
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].dwExp		= sInfo[i].dwExp;
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].wMemeber[0]	= sInfo[i].wMemeber[0];
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].wMemeber[1]	= sInfo[i].wMemeber[1];
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].wMemeber[2]	= sInfo[i].wMemeber[2];
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].wMemeber[3]	= sInfo[i].wMemeber[3];
			lpGroupInfoTable->sGroupInfo[sInfo[i].byType-1].wMemeber[4]	= sInfo[i].wMemeber[4];
			g_pGroupInfoHash->Insert(lpGroupInfoTable, sInfo[i].byLevel);
		}
	}
}

void LoadKeyInfo()
{
	// Key Info //
	SKEYINFO	szInfo[100];
	char		szKeyOtherInfo[10][128] =
	{
		"", "", "Insert", "Page Up", "Home, Mouse Wheel up",	// MSG_ID : 448 ; 마우스 왼쪽 버튼, 449 마우스 오른쪽 버튼
		"End, Mouse Wheel Down", "Delete, SHIFT + Mouse Wheel up", "Page Down, SHIFT + Mouse Wheel Down"
	};

	__lstrcpyn(szKeyOtherInfo[0], g_Message[ETC_MESSAGE448].szMessage, sizeof(g_Message[ETC_MESSAGE448].szMessage));
	__lstrcpyn(szKeyOtherInfo[0], g_Message[ETC_MESSAGE449].szMessage, sizeof(g_Message[ETC_MESSAGE449].szMessage));
		
	int nTotalSize = DecodeCDBData(GetFile("KeyInfo.cdb", DATA_TYPE_MANAGER), szInfo);
	int nMaxNode = nTotalSize / sizeof(SKEYINFO);	
	
	// 키 설정 데이터 //
	for(int i = 0; i < nMaxNode; i++)
	{
		if(i<MAX_KEY-1)
			wsprintf(g_sKeyConfig.szKeyInfo[i], "%s [%s]", szInfo[i].szInfo, ReturnKey(g_sKeyConfig.snKey[i]));
		else
			wsprintf(g_szKeyOtherInfo[i-(MAX_KEY-1)], "%s [%s]", szInfo[i].szInfo, szKeyOtherInfo[i-(MAX_KEY-1)]);
	}
}

void LoadCPTable()
{
	DecodeCDBData(GetFile("cptable.cdb", DATA_TYPE_MANAGER), g_pCPTable);
}

void LoadPreLoadInfo()
{
	char szInfo[0xff] = {0, };
	
	PRE_LOAD sPreLoadInfo[500];

	int nTotalSize = DecodeCDBData(GetFile("PreLoad.cdb", DATA_TYPE_MANAGER), sPreLoadInfo);	
	int nMaxNode = nTotalSize / sizeof(PRE_LOAD);

	for(int i = 0; i < nMaxNode; i++)
	{
		wsprintf(szInfo, "%s.mod", sPreLoadInfo[i].szFileName);

		// 0번 Item, 1번 Effect, 2번 DamageNumber, 3번 Character, 4번 UI, 5번 NPC, 6번 Monster.
		switch(sPreLoadInfo[i].byType)
		{
		case 0:
			g_pGeometry->DisableUnloadPreLoadedResource(GetFile(szInfo, DATA_TYPE_ITEM));
			break;
		case 1:
			g_pGeometry->DisableUnloadPreLoadedResource(GetFile(szInfo, DATA_TYPE_EFFECT));
			break;
		case 2:
			g_pGeometry->DisableUnloadPreLoadedResource(GetFile(szInfo, DATA_TYPE_DAMAGENUMBER));
			break;
		case 3:
			g_pGeometry->DisableUnloadPreLoadedResource(GetFile(szInfo, DATA_TYPE_CHARACTER));
			break;
		case 4:
			g_pGeometry->DisableUnloadPreLoadedResource(GetFile(szInfo, DATA_TYPE_UI));
			break;
		case 5:
			g_pGeometry->DisableUnloadPreLoadedResource(GetFile(szInfo, DATA_TYPE_NPC));
			break;
		case 6:
			g_pGeometry->DisableUnloadPreLoadedResource(GetFile(szInfo, DATA_TYPE_MONSTER));
			break;
		}
	}
}


void LoadBaseItemTable()
{
	CBaseItem*					pBaseItem = NULL;
	BASEITEM_WEAPONEX			sWeaponItem[700];
	BASEITEM_ARMOREX			sArmorItem[900];	
	BASEITEM_SUPPLIESEX			sSuppliesItem[500];
	BASEITEM_ZODIACEX			sZodiacItem[500];
	BASEITEM_RIDEEX				sRideItem[10];
	BASEITEM_SPECIALEX			sSpecialItem[500];
	BASEITEM_GUARDIANEX			sGuardianItem[500];
	BASEITEM_MAGICARRAYEX		sMagicArrayItem[500];
	BASEITEM_MATERIALSEX		sMaterials[500];
	BASEITEM_MIX_UPGRADEEX		sMixUpgrade[500];
	BASEITEM_MAGICFIELD_ARRAYEX sMagicFieldArray[500];
	BASEITEM_CONSUMABLEEX		sCunsumableItem[1000];	
	BASEITEM_UPGRADEEX			sUpGradeItem[50];
	BASEITEM_LIQUIDEX			sLiQuidItem[50];
	BASEITEM_EDITIONEX			sEditionItem[50];
	BASEITEM_BAGEX				sBagItem[50];

	memset(sWeaponItem,			0, sizeof(sWeaponItem));
	memset(sArmorItem,			0, sizeof(sArmorItem));
	memset(sSuppliesItem,		0, sizeof(sSuppliesItem));
	memset(sRideItem,			0, sizeof(sRideItem));
	memset(sSpecialItem,		0, sizeof(sSpecialItem));
	memset(sZodiacItem,			0, sizeof(sZodiacItem));
	memset(sGuardianItem,		0, sizeof(sGuardianItem));	
	memset(sMagicArrayItem,		0, sizeof(sMagicArrayItem));				
	memset(sMaterials,			0, sizeof(sMaterials));				
	memset(sMixUpgrade,			0, sizeof(sMixUpgrade));
	memset(sMagicFieldArray,	0, sizeof(sMagicFieldArray));
	memset(sCunsumableItem,		0, sizeof(sCunsumableItem));
	memset(sUpGradeItem,		0, sizeof(sUpGradeItem));
	memset(sLiQuidItem,			0, sizeof(sLiQuidItem));
	memset(sEditionItem,		0, sizeof(sEditionItem));	
	memset(sBagItem,			0, sizeof(sBagItem));	

	DWORD dwDefaultSize = sizeof(COMMONBASEITEM);
	
	// Weapon 
	int nTotalSize	= DecodeCDBData(GetFile("ItemWeapon.cdb", DATA_TYPE_MANAGER), sWeaponItem);
	int nSize		= sizeof(BASEITEM_WEAPON) + dwDefaultSize;
	int nMaxNode = nTotalSize / nSize;
	for(int i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sWeaponItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);			
	}
	
	// Armor 
	nTotalSize	= DecodeCDBData(GetFile("ItemArmor.cdb", DATA_TYPE_MANAGER), sArmorItem);
	nSize		= sizeof(BASEITEM_ARMOR) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sArmorItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}

	// Supplies
	nTotalSize	= DecodeCDBData(GetFile("ItemSupplies.cdb", DATA_TYPE_MANAGER), sSuppliesItem);
	nSize		= sizeof(BASEITEM_SUPPLIES) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sSuppliesItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// Ride
	nTotalSize	= DecodeCDBData(GetFile("ItemRide.cdb", DATA_TYPE_MANAGER), sRideItem);
	nSize		= sizeof(BASEITEM_RIDE) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sRideItem[i], sizeof(CBaseItem));
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// Special	
	nTotalSize	= DecodeCDBData(GetFile("ItemSpecial.cdb", DATA_TYPE_MANAGER), sSpecialItem);
	nSize		= sizeof(BASEITEM_SPECIAL) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sSpecialItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// Zodiac 
	nTotalSize	= DecodeCDBData(GetFile("ItemZodiac.cdb", DATA_TYPE_MANAGER), sZodiacItem);
	nSize		= sizeof(BASEITEM_ZODIAC) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sZodiacItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// Guardian	
	nTotalSize	= DecodeCDBData(GetFile("ItemGuardian.cdb", DATA_TYPE_MANAGER), sGuardianItem);
	nSize		= sizeof(BASEITEM_GUARDIAN) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sGuardianItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// MagicArray
	nTotalSize	= DecodeCDBData(GetFile("ItemMagicArray.cdb", DATA_TYPE_MANAGER), sMagicArrayItem);
	nSize		= sizeof(BASEITEM_MAGICARRAY) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sMagicArrayItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// Material
	nTotalSize	= DecodeCDBData(GetFile("ItemMaterials.cdb", DATA_TYPE_MANAGER), sMaterials);
	nSize		= sizeof(BASEITEM_MATERIALS) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sMaterials[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// MixUpgrade 
	nTotalSize	= DecodeCDBData(GetFile("ItemMixUpgrade.cdb", DATA_TYPE_MANAGER), sMixUpgrade);
	nSize		= sizeof(BASEITEM_MIX_UPGRADE) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sMixUpgrade[i], sizeof(CBaseItem));
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// MagicFieldArray
	nTotalSize	= DecodeCDBData(GetFile("ItemMagicFieldArray.cdb", DATA_TYPE_MANAGER), sMagicFieldArray);
	nSize		= sizeof(BASEITEM_MAGICFIELD_ARRAY) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sMagicFieldArray[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// Consumable
	nTotalSize	= DecodeCDBData(GetFile("ItemConsumable.cdb", DATA_TYPE_MANAGER), sCunsumableItem);
	nSize		= sizeof(BASEITEM_CONSUMABLE) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sCunsumableItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// Upgrade 
	nTotalSize	= DecodeCDBData(GetFile("ItemUpgrade.cdb",DATA_TYPE_MANAGER), sUpGradeItem);
	nSize		= sizeof(BASEITEM_UPGRADE) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sUpGradeItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// liquid 
	nTotalSize	= DecodeCDBData(GetFile("Itemliquid.cdb",DATA_TYPE_MANAGER), sLiQuidItem);
	nSize		= sizeof(BASEITEM_LIQUID) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sLiQuidItem[i], sizeof(CBaseItem));
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	// Edition //
	nTotalSize	= DecodeCDBData(GetFile("ItemEdition.cdb",DATA_TYPE_MANAGER), sEditionItem);
	nSize		= sizeof(BASEITEM_EDITION) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sEditionItem[i], nSize);
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}

	// Bag
	nTotalSize	= DecodeCDBData(GetFile("ItemBag.cdb", DATA_TYPE_MANAGER), sBagItem);
	nSize		= sizeof(BASEITEM_BAG) + dwDefaultSize;
	nMaxNode	= nTotalSize / nSize;
	for(i = 0; i < nMaxNode; i++)
	{
		pBaseItem = new CBaseItem;
		memset(pBaseItem, 0, sizeof(CBaseItem));		
		memcpy(pBaseItem, &sBagItem[i], nSize);		
		g_pItemTableHash->Insert(pBaseItem, pBaseItem->wID);				
	}	

	SetCommonServerBaseItemHash(g_pItemTableHash);	
}

void LoadItemTable()
{
	SET_ITEM_INFO		sSetItemInfo[200];
	ITEM_MAKING_INFO	sItemMakingInfo[200];	
	LPSET_ITEM_INFO		pSetItemInfo	= NULL;
	LPITEM_MAKING_INFO	pItemMakingInfo = NULL;

	memset(sSetItemInfo, 0, sizeof(sSetItemInfo));
	memset(sItemMakingInfo, 0, sizeof(sItemMakingInfo));

	g_pExecutive->PreLoadGXObject(GetFile("GuildFlag.chr", DATA_TYPE_ITEM));
	// sung-han 2005-02-14 길드전 혜택중 길드 깃발
	/*
	g_pExecutive->PreLoadGXObject(GetFile("GuildFlag2.chr", DATA_TYPE_ITEM));
	g_pExecutive->PreLoadGXObject(GetFile("GuildFlag3.chr", DATA_TYPE_ITEM));
	g_pExecutive->PreLoadGXObject(GetFile("GuildFlag4.chr", DATA_TYPE_ITEM));
	g_pExecutive->PreLoadGXObject(GetFile("GuildFlag5.chr", DATA_TYPE_ITEM));
	*/

	// SetItemInfo 
	int nTotalSize  = DecodeCDBData(GetFile("ItemSetInfo.cdb", DATA_TYPE_MANAGER), sSetItemInfo );
	int nMaxNode	= nTotalSize / sizeof( SET_ITEM_INFO );

	for( int i=0; i<nMaxNode; i++ )
	{
		pSetItemInfo = new SET_ITEM_INFO;
		memcpy( pSetItemInfo, &sSetItemInfo[i], sizeof( SET_ITEM_INFO ) );
		g_pSetItemInfoHash->Insert( pSetItemInfo, pSetItemInfo->wSetItemID );
	}

	//	ItemMakingInfo 
	nTotalSize	= DecodeCDBData(GetFile("ItemMaking.cdb", DATA_TYPE_MANAGER), sItemMakingInfo );
	nMaxNode	= nTotalSize / sizeof( ITEM_MAKING_INFO );

	for( i=0; i<nMaxNode; i++)
	{
		pItemMakingInfo = new ITEM_MAKING_INFO;
		memcpy( pItemMakingInfo, &sItemMakingInfo[i], sizeof( ITEM_MAKING_INFO ) );
		g_pItemMakingInfoHash->Insert( pItemMakingInfo,pItemMakingInfo->wID );
	}
	
	nTotalSize = DecodeCDBData(GetFile("itemAttrDefine.cdb", DATA_TYPE_MANAGER), g_pItemAttrLayer->m_ItemAttrDefine);	
	nTotalSize = DecodeCDBData(GetFile("itemAttrValueList.cdb", DATA_TYPE_MANAGER), g_pItemAttrLayer->m_ItemAttrValueList);

	PreLoadItem();
}
