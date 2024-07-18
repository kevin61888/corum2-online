/*#####################################################################//
//##################### 2004. 11. 클래스화 예정.. #####################
//#####################################################################
//
//	Version		: Dev/Develop
//	ProjectName	: WorldServer
//	FileName	: RivalGuildWar.h
//	Date		: 2004. 10. 15. ( minjin )
//	Description	: Declare Rival Guild War Global Function
//
//#####################################################################*/

#ifndef __RIVAL_GUILD_WAR__
#define __RIVAL_GUILD_WAR__

#include "WorldUser.h"
#include "ServerTable.h"
#include "StadiumMgr.h"
#include "DebugLog.h"

extern void Recv_CMD_STADIUM_STATE(SERVER_DATA* pServer, char* pMsg, DWORD dwLength);
extern void Recv_CTWS_TM_REMOVE(CWorldUser* pUser);
extern bool CheckHaveTeamMatch(CWorldUser* pUser);

extern CStadiumMgr< CStadium >		g_StadiumMgr;

extern CDebugLog g_GuildWarLog;

#endif	//	__RIVAL_GUILD_WAR__