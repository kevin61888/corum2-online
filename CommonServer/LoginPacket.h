#pragma once


#include "ProtocolDefinition.h"
#include "Item.h"

#pragma pack(push,1)

enum NATIONAL_CODE
{
	National_Code_Korea,
	National_Code_Japan,
	National_Code_China,
	National_Code_Taiwan,
};

//-----------------------------------------------------------------------------
// Client에서 서버에 Login을 시도한다.(본섭용)
//-----------------------------------------------------------------------------
struct CTWS_LOGIN : public LOGIN_PACKET_HEADER
{
	BYTE	bySigniture[4];
	BYTE	byNationCode;
	DWORD	dwVersion;
	char	szID[MAX_ID_LENGTH];
	char	szPassword[MAX_PASSWORD_LENGTH];

#ifdef __PACKET_ENCRYPTION
	BYTE	byClientKey[10];
#endif

	CTWS_LOGIN()			
	{	
		bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_LOGIN;	
		bySigniture[0] = '@'; bySigniture[1] = 'S'; bySigniture[2] = 'A'; bySigniture[3] = 'D';	

		byNationCode = 
#if defined JAPAN_LOCALIZING
			National_Code_Japan;
#elif defined TAIWAN_LOCALIZING  			
			National_Code_Taiwan;
#elif defined CHINA_LOCALIZING  
			National_Code_China;
#elif defined KOREA_LOCALIZING 
			National_Code_Korea;
#else
			National_Code_Korea;
#endif			
	}

	DWORD	GetPacketSize()	{	return sizeof(CTWS_LOGIN);	}
};


//-----------------------------------------------------------------------------
// Client에서 서버에 Login을 시도한다.(넷마블용)
//-----------------------------------------------------------------------------
struct CTWS_NETMARBLE_LOGIN : public LOGIN_PACKET_HEADER
{
	BYTE	bySigniture[4];
	DWORD	dwVersion;
	BYTE	bNetmarleLogin;
	BYTE	byNationCode;

	char	szID[MAX_ID_LENGTH];
	char	szPassword[MAX_PASSWORD_LENGTH];
	char	szCPCookie[MAX_CP_COOKIE_LENGTH];

#ifdef __PACKET_ENCRYPTION
	BYTE	byClientKey[10];
#endif

	CTWS_NETMARBLE_LOGIN()			
	{	
		bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_LOGIN_NETMARBLE;	
		memset(szID, 0, MAX_ID_LENGTH);
		memset(szPassword, 0, MAX_PASSWORD_LENGTH);
		bySigniture[0] = '@'; bySigniture[1] = 'S'; bySigniture[2] = 'A'; bySigniture[3] = 'D';
		byNationCode = National_Code_Korea;
	}

	DWORD	GetPacketSize()	{	return sizeof(CTWS_NETMARBLE_LOGIN);	}
};


//-----------------------------------------------------------------------------
// Client에게 Login에 실패했다고 알려준다. 
//-----------------------------------------------------------------------------
struct WSTC_LOGIN_FAIL : public LOGIN_PACKET_HEADER
{
	BYTE	bResult;	//0: 비번틀림, 2: 없는아이디 , 3: 아이디사용중, 4: 서버 최대인원 초과, 5: 테스트 기간에 접속할 수 없는 아이디, 
						//6: 버전틀림, 7: 현재 서버에서 로그인 시도중, 8: 캐릭터 자리 겹침 
						//1: 로그인 성공(사용안함)
	DWORD	dwExData;	//기타 정보 데이터 

	WSTC_LOGIN_FAIL()	{	bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_LOGIN_FAIL;	dwExData = 0; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_LOGIN_FAIL);	}
};


