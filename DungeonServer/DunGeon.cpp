#include "StdAfx.h"
#include "Dungeon.h"
#include "RecordSet.h"
#include "recvmsg.h"
#include "DungeonLayer.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "OwnServer.h"
#include "monster.h"
#include "LayerFormation.h"
#include "MonsterManager.h"
#include "Section.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "EventDungeonManager.h"
#include <crtdbg.h>
#include "effect.h"
#include "User.h"


CDungeon::CDungeon()
{
	memset(this,0, sizeof(CDungeon));
	m_pDefenceParty		= new CVoidList;
	m_pRemovePCList		= new CVoidList;	
	m_pDungeonUserList	= new CVoidList;
	
	m_pPartyUserHash = new PARTYUSER_HASH;
	m_pPartyUserHash->InitializeHashTable( 200, 500, FALSE );

	m_pGuildUserHash = new GUILDUSER_HASH;
	m_pGuildUserHash->InitializeHashTable( 200, 500, FALSE );

	m_pChangeDungeon = new DB_CHANGE_DUNGEON;
	memset(m_pChangeDungeon, 0, sizeof(DB_CHANGE_DUNGEON));
}


CDungeon::~CDungeon()
{
	if (m_pRemovePCList)
	{
		POSITION_ pos = m_pRemovePCList->GetHeadPosition();

		while(pos)
		{
			POSITION_ del = pos;
			CUser* pRemoveUser = (CUser*)m_pRemovePCList->GetNext(pos);
			pRemoveUser->m_posRemoveList = NULL;
			m_pRemovePCList->RemoveAt(del);
			OnDisconnectUser(pRemoveUser->m_dwConnectionIndex);		
		}

		delete m_pRemovePCList;
	}

	if (m_pDungeonUserList)
	{
		m_pDungeonUserList->RemoveAll();
		delete m_pDungeonUserList;
		m_pDungeonUserList = NULL;
	}

	if (m_pChangeDungeon)
	{
		delete m_pChangeDungeon;
		m_pChangeDungeon = NULL;
	}
	
	if (NULL != m_pEventDungeonProperty)
	{
		delete m_pEventDungeonProperty;
		m_pEventDungeonProperty = NULL;
	}	
}

// DungeonTable에서 DungeonList의 빠른삭제를 위한 DungeonList Node Pointer 리턴!
POSITION_ CDungeon::GetDungeonListPos()
{
	return m_DungeonListPos;
}	

// 방어자가 될 파티번호 리스트 지워!!
void CDungeon::RemoveAllDefencePartyList()
{
	POSITION_ pos = m_pDefenceParty->GetHeadPosition();

	while(pos)
	{
		POSITION_ del = pos;
		DWORD* pdwPartyID  = (DWORD*)m_pDefenceParty->GetNext(pos);

		delete pdwPartyID;
		m_pDefenceParty->RemoveAt(del);
	}

	for(int i=0; i<GetTotalLayer(); i++)
	{
		// 이미 접속되어 있던놈들 조사해서 방어자도 공격자도 아닌 상태로 만들어라.
		POSITION_ pos = m_pDungeonLayer[i]->m_pPcList->GetHeadPosition();

		while(pos)
		{
			 CUser* pUser = (CUser*)m_pDungeonLayer[i]->m_pPcList->GetNext(pos);

			 if (pUser && pUser->GetCurDungeon() == this)
			 {
				 pUser->SetAttackMode(ATTACK_MODE_NONE);
			 }
		}		
	}
}

void CDungeon::SetDungeonTitle(char* szTitle)
{
	if(strlen(szTitle) > MAX_DUNGEON_TITLE_LENGTH)
	{
		Log(LOG_IMPORTANT, "Dungeon Name Is Too Long!(%s)", szTitle);
		return;
	}

	__lstrcpyn(m_szDungeonTitle, szTitle, MAX_DUNGEON_TITLE_LENGTH );
}

//-----------------------------------------------------------------------------
// 던전을 생성한다(일반 던전서버일 경우) 
//	
//※ 주의 : 일반던전일 경우에만 서버 초기 실행시 실행된다. 서버 돌아가고 있는 도중
//			사용하지 말자..    외부 파일 읽어드리고 new로 메모리 할당해서 느림.ㅡㅡ 
//-----------------------------------------------------------------------------
BOOL CDungeon::Build()
{
	DWORD	dwLayer=0;
	BYTE	i=0;

	for(i = 0; i < MAX_LAYER_PER_DUNGEON; i++)
	{
		m_pDungeonLayer[ i ] = NULL;
	}
	
	for(i=0; i<m_pInfo->m_wLayerCount; i++)
	{
		dwLayer = m_pInfo->m_dwLayerFormation[i];

		CDungeonLayer* pLayer = new CDungeonLayer;
		m_pDungeonLayer[ i ] = pLayer;

		pLayer->Create(dwLayer, i ,this);
		pLayer->m_bLayer = (BYTE)i;
	}
	
	srand(time(NULL));	
	
	return	TRUE;	
}


//-----------------------------------------------------------------------------
// Build()로 생성한 던전을 해제 시킨다. 
//	
//※ 주의 : 초보자 던전일경우 모든 메모리할당은 메모리 풀로 할당하므로 
//			이함수를 호출하여서는 안된당..  초보자 던전일 경우 ReleaseSession()으로 
//			던전을 해제 한다. 
//-----------------------------------------------------------------------------
void CDungeon::Destroy()
{
	if (m_pDefenceParty)
	{
		RemoveAllDefencePartyList();

		delete m_pDefenceParty;
		m_pDefenceParty = NULL;
	}
	
	for(DWORD i=0; i<m_pInfo->m_wLayerCount; i++)
	{
		m_pDungeonLayer[ i ]->Destroy();
		
		delete m_pDungeonLayer[ i ];
		m_pDungeonLayer[ i ] = NULL;
	}

	if( m_pPartyUserHash )
	{
		m_pPartyUserHash->Destroy(  );
		delete m_pPartyUserHash;
		m_pPartyUserHash = NULL;
	}

	if( m_pGuildUserHash )
	{
		m_pGuildUserHash->Destroy(  );
		delete m_pGuildUserHash;
		m_pGuildUserHash = NULL;
	}
}



//-----------------------------------------------------------------------------
// 던전 정보를 셋팅한다. 
//-----------------------------------------------------------------------------
BOOL CDungeon::SetDungeonInfo(DUNGEON_DATA_EX* pInfo, BYTE bDungeonKind)
{
	if(strlen(pInfo->m_szDungeonName) > MAX_DUNGEON_NAME_LENGTH)
	{
		Log(LOG_IMPORTANT, "Dungeon Name Is Too Long!(%s)", pInfo->m_szDungeonName );
		return FALSE;
	}

	m_pInfo = pInfo;
		
	SetDungeonKind(bDungeonKind);	
	m_pInfo->DecisionDungeonType(pInfo->m_dwID);

	return TRUE;
}


