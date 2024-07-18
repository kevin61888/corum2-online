#include "stdafx.h"
#include "InitGame.h"
#include "GameControl.h"
#include "InitGame.h"
#include "resource.h"
#include "IntroProcess.h"
#include <stdio.h>
#include <crtdbg.h>
#include "develop.h"
#include "GMTool.h"
#include "message.h"
#include "DungeonTable.h"
#include "CDungeonInfoWnd.h"
#include "NMCrypt.h"
#include <assert.h>
#include "GetDxVer.h"
#include "InputManager.h"
#include "UserInterface.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "LoginProcess.h"
#include "IMEEdit.h"
#include "GlobalVariableLogin.h"
#include "MinboboWork.h"
#include "LocalTimeObserver.h"
#include "NetworkTimeObserver.h"
#include "ChinaHackToolBlock.h"


int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
#ifndef DEVELOP_MODE	
	if(!CheckGlobalEventHandle())
	{
		RunAutoPatch();	
		return 0;
	}

	if(IsHackToolUsed())
	{
		return 0;
	}
#endif
	
	int nResult = GetDXVersion();
	if(nResult < 0x801)
	{
		// MSG_ID : 443 ; Direct X 8.1 ÀÌ»ó ¹öÁ¯ÀÌ ÇÊ¿äÇÕ´Ï´Ù.
		MessageBox(g_hMainWnd,"You must install the latest version of DirectX from 8.1 Version", "CorumOnlineProject", MB_OK);	
		return FALSE;
	}
				
#if IS_KOREA_LOCALIZING()
	{
		// ·Î±×ÀÎ °áÁ¤ ? º»¼· : ³Ý¸¶ºí 
		if(lstrlen(lpCmdLine))
		{
			if(!CmdLineProcessForNetMarble(lpCmdLine))
				return 0;

			if (TRUE == g_NetMarble.bIsTryCorrectLogin)
			{
				g_bExecuteType = EXECUTE_TYPE_NETMARBLE;
			}
			else
			{ 
				asm_int3();
				return 0;
			}		
		}
	} 
#endif




#ifdef DEVELOP_MODE
	GetCurrentDirectory(_MAX_PATH, g_Dev.szDevIniPath);
	lstrcat(g_Dev.szDevIniPath, "\\Dev.ini");
#else
	if(FindWindow(WINDOW_CLASS_NAME, WINDOW_TITLE_NAME))
		return 0;
#endif
	
  	RegisterWindowClass(hInstance);
	InitInstance(hInstance, nShowCmd);	

#ifdef DEVELOP_MODE
	CentreWindow(g_hMainWnd);
#endif

#ifdef _DEBUG
	int	flag = _CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(flag);
#endif	
		
	if(!InitGame())	return 0;

#ifdef __USE_CLIENT_SPEEDHACK_CHECKER
	InitializeClietSpeedHackChecker();
#endif

#ifdef DEVELOP_MODE
	g_Dev.bRenderFramePerSec	= FALSE;
#endif

#ifdef DEVELOP_MODE
	CTWS_LOGIN	packet;
	if(g_Dev.bBeginType == DEVELOP_START_NORMAL)	goto lb_Normal;

	if( !g_pNet->ConnectServer( g_Dev.szWorldIP, g_Dev.wWorldPort, SERVER_INDEX_WORLD ) )	
	{
		// MSG_ID : 441 ; (Error! Develop Mode Only!) Fail to connect World Server! , 442 ; Connect Fail
		MessageBox(g_hMainWnd, g_Message[ETC_MESSAGE441].szMessage, g_Message[ETC_MESSAGE442].szMessage, MB_OK);	
		return 0;
	}
	
	packet.dwVersion = g_dwVersion;
	__lstrcpyn(packet.szID, g_Dev.szID, MAX_ID_REAL_LENGTH);					
	__lstrcpyn(packet.szPassword, g_Dev.szPassword, MAX_PASSWORD_REAL_LENGTH);	
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);
	goto lb_Process;	
#endif

