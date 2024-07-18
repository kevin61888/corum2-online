#pragma once

#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_common\\math.inl"
#else
#include "./../4DyuchiGRX_common\\math.inl"
#endif
#include "ProtocolDefinition.h"	
#include "GM_Command.h"


#pragma pack(push,1)


//-----------------------------------------------------------------------------
// 던전에 Join 메세지를 날림 
//-----------------------------------------------------------------------------
struct CTDS_DUNGEON_JOIN : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;							// 월드 서버에서 받은 유저 인덱스. 인증시 필요.
	DWORD	dwPartyID;		
	WORD	wDungeonID;								// 들어가고픈 던전 번호.
	BYTE	bIsPortalJoin;							// 포탈로 Join시에는 1
	BYTE	bLayerNum;								// 포탈 Join 시에만 사용, 이동하고자하는 층수 포탈이 아닐때에는 기본값 -1;
	VECTOR2 v2Pos;									// 포탈 Join 시에만 사용, 이동하고자하는 특정 위치 
	WORD	wPrevDungeonID;							// 들어가기전 던전 아이디 
	char	szChrName[MAX_CHARACTER_NAME_LENGTH];	// 정보를 받아올때 필요하다.

	CTDS_DUNGEON_JOIN()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_DUNGEON_JOIN;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_DUNGEON_JOIN);	}
};


struct CTDS_DUNGEON_OUT : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;

	CTDS_DUNGEON_OUT()		{	bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_DUNGEON_OUT;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_DUNGEON_OUT);	}
};


struct CTDS_DUNGEON_MOVE : public GAME_PACKET_HEADER
{
	BYTE	bMoveType;			// Move types
	VECTOR2	v2MoveStartPos;		// Start position
	VECTOR2	v2MoveDirection;	// Direction	
	WORD	wDestX;
	WORD	wDestZ;

	CTDS_DUNGEON_MOVE()		{	bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_DUNGEON_MOVE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_DUNGEON_MOVE);	}
};


struct CTDS_DUNGEON_STOP : public GAME_PACKET_HEADER
{
	VECTOR2	v2StopPos;			// Stop position
//	BOOL	bCollision;			// 안쓴다! by minjin with deepdark
	DWORD	dwMonsterIndex;		// Collsion Monster Index

	CTDS_DUNGEON_STOP()		{	bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_DUNGEON_STOP;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_DUNGEON_STOP);	}
};


struct CTDS_DUNGEON_ATTACK_USER_MON : public GAME_PACKET_HEADER
{
	DWORD	dwMonsterIndex;		// 터지고 있는 몬스터의 id
	VECTOR2	v2UserPos;
	VECTOR2	v2MonsterPos;
	
	CTDS_DUNGEON_ATTACK_USER_MON()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_DUNGEON_ATTACK_USER_MON;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_DUNGEON_ATTACK_USER_MON);	}
};


struct	CTDS_DUNGEON_ATTACK_USER_USER : public GAME_PACKET_HEADER
{	
	DWORD	dwUserIndex;		// 터지고 있는 유저의 id
	VECTOR2	v2OffenseUserPos;
	VECTOR2	v2DefenseUserPos;

	CTDS_DUNGEON_ATTACK_USER_USER()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_DUNGEON_ATTACK_USER_USER;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_DUNGEON_ATTACK_USER_USER);	}
};


struct	CTDS_DUNGEON_CHAT : public GAME_PACKET_HEADER
{
	BYTE	bChatType;
	BYTE	bChatStrLength;
	char	szChatStr[255];
	
	CTDS_DUNGEON_CHAT()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_DUNGEON_CHAT;	}
	DWORD	GetPacketSize()	{	return (sizeof(CTDS_DUNGEON_CHAT) - ( 255 - bChatStrLength ) ); }
};


//------------------------------------------------------------------------------//
// 마을, 던젼에서 스킬 레벨업을 하였을 경우 서버로 보냄.					
//------------------------------------------------------------------------------//
struct CTDS_SKILL_LEVELUP : public GAME_PACKET_HEADER
{
	int		nSkillIndex;

	CTDS_SKILL_LEVELUP()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_SKILL_LEVELUP;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_SKILL_LEVELUP);	}
};


struct	CTDS_DUNGEON_CASTING : public GAME_PACKET_HEADER
{
	BYTE	bOwnType;
	BYTE	bTargetType;
	BYTE	bSkillKindLR;	
	DWORD	dwTargetIndex;		// 타겟 인덱스.
	WORD	wTileIndex_X;
	WORD	wTileIndex_Z;
		
