#include <crtdbg.h>
#include "GameControl.h"
#include "InitGame.h"
#include "Map.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "DungeonTable.h"
#include "HashTable.h"
#include "ChrInfo.h"
#include "WorldProcess.h"
#include "../CommonServer/CommonHeader.h"
#include "UserInterface.h"
#include "GameMenuWnd.h"
#include "message.h"
#include "initgame.h"
#include "DSMiniMap.h"
#include "GroupWnd.h"
#include "PlayerShopWnd.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "gamemenuwnd.h"
#include "PathFinding\SearchModule_Astar.h"
#include "NetworkClient.h"
#include "Effect.h"
#include "PartyEntryWnd.h"
#include "CodeFun.h"
#include "NetworkTimeObserver.h"
#include "NMCrypt.h"


void	(*Render[ MAX_RENDER_MODE ])();
BOOL	(*InitGameProcess[ MAX_UPDATE_GAME ])();
void	(*UpdateGameProcess[ MAX_UPDATE_GAME ])();
void	(*ReleaseGameProcess[ MAX_UPDATE_GAME ])();
DWORD	(__stdcall *AfterRender[ MAX_UPDATE_GAME ])();
DWORD	(__stdcall *BeforeRender[ MAX_UPDATE_GAME ])();


char				g_szBasePath[ _MAX_PATH ];
char				g_szDataPath[ _MAX_PATH ];
char				g_szTempFileName[ _MAX_PATH ];
#ifdef __PACKAGE_MODE__
char				g_szPackPath[ _MAX_PATH ];
#endif
BYTE				g_bPause				= 0;
BYTE				g_bIsRunning			= TRUE;	
BYTE				g_bRenderMode			= RENDER_MODE_MULTIPLE;	
DWORD				g_dwDescNum				= 0;
DWORD				g_dwUserNum				= 0;
DWORD				g_dwAllocSound			= 0;
DWORD				g_dwFreeSound			= 0;
DWORD				g_dwLastSearchHackTick	= 0;
DWORD				g_dwLastCheckTick		= 0;
DWORD				g_dwCurTick				= 0;
DWORD				g_dwLayerID				= 0;
DWORD				g_dwCurInputFocus		= 0;					
BOOL				g_bCap					= FALSE;
BOOL				g_bAdultMode			= 0;
BOOL				g_bNowLoading			= FALSE;
int					iDeleteObject			= 0;

RECT				g_rcScreenRect;
RECT				g_rcSelectBox;
UPDATE_GAME_STATUS	g_bGameStatus;	
MouseState			g_Mouse;
CAMERA_INFO			g_Camera;
KEY_PRESS_STATE		g_KeyPress;
GXMAP_HANDLE		g_pMapHandle = NULL;
CPTable				g_pCPTable[MAX_CP_TABLE];
HELP_LOAD_SPRITE	g_helpLoadingSprite;
FOG_DESC			g_FogDesc;
NETMARBLE_LOGIN		g_NetMarble;

CSoundControl*		g_pSoundControl			= NULL;
INPUT_BUFFER*		g_InputBuffer			= NULL;
CMainUser*			g_pMainPlayer			= NULL;						
CMap*				g_pMap					= NULL;
DUNGEON_DATA_EX*	g_pThisDungeon			= NULL;


HELP_LOAD_SPRITE::HELP_LOAD_SPRITE()
{
	pHelpMapSpriteObj = NULL;
	pHelpMsgSpriteObj = NULL;
	dwMapID	= 0;
	iHelp = 0;
	bIsLoading = FALSE;
}

void HELP_LOAD_SPRITE::SetLoadingFlag(bool bFlag)
{
	bIsLoading = bFlag;
}

void HELP_LOAD_SPRITE::Load(DWORD dwMapID)
{
	DWORD id;

	ReleaseSprites();

	if (dwMapID < 100)
	{ 
		// 마을 
		id = dwMapID;
	}
	else if (dwMapID >= 100 && dwMapID < 200)
	{ 
		// 고대 던전 
		id = 100;
	}
	else if (dwMapID >= 200 && dwMapID < 300)
	{ 
		// 지하수로 던전 
		id = 200;
	}
	else if (dwMapID >= 300 && dwMapID < 401)
	{
		// 성 지하 던전
		id = 300;
	}
	else if (dwMapID >= 401 && dwMapID < 500)
	{ 
		// 지하 던전
		id = 401;
	}
	else if (dwMapID >= 500 && dwMapID < 600)
	{ 
		// 아이스 던전 
		id = 500;
	}
	else if (dwMapID >= 600 && dwMapID < 700)
	{ 
		// 숲 던전
		id = 600;
	}
	else if (dwMapID >= 700 && dwMapID < 800 )
	{
		// 이벤트 던전때문에 던전아이디를 달리해놓았다.
		// 결국 성 지하 던전 이다.
		id = 300;
	}
	else if (dwMapID >= 900 && dwMapID < 1000)
	{ 
		// 용암 던전 
		id = 900;
	}
	else
	{
		id = dwMapID;
	}
	
	// 던전 해당 맵 이미지 파일을 얻는다.
	char szBuf[255];

	if (dwMapID >= 10000)
	{
		// 월드멥
		wsprintf(szBuf, "L_worldmap%d.tga", dwMapID%10000);
	}
	else if (dwMapID >= 1001 && dwMapID <= 1099)
	{ 
		// 통로 던전 
		if (dwMapID >= 1001 && dwMapID <= 1010)
		{
			wsprintf(szBuf, "L_T_%02d.tga", dwMapID%100);
		}
		else if (dwMapID >= 1011 && dwMapID <= 1016)
		{
			wsprintf(szBuf, "L_T_%02d.tga", 11);	
		}
		else if (dwMapID >= 1014 && dwMapID <= 1016)
		{
			wsprintf(szBuf, "L_T_%02d.tga", 14);	
		}
		else if (dwMapID >= 1017 && dwMapID <= 1019)
		{
			wsprintf(szBuf, "L_T_%02d.tga", 17);	
		}	
		else if (dwMapID >= 1020 && dwMapID <= 1022)
		{
			wsprintf(szBuf, "L_T_%02d.tga", 20);	
		}
		else if (dwMapID >= 1023)
		{
			wsprintf(szBuf, "L_T_%02d.tga", 23);	
		}
	}
	else if (dwMapID >= 1101 && dwMapID <=1105)
	{ 
		// 탑던전 
		wsprintf(szBuf, "L_%d.tga", 1100);
	}
	else if (dwMapID >= 1111 && dwMapID <=1115)
	{ 
		wsprintf(szBuf, "L_%d.tga", 1101);
	}  
	else if (dwMapID == 801)//길드던전
	{ 
		wsprintf(szBuf, "L_T_%d.tga", 801);
	}  
	else if (dwMapID >= 1201 && dwMapID <= 1205)//수중던전
	{ 
		wsprintf(szBuf, "L_T_%d.tga", 1205);
	}
	else if (dwMapID >= 1221 && dwMapID <= 1225)//등대던전
	{ 
		wsprintf(szBuf, "L_T_%d.tga", 1225);
	}
	else
	{ 
		wsprintf(szBuf, "L_%d.tga", id);
	}
	// sung-han 2005-03-23 로딩 이미지 변경(무조건 랜덤)
	//---------------------------------------------------------
	srand(timeGetTime());
	int iNum = rand()%4;//이미지가 4개 있다. 추가되면 숫자를 늘려라.
	ZeroMemory(szBuf, sizeof(szBuf));
	wsprintf(szBuf, "loadimg_%03d.tga", iNum);
	//---------------------------------------------------------

	wsprintf(szMapName, "%s", GetFile(szBuf, DATA_TYPE_MAP));
	pHelpMapSpriteObj = g_pRenderer->CreateSpriteObject(szMapName, FALSE);

	// 도움말 이미지 파일을 얻는다.
	int iHelp = GetRandom(20)+1; // 헬프 메시지 번호를  얻는다.
	wsprintf(szBuf, "help_%03d.tga", iHelp);
	wsprintf(szHelpMsg, "%s", GetFile(szBuf, DATA_TYPE_UI));

	pHelpMsgSpriteObj = g_pRenderer->CreateSpriteObject(szHelpMsg, FALSE);
}

void HELP_LOAD_SPRITE::Show()
{
	if ( !bIsLoading )
	{
		return;
	}

	VECTOR2	v2Pos;	
	VECTOR2	v2Scale;

	v2Scale.x = 1.0f;
	v2Scale.y = 1.0f;
	
	if (pHelpMapSpriteObj)
	{
		// sung-han 2005-03-23 로딩 이미지 변경(무조건 랜덤)
		// 여기서 로딩 위치를 잡는다.
//		v2Pos.x = 256;
//		v2Pos.y = 256;
		v2Pos.x = 0;
		v2Pos.y = 92;

		g_pExecutive->GetGeometry()->RenderSprite( pHelpMapSpriteObj, NULL,0.0f,&v2Pos,NULL,
			0xffffffff,__ORDER_LOAD_INTERFACE__+3,RENDER_TYPE_DISABLE_TEX_FILTERING);
	}

	// HELP 메시지 이미지 
	if (pHelpMsgSpriteObj)
	{
		v2Pos.x = 8;
		v2Pos.y = 720;

		g_pExecutive->GetGeometry()->RenderSprite( pHelpMsgSpriteObj, NULL,0.0f,&v2Pos,NULL,
			0xffffffff,__ORDER_LOAD_INTERFACE__+3,RENDER_TYPE_DISABLE_TEX_FILTERING);
	}
}

void HELP_LOAD_SPRITE::ReleaseSprites()
{
	if (pHelpMapSpriteObj)
	{
		pHelpMapSpriteObj->Release();
		pHelpMapSpriteObj = NULL;
	}

	if (pHelpMsgSpriteObj)
	{
		pHelpMsgSpriteObj->Release();
		pHelpMsgSpriteObj = NULL;
	}
}

#include <direct.h>
void CaptureScreen()
{
	static int i = 0;

	if( g_bCap )
	{
		if( g_pRenderer )
		{
			g_dwCurTick = timeGetTime();

			SYSTEMTIME time;
			char szFileName[MAX_PATH] = {0,};
			GetLocalTime(&time);
			
			char szTargetFolder[MAX_PATH] = {0,}; 
			const char szScreenShotFolder[] = "/MyScreenshot";
			sprintf(szTargetFolder,"%s%s",g_szBasePath,szScreenShotFolder);
			
			mkdir(szTargetFolder);
			::sprintf( szFileName, "%s/Save %04d-%02d-%02d %02d-%02d(%d).tga", //2002 09 25 11 15
				szTargetFolder,
				time.wYear,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				i++);
			
			g_pRenderer->CaptureScreen( szFileName );
		}
		g_bCap = FALSE;
	}
}


void RenderNormal()
{	
#ifndef DEVELOP_MODE
	g_dwCurTick = timeGetTime();

	if (g_pExecutive->Run(0xff000000, BeforeRender[ GetGameStatus() ], AfterRender[ GetGameStatus() ], 0) )	
	{
		if( g_bPause ) return;	

		(*UpdateGameProcess[ GetGameStatus()])();
	}
	
	if (NULL != g_pSoundLib)
	{
		g_pSoundLib->Run();
	}

	CaptureScreen();

	g_pGeometry->Present(g_hMainWnd);
#else
	Sleep(2);

	if(g_Dev.bRenderFramePerSec)	
	{
		g_pRenderer->BeginPerformanceAnalyze();
	}

	g_dwCurTick = timeGetTime();		

	if (g_pExecutive->Run(0xff000000, BeforeRender[ GetGameStatus() ], AfterRender[ GetGameStatus()], 0) )	
	{
		if( g_bPause ) return;
		
		(*UpdateGameProcess[ GetGameStatus()])();
	}

	if (NULL != g_pSoundLib)
	{
		g_pSoundLib->Run();
	}

	if(g_Dev.bRenderFramePerSec)	
	{
		g_pRenderer->EndPerformanceAnalyze();
	}
	
	CaptureScreen();

	g_pGeometry->Present(g_hMainWnd);
#endif
}

void RenderMultiple()
{	
#ifndef DEVELOP_MODE	
	if( g_bPause ) return;	
	
	if(g_Dev.bRenderFramePerSec)	
		g_pRenderer->BeginPerformanceAnalyze();
	
	g_dwCurTick = timeGetTime();	
	DWORD	dwGameFrameProcess	=	g_pExecutive->Process();
	if( dwGameFrameProcess)
	{
		if( g_bPause ) return;	
		(*UpdateGameProcess[ GetGameStatus() ])();
	}
	
	g_pGeometry->BeginRender(0,0xff000000, BEGIN_RENDER_FLAG_USE_RENDER_TEXTURE);
	if(BeforeRender[ GetGameStatus()])
		(*BeforeRender[ GetGameStatus() ])();

	g_pExecutive->Render();

	if(AfterRender[ GetGameStatus()])
		(*AfterRender[ GetGameStatus() ])();
	g_pGeometry->EndRender();
	
	// 사운드.
	if (NULL != g_pSoundLib)
	{
		g_pSoundLib->Run();
	}

	if(g_Dev.bRenderFramePerSec)	
		g_pRenderer->EndPerformanceAnalyze();

	CaptureScreen();
	g_pGeometry->Present( g_hMainWnd );
#else
	if( g_bPause ) return;	
	Sleep(2);
	if(g_Dev.bRenderFramePerSec)	
		g_pRenderer->BeginPerformanceAnalyze();
	
	g_dwCurTick = timeGetTime();	
	DWORD	dwGameFrameProcess	=	g_pExecutive->Process();
	if( dwGameFrameProcess)
	{
		if( g_bPause ) return;	
		(*UpdateGameProcess[ GetGameStatus() ])();
	}
	
	g_pGeometry->BeginRender(0,0xff000000, BEGIN_RENDER_FLAG_USE_RENDER_TEXTURE);
	if(BeforeRender[ GetGameStatus()])
		(*BeforeRender[ GetGameStatus() ])();

	g_pExecutive->Render();

	if(AfterRender[ GetGameStatus()])
		(*AfterRender[ GetGameStatus() ])();
	g_pGeometry->EndRender();
	
	if (NULL != g_pSoundLib)
	{
		g_pSoundLib->Run();
	}

	if(g_Dev.bRenderFramePerSec)	
		g_pRenderer->EndPerformanceAnalyze();

	CaptureScreen();
	g_pGeometry->Present( g_hMainWnd );
#endif
}

