#pragma once


#include <TIME.H>
#include "ProtocolDefinition.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_common\\math.inl"
#else
#include "./../4DyuchiGRX_common\\math.inl"
#endif
#include "GM_Command.h"
#include "Item.h"


#pragma pack(push,1)


//-----------------------------------------------------------------------------
// 선택한 던전정보를 쿼리 한다. 
//-----------------------------------------------------------------------------
struct CTWS_REQUEST_DUNGEON_INFO : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;

	CTWS_REQUEST_DUNGEON_INFO()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REQUEST_DUNGEON_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_REQUEST_DUNGEON_INFO);	}
};



//-----------------------------------------------------------------------------
// Client의 던전정보 요구에 응답함 (일반던전)
//-----------------------------------------------------------------------------
struct WSTC_REPLY_DUNGEON_INFO : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	WORD	wGroupID;
	char	szDungeonName[ MAX_DUNGEON_NAME_LENGTH ];
	WORD	wAreaNum;
	WORD	wAreaLevel_h;								//0 = 왕초보존, 1 = 초보존, 2 = 중수존, 3 = 고수존, 4 = 초절정고수존
	WORD	wAreaLevel_l;
	WORD	wInitLevel;									// 던전 기본 레벨
	WORD	wUpgradeLevel;								// 던전 업그레이드된 레빌
	WORD	wBattleTimeLimit;							// 공성전 제한 시간.
	WORD	wProduction;								// 생산품 아이디.
	BYTE	byProductionCount;
	BYTE	bSiege;		
	BYTE	bOperationType;								// 던전 운영 상태..0 = 노멀모드, 1 = 방어모드, 2 = 산출모드, 3 = 레벨업모드
	WORD	wLayerCount;
	DWORD	dwOwnerIndex;
	DWORD	dwOwnerGuildNum;
	CItem	cMagicArrayItem;							// 마법진 아이템.
	DWORD	dwAccEntrance;								// 누적 입장료
	DWORD	dwEntrance;									// 입장료
	DWORD	dwAccExp;									// 누적 경험치
	CItem	m_cGuardianItem;							// 가디언 아이템
	BYTE	byDefenseCount;
	BYTE	byProperty;
	BYTE	byEntererMinLevel;							// 들어올놈의 최소 레벨
	BYTE	byEntererMaxLevel;							// 들어올놈의 최대 레벨
	DWORD	dwRemainSiegeStartTick;						// 공성전 할려면 몇시간 남았는가?
	DWORD	dwRemainSiegeEndTick;						// 공성전 방어하기 몇시간 남았는가?
	DWORD	dwRemainLevelStartTick;						// 던전 레벨 (시간이 지남에 따라 레벨이 증가한다.)
	
	char	szOwner[ MAX_CHARACTER_NAME_LENGTH ];		//소유주 또는 소유길드명 	
	char    szManager[ MAX_CHARACTER_NAME_LENGTH ];		//관리자 캐릭터명..  길드소유인경우 디폴드는 길마
	
	char	szSchoolName[MAX_CHARACTER_NAME_LENGTH];	// 일본판의 학교길드 이름을 위한 정보.

	BYTE	bMemoLength;
	char	szMemo[ MAX_DUNGEON_MEMO_LENGTH ];			//던전 관리자의 메모 
	WSTC_REPLY_DUNGEON_INFO(){	memset(this, 0, sizeof(this));	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REPLY_DUNGEON_INFO;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_REPLY_DUNGEON_INFO) - (MAX_DUNGEON_MEMO_LENGTH - bMemoLength));	}
};


//-----------------------------------------------------------------------------
// 던전에 Join을 시도 한다. 
//-----------------------------------------------------------------------------
struct CTWS_TRY_TO_JOIN_DUNGEON : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;	
	
	CTWS_TRY_TO_JOIN_DUNGEON()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_TRY_TO_JOIN_DUNGEON;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_TRY_TO_JOIN_DUNGEON);	}
};


//-----------------------------------------------------------------------------
// 월드서버가 클라이언트에게 해당 던전에 Join 할 수 없다는 메세지를 보냄 
//-----------------------------------------------------------------------------
struct WSTC_DUNGEON_JOIN_FAILED : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;		// 해당 던전 ID
	BYTE	bFailCode;		// 1이면 던전 서버연결 안됨, 2이면 Dungeon에 Server정보 포인터 연결안됨 ,3이면 DungeonServer AcceptTable에 추가 실패 
							// 100 : 이벤트 입구 시간 폐쇄., 101 : 이벤트 인원 초과.

	WSTC_DUNGEON_JOIN_FAILED()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_DUNGEON_JOIN_FAILED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DUNGEON_JOIN_FAILED);	} 
};


//-----------------------------------------------------------------------------
// 월드서버가 클라이언트에게 접속이 허용되었으니 이제 해당 던전서버로 접속하라는 메세지를 보낸다. 
//-----------------------------------------------------------------------------
struct WSTC_CONNECT_DUNGEON_SERVER : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwDungeonIP;
	
	DWORD	dwPartyID;
	WORD	wPort;
	
	WSTC_CONNECT_DUNGEON_SERVER()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CONNECT_DUNGEON_SERVER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CONNECT_DUNGEON_SERVER);	}
};


// 공성전 시작
struct WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwSiegeEndTick;
	
	WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_STARTSIEGE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE);	}
};


// 레벨업
struct WSTC_REFRESHEVENT_DUNGEON_LEVELUP : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwLevelUpDestTick;
	WSTC_REFRESHEVENT_DUNGEON_LEVELUP()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_LEVELUP;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_LEVELUP);	}
};


// 생산품 생산
struct WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION : public WORLD_PACKET_HEADER
{
	BYTE	byProductionCount;
	WORD	wDungeonID;
	WORD	wProduction;
	WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_CREATEPRODUCTION;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION);	}
};


// 공성전 끝
struct WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE : public WORLD_PACKET_HEADER
{
	
	BYTE	bDefenseSuccess;
	WORD	wDungeonID;
	DWORD	dwSiegeStartTick;
	WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_ENDSIEGE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE);	}
};


// 주인이 바꼇다.
struct WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwUserIndex;
	DWORD	dwGuildId;
	char	szOwner[ MAX_CHARACTER_NAME_LENGTH ];
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_CHANGE_OWNER;
		memset(szOwner, 0, sizeof(szOwner));
		memset(szGuildName, 0, sizeof(szGuildName));
	}

	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER);	}
};


//-----------------------------------------------------------------------------
// GM용 공지사항 패킷 
//-----------------------------------------------------------------------------
struct CTWS_GM_NOTICE : public WORLD_PACKET_HEADER
{
	WORD	wMsgLen;
	char	szMsg[1024];

	CTWS_GM_NOTICE()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_NOTICE;	}
	DWORD	GetPacketSize()	{	return 4 + wMsgLen;	}
};


//-----------------------------------------------------------------------------
// 포탈 등으로 월드서버의 최근 던전정보가 갱신되어야 할때 
//-----------------------------------------------------------------------------
struct CTWS_REFRESH_RECENT_DUNGEON : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;

	CTWS_REFRESH_RECENT_DUNGEON()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESH_RECENT_DUNGEON;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_REFRESH_RECENT_DUNGEON);	}
};


// 주인이 바꼇다.
struct WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwGuildId;
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD()
	{
		bGameStatus = UPDATE_GAME_WORLD;
		bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD;
		memset(szGuildName, 0, sizeof(szGuildName));
	}

	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD);	}
};


//-----------------------------------------------------------------------------
// 이벤트 던젼 정보.
//-----------------------------------------------------------------------------
struct WSTC_EVENT_DUNGEON_INFO : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;								// 던젼 ID
	BYTE	byEventState;							// 현재 상태.
	char	szDungeonName[MAX_DUNGEON_NAME_LENGTH]; // 던젼 이름.
	DWORD	dwReadyOpenTime;						// 입구 열릴 때	까지의 시간.	
	BYTE	byWorldMapID;							// World Map Number.
	BYTE	byEventType;							// 이벤트 던젼의 타입.
	BYTE	byEnterMinLevel;						// 입장 가능 최소 레벨.
	BYTE	byEnterMaxLevel;						// 입장 가능 최대 레벨.
	DWORD	dwEventEndTick;							// 이벤트 던젼 종료 시간
	WSTC_EVENT_DUNGEON_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_EVENT_DUNGEON_INFO; }
	DWORD	GetPacketSize()		{ return sizeof(WSTC_EVENT_DUNGEON_INFO);	}
};


