#include "stdafx.h"
#include "MsgProcForServer.h"
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

#include "GuildMatcher.h"
#include "RivalGuildWar.h"

extern CGuildMatcher g_GuildMatcher;

//============================================================
// 유저가 던전에서 월드로 나왔다.
//============================================================

void SendGotoWorldError( CWorldUser* pUser , const BYTE byCause)
{
	//여기는 TeamMatch Stadium
	WSTC_GO_TO_WORLD_FAIL	FailPacket;
	FailPacket.bErrorCode = byCause;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	
	Log(LOG_IMPORTANT, "SendGotoWorldError[%d]",  byCause);
}
void CmdGotoWorld(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_GOTOWORLD* pPacket = (DSTWS_GOTOWORLD*)pMsg;
	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = g_pUserTable->GetUserInfo( pPacket->dwUserIndex );
	if (!pUser)	
	{
		Log(LOG_IMPORTANT, "User is not found at CmdGotoWorld!");
		return;
	}

	pUser->m_dwMoney		= pPacket->dwMoney;
	pUser->m_wCriminalTime	= pPacket->wCriminalTime;	
	
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo( pPacket->wToDungeonID );	

	if(!pDungeon)
	{
		SendGotoWorldError( pUser, GO_TO_WORLD_ERROR_DUNGEON_NO);//월드맵 이동 금지 : hwoarang 050202 
		return;
	} // if(!pDungeon)


	if( pDungeon && pDungeon->IsStadium() )//관전자도 나갈때는 월드로 못간다. 던전으로 이동 시켜 버려라
	{
		SendGotoWorldError( pUser, GO_TO_WORLD_ERROR_STADIUM);//월드맵 이동 금지 : hwoarang 050202 
		return;
	}
	
	VECTOR3 vDungeonPos, vStartPos;
	vDungeonPos.x = (float)pDungeon->m_dWorldPos_X;
	vDungeonPos.y = 0.0f;
	vDungeonPos.z = (float)pDungeon->m_dWorldPos_Z;

	BOOL bRet;
	if(pDungeon->m_dwID < 10)
	{
		bRet = FindEmptyPosNearDungeon((BYTE)pDungeon->m_wWorldMapID, &vDungeonPos, &vStartPos, TRUE);
	}
	else
	{
		bRet = FindEmptyPosNearDungeon((BYTE)pDungeon->m_wWorldMapID, &vDungeonPos, &vStartPos, FALSE);
	}

	if(!bRet)
	{
		SendGotoWorldError( pUser, GO_TO_WORLD_ERROR_DUNGEON_POS);//월드맵 이동 금지 : hwoarang 050202 
		return;
	}

	WSTC_MOVE_TO_WORLD	packet;
	packet.bWorldMapID		= (BYTE)pDungeon->m_wWorldMapID;		//대륙 번호 
	packet.wToDungeonID		= pPacket->wToDungeonID;
	packet.fStartPos_X		= vStartPos.x;
	packet.fStartPos_Z		= vStartPos.z;
	packet.wNowDungeonID	= pPacket->wNowDungeonID;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	
	// 현재 유저의 위치 갱신 
	pUser->m_v3CurWorldPos.x = vStartPos.x;
	pUser->m_v3CurWorldPos.y = 0.0f;
	pUser->m_v3CurWorldPos.z = vStartPos.z;
	pUser->SetCurDungeon(pPacket->wToDungeonID);


	if( pUser->IsHaveGMC() )//길드전시 월드로 가면 매칭 안됨 : hwoarang 050202
	{
		CPlayerInfo Player( pUser );
		g_GuildMatcher.Remove( pUser->m_GMC, Player );
	}
}


