#include "system_info.h"

int WhatKindOfPlatform()
{
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;
		
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure.
	// If that fails, try using the OSVERSIONINFO structure.

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);
	if( !bOsVersionInfoEx )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
		 return What_Kind_Of_Platform_Win32_Unknown;
	}

	switch (osvi.dwPlatformId)
	{
		// Tests for Windows NT product family.
		case VER_PLATFORM_WIN32_NT:
			return int (What_Kind_Of_Platform_Win32_Window);

		break;

		// Test for the Windows 95 product family.
		case VER_PLATFORM_WIN32_WINDOWS:
			return int (What_Kind_Of_Platform_Win32_WinNT);
		break;

		default:
			return int (What_Kind_Of_Platform_Win32_Unknown);
		break;
	}

	return int (What_Kind_Of_Platform_Win32_Unknown);
}