struct CTWS_REQUEST_OWN_DUNGEON_INFO : public WORLD_PACKET_HEADER
{	
	CTWS_REQUEST_OWN_DUNGEON_INFO()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REQUEST_OWN_DUNGEON_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_REQUEST_OWN_DUNGEON_INFO);	}
};


//-----------------------------------------------------------------------------
// 이벤트 던젼의 상태 변화.
//-----------------------------------------------------------------------------
struct WSTC_EVENT_DUNGEON_STATE_INFO : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;			// 던젼 ID
	BYTE	byEventState;		// 이벤트 던젼 상태.		
	DWORD	dwReadyOpenTime;	// 입구 열릴 때 까지의 시간.
	DWORD	dwEventEndTick;		// 이벤트 던젼 종료 시간
	
	WSTC_EVENT_DUNGEON_STATE_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_EVENT_DUNGEON_STATE_INFO; }
	DWORD	GetPacketSize()			{ return sizeof(WSTC_EVENT_DUNGEON_STATE_INFO); }
};


//-----------------------------------------------------------------------------
// Event Dungeon 정보를 보내달라고 요청.
//-----------------------------------------------------------------------------
struct CTWS_QUERY_EVENT_DUNGEON_INFO : public WORLD_PACKET_HEADER
{
	BYTE	bRequestType;		// 요청 타입. 1 : 이벤트 던젼의 정보, 2 : 이벤트 던젼의 상태.

	CTWS_QUERY_EVENT_DUNGEON_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_QUERY_EVENT_DUNGEON_INFO; }
	DWORD	GetPacketSize()	{ return sizeof(CTWS_QUERY_EVENT_DUNGEON_INFO); }
};


//-----------------------------------------------------------------------------
// 던전서버가 월드서버에게 해당 유저가 던전으로 나간다는 신호를 보내주면(DSTC_GO_TO_WORLD)
// 월드서버는 이 유저가 월드상의 위치를 판별하여 패킷을 주고, 클라이언트는 이패킷을 받고 
// 월드맵 로딩을 시작한다. 
//-----------------------------------------------------------------------------
struct WSTC_MOVE_TO_WORLD : public WORLD_PACKET_HEADER
{
	BYTE	bWorldMapID;		// 1번 부터 현
	WORD	wToDungeonID;
	WORD	wNowDungeonID;		// 월드맵으로 나가기 전에 현재 있는 던젼 아이디
	float	fStartPos_X;
	float	fStartPos_Z;

	WSTC_MOVE_TO_WORLD()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GO_TO_WORLD;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MOVE_TO_WORLD);	}
};

const BYTE GO_TO_WORLD_ERROR_DUNGEON_NO = 1;
const BYTE GO_TO_WORLD_ERROR_DUNGEON_POS = 2;
const BYTE GO_TO_WORLD_ERROR_STADIUM = 3;
//-----------------------------------------------------------------------------
// World로 이동 실패!
//-----------------------------------------------------------------------------
struct WSTC_GO_TO_WORLD_FAIL : public WORLD_PACKET_HEADER
{
	BYTE	bErrorCode;

	WSTC_GO_TO_WORLD_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GO_TO_WORLD_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GO_TO_WORLD_FAIL);	}
};


// 입장료 획득
struct WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwMoney;
	WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION);	}
};


// 던전 경험치 획득
struct WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwAccExp;
	WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION);	}
};


// 입장료 수정
struct WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	DWORD	dwMoney;
	WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT);	}
};


//-----------------------------------------------------------------------------
// 월드맵 이동에 성공함 
//-----------------------------------------------------------------------------
struct CTWS_GO_TO_WORLD_SUCCESS : public WORLD_PACKET_HEADER
{
	BYTE	bWorldMapId;		//월드맵 번호 
	
	CTWS_GO_TO_WORLD_SUCCESS()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GO_TO_WORLD_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GO_TO_WORLD_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// 월드맵 이동 패킷 
//-----------------------------------------------------------------------------
struct CTC_WORLD_MOVE : public WORLD_PACKET_HEADER
{	
	DWORD	dwCharIndex;
	BYTE	bMoveType;			// Move types
	VECTOR3	v3MoveStartPos;		// Start position
	VECTOR3	v3MoveDirection;	// Direction
	
	WORD	wDestX;
	WORD	wDestZ;

	CTC_WORLD_MOVE()		{	bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_WORLD_MOVE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_WORLD_MOVE);	}
};


//-----------------------------------------------------------------------------
// 월드맵 이동 패킷 
//-----------------------------------------------------------------------------
struct CTC_WORLD_STOP : public WORLD_PACKET_HEADER
{	
	DWORD	dwCharIndex;
	VECTOR3	v3StopPos;			// Stop position
	CTC_WORLD_STOP()		{	bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_WORLD_STOP;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_WORLD_STOP);	}
};


struct WORLD_PARTY_APPEAR_INFO
{
	DWORD	dwCharIndex;
	WORD	wHead;
	WORD	wClass;
	WORD	wArmor;
	WORD	wHandR;
	WORD	wHandL;
	WORD	wHelmet;	
	
	float	fAppear_X;	//나타날 위치 
	float	fAppear_Z;	//나타날 위치 
};


//-----------------------------------------------------------------------------
// 월드상에 유저를 Appear 되게 하라 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_APPEAR : public WORLD_PACKET_HEADER
{
	WORLD_PARTY_APPEAR_INFO	AppearInfo;

	WSTC_WORLD_APPEAR()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_WORLD_APPEAR; }
	DWORD	GetPacketSize()	{ return sizeof(WSTC_WORLD_APPEAR); }
};


//-----------------------------------------------------------------------------
// 월드상에 유저를 Disappear 되게 하라 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_DISAPPEAR : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;
	
	WSTC_WORLD_DISAPPEAR()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_WORLD_DISAPPEAR; }
	DWORD	GetPacketSize()	{ return sizeof(WSTC_WORLD_DISAPPEAR); }
};


//-----------------------------------------------------------------------------
// 유저가 월드멥에 들어왔을때 들어온 본인에게 해당 월드맵에 있었던 모든 파티유저의 Appear 정보를 보내줌 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_PARTY_USER_INFO : public WORLD_PACKET_HEADER
{
	BYTE	bUserCount;		//Appear 될 유저 명수 
	WORLD_PARTY_APPEAR_INFO	AppearInfo[ MAX_PARTY_USER ];

	WSTC_WORLD_PARTY_USER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_WORLD_PARTY_USER_INFO; }
	DWORD	GetPacketSize()	{ return ( 3 + (sizeof(WORLD_PARTY_APPEAR_INFO) * bUserCount) ); }
};


//-----------------------------------------------------------------------------
// 유저를 특정 지정한 위치에 옮긴다. 
//-----------------------------------------------------------------------------
struct CTC_MOVE_POSITION : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;
	VECTOR3 v3Pos;
	float	fDirection;

	CTC_MOVE_POSITION()		{	bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_WORLD_SET_POSITION;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_MOVE_POSITION);	}
};


//-----------------------------------------------------------------------------
// 파티를 맺은 유저들에게 본인이 던전서버에 Connect를 시도한다는 사실을 
// 알려주어 클라이언트 화면에서는 잠시 사라지게 한다. 그후 Dungeon에 조인을
// 성공하면 Player를 Remove 한다. 
//-----------------------------------------------------------------------------
struct WSTC_PARTY_USER_TRY_TO_CONNECT : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;

	WSTC_PARTY_USER_TRY_TO_CONNECT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USER_TRY_TO_CONNECT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_USER_TRY_TO_CONNECT);	}
};


//-----------------------------------------------------------------------------
// Party 유저가 비공정에 탑승함 
//-----------------------------------------------------------------------------
struct CTC_AIRSHIP_RIDING : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;

	CTC_AIRSHIP_RIDING()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_AIRSHIP_RIDING;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_AIRSHIP_RIDING);	}
};


