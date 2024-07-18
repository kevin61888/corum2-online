#ifndef __MSG_PROC_H__
#define __MSG_PROC_H__

#pragma once

#include "WorldUser.h"

#include "SchoolGuildDungeon.h"
extern CSchoolGuildDungeonHash*	g_pSchoolGuildDungeonTableHash;

// 이벤트 던젼들의 정보를 모두 보냄.
void SendAllEventInfo(DWORD dwConnectionIndex);

// 이벤트 던젼들의 상태만을 보냄.
void SendAllEventState(DWORD dwConnectionIndex);

void CmdWorldLogin(DWORD dwConnectionIndex ,char* pMsg,DWORD dwLength);
void CmdQueryEventInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdRequestDungeonInfo(CWorldUser* pUser ,char* pMsg,DWORD dwLength);
void CmdTryToJoinDungeon(CWorldUser* pUser ,char* pMsg,DWORD dwLength);
void CmdCharacterInfoSearch(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdHelpInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdCommandMsg(CWorldUser* pUser ,char* pMsg,DWORD dwLength);
void CmdSkillKeyRt(CWorldUser* pUser ,char* pMsg,DWORD dwLength);

void CmdCheckGM(CWorldUser* pUser ,char* pMsg,DWORD dwLength);
void CmdCMKickUser(CWorldUser* pUser ,char* pMsg,DWORD dwLength);
void CmdCheckMonitoring(CWorldUser* pUser,char* pMsg,DWORD dwLength);

void CmdCommand(CWorldUser* pUser ,char* pMsg, DWORD dwLength); // 작업 번호.
void DGCmdCommand(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdMessageRQ(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdChatUser(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdRequestOwnDungeonInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdDSChatUser(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdNotice(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdRefreshRecentDungeon(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdGoToWorldSuccess(CWorldUser* pUser ,char* pMsg, DWORD dwLength); //월드맵 이동에 성공함
void CmdWorldMove(CWorldUser* pUser ,char* pMsg, DWORD dwLength); //월드맵 이동 Packet
void CmdWorldStop(CWorldUser* pUser ,char* pMsg, DWORD dwLength); //월드맵 Stop Packet
void CmdWorldSetPosition(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdAirshipRiding(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdAirshipGetoff(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdLoadingStatus(CWorldUser* pUser ,char* pMsg, DWORD dwLength);

// 파티 매칭 : 최덕석 2005.1.24
void CmdPMatchRegister(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdPMatchEscape(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdPMatchRequestResult(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdPMatchChange(CWorldUser* pUser, char* pMsg, DWORD dwLength);

///////////////////////////////////////////////////////////
// 전병렬 ->
void CmdGuildMatchBeginSetting(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildMatchStatusRequest(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildMatchCancelObserver(CWorldUser* pUser, char* pMsg, DWORD dwLength);
// <- 전병렬
////////////////////////////////////////////////////////////

extern void Recv_CMD_TEAM_MATCH(CWorldUser* pCaster, char* pMsg, DWORD dwLength);// 팀전 : hwoarang 050129
//--------------------------------------------------------------------------------------
// GM COMMAND
//--------------------------------------------------------------------------------------
void CmdGmCommand( CWorldUser* pUser, char* pMsg, DWORD dwLength);

#endif // __MSG_PROC_H__