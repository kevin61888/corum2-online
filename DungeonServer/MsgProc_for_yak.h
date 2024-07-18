#pragma once

#include "stdafx.h"

void InitializePacketProcYak();
void CmdDungeonSkill_ChangeLR( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdPKMode( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdDungeonSkillCasting( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdFinishMapLoading( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdSkill( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdUserDieOK( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );		
void CmdAccEntrance_Acquisition( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdProduct_Acquisition( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdAccExp_Acquisition( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdEntrance_Edit( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );