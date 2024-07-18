#pragma once


#include "ProtocolDefinition.h"
#include "CommonGlobalDefine.h"


#pragma pack(push,1)


//-----------------------------------------------------------------------------
// 던전이나 마을서버가 월드서버에 접속할때 쓰는 패킷 
//-----------------------------------------------------------------------------
struct DSTWS_SERVER_ATTACH
{
	WORD	wHeader;
	DWORD	dwID;		
	BYTE	bType;

	DSTWS_SERVER_ATTACH()	{	wHeader = Protocol_Server::CMD_SERVER_ATTACH;		}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_SERVER_ATTACH);	}
};


//-----------------------------------------------------------------------------
// 월드서버에서 다른 모든 서버로 종료하라는 메세지를 보냄 
//-----------------------------------------------------------------------------
struct WSTS_SERVER_TERMINATE
{
	WORD	wHeader;
	
	WSTS_SERVER_TERMINATE()	{	wHeader = Protocol_Server::CMD_SERVER_TERMINATE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_SERVER_TERMINATE);	}
};


//-----------------------------------------------------------------------------
// 던전 or 마을 서버에게 유저 접속을 허용하라는 메세지를 보낸다. 
//-----------------------------------------------------------------------------
struct WSTDS_ADD_USER_ACCEPT_ALLOWED
{
	WORD	wHeader;
	WORD	wDungeonID;		// 접속하고자 하는 해당 던전이나 마을의 ID	
	BYTE	bType;			// User Accept를 요구하는 타입, (예) 보통접속, 포탈이동으로 인한 이동.. 등등 
	DWORD	dwUserIndex;	// 캐릭터 고유의 인덱스 
	BYTE	bUserType;
	
	WSTDS_ADD_USER_ACCEPT_ALLOWED()	{	wHeader = Protocol_Server::CMD_ADD_USER_ACCEPT_ALLOWED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_ADD_USER_ACCEPT_ALLOWED);	}
};


//-----------------------------------------------------------------------------
// 월드서버에게 해당유저의 접속이 허용되었다고 알림 
//-----------------------------------------------------------------------------
struct DSTWS_USER_ACCEPT_ALLOWED
{
	WORD	wHeader;
	BYTE	bServerType;	// 서버 타입 
	WORD	wDungeonID;		// 접속을 허용한 던전 ID	
	BYTE	bType;			// User Accept를 요구하는 타입, (예) 보통접속, 포탈이동으로 인한 이동.. 등등 
	DWORD	dwUserIndex;
	
	DSTWS_USER_ACCEPT_ALLOWED()	{	wHeader = Protocol_Server::CMD_USER_ACCEPT_ALLOWED;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_USER_ACCEPT_ALLOWED);	}
};


//-----------------------------------------------------------------------------
// 월드서버와 던전서버가 접속 인증을 거치는 동안 유저의 접속이 끊어졌을경우 월드서버가 해당 던전서버에게 인증 테이블에서 그 유저를 삭제하라는 메세지
//-----------------------------------------------------------------------------
struct WSTDS_REMOVE_USER_ACCEPT_ALLOWED
{
	WORD	wHeader;
	WORD	wDungeonID;		//접속하고자 하는 해당 던전 ID	
	DWORD	dwUserIndex;	
	BYTE	m_bUserType;	// USER_TYPE_GM, USER_TYPE_NORMAL
							// GM 명령을 던전으로 바로 보내는 시스템이라 이 속성을 추가한다.
	
	WSTDS_REMOVE_USER_ACCEPT_ALLOWED()	{	wHeader = Protocol_Server::CMD_REMOVE_USER_ACCEPT_ALLOWED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_REMOVE_USER_ACCEPT_ALLOWED);	}
};


//-----------------------------------------------------------------------------
// 월드서버에게 해당유저의 접속이 허용되었다고 알림 
//-----------------------------------------------------------------------------
struct DSTWS_USER_ACCEPT_FAILED
{
	WORD	wHeader;
	BYTE	bServerType;	// 서버 타입 
	WORD	wDungeonID;		// 접속을 허용한 던전 ID	
	BYTE	bType;			// User Accept를 요구하는 타입, (예) 보통접속, 포탈이동으로 인한 이동.. 등등 
	DWORD	dwUserIndex;
	
	DSTWS_USER_ACCEPT_FAILED()	{	wHeader = Protocol_Server::CMD_USER_ACCEPT_FAILED;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_USER_ACCEPT_FAILED);	}
};


//-----------------------------------------------------------------------------
// 월드서버에게 마법진 설치 되었다고 알림.
//-----------------------------------------------------------------------------
struct DSTWS_DUNGEON_MAGICARRAY_ATTACH
{
	WORD	wHeader;
	BYTE	bServerType;	//서버 타입
	WORD	wDungeonID;
	CItem	cItem;			// 아이템

	DSTWS_DUNGEON_MAGICARRAY_ATTACH()	{	wHeader = Protocol_Server::CMD_DUNGEON_MAGICARRAY_ATTACH;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_MAGICARRAY_ATTACH);	}	
};


// 공성전 시작할 시간일때 월드서버가 던전서버에게 보내준다.
struct WSTS_START_SIEGE
{
	WORD	wHeader;
	WORD	wDungeonID;
	
	WSTS_START_SIEGE()	{	wHeader = Protocol_Server::CMD_START_SIEGE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_START_SIEGE);	}
};


// 공성전 끝날 시간일때 월드서버가 던전서버에게 보내준다.
struct WSTS_END_SIEGE
{
	WORD	wHeader;
	WORD	wDungeonID;

