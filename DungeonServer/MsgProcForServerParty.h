#pragma once


void CmdPartyLeader(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdPartyMsg(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdPartyInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdPartyBoard(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdFriendInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdPartyConfig(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);