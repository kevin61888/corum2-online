#include "stdafx.h"
#include <sys/TIMEB.H>
#include "Dungeon.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "OwnServer.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonLayer.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "DungeonTable.h"
#include "AcceptTable.h"
#include "recvmsg.h"
#include "RecordSet.h"
#include "Map.h"
#include "Section.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "MonsterTable.h"
#include "ServerFunction.h"
#include "GameSystemNative.h"
#include "DBProc.h"
#include "MsgProc.h"
#include "MsgProc_for_jjw6263.h"
#include "MsgProc_for_minbobo.h"
#include "MsgProc_for_Yak.h"
#include "MsgProc_for_Y2jin.h"
#include "ItemAttrLayer.h"

#define ITEM_SKILL_BOOK_START	20519
#define ITEM_SKILL_BOOK_END		20578

#define BAG_ITEM_NUM_FRIST	3600
#define BAG_ITEM_NUM_LAST	3700


#define FAIL_VALUE(x1,x2)	{ x1 = x2; __leave; }


void (*PacketProc[ MAX_UPDATE_GAME ][ MAX_PACKET_NUM ])( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength );


void InitializePacketProc()
{
	memset(PacketProc, 0, sizeof(PacketProc));

	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_JOIN ]					= CmdDungeonJoin;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_MOVE ]					= CmdDungeonMove;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_STOP ]					= CmdDungeonStop;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_ATTACK_USER_MON ]			= CmdDungeonAttack_User_Mon;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_ATTACK_USER_USER ]		= CmdDungeonAttack_User_User;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_CHAT ]					= CmdDungeonChat;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_CASTING ]					= CmdDungeonSkillCasting; 
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_SKILL_LEVELUP ]					= CmdSkillLevelUp;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ITEM_PICKUP ]						= CmdPickupItem;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ITEM_USED ]						= CmdItemUsed;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_STATUS_LEVELUP]					= CmdStatusLevelUp;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_REQUEST_INFO ]			= CmdDungeonRequestInfo;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_CALLDUARDIAN ]			= CmdCallGuardian;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_MONSTER_COMMAND ]					= CmdMonsterCommand;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_ATTACK ]					= CmdGuardianAttack;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_COMMAND ]				= CmdGuardianCommand;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_COMMAND_MOVE ]			= CmdGuardianCommandMove;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_COMMAND_ATTACK]			= CmdGuardianCommandAttack;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GUARDIAN_SKILL_SELECT]			= CmdGuardianSkillSelect;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_GM::CMD_QUICK_MOVE ]						= CmdGMQuickMove;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PORTAL_MOVE]						= CmdPortalMove;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_REQUEST_SECTION_INFO_FOR_PORTAL]  = CmdRequestSectionInfoForPortal;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_FIND_HACK ]						= CmdFindHack;
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_TICK_CHECK]						= CmdTickCheck;
	
#ifndef NEW_COMMAND
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_GM_COMMAND]						= CmdGmCommand;			
#endif
						
	InitializePacketProcJJW6263();
	InitializePacketProcMinbobo();
	InitializePacketProcYak();
	InitializePacketProcY2jin();	
}


// 포탈 패킷
void CmdPortalMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PORTAL_MOVE *pPacket = (CTDS_PORTAL_MOVE*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);



	if(!pUser)					return;
	if(!pUser->GetCurDungeon())	return;
	CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();

	BYTE byPortalFail = NULL;

	__try
	{
		if(	DUNGEON_TYPE_VILLAGE == DungeonIDtoType( pPacket->wToDungeonID )// hwoarang 마을이라면
		&&	pPacket->bLayerNo != 255	 	// 몇층인지 모를때 255를 넣기때문에 필요
		&&	pPacket->bLayerNo != 0			// 마을에서는 0층 이상은 대전 레이어 이다.
			)
		{
			FAIL_VALUE(byPortalFail, 9);
		}

		if (DUNGEON_TYPE_EVENT == pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType())
			FAIL_VALUE(byPortalFail, 15);
		
		if (TRUE == pUser->GetCurDungeon()->GetDungeonDataEx()->IsEventDungeon() && 0 == pPacket->bPortalType)
			FAIL_VALUE(byPortalFail, 16);

		if (NULL == pLayer)
			FAIL_VALUE(byPortalFail, 9);

		if (UNIT_STATUS_DEAD == pUser->GetUnitStatus())
			FAIL_VALUE(byPortalFail, 7);

		if (TRUE == pUser->m_bPortalMoving)
			FAIL_VALUE(byPortalFail, 5); 

//	길드전이 GM 타입 포탈을 사용한다.: hwoarang 050202
//		if (Portal_Type_GM == pPacket->bPortalType && FALSE == pUser->IsGMUser())
//			FAIL_VALUE(byPortalFail, 9);

		CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo( pPacket->wToDungeonID );

		if(pDungeon)	
		{
			// 같은 포트이다. 이 던전서버에 할당된 던전이면 바로 처리

			if( pDungeon->GetDungeonDataEx()->m_bSiege && !pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser) )
			{
				pUser->GoToWorld( pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD );
				return;

				// 매칭중이라면......
				// 길드전중이었다면
				// 허용한다.
			/*	if( (pUser->m_sMatchDescInfo.m_bMatching) 
					|| (pUser->m_byGuildFlag != G_W_F_NONE && pUser->m_byGuildFlag != G_W_F_OBSERVER) 
					)
				{
					// 포탈 안됩니다.만, 해준다.
					pUser->GoToWorld( pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD );
					return;
				}
				else
				{
					FAIL_VALUE(byPortalFail, 8);
				}				*/
			}

			if ( pUser->GetPortalType() != Portal_Type_GM && pUser->GetCurDungeonID() == pPacket->wToDungeonID	&& pUser->GetCurLayerIndex() == pPacket->bLayerNo )
				FAIL_VALUE(byPortalFail, 6);				
						
			if (pDungeon->GetDungeonDataEx()->IsConquer() && !pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser) )
			{
				if (pUser->GetPortalType() == Portal_Type_GM)
				{
					// GM 이 유저를 불러 냈기 때문에 무조건 간다.
				}
				else if (pUser->m_dwMoney >= pDungeon->GetDungeonDataEx()->GetEntrancePay())
				{
					// 입장료 깍아라.
					pUser->m_dwMoney -= pDungeon->GetDungeonDataEx()->GetEntrancePay();
					pDungeon->GetDungeonDataEx()->SetAccEntrance(pDungeon->GetDungeonDataEx()->GetEntrancePay());
				}
				else
				{
					FAIL_VALUE(byPortalFail, 17);					
				}
			}

			// 포탈 이동중이라는 Flag
			pUser->m_bPortalMoving = 1;	
			
			if(pUser->GetCurDungeonID() == pPacket->wToDungeonID)	
			{
				// 같은 던전이지만 층끼리의 이동일때(층간이동)
				
				// 유저를 레이어에서 사라지게 하는 처리를 하고 (Remove)
				pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);
				pUser->GetCurDungeon()->RemoveUserForInsidePortal(pUser);

				UpdateUserDBThread(pUser);

				// 같은던전에서의 포탈이동용(층끼리만 이동하는 포탈의 경우 유저 삽입),
				// 유저가 포탈이동으로 들어가는 처리를 한다. 
				pUser->GetCurDungeon()->SendPortalMsg( pUser, pPacket->bLayerNo, &pPacket->v2Pos, FALSE );
			}
			else		
			{
				// 같은 서버이나 다른던전으로의 이동일때 

				// 다른 클라이언트 들에게 이 유저가 사라지는 메시지를 보낸다.
				DSTC_PORTAL_DISAPPEAR DisAppear;
				DisAppear.dwUserIndex	= pUser->GetID();
				DisAppear.bType			= 1;
				pLayer->BroadCastSectionMsg( (char*)&DisAppear, DisAppear.GetPacketSize(), pUser );
				
				UpdateUserForAI(pUser);
				UpdateUserDBThread(pUser);

				pDungeon->SendPortalMsg(pUser, pPacket->bLayerNo, &pPacket->v2Pos, TRUE);			
				// 현재 던전에서 유저를 삭제 하고 
				// 포탈시 싱크가 맞지 않아 에러가 발생하여 위치를 변경함 2005.02.14 김영대
				// pDungeon->SendPortalMsg(pUser, pPacket->bLayerNo, &pPacket->v2Pos, TRUE); 다음에 유저리스트에서 삭제
				pUser->GetCurDungeon()->RemoveUser( pUser );
				pUser->SetCurDungeon(NULL);
			}
		}
		else	
		{
			// 포트가 틀린 던젼.이 던전서버에 할당된 던전이 아니면 월드로 제어권 돌림 

			if(pUser->pBufForPortal)
			{
				// Portal 처리 마치기 전에 또 한번 쓴 경우 겠지...
				LALFree(g_pPortalBufferPool, pUser->pBufForPortal );
				pUser->pBufForPortal = NULL;
				
				FAIL_VALUE(byPortalFail, 5);				
			}

			// 포탈 이동중이라는 Flag
			pUser->m_bPortalMoving = 1;	

			// 승인 요청의 응답을 받을때 까지 PORTAL_BUCKET을 저장하고 있자..
			PORTAL_BUCKET* pPortal =  (PORTAL_BUCKET*)LALAlloc(g_pPortalBufferPool);	
			memset(pPortal, 0, sizeof(PORTAL_BUCKET));	
			
			pPortal->wPortalItemID	= pPacket->wPortalItemID;
			pPortal->bLayerNo		= pPacket->bLayerNo;
			pPortal->wToDungeonID	= pPacket->wToDungeonID;
			pPortal->v2Pos			= pPacket->v2Pos;
			pUser->pBufForPortal	= pPortal;
					
			// 해당 던전서버에 이 유저의 접속 승인을 요청 
			DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL	packet;	
			packet.wFromDungeonID			= pUser->GetCurDungeonID();
			packet.wDungeonID				= pPacket->wToDungeonID;
			packet.bLayerIndex				= pPacket->bLayerNo;
			packet.dwUserIndex				= pUser->GetID();
			packet.dwUserMoney				= pUser->m_dwMoney;
			packet.dwUserLevel				= pUser->GetLevel();
			packet.bConnectionToGameRoom	= pUser->m_bConnectToGameRoom;
			packet.dwTargetUserIndex		= pPacket->dwTargetUserIndex;
			packet.bPortalType				= pPacket->bPortalType;
			g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}
	__finally
	{
		// 5: "포탈 이동 중에는 사용할 수 없습니다."
		// 6: "같은 던전 같은 층에서 포탈을 사용할 수 없습니다."
		// 7: "죽었을때는 이동할 수 없습니다."
		// 8: "이동하려는 던전이 공성전이 진행중 입니다. 공성중에는 이동할 수 없습니다."
		// 9: "조건이 맞지않아 입장할 수 없습니다."
		// 15: "이벤트 던젼에서는 포탈기능을 이용할 수 없습니다."
		// 16: "이벤트 던젼이 목적지인 경우, 포탈기능을 이용할 수 없습니다."
		// 17: "입장료가 부족합니다."
		if(byPortalFail)
		{
			SendPortalFailMessage(pUser, pPacket->wToDungeonID, byPortalFail, pPacket->wPortalItemID);
		}
	}
}


void CmdRequestSectionInfoForPortal( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if (!pUser)	return;

	CTDS_REQUEST_SECTION_INFO_FOR_PORTAL* pPacket = (CTDS_REQUEST_SECTION_INFO_FOR_PORTAL*)pMsg;
	
	CDungeon* pDungeon = (CDungeon*)g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(!pDungeon)
	{
		Log(LOG_IMPORTANT, "Unknown DungeonID Received...  at CmdRequestSectionInfoForPortal() (DungeonID:%d)"
			, pPacket->wDungeonID);
		return;
	}

	CDungeonLayer*	pLayer = pDungeon->GetDungeonLayer( pPacket->bLayerIndex );
	if (!pLayer)	return;

	// 던전이 바뀌는 경우 
	if(pPacket->bJoinDungeon)
	{
		pDungeon->JoinUserForPortal(pUser, pPacket->bLayerIndex);
	}
	
	// 내가 나타날 위치에 뭔가가 있나?
	VECTOR2 v2Start = *pUser->GetReservPotalPosition();
	MAP_TILE* pTile = pLayer->GetMap()->GetTile( v2Start.x, v2Start.y );
	
	if(!pTile)
	{
		Log(LOG_IMPORTANT, "Invalid User Position at CmdRequestSectionInfoForPortal() (Name:%s)",pUser->m_szName );
		pUser->CallBackCompulsiveDisconnectUser();
		return;
	}

	if (pTile->wAttr.uOccupied)
	{
		v2Start = pLayer->GetAroundEmptyTile(pTile->wIndex_X, pTile->wIndex_Z);
	}

	// 해당 던전에 추가 
	pLayer->InsertUser( pUser,  &v2Start);
	pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);

	if (pDungeon->GetDungeonDataEx()->m_bSiege)
	{
		pDungeon->SendSiegeInfo(pUser);
		pUser->SetAttackMode(pDungeon->GetAttackMode(pUser));
	}
		
	// 포탈 이동중인 Flag Off
	pUser->m_bPortalMoving = 0;		

	// 이동하는 본인과, 다른 주변 유저들에게 해당섹션과, 링크섹션 정보를 모두 보냄 
	pUser->GetCurDungeon()->SendJoinSectionInfo( pUser , TRUE);
	{
		DSTC_RESPONSE_PORTAL_JOIN_USER packet;	
		packet.vecStart		= *pUser->GetCurPosition();
		packet.dwUserIndex	= pUser->GetID();

		pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum());		
	}

	if (pUser->GetCurLayerIndex()	&&
		pUser->IsCurDungeonSiege()	&&
		pUser->GetAttackMode() != ATTACK_MODE_DEFENCE &&
		!pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY])
	{	
		pUser->GetCurDungeon()->GotoLobbyUser(pUser);
	}
	else if (!pUser->GetCurLayerIndex() && pUser->IsCurDungeonSiege())
	{
		pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] = 1;	
	}
	
	if (pUser->m_sPKDescInfo.m_bPKCount | pUser->m_sPKDescInfo.m_bPKRepeatCount)
	{
		pUser->m_sPKDescInfo.m_pPKList = g_pThis->m_pPKUserCheckList->AddTail(pUser);
	}
}


