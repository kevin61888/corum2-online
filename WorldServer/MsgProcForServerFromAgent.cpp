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


void ACmdGuildCharacterDelete(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	ASTWS_GUILD_CHARACTER_DELETE* pPacket = (ASTWS_GUILD_CHARACTER_DELETE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	LP_GUILD_TABLE lpGuildTable	= g_pGuildTableHash->GetData(pPacket->dwGuildId);

	if(!lpGuildTable || !lpGuildTable->pGuildUserHash)
		return;

	char szQuery[0xff]={0,};

	if(pPacket->byRank==1)
	{
		WSTC_GUILD_DELETE	pSendPacket;

		// 길드 유저가 한명일 경우 //
		if(lpGuildTable->sGuild.byCount==1)
		{
			wsprintf(szQuery, "CZP_GUILDOUT_PREVALL %u", 0);
			g_pDb->THExecuteSQL(szQuery, FALSE, NULL, NULL, (BYTE)GAME_DB);

			wsprintf(szQuery, "CZP_GUILD_CHANGE 2, %u, 0, '', '', 0, 0", pPacket->dwGuildId);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);

			wsprintf(szQuery, "Delete From GuildUserInfo Where GuildId= %u", pPacket->dwGuildId);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILDUSER_DELETE, NULL, (BYTE)GAME_DB);

			wsprintf(szQuery, "Update mapinfo set Manager='---', OwnerGuildNum=0 where OwnerIndex = %u", pPacket->dwGuildId);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_OWNER, NULL, (BYTE)GAME_DB);
			
			wsprintf(szQuery, "Delete From GuildInfo Where [ID] = %u", pPacket->dwGuildId);
			g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);
			
#ifdef GUILD_WAR
			wsprintf(szQuery, "CZP_GUILDLIST_DELETE %u", pPacket->dwGuildId);
			g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);
