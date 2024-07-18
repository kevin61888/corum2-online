#include "stdafx.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "../CommonServer/CommonHeader.h"
#include "OwnServer.h"
#include "User.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "DungeonLayer.h"
#include "Map.h"
#include "TraderMonster.h"
#include "GameSystemNative.h"
#include "EventDungeonManager.h"
#include "MsgProc_for_jjw6263.h"


DWORD	g_pdwExp[MAX_LEVEL +1];				// 레벨당 경험치.
DWORD	g_pdwGuardianExp[MAX_LEVEL +1];		// 레벨당 경험치.
BYTE	g_byStstus[MAX_LEVEL];
BYTE	g_byGuardianStatus[MAX_LEVEL];


BOOL SetChatLogInDungeon(CHAT_TYPE eChatType, CUser * pUser, CUser * pRecvUser, char * pszChat, int ChatLlen)
{	
	if (!g_bEnableChatLog)	
	{
		return FALSE;
	}

	if (NULL == pUser)	
	{
		return FALSE;
	}

	if (pszChat == NULL)
	{
		return FALSE;
	}

	if (!pUser->GetCurDungeon())
	{
		return FALSE;
	}

	CHAT_LOG	ChatLog;
	
	//DB Updates..
	char		szQuery[1024]={0,};
	int			nColumn			= 6;	
	int			col				= 0;
	DWORD		dwParamOffset	= 0;
	DBBINDING*	pBinding		= g_pLogDb->THCreateParamInfo(nColumn);	

	if(!pBinding)
	{
		::Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetItemLog Function()!");
		return 0;
	}
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(ChatLog.bMessageType);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(ChatLog.bMessageType);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof( ChatLog.bUserType);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(ChatLog.bUserType);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof( ChatLog.szSender);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(ChatLog.szSender);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof( ChatLog.szRecver);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(ChatLog.szRecver);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(ChatLog.wDungeonID);
	pBinding[col++].wType	= DBTYPE_I2;		
	dwParamOffset			+= sizeof(ChatLog.wDungeonID);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(ChatLog.szContents);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(ChatLog.szContents);

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	wsprintf( 
		szQuery, 
		"Insert into Chat_Log_%d_%d%02d%02d (MessageType, UserType, Sender, Recver, DungeonID,  Contents) values( ?, ?, ?, ?, ?, ?)",
		g_pThis->GetServerSetNum(), 
		CurTime.wYear, CurTime.wMonth, CurTime.wDay
		);

	// 로그에 남길내용을 넣어준다. 
	ChatLog.bMessageType	= (BYTE)eChatType;
	ChatLog.bUserType		= (BYTE)pUser->m_bUserType; 
	ChatLog.wDungeonID		= (WORD)pUser->GetCurDungeonID();
	__lstrcpyn(ChatLog.szContents, pszChat, ChatLlen);
	__lstrcpyn(ChatLog.szSender, pUser->m_szName, lstrlen(pUser->m_szName));
	if (pRecvUser)
	{
		__lstrcpyn(ChatLog.szRecver, pRecvUser->m_szName, lstrlen(pRecvUser->m_szName));
	}
	else
	{
		memset(ChatLog.szRecver, 0, sizeof(ChatLog.szRecver));
	}
	
	g_pLogDb->THExecuteSQLByParam( szQuery, 
		pBinding, 
		&ChatLog.bMessageType, 
		sizeof(ChatLog), 
		nColumn, 
		FALSE, 
		0, 
		NULL, 
		LOG_DB
		);
	
	return TRUE;	
}


BOOL SpeedHackLog( CUser* pUser, DWORD dwHackApplication )
{
	if (!pUser->GetCurDungeon())	return FALSE;
	else if(!g_bEnableSpeedHackLog)	return FALSE;

	SPEEDHACK_LOG	Speed;
	memset( &Speed, 0, sizeof( SPEEDHACK_LOG ) );
	
	__lstrcpyn( Speed.szName, (LPSTR)pUser->GetName(), MAX_CHARACTER_NAME_LENGTH );
	Speed.bServerSet	= (BYTE)g_pThis->GetServerSetNum();
	Speed.dwDungeonID	= pUser->GetCurDungeonID();

	char szQuery[1024]={0,};
	
	wsprintf( szQuery, "Insert into SpeedHackLog(Name, Serverset, DungeonID, HackApplication) Values( '%s', %d, %d, %d )", 
		Speed.szName, Speed.bServerSet, Speed.dwDungeonID, dwHackApplication  );
	g_pLogDb->THExecuteSQL(szQuery, FALSE, FALSE, NULL, LOG_DB);

	return TRUE;
}


