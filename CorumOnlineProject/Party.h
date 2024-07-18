//==============================================================//
// Code by Jang.									2003.08.01	//
// Object : 파티 관련 작업.										//
//==============================================================//
#ifndef		__PARTY_INC__
#define		__PARTY_INC__

#include	"InitGame.h"

//==============================================================//
// Function.													//
//==============================================================//
void CmdParty( char* pMsg, DWORD dwLen ); 
void CmdPartyJoin( char* pMsg, DWORD dwLen ); 
void CmdPartyJoinId( char* pMsg, DWORD dwLen ); 
void CmdPartyLogout( char* pMsg, DWORD dwLen ); 
void CmdPartyLevel( char* pMsg, DWORD dwLen );
void CmdPartyInfo( char* pMsg, DWORD dwLen );
void CmdPartyLeader( char* pMsg, DWORD dwLen );
void CmdPartyHP( char* pMsg, DWORD dwLen );
void CmdPartyPotal( char* pMsg, DWORD dwLen );
void CmdPartyPotalUsed( char* pMsg, DWORD dwLen );
void CmdPartyPotalFail(char* pMsg, DWORD dwLen);
void CmdPartyPotalMove(char* pMsg, DWORD dwLen);
void CmdPartyPotalWorldMove(char* pMsg, DWORD dwLen);
void CmdPartyArray(char* pMsg, DWORD dwLen);
void CmdServerPartyPortalFail( char* pMsg, DWORD dwLen );
void CmdPartyUserRequest(char* pMsg, DWORD dwLen);
void CmdPartyUserRequestFail(char* pMsg, DWORD dwLen);
void CmdPartyInfoRequest(char* pMsg, DWORD dwLen);
void CmdPartyBoard(char* pMsg, DWORD dwLen);
void CmdPartySet(char* pMsg, DWORD dwLen);
void CmdPartyEntry( char* pMsg, DWORD dwLen );
// 파티 매칭 : 최덕석 2005.1.24
void CmdPMatchRegisterResult(char* pMsg, DWORD dwLen);
void CmdPMatchEscapeResult(char* pMsg, DWORD dwLen);
void CmdPMatchRequest(char* pMsg, DWORD dwLen);
void CmdPMatchChangeResult(char* pMsg, DWORD dwLen);
void CmdPartyConfig(char *pMsg, DWORD dwLen);
void CmdPartyFriend(char *pMsg, DWORD dwLen);

#endif
//==============================================================//
// End.															//
//==============================================================//

