#include "MsgProc_for_y2jin.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "GameSystem.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "MonsterTable.h"
#include "Monster.h"
#include "DungeonTable.h"
#include "OwnServer.h"
#include "recvmsg.h"
#include "GameSystemNative.h"


void InitializePacketProcY2jin()
{
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_EMOTICON ]						= CmdEmoticon;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_REQUEST_MATCH_MAN2MAN ]			= CmdRequestMatchM2M;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_REPLY_MATCH_MAN2MAN ]				= CmdReplyMatchM2M; // -- seven

#ifdef NEW_COMMAND
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GM_COMMAND]						= CmdNewGMCommand;
#endif
}

void CmdEmoticon( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_EMOTICON * pPacket = (CTDS_EMOTICON*)pMsg;
	CDungeonLayer*	pLayer;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		
	if(!pUser)					return;
	if(!pUser->GetCurDungeon())	return;
	
	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	DSTC_DUNGEON_EMOTICON		packet;
	packet.bWhatEmoticon	= pPacket->bWhatEmoticon;
	packet.dwUserIndex		= pUser->GetID();
		
//	CDungeon* pDungeon = pUser->GetCurDungeon();
	pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum() );
}

//
// 유저가 대결 신청.
//
void CmdRequestMatchM2M( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_REQUEST_MATCH_MAN2MAN * pPacket = (CTDS_REQUEST_MATCH_MAN2MAN *)pMsg;
	CDungeonLayer*	pLayer;

	CUser* pRequestUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pRequestUser)					return;
	if(!pRequestUser->GetCurDungeon())	return;

	CUser* pRequestedUser = g_pUserHash->GetData(pPacket->dwTargetUserIndex);
	if(!pRequestedUser )					return;
	if(!pRequestedUser->GetCurDungeon())	return;
	
	if(CheckShop(pRequestUser))
		return;

	if(pRequestedUser->m_byRest==1)
		return;

	if(CheckShop(pRequestUser))
		return;	

	if(pRequestUser->m_byRest==1)
		return;
	
	// 대전중인사람은 대전신청을 할수 없다.
	if (pRequestUser->m_sMatchDescInfo.m_bMatching)
	{
		DSTC_DUNGEON_REPLY_MATCH_MAN2MAN	ReplyPacket;
		ReplyPacket.bReplyCode = 3;		// 대전 중이다. 
		NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		
		return;		
	}

	if (pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex)
	{ 
		// 현재 대전 신청자는 대전 접선중 이다. 
		DSTC_DUNGEON_REPLY_MATCH_MAN2MAN	ReplyPacket;
		
		ReplyPacket.bReplyCode = 4;
		NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		
		return;
	}

	if (pRequestedUser->m_sMatchDescInfo.m_dwMatchUserIndex)
	{ 
		// 현재 신청받은자는 대전 접선중 이다.
		DSTC_DUNGEON_REPLY_MATCH_MAN2MAN	ReplyPacket;		
		ReplyPacket.bReplyCode = 4;
		NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		return;
	}

	// 신청자, 신청받은자 누구인지 명시 
	pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex	= pRequestedUser->GetID();
	pRequestedUser->m_sMatchDescInfo.m_dwMatchUserIndex	= pRequestUser->GetID();

	// 같은 층에 있는지 검사.
	CDungeonLayer *pRequestedUserInLayer = pRequestedUser->GetCurDungeonLayer();
	pLayer = pRequestUser->GetCurDungeonLayer();

	if( !pLayer || !pRequestedUserInLayer || 
		pRequestedUserInLayer != pLayer)
	{
		// Layer 포인터가 썩었거나 같은 층에 있지않다면 리턴 
		return;
	}
	
	DSTC_DUNGEON_REQUEST_MATCH_MAN2MAN		packet;
	packet.dwRequestUserIndex = pRequestUser->GetID();
		
	NetSendToUser(pRequestedUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}

