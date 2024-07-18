#pragma once


void CmdServerTerminate(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdAddUserAcceptAllowed(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdRemoveUserAcceptAllowed(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdStartSiege(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdEndSiege(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdDungeonInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdDungeonLevelUp(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdRefreshAllDungeonServerData(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdKickUser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdNoticeAction(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdDungeonMessage(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdRequestDungeonAlive(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdGuildInfoMsg(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdCharacterSearch(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdGmCommand_WSDS(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdSetDoubleExp(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdSetMagicItemDropRate(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdSetExpGain(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdServerAttachResult(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdCheckDungeonJoinRt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdCBLevelChk(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
void CmdSetPartyBonusExpGain(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength);