//-----------------------------------------------------------------------------
// 던전 유저에 유저 추가 
//-----------------------------------------------------------------------------
DUNGEON_JOIN_FAIL CDungeon::JoinUser(CUser* pUser, BYTE bLayerIndex, VECTOR2 *pv2SpecifiedPos)
{	
	if( (char)bLayerIndex < 0 || bLayerIndex >= GetTotalLayer() ) 
	{
		Log(LOG_IMPORTANT, "Invalid LayerIndex entered! (CDungeon::JoinUser())");
		return DUNGEON_JOIN_FAIL_INVALID_LAYERINDEX;
	}

	pUser->SetLayerIndex(bLayerIndex);
	InsertUser(pUser); 
	
	VECTOR2		v2Start = {0.f, 0.f};
	DSTC_JOIN	dungeonJoin;
	
	if(pv2SpecifiedPos)	
	{
		// 특정 위치로의 Join일 경우 ( 포탈 )
		v2Start = *pv2SpecifiedPos;
		dungeonJoin.bIsPortal = TRUE;		

		//내가 나타날 위치에 뭔가가 있나?
		const MAP_TILE* pTile = m_pDungeonLayer[bLayerIndex]->GetMap()->GetTile(v2Start.x, v2Start.y);

		if (!pTile)
		{
			Log(LOG_IMPORTANT, "Invalid Start Spot (CDungeon::JoinUser())");
			// 시작위치 찾을 수 없슴!!
			return DUNGEON_JOIN_FAIL_INVALID_START_SPOT;		
		}

		if (pTile->pData)
		{
			VECTOR2 vCur = m_pDungeonLayer[bLayerIndex]->GetAroundEmptyTile(pTile->wIndex_X, pTile->wIndex_Z);
			pUser->SetCurPosition(&vCur);
			//	pUser->SetCurPosition(&m_pDungeonLayer[bLayerIndex]->GetAroundEmptyTile(pTile->wIndex_X, pTile->wIndex_Z));
		}
	}
	else	
	{
		// Start Spot에서의 Join일 경우 
		if(!m_pDungeonLayer[bLayerIndex]->GetStartTile(pUser->m_bDestWorldSpot, &v2Start))
		{
			// 시작위치 찾을 수 없슴!!
			Log(LOG_IMPORTANT, "Invalid Start Spot (CDungeon::JoinUser())");
			return DUNGEON_JOIN_FAIL_INVALID_START_SPOT;		
		}
	}
	
	// 가디언 포인터 초기화.
	for( BYTE i = 0; i < MAX_USER_GUARDIAN; i++ )	
	{
		pUser->SetSummonMonster(OBJECT_TYPE_GUARDIAN, i, NULL);
	}
	
	if( !m_pDungeonLayer[bLayerIndex]->InsertUser( pUser, &v2Start ) )	
	{
		// 만약 조인 유저가 실패 했다면.
		dungeonJoin.bApproval = FALSE;
		NetSendToUser( pUser->m_dwConnectionIndex, (char*)&dungeonJoin, dungeonJoin.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		Log(LOG_IMPORTANT, "InsertUser Layer Failed! (CDungeon::JoinUser())");
		return DUNGEON_JOIN_FAIL_INSERTLAYERUSER_FAILED;
	}

	if (!GetDungeonDataEx()->IsDungeonOwner(pUser) && GetDungeonDataEx()->GetEntrancePay())
	{
		if (pUser->GetMoney() < GetDungeonDataEx()->GetEntrancePay())
		{
			return DUNGEON_JOIN_FAIL_NOT_ENOUGH_ENTER_KARZ;
		}
		
		pUser->SetMoney(pUser->GetMoney()-GetDungeonDataEx()->GetEntrancePay());
		GetDungeonDataEx()->SetAccEntrance(GetDungeonDataEx()->GetEntrancePay());
	}

	pUser->UpdateAllStatus();
	pUser->InitUsingItem();

	// 조인 메시지를 만든다. 기본정보
	dungeonJoin.bApproval		= TRUE;
	dungeonJoin.dwUserIndex		= pUser->GetID();
	__lstrcpyn( dungeonJoin.szName, (char*)pUser->GetName(), MAX_CHARACTER_REAL_LENGTH);
	dungeonJoin.wHead			= pUser->GetHeadModelNo();
	dungeonJoin.wClass			= pUser->GetClass();
	dungeonJoin.wRace			= pUser->GetRace();
	dungeonJoin.wGrade			= pUser->GetGrade();
	dungeonJoin.wHP				= (WORD)pUser->GetHP();
	dungeonJoin.wMP				= (WORD)pUser->GetSP();
	dungeonJoin.wPoint			= pUser->GetNotUseStatPoint();
	dungeonJoin.wPointSkill		= pUser->GetLeftoverStatSkillPoint();
	dungeonJoin.dwExp			= pUser->GetExp();
	dungeonJoin.dwLevel			= pUser->GetLevel();
	dungeonJoin.dwHonor			= pUser->GetHonor();
	dungeonJoin.dwEgo			= pUser->GetNewestEgo();
	dungeonJoin.dwStr			= pUser->GetNewestStr();
	dungeonJoin.dwInt			= pUser->GetNewestInt();
	dungeonJoin.dwDex			= pUser->GetNewestDex();
	dungeonJoin.dwVit			= pUser->GetNewestVit();
	dungeonJoin.dwLuck			= pUser->GetLuck();
	dungeonJoin.dwMatchRecords	= pUser->GetMatchDescInfo()->m_dwMatchRecords;
	dungeonJoin.dwMatchWin		= pUser->GetMatchDescInfo()->m_dwMatchWin;
	dungeonJoin.dwMatchLose		= pUser->GetMatchDescInfo()->m_dwMatchLose;
	dungeonJoin.byCurrentHand	= pUser->GetCurrentHand();
	dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON] = pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON);
	dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON] = pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON);
		
	if(dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON] < 1 
		|| dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON] > __SKILL_ATTACK__)
	{
		dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]	= __SKILL_ATTACK__;
		dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]	= __SKILL_NONE_SELECT__;
	}
	else if(dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON] < 1 
		|| dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON] > __SKILL_ATTACK__)
	{
		dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]	= __SKILL_ATTACK__;
		dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]	= __SKILL_NONE_SELECT__;
	}
	else
	{		
		if (dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON] > 0 
			&& dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON] < __SKILL_ATTACK__)
		{
			Effect* pEffect = g_pEffectLayer->GetEffectInfo(
								dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]);
			if (!pEffect->wProperty)
			{
				dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]	= __SKILL_ATTACK__;
				dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]	= __SKILL_NONE_SELECT__;
			}
		}
		else if (dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON] > 0 
			&& dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON] < __SKILL_ATTACK__)
		{
			Effect* pEffect = g_pEffectLayer->GetEffectInfo(
								dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]);
			if (!pEffect->wProperty)
			{
				dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_LBUTTON]	= __SKILL_ATTACK__;
				dungeonJoin.bySelectedSkill[SELECT_ATTACK_TYPE_RBUTTON]	= __SKILL_NONE_SELECT__;
			}
		}		
	}

	memcpy(dungeonJoin.pEquip, pUser->GetItem(ITEM_NATIVE_EQUIP, 0)	, sizeof(dungeonJoin.pEquip));
	memcpy(dungeonJoin.pInv_Large, pUser->GetItem(ITEM_NATIVE_INV_LARGE, 0), sizeof(dungeonJoin.pInv_Large));
	memcpy(dungeonJoin.pInv_Small, pUser->GetItem(ITEM_NATIVE_INV_SMALL, 0), sizeof(dungeonJoin.pInv_Small));
	memcpy(dungeonJoin.pBelt, pUser->GetItem(ITEM_NATIVE_BELT, 0), sizeof(dungeonJoin.pBelt));

	DWORD dwSkillValue	= pUser->GetExp();	
	if(pUser->m_byLastUsedSkillItem!=0)
	{
		dwSkillValue = pUser->GetExp() + pUser->m_byLastUsedSkillItem - 50;
	}
	
	// 스탯 검사 //
	for( i = 0; i < MAX_SKILL; i++ )
	{
		dungeonJoin.pwSkillLevel[i] = pUser->GetSkillLevel( i );
	}
	
	pUser->m_wPerWeight	= pUser->GetAverageWeight();
	
	float fSpeed = 0.0f;
	if(pUser->m_wPerWeight >= WEIGTH_80PER_OVER)
	{
		fSpeed = pUser->GetMoveSpeed() / 3;
	}

	__lstrcpyn(dungeonJoin.szGuildName, pUser->m_szGuildName, MAX_GUILD_REAL_NAME_LENGTH);
	__lstrcpyn(dungeonJoin.szNickName, pUser->m_szNickName, MAX_NICK_REAL_LENGTH);
	dungeonJoin.byType			= pUser->m_byGroupType;
	dungeonJoin.byRank			= pUser->m_byRank;
	dungeonJoin.dwGuildId		= pUser->m_dwGuildId;
	memcpy(dungeonJoin.pInv_Guardian, pUser->GetItem(ITEM_NATIVE_INV_GUARDIAN, 0), sizeof(dungeonJoin.pInv_Guardian));
	dungeonJoin.dwMoney			= pUser->GetMoney();

	if(g_pGuilRankId == pUser->m_dwGuildId && pUser->m_dwGuildId != 0)
	{
		dungeonJoin.byGuildFlag	= 1;
		pUser->m_byGuildFlag	= 1;
	}
	else
	{
		dungeonJoin.byGuildFlag	= 0;
		pUser->m_byGuildFlag	= 0;
	}
	
	dungeonJoin.wMaxHP			= (WORD)pUser->GetMaxHP();
	dungeonJoin.wMaxMP			= (WORD)pUser->GetMaxSP();
	memcpy( dungeonJoin.pwAttackDamage_R, pUser->GetPtrOfAttackDamageR(), sizeof(WORD)*2 );
	memcpy( dungeonJoin.pwAttackDamage_L, pUser->GetPtrOfAttackDamageL(), sizeof(WORD)*2 );
	dungeonJoin.wAttackAcuracy	= pUser->GetAttackAcuracy();
	dungeonJoin.wAvoid			= pUser->GetAvoid();
	dungeonJoin.wBlockRate		= pUser->GetBlockRate();
	
	dungeonJoin.wFireResist		= pUser->GetFireResist();
	dungeonJoin.wIceResist		= pUser->GetIceResist();
	dungeonJoin.wLightResist	= pUser->GetLightResist();
	dungeonJoin.wPoiResist		= pUser->GetPoiResist();
	dungeonJoin.wPhyResist		= pUser->GetPhyResist();
	
	dungeonJoin.fMoveSpeed		= (pUser->GetMoveSpeed()-fSpeed)/1000.f;
	dungeonJoin.dwHealHPSec		= pUser->m_dwHealHPSec;
	dungeonJoin.dwHealMPSec		= pUser->m_dwHealMPSec;
	dungeonJoin.wAttackSpeed	= pUser->GetAttackSpeed();

	dungeonJoin.dwLayerID		= pUser->GetCurDungeonLayerID();
	dungeonJoin.wMapID          = pUser->GetCurDungeonID();
	dungeonJoin.bLayer			= bLayerIndex;
	dungeonJoin.v2CurPos		= *pUser->GetCurPosition();	
	
	dungeonJoin.bAttackMode		= (BYTE)pUser->GetAttackMode();
	dungeonJoin.wSiegePKCount	= pUser->m_wSiegePKCount;
	dungeonJoin.wCriminal		= pUser->m_wCriminalPropensity ;
	dungeonJoin.wClassRank		= pUser->m_wClassRank;
	
	CDungeonLayer *pLayer = GetDungeonLayer( bLayerIndex );//: hwoarang 050202 
	
	if(	pLayer 
	&&	pLayer->m_pTeamMatch)
	{
		dungeonJoin.dwGuildWarFlag	= pLayer->m_pTeamMatch->GetTeamMatchFlag( pUser );
		dungeonJoin.team_index		= pLayer->m_pTeamMatch->GetTeamIndex( pUser );
	}
	else
	{
		dungeonJoin.dwGuildWarFlag = G_W_F_NONE;
		dungeonJoin.team_index = 0;
	}

	// 조인을 요청한 유저에게 메시지를 보내고.
	NetSendToUser( pUser->m_dwConnectionIndex, (char*)&dungeonJoin, dungeonJoin.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
	
	pUser->SetStatus(UNIT_STATUS_NORMAL);	
	ITEM_SHOP* pItemShop = (ITEM_SHOP*)LALAlloc(g_pItemShopPool);
	
	if(pItemShop)
	{
		pItemShop->byType = 0;
		g_pItemShopHash->Insert(pItemShop, pUser->GetID());
	}
	
	// 포탈이 아닌경우에만 
	if(!pv2SpecifiedPos)	
	{
		// 월드서버에게 Dungeon Join에 성공했다는 메세지를 보낸다. 
		DSTWS_DUNGEON_JOIN_SUCCESS	SuccessPacket;
		SuccessPacket.dwChar_Index	= pUser->GetID();
		SuccessPacket.wDungeonID	= WORD(GetID() + pUser->m_bDestWorldSpot);
		g_pNet->SendToServer(WSINDEX, (char*)&SuccessPacket, SuccessPacket.GetPacketSize(),FLAG_SEND_NOT_ENCRYPTION);
	}
	
	pUser->m_dwTemp[ USER_TEMP_LASTDBUPDATE ]	= g_dwTickCount;
	pUser->m_dwTemp[ USER_TEMP_LASTMOVETICK ]	= 0;
	pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = 0;
	
	//특정 위치로의 Join일 경우 ( 포탈 )
	if(pv2SpecifiedPos)	
	{
		SendJoinSectionInfo( pUser , 1 );
		
		DSTC_RESPONSE_PORTAL_JOIN_USER packet;		
		packet.vecStart		= *pUser->GetCurPosition();
		packet.dwUserIndex	= pUser->GetID();
		m_pDungeonLayer[bLayerIndex]->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), pUser );		
	}
	else
	{
		SendJoinSectionInfo( pUser , 0 );
	}		
	
	g_pUserHash->Insert(pUser, pUser->GetID());
			
	if (!pUser->IsPKZone())
	{
		if(pUser->m_sPKDescInfo.m_bPKMode)
			pUser->SetPKMode(false);
	}

	pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);
	pUser->SetPartyID(pUser->m_dwPartyId);
	pUser->SetGuildID(pUser->m_dwGuildId);

	if (m_pInfo->m_bSiege)
	{
		SendSiegeInfo(pUser);
		pUser->SetAttackMode(GetAttackMode(pUser));			
	}

	if (pUser->GetCurLayerIndex() && pUser->IsCurDungeonSiege() && pUser->GetAttackMode() != ATTACK_MODE_DEFENCE &&
		!pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY])
	{
		pUser->GetCurDungeon()->GotoLobbyUser(pUser);
	}
	else if (!pUser->GetCurLayerIndex() && pUser->IsCurDungeonSiege())
	{
		pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] = 1;	
	}

	if (pUser->GetPKDescInfo()->m_bPKCount | pUser->GetPKDescInfo()->m_bPKRepeatCount)
	{
		pUser->GetPKDescInfo()->m_pPKList = g_pThis->m_pPKUserCheckList->AddTail(pUser);
	}

	Event_DungeonJoinUser( pUser, this );
	
	return DUNGEON_JOIN_SUCCESS;
}


