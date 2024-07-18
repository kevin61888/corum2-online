#include "stdafx.h"
#include "DBProc.h"
#include "WorldUser.h"
#include "GuildHash.h"
#include "MessengerHash.h"
#include "WorldUserTable.h"
#include "ServerTable.h"
#include "DungeonTable.h"
#include "ConnectionDistributionTable.h"
#include "DUNGEON_DATA_EX.h"
#include "ServerFunction.h"
#include "recvmsg.h"
#include "AcceptTable.h"
#include "NTOS.h"
#include "../CommonServer/CommonHeader.h"


void QueryTypeCharacterInfo(DBRECEIVEDATA* pResult)
{
	WSTC_SEARCH				pSendPacket;
	SCHARACTER_SEARCH		sCharacterSearch;
	WSTDS_CHARACTER_SEARCH	pServerPacket;

	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
		return;
	else if(pResult->nError < 0)
		return;
			
	memcpy(&sCharacterSearch, pResult->Query.select.pResult, sizeof(SCHARACTER_SEARCH));

	CWorldUser* pSearchUser = g_pUserTable->GetUserInfo(sCharacterSearch.dwUserIndex);

	// 월드에서 클라이언트로 결과 보내기 
	memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
	__lstrcpyn(pSendPacket.szName, sCharacterSearch.szName, MAX_CHARACTER_NAME_REAL_LENGTH);
	pSendPacket.byType		= 0;
	pSendPacket.wDungeonID	= 0;
	pSendPacket.dwUserIndex	= sCharacterSearch.dwUserIndex;

	if(pSearchUser)
		pSendPacket.byMsg	= 0;
	else
		pSendPacket.byMsg	= 1;		
	
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
	
	// 던전 서버 검색 	
	memset(pServerPacket.szName, 0, sizeof(pServerPacket.szName));
	__lstrcpyn(pServerPacket.szName, sCharacterSearch.szName, MAX_CHARACTER_NAME_REAL_LENGTH);
	pServerPacket.dwSearchUserIndex	= sCharacterSearch.dwUserIndex;
	pServerPacket.dwUserIndex		= pUser->m_dwUserIndex;	
	g_pServerTable->BroadCast((char*)&pServerPacket, pServerPacket.GetPacketSize());
}


