#pragma once

#include "StdAfx.h"

void InitializePacketProcJJW6263();
BOOL IsUltra();


void CmdEventUserPrevDungeonID(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdEventClearEffectFinish(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdItemUpgrade( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdItemUpgradeFail( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdItemMaking( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdItemMakingCancel( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdItemMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdItemUsedNative( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdSwitchWeapon( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