void EnableEffect(GXOBJECT_HANDLE handle)
{
	g_pExecutive->GXOEnableAsEffect(handle);
}

void DisalbeEffect(GXOBJECT_HANDLE handle)
{
	g_pExecutive->GXODisableAsEffect(handle);
}

UPDATE_GAME_STATUS GetGameStatus()
{
	return g_bGameStatus;
}
BOOL SetGameStatus( UPDATE_GAME_STATUS bStatus )
{
	if( bStatus >= MAX_UPDATE_GAME ) 
		return false;

	(*ReleaseGameProcess[GetGameStatus()])();
	BOOL bRes = (*InitGameProcess[ bStatus ])();

	if( bRes ) 
	{
		g_bGameStatus = bStatus;
	}
	else 
	{
		(*ReleaseGameProcess[ bStatus ])();
		PostMessage( g_hMainWnd, WM_CLOSE, 0, 0 );
		g_bPause = TRUE;
	}

	return bRes;
}

BOOL SetRenderMode(BYTE bMode)
{
	if(bMode > MAX_RENDER_MODE)	return FALSE;
	
	g_bRenderMode = bMode;
	return TRUE;
}

void SetAction( GXOBJECT_HANDLE handle, WORD wAct, BYTE bFlag )
{
	LPObjectDesc	pDesc = (LPObjectDesc)g_pExecutive->GetData( handle );

	if(!pDesc)	
	{	
		// MSG_ID : 278 ; Fail to ChangeAction!
		MessageBox(g_hMainWnd,g_Message[ETC_MESSAGE278].szMessage,NULL,NULL);	
		return;	
	}  

	if(pDesc->bType==OBJECT_TYPE_PLAYER || pDesc->bType==OBJECT_TYPE_MAINPLAYER)
	{
		CUser* pUser = (CUser*)pDesc->pInfo;

		if(pUser)
		{	
			pUser->m_dwTemp[USER_TEMP_IDLETIME] = 0;
			wAct	%= max(pUser->m_byItemType*50, 50);
			wAct	=  WORD(pUser->m_byItemType*50+wAct);			
		}
	}

	pDesc->bActionFlag	= bFlag;
	pDesc->nFrame		= 0;
	pDesc->nCurFrame	= 0;
	g_pExecutive->GXOSetCurrentFrame( handle, 0 );
	g_pExecutive->GXOSetCurrentMotionIndex( handle, wAct );
}

void SetAction( GXOBJECT_HANDLE handle, WORD wAct, int nframe /*= 0*/, BYTE bFlag /*= ACTION_LOOP*/)
{
	LPObjectDesc	pDesc = (LPObjectDesc)g_pExecutive->GetData( handle );

	if(!pDesc)	
	{	
		// MSG_ID : 278 ; Fail to ChangeAction!
		MessageBox(g_hMainWnd,g_Message[ETC_MESSAGE278].szMessage,NULL,NULL);	
		return;	
	}	
	
	if(pDesc->bType==OBJECT_TYPE_PLAYER || pDesc->bType==OBJECT_TYPE_MAINPLAYER)
	{
		CUser* pUser = (CUser*)pDesc->pInfo;

		if(pUser)
		{		
			pUser->m_dwTemp[USER_TEMP_IDLETIME] = 0;
			wAct	%=	max(pUser->m_byItemType*50, 50);
			wAct	=	WORD(pUser->m_byItemType*50+wAct);			
		}
	}
	
	pDesc->bActionFlag	= bFlag;
	g_pExecutive->GXOSetCurrentMotionIndex( handle, wAct );
	
	pDesc->nFrame	= 0;
	g_pExecutive->GXOSetCurrentFrame( handle, nframe );
	pDesc->nCurFrame = nframe;
	
}

void SetActionNext( GXOBJECT_HANDLE handle, WORD wAct, WORD wNextAct, BYTE bNextFlag, int nNextFrame )
{
	LPObjectDesc pDesc = (LPObjectDesc)g_pExecutive->GetData(handle);

	if(!pDesc)	
	{	
		// MSG_ID : 278 ; Fail to ChangeAction!
		MessageBox(g_hMainWnd,g_Message[ETC_MESSAGE278].szMessage,NULL,NULL);	
		return;	
	}	

	if(pDesc->bType==OBJECT_TYPE_PLAYER || pDesc->bType==OBJECT_TYPE_MAINPLAYER)
	{
		CUser* pUser = (CUser*)pDesc->pInfo;

		if(pUser)
		{
			pUser->m_dwTemp[USER_TEMP_IDLETIME] = 0;
			wAct		%=	max(pUser->m_byItemType*50, 50);
			wNextAct	%=	max(pUser->m_byItemType*50, 50);
			wAct		=	WORD(pUser->m_byItemType*50+wAct);
			wNextAct	=	WORD(pUser->m_byItemType*50+wNextAct);			
		}
	}

	pDesc->bActionFlag	= ACTION_NEXT;
	pDesc->bNextFlag	= bNextFlag;
	pDesc->wNextAct		= wNextAct;
	pDesc->nNextFrame	= nNextFrame;

	g_pExecutive->GXOSetCurrentMotionIndex( handle, wAct );	
	g_pExecutive->GXOSetCurrentFrame( handle, 0 );
	pDesc->nCurFrame = 0;
}

void SetUserPosition( CUser* pUser, VECTOR3* pv3Pos, BOOL bFlag /* = TRUE  */ )
{
	MAP_TILE* pTile;
	pTile = g_pMap->GetTile( pv3Pos->x, pv3Pos->z );

	if (!pTile)		
		return;
	
	if ( !pUser->m_hPlayer.pHandle ) 
		return;
	

	pUser->m_v3CurPos.x = pv3Pos->x;
	pUser->m_v3CurPos.y	= pv3Pos->y;
	pUser->m_v3CurPos.z = pv3Pos->z;
	
	if (pUser->m_v3CurPos.x < 0)							pUser->m_v3CurPos.x = 0;
	else if(pUser->m_v3CurPos.x > g_pMap->m_fMapXLength)	pUser->m_v3CurPos.x = g_pMap->m_fMapXLength-1;
	if (pUser->m_v3CurPos.z < 0)							pUser->m_v3CurPos.z = 0;
	else if(pUser->m_v3CurPos.z > g_pMap->m_fMapZLength)	pUser->m_v3CurPos.z = g_pMap->m_fMapZLength-1;
	
	
	if( bFlag )
	{
		GXSetPosition( pUser->m_hPlayer.pHandle, &pUser->m_v3CurPos, FALSE , TRUE);

		if( g_pMainPlayer != pUser )
		{
			if( pUser->m_pCurTile != pTile )
			{
				g_pMap->SetTileOccupied( pUser->m_pCurTile->wIndex_X, pUser->m_pCurTile->wIndex_Z, TILE_EMPTY, pUser );
				g_pMap->SetTileOccupied( pTile->wIndex_X, pTile->wIndex_Z, TILE_OCCUPIED_PC, pUser );
		
				pUser->m_pCurTile = pTile;
				pUser->m_wCurSectionNum = pTile->wAttr.uSection;
			}
		}
	}
	
	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();
	
	if(pGameMenuWnd->m_bShadowFlag==FALSE)
		GXSetPosition(pUser->m_hShadowHandle, &pUser->m_v3CurPos, FALSE, TRUE);
}