void QueryTypeUserInfo(DBRECEIVEDATA* pResult)
{	
	char		szSQL[0xff] = {0,};
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)	return;
	
	if(pResult->nError < 0)
	{
		Log(LOG_IMPORTANT, "Query Error: QueryTypeUserEquip()...  (User: %s)", pUser->m_szID);
		return;
	}

	char* pQueryResult	= pResult->Query.select.pResult;
	DWORD dwLen			= MAX_ID_LENGTH;
	memcpy(pUser->m_szID, pQueryResult, dwLen);

	pQueryResult += dwLen;
	dwLen = MAX_CHARACTER_NAME_LENGTH;
	memcpy(pUser->m_szCharacterName, pQueryResult, dwLen);
	
	WSTC_WORLD_USER_INFO	packet;	
	packet.dwGuildId		= 0;
	packet.dwUserIndex		= pUser->m_dwUserIndex;
	packet.byCurrentHand	= pUser->m_byCurrentHand;

	// 40 = MAX_ID_LENGTH + MAX_CHARACTER_NAME_LENGTH
	pQueryResult += dwLen;
	dwLen = pResult->Query.select.dwRowSize - (MAX_ID_LENGTH + MAX_CHARACTER_NAME_LENGTH) - 1;
	memcpy(&packet.dwRecentOutMap, pQueryResult, dwLen);

	pQueryResult+= dwLen;
	BYTE bInvItemCheck = FALSE;
	memcpy(&bInvItemCheck,pQueryResult,sizeof(BYTE));

	if(!bInvItemCheck)
	{
		for(int i = 0; i < MAX_EQUIP; i++)
		{
			if(packet.Equip[i].GetID() != 0)
			{
				WORD wOldID = packet.Equip[i].GetID();
				WORD wNewID = (WORD)(( wOldID / 100 * 200 ) + wOldID % 100);
				packet.Equip[i].SetID(wNewID);
			}
		}
	}
	
	DUNGEON_DATA_EX* pDungeon = NULL;
	pDungeon = g_pDungeonTable->GetDungeonInfo( (WORD)packet.dwRecentOutPos );

	if( !pDungeon || (pDungeon && !pDungeon->m_dWorldPos_X))
	{
		packet.dwRecentOutPos = 1;
		pDungeon = g_pDungeonTable->GetDungeonInfo( (WORD)packet.dwRecentOutPos );
		Log(LOG_IMPORTANT, "Dungeon World coordinate error");
	}	
		
	// 이벤트 던젼은 위치가 유동적이므로, 그 부근에선 시작될 수 없다. 또한 이벤트 던젼 번호가 여기에 들어가 있는 것 부터가 수상하다.	
	if (DUNGEON_TYPE_EVENT == pDungeon->GetDungeonType())
	{
		Log(LOG_IMPORTANT, "Someone try find empty position near Evnet Dungeon.");

		packet.dwRecentOutPos = 1;
		pDungeon = g_pDungeonTable->GetDungeonInfo( (WORD)packet.dwRecentOutPos );
	}

	if(!pDungeon)
	{
		// Recent Dungeon 정보가 존재 하지 않음 
		WSTC_WORLD_LOGIN_FAIL	FailPacket;
		FailPacket.bErrorCode = 3;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		Log(LOG_IMPORTANT, "Recent DungeonInfo is not found! (Name:%s, DungeonID:%d)", pUser->m_szCharacterName, packet.dwRecentOutPos);
		return;	
	}

	// 원래 DB 필드상의 RecentOutMap의 의미는 월드서버의 구분자이나 
	// 클라이언트에서는 어느 월드서버인지의 정보는 알필요 없으므로 RecentOutMap 변수에는 월드맵 대륙번호를
	// 보내줘서 최근 로그아웃한 대륙번호에 맞는 맵파일을 로딩하게끔 한다. 
	packet.dwRecentOutMap = pDungeon->m_wWorldMapID;
	
	VECTOR3 vDungeonPos = { (float)pDungeon->m_dWorldPos_X, 0.0f, (float)pDungeon->m_dWorldPos_Z };
	VECTOR3 vStartPos	= { 0.f, 0.f, 0.f };
	
	BOOL bRet;
	if(pDungeon->m_dwID < 10)
	{
		// 마을인 경우
		bRet = FindEmptyPosNearDungeon((BYTE)packet.dwRecentOutMap, &vDungeonPos, &vStartPos, TRUE);		 
	}
	else
	{
		bRet = FindEmptyPosNearDungeon((BYTE)packet.dwRecentOutMap, &vDungeonPos, &vStartPos, FALSE);
	}
	
	if(!bRet)
	{
		//시작 위치를 찾을 수 없음 
		WSTC_WORLD_LOGIN_FAIL	FailPacket;
		FailPacket.bErrorCode = 4;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		Log(LOG_IMPORTANT, "Empty position is not found! (Name:%s, DungeonID:%d)"
			, pUser->m_szCharacterName, packet.dwRecentOutPos);
		return;	
	}

	packet.fStartPos_X = vStartPos.x;
	packet.fStartPos_Z = vStartPos.z;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

	if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
	{
		WSTC_BILLING_INFO	BillPacket;
		BillPacket.bInfoType = BILLING_INFO_START_BILLING;
		BillPacket.bCurBillingType = pUser->m_bBillingType;
		BillPacket.BillingEndDate = pUser->m_BillingEday;
		BillPacket.dwRemainTime = pUser->m_dwRemainTime;	//클라이언트에서 Tick단위로 처리했다. =_=;;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&BillPacket, BillPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);	
	}

	//현재 유저의 위치 갱신 
	pUser->m_v3CurWorldPos.x	= vStartPos.x;
	pUser->m_v3CurWorldPos.y	= 0.0f;
	pUser->m_v3CurWorldPos.z	= vStartPos.z;
	pUser->m_dwCurServerPos		= POS_SERVER_WORLD;
	pUser->m_dwPrevDungeonId	= pDungeon->m_dwID;
	pUser->m_bWorldId = (BYTE)packet.dwRecentOutMap;
	memcpy(pUser->m_pEquip,  packet.Equip, sizeof(packet.Equip) );
	pUser->m_ListPosInWorld = g_pUserTable->m_pUserListInWorld->AddTail(pUser);	//월드에 있는 유저들 List에 추가 
	if(!pUser->m_ListPosInWorld)
	{
		Log(LOG_IMPORTANT, "WorldUserList Add Failed! (Name:%s)", pUser->m_szCharacterName);
	}
	else
		Log(LOG_JUST_FILEWRITE, "WorldUserList Added! in QueryTypeUserInfo() (Name:%s)", pUser->m_szCharacterName);
	
	WSTC_SKILL_KEY pSkillKey;

	for(int i = 0; i < 16; i++)
	{
		pUser->m_bySkillKey[i]	= packet.bySkillKey[i];
		pUser->m_byPosKey[i]	= packet.byPosKey[i];
		pSkillKey.bSkillKey[i]	= packet.bySkillKey[i];
		pSkillKey.bPosKey[i]	= packet.byPosKey[i];
	}	
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSkillKey, pSkillKey.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);		
		
	// Data 
	pUser->m_dwUserIndex	= packet.dwUserIndex;
	pUser->m_dwPartyId		= 0;
	pUser->m_byType			= packet.byType;
	pUser->m_byRank			= packet.byRank;
	pUser->m_dwGuildId		= packet.dwGuildId;
	pUser->m_dwMoney		= packet.dwMoney;
	pUser->m_wClass			= packet.wClass;
	pUser->m_wHead			= packet.wHead;			//Packet에는 꼭 넣을 필요 없다!!  추후수정 
	pUser->m_byHelp[0]		= packet.byHelpTypeA;
	pUser->m_byHelp[1]		= packet.byHelpTypeB;
	pUser->m_byHelp[2]		= packet.byHelpInterface;
	pUser->m_wCriminalTime	= packet.wCriminalTime; 
	pUser->m_dwLevel		= packet.dwLev;
	pUser->m_byCurrentHand	= packet.byCurrentHand;
	pUser->m_byMessenger	= packet.byMessenger;
	pUser->m_dwPrevGuildId	= packet.dwPrevGuildId;
	
	memset(&pUser->m_sGuildJoinTime, 0, sizeof(pUser->m_sGuildJoinTime));
	memset(pUser->m_szGuildName, 0, sizeof(pUser->m_szGuildName));
	memset(pUser->m_szNickName, 0, sizeof(pUser->m_szNickName));

	memcpy(&pUser->m_sGuildJoinTime, &packet.sGuildJoinTime, sizeof(pUser->m_sGuildJoinTime));
	__lstrcpyn(pUser->m_szGuildName, packet.szGuildName, MAX_GUILD_NAME_LENGTH);
	__lstrcpyn(pUser->m_szNickName, packet.szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);		

	pUser->m_dwStartTick		= g_dwCurTick;
	
	if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
	{
		if( IS_ABLE_SERVICE_TYPE( ST_BILLING ) )
		{
			pUser->m_dwBillingStartTick = g_dwCurTick;
		}
	}

	g_pUserTable->Add(pUser);
	
#ifdef CHECK_KEEP_ALIVE
	g_pCDTable->Add(pUser);		//CDTable 추가 
