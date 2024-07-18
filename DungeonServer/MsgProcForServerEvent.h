#pragma once


void CmdEventDungeonInit(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdEventDungeonOpenInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdEventDungeonCloseInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdEventDungeonFirstTimeOut(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdEventDungeonJoinUserCount(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdEventDungeonClose(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);