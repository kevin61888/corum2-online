//------------------------------------------------------------------------------
//#define PARTY_BOARD

//------------------------------------------------------------------------------
// 실제 유저에게 배포용으로 빌드하는 경우만 풀어라.
// 이게 풀려있으면 디버깅이 불가하다!!!!
// 그러나 배포용 EXE 빌드에는 반드시 풀고 할 것!!!!
//#define __RELEASE_BUILD	
//------------------------------------------------------------------------------

/*==============================================================================*/
/*						FOR DEVELOPER DEFINITIONS								*/
//#define INNER_TEST_SERVER_CONNECT
/*==============================================================================*/

/*==============================================================================*/
/*						THE GAME SYSTEM DEFINITIONS								*/
#define	EQUIP_CHK
/*==============================================================================*/


// 내부에서 팩 쓰고 싶어서 
#if defined(DEVELOP_MODE)
	#define __PACKAGE_MODE__
#endif

#if !defined(DEVELOP_MODE)
	#ifndef __PACKAGE_MODE__
		#define __PACKAGE_MODE__
	#endif
#endif

#define	__RANDOM_MEMORY_ALLOCATION			// 메인 캐릭터 생성 시, 메모리 위치 랜덤화!!
//#define	__USE_CLIENT_SPEEDHACK_CHECKER	// 클라이언트 스피드핵 체크 활성화!
//#define	__WITH_EXCEPTION_FILTER			// 크래쉬 덤프를 남기는 내부용 클라이언트 제작 시 풀어라.
//#define	__PACKET_ENCRYPTION				// 패킷 암호화를 위해서는 푼다.
//#define	__TRAFFIC_LOG					// 클라이언트의 송수신 트래픽을 기록하고 싶으면 풀어라~!
											// 1 분 동안의 트래픽을 기록한다! by deepdark~!


#pragma once


#include "Localizing.h"
#include "define.h"


#if IS_CHINA_LOCALIZING()
//#	define __SKILL_MASTERY_ENABLE	// 스킬 마스터리를 활성화할것인가? 하고 싶으면 풀어버려.
#	define __ITEM_DROPBOX_ENABLE
#	define __USE_STAT_LIMIT
#endif

#if IS_JAPAN_LOCALIZING()
	#ifndef __PACKAGE_MODE__
		#define __PACKAGE_MODE__
	#endif
	#define __USE_STAT_LIMIT
#endif


#define MODE_FOR_DEBUG				100


//#define J0_WORK_SECTION

#define DEVELOP_START_NORMAL		0
#define DEVELOP_START_WORLD			1
#define DEVELOP_START_MAP			2


struct DEVELOP_VARIABLE
{
	BYTE	bBeginType;
	BYTE	bFullScreen;
	BYTE	bRenderFramePerSec;
	WORD	wWorldPort;
	WORD	wBeginMapID;

	char szID[MAX_ID_LENGTH];
	char szPassword[MAX_PASSWORD_LENGTH];
	char szWorldIP[16];
	char szCharacterName[ MAX_CHARACTER_NAME_LENGTH ];
	char szDevIniPath[ _MAX_PATH ];
};


struct Protocol_Debug
{
	enum 
	{
		CMD_LOGIN					= 0
	};
};


#pragma pack(push,1)

//-----------------------------------------------------------------------------
// Packet: DEBUG_LOGIN
// Desc: 개발자용 LoginPacket
//-----------------------------------------------------------------------------
struct DEBUG_LOGIN
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	char	szID[ MAX_ID_LENGTH ];
	char	szPassword[ MAX_PASSWORD_LENGTH ];


	DEBUG_LOGIN()	{	bGameStatus = MODE_FOR_DEBUG;	bHeader = Protocol_Debug::CMD_LOGIN;	}
	DWORD	GetPacketSize()	{	return sizeof(DEBUG_LOGIN);	}
};

#pragma pack(pop)





