struct RS_CharacterLogInfo
{
	DWORD	dwCharacterIndex;						// DB의 CharacterIndex
	char	szName[ MAX_CHARACTER_NAME_LENGTH ];	// 캐릭터 이름 
	WORD	wChrNum;								// 캐릭터 번호 (1 ~ 4)
	WORD	wHead;
	WORD	wClass;									// 클래스 
	WORD	wGrade;									// 그레이드 100이면 GM
	DWORD	dwLevel;								// 레벨 
	DWORD	dwExp;									// 경험치 
	DWORD	dwEgo;									// EGO
	DWORD	dwStr;									// STR
	DWORD	dwVit;									// VIT
	DWORD	dwDex;									// DEX
	DWORD	dwInt;									// INT		
	DWORD	dwHonor;								// 명성
	DWORD	dwLuck;									// 행운
	DWORD	dwRecentWorldmapID;						// 최근 종료한 WorldMap ID
	WORD	wLHandID;			
	WORD	wRHandID;
	WORD	wHelmetID;
	WORD	wMantleID;								// 망또
	WORD	wMailID;								// 갑옷	
};


//-----------------------------------------------------------------------------
// Login에 성공했을때 4개의 캐릭터 정보와 함께 보내준다. 
//-----------------------------------------------------------------------------
struct WSTC_LOGIN_SUCCESS : public LOGIN_PACKET_HEADER
{
	BYTE				bAdultMode;		// 성인모드
	BYTE				bNum;			// 캐릭터 갯수 
	RS_CharacterLogInfo	Character[4];

	WSTC_LOGIN_SUCCESS()	
	{	
		memset(Character, 0, sizeof(Character));
		bGameStatus = UPDATE_GAME_LOGIN;	
		bHeader = Protocol_Login::CMD_LOGIN_SUCCESS;	
	}

	DWORD	GetPacketSize()	{	return 4 + sizeof(RS_CharacterLogInfo)*bNum;	}	
};


//-----------------------------------------------------------------------------
// 서버들의 생존 정보를 보내주시오.
//-----------------------------------------------------------------------------
struct CTAS_REQUEST_SERVER_ALIVE : public LOGIN_PACKET_HEADER
{
	BYTE	bExpansion;

	CTAS_REQUEST_SERVER_ALIVE()		{ bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_REQUEST_SERVER_ALIVE;	}
	DWORD	GetPacketSize()			{ return sizeof(CTAS_REQUEST_SERVER_ALIVE); }
};


//-----------------------------------------------------------------------------
// 클라이언트에게 서버들의 생존 정보를 보낸다.
//-----------------------------------------------------------------------------
struct ASTC_SERVER_ALIVE_INFO : public LOGIN_PACKET_HEADER
{
	DWORD	dwServerID;				// Server ID.
	WORD	wServerType;			// 0 : World server, 1 : dungeon.
	WORD	wConnectedUserCount;	// 유저 접속 수치.
	DWORD	dwMagicItemDropRate;	// 마법 아이템 드롭률.
	DWORD	dwExpGainRate;			// 경험치 획득률.

	ASTC_SERVER_ALIVE_INFO()		{ bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_SERVER_ALIVE_INFO;	}
	DWORD	GetPacketSize()			{ return sizeof(ASTC_SERVER_ALIVE_INFO); }
};


//-----------------------------------------------------------------------------
// 내가 모니터링 툴인지 체크해 주시오.
//-----------------------------------------------------------------------------
struct CTAS_CHECK_MONITORING : public LOGIN_PACKET_HEADER
{
	BYTE	bExpansion;

	CTAS_CHECK_MONITORING()		{ bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_CHECK_MONITORING;	}
	DWORD	GetPacketSize()		{ return sizeof(CTAS_CHECK_MONITORING); }
};


//-----------------------------------------------------------------------------
// CMTool 에서의 공지사항 패킷이 왔다.  
//-----------------------------------------------------------------------------
struct CTAS_CMTOOL_NOTICE : public LOGIN_PACKET_HEADER
{
	WORD	wMsgLen;
	char	szMsg[1024];
	
	CTAS_CMTOOL_NOTICE()			{ bGameStatus = UPDATE_GAME_LOGIN; bHeader = Protocol_Login::CMD_CMTOOL_NOTICE; }
	DWORD	GetPacketSize()			{ return 4 + wMsgLen; }
};


//-----------------------------------------------------------------------------
// 현재 유저의 접속 여부를 파악한다.
//-----------------------------------------------------------------------------
struct CTAS_IS_ALIVE_USER : public LOGIN_PACKET_HEADER
{
	char	szID[MAX_ID_LENGTH];
	DWORD	Kind;
	
