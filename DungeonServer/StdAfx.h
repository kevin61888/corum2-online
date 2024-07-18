#if !defined(AFX_STDAFX_H__71751A48_D7EF_4A0E_814A_24507D90D4D1__INCLUDED_)
#define AFX_STDAFX_H__71751A48_D7EF_4A0E_814A_24507D90D4D1__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 
#define WIN32_LEAN_AND_MEAN//windwos 헤더 넣어도 문제 없도록

#define _WIN32_WINNT 0x0500


#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <ASSERT.H>
#include <CRTDBG.H>
#include "GlobalDefine.h"
#include "TrafficLog.h"


#pragma comment(lib, "SS3DGFunc.lib")
#pragma comment(lib, "CommonServer.lib")


#pragma warning(disable : 4100) // 사용되지 않은 함수 인자 경고
#pragma warning(disable : 4706)	// 비교 구문 내에서 값 대입 경고
#pragma warning(disable : 4710)	// 릴리즈 빌드 시, inline 되지 않음 경고
#pragma warning(disable : 4702) // 접근할수 없는 코드 워닝
// 아래 워닝은 가급적 없애라... 실수하면 대박 버그 가능성이 항상 존재한다....!!!
#pragma warning(disable : 4244)	// 타입 불일치 경고... 절대 disable 하지 말고 일일히 확인하고 캐스트해라!!
#pragma warning(disable : 4127)

#include "../4DyuchiGXGFunc/global.h"
#include "../CommonServer/CommonAccessory.h"
#include "../CommonServer/ComInterface.h"
#include "../CommonServer/GamePacket.h"
#include "../CommonServer/DungeonPacket.h"
#include "../CommonServer/ServerPacket.h"
#include "../CommonServer/RivalGuildWarPacket.h"
#include "../CommonServer/GMPacket.h"
#include "../CommonServer/NTNetwork.h"

#include <mmsystem.h>
#pragma comment (lib, "Winmm.lib")

#ifdef _DEBUG
#	define CHECK_MEMORY()			if( !_CrtCheckMemory( ) ) __asm int 3
#else
#	define CHECK_MEMORY()
#endif


#endif 





