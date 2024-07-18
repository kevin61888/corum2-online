#include "stdafx.h"
#include "recvmsg.h"
#include "MsgProc.h"
#include "OwnServer.h"
#include "DungeonTable.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonLayer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "Monster.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "AcceptTable.h"
#include "MsgProcForServer.h"
#include "MsgProcForServerGuild.h"
#include "MsgProcForServerParty.h"
#include "MsgProcForServerPortal.h"
#include "RecordSet.h"
#include "DBProc.h"
#include "MonsterManager.h"
#include "GameSystemNative.h"
#include "EventDungeonManager.h"
#include "MsgProcForServerEvent.h"
#include "MsgProc_for_jjw6263.h"
#include "GameSystemNative.h"
#include "RivalGuildWar.h"

void __stdcall OnRecvFromServerTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	if(!g_bIsServerRunning)	return;
	if(!dwLength)	
	{
		g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
		return;
	}

	int nSize = 0;
	/*
	memcpy(&nSize,pMsg,SIZE_PACKET);
	if ( ! g_pNet->isCheckSize( nSize , dwLength - SIZE_PACKET) )
	{
		Log(LOG_IMPORTANT, "Illegal Packet or Hacked Packet !, %d , %d ",nSize , dwLength);
		return ;
	}
	char * szPacket =  new char[dwLength+1];
	memset(szPacket,0x00,dwLength +1);
	g_pNet->MakeDecPaket(pMsg + SIZE_PACKET,(dwLength -SIZE_PACKET) ,szPacket);
	*/
	char * szPacket  = pMsg;
	nSize  = 	dwLength;

	WORD wHeader;
	memcpy(&wHeader, szPacket, 2);

	if(dwConnectionIndex != WSINDEX)
	{
		char szIp[ 16 ]={0,};	WORD wPort=0;
		g_pNet->GetServerAddress(dwConnectionIndex, szIp, &wPort);
		Log(LOG_IMPORTANT, "Unknown packet received by ServerSide!(%s)", szIp);
		//delete[] szPacket;
		return;
	}

	switch(wHeader)
	{
		case Protocol_Server::CMD_SERVER_TERMINATE:					CmdServerTerminate(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_ADD_USER_ACCEPT_ALLOWED:			CmdAddUserAcceptAllowed(dwConnectionIndex, szPacket, nSize);			break;
		case Protocol_Server::CMD_REMOVE_USER_ACCEPT_ALLOWED:		CmdRemoveUserAcceptAllowed(dwConnectionIndex, szPacket, nSize);		break;
		case Protocol_Server::CMD_START_SIEGE:						CmdStartSiege(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_END_SIEGE:						CmdEndSiege(dwConnectionIndex, szPacket, nSize);						break;
		case Protocol_Server::CMD_PARTY_MSG:						CmdPartyMsg(dwConnectionIndex, szPacket, nSize);						break;
		case Protocol_Server::CMD_DUNGEON_INFO:						CmdDungeonInfo(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_DUNGEON_LEVELUP:					CmdDungeonLevelUp(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_REFRESH_ALL_DS_DATA:				CmdRefreshAllDungeonServerData(dwConnectionIndex, szPacket, nSize);	break;
		case Protocol_Server::CMD_REQUEST_USER_ACCEPT_FOR_PORTAL:	CmdRequestUserAcceptForPortal(dwConnectionIndex, szPacket, nSize);	break;		
		case Protocol_Server::CMD_USER_ACCEPT_ALLOWED_FOR_PORTAL:	CmdUserAcceptAllowedForPortal(dwConnectionIndex, szPacket, nSize);	break;		
		case Protocol_Server::CMD_USER_ACCEPT_FAILED_FOR_PORTAL:	CmdUserAcceptFailedForPortal(dwConnectionIndex, szPacket, nSize);	break;	
		case Protocol_Server::CMD_PARTY_INFO:						CmdPartyInfo(dwConnectionIndex, szPacket, nSize);					break;	
		case Protocol_Server::CMD_REFRESH_DUNGEON_OWNER_GUILD:		CmdRefreshDungeonOwnerGuild(dwConnectionIndex, szPacket, nSize);		break;	
		case Protocol_Server::CMD_EVENT_DUNGEON_INIT:				CmdEventDungeonInit(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_GUILD_INFO:						CmdServerGuildInfo(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_GUILD_FAIL:						CmdServerGuildFail(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_KICK_USER:						CmdKickUser(dwConnectionIndex, szPacket, nSize);						break;
		case Protocol_Server::CMD_NOTICE_ACTION:					CmdNoticeAction(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_DUNGEON_MESSAGE:					CmdDungeonMessage(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_REQUEST_DUNGEON_ALIVE:			CmdRequestDungeonAlive(dwConnectionIndex, szPacket, nSize);			break;
		case Protocol_Server::CMD_GUILD_MSG:						CmdGuildMsg(dwConnectionIndex, szPacket, nSize);						break;
		case Protocol_Server::CMD_GUILDUSER_INFO:					CmdGuildUserInfo(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_GUILDINFO_MSG:					CmdGuildInfoMsg(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_CHARACTER_SEARCH:					CmdCharacterSearch(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_GM_COMMAND_WS_DS:					CmdGmCommand_WSDS( dwConnectionIndex, szPacket, nSize );				break;
		case Protocol_Server::CMD_FRIENDUSER_INFO:					CmdFriendInfo( dwConnectionIndex, szPacket, nSize );					break;
		case Protocol_Server::CMD_EVENT_DUNGEON_OPEN_INFO:			CmdEventDungeonOpenInfo( dwConnectionIndex, szPacket, nSize);		break;
		case Protocol_Server::CMD_EVENT_DUNGEON_CLOSE_INFO:			CmdEventDungeonCloseInfo( dwConnectionIndex, szPacket, nSize);		break;
		case Protocol_Server::CMD_EVENT_DUNGEON_FIRST_TIMEOUT:		CmdEventDungeonFirstTimeOut( dwConnectionIndex, szPacket, nSize);	break;	
		case Protocol_Server::CMD_EVENT_JOINUSER_COUNT:				CmdEventDungeonJoinUserCount( dwConnectionIndex, szPacket, nSize);	break;		
		case Protocol_Server::CMD_USER_PORTAL:						CmdUserPortal(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_SET_DOUBLE_EXP:					CmdSetDoubleExp(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_SET_MAGICITEM_DROPRATE:			CmdSetMagicItemDropRate(dwConnectionIndex, szPacket, nSize);			break;
		case Protocol_Server::CMD_SET_EXPGAIN:						CmdSetExpGain(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_GUILD_LIST:						CmdGuildList(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_GULIDWAR_DELETE:					CmdGuildWarDelete(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_GUILD_RANK:						CmdGuildWarRank(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_GUILDUSER_RANK:					CmdGuildWarUserRank(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_SERVER_ATTACH_RESULT:				CmdServerAttachResult(dwConnectionIndex, szPacket, nSize);			break;
		case Protocol_Server::CMD_PARTY_BOARD:						CmdPartyBoard(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_EVENT_DUNGEON_CLOSE:				CmdEventDungeonClose(dwConnectionIndex, szPacket, nSize);			break;
		case Protocol_Server::CMD_GUILDWAR_MESSAGE:					CmdGuildWarMessage(dwConnectionIndex, szPacket, nSize);				break;
		case Protocol_Server::CMD_CHECK_DUNGEON_JOIN_RT:			CmdCheckDungeonJoinRt(dwConnectionIndex, szPacket, nSize);			break;
		case Protocol_Server::CMD_CB_LEVELCHK:						CmdCBLevelChk(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_PARTY_LEADER:						CmdPartyLeader(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_SET_PARTY_BONUS_EXP_GAIN:			CmdSetPartyBonusExpGain(dwConnectionIndex, szPacket, nSize);			break;
		case Protocol_Server::CMD_PARTY_CONFIG:						CmdPartyConfig(dwConnectionIndex, szPacket, nSize);					break;
		case Protocol_Server::CMD_TEAM_MATCH:						RECV_CMD_TEAM_MATCH(dwConnectionIndex, szPacket, nSize);				break;//: 050202 hwoarang
	}

	//delete[] szPacket;
}


void __stdcall OnRecvFromUserTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	BYTE bStatus = 0;
	BYTE bHeader = 0;

	try
	{
		try
		{
			if(!g_bIsServerRunning)		return;
			if(!dwLength)	
			{
				g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
				return;
			}
		}
		catch(...)
		{
			throw "Exception from g_pNet->CompulsiveDiscon";
		}
	
		int nSize = 0;
	/*
		memcpy(&nSize,pMsg,SIZE_PACKET);

		if ( ! g_pNet->isCheckSize( nSize , dwLength - SIZE_PACKET) )
		{
			Log(LOG_IMPORTANT, "Illegal Packet or Hacked Packet !, %d , %d ",nSize , dwLength);
			return ;
		}

		char * szPacket =  new char[dwLength+1];
		memset(szPacket,0x00,dwLength +1);
		g_pNet->MakeDecPaket(pMsg + SIZE_PACKET,(dwLength -SIZE_PACKET) ,szPacket);
	*/	

		char * szPacket  = pMsg;
		nSize  = 	dwLength;

		bStatus = *((BYTE*)(szPacket));
		bHeader = *((BYTE*)(szPacket+1));
		
		if(bStatus >= MAX_UPDATE_GAME)	
		{
	//		delete[] szPacket;
			return;	
		}
		
		if(PacketProc[ bStatus ][ bHeader ])
		{
			try
			{
				(*PacketProc[ bStatus ][ bHeader ])(dwConnectionIndex, szPacket, nSize);
	//		delete[] szPacket;
			}
			catch(...)
			{
				throw "Exception from PacketProc";
			}
		}
		else
		{
			if(bStatus == UPDATE_GAME_PLAY && bHeader == Protocol_CTS::CMD_ITEM_PICKUP)
			{
	//			delete[] szPacket;
				return;
			}
			if(bStatus == UPDATE_GAME_PLAY && bHeader == Protocol_CTS::CMD_ITEM_USED)
			{
	//			delete[] szPacket;
				return;
			}
			
			try
			{
				char szIp[16] = {0,};
				WORD wPort = 0;
				g_pNet->GetUserAddress(dwConnectionIndex, szIp, &wPort);
				Log(LOG_IMPORTANT, "Unknown packet received!(IP:%s), %d, %d", szIp, bStatus, bHeader );
				
		//		delete[] szPacket;
				return;
			}
			catch(...)
			{
				throw "Exception from g_pNet->GetUserAddress";
			}
		}
	
	}
	catch(char *msg)
	{
		Log(LOG_IMPORTANT, "%s, %d, %d", msg, bStatus, bHeader);
	}
	catch(...)
	{
		Log(LOG_IMPORTANT, "Unhandled excpetion!!");
		// throw;
	}
}


void ReceivedFromDB(DBRECEIVEDATA* pResult)
{
	if(!g_bIsServerRunning)
		return;
	
	switch(pResult->bQueryType)
	{
		case QUERY_TYPE_SELECT:	// THOpenRecord 
		{
			if(!pResult->Query.select.pResult || !pResult->Query.select.dwRowCount)
			{
				Log( LOG_NORMAL, "pResult->Query.select.pResult == NULL" );
				return;
			}
			else
			{
				if( pResult->Query.select.dwRowCount == 0 )
					Log( LOG_NORMAL, "pResult->Query.select.pResult != NULL, dwRowCount == 0" );
			}

			switch(pResult->dwQueryUID)
			{
				case DB_USER_CHARACTER:			QueryTypeUserCharacter(pResult);		break;				
				case DB_BANK_USER_INFO:			QueryTypeBankUserInfo(pResult);			break;				
				case DB_USER_PLAYERSHOP:		QueryTypeUserPlayerShop(pResult);		break;								
				case DB_UUID_DUNGEON_JOIN:		QueryTypeUUIDDungeonJoin(pResult);		break;								
				case DB_CREATE_GUARDIAN:		QueryTypeCreateGuardian(pResult);		break;								
				case DB_UUID_GUARDIAN_INFO:		QueryTypeUUIDGuardianInfo(pResult);		break;								
				case DB_PRESENT_COUNT_CHECK:	QueryTypePresentCountCheck(pResult);	break;
			}
		}
		break;

		case QUERY_TYPE_EXECUTE: // THExecuteSQL
		{
			if(pResult->nError < 0)	break;
				
			switch( pResult->dwQueryUID )
			{
				case DB_CHANGE_DUNGEON_OWNER:	QueryTypeChangeDungeonOwner(pResult);	break;
			}			
		}
		break;

		case QUERY_TYPE_EXECUTE_BY_PARAM:	// THExecuteSQLByParam
		{
			switch(pResult->dwQueryUID)
			{
				case QUERY_TYPE_USER_PLAYERSHOP:	QueryTypeUserPlayershop(pResult);	break;
				case QUERY_TYPE_USER_UPDATE:		QueryTypeUserUpdate(pResult);		break;
				case QUERY_TYPE_USER_BANK_UPDATE:	QueryTypeUserBankUpdate(pResult);	break;	
				case QUERY_TYPE_TRADE:				QueryTypeTrade(pResult);			break;	
			}
		}
		break;
	}	
}


void ReceivedFromDBLog(DBRECEIVEDATA* pResult)
{
	if(!g_bIsServerRunning)	return;
}

void __stdcall SuccessToConnectWorldServer(DWORD dwConnectionIndex, void* pExt)
{
	g_pNet->PauseTimer( 2 );
	Log(LOG_JUST_DISPLAY, "@ World Server Connect Successfully!");
	
	g_pThis->SetWSConnectionIndex(dwConnectionIndex);

	DSTWS_SERVER_ATTACH	packet;
	packet.bType	= g_pThis->GetServerType();
	packet.dwID		= g_pThis->GetServerID();

	g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION );
	
	ReportToCMListener(1, &g_MachineName);

	if(!g_pDungeonTable)
	{
		Log(LOG_JUST_DISPLAY, "@!!!!!!!! This Dungeon Have No DungeonTable!!!");	
		return;
	}

	POSITION_ DungeonPos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();
	while( DungeonPos )
	{
		CDungeon* pDungeon = (CDungeon*)g_pDungeonTable->m_pDungeonList->GetNext( DungeonPos );	

		// 던젼 층의 처리
		for(BYTE i = 0; i < pDungeon->GetTotalLayer(); i++ )
		{
			CDungeonLayer *pDL =  pDungeon->GetDungeonLayer(i);

			if(	pDL 
			&&	pDL->m_pTeamMatch )//길드전 상태 보고: Hwoarang 050202
			{
				pDL->m_pTeamMatch->Clear();//월드랑 붙을때 초기화
				Send_DSTWS_STADIUM_STATE_MODIFY(pDL->m_pTeamMatch);
			}
		}
		
	}
}

void __stdcall FailToConnectWorldServer(void* pExt)
{
	Log(LOG_IMPORTANT, "Fail To Connect WorldServer! (IP:%s, Port:%d)", g_pThis->GetIPForWorldServer(), g_pThis->GetPortForWorldServer() );

	g_pNet->ResumeTimer( 2 );
}


void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	if(!g_bIsServerRunning)
		return;

	if(WSINDEX == dwConnectionIndex)
	{
		Log(LOG_FATAL, "World Server Disconnected! Server Will Terminate...");
		
		g_pNet->ResumeTimer(2);

		POSITION_ pos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();

		CDungeon *pDungeon = NULL;

		while(pos)
		{
			pDungeon = (CDungeon*)g_pDungeonTable->m_pDungeonList->GetNext(pos);	

			if(pDungeon->GetDungeonDataEx()->m_bSiege)
			{
				pDungeon->EndSiege();
			}
		}
	}
	else
	{
		char szIP[ MAX_IP_LENGTH ] = {0,};
		WORD wPort=0;

		g_pNet->GetServerAddress(dwConnectionIndex, szIP, &wPort);

		Log(LOG_IMPORTANT, "Unknown Server Disconnected!(IP=%s)",szIP);
	}
}


void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if(!g_bIsServerRunning)			return;
	if(!pUser)						return;
	if(!pUser->GetCurDungeon())		return;	

	__DisconnectUserInTeamMatch( pUser );//: hwoarang 050202 //접속 끝날때 팀 카운트 줄여준다
	__DisconnectUserInMatch(pUser);
	__DisconnectUserInEventDungeon(pUser);
	__DisconnectUserDisAppear(pUser);
	__DisconnectUserItemReturnToInv(pUser);	
	__DisconnectUserUpdate(pUser, dwConnectionIndex);
}


void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
	
}

void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
		
}


void __DisconnectUserInMatch(CUser* pUser)
{
	// 대전관련 
	if (pUser->m_sMatchDescInfo.m_bMatching)
	{
		// 대전중인 유저가 강제 종료 하였다. 
		CUser * pMatchUser = g_pUserHash->GetData(pUser->m_sMatchDescInfo.m_dwMatchUserIndex);

		if(pMatchUser)
		{
			DSTC_END_MATCH_MAN2MAN	packet;

			// 비정상종료 
			packet.bJudgmentCode = 3;				
			memset(packet.szWinnerName, 0, sizeof(packet.szWinnerName));
			memset(packet.szLoserName, 0, sizeof(packet.szLoserName));
			__lstrcpyn(packet.szWinnerName, pMatchUser->m_szName,20);
			__lstrcpyn(packet.szLoserName, pUser->m_szName, 20);

			// 마을에 대전 정보를 알린다.
			pMatchUser->GetCurDungeon()->GetDungeonLayer(0)->BroadCastMsg((char *)&packet, packet.GetPacketSize());

			CDungeonLayer * pLayer = pMatchUser->GetCurDungeonLayer();

			if (pLayer && pLayer->m_bMatching == TRUE)
			{
				// 대전하고 있는 레이어 안에 있는 유저에게도 알린다. 
				pLayer->BroadCastMsg((char *)&packet, packet.GetPacketSize());
				pLayer->m_dwEndMatchJudgmentTick = 0;	
				pLayer->m_dwMatchEndTime = g_dwTickCount + 5000;
			}

			// 마을로 간다. 
			// pMatchUser->GetCurDungeon()->GoToWorldUser(pMatchUser, pMatchUser->m_bDestWorldSpot);
		}
	}
	else
	{
		// 대전중이 아니다.
		if (NULL != pUser->m_sMatchDescInfo.m_dwMatchUserIndex)
		{
			// 대전 신청하고 종료
			pUser->CancelMatch(MATCH_STATUS_USER_REQUEST_AFTER_EXIT_GAME);
		}
	}
	
	if (pUser->m_sPKDescInfo.m_pPKList)
	{
		g_pThis->m_pPKUserCheckList->RemoveAt(pUser->m_sPKDescInfo.m_pPKList);
	}
}


void __DisconnectUserInEventDungeon(CUser* pUser)
{
	Event_DisconnectUser( pUser );
}


void __DisconnectUserDisAppear(CUser* pUser)
{
	DSTC_DISAPPEAR	DisAppear;
	DisAppear.dwUserIndex = pUser->GetID();
	
	// 메시지를 보낸다.
	if( pUser->GetCurLayerIndex() != INVALID_DUNGEON_LAYER )
	{
		pUser->GetCurDungeonLayer()->BroadCastSectionMsg((char*)&DisAppear, DisAppear.GetPacketSize(), pUser );
		UpdateUserForAI( pUser );
	}
	else
	{
		Log(LOG_NORMAL, "User that has invalid dungeon layer Entered!(Name:%s)", pUser->m_szName);
	}
}


void __DisconnectUserItemReturnToInv(CUser* pUser)
{
	if( IsUltra() )
	{
		
	}
	else
	{
		if(pUser->m_ItemGuardian.GetID()!=0)
		{
			if(GetItemType(pUser->m_ItemGuardian.GetID())==ITEM_TYPE_SMALL)
			{
				Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
				memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
			}
		}
			
		// 마우스에 들고 있다면.
		if (pUser->m_ItemMouse.GetID())
		{
			switch(GetItemType(pUser->m_ItemMouse.GetID()))
			{
			case ITEM_TYPE_LARGE:
				{
					BOOL bChk = FALSE;
					
					for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].GetID()==0)
						{
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							bChk = TRUE;
							break;
						}									
					}

					if (!bChk)
					{
						// 떨구어야 겠다.
						AppearItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), ITEM_TYPE_LARGE);
					}
				}
				break;
			case ITEM_TYPE_SMALL:
				{
					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);
				}
				break;
			}

			memset(&pUser->m_ItemMouse, 0, sizeof(pUser->m_ItemMouse));
		}
			
		for(int i = 1; i < 4; i++)
		{
			if(pUser->m_pUpgradeItem[i].GetID()!=0)
			{
				if(GetItemType(pUser->m_pUpgradeItem[i].GetID())==ITEM_TYPE_SMALL)
				{				
					BYTE byCnt = (BYTE)Insert_SmallInvItem(pUser, &pUser->m_pUpgradeItem[i], 0, pUser->m_pUpgradeItem[i].GetQuantity(), FALSE);

					if(byCnt==0)
						memset(&pUser->m_pUpgradeItem[i], 0, sizeof(CItem));
					else
						pUser->m_pUpgradeItem[i].SetQuantity(byCnt);
				}
			}
		}

		for(i = 1; i < 4; i++)
		{
			if(pUser->m_pMakingItem[i].GetID()!=0)
			{
				if(GetItemType(pUser->m_pMakingItem[i].GetID())==ITEM_TYPE_SMALL)
				{				
					BYTE byCnt = (BYTE)Insert_SmallInvItem(pUser, &pUser->m_pMakingItem[i], 0, pUser->m_pMakingItem[i].GetQuantity(), FALSE);

					if(byCnt==0)
						memset(&pUser->m_pMakingItem[i], 0, sizeof(CItem));
					else
						pUser->m_pMakingItem[i].SetQuantity(byCnt);
				}
			}
		}
		
		for(i = 1; i < 4; i++)
		{
			if(pUser->m_pUpgradeItem[i].GetID()!=0)
			{
				if(GetItemType(pUser->m_pUpgradeItem[i].GetID())==ITEM_TYPE_SMALL)					
					Insert_SmallBankItem(pUser, &pUser->m_pUpgradeItem[i], 0, pUser->m_pUpgradeItem[i].GetQuantity(), FALSE);										
			}
		}

		for(i = 1; i < 4; i++)
		{
			if(pUser->m_pMakingItem[i].GetID()!=0)
			{
				if(GetItemType(pUser->m_pMakingItem[i].GetID())==ITEM_TYPE_SMALL)					
					Insert_SmallBankItem(pUser, &pUser->m_pMakingItem[i], 0, pUser->m_pMakingItem[i].GetQuantity(), FALSE);										
			}
		}

		if(pUser->m_pUpgradeItem[0].GetID() != 0 && GetItemType(pUser->m_pUpgradeItem[0].GetID())==ITEM_TYPE_LARGE)
		{			
			for(i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(pUser->m_pInv_Large[i].GetID()==0)
				{
					memcpy(&pUser->m_pInv_Large[i], &pUser->m_pUpgradeItem[0], sizeof(CItem));
					memset(&pUser->m_pUpgradeItem[0], 0, sizeof(CItem));
					break;
				}									
			}

			if(pUser->m_pUpgradeItem[0].GetID()!=0)
			{
				for(i = 0; i < MAX_BANK_LARGE_POOL; i++)
				{
					if(pUser->m_pBank_Large[i].GetID()==0)
					{
						memcpy(&pUser->m_pBank_Large[i], &pUser->m_pUpgradeItem[0], sizeof(CItem));
						memset(&pUser->m_pUpgradeItem[0], 0, sizeof(CItem));
						break;
					}					
				}
			}
		}
		
		//pUser->m_pMakingItem[0].GetID() != 0 && 
		if(GetItemType(pUser->m_pMakingItem[0].GetID())==ITEM_TYPE_LARGE)
		{			
			for(i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(pUser->m_pInv_Large[i].GetID()==0)
				{
					memcpy(&pUser->m_pInv_Large[i], &pUser->m_pMakingItem[0], sizeof(CItem));
					memset(&pUser->m_pMakingItem[0], 0, sizeof(CItem));
					break;
				}									
			}

			if(pUser->m_pMakingItem[0].GetID()!=0)
			{
				for(i = 0; i < MAX_BANK_LARGE_POOL; i++)
				{
					if(pUser->m_pBank_Large[i].GetID()==0)
					{
						memcpy(&pUser->m_pBank_Large[i], &pUser->m_pMakingItem[0], sizeof(CItem));
						memset(&pUser->m_pMakingItem[0], 0, sizeof(CItem));
						break;
					}					
				}
			}
		}
		
		for(i = 0; i < 4; i++)
			memset(&pUser->m_pUpgradeItem[i], 0, sizeof(pUser->m_pUpgradeItem[i]));

		for(i = 0; i < 4; i++)
			memset(&pUser->m_pMakingItem[i], 0, sizeof(pUser->m_pMakingItem[i]));

		// User Trade //
		if(pUser->m_nItemTradeSeriel != 0)
		{
			ITEM_TRADE* pItemTrade = g_pItemTradeHash->GetData(pUser->m_nItemTradeSeriel);

			if(pItemTrade)
			{
				DWORD dwIndex = 0;

				if(pUser)
				{
					if(pItemTrade->TradeSerialStruct.dwHiUserIndex == pUser->GetID())
						dwIndex = pItemTrade->TradeSerialStruct.dwLowUserIndex;
					else
						dwIndex = pItemTrade->TradeSerialStruct.dwHiUserIndex;						

					CUser* pSendUser = g_pUserHash->GetData(dwIndex);
				
					// Item Move //
					for(int i = 0; i < MAX_TRADE_POOL; i++)
					{
					//	int nValue = pUser->m_pTradeItem[i].GetID()/ITEM_DISTRIBUTE;

						if(pUser->m_pTradeItem[i].GetID()!=0)
						{
							if(GetItemType(pUser->m_pTradeItem[i].GetID())==ITEM_TYPE_SMALL)
							{
								// 스몰 아이템 //
								Insert_SmallInvItem(pUser, &pUser->m_pTradeItem[i], 0
									, pUser->m_pTradeItem[i].GetQuantity(), TRUE);
							}
							else if(GetItemType(pUser->m_pTradeItem[i].GetID())==ITEM_TYPE_LARGE)
							{
								// 큰 아이템 //
								for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
								{
									if(pUser->m_pInv_Large[j].GetID()==0)
									{
										memcpy(&pUser->m_pInv_Large[j], &pUser->m_pTradeItem[i], sizeof(CItem));
										break;
									}
								}
							}
						}

						memset(&pUser->m_pTradeItem[i], 0, sizeof(CItem));
					}				
									
					if(pSendUser)
					{
						// 취소메세지 //
						DSTC_TRADE_CANCELFAIL pSnedUserPacket;
						pSnedUserPacket.byType	= 0;
						memset(pSnedUserPacket.szCharacterName, 0, sizeof(pSnedUserPacket.szCharacterName));
						__lstrcpyn(pSnedUserPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSnedUserPacket, pSnedUserPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						
						if(pSendUser->m_nItemTradeSeriel==0)
						{
							g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
							memset(pItemTrade, 0, sizeof(ITEM_TRADE));
						}
					}
					
					if(!pSendUser)
					{
						g_pItemTradeHash->Delete(pItemTrade, pUser->m_nItemTradeSeriel, TRUE);
						memset(pItemTrade, 0, sizeof(ITEM_TRADE));
					}

					pUser->m_nItemTradeSeriel	= 0;
					pUser->m_byExTrade			= 0;	
				}
			}
		}	
			
		ItemShopRemove(pUser);
	}
}


void __DisconnectUserUpdate(CUser* pUser,DWORD dwConnectionIndex)
{	
#if defined JAPAN_LOCALIZING
	// 일본 서버에서는 사용자 되살아날때 전체 HP 의 50 % 회복 ( 2004. 08. 12. minjin )
	if ( pUser->GetHP() == 0 )
		pUser->SetHP((WORD)(pUser->GetOriginalMaxHP() / 2));
#else
	// 다른곳은 HP 1 로 되살아남
	if (!pUser->GetHP())
		pUser->SetHP(1);
#endif

	UpdateUserDBThread( pUser );

	char szMon[255] = {0,};
	wsprintf( szMon, "CZP_KillingMonsterCount '%s', %d", pUser->GetName(), pUser->GetKillMonCount() );
	g_pDb->THExecuteSQL( szMon, FALSE, 0, NULL, GAME_DB );
	pUser->SetKillMonCount(NULL);
	
	if(pUser->pBufForPortal)
	{
		LALFree(g_pPortalBufferPool, pUser->pBufForPortal);
		pUser->pBufForPortal = NULL;
	}	

	if(pUser->GetCurDungeon())
	{
		pUser->GetCurDungeon()->RemoveUser( pUser );
	}

	ACCEPT_USER* pAcceptUser = (ACCEPT_USER*)0xffffffff;

	while( pAcceptUser )
	{
		pAcceptUser = g_pAcceptTable->GetAcceptInfo(pUser->GetID());

		if(pAcceptUser)
		{
			Log(LOG_NORMAL, "Remained User in AcceptTable Occured!");
			g_pAcceptTable->Remove( pAcceptUser );
		}
		else
		{
			break;
		}
	}

	g_pUserHash->Delete( pUser, pUser->GetID() );

	delete pUser;

	g_pNet->SetUserInfo( dwConnectionIndex, NULL );	
}








