/*#####################################################################//
//##################### 2004. 11. 클래스화 예정.. #####################
//#####################################################################
//
//	Version		: Dev/Develop
//	ProjectName	: CorumOnlineProject
//	FileName	: RivalGuildWar.h
//	Date		: 2004. 10. 18. ( minjin )
//	Description	: Declare Rival Guild War Global Function
//
//#####################################################################*/

#ifndef __RIVAL_GUILD_WAR__
#define __RIVAL_GUILD_WAR__

#include "User.h"


const int MT_DEAH_MATCH			= 1;
const int MT_KILLING_FIELD		= 2;
const int MT_KING_OF_THE_HILL	= 3;
const int MT_CAPTURE_THE_FLAG	= 4;
const int MT_LEADER_KILL		= 5;

const int ML_HIGH				= 1;
const int ML_LOW				= 2;


extern void Recv_CMD_TEAM_MATCH(char* pMsg, DWORD dwLen);
extern void Send_CMD_TEAM_MATCH_INSERT(TEAM_MATCH_PACKET TeamMatch, WORD wType);
extern void Cmd_CTWS_GuildWarPacket(TEAM_MATCH_PACKET TeamMatch, WORD wType);
extern void Cmd_WSTC_RivalGuildWar( char* pMsg, DWORD dwLen );
extern void CmdGuildWarInvite( char* pMsg, DWORD dwLen );
extern void CmdGuildWarBBS( char* pMsg, DWORD dwLen );


FLAG_TYPE GetFormConvert(int nForm);
FLAG_TYPE GetTypeConvert(int nType);
GUILD_WAR_TIME GetWarTime(int nTime);
GUILD_WAR_NUMBER GetWarNumber(int nNumber);
int GetFormConvert(FLAG_TYPE nType);
int GetWarTime(GUILD_WAR_TIME Time);
int GetWarNumber(GUILD_WAR_NUMBER Number);
extern void CmdGuildMatchBeginSetting( char* pMsg, DWORD dwLen );
extern void CmdGuildMatchStatus( char* pMsg, DWORD dwLen );
extern void CmdGuildMatchStatusRespon( char* pMsg, DWORD dwLen );
extern void CmdGuildMatchBegin( char* pMsg, DWORD dwLen );

#endif