void SetMonsterPosition( CMonster* pMonster, VECTOR3* pv3Pos, BOOL bFlag /* = TRUE  */ )
{
	MAP_TILE* pTile;
	pTile = g_pMap->GetTile( pv3Pos->x, pv3Pos->z );
	if (!pTile)	
		return;
	
	if( !pMonster->m_hMonster.pHandle )
		return;

	pMonster->m_v3CurPos.x	= pv3Pos->x;
	pMonster->m_v3CurPos.y	= pv3Pos->y;
	pMonster->m_v3CurPos.z	= pv3Pos->z;
	
	if( bFlag == TRUE )
	{
		GXSetPosition( pMonster->m_hMonster.pHandle, &pMonster->m_v3CurPos, FALSE );
		
		if( pMonster->m_pEffectTarget )
		{
			GXSetPosition( pMonster->m_pEffectTarget->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
		}
		
		if( pMonster->m_pEffectSelect )
		{
			GXSetPosition( pMonster->m_pEffectSelect->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
		}

		if( pMonster->m_pCurTile != pTile )
		{
			g_pMap->SetTileOccupied( pMonster->m_pCurTile->wIndex_X, pMonster->m_pCurTile->wIndex_Z, TILE_EMPTY, pMonster );
			g_pMap->SetTileOccupied( pTile->wIndex_X, pTile->wIndex_Z, TILE_OCCUPIED_PC, pMonster );

			pMonster->m_pCurTile = pTile;
			pMonster->m_wCurSectionNum = pTile->wAttr.uSection;
		}
	}	
	else if( bFlag == 2)
	{
		GXSetPosition( pMonster->m_hMonster.pHandle, &pMonster->m_v3CurPos, FALSE );		
		
		if( pMonster->m_pEffectSelect )
		{
			GXSetPosition( pMonster->m_pEffectSelect->hEffect.pHandle, &pMonster->m_v3CurPos, FALSE );
		}
	}

	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();

	if(pGameMenuWnd->m_bShadowFlag==FALSE)
		GXSetPosition(pMonster->m_hShadowHandle, &pMonster->m_v3CurPos, FALSE);
}


void ChangeLanguage(BOOL bHangul)
{
	// 한글에만 적용되도록 한다.
#if IS_KOREA_LOCALIZING()
	DWORD dwConVersion,dwSenTence;
	::ImmGetConversionStatus(g_hImc,&dwConVersion,&dwSenTence);
	
	if(bHangul)	
		dwConVersion = IME_CMODE_HANGUL; //한글 변환 
	else	
		dwConVersion = 0;				 //영문변환 	

	::ImmSetConversionStatus(g_hImc, dwConVersion, dwSenTence);
#endif 
}

LPMouseState GetMouse()
{ 
	return &g_Mouse; 
}

void ShowObject(GXOBJECT_HANDLE handle)
{
	g_pExecutive->EnableRender(handle);
	g_pExecutive->EnableSchedule(handle);
}

void HideObject(GXOBJECT_HANDLE handle)
{
	g_pExecutive->DisableRender(handle);
	g_pExecutive->DisableSchedule(handle);
}

void EnablePick(GXOBJECT_HANDLE handle)
{
	g_pExecutive->EnablePick(handle);
}

void DisablePick(GXOBJECT_HANDLE handle)
{
	g_pExecutive->DisablePick(handle);
}

void SetObjectPosition(GXOBJECT_HANDLE handle, float fX, float fY, float fZ)
{
	VECTOR3 v3;

	v3.x = fX;
	v3.y = fY;
	v3.z = fZ;

	GXSetPosition(handle, &v3, FALSE);
}

char* GetFile(BYTE bResType, DWORD dwResUID)
{
	SPLITRESUID nUID;
	nUID.uTypeID = bResType;
	nUID.uUniqID = dwResUID;

	return g_ResDefined.GetResFileNameOnly(*(DWORD*)&nUID);
}

char* FILEDEF(BYTE bResType, DWORD dwResUID)
{
	SPLITRESUID nUID;
	nUID.uTypeID = bResType;
	nUID.uUniqID = dwResUID;

	return g_ResDefined.GetResFileNameOnly(*(DWORD*)&nUID);
}

char* GetFile( char* szFileName, BYTE bDataType )
{
	memset(g_szTempFileName, 0, sizeof(g_szTempFileName));

	char szDataPath[_MAX_PATH]={0,};
	memset(szDataPath, 0, _MAX_PATH);

	__lstrcpyn(szDataPath, g_szDataPath, _MAX_PATH);

#ifdef __PACKAGE_MODE__
	
	char szDrive[_MAX_PATH]={0,};
	char szDir[_MAX_PATH]={0,};
	char szFName[_MAX_PATH]={0,};
	char szExt[_MAX_PATH]={0,};

	_splitpath(szFileName, szDrive, szDir, szFName, szExt);

	MakeLowerString(szExt, szExt);

	if (0 == __strcmp(szExt, ".cdb") ||
		0 == __strcmp(szExt, ".cdt") ||
		0 == __strcmp(szExt, ".wav") ||
		0 == __strcmp(szExt, ".txt") ||
		0 == __strcmp(szExt, ".cur") ||
		0 == __strcmp(szExt, ".map") ||
		0 == __strcmp(szExt, ".mp3") ||
		0 == __strcmp(szExt, ".ttb"))
	{
		__lstrcpyn(szDataPath, g_szDataPath, _MAX_PATH);
	}
	else
	{
		__lstrcpyn(szDataPath, g_szPackPath, _MAX_PATH);
	}

#endif

	switch((int)bDataType)
	{
		case DATA_TYPE_MAP:			wsprintf(g_szTempFileName, "%s\\Map\\",			szDataPath); break;
		case DATA_TYPE_CHARACTER:	wsprintf(g_szTempFileName, "%s\\Character\\",	szDataPath); break;
		case DATA_TYPE_WEAPON:		wsprintf(g_szTempFileName, "%s\\Weapon\\",		szDataPath); break;
		case DATA_TYPE_MONSTER:		wsprintf(g_szTempFileName, "%s\\Monster\\",		szDataPath); break;		
		case DATA_TYPE_EFFECT:		wsprintf(g_szTempFileName, "%s\\Effect\\",		szDataPath); break;
		case DATA_TYPE_UI:			wsprintf(g_szTempFileName, "%s\\UI\\",			szDataPath); break;
		case DATA_TYPE_SOUND:		wsprintf(g_szTempFileName, "%s\\Sound\\",		szDataPath); break;
		case DATA_TYPE_MANAGER:		wsprintf(g_szTempFileName, "%s\\Manager\\",		szDataPath); break;
		case DATA_TYPE_DEBUG:		wsprintf(g_szTempFileName, "%s\\Debug\\",		szDataPath); break;
		case DATA_TYPE_BUTTON:		wsprintf(g_szTempFileName, "%s\\Button\\",		szDataPath); break;
		case DATA_TYPE_NPC:			wsprintf(g_szTempFileName, "%s\\Npc\\",			szDataPath); break;
		case DATA_TYPE_ITEM:		wsprintf(g_szTempFileName, "%s\\Item\\",		szDataPath); break;
		case DATA_TYPE_DAMAGENUMBER:wsprintf(g_szTempFileName, "%s\\DamageNumber\\",szDataPath); break;
		case DATA_TYPE_CDT:			wsprintf(g_szTempFileName, "%s\\Cdt\\",			szDataPath); break;	 		
	}

	SetCurrentDirectory(g_szTempFileName);
	lstrcat(g_szTempFileName, szFileName);

	return g_szTempFileName;
}

void CreateConvertCDBToMAP(char* pszFileName)
{
	// cdb -> map 파일 생성.	
	// 미리 길이를 알 수 있는 방법이나 그런 게 없을까? 일단 100K로 세팅.
	char* szTempBuf = new char[102400];
	ZeroMemory(szTempBuf, 102400);

	DWORD dwTotalRead = DecodeCDBData( pszFileName, szTempBuf, DECODE_KEY, DECODE_SUBKEY);
	pszFileName[lstrlen(pszFileName)-3]='m';
	pszFileName[lstrlen(pszFileName)-2]='a';
	pszFileName[lstrlen(pszFileName)-1]='p';
	
	HANDLE hFile = CreateFile( pszFileName ,GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	if(hFile == INVALID_HANDLE_VALUE)
	{
#ifdef DEVELOP_MODE
		asm_int3();
		MessageBox(NULL, "걸렸당", "으악", MB_OK);
#endif
		return;
	}
	else
	{
		DWORD dwWrite;
		WriteFile(hFile, szTempBuf,dwTotalRead , &dwWrite,  NULL);
		CloseHandle(hFile);
	}

	delete [] szTempBuf;
}

void SendLoadingStatus(BYTE bIsLoading)
{
	CTWS_LOADING_STATUS	packet;
	packet.bIsLoading = bIsLoading;
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);	
}

void LoadWorldMap( DWORD dwMapResourceUID )	
{
	SendLoadingStatus(TRUE);

	// 월드맵 Resource 번호는 10000 + WorldMapID
	dwMapResourceUID += 10000;		

	char szFileName[0xff]={0,};
	memset(szFileName, 0, sizeof(szFileName));
	wsprintf(szFileName, "%d.cdb", dwMapResourceUID);
	lstrcpy(szFileName, GetFile(szFileName, DATA_TYPE_MAP));
	
	CreateConvertCDBToMAP(szFileName);
	LoadWorldMapScript(szFileName, 0);
	DeleteFile(szFileName);

	// 이부분은 추후에 CMap에 들어갈지도..
	// 이제 맵 속성 타일을 읽어드린다.
	HANDLE	hFile = NULL;
	DWORD	dwRead = 0,	dwTotalReadByte=0; 
	BOOL	bResult = 0;

	g_ItemMoveManager.Initialize();
	g_helpLoadingSprite.Load(dwMapResourceUID);
	g_helpLoadingSprite.SetLoadingFlag(TRUE);

	wsprintf(szFileName, "%d.ttb", dwMapResourceUID);
	lstrcpy(szFileName, GetFile(szFileName, DATA_TYPE_MAP));
	
	hFile = CreateFile( szFileName ,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		// MSG_ID : 279 ; Fail to open map attribute file!
		MessageBox(0,g_Message[ETC_MESSAGE279].szMessage,0,0);	
	}

	DWORD dwXLen, dwZLen, dwTileSize, dwObjNum;
	
	ReadFile(hFile, &dwXLen, sizeof(DWORD), &dwRead, NULL);
	ReadFile(hFile, &dwZLen, sizeof(DWORD), &dwRead, NULL);
	ReadFile(hFile, &dwTileSize, sizeof(DWORD), &dwRead, NULL);
	ReadFile(hFile, &dwObjNum, sizeof(DWORD), &dwRead, NULL);
	
	CMap* pMap = new CMap(dwXLen, dwZLen, (float)dwTileSize);	
		
	// 속성 정보 읽어드림 
	MAP_TILE *pTile=0;	int nZ=0, nX=0;
	for(DWORD i=0; i< (dwXLen * dwZLen); i++)
	{
		pTile	= pMap->m_pTile + i;
		bResult = ReadFile(hFile, &pTile->wAttr , 2, &dwRead, NULL);

		if( pTile->wAttr.uAttr == 1 )	//벽 
			pTile->wAttr.uOccupied = TILE_OCCUPIED_DONT_MOVE;
		
		if(!bResult)
		{
			// MSG_ID : 280 ; Fail to read!
			MessageBox(0, g_Message[ETC_MESSAGE280].szMessage, 0, 0);	
			break;
		}
		
		if(bResult && dwRead == 0 )	
		{ 
			// MSG_ID : 281 ; End to read!
			MessageBox(0, g_Message[ETC_MESSAGE281].szMessage, 0, 0);	
			break;
		}
		
		nX				= i % dwXLen;		
		nZ				= i / dwXLen;
		pTile->x		= (float)(nX * dwTileSize);
		pTile->z		= (float)(nZ * dwTileSize);
		pTile->wIndex_X	= WORD(nX);
		pTile->wIndex_Z = WORD(nZ);
	}
	
	// Read Section Information
	bResult = ReadFile(hFile, &pMap->m_wTotalSectionMany, sizeof(WORD), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(WORD))	asm_int3();
	
	for(i=0; i<pMap->m_wTotalSectionMany; i++)
	{
		dwTotalReadByte = 0;
								
		bResult = ReadFile(hFile, &pMap->m_Section[i], sizeof(Section_Link_Info), &dwRead, NULL);
		if(!bResult || dwRead != sizeof(Section_Link_Info))	asm_int3();
	}
	
	CloseHandle(hFile);
	
	g_pMap = pMap;

	SendLoadingStatus(FALSE);
}


DWORD __stdcall LoadingDungeonProc(DWORD dwCurCount, DWORD dwTotalCount, void* pArg)
{
	int x = (10 * (dwCurCount+1)) / dwTotalCount;

	for(int i=0; i<10; i++)
		SPR(SPR_LOADING_BLOCK_11 + i)->ShowSprite(FALSE);

	for(i=0; i<x; i++)
		SPR(SPR_LOADING_BLOCK_11 + i)->ShowSprite(TRUE);

	SPR(SPR_LOADING_BLOCK_HEAD)->vPos.x = (float)(484 + (9*x));
	SPR(SPR_LOADING_BLOCK_HEAD)->ShowSprite(TRUE);

	g_pGeometry->BeginRender(0, 0xff000000, BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER);
	g_pSprManager->RenderAllSprite();
	g_helpLoadingSprite.Show();
	g_pGeometry->EndRender();
	g_pGeometry->Present( g_hMainWnd );

	if(dwTotalCount == (dwCurCount+1))
	{
		for(int i=0; i<20; i++)
			SPR(SPR_LOADING_BLOCK_1 + i)->ShowSprite(FALSE);

		SPR(SPR_LOADING_BLOCK_HEAD)->ShowSprite(FALSE);
		SPR(SPR_LOADING_BACK)->ShowSprite(FALSE);
	}
	
	return 0l;
}

void InitMap( DWORD dwLayerID )
{
	char szMapModelFile[0xff]={0,};
	char szMapAttrFile[0xff]={0,};
	char szFileNameTTB[0xff]={0,};
	char szFileNameMAP[0xff]={0,};
	
	wsprintf(szFileNameTTB, "%d.ttb", dwLayerID);
	wsprintf(szFileNameMAP, "%d.map", dwLayerID);

	lstrcpy(szMapModelFile, GetFile(szFileNameMAP, DATA_TYPE_MAP));
	lstrcpy(szMapAttrFile, GetFile(szFileNameTTB, DATA_TYPE_MAP));
	
	g_pExecutive->DeleteAllGXMapObjectsWitLoadMapScript();
	g_pExecutive->DeleteGXMap( NULL );	

	if (g_pMap) 
	{
		delete g_pMap;
		g_pMap = NULL;		
	}
	
	if( !g_pExecutive->LoadMapScript( szMapModelFile, LoadingDungeonProc, LOAD_MAP_FLAG_DEFAULT_PROC_AUTOANIMATION ) ) 
		asm_int3();	
	
	// 이부분은 추후에 CMap에 들어갈지도..
	// 이제 맵 속성 타일을 읽어드린다.
	HANDLE	hFile = NULL;
	DWORD	dwRead = 0,	dwTotalReadByte=0; 
	BOOL	bResult = 0;

	hFile = CreateFile( szMapAttrFile ,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE)
		MessageBox(0,g_Message[ETC_MESSAGE279].szMessage,0,0);	// MSG_ID : 279 ; Fail to open map attribute file!

	DWORD dwXLen, dwZLen, dwTileSize, dwObjNum;
	
	ReadFile(hFile, &dwXLen, sizeof(DWORD), &dwRead, NULL);
	ReadFile(hFile, &dwZLen, sizeof(DWORD), &dwRead, NULL);
	ReadFile(hFile, &dwTileSize, sizeof(DWORD), &dwRead, NULL);
	ReadFile(hFile, &dwObjNum, sizeof(DWORD), &dwRead, NULL);

	CMap* pMap = new CMap(dwXLen, dwZLen, TILE_SIZE);	
		
	//속성 정보 읽어드림 
	MAP_TILE *pTile = 0;	int nZ=0, nX=0;
	for(DWORD i=0; i< (dwXLen * dwZLen); i++)
	{
		pTile	= pMap->m_pTile + i;
		bResult = ReadFile(hFile, &pTile->wAttr , 2, &dwRead, NULL);

		if( pTile->wAttr.uAttr == 1 )	//벽 
			pTile->wAttr.uOccupied = TILE_OCCUPIED_DONT_MOVE;
		
		if(!bResult)
		{
			MessageBox(0, g_Message[ETC_MESSAGE280].szMessage, 0, 0);	// MSG_ID : 280 ; Fail to read!
			break;
		}
		
		if(bResult && dwRead == 0 )	
		{ 
			MessageBox(0, g_Message[ETC_MESSAGE281].szMessage, 0, 0);	// MSG_ID : 281 ; End of file!
			break;
		}
		
		nX				= i % dwXLen;		
		nZ				= i / dwXLen;
		pTile->x		= nX * TILE_SIZE;
		pTile->z		= nZ * TILE_SIZE;
		pTile->wIndex_X	= WORD(nX);
		pTile->wIndex_Z = WORD(nZ);
		
		if( pTile->wAttr.uAttr == 9 )	//cp
		{
			VECTOR3 vec;
			vec.x = pTile->x+TILE_SIZE/2;
			vec.z = pTile->z+TILE_SIZE/2;
			vec.y = 0;			
			
			EffectDesc*	pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_CP_POSITION), 0xff, __CHR_EFFECT_NONE__);
			pEffectDesc->vecBasePosion = vec;
			GXSetPosition( pEffectDesc->hEffect.pHandle, &vec, FALSE );
			::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
			HideObject(pEffectDesc->hEffect.pHandle);

			// cp 정보담는 구조체생성.
			CP_DESC* pCPDesc = new CP_DESC;
			memset(pCPDesc, 0, sizeof(pCPDesc));

			pCPDesc->pTile = pTile;
			pCPDesc->pEffectDesc = pEffectDesc;
			pMap->m_pCPList->AddTail(pCPDesc);
			pCPDesc->bID = BYTE(pMap->m_pCPList->GetCount());
			pCPDesc->pSpr_Cp_GuageBar = g_pSprManager->CreateSprite(SPR_CP_GUAGEBAR, 0, 0, 160, 33, 2, 4, FALSE, 2);
			pCPDesc->pSpr_Cp_GuageBoard = g_pSprManager->CreateSprite(SPR_CP_GUAGEBOARD, 0, 0, 1, 179, 128, 6, FALSE, 1);
			pCPDesc->pSpr_Minimap_Dot = g_pSprManager->CreateSprite(SPR_CP_MINIMAPDOT1+pCPDesc->bID-1, 0, 0, 164, 33, 4, 4, FALSE, 5000);			
		}
	}

	pMap->m_bCPRemainCount = 0;

	//Read Section Information
	bResult = ReadFile(hFile, &pMap->m_wTotalSectionMany, sizeof(WORD), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(WORD))	asm_int3();
	
	for(i = 0; i < pMap->m_wTotalSectionMany; i++)
	{
		dwTotalReadByte = 0;
								
		bResult = ReadFile(hFile, &pMap->m_Section[i], sizeof(Section_Link_Info), &dwRead, NULL);
		if(!bResult || dwRead != sizeof(Section_Link_Info))	asm_int3();
	}

	WORD	wTotalMonsertInfoNum = 0;
	bResult = ReadFile(hFile, &wTotalMonsertInfoNum, sizeof(wTotalMonsertInfoNum), &dwRead, NULL);
	if( wTotalMonsertInfoNum != 0 )
	{
		SetFilePointer (hFile, 6 * wTotalMonsertInfoNum, NULL, FILE_CURRENT);		
	}

	// 타일 위치에 따른 섹션 정보를 기록해둔다.
	DWORD x, z, s;
	DWORD m_dwTileNumHeight	= pMap->m_dwMapZTileMany;
	DWORD m_dwTileNumWidth	= pMap->m_dwMapXTileMany;
	
	for( z = 0; z < m_dwTileNumHeight; z++ )
	{
		for( x = 0; x < m_dwTileNumWidth; x++ )
		{
			pMap->m_pTile[m_dwTileNumWidth * z + x].wIndex_X = WORD(x);
			pMap->m_pTile[m_dwTileNumWidth * z + x].wIndex_Z = WORD(z);
			pMap->m_pTile[m_dwTileNumWidth * z + x].x = WORD(x * TILE_WIDTH);
			pMap->m_pTile[m_dwTileNumWidth * z + x].z = WORD(z * TILE_HEIGHT);
			
			for( s=0; s<pMap->m_wTotalSectionMany; s++ )
			{
				if( pMap->m_pTile[m_dwTileNumWidth * z + x].wIndex_X	>= pMap->m_Section[s].x1 &&
					pMap->m_pTile[m_dwTileNumWidth * z + x].wIndex_X	<= pMap->m_Section[s].x2 &&
					pMap->m_pTile[m_dwTileNumWidth * z + x].wIndex_Z	>= pMap->m_Section[s].y1 &&
					pMap->m_pTile[m_dwTileNumWidth * z + x].wIndex_Z	<= pMap->m_Section[s].y2 )
				{
					pMap->m_pTile[m_dwTileNumWidth * z + x].wAttr.uSection = pMap->m_Section[s].wSectionNum;
				}
			}
		}
	}

	//------------------------------------	Move Spot 읽어오기 -----------------------------------------------

	// ttb로 부터 MoveSpot 갯수 읽어오기 
	WORD	wSpotNum = 0;
	WORD	wID = 0;
	bResult = ReadFile(hFile, &wSpotNum, sizeof(WORD), &dwRead, NULL);
	if(!bResult || dwRead > sizeof(WORD)*MAX_MOVE_SPOT_NUM)	asm_int3();
	
	for(i=0; i<wSpotNum; i++)
	{
		bResult = ReadFile(hFile, &wID, sizeof(WORD), &dwRead, NULL);
		
//		if(!bResult || wID > MAX_MOVE_SPOT_NUM || wID == 0)
//			asm_int3();

		Move_Spot MoveSpot;	
		bResult = ReadFile(hFile, &MoveSpot.rcMoveSpot, sizeof(RECT), &dwRead, NULL);	
		
		int nStartX = min(MoveSpot.rcMoveSpot.right, MoveSpot.rcMoveSpot.left);
		int nStartZ = min(MoveSpot.rcMoveSpot.bottom, MoveSpot.rcMoveSpot.top);
		int nEndX = max(MoveSpot.rcMoveSpot.right, MoveSpot.rcMoveSpot.left);
		int nEndZ = max(MoveSpot.rcMoveSpot.bottom, MoveSpot.rcMoveSpot.top);
		for(int z = nStartZ; z <= nEndZ; z++)
		{
			for(int x = nStartX; x <= nEndX; x++)
			{
				MAP_TILE* pTile = pMap->GetMap(x, z);			
				if (pTile && pTile->wAttr.uOccupied == TILE_EMPTY)
				{
					VECTOR3 vec;
					vec.x = pTile->x;
					vec.z = pTile->z;
					vec.y = 0;					
					
					EffectDesc*	pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_TELEPORT_POSITION), 0xff, __CHR_EFFECT_NONE__);
					
					GXSetPosition( pEffectDesc->hEffect.pHandle, &vec, FALSE );
					MOTION_DESC		MotionDesc;
					g_pExecutive->GXOGetMotionDesc(pEffectDesc->hEffect.pHandle,&MotionDesc,1,0);
					::SetAction( pEffectDesc->hEffect.pHandle, 1, rand()%MotionDesc.dwLastFrame , ACTION_LOOP );					
				}
			}
		}

		if(!bResult)
			asm_int3();
	}
									
	CloseHandle(hFile);

	if(pMap->m_fMapXLength>pMap->m_fMapZLength)	
		g_pGeometry->SetRadiusForLoadResource(pMap->m_fMapXLength);
	else
		g_pGeometry->SetRadiusForLoadResource(pMap->m_fMapZLength);	
	
	g_pMap = pMap;
	
	GXOBJECT_HANDLE gxo[200];
	DWORD dwCount = g_pExecutive->GXOGetAllObjectsWitLoadMapScript(gxo, 200);
	for( i = 0; i < dwCount; ++i)
	{
		g_pExecutive->DisablePick(gxo[i]);
		g_pExecutive->DisableSendShadow(gxo[i]);
	}
}

