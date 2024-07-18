#pragma once

#ifndef __GM_TOOL_HEADER__
#define __GM_TOOL_HEADER__

#include "stdafx.h"

#define USER_GRADE_GM			100
#define UM_SENDMESSAGE			WM_USER+20

INT_PTR CALLBACK GMToolTab1(HWND hdlg,  UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK GMDialog(HWND hdlg,  UINT uMsg, WPARAM wParam, LPARAM lParam);

typedef struct GlobalVariableGM
{
	BYTE	bQuickMove;			
	BYTE	bInvisibleMode;

} GLOBAL_VARIABLE_GM, *LPGlobalVariable_GM;

extern HWND					g_hGMToolDlg;
extern HWND					g_hGMToolTab1;
extern LPGlobalVariable_GM	g_pGVGm;
#endif