DWORD CDungeon::JoinUserForPortal(CUser* pUser, BYTE bLayerIndex )
{
	if((char)bLayerIndex < 0 || bLayerIndex >= GetTotalLayer()) 
	{
		Log(LOG_IMPORTANT, "Invalid LayerIndex entered! (CDungeon::JoinUserForPortal())");
		return 0xffffffff;
	}	
	
	pUser->SetLayerIndex(bLayerIndex);
	InsertUser(pUser);
	
	if(!pUser->IsPKZone())
	{
		if (pUser->GetPKDescInfo()->m_bPKMode)
		{
			pUser->SetPKMode(false);
		}
	}	

	// 가디언 포인터 초기화.
	for( BYTE i = 0; i < MAX_USER_GUARDIAN; i++ )	
	{
		pUser->SetSummonMonster(OBJECT_TYPE_GUARDIAN, i, NULL);
	}
	
	pUser->m_bCurrentHand						= 1;
	pUser->m_dwTemp[ USER_TEMP_LASTDBUPDATE ]	= g_dwTickCount;
	pUser->m_dwTemp[ USER_TEMP_LASTMOVETICK ]	= 0;
	pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] = 0;	

	pUser->SetPartyID(pUser->m_dwPartyId);
	pUser->SetGuildID(pUser->m_dwGuildId);

	pUser->UpdateAllStatus();
	pUser->m_pUsingItemList->RemoveAll();
	pUser->InitUsingItem();
	
	return 1;
}


BOOL CDungeon::PermissionJoinDungeon(CUser* pUser)
{
	DUNGEON_JOIN_FAIL eJoinFail = pUser->IsEnterDungeon();
							
	if (DUNGEON_JOIN_SUCCESS == eJoinFail)
	{
		if(pUser->pBufForPortal)	
		{
			// 포탈 Join일 경우 
			PORTAL_BUCKET* pPortal = (PORTAL_BUCKET*)pUser->pBufForPortal;
			eJoinFail = JoinUser( pUser, pPortal->bLayerNo, &pPortal->v2Pos );
			
			LALFree(g_pPortalBufferPool, pUser->pBufForPortal);
			pUser->pBufForPortal = NULL;

			if(DUNGEON_JOIN_SUCCESS != eJoinFail)
			{
				Log( LOG_NORMAL, "JoinUser Failed! ReceivedFromDB(portal join)! %s", pUser->GetName() );
			}
		}
		else
		{
			// 일반 Join
			eJoinFail = JoinUser( pUser );	

			if(DUNGEON_JOIN_SUCCESS != eJoinFail)
			{
				Log( LOG_NORMAL, "JoinUser Failed! ReceivedFromDB(normal join)! %s", pUser->GetName() );
			}
		}

		return TRUE;
	}
	else
	{ 
		SendDungeonJoinFailMessage(pUser->m_dwConnectionIndex, eJoinFail);
		pUser->CallBackCompulsiveDisconnectUser();
		return FALSE;
	}	
}

char* CDungeon::GetDungeonName()
{
	return m_pInfo->m_szDungeonName;
}

WORD CDungeon::GetTotalLayer() const
{
	return m_pInfo->m_wLayerCount;
}

WORD CDungeon::GetDungeonLayerID(BYTE bLayer) const
{	
	return (WORD)m_pInfo->m_dwLayerFormation[ bLayer ];	
}

// 공격자인지 방어자인지
BYTE CDungeon::GetAttackMode(CUser* pUser) const
{
	if (!m_pInfo->m_bSiege)
		return 0;

	BYTE bAttackMode = ATTACK_MODE_OFFENSE;
	
	if (m_pInfo->m_dwOwnerGuildNum && pUser->m_dwGuildId == m_pInfo->m_dwOwnerGuildNum)
	{
		// 던전 주인과 같은 길드원은 방어자.
		return ATTACK_MODE_DEFENCE;
	}
	else if (m_pInfo->IsDungeonOwner(pUser))
	{
		// 던전 주인은 방어자
		return ATTACK_MODE_DEFENCE;
	}
	else
	{
		POSITION_ pos = m_pDefenceParty->GetHeadPosition();

		while(pos)
		{
			// 방어자 파티 검사해봐라.
			DWORD* pdwPartyID = (DWORD*)m_pDefenceParty->GetNext(pos);

			if (*pdwPartyID == pUser->m_dwPartyId)
			{
				bAttackMode = ATTACK_MODE_DEFENCE;
				break;
			}			
		}		
	}	
	
	return bAttackMode;
}