//-----------------------------------------------------------------------------
// Party 유저가 비공정에 하차함 
//-----------------------------------------------------------------------------
struct CTC_AIRSHIP_GETOFF : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;
	VECTOR3	v3Pos;			//하차한 위치 

	CTC_AIRSHIP_GETOFF()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_AIRSHIP_GETOFF;	}
	DWORD	GetPacketSize()	{	return sizeof(CTC_AIRSHIP_GETOFF);	}
};


// 던전 가디언이 바뀌었군.
struct WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM : public WORLD_PACKET_HEADER
{
	WORD	wDungeonID;
	CItem	cGuardianItem;
	WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_GUARDIANITEM;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM);	}
};


// 던전 마법진이 바뀌었군.
struct WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	CItem	cMagicFieldArrayItem;
	WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_MAGICFIELDARRAYITEM;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY);	}
};


//#ifndef NEW_COMMAND

//-----------------------------------------------------------------------------
// GM 커맨드.
//-----------------------------------------------------------------------------
struct CTWS_GM_COMMAND : public WORLD_PACKET_HEADER
{		
	WORD	wCommand;
	char	szCharName[ MAX_CHARACTER_NAME_LENGTH ];
	DWORD	dwTemp[5];
	
	CTWS_GM_COMMAND()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GM_COMMAND;	}
	DWORD	GetPacketSize()	{	return sizeof( CTWS_GM_COMMAND );	}
};


struct CTWS_NEW_GM_COMMAND  : public WORLD_PACKET_HEADER
{
	union
	{
		GM_CMD_PARAM					sBaseGMCmdParam;
		GM_CMD_PARAM_Recall_Someone		sGMCmdParamRecallSomeone;
		GM_CMD_PARAM_Get_Move_Someone	sGMCmdParamGetMoveSomeone;
		GM_CMD_PARAM_Dungeon_Portal		sGMCmdParamDungeonPortal;
		GM_CMD_PARAM_Join_Guild			sGMCmdParamJoinGuild;
		GM_CMD_PARAM_Kick_User			sGMCmdParamKickUser;
	};
		
	CTWS_NEW_GM_COMMAND()		
	{ 
		bGameStatus = UPDATE_GAME_WORLD;	
		bHeader = Protocol_World::CMD_GM_COMMAND;	
	}

	DWORD GetPacketSize()	{ return sizeof( CTWS_NEW_GM_COMMAND );	}
};



//-----------------------------------------------------------------------------
// GM 커맨드 결과값.
//-----------------------------------------------------------------------------
struct WSTC_GM_COMMAND_RESULT : public WORLD_PACKET_HEADER
{
	WSTC_GM_COMMAND_RESULT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GM_COMMAND_RESULT;	}
	DWORD	GetPaketSize()		{	return sizeof( WSTC_GM_COMMAND_RESULT );	}
};


//-----------------------------------------------------------------------------
// 내가 모니터링 툴인지 체크해 주시오.
//-----------------------------------------------------------------------------
struct CTWS_CHECK_MONITORING : public WORLD_PACKET_HEADER
{		
	CTWS_CHECK_MONITORING()		{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHECK_MONITORING;	}
	DWORD	GetPacketSize()		{ return sizeof(CTWS_CHECK_MONITORING); }	
};


// 디펜스 횟수
struct WSTC_REFRESHEVENT_DEFENSECOUNT : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	BYTE	byDefenseCount;
	WSTC_REFRESHEVENT_DEFENSECOUNT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DEFENSECOUNT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DEFENSECOUNT);	}
};


// 운영모드 갱신
struct WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE : public WORLD_PACKET_HEADER
{	
	WORD	wDungeonID;
	BYTE	byOperationMode;
	WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_REFRESHEVENT_DUNGEON_OPERATIONMODE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE);	}
};


//----------------------------------------------------------------------------------
// 과금결제 관련 정보 패킷 
//----------------------------------------------------------------------------------
struct WSTC_BILLING_INFO : public WORLD_PACKET_HEADER
{
	BYTE		bCurBillingType;
	BYTE		bInfoType;
	DWORD		dwRemainTime;
	DBTIMESTAMP	BillingEndDate;

	WSTC_BILLING_INFO()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_BILLING_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_BILLING_INFO);	}
};


// 중국 빌링용. 빌링서버가 유저를 kick할 때 사유를 알려준다.
struct WSTC_CHINA_BILL_KICK_CODE : public WORLD_PACKET_HEADER
{
	BYTE		btKickCode;

	WSTC_CHINA_BILL_KICK_CODE() { bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHINA_BILL_KICK_CODE; btKickCode = 0;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CHINA_BILL_KICK_CODE);	}
};


//----------------------------------------------------------------------------------
// 클라이언트는 타이머 체크할 준비가 되엇다.
//----------------------------------------------------------------------------------
struct CTWS_TIMER_READY
{
	BYTE	bGameStatus;
	BYTE	bHeader;

	CTWS_TIMER_READY()			{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_TIMER_READY;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_TIMER_READY);	}
};


struct WSTC_TIMER_SYNC
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	WORD	wIntervalSec;

	WSTC_TIMER_SYNC()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_TIMER_SYNC;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_TIMER_SYNC);	}
};


struct CTWS_TIMER_ACK
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	DWORD	dwTime;

	CTWS_TIMER_ACK()			{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_TIMER_ACK;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_TIMER_ACK);	}
};


struct WSTC_CB_MESSAGE
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	BYTE	byMessageType;

	WSTC_CB_MESSAGE()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CB_MESSAGE;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_CB_MESSAGE);	}	
};


//-----------------------------------------------------------------------------
// 파티 유저 정보 갱신. 
//-----------------------------------------------------------------------------
struct CTWS_PARTYUSER_WORLDINFO : public WORLD_PACKET_HEADER
{
	WORD	wHelmet;
	WORD	wHandR;
	WORD	wHandL;
	WORD	wArmor;
	BYTE	byCurrentHand;

	CTWS_PARTYUSER_WORLDINFO()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTYUSER_WORLDINFO;	}
	DWORD	GetPacketSize()			{	return sizeof(CTWS_PARTYUSER_WORLDINFO);	}	
};


struct WSTC_GUILDWAR_MESSAGE : public WORLD_PACKET_HEADER
{
	char	szGuildName[MAX_GUILD_NAME_LENGTH];	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	char	szNameDst[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byType;
	BYTE	byLayerIndex;
	WORD	wDungeonID;

	WSTC_GUILDWAR_MESSAGE()			{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_MESSAGE; }
	DWORD		GetPacketSize()		{ return sizeof(WSTC_GUILDWAR_MESSAGE); }
};


//-----------------------------------------------------------------------------
// 파티 게시판 정보 삭제.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_BOARDDELETE : public WORLD_PACKET_HEADER
{
	DWORD	dwPartyId;

	CTWS_PARTY_BOARDDELETE()		{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_BOARDDELETE; }
	DWORD		GetPacketSize()		{ return sizeof(CTWS_PARTY_BOARDDELETE); }
};


struct CTWS_PARTY_INFOREQUEST : public WORLD_PACKET_HEADER
{
	DWORD	dwUserIndex;

	CTWS_PARTY_INFOREQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_INFOREQUEST;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_PARTY_INFOREQUEST);	}
};


struct WSTC_PARTY_INFOREQUEST : public WORLD_PACKET_HEADER
{
	DWORD	dwPartyId;
	DWORD	dwLeaderUserIndex;	
	char	szCharName[MAX_CHARACTER_NAME_LENGTH];
	char	szMemo[32];
	WORD	wDungeonID;
	BYTE	byType;

	WSTC_PARTY_INFOREQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_INFOREQUESTRT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_PARTY_INFOREQUEST);	}
};


struct WSTC_PARTY_ENTRY : public WORLD_PACKET_HEADER
{				
	BYTE	byType;	

	WSTC_PARTY_ENTRY()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_ENTRYRT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_PARTY_ENTRY);	}
};


struct CTWS_PARTY_ENTRY : public WORLD_PACKET_HEADER
{				
	DWORD	dwUserIndex;
	DWORD	dwPartyId;	
	char	szPartyEntryMemo[MAX_PARTYENTRY_TEXT];	

	CTWS_PARTY_ENTRY()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_ENTRY;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_PARTY_ENTRY);	}
};