BOOL SpeedHackCheck_Attack( CUser* pUser, BOOL bUserMon )
{
	if( pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] == 0 )
		return FALSE;

	Log(LOG_NORMAL, "Attack Tick %d", g_dwTickCount - pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] );

	if( ( g_dwTickCount - pUser->m_dwTemp[ USER_TEMP_LASTATTACKTICK ] ) < (DWORD)( pUser->GetAttackSpeed() + 500 ) )
	{
		if( bUserMon )
		{
			DSTC_ATTACK_USER_MON	AttackMon;

			AttackMon.dwUserIndex	= pUser->GetID();
			AttackMon.bType			= 5;
			NetSendToUser( pUser->m_dwConnectionIndex, (char*)&AttackMon, AttackMon.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
		}
		else
		{
			DSTC_ATTACK_USER_USER	AttackUser;

			AttackUser.dwOffenseUserIndex	= pUser->GetID();
			AttackUser.bType				= 5;
			NetSendToUser( pUser->m_dwConnectionIndex, (char*)&AttackUser, AttackUser.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}

		return TRUE;
	}
 
	return FALSE;	
}


int AttackResult_Unit_Unit( CUnit* pAttackUnit, CUnit* pDefenseUnit, LPATTACK_RESULT pResult )
{
	if( !pAttackUnit )			return 0;
	else if( !pDefenseUnit )	return 0;
	else if(pDefenseUnit->GetUnitStatus() == UNIT_STATUS_DEAD 	|| !pDefenseUnit->GetHP())	return 0;
	
	if( pAttackUnit->GetEffectDesc(__SKILL_HIDING__) )
	{
		pAttackUnit->DetachSkill( pAttackUnit->GetEffectDesc(__SKILL_HIDING__) );
	}

	if( pDefenseUnit->GetEffectDesc(__SKILL_SLEEP__) )
	{
		pDefenseUnit->DetachSkill( pDefenseUnit->GetEffectDesc(__SKILL_SLEEP__) );
	}

	if (pDefenseUnit->GetEffectDesc(__SKILL_SOULETER__))
	{
		pDefenseUnit->DetachSkill(pDefenseUnit->GetEffectDesc(__SKILL_SOULETER__));
	}

	if (pDefenseUnit->GetEffectDesc(__SKILL_CALMDOWN__))	// 추가 : 최덕석 2005.1.14
	{
		pDefenseUnit->DetachSkill(pDefenseUnit->GetEffectDesc(__SKILL_CALMDOWN__));
	}

	int		iRand = 0;
	WORD	pwAttackDamage[2]={0,0};
	
	pAttackUnit->GetAttackingDamage(pAttackUnit->GetAttackDamageMin_L()
		, pAttackUnit->GetAttackDamageMax_L()
		, &pwAttackDamage[0]
		, &pwAttackDamage[1]
		, SELECT_ATTACK_TYPE_LBUTTON);	
	
	if( pDefenseUnit->IsAvoidSuccess(pAttackUnit) )
	{
		pResult->byType = 2;		// Miss
		goto lb_exit;
	}
	else if( pDefenseUnit->IsBlockSuccess() )
	{
		pResult->byType = 1;		// Block.
		goto lb_exit;
	}
	
	iRand = rand()%( pwAttackDamage[1] - pwAttackDamage[0] + 1 );
	iRand += pwAttackDamage[0];
	iRand = GetPropertyRealDamageByResist(	
				SKILL_PROPERTY_KIND_PHY
				, iRand
				, BYTE(pAttackUnit->GetObjectType())
				, pAttackUnit->GetID()
				, BYTE(pDefenseUnit->GetObjectType())
				, pDefenseUnit->GetID()
			);
	
	if( iRand <= 0 )
	{
		iRand	= 1;
	}

	pResult->byType		= 4;
	pResult->dwDamage	= pDefenseUnit->GetReduceDamageForObject(iRand, pAttackUnit);
	
	pDefenseUnit->SetDamageOver( pAttackUnit, pResult->dwDamage );

	SetBadAction(pAttackUnit, pDefenseUnit);
	
lb_exit:
	pAttackUnit->SetAttackTypeAndFrame( pDefenseUnit, pResult->byType );
	return 1;
}


void SetBadAction(CUnit* pOffense, CUnit* pDefense)
{
	if (!pOffense || !pDefense)
		return;

	if (pOffense->GetObjectType() == OBJECT_TYPE_PLAYER
		&& pDefense->GetObjectType() == OBJECT_TYPE_PLAYER)
	{
		if (pOffense->IsBadAction(pDefense))
			pOffense->SetBad(true, CRIMINAL_USER_ATTACK, 0);
	}
	else if (pOffense->GetObjectType() == OBJECT_TYPE_PLAYER
		&& pDefense->GetObjectType() == OBJECT_TYPE_MONSTER)
	{
		if (pOffense->IsBadAction(pDefense))
			pOffense->SetBad(true, CRIMINAL_ATTACK_SUMMON_BY_USER, 0);
	}
	else if (pOffense->GetObjectType() == OBJECT_TYPE_MONSTER
		&& pDefense->GetObjectType() == OBJECT_TYPE_PLAYER)
	{
		if (pOffense->IsBadAction(pDefense))
			pOffense->SetBad(true, CRIMINAL_ATTACK_USER_BY_SUMMON, 0);
	}
	else if (pOffense->GetObjectType() == OBJECT_TYPE_MONSTER
		&& pDefense->GetObjectType() == OBJECT_TYPE_MONSTER)
	{
		if (pOffense->IsBadAction(pDefense))
			pOffense->SetBad(true, CRIMINAL_ATTACK_USER_BY_SUMMON, 0);
	}	
}

// overloaded by minjin. 2004. 11. 16.
// for more simple function interface
BOOL IsGuildWar(CUser *pOwnUser, CUser* pTargetUser)
{
	CDungeonLayer *pLayer = pOwnUser->GetCurDungeonLayer();//길드전인가 체크 : hwoarang 050202
	if(pLayer)
	{
		if(pLayer->m_pTeamMatch)
		{
			return pLayer->m_pTeamMatch->OnAttack(pOwnUser, pTargetUser);
		}
	} // if(pLayer)

	return IsGuildWar(pOwnUser->GetCurDungeon()
				, pOwnUser->m_dwGuildId
				, pTargetUser->m_dwGuildId
				, pOwnUser->m_dwPartyId
				, pTargetUser->m_dwPartyId
				, pTargetUser->m_enGodMode);
}

BOOL IsGuildWar(const	CDungeon*	pDungeon,
						DWORD		dwOffenseGuildWarID,
						DWORD		dwDefenseGuildWarID,
						DWORD		dwOffencePartyId,
						DWORD		dwDefencePartyId,
						ENUM_GODMODE_STATUS eGodMode)
{
	GUILD_NODE	sGuildNode;
	GUILD_NODE* lpGuildNode = NULL;

	if(dwDefenseGuildWarID > dwOffenseGuildWarID)
	{
		sGuildNode.IdStruct.dwLowId	= dwOffenseGuildWarID;
		sGuildNode.IdStruct.dwHiId	= dwDefenseGuildWarID;
	}
	else if(dwDefenseGuildWarID < dwOffenseGuildWarID)
	{
		sGuildNode.IdStruct.dwLowId	= dwDefenseGuildWarID;
		sGuildNode.IdStruct.dwHiId	= dwOffenseGuildWarID;
	}
	else
	{
		sGuildNode.IdStruct.dwLowId	= 0;
		sGuildNode.IdStruct.dwHiId	= 0;
	}

	lpGuildNode = g_pGuildListHash->GetData(sGuildNode.i64Id);

	BOOL bChk = FALSE;

	if(lpGuildNode && !pDungeon->GetDungeonDataEx()->IsVillage())
		bChk = TRUE;

	if(dwOffencePartyId!=0 && dwOffencePartyId == dwDefencePartyId)
		bChk = FALSE;	

	if(eGodMode == GODMODE_STATUS_MAPLOADING)
		bChk = FALSE;	

	return bChk;
}


void SendDungeonJoinFailMessage(DWORD dwConnectionIndex, DUNGEON_JOIN_FAIL eDungeonFail)
{
	DSTC_DUNGEON_JOIN_FAILED packet;
	packet.bErrorCode = (BYTE)eDungeonFail;
	NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


BOOL EpsilonVector(const VECTOR2* const v2Vec1, const VECTOR2* const v2Vec2, float fEpsilon )
{
	return ( CalcDistance( v2Vec1, v2Vec2 ) < fEpsilon ) ? TRUE : FALSE;
}


CUnit*	CreateUnitInstance(UNIT_CLASS_ID eClsID)
{
	CUnit* pUnit = NULL;
	switch(eClsID)
	{
	case UNIT_CLASS_ID_USER:
		{
			pUnit = new CUser;
		}
		break;
	case UNIT_CLASS_ID_MONSTER:
		{
			pUnit = new CMonster;
		}
		break;
	case UNIT_CLASS_ID_TRADER_MONSTER:
		{
			pUnit = new CTraderMonster;
		}
		break;
	}

	return pUnit;
}


void MakeAppearMonser( DSTC_APPEAR_MON* pAppearMon, CMonster* pMonster )
{
	pAppearMon->bMonsterKind	= (BYTE)pMonster->GetRace();
	pAppearMon->bClass			= (BYTE)pMonster->GetClass();
	pAppearMon->dwLevel			= pMonster->GetLevel();

	if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN )
	{
		__lstrcpyn( pAppearMon->szMonsterName, (LPSTR)pMonster->GetName(), sizeof(pAppearMon->szMonsterName) );

		if( !pMonster->GetTemp(NPC_TEMP_DUNGEONGUARDIAN) )
		{
			pAppearMon->dwLordIndex	= pMonster->GetLord()->GetID();
			pAppearMon->bZipCode	= (BYTE)pMonster->GetTemp(NPC_TEMP_SLOT);
			pAppearMon->bScale		= pMonster->GetBaseMonsterInfo()->bScale;
		}
		else
		{
			pAppearMon->dwLordIndex	= 0xffffffff; // 땜 .. 클라이언트에서 유저와 연결시키지 않기 위해서.
			pAppearMon->bScale		= 10;
		}
			
	}
	else
	{
		if( pMonster->GetLord() )		// 서모닝 몬스터와 마인드 컨드롤 몬스터.
		{
			pAppearMon->dwLordIndex = pMonster->GetLord()->GetID();
			pAppearMon->bZipCode	= (BYTE)pMonster->GetTemp(NPC_TEMP_SLOT);
		}

		__lstrcpyn( pAppearMon->szMonsterName
			, (LPSTR)pMonster->GetBaseMonsterInfo()->szMonsterName_Kor
			, sizeof(pMonster->GetBaseMonsterInfo()->szMonsterName_Kor) );

		pAppearMon->bScale = pMonster->GetBaseMonsterInfo()->bScale;
	}

	pAppearMon->dwMonsterIndex	= pMonster->GetID();
	pAppearMon->v2CurPos		= *pMonster->GetCurPosition();
	pAppearMon->dwHP			= pMonster->GetHP();
	pAppearMon->dwMaxHP			= pMonster->GetMaxHP();
	pAppearMon->fSpeed			= pMonster->GetMoveSpeed();
	pAppearMon->wMonNo			= pMonster->GetBaseMonsterInfo()->wMODNo;
	pAppearMon->wNeu			= pMonster->GetBaseMonsterInfo()->wSneutral;
	pAppearMon->wDamage[0]		= pMonster->GetBaseMonsterInfo()->wSdamage1;
	pAppearMon->wDamage[1]		= pMonster->GetBaseMonsterInfo()->wSdamage2;
	pAppearMon->wAttack[0]		= pMonster->GetBaseMonsterInfo()->wSattack1;
	pAppearMon->wAttack[1]		= pMonster->GetBaseMonsterInfo()->wSattack2;
	pAppearMon->wDeath[0]		= pMonster->GetBaseMonsterInfo()->wSdeath1;
	pAppearMon->wDeath[1]		= pMonster->GetBaseMonsterInfo()->wSdeath2;
	pAppearMon->wMove[0]		= pMonster->GetBaseMonsterInfo()->wSmove1;
	pAppearMon->wMove[1]		= pMonster->GetBaseMonsterInfo()->wSmove2;
	
	if (pMonster->GetLord())
		pAppearMon->dwLordIndex		= pMonster->GetLord()->GetID();
	else
		pAppearMon->dwLordIndex = 0;
}


BYTE GetStatPointByLevel(GAME_OBJECT_TYPE eObjectType, DWORD dwLevel)
{
	switch (eObjectType)
	{
	case OBJECT_TYPE_PLAYER:
		return g_byStstus[dwLevel];
	case OBJECT_TYPE_MONSTER:
		return g_byGuardianStatus[dwLevel];
	}

	return 0;
}

DWORD GetExpTableOfLevel(GAME_OBJECT_TYPE eObjectType, DWORD dwLevel)
{
	switch (eObjectType)
	{
	case OBJECT_TYPE_PLAYER:
		return g_pdwExp[dwLevel];
	case OBJECT_TYPE_MONSTER:
		return g_pdwGuardianExp[dwLevel];
	}

	return 0;
}


void MonsterKillByUser( CUser* pUser, CMonster* pMonster )
{	
	DSTC_EVENT_ALIVE_MONSTER_COUNT packet;
	
	if ( pMonster->IsElemental() )
	{
		goto lb_dead;
	}
	else if(pMonster->GetUnitStatus() == UNIT_STATUS_DEAD)
	{
		__asm int 3
	}
		
	if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN )
	{
		if( pMonster->GetTemp(NPC_TEMP_DUNGEONGUARDIAN))	// 던전 가디언 사망, 던전 소유자 변경
		{
			pUser->GetCurDungeon()->SetDungeonOwner(pUser->GetID(), pUser->m_dwGuildId, pUser->m_dwPartyId
				, pUser->m_szId, pUser->GetName() );
		}
		else
		{
			if( pMonster->GetLord() )	// PVP?, 펫 가디언 사망
			{
				__asm nop;
			}
		}

		goto lb_dead;
	}

	// 남은 몬스터 카운트 	
	if(pMonster->IsNormalMonster())
	{
		--(pMonster->GetCurDungeonLayer()->m_wCurMonster);
	}
	
	if( pMonster->GetRace() == OBJECT_TYPE_MONSTER )
	{
		WORD wCount = WORD((rand()%(pMonster->GetBaseMonsterInfo()->wItemCount.wMax+1))
						+ pMonster->GetBaseMonsterInfo()->wItemCount.wMin);
		
		if( wCount > pMonster->GetBaseMonsterInfo()->wItemCount.wMax )
			wCount = pMonster->GetBaseMonsterInfo()->wItemCount.wMax;
		
		for( int i=0; i<wCount; i++ )
		{
			CreateItemByMonster( pMonster, pUser );
		}

		if (pMonster->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() == DUNGEON_TYPE_CONQUER)
		{
			pMonster->GetCurDungeon()->GetDungeonDataEx()->SetAccExp(pMonster->GetCurDungeon()->GetDungeonDataEx()->m_dwAccExp+3);
		}

		// 몬스터 죽이면 hp, sp가 회복된다.
		DWORD dwHPRecovery = pUser->GetItemAttr(ITEM_ATTR_MOP_KILL_TO_HP);

		DSTC_USER_STATUS UserStatus;
		UserStatus.bStatusMany	= 0;

		if (dwHPRecovery)
		{
			pUser->SetHP((WORD)(pUser->GetHP()+dwHPRecovery));
								
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetHP();		
		}

		DWORD dwMPRecovery = pUser->GetItemAttr(ITEM_ATTR_MOP_KILL_TO_SP);

		if (dwMPRecovery)
		{
			pUser->SetSP((WORD)(pUser->GetSP()+dwMPRecovery));
			
			UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
			UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pUser->GetSP();
		}
		
		if (UserStatus.bStatusMany)
		{
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);
		}

		// 몬스터를 잡으면 범죄시간 줄여 준다.
		pUser->SetCriminalPropensity(CRIMINAL_MONSTER_KILL, pMonster->GetLevel());
	}

	pUser->SetKillMonCount(pUser->GetKillMonCount() + 1);		

