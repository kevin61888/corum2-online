#pragma once
/*	
#######################################################
	#include 선언하는곳
#######################################################
*/
#include <crtdbg.h>
#include <assert.h>
#include "MatchInfo.h"
#include "ObjectDefine.h"
#include "gamedefine.h"
#include "Chat.h"


/*	
#######################################################
	#define 선언하는곳
#######################################################
*/
//=========================================//
// GM Command Deinfe.
#define	GM_COMMAND_PK_MODE		1
#define GM_COMMAND_CHAT_DISABLE	2
#define GM_COMMAND_CREATE		3
#define GM_COMMAND_POTAL		4
#define GM_COMMAND_RECALL		5
#define GM_COMMAND_USERPOTAL	6

#define ZODIAC_USE_TYPE_INVENTORY	0
#define ZODIAC_USE_TYPE_BELT		1


//////////////////////////////////////////////////////////////////////////
// enums
enum DUNGEON_LAYER_PROPERTY
{
	Dungeon_Layer_Property_Village,						// 마을
	Dungeon_Layer_Property_Ancient,						// 고대 
	Dungeon_Layer_Property_Underground_Waterway,		// 지하수로 
	Dungeon_Layer_Property_Underground,					// 지하 
	Dungeon_Layer_Property_Castle,						// 성
	Dungeon_Layer_Property_Ice,							// 얼음 
	Dungeon_Layer_Property_Forest,						// 숲
	Dungeon_Layer_Property_Lava,						// 용암
	Dungeon_Layer_Property_Towner,						// 탑 
	Dungeon_Layer_Property_Aqua,						// 수중던전
	Dungeon_Layer_Property_Ameritart,					// 드래곤의 성역
	Dungeon_Layer_Property_Light_House,					// 등대던전
	Dungeon_Layer_Property_GuildWar_Field,				// 길드전 필드
};


/*	
#######################################################
	class 선언하는곳
#######################################################
*/
class CMonster;
class CUser;
class CMap;
class COnlyList;
class CItem;
struct ITEM;
struct DSTC_JOIN;
struct DSTC_APPEAR;
struct DSTC_APPEAR_MON;
struct DSTC_CHANGE_LAYER;
struct CTDS_ITEM_PICKUP;
struct DSTC_ITEM_PICKUP;
extern CMap*		g_pMap;

typedef struct GlobalVariableDungeon
{
	CMap*					pMap;

	// 인터페이스 공사할떄 이안으로 들어갈 변수들.
	//--------------------------------//

	//-- 채팅 
	BYTE					bCurChatLine;
	BYTE					bChatMode;
	char					szChatDisplayBuf[ MAX_DISPLAY_CHAT_LINE ][ MAX_CHAT_INPUT_LENGTH ];		//채팅 Display용 버퍼 
	BOOL					bMainUserPotalJoin;	// 아 짜증나 이 구조로 짜서 이변수가 필요하네..후..짜증난다.
	//

	/*
	// 아래 동작이 좀 이상하여 주석처리.
	~GlobalVariableDungeon()
	{
		// Delete Map		
		if( g_pMap )
		{
			delete g_pMap;
			g_pMap = NULL;
		}
	}
	*/

} GLOBAL_VARIABLE_DUNGEON, *LPGlobalVariable_Dungeon;


/*	
#######################################################
	extern 변수 선언하는곳
#######################################################
*/
extern int			g_iCameraX;
extern INT			g_iCurrentLayerUserNo;
extern INT			g_iNeedUserNo;
extern INT			g_iNeedKillMonsterNo;
extern BOOL			g_bIsLastLayer;
extern INT			g_iCurrentKillMonsterNo;
extern INT			g_iTotalUserCount;
extern DWORD		g_dwChangeLayerRemainTime;
extern BOOL			g_bMagicChk;
extern BOOL			g_bRButton;
extern BOOL			g_bBeltChk;
extern BOOL			g_bKeyChkUp;
extern BOOL			g_bGuildChk;
extern BOOL			g_bLshift;
extern BOOL			g_bLCtrl;
extern BOOL			g_bLAlt;
extern CMatchInfo	g_MatchInfo;
extern COnlyList*	g_pDyingMonsterList;		// 몬스터 잔상을 위해 시간되면 죽을놈들
extern LPGlobalVariable_Dungeon g_pGVDungeon;