void CmdPickupItem( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_ITEM_PICKUP*	pItemPickup = (CTDS_ITEM_PICKUP*)pMsg;
	DSTC_GUILD_ITEM		pPacket;
	DSTC_ITEM_PICKUP	PickupItem;
	DSTC_SHOP_FAIL		pShopPacket;
	DSTC_DISAPPEAR_ITEM	DisappearItem;
	ITEM_SERVER*		pItem			= NULL;
	CDungeonLayer*		pLayer			= NULL;
	CItem				ItemLog;
	BOOL				bAppear			= TRUE;
	int					i				= 0;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	
	// 파티플 카르츠 분배 : 2005.1.26
	CUser* pMember[MAX_PARTY_USER] = {NULL, };
	WORD wMemberCount = 0;

	if(!pUser)						return;
	if(!pUser->GetCurDungeon())		return;	//포탈 이동중 
	
	pLayer = pUser->GetCurDungeonLayer();
	
	if(!pLayer)						return;		
	if(pUser->m_bPortalMoving==1)	return;	
	if(pUser->m_byRest==1)			return;
	if(pItemPickup->bSectionNum==0)	return;	
	if(CheckShop(pUser))			return;

	if(!(pItemPickup->bInv>=57 && pItemPickup->bInv<=60))
		pUser->m_dwPlayserShop = 0;

	memset( &ItemLog, 0, sizeof( CItem ) );
	memset( &PickupItem, 0, sizeof( DSTC_ITEM_PICKUP ) );
		
	PickupItem.bInv		= pItemPickup->bInv;
	PickupItem.bStatus	= UPDATE_GAME_PLAY;
	PickupItem.bHeader	= Protocol_DSTC::CMD_ITEM_PICKUP;

	if(pUser->GetPartyID())
	{
		// 같은 층의 파티원을 모두 구한다 : 최덕석 2005.1.26
		Node<CUser>* pnode = pUser->GetCurDungeon()->GetPartyUserHash()->GetBucketHead(pUser->GetPartyID());
		while(pnode)
		{
			if(pnode->pData->GetCurDungeon()
			&& pnode->pData->GetUnitStatus() != UNIT_STATUS_DEAD)
			{
				if(pnode->pData->GetCurDungeonLayer() == pUser->GetCurDungeonLayer()
					&& pnode->pData->GetPartyID() == pUser->GetPartyID())
				{
					pMember[wMemberCount++] = pnode->pData;
				}
			}

			pnode = pnode->pNext;
		}
	}

	switch( pItemPickup->bInv )
	{
	case 1:		// 바닥에서 바로 장비 아이템으로 이동할 때 //
		{			

			if(!pLayer->GetSection(pItemPickup->bSectionNum))
				return;

			pItem = pLayer->GetSection(pItemPickup->bSectionNum)
						->m_pItemHash->GetData(pItemPickup->i64ItemID);

			if(!pItem || pItem->Item.m_wItemID==0)
				return;

			if(SetIsCopyItem(pUser, &PickupItem, &pItem->Item, 1, DEF_COPYITEM_TYPE_LARGE))
				goto lb_exit1;

			int nValue = GetItemIndex(&pItem->Item);
			
			if(nValue<0)
				return;
			if(GetItemType(pItem->Item.GetID())==ITEM_TYPE_SMALL)
				return;

			// 파티풀 드롭 아이템이고 습득자가 파티원이면
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// 습득 가능
			}
			// 개인 아이템이면
			else if(pItem->dwOwnerIndex)
			{
				// 아이템 소유자가 아니면
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// 습득 불가
					return;	
				}
			}
														
			if(GetItemType(pItem->Item.GetID())==ITEM_TYPE_MONEY)
			{
				__int64 nValueMon;
				__int64 nValueItemMon;

				DWORD dwMoney = pItem->Item.m_Item_Money.dwMoney;

				if(pUser->GetPartyID() && wMemberCount > 1)
				{
					// 파티플의 경우는 카르츠를 공유 : 최덕석 2005.1.26
					memcpy( &PickupItem.Item, &pItem->Item, sizeof( CItem ) );

					PickupItem.Item.m_Item_Money.dwMoney /= wMemberCount;

					for(int m=0;m<wMemberCount;m++)
					{
						// 다른 멤버들에게 돈을 지급

						if(pMember[m] == pUser)
							continue;

						nValueMon		= pMember[m]->m_dwMoney;
						nValueItemMon	= PickupItem.Item.m_Item_Money.dwMoney;

						if(nValueMon+nValueItemMon>__MAX_MONEY__)
						{
							pMember[m]->m_dwMoney	= __MAX_MONEY__;
							PickupItem.dwMoney	= __MAX_MONEY__;					
							PickupItem.bZipCode	= 1;
						}
						else
						{
							pMember[m]->m_dwMoney += nValueItemMon;
							PickupItem.dwMoney	= pMember[m]->m_dwMoney;					
							PickupItem.bZipCode	= 0;
						}

						NetSendToUser( pMember[m]->GetConnectionIndex(), (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

						// 아이템 로그 추가 김영대 2005.02.23
						SetItemLogKarz(ITEM_LOG_KARZ_PICKUP,pLayer->GetParent()->GetID(),
										pMember[m]->m_szName,NULL,PickupItem.Item.m_Item_Money.dwMoney);

						dwMoney -= PickupItem.Item.m_Item_Money.dwMoney;
					}
				}

				// 나머지 돈은 모두 토글러에게
				PickupItem.Item.m_Item_Money.dwMoney = dwMoney;
				pItem->Item.m_Item_Money.dwMoney = dwMoney;

				nValueMon		= pUser->m_dwMoney;
				nValueItemMon	= PickupItem.Item.m_Item_Money.dwMoney;

				if(nValueMon+nValueItemMon>__MAX_MONEY__)
				{
					pUser->m_dwMoney = __MAX_MONEY__;
					PickupItem.dwMoney	= __MAX_MONEY__;					
					PickupItem.bZipCode	= 1;
				}
				else
				{
					pUser->m_dwMoney += nValueItemMon;
					PickupItem.dwMoney	= pUser->m_dwMoney;					
					PickupItem.bZipCode	= 0;
				}

				// 아이템 로그 추가 김영대 2005.02.23
				//SetItemLogKarz(ITEM_LOG_KARZ_PICKUP,pLayer->GetParent()->GetID(),
				//				pUser->m_szName,NULL,PickupItem.Item.m_Item_Money.dwMoney);
				// 아래에 로그처리를 따로 함.

			}
			else if(GetItemType(pItem->Item.GetID())==ITEM_TYPE_LARGE)
			{
				//파티플 랜덤루팅 분배
				if(pUser->GetPartyID()
				&& wMemberCount > 1
				&& pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
				{
                    pUser = pMember[ rand() % wMemberCount ];

					// 큰 인벤토리 칸에 대해
					for(int zip=0;zip<MAX_INV_LARGE_POOL;zip++)
					{
						// 비었으면
						if(pUser->m_pInv_Large[zip].m_wItemID==0)
							break;
					}

					// 인벤이 가득 차면
					if(zip == MAX_INV_LARGE_POOL)
					{
						// 줍지 못하고 다른 사람에게 기회가 간다
						return;
					}

					pItemPickup->bZipCode = zip;
				}

#if defined TAIWAN_LOCALIZING
				// 대만은 무게 제한 100% 이상시 아이템을 집을수 없게한다.
				if (FALSE == pUser->IsPickUpItem())
					return;					
#endif

				if(pUser->m_pInv_Large[ pItemPickup->bZipCode ].m_wItemID==0)
				{					
					PickupItem.bZipCode = pItemPickup->bZipCode;
					memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pItem->Item, sizeof(CItem));

					// 아이템 로그 추가 김영대 2005.02.23
					SetItemLog(&PickupItem.Item,ITEM_LOG_PICKUP	, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				}
				else
					return;
			}
			goto lb_exit1;
		}
		break;

	case 2:		// 바닥에서 작은 아이템으로 이동할 때.
		{
			if(!pLayer->GetSection( pItemPickup->bSectionNum))
				return;

			pItem = pLayer->GetSection(pItemPickup->bSectionNum)->m_pItemHash->GetData(pItemPickup->i64ItemID);
			
			if(!pItem)
				return;

			if(SetIsCopyItem(pUser, &PickupItem, &pItem->Item, 2, DEF_COPYITEM_TYPE_SMALL))
				goto lb_exit1;

			if(!CheckItem(&pItem->Item, ITEM_TYPE_SMALL))
				return;
			
			if(pItem->Item.GetQuantity()==0)
				return;

			// 파티풀 드롭 아이템이고 습득자가 파티원이면
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// 습득 가능
			}
			// 개인 아이템이면
			else if(pItem->dwOwnerIndex)
			{
				// 아이템 소유자가 아니면
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// 습득 불가
					return;	
				}
			}

			// 파티플 렌덤 루팅 : 최덕석 2005.1.26
			if(pUser->GetPartyID() && wMemberCount > 1 && pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
			{
				pUser = pMember[ rand() % wMemberCount ];

				// 투두 : 다른 유저들에게 메세지를 날려줌
				// "누가 뭘 몇개 먹었습니다."
			}

			//파티플 랜덤루팅 분배
			if(pUser->GetPartyID()
			&& wMemberCount > 1
			&& pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
			{
				pUser = pMember[ rand() % wMemberCount ];

				// 한칸에 최대로 넣을 수 있는 수를 구한다
				CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->Item.m_wItemID);
				if(!pBaseItem)
					return;
				int nMaxQuantity = pBaseItem->GetMaxQuantity();

				// 작은 인벤토리 칸에 대해
				for(int zip=0;zip<MAX_INV_SMALL_POOL;zip++)
				{
					// 넣을 수 있으면
					if(pUser->m_pInv_Small[zip].m_wItemID==0
					|| (pUser->m_pBelt[zip].m_wItemID == pItem->Item.m_wItemID
						&& pUser->m_pInv_Small[zip].GetQuantity() + pItem->Item.GetQuantity() < nMaxQuantity))
					{
						break;
					}
				}

				// 인벤이 가득차면
				if(zip == MAX_INV_SMALL_POOL)
				{
					// 줍지 못하고 다른 사람에게 기회가 간다
					return;
				}

				pItemPickup->bZipCode = zip;
			}

#if defined TAIWAN_LOCALIZING
			// 대만은 무게 제한 100% 이상시 아이템을 집을수 없게한다.
			if (FALSE == pUser->IsPickUpItem()) 	
				return;
#endif

			BYTE byCnt = 
				(BYTE)Insert_SmallInvItem(pUser, &pItem->Item, pItemPickup->bZipCode, pItem->Item.GetQuantity(), FALSE);

			if(byCnt>0)
			{
				pItem->Item.SetQuantity(byCnt);
				bAppear = FALSE;
			}
			
			CoypPickupMsg(&PickupItem, &pItem->Item, 2, pItemPickup->bZipCode);	

			// 아이템 로그 추가 김영대 2005.02.23
			SetItemLog(&PickupItem.Item,ITEM_LOG_PICKUP	, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			goto lb_exit1;
		}
		break;
	case 3:		// 마우스에서 벨트로 작은 아이템을 이동할 때 //
		{	
			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;			

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 3, DEF_COPYITEM_TYPE_SMALL))
				goto lb_exit2;
						
			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
							
			PickupItem.bZipCode	= pItemPickup->bZipCode;
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

			Insert_SmallBeltItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, pUser->m_ItemMouse.GetQuantity(),  FALSE);
		
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 4:		// 마우스에서 바닥으로 작은 아이템을 이동할 때 //
		{
			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;
						
			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;

			//sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 드롭..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

				PickupItem.bInv		= 14;
				PickupItem.bZipCode	= 0;
								
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
			
			AppearItem(pUser, &pUser->m_ItemMouse, pUser->m_ItemMouse.GetQuantity(), 0, ITEM_TYPE_SMALL);

			PickupItem.bEquipCode	= 0;			
			PickupItem.bZipCode		= 0;
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 5:		// 바닥에서 마우스로 이동할 때 //
		{
			if(!pLayer->GetSection( pItemPickup->bSectionNum ))
				return;

			pItem = pLayer->GetSection( pItemPickup->bSectionNum )->m_pItemHash->GetData(pItemPickup->i64ItemID);
			
			if(!pItem)
				return;

#if defined TAIWAN_LOCALIZING
				// 대만은 무게 제한 100% 이상시 아이템을 집을수 없게한다.				
				if (FALSE == pUser->IsPickUpItem())
					return;
#endif

			// 파티플 드롭 아이템고 파티 설정이 렌덤분배면
			if(pItem->dwPartyIndex && pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
			{
				// 아이템 드레그 불가
				return;
			}

			// 파티풀 드롭 아이템이고 습득자가 파티원이면
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// 습득 가능
			}
			// 개인 아이템이면
			else if(pItem->dwOwnerIndex)
			{
				// 아이템 소유자가 아니면
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// 습득 불가
					return;	
				}
			}

			PickupItem.bZipCode	= 0;
			memcpy( &pUser->m_ItemMouse, &pItem->Item, sizeof( CItem ) );
			goto lb_exit1;
		}
		break;			
	case 6:		// 마우스에서 장비 아이템으로 이동할 때 //
		{

			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 6, DEF_COPYITEM_TYPE_LARGE))
				goto lb_exit2;
						
			if(pUser->m_pInv_Large[ pItemPickup->bZipCode ].m_wItemID==0)
			{				
				PickupItem.bZipCode = pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}			
		}
		break;		
	case 7:		// 장비 아이템에서 마우스로 이동할 때 //
		{
			if(!CheckItem(&pUser->m_pInv_Large[pItemPickup->bZipCode], ITEM_TYPE_LARGE))
				return;
			
			if(pUser->m_ItemMouse.m_wItemID==0)
			{				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pInv_Large[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 8: // 공간이 비어있을경우.
		{
			if(pUser->GetUnitStatus()!=UNIT_STATUS_NORMAL)
				return;					

			int nVal = GetItemIndex(&pUser->m_ItemMouse);

			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;			

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 8, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;

			// 체크 클래스 //
			CBaseItem * pItem = GetBaseItem(pUser->m_ItemMouse.GetID());
			if(! pItem ) return;


			// 가방 아이템 추가 
			// 착용레벨이 되지 않으면 가방을 착용할수 없다.
			// 2005.01.20 by 김영대 
			if ( (pItem->wID >= BAG_ITEM_NUM_FRIST && pItem->wID <= BAG_ITEM_NUM_LAST) && 
				 ( pUser->GetLevel() <  pItem->BaseItem_Bag.wMin_Lev  ) )
			{
				
				PickupItem.bInv        = pItemPickup->bInv;
				PickupItem.dwMoney     =  3;					
				PickupItem.bZipCode		= pItemPickup->bZipCode;					
				PickupItem.dwId			= pItemPickup->dwId;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				goto lb_exit2;
			}

			BOOL bChk	= FALSE;
			BOOL bClass = pUser->CheckItem(&pUser->m_ItemMouse);
							
			if(!bClass)
			{
				for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[i].GetID()==0)
					{
						PickupItem.bZipCode = i;
						bChk = TRUE;
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						break;
					}
				}
				if(bChk)
				{
					PickupItem.dwMoney	= 1;					
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				}
				else
				{
					PickupItem.dwMoney	= 2;					
					AppearItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), ITEM_TYPE_LARGE);
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));						
				}						
				goto lb_exit2;
			}

			// Check //				
			if(nVal>=0 && nVal<=7)
			{					
				if((nVal>=2 && nVal<=5) || nVal==7)
				{
					// 투핸드 //
					PickupItem.bEquipCode	= pItemPickup->bEquipCode;					
					PickupItem.bZipCode		= 0;						
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));														

					if(PickupItem.bEquipCode==EQUIP_TYPE_RHAND1)
					{								
						if(pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID!=0)
						{
							BOOL bChk = FALSE;

							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].m_wItemID==0)
								{
									PickupItem.bZipCode	= i;
									bChk = TRUE;
									break;
								}
							}

							if(bChk==TRUE)
							{
								memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[EQUIP_TYPE_LHAND1], sizeof(CItem));
								memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
							}
							else
							{
								AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, 0, ITEM_TYPE_LARGE);
								memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
							}
						}
					}
					else if(PickupItem.bEquipCode==EQUIP_TYPE_RHAND2)
					{
						if(pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID!=0)
						{
							BOOL bChk = FALSE;

							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].m_wItemID==0)
								{
									PickupItem.bZipCode	= i;
									bChk = TRUE;
									break;
								}
							}

							if(bChk==TRUE)
							{
								memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[EQUIP_TYPE_LHAND2], sizeof(CItem));
								memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));										
							}
							else
							{
								AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, 0, ITEM_TYPE_LARGE);
								memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));
							}
						}
					}
				}
				else
				{
					// 원핸드 //
					PickupItem.bEquipCode	= pItemPickup->bEquipCode;					
					PickupItem.bZipCode		= 0;					
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
					memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				}
			}
			else if(nVal==12)
			{
				PickupItem.bEquipCode	= pItemPickup->bEquipCode;				
				PickupItem.bZipCode		= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(PickupItem.bEquipCode==EQUIP_TYPE_LHAND1)
				{								
					if(pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID!=0)
					{
						int nVal = pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID/ITEM_DISTRIBUTE;

						if((nVal>=2 && nVal<=5) || nVal==7)
						{
							BOOL bChk = FALSE;

							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].m_wItemID==0)
								{
									PickupItem.bZipCode	= i;
									bChk = TRUE;
									break;
								}
							}

							if(bChk==TRUE)
							{
								memcpy(	&pUser->m_pInv_Large[PickupItem.bZipCode],&pUser->m_pEquip[EQUIP_TYPE_RHAND1], sizeof(CItem));
								memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
							}
							else
							{
								AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, 0, ITEM_TYPE_LARGE);
								memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
							}
						}
					}
				}
				else if(PickupItem.bEquipCode==EQUIP_TYPE_LHAND2)
				{
					if(pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID!=0)
					{
						int nVal = pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID/ITEM_DISTRIBUTE;

						if((nVal>=2 && nVal<=5) || nVal==7)
						{
							BOOL bChk = FALSE;

							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].m_wItemID==0)
								{
									PickupItem.bZipCode	= i;
									bChk = TRUE;
									break;
								}
							}

							if(bChk==TRUE)
							{
								memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[EQUIP_TYPE_RHAND2], sizeof(CItem));
								memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));										
							}
							else
							{
								AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, 0, ITEM_TYPE_LARGE);
								memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));
							}
						}
					}
				}
			}
			else
			{
				if(!CheckEquip((BYTE)nVal, pItemPickup->bEquipCode))
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[ i ].m_wItemID==0)
						{							
							PickupItem.bInv		= 6;
							PickupItem.bZipCode = i;
							memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
							goto lb_exit2;
						}
					}
					return;
				}

				PickupItem.bEquipCode	= pItemPickup->bEquipCode;				
				PickupItem.bZipCode		= 0;				
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			}
			
			// 착용한 아이템을 계산하고 통보.
			pUser->SendAllStatus();

			// Mod 교체 메세지 //
			if( ((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE) >= 0 && (PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE) <=7 )
				|| PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE==12)
			{
				if(pUser->m_bCurrentHand==1)
					ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
				else
					ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
			}
			else if((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)==11)
			{
				ItemArmorMod(pUser, pUser->m_pEquip[EQUIP_TYPE_MAIL].m_wItemID);
			}
			else if((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)==10)
			{
				ItemHelmetMod(pUser, pUser->m_pEquip[EQUIP_TYPE_HELMET].m_wItemID);
			}

			goto lb_exit2;
		}
		break;
	case 9:	// 아이템이 있을경우.
		{
			if(pUser->GetUnitStatus()!=UNIT_STATUS_NORMAL)
				return;

			else if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			else if((pUser->m_ItemMouse.m_wItemID/ITEM_DISTRIBUTE)>MAX_LARGE_ITEM_INDEX)
				return;
						
			else if(GetItemType(pUser->m_ItemMouse.GetID())!=ITEM_TYPE_LARGE)
				return;

			else if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 9, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;


			// 창고에서 장착으로 이동시 아이템이 땅에 떨어지는것을 방지 
			// 2005.01.25 김영대 
			BOOL bChk = FALSE;
			
			for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
			{
				if(pUser->m_pInv_Large[i].GetID()==0)
				{
					bChk = TRUE;
					break;
				}
			}

			if ( bChk == FALSE)
			{
				PickupItem.bInv        = pItemPickup->bInv;
				PickupItem.dwMoney     =  4;					
				PickupItem.bZipCode		= pItemPickup->bZipCode;					
				PickupItem.dwId			= pItemPickup->dwId;
				PickupItem.bEquipCode	= 2;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				goto lb_exit2;
			}


			PickupItem.dwMoney	= 0;

			if(pUser->m_ItemMouse.m_wItemID!=0)
			{	
				// 체크 클래스 //
				BOOL bChk = FALSE;

				if(!GetBaseItem(pUser->m_ItemMouse.GetID()))
					return;

				BOOL bClass = pUser->CheckItem(&pUser->m_ItemMouse);					
				
				if(!bClass)
				{						
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].GetID()==0)
						{
							PickupItem.bZipCode = i;
							bChk = TRUE;
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							break;
						}
					}

					if(bChk)
					{
						PickupItem.dwMoney		= 1;						
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}
					else
					{
						PickupItem.dwMoney		= 2;						
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						AppearItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), ITEM_TYPE_LARGE);
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));							
					}						
					goto lb_exit2;
				}

				int nVal = pUser->m_ItemMouse.m_wItemID/ITEM_DISTRIBUTE;
									
				if(nVal>=0 && nVal<=7)
				{
					if((nVal>=2 && nVal<=5) || nVal==7)
					{						
						PickupItem.bEquipCode	= pItemPickup->bEquipCode;						
						PickupItem.bZipCode		= 0;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

						BOOL bChk = FALSE;

						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								PickupItem.bZipCode	= i;
								bChk = TRUE;
								break;
							}
						}

						if(bChk==TRUE)
						{
							memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));								
							memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						}
						else
						{
							AppearItem(pUser, &pUser->m_pEquip[PickupItem.bEquipCode], 0, 0, ITEM_TYPE_LARGE);
							// Item //												
							memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						}

						// 옆에 아이템 //
						if(PickupItem.bEquipCode==EQUIP_TYPE_RHAND1)
						{								
							if(pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID!=0)
							{
								BOOL bChk = FALSE;

								for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(pUser->m_pInv_Large[i].m_wItemID==0)
									{
										PickupItem.bZipCode2	= i;
										bChk = TRUE;
										break;
									}
								}

								if(bChk==TRUE)
								{
									memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode2], &pUser->m_pEquip[EQUIP_TYPE_LHAND1], sizeof(CItem));
									memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
								}
								else
								{
									AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, 0, ITEM_TYPE_LARGE);
									// Item //																	
									memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND1], 0, sizeof(CItem));
								}
							}
						}
						else if(PickupItem.bEquipCode==EQUIP_TYPE_RHAND2)
						{
							if(pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID!=0)
							{
								BOOL bChk = FALSE;

								for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(pUser->m_pInv_Large[i].m_wItemID==0)
									{
										PickupItem.bZipCode2	= i;
										bChk = TRUE;
										break;
									}
								}

								if(bChk==TRUE)
								{
									memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode2], &pUser->m_pEquip[EQUIP_TYPE_LHAND2], sizeof(CItem));
									memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));										
								}
								else
								{
									AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, 0, ITEM_TYPE_LARGE);
									// Item //																	
									memset(&pUser->m_pEquip[EQUIP_TYPE_LHAND2], 0, sizeof(CItem));
								}
							}
						}
					}
					else
					{
						PickupItem.bEquipCode	= pItemPickup->bEquipCode;						
						PickupItem.bZipCode		= 0;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

						BOOL bChk = FALSE;

						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								PickupItem.bZipCode	= i;
								bChk = TRUE;
								break;
							}
						}

						if(bChk==TRUE)
						{
							memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));								
							memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						}
						else
						{
							return;
							AppearItem(pUser, &pUser->m_pEquip[PickupItem.bEquipCode], 0, 0, ITEM_TYPE_LARGE);
							// Item //												
							memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						}
					}
				}
				else if(nVal==12)
				{
					PickupItem.bEquipCode	= pItemPickup->bEquipCode;					
					PickupItem.bZipCode		= 0;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					BOOL bChk = FALSE;

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							PickupItem.bZipCode	= i;
							bChk = TRUE;
							break;
						}
					}

					if(bChk==TRUE)
					{
						memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));
						memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}
					else
					{
						AppearItem(pUser, &pUser->m_pEquip[PickupItem.bEquipCode], 0, 0, ITEM_TYPE_LARGE);
						// Item //												
						memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}

					// 옆에 아이템 //
					if(PickupItem.bEquipCode==EQUIP_TYPE_LHAND1)
					{								
						if(pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID!=0)
						{
							int nVal = pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID/ITEM_DISTRIBUTE;

							if((nVal>=2 && nVal<=5) || nVal==7)
							{
								BOOL bChk = FALSE;

								for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(pUser->m_pInv_Large[i].m_wItemID==0)
									{
										PickupItem.bZipCode2	= i;
										bChk = TRUE;
										break;
									}
								}

								if(bChk==TRUE)
								{
									memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode2], &pUser->m_pEquip[EQUIP_TYPE_RHAND1], sizeof(CItem));
									memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
								}
								else
								{
									AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, 0, ITEM_TYPE_LARGE);
									// Item //																	
									memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND1], 0, sizeof(CItem));
								}
							}
						}
					}
					else if(PickupItem.bEquipCode==EQUIP_TYPE_LHAND2)
					{
						if(pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID!=0)
						{
							int nVal = pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID/ITEM_DISTRIBUTE;

							if((nVal>=2 && nVal<=5) || nVal==7)
							{
								BOOL bChk = FALSE;

								for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
								{
									if(pUser->m_pInv_Large[i].m_wItemID==0)
									{
										PickupItem.bZipCode2	= i;
										bChk = TRUE;
										break;
									}
								}

								if(bChk==TRUE)
								{
									memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode2], &pUser->m_pEquip[EQUIP_TYPE_RHAND2], sizeof(CItem));
									memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));
								}
								else
								{
									AppearItem(pUser, &pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, 0, ITEM_TYPE_LARGE);
									// Item //																	
									memset(&pUser->m_pEquip[EQUIP_TYPE_RHAND2], 0, sizeof(CItem));
								}
							}
						}
					}						
				}
				else
				{
					if(!CheckEquip((BYTE)nVal, pItemPickup->bEquipCode))
					{
						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[ i ].m_wItemID==0)
							{							
								PickupItem.bInv		= 6;
								PickupItem.bZipCode = i;
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
								goto lb_exit2;
							}
						}
						return;
					}

					BOOL bChk = FALSE;						

					PickupItem.bEquipCode	= pItemPickup->bEquipCode;					
					PickupItem.bZipCode		= 0;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
					
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							PickupItem.bZipCode	= i;
							bChk = TRUE;
							break;
						}
					}

					if(bChk==TRUE)
					{
						memcpy(&pUser->m_pInv_Large[PickupItem.bZipCode], &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));
						memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}
					else
					{
						AppearItem(pUser, &pUser->m_pEquip[PickupItem.bEquipCode], 0, 0, ITEM_TYPE_LARGE);
						memcpy(&pUser->m_pEquip[PickupItem.bEquipCode], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					}
				}
				
				// 착용한 아이템을 계산하고 통보.
				pUser->SendAllStatus();
			}

			// Mod 교체 메세지 //
			if(((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)>=0 && (PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)<=7) ||
				PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE==12)
			{
				if(pUser->m_bCurrentHand==1)
				{
					ItemHandMod(pUser,
						pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
				}
				else
				{
					ItemHandMod(pUser,
						pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
				}
			}
			else if((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)==11)
			{
				ItemArmorMod(pUser, pUser->m_pEquip[EQUIP_TYPE_MAIL].m_wItemID);
			}
			else if((PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE)==10)
			{
				ItemHelmetMod(pUser, pUser->m_pEquip[EQUIP_TYPE_HELMET].m_wItemID);
			}

			goto lb_exit2;
		}
		break;
	case 10:		// 아이템을 바닥으로 이동시킬 때.
		{						
			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;

			//sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 드롭..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[ i ].m_wItemID==0)
					{
						PickupItem.bInv		= 6;
						PickupItem.bZipCode = i;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}
				}
				return;
			}
			
			memcpy(&ItemLog, &pUser->m_ItemMouse, sizeof(CItem));

			AppearItem(pUser
				, &pUser->m_ItemMouse
				, 0
				, 0
				, 0
				, pItemPickup->fItemX
				, 0
				, pItemPickup->fItemZ);
			
			PickupItem.bEquipCode	= 0;			
			PickupItem.bZipCode		= 0;
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;			
		}
		break;
	case 11:		// 아이템장착 슬롯에서 마우스로 이동.
		{
			int nValue = GetItemIndex(&pUser->m_pEquip[pItemPickup->bEquipCode]);
								
			if(!CheckItem(&pUser->m_pEquip[pItemPickup->bEquipCode], ITEM_TYPE_LARGE))
				return;
						
			else if(pUser->m_ItemMouse.m_wItemID!=0)
				return;			
			else if(pUser->GetUnitStatus()!=UNIT_STATUS_NORMAL)
				return;

			PickupItem.bEquipCode	= pItemPickup->bEquipCode;			
			PickupItem.bZipCode		= 0;

							
			memcpy(&PickupItem.Item, &pUser->m_pEquip[pItemPickup->bEquipCode], sizeof(CItem));
			memcpy(&pUser->m_ItemMouse, &pUser->m_pEquip[PickupItem.bEquipCode], sizeof(CItem));
			memset(&pUser->m_pEquip[PickupItem.bEquipCode], 0, sizeof(CItem));
			
			// 착용한 아이템을 계산하고 통보.
			pUser->SendAllStatus();
			// Mod 교체 메세지 //
			nValue = PickupItem.Item.m_wItemID/ITEM_DISTRIBUTE;

			if((nValue>=0 && nValue<=7) || nValue==12)
			{
				if(pUser->m_bCurrentHand==1)
					ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
				else
					ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
			}
			else if(nValue==11)
				ItemArmorMod(pUser, pUser->m_pEquip[EQUIP_TYPE_MAIL].m_wItemID);
			else if(nValue==10)
				ItemHelmetMod(pUser, pUser->m_pEquip[EQUIP_TYPE_HELMET].m_wItemID);


			goto lb_exit2;			
		}
		break;
	case 12:		// 작은 아이템을 바닥에서 마우스로 이동할 때.
		{
			if(!pLayer->GetSection(pItemPickup->bSectionNum))
				return;

			pItem = pLayer->GetSection(pItemPickup->bSectionNum)->m_pItemHash->GetData(pItemPickup->i64ItemID);

			if(!pItem)
				return;
			
			// 파티플 드롭 아이템이고 파티 설정이 렌덤분배이면
			if(pItem->dwPartyIndex && pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
			{
				// 마우스 드래그 불가
				return;
			}
			
			// 파티풀 드롭 아이템이고 습득자가 파티원이면
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// 습득 가능
			}
			// 개인 아이템이면
			else if(pItem->dwOwnerIndex)
			{
				// 아이템 소유자가 아니면
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// 습득 불가
					return;	
				}
			}
			
