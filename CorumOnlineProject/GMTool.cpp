#include "GMTool.h"
#include <commctrl.h>
#include "InitGame.h"
#include "resource.h"
#include "NetworkClient.h"

HWND				g_hGMToolDlg	= NULL;
HWND				g_hGMToolTab1	= NULL;
LPGlobalVariable_GM g_pGVGm			= NULL;

void SendMsgByMainThread(char* szMsg, WORD dwLength, WORD wServerType )
{
	DWORD dwParam2 = MAKELONG(dwLength, wServerType);
	SendMessage(g_hMainWnd, UM_SENDMESSAGE, (WPARAM)szMsg, (LPARAM)dwParam2);
}

INT_PTR CALLBACK GMToolTab1(HWND hdlg,  UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{

		}
		break;

	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_CHECK_QUICKMOVE:
				{
					if(g_pGVGm->bQuickMove)
						g_pGVGm->bQuickMove = 0;
					else
						g_pGVGm->bQuickMove = 1;
				}
				break;
				
			case IDC_CHECK_INVISIBLE:
				{
					if(g_pGVGm->bInvisibleMode)
						g_pGVGm->bInvisibleMode = 0;
					else
						g_pGVGm->bInvisibleMode = 1;
				}
				break;

			case IDC_NOTICE_SEND:
				{
					char szMsg[1024];
					memset(szMsg, 0, 1024);
					GetDlgItemText(hdlg, IDC_NOTICE, szMsg, 1024);

					CTWS_GM_NOTICE packet;
					memset(packet.szMsg, 0, 1024);

					packet.wMsgLen = WORD(lstrlen(szMsg) + 1);
				
					__lstrcpyn(packet.szMsg, szMsg, packet.wMsgLen);
					SendMsgByMainThread((char*)&packet, (WORD)packet.GetPacketSize(), SERVER_INDEX_WORLD);
				}
				break;
			}
		}
		break;
	}
	return 0l;
}

INT_PTR CALLBACK GMDialog(HWND hdlg,  UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		{
			RECT rect, rect_dlg;
			GetWindowRect(g_hMainWnd, &rect);
			GetWindowRect(hdlg, &rect_dlg);
			SetWindowPos(hdlg, HWND_NOTOPMOST, rect.right, rect.top, 
				rect_dlg.right - rect_dlg.left, rect.bottom - rect.top, SWP_NOZORDER);
			
			//--------------------  Tab Control 초기화  ------------------------
			TC_ITEM		item;
			HWND		hTabWnd = GetDlgItem(hdlg, IDC_GM_TAB);
			char*		itemlist[4] = {"Tab 1", "Tab 2", "Tab 3", "Tab 4"};
			
			for(int i=0; i<4; i++ )
			{
				item.mask = TCIF_TEXT;
				item.pszText = itemlist[i];
				::SendMessage(hTabWnd, TCM_INSERTITEM, i, (LPARAM)&item);
			}
			//------------------------------------------------------------------	
			

			//---------------  GMTool Dialog 사이즈에 Tab Ctrl 맞춤 ----------------
			RECT WindowRect, TabRect;//,ItemRect;	//TabRect은 탭 컨트롤 전체 영역, ItemRect은 위에 탭부분만..
		
			GetClientRect(hdlg, &WindowRect);
			GetClientRect(hTabWnd, &TabRect);
			//SendMessage(hTabWnd, TCM_GETITEMRECT, 0, (LPARAM)&ItemRect);
			
			int left, top, width, height;
			left = WindowRect.left + 1;
			top = WindowRect.top + 1;			// + 30/*Tab Control위에 빈칸 남겨놓은 만큼*/;
			width = WindowRect.right - WindowRect.left - 2;
			height = WindowRect.bottom - WindowRect.top - 2;

			::MoveWindow(hTabWnd, left, top, width, height, TRUE);
			//----------------------------------------------------------------------
		}
		break;
	case WM_LBUTTONDOWN:
		{
			WORD wX	= LOWORD( lParam );
			WORD wY = HIWORD( lParam );
			
			::PostMessage(hdlg, WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( wX, wY ) );
		}
		break;
	}
	return 0l;
}