lb_dead:

	Event_MonsterKillByUser(pUser, pMonster);

	if( pUser->GetCurDungeon()->GetDungeonDataEx()->IsConquer() &&
		pUser->GetCurDungeon()->GetDungeonDataEx()->m_bSiege)
	{
		CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
		if(pLayer)
		{			
			if(pUser->GetCurDungeon()->GetDungeonDataEx()->IsConquer())
			{
				DSTC_EVENT_DUNGEON_SURVIVAL_DISPLAY packet;				
				packet.byDisplayMode			= SIEGEINFOWND_TYPE_SIEGE;
				packet.wDungeonID				= pUser->GetCurDungeonID(); 
				packet.wCurrentMonterLayerCount	= pLayer->m_wTotalMonster;
				packet.wKillMonsterLayerCount	= pLayer->m_wCurMonster;
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&packet,packet.GetPacketSize()
					, FLAG_SEND_NOT_ENCRYPTION );
			}
		}
	}		

	// 몬스터 삭제하기.
	pMonster->SetStatus(UNIT_STATUS_DEAD);
}


void MonsterKillByMonster( CMonster* pAttackMon, CMonster* pDefenseMon )
{	
	CUser*		pUser		= NULL;	
	CMonster*	pMonster	= NULL;	

	switch( pDefenseMon->GetRace() )
	{
	case OBJECT_TYPE_MONSTER:	// 일반 몬스터가 죽었다.		
		{
			// 남은 몬스터 카운트 
			if (pAttackMon->GetCurDungeonLayer()->m_wCurMonster > 0)
			{
				if (pDefenseMon->IsNormalMonster())
					--pAttackMon->GetCurDungeonLayer()->m_wCurMonster;
			}
			
			// 가디언에 의해서 죽었다면.
			if( pAttackMon->GetLord() && ( !pAttackMon->GetTemp(NPC_TEMP_DUNGEONGUARDIAN)) )	
			{
				// 일단 부모에게 모슨 경험치를 줄것이다.
				pUser		= (CUser*)pAttackMon->GetLord();
				pMonster	= pAttackMon;

				if(pUser)
				{
					pUser->SetKillMonCount(pUser->GetKillMonCount() + 1);
				}
				
				WORD wCount = WORD( (rand()%(pDefenseMon->GetBaseMonsterInfo()->wItemCount.wMax+1)) 
								+ pDefenseMon->GetBaseMonsterInfo()->wItemCount.wMin);
				
				if( wCount > pDefenseMon->GetBaseMonsterInfo()->wItemCount.wMax ) 
				{
					wCount = pDefenseMon->GetBaseMonsterInfo()->wItemCount.wMax;
				}
				
				for( int i=0; i<wCount; i++ )
				{
					CreateItemByMonster( pDefenseMon, pUser );
				}
			}
						
			// 몬스터를 잡으면 범죄시간 줄여 준다.
			if (pAttackMon->GetLord())
			{
				((CUser*)pAttackMon->GetLord())->SetCriminalPropensity(CRIMINAL_MONSTER_KILL
					, pDefenseMon->GetLevel());
			}
		}	
		break;

	case OBJECT_TYPE_GUARDIAN:	// 가디언이 죽었다.		
		{			
			if(pDefenseMon->GetLord())	// 펫 가디언 사망
			{
				CUser *pLord = (CUser *)pDefenseMon->GetLord();
				if(pLord)
				{
					CItem* pGuardianItem = &pLord->m_pInv_Guardian[pDefenseMon->GetTemp(NPC_TEMP_SLOT)];
					pDefenseMon->SubExp( pDefenseMon->GetExpDying() );

#ifndef JAPAN_LOCALIZING
					// 몬스터이고 주인이 없는 녀석(엘리, 가디언 제외) 한테 죽었다면
					if (pAttackMon->GetLord() == NULL
					 && pAttackMon->GetObjectType() == OBJECT_TYPE_MONSTER)
					{
						// 가디언의 수명 페널티를 적용
						SendExchangeGuardianLife(pLord, -pGuardianItem->GetBaseItem()->BaseItem_Guardian.wDyingPenalty);
					}
#endif

				}
			}
			else	// 던전 가디언 사망, 던전의 주인이 변경
			{
				if (pAttackMon->GetLord())
				{
					pUser = (CUser*)pAttackMon->GetLord();
					pUser->GetCurDungeon()->SetDungeonOwner(pUser->GetID(), pUser->m_dwGuildId, pUser->m_dwPartyId
						, pUser->m_szId, pUser->GetName() );
				}
			}			
		}
		break;
	}
	
	Event_MonsterKillByMonster(pAttackMon, pDefenseMon);

	pDefenseMon->SetStatus( UNIT_STATUS_DEAD);
}