#endif

	wsprintf(szSQL, "Select Char_Index, Type, Name From CharacterInfo Where [ID] = '%s'", pUser->m_szID);
	g_pDb->THOpenRecord(szSQL, QUERY_TYPE_GUILD_CHK, (void*)pUser->m_dwConnectionIndex, 4, (BYTE)GAME_DB);

	// List Add //		
	if(pUser->m_dwGuildId!=0)
	{
		LP_GUILD_TABLE	lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);
		
		if(lpGuildTable)
		{
			LP_GUILD_OFFLINE_USER pGuildOfflineUser = lpGuildTable->pGuildOfflineHash->GetData(pUser->m_dwUserIndex);

			if(pGuildOfflineUser)
			{
				ListNode<CWorldUser>* pGuildUserNodeRank = lpGuildTable->pGuildUserHash->GetHead();

				while(pGuildUserNodeRank)
				{
					CWorldUser* pGuildUserRank = pGuildUserNodeRank->pData;

					if(pGuildUserRank)
					{
						if(pGuildUserRank->m_byRank==1)
						{
							WSTC_GUILD_OFFLINEUSER_DELETE pOffLineDeletePacket;
							pOffLineDeletePacket.dwUserIndex = pGuildOfflineUser->dwUserIndex;
							g_pNet->SendToUser(pGuildUserRank->m_dwConnectionIndex, (char*)&pOffLineDeletePacket, pOffLineDeletePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}
					}

					pGuildUserNodeRank = pGuildUserNodeRank->pNext;
				}
				
				lpGuildTable->pGuildOfflineHash->Delete(pGuildOfflineUser, pGuildOfflineUser->dwUserIndex, TRUE);
				memset(pGuildOfflineUser, 0, sizeof(pGuildOfflineUser));
			}			

			// 길드 마스터일 경우 메세지 보내기 //
			if(pUser->m_byRank == 1)
			{
				ListNode<GUILD_OFFLINE_USER>*	lpGuildOfflineUserNode = lpGuildTable->pGuildOfflineHash->GetHead();		
							
				while(lpGuildOfflineUserNode)
				{
					pGuildOfflineUser = lpGuildOfflineUserNode->pData;
					
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
						g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pOffLinePacket, pOffLinePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}

					lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
				}				
			}

			// 길드 정보 //
			WSTC_GUILD_USER pPacket;
			memset(pPacket.szGuildName, 0, sizeof(pPacket.szGuildName));

			for( int i = 0 ; i > GUILD::GUILD_RANK_COUNT_MAX ; ++i )
			{
				::ZeroMemory( 
					pPacket.szRankName[i],
					MAX_GUILD_NAME_LENGTH 
					);
			}

			pPacket.byType			= pUser->m_byType;
			pPacket.byCount			= lpGuildTable->sGuild.byCount;
			pPacket.dwGuildId		= lpGuildTable->sGuild.dwGuildId;			
			pPacket.dwExp			= lpGuildTable->sGuild.dwExp;
			pPacket.wLevel			= lpGuildTable->sGuild.wLevel;
			pPacket.dwPoint			= lpGuildTable->sGuild.dwPoint;
			pPacket.byMember[0]		= lpGuildTable->sGuild.byMember[0];
			pPacket.byMember[1]		= lpGuildTable->sGuild.byMember[1];
			pPacket.byMember[2]		= lpGuildTable->sGuild.byMember[2];
			pPacket.byMember[3]		= lpGuildTable->sGuild.byMember[3];
			pPacket.byLevelIndex	= lpGuildTable->bLevelIndex;
			
			__lstrcpyn(pPacket.szGuildName, pUser->m_szGuildName, MAX_GUILD_NAME_LENGTH);

			for( int i = 0 ; i < GUILD::GUILD_RANK_COUNT_MAX ; ++i )
			{
				__lstrcpyn(
					pPacket.szRankName[i], 
					lpGuildTable->sGuild.szRankName[i], 
					MAX_RANK_REAL_NICKNAME_LENGTH
					);
			}
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
											
			lpGuildTable->pGuildUserHash->Insert(pUser, pUser->m_dwUserIndex); 
					
			ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
								
			while(lpGuildUserNode)
			{
				CWorldUser* pGuildUser = lpGuildUserNode->pData;
				
				if(!pGuildUser)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}
				
				// 메세지 보내기 //
				if(pGuildUser->m_dwUserIndex==pUser->m_dwUserIndex)
				{
					// 길드 전체에게 보내기(자기 자신은 빼고) //			
					ListNode<CWorldUser>* lpGuildUserSubNode = lpGuildTable->pGuildUserHash->GetHead();		
											
					while(lpGuildUserSubNode)
					{
						CWorldUser* pGuildUserSubUser = lpGuildUserSubNode->pData;

						if(pGuildUserSubUser->m_dwUserIndex!=pUser->m_dwUserIndex)
						{
							WSTC_GUILD_USERINFO pPacketSend;					
							
							memset(pPacketSend.szNickName, 0, sizeof(pPacketSend.szNickName));
							memset(pPacketSend.szCharacterName, 0, sizeof(pPacketSend.szCharacterName));

							pPacketSend.bGuildCount	= 1;
							pPacketSend.dwLev		= pGuildUserSubUser->m_dwLevel;
							pPacketSend.wClass		= pGuildUserSubUser->m_wClass;
							pPacketSend.byRank		= pGuildUserSubUser->m_byRank;
							pPacketSend.dwGuildId	= pGuildUserSubUser->m_dwGuildId;
							pPacketSend.dwUserIndex	= pGuildUserSubUser->m_dwUserIndex;							
							__lstrcpyn(pPacketSend.szNickName, pGuildUserSubUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);
							__lstrcpyn(pPacketSend.szCharacterName, pGuildUserSubUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacketSend, pPacketSend.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}
						lpGuildUserSubNode = lpGuildUserSubNode->pNext;
					}
				}
				else
				{					
					// 나중에 들어온 한명만 //
					WSTC_GUILD_USERINFO pPacketSend;
					memset(pPacketSend.szNickName, 0, sizeof(pPacketSend.szNickName));
					memset(pPacketSend.szCharacterName, 0, sizeof(pPacketSend.szCharacterName));				

					// 월드에 있을 때 //
					pPacketSend.bGuildCount	= 1;
					pPacketSend.byRank		= pUser->m_byRank;
					pPacketSend.dwLev		= packet.dwLev;
					pPacketSend.wClass		= packet.wClass;
					pPacketSend.dwGuildId	= packet.dwGuildId;
					pPacketSend.dwUserIndex	= packet.dwUserIndex;
					__lstrcpyn(pPacketSend.szNickName, pUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);
					__lstrcpyn(pPacketSend.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);

					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPacketSend, pPacketSend.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}

				lpGuildUserNode = lpGuildUserNode->pNext;
			}
		}
		else
		{
			// DB Update //
			char szQuery[0xff]={0,};
									
			memset(szQuery, 0, sizeof(szQuery));
			
			wsprintf(szQuery, "CZP_GUILD_CHANGE 0, 0, %u, '', '', 0, 0", pUser->m_dwUserIndex);							
			g_pDb->THExecuteSQL(szQuery, FALSE, QUERY_TYPE_GUILD_USER, pUser, (BYTE)GAME_DB);

			pUser->m_dwPrevGuildId	= pUser->m_dwGuildId;
			
			wsprintf(szQuery, "CZP_GUILDOUT_PREV %u, %u", pUser->m_dwGuildId, pUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pUser, (BYTE)GAME_DB);

			pUser->GuildIdPoint();

			memset(pUser->m_szGuildName, 0, sizeof(pUser->m_szGuildName));
			memset(pUser->m_szNickName, 0, sizeof(pUser->m_szNickName));
			pUser->m_dwGuildId		= 0;
			pUser->m_byRank			= 0;
			pUser->m_byType			= 0;
			
			pUser->GuildIdChange();			
		}
	}
	
	g_pUserTable->AddCharacterToSearchTable(pUser);	
	
	// DB Update //
	char szQuery[0xff]={0,};
	
	LP_MESSENGER_TABLE lpMessengerTable = (LP_MESSENGER_TABLE)LALAlloc(g_pMessengerTablePool);

	if(lpMessengerTable)
	{
		lpMessengerTable->pMessengerUserHash = new CMessengerUserHash;
		lpMessengerTable->pMessengerUserHash->InitializeHashTable(40, 40);
		g_pMessengerTableHash->Insert(lpMessengerTable, pUser->m_dwUserIndex);

		if(pUser->m_byMessenger>0)
		{
			memset(szQuery, 0, sizeof(szQuery));	
			wsprintf(szQuery
				, "Select Dst_Char_Index, Name, Type, Class From Messenger Where Char_Index = %u", pUser->m_dwUserIndex);	
			g_pDb->THOpenRecord(szQuery, QUERY_TYPE_MESSENGER, (void*)pUser->m_dwConnectionIndex, 32, (BYTE)GAME_DB);
		}		
	}									

