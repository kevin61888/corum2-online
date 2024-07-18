#pragma once


#include "stdafx.h"
#include "Define.h"
#include "GXDefault.h"
#include "ObjectDefine.h"
#include "GlobalVariableLogin.h"


#define __MAX_BUUFER_SIZE__			58
#define MAX_RENDER_MODE				2
#define RENDER_MODE_NORMAL			0
#define RENDER_MODE_MULTIPLE		1
#define DECODE_KEY			"具昞秀天才,骨淫萬萬歲"
#define DECODE_SUBKEY		2
#define TEXT_COLOR_BLUE		0xffada9ff
#define TEXT_COLOR_YELLOW	0xfffcf254
#define TEXT_COLOR_GREEN	0xff00ff00
#define TEXT_COLOR_ORANGE	0Xffffa200
#define TEXT_COLOR_RED		0Xffc42f2f
#define	TEXT_COLOR_PURPLE	0xff9966FF
#define MAX_HACK_APPLICATION	3
#define VECTOR2_TO_VECTOR3(v2, v3) {\
									v3.x = v2.x;\
									v3.y = 0;\
									v3.z = v2.y;\
									}
#define VECTOR3_TO_VECTOR2(v3, v2) {\
									v2.x = v3.x;\
									v2.y = v3.z;\
									}
enum CAMERA_MOVE_OPTION
{
	CAMERA_MOVE_OPTION_CHARACTER_MOVE	= 0,
	CAMERA_MOVE_OPTION_SCREEN_FRAME		= 1,	
	CAMERA_MOVE_OPTION_EFFECTING		= 2,
	CAMERA_MOVE_OPTION_INTERPOLATION	= 3
};


class	CMonster;
class	CUser;
class	CMainUser;
class	CMap;
struct	ITEM;
class	EffectDesc;
struct	BlockLocation;
class	CSoundControl;
class	DUNGEON_DATA_EX;


static char* g_szHackApplication[MAX_HACK_APPLICATION] =
{
	"Detected By DungeonServer",
	"A Speeder",
	"오토마우스",
};


typedef struct mousestate
{
	bool	bLDown;				// LButton이 눌렸는가..
	bool	bRDown;				// RButton이 눌렸는가.. 
	bool	bMDown;				// MButton이 눌렸는가..

	POINT	MouseOldPos;		// 바로전 마우스 좌표.
	POINT	MousePos;			// 2D 마우스 좌표.
	VECTOR3 v3Mouse;			// 3D에서 변환한 마우스 좌표.

	DWORD	dwLWaitTick;
	DWORD	dwRWaitTick;
	DWORD	dwLButtonDownTime;	// 마지막으로 L버튼이 눌린 시각.
	DWORD	dwRButtonDownTime;	// 마지막으로 R버튼이 눌린 시각.
	
	mousestate()
	{
		bLDown				= false;
		bRDown				= false;
		bMDown				= false;

		MouseOldPos.x		= 0;
		MouseOldPos.y		= 0;
		
		MousePos.x			= 0;
		MousePos.y			= 0;

		v3Mouse.x			= 0;
		v3Mouse.y			= 0;
		v3Mouse.z			= 0;

		dwLWaitTick			= 0;
		dwRWaitTick			= 0;
		dwLButtonDownTime	= 0;
		dwRButtonDownTime	= 0;
	}
	
} MouseState, * LPMouseState;


struct CAMERA_INFO
{
	VECTOR3		v3CameraPos;
	VECTOR3		v3AxsiY;
	VECTOR3		v3Angle;
	VECTOR3		v3ListenerAngle;
	CAMERA_DESC	CameraDesc;

	float		fCameraDistance_x;	// 주인공과 카메라사이의 z거리.
	float		fCameraDistance_y;	// 주인공과 카메라사이의 z거리.
	float		fCameraDistance_z;	// 주인공과 카메라사이의 z거리.
	float		fCameraAngle;
	
	long int	iCameraMoveOption;	// 카메라 이동과 관련된 플래그.
	
	float		fAccel_x;			// 카메라 정지시, '가속도'와 관련된 플래그.
	float		fAccel_y;	
};


struct KEY_PRESS_STATE
{
	bool	bVK_UP;			
	bool	bVK_DOWN;		
	bool	bVK_LEFT;
	bool	bVK_RIGHT;

	bool IsUp()		{	return bVK_UP;		}
	bool IsDown()	{	return bVK_DOWN;	}
	bool IsLeft()	{	return bVK_LEFT;	}
	bool IsRight()	{	return bVK_RIGHT;	}
	bool IsAllUp()	
	{
		if(bVK_DOWN || bVK_LEFT || bVK_RIGHT || bVK_UP)
			return false;
		else
			return true;
	}
};