//			int nValue = GetItemIndex(&pItem->Item);

			if(!CheckItem(&pItem->Item, ITEM_TYPE_SMALL))
				return;
			
			if(pItem->Item.GetQuantity()==0)
				return;
							
			PickupItem.bZipCode	= 0;					
			memcpy(&PickupItem.Item, &pItem->Item, sizeof(CItem));
			memcpy(&pUser->m_ItemMouse, &pItem->Item, sizeof(CItem));
			goto lb_exit1;			
		}
		break;
	case 13:		// 돈을 버릴 때.
		{


#ifdef JAPAN_LOCALIZING		//넷클루 요청으로 일본은 돈을 떨어뜨리지 않는다 .. 2005.02.05 김영대

			PickupItem.dwMoney						=	pUser->m_dwMoney;
			PickupItem.Item.m_wItemID				=	__ITEM_KARZ__;
			PickupItem.Item.m_Item_Money.dwMoney	=	pItemPickup->dwMoney;
			goto lb_exit2;
#else
			if(pUser->m_dwMoney<pItemPickup->dwMoney)
				return;

			AppearItem(pUser, NULL, 0, pItemPickup->dwMoney, ITEM_TYPE_MONEY);		
							
			pUser->m_dwMoney						-=	pItemPickup->dwMoney;			
			PickupItem.dwMoney						=	pUser->m_dwMoney;
			PickupItem.Item.m_wItemID				=	__ITEM_KARZ__;
			PickupItem.Item.m_Item_Money.dwMoney	=	pItemPickup->dwMoney;


			goto lb_exit2;
#endif
		}
		break;
	case 14:		// 마우스에서 소비 창으로 작은 아이템을 이동할 때.
		{	
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);


			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;
			
			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 14, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;				
			
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

			Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, pUser->m_ItemMouse.GetQuantity(), TRUE);
			
			PickupItem.bZipCode	= pItemPickup->bZipCode;
							
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;			
		}
		break;
	case 15:		// 소비 창에서 마우스로 작은 아이템을 이동할 때.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(!CheckItem(&pUser->m_pInv_Small[pItemPickup->bZipCode], ITEM_TYPE_SMALL))
				return;
			
			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;
			
			if(nValue>=ITEM_CONSUMABLEST_INDEX && nValue<=ITEM_CONSUMABLEEND_INDEX)
			{
				if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_Item_Consumable.bInvIndex>=1)
					return;
			}	
			
			// 아이템 보험 적용 되어있는지?? 적용되어 있으면 마우스로 집어지지 마라. 
			if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_wItemID == __ITEM_INSURANCE_INDEX__)
			{
				if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_Item_Zodiac.bInvIndex >=1 )
					return;
			}

			if(pUser->m_ItemMouse.m_wItemID == 0)
			{				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 16:		// 소비 아이템을 벨트에서 마우스로 이동할 때.
		{
//			int nValue = GetItemIndex(&pUser->m_pBelt[pItemPickup->bZipCode]);
			
			if(!CheckItem(&pUser->m_pBelt[pItemPickup->bZipCode], ITEM_TYPE_SMALL))
				return;
			
			if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==0)
				return;
			
			if(pUser->m_ItemMouse.m_wItemID==0)
			{				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_pBelt[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pBelt[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pBelt[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 17:		// Switch Weapon 메세지.
		{							
			if(pUser->m_bCurrentHand==1)// LEFT
			{
				pUser->m_bCurrentHand	= 2;
				PickupItem.bZipCode		= 2;
			}
			else	// RIGHT
			{
				pUser->m_bCurrentHand	= 1;
				PickupItem.bZipCode		= 1;
			}					
			
			// 착용한 아이템을 계산하고 통보.
			pUser->SendAllStatus();
																	
			// Mod 교체 메세지 //
			if(pUser->m_bCurrentHand==1)
			{
				ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID
					, pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID);
			}
			else
			{
				ItemHandMod(pUser, pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID
					, pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID);
			}
							
			goto lb_exit2;
		}
		break;
	case 18:		// 아이템 상점에 아이템을 팔 때.(소모품)
		{	
			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;			

			else if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 18, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;
						
			CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);
			
			if(!pBaseItem)
				return;
			
			__int64 nValueMon		= pUser->m_dwMoney;
			__int64 nValuePackMon	= pItemPickup->dwMoney;
			
			PickupItem.bEquipCode		= 1;
			PickupItem.dwId				= pUser->m_ItemMouse.m_wItemID;
			PickupItem.Item.m_wItemID	= pUser->m_ItemMouse.m_wItemID;
			PickupItem.bZipCode			= 0;
								
			__int64 nValuePri = pBaseItem->GetSellPrice();
										
			if(nValueMon+(nValuePri*nValuePackMon)>__MAX_MONEY__)
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, pUser->m_ItemMouse.GetQuantity(), TRUE);

				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}								

			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

			if(pUser->m_ItemMouse.GetQuantity()!=pItemPickup->dwMoney)
			{
				if(pUser->m_ItemMouse.GetQuantity()<pItemPickup->dwMoney)
					return;

				BYTE byPackCnt = BYTE(pUser->m_ItemMouse.GetQuantity()-pItemPickup->dwMoney);

				pUser->m_ItemMouse.SetQuantity(byPackCnt);
				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, byPackCnt, TRUE);					
			}																
			
			pUser->m_dwMoney +=	pBaseItem->GetSellPrice()*pItemPickup->dwMoney;					
			
			PickupItem.bEquipCode	=	0;
			PickupItem.dwMoney		=	pUser->m_dwMoney;					
			PickupItem.bZipCode		=	pItemPickup->bZipCode;
			PickupItem.bZipCode2	=	BYTE(pItemPickup->dwMoney);
			
			memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 19:		// 아이템 상점에 아이템을 살 때.
		{
			CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItemPickup->dwId);

			if(!pBaseItem)
				break;

			if(GetItemType(WORD(pItemPickup->dwId))==ITEM_TYPE_LARGE)
				return;				

			if(!CheckStore(pItemPickup->dwId))
				return;
			
//			BOOL		bChk		= FALSE;				
//			int			nValue		= pBaseItem->wID/ITEM_DISTRIBUTE;
			__int64		nMonValue	= 0;
			
			nMonValue = pItemPickup->dwMoney * pBaseItem->GetPrice();				

			if((pUser->m_dwMoney>=nMonValue) && pItemPickup->dwMoney!=0)
			{
				CItem cItem;
				CreateItem(&cItem, WORD(pItemPickup->dwId), BYTE(pItemPickup->dwMoney));

				BYTE byPackCnt = (BYTE)Insert_SmallInvItem(pUser, &cItem, pItemPickup->bZipCode, cItem.GetQuantity(), TRUE, FALSE);
				
				memcpy(&PickupItem.Item, &cItem, sizeof(CItem));
				PickupItem.Item.SetQuantity(BYTE(pItemPickup->dwMoney-byPackCnt));
								
				PickupItem.bZipCode		= pItemPickup->bZipCode;
				PickupItem.dwId			= pItemPickup->dwId;				
				PickupItem.bEquipCode	= BYTE(pItemPickup->dwMoney);
				PickupItem.bZipCode2	= 0;

				if(byPackCnt!=0)
				{
					pUser->m_dwMoney	-=	pBaseItem->GetPrice()*(pItemPickup->dwMoney-byPackCnt);												
					PickupItem.dwMoney	=	pUser->m_dwMoney;
				}
				else
				{
					pUser->m_dwMoney	-=	pBaseItem->GetPrice()*pItemPickup->dwMoney;												
					PickupItem.dwMoney	=	pUser->m_dwMoney;
				}					
			}
			else
			{				
				PickupItem.bZipCode2	= 1;
				PickupItem.dwId			= pItemPickup->dwId;
			}
			goto lb_exit2;
		}
		break;		
	case 24:		// 마우스에서 창고로(장비 아이템).
		{
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);


			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;
			
			if(pUser->m_pBank_Large[pItemPickup->bZipCode].GetID()!=0)
				return;
						
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 24, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;

			//sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 창고..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() == 2 )
			{
				for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[ i ].m_wItemID==0)
					{
						PickupItem.bInv		= 6;
						PickupItem.bZipCode = i;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}
				}
			}
			
			PickupItem.bZipCode	=	pItemPickup->bZipCode;
			memcpy(&pUser->m_pBank_Large[pItemPickup->bZipCode], &pUser->m_ItemMouse, sizeof(CItem));
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

			// 아이템 로그 추가 김영대 2005.02.23
			SetItemLog(&pUser->m_pBank_Large[pItemPickup->bZipCode],ITEM_LOG_BANK_IN, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			goto lb_exit2;
		}
		break;
	case 25:		// 마우스에서 창고로(소비 아이템).
		{
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);


			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_SMALL))
				return;
						
			if(pItemPickup->bZipCode>=MAX_BANK_SMALL_POOL)
				return;

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 25, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;

			//sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 창고..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() == 2 )
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

				PickupItem.bInv		= 14;
				PickupItem.bZipCode	= 0;
								
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
			
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
							
			if(pUser->m_ItemMouse.GetQuantity() >= pItemPickup->dwMoney)
			{
				BYTE	byPacketCount	= BYTE(pItemPickup->dwMoney);	
				BYTE	byOutCount		= BYTE(pUser->m_ItemMouse.GetQuantity()-pItemPickup->dwMoney);
									
				if(byPacketCount == pUser->m_ItemMouse.GetQuantity())
				{
					Insert_SmallBankItem(pUser
						, &pUser->m_ItemMouse
						, pItemPickup->bZipCode
						, pUser->m_ItemMouse.GetQuantity());
				}
				else
				{
					CItem cBankItem;

					memcpy(&cBankItem, &pUser->m_ItemMouse, sizeof(CItem));
					cBankItem.SetQuantity(BYTE(pItemPickup->dwMoney));
					cBankItem.SetSerialNum(g_pThis->GetNewSerial());
					
					Insert_SmallBankItem(pUser, &cBankItem, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));
					
					if(byOutCount!=0)
					{
						pUser->m_ItemMouse.SetQuantity(byOutCount);
						Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, byOutCount, TRUE);
					}
				}																			
				PickupItem.dwMoney	=	pItemPickup->dwMoney;
				PickupItem.bZipCode	=	pItemPickup->bZipCode;					
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				// 아이템 로그 추가 김영대 2005.02.23
				SetItemLog(&PickupItem.Item,ITEM_LOG_BANK_IN, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

				goto lb_exit2;
			}				
		}
		break;
	case 26:		// 창고에서 마우스로(장비 아이템).
		{
			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
//			int nValue = GetItemIndex(&pUser->m_pBank_Large[pItemPickup->bZipCode]);

			if(!CheckItem(&pUser->m_pBank_Large[pItemPickup->bZipCode], ITEM_TYPE_LARGE))
				return;
			
			if(pUser->m_ItemMouse.GetID()!=0)
				return;
						
			PickupItem.bZipCode	=	pItemPickup->bZipCode;
			memcpy(&pUser->m_ItemMouse, &pUser->m_pBank_Large[pItemPickup->bZipCode], sizeof(CItem));
			memcpy(&PickupItem.Item, &pUser->m_pBank_Large[pItemPickup->bZipCode], sizeof(CItem));
			memset(&pUser->m_pBank_Large[pItemPickup->bZipCode], 0, sizeof(CItem));

			// 아이템 로그 추가 김영대 2005.02.23
			SetItemLog(&PickupItem.Item,ITEM_LOG_BANK_OUT, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			goto lb_exit2;
		}
		break;
	case 27:		// 창고에서 마우스로(소비 아이템).
		{
			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
//			int nValue = GetItemIndex(&pUser->m_pBank_Small[pItemPickup->bZipCode]);

			if(!CheckItem(&pUser->m_pBank_Small[pItemPickup->bZipCode], ITEM_TYPE_SMALL))
				return;
			
			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			PickupItem.bZipCode	=	pItemPickup->bZipCode;
			memcpy(&pUser->m_ItemMouse, &pUser->m_pBank_Small[pItemPickup->bZipCode], sizeof(CItem));				
			memcpy(&PickupItem.Item, &pUser->m_pBank_Small[pItemPickup->bZipCode], sizeof(CItem));
			memset(&pUser->m_pBank_Small[pItemPickup->bZipCode], 0, sizeof(CItem));

			// 아이템 로그 추가 김영대 2005.02.23
			SetItemLog(&PickupItem.Item,ITEM_LOG_BANK_OUT, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			goto lb_exit2;
		}
		break;
	case 28:		// 마우스에서 나누기로 바닥으로 떨어질 때(소비 아이템).
		{
//			BOOL	bChk		= FALSE;
//			int		nValue		= pUser->m_ItemMouse.m_wItemID/ITEM_DISTRIBUTE;
			BYTE	byPacketCnt	= BYTE(pUser->m_ItemMouse.GetQuantity() - pItemPickup->dwMoney);
			BYTE	byOutCnt	= BYTE(pItemPickup->dwMoney);

			if(GetItemType(pUser->m_ItemMouse.GetID())!=ITEM_TYPE_SMALL)
				return;

			//sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 드롭..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

				PickupItem.bInv		= 14;
				PickupItem.bZipCode	= 0;
								
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
						
			PickupItem.dwMoney	= pItemPickup->dwMoney;
			memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(byPacketCnt<0)
				return;

			if(pUser->m_ItemMouse.GetQuantity() < pItemPickup->dwMoney)
			{
				if(GetItemType(pUser->m_ItemMouse.m_wItemID)==ITEM_TYPE_SMALL)
				{
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);
					
					PickupItem.bZipCode	= 0;
									
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}
				return;
			}

			if(byPacketCnt==0)
			{
				pUser->m_ItemMouse.SetQuantity(byOutCnt);
				AppearItem(pUser, &pUser->m_ItemMouse, BYTE(pItemPickup->dwMoney), 0, 0, pItemPickup->fItemX, 0, pItemPickup->fItemZ);
				memcpy(&ItemLog, &pUser->m_ItemMouse, sizeof(CItem));	//kukza 041004
			}
			else
			{
				pUser->m_ItemMouse.SetQuantity(byPacketCnt);
				byPacketCnt = (BYTE)Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, byPacketCnt, TRUE, FALSE);
									
				if(byPacketCnt!=0)
					byOutCnt += byPacketCnt;
					
				if(byOutCnt!=0)
				{
					CItem cInsertITem;
					memcpy(&cInsertITem, &pUser->m_ItemMouse, sizeof(CItem));
					cInsertITem.SetSerialNum(g_pThis->GetNewSerial());
					cInsertITem.SetQuantity(byOutCnt);
					AppearItem(pUser, &cInsertITem, byOutCnt, 0, 0, pItemPickup->fItemX, 0, pItemPickup->fItemZ);
					memcpy(&ItemLog, &cInsertITem, sizeof(CItem));		//kukza 041004
				}
			}

			// 아이템 로그 추가 김영대 2005.02.23 아래에서 로그 남김
			//SetItemLog(&pUser->m_ItemMouse,ITEM_LOG_DROP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

			memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
			goto lb_exit2;
		}
		break;		
	case 30:// 길드 생성할때 
		{			
			if(pUser->m_byGroupType==__GCTYPE_NONE__)
			{	
				BOOL bChk				= FALSE;				
				PickupItem.bZipCode2	= pItemPickup->bZipCode2;

				// 아이템 인증 //	
				if(pItemPickup->bZipCode2==__GCTYPE_GUILD__)
				{
					if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_wItemID==ITEM_GUILD_INDEX)
					{
						BYTE bQuantity = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(bQuantity>1)
						{
							pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(bQuantity-1));
							PickupItem.bZipCode	= pItemPickup->bZipCode;
							memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						}
						else
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
							PickupItem.bZipCode	= pItemPickup->bZipCode;
							memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						}
						bChk = TRUE;
					}
					else
					{
						for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(pUser->m_pInv_Small[i].m_wItemID==ITEM_GUILD_INDEX)
							{
								BYTE bQuantity = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

								if(bQuantity>1)
								{
									pUser->m_pInv_Small[i].SetQuantity(BYTE(bQuantity-1));
									PickupItem.bZipCode	= i;
									memcpy(&PickupItem.Item, &pUser->m_pInv_Small[i], sizeof(CItem));
								}
								else
								{
									memset(&pUser->m_pInv_Small[i], 0, sizeof(CItem));
									PickupItem.bZipCode	= i;
									memcpy(&PickupItem.Item, &pUser->m_pInv_Small[i], sizeof(CItem));
								}
								bChk = TRUE;
								break;
							}
						}
					}
				}
				else if(pItemPickup->bZipCode2==__GCTYPE_CLEN__)
				{
					if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_wItemID==ITEM_CLEN_INDEX)
					{
						BYTE bQuantity = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(bQuantity>1)
						{
							pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(bQuantity-1));
							PickupItem.bZipCode	= pItemPickup->bZipCode;
							memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						}
						else
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
							PickupItem.bZipCode	= pItemPickup->bZipCode;
							memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						}
						bChk = TRUE;
					}
					else
					{
						for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(pUser->m_pInv_Small[i].m_wItemID==ITEM_CLEN_INDEX)
							{
								BYTE bQuantity = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

								if(bQuantity>1)
								{
									pUser->m_pInv_Small[i].SetQuantity(BYTE(bQuantity-1));
									PickupItem.bZipCode	= i;
									memcpy(&PickupItem.Item, &pUser->m_pInv_Small[i], sizeof(CItem));
								}
								else
								{
									memset(&pUser->m_pInv_Small[i], 0, sizeof(CItem));
									PickupItem.bZipCode	= i;
									memcpy(&PickupItem.Item, &pUser->m_pInv_Small[i], sizeof(CItem));
								}
								bChk = TRUE;
							}							
						}
					}
				}					
				
				if(bChk==TRUE)
				{											
					goto lb_exit2;
				}
				else

				{
					// 아이템 인증 실패 //
					pPacket.bResult		= FALSE;
					pPacket.bZipCode	= 3;
					NetSendToUser(dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
				}
			}
			else
			{					
				pPacket.bResult	= FALSE;

				if(pUser->m_byGroupType==__GCTYPE_GUILD__)
					pPacket.bZipCode = __GCTYPE_GUILD__;
				else
					pPacket.bZipCode = __GCTYPE_CLEN__;				

				NetSendToUser(dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
			}
		}
		break;
	case 32:		// 아이템에서 뱅크로 돈이동.
		{
			if(pItemPickup->dwMoney<=pUser->m_dwMoney)
			{
				__int64 nValueBankMon	= pUser->m_dwBankMoney;
				__int64 nValueMon		= pItemPickup->dwMoney;

				if(nValueBankMon+nValueMon>__MAX_MONEY__)
				{
					DWORD dwMoney = __MAX_MONEY__ - pUser->m_dwBankMoney;

					pUser->m_dwMoney		-= dwMoney;
					pUser->m_dwBankMoney	+= dwMoney;

					// 아이템 로그 추가 김영대 2005.02.23
					SetItemLogKarz(	ITEM_LOG_BANK_KARZ_IN,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,dwMoney);
				}
				else
				{
					pUser->m_dwMoney		-= pItemPickup->dwMoney;
					pUser->m_dwBankMoney	+= pItemPickup->dwMoney;
					// 아이템 로그 추가 김영대 2005.02.23
					SetItemLogKarz(	ITEM_LOG_BANK_KARZ_IN,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItemPickup->dwMoney);
				}	
				
				PickupItem.dwMoney	= pUser->m_dwMoney;
				PickupItem.dwId		= pUser->m_dwBankMoney;										
				goto lb_exit2;
			}				
		}
		break;
	case 33:		// 뱅크에서 아이템으로 돈이동.
		{
			if(pItemPickup->dwMoney<=pUser->m_dwBankMoney)
			{
				__int64 nValueMon		= pUser->m_dwMoney;
				__int64 nValuePackMon	= pItemPickup->dwMoney;

				if(nValueMon+nValuePackMon>__MAX_MONEY__)
				{
					DWORD dwMoney = __MAX_MONEY__ - pUser->m_dwMoney;

					pUser->m_dwMoney		+= dwMoney;
					pUser->m_dwBankMoney	-= dwMoney;

					// 아이템 로그 추가 김영대 2005.02.23
					SetItemLogKarz(	ITEM_LOG_BANK_KARZ_OUT,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,dwMoney);
				}
				else
				{
					pUser->m_dwMoney		+= pItemPickup->dwMoney;
					pUser->m_dwBankMoney	-= pItemPickup->dwMoney;
					// 아이템 로그 추가 김영대 2005.02.23
					SetItemLogKarz(	ITEM_LOG_BANK_KARZ_OUT,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItemPickup->dwMoney);

				}									
				PickupItem.dwMoney	= pUser->m_dwMoney;
				PickupItem.dwId		= pUser->m_dwBankMoney;										
				goto lb_exit2;
			}
		}
		break;
	case 34:		// Weapon 아이템 상점에 팔 때.
		{
			if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 34, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;
					
			if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);

				if(!pBaseItem)
					break;
				
				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nSellPrice	= pBaseItem->BaseItem_Weapon.dwSellPrice;

				if(nMoney+nSellPrice<=__MAX_MONEY__)
				{
					pUser->m_dwMoney		= DWORD(nMoney+nSellPrice); 
					PickupItem.dwMoney		= DWORD(nMoney+nSellPrice);					
					PickupItem.bEquipCode	= 0;						
					bChk = TRUE;
				}
				else
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							if(bChk==FALSE)
							{								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;

								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								bChk = TRUE;
							}
						}
					}
				
					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
						PickupItem.bEquipCode = 2;
					}
				}
				
				// 마우스에서 삭제 //
				memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(bChk==TRUE)
					goto lb_exit2;
			}
		}
		break;
	case 35:		// Weapon 아이템 살 때.
		{			
			int nValue = pItemPickup->dwId/ITEM_DISTRIBUTE;														
			
			if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItemPickup->dwId);

				if(!pBaseItem)
					break;

				if(!CheckStore(pItemPickup->dwId))
					return;

				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nPrice		= pBaseItem->BaseItem_Weapon.dwPrice;

				if(nMoney-nPrice>=0)
				{
					if(pUser->m_pInv_Large[pItemPickup->bZipCode].m_wItemID==0)
					{
						if(bChk==FALSE)
						{
							CreateItem(
								&pUser->m_pInv_Large[pItemPickup->bZipCode], 
								WORD(pItemPickup->dwId), 
								0, 
								Store_Flag_In_Store	
							);

							memcpy(&PickupItem.Item, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
							pUser->m_dwMoney = DWORD(nMoney - nPrice);
							
							PickupItem.bEquipCode	= 1;
							PickupItem.bZipCode		= pItemPickup->bZipCode;
							PickupItem.dwId			= pItemPickup->dwId;
							PickupItem.dwMoney		= pUser->m_dwMoney;
							bChk = TRUE;
						}							
					}
					else
					{						
						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								CreateItem(
									&pUser->m_pInv_Large[i], 
									WORD(pItemPickup->dwId), 
									0,
									Store_Flag_In_Store
								);

								memcpy(&PickupItem.Item, &pUser->m_pInv_Large[i], sizeof(CItem));
								pUser->m_dwMoney = DWORD(nMoney - nPrice);
								bChk = TRUE;
								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;
								PickupItem.dwId			= pItemPickup->dwId;
								PickupItem.dwMoney		= pUser->m_dwMoney;
								break;
							}
						}
					}
				
					if(bChk==FALSE)
					{
						// 실패 //						
						PickupItem.bEquipCode	= 2;
						PickupItem.dwId			= pItemPickup->dwId;
					}						
				}
				else
				{
					// 실패 //					
					PickupItem.bEquipCode	= 3;
					PickupItem.dwId			= pItemPickup->dwId;
				}
				
				goto lb_exit2;
			}
		}
		break;
	case 36:		// Armor 아이템 상점에 팔 때.
		{
			if(pUser->m_ItemMouse.m_wItemID==0)
				return;
			
			int nValue = pUser->m_ItemMouse.m_wItemID/ITEM_DISTRIBUTE;
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 36, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;				

			if(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);

				if(!pBaseItem)
					break;
				
				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nSellPrice	= pBaseItem->BaseItem_Armor.dwSellPrice;

				if(nMoney+nSellPrice<=__MAX_MONEY__)
				{
					pUser->m_dwMoney		= DWORD(nMoney+nSellPrice); 
					PickupItem.dwMoney		= DWORD(nMoney+nSellPrice);					
					PickupItem.bEquipCode	= 0;
					bChk = TRUE;
				}
				else
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							if(bChk==FALSE)
							{								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;

								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								bChk = TRUE;
							}
						}
					}
				
					if(bChk==FALSE)
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
				}
				
				// 마우스에서 삭제 //
				memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(bChk==TRUE)
					goto lb_exit2;		
			}
		}
		break;
	case 37:		// Armor 아이템 살 때.
		{				
			int nValue = pItemPickup->dwId/ITEM_DISTRIBUTE;	
			
			if((nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX) || (pItemPickup->dwId >= 3600 && pItemPickup->dwId <= 3618))				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItemPickup->dwId);
				
				if(!pBaseItem)
					break;

				if(!CheckStore(pItemPickup->dwId))
					return;

				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nPrice		= pBaseItem->BaseItem_Armor.dwPrice;

				if(nMoney-nPrice>=0)
				{
					if(pUser->m_pInv_Large[pItemPickup->bZipCode].m_wItemID==0)
					{
						CreateItem(
							&pUser->m_pInv_Large[pItemPickup->bZipCode], 
							WORD(pItemPickup->dwId), 
							0, 
							Store_Flag_In_Store
						);
						memcpy(&PickupItem.Item, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
						pUser->m_dwMoney = DWORD(nMoney - nPrice);
						
						PickupItem.bEquipCode	= 1;
						PickupItem.bZipCode		= pItemPickup->bZipCode;
						PickupItem.dwId			= pItemPickup->dwId;
						PickupItem.dwMoney		= pUser->m_dwMoney;
						
						bChk = TRUE;
					}
					else
					{						
						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								if(bChk==FALSE)
								{
									CreateItem(
										&pUser->m_pInv_Large[i], 
										WORD(pItemPickup->dwId), 
										0,
										Store_Flag_In_Store
									);
									memcpy(&PickupItem.Item, &pUser->m_pInv_Large[i], sizeof(CItem));
									pUser->m_dwMoney = DWORD(nMoney - nPrice);
									bChk = TRUE;
									
									PickupItem.bZipCode		= i;
									PickupItem.bEquipCode	= 1;
									PickupItem.dwId			= pItemPickup->dwId;
									PickupItem.dwMoney		= pUser->m_dwMoney;
								}
							}
						}
					}
				
					if(bChk==FALSE)
					{
						// 실패 //						
						PickupItem.bEquipCode	= 2;
						PickupItem.dwId			= pItemPickup->dwId;
					}						
				}
				else
				{
					// 실패 //
					PickupItem.bEquipCode	= 3;
					PickupItem.dwId			= pItemPickup->dwId;
				}
				
				goto lb_exit2;
			}				
		}
		break;
	case 38:		// 마우스에서 Guardian 창으로 Guardian 아이템 갈 때.
		{
			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 38, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;			

			if(nValue==ITEM_GUARDIAN_INDEX)
			{
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				if(pUser->m_pInv_Guardian[PickupItem.bZipCode].m_wItemID==0)
				{
					memcpy(&pUser->m_pInv_Guardian[PickupItem.bZipCode], &pUser->m_ItemMouse, sizeof(CItem));
				}
				else
				{
					BOOL bChk = FALSE;

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							PickupItem.bZipCode2 = i;								
							bChk = TRUE;
							break;
						}
					}

					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
					}
				}								
				
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 39:		// Guardian 창에서 마우스로 Guardian 아이템이 갈 때.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Guardian[pItemPickup->bZipCode]);

			if(nValue==ITEM_GUARDIAN_INDEX)
			{	
				pUser->UnSummonGuardian(0);
				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_pInv_Guardian[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pInv_Guardian[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pInv_Guardian[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 40:		// 마우스에서 Guardian 인테페이스로 소비 아이템이 갈 때.
		{
			int		nValue		= GetItemIndex(&pUser->m_ItemMouse);
			BYTE	byPacketCnt	= 0;

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
					
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 40, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;
			
			if(nValue == ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX))
			{
				PickupItem.bZipCode	= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				// 아이템 나누기 
				if(pUser->m_ItemGuardian.m_wItemID == 0)
				{
					memcpy(&pUser->m_ItemGuardian, &pUser->m_ItemMouse, sizeof(CItem));
				}
				else
				{
					if(pUser->m_ItemMouse.m_wItemID == pUser->m_ItemGuardian.m_wItemID)
					{
						CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemGuardian.m_wItemID);

						if(!pBaseItem)
							break;
						
						int nMaxQuantity	= 1;

						nMaxQuantity = pBaseItem->GetMaxQuantity();							

						int nItemQuantity	= nMaxQuantity - pUser->m_ItemGuardian.GetQuantity();
						int nItemMouse		= pUser->m_ItemMouse.GetQuantity();

						if(nItemQuantity>=nItemMouse)
						{
							pUser->m_ItemGuardian.SetQuantity(BYTE(pUser->m_ItemGuardian.GetQuantity()+nItemMouse));
						}
						else
						{
							pUser->m_ItemGuardian.SetQuantity(BYTE(pUser->m_ItemGuardian.GetQuantity()+nItemQuantity));
							byPacketCnt = BYTE(nItemMouse-nItemQuantity);
							
							for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(pUser->m_pInv_Small[i].m_wItemID==0)
								{						
									memcpy(&pUser->m_pInv_Small[i], &pUser->m_ItemMouse, sizeof(CItem));
									pUser->m_pInv_Small[i].SetQuantity(byPacketCnt);
									byPacketCnt = 0;
									break;
								}
								else if(pUser->m_pInv_Small[i].m_wItemID==pUser->m_ItemMouse.m_wItemID)
								{
									CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_pInv_Small[i].m_wItemID);

									if(!pBaseItem)
										break;

									int nMaxQuantity	= 1;

									nMaxQuantity = pBaseItem->GetMaxQuantity();										

									if(pUser->m_pInv_Small[i].GetQuantity()<nMaxQuantity)
									{
										int nCount = nMaxQuantity - pUser->m_pInv_Small[i].GetQuantity();

										if(nCount>=byPacketCnt)
										{
											pUser->m_pInv_Small[i].m_Item_Supplies.bQuantity+=byPacketCnt;
											byPacketCnt = 0;									
											break;
										}
										else
										{
											pUser->m_pInv_Small[i].m_Item_Supplies.bQuantity+=nCount;
											byPacketCnt -= nCount;
										}						
									}
								}									
							}

							if(byPacketCnt>0)
							{
								CItem cInsertItem;
								memcpy(&cInsertItem, &pUser->m_ItemMouse, sizeof(CItem));
								cInsertItem.SetQuantity(byPacketCnt);
								cInsertItem.SetSerialNum(g_pThis->GetNewSerial());
								AppearItem(pUser, &cInsertItem, byPacketCnt, 0, ITEM_TYPE_SMALL);
							}
						}
					}
					else
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);
					}
				}																										

				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}			
		}
		break;
	case 41:		// Guardian 인터페이스에서 마우스로 소비 아이템이 갈 때.
		{
			int nValue = GetItemIndex(&pUser->m_ItemGuardian);

			if(pUser->m_ItemGuardian.GetQuantity()==0)
				return;
			if(pUser->m_ItemMouse.GetID()!=0)
				return;

			if(nValue == ITEM_SUPPLIES_INDEX || (nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX))
			{						
				PickupItem.bZipCode	= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemGuardian, sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_ItemGuardian, sizeof(CItem));
				memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 42:		// Guardian 아이템을 마우스에서 던젼으로.
		{
			int nVal = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 42, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;
							
			if(nVal==ITEM_GUARDIAN_INDEX)
			{				
				PickupItem.bZipCode		= 0;
				PickupItem.bZipCode2	= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				
				if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cGuardianItem.m_wItemID==0)
				{
					pUser->GetCurDungeon()->GetDungeonDataEx()->SetGuardianItem(&pUser->m_ItemMouse);										
				}
				else
				{
					BOOL bChk = FALSE;

					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							PickupItem.bZipCode		= i;
							PickupItem.bZipCode2	= 1;
							bChk = TRUE;
							break;
						}
					}

					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
						PickupItem.bZipCode2	= 2;
					}
				}															
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 43:	// Guardian 아이템을 던젼 관리창에서 마우스로.
		{		
			DUNGEON_DATA_EX* pDungeon = pUser->GetCurDungeon()->GetDungeonDataEx();
			
			if (!pDungeon->m_bSiege)
			{					
				if(pDungeon->m_cGuardianItem.m_wItemID!=0)
				{
					memcpy(&pUser->m_ItemMouse, &pDungeon->m_cGuardianItem, sizeof(CItem));
					
					PickupItem.bZipCode	= 0;
					memcpy(&PickupItem.Item, &pDungeon->m_cGuardianItem, sizeof(CItem));
					memset(&pDungeon->m_cGuardianItem, 0, sizeof(CItem));
					pDungeon->SetGuardianItem(&pDungeon->m_cGuardianItem);
					
					goto lb_exit2;
				}
			}
		}
		break;
	case 44:	// Guardian 에게 아이템 사용.
		{
			DWORD dwRand = 0;

			int nValue = GetItemIndex(&pUser->m_ItemGuardian);

			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemGuardian.m_wItemID);

			if(!pBaseItem)
				return;		
			
			if (!pUser->m_pInv_Guardian[0].GetID())
				return;			

			if(nValue==ITEM_SUPPLIES_INDEX)
			{
				int nCount = pUser->m_ItemGuardian.GetQuantity();

				if(nCount==0)
					return;				

				if (GUARDIAN_ITEM_TYPE_EGG == pUser->m_pInv_Guardian[0].GetBaseItem()->BaseItem_Guardian.bType)
				{
					if( pBaseItem->BaseItem_Supplies.bType == 5 )		
					{
						// CompleteTime
						dwRand	= ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) + pBaseItem->BaseItem_Supplies.wMin;
						
						pUser->m_pInv_Guardian[0].SetGuardianEggCompleteTime(pUser->m_pInv_Guardian[0].m_Item_Guardian.dwCompleteTime+(WORD)dwRand);
						SendToUpdate_GuardianItem(dwConnectionIndex, &pUser->m_pInv_Guardian[0]);						
					}
					else
					{
						return;
					}

					PickupItem.bZipCode	= 1;	// complete타임이 늘어간다.
					PickupItem.dwMoney = 0;
				}
				else
				{
					CMonster* pGuardian = pUser->GetGuardian(0);
					if(!pGuardian)
						return;
					
					float fPer = FLOAT(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery 
						+ g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

					if( pBaseItem->BaseItem_Supplies.bType == 1 )		// HP
					{
						dwRand	= ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) + pBaseItem->BaseItem_Supplies.wMin;
						dwRand  = DWORD((float)dwRand * (fPer / 100.f));
						pGuardian->SetHP(DWORD(pGuardian->GetHP() 
							+ ( pGuardian->GetMaxHP() * ( (float)dwRand  / 100.f ) ) ));

						PickupItem.bZipCode	= 0;	// 가디언 에너지가 빨렷다.
						PickupItem.dwMoney	= pGuardian->GetHP();
					}			
					else if( pBaseItem->BaseItem_Supplies.bType == 3 ) // HP POINT
					{
						dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) + pBaseItem->BaseItem_Supplies.wMin;
						pGuardian->SetHP( DWORD(pGuardian->GetHP() 
							+ ( dwRand * ( fPer / 100.f ) ) ));
						
						PickupItem.bZipCode	= 0;	// 가디언 에너지가 빨렷다.
						PickupItem.dwMoney	= pGuardian->GetHP();
					}								
					else  if( pBaseItem->BaseItem_Supplies.bType == 6 ) // 보약 사용 (시간 연장) : 최덕석 2005.1.17
					{
						dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) + pBaseItem->BaseItem_Supplies.wMin;
						::SendExchangeGuardianLife(pUser, dwRand);

						PickupItem.bZipCode	= 2;	// 생명 연장됨
						PickupItem.dwMoney	= dwRand;
					}
					else if( pBaseItem->BaseItem_Supplies.bType == 7)	// 가디언 스킬북 : 최덕석 2005.1.17
					{
						// Skill Book의 Min가 Skill Kind, Max이 Skill Level이다. : 최덕석 2005.1.17
						const int iSkillKind = pBaseItem->BaseItem_Supplies.wMin;
						const int iSkillLevel = pBaseItem->BaseItem_Supplies.wMax;
						
						PickupItem.bZipCode = 3;	// 스킬북 관련
						PickupItem.bZipCode2 = 0;
						
						const int iOldSkillLevel = pGuardian->GetSkillLevel(iSkillKind);

					
						// 전에 더 높은 레벨의 것을 배웠다면
						if(iOldSkillLevel >= iSkillLevel)
						{
							PickupItem.dwMoney = 2;
							goto lb_exit2;
						}

						// 0과 1사이는 예외처리
						if( iOldSkillLevel == 0 && iSkillLevel != 1)
						{
							PickupItem.dwMoney = 3;
							goto lb_exit2;
						}

						// 바로 밑(5씩)의 레벨을 배우지 않았다면
						if( iOldSkillLevel < iSkillLevel - 5)
						{
							PickupItem.dwMoney = 3;
							goto lb_exit2;
						}

						// 최대로 배울 수 있는 수를 넘었다면
						if(iSkillLevel == 1 && pGuardian->GetTemp(NPC_TEMP_SKILLMANY) >= MAX_GUARDIAN_USE_SKILL)
						{
							PickupItem.dwMoney = 1;
							goto lb_exit2;
						}
						
						// 가디언 레벨이 낮거나 너무 높다면
						if( pGuardian->GetLevel() < pBaseItem->BaseItem_Supplies.wMin_Lev
						||  pGuardian->GetLevel() > pBaseItem->BaseItem_Supplies.wMax_Lev )
						{
							PickupItem.dwMoney = 4;
							goto lb_exit2;
						}
						

						PickupItem.bZipCode2 = iSkillLevel;	// 성공
						PickupItem.dwMoney = iSkillKind;	
						pGuardian->SetSkillLevel(iSkillKind, iSkillLevel);

						// 클라이언트 동기화
						pGuardian->SendAllStatus();
					}
					else  if( pBaseItem->BaseItem_Supplies.bType == 8 ) // 스탯 포멧터 : 최덕석 2005.3.4
					{
						pGuardian->SetDex(0);
						pGuardian->SetEgo(0);
						pGuardian->SetInt(0);
						pGuardian->SetStr(0);
						pGuardian->SetVit(0);
						pGuardian->SetStatPoint((pGuardian->GetLevel() - 1) * 3);
						
						PickupItem.bZipCode	= 4;	// 스텟 초기화 됨!
						PickupItem.dwMoney	= 0;

						// 최신의 상태로 바꾸고, 클라이언트 동기화
						pGuardian->UpdateAllStatus();
						pGuardian->SendAllStatus();
					}
					else
					{
						return;
					}

					if(pGuardian->GetHP() > pGuardian->GetMaxHP())
						pGuardian->SetHP(pGuardian->GetMaxHP());
				}

				if(nCount>1)
					pUser->m_ItemGuardian.SetQuantity(BYTE(nCount-1));
				else if(nCount==1)
					memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));				
				
				memcpy(&PickupItem.Item, &pUser->m_ItemGuardian, sizeof(CItem));
				goto lb_exit2;
			}
		}
		break;
	case 45:		// Guardian이 사라지면 물약 아이템 이동.
		{
			int nValue = GetItemIndex(&pUser->m_ItemGuardian);

			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 45, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;
			
			if(nValue==ITEM_SUPPLIES_INDEX)
			{				
				PickupItem.bZipCode	= 0;
				memcpy(&PickupItem.Item, &pUser->m_ItemGuardian, sizeof(CItem));

//				BYTE byPacketCnt = pUser->m_ItemGuardian.GetQuantity();

				Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);

				memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
			}
			goto lb_exit2;
		}
		break;
	case 46:
		{
			// sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 거래..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
				return;

			if(!CheckTrade(pUser->m_nItemTradeSeriel))
				return;
			
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			if(!CheckItem(&pUser->m_ItemMouse, ITEM_TYPE_LARGE))
				return;
			
			// Item Move //
			if(pUser->m_pTradeItem[pItemPickup->bZipCode].GetID()==0)
			{				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
				memcpy(&pUser->m_pTradeItem[pItemPickup->bZipCode], &pUser->m_ItemMouse, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				
				goto lb_exit2;
			}
			else
			{
				return;
			}
		}
		break;
	case 47:		// 작은 아이템이 마우스에서 트레이드 창으로 이동.
		{
			// sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 거래..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
				return;

			if(!CheckTrade(pUser->m_nItemTradeSeriel))
				return;

//			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			if(pUser->m_ItemMouse.GetID()==0)
				return;

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(pUser->m_ItemMouse.GetQuantity() < pItemPickup->dwMoney)
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
				{
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);
					
					PickupItem.bZipCode	= 0;
									
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}

				return;
			}
			
			if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)				
			{				
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				//if(pUser->m_pTradeItem[pItemPickup->bZipCode].GetID()==0)
				{
					if(pUser->m_ItemMouse.GetQuantity()>=pItemPickup->dwMoney)
					{
						BYTE	byPacketCount	= BYTE(pItemPickup->dwMoney);	
						BYTE	byOutCount		= BYTE(pUser->m_ItemMouse.GetQuantity()-pItemPickup->dwMoney);
											
						if(byPacketCount==pUser->m_ItemMouse.GetQuantity())
						{
							Insert_TradeItem(pUser
								, &pUser->m_ItemMouse
								, pItemPickup->bZipCode
								, pUser->m_ItemMouse.GetQuantity()
								, FALSE);
						}
						else
						{
							pUser->m_ItemMouse.SetQuantity(BYTE(pItemPickup->dwMoney));

							Insert_TradeItem(pUser
								, &pUser->m_ItemMouse
								, pItemPickup->bZipCode
								, BYTE(pItemPickup->dwMoney)
								, TRUE);
							
							if(byOutCount!=0)
							{
								pUser->m_ItemMouse.SetQuantity(byOutCount);
								Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, byOutCount, TRUE);
							}
						}		
						
						PickupItem.dwMoney	=	pItemPickup->dwMoney;
						PickupItem.bZipCode	=	pItemPickup->bZipCode;					
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}																
				}				
			}
			else
			{
				return;
			}
		}
		break;
	case 48:		// 큰 아이템이 트레이드 창에서 마우스로 이동.
		{
			if(!CheckTrade(pUser->m_nItemTradeSeriel))
				return;

//			int nValue = pUser->m_pTradeItem[pItemPickup->bZipCode].GetID()/ITEM_DISTRIBUTE;				
			
			if(pUser->m_ItemMouse.GetID()==0)
			{				
				if(GetItemType(pUser->m_pTradeItem[pItemPickup->bZipCode].GetID())!=ITEM_TYPE_LARGE)
					return;
				
				PickupItem.bZipCode	= pItemPickup->bZipCode;
					
				memcpy(&PickupItem.Item, &pUser->m_pTradeItem[pItemPickup->bZipCode], sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->m_pTradeItem[pItemPickup->bZipCode], sizeof(CItem));
				memset(&pUser->m_pTradeItem[pItemPickup->bZipCode], 0, sizeof(CItem));
				goto lb_exit2;						
			}
		}
		break;
	case 49:		// 작은 아이템이 트레이드 창에서 마우스로 이동.
		{
			if(!CheckTrade(pUser->m_nItemTradeSeriel))
				return;

//			int nValue = GetItemIndex(&pUser->m_pTradeItem[pItemPickup->bZipCode]);

			if(pUser->m_pTradeItem[pItemPickup->bZipCode].GetQuantity()==0)
				return;
			
			if(GetItemType(pUser->m_pTradeItem[pItemPickup->bZipCode].GetID())==ITEM_TYPE_SMALL)
			{
				if(pUser->m_ItemMouse.GetID()==0)
				{					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
						
					memcpy(&PickupItem.Item, &pUser->m_pTradeItem[pItemPickup->bZipCode], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pTradeItem[pItemPickup->bZipCode], sizeof(CItem));
					memset(&pUser->m_pTradeItem[pItemPickup->bZipCode], 0, sizeof(CItem));
					goto lb_exit2;					
				}
				else
					return;
			}
			else
				return;
		}
		break;
	case 50:		// 포탈에 위치 저장.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_nItemTradeSeriel!=0)
				return;

			if(nValue==ITEM_ZODIAC_INDEX)
			{
				if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_INDEX__)
				{				
					BOOL bPortalChk = FALSE;
					
					if(!pUser->GetCurDungeon()->GetDungeonDataEx()->IsDungeonOwner(pUser))
					{
#if defined TAIWAN_LOCALIZING
						// 대만은 모든 던전 0층에서 조디악 위치를 기억할수있다.					
						// 마을 은 안되게
						if( pUser->GetCurDungeon()->GetID() > 10 &&
							pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_EVENT)
#else
						if(pUser->GetCurDungeon()->GetID() >= 2000 && pUser->GetCurDungeon()->GetID() < 4000)
#endif
						{
							if(pUser->GetCurLayerIndex() == 0)
							{
								bPortalChk = TRUE;
							}
						}
					}
					else
					{
						bPortalChk = TRUE;
					}

					if(bPortalChk)
					{						
						PickupItem.bZipCode	= pItemPickup->bZipCode;						
						
						SetPositionPotalItem(&pUser->m_pInv_Small[pItemPickup->bZipCode], pUser->GetCurLayerIndex()
							,pUser->GetCurDungeonID(), (WORD)pUser->GetCurPosition()->x
							,(WORD)pUser->GetCurPosition()->y);

						memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));					
						
						goto lb_exit2;
					}
					else
					{
						return;
					}
				}							
			}
			else
			{
				return;
			}
		}
		break;
	case 51:		// 포탈 사용.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_nItemTradeSeriel!=0)
				return;

			if(nValue==ITEM_ZODIAC_INDEX)
			{
				if(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetID() == __ITEM_PORTALUSED_INDEX__)
				{
					BOOL bPortalChk = FALSE;

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(
						pUser->m_pInv_Small[pItemPickup->bZipCode].m_Item_Zodiac.wMapId);

					if(pDungeon)
					{
						if(!pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser))
						{
#if defined TAIWAN_LOCALIZING
								// 대만은 모든 던전 0층에서 조디악 위치를 기억할수있다.
								// 마을은 안되게
								if( pUser->GetCurDungeon()->GetID() > 10 &&
									pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_EVENT)
#else
								if(pDungeon->GetID()>=2000 && pDungeon->GetID()<4000)
#endif
							{
								if(pUser->m_pInv_Small[pItemPickup->bZipCode].m_Item_Zodiac.bLayer==0)
									bPortalChk = TRUE;
							}
						}
						else
						{
							bPortalChk = TRUE;
						}
					}
					else
					{
						bPortalChk = TRUE;
					}

					if(bPortalChk)
					{						
						PickupItem.bZipCode		= pItemPickup->bEquipCode;
						PickupItem.bEquipCode	= pItemPickup->bZipCode;
						PickupItem.dwMoney		= pUser->m_pInv_Small[pItemPickup->bEquipCode].GetID();
							
						memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bEquipCode], sizeof(CItem));

						if(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetQuantity()==1)
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bEquipCode], 0, sizeof(CItem));
						}
						else
						{
							pUser->m_pInv_Small[pItemPickup->bEquipCode].SetQuantity(
								BYTE(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetQuantity()-1));
						}
						
						if(pUser->m_ItemGuardian.GetID()!=0)
						{
							Insert_SmallInvItem(pUser
								, &pUser->m_ItemGuardian
								, 0
								, pUser->m_ItemGuardian.GetQuantity()
								, TRUE);							
							memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
						}
							

						//  마을 포탈시 가디언이 소환 되어 있을경우 귀환시킨후 포탈 
						//  김영대 2005.02.07
						if (pUser->IsSummonGuardian(0) ) 	pUser->UnSummonGuardian(0);

						CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

						if(pDungeon)
						{
							goto lb_exit2;
						}

						CheckItemSlot(pUser);
						
						goto lb_exit2;
					}
					else
					{
						return;
					}
				}							
			}
			else
			{
				return;
			}
		}
		break;
	case 52:		// 귀환 스크롤 사용.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;
			
			if(pUser->m_nItemTradeSeriel!=0)
				return;

			if(nValue==ITEM_ZODIAC_INDEX)
			{
				//if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL1__ || 
				//	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL2__ ||
				//	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL3__ ||
				//	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL4__ ||
				//	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL5__)
				// 마을 귀환 스크롤 추가 김영대 2004.02.14
				WORD wItemID = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();
				if (wItemID >= __ITEM_PORTAL_VILL1__  &&  wItemID <= __ITEM_PORTAL_VILL6__)
				{					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					PickupItem.dwMoney	= pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

											
					SetPortal(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));

					if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==1)
						memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
					else
						pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()-1));
						
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);							
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}
					
					//  마을 포탈시 가디언이 소환 되어 있을경우 귀환시킨후 포탈 
					//  김영대 2005.02.07
					if (pUser->IsSummonGuardian(0) ) 	pUser->UnSummonGuardian(0);


					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;
						
					CheckItemSlot(pUser);

					goto lb_exit2;
				}							
			}
			else
				return;
		}
		break;
	case 53:		// 벨트에서 귀환 스크롤 사용.
		{
			int nValue = GetItemIndex(&pUser->m_pBelt[pItemPickup->bZipCode]);

			if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
			if(nValue==ITEM_ZODIAC_INDEX)
			{
				// 마을 귀환 스크롤 추가 김영대 2004.02.14
				WORD wItemID = pUser->m_pBelt[pItemPickup->bZipCode].GetID();
				if (wItemID >= __ITEM_PORTAL_VILL1__  &&  wItemID <= __ITEM_PORTAL_VILL6__)
				//if(pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL1__ || pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL2__ || pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL3__ || pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL4__ || pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_VILL5__)
				{					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					PickupItem.dwMoney	= pUser->m_pBelt[pItemPickup->bZipCode].GetID();
						
					SetPortal(&pUser->m_pBelt[pItemPickup->bZipCode]);

					memcpy(&PickupItem.Item, &pUser->m_pBelt[pItemPickup->bZipCode], sizeof(CItem));						

					if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==1)
						memset(&pUser->m_pBelt[pItemPickup->bZipCode], 0, sizeof(CItem));
					else
						pUser->m_pBelt[pItemPickup->bZipCode].SetQuantity(BYTE(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()-1));
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);							
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}

					//  마을 포탈시 가디언이 소환 되어 있을경우 귀환시킨후 포탈 
					//  김영대 2005.02.07
					if (pUser->IsSummonGuardian(0) ) 	pUser->UnSummonGuardian(0);

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					goto lb_exit2;
				}							
			}
			else
				return;
		}
		break;
	case 54:		// 포탈 사용.
		{
			int nValue = GetItemIndex(&pUser->m_pBelt[pItemPickup->bZipCode]);

			if(pUser->m_ItemMouse.GetID()!=0)
				return;

			if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
			if(nValue==ITEM_ZODIAC_INDEX)
			{
				// 벨트에서 조디악 쌍둥이카드 사용안되는 버그 수정 
				// 김영대 2005.02.07
				if(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetID()==__ITEM_PORTALUSED_INDEX__)
				{
					BOOL bPortalChk = FALSE;

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pUser->m_pBelt[pItemPickup->bZipCode].m_Item_Zodiac.wMapId);

					if(pDungeon)
					{
						if(!pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser))
						{
#if defined TAIWAN_LOCALIZING
								// 대만은 모든 던전 0층에서 조디악 위치를 기억할수있다.	
								// 마을 은 안되게
								if( pUser->GetCurDungeon()->GetID() > 10 &&
									pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() != DUNGEON_TYPE_EVENT)
#else

								if(pDungeon->GetID()>=2000 && pDungeon->GetID()<4000)
#endif
							{
								if(pUser->m_pBelt[pItemPickup->bZipCode].m_Item_Zodiac.bLayer==0)
								{
									bPortalChk = TRUE;
								}
							}
						}
						else
						{
							bPortalChk = TRUE;
						}
					}
					else
					{
						bPortalChk = TRUE;
					}

					if(bPortalChk)
					{													
						PickupItem.bZipCode		= pItemPickup->bEquipCode;
						PickupItem.bEquipCode	= pItemPickup->bZipCode;
						PickupItem.dwMoney		= pUser->m_pInv_Small[pItemPickup->bEquipCode].GetID();

						memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bEquipCode], sizeof(CItem));
						
						if(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetQuantity()==1)
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bEquipCode], 0, sizeof(CItem));
						}
						else
						{
							pUser->m_pInv_Small[pItemPickup->bEquipCode].SetQuantity(
								BYTE(pUser->m_pInv_Small[pItemPickup->bEquipCode].GetQuantity()-1));
						}
						
						if(pUser->m_ItemGuardian.GetID()!=0)
						{
							Insert_SmallInvItem(pUser
								, &pUser->m_ItemGuardian
								, 0
								, pUser->m_ItemGuardian.GetQuantity()
								, TRUE);							

							memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
						}
						
						//  마을 포탈시 가디언이 소환 되어 있을경우 귀환시킨후 포탈 
						//  김영대 2005.02.07
						if (pUser->IsSummonGuardian(0) ) 	pUser->UnSummonGuardian(0);

						CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

						if(pDungeon)
							goto lb_exit2;
												
						CheckItemSlot(pUser);

						goto lb_exit2;
					}
					else
						return;
				}							
			}
			else
				return;
		}
		break;
	case 55:		// 벨트에서 위치 저장.
		{
			int nValue = GetItemIndex(&pUser->m_pBelt[pItemPickup->bZipCode]);

			if(pUser->m_pBelt[pItemPickup->bZipCode].GetQuantity()==0)
				return;

 			if(pUser->m_nItemTradeSeriel!=0)
				return;
			
			if(nValue==ITEM_ZODIAC_INDEX)
			{
				if(pUser->m_pBelt[pItemPickup->bZipCode].GetID()==__ITEM_PORTAL_INDEX__)
				{					
					PickupItem.bZipCode	= pItemPickup->bZipCode;						
					
					SetPositionPotalItem(&pUser->m_pBelt[pItemPickup->bZipCode], pUser->GetCurLayerIndex()
						,pUser->GetCurDungeonID(), (WORD)pUser->GetCurPosition()->x
						,(WORD)pUser->GetCurPosition()->y);
					
					memcpy(&PickupItem.Item, &pUser->m_pBelt[pItemPickup->bZipCode], sizeof(CItem));
					
					goto lb_exit2;
				}							
			}
			else
				return;
		}
		break;
	case 56:		// 파티 마법진 아이템(퍼스널 포탈) 사용할 때.
		{
			CUser* pSendUser = g_pUserHash->GetData(pItemPickup->dwMoney);

			if(pSendUser)
			{
				if(!pSendUser->GetCurDungeon() || !pUser->GetCurDungeon())
				{
					// 실패 //
					DSTC_PARTY_PORTALFAIL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 2;
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 2;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				if(pSendUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID == 
					pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID)
				{
					if(pSendUser->GetCurLayerIndex() == pUser->GetCurLayerIndex())
					{
						// 실패 //
						DSTC_PARTY_PORTALFAIL pSendPacket;
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						pSendPacket.byResult = 5;
						NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
							, FLAG_SEND_NOT_ENCRYPTION);
						
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						pSendPacket.byResult = 5;
						NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
							, FLAG_SEND_NOT_ENCRYPTION);
						return;
					}
				}

				if(pUser->IsCurDungeonEventInLive())
				{
					// 실패 //
					DSTC_PARTY_PORTALFAIL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 5;
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 5;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}	

/*
					// Item Chk //
					if(	pItemPickup->bZipCode2 == 0 ||
						pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
						pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
					{
*/				
					// 포탈 이동 메세지 //
					DSTC_PARTY_PORTALMOVE pSendPacket;
					pSendPacket.wDungeonID		= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
					pSendPacket.byLayerIndex	= pUser->GetCurLayerIndex();
					pSendPacket.dwPartyId		= pUser->m_dwPartyId;
					pSendPacket.fPosX			= pUser->GetCurPosition()->x;
					pSendPacket.fPosZ			= pUser->GetCurPosition()->y;						
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
					
					// 아이템 메세지 //						
					if(pItemPickup->bZipCode2==1)
					{
						// 퍼스널 리콜
						BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
							pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
						{
							if(byCount>1)
								pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
							else
								memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
					}						
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));												

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}

					goto lb_exit2;
