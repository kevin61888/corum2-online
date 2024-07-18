#pragma once

#define NEW_COMMAND


// Windows Header Files:
#include <windows.h>
#include <windowsx.h>
#include <ole2.h>
#include <initguid.h>
#include <mmsystem.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <basetsd.h>
#include <ddraw.h>
#include <dmusici.h>
#include <TIME.H>
#include <ASSERT.H>

#include "StringFunc.h"
#include "oledb.h"
#include "../CommonServer/LoginPacket.h"
#include "../CommonServer/CharSelectPacket.h"
#include "../CommonServer/DungeonPacket.h"
#include "../CommonServer/GamePacket.h"
#include "../CommonServer/GMPacket.h"
#include "../CommonServer/RivalGuildWarPacket.h"
#include "../CommonServer/WorldPacket.h"
#include "../CommonServer/PartyMatchPacket.h"


#define FILE(id)		g_Res.GetResFileNameOnly(##id)	
#define SPR(id)			g_pSprManager->GetSprite(##id)
#define	RELEASE_SPR(id)	g_pSprManager->RemoveSprite(##id)
#define DIRECTINPUT_VERSION 0x0800


#pragma warning(disable : 4706)		// 비교구문 내에서 변수 값 할당 경고.
#pragma warning(disable : 4505)		// 사용되지 않은 static 함수 제거 경고
#pragma warning(disable : 4710)		// 릴리즈 빌드 시, inline 되지 않음 경고
#pragma warning(disable : 4100)		// 사용되지 않은 함수 인자 경고

// 아래 워닝은 가급적 없애라... 실수하면 대박 버그 가능성이 항상 존재한다....!!!
#pragma warning(disable : 4244)		// 타입 불일치 경고
#pragma warning(disable : 4018)		// 비교 구문 부호 불일치 경고
//#pragma warning(disable : 4245)		// 대입문 부호 불일치 경고


#include <mmsystem.h>
#pragma comment (lib, "Winmm.lib")
