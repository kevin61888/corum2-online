#ifndef _SYSTEM_INFO_H_
#define _SYSTEM_INFO_H_

#include "windows.h"


enum WHAT_KIND_OF_PLATFORM
{
	What_Kind_Of_Platform_Win32_Unknown = 0,
	What_Kind_Of_Platform_Win32_Window,			// Windows 95 product family.
	What_Kind_Of_Platform_Win32_WinNT,			// for Windows NT product family.
};

int WhatKindOfPlatform();


#endif
//_SYSTEM_INFO_H_
