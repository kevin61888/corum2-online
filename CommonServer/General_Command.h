#ifndef __GENERAL_COMMAND_H__
#define __GENERAL_COMMAND_H__


#pragma once


#pragma pack(push,1)
enum GENERAL_COMMAND
{
	General_Command_Empty				= -1,
	General_Command_WHISPER				= 0,
	General_Command_MESSAGE				= 1,
	General_Command_REFUSAL				= 2,
	General_Command_APPROVAL			= 3,
	General_Command_GUARDIAN_ATTACK		= 4,
	General_Command_GUARDIAN_DEFENSE	= 5,
	General_Command_MONSTER_ATTACK		= 6,
	General_Command_MONSTER_DEFENSE		= 7,
	General_Command_PARTY				= 8,
	General_Command_GUILD				= 9,	
	General_Command_WHISPER2			= 10,
	General_Command_GUILD2				= 11,
	General_Command_PARTY2				= 12,
	General_Command_PARTYAPPROVE		= 13,
	General_Command_PARTYREFUSAL		= 14,
	General_Command_WHISPER3			= 15,
	General_Command_HELP				= 16,	
	General_Command_CHARACTERINFO		= 17,
	General_Command_BILL_REMAINTIME		= 18, 
};
#pragma pack(push,1)


struct GENERAL_CMD_PARAM
{
	enum GENERAL_COMMAND	eGeneralCommand;
};


#pragma pack(pop)


#endif