// 방어자가 될 파티번호 추가.
void CDungeon::InsertDefensePartyList(DWORD dwPartyID)
{
	CUser* pOwner = g_pUserHash->GetData(m_pInfo->m_dwOwnerIndex);

	if (pOwner && pOwner->m_dwPartyId && pOwner->m_dwPartyId == dwPartyID)
	{
		// 던전 주인과 같은 파티구나.
		goto lbl_add;
	}
	
	return;
	
lbl_add:
	
	BOOL bAdd = TRUE;
	POSITION_ pos = m_pDefenceParty->GetHeadPosition();

	while(pos)
	{
		// 이미 넣었는지 체크한다.
		DWORD* pdwPartyID = (DWORD*)m_pDefenceParty->GetNext(pos);

		if (*pdwPartyID == dwPartyID)
		{
			bAdd = FALSE;
			break;
		}
	}

	if(bAdd)
	{
		// 방어자 파티 번호 추가하라.
		DWORD* pdwPartyID = new DWORD;
		*pdwPartyID = dwPartyID;
		m_pDefenceParty->AddTail(pdwPartyID);
	}
}


// 포탈이동용, 유저를 레이어에서 제거 (다른곳으로 이동을 위하여)
// 같은던전에서의 포탈이동용(층끼리만 이동하는 포탈의 경우 Layer 유저삭제),  유저가 나가는 처리를 한다. 
void CDungeon::RemoveUserForInsidePortal( CUser* pUser ) const
{	
	// 일단 다른 유저에게 나갔다는 메시지를 보낸다.
	DSTC_PORTAL_DISAPPEAR	DisAppear;
	DisAppear.dwUserIndex	= pUser->GetID();
	DisAppear.bType			= 1;	//Self Portal
	
	CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	pLayer->BroadCastSectionMsg( (char*)&DisAppear, DisAppear.GetPacketSize(), pUser );

	UpdateUserForAI( pUser );
	pLayer->RemoveUser( pUser );
}

// 같은던전에서의 포탈이동용(층끼리만 이동하는 포탈의 경우 유저 삽입),  유저가 포탈이동으로 들어가는 처리를 한다. 
BOOL CDungeon::SendPortalMsg( CUser* pUser, BYTE bDestLayerID, VECTOR2 *pv2Pos , BYTE bChangeDungeon ) const
{
	if((char)bDestLayerID < 0 || bDestLayerID >= GetTotalLayer()) 
	{
		Log(LOG_IMPORTANT, "Invalid LayerID entered! (CDungeon::SendPortalMsg())");
		return FALSE;
	}

	MAP_TILE* pTile = m_pDungeonLayer[ bDestLayerID ]->GetMap()->GetTile( pv2Pos->x, pv2Pos->y );

	if(!pTile)
	{
		Log(LOG_IMPORTANT, "Invalid user position entered! (CDungeon::SendPortalMsg())");
		return FALSE;
	}

	if (pUser->GetUnitStatus() == UNIT_STATUS_DEAD)
	{
		return FALSE;
	}

	// PK Zone 이 아닌경우 PK Mode 를 해제 한다.  2005.03.28 김영대
	if (!pUser->IsPKZone())
	{
		if (pUser->m_sPKDescInfo.m_bPKMode )
		{
			pUser->SetPKMode(false);
		}
	}

	
	pUser->SetAttackMode(GetAttackMode(pUser));
	pUser->SetReservPotalPosition(pv2Pos);
	
	DSTC_PORTAL_JOIN_USER packet;
	packet.wDungeonID		= GetID();								//던전아이디
	packet.wDungeonLayer	= GetDungeonLayerID( bDestLayerID );	//레이어 고유번호
	packet.v2CurPos			= *pv2Pos;								//유저현재 위치
	packet.bCurLayerIndex	= bDestLayerID;							//대상 레이어 층수
	packet.bChangeDungeon   = bChangeDungeon;		
	packet.bAttackMode		= (BYTE)pUser->GetAttackMode();

	// 그 Layer에서 나타나는 몬스터종류들을 클라이언트가 PreLoad 하기위해 보냄 
	BASEMONSTER* pMonster = NULL;
	memset( packet.pdwMonsterKind, 0, sizeof(DWORD)*MAX_MONSTER_KIND );

	for( int i = 0; i < MAX_MONSTER_KIND; i++ )
	{
		pMonster = g_pBaseMonsterHash->GetData( m_pDungeonLayer[bDestLayerID]->m_pLayerFormation->pdwMonsterKind[i] );
		packet.pdwMonsterKind[i] = (pMonster) ? pMonster->wMODNo : 0;
	}

	NetSendToUser( pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );		

	return FALSE;
}

void CDungeon::ChangeLayerMonster( CMonster* pMonster, int nDestLayerIndex, VECTOR2& v2Dest) const
{
	pMonster->RemoveCurDungeonLayer(TRUE);
	
	pMonster->SetDestPosition(&v2Dest);
	
	m_pDungeonLayer[nDestLayerIndex]->InsertMonster(pMonster, &v2Dest);
	
	// 몬스터 생성을 알린다.
	DSTC_APPEAR_MON AppearMon;
	MakeAppearMonser( &AppearMon, pMonster );
	CDungeonLayer *pLayer = pMonster->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg((char*)&AppearMon, AppearMon.GetPacketSize()
		, pMonster->GetPrevSectionNum() );
	}

	CSection* pSection = m_pDungeonLayer[nDestLayerIndex]->GetSection(pMonster->GetPrevSectionNum() );
	if( !pSection ) return;

	POSITION_ pos = pSection->m_pPcList->GetHeadPosition();
	POSITION_ posSkill = NULL;
	CUser* pUser = 0;

	while( pos )
	{
		pUser = (CUser*)pSection->m_pPcList->GetNext( pos );
		
		// 가지고 있던 상태 스킬을 다시 보내줘라.
		posSkill = pMonster->GetUsingStatusEffectList()->GetHeadPosition();

		while(posSkill)
		{
			EffectDesc* pEffectDesc = (EffectDesc*)pMonster->GetUsingStatusEffectList()->GetNext(posSkill);
			g_pEffectLayer->SendSkillStatusUserMon(pUser, pMonster, pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel, pEffectDesc->dwDestTick-g_dwTickCount);
		}
	}
}


void CDungeon::ChangeLayerUser( CUser* pUser, BYTE byCurLayerIndex, int nDestLayerIndex, BYTE byDestLayerSpot ) const
{
	if (NULL == pUser)	return;

	if (0 == nDestLayerIndex)
	{
		pUser->GetMatchDescInfo()->m_bMatching = FALSE;
		pUser->GetMatchDescInfo()->m_dwMatchUserIndex = 0;
	}

	if (nDestLayerIndex < 0 || nDestLayerIndex >= GetTotalLayer()) 
	{
		// 더이상 올라갈때도 없는가?
		return;
	}
	
	// 일단 다른 유저에게 나갔다는 메시지를 보낸다.
	CDungeonLayer*	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	DSTC_DISAPPEAR	DisAppear;
	DisAppear.dwUserIndex = pUser->GetID();	
	pLayer->BroadCastSectionMsg( (char*)&DisAppear, DisAppear.GetPacketSize(), pUser );

	VECTOR2 v2Start = {0.f, 0.f};

	if(!m_pDungeonLayer[nDestLayerIndex]->GetStartTile(!byDestLayerSpot, &v2Start))
	{
		// 시작위치 찾을 수 없슴!! 
		// 시작위치 없는 던전과 레이어 정보 기록.
		Log(LOG_IMPORTANT, "### No Start Pos: Dungeon Name: %s, Kind: %d, ID: %d, TotalLayer: %d, DestLayerIndex: %d, DestLayerSpot: %d",
			pUser->GetCurDungeon()->GetDungeonKind(),
			pUser->GetCurDungeon()->GetDungeonName(),
			pUser->GetCurDungeon()->GetID(),
			pUser->GetCurDungeon()->GetTotalLayer(),
			nDestLayerIndex, byDestLayerSpot);
		
		__asm int 3		
	}

	// 소환된놈들도 같이 체인지 레이여.
	CMonster* pElementMonster[MAX_USER_GUARDIAN];
	memcpy(pElementMonster, pUser->m_pMonster, sizeof(pElementMonster));
	memset(pUser->m_pMonster, 0, sizeof(pUser->m_pMonster));
	
	for(BYTE i = 0; i < MAX_USER_GUARDIAN; ++i)
	{
		if( pElementMonster[i] && pElementMonster[i]->IsElemental()	&&
			pElementMonster[i]->GetUnitStatus() != UNIT_STATUS_DEAD	&& !m_pInfo->IsVillage())
		{
			ChangeLayerMonster(pElementMonster[i], nDestLayerIndex, v2Start);
			pElementMonster[i]->SetLord(pUser);
		}
	}

	m_pDungeonLayer[ byCurLayerIndex ]->RemoveUser( pUser );
	m_pDungeonLayer[ nDestLayerIndex ]->InsertUser( pUser, &v2Start );
	
	memcpy(pUser->m_pMonster, pElementMonster, sizeof(pElementMonster));

	for(i = 0; i < MAX_USER_GUARDIAN; ++i)
	{
		if (pUser->m_pMonster[i] && pUser->m_pMonster[i]->IsElemental()	&& pUser->m_pMonster[i]->GetHP())
		{
			pUser->m_pMonster[i]->SetStatus(UNIT_STATUS_NORMAL);
		}
	}	
	
	DSTC_CHANGE_LAYER ChangeLayer;
	ChangeLayer.wDungeonIndex	= (WORD)GetID();
	ChangeLayer.wDungeonLayer	= GetDungeonLayerID( (BYTE)nDestLayerIndex );
	ChangeLayer.v2CurPos		= *pUser->GetCurPosition();
	ChangeLayer.bCurLayerIndex  = (BYTE)nDestLayerIndex;
	ChangeLayer.wTotalMonster	= (WORD)m_pDungeonLayer[ nDestLayerIndex ]->m_wTotalMonster;
	ChangeLayer.wCurMonster     = (WORD)m_pDungeonLayer[ nDestLayerIndex ]->m_wCurMonster;
	ChangeLayer.bAttackMode		= (BYTE)pUser->GetAttackMode();
	memset( ChangeLayer.pdwMonsterKind, 0, sizeof(DWORD)*MAX_MONSTER_KIND );
	
	// 그 Layer에서 나타나는 몬스터종류들을 클라이언트가 PreLoad 하기위해 보냄 
	LPBASEMONSTER pMonster = NULL;

	for( i = 0; i < MAX_MONSTER_KIND; i++ )
	{
		pMonster = g_pBaseMonsterHash->GetData( m_pDungeonLayer[nDestLayerIndex]->m_pLayerFormation->pdwMonsterKind[i] );

		if( pMonster )
		{
			ChangeLayer.pdwMonsterKind[i] = pMonster->wMODNo;
		}
		else 
		{
			ChangeLayer.pdwMonsterKind[i] = 0;
		}
	}
	
	NetSendToUser( pUser->m_dwConnectionIndex, (char*)&ChangeLayer, ChangeLayer.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

	SendJoinSectionInfo( pUser );
	
	if (m_pInfo->m_bSiege)
	{
		SendSiegeInfo(pUser);
	}
	
	if (!pUser->IsPKZone())
	{
		if (pUser->m_sPKDescInfo.m_bPKMode )
		{
			pUser->SetPKMode(false);
		}
	}

	pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);

	if(	byCurLayerIndex && pUser->IsCurDungeonSiege() &&
		pUser->GetAttackMode() != ATTACK_MODE_DEFENCE &&
		!pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY])
	{
		pUser->GetCurDungeon()->GotoLobbyUser(pUser);
	}
	else if(!byCurLayerIndex && pUser->IsCurDungeonSiege())
	{
		pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] = 1;	
	}
}


