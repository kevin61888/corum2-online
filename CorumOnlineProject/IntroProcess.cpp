#include "stdafx.h"
#include "IntroProcess.h"
#include "InitGame.h"
#include "GameControl.h"
#include "CorumResource.h"
#include "DefResource.h"

LPGlobalVariable_Intro	g_pGvIntro = NULL;
//==================================================================//
// 인트로 상태에서 초기화 함수.										//
//==================================================================//
BOOL InitGameIntro()
{
	g_pGvIntro = new GLOBAL_VARIABLE_INTRO;
	memset(g_pGvIntro, 0, sizeof(g_pGvIntro));

	V2_SPRITE* pSpr = g_pSprManager->CreateSprite(SPR_INTRO_ESOFNET, 0, 0);
	if(!pSpr) return FALSE;

	g_pGvIntro->bCurLogoCount++;
	g_pGvIntro->dwStartRenderTick = timeGetTime();

	return TRUE;
}
//==================================================================//
// 인트로 상태에서 삭제함수.										//
//==================================================================//
void ReleaseGameIntro()
{	
	if(g_pGvIntro)
	{
		g_pGvIntro->dwStartRenderTick = 0;

		g_pSprManager->RemoveSprite(SPR_INTRO_ESOFNET + g_pGvIntro->bCurLogoCount - 1);
	
		delete g_pGvIntro;
		g_pGvIntro = NULL;
	}	
}
//==================================================================//
// 인트로 상태에서 프로세스(초당 게임 프레임만큼 호출).				//
//==================================================================//
void UpdateGameIntro()
{
	if(!g_pGvIntro)	
		return;
	
	if(g_dwCurTick - g_pGvIntro->dwStartRenderTick >= LOGO_SHOW_TIME)
	{
		g_pSprManager->RemoveSprite(SPR_INTRO_ESOFNET + g_pGvIntro->bCurLogoCount-1);

		g_pGvIntro->bCurLogoCount++;

		if(g_pGvIntro->bCurLogoCount > MAX_LOGO_COUNT)
		{
			
			SetGameStatus( UPDATE_GAME_LOGIN );
			return;
		}		
		g_pSprManager->CreateSprite(SPR_INTRO_ESOFNET+g_pGvIntro->bCurLogoCount-1,0,0, TRUE, 100);	
		g_pGvIntro->dwStartRenderTick = timeGetTime();
	}
}
//==================================================================//
// 인트로 상태에서 렌더함수.										//
//==================================================================//
DWORD __stdcall AfterRenderGameIntro()
{
	g_pSprManager->RenderAllSprite();
	g_helpLoadingSprite.Show();
	return 0;
}

DWORD __stdcall BeforeRenderGameIntro()
{
	
	return 0;
}
//==================================================================//
//																	//
//==================================================================//