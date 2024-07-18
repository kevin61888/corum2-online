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


void CmdPartyLeader(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_LEADER* pPacket = (WSTDS_PARTY_LEADER*)pMsg;
	CUser*				pUser	= g_pUserHash->GetData(pPacket->dwUserIndex);

	if(pUser)
	{
		pUser->SetPartyLeader(pPacket->byLeader);
	}	
}

void CmdPartyMsg(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_MSG*	pPacket = (WSTDS_PARTY_MSG*)pMsg;
	CUser*				pUser	= g_pUserHash->GetData(pPacket->dwUserIndex);
	CDungeonLayer*		pLayer;
	
	if(pUser)
	{
		if(!pUser->GetCurDungeon())
			return;

		pLayer = pUser->GetCurDungeonLayer();
		
		if(!pLayer )
			return;

		// 파티 메시지를 클라이언트에게 알려줘
		pUser->SetPartyID(pPacket->dwPartyId);
		
		DSTC_PARTY packet;
		packet.dwUserIndex	= pUser->GetID();
		packet.dwPartyID	= pPacket->dwPartyId;

		pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum());

		if(pUser->m_dwPartyId==0)
		{
			POSITION_ pos = pUser->m_pUsingMagicArrayList->GetHeadPosition();
			
			while(pos)
			{
				// 마법진 지워버려.
				ITEM_MAGICARRAY_DESC* pDesc = (ITEM_MAGICARRAY_DESC*)pUser->m_pUsingMagicArrayList->GetNext(pos);
				pUser->DetachMagicArray(pDesc);
			}			
		}

		if (pUser->GetCurDungeon()->GetDungeonDataEx()->m_bSiege )
		{
			pUser->SetAttackMode(pUser->GetCurDungeon()->GetAttackMode(pUser));

			// 배신자 안녕ㅡ.ㅡ
			if (pUser->GetCurLayerIndex() && !pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] &&
				pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
			{
				pUser->GetCurDungeon()->GotoLobbyUser(pUser);
			}
		}

		if(pUser->m_dwPartyId!=0)
		{
			WSTDS_PARTY_LEVEL pLevelPacket;
			pLevelPacket.dwLevel		= pUser->GetLevel();
			pLevelPacket.dwPartyId		= pUser->m_dwPartyId;
			pLevelPacket.dwUserIndex	= pUser->GetID();
			g_pNet->SendToServer(dwConnectionIndex, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			WSTDS_PARTY_HP pHpPacket;
			pHpPacket.dwPartyId			= pUser->m_dwPartyId;
			pHpPacket.dwUserIndex		= pUser->GetID();
			pHpPacket.wMaxHp			= (WORD)pUser->GetMaxHP();
			pHpPacket.wHP				= (WORD)pUser->GetHP();
			g_pNet->SendToServer(dwConnectionIndex, (char*)&pHpPacket, pHpPacket .GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}	

		if(pUser->m_dwGuildId!=0)
		{
			WSTDS_GUILD_LEVEL pLevelPacket;
			pLevelPacket.dwLevel		= pUser->GetLevel();
			pLevelPacket.dwGuildId		= pUser->m_dwGuildId;
			pLevelPacket.dwUserIndex	= pUser->GetID();
			g_pNet->SendToServer(dwConnectionIndex, (char*)&pLevelPacket, pLevelPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}		
	}
}


void CmdPartyInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_INFO* pPacket = (WSTDS_PARTY_INFO*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndexRv);

	if(pUser)
	{
		if (!pUser->GetCurDungeon())	return;
		DSTWS_PARTY_INFO pSendPacket;
		memset(pSendPacket.szCharName, 0, sizeof(pSendPacket.szCharName));
		__lstrcpyn(pSendPacket.szCharName, pPacket->szCharName, MAX_CHARACTER_REAL_LENGTH);
		pSendPacket.dwUserIndex		= pPacket->dwUserIndex;
		pSendPacket.dwUserIndexRv	= pPacket->dwUserIndexRv;
		pSendPacket.wDungeonID		= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
		pSendPacket.byLayerIndex	= pUser->GetCurLayerIndex();
		g_pNet->SendToServer(dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CmdFriendInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_FRIENDUSER_INFO* pPacket = (WSTDS_FRIENDUSER_INFO*)pMsg;

	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndexRv);

	if(pUser)
	{
		if(!pUser->GetCurDungeon())
			return;

		DSTWS_FRIENDUSER_INFO pSendPacket;		
		pSendPacket.dwUserIndex		= pPacket->dwUserIndex;
		pSendPacket.dwUserIndexRv	= pPacket->dwUserIndexRv;
		pSendPacket.wDungeonID		= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
		pSendPacket.byLayerIndex	= pUser->GetCurLayerIndex();
		g_pNet->SendToServer(dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


void CmdPartyBoard(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_BOARD* pPacket = (WSTDS_PARTY_BOARD*)pMsg;

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	if(pDungeon)
	{	
		DSTC_PARTY_BOARD pPartyBoardPacket;
		pPartyBoardPacket.dwPartyId	= pPacket->dwPartyId;
		pDungeon->BroadCast((char*)&pPartyBoardPacket, pPartyBoardPacket.GetPacketSize());
	}
}

void CmdPartyConfig(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	WSTDS_PARTY_CONFIG* pPacket = (WSTDS_PARTY_CONFIG*)pMsg;

	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->wHeader, dwLength);
		return;
	}

	// 설정을 변경 할 유저를 찾고
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	// 유저가 있으면
	if(pUser)
	{
		// 대상의 파티 설정을 변경
		pUser->m_PartyConfig = pPacket->pConfig;
	
		// 클라이언트에게 파티 설정 변경을 알림
		DSTC_PARTY_CONFIG PartyConfig;
		PartyConfig.pConfig = pPacket->pConfig;
		PartyConfig.bMsg = pPacket->bMsg;
		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&PartyConfig, PartyConfig.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);
	}
}