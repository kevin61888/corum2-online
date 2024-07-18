#include "stdafx.h"
#include "Party.h"
#include "WorldUserTable.h"
#include "DungeonTable.h"
#include "Guild.h"
#include "PartyDef.h"
#include "MsgProc.h"


DWORD	g_dwPartyId	= 1;


void CmdPartyUserWorldInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_PARTYUSER_WORLDINFO* pPacket = (CTWS_PARTYUSER_WORLDINFO*)pMsg;

	pUser->m_byCurrentHand							= pPacket->byCurrentHand;
	pUser->m_pEquip[ EQUIP_TYPE_HELMET ].m_wItemID	= pPacket->wHelmet;
	pUser->m_pEquip[ EQUIP_TYPE_ARMOR  ].m_wItemID	= pPacket->wArmor; 

	if(pUser->m_byCurrentHand==1)
	{
		pUser->m_pEquip[ EQUIP_TYPE_LHAND1 ].m_wItemID	= pPacket->wHandL;
		pUser->m_pEquip[ EQUIP_TYPE_RHAND1 ].m_wItemID	= pPacket->wHandR;
	}
	else
	{
		pUser->m_pEquip[ EQUIP_TYPE_LHAND2 ].m_wItemID	= pPacket->wHandL;
		pUser->m_pEquip[ EQUIP_TYPE_RHAND2 ].m_wItemID	= pPacket->wHandR;
	}
}

void CmdPartyUserRequestFail(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_PARTY_USERREQUEST_FAIL*	pPacket = (CTWS_PARTY_USERREQUEST_FAIL*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_PARTY_USERREQUEST_FAIL		pFailPacket;

	CWorldUser* pPartyUser =  g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pPartyUser)
	{
		pFailPacket.byType	= 3;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}	
}

void CmdPartyUserRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_PARTY_USERREQUEST* pPacket = (CTWS_PARTY_USERREQUEST*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pPacket->dwPartyId);

	if(lpPartyTable)
	{
		if(lpPartyTable->pPartyUserList)
		{
			POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			while(pos)
			{
				CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				if(pPartyUser)
				{
					if(pPartyUser->m_byLeader==1)
					{
						CWorldUser* pSendUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

						if(pSendUser)
						{
							WSTC_PARTY_USERREQUEST	pRequestPacket;
							memset(pRequestPacket.szName, 0, sizeof(pRequestPacket.szName));
							memset(pRequestPacket.szRequestName, 0, sizeof(pRequestPacket.szRequestName));
							pRequestPacket.dwUserIndex		= pPacket->dwUserIndex;
							pRequestPacket.dwReqUserIndex	= pPacket->dwReqUserIndex;
							__lstrcpyn(pRequestPacket.szName, pSendUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							__lstrcpyn(pRequestPacket.szRequestName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							g_pNet->SendToUser(pPartyUser->m_dwConnectionIndex, (char*)&pRequestPacket, pRequestPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}					
						else
						{
							WSTC_PARTY_USERREQUEST_FAIL pFailPacket;
							pFailPacket.byType	= 1;
							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);							
						}
						return;
					}
				}
			}
		}		
	}	
	
	WSTC_PARTY_USERREQUEST_FAIL pFailPacket;
	pFailPacket.byType	= 2;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void CmdPartyInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_PARTY_INFO* pPacket = (CTWS_PARTY_INFO*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);
					
	if(lpPartyTable)
	{	
		 if(lpPartyTable->pPartyUserList)
		 {
			 POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

			 while(pos)
			 {
				 CWorldUser* pPartyUser =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

				 if(pPartyUser)
				 {
					 if(pPartyUser->m_dwUserIndex==pPacket->dwUserIndex)
					 {						
						 DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pPartyUser->m_dwDungeonID);

						if(!pDungeon)
						{
							WSTC_PARTY_INFO pSendPacket;
							memset(pSendPacket.szCharName, 0, sizeof(pSendPacket.szCharName));
							__lstrcpyn(pSendPacket.szCharName, pPartyUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
							pSendPacket.dwUserIndex		= pPartyUser->m_dwUserIndex;
							pSendPacket.wDungeonID		= 0;
							pSendPacket.byLayerIndex	= 0;
							g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							return;
						}
						else
						{
							WSTDS_PARTY_INFO pPartyInfo;
							memset(pPartyInfo.szCharName, 0, sizeof(pPartyInfo.szCharName));
							__lstrcpyn(pPartyInfo.szCharName, pPacket->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
							pPartyInfo.dwUserIndex		= pUser->m_dwUserIndex;
							pPartyInfo.dwUserIndexRv	= pPartyUser->m_dwUserIndex;												
							g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPartyInfo, pPartyInfo.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							return;
						}
					 }
				}
			 }
		}
	}
}

void CmdPartySet(CWorldUser* pUser, char* pMsg,DWORD dwLength)
{
	CTWS_PARTY_SET* pPacket = (CTWS_PARTY_SET*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	WSTC_PARTY_SET	pSendPacket;

	pUser->m_bParty		= pPacket->bParty;	
	pSendPacket.bParty	= pUser->m_bParty;
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void CmdPartyLogOutUser(CWorldUser* pUser ,char* pMsg,DWORD dwLength)
{
	pUser;

	CTWS_PARTY_LOGOUTRT* pPacket = (CTWS_PARTY_LOGOUTRT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	CWorldUser* pUserDst = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(!pUserDst)
		return;

	if(pPacket->dwPartyId==0)
		return;

	if(pPacket->dwPartyId!=pUserDst->m_dwPartyId)
		return;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUserDst->m_dwPartyId);
		
	if(!lpPartyTable || !lpPartyTable->pPartyUserList)
		return;

	if(lpPartyTable->bCount>2)
	{
		POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();
		POSITION_ posTemp;

		while(pos)
		{			
			posTemp = pos;

			CWorldUser* pPartyUser = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

			if(pPartyUser)
			{
				if(pPartyUser->m_dwUserIndex==pUserDst->m_dwUserIndex)
				{
					POSITION_ posDst = lpPartyTable->pPartyUserList->GetHeadPosition();

					while(posDst)
					{
						CWorldUser* pUserRv = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(posDst);

						if(!pUserRv)
							continue;

						if(pUserRv->m_dwUserIndex!=pUserDst->m_dwUserIndex)
						{												
							// 메세지 보내기 //
							WSTC_PARTY_LOGOUT pSendLogout;
							memset(pSendLogout.szName, 0, sizeof(pSendLogout.szName));
							__lstrcpyn(pSendLogout.szName, pUserDst->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);						
							pSendLogout.bResultCode	= 0;
							pSendLogout.dwUserIndex	= pUserDst->m_dwUserIndex;
							g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pSendLogout, pSendLogout.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}
						else
						{
							DUNGEON_DATA_EX* pDungeon;
							pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUserDst->m_dwDungeonID);	
							
							if(!pDungeon)
							{
								Log(LOG_NORMAL, "Dungeon = NULL(CmdPartyLogOutUser)");
								continue;
							}
	
							WSTDS_PARTY_MSG	pPartyMsg;
							pPartyMsg.dwPartyId		= 0;
							pPartyMsg.dwUserIndex	= pUserDst->m_dwUserIndex;
							g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex
								, (char*)&pPartyMsg, pPartyMsg.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

							// 메세지 보내기 //
							WSTC_PARTY_LOGOUT pSendLogout;
							memset(pSendLogout.szName, 0, sizeof(pSendLogout.szName));
							__lstrcpyn(pSendLogout.szName, pUserDst->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);						
							pSendLogout.bResultCode	= 1;
							pSendLogout.dwUserIndex	= pUserDst->m_dwUserIndex;
							g_pNet->SendToUser(pUserDst->m_dwConnectionIndex, (char*)&pSendLogout, pSendLogout.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						}
					}
					
					if(pPartyUser->m_byLeader == 1)
					{
						pPartyUser->m_byLeader	= 0;						
						lpPartyTable->pPartyUserList->RemoveAt(posTemp);
						pUserDst->m_dwPartyId	= 0;
						lpPartyTable->bCount	-= 1;

						// 새로운 리더를 보내준다 //
						POSITION_ posLeader = lpPartyTable->pPartyUserList->GetHeadPosition();

						BYTE	byLeader		= 0;
						DWORD	dwUserIndeex	= 0;
						char	szCharName[MAX_CHARACTER_NAME_LENGTH]={0,};

						memset(szCharName, 0, sizeof(szCharName));

						while(posLeader)
						{
							CWorldUser* pPartyUserLeader = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(posLeader);
							
							if(!pPartyUserLeader)
								continue;

							if(byLeader==0)
							{
								pPartyUserLeader->m_byLeader = 1;
								byLeader		= 1;
								dwUserIndeex	= pPartyUserLeader->m_dwUserIndex;
								__lstrcpyn(szCharName, pPartyUserLeader->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);

								LP_PARTYBOARD_TABLE lpPartyTable = 
									g_pPartyBoardTableHash->GetData(pPartyUserLeader->m_dwPartyId);

								if(lpPartyTable)
									lpPartyTable->dwPartyUserLeader	= pPartyUserLeader->m_dwUserIndex;

								// Add by Beckham 던젼서버에 내가 파티짱이라는걸 알려준다.
								DUNGEON_DATA_EX* pDungeon = NULL;
								pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pPartyUser->m_dwDungeonID);	
							
								if(pDungeon)
								{
									WSTDS_PARTY_LEADER pLeaderDungeonPacket;
									pLeaderDungeonPacket.dwUserIndex	= pPartyUser->m_dwUserIndex;
									pLeaderDungeonPacket.byLeader		= 0;
									g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex
										, (char*)&pLeaderDungeonPacket
										, pLeaderDungeonPacket.GetPacketSize()
										, FLAG_SEND_NOT_ENCRYPTION);	
								}
								
								pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pPartyUserLeader->m_dwDungeonID);	
							
								if(pDungeon)
								{
									WSTDS_PARTY_LEADER pLeaderDungeonPacket;
									pLeaderDungeonPacket.dwUserIndex	= pPartyUserLeader->m_dwUserIndex;
									pLeaderDungeonPacket.byLeader		= 1;
									g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex
										, (char*)&pLeaderDungeonPacket
										, pLeaderDungeonPacket.GetPacketSize()
										, FLAG_SEND_NOT_ENCRYPTION);	
								}
								// Add by Beckham 
							}						
							
							// Packet 보내기 //														
							WSTC_PARTY_LEADER pLeaderPackect;
							memset(pLeaderPackect.szCharName, 0, sizeof(pLeaderPackect.szCharName));
							__lstrcpyn(pLeaderPackect.szCharName, szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
							pLeaderPackect.dwUserIndex	= dwUserIndeex;
							g_pNet->SendToUser(pPartyUserLeader->m_dwConnectionIndex, (char*)&pLeaderPackect
								, pLeaderPackect.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
							
							// 매칭 대기자에서 뺀다
							CmdPMatchEscape(pPartyUserLeader, NULL, 0);
						}														
					}
					else
					{
						pPartyUser->m_byLeader	= 0;						
						lpPartyTable->pPartyUserList->RemoveAt(posTemp);
						pUserDst->m_dwPartyId	= 0;
						lpPartyTable->bCount	-= 1;						
					}
					
					break;
				}
			}			
		}
	}
	else
	{
		// 파티 삭제 //				
		POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();
		POSITION_ posTemp;

		while(pos)
		{			
			posTemp = pos;

			CWorldUser*	pUserRv	= (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);			

			if(!pUserRv)
				continue;				

			pUserRv->m_dwPartyId	= 0;
			pUserRv->m_byLeader		= 0;
									

			DUNGEON_DATA_EX*	pDungeon;
			WSTDS_PARTY_MSG		pPartyMsg;
			WSTC_PARTY_LOGOUT	pSendLogout;

			pDungeon	= g_pDungeonTable->GetDungeonInfo((WORD)pUserRv->m_dwDungeonID);				

			if(pDungeon)
			{			
				pPartyMsg.dwPartyId		= 0;
				pPartyMsg.dwUserIndex	= pUserRv->m_dwUserIndex;
				g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPartyMsg, pPartyMsg.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
			
											
			// 메세지 보내기 //			
			memset(pSendLogout.szName, 0, sizeof(pSendLogout.szName));
			__lstrcpyn(pSendLogout.szName, pUserDst->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pSendLogout.bResultCode	= 1;
			g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pSendLogout, pSendLogout.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			// 매칭 대기자에서 뺀다
			CmdPMatchEscape(pUserRv, NULL, 0);
		}
		// Hash 에서 삭제 //
		g_pPartyTableHash->Delete(lpPartyTable, pPacket->dwPartyId, TRUE);

		LP_PARTYBOARD_TABLE lpPartyTable = g_pPartyBoardTableHash->GetData(pPacket->dwPartyId);
		
		if(lpPartyTable)
		{
			WSTDS_PARTY_BOARD pPartyBoardPacket;
			pPartyBoardPacket.dwPartyId	= pPacket->dwPartyId;
			
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

void CmdPartyLogoutCl(CWorldUser* pUser ,char* pMsg,DWORD dwLength)
{
	CTWS_PARTY_LOGOUT* pPacket = (CTWS_PARTY_LOGOUT*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	if(pPacket->dwPartyId==0)	
		return;

	if(pPacket->dwPartyId!=pUser->m_dwPartyId)	
		return;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);
		
	if(!lpPartyTable || !lpPartyTable->pPartyUserList)
		return;

	if(lpPartyTable->bCount>2)
	{
		POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();
		POSITION_ posTemp;

		while(pos)
		{			
			posTemp = pos;

			CWorldUser* pPartyUser = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

			if(!pPartyUser)
				continue;

			if(pPartyUser->m_dwUserIndex==pUser->m_dwUserIndex)
			{
				POSITION_ posDst = lpPartyTable->pPartyUserList->GetHeadPosition();

				while(posDst)
				{
					CWorldUser* pUserRv = (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(posDst);

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
					else
					{
						DUNGEON_DATA_EX* pDungeon;
						pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);	

						if(!pDungeon)
						{
							Log(LOG_NORMAL, "Dungeon = NULL(CmdPartyLogoutCl)");
							continue;
						}

						WSTDS_PARTY_MSG	pPartyMsg;
						pPartyMsg.dwPartyId		= 0;
						pPartyMsg.dwUserIndex	= pUser->m_dwUserIndex;
						g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPartyMsg, pPartyMsg.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						// 메세지 보내기 //
						WSTC_PARTY_LOGOUT pSendLogout;
						memset(pSendLogout.szName, 0, sizeof(pSendLogout.szName));
						__lstrcpyn(pSendLogout.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);						
						pSendLogout.bResultCode	= 1;
						pSendLogout.dwUserIndex	= pUser->m_dwUserIndex;
						g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendLogout, pSendLogout.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					}
				}
				
				//== ==//
				if(pPartyUser->m_byLeader==1)
				{
					pPartyUser->m_byLeader	= 0;										
					lpPartyTable->pPartyUserList->RemoveAt(posTemp);
					pUser->m_dwPartyId		= 0;
					lpPartyTable->bCount	-= 1;					

					// 새로운 리더를 보내준다 //
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
							byLeader				= 1;
							dwUserIndeex			= pUserLeader->m_dwUserIndex;
							__lstrcpyn(szCharName, pUserLeader->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);

							LP_PARTYBOARD_TABLE lpPartyTable = g_pPartyBoardTableHash->GetData(pUserLeader->m_dwPartyId);

							if(lpPartyTable)
								lpPartyTable->dwPartyUserLeader	= pUserLeader->m_dwUserIndex;

							// Add by Beckham 던젼서버에 내가 파티짱이라는걸 알려준다.
							DUNGEON_DATA_EX* pDungeon = NULL;
							pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pPartyUser->m_dwDungeonID);	
						
							if(pDungeon)
							{
								WSTDS_PARTY_LEADER pLeaderDungeonPacket;
								pLeaderDungeonPacket.dwUserIndex	= pPartyUser->m_dwUserIndex;
								pLeaderDungeonPacket.byLeader		= 0;
								g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex
									, (char*)&pLeaderDungeonPacket
									, pLeaderDungeonPacket.GetPacketSize()
									, FLAG_SEND_NOT_ENCRYPTION);	
							}
							
							pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUserLeader->m_dwDungeonID);	
						
							if(pDungeon)
							{
								WSTDS_PARTY_LEADER pLeaderDungeonPacket;
								pLeaderDungeonPacket.dwUserIndex	= pUserLeader->m_dwUserIndex;
								pLeaderDungeonPacket.byLeader		= 1;
								g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex
									, (char*)&pLeaderDungeonPacket
									, pLeaderDungeonPacket.GetPacketSize()
									, FLAG_SEND_NOT_ENCRYPTION);	
							}
							// Add by Beckham 
						}
						
						// Packet 보내기 //
						WSTC_PARTY_LEADER pLeaderPackect;
						memset(pLeaderPackect.szCharName, 0, sizeof(pLeaderPackect.szCharName));
						__lstrcpyn(pLeaderPackect.szCharName, szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
						pLeaderPackect.dwUserIndex	= dwUserIndeex;							
						g_pNet->SendToUser(pUserLeader->m_dwConnectionIndex, (char*)&pLeaderPackect
							, pLeaderPackect.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

						// 매칭 대기자에서 뺀다
						CmdPMatchEscape(pUserLeader, NULL, 0);
					}														
				}
				else
				{
					pPartyUser->m_byLeader	= 0;									
					lpPartyTable->pPartyUserList->RemoveAt(posTemp);
					pUser->m_dwPartyId		= 0;
					lpPartyTable->bCount	-= 1;					
				}				
				break;
			}
		}
	}
	else
	{
		// 파티 삭제 //				
		POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();
		POSITION_ posTemp;

		while(pos)
		{			
			posTemp = pos;

			CWorldUser* pUserRv	= (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);			

			if(!pUserRv)
				continue;
						
			pUserRv->m_dwPartyId = 0;			

			DUNGEON_DATA_EX*	pDungeon;
			WSTDS_PARTY_MSG		pPartyMsg;
			WSTC_PARTY_LOGOUT	pSendLogout;

			pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pUserRv->m_dwDungeonID);

			if(pDungeon)
			{
				pPartyMsg.dwPartyId		= 0;
				pPartyMsg.dwUserIndex	= pUserRv->m_dwUserIndex;				
				g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pPartyMsg, pPartyMsg.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}			
											
			// 메세지 보내기 //			
			memset(pSendLogout.szName, 0, sizeof(pSendLogout.szName));
			__lstrcpyn(pSendLogout.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pSendLogout.bResultCode	= 1;
			g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pSendLogout, pSendLogout.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			// 매칭 대기자에서 뺀다
			CmdPMatchEscape(pUserRv, NULL, 0);

		}
		// Hash 에서 삭제 //
		g_pPartyTableHash->Delete(lpPartyTable, pPacket->dwPartyId, TRUE);

		LP_PARTYBOARD_TABLE lpPartyTable = g_pPartyBoardTableHash->GetData(pPacket->dwPartyId);
		
		if(lpPartyTable)
		{
			WSTDS_PARTY_BOARD pPartyBoardPacket;
			pPartyBoardPacket.dwPartyId	= pPacket->dwPartyId;

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

void CmdPartyApprove(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PARTY_APPROVE* pPacket = (CTWS_PARTY_APPROVE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	LP_PARTY_TABLE		lpParty = 0;
	WSTC_PARTY_JOINID	pPacketId;
	WSTC_PARTY_JOIN		pFailPacket;
	CWorldUser*			pUserRv	= g_pUserTable->GetUserInfo(pPacket->dwUserIndex);		
	DUNGEON_DATA_EX*	pDungeon;
	DUNGEON_DATA_EX*	pDungeonRecv;
	
	// 로그 아웃 //
	if(!pUserRv)	
		return;	

	// 이미 파티에 가입 //
	if(pUser->m_dwPartyId!=0)
		return;

	if(pUser->m_dwGuildId && pUserRv->m_dwGuildId)
	{
		if(IsGuildWar(pUser->m_dwGuildId, pUserRv->m_dwGuildId))
		{
			WSTC_PARTY_USERREQUEST_FAIL pGuildPacket;
			pGuildPacket.byType = 5;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pGuildPacket, pGuildPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pGuildPacket, pGuildPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);		
			return;
		}
	}
	
	if(pUserRv->m_dwPartyId!=0)
	{
		pDungeon	= g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);	

		if(!pDungeon)
		{
			Log(LOG_NORMAL, "Dungeon = NULL(CmdPartyApprove)");
			return;
		}

		// 기존 리스트에 추가 //
		lpParty = g_pPartyTableHash->GetData(pUserRv->m_dwPartyId);

		if(!lpParty)
		{
			memset(pFailPacket.szName, 0, sizeof(pFailPacket.szName));			
			pFailPacket.bResult = 7;
			__lstrcpyn(pFailPacket.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);			
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
			return;
		}
		else if(lpParty->bCount>=MAX_PARTY_USER)
		{
			memset(pFailPacket.szName, 0, sizeof(pFailPacket.szName));			
			__lstrcpyn(pFailPacket.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pFailPacket.bResult		= 2;			
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailPacket, pFailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		lpParty->pPartyUserList->AddTail(pUser);
		lpParty->bCount	+= 1;
						
		// Loop //
		POSITION_ pos = lpParty->pPartyUserList->GetHeadPosition();
							
		while(pos)
		{
			CWorldUser* pUserNode = (CWorldUser*)lpParty->pPartyUserList->GetNext(pos);

			if(pUser->m_dwUserIndex==pUserNode->m_dwUserIndex)
			{
				// 가입한 유저에게 메세지 보내기 //
				POSITION_ posTemp = lpParty->pPartyUserList->GetHeadPosition();

				while(posTemp)
				{
					CWorldUser* pUserTemp = (CWorldUser*)lpParty->pPartyUserList->GetNext(posTemp);

					// 자기 자신 //
					if(pUserTemp->m_dwUserIndex==pUser->m_dwUserIndex)
						pPacketId.bResultCode	= 0;
					else
						pPacketId.bResultCode	= 4;

					pPacketId.dwPartyId		= pUserRv->m_dwPartyId;
					pPacketId.dwUserIndex	= pUserTemp->m_dwUserIndex;
					pPacketId.byLeader		= pUserTemp->m_byLeader;
					pPacketId.wClass		= pUserTemp->m_wClass; 
					
					memset(pPacketId.szName, 0, sizeof(pPacketId.szName));
					__lstrcpyn(pPacketId.szName, pUserTemp->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacketId, pPacketId.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}				
			}
			else
			{
				// 최근 파티 친구로 등록
				AddPartyFriend(pUser->m_dwUserIndex, pUserNode->m_dwUserIndex);
				AddPartyFriend(pUserNode->m_dwUserIndex, pUser->m_dwUserIndex);

				if(pUserRv->m_dwUserIndex==pUserNode->m_dwUserIndex)
				{
					// 가입을 시킨 사람 //
					pPacketId.bResultCode	= 1;
					pPacketId.byLeader		= 0;
					pPacketId.wClass		= pUser->m_wClass; 					
					pPacketId.dwUserIndex	= pUser->m_dwUserIndex;
					pPacketId.dwPartyId		= pUserRv->m_dwPartyId;
					
					memset(pPacketId.szName, 0, sizeof(pPacketId.szName));
					__lstrcpyn(pPacketId.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pPacketId, pPacketId.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				else
				{
					CWorldUser* pUserDst = g_pUserTable->GetUserInfo(pUserNode->m_dwUserIndex);

					if(!pUserDst)
						continue;					

					// 파티원 //					
					pPacketId.bResultCode	= 2;
					pPacketId.byLeader		= 0;
					pPacketId.wClass		= pUser->m_wClass; 
					pPacketId.dwUserIndex	= pUser->m_dwUserIndex;
					pPacketId.dwPartyId		= pUserRv->m_dwPartyId;

					memset(pPacketId.szName, 0, sizeof(pPacketId.szName));
					__lstrcpyn(pPacketId.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					g_pNet->SendToUser(pUserDst->m_dwConnectionIndex, (char*)&pPacketId, pPacketId.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}
		
		pUser->m_dwPartyId	= pUserRv->m_dwPartyId;		
		
		// 메세지 날리기 //	
		WSTDS_PARTY_MSG		pSendPacket;				
		pSendPacket.dwPartyId	= pUser->m_dwPartyId;		
		pSendPacket.dwUserIndex	= pUser->m_dwUserIndex;
		g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{	
		// 메세지 날리기 //	
		WSTDS_PARTY_MSG		pSendPacket;
		
		pDungeon		= g_pDungeonTable->GetDungeonInfo((WORD)pUser->m_dwDungeonID);
		pDungeonRecv	= g_pDungeonTable->GetDungeonInfo((WORD)pUserRv->m_dwDungeonID);

		if(!pDungeon || !pDungeonRecv)
		{
			Log(LOG_NORMAL, "Dungeon = NULL(CmdPartyApprove2)");
			return;
		}

		// 새로 생성 //		
		lpParty					= (LP_PARTY_TABLE)LALAlloc(g_pPartyTablePool);
		lpParty->pPartyUserList = new COnlyList(MAX_PARTY);
		lpParty->dwPartyId		= g_dwPartyId;
		lpParty->bCount			= 2;
		g_pPartyTableHash->Insert(lpParty, lpParty->dwPartyId);
		
		// 두명 //
		pUser->m_byLeader	= 0;
		pUserRv->m_byLeader	= 1;	
		lpParty->pPartyUserList->AddTail(pUser);
		lpParty->pPartyUserList->AddTail(pUserRv);
						
		LP_PARTYBOARD_TABLE lpPartyTable = g_pPartyBoardTableHash->GetData(pUserRv->m_dwPartyId);

		if(lpPartyTable)
			lpPartyTable->dwPartyUserLeader	= pUserRv->m_dwUserIndex;

		// 유저 세팅 //		
		memset(pPacketId.szName, 0, sizeof(pPacketId.szName));
		__lstrcpyn(pPacketId.szName, pUserRv->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);

		pPacketId.dwPartyId		= g_dwPartyId;
		pPacketId.dwUserIndex	= pUserRv->m_dwUserIndex;
		pPacketId.wClass		= pUserRv->m_wClass;
		pPacketId.bResultCode	= 0;		
		pPacketId.byLeader		= 1;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPacketId, pPacketId.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);

		memset(pPacketId.szName, 0, sizeof(pPacketId.szName));
		__lstrcpyn(pPacketId.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);

		pPacketId.dwPartyId		= g_dwPartyId;
		pPacketId.wClass		= pUser->m_wClass;
		pPacketId.dwUserIndex	= pUser->m_dwUserIndex;
		pPacketId.bResultCode	= 1;
		pPacketId.byLeader		= 0;				
		g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pPacketId, pPacketId.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);

		pUser->m_dwPartyId		= g_dwPartyId;
		pUserRv->m_dwPartyId	= g_dwPartyId;
						
		g_dwPartyId+=1;		
						
		pSendPacket.dwPartyId	= pUser->m_dwPartyId;
		pSendPacket.dwUserIndex	= pUser->m_dwUserIndex;
		g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
				
		pSendPacket.dwPartyId	= pUserRv->m_dwPartyId;		
		pSendPacket.dwUserIndex	= pUserRv->m_dwUserIndex;		
		g_pNet->SendToServer(pDungeonRecv->m_pServer->dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);

		// 파티장의 파티 설정 초기화
		pUserRv->m_PartyConfig.exp = PARTY_CONFIG_EXP_SHARE;
		pUserRv->m_PartyConfig.item = PARTY_CONFIG_ITEM_FIRST;

		// 던전에게 파티장의 파티 설정 초기화를 통보
		WSTDS_PARTY_CONFIG PartyConfig;
		PartyConfig.dwUserIndex = pUserRv->m_dwUserIndex;
		PartyConfig.pConfig = pUserRv->m_PartyConfig;
		PartyConfig.bMsg = 0;
		g_pNet->SendToServer(pDungeonRecv->m_pServer->dwConnectionIndex, (char*)&PartyConfig, PartyConfig.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);

		// 최근 파티 친구로 등록
		AddPartyFriend(pUser->m_dwUserIndex, pUserRv->m_dwUserIndex);
		AddPartyFriend(pUserRv->m_dwUserIndex, pUser->m_dwUserIndex);
	}

	// 파티 시스템 강화 : 최덕석 2005.1.29
	// 파티 설정을 파장의 것으로 동기화
	pUser->m_PartyConfig = pUserRv->m_PartyConfig;

	// 던전에게 파티원의 파티 설정 변경 통보
	WSTDS_PARTY_CONFIG PartyConfig;
	PartyConfig.dwUserIndex = pUser->m_dwUserIndex;
	PartyConfig.pConfig = pUser->m_PartyConfig;
	PartyConfig.bMsg = 0;
	g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&PartyConfig, PartyConfig.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);
}

void CmdPartyJoin(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PARTY_JOIN*	pPacket = (CTWS_PARTY_JOIN*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	CWorldUser*			pUserRv	= g_pUserTable->GetUserInfo(pPacket->dwUserIndex);	
	WSTC_PARTY_JOIN		pSendPacket;

	memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
	pSendPacket.dwUserIndex	= pUser->m_dwUserIndex;	

	if(!pUserRv)
	{
		// 로그 아웃 //
		pSendPacket.bResult = 6;
		goto lb_Result;		
	}
		
	if(pUserRv->m_bParty==FALSE)
	{
		// 파티 거부 //
		pSendPacket.bResult = 5;
		goto lb_Result;
	}
	
	if(pPacket->bErrCode!=0)
	{
		pSendPacket.bResult = BYTE(2+pPacket->bErrCode);
		goto lb_Result;
	}
	
	if(pUser->m_dwPartyId==0)
	{
		if(pUserRv->m_dwPartyId==0)
		{	
			// 파티 메세지 //			
			pSendPacket.bResult		= 0;
			goto lb_Result;
		}
		else		
		{
			// 유저가 다른 파티에 있을 경우.
			pSendPacket.bResult = 1;
			goto lb_Result;
		}
	}
	else
	{
		LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);

		if(!lpPartyTable)
		{
			pSendPacket.bResult = 7;
			goto lb_Result;
		}
			
		if(pUserRv->m_dwPartyId!=0)
		{
			pSendPacket.bResult = 1;
			goto lb_Result;
		}
		else if(lpPartyTable->bCount<MAX_PARTY_USER)
		{
			pSendPacket.bResult = 0;
			goto lb_Result;
		}		
		else
		{
			// 더이상 파티 인원을 추가 할수없다 //
			pSendPacket.bResult = 2;
			goto lb_Result;			
		}
	}

lb_Result:

	if(pSendPacket.bResult==0 || pSendPacket.bResult==3 || pSendPacket.bResult==4)
	{
		__lstrcpyn(pSendPacket.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);		
		g_pNet->SendToUser(pUserRv->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		__lstrcpyn(pSendPacket.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CmdPartyEntry(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PARTY_ENTRY*	pPacket = (CTWS_PARTY_ENTRY*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_PARTY_ENTRY	pSendPacket;

	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pPacket->dwPartyId);

	if(lpPartyTable)
	{
		if(g_pPartyBoardTableHash->GetData(pPacket->dwPartyId))
		{			
			// Error //
			pSendPacket.byType	= 5;			
		}
		else
		{
			LP_PARTYBOARD_TABLE lpPartyBoardTable = (LP_PARTYBOARD_TABLE)LALAlloc(g_pPartyBoardTablePool);

			if(lpPartyBoardTable)
			{			
				if(pUser->m_byLeader==1)
				{
					memset(lpPartyBoardTable, 0, sizeof(PARTYBOARD_TABLE));
					
					time(&pUser->m_sTime);
					time(&lpPartyBoardTable->sTime);					
					lpPartyBoardTable->dwPartyId			= pPacket->dwPartyId;
					lpPartyBoardTable->dwPartyUserLeader	= pPacket->dwUserIndex;
					memset(lpPartyBoardTable->szCharName, 0, sizeof(lpPartyBoardTable->szCharName));
					memset(lpPartyBoardTable->szMemo, 0, sizeof(lpPartyBoardTable->szMemo));
					__lstrcpyn(lpPartyBoardTable->szCharName, pUser->m_szCharacterName, MAX_PARTYENTRY_TEXT);
					__lstrcpyn(lpPartyBoardTable->szMemo, pPacket->szPartyEntryMemo, MAX_PARTYENTRY_TEXT);
					g_pPartyBoardTableHash->Insert(lpPartyBoardTable, lpPartyBoardTable->dwPartyId);

					pSendPacket.byType	= 0;

					WSTC_PARTY_INFOREQUEST pPartyPacket;
					memset(pPartyPacket.szCharName, 0, sizeof(pPartyPacket.szCharName));
					memset(pPartyPacket.szMemo, 0, sizeof(pPartyPacket.szMemo));
					__lstrcpyn(pPartyPacket.szCharName, lpPartyBoardTable->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pPartyPacket.szMemo, lpPartyBoardTable->szMemo, 32);
					pPartyPacket.dwPartyId			= lpPartyBoardTable->dwPartyId;
					pPartyPacket.dwLeaderUserIndex	= lpPartyBoardTable->dwPartyUserLeader;
					pPartyPacket.wDungeonID		= (WORD)pUser->m_dwDungeonID;
					pPartyPacket.byType				= 0;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPartyPacket, pPartyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
				else			
				{
					// Error //
					pSendPacket.byType	= 1;
				}
			}
			else
			{
				// Error //
				pSendPacket.byType	= 2;
			}		
		}		
	}
	else
	{
		// Error //
		pSendPacket.byType	= 2;
	}	
	g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

void CmdPartyInfoRequest(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PARTY_INFOREQUEST* pPacket = (CTWS_PARTY_INFOREQUEST*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	ListNode<PARTYBOARD_TABLE>* lpPartyTableNode = g_pPartyBoardTableHash->GetHead();

	while(lpPartyTableNode)
	{
		LP_PARTYBOARD_TABLE lpPartyBoardTable = (LP_PARTYBOARD_TABLE)lpPartyTableNode->pData;
				
		if(lpPartyBoardTable)
		{
			int nTime = (int)difftime(pUser->m_sTime, lpPartyBoardTable->sTime);
												
			if(nTime>=30)
			{
				CWorldUser* pLeaderUser = g_pUserTable->GetUserInfo(lpPartyBoardTable->dwPartyUserLeader);

				if(pLeaderUser)
				{
					WSTC_PARTY_INFOREQUEST pPartyPacket;
					memset(pPartyPacket.szCharName, 0, sizeof(pPartyPacket.szCharName));
					memset(pPartyPacket.szMemo, 0, sizeof(pPartyPacket.szMemo));
					__lstrcpyn(pPartyPacket.szCharName, lpPartyBoardTable->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
					__lstrcpyn(pPartyPacket.szMemo, lpPartyBoardTable->szMemo, 32);
					pPartyPacket.dwPartyId			= lpPartyBoardTable->dwPartyId;
					pPartyPacket.dwLeaderUserIndex	= lpPartyBoardTable->dwPartyUserLeader;
					pPartyPacket.wDungeonID		= (WORD)pLeaderUser->m_dwDungeonID;
					
					ListNode<PARTYBOARD_TABLE>* lpPartyTableNodeEnd = lpPartyTableNode->pNext;

					if(!lpPartyTableNodeEnd)
						pPartyPacket.byType	= 0;
					else
						pPartyPacket.byType	= 1;

					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPartyPacket, pPartyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}
		lpPartyTableNode = lpPartyTableNode->pNext;
	}

	if(g_pPartyBoardTableHash->GetCount()==0)
	{
		WSTC_PARTY_ENTRY pPartyEntryPacket;
		pPartyEntryPacket.byType = 4;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pPartyEntryPacket, pPartyEntryPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CmdPartyBoardDelete(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PARTY_BOARDDELETE* pPacket = (CTWS_PARTY_BOARDDELETE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	LP_PARTYBOARD_TABLE lpPartyTable = g_pPartyBoardTableHash->GetData(pPacket->dwPartyId);
		
	if(lpPartyTable)
	{
		WSTDS_PARTY_BOARD pPartyBoardPacket;
		pPartyBoardPacket.dwPartyId	= pPacket->dwPartyId;
		
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

		WSTC_PARTY_ENTRY pSendPacket;	
		pSendPacket.byType	= 6;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CmdPartyConfig(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PARTY_CONFIG* pPacket = (CTWS_PARTY_CONFIG*)pMsg;
	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	// 파장이 아니면
	if(!pUser->m_byLeader)
	{
		// 파티 설정은 불가능
		return;
	}

	// 던전에 있는 파장을 포함한 모든 파티원들을 찾고
	LP_PARTY_TABLE lpPartyTable = g_pPartyTableHash->GetData(pUser->m_dwPartyId);

	if(lpPartyTable && lpPartyTable->pPartyUserList)
	{
		POSITION_ pos = lpPartyTable->pPartyUserList->GetHeadPosition();

		while(pos)
		{
			// 파티원이 속해있는 모든 던전에 파티 설정 갱신 통보
			// 파티원이 월드에 있다면, 던전에 조인할 때 통보 --> CmdDungeonJoinSuccess
			CWorldUser* pMember =  (CWorldUser*)lpPartyTable->pPartyUserList->GetNext(pos);

			if(pMember)
			{
				DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pMember->m_dwDungeonID);

				// 월드의 유저 파티 설정도 갱신
				// 월드는 항상 최신의 파티 설정을 들고 있다
				pMember->m_PartyConfig = pPacket->pConfig;

				if(pDungeon)
				{
					// 던전에게 파티 설정 갱신 통보
					WSTDS_PARTY_CONFIG PartyConfig;
					PartyConfig.dwUserIndex = pMember->m_dwUserIndex;
					PartyConfig.pConfig = pPacket->pConfig;
					PartyConfig.bMsg = 1;
					g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&PartyConfig, PartyConfig.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}
		}
	}
}

void CmdPartyFriend(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_PARTY_FRIEND* pPacket = (CTWS_PARTY_FRIEND*)pMsg;
	
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	char szQuery[1024] = {0,};

	wsprintf(szQuery, "SELECT CI.Char_Index, CI.Class, CI.Name FROM PartyFriend PF, CharacterInfo CI WHERE PF.Char_Index=%d AND PF.Friend_Index=CI.Char_Index ORDER BY PF.[date] DESC", pUser->m_dwUserIndex);	
	g_pDb->THOpenRecord(szQuery, QUERY_TYPE_PARTY_FRIEND, (void*)pUser->m_dwConnectionIndex, 32, (BYTE)GAME_DB);	
}

void AddPartyFriend(DWORD dwUserIndex, DWORD dwFriendIndex)
{
	char szQuery[128]	= {0,};
	wsprintf(szQuery, "SP_PARTY_FRIEND_ADD %d, %d", dwUserIndex, dwFriendIndex);
	g_pDb->THExecuteSQL( szQuery, FALSE, 0, NULL, GAME_DB );

}