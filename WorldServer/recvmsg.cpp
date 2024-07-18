#include "stdafx.h"
#include "recvmsg.h"
#include "DungeonTable.h"
#include "AcceptTable.h"		
#include "PartyHash.h"
#include "Map.h"
#include "ConnectionDistributionTable.h"
#include "MsgProc.h"
#include "MsgProcForServer.h"
#include "MsgProcForServerParty.h"
#include "MsgProcForServerGuild.h"
#include "MsgProcForServerFromAgent.h"
#include "MsgProcForServerPortal.h"
#include "DBProc.h"
#include "Guild.h"
#include "Party.h"
#include "Messenger.h"
#include "ServerFunction.h"
#include "NTOS.h"
#include "RivalGuildWar.h"	


void	(*PacketProc[ MAX_UPDATE_GAME ][ MAX_PACKET_NUM ])(CWorldUser* pUser ,char* pMsg,DWORD dwLength);


extern STMPOOL_HANDLE g_pGuildOfflinePool;


void InitializePacketProc()
{
	memset(PacketProc, 0, sizeof(PacketProc));

	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CHECK_GM ]					= CmdCheckGM;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CM_KICK_USER ]				= CmdCMKickUser;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CHECK_MONITORING ]			= CmdCheckMonitoring;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REQUEST_DUNGEON_INFO ]		= CmdRequestDungeonInfo;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_TRY_TO_JOIN_DUNGEON ]		= CmdTryToJoinDungeon;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_NOTICE ]					= CmdNotice;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REFRESH_RECENT_DUNGEON ] = CmdRefreshRecentDungeon;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_SUCCESS ]			= CmdCommand;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_DSUCCESS ]			= DGCmdCommand;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_MESSAGERQ ]		= CmdMessageRQ;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CHAT_USER ]				= CmdChatUser;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_DGCHAT_USER]				= CmdDSChatUser;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_CREATE]				= CmdGuildCreate;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDJOIN_MESSAGE]			= CmdGuildJoinMessage;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_JOIN]				= CmdGuildJoin;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_RANK]				= CmdGuildRank;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_INFO]				= CmdGuildInfo;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_USERDEL]				= CmdGuildUserDel;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_DELETE]				= CmdGuildDelete;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_JOIN]				= CmdPartyJoin;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_APPROVE]				= CmdPartyApprove;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_LOGOUTCL]			= CmdPartyLogoutCl;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_SKILL_KEYRT]				= CmdSkillKeyRt;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_COMMAND_MSG]				= CmdCommandMsg;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_SET]					= CmdPartySet;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_INFO]				= CmdPartyInfo;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_LOGOUTUSER]			= CmdPartyLogOutUser;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_QUERY_EVENT_DUNGEON_INFO]	= CmdQueryEventInfo;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_REQUEST_OWN_DUNGEON_INFO]	= CmdRequestOwnDungeonInfo;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_HELP_INFO]					= CmdHelpInfo;	
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GO_TO_WORLD_SUCCESS ]		= CmdGoToWorldSuccess;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_MOVE ]				= CmdWorldMove;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_STOP ]				= CmdWorldStop;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_WORLD_SET_POSITION ]		= CmdWorldSetPosition;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_AIRSHIP_RIDING ]			= CmdAirshipRiding;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_AIRSHIP_GETOFF ]			= CmdAirshipGetoff;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CHECK_CONNECTION ]			= CmdCheckConnection;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_LOADING_STATUS ]			= CmdLoadingStatus;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_USERREQUEST]			= CmdPartyUserRequest;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_USERREQUESTWS_FAIL]	= CmdPartyUserRequestFail;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDUSER_INFO]			= CmdGuildUserInfo;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_REQUEST]				= CmdGuildRequest;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_USERREQUESTWS_FAIL]	= CmdGuildUserRequestFail;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_DATA_REQUEST]		= CmdGuildDataRequest;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_MESSENGER_USERADD]			= CmdMessengerUserAdd;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_CHARACTER_SEARCH]			= CmdCharacterInfoSearch;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_MESSENGER_ANSWER]			= CmdMessengerAnswer;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_FRIENDUSER_INFO]			= CmdMessengerInfo;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_FRIENDUSER_DELETE]			= CmdMessengerDelete;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_MESSENGER_STATUS]			= CmdMessengerStatus;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_USER_GUILDMASTER]			= CmdUserGuildMaster;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDWAR_REQUEST]			= CmdGuildWarRequest;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDWAR_ERRORRT]			= CmdGuildWarErrorRt;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDWAR_MASTERRT]			= CmdGuildWarMasterRt;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDWAR_CREATE]			= CmdGuildWarCreate;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDWAR_EXIT]				= CmdGuildWarExit;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILDWAR_RANK]				= CmdGuildWarRank;		
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_ENTRY]				= CmdPartyEntry;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_INFOREQUEST]			= CmdPartyInfoRequest;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_BOARDDELETE]			= CmdPartyBoardDelete;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_CONFIG]				= CmdPartyConfig;			// 파티 설정 : 최덕석 2005.1.25
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTY_FRIEND]				= CmdPartyFriend;			// 파티 설정 : 최덕석 2005.1.25
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PARTYUSER_WORLDINFO]		= CmdPartyUserWorldInfo;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GM_COMMAND ]				= CmdGmCommand;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_TIMER_READY]				= CmdTimerReady;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_TIMER_ACK]					= CmdTimerAck;
	// 파티 매칭 : 최덕석 2005.1.24
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PMATCH_REGISTER]			= CmdPMatchRegister;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PMATCH_ESCAPE]				= CmdPMatchEscape;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PMATCH_REQUEST_RESULT]		= CmdPMatchRequestResult;
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_PMATCH_CHANGE]				= CmdPMatchChange;


	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_TEAM_MATCH]				= Recv_CMD_TEAM_MATCH;//팀매치 관련 패킷 : hwoarang 050129 
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_MATCHBEGIN_SETTING]	= CmdGuildMatchBeginSetting;//팀매치 관련 패킷 : hwoarang 050129 
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_MATCH_STATUS_REQUEST]		= CmdGuildMatchStatusRequest;//팀매치 관련 패킷 : hwoarang 050129 
	PacketProc[ UPDATE_GAME_WORLD ][ Protocol_World::CMD_GUILD_MATCH_CANCEL_OBSERVER]		= CmdGuildMatchCancelObserver;//팀매치 관련 패킷 : hwoarang 050129 
}


