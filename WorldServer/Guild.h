#ifndef __GUILD_H__
#define __GUILD_H__

#pragma once

#include "WorldUser.h"
#include "GuildHash.h"

extern char g_szGuildBattleText[MAX_GUILD_TEXT][0xff];

#define	MIN_GUILD_LEVEL				3

#if defined(JAPAN_LOCALIZING)
// Remark
#define	MIN_GUILD_MEMBER			1
#else
#define	MIN_GUILD_MEMBER			1
#endif

enum GUILD_WAR_ERRCODE
{
	GWE_VS_NOT_EXIST_MASTER		= 0,	// "상대 길드마스터가 게임에 접속해 있지 않습니다."
	GWE_VS_OUTOFBOUNDS_MAX_WAR	= 1,	// "상대 길드가 최대 8개의 다른 길드와 길드전 중입니다."
	GWE_VS_MUST_LEVEL3			= 2,	// "상대 길드 레벨 3이상이어야 길드전이 가능합니다."
	GWE_VS_MORE_GUILD_MEMBER	= 3,	// "상대 길드마스터를 제외한 길드원이 최소 1명 이상 존재해야 합니다."
	GWE_VS_NOT_ACCEPT_WAR		= 4,	// "%s 길드 마스터가 길드전 선포 신청을 거절하였습니다."
	GWE_NOT_GUILD_MASTER		= 5,	// "길드 마스터가 아닙니다."
	GWE_OUTOFBOUNDS_MAX_WAR		= 6,	// "길드가 최대 8개의 다른 길드와 길드전 중입니다."
	GWE_MUST_LEVEL3				= 7,	// "길드 레벨 3이상이어야 길드전이 가능합니다."
	GWE_MORE_GUILD_MEMBER		= 8,	// "길드마스터를 제외한 길드원이 최소 1명 이상 존재해야 합니다."
	GWE_NOW_ON_GUILD_WAR		= 9,	// "현재 길드전이 진행되고 있습니다."
	GWE_SAME_GUILD				= 10,	// "같은 길드원끼리 길드전을 신청할 수 없습니다."
	GWE_CANNOT_REMOVE_GUILD		= 11,	// "길드전 중 길드삭제가 불가능 합니다."
	GWE_NOW_WAIT_ACCEPT			= 12,	// "현재 길드전 협상중입니다. 잠시 후에 다시 시도해주세요."
	GWE_BEFORE_END_TIME			= 13,	// "길드전 승락시 3일(72시간)이후에 종료가 가능합니다."
	GWE_GUILD_DATA_ERROR		= 100,	// 길드전 데이터 없을때.. -_-;;;
	GWE_GUILD_WAR_SUCCESS		= 255
};

void CmdGuildUserDel(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildRank(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildJoin(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdGuildCreate(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdUserGuildMaster(CWorldUser* pUser, char* pMsg, DWORD dwcLength);
void CmdGuildDelete(CWorldUser* pUser, char* pMsg, DWORD dwcLength);
void CmdGuildJoinMessage(CWorldUser* pUser ,char* pMsg, DWORD dwLength);
void CmdGuildRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildUserInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildDataRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildUserRequestFail(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarErrorRt(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarMasterRt(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarCreate(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarExit(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdGuildWarRank(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void SendToUserGuildWarFail(CWorldUser* pUser, BYTE byErrorCode);
int	 GuildWarCheckCondition( DWORD dwGuildID, DWORD dwDstGuildID );

BOOL InitGuildInformations();

BOOL IsGuildWar(DWORD dwOffenceId, DWORD dwDefenseID);

void GuildWarTime(LP_GUILDWAR_TIME lpGuildWarTime, DWORD dwGuildId, DWORD dwGuildIdDst);

void GuildPoint(DWORD dwGuildId);

extern CWorldUser* IsThereGuildMaster(ListNode<CWorldUser>* pGuildUserPos, const bool bIsOnlyGuildMaster = true);//: 050202 hwoarang 길마 외에 2등급까지 받는다

#endif	// __GUILD_H__