/*
								}
								else
								{
									// 실패 //
									DSTC_PARTY_PORTALFAIL pSendPacket;
									memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
									__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									pSendPacket.byResult = 3;
									NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				
									memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
									__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
									pSendPacket.byResult = 3;
									NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
									return;
								}*/
				
			}
			else
			{
				if(pUser->IsCurDungeonEventInLive())
				{
					// 실패 //
					DSTC_PARTY_PORTALFAIL pSendPacket;
					pSendPacket.byResult = 5;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION);
					return;
				}	

/*
				// Item Chk //
				if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
					pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
				{*/

					// 월드 포탈 이동 메세지 //
					WSTDS_PARTY_PORTALMOVE pSendPacket;
					pSendPacket.wDungeonID			= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
					pSendPacket.byLayerIndex		= pUser->GetCurLayerIndex();
					pSendPacket.dwPartyId			= pUser->m_dwPartyId;
					pSendPacket.fPosX				= pUser->GetCurPosition()->x;
					pSendPacket.fPosZ				= pUser->GetCurPosition()->y;
					pSendPacket.dwUserIndex			= pUser->GetID();
					pSendPacket.dwPortalUserIndex	= pItemPickup->dwMoney;
					g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					// 아이템 메세지 //
					if(pItemPickup->bZipCode2==1)
					{
						// 퍼스널 리콜
						BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
							pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
						{   
							if(byCount>1)
								pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
							else
								memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
					}						
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));												

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}

					goto lb_exit2;
