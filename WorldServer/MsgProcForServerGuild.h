#pragma once


#include "ServerTable.h"


void CmdGuildPotalFail(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdGuildPortalUsed(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdGuildAllPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdGuildLevel(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdGuildWarPoint(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdGuildUserInfoRt(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);