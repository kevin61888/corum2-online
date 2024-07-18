#pragma once


#include "CResNameResolver.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGX_UTIL/mouselib.h"
#include "./../SS3D_0719/4dyuchigx/4DyuchiGXGFunc/global.h"
#else
#include "./../4DyuchiGX_UTIL/mouselib.h"
#include "./../4DyuchiGXGFunc/global.h"
#endif

#include "LogWnd.h"
#include "HashTable.h"
#include "MessagePool.h"
#include "Develop.h"
#include "DUNGEON_DATA_EX.h"
#include "GameControl.h"
#include "DungeonProcess_Sound.h"
#include "ItemMoveManager.h"
#include "ItemUsedManager.h"
#include "ItemAttrLayer.h"
#ifdef _USE_IME
#include "IMEEdit.h"
#endif 

#pragma pack(push,1)
struct COMMONBASEITEM
{
	WORD	wID;
	char	szItemName_Kor[50];		
	char	szItemName_Eng[35];
	DWORD	dwCode_ID;
	BYTE	bCode_Type;
	BYTE	bRandItem;
	BYTE	bMovable;// 0:제약없슴, 1:창고, 상점, 본인 인터페이스로 이동 가능, 판매가능(거래,노점,드롭 금지), 2:상점에 팔기만 가능(캐릭터 내에서만 이동)
};
struct BASEITEM_WEAPONEX : public COMMONBASEITEM
{
	BASEITEM_WEAPON	sWeaponItem;
};
struct BASEITEM_ARMOREX : public COMMONBASEITEM
{
	BASEITEM_ARMOR	sArmorItem;
};
struct BASEITEM_SUPPLIESEX : public COMMONBASEITEM
{
	BASEITEM_SUPPLIES	sSuppliesItem;
};
struct BASEITEM_ZODIACEX : public COMMONBASEITEM
{
	BASEITEM_ZODIAC	sZodiacItem;
};
struct BASEITEM_RIDEEX : public COMMONBASEITEM
{
	BASEITEM_RIDE	sRideItem;
};
struct BASEITEM_SPECIALEX : public COMMONBASEITEM
{
	BASEITEM_SPECIAL	sSpecialItem;
};
struct BASEITEM_GUARDIANEX : public COMMONBASEITEM
{
	BASEITEM_GUARDIAN	sGuardianItem;
};
struct BASEITEM_MAGICARRAYEX : public COMMONBASEITEM
{
	BASEITEM_MAGICARRAY sMagicArrayItem;
};
struct BASEITEM_MATERIALSEX : public COMMONBASEITEM
{
	BASEITEM_MATERIALS sMaterials;
};
struct BASEITEM_MIX_UPGRADEEX : public COMMONBASEITEM
{
	BASEITEM_MIX_UPGRADE sMixUpgrade;
};
struct BASEITEM_MAGICFIELD_ARRAYEX : public COMMONBASEITEM
{
	BASEITEM_MAGICFIELD_ARRAY sMagicFieldArray;
};
struct BASEITEM_CONSUMABLEEX : public COMMONBASEITEM
{
	BASEITEM_CONSUMABLE sConsumable;
};
struct BASEITEM_UPGRADEEX : public COMMONBASEITEM
{
	BASEITEM_UPGRADE sUpGrade;
};
struct BASEITEM_LIQUIDEX : public COMMONBASEITEM
{
	BASEITEM_LIQUID sLiQuid;
};
struct BASEITEM_EDITIONEX : public COMMONBASEITEM
{
	BASEITEM_EDITION sEdition;
};
struct BASEITEM_BAGEX : public COMMONBASEITEM
{
	BASEITEM_BAG eBag;
};
#pragma pack(pop)


struct	ISoundEffect;
class	CObjectManager;
class	CSpriteManager;
class	CDungeonTable;
class	CItemManager;
class	CMonsterManager;
class	CUserHash; 
class	CMonsterHash;
class	CEffectHash;
class	CItemTableHash;
class	COnlyList;
class	CItemResourceHash;
class	CItemOptionHash;
class	CItemStoreHash;
class	CMessageHash;
class	CHelpInfoHash;
class	CInputManager;
class	CDebugLog;
class	CNetworkClient;
struct	ChrInfoEx;
struct	EffectLayer;
struct	A_STAR;
struct	INPUT_BUFFER;
struct	ChrInfoLayer;


#define	MAX_SNOW_EFFECT				100
#define WINDOW_CLASS_NAME			"Corum Online Project"
#define WINDOW_TITLE_NAME			"Corum Online Project"
#define SCREEN_WIDTH				1024
#define SCREEN_HEIGHT				768
#define MAX_AUDIO_OBJECT_NUM		128
#define MAX_OBJECT_NUM				1000	
#define MAX_FONT_NUM				4
#define MAX_DATA_FOR_USER_POOL		200
#define MAX_KIND_OF_DEBUG_TILE		3
#define MAX_DEBUG_TILE_NUM			300
#define UM_GAME_RESTART				WM_USER + 30
#define UM_PORTAL_CHANGE_LAYER		WM_USER + 31