//
// 유저가 대결에 응답했다.
//
void CmdReplyMatchM2M( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_REPLY_MATCH_MAN2MAN * pPacket = (CTDS_REPLY_MATCH_MAN2MAN *)pMsg;
	
	// 응답한사람
	CUser* pReplyUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		
	if(!pReplyUser)					return;
	if(!pReplyUser->GetCurDungeon())return;

	CUser* pRequestUser = g_pUserHash->GetData(pPacket->dwRequestUserIndex);
	if (!pRequestUser) 
	{
		pReplyUser->SetStatus(UNIT_STATUS_NORMAL);
		return;
	}

	if (!pRequestUser->GetCurDungeon())
		return;

	if (pPacket->bReplyCode != 1)
	{ // 대전을 원치 않는다. 
		DSTC_DUNGEON_REPLY_MATCH_MAN2MAN ReplyPacket;
		
		ReplyPacket.bReplyCode				= pPacket->bReplyCode;
		ReplyPacket.dwMatchUserIndex		= pReplyUser->GetID();	
		pReplyUser->m_sMatchDescInfo.m_dwMatchUserIndex		= 0;
		pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex	= 0;
		NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	if ( pReplyUser->GetCurDungeonID() != pRequestUser->GetCurDungeonID() || 
		!pReplyUser->GetCurDungeon()->GetDungeonDataEx()->IsVillage() || 
		!pRequestUser->GetCurDungeon()->GetDungeonDataEx()->IsVillage() 	) 
	{		
		return;
	}

	if (pReplyUser->m_byExTrade || pRequestUser->m_byExTrade)
	{ // 대전상대중 누군가 교환중이다.
		DSTC_DUNGEON_REPLY_MATCH_MAN2MAN ReplyPacket;
		
		ReplyPacket.bReplyCode				= 6;
		pReplyUser->m_sMatchDescInfo.m_dwMatchUserIndex		= 0;
		pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex	= 0;
		NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		NetSendToUser(pReplyUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pReplyUser->GetID());

	if(pItemShop)
	{
		if(pItemShop->byType==1)
		{
			DSTC_DUNGEON_REPLY_MATCH_MAN2MAN ReplyPacket;		
			ReplyPacket.bReplyCode				= 7;
			ReplyPacket.dwMatchUserIndex		= pRequestUser->GetID();	
			pReplyUser->m_sMatchDescInfo.m_dwMatchUserIndex		= 0;
			pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex	= 0;
			NetSendToUser(pReplyUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			ReplyPacket.bReplyCode				= 9;
			ReplyPacket.dwMatchUserIndex		= pReplyUser->GetID();	
			NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
	}

	if(pReplyUser->m_byRest==1)
	{
		DSTC_DUNGEON_REPLY_MATCH_MAN2MAN ReplyPacket;		
		ReplyPacket.bReplyCode				= 8;
		ReplyPacket.dwMatchUserIndex		= pRequestUser->GetID();	
		pReplyUser->m_sMatchDescInfo.m_dwMatchUserIndex		= 0;
		pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex	= 0;
		NetSendToUser(pReplyUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		ReplyPacket.bReplyCode				= 12;
		ReplyPacket.dwMatchUserIndex		= pReplyUser->GetID();	
		NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}

	pItemShop = g_pItemShopHash->GetData(pRequestUser->GetID());

	if(pItemShop)
	{
		if(pItemShop->byType==1)
		{
			DSTC_DUNGEON_REPLY_MATCH_MAN2MAN ReplyPacket;		
			ReplyPacket.bReplyCode				= 7;
			ReplyPacket.dwMatchUserIndex		= pReplyUser->GetID();	
			pReplyUser->m_sMatchDescInfo.m_dwMatchUserIndex		= 0;
			pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex	= 0;
			NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

			ReplyPacket.bReplyCode				= 9;
			ReplyPacket.dwMatchUserIndex		= pRequestUser->GetID();	
			NetSendToUser(pReplyUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
	}

	if(pRequestUser->m_byRest==1)
	{
		DSTC_DUNGEON_REPLY_MATCH_MAN2MAN ReplyPacket;		
		ReplyPacket.bReplyCode				= 8;
		ReplyPacket.dwMatchUserIndex		= pReplyUser->GetID();	
		pReplyUser->m_sMatchDescInfo.m_dwMatchUserIndex		= 0;
		pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex	= 0;
		NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		ReplyPacket.bReplyCode			= 10;
		ReplyPacket.dwMatchUserIndex	= pRequestUser->GetID();	
		NetSendToUser(pReplyUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
	
	// 대전 레이어 물색
	// 1층 부터 끝층까지
	int nMatchLayer = 0;
	int nTotalLayerNum = pReplyUser->GetCurDungeon()->GetTotalLayer();
	CDungeon* pDungeon = pReplyUser->GetCurDungeon();
	for (int i = 1; i < nTotalLayerNum; i++)
	{ 

 		if (pDungeon->GetDungeonLayer(i)->m_bMatching == FALSE)
		{ // 대전중이지 않는 던전 
			nMatchLayer = i;
			break;
		}
	}

	if (nMatchLayer == 0)
	{ // 대전장소가 꽉찼다.		
		DSTC_DUNGEON_REPLY_MATCH_MAN2MAN	ReplyPacket;
		
		ReplyPacket.bReplyCode = 5;		// 대전 장소가 꽉차있다. 

		pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex = 0;
		pReplyUser->m_sMatchDescInfo.m_dwMatchUserIndex = 0;

		NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		NetSendToUser(pReplyUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

		return;	
	}

	// 대답을 클라이언트에 알림
	DSTC_DUNGEON_REPLY_MATCH_MAN2MAN	ReplyPacket;
	ReplyPacket.bReplyCode = pPacket->bReplyCode;

	switch(pPacket->bReplyCode)
	{
		case 1: // 수락
		{ 

			if (pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex != pReplyUser->GetID())
			{ // 요청 받은 놈이 다르다.
				return;
			}

			VECTOR2	v2Pos = {0.f, 0.f}, v2Pos2 = {0.f, 0.f};
			
			if(!pDungeon->GetDungeonLayer(nMatchLayer)->GetStartTile(0, &v2Pos))
			{
				ReplyPacket.bReplyCode = 0;	
		
				NetSendToUser(pRequestUser->m_dwConnectionIndex
					, (char*)&ReplyPacket
					, ReplyPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				NetSendToUser(pReplyUser->m_dwConnectionIndex
					, (char*)&ReplyPacket
					, ReplyPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);

				return;		//시작위치 찾을 수 없슴!! 
			}

			
			if(!pDungeon->GetDungeonLayer(nMatchLayer)->GetStartTile(1, &v2Pos2))
			{
				ReplyPacket.bReplyCode = 0;

				NetSendToUser(pRequestUser->m_dwConnectionIndex
					, (char*)&ReplyPacket
					, ReplyPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);
				NetSendToUser(pReplyUser->m_dwConnectionIndex
					, (char*)&ReplyPacket
					, ReplyPacket.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION);

				return;		//시작위치 찾을 수 없슴!! 
			}
			
			// 유저에게 대결 플레그 세팅
			pRequestUser->m_sMatchDescInfo.m_bMatching	= TRUE;
			pReplyUser->m_sMatchDescInfo.m_bMatching		= TRUE;

			pDungeon->GetDungeonLayer(nMatchLayer)->m_bMatching = TRUE;
			
			ReplyPacket.dwMatchUserIndex = pReplyUser->GetID();
			NetSendToUser(pRequestUser->m_dwConnectionIndex
				, (char*)&ReplyPacket
				, ReplyPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
			ReplyPacket.dwMatchUserIndex = pRequestUser->GetID();
			NetSendToUser(pReplyUser->m_dwConnectionIndex
				, (char*)&ReplyPacket
				, ReplyPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);

			//유저를 레이어에서 사라지게 하는 처리를 하고 (Remove)
			pDungeon->RemoveUserForInsidePortal(pRequestUser);
			pDungeon->RemoveUserForInsidePortal( pReplyUser );	

			//같은던전에서의 포탈이동용(층끼리만 이동하는 포탈의 경우 유저 삽입),  유저가 포탈이동으로 들어가는 처리를 한다. 
			pDungeon->SendPortalMsg( pReplyUser, nMatchLayer, &v2Pos, false );
			pDungeon->SendPortalMsg( pRequestUser, nMatchLayer, &v2Pos2, false);

		}
		break;
		default:
		{
			// 기타등등의 메시지는 바로 돌려보낸다.
			// 상대 유저 인덱스를 초기화 한다.
			pRequestUser->m_sMatchDescInfo.m_dwMatchUserIndex	= 0;
			pReplyUser->m_sMatchDescInfo.m_dwMatchUserIndex		= 0;

			NetSendToUser(pRequestUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			NetSendToUser(pReplyUser->m_dwConnectionIndex, (char*)&ReplyPacket, ReplyPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
		break;
	}

}

void CmdNewGMCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTDS_NEW_GM_COMMAND * pCommand = (CTDS_NEW_GM_COMMAND *)pMsg;

	CUser * pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pUser) return;
	
	if( !pUser->GetCurDungeon())	
		return;

	if (!pUser->IsGMUser())
	{
		return;
	}

	switch(pCommand->sBaseGMCmdParam.eGMCommand) 
	{
	case GM_Command_Create_Item:
		{
			if (pCommand->sGMCmdParamCreateItem.wItemID < 0)
			{
				goto Send_Return_Err_packet;
			}

			CreateItemByGM( pUser, pCommand->sGMCmdParamCreateItem.wItemID );
		}
		break;

	case GM_Command_Summon_Monster:
		{
			if (pCommand->sGMCmdParamSummonMonster.wMonsterID < 0)
				goto Send_Return_Err_packet;

			if (SUMMON_MONSTER_MAX_COUNT < pCommand->sGMCmdParamSummonMonster.wMonsterCount)
				goto Send_Return_Err_packet;

			for (
				int i = 0; 
				i < pCommand->sGMCmdParamSummonMonster.wMonsterCount; 
				i++ )
			{
				pUser->GetCurDungeonLayer()->CallMonsterByGM( 
					pUser, 
					0, 
					pCommand->sGMCmdParamSummonMonster.wMonsterID, 
					0	
				); 
			}
		}
		break;

	case GM_Command_Silence:
		{
			if (TRUE == pCommand->sGMCmdParamSilence.bAll)
			{
				// Every body Set the mouth!
				pUser->GetCurDungeon()->SetTemp(DUNGEON_TEMP_CHAT_DISABLE, 
												pCommand->sGMCmdParamSilence.bFlag);
				
			}
			else
			{
				// Someone user set the mouth!		
				CUser * pUser = 
					g_pUserHash->GetData(pCommand->sGMCmdParamSilence.dwUserIndex);
				if (!pUser)	
					goto Send_Return_Err_packet;

				pUser->m_dwTemp[USER_TEMP_GM_DRIVED_SILENCE_FLAG] = 
					pCommand->sGMCmdParamSilence.bFlag;		
			}
		}
		break;

	case GM_Command_PK_Mode:
		{
			pUser->GetCurDungeon()->SetTemp(
				DUNGEON_TEMP_PKMODE, 
				pCommand->sGMCmdParamPKMode.bPKMode
			);
			
			// 모든 유저에게 PK(평화, 전쟁)모드로 돌린다. 
			pUser->GetCurDungeon()->AllUserPKMode(pCommand->sGMCmdParamPKMode.bPKMode);

			// 메시지를 클라이언트에 출력해야하는데 
			// dstc_pk_mode 에다가
			// type 물려서 보내자
			// NOT YET IMPLEMENT ...
		}
		break;

	case GM_Command_Observe:
		{
			// NOT YET IMPLEMENT ...
		}
		break;

	case GM_Command_Change_Monster:
		{
			// NOT YET IMPLEMENT ...
		}
		break;

	case GM_Command_Empty:
	default:
		{
			goto Send_Return_Err_packet;
		}
		break;
	}

Send_Return_Err_packet:
	return;
}