	CTDS_DUNGEON_CASTING()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_DUNGEON_CASTING;	}
	DWORD	GetPacketSize()	{	return (sizeof(CTDS_DUNGEON_CASTING)); }
};


struct CTDS_ITEM_PICKUP : public GAME_PACKET_HEADER
{
	DWORD	dwMoney;			// 돈을 보낼때.
	DWORD	dwId;
	BYTE	bInv;				// 어디에 넣을것인가.
	BYTE	bZipCode;			// 몇번쨰 블럭에 넣을것인가.
	BYTE	bEquipCode;			// 몇번째 장착블럭에 넣을것인가.
	BYTE	bZipCode2;			// 몇번쨰 블럭에 넣을것인가.
	BYTE	bEqiupCode2;		// 몇번째 장착블럭에 넣을것인가.
	BYTE	bSectionNum;		// 아이템이 위치하고 있는 섹션번호.
	__int64	i64ItemID;
	float	fItemX;
	float	fItemY;
	float	fItemZ;
	DWORD	dwShop;
	
	CTDS_ITEM_PICKUP()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_ITEM_PICKUP; }
	DWORD	GetPacketSize()		{	return sizeof(CTDS_ITEM_PICKUP);	}
};


struct CTDS_ITEM_USED : public GAME_PACKET_HEADER
{
	BYTE	bInv;				// 어느곳인가.	1 - Small Inv, 2 - Belt
	BYTE	bZipCode;			// 몇번쨰 블럭의 아이템을 사용할것인가.
	
	CTDS_ITEM_USED()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_ITEM_USED;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_ITEM_USED);	}
};


//------------------------------------------------------------------------------//
// 마을, 던젼에서 Status 레벨업을 하였을 경우 서버로 보냄.				
//------------------------------------------------------------------------------//
struct CTDS_CHAR_LEVELUP : public GAME_PACKET_HEADER
{
	BYTE	byStatusType;
	BYTE	byObjectType;

	CTDS_CHAR_LEVELUP()			{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_STATUS_LEVELUP;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_CHAR_LEVELUP);	}
};


struct CTDS_DUNGEON_CHANGE_LRSKILLKIND : public GAME_PACKET_HEADER
{
	BYTE	bySelectSkillKind[2];

	CTDS_DUNGEON_CHANGE_LRSKILLKIND()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_DUNGEON_CHANGE_LRSKILLKIND;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_DUNGEON_CHANGE_LRSKILLKIND);	}	
};


//------------------------------------------------------------------------------//
// 길드 생성 아이템을 사용 했을 경우.										
//------------------------------------------------------------------------------//
struct CTDS_GUILD_ITEM : public GAME_PACKET_HEADER
{
	BYTE	bZipCode;
	BYTE	bGuildType;
	
	CTDS_GUILD_ITEM()			{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_GUILD_ITEM;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_GUILD_ITEM);	}	
};


struct CTDS_DUNGEON_REQUEST_INFO : public GAME_PACKET_HEADER
{
	BYTE	bRequestType;
	DWORD	dwRequestIndex;
	
	CTDS_DUNGEON_REQUEST_INFO()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_DUNGEON_REQUEST_INFO; }
	DWORD	GetPacketSize()			{	return sizeof(CTDS_DUNGEON_REQUEST_INFO);	}
};


//------------------------------------------------------------------------------//
// 길드 유저 정보.														
//------------------------------------------------------------------------------//
struct CTDS_GUILD_USERDATA : public GAME_PACKET_HEADER
{
	char	szGuildName[MAX_GUILD_NAME_LENGTH];
	char	szNickName[MAX_GUILD_NAME_LENGTH];	
	BYTE	byType;
	BYTE	byRank;	
	DWORD	dwGuildId;

	CTDS_GUILD_USERDATA()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_GUILD_USERDATA;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_GUILD_USERDATA);	}	
};


struct CTDS_DUNGEON_CALLGARDIAN : public GAME_PACKET_HEADER
{
	BYTE	bZipCode;

	CTDS_DUNGEON_CALLGARDIAN()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_DUNGEON_CALLDUARDIAN; }
	DWORD	GetPacketSize()			{	return sizeof( CTDS_DUNGEON_CALLGARDIAN );	}
};


