// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once

#if _MSC_VER <= 1200
#pragma warning (disable: 4786)
#endif

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

////#define GC_REDIRECT_TO_LOCAL
//#ifdef _USE_GC
//	#include <gc_cpp.h>
//	#define USE_STD_ALLOCATOR
//	#ifdef USE_STD_ALLOCATOR
//	#   include "gc_allocator.h"
//	#elif __GNUC__
//	#   include "new_gc_alloc.h"
//	#else
//	#   include "gc_alloc.h"
//	#endif
//	extern "C" {
//	#include "private/gc_priv.h"
//	}
//#endif

#include <winsock2.h>

//#ifdef GC_NAME_CONFLICT
//#   define USE_GC UseGC
//    struct foo * GC;
//#else
//#   define USE_GC GC
//#endif

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <windows.h>
#include <ShellApi.h>
#include <stdio.h>
#include <stdlib.h>				// _tsplitpath 가 정의
#include <tchar.h>

#include <assert.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>

//#ifdef _DEBUG_MEM
//	#include <MemoryMgr/StdAfx.h>
//	#include <MemoryMgr/mmgr.h>
//#endif

#include <BaseLibrary/Utility/Macro/CommonMacros.h>
#include <BaseLibrary/Utility/Macro/DebugMacros.h>		// #pragma todo() 를 사용하기 위해 정의
#include <BaseLibrary/Utility/Debugging/BaseLog.h>