void SendToWorldServerForGuildPoint(DWORD dwOffenseUserID, DWORD dwDefenseUserID, BYTE byGuildPoint
									, DWORD dwOffenseGuildID, DWORD dwDefenseGuildID)
{	
	DSTWS_GUILDWAR_POINT pServerPacket;
	pServerPacket.byPoint				= byGuildPoint;
	pServerPacket.dwUserIndex			= dwOffenseUserID;
	pServerPacket.dwGuildId				= dwOffenseGuildID;
	pServerPacket.dwGuildIdDst			= dwDefenseGuildID;
	pServerPacket.wDungeonID			= 0;
	pServerPacket.byLayerIndex			= 0;
	pServerPacket.dwUserIndexAttack		= dwOffenseUserID;
	pServerPacket.dwUserIndexDefence	= dwDefenseUserID;

	CUser* pTargetUser = g_pUserHash->GetData(dwDefenseUserID);

	if(pTargetUser && pTargetUser->GetCurDungeon())
	{
		pServerPacket.wDungeonID	= pTargetUser->GetCurDungeonID();
		pServerPacket.byLayerIndex	= pTargetUser->GetCurLayerIndex();
	}
	
	g_pNet->SendToServer(WSINDEX, (char*)&pServerPacket, pServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
}


// 전적 카운트 
void MatchRecordCount(CUser * pUser)
{
	if (pUser)
		pUser->m_sMatchDescInfo.m_dwMatchRecords += 1;
}


// 승리 카운트 
void MatchWinCount(CUser * pUser)
{
	if (pUser)
		pUser->m_sMatchDescInfo.m_dwMatchWin += 1;
}


// 패배 카운트 
void MatchLoseCount(CUser * pUser)
{
	if (pUser)
		pUser->m_sMatchDescInfo.m_dwMatchLose += 1;
}


void SendPortalFailMessage(CUser* pUser, WORD wToDungeonID, BYTE bErrorCode, WORD wPortalItemID)
{
	CItem				sItem;
	DSTC_ITEM_PICKUP	pPacket;
	
	if( IsUltra() )
	{
		
	}
	else
	{
		if(wPortalItemID!=0)
		{
			CreateItem(&sItem, wPortalItemID, 1);
			Insert_SmallInvItem(pUser, &sItem, 0, sItem.GetQuantity(), TRUE);	
			pPacket.bInv		= 14;
			pPacket.bZipCode	= 0;
			memcpy(&pPacket.Item, &sItem, sizeof(CItem));
			NetSendToUser(pUser->m_dwConnectionIndex, (char*)&pPacket, pPacket.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION);		
		}
	}
	
	DSTC_PORTAL_MOVE_OUTSIDE_FAIL FailPacket;
	FailPacket.bErrorCode = bErrorCode;
	FailPacket.wToDungeonID = wToDungeonID;	
	NetSendToUser(pUser->m_dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);
}


//선물상자를 열었을때 어떤 아이템이 튀어나올지를 정함 
WORD UnpackPresentBox(CItem* pItem, WORD* pCount)
{
//	if(pItem->GetID() <= BASE_ITEMPRESENT_INDEX || pItem->GetID() > BASE_ITEMPRESENT_INDEX+MAX_ITEMPRESENT)
	if(pItem->GetID() != __ITEM_HEROITEM_INDEX__)
	{
		return 0;	//선물상자 Item이 아님 
	}

	int iRate = (rand()%(SHRT_MAX+1));
	DWORD dwBotaryID = 0;
	WORD wPresentItemID = 0;
	DWORD dwBasePrItem = 1 ;//pItem->GetID() - (__ITEM_HEROITEM_INDEX__ -1);

	//BasePresentBox 테이블로 부터 해당하는 보따리 번호를 추출해낸다. 
	for(int i  = 0; i < MAX_ITEMPRESENT_RATE ; ++i)
	{
		if(iRate <= g_pBasePresentBox[dwBasePrItem].ItemRate[i].wBotaryRate)
		{
			dwBotaryID = g_pBasePresentBox[ dwBasePrItem ].ItemRate[i].wBotaryID;
			*pCount = g_pBasePresentBox[ dwBasePrItem ].ItemRate[i].wBotaryQuantity;
			break;
		}
	}


	Log(LOG_IMPORTANT, "Pre  dwBotaryID : %d , pCount : %d", dwBotaryID, *pCount );

/*	if( dwBotaryID == 0 )
	{
		int ntest = 0;
	}
*/
	//뽑은 보따리안에 있는 아이템 번호를 뽑아내자. 
	if (dwBotaryID > BASE_ITEMBOTARY_INDEX  && dwBotaryID < (BASE_ITEMBOTARY_INDEX+200))
	{
		iRate = rand();
		for(int j  = 0; j < 20; ++j)
		{
			if (iRate <= g_pBaseBotary[ dwBotaryID - BASE_ITEMBOTARY_INDEX].ItemRate[j].ItemRate)
			{
				wPresentItemID =  g_pBaseBotary[ dwBotaryID - BASE_ITEMBOTARY_INDEX].ItemRate[j].ItemID;
				break;
			}			
		} // for(int j  = 0; j < 20; ++j)
		Log(LOG_IMPORTANT, "Pre  12 %d ",wPresentItemID  );
	}
	else if( pItem->GetID() == __ITEM_HEROITEM_INDEX__ )
	{
		wPresentItemID	= (WORD)dwBotaryID;
		Log(LOG_IMPORTANT, "Pre  13 %d ",wPresentItemID  );
	}
	else
	{
		//잘못된 보따리 아이템 번호가 나옴 
		Log(LOG_IMPORTANT, "Fail to extract BotaryID! in UnpackPresentBox() Function");
		return 0;		
	}

	return wPresentItemID;	
}

//유저 인벤토리의 빈자리를 찾는다. 
int	GetUserEmptyInventoryPos(CUser* pUser, BOOL bSmallInv)
{
#ifndef JAPAN_LOCALIZING
	// 일본이 아닐때!!!
	if ( bSmallInv )
	{
		return GetSmallInvEmptyPos( pUser );
	}
	else
	{
		return GetLargeInvEmptyPos( pUser );
	}
#else
	// 일본은 라지던 스몰이던 어디 한군데만 없으면 자리없는걸로 친다!!
	if ( bSmallInv )
	{
		if ( 0 < GetLargeInvEmptyPos( pUser ) )
		{
			return GetSmallInvEmptyPos( pUser );
		}
	}
	else
	{
		if ( 0 < GetSmallInvEmptyPos( pUser ) )
		{
			return GetLargeInvEmptyPos( pUser );
		}
	}

	return -1;
#endif
}

int GetSmallInvEmptyPos( CUser* pUser )
{
	for ( int i = 0; i < MAX_INV_SMALL_POOL; i++ )
	{
		if ( pUser->m_pInv_Small[i].GetID() == 0 ) 
			return i;
	}

	return -1;
}

int GetLargeInvEmptyPos( CUser* pUser )
{
	for( int i = 0; i < MAX_INV_LARGE_POOL; i++ )
	{
		if ( pUser->m_pInv_Large[i].GetID() == 0 ) 
			return i;
	}

	return -1;
}


void CPlayerShopLogHash::DeleteData(PLAYERSHOP_LOG* pData)
{
	PLAYERSHOP_LOG* pPlayerShopLog = (PLAYERSHOP_LOG*)pData;

	if(pPlayerShopLog)
	{
		memset(pPlayerShopLog, 0, sizeof(pPlayerShopLog));
		LALFree(g_pPlayerShopLogPool, pPlayerShopLog);
		pPlayerShopLog = NULL;
	}
}

void CItemTradeHash::DeleteData(ITEM_TRADE* pData)
{
	ITEM_TRADE* pItemTrade = (ITEM_TRADE*)pData;

	if(pItemTrade)
	{
		LALFree(g_pItemTradePool, pItemTrade);		
		pItemTrade = NULL;
	}
}

void CItemStoreHash::DeleteData(ITEM_STORE* pData)
{
	ITEM_STORE* pItemStore = (ITEM_STORE*)pData;

	if(pItemStore)
	{
		LALFree(g_pItemStorePool, pItemStore);		
		pItemStore = NULL;
	}
}

void CItemShopHash::DeleteData(ITEM_SHOP* pData)
{
	ITEM_SHOP* pItemShop = (ITEM_SHOP*)pData;

	if(pItemShop)
	{
		LALFree(g_pItemShopPool, pItemShop);		
		pItemShop = NULL;
	}
}

void CShopTileHash::DeleteData(SHOP_TILE* pData)
{
	SHOP_TILE* pShopTile = (SHOP_TILE*)pData;

	if(pShopTile)
	{
		LALFree(g_pShopTilePool, pShopTile);		
		pShopTile = NULL;
	}
}

void CUserHash::DeleteData(CUser* pUser)
{
	
}


void CGuildListHash::DeleteData(GUILD_NODE* pData)
{
	GUILD_NODE* pGuildNode = (GUILD_NODE*)pData;

	if(pGuildNode)
	{		
		LALFree(g_pGuildListPool, pGuildNode);
		pGuildNode = NULL;
	}
}

void CVirtualLinkItemNativeHash::DeleteData(CItem* pData)
{
	CItem* pVirtualLinkItemNative = (CItem*)pData;

	if(pVirtualLinkItemNative)
	{
		LALFree(g_pVirtualLinkItemNativePool, pVirtualLinkItemNative);		
		pVirtualLinkItemNative = NULL;
	}
}