	WSTS_END_SIEGE()	{	wHeader = Protocol_Server::CMD_END_SIEGE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_END_SIEGE);	}
};


//-----------------------------------------------------------------------------
// 월드 서버에서 던젼서버로 파티 아이디를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_MSG
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwPartyId;
	
	WSTDS_PARTY_MSG()	{	wHeader = Protocol_Server::CMD_PARTY_MSG;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_MSG);	}
};


// 던전 주인이 바꼇다.
struct DSTWS_DUNGEON_OWNER_CHANGE
{
	WORD	wHeader;
	WORD	wDungeonID;				
	DWORD	dwOwnerIndex;
	DWORD	dwGuildId;
	char	szUserLoginID[MAX_CHARACTER_NAME_LENGTH ];
	char	szCharacterName[ MAX_CHARACTER_NAME_LENGTH ];

	DSTWS_DUNGEON_OWNER_CHANGE()	{	wHeader = Protocol_Server::CMD_DUNGEON_OWNER_CHANGE;	memset(szUserLoginID, 0, sizeof(szUserLoginID));	memset(szCharacterName, 0, sizeof(szCharacterName));	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_OWNER_CHANGE);	}
};


struct DSTWS_DUNGEON_GUARDIANITEM_ATTACH
{
	WORD	wHeader;
	BYTE	bServerType;	//서버 타입
	WORD	wDungeonID;
	CItem	cItem;			// 아이템

	DSTWS_DUNGEON_GUARDIANITEM_ATTACH()	{	wHeader = Protocol_Server::CMD_DUNGEON_GUARDIANITEM_ATTACH;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_GUARDIANITEM_ATTACH);	}	
};


// 월드에 던전 붙었을때 던전 정보를 보내줘라.
struct WSTS_DUNGEON_INFO
{
	WORD	wHeader;
	WORD	wDungeonID;
	BYTE	bSiege;		
	WORD	wUpgradeLevel;	// 던전 업그레이드된 레빌

	WSTS_DUNGEON_INFO()	{	memset(this, 0, sizeof(this)); wHeader = Protocol_Server::CMD_DUNGEON_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_DUNGEON_INFO);	}
};


// 파티원들 알아와
struct DSTWS_DUNGEON_GROUPEXP
{
	WORD	wHeader;
	BYTE	bType;			// 1 : party	2 : guild
	DWORD	dwExp;
	DWORD	dwMonsterLevel;
	DWORD	dwOwnUserIndex;

	DSTWS_DUNGEON_GROUPEXP()	{	wHeader = Protocol_Server::CMD_DUNGEON_GROUPEXP;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_GROUPEXP);	}
};


// 던전 레벨업
struct WSTS_DUNGEON_LEVELUP
{
	WORD	wHeader;
	WORD	wDungeonID;

	WSTS_DUNGEON_LEVELUP()	{	wHeader = Protocol_Server::CMD_DUNGEON_LEVELUP;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_DUNGEON_LEVELUP);	}
};


//-----------------------------------------------------------------------------
// 월드 서버가 던전서버에게 Data를 Reload 하라고 패킷을 보냄 
//-----------------------------------------------------------------------------
struct WSTS_REFRESH_ALL_DUNGEON_SERVER_DATA
{
	WORD	wHeader;

	WSTS_REFRESH_ALL_DUNGEON_SERVER_DATA()		{	wHeader = Protocol_Server::CMD_REFRESH_ALL_DS_DATA;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_REFRESH_ALL_DUNGEON_SERVER_DATA);	}
};


// 점령던전의 주인의 길드가 바꼇다.
struct WSTDS_REFRESH_DUNGEON_OWNER_GUILD
{
	WORD	wHeader;	
	WORD	wDungeonID;
	DWORD	dwGuildID;

	WSTDS_REFRESH_DUNGEON_OWNER_GUILD()	{	wHeader = Protocol_Server::CMD_REFRESH_DUNGEON_OWNER_GUILD;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_REFRESH_DUNGEON_OWNER_GUILD);	}
};


// 유저가 월드로 갈때보내는 
struct DSTWS_GOTOWORLD
{
	WORD	wHeader;
	DWORD	dwUserIndex;
	DWORD	dwMoney;
	short	wCriminalTime;
	WORD	wToDungeonID;		//월드맵에서 어떤 Dungeon 옆으로 보낼것인가 의 던전 ID
	WORD	wNowDungeonID;		//월드맵으로 나가기 전에 현재 있는 던젼 아이디  

	DSTWS_GOTOWORLD()	{	wHeader = Protocol_Server::CMD_GOTOWORLD;		}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_GOTOWORLD);	}
};


//-----------------------------------------------------------------------------
// 던전서버가 다른 던전서버에게 User Accept승인을 요청한다. (DS <-> WS <-> DS)
//-----------------------------------------------------------------------------
struct DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL
{
	WORD	wHeader;
	WORD	wFromDungeonID;
	WORD	wDungeonID;		
	BYTE	bLayerIndex;		// append by seven for GM 
	DWORD	dwTargetUserIndex;	// append by seven for GM [12/10/2004]
	BYTE	bPortalType;		//enum PORTAL_TYPE 참조 
								// append by seven [12/20/2004] 

	DWORD	dwUserIndex;
	DWORD	dwUserMoney;
	DWORD	dwUserLevel;
	BYTE	bConnectionToGameRoom;

	DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL()	
	{	
		wHeader = Protocol_Server::CMD_REQUEST_USER_ACCEPT_FOR_PORTAL;	
		dwTargetUserIndex = 0;
	}
	DWORD	GetPacketSize()	{	return sizeof(DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL);	}
};


