#include "stdafx.h"
#include "Guild.h"
#include <afxdisp.h>
#include "DungeonTable.h"
#include "WorldUserTable.h"
#include "ServerFunction.h"
#include "GlobalDefine.h"
#include <memory.h>
#include "GuildHash.h"

char g_szGuildBattleText[MAX_GUILD_TEXT][0xff];


BOOL IsGuildWar(DWORD dwOffenceId, DWORD dwDefenseID)
{
	LP_GUILD_LIST lpGuildListTable = g_pGuildWarHash->GetData(dwOffenceId);

	if(lpGuildListTable)
	{
		for(int i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildListTable->sGuildNode[i].dwGuildId==dwDefenseID)
				return TRUE;
		}		
	}		
	return FALSE;
}

void CmdGuildUserDel(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_GUILD_USERDEL* pPacket = (CTWS_GUILD_USERDEL*)pMsg;
	char szQuery[0xff] = {0,};

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_USER_DELETE	pSendPacket;	
	
	if(pUser->m_byRank==1 && pUser->m_dwUserIndex==pPacket->dwUserIndex)
		return;	

	if(pUser->m_byRank==1) // 추방!!
		pSendPacket.byChk = 1;
	else 
		pSendPacket.byChk = 0;
				
	LP_GUILD_TABLE lpGuildTable	= g_pGuildTableHash->GetData(pUser->m_dwGuildId);
					
	if(!lpGuildTable)
		return;
	if(!lpGuildTable->pGuildOfflineHash)
		return;
	if(!lpGuildTable->pGuildUserHash)
		return;			

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
		if(pGuildUser->m_dwUserIndex==pPacket->dwUserIndex)
		{
			if(pGuildUser->m_dwGuildId!=pUser->m_dwGuildId)				
				return;
			if(lpGuildTable->sGuild.byMember[pGuildUser->m_byRank-1]==0)
				return;
			
			if(pUser->m_dwUserIndex==pGuildUser->m_dwUserIndex)
			{
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime);
			
				COleDateTime PrevTime(pUser->m_sGuildJoinTime.year, pUser->m_sGuildJoinTime.month, pUser->m_sGuildJoinTime.day, pUser->m_sGuildJoinTime.hour, pUser->m_sGuildJoinTime.minute, pUser->m_sGuildJoinTime.second);
			
				if(pUser->m_sGuildJoinTime.year==0)	
					return;
				if(pUser->m_sGuildJoinTime.day==0)
					return;
				if(pUser->m_sGuildJoinTime.month==0)
					return;

				COleDateTime CurTime(SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
			
				COleDateTimeSpan ts;// 시간이 -로 되어 오류가 발생한다. 수정 김영대 2004.02.17
				if ( PrevTime.GetHour() < 0)
					ts = 0;	
				else
					ts = CurTime - PrevTime;

				DWORD dwDay = ts.GetDays();

				if(dwDay<3)
				{
					// 에러 패킷 //
					WSTC_GUILD_ERROR pErrorPacket;
					pErrorPacket.byError	= 2;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
			}

			lpGuildTable->sGuild.byCount -= 1;

			switch(pGuildUser->m_byRank)
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

			// 삭제하고 메세지 보내기 //					
			lpGuildTable->pGuildUserHash->Delete(pGuildUser, pGuildUser->m_dwUserIndex, TRUE);
			
			pGuildUser->m_dwPrevGuildId	= pGuildUser->m_dwGuildId;
			
			wsprintf(szQuery, "CZP_GUILDOUT_PREV %u, %u", pGuildUser->m_dwGuildId, pGuildUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pGuildUser, (BYTE)GAME_DB);
				
			pGuildUser->GuildIdPoint();

			// User 정보 설정 //
			memset(pGuildUser->m_szGuildName, 0, sizeof(pGuildUser->m_szGuildName));
			memset(pGuildUser->m_szNickName, 0, sizeof(pGuildUser->m_szNickName));
			pGuildUser->m_byType		= 0;
			pGuildUser->m_byRank		= 0;	
			pGuildUser->m_dwGuildId		= 0;

			pGuildUser->GuildIdChange();

			pSendPacket.dwUserIndex = pPacket->dwUserIndex; // by deepdark... 
			g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			
			// 서버로 메세지 보내기 //			
			DUNGEON_DATA_EX* pDungeon;
			pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pGuildUser->m_dwDungeonID);	

			if(pDungeon)
			{
				WSTDS_GUILD_MSG	pGuildMsg;
				pGuildMsg.dwUserIndex	= pGuildUser->m_dwUserIndex;
				g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pGuildMsg, pGuildMsg.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
			
			// DB 쿼리 //
			wsprintf(szQuery, "CZP_GUILD_CHANGE 0, 0, %u, '', '', 0, 0", pGuildUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, pGuildUser, (BYTE)GAME_DB);

			wsprintf(szQuery, "Delete From GuildUserInfo Where Char_Index = %u ", pGuildUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILDUSER_DELETE, pGuildUser, (BYTE)GAME_DB);				
			
			wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pUser->m_dwGuildId);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, pUser, (BYTE)GAME_DB);

			memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
			__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pSendPacket.dwGuildId	= pPacket->dwGuildId;
			pSendPacket.dwUserIndex	= pPacket->dwUserIndex;
			pSendPacket.byMember[0]	= lpGuildTable->sGuild.byMember[0];
			pSendPacket.byMember[1]	= lpGuildTable->sGuild.byMember[1];
			pSendPacket.byMember[2]	= lpGuildTable->sGuild.byMember[2];
			pSendPacket.byMember[3]	= lpGuildTable->sGuild.byMember[3];

			ListNode<CWorldUser>* lpGuildUserSubNode = lpGuildTable->pGuildUserHash->GetHead();
							
			while(lpGuildUserSubNode)				
			{					
				CWorldUser* pGuildSubUser = lpGuildUserSubNode->pData;

				if(pGuildSubUser)
					g_pNet->SendToUser(pGuildSubUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

				lpGuildUserSubNode = lpGuildUserSubNode->pNext;
			}
			return;
		}	
		
		lpGuildUserNode = lpGuildUserNode->pNext;
	}

	LP_GUILD_OFFLINE_USER lpOfflineUser = lpGuildTable->pGuildOfflineHash->GetData(pPacket->dwUserIndex);
	
	if(lpOfflineUser)
	{
		if(lpGuildTable->sGuild.byMember[lpOfflineUser->byRank-1]==0)
			return;

		WSTC_GUILD_OFFLINEUSER_DELETE pOffLineDeletePacket;
		pOffLineDeletePacket.dwUserIndex = lpOfflineUser->dwUserIndex;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pOffLineDeletePacket, pOffLineDeletePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		
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

		WSTC_GUILD_COUNT pSendPacket;
		pSendPacket.byCount		= lpGuildTable->sGuild.byCount;
		pSendPacket.byMember[0]	= lpGuildTable->sGuild.byMember[0];
		pSendPacket.byMember[1]	= lpGuildTable->sGuild.byMember[1];
		pSendPacket.byMember[2]	= lpGuildTable->sGuild.byMember[2];
		pSendPacket.byMember[3]	= lpGuildTable->sGuild.byMember[3];
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	

		wsprintf(szQuery, "CZP_GUILDOUT_PREV %u, %u", pPacket->dwGuildId, lpOfflineUser->dwUserIndex);
		g_pDb->THExecuteSQL(szQuery, FALSE, NULL, NULL, (BYTE)GAME_DB);

		// DB 쿼리				
		wsprintf(szQuery, "CZP_GUILD_CHANGE 0, 0, %u, '', '', 0, 0", lpOfflineUser->dwUserIndex);		
		g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, pUser, (BYTE)GAME_DB);

		wsprintf(szQuery, "Delete From GuildUserInfo Where Char_Index = %u ", lpOfflineUser->dwUserIndex);
		g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILDUSER_DELETE, pUser, (BYTE)GAME_DB);		
		
		wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pUser->m_dwGuildId);
		g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, pUser, (BYTE)GAME_DB);

		lpGuildTable->pGuildOfflineHash->Delete(lpOfflineUser, lpOfflineUser->dwUserIndex, TRUE);		
		memset(lpOfflineUser, 0, sizeof(GUILD_OFFLINE_USER));

		GuildPoint(pUser->m_dwGuildId);
	}		
}

void GuildPoint(DWORD dwGuildId)
{
	char szQuery[0xff]={0,};

	LP_GUILD_LIST	lpGuildListTable	= g_pGuildWarHash->GetData(dwGuildId);	
	LP_GUILD_TABLE	lpGuildTable		= g_pGuildTableHash->GetData(dwGuildId);
		
	if(!lpGuildListTable)
		return;	
	if(!lpGuildTable)
		return;	

	for(int i = 0; i < MAX_GUILD_WAR; i++)
	{
		if(lpGuildListTable->sGuildNode[i].dwGuildId!=0)
		{
			LP_GUILD_LIST	lpGuildListTableDst	= g_pGuildWarHash->GetData(lpGuildListTable->sGuildNode[i].dwGuildId);
			LP_GUILD_TABLE	lpGuildTableDst		= g_pGuildTableHash->GetData(lpGuildListTable->sGuildNode[i].dwGuildId);

			if(!lpGuildListTableDst)
				continue;	
			if(!lpGuildTableDst)
				continue;
		
			for(int j = 0; j < MAX_GUILD_WAR; j++)
			{
				if(lpGuildListTableDst->sGuildNode[j].dwGuildId==lpGuildListTable->dwGuildId)
				{
					lpGuildListTable->sGuildNode[i].wPoint2		+= 3;
					lpGuildListTableDst->sGuildNode[j].wPoint1	+= 3;

					WSTC_GUILDWAR_POINT pPointPacket;
					pPointPacket.dwGuildId	= lpGuildListTable->sGuildNode[i].dwGuildId;
					pPointPacket.wPoint1	= lpGuildListTable->sGuildNode[i].wPoint1;
					pPointPacket.wPoint2	= lpGuildListTable->sGuildNode[i].wPoint2;
					pPointPacket.byType		= 7;												

					ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();

					while(pGuildUserPos)
					{
						CWorldUser* pGuildUser = pGuildUserPos->pData;

						if(pGuildUser)
							g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPointPacket, pPointPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

						pGuildUserPos = pGuildUserPos->pNext;
					}
					
					pPointPacket.dwGuildId	= lpGuildListTableDst->sGuildNode[j].dwGuildId;
					pPointPacket.wPoint1	= lpGuildListTableDst->sGuildNode[j].wPoint1;
					pPointPacket.wPoint2	= lpGuildListTableDst->sGuildNode[j].wPoint2;			
					pPointPacket.byType		= 6;

					pGuildUserPos = lpGuildTableDst->pGuildUserHash->GetHead();

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

					sprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTableDst->sGuild.dwGuildId);
					g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR, 1, FALSE, 0, NULL, (BYTE)GAME_DB);
				}
			}						
		}
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
	
	sprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTable->sGuild.dwGuildId);
	g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR, 1, FALSE, 0, NULL, (BYTE)GAME_DB);			
}

void CmdGuildInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{		
	char				szQuery[0xff]={0,};	
	CTWS_GUILD_INFO*	pPacket = (CTWS_GUILD_INFO*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_GUILD_INFO		pSendPacket;
	LP_GUILD_TABLE		lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);		
					
	if(pPacket->bRank<1 && pPacket->bRank>4)
		return;
	
	memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
	memset(pSendPacket.szNickName, 0, sizeof(pSendPacket.szNickName));
	pSendPacket.dwUserIndex	= pPacket->dwUserIndex;

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
			
			if(pGuildUser->m_dwUserIndex==pPacket->dwUserIndex)
			{
				LP_GROUPINFO_TABLE lpGroupInfoTable	= g_pGroupInfoTableHash->GetData(lpGuildTable->sGuild.wLevel);

				if(!lpGroupInfoTable)
					return;
					
				BOOL bChk = FALSE;

				if(pGuildUser->m_byRank!=pPacket->bRank)
				{
					if(lpGroupInfoTable->sGroupInfo->wMemeber[pPacket->bRank]>lpGuildTable->sGuild.byMember[pPacket->bRank-1])
						bChk = TRUE;
				}
				else
					bChk = TRUE;
					
				if(bChk)
				{
					lpGuildTable->sGuild.byMember[pPacket->bRank-1]			+= 1;
					lpGuildTable->sGuild.byMember[pGuildUser->m_byRank-1]	-= 1;

					wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pUser->m_dwGuildId);
					g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, pUser, (BYTE)GAME_DB);

					pGuildUser->m_byRank = pPacket->bRank;			
					memset(pGuildUser->m_szNickName, 0, sizeof(pGuildUser->m_szNickName));			
					__lstrcpyn(pGuildUser->m_szNickName, pPacket->szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);			
											
					__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pSendPacket.szNickName, pPacket->szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);
					pSendPacket.bRank		= pPacket->bRank;
					pSendPacket.bMsgType	= pPacket->bMsgType;

					ListNode<CWorldUser>* lpGuildUserSubNode = lpGuildTable->pGuildUserHash->GetHead();		
									
					while(lpGuildUserSubNode)
					{
						CWorldUser* pGuildUserSub = lpGuildUserSubNode->pData;
						
						if(!pGuildUserSub)
						{
							lpGuildUserSubNode = lpGuildUserSubNode->pNext;
							continue;
						}
						
						pSendPacket.bSerType = (BYTE)pGuildUserSub->m_dwCurServerPos;

						g_pNet->SendToUser(pGuildUserSub->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						WSTC_GUILD_COUNT pSendCountPacket;
						pSendCountPacket.byCount		= lpGuildTable->sGuild.byCount;
						pSendCountPacket.byMember[0]	= lpGuildTable->sGuild.byMember[0];
						pSendCountPacket.byMember[1]	= lpGuildTable->sGuild.byMember[1];
						pSendCountPacket.byMember[2]	= lpGuildTable->sGuild.byMember[2];
						pSendCountPacket.byMember[3]	= lpGuildTable->sGuild.byMember[3];
						g_pNet->SendToUser(pGuildUserSub->m_dwConnectionIndex, (char*)&pSendCountPacket, pSendCountPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						lpGuildUserSubNode = lpGuildUserSubNode->pNext;
					}
									
					// Guild 임시 작업 //
			
					wsprintf(szQuery, "Update CharacterInfo Set NickName = '%s', Rank=%u Where Char_Index = %u", 
								pPacket->szNickName, pPacket->bRank, pGuildUser->m_dwUserIndex);

					g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, pGuildUser, (BYTE)GAME_DB);			

					WSTDS_GUILDINFO_MSG pSendServerPacket;
					memset(pSendServerPacket.szNickName, 0, sizeof(pSendServerPacket.szNickName));
					pSendServerPacket.byRank		= pGuildUser->m_byRank;
					pSendServerPacket.dwGuildId		= pGuildUser->m_dwGuildId;
					pSendServerPacket.dwUserIndex	= pGuildUser->m_dwUserIndex;
					__lstrcpyn(pSendServerPacket.szNickName, pGuildUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);

					DUNGEON_DATA_EX* pDungeon;
					pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pGuildUser->m_dwDungeonID);
					
					if(pDungeon)
						g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pSendServerPacket, pSendServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);					
				}
				else
				{
					WSTC_RANK_FAIL pRankSendPacket;
					pRankSendPacket.byType	= 1;
					pRankSendPacket.byRank	= pPacket->bRank;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pRankSendPacket, pRankSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				return;
			}

			lpGuildUserNode = lpGuildUserNode->pNext;
		}
	}

	ListNode<GUILD_OFFLINE_USER>* lpGuildOfflineUserNode = lpGuildTable->pGuildOfflineHash->GetHead();		
							
	while(lpGuildOfflineUserNode)
	{
		LP_GUILD_OFFLINE_USER pGuildOfflineUser = lpGuildOfflineUserNode->pData;
		
		if(pGuildOfflineUser)
		{
			if(pGuildOfflineUser->dwUserIndex==pPacket->dwUserIndex)
			{
				LP_GROUPINFO_TABLE lpGroupInfoTable	= g_pGroupInfoTableHash->GetData(lpGuildTable->sGuild.wLevel);

				if(!lpGroupInfoTable)
					return;
							
				BOOL bChk = FALSE;

				if(pGuildOfflineUser->byRank!=pPacket->bRank)
				{
					if(lpGroupInfoTable->sGroupInfo->wMemeber[pPacket->bRank]>lpGuildTable->sGuild.byMember[pPacket->bRank-1])
						bChk = TRUE;
				}
				else
					bChk = TRUE;
				
				if(bChk)
				{
					lpGuildTable->sGuild.byMember[pPacket->bRank-1]				+= 1;
					lpGuildTable->sGuild.byMember[pGuildOfflineUser->byRank-1]	-= 1; 								

					wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pUser->m_dwGuildId);
					g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, pUser, (BYTE)GAME_DB);

					// 캐릭터가 Off Line일 경우 //
					wsprintf(szQuery, "Update CharacterInfo Set NickName = '%s', Rank=%u Where Char_Index = %u", 
						pPacket->szNickName, pPacket->bRank, pPacket->dwUserIndex);
					g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, pUser, (BYTE)GAME_DB);	

					// 패킷 날리기 //
					__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_LENGTH);
					__lstrcpyn(pSendPacket.szNickName, pPacket->szNickName, MAX_CHARACTER_NAME_LENGTH);
					pSendPacket.bRank		= pPacket->bRank;
					pSendPacket.bMsgType	= pPacket->bMsgType;			
					pSendPacket.bSerType	= (BYTE)pUser->m_dwCurServerPos;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

					ListNode<CWorldUser>* lpGuildUserSubNode = lpGuildTable->pGuildUserHash->GetHead();		
									
					while(lpGuildUserSubNode)
					{
						CWorldUser* pGuildUserSub = lpGuildUserSubNode->pData;
						
						if(!pGuildUserSub)
						{
							lpGuildUserSubNode = lpGuildUserSubNode->pNext;
							continue;
						}
						
						WSTC_GUILD_COUNT pSendCountPacket;
						pSendCountPacket.byCount		= lpGuildTable->sGuild.byCount;
						pSendCountPacket.byMember[0]	= lpGuildTable->sGuild.byMember[0];
						pSendCountPacket.byMember[1]	= lpGuildTable->sGuild.byMember[1];
						pSendCountPacket.byMember[2]	= lpGuildTable->sGuild.byMember[2];
						pSendCountPacket.byMember[3]	= lpGuildTable->sGuild.byMember[3];
						g_pNet->SendToUser(pGuildUserSub->m_dwConnectionIndex, (char*)&pSendCountPacket, pSendCountPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						lpGuildUserSubNode = lpGuildUserSubNode->pNext;
					}
				}
				else
				{
					WSTC_RANK_FAIL pRankSendPacket;
					pRankSendPacket.byType	= 1;
					pRankSendPacket.byRank	= pPacket->bRank;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pRankSendPacket, pRankSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				return;
			}			
		}
		lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
	}					
}