// 파티 설정, 아이템/경험치 분배 : 최덕석 2005.1.25
struct CTWS_PARTY_CONFIG : public WORLD_PACKET_HEADER
{
	PARTY_CONFIG pConfig;

	CTWS_PARTY_CONFIG()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_CONFIG;	}
	DWORD	GetPacketSize()		{	return (sizeof(CTWS_PARTY_CONFIG)); }
};

// 최근 파티원 목록 전송
struct WSTC_PARTY_FRIEND : public WORLD_PACKET_HEADER
{
	DWORD dwCount;
	PARTY_FRIEND pFriend[20];

	WSTC_PARTY_FRIEND()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_FRIEND;	}
	DWORD	GetPacketSize()		{	return (sizeof(WSTC_PARTY_FRIEND)); }
};

struct CTWS_PARTY_FRIEND : public WORLD_PACKET_HEADER
{
	CTWS_PARTY_FRIEND()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_FRIEND;	}
	DWORD	GetPacketSize()		{	return (sizeof(CTWS_PARTY_FRIEND)); }
};

struct WSTC_GUILDWAR_RANK : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId[4];
	DWORD		dwPoint[4];
	char		szName[4][MAX_GUILD_NAME_LENGTH];	
	DWORD		dwRank;
	
	WSTC_GUILDWAR_RANK()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDWAR_RANKRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_RANK); }
};


struct CTWS_GUILDWAR_RANK : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	
	CTWS_GUILDWAR_RANK()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDWAR_RANK; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDWAR_RANK); }
};


struct WSTC_GUILDWAR_EXIT : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;	
	DWORD		dwPoint;	
	BYTE		byType;
	
	WSTC_GUILDWAR_EXIT()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDWAR_EXITRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_EXIT); }
};


struct CTWS_GUILDWAR_EXIT : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	DWORD		dwGuildIdDst;		
	
	CTWS_GUILDWAR_EXIT()	{ bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDWAR_EXIT; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDWAR_EXIT); }
};


struct WSTC_GUILDWAR_POINT : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	WORD	wPoint1;
	WORD	wPoint2;	
	BYTE	byType;

	WSTC_GUILDWAR_POINT()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_POINT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_GUILDWAR_POINT);	}
};


struct CTWS_GUILDWAR_CREATE : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;

	CTWS_GUILDWAR_CREATE()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_CREATE;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_GUILDWAR_CREATE);	}
};


struct WSTC_GUILDWAR_JOIN : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;
	char	szGuildName[MAX_GUILD_NAME_LENGTH];	

	WSTC_GUILDWAR_JOIN()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_JOIN;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_JOIN);	}
};


struct CTWS_GUILDWAR_REQUESTRT : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;

	CTWS_GUILDWAR_REQUESTRT()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_MASTERRT;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_GUILDWAR_REQUESTRT);	}
};


struct CTWS_GUILDWAR_ERRROR : public WORLD_PACKET_HEADER
{
	BYTE	byError;	
	DWORD	dwUserIndex;
	char	szGuildName[MAX_GUILD_NAME_LENGTH];

	CTWS_GUILDWAR_ERRROR()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_ERRORRT;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDWAR_ERRROR);	}
};


struct WSTC_GUILDWAR_ERRROR : public WORLD_PACKET_HEADER
{
	BYTE	byError;
	char	szNameTemp[MAX_CHARACTER_NAME_LENGTH];
		
	WSTC_GUILDWAR_ERRROR()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_ERROR;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_ERRROR);	}
};


struct WSTC_GUILDWAR_REQUEST : public WORLD_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;
	char	szGuildName[MAX_GUILD_NAME_LENGTH];
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_GUILDWAR_REQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_MASTER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDWAR_REQUEST);	}
};


struct CTWS_GUILDWAR_REQUEST : public WORLD_PACKET_HEADER
{			
	DWORD	dwGuildId;
	DWORD	dwDstGuildId;
	DWORD	dwUserIndex;

	CTWS_GUILDWAR_REQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDWAR_REQUEST;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDWAR_REQUEST);	}
};


struct WSTC_GUILD_PORTALFAIL : public WORLD_PACKET_HEADER
{	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	
	WSTC_GUILD_PORTALFAIL()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_PORTALFAIL; }
	DWORD	GetPacketSize()	{ return sizeof(WSTC_GUILD_PORTALFAIL); }
};


struct WSTC_GUILD_PORTALUSED : public WORLD_PACKET_HEADER
{	
	DWORD	dwGuildId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	dwResult;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_GUILD_PORTALUSED()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_PORTALUSED;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_GUILD_PORTALUSED)); }
};


struct WSTC_GUILD_PORTAL : public WORLD_PACKET_HEADER
{	
	DWORD	dwGuildId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_GUILD_PORTAL()	{ bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_GUILD_PORTAL;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_GUILD_PORTAL)); }
};


//-----------------------------------------------------------------------------
// 길드 관련 에러 메세지.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_ERROR : public WORLD_PACKET_HEADER
{	
	BYTE		byError;
	
	WSTC_GUILD_ERROR()		{ bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_FAIL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_ERROR); }
};


//-----------------------------------------------------------------------------
// 길드전 정보.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_LIST : public WORLD_PACKET_HEADER
{
	char		szName[MAX_GUILD_NAME_LENGTH];
	DWORD		dwGuildId;
	WORD		wPoint1;
	WORD		wPoint2;
	time_t		sTime;	
	time_t		sCurTime;
	
	WSTC_GUILD_LIST()		{ bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_LIST; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_LIST); }
};


typedef struct __tag_sScriptex__
{
	ENCOMMAND	enCommand;
	char		szId[MAX_ID_LENGTH];
	char		szMessage[MAX_INPUT_BUFFER_SIZE];	
	char		szText[MAX_INPUT_BUFFER_SIZE];

}SSCRIPTEX, *LPSSCRIPTEX;


//-----------------------------------------------------------------------------
// 명령어를 클라이언트에서 받음(월드서버).
//-----------------------------------------------------------------------------
struct WSTC_WS_CHAT : public WORLD_PACKET_HEADER
{	
	SSCRIPTEX	cScript;	

	WSTC_WS_CHAT()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_COMMAND_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_WS_CHAT);	}
};


//-----------------------------------------------------------------------------
// 받은 명령어가 실패함(월드서버).
//-----------------------------------------------------------------------------
struct WSTC_WSF_CHAT : public WORLD_PACKET_HEADER
{		
	int			nFail;
	ENCOMMAND	enCommand;
	char		szId[MAX_ID_LENGTH];

	WSTC_WSF_CHAT()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_COMMAND_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_WSF_CHAT); }
};


//-----------------------------------------------------------------------------
// 클라이언트에게 귓속말을 보냄(월드서버).
//-----------------------------------------------------------------------------
struct WSTC_CHAT_WHISPER : public WORLD_PACKET_HEADER
{	
	int		nSuccess;
	char	szId[MAX_ID_LENGTH];
	char	szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_CHAT_WHISPER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_WHISPER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CHAT_WHISPER);	}
};


//-----------------------------------------------------------------------------
// Desc : 명령어를 클라이언트에서 받음(던젼 서버).
//-----------------------------------------------------------------------------
struct WSTC_DS_CHAT : public WORLD_PACKET_HEADER
{	
	SSCRIPTEX	cScript;

	WSTC_DS_CHAT()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_COMMAND_DSUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DS_CHAT);	}
};


//-----------------------------------------------------------------------------
// 쪽지를 보냄(월드 서버).
//-----------------------------------------------------------------------------
struct WSTC_CHAT_COMMAND : public WORLD_PACKET_HEADER
{	
	int		nSuccess;
	BYTE	byMessageLen;
	char	szId[MAX_ID_LENGTH];
	char	szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_CHAT_COMMAND() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MESSAGE;	
	}

	DWORD GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_CHAT_COMMAND)-sizeof(szMessage)+byMessageLen;	
	}
};


//-----------------------------------------------------------------------------
// 쪽지를 보냄(던젼 서버).
//-----------------------------------------------------------------------------
struct CTWS_DSCHAT_COMMAND : public WORLD_PACKET_HEADER
{	
	int		nSuccess;
	BYTE	byMessageLen;
	char	szId[MAX_ID_LENGTH];
	char	szMessage[MAX_INPUT_BUFFER_SIZE];