//-----------------------------------------------------------------------------
// 포탈용 User Accept Fail 됨 
//-----------------------------------------------------------------------------
struct DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL
{
	WORD	wHeader;
	WORD	wFromDungeonID;
	DWORD	dwUserIndex;
	BYTE	bErrorCode;

	DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL()	{	wHeader = Protocol_Server::CMD_USER_ACCEPT_FAILED_FOR_PORTAL;	}
	DWORD	GetPacketSize()	{	return sizeof(DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL);	}
};


//-----------------------------------------------------------------------------
// 해당유저의 접속이 허용되었다고 알림 
//-----------------------------------------------------------------------------
struct DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL
{
	WORD	wHeader;
	WORD	wFromDungeonID;
	DWORD	dwUserIndex;
	DWORD	dwConnectIP;	//접속해야할 IP와 
	WORD	wPort;			//Port를 보내줌 

	// GM 명령시 이동할 던전의 위치를 모르기때문에 포지션 셋팅이 안되어 있을때 위치정보를 알아온다.
	BYTE	bLayerIndex;
	VECTOR2	v2Pos;	
	
	DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL()	
	{	
		wHeader = Protocol_Server::CMD_USER_ACCEPT_ALLOWED_FOR_PORTAL;	
		bLayerIndex = 0;
		v2Pos.x	= v2Pos.y = 0.f;
	}
	DWORD	GetPacketSize()	{	return sizeof(DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL);	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 파티원 레벨를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_LEVEL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwPartyId;
	DWORD	dwLevel;
	
	WSTDS_PARTY_LEVEL()	{	wHeader = Protocol_Server::CMD_PARTY_LEVEL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_LEVEL);	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 파티원 정보를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_INFO
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;	
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTDS_PARTY_INFO()	{	wHeader = Protocol_Server::CMD_PARTY_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_INFO);	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 파티원 정보를 보낸다.
//-----------------------------------------------------------------------------
struct DSTWS_PARTY_INFO
{
	WORD	wHeader;	
	WORD	wDungeonID;
	BYTE	byLayerIndex;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;	
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];
	
	DSTWS_PARTY_INFO()	{	wHeader = Protocol_Server::CMD_PARTY_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_PARTY_INFO);	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 파티원 레벨를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_HP
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwPartyId;	
	WORD	wHP;
	WORD	wMaxHp;
	
	WSTDS_PARTY_HP()	{	wHeader = Protocol_Server::CMD_PARTY_HP;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_HP);	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 포탈 메세지.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_PORTAL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	DWORD	dwPartyId;
	BYTE	byIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTDS_PARTY_PORTAL()	{	wHeader = Protocol_Server::CMD_PARTY_PORTAL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_PORTAL);	}
};

struct WSTDS_PARTY_PORTALUSED
{
	WORD	wHeader;	
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	dwResult;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTDS_PARTY_PORTALUSED()	{	wHeader = Protocol_Server::CMD_PARTY_PORTALUSED;	}
	DWORD	GetPacketSize()		{	return (sizeof(WSTDS_PARTY_PORTALUSED)); }
};


struct WSTDS_PARTY_PORTALMOVE
{
	WORD	wHeader;		
	WORD	wDungeonID;
	BYTE	byLayerIndex;
	float	fPosX;
	float	fPosZ;
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPortalUserIndex;

	WSTDS_PARTY_PORTALMOVE()	{ 	wHeader = Protocol_Server::CMD_PARTY_PORTALMOVE;}
	DWORD	GetPacketSize()		{	return (sizeof(WSTDS_PARTY_PORTALMOVE)); }
};


// 파티 설정 동기화 : 최덕석 2005.1.25
struct WSTDS_PARTY_CONFIG
{
	WORD wHeader;
	DWORD dwUserIndex;
	PARTY_CONFIG pConfig;
	BYTE bMsg;

	WSTDS_PARTY_CONFIG()	{ 	wHeader = Protocol_Server::CMD_PARTY_CONFIG;}
	DWORD	GetPacketSize()		{	return (sizeof(WSTDS_PARTY_CONFIG)); }
};



// 던전 운영모드가 바뀌었다.
struct DSTWS_DUNGEON_CHANGE_OPERATION_TYPE
{
	WORD	wHeader;
	BYTE	bOperationType;
	WORD	wDungeonID;

	DSTWS_DUNGEON_CHANGE_OPERATION_TYPE()	{	wHeader = Protocol_Server::CMD_DUNGEON_CHANGE_OPERATION_TYPE;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_CHANGE_OPERATION_TYPE);	}
};


//-----------------------------------------------------------------------------
// 월드서버에게 유저가 던전 Join에 성공했다는 메세지를 보냄 
//-----------------------------------------------------------------------------
struct DSTWS_DUNGEON_JOIN_SUCCESS
{
	WORD	wHeader;
	DWORD	dwChar_Index;
	WORD	wDungeonID;

	DSTWS_DUNGEON_JOIN_SUCCESS()	{	wHeader = Protocol_Server::CMD_DUNGEON_JOIN_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_JOIN_SUCCESS);	}
};


// 생산품 생성하라.
struct DSWST_CREATEPRODUCTION
{
	WORD	wHeader;
	WORD	wDungeonID;
	WORD	wProduction;
	BYTE	byProductionCount;
	
	DSWST_CREATEPRODUCTION()	{	wHeader = Protocol_Server::CMD_CREATEPRODUCTION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSWST_CREATEPRODUCTION);	}
};


// 생산품 획득했따.
struct DSTWS_REFRESH_PRODUCT_ACQUISITION
{
	WORD	wHeader;
	WORD	wDungeonID;
	WORD	wProduction;

