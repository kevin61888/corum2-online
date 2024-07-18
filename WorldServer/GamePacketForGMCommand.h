#pragma once
#include "GM_Command.h"
#include "Define.h"
#include "ProtocolDefinition.h"	

#pragma pack(push,1)

struct GM_CMD_PARAM
{
	enum GM_COMMAND		eGMCommand;
};


//
// GM_CMD_PARAM_Create_Item 
//
struct GM_CMD_PARAM_Create_Item : public GM_CMD_PARAM
{
	WORD wItemID;	
};

//
// GM_CMD_PARAM_Summon_Monster
//
struct GM_CMD_PARAM_Summon_Monster : public GM_CMD_PARAM
{
	WORD	wMonsterID;
	WORD	wMonsterCount;
};

//
// GM_CMD_PARAM_Silence 
//
struct GM_CMD_PARAM_Silence : public GM_CMD_PARAM
{
	BYTE	bFlag;
	BYTE	bAll;
	
	DWORD	dwUserIndex;
};

//
// GM_CMD_PARAM_Recall_Someone 
//
struct GM_CMD_PARAM_Recall_Someone : public GM_CMD_PARAM
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
};


//
// GM_CMD_PARAM_Get_Move_Someone 
//
struct GM_CMD_PARAM_Get_Move_Someone : public GM_CMD_PARAM
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];		

};


//
// GM_CMD_PARAM_Dungeon_Portal
//
struct GM_CMD_PARAM_Dungeon_Portal : public GM_CMD_PARAM
{
	WORD	wDungeonID;		// 어느던전			
	BYTE	bLayerFloor;	// 몇 층

};

//
// GM_CMD_PARAM_PK_Mode 
//
struct GM_CMD_PARAM_PK_Mode : public GM_CMD_PARAM
{
	BYTE	bPKMode;
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
};


//
// GM_CMD_PARAM_Observe 
//
struct GM_CMD_PARAM_Observe : public GM_CMD_PARAM
{
	BYTE	bObserveFlag;		// TRUE : disappear, FALSE : appear
};


//
// GM_CMD_PARAM_Join_Guild
//
struct GM_CMD_PARAM_Join_Guild : public GM_CMD_PARAM
{
	char	szGuildName[MAX_GUILD_NAME_REAL_LENGTH];
};


//
// GM_CMD_PARAM_Change_Monster 
//
struct GM_CMD_PARAM_Change_Monster : public GM_CMD_PARAM
{
	WORD	wChangeMonsterID;
};

struct CTDS_NEW_GM_COMMAND
{
	BYTE	bStatus;
	BYTE	bHeader;

	union
	{
		GM_CMD_PARAM				sBaseGMCmdParam;
		GM_CMD_PARAM_Create_Item	sGMCmdParamCreateItem;
		GM_CMD_PARAM_Summon_Monster	sGMCmdParamSummonMonster;
		GM_CMD_PARAM_Silence		sGMCmdParamSilence;
		GM_CMD_PARAM_Recall_Someone	sGMCmdParamRecallSomeone;
		GM_CMD_PARAM_Get_Move_Someone sGMCmdParamGetMoveSomeone;
		GM_CMD_PARAM_Dungeon_Portal	sGMCmdParamDungeonPortal;
		GM_CMD_PARAM_PK_Mode		sGMCmdParamPKMode;
		GM_CMD_PARAM_Observe		sGMCmdParamObserve;
		GM_CMD_PARAM_Join_Guild		sGMCmdParamJoinGuild;
		GM_CMD_PARAM_Change_Monster	sGMCmdParamChangeMonster;		
	};
		
	CTDS_NEW_GM_COMMAND()		
	{ 
		bStatus = UPDATE_GAME_PLAY; 
		bHeader = Protocol_CTS::CMD_GM_COMMAND;	
	}
	DWORD GetPacketSize()	{ return sizeof( CTDS_NEW_GM_COMMAND );	}
};


#pragma pack(pop)