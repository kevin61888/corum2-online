#include "stdafx.h"
#include "MsgProcForServerParty.h"
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
#include "PartyMatcher.h"


void CmdPartyPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_PORTAL* pPacket = (WSTDS_PARTY_PORTAL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{
		CWorldUser* pPortalUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwPartalUserIndex);		

		if(!pPortalUser)
			return;

		if(pPortalUser->m_dwPartyId==pUser->m_dwPartyId)
		{
			if(pUser->m_byLeader==1)
			{
				WSTC_PARTY_PORTAL pSendPacket;
				memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
				__lstrcpyn(pSendPacket.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
				pSendPacket.dwPartyId			= pPacket->dwPartyId;
				pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
				pSendPacket.dwPartalUserIndex	= pPacket->dwPartalUserIndex;
				pSendPacket.byType				= 1;	// 퍼스널 리콜 
				pSendPacket.byIndex				= pPacket->byIndex;
				g_pNet->SendToUser(pPortalUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
				return;
			}
		}			
	}
}

void CmdPartyAllPortal(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_ALLPORTAL* pPacket = (WSTDS_PARTY_ALLPORTAL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{		
		if(pUser->m_byLeader==1)
		{
			LP_PARTY_TABLE lpPartyTable = (LP_PARTY_TABLE)g_pPartyTableHash->GetData(pUser->m_dwPartyId);
		
			if(lpPartyTable)
			{
				POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

				while(pos)
				{
					CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

					if(pPartyUser)
					{
						if(pPartyUser->m_dwUserIndex!=pUser->m_dwUserIndex)
						{
							if(pUser->m_dwPartyId==pPartyUser->m_dwPartyId)
							{
								WSTC_PARTY_PORTAL pSendPacket;
								memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
								__lstrcpyn(pSendPacket.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
								pSendPacket.dwPartyId			= pUser->m_dwPartyId;
								pSendPacket.dwUserIndex			= pUser->m_dwUserIndex;
								pSendPacket.dwPartalUserIndex	= pPartyUser->m_dwUserIndex;
								pSendPacket.byIndex				= pPacket->byIndex;
								pSendPacket.byType				= 0;	// 파티 전원 리콜
								g_pNet->SendToUser(pPartyUser->m_dwConnectionIndex
									, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
							}			
						}
					}
				}			
			}	
		}			
	}
}


void CmdPartyPortalUsed(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_PORTALUSED* pPacket = (WSTDS_PARTY_PORTALUSED*)pMsg;
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
			if(pPortalUser->m_dwPartyId!=pUser->m_dwPartyId)
				return;

			WSTC_PARTY_PORTALUSED pSendPacket;
			memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
			__lstrcpyn(pSendPacket.szName, pPortalUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pSendPacket.dwPartyId			= pPacket->dwPartyId;
			pSendPacket.dwUserIndex			= pPacket->dwUserIndex;
			pSendPacket.dwPartalUserIndex	= pPacket->dwPartalUserIndex;
			pSendPacket.dwResult			= pPacket->dwResult;
			pSendPacket.byIndex				= pPacket->byIndex;
			pSendPacket.byType				= pPacket->byType;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
		}
	}
}


void CmdPartyPotalFail(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_PORTALFAIL* pPacket = (WSTDS_PARTY_PORTALFAIL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{
		WSTC_PARTY_PORTALFAIL pSendPacket;
		memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
		__lstrcpyn(pSendPacket.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pSendPacket.byResult	= pPacket->byResult;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CmdPartyHp(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_HP* pPacket = (WSTDS_PARTY_HP*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);	

	if(!pUser)
		return;
	
	if(pUser->m_dwPartyId==pPacket->dwPartyId)
	{
		LP_PARTY_TABLE lpPartyTable = (LP_PARTY_TABLE)g_pPartyTableHash->GetData(pUser->m_dwPartyId);

		if(lpPartyTable)
		{
			POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			while(pos)
			{
				CWorldUser* pPartyUser = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				if(pPartyUser)
				{
					if(pPartyUser->m_dwUserIndex!=pUser->m_dwUserIndex)
					{
						if(!pPartyUser)
							continue;
					
						// 메세지 보내기 //
						WSTC_PARTY_HP pHpPacket;
						pHpPacket.dwUserIndex	= pUser->m_dwUserIndex;
						pHpPacket.wHp			= pPacket->wHP;
						pHpPacket.wMaxHp		= pPacket->wMaxHp;
						pHpPacket.dwPartyId		= pPacket->dwPartyId;
						g_pNet->SendToUser(pPartyUser->m_dwConnectionIndex, (char*)&pHpPacket, pHpPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
					else
					{
						// 자기 자신에게 //
						pPartyUser->m_wHp		= pPacket->wHP;
						pPartyUser->m_wMaxHp	= pPacket->wMaxHp;					
							
						POSITION_ posDst = lpPartyTable->pPartyUserList->GetHeadPosition();

						while(posDst)
						{
							CWorldUser* pPartyUserDst = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(posDst);

							if(pPartyUserDst->m_dwUserIndex!=pUser->m_dwUserIndex)
							{
								// 메세지 보내기 //
								WSTC_PARTY_HP pHpPacket;
								pHpPacket.dwUserIndex	= pPartyUserDst->m_dwUserIndex;
								pHpPacket.dwPartyId		= pPacket->dwPartyId;
								pHpPacket.wHp			= pPartyUserDst->m_wHp;
								pHpPacket.wMaxHp		= pPartyUserDst->m_wMaxHp;
								g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pHpPacket, pHpPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							}
						}
					}
				}
			}
		}
	}
}

void CmdPartyLevel(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_LEVEL* pPacket = (WSTDS_PARTY_LEVEL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(!pUser)
		return;
	
	// 레벨이 올랐으면 파티 매칭에 새로 등록해야함 : 최덕석 2005.1.29
	MATCH_CONDITION_PARTY PMatchCondition;
	PMatchCondition.bExp = pUser->m_PMatchCondition.bExp;
	PMatchCondition.bItem = pUser->m_PMatchCondition.bItem;
	PMatchCondition.bLevel = (BYTE)pUser->m_dwLevel;

	// 매칭 질의 대기중이면
	if(pUser->m_dwPMatchIndex)
	{
		// 매칭 질의를 취소시키고
		g_PartyMatcher.MatchFail(pUser->m_dwPMatchIndex);
	}
	// 매칭 대기자에서 삭제를 성공했다면 (대기자에 있었다면)
	if(g_PartyMatcher.Remove(PMatchCondition, pUser->m_dwUserIndex) == true)
	{
		// 새로운 조건으로 다시 대기자에 들어감
		PMatchCondition.bLevel = (BYTE)pPacket->dwLevel;
		g_PartyMatcher.Insert(PMatchCondition, pUser->m_dwUserIndex, false);
	}

	// 유저의 레벨을 동기화
	pUser->m_dwLevel = pPacket->dwLevel;

	if( IS_ABLE_SERVICE_TYPE( ST_CHINA_BILLING ) )
	{
		// China Billing 상용화 관련 레벨 체크 //
		if(pUser->m_sChinaBillingInfo.eType!=enum_level_approval)
		{
			if(pUser->m_dwLevel>=10)
			{
				// World Server Billing Info Request //
				WSTAS_REQUEST_CHINA_BILLINGINFO pRequstChinaBillingInfo;
				pRequstChinaBillingInfo.dwPropID	= pUser->m_dwPropID;
				pRequstChinaBillingInfo.dwUserIndex	= pUser->m_dwUserIndex;
				g_pNet->SendToServer(AGENTINDEX, (char*)&pRequstChinaBillingInfo, pRequstChinaBillingInfo.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
			}
		}
	}

	if(pUser->m_dwPartyId==pPacket->dwPartyId && pUser->m_dwPartyId!=0)
	{
		LP_PARTY_TABLE lpPartyTable = (LP_PARTY_TABLE)g_pPartyTableHash->GetData(pUser->m_dwPartyId);

		if(lpPartyTable)
		{
			POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			while(pos)
			{
				CWorldUser* pPartyUser = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				if(!pPartyUser)
					continue;
				
				if(pPartyUser->m_dwUserIndex!=pUser->m_dwUserIndex)
				{
					// 메세지 보내기 //
					WSTC_PARTY_LEVEL pLevelPacket;
					pLevelPacket.dwUserIndex	= pUser->m_dwUserIndex;
					pLevelPacket.dwLevel		= pPacket->dwLevel;
					pLevelPacket.dwPartyId		= pPacket->dwPartyId;
					g_pNet->SendToUser(pPartyUser->m_dwConnectionIndex, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				else
				{
					// 자기 자신에게 //
					pPartyUser->m_dwLevel	= pPacket->dwLevel;
					
					POSITION_ posDst = lpPartyTable->pPartyUserList->GetHeadPosition();

					while(posDst)
					{
						CWorldUser* pPartyUserDst = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(posDst);

						if(!pPartyUserDst)
							continue;

						if(pPartyUserDst->m_dwUserIndex!=pUser->m_dwUserIndex)
						{
							// 메세지 보내기 //
							WSTC_PARTY_LEVEL pLevelPacket;
							pLevelPacket.dwUserIndex	= pPartyUserDst->m_dwUserIndex;
							pLevelPacket.dwLevel		= pPartyUserDst->m_dwLevel;
							pLevelPacket.dwPartyId		= pPacket->dwPartyId;
							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}
					}
				}
			}
		}
	}

	// 길드한테 보내기 //
	if(pUser->m_dwGuildId!=0)
	{
		LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);

		if(lpGuildTable)
		{			
			ListNode<CWorldUser>* pGuildUserPos = lpGuildTable->pGuildUserHash->GetHead();			

			while(pGuildUserPos)
			{
				CWorldUser* pGuildUser = pGuildUserPos->pData;

				if(pGuildUser)
				{
					WSTC_GUILDUSER_LEVEL pGuildLevelPacket;
					pGuildLevelPacket.dwUserIndex	= pUser->m_dwUserIndex;
					pGuildLevelPacket.dwLevel		= pUser->m_dwLevel;
					g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pGuildLevelPacket, pGuildLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				pGuildUserPos = pGuildUserPos->pNext;
			}
		}
	}
}


void CmdDungeonGroupExp(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_DUNGEON_GROUPEXP* pPacket = (DSTWS_DUNGEON_GROUPEXP*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}
	
	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwOwnUserIndex);

	if(!pUser)					return;
	if(pPacket->bType != 2)		return;	// guild
	if(pUser->m_dwGuildId == 0) return;
	if(pUser->m_dwPartyId == 0) return;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);

	if(!lpPartyTable)
		return;

	POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

	while(pos)
	{
		CWorldUser* pPartyUser = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);				
	
		if(!pPartyUser) continue;

		if(pUser->m_dwUserIndex != pPartyUser->m_dwUserIndex)
		{
			if(pUser->m_dwDungeonID == pPartyUser->m_dwDungeonID)
			{
				if(pPartyUser->m_dwGuildId == pUser->m_dwGuildId)
				{
					LP_GUILD_TABLE lpGuildTable = g_pGuildTableHash->GetData(pUser->m_dwGuildId);

					if(lpGuildTable)								
					{
						lpGuildTable->sGuild.dwExp += pPacket->dwExp;

						LP_GROUPINFO_TABLE lpGroupInfoTable		=
							g_pGroupInfoTableHash->GetData(lpGuildTable->sGuild.wLevel+1);
						LP_GROUPINFO_TABLE lpCurGroupInfoTable	= 
							g_pGroupInfoTableHash->GetData(lpGuildTable->sGuild.wLevel);

						if(lpGroupInfoTable && lpCurGroupInfoTable)
						{																
							if(lpGroupInfoTable->sGroupInfo[lpGuildTable->sGuild.byType-1].dwExp<=lpGuildTable->sGuild.dwExp)
							{
								lpGuildTable->sGuild.wLevel +=	1;
								lpGuildTable->bLevelIndex	=	0;
																				
								ListNode<CWorldUser>* lpGuildPos = lpGuildTable->pGuildUserHash->GetHead();
									
								while(lpGuildPos)
								{
									CWorldUser* pGuildUser = lpGuildPos->pData;
									
									if(pGuildUser)
									{
										WSTC_GUILD_EXP pExpPacket;
										pExpPacket.dwGuildId	= lpGuildTable->sGuild.dwGuildId;
										pExpPacket.dwExp		= lpGuildTable->sGuild.dwExp;
										pExpPacket.wLevel		= lpGuildTable->sGuild.wLevel;
										pExpPacket.byType		= 1;
										pExpPacket.byIndex		= 0;
										g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pExpPacket, pExpPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									}
									
									lpGuildPos = lpGuildPos->pNext;
								}

								// 레벨업 //
								char szQuery[0xff]={0,};
								wsprintf(szQuery, "Update GuildInfo Set Level= %u, Exp = %u Where Id = %u", lpGuildTable->sGuild.wLevel, lpGuildTable->sGuild.dwExp, pUser->m_dwGuildId);
								g_pDb->THExecuteSQL(szQuery, TRUE, QUERY_TYPE_GUILD_LEVEL, pUser, (BYTE)GAME_DB);
							}
							else
							{					
								DWORD dwExp			= lpGroupInfoTable->sGroupInfo[lpGuildTable->sGuild.byType-1].dwExp - lpCurGroupInfoTable->sGroupInfo[lpGuildTable->sGuild.byType-1].dwExp;
								DWORD dwGuildExp	= lpGuildTable->sGuild.dwExp - lpCurGroupInfoTable->sGroupInfo[lpGuildTable->sGuild.byType-1].dwExp;

								if((dwExp/4)*(lpGuildTable->bLevelIndex+1)<=dwGuildExp)
								{
									// 경험치 보내기 //
									lpGuildTable->bLevelIndex+=1;

									ListNode<CWorldUser>* lpGuildPos = lpGuildTable->pGuildUserHash->GetHead();
									
									while(lpGuildPos)
									{
										CWorldUser* pGuildUser = lpGuildPos->pData;
										
										if(pGuildUser)
										{
											WSTC_GUILD_EXP pExpPacket;
											pExpPacket.dwGuildId	= lpGuildTable->sGuild.dwGuildId;
											pExpPacket.dwExp		= lpGuildTable->sGuild.dwExp;
											pExpPacket.wLevel		= lpGuildTable->sGuild.wLevel;
											pExpPacket.byType		= 0;
											pExpPacket.byIndex		= lpGuildTable->bLevelIndex;
											g_pNet->SendToUser(pGuildUser->m_dwConnectionIndex, (char*)&pExpPacket, pExpPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
										}

										lpGuildPos = lpGuildPos->pNext;
									}													
								}
							}
						}
					}
				}
			}

			return;
		}
	}
}


void CmdPartyInfo(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	DSTWS_PARTY_INFO* pPacket = (DSTWS_PARTY_INFO*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	// 파티원 정보 보내기 //
	CWorldUser* pUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{		
		WSTC_PARTY_INFO pSendPacket;
		memset(pSendPacket.szCharName, 0, sizeof(pSendPacket.szCharName));
		__lstrcpyn(pSendPacket.szCharName, pPacket->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pSendPacket.dwUserIndex		= pPacket->dwUserIndexRv;
		pSendPacket.wDungeonID		= pPacket->wDungeonID;
		pSendPacket.byLayerIndex	= pPacket->byLayerIndex;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CmdPartyPortalMove(SERVER_DATA* pServer, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_PORTALMOVE* pPacket = (WSTDS_PARTY_PORTALMOVE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pUser)
	{
		CWorldUser* pPortalUser = (CWorldUser*)g_pUserTable->GetUserInfo(pPacket->dwPortalUserIndex);

		if(pPortalUser)
		{
			WSTC_PARTY_PORTALMOVE pSendPacket;
			pSendPacket.wDungeonID		= pPacket->wDungeonID;
			pSendPacket.byLayerIndex	= pPacket->byLayerIndex;
			pSendPacket.dwPartyId		= pPacket->dwPartyId;
			pSendPacket.fPosX			= pPacket->fPosX;
			pSendPacket.fPosZ			= pPacket->fPosZ;						
			g_pNet->SendToUser(pPortalUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
		}
	}	
}