#ifdef	GUILD_WAR
	LP_GUILD_LIST lpGuildList = g_pGuildWarHash->GetData(pUser->m_dwGuildId);
		
	if(lpGuildList)
	{
		for(int i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildList->sGuildNode[i].dwGuildId!=0)
			{	
				LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(lpGuildList->sGuildNode[i].dwGuildId);
				
				if(lpGuildTable)
				{
					WSTC_GUILD_LIST pPacket;
					memset(pPacket.szName, 0, sizeof(pPacket.szName));										
					__lstrcpyn(pPacket.szName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
					time(&pPacket.sCurTime);
					pPacket.sTime		= lpGuildList->sGuildNode[i].sTime;
					pPacket.dwGuildId	= lpGuildList->sGuildNode[i].dwGuildId;
					pPacket.wPoint1		= lpGuildList->sGuildNode[i].wPoint1;
					pPacket.wPoint2		= lpGuildList->sGuildNode[i].wPoint2;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}
	}	
#endif
	
	if(!pUser->m_ListPosInWorld || pUser->m_dwCurServerPos != POS_SERVER_WORLD)
	{
		__asm int 3
	}
}


#pragma pack(push, 1)

void QueryMessenger(DBRECEIVEDATA* pResult)
{
	if(!pResult)
		return;

	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (!pUser)	return;

	MESSENGER_USER			sMessengerUser[MAX_MESSENGER_USER];
	LP_MESSENGER_USER		lpMessengerUser;	
	WSTC_MESSENGER_USER		pSendPacket;
	WSTC_MESSENGER_USERINFO	pOnlinePacket;

	LP_MESSENGER_TABLE		lpMessengerTable = g_pMessengerTableHash->GetData(pUser->m_dwUserIndex);

	if(!lpMessengerTable)
		return;
		
	BOOL bChk = FALSE;

	for(DWORD i = 0; i < pResult->Query.select.dwRowCount; i++)
	{
		if(i>=MAX_MESSENGER_USER)
			return;

		memset(&sMessengerUser[i], 0, sizeof(sMessengerUser[i]));
		memcpy(&sMessengerUser[i], pResult->Query.select.pResult+pResult->Query.select.dwRowSize*i, sizeof(MESSENGER_USER));

		if(sMessengerUser[i].wClass==0)
			bChk = TRUE;
		else
		{
			lpMessengerUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);
			memset(lpMessengerUser, 0, sizeof(MESSENGER_USER));
			lpMessengerUser->byType			= sMessengerUser[i].byType; 
			lpMessengerUser->dwUserIndex	= sMessengerUser[i].dwUserIndex;
			lpMessengerUser->wClass			= sMessengerUser[i].wClass;		
			__lstrcpyn(lpMessengerUser->szCharName, sMessengerUser[i].szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);				
			lpMessengerTable->pMessengerUserHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);

			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));			
			pSendPacket.dwUserIndex	= lpMessengerUser->dwUserIndex;
			pSendPacket.wClass		= lpMessengerUser->wClass;		
			__lstrcpyn(pSendPacket.szName, lpMessengerUser->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
			
			CWorldUser* pOnLineUser = g_pUserTable->GetUserInfo(lpMessengerUser->dwUserIndex);

			if(pOnLineUser)		
			{
				pSendPacket.byType		= 0;
				pSendPacket.byStatus	= pOnLineUser->m_byMessengerStatus;
			}
			else
			{
				pSendPacket.byType		= 1;
				pSendPacket.byStatus	= 1;
			}
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			if(pOnLineUser)
			{
				LP_MESSENGER_TABLE lpMessengerSubTable = g_pMessengerTableHash->GetData(pOnLineUser->m_dwUserIndex);
							
				if(lpMessengerSubTable)
				{
					LP_MESSENGER_USER lpMessengerSubUser = lpMessengerSubTable->pMessengerUserHash->GetData(pUser->m_dwUserIndex);

					if(lpMessengerSubUser)
					{
						pOnlinePacket.byType		= 0;
						pOnlinePacket.dwUserIndex	= pUser->m_dwUserIndex;
						g_pNet->SendToUser(pOnLineUser->m_dwConnectionIndex, (char*)&pOnlinePacket, pOnlinePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
				}			
			}
		}				
	}

	if(bChk)
	{
		char szQuery[0xff];
		wsprintf(szQuery, "Delete From Messenger Where Char_Index = %u And Class = 0", pUser->m_dwUserIndex);
		g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pUser, (BYTE)GAME_DB);
	}
}