	CTWS_DSCHAT_COMMAND() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MESSAGE;	
	}

	DWORD GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(CTWS_DSCHAT_COMMAND)-sizeof(szMessage)+byMessageLen;	
	}
};


//-----------------------------------------------------------------------------
// 클라이언트에서 쪽지 요청을 받음.
//-----------------------------------------------------------------------------
struct CTWS_CHAT_MESSAGERQ : public WORLD_PACKET_HEADER
{		
	char	szId[MAX_ID_LENGTH];	

	CTWS_CHAT_MESSAGERQ() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MESSAGERQ;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_CHAT_MESSAGERQ);	}
};


//-----------------------------------------------------------------------------
// 클라이언트에서 쪽지 요청을 받음.
//-----------------------------------------------------------------------------
struct WSTC_CHAT_RQ  : public WORLD_PACKET_HEADER
{
	int			nIndex;
	int			nCount;
	BYTE		byMessageLen;
	WORD		wdTime[6];
	char		szId[MAX_ID_LENGTH];	
	char		szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_CHAT_RQ() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_RQ;	
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_CHAT_RQ)-sizeof(szMessage)+byMessageLen;	
	}
};


//-----------------------------------------------------------------------------
// 채팅 메세지를 클라이언트로 받는다.
//-----------------------------------------------------------------------------
struct WSTC_CHAT_USER : public WORLD_PACKET_HEADER
{
	int			nIndex;
	BYTE		byMessageLen;
	char		szDstId[MAX_ID_LENGTH];
	char		szId[MAX_ID_LENGTH];
	char		szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_CHAT_USER() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_CHAT_USER; 
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_CHAT_USER)-sizeof(szMessage)+byMessageLen; 
	}
};


//-----------------------------------------------------------------------------
// 채팅 메세지를 클라이언트로 받는다.
//-----------------------------------------------------------------------------
struct CTWS_DGCHAT_USER : public WORLD_PACKET_HEADER
{
	int			nIndex;
	BYTE		byMessageLen;
	char		szDstId[MAX_ID_LENGTH];
	char		szId[MAX_ID_LENGTH];
	char		szMessage[MAX_INPUT_BUFFER_SIZE];

	CTWS_DGCHAT_USER() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_DGCHAT_USER; 
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(CTWS_DGCHAT_USER)-sizeof(szMessage)+byMessageLen; 
	}
};


//-----------------------------------------------------------------------------
// 받은 명령어가 실패함(던젼 서버).
//-----------------------------------------------------------------------------
struct WSTC_DSF_CHAT : public WORLD_PACKET_HEADER
{	
	int			nFail;
	ENCOMMAND	enCommand;
	char		szId[MAX_ID_LENGTH];

	WSTC_DSF_CHAT()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_COMMAND_DGFAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DSF_CHAT);	}
};


//-----------------------------------------------------------------------------
// 클라이언트 귓속말을 보냄(던젼 서버).
//-----------------------------------------------------------------------------
struct WSTC_DSCHAT_WHISPER : public WORLD_PACKET_HEADER
{
	int		nSuccess;
	BYTE	byMessageLen;
	char	szId[MAX_ID_LENGTH];
	char	szMessage[MAX_INPUT_BUFFER_SIZE];

	WSTC_DSCHAT_WHISPER() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_DGWHISPER;	
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_DSCHAT_WHISPER)-sizeof(szMessage)+byMessageLen;	
	}
};


//-----------------------------------------------------------------------------
// 채팅 메세지를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_DGCHAT_USER : public WORLD_PACKET_HEADER
{
	int			nIndex;
	BYTE		byMessageLen;
	char		szDstId[MAX_ID_LENGTH];
	char		szId[MAX_ID_LENGTH];
	char		szMessage[MAX_INPUT_BUFFER_SIZE];
	
	BYTE		bSerType;

	WSTC_DGCHAT_USER() 
	{ 
		memset(this, 0, sizeof(*this));
		bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_WSDGCHAT_USER; 
	}
	DWORD	GetPacketSize()	
	{	
		byMessageLen = BYTE(lstrlen(szMessage)+1);
		return sizeof(WSTC_DGCHAT_USER)-sizeof(szMessage)+byMessageLen; 
	}
};


//-----------------------------------------------------------------------------
// 길드 정보를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_USER : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];	
	char		szRankName[4][MAX_GUILD_NAME_LENGTH];
	BYTE		byType;
	BYTE		byCount;
	DWORD		dwGuildId;
	WORD		wLevel;
	DWORD		dwExp;
	DWORD		dwPoint;
	BYTE		byMember[4];
	BYTE		byLevelIndex;

	WSTC_GUILD_USER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_USER; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_USER); }
};


//-----------------------------------------------------------------------------
// 길드원 정보를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_USERINFO : public WORLD_PACKET_HEADER
{
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];		
	DWORD		dwUserIndex;
	BYTE		byRank;	
	DWORD		dwLev;
	WORD		wClass;
	DWORD		dwGuildId;	
	BYTE		bGuildCount;

	WSTC_GUILD_USERINFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_USERINFO; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_USERINFO); }
};


//-----------------------------------------------------------------------------
// 지울 길드원 정보를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_USERINFODEL : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];	

	WSTC_GUILD_USERINFODEL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_USERINFODEL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_USERINFODEL); }
};


//-----------------------------------------------------------------------------
// 길드 생성 메세지를 보낸다(월드 서버).
//-----------------------------------------------------------------------------
struct CTWS_GUILD_CREATE : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE		bType;
	DWORD		dwLev;
	WORD		wClass;

	CTWS_GUILD_CREATE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_CREATE; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_CREATE); }	
};


//-----------------------------------------------------------------------------
// 길드 생성 결과를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_CREATE : public WORLD_PACKET_HEADER
{
	BOOL		bCreate;		
	BYTE		bType;	

	WSTC_GUILD_CREATE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_WSGUILD_CREATE; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_CREATE); }
};


//-----------------------------------------------------------------------------
// 길드원 정보를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_DGGUILD_USERINFO : public WORLD_PACKET_HEADER
{
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];		
	BYTE		byRank;
	DWORD		dwLev;
	WORD		wClass;
	DWORD		dwGuildId;	
	DWORD		dwUserIndex;

	WSTC_DGGUILD_USERINFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_DGGUILD_USERINFO; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DGGUILD_USERINFO); }
};


//-----------------------------------------------------------------------------
// 길드 정보를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_DGGUILD_USER : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];	
	char		szRankName[4][MAX_GUILD_NAME_LENGTH];
	BYTE		byType;
	BYTE		byCount;
	DWORD		dwGuildId;	
	WORD		wLevel;

	WSTC_DGGUILD_USER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_DGGUILD_USER; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DGGUILD_USER); }
};


//-----------------------------------------------------------------------------
// 길드/클랜 가입 메세지를 보낸다.(월드 서버).
//-----------------------------------------------------------------------------
struct CTWS_GUILDJOIN_MESSAGE : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];	
	char		szCharacterName[MAX_GUILD_NAME_LENGTH];
	BYTE		bType;	
	DWORD		dwUserIndex;

	CTWS_GUILDJOIN_MESSAGE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDJOIN_MESSAGE; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDJOIN_MESSAGE); }
};


//-----------------------------------------------------------------------------
// 길드 가입 메세지를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_GUILDJOIN_MESSAGE : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_CHARACTER_NAME_LENGTH];	
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		bType;
	DWORD		dwUserIndex;

	WSTC_GUILDJOIN_MESSAGE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDJOINRECV_MESSAGE; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDJOIN_MESSAGE); }
};


//------------------------------------------------------------------------------//
// 길드 조인 메세지.														//
//------------------------------------------------------------------------------//
struct CTWS_GUILD_JOIN : public WORLD_PACKET_HEADER
{
	BOOL	bResult;
	BYTE	bGuildType;
	DWORD	dwLev;
	WORD	wClass;
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	DWORD	dwUserIndex;

	CTWS_GUILD_JOIN()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_JOIN;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_JOIN);	}
};


