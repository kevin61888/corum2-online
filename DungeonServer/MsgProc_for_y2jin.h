#pragma once

#include "stdafx.h"

void InitializePacketProcY2jin();
void CmdEmoticon( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdRequestMatchM2M( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
void CmdReplyMatchM2M( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );

#ifdef NEW_COMMAND
void CmdNewGMCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );
#endif