	DSTWS_REFRESH_PRODUCT_ACQUISITION()	{	wHeader = Protocol_Server::CMD_REFRESH_PRODUCT_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_PRODUCT_ACQUISITION);	}
};


// 경험치 획득했따.
struct DSTWS_REFRESH_ACCEXP_ACQUISITION
{
	WORD	wHeader;
	WORD	wDungeonID;
	DWORD	dwAccExp;

	DSTWS_REFRESH_ACCEXP_ACQUISITION()	{	wHeader = Protocol_Server::CMD_REFRESH_ACCEXP_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_ACCEXP_ACQUISITION);	}
};


// 입장료 수정했따.
struct DSTWS_REFRESH_ENTRANCE_EDIT
{
	WORD	wHeader;
	WORD	wDungeonID;
	int		nMoney;

	DSTWS_REFRESH_ENTRANCE_EDIT()	{	wHeader = Protocol_Server::CMD_REFRESH_ENTRANCE_EDIT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_ENTRANCE_EDIT);	}
};


// 입장료 획득했다.
struct DSTWS_REFRESH_ACCENTRANCE_ACQUISITION
{
	WORD	wHeader;
	WORD	wDungeonID;
	int		nMoney;

	DSTWS_REFRESH_ACCENTRANCE_ACQUISITION()	{	wHeader = Protocol_Server::CMD_REFRESH_ACCENTRANCE_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_ACCENTRANCE_ACQUISITION);	}
};


struct WSTDS_PARTY_ALLPORTAL
{
	WORD	wHeader;
	DWORD	dwUserIndex;	
	DWORD	dwPartyId;	
	BYTE	byIndex;
	
	WSTDS_PARTY_ALLPORTAL()	{	wHeader = Protocol_Server::CMD_PARTY_ALLPORTAL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_PARTY_ALLPORTAL);	}
};


//-----------------------------------------------------------------------------
// 던젼 서버를 재정비 하라는 명령을 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_INIT
{
	WORD	wHeader;
	WORD	wDungeonID;
	BOOL	bTimeUp;		// 시간이 종료되어 재정비하는 것인가?

	WSTDS_EVENT_DUNGEON_INIT()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_INIT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_EVENT_DUNGEON_INIT);	}
};


//-----------------------------------------------------------------------------
// 월드서버로 이벤트 던젼의 유저가 퇴장 했음을 알린다.
//-----------------------------------------------------------------------------
struct DSTWS_EVENT_DUNGEON_USER_OUT
{
	WORD	wHeader;
	WORD	wDungeonID;		// 이벤트 던젼 아이디.

	DSTWS_EVENT_DUNGEON_USER_OUT()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_USER_OUT; }
	DWORD	GetPacketSize()			{ return sizeof(DSTWS_EVENT_DUNGEON_USER_OUT);	}
};


//-----------------------------------------------------------------------------
// Desc: 던젼 서버가 붙었을 때, 이벤트 던젼이 포함되어 있다면 이벤트 던젼 정보를 세팅하도록 한다.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_SET
{
	WORD	wHeader;
	WORD	wDungeonID;
	BYTE	m_byEventType;		// 이벤트 던젼 타입.	

	WSTDS_EVENT_DUNGEON_SET()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_SET; }
	DWORD	GetPacketSize()		{ return sizeof(WSTDS_EVENT_DUNGEON_SET); }
};


//-----------------------------------------------------------------------------
// 이벤트 던전이 유저에 의해 클리어 되었다.
//-----------------------------------------------------------------------------
struct DSTWS_EVENT_DUNGEON_CLEAR
{
	WORD	wHeader;
	WORD	wDungeonID;

	DSTWS_EVENT_DUNGEON_CLEAR()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_CLEAR; }
	DWORD	GetPacketSize()		{ return sizeof(DSTWS_EVENT_DUNGEON_CLEAR); }
};


//-----------------------------------------------------------------------------
// 월드서버에서 던전서버로 길드정보를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_INFO
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwGuildId;	
	BYTE	byType;
	BYTE	byRank;
	char	szGuildName[20];
	char	szNickName[20];	
	
	WSTDS_GUILD_INFO()		{	wHeader = Protocol_Server::CMD_GUILD_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_INFO);	}
};


//-----------------------------------------------------------------------------
// 월드서버에서 던전서버로 길드생성 실패정보를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_FAIL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;		
	BYTE	byType;	
	
	WSTDS_GUILD_FAIL()		{	wHeader = Protocol_Server::CMD_GUILD_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_FAIL);	}
};


struct WSTDS_PARTY_PORTALFAIL
{
	WORD	wHeader;
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	DWORD	dwUserIndex;

	WSTDS_PARTY_PORTALFAIL(){	wHeader = Protocol_Server::CMD_PARTY_PORTALFAIL;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTDS_PARTY_PORTALFAIL));			}
};


//-----------------------------------------------------------------------------
// World 서버가 Dungeon 서버에게 해당 User를 강제 Logout 하라고 날림 
//-----------------------------------------------------------------------------
struct WSTDS_KICK_USER
{
	WORD	wHeader;
	DWORD	dwCharIndex;

	WSTDS_KICK_USER()	{	wHeader = Protocol_Server::CMD_KICK_USER;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_KICK_USER);	}
};


//-----------------------------------------------------------------------------
// 공지 메시지를 보내시오.
//-----------------------------------------------------------------------------
struct WSTDS_NOTICE_ACTION
{
	WORD	wHeader;
	WORD	wMsgLen;
	char	szMessage[1024];

	WSTDS_NOTICE_ACTION()	{ wHeader = Protocol_Server::CMD_NOTICE_ACTION; }
	DWORD	GetPacketSize()		{  return 4 + wMsgLen; }
};