void CDungeon::ChangeLayerUser( CUser* pUser, BYTE byCurLayerIndex, BYTE byMoveSpotNum ) const
{
	int wDestLayer = m_pDungeonLayer[byCurLayerIndex]->m_pMoveSpot[byMoveSpotNum].wDestLayerID - 1;
	
	pUser->SetStatus( UNIT_STATUS_NORMAL );

#if defined JAPAN_LOCALIZING
	if( pUser->GetHP() == 0 )
	{
		DSTC_USER_STATUS	UserStatus;
		UserStatus.bStatusMany	= 0;

		// 대전 후 HP 를 반땅 채워준다.
		pUser->SetStatus(UNIT_STATUS_NORMAL, TRUE);
		pUser->SetHP( pUser->GetMaxHP() / 2 );

		// HP 반땅 채워준다.(클라이언트에 알린다.)
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetMaxHP() / 2;
		
		NetSendToUser(pUser->m_dwConnectionIndex, (char*)&UserStatus,  UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
#endif

	if (wDestLayer >= 10000)
	{
		// 월드로 갈때.
		if (pUser->GetCurDungeon()->m_pInfo->GetDungeonType() == DUNGEON_TYPE_VILLAGE)
		{
			byMoveSpotNum = 0;
		}
		
		// wMoveSpotNum은 Spot의 번호 (정상적이면 0,1 두가지이다)
		pUser->GoToWorld(byMoveSpotNum, GOTOWORLD_STATUS_GOTOWORLD);		
	}
	else
	{
		ChangeLayerUser(pUser, byCurLayerIndex, wDestLayer, byMoveSpotNum);
	}	 
}

void CDungeon::InsertUser(CUser* pUser)
{	
	pUser->m_posDungeonUserList = m_pDungeonUserList->AddHead(pUser);
	
	pUser->InsertToDungeon(this);
	
	++m_wDungeonUserCount;
}

void CDungeon::RemoveUser(CUser* pUser)
{
	if(pUser->m_posDungeonUserList)
		m_wDungeonUserCount--;

	m_pDungeonUserList->RemoveAt(pUser->m_posDungeonUserList);
	pUser->m_posDungeonUserList = NULL;

	CDungeonLayer* pLayer = GetDungeonLayer( pUser->GetCurLayerIndex() );	
	if(!pLayer) return;
	
	if (pUser->m_dwPartyId)
		m_pPartyUserHash->Delete(pUser, pUser->m_dwPartyId);

	if (pUser->m_dwGuildId)
		m_pGuildUserHash->Delete(pUser, pUser->m_dwGuildId);	
	
	pLayer->RemoveUser( pUser );
	pUser->RemoveResource();	
}


// 현재 공격자인지 방어자인지 구별해낸다.
void CDungeon::DistinctionAttackMode()
{
	CUser* pOnwer = g_pUserHash->GetData(m_pInfo->m_dwOwnerIndex);

	if (pOnwer && pOnwer->GetID() == m_pInfo->m_dwOwnerIndex && pOnwer->m_dwPartyId)
		InsertDefensePartyList(pOnwer->m_dwPartyId);

	CUser* pUser = NULL;

	for(int i = 0; i < GetTotalLayer(); ++i)
	{	
		POSITION_ pos = m_pDungeonLayer[i]->m_pPcList->GetHeadPosition();

		while(pos)
		{
			pUser = (CUser*)m_pDungeonLayer[i]->m_pPcList->GetNext(pos);
			pUser->SetAttackMode(GetAttackMode(pUser));			
		}
	}
}

void CDungeon::BroadCast(char* pMsg, DWORD dwLength)
{
	POSITION_	pos		= m_pDungeonUserList->GetHeadPosition();
	CUser*		pUser	= NULL;

	while(pos)
	{
		pUser = (CUser*)m_pDungeonUserList->GetNext(pos);
		NetSendToUser(pUser->m_dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CDungeon::BroadCastInCircle(CDungeonLayer* pDungeonLayer, VECTOR2* v2Center, BYTE byRadiusTileCount, char* pMsg, DWORD dwLength)
{
	CMap*		pMap = pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( v2Center->x, v2Center->y );
	if (!pStartingPointTile)	return;

	CSection* pStartingPointSection = pDungeonLayer->GetSection(pStartingPointTile->wAttr.uSection);
	if (!pStartingPointSection)	return;

	// 색션안에 있는놈들
	POSITION_ pos	= pStartingPointSection->m_pPcList->GetHeadPosition();
	CUser* pUser	= NULL;
		
	while(pos)
	{
		pUser = (CUser*)pStartingPointSection->m_pPcList->GetNext(pos);

		if (IsCollitionByCircle(pDungeonLayer, v2Center, pUser->GetCurPosition(), byRadiusTileCount))
		{
			NetSendToUser(pUser->m_dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);
		}
	}

	// 링크된 섹션안에 있는놈들
	CSection* pLinkSection = NULL;

	for( int i = 0; pStartingPointSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = pDungeonLayer->GetSection(pStartingPointSection->m_wLink[i]);
		if( !pLinkSection ) continue;

		pos = pLinkSection->m_pPcList->GetHeadPosition();

		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			
			if (IsCollitionByCircle(pDungeonLayer, v2Center, pUser->GetCurPosition()
				, byRadiusTileCount))
			{
				NetSendToUser(pUser->m_dwConnectionIndex, pMsg, dwLength, FLAG_SEND_NOT_ENCRYPTION);
			}
		}
	}
}


//========================================================================================
// 인자로 들어온 유저의 현재 섹션과 링크 섹션의 
// 유저와 몬스터의 정보를 인자의 유저에게 보내고 다른유저에게 인자의 유저의 정보를 보낸다.
//========================================================================================
void CDungeon::SendJoinSectionInfo( CUser* pUser  , BYTE bPortal ) const
{
	if( !pUser ) return;

	CDungeonLayer*	pLayer = GetDungeonLayer( pUser->GetCurLayerIndex() );
	if( !pLayer ) return;
	
	CSection* pSection = pLayer->GetSection( pUser->GetPrevSectionNum() );
	if( !pSection ) return;
	
	// 주섹션 정보 교환.
	pLayer->SendSectionInfo( (BYTE)pSection->m_bSectionNum, pUser, bPortal );
	
	// 링크 섹션 정보 교환.
	for( int i=0; pSection->m_wLink[i] != 0; i++ )
	{
		pLayer->SendSectionInfo( (BYTE)pSection->m_wLink[i], pUser, bPortal );
	}
}


// 현재 층의 시피 정보를 보내라.
void CDungeon::SendSiegeInfo(CUser* pUser) const
{
	// 새로 조인한놈에게 현재 층에 있는 cp정보를 준다.
	CDungeonLayer* pLayer = GetDungeonLayer( pUser->GetCurLayerIndex() );
	if( !pLayer ) return;
	
	DSTC_DUNGEON_SIEGEINFO packet;
	packet.wMonsterTotalCount = pLayer->m_wTotalMonster;
	packet.wMonsterCurCount = 0;

	// 일단 짱나더라도 현재 층에 있는 몬스터의 숫를 알아 오라. ㅡ.ㅡ	
	POSITION_ pos = pLayer->m_pMonsterList->GetHeadPosition();

	while(pos)
	{
		CMonster* pMonster = (CMonster*)pLayer->m_pMonsterList->GetNext(pos);
		
		if (pMonster->IsNormalMonster() )
			packet.wMonsterCurCount++;
	}	

	pLayer->m_wCurMonster = packet.wMonsterCurCount;
	pos = pLayer->GetMap()->m_pCPList->GetHeadPosition();

	while(pos)
	{
		// cp 상태도 보내준다.
		CP_DESC* pCPDesc = (CP_DESC*)pLayer->GetMap()->m_pCPList->GetNext(pos);
		
		packet.sCP_Info[packet.bCPTotalCount].bCPID		= pCPDesc->bID;
		packet.sCP_Info[packet.bCPTotalCount].bDestroy	= (BYTE)pCPDesc->bDestroy;
		packet.sCP_Info[packet.bCPTotalCount].wProperty = pCPDesc->wProperty;	
		packet.bCPTotalCount++;
	}

	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
}


BOOL CDungeon::CreateDungeonGuardian()
{
	// 던젼의 최하층을 구한다.
	char szSQL[1024]={0,};
	LPGUARDIAN_CREATION_INFO	pInfo=0;
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(m_pInfo->m_cGuardianItem.GetID());

	// 생성되어 있는것이거나 아이템이 없는경우.
	if( m_pInfo->m_cGuardianItem.m_Item_Guardian.dwID == 0 )
	{
		pInfo = new GUARDIAN_CREATION_INFO;
		memset( pInfo, 0, sizeof( GUARDIAN_CREATION_INFO ) );
		
		pInfo->pDungeon			= this;
		pInfo->bUserGuardian	= TRUE;
		wsprintf( szSQL, "select * from GuardianInfo where [ID]=%d", m_pInfo->m_cGuardianItem.m_Item_Guardian.dwID );	
		g_pDb->THOpenRecord( szSQL, DB_UUID_GUARDIAN_INFO, pInfo, 1, GAME_DB );
	}
	else if( GUARDIAN_CREATE_STATUS_ALREADYCALL == m_pInfo->m_cGuardianItem.m_Item_Guardian.bCreate )
	{
		pInfo = new GUARDIAN_CREATION_INFO;
		memset( pInfo, 0, sizeof( GUARDIAN_CREATION_INFO ) );
		
		pInfo->pDungeon			= this;
		pInfo->bUserGuardian	= TRUE;
		wsprintf( szSQL, "select * from GuardianInfo where [ID] = %d", m_pInfo->m_cGuardianItem.m_Item_Guardian.dwID );	
		g_pDb->THOpenRecord( szSQL, DB_UUID_GUARDIAN_INFO, pInfo, 1, GAME_DB );
	}
	else if( GUARDIAN_CREATE_STATUS_NONE == m_pInfo->m_cGuardianItem.m_Item_Guardian.bCreate )
	{
		// 생성해서 불러줘야한다.
		LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData(pBaseItem->BaseItem_Guardian.wBaseGuardianID);
		if( !pBaseMonster ) return FALSE;
	
		pInfo = new GUARDIAN_CREATION_INFO;
		memset( pInfo, 0, sizeof( GUARDIAN_CREATION_INFO ) );
		pInfo->bUserGuardian	= FALSE;
		pInfo->pItem			= &m_pInfo->m_cGuardianItem;
		m_pInfo->m_cGuardianItem.m_Item_Guardian.bCreate = GUARDIAN_CREATE_STATUS_CREATING;	// 생성중이란표시.
		
		// 처음 생성될떄의 HP와 MP를 구한다. // 여긴 왜 지역변수에 다시 저장해서 쓸까???-_-
		DWORD dwHP = pBaseMonster->dwBaseHP;
		DWORD dwMP = pBaseMonster->wBaseMP;
	
		wsprintf( szSQL, "CZP_INSERT_GUARDIAN_INFO '%s', %d, %d, %d", pBaseMonster->szMonsterName_Kor
			, pBaseItem->BaseItem_Guardian.wBaseGuardianID, dwHP, dwMP );
		g_pDb->THOpenRecord( szSQL, DB_CREATE_GUARDIAN, pInfo, 1, GAME_DB );
	}

	return TRUE;
}

BOOL CDungeon::RemoveDungeonGuardian()
{
	if( !m_pDungeonGuardian ) return FALSE;

	DSTC_DISAPPEAR_MON	DisAppearMon;
	DisAppearMon.dwMonsterIndex = m_pDungeonGuardian->GetID();
	
	CDungeonLayer *pLayer = m_pDungeonGuardian->GetCurDungeonLayer();
	if(pLayer)
	{
		pLayer->BroadCastSectionMsg( (char*)&DisAppearMon, DisAppearMon.GetPacketSize()
		, (BYTE)m_pDungeonGuardian->GetCurSection()->m_bSectionNum );
	}
	
	UpdateMonsterForAI( m_pDungeonGuardian );

	m_pDungeonGuardian->SetStatus(UNIT_STATUS_DEAD);
		
	return TRUE;
}

void CDungeon::RemoveAllUserMonster( CUser* pUser )
{
	CMonster* pMonster = NULL;

	for( BYTE i = 0; i < MAX_USER_GUARDIAN; i++ )
	{
		if (pUser->IsSummonGuardian(i))
		{
			pUser->UnSummonGuardian(i);
			SendSummonEggStatus(pUser->m_dwConnectionIndex, FALSE, pUser->GetItem(ITEM_NATIVE_INV_GUARDIAN, i));
		}
		
		pMonster = pUser->GetGuardian(i);
		
		if( pMonster )
		{
			pMonster->SetStatus( UNIT_STATUS_DEAD);
			
			UpdateMonsterForAI( pMonster );
		}

		pMonster = pUser->m_pMonster[i];

		if( pMonster )
		{
			if (pMonster->IsElemental() && pMonster->GetHP())
			{
				if (pMonster->GetEffectDesc(__SKILL_BLUEELEMENTAL__))
					pMonster->DetachSkill(pMonster->GetEffectDesc(__SKILL_BLUEELEMENTAL__));
				else if (pMonster->GetEffectDesc(__SKILL_GREENELEMENTAL__))
					pMonster->DetachSkill(pMonster->GetEffectDesc(__SKILL_GREENELEMENTAL__));
				else if (pMonster->GetEffectDesc(__SKILL_REDELEMENTAL__))
					pMonster->DetachSkill(pMonster->GetEffectDesc(__SKILL_REDELEMENTAL__));

			}
			else if(pMonster->IsMindControll())
			{
				if (pMonster->GetEffectDesc(__SKILL_MINDCONTROL__))
					pMonster->DetachSkill(pMonster->GetEffectDesc(__SKILL_MINDCONTROL__));
				if (pMonster->GetEffectDesc(__SKILL_MINDEXPLOSION__))
					pMonster->DetachSkill(pMonster->GetEffectDesc(__SKILL_MINDEXPLOSION__));
			}

			UpdateMonsterForAI( pMonster );
		}
	}
}

void CDungeon::GotoLobbyUser(CUser* pUser) const
{
	if (!pUser->GetCurDungeon())	return;

	VECTOR2 vec = {0.f, 0.f};
	m_pDungeonLayer[0]->GetStartTile(0, &vec);
	
	pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);
	RemoveUserForInsidePortal(pUser);
	SendPortalMsg(pUser, 0, &vec, false);
	pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] = 1;	
}


// 던전 공성전 시작하라.
void CDungeon::StartSiege()
{
	m_pInfo->m_bSiege = true;
	m_pInfo->m_dwOriginalOwnerIndex = m_pInfo->m_dwOwnerIndex;

	SetDungeonOwner(m_pInfo->m_dwOwnerIndex, m_pInfo->m_dwOwnerGuildNum, 0, NULL, m_pInfo->m_szOwner);

	DistinctionAttackMode();
	
	for(int i = 0; i < GetTotalLayer(); ++i)
	{
		ReviveAllMonster();

		CVoidList* pCPList	= m_pDungeonLayer[i]->GetMap()->m_pCPList;
		POSITION_ pos		= pCPList->GetHeadPosition();

		m_pDungeonLayer[i]->GetMap()->m_bCPRemainCount = (BYTE)pCPList->GetCount();

		// cp에 함정속성 적용시키기.
		while(pos)
		{
			CP_DESC* pCPDesc = (CP_DESC*)pCPList->GetNext(pos);
			pCPDesc->bDestroy	= 0; // cp 생성.
			pCPDesc->dwStartTick= 0;
			pCPDesc->wProperty	= WORD(rand() % MAX_CP_TABLE);			
		}
		
		// 공격모드유저들에게 알려줘야지.		
		POSITION_ position = m_pDungeonLayer[i]->m_pPcList->GetHeadPosition();

		while(position)
		{
			CUser* pUser = (CUser*)m_pDungeonLayer[i]->m_pPcList->GetNext(position);
							
			if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
			{
				if (i)
				{
					GotoLobbyUser(pUser);
					continue;
				}
				else
				{
					pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] = 1;
				}
			}

			SendSiegeInfo(pUser);				
		}				
	}	
}


// 공성전 끝났군.
void CDungeon::EndSiege()
{
	m_pInfo->m_bSiege = false;	
	
	for(int i = 0; i < GetTotalLayer(); ++i)
	{
		POSITION_ pos = m_pDungeonLayer[i]->GetMap()->m_pCPList->GetHeadPosition();
		while(pos)
		{
			CP_DESC* pCPDesc		= (CP_DESC*)m_pDungeonLayer[i]->GetMap()->m_pCPList->GetNext(pos);
			pCPDesc->bDestroy		= 1;		// cp 파괴
			pCPDesc->dwStartTick	= 0;
		}

		// 초기화를 해야 내려갈수 있어야 한다.
		m_pDungeonLayer[i]->GetMap()->m_bCPRemainCount = 0;	
		
		// 공격모드유저들에게 알려줘야지.		
		POSITION_ position = m_pDungeonLayer[i]->m_pPcList->GetHeadPosition();
		while(position)
		{
			CUser* pUser = (CUser*)m_pDungeonLayer[i]->m_pPcList->GetNext(position);
			
			DSTC_ATTACK_MODE packet;
			packet.dwUserIndex = pUser->GetID();
			packet.bAttackMode = 0;

			CDungeonLayer*	pLayer = GetDungeonLayer( pUser->GetCurLayerIndex() );
			if( !pLayer ) continue;

			pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum() );
			pUser->SetAttackMode(ATTACK_MODE_NONE);
							
			pUser->DetachMagicFieldArray(&pUser->m_UseMagicFieldArrayDesc);				

			pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] = 0;				
		}		
		
		// 죽은 몬스터가 있으면 바로 살려내라.
		ReviveAllMonster();		
	}	
	
	// 오 방어 했네?
	if (m_pInfo->m_dwOwnerIndex)
	{
		// 원소 속성석이 아니라면
		if (g_DungeonProductionItemMinMax[m_pInfo->m_byProperty].wItemIDDefault != m_pInfo->m_wProduction)
		{
			if (m_pInfo->m_cMagicFieldArrayItem.GetID())
			{
				if(m_pInfo->m_cMagicFieldArrayItem.GetQuantity()>1)
					m_pInfo->m_cMagicFieldArrayItem.SetQuantity(BYTE(m_pInfo->m_cMagicFieldArrayItem.GetQuantity()-1));
				else
					memset(&m_pInfo->m_cMagicFieldArrayItem, 0, sizeof(CItem));
			}			
		}
	
		if (m_pInfo->m_dwOriginalOwnerIndex == m_pInfo->m_dwOwnerIndex)
		{
			m_pInfo->CreateProduction(TRUE);
			m_pInfo->SetDefenseCount(BYTE(m_pInfo->m_byDefenseCount<0xff?m_pInfo->m_byDefenseCount+1:0xff), TRUE);
		}
		else
		{
			m_pInfo->CreateProduction(FALSE); 
			m_pInfo->SetDefenseCount(0, TRUE);
		}

		m_pInfo->SetMagicFieldArray(&m_pInfo->m_cMagicFieldArrayItem);
	}				
	
	m_pInfo->m_dwOriginalOwnerIndex = m_pInfo->m_dwOwnerIndex;
	RemoveAllDefencePartyList();
	RemoveDungeonGuardian();	
}


