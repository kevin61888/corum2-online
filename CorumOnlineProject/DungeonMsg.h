#pragma once

#include	"Struct.h"
#include	"DungeonMsg_yak.h"
#include	"DungeonMsg_jjw6263.h"
#include	"DungeonMsg_seven.h"
#include	"DungeonMsg_deepdark.h"
#include	"DungeonMsg_minbobo.h"

void		InitPacketProcDungeon();

void		CmdJoinDungeon( char* pMsg, DWORD dwLen );
void		CmdChangeLayer( char* pMsg, DWORD dwLen );
void		CmdChangeSection( char* pMsg, DWORD dwLen );
void		CmdLevelUP( char* pMsg, DWORD dwLen );
void		CmdAppearItem( char* pMsg, DWORD dwLen );
void		CmdDisappearItem( char* pMsg, DWORD dwLen );
void		CmdDungeonErrorCode( char* pMsg, DWORD dwLen );
void		CmdPickupItem( char* pMsg, DWORD dwLen );
void		CmdDungeonMessage( char* pMsg, DWORD dwLen );
void		CmdDungeonJoinFailedForDungeon( char* pMsg, DWORD dwLen );
void		CmdDungeonGuildWarDieMsg( char* pMsg, DWORD dwLen );