void SetProgressBar(DWORD dwTotalProcess, DWORD dwCurProcess)
{
	int x = (20 * dwCurProcess) / dwTotalProcess;

	for(int i=0; i<20; i++)
		SPR(SPR_LOADING_BLOCK_1 + i)->ShowSprite(FALSE);

	for(i=0; i<x; i++)
		SPR(SPR_LOADING_BLOCK_1 + i)->ShowSprite(TRUE);

	SPR(SPR_LOADING_BLOCK_HEAD)->vPos.x = (float)(394 + (9*x));
	SPR(SPR_LOADING_BLOCK_HEAD)->ShowSprite(TRUE);

	g_pGeometry->BeginRender(0, 0xff000000, BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER);
	g_pSprManager->RenderAllSprite();
	g_helpLoadingSprite.Show();
	g_pGeometry->EndRender();
	g_pGeometry->Present( g_hMainWnd );

	if(dwTotalProcess == dwCurProcess)
	{
		for(int i=0; i<20; i++)
			SPR(SPR_LOADING_BLOCK_1 + i)->ShowSprite(FALSE);

		SPR(SPR_LOADING_BLOCK_HEAD)->ShowSprite(FALSE);
	}
}

void ShowLoading()
{
	SPR(SPR_LOADING_BACK)->ShowSprite(TRUE);

	g_pGeometry->BeginRender(0, 0xff000000, BEGIN_RENDER_FLAG_DONOT_CLEAR_FRAMEBUFFER);
	g_pSprManager->RenderAllSprite();
	g_helpLoadingSprite.Show();
	g_pGeometry->EndRender();
	g_pGeometry->Present( g_hMainWnd );

	g_helpLoadingSprite.Show();
}


void LoadTotalDungeonInfo()
{
	char szBuf[ 0xff ]={0,};
	char buf[0xff]={0,};
	GetCurrentDirectory(0xff, szBuf);

	char szFileName[0xff]={0,};
	char* p = GetFile("TotalMap.cdb", DATA_TYPE_MAP);

	CreateConvertCDBToMAP(p);
	
	FILE*	fp;
	fp = fopen(p, "rt");
	
	while (fscanf(fp,"%s",buf)!=EOF)
	{
		//-- World Map Dungeon 정보 읽어 드림 
		if (!__strcmp(buf,"WORLD_DUNGEON_INFO"))
		{
			
			DWORD	dwDungeonNum;
			DWORD	dwID;
			DWORD	dwWorldMapID;
			char	szDungeonName[ MAX_DUNGEON_NAME_LENGTH ]={0,};
			
			fscanf(fp,"%u",&dwDungeonNum);
			fscanf(fp,"%s",buf);		
			for (DWORD i=0; i<dwDungeonNum; i++)
			{	
				fscanf(fp,"%u",&dwID);
				fscanf(fp,"%s",szDungeonName);
				fscanf(fp,"%u", &dwWorldMapID);

				DUNGEON_DATA_EX* pInfo = g_pDungeonTable->GetDungeonInfo(dwID);
				if (!pInfo)	
				{
					pInfo = g_pDungeonTable->AllocDungeonInfo();
					pInfo->m_dwID = dwID;
					pInfo->m_wWorldMapID = (BYTE)dwWorldMapID;
					g_pDungeonTable->AddDungeonInfo(pInfo);
				}
				
				memset(pInfo->m_szDungeonName, 0, sizeof(pInfo->m_szDungeonName));
				__lstrcpyn(pInfo->m_szDungeonName, szDungeonName, sizeof(szDungeonName));
			}
		}
	} 

	fclose(fp);
	DeleteFile(szFileName);
}


BOOL LoadWorldMapScript(char* szMapFile, DWORD dwFlag)	
{
	g_pExecutive->DeleteAllGXMapObjectsWitLoadMapScript();
	g_pExecutive->DeleteGXMap( NULL );

	char	buf[128]={0,};
	g_pExecutive->CreateGXMap(NULL,0,NULL);	

	IHeightField*	pHField = NULL;
	I3DStaticModel*	pStaticModel = NULL;
	DWORD	dwTotalProgress = 4;
	DWORD	dwCurProgress = 0;
	
	FILE*	fp;
	fp = fopen(szMapFile, "rb");
	srand(NULL);
	g_helpLoadingSprite.SetLoadingFlag(TRUE);
	
	while (fscanf(fp,"%s",buf)!=EOF)
	{
		if (!__strcmp(buf,PID_GX_MAP))
		{
			while (__strcmp(buf,"}")) 
			{
				fscanf(fp,"%s",buf);
				if (!__strcmp(buf,PID_STATIC_MODEL))  
				{
					fscanf(fp,"%s",buf);
					
					g_pGeometry->CreateStaticModel(&pStaticModel,4000,500,0);	
								
					if (!pStaticModel->ReadFile(buf, NULL, 0))
					{
						pStaticModel->Release();
						pStaticModel = NULL;
					}

					SetProgressBar(dwTotalProgress,++dwCurProgress);	//1
				}
			
				if (!__strcmp(buf,PID_HEIGHT_FIELD))
				{
					fscanf(fp,"%s",buf);
					g_pGeometry->CreateHeightField(&pHField,0);

					if (!pHField->ReadFile(buf, 0, NULL, 0))
					{
						pHField->Release();
						pHField = NULL;
					}

					SetProgressBar(dwTotalProgress,++dwCurProgress);	//2
				}			
			}

			if (pHField)
				g_pExecutive->InsertHFieldToGXMap(pHField);

			if (pStaticModel)
				g_pExecutive->InsertStaticModelTOGXMap(pStaticModel);

			g_pExecutive->EndBuildMap(3200.0f, 0.0f);

			if (pStaticModel)
			{
				if (dwFlag & STATIC_MODEL_LOAD_OPTIMIZE)
					pStaticModel->Optimize();
					pStaticModel->Release();
			}
			if (pHField)
			{
				pHField->Release();
			}
		}

		if (!__strcmp(buf,PID_GX_OBJECT))
		{
			DWORD	dwGXObjectsNum;
			fscanf(fp,"%u",&dwGXObjectsNum);			

			fscanf(fp,"%s",buf);		// {
			for (DWORD i=0; i<dwGXObjectsNum; i++)
			{
				DWORD		dwFlag;
				DWORD		dwID;
				VECTOR3		v3Scale,v3Pos,v3Axis;
				float		fRad;

				// | 파일명 | 고유ID | 스케일 | 위치 | 축벡터 | 방향각 | 플래그 
				fscanf(fp,"%s",buf);		// 파일명
				fscanf(fp,"%u",&dwID);		// 고유ID
				fscanf(fp,"%f %f %f",&v3Scale.x,&v3Scale.y,&v3Scale.z);
				fscanf(fp,"%f %f %f",&v3Pos.x,&v3Pos.y,&v3Pos.z);				
				
				DUNGEON_DATA_EX* pDungeon;
				if(dwID < 10000)
				{
					pDungeon = g_pDungeonTable->GetDungeonInfo(dwID);
					
					if(!pDungeon)
						asm_int3();

					pDungeon->vPos = v3Pos;
				}
				
				fscanf(fp,"%f %f %f %f",&v3Axis.x,&v3Axis.y,&v3Axis.z,&fRad);
				fscanf(fp,"%x",&dwFlag);
				

				g_pGVWorld->hMapObjHandle[ g_pGVWorld->wMapObjectNum ] = CreateHandleObject(GetFile(buf, DATA_TYPE_MAP),NULL,NULL,GXOBJECT_CREATE_TYPE_DEFAULT_PROC);
				g_pExecutive->DisableSendShadow(g_pGVWorld->hMapObjHandle[ g_pGVWorld->wMapObjectNum ]);
				
				
				g_pExecutive->SetID(g_pGVWorld->hMapObjHandle[ g_pGVWorld->wMapObjectNum ],dwID);
				//물은 Picking 안되게 
				if(dwID == WORLD_OBJECT_WATER)
					g_pExecutive->DisablePick(g_pGVWorld->hMapObjHandle[ g_pGVWorld->wMapObjectNum ]);

				// 일단 스케일은 무시..
				GXSetPosition(g_pGVWorld->hMapObjHandle[ g_pGVWorld->wMapObjectNum ],&v3Pos, FALSE);
				g_pExecutive->GXOSetDirection(g_pGVWorld->hMapObjHandle[ g_pGVWorld->wMapObjectNum ],&v3Axis,fRad);
				SetBoundingVolume(g_pGVWorld->hMapObjHandle[ g_pGVWorld->wMapObjectNum ], 40.0f);

				g_pGVWorld->wMapObjectNum++;
			}

			fscanf(fp,"%s",buf);		

			SetProgressBar(dwTotalProgress,++dwCurProgress);	//3
		}

		if (!__strcmp(buf,PID_GX_LIGHT))
		{
			DWORD	dwGXLightsNum;
			fscanf(fp,"%u",&dwGXLightsNum);
		
			fscanf(fp,"%s",buf);		
			for (DWORD i=0; i<dwGXLightsNum; i++)
			{				
				LIGHT_DESC	lightDesc;
				memset(&lightDesc,0,sizeof(LIGHT_DESC));
				DWORD		dwFlag;

				// | RGBA | 위치 |범위(CM) | 플래그 
				fscanf(fp,"%x",&lightDesc.dwDiffuse);
				fscanf(fp,"%f %f %f %f",&lightDesc.v3Point.x,&lightDesc.v3Point.y,&lightDesc.v3Point.z,&lightDesc.fRs);
				fscanf(fp,"%x",&dwFlag);
				dwFlag = GXLIGHT_TYPE_STATIC;

				GXLIGHT_HANDLE	gxl = g_pExecutive->CreateGXLight(&lightDesc,NULL,NULL,0xffffffff, NULL, dwFlag);
				g_pExecutive->GXLEnableDynamicLight(gxl);
			}

			fscanf(fp,"%s",buf);		

			SetProgressBar(dwTotalProgress,++dwCurProgress);	//4
		}
				
		if (!__strcmp(buf,PID_GX_TRIGGER))
		{
			DWORD	dwGXTriggerNum;
			fscanf(fp,"%u",&dwGXTriggerNum);
		
			fscanf(fp,"%s",buf);	

			for (DWORD i=0; i<dwGXTriggerNum; i++)
			{
				EVENT_TRIGGER_DESC	triggerDesc;
				memset(&triggerDesc,0,sizeof(triggerDesc));
				DWORD		dwFlag;
				DWORD		dwID;
					
				// 고유아이디 | RGBA | 위치 | 스케일 | 회전 | 플래그 
				fscanf(fp,"%u",&dwID);
				fscanf(fp,"%x",&triggerDesc.dwColor);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Pos.x,&triggerDesc.v3Pos.y,&triggerDesc.v3Pos.z);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Scale.x,&triggerDesc.v3Scale.y,&triggerDesc.v3Scale.z);
				fscanf(fp,"%f %f %f",&triggerDesc.v3Rot.x,&triggerDesc.v3Rot.y,&triggerDesc.v3Rot.z);
				fscanf(fp,"%x",&dwFlag);

				GXTRIGGER_HANDLE	gxt = g_pExecutive->CreateGXEventTrigger(NULL,NULL,dwFlag);
				g_pExecutive->SetID(gxt,dwID);
				g_pExecutive->GXTSetPosition(gxt,&triggerDesc.v3Pos);
				g_pExecutive->GXTSetScale(gxt,&triggerDesc.v3Scale);
				g_pExecutive->GXTSetRotation(gxt,&triggerDesc.v3Rot);
			}

			fscanf(fp,"%s",buf);		
		}			
	} 

	fclose(fp);
	return TRUE;
}

