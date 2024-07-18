//==============================================================//
// Code by Jang.									2003.08.01	//
// Object : 길드관련과 길드전 작업.								//
//==============================================================//
#ifndef		__GUILD_INC__
#define		__GUILD_INC__

#include	"stdafx.h"

//==============================================================//
// Function.													//
//==============================================================//
void CmdGuildItem( char* pMsg, DWORD dwLen ); 
void CmdGuildCreate( char* pMsg, DWORD dwLen ); 
void CmdDgGuildUserInfo( char* pMsg, DWORD dwLen ); 
void CmdDgGuildUser( char* pMsg, DWORD dwLen ); 
void CmdGuildJoinMessage( char* pMsg, DWORD dwLen ); 
void CmdGuildJoinResult( char* pMsg, DWORD dwLen ); 
void CmdGuildJoinInfo( char* pMsg, DWORD dwLen ); 
void CmdGuildRankRt( char* pMsg, DWORD dwLen ); 
void CmdUserDelete( char* pMsg, DWORD dwLen ); 
void CmdGuildDelete( char* pMsg, DWORD dwLen ); 
void CmdGuildUser(char* pMsg, DWORD dwLen);
void CmdGuildUserInfo(char* pMsg, DWORD dwLen);
void CmdGuildUserInfoDel(char* pMsg, DWORD dwLen);
void CmdGuildInfo(char* pMsg, DWORD dwLen);
void CmdGuildFail(char* pMsg, DWORD dwLen);
void CmdGuildoffLine(char* pMsg, DWORD dwLen);
void CmdGuildoffLineDelete(char* pMsg, DWORD dwLen);
void CmdGuildUserInfoRt(char* pMsg, DWORD dwLen);
void CmdGuildRequest(char* pMsg, DWORD dwLen);
void CmdGuildUserRequestFail(char* pMsg, DWORD dwLen);
void CmdGuildDataRequest(char* pMsg, DWORD dwLen);
void CmdGuildCount(char* pMsg, DWORD dwLen);
void CmdGuildInfoRefresh(char* pMsg, DWORD dwLen);
void CmdGuildExp(char* pMsg, DWORD dwLen);
void CmdGuildLevel(char* pMsg, DWORD dwLen);
void CmdGuildRankFail(char* pMsg, DWORD dwLen);
void CmdGuildUserLevel(char* pMsg, DWORD dwLen);
void CmdGuildUserMaster(char* pMsg, DWORD dwLen);
void CmdGuildUserGuildMaster(char* pMsg, DWORD dwLen);
void CmdGuildList(char* pMsg, DWORD dwLen);
void CmdGuildError(char* pMsg, DWORD dwLen);
void CmdGuildPortal(char* pMsg, DWORD dwLen);
void CmdGuildPotalUsed(char* pMsg, DWORD dwLen);
void CmdGuildPotalFail(char* pMsg, DWORD dwLen);
void CmdServerGuildPortalFail( char* pMsg, DWORD dwLen );
void CmdGuildWarMaster( char* pMsg, DWORD dwLen );
void CmdGuildWarError( char* pMsg, DWORD dwLen );
void CmdGuildWarJoin( char* pMsg, DWORD dwLen );
void CmdGuildWarPoint( char* pMsg, DWORD dwLen );
void CmdGuildWarExit( char* pMsg, DWORD dwLen );
void CmdGuildWarRank( char* pMsg, DWORD dwLen );
void CmdGuildUserRank( char* pMsg, DWORD dwLen );
void CmdGuildWarMessage( char* pMsg, DWORD dwLen );
void CmdGuildWarMessageGuild( char* pMsg, DWORD dwLen );
void CmdGuildFlag( char* pMsg, DWORD dwLen );
void CmdGuildUserFlag( char* pMsg, DWORD dwLen );

BOOL IsGuildCreate();

#endif
//==============================================================//
// End.															//
//==============================================================//

