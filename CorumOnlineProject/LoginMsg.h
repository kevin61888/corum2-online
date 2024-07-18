#pragma once

void CmdLoginSuccess( char* pMsg, DWORD dwLen );
void CmdLoginFail( char* pMsg, DWORD dwLen );
void CmdInvalidGM( char* pMsg, DWORD dwLen );
void CmdLoginFailWithReason( char* pMsg, DWORD dwLen );

void CmdEncryptionKey(char* pMsg, DWORD dwLen); // added by deepdark.