//-----------------------------------------------------------------------------
// 던젼이 클라이언트에게 뿌려줄 메시지를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_DUNGEON_MESSAGE
{
	WORD	wHeader;
	BYTE	byType;					// 타입. 확장해서 알아서 사용하시길.
	DWORD	dwMessageColor;			// 메시지의 컬러.
	WORD	wMsgLen;
	char	szMessage[1024];	

	WSTDS_DUNGEON_MESSAGE()		{ wHeader = Protocol_Server::CMD_DUNGEON_MESSAGE; }
	DWORD	GetPacketSize()		{ return 9 + wMsgLen; }
};


//-----------------------------------------------------------------------------
// 던젼서버가 살아있는지 검사한다.
//-----------------------------------------------------------------------------
struct WSTDS_REQUEST_DUNGEON_ALIVE
{
	WORD	wHeader;
	WORD	wType;					// 요청 정보 타입. 일단 지금은 사용되지 않는다.

	WSTDS_REQUEST_DUNGEON_ALIVE()	{ wHeader = Protocol_Server::CMD_REQUEST_DUNGEON_ALIVE; }
	DWORD	GetPacketSize()			{ return sizeof(WSTDS_REQUEST_DUNGEON_ALIVE); }
};


//-----------------------------------------------------------------------------
// 던젼 서버가 월드서버에게 살아 있다는 메시지를 날려줌.
//-----------------------------------------------------------------------------
struct DSTWS_DUNGEON_ALIVE_INFO
{
	WORD	wHeader;
	DWORD	dwServerID;					// 던젼 ID
	DWORD	dwConnectedUserCount;		// 해당 던젼 서버에 붙어 있는 유저 수.

	DSTWS_DUNGEON_ALIVE_INFO()			{ wHeader = Protocol_Server::CMD_DUNGEON_ALIVE_IFNO; }
	DWORD	GetPacketSize()				{ return sizeof(DSTWS_DUNGEON_ALIVE_INFO);			}
};


//-----------------------------------------------------------------------------
// 월드 서버에서 던젼서버로 길드메세지를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_MSG
{
	WORD	wHeader;	
	DWORD	dwUserIndex;	
	
	WSTDS_GUILD_MSG()	{	wHeader = Protocol_Server::CMD_GUILD_MSG;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_MSG);	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 파티원 정보를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_GUILDUSER_INFO
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;
	
	WSTDS_GUILDUSER_INFO()	{	wHeader = Protocol_Server::CMD_GUILDUSER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILDUSER_INFO);	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 길드원 정보를 보낸다.
//-----------------------------------------------------------------------------
struct DSTWS_GUILDUSER_INFO
{
	WORD	wHeader;	
	WORD	wDungeonID;
	BYTE	byLayerIndex;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;
	
	DSTWS_GUILDUSER_INFO()	{	wHeader = Protocol_Server::CMD_GUILDUSER_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_GUILDUSER_INFO);	}
};


struct WSTDS_GUILDINFO_MSG
{
	WORD	wHeader;	
	DWORD	dwUserIndex;	
	DWORD	dwGuildId;	
	BYTE	byRank;
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTDS_GUILDINFO_MSG()	{	wHeader = Protocol_Server::CMD_GUILDINFO_MSG;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILDINFO_MSG);	}
};


struct WSTDS_GUILD_LEVEL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwGuildId;
	DWORD	dwLevel;
	
	WSTDS_GUILD_LEVEL()	{	wHeader = Protocol_Server::CMD_GUILD_LEVEL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_LEVEL);	}
};


struct WSTDS_CHARACTER_SEARCH
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwSearchUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTDS_CHARACTER_SEARCH()	{	wHeader = Protocol_Server::CMD_CHARACTER_SEARCH;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTDS_CHARACTER_SEARCH);	}
};


struct WSTDS_SEARCH
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwSearchUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];		
	BYTE	byMsg;

	WSTDS_SEARCH()				{	wHeader = Protocol_Server::CMD_CHARACTER_SEARCHRT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTDS_SEARCH);	}
};


//-----------------------------------------------------------------------------
// GM_COMMAND
//-----------------------------------------------------------------------------
struct WSTDS_GM_COMMAND
{
	WORD	wHeader;

	WSTDS_GM_COMMAND()		{ wHeader = Protocol_Server::CMD_GM_COMMAND_WS_DS;	}
	DWORD	GetPacketSize()	{ return sizeof( WSTDS_GM_COMMAND );	}
};


struct DSTWS_GM_COMMAND
{
	WORD	wHeader;

	DSTWS_GM_COMMAND()		{ wHeader = Protocol_Server::CMD_GM_COMMAND_DS_WS;	}
	DWORD	GetPacketSize()	{ return sizeof( DSTWS_GM_COMMAND );	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 친구 정보를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_FRIENDUSER_INFO
{
	WORD	wHeader;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;
	
	WSTDS_FRIENDUSER_INFO()	{	wHeader = Protocol_Server::CMD_FRIENDUSER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_FRIENDUSER_INFO);	}
};


//-----------------------------------------------------------------------------
// 던젼서버에서 월드서버로 친구 정보를 보낸다.
//-----------------------------------------------------------------------------
struct DSTWS_FRIENDUSER_INFO
{
	WORD	wHeader;	
	WORD	wDungeonID;
	BYTE	byLayerIndex;	
	DWORD	dwUserIndex;
	DWORD	dwUserIndexRv;
	
	DSTWS_FRIENDUSER_INFO()	{	wHeader = Protocol_Server::CMD_FRIENDUSER_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_FRIENDUSER_INFO);	}
};