//-----------------------------------------------------------------------------
// 길드 가입 대답.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_JOINRESULT : public WORLD_PACKET_HEADER
{
	BOOL		bResult;		
	BYTE		bErrorCode;
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	DWORD		dwUserIndex;

	WSTC_GUILD_JOINRESULT() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_JOINRESULT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_JOINRESULT); }
};


//-----------------------------------------------------------------------------
// 길드원 정보를 클라이언트로 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_JOININFO : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];		
	BYTE		byType;
	BYTE		byRank;
	DWORD		dwGuildId;

	WSTC_GUILD_JOININFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_JOININFO; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_JOININFO); }
};


//-----------------------------------------------------------------------------
// 길드원 등급 변경.
//-----------------------------------------------------------------------------
struct CTWS_GUILD_RANK : public WORLD_PACKET_HEADER
{
	char	szRankName[4][MAX_CHARACTER_NAME_LENGTH];
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];
			
	CTWS_GUILD_RANK() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_RANK; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_RANK); }
};


//-----------------------------------------------------------------------------
// 길드 등급 설정
//-----------------------------------------------------------------------------
struct WSTC_GUILD_RANK : public WORLD_PACKET_HEADER
{
	BOOL		bResult;	
	BYTE		bSerType;
	
	WSTC_GUILD_RANK() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_RANKRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_RANK); }
};


//-----------------------------------------------------------------------------
// 길드원 정보 변경.
//-----------------------------------------------------------------------------
struct CTWS_GUILD_INFO : public WORLD_PACKET_HEADER
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	bRank;
	BYTE	bMsgType;
	DWORD	dwUserIndex;

	CTWS_GUILD_INFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_INFO; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_INFO); }
};		


//-----------------------------------------------------------------------------
// 변경된 정보를 길드원에게 보내준다.
//-----------------------------------------------------------------------------
struct WSTC_GUILD_INFO : public WORLD_PACKET_HEADER
{	
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		bRank;
	BYTE		bMsgType;
	BYTE		bSerType;
	DWORD		dwUserIndex;
	
	WSTC_GUILD_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_INFO); }
};


//-----------------------------------------------------------------------------
// 길드원 삭제.
//-----------------------------------------------------------------------------
struct CTWS_GUILD_USERDEL : public WORLD_PACKET_HEADER
{			
	BYTE	byUserType;
	DWORD	dwGuildId;	
	DWORD	dwUserIndex;
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];

	CTWS_GUILD_USERDEL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_USERDEL; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_USERDEL); }
};


//-----------------------------------------------------------------------------
// 길드원을 삭제 한다.
//-----------------------------------------------------------------------------
struct WSTC_USER_DELETE : public WORLD_PACKET_HEADER
{	
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD		dwGuildId;
	DWORD		dwUserIndex;	
	BYTE		byMember[4];
	BYTE		byChk;

	WSTC_USER_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_USER_DELETE;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_USER_DELETE);	}
};


//-----------------------------------------------------------------------------
// 길드 삭제 메세지.
//-----------------------------------------------------------------------------
struct CTWS_GUILD_DELETE : public WORLD_PACKET_HEADER
{
	char		szGuildName[MAX_GUILD_NAME_LENGTH];	
	DWORD		dwGuildId;
	
	CTWS_GUILD_DELETE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_DELETE; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_DELETE); }	
};


//-----------------------------------------------------------------------------
// 명령어를 클라이언트에서 받음(월드서버).
//-----------------------------------------------------------------------------
struct WSTC_GUILD_DELETE : public WORLD_PACKET_HEADER
{	
	DWORD	dwGuildId;
	BYTE	bServerType;

	WSTC_GUILD_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_DELETERT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_DELETE);	}
};


//-----------------------------------------------------------------------------
// 파티를 승인받는다.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_JOIN : public WORLD_PACKET_HEADER
{
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		bErrCode;	
	DWORD		dwUserIndex;

	CTWS_PARTY_JOIN() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_JOIN; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_JOIN); }
};


//-----------------------------------------------------------------------------
// 파티를 승인 결과를 보낸다.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_JOIN : public WORLD_PACKET_HEADER
{
	BYTE		bResult;	
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_JOIN() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_JOINRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_JOIN); }
};


//-----------------------------------------------------------------------------
// 파티를 승인했을 경우.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_APPROVE : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];	

	CTWS_PARTY_APPROVE() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_APPROVE; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_APPROVE); }
};


//-----------------------------------------------------------------------------
// 던젼에 들어갔을 경우.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_JOINID : public WORLD_PACKET_HEADER
{	
	DWORD		dwPartyId;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	WORD		wClass;
	BYTE		bResultCode;	
	DWORD		dwUserIndex;
	BYTE		byLeader;

	WSTC_PARTY_JOINID() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_JOINID; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_JOINID); }
};


//-----------------------------------------------------------------------------
// 파티에서 나갈때.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_LOGOUT : public WORLD_PACKET_HEADER
{
	char		szName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE		bResultCode;
	DWORD		dwUserIndex;

	WSTC_PARTY_LOGOUT() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_LOGOUT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_LOGOUT); }
};


//-----------------------------------------------------------------------------
// 파티에 나갈 경우.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_LOGOUT : public WORLD_PACKET_HEADER
{
	DWORD		dwPartyId;

	CTWS_PARTY_LOGOUT() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_LOGOUTCL; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_LOGOUT); }
};


//-----------------------------------------------------------------------------
// Skill Key를 보낸다. 
//-----------------------------------------------------------------------------
struct WSTC_SKILL_KEY : public WORLD_PACKET_HEADER
{
	BYTE	bSkillKey[16];
	BYTE	bPosKey[16];

	WSTC_SKILL_KEY()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_SKILL_KEY; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_SKILL_KEY);	}
};


//-----------------------------------------------------------------------------
// Skill Key를 보낸다. 
//-----------------------------------------------------------------------------
struct CTWS_SKILL_KEY : public WORLD_PACKET_HEADER
{
	BYTE	bSkillKey[16];
	BYTE	bPosKey[16];

	CTWS_SKILL_KEY()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_SKILL_KEYRT;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_SKILL_KEY);	}
};


//-----------------------------------------------------------------------------
// 귓말 허용, 거부 메세지.
//-----------------------------------------------------------------------------
struct CTWS_COMMAND_MSG : public WORLD_PACKET_HEADER
{	
	BOOL		bCommand;

	CTWS_COMMAND_MSG() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MSG; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_COMMAND_MSG); }
};


//-----------------------------------------------------------------------------
// 귓말 허용, 거부 메세지.
//-----------------------------------------------------------------------------
struct WSTC_COMMAND_MSG : public WORLD_PACKET_HEADER
{
	BOOL		bCommand;

	WSTC_COMMAND_MSG() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_COMMAND_MSGRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_COMMAND_MSG); }
};


//-----------------------------------------------------------------------------
// 파티 설정 메세지.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_SET : public WORLD_PACKET_HEADER
{	
	BOOL		bParty;
	
	CTWS_PARTY_SET()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_SET;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_SET); }
};


//-----------------------------------------------------------------------------
// 파티 설정 메세지.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_SET : public WORLD_PACKET_HEADER
{
	
	BOOL		bParty;
	
	WSTC_PARTY_SET()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_SETRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_SET); }
};


struct WSTC_PARTY_LEVEL : public WORLD_PACKET_HEADER
{
	DWORD		dwPartyId;
	DWORD		dwUserIndex;
	DWORD		dwLevel;

	WSTC_PARTY_LEVEL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_LEVEL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_LEVEL); }
};


//-----------------------------------------------------------------------------
// 파티 리더 변경.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_LEADER : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_LEADER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_LEADER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_LEADER);	}
};


//-----------------------------------------------------------------------------
// 파티원 정보.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_INFO : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	CTWS_PARTY_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_INFO);	}
};


//-----------------------------------------------------------------------------
// 파티원 정보.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_INFO : public WORLD_PACKET_HEADER
{	
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_INFO);	}
};


//-----------------------------------------------------------------------------
// 파티에 나갈 경우.
//-----------------------------------------------------------------------------
struct CTWS_PARTY_LOGOUTRT : public WORLD_PACKET_HEADER
{
	DWORD		dwPartyId;
	DWORD		dwUserIndex;