#ifdef DEVELOP_MODE
lb_Normal:
#endif

	SetGameStatus(UPDATE_GAME_INTRO);	

#ifdef DEVELOP_MODE
lb_Process:
#endif

	if(IsHackToolUsed())
	{
		g_bIsRunning = FALSE;
	}

	MSG msg;

	while(g_bIsRunning)
	{
		if(	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			(*Render[ g_bRenderMode ])();
		}

#ifdef __USE_CLIENT_SPEEDHACK_CHECKER
		if(IsSpeedHackClient()) 
		{
#if IS_CHINA_LOCALIZING()
			MessageBox(NULL, _T("ËÙ¶ÈÒì³£,Ç¿ÖÆÍË³öÓÎÏ·!"), _T("ËÙ¶ÈÒì³£!"), MB_OK|MB_ICONWARNING);
#else
#ifndef DEVELOP_MODE			
			MessageBox(NULL, _T("DON'T USE SPEEDHACK!! ErrCode: 1"), _T("SPEEDHACK!"), MB_OK|MB_ICONWARNING);
#endif
#endif
			
#ifndef DEVELOP_MODE
			break;
#endif
		}
#endif 
	}

/*#if IS_CHINA_LOCALIZING()
#ifndef DEVELOP_MODE	
	OpenWebSite("http://corum.9you.com");
#endif
#endif
*/
	ReleaseGame();
	return 0L;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#ifdef _USE_IME
	if(GET_IMEEDIT()->ProcessIMEMessage(message, wParam, lParam)) return 0;