struct CTDS_GUARDIAN_ATTACK : public GAME_PACKET_HEADER	
{
	BYTE	bZipCode;
	BYTE	bAttackType;		// Monster = 0? Player = 1?	...
	DWORD	dwTargetIndex;		// Player Index = Connection Index

	CTDS_GUARDIAN_ATTACK()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_GUARDIAN_ATTACK;	}
	DWORD	GetPacketSize()			{	return sizeof( CTDS_GUARDIAN_ATTACK );	}
};


struct CTDS_GUARDIAN_COMMAND : public GAME_PACKET_HEADER
{
	BYTE	bZipCode;
	BYTE	bCommand;			// 공통적으로 쓰는 명령.	0 - 방어모드로 전환.

	CTDS_GUARDIAN_COMMAND()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_GUARDIAN_COMMAND;	}
	DWORD	GetPacketSize()			{	return sizeof( CTDS_GUARDIAN_COMMAND );	}
};


struct CTDS_MONSTER_COMMAND : public GAME_PACKET_HEADER
{
	BYTE	bCommand;			// 0 - Defense  1 - Attack	
	BYTE	bAttackType;		// Monster = 0? Player = 1?	...
	DWORD	dwTargetIndex;		// Player Index = Connection Index

	CTDS_MONSTER_COMMAND()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_MONSTER_COMMAND;	}
	DWORD	GetPacketSize()			{	return sizeof( CTDS_MONSTER_COMMAND );	}
};


struct CTDS_GUARDIAN_COMMAND_MOVE : public GAME_PACKET_HEADER
{
	VECTOR2		v2DestPos;		// 목적지.
	DWORD		pdwGuardianIndex[MAX_USER_GUARDIAN];
	DWORD		pdwMonsterIndex[MAX_USER_GUARDIAN];

	CTDS_GUARDIAN_COMMAND_MOVE()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_GUARDIAN_COMMAND_MOVE; }
	DWORD	GetPacketSize()			{	return sizeof( CTDS_GUARDIAN_COMMAND_MOVE ); }
};


struct CTDS_GUARDIAN_COMMAND_ATTACK : public GAME_PACKET_HEADER
{
	BYTE		bAttackType;		// Monster = 0? Player = 1?	...
	DWORD		dwTargetIndex;		// Player Index = Connection Index
	DWORD		pdwGuardianIndex[MAX_USER_GUARDIAN];
	DWORD		pdwMonsterIndex[MAX_USER_GUARDIAN];

	CTDS_GUARDIAN_COMMAND_ATTACK()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_GUARDIAN_COMMAND_ATTACK; }
	DWORD	GetPacketSize()			{	return sizeof( CTDS_GUARDIAN_COMMAND_ATTACK );	}
};

struct CTDS_GUARDIAN_SKILL_SELECT : public GAME_PACKET_HEADER
{
	BYTE		bSkill;
	
	CTDS_GUARDIAN_SKILL_SELECT()	{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_GUARDIAN_SKILL_SELECT; }
	DWORD	GetPacketSize()			{	return sizeof( CTDS_GUARDIAN_SKILL_SELECT );	}
};

struct CTDS_SKILL : public GAME_PACKET_HEADER 
{
	BYTE	bSkillKind;
	BYTE	bOwnType;
	BYTE	bTargetType;	// OBJECT_TYPE_PLAYER, OBJECT_TYPE_MONSTER, OBJECT_TYPE_TILE
	BYTE	bPK;			// pk 모드인지..
	DWORD	dwOwnIndex;
	DWORD	dwTargetIndex;
	WORD	wTileIndex_X;	// 혹시 대상이 사라졌을때를 위해 좌표를 알아야 함..
	WORD	wTileIndex_Z;	// 혹시 대상이 사라졌을때를 위해 좌표를 알아야 함..
	DWORD	dwTime;			// 적용되어질 타임.
	
	CTDS_SKILL()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_SKILL;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_SKILL);	}	
};


//------------------------------------------------------------------------------//
// 길드 유저 정보.														
//------------------------------------------------------------------------------//
struct CTDS_GUILD_JOININFO : public GAME_PACKET_HEADER
{
	char	szGuildName[MAX_GUILD_NAME_LENGTH];		
	BYTE	byType;
	BYTE	byRank;
	DWORD	dwGuildId;
	
	CTDS_GUILD_JOININFO()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_GUILD_JOININFO;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_GUILD_JOININFO);	}
};