void CmdGuildRank(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	WSTC_GUILD_RANK		pGuildRank;
	CTWS_GUILD_RANK*	pPacket = (CTWS_GUILD_RANK*)pMsg;
	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	LP_GUILD_TABLE		lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);		

	if(pUser->m_dwGuildId==0)
	{
		// 등급 조정 메세지 //
		pGuildRank.bResult = FALSE;
		pGuildRank.bSerType	= (BYTE)pUser->m_dwCurServerPos;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pGuildRank, pGuildRank.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
	
	for( int i = 0 ; i < GUILD::GUILD_RANK_COUNT_MAX ; ++i )
	{
		::ZeroMemory(
			lpGuildTable->sGuild.szRankName[i], 
			MAX_CHARACTER_NAME_LENGTH
			);

		__lstrcpyn(
			lpGuildTable->sGuild.szRankName[i], 
			pPacket->szRankName[i], 
			MAX_RANK_REAL_NICKNAME_LENGTH
			);
	}

	// DB 쿼리 //			
	char szQuery[0xff]={0,};
	
	wsprintf(szQuery, "Update GuildInfo Set Rank1 = '%s', Rank2 = '%s', Rank3 = '%s', Rank4 = '%s' Where [ID] = %u", 
		pPacket->szRankName[GUILD::GUILD_RANK_MASTER], 
		pPacket->szRankName[GUILD::GUILD_RANK_SUB_MASTER], 
		pPacket->szRankName[GUILD::GUILD_RANK_HIGH], 
		pPacket->szRankName[GUILD::GUILD_RANK_NORMAL],  
		pUser->m_dwGuildId
		);
	g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_RANK, pUser, (BYTE)GAME_DB);
	
	// 등급 조정 메세지 //
	pGuildRank.bResult	= TRUE;
	pGuildRank.bSerType	= (BYTE)pUser->m_dwCurServerPos;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pGuildRank, pGuildRank.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
}

