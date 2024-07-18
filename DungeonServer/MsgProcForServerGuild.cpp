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


void CmdGuildMsg(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_MSG* pPacket = (WSTDS_GUILD_MSG*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	
	if(pUser)
	{		
		if(g_pGuilRankId==pUser->m_dwGuildId)
		{
			DSTC_GUILDUSER_FLAG pFlagPacket;
			pFlagPacket.dwGuildId	= pUser->m_dwGuildId;
			pFlagPacket.dwUserIndex	= pUser->GetID();
			pFlagPacket.byGuildFlag		= 0;
			
			if(pUser->GetCurDungeon())
			{
				CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();

				if(pLayer)
				{
					pLayer->BroadCastSectionMsg((char*)&pFlagPacket, pFlagPacket.GetPacketSize()
					, pUser->GetPrevSectionNum());
				}
			}
		}

		memset(pUser->m_szGuildName, 0, sizeof(pUser->m_szGuildName));
		memset(pUser->m_szNickName, 0, sizeof(pUser->m_szNickName));
		pUser->SetGuildID(0);
		pUser->m_byRank				= 0;
		pUser->m_byGroupType		= 0;
		
		CDungeonLayer*	pLayer;

		if(!pUser->GetCurDungeon())
			return;

		pLayer = pUser->GetCurDungeonLayer();

		if(!pLayer)
			return;

		if (pUser->GetCurDungeon()->GetDungeonDataEx()->m_bSiege )
		{
			pUser->SetAttackMode(pUser->GetCurDungeon()->GetAttackMode(pUser));
			
			// 배신자 안녕ㅡ.ㅡ
			if(	pUser->GetCurLayerIndex() && !pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] && pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
			{
				pUser->GetCurDungeon()->GotoLobbyUser(pUser);
			}		
		}

		DSTC_GUILD_INFO pSendPacket;
		memset(pSendPacket.szNickName, 0, sizeof(pSendPacket.szNickName));
		__lstrcpyn(pSendPacket.szNickName, pUser->m_szNickName, MAX_NICK_REAL_LENGTH);
		pSendPacket.byRank		= pUser->m_byRank;
		pSendPacket.dwGuildId	= pUser->m_dwGuildId;
		pSendPacket.dwUserIndex	= pUser->GetID();		
		pLayer->BroadCastSectionMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), pUser->GetPrevSectionNum());
	}
}


void CmdServerGuildFail(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_FAIL* pPacket = (WSTDS_GUILD_FAIL*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(pUser)
	{
		CItem cItem;
		memset(&cItem, NULL, sizeof(CItem));

		if(pPacket->byType==__GCTYPE_GUILD__)
			CreateItem(&cItem, ITEM_GUILD_INDEX, 1);
		else if(pPacket->byType==__GCTYPE_CLEN__)
			CreateItem(&cItem, ITEM_CLEN_INDEX, 1);

		Insert_SmallInvItem(pUser, &cItem, 0, 1, FALSE);

		DSTC_GUILD_FAIL pGuildFail;
		memcpy(&pGuildFail.cItem, &cItem, sizeof(CItem));
		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pGuildFail, pGuildFail.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);				
	}
}


void CmdServerGuildInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_INFO* pPacket = (WSTDS_GUILD_INFO*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(pUser)
	{
		pUser->SetGuildID(pPacket->dwGuildId);
		pUser->m_byGroupType		= pPacket->byType;
		pUser->m_byRank		= pPacket->byRank;
		__lstrcpyn(pUser->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_LENGTH);
		__lstrcpyn(pUser->m_szNickName, pPacket->szNickName, MAX_GUILD_NAME_LENGTH);

		CDungeonLayer*	pLayer;

		if(!pUser->GetCurDungeon())
			return;

		pLayer = pUser->GetCurDungeonLayer();

		if(!pLayer)
			return;

		if (pUser->GetCurDungeon()->GetDungeonDataEx()->m_bSiege )
		{
			pUser->SetAttackMode(pUser->GetCurDungeon()->GetAttackMode(pUser));
			
			// 배신자 안녕ㅡ.ㅡ
			if(	pUser->GetCurLayerIndex() && !pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY]	&& pUser->GetAttackMode() == ATTACK_MODE_OFFENSE )
			{
				pUser->GetCurDungeon()->GotoLobbyUser(pUser);
			}
		}

		DSTC_GUILD_INFO pSendPacket;
		memset(pSendPacket.szNickName, 0, sizeof(pSendPacket.szNickName));
		__lstrcpyn(pSendPacket.szNickName, pUser->m_szNickName, MAX_NICK_REAL_LENGTH);
		pSendPacket.byRank		= pUser->m_byRank;
		pSendPacket.dwGuildId	= pUser->m_dwGuildId;
		pSendPacket.dwUserIndex	= pUser->GetID();
		
		pLayer->BroadCastSectionMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), pUser->GetPrevSectionNum());
	}	
}


void CmdGuildUserInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILDUSER_INFO* pPacket = (WSTDS_GUILDUSER_INFO*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndexRv);

	if(pUser)
	{
		if(!pUser->GetCurDungeon())
			return;

		DSTWS_GUILDUSER_INFO pSendPacket;		
		pSendPacket.dwUserIndex		= pPacket->dwUserIndex;
		pSendPacket.dwUserIndexRv	= pPacket->dwUserIndexRv;
		pSendPacket.wDungeonID		= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
		pSendPacket.byLayerIndex	= pUser->GetCurLayerIndex();
		g_pNet->SendToServer(dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CmdGuildList(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_LIST* pPacket = (WSTDS_GUILD_LIST*)pMsg;	
	
	GUILD_NODE* pGuildNode = (GUILD_NODE*)LALAlloc(g_pGuildListPool);

	if(pGuildNode)
	{
		
		if(pPacket->dwGuildId>pPacket->dwDstGuildId)
		{
			pGuildNode->IdStruct.dwHiId	= pPacket->dwGuildId;
			pGuildNode->IdStruct.dwLowId	= pPacket->dwDstGuildId;
		}
		else
		{
			pGuildNode->IdStruct.dwHiId	= pPacket->dwDstGuildId;
			pGuildNode->IdStruct.dwLowId	= pPacket->dwGuildId;			
		}
		
		g_pGuildListHash->Insert(pGuildNode, pGuildNode->i64Id);
	}	
}


void CmdGuildWarDelete(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILDWAR_DELETE* pPacket = (WSTDS_GUILDWAR_DELETE*)pMsg;	

	GUILD_NODE sGuildNode;

	if(pPacket->dwGuildId>pPacket->dwGuildIdDst)
	{
		sGuildNode.IdStruct.dwHiId	= pPacket->dwGuildId;
		sGuildNode.IdStruct.dwLowId	= pPacket->dwGuildIdDst;
	}
	else
	{
		sGuildNode.IdStruct.dwHiId	= pPacket->dwGuildIdDst;
		sGuildNode.IdStruct.dwLowId	= pPacket->dwGuildId;			
	}

	GUILD_NODE* lpGuildNode = g_pGuildListHash->GetData(sGuildNode.i64Id);

	if(lpGuildNode)
	{
		g_pGuildListHash->Delete(lpGuildNode, lpGuildNode->i64Id, TRUE);
		memset(lpGuildNode, 0, sizeof(GUILD_NODE));
		lpGuildNode = NULL;
	}
}


void CmdGuildWarRank(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILD_RANK* pPacket = (WSTDS_GUILD_RANK*)pMsg;

	if(pPacket->dwGuildId==g_pGuilRankId)
		return;

	// 1등 길드 변경 //	
	DSTC_GUILD_FLAG pGuildPacket;
	pGuildPacket.dwGuildId		= pPacket->dwGuildId;
	pGuildPacket.dwPrevGuildId	= g_pGuilRankId;
	pGuildPacket.byGuildFlag	= 1;
	g_pDungeonTable->BroadCast((char*)&pGuildPacket, pGuildPacket.GetPacketSize());

	g_pGuilRankId = pPacket->dwGuildId;			
}


void CmdGuildWarUserRank(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILDUSER_RANK* pPacket = (WSTDS_GUILDUSER_RANK*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(pUser)
	{
		pUser->m_byGuildFlag = pPacket->byGuildFlag;

		if(pPacket->byChk==1)
		{
			DSTC_GUILDUSER_FLAG pFlagPacket;
			pFlagPacket.dwGuildId	= pPacket->dwGuildId;
			pFlagPacket.dwUserIndex	= pPacket->dwUserIndex;
			pFlagPacket.byGuildFlag	= pPacket->byGuildFlag;
			
			if(pUser->GetCurDungeon())
			{
				CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();

				if(pLayer)
					pLayer->BroadCastSectionMsg((char*)&pFlagPacket, pFlagPacket.GetPacketSize(), pUser->GetPrevSectionNum());
			}
		}		
	}
}


void CmdGuildWarMessage(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_GUILDWAR_MESSAGE* pPacket = (WSTDS_GUILDWAR_MESSAGE*)pMsg;

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if(pDungeon)
	{
		DSTC_GUILD_WAR_MESSAGE pMessagePacket;
		memset(pMessagePacket.szGuildName, 0, sizeof(pMessagePacket.szGuildName));
		memset(pMessagePacket.szGuildNameDst, 0, sizeof(pMessagePacket.szGuildNameDst));
		__lstrcpyn(pMessagePacket.szGuildName, pPacket->szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
		__lstrcpyn(pMessagePacket.szGuildNameDst, pPacket->szGuildNameDst, MAX_GUILD_REAL_NAME_LENGTH);
		pMessagePacket.byType = pPacket->byType;
		pDungeon->BroadCast((char*)&pMessagePacket, pMessagePacket.GetPacketSize());
	}
}


void CmdRefreshDungeonOwnerGuild(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_REFRESH_DUNGEON_OWNER_GUILD* pPacket = (WSTDS_REFRESH_DUNGEON_OWNER_GUILD*)pMsg;

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if (pDungeon)
	{
		DUNGEON_DATA_EX* pDungeonEx = pDungeon->GetDungeonDataEx();
		pDungeonEx->m_dwOwnerGuildNum = pPacket->dwGuildID;

		if (pDungeonEx->m_bSiege)
		{		
			ListNode<CUser>* pnode = pDungeon->GetGuildUserHash()->GetHead();

			while(pnode)
			{
				pnode->pData->SetAttackMode(pDungeon->GetAttackMode(pnode->pData));

				// 배신자 안녕ㅡ.ㅡ
				if (pnode->pData->GetCurLayerIndex() && !pnode->pData->m_dwTemp[USER_TEMP_FLAG_LOBBY] && pnode->pData->GetAttackMode() == ATTACK_MODE_OFFENSE && pnode->pData->GetCurLayerIndex())
				{
					pnode->pData->GetCurDungeon()->GotoLobbyUser(pnode->pData);
				}

				pnode = pnode->pNext;
			}
		}		
	}
}