struct HELP_LOAD_SPRITE 
{
	IDISpriteObject *	pHelpMapSpriteObj;
	IDISpriteObject *	pHelpMsgSpriteObj;
	DWORD				dwMapID;
	int					iHelp;
	char				szMapName[255];
	char				szHelpMsg[255];
	bool				bIsLoading;
	
	HELP_LOAD_SPRITE();
	
	void SetLoadingFlag(bool bFlag);
	
	void Load(DWORD dwMapID);

	void Show();

	void ReleaseSprites();
};

struct A_STAR
{
	DWORD	dwCurveNum;			// 길찾기 중에서 방향이 달라지는 지점의 개수.
	DWORD	dwCurCurve;			// 현재까지 진행된 커브.

	BlockLocation*	pLocation;	// 커브할곳의 위치값을 가진 배열.
};


extern char				g_szBasePath[ _MAX_PATH ];
extern char				g_szDataPath[ _MAX_PATH ];
extern char				g_szTempFileName[ _MAX_PATH ];
#ifdef __PACKAGE_MODE__
extern char				g_szPackPath[ _MAX_PATH ];
#endif
extern BYTE				g_bRenderMode;
extern BYTE				g_bIsRunning;
extern DWORD			g_dwDescNum;
extern DWORD			g_dwUserNum;
extern DWORD			g_dwAllocSound;
extern DWORD			g_dwFreeSound;
extern DWORD			g_dwCurTick;
extern DWORD			g_dwLayerID;
extern DWORD			g_dwLastSearchHackTick;
extern DWORD			g_dwLastCheckTick;
extern DWORD			g_dwCurInputFocus;
extern int				g_nChatModeExt;
extern int				iDeleteObject;
extern BOOL				g_bCap;
extern BOOL				g_bNowLoading;
extern BOOL				g_bAdultMode;		
extern HFONT			g_hFont;			

extern MouseState		g_Mouse;
extern GXMAP_HANDLE		g_pMapHandle;
extern DUNGEON_DATA_EX*	g_pThisDungeon;
extern HELP_LOAD_SPRITE g_helpLoadingSprite;	
extern RECT				g_rcScreenRect;
extern RECT				g_rcSelectBox;		
extern CAMERA_INFO		g_Camera;
extern KEY_PRESS_STATE	g_KeyPress;
extern FOG_DESC			g_FogDesc;
extern NETMARBLE_LOGIN	g_NetMarble;

extern CMainUser*		g_pMainPlayer;		
extern CSoundControl*	g_pSoundControl;


extern void	(*Render[ MAX_RENDER_MODE ])();
extern BOOL	(*InitGameProcess[ MAX_UPDATE_GAME ])();
extern void (*UpdateGameProcess[ MAX_UPDATE_GAME ])();
extern void	(*ReleaseGameProcess[MAX_UPDATE_GAME])();
extern DWORD (__stdcall *AfterRender[ MAX_UPDATE_GAME ])();
extern DWORD (__stdcall *BeforeRender[ MAX_UPDATE_GAME ])();
extern UPDATE_GAME_STATUS GetGameStatus();
extern BOOL SetGameStatus( UPDATE_GAME_STATUS bStatus );


