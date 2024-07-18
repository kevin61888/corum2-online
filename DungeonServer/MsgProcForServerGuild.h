#pragma once


void CmdGuildMsg(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdServerGuildFail(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdServerGuildInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdGuildUserInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdGuildList(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdGuildWarDelete(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdGuildWarRank(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdGuildWarUserRank(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdGuildWarMessage(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdRefreshDungeonOwnerGuild(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);