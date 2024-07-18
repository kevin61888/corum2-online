#include "stdafx.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DungeonTable.h"
#include "Dungeon.h"
#include "OwnServer.h"
#include "DungeonLayer.h"
#include "DUNGEON_DATA_EX.h"
#include "ServerFunction.h"
#include "AcceptTable.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "recvmsg.h"
#include "DBProc.h"
#include "RecordSet.h"
#include "GameSystemNative.h"


// 유저가 월드상에서 일반 던젼 조인일떄 월드서버에서 던젼에게 이 유저의 접속을 허용하라는 패킷이다.
void CmdAddUserAcceptAllowed(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_ADD_USER_ACCEPT_ALLOWED *pPacket = (WSTDS_ADD_USER_ACCEPT_ALLOWED*)pMsg;

	ACCEPT_USER* pUser = (ACCEPT_USER*)0xffffffff;

	while(pUser)
	{
		pUser = g_pAcceptTable->GetAcceptInfo(pPacket->dwUserIndex);
		if(pUser)
		{
			g_pAcceptTable->Remove(pUser);	
		}
		else 
		{
			goto lb_Continue;
		}	
	}

lb_Continue:
	
	// 접속 허용 명단에 추가 
	pUser = g_pAcceptTable->AllocNewAccept(pPacket->wDungeonID, pPacket->dwUserIndex);

	if(!g_pAcceptTable->Add(pUser))
	{
		Log(LOG_IMPORTANT, "Adding User to AcceptTable is Failed!");
		
		DSTWS_USER_ACCEPT_FAILED packet;	
		packet.wDungeonID	= pPacket->wDungeonID;
		packet.bServerType	= g_pThis->GetServerType();
		packet.bType		= pPacket->bType;
		packet.dwUserIndex	= pPacket->dwUserIndex;
		g_pNet->SendToServer(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	pUser->bConnectToGameRoom = (pPacket->bType == ACCEPT_ALLOWED_TYPE_GAMEROOM);
	pUser->bUserType		  = pPacket->bUserType;
	
	// 다시 월드서버에게 유저허용이 승인되었다고 알림 
	DSTWS_USER_ACCEPT_ALLOWED packet;	
	packet.bServerType	= g_pThis->GetServerType();
	packet.wDungeonID	= pPacket->wDungeonID;
	packet.bType		= pPacket->bType;
	packet.dwUserIndex	= pPacket->dwUserIndex;
	g_pNet->SendToServer(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


// 월드서버에서 던전서버에게 승인 받는 사이에 유저가 접속을 끊은경우 Aceptt 허용 테이블에 있는 그 유저를 삭제
void CmdRemoveUserAcceptAllowed(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_REMOVE_USER_ACCEPT_ALLOWED* pPacket = (WSTDS_REMOVE_USER_ACCEPT_ALLOWED*)pMsg;

	ACCEPT_USER* pUser = (ACCEPT_USER*)0xffffffff;

	while(pUser)
	{
		pUser = g_pAcceptTable->GetAcceptInfo(pPacket->dwUserIndex);

		if(pUser)
		{
			g_pAcceptTable->Remove(pUser);
			Log(LOG_IMPORTANT, "Accepted UserIndex is removed at CmdRemoveUserAcceptAllowed Function!");
		}
		else
		{
			return;
		}
	}	
}


void CmdCheckDungeonJoinRt(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_CHECK_DUNGEONJOIN_RT* pServerPacket = (WSTDS_CHECK_DUNGEONJOIN_RT*)pMsg;
	
	CUser* pUserChk = g_pUserChkHash->GetData(pServerPacket->dwUserIndex);

	if(pUserChk)
	{
		if(pServerPacket->byChk)
		{
			// 안짤리고 남아있는 유저가 있는경우 정상 종료및 Update 시킨후 Join 쿼리에 들어간다. 
			CUser* pUser = g_pUserHash->GetData( pServerPacket->dwUserIndex );
			
			if(pUser)
			{
				pUser->CallBackCompulsiveDisconnectUser();
			}
			else
			{
				// 최종 조인 처리는 데이타베이스의 응답을 받고 처리한다.
				char szSQL[2048]={0,};
				wsprintf(szSQL, "CZP_DUNGEON_USER_JOIN '%s'", pServerPacket->szCharacerName);	
				g_pDb->THOpenRecord(szSQL, DB_UUID_DUNGEON_JOIN, (void*)pUserChk->m_dwConnectionIndex, 1, GAME_DB );
			}			
		}
		else
		{
			Log(LOG_IMPORTANT, "Abnormal User!!!! (Name:%s)", pUserChk->m_szName );
			DWORD dwConnectionIndex = pUserChk->m_dwConnectionIndex;
			OnDisconnectUser(dwConnectionIndex);
			g_pNet->CompulsiveDisconnectUser( dwConnectionIndex );
			g_pNet->SetUserInfo(dwConnectionIndex, NULL);
		}
		
		g_pUserChkHash->Delete(pUserChk, pServerPacket->dwUserIndex);
	}		
}


void CmdKickUser(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_KICK_USER *pPacket = (WSTDS_KICK_USER*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwCharIndex);

	if(pUser)
	{
		Log(LOG_JUST_DISPLAY, "User Kicked! (Name:%s)", pUser->m_szName );

		DWORD dwConnectionIndex = pUser->m_dwConnectionIndex;

		OnDisconnectUser(dwConnectionIndex);
		g_pNet->CompulsiveDisconnectUser( dwConnectionIndex );
		g_pNet->SetUserInfo(dwConnectionIndex, NULL);

		ListNode<CUser>* pDeleteUserPos = g_pUserHash->GetHead();
		
		while(pDeleteUserPos)
		{
			CUser* pDeleteUser = pDeleteUserPos->pData;

			if(!pDeleteUser)
			{
				pDeleteUserPos = pDeleteUserPos->pNext;
				continue;
			}
            /*
			if(!pUser)
			{
				pDeleteUserPos = pDeleteUserPos->pNext;
				continue;
			}
			*/

			if(pDeleteUser->GetID() == pUser->GetID())
			{
				Log(LOG_NORMAL, "CmdKickUser OnDisconnectUser Fail(Name : %s, UserIndex : %u, ConnectionIndex = %u)"
					, pDeleteUser->m_szName, pDeleteUser->GetID(), pDeleteUser->m_dwConnectionIndex);

				g_pUserHash->Delete(pDeleteUser, pDeleteUser->GetID());

				delete pDeleteUser;
				pDeleteUser = NULL;
			}
			
			pDeleteUserPos = pDeleteUserPos->pNext;
		}
	}
}


void CmdRequestDungeonAlive(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSTWS_DUNGEON_ALIVE_INFO SendPacket;
	SendPacket.dwServerID			= g_pThis->GetServerID();
	SendPacket.dwConnectedUserCount	= g_pNet->GetConnectedUserNum();	

	g_pNet->SendToServer(WSINDEX, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void CmdDungeonMessage(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_DUNGEON_MESSAGE* pRecievePacket = (WSTDS_DUNGEON_MESSAGE*)pMsg;

	DSTC_DUNGEON_MESSAGE SendPacket;

	__lstrcpyn(SendPacket.szMessage, pRecievePacket->szMessage, sizeof(SendPacket.szMessage));
	SendPacket.wMsgLen			= pRecievePacket->wMsgLen;
	SendPacket.byType			= pRecievePacket->byType;
	SendPacket.dwMessageColor	= pRecievePacket->dwMessageColor;

	if (NULL != g_pDungeonTable)
	{
		g_pDungeonTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());
	}
}


void CmdNoticeAction(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_NOTICE_ACTION* pRecievePacket = (WSTDS_NOTICE_ACTION*)pMsg;

	DSTC_DUNGEON_NOTICE	NoticePacket;

	memset(NoticePacket.szMsg, 0, 1024);
	__lstrcpyn(NoticePacket.szMsg, pRecievePacket->szMessage, 1024);	
	NoticePacket.wMsgLen = pRecievePacket->wMsgLen;
	
	if (NULL != g_pDungeonTable)
	{
		g_pDungeonTable->BroadCast((char*)&NoticePacket, NoticePacket.GetPacketSize());
	}

	return;
}


void CmdServerTerminate(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	Log(LOG_NORMAL, "Received Termination Message From World Server! Server Terminate...");
	SetEvent( g_pThis->m_hKeyEvent[4] );
}


void CmdRefreshAllDungeonServerData(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	Log(LOG_NORMAL, "Start reload dungeonserver data!");

	if(ReLoadDungeonServerData() == FALSE)
	{
		Log(LOG_NORMAL, "Fail to reload dungeonserver data!");
		return;
	}

	Log(LOG_NORMAL, "End reload dungeonserver data!");
}


void CmdStartSiege(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTS_START_SIEGE* pPacket = (WSTS_START_SIEGE*)pMsg;
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	
	if(pDungeon)
	{
		pDungeon->StartSiege();
	}
}


void CmdEndSiege(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTS_END_SIEGE* pPacket = (WSTS_END_SIEGE*)pMsg;
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if (pDungeon)
	{
		pDungeon->EndSiege();	
	}
}


void CmdDungeonLevelUp(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTS_DUNGEON_LEVELUP* pPacket = (WSTS_DUNGEON_LEVELUP*)pMsg;
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if (pDungeon)
	{
		pDungeon->GetDungeonDataEx()->LevelUpgrade();
	}
}


void CmdDungeonInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTS_DUNGEON_INFO* pPacket = (WSTS_DUNGEON_INFO*)pMsg;
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	
	if (NULL == pDungeon)
	{
		Log(LOG_NORMAL, "pDungeon is NULL at CmdDungeonInfo function.");
		return;
	}

	DUNGEON_DATA_EX* pDungeonDataEx = pDungeon->GetDungeonDataEx();
	pDungeonDataEx->m_wUpgradeLevel	 = pPacket->wUpgradeLevel;
	
	if (pPacket->bSiege)
	{
		pDungeon->StartSiege();	
	}
}


void CmdServerAttachResult( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	WSTS_SERVER_ATTACH_RESULT* pPacket = (WSTS_SERVER_ATTACH_RESULT*)pMsg;
	
	g_pThis->m_dwGLOBAL_EXP_GAIN				= pPacket->dwEXPGAIN;
	g_pThis->m_GLOBAL_MAGIC_FIND_PBT			= pPacket->dwMagicDropRate;
	g_pThis->m_bAdultMode						= pPacket->bAdultMode;
	g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN	= pPacket->dwPartyBonusEXPGAIN;

	Log(LOG_JUST_DISPLAY, "Exp gain : %d", pPacket->dwEXPGAIN);
	Log(LOG_JUST_DISPLAY, "Magic Item Drop Rate : %d", pPacket->dwMagicDropRate);
	Log(LOG_JUST_DISPLAY, "Adult Mode : %d", pPacket->bAdultMode);	
	Log(LOG_JUST_DISPLAY, "Party Bonus Exp Gain : %d", pPacket->dwPartyBonusEXPGAIN);
}


void CmdSetDoubleExp(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_SET_DOUBLE_EXP *pPacket = (WSTDS_SET_DOUBLE_EXP*)pMsg;

	if(pPacket->bEnable)
	{
		g_pThis->m_GLOBAL_EXP_GAIN_DOUBLE = TRUE;
		Log(LOG_NORMAL, "Double Exp Enabled!");
	}
	else
	{
		g_pThis->m_GLOBAL_EXP_GAIN_DOUBLE = FALSE;
		Log(LOG_NORMAL, "Double Exp Disabled!");
	}
}


void CmdSetMagicItemDropRate(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_SET_MAGICITEM_DROPRATE* pPacket = (WSTDS_SET_MAGICITEM_DROPRATE*)pMsg;
	g_pThis->m_GLOBAL_MAGIC_FIND_PBT = pPacket->wDropRate;
	
	Log(LOG_NORMAL, "Magic Item Drop Rate : %d", pPacket->wDropRate);
}


void CmdSetExpGain(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_SET_EXPGAIN* pPacket = (WSTDS_SET_EXPGAIN*)pMsg;
	g_pThis->m_dwGLOBAL_EXP_GAIN = pPacket->wEXP;

	Log(LOG_NORMAL, "Exp Gain : %d", pPacket->wEXP);
}


void CmdSetPartyBonusExpGain(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_SET_PARTY_BONUS_EXPGAIN* pPacket = (WSTDS_SET_PARTY_BONUS_EXPGAIN*)pMsg;
	g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN = pPacket->wEXP;

	Log(LOG_NORMAL, "Party Bonus Exp Gain : %d", pPacket->wEXP);
}


void CmdCBLevelChk(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_CB_LEVELCHK* pPacket = (WSTDS_CB_LEVELCHK*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(pUser)
		pUser->GoToWorld(pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD);
}


void CmdGmCommand_WSDS(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GM_COMMAND* pCommand = (WSTDS_GM_COMMAND*)pMsg; 
	pCommand;
}