//=========================================================================
// 던젼에서 월드에게 유저접속명단에 승인되었다고 알려줌 
//=========================================================================
void CmdUserAcceptAllowed(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_USER_ACCEPT_ALLOWED*	pPacket = (DSTWS_USER_ACCEPT_ALLOWED*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);	
	
	if(pUser)
	{		
		pUser->m_dwStatus = USER_STATUS_NORMAL;

		// 던전서버에서도 확실하게 끊어버리자 
		WSTDS_KICK_USER	KickPacket;
		KickPacket.dwCharIndex = pUser->m_dwUserIndex;
		g_pServerTable->BroadCast((char*)&KickPacket, KickPacket.GetPacketSize());
			
		WSTC_CONNECT_DUNGEON_SERVER packet;
		packet.wDungeonID	= pPacket->wDungeonID;
		packet.dwDungeonIP	= pServer->dwIPForUser;
		packet.wPort		= pServer->wPort;
		packet.dwPartyID	= pUser->m_dwPartyId;
		
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);    

		if(!pUser->m_dwPartyId)	return;

		LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData( pUser->m_dwPartyId );
						
		if(lpPartyTable)
		{	
			 if(lpPartyTable->pPartyUserList)
			 {
				WSTC_PARTY_USER_TRY_TO_CONNECT	TryPacket;
				TryPacket.dwCharIndex = pUser->m_dwUserIndex;

				 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

				 while(pos)
				 {
					 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

					 if(pPartyUser->m_dwUserIndex == pUser->m_dwUserIndex || pPartyUser->m_dwCurServerPos != POS_SERVER_WORLD)
					 {
						 // 본인이거나 월드에 없는 유저면 Skip
						 continue;	
					 }
					 
					 g_pNet->SendToUser( pPartyUser->m_dwConnectionIndex, (char*)&TryPacket, TryPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				 }
			 }
		}
	}
	else	
	{
		// 던전서버에게 승인 받는 사이에 유저가 접속을 끊은경우( 이곳으로 진입하는경우는 정말로 거의 없을것이다..ㅡ_-;;)
		// 이경우 다시 해당 던전서버에게 Aceptt 허용 테이블에 있는 그 유저를 삭제하라는 메세지를 보낸다. 

		WSTDS_REMOVE_USER_ACCEPT_ALLOWED packet;
		packet.dwUserIndex	= pPacket->dwUserIndex;
		packet.wDungeonID	= pPacket->wDungeonID;
		g_pNet->SendToServer(pServer->dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		Log(LOG_NORMAL, "User Accepted But..   User Disconnect from WorldServer!");
	}
}


//=========================================================================
// 던젼에서 월드에게 유저접속명단에 승인 실패 하였다고 알림  
//=========================================================================
void CmdUserAcceptFailed(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_USER_ACCEPT_FAILED*	pPacket = (DSTWS_USER_ACCEPT_FAILED*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);
	
	if(pUser)
	{
		WSTC_DUNGEON_JOIN_FAILED	packet;
		packet.bFailCode = DUNGEON_JOIN_FAIL_USER_ACCEPT_FAIL;
		packet.wDungeonID = pPacket->wDungeonID;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}	
}


//===========================================================================================
// 던젼에서 월드에게 Dungeon Join하기전에 같은 인덱스의 유저가 있는지 확인 
//===========================================================================================
void CmdCheckDungeonJoin(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_CHECK_DUNGEONJOIN*	pServerPacket = (DSTWS_CHECK_DUNGEONJOIN*)pMsg;
	WSTDS_CHECK_DUNGEONJOIN_RT	pPacket;
	
	memset(pPacket.szCharacerName, 0, sizeof(pPacket.szCharacerName));
	__lstrcpyn(pPacket.szCharacerName, pServerPacket->szCharacerName, MAX_CHARACTER_NAME_REAL_LENGTH + 1);	
	pPacket.dwUserIndex			= pServerPacket->dwUserIndex;
	pPacket.dwConnectionIndex	= pServerPacket->dwConnectionIndex;
	pPacket.byChk				= 0;
	
	CWorldUser* pUser = g_pUserTable->GetUserInfo(pServerPacket->dwUserIndex);

	if(pUser)
	{
		if(__stricmp(pUser->m_szCharacterName, pServerPacket->szCharacerName)==0)		
			pPacket.byChk = 1;
		
		if(pUser->m_dwDungeonID)
			pPacket.byChk = 2;
		
		if(pUser->m_byPortal==1)
		{
			if(__stricmp(pUser->m_szCharacterName, pServerPacket->szCharacerName)==0)		
			{
				pPacket.byChk = 1;
			}
			else
			{
				pPacket.byChk = 0;
			}
			
			pUser->m_byPortal	= 0;
		}
	}

	if( IS_ABLE_SERVICE_TYPE( ST_CHINA_BILLING ) )
	{

		if (!pUser)	return;

		Log(LOG_IMPORTANT, "<BILLING INFO > (In Check Dungeon) : type(%d), point(%d)", pUser->m_sChinaBillingInfo.eType, pUser->m_sChinaBillingInfo.dwPoint);
		
		// Billing Check //
		if (pUser->m_sChinaBillingInfo.eType != enum_level_approval)
		{
			if(pUser->m_dwLevel >= CHINA_BILLING_FREE_LEVEL_LIMIT)
			{			
				// 결제가 안되어있으니 다시 체크하라고 메시지 보낸다.
				WSTC_CB_MESSAGE pCBMessagePacket;
				pCBMessagePacket.byMessageType = 2;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pCBMessagePacket, pCBMessagePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

				WSTAS_REQUEST_CHINA_BILLING pPacket;
				pPacket.dwPropID	= pUser->m_dwPropID;
				pPacket.dwUserIndex	= pUser->m_dwUserIndex;
				g_pNet->SendToServer(AGENTINDEX, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			

				return;
			}
		}

		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pServerPacket->wDungeonID);

		if(pDungeon)
		{
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}

	}
	else
	{
		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pServerPacket->wDungeonID);
		if(pDungeon)
		{
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}
}

//=========================================================================
// 던젼에서 월드에게 Dungeon Join에 성공했다고 알림 
//=========================================================================
void CmdDungeonJoinSuccess(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_DUNGEON_JOIN_SUCCESS *pPacket = (DSTWS_DUNGEON_JOIN_SUCCESS*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwChar_Index);

	if(pUser)
	{	
		pUser->m_dwCurServerPos	= POS_SERVER_DUNGEON;		
		pUser->SetCurDungeon(pPacket->wDungeonID);
		
		if(pUser->m_ListPosInWorld)
		{
			g_pUserTable->m_pUserListInWorld->RemoveAt(pUser->m_ListPosInWorld);
			pUser->m_ListPosInWorld = NULL;

			Log(LOG_JUST_FILEWRITE, "WorldUserList Removed! in CmdDungeonJoinSuccess() (Name:%s)", pUser->m_szCharacterName);
		}

		if( pUser->IsHaveGMC() )//길드전시 던전에 조인되어야만 매칭에 등록됨 : hwoarang 050202
		{//이건 월드갔다 오는 애들 이고
			CPlayerInfo Player( pUser );
			g_GuildMatcher.Insert( pUser->m_GMC, Player );
		} // if( pUser->IsHaveGMC() )//길드전시 던전에 조인되어야만 매칭에 등록됨 : hwoarang 050202

		if( CheckHaveTeamMatch(pUser) )// : hwoarang 050220 길드전 중이었냐?
		{
			return;
		}

		// 월드에 있는 파티원에게 대상이 던전으로 갔다("월드에서 나갔다")라고 알림
		LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData( pUser->m_dwPartyId );
						
		if(lpPartyTable)
		{	
			 if(lpPartyTable->pPartyUserList)
			 {
				 WSTC_WORLD_DISAPPEAR	DisappearPacket;
				 DisappearPacket.dwCharIndex = pUser->m_dwUserIndex;
				 				 
				 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

				 while(pos)
				 {
					 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

					 if( pPartyUser->m_dwUserIndex==pUser->m_dwUserIndex || pPartyUser->m_dwCurServerPos != POS_SERVER_WORLD)
					 {
						 //본인이거나 월드에 없는 유저면 Skip
						 continue;	
					 }

					 g_pNet->SendToUser( pPartyUser->m_dwConnectionIndex, (char*)&DisappearPacket, DisappearPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				 }
			 }
		}

		//////////////////////
		// 파티 관련 액션 수행
		if(!pUser->m_dwPartyId)
			return;

		// 던전에게 파티의 설정 정보를 보냄
		WSTDS_PARTY_CONFIG config;
		config.dwUserIndex = pUser->m_dwUserIndex;
		config.pConfig = pUser->m_PartyConfig;
		config.bMsg = 0;
		g_pNet->SendToServer(
			g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID)->m_pServer->dwConnectionIndex,
			(char*)&config, 
			config.GetPacketSize(), 
			FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		// 던전에서 유저가 성공적으로 던전에 조인했다는 메세지를 날렸는데 유저해쉬에는 유저가 없다.. ㅡㅡ 
		Log(LOG_NORMAL, "User is not found! at CmdDungeonJoinSuccess()");
	}
}


//=========================================================================
// 던젼에서 월드에게 커넥트 성공 했다고 알린다.
//=========================================================================
void CmdServerAttach(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSTWS_SERVER_ATTACH *pPacket = (DSTWS_SERVER_ATTACH*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}
	
	SERVER_DATA* pServer = g_pServerTable->GetServerInfo(pPacket->dwID);

	if(!pServer)
	{
		Log(LOG_IMPORTANT, "Server That has Invalid ID Try to Login!(ID=%d)", pPacket->dwID);
		return;
	}
	
	if(pServer->dwServerStatus == SERVER_STATUS_CONNECTED)
	{
		Log(LOG_IMPORTANT, "Server is already attached!(ID=%d)", pPacket->dwID);
	}
	
	char szIP[ MAX_IP_LENGTH ]={0,};	WORD wPort=0;
	g_pNet->GetServerAddress(dwConnectionIndex, szIP, &wPort);

	// DB에 등록된 서버 IP와 다른 경우 
	if(__strcmp(pServer->szIPForServer, szIP))		
	{
		Log(LOG_IMPORTANT, "Invalid Server IP Address Connected! Check ServerTable For System DB!(%s)", szIP);
		return;
	}
		
	pServer->dwConnectionIndex	= dwConnectionIndex;
	pServer->dwServerStatus		= SERVER_STATUS_CONNECTED;
	g_pNet->SetServerInfo(dwConnectionIndex, pServer);
	
	char* szServerType = NULL;

	switch(pServer->bServerType)
	{
		case SERVER_TYPE_VILLAGE:		szServerType = "Village Server";		break;
		case SERVER_TYPE_DUNGEON:		szServerType = "Dungeon Server";		break;
		case SERVER_TYPE_LOGINAGENT:	szServerType = "LoginAgent Server";		break;
		default: Log(LOG_IMPORTANT, "Invalid Server Type Entered At CmdServerAttach()~!"); return;		
	}

	// 결과값을 보내준다.
	Log(LOG_JUST_DISPLAY, "@ %s Attached! (ID=%d, IP=%s)",szServerType, pServer->dwID, szIP);

	WSTS_SERVER_ATTACH_RESULT packet;
	packet.bAdultMode			= g_pThis->m_bAdultMode;
	packet.dwEXPGAIN			= g_pThis->m_dwGLOBAL_EXP_GAIN * 100;
	packet.dwMagicDropRate		= g_pThis->m_GLOBAL_MAGIC_FIND_PBT * 100;
	packet.dwPartyBonusEXPGAIN	= g_pThis->m_dwGLOBAL_PARTY_BONUS_EXP_GAIN * 100;

	g_pNet->SendToServer(dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);	
				
	DUNGEON_DATA_EX* cur	= NULL;
	DUNGEON_DATA_EX* next	= NULL;

	for(DWORD i = 0; i < g_pDungeonTable->m_dwMaxBucketNum; i++)
	{
		cur = g_pDungeonTable->m_ppInfoTable[i];

		while (cur)
		{
			next = cur->m_pNextData;

			if (cur->m_dwPort == pServer->wPort)
			{
				WSTS_DUNGEON_INFO packet;
				packet.wDungeonID		= (WORD)cur->m_dwID;
				packet.bSiege			= cur->m_bSiege;
				packet.wUpgradeLevel	= cur->m_wUpgradeLevel;
				
				g_pNet->SendToServer(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				
				// 이벤트 던젼 입장객 초기화 
				cur->m_iEventEntered	= 0;
			}

			cur = next; 
		}		
	}
	
#ifdef GUILD_WAR
	// Dungeon Server GuildWar Info Send 
	ListNode<SGUILD_LIST>* lpGuildWarPos =  g_pGuildWarHash->GetHead();

	while(lpGuildWarPos)
	{
		LP_GUILD_LIST lpGuildList = (LP_GUILD_LIST)lpGuildWarPos->pData;

		if(lpGuildList)
		{
			for(int i = 0; i < MAX_GUILD_WAR; i++)
			{
				if(lpGuildList->sGuildNode[i].dwGuildId!=0)
				{
					WSTDS_GUILD_LIST pServerPacket;
					pServerPacket.dwGuildId		= lpGuildList->dwGuildId;
					pServerPacket.dwDstGuildId	= lpGuildList->sGuildNode[i].dwGuildId;
					g_pNet->SendToServer(dwConnectionIndex, (char*)&pServerPacket, pServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}

		lpGuildWarPos = lpGuildWarPos->pNext;
	}
	
	POSITION_ pos = g_pRankList->GetHeadPosition();

	if(pos)
	{
		LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(pos);

		if(lpGuildWarRank)
		{
			WSTDS_GUILD_RANK pRankPacket;
			pRankPacket.dwGuildId = lpGuildWarRank->dwGuildId;
			g_pNet->SendToServer(dwConnectionIndex, (char*)&pRankPacket, pRankPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		}
	}
#endif	
}


void CmdDungeonMagicArrayItem(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_DUNGEON_MAGICARRAY_ATTACH* pPacket = (DSTWS_DUNGEON_MAGICARRAY_ATTACH*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (!pDungeon)	return;
		
	memcpy(&pDungeon->m_cMagicFieldArrayItem, &pPacket->cItem, sizeof(CItem));
	
	WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY packet;
	packet.wDungeonID = (WORD)pDungeon->m_dwID;
	memcpy(&packet.cMagicFieldArrayItem, &pDungeon->m_cMagicFieldArrayItem, sizeof(CItem));
	pDungeon->SendToUsers((char*)&packet, packet.GetPacketSize());
}

void CmdDungeonGuardianItem(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_DUNGEON_GUARDIANITEM_ATTACH* pPacket = (DSTWS_DUNGEON_GUARDIANITEM_ATTACH*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (!pDungeon)	return;

	memcpy(&pDungeon->m_cGuardianItem, &pPacket->cItem, sizeof(CItem));
	
	WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM packet;
	packet.wDungeonID = (WORD)pDungeon->m_dwID;
	memcpy(&packet.cGuardianItem, &pDungeon->m_cGuardianItem, sizeof(CItem));
	pDungeon->SendToUsers((char*)&packet, packet.GetPacketSize());
}

//============================================================
// 던전 주인이 바꼇다.
//============================================================
void CmdDungeonOwnerChange(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_DUNGEON_OWNER_CHANGE* pPacket = (DSTWS_DUNGEON_OWNER_CHANGE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}
	
	DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (!pDungeon)	return;
	pDungeon->SetOwner(pPacket->dwOwnerIndex, pPacket->dwGuildId, pPacket->szUserLoginID, pPacket->szCharacterName);
	pDungeon->SendRefreshOwner();
}


void CmdFriendUserInfoRt(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_FRIENDUSER_INFO* pPacket = (DSTWS_FRIENDUSER_INFO*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}
	
	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{		
		CWorldUser* pGuildUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndexRv);

		if(!pGuildUser)
			return;

		WSTC_FRIENDUSER_INFO pSendPacket;
		memset(pSendPacket.szCharName, 0, sizeof(pSendPacket.szCharName));
		__lstrcpyn(pSendPacket.szCharName, pGuildUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pSendPacket.dwUserIndex		= pPacket->dwUserIndexRv;
		pSendPacket.wDungeonID		= pPacket->wDungeonID;
		pSendPacket.byLayerIndex	= pPacket->byLayerIndex;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CmdRefreshProduct_Acquisition(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_REFRESH_PRODUCT_ACQUISITION* pPacket = (DSTWS_REFRESH_PRODUCT_ACQUISITION*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID );
	if(pDungeon)
	{
		pDungeon->SendRefreshProductionInfo(pPacket->wProduction, 0);		
	}
}

void CmdCreateProduct(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSWST_CREATEPRODUCTION* pPacket = (DSWST_CREATEPRODUCTION*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID );
	if(pDungeon)
	{
		pDungeon->SendRefreshProductionInfo(pPacket->wProduction, pPacket->byProductionCount);		
	}
}

void CmdRefreshAccEntrance_Acquisition(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_REFRESH_ACCENTRANCE_ACQUISITION* pPacket = (DSTWS_REFRESH_ACCENTRANCE_ACQUISITION*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID );
	if(pDungeon)
	{
		pDungeon->SetAccEntrance(pPacket->nMoney, TRUE);		
	}
}

void CmdRefreshAccExp_Acquisition(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_REFRESH_ACCEXP_ACQUISITION* pPacket = (DSTWS_REFRESH_ACCEXP_ACQUISITION*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID );
	if(pDungeon)
	{
		pDungeon->SetAccExp(pPacket->dwAccExp);		
	}
}

void CmdRefreshEntrance_Edit(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_REFRESH_ENTRANCE_EDIT* pPacket = (DSTWS_REFRESH_ENTRANCE_EDIT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID );
	if(pDungeon)
	{
		pDungeon->SetEntrance(pPacket->nMoney);		
	}
}

void CmdRefreshDefenseCount(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_REFRESH_DEFENSECOUNT* pPacket = (DSTWS_REFRESH_DEFENSECOUNT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID );
	if(pDungeon)
	{
		pDungeon->SetDefenseCount(pPacket->byDefenseCount);		
	}
}

void CmdRefreshOperationMode(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_DUNGEON_REFRESH_OPERATIONMODE* pPacket = (DSTWS_DUNGEON_REFRESH_OPERATIONMODE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID );
	if(pDungeon)
	{
		pDungeon->SetOperationMode(pPacket->byOperationMode);		
	}
}


void CmdDungeonAliveInfo(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	if (NULL == g_pUserTable)
	{
		Log(LOG_JUST_DISPLAY, "g_pUserTable is NULL at CmdDungeonAliveInfo function.");
		return;
	}

	if (NULL == g_pThis)
	{
		Log(LOG_JUST_DISPLAY, "g_pThis is NULL at CmdDungeonAliveInfo fucntion.");
		return;
	}

	// 던젼 서버들이 정보를 보내왔다.
	DSTWS_DUNGEON_ALIVE_INFO* pRecievePakcet = (DSTWS_DUNGEON_ALIVE_INFO*)pMsg;
	if(pRecievePakcet->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pRecievePakcet->wHeader, dwLength);
		return;
	}

	WSTAS_SERVER_ALIVE_INFO SendPacket;
	SendPacket.dwServerID			= pRecievePakcet->dwServerID;
	SendPacket.wConnectedUserCount	= (WORD)pRecievePakcet->dwConnectedUserCount;
	SendPacket.dwExpGainRate		= 0;
	SendPacket.dwMagicItemDropRate	= 0;
	SendPacket.wServerType			= 1;		// ALIVE_DUNGEON_SERVER	

	// 받은 정보를 에이전트 서버에게 전송.
	g_pNet->SendToServer(AGENTINDEX, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void CmdCharacterSearch(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_SEARCH*	pPacket = (WSTDS_SEARCH*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(!pUser)
		return;

	WSTC_SEARCH	pSendPacket;
	memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
	__lstrcpyn(pSendPacket.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
	pSendPacket.byType		= 1;
	pSendPacket.wDungeonID	= (WORD)pServer->dwID;
	pSendPacket.dwUserIndex	= pPacket->dwSearchUserIndex;
	pSendPacket.byMsg		= pPacket->byMsg;				
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


//======================================
// 이벤트 던젼에서 유저가 들어왔음 
//======================================
void CmdEventDungeonUserEnter(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL* pPacket = (DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	if (NULL != g_pDungeonTable)
	{
		DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
		if (NULL == pDungeon)	return;

		// 입장객 카운트를 증가시킨다. ㅡ_-;
		++pDungeon->m_iEventEntered;

		if(pDungeon->m_pServer)
		{
			// 입장객 카운트를 던전서버에 알려준다.
			WSTDS_EVENT_JOINUSER_COUNT packet;
			packet.wDungeonID		= (WORD)pDungeon->m_dwID;
			packet.wJoinUserCount	= (WORD)pDungeon->m_iEventEntered;
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex,(char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
		}
	}
}


//======================================
// 이벤트 던젼에서 유저가 아웃했음.
//======================================
void CmdEventDungeonUserOut(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_EVENT_DUNGEON_USER_OUT* pPacket = (DSTWS_EVENT_DUNGEON_USER_OUT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (NULL == pDungeon)	return;
		
	if (pDungeon->m_iEventEntered > 0)
	{
		--pDungeon->m_iEventEntered;

		if(pDungeon->m_pServer)
		{
			// 입장객 카운트를 던전서버에 알려준다.
			WSTDS_EVENT_JOINUSER_COUNT packet;
			packet.wDungeonID		= (WORD)pDungeon->m_dwID;
			packet.wJoinUserCount	= (WORD)pDungeon->m_iEventEntered;
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex,(char*)&packet,packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
		}
	}	
}


//-----------------------------------------------------------------------------
// GM_COMMAND
//-----------------------------------------------------------------------------
void CmdGmCommand_DSWS(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_GM_COMMAND*	pCommand = (DSTWS_GM_COMMAND*)pMsg; 
	if(pCommand->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pCommand->wHeader, dwLength);
		return;
	}
}
