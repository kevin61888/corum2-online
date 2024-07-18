#pragma once


void InitializePacketProc();


void CmdDungeonJoin(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdDungeonMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdDungeonStop( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdDungeonAttack_User_Mon( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdDungeonAttack_User_User( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdDungeonChat( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdSkillLevelUp( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPickupItem( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdItemUsed( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdStatusLevelUp( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdDungeonRequestInfo( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdCallGuardian( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuardianAttack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuardianCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdMonsterCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuardianCommandMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuardianCommandAttack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGuardianSkillSelect( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGMQuickMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPortalMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdRequestSectionInfoForPortal( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

void CmdFindHack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdTickCheck( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdGmCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