void QueryTypeMessage(DBRECEIVEDATA* pResult, BYTE byQueryType)
{
	if(pResult->Query.select.dwRowCount==0)
		return;
		
	DWORD dwResult		= (DWORD)*pResult->Query.select.pResult;
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (!pUser)	return;

	if(byQueryType==QUERY_TYPE_MESSAGE)
	{
		WSTC_WSF_CHAT pSendPakcet;				
		pSendPakcet.enCommand	= COMMAND_MESSAGE;	
		pSendPakcet.nFail		= dwResult;
		memset(pSendPakcet.szId, 0, sizeof(pSendPakcet.szId));
		__lstrcpyn(pSendPakcet.szId, pUser->m_szSearchName, MAX_ID_LENGTH);
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPakcet, pSendPakcet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}
	else if(byQueryType==QUERY_TYPE_MESSAGE2)
	{
		WSTC_DSF_CHAT pSendPakcet;
		pSendPakcet.enCommand	= COMMAND_MESSAGE;	
		pSendPakcet.nFail		= dwResult;
		memset(pSendPakcet.szId, 0, sizeof(pSendPakcet.szId));
		__lstrcpyn(pSendPakcet.szId, pUser->m_szSearchName, MAX_ID_LENGTH);
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPakcet, pSendPakcet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}
}

void QueryTypeMessageRQ(DBRECEIVEDATA* pResult)
{
	if(pResult->Query.select.dwRowCount==0)
		return;

	char		szProc[0xff]={0,};
	int			nResult	= pResult->Query.select.dwRowCount;
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);				

	if (!pUser)	return;
	
	SMESSAGE sMessageRQ[10];

	for(int i = 0; i < nResult; i++)
	{
		memcpy(&sMessageRQ[i], pResult->Query.select.pResult+pResult->Query.select.dwRowSize*i, sizeof(SMESSAGE));
	}

	if(nResult==0)
	{
		WSTC_CHAT_RQ pPacket;
		pPacket.nCount	= 0;
		pPacket.nIndex	= 0;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		WSTC_CHAT_RQ pPacket;

		for(int i = 0; i < nResult; i++)
		{	
			memset(pPacket.szId, 0, sizeof(pPacket.szId));
			memset(pPacket.szMessage, 0, sizeof(pPacket.szMessage));

			// modified by deepdark.
			pPacket.wdTime[0] = sMessageRQ[i].sDate.year;
			pPacket.wdTime[1] = sMessageRQ[i].sDate.month;
			pPacket.wdTime[2] = sMessageRQ[i].sDate.day;
			pPacket.wdTime[3] = sMessageRQ[i].sDate.hour;
			pPacket.wdTime[4] = sMessageRQ[i].sDate.minute;
			pPacket.wdTime[5] = sMessageRQ[i].sDate.second;

			pPacket.nCount	= nResult;
			pPacket.nIndex	= i;
			__lstrcpyn(pPacket.szId, sMessageRQ[i].szFromName, MAX_ID_LENGTH);
			__lstrcpyn(pPacket.szMessage, sMessageRQ[i].szMessage, MAX_INPUT_BUFFER_SIZE);

			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}

	// 삭제 //
	wsprintf(szProc, "Delete From MessageTable where [NameTo] = '%s'", pUser->m_szCharacterName);
	g_pDb->THExecuteSQL(szProc, FALSE, QUERY_TYPE_MESSAGEDEL, pUser, (BYTE)GAME_DB);
}


