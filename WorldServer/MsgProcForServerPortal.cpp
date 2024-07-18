#include "stdafx.h"
#include "MsgProcForServerPortal.h"
#include "MessengerHash.h"
#include "GuildHash.h"
#include "PartyHash.h"
#include "ChinaBillingProc.h"
#include "WorldUserTable.h"
#include "OwnServer.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonTable.h"
#include "recvmsg.h"
#include "AcceptTable.h"
#include "WorldUser.h"
#include "ServerFunction.h"


//=============================================
// 이벤트 던젼 포탈을 시켜줘도 되는가?
//=============================================
int ChkDungeonPortalCondition( DUNGEON_DATA_EX* pDungeon )
{
	switch ( pDungeon->m_bEventState )
	{
		case EVENT_DUNGEON_READY:			return 20;
		case EVENT_DUNGEON_OPEN_FULL:		return 19;
		case EVENT_DUNGEON_OPEN_TIME_UP:	return 18;
		case EVENT_DUNGEON_CLOSE:			return 20;
		case EVENT_DUNGEON_FIRST_CLOSE:		return 20;
	}

	return 0;
}

//=============================================
// 포탈할 던전서버에 이 유저의 접속 승인을 요청
//=============================================
void CmdRequestUserAcceptForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL *pPacket = (DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL*)pMsg;

	BYTE bErrorCode = 0;
	
	DUNGEON_DATA_EX *pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	DUNGEON_DATA_EX *pDungeonFrom = g_pDungeonTable->GetDungeonInfo(pPacket->wFromDungeonID);

	if(pDungeon)
	{		
		// 공성전 중에는 이동 못함
		if(pDungeon->m_bSiege)	 
		{
			if( pDungeonFrom && pDungeonFrom->IsStadium() )
			{
				bErrorCode = 254;
			}
			else
			{
				bErrorCode = 8;				
			}
			goto lb_Fail;
		}
		else if(	IS_ABLE_SERVICE_TYPE(ST_DEVELOP) 
				||	IS_ABLE_NATION(NC_JAPAN) )//hwoarang
		{
			if(pDungeon->m_byEntererMinLevel > pPacket->dwUserLevel || pDungeon->m_byEntererMaxLevel < pPacket->dwUserLevel)
			{
				// 레벨제한 걸렷나?
				bErrorCode = 9;		
				goto lb_Fail;
			}
		}
		else if (pDungeon->IsConquer() && pDungeon->m_dwOwnerIndex != pPacket->dwUserIndex )
		{
			// 입장료 깍아라.
			if (pPacket->dwUserMoney >= pDungeon->GetEntrancePay())
			{
				pPacket->dwUserMoney -= pDungeon->GetEntrancePay();				
			}
			else
			{
				bErrorCode =  17;
				goto lb_Fail;
			}
		}

		// 이벤트 던젼이면
		if(pDungeon->GetDungeonType() == DUNGEON_TYPE_EVENT)
		{
			bErrorCode = (BYTE)ChkDungeonPortalCondition( pDungeon );			
		}

		if(pDungeon->m_pServer && pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
		{			
			CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);
			
			if(pUser)
				pUser->m_byPortal = 1;
			
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);

			return;
		}
		else
		{
			//던전 서버가 접속되어 있지 않음 
			bErrorCode = 1;		
			goto lb_Fail;
		}		
	}
	else
	{
		// ID에 해당하는 던전정보 없음 
		bErrorCode = 2;		
	}

lb_Fail:
	
	DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL FailPacket;	
	FailPacket.dwUserIndex = pPacket->dwUserIndex;
	FailPacket.bErrorCode = bErrorCode;
	g_pNet->SendToServer(pServer->dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


//=========================================================
// 포탈을 승인한  던전서버에 접속할 Address와 Port를 보내줌
//=========================================================
void CmdUserAcceptAllowedForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL* pPacket = (DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wFromDungeonID);

	if(pDungeon)
	{
		if(pDungeon->m_pServer && pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)
		{
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);
		}
		else
		{
			Log(LOG_IMPORTANT, "Dungeon Server is not connected at CmdUserAcceptAllowedForPortal() (DungeonID:%d)", pPacket->wFromDungeonID);
		}
	}
	else
	{
		Log(LOG_IMPORTANT, "Dungeon data is not found! at CmdUserAcceptAllowedForPortal() (DungeonID:%d)", pPacket->wFromDungeonID);
	}
}


//=================================================
// 포탈할 던전서버에서 접속 허용 명단 추가 실패  
//=================================================
void CmdUserAcceptFailedForPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL* pPacket = (DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL*)pMsg;

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wFromDungeonID);

	if(pDungeon)
	{
		// 해당 던전서버로 메세지 보내줌 
		if(pDungeon->m_pServer && pDungeon->m_pServer->dwServerStatus == SERVER_STATUS_CONNECTED)	
		{
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);
		}
		else
		{
			Log(LOG_IMPORTANT, "Dungeon Server is not connected at CmdUserAcceptFailedForPortal() (DungeonID:%d)", pPacket->wFromDungeonID);
		}
	}
	else
	{
		Log(LOG_IMPORTANT, "Dungeon data is not found! at CmdUserAcceptFailedForPortal() (DungeonID:%d)", pPacket->wFromDungeonID);
	}
} 
