#pragma once


#include "ServerTable.h"


void ACmdGuildCharacterDelete(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void ACmdRequestServerAlive(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void ACmdCMToolNotice(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdKickUser(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdAddNewUser(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void AcmdBillingSmallTime(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void AcmdChangeBillingType(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void AcmdServerAttachResult(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);