//==============================================================//
// Code by Jang.									2003.11.10	//
// Object : 친구기능 관련 작업.									//
//==============================================================//
#ifndef		__MESSENGER_INC__
#define		__MESSENGER_INC__

#include	"InitGame.h"

//==============================================================//
// Function.													//
//==============================================================//
void CmdMessengerUser( char* pMsg, DWORD dwLen ); 
void CmdMessengerUserInfo( char* pMsg, DWORD dwLen ); 
void CmdMessengerUserFail( char* pMsg, DWORD dwLen );
void CmdMessengerUserAdd( char* pMsg, DWORD dwLen );
void CmdMessengerFail( char* pMsg, DWORD dwLen );
void CmdMessengerSuccess( char* pMsg, DWORD dwLen );
void CmdFriendInfo( char* pMsg, DWORD dwLen );
void CmdFriendDelete( char* pMsg, DWORD dwLen );
void CmdFriendStaus( char* pMsg, DWORD dwLen );

#endif
//==============================================================//
// End.															//
//==============================================================//
