#ifndef __DBPROC_H__
#define __DBPROC_H__

#pragma once


#include "GlobalDefine.h"



#pragma pack(push,1)
typedef struct __tag_sMessage__
{
	char		szFromName[MAX_CHARACTER_NAME_LENGTH];		
	DBTIMESTAMP sDate;
	char		szMessage[255];

}SMESSAGE, *LPSMESSAGE;


typedef struct __tag_guild_master__
{
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	WORD	wClass;
	DWORD	dwLevel;
	char	szGuildName[MAX_CHARACTER_NAME_LENGTH];
	char	szNickName[MAX_CHARACTER_NAME_LENGTH];
	BYTE	byType;
	BYTE	byRank;
	DWORD	dwGuildId;	

}GUILD_MASTER, LP_GUILD_MASTER;


typedef struct __tag_guild_id__
{
	DWORD		dwGuildId;
	char		szGuildName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;

}SGUILD_ID, *LP_GUILD_ID;


struct GUILD_CHK_TEMP
{	
	DWORD	dwUserIndex;
	BYTE	bType;	
	char	szName[MAX_CHARACTER_NAME_LENGTH];
};
#pragma pack(pop)



void QueryTypeCharacterInfo(DBRECEIVEDATA* pResult);
void QueryTypeUserInfo(DBRECEIVEDATA* pResult);
void QueryMessenger(DBRECEIVEDATA* pResult);
void QueryTypeMessage(DBRECEIVEDATA* pResult, BYTE byQueryType);
void QueryTypeMessageRQ(DBRECEIVEDATA* pResult);
void QueryGuildId(DBRECEIVEDATA* pResult);
void QueryGuilChk(DBRECEIVEDATA* pResult);
void QueryTypeGMCheck(DBRECEIVEDATA* pResult);
void QueryTypeWorldChk(DBRECEIVEDATA* pResult);
void QueryTypePartyFriend(DBRECEIVEDATA* pResult);
void QueryTypeLadderInfo(DBRECEIVEDATA* pResult);
#endif 