#pragma once

#include "ServerTable.h"


void CmdPartyPotalFail(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdPartyPortalUsed(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdPartyAllPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdPartyPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdPartyHp(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdPartyLevel(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdDungeonGroupExp(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdPartyInfo(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdPartyPortalMove(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