/*	
#######################################################
	Function 선언하는곳
#######################################################
*/
void SendPKMode(BOOL bPKMode);
BOOL InitGameDungeon();
void UpdateGameDungeon();
void ReleaseGameDungeon();
//======================================================================//
// Send Packet Func 관련.												//
//======================================================================//
void SendMovePacket();
void SendStopPacket();
BOOL SendPacketRequestDungeonInfo(WORD wDungeonID);
//======================================================================//
// Effect 관련.															//
//======================================================================//
void EffectOnceAndHideFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
//======================================================================//
// Render 관련.															//
//======================================================================//
DWORD __stdcall AfterRenderGameDungeon();
DWORD __stdcall BeforeRenderGameDungeon();
void ChatRender();									// 채팅 메세지 출력.
void SkillRender();									// 스킬 키 출력.
void EventDungeonRender( void );					// 이벤트 던젼 정보 렌더링.
//======================================================================//
// NPC 관련.															//
//======================================================================//
BOOL LoadNpcInfo(DWORD dwMapID);
void ConvertKeyDown(char* szKey);
void RequestDungeonInfo( BYTE bType, DWORD dwIndex );
void SetFocusInput(int nCurFocus, int nInputType, int nOrder, int nLeft, int nRight, int nTop, int nBottom, BOOL bPos = FALSE);
BOOL ShortKeyEmoticon(WORD wParam, int iLowKey, int iHighKey);	
//======================================================================//
// Game Func.															//
//======================================================================//
BOOL PathFindDungeon();
BOOL SetPathFindMove();	// 함수가 한번씩 호출 될떄마다 길찾기 결과를 하나씩 적용한다.
void DungeonEnvironmentSetting(DUNGEON_DATA_EX* pDungeon);
//======================================================================//
// Init, Remove.														//
//======================================================================//
BOOL	InitChrInfo( char* szFileName, DWORD dwKind, DWORD dwFlag );
void	InitMainPlayer( DSTC_JOIN* pJoin );
void	InitMainPlayer( DSTC_CHANGE_LAYER* pLayer );
CUser*	InitPlayer( DSTC_APPEAR* pAppear );
void	InitMonster( DSTC_APPEAR_MON* pAppear );
#ifdef DEVELOP_MODE
void LoadDebugTile();
void UnloadDebugTile();
#endif
//======================================================================//
// Input Func.															//
//======================================================================//
void OnKeyDownDungeon(WPARAM wParam, LPARAM lParam);
void OnKeyUpDungeon(WPARAM wParam, LPARAM lParam);
void OnLButtonDownDungeon(WPARAM wParam, LPARAM lParam);
void OnLButtonUpDungeon(WPARAM wParam, LPARAM lParam);
void OnRButtonDownDungeon(WPARAM wParam, LPARAM lParam);
void OnRButtonUpDungeon(WPARAM wParam, LPARAM lParam);
void OnMouseMoveDungeon(WPARAM wParam, LPARAM lParam);
void OnMouseWheelDungeon(WPARAM wParam, LPARAM lParam);
void OnCharDungeon(BYTE bCh);
void OnTimerEventDungeon(DWORD dwTimerIndex);
void MouseEventDungeon();
BOOL OnLButtonDownInterfaceDungeon();	// OnLButtonDownDungeon안에 들어가는 함수 만약 TRUE이면 return.
//======================================================================//
// Update Func.															//
//======================================================================//
void PathFindMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void PlayerMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void DamageNumberFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void UpdateMainPlayer( WORD wSectionNum );
void UpdatePlayer();
void UpdateMonster();
void UpdateItem();
void GXClickProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void PlayerShopProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void ShopProc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void OpenQuantityWnd(BYTE bType);
void DisplayMatchInfoOnDungeon();
BOOL ItemCollision(int nIndex);
BOOL BeltCollision();
BOOL GuardianCollision();
void MagicArrayUsed(BYTE byIndex);
void ChatMsg();
void ErrMessage(char* szInfo, DWORD dwColor, BYTE byType = DEF_CHAT_TYPE_ALLCHAT);
void InputChatName(BOOL bChk = FALSE);
void SetKey(int nKey);
void ChatOff();

void CameraAccelatingMove(void);
void SelectSummonMonster(CMonster* pMonster, BOOL bSelect);
void PlayItemSound(WORD	wItemId);
DWORD __stdcall AfterInterporation(AFTER_INTERPOLATION_CALL_BACK_ARG* pArg);
BOOL FindEmptyPosNearDungeon(VECTOR3* vpDungeonPos, VECTOR3 *vpNearPos, BOOL bVillage = FALSE);

//------------------------------------------------------------------------
// minjin ( 2004. 10. 06. ) Source Code Refactoring
//------------------------------------------------------------------------
void SetEffectIndexAll( CUser* pUser, DWORD dwEffectIndex );
void RenderRectChatBox( CUser* pUser, VECTOR2* v2Scale, VECTOR2* v2Position, int nIndex );
void RenderChatMsgOnBox( CUser* pUser, VECTOR2* pV2Pos, int nIndex, DWORD dwColor );
void RenderChatMsgOnBox( CUser* pUser, int x, int y, int nIndex, DWORD dwColor );