	CTWS_PARTY_LOGOUTRT() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_LOGOUTUSER; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_PARTY_LOGOUTRT); }
};


//-----------------------------------------------------------------------------
// 파티 HP.
//-----------------------------------------------------------------------------
struct WSTC_PARTY_HP : public WORLD_PACKET_HEADER
{
	DWORD		dwPartyId;
	DWORD		dwUserIndex;	
	WORD		wHp;
	WORD		wMaxHp;

	WSTC_PARTY_HP() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_PARTY_HP; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_PARTY_HP); }
};


struct WSTC_PARTY_PORTAL : public WORLD_PACKET_HEADER
{	
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_PORTAL()	{ bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_PARTY_PORTAL;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_PARTY_PORTAL)); }
};


struct WSTC_PARTY_PORTALUSED : public WORLD_PACKET_HEADER
{	
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	dwResult;
	BYTE	byIndex;
	BYTE	byType;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_PARTY_PORTALUSED()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_PORTALUSED;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_PARTY_PORTALUSED)); }
};


struct WSTC_PARTY_PORTALMOVE : public WORLD_PACKET_HEADER
{		
	WORD	wDungeonID;
	BYTE	byLayerIndex;
	float	fPosX;
	float	fPosZ;
	DWORD	dwPartyId;

	WSTC_PARTY_PORTALMOVE()	{ bGameStatus = UPDATE_GAME_WORLD;		bHeader = Protocol_World::CMD_PARTY_PORTALMOVE;	}
	DWORD	GetPacketSize()	{	return (sizeof(WSTC_PARTY_PORTALMOVE)); }
};


struct WSTC_HELP_INFO : public WORLD_PACKET_HEADER
{	
	BYTE	bHelpInfo[3];	

	WSTC_HELP_INFO()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_HELP_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_HELP_INFO);	}
};


struct WSTC_PARTY_PORTALFAIL : public WORLD_PACKET_HEADER
{	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byResult;
	
	WSTC_PARTY_PORTALFAIL()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_PORTALFAIL; }
	DWORD	GetPacketSize()	{ return sizeof(WSTC_PARTY_PORTALFAIL); }
};


//-----------------------------------------------------------------------------
// 이벤트 던젼 입장 실패.
//-----------------------------------------------------------------------------
struct WSTC_EVENT_DUNGEON_ENTER_FAILED : public WORLD_PACKET_HEADER
{
	BYTE	byErrorCode;	// 0 : 적합한 레벨이 아님.  1 : 파티 전용 던젼임.

	WSTC_EVENT_DUNGEON_ENTER_FAILED()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_EVENT_DUNGEON_ENTER_FAILED; }	
	DWORD	GetPacketSize()				{ return sizeof(WSTC_EVENT_DUNGEON_ENTER_FAILED);	}
};


struct WSTC_GUILD_OFFLINEUSER : public WORLD_PACKET_HEADER
{
	DWORD	dwUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];	
	WORD	wClass;
	DWORD	dwLevel;
	BYTE	byRank;
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];		

	WSTC_GUILD_OFFLINEUSER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_OFFLINE_USER; }
	DWORD	GetPacketSize()		{ return sizeof(WSTC_GUILD_OFFLINEUSER); }
};


struct WSTC_GUILD_OFFLINEUSER_DELETE : public WORLD_PACKET_HEADER
{
	DWORD	dwUserIndex;
	
	WSTC_GUILD_OFFLINEUSER_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_OFFLINE_USER_DELETE; }
	DWORD	GetPacketSize()		{ return sizeof(WSTC_GUILD_OFFLINEUSER_DELETE); }
};


struct CTWS_PARTY_USERREQUEST : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;
	DWORD		dwReqUserIndex;
	DWORD		dwPartyId;

	CTWS_PARTY_USERREQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USERREQUEST;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_PARTY_USERREQUEST);	}
};


struct WSTC_PARTY_USERREQUEST : public WORLD_PACKET_HEADER
{		
	DWORD		dwUserIndex;
	DWORD		dwReqUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	char		szRequestName[MAX_CHARACTER_NAME_LENGTH];		

	WSTC_PARTY_USERREQUEST()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USERREQUESTWS;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTC_PARTY_USERREQUEST);	}
};


struct WSTC_PARTY_USERREQUEST_FAIL : public WORLD_PACKET_HEADER
{		
	BYTE		byType;	

	WSTC_PARTY_USERREQUEST_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USERREQUEST_FAIL;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTC_PARTY_USERREQUEST_FAIL);	}
};


struct CTWS_PARTY_USERREQUEST_FAIL : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	BYTE		byType;	

	CTWS_PARTY_USERREQUEST_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_PARTY_USERREQUESTWS_FAIL;	}
	DWORD	GetPacketSize()			{	return sizeof(CTWS_PARTY_USERREQUEST_FAIL);	}
};



//-----------------------------------------------------------------------------
// 길드원 정보.
//-----------------------------------------------------------------------------
struct CTWS_GUILDUSER_INFO : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;	

	CTWS_GUILDUSER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDUSER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILDUSER_INFO);	}
};


//-----------------------------------------------------------------------------
// 길드원 정보.
//-----------------------------------------------------------------------------
struct WSTC_GUILDUSER_INFO : public WORLD_PACKET_HEADER
{
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_GUILDUSER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILDUSER_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDUSER_INFO);	}
};


struct CTWS_GUILD_REQUEST : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	DWORD		dwUserIndex;	
	DWORD		dwRequestUserIndex;

	CTWS_GUILD_REQUEST()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_REQUEST;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_GUILD_REQUEST);	}
};


struct WSTC_GUILD_REQUEST : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;	
	DWORD		dwRequestUserIndex;
	char		szName[MAX_CHARACTER_NAME_REAL_LENGTH];
	char		szRequestName[MAX_CHARACTER_NAME_REAL_LENGTH];

	WSTC_GUILD_REQUEST()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_REQUESTRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_REQUEST);	}
};


struct CTWS_GUILD_USERREQUEST_FAIL : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	BYTE		byType;	

	CTWS_GUILD_USERREQUEST_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_USERREQUESTWS_FAIL;	}
	DWORD	GetPacketSize()			{	return sizeof(CTWS_GUILD_USERREQUEST_FAIL);	}
};


struct WSTC_GUILD_USERREQUEST_FAIL : public WORLD_PACKET_HEADER
{		
	BYTE		byType;	

	WSTC_GUILD_USERREQUEST_FAIL()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_USERREQUEST_FAIL;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTC_GUILD_USERREQUEST_FAIL);	}
};


struct CTWS_GUILD_DATA_REQUEST : public WORLD_PACKET_HEADER
{		
	DWORD		dwGuildId;

	CTWS_GUILD_DATA_REQUEST()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_DATA_REQUEST;	}
	DWORD	GetPacketSize()			{	return sizeof(CTWS_GUILD_DATA_REQUEST);	}
};


struct WSTC_GUILD_DATA_REQUEST : public WORLD_PACKET_HEADER
{		
	DWORD		dwGuildId;
	char		szName[MAX_GUILD_NAME_LENGTH];

	WSTC_GUILD_DATA_REQUEST()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_DATA_REQUESTRT;	}
	DWORD	GetPacketSize()			{	return sizeof(WSTC_GUILD_DATA_REQUEST);	}
};


struct WSTC_GUILD_COUNT : public WORLD_PACKET_HEADER
{
	BYTE	byCount;
	BYTE	byMember[4];	
	
	WSTC_GUILD_COUNT()			{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_COUNT; }
	DWORD	GetPacketSize()		{ return sizeof(WSTC_GUILD_COUNT); }
};


struct WSTC_GUILD_EXP : public WORLD_PACKET_HEADER
{	
	DWORD		dwGuildId;
	DWORD		dwExp;
	WORD		wLevel;
	BYTE		byType;
	BYTE		byIndex;
	
	WSTC_GUILD_EXP()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GUILD_EXP;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_EXP); }
};


struct WSTC_GUILD_LEVEL : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	DWORD		dwUserIndex;
	DWORD		dwLevel;

	WSTC_GUILD_LEVEL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILD_LEVEL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILD_LEVEL); }
};


struct WSTC_RANK_FAIL : public WORLD_PACKET_HEADER
{
	BYTE		byType;
	BYTE		byRank;

