#pragma once
//#include "../4DyuchiGXGFunc/global.h"
//#include "GameDefine.h"
#include "Define.h"
#include "struct.h"

struct	MAP_TILE;
struct	V2_SPRITE;
class	DUNGEON_DATA_EX;
class	CUser;
class	DUNGEON_DATA_EX;

#define	VAR							g_pGVWorld

#define WORLD_CAMERA_DISTANCE_Y			1500.0f
#define WORLD_CAMERA_DISTANCE_Z			-1000.0f
#define WORLD_PLANE_CAMERA_DISTANCE_Y	1950.0f
#define WORLD_PLANE_CAMERA_DISTANCE_Z	-2500.0f

#define WORLD_CAMERA_ANGLE			-0.808261f
#define WORLD_CAMERA_PLANE_ANGLE	-0.598261f

#define MINIMAP_RATIO_X				0.014322	// 220 / 15360	
#define MINIMAP_RATIO_Y				0.011718	// 220 / 15360	

#define DASH_TICK					200
#define MAX_MAP_OBJECT_NUM			0xff


#define WORLD_OBJECT_WATER			100000

#define WORLD_WALK_SPEED			10.0f
#define WORLD_RUNNING_SPEED			20.0f

#define WORLD_WALKING_SPEED_RATIO		0.12f 
#define WORLD_RUNNING_SPEED_RATIO		0.40f 


#define JOIN_TIME_OUT				15000


BOOL InitGameWorld();
void UpdateGameWorld();
void ReleaseGameWorld();
DWORD __stdcall AfterRenderGameWorld();
DWORD __stdcall BeforeRenderGameWorld();

void InitPacketProcWorld();
void OnKeyDownWorld(WPARAM wParam, LPARAM lParam);
void OnKeyUpWorld(WPARAM wParam, LPARAM lParam);
void OnLButtonDownWorld(WPARAM wParam, LPARAM lParam);
void OnLButtonUpWorld(WPARAM wParam, LPARAM lParam);
void OnRButtonDownWorld(WPARAM wParam, LPARAM lParam);
void OnRButtonUpWorld(WPARAM wParam, LPARAM lParam);
void OnMouseMoveWorld(WPARAM wParam, LPARAM lParam);
void OnMouseWheelWorld(WPARAM wParam, LPARAM lParam);
void OnCharWorld(BYTE bCh);
void OnTimerEventWorld(DWORD dwTimerIndex);
void MouseEventWorld();
void SetWorldUserPosition( LP_PARTY_USER pUser, VECTOR3* pv3Pos, BOOL bFlag  = TRUE   );

void MoveProcessWalk();
void MoveProcessPlane();
void AddChatBuffer(char* szBuf);
void SetMinimapPos(VECTOR3 vPos);

BOOL SetPathFindMoveWorld();

void MoveToUserPrevPos();
void PlayerWorldMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void InitWorldMessage();
void RenderChatMessage(int nIndex, int nRect);
void FreeWorldPartyUser( LP_PARTY_USER pPartyUser );
void DungeonEnter(WORD dwDungeonID);
void ClearEventInfo(void);
void SendQueryEventDungeonInfo(void);
void ShowEventDungeonModel(DUNGEON_DATA_EX* pDungeon);
void DrawEventDungeonName(void);
bool DeleteDungeonObjects(void);
void DisplayDungeonInfoOnWorldMap(DUNGEON_DATA_EX* pDungeon);
void OnCrashUser(GXOBJECT_HANDLE handle, LPObjectDesc pData, GXSCHEDULE_PROC_MSG_COLLISION_ARG* pArg);

struct WORLD_PARTY_APPEAR_INFO;
//struct LP_PARTY_USER;

BOOL InitWorldPlayer( WORLD_PARTY_APPEAR_INFO* pAppear, LP_PARTY_USER pPlayer );
void SetActionForPartyUser( GXOBJECT_HANDLE handle, WORD wAct, int nframe, BYTE bFlag );

extern BYTE			g_bPosType;
extern char			g_sNoticeMessage[MAX_NOTICE_COUNT][MAX_NOTICE_SIZE];
extern BYTE			g_byNoticeCount;
extern BOOL			g_bNoticeFlag;
extern DWORD		g_dwPrevTime;
extern DWORD		g_dwNextTime;
extern BYTE			g_byLen[10];
extern int			g_nPGSelect;
extern SMESSAGERQ	g_sMessageRQ[__MAX_READ_MESSAGE__];
extern SWSCHATMSG	g_sWordChatMsg[__MAX_WORD_QUEUE__];

extern void		RenderNotice();
extern void		CheckNoticeTime(void);

extern int		g_nChatModeExt;
extern BOOL		g_bReceiveEventInfo;

typedef struct GlobalVariableWorld
{
	BYTE				bDisplayDungeonInfo;
	VECTOR3				vLightDistance;			//라이트와의 거리 벡터 
	GXOBJECT_HANDLE		hMainSpotWorld;			//월드에 라이트 비출 MainLight	
	GXOBJECT_HANDLE		hMapObjHandle[ MAX_MAP_OBJECT_NUM ];
	WORD				wMapObjectNum;

	BYTE				bCurChatLine;
	char				szChatDisplayBuf[ MAX_DISPLAY_CHAT_LINE ][ MAX_CHAT_INPUT_LENGTH ];		//채팅 Display용 버퍼 
	V2_SPRITE*			pMinimapPos;										//미니맵에 포시할 현재 위차 점 Sprite
	DWORD				dwCurTotalWorldUser;
	BYTE				bChatMode;
	BYTE				bStartLoading;
	
	float				fUserJumpStart_Y;		//유저가 점프를 시작할 시점의 y 좌표		

	DWORD				dwRightMoveTick;
	DWORD				dwLeftMoveTick;
	DWORD				dwUpMoveTick;
	DWORD				dwDownMoveTick;

	DWORD				dwCrashDungeonTick;		//던전에 Join 하려고 던전에 충돌된 당시의 Tick

	BYTE				bMainPlayerJumping;


	DUNGEON_DATA_EX*	pCurSelectDungeon;
	
	
	//BYTE				bAirShipUpDowning;		//비공정 이륙 또는 착륙중 Flag
	//BYTE				bReserveJumpStop;		//점프중에 방향키를 놓았을때 중간에 떨어지지 않게 하기위한 Flag

} GLOBAL_VARIABLE_WORLD, *LPGlobalVariable_World;

extern LPGlobalVariable_World	g_pGVWorld;