// 가디언을 잡았지만 공성은 아직 끝나지 않았을때 다시 공성초기화를 해줘.
void CDungeon::RestartSiege()
{	
	RemoveAllDefencePartyList();
	DistinctionAttackMode();
	
	// 공격모드유저들에게 알려줘야지.
	for(int i = 0; i < GetTotalLayer(); ++i)
	{
		// 마지막 층은 다시 생성해줘.
		if (GetTotalLayer()-1 == i) 
		{			
			POSITION_ pos = m_pDungeonLayer[i]->GetMap()->m_pCPList->GetHeadPosition();

			while(pos)
			{
				CP_DESC* pCPDesc = (CP_DESC*)m_pDungeonLayer[i]->GetMap()->m_pCPList->GetNext(pos);
				pCPDesc->bDestroy	= 0; // cp 생성.
				pCPDesc->dwStartTick= 0;
				pCPDesc->wProperty	= WORD(rand() % MAX_CP_TABLE);
			}

			m_pDungeonLayer[i]->GetMap()->m_bCPRemainCount = BYTE(m_pDungeonLayer[i]->GetMap()->m_pCPList->GetCount());
		}
		
		POSITION_ pos = m_pDungeonLayer[i]->m_pPcList->GetHeadPosition();

		while(pos)
		{
			CUser* pUser = (CUser*)m_pDungeonLayer[i]->m_pPcList->GetNext(pos);
							
			if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
			{
				if (i)
				{
					GotoLobbyUser(pUser);
					continue;
				}
				else
				{
					pUser->m_dwTemp[USER_TEMP_FLAG_LOBBY] = 1;
				}
			}
			
			SendSiegeInfo(pUser);			
		}			
	}	
}


