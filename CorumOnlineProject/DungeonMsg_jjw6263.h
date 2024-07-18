#pragma once

#include "Struct.h"

void	InitPacketProcDungeonJJW6263();

void	CmdUpgradeCancel(char* pMsg, DWORD dwLen);
void	CmdMakingItemCancel(char* pMsg, DWORD dwLen);
void	CmdEnentDungeonChangeLayer( char* pMsg, DWORD dwLen);
void	CmdEventClose(char* pMsg, DWORD dwLen);
void	CmdEventDungeonClear(char* pMsg, DWORD dwLen);
void	CmdEventDungeonClearPresent(char* pMsg, DWORD dwLen);
void	CmdEventDungeonLastLayer(char* pMsg, DWORD dwLen);
void	CmdEventDungeonFailed(char* pMsg, DWORD dwLen);
void	CmdEventAliveMonsterCount(char* pMsg, DWORD dwLen);
void	CmdEventExceptionResult(char* pMsg, DWORD dwLen);
void	CmdEventDungeonEnter(char* pMsg, DWORD dwLen);
void	CmdEventDungeonOpenInfo(char* pMsg, DWORD dwLen);
void	CmdEventClearCondition(char* pMsg, DWORD dwLen);
void	CmdEventAliveUserCount(char* pMsg, DWORD dwLen);
void	CmdEventDungeonLayerClear(char* pMsg, DWORD dwLen);
void	CmdEventDungeonFirstTimeOut(char* pMsg, DWORD dwLen);
void	CmdEventDungeonTraninigDisPlay(char* pMsg, DWORD dwLen);
void	CmdEventDungeonSurvivalDisPlay(char* pMsg, DWORD dwLen);
void	CmdEventDungeonCloseDisPlay(char* pMsg, DWORD dwLen);
void	CmdEventDungeonJoinUserCount(char* pMsg, DWORD dwLen);
void	CmdEventDungeonClearUserSend(char* pMsg, DWORD dwLen);
void	CmdEventDungeonTraninigOneLayerClayer(char* pMsg, DWORD dwLen);
void	CmdUpgradeItemSuccess( char* pMsg, DWORD dwLen );
void	CmdUpgradeItemFail( char* pMsg, DWORD dwLen );
void	CmdUpgradeItemFailInsurance( char* pMsg, DWORD dwLen );
void	CmdMakingItemSuccess( char* pMsg, DWORD dwLen );
void	CmdMakingItemFail( char* pMsg, DWORD dwLen );
void	CmdItemMove( char* pMsg, DWORD dwLen );
void	CmdItemUsedNative( char* pMsg, DWORD dwLen );
void	CmdUpgradeItem_ReturnInv( char* pMsg, DWORD dwLen );
void	CmdMakingItem_ReturnInv( char* pMsg, DWORD dwLen );
void	CmdGuardianSupplies_ReturnInv( char* pMsg, DWORD dwLen );
void	CmdSwitchWeapon( char* pMsg, DWORD dwLen );

BOOL	IsUltra();