//------------------------------------------------------------------------------//
// 캐릭터 정보 변경.														
//------------------------------------------------------------------------------//
struct CTDS_GUILD_INFO : public GAME_PACKET_HEADER
{
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];	
	BYTE	byRank;

	CTDS_GUILD_INFO()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_GUILD_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_GUILD_INFO);	}
};


//------------------------------------------------------------------------------//
// 아이템 트레이드 요청.													
//------------------------------------------------------------------------------//
struct CTDS_TRADE_REQUEST : public GAME_PACKET_HEADER
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	DWORD	dwUserIndex;	
	BYTE	byCode;

	CTDS_TRADE_REQUEST()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_TRADE_REQUEST;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_TRADE_REQUEST);	}
};


//------------------------------------------------------------------------------//
// 아이템 트레이드 결과값.													
//------------------------------------------------------------------------------//
struct CTDS_TRADE_RESULT : public GAME_PACKET_HEADER
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];	
	DWORD	dwUserIndex;
	BYTE	byCode;

	CTDS_TRADE_RESULT()			{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_TRADE_RESULT;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_TRADE_RESULT);	}
};


struct CTDS_TRADE_CALCEL : public GAME_PACKET_HEADER
{
	BYTE	bType;	

	CTDS_TRADE_CALCEL()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_TRADE_CANCEL;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_TRADE_CALCEL);	}
};


struct CTDS_TRADE_OK : public GAME_PACKET_HEADER
{
	BYTE	bType;
	DWORD	dwMoney;

	CTDS_TRADE_OK()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_TRADE_OK;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_TRADE_OK);	}
};


//------------------------------------------------------------------------------//
// 트레이드.																
//------------------------------------------------------------------------------//
struct CTDS_TRADE_TRADE : public GAME_PACKET_HEADER
{
	BYTE	bType;	
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];

	CTDS_TRADE_TRADE()			{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_TRADE_TRADE;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_TRADE_TRADE);	}
};


struct CTDS_EMOTICON : public GAME_PACKET_HEADER
{
	BYTE	bWhatEmoticon;
	
	CTDS_EMOTICON()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_EMOTICON;	}
	DWORD GetPacketSize()	{	return sizeof(CTDS_EMOTICON);	}
};


struct	CTDS_DUNGEON_PKMODE : public GAME_PACKET_HEADER
{
	BYTE	bPKMode;
		
	CTDS_DUNGEON_PKMODE()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_DUNGEON_PKMODE;	}
	DWORD	GetPacketSize()	{	return (sizeof(CTDS_DUNGEON_PKMODE)); }
};


// 노점상 열어.
struct CTDS_PLAYERSHOP_OPEN : public GAME_PACKET_HEADER
{
	char	szTitle[MAX_PLAYER_SHOP_TITLE];					// 노점상 제목
	
	CTDS_PLAYERSHOP_OPEN()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_PLAYERSHOP_OPEN;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_PLAYERSHOP_OPEN);	}	
};


// 노점상 닫아.
struct CTDS_PLAYERSHOP_CLOSE : public GAME_PACKET_HEADER 
{
	CTDS_PLAYERSHOP_CLOSE()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_PLAYERSHOP_CLOSE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_PLAYERSHOP_CLOSE);	}	
};


struct CTDS_REQUEST_MATCH_MAN2MAN : public GAME_PACKET_HEADER
{
	DWORD	dwTargetUserIndex;
	WORD	wVillageID;

	CTDS_REQUEST_MATCH_MAN2MAN()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_REQUEST_MATCH_MAN2MAN;	}	
	DWORD GetPacketSize()	{	return sizeof(CTDS_REQUEST_MATCH_MAN2MAN);	}	
};


struct CTDS_REPLY_MATCH_MAN2MAN : public GAME_PACKET_HEADER
{
	BYTE	bReplyCode;
	DWORD	dwRequestUserIndex;
	
	CTDS_REPLY_MATCH_MAN2MAN()	{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_REPLY_MATCH_MAN2MAN;	bReplyCode = 0; }	
	DWORD GetPacketSize()	{	return sizeof(CTDS_REPLY_MATCH_MAN2MAN);	}	
};


// 그놈 노점상 아템 보여줘.
struct CTDS_PLAYERSHOP_SHOWALL : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;

	CTDS_PLAYERSHOP_SHOWALL()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_PLAYERSHOP_SHOWALL;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_PLAYERSHOP_SHOWALL);	}	
};