/*
				}
				else
				{
					// 실패 //
					WSTDS_PARTY_PORTALFAIL pSendServerPacket;						
					memset(pSendServerPacket.szName, 0, sizeof(pSendServerPacket.szName));
					__lstrcpyn(pSendServerPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendServerPacket.byResult		= 3;
					pSendServerPacket.dwUserIndex	= pItemPickup->dwMoney;
					g_pNet->SendToServer(WSINDEX,  (char*)&pSendServerPacket, pSendServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
*/
				
			}
		}
		break;
	case 57:	// 마우스에서 노점으로 아이템 옴길때 
		{
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			//sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 노점..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[ i ].m_wItemID==0)
					{
						PickupItem.bInv		= 6;
						PickupItem.bZipCode = i;
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}
				}
			}

			if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			if(pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType()!=DUNGEON_TYPE_VILLAGE)
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_LARGE)
				{
					if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 57, DEF_COPYITEM_TYPE_LARGE, TRUE))
						goto lb_exit2;
					
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[ i ].m_wItemID==0)
						{
							PickupItem.bInv		= 6;
							PickupItem.bZipCode = i;
							memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
							memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
							goto lb_exit2;
						}
					}				
										
					AppearItem(pUser
						, &pUser->m_ItemMouse
						, 0
						, 0
						, 0
						, pUser->GetCurPosition()->x
						, 0
						, pUser->GetCurPosition()->y);
					
					PickupItem.bEquipCode	= 0;
					PickupItem.bInv			= 10;
					PickupItem.bZipCode		= 0;
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}
				return;
			}

			if(pUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()!=0)
			{
				if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
				{
					for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
					{
						if(pUser->m_sPlayerShop.cItem[i].GetID()==0)
						{								
							PlayerShopLog(pUser, &pUser->m_ItemMouse, pItemPickup->bInv, pItemPickup->bZipCode, pItemPickup->dwMoney, 0, TRUE);
							return;
						}
					}
				}					

				for(i = 0; i < MAX_INV_LARGE_POOL; i++)
				{
					if(pUser->m_pInv_Large[i].GetID()==0)
					{													
						PickupItem.bZipCode		= BYTE(i);
						PickupItem.dwMoney		= 0;
						PickupItem.bZipCode2	= 2;
						memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
						memcpy(&PickupItem.Item, &pUser->m_pInv_Large[i], sizeof(CItem));
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}						
				}												
				
				AppearItem(pUser, &pUser->m_ItemMouse, pUser->m_ItemMouse.GetQuantity(), 0, ITEM_TYPE_LARGE);
				PickupItem.bEquipCode	= 0;				
				PickupItem.bZipCode		= 0;
				memset(&PickupItem.Item, 0, sizeof(CItem));
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit1;
			}
			else
			{
				PlayerShopLog(pUser, &pUser->m_ItemMouse, pItemPickup->bInv, pItemPickup->bZipCode, pItemPickup->dwMoney, 0, TRUE);
				return;
			}								
		}
		break;
	case 58:		// 노점에서 마우스로 아이템 옮길때.
		{
			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			if(pUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()==0)
				return;

			CItem cItem;
			memset(&cItem, 0, sizeof(CItem));
			PlayerShopLog(pUser, &cItem, pItemPickup->bInv, pItemPickup->bZipCode, pUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode], 0, TRUE);
			return;								
		}
		break;
	case 59:	// 마우스에서 노점 소비 아이템.
		{				
//			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			//sung-han 2005-03-15 거래, 드롭, 노점않되는 아이템 처리, 여기는 노점..
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if( pBaseItem && pBaseItem->GetMovable() != 0 )
			{
				memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

				Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

				PickupItem.bInv		= 14;
				PickupItem.bZipCode	= 0;
								
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;
			}

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;				

			if(pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType()!=DUNGEON_TYPE_VILLAGE)
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
				{
					if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 59, DEF_COPYITEM_TYPE_SMALL, TRUE))
						goto lb_exit2;
					
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

					PickupItem.bInv		= 14;
					PickupItem.bZipCode	= 0;
									
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}
				return;
			}
			
			if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
			{
				CItem cItem;
				memcpy(&cItem, &pUser->m_ItemMouse, sizeof(CItem));
				cItem.SetQuantity(BYTE(pItemPickup->dwMoney));										

				if(pUser->m_ItemMouse.GetQuantity() < pItemPickup->dwMoney)
				{
					if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
					{
						memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

						Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), TRUE);

						PickupItem.bInv		= 14;
						PickupItem.bZipCode	= 0;
										
						memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
						goto lb_exit2;
					}
					return;
				}

				PlayerShopLog(pUser, &cItem, pItemPickup->bInv, pItemPickup->bZipCode, pItemPickup->dwId, pItemPickup->dwMoney, TRUE);
			}
			else
				return;
		}
		break;
	case 60:		// 노점에서 마우스로 아이템 옮길때.
		{
			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			if(pUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()==0)
				return;
			
			CItem cItem;
			memset(&cItem, 0, sizeof(CItem));
			PlayerShopLog(pUser, &cItem, pItemPickup->bInv, pItemPickup->bZipCode, pUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode], pItemPickup->dwMoney, TRUE);
			return;								
		}
		break;
	case 61:		// 파티 마법진 사용.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(!pUser->GetCurDungeon())
				return;

			if(pUser->GetCurDungeon()->GetDungeonDataEx()->IsVillage())
				return;				

			if(nValue==ITEM_MAGICARRAY_INDEX)
			{
				WORD wItemId = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

				if(	(wItemId>=__ITEM_PARTY_LIFELINK1__	&&	wItemId<=__ITEM_PARTY_LIFELINK5__)	||
					(wItemId>=__ITEM_PARTY_AMBUSH1__	&&	wItemId<=__ITEM_PARTY_AMBUSH5__)	||
					(wItemId>=__ITEM_PARTY_ST1__		&&	wItemId<=__ITEM_PARTY_ST5__)			)
				{
					if(pUser->m_pUsingMagicArrayList->GetCount()==0)
					{
						// 파티 마법진 사용 //						
						pLayer->AttachMagicArrayToParty(pUser, pUser->m_pInv_Small[pItemPickup->bZipCode].GetID());
						
						if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()>1)
						{
							pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(
								BYTE(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()-1));
						}
						else
						{
							memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
						
						PickupItem.bZipCode		= pItemPickup->bZipCode;
						memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
						goto lb_exit2;
					}
					else
					{
						DSTC_PARTY_MAIGCARRAYFAIL pPaccket;
						pPaccket.byErrorCode = 1;
						NetSendToUser( dwConnectionIndex, (char*)&pPaccket, pPaccket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						return;
					}						
				}
				else
				{
					return;
				}
			}
		}
		break;
	case 62:		// 파티원 전체 포탈.
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(!pUser->GetCurDungeon())
				return;
			
			if(pUser->IsCurDungeonSiege())
				return;				

			if(nValue==ITEM_MAGICARRAY_INDEX)
			{
				WORD wItemId = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

				if(wItemId==__ITEM_PARTY_PORTAL2__)
				{
					if(pUser->IsCurDungeonEventInLive())
						return;

					// 월드 서버로 보낸다 //
					WSTDS_PARTY_ALLPORTAL pSendPacket;						
					pSendPacket.dwPartyId	= pUser->m_dwPartyId;						
					pSendPacket.dwUserIndex	= pUser->GetID();
					pSendPacket.byIndex		= pItemPickup->bZipCode;						
					g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()>1)
					{
						pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(
							BYTE(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()-1));
					}
					else
					{
						memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
					}

					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
					goto lb_exit2;
				}
				else
				{
					return;
				}
			}
		}
		break;
	case 63:		// 마우스에서 던전관리 창으로 마법진 이동
		{	
			if (pUser->GetCurDungeon()->GetDungeonDataEx()->m_bSiege)	return;
			if (!pUser->GetCurDungeon()->GetDungeonDataEx()->IsDungeonOwner(pUser))
				break;

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 63, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;							
			
			if(nValue==ITEM_MATERIALEND_INDEX)
			{
				switch (pUser->m_ItemMouse.GetID())
				{
				case __ITEM_METERIALS_FIRE_PROPERTY_INDEX__:
				case __ITEM_METERIALS_ICE_PROPERTY_INDEX__:
				case __ITEM_METERIALS_LIGHT_PROPERTY_INDEX__:					
					PickupItem.bZipCode		= 0;
					PickupItem.dwMoney		= pItemPickup->dwMoney;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));					
					
					Insert_MagicFieldArrayItem(pUser, &pUser->m_ItemMouse, BYTE(pItemPickup->dwMoney));
															
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;					
				}																		
			}
		}
		break;
	case 64:	// 던전관리창에서 마법진을 마우스로
		{
			if (pUser->GetCurDungeon()->GetDungeonDataEx()->m_bSiege)	return;
			if (pUser->m_ItemMouse.GetID())	return;
			if (!pUser->GetCurDungeon()->GetDungeonDataEx()->IsDungeonOwner(pUser))
				break;
			
			int nValue = pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetID()/ITEM_DISTRIBUTE;

			if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity()==0)
				return;

			if(nValue==ITEM_MATERIALEND_INDEX)
			{				
				PickupItem.bZipCode		= 0;					
				memcpy(&PickupItem.Item, &pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem, sizeof(CItem));
				memcpy(&pUser->m_ItemMouse, &pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem, sizeof(CItem));
				pUser->m_ItemMouse.SetQuantity(1);

				if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity()>1)
					pUser->m_ItemMouse.m_Serial	= *g_pThis->GetNewSerial();
				
				if(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity()>1)
					pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.SetQuantity(BYTE(pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem.GetQuantity()-1));
				else
					memset(&pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem, 0, sizeof(CItem));
				
				pUser->GetCurDungeon()->GetDungeonDataEx()->SetMagicFieldArray(&pUser->GetCurDungeon()->GetDungeonDataEx()->m_cMagicFieldArrayItem);
				goto lb_exit2;					
			}
		}
		break;
	case 65:
		{
			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			BOOL bIsCorrect = TRUE;

			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if(pBaseItem)
			{
				if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_LARGE)
				{
					if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 65, DEF_COPYITEM_TYPE_LARGE, TRUE))
						goto lb_exit2;

					if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
					{
						if(nValue == ITEM_CLASS_RING || nValue == ITEM_CLASS_AMULET)
						{
							bIsCorrect = FALSE;
						}
						else if(pUser->m_ItemMouse.m_bSlot.uMaxSlot == 0)
						{
							bIsCorrect = FALSE;
						}
						else if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
						{
							if(pBaseItem->BaseItem_Weapon.bItemGrade == 0)
							{
								bIsCorrect = FALSE;
							}
						}
						else if(nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX)
						{
							if(pBaseItem->BaseItem_Armor.bItemGrade == 0)
							{
								bIsCorrect = FALSE;
							}
						}

						for(int i = 1; i < 4; i++)
						{
							CBaseItem* pBaseItemSmall = g_pBaseItemHash->GetData(pUser->m_pUpgradeItem[i].GetID());
							int nValueSmall = pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE;

							if(pBaseItemSmall && nValueSmall == ITEM_UPGRADE_INDEX)
							{
								if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
								{
									if( pBaseItem->BaseItem_Weapon.bItemGrade != pBaseItemSmall->BaseItem_UpGrade.wW_Grade )
									{
										bIsCorrect = FALSE;
									}
									if( pBaseItemSmall->BaseItem_UpGrade.wFormula[nValue] == 0 )
									{
										bIsCorrect = FALSE;	
									}
								}
								else if(nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX)
								{
									if( pBaseItem->BaseItem_Armor.bItemGrade != pBaseItemSmall->BaseItem_UpGrade.wW_Grade )
									{
										bIsCorrect = FALSE;
									}
									if( pBaseItemSmall->BaseItem_UpGrade.wFormula[nValue] == 0 )
									{
										bIsCorrect = FALSE;
									}
								}
							}
							else if(pBaseItemSmall && nValueSmall == ITEM_LIQUID_INDEX)
							{
								if(nValue >= ITEM_WEAPONST_INDEX && nValue <= ITEM_WEAPONEND_INDEX)
								{
									if( pBaseItem->BaseItem_Weapon.bItemGrade != pBaseItemSmall->BaseItem_LiQuid.wW_Grade )
									{
										bIsCorrect = FALSE;
									}
								}
								else if(nValue >= ITEM_ARMORST_INDEX && nValue <= ITEM_ARMOREND_INDEX)
								{
									if( pBaseItem->BaseItem_Armor.bItemGrade != pBaseItemSmall->BaseItem_LiQuid.wW_Grade )
									{
										bIsCorrect = FALSE;
									}
								}
							}
							else if(pBaseItemSmall && nValueSmall == ITEM_EDITION_INDEX)
							{
								if( pBaseItemSmall->BaseItem_Edition.wFormula[nValue] == 0 )
								{
									bIsCorrect = FALSE;
								}
							}
						}
						
						if(bIsCorrect && pUser->m_pUpgradeItem[0].GetID()==0)
						{
							PickupItem.bZipCode		= 0;					
							memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
							memcpy(&pUser->m_pUpgradeItem[0], &pUser->m_ItemMouse, sizeof(CItem));										
							memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
							goto lb_exit2;						
						}
						else
						{
							for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
							{
								if(pUser->m_pInv_Large[i].GetID() == 0)
								{
									PickupItem.bZipCode = 254;
									PickupItem.bEquipCode = i;
									memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
									memcpy(&pUser->m_pInv_Large[i],&PickupItem.Item,sizeof(CItem));
									memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
									goto lb_exit2;						
								}
							}
						}
					}
				}
			}
		}
		break;
	case 66:
		{
			int nValue = GetItemIndex(&pUser->m_pUpgradeItem[0]);

			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_pUpgradeItem[0].GetID())==ITEM_TYPE_LARGE)
			{
				if(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_ARMOREND_INDEX)
				{											
					PickupItem.bZipCode		= 0;					
					memcpy(&PickupItem.Item, &pUser->m_pUpgradeItem[0], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pUpgradeItem[0], sizeof(CItem));										
					memset(&pUser->m_pUpgradeItem[0], 0, sizeof(CItem));
					goto lb_exit2;
				}
			}
		}
		break;			
	case 67:
		{
			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemMouse.GetID());
			if(NULL == pBaseItem)
				return;

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(pUser->m_pUpgradeItem[pItemPickup->bZipCode].GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
			{
				if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 67, DEF_COPYITEM_TYPE_SMALL, TRUE))
					goto lb_exit2;					
				
				if( nValue == ITEM_UPGRADE_INDEX || nValue == ITEM_LIQUID_INDEX || nValue == ITEM_EDITION_INDEX )	
				{					
					PickupItem.bZipCode		= pItemPickup->bZipCode;
					PickupItem.dwMoney		= pItemPickup->dwMoney;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					BOOL bChk = FALSE;

					if(pUser->m_pUpgradeItem[0].GetID() != 0)
					{
						CBaseItem* pBaseItemLarge = g_pBaseItemHash->GetData(pUser->m_pUpgradeItem[0].GetID());
						int nValueLarge = pUser->m_pUpgradeItem[0].GetID() / ITEM_DISTRIBUTE;

						if(pBaseItemLarge)
						{
							if(nValue == ITEM_UPGRADE_INDEX)
							{
								if(nValueLarge >= ITEM_WEAPONST_INDEX && nValueLarge <= ITEM_WEAPONEND_INDEX)
								{
									if(pBaseItemLarge->BaseItem_Weapon.bItemGrade != pBaseItem->BaseItem_UpGrade.wW_Grade)
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
									if(pBaseItem->BaseItem_UpGrade.wFormula[nValueLarge] == 0)
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
								}
								else if(nValueLarge >= ITEM_ARMORST_INDEX && nValueLarge <= ITEM_ARMOREND_INDEX)
								{
									if(pBaseItemLarge->BaseItem_Armor.bItemGrade != pBaseItem->BaseItem_UpGrade.wW_Grade)
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
									if(pBaseItem->BaseItem_UpGrade.wFormula[nValueLarge] == 0)
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
								}
							}
							else if(nValue == ITEM_LIQUID_INDEX)
							{
								if(nValueLarge >= ITEM_WEAPONST_INDEX && nValueLarge <= ITEM_WEAPONEND_INDEX)
								{
									if( pBaseItemLarge->BaseItem_Weapon.bItemGrade != pBaseItem->BaseItem_LiQuid.wW_Grade )
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
								}
								else if(nValueLarge >= ITEM_ARMORST_INDEX && nValueLarge <= ITEM_ARMOREND_INDEX)
								{
									if( pBaseItemLarge->BaseItem_Armor.bItemGrade != pBaseItem->BaseItem_LiQuid.wW_Grade )
									{
										PickupItem.bEquipCode	= 0;
										Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
										memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
										goto lb_exit2;
									}
								}
							}
							else if(nValue == ITEM_EDITION_INDEX)
							{
								if( pBaseItem->BaseItem_Edition.wFormula[nValueLarge] == 0 )
								{
									PickupItem.bEquipCode	= 0;
									Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
									memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
									goto lb_exit2;
								}
							}	
						}						
					}
					else
					{
						for(int i = 1; i < 4; i++)
						{
							if(pUser->m_pUpgradeItem[i].GetID() != 0)
							{
								int nValueSmall = pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE;
								CBaseItem* pBaseItemSmall = g_pBaseItemHash->GetData(pUser->m_pUpgradeItem[i].GetID());

								if(pBaseItemSmall)
								{
									if(nValueSmall == ITEM_UPGRADE_INDEX && nValue == ITEM_LIQUID_INDEX)
									{
										if(pBaseItem->BaseItem_LiQuid.wW_Grade != pBaseItemSmall->BaseItem_UpGrade.wW_Grade)
										{
											PickupItem.bEquipCode	= 0;
											Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
											memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
											goto lb_exit2;
										}
									}
									else if(nValueSmall == ITEM_LIQUID_INDEX && nValue == ITEM_UPGRADE_INDEX)
									{
										if(pBaseItem->BaseItem_UpGrade.wW_Grade != pBaseItemSmall->BaseItem_LiQuid.wW_Grade)
										{
											PickupItem.bEquipCode	= 0;
											Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));	
											memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
											goto lb_exit2;
										}
									}
								}
							}
							
							if(pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE == ITEM_UPGRADE_INDEX)
							{
								// 유저의 스몰인벤에 돌려놓는다.
								if(nValue == ITEM_UPGRADE_INDEX)
								{
									bChk = TRUE;
									break;
								}	
							}
							if(pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE == ITEM_LIQUID_INDEX)
							{
								// 유저의 스몰인벤에 돌려놓는다.
								if(nValue == ITEM_LIQUID_INDEX)
								{
									bChk = TRUE;
									break;
								}
							}
							if(	pUser->m_pUpgradeItem[i].GetID() / ITEM_DISTRIBUTE == ITEM_EDITION_INDEX)
							{
								// 유저의 스몰인벤에 돌려놓는다.
								if(nValue == ITEM_EDITION_INDEX)
								{
									bChk = TRUE;
									break;
								}	
							}
						}	
					}

					// 유저의 업그레이드 아이템에 추가한다.
					if(bChk==FALSE)
					{
						PickupItem.bEquipCode	= 0;
						Insert_UpgradeItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));
					}
					// 유저의 스몰인벤에 돌려놓는다.
					else
					{
						PickupItem.bEquipCode	= 1;
						Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity(), ITEM_TYPE_SMALL, TRUE);
					}
					memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
					goto lb_exit2;
				}
			}
		}
		break;
	case 68:
		{
			int nValue = GetItemIndex(&pUser->m_pUpgradeItem[pItemPickup->bZipCode]);

			if(pUser->m_pUpgradeItem[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_pUpgradeItem[pItemPickup->bZipCode].GetID())==ITEM_TYPE_SMALL)
			{
				if( nValue == ITEM_UPGRADE_INDEX || nValue == ITEM_LIQUID_INDEX || nValue == ITEM_EDITION_INDEX )
				{											
					PickupItem.bZipCode		= pItemPickup->bZipCode;					
					memcpy(&PickupItem.Item, &pUser->m_pUpgradeItem[pItemPickup->bZipCode], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pUpgradeItem[pItemPickup->bZipCode], sizeof(CItem));										
					memset(&pUser->m_pUpgradeItem[pItemPickup->bZipCode], 0, sizeof(CItem));
					goto lb_exit2;
				}
			}
		}
		break;
	case 69:
		{
//			int		nValue	= GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);
			WORD	wId		= pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

			if(pUser->GetLevel() < 10)
				return;
			
			if(	wId==__ITEM_STATUS_INT__ ||
				(wId>=__ITEM_STATUS_INTSTART__ && wId<=__ITEM_STATUS_INTEND__) ||
				wId==__ITEM_SKILL_INT__ ||
				(wId>=__ITEM_SKILL_INTSTART__ && wId<=__ITEM_SKILL_INTEND__))
			{
				CBaseItem* pBaseItem = g_pBaseItemHash->GetData(wId);

				if(!pBaseItem)
					return;

				BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

				if(byCount>0)
				{
					memset(&ItemLog, 0, sizeof(CItem));
					memcpy(&ItemLog, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));

					if(byCount==1)
						memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
					else
						pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));

					if(wId==__ITEM_STATUS_INT__ || (wId>=__ITEM_STATUS_INTSTART__ && wId<=__ITEM_STATUS_INTEND__))
						pUser->InitStat(pBaseItem->BaseItem_Special.bClassGrade-50);
					else if(wId==__ITEM_SKILL_INT__ || (wId>=__ITEM_SKILL_INTSTART__ && wId<=__ITEM_SKILL_INTEND__))
						pUser->InitSkill(pBaseItem->BaseItem_Special.bClassGrade-50);
					
					goto lb_exit2;
				}					
			}				
		}
		break;
	case 70: 
		{		
			CItem cItem;
							
			PickupItem.bZipCode	= 0;

			if(pItemPickup->bZipCode==__GCTYPE_GUILD__)
			{// 길드 생성시 취소 버튼 눌렀을때 일루 오넹 ;;					
				CreateItem(&cItem, ITEM_GUILD_INDEX, 1);
				memcpy(&PickupItem.Item, &cItem, sizeof(CItem));					
				Insert_SmallInvItem(pUser, &cItem, 0, 1, TRUE);
				goto lb_exit2;
			}
			else if(pItemPickup->bZipCode==__GCTYPE_CLEN__)
			{
				CreateItem(&cItem, ITEM_CLEN_INDEX, 1);
				memcpy(&PickupItem.Item, &cItem, sizeof(CItem));
				Insert_SmallInvItem(pUser, &cItem, 0, 1, TRUE);
				goto lb_exit2;
			}
			else
				return;
		}
		break;
	case 71:	// 바닥에서 벨트로 이동			
		{
 			if(!pLayer->GetSection( pItemPickup->bSectionNum))
				return;

			pItem = pLayer->GetSection(pItemPickup->bSectionNum)->m_pItemHash->GetData(pItemPickup->i64ItemID);
			
			// 이상한 아이템이면
			if(!pItem || pItem->Item.m_wItemID==0)
				return;

			// 복사 아이템 이면
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 71, DEF_COPYITEM_TYPE_SMALL, TRUE))
				goto lb_exit2;

			// 파티풀 드롭 아이템이고 습득자가 파티원이면
			if(pItem->dwPartyIndex != 0 && pUser->GetPartyID() == pItem->dwPartyIndex)
			{
				// 습득 가능
			}
			// 개인 아이템이면
			else if(pItem->dwOwnerIndex)
			{
				// 아이템 소유자가 아니면
				if(pItem->dwOwnerIndex!=pUser->GetID())
				{
					// 습득 불가
					return;	
				}
			}

			// 작은 아이템이면
			if(GetItemType(pItem->Item.m_wItemID)==ITEM_TYPE_SMALL)				
			{
				if(pItem->Item.GetQuantity()==0)
					return;

				CItem cItem;
				memcpy(&cItem,&pItem->Item,sizeof(CItem));

				// 파티플 렌덤 루팅 : 최덕석 2005.1.26
				// 파티플 중이고, 파티가 렌덤루팅 이면
				if(pUser->GetPartyID() && wMemberCount > 1 && pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM)
				{
					// 아이템을 습득할 유저 렌덤하게 변경
					pUser = pMember[ rand() % wMemberCount ];

					// 한칸에 최대로 넣을 수 있는 수를 구한다
					CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->Item.m_wItemID);
					if(!pBaseItem)
						return;
					int nMaxQuantity = pBaseItem->GetMaxQuantity();

					// 벨트의 모든 칸에 대해
					for(int zip = 0;zip < MAX_BELT_POOL; zip++)
					{
						// 넣을 수 있는 칸이면
						if(	(pUser->m_pBelt[zip].m_wItemID == 0)
						|| 	(pUser->m_pBelt[zip].m_wItemID == pItem->Item.m_wItemID
							&& pUser->m_pBelt[zip].m_Item_Supplies.bQuantity + pItem->Item.GetQuantity() < nMaxQuantity))
						{
							break;
						}
					}

					// 벨트가 가득차면
					if(zip == MAX_BELT_POOL)
					{
						// 줍지 못하고 다른 사람에게 기회가 간다
						return;
					}

					pItemPickup->bZipCode = zip;

					// 투두 : 다른 유저들에게 메세지를 날려줌
					// "누가 뭘 몇개 먹었습니다."
				}

#if defined TAIWAN_LOCALIZING
				// 대만은 무게 제한 100% 이상시 아이템을 집을수 없게한다.
				if (FALSE == pUser->IsPickUpItem()) 	
					return;
#endif

				BYTE bItemCnt = Insert_SmallBeltItem(pUser
					, &pItem->Item
					, pItemPickup->bZipCode
					, pItem->Item.GetQuantity()
					, FALSE);

				if(bItemCnt>0)
				{
					pItem->Item.SetQuantity(bItemCnt);
					bAppear = FALSE;
				}

				PickupItem.bZipCode	= pItemPickup->bZipCode;
				memcpy(&pItem->Item,&cItem,sizeof(CItem));
				memcpy(&PickupItem.Item, &pItem->Item, sizeof(CItem));

				// 아이템 로그 추가 김영대 2005.02.23
				SetItemLog(&PickupItem.Item,ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );

				goto lb_exit1;
			}
			// 아니면
			else
			{
				return;
			}
		}
		break;
	case 72:	// PlayerShop에서 살때    
		{							
			CUser* pShopUser = g_pUserHash->GetData(pItemPickup->dwMoney);
			
			if(!pShopUser)
			{					
				pShopPacket.byType	= 7;
				goto lb_shop;
			}

			ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pShopUser->GetID());

			if(pItemShop->byType!=1)
			{					
				pShopPacket.byType	= 8;				
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()==0)
			{					
				pShopPacket.byType	= 9;										
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()!=pItemPickup->dwId)
			{
				pShopPacket.byType	= 10;				
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode]!=pItemPickup->dwShop)
			{
				pShopPacket.byType	= 19;
				goto lb_shop;
			}

			if(SetIsCopyItem(pUser
				, &PickupItem
				, &pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode]
				, 72
				, DEF_COPYITEM_TYPE_LARGE))
			{
				goto lb_exit2;
			}
			
			__int64	nMoney		= (__int64)pUser->m_dwMoney;
			__int64	nPrice		= (__int64)pShopUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode];

			if( (nMoney - nPrice) >=0 )
			{
				__int64	nShopMoney	= pShopUser->m_dwMoney;

				if(nShopMoney + nPrice <= __MAX_MONEY__)
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].GetID() == 0)
						{						
							CItem cItem;
							memset(&cItem, 0, sizeof(CItem));

							PlayerShopPriceLog(pShopUser
								, &cItem
								, 72
								, pItemPickup->bZipCode
								, DWORD(nPrice)
								, 0
								, TRUE
								, pUser->GetID()
								, DWORD(nMoney-nPrice)
								, i
								, pItemPickup->bZipCode);

							return;								
						}
					}

					pShopPacket.byType	= 13;
					goto lb_shop;
				}
				else
				{						
					pShopPacket.byType	= 12;
					goto lb_shop;
				}
			}
			else
			{
				pShopPacket.byType	= 11;
				goto lb_shop;
			}				
		}
		break;
	case 73:
		{
			CUser* pShopUser = g_pUserHash->GetData(pItemPickup->dwMoney);
			
			if(!pShopUser)
			{					
				pShopPacket.byType	= 7;
				goto lb_shop;
			}

			ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pShopUser->GetID());

			if(pItemShop->byType!=1)
			{					
				pShopPacket.byType	= 8;				
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()==0)
			{					
				pShopPacket.byType	= 9;										
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetID()!=pItemPickup->dwId)
			{
				pShopPacket.byType	= 10;				
				goto lb_shop;
			}

			if(pShopUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode]!=pItemPickup->dwShop)
			{
				pShopPacket.byType	= 19;
				goto lb_shop;
			}

			if(pItemPickup->bZipCode2 == 0)
			{
				pShopPacket.byType	= 21;
				goto lb_shop;
			}

			if(SetIsCopyItem(pUser, &PickupItem, &pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode], 73, DEF_COPYITEM_TYPE_SMALL))
				goto lb_exit2;

			__int64	nMoney		= (__int64)pUser->m_dwMoney;
			__int64	nPrice		= (__int64)pShopUser->m_sPlayerShop.dwCustomSellPrice[pItemPickup->bZipCode] * pItemPickup->bZipCode2;

			if((nMoney-nPrice)>=0)
			{
				__int64	nShopMoney	= pShopUser->m_dwMoney;

				if(nShopMoney+nPrice<=__MAX_MONEY__)
				{						
					CItem cItem[MAX_INV_SMALL_POOL];
					CItem cInsertItem;

					for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						memcpy(&cItem[i], &pUser->m_pInv_Small[i], sizeof(CItem));
					}

					if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetQuantity()<pItemPickup->bZipCode2)
					{
						pItemPickup->bZipCode2 = pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetQuantity();
					}

					memcpy(&cInsertItem, &pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode], sizeof(CItem));
					cInsertItem.SetQuantity(pItemPickup->bZipCode2);												

					if(Insert_SmallInvItem2(cItem, &cInsertItem, 0, cInsertItem.GetQuantity(), TRUE)==0)
					{
						CItem cPriceItem;

						if(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetQuantity()==pItemPickup->bZipCode2)
						{
							memset(&cPriceItem, 0, sizeof(CItem));
						}
						else
						{
							memcpy(&cPriceItem, &pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode], sizeof(CItem));
							cPriceItem.SetQuantity(
								BYTE(pShopUser->m_sPlayerShop.cItem[pItemPickup->bZipCode].GetQuantity()-pItemPickup->bZipCode2));
						}								
						
						PlayerShopPriceLog(pShopUser
							, &cPriceItem
							, 73
							, pItemPickup->bZipCode
							, DWORD(nPrice)
							, 0
							, TRUE
							, pUser->GetID()
							, DWORD(nMoney-nPrice)
							, pItemPickup->bZipCode
							, pItemPickup->bZipCode2);

						return;							
					}
					
					pShopPacket.byType	= 13;
					goto lb_shop;
				}
				else
				{						
					pShopPacket.byType	= 12;
					goto lb_shop;
				}
			}
			else
			{
				pShopPacket.byType	= 11;
				goto lb_shop;
			}				
		}
		break;
	case 74:
		{
			CItem* pItem  = NULL;
			
			if(pItemPickup->bEquipCode==0)
			{
				if(pItemPickup->bZipCode>=MAX_INV_SMALL_POOL)
					return;

				int nValue = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()/ITEM_DISTRIBUTE;
				
				if(nValue<ITEM_CONSUMABLEST_INDEX || nValue>ITEM_CONSUMABLEEND_INDEX)
					return;

				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(pUser->m_pInv_Small[i].GetID()==pUser->m_pInv_Small[pItemPickup->bZipCode].GetID())
					{
						if(pUser->m_pInv_Small[i].m_Item_Consumable.bInvIndex>=1)							
							return;
					}

					// 같은 기능을 가진 아이템이면
					if(IsSamePropertyConsumableItem(&pUser->m_pInv_Small[i], &pUser->m_pInv_Small[pItemPickup->bZipCode]))
					{
						if(pUser->m_pInv_Small[i].m_Item_Consumable.bInvIndex>=1)							
							return;
					}
				}

				pItem = &pUser->m_pInv_Small[pItemPickup->bZipCode];
			}
			else
				return;
			
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());
			if (pItem->m_Item_Consumable.bInvIndex || pUser->GetLevel() > pBaseItem->BaseItem_Consumable.wMax_Lev)
			{
				return;
			}		
			
			pItem->m_Item_Consumable.dwEndDate = GetDueDay(pBaseItem->BaseItem_Consumable.wLast_Time);
			pItem->m_Item_Consumable.bInvIndex = pItemPickup->bZipCode+1;
			pUser->AttachItem(pItem);
			
			//== ==//			
			PickupItem.bZipCode		= pItemPickup->bZipCode;
			PickupItem.bEquipCode	= pItemPickup->bEquipCode;
			memcpy(&PickupItem.Item, pItem, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 75:	// 길드 리콜 
		{
			int nValue = GetItemIndex(&pUser->m_pInv_Small[pItemPickup->bZipCode]);

			if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(!pUser->GetCurDungeon())
				return;
			
			if(pUser->IsCurDungeonSiege())
				return;				

			if(nValue==ITEM_MAGICARRAY_INDEX)
			{
				WORD wItemId = pUser->m_pInv_Small[pItemPickup->bZipCode].GetID();

				if(wItemId==__ITEM_GUILD_RECALL_INDEX__)
				{						
					// 월드 서버로 보낸다 //
					WSTDS_GUILD_ALLPORTAL pSendPacket;						
					pSendPacket.dwGuildId	= pUser->m_dwGuildId;						
					pSendPacket.dwUserIndex	= pUser->GetID();
					pSendPacket.byIndex		= pItemPickup->bZipCode;
					
					g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
											
					if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()>1)
						pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(
							BYTE(pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity()-1));
					else
						memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));

					PickupItem.bZipCode		= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));
					goto lb_exit2;
				}
				else
				{
					return;
				}
			}
		}
		break;
	case 76:
		{
			CUser* pSendUser = g_pUserHash->GetData(pItemPickup->dwMoney);

			if(pSendUser)
			{
				if(!pSendUser->GetCurDungeon() || !pUser->GetCurDungeon())
				{
					// 실패 //
					DSTC_GUILD_PORTALFAIL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 2;
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 2;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				if(pSendUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID == 
					pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID)
				{
					if(pSendUser->GetCurLayerIndex()==pUser->GetCurLayerIndex())
					{
						// 실패 //
						DSTC_GUILD_PORTALFAIL pSendPacket;
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						pSendPacket.byResult = 5;
						NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						
						memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
						__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
						pSendPacket.byResult = 5;
						NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
						return;
					}
				}
				
/*
				// Item Chk //
				if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
					pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
				{*/

					// 포탈 이동 메세지 //
					DSTC_PARTY_PORTALMOVE pSendPacket;
					pSendPacket.wDungeonID		= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
					pSendPacket.byLayerIndex	= pUser->GetCurLayerIndex();
					pSendPacket.dwPartyId		= pUser->m_dwPartyId;
					pSendPacket.fPosX			= pUser->GetCurPosition()->x;
					pSendPacket.fPosZ			= pUser->GetCurPosition()->y;						
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION );
					
					// 아이템 메세지 //						
					if(pItemPickup->bZipCode2==1)
					{
						BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
							pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
						{
							if(byCount>1)
								pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
							else
								memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
					}
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));												

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}

					goto lb_exit2;
/*
				}
				else
				{
					// 실패 //
					DSTC_PARTY_PORTALFAIL pSendPacket;
					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 3;
					NetSendToUser(pSendUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

					memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
					__lstrcpyn(pSendPacket.szName, pSendUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendPacket.byResult = 3;
					NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}*/

			}
			else
			{
/*
				// Item Chk //
				if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
					pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
				{*/

					// 월드 포탈 이동 메세지 //
					WSTDS_PARTY_PORTALMOVE pSendPacket;
					pSendPacket.wDungeonID			= WORD(pUser->GetCurDungeon()->GetDungeonDataEx()->m_dwID);
					pSendPacket.byLayerIndex		= pUser->GetCurLayerIndex();
					pSendPacket.dwPartyId			= pUser->m_dwPartyId;
					pSendPacket.fPosX				= pUser->GetCurPosition()->x;
					pSendPacket.fPosZ				= pUser->GetCurPosition()->y;
					pSendPacket.dwUserIndex			= pUser->GetID();
					pSendPacket.dwPortalUserIndex	= pItemPickup->dwMoney;
					g_pNet->SendToServer(WSINDEX, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					
					// 아이템 메세지 //
					if(pItemPickup->bZipCode2==1)
					{
						BYTE byCount = pUser->m_pInv_Small[pItemPickup->bZipCode].GetQuantity();

						if(	pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL1__ ||
							pUser->m_pInv_Small[pItemPickup->bZipCode].GetID()==__ITEM_PARTY_PORTAL2__)
						{
							if(byCount>1)
								pUser->m_pInv_Small[pItemPickup->bZipCode].SetQuantity(BYTE(byCount-1));
							else
								memset(&pUser->m_pInv_Small[pItemPickup->bZipCode], 0, sizeof(CItem));
						}
					}
					
					PickupItem.bZipCode	= pItemPickup->bZipCode;
					memcpy(&PickupItem.Item, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));												

					CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(PickupItem.Item.m_Item_Zodiac.wMapId);

					if(pDungeon)
						goto lb_exit2;

					CheckItemSlot(pUser);
					
					if(pUser->m_ItemGuardian.GetID()!=0)
					{
						Insert_SmallInvItem(pUser, &pUser->m_ItemGuardian, 0, pUser->m_ItemGuardian.GetQuantity(), TRUE);
						memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));
					}
					goto lb_exit2;