// 공성 끝날때 마지막으로 가디언 죽인놈을 기억하기 위해 사용한다.
void CDungeon::SetDungeonOwner(DWORD dwUserIndex, DWORD dwGuildID, DWORD dwPartyID, char* pLoginID, const char* const pCharacterName)
{
	memset(m_pChangeDungeon, 0, sizeof(DB_CHANGE_DUNGEON));
	m_pChangeDungeon->pDungeon		= this;
	m_pChangeDungeon->dwUserIndex	= dwUserIndex;
	m_pChangeDungeon->dwUserGuildId	= dwGuildID;
	m_pChangeDungeon->dwUserPartyId	= dwPartyID;
	
	if (pLoginID)
		__lstrcpyn(m_pChangeDungeon->szLoginID, pLoginID, MAX_CHARACTER_REAL_LENGTH);

	__lstrcpyn(m_pChangeDungeon->szName, (char*)pCharacterName, MAX_CHARACTER_REAL_LENGTH);

	
	DWORD dwChangeUserIndex = (dwUserIndex ^ m_pInfo->m_dwOwnerIndex);
	
	if (dwChangeUserIndex || (dwGuildID ^ m_pInfo->m_dwOwnerGuildNum))
	{
		if(IsGuildWar(this, dwGuildID, m_pInfo->m_dwOwnerGuildNum, 0, 0, GODMODE_STATUS_RELEASE))
		{
			SendToWorldServerForGuildPoint(dwUserIndex, 0, 100, dwGuildID, m_pInfo->m_dwOwnerGuildNum);
		}		

		char szUpdate[255]={0,};
		wsprintf( szUpdate, "update mapinfo set OwnerID = '%s', Owner = '%s', OwnerGuildNum = %d, OwnerIndex = %d where id = %d"
			, m_pChangeDungeon->szLoginID
			, m_pChangeDungeon->szName
			, m_pChangeDungeon->dwUserGuildId
			, m_pChangeDungeon->dwUserIndex
			, GetID());

		g_pDb->THExecuteSQL( szUpdate, TRUE, DB_CHANGE_DUNGEON_OWNER, m_pChangeDungeon, GAME_DB );
	}
}


// 죽었던 몬스터 전부 살려줘라.
void CDungeon::ReviveAllMonster()
{
	//죽은 몬스터가 있으면 일정시간뒤 살린다.
	for(int i = 0; i < GetTotalLayer(); ++i)
	{
		POSITION_ MonsterPos = m_pDungeonLayer[i]->m_pResponseMonsterList->GetHeadPosition();
		while( MonsterPos )
		{
			CMonster* pMonster = (CMonster*)m_pDungeonLayer[i]->m_pResponseMonsterList->GetNext( MonsterPos );
			m_pDungeonLayer[i]->ReviveMonster(pMonster, 0);
		}
	}	
}


//========================================================
// 던젼 내부의 모든 유저를 월드로 안내(?) 한다. 전체 유저를 월드맵으로 쫓아버리는 처리.	
//========================================================
void CDungeon::BanAllUser()
{	
	POSITION_	UserPos	= 0;
	CUser*		pUser	= 0;
	int			i		= 0;

	if(NULL == m_pInfo)		return;
	
	for(i = 0; i < GetTotalLayer(); i++)
	{
		UserPos = m_pDungeonLayer[i]->m_pPcList->GetHeadPosition();

		while( UserPos )
		{
			pUser = (CUser*)m_pDungeonLayer[i]->m_pPcList->GetNext( UserPos );		

			if(DUNGEON_TYPE_EVENT == m_pInfo->GetDungeonType())
			{
				// 편법을 이용하여 마을 주변으로 이동시킴.
				pUser->GoToWorld(0, GOTOWORLD_STATUS_GOTOWORLD);
			}
			else
			{
				pUser->GoToWorld(pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD);
			}			
		}
	}
}