#endif
			POSITION_ pos = g_pRankList->GetHeadPosition();

			while(pos)
			{				
				POSITION_ posTemp = pos;

				LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(pos);

				if(lpGuildWarRank->dwGuildId==pPacket->dwGuildId)
				{
					memset(lpGuildWarRank, 0, sizeof(lpGuildWarRank));
					LALFree(g_pRankListPool, lpGuildWarRank);
					lpGuildWarRank = NULL;
					g_pRankList->RemoveAt(posTemp);
					break;
				}
			}

			pSendPacket.dwGuildId	= pPacket->dwGuildId;

			if(lpGuildTable)
			{
				ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
											
				while(lpGuildUserNode)
				{
					CWorldUser* pGuildUser = lpGuildUserNode->pData;
					
					if(!pGuildUser)
					{
						lpGuildUserNode = lpGuildUserNode->pNext;
						continue;
					}

					// 던전 서버로 메세지 보내기 //
					DUNGEON_DATA_EX* pDungeon;
					pDungeon = g_pDungeonTable->GetDungeonInfo(pGuildUser->m_dwDungeonID);	

					if(pDungeon)
					{
						WSTDS_GUILD_MSG	pGuildMsg;
						pGuildMsg.dwUserIndex	= pGuildUser->m_dwUserIndex;
						g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pGuildMsg, pGuildMsg.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
					
					// 삭제 메세지 보내기 //
					memset(pGuildUser->m_szGuildName,0, sizeof(pGuildUser->m_szGuildName));
					memset(pGuildUser->m_szNickName, 0, sizeof(pGuildUser->m_szNickName));					
					
					pGuildUser->GuildIdPoint();

					pGuildUser->m_byType	= 0;
					pGuildUser->m_byRank	= 0;	
					pGuildUser->m_dwGuildId	= 0;			
					pSendPacket.bServerType = (BYTE)pGuildUser->m_dwCurServerPos;
					
					pGuildUser->GuildIdChange();

					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

					lpGuildUserNode = lpGuildUserNode->pNext;
				}
			}
			
			// Guild List 삭제 //
			g_pGuildTableHash->Delete(lpGuildTable, pPacket->dwGuildId, TRUE);	
			lpGuildTable = NULL;
		}
		else
		{
			// 길드 이양 //
			CWorldUser* pMasterUser = NULL;

			ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
											
			while(lpGuildUserNode)
			{
				CWorldUser* pGuildUser = lpGuildUserNode->pData;
				
				if(!pGuildUser)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}

				if(pMasterUser==NULL)
				{
					pMasterUser = pGuildUser;
				}
				else
				{
					if(pMasterUser->m_byRank>pGuildUser->m_byRank)
					{
						pMasterUser = pGuildUser;
					}
					else if(pMasterUser->m_byRank==pGuildUser->m_byRank)
					{
						if(pMasterUser->m_dwLevel<pGuildUser->m_dwLevel)
						{
							pMasterUser = pGuildUser;
						}
					}
				}

				lpGuildUserNode = lpGuildUserNode->pNext;
			}						

			LP_GUILD_OFFLINE_USER lpMasterOfflineUser = NULL;
			
			ListNode<GUILD_OFFLINE_USER>* lpGuildOfflineUserNode = lpGuildTable->pGuildOfflineHash->GetHead();

			while(lpGuildOfflineUserNode)
			{
				LP_GUILD_OFFLINE_USER pOfflineGuildUser = lpGuildOfflineUserNode->pData;
				
				if(!pOfflineGuildUser)
				{
					lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
					continue;
				}

				if(pOfflineGuildUser->byRank==1)
				{
					lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
					continue;
				}

				if(lpMasterOfflineUser==NULL)
					lpMasterOfflineUser = pOfflineGuildUser;
				else
				{
					if(lpMasterOfflineUser->byRank>pOfflineGuildUser->byRank)
						lpMasterOfflineUser = pOfflineGuildUser;
					else if(lpMasterOfflineUser->byRank==pOfflineGuildUser->byRank)
					{
						if(lpMasterOfflineUser->dwLevel<pOfflineGuildUser->dwLevel)
							lpMasterOfflineUser = pOfflineGuildUser;
					}
				}

				lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
			}						
						
			lpGuildTable->sGuild.byCount -= 1;

			wsprintf(szQuery, "CZP_GUILDOUT_PREV %u %u", pPacket->dwGuildId, pPacket->dwIndex);
			g_pDb->THExecuteSQL(szQuery, FALSE, NULL, NULL, (BYTE)GAME_DB);

			wsprintf(szQuery, "CZP_GUILD_CHANGE 0, 0, %u, '', '', 0, 0", pPacket->dwIndex);			
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);
			wsprintf(szQuery, "Delete From GuildUserInfo Where Char_Index = %u", pPacket->dwIndex);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILDUSER_DELETE, NULL, (BYTE)GAME_DB);

			if(!pMasterUser && !lpMasterOfflineUser)
				return;
						
			CWorldUser* pDeleteUser = lpGuildTable->pGuildUserHash->GetData(pPacket->dwIndex);
			
			if(pDeleteUser)
				lpGuildTable->pGuildUserHash->Delete(pDeleteUser, pPacket->dwIndex, TRUE);

			LP_GUILD_OFFLINE_USER lpGuildOfflineUser = lpGuildTable->pGuildOfflineHash->GetData(pPacket->dwIndex);

			if(lpGuildOfflineUser)
				lpGuildTable->pGuildOfflineHash->Delete(lpGuildOfflineUser, pPacket->dwIndex, TRUE);

			if(pMasterUser && lpMasterOfflineUser)
			{
				if(pMasterUser->m_byRank<lpMasterOfflineUser->byRank)
					lpMasterOfflineUser = NULL;
				else if(pMasterUser->m_byRank==lpMasterOfflineUser->byRank)
				{
					if(pMasterUser->m_dwLevel==lpMasterOfflineUser->dwLevel)
						lpMasterOfflineUser = NULL;
					else if(pMasterUser->m_dwLevel>lpMasterOfflineUser->dwLevel)
						lpMasterOfflineUser = NULL;
					else
						pMasterUser = NULL;
				}
				else 
					pMasterUser = NULL;
			}

			char	szName[MAX_CHARACTER_NAME_LENGTH]={0,};
			DWORD	dwUserIndex	= 0;

			memset(szName, 0, sizeof(szName));

			DUNGEON_DATA_EX* pDungeon = NULL;

			if(pMasterUser)
				pDungeon = g_pDungeonTable->GetDungeonInfo(pMasterUser->m_dwDungeonID);				

			if(!pMasterUser)
			{
				lpGuildTable->sGuild.byMember[lpGuildOfflineUser->byRank-1] -=  1;
				lpGuildOfflineUser->byRank = 1;

				wsprintf(szQuery, "Update mapinfo set Manager='---', OwnerGuildNum=0 where OwnerIndex = %u", lpMasterOfflineUser->dwUserIndex);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_OWNER, NULL, (BYTE)GAME_DB);							

				wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pPacket->dwGuildId);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, NULL, (BYTE)GAME_DB);

				wsprintf(szQuery, "CZP_GUILD_CHANGE 1, 0, %u, '', '', 0, %u", lpGuildOfflineUser->dwUserIndex, lpGuildOfflineUser->byRank);			
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);

				dwUserIndex	= lpGuildOfflineUser->dwUserIndex;
				__lstrcpyn(szName, lpGuildOfflineUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			}
			else if(!lpMasterOfflineUser)
			{
				lpGuildTable->sGuild.byMember[pMasterUser->m_byRank-1] -=  1;
				pMasterUser->m_byRank = 1;
				
				wsprintf(szQuery, "Update mapinfo set Manager='---', OwnerGuildNum=0 where OwnerIndex = %u", pMasterUser->m_dwUserIndex);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_OWNER, NULL, (BYTE)GAME_DB);

				wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pPacket->dwGuildId);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, NULL, (BYTE)GAME_DB);

				wsprintf(szQuery, "CZP_GUILD_CHANGE 1, 0, %u, '', '', 0, %u", pMasterUser->m_dwUserIndex, pMasterUser->m_byRank);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);

				dwUserIndex	= pMasterUser->m_dwUserIndex;
				__lstrcpyn(szName, pMasterUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);

				// 던전 서버에 보내기 //
				if(pDungeon)
				{
					WSTDS_GUILD_INFO pServerPacket;
					memset(pServerPacket.szGuildName, 0, sizeof(pServerPacket.szGuildName));
					memset(pServerPacket.szNickName, 0, sizeof(pServerPacket.szNickName));
					pServerPacket.dwUserIndex	= pMasterUser->m_dwUserIndex;
					pServerPacket.dwGuildId		= pMasterUser->m_dwGuildId;
					pServerPacket.byRank		= pMasterUser->m_byRank;
					pServerPacket.byType		= pMasterUser->m_byType;
					__lstrcpyn(pServerPacket.szGuildName, pMasterUser->m_szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
					__lstrcpyn(pServerPacket.szNickName, pMasterUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);		
					g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pServerPacket, pServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				}				
			}

			WSTC_USER_MASTER pMasterPacket;

			memset(pMasterPacket.szCharacterName, 0, sizeof(pMasterPacket.szCharacterName));			
			__lstrcpyn(pMasterPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);			
			pMasterPacket.dwGuildId			= pPacket->dwGuildId;		
			pMasterPacket.dwUserIndex		= pPacket->dwIndex;
			pMasterPacket.dwMasterUserIndex	= dwUserIndex;
			pMasterPacket.byMember[0]		= lpGuildTable->sGuild.byMember[0];
			pMasterPacket.byMember[1]		= lpGuildTable->sGuild.byMember[1];
			pMasterPacket.byMember[2]		= lpGuildTable->sGuild.byMember[2];
			pMasterPacket.byMember[3]		= lpGuildTable->sGuild.byMember[3];
			pMasterPacket.byType			= 0;

			lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
											
			while(lpGuildUserNode)
			{
				CWorldUser* pGuildUser = lpGuildUserNode->pData;
				
				if(!pGuildUser)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}

				g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pMasterPacket, pMasterPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				lpGuildUserNode = lpGuildUserNode->pNext;
			}
						
			if(pMasterUser)
			{
				lpGuildOfflineUserNode = lpGuildTable->pGuildOfflineHash->GetHead();		
							
				while(lpGuildOfflineUserNode)
				{
					LP_GUILD_OFFLINE_USER pGuildOfflineUser = lpGuildOfflineUserNode->pData;
					
					if(pGuildOfflineUser)
					{
						WSTC_GUILD_OFFLINEUSER pOffLinePacket;
						memset(pOffLinePacket.szName, 0, sizeof(pOffLinePacket.szName));
						memset(pOffLinePacket.szNickName, 0, sizeof(pOffLinePacket.szNickName));
						pOffLinePacket.byRank		= pGuildOfflineUser->byRank;
						pOffLinePacket.dwLevel		= pGuildOfflineUser->dwLevel;
						pOffLinePacket.dwUserIndex	= pGuildOfflineUser->dwUserIndex;
						pOffLinePacket.wClass		= pGuildOfflineUser->wClass;
						__lstrcpyn(pOffLinePacket.szName, pGuildOfflineUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
						__lstrcpyn(pOffLinePacket.szNickName, pGuildOfflineUser->szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);
						g_pNet->SendToUser(pMasterUser->m_dwConnectionIndex, (char*)&pOffLinePacket, pOffLinePacket.GetPacketSize()
							, FLAG_SEND_NOT_ENCRYPTION);
					}

					lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
				}
			}
		}
	}
	else
	{				
		WSTC_USER_DELETE pSendPacket;	

		LP_GUILD_OFFLINE_USER lpOfflineUser = lpGuildTable->pGuildOfflineHash->GetData(pPacket->dwIndex);

		if(!lpOfflineUser)
			return;		

		if(lpGuildTable->sGuild.byMember[lpOfflineUser->byRank-1]==0)
			return;

		lpGuildTable->sGuild.byCount -= 1;
		
		switch(lpOfflineUser->byRank)
		{
		case 2:
			lpGuildTable->sGuild.byMember[1] -= 1;
			break;
		case 3:
			lpGuildTable->sGuild.byMember[2] -= 1;
			break;
		case 4:
			lpGuildTable->sGuild.byMember[3] -= 1;
			break;
		}		

		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pSendPacket.dwGuildId	= pPacket->dwGuildId;		
		pSendPacket.dwUserIndex	= pPacket->dwIndex;
		pSendPacket.byMember[0]	= lpGuildTable->sGuild.byMember[0];
		pSendPacket.byMember[1]	= lpGuildTable->sGuild.byMember[1];
		pSendPacket.byMember[2]	= lpGuildTable->sGuild.byMember[2];
		pSendPacket.byMember[3]	= lpGuildTable->sGuild.byMember[3];

		ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();
								
		while(lpGuildUserNode)
		{
			CWorldUser* pGuildUser = lpGuildUserNode->pData;
			
			if(!pGuildUser)
			{
				lpGuildUserNode = lpGuildUserNode->pNext;
				continue;						
			}

			g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			lpGuildUserNode = lpGuildUserNode->pNext;
		}
		
		// 메세지 보내기 //				
		if(lpOfflineUser)
		{
			ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();
								
			while(lpGuildUserNode)
			{
				CWorldUser* pGuildUser = lpGuildUserNode->pData;
				
				if(!pGuildUser)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}
		
				if(pGuildUser->m_byRank==1)
				{
					WSTC_GUILD_OFFLINEUSER_DELETE pOffLineDeletePacket;
					pOffLineDeletePacket.dwUserIndex = lpOfflineUser->dwUserIndex;
					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pOffLineDeletePacket, pOffLineDeletePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				lpGuildUserNode = lpGuildUserNode->pNext;
			}
			
			wsprintf(szQuery, "Update mapinfo set Manager='---', OwnerGuildNum=0 where OwnerIndex = %u", lpOfflineUser->dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_OWNER, NULL, (BYTE)GAME_DB);
			
			lpGuildTable->pGuildOfflineHash->Delete(lpOfflineUser, lpOfflineUser->dwUserIndex, TRUE);
			memset(lpOfflineUser, 0, sizeof(GUILD_OFFLINE_USER));
		}

		// DB 쿼리 //		
		wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pPacket->dwGuildId);
		g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, NULL, (BYTE)GAME_DB);			

		wsprintf(szQuery, "Delete From GuildUserInfo Where Char_Index = %u", pPacket->dwIndex);
		g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILDUSER_DELETE, NULL, (BYTE)GAME_DB);
	}
}