#endif 

	switch (message) 
	{
	case UM_GAME_RESTART:	
		{						
			g_pThisDungeon = NULL;			
			g_pDungeonTable->RemoveDungeonTable();
			CDungeonInfoWnd::GetInstance()->ResetResource();
			SetGameStatus(UPDATE_GAME_LOGIN); 				
		}		
		break;

	case UM_PORTAL_CHANGE_LAYER:
		{
			DSTC_PORTAL_JOIN_USER* pPacket = (DSTC_PORTAL_JOIN_USER*)wParam;

			ChangeLayerForPortal(pPacket);
			
			CTDS_REQUEST_SECTION_INFO_FOR_PORTAL packet;
			packet.wDungeonID	= pPacket->wDungeonID;
			packet.bLayerIndex	= pPacket->bCurLayerIndex;
			packet.bJoinDungeon = pPacket->bChangeDungeon;
			g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
			
			delete pPacket;

			g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);			
		}
		break;

	case WM_SOCKET_EVENT:	
		g_pNet->SocketEventProc();			
		break;
	
	case WM_SETCURSOR:
		break;
	case UM_SENDMESSAGE:	
		{
			WORD wLength	 = LOWORD(lParam);
			WORD wServerType = HIWORD(lParam);

			g_pNet->SendMsg((char*)wParam, wLength, (BYTE)wServerType);
		}
		break;
		
	case WM_SYSKEYDOWN:
		{
			g_dwStatusTime = timeGetTime();
			
			if(g_byStatusMessenger==1)
				g_byChkMessenger = 1;		
	
			if(GetAsyncKeyState(VK_MENU) & 0x80000)
			{
				g_bLAlt = TRUE;
				
				if (g_pThisDungeon)
					SendPKMode(true);

				if(GetAsyncKeyState(VK_F4) & 0x80000)
					g_bIsRunning = FALSE;

				if(OnKeyDown[ GetGameStatus() ])		
					(*OnKeyDown[ GetGameStatus() ])(wParam, lParam);
			}
			else if(GetAsyncKeyState(VK_F10) & 0x80000)
			{	
				if(OnKeyDown[ GetGameStatus() ])		
					(*OnKeyDown[ GetGameStatus() ])(wParam, lParam);
			}
		}
		break;
	case WM_SYSKEYUP:
		{
			int	nVirtKey = (int) wParam; 

			if (nVirtKey == VK_MENU)
			{
				g_bLAlt = FALSE;
				
				if (g_pThisDungeon)
					SendPKMode(false);
			}
			else if (nVirtKey == VK_CONTROL)
			{
				g_bLCtrl = FALSE;
			}

			if(OnKeyUp[ GetGameStatus() ])		
				(*OnKeyUp[ GetGameStatus() ])(wParam, lParam);
		}
		break;
	case WM_CHAR:			
		{
			g_dwStatusTime = timeGetTime();

			if(g_byStatusMessenger==1)
				g_byChkMessenger = 1;

			unsigned char ch = (unsigned char)wParam;
		
			if ( IsDBCSLeadByte( ch ) ) 
			{  
				MSG msg; 
				int i = 10;  
				while (!PeekMessage((LPMSG)&msg, hWnd, WM_CHAR, WM_CHAR, PM_REMOVE)) 
				{     
					if ( --i == 0 ) 
						break;
					Yield(); 
				}
				break;
			} 			

			if(OnChar[ GetGameStatus() ])			
				(*OnChar[ GetGameStatus() ])((BYTE)wParam);			
		}
		break;
	case 0x020A:			
		if(OnMouseWheel[ GetGameStatus() ])	
			(*OnMouseWheel[ GetGameStatus() ])(wParam, lParam);	
		break;

	case WM_KEYDOWN:		
		{			
			g_dwStatusTime = timeGetTime();

			if(g_byStatusMessenger==1)
				g_byChkMessenger = 1;

			switch( wParam )
			{
				case VK_UP:			g_KeyPress.bVK_UP = true;		break;
				case VK_DOWN:		g_KeyPress.bVK_DOWN = true;		break;
				case VK_LEFT:		g_KeyPress.bVK_LEFT = true;		break;
				case VK_RIGHT:		g_KeyPress.bVK_RIGHT = true;	break;
				case VK_CONTROL:	g_bLCtrl = true;				break;
				case 145:
					{
						g_bCap = TRUE;
					}
			}

			if(OnKeyDown[ GetGameStatus() ])		
				(*OnKeyDown[ GetGameStatus() ])(wParam, lParam);		
		}
		break;
	
	case WM_KEYUP:			
		{
			switch( wParam )
			{
				case VK_UP:			g_KeyPress.bVK_UP		= false;	break;
				case VK_DOWN:		g_KeyPress.bVK_DOWN		= false;	break;
				case VK_LEFT:		g_KeyPress.bVK_LEFT		= false;	break;
				case VK_RIGHT:		g_KeyPress.bVK_RIGHT	= false;	break;
				case VK_CONTROL:	g_bLCtrl				= false;	break;
			}

			if(OnKeyUp[ GetGameStatus() ])		
				(*OnKeyUp[ GetGameStatus() ])(wParam, lParam);		
		}
		break;

	case WM_LBUTTONDOWN:	
		{
			g_dwStatusTime = timeGetTime();
			
			if(g_byStatusMessenger==1)
				g_byChkMessenger = 1;
			
			g_Mouse.bLDown		= true;
			g_Mouse.MousePos.x	= LOWORD( lParam );
			g_Mouse.MousePos.y	= HIWORD( lParam );
			
			if(OnLButtonDown[ GetGameStatus() ])
			{
				(*OnLButtonDown[ GetGameStatus() ])(wParam, lParam);	
			}
		}
		break;

	case WM_LBUTTONUP:		
		{
			g_Mouse.bLDown		= false;
			g_Mouse.MousePos.x	= LOWORD( lParam );
			g_Mouse.MousePos.y	= HIWORD( lParam );
			if(OnLButtonUp[ GetGameStatus() ])
				(*OnLButtonUp[ GetGameStatus() ])(wParam, lParam);	
		}	
		break;

	case WM_RBUTTONDOWN:	
		{
			g_dwStatusTime = timeGetTime();
			
			if(g_byStatusMessenger==1)
				g_byChkMessenger = 1;
			
			g_Mouse.bRDown		= true;
			g_Mouse.MousePos.x	= LOWORD( lParam );
			g_Mouse.MousePos.y	= HIWORD( lParam );
			if(OnRButtonDown[ GetGameStatus() ])
				(*OnRButtonDown[ GetGameStatus() ])(wParam, lParam);	
		}	
		break;
	
	case WM_RBUTTONUP:		
		{
			g_Mouse.bRDown		= false;
			g_Mouse.MousePos.x	= LOWORD( lParam );
			g_Mouse.MousePos.y	= HIWORD( lParam );
			if(OnRButtonUp[ GetGameStatus() ])
				(*OnRButtonUp[ GetGameStatus() ])(wParam, lParam);	
		}	
		break;

	case WM_MBUTTONDOWN:
		{
			g_Mouse.bMDown			= true;
			g_Mouse.MouseOldPos.x	= g_Mouse.MousePos.x;
			g_Mouse.MouseOldPos.y	= g_Mouse.MousePos.y;
			g_Mouse.MousePos.x		= LOWORD( lParam );
			g_Mouse.MousePos.y		= HIWORD( lParam );
		}
		break;
	
	case WM_MBUTTONUP:
		{
			g_Mouse.bMDown		= false;
			g_Mouse.MousePos.x	= LOWORD( lParam );
			g_Mouse.MousePos.y	= HIWORD( lParam );
		}
		break;

	case WM_MOUSEMOVE:		
		{
			g_dwStatusTime = timeGetTime();
			
			if(g_byStatusMessenger==1)
				g_byChkMessenger = 1;
			
			g_Mouse.MouseOldPos.x	= g_Mouse.MousePos.x;
			g_Mouse.MouseOldPos.y	= g_Mouse.MousePos.y;

			g_Mouse.MousePos.x	= LOWORD( lParam );
			g_Mouse.MousePos.y	= HIWORD( lParam );
			
			if(OnMouseMove[ GetGameStatus() ])
				(*OnMouseMove[ GetGameStatus() ])(wParam, lParam);
		}
		break;
		
	case WM_DESTROY:
		{
			g_bIsRunning = FALSE;
		}	
		break;
		
