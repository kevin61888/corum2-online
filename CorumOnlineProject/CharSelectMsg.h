#pragma once

#include "stdafx.h"

void CmdCreateCharacterFail( char* pMsg, DWORD dwLen );
void CmdCreateCharacterSuccess( char* pMsg, DWORD dwLen );
void CmdWorldUserInfo( char* pMsg, DWORD dwLen );
void CmdCharacterSelectFail( char* pMsg, DWORD dwLen );
void CmdWorldLoginFail( char* pMsg, DWORD dwLen );
void AcmdConnectWorldServer( char* pMsg, DWORD dwLen );
void CmdDeleteCharacterResult( char* pMsg, DWORD dwLen );
void AcmdItemmallInfo( char* pMsg, DWORD dwLen );
void AcmdWarningTitle( char* pMsg, DWORD dwLen );
void CmdChrMoveChk( char* pMsg, DWORD dwLen );
void CmdMoveCharacterSuccess( char* pMsg, DWORD dwLen );
void CmdMoveCharacterFail( char* pMsg, DWORD dwLen );