void ACmdRequestServerAlive(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	if (NULL == g_pUserTable)
	{
		Log(LOG_JUST_DISPLAY, "g_pUserTable is NULL at ACmdRequestServerAlive function.");
		return;
	}

	if (NULL == g_pThis)
	{
		Log(LOG_JUST_DISPLAY, "g_pThis is NULL at ACmdRequestServerAlive fucntion.");
		return;
	}

	ASTWS_REQUEST_SERVER_ALIVE* pRecievePakcet = (ASTWS_REQUEST_SERVER_ALIVE*) pMsg;
	if(pRecievePakcet->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pRecievePakcet->wHeader, dwLength);
		return;
	}

	// 각 던젼 서버들에게 살아 있는지 정보를 보내달라 요청.
	WSTDS_REQUEST_DUNGEON_ALIVE	SendPacket;	
	g_pServerTable->BroadCast((char*)&SendPacket, SendPacket.GetPacketSize());

	// 월드 서버 자신의 정보를 보냄.
	WSTAS_SERVER_ALIVE_INFO ServerAlivePacket;	
	ServerAlivePacket.wServerType			= 0;			// ALIVE_WORLD_SERVER;
	ServerAlivePacket.wConnectedUserCount	= (WORD)g_pUserTable->GetCount();
	ServerAlivePacket.dwServerID			= g_pThis->GetServerID();
	ServerAlivePacket.dwExpGainRate			= g_pThis->m_dwGLOBAL_EXP_GAIN;
	ServerAlivePacket.dwMagicItemDropRate	= g_pThis->m_GLOBAL_MAGIC_FIND_PBT;	
	g_pNet->SendToServer(AGENTINDEX, (char*)&ServerAlivePacket, ServerAlivePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void ACmdCMToolNotice(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	ASTWS_CMTOOL_NOTICE* pRecievePacket = (ASTWS_CMTOOL_NOTICE*)pMsg;
	if(pRecievePacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pRecievePacket->bHeader, dwLength);
		return;
	}

	WSTDS_NOTICE_ACTION NoticeActionPacket;
	memset(NoticeActionPacket.szMessage, 0, 1024);	
	__lstrcpyn(NoticeActionPacket.szMessage, pRecievePacket->szMsg, 1024);	
	NoticeActionPacket.wMsgLen = pRecievePacket->wMsgLen;

	// 혹시라도 버퍼가 덮어졌는지 체크.
#ifdef _DEBUG
	if (1024 <= NoticeActionPacket.wMsgLen)
	{
		Log(LOG_IMPORTANT, "Dangerous. Notice array overscope. at CmdNotice function.");
		return;
	}
#endif

	// 각 던젼 서버들에게 브로드 캐스팅을 명한다. 	
	g_pServerTable->BroadCast((char*)&NoticeActionPacket, NoticeActionPacket.GetPacketSize());	

	// 그리고 던젼에 접속해 있지 않은 월드에만 붙은 유저들에게 공지 메시지를 보내준다.	
	CTWS_GM_NOTICE NoticePacket;	
	__lstrcpyn(NoticePacket.szMsg,pRecievePacket->szMsg,1024);	
	NoticePacket.wMsgLen = pRecievePacket->wMsgLen;	
	g_pUserTable->SendUserInWorld((char*)&NoticePacket,NoticePacket.GetPacketSize());
}