void CmdGuildJoin(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	dwLength;

	CTWS_GUILD_JOIN*		pPacket = (CTWS_GUILD_JOIN*)pMsg;
	WSTC_GUILD_JOINRESULT	pJoinPacket;
	CWorldUser*				pUserRev = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);
	
	memset(pJoinPacket.szCharacterName, 0, sizeof(pJoinPacket.szCharacterName));

	if(!pUserRev)
		return;

	if(pPacket->bResult==TRUE)
	{
		LP_GUILD_TABLE	lpGuildTable = g_pGuildTableHash->GetData(pUserRev->m_dwGuildId);

		if(!lpGuildTable)
			return;	
		
		LP_GROUPINFO_TABLE lpGroupInfoTable = (LP_GROUPINFO_TABLE)g_pGroupInfoTableHash->GetData(lpGuildTable->sGuild.wLevel);

		if(!lpGroupInfoTable)
			return;
		
		if(pUser->m_dwPrevGuildId==lpGuildTable->sGuild.dwGuildId)
		{
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);

			COleDateTime PrevTime(pUser->m_sGuildJoinTime.year, pUser->m_sGuildJoinTime.month, pUser->m_sGuildJoinTime.day, pUser->m_sGuildJoinTime.hour, pUser->m_sGuildJoinTime.minute, pUser->m_sGuildJoinTime.second);
				
			if(pUser->m_sGuildJoinTime.year==0)	
				return;
			if(pUser->m_sGuildJoinTime.day==0)
				return;
			if(pUser->m_sGuildJoinTime.month==0)
				return;

			COleDateTime CurTime(SystemTime);
				
			COleDateTimeSpan ts = CurTime - PrevTime;

			if( ts.GetStatus() == COleDateTimeSpan::valid )
			{
				DWORD dwDay = ts.GetDays();			
				if(dwDay<1)
				{
					// 에러 패킷 //
					WSTC_GUILD_ERROR pErrorPacket;
					pErrorPacket.byError	= 1;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
			}
			else
			{// 날짜가 틀렸기 때문에
				WSTC_GUILD_ERROR pErrorPacket;
				pErrorPacket.byError	= 1;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
		}
				
		DUNGEON_DATA_EX* pDungeon;
		pDungeon = g_pDungeonTable->GetDungeonInfo(pUser->m_dwDungeonID);	

		if(!pDungeon)
		{
			Log(LOG_NORMAL, "Dungeon = NULL(CmdGuildJoin)");
			return;
		}

		if(lpGuildTable->sGuild.byCount<lpGroupInfoTable->sGroupInfo->wMemeber[0])
		{						
			lpGuildTable->sGuild.byCount+=1;
			lpGuildTable->sGuild.byMember[3]+=1;
		
			// 길드 정보 보내기 //
			WSTC_GUILD_USER pGuildUserPacket;

			memset(pGuildUserPacket.szGuildName, 0, sizeof(pGuildUserPacket.szGuildName));

			for( int i = 0 ; i < GUILD::GUILD_RANK_COUNT_MAX ; ++i )
			{
				::ZeroMemory(
					pGuildUserPacket.szRankName[i], 
					MAX_GUILD_NAME_LENGTH
					);
			}

			pGuildUserPacket.byType			= lpGuildTable->sGuild.byType;
			pGuildUserPacket.byCount		= lpGuildTable->sGuild.byCount;				
			pGuildUserPacket.dwGuildId		= lpGuildTable->sGuild.dwGuildId;
			pGuildUserPacket.dwPoint		= lpGuildTable->sGuild.dwPoint;
			pGuildUserPacket.dwExp			= lpGuildTable->sGuild.dwExp;
			pGuildUserPacket.wLevel			= lpGuildTable->sGuild.wLevel;			
			pGuildUserPacket.byMember[0]	= lpGuildTable->sGuild.byMember[0];
			pGuildUserPacket.byMember[1]	= lpGuildTable->sGuild.byMember[1];
			pGuildUserPacket.byMember[2]	= lpGuildTable->sGuild.byMember[2];
			pGuildUserPacket.byMember[3]	= lpGuildTable->sGuild.byMember[3];
			pGuildUserPacket.byLevelIndex	= lpGuildTable->bLevelIndex;
			
			__lstrcpyn(pGuildUserPacket.szGuildName, lpGuildTable->sGuild.szGuildName, MAX_RANK_REAL_NICKNAME_LENGTH);

			for( int i = 0 ; i < GUILD::GUILD_RANK_COUNT_MAX ; ++i )
			{
				__lstrcpyn(
					pGuildUserPacket.szRankName[i],
					lpGuildTable->sGuild.szRankName[i],
					MAX_RANK_REAL_NICKNAME_LENGTH
					);
			}
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pGuildUserPacket, pGuildUserPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			// 정보 설정 //
			memset(pUser->m_szGuildName, 0, sizeof(pUser->m_szGuildName));
			strncpy(pUser->m_szGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);				
			memset(pUser->m_szNickName, 0, MAX_CHARACTER_REAL_NICKNAME_LENGTH);
			pUser->m_byType		= lpGuildTable->sGuild.byType;
			pUser->m_byRank		= 4;
			pUser->m_dwGuildId	= lpGuildTable->sGuild.dwGuildId;

			//pUser->GuildIdPoint();
			pUser->GuildIdChange();
				
			// 길드원 정보 보내기 //
			lpGuildTable->pGuildUserHash->Insert(pUser, pUser->m_dwUserIndex);
			
			if(pUser)
			{
				POSITION_ pos = g_pRankList->GetHeadPosition();

				if(pos)
				{
					LP_GUILDWAR_RANK lpGuildRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(pos);

					if(lpGuildRank)
					{
						if(lpGuildRank->dwGuildId==pUser->m_dwGuildId)
						{
							WSTDS_GUILDUSER_RANK pGuildUserRankPacket;
							pGuildUserRankPacket.dwUserIndex	= pUser->m_dwUserIndex;
							pGuildUserRankPacket.dwGuildId		= pUser->m_dwGuildId;
							pGuildUserRankPacket.byGuildFlag	= 1;
							pGuildUserRankPacket.byChk			= 1;

							DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);

							if(pDungeon)
								g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pGuildUserRankPacket, pGuildUserRankPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}
					}

				}														
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
						
				// 메세지 보내기 //
				if(pGuildUser->m_dwUserIndex==pUser->m_dwUserIndex)
				{
					// 길드 전체에게 보내기(자기 자신은 빼고) //
					ListNode<CWorldUser>* lpGuildUserNodeSub = lpGuildTable->pGuildUserHash->GetHead();														
				
					while(lpGuildUserNodeSub)
					{
						CWorldUser* pGuildUserSub = lpGuildUserNodeSub->pData;
						
						if(!pGuildUser)
						{
							lpGuildUserNodeSub = lpGuildUserNodeSub->pNext;
							{
								lpGuildUserNodeSub = lpGuildUserNodeSub->pNext;
								continue;
							}
						}

						if(pGuildUserSub->m_dwUserIndex!=pUser->m_dwUserIndex)
						{
							WSTC_GUILD_USERINFO pPacketSend;					
							
							memset(pPacketSend.szNickName, 0, sizeof(pPacketSend.szNickName));
							memset(pPacketSend.szCharacterName,0, sizeof(pPacketSend.szCharacterName));

							pPacketSend.bGuildCount	= 1;
							pPacketSend.dwLev		= pGuildUserSub->m_dwLevel;
							pPacketSend.wClass		= pGuildUserSub->m_wClass;
							pPacketSend.byRank		= pGuildUserSub->m_byRank;
							pPacketSend.dwGuildId	= pGuildUserSub->m_dwGuildId;
							pPacketSend.dwUserIndex	= pGuildUserSub->m_dwUserIndex;
							__lstrcpyn(pPacketSend.szNickName, pGuildUserSub->m_szNickName, MAX_CHARACTER_NAME_REAL_LENGTH);
							__lstrcpyn(pPacketSend.szCharacterName, pGuildUserSub->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);					
							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacketSend, pPacketSend.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);								
						}
						lpGuildUserNodeSub = lpGuildUserNodeSub->pNext;
					}						
				}
				else
				{					
					// 나중에 들어온 한명만 //
					WSTC_GUILD_USERINFO pPacketSend;
					
					memset(pPacketSend.szNickName, 0, sizeof(pPacketSend.szNickName));
					memset(pPacketSend.szCharacterName, 0, sizeof(pPacketSend.szCharacterName));
																									
					// 월드에 있을 때 //
					pPacketSend.bGuildCount	= 2;
					pPacketSend.dwLev		= pPacket->dwLev;
					pPacketSend.wClass		= pPacket->wClass;
					pPacketSend.byRank		= pUser->m_byRank;
					pPacketSend.dwGuildId	= pUser->m_dwGuildId;
					pPacketSend.dwUserIndex	= pUser->m_dwUserIndex;
					__lstrcpyn(pPacketSend.szNickName, pUser->m_szNickName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pPacketSend.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pPacketSend, pPacketSend.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

					WSTC_GUILD_COUNT pSendPacket;
					pSendPacket.byCount		= lpGuildTable->sGuild.byCount;
					pSendPacket.byMember[0]	= lpGuildTable->sGuild.byMember[0];
					pSendPacket.byMember[1]	= lpGuildTable->sGuild.byMember[1];
					pSendPacket.byMember[2]	= lpGuildTable->sGuild.byMember[2];
					pSendPacket.byMember[3]	= lpGuildTable->sGuild.byMember[3];
					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				lpGuildUserNode = lpGuildUserNode->pNext;
			}
							
			// DB 쿼리하기 //				
			char	szQuery[0xff]={0,};		

			wsprintf(
				szQuery, 
				"CZP_GUILD_CHANGE 0, %u, %u, '%s', '%s', %u, %u",
				pUser->m_dwGuildId, 
				pUser->m_dwUserIndex, 
				pUser->m_szGuildName, 
				pUser->m_szNickName, 
				pUser->m_byType, 
				pUser->m_byRank
				);			

			g_pDb->THExecuteSQL(
				szQuery, 
				TRUE, 
				QUERY_TYPE_GUILD_USER, 
				pUser, 
				GAME_DB
				);

			wsprintf(
				szQuery, 
				"INSERT INTO GuildUserInfo (Char_Index, GuildId) VALUES(%u, %u)", 
				pUser->m_dwUserIndex, 
				pUser->m_dwGuildId
				);

			g_pDb->THExecuteSQL(
				szQuery, 
				TRUE, 
				QUERY_TYPE_GUILDUSER_DELETE, 
				NULL, 
				GAME_DB
				);
			
			wsprintf(
				szQuery, 
				"UPDATE MAPINFO SET Manager='%s', OwnerGuildNum= %d where OwnerIndex = %u", 
				(strlen(pUser->m_szGuildName) ? pUser->m_szGuildName : "---"), 
				pUser->m_dwGuildId, pUser->m_dwUserIndex
				);	

			g_pDb->THExecuteSQL(
				szQuery, 
				TRUE, 
				QUERY_TYPE_OWNER, 
				pUser, 
				GAME_DB
				);
			
			wsprintf(
				szQuery, 
				"UPDATE GuildInfo SET [Count]=%u, [Member1]=%u, [Member2]=%u, [Member3]=%u, [Member4]=%u Where [Id] = %u",
				lpGuildTable->sGuild.byCount, 
				lpGuildTable->sGuild.byMember[0], 
				lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pUserRev->m_dwGuildId);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, pUserRev, GAME_DB);

			wsprintf(szQuery, "CZP_GUILDJOIN_TIME %u", pUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pUser, GAME_DB);


			// 가입 성공 메세지 //
			pJoinPacket.bResult		= TRUE;			
			pJoinPacket.bErrorCode	= 0;
			pJoinPacket.dwUserIndex	= pUser->m_dwUserIndex;
			__lstrcpyn(pJoinPacket.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);	
			g_pNet->SendToUser(pUserRev->m_dwConnectionIndex, (char*)&pJoinPacket, pJoinPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			WSTC_GUILD_JOININFO pJoinInfoPacket;
			memset(pJoinInfoPacket.szGuildName, 0, sizeof(pJoinInfoPacket.szGuildName));				
			__lstrcpyn(pJoinInfoPacket.szGuildName, pUser->m_szGuildName, MAX_GUILD_REAL_NAME_LENGTH);						
			pJoinInfoPacket.byType		= pUser->m_byType;
			pJoinInfoPacket.byRank		= pUser->m_byRank;
			pJoinInfoPacket.dwGuildId	= pUser->m_dwGuildId;				
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pJoinInfoPacket, pJoinInfoPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			// 던전 서버에 보내기 //				
			WSTDS_GUILD_INFO pServerPacket;
			memset(pServerPacket.szGuildName, 0, sizeof(pServerPacket.szGuildName));
			memset(pServerPacket.szNickName, 0, sizeof(pServerPacket.szNickName));
			pServerPacket.dwUserIndex	= pUser->m_dwUserIndex;
			pServerPacket.dwGuildId		= pUser->m_dwGuildId;
			pServerPacket.byRank		= pUser->m_byRank;
			pServerPacket.byType		= pUser->m_byType;
			__lstrcpyn(pServerPacket.szGuildName, pUser->m_szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
			__lstrcpyn(pServerPacket.szNickName, pUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);		
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pServerPacket, pServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

			// 길드전 정보 //
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

			return;
		}			
		// 인원수가 넘었을 경우 //			
	}	
	else
	{
		// 가입을 거절했을 경우 //		
		pJoinPacket.bResult		= FALSE;		
		pJoinPacket.dwUserIndex	= pUser->m_dwUserIndex;
		__lstrcpyn(pJoinPacket.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_LENGTH);

		if(pPacket->bGuildType==3 || pPacket->bGuildType==4)
			pJoinPacket.bErrorCode	= pPacket->bGuildType;									
		else
			pJoinPacket.bErrorCode	= 0;

		g_pNet->SendToUser(pUserRev->m_dwConnectionIndex, (char*)&pJoinPacket, pJoinPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}

//nvarchar 인가 그걸 쓰면 white 스트링이 없다.
void ClearWhiteString(IN string &strOrg)//공백문자를 처음부터 쓰게 되면 문제가 있다.
{// nvarchar 인가..그거 쓰면 끝에 공백 문자 없다.
	const char szSpace = ' ';
	
	size_t idx = strOrg.find(szSpace);//공백문자를 찾고
	if(string::npos != idx)//못찾은게 아니라면
	{//나머지 부분을 삭제.
		strOrg.at(idx) = '\0';
		string strTemp(strOrg.c_str());
		strOrg.swap(strTemp);// 
	}
//	char *pDest = :strchr(szStr, szSpace);
//	
}


void TrimLeft(
	const char*  pszParam,
	char*        pszResult
	)
{
	char szBuffer[MAX_PATH];
	strcpy(szBuffer, "\0");
	if ( (TRUE != ::IsBadStringPtr(pszParam, MAX_PATH)) &&
		 (strlen(pszParam) > 0) )
	{
		DWORD dwIndex = 0;
		while ( (dwIndex < strlen(pszParam)) && (pszParam[dwIndex] == ' ') )
			dwIndex++;
		if (dwIndex < strlen(pszParam))
			strcpy(szBuffer, &pszParam[dwIndex]);
	} // if
	strcpy(pszResult, szBuffer);
}
	
//
// Trims trailing spaces and control characters from a string
//
void TrimRight(
	const char*  pszParam,
	char*        pszResult
	)
{
	char szBuffer[MAX_PATH];
	strcpy(szBuffer, "\0");
	if ( (TRUE != ::IsBadStringPtr(pszParam, MAX_PATH)) &&
		 (strlen(pszParam) > 0) )
	{
		int nIndex = strlen(pszParam) - 1;
		while ( (nIndex >= 0) && (pszParam[nIndex] == ' ' || pszParam[nIndex] == -95) )
			nIndex--;
		if (nIndex >= 0)
		{
			memcpy(
				(PBYTE)szBuffer, 
				(PBYTE)pszParam, 
				(nIndex + 1)
				); 
			szBuffer[nIndex+1] = 0;
		} // if
	} // if
	strcpy(pszResult, szBuffer);
}

//

void CmdGuildCreate(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTWS_GUILD_CREATE*	pPacket = (CTWS_GUILD_CREATE*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
		
	// 길드를 검색해서 결과를 보내고 성공하면 쿼리한다 //
	ListNode<GUILD_TABLE>*	lpGuildTableNode = g_pGuildTableHash->GetHead();		
	LP_GUILD_TABLE			lpGuildTable;	
		
	// 던전 서버에 보내기 //
	DUNGEON_DATA_EX* pDungeon;
	pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);	

	if(!pDungeon)
	{
		Log(LOG_NORMAL, "Dungeon = NULL(CmdGuildCreate)");
		return;
	}
	

	if(pUser->m_dwGuildId!=0)
	{
		WSTC_GUILD_CREATE pResult;
		pResult.bCreate = FALSE;
		pResult.bType	= 3;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pResult, pResult.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);						
	}
	

	int nLen = strlen(pPacket->szGuildName);

	char szTmpName[MAX_GUILD_NAME_LENGTH];
	memset(szTmpName,0x00,MAX_GUILD_NAME_LENGTH);


	TrimRight(pPacket->szGuildName,szTmpName);

	
	for(int i = 0; i < nLen; i++)
	{
		if(pPacket->szGuildName[i]==32)
			wsprintf(&pPacket->szGuildName[i], "");
	}
		
	while(lpGuildTableNode)
	{
		lpGuildTable = lpGuildTableNode->pData;

		// 길드 생성 오류
		// 임상우 2005.01.29
		if(	0 == ::_strnicmp( szTmpName , lpGuildTable->sGuild.szGuildName,strlen(szTmpName) ) )
		{	// 실패 //			
			WSTC_GUILD_CREATE pResult;
			pResult.bCreate = FALSE;
			pResult.bType	= lpGuildTable->sGuild.byType;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pResult, pResult.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);						

			WSTDS_GUILD_FAIL pServerPacket;			
			pServerPacket.dwUserIndex	= pUser->m_dwUserIndex;		
			pServerPacket.byType		= pPacket->bType;
			g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pServerPacket, pServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);			
			return;
		}
		lpGuildTableNode = lpGuildTableNode->pNext;
	}
	
	char szQuery[0xff]={0,};
	// 쿼리하기 //
	wsprintf(szQuery, "Insert into GuildInfo (Name, Rank1, Rank2, Rank3, Rank4, Type, [Count], Member1, Member2, Member3, Member4) VALUES('%s', '%s', '%s', '%s', '%s', %u, %u, %u, %u, %u, %u)", 
		szTmpName, g_szDefaultGuildRank[0], g_szDefaultGuildRank[1], g_szDefaultGuildRank[2], g_szDefaultGuildRank[3], pPacket->bType, 1, 1, 0, 0, 0);
	g_pDb->THExecuteSQL(szQuery, (BYTE)GAME_DB);

	// 쿼리하기 //
	wsprintf(szQuery, "Select ID, Name, Type From GuildInfo where Name = '%s'", szTmpName);
	g_pDb->THOpenRecord(szQuery, QUERY_GUILD_ID, (void*)pUser->m_dwConnectionIndex, 1, (BYTE)GAME_DB);	

	pUser->m_dwLevel	= pPacket->dwLev;
	pUser->m_wClass		= pPacket->wClass;
}

void CmdUserGuildMaster(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_USER_GUILDMASTER*	pPacket = (CTWS_USER_GUILDMASTER*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	// 길드 이양 //
	LP_GUILD_TABLE		lpGuildTable=0;
	char				szQuery[0xff]={0,};	

	lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);	

	if(!lpGuildTable)
		return;
	
	CWorldUser* pMasterUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(!pMasterUser)
	{
		WSTC_GUILD_ERROR pErrorPacket;
		pErrorPacket.byError = 3;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		return;
	}

	if(pUser->m_byRank!=1)
		return;

	DUNGEON_DATA_EX* pDungeon = NULL;

	if(pUser)
	{
		pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);	

		if(!pDungeon)
			return;
	}

	DUNGEON_DATA_EX* pMasterDungeon = NULL;

	if(pMasterUser)
	{
		pMasterDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pMasterUser->m_dwDungeonID);	

		if(!pMasterDungeon)
			return;
	}
			
	pUser->m_byRank			= pMasterUser->m_byRank;
	pMasterUser->m_byRank	= 1;


	wsprintf(szQuery, "CZP_GUILD_CHANGE 1, 0, %u, '', '', 0, %u", pUser->m_dwUserIndex, pUser->m_byRank);	
	g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);
	
	wsprintf(szQuery, "CZP_GUILD_CHANGE 1, 0, %u, '', '', 0, %u", pMasterUser->m_dwUserIndex, pMasterUser->m_byRank);	
	g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);


	WSTDS_GUILD_INFO pDsServerPacket;
	memset(pDsServerPacket.szGuildName, 0, sizeof(pDsServerPacket.szGuildName));
	memset(pDsServerPacket.szNickName, 0, sizeof(pDsServerPacket.szNickName));
	pDsServerPacket.dwUserIndex		= pUser->m_dwUserIndex;
	pDsServerPacket.dwGuildId		= pUser->m_dwGuildId;
	pDsServerPacket.byRank			= pUser->m_byRank;
	pDsServerPacket.byType			= pUser->m_byType;
	__lstrcpyn(pDsServerPacket.szGuildName, pUser->m_szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
	__lstrcpyn(pDsServerPacket.szNickName, pUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);		
	g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pDsServerPacket, pDsServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	
	memset(pDsServerPacket.szGuildName, 0, sizeof(pDsServerPacket.szGuildName));
	memset(pDsServerPacket.szNickName, 0, sizeof(pDsServerPacket.szNickName));
	pDsServerPacket.dwUserIndex		= pMasterUser->m_dwUserIndex;
	pDsServerPacket.dwGuildId		= pMasterUser->m_dwGuildId;
	pDsServerPacket.byRank			= pMasterUser->m_byRank;
	pDsServerPacket.byType			= pMasterUser->m_byType;
	__lstrcpyn(pDsServerPacket.szGuildName, pMasterUser->m_szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
	__lstrcpyn(pDsServerPacket.szNickName, pMasterUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);		
	g_pNet->SendToServer(pMasterDungeon->m_pServer->dwConnectionIndex, (char*)&pDsServerPacket, pDsServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);

	WSTC_USER_GUILDMASTER pSendPacket; 
	memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
	memset(pSendPacket.szMasterName, 0, sizeof(pSendPacket.szMasterName));
	__lstrcpyn(pSendPacket.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
	__lstrcpyn(pSendPacket.szMasterName, pMasterUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);		
	pSendPacket.dwUserIndex			= pUser->m_dwUserIndex;
	pSendPacket.dwMasterUserIndex	= pMasterUser->m_dwUserIndex;
	pSendPacket.byRank				= pUser->m_byRank;
	pSendPacket.byMasterRank		= pMasterUser->m_byRank;

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
	
	// Minbobo Fix //
	ListNode<GUILD_OFFLINE_USER>*	lpGuildOfflineUserNode = lpGuildTable->pGuildOfflineHash->GetHead();		
							
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
			g_pNet->SendToUser(pMasterUser->m_dwConnectionIndex, (char*)&pOffLinePacket, pOffLinePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
		lpGuildOfflineUserNode = lpGuildOfflineUserNode->pNext;
	}
}

void CmdGuildDelete(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_GUILD_DELETE*	pPacket = (CTWS_GUILD_DELETE*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_GUILD_DELETE	pSendPacket;
	LP_GUILD_TABLE		lpGuildTable=0;
	char				szQuery[0xff]={0,};	

	lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);	

	if(lpGuildTable)
	{
		if(lpGuildTable->sGuild.byCount==1)
		{			
			LP_GUILD_LIST lpGuildListTable	= g_pGuildWarHash->GetData(pUser->m_dwGuildId);		

			if(lpGuildListTable)
			{
				BOOL bChk = FALSE;

				for(int i = 0; i < MAX_GUILD_WAR; i++)
				{
					if(lpGuildListTable->sGuildNode[i].dwGuildId!=0)
					{
						bChk = TRUE;
						break;
					}
				}

				if(bChk)
				{
					WSTC_GUILDWAR_ERRROR pErrorPacket;
					pErrorPacket.byError = 11;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
					return;
				}
			}		

			pUser->m_dwPrevGuildId	= 0;
			wsprintf(szQuery, "CZP_GUILDOUT_PREVALL %u", 0);
			g_pDb->THExecuteSQL(szQuery, FALSE, NULL, NULL, (BYTE)GAME_DB);

			wsprintf(szQuery, "CZP_GUILD_CHANGE 2, %u, 0, '', '', 0, 0", pUser->m_dwGuildId);			
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, pUser, (BYTE)GAME_DB);

			wsprintf(szQuery, "Delete From GuildUserInfo Where GuildId = %u ", pUser->m_dwGuildId);					
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILDUSER_DELETE, pUser, (BYTE)GAME_DB);

			wsprintf(szQuery, "Update mapinfo set Manager = '---', OwnerGuildNum=0 where OwnerIndex = %u", pUser->m_dwGuildId);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_OWNER, (void*)pUser->m_dwConnectionIndex, (BYTE)GAME_DB);
				
			wsprintf(szQuery, "Delete From GuildInfo Where [ID] = %u", pUser->m_dwGuildId);
			g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);

#ifdef GUILD_WAR
			wsprintf(szQuery, "CZP_GUILDLIST_DELETE %u", pUser->m_dwGuildId);
			g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);
#endif

			POSITION_ pos = g_pRankList->GetHeadPosition();

			while(pos)
			{				
				POSITION_ posTemp = pos;

				LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(pos);

				if(lpGuildWarRank->dwGuildId==pUser->m_dwGuildId)
				{
					memset(lpGuildWarRank, 0, sizeof(lpGuildWarRank));
					LALFree(g_pRankListPool, lpGuildWarRank);
					lpGuildWarRank = NULL;
					g_pRankList->RemoveAt(posTemp);
					break;
				}
			}
		
			pSendPacket.dwGuildId	= pUser->m_dwGuildId;

			
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
				pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pGuildUser->m_dwDungeonID);	

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

			// Guild List 삭제 //
			g_pGuildTableHash->Delete(lpGuildTable, pPacket->dwGuildId, TRUE);	
			lpGuildTable = NULL;
		}
		else
		{
			// 길드 이양.
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

				if(pGuildUser->m_byRank==1)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}

				if(pMasterUser==NULL)
					pMasterUser = pGuildUser;
				else
				{
					if(pMasterUser->m_byRank>pGuildUser->m_byRank)
						pMasterUser = pGuildUser;
					else if(pMasterUser->m_byRank==pGuildUser->m_byRank)
					{
						if(pMasterUser->m_dwLevel<pGuildUser->m_dwLevel)
							pMasterUser = pGuildUser;
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

			if(!pMasterUser && !lpMasterOfflineUser)
				return;

			DUNGEON_DATA_EX* pDungeon = NULL;

			if(pMasterUser)
				pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pMasterUser->m_dwDungeonID);	

			DUNGEON_DATA_EX* pDeleteDungeon = NULL;

			if(pUser)
				pDeleteDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);	
			
			lpGuildTable->sGuild.byCount -= 1;

			pUser->m_dwPrevGuildId	= pUser->m_dwGuildId;


			wsprintf(szQuery, "CZP_GUILDOUT_PREV %u, %u", pPacket->dwGuildId, pUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, FALSE, NULL, NULL, (BYTE)GAME_DB);

			wsprintf(szQuery, "CZP_GUILD_CHANGE 0, 0, %u, '', '', 0, 0", pUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);
			
			wsprintf(szQuery, "Delete From GuildUserInfo Where Char_Index = %u", pUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILDUSER_DELETE, NULL, (BYTE)GAME_DB);
			
			if(lpMasterOfflineUser)
			{
				wsprintf(szQuery, "CZP_GUILD_CHANGE 1, %u, %u, '%s', '%s', %u, %u", 
							lpGuildTable->sGuild.dwGuildId, lpMasterOfflineUser->dwUserIndex, lpGuildTable->sGuild.szGuildName, lpMasterOfflineUser->szNickName, 1, 1);	

				lpMasterOfflineUser->byRank	= 1;
			}
			else
			{
				wsprintf(szQuery, "CZP_GUILD_CHANGE 1, %u, %u, '%s', '%s', %u, %u", 
							lpGuildTable->sGuild.dwGuildId, pMasterUser->m_dwUserIndex, lpGuildTable->sGuild.szGuildName, pMasterUser->m_szNickName, 1, 1);	

				pMasterUser->m_byRank	= 1;				
			}						
			g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_USER, NULL, (BYTE)GAME_DB);

			CWorldUser* pDeleteUser = lpGuildTable->pGuildUserHash->GetData(pUser->m_dwUserIndex);
			
			if(pDeleteUser)
			{
				if(pDeleteDungeon)
				{
					WSTDS_GUILD_MSG pDeleteServerPacket;								
					pDeleteServerPacket.dwUserIndex	= pDeleteUser->m_dwUserIndex;
					g_pNet->SendToServer(pDeleteDungeon->m_pServer->dwConnectionIndex, (char*)&pDeleteServerPacket, pDeleteServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				}
				
				pDeleteUser->GuildIdPoint();

				pDeleteUser->m_byRank		= 0;
				pDeleteUser->m_byType		= 0;
				pDeleteUser->m_dwGuildId	= 0;
				memset(pDeleteUser->m_szNickName, 0, sizeof(pDeleteUser->m_szNickName));
				memset(pDeleteUser->m_szGuildName, 0, sizeof(pDeleteUser->m_szGuildName));

				pDeleteUser->GuildIdChange();

				if(pDeleteDungeon)
				{
					WSTDS_GUILD_INFO pDsServerPacket;
					memset(pDsServerPacket.szGuildName, 0, sizeof(pDsServerPacket.szGuildName));
					memset(pDsServerPacket.szNickName, 0, sizeof(pDsServerPacket.szNickName));
					pDsServerPacket.dwUserIndex		= pDeleteUser->m_dwUserIndex;
					pDsServerPacket.dwGuildId		= pDeleteUser->m_dwGuildId;
					pDsServerPacket.byRank			= pDeleteUser->m_byRank;
					pDsServerPacket.byType			= pDeleteUser->m_byType;
					__lstrcpyn(pDsServerPacket.szGuildName, pDeleteUser->m_szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
					__lstrcpyn(pDsServerPacket.szNickName, pDeleteUser->m_szNickName, MAX_CHARACTER_REAL_NICKNAME_LENGTH);		
					g_pNet->SendToServer(pDeleteDungeon->m_pServer->dwConnectionIndex, (char*)&pDsServerPacket, pDsServerPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
				}				

				lpGuildTable->pGuildUserHash->Delete(pDeleteUser, pDeleteUser->m_dwUserIndex, TRUE);				
			}
			
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

			if(!pMasterUser)
			{
				lpGuildTable->sGuild.byMember[lpMasterOfflineUser->byRank-1] -=  1;
				lpMasterOfflineUser->byRank = 1;


				wsprintf(szQuery, "Update mapinfo set Manager='---', OwnerGuildNum=0 where OwnerIndex = %u", lpMasterOfflineUser->dwUserIndex);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_OWNER, NULL, (BYTE)GAME_DB);							

				wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pPacket->dwGuildId);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, NULL, (BYTE)GAME_DB);

				dwUserIndex	= lpMasterOfflineUser->dwUserIndex;
				__lstrcpyn(szName, lpMasterOfflineUser->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			}
			else if(!lpMasterOfflineUser)
			{
				lpGuildTable->sGuild.byMember[pMasterUser->m_byRank-1] -=  1;
				pMasterUser->m_byRank = 1;
		

				wsprintf(szQuery, "Update mapinfo set Manager='---', OwnerGuildNum=0 where OwnerIndex = %u", pMasterUser->m_dwUserIndex);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_OWNER, NULL, (BYTE)GAME_DB);

				wsprintf(szQuery, "Update GuildInfo Set Count=%u, Member1=%u, Member2=%u, Member3=%u, Member4=%u Where [Id] = %u", lpGuildTable->sGuild.byCount, lpGuildTable->sGuild.byMember[0], lpGuildTable->sGuild.byMember[1], lpGuildTable->sGuild.byMember[2], lpGuildTable->sGuild.byMember[3], pPacket->dwGuildId);
				g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_COUNT, NULL, (BYTE)GAME_DB);

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
			__lstrcpyn(pMasterPacket.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);			
			pMasterPacket.dwGuildId			= pPacket->dwGuildId;		
			pMasterPacket.dwUserIndex		= pUser->m_dwUserIndex;
			pMasterPacket.dwMasterUserIndex	= dwUserIndex;
			pMasterPacket.byMember[0]		= lpGuildTable->sGuild.byMember[0];
			pMasterPacket.byMember[1]		= lpGuildTable->sGuild.byMember[1];
			pMasterPacket.byMember[2]		= lpGuildTable->sGuild.byMember[2];
			pMasterPacket.byMember[3]		= lpGuildTable->sGuild.byMember[3];
			pMasterPacket.byType			= 1;

			lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		
											
			while(lpGuildUserNode)
			{
				CWorldUser* pGuildUser = lpGuildUserNode->pData;
				
				if(!pGuildUser)
				{
					lpGuildUserNode = lpGuildUserNode->pNext;
					continue;
				}
				g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pMasterPacket, pMasterPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				lpGuildUserNode = lpGuildUserNode->pNext;
			}
		}
	}
}