//-----------------------------------------------------------------------------
// 월드 서버가 던젼 서버에게 이벤트 던젼 오픈된 정보를 보내준다.미니맵을 던젼에서 띄우고 월드맵 위치를 표시하기 위해 사용됨.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_OPEN_INFO
{
	WORD	wHeader;
	WORD	wDungeonID;								// 던젼 ID
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH];	// 던젼 이름
	BYTE	byEventState;							// 이벤트 던젼 상태.
	DWORD	dwEventEndTick;							// 이벤트 던젼 종료 시간

	WSTDS_EVENT_DUNGEON_OPEN_INFO()		{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_OPEN_INFO; }
	DWORD	GetPacketSize()				{ return sizeof(WSTDS_EVENT_DUNGEON_OPEN_INFO); }
};

struct WSTDS_USER_PORTAL
{
	WORD	wHeader;

	BYTE	byGMCommand;			// enum GM_COMMAND 참조 
	DWORD	dwUserIndex;			// 이동할 놈 
	DWORD	dwTargetToUserIndex;	// 누군가에게 가야할 놈 
	
	DUNGEON_MOVE_INFO DMI;// : 050129 hwoarang 

	WSTDS_USER_PORTAL()		
	{	
		wHeader = Protocol_Server::CMD_USER_PORTAL;	
		dwTargetToUserIndex = 0;
	}

	DWORD	GetPacketSize()	{	return sizeof(WSTDS_USER_PORTAL);	}
};

//-----------------------------------------------------------------------------
// Event용 PC방 유저 2배 경험치 Enable 던전서버에 Broadcasting 하는 패킷 
//-----------------------------------------------------------------------------
struct WSTDS_SET_DOUBLE_EXP
{
	WORD wHeader;
	BYTE bEnable;

	WSTDS_SET_DOUBLE_EXP()	{	wHeader = Protocol_Server::CMD_SET_DOUBLE_EXP;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_SET_DOUBLE_EXP);	}
};


//-----------------------------------------------------------------------------
// Event용 마법 아이템 드럽률을 보내줘라.
//-----------------------------------------------------------------------------
struct WSTDS_SET_MAGICITEM_DROPRATE
{
	WORD wHeader;
	WORD wDropRate;

	WSTDS_SET_MAGICITEM_DROPRATE()	{	wHeader = Protocol_Server::CMD_SET_MAGICITEM_DROPRATE;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_SET_MAGICITEM_DROPRATE);	}
};


//-----------------------------------------------------------------------------
// Event용 EXP
//-----------------------------------------------------------------------------
struct WSTDS_SET_EXPGAIN
{
	WORD wHeader;
	WORD wEXP;

	WSTDS_SET_EXPGAIN()	{	wHeader = Protocol_Server::CMD_SET_EXPGAIN;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_SET_EXPGAIN);	}
};


//-----------------------------------------------------------------------------
// 길드전 리스트를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_LIST
{
	WORD	wHeader;	
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	
	WSTDS_GUILD_LIST()		{	wHeader = Protocol_Server::CMD_GUILD_LIST;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_LIST);	}
};


struct WSTDS_GUILD_ALLPORTAL
{
	WORD	wHeader;	
	DWORD	dwUserIndex;	
	DWORD	dwGuildId;	
	BYTE	byIndex;
	
	WSTDS_GUILD_ALLPORTAL()	{	wHeader = Protocol_Server::CMD_GUILD_ALLPORTAL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_ALLPORTAL);	}
};


struct WSTDS_GUILD_PORTALUSED
{
	WORD	wHeader;	
	DWORD	dwGuildId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	dwResult;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTDS_GUILD_PORTALUSED()	{	wHeader = Protocol_Server::CMD_GUILD_PORTALUSED;	}
	DWORD	GetPacketSize()		{	return (sizeof(WSTDS_GUILD_PORTALUSED)); }
};


struct WSTDS_GUILD_PORTALFAIL
{
	WORD	wHeader;		
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	DWORD	dwUserIndex;

	WSTDS_GUILD_PORTALFAIL(){	wHeader = Protocol_Server::CMD_GUILD_PORTALFAIL;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTDS_GUILD_PORTALFAIL));			}
};


struct DSTWS_GUILDWAR_POINT
{
	WORD	wHeader;		
	DWORD	dwGuildId;
	DWORD	dwGuildIdDst;
	BYTE	byPoint;
	DWORD	dwUserIndex;
	WORD	wDungeonID;
	BYTE	byLayerIndex;
	DWORD	dwUserIndexAttack;
	DWORD	dwUserIndexDefence;

	DSTWS_GUILDWAR_POINT()	{	wHeader = Protocol_Server::CMD_GULIDWAR_POINT;	}
	DWORD	GetPacketSize()	{	return (sizeof(DSTWS_GUILDWAR_POINT));			}
};


struct WSTDS_GUILDWAR_DELETE
{
	WORD	wHeader;		
	DWORD	dwGuildId;
	DWORD	dwGuildIdDst;	
	
	WSTDS_GUILDWAR_DELETE()	{	wHeader = Protocol_Server::CMD_GULIDWAR_DELETE;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTDS_GUILDWAR_DELETE));			}
};


//-----------------------------------------------------------------------------
// 길드 래더 1등에 정보를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_GUILD_RANK
{
	WORD	wHeader;	
	DWORD	dwGuildId;	
	
	WSTDS_GUILD_RANK()		{	wHeader = Protocol_Server::CMD_GUILD_RANK;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILD_RANK);	}
};