void SetBoundingVolume(GXOBJECT_HANDLE handle, float fRadius)
{
	BOUNDING_VOLUME bound;
	memset(&bound,0, sizeof(BOUNDING_VOLUME));
	bound.dwType	= BOUNDING_VOLUME_TYPE_SPHERE;		
	bound.fRadius	= fRadius;							
	bound.dwCollisionTargetFlag	=	 COLLISION_TARGET_FLAG_TEST_DYNAMIC_OBJECT;
	g_pExecutive->GXOSetBoundingVolume( handle, &bound );
}

void ReleaseBoundingVolume(GXOBJECT_HANDLE handle)
{
	BOUNDING_VOLUME bound;
	memset(&bound,0, sizeof(BOUNDING_VOLUME));
	bound.dwType	= BOUNDING_VOLUME_TYPE_NONE;		//바운딩 타입 
	bound.fRadius	= 0.0f;								//반지름 
	bound.dwCollisionTargetFlag	=	 COLLISION_TARGET_FLAG_TEST_NONE;
	g_pExecutive->GXOSetBoundingVolume( handle, &bound );
}

BOOL EpsilonVector(VECTOR3*	v3Vec1, VECTOR3* v3Vec2, float fEpsilon )
{
	float	fDist = CalcDistance( v3Vec1, v3Vec2 );
	return ( fDist < fEpsilon ) ? TRUE : FALSE;
}

BOOL ChangeAddressType(DWORD dwIP, char* OUT szAddress)
{
	if(!szAddress)	return FALSE;

	wsprintf(szAddress, "%d.%d.%d.%d", *((BYTE*)&dwIP), *((BYTE*)&dwIP+1), *((BYTE*)&dwIP+2), *((BYTE*)&dwIP+3));
	return TRUE;
}

void OnIMEComposition(BOOL bResultStr)	
{
	if(g_nChatModeExt==0)
		return;	

	// Composition Box 화면에 안나오게 처리..
	COMPOSITIONFORM cf;		
	cf.dwStyle = CFS_POINT|CFS_FORCE_POSITION;
	cf.ptCurrentPos.x = -100;
	cf.ptCurrentPos.y = -100;
	ImmSetCompositionWindow(g_hImc, &cf);	
	
	DWORD dwInputIndex = g_pInputManager->GetInputBufferLength(g_pInputManager->GetCurFocusInputID());
	char szBuf[4] = {0};

	memset(szBuf, 0, sizeof(szBuf));

	switch(g_pInputManager->GetCurFocusInputID())
	{
	case INPUT_BUFFER_19:
		if(GetGameStatus()==UPDATE_GAME_CHAR_SELECT)
		{
			if(dwInputIndex>=MAX_CHARACTER_NAME_REAL_LENGTH - 1)
				return;
		}
		else
		{
			if(dwInputIndex>=__MAX_BUUFER_SIZE__)
				return;
		}		
		break;
	case INPUT_BUFFER_6:
		if(dwInputIndex>=__MAX_IDBUUFER_SIZE__)
			return;
		break;
	case INPUT_BUFFER_7:
		if(dwInputIndex>=MAX_GUILD_NAME_REAL_LENGTH-1)
			return;
		break;
	case INPUT_BUFFER_8:
	case INPUT_BUFFER_9:
	case INPUT_BUFFER_10:
	case INPUT_BUFFER_11:
		if(dwInputIndex>=__MAX_RANK_SIZE__)
			return;
		break;
	case INPUT_BUFFER_12:
		if(dwInputIndex>=__MAX_IDBUUFER_SIZE__)
			return;
		break;
	case INPUT_BUFFER_2:
		if(dwInputIndex>=__MAX_PLAYERSHOP_MEMO__-1)
			return;
	case INPUT_BUFFER_20:
		if(GetGameStatus()==UPDATE_GAME_PLAY)
		{
			if(dwInputIndex>=MAX_PARTYENTRY_TEXT-1)
				return;
		}
		break;
	}

	//ID, Password 길이 체크 
	if(GetGameStatus()== UPDATE_GAME_LOGIN)
	{
		if(dwInputIndex >= MAX_ID_REAL_LENGTH)	//ID와 Password 길이제한이 같다 
			return;
	}

	g_pInputManager->GetInputBuffer(g_pInputManager->GetCurFocusInputID());
	if(bResultStr)
	{		
		ImmGetCompositionString(g_hImc, GCS_RESULTSTR ,szBuf, 2);										

		if(szBuf[0]==-95 && szBuf[1]==-95)
			g_pInputManager->SetCharacter(g_pInputManager->GetCurFocusInputID(), 2);
		else
			g_pInputManager->InsertCharacter(g_pInputManager->GetCurFocusInputID(), szBuf, 2);
	}
	else
	{
		ImmGetCompositionString(g_hImc, GCS_COMPSTR ,szBuf, 2);		
		
		memcpy(g_pInputManager->GetInputBuffer(g_pInputManager->GetCurFocusInputID())+g_pInputManager->GetInputBufferLength(g_pInputManager->GetCurFocusInputID()), szBuf, 2);
	
		// 한글 입력하는 도중에 백스페이스로 다시 지웠다.
		if(!lstrlen(szBuf))
			::SendMessage(g_hMainWnd, WM_KEYDOWN, VK_BACK, 0);
	}
}

void SetCameraTarget(GXOBJECT_HANDLE handle)
{
	if (NULL == handle)
	{
		_asm int 3
	}

	VECTOR3 vAfterPos;
	g_pExecutive->GXOGetPosition(handle, &vAfterPos);

	vAfterPos.x = vAfterPos.x;							
	vAfterPos.y = g_Camera.fCameraDistance_y;							
	vAfterPos.z = vAfterPos.z + (g_Camera.fCameraDistance_z - 400.0f);
	
	g_pGeometry->SetCameraPos(&vAfterPos, 0);
}

void SetCameraTargetWithHeight(GXOBJECT_HANDLE handle)
{
	VECTOR3 vAfterPos;
	g_pExecutive->GXOGetPosition(handle, &vAfterPos);

	vAfterPos.x = vAfterPos.x;							
	vAfterPos.y = vAfterPos.y + g_Camera.fCameraDistance_y;							
	vAfterPos.z = vAfterPos.z + (g_Camera.fCameraDistance_z - 400.0f);
	
	g_pGeometry->SetCameraPos(&vAfterPos, 0);
}

void SetCameraYaw( VECTOR3* pCameraTo, float fYew )		
{
	CAMERA_DESC	CameraDesc;
	VECTOR3	v3NewPos, v3CameraAngleRad, v3Rotate, v3Dir;

	g_pGeometry->GetCameraDesc( &CameraDesc, 0 );
	g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );

	RotatePositionWithPivot( &v3NewPos, pCameraTo, &CameraDesc.v3From, &v3CameraAngleRad, fYew, 0.0f );
	v3Rotate.x = 0.0f; v3Rotate.y = fYew; v3Rotate.z = 0.0f;
	g_pGeometry->RotateCameraRad(&v3Rotate, 0 );
	g_pGeometry->SetCameraPos( &v3NewPos, 0 );

	VECTOR3_SUB_VECTOR3( &v3Dir, &v3NewPos, pCameraTo );
	Normalize( &v3Dir, &v3Dir );
	
	float fDis = CalcDistance( &v3NewPos, pCameraTo );

	g_Camera.fCameraDistance_x = ( v3Dir.x * fDis );
	g_Camera.fCameraDistance_y = ( v3Dir.y * fDis  );
	g_Camera.fCameraDistance_z = ( v3Dir.z * fDis  );
}

void SetCameraPitch( VECTOR3* pCameraTo, float fPitch )
{
	CAMERA_DESC CameraDesc;
	VECTOR3 v3NewPos, v3CameraAngleRad, v3Rotate, v3Dir;

	g_pGeometry->GetCameraDesc( &CameraDesc, 0 );
	g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );

	RotatePositionWithPivot( &v3NewPos, pCameraTo, &CameraDesc.v3From, &v3CameraAngleRad, 0.0f, fPitch );
	v3Rotate.x = fPitch; v3Rotate.y = 0.0f; v3Rotate.z = 0.0f;
	g_pGeometry->RotateCameraRad(&v3Rotate, 0 );
	g_pGeometry->SetCameraPos( &v3NewPos, 0 );

	VECTOR3_SUB_VECTOR3( &v3Dir, &v3NewPos, pCameraTo );
	Normalize( &v3Dir, &v3Dir );
	
	float fDis = CalcDistance( &v3NewPos, pCameraTo );

	g_Camera.fCameraDistance_x = ( v3Dir.x * fDis );
	g_Camera.fCameraDistance_y = ( v3Dir.y * fDis  );
	g_Camera.fCameraDistance_z = ( v3Dir.z * fDis  );
}

void SetCameraPos(VECTOR3* pCameraTo, float wLength )
{
	CAMERA_DESC	CameraDesc;
	VECTOR3	v3CameraForm, v3Dir;

	g_pGeometry->GetCameraDesc( &CameraDesc, 0 );

	VECTOR3_SUB_VECTOR3( &v3Dir, pCameraTo, &CameraDesc.v3From );
	Normalize( &v3Dir, &v3Dir );

	v3CameraForm.x = CameraDesc.v3From.x + ( v3Dir.x * wLength );
	v3CameraForm.y = CameraDesc.v3From.y + ( v3Dir.y * wLength );
	v3CameraForm.z = CameraDesc.v3From.z + ( v3Dir.z * wLength );
	
	VECTOR3_SUB_VECTOR3( &v3Dir, &v3CameraForm, pCameraTo );
	Normalize( &v3Dir, &v3Dir );
	
	float fDis = CalcDistance( &v3CameraForm, pCameraTo );

	g_Camera.fCameraDistance_x = ( v3Dir.x * fDis );
	g_Camera.fCameraDistance_y = ( v3Dir.y * fDis  );
	g_Camera.fCameraDistance_z = ( v3Dir.z * fDis  );

	g_pGeometry->SetCameraPos( &v3CameraForm, 0 );
}

DWORD GetRandom( DWORD dwMax )
{
	DWORD dwRnd = rand();
	dwRnd = dwRnd%dwMax;

	return dwRnd;
}

int FillSelectedMonster( DWORD* pdwGuardian, DWORD* pdwMonster )
{
	memset( pdwGuardian, 0, sizeof( DWORD ) * MAX_USER_GUARDIAN );
	memset( pdwMonster, 0, sizeof( DWORD ) * MAX_USER_GUARDIAN );

	int i, cnt, total;
	total = 0;

	// Guardian...
	cnt = 0;
	for( i=0; i<MAX_USER_GUARDIAN; i++ )
	{
		if( g_pMainPlayer->m_pGuardian[i] )
		{
			if( g_pMainPlayer->m_pGuardian[i]->m_dwTemp[ MONSTER_TEMP_SELECTED ] )
			{
				pdwGuardian[cnt] = g_pMainPlayer->m_pGuardian[i]->m_dwMonsterIndex;
				cnt++;
				total++;
			}
		}
	}

	// Monster...
	cnt = 0;
	for( i=0; i<MAX_USER_GUARDIAN; i++ )
	{
		if( g_pMainPlayer->m_pMonster[i] )
		{
			if( g_pMainPlayer->m_pMonster[i]->m_dwTemp[ MONSTER_TEMP_SELECTED ] )
			{
				pdwMonster[cnt] = g_pMainPlayer->m_pMonster[i]->m_dwMonsterIndex;
				cnt++;
				total++;
			}
		}
	}

	return total;
}