void AcmdKickUser(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	ASTWS_KICK_USER *pPacket = (ASTWS_KICK_USER*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser *pUser = g_pUserTable->GetUserInfo(pPacket->dwCharIndex);
	
	if(pUser)
	{
		// 중국 빌링의 경우..... 0이 아닌 값이 들어올 수 있으며, 그런 경우 빌링 서버의 Kick 명령이다.
		if(pPacket->btKickCode)
		{
			WSTC_CHINA_BILL_KICK_CODE KickPacket;

			KickPacket.btKickCode = pPacket->btKickCode;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&KickPacket, KickPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			// Kick 대상 리스트에 이 넘을 넣는다.... ^^;;
			// 추후 Process loop에서 이 리스트 검사하여 대기시간 5초를 넘은 유저를 KICK한다!!!!
			AddChinaBillingKickList(pPacket->dwCharIndex);
		}
		else
		{
			Log(LOG_NORMAL, "World User Kicked! (Name:%s)", pUser->m_szCharacterName);
			pUser->m_bKicked = 1;	
			DWORD dwConnectionIndex = pUser->m_dwConnectionIndex;
			OnDisconnectUser(dwConnectionIndex);
			g_pNet->SetUserInfo(dwConnectionIndex, NULL);
			g_pNet->CompulsiveDisconnectUser(dwConnectionIndex);
		}
	}

	// 확실하게 Kick 해버리자. 
	WSTDS_KICK_USER	packet;
	packet.dwCharIndex = pPacket->dwCharIndex;
	g_pServerTable->BroadCast((char*)&packet, packet.GetPacketSize());
}


void AcmdAddNewUser(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	ASTWS_ADD_NEW_USER* pPacket = (ASTWS_ADD_NEW_USER*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}
	
	// 이미 AcceptTable에 추가되어있는 IP라면 다 지워버리고 
	ACCEPT_USER* pUser = (ACCEPT_USER*)0xffffffff;
	while(pUser)
	{
		pUser = g_pAcceptTable->GetAcceptInfo(pPacket->dwChar_Index);

		if(pUser)
		{
			g_pAcceptTable->Remove(pUser);
			Log(LOG_IMPORTANT, "Request CharacterIndex is already accepted! (AcmdAddNewUser Function!)");
		}
		else 
		{
			goto lb_Continue;
		}	
	}

lb_Continue:		
	ACCEPT_USER *pAccept	= g_pAcceptTable->AllocNewAccept();
	pAccept->dwChar_Index	= pPacket->dwChar_Index;
	pAccept->dwSerialCode	= pPacket->dwSerialCode;
	pAccept->bBillingType	= pPacket->bBillingType;
	pAccept->BillingEday	= pPacket->BillingEday;
	pAccept->dwRemainTime	= pPacket->dwRemainTime;
	
	if(!g_pAcceptTable->Add(pAccept))	
	{
		Log(LOG_IMPORTANT, "Add User Failed!");
		WSTAS_USER_ACCEPT_FAILED FailPacket;
		FailPacket.dwPropID = pPacket->dwPropID;
		FailPacket.bErrorCode = 1;
		g_pNet->SendToServer(pServer->dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	// Accept 된 시점의 TickCount
	pAccept->dwAcceptedTick = g_dwCurTick;	

	WSTAS_USER_ACCEPT_ALLOWED	Packet;
	Packet.dwPropID = pPacket->dwPropID;
	g_pNet->SendToServer(pServer->dwConnectionIndex, (char*)&Packet, Packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void AcmdBillingSmallTime(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	ASTWS_BILLING_SMALL_TIME *pPacket = (ASTWS_BILLING_SMALL_TIME*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = g_pUserTable->GetUserInfo( pPacket->dwChar_Index );

	if(pUser)
	{
		WSTC_BILLING_INFO	packet;
		memset(&packet.BillingEndDate, 0, sizeof(packet.BillingEndDate));	//종량제에서는 EndDate 안쓴다. 

		packet.bCurBillingType = pPacket->bBillingType;
		packet.bInfoType = BILLING_INFO_SMALL_TIME;
		packet.dwRemainTime = pPacket->dwSecond;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	}
	else
	{
		Log(LOG_IMPORTANT, "User is not found! at AcmdBillingSmallTime Function()!");
	}
}

void AcmdChangeBillingType(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	ASTWS_CHANGE_BILLING_TYPE *pPacket = (ASTWS_CHANGE_BILLING_TYPE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = g_pUserTable->GetUserInfo( pPacket->dwChar_Index );

	if(pUser)
	{
		WSTC_BILLING_INFO	packet;
		packet.bCurBillingType = pPacket->bBillingType;
		packet.bInfoType = BILLING_INFO_START_BILLING;
		packet.dwRemainTime = pPacket->dwRemainTime;
		packet.BillingEndDate = pPacket->eDay;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
	else
	{
		Log(LOG_IMPORTANT, "User is not found! at AcmdChangeBillingType Function()!");
	}
}


void AcmdServerAttachResult(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	ASTWS_SERVER_ATTACH_RESULT* pPacket = (ASTWS_SERVER_ATTACH_RESULT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}
	
	if(pPacket->bErrorCode)
	{
		Log(LOG_NORMAL, "Fail to attach LoginAgent Server! (ErrorCode:%d)", pPacket->bErrorCode);
		return;
	}

	// LoginAgent 서버는 ID값을 0으로 셋팅하자 
	SERVER_DATA* pAgent = g_pServerTable->SetLoginAgentServer(dwConnectionIndex);
	
	if(!pAgent)
	{
		Log(LOG_IMPORTANT, "Fail to alloc LoginAgent Server!");
		return;
	}
	
	g_pThis->m_bAdultMode = pPacket->bAdultMode;	
}