	CTAS_IS_ALIVE_USER()
	{
		bGameStatus = UPDATE_GAME_LOGIN;
		bHeader = Protocol_Login::CMD_IS_ALIVE_USER;
		memset(szID, 0, MAX_ID_LENGTH);
	}
	
	DWORD	GetPacketSize()			{ return sizeof(CTAS_IS_ALIVE_USER); }
};


//-----------------------------------------------------------------------------
// 현재 유저의 접속 여부를 파악해서 클라이언트로 보내준다.
//-----------------------------------------------------------------------------
struct ASTC_IS_ALIVE_USER : public LOGIN_PACKET_HEADER
{
	BOOL	bIsAliveUser;

	ASTC_IS_ALIVE_USER()			{ bGameStatus = UPDATE_GAME_LOGIN; bHeader = Protocol_Login::CMD_ANSWER_ALIVE_USER; }
	DWORD	GetPacketSize()			{ return sizeof(ASTC_IS_ALIVE_USER); }
};


//-----------------------------------------------------------------------------
// CM툴에서 로그인서버에게 해당 User를 강제 Logout 하라고 날림 
//-----------------------------------------------------------------------------
struct CTAS_KICK_USER : public LOGIN_PACKET_HEADER
{
	DWORD	dwCharIndex;
	DWORD	dwPropID;
	
	CTAS_KICK_USER()		{	bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_CM_KICK_USER;		}
	DWORD	GetPacketSize()	{	return sizeof(CTAS_KICK_USER);	}
};


//-----------------------------------------------------------------------------
// Client에게 제재중인 계정이라 로그인 실패 되었다고 알려준다. 
//-----------------------------------------------------------------------------
struct WSTC_BLOCK_ACCOUNT : public LOGIN_PACKET_HEADER
{
	BYTE	bLen;
	char	szReason[0xff];	

	WSTC_BLOCK_ACCOUNT()	{	bGameStatus = UPDATE_GAME_LOGIN;	bHeader = Protocol_Login::CMD_LOGIN_FAIL_WITH_REASON; memset(szReason, 0, 0xff);}
	DWORD	GetPacketSize()	{	return 3 + bLen;	}
};


//-----------------------------------------------------------------------------
// 중국 빌링 시, 빌링 인증 성공하면 남은 시간/기간의 정보를 클라이언트에게 전송 
//-----------------------------------------------------------------------------
struct WSTC_CHINA_BILL_REMAIN_TIME : public LOGIN_PACKET_HEADER
{
	BYTE				btTimeType;
	DWORD				dwTimeValue;

	WSTC_CHINA_BILL_REMAIN_TIME()
	{
		bGameStatus = UPDATE_GAME_LOGIN;
		bHeader		= Protocol_Login::CMD_CHINA_BILL_REMAINED_TIME;
		btTimeType	= 0xff;
		dwTimeValue = 0;
	}

	void	SetTimeType(BYTE btType){ btTimeType = btType; }
	void	SetTime(DWORD dwTime)	{ dwTimeValue = dwTime; }
	BYTE	GetTimeType()			{ return btTimeType; }
	DWORD	GetTime()				{ return dwTimeValue; }

	DWORD	GetPacketSize()			{ return sizeof(WSTC_CHINA_BILL_REMAIN_TIME); }
};


//-----------------------------------------------------------------------------
// 클라이언트에게 서버 파트 키를 전송.
//-----------------------------------------------------------------------------
struct WSTC_ENCRYPTION_KEY : public LOGIN_PACKET_HEADER
{
	BYTE				byServerKey[10];

	WSTC_ENCRYPTION_KEY()
	{
		bGameStatus = UPDATE_GAME_LOGIN;
		bHeader		= Protocol_Login::CMD_ENCRYPTION_KEY;
	}

	LPBYTE	GetKey()				{ return byServerKey;	}
	void	SetKey(LPBYTE btKey)	{ CopyMemory(byServerKey, btKey, 10);	}

	DWORD	GetPacketSize()			{ return sizeof(WSTC_ENCRYPTION_KEY); }
};


#pragma pack(pop)