/*
				}
				else
				{
					// 실패 //
					WSTDS_GUILD_PORTALFAIL pSendServerPacket;						
					memset(pSendServerPacket.szName, 0, sizeof(pSendServerPacket.szName));
					__lstrcpyn(pSendServerPacket.szName, pUser->m_szName, MAX_CHARACTER_REAL_LENGTH);
					pSendServerPacket.byResult		= 3;
					pSendServerPacket.dwUserIndex	= pItemPickup->dwMoney;
					g_pNet->SendToServer(WSINDEX, (char*)&pSendServerPacket, pSendServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}*/

			}
		}
		break;
	case 77:	//영웅의 증표 아이템 //태극기 아이템
		{
			CItem* pInv=0;

			if(pItemPickup->bEqiupCode2 == 1 )
				pInv = &pUser->m_pBelt[pItemPickup->bZipCode];
			else
				pInv = &pUser->m_pInv_Small[pItemPickup->bZipCode];

			int nValue = pInv->m_wItemID/ITEM_DISTRIBUTE;
			
			if(nValue==0)
				return;

			//  선물상자  김영대 2005.02.02
			if(pInv->m_wItemID ==  __ITEM_HEROITEM_INDEX__)
			{
				WORD wItemQuantity=0;
				WORD wPresentID = UnpackPresentBox(pInv, &wItemQuantity);

				if(!wPresentID)	//선물상자 풀기 실패 
				{
					DSTC_ITEM_PICKUP	Error;
					memset( &Error, 0, sizeof( DSTC_ITEM_PICKUP ) );
					Error.bStatus	= UPDATE_GAME_PLAY;
					Error.bHeader	= Protocol_DSTC::CMD_ITEM_PICKUP;
					Error.bInv = 78;
					Error.bEqiupCode2 = 252;	//선물상자 풀기 실패!!
					Log(LOG_IMPORTANT, "Pre  : Error Code 252 ");
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&Error, Error.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
				
				DWORD dwEmpty=0;
				DWORD dwEmptyPos=0;
				nValue = wPresentID / ITEM_DISTRIBUTE;

				if(GetItemType(wPresentID)==ITEM_TYPE_SMALL)
				{
//					dwEmpty = 0;
					for(int i=0; i<MAX_INV_SMALL_POOL; i++)
					{
						if(pUser->m_pInv_Small[i].GetID() == 0) 
						{
							dwEmpty++;
							dwEmptyPos = i;
							break;
						}
					}
				}
				else if(GetItemType(wPresentID)==ITEM_TYPE_LARGE)
				{
//					dwEmpty = 0;
					for(int i=0; i<MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].GetID() == 0) 
						{
							dwEmpty++;
							dwEmptyPos = i;
							break;
						}
					}
				}

				if(!dwEmpty)	//인벤토리에 빈칸이 없음 
				{
					DSTC_ITEM_PICKUP	Error;
					memset( &Error, 0, sizeof( DSTC_ITEM_PICKUP ) );
					Error.bStatus	= UPDATE_GAME_PLAY;
					Error.bHeader	= Protocol_DSTC::CMD_ITEM_PICKUP;
					Error.bInv = 78;
					Error.bEqiupCode2 = 253;	//빈칸없음!!
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&Error, Error.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}

				char szQuery[0xff]={0,};
				wsprintf(szQuery, "CZP_CHECK_PRESENT_COUNT %d,%d,%d", wPresentID, wItemQuantity, dwEmptyPos);
				g_pDb->THOpenRecord(szQuery, DB_PRESENT_COUNT_CHECK, (void*)pUser, 1, GAME_DB);


				int nCount = pInv->GetQuantity();

				if(nCount>1)
					pInv->SetQuantity(BYTE(nCount-1));
				else if(nCount==1)
					memset(pInv, 0, sizeof(CItem));

				PickupItem.bInv		= 77;
				PickupItem.bZipCode	= pItemPickup->bZipCode;
				PickupItem.bEqiupCode2 = pItemPickup->bEqiupCode2;
				memcpy(&PickupItem.Item, pInv, sizeof(CItem));
	
				// 아이템 로그 추가 김영대 2005.02.23
				SetItemLog(&PickupItem.Item, ITEM_LOG_SEAL_OF_HERO, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				goto lb_exit2;
			}
		}
		break;		
	case 79:
		{
			int nValue = GetItemIndex(&pUser->m_ItemMouse);

			if(pUser->m_ItemMouse.GetQuantity()==0)
				return;
			
			if(pUser->m_pMakingItem[pItemPickup->bZipCode].GetID()!=0)			
				return;

			if(GetItemType(pUser->m_ItemMouse.GetID())==ITEM_TYPE_SMALL)
			{
				if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 79, DEF_COPYITEM_TYPE_SMALL, TRUE))
					goto lb_exit2;									
			
				if(	(nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX) ||
					pUser->m_ItemMouse.GetID() == __ITEM_MAKING_INDEX__ )
				{					
					PickupItem.bZipCode		= pItemPickup->bZipCode;
					PickupItem.dwMoney		= pItemPickup->dwMoney;
					memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));

					// 유저의 제조 아이템에 추가한다.
					PickupItem.bEquipCode	= 0;
					Insert_MakingItem(pUser, &pUser->m_ItemMouse, pItemPickup->bZipCode, BYTE(pItemPickup->dwMoney));
				}						
				// 유저의 스몰인벤에 돌려놓는다.
				else
				{
					PickupItem.bEquipCode	= 1;
					Insert_SmallInvItem(pUser, &pUser->m_ItemMouse, 0, pUser->m_ItemMouse.GetQuantity()
						, ITEM_TYPE_SMALL, TRUE);
				}

				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));
				goto lb_exit2;					
			}
		}
		break;
	case 80:
		{
			int nValue = GetItemIndex(&pUser->m_pMakingItem[pItemPickup->bZipCode]);

			if(pUser->m_pMakingItem[pItemPickup->bZipCode].GetQuantity()==0)
				return;

			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_pMakingItem[pItemPickup->bZipCode].GetID())==ITEM_TYPE_SMALL)
			{
				if( (nValue >= ITEM_MATERIALST_INDEX && nValue <= ITEM_MATERIALEND_INDEX) ||
					pUser->m_pMakingItem[pItemPickup->bZipCode].GetID() == __ITEM_MAKING_INDEX__)
				{											
					PickupItem.bZipCode		= pItemPickup->bZipCode;					
					memcpy(&PickupItem.Item, &pUser->m_pMakingItem[pItemPickup->bZipCode], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pMakingItem[pItemPickup->bZipCode], sizeof(CItem));										
					memset(&pUser->m_pMakingItem[pItemPickup->bZipCode], 0, sizeof(CItem));
					goto lb_exit2;
				}
			}
		}
		break;
	case 81:// 아이템생성창에서 마우스로 이동 
		{
			int nValue = GetItemIndex(&pUser->m_pMakingItem[0]);

			if(pUser->m_ItemMouse.GetID()!=0)
				return;
			
			if(GetItemType(pUser->m_pMakingItem[0].GetID())==ITEM_TYPE_LARGE)
			{
				// 가방아이템 생성시 아이템이 안떨어지는 현상 수정
				// 2005.01.25 김영대  nValue == ITEM_BAG_INDEX 추가
				if(	(nValue>=ITEM_WEAPONST_INDEX && nValue<= ITEM_WEAPONEND_INDEX) ||
					( nValue>=ITEM_ARMORST_INDEX && nValue<= ITEM_ARMOREND_INDEX)  ||
					nValue == ITEM_RIDE_INDEX || 
					nValue == ITEM_GUARDIAN_INDEX ||
					nValue == ITEM_BAG_INDEX ) 
				{											
					PickupItem.bZipCode		= 0;					
					memcpy(&PickupItem.Item, &pUser->m_pMakingItem[0], sizeof(CItem));
					memcpy(&pUser->m_ItemMouse, &pUser->m_pMakingItem[0], sizeof(CItem));										
					memset(&pUser->m_pMakingItem[0], 0, sizeof(CItem));
					goto lb_exit2;
				}
			}				
		}
		break;
	//case 82: 사용중
	case 83:	// 가디언 아이템 상점에 내다 팔때 
		{
			if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 34, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;
					
			if(nValue==ITEM_GUARDIAN_INDEX)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);

				if(!pBaseItem)
					break;
				
				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nSellPrice	= pBaseItem->BaseItem_Guardian.dwSellPrice;

				if(nMoney+nSellPrice<=__MAX_MONEY__)
				{
					pUser->m_dwMoney		= DWORD(nMoney+nSellPrice); 
					PickupItem.dwMoney		= DWORD(nMoney+nSellPrice);	
					PickupItem.bEquipCode	= 0;						
					bChk = TRUE;
				}
				else
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							if(bChk==FALSE)
							{								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;

								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								bChk = TRUE;
							}
						}
					}
				
					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
						PickupItem.bEquipCode = 2;
					}
				}
				
				// 마우스에서 삭제 //
				memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(bChk==TRUE)
					goto lb_exit2;
			}
		}
		break;
	case 84:	// 가방 아이템 상점에 팔때 
		{
			if(pUser->m_ItemMouse.m_wItemID==0)
				return;

			int nValue = GetItemIndex(&pUser->m_ItemMouse);
			
			if(SetIsCopyItem(pUser, &PickupItem, &pUser->m_ItemMouse, 34, DEF_COPYITEM_TYPE_LARGE, TRUE))
				goto lb_exit2;
					
			if(nValue==ITEM_BAG_INDEX)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pUser->m_ItemMouse.m_wItemID);

				if(!pBaseItem)
					break;
				
				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nSellPrice	= pBaseItem->BaseItem_Bag.dwSellPrice;

				if(nMoney+nSellPrice<=__MAX_MONEY__)
				{
					pUser->m_dwMoney		= DWORD(nMoney+nSellPrice); 
					PickupItem.dwMoney		= DWORD(nMoney+nSellPrice);	
					PickupItem.bEquipCode	= 0;						
					bChk = TRUE;
				}
				else
				{
					for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
					{
						if(pUser->m_pInv_Large[i].m_wItemID==0)
						{
							if(bChk==FALSE)
							{								
								PickupItem.bZipCode		= i;
								PickupItem.bEquipCode	= 1;

								memcpy(&pUser->m_pInv_Large[i], &pUser->m_ItemMouse, sizeof(CItem));
								memcpy(&PickupItem.Item, &pUser->m_ItemMouse, sizeof(CItem));
								bChk = TRUE;
							}
						}
					}
				
					if(bChk==FALSE)
					{
						AppearItem(pUser, &pUser->m_ItemMouse, 0, 0, ITEM_TYPE_LARGE);
						PickupItem.bEquipCode = 2;
					}
				}
				
				// 마우스에서 삭제 //
				memcpy( &ItemLog, &pUser->m_ItemMouse, sizeof( CItem ) );
				memset(&pUser->m_ItemMouse, 0, sizeof(CItem));

				if(bChk==TRUE)
					goto lb_exit2;
			}
		}
		break;	
	case 85: // 가방 상점에 살때  
		{
			if(pItemPickup->dwId >= 3600 && pItemPickup->dwId <= 3618)				
			{
				BOOL		bChk		= FALSE;
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItemPickup->dwId);
				
				if(!pBaseItem)
					break;

				if(!CheckStore(pItemPickup->dwId))
					return;

				__int64		nMoney		= pUser->m_dwMoney;
				__int64		nPrice		= pBaseItem->BaseItem_Bag.dwPrice;

				if(nMoney-nPrice>=0)
				{
					if(pUser->m_pInv_Large[pItemPickup->bZipCode].m_wItemID==0)
					{
						CreateItem(&pUser->m_pInv_Large[pItemPickup->bZipCode], WORD(pItemPickup->dwId), 0, Store_Flag_In_Store);
						memcpy(&PickupItem.Item, &pUser->m_pInv_Large[pItemPickup->bZipCode], sizeof(CItem));
						pUser->m_dwMoney = DWORD(nMoney - nPrice);
						
						PickupItem.bEquipCode	= 1;
						PickupItem.bZipCode		= pItemPickup->bZipCode;
						PickupItem.dwId			= pItemPickup->dwId;
						PickupItem.dwMoney		= pUser->m_dwMoney;
						
						bChk = TRUE;
					}
					else
					{						
						for(BYTE i = 0; i < MAX_INV_LARGE_POOL; i++)
						{
							if(pUser->m_pInv_Large[i].m_wItemID==0)
							{
								if(bChk==FALSE)
								{
									CreateItem(&pUser->m_pInv_Large[i], WORD(pItemPickup->dwId), 0, Store_Flag_In_Store);
									memcpy(&PickupItem.Item, &pUser->m_pInv_Large[i], sizeof(CItem));
									pUser->m_dwMoney = DWORD(nMoney - nPrice);
									bChk = TRUE;
									
									PickupItem.bZipCode		= i;
									PickupItem.bEquipCode	= 1;
									PickupItem.dwId			= pItemPickup->dwId;
									PickupItem.dwMoney		= pUser->m_dwMoney;
								}
							}
						}
					}
				
					if(bChk==FALSE)
					{
						// 실패 //						
						PickupItem.bEquipCode	= 2;
						PickupItem.dwId			= pItemPickup->dwId;
					}						
				}
				else
				{
					// 실패 //
					PickupItem.bEquipCode	= 3;
					PickupItem.dwId			= pItemPickup->dwId;
				}
				
				goto lb_exit2;
			}				
		}
		break;
	case 100:			// 아이템 보험 
		{
			CItem* pItem  = NULL;

			if(pItemPickup->bEquipCode==0)
			{
				if(pItemPickup->bZipCode>=MAX_INV_SMALL_POOL)
					return;

				if(pUser->m_pInv_Small[pItemPickup->bZipCode].GetID() != __ITEM_INSURANCE_INDEX__)
					return;					

				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(pUser->m_pInv_Small[i].GetID()==pUser->m_pInv_Small[pItemPickup->bZipCode].GetID())
					{
						if(pUser->m_pInv_Small[i].m_Item_Zodiac.bInvIndex>=1)							
							return;
					}						
				}

				pItem = &pUser->m_pInv_Small[pItemPickup->bZipCode];
			}
			else 
				return;


			pItem->m_Item_Zodiac.bInvIndex = pItemPickup->bZipCode+1;
			pUser->AttachItem(pItem);

			memset(&ItemLog, 0, sizeof(CItem));
			memcpy(&ItemLog, &pUser->m_pInv_Small[pItemPickup->bZipCode], sizeof(CItem));

						
			PickupItem.bZipCode		= pItemPickup->bZipCode;
			PickupItem.bEquipCode	= pItemPickup->bEquipCode;
			memcpy(&PickupItem.Item, pItem, sizeof(CItem));
			goto lb_exit2;
		}
		break;
	case 101:
		{
			int nValue = GetItemIndex(&pUser->m_ItemGuardian);

			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pUser->m_ItemGuardian.m_wItemID);

			if(!pBaseItem)
				return;		
			
			if (!pUser->m_pInv_Guardian[0].GetID())
				return;	
			
			if(nValue >= ITEM_CONSUMABLEST_INDEX && nValue <= ITEM_CONSUMABLEEND_INDEX)
			{
				if(pUser->m_ItemGuardian.GetID() >= 20519 && pUser->m_ItemGuardian.GetID() <= 20578)
				{
					int nCount = pUser->m_ItemGuardian.GetQuantity();

					if(nCount==0)
						return;	

					CMonster* pMonster = pUser->GetGuardian(0);
					if(pMonster)
					{
						for(int i = 0; i < MAX_BASEITEM_CONSUMABLE_ITEMATTR; ++i)
						{
							if(pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code)
							{
								pMonster->SetItemAttr(
													  (ITEM_ATTR_CODE)pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code
													, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value
													);

								DWORD dwVirtualCode = 
									g_pItemAttrLayer->GetVirtualCode(
															  pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code
															, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value
															);

								// formula 6을 은 스킬레벨 올려주는것이기 때문에..
								if (g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].wFormula == 6)
								{
									pMonster->SetSkillLevel(
														  g_pItemAttrLayer->m_ItemAttrDefine[dwVirtualCode].bSkillID
														, pMonster->GetItemAttr((ITEM_ATTR_CODE)dwVirtualCode)
														);
								}
							}
						}
						
						if(nCount>1)
							pUser->m_ItemGuardian.SetQuantity(BYTE(nCount-1));
						else if(nCount==1)
							memset(&pUser->m_ItemGuardian, 0, sizeof(CItem));				

						memcpy(&PickupItem.Item, &pUser->m_ItemGuardian, sizeof(CItem));
						goto lb_exit2;
					}
				}
			}
		}
		break;
	}
	return;