DWORD			GetRandom( DWORD dwMax );
char*			GetFile( char* szFileName, BYTE bDataType );
char*			GetFile(BYTE bResType, DWORD dwResUID);
void			LoadCPTable();
BOOL			LoadNpcInfo();
void			PreLoadItem();
void			PreLoadDamageNumber();
void			PreLoadCurLayerMonster(DWORD* pdwMonsterKind);
void			RenderNormal();
void			RenderMultiple();
void			SetAction( GXOBJECT_HANDLE handle, WORD wAct, BYTE bFlag = ACTION_LOOP );
void			SetAction( GXOBJECT_HANDLE handle, WORD wAct, int nframe = 0, BYTE bFlag = ACTION_LOOP );
void			SetActionNext( GXOBJECT_HANDLE handle, WORD wAct, WORD wNextAct, BYTE bNextFlag, int nNextFrame = 0  );
void			SetUserPosition( CUser* pUser, VECTOR3* pv3Pos, BOOL bFlag = TRUE );
void			SetMonsterPosition( CMonster* pMonster, VECTOR3* pv3Pos, BOOL bFlag = TRUE );
void			SetProgressBar(DWORD dwTotalProcess, DWORD dwCurProcess);
BOOL			SetRenderMode( BYTE bMode );
void			ClearInputBuffer(BYTE bBufferIndex);
void			ChangeLanguage(BOOL bHangul);
void			ShowObject(GXOBJECT_HANDLE handle);
void			HideObject(GXOBJECT_HANDLE handle);
void			EnablePick(GXOBJECT_HANDLE handle);
void			DisablePick(GXOBJECT_HANDLE handle);
void			EnableEffect(GXOBJECT_HANDLE handle);
void			DisalbeEffect(GXOBJECT_HANDLE handle);
void			SetObjectPosition(GXOBJECT_HANDLE handle, float fX, float fY, float fZ);
BOOL			LoadWorldMapScript(char* szMapFile, DWORD dwFlag);		
void			InitMap( DWORD dwLayerID );
void			LoadWorldMap( DWORD dwMapResourceUID );	
void			SetBoundingVolume(GXOBJECT_HANDLE handle, float fRadius);
void			ReleaseBoundingVolume(GXOBJECT_HANDLE handle);
BOOL			EpsilonVector(VECTOR3*	v3Vec1, VECTOR3* v3Vec2, float fEpsilon );
BOOL			ChangeAddressType(DWORD dwIP, char* OUT szAddress);
BOOL			InitSearchModule(CMap* pMap);	
char*			GetFile( char* szFileName, BYTE bDataType );
char*			GetFile(BYTE bResType, DWORD dwResUID);
char*			FILEDEF(BYTE bResType, DWORD dwResUID);	
void			OnIMEComposition(BOOL bResultStr);	
void			SetCameraTarget(GXOBJECT_HANDLE handle);		
void			SetCameraTargetWithHeight(GXOBJECT_HANDLE handle);
void			SetCameraYaw( VECTOR3* pCameraTo, float fYew );
void			SetCameraPitch( VECTOR3* pCameraTo, float fPitch );
void			SetCameraPos(VECTOR3* pCameraTo, float wLength );
void			InitCamera();		
void			InitDungeonCamera();		
void			TerminateDungeonCamera();
void			UpdateMainPlayerToMap();	
void			ShowLoading();
void			FreeUser( CUser* pUser );
void			FreeMonster( CMonster* pMonster );
void			FreeItem( ITEM* pItem );
void			FreeEffect( EffectDesc* pEffect );
void			RemoveAllItem();
void			RemoveItem( ITEM* pItem );
void			ReleaseMainPlayer();
BOOL			IsPKZone();
void			RemovePlayer( CUser* pUser );		
void			RemoveAllPlayer();
void			RemoveAllMonster();
void			RemoveMonster( CMonster* pMonster );
void			RemoveAllEffectDesc();
void			RemoveEffectDesc( EffectDesc*	pEffectDesc );
void			SendLoadingStatus(BYTE bIsLoading);
void			MinimizePlayer(GXOBJECT_HANDLE handle, BOOL bMinimize = TRUE);
bool			RemoveCharacter(char* pszResult, char* pszSource, char cCharacter);
bool			MakeLowerString(char* pszResult, char* pszSource);
char*			CheckHackApplication( char* szCaption );
void			SearchHack();
void			CheckTick();
LPObjectDesc	AllocObjDesc();
void			FreeObjDesc( LPObjectDesc pDesc );
GXOBJECT_HANDLE	CreateHandleObject( char* szFileName, GXSchedulePROC pProc, void* pData, DWORD dwFlag );
void			DeleteHandleObject( GXOBJECT_HANDLE hHandle );
BOOL			CreateSound( SOUND_HANDLE_DESC* pSoundDesc, DWORD dwSoundNum, VECTOR3 *pv3SoundPos );
BOOL			ReleaseSound( SOUND_HANDLE_DESC* pSoundDesc );
void			SetListener(VECTOR3 * pV3Angle);
GXLIGHT_HANDLE	CreateLight(LIGHT_DESC* pLightDesc, VECTOR3* pVecPostion, BYTE bLightNum);
void			ReleaseMainPlayerResource();
void			InitializeEffect(BOOL bChk = FALSE);
DWORD			DecodeCDBData(char* szLoadFile,  void* pReturnValue, char* szDecodeKey = DECODE_KEY, int nDecodeSubKey = DECODE_SUBKEY);
BOOL			InitChrInfo( char* szFileName, DWORD dwKind, DWORD dwFlag );
LPMouseState	GetMouse();
void			InitBaseMonsterInfo();
int				FillSelectedMonster( DWORD* pdwGuardian, DWORD* pdwMonster );		
void			ObjectSoundPlay(LPObjectDesc pObjDesc, DWORD dwMotionIndex, BYTE bFrameIndex);
void			LoadTotalDungeonInfo();
DWORD			GetUserIndexByNameFromUserHash(const char * pszCharacterName);
void			OpenWebSite(const char * szSite);
char*			_StrTok(const char* str, const char sep);
BOOL			CmdLineProcessForNetMarble(LPSTR lpCmdLine);
BOOL			RunAutoPatch();
#ifdef __PACKAGE_MODE__
int				LoadPakList(char* pszFileName, PACKFILE_NAME_TABLE* pOut, int iPakTableSize);
#endif