BOOL InitChrInfo( char* szFileName, DWORD dwKind, DWORD dwFlag )
{
	FILE*	fp;
	ChrInfoLayer*	pChrLayerInfo = NULL;

	fp = fopen( szFileName, "rb" );

	if( !fp )
	{
		return FALSE;
	}
	
	switch(dwFlag)
	{
	case OBJECT_TYPE_PLAYER:
		{
			if( dwKind >= MAX_CHAR_KIND )
			{
				fclose(fp);
				return FALSE;
			}
			pChrLayerInfo = g_pChrInfoUser;
		}break;
	case OBJECT_TYPE_MONSTER:
		{
			if( dwKind >= MAX_MONSTER_KIND )
			{
				fclose(fp);
				return FALSE;
			}
			pChrLayerInfo = g_pChrInfoMonster;
		
		}break;
	case OBJECT_TYPE_EFFECT:
		{
		
			if( dwKind >= MAX_EFFECT_KIND )
			{
				fclose(fp);
				return FALSE;
			}
			pChrLayerInfo = g_pChrInfoEffect;
		}break;
	
	default:
		{
			fclose(fp);
			return FALSE;
		}
	}

	if (pChrLayerInfo->m_pChrInfo[dwKind])
	{
		fclose(fp);
		return FALSE;
	}
	
	DWORD dwAniMany, dwMotionMany;
	fread( &dwAniMany, sizeof( DWORD ), 1, fp );
	fread( &dwMotionMany, sizeof( DWORD ), 1, fp );
	pChrLayerInfo->m_pChrInfo[dwKind] = new ChrInfo[dwAniMany*dwMotionMany];

	//Effect Frame
	for(DWORD i=0; i<dwAniMany; i++ )
	{
		for(DWORD j=0; j<dwMotionMany; j++ )
		{
			fread( &pChrLayerInfo->m_pChrInfo[dwKind][i*dwMotionMany+j], sizeof(ChrInfo), 1, fp);
		}
	}
	
	fclose( fp );
	return TRUE;
}

BOOL InitSearchModule(CMap* pMap)
{
	if(!pMap)
		return FALSE;

	if(g_pSw)
		delete g_pSw;
	
	g_pSw = new Sw;		//	인스턴스 생성
	g_pSw->Initialize( 20, 20, pMap->m_dwMapXTileMany, pMap->m_dwMapZTileMany
		, sizeof(MAP_TILE), (PVOID)&( pMap->GetTile(0, 0)->wAttr ) );
	memset( &g_PathFinder, 0, sizeof( A_STAR ) );

	return TRUE;
}

void InitCamera()
{
	VECTOR3 From;
	From.x = g_Camera.fCameraDistance_x = 0.0f;
	From.y = g_Camera.fCameraDistance_y = 1000.0f;
	From.z = g_Camera.fCameraDistance_z = -1000.0f;
	
	g_pGeometry->ResetCamera( &From, 100.0f,80000.0f, DEG45, 0 );	
		
	//카메라를 x축을 기준으로 약간 회전시킨다. 카메라가 위에서 아래로 쳐다보게 한다.
	VECTOR3		Rotate;
	Rotate.x = -1.0f * DEG45;	Rotate.y = 0.0f;	Rotate.z = 0.0f;
	g_pGeometry->RotateCameraRad( &Rotate,0 );

	g_Camera.v3AxsiY.y = 1.0f;		
}

void InitDungeonCamera()
{
	VECTOR3 From;
	From.x = g_Camera.fCameraDistance_x = 0.0f;
	From.y = g_Camera.fCameraDistance_y = 1000.0f;
	From.z = g_Camera.fCameraDistance_z = -1000.0f;
	
	g_pGeometry->ResetCamera( &From,
							  DUNGEON_CAMERA_NEAR,
							  DUNGEON_CAMERA_FAR,
							  DEG45, 0 );	
		
	// 카메라를 x축을 기준으로 약간 회전시킨다. 카메라가 위에서 아래로 쳐다보게 한다.
	VECTOR3		Rotate;
	Rotate.x = -1.0f * DEG45;	Rotate.y = 0.0f;	Rotate.z = 0.0f;
	g_pGeometry->RotateCameraRad( &Rotate,0 );

	g_Camera.v3AxsiY.y = 1.0f;
}

void TerminateDungeonCamera()
{
	// 던전에서 줄여뒀던 카메라 시야범위를 원래대로 돌려놓는다.
	// 월드에서 사용하는 하잇필드의 메모리를 확보할 수 있게 한다. 	
	g_pGeometry->SetCameraViewDistance(WORLD_CAMERA_NEAR,WORLD_CAMERA_FAR,0);
}

void UpdateMainPlayerToMap()
{
	
}

void FreeUser( CUser* pUser )
{
	if( !pUser ) asm_int3();

	DeleteHandleObject( pUser->m_hPlayer.pHandle );
	FreeObjDesc( pUser->m_hPlayer.pDesc );
	pUser->m_hPlayer.pDesc = NULL;
	
	DeleteHandleObject( pUser->m_hPlayerHead.pHandle );	// 머리는 Desc가 없다.

	if(pUser->m_hPlayerRHand.pHandle)
		DeleteHandleObject( pUser->m_hPlayerRHand.pHandle );
	if(pUser->m_hPlayerLHand.pHandle)
		DeleteHandleObject( pUser->m_hPlayerLHand.pHandle );
	if(pUser->m_hPlayerHelmet.pHandle)			
		DeleteHandleObject( pUser->m_hPlayerHelmet.pHandle );
	if(pUser->m_hPlayerFlag.pHandle)			
		DeleteHandleObject( pUser->m_hPlayerFlag.pHandle );
	
	if(pUser->m_hPlayerRHand.pDesc)
	{
		FreeObjDesc(pUser->m_hPlayerRHand.pDesc);
		pUser->m_hPlayerRHand.pDesc = NULL;
	}

	if(pUser->m_hPlayerFlag.pDesc)
	{
		FreeObjDesc(pUser->m_hPlayerFlag.pDesc);
		pUser->m_hPlayerFlag.pDesc = NULL;
	}

	if(pUser->m_hPlayerLHand.pDesc)
	{
		FreeObjDesc(pUser->m_hPlayerLHand.pDesc);
		pUser->m_hPlayerLHand.pDesc = NULL;
	}

	DeleteHandleObject( pUser->m_hMoveStartEffect.pHandle );
	FreeObjDesc( pUser->m_hMoveStartEffect.pDesc );
	pUser->m_hMoveStartEffect.pDesc = NULL;

	DeleteHandleObject( pUser->m_hMoveStopEffect.pHandle );
	FreeObjDesc( pUser->m_hMoveStopEffect.pDesc );
	pUser->m_hMoveStopEffect.pDesc = NULL;
	
	DeleteHandleObject( pUser->m_hPlayerShop.pHandle );
	FreeObjDesc( pUser->m_hPlayerShop.pDesc );
	pUser->m_hPlayerShop.pDesc = NULL;

	pUser->DeleteMagicArray();
	
	if(pUser->m_pBaloonChat.pSpr)
	{
		pUser->m_pBaloonChat.pSpr->Release();
		pUser->m_pBaloonChat.pSpr = NULL;
	}	
	
	if(pUser->m_hShadowHandle)
	{
		DeleteHandleObject(pUser->m_hShadowHandle);		
		pUser->m_hShadowHandle = NULL;
	}	
	
	if(pUser->m_hSelfPortalEffect.pHandle)
	{
		DeleteHandleObject(pUser->m_hSelfPortalEffect.pHandle);
		pUser->m_hSelfPortalEffect.pHandle = NULL;
		FreeObjDesc( pUser->m_hSelfPortalEffect.pDesc );
		pUser->m_hSelfPortalEffect.pDesc = NULL;
	}

	if(pUser->m_hPlayerEmoticon.pHandle)
	{
		DeleteHandleObject(pUser->m_hPlayerEmoticon.pHandle);
		pUser->m_hPlayerEmoticon.pHandle = NULL;
		FreeObjDesc( pUser->m_hPlayerEmoticon.pDesc );
		pUser->m_hPlayerEmoticon.pDesc = NULL;
	}
	
	pUser->RemoveResource();
	memset( pUser, 0, sizeof( CUser ) );
	LALFree( g_pUserPool, (void*)pUser );
	
	pUser = NULL;
}

void FreeMonster( CMonster* pMonster )
{	
	if( !pMonster )	asm_int3();

	if(pMonster->m_hShadowHandle)
	{
		DeleteHandleObject(pMonster->m_hShadowHandle);
		pMonster->m_hShadowHandle = NULL;
	}	

	DeleteHandleObject( pMonster->m_hMonster.pHandle );
	FreeObjDesc( pMonster->m_hMonster.pDesc );
	pMonster->m_hMonster.pHandle = NULL;
	pMonster->m_hMonster.pDesc = NULL;

	if( pMonster->m_pEffectTarget )
	{
		RemoveEffectDesc(pMonster->m_pEffectTarget);
		pMonster->m_pEffectTarget = NULL;
	}
	
	if( pMonster->m_pEffectSelect )
	{ 
		RemoveEffectDesc(pMonster->m_pEffectSelect);
		pMonster->m_pEffectSelect = NULL;
	}

#if !defined(SOUND_NEW)
	pMonster->ReleaseSoundHandle();
#endif

	pMonster->RemoveResource();
	memset( pMonster, 0, sizeof( CMonster ) );
	LALFree( g_pMonsterPool, (void*)pMonster );
	pMonster = NULL; 
}

void FreeItem( ITEM* pItem )
{
	if( !pItem )
		asm_int3();
	
	DeleteHandleObject( pItem->hItem.pHandle );
	pItem->hItem.pHandle = NULL;

	FreeObjDesc( pItem->hItem.pDesc );
	pItem->hItem.pDesc = NULL;

	memset( pItem, 0, sizeof( ITEM ) );
	LALFree( g_pItemPool, (void*)pItem );
	pItem = NULL;
}

void FreeEffect( EffectDesc* pEffectDesc )
{
	DeleteHandleObject( pEffectDesc->hEffect.pHandle );
	pEffectDesc->hEffect.pHandle = NULL;
	if (pEffectDesc->hEffect.pDesc)
	{
		pEffectDesc->hEffect.pDesc->ObjectFunc = NULL;
		FreeObjDesc( pEffectDesc->hEffect.pDesc );
	}
	
	pEffectDesc->hEffect.pDesc = NULL;

	memset( pEffectDesc, 0, sizeof( EffectDesc ) );
	LALFree( g_pEffectPool, (void*)pEffectDesc );
	pEffectDesc = NULL;
}


void RemoveAllItem()
{
	ListNode<ITEM>*		pNode;
	ListNode<ITEM>*		pDel;

	pNode = g_pItemHash->GetHead();
	while( pNode )
	{
		pDel = pNode;
		pNode = pNode->pNext;
		RemoveItem( pDel->pData );
	}
}


void RemoveItem( ITEM*	pItem )
{
	if( !pItem ) return;
	
	g_pItemHash->Delete( pItem, pItem->Item.m_Serial.i64Serial );
	
	FreeItem( pItem );
	pItem = NULL;
}


void RemoveAllPlayer()
{
	ListNode<CUser>*	pNode;
	ListNode<CUser>*	pDel;

	pNode = g_pUserHash->GetHead();
	while( pNode )
	{
		pDel = pNode;
		pNode = pNode->pNext;
		if( pDel->pData != g_pMainPlayer )
		{
			RemovePlayer( pDel->pData );
		}
	}
}


void RemoveAllMonster()
{
	ListNode<CMonster>*	pNode;
	ListNode<CMonster>*	pDel;
	
	pNode = g_pMonsterHash->GetHead();
	while( pNode )
	{
		pDel	= pNode;
		pNode	= pNode->pNext;
		RemoveMonster( pDel->pData );
	}
}


void RemoveMonster( CMonster* pMonster )
{
	if( !pMonster ) return;
	
	if( pMonster->m_dwMonsterIndex == g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ] )
	{
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0; 
	}
	if( pMonster->m_dwMonsterKind == OBJECT_TYPE_GUARDIAN )
	{
		if( g_pMainPlayer->m_pGuardian[ pMonster->m_bZipCode ] == pMonster )
		{
			g_pMainPlayer->m_pGuardian[ pMonster->m_bZipCode ] = NULL;									
		}
	}
	else if( pMonster->m_dwLordIndex == g_pMainPlayer->m_dwUserIndex )
	{
		if( g_pMainPlayer->m_pMonster[ pMonster->m_bZipCode ] == pMonster )
		{
			g_pMainPlayer->m_pMonster[ pMonster->m_bZipCode ] = NULL;
		}
	}

	g_pMonsterHash->Delete( pMonster, pMonster->m_dwMonsterIndex );

	if (pMonster->m_pCurTile)
		g_pMap->SetTileOccupied( pMonster->m_pCurTile->wIndex_X, pMonster->m_pCurTile->wIndex_Z, TILE_EMPTY, pMonster );
		
	FreeMonster( pMonster );
	pMonster = NULL;
}


void RemovePlayer( CUser* pUser )
{
	LP_PARTY_USER lpPartyNode = g_pPartyUserHash->GetData( pUser->m_dwUserIndex );

	if(lpPartyNode)
	{
		CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

		if(pMiniMapWnd->GetPartyPtr(lpPartyNode->dwResourceId))
		{
			pMiniMapWnd->SetPartyPtr(lpPartyNode->dwResourceId);

			if(pMiniMapWnd->m_bInit==TRUE)	
				pMiniMapWnd->SetRender(lpPartyNode->dwResourceId, FALSE);
		}
	}
	
	if( !pUser ) 
	{
		asm_int3();
		return;
	}
	
	if( pUser->m_dwUserIndex == g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ] )
	{
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0; 
	}

	g_pMap->SetTileOccupied( pUser->m_pCurTile->wIndex_X, pUser->m_pCurTile->wIndex_Z, TILE_EMPTY, pUser );
	
	
	g_pUserHash->Delete( pUser, pUser->m_dwUserIndex );
	FreeUser( pUser );
	pUser = NULL;

	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	pGroupWnd->SetPartyUI();

	g_dwUserNum--;
}


void RemoveAllEffectDesc()
{
	ListNode<EffectDesc>*		pNode;
	ListNode<EffectDesc>*		pDel;

	pNode = g_pEffectHash->GetHead();
	while( pNode )
	{
		pDel = pNode;
		pNode = pNode->pNext;
		RemoveEffectDesc( pDel->pData );
	}
}


void RemoveEffectDesc( EffectDesc*	pEffectDesc )
{
	g_Camera.iCameraMoveOption = CAMERA_MOVE_OPTION_SCREEN_FRAME;
	if (pEffectDesc->m_sLightDescEx.m_handle)
		g_pEffectLayer->DetachLight(pEffectDesc);
	
	for(int i = 0; i < 8; ++i)
	{
		if (pEffectDesc->m_pSound[i])
		{
			SAFE_REL_SOUND(pEffectDesc->m_pSound[i]);
			SAFE_REL_SOUND_FILE(pEffectDesc->m_hSoundFile[i]);
		}
	}
		
	g_pEffectHash->Delete( pEffectDesc, pEffectDesc->dwHashKey );
	
	FreeEffect( pEffectDesc );
	pEffectDesc = NULL;
}

