#include "stdafx.h"
#include "MsgProcForServerGuild.h"
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


void CmdGuildPortalUsed(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_PORTALUSED* pPacket = (WSTDS_GUILD_PORTALUSED*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{
		CWorldUser* pPortalUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwPartalUserIndex);

		if(pPortalUser)
		{
			if(pPortalUser->m_dwGuildId!=pUser->m_dwGuildId)
				return;

			WSTC_GUILD_PORTALUSED pSendPacket;
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pPortalUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pSendPacket.dwGuildId			= pPacket->dwGuildId;
			pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
			pSendPacket.dwPartalUserIndex	= pPacket->dwPartalUserIndex;
			pSendPacket.dwResult			= pPacket->dwResult;
			pSendPacket.byIndex				= pPacket->byIndex;
			pSendPacket.byType				= pPacket->byType;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}
}


void CmdGuildAllPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_ALLPORTAL* pPacket = (WSTDS_GUILD_ALLPORTAL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{
		LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pPacket->dwGuildId);
		
		if(lpGuildTable)			
		{
			ListNode<CWorldUser>* pGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();

			while(pGuildUserNode)
			{
				CWorldUser* pGuildUser = pGuildUserNode->pData;

				if(pGuildUser)
				{
					WSTC_GUILD_PORTAL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					pSendPacket.dwGuildId			= pUser->m_dwGuildId;
					pSendPacket.dwUserIndex			= pUser->m_dwUserIndex;
					pSendPacket.dwPartalUserIndex	= pGuildUser->m_dwUserIndex;
					pSendPacket.byIndex				= pPacket->byIndex;
					pSendPacket.byType				= 0;
					
					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
				}

				pGuildUserNode = pGuildUserNode->pNext;
			}
		}		
	}
}