void QueryGuildId(DBRECEIVEDATA* pResult)
{
	char		szProc[0xff]={0,};
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (!pUser)return;			

	LP_GUILD_TABLE		lpGuildAdd=0;	
	WSTC_GUILD_CREATE	pResultPacket;
	SGUILD_ID			sGuildId;

	DUNGEON_DATA_EX* pDungeon;
	pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);

	if(!pDungeon)
	{
		Log(LOG_NORMAL, "Dungeon = NULL(QueryGuildId)");
		return;
	}

	memcpy(&sGuildId, pResult->Query.select.pResult, sizeof(SGUILD_ID));
	
	// 길드 해쉬에 추가 //
	lpGuildAdd = (LP_GUILD_TABLE)LALAlloc(g_pGuildTablePool);	
	memset(lpGuildAdd, 0, sizeof(GUILD_TABLE));

	// Off-Line Guild User Hash //
	lpGuildAdd->pGuildOfflineHash = new CGuildOfflineHash;
	lpGuildAdd->pGuildOfflineHash->InitializeHashTable(100, 100);

	lpGuildAdd->pGuildUserHash = new CGuildUserTableHash;
	lpGuildAdd->pGuildUserHash->InitializeHashTable(100, 100);			
	
	lpGuildAdd->sGuild.byMember[0]	= 1;
	lpGuildAdd->sGuild.byMember[1]	= 0;
	lpGuildAdd->sGuild.byMember[2]	= 0;
	lpGuildAdd->sGuild.byMember[3]	= 0;
	lpGuildAdd->sGuild.byCount		= 1;
	lpGuildAdd->sGuild.byType		= sGuildId.byType;		
	lpGuildAdd->sGuild.dwGuildId	= sGuildId.dwGuildId;
	lpGuildAdd->sGuild.wLevel		= 1;
	__lstrcpyn(lpGuildAdd->sGuild.szGuildName, sGuildId.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);

	// modified by deepdark.
	for( int i = 0 ; i < GUILD::GUILD_RANK_COUNT_MAX ; ++i )
	{
		__lstrcpyn(
			lpGuildAdd->sGuild.szRankName[i], 
			g_szDefaultGuildRank[i], 
			MAX_RANK_REAL_NICKNAME_LENGTH
			);
	}

	pUser->m_byRank		= 1;
	pUser->m_byType		= sGuildId.byType;		
	pUser->m_dwGuildId	= sGuildId.dwGuildId;	
	__lstrcpyn(pUser->m_szGuildName, sGuildId.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
	memset(pUser->m_szNickName, 0, MAX_GUILD_NAME_LENGTH);
	//__lstrcpyn(pUser->m_szNickName, "");

	lpGuildAdd->pGuildUserHash->Insert(pUser, pUser->m_dwUserIndex);
	g_pGuildTableHash->Insert(lpGuildAdd, lpGuildAdd->sGuild.dwGuildId);

	pUser->GuildIdChange();

#ifdef GUILD_WAR

	// GuildList Insert //	
	wsprintf(szProc, "CZP_GUILDLIST_INSERT %u", lpGuildAdd->sGuild.dwGuildId);
	g_pDb->THExecuteSQL(szProc, FALSE, NULL, NULL, (BYTE)GAME_DB); // 리턴값 필요없어..!! BY DEEPDARK.

#endif
	
	LP_GUILDWAR_RANK lpGuildWarRankNew = (LP_GUILDWAR_RANK)LALAlloc(g_pRankListPool);

	if(g_pRankList->GetCount()==0)	// 처음 생성한 길드을 1등 길드로 만든다.
	{
		WSTDS_GUILD_RANK pRankPacket;
		pRankPacket.dwGuildId = lpGuildAdd->sGuild.dwGuildId;												
		g_pServerTable->BroadCast((char*)&pRankPacket, pRankPacket.GetPacketSize());
		
		if(lpGuildAdd)
		{		
			if(pUser)
			{
				WSTDS_GUILDUSER_RANK pGuildUserRankPacket;
				pGuildUserRankPacket.dwUserIndex	= pUser->m_dwUserIndex;
				pGuildUserRankPacket.dwGuildId		= pUser->m_dwGuildId;
				pGuildUserRankPacket.byGuildFlag	= 1;

				DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);

				if(pDungeon)
					g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pGuildUserRankPacket, pGuildUserRankPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
			}
		}
	}

	memset(lpGuildWarRankNew, 0, sizeof(GUILDWAR_RANK));				
	lpGuildWarRankNew->dwExp		= 0;
	lpGuildWarRankNew->dwPoint		= 0;
	lpGuildWarRankNew->dwGuildId	= lpGuildAdd->sGuild.dwGuildId;
	__lstrcpyn(lpGuildWarRankNew->szName, lpGuildAdd->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
	g_pRankList->AddTail(lpGuildWarRankNew);
	
	// 길드 정보 //
	WSTC_DGGUILD_USER pDgGuildPacket;
	
	pDgGuildPacket.byType		= sGuildId.byType;
	pDgGuildPacket.byCount		= lpGuildAdd->sGuild.byCount;
	pDgGuildPacket.dwGuildId	= sGuildId.dwGuildId;	
	pDgGuildPacket.wLevel		= lpGuildAdd->sGuild.wLevel;

	memset(pDgGuildPacket.szGuildName, 0, sizeof(pDgGuildPacket.szGuildName));

	for( int i = 0 ; i < GUILD::GUILD_RANK_COUNT_MAX ; ++i )
	{
		::ZeroMemory( 
			pDgGuildPacket.szRankName[i], 
			sizeof(pDgGuildPacket.szRankName[i])
			);
	}

	__lstrcpyn(pDgGuildPacket.szGuildName, sGuildId.szGuildName, MAX_GUILD_NAME_LENGTH);
	for( int i= 0 ; i < GUILD::GUILD_RANK_COUNT_MAX ; ++i )
	{
		__lstrcpyn(
			pDgGuildPacket.szRankName[i], 
			lpGuildAdd->sGuild.szRankName[i], 
			MAX_RANK_REAL_NICKNAME_LENGTH
			);
	}
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pDgGuildPacket, pDgGuildPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	

	// 유저에게 길드 리스트 보내기 //
	WSTC_DGGUILD_USERINFO pPacketSend;
	memset(pPacketSend.szNickName, 0, sizeof(pPacketSend.szNickName));
	memset(pPacketSend.szCharacterName,0, sizeof(pPacketSend.szCharacterName));
	
	pPacketSend.byRank		= pUser->m_byRank;
	pPacketSend.dwLev		= pUser->m_dwLevel;
	pPacketSend.wClass		= pUser->m_wClass;
	pPacketSend.dwGuildId	= pUser->m_dwGuildId;
	pPacketSend.dwUserIndex	= pUser->m_dwUserIndex;
	__lstrcpyn(pPacketSend.szNickName, pUser->m_szNickName, MAX_CHARACTER_NAME_LENGTH);
	__lstrcpyn(pPacketSend.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_LENGTH);
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacketSend, pPacketSend.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	


	// 던전 서버에 보내기 //		
	WSTDS_GUILD_INFO pServerPacket;
	memset(pServerPacket.szGuildName, 0, sizeof(pServerPacket.szGuildName));
	memset(pServerPacket.szNickName, 0, sizeof(pServerPacket.szNickName));
	pServerPacket.dwUserIndex	= pUser->m_dwUserIndex;
	pServerPacket.dwGuildId		= pUser->m_dwGuildId;
	pServerPacket.byRank		= pUser->m_byRank;
	pServerPacket.byType		= pUser->m_byType;
	__lstrcpyn(pServerPacket.szGuildName, pUser->m_szGuildName, MAX_GUILD_NAME_LENGTH);
 	__lstrcpyn(pServerPacket.szNickName, pUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);		
	g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pServerPacket, pServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
			
	// 유저에게 쿼리하기 //		
	wsprintf(szProc, "CZP_GUILD_CHANGE 0, %u, %u, '%s', '%s', %u, %u", 
		pUser->m_dwGuildId, pUser->m_dwUserIndex, pUser->m_szGuildName, pUser->m_szNickName, pUser->m_byType, pUser->m_byRank);	
	g_pDb->THExecuteSQL(szProc, TRUE, QUERY_TYPE_GUILD_USER, pUser, (BYTE)GAME_DB);		

	wsprintf(szProc, "Insert into GuildUserInfo (Char_Index, GuildId) VALUES(%u, %u)", pUser->m_dwUserIndex, pUser->m_dwGuildId);
	g_pDb->THExecuteSQL(szProc, TRUE, QUERY_TYPE_GUILDUSER_DELETE, NULL, (BYTE)GAME_DB);

	wsprintf(szProc, "Update mapinfo set Manager='%s', OwnerGuildNum= %d where OwnerIndex = %u", (strlen(pUser->m_szGuildName) ? pUser->m_szGuildName : "---"), pUser->m_dwGuildId, pUser->m_dwUserIndex);	
	g_pDb->THExecuteSQL(szProc, TRUE, QUERY_TYPE_OWNER, (void*)pUser->m_dwConnectionIndex, (BYTE)GAME_DB);	

	wsprintf(szProc, "CZP_GUILDJOIN_TIME %u", pUser->m_dwUserIndex);
	g_pDb->THExecuteSQL(szProc, FALSE, NULL, pUser, (BYTE)GAME_DB);
	
	pResultPacket.bCreate	= TRUE;
	pResultPacket.bType		= sGuildId.byType;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pResultPacket, pResultPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void QueryGuilChk(DBRECEIVEDATA* pResult)
{
	int			nCount	= pResult->Query.select.dwRowCount;
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);
	if (!pUser)	return;
	GUILD_CHK_TEMP	sGuildChkTemp[4];

	for(int i = 0; i < nCount; i++)
	{
		memset(&sGuildChkTemp[i], 0, sizeof(sGuildChkTemp[i]));
		memcpy(&sGuildChkTemp[i], pResult->Query.select.pResult+pResult->Query.select.dwRowSize*i, sizeof(sGuildChkTemp[i]));

		pUser->m_sGuildChk[i].bType	= sGuildChkTemp[i].bType;
		memset(pUser->m_sGuildChk[i].szName, 0, sizeof(pUser->m_sGuildChk[i].szName));
		__lstrcpyn(pUser->m_sGuildChk[i].szName, sGuildChkTemp[i].szName, MAX_CHARACTER_NAME_LENGTH);

		if(__strcmp(pUser->m_szCharacterName, sGuildChkTemp[i].szName)==0)
			pUser->m_dwUserIndex = sGuildChkTemp[i].dwUserIndex;
	}
}

