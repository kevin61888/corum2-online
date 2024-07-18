#pragma once

#define	MAX_LOGO_COUNT				1
#define	LOGO_SHOW_TIME				2000	//Miliseconds

BOOL InitGameIntro();
void UpdateGameIntro();
void ReleaseGameIntro();

typedef struct GlobalVariableIntro
{
	BYTE	bCurLogoCount;
	DWORD	dwStartRenderTick;

} GLOBAL_VARIABLE_INTRO, *LPGlobalVariable_Intro;

// ·»´õ ÇÔ¼ö.
DWORD __stdcall AfterRenderGameIntro();
DWORD __stdcall BeforeRenderGameIntro();