lb_exit1: 	// Item DisAppear 를 포함한다.

	// 줍기를 실패했다면.
	if( PickupItem.bInv == 0 )
	{
		return;	
	}

	memcpy( &PickupItem.Item, &pItem->Item, sizeof( CItem ) );
	NetSendToUser( pUser->GetConnectionIndex(), (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	// 아이템이 없어진것을 알려야 한다.
	if(bAppear)
	{
		DisappearItem.i64ItemID = pItem->Item.m_Serial.i64Serial;
		pLayer->BroadCastSectionMsg( (char*)&DisappearItem, DisappearItem.GetPacketSize(), pItemPickup->bSectionNum );
	
		// 마지막으로 아이템을 삭제한다.
		if( pLayer->RemoveItem( pItem ) == false ) __asm int 3;
	}

	CheckSpeed(pUser);
	
	goto lb_Item_log;

lb_exit2:

	// 줍기를 실패했다면.
	if( PickupItem.bInv == 0 )
	{
		return;	
	}

	NetSendToUser( pUser->GetConnectionIndex(), (char*)&PickupItem, PickupItem.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	CheckSpeed(pUser);
	
	goto lb_Item_log;

lb_shop:
	
	NetSendToUser( pUser->GetConnectionIndex(), (char*)&pShopPacket, pShopPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

lb_Item_log:
	
	int  iCount = 0;

	// 줍기를 실패했다면.
	if( PickupItem.bInv == 0 )
	{
		return;	
	}

	switch( PickupItem.bInv )
	{
	// 증가.---------------------------------------------------------------------------------------
	case 1:	// 바닥에서 큰아이템 창으로.
		{
//			int nValue = pItem->Item.m_wItemID/ITEM_DISTRIBUTE;

			if(GetItemType(pItem->Item.m_wItemID)==ITEM_TYPE_MONEY)
			{
				if(g_pThis->GetServerType()==SERVER_TYPE_VILLAGE)
				{
					SetItemLogKarz(	ITEM_LOG_KARZ_PICKUP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItem->Item.m_Item_Money.dwMoney);
				}
				else
				{
					if(pItem->Item.m_Item_Money.dwMoney >= 10000)
					{
						SetItemLogKarz(	ITEM_LOG_KARZ_PICKUP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItem->Item.m_Item_Money.dwMoney);
					}
				}				
			}
			else	//kukza 041004
			{
				if( pItem ) // 일단 코드를 잘 몰라서..
				{
					SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), pItem->Item.GetQuantity() );
					SetItemLog( &pItem->Item, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				}
				else
				{
					SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), PickupItem.Item.GetQuantity() );
					SetItemLog( &PickupItem.Item, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				}
			}
		}
		break;
	case 2:	// 바닥에서 작은아이템 창으로.
	case 5: // 바닥에서 마우스로 이동.
	case 12: // 작은 아이템을 바닥에서 마우스으로 이동할떄.
		{
			if( pItem ) // 일단 코드를 잘 몰라서..
			{
				SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), pItem->Item.GetQuantity() );
				SetItemLog( &pItem->Item, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
			}
			else
			{
				SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), PickupItem.Item.GetQuantity() );
				SetItemLog( &PickupItem.Item, ITEM_LOG_PICKUP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
			}
		}
		break;
	
	case 19: // 아이템상점에 아이템을 살떄.
	case 35: // 무기 아이템 살떄.
	case 37: // 방어구 아이템 살떄.
		{
			if( pItem ) // 일단 코드를 잘 몰라서..
			{
				SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), pItem->Item.GetQuantity() );
				SetItemLog( &pItem->Item, ITEM_LOG_CREATE_BY_SHOP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
			}
			else
			{
				SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), PickupItem.Item.GetQuantity() );
				SetItemLog( &PickupItem.Item, ITEM_LOG_CREATE_BY_SHOP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
			}
		}
		break;
 
	// 감소.---------------------------------------------------------------------------------------	
	case 28: // 마우스에서 나누기 버든으로 바닥에 떨어질떄.		//kukza 041004
	case 10: // 아이템을 바닥으로 이동.
		{
			iCount = pUser->m_ItemMouse.GetQuantity() * -1;
			SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &ItemLog, ITEM_LOG_DROP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;
 
	case 18: // 아이템상점에 아이템을 팔떄.
	case 34: // 무기 아이템 팔떄.
	case 36: // 방어구 아이템 팔떄.
	case 83: // 가디언 아이템 팔때.
		{
			if( pItem ) // 일단 코드를 잘 몰라서..
			{
				iCount = ItemLog.GetQuantity() * -1;
				SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
				SetItemLog( &ItemLog, ITEM_LOG_DELETE_BY_SHOP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				if(ItemLog.GetID() != 0)
				{
					CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(ItemLog.GetID());
					if(pBaseItem)
					{
						DWORD dwSellPrice = pBaseItem->GetSellPrice();
						SetItemLogKarz(	ITEM_LOG_DELETE_BY_SHOP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,dwSellPrice,&ItemLog);
					}					
				}
			}
			else
			{
				iCount = ItemLog.GetQuantity() * -1;
				SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
				SetItemLog( &ItemLog, ITEM_LOG_DELETE_BY_SHOP, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
				if(ItemLog.GetID() != 0)
				{
					CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(ItemLog.GetID());
					if(pBaseItem)
					{
						DWORD dwSellPrice = pBaseItem->GetSellPrice();
						SetItemLogKarz(	ITEM_LOG_DELETE_BY_SHOP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,dwSellPrice,&ItemLog);
					}					
				}
			}
		}
		break;

//	case 28: // 마우스에서 나누기 버든으로 바닥에 떨어질떄. 
//		break;
	case 13: // 돈을 버릴떄.
		{
			if( pItem ) // 일단 코드를 잘 몰라서..
			{
				iCount = pItem->Item.GetQuantity() * -1;
				SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			}
			else
			{
				iCount = PickupItem.Item.GetQuantity() * -1;
				SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );

				SetItemLogKarz(	ITEM_LOG_KARZ_DROP,pLayer->GetParent()->GetID(),pUser->m_szName,NULL,pItemPickup->dwMoney);
			}
		}
		break;

	case 20: // 아이템 창에서 물약을 사용할떄.
	case 21: // 아이템 발트에서 물약을 사용할떄.
	case 30: // 길드 생성 아이템 사용.
	case 44: // 가디언에게 물약 사용.
	case 51: // 조디악 아이템 사용.
	case 52: // 귀환 스크롤 사용.
	case 53: // 벨트에서 귀환 스크롤 사용.
	case 55: // 벨트에서 조디악 카드 사용.
	case 56: // 파티 포탈 메시지.
//	case 29: // 마우스에서 나누기 버튼으로 아이템창으로 떨어질떄.
		iCount = -1;
		if( pItem )
		{
			SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &pItem->Item, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		else
		{
			SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &PickupItem.Item, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;

	case 69: // 스킬포메터 및 파워오브홀러 사용시
		{
			iCount = -1;
			SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &ItemLog, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;

	case 74: // 우클릭으로 마일리지몰 아이템 사용
		iCount = -1;
		if( pItem )
		{
			SetItemCount( pItem->Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &pItem->Item, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		else
		{
			SetItemCount( PickupItem.Item.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &PickupItem.Item, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;
	case 100:
		{
			iCount = -1;
			SetItemCount( ItemLog.m_wItemID, pLayer->GetParent()->GetID(), iCount );
			SetItemLog( &ItemLog, ITEM_LOG_USED, pLayer->GetParent()->GetID(), pUser->m_szName, NULL );
		}
		break;		
	}
}


void CmdItemUsed( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_ITEM_USED*		pItemUsed = (CTDS_ITEM_USED*)pMsg;
	
	DSTC_ITEM_PICKUP	ItemPickUP;
	DSTC_USER_STATUS	UserStatus;
	DWORD				dwRand;
	float				fPer;
	CItem*				pItem;		// 아이템의 주소를 가지고있을것.	
	CBaseItem*			pBaseItem;
	CUser*				pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if( !pUser )
		return;

	if ( !pUser->GetCurDungeon() )
		return;

	if( pUser->GetUnitStatus() == UNIT_STATUS_DEAD || pUser->GetUnitStatus() == UNIT_STATUS_CASTING )
		return;

	if(CheckShop(pUser))
		return;

	if(pUser->m_byRest==1)
		return;
	
	switch( pItemUsed->bInv )
	{
	case 1:	//small inv...
		ItemPickUP.bInv		= 20;

		if( pItemUsed->bZipCode >= MAX_INV_SMALL_POOL )
			return;

		pItem = &pUser->m_pInv_Small[ pItemUsed->bZipCode ];

		break;

	case 2:	//belt...
		ItemPickUP.bInv		= 21;

		if( pItemUsed->bZipCode	>= MAX_BELT_POOL )
			return;

		pItem = &pUser->m_pBelt[ pItemUsed->bZipCode ];

		break;

	default:
		return;
	}

	switch( (pItem->m_wItemID / ITEM_DISTRIBUTE) )
	{
	case 50: // 물약.
		{ 
			UserStatus.bStatusMany = 0;

			int iItemDelay = g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Delay
				+ g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Delay;
			
			if( g_dwTickCount - pUser->m_dwTemp[USER_TEMP_LAST_ITEM_CASTING] < (DWORD)iItemDelay )
			{
				return;
			}
			
			pUser->m_dwTemp[USER_TEMP_LAST_ITEM_CASTING] = g_dwTickCount;
			pBaseItem = g_pBaseItemHash->GetData( pItem->m_wItemID );

			if( !pBaseItem )
			{
				return;
			}

			CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();

			if (pLayer)
			{
				if(pBaseItem->BaseItem_Supplies.bType == 1 || pBaseItem->BaseItem_Supplies.bType == 3)
				{
					DSTC_DUNGEON_USED_POTION packet;
					packet.dwUserIndex = pUser->GetID();
					packet.bType = BYTE(pItem->m_wItemID - 5000);
					pLayer->BroadCastSectionMsg((char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum());
				}				
			}
			
			fPer = FLOAT(g_pBaseClassInfo[ pUser->GetClass() ].wPotion_Recovery 
				+ g_pBaseRaceInfo[ pUser->GetClass() ].wPotion_Recovery);

			if( pBaseItem->BaseItem_Supplies.bType == 1 )		
			{
				// HP
				dwRand	= ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) )
					+ pBaseItem->BaseItem_Supplies.wMin;
				dwRand  = DWORD((float)dwRand * (fPer / 100.f));
				pUser->SetHP(WORD(pUser->GetHP() + ( pUser->GetMaxHP() * ( (float)dwRand  / 100.f ) ) ));
					 
				// Packet...
				UserStatus.bStatusMany			= 1;
				UserStatus.pStatus[0].dwCode	= USER_HP;
				UserStatus.pStatus[0].dwMin		= pUser->GetHP();
			}
			else if( pBaseItem->BaseItem_Supplies.bType == 2 )	
			{
				// MP
				dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) ) 
					+ pBaseItem->BaseItem_Supplies.wMin;
				dwRand = DWORD((float)dwRand * (fPer / 100.f ));
				pUser->SetSP(WORD(pUser->GetSP() + ( pUser->GetMaxSP() * ( (float)dwRand / 100.f ) )));
				
				UserStatus.bStatusMany			= 1;
				UserStatus.pStatus[0].dwCode	= USER_MP;
				UserStatus.pStatus[0].dwMin		= pUser->GetSP();
			}
			else if( pBaseItem->BaseItem_Supplies.bType == 3 ) 
			{
				// HP POINT
				dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) )
					+ pBaseItem->BaseItem_Supplies.wMin;
				pUser->SetHP( WORD(pUser->GetHP() + ( dwRand * ( fPer / 100.f ) ) ));

				if( pUser->GetHP() > pUser->GetMaxHP() )
				{
					pUser->SetHP((WORD)pUser->GetMaxHP());
				}

				UserStatus.bStatusMany			= 1;
				UserStatus.pStatus[0].dwCode	= USER_HP;
				UserStatus.pStatus[0].dwMin		= pUser->GetHP();
			}
			else if( pBaseItem->BaseItem_Supplies.bType == 4 ) 
			{
				// sp, hp POINT
				dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) )
					+ pBaseItem->BaseItem_Supplies.wMin;
				pUser->SetHP( WORD(pUser->GetHP() + ( dwRand * ( fPer / 100.f ) ) ));

				if( pUser->GetHP() > pUser->GetMaxHP() ) 
				{
					pUser->SetHP((WORD)pUser->GetMaxHP());
				}
				
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetHP();

				dwRand = ( rand()%(pBaseItem->BaseItem_Supplies.wMax - pBaseItem->BaseItem_Supplies.wMin+1) )
					+ pBaseItem->BaseItem_Supplies.wMin;
				pUser->SetSP( WORD(pUser->GetSP() + ( dwRand * ( fPer / 100.f ) ) ));

				if( pUser->GetSP() > pUser->GetMaxSP()) 
				{
					pUser->SetSP((WORD)pUser->GetMaxSP());
				}

				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetSP();
			}
			else
			{
				return;
			}
		}
		break;

	default:
		return;
	}

	// Item Log.
	SetItemCount( pItem->m_wItemID, pUser->GetCurDungeonID(), -1 );
	SetItemLog( pItem, ITEM_LOG_USED, pUser->GetCurDungeonID(), pUser->m_szName, NULL );

	// 마지막으로 결과를 보낸다.
	pItem->m_Item_Supplies.bQuantity--;
	
	if( pItem->m_Item_Supplies.bQuantity == 0 )
	{
		// 아이템이 다 소모됐으면 지운다.
		memset( pItem, 0, sizeof( CItem ) );
	}
	
	ItemPickUP.bZipCode	= pItemUsed->bZipCode;	
	memcpy( &ItemPickUP.Item, pItem, sizeof( CItem ) );

	NetSendToUser( dwConnectionIndex, (char*)&ItemPickUP, ItemPickUP.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	NetSendToUser( dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
	
	CheckSpeed(pUser);
}


void CmdDungeonChat( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_CHAT* pPacket = (CTDS_DUNGEON_CHAT*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		
	if(!pUser)					return;
	else if(!pUser->GetCurDungeon())	return;
	else if( pUser->GetCurDungeon()->GetTemp(DUNGEON_TEMP_CHAT_DISABLE) )	return;		// 던젼 채팅 모두 못하기 일떄 리턴.
	else if( TRUE == pUser->m_dwTemp[USER_TEMP_GM_DRIVED_SILENCE_FLAG])		return;		// GM이 채팅 못하게 명령 내린상태 

	DSTC_DUNGEON_CHAT		packet;
	packet.bChatType		= pPacket->bChatType;
	packet.bChatStrLength	= pPacket->bChatStrLength;
	packet.dwUserIndex		= pUser->GetID();
	memcpy(packet.szChatStr, pPacket->szChatStr, packet.bChatStrLength);
	
	CDungeonLayer*	pLayer;
	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	// 대만은 GM 의 대화내용을 저장한다.
	// seven / [12/13/2004] 
#if defined (TAIWAN_LOCALIZING)
	packet.szChatStr[packet.bChatStrLength] = 0;
	
	if (pUser->IsGMUser() ||
		0 == __strncmp(pUser->m_szName, "GM", 2))	
	{
		// GM 유저와 무늬만 GM 유저의 대화를 남긴다.
		SetChatLogInDungeon((CHAT_TYPE)pPacket->bChatType, 
			pUser, NULL, pPacket->szChatStr, pPacket->bChatStrLength );
	}
#endif


#ifdef JAPAN_LOCALIZING

	// 일본은 대화내용을 섹션에 전달하지 않고,
	// 둥근 영역에 전달한다		( 2004. 08. 12. minjin )
	BYTE byRadTileCount = 5;	// For BroadCast Msg in Circle ( Radius Tile Count )

	pUser->GetCurDungeon()->BroadCastInCircle( pLayer, (VECTOR2*)pUser->GetCurPosition(), byRadTileCount, (char*)&packet, packet.GetPacketSize());

#else

	pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum() );		

#endif

}


void CmdDungeonJoin(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTDS_DUNGEON_JOIN*  join = (CTDS_DUNGEON_JOIN*)pMsg;

	BYTE bDestWorldSpot = 0;

	if (join->wDungeonID > 2000 && join->wDungeonID < 3000)
	{
		bDestWorldSpot		= join->wDungeonID % 2 == 0;
		join->wDungeonID	-= bDestWorldSpot;				
	}

	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo( join->wDungeonID );

	if(!pDungeon)	
	{
		Log(LOG_NORMAL, "Dungeon Info is not found at CmdDungeonJoin()! (DungeonNum:%d)", join->wDungeonID);
		SendDungeonJoinFailMessage(dwConnectionIndex, DUNGEON_JOIN_FAIL_DUNGEON_INFO_IS_NOT_FOUND);
		return;
	}

	if( pDungeon->GetDungeonTotalUserCount() >= MAX_USER_PER_DUNGEON )
	{
		SendDungeonJoinFailMessage(dwConnectionIndex, DUNGEON_JOIN_FAIL_USER_OVERFLOW);
		return;
	}

	// 월드맵을 통해 인증된 유저만 받아들인다.
	ACCEPT_USER*	pAccept				= g_pAcceptTable->GetAcceptInfo(join->dwUserIndex);
	BOOL			bConnectToGameRoom	= FALSE;
	BYTE			bUserType			= USER_TYPE_NORMAL;

	if(!pAccept)
	{
		Log(LOG_IMPORTANT, "Unauthorized User Joined! (Name:%s)", join->szChrName);
		SendDungeonJoinFailMessage(dwConnectionIndex, DUNGEON_JOIN_FAIL_UNAUTHORIZED_USER);
		return;
	}
	else
	{
		//정상적으로 인증되었으면 AcceptTable에서 Remove하고 
		bConnectToGameRoom = pAccept->bConnectToGameRoom;
		bUserType		   = pAccept->bUserType;
		g_pAcceptTable->Remove(pAccept);
	}	

	// 유저를 생성한다.
	CUser* pUser = (CUser*)CreateUnitInstance(UNIT_CLASS_ID_USER);
	
	pUser->SetCurDungeon(pDungeon);							// 쿼리 받은후에 필요 하다.
	pUser->m_dwPartyId				= join->dwPartyID;		// 쿼리 받은후에 필요 하다.
	pUser->m_dwConnectionIndex		= dwConnectionIndex;	
	pUser->m_bDestWorldSpot			= bDestWorldSpot;	
	pUser->m_bConnectToGameRoom		= bConnectToGameRoom;
	pUser->m_bUserType				= bUserType;
	pUser->m_wRecentJoinDungeonID	= join->wPrevDungeonID;	
	
	//포탈 Join일 경우 
	if(join->bIsPortalJoin)
	{
		PORTAL_BUCKET* pPortal	= (PORTAL_BUCKET*)LALAlloc(g_pPortalBufferPool);
		pPortal->wToDungeonID	= join->wDungeonID;
		pPortal->bLayerNo		= join->bLayerNum;
		pPortal->v2Pos			= join->v2Pos;
		pUser->pBufForPortal	= pPortal;
	}

	g_pNet->SetUserInfo( pUser->m_dwConnectionIndex, pUser );
	
	DSTWS_CHECK_DUNGEONJOIN pCheckPacket;
	memset(pCheckPacket.szCharacerName, 0, sizeof(pCheckPacket.szCharacerName));
	__lstrcpyn(pCheckPacket.szCharacerName, join->szChrName, MAX_CHARACTER_REAL_LENGTH + 1);
	pCheckPacket.wDungeonID			= join->wDungeonID;
	pCheckPacket.dwUserIndex		= join->dwUserIndex;
	pCheckPacket.dwConnectionIndex	= dwConnectionIndex;
	g_pNet->SendToServer(WSINDEX, (char*)&pCheckPacket, pCheckPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	
	CUser* pUserChk = g_pUserChkHash->GetData(join->dwUserIndex);

	if(pUserChk)
	{
		g_pUserChkHash->Delete(pUserChk, join->dwUserIndex);
	}

	g_pUserChkHash->Insert(pUser, join->dwUserIndex);	
}


void CmdDungeonMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_MOVE*	pMove	= (CTDS_DUNGEON_MOVE*)pMsg;
	CDungeonLayer*		pLayer	= NULL;
	CUser*				pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if(!pUser)											return;
	if(!pUser->GetCurDungeon())							return;
	if(pUser->m_enGodMode == GODMODE_STATUS_MAPLOADING)	return;	

	pLayer = pUser->GetCurDungeonLayer();

	if(!pLayer)											return;
	if(!pLayer->GetSection(pUser->GetPrevSectionNum())) return; 	

	if(!pUser->GetHP())									return;
	if(pUser->GetUnitStatus() == UNIT_STATUS_DEAD)		return;
	
	float fDirection = atan2f(FLOAT(pMove->wDestZ - pUser->GetCurMapTile()->wIndex_Z)
							, FLOAT(pMove->wDestX - pUser->GetCurMapTile()->wIndex_X));

	VECTOR2 v2Direction = {	cosf(fDirection), sinf(fDirection) };
	pUser->SetCurDirection(&v2Direction);

	if(pMove->bMoveType==UNIT_STATUS_RUNNING)
	{
		if(pUser->m_wPerWeight< WEIGTH_100PER_OVER)
		{
			pUser->SetStatus( pMove->bMoveType );
			pUser->SetLastMoveTick(g_dwTickCount);
		}
	}
	else
	{
		pUser->SetStatus( pMove->bMoveType );
		pUser->SetLastMoveTick(g_dwTickCount);
	}			
		
	DSTC_MOVE move;

	float fSpeed = 0.0f;

	if(pMove->bMoveType == UNIT_STATUS_RUNNING)
	{
		if(pUser->m_wPerWeight >= WEIGTH_80PER_OVER)
			fSpeed = pUser->GetMoveSpeed()/3;
	}		

	move.dwIndex			= pUser->GetID();
	move.bMoveType			= pMove->bMoveType;
	move.fSpeed				= (pUser->GetMoveSpeed()-fSpeed)/1000.f;
	move.v2MoveStartPos		= *pUser->GetCurPosition();
	move.v2MoveDirection	= v2Direction;
	move.wDestX				= pMove->wDestX;
	move.wDestZ				= pMove->wDestZ;
	
	// 메시지 전송.
	pLayer->BroadCastSectionMsg( (char*)&move, move.GetPacketSize(), pUser );

	// 스핵을 위한 정보 저장.
	pUser->m_dwTemp[ USER_TEMP_LASTMOVETICK ] = g_dwTickCount;
}

void CmdDungeonStop( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{	
	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser )	return;
	else if(!pUser->GetCurDungeon())	return;
	else if (pUser->m_enGodMode == GODMODE_STATUS_MAPLOADING) return;
	
	CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;
	else if (!pUser->GetHP() || pUser->GetUnitStatus() == UNIT_STATUS_DEAD)	return;

	CTDS_DUNGEON_STOP*	pStop = (CTDS_DUNGEON_STOP*)pMsg;

	//점유지역을 설정한다.
	MAP_TILE*	pStopTile = pLayer->GetMap()->GetTile( pStop->v2StopPos.x, pStop->v2StopPos.y );
	MAP_TILE*	pTile = pUser->GetCurMapTile();	

	if( !pTile || ( pTile->wAttr.uSection == 0 ) )
	{
		pUser->SetStatus( UNIT_STATUS_NORMAL );
		pUser->SendStopPacket(SEND_MODE_BROADCAST_SECTION);
		return;
	}

	if (!pStopTile || ( pStopTile->wAttr.uSection == 0 ))
	{
		pUser->SetStatus( UNIT_STATUS_NORMAL );
		pUser->SendStopPacket(SEND_MODE_BROADCAST_SECTION);
		return;
	}
	
	if (labs((int)pStopTile->wIndex_X - (int)pTile->wIndex_X) <= 1
		&& labs((int)pStopTile->wIndex_Z - (int)pTile->wIndex_Z <= 1))
	{
		pUser->SetCurPosition(&pStop->v2StopPos);
	}
	
	// 타일이 바뀌었으면 바뀐것을 처리.
	if( ( pUser->GetCurMapTile()->wIndex_X != pTile->wIndex_X ) || ( pUser->GetCurMapTile()->wIndex_Z != pTile->wIndex_Z ) )
	{
		// 만약 다른 층으로 이동해야할때라면..
		for( BYTE i = 0; i < pLayer->m_wMoveSpotNum; i++ )
		{			
			if (pUser->IsChangeLayer(pTile->wIndex_X, pTile->wIndex_Z, i))
			{				
				pUser->GetCurDungeon()->ChangeLayerUser(pUser, pUser->GetCurLayerIndex(), i);
				return;
			}
		}
	}
		
	if( pUser->GetPrevSectionNum() != pTile->wAttr.uSection )
	{
		int					i=0;
		BYTE				pIntersectSection[MAX_SECTION_LINK_NUM]={0,};
		CUser*				pDisAppearUser=0;
		CMonster*			pDisAppearMonster=0;
		POSITION_			UserPos;
		POSITION_			MonsterPos;
		CSection*			pSection=0;
		DSTC_CHANGE_SECTION	Section;
		DSTC_DISAPPEAR		DisAppear;
		BYTE				oldSectionNum = pUser->GetPrevSectionNum();
	
		pLayer->RemoveUserSection( pUser );
		pLayer->InsertUserSection( pUser, pTile->wAttr.uSection );
		pLayer->IntersectSection( pUser->GetPrevSectionNum(), oldSectionNum, pIntersectSection );		
		pSection = pLayer->GetSection( oldSectionNum );
		
		// 지워야할 섹션의 유저들에게 삭제 메세지를 보내고.
		DisAppear.dwUserIndex		= pUser->GetID();
		for( i=0; pIntersectSection[i] != 0; i++ )
		{
			pLayer->BroadCastSectionMsgExceptLink( (char*)&DisAppear, DisAppear.GetPacketSize(), pIntersectSection[i] );
		
			// 여기서 본유저에게 보내줄 DisAppear정보를 만든다.
			pSection = pLayer->GetSection( pIntersectSection[i] );
			if( !pSection ) continue;

			UserPos = pSection->m_pPcList->GetHeadPosition();

			while( UserPos )
			{
				pDisAppearUser = (CUser*)pSection->m_pPcList->GetNext( UserPos );
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount++] = pDisAppearUser->GetID();			
			}

			MonsterPos = pSection->m_pMonsterList->GetHeadPosition();

			while( MonsterPos )
			{
				pDisAppearMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount + Section.bDisAppearMonsterCount++] = pDisAppearMonster->GetID();
			}
		}
		
		Section.wChageSectionNum = pUser->GetPrevSectionNum();
		NetSendToUser( pUser->m_dwConnectionIndex, (char*)&Section, Section.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

		// 바뀐 섹션에서 세로 받을 정보를 받는다.
		pLayer->BroadCastIntersectSectionInfo( oldSectionNum, pUser->GetPrevSectionNum(), pUser );
	}

	pUser->SetStatus( UNIT_STATUS_NORMAL);
	pUser->SendStopPacket(SEND_MODE_BROADCAST_EXCEPT_SENDER);
		
	if (pUser->GetEffectDesc(__SKILL_HIDING__))
		UpdateUserForAI(pUser);
}


void CmdDungeonAttack_User_Mon( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_ATTACK_USER_MON*	pAttackUserMon = (CTDS_DUNGEON_ATTACK_USER_MON*)pMsg;
	DSTC_ATTACK_USER_MON	Attack;
	CDungeonLayer*			pLayer;

	CUser*		pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;

	if(!pUser->GetCurDungeon())	return;
	if (!pUser->GetHP() || pUser->GetUnitStatus() == UNIT_STATUS_DEAD)	return;

	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	CMonster* pMonster= g_pMonsterTable->GetMonster( pAttackUserMon->dwMonsterIndex );

	if( pUser->m_enGodMode )
	{
		goto lbl_attack_fail;
	}
	else if(!pMonster) 
	{
		goto lbl_attack_fail;
	}
	else
	{	
		if (g_dwTickCount - pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] < pUser->GetAttackSpeed())
		{
			goto lbl_attack_fail;
		}
		else if (pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON) != __SKILL_ATTACK__)
		{
			goto lbl_attack_fail;
		}

		float fDist = CalcDistance( pUser->GetCurPosition(), pMonster->GetCurPosition() );
		
		// 아메리타트는 덩치커 어택 범위가 넓음 : 최덕석 2005.2.14
		if( fDist > (pMonster->GetBaseMonsterInfo()->dwID == 81 ? 800.0f : 400.0f) ) goto lbl_attack_fail;	

		// 대만 버전에서는 아이템 적재량 초과 시 공격 못한다.
#if defined TAIWAN_LOCALIZING
		if (pUser->GetAverageWeight() >= 100)
		{
			goto lbl_attack_fail;
		}
#endif 
		// 소환몬스터는 pk 모드일때에만 공격할수 있어.
		BOOL bChk = FALSE;
		if (pMonster->GetLord())
		{
			bChk = IsGuildWar( pUser, (CUser*)pMonster->GetLord());
//						, pUser->m_dwGuildId
//						, ((CUser*)pMonster->GetLord())->m_dwGuildId
//						, pUser->m_dwPartyId
//						, ((CUser*)pMonster->GetLord())->m_dwPartyId
//						, GODMODE_STATUS_RELEASE);
		}			

		if(!pUser->IsAlliance((CUnit*)pMonster))
		{			
			if(pMonster->GetLord() && pMonster->GetLord()->GetObjectType() != OBJECT_TYPE_MONSTER)
			{
				if(!bChk)
				{
					if (!pUser->m_sPKDescInfo.m_bPKMode)				
						goto lbl_attack_fail;
				}				
			}
		}
		else
		{
			goto lbl_attack_fail;
			
		}
	}
	
	//만약 이 몬스터가 비선공 몬스터이고 타겟없이 방황하고 있는넘이면 타겟을 셋팅해준다. 
	if(	pMonster->GetAI_FirstAttack() == AI_FIRST_ATTACK_NONE && !pMonster->GetUnitForAI())
	{
		pMonster->ChangeTargetObject(pUser);
	}
	
	//무조건 타겟 변경해야 하는 몬스터이면 공격성공여부에 상관없이 타겟을 바꿔준다. 
	if( pMonster->GetAI_TargetChangeType() == AI_CHANGE_TARGET_TYPE_ABSOLUTELY_CHANGE)
	{
		pMonster->ChangeTargetObject(pUser);
	}	

	ATTACK_RESULT	AttackResult;
	if( AttackResult_Unit_Unit( pUser, pMonster, &AttackResult ) == 0 ) 
	{
		goto lbl_attack_fail;
	}
	  
	if( AttackResult.byType == 3 || AttackResult.byType == 4 )	// 공격이 성공했다면.
	{
		if( pMonster->GetHP() >= AttackResult.dwDamage )
		{
			pMonster->SetHP(pMonster->GetHP() - AttackResult.dwDamage, pUser);

			if (pMonster->GetLord())
			{
				SetBadAction(pUser, pMonster->GetLord());				
			}

			pMonster->ChangeTargetObject(pUser, AttackResult.dwDamage);			
		}
		else
		{
			pMonster->SetHP(0, pUser);
			if (pMonster->GetLord())
			{
				SetBadAction(pUser, pMonster->GetLord());
			}
		}
		
		// 데미지 입힌 것에 따라 hp, sp가 회복된다.
		if (50 > rand()%100)//: sung han 050204
		{			
			DWORD dwHPRecovery = DWORD(AttackResult.dwDamage * pUser->GetItemAttr(ITEM_ATTR_ATTACK_DMG_TO_HP) / 100.f);
			DSTC_USER_STATUS	UserStatus;
			UserStatus.bStatusMany	= 0;

			if (dwHPRecovery)
			{
				pUser->SetHP(WORD(pUser->GetHP()+dwHPRecovery));
									
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetHP();			
			}

			DWORD dwMPRecovery = DWORD(AttackResult.dwDamage * pUser->GetItemAttr(ITEM_ATTR_ATTACK_DMG_TO_SP) / 100.f);

			if (dwMPRecovery)
			{
				pUser->SetSP(WORD(pUser->GetSP()+dwMPRecovery));
				
				UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
				UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetSP();
			}
			
			if (UserStatus.bStatusMany)
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}		
	}
	
	Attack.dwUserIndex		= pUser->GetID();
	Attack.dwMonsterIndex	= pMonster->GetID();
	Attack.v2UserPos		= *pUser->GetCurPosition();		//pAttackUserMon->v2UserPos;
	Attack.v2MonsterPos		= *pMonster->GetCurPosition();
	Attack.bType			= AttackResult.byType;
	Attack.dwCurMonsterHP	= pMonster->GetHP();
	Attack.dwDamage			= AttackResult.dwDamage;
	Attack.bStatusKind		= USER_HP;					// 걍 같이 쓰자.
	Attack.bAttackType		= pUser->m_sAttackDescInfo.m_bCurAttackType;
	Attack.bAttackFrame		= pUser->m_sAttackDescInfo.m_bCurAttackFrame;

	pLayer->BroadCastSectionMsg( (char*)&Attack, Attack.GetPacketSize(), pUser->GetPrevSectionNum() );	
	
	if(!pMonster->GetHP())	// 몬스터는 죽었다. 죽었으면 경험치를 준다.
	{
		MonsterKillByUser( pUser, pMonster );
		UpdateMonsterForAI( pMonster );
		
		return;
	}
	else
	{
		pUser->SendItemSkill(ITEM_SKILL_ATTACK, OBJECT_TYPE_MONSTER, pMonster);
	}

	pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = g_dwTickCount;
	return;

lbl_attack_fail:
	Attack.dwUserIndex	= pUser->GetID();
	Attack.bType		= 5;
	NetSendToUser( pUser->m_dwConnectionIndex, (char*)&Attack, Attack.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);	
}
extern void DieOnGuildWar(CDungeonLayer *pLayer, CUser* pDieUser);

void CmdDungeonAttack_User_User( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_ATTACK_USER_USER*	pAttackUserUser = (CTDS_DUNGEON_ATTACK_USER_USER*)pMsg;
	DSTC_ATTACK_USER_USER			Attack;
	CUser*							pOffense, *pDefense;
	CDungeonLayer*					pLayer;
	BOOL							bChk	= FALSE;
	float							fDist = 0;

	pOffense = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );

	if(!pOffense)
		return;	
	if(!pOffense->GetCurDungeon())
		return;
	if (!pOffense->GetHP() || pOffense->GetUnitStatus() == UNIT_STATUS_DEAD)	return;	
	pLayer = pOffense->GetCurDungeonLayer();
	
	if(!pLayer){return;}

	if(pOffense->GetUnitStatus()==UNIT_STATUS_DEAD)	
		goto lbl_attack_fail;
	
	pDefense = g_pUserHash->GetData( pAttackUserUser->dwUserIndex );
	
	if(!pDefense) 
		goto lbl_attack_fail;

	else if(!pDefense->GetCurDungeon())	
		goto lbl_attack_fail;	
	else if(pDefense->GetUnitStatus() == UNIT_STATUS_DEAD 
			|| pDefense->m_enGodMode) 
		goto lbl_attack_fail;
	else if (g_dwTickCount - pOffense->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] 
			< pOffense->GetAttackSpeed())
		goto lbl_attack_fail;
	else if (pOffense->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON) != __SKILL_ATTACK__)
		goto lbl_attack_fail;

	fDist = CalcDistance( pOffense->GetCurPosition(), pDefense->GetCurPosition() );
	if( fDist > 250.0f ) goto lbl_attack_fail;

	// 대만 버전에서는 아이템 적재량 초과시 공격 못한다.
#if defined TAIWAN_LOCALIZING
	if (pOffense->GetAverageWeight() >= 100)
	{
		goto lbl_attack_fail;	
	}
#endif 

	bChk = IsGuildWar(pOffense, pDefense);