void __stdcall OnRecvFromServerTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	if(!g_bIsServerRunning)	return;

	START_FUNCTION_PROFILE();

	int nSize = 0;
	/*
	memcpy(&nSize,pMsg,SIZE_PACKET);
	if ( ! g_pNet->isCheckSize( nSize , dwLength - SIZE_PACKET) )
	{
		Log(LOG_IMPORTANT, "Illegal Packet or Hacked Packet !, %d , %d ",nSize , dwLength);
		return ;
	}
	char * szPacket =  new char[dwLength];
	memset(szPacket,0x00,dwLength);
	g_pNet->MakeDecPaket(pMsg + SIZE_PACKET,( dwLength - SIZE_PACKET),szPacket);
	*/
	char * szPacket  = pMsg;
	nSize  = 	dwLength;

	WORD wHeader;
	memcpy(&wHeader, szPacket, 2);
	
	SERVER_DATA* pServer = (SERVER_DATA*)g_pNet->GetServerInfo(dwConnectionIndex);

	// Login 하지 않은 서버로부터의 패킷 처리 
	if(!pServer)
	{
		switch(wHeader)
		{
			case Protocol_Server::CMD_SERVER_ATTACH:				CmdServerAttach(dwConnectionIndex, szPacket, nSize);			break;
			case Protocol_Agent_Server::ACMD_SERVER_ATTACH_RESULT:	AcmdServerAttachResult(dwConnectionIndex, szPacket, nSize);	break;
		
			default:
			{
				char szIp[ 16 ]={0,}; WORD wPort=0;
				g_pNet->GetServerAddress(dwConnectionIndex, szIp, &wPort);
				Log(LOG_IMPORTANT, "Unknown packet received by ServerSide!(%s)", szIp);
				//delete[] szPacket;
				return;
			}
		}
	}

	switch(wHeader)
	{
		case Protocol_Server::CMD_USER_ACCEPT_ALLOWED:					CmdUserAcceptAllowed(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_USER_ACCEPT_FAILED:					CmdUserAcceptFailed(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_DUNGEON_MAGICARRAY_ATTACH:			CmdDungeonMagicArrayItem(pServer, szPacket, nSize);				break;
		case Protocol_Server::CMD_DUNGEON_OWNER_CHANGE:					CmdDungeonOwnerChange(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_DUNGEON_GUARDIANITEM_ATTACH:			CmdDungeonGuardianItem(pServer, szPacket, nSize);				break;
		case Protocol_Server::CMD_DUNGEON_GROUPEXP:						CmdDungeonGroupExp(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_GOTOWORLD:							CmdGotoWorld(pServer, szPacket, nSize);							break;
		case Protocol_Server::CMD_REQUEST_USER_ACCEPT_FOR_PORTAL:		CmdRequestUserAcceptForPortal(pServer, szPacket, nSize);			break;
		case Protocol_Server::CMD_USER_ACCEPT_ALLOWED_FOR_PORTAL:		CmdUserAcceptAllowedForPortal(pServer, szPacket, nSize);			break;
		case Protocol_Server::CMD_USER_ACCEPT_FAILED_FOR_PORTAL:		CmdUserAcceptFailedForPortal(pServer, szPacket, nSize);			break;	
		case Protocol_Server::CMD_DUNGEON_JOIN_SUCCESS:					CmdDungeonJoinSuccess(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_REFRESH_PRODUCT_ACQUISITION:			CmdRefreshProduct_Acquisition(pServer, szPacket, nSize);			break;
		case Protocol_Server::CMD_CREATEPRODUCTION:						CmdCreateProduct(pServer, szPacket, nSize);						break;
		case Protocol_Server::CMD_REFRESH_ACCENTRANCE_ACQUISITION:		CmdRefreshAccEntrance_Acquisition(pServer, szPacket, nSize);		break;	
		case Protocol_Server::CMD_REFRESH_ACCEXP_ACQUISITION:			CmdRefreshAccExp_Acquisition(pServer, szPacket, nSize);			break;	
		case Protocol_Server::CMD_REFRESH_ENTRANCE_EDIT:				CmdRefreshEntrance_Edit(pServer, szPacket, nSize);				break;	
		case Protocol_Server::CMD_REFRESH_DEFENSECOUNT:					CmdRefreshDefenseCount(pServer, szPacket, nSize);				break;	
		case Protocol_Server::CMD_REFRESH_DUNGEON_OPERATIONMODE:		CmdRefreshOperationMode(pServer, szPacket, nSize);				break;			
		case Protocol_Server::CMD_EVENT_DUNGEON_USER_OUT:				CmdEventDungeonUserOut(pServer, szPacket, nSize);				break;
		case Protocol_Server::CMD_DUNGEON_ALIVE_IFNO:					CmdDungeonAliveInfo(pServer, szPacket, nSize);					break;		
		case Protocol_Server::CMD_PARTY_LEVEL:							CmdPartyLevel(pServer, szPacket, nSize);							break;
		case Protocol_Server::CMD_PARTY_INFORT:							CmdPartyInfo(pServer, szPacket, nSize);							break;
		case Protocol_Server::CMD_PARTY_HP:								CmdPartyHp(pServer, szPacket, nSize);							break;
		case Protocol_Server::CMD_PARTY_PORTAL:							CmdPartyPortal(pServer, szPacket, nSize);						break;
		case Protocol_Server::CMD_PARTY_PORTALMOVE:						CmdPartyPortalMove(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_PARTY_PORTALUSED:						CmdPartyPortalUsed(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_GUILD_PORTALUSED:						CmdGuildPortalUsed(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_PARTY_ALLPORTAL:						CmdPartyAllPortal(pServer, szPacket, nSize);						break;
		case Protocol_Server::CMD_GUILD_ALLPORTAL:						CmdGuildAllPortal(pServer, szPacket, nSize);						break;		
		case Protocol_Server::CMD_PARTY_PORTALFAIL:						CmdPartyPotalFail(pServer, szPacket, nSize);						break;
		case Protocol_Server::CMD_GUILD_PORTALFAIL:						CmdGuildPotalFail(pServer, szPacket, nSize);						break;
		case Protocol_Server::CMD_GUILD_LEVEL:							CmdGuildLevel(pServer, szPacket, nSize);							break;
		case Protocol_Server::CMD_GUILDUSER_INFORT:						CmdGuildUserInfoRt(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_CHARACTER_SEARCHRT:					CmdCharacterSearch(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_FRIENDUSER_INFORT:					CmdFriendUserInfoRt(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_GULIDWAR_POINT:						CmdGuildWarPoint(pServer, szPacket, nSize);						break;
		case Protocol_Server::CMD_CHECK_DUNGEON_JOIN:					CmdCheckDungeonJoin(pServer, szPacket, nSize);					break;
		case Protocol_Server::CMD_GM_COMMAND_DS_WS:						CmdGmCommand_DSWS(pServer, szPacket, nSize );					break;
		case Protocol_Server::CMD_EVENT_DUNGEON_USER_ENTER_BY_PORTAL:	CmdEventDungeonUserEnter(pServer, szPacket, nSize);				break;

		case Protocol_Server::CMD_STADIUM_STATE:						Recv_CMD_STADIUM_STATE(pServer, szPacket, nSize); break;//운동장이 만들어 졌다.: hwoarang 050202

		case Protocol_Agent_Server::ACMD_ADD_NEW_USER:					AcmdAddNewUser(pServer, szPacket, nSize);						break;
		case Protocol_Agent_Server::ACMD_KICK_USER:						AcmdKickUser(pServer, szPacket, nSize);							break;
		case Protocol_Agent_Server::ACMD_REQUEST_SERVER_ALIVE:			ACmdRequestServerAlive(pServer, szPacket, nSize);				break;
		case Protocol_Agent_Server::ACMD_CMTOOL_NOTICE:					ACmdCMToolNotice(pServer, szPacket, nSize);						break;
		case Protocol_Agent_Server::ACMD_BILLING_SMALL_TIME:			AcmdBillingSmallTime(dwConnectionIndex, szPacket, nSize);		break;
		case Protocol_Agent_Server::ACMD_CHANGE_BILLING_TYPE:			AcmdChangeBillingType(dwConnectionIndex, szPacket, nSize);		break;
		case Protocol_Agent_Server::ACMD_GUILD_CHARACTER_DELETE:		ACmdGuildCharacterDelete(pServer, szPacket, nSize);				break;
	}

	//delete[] szPacket;

	END_FUNCTION_PROFILE();
}

void __stdcall OnRecvFromUserTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength)
{
	BYTE bStatus = 0;
	BYTE bHeader = 0;

	try
	{

		if(!g_bIsServerRunning)	return;
		
		try
		{
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

		START_FUNCTION_PROFILE();

		int nSize = 0;
		/*
		memcpy(&nSize,pMsg,SIZE_PACKET);
		if ( ! g_pNet->isCheckSize( nSize , dwLength - SIZE_PACKET) )
		{
			Log(LOG_IMPORTANT, "Illegal Packet or Hacked Packet !, %d , %d ",nSize , dwLength);
			return ;
		}
		char * szPacket = new char [dwLength];
		memset(szPacket,0x00,dwLength);
		g_pNet->MakeDecPaket(pMsg + SIZE_PACKET,( dwLength - SIZE_PACKET) ,szPacket);
		*/
		char * szPacket = pMsg;
		nSize = dwLength;

		bStatus = *((BYTE*)(szPacket));
		bHeader = *((BYTE*)(szPacket+1));
		
		CWorldUser *pUser = NULL;
		
		try
		{
			pUser = (CWorldUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		}
		catch(...)
		{
			throw "g_pNet->GetUserInfo";
		}

		// Login 하지 않은 유저로부터의 패킷 처리 
		if(!pUser)
		{	
			if(bStatus == UPDATE_GAME_CHAR_SELECT && bHeader == Protocol_CharSelect::CMD_WORLD_LOGIN)
			{
				try
				{
					CmdWorldLogin(dwConnectionIndex, szPacket, nSize);
				}
				catch(...)
				{
					throw "CmdWorldLogin";
				}

			}
			else
			{
				try
				{
					char szIp[ 16 ]={0,}; WORD wPort=0;
					g_pNet->GetUserAddress(dwConnectionIndex, szIp, &wPort);
					Log(LOG_IMPORTANT, "Unknown packet received!(%s), (bStatus:%d, bHeader:%d)", szIp, bStatus, bHeader);
				}
				catch(...)
				{
					throw "Exception from g_pNet->GetUserAddress";
				}

			}

			END_FUNCTION_PROFILE();
			//delete[] szPacket;
			return;
		}
		
		if(bStatus>=MAX_UPDATE_GAME)
		{
			//delete[] szPacket;
			return;
		}

		if(PacketProc[ bStatus ][ bHeader ])
		{
			try
			{
				(*PacketProc[ bStatus ][ bHeader ])(pUser, szPacket, nSize);
			}
			catch(...)
			{
				throw "Exception from PacketProc";
			}
		}
		else
		{
			try
			{
				char szIp[ 16 ]={0,}; WORD wPort=0;
				g_pNet->GetUserAddress(dwConnectionIndex, szIp, &wPort);
				Log(LOG_IMPORTANT, "Unknown packet received!(ID:%s, IP:%s, bStatus:%d, bHeader:%d)", pUser->m_szID, szIp, bStatus, bHeader);
			}
			catch(...)
			{
				throw "Exception from g_pNet->GetUserAddress";
			}
		}

		//delete[] szPacket;
		END_FUNCTION_PROFILE();
	}
	catch(char *msg)
	{
		Log(LOG_IMPORTANT, "%s, %d, %d", msg, bStatus, bHeader);
	}
	catch(...)
	{
		Log(LOG_IMPORTANT, "Unhandled excpetion!!");
	}
}


void ReceivedFromDB(DBRECEIVEDATA* pResult)
{	
	if(!g_bIsServerRunning)	return;
	
	START_FUNCTION_PROFILE();
	
	switch(pResult->bQueryType)
	{
		case QUERY_TYPE_SELECT:	
		{
			if(!pResult->Query.select.pResult && pResult->dwQueryUID != QUERY_TYPE_MESSAGERQ)
			{
				Log(LOG_IMPORTANT, "pResult->Query.select.pResult is NULL!(QueryID:%d ,RowSize:%d)", pResult->dwQueryUID, pResult->Query.select.dwRowSize);
				return;
			}			
			
			switch(pResult->dwQueryUID)
			{
				case QUERY_TYPE_USER_INFO:		QueryTypeUserInfo(pResult);						break;
				case QUERY_TYPE_GM_CHECK:		QueryTypeGMCheck(pResult);						break;				
				case QUERY_TYPE_MESSAGE:		QueryTypeMessage(pResult, QUERY_TYPE_MESSAGE);	break;	
				case QUERY_TYPE_MESSAGE2:		QueryTypeMessage(pResult, QUERY_TYPE_MESSAGE2);	break;
				case QUERY_TYPE_MESSAGERQ:		QueryTypeMessageRQ(pResult);					break;			
				case QUERY_TYPE_GUILD_CHK:		QueryGuilChk(pResult);							break;			
				case QUERY_GUILD_ID:			QueryGuildId(pResult);							break;
				case QUERY_TYPE_MESSENGER:		QueryMessenger(pResult);						break;
				case QUERY_TYPE_CHARACTERINFO:	QueryTypeCharacterInfo(pResult);				break;
				case QUERY_TYPE_WORLDCHK:		QueryTypeWorldChk(pResult);						break;
				case QUERY_TYPE_OWNER:			__asm int 3;									break;
				case QUERY_TYPE_PARTY_FRIEND:	QueryTypePartyFriend(pResult);					break;
				case QUERY_TYPE_LADDER_INFO:	QueryTypeLadderInfo( pResult );					break;
			}
		}
		break;		
		
		case QUERY_TYPE_EXECUTE: 			break;
		case QUERY_TYPE_EXECUTE_BY_PARAM:	break;
	}	

	END_FUNCTION_PROFILE();
}


void __stdcall FailToConnectLoginAgentServer(void* pExt)
{
	pExt;
	START_FUNCTION_PROFILE();

	Log(LOG_IMPORTANT, "Fail To Connect LoginAgent Server! (IP:%s, Port:%d)", g_pThis->GetIPForAgentServer(), g_pThis->GetPortForAgentServer() );

	END_FUNCTION_PROFILE();
}


void __stdcall SuccessToConnectLoginAgentServer(DWORD dwConnectionIndex, void* pExt)
{
	pExt;
	START_FUNCTION_PROFILE();

	Log(LOG_JUST_DISPLAY, "@ LoginAgent Server Connect Successfully!");
	
	g_pThis->SetAgentConnectionIndex(dwConnectionIndex);

	WSTAS_SERVER_ATTACH	packet;
	packet.dwWorldServerID = g_pThis->GetServerID();
	g_pNet->SendToServer(AGENTINDEX, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	
	ReportToCMListener(1, &g_MachineName);

	END_FUNCTION_PROFILE();
}


void __stdcall OnAcceptUser(DWORD dwConnectionIndex)
{
	START_FUNCTION_PROFILE();

	if(!g_pThis->IsUserAcceptAllowed())
		g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);

	END_FUNCTION_PROFILE();
}

void __stdcall OnAcceptServer(DWORD dwConnectionIndex)
{
	dwConnectionIndex;
}

void __stdcall OnDisconnectServer(DWORD dwConnectionIndex)
{
	if(!g_bIsServerRunning)	return;

	START_FUNCTION_PROFILE();

	SERVER_DATA* pServer = (SERVER_DATA*)g_pNet->GetServerInfo(dwConnectionIndex);

	if(pServer)
	{
		char* szServerType = NULL;

		switch(pServer->bServerType)
		{
			case SERVER_TYPE_VILLAGE:		szServerType = "Village Server";			break;
			case SERVER_TYPE_DUNGEON:		szServerType = "Dungeon Server";			break;
			case SERVER_TYPE_LOGINAGENT:	
				{
					szServerType = "LoginAgent Server";		
					// ShutDown
					SetEvent(g_pThis->m_hKeyEvent[6]);		
				}
				break;
			default:	Log(LOG_IMPORTANT, "Invalid Type Server Disconnected!");		break;
		}

		Log(LOG_FATAL, "%s Disconnected! (ID=%d)", szServerType, pServer->dwID);

		g_StadiumMgr.RemoveStadium( pServer->dwID );//맵 떨어질때 운동장 정보 삭제: hwoarang 050202

		pServer->dwServerStatus = SERVER_STATUS_NOT_CONNECTED;
		g_pNet->SetServerInfo(dwConnectionIndex, NULL);
	}
	else
	{
		char szIP[16]={0,};	WORD wPort=0;
		g_pNet->GetServerAddress(dwConnectionIndex, szIP, &wPort);

		Log(LOG_NORMAL, "Unattached Server Disconnected! (IP=%s)", szIP);
	}

	END_FUNCTION_PROFILE();
}


void __stdcall OnDisconnectUser(DWORD dwConnectionIndex)
{
	if(!g_bIsServerRunning)	return;

	START_FUNCTION_PROFILE();
	
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	if(pUser)
	{
		// 파티매칭 대기자에서 삭제
		CmdPMatchEscape(pUser, NULL, 0);

		g_pNtos->RemoveTimer( &(pUser->m_NTOS));

		// 일반 유저들의 종료 처리.
		if(strlen(pUser->m_szCharacterName))
		{
			// 던전이 갱신되었을때 보내져야될 던전 리스트. 유저 지워질때 던전 리스트도 전부 지워지므로..여기서 지울필요 없다.
			if (pUser->m_pListRefreshFromDungeon)
			{
				POSITION_ posDungeon = pUser->m_pListRefreshFromDungeon->GetHeadPosition();
				
				while(posDungeon)
				{					
					POSITION_ delPosDungeon = posDungeon;

					DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)pUser->m_pListRefreshFromDungeon->GetNext(posDungeon);
					POSITION_ posUser = pDungeon->m_pListSendToUser->GetHeadPosition();

					while(posUser)
					{	
						// 던전 갱신되었을때 보내져야될 유저리스트에서 유저를 찾아 없애라.
						POSITION_ delPosUser = posUser;
						CWorldUser* pWorldUser = (CWorldUser*)pDungeon->m_pListSendToUser->GetNext(posUser);
						
						if (pWorldUser == pUser)
						{
							pDungeon->m_pListSendToUser->RemoveAt(delPosUser);
							pUser->m_pListRefreshFromDungeon->RemoveAt(delPosDungeon);
							break;
						}
					}			
				}
			}	

			Recv_CTWS_TM_REMOVE(pUser);//: hwoarang 050202

			__GuildDisconnectUser(pUser);			
			__PartyDisconnectUser(pUser);
			__MessengerDisconnectUser(pUser);
			__SaveCharacter(pUser);

			if(pUser->m_ListPosInWorld)
			{
				if(pUser->m_dwCurServerPos != POS_SERVER_WORLD)
				{
					Log(LOG_IMPORTANT, "WorldUser Info is not matched!(Name:%s)", pUser->m_szCharacterName);
				}

				g_pUserTable->m_pUserListInWorld->RemoveAt(pUser->m_ListPosInWorld);
				pUser->m_ListPosInWorld = NULL;
				
				Log(LOG_JUST_FILEWRITE, "WorldUserList Removed! in OnDisconnectUser() (Name:%s)", pUser->m_szCharacterName);
			}
			
		}	// 여기까지는 캐릭터 선택한 유저만....
		
		if(pUser->m_dwUserIndex)		
		{
			// 정상 종료인 유저만 에이전트에 Disconnect 되었다고 정보 날리고 
//			if(!pUser->m_bKicked)	//Kick 을 하는 유저도 LoginTable에서 지워 줘야 한다: 050116 hwoarang
//			{
				WSTAS_WORLD_USER_DISCONNECT packet;
				packet.dwPropID = pUser->m_dwPropID;
				g_pNet->SendToServer(AGENTINDEX, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
//			}
			
			// 던전서버에서도 확실하게 끊어버리자 
			WSTDS_KICK_USER	KickPacket;
			KickPacket.dwCharIndex = pUser->m_dwUserIndex;
			g_pServerTable->BroadCast((char*)&KickPacket, KickPacket.GetPacketSize());

			// 이미 AcceptTable에 추가되어있는 IP라면 다 지워버리고 
			ACCEPT_USER* pAcceptUser = (ACCEPT_USER*)0xffffffff;

			while(pAcceptUser)
			{
				pAcceptUser = g_pAcceptTable->GetAcceptInfo(pUser->m_dwUserIndex);

				if(pAcceptUser)
				{
					g_pAcceptTable->Remove(pAcceptUser);
					Log(LOG_IMPORTANT, "Accepted User Remained! (OnDisconnectUser Function!)");
				}
			}

			g_pUserTable->Remove(pUser);

#ifdef CHECK_KEEP_ALIVE
			g_pCDTable->Remove(pUser);
#endif
		}
		else
		{
			Log(LOG_FATAL, "Invalid User Disconnected!");			
		}		
	}

	g_pNet->SetUserInfo(dwConnectionIndex, NULL);

	END_FUNCTION_PROFILE();
}

void __SaveCharacter(CWorldUser* pUser)
{
	WORD		nColumn			= 2;
	char		szQuery[1024]	= {0,};
	DWORD		dwParamOffset	= 0;
	SKILL_KEY	sSkillKey;	

	DBBINDING*	pBinding = g_pDb->THCreateParamInfo(nColumn);

	if(!pBinding)	
		MessageBox(NULL,"Failed THCreateParamInfo",NULL,NULL);

	for(int i = 0; i < 16; i++)
	{
		sSkillKey.bySkillKey[i] = pUser->m_bySkillKey[i];						
		sSkillKey.byPosKey[i]	= pUser->m_byPosKey[i];
	}
	
	pBinding[0].obValue		=	dwParamOffset;
	pBinding[0].cbMaxLen	=	sizeof(sSkillKey.bySkillKey);
	pBinding[0].wType		=	DBTYPE_BYTES;
	dwParamOffset			+=	sizeof(sSkillKey.bySkillKey);

	pBinding[1].obValue	=	dwParamOffset;
	pBinding[1].cbMaxLen	=	sizeof(sSkillKey.byPosKey);
	pBinding[1].wType		=	DBTYPE_BYTES;
	dwParamOffset			+=	sizeof(sSkillKey.byPosKey);	

	wsprintf(szQuery, "Update CharacterInfo Set SkillKey=?, PosKey=?, HelpTypeA=%u, HelpTypeB=%u, HelpInterface=%u where [Name] = '%s'", pUser->m_byHelp[0], pUser->m_byHelp[1], pUser->m_byHelp[2], pUser->m_szCharacterName);
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, &sSkillKey, dwParamOffset, (BYTE)nColumn, FALSE, QUERY_TYPE_SKILLKEY, pUser, (BYTE)GAME_DB);// 쿼리 결과 받을 이유없어 수정. by deepdark.

	// Ladder 기록
	int nPlayTime	= GetPlayTime(g_dwCurTick, pUser->m_dwStartTick, ONLY_SEC);
	int nHour		= GetPlayTime(g_dwCurTick, pUser->m_dwStartTick, TIME_HOUR);
	int nDay		= nHour / 24;

	wsprintf(szQuery, "CZP_UPDATE_LADDER '%s', %u, %u, %u, %u", pUser->m_szCharacterName, nPlayTime, nDay, g_pThis->GetServerID(), pUser->m_dwDungeonID);
	g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pUser, (BYTE)GAME_DB); // 쿼리 결과 받을 이유없어 수정. by deepdark.
	
	// 지우기 				
	wsprintf(szQuery, "CZP_USER_LOGOUT '%s', %d, %d", pUser->m_szCharacterName, g_pThis->GetServerID(), pUser->m_dwDungeonID);
	g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);
}

void SaveCharacterByBlockMode(CWorldUser* pUser)
{
	WORD	nColumn			= 2;
	char	szQuery[1024]	= {0,};

	DBBINDING*	pBinding = g_pDb->CreateParamInfo(nColumn);

	if(!pBinding)	
		MessageBox(NULL,"Failed CreateParamInfo",NULL,NULL);

	DWORD		dwParamOffset = 0;
	SKILL_KEY	sSkillKey;		
	
	for(int i = 0; i < 16; i++)
	{
		sSkillKey.bySkillKey[i] = pUser->m_bySkillKey[i];						
		sSkillKey.byPosKey[i]	= pUser->m_byPosKey[i];
	}
	
	pBinding[0].obValue		=	dwParamOffset;
	pBinding[0].cbMaxLen	=	sizeof(sSkillKey.bySkillKey);
	pBinding[0].wType		=	DBTYPE_BYTES;
	dwParamOffset			+=	sizeof(sSkillKey.bySkillKey);

	pBinding[1].obValue		=	dwParamOffset;
	pBinding[1].cbMaxLen	=	sizeof(sSkillKey.byPosKey);
	pBinding[1].wType		=	DBTYPE_BYTES;
	dwParamOffset			+=	sizeof(sSkillKey.byPosKey);	

	wsprintf(szQuery, "Update CharacterInfo Set SkillKey=?, PosKey=?, HelpTypeA=%u, HelpTypeB=%u, HelpInterface=%u where [Name] = '%s'", pUser->m_byHelp[0], pUser->m_byHelp[1], pUser->m_byHelp[2], pUser->m_szCharacterName);
	int nRet = g_pDb->ExecuteSQLByParam(szQuery, pBinding, &sSkillKey, (BYTE)nColumn, (BYTE)GAME_DB);
	if(nRet < 0)
	{
		Log(LOG_NORMAL, "Save Character Failed! (Name:%d)", pUser->m_szCharacterName);
	}

	g_pDb->ReleaseParamInfo(pBinding, (BYTE)GAME_DB);

	// Ladder 기록
	int nPlayTime	= GetPlayTime(g_dwCurTick, pUser->m_dwStartTick, ONLY_SEC);
	int nHour		= GetPlayTime(g_dwCurTick, pUser->m_dwStartTick, TIME_HOUR);
	int nDay		= nHour / 24;

	wsprintf(szQuery, "CZP_UPDATE_LADDER '%s', %u, %u, %u, %u", pUser->m_szCharacterName, nPlayTime, nDay, g_pThis->GetServerID(), pUser->m_dwDungeonID);
	g_pDb->ExecuteSQL(szQuery, (BYTE)GAME_DB);	

	// 지우기
	wsprintf(szQuery, "CZP_USER_LOGOUT '%s', %d, %d", pUser->m_szCharacterName, g_pThis->GetServerID(), pUser->m_dwDungeonID);
	g_pDb->ExecuteSQL(szQuery, (BYTE)GAME_DB);
}


void __MessengerDisconnectUser(CWorldUser* pUser)
{
	LP_MESSENGER_TABLE		lpMessengerTable = g_pMessengerTableHash->GetData(pUser->m_dwUserIndex);
	WSTC_MESSENGER_USERINFO	pOnlinePacket;

	if( NULL == lpMessengerTable)
	{
		return;	
	}
	
	ListNode<MESSENGER_USER>* lpMessengerUserPos =  lpMessengerTable->pMessengerUserHash->GetHead();

	while(lpMessengerUserPos)
	{
		LP_MESSENGER_USER lpMessengerUser = lpMessengerUserPos->pData;

		if(NULL == lpMessengerUser)
		{
			lpMessengerUserPos = lpMessengerUserPos->pNext;
			continue;
		}

		if(lpMessengerUser->byType != 0)
		{
			lpMessengerUserPos = lpMessengerUserPos->pNext;
			continue;			
		}
		
		CWorldUser* pOnlineUser = g_pUserTable->GetUserInfo(lpMessengerUser->dwUserIndex);

		if(NULL == pOnlineUser)
		{
			lpMessengerUserPos = lpMessengerUserPos->pNext;
			continue;
		}

		LP_MESSENGER_TABLE lpMessengerSubTable = g_pMessengerTableHash->GetData(pOnlineUser->m_dwUserIndex);

		// 친구 오프라인 표시 안되는 버그 수정 김영대 2005.02.06	
		if(NULL == lpMessengerSubTable)
		{
			lpMessengerUserPos = lpMessengerUserPos->pNext;
			continue;
		}

		LP_MESSENGER_USER lpMessengerSubUser = lpMessengerSubTable->pMessengerUserHash->GetData(pUser->m_dwUserIndex);

		if(NULL == lpMessengerSubUser)
		{
			lpMessengerUserPos = lpMessengerUserPos->pNext;
			continue;
		}

		pOnlinePacket.byType		= 1;
		pOnlinePacket.dwUserIndex	= pUser->m_dwUserIndex;
		g_pNet->SendToUser(pOnlineUser->m_dwConnectionIndex, (char*)&pOnlinePacket, pOnlinePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		lpMessengerUserPos = lpMessengerUserPos->pNext;
	}
	
	// 메세지 보내기 
	g_pMessengerTableHash->Delete(lpMessengerTable, pUser->m_dwUserIndex, TRUE);
}

void __PartyDisconnectUser(CWorldUser* pUser)
{
	if(pUser->m_dwPartyId != 0)
	{			
		LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);
		
		if(!lpPartyTable || !(lpPartyTable->pPartyUserList))
			return;
			
		if(lpPartyTable->bCount > 2)
		{						
			POSITION_ pos		= lpPartyTable->pPartyUserList->GetHeadPosition();
			POSITION_ posTemp	= 0;

			while(pos)
			{			
				posTemp = pos;

				CWorldUser* pPartyUser = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				// 자기 자신일 경우 //
				if(pPartyUser->m_dwUserIndex==pUser->m_dwUserIndex)
				{
					POSITION_ posDst = lpPartyTable->pPartyUserList->GetHeadPosition();

					while(posDst)
					{
						CWorldUser*	pUserRv	= (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(posDst);

						if(!pUserRv)								
							continue;								

						if(pUserRv->m_dwUserIndex!=pUser->m_dwUserIndex)
						{																			
							// 메세지 보내기 //
							WSTC_PARTY_LOGOUT pSendLogout;
							memset(pSendLogout.szName, 0, sizeof(pSendLogout.szName));
							__lstrcpyn(pSendLogout.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);									
							pSendLogout.bResultCode	= 0;
							pSendLogout.dwUserIndex	= pUser->m_dwUserIndex;
							g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pSendLogout, pSendLogout.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}
					}
				
					if(pPartyUser->m_byLeader==1)
					{								
						pPartyUser->m_byLeader	= 0;																
						lpPartyTable->pPartyUserList->RemoveAt(posTemp);
						lpPartyTable->bCount	-= 1;									

						// 새로운 리더를 보내준다 
						POSITION_ posLeader = lpPartyTable->pPartyUserList->GetHeadPosition();

						BYTE	byLeader		= 0;
						DWORD	dwUserIndeex	= 0;
						char	szCharName[MAX_CHARACTER_NAME_LENGTH]={0,};

						memset(szCharName, 0, sizeof(szCharName));

						while(posLeader)
						{
							CWorldUser* pUserLeader = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(posLeader);
							
							if(!pUserLeader)
								continue;

							if(byLeader==0)
							{
								pUserLeader->m_byLeader = 1;
								byLeader		= 1;
								dwUserIndeex	= pUserLeader->m_dwUserIndex;
								__lstrcpyn(szCharName, pUserLeader->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
								
								LP_PARTYBOARD_TABLE lpPartyTable = 
									g_pPartyBoardTableHash->GetData(pUserLeader->m_dwPartyId);

								if(lpPartyTable)
									lpPartyTable->dwPartyUserLeader	= pUserLeader->m_dwUserIndex;
								
								DUNGEON_DATA_EX* pDungeon = NULL;
								pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pPartyUser->m_dwDungeonID);	
							
								if(pDungeon)
								{
									WSTDS_PARTY_LEADER pLeaderDungeonPacket;
									pLeaderDungeonPacket.dwUserIndex	= pPartyUser->m_dwUserIndex;
									pLeaderDungeonPacket.byLeader		= 0;
									g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pLeaderDungeonPacket, pLeaderDungeonPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
								}
								
								pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUserLeader->m_dwDungeonID);	
							
								if(pDungeon)
								{
									WSTDS_PARTY_LEADER pLeaderDungeonPacket;
									pLeaderDungeonPacket.dwUserIndex	= pUserLeader->m_dwUserIndex;
									pLeaderDungeonPacket.byLeader		= 1;
									g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pLeaderDungeonPacket, pLeaderDungeonPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
								}								
							}																																	
							
							WSTC_PARTY_LEADER pLeaderPackect;
							memset(pLeaderPackect.szCharName, 0, sizeof(pLeaderPackect.szCharName));
							__lstrcpyn(pLeaderPackect.szCharName, szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
							pLeaderPackect.dwUserIndex	= dwUserIndeex;

							g_pNet->SendToUser(pUserLeader->m_dwConnectionIndex, (char*)&pLeaderPackect, pLeaderPackect.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

							// 매칭 대기자에서 뺀다
							CmdPMatchEscape(pUserLeader, NULL, 0);
						}														
					}
					else
					{								
						pPartyUser->m_byLeader	= 0;																								
						lpPartyTable->pPartyUserList->RemoveAt(posTemp);
						lpPartyTable->bCount	-= 1;									
					}
					
					break;
				}					
			}
		}
		else
		{
			// 파티 삭제 			
			POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();
			POSITION_ posTemp;

			DWORD dwPartyId = pUser->m_dwPartyId;

			while(pos)
			{			
				posTemp = pos;

				CWorldUser*	pUserRv	= (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);						

				if(!pUserRv)						
					continue;																		

				pUserRv->m_dwPartyId = 0;

				if(pUser->m_dwUserIndex!=pUserRv->m_dwUserIndex)
				{
					DUNGEON_DATA_EX* pDungeon;
					pDungeon = g_pDungeonTable->GetDungeonInfo( (WORD)pUserRv->m_dwDungeonID );	

					if(pDungeon)
					{
						WSTDS_PARTY_MSG	pPartyMsg;								
						pPartyMsg.dwPartyId		= 0;
						pPartyMsg.dwUserIndex	= pUserRv->m_dwUserIndex;								
						g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPartyMsg, pPartyMsg.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
																					
					// 메세지 보내기 
					WSTC_PARTY_LOGOUT pSendLogout;
					memset(pSendLogout.szName, 0, sizeof(pSendLogout.szName));
					__lstrcpyn(pSendLogout.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					pSendLogout.bResultCode	= 1;
					pSendLogout.dwUserIndex	= pUser->m_dwUserIndex;
					g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pSendLogout, pSendLogout.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}

				// 매칭 대기자에서 뺀다
				CmdPMatchEscape(pUserRv, NULL, 0);
			}

			// Hash 에서 삭제 
			g_pPartyTableHash->Delete(lpPartyTable, dwPartyId, TRUE);

			LP_PARTYBOARD_TABLE lpPartyTable = g_pPartyBoardTableHash->GetData(dwPartyId);
		
			if(lpPartyTable)
			{
				WSTDS_PARTY_BOARD pPartyBoardPacket;
				pPartyBoardPacket.dwPartyId	= dwPartyId;

				POSITION_ pos = g_pDungeonTable->m_pDungeonVillList->GetHeadPosition();
				
				while(pos)
				{
					DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pDungeonVillList->GetNext(pos);

					if(pDungeon && pDungeon->m_pServer)
					{
						if(pDungeon->IsVilliage() )//마을이라면: hwoarang 050202
						{
							pPartyBoardPacket.wDungeonID = (WORD)pDungeon->m_dwID;
							g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPartyBoardPacket, pPartyBoardPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}
					}
				}

				g_pPartyBoardTableHash->Delete(lpPartyTable, lpPartyTable->dwPartyId);
			}
		}
	}
}

void __GuildDisconnectUser(CWorldUser* pUser)
{
	if(pUser->m_dwGuildId != 0)
	{
		LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);

		if(lpGuildTable)
		{	
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);
				
			LP_GUILD_OFFLINE_USER lpGuildOfflineTable = (LP_GUILD_OFFLINE_USER)LALAlloc(g_pGuildOfflinePool);
			memset(lpGuildOfflineTable, 0, sizeof(lpGuildOfflineTable));
			lpGuildOfflineTable->byRank			= pUser->m_byRank;
			lpGuildOfflineTable->dwUserIndex	= pUser->m_dwUserIndex;
			lpGuildOfflineTable->wClass			= pUser->m_wClass;
			lpGuildOfflineTable->dwLevel		= pUser->m_dwLevel;			
			lpGuildOfflineTable->sDate.day		= SystemTime.wDay;
			lpGuildOfflineTable->sDate.hour		= SystemTime.wHour;
			lpGuildOfflineTable->sDate.minute	= SystemTime.wMinute;
			lpGuildOfflineTable->sDate.month	= SystemTime.wMonth;
			lpGuildOfflineTable->sDate.second	= SystemTime.wSecond;
			lpGuildOfflineTable->sDate.year		= SystemTime.wYear;					
						  
			__lstrcpyn(lpGuildOfflineTable->szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			__lstrcpyn(lpGuildOfflineTable->szNickName, pUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);			
			lpGuildTable->pGuildOfflineHash->Insert(lpGuildOfflineTable, lpGuildOfflineTable->dwUserIndex);

			ListNode<CWorldUser>* pGuildUserNodeRank = lpGuildTable->pGuildUserHash->GetHead();

			while(pGuildUserNodeRank)
			{
				CWorldUser* pGuildUserRank = pGuildUserNodeRank->pData;

				if(pGuildUserRank)
				{
					if(pGuildUserRank->m_byRank==1)
					{
						WSTC_GUILD_OFFLINEUSER pOffLinePacket;
						memset(pOffLinePacket.szName, 0, sizeof(pOffLinePacket.szName));
						memset(pOffLinePacket.szNickName, 0, sizeof(pOffLinePacket.szNickName));
						pOffLinePacket.byRank		= lpGuildOfflineTable->byRank;
						pOffLinePacket.dwLevel		= lpGuildOfflineTable->dwLevel;
						pOffLinePacket.dwUserIndex	= lpGuildOfflineTable->dwUserIndex;
						pOffLinePacket.wClass		= lpGuildOfflineTable->wClass;
						__lstrcpyn(pOffLinePacket.szName, lpGuildOfflineTable->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
						__lstrcpyn(pOffLinePacket.szNickName, lpGuildOfflineTable->szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);
						g_pNet->SendToUser(pGuildUserRank->m_dwConnectionIndex, (char*)&pOffLinePacket, pOffLinePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
				}

				pGuildUserNodeRank = pGuildUserNodeRank->pNext;
			}

			ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
									
			while(lpGuildUserNode)
			{
				CWorldUser* pGuildUser = lpGuildUserNode->pData;
				
				if(!pGuildUser)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}

				if(pGuildUser->m_dwUserIndex==pUser->m_dwUserIndex)
				{
					ListNode<CWorldUser>* lpGuildUserNodeSub = lpGuildTable->pGuildUserHash->GetHead();		

					while(lpGuildUserNodeSub)
					{
						CWorldUser* pGuildUserSub = lpGuildUserNodeSub->pData;
				
						if(!pGuildUserSub)
						{
							lpGuildUserNode = lpGuildUserNode->pNext;
							continue;
						}

						if(pGuildUserSub->m_dwUserIndex!=pUser->m_dwUserIndex)
						{								
							WSTC_GUILD_USERINFODEL	pPacketSend;								
							memset(pPacketSend.szCharacterName, 0, sizeof(pPacketSend.szCharacterName));							
							__lstrcpyn(pPacketSend.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							pPacketSend.dwUserIndex	= pUser->m_dwUserIndex;
							g_pNet->SendToUser(pGuildUserSub->m_dwConnectionIndex, (char*)&pPacketSend, pPacketSend.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}

						lpGuildUserNodeSub = lpGuildUserNodeSub->pNext;
					}
					
					lpGuildTable->pGuildUserHash->Delete(pUser, pUser->m_dwUserIndex, TRUE);
					break;
				}

				lpGuildUserNode = lpGuildUserNode->pNext;
			}
		}			
	}
}


void ReceivedFromDBLog(DBRECEIVEDATA* pResult)	//kjk(41123)
{
	if(!g_bIsServerRunning)	return;
}