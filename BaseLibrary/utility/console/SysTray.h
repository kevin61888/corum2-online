// 2002년 9월 3일
// 변동호
#pragma once

#include <winsock2.h>
#include <windows.h>

#define WM_TRAYNOTIFY (WM_USER+0x40)


class CSysTray
{
private:

	HWND		m_hWnd;
    HINSTANCE	m_hInstance;

	UINT		m_uInt;
	UINT		m_uIconCount;

public:
	// constructor and destructor
	CSysTray(HWND hWnd, HINSTANCE hInstance);
	virtual ~CSysTray(void);


	BOOL AddIcon(char *pToolTip, HICON hIcon, UINT uID);
    BOOL RemoveIcon(void);

    void ShowPopupMenu(HWND hWnd, HMENU hMenu);
	void ShowPopupMenuEx(HWND hWnd, HMENU hMenu);

    UINT GetCount() { return m_uIconCount; };
};