void QueryTypeWorldChk(DBRECEIVEDATA* pResult)
{
	if(pResult->Query.select.dwRowCount == 0)
		return;

	ACCEPT_USER* pAccept = (ACCEPT_USER*)pResult->pData;

	WORLD_USER_CHK sWorldUserChk;
	memset(&sWorldUserChk, 0, sizeof(sWorldUserChk));
	memcpy(&sWorldUserChk, pResult->Query.select.pResult, sizeof(pResult->Query.select.dwRowSize)*pResult->Query.select.dwRowCount);

	for(DWORD i = 0; i < pResult->Query.select.dwRowCount; i++)
	{
		CWorldUser* pAlreadyUser = g_pUserTable->GetUserInfo(sWorldUserChk.dwUserIndex[i]);
		
		if(pAlreadyUser)
		{
			// 던전서버에서도 확실하게 끊어버리자 
			WSTDS_KICK_USER	KickPacket;
			KickPacket.dwCharIndex = pAlreadyUser->m_dwUserIndex;
			g_pServerTable->BroadCast((char*)&KickPacket, KickPacket.GetPacketSize());
			
			OnDisconnectUser(pAlreadyUser->m_dwConnectionIndex);
			g_pNet->SetUserInfo(pAlreadyUser->m_dwConnectionIndex, NULL);
			g_pNet->CompulsiveDisconnectUser(pAlreadyUser->m_dwConnectionIndex);
			
			Log(LOG_FATAL, "Overlapped User Compulsive Disconnected! (Name:%s)", pAlreadyUser->m_szCharacterName );
			
			pAlreadyUser = g_pUserTable->GetUserInfo(sWorldUserChk.dwUserIndex[i]);
			
			if(pAlreadyUser)
			{
				g_pUserTable->Remove(pAlreadyUser);
				Log(LOG_FATAL, "User Retry Overlapped!! (CmdWorldLogin Function! Name:%s)", pAlreadyUser->m_szCharacterName);
			}

			return;
		}
	}
	
	CWorldUser* pUser = g_pUserTable->AllocNewUser();
	
	pUser->m_dwPropID			= pAccept->dwPropID;		//Member DB의 CHR_LOG_INFO 테이블의 고유값 
	pUser->m_dwUserIndex		= pAccept->dwChar_Index;
	pUser->m_dwConnectionIndex	= pAccept->dwConnection;
	pUser->m_bBillingType		= pAccept->bBillingType;
	pUser->m_dwRemainTime		= pAccept->dwRemainTime;
	pUser->m_BillingEday		= pAccept->BillingEday;
	
	g_pNtos->AddTimer( &(pUser->m_NTOS));
	TimerSync( pUser);
	
	g_pNet->SetUserInfo(pUser->m_dwConnectionIndex, pUser);
	
	pUser->m_dwStatus = USER_STATUS_LOGIN_SUCCESS;
	
	// Agent 서버에게 해당 유저가 이서버(월드서버)에 성공적으로 로그인했다는 정보를 알려준다. 
	WSTAS_USER_WORLD_LOGIN_SUCCESS	packet;
	packet.dwPropID		= pUser->m_dwPropID;
	packet.dwUserIndex	= pUser->m_dwUserIndex;
	g_pNet->SendToServer(AGENTINDEX, (char*)&packet, packet.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

	if(pAccept->bEventFlag == EVENT_NUM_PC_ROOM_EXP_TWICE)
		pUser->m_bConnectGameRoom = 1;		//PC방 접속 유저 

	char szQuery[ 0xff ]={0,};
	wsprintf(szQuery, "EXEC CZP_WORLD_INFO %d", pAccept->dwChar_Index);
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_USER_INFO, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);

	g_pAcceptTable->Remove(pAccept);
}