#ifdef GM_TOOL
	case WM_MOVE:
		{
			RECT rect, rect_dlg;
			GetWindowRect(g_hMainWnd, &rect);
			GetWindowRect(g_hGMToolDlg, &rect_dlg);
			SetWindowPos(g_hGMToolDlg, HWND_NOTOPMOST, rect.right, rect.top, 
			rect_dlg.right - rect_dlg.left, rect.bottom - rect.top, SWP_NOZORDER);
		}
		break;
#endif

	case WM_TIMER:			
		if(OnTimerEvent[ GetGameStatus() ])	
			(*OnTimerEvent[ GetGameStatus() ])((DWORD)wParam);	
		break;

#if IS_KOREA_LOCALIZING()		
	case WM_IME_COMPOSITION:
		{
			if (GCS_RESULTSTR == lParam)
				OnIMEComposition(TRUE);
			else
				OnIMEComposition(FALSE);

			return DefWindowProc( hWnd, message, wParam, lParam);
		}
		break;

	case WM_IME_ENDCOMPOSITION:
		{
			return DefWindowProc( hWnd, message, wParam, lParam);
			OutputDebugString("WM_IME_ENDCOMPOSITION\n");
		}
		break;

	case WM_IME_NOTIFY:
		{
 			if(wParam == IMN_SETCONVERSIONMODE)
			{
				if(GetGameStatus() == UPDATE_GAME_LOGIN && g_pInputManager->GetCurFocusInputID() == INPUT_FOCUS_PASSWORD)		
				{
					// ·Î±×ÀÎÈ­¸éÀÌ°í, Æ÷Ä¿½º°¡ PasswordÃ¢¿¡ ¸ÂÃçÁ® ÀÖÀ»¶§´Â ÇÑ±Û·Î ÀüÈ¯ºÒ°¡!
					ChangeLanguage(FALSE);
				}
			}
		}
		break;
#endif 
	case WM_SYSCOMMAND:
		switch( wParam )
		{
		case SC_SCREENSAVE:
			return 0;
		default:
			break;
		}
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

   return 0;
}