void CmdGuildJoinMessage(CWorldUser* pUser ,char* pMsg, DWORD dwLength)
{
	CTWS_GUILDJOIN_MESSAGE* pPacket = (CTWS_GUILDJOIN_MESSAGE*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_GUILDJOIN_MESSAGE	pSendMessgae;
	WSTC_GUILD_JOINRESULT	pJoinPacket;
	CWorldUser* pRecvUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);
		
	memset(pSendMessgae.szCharacterName, 0, sizeof(pSendMessgae.szCharacterName));
	memset(pSendMessgae.szGuildName, 0, sizeof(pSendMessgae.szGuildName));	
	memset(pJoinPacket.szCharacterName, 0, sizeof(pJoinPacket.szCharacterName));

	if(!pRecvUser)
		return;

	BOOL bChk = FALSE;

	for(int i = 0; i < 4; i++)
	{
		if(__strcmp(pRecvUser->m_szCharacterName, pRecvUser->m_sGuildChk[i].szName)!=0)
		{
			if(pRecvUser->m_sGuildChk[i].bType==__GCTYPE_CLEN__)
			{
				bChk = TRUE;
				break;
			}
		}
	}

	if(bChk==FALSE)
	{
		if(pRecvUser->m_byType==0)
		{
			__lstrcpyn(pSendMessgae.szGuildName, pPacket->szGuildName, MAX_GUILD_REAL_NAME_LENGTH);	
			__lstrcpyn(pSendMessgae.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pSendMessgae.bType			= pPacket->bType;
			pSendMessgae.dwUserIndex	= pUser->m_dwUserIndex;			
			g_pNet->SendToUser(pRecvUser->m_dwConnectionIndex, (char*)&pSendMessgae, pSendMessgae.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
		else
		{
			// 에러 코드 //
			pJoinPacket.bErrorCode	= pRecvUser->m_byType;
			pJoinPacket.bResult		= FALSE;			
			pJoinPacket.dwUserIndex	= pRecvUser->m_dwUserIndex;
			__lstrcpyn(pJoinPacket.szCharacterName, pRecvUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pJoinPacket, pJoinPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}
	else
	{
		// 에러 코드 //		
		pJoinPacket.bErrorCode	= pRecvUser->m_byType;
		pJoinPacket.bResult		= FALSE;		
		pJoinPacket.dwUserIndex	= pRecvUser->m_dwUserIndex;
		__lstrcpyn(pJoinPacket.szCharacterName, pRecvUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pJoinPacket, pJoinPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CmdGuildRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILD_REQUEST* pPacket = (CTWS_GUILD_REQUEST*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pPacket->dwGuildId);

	if(lpGuildTable)
	{
		CWorldUser* pGuildUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

		if(pGuildUser)
		{
			if(lpGuildTable->pGuildUserHash)		 
			{
				ListNode<CWorldUser>* pGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();

				while(pGuildUserNode)
				{
					CWorldUser* pMasterUser =  pGuildUserNode->pData;

					if(pMasterUser)
					{
						if(pMasterUser->m_byRank==1)
						{
							WSTC_GUILD_REQUEST	pSendPacket;
							memset(pSendPacket.szName, 0, MAX_CHARACTER_NAME_REAL_LENGTH);
							memset(pSendPacket.szRequestName, 0, MAX_CHARACTER_NAME_REAL_LENGTH);
							__lstrcpyn(pSendPacket.szName, pGuildUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							__lstrcpyn(pSendPacket.szRequestName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
							pSendPacket.dwRequestUserIndex	= pPacket->dwRequestUserIndex;
							g_pNet->SendToUser(pMasterUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);							
							break;
						}
					}
					pGuildUserNode	= pGuildUserNode->pNext;
				}
			}
		}
	}	
}

void CmdGuildUserInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILDUSER_INFO* pPacket = (CTWS_GUILDUSER_INFO*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);
					
	if(lpGuildTable)
	{	
		if(lpGuildTable->pGuildUserHash)		 
		{
			ListNode<CWorldUser>* pGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();

			while(pGuildUserNode)
			{
				CWorldUser* pGuildUser =  pGuildUserNode->pData;

				if(pGuildUser)
				{
					if(pGuildUser->m_dwUserIndex==pPacket->dwUserIndex)
					{						
						DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pGuildUser->m_dwDungeonID);

						if(!pDungeon)	
						{
							WSTC_GUILDUSER_INFO pSendPacket;
							memset(pSendPacket.szCharName, 0, sizeof(pSendPacket.szCharName));
							__lstrcpyn(pSendPacket.szCharName, pGuildUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							pSendPacket.dwUserIndex		= pGuildUser->m_dwUserIndex;
							pSendPacket.wDungeonID		= 0;
							pSendPacket.byLayerIndex	= 0;
							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							return;
						}
						else
						{
							WSTDS_GUILDUSER_INFO pGuildInfo;							
							pGuildInfo.dwUserIndex		= pUser->m_dwUserIndex;
							pGuildInfo.dwUserIndexRv	= pGuildUser->m_dwUserIndex;												
							g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pGuildInfo, pGuildInfo.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							return;
						}						
					}
				}

				pGuildUserNode	= pGuildUserNode->pNext;
			}
		}
	}
}

void CmdGuildDataRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILD_DATA_REQUEST* pPacket = (CTWS_GUILD_DATA_REQUEST*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pPacket->dwGuildId);

	if(lpGuildTable)
	{
		WSTC_GUILD_DATA_REQUEST pSendPacket;
		memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
		__lstrcpyn(pSendPacket.szName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
		pSendPacket.dwGuildId	= pPacket->dwGuildId;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}	
}

void CmdGuildUserRequestFail(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILD_USERREQUEST_FAIL*	pPacket = (CTWS_GUILD_USERREQUEST_FAIL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_GUILD_USERREQUEST_FAIL		pFailPacket;

	CWorldUser* pGuildUser =  g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pGuildUser)
	{
		pFailPacket.byType	= 3;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


//////////////////////////////////////////////////////////////////////////
// PACKET		: WSTC_GUILDWAR_ERROR
// Date			: 2004. 09. 24. ( Written by minjin )
// Description	: 길드전 못하는 이유 전송하기
//////////////////////////////////////////////////////////////////////////
void SendToUserGuildWarFail(CWorldUser* pUser, BYTE byErrorCode)
{
	// 길드전 결과 사용자한테 전송하기..
	WSTC_GUILDWAR_ERRROR	pErrorPacket;
	pErrorPacket.byError	= byErrorCode;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

//////////////////////////////////////////////////////////////////////////
// Date			: 2004. 09. 24. ( Written by minjin )
// Description	: 길드 마스터가 있으면 길마 Pointer / 없으면 FALSE
//////////////////////////////////////////////////////////////////////////
CWorldUser* IsThereGuildMaster(ListNode<CWorldUser>* pGuildUserPos, const bool bIsOnlyGuildMaster)//: 050202 hwoarang 길마 외에 2등급까지 받는다
{
	CWorldUser*	pMasterUser	= NULL;

	while ( pGuildUserPos ) 
	{
		pMasterUser = pGuildUserPos->pData;

		if ( pMasterUser ) 
		{
			if( pMasterUser->m_byRank == 1 ) 
			{
				Log ( LOG_NORMAL, "IsThereGuildMaster() Pointer : %p", pMasterUser );
				return pMasterUser;
			} // if ( pMasterUser->m_byRank == 1 )
			if( !bIsOnlyGuildMaster 
			&&	pMasterUser->m_byRank == 2)
			{
				Log ( LOG_NORMAL, "IsThereGuildMaster() Sencond Pointer : %p", pMasterUser );
				return pMasterUser;
			}
		}
		else
		{
			return FALSE;
		}
		pGuildUserPos = pGuildUserPos->pNext;
	}

	// 리스트 끝까지 검색한거같은데.. 그렇다면..
	// 허거!! 길마가 엄따..!!! -_-;;
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// Date			: 2004. 09. 23. ( Written by minjin )
// Description	: 길드전 할수 있는지 없는지 검사
//				  할 수 있으면 GWE_GUILD_WAR_SUCCESS
//				  할수 없으면, GUILD_WAR_ERRCODE 값 리턴
//
//////////////////////////////////////////////////////////////////////////
// 길드전 조건
// 1. 한개 길드가 수행할 수 있는 최대 길드전 수는 8 개 (일본은 5 개) -> MAX_GUILD_WAR 로 define 되어있음
// 2. 길드 레벨이 3 이상이어야 함										MIN_GUILD_LEVEL
// 3. 길드 마스터가 접속되어 있어야 함									MIN_GUILD_MEMBER
// 4. 길드마스터를 제외한 나머지 길드원이 최소 한명 이상 있어야 함
//////////////////////////////////////////////////////////////////////////
int GuildWarCheckCondition( DWORD dwGuildID, DWORD dwDstGuildID )
{
	LP_GUILD_LIST	lpGuildList		= g_pGuildWarHash->GetData(dwGuildID);		// 신청한 길드
	LP_GUILD_LIST	lpGuildListDst	= g_pGuildWarHash->GetData(dwDstGuildID);	// 상대 길드

	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData(dwGuildID);	// 신청한 길드
	LP_GUILD_TABLE	lpGuildTableDst	= g_pGuildTableHash->GetData(dwDstGuildID);	// 상대 길드


	if( !lpGuildList )			// 신청한 길드의 길드리스트.. 이상..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildList is Invalidate. Check the GuildList TABLE" );
		return GWE_GUILD_DATA_ERROR;
	}

	if( !lpGuildListDst )		// 상대 길드의 길드리스트.. 이상..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildListDst is Invalidate. Check the GuildList TABLE" );
		return GWE_GUILD_DATA_ERROR;
	}

	if( !lpGuildTable )			// 신청한 길드의 길드테이블이 이상..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildTable is Invalidate. Check the GuildList TABLE" );
		return GWE_GUILD_DATA_ERROR;
	}

	if( !lpGuildTableDst )		// 상대 길드의 길드테이블이 이상..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildTableDst is Invalidate. Check the GuildList TABLE" );
		return GWE_GUILD_DATA_ERROR;
	}

	CWorldUser* pMasterUser;
	CWorldUser* pDstMasterUser;

	int i;	// -_-;;

	for( i = 0; i < MAX_GUILD_WAR; i++ ) 
	{
		// 벌써 우리랑 길드전 하구있는 길드에 대고 길드전 신청하면 무시!!
		if( lpGuildList->sGuildNode[i].dwGuildId == dwDstGuildID ) 	return GWE_NOW_ON_GUILD_WAR;
	}

	// 우선 우리 길드 상태부터 체크..
	for( i = 0; i < MAX_GUILD_WAR; i++ ) // 최대 길드전 수만큼 하구 있는거 아니야?
	{
		if( i >= MAX_GUILD_WAR )	
		{
			return GWE_OUTOFBOUNDS_MAX_WAR;
		}
		if( lpGuildList->sGuildNode[i].dwGuildId == 0 )	break;	// 우리 길드
	}

	pMasterUser = IsThereGuildMaster( lpGuildTable->pGuildUserHash->GetHead() );

	if( !pMasterUser )														return GWE_NOT_GUILD_MASTER;
	if( lpGuildTable->sGuild.wLevel < MIN_GUILD_LEVEL )						return GWE_MUST_LEVEL3;
	if( lpGuildTable->pGuildUserHash->GetCount() <= MIN_GUILD_MEMBER )		return GWE_MORE_GUILD_MEMBER;


	// 상대 길드 상태 체크..
	for( i = 0; i < MAX_GUILD_WAR; i++ ) // 최대 길드전 수만큼 하구 있는거 아니야?
	{
		if( i >= MAX_GUILD_WAR )
		{
			return GWE_VS_OUTOFBOUNDS_MAX_WAR;
		}
		if( lpGuildListDst->sGuildNode[i].dwGuildId == 0 )	break;	// 상대 길드
	}

	pDstMasterUser = IsThereGuildMaster( lpGuildTableDst->pGuildUserHash->GetHead() );

	if( !pDstMasterUser )													return GWE_VS_NOT_EXIST_MASTER;
	if( lpGuildTableDst->sGuild.wLevel < MIN_GUILD_LEVEL )					return GWE_VS_MUST_LEVEL3;
	if( lpGuildTableDst->pGuildUserHash->GetCount() <= MIN_GUILD_MEMBER )	return GWE_VS_MORE_GUILD_MEMBER;


	// 모두 무사통과.
	// 길드전 해라..
	return GWE_GUILD_WAR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// PACKET		: CTWS_GUILDWAR_REQUEST
// Date			: 2004. 09. 23. ( Modified by minjin )
// Description	: 길드 마스터가 아닌 길드원이 다른 길드원에게
//				  길드전을 요청했을 경우 처리
// ETC			: 기존 함수는 맨 밑에다가 옮겨놨다......
//////////////////////////////////////////////////////////////////////////
void CmdGuildWarRequest(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	// 길마가 아닌 유저가 길드전 하자구 했을때
	CTWS_GUILDWAR_REQUEST* pPacket = (CTWS_GUILDWAR_REQUEST*) pMsg;

	// 패킷 검사
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	if( pPacket->dwGuildId		== 0 )	return;
	if( pPacket->dwDstGuildId	== 0 ) 	return;

	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData(pPacket->dwGuildId);		// 신청한 길드
	LP_GUILD_TABLE	lpGuildTableDst	= g_pGuildTableHash->GetData(pPacket->dwDstGuildId);	// 상대 길드

	BYTE		byErrType		= 0;		// 앞으로 쓸 에러코드 변수 초기화
	CWorldUser*	pGuildMaster;				// 우리편 길마 포인터
	CWorldUser*	pDstGuildMaster;			// 상대편 길마 포인터
	
	byErrType = (BYTE)GuildWarCheckCondition(	pPacket->dwGuildId, pPacket->dwDstGuildId );
	
	if ( byErrType == GWE_GUILD_DATA_ERROR )	return;

	pGuildMaster	= IsThereGuildMaster( lpGuildTable->pGuildUserHash->GetHead() );
	pDstGuildMaster = IsThereGuildMaster( lpGuildTableDst->pGuildUserHash->GetHead() );

	if ( byErrType != GWE_GUILD_WAR_SUCCESS )	// 길드전 할 수 있으면 GWE_GUILD_WAR_SUCCESS 리턴한다..
	{
		SendToUserGuildWarFail( pUser, byErrType );
	}
	else
	{	
		// 길마한테 길드전 하자구 알린다
		WSTC_GUILDWAR_REQUEST pSendPacket;
		__lstrcpyn( pSendPacket.szGuildName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH );
		__lstrcpyn( pSendPacket.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH );
		pSendPacket.dwGuildId		= pPacket->dwGuildId;
		pSendPacket.dwDstGuildId	= pPacket->dwDstGuildId;
		pSendPacket.dwUserIndex		= pUser->m_dwUserIndex;
		g_pNet->SendToUser( pGuildMaster->m_dwConnectionIndex, (char*) &pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION );
	}
}

//////////////////////////////////////////////////////////////////////////
// PACKET		: CTWS_GUILDWAR_REQUESTRT
// Date			: 2004. 09. 24. ( Modified by minjin )
// Description	: 길드 마스터가 길드전 하자고 하거나,
//				  길드 마스터가 아닌 사람이 길드전을 하자고 해서 길드 마스터가 승인했을때,
//				  길드전 선포해달라는 메시지..
// ETC			: 기존 함수는 맨 밑에다가 옮겨놨다......
//////////////////////////////////////////////////////////////////////////
void CmdGuildWarMasterRt(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILDWAR_REQUESTRT*	pPacket = (CTWS_GUILDWAR_REQUESTRT*)pMsg;
	WSTC_GUILDWAR_ERRROR		pErrorPacket;
	
	// 패킷 검사
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	if( pPacket->dwGuildId		== 0 )	return;
	if( pPacket->dwDstGuildId	== 0 ) 	return;

	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData(pPacket->dwGuildId);		// 신청한 길드
	LP_GUILD_TABLE	lpGuildTableDst	= g_pGuildTableHash->GetData(pPacket->dwDstGuildId);	// 상대 길드

	BYTE		byErrType		= 0;		// 앞으로 쓸 에러코드 변수 초기화
	CWorldUser*	pGuildMaster;				// 우리편 길마 포인터
	CWorldUser*	pDstGuildMaster;			// 상대편 길마 포인터
	
	byErrType = (BYTE)GuildWarCheckCondition(	pPacket->dwGuildId, pPacket->dwDstGuildId );

	if ( byErrType == GWE_GUILD_DATA_ERROR )	return;

	pGuildMaster	= IsThereGuildMaster( lpGuildTable->pGuildUserHash->GetHead() );
	pDstGuildMaster = IsThereGuildMaster( lpGuildTableDst->pGuildUserHash->GetHead() );

	if( byErrType != GWE_GUILD_WAR_SUCCESS )	// 길드전 할 수 있으면 GWE_GUILD_WAR_SUCCESS 리턴한다..
	{
		SendToUserGuildWarFail( pUser, byErrType );
	}
	else
	{
		// 길드전 해보자
		WSTC_GUILDWAR_JOIN pSendPacket;
		__lstrcpyn(pSendPacket.szGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);							
		pSendPacket.dwGuildId		= pPacket->dwGuildId;
		pSendPacket.dwDstGuildId	= pPacket->dwDstGuildId;
		pSendPacket.dwUserIndex		= pPacket->dwUserIndex;
		g_pNet->SendToUser(pDstGuildMaster->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		WSTDS_GUILDWAR_MESSAGE pMessage;
		__lstrcpyn(pMessage.szGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
		__lstrcpyn(pMessage.szGuildNameDst, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
		pMessage.byType	= 0;
		
		g_pServerTable->BroadCast((char*)&pMessage, pMessage.GetPacketSize());
	}
}

void CmdGuildWarCreate(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILDWAR_CREATE*	pPacket = (CTWS_GUILDWAR_CREATE*)pMsg;	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_GUILDWAR_ERRROR	pErrorPacket;
	char					szQuery[0xff]={0,};
	BYTE					byGuildListIndex	= 0;
	BYTE					byGuildListDstIndex = 0;
	
	if(pPacket->dwGuildId==0)
		return;
	if(pPacket->dwDstGuildId==0)
		return;

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 1, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
			
	LP_GUILD_LIST lpGuildList		= g_pGuildWarHash->GetData(pPacket->dwGuildId);
	LP_GUILD_LIST lpGuildListDst	= g_pGuildWarHash->GetData(pPacket->dwDstGuildId);

	if(lpGuildList && lpGuildListDst)
	{

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 2, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif

		BOOL	bChk		= FALSE;
//		BYTE	byErrType	= 0;

		if(pPacket->dwGuildId==pPacket->dwDstGuildId)
		{
			pErrorPacket.byError = 10;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 3, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif

		for(int i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildList->sGuildNode[i].dwGuildId==pPacket->dwDstGuildId)
			{
				pErrorPacket.byError = 9;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
		}

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 4, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
		
		for(i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildList->sGuildNode[i].dwGuildId==0)
			{
				bChk = TRUE;
				break;
			}
		}

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 5, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
		if(bChk)
		{
			LP_GUILD_TABLE lpGuildTable		= g_pGuildTableHash->GetData(pPacket->dwGuildId);
			LP_GUILD_TABLE lpGuildTableDst	= g_pGuildTableHash->GetData(pPacket->dwDstGuildId);

			if(lpGuildTable && lpGuildTableDst)
			{

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 6, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif

				if(pUser->m_byRank!=1)
				{					
					pErrorPacket.byError	= 5;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 7, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
				if(lpGuildTable->sGuild.wLevel<3)
				{
					pErrorPacket.byError = 2;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 8, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
				if(lpGuildTable->pGuildUserHash->GetCount()<=1)
				{
					pErrorPacket.byError = 3;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 9, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
				for(i = 0; i < MAX_GUILD_WAR; i++)
				{					
					if(lpGuildListDst->sGuildNode[i].dwGuildId==0)
					{
						bChk = TRUE;
						break;
					}
				}	
				
				if(!bChk)
				{
					pErrorPacket.byError	= 6;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 10, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
				if(lpGuildTable->sGuild.wLevel<3)
				{
					pErrorPacket.byError = 7;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
				
#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 11, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
				if(lpGuildTable->pGuildUserHash->GetCount()<=1)
				{
					pErrorPacket.byError = 8;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
								
#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 12, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
				ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();

				while(pGuildUserPos)
				{
					CWorldUser* pMasterUser = pGuildUserPos->pData;

					if(pMasterUser)
					{
						if(pMasterUser->m_byRank==1)
						{

#ifdef GUILD_WARTEST
	Log(LOG_JUST_FILEWRITE, "LogTest 13, GuildId1 : %u, GuildId2 : %u", pPacket->dwGuildId, pPacket->dwDstGuildId);
#endif
							// 길드리스트 추가 //							
							for(i = 0; i < MAX_GUILD_WAR; i++)
							{
								if(lpGuildList->sGuildNode[i].dwGuildId==0)
								{
									lpGuildList->sGuildNode[i].dwGuildId	= lpGuildTableDst->sGuild.dwGuildId;
									lpGuildList->sGuildNode[i].wPoint1		= 0;
									lpGuildList->sGuildNode[i].wPoint2		= 0;									
									byGuildListIndex						= (BYTE)i;
									
									time(&lpGuildList->sGuildNode[i].sTime);
									localtime(&lpGuildList->sGuildNode[i].sTime);
									break;
								}
							}

							for(i = 0; i < MAX_GUILD_WAR; i++)
							{
								if(lpGuildListDst->sGuildNode[i].dwGuildId==0)
								{
									lpGuildListDst->sGuildNode[i].dwGuildId		= lpGuildTable->sGuild.dwGuildId;
									lpGuildListDst->sGuildNode[i].wPoint1		= 0;
									lpGuildListDst->sGuildNode[i].wPoint2		= 0;
									byGuildListDstIndex							= (BYTE)i;

									time(&lpGuildListDst->sGuildNode[i].sTime);
									localtime(&lpGuildListDst->sGuildNode[i].sTime);
									break;
								}
							}

							// 디비 업데이트 //
							DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);

							if(!pBinding)
							{
								Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
								return;
							}

							GUILD_NODE	sGuildNode[MAX_GUILD_WAR];
							GUILD_NODE	sGuildNodeDst[MAX_GUILD_WAR];
							memset(szQuery, 0, sizeof(szQuery));
							memset(sGuildNode, 0, sizeof(sGuildNode));
							memcpy(sGuildNode, lpGuildList->sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR);

							pBinding[0].obValue		= 0;
							pBinding[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
							pBinding[0].wType		= DBTYPE_BYTES;					
							
							wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTable->sGuild.dwGuildId);
							g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR, 1, FALSE, 0, NULL, (BYTE)GAME_DB);

							DBBINDING* pBindingDst = g_pDb->THCreateParamInfo(1);

							if(!pBindingDst)
							{
								Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
								return;
							}
														
							memset(szQuery, 0, sizeof(szQuery));
							memset(sGuildNodeDst, 0, sizeof(sGuildNodeDst));
							memcpy(sGuildNodeDst, lpGuildListDst->sGuildNode, sizeof(GUILD_NODE)*MAX_GUILD_WAR);

							pBindingDst[0].obValue	= 0;
							pBindingDst[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
							pBindingDst[0].wType	= DBTYPE_BYTES;					
						
							wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTableDst->sGuild.dwGuildId);
							g_pDb->THExecuteSQLByParam(szQuery, pBindingDst, sGuildNodeDst, sizeof(GUILD_NODE)*MAX_GUILD_WAR, 1, FALSE, 0, NULL, (BYTE)GAME_DB);

							// 던전 서버에 추가 //
							WSTDS_GUILD_LIST pserverPacket;
							pserverPacket.dwGuildId		= pPacket->dwGuildId;
							pserverPacket.dwDstGuildId	= pPacket->dwDstGuildId;							
							g_pServerTable->BroadCast((char*)&pserverPacket, pserverPacket.GetPacketSize());

							// 양쪽 길드원들에게 추가 //
							WSTC_GUILD_LIST pUserPacket;
							time(&pUserPacket.sCurTime);
							pUserPacket.dwGuildId	= pPacket->dwDstGuildId;							
							pUserPacket.wPoint1		= lpGuildList->sGuildNode[byGuildListIndex].wPoint1;
							pUserPacket.wPoint2		= lpGuildList->sGuildNode[byGuildListIndex].wPoint2;
							pUserPacket.sTime		= lpGuildList->sGuildNode[byGuildListIndex].sTime;
							memset(pUserPacket.szName, 0, sizeof(pUserPacket.szName));
							__lstrcpyn(pUserPacket.szName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
							
							ListNode<CWorldUser>* pUserPos = lpGuildTable->pGuildUserHash->GetHead();

							while(pUserPos)
							{
								CWorldUser* pGuildUser = pUserPos->pData;

								if(pGuildUser)
									g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pUserPacket, pUserPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
								
								pUserPos = pUserPos->pNext;
							}

							pUserPacket.dwGuildId	= pPacket->dwGuildId;							
							pUserPacket.wPoint1		= lpGuildListDst->sGuildNode[byGuildListDstIndex].wPoint1;
							pUserPacket.wPoint2		= lpGuildListDst->sGuildNode[byGuildListDstIndex].wPoint2;
							pUserPacket.sTime		= lpGuildListDst->sGuildNode[byGuildListDstIndex].sTime;
							memset(pUserPacket.szName, 0, sizeof(pUserPacket.szName));
							__lstrcpyn(pUserPacket.szName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
																					
							pUserPos = lpGuildTableDst->pGuildUserHash->GetHead();

							while(pUserPos)
							{
								CWorldUser* pGuildUser = pUserPos->pData;

								if(pGuildUser)
									g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pUserPacket, pUserPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
								
								pUserPos = pUserPos->pNext;
							}
							// 전체 공지 //
							WSTC_DGCHAT_USER pAllUserPacket;	
							pAllUserPacket.nIndex	= 5;
							memset(pAllUserPacket.szDstId,	0, sizeof(pAllUserPacket.szDstId));
							memset(pAllUserPacket.szId,		0, sizeof(pAllUserPacket.szId));
							memset(pAllUserPacket.szMessage, 0, sizeof(pAllUserPacket.szMessage));
							
							wsprintf(pAllUserPacket.szMessage, g_szGuildBattleText[0], lpGuildTableDst->sGuild.szGuildName, lpGuildTable->sGuild.szGuildName, lpGuildTable->sGuild.szGuildName, lpGuildTableDst->sGuild.szGuildName);

							g_pNet->BroadcastUser((char*)&pAllUserPacket, pAllUserPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							
							LP_GUILDWAR_TIME lpGuildWarTimeNew = (LP_GUILDWAR_TIME)LALAlloc(g_pGuildWarTimePool);
							GuildWarTime(lpGuildWarTimeNew, pPacket->dwGuildId, pPacket->dwDstGuildId);
							lpGuildWarTimeNew->sTime	= lpGuildList->sGuildNode[byGuildListIndex].sTime;
							g_pGuildWarTimeList->AddTail(lpGuildWarTimeNew);
															
							return;
						}
					}
					pGuildUserPos = pGuildUserPos->pNext;
				}
				pErrorPacket.byError = 0;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
		}		
		else
		{
			pErrorPacket.byError = 1;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
	}
}

/*
void CmdGuildWarRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILDWAR_REQUEST* pPacket = (CTWS_GUILDWAR_REQUEST*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	if(pPacket->dwGuildId==0)
		return;
	if(pPacket->dwDstGuildId==0)
		return;
		
	// 길드전 신청 //
	LP_GUILD_LIST lpGuildList = g_pGuildWarHash->GetData(pPacket->dwGuildId);

	if(lpGuildList)
	{
		BOOL	bChk		= FALSE;
		BYTE	byErrType	= 0;

		for(int i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildList->sGuildNode[i].dwGuildId==pPacket->dwDstGuildId)
				return;
		}

		for(i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildList->sGuildNode[i].dwGuildId==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk)
		{
			LP_GUILD_TABLE lpGuildTable		= g_pGuildTableHash->GetData(pPacket->dwGuildId);
			LP_GUILD_TABLE lpGuildTableDst	= g_pGuildTableHash->GetData(pPacket->dwDstGuildId);

			if(lpGuildTable && lpGuildTableDst)
			{
				ListNode<CWorldUser>* pGuildUserPos = lpGuildTableDst->pGuildUserHash->GetHead();

				bChk = FALSE;

				while(pGuildUserPos)
				{
					CWorldUser* pMasterUser = pGuildUserPos->pData;

					if(pMasterUser)
					{
						if(pMasterUser->m_byRank==1)
						{
							bChk = TRUE;
							break;
						}
					}
					pGuildUserPos = pGuildUserPos->pNext;
				}

				if(!bChk)
				{
					byErrType = 0;
				}
				else
				{
					 bChk = FALSE;

					if(lpGuildTableDst->sGuild.wLevel>=3)
					{
						if(lpGuildTableDst->pGuildUserHash->GetCount()>1)
						{
							for(i = 0; i < MAX_GUILD_WAR; i++)
							{
								if(lpGuildList->sGuildNode[i].dwGuildId==0)
								{
									bChk = TRUE;
									break;
								}
							}
						}						
						else
							byErrType = 3;
					}
					else
						byErrType = 2;
					
					if(!bChk)
						byErrType = 0;
				}							

				if(bChk)
				{								
					if(lpGuildTable->sGuild.wLevel>=3)
					{
						if(lpGuildTable->pGuildUserHash->GetCount()>1)
						{
							ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();

							while(pGuildUserPos)
							{
								CWorldUser* pMasterUser = pGuildUserPos->pData;

								if(pMasterUser)
								{
									if(pMasterUser->m_byRank==1)
									{
										WSTC_GUILDWAR_REQUEST pSendPacket;
										memset(pSendPacket.szGuildName, 0, sizeof(pSendPacket.szGuildName));
										memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
										__lstrcpyn(pSendPacket.szGuildName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
										__lstrcpyn(pSendPacket.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
										pSendPacket.dwGuildId		= pPacket->dwGuildId;
										pSendPacket.dwDstGuildId	= pPacket->dwDstGuildId;
										pSendPacket.dwUserIndex		= pUser->m_dwUserIndex;
										g_pNet->SendToUser(pMasterUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);																			
										return;
									}								
								}
								pGuildUserPos = pGuildUserPos->pNext;
							}
						}					
					}
				}
				else
				{
					// Error 패킷 //
					WSTC_GUILDWAR_ERRROR pErrorPacket;
					pErrorPacket.byError	= byErrType;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}		
	}
}

void CmdGuildWarMasterRt(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILDWAR_REQUESTRT*	pPacket = (CTWS_GUILDWAR_REQUESTRT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_GUILDWAR_ERRROR		pErrorPacket;
	
	if(pPacket->dwGuildId==0)
		return;
	if(pPacket->dwDstGuildId==0)
		return;
			
	LP_GUILD_LIST lpGuildList		= g_pGuildWarHash->GetData(pPacket->dwGuildId);
	LP_GUILD_LIST lpGuildListDst	= g_pGuildWarHash->GetData(pPacket->dwDstGuildId);

	if(lpGuildList && lpGuildListDst)
	{
		BOOL	bChk		= FALSE;
//		BYTE	byErrType	= 0;

		if(pPacket->dwGuildId==pPacket->dwDstGuildId)
		{
			pErrorPacket.byError = 10;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		for(int i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildList->sGuildNode[i].dwGuildId==pPacket->dwDstGuildId)
			{
				pErrorPacket.byError = 9;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
		}

		for(i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(lpGuildList->sGuildNode[i].dwGuildId==0)
			{
				bChk = TRUE;
				break;
			}
		}

		if(bChk)
		{
			LP_GUILD_TABLE lpGuildTable		= g_pGuildTableHash->GetData(pPacket->dwGuildId);
			LP_GUILD_TABLE lpGuildTableDst	= g_pGuildTableHash->GetData(pPacket->dwDstGuildId);

			if(lpGuildTable && lpGuildTableDst)
			{
				if(pUser->m_byRank!=1)
				{					
					pErrorPacket.byError	= 5;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				if(lpGuildTable->sGuild.wLevel<3)
				{
					pErrorPacket.byError = 7;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				if(lpGuildTable->pGuildUserHash->GetCount()<=1)
				{
					pErrorPacket.byError = 8;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				for(int i = 0; i < MAX_GUILD_WAR; i++)
				{
					if(pPacket->dwGuildId==pPacket->dwDstGuildId)
						return;
				}

				for(i = 0; i < MAX_GUILD_WAR; i++)
				{					
					if(lpGuildListDst->sGuildNode[i].dwGuildId==0)
					{
						bChk = TRUE;
						break;
					}
				}	
				
				if(!bChk)
				{
					pErrorPacket.byError	= 1;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				if(lpGuildTable->sGuild.wLevel<3)
				{
					pErrorPacket.byError = 2;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
				
				if(lpGuildTable->pGuildUserHash->GetCount()<=1)
				{
					pErrorPacket.byError = 3;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
													
				ListNode<CWorldUser>* pGuildUserPos = lpGuildTableDst->pGuildUserHash->GetHead();

				while(pGuildUserPos)
				{
					CWorldUser* pMasterUser = pGuildUserPos->pData;

					if(pMasterUser)
					{
						if(pMasterUser->m_byRank==1)
						{
							WSTC_GUILDWAR_JOIN pSendPacket;
							memset(pSendPacket.szGuildName, 0, sizeof(pSendPacket.szGuildName));
							__lstrcpyn(pSendPacket.szGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);							
							pSendPacket.dwGuildId		= pPacket->dwGuildId;
							pSendPacket.dwDstGuildId	= pPacket->dwDstGuildId;
							pSendPacket.dwUserIndex		= pPacket->dwUserIndex;
							g_pNet->SendToUser(pMasterUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

							// 길드전 신청 //
							WSTDS_GUILDWAR_MESSAGE pMessage;
							memset(pMessage.szGuildName, 0, sizeof(pMessage.szGuildName));
							memset(pMessage.szGuildNameDst, 0, sizeof(pMessage.szGuildNameDst));
							__lstrcpyn(pMessage.szGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
							__lstrcpyn(pMessage.szGuildNameDst, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
							pMessage.byType	= 0;							
							g_pServerTable->BroadCast((char*)&pMessage, pMessage.GetPacketSize());
							return;
						}								
					}
					pGuildUserPos = pGuildUserPos->pNext;
				}
				pErrorPacket.byError = 0;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				return;
			}
		}		
		else
		{
			pErrorPacket.byError	= 6;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
	}
}
//*/

void CmdGuildWarErrorRt(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILDWAR_ERRROR* pPacket = (CTWS_GUILDWAR_ERRROR*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	CWorldUser* pSendUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pSendUser)
	{
		if(pPacket->byError==1)
		{
			WSTC_GUILDWAR_ERRROR pSendPacket;
			memset(pSendPacket.szNameTemp, 0, sizeof(pSendPacket.szNameTemp));
			__lstrcpyn(pSendPacket.szNameTemp, pPacket->szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
			pSendPacket.byError	= 4;
			g_pNet->SendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
		else if(pPacket->byError==2)
		{
			WSTC_GUILDWAR_ERRROR pSendPacket;
			memset(pSendPacket.szNameTemp, 0, sizeof(pSendPacket.szNameTemp));
			__lstrcpyn(pSendPacket.szNameTemp, pPacket->szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
			pSendPacket.byError	= 4;
			g_pNet->SendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
		}		

		if(pPacket->byError==1 || pPacket->byError==2)
		{
			LP_GUILD_TABLE lpGuildTable		= g_pGuildTableHash->GetData(pUser->m_dwGuildId);
			LP_GUILD_TABLE lpGuildTableDst	= g_pGuildTableHash->GetData(pSendUser->m_dwGuildId);

			if(lpGuildTable && lpGuildTableDst)
			{
				// 길드전 신청 //
				WSTDS_GUILDWAR_MESSAGE pMessage;
				memset(pMessage.szGuildName, 0, sizeof(pMessage.szGuildName));
				memset(pMessage.szGuildNameDst, 0, sizeof(pMessage.szGuildNameDst));
				__lstrcpyn(pMessage.szGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
				__lstrcpyn(pMessage.szGuildNameDst, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
				pMessage.byType	= 1;				
				g_pServerTable->BroadCast((char*)&pMessage, pMessage.GetPacketSize());
			}			
		}
	}
}


void CmdGuildWarExit(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILDWAR_EXIT* pPacket = (CTWS_GUILDWAR_EXIT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	LP_GUILD_LIST	lpGuildList		= g_pGuildWarHash->GetData(pPacket->dwGuildId);
	LP_GUILD_LIST	lpGuildListDst	= g_pGuildWarHash->GetData(pPacket->dwGuildIdDst);
	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData(pPacket->dwGuildId);
	LP_GUILD_TABLE	lpGuildTableDst	= g_pGuildTableHash->GetData(pPacket->dwGuildIdDst);

	char	szQuery[0xff]={0,};
	char	szWinGuildName[MAX_GUILD_NAME_LENGTH]={0,};
	char	szLoseGuildName[MAX_GUILD_NAME_LENGTH]={0,};	
	memset(szWinGuildName, 0, sizeof(szWinGuildName));
	memset(szLoseGuildName, 0, sizeof(szWinGuildName));		

	if(lpGuildList && lpGuildListDst && lpGuildTable && lpGuildTableDst)
	{
		for(int i = 0; i < MAX_GUILD_WAR; i++)
		{
			if(pPacket->dwGuildIdDst==lpGuildList->sGuildNode[i].dwGuildId)
			{
				struct tm	when;
				time_t		now;
//				BYTE		byIndex = 0;
						
				time(&now);
				when = *localtime(&now);				

				int nTime	= (int)difftime(now, lpGuildList->sGuildNode[i].sTime);
				int nHou	= (nTime/60)/60;
									
				if(nHou>=GUILDWAR_TIME)
				{
					for(int j = 0; j < MAX_GUILD_WAR; j++)
					{
						if(lpGuildListDst->sGuildNode[j].dwGuildId==pPacket->dwGuildId)
						{
							if(lpGuildList->sGuildNode[i].wPoint1==lpGuildListDst->sGuildNode[j].wPoint2)
							{
								if(lpGuildList->sGuildNode[i].wPoint2==lpGuildListDst->sGuildNode[j].wPoint1)
								{
									if(lpGuildList->sGuildNode[i].wPoint1>lpGuildList->sGuildNode[i].wPoint2)
									{
										lpGuildTable->sGuild.dwPoint	+= 3;
										lpGuildTableDst->sGuild.dwPoint	+= 1;
										__lstrcpyn(szWinGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
										__lstrcpyn(szLoseGuildName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
									}
									else if(lpGuildList->sGuildNode[i].wPoint1<lpGuildList->sGuildNode[i].wPoint2)
									{
										lpGuildTable->sGuild.dwPoint	+= 1;
										lpGuildTableDst->sGuild.dwPoint	+= 3;
										__lstrcpyn(szLoseGuildName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
										__lstrcpyn(szWinGuildName, lpGuildTableDst->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
									}
									
									POSITION_ pos = g_pRankList->GetHeadPosition();

									while(pos)
									{
										LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(pos);

										if(lpGuildWarRank)
										{
											if(lpGuildWarRank->dwGuildId==lpGuildTable->sGuild.dwGuildId)
												lpGuildWarRank->dwPoint = lpGuildTable->sGuild.dwPoint;
											else if(lpGuildWarRank->dwGuildId==lpGuildTableDst->sGuild.dwGuildId)
												lpGuildWarRank->dwPoint = lpGuildTableDst->sGuild.dwPoint;
										}
									}
													
									// DB Update //
									wsprintf(szQuery, "CZP_GUILDLIST_POINT %u, %u", lpGuildTable->sGuild.dwGuildId, lpGuildTable->sGuild.dwPoint);
									g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);

									wsprintf(szQuery, "CZP_GUILDLIST_POINT %u, %u", lpGuildTableDst->sGuild.dwGuildId, lpGuildTableDst->sGuild.dwPoint);
									g_pDb->THExecuteSQL(szQuery, FALSE, 0, NULL, (BYTE)GAME_DB);									

									// 순위 재검색 //
									POSITION_ posRank = g_pRankList->GetHeadPosition();

									DWORD dwPrevGuildId = 0;

									while(posRank)
									{
										POSITION_ posPrev	= posRank;
										POSITION_ posDelete	= posRank;
										POSITION_ posChange	= NULL; 

										LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(posRank);

										if(lpGuildWarRank)
										{
											if(!dwPrevGuildId)
												dwPrevGuildId = lpGuildWarRank->dwGuildId;

											while(posPrev)
											{
												POSITION_ posGuild = posPrev;

												LP_GUILDWAR_RANK lpGuildWarPrev = (LP_GUILDWAR_RANK)g_pRankList->GetPrev(posPrev);

												if(lpGuildWarPrev)
												{
													if(lpGuildWarPrev->dwGuildId!=lpGuildWarRank->dwGuildId)
													{
														if(lpGuildWarPrev->dwPoint<lpGuildWarRank->dwPoint)
															posChange = posGuild;
														else if(lpGuildWarPrev->dwPoint==lpGuildWarRank->dwPoint)
														{
															if(lpGuildWarPrev->dwExp<lpGuildWarRank->dwExp)
																posChange = posGuild;
														}
													}												
												}
											}

											if(posChange)
											{
												g_pRankList->RemoveAt(posDelete);
												g_pRankList->InsertBefore(posChange, lpGuildWarRank);
											}
										}
									}										

									// 1등이 변경되면 던전서버에 알려준다 //
									posRank = g_pRankList->GetHeadPosition();

									if(posRank)
									{
										LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(posRank);

										if(lpGuildWarRank)
										{
											if(dwPrevGuildId!=0 && lpGuildWarRank->dwGuildId!=dwPrevGuildId)
											{
												WSTDS_GUILD_RANK pRankPacket;
												pRankPacket.dwGuildId = lpGuildWarRank->dwGuildId;												
												g_pServerTable->BroadCast((char*)&pRankPacket, pRankPacket.GetPacketSize());												
											}
										}
										

										LP_GUILD_TABLE lpGuildTableRank = g_pGuildTableHash->GetData(dwPrevGuildId);

										if(lpGuildTableRank)
										{													
											ListNode<CWorldUser>* lpGuildUserRankNode = lpGuildTableRank->pGuildUserHash->GetHead();

											while(lpGuildUserRankNode)
											{
												CWorldUser* pGuildUser = lpGuildUserRankNode->pData;

												if(pGuildUser)
												{
													WSTDS_GUILDUSER_RANK pGuildUserRankPacket;
													pGuildUserRankPacket.dwUserIndex	= pGuildUser->m_dwUserIndex;
													pGuildUserRankPacket.dwGuildId		= pGuildUser->m_dwGuildId;
													pGuildUserRankPacket.byGuildFlag			= 0;
													pGuildUserRankPacket.byChk			= 0;

													DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pGuildUser->m_dwDungeonID);

													if(pDungeon)
														g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pGuildUserRankPacket, pGuildUserRankPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
												}

												lpGuildUserRankNode = lpGuildUserRankNode->pNext;
											}
										}

										LP_GUILD_TABLE lpGuildTableRankDst = g_pGuildTableHash->GetData(lpGuildWarRank->dwGuildId);

										if(lpGuildTableRankDst)
										{													
											ListNode<CWorldUser>* lpGuildUserRankNode = lpGuildTableRankDst->pGuildUserHash->GetHead();

											while(lpGuildUserRankNode)
											{
												CWorldUser* pGuildUser = lpGuildUserRankNode->pData;

												if(pGuildUser)
												{
													WSTDS_GUILDUSER_RANK pGuildUserRankPacket;
													pGuildUserRankPacket.dwUserIndex	= pGuildUser->m_dwUserIndex;
													pGuildUserRankPacket.dwGuildId		= pGuildUser->m_dwGuildId;
													pGuildUserRankPacket.byGuildFlag			= 1;
													pGuildUserRankPacket.byChk			= 0;

													DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pGuildUser->m_dwDungeonID);

													if(pDungeon)
														g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pGuildUserRankPacket, pGuildUserRankPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
												}
												lpGuildUserRankNode = lpGuildUserRankNode->pNext;
											}
										}
									}
									
									// 종료 패킷 //
									WSTC_GUILDWAR_EXIT pExitPacket;
									pExitPacket.dwGuildId	= lpGuildList->sGuildNode[i].dwGuildId;
									pExitPacket.dwPoint		= lpGuildTable->sGuild.dwPoint;									
									
									if(lpGuildList->sGuildNode[i].wPoint1>lpGuildList->sGuildNode[i].wPoint2)
										pExitPacket.byType = 0;
									else if(lpGuildList->sGuildNode[i].wPoint1<lpGuildList->sGuildNode[i].wPoint2)
										pExitPacket.byType = 1;
									else
										pExitPacket.byType = 2;

									ListNode<CWorldUser>* lpGuildUserNode = lpGuildTable->pGuildUserHash->GetHead();		

									while(lpGuildUserNode)
									{
										CWorldUser* pGuildUser = lpGuildUserNode->pData;
								
										if(pGuildUser)
											g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pExitPacket, pExitPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
										
										lpGuildUserNode = lpGuildUserNode->pNext;
									}			

									pExitPacket.dwGuildId	= lpGuildListDst->sGuildNode[j].dwGuildId;
									pExitPacket.dwPoint		= lpGuildTableDst->sGuild.dwPoint;									

									if(lpGuildListDst->sGuildNode[j].wPoint1>lpGuildListDst->sGuildNode[j].wPoint2)
										pExitPacket.byType = 0;
									else if(lpGuildListDst->sGuildNode[j].wPoint1<lpGuildListDst->sGuildNode[j].wPoint2)
										pExitPacket.byType = 1;
									else
										pExitPacket.byType = 2;
									
									ListNode<CWorldUser>* lpGuildUserNodeSub = lpGuildTableDst->pGuildUserHash->GetHead();		

									while(lpGuildUserNodeSub)
									{
										CWorldUser* pGuildUser = lpGuildUserNodeSub->pData;
								
										if(pGuildUser)
											g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pExitPacket, pExitPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
										
										lpGuildUserNodeSub = lpGuildUserNodeSub->pNext;
									}

									// 전체 공지 //
									WSTC_DGCHAT_USER pAllUserPacket;	
									pAllUserPacket.nIndex	= 5;
									memset(pAllUserPacket.szDstId,	0, sizeof(pAllUserPacket.szDstId));
									memset(pAllUserPacket.szId,		0, sizeof(pAllUserPacket.szId));
									memset(pAllUserPacket.szMessage, 0, sizeof(pAllUserPacket.szMessage));																		

									if(lpGuildList->sGuildNode[i].wPoint1>lpGuildList->sGuildNode[i].wPoint2)
										wsprintf(pAllUserPacket.szMessage, g_szGuildBattleText[1], szWinGuildName, szLoseGuildName, szWinGuildName, szWinGuildName);
									else if(lpGuildList->sGuildNode[i].wPoint1<lpGuildList->sGuildNode[i].wPoint2)
										wsprintf(pAllUserPacket.szMessage, g_szGuildBattleText[1], szWinGuildName, szLoseGuildName, szWinGuildName, szWinGuildName);									
									else
										wsprintf(pAllUserPacket.szMessage, g_szGuildBattleText[2], lpGuildTable->sGuild.szGuildName, lpGuildTableDst->sGuild.szGuildName);									

									g_pNet->BroadcastUser((char*)&pAllUserPacket, pAllUserPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);							
									
									// 초기화 //
									WSTDS_GUILDWAR_DELETE pServerPacket;
									pServerPacket.dwGuildId		= lpGuildTable->sGuild.dwGuildId;
									pServerPacket.dwGuildIdDst	= lpGuildTableDst->sGuild.dwGuildId;																
							
									pos = g_pDungeonTable->m_pDungeonList->GetHeadPosition();
									
									while(pos)
									{
										DUNGEON_DATA_EX* pDungeonDataEx = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pDungeonList->GetNext(pos);

										if(pDungeonDataEx)
										{									
											if(pDungeonDataEx->m_pServer)
												g_pNet->SendToServer(pDungeonDataEx->m_pServer->dwConnectionIndex, (char*)&pServerPacket, pServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
										}								
									}

									memset(&lpGuildList->sGuildNode[i], 0, sizeof(lpGuildList->sGuildNode[i]));
									memset(&lpGuildListDst->sGuildNode[j], 0, sizeof(lpGuildListDst->sGuildNode[j]));

									DBBINDING* pBinding = g_pDb->THCreateParamInfo(1);
										
									GUILD_NODE	sGuildNode[MAX_GUILD_WAR];									
									memset(szQuery, 0, sizeof(szQuery));
									memset(sGuildNode, 0, sizeof(sGuildNode));										
									memcpy(sGuildNode, lpGuildList->sGuildNode, sizeof(sGuildNode));
																											
									pBinding[0].obValue		= 0;
									pBinding[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
									pBinding[0].wType		= DBTYPE_BYTES;											
									
									wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTable->sGuild.dwGuildId);
									g_pDb->THExecuteSQLByParam(szQuery, pBinding, sGuildNode, 1, (BYTE)GAME_DB);

									DBBINDING* pBindingDst = g_pDb->THCreateParamInfo(1);	
									
									memset(szQuery, 0, sizeof(szQuery));
									memset(sGuildNode, 0, sizeof(sGuildNode));										
									memcpy(sGuildNode, lpGuildListDst->sGuildNode, sizeof(sGuildNode));
																											
									pBindingDst[0].obValue		= 0;
									pBindingDst[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
									pBindingDst[0].wType		= DBTYPE_BYTES;
								
									wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildTableDst->sGuild.dwGuildId);
									g_pDb->THExecuteSQLByParam(szQuery, pBindingDst, sGuildNode, 1, (BYTE)GAME_DB);

									SGUILDWAR_TIME sGuildWarTime;
									
									GuildWarTime(&sGuildWarTime, lpGuildTable->sGuild.dwGuildId, lpGuildTableDst->sGuild.dwGuildId);

									pos = g_pGuildWarTimeList->GetHeadPosition();

									while(pos)
									{
										LP_GUILDWAR_TIME lpGuildWarTime = (LP_GUILDWAR_TIME)g_pGuildWarTimeList->GetNext(pos);

										if(lpGuildWarTime)
										{
											if(lpGuildWarTime->n64Id==sGuildWarTime.n64Id)
											{
												g_pGuildWarTimeList->RemoveAt(pos);
												LALFree(g_pGuildWarTimePool, lpGuildWarTime);
												break;
											}
										}
									}									
								}
							}
							return;
						}
					}
				}
				else
				{
					WSTC_GUILDWAR_ERRROR pErrorPacket;
					pErrorPacket.byError = 13;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pErrorPacket, pErrorPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
				}
				return;
			}
		}
	}	
}

void CmdGuildWarRank(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_GUILDWAR_RANK* pPacket = (CTWS_GUILDWAR_RANK*)pMsg;	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	int nIndex = 0;
	
	WSTC_GUILDWAR_RANK pSendPacket;

	for(int i = 0; i < 4; i++)
	{
		pSendPacket.dwGuildId[i]	= 0;
		pSendPacket.dwPoint[i]		= 0;
		memset(pSendPacket.szName[i], 0, sizeof(pSendPacket.szName[i]));
	}

	DWORD dwRank = 1;
		
	POSITION_ pos = g_pRankList->GetHeadPosition();

	while(pos)
	{
		LP_GUILDWAR_RANK lpGuildRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(pos);

		if(lpGuildRank)
		{
			if(pSendPacket.dwGuildId[nIndex]==0 && nIndex<3)
			{
				pSendPacket.dwGuildId[nIndex]	= lpGuildRank->dwGuildId;
				pSendPacket.dwPoint[nIndex]		= lpGuildRank->dwPoint;
				__lstrcpyn(pSendPacket.szName[nIndex], lpGuildRank->szName, MAX_GUILD_REAL_NAME_LENGTH);
				nIndex++;
			}

			if(lpGuildRank->dwGuildId==pPacket->dwGuildId)
			{
				pSendPacket.dwGuildId[3]	= lpGuildRank->dwGuildId;
				pSendPacket.dwPoint[3]		= lpGuildRank->dwPoint;
				pSendPacket.dwRank			= dwRank;
				__lstrcpyn(pSendPacket.szName[3], lpGuildRank->szName, MAX_GUILD_REAL_NAME_LENGTH);				
			}
			dwRank+=1;						
		}
	}
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
}

BOOL InitGuildInformations()
{
	int							i				= 0;
	int							nCount			= 0;
	int							nUserCount		= 0;
//	int							nRet			= 0;
//	int							nLoopCnt		= 0;
//	int							nGuildUserCount	= 0;
	
	char						szSQL[0xff]={0,};
	LP_GUILD_TABLE				lpGuildTable=0;
//	LP_GROUPINFO_TABLE			lpGroupInfo=0;
//	LP_GROUPINFO_TABLE			lpGroupInfoTable=0;
	LP_GUILD_OFFLINE_USER		lpGuildOfflineTable=0;
	GUILD_OFFLINE_USER			sGuildOfflineUser[100];
	LP_GUILD_LIST				lpGuildListTable=0;
	GUILD*						psGuild=0;
	SGUILD_LIST*				psGuildList=0;

	ListNode<SGUILD_LIST>* lpGuildWar;
	
	// 길드 관련 임시로 쓸 메모리 버퍼 생성!!
	psGuild = new GUILD[MAX_GUILD_COUNT];
	if(!psGuild)
	{
		Log(LOG_IMPORTANT, "psGuild is NULL!!");
		goto lb_Exit;
	}

	psGuildList = (GUILD_LIST*) new SGUILD_LISTJAPAN[MAX_GUILD_COUNT];
	if(!psGuildList)
	{
		Log(LOG_IMPORTANT, "psGuildList is NULL!!");
		goto lb_Exit;
	}
	
	Log(LOG_NORMAL, "GuildInfo Load - Start");	// Guild 정보.
	
	memset(psGuild, 0, sizeof(GUILD)*MAX_GUILD_COUNT);

	nCount = g_pDb->OpenRecord("CZP_GUILDINFO_LOAD", psGuild, MAX_GUILD_COUNT, (BYTE)GAME_DB);

	if(nCount<0)
	{
		Log(LOG_IMPORTANT, "Fail to query GuildInfo!");
		goto lb_Exit;
	}
	//Log(LOG_IMPORTANT, "GuildCount: %d", nCount );//0500315 재원
						
	for(i = 0; i < nCount; i++)
	{
	//	Log(LOG_IMPORTANT, "GuildLoad: %d", i );//0500315 재원
		// Hash Insert //		
		lpGuildTable = (LP_GUILD_TABLE)LALAlloc(g_pGuildTablePool);
		memset(lpGuildTable, 0, sizeof(lpGuildTable));
		
		// Guild User Hash //
		lpGuildTable->pGuildUserHash	= new CGuildUserTableHash;
		lpGuildTable->pGuildUserHash->InitializeHashTable(100, 100);
		
		// Off-Line Guild User Hash //
		lpGuildTable->pGuildOfflineHash	= new CGuildOfflineHash;
		lpGuildTable->pGuildOfflineHash->InitializeHashTable(100, 100);

		memcpy(&lpGuildTable->sGuild, &psGuild[i], sizeof(GUILD));

		lpGuildTable->dwPrevGuildExp	= psGuild[i].dwExp;
		
		LP_GROUPINFO_TABLE lpGroupInfoTable		= g_pGroupInfoTableHash->GetData(lpGuildTable->sGuild.wLevel+1);
		LP_GROUPINFO_TABLE lpCurGroupInfoTable	= g_pGroupInfoTableHash->GetData(lpGuildTable->sGuild.wLevel);

		if(lpGroupInfoTable && lpCurGroupInfoTable)
		{
			DWORD dwExp			= lpGroupInfoTable->sGroupInfo[lpGuildTable->sGuild.byType-1].dwExp - lpCurGroupInfoTable->sGroupInfo[lpGuildTable->sGuild.byType-1].dwExp;
			DWORD dwGuildExp	= lpGuildTable->sGuild.dwExp - lpCurGroupInfoTable->sGroupInfo[lpGuildTable->sGuild.byType-1].dwExp;
			
			lpGuildTable->bLevelIndex = ((0 == dwExp) ? 0 : (BYTE)(dwGuildExp/(dwExp/4)));//: 050111 jaewon
		}
		g_pGuildTableHash->Insert(lpGuildTable, lpGuildTable->sGuild.dwGuildId);

		// Offline User //
		wsprintf(szSQL, "CZP_GUILD_USER %u", lpGuildTable->sGuild.dwGuildId);
		memset(sGuildOfflineUser, 0, sizeof(sGuildOfflineUser));
		nUserCount = g_pDb->OpenRecord(szSQL, sGuildOfflineUser, 100, (BYTE)GAME_DB);		
		
		for(int j = 0; j < nUserCount; j++)
		{			
			lpGuildOfflineTable = (LP_GUILD_OFFLINE_USER)LALAlloc(g_pGuildOfflinePool);
			memset(lpGuildOfflineTable, 0, sizeof(lpGuildOfflineTable));
			memcpy(lpGuildOfflineTable, &sGuildOfflineUser[j], sizeof(GUILD_OFFLINE_USER));						

			if(lpGuildTable)
				lpGuildTable->pGuildOfflineHash->Insert(lpGuildOfflineTable, lpGuildOfflineTable->dwUserIndex);
		}
		
		LP_GUILDWAR_RANK lpGuildWarRankNew = (LP_GUILDWAR_RANK)LALAlloc(g_pRankListPool);

		memset(lpGuildWarRankNew, 0, sizeof(GUILDWAR_RANK));
				
		lpGuildWarRankNew->dwExp		= lpGuildTable->sGuild.dwExp;
		lpGuildWarRankNew->dwPoint		= lpGuildTable->sGuild.dwPoint;
		lpGuildWarRankNew->dwGuildId	= lpGuildTable->sGuild.dwGuildId;
		__lstrcpyn(lpGuildWarRankNew->szName, lpGuildTable->sGuild.szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
		
		POSITION_ pos = g_pRankList->GetHeadPosition();

		if(!pos)
			g_pRankList->AddTail(lpGuildWarRankNew);
		else
		{
			BOOL bChk = FALSE;

			while(pos)
			{
				POSITION_ posTemp = pos;

				LP_GUILDWAR_RANK lpGuildWarRank = (LP_GUILDWAR_RANK)g_pRankList->GetNext(pos);

				if(lpGuildWarRank->dwPoint<lpGuildWarRankNew->dwPoint)
				{
					g_pRankList->InsertBefore(posTemp, lpGuildWarRankNew);
					bChk = TRUE;
					break;
				}
				else if(lpGuildWarRank->dwPoint==lpGuildWarRankNew->dwPoint)
				{
					if(lpGuildWarRank->dwExp<lpGuildWarRankNew->dwExp)
					{
						g_pRankList->InsertBefore(posTemp, lpGuildWarRankNew);
						bChk = TRUE;
						break;
					}
				}
			}

			if(!bChk)
				g_pRankList->AddTail(lpGuildWarRankNew);			
		}

		// Exp //
		/*SGROUPINFO_TABLE* pGroupInfoTable[12];
				
		for(int i = 1; i < 13; i++)
			pGroupInfoTable[i-1] = g_pGroupInfoTableHash->GetData(i);

		char szQuery[0xff];

		for(i = 0; i < 12; i++)
		{
			if(lpGuildTable->sGuild.dwExp>=pGroupInfoTable[11-i]->sGroupInfo->dwExp)
			{
				wsprintf(szQuery, "Update GuildInfo Set Level = %u Where Id = %u", 12-i, lpGuildTable->sGuild.dwGuildId);
				g_pDb->ExecuteSQL(szQuery, GAME_DB);
				break;
			}
		}*/
	}	
	
#ifdef	GUILD_WAR

	memset(psGuildList, 0, sizeof(SGUILD_LIST)*MAX_GUILD_COUNT);

	// Guild List Load //
	nCount = g_pDb->OpenRecord("CZP_GUILD_LIST", psGuildList, MAX_GUILD_COUNT, (BYTE)GAME_DB);
	
	if(nCount<0)
	{
		Log(LOG_IMPORTANT, "Fail to query GuildInfo!");
		goto lb_Exit;
	}			

	for(i = 0; i < nCount; i++)	
	{
		lpGuildListTable = (LP_GUILD_LIST)LALAlloc(g_pGuildWarPool);
		memset(lpGuildListTable, 0, sizeof(SGUILD_LIST));
		memcpy(lpGuildListTable, &psGuildList[i], sizeof(SGUILD_LIST));						

		if(lpGuildListTable)
			g_pGuildWarHash->Insert(lpGuildListTable, lpGuildListTable->dwGuildId);		

		/*DBBINDING* pBinding = g_pDb->CreateParamInfo(1, GAME_DB);
	
		char		szQuery[0xff];
		GUILD_NODE	sGuildNode[MAX_GUILD_WAR];
		memset(szQuery, 0, sizeof(szQuery));
		memset(sGuildNode, 0, sizeof(sGuildNode));	
		
		if(lpGuildListTable->dwGuildId==194)
		{
			sGuildNode[0].dwGuildId		= 198;
			sGuildNode[0].wPoint1		= 0;
			sGuildNode[0].wPoint2		= 0;
			time(&sGuildNode[0].sTime);
			localtime(&sGuildNode[0].sTime);
		}									
		else if(lpGuildListTable->dwGuildId==198)
		{
			sGuildNode[0].dwGuildId		= 194;
			sGuildNode[0].wPoint1		= 0;
			sGuildNode[0].wPoint2		= 0;
			time(&sGuildNode[0].sTime);
			localtime(&sGuildNode[0].sTime);
		}
		pBinding[0].obValue		= 0;
		pBinding[0].cbMaxLen	= sizeof(GUILD_NODE)*MAX_GUILD_WAR;
		pBinding[0].wType		= DBTYPE_BYTES;											
		wsprintf(szQuery, "CZP_GUILDLIST_UPDATE %u, ?", lpGuildListTable->dwGuildId);
		g_pDb->ExecuteSQLByParam(szQuery, pBinding, sGuildNode, 1, GAME_DB);
		g_pDb->ReleaseParamInfo(pBinding, (BYTE)GAME_DB);*/
	}	

#endif

	// Guild Text //

	for(i = 0; i < MAX_GUILD_TEXT; i++)
		memset(g_szGuildBattleText[i], 0, sizeof(g_szGuildBattleText[i]));

	nCount = g_pDb->OpenRecord("Select Text From GuildText Order by Id", g_szGuildBattleText, MAX_GUILD_TEXT, (BYTE)GAME_DB);

	// Guild War Time //
	
	lpGuildWar  = g_pGuildWarHash->GetHead();

	while(lpGuildWar)
	{
		LP_GUILD_LIST lpGuildList = lpGuildWar->pData;

		if(lpGuildList)
		{
			for(int i = 0; i < MAX_GUILD_WAR; i++)
			{
				if(lpGuildList->sGuildNode[i].dwGuildId!=0)
				{
					SGUILDWAR_TIME sGuildWarTime;
					GuildWarTime(&sGuildWarTime, lpGuildList->dwGuildId, lpGuildList->sGuildNode[i].dwGuildId);
					sGuildWarTime.sTime = lpGuildList->sGuildNode[i].sTime;

					if(sGuildWarTime.n64Id!=0)
					{
						POSITION_ pos = g_pGuildWarTimeList->GetHeadPosition();

						BOOL bChk = TRUE;

						while(pos)
						{							
							LP_GUILDWAR_TIME lpGuildWarTime = (LP_GUILDWAR_TIME)g_pGuildWarTimeList->GetNext(pos);						

							if(lpGuildWarTime)
							{
								if(lpGuildWarTime->n64Id==sGuildWarTime.n64Id)
								{
									bChk = FALSE;									
									break;
								}
							}
						}		
						
						if(bChk)
						{
							LP_GUILDWAR_TIME lpGuildWarTimeNew = (LP_GUILDWAR_TIME)LALAlloc(g_pGuildWarTimePool);
							
							lpGuildWarTimeNew->n64Id	= sGuildWarTime.n64Id;
							lpGuildWarTimeNew->sTime	= sGuildWarTime.sTime;

							pos = g_pGuildWarTimeList->GetHeadPosition();

							if(!pos)
								g_pGuildWarTimeList->AddTail(lpGuildWarTimeNew);
							else
							{
								BOOL bChk = FALSE;

								while(pos)
								{
									POSITION_ posTemp = pos;

									LP_GUILDWAR_TIME lpGuildWarTime = (LP_GUILDWAR_TIME)g_pGuildWarTimeList->GetNext(pos);

									if(lpGuildWarTime->sTime<lpGuildWarTimeNew->sTime)
									{
										g_pGuildWarTimeList->InsertBefore(posTemp, lpGuildWarTimeNew);
										bChk = TRUE;
										break;
									}
								}

								if(!bChk)
									g_pGuildWarTimeList->AddTail(lpGuildWarTimeNew);			
							}
						}
					}					
				}
			}			
		}
		lpGuildWar = lpGuildWar->pNext;
	}

	Log(LOG_NORMAL, "GuildInfo Load - End");

	// 임시로 생성한 길드 관련 구조체 배열 제거!
	if(psGuild)		{ delete [] psGuild;psGuild=0;}
	if(psGuildList) { delete [] psGuildList;psGuildList=0;}

	return TRUE;

lb_Exit:
	if(psGuild)		{ delete [] psGuild;psGuild=0;}
	if(psGuildList) { delete [] psGuildList;psGuildList=0;}

	return FALSE;
}

void GuildWarTime(LP_GUILDWAR_TIME lpGuildWarTime, DWORD dwGuildId, DWORD dwGuildIdDst)
{
	if(dwGuildId<dwGuildIdDst)
	{
		lpGuildWarTime->war_time.dwLowId	= dwGuildId;
		lpGuildWarTime->war_time.dwHiId	= dwGuildIdDst;
	}
	else if(dwGuildId>dwGuildIdDst)
	{
		lpGuildWarTime->war_time.dwLowId	= dwGuildIdDst;
		lpGuildWarTime->war_time.dwHiId	= dwGuildId;
	}	
	else
	{
		lpGuildWarTime->war_time.dwLowId	= 0;
		lpGuildWarTime->war_time.dwHiId	= 0;
	}
}
//======================================================//
// End.													//
//======================================================//