//========================================================
// 몬스터 제거 후 bCreateMonster == TRUE면 생성 이벤트 던젼의 진행시간이 끝났거나, 클리어 되었을 때
//========================================================
void CDungeon::ResetMonster(BOOL bCreateMonster)
{
	for(int i = 0; i < GetTotalLayer(); i++)
	{
		if(bCreateMonster)
		{
			m_pDungeonLayer[i]->ResetMonster();
		}
		else 
		{
			m_pDungeonLayer[i]->ResetMonster(bCreateMonster);
		}
	}
}


//========================================================
// 누적되어 있는 경험치를 모든 유저에게 먹게한다.
//========================================================
void CDungeon::AddAllAccumulationExp(VOID) const
{
	CUser*		pUser	= NULL;
	POSITION_	pos		= m_pDungeonUserList->GetHeadPosition();	

	while(pos)
	{
		pUser = (CUser*)m_pDungeonUserList->GetNext(pos);
		pUser->AddExp(pUser->m_dwAccumulationExp);				
		pUser->m_dwAccumulationExp = 0;
	}
}


//===================================================================
// 이미 획득한 경험치중, 누적된 것들을 빼버린다.(이벤트 던젼 실패시.)
//===================================================================
void CDungeon::SubAllAccumulationExp(VOID)
{
	CUser* pUser=0;
	POSITION_ pos = m_pDungeonUserList->GetHeadPosition();

	while(pos)
	{
		pUser = (CUser*)m_pDungeonUserList->GetNext(pos);
		pUser->SubExp(pUser->m_dwAccumulationExp);				
		pUser->m_dwAccumulationExp = 0;
	}
}


//===================================================================
// 보너스 경험치 퍼센트 +
//===================================================================
void CDungeon::PlusAccumulationExp(LONG byPercent) const
{	
	CUser*		pUser	= NULL;
	DWORD		dwExp	= 0;

	POSITION_	pos		= m_pDungeonUserList->GetHeadPosition();
	while(pos)
	{
		pUser = (CUser*)m_pDungeonUserList->GetNext(pos);
		dwExp = (	GetExpTableOfLevel(OBJECT_TYPE_PLAYER, (BYTE)pUser->GetLevel() + 1) - 
					GetExpTableOfLevel(OBJECT_TYPE_PLAYER, (BYTE)pUser->GetLevel()) ) / (100 / byPercent);
		pUser->m_dwAccumulationExp += dwExp;			
	}
}


//===================================================================
// 보너스 경험치 퍼센트 - 
//===================================================================
void CDungeon::MinusAccumulationExp(BYTE byPercent)
{
	CUser*		pUser	= NULL;
	DWORD		dwExp	= 0;

	POSITION_	pos		= m_pDungeonUserList->GetHeadPosition();
	while(pos)
	{
		pUser = (CUser*)m_pDungeonUserList->GetNext(pos);
		dwExp = (	GetExpTableOfLevel(OBJECT_TYPE_PLAYER, (BYTE)pUser->GetLevel() + 1) -
					GetExpTableOfLevel(OBJECT_TYPE_PLAYER, (BYTE)pUser->GetLevel()) ) / (100 / byPercent);
		pUser->m_dwAccumulationExp -= dwExp;			
	}
}


//===================================================================
// 1차 이벤트 던전 클리어 처리 
//===================================================================
void CDungeon::EventFirstTimeUpProcess()
{
	if(NULL == m_pEventDungeonProperty)		return;

	if( EVENT_DUNGEON_SURVIVAL == m_pEventDungeonProperty->byDungeonType )
	{
		if(m_pEventDungeonProperty->byFlagExpStyle)
		{
			AddAllAccumulationExp();
		}

		ResetMonster(FALSE);
	}
	
	m_dwEventFirstCloseTick = g_dwTickCount;
}


//=========================================================
// 이벤트 던젼 클리어 처리.
//=========================================================
void CDungeon::EventClearProcess()
{	
	m_dwEventClearTick		= g_dwTickCount;
	m_dwEventFirstCloseTick = 0;
}


//=========================================================
// 이벤트 던젼 시간 종료 처리.
//=========================================================
void CDungeon::EventTimeUpProcess()
{
	if(NULL == m_pEventDungeonProperty)		return;	

	if( EVENT_DUNGEON_SURVIVAL == m_pEventDungeonProperty->byDungeonType )
	{
		Event_SurvivalClear( this );
	}

	if( EVENT_DUNGEON_TRAINING == m_pEventDungeonProperty->byDungeonType )
	{
		DSTC_EVENT_DUNGEON_FAILED packet;
		BroadCast((char*)&packet, packet.GetPacketSize());
	}

	ResetMonster(FALSE);
}


//=========================================================
// 생존 1차 클리어 되면 모든 유저 PK모드 
//=========================================================
void CDungeon::AllUserPKMode(BOOL bIsPKMode)
{	
	CUser*		pUser	= NULL;
	POSITION_	pos		= m_pDungeonUserList->GetHeadPosition();

	while(pos)
	{
		pUser = (CUser*)m_pDungeonUserList->GetNext(pos);

		if(TRUE != pUser->GetMatchDescInfo()->m_bMatching)
		{
			// 대전중이지 않을 때만 
			pUser->SetPKMode(bIsPKMode);
		}
	}
}


//=========================================================
// 월드에 이벤트 던젼 포탈로 들어갔다고 알려준다.
//=========================================================
void CDungeon::ReportUserEnterEventDungeonByPortal(WORD wDungeonID)
{	
	if(NULL == m_pInfo)		return;
	
	DSTWS_EVENT_DUNGEON_USER_ENTER_BY_PORTAL SendPacket;
	SendPacket.wDungeonID = wDungeonID;

	g_pNet->SendToServer(WSINDEX, (char*)&SendPacket, SendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);	
}


DUNGEON_DATA_EX* CDungeon::GetDungeonDataEx() const 
{
	return m_pInfo;
}

const EVENT_DUNGEON_PROPERTY* const CDungeon::GetEventDungeonProperty() const
{
	return m_pEventDungeonProperty;
}

void CDungeon::SetEventDungeonProperty(EVENT_DUNGEON_PROPERTY* const pEventDungeonProperty)
{
	m_pEventDungeonProperty = pEventDungeonProperty;
}

PARTYUSER_HASH* CDungeon::GetPartyUserHash() const
{
	return m_pPartyUserHash;
}

GUILDUSER_HASH* CDungeon::GetGuildUserHash() const
{
	return m_pGuildUserHash;
}

void CDungeon::SetDungeonGuardian(CMonster* const pMonster)
{
	m_pDungeonGuardian = pMonster;
}

const CMonster*	CDungeon::GetDungeonGuardian()
{
	return m_pDungeonGuardian;
}

CVoidList* CDungeon::GetDungeonUserList() const 
{ 
	return m_pDungeonUserList;	
}

CDungeon*	CDungeon::GetPrevDungeon() const
{
	return m_pPrevDungeon;
}

CDungeon*	CDungeon::GetNextDungeon() const
{
	return m_pNextDungeon;
}

void CDungeon::SetPrevDungeon(CDungeon* const pDungeon)
{
	m_pPrevDungeon = pDungeon;
}

void CDungeon::SetNextDungeon(CDungeon* const pDungeon)
{
	m_pNextDungeon = pDungeon;
}

CDungeonLayer* CDungeon::GetDungeonLayer(const BYTE byLayerIndex) const
{
	if (byLayerIndex >= MAX_LAYER_PER_DUNGEON)
		return NULL;
		
	return m_pDungeonLayer[byLayerIndex];
}

CVoidList* CDungeon::GetRemovePCList()	const
{
	return m_pRemovePCList;
}

DWORD CDungeon::GetEventClearTick() const
{
	return m_dwEventClearTick;
}

DWORD CDungeon::GetEventFirstCloseTick() const
{
	return m_dwEventFirstCloseTick;
}

void CDungeon::SetEventFirstCloseTick(DWORD dwEventFirstCloseTick)
{
	m_dwEventFirstCloseTick = dwEventFirstCloseTick;
}

void CDungeon::SetEventClearTick(DWORD dwEventClearTick)
{
	m_dwEventClearTick = dwEventClearTick;
}

DWORD CDungeon::GetTemp(DUNGEON_TEMP eTemp_Kind)
{
	assert(eTemp_Kind < DUNGEON_TEMP_MAX);
	return m_dwTemp[eTemp_Kind];
}

void CDungeon::SetTemp(DUNGEON_TEMP eTemp_Kind, DWORD dwValue)
{
	assert(eTemp_Kind < DUNGEON_TEMP_MAX);
	m_dwTemp[eTemp_Kind] = dwValue;
}


// 이벤트 던젼의 종류를 리턴한다.
int	CDungeon::GetEventDungeonType()
{
	if (NULL != m_pEventDungeonProperty)
	{
		return m_pEventDungeonProperty->byDungeonType;
	}

	return -1;
}


// 현재 이벤트 던전의 난이도를 리턴한다.
int	CDungeon::GetEventDungeonLevel(VOID)
{
	switch(m_wID)
	{
		case 5003:return 0;
		case 5004:return 1;
		case 5007:return 2;
		case 5008:return 3;
		default:return -1;
	}
}