void CmdGuildPotalFail(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_PORTALFAIL* pPacket = (WSTDS_GUILD_PORTALFAIL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{
		WSTC_GUILD_PORTALFAIL pSendPacket;
		memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
		__lstrcpyn(pSendPacket.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pSendPacket.byResult	= pPacket->byResult;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CmdGuildLevel(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_LEVEL* pPacket = (WSTDS_GUILD_LEVEL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);	

	if(!pUser)
		return;
		
	if(pUser->m_dwGuildId==pPacket->dwGuildId)
	{
		LP_GUILD_TABLE lpGuildTable = (LP_GUILD_TABLE)g_pGuildTableHash->GetData(pUser->m_dwGuildId);

		if(lpGuildTable)
		{
			ListNode<CWorldUser>* lpGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();
			
			while(lpGuildUserPos)
			{
				CWorldUser* pGuildUser = lpGuildUserPos->pData;

				if(!pGuildUser)
				{
					lpGuildUserPos = lpGuildUserPos->pNext;
					continue;
				}
				
				if(pGuildUser->m_dwUserIndex!=pUser->m_dwUserIndex)
				{
					// 메세지 보내기 //
					WSTC_GUILD_LEVEL pLevelPacket;
					pLevelPacket.dwUserIndex	= pUser->m_dwUserIndex;
					pLevelPacket.dwLevel		= pPacket->dwLevel;
					pLevelPacket.dwGuildId		= pPacket->dwGuildId;
					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				else
				{
					// 자기 자신에게 //
					pGuildUser->m_dwLevel	= pPacket->dwLevel;
					
					ListNode<CWorldUser>* lpGuildUserDstPos = lpGuildTable->pGuildUserHash->GetHead();

					while(lpGuildUserDstPos)
					{
						CWorldUser* pGuildDstUser = lpGuildUserPos->pData;

						if(!pGuildDstUser)
						{
							lpGuildUserDstPos = lpGuildUserDstPos->pNext;
							continue;
						}

						if(pGuildDstUser->m_dwUserIndex!=pUser->m_dwUserIndex)
						{
							// 메세지 보내기 //
							WSTC_GUILD_LEVEL pLevelPacket;
							pLevelPacket.dwUserIndex	= pGuildDstUser->m_dwUserIndex;
							pLevelPacket.dwLevel		= pGuildDstUser->m_dwLevel;
							pLevelPacket.dwGuildId		= pPacket->dwGuildId;
							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}

						lpGuildUserDstPos = lpGuildUserDstPos->pNext;
					}
				}

				lpGuildUserPos = lpGuildUserPos->pNext;
			}
		}
	}
}


void CmdGuildWarPoint(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_GUILDWAR_POINT* pPacket = (DSTWS_GUILDWAR_POINT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);
	
	if(!pUser)
		return;

	char szQuery[0xff]={0,};

	LP_GUILD_LIST	lpGuildListTable	= g_pGuildWarHash->GetData(pPacket->dwGuildId);
	LP_GUILD_LIST	lpGuildListTableDst	= g_pGuildWarHash->GetData(pPacket->dwGuildIdDst);
	LP_GUILD_TABLE	lpGuildTable		= g_pGuildTableHash->GetData(pPacket->dwGuildId);
	LP_GUILD_TABLE	lpGuildTableDst		= g_pGuildTableHash->GetData(pPacket->dwGuildIdDst);

	if(!lpGuildListTable)
		return;
	if(!lpGuildListTableDst)
		return;	
	if(!lpGuildTable)
		return;
	if(!lpGuildTableDst)
		return;

	for(int i = 0; i < MAX_GUILD_WAR; i++)
	{
		if(lpGuildListTable->sGuildNode[i].dwGuildId==pPacket->dwGuildIdDst)
		{
			lpGuildListTable->sGuildNode[i].wPoint1 = (WORD)(lpGuildListTable->sGuildNode[i].wPoint1 + pPacket->byPoint);

			WSTC_GUILDWAR_POINT pPointPacket;
			pPointPacket.dwGuildId	= pPacket->dwGuildIdDst;
			pPointPacket.wPoint1	= lpGuildListTable->sGuildNode[i].wPoint1;
			pPointPacket.wPoint2	= lpGuildListTable->sGuildNode[i].wPoint2;
						
			switch(pPacket->byPoint)
			{
			case 1:
				pPointPacket.byType	= 0;
				break;
			case 3:
				pPointPacket.byType	= 2;
				break;
			case 10:
				pPointPacket.byType	= 4;
				break;
			case 100:
				pPointPacket.byType	= 8;
				break;
			}

			ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();

			while(pGuildUserPos)
			{
				CWorldUser* pGuildUser = pGuildUserPos->pData;

				if(pGuildUser)
					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPointPacket, pPointPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

				pGuildUserPos = pGuildUserPos->pNext;
			}
			
			// 디비 업데이트 //
			DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);

			if(!pBinding)
			{
				Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
				return;
			}

			GUILD_NODE sGuildNode[MAX_GUILD_WAR];	
			memset(szQuery, 0, sizeof(szQuery));
			memset(sGuildNode, 0, sizeof(sGuildNode));
			memcpy(sGuildNode, lpGuildListTable->sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR);

			pBinding[0].obValue		= 0;
			pBinding[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
			pBinding[0].wType		= DBTYPE_BYTES;					
			
			wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTable->sGuild.dwGuildId);
			g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR, 1, FALSE, 0, NULL, (BYTE)GAME_DB);			
			
			// 길드전 메세지 //
			if(pPacket->byPoint==1 || pPacket->byPoint==3 || pPacket->byPoint==10)
			{
				WSTC_GUILDWAR_MESSAGE pMessage;
				memset(pMessage.szGuildName, 0, sizeof(pMessage.szGuildName));
				memset(pMessage.szName, 0, sizeof(pMessage.szName));
				memset(pMessage.szNameDst, 0, sizeof(pMessage.szNameDst));
				__lstrcpyn(pMessage.szGuildName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
								
				pMessage.byType			= 3;
				pMessage.wDungeonID	= pPacket->wDungeonID;
				pMessage.byLayerIndex	= pPacket->byLayerIndex;

				CWorldUser* pAttackUser		= g_pUserTable->GetUserInfo(pPacket->dwUserIndexAttack);
				CWorldUser* pDefenceUser	= g_pUserTable->GetUserInfo(pPacket->dwUserIndexDefence);

				if(pAttackUser && pDefenceUser)
				{
					__lstrcpyn(pMessage.szName, pAttackUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pMessage.szNameDst, pDefenceUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
				
					pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();

					while(pGuildUserPos)
					{
						CWorldUser* pGuildUser = pGuildUserPos->pData;

						if(pGuildUser)
							g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pMessage, pMessage.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
						
						pGuildUserPos = pGuildUserPos->pNext;
					}
				}							
			}
		}

		if(lpGuildListTableDst->sGuildNode[i].dwGuildId==pPacket->dwGuildId)
		{
			lpGuildListTableDst->sGuildNode[i].wPoint2 = (WORD)(lpGuildListTableDst->sGuildNode[i].wPoint2 + pPacket->byPoint);

			WSTC_GUILDWAR_POINT pPointPacket;
			pPointPacket.dwGuildId	= pPacket->dwGuildId;
			pPointPacket.wPoint1	= lpGuildListTableDst->sGuildNode[i].wPoint1;
			pPointPacket.wPoint2	= lpGuildListTableDst->sGuildNode[i].wPoint2;

			switch(pPacket->byPoint)
			{
			case 1:
				pPointPacket.byType	= 1;
				break;
			case 3:
				pPointPacket.byType	= 3;
				break;
			case 10:
				pPointPacket.byType	= 5;
				break;
			case 100:
				pPointPacket.byType	= 9;
				break;
			}

			ListNode<CWorldUser>* pGuildUserPos = lpGuildTableDst->pGuildUserHash->GetHead();

			while(pGuildUserPos)
			{
				CWorldUser* pGuildUser = pGuildUserPos->pData;

				if(pGuildUser)
					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPointPacket, pPointPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				
				pGuildUserPos = pGuildUserPos->pNext;
			}

			// 디비 업데이트 //
			DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);

			if(!pBinding)
			{
				Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
				return;
			}

			GUILD_NODE sGuildNode[MAX_GUILD_WAR];	
			memset(szQuery, 0, sizeof(szQuery));
			memset(sGuildNode, 0, sizeof(sGuildNode));
			memcpy(sGuildNode, lpGuildListTableDst->sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR);

			pBinding[0].obValue		= 0;
			pBinding[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
			pBinding[0].wType		= DBTYPE_BYTES;					
			
			wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTableDst->sGuild.dwGuildId);
			g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR, 1, FALSE, 0, NULL, (BYTE)GAME_DB);			

			// 길드전 메세지 //
			if(pPacket->byPoint==1 || pPacket->byPoint==3 || pPacket->byPoint==10)
			{
				WSTC_GUILDWAR_MESSAGE pMessage;
				memset(pMessage.szGuildName, 0, sizeof(pMessage.szGuildName));
				memset(pMessage.szName, 0, sizeof(pMessage.szName));
				memset(pMessage.szNameDst, 0, sizeof(pMessage.szNameDst));
				__lstrcpyn(pMessage.szGuildName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
								
				pMessage.byType			= 2;
				pMessage.wDungeonID	= pPacket->wDungeonID;
				pMessage.byLayerIndex	= pPacket->byLayerIndex;

				CWorldUser* pAttackUser		= g_pUserTable->GetUserInfo(pPacket->dwUserIndexAttack);
				CWorldUser* pDefenceUser	= g_pUserTable->GetUserInfo(pPacket->dwUserIndexDefence);

				if(pAttackUser && pDefenceUser)
				{
					__lstrcpyn(pMessage.szName, pAttackUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pMessage.szNameDst, pDefenceUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
				
					pGuildUserPos = lpGuildTableDst->pGuildUserHash->GetHead();

					while(pGuildUserPos)
					{
						CWorldUser* pGuildUser = pGuildUserPos->pData;

						if(pGuildUser)
							g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pMessage, pMessage.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
						
						pGuildUserPos = pGuildUserPos->pNext;
					}
				}								
			}
		}
	}		
}


void CmdGuildUserInfoRt(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_GUILDUSER_INFO* pPacket = (DSTWS_GUILDUSER_INFO*)pMsg;
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

		WSTC_GUILDUSER_INFO pSendPacket;
		memset(pSendPacket.szCharName, 0, sizeof(pSendPacket.szCharName));
		__lstrcpyn(pSendPacket.szCharName, pGuildUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pSendPacket.dwUserIndex		= pPacket->dwUserIndexRv;
		pSendPacket.wDungeonID		= pPacket->wDungeonID;
		pSendPacket.byLayerIndex	= pPacket->byLayerIndex;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}