extern BYTE							g_byChatLineCnt;
extern BYTE							g_byCharMoveCnt;
extern BYTE							g_byCharMoveSuccess;
extern BYTE							g_byCharMoveFail;
extern BYTE							g_bVilType;
extern BYTE							g_byChkMessenger;
extern BYTE							g_byStatusMessenger;
extern BYTE							g_bGM;
extern BYTE							g_byCharacterAllCount;
extern BYTE							g_bExecuteType;
extern char*						g_pszKeyArray[0xff];
extern char							g_szKeyOtherInfo[10][128];

extern int							g_nKey[0xff];
extern int							g_nHanKey[0xff];
extern DWORD						g_dwVersion;
extern DWORD						g_dwStatusTime;
extern DWORD						g_dwEventMapID;	
extern DWORD						g_dwFontFlag;
extern BOOL							g_bCharMoveChk;
extern BOOL							g_bSmallBankChk;
extern BOOL							g_bLoadingChk;
extern float						g_fDist;
extern float						g_fBGMVolume;
extern float						g_fEffectVolume;
extern HINSTANCE					g_hInst;
extern HINSTANCE					g_hInstance;
extern HWND							g_hMainWnd;
extern HIMC							g_hImc;
extern HWND							g_hMainWnd;
extern VECTOR3						g_v3Temp;
extern WORD							g_wEffectLevel;		// 이펙트 단계 : 최덕석 2005.04.01

extern GXOBJECT_HANDLE				g_hHandle;
extern GXOBJECT_HANDLE				g_hEffectHandle;

extern GUILD_INFO					g_pGuildInfo;
extern DUNGEONPRODUCTIONITEMMINMAX	g_DungeonProductionItemMinMax[DUNGEON_PROPERTY_MAX];
extern BASE_CLASS_INFO				g_sBaseClassInfo[5];
extern SSKILL_LIST_MANAGER			g_sSkillListManager;
extern SSKILL_DPINFO				g_sSkillInfoDP[MAX_SKILL];
extern A_STAR						g_PathFinder; 
extern SKEY_CONFIG					g_sKeyConfig;
extern HCURSOR						g_hCursor[ __MAX_MOUSE_POINTER__ ];	
extern DEVELOP_VARIABLE				g_Dev;

extern STMPOOL_HANDLE				g_pSkillReosurcePool;
extern STMPOOL_HANDLE				g_pObjDescPool;
extern STMPOOL_HANDLE				g_pUserPool;
extern STMPOOL_HANDLE				g_pMonsterPool;
extern STMPOOL_HANDLE				g_pEffectPool;
extern STMPOOL_HANDLE				g_pItemPool;
extern STMPOOL_HANDLE				g_pItemResourcePool;
extern STMPOOL_HANDLE				g_pGuildPool;
extern STMPOOL_HANDLE				g_pItemStorePool;
extern STMPOOL_HANDLE				g_pItemOptionPool;
extern STMPOOL_HANDLE				g_pHelpInfoPool;
extern STMPOOL_HANDLE				g_pGroupInfoPool;
extern STMPOOL_HANDLE				g_pGuildOfflinePool;
extern STMPOOL_HANDLE				g_pGuildDataPool;
extern STMPOOL_HANDLE				g_pGuildOfflinePool;
extern STMPOOL_HANDLE				g_pMessengerUserPool;
extern STMPOOL_HANDLE				g_pGuildWarPool;
extern STMPOOL_HANDLE				g_pPartyBoardPool;
extern STMPOOL_HANDLE				g_pChatPool;
extern STMPOOL_HANDLE				g_pPartyPool;

extern CGuildUserHash*				g_pGuildUserHash;
extern CHelpInfoHash*				g_pHelpInfoHash;
extern CItemOptionHash*				g_pItemOptionHash;
extern CItemStoreHash*				g_pItemStoreHash;
extern CUserHash*					g_pUserHash;
extern CMonsterHash*				g_pMonsterHash;
extern CEffectHash*					g_pEffectHash;
extern LPBASEITEM_HASH				g_pItemTableHash;
extern CItemResourceHash*			g_pItemResourceHash;
extern CSetItemInfoHash*			g_pSetItemInfoHash;
extern CItemMakingInfoHash*			g_pItemMakingInfoHash;
extern CPartyUserHash*				g_pPartyUserHash;
extern CGroupInfoHash*				g_pGroupInfoHash;
extern CGuildOffLineHash*			g_pGuildOfflineHash;
extern CGuildInfoHash*				g_pGuildInfoHash;
extern CMessengerUserHash*			g_pMessengerOnLineHash;
extern CMessengerUserHash*			g_pMessengerOffLineHash;
extern CGuildListHash*				g_pGuildWarHash;
extern CPartyBoardHash*				g_pPartyBoardHash;
extern ITEM_HASH*					g_pItemHash;

