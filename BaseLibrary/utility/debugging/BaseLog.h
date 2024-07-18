#pragma once

#include "BaseLogDefine.h"

extern void g_BaseLog_ErrorBox( const char* pFormat, ... );
extern void g_BaseLog_SimpleLog(const char* pFormat, ...);
extern void g_BaseLog_DetailLog(const char* pFormat, ...);
extern void g_BaseLog_Flush(void);
extern void g_BaseLog_Dump(DWORD64 dw64Offset, DWORD dwSize, BOOL bAlign);
