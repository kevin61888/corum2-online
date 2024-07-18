#if !defined(AFX_STDAFX_H__89C3FB5E_8F46_4453_94E6_61EADBBCB310__INCLUDED_)
#define AFX_STDAFX_H__89C3FB5E_8F46_4453_94E6_61EADBBCB310__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT 0x0500


#include <afx.h>
#include <winsock2.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <ole2.h>
#include <initguid.h>
#include <mstcpip.h>


#include "GlobalDefine.h"
#include "../CommonServer/CommonAccessory.h"
#include "../CommonServer/ComInterface.h"
#include "../CommonServer/WorldPacket.h"
#include "../CommonServer/ServerPacket.h"
#include "../CommonServer/CharSelectPacket.h"
#include "../CommonServer/AgentServerPacket.h"
#include "../CommonServer/RivalGuildWarPacket.h"

#include "../CommonServer/stlsupport.h"

#include "../CommonServer/NTNetwork.h"


extern BYTE GAME_DB;
extern BYTE TOTAL_DB;
extern BYTE MEMBER_DB;
extern BYTE LOG_DB;
extern CNTNetWork	* g_pNet;
extern IOleDBThread	* g_pDb;
extern IOleDBThread	* g_pLogDb;

extern DWORD g_dwCurTick;


#pragma warning(disable : 4100)		// 사용되지 않은 함수 인자 경고
#pragma warning(disable : 4702)		// 접근할수 없는 코드 워닝 


#include <mmsystem.h>
#pragma comment (lib, "Winmm.lib")

//로컬 매니저 적용 : 050104 hwoarang
#include <cassert>
#include <string>
using namespace std;

#include "LocalMgr.h"
extern CLocal		g_Local;
extern CDBInfoMgr	g_DBInfo;
extern string CURRENT_VERSION_STRING;

#define IS_ABLE_NATION(nation_code)			(g_Local.IsAbleNation(nation_code)) 
#define IS_ABLE_SERVICE_TYPE(dwFlag)		(g_Local.IsAbleServiceType(dwFlag)) 
#define MY_NATION_CODE						(g_Local.NationCode()) 

#endif 