/*			pOffense->GetCurDungeon()
			, pOffense->m_dwGuildId
			, pDefense->m_dwGuildId
			, pOffense->m_dwPartyId
			, pDefense->m_dwPartyId		
			, pDefense->m_enGodMode
		);
*/
	CDungeon* pDungeon = pOffense->GetCurDungeon();
	if(pDungeon->GetDungeonDataEx()->IsStadium() //경기장이고
	&& pLayer->m_pTeamMatch )//팀매치 매니저가 있으면
	{	//같은편은 안때려야 하는 처리 필요 : hwoarang 050202
		if( !pLayer->m_pTeamMatch->OnAttack(pOffense, pDefense) )
		{
			goto lbl_attack_fail;
		}
		else
		{
			bChk = true;// 길드전 체크 플래그
		}
	}

	if(pOffense->m_sPKDescInfo.m_bPKMode==0 && !bChk)
	{	// pk mode도 아닌것이..
		goto lbl_attack_fail;
	}	
	
	ATTACK_RESULT	AttackResult;
	if( AttackResult_Unit_Unit( pOffense, pDefense, &AttackResult ) == 0 ) 
	{
		goto lbl_attack_fail;
	}

	if( AttackResult.byType == 3 || AttackResult.byType == 4 )	// 공격이 성공했다면.
	{
		Attack.bStatusKind = (BYTE)pDefense->SetDamage( pOffense->GetLevel(), AttackResult.dwDamage );		
	}

	Attack.dwDamage				= AttackResult.dwDamage;
	Attack.bType				= AttackResult.byType;
	Attack.dwDefenseUserIndex	= pDefense->GetID();
	Attack.dwOffenseUserIndex	= pOffense->GetID();
	Attack.v2DefenseUserPos		= *pDefense->GetCurPosition();
	Attack.v2OffenseUserPos		= *pOffense->GetCurPosition();
	Attack.bAttackType			= pOffense->m_sAttackDescInfo.m_bCurAttackType;
	Attack.bAttackFrame			= pOffense->m_sAttackDescInfo.m_bCurAttackFrame;

	if(	!pDefense->GetHP())//HP가 없는데. --> 죽었는데.
	{
		if( !pLayer->OnDie(pOffense, pDefense) )//길드전 사망처리가 죽이면 안된다고 한다. : hwoarang 050202
		{
			if(	pDungeon->GetDungeonDataEx()->IsStadium() //경기장이고
			&&	pLayer->m_pTeamMatch )//팀매치 매니저가 있으면
			{
				DieOnGuildWar( pLayer, pDefense);
			}
			else
			{//여기 들어올 일은 없음
				pDefense->SetHP(1);
			}
		}
		else
		{
			if(bChk && pDefense->GetUnitStatus()!=UNIT_STATUS_DEAD)
			{
				pOffense->GuildMemberKill(pDefense);
			}
		
			// 1:1 대전이 아니다
			if (!pDefense->m_sMatchDescInfo.m_bMatching)
			{ 
				SetBadAction(pOffense, pDefense);
							
				if (pOffense->GetCurDungeon()->GetDungeonDataEx()->IsConquer() && pOffense->GetCurDungeon()->GetDungeonDataEx()->m_bSiege)
					pOffense->m_wSiegePKCount++;

				pDefense->SetStatus( UNIT_STATUS_DEAD );


//재원 작업중. 실제 서비스에는 중국에만 해당됨.
#ifdef CHINA_LOCALIZING				
				//길드전이면 안되고. PVP 일때도 안되고.
				pDefense->PKDropItem( pOffense);
#endif

				pDefense->SendItemSkill(ITEM_SKILL_DIE, OBJECT_TYPE_PLAYER, pOffense);
			}
			else
			{	// 1:1 대전
				DSTC_END_MATCH_MAN2MAN	packet;
				packet.bJudgmentCode	= 0;	// KO
				memset(packet.szWinnerName, 0, sizeof(packet.szWinnerName));
				memset(packet.szLoserName, 0, sizeof(packet.szLoserName));
				__lstrcpyn(packet.szWinnerName, pOffense->m_szName, MAX_CHARACTER_REAL_LENGTH);
				__lstrcpyn(packet.szLoserName, pDefense->m_szName, MAX_CHARACTER_REAL_LENGTH);	
				
				// 승리 카운트 
				MatchWinCount(pOffense);
				// 패배 카운트
				MatchLoseCount(pDefense);
				
				pOffense->GetCurDungeonLayer()->BroadCastMsg((char *)&packet
					, packet.GetPacketSize());

				CDungeonLayer*	pTopLayer = pOffense->GetCurDungeon()->GetDungeonLayer(0); // 마을 0 레이어
				if (pTopLayer)
					pTopLayer->BroadCastMsg((char *)&packet, packet.GetPacketSize());


				pLayer->m_dwEndMatchJudgmentTick = 0;
				pLayer->m_dwMatchEndTime = g_dwTickCount + MATCH_JUDGMENT_TERM_TICK;	// 5초후에 이펙트 후 끝낸다.
			}
		}
	}
	else
	{//죽지는 않았네.		
		SetBadAction(pOffense, pDefense);
		
		pOffense->SendItemSkill(ITEM_SKILL_ATTACK, OBJECT_TYPE_PLAYER, pDefense);
		pDefense->SendItemSkill(ITEM_SKILL_DEFENSE, OBJECT_TYPE_PLAYER, pOffense);

		BYTE bSend = FALSE;
		if (pDefense->GetItemAttr(ITEM_ATTR_DMG_REFLECT_POINT))
		{
			int nDamage = pDefense->GetItemAttr(ITEM_ATTR_DMG_REFLECT_POINT);
			
			pOffense->SetDamage(pDefense->GetLevel(), nDamage);
			if (!pOffense->GetHP())
			{
				pOffense->SetHP(1);
			}

			bSend = TRUE;
		}

		if (pDefense->GetItemAttr(ITEM_ATTR_DMG_REFLECT))
		{
			int nDamage = INT(Attack.dwDamage*pDefense->GetItemAttr(ITEM_ATTR_DMG_REFLECT)/100.F);
			pOffense->SetDamage(pDefense->GetLevel(), nDamage);
			if (!pOffense->GetHP())
			{
				pOffense->SetHP(1);
			}
			bSend = TRUE;
		}

		if (bSend)
		{
			DSTC_USER_STATUS	UserStatus;
			UserStatus.bStatusMany = 0;
			UserStatus.pStatus[UserStatus.bStatusMany].dwMin = pOffense->GetHP();
			UserStatus.pStatus[UserStatus.bStatusMany++].dwCode = USER_HP;			
			NetSendToUser(pOffense->m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}		

	if( Attack.bStatusKind == USER_HP )
	{
		Attack.dwCurDefenseUserHP	= pDefense->GetHP();
	}
	else if( Attack.bStatusKind == USER_MP )
	{
		Attack.dwCurDefenseUserHP	= (pDefense->GetHP()<<16) | pDefense->GetSP();
	}

	pLayer->BroadCastSectionMsg( (char*)&Attack, Attack.GetPacketSize(), pOffense->GetPrevSectionNum() );
	pOffense->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = g_dwTickCount;

	return;

lbl_attack_fail:
	{
		Attack.dwOffenseUserIndex	= pOffense->GetID();
		Attack.bType				= 5;
		NetSendToUser( pOffense->m_dwConnectionIndex, (char*)&Attack, Attack.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	}
}

void CmdSkillLevelUp( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_SKILL_LEVELUP* pPacket = (CTDS_SKILL_LEVELUP*)pMsg;
	DSTC_SKILL_INFO		pSendPacket;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	if(!pUser)	return;	
	if(pUser->GetCurLayerIndex()	 == INVALID_DUNGEON_LAYER)	return;
	
	if(pUser->m_wPointSkill>0)
	{		
		Effect* pEffectUp	= g_pEffectLayer->GetEffectInfo((BYTE)pPacket->nSkillIndex);

		// 같은 직업만 올릴수 있는 스킬인지 검사해라.
		if (!(pEffectUp->bAbleClass & (1<<(pUser->GetClass()-1))))	return;

		BYTE bSkillMastery = g_pEffectLayer->GetSkillMasteryKind((BYTE)pPacket->nSkillIndex);
		Effect* pEffectTar	= g_pEffectLayer->GetEffectInfo(bSkillMastery);

#ifndef __SKILL_MASTERY_ENABLE
	if (bSkillMastery == pEffectUp->bID)
		return;
#endif
		if(pEffectUp->dwMinMastery <=(DWORD)pEffectTar->GetMaxMastery(pUser->GetLevel(), pUser->m_pwSkillLevel[bSkillMastery], pUser->GetClass()))
		{
			
			if(pUser->m_pwSkillLevel[pPacket->nSkillIndex]<((bSkillMastery == pPacket->nSkillIndex)?255:25))
			{
				pUser->m_wPointSkill	-= 1;
				pUser->m_pwSkillLevel[pPacket->nSkillIndex] += 1;

				if (pUser->m_pwSkillLevel[bSkillMastery] < 255)
					pUser->m_pwSkillLevel[bSkillMastery] = (pUser->m_pwSkillLevel[bSkillMastery]+1);

				pSendPacket.bSkillInfo		= TRUE;
				pSendPacket.bySkillIndex	= (BYTE)pPacket->nSkillIndex;
				
				pUser->SendAllStatus();	// 모든 상태를 갱신해서 보낸다.					
			}
		}				
	}
	else
	{
		pSendPacket.bySkillIndex	= pPacket->nSkillIndex;
		pSendPacket.bSkillInfo		= FALSE;	
	}

	NetSendToUser(dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


void CmdStatusLevelUp( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{	
	CTDS_CHAR_LEVELUP*	pPacket = (CTDS_CHAR_LEVELUP*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);

	if(!pUser)	return;	
	else if (pUser->GetCurLayerIndex() == INVALID_DUNGEON_LAYER)	return;
	else if (pUser->GetUnitStatus() == UNIT_STATUS_DEAD)	return;

	CUnit* pUnit = NULL;
	if (pPacket->byObjectType == OBJECT_TYPE_MONSTER)
		pUnit = pUser->GetGuardian(0);
	else if (pPacket->byObjectType == OBJECT_TYPE_PLAYER)
		pUnit = pUser;
	
	if (pUnit)
		pUnit->AddStatusPoint((STATUS_POINT_KIND)pPacket->byStatusType);
	
}


void CmdDungeonRequestInfo(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTDS_DUNGEON_REQUEST_INFO*	pInfo = (CTDS_DUNGEON_REQUEST_INFO*)pMsg;
	CDungeonLayer*				pLayer;
	
	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;
	if(!pUser->GetCurDungeon())	return;

	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;
	
	switch( pInfo->bRequestType)
	{
	case 1:			// User
		{
			CSection*	pSection;
			BOOL		bSendInfo = FALSE;
			CUser*		pInfoUser = g_pUserHash->GetData( pInfo->dwRequestIndex ); //pUser->GetCurDungeon()->GetUser( pInfo->dwRequestIndex ); //(CUser*)g_pNet->GetUserInfo( pInfo->dwRequestIndex );
			if( !pInfoUser ) return;
			
			pSection = pLayer->GetSection( pUser->GetPrevSectionNum() );
			if( !pSection ) return;

			if( pUser->GetPrevSectionNum() != pInfoUser->GetPrevSectionNum() )
			{
				for( int i=0; pSection->m_wLink[i] != 0; i++ )
				{
					if( pInfoUser->GetPrevSectionNum() == pSection->m_wLink[i] )
					{
						bSendInfo = TRUE;
						break;
					}
				}
			}
			else bSendInfo = TRUE;
			
			if( !bSendInfo ) return;

			DSTC_APPEAR		AppearUser;
			MakeAppearUser(&AppearUser, pInfoUser, 0);
			
			if( !(AppearUser.dwGuildWarFlag & G_W_F_OBSERVER) )//본인이 옵저버가 아니면 보내준다.
			{
				NetSendToUser( dwConnectionIndex, (char*)&AppearUser, AppearUser.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			}

			if( pInfoUser->GetUnitStatus() == UNIT_STATUS_WALKING || pInfoUser->GetUnitStatus() == UNIT_STATUS_RUNNING )
			{
				DSTC_MOVE move;

				float fSpeed = 0.0f;

				if(pInfoUser->GetUnitStatus() == UNIT_STATUS_RUNNING)
				{
					if(pUser->m_wPerWeight >= WEIGTH_80PER_OVER)
						fSpeed = pUser->GetMoveSpeed()/3;
				}
					
				move.dwIndex			= pInfoUser->GetID();
				move.bMoveType			= (BYTE)pInfoUser->GetUnitStatus();
				move.fSpeed				= (pInfoUser->GetMoveSpeed()-fSpeed)/1000.f;
				move.v2MoveStartPos		= *pInfoUser->GetCurPosition();
				move.v2MoveDirection	= *pInfoUser->GetCurDirection();
//				move.wDestX				= pInfoUser->wDestX;
//				move.wDestZ				= pInfoUser->wDestZ;
			
				// 메시지 전송.
				NetSendToUser( dwConnectionIndex, (char*)&move, move.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
// 				pUser->GetCurDungeon()->m_pDungeonLayer[ pUser->GetCurLayerIndex() ]->BroadCastSectionMsg( (char*)&move, move.GetPacketSize(), pUser );
			}
		}
		break;

	case 2:			// Monster
		{
			CSection*	pSection;
			BOOL		bSendInfo = FALSE;
			CMonster*	pInfoMonster = g_pMonsterTable->GetMonster( pInfo->dwRequestIndex );
			if( !pInfoMonster ) return;

			pSection = pLayer->GetSection( pUser->GetPrevSectionNum() );
			if( !pSection ) return;
			if( pUser->GetPrevSectionNum() != pInfoMonster->GetPrevSectionNum() )
			{
				for( int i=0; pSection->m_wLink[i] != 0; i++ )
				{
					if( pInfoMonster->GetPrevSectionNum() == pSection->m_wLink[i] )
					{
						bSendInfo = TRUE;
						break;
					}
				}
			}
			else bSendInfo = TRUE;

			if( !bSendInfo ) return;

			DSTC_APPEAR_MON	AppearMon;
			MakeAppearMonser( &AppearMon, pInfoMonster );
			NetSendToUser( dwConnectionIndex, (char*)&AppearMon, AppearMon.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

			if( pInfoMonster->GetUnitStatus() == UNIT_STATUS_WALKING 
				|| pInfoMonster->GetUnitStatus() == UNIT_STATUS_RUNNING )
			{
				DSTC_MOVE_MON	MoveMon;
				MoveMon.fSpeed			= pInfoMonster->GetMoveSpeed();
				MoveMon.dwMonsterIndex	= pInfoMonster->GetID();
				MoveMon.v2MoveStartPos	= *pInfoMonster->GetCurPosition();
				MoveMon.v2MoveStopPos	= *pInfoMonster->GetDestPosition();
				MoveMon.v2MoveDirection	= *pInfoMonster->GetCurDirection();
				MoveMon.bMoveType		= UNIT_STATUS_WALKING;

				NetSendToUser( dwConnectionIndex, (char*)&MoveMon, MoveMon.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
//				pInfoMonster->GetCurDungeonLayer()->BroadCastSectionMsgExceptLink( (char*)&MoveMon, MoveMon.GetPacketSize(), pInfoMonster->GetPrevSectionNum() );
			}
		}
		break;
	}
}


void CmdCallGuardian(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{	
	CTDS_DUNGEON_CALLGARDIAN*	pCall = ( CTDS_DUNGEON_CALLGARDIAN* )pMsg;

	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );	

	if( !pUser )														return;
	else if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)					return;
	else if( pUser->GetGuardian(pCall->bZipCode) )					return;
	else if( pUser->m_pInv_Guardian[ pCall->bZipCode ].m_wItemID == 0 ) return;
	else if( pUser->m_pInv_Guardian[pCall->bZipCode].m_Item_Guardian.bCreate == GUARDIAN_CREATE_STATUS_CREATING ) return;

	char szSQL[1024] = {0,};
	LPGUARDIAN_CREATION_INFO pInfo = NULL;
	CItem* pGuardianItem = &pUser->m_pInv_Guardian[pCall->bZipCode];
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pGuardianItem->GetID());
	
	if ( GUARDIAN_ITEM_TYPE_EGG == pBaseItem->BaseItem_Guardian.bType  )
	{
		BOOL bSend		= TRUE;
		BOOL bSummon	= FALSE;
		
		if ( (!pGuardianItem->m_Item_Guardian.dwEndLife || IsGrowGuardianItem(pGuardianItem) ) &&
			!pUser->IsSummonGuardian(pCall->bZipCode))
		{
			// 알부화 놀이를 시작한다.
			if (!pGuardianItem->m_Item_Guardian.dwEndLife)
			{
				pGuardianItem->m_Item_Guardian.dwEndLife = GetDueDay(pBaseItem->BaseItem_Guardian.dwDueDayTime);
			}
			
			pUser->AttachItem(pGuardianItem);
			bSummon = TRUE;
		}
		else if ( (!pGuardianItem->m_Item_Guardian.dwEndLife || IsGrowGuardianItem(pGuardianItem) )	&&
			pUser->IsSummonGuardian(pCall->bZipCode))
		{
			// 알부화 놀이를 접어.
			pUser->UnSummonGuardian(pCall->bZipCode);
		}
		else if (IsOverDueDay(pGuardianItem->m_Item_Guardian.dwEndLife))
		{
			// 저런.. 알이 썩었자나.
			pGuardianItem->m_wItemID = pBaseItem->BaseItem_Guardian.wDieItemID;
			pUser->UnSummonGuardian(pCall->bZipCode);
		}
		else if (!IsOverDueDay(pGuardianItem->m_Item_Guardian.dwEndLife) &&
			pBaseItem->BaseItem_Guardian.wCompleteTime == pGuardianItem->m_Item_Guardian.dwCompleteTime)
		{
			// 오우 부화 시켯군.
			pGuardianItem->m_wItemID = pBaseItem->BaseItem_Guardian.wCreatureID;
			pGuardianItem->m_Item_Guardian.dwCompleteTime = 0;	// 이제부터는 경험치로 이용된다.
			pGuardianItem->m_Item_Guardian.dwEndLife = 0;
			pUser->UnSummonGuardian(pCall->bZipCode);
		}
		else 
		{
			bSend = FALSE;
		}
		
		if (bSend)
		{
			SendToUpdate_GuardianItem(dwConnectionIndex, pGuardianItem);
		}

		SendSummonEggStatus(dwConnectionIndex, bSummon, pGuardianItem);		
	}
	else if ( GUARDIAN_ITEM_TYPE_ADULT == pBaseItem->BaseItem_Guardian.bType )
	{
		if( GUARDIAN_CREATE_STATUS_ALREADYCALL == pGuardianItem->m_Item_Guardian.bCreate )
		{	
			if (IsOverDueDay(pGuardianItem->m_Item_Guardian.dwEndLife))
			{
				// 저런.. 영혼으로 변경.
				pGuardianItem->m_wItemID = pBaseItem->BaseItem_Guardian.wSoulItemID;
				pUser->UnSummonGuardian(pCall->bZipCode);
				SendToUpdate_GuardianItem(dwConnectionIndex, pGuardianItem);	
			}
			else
			{
				// 이미 생성된 가디언이라면 바로 불러버린다.
				pInfo = new GUARDIAN_CREATION_INFO;
				memset( pInfo, 0, sizeof( GUARDIAN_CREATION_INFO ) );
				pInfo->pUser	= pUser;
				pInfo->bZipCode	= pCall->bZipCode;

				wsprintf( szSQL, "select * from GuardianInfo where ID=%d", pGuardianItem->m_Item_Guardian.dwID );	
				g_pDb->THOpenRecord( szSQL, DB_UUID_GUARDIAN_INFO, pInfo, 1, GAME_DB );
			}			
		}
		else if( GUARDIAN_CREATE_STATUS_NONE == pGuardianItem->m_Item_Guardian.bCreate )
		{
			// 아직 생성되지 않은 상태라면 생성한다.
			DWORD			dwHP, dwMP;
			LPBASEMONSTER	pBaseMonster;
			pBaseMonster = g_pBaseMonsterHash->GetData( pBaseItem->BaseItem_Guardian.wBaseGuardianID );
			if( !pBaseMonster ) return;
		
			pInfo = new GUARDIAN_CREATION_INFO;
			memset( pInfo, 0, sizeof( GUARDIAN_CREATION_INFO ) );

			pInfo->bUserGuardian	= TRUE;
			pInfo->pUser			= pUser;
			pInfo->bZipCode			= pCall->bZipCode;

			pGuardianItem->m_Item_Guardian.bCreate			= GUARDIAN_CREATE_STATUS_CREATING;	// 생성중이란표시.
			pGuardianItem->m_Item_Guardian.dwCompleteTime	= 0;	// 이제부터는 경험치로 이용된다.
			pGuardianItem->m_Item_Guardian.dwEndLife		= GetDueDay(pBaseItem->BaseItem_Guardian.dwDueDayTime);

			// 처음 생성될떄의 HP와 MP를 구한다.
			dwHP = pBaseMonster->dwBaseHP	+ pBaseMonster->bLHP;
			dwMP = pBaseMonster->wBaseMP	+ pBaseMonster->bLMP;
		
			wsprintf( szSQL, "CZP_INSERT_GUARDIAN_INFO '%s', %d, %d, %d", pBaseMonster->szMonsterName_Kor
				, pBaseItem->BaseItem_Guardian.wBaseGuardianID, dwHP, dwMP );
			g_pDb->THOpenRecord( szSQL, DB_CREATE_GUARDIAN, pInfo, 1, GAME_DB );
		}
	}
}


void CmdGuardianAttack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_ATTACK*	pAttack = ( CTDS_GUARDIAN_ATTACK* )pMsg;

	CUser*	pUser;
	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;

	if( !pUser->GetGuardian( pAttack->bZipCode ) ) return;		// 가디언이 없다면 리턴.~
	
	switch( pAttack->bAttackType )
	{
	case 0:	// 타겟을 몬스터로 설정할때.
		{
			CMonster*	pTargetMon = g_pMonsterTable->GetMonster( pAttack->dwTargetIndex );
			if( !pTargetMon ) return;
				
			if( pTargetMon == pUser->GetGuardian(pAttack->bZipCode) ) return;	// 자해는 안돼.

			pUser->GetGuardian(pAttack->bZipCode)->SetTargetAI((CUnit*) pTargetMon );
		}
		break;

	case 1:	// 타켓을 유저로 설정할때.
		{
			CUser*		pTargetUser	= g_pUserHash->GetData( pAttack->dwTargetIndex );
			if( !pTargetUser ) return;
			
			if( pTargetUser == pUser ) return; // 시건방진넘.

			pUser->GetGuardian( pAttack->bZipCode )->SetTargetAI((CUnit*) pTargetUser );
		}
		break;
	}
}

void CmdGuardianCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_COMMAND*	pCommand = ( CTDS_GUARDIAN_COMMAND* )pMsg;

	CUser*	pUser;
	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;
	if(!pUser->GetCurDirection())	return;

	if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)
		return;

	CMonster* pGuardian = pUser->GetGuardian( pCommand->bZipCode );
	if( !pGuardian ) return;

	switch( pCommand->bCommand )
	{
	case 0:			// 방어모드.
		{
			pGuardian->ClearTargetAI();
		}
		break;

	case 1:			// Remove Guardian.
		{
			pUser->UnSummonGuardian(pCommand->bZipCode);
		}
		break;
	}
}

void CmdMonsterCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_MONSTER_COMMAND*	pCommand = (CTDS_MONSTER_COMMAND*)pMsg;

	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;
	int i;

	switch( pCommand->bCommand )
	{
	case 0:		// 방어.
		{
			for( i=0; i<MAX_USER_GUARDIAN; i++ )
			{
				pUser->m_pMonster[i]->ClearTargetAI();
			}
		}
		break;

	case 1:		// 공격.
		{
			if( pCommand->bAttackType == 0 )	// Attack Monster...
			{
				CMonster*	pTargetMon = g_pMonsterTable->GetMonster( pCommand->dwTargetIndex );
				if( !pTargetMon ) return;

				for( i=0; i<MAX_USER_GUARDIAN; i++ )
				{
					if( pUser->m_pMonster[i] )
					{
						if( pTargetMon == pUser->m_pMonster[i] ) return;
						if( pTargetMon == pUser->GetGuardian(BYTE(i)) ) return;
					}
				}
				
				for( i=0; i<MAX_USER_GUARDIAN; i++ )
				{
					if( pUser->m_pMonster[i] )
					{
						pUser->m_pMonster[i]->SetTargetAI((CUnit*) pTargetMon );
					}
				}
			}
			else if( pCommand->bAttackType	== 1 ) // Attack User...
			{
				CUser*		pTargetUser	= g_pUserHash->GetData( pCommand->dwTargetIndex );
				if( !pTargetUser ) return;
				
				if( pTargetUser == pUser ) return; // 시건방진넘.

				for( i=0; i<MAX_USER_GUARDIAN; i++ )
				{
					if( pUser->m_pMonster[i] )
					{
						pUser->m_pMonster[i]->SetTargetAI((CUnit*) pTargetUser );
					}
				}
			}
		}
		break;
	}
}

void CmdGuardianCommandMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_COMMAND_MOVE*	pMove = (CTDS_GUARDIAN_COMMAND_MOVE*)pMsg;
	
	int i;
	CMonster*	pMonster;

	// Guardian.
	for( i=0; pMove->pdwGuardianIndex[i] != 0 || i < MAX_USER_GUARDIAN; i++ )
	{
		pMonster = g_pMonsterTable->GetMonster( pMove->pdwGuardianIndex[i] );
		if( pMonster )
		{
			if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) continue;
			pMonster->ScheduleMoveDestPos( &pMove->v2DestPos );
		}
	}

	// Monster.
	for( i=0; pMove->pdwMonsterIndex[i] != 0 || i < MAX_USER_GUARDIAN; i++ )
	{
		pMonster = g_pMonsterTable->GetMonster( pMove->pdwMonsterIndex[i] );
		if( pMonster )
		{
			if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) continue;
			pMonster->ScheduleMoveDestPos( &pMove->v2DestPos );
		}
	}
}

void CmdGuardianCommandAttack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_COMMAND_ATTACK*	pAttack = (CTDS_GUARDIAN_COMMAND_ATTACK*)pMsg;

	int i=0;
	CMonster*	pMonster=0, *pTargetMon=0;
	CUser*		pTargetUser=0;
	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;

	if(pUser->GetUnitStatus()==UNIT_STATUS_DEAD)
		return;

	if( pAttack->bAttackType == 0 )
	{
		pTargetMon = g_pMonsterTable->GetMonster( pAttack->dwTargetIndex );
		if( !pTargetMon ) return;
	}
	else if( pAttack->bAttackType == 1 )
	{		
		pTargetUser = g_pUserHash->GetData( pAttack->dwTargetIndex );
		
		if(!pTargetUser)
			return;
	
		if(!IsGuildWar(pUser, pTargetUser) )
		{
			CDungeon *pDungeon =pUser->GetCurDungeon();
			
			if(pUser->IsAlliance((CUnit*)pTargetUser))
			{
				return;
			}

			if( pDungeon->GetDungeonDataEx()->IsStadium() )//가디언이 때릴 수 있게 : hwoarang 050219 
			{//경기장이면
			}
			else
			{//경기장아니면
				if(pUser->m_sPKDescInfo.m_bPKMode==0)
				{
					return;
				}
			}
		}
/*
		if(!IsGuildWar(pUser->GetCurDungeon()
			, pUser->m_dwGuildId
			, pTargetUser->m_dwGuildId
			, pUser->m_dwPartyId
			, pTargetUser->m_dwPartyId
			, pTargetUser->m_enGodMode)	)					
		{
			if(pUser->m_sPKDescInfo.m_bPKMode==0)
				return;
			if(pUser->IsAlliance((CUnit*)pTargetUser))
				return;
		}		
*/	}

	// Guardian.
	for( i=0; pAttack->pdwGuardianIndex[i] != 0 || i < MAX_USER_GUARDIAN; i++ )
	{
		pMonster = g_pMonsterTable->GetMonster( pAttack->pdwGuardianIndex[i] );
		if( pMonster )
		{
			pMonster->SetStatus( UNIT_STATUS_NORMAL, TRUE );
			if( pAttack->bAttackType == 0 )
			{	
				if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) 
					continue;
				pMonster->SetTargetAI((CUnit*) pTargetMon );
			}
			else if( pAttack->bAttackType == 1 )
			{
				if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) 
					continue;
				pMonster->SetTargetAI((CUnit*) pTargetUser );
			}
		}
	}

	// Monster.
	for( i=0; pAttack->pdwMonsterIndex[i] != 0 || i < MAX_USER_GUARDIAN; i++ )
	{
		pMonster = g_pMonsterTable->GetMonster( pAttack->pdwMonsterIndex[i] );
		if( pMonster )
		{
			pMonster->SetStatus( UNIT_STATUS_NORMAL, TRUE );
			if( pAttack->bAttackType == 0 )
			{	
				if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) continue;
				pMonster->SetTargetAI((CUnit*) pTargetMon );
			}
			else if( pAttack->bAttackType == 1 )
			{
				if( pMonster->GetUnitStatus() == UNIT_STATUS_DEAD ) continue;
				pMonster->SetTargetAI((CUnit*) pTargetUser );
			}
		}
	}
}

// 가디언 스킬 선택
void CmdGuardianSkillSelect( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GUARDIAN_SKILL_SELECT* pGuardianSkill = (CTDS_GUARDIAN_SKILL_SELECT*)pMsg;
	CUser*	pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	CMonster *pGuardian = NULL;

	if(!pUser ) return;
	pGuardian = pUser->GetGuardian(0);
	if(!pGuardian) return;

	pGuardian->SetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON, pGuardianSkill->bSkill);
}

void CmdGMQuickMove( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GM_QUICK_MOVE* pMove = (CTDS_GM_QUICK_MOVE*)pMsg;
	CDungeonLayer*		pLayer;

	CUser*	pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser )	return;
	if( !pUser->GetCurDungeon())	return;
	
	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	CMap*		pMap = pLayer->GetMap();
	MAP_TILE*	pTile = pMap->GetTile( pMove->v2MoveStartPos.x, pMove->v2MoveStartPos.y );
	
	if( !pLayer->GetSection(pUser->GetPrevSectionNum()) ) return;
	if( pUser->GetPrevSectionNum() != pTile->wAttr.uSection )
	{
		int					i=0;
		BYTE				IntersectSection[MAX_SECTION_LINK_NUM]={0,};
		CUser*				pDisAppearUser=0;
		CMonster*			pDisAppearMonster=0;
		POSITION_			UserPos;
		POSITION_			MonsterPos;
		CSection*			pSection=0;
		DSTC_CHANGE_SECTION	Section;
		DSTC_DISAPPEAR		DisAppear;
		BYTE				oldSectionNum = pUser->GetPrevSectionNum();
	
		pLayer->RemoveUserSection( pUser );
		pLayer->InsertUserSection( pUser,  pTile->wAttr.uSection );
		
		pLayer->IntersectSection( pUser->GetPrevSectionNum(), oldSectionNum, IntersectSection );

		// 지워야할 섹션의 유저들에게 삭제 메세지를 보내고.
		DisAppear.dwUserIndex		= pUser->GetID();
		for( i=0; IntersectSection[i] != 0; i++ )
		{
			pLayer->BroadCastSectionMsgExceptLink( (char*)&DisAppear, DisAppear.GetPacketSize(), IntersectSection[i] );
		
			// 여기서 본유저에게 보내줄 DisAppear정보를 만든다.
			pSection = pLayer->GetSection( IntersectSection[i] );
			if( !pSection ) continue;

			UserPos = pSection->m_pPcList->GetHeadPosition();
			while( UserPos )
			{
				pDisAppearUser = (CUser*)pSection->m_pPcList->GetNext( UserPos );
				// mod by deepdark
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount++] = pDisAppearUser->GetID();
			//	Section.pdwDisAppearUserIndex[dwUserCount] = pDisAppearUser->GetID();
			//	dwUserCount++;
			}

			MonsterPos = pSection->m_pMonsterList->GetHeadPosition();
			while( MonsterPos )
			{
				pDisAppearMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
				// mod by deepdark
				Section.pdwDisAppearIndex[Section.bDisAppearUserCount + Section.bDisAppearMonsterCount++] = pDisAppearMonster->GetID();
			//	Section.pdwDisAppearMonsterIndex[dwMonsterCount] = pDisAppearMonster->GetID();
			//	dwMonsterCount++;
			}
		}
		
		Section.wChageSectionNum = pUser->GetPrevSectionNum();
		NetSendToUser( pUser->m_dwConnectionIndex, (char*)&Section, Section.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		
		// 바뀐 섹션에서 세로 받을 정보를 받는다.
		pLayer->BroadCastIntersectSectionInfo( oldSectionNum, pUser->GetPrevSectionNum(), pUser );
	}

	MAP_TILE* pDestTile = pMap->GetMap( pMove->wDestX, pMove->wDestZ );
	if(!pDestTile)
		__asm int 3

	// 이동하는곳을 설정하고 패킷을 보낸다.
	VECTOR2 v2Start = { pDestTile->x, pDestTile->z };
//	v2Start.x = pDestTile->x;
//	v2Start.y = pDestTile->z;
	pUser->SetCurPosition(&v2Start);
	pUser->SetCurDirection(&pMove->v2MoveDirection);
	pUser->SetStatus( UNIT_STATUS_NORMAL );
//	pUser->m_dwLastMoveTick	= g_dwTickCount;
	pUser->m_pTileDest		= pMap->GetMap( pMove->wDestX, pMove->wDestZ );

	DSTC_STOP	stop;
	stop.dwUserIndex	= pUser->GetID();
	stop.v2StopPos		= *pUser->GetCurPosition();
	
	// 메시지 전송.
	pLayer->BroadCastSectionMsg( (char*)&stop, stop.GetPacketSize(), pUser->GetCurMapTile()->wAttr.uSection );
}


void CmdFindHack( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_FIND_HACK*	pHack = (CTDS_FIND_HACK*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pUser)	return;

	SpeedHackLog( pUser, pHack->dwHackNum );
}

void CmdTickCheck( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_TICK_CHECK* pTick = (CTDS_TICK_CHECK*)pMsg;

	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pUser) return;

	// 처음 들어온 것임으로 값만 기억하고 그냥 리턴하자.
	if( pUser->m_dwTemp[ USER_TEMP_LASTCLIENTTICK ] == 0  && pUser->m_dwTemp[ USER_TEMP_LASTSERVERTICK ] == 0 )
	{
		pUser->m_dwTemp[ USER_TEMP_LASTCLIENTTICK ] = pTick->dwCurClientTick;
		pUser->m_dwTemp[ USER_TEMP_LASTSERVERTICK ] = g_dwTickCount;
		return;
	}

	// 여기까지 왔다면 이제 검사를 해보자.

	DWORD	dwServerCheckTick;		// 서버 시간이 얼마나 증가 했나.
	DWORD	dwClientCheckTick;		// 클라이언트의 시간은 얼마나 증가했나.

	dwServerCheckTick = g_dwTickCount - pUser->m_dwTemp[ USER_TEMP_LASTSERVERTICK ];
	dwClientCheckTick = pUser->m_dwTemp[ USER_TEMP_LASTCLIENTTICK ] - pTick->dwCurClientTick;

	pUser->m_dwTemp[ USER_TEMP_LASTSERVERTICK ] = g_dwTickCount;
	pUser->m_dwTemp[ USER_TEMP_LASTCLIENTTICK ] = pTick->dwCurClientTick;
} 


#if defined NEW_COMMAND

// MsgProc_for_y2jinc.cpp 참조..

#else	
// ! NEW_COMMAND

void CmdGmCommand( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_GM_COMMAND*	pCommand = (CTDS_GM_COMMAND*)pMsg;
	
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if(!pUser) return;
	
	if( !pUser->GetCurDungeon())	
		return;

#if defined TAIWAN_LOCALIZING
	if (!pUser->IsGMUser())
	{
		return;
	}
#endif

	switch( pCommand->wCommand )
	{
	case GM_COMMAND_PK_MODE:
		{
			pUser->GetCurDungeon()->SetTemp(DUNGEON_TEMP_PKMODE,pCommand->dwTemp[0]);
		}
		break;
		
	case GM_COMMAND_CHAT_DISABLE:
		{
			if( pCommand->dwTemp[0] == 1 )	// mean All.
			{
				pUser->GetCurDungeon()->SetTemp(DUNGEON_TEMP_CHAT_DISABLE, pCommand->dwTemp[1]);
			}	
		}
		break;
		
	case GM_COMMAND_CREATE:
		{
			if( pCommand->dwTemp[0] == 1 ) // mean Monster create.
			{
				pUser->GetCurDungeonLayer()->CallMonsterByGM( pUser
					, 0
					, pCommand->dwTemp[1]
					, pCommand->dwTemp[2] );
			}
			else if( pCommand->dwTemp[0] == 2 )
			{
				CreateItemByGM( pUser, pCommand->dwTemp[1] );
			}
		}
	}
}
#endif