BOOL IsPKZone()
{
#if IS_JAPAN_LOCALIZING()
	return (g_pThisDungeon->IsConquer()&& g_pMainPlayer->m_bCurLayer) || g_bAdultMode;
#else
	return (g_pThisDungeon->IsConquer()&& g_pMainPlayer->m_bCurLayer) || (!g_pThisDungeon->IsPathWay() && !g_pThisDungeon->IsVillage() && g_pThisDungeon->m_wLayerCount-1 == g_pMainPlayer->m_bCurLayer) ;
#endif
}

char* CheckHackApplication( char* szCaption )
{
	if( !szCaption )
		return NULL;

	DWORD	dwId;
	HWND	hHandle;
	HANDLE  hProcessHandle;
	
	hHandle = FindWindow( NULL, szCaption );
	
	if( NULL == hHandle )
		return NULL;

	GetWindowThreadProcessId( hHandle, &dwId );
	hProcessHandle = OpenProcess( PROCESS_TERMINATE, FALSE, dwId );
	TerminateProcess( hProcessHandle, 0 );

	return szCaption;
}

void SearchHack()
{
	DWORD				dwI;
	char*				szHack;
	CTDS_FIND_HACK		Hack;

	if( g_dwCurTick - g_dwLastSearchHackTick >= 30000 )	// 30 Sec.
		g_dwLastSearchHackTick = g_dwCurTick;
	else 
		return;
	
	for( dwI=1; dwI<MAX_HACK_APPLICATION; dwI++ )
	{
		szHack = CheckHackApplication( g_szHackApplication[dwI] );
		if( !szHack )
			continue;

		// 걸렸으. 개새리... 씹새리...
		Hack.dwHackNum	= dwI;
		g_pNet->SendMsg( (char*)&Hack, Hack.GetPacketSize(), SERVER_INDEX_ZONE );
	}
}

void CheckTick()
{
	if( g_dwCurTick - g_dwLastCheckTick >= 60000 )
		g_dwLastCheckTick = g_dwCurTick;
	else 
		return;

	CTDS_TICK_CHECK	Tick;
	Tick.dwCurClientTick	= g_dwCurTick;

	g_pNet->SendMsg( (char*)&Tick, Tick.GetPacketSize(), SERVER_INDEX_ZONE );
}

LPObjectDesc AllocObjDesc()
{
	LPObjectDesc pDesc;
	
	pDesc = (LPObjectDesc)LALAlloc( g_pObjDescPool );
	if( !pDesc )
	{
		MessageBox( g_hMainWnd, "Alloc ObjectDesc Failed!", "Failed", MB_ICONSTOP );
	}
	else
	{
		memset( pDesc, 0, sizeof( OBJECT_DESC ) );
	}
	
	pDesc->bSkip = TRUE;
	    
	g_dwDescNum++;
	return pDesc;
}

void FreeObjDesc( LPObjectDesc pDesc )
{
	if( !pDesc ) return;
	LALFree( g_pObjDescPool, (void*)pDesc );
	memset(pDesc, 0, sizeof(OBJECT_DESC));
	pDesc = NULL;
	g_dwDescNum--;	
}

GXOBJECT_HANDLE	CreateHandleObject( char* szFileName, GXSchedulePROC pProc, void* pData, DWORD dwFlag )
{
	GXOBJECT_HANDLE	hHandle;
	
	hHandle = g_pExecutive->CreateGXObject( szFileName, pProc, pData, dwFlag );
	
#ifdef _DEBUG
	if(_CrtCheckMemory()==NULL)
		asm_int3()
#endif

	return hHandle;
}

void DeleteHandleObject( GXOBJECT_HANDLE hHandle )
{
	if(g_hHandle==hHandle)
		g_hHandle = NULL;
	if(g_hEffectHandle==hHandle)
		g_hEffectHandle = NULL;

	if(hHandle == NULL)
		return;

	if( !g_pExecutive->DeleteGXObject( hHandle ) )
	{
#ifdef _DEBUG
		if(_CrtCheckMemory()==NULL)
			asm_int3()
#endif
		MessageBox( g_hMainWnd, "DeleteGXObject Failed!", "Failed!", MB_ICONSTOP );
	}
}


DWORD DecodeCDBData(char* szLoadFile,  void* pReturnValue, char* szDecodeKey, int nDecodeSubKey)
{
	int nKeyLen = lstrlen(szDecodeKey);

	FILE* fp;	
	DWORD	dwCur = 0;
	DWORD   dwTotalLen = 0;
	BOOL bRet = TRUE;
	
	fp = fopen( szLoadFile, "rb" );
	if(!fp)
		return FALSE;

	fread(&dwTotalLen, sizeof(DWORD), 1, fp );

	char* szBuffer = new char[ dwTotalLen ];
	
	int nRemain;
	while( bRet )
	{
		if(!fread(szBuffer + dwCur, nKeyLen, 1, fp ))
			bRet = FALSE;

		 nRemain = nKeyLen;
		if(!bRet)
			nRemain = dwTotalLen - dwCur;

		for(int k=0; k<nRemain; k++ )
			szBuffer[ dwCur + k ] ^= (szDecodeKey[k] + nDecodeSubKey);

		dwCur += nRemain;
	}
	
	memcpy(pReturnValue, szBuffer, dwTotalLen);
	delete [] szBuffer;
	fclose(fp);

	return dwCur;	//총 읽어드린 바이트수를 리
}


void ReleaseMainPlayer()
{			
	if(!g_pMainPlayer)
		return;

	g_pMainPlayer->RemoveResource();
	if(g_pMainPlayer->m_hShadowLightHandle)
	{
		g_pExecutive->GXODetachLight(g_pMainPlayer->m_hPlayer.pHandle, g_pMainPlayer->m_hShadowLightHandle);
		g_pExecutive->DeleteGXLight(g_pMainPlayer->m_hShadowLightHandle);
		g_pMainPlayer->m_hShadowLightHandle = NULL;
	}

	//머리 몸통 오브젝트 해제 
	if(g_pMainPlayer->m_hPlayer.pHandle)
	{
		DeleteHandleObject( g_pMainPlayer->m_hPlayer.pHandle );
		g_pMainPlayer->m_hPlayer.pHandle = NULL;
	}
	
	if(g_pMainPlayer->m_hPlayerHead.pHandle)
	{
		DeleteHandleObject( g_pMainPlayer->m_hPlayerHead.pHandle );
		g_pMainPlayer->m_hPlayerHead.pHandle = NULL;
	}
	
	if(g_pMainPlayer->m_hPlayerFlag.pHandle)
	{
		DeleteHandleObject( g_pMainPlayer->m_hPlayerFlag.pHandle );
		g_pMainPlayer->m_hPlayerFlag.pHandle = NULL;
	}
	
	if(g_pMainPlayer->m_hPlayerLHand.pHandle)			
	{
		DeleteHandleObject( g_pMainPlayer->m_hPlayerLHand.pHandle );
		g_pMainPlayer->m_hPlayerLHand.pHandle = NULL;
	}
		
	if(g_pMainPlayer->m_hPlayerRHand.pHandle)			
	{
		DeleteHandleObject( g_pMainPlayer->m_hPlayerRHand.pHandle );
		g_pMainPlayer->m_hPlayerRHand.pHandle = NULL;
	}
		
	if(g_pMainPlayer->m_hPlayerHelmet.pHandle)			
	{
		DeleteHandleObject( g_pMainPlayer->m_hPlayerHelmet.pHandle );
		g_pMainPlayer->m_hPlayerHelmet.pHandle = NULL;
	}

	if(g_pMainPlayer->m_hMoveStartEffect.pHandle)
	{
		DeleteHandleObject( g_pMainPlayer->m_hMoveStartEffect.pHandle );
		g_pMainPlayer->m_hMoveStartEffect.pHandle = NULL;
	}
	
	if(g_pMainPlayer->m_hMoveStopEffect.pHandle)
	{
		DeleteHandleObject( g_pMainPlayer->m_hMoveStopEffect.pHandle );
		g_pMainPlayer->m_hMoveStopEffect.pHandle = NULL;
	}

	if(g_pMainPlayer->m_hPlayerShop.pHandle)
	{
		DeleteHandleObject( g_pMainPlayer->m_hPlayerShop.pHandle );
		g_pMainPlayer->m_hPlayerShop.pHandle = NULL;
	}

	if(g_pMainPlayer->m_hMoveStartEffect.pDesc)
	{
		FreeObjDesc( g_pMainPlayer->m_hMoveStartEffect.pDesc);
		g_pMainPlayer->m_hMoveStartEffect.pDesc = NULL;
	}

	if(g_pMainPlayer->m_hMoveStopEffect.pDesc)
	{
		FreeObjDesc( g_pMainPlayer->m_hMoveStopEffect.pDesc);
		g_pMainPlayer->m_hMoveStopEffect.pDesc = NULL;
	}

	if(g_pMainPlayer->m_hPlayerFlag.pDesc)
	{
		FreeObjDesc( g_pMainPlayer->m_hPlayerFlag.pDesc);
		g_pMainPlayer->m_hPlayerFlag.pDesc = NULL;
	}
	
	if(g_pMainPlayer->m_hPlayerShop.pDesc)
	{
		FreeObjDesc( g_pMainPlayer->m_hPlayerShop.pDesc);
		g_pMainPlayer->m_hPlayerShop.pDesc = NULL;
	}

	if(g_pMainPlayer->m_hShadowHandle)
	{
		DeleteHandleObject(g_pMainPlayer->m_hShadowHandle);
		g_pMainPlayer->m_hShadowHandle = NULL;
	}
	g_pMainPlayer->DeleteMagicArray();
	
	if(g_pMainPlayer->m_hSelfPortalEffect.pHandle)
	{
		DeleteHandleObject(g_pMainPlayer->m_hSelfPortalEffect.pHandle);
		g_pMainPlayer->m_hSelfPortalEffect.pHandle = NULL;
		FreeObjDesc( g_pMainPlayer->m_hSelfPortalEffect.pDesc );
		g_pMainPlayer->m_hSelfPortalEffect.pDesc = NULL;
	}

	if (g_pMainPlayer->m_hPlayerEmoticon.pHandle)
	{
		DeleteHandleObject(g_pMainPlayer->m_hPlayerEmoticon.pHandle);
		g_pMainPlayer->m_hPlayerEmoticon.pHandle = NULL;
		FreeObjDesc( g_pMainPlayer->m_hPlayerEmoticon.pDesc );
		g_pMainPlayer->m_hPlayerEmoticon.pDesc = NULL;
	}	

	if(g_pMainPlayer->m_pBaloonChat.pSpr)		
	{
		g_pMainPlayer->m_pBaloonChat.pSpr->Release();
		g_pMainPlayer->m_pBaloonChat.pSpr = NULL;
	}
	
	if (g_pMainPlayer->m_MatchInfoSpr.pSpr)
	{
		g_pMainPlayer->m_MatchInfoSpr.pSpr->Release();
		g_pMainPlayer->m_MatchInfoSpr.pSpr = NULL; 
	}	
	
	delete g_pMainPlayer;
	g_pMainPlayer = NULL;
}

void ReleaseMainPlayerResource()	// <== 메인 플레이어에 걸려있는 맵바뀔떄 지워햐할것들을 지우다.
{
	g_pMainPlayer->RemoveResource();
	
	if(g_pMainPlayer->m_hShadowLightHandle)
	{
		g_pExecutive->GXODetachLight(g_pMainPlayer->m_hPlayer.pHandle, g_pMainPlayer->m_hShadowLightHandle);
		g_pExecutive->DeleteGXLight(g_pMainPlayer->m_hShadowLightHandle);
		g_pMainPlayer->m_hShadowLightHandle = NULL;
	}

	// Delete Shadow Mod
	if(g_pMainPlayer->m_hShadowHandle)
	{
		DeleteHandleObject(g_pMainPlayer->m_hShadowHandle);
		g_pMainPlayer->m_hShadowHandle = NULL;
	}
}

GXLIGHT_HANDLE CreateLight(LIGHT_DESC* pLightDesc, VECTOR3* pVecPostion, BYTE bLightNum)
{
	DWORD dwEffectOptionLevel = CGameMenuWnd::GetInstance()->m_byEffectIndex;
	GXLIGHT_HANDLE hLight = NULL;

	if (dwEffectOptionLevel == __EFFECT_OPTION__MAX__ )
	{
		pLightDesc->dwAmbient = 0xffff0000;
		pLightDesc->dwDiffuse = 0xffff0000;
		pLightDesc->dwSpecular = 0;
		pLightDesc->v3Point.x = pVecPostion->x;
		pLightDesc->v3Point.y = 100.0f;
		pLightDesc->v3Point.z = pVecPostion->z;
		pLightDesc->fFov = PI/2.0f;
		pLightDesc->v3Up.x = 0.0f;
		pLightDesc->v3Up.y = 1.0f;
		pLightDesc->v3Up.z = 0.0f;

		pLightDesc->fRs = 250.0f;
		pLightDesc->v3To = *pVecPostion;		

		hLight = g_pExecutive->CreateGXLight(pLightDesc,NULL,0,bLightNum, NULL, 0);

		g_pExecutive->GXLEnableDynamicLight(hLight);		
	}	

	return hLight;
}


void InitializeEffect(BOOL bChk)
{
	if (g_pEffectLayer)
	{
		// 있던넘 다시 지우고 다시 로드하기 위해서.
		delete g_pEffectLayer;
		g_pEffectLayer = NULL;
	}

	g_pEffectLayer = new EffectLayer;
	g_pEffectLayer->Init(bChk);		
}