struct WSTDS_GUILDUSER_RANK
{
	WORD	wHeader;	
	DWORD	dwGuildId;	
	DWORD	dwUserIndex;
	BYTE	byGuildFlag;	
	BYTE	byChk;
	
	WSTDS_GUILDUSER_RANK()		{	wHeader = Protocol_Server::CMD_GUILDUSER_RANK;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_GUILDUSER_RANK);	}
};


struct WSTS_SERVER_ATTACH_RESULT
{
	WORD	wHeader;	
	BYTE	bAdultMode;
	DWORD	dwEXPGAIN;
	DWORD	dwMagicDropRate;
	DWORD	dwPartyBonusEXPGAIN;
	
	WSTS_SERVER_ATTACH_RESULT()	{	wHeader = Protocol_Server::CMD_SERVER_ATTACH_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTS_SERVER_ATTACH_RESULT);	}
};


//-----------------------------------------------------------------------------
// 이벤트 던젼에 유저가 포탈로 들어왔을 때, 월드서버에게 유저 카운트를 할 것을 알린다.
//-----------------------------------------------------------------------------
struct DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL
{
	WORD	wHeader;	
	WORD	wDungeonID;		// 던젼의 아이디.

	DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL() { wHeader = Protocol_Server::CMD_EVENT_DUNGEON_USER_ENTER_BY_PORTAL;	}
	DWORD	GetPacketSize()	{ return sizeof( DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL );	}
};


//-----------------------------------------------------------------------------
// 월드 서버가 던젼 서버에게 이벤트 던전 1차 이벤트가 종료되었음을 알린다.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT
{
	WORD	wHeader;
	WORD	wDungeonID;								// 던젼 ID
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH];	// 던젼 이름
	
	WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT()		{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_FIRST_TIMEOUT; }
	DWORD	GetPacketSize()					{ return sizeof(WSTDS_EVENT_DUNGEON_FIRST_TIMEOUT); }
};


//-----------------------------------------------------------------------------
// 마을에 삭제된 파티 정보를 보낸다.
//-----------------------------------------------------------------------------
struct WSTDS_PARTY_BOARD
{
	WORD	wHeader;
	DWORD	dwPartyId;
	WORD	wDungeonID;

	WSTDS_PARTY_BOARD()			{ wHeader = Protocol_Server::CMD_PARTY_BOARD; }
	DWORD	GetPacketSize()		{ return sizeof(WSTDS_PARTY_BOARD); }
};


//-----------------------------------------------------------------------------
// 월드 서버가 던젼 서버에게 이벤트 던전 조금있으면 종료될꺼라는걸 알린다.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_CLOSE 
{
	WORD	wHeader;
	WORD	wDungeonID;

	WSTDS_EVENT_DUNGEON_CLOSE()				{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_CLOSE; }
	DWORD	GetPacketSize()					{ return sizeof(WSTDS_EVENT_DUNGEON_CLOSE); }
};


//-----------------------------------------------------------------------------
// 월드 서버가 던젼 서버에게 이벤트 던젼 종료된 정보를 보내준다.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_DUNGEON_CLOSE_INFO
{
	WORD	wHeader;
	WORD	wDungeonID;								// 던젼 ID	
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH];	// 던젼 이름
	BYTE	byEventState;							// 이벤트 던젼 상태.

	WSTDS_EVENT_DUNGEON_CLOSE_INFO()	{ wHeader = Protocol_Server::CMD_EVENT_DUNGEON_CLOSE_INFO; }
	DWORD	GetPacketSize()				{ return sizeof(WSTDS_EVENT_DUNGEON_CLOSE_INFO); }
};


struct WSTDS_GUILDWAR_MESSAGE
{
	WORD	wHeader;
	char	szGuildName[MAX_GUILD_NAME_LENGTH];
	char	szGuildNameDst[MAX_GUILD_NAME_LENGTH];
	BYTE	byType;
	WORD	wDungeonID;

	WSTDS_GUILDWAR_MESSAGE()	{	wHeader = Protocol_Server::CMD_GUILDWAR_MESSAGE;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTDS_GUILDWAR_MESSAGE);				}
};


//-----------------------------------------------------------------------------
// 월드서버가 던젼서버에게 이벤트 던젼에 유저가 조인을 하면 몇명이 있는지 알려준다.
//-----------------------------------------------------------------------------
struct WSTDS_EVENT_JOINUSER_COUNT
{
	WORD	wHeader;
	WORD	wDungeonID;
	WORD	wJoinUserCount;

	WSTDS_EVENT_JOINUSER_COUNT()			{ wHeader = Protocol_Server::CMD_EVENT_JOINUSER_COUNT; }
	DWORD	GetPacketSize()					{ return sizeof(WSTDS_EVENT_JOINUSER_COUNT); }
};


// 디펜스 카운터가 갱신되었다.
struct DSTWS_REFRESH_DEFENSECOUNT
{
	WORD	wHeader;
	WORD	wDungeonID;
	BYTE	byDefenseCount;

	DSTWS_REFRESH_DEFENSECOUNT()	{	wHeader = Protocol_Server::CMD_REFRESH_DEFENSECOUNT;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_REFRESH_DEFENSECOUNT);	}
};


struct DSTWS_DUNGEON_REFRESH_OPERATIONMODE
{
	WORD	wHeader;
	BYTE	byOperationMode;
	WORD	wDungeonID;

	DSTWS_DUNGEON_REFRESH_OPERATIONMODE()	{	wHeader = Protocol_Server::CMD_REFRESH_DUNGEON_OPERATIONMODE;	}
	DWORD	GetPacketSize()	{	return sizeof(DSTWS_DUNGEON_REFRESH_OPERATIONMODE);	}	
};