extern CMessagePool					g_Message;
extern CMessagePool					g_CmdMessage;
extern CMessagePool					g_ConvMessage;
extern CMessagePool					g_NotConvMessage;
extern CMessagePool					g_Emoticon;		
	
extern I4DyuchiGXExecutive*			g_pExecutive;
extern I4DyuchiGXGeometry*			g_pGeometry;
extern I4DyuchiGXRenderer*			g_pRenderer;

extern CNetworkClient*				g_pNet;
extern CResNameResolver				g_Res;
extern CResNameResolver				g_ResDefined;		
extern CObjectManager*				g_pObjManager;		
extern CSpriteManager*				g_pSprManager;
extern CDungeonTable*				g_pDungeonTable;
extern EffectLayer*					g_pEffectLayer;
extern IDIFontObject*				g_pFont[ MAX_FONT_NUM ];
extern CItemManager*				g_pItemManager;			
extern CMonsterManager*				g_pMonsterManager;		
extern CInputManager*				g_pInputManager;
extern CItemAttrLayer*				g_pItemAttrLayer;

extern COnlyList*					g_pEffectList;
extern COnlyList*					g_pAllChatList;
extern COnlyList*					g_pPartyChatList;
extern COnlyList*					g_pGuildChatList;
extern COnlyList*					g_pFriendChatList;
extern COnlyList*					g_pWhisperChatList;

extern ChrInfoLayer*				g_pChrInfoUser;
extern ChrInfoLayer*				g_pChrInfoMonster;
extern ChrInfoLayer*				g_pChrInfoEffect;

extern CItemMoveManager				g_ItemMoveManager;
extern CItemUsedManager				g_ItemUsedManager;

extern void	(*OnKeyDown[ MAX_UPDATE_GAME ])		(WPARAM wParam, LPARAM lParam);
extern void	(*OnKeyUp[ MAX_UPDATE_GAME ])		(WPARAM wParam, LPARAM lParam);
extern void	(*OnLButtonDown[ MAX_UPDATE_GAME ])	(WPARAM wParam, LPARAM lParam);
extern void	(*OnLButtonUp[ MAX_UPDATE_GAME ])	(WPARAM wParam, LPARAM lParam);
extern void	(*OnRButtonDown[ MAX_UPDATE_GAME ])	(WPARAM wParam, LPARAM lParam);
extern void	(*OnRButtonUp[ MAX_UPDATE_GAME ])	(WPARAM wParam, LPARAM lParam);
extern void	(*OnMouseMove[ MAX_UPDATE_GAME ])	(WPARAM wParam, LPARAM lParam);
extern void	(*OnMouseWheel[ MAX_UPDATE_GAME ])	(WPARAM wParam, LPARAM lParam);
extern void	(*OnTimerEvent[ MAX_UPDATE_GAME ])	(DWORD dwTimerIndex);
extern void	(*OnChar[ MAX_UPDATE_GAME ])		(BYTE bCh);


int					KeyCheck(int nKey);
int					KeySearch(char* pszKey);
char*				KeySearch(int nKey);
void				LoadBaseItemTable();
void				LoadItemTable();
void				LoadPreLoadInfo();
void				LoadItemResourceTable();
void				LoadSkillresourceTable();
void				LoadGroupInfo();
void				InitializePool();
void				InitializeHash();
void				LoadCursorDoIt();
BOOL				InitGame();
BOOL				InitCOMObject();
BOOL				CreateFont();
BOOL				ChangeCursor(int nCursorIndex);
void				InitFunctionPointer();
void				ReleaseGame();
void				LoadKeyInfo();
void				LoadLevelExp();
void				LoadHelpInfo();
char*				ReturnKey(int nKey);
IDIFontObject*		GetFont();
DWORD __stdcall		DefaultErrorHandleProc(ERROR_TYPE type,DWORD dwErrorPriority,void* pCodeAddress,char* szStr);
BOOL				IsCompressedTextureFormatOk();	
HWND				InitInstance(HINSTANCE hInstance, int nCmdShow);
ATOM				RegisterWindowClass(HINSTANCE hInstance);
void				CentreWindow(HWND hwnd);
LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
DWORD				GetExpTableOfLevel(GAME_OBJECT_TYPE eObjectType, DWORD dwLevel);
DWORD				GetGuardianItemLevel(DWORD dwExp);