void PreLoadCurLayerMonster(DWORD* pdwMonsterKind)
{
	DWORD	pdwMod[10];
	memset( pdwMod, 0, sizeof(DWORD)*10);
	BOOL	bLoad;
	char szBuf[0xff]={0,};

	for( int i=0; i<MAX_MONSTER_KIND; i++ )
	{
		if( pdwMonsterKind[i] == 0 ) break;
		bLoad = TRUE;
		for( int j=0; pdwMod[j] != 0; j++ )
		{
			if( pdwMod[j] == pdwMonsterKind[i] )
			{
				bLoad = FALSE;
				break;
			}
		}

		if( bLoad )
		{
			pdwMod[j] = pdwMonsterKind[i];
		}
	}
	
	for( i=0; pdwMod[i]!=0; i++ )
	{
		wsprintf( szBuf, "m%05d.chr", pdwMod[i] ); 
		g_pExecutive->PreLoadGXObject( GetFile( szBuf, DATA_TYPE_MONSTER ) );
	}

	// 발록을 로드한다. 임시.
	wsprintf( szBuf, "m%05d.chr", 10 ); 
	g_pExecutive->PreLoadGXObject( GetFile( szBuf, DATA_TYPE_MONSTER ) );
}


void PreLoadItem()
{	
	char					szInfo[0xff]	= {0,};
	ListNode<CBaseItem>*	lpItemTableNode	= NULL;
	CBaseItem*				lpItemTable		= NULL;
	
	lpItemTableNode = g_pItemTableHash->GetHead();

	while(lpItemTableNode)
	{	
		lpItemTable = lpItemTableNode->pData;
				
		if(lpItemTable)
		{
			if(lpItemTable->dwCode_ID == ITEM_KIND_WEAPON)
			{		
				if(lpItemTable->BaseItem_Weapon.bModCount !=0 )
				{			
					LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(lpItemTable->wID);
					
					if(lpItemResource)
					{
						if(lpItemResource->wModCount>0)
						{
							for(int i = 0; i < lpItemResource->wModCount; i++)
							{			
								if(lpItemResource->byPreLoad==1)
								{																		
									ItemDataName(szInfo, lpItemTable->wID, i);
									g_pExecutive->PreLoadGXObject(GetFile(szInfo, DATA_TYPE_ITEM));
								}						
							}
						}
					}
				}			
			}
			else if(lpItemTable->dwCode_ID == ITEM_KIND_AROMR)				
			{	
				// Mod //
				if(lpItemTable->BaseItem_Armor.bModCount!=0)
				{			
					LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(lpItemTable->wID);
					
					if(lpItemResource)
					{
						if(lpItemResource->wModCount >0 )
						{
							for(int i = 0; i < lpItemResource->wModCount; i++)
							{
								if(lpItemResource->byPreLoad==1)
								{							
									ItemDataName(szInfo, lpItemTable->wID, i);
									
									if(lpItemResource->wID/ITEM_DISTRIBUTE==11)
										g_pExecutive->PreLoadGXObject(GetFile(szInfo, DATA_TYPE_CHARACTER));
									else
										g_pExecutive->PreLoadGXObject(GetFile(szInfo, DATA_TYPE_ITEM));														
								}					
							}
						}
					}
				}		
			}			
			else if(lpItemTable->dwCode_ID == ITEM_KIND_RIDE)
			{	
				if(lpItemTable->BaseItem_Ride.bModCount != 0)
				{
					LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(lpItemTable->wID);
					
					if(lpItemResource)
					{
						if(lpItemResource->wModCount >0 )
						{
							for(int i = 0; i < lpItemResource->wModCount; i++)
							{					
								if(lpItemResource->byPreLoad==1)
								{
									ItemDataName(szInfo, lpItemTable->wID, i);
									g_pExecutive->PreLoadGXObject(GetFile(szInfo, DATA_TYPE_ITEM));
								}
							}
						}
					}
				}
			}
		}

		lpItemTableNode = lpItemTableNode->pNext;		
	}

	// Head //
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

void PreLoadDamageNumber()
{
	int i, j;
	char szBuf[1024]={0,};

	// 케릭터
	for( i=0; i<4; i++ )
	{
		for( j=0; j<10; j++ )
		{
			wsprintf( szBuf, "dfr%d.chr", j );
			g_pExecutive->PreLoadGXObject( GetFile( szBuf, DATA_TYPE_DAMAGENUMBER ) );
		}
	}
	
	g_pExecutive->PreLoadGXObject( GetFile( "dfrblock.chr", DATA_TYPE_DAMAGENUMBER ) );
	g_pExecutive->PreLoadGXObject( GetFile( "dfrmiss.chr", DATA_TYPE_DAMAGENUMBER ) );

	// 데미지 번호를 로드하자.
	for( i=0; i<4; i++ )
	{
		for( j=0; j<10; j++ )
		{
			wsprintf( szBuf, "dfy%d.chr", j );
			
			g_pExecutive->PreLoadGXObject( GetFile( szBuf, DATA_TYPE_DAMAGENUMBER ) );
		}
	}
	
	g_pExecutive->PreLoadGXObject( GetFile( "dfyblock.chr", DATA_TYPE_DAMAGENUMBER ) );
	g_pExecutive->PreLoadGXObject( GetFile( "dfymiss.chr", DATA_TYPE_DAMAGENUMBER ) );
	g_pExecutive->PreLoadGXObject( GetFile( "dfcritical.chr", DATA_TYPE_DAMAGENUMBER ) );
}

void InitBaseMonsterInfo()
{

}

void MinimizePlayer(GXOBJECT_HANDLE handle, BOOL bMinimize)
{
	VECTOR3 vScale;
	if(bMinimize)
	{
		vScale.x = 0.6f;
		vScale.y = 0.6f;
		vScale.z = 0.6f;
		g_pExecutive->GXOSetScale(handle, &vScale);
	}
	else
	{
		vScale.x = 1.0f;
		vScale.y = 1.0f;
		vScale.z = 1.0f;
		g_pExecutive->GXOSetScale(handle, &vScale);
	}
}

// 사운드 리스너 세팅
// 리스터위치는 메인 캐릭터의 위치로 하고
// 회전각은 카메라의 Y축 회전각만 적용하여 세팅한다.
void SetListener(VECTOR3 * pV3Angle)
{
	if (NULL == g_pMainPlayer || NULL == g_pSoundLib)
		return;

	VECTOR3		v3CameraAngleRad;

	g_pGeometry->GetCameraAngleRad( &v3CameraAngleRad, 0 );
	v3CameraAngleRad.x = v3CameraAngleRad.z = 0.f;

	g_pSoundLib->SetListener(&g_pMainPlayer->m_v3CurPos, &v3CameraAngleRad);
}


// -----------------------------------------------------
// Name			: OpenWebSite
// Desc			: 웹사이트를 띄운다.
// Purpose		: 
// Parameter	: szSite ( "http://www.naver.com" )
// return		: void 
// Date/Code By : 040329 / seven
// -----------------------------------------------------
void OpenWebSite(const char * szSite)
{
	SECURITY_ATTRIBUTES saProcess; 
	SECURITY_ATTRIBUTES saThread;
	STARTUPINFO siStartInfo; 
	PROCESS_INFORMATION piProcInfo;	
	char		szCurDirectory[255];
	char		szBuf[255];

	memset(&saProcess, 0, sizeof(SECURITY_ATTRIBUTES));
	memset(&saThread, 0, sizeof(SECURITY_ATTRIBUTES));
	memset(&siStartInfo, 0, sizeof(STARTUPINFO));
	memset(&piProcInfo, 0, sizeof(PROCESS_INFORMATION));

	saProcess.nLength = sizeof(SECURITY_ATTRIBUTES);
	saThread.nLength = sizeof(SECURITY_ATTRIBUTES);

	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.dwX = 0;
	siStartInfo.dwY = 0;
	siStartInfo.dwXSize = 1024;
	siStartInfo.dwYSize = 768;
	siStartInfo.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE; 

	saProcess.nLength = sizeof(SECURITY_ATTRIBUTES);
	saThread.nLength = sizeof(SECURITY_ATTRIBUTES);

	// web page open 
	GetCurrentDirectory(255, szCurDirectory);
	wsprintf(szBuf, "explorer.exe %s", szSite);
	CreateProcess( NULL, szBuf, &saProcess, &saThread, 
		FALSE, NULL, NULL, szCurDirectory, &siStartInfo, &piProcInfo);
}


bool RemoveCharacter(char* pszResult, char* pszSource, char cCharacter)
{
	if (NULL == pszResult || NULL == pszSource)
	{
		return false;
	}

	int i		= 0;
	int iCount	= 0;

	for (i = 0; i < (int)lstrlen(pszSource); i++)
	{
		if (cCharacter != pszSource[i])
		{
			pszResult[iCount++] = pszSource[i];
		}		
	}

	pszResult[iCount] = '\0';

	return true;
}

bool MakeLowerString(char* pszResult, char* pszSource)
{
	if (NULL == pszResult || NULL == pszSource)
	{
		return false;
	}

	int i = 0;
	for (i = 0; i < (int)lstrlen(pszSource); i++)
	{
		if (64 < (int)pszSource[i] && 91 > (int)pszSource[i])
		{
			pszResult[i] = (char)(pszSource[i] + 32);
		}
		else
		{
			pszResult[i] = pszSource[i];
		}
	}

	return true;
}


#ifdef __PACKAGE_MODE__
int LoadPakList(char* pszFileName, PACKFILE_NAME_TABLE* pOut, int iPakTableSize)
{
	FILE* fp = fopen(pszFileName, "rt");
	if (NULL == fp)
	{
		return false;
	}	

	// read datas..
	char szCurrentDirectory[_MAX_PATH]={0,};
	char szOneLine[_MAX_PATH]={0,};	

	GetCurrentDirectory(_MAX_PATH, szCurrentDirectory);
	memset(pOut, 0, sizeof(PACKFILE_NAME_TABLE) * iPakTableSize);

	int iCount = 0;	

	for (;;)
	{
		if (0 != feof(fp))
		{
			break;
		}
		
		fgets(szOneLine, _MAX_PATH, fp);
		RemoveCharacter(szOneLine, szOneLine, '\n'); // carrage return character removing.

		// Prevent access violation.
		if (iCount >= iPakTableSize)
		{
			break;
		}

		if (0 < lstrlen(szOneLine))
		{
			__lstrcpyn(pOut[iCount].szFileName, szCurrentDirectory, MAX_PATH);
			strncat(pOut[iCount].szFileName, "\\Data\\", _MAX_PATH);
			strncat(pOut[iCount].szFileName, szOneLine, _MAX_PATH);
			
			pOut[iCount].dwFlag = 0;
			iCount++;
		}	

		memset(szOneLine, 0, _MAX_PATH);
	}

	// restore used resource.
	fclose(fp);

	// return its data.	
	return iCount;
}
#endif


DWORD GetUserIndexByNameFromUserHash(const char * pszCharacterName)
{
	ListNode<CUser>* pUserNode =  g_pUserHash->GetHead();
	while (pUserNode)
	{
		if ( !lstrcmp(pUserNode->pData->m_szName, pszCharacterName) )
		{
			return pUserNode->pData->m_dwUserIndex;
		}
		
		pUserNode = pUserNode->pNext;
	}	

	// 찾지 못하였다.
	return 0;	
}


char* _StrTok(const char* str, const char sep)
{
	// strtok()와 같은 기능의 함수이지만 separator를 1개만 받고
	// 인자가 비어있는 경우도 인자로 인정함
	static const int TOK_BUFSIZE = 1024*40;
	static char* pnow = NULL;
	static char buf[TOK_BUFSIZE];
	static char token[TOK_BUFSIZE];

	if(str)
	{
		assert(lstrlen(str) < TOK_BUFSIZE);		// ASSERT
		lstrcpy(buf, str);
		pnow = buf;
	}

	if(!pnow || *pnow == 0)
		return NULL;

	int tlen = 0;
	char* porg = pnow;
	while(*pnow != 0)
	{
		if(*pnow == sep)
		{
			++pnow;
			break;
		}

		++tlen;
		++pnow;
	}

	if(tlen > 0)
	{
		__lstrcpyn(token, porg, tlen);
		token[tlen] = 0;
	}
	else
	{
		token[0] = 0;
	}

	return token;
}


BOOL CmdLineProcessForNetMarble(LPSTR lpCmdLine)
{
	if(lstrlen(lpCmdLine) >= 1024*40)
		return FALSE;

	char buf[1024*20]	= {0,};
	char arg[6][1024*2] = {0,};
	int argcnt			= 0;

	if(!GetNMClipData(buf, sizeof(buf)-1, lpCmdLine, true))
		return FALSE;

	char* token = _StrTok(buf, ',');

	while(token && argcnt < 6)
	{
		lstrcpy(arg[argcnt], token);
		argcnt++;
		token = _StrTok(NULL, ',');
	}

	if(lstrlen(arg[0]) > 16)
		return FALSE;

	__lstrcpyn(g_NetMarble.szIP, arg[0], lstrlen(arg[0]));			
	g_NetMarble.dwPort  = (UINT)::atoi(arg[1]);					
	__lstrcpyn(g_NetMarble.szCPCookie, arg[4], lstrlen(arg[4]));
	g_NetMarble.bIsTryCorrectLogin = TRUE;

	return TRUE;
}


// AutoPatch.exe 실행	
BOOL RunAutoPatch()		
{
	STARTUPINFO				sp;
	PROCESS_INFORMATION		pi;
	BOOL					bR;
	
	memset( &sp, 0, sizeof(STARTUPINFO));
	sp.cb			= sizeof(STARTUPINFO);
	sp.dwFlags		= STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USESTDHANDLES; //STARTF_USESHOWWINDOW 
	sp.wShowWindow	= SW_SHOW;
	
	bR = CreateProcess( CORUM_AUTOPATCH_EXE_FILE,	// name of executable module
		"",							// command line string 넷마블.
		NULL,						// SD
		NULL,						// SD
		FALSE,						// handle inheritance option
		CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,	// creation flags
		NULL,						// new environment block
		NULL,						// current directory name
		&sp,						// startup information
		&pi							// process information
		);
	
	if( bR == FALSE )
		return FALSE;
	
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread );
	
	return TRUE;
}
