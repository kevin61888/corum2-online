#ifndef __MSG_PROC_FOR_SERVER_H__
#define __MSG_PROC_FOR_SERVER_H__

#pragma once

#include "ServerTable.h"


void CmdDungeonAliveInfo(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdCharacterSearch(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdEventDungeonUserOut(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdServerAttach(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdUserAcceptAllowed(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdUserAcceptFailed(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdDungeonMagicArrayItem(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdDungeonGuardianItem(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdDungeonOwnerChange(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdGotoWorld(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdFriendUserInfoRt(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdRefreshProduct_Acquisition(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdRefreshAccEntrance_Acquisition(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdRefreshAccExp_Acquisition(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdRefreshEntrance_Edit(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdCreateProduct(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdRefreshDefenseCount(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdRefreshOperationMode(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdDungeonJoinSuccess(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdCheckDungeonJoin(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdGmCommand_DSWS(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
void CmdEventDungeonUserEnter(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);


#endif	