void QueryTypeGMCheck(DBRECEIVEDATA* pResult)
{
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(!pUser)
	{
		Log(LOG_NORMAL, "WorldUser Pointer is NULL at QueryTypeGMCheck!");
		return;
	}

	if(pResult->Query.select.dwRowCount == 0)
	{
		WSTC_INVALID_GM packet;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
	
	if (NULL == pResult->Query.select.pResult)
	{
		Log(LOG_NORMAL, "pResult->Query.select.pResult is NULL at QueryTypeGMCheck.");
		return;
	}
	
	pUser->m_bUserType = USER_TYPE_NORMAL;
	
	int iOffset = 0;
	
	int i = 0;
	
	for(i = 0; (DWORD)i < pResult->Query.select.dwRowCount; i++)
	{

		iOffset = i * pResult->Query.select.dwRowSize;

		//DB에 등록된 GM IP
		char szRegisteredIP[16]={0,};

		memcpy(szRegisteredIP,
			   pResult->Query.select.pResult + iOffset,
			   pResult->Query.select.dwRowSize);

		//현재 접속한 Client의 IP
		char szIp[16]={0,};	WORD wPort=0;
		g_pNet->GetUserAddress(pUser->m_dwConnectionIndex, szIp, &wPort);

		// GM 판정.
		// IP 체크하는 부분 넣었습니다. 
		// 이제부터 GM 체크 확실히 하겠음.
		if(0 == __strcmp(szIp, szRegisteredIP))
		{
			// GM 이다!
			pUser->m_bUserType	= USER_TYPE_GM;	
			break;			
		}	
	}

	// GM 판정에 대한 결과를 유저에게 보내준다.

	if (pUser->IsGMUser())
	{
		// GM으로 로그인 성공했습니다.
	
		WSTC_GM_LOGIN_SUCCESS packet;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
	}
	else
	{					
		// 어딜 감히! GM도 아니면서. 땍!
		WSTC_INVALID_GM packet;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}		
}

void QueryTypePartyFriend(DBRECEIVEDATA* pResult)
{
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo((DWORD)pResult->pData);

	if(pUser == NULL)
		return;
	if(pResult->Query.select.dwRowCount == 0)
		return;

	WSTC_PARTY_FRIEND pPartyFriend;
	pPartyFriend.dwCount = pResult->Query.select.dwRowCount;
	
	int iOffset = 0;

	for(int i = 0; (DWORD)i < pResult->Query.select.dwRowCount && i < 20; i++)
	{
		iOffset = i * pResult->Query.select.dwRowSize;

		DWORD dwUserIndex;
		memcpy(&dwUserIndex, pResult->Query.select.pResult + iOffset, 4);
		memcpy(&pPartyFriend.pFriend[i].bClass, pResult->Query.select.pResult + iOffset + 4, 1);
		memcpy(pPartyFriend.pFriend[i].szCharacterName, pResult->Query.select.pResult + iOffset + 6, MAX_CHARACTER_NAME_LENGTH);
		
		if(g_pUserTable->GetUserInfo(dwUserIndex))
		{
			pPartyFriend.pFriend[i].bOnline = 0;
		}
		else
		{
			pPartyFriend.pFriend[i].bOnline = 3;
		}
	}

	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPartyFriend, pPartyFriend.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

struct tagQueryTypeLadderInfo
{
	UINT				uMaxPage;
	UINT				uMyPoint;
	UINT				uMyRank;

	GuildLadderInfo		Info;
};
void QueryTypeLadderInfo( DBRECEIVEDATA* pResult )
{
	CWorldUser* pUser = (CWorldUser*)g_pNet->GetUserInfo( reinterpret_cast< DWORD >(pResult->pData) );

	if( NULL == pUser )
		return;

	if( 0 == pResult->Query.select.dwRowCount )
		return;

	if( pResult->Query.select.dwRowSize != sizeof(tagQueryTypeLadderInfo) )
		return;

	tagQueryTypeLadderInfo*	pInfo = (tagQueryTypeLadderInfo*)pResult->Query.select.pResult;

	TEAM_MATCH_PACKET packet;
	packet.wType = CTWS_TM_LADDER_LIST;

	GuildLadderInfoPage&	rPage = packet.Body.m_GuildLadderInfoPage;
	rPage.m_uCurPage = 0;

	rPage.m_uMaxPage = pInfo->uMaxPage;
	rPage.m_uMyGuildLadderPoint = pInfo->uMyPoint;
	rPage.m_uMyGuildLadderRank = pInfo->uMyRank;
	rPage.m_uUseCount = pResult->Query.select.dwRowCount;

	for( UINT uIter = 0 ; uIter < rPage.m_uUseCount ; ++uIter )
	{
		::memcpy( &rPage.m_GuildLadderInfo[uIter], &pInfo->Info, sizeof(GuildLadderInfo) );
		++pInfo;
	}

	if( pUser )
	{
		LP_GUILD_TABLE	lpGuildTable = g_pGuildTableHash->GetData( pUser->m_dwGuildId );		// 신청한 길드

		if( lpGuildTable )
		{
			::memcpy( rPage.m_szMyGuildName, lpGuildTable->sGuild.szGuildName, sizeof(rPage.m_szMyGuildName) );
		}
		else
		{
			::strcpy( rPage.m_szMyGuildName, _T("None") );
			rPage.m_uMyGuildLadderPoint = 0;
			rPage.m_uMyGuildLadderRank = 0;
		}
	}

	g_pNet->SendToUser( pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );		
}