	WSTC_RANK_FAIL()		{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_RANK_FAIL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_RANK_FAIL); }
};


struct WSTC_MESSENGER_USER : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;
	WORD		wClass;	
	BYTE		byStatus;

	WSTC_MESSENGER_USER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USER; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_USER); }
};


struct WSTC_MESSENGER_USERINFO : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	BYTE		byType;

	WSTC_MESSENGER_USERINFO() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USERINFO; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_USERINFO); }
};


struct CTWS_MESSENGER_USERADD : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	
	CTWS_MESSENGER_USERADD() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USERADD; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_MESSENGER_USERADD); }
};


struct WSTC_MESSENGER_USERFAIL : public WORLD_PACKET_HEADER
{	
	BYTE		byFail;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	
	WSTC_MESSENGER_USERFAIL()	{ bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USERFAIL; }
	DWORD	GetPacketSize()		{	return sizeof(WSTC_MESSENGER_USERFAIL); }
};


struct WSTC_MESSENGER_USERADD : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	
	WSTC_MESSENGER_USERADD() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_USERADDRT; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_USERADD); }
};


//-----------------------------------------------------------------------------
// 캐릭터 접속 여부 체크.
//-----------------------------------------------------------------------------
struct CTWS_CHARACTER_SEARCH : public WORLD_PACKET_HEADER
{	
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	CTWS_CHARACTER_SEARCH()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHARACTER_SEARCH;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_CHARACTER_SEARCH);	}
};


//-----------------------------------------------------------------------------
// 캐릭터 접속 여부 체크.
//-----------------------------------------------------------------------------
struct WSTC_SEARCH : public WORLD_PACKET_HEADER
{	
	BYTE	byMsg;
	BYTE	byType;	
	WORD	wDungeonID;
	DWORD	dwUserIndex;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_SEARCH()			{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHARACTER_SEARCHRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_SEARCH);	}
};


struct CTWS_MESSENGER_ANSWER : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;

	CTWS_MESSENGER_ANSWER() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_ANSWER; }
	DWORD	GetPacketSize()	{	return sizeof(CTWS_MESSENGER_ANSWER); }
};


struct WSTC_MESSENGER_FAIL : public WORLD_PACKET_HEADER
{
	char		szName[MAX_CHARACTER_NAME_LENGTH];	

	WSTC_MESSENGER_FAIL() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_FAIL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_FAIL); }
};


struct WSTC_MESSENGER_SUCCESS : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;
	WORD		wClass;

	WSTC_MESSENGER_SUCCESS() { bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_MESSENGER_SUCCESS; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_SUCCESS); }
};


struct WSTC_GUILDUSER_LEVEL : public WORLD_PACKET_HEADER
{
	DWORD		dwUserIndex;		
	DWORD		dwLevel;

	WSTC_GUILDUSER_LEVEL()	{	bGameStatus = UPDATE_GAME_WORLD; bHeader = Protocol_World::CMD_GUILDUSER_LEVEL; }
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GUILDUSER_LEVEL); }
};


struct CTWS_FRIENDUSER_INFO : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;	

	CTWS_FRIENDUSER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_FRIENDUSER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_FRIENDUSER_INFO);	}
};


struct WSTC_FRIENDUSER_INFO : public WORLD_PACKET_HEADER
{
	WORD		wDungeonID;
	BYTE		byLayerIndex;
	DWORD		dwUserIndex;
	char		szCharName[MAX_CHARACTER_NAME_LENGTH];

	WSTC_FRIENDUSER_INFO()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_FRIENDUSER_INFORT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_FRIENDUSER_INFO);	}
};


struct CTWS_FRIENDUSER_DELETE : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;	

	CTWS_FRIENDUSER_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_FRIENDUSER_DELETE;	}
	DWORD	GetPacketSize()		{	return sizeof(CTWS_FRIENDUSER_DELETE);	}
};


struct WSTS_FRIENDUSER_DELETE : public WORLD_PACKET_HEADER
{	
	DWORD		dwUserIndex;	

	WSTS_FRIENDUSER_DELETE()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_FRIENDUSER_DELETERT;	}
	DWORD	GetPacketSize()		{	return sizeof(WSTS_FRIENDUSER_DELETE);	}
};


struct CTWS_MESSENGER_STATUS : public WORLD_PACKET_HEADER
{
	BYTE		byType;
	DWORD		dwUserIndex;

	CTWS_MESSENGER_STATUS()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_MESSENGER_STATUS;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_MESSENGER_STATUS);	}
};

struct WSTC_MESSENGER_STATUS : public WORLD_PACKET_HEADER
{
	BYTE		byType;
	DWORD		dwUserIndex;

	WSTC_MESSENGER_STATUS()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_MESSENGER_STATUSRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_MESSENGER_STATUS);	}
};


//-----------------------------------------------------------------------------
// 길드마스터 이양.
//-----------------------------------------------------------------------------
struct WSTC_USER_MASTER : public WORLD_PACKET_HEADER
{	
	char		szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD		dwGuildId;
	DWORD		dwUserIndex;
	DWORD		dwMasterUserIndex;	
	BYTE		byMember[4];	
	BYTE		byType;

	WSTC_USER_MASTER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_USER_MASTER;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_USER_MASTER);	}
};


struct CTWS_USER_GUILDMASTER : public WORLD_PACKET_HEADER
{
	DWORD		dwGuildId;
	DWORD		dwUserIndex;	
	
	CTWS_USER_GUILDMASTER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_USER_GUILDMASTER;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_USER_GUILDMASTER);	}
};


struct WSTC_USER_GUILDMASTER : public WORLD_PACKET_HEADER
{
	char		szName[MAX_CHARACTER_NAME_LENGTH];
	char		szMasterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD		dwUserIndex;
	DWORD		dwMasterUserIndex;
	BYTE		byRank;
	BYTE		byMasterRank;

	WSTC_USER_GUILDMASTER()	{ bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_USER_GUILDMASTERRT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_USER_GUILDMASTER);	}
};


//-----------------------------------------------------------------------------
// CM툴에서 월드서버에게 해당 User를 강제 Logout 하라고 날림 
//-----------------------------------------------------------------------------
struct CTWS_KICK_USER : public WORLD_PACKET_HEADER
{
	DWORD	dwCharIndex;

	CTWS_KICK_USER()		{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CM_KICK_USER;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_KICK_USER);	}
};

//-----------------------------------------------------------------------------
// 클라이언트가 월드서버에게 현재 자신이 맵로딩중인지 아닌지를 알려준다. 
//-----------------------------------------------------------------------------
struct CTWS_LOADING_STATUS : public WORLD_PACKET_HEADER
{
	BYTE	bIsLoading;	

	CTWS_LOADING_STATUS()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_LOADING_STATUS;	}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_LOADING_STATUS);	}
};


//-----------------------------------------------------------------------------
// 등록된 GM이 아님 
//-----------------------------------------------------------------------------
struct WSTC_INVALID_GM : public WORLD_PACKET_HEADER
{
	WSTC_INVALID_GM()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_INVALID_GM;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_INVALID_GM);	}
};


//-----------------------------------------------------------------------------
// 등록된 GM임 
//-----------------------------------------------------------------------------
struct WSTC_GM_LOGIN_SUCCESS : public WORLD_PACKET_HEADER
{
	WSTC_GM_LOGIN_SUCCESS()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_GM_LOGIN_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_GM_LOGIN_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// GM의 실행환경을 체크한다. 
//-----------------------------------------------------------------------------
struct CTWS_CHECK_GM : public WORLD_PACKET_HEADER
{
	CTWS_CHECK_GM()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHECK_GM;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_CHECK_GM);	}	
};


//-----------------------------------------------------------------------------
// Client Connection Check용 KeepAlive Packet
//-----------------------------------------------------------------------------
struct ECHO_KEEP_ALIVE : public WORLD_PACKET_HEADER
{
	ECHO_KEEP_ALIVE()	{	bGameStatus = UPDATE_GAME_WORLD;	bHeader = Protocol_World::CMD_CHECK_CONNECTION;	}
	DWORD	GetPacketSize()	{	return sizeof(ECHO_KEEP_ALIVE);	}
};

#pragma pack(pop)