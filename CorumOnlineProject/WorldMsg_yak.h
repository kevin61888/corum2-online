#pragma once

#include "stdafx.h"

void	CmdReplyDungeonInfo( char* pMsg, DWORD dwLen );
void	CmdRefreshEventDungeon_StartSiege(char* pMsg, DWORD dwLen);
void	CmdRefreshEventDungeon_EndSiege(char* pMsg, DWORD dwLen);
void	CmdRefreshEventDungeon_Change_Owner_Guild( char* pMsg, DWORD dwLen );
void	CmdRefreshEventDungeon_AccEntrance_Acquisition( char* pMsg, DWORD dwLen );
void	CmdRefreshEventDungeon_Entrance_Edit( char* pMsg, DWORD dwLen );
void	CmdRefreshEventDungeon_AccExp_Acquisition( char* pMsg, DWORD dwLen );
void	CmdRefreshEventDungeon_GuardianItem(char* pMsg, DWORD dwLen);
void	CmdRefreshEventDungeon_MagicFieldArrayItem(char* pMsg, DWORD dwLen);
void	CmdRefreshEventDungeon_Change_Owner(char* pMsg, DWORD dwLen);
void	CmdRefreshEventDungeon_LevelUp(char* pMsg, DWORD dwLen);
void	CmdRefreshEventDungeon_CreateProduction(char* pMsg, DWORD dwLen);
void	CmdRefreshDefenseCount(char* pMsg, DWORD dwLen);
void	CmdRefreshEventDungeon_OperationMode(char* pMsg, DWORD dwLen);