struct DSTWS_CHECK_DUNGEONJOIN
{
	WORD	wHeader;	
	WORD	wDungeonID;
	char	szCharacerName[MAX_CHARACTER_NAME_LENGTH];
	DWORD	dwUserIndex;
	DWORD	dwConnectionIndex;
	
	DSTWS_CHECK_DUNGEONJOIN()		{	wHeader = Protocol_Server::CMD_CHECK_DUNGEON_JOIN;	}
	DWORD	GetPacketSize()			{	return sizeof(DSTWS_CHECK_DUNGEONJOIN);	}
};


struct WSTDS_CHECK_DUNGEONJOIN_RT
{
	WORD	wHeader;
	char	szCharacerName[MAX_CHARACTER_NAME_LENGTH];
	DWORD	dwUserIndex;
	BYTE	byChk;
	DWORD	dwConnectionIndex;

	WSTDS_CHECK_DUNGEONJOIN_RT()	{	wHeader = Protocol_Server::CMD_CHECK_DUNGEON_JOIN_RT;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTDS_CHECK_DUNGEONJOIN_RT);	}
};


struct WSTDS_CB_LEVELCHK
{
	WORD	wHeader;	
	DWORD	dwUserIndex;

	WSTDS_CB_LEVELCHK()		{	wHeader = Protocol_Server::CMD_CB_LEVELCHK;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_CB_LEVELCHK);			}
};


//-----------------------
// 내가 파티 리더야 
//-----------------------
struct WSTDS_PARTY_LEADER
{
	WORD	wHeader;
	DWORD	dwUserIndex;
	BYTE	byLeader;
	
	WSTDS_PARTY_LEADER()			{	wHeader = Protocol_Server::CMD_PARTY_LEADER;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTDS_PARTY_LEADER);				}
};


//-----------------------------------------------------------------------------
// Event용 Party Bonus EXP
//-----------------------------------------------------------------------------
struct WSTDS_SET_PARTY_BONUS_EXPGAIN
{
	WORD wHeader;
	WORD wEXP;

	WSTDS_SET_PARTY_BONUS_EXPGAIN()	{	wHeader = Protocol_Server::CMD_SET_PARTY_BONUS_EXP_GAIN;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTDS_SET_PARTY_BONUS_EXPGAIN);	}
};

typedef enum eStadiumState
{
	STADIUM_MODIFY	= 1,//상태보고
	STADIUM_END		= 2,//경기종료
	STADIUM_POINT	= 3,//점수
}STADIUM_STATE;

typedef struct tagDSTWS_STADIUM_STATE
{
	WORD	wHeader;
	
	WORD	wType;

	union
	{
		STADIUM_DATA		SD;
		STADIUM_TEAM_POINT	STP;
	}Body;

	bool bIsSuccess;

	tagDSTWS_STADIUM_STATE()	{	wHeader = Protocol_Server::CMD_STADIUM_STATE;	}
	DWORD	GetPacketSize()	
	{
		const size_t base_size = sizeof(*this) - sizeof(Body);
		switch(wType)
		{
		case STADIUM_MODIFY://상태보고
		case STADIUM_END://경기종료
			{
				return base_size + sizeof(STADIUM_DATA) + sizeof(bool);
			}break;
		case STADIUM_POINT://점수
			{
				return base_size + sizeof(STADIUM_TEAM_POINT);
			}break;
		}
		return sizeof(*this);	
	}
}DSTWS_STADIUM_STATE;

// 팀매치 시작. // : hwoarang 050202
typedef struct tagWSTDS_PREPARE_TEAM_MATCH
{
	FLAG_TYPE	match_type;
	FLAG_TYPE	match_level;
	FLAG_TYPE	player_count;
	FLAG_TYPE	team_count;
	GUILD_WAR_TIME	match_time;

	DWORD		dwExFlag;//확장타입
}WSTDS_PREPARE_TEAM_MATCH;

typedef struct tagWSTDS_REG_PLAYER
{
	char		szPlayerName[MAX_CHARACTER_NAME_LENGTH];
	FLAG_TYPE	team_index;

	DWORD		dwExFlag;//확장타입
}WSTDS_REG_PLAYER;

typedef enum eWSTDS_TEAM_MATCH
{
	CMD_WSTDS_PREPARE_MATCH = 1,//길드전 준비해라
	CMD_WSTDS_REG_PLAYER	= 2,//선수 추가
	CMD_WSTDS_UN_REG_PLAYER = 3,
	CMD_WSTDS_END_MATCH		= 4,//길드전 끝내라
}WSTDS_TEAM_MATCH_PACKET_TYPE;

typedef struct tagWSTDS_TEAM_MATCH
{
	WORD		wHeader;

	WORD		wType;

	WORD		wDungeonID;
	BYTE		byLayerIndex;

	union
	{
		WSTDS_PREPARE_TEAM_MATCH	PTM;
		WSTDS_REG_PLAYER			RP;
	}Body;
	
	tagWSTDS_TEAM_MATCH()	{	wHeader = Protocol_Server::CMD_TEAM_MATCH;	}
	DWORD	GetPacketSize()	
	{
		const size_t base_size = sizeof(*this) - sizeof(Body);
		switch( wType)
		{
		case CMD_WSTDS_PREPARE_MATCH:
			{
				return base_size + sizeof(Body.PTM);
			}break;
		case CMD_WSTDS_REG_PLAYER:
			{
				return base_size + sizeof(Body.RP);
			}
		case CMD_WSTDS_END_MATCH:
			{
				return base_size;
			}break;
		} // switch( wType)

		return sizeof(*this);
	}

}WSTDS_TEAM_MATCH;

#pragma pack(pop)