// 맵로딩 끝낫다.
struct CTDS_FINISH_MAPLOADING : public GAME_PACKET_HEADER
{
	CTDS_FINISH_MAPLOADING()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_FINISH_MAPLOADING;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_FINISH_MAPLOADING);	}	
};


// 대전이 준비됐음을 알린다. 
struct CTDS_READY_MATCH_MAN2MAN : public GAME_PACKET_HEADER
{
	CTDS_READY_MATCH_MAN2MAN() 	{ bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_READY_MATCH_MAN2MAN; }
	DWORD GetPacketSize()	{	return sizeof(CTDS_READY_MATCH_MAN2MAN);	}
};


struct CTDS_ITEMUPGRADE : public GAME_PACKET_HEADER
{
	CTDS_ITEMUPGRADE()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_ITEMUPGRADE;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_ITEMUPGRADE);	}
};


// 던전 입장료 획득해라.
struct CTDS_ACCENTRANCE_ACQUISITION : public GAME_PACKET_HEADER
{
	WORD	wDungeonID;
	
	CTDS_ACCENTRANCE_ACQUISITION()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_ACCENTRANCE_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_ACCENTRANCE_ACQUISITION);	}	
}; 


// 던전 마법진 획득해라.
struct CTDS_PRODUCT_ACQUISITION : public GAME_PACKET_HEADER
{
	WORD	wDungeonID;

	CTDS_PRODUCT_ACQUISITION()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_PRODUCT_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_PRODUCT_ACQUISITION);	}	
}; 


// 던전 누적된 경험치를 획득해라.
struct CTDS_ACCEXP_ACQUISITION : public GAME_PACKET_HEADER
{
	WORD	wDungeonID;

	CTDS_ACCEXP_ACQUISITION()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_ACCEXP_ACQUISITION;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_ACCEXP_ACQUISITION);	}	
}; 


// 던전 입장료 수정해라.
struct CTDS_ENTRANCE_EDIT : public GAME_PACKET_HEADER
{
	WORD	wDungeonID;
	DWORD	dwMoney;

	CTDS_ENTRANCE_EDIT()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_ENTRANCE_EDIT;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_ENTRANCE_EDIT);	}	
}; 


// 나 이제 죽을래
struct CTDS_USER_DIE_OK : public GAME_PACKET_HEADER
{
	CTDS_USER_DIE_OK()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_USER_DIE_OK;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_USER_DIE_OK);	}	
}; 


struct CTDS_ITEMMAKING : public GAME_PACKET_HEADER
{
	CTDS_ITEMMAKING()	{ bStatus = UPDATE_GAME_PLAY;		bHeader = Protocol_CTS::CMD_ITEMMAKING;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_ITEMMAKING);	}
};


struct PORTAL_BUCKET
{
	WORD	wPortalItemID;		//사용한 포탈 아이템 ID
	WORD	wToDungeonID;		//이동하고자 하는 DungeonID
	BYTE	bLayerNo;			//그 던전의 LayerID
	VECTOR2	v2Pos;	
	DWORD	dwConnectIP;		
	WORD	wPort;
};


//------------------------------------------------------------------------------//
// 포탈 이동 첫번째 패킷 
//------------------------------------------------------------------------------//
struct CTDS_PORTAL_MOVE : public GAME_PACKET_HEADER
{
	WORD	wPortalItemID;		// 포탈 아이템 아이디
	WORD	wToDungeonID;		// 이동하고자 하는 DungeonID
	BYTE	bLayerNo;			// 그 던전의 층수
	VECTOR2 v2Pos;				// 이동하고자 하는곳의 좌표
	BYTE	bPortalType;		// 포탈 타입. 0 : 일반. 1 : by NPC.
	WORD	wPrevDungeonID;		// 들어가기전 던전 아이디

	DWORD	dwTargetUserIndex;	// GM 명령에 특정유저에게로 이동할때 필요 
	
	CTDS_PORTAL_MOVE()		
	{ 
		bStatus = UPDATE_GAME_PLAY; 
		bHeader = Protocol_CTS::CMD_PORTAL_MOVE; 
		wPortalItemID = 0; 
		dwTargetUserIndex = 0;
	}	
	DWORD GetPacketSize()	{	return sizeof(CTDS_PORTAL_MOVE);	}
};


//------------------------------------------------------------------------------//
// 포탈 이동하는 놈이 맵로딩이 끝나면 던전서버에 SectionInfo를 요구한다.  
//------------------------------------------------------------------------------//
struct CTDS_REQUEST_SECTION_INFO_FOR_PORTAL : public GAME_PACKET_HEADER
{
	WORD	wDungeonID;
	BYTE	bLayerIndex;
	BYTE	bJoinDungeon;		// 던전간 이동하는 포탈인경우 이동한 던전에 Join 하라는 플래그 

	CTDS_REQUEST_SECTION_INFO_FOR_PORTAL()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_REQUEST_SECTION_INFO_FOR_PORTAL;	}
	DWORD GetPacketSize()	{	return sizeof(CTDS_REQUEST_SECTION_INFO_FOR_PORTAL);	}
};


//------------------------------------------------------------------------------//
// 파티 포탈.																
//------------------------------------------------------------------------------//
struct CTDS_PARTY_PORTAL : public GAME_PACKET_HEADER
{
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byZipCode;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	CTDS_PARTY_PORTAL()			{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_PARTY_PORTAL;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_PARTY_PORTAL);	}
};


struct CTDS_PARTY_PORTALUSED : public GAME_PACKET_HEADER
{
	DWORD	dwPartyId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byResult;
	BYTE	byIndex;
	BYTE	byType;

	CTDS_PARTY_PORTALUSED()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_PARTY_PORTALUSED;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_PARTY_PORTALUSED);	}
};


struct CTDS_PARTY_PORTALFAIL : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byResult;

	CTDS_PARTY_PORTALFAIL()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_PARTY_PORTALFAIL;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_PARTY_PORTALFAIL);	}
};


struct CTDS_UPGRADE_FAIL : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;	
	
	CTDS_UPGRADE_FAIL()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_UPGRADE_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_UPGRADE_FAIL);	}
};


//-----------------------------------------------------------------------------
// 이벤트 던젼처음 입장했을때 어느마을에서 포탈을 탔는지 던전서버에 알려준다.
//-----------------------------------------------------------------------------
struct CTDS_EVENT_USER_PREV_DUNGEON_ID : public GAME_PACKET_HEADER
{
	WORD	wPrevDungeonID;
	
	CTDS_EVENT_USER_PREV_DUNGEON_ID()	{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_CTS::CMD_EVENT_USER_PREV_DUNGEON_ID; }	
	DWORD	GetPacketSize()				{ return sizeof(CTDS_EVENT_USER_PREV_DUNGEON_ID); }
};


//-----------------------------------------------------------------------------
// 이벤트 던젼 클리어 했을떄 이펙트가 끝나면 던전서버로 알려준다.
//-----------------------------------------------------------------------------
struct CTDS_EVENT_CLEAR_EFFECT_FINISH : public GAME_PACKET_HEADER
{
	CTDS_EVENT_CLEAR_EFFECT_FINISH()	{ bStatus = UPDATE_GAME_PLAY;  bHeader = Protocol_CTS::CMD_EVENT_CLEAR_EFFECT_FINISH; }	
	DWORD	GetPacketSize()				{ return sizeof(CTDS_EVENT_CLEAR_EFFECT_FINISH); }
};


struct CTDS_MAKING_CANCEL : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;	
	
	CTDS_MAKING_CANCEL()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_MAKING_CANCEL;	}
	DWORD	GetPacketSize()	{	return sizeof(CTDS_MAKING_CANCEL);	}
};


struct CTDS_FIND_HACK : public GAME_PACKET_HEADER
{
	DWORD	dwHackNum;

	CTDS_FIND_HACK()	{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_FIND_HACK;	}
	DWORD GetPacketSize()	{	return sizeof(CTDS_FIND_HACK);	}
};


struct CTDS_TICK_CHECK : public GAME_PACKET_HEADER
{
	DWORD	dwCurClientTick;
	
	CTDS_TICK_CHECK()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_TICK_CHECK;	}
	DWORD GetPacketSize()	{ return sizeof( CTDS_TICK_CHECK );	}
};


