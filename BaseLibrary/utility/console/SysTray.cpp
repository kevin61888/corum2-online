#include "../../StdAfx.h"
#include "SysTray.h"


// constructor and destructor
CSysTray::CSysTray(HWND hWnd, HINSTANCE hInstance) : m_hWnd(hWnd), m_hInstance(hInstance), m_uInt(0), m_uIconCount(0)
{
	//DETLOG1(g_Log, "this:0x%08x", this);		// TODO : 로우레벨 로그로 출력할 것.
}

CSysTray::~CSysTray(void)
{
	if( m_uIconCount > 0 )
		RemoveIcon();
	//DETLOG1(g_Log, "this:0x%08x", this);		// TODO : 로우레벨 로그로 출력할 것.
}

// add item
BOOL CSysTray::AddIcon(char *pToolTip, HICON hIcon, UINT uID)
{
	BOOL res; 
	m_uInt	= uID;

	NOTIFYICONDATA tnid; 
	tnid.cbSize	= sizeof(NOTIFYICONDATA); 
	tnid.hWnd	= m_hWnd;
	tnid.uID	= uID;
	tnid.uFlags	= NIF_MESSAGE|NIF_ICON|NIF_TIP; 
	tnid.hIcon	= hIcon;
	tnid.uCallbackMessage	= WM_TRAYNOTIFY; 
	if (pToolTip)
		::_snprintf(tnid.szTip, 64, "%s", pToolTip);
	else
		::_snprintf(tnid.szTip, 64, "Application");

	res = Shell_NotifyIcon(NIM_ADD, &tnid); 

	DestroyIcon(tnid.hIcon); 

	if (res) {
		m_uIconCount++;
	}

	return res;
}

// remove item
BOOL CSysTray::RemoveIcon(void)
{
    BOOL res;
    NOTIFYICONDATA tnid;

    //fill in the structure
    tnid.cbSize = sizeof(NOTIFYICONDATA);
    tnid.hWnd	= m_hWnd;
    tnid.uID	= m_uInt;

    //give the notify command
    res = Shell_NotifyIcon(NIM_DELETE, &tnid);

    //decrement the counter
	if (res) {
        m_uIconCount--;
	}

    return res;
}

// show popup menu
void CSysTray::ShowPopupMenu(HWND hWnd, HMENU hMenu)
{
    POINT mouse;
    GetCursorPos(&mouse);

    TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, mouse.x, mouse.y, 0, hWnd, NULL);
    DestroyMenu(hMenu);
}

// show popup menu
void CSysTray::ShowPopupMenuEx(HWND hWnd, HMENU hMenu)
{
    POINT mouse;
    GetCursorPos(&mouse);

    TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, mouse.x, mouse.y, 0, hWnd, NULL);
}
