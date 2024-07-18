#ifndef __MSGPROC_FOR_SERVER_H__
#define __MSGPROC_FOR_SERVER_H__

#pragma once

#include "WorldServerTable.h"

void CmdServerAttach(				DWORD dwConnectionIndex,	char* pMsg, DWORD dwLength);
void AcmdServerAliveInfo(			WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdUserAcceptAllowed(			WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdUserAcceptFailed(			WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdUserWorldLoginSuccess(		WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdWorldUserDisconnect(		WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdRequestChinaBilling(		WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdRequestChinaBillingInfo(	WORLD_SERVER_DATA* pServer, char* pMsg, DWORD dwLength);

#endif // __MSGPROC_FOR_SERVER_H__