struct CTDS_NEW_GM_COMMAND : public GAME_PACKET_HEADER
{
	union
	{
		GM_CMD_PARAM					sBaseGMCmdParam;
		GM_CMD_PARAM_Create_Item		sGMCmdParamCreateItem;
		GM_CMD_PARAM_Summon_Monster		sGMCmdParamSummonMonster;
		GM_CMD_PARAM_Silence			sGMCmdParamSilence;
		GM_CMD_PARAM_Recall_Someone		sGMCmdParamRecallSomeone;
		GM_CMD_PARAM_Get_Move_Someone	sGMCmdParamGetMoveSomeone;
		GM_CMD_PARAM_Dungeon_Portal		sGMCmdParamDungeonPortal;
		GM_CMD_PARAM_PK_Mode			sGMCmdParamPKMode;
		GM_CMD_PARAM_Observe			sGMCmdParamObserve;
		GM_CMD_PARAM_Join_Guild			sGMCmdParamJoinGuild;
		GM_CMD_PARAM_Change_Monster		sGMCmdParamChangeMonster;		
	};
		
	CTDS_NEW_GM_COMMAND()	{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_GM_COMMAND;	}
	DWORD GetPacketSize()	{ return sizeof( CTDS_NEW_GM_COMMAND );	}
};


struct CTDS_GM_COMMAND : public GAME_PACKET_HEADER
{
	WORD	wCommand;	
	DWORD	dwTemp[10];
	
	CTDS_GM_COMMAND()		{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_GM_COMMAND;	}
	DWORD GetPacketSize()	{ return sizeof( CTDS_GM_COMMAND );	}
};


//------------------------------------------------------------------------------//
// 플레이어 앉기.
//------------------------------------------------------------------------------//
struct CTDS_PLAYER_REST : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;
	BYTE	byType;
	
	CTDS_PLAYER_REST()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_PLAYER_REST;	}
	DWORD GetPacketSize()	{	return sizeof(CTDS_PLAYER_REST);	}
};


struct CTDS_GUILD_PORTALUSED : public GAME_PACKET_HEADER
{
	DWORD	dwGuildId;
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byResult;	
	BYTE	byIndex;
	BYTE	byType;

	CTDS_GUILD_PORTALUSED()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_GUILD_PORTALUSED;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_GUILD_PORTALUSED);	}
};


struct CTDS_GUILD_PORTALFAIL : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;
	DWORD	dwPartalUserIndex;
	BYTE	byResult;

	CTDS_GUILD_PORTALFAIL()		{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_GUILD_PORTALFAIL;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_GUILD_PORTALFAIL);	}
};


struct CTDS_WORLD_MOVE : public GAME_PACKET_HEADER
{
	DWORD	dwUserIndex;

	CTDS_WORLD_MOVE()			{	bStatus = UPDATE_GAME_PLAY;	bHeader = Protocol_CTS::CMD_WORLD_MOVE;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_WORLD_MOVE);	}
};


struct CTDS_ITEM_MOVE : public GAME_PACKET_HEADER
{
	BYTE	bSrc;
	BYTE	bDest;
	BYTE	bSrcIndex;
	BYTE	bDestIndex;
	DWORD	dwUnitID;
	WORD	wPlayerShopItemID;	
	DWORD	dwMoney;
	BYTE	bQuantity;
	INT64	i64ItemID;
	BYTE	bSectionNum;
		
	CTDS_ITEM_MOVE()			{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_ITEM_MOVE;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_ITEM_MOVE);		}
};


struct CTDS_ITEM_USED_NATIVE : public GAME_PACKET_HEADER
{
	BYTE	bSrc;
	BYTE	bSrcIndex;
	BYTE	bCommonIndex;	
	DWORD	dwCommonId;
	DWORD	dwPortalUserIndex;
	
	CTDS_ITEM_USED_NATIVE()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_ITEM_USED_NATIVE;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_ITEM_USED_NATIVE);	}
};


struct CTDS_PARTY_PORTAL_RECOGNITION : public GAME_PACKET_HEADER
{
	BYTE	bySlotIndex;
	BYTE	byRecallType;
	DWORD	dwPortalUserIndex;

	CTDS_PARTY_PORTAL_RECOGNITION()	{ bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_PARTY_PORTAL_RECOGNITION; }
	DWORD	GetPacketSize()		{	return sizeof(CTDS_PARTY_PORTAL_RECOGNITION);	}
};


struct CTDS_SWITCH_WEAPON : public GAME_PACKET_HEADER
{
	CTDS_SWITCH_WEAPON()		{	bStatus = UPDATE_GAME_PLAY; bHeader = Protocol_CTS::CMD_SWITCH_WEAPON;	}
	DWORD	GetPacketSize()		{	return sizeof(CTDS_SWITCH_WEAPON);	}
};


#pragma pack(pop)