#include "stdafx.h"
#include "Monster.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "Map.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonLayer.h"
#include "Section.h"
#include "MonsterManager.h"
#include "PathFinding\\SearchModule_Astar.h"
#include "OwnServer.h"
#include "MonsterTable.h"
#include "ClassMemoryPool.h"
#include "DBProc.h"

static float pfClass[6] = { 0.f, 4.f, 3.f, 2.f, 1.5f, 1.f };

CMonster::CMonster()
{
}

CMonster::~CMonster()
{
	RemoveResource();
}


void CMonster::SetStatusStop()
{
	if( GetUnitStatus() != UNIT_STATUS_WALKING ) return;
	
	m_v2DestPos	= *GetCurPosition();
	SendStopPacket(SEND_MODE_BROADCAST_SECTION);
	
	// 다음 ai를 지연 시키기 위해서 딜레이 타임을 설정한다.
	//SetStatus(NPC_STATUS_DELAYTIME);
	m_dwTemp[NPC_TEMP_LASTSTATUSTICK]	= g_dwTickCount;
}



BOOL CMonster::SetPathFindMove(BYTE bMonsterStatus)
{
	if( m_PathFinder.dwCurveNum == 0 ) 
	{
		m_PathFinder.dwCurCurve = 0;
		SetStatus( bMonsterStatus, FALSE);
		return FALSE;
	}

	if( m_PathFinder.dwCurveNum == m_PathFinder.dwCurCurve )
	{
		m_PathFinder.dwCurveNum = 0;
		m_PathFinder.dwCurCurve = 0;
		SetStatus( bMonsterStatus, FALSE);
		return FALSE;
	}
	
	m_v2DestPos.x = TILE_X( m_PathFinder.pLocation[m_PathFinder.dwCurCurve].dwBlock_X );
	m_v2DestPos.y = TILE_Z( m_PathFinder.pLocation[m_PathFinder.dwCurCurve].dwBlock_Y );
	m_PathFinder.dwCurCurve++;
	
	// 몬스터가 가는 방향을 구한다.
	VECTOR2 v2Direct = {0.f, 0.f};
	VECTOR3_SUB_VECTOR3( &v2Direct, &m_v2DestPos, GetCurPosition());
	SetCurDirection(&v2Direct);
	Normalize( &v2Direct, &v2Direct );
	SetCurDirection(&v2Direct);
	
	if(GetUnitStatus() == UNIT_STATUS_RUNNING)
		SendRunningPacket();
	else
		SendMovePacket();	

	return TRUE;
}

BOOL CMonster::SetMonsterAI( DWORD dwAI )
{	
	switch( dwAI )
	{
	case 0:
		AI		= AI0;
		break;
	case 1:
		AI		= AI1;
		break;

	case 2:
		//AI		= ( void (CMonster::*)() )&CMonster::AI2;
		AI		= AI_New;
		break;

	case 3:
		AI		= AI3;
		break;

	case 4:
		AI		= AI4;
		break;
	
	case 5:
		AI		= AI5;
		break;

	case 6:
		AI		= AI6;
		break;
	
	case 8:
		AI		= AI8;
		break;

	case 9:
		AI		= AI9;
		break;

	default:
		AI		= NULL;
		return FALSE;
	}
	
	m_dwAI = dwAI;
	
	return TRUE;
}

void CMonster::SetDamageOver(const CUnit* pUnit, DWORD dwDamage)
{
	if( pUnit == GetLord() ) return;
	
	if( g_dwTickCount - m_dwTemp[ NPC_TEMP_DAMAGETICK ] >= 5000 )
	{
		m_dwTemp[ NPC_TEMP_DAMAGETICK ] = 0;
	}
	
	AI_DEFENSE_PARAM aiParam;
	aiParam.bObjectType = BYTE(pUnit->GetObjectType());
	aiParam.pObject = pUnit;
	aiParam.dwTemp[AI_DEFENSE_TEMP_DAMAGE] = dwDamage;

	(this->*AI_DefenseType[GetAIType(AI_KIND_DEFENSETYPE)])(&aiParam);
	
	m_dwTemp[ NPC_TEMP_DAMAGETICK ] = g_dwTickCount;
}

void CMonster::SetStatus( BYTE bStatus, BOOL bClearTarget, BOOL bCompulsion )
{
	if( !bCompulsion && GetUnitStatus() == UNIT_STATUS_DEAD ) return;		// 죽어있는 상태라면 절대 상태를 바꾸지 않는다.

	// 일단 바뀌기전의 상태를 마무리하고 상태를 바꾸어 준다.
	if( bStatus != GetUnitStatus() )
	{
		switch( GetUnitStatus() )
		{
		case UNIT_STATUS_WALKING:	// 내가 만약 걷고 있었다면 정지 해야겠지?
			if (bClearTarget)	ClearTargetAI();
			m_v2DestPos			= *GetCurPosition();
			SendStopPacket(SEND_MODE_BROADCAST_SECTION);
			break;	
		}
	}
	
	SetUnitStatus((UNIT_STATUS)bStatus);

	// 이제 바뀔상태로 바꾼후 적절한처리를 해준다.
	switch( GetUnitStatus() )
	{
	case UNIT_STATUS_NORMAL:
		if (bClearTarget) ClearTargetAI();
		break;

	case UNIT_STATUS_DEAD:
		{
			// 공성할땐 태어나지 마
			if ((!IsCurDungeonSiege() && (IsNormalMonster() || IsNPC())) 
				|| (IsCurDungeonSiege() 
					&& (GetCurLayerIndex()+1) == GetCurDungeon()->GetTotalLayer() 
					&& (IsNormalMonster() 
						|| IsGuardian() == GUARDIAN_TYPE_DUNGEON)))

				m_dwTemp[NPC_TEMP_KILLTICK]	= g_dwTickCount;
			else
				m_dwTemp[NPC_TEMP_KILLTICK]	= 0xffffffff;
			
			GetCurDungeonLayer()->m_pDeadMonsterList->AddHead(this);
		}
		break;
	}
}

// 시간나면 고쳐야해.
void CMonster::SetTargetAI(const CUnit* pUnit)
{
	/*
	CUnit* pLord = GetLord();
		
	if(pLord)		// 소환물이냐??
	{
		if(!IsGuildWar(pLord->GetCurDungeon()
			, ((CUser*)pLord)->m_dwGuildId
			, ((CUser*)pUnit)->m_dwGuildId
			, ((CUser*)pLord)->m_dwPartyId
			, ((CUser*)pUnit)->m_dwPartyId
			, ((CUser*)pUnit)->m_enGodMode)	
			)
		{
			if(IsCurDungeonSiege())
			{					
				if(GetCurLayerIndex()==0)
					return;		
				if (pLord->GetID() == pUnit->GetID() )
					return;

			}
		}		
		else
		{
			if(!IsCurDungeonVillage())
			{
				m_pUnitForAI = const_cast<CUnit*>(pUnit);
				m_dwTemp[ NPC_TEMP_AIKIND ] = pUnit->GetObjectType();
				m_v2TagetAIPos = *pUnit->GetCurPosition();
			}
		}
	}
	else
	{
		if(IsCurDungeonSiege())
		{
			
		}		
	
	}	

*/
	if (IsCurDungeonSiege())
	{
		// 공성중에는 일반몹들은 방어자 편이 되므로. 인식하지 마라.
		// 추후 수정되야 한다.
		if (!GetLord() && pUnit->GetAttackMode() == ATTACK_MODE_DEFENCE )
			return;
	}
	// sung-han 2005-01-13 서머너 스킬 컨퓨즈 비정상 작동하는 부분 수정
	//-------------------------------------------------------------------	
	//if (IsAlliance(pUnit))
	if (IsAlliance(pUnit) && m_dwTemp[NPC_TEMP_TARGET] != 1)
		return;
	//-------------------------------------------------------------------

	m_pUnitForAI = const_cast<CUnit*>(pUnit);
	m_dwTemp[ NPC_TEMP_AIKIND ] = pUnit->GetObjectType();
	m_v2TargetAIPos = *pUnit->GetCurPosition();
	m_v2TargetAIDir = *pUnit->GetCurDirection();
	
	SetStatus( UNIT_STATUS_NORMAL );	// 찾자마자 공격 위치로! : 최덕석 2005.1.22
}



void CMonster::ClearTargetAI()
{
	m_pUnitForAI	= NULL;
	m_dwTemp[ NPC_TEMP_AIKIND ] = 0;
	m_v2TargetAIPos.x = 0;
	m_v2TargetAIPos.y = 0;
	m_v2TargetAIDir.x = 0;
	m_v2TargetAIDir.y = 0;
}

void CMonster::SetName(char* pszName)
{
	__lstrcpyn( m_szGuardianName, pszName, sizeof(m_szGuardianName) );
}

const BASEMONSTER* CMonster::GetBaseMonsterInfo() const
{
	return m_pBaseMonster;
}

const char* CMonster::GetName() const
{
	return m_szGuardianName;
}

void CMonster::SetGuardianDBID(DWORD dwGuardianDBID)
{
	m_dwSerial_NO = dwGuardianDBID;
}

DWORD CMonster::GetGuardianDBID() const
{
	return m_dwSerial_NO;
}

void CMonster::ScheduleMoveDestPos(const VECTOR2* pDestPos )
{
	SetStatus( UNIT_STATUS_NORMAL, TRUE);
	
	if( PathFind( pDestPos ) )
	{
		SetStatus( UNIT_STATUS_WALKING);
	}
}

short CMonster::GetAllResist() const
{
	return m_wAll_resist;
}

void CMonster::AddExp(int iExp, BOOL bAlphaExp)
{
	if( iExp <= 0 )			return;
	if( GetLevel() == 200 ) return;
	
	SetExp(GetExp()+iExp);
	
	if (GetRace() == OBJECT_TYPE_GUARDIAN)
	{
		DSTC_GUARDIAN_STATUS GuardianStatus;
		GuardianStatus.bStatusMany			= 1;
		GuardianStatus.dwGuardianIndex		= GetID();
		GuardianStatus.pStatus[0].dwCode	= MONSTER_EXP;
		GuardianStatus.pStatus[0].dwMin		= GetExp();
		
		// 주인이 있는 가디언이면
		if (GetLord())
		{
			CItem cGuardianItem;
			memcpy(&cGuardianItem, ((CUser*)GetLord())->GetItem(ITEM_NATIVE_INV_GUARDIAN, 0), sizeof(CItem));
			cGuardianItem.m_Item_Guardian.dwCompleteTime = GetExp();
			((CUser*)GetLord())->SetItem(ITEM_NATIVE_INV_GUARDIAN, 0, &cGuardianItem);

			// 경험치가 레벨에 도달 할 때까지 레벨업
			while( GetExp() >= GetExpTableOfLevel(OBJECT_TYPE_MONSTER, GetLevel() + 1) )
			{
				LevelUP();

				GuardianStatus.bStatusMany			= 3;
				GuardianStatus.pStatus[1].dwCode	= MONSTER_MAXHP;
				GuardianStatus.pStatus[1].dwMin		= GetMaxHP();

				GuardianStatus.pStatus[2].dwCode	= MONSTER_HP;
				GuardianStatus.pStatus[2].dwMin		= GetMaxHP();		
			}

			// 가디언의 최신 상태를 클라이언트에게 전송
			NetSendToUser( ((CUser*)GetLord())->m_dwConnectionIndex, (char*)&GuardianStatus
				, GuardianStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
		}
	}
}


void CMonster::SubExp( int iExp )
{
	if( iExp <= 0 ) return;

	DWORD dwLevelExp=0;
	
	if( GetLevel() != 1 )
	{
		dwLevelExp = GetExpTableOfLevel(OBJECT_TYPE_MONSTER,  GetLevel() - 1 );
	}

	if( dwLevelExp > ( GetExp() - iExp ) )
	{
		SetExp(dwLevelExp);
	}

	DSTC_GUARDIAN_STATUS	GuardianStatus;
	GuardianStatus.bStatusMany			= 1;
	GuardianStatus.dwGuardianIndex		= GetID();
	GuardianStatus.pStatus[0].dwCode	= MONSTER_EXP;
	GuardianStatus.pStatus[0].dwMin		= GetExp();

	NetSendToUser( ((CUser*)GetLord())->m_dwConnectionIndex, (char*)&GuardianStatus, GuardianStatus.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION );
}


void* CMonster::FindObject(BYTE bSearchType)
{
	if( m_dwTemp[ NPC_TEMP_PEACE ] != 0 )		return NULL;
	if( m_dwTemp[ NPC_TEMP_FINDUSER ] != 0 )	return NULL;
	
	CMonster*	pMonster=0;
	CUser*		pUser=0;
	
	m_dwTemp[NPC_TEMP_AIKIND] = 0;

	if( m_dwTemp[NPC_TEMP_TARGET] == 0 || m_dwTemp[NPC_TEMP_TARGET] == 3 )		
	{
		// 소환물 먼저 검색, 그다음 User를 검색한다.
		pMonster = (CMonster*)(this->*AI_FindObject[bSearchType])(OBJECT_TYPE_SUMMON);

		if(pMonster)
		{
			SetTargetAI( pMonster );
		}
		else
		{
			pUser = (CUser*)(this->*AI_FindObject[bSearchType])(OBJECT_TYPE_PLAYER);

			if (pUser)
			{
				SetTargetAI( pUser );
			}
		}		
	}
	
	//m_dwTemp[NPC_TEMP_TARGET] == 1 인경우는 Confuse 스킬 걸렸을경우이다. 
	if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )		// Monster를 검색한다.
	{		
		pMonster = (CMonster*)(this->*AI_FindObject[bSearchType])( OBJECT_TYPE_MONSTER );

		if (pMonster)
			SetTargetAI( pMonster );		
	}

	if( m_dwTemp[NPC_TEMP_AIKIND] == OBJECT_TYPE_PLAYER	|| m_dwTemp[NPC_TEMP_AIKIND] == OBJECT_TYPE_MONSTER) 
	{
		return m_pUnitForAI;
	}
	
	return NULL;
}

void* CMonster::AI_FindObjectNear(BYTE bObjectType)
{
	void*	pFindObject = NULL;
	double	fMin	= 1000000.f;			// 이몬스터에서 가장 가까운 유저를 찾기 위함.
	double	fLength=0.f;
	
	POSITION_	UserPos=0, MonsterPos=0;
	CSection*	pLinkSection=0;
	CUser*		pUser=0;
	CMonster*	pMonster=0;
	int i=0;
	
	if (bObjectType == OBJECT_TYPE_PLAYER)	goto lbl_user;
	else if (bObjectType == OBJECT_TYPE_MONSTER)	goto lbl_monster;
	else if (bObjectType == OBJECT_TYPE_SUMMON)		goto lbl_summon;	//소환물 공격 
	else	return	NULL;
	
lbl_user:
	UserPos		= GetCurSection()->m_pPcList->GetHeadPosition();
	while( UserPos )
	{
		pUser	= (CUser*)GetCurSection()->m_pPcList->GetNext( UserPos );
		fLength = CalcDistance( GetCurPosition(), pUser->GetCurPosition() );
		
		if( fLength < fMin 
			&& pUser->GetUnitStatus() != UNIT_STATUS_DEAD 
			&& (!pUser->GetEffectDesc(__SKILL_HIDING__ )
			//(정우창 2005.03.07) - 은신상태에서 이동을 할 경우에도 은신상태로 수정(클라이언트) 후 이동할 경우 Monster가 은신상태가 풀린상태로 판단하는 부분 수정 
			/*	|| (pUser->GetEffectDesc(__SKILL_HIDING__)					
				&& (pUser->GetUnitStatus() == UNIT_STATUS_WALKING 
				|| pUser->GetUnitStatus() == UNIT_STATUS_RUNNING)) */ )  
			&& !pUser->m_enGodMode)
		{
			// 만약 자신의 주인이라면 타켓으로 삼지 않는다.
			if( GetLord() )
			{
				if( GetLord() != pUser )
				{
					fMin		= fLength;
					pFindObject = pUser;
				}
			}
			else if( IsCurDungeonSiege() )	
			{// 만약 공성중이라면.
				// 공격자만
				if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
				{
					fMin		= fLength;
					pFindObject = pUser;					
				}
			}
			else
			{
				fMin		= fLength;
				pFindObject = pUser;
			}
		}
	}
	
	for( i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;

		UserPos = pLinkSection->m_pPcList->GetHeadPosition();
		
		while( UserPos )
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext( UserPos );
			fLength = CalcDistance( GetCurPosition(), pUser->GetCurPosition() );
			
			if( fLength < fMin && pUser->GetUnitStatus() != UNIT_STATUS_DEAD 
				&& (!pUser->GetEffectDesc(__SKILL_HIDING__ )
				//(정우창 2005.03.07) - 은신상태에서 이동을 할 경우에도 은신상태로 수정(클라이언트) 후 이동할 경우 Monster가 은신상태가 풀린상태로 판단하는 부분 수정  
				/*	|| (pUser->GetEffectDesc(__SKILL_HIDING__ )
					&& (pUser->GetUnitStatus() == UNIT_STATUS_WALKING 
					|| pUser->GetUnitStatus() == UNIT_STATUS_RUNNING)) */ ) 
				&& !pUser->m_enGodMode)
			{
				// 만약 자신의 주인이라면 타켓으로 삼지 않는다.
				if( GetLord() )
				{
					if( GetLord() != pUser )
					{
						fMin		= fLength;
						pFindObject = pUser;
					}
				}
				else if( IsCurDungeonSiege() )	// 만약 공성중이라면.
				{
					// 던젼 주인이라면 타켓으로 잡지 않는다.
					if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
					{
						fMin		= fLength;
						pFindObject = pUser;						
					}
				}
				else
				{
					fMin		= fLength;
					pFindObject = pUser;
				}
			}
		}
	}
	
	// 마지막으로 시아안에 있는지를 검사한다.
	if( fMin > m_fSight )
	{
		return NULL;
	}

	return pFindObject;

lbl_monster:
	MonsterPos		= GetCurSection()->m_pMonsterList->GetHeadPosition();
	while( MonsterPos )
	{
		pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );
		fLength		= CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );

		if( fLength < fMin && pMonster->GetUnitStatus() != UNIT_STATUS_DEAD 
			&& this != pMonster )
		{
			if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )
			{
				fMin						= fLength;
				pFindObject = pMonster;
			}
			else if( m_dwTemp[NPC_TEMP_TARGET] == 0 )
			{
				if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN 
					&& !pMonster->m_dwTemp[ NPC_TEMP_DUNGEONGUARDIAN ] )
				{
					fMin						= fLength;
					pFindObject = pMonster;
				}
			}
		}
	}
	
	for( i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;
		
		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		
		while( MonsterPos )
		{
			pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );
			fLength		= CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );

			if( fLength < fMin && pMonster->GetUnitStatus()	!= UNIT_STATUS_DEAD 
				&& this != pMonster )
			{
				if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )
				{
					fMin		= fLength;
					pFindObject = pMonster;
				}
				else if( m_dwTemp[NPC_TEMP_TARGET] == 0 )
				{
					if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN 
						&& !pMonster->m_dwTemp[ NPC_TEMP_DUNGEONGUARDIAN ])
					{
						fMin		= fLength;
						pFindObject = pMonster;
					}
				}
			}
		}
	}

	// 마지막으로 시아안에 있는지를 검사한다.
	if( fMin > m_fSight )
	{
		return NULL;
	}


lbl_summon:
	MonsterPos		= GetCurSection()->m_pMonsterList->GetHeadPosition();
	while( MonsterPos )
	{
		pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );

		if(pMonster == this || !pMonster->GetLord())	continue;	//자기자신이거나 소환물이 아닐경우는 Skip

		fLength		= CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );

		if( fLength < fMin && pMonster->GetUnitStatus() != UNIT_STATUS_DEAD 
			&& this != pMonster )
		{
			if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )
			{
				fMin		= fLength;
				pFindObject = pMonster;
			}
			else if( m_dwTemp[NPC_TEMP_TARGET] == 0 )
			{
				if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN 
					&& !pMonster->m_dwTemp[ NPC_TEMP_DUNGEONGUARDIAN ] )
				{
					fMin		= fLength;
					pFindObject = pMonster;
				}
			}
		}
	}
	
	for( i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;
		
		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		
		while( MonsterPos )
		{
			pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );
			if(pMonster == this || !pMonster->GetLord())	continue;	//자기자신이거나 소환물이 아닐경우는 Skip

			fLength		= CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );

			if( fLength < fMin && pMonster->GetUnitStatus() != UNIT_STATUS_DEAD 
				&& this != pMonster )
			{
				if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )
				{
					fMin		= fLength;
					pFindObject = pMonster;
				}
				else if( m_dwTemp[NPC_TEMP_TARGET] == 0 )
				{
					if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN 
						&& !pMonster->m_dwTemp[ NPC_TEMP_DUNGEONGUARDIAN ])
					{
						fMin		= fLength;
						pFindObject = pMonster;
					}
				}
			}
		}
	}

	// 마지막으로 시아안에 있는지를 검사한다.
	if( fMin > m_fSight )
	{
		return NULL;
	}

	return pFindObject;
}


BOOL CMonster::ChangeTargetObject(const CUnit* pAttackUser, int nAttackDemage)
{
	if(!m_pCurSection)	return FALSE;
	if(m_dwTemp[NPC_TEMP_FINDUSER])	return FALSE;

	//폭주모드 발동 HP보다 작아지면 폭주모드를 On한다. 
	float fOnHP = (float)(GetMaxHP()*GetAI_MinHPRateForCongestionMode());
	if (GetHP() <= fOnHP)
	{
		CheckCongestionMode( TRUE );
	}
	
	//타겟변경없음 한놈만 죽어라 공격한다. 
	if (m_pUnitForAI 
		&& GetAI_TargetChangeType() == AI_CHANGE_TARGET_TYPE_NONE)
		return FALSE;

	if(GetLord())
		return FALSE;

	BOOL bRet = FALSE;
	//nAttackDemage가 음수값이 들어오는 경우는 데미지양 또는 attack 성공여부에 상관없이
	//무조건 공격한 놈으로 타겟을 바꾸어준다. 
	// sung-han 2005-01-13 서머너 스킬 컨퓨즈 비정상 작동하는 부분 수정
	//-------------------------------------------------------------------
	//if(nAttackDemage <= 0)
	if( nAttackDemage <= 0 && m_dwTemp[NPC_TEMP_TARGET] != 1 )
	{
		SetTargetAI( pAttackUser );
		bRet = TRUE;
	}
	//-------------------------------------------------------------------
	//nAttackDemage가 양수값이 들어오는 경우는 몬스터 HP의 5% 이상의 데미지를 입힌놈으로 타겟을 바꾼다.
	else if(nAttackDemage > (GetMaxHP() * 0.05) )	
	{
		SetTargetAI( pAttackUser );
		bRet = TRUE;
	}

	//Family System 적용 부분 
	//타겟이 변경된경우 해당 섹션안에 같은 종류의 몬스터가 AI_CHANGE_TARGET_TYPE_SAME_KIND
	//이면 이 몬스터가 변경한 타겟으로 바꿔준다. 
	if(bRet)
	{
		POSITION_ pos = GetCurSection()->m_pMonsterList->GetHeadPosition();
		CMonster *pMonster=0;
		while(pos)
		{
			pMonster = (CMonster*)GetCurSection()->m_pMonsterList->GetNext( pos );

			if ( !pMonster )			continue;	// added by minjin.
			if ( !m_pBaseMonster )		continue;	// added by minjin.

			if(pMonster == this 
				|| pMonster->IsGuardian() 
				||IsGuardian() )	
				continue;		//자기 자신이거나 가디언은 스킵 
			if(pMonster->m_pBaseMonster->dwID == m_pBaseMonster->dwID &&		//같은종류의 몬스터이고 AI_CHANGE_TARGET_TYPE_SAME_KIND 인경우
				pMonster->GetAI_TargetChangeType() == AI_CHANGE_TARGET_TYPE_SAME_KIND &&
				pMonster->GetTemp(NPC_TEMP_TARGET) == 0)	// 컨퓨즈 걸린 놈은 종족 인식도 안됨! : 최덕석 2005.1.22
			{
				pMonster->SetTargetAI( pAttackUser );
				OutputDebugString("Family System On!\n");
			}
		}

		//링크 섹션까지 뒤진다. 
		CSection *pLinkSection=0;
		for( int i=0; GetCurSection()->m_wLink[i] != 0; i++ )
		{
			pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
			if( !pLinkSection ) continue;

			pos = pLinkSection->m_pMonsterList->GetHeadPosition();
			while(pos)
			{
				pMonster = (CMonster*)pLinkSection->m_pMonsterList->GetNext( pos );

				if ( !pMonster ) continue; // added by minjin.

				if(pMonster == this 
					|| pMonster->IsGuardian() 
					|| IsGuardian())	
					continue;			//자기 자신이거나 가디언은 스킵 
				else if(!pMonster->m_pBaseMonster 
					|| !m_pBaseMonster) 
					continue;
				
				if(pMonster->m_pBaseMonster->dwID == m_pBaseMonster->dwID 
					&& pMonster->GetAI_TargetChangeType() == AI_CHANGE_TARGET_TYPE_SAME_KIND)
				{
					pMonster->SetTargetAI( pAttackUser );
					OutputDebugString("Family System On! (Link Section)\n");
				}
			}
		}
	}

	return bRet;
}

void* CMonster::AI_FindObjectLeastHP(BYTE bObjectType)
{
	void*	pFindObject = NULL;
	//double	fMin	= 1000000.f;			// 이몬스터에서 가장 가까운 유저를 찾기 위함.
	double	fLength = 0.f;
	DWORD	dwCurMinHP = 0xffffffff;
	DWORD	dwHP=0;
	
	POSITION_	UserPos=0, MonsterPos=0;
	CSection*	pLinkSection=0;
	CUser*		pUser=0;
	CMonster*	pMonster=0;
	int i=0;
	
	if (bObjectType == OBJECT_TYPE_PLAYER)	goto lbl_user;
	else if (bObjectType == OBJECT_TYPE_MONSTER)	goto lbl_monster;
	else if (bObjectType == OBJECT_TYPE_SUMMON)		goto lbl_summon;
	else	return	NULL;
	
lbl_user:
	UserPos		= GetCurSection()->m_pPcList->GetHeadPosition();
	while( UserPos )
	{
		pUser	= (CUser*)GetCurSection()->m_pPcList->GetNext( UserPos );
		fLength = CalcDistance( GetCurPosition(), pUser->GetCurPosition() );
		if( fLength > m_fSight )	continue;		//시야 밖이면 다른놈을 찾고..

		dwHP = pUser->GetHP();
		if( dwHP < dwCurMinHP && pUser->GetUnitStatus() != UNIT_STATUS_DEAD 
			&& (!pUser->GetEffectDesc(__SKILL_HIDING__ )
			//(정우창 2005.03.07) - 은신상태에서 이동을 할 경우에도 은신상태로 수정(클라이언트) 후 이동할 경우 Monster가 은신상태가 풀린상태로 판단하는 부분 수정  
			/*	|| (pUser->GetEffectDesc( __SKILL_HIDING__ )
				&& (pUser->GetUnitStatus() == UNIT_STATUS_WALKING 
				|| pUser->GetUnitStatus() == UNIT_STATUS_RUNNING)) */ ) 
			&& !pUser->m_enGodMode)
		{
			// 만약 자신의 주인이라면 타켓으로 삼지 않는다.
			if( GetLord() )
			{
				if( GetLord() != pUser )
				{
					dwCurMinHP	= dwHP;
					pFindObject = pUser;
				}
			}
			else if( IsCurDungeonSiege() )	
			{// 만약 공성중이라면.
				// 공격자만
				if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
				{
					dwCurMinHP	= dwHP;
					pFindObject = pUser;					
				}
			}
			else
			{
				dwCurMinHP	= dwHP;
				pFindObject = pUser;
			}
		}
	}
	
	for( i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;

		UserPos = pLinkSection->m_pPcList->GetHeadPosition();
		
		while( UserPos )
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext( UserPos );
			fLength = CalcDistance( GetCurPosition(), pUser->GetCurPosition());
			if( fLength > m_fSight )	continue;		//시야 밖이면 다른놈을 찾고..

			dwHP = pUser->GetHP();
			if( dwHP < dwCurMinHP && pUser->GetUnitStatus() != UNIT_STATUS_DEAD 
				&& (!pUser->GetEffectDesc(__SKILL_HIDING__ )
				//(정우창 2005.03.07) - 은신상태에서 이동을 할 경우에도 은신상태로 수정(클라이언트) 후 이동할 경우 Monster가 은신상태가 풀린상태로 판단하는 부분 수정  
				/*	|| (pUser->GetEffectDesc(__SKILL_HIDING__)
						&& (pUser->GetUnitStatus() == UNIT_STATUS_WALKING 
							|| pUser->GetUnitStatus() == UNIT_STATUS_RUNNING)) */ ) 
				&& !pUser->m_enGodMode)
			{
				// 만약 자신의 주인이라면 타켓으로 삼지 않는다.
				if( GetLord() )
				{
					if( GetLord() != pUser )
					{
						dwCurMinHP	= dwHP;
						pFindObject = pUser;
					}
				}
				else if( IsCurDungeonSiege() )	// 만약 공성중이라면.
				{
					// 던젼 주인이라면 타켓으로 잡지 않는다.
					if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
					{
						dwCurMinHP	= dwHP;
						pFindObject = pUser;						
					}
				}
				else
				{
					dwCurMinHP	= dwHP;
					pFindObject = pUser;
				}
			}
		}
	}
	
	return pFindObject;

lbl_monster:
	MonsterPos		= GetCurSection()->m_pMonsterList->GetHeadPosition();
	while( MonsterPos )
	{
		pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );
		fLength		= CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );
		if( fLength > m_fSight )	continue;		//시야 밖이면 다른놈을 찾고..

		dwHP = pMonster->GetExp();
		if( dwHP < dwCurMinHP && pMonster->GetUnitStatus() != UNIT_STATUS_DEAD 
			&& this != pMonster )
		{
			if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )
			{
				dwCurMinHP	= dwHP;
				pFindObject = pMonster;
			}
			else if( m_dwTemp[NPC_TEMP_TARGET] == 0 )
			{
				if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN 
					&& !pMonster->m_dwTemp[ NPC_TEMP_DUNGEONGUARDIAN ] )
				{
					dwCurMinHP	= dwHP;
					pFindObject = pMonster;
				}
			}
		}
	}
	
	for( i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;
		
		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		
		while( MonsterPos )
		{
			pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );
			fLength		= CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );

			if( fLength > m_fSight )	continue;		//시야 밖이면 다른놈을 찾고..

			dwHP = pMonster->GetExp();
			if( dwHP < dwCurMinHP && pMonster->GetUnitStatus() != UNIT_STATUS_DEAD 
				&& this != pMonster )
			{
				if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )
				{
					dwCurMinHP	= dwHP;
					pFindObject = pMonster;
				}
				else if( m_dwTemp[NPC_TEMP_TARGET] == 0 )
				{
					if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN
						&& !pMonster->m_dwTemp[ NPC_TEMP_DUNGEONGUARDIAN ])
					{
						dwCurMinHP	= dwHP;
						pFindObject = pMonster;
					}
				}
			}
		}
	}

	return pFindObject;

	
lbl_summon:
	MonsterPos		= GetCurSection()->m_pMonsterList->GetHeadPosition();
	while( MonsterPos )
	{
		pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );

		if(pMonster == this || !pMonster->GetLord())	continue;	//자기자신이거나 소환물이 아닐경우는 Skip

		fLength		= CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );
		if( fLength > m_fSight )	continue;		//시야 밖이면 다른놈을 찾고..

		dwHP = pMonster->GetExp();
		if( dwHP < dwCurMinHP && pMonster->GetUnitStatus() != UNIT_STATUS_DEAD 
			&& this != pMonster )
		{
			if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )
			{
				dwCurMinHP	= dwHP;
				pFindObject = pMonster;
			}
			else if( m_dwTemp[NPC_TEMP_TARGET] == 0 )
			{
				if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN 
					&& !pMonster->m_dwTemp[ NPC_TEMP_DUNGEONGUARDIAN ] )
				{
					dwCurMinHP	= dwHP;
					pFindObject = pMonster;
				}
			}
		}
	}
	
	for( i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;
		
		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		
		while( MonsterPos )
		{
			pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );
			if(pMonster == this || !pMonster->GetLord())	continue;	//자기자신이거나 소환물이 아닐경우는 Skip

			fLength		= CalcDistance( GetCurPosition(), pMonster->GetCurPosition());

			if( fLength > m_fSight )	continue;		//시야 밖이면 다른놈을 찾고..

			dwHP = pMonster->GetExp();
			if( dwHP < dwCurMinHP && pMonster->GetUnitStatus() != UNIT_STATUS_DEAD 
				&& this != pMonster )
			{
				if( m_dwTemp[NPC_TEMP_TARGET] == 1 || m_dwTemp[NPC_TEMP_TARGET] == 3 )
				{
					dwCurMinHP	= dwHP;
					pFindObject = pMonster;
				}
				else if( m_dwTemp[NPC_TEMP_TARGET] == 0 )
				{
					if( pMonster->GetRace() == OBJECT_TYPE_GUARDIAN
						&& !pMonster->m_dwTemp[ NPC_TEMP_DUNGEONGUARDIAN ])
					{
						dwCurMinHP	= dwHP;
						pFindObject = pMonster;
					}
				}
			}
		}
	}

	return pFindObject;
}

void* CMonster::AI_FindObjectAttachDetectAndNearAndFewHP(BYTE bObjectType)
{
	void*	pFindObject = NULL;
	double	fMin	= 1000000.f;
	double	fMax	= 0.f;			// 이몬스터에서 가장 가까운 유저를 찾기 위함.
	double	fLength	= 0.f;
	double  fTarget	= 0.f;
	double	fTemp	= 0.f;

	POSITION_	UserPos=0;
	POSITION_	MonsterPos=0;
	CSection*	pLinkSection=0;
	CMonster*	pMonster=0;
	CUser*		pUser=0;
	int i=0;

	if (bObjectType == OBJECT_TYPE_PLAYER)	goto lbl_user;
	else if (bObjectType == OBJECT_TYPE_MONSTER)	goto lbl_monster;
	else if (bObjectType == OBJECT_TYPE_SUMMON)		goto lbl_summon;
	else	return	NULL;
	
lbl_user:
	UserPos		= GetCurSection()->m_pPcList->GetHeadPosition();
	while( UserPos )
	{
		pUser	= (CUser*)GetCurSection()->m_pPcList->GetNext( UserPos );
		
		if ( !pUser )				continue;	// added by minjin.
		if ( !m_pBaseMonster )		continue;	// added by minjin.

		// 대상과의 거리를 구한다.
		fLength = CalcDistance( GetCurPosition(), pUser->GetCurPosition());
		if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
		
		fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
		fTarget = (float)pUser->GetLevel() / (float)pUser->GetHP() 
			* pfClass[ pUser->GetClass() ] * fTemp;

		if( pUser->GetUnitStatus() != UNIT_STATUS_DEAD
			&& !pUser->m_enGodMode)
		{
			if( fMax < fTarget )
			{
				// 만약 자신의 주인이라면 타켓으로 삼지 않는다.
				if( GetLord() )
				{
					if( GetLord() != pUser )
					{
						fMax = fLength;
						pFindObject = pUser;
					}
				}
				else if( IsCurDungeonSiege() )	// 만약 공성중이라면.
				{
					if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
					{
						// 던젼 주인이라면 타켓으로 잡지 않는다.
						fMin		= fLength;
						pFindObject = pUser;						
					}
				}
				else
				{
					fMax = fLength;
					pFindObject = pUser;
				}
			}
		}
	}
	
	for(  i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;

		UserPos = pLinkSection->m_pPcList->GetHeadPosition();
		
		while( UserPos )
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext( UserPos );
			
			if ( !pUser )				continue;	// added by minjin.
			if ( !m_pBaseMonster )		continue;	// added by minjin.

			fLength = CalcDistance( GetCurPosition(), pUser->GetCurPosition() );
			if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.

			fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
			fTarget = (float)pUser->GetLevel() / (float)pUser->GetHP() 
				* pfClass[ pUser->GetClass() ] * fTemp;

			if( pUser->GetUnitStatus() != UNIT_STATUS_DEAD 
				&& (!pUser->GetEffectDesc(__SKILL_HIDING__ )
				//(정우창 2005.03.07) - 은신상태에서 이동을 할 경우에도 은신상태로 수정(클라이언트) 후 이동할 경우 Monster가 은신상태가 풀린상태로 판단하는 부분 수정 
			/*		|| (pUser->GetEffectDesc(__SKILL_HIDING__ )
						&& (pUser->GetUnitStatus() == UNIT_STATUS_WALKING 
							|| pUser->GetUnitStatus() == UNIT_STATUS_RUNNING)) */ ) 
				&& !pUser->m_enGodMode)
			{
				if( fMax < fTarget )
				{
					// 만약 자신의 주인이라면 타켓으로 삼지 않는다.
					if( GetLord() )
					{
						if( GetLord() != pUser )
						{
							fMax = fLength;
							pFindObject = pUser;
						}
					}
					else if( IsCurDungeonSiege() )	// 만약 공성중이라면.
					{
						// 던젼 주인이라면 타켓으로 잡지 않는다.
						if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
						{
							fMin		= fLength;
							pFindObject = pUser;
							
						}
					}
					else
					{
						fMax = fLength;
						pFindObject = pUser;
					}
				}
			}
		}
	}
	return pFindObject;

lbl_monster:
	MonsterPos		= GetCurSection()->m_pMonsterList->GetHeadPosition();
	while( MonsterPos )
	{
		pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );

		if ( !pUser )				continue;	// added by minjin.
		if ( !m_pBaseMonster )		continue;	// added by minjin.

		// 대상과의 거리를 구한다.
		fLength = CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );
		if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
			
		fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
		fTarget = (float)pMonster->GetLevel() / (float)pMonster->GetHP() 
			* pfClass[ pMonster->GetClass() ] * fTemp;

		if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD && this != pMonster )
		{
			if( fMax < fTarget )
			{
				// 만약 자신의 주인이 소환한 몬스터는 타켓으로 삼지 않는다.
				if( GetLord() )
				{
					if( GetLord() != pMonster->GetLord() )
					{
						fMax = fLength;
						pFindObject = pMonster;
					}
				}
				else
				{
					fMax = fLength;
					pFindObject = pMonster;
				}
			}
		}

	}
	
	for(  i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;
		
		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		
		while( MonsterPos )
		{
			pMonster		= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );
			// 대상과의 거리를 구한다.
			fLength = CalcDistance( GetCurPosition(), pMonster->GetCurPosition());
			if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
				
			fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
			fTarget = (float)pMonster->GetLevel() / (float)pMonster->GetHP() 
				* pfClass[ pMonster->GetClass() ] * fTemp;

			if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD && this != pMonster )
			{
				if( fMax < fTarget )
				{
					// 만약 자신의 주인이 소환한 몬스터는 타켓으로 삼지 않는다.
					if( GetLord() )
					{
						if( GetLord() != pMonster->GetLord() )
						{
							fMax = fLength;
							pFindObject = pMonster;
						}
					}
					else
					{
						fMax = fLength;
						pFindObject = pMonster;
					}
				}
			}
		}
	}
	
	return pFindObject;

	
lbl_summon:
	MonsterPos		= GetCurSection()->m_pMonsterList->GetHeadPosition();
	while( MonsterPos )
	{
		pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );

		if(pMonster == this || !pMonster->GetLord())	continue;	//자기자신이거나 소환물이 아닐경우는 Skip

		// 대상과의 거리를 구한다.
		fLength = CalcDistance( GetCurPosition(), pMonster->GetCurPosition());
		if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
			
		fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
		fTarget = (float)pMonster->GetLevel() / (float)pMonster->GetHP() 
			* pfClass[ pMonster->GetClass() ] * fTemp;

		if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD && this != pMonster )
		{
			if( fMax < fTarget )
			{
				// 만약 자신의 주인이 소환한 몬스터는 타켓으로 삼지 않는다.
				if( GetLord() )
				{
					if( GetLord() != pMonster->GetLord() )
					{
						fMax = fLength;
						pFindObject = pMonster;
					}
				}
				else
				{
					fMax = fLength;
					pFindObject = pMonster;
				}
			}
		}

	}
	
	for( i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;
		
		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		
		while( MonsterPos )
		{
			pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );

			if(pMonster == this || !pMonster->GetLord())	continue;	//자기자신이거나 소환물이 아닐경우는 Skip
			// 대상과의 거리를 구한다.
			fLength = CalcDistance( GetCurPosition(), pMonster->GetCurPosition());
			if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
				
			fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
			fTarget = (float)pMonster->GetLevel() / (float)pMonster->GetHP() 
				* pfClass[ pMonster->GetClass() ] * fTemp;

			if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD && this != pMonster )
			{
				if( fMax < fTarget )
				{
					// 만약 자신의 주인이 소환한 몬스터는 타켓으로 삼지 않는다.
					if( GetLord() )
					{
						if( GetLord() != pMonster->GetLord() )
						{
							fMax = fLength;
							pFindObject = pMonster;
						}
					}
					else
					{
						fMax = fLength;
						pFindObject = pMonster;
					}
				}
			}
		}
	}
	
	return pFindObject;
}

void* CMonster::AI_FindObjectNearAndFewHP(BYTE bObjectType)
{
	void*	pFindObject = NULL;
	double	fMin	= 1000000.f;
	double	fMax	= 0.f;			// 이몬스터에서 가장 가까운 유저를 찾기 위함.
	double	fLength	= 0.f;
	double  fTarget	= 0.f;
	double	fTemp	= 0.f;

	POSITION_	UserPos=0;
	POSITION_	MonsterPos=0;
	CSection*	pLinkSection=0;
	CMonster*	pMonster=0;
	CUser*		pUser=0;
	int i=0;

	if (bObjectType == OBJECT_TYPE_PLAYER)	goto lbl_user;
	else if (bObjectType == OBJECT_TYPE_MONSTER)	goto lbl_monster;
	else if (bObjectType == OBJECT_TYPE_SUMMON)		goto lbl_summon;
	else	return	NULL;
	
lbl_user:
	UserPos		= GetCurSection()->m_pPcList->GetHeadPosition();
	while( UserPos )
	{
		pUser	= (CUser*)GetCurSection()->m_pPcList->GetNext( UserPos );
		
		// 대상과의 거리를 구한다.
		fLength = CalcDistance( GetCurPosition(), pUser->GetCurPosition());
		if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
			
		fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
		fTarget = (float)pUser->GetLevel() / (float)pUser->GetHP() 
			* pfClass[ pUser->GetClass() ] * fTemp;

		if( pUser->GetUnitStatus() != UNIT_STATUS_DEAD 
			&& (!pUser->GetEffectDesc(__SKILL_HIDING__ )
			//(정우창 2005.03.07) - 은신상태에서 이동을 할 경우에도 은신상태로 수정(클라이언트) 후 이동할 경우 Monster가 은신상태가 풀린상태로 판단하는 부분 수정 
			/*	|| (pUser->GetEffectDesc(__SKILL_HIDING__ )
					&& (pUser->GetUnitStatus() == UNIT_STATUS_WALKING 
						|| pUser->GetUnitStatus() == UNIT_STATUS_RUNNING)) */ ) 
			&& !pUser->m_enGodMode)
		{
			if( fMax < fTarget )
			{
				// 만약 자신의 주인이라면 타켓으로 삼지 않는다.
				if( GetLord() )
				{
					if( GetLord() != pUser )
					{
						fMax = fLength;
						pFindObject = pUser;
					}
				}
				else if( IsCurDungeonSiege() )	// 만약 공성중이라면.
				{
					if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
					{
						// 던젼 주인이라면 타켓으로 잡지 않는다.
						fMin		= fLength;
						pFindObject = pUser;						
					}
				}
				else
				{
					fMax = fLength;
					pFindObject = pUser;
				}
			}
		}
	}
	
	for(  i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;

		UserPos = pLinkSection->m_pPcList->GetHeadPosition();
		
		while( UserPos )
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext( UserPos );
			
			fLength = CalcDistance( GetCurPosition(), pUser->GetCurPosition());
			if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.

			fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
			fTarget = (float)pUser->GetLevel() / (float)pUser->GetHP() 
				* pfClass[ pUser->GetClass() ] * fTemp;

			if( pUser->GetUnitStatus() != UNIT_STATUS_DEAD && 
				(!pUser->GetEffectDesc(__SKILL_HIDING__ )
				//(정우창 2005.03.07) - 은신상태에서 이동을 할 경우에도 은신상태로 수정(클라이언트) 후 이동할 경우 Monster가 은신상태가 풀린상태로 판단 
		/*			|| (pUser->GetEffectDesc(__SKILL_HIDING__ )
						&& (pUser->GetUnitStatus() == UNIT_STATUS_WALKING 
							|| pUser->GetUnitStatus() == UNIT_STATUS_RUNNING)) */ ) 
				&& !pUser->m_enGodMode)
			{
				if( fMax < fTarget )
				{
					// 만약 자신의 주인이라면 타켓으로 삼지 않는다.
					if( GetLord() )
					{
						if( GetLord() != pUser )
						{
							fMax = fLength;
							pFindObject = pUser;
						}
					}
					else if( IsCurDungeonSiege() )	// 만약 공성중이라면.
					{
						// 던젼 주인이라면 타켓으로 잡지 않는다.
						if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
						{
							fMin		= fLength;
							pFindObject = pUser;
							
						}
					}
					else
					{
						fMax = fLength;
						pFindObject = pUser;
					}
				}
			}
		}
	}
	return pFindObject;

lbl_monster:
	MonsterPos		= GetCurSection()->m_pMonsterList->GetHeadPosition();
	while( MonsterPos )
	{
		pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );

		// 대상과의 거리를 구한다.
		fLength = CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );
		if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
			
		fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
		fTarget = (float)pMonster->GetLevel() / (float)pMonster->GetHP() 
			* pfClass[ pMonster->GetClass() ] * fTemp;

		if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD && this != pMonster )
		{
			if( fMax < fTarget )
			{
				// 만약 자신의 주인이 소환한 몬스터는 타켓으로 삼지 않는다.
				if( GetLord() )
				{
					if( GetLord() != pMonster->GetLord() )
					{
						fMax = fLength;
						pFindObject = pMonster;
					}
				}
				else
				{
					fMax = fLength;
					pFindObject = pMonster;
				}
			}
		}
	}
	
	for(  i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;
		
		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		
		while( MonsterPos )
		{
			pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );
			// 대상과의 거리를 구한다.
			fLength = CalcDistance( GetCurPosition(), pMonster->GetCurPosition() );
			if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
				
			fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
			fTarget = (float)pMonster->GetLevel() / (float)pMonster->GetHP() 
				* pfClass[ pMonster->GetClass() ] * fTemp;

			if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD && this != pMonster )
			{
				if( fMax < fTarget )
				{
					// 만약 자신의 주인이 소환한 몬스터는 타켓으로 삼지 않는다.
					if( GetLord() )
					{
						if( GetLord() != pMonster->GetLord() )
						{
							fMax = fLength;
							pFindObject = pMonster;
						}
					}
					else
					{
						fMax = fLength;
						pFindObject = pMonster;
					}
				}
			}
		}
	}
	
	return pFindObject;
	
lbl_summon:
	MonsterPos		= GetCurSection()->m_pMonsterList->GetHeadPosition();
	while( MonsterPos )
	{
		pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );

		if(pMonster == this || !pMonster->GetLord())	continue;	//자기자신이거나 소환물이 아닐경우는 Skip

		// 대상과의 거리를 구한다.
		fLength = CalcDistance( GetCurPosition(), pMonster->GetCurPosition());
		if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
			
		fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
		fTarget = (float)pMonster->GetLevel() / (float)pMonster->GetHP() 
			* pfClass[ pMonster->GetClass() ] * fTemp;

		if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD && this != pMonster )
		{
			if( fMax < fTarget )
			{
				// 만약 자신의 주인이 소환한 몬스터는 타켓으로 삼지 않는다.
				if( GetLord() )
				{
					if( GetLord() != pMonster->GetLord() )
					{
						fMax = fLength;
						pFindObject = pMonster;
					}
				}
				else
				{
					fMax = fLength;
					pFindObject = pMonster;
				}
			}
		}
	}
	
	for(  i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;
		
		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		
		while( MonsterPos )
		{
			pMonster	= (CMonster*)GetCurSection()->m_pMonsterList->GetNext( MonsterPos );

			if(pMonster == this || !pMonster->GetLord())	continue;	//자기자신이거나 소환물이 아닐경우는 Skip
			// 대상과의 거리를 구한다.
			fLength = CalcDistance( GetCurPosition(), pMonster->GetCurPosition());
			if( fLength > m_fSight ) continue;		// 시야 밖의 유저이면 무시.
				
			fTemp	= 1.f - ( fLength / TILE_SIZE ) / (double)m_pBaseMonster->bSR * 0.5f;
			fTarget = (float)pMonster->GetLevel() / (float)pMonster->GetHP() 
				* pfClass[ pMonster->GetClass() ] * fTemp;

			if( pMonster->GetUnitStatus() != UNIT_STATUS_DEAD && this != pMonster )
			{
				if( fMax < fTarget )
				{
					// 만약 자신의 주인이 소환한 몬스터는 타켓으로 삼지 않는다.
					if( GetLord() )
					{
						if( GetLord() != pMonster->GetLord() )
						{
							fMax = fLength;
							pFindObject = pMonster;
						}
					}
					else
					{
						fMax = fLength;
						pFindObject = pMonster;
					}
				}
			}
		}
	}
	
	return pFindObject;

}

void CMonster::AI_DefenseNormal(const AI_DEFENSE_PARAM* pParam)
{
	
}

void CMonster::AI_DefenseRecovery(const AI_DEFENSE_PARAM* pParam)
{
	if (GetMaxHP()/2 > GetHP())
	{
		SetTargetAI(this);
		SendSkillPacket(BYTE(9 + rand() % 3), FALSE);
	}
}

void CMonster::AI_Defense_ChangeTargetToFierceObject(const AI_DEFENSE_PARAM* pParam)
{
	if( m_dwTemp[ NPC_TEMP_DAMAGE ] < pParam->dwTemp[AI_DEFENSE_TEMP_DAMAGE] )
	{
		m_dwTemp[ NPC_TEMP_DAMAGE ] = pParam->dwTemp[AI_DEFENSE_TEMP_DAMAGE];

		SetTargetAI( (CMonster*)pParam->pObject );		// 더큰 데미지를 준 넘으로 타겟을 바꾼다.
		
		SetStatus( UNIT_STATUS_NORMAL);
	}
}

void* CMonster::FindUser_Guardian()
{
	if( m_dwTemp[ NPC_TEMP_PEACE ] != 0 )		return NULL;
	if( m_dwTemp[ NPC_TEMP_FINDUSER ] != 0 )	return NULL;

	double	fMin	= 1000000.f;			// 이몬스터에서 가장 가까운 유저를 찾기 위함.
	double	fLength = 0.f;						// 가디언과의 거리.
//	double	fLengthLord= 0.f;					// 주인과의 거리.

	POSITION_	UserPos=0;//, MonsterPos;
	CSection*	pLinkSection=0;
//	CMonster*	pMonster=0;
	CUser*		pUser=0;
	
	m_dwTemp[NPC_TEMP_AIKIND] = 0;
	
	// 가디언은 기본적으로 일단 몬스터를 검사한다.

	
	UserPos		= GetCurSection()->m_pPcList->GetHeadPosition();
	// 유저에서 검사한다.
	while( UserPos )
	{
		pUser		= (CUser*)GetCurSection()->m_pPcList->GetNext( UserPos );
		fLength		= CalcDistance( GetCurPosition(), pUser->GetCurPosition());
//		fLengthLord = CalcDistance( &GetLord()->GetCurPosition(), &pUser->GetCurPosition() );

		if( fLength < fMin && 
			pUser->GetUnitStatus() != UNIT_STATUS_DEAD /*&&*/ 
			/*pUser != GetLord()*/ )
		{
			if( fLength <= m_fSight )
			{
				if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
				{
					if (GetLord() && (GetLord() != pUser))
					{
						fMin						= fLength;
						m_pUnitForAI				= pUser;
						m_dwTemp[NPC_TEMP_AIKIND]	= pUser->GetObjectType();						
					}					
				}				
			}
		}
	}
	
	for( int i=0; GetCurSection()->m_wLink[i] != 0; i++ )
	{
		pLinkSection = GetCurDungeonLayer()->GetSection( GetCurSection()->m_wLink[i] );
		if( !pLinkSection ) continue;

		UserPos = pLinkSection->m_pPcList->GetHeadPosition();
		
		while( UserPos )
		{
			pUser		= (CUser*)GetCurSection()->m_pPcList->GetNext( UserPos );
			fLength		= CalcDistance( GetCurPosition(), pUser->GetCurPosition());
//			fLengthLord = CalcDistance( &GetLord()->GetCurPosition(), &pUser->GetCurPosition() );

			if( fLength < fMin && 
				pUser->GetUnitStatus() != UNIT_STATUS_DEAD /*&&*/ 
				/*pUser != GetLord()*/ )
			{
				if( fLength <= m_fSight )
				{
					if( ( GetCurDungeon()->GetDungeonDataEx()->m_dwOwnerGuildNum == 0 ) || 
						( pUser->m_dwGuildId != GetCurDungeon()->GetDungeonDataEx()->m_dwOwnerGuildNum ) )
					{
						if (pUser->GetAttackMode() == ATTACK_MODE_OFFENSE)
						{
							if( __strcmp( pUser->m_szName, GetCurDungeon()->GetDungeonDataEx()->m_szOwner ) != 0 )
							{
								fMin						= fLength;
								m_pUnitForAI				= pUser;
								m_dwTemp[NPC_TEMP_AIKIND]	= pUser->GetObjectType();
							}
						}
					}
				}
			}
		}
	}
	
	if( m_dwTemp[NPC_TEMP_AIKIND] == OBJECT_TYPE_PLAYER 
		|| m_dwTemp[NPC_TEMP_AIKIND] == OBJECT_TYPE_MONSTER)
		return m_pUnitForAI;
	
	return NULL;
}

void CMonster::SearchUser()
{
	CMap*	pMap = GetCurDungeonLayer()->GetMap();
	BYTE	bSight = (BYTE)( m_fSight / TILE_SIZE );
	int		iRndX=0, iRndZ=0;
	static int iDir = 0;	// rand 함수 제거용, 낭비잖아! : 최덕석 2005.1.11
	
	iRndX = (rand() % bSight) + 1;
	iRndZ = (rand() % bSight) + 1;
	
	if( iDir++ % 2 )		// 위의 rand 함수 제거의 연장 : 최덕석
	{
		iRndX *= -1;
		iRndZ *= -1;
	}
	
	// 타일세팅
	iRndX += GetCurMapTile()->wIndex_X;
	iRndZ += GetCurMapTile()->wIndex_Z;
	
	if( iRndX < 0 ) iRndX = 0;
	else if( (DWORD)iRndX > pMap->m_dwTileNumWidth-1 ) iRndX = pMap->m_dwTileNumWidth-1; 
	
	if( iRndZ < 0 ) iRndZ = 0;
	else if( (DWORD)iRndZ > pMap->m_dwTileNumHeight-1 ) iRndZ = pMap->m_dwTileNumHeight-1;
	
	VECTOR2	v2Dest = {(float)iRndX, (float)iRndZ};
	if( PathFind( &v2Dest ) )
	{
		SetStatus( UNIT_STATUS_WALKING);
	}
	else 
	{
		SetStatus( UNIT_STATUS_NORMAL);
	}
}

void CMonster::SearchUser_Guardian()
{
	if( GetLord() )	// 주인이 있는것이라면 주인과 일절 거리가 떨어지면 주인에게로 이동한다.
	{
		if(!GetLord()->GetClass())
		{
			Log( LOG_NORMAL, "Invalid UserLord in SearchUser_Guardian Func" );
			return;
		}		

		float fLength = CalcDistance( GetLord()->GetCurPosition(), GetCurPosition());
		
		if( fLength >= 400.f )
		{
			if (!GetLord()->GetCurMapTile())	return;
			
			VECTOR2	LordPos = 
			{
				GetLord()->GetCurMapTile()->wIndex_X, 
				GetLord()->GetCurMapTile()->wIndex_Z
			};
			
			if( PathFind( &LordPos ) )
			{
				SetStatus( UNIT_STATUS_WALKING);
			}
			else SetStatus( UNIT_STATUS_NORMAL);
		}

		/////
		// 가디언이 시야 밖이면 속도 부스트 사용 : 최덕석
		// 시야 범위 1.3배면
		if( fLength > m_fSight * 1.3)
		{
			// 순간이동 속도로 설정
			SpeedUp(100);
		}
		// 시야 범위 밖이면
		else if( fLength >= m_fSight)
		{
			// 속도 부스트
			SpeedUp(2.5);
		}
		// Ai4 쪽에서 자동으로 SpeedNormal 호출 : 최덕석
		/////

		return;
	}
//----------------------------------------------------------------------------

	if( m_dwTemp[ NPC_TEMP_PEACE ] != 0 ) return;

	CMap*	pMap = GetCurDungeonLayer()->GetMap();
	BYTE	bSight = 2;//(BYTE)( m_fSight / TILE_SIZE );
	int		iRndX=0, iRndZ=0, iDir=0;

	iDir  = rand() % 2;
	iRndX = (rand() % bSight) + 1;
	iRndZ = (rand() % bSight) + 1;
	
	if( iDir )
	{
		iRndX *= -1;
		iRndZ *= -1;
	}
	
	// 타일세팅 주인의 타일을 기준으로 움직인다. ㅋ ㅋ ㅋ 

	if( GetLord() )
	{
		iRndX += GetLord()->GetCurMapTile()->wIndex_X;
		iRndZ += GetLord()->GetCurMapTile()->wIndex_Z;
	}
	else
	{
		iRndX += GetCurMapTile()->wIndex_X;
		iRndZ += GetCurMapTile()->wIndex_Z;
	}
	
	if( iRndX < 0 ) iRndX = 0;
	else if( (DWORD)iRndX > pMap->m_dwTileNumWidth-1 ) iRndX = pMap->m_dwTileNumWidth-1; 
	
	if( iRndZ < 0 ) iRndZ = 0;
	else if( (DWORD)iRndZ > pMap->m_dwTileNumHeight-1 ) iRndZ = pMap->m_dwTileNumHeight-1;
	
	VECTOR2	v2Dest = { (float)iRndX, (float)iRndZ };
	if( PathFind( &v2Dest ) )
	{
		SetStatus( UNIT_STATUS_WALKING);
		/*
		if( GetLord() )
		{
			if( GetLord()->GetUnitStatus() == PC_STATUS_NORMAL )
				m_dwTemp[NPC_TEMP_MOVE]	= 1;		// 이 플래그가 1이면 이동 종료후 딜레이타임으로 들어간다.
			else 
				m_dwTemp[NPC_TEMP_MOVE]	= 0;
		}*/
	}
	else 
	{
		SetStatus( UNIT_STATUS_NORMAL);
	}
}

void CMonster::DisAppearUnit( DWORD dwUnitID )
{
	if( !m_pUnitForAI )	
		return;
	if( m_pUnitForAI->GetID() != dwUnitID ) 
		return;
		
	m_pUnitForAI = NULL;
	if( GetUnitStatus() == UNIT_STATUS_WALKING )	// 움직이고 있는 상태라면 정지 메시지를 보낸다.
	{
		DSTC_STOP_MON	StopMon;
		StopMon.dwMonsterIndex	= GetID();
		StopMon.v2StopPos		= *GetCurPosition();
		GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&StopMon
			, StopMon.GetPacketSize()
			, (BYTE)GetCurSection()->m_bSectionNum );
	}

	SetStatus( UNIT_STATUS_NORMAL);
}

BOOL CMonster::PathFind(const VECTOR2* v2Dest /* = NULL  */)
{
	BlockLocation* pBlock;
	DWORD dwStartX, dwStartZ, dwEndX, dwEndZ; // 시작 지점과 목적지.
	
	dwStartX = (DWORD)( GetCurPosition()->x / TILE_SIZE );
	dwStartZ = (DWORD)( GetCurPosition()->y / TILE_SIZE );
	
	if( m_pUnitForAI )
	{
		dwEndX = (DWORD)( m_pUnitForAI->GetCurPosition()->x / TILE_SIZE );
		dwEndZ = (DWORD)( m_pUnitForAI->GetCurPosition()->y / TILE_SIZE ); 
	}
	else 
	{
		if( v2Dest )
		{
			dwEndX = (DWORD)v2Dest->x;
			dwEndZ = (DWORD)v2Dest->y;
		}
		else
		{
			// 타겟도 없고, 갈 곳도 없음 : 최덕석 2005.1.25
			return FALSE;
		}
	}

	// 아메리타트이고 위치가 도착지가 결계 밖이면, 임시
	if( this->m_pBaseMonster->dwID == 81 && 
		(dwEndX < 86 || dwEndX > 100 || dwEndZ < 60 || dwEndZ > 73))
	{
		// 나갈 수 없음
		return FALSE;
	}

	m_PathFinder.dwCurveNum	= GetCurDungeonLayer()->m_pSw->FindShortestWay( dwStartX, 
													dwStartZ,
													dwEndX,
													dwEndZ, &pBlock );
	// 길찾기에 성공 하였다면.
	if( m_PathFinder.dwCurveNum > 0 )
	{
		m_PathFinder.dwCurCurve = 0;
		memcpy( m_PathFinder.pLocation, pBlock, sizeof(BlockLocation) * m_PathFinder.dwCurveNum );
		return TRUE;
	}

	return FALSE;
}

BYTE CMonster::GetAIType(BYTE bAIKind)
{
	switch(bAIKind)
	{
	case AI_KIND_MOVETYPE:
		if (m_pBaseMonster->sAiType.bMoveType == 2)
			return BYTE(rand()%2);
		
		return m_pBaseMonster->sAiType.bMoveType;

	case AI_KIND_SEARCHTYPE:
		return m_pBaseMonster->AI.bSetTargetType;

	case AI_KIND_ATTACKTYPE:
		if (m_pBaseMonster->sAiType.bAttackType == 2)
			return 0;
		return m_pBaseMonster->sAiType.bAttackType;

	case AI_KIND_DEFENSETYPE:
		if (m_pBaseMonster->sAiType.bDefenseType == 3)
			return BYTE(rand()%3);
		
		return m_pBaseMonster->sAiType.bDefenseType;
	}
	return 0;
}

// NPC
void CMonster::AI0()
{
	
}

void CMonster::AI1()
{
	if( !m_pUnitForAI)		// 기준 유저가 없으면 적당한 유저를 찾는다.
	{
		if( !FindObject( GetAIType(AI_KIND_SEARCHTYPE)) )
		{
			SearchUser();	// 적당한 유저를 찾지 못했다면 자리를 이동해본다.
			return;
		}	
	}
	
	float fLength=0.f;

	// AI 타겟과의 거리를 구한다. 
	if( m_pUnitForAI )
	{
		fLength		= CalcDistance( GetCurPosition(), m_pUnitForAI->GetCurPosition() );
	}
	
	// 검색 범위를 지나갔다면 끝.
	if( fLength > m_fSight )						
	{
		SetStatus( UNIT_STATUS_NORMAL);
		m_pUnitForAI	= NULL;
	}
	// 타겟과의 거리가 공격가능거리보다 멀고 현재 몬스터 상태가 NORMAL 상태이면 	
	else if( (int)fLength > (int)m_fAttackRange && ( GetUnitStatus() == UNIT_STATUS_NORMAL ) )
	{
		// 스킬을 쓸수 없는 몬스터이거나 현재 스킬을 사용할 수 없는 상태이면 
		if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
		{
			if( PathFind() )	//길찾기를 해서 
				SetStatus( UNIT_STATUS_WALKING );	//성공이면 이동 
			else
				SetStatus( UNIT_STATUS_NORMAL );	//실패면 가만히 있어라 
		}
	}
	else
	{
		// Attack
		DWORD	dwTime = g_dwTickCount;
		
		if( GetUnitStatus() == UNIT_STATUS_NORMAL )
		{
			if(!m_dwTemp[NPC_TEMP_SKILLMANY] 
				|| !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
			{
				if( dwTime - m_dwTemp[NPC_TEMP_LASTATTACKTICK] > GetAttackSpeed() )
				{
					m_dwTemp[NPC_TEMP_LASTATTACKTICK] = dwTime;
					SendAttackPacket();
				}
			}
		}
	}
}

void CMonster::AI2()
{
	if (!m_pUnitForAI)		// 기준 유저가 없으면 적당한 유저를 찾는다.
	{
		if( !FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )	// 적당한 유저를 못찾았으면 걍 멍한상태로 계속.
		{
			SearchUser();	// 적당한 유저를 찾지 못했다면 자리를 이동해본다.
			return;
		}	
	}
	
	int fLength=0.f;

	if( m_pUnitForAI )
	{
		fLength	= CalcDistance( GetCurPosition(), m_pUnitForAI->GetCurPosition() );		// 가디언과의 거리.
	}
	
	if( fLength > m_fSight )	// 검색 범위를 지나갔다면 끝.
	{
		SetStatus( UNIT_STATUS_NORMAL );
		m_pUnitForAI	= NULL;
		
	}
	else if( (int)fLength > (int)m_fAttackRange && GetUnitStatus() == UNIT_STATUS_NORMAL)
	{
		if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
		{
			if( PathFind() )
				SetStatus( UNIT_STATUS_WALKING );
			else 
				SetStatus( UNIT_STATUS_NORMAL );
		}
	}
	else
	{
		// Attack
		DWORD	dwTime = g_dwTickCount;
		
		if( GetUnitStatus() == UNIT_STATUS_NORMAL )
		{
			if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
			{
				if( dwTime - m_dwTemp[NPC_TEMP_LASTATTACKTICK] > GetAttackSpeed() )
				{
					m_dwTemp[NPC_TEMP_LASTATTACKTICK] = dwTime;
					SendAttackPacket();
				}
			}
		}
	}
}

BOOL CMonster::AttackAfterMove()
{
	if( !m_pUnitForAI )		
		return FALSE;
	
	float fLength = CalcDistance( GetCurPosition(), m_pUnitForAI->GetCurPosition() );		
	
	if( (int)fLength <= (int)m_fAttackRange)
	{
		DWORD	dwTime = g_dwTickCount;
		
		if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
		{
			if( dwTime - m_dwTemp[NPC_TEMP_LASTATTACKTICK] > GetAttackSpeed() )
			{
				m_dwTemp[NPC_TEMP_LASTATTACKTICK] = dwTime;
				SendAttackPacket();
				return TRUE;
			}
		}		
	}

	return FALSE;
}

// 현재 이 몬스터가 폭주모드 인가?
BOOL CMonster::IsCongestionMode() const
{
	return (m_dwTemp[NPC_TEMP_CONGESTION_MODE] > 0);
}

int CMonster::GetExpDying()
{
	if( GetLevel() == 255 ) return 0;

	return (int)(min(GetExp() - GetExpTableOfLevel(OBJECT_TYPE_MONSTER, GetLevel() ),
				(GetExpTableOfLevel(OBJECT_TYPE_MONSTER,  GetLevel() + 1) - GetExpTableOfLevel(OBJECT_TYPE_MONSTER, GetLevel() ))*0.05f ));
}

// 일반적인 몬스터 AI
void CMonster::AI_New()
{
	if (!m_pUnitForAI)		// 기준 유저가 없으면 적당한 유저를 찾는다.
	{
		if(GetAI_FirstAttack() == AI_FIRST_ATTACK_NONE)	//비선공 몬스터
		{
			SearchUser();	//자리를 이동해본다.
			return;
		}
		else if( !FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )
		{
			SearchUser();	//자리를 이동해본다.
			return;
		}
	}
	
	float fLength=0.f;
	if( m_pUnitForAI )
		fLength	= CalcDistance( GetCurPosition(), m_pUnitForAI->GetCurPosition());		// 가디언과의 거리.
		
	if( fLength > m_fSight )	// 검색 범위를 지나갔다면 끝.
	{
		SetStatus( UNIT_STATUS_NORMAL );
		m_pUnitForAI	= NULL;
	}
	//대상이 공격범위 밖에 있고, 몬스터 상태가 Normal 상태이면 이동
	else if( (int)fLength > (int)m_fAttackRange)	
	{
		if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
		{
			if( PathFind() )
			{
				if(GetAI_RunningRate() > 1.0f)
					SetStatus( UNIT_STATUS_RUNNING );
				else
					SetStatus( UNIT_STATUS_WALKING );
			}
			else 
			{
				SetStatus( UNIT_STATUS_NORMAL );
			}
		}
	}
	else
	{
		// Attack
		DWORD	dwTime = g_dwTickCount;
		
		if( GetUnitStatus() == UNIT_STATUS_NORMAL )
		{
			if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
			{
				if( dwTime - m_dwTemp[NPC_TEMP_LASTATTACKTICK] > GetAttackSpeed() )
				{
					m_dwTemp[NPC_TEMP_LASTATTACKTICK] = dwTime;
					SendAttackPacket();
				}
			}
		}
	}
}

void CMonster::AI3()
{
	if (!m_pUnitForAI)		// 기준 유저가 없으면 적당한 유저를 찾는다.
	{		
		if( !FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )	// 적당한 유저를 못찾았으면 걍 멍한상태로 계속.
		{
			SearchUser();	// 적당한 유저를 찾지 못했다면 자리를 이동해본다.
			return;
		}	
	}
	
	float fLength = 0.f;

	// 대상과의 거리를 구한다.
	if (m_pUnitForAI)
	{
		fLength	= CalcDistance( GetCurPosition(), m_pUnitForAI->GetCurPosition());
	}
	
	if( fLength > m_fSight )	// 검색 범위를 지나갔다면 끝.
	{
		SetStatus( UNIT_STATUS_NORMAL );
		m_pUnitForAI	= NULL;
	}
	else if( (int)fLength > (int)m_fAttackRange && ( GetUnitStatus() == UNIT_STATUS_NORMAL ) )
	{
		if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
		{
			if( PathFind() )
				SetStatus( UNIT_STATUS_WALKING );
			else 
				SetStatus( UNIT_STATUS_NORMAL );
		}
	}
	else
	{
		// Attack
		DWORD	dwTime = g_dwTickCount;
		
		if( GetUnitStatus() == UNIT_STATUS_NORMAL )
		{
			if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
			{
				if( dwTime - m_dwTemp[NPC_TEMP_LASTATTACKTICK] > GetAttackSpeed() )
				{
					m_dwTemp[NPC_TEMP_LASTATTACKTICK] = dwTime;
					SendAttackPacket();
				}
			}
		}
	}
}

// for guardian
void CMonster::AI4()
{	
	SpeedNormal();

	if (!m_pUnitForAI)	
	{
		if( !FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )
		{
			SearchUser_Guardian();
			return;
		}
	}
	if (m_pUnitForAI &&  (m_pUnitForAI->GetUnitStatus() == UNIT_STATUS_NONE || m_pUnitForAI->IsAlliance((CUnit*)this))	)
		return;
	
	if (!GetLord())
		return;

	int iLengthMon		= CalcDistance(GetCurPosition(), m_pUnitForAI->GetCurPosition());				// 몹과의 거리
	int iLengthLord	= CalcDistance(GetCurPosition(), GetLord()->GetCurPosition());						// 주인과의 거리
	
	BYTE bSkill = 0;
	
	// 쓸 수 있는 스킬이 있다면
	if(m_dwTemp[NPC_TEMP_SKILLMANY] > 0)
	{
		// 가디언이면
		if(this->IsGuardian())
		{
			// 선택된 스킬을 쓴다
			bSkill = GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON);
		
			// 선택된 스킬이 없다면
			if(bSkill == 0)
			{
				// 랜덤하게 쓴다
				bSkill = rand()%m_dwTemp[NPC_TEMP_SKILLMANY];
			}
			// 선택된 스킬이 있다면
			else
			{
				// 선택된 스킬의 Index를 구한다
				for(int i=0;i<MAX_GUARDIAN_USE_SKILL;i++)
				{
					if(m_MonsterSkill[i].bySkillKind == bSkill)
					{
						bSkill = i;
						break;
					}
				}

				// 선택된 스킬이 배우지 않은 것이면
				if(i == MAX_GUARDIAN_USE_SKILL)
				{
					// 렌덤하게 쓴다
					bSkill = rand()%m_dwTemp[NPC_TEMP_SKILLMANY];
				}
			}
		}
		// 엘리멘탈이면
		else
		{
			// 렌덤하게 쓴다
			bSkill = rand()%m_dwTemp[NPC_TEMP_SKILLMANY];
		}
	}

	switch(GetUnitStatus())
	{
	case UNIT_STATUS_NORMAL:
		if(iLengthMon > (int)m_fAttackRange)	// 멀면 가까이 가기
		{
			if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket(bSkill))
			{
				if( PathFind() )
					SetStatus( UNIT_STATUS_WALKING );
				else
					SetStatus( UNIT_STATUS_NORMAL );
			}
		} 
		else if(iLengthLord > m_fSight)	// 싸우면서 주인이 안보이면, 주인에게로 복귀
		{
			m_pUnitForAI = NULL;
			SetStatus( UNIT_STATUS_NORMAL);
		}
		else
		{
			// 쳐라!!
			DWORD	dwTime = g_dwTickCount;
			if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket(bSkill))
			{
				if( dwTime - m_dwTemp[NPC_TEMP_LASTATTACKTICK] > GetAttackSpeed() )
				{
					m_dwTemp[NPC_TEMP_LASTATTACKTICK] = dwTime;
					SendAttackPacket();
				}
			}
			
		}
		break;

	}
}

void CMonster::AI5()
{
	if( !m_pUnitForAI )	
	{
		if( !FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )
		{
			SearchUser_Guardian();
			return;
		}
	}

	//둘간의 거리를 구한다.
	float fLength=0.f;

	if( m_pUnitForAI )
		fLength		= CalcDistance( GetCurPosition(), m_pUnitForAI->GetCurPosition() );				// 가디언과의 거리.

	if( fLength > m_fSight )	// 주인과의 거리가 멀면 다시 생각한다.
	{
		SetStatus( UNIT_STATUS_NORMAL );
		m_pUnitForAI	= NULL;
		
	}
	else if( (int)fLength > (int)m_fAttackRange && ( GetUnitStatus() == UNIT_STATUS_NORMAL ) )
	{
		if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
		{
			if( PathFind() )
				SetStatus( UNIT_STATUS_WALKING );
			else
				SetStatus( UNIT_STATUS_NORMAL );
		}
	}
	else
	{
		// Attack
		DWORD	dwTime = g_dwTickCount;
		
		if( GetUnitStatus() == UNIT_STATUS_NORMAL )
		{
			if(!m_dwTemp[NPC_TEMP_SKILLMANY] || !SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY])) )
			{
				if( dwTime - m_dwTemp[NPC_TEMP_LASTATTACKTICK] > GetAttackSpeed() )
				{
					m_dwTemp[NPC_TEMP_LASTATTACKTICK] = dwTime;
					SendAttackPacket();
				}
			}
		}
	}
}

void CMonster::AI6()
{
	if (!m_pUnitForAI)		// 기준 유저가 없으면 적당한 유저를 찾는다.
	{
		if( !FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )
		{			
			return;
		}	
	}

	if (m_dwTemp[NPC_TEMP_SKILLMANY])
	{
		SendSkillPacket((BYTE)(rand()%m_dwTemp[NPC_TEMP_SKILLMANY]));
	}
}

// 보호자 : 최덕석 2005.2.7
void CMonster::AI8()
{
	SpeedNormal();
	
	if (!m_pUnitForAI)	
	{
		if( !FindObject(GetAIType(AI_KIND_SEARCHTYPE)))
		{
			if(GetLord())
			{
				SearchUser_Guardian();
			}
			else
			{
				SearchUser();
			}
			
			return;
		}
	}

	int iLengthMon	= CalcDistance(GetCurPosition(), m_pUnitForAI->GetCurPosition());				// 몹과의 거리
	int iLengthLord;

	// 주인이 살았을 때
	if(GetLord())
	{
		iLengthLord = CalcDistance(GetCurPosition(), GetLord()->GetCurPosition());					// 주인과의 거리
	
		// 힐 사용을 위해 잠시 주인을 타겟으로
		CUnit *pOldUnit = m_pUnitForAI;
		m_pUnitForAI = GetLord();

		// 주인을 보호하라!!
		if((rand() % 10) > 8 && SendSkillPacket(0))
		{
			m_pUnitForAI = pOldUnit;
			return;
		}

		// 타겟을 다시 잡음
		m_pUnitForAI = pOldUnit;
	}
	// 주인이 죽으면 일반 몹
	else
	{
		iLengthLord = 0;
	}

	switch(GetUnitStatus())
	{
	case UNIT_STATUS_NORMAL:
		if(iLengthMon > (int)m_fAttackRange)	// 멀면 가까이 가기
		{
			if(!SendSkillPacket(2) )
			{
				if( PathFind() )
					SetStatus( UNIT_STATUS_NORMAL );
			}
		} 
		else if(iLengthLord > m_fSight)	// 싸우면서 주인이 안보이면, 주인에게로 복귀
		{
			m_pUnitForAI = NULL;
			SetStatus( UNIT_STATUS_NORMAL);
		}
		else
		{
			// 쳐라!!
			SendSkillPacket(1);
		}
		break;

	}
}

// 아메리타트 : 최덕석 2005.1.26
void CMonster::AI9()
{

#define DRAGON_AI_HEAL_RATE			5000		// Self healing rate
#define DRAGON_AI_HEAL_AMOUNT		1000		// heal amount

	static DWORD dwLastHeal = 0;

	float fHPRate = (float)GetHP() / (float)GetMaxHP();
	
	// 대상이 없다면
	if (!m_pUnitForAI)
	{
		// 대상을 찾아보고 없다면
		if( !FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )
		{	
			// HP를 회복한다
			if((g_dwTickCount - dwLastHeal) > DRAGON_AI_HEAL_RATE)
			{
				SetHP(min(GetHP()+DRAGON_AI_HEAL_AMOUNT, GetMaxHP()));
				dwLastHeal = g_dwTickCount;

				// 폭주모드 이면
				if(m_bCongestionMode)
				{
					// 폭주 해제!
					m_bCongestionMode = FALSE;

					// 폭주 상태를 알린다
					DSTC_CONGESTION_MODE CongestionMode;
					CongestionMode.bOn = 0;
					CongestionMode.dwMonsterID = GetID();
					GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&CongestionMode, CongestionMode.GetPacketSize(), GetPrevSectionNum() );
				
					// 이속 감소
					m_fMoveSpeed /= 1.5;
				}
			}

			// 어슬렁 거려라
			SearchUser();

			return;
		}
	}


	// HP가 20% 남고, 유에린이 죽었다면 다시 소환!!
	if(fHPRate < 0.2 && !GetStaff())
	{
		// 투두 : 유에린 소환
		LPBASEMONSTER pBaseMonster = g_pBaseMonsterHash->GetData( 82 );
		CMonster* pMonster = g_pMonsterTable->Add(UNIT_CLASS_ID_MONSTER);
		pMonster->SetDestPosition(this->GetCurPosition());
		this->GetCurDungeonLayer()->InsertMonster( pMonster, this->GetCurPosition() );
		pMonster->Initialize(pBaseMonster, NULL, NULL, 0, this->GetLevel(), 0);
		pMonster->SetInfoIndex( 0xffff );		// 몬
	
		// 몬스터 생성을 알린다.
		DSTC_APPEAR_MON AppearMon;
		MakeAppearMonser( &AppearMon, pMonster );
		pMonster->GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&AppearMon, AppearMon.GetPacketSize(), pMonster->GetPrevSectionNum() );

		pMonster->SetLord(this);
		SetStaff(pMonster);
	}

	// HP가 10%보다 적고, 폭주가 아니면
	if(fHPRate < 0.1 && !m_bCongestionMode)
	{
		// 폭주 시켜라!!
		m_bCongestionMode = TRUE;

		// 폭주 상태를 알림
		DSTC_CONGESTION_MODE CongestionMode;
		CongestionMode.bOn = 1;
		CongestionMode.dwMonsterID = GetID();
		GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&CongestionMode, CongestionMode.GetPacketSize(), GetPrevSectionNum() );
	
		// 폭주 모드에선 이속 증가
		m_fMoveSpeed *= 1.5;
	}

	float fLength	= CalcDistance( GetCurPosition(), m_pUnitForAI->GetCurPosition());
	

	DWORD dwWaiting = 5000;
	if(m_bCongestionMode)
		dwWaiting -= 1500;

	// 시야 밖으로 도망 친다면
	if( fLength > m_fSight)
	{
		// 파이어 볼을 한방 먹여주고 무시한다
		SendSkillPacket(0, false);
		SetStatus(UNIT_STATUS_NORMAL);

		m_dwTemp[NPC_TEMP_LASTSKILLTICK] = g_dwTickCount;
		m_pUnitForAI = NULL;
		return ;
	}

//	if(g_dwTickCount - m_dwTemp[NPC_TEMP_LASTATTACKTICK] < (GetAttackSpeed() - (m_bCongestionMode?0:500)) )
	if(g_dwTickCount + (m_bCongestionMode?0:500) < GetAttackSpeed()+ m_dwTemp[NPC_TEMP_LASTATTACKTICK]  )
		return;

	if(g_dwTickCount < dwWaiting + m_dwTemp[NPC_TEMP_LASTSKILLTICK] )
		return;

	// 공격 범위 밖으로 도망친다면
	if( fLength > m_fAttackRange)
	{
		if(SendSkillPacket(0))
		{
			SetStatus(UNIT_STATUS_NORMAL);
			return;
		}

		// 모션 딜레이가 끝났다면
		if(g_dwTickCount - m_dwTemp[NPC_TEMP_LASTSKILLTICK] > dwWaiting)
		{
			if(PathFind())
			{
				SetStatus(UNIT_STATUS_WALKING);
			}
			else
			{
				// 무시하고 다른 놈을 찾는다
				SetStatus(UNIT_STATUS_NORMAL);
				m_pUnitForAI = NULL;
			}
		}

		return;
	}

	// 모션 딜레이가 끝났다면
	if(g_dwTickCount - m_dwTemp[NPC_TEMP_LASTSKILLTICK] > dwWaiting)
	{
		if(SendSkillPacket( ((rand() % 100) < 80) ? 1:0) )
			return;
		
		SendAttackPacket();
		m_dwTemp[NPC_TEMP_LASTATTACKTICK] = g_dwTickCount;
	}
}

BYTE CMonster::GetAI_FirstAttack() const
{
	return m_pBaseMonster->AI.bFirstAttack;
}

BYTE CMonster::GetAI_TargetChangeType() const
{
	return m_pBaseMonster->AI.bTargetChangeType;
}

float CMonster::GetAI_MinHPRateForCongestionMode() const
{
	return m_pBaseMonster->AI.fMinHPRateForCongestionMode;
}

float CMonster::GetAI_RunningRate() const
{
	return m_pBaseMonster->AI.fRunningRate;
}

DWORD CMonster::GetAI_MonsterIDForCongestionMode() const
{
	return m_pBaseMonster->AI.dwMonsterIDForCongestionMode;
}

WORD CMonster::GetAI_CongestionTime() const
{
	return m_pBaseMonster->AI.wCongestionTime;
}

void CMonster::CheckCongestionMode(BOOL bOnCheck)
{
	if(IsGuardian())	return;

	if(bOnCheck)	//ON 체크 
	{
		//폭주모드가 적용되는 몬스터 이고 폭주모드가 Off 상태라면 주변에 대장 몬스터가 있는지 검색한다. 
		//만약 주변에 대장몬스터가 발견되면 폭주모드 On!
		if(GetCurSection()->m_pPcList->GetCount() > 0)	//유저가 있을때만 폭주모드 발동 
		{
			if(GetAI_MonsterIDForCongestionMode() != 0 && !IsCongestionMode() )
			{
				//이 섹션안에 대장몬스터가 존재 한다면 폭주모드를 발동 시킨다. 
				if( GetCurSection()->FindMonster( GetAI_MonsterIDForCongestionMode()) )
				{
					m_dwTemp[NPC_TEMP_CONGESTION_MODE] = g_dwTickCount;	//폭주모드가 발동한 Tick을 입력 

					DSTC_CONGESTION_MODE packet;
					packet.bOn = TRUE;
					packet.dwMonsterID = GetID();
					GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), (BYTE)GetCurSection()->m_bSectionNum );
				}
			}
		}
	}
	else	//OFF 체크 
	{
		if(GetAI_MonsterIDForCongestionMode() && m_dwTemp[ NPC_TEMP_CONGESTION_MODE ] > 0)
		{
			DWORD dwSec;
			if(g_dwTickCount < m_dwTemp[ NPC_TEMP_CONGESTION_MODE ])		//4Byte 값 돌아간 경우 	
			{
				dwSec = 0xffffffff - m_dwTemp[ NPC_TEMP_CONGESTION_MODE ];
				dwSec += g_dwTickCount;
			}
			else
			{
				dwSec = g_dwTickCount - m_dwTemp[ NPC_TEMP_CONGESTION_MODE ];	//정상적인 경우 
			}

			if(dwSec >= DWORD(GetAI_CongestionTime() * 1000))	//폭주모드 시간이 다 된경우는 꺼준다.
			{
				m_dwTemp[ NPC_TEMP_CONGESTION_MODE ] = 0;				

				DSTC_CONGESTION_MODE packet;
				packet.bOn = FALSE;
				packet.dwMonsterID = GetID();
				GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), (BYTE)GetCurSection()->m_bSectionNum );
			}			
		}
	}
}

void CMonster::ReSearchPath()
{
	m_PathFinder.dwCurveNum = 0;
	SendStopPacket(SEND_MODE_BROADCAST_SECTION);
		
	// 잠시 쉬게 하자.
	SetStatus( UNIT_STATUS_DELAYTIME );
	m_dwTemp[NPC_TEMP_LASTSTATUSTICK]	= g_dwTickCount;
}

void CMonster::SpeedUp(float fRate)
{
	if(!m_bSpeedUp) {
		m_fMoveSpeed *= fRate;
		m_bSpeedUp = TRUE;
	}
}

void CMonster::SpeedNormal()
{
	if(m_bSpeedUp) {
		UpdateMoveSpeed();
		m_bSpeedUp = FALSE;
	}
}

void CMonster::Move(float fRunningRate)
{
	CMap*		pMap  = GetCurDungeonLayer()->GetMap();
	MAP_TILE*	pTile = 0;
	static int	iAITick = 0;	// 자연스러운 AI 구현을 위한 Tick : 최덕석 2005.1.11

	//디폴트 속도는 0.01초단 1Cm
	DWORD	dwCurTick	= g_dwTickCount;	// 현재의 시간을 구하고.
	float	dwMoveTime	= float(dwCurTick - GetLastMoveTick()) * (GetMoveSpeed()*fRunningRate);	// ms를 백분의 1초로 바꾼다.
	SetLastMoveTick(dwCurTick);
		
	//---------------------------------------------------------------//
	// 타켓이 없다면 타켓을 한번식 검사한다.
	if (m_pUnitForAI == NULL)
	{
		switch( GetRace() )
		{
		case OBJECT_TYPE_MONSTER:
			{
				// 드래곤이 아니라면.(드래곤은 도착했을때 찾지 않는다.)
				if (m_pBaseMonster->bRace != MONSTER_RANK_DRAGON)
				{
					// 몬수터이면 먼저 마인드 콘드롤이 걸려있는지 확인한다.
					if( !GetLord() )	// 주인이 있는 몬스터는 하지 않는다.
					{
						//선공몬스터 일때만 대상을 찾는다. 
						if(GetAI_FirstAttack() != AI_FIRST_ATTACK_NONE)		
						{
							if( FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )
							{
								// SetStatus( UNIT_STATUS_NORMAL, FALSE );		// ai 개선 : 최덕석 2005.1.11
								PathFind();	// 가다가 찾으면 타겟으로 이동 : 최덕석 2005.1.10
								// SetPathFindMove(UNIT_STATUS_WALKING);
								if(GetAI_RunningRate() > 1.0f)
									SetPathFindMove( UNIT_STATUS_RUNNING );
								else
									SetPathFindMove( UNIT_STATUS_WALKING );

							}
						}
					}
				}
			}
			break;

		case OBJECT_TYPE_GUARDIAN:
			{
				CUnit *lord = GetLord();
				if( lord )
				{
					if( FindObject(GetAIType(AI_KIND_SEARCHTYPE)) )
					{
						// SetStatus( UNIT_STATUS_NORMAL, FALSE );	// ai 개선 : 2005.1.11 duk9
						PathFind();	// 가다가 찾으면 타겟으로 이동 : 최덕석 2005.1.10
						SetPathFindMove(UNIT_STATUS_WALKING);
					}
				}
			}
			break;
		}		
	}
	else
	{
		float fAngle = CalcDistance(m_pUnitForAI->GetCurDirection(), &m_v2TargetAIDir);

		iAITick++;

		if (fAngle > 0.7 && (iAITick%20) == 1)	// 0.4은 방위각 30도 정도, 3마리중 1마리는 똑똑함 : 최덕석 2005.1.11
		{
			PathFind();
			if( !SetPathFindMove(UNIT_STATUS_WALKING) )
			{
				if( GetRace() == OBJECT_TYPE_GUARDIAN && m_dwTemp[NPC_TEMP_MOVE] == 1 )
				{
					m_dwTemp[NPC_TEMP_LASTSTATUSTICK]	= g_dwTickCount;
					SetStatus( UNIT_STATUS_DELAYTIME );
				}
			}
			m_v2TargetAIDir = *m_pUnitForAI->GetCurDirection();
		}
		
		m_v2TargetAIPos = *m_pUnitForAI->GetCurPosition();
	}
	
	// 도착 지점인지를 체크한다.
	if( EpsilonVector( &m_v2DestPos, GetCurPosition(), dwMoveTime ) )
	{
		SetCurPosition(&m_v2DestPos);

		if( !SetPathFindMove(UNIT_STATUS_NORMAL) )	// 길찾기가 다끝났군.
		{
			if( GetRace() == OBJECT_TYPE_GUARDIAN && m_dwTemp[NPC_TEMP_MOVE] == 1 )
			{
				m_dwTemp[NPC_TEMP_LASTSTATUSTICK] = g_dwTickCount;
				SetStatus( UNIT_STATUS_DELAYTIME );
			}
		}

		// 폭주모드 조건에 맞는다면 폭주모드를 적용시킨다.
 		CheckCongestionMode(TRUE);
	}
	else
	{
		// 아니면 계속 진행.
		VECTOR2 v2Start = *GetCurPosition();
		VECTOR2 v2Move = 
		{
			( GetCurDirection()->x * dwMoveTime ), 
			( GetCurDirection()->y * dwMoveTime )
		};		

		v2Start.x += v2Move.x;
		v2Start.y += v2Move.y;	
		
		pTile = pMap->GetTile( v2Start.x, v2Start.y );
		
		// 만약 갈수없는 타일이라면...
		if( !pTile )
		{
			ReSearchPath();
			return;
		}
		
		if( pTile->wAttr.uSection == 0 )
		{
			ReSearchPath();
			return;
		}

		if( ( GetCurMapTile()->wIndex_X != pTile->wIndex_X ) || ( GetCurMapTile()->wIndex_Z != pTile->wIndex_Z ) )
		{
//			BOOL bErrorTile = FALSE;
			
			if(!pTile || (pTile->pData != this && ((pTile->wAttr.uOccupied != TILE_EMPTY) || (pTile->pData != NULL)	) ))
			{
				// 이동한 타일이 못가는 타일이면.
				ReSearchPath();
				return;
			}
			
			/*m_PathFinder.dwCurveNum = 0;

			m_v2DestPos.x = TILE_X( pTile->wIndex_X );
			m_v2DestPos.y = TILE_Z( pTile->wIndex_Z );
			
			VECTOR2 v2Direct;
			VECTOR3_SUB_VECTOR3( &v2Direct, &m_v2DestPos, GetCurPosition() );
			Normalize( &v2Direct, &v2Direct );
			SetCurDirection(&v2Direct);
			SendStopPacket(SEND_MODE_BROADCAST_SECTION);
			
			// 잠시 쉬게 하자.
			SetStatus( UNIT_STATUS_DELAYTIME );
			m_dwTemp[NPC_TEMP_LASTSTATUSTICK]	= g_dwTickCount;*/
		}
		
		SetCurPosition(&v2Start);
	}
	
	// 섹션를 체크하고 만약 섹션이 바뀌었따면. 레이어에 알려서 처리한다.
	pTile = pMap->GetTile( GetCurPosition()->x, GetCurPosition()->y );
	
	assert(pTile);

	if( GetCurSection()->m_bSectionNum != pTile->wAttr.uSection )
	{
		int			i=0;
		BYTE		IntersectSection[MAX_SECTION_LINK_NUM]={0,};
		BYTE		oldSectionNum = (BYTE)GetCurSection()->m_bSectionNum;
		GetCurDungeonLayer()->RemoveMonsterSection( this );
		GetCurDungeonLayer()->InsertMonsterSection( this,  pTile->wAttr.uSection );
	
		DSTC_DISAPPEAR_MON	DisAppear;
		DisAppear.dwMonsterIndex	= GetID();

		GetCurDungeonLayer()->IntersectSection( GetPrevSectionNum(), oldSectionNum, IntersectSection );
		
		for( i=0; IntersectSection[i] != 0; i++ )
		{
			GetCurDungeonLayer()->BroadCastSectionMsgExceptLink((char*)&DisAppear, DisAppear.GetPacketSize()
				, IntersectSection[i] );
		}

		// 여기까지 왔다면 현재자신의 섹션과 이전의 섹션의 차집합을 구해 속하는 유저에게 메시지를 보내야한다.
		GetCurDungeonLayer()->IntersectSection(oldSectionNum, GetPrevSectionNum(), IntersectSection );
	
		DSTC_APPEAR_MON	AppearMon;
		MakeAppearMonser( &AppearMon, this );

		for( i=0; IntersectSection[i] != 0; i++ )
		{
			GetCurDungeonLayer()->BroadCastSectionMsgExceptLink( (char*)&AppearMon, AppearMon.GetPacketSize()
				, IntersectSection[i]);

			if( GetUnitStatus() == UNIT_STATUS_WALKING )
			{
				DSTC_MOVE_MON	MoveMon;
				MoveMon.fSpeed			= (GetMoveSpeed() * fRunningRate);
				MoveMon.dwMonsterIndex	= GetID();
				MoveMon.v2MoveStartPos	= *GetCurPosition();
				MoveMon.v2MoveStopPos	= m_v2DestPos;
				MoveMon.v2MoveDirection	= *GetCurDirection();
				MoveMon.bMoveType		= UNIT_STATUS_WALKING;

				GetCurDungeonLayer()->BroadCastSectionMsgExceptLink((char*)&MoveMon, MoveMon.GetPacketSize()
					, IntersectSection[i] );
			}
		}
	}
}


void CMonster::Attack()
{

}

void CMonster::Damage()
{

}

void CMonster::Delay()
{
	DWORD	dwTime = g_dwTickCount;
	if( dwTime - m_dwTemp[NPC_TEMP_LASTSTATUSTICK] 
		>= m_pBaseMonster->wResponseSpeed)
 	{
		SetStatus( UNIT_STATUS_NORMAL );
	}
}

void CMonster::SendAllStatus()
{
	CUser* pMasterUser = (CUser*)GetLord();
	if (!pMasterUser)
		return;

	DSTC_APPENDIX_GUARDIAN_INFO AppendixGuardian;
	AppendixGuardian.dwMonsterIndex = GetID();
	AppendixGuardian.wDex = (WORD)GetDex();
	AppendixGuardian.wEgo = (WORD)GetEgo();
	AppendixGuardian.wInt = (WORD)GetInt();
	AppendixGuardian.wStr = (WORD)GetStr();
	AppendixGuardian.wVit = (WORD)GetVit();
	AppendixGuardian.wStatPoint = GetNotUseStatPoint();
	AppendixGuardian.dwExp = GetExp();
	memcpy(AppendixGuardian.pwAttackDamage_L, GetPtrOfAttackDamageL(), sizeof(WORD)*2);
	AppendixGuardian.wAttackAcuracy = GetAttackAcuracy();
	AppendixGuardian.wAvoid = GetAvoid();
	AppendixGuardian.wFireResist = GetFireResist();
	AppendixGuardian.wIceResist = GetIceResist();
	AppendixGuardian.wLightResist = GetLightResist();
	AppendixGuardian.wPhyResist = GetPhyResist();
	AppendixGuardian.wPoiResist = GetPoiResist();
	AppendixGuardian.dwMaxHP = GetMaxHP();

	// 가디언 스킬 선택
	for(int i = 0; i < MAX_GUARDIAN_USE_SKILL; ++i)
	{
		AppendixGuardian.pSkill[i].bKind = m_MonsterSkill[i].bySkillKind;
		AppendixGuardian.pSkill[i].bLevel = m_MonsterSkill[i].bSkillLevel;
	}
	
	NetSendToUser( pMasterUser->GetConnectionIndex()
				, (char*)&AppendixGuardian
				, AppendixGuardian.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION 
				);

	//// 가디언 아이템 동기화 : 최덕석 2005.03.29
	CItem* pGuardianItem = &pMasterUser->m_pInv_Guardian[0];

	// 레벨 및 경험치 동기화
	pGuardianItem->m_Item_Guardian.dwCompleteTime = GetExp();

	// 스탯 동기화
	pGuardianItem->Guardian.wStat[0] = GetEgo();
	pGuardianItem->Guardian.wStat[1] = GetStr();
	pGuardianItem->Guardian.wStat[2] = GetInt();
	pGuardianItem->Guardian.wStat[3] = GetDex();
	pGuardianItem->Guardian.wStat[4] = GetVit();
	
	// 스킬 동기화
	// 아이텀 정보에 여유 공간이 없어서 지저분하게 사용
	WORD wSkillLevel = 0;
	for(i = 0;i < MAX_GUARDIAN_USE_SKILL; ++i)
	{
		wSkillLevel |= (WORD)(m_MonsterSkill[i].bSkillLevel/5) << 3 * i;
		pGuardianItem->Guardian.bSkill[i] = m_MonsterSkill[i].bySkillKind;
	}
	pGuardianItem->bLevelH = wSkillLevel >> 8;
	pGuardianItem->Guardian.bLevelL = wSkillLevel;

	::SendToUpdate_GuardianItem(pMasterUser->GetConnectionIndex(), pGuardianItem);
}

void CMonster::UpdateAllStatus()
{
	UpdateSight();
	UpdateAttackSpeed();
	m_fAttackRange	= m_pBaseMonster->wA_Range;
	UpdateMaxHP();
	UpdateMaxSP();
	UpdateMoveSpeed();
	UpdateAttackAcuracy();
	UpdateBlockRate();
	UpdateAvoid();
	UpdateAD();
	UpdateFireResist();
	UpdateIceResist();
	UpdatePhyResist();
	UpdateLightResist();
	UpdatePoiResist();
	UpdateBlockRate();
	SetResist();
}

DWORD CMonster::GetOriginalMaxHP() const
{
	return m_pBaseMonster->dwBaseHP + m_pBaseMonster->bLHP * GetLevel() + GetVit() * 5;
}

void CMonster::UpdateMaxHP()
{
	DWORD dwHP = 0;

	// baseHP + LHP * Level
	if (m_dwTemp[NPC_TEMP_DUNGEONGUARDIAN])
	{
		// 수호가디언hp * (40+INT(던전레벨/10))로 변경.
		dwHP =  GetOriginalMaxHP();
		dwHP *= (40 + GetCurDungeon()->GetDungeonDataEx()->GetLevel()/10 );
	}
	else
	{
		dwHP = GetOriginalMaxHP();
	}

	dwHP += (DWORD)GetAlphaStat(USER_MAXHP);
	dwHP += DWORD(dwHP*GetAlphaStat(USER_MAXHP_PERCENT)/100.f);

	m_dwMaxHP = dwHP;
}

void CMonster::UpdateMaxSP()
{
	// baseSP + LSP * Level
	// m_dwMaxMP = m_pBaseMonster->wBaseMP + m_pBaseMonster->bLMP * GetLevel();
	m_dwMaxMP = m_pBaseMonster->wBaseMP + m_pBaseMonster->bLMP * GetLevel() + GetInt() * 5;		// INT에 따른 MP 증가, 가디언은 MP가 없음!? : 최덕석 2005.1.13
}

void CMonster::UpdateMoveSpeed()
{
	// 몬스터가 아메리타트면
	if(m_dwAI == 9)
	{
		// 이동 속도는 테이블의 것
		m_fMoveSpeed = m_pBaseMonster->wMS;
	}
	else
	{
		float fMoveSpeed = (m_pBaseMonster->bLMS*GetLevel() + m_pBaseMonster->wMS) * 0.001f;
	
		fMoveSpeed += GetAlphaStat(USER_MOVESPEED);
		fMoveSpeed += fMoveSpeed*max(GetAlphaStat(USER_MOVESPEED_PERCENT), -100)/100.f;
	
		m_fMoveSpeed = fMoveSpeed;
	}

	m_bSpeedUp = FALSE;	// 가디언은 정상 속도로 움직여라, AI 부분 : 최덕석 2005.1.13
}

WORD CMonster::GetOriginalAttackAcuracy() const
{
	return WORD(m_pBaseMonster->dwBaseAA + m_pBaseMonster->bLAA * GetLevel() + GetDex() * 10);
}

WORD CMonster::GetOriginalAD_Min() const
{
	//-	baseAD + LAD * Level + (n * 10)
	return WORD(m_pBaseMonster->dwBaseAD_Min + m_pBaseMonster->wLAD.wMin * GetLevel() + GetStr() * 10);
}

WORD CMonster::GetOriginalAD_Max() const
{
	return WORD(m_pBaseMonster->dwBaseAD_Max + m_pBaseMonster->wLAD.wMax * GetLevel() + GetStr() * 10);
}

WORD CMonster::GetOriginalAvoid() const
{
	return WORD(m_pBaseMonster->wBaseDP + m_pBaseMonster->bLDP * GetLevel() + GetDex() * 10);
}

void CMonster::UpdateAttackAcuracy()
{
	// baseAA + LAA * Level
	m_wAttackAcuracy = GetOriginalAttackAcuracy();
	m_wAttackAcuracy = WORD(m_wAttackAcuracy + (m_wAttackAcuracy * GetAlphaStat(USER_ATTACKACURACY_PERCENT) / 100.f));
}

void CMonster::UpdateAD()
{
	// -	baseAD + LAD * Level + (n * 10)
	WORD wMinMax[2] = { GetOriginalAD_Min(), GetOriginalAD_Max()};
	
	wMinMax[0] = WORD(wMinMax[0] + GetAlphaStat(USER_ATTACK_R));
	wMinMax[1] = WORD(wMinMax[1] + GetAlphaStat(USER_ATTACK_R));
	
	wMinMax[0] = WORD(wMinMax[0] + wMinMax[0]*GetAlphaStat(USER_ATTACK_R_PERCENT)/100.f);
	wMinMax[1] = WORD(wMinMax[1] + wMinMax[1]*GetAlphaStat(USER_ATTACK_R_PERCENT)/100.f);

	SetAttackDamage_L(wMinMax[0], wMinMax[1]);
}

void CMonster::UpdateAvoid()
{
	// baseDP + LDP * Level
	m_wAvoid = GetOriginalAvoid();
	m_wAvoid = WORD(m_wAvoid + GetAlphaStat(USER_AVOID));
}

void CMonster::UpdateBlockRate()
{
	// baseBR + LBR * level / 100
	float fBlock = (float)m_pBaseMonster->bBaseBR 
					+ ( (float)m_pBaseMonster->bLBR * (float)GetLevel() / 100.f );
	m_wBlockRate = (BYTE)fBlock;
}

void CMonster::SetResist()
{
	UpdateFireResist();
	UpdateIceResist();
	UpdateLightResist();
	UpdatePoiResist();
	UpdatePhyResist();
}

void CMonster::UpdateSight()
{
	m_fSight = m_pBaseMonster->bSR * 125.0f;
	m_fSight += m_fSight*max(GetAlphaStat(USER_BLIND), -100)/100.f;
	m_fSight = max(m_fSight, TILE_SIZE);
}

void CMonster::UpdateFireResist()
{
	m_wFireResist = WORD(m_pBaseMonster->wFire_resist + ( ((float)m_pBaseMonster->bLRF * (float)GetLevel() + GetInt() * 10) / 100.f ));
	m_wFireResist += WORD(GetAlphaStat(USER_FIRE_RESIST ));
}

void CMonster::SetAllResist()
{
	m_wAll_resist = WORD(m_pBaseMonster->wAll_resist + ( ((float)m_pBaseMonster->bLRI * (float)GetLevel() + GetInt() * 10) / 100.f ));
	m_wAll_resist += WORD(GetAlphaStat(USER_ICE_RESIST ));
}

void CMonster::UpdateIceResist()
{
	m_wIceResist = WORD(m_pBaseMonster->wIce_resist + ( ((float)m_pBaseMonster->bLRI * (float)GetLevel() + GetInt() * 10) / 100.f ));
	m_wIceResist += WORD(GetAlphaStat(USER_ICE_RESIST));
}

void CMonster::UpdateLightResist()
{
	m_wLightResist = WORD(m_pBaseMonster->wLight_resit + ( ((float)m_pBaseMonster->bLRL * (float)GetLevel() + GetInt() * 10) / 100.f ));
	m_wLightResist += WORD(GetAlphaStat(USER_LIGHT_RESIST));
}

void CMonster::UpdatePoiResist()
{
	m_wPoiResist = WORD(m_pBaseMonster->wPoi_resist + ( ((float)m_pBaseMonster->bLRPo * (float)GetLevel() + GetInt() * 10) / 100.f ));
	m_wPoiResist += (WORD)GetAlphaStat(USER_POI_RESIST);
}

void CMonster::UpdatePhyResist()
{
	m_wPhyResist = WORD(m_pBaseMonster->wPhy_resist + ( ((float)m_pBaseMonster->bLRPh * (float)GetLevel() + GetInt() * 10) / 100.f ));
	m_wPhyResist += WORD(GetAlphaStat(USER_PHY_RESIST));
}

void CMonster::UpdateAttackSpeed()
{
	m_wAttackSpeed = m_pBaseMonster->wAW;
	m_wAttackSpeed += WORD(m_wAttackSpeed*GetAlphaStat(USER_ATTACKRATE_PERCENT)/100.f);

	if ((int)m_wAttackSpeed < 0)
		m_wAttackSpeed = 0;
}

void CMonster::Update()
{	
	//폭주모드가 꺼져야 하는조건인지를 체크해서 꺼져야 한다면 Off 시킨다. 
	CheckCongestionMode(FALSE);

	if( GetUnitStatus() == UNIT_STATUS_NORMAL && !m_dwTemp[ NPC_TEMP_AIFLAG ] ) 
	{
		// 현재 몬스터가 멍한 상태로 있다면 AI를 돌린다.
		(this->*AI)();
	}

	switch( GetUnitStatus() )
	{
	case UNIT_STATUS_RUNNING:	Move(GetAI_RunningRate());	break;
	case UNIT_STATUS_WALKING:	Move();						break;
	case UNIT_STATUS_ATTACKING:	Attack();					break;
	case UNIT_STATUS_DAMAGING:	Damage();					break;
	case UNIT_STATUS_DELAYTIME:	Delay();					break;
	}

	POSITION_ pos = GetUsingStatusEffectList()->GetHeadPosition();
	
	// Effect를 업데이트 한다.
	while( pos )
	{
		// 적용중인 이펙트를 찾아라.
		EffectDesc* pEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
		
		if (g_dwTickCount > pEffectDesc->dwDestTick)
		{			
			DetachSkill(pEffectDesc);
			pEffectDesc = NULL;
		}
		else
		{
			switch( pEffectDesc->pEffect->bID )
			{
			case __SKILL_POSIONCLOUD__:
			case __SKILL_POISONING__:				// 중독 처리.
				{
					if( g_dwTickCount - pEffectDesc->dwLastUpdateTick > 1000 )	// ㅂ초만다 한번씩.
					{
						int	dwCount = ( g_dwTickCount - pEffectDesc->dwLastUpdateTick ) / 1000;
						int	iValue = pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN]*dwCount;
						
						// 중독건놈에게도 경험치를 줘.
						CUser* pAttackUser = NULL;
						
						if( (int)GetHP()-iValue < 1 )
						{
							SetHP(1, pAttackUser);
							DetachSkill(pEffectDesc);
							pEffectDesc = NULL;
						}
						else
						{
							SetHP(GetHP()-iValue, pAttackUser);
							
							pEffectDesc->dwLastUpdateTick = g_dwTickCount;
						}
					}
				}
				break;
			}
		}		
	}
}


void CMonster::SendMovePacket()
{
	// 움직임 메시지 보내기.
	DSTC_MOVE_MON	MoveMon;
	MoveMon.fSpeed			= GetMoveSpeed();
	MoveMon.dwMonsterIndex	= GetID();
	MoveMon.v2MoveStartPos	= *GetCurPosition();
	MoveMon.v2MoveStopPos	= m_v2DestPos;
	MoveMon.v2MoveDirection	= *GetCurDirection();
	MoveMon.bMoveType		= UNIT_STATUS_WALKING;
	GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&MoveMon, MoveMon.GetPacketSize(), GetCurSection()->m_bSectionNum );

	SetStatus( UNIT_STATUS_WALKING );
	SetLastMoveTick(g_dwTickCount);
}

USER_PROPERTY_CODE CMonster::SetDamage(DWORD dwOffenseLevel, DWORD dwDamage )
{
	USER_PROPERTY_CODE eResult = USER_HP;

	if( !GetEffectDesc(__SKILL_AIREALCOAT__ ))	// 정상적으로 HP를 깍는다.
	{
		if( ( (int)GetHP() - (int)dwDamage ) <= 0 ) 
		{
			SetHP(0);
		}
		else
		{
			SetHP((GetHP() - dwDamage));
		}
	}
	else		// SP로 깍는다.
	{
		// HP깍는량 : 받을데미지 * (100 - 스킬.min) / 100
		EffectDesc* pEffectDesc = GetEffectDesc(__SKILL_AIREALCOAT__);
		BYTE bySkillLevel = pEffectDesc->bSkillLevel;
		
		float fDamage = dwDamage*(100-pEffectDesc->pEffect->Value[bySkillLevel].nMin) / 100.f;

		if( ( (int)GetHP() - (int)fDamage ) <= 0 )
		{
			SetHP(0);
		}
		else 
		{
			SetHP(DWORD(GetHP() - fDamage));
		}
		
		fDamage = (dwDamage-fDamage)*(pEffectDesc->pEffect->Value[bySkillLevel].nMax) / 100.f;
		
		if( ( (int)GetSP() - (int)fDamage ) <= 0 ) 
		{
			SetSP(0);
		}
		else 
		{
			SetSP(DWORD(GetSP()-fDamage));
		}

		if( !GetSP() )	// 스킬을 끈다.
		{
			DetachSkill( pEffectDesc );
		}

		eResult =  USER_MP;
	}

	return eResult;
}

void CMonster::SendRunningPacket()
{
//	Log( LOG_JUST_DISPLAY, "Move" );
	// 움직임 메시지 보내기.
	DSTC_MOVE_MON	MoveMon;
	MoveMon.fSpeed			= GetMoveSpeed() * GetAI_RunningRate();
	MoveMon.dwMonsterIndex	= GetID();
	MoveMon.v2MoveStartPos	= *GetCurPosition();
	MoveMon.v2MoveStopPos	= m_v2DestPos;
	MoveMon.v2MoveDirection	= *GetCurDirection();
	MoveMon.bMoveType		= UNIT_STATUS_RUNNING;
	GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&MoveMon, MoveMon.GetPacketSize(), GetCurSection()->m_bSectionNum );

	SetStatus( UNIT_STATUS_RUNNING );
	SetLastMoveTick(g_dwTickCount);
}

void CMonster::SendStopPacket(ENUM_SEND_MODE eSendMode) const
{
	assert(eSendMode == SEND_MODE_BROADCAST_SECTION);
	
	if (eSendMode == SEND_MODE_BROADCAST_SECTION)
	{
		DSTC_STOP_MON	StopMon;
		StopMon.dwMonsterIndex	= GetID();
		StopMon.v2StopPos		= *GetCurPosition();

		GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&StopMon
			, StopMon.GetPacketSize()
			, GetCurSection()->m_bSectionNum
		);
	}
}

extern void DieOnGuildWar(CDungeonLayer *pLayer, CUser* pDieUser);

void CMonster::SendAttackPacket()
{
	ATTACK_RESULT AttackResult;
	
	DSTC_ATTACK_MON_MON		AttackMonMon;
	DSTC_ATTACK_MON_USER	AttackMonUser;

	if( m_pUnitForAI == NULL )
		return;

	if( AttackResult_Unit_Unit( this, m_pUnitForAI, &AttackResult) == 0 ) 
		return;

	switch( m_pUnitForAI->GetObjectType() )
	{
	case OBJECT_TYPE_PLAYER:	// Mon->User
		{			
			CUser* pAIUser = (CUser*)m_pUnitForAI;
			if (pAIUser->m_enGodMode)	return;

//			goto lb_mon_user;

			// 2)공유 데미지
			DWORD	dwTotalPartyHP = 0;
			CUser*	pPartyUser[MAX_PARTY_USER] = {0,};	
			BYTE	bCount = 0;

			if( AttackResult.byType == 4 )
			{
				// 맞았을때.
				if (pAIUser->m_pUseMagicArrayDesc[ITEM_LIFE_LINK_SUBID])
				{
					// 데미지 분산을 시켜줘라.
					//1) 직접맞은놈이 받는 데미지 = X * ( 100 - point_1) / 100
					//2) 파티원이 공유하는 데미지 = X * point_1 / 100 * (자기HP) / (파티원HP합)
					
					CBaseItem*	pItem		= pAIUser->m_pUseMagicArrayDesc[ITEM_LIFE_LINK_SUBID]->pItem;
					CSection*	pSection	= GetCurDungeonLayer()->GetSection( GetPrevSectionNum() );
		
					if(!pItem)
						return;

					BOOL bSameSection;

					if( pSection )
					{						
						Node<CUser>* pNode = GetCurDungeon()->GetPartyUserHash()->GetBucketHead(pAIUser->m_dwPartyId);

						while(pNode)
						{
							bSameSection = FALSE;
							for( int i=0; pSection->m_wLink[i] != 0; i++ )
							{
								// 일정한 섹션범위에 같이 있을경우에만..
								if (GetPrevSectionNum() == pNode->pData->GetPrevSectionNum() ||
									pNode->pData->GetPrevSectionNum() == pSection->m_wLink[i])
								{
									bSameSection = TRUE;
									break;
								}
							}
							
							if (bSameSection)
							{
								dwTotalPartyHP+=pNode->pData->GetHP();
								pPartyUser[bCount++] = pNode->pData;

							}

							pNode = pNode->pNext;
						}
					}

					for(int i = 0; i < bCount; ++i)
					{
						DWORD dwShareDamage = DWORD(AttackResult.dwDamage * pItem->BaseItem_MagicArray.wPoint1 / 100.f * pPartyUser[i]->GetHP() / dwTotalPartyHP);
						BYTE bKind = BYTE(pPartyUser[i]->SetDamage(GetLevel(), dwShareDamage ));
						
						DSTC_USER_STATUS	UserStatus;
						UserStatus.bStatusMany	= 0;
													
						if( bKind == USER_HP )
						{
							UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
							UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pPartyUser[i]->GetHP();
						}
						else if(bKind == USER_MP )
						{
							UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
							UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pPartyUser[i]->GetSP();
							UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HP;
							UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pPartyUser[i]->GetHP();
						}
						
						GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&UserStatus, UserStatus.GetPacketSize(), GetCurSection()->m_bSectionNum );
					}

					// 1) 일단 받을껀 받고.
					AttackResult.dwDamage = DWORD(AttackResult.dwDamage * (100 - pItem->BaseItem_MagicArray.wPoint1) / 100.f);
				}
				
				AttackMonUser.bStatusKind = BYTE(pAIUser->SetDamage(GetLevel(), AttackResult.dwDamage ));

				CDungeon		*pDungeon = pAIUser->GetCurDungeon();
				CDungeonLayer	*pLayer = pAIUser->GetCurDungeonLayer();

				if(	!pAIUser->GetHP()// 죽었네.
				&&	pLayer//레이어가 있고
				&&	!pLayer->OnDie( (CUser*)GetLord(), pAIUser) )//길드전 사망처리가 죽이면 안된다고 한다. : hwoarang 050202
				{
					if(	pDungeon
					&&	pDungeon->GetDungeonDataEx()->IsStadium() //경기장이고
					&&	pLayer->m_pTeamMatch )//팀매치 매니저가 있으면
					{
						DieOnGuildWar( pLayer, (CUser*)pAIUser);
					}
				}

				if( AttackMonUser.bStatusKind == USER_HP )
				{
					AttackMonUser.dwCurUserHP = pAIUser->GetHP();
				}
				else if( AttackMonUser.bStatusKind == USER_MP )
				{
					// 으악 짜증..
					AttackMonUser.dwCurUserHP	= (pAIUser->GetHP()<<16) | pAIUser->GetSP();
				}

				if( !pAIUser->GetHP() )
				{//사망
					if(pAIUser->GetUnitStatus()!=UNIT_STATUS_DEAD)
					{
						// 합쳐야 하는데.
						CUser* pLordUser = (CUser*)GetLord();
						if(pLordUser)
						{							
							BOOL bChk = IsGuildWar(pLordUser, pAIUser);
/*													pLordUser->GetCurDungeon()
													, pLordUser->m_dwGuildId
													, pAIUser->m_dwGuildId
													, pLordUser->m_dwPartyId
													, pAIUser->m_dwPartyId
													, pAIUser->m_enGodMode);
*/							
							SetBadAction(pLordUser, pAIUser);
							
							if(bChk)
							{
								pLordUser->GuildMemberKill(pAIUser);
							}
						}
					}

					pAIUser->SetStatus(UNIT_STATUS_DEAD);
					pAIUser->SendItemSkill(ITEM_SKILL_DIE, OBJECT_TYPE_MONSTER, this);										
				}
				else
				{					
					pAIUser->SendItemSkill(ITEM_SKILL_DEFENSE, OBJECT_TYPE_MONSTER, this);
					bool bSend = false;
					
					if (pAIUser->GetItemAttr(ITEM_ATTR_DMG_REFLECT_POINT))
					{
						int nDamage = pAIUser->GetItemAttr(ITEM_ATTR_DMG_REFLECT_POINT);
						if (GetHP() > (DWORD)nDamage)	SetHP(GetHP() - nDamage);
						else SetHP(1);
						bSend = true;
					}

					if (pAIUser->GetItemAttr(ITEM_ATTR_DMG_REFLECT))
					{
						int nDamage = INT(AttackResult.dwDamage
										*pAIUser->GetItemAttr(ITEM_ATTR_DMG_REFLECT)/100.f);
						if (GetHP() > (DWORD)nDamage)	SetHP(GetHP() - nDamage);
						else SetHP(1);
						bSend = true;
					}

					if (bSend)
					{
						// 아이템에 의해 적용되어진 결과값을 보내줘라.
						DSTC_SKILL_RESULT packet;
						packet.bType = 1;	
						packet.wKind = ITEM_ATTR_DMG_REFLECT_POINT;
						packet.dwSkillResult[0] = GetID();
						packet.dwSkillResult[1] = GetHP();

						GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize()
							, GetCurSection()->m_bSectionNum );
					}
				}
			}

			AttackMonUser.dwMonsterIndex	= GetID();
			AttackMonUser.dwUserIndex		= pAIUser->GetID();
			AttackMonUser.v2MonsterPos		= *GetCurPosition();
			AttackMonUser.bType				= AttackResult.byType;
			AttackMonUser.dwDamage			= AttackResult.dwDamage;
			
			GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&AttackMonUser, AttackMonUser.GetPacketSize()
				, GetCurSection()->m_bSectionNum );
		}
		break;

	case OBJECT_TYPE_MONSTER:// Mon->Mon
		{
			// 합쳐야 해...
			CMonster* pAIMonster = (CMonster*)m_pUnitForAI;
			
//			goto lb_mon_mon;
			if( AttackResult.byType == 4 )
			{
				CUser* pLordUser			= (CUser*)GetLord();
				CUser* pAIMonsterByLordUser = (CUser*)pAIMonster->GetLord();

				if( pAIMonster->GetHP() < AttackResult.dwDamage )
				{
					pAIMonster->SetHP(0, pLordUser );
					if (pAIMonsterByLordUser && pLordUser)
					{
						SetBadAction(pLordUser, pAIMonsterByLordUser);						
					}
				}
				else
				{
					pAIMonster->SetHP(pAIMonster->GetHP() - AttackResult.dwDamage, pLordUser);
					
					if (pAIMonsterByLordUser && pLordUser)
					{
						SetBadAction(pLordUser, pAIMonsterByLordUser);						
					}
					
					//만약 이 몬스터가 비선공 몬스터이고 타겟없이 방황하고 있는넘이면 타겟을 셋팅해준다. 
					if(	pAIMonster->GetAI_FirstAttack() == AI_FIRST_ATTACK_NONE && !pAIMonster->m_pUnitForAI)
					{
						pAIMonster->ChangeTargetObject(this, AttackResult.dwDamage);
					}
				}				
			}

			AttackMonMon.dwOffenseMonIndex	= GetID();
			AttackMonMon.dwDefenseMonIndex	= pAIMonster->GetID();
			AttackMonMon.v2OffenseMonPos	= *GetCurPosition();
			AttackMonMon.v2DefenseMonPos	= *pAIMonster->GetCurPosition();
			AttackMonMon.bType				= AttackResult.byType;
			AttackMonMon.dwDamage			= AttackResult.dwDamage;
			AttackMonMon.dwCurDefenseMonHP	= pAIMonster->GetHP();
			
			GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&AttackMonMon, AttackMonMon.GetPacketSize()
				, GetCurSection()->m_bSectionNum );
			
			if ( AttackResult.byType == 4 && pAIMonster->GetHP() == 0)
			{
				UpdateMonsterForAI( pAIMonster );
				MonsterKillByMonster( this, pAIMonster );
			}			
			else
			{
				if (AttackResult.byType!=4 && pAIMonster->GetHP() == 0)
					Log(LOG_NORMAL, "똘똘이 몬스터 바보");
			}
		}
		break;

	default:
		return;
	}	
}

BOOL CMonster::IsBadAction(CUnit* pDefenseUnit)
{
	if (GetLord())
	{
		switch(pDefenseUnit->GetObjectType())
		{
		case OBJECT_TYPE_PLAYER:
			return GetLord()->IsBadAction((CUser*)pDefenseUnit);
		case OBJECT_TYPE_MONSTER:
			return GetLord()->IsBadAction((CMonster*)pDefenseUnit);
		}
	}

	return FALSE;
}


void CMonster::LevelUP()
{
	SetLevel(GetLevel()+1);
	UpdateAllStatus();
	SetSP(GetMaxSP());
	SetHP(GetMaxHP());
	SetStatPoint(WORD(GetNotUseStatPoint()+GetStatPointByLevel(GetObjectType(), GetLevel()-2)));
	SendAllStatus();

	///// 가디언 스탯 및 레벨업 시 생명연장 : 최덕석 2005.1.18
	// 가디언이고
	if (IsGuardian())
	{
		CUser* pMasterUser = (CUser*)GetLord();
		
		// 주인이 있다면
		if (pMasterUser)
		{
			CItem* pGuardianItem = &pMasterUser->m_pInv_Guardian[GetTemp(NPC_TEMP_SLOT)];
			
			// 가디언 수명을 연장
			::SendExchangeGuardianLife(pMasterUser, pGuardianItem->GetBaseItem()->BaseItem_Guardian.wLvLifeUp);
				
			// Client에게 알림, 위치 문제로 버그가 발생, 밑으로 내림
			DSTC_GUARDIAN_LEVELUP	LevelUP;
			LevelUP.dwGuardianIndex	= GetID();
			LevelUP.dwLevel			= GetLevel();
			GetCurDungeonLayer()->BroadCastSectionMsg((char*)&LevelUP, LevelUP.GetPacketSize(), GetPrevSectionNum());

			// 가디언 정보를 DB에 저장
			UpdateGuardianDB(this);
		}
	}
}


DWORD CMonster::GetTemp(MONSTER_TEMP eMonster_Temp) const
{
	return m_dwTemp[eMonster_Temp];
}


void CMonster::SetTemp(MONSTER_TEMP eMonster_Temp, DWORD dwValue)
{
	m_dwTemp[eMonster_Temp] = dwValue;
}


void CMonster::RemoveCurDungeonLayer(BOOL bSendDisapper)
{
	if (bSendDisapper)
	{
		DSTC_DISAPPEAR_MON	DisAppear;
		DisAppear.dwMonsterIndex	= GetID();
		GetCurDungeonLayer()->BroadCastSectionMsg( 
			(char*)&DisAppear
			, DisAppear.GetPacketSize()
			, GetPrevSectionNum() 
		);
	}

	GetCurDungeonLayer()->GetMap()->SetTileOccupied( 
		GetCurMapTile()->wIndex_X
		, GetCurMapTile()->wIndex_Z
		, TILE_EMPTY
		, this 
	);

	GetCurDungeonLayer()->RemoveMonsterSection( this );
	GetCurDungeonLayer()->m_pMonsterList->RemoveAt( m_pLayerListNode );
	
	m_pLayerListNode = NULL;
}

void CMonster::SetCurDungeonLayer(CDungeonLayer* const pDungeonLayer)
{
	CUnit::SetCurDungeonLayer(pDungeonLayer);
	if (m_pLayerListNode)
		pDungeonLayer->m_pMonsterList->RemoveAt(m_pLayerListNode);

	m_pLayerListNode = pDungeonLayer->m_pMonsterList->AddTail(this);	
}

void CMonster::SetBaseMonsterInfo(BASEMONSTER* pBaseMonster)
{
	m_pBaseMonster = pBaseMonster;
	memcpy(m_MonsterSkill, pBaseMonster->sSkill, sizeof(m_MonsterSkill));

	UpdateAllStatus();

	if (!m_pBaseMonster->bRace)
		SetRace(OBJECT_TYPE_TRADER_MONSTER);
	else if (m_pBaseMonster->bRace == OBJECT_TYPE_GUARDIAN)
		SetRace(OBJECT_TYPE_GUARDIAN);
	else
		SetRace(OBJECT_TYPE_MONSTER);

	SetClass(m_pBaseMonster->bClass);	

	SetMonsterAI( pBaseMonster->bAI );
	
	// 스킬이 몇개인지 저장해놓는다.
	for( int i=0; i<MAX_MONSTER_USE_SKILL; ++i )
	{
		if( m_MonsterSkill[i].bySkillKind != 0 )
		{
			++m_dwTemp[ NPC_TEMP_SKILLMANY ];
		}
	}
}

void CMonster::RemoveResponList()
{
	assert(m_pResponListPos);
	
	GetCurDungeonLayer()->m_pResponseMonsterList->RemoveAt( m_pResponListPos );
	m_pResponListPos = NULL;
}

void CMonster::AddResponList()
{
	m_pResponListPos = GetCurDungeonLayer()->m_pResponseMonsterList->AddTail( this );
}

void CMonster::SetCurSection(CSection* pSection)
{
	assert(pSection);

	if (m_pPrevSectionNode)
		RemoveCurSection();
	
	m_pPrevSectionNode = pSection->m_pMonsterList->AddTail( this );
	m_pCurSection = pSection;
	SetPrevSectionNum(pSection->m_bSectionNum);
}

void CMonster::RemoveCurSection()
{
	assert(GetCurSection());

	GetCurSection()->m_pMonsterList->RemoveAt( m_pPrevSectionNode );	//Section에 있는 PCList에서 유저 삭제
	m_pPrevSectionNode = NULL;
}

CSection* CMonster::GetCurSection() const
{
	return m_pCurSection;
}

void CMonster::SetDestPosition(const VECTOR2* v2Dest)
{
	m_v2DestPos = *v2Dest;
}

const VECTOR2* CMonster::GetDestPosition() const
{
	return &m_v2DestPos;
}

// 몬스터 스킬 발사
BOOL CMonster::SendSkillPacket(BYTE bSkillArrayIndex, BOOL bSkillTick)
{
	Effect*	pEffect=0;
	BYTE	bSkillLevel=0;
	BYTE	bSkillKind=0;

	if(m_pUnitForAI == NULL)
		return FALSE;

	// 스킬 사용 여부를 렌덤하게
	// 난수 값을 얻음
	int iRand = rand();
	
	if (bSkillTick)
	{		
		// 스킬을 사용할수 있는 상태인가.
		if( g_dwTickCount - m_dwTemp[NPC_TEMP_LASTSKILLTICK] 
			> m_pBaseMonster->wSkillRate - GetEgo() * 5)
		{
			m_dwTemp[NPC_TEMP_LASTSKILLTICK] = g_dwTickCount;
		}
		else 
		{
			return FALSE;
		}		
	}
	else 
	{
		m_dwTemp[NPC_TEMP_LASTSKILLTICK] = g_dwTickCount;
	}

	// 스킬을 사용할 확률과 난수를 비교
	if(m_pBaseMonster->wNonSkillRate > (iRand % 100))
	{
		return FALSE;
	}

	// 사용할 스킬의 종류 및 레벨을 구함
	bSkillKind	= m_MonsterSkill[ bSkillArrayIndex ].bySkillKind;
	bSkillLevel	= m_MonsterSkill[ bSkillArrayIndex ].bSkillLevel;

	// 사용할 스킬정보를 받아오고.
	pEffect = g_pEffectLayer->GetEffectInfo( bSkillKind );

	switch( m_pUnitForAI->GetObjectType() )
	{
	// 유저에게 스킬을 사요하면
	case OBJECT_TYPE_PLAYER:
		if (pEffect->bSkillTarget & TARGETTYPE_ENEMY_PLAYER
			|| pEffect->bSkillTarget & TARGETTYPE_FRIEND_PLAYER)
		{
			SkillToUser( bSkillKind, bSkillLevel, (CUser*)m_pUnitForAI );
		}
		else if (pEffect->bSkillTarget & TARGETTYPE_TILE)
		{
			SkillToTile( bSkillKind, bSkillLevel, m_pUnitForAI->GetCurMapTile() );
		}
		break;

	// 몹에게 스킬을 사용하면
	case OBJECT_TYPE_MONSTER:
		if (pEffect->bSkillTarget & TARGETTYPE_FRIEND_MONSTER
			|| pEffect->bSkillTarget & TARGETTYPE_ENEMY_MONSTER)
		{
			
			SkillToMonster( bSkillKind, bSkillLevel, (CMonster*)m_pUnitForAI, NULL );
		}
		else if (pEffect->bSkillTarget & TARGETTYPE_TILE)
		{
			SkillToTile( bSkillKind, bSkillLevel, m_pUnitForAI->GetCurMapTile());
		}
		break;
	}

	SetStatus(UNIT_STATUS_WALKING);
	SetStatusStop();

	return TRUE;
}

//
void CMonster::SkillToMonster(BYTE bSkillKind
							, BYTE bSkillLevel
							, const CMonster* pTargetMonster
							, const WORD* pwMinMax)
{
	m_SkillDesc.bOwnType			= OBJECT_TYPE_MONSTER;
	m_SkillDesc.bPK					= (GetLord()) ? ((CUser*)GetLord())->m_sPKDescInfo.m_bPKMode : 1;
	m_SkillDesc.bSectionNum			= this->GetPrevSectionNum();
	m_SkillDesc.bSkillKind			= bSkillKind;
	m_SkillDesc.bSkillLevel			= bSkillLevel;
	m_SkillDesc.bTargetType			= OBJECT_TYPE_MONSTER;
	m_SkillDesc.dwOwnIndex			= GetID();
	m_SkillDesc.dwSkillKeepTime		= g_pEffectLayer->GetEffectInfo(bSkillKind)->Value[bSkillLevel].nDuration;
	m_SkillDesc.dwTargetIndex		= pTargetMonster->GetID();
	m_SkillDesc.pDungeonLayer		= GetCurDungeonLayer();
	m_SkillDesc.v2OwnObjectPos		= *GetCurPosition();
	m_SkillDesc.wTileIndex_X		= WORD(pTargetMonster->GetCurPosition()->x/TILE_SIZE);
	m_SkillDesc.wTileIndex_Z		= WORD(pTargetMonster->GetCurPosition()->y/TILE_SIZE);
	m_SkillDesc.pMonsterMaster		= NULL;
	

	// 주어진 데미지 값이 있다면
	if(pwMinMax)
	{
		m_SkillDesc.wDamageMinMax[0]	= pwMinMax[0];
		m_SkillDesc.wDamageMinMax[1]	= pwMinMax[1];		
	}
	// 주어진 데미지 값이 없다면
	else
	{
		GetAttackSkillDamage(bSkillKind, bSkillLevel,&m_SkillDesc.wDamageMinMax[0], &m_SkillDesc.wDamageMinMax[1]);
	}
	
	g_pEffectLayer->SendSkill(&m_SkillDesc);
}

void CMonster::SkillToUser(BYTE bSkillKind
						 , BYTE bSkillLevel
						 , const CUser* pTargetUser)
{	
	m_SkillDesc.bOwnType		= OBJECT_TYPE_MONSTER;
	m_SkillDesc.bPK				= (GetLord()) ? ((CUser*)GetLord())->m_sPKDescInfo.m_bPKMode : 1;
	m_SkillDesc.bSectionNum		= this->GetPrevSectionNum();
	m_SkillDesc.bSkillKind		= bSkillKind;
	m_SkillDesc.bSkillLevel		= bSkillLevel;
	m_SkillDesc.bTargetType		= OBJECT_TYPE_PLAYER;
	m_SkillDesc.dwOwnIndex		= GetID();
	m_SkillDesc.dwSkillKeepTime	= g_pEffectLayer->GetEffectInfo(bSkillKind)->Value[bSkillLevel].nDuration;
	m_SkillDesc.dwTargetIndex	= pTargetUser->GetID();
	m_SkillDesc.pDungeonLayer	= GetCurDungeonLayer();
	m_SkillDesc.v2OwnObjectPos	= *GetCurPosition();
	m_SkillDesc.wTileIndex_X	= WORD(pTargetUser->GetCurPosition()->x/TILE_SIZE);
	m_SkillDesc.wTileIndex_Z	= WORD(pTargetUser->GetCurPosition()->y/TILE_SIZE);
	m_SkillDesc.pMonsterMaster	= NULL;
		
	GetAttackSkillDamage(bSkillKind, bSkillLevel,&m_SkillDesc.wDamageMinMax[0], &m_SkillDesc.wDamageMinMax[1]);

	g_pEffectLayer->SendSkill(&m_SkillDesc);
}

void CMonster::SkillToTile(BYTE bSkillKind
						 , BYTE bSkillLevel
						 , const MAP_TILE* pTile)
{	
	m_SkillDesc.bOwnType		= OBJECT_TYPE_MONSTER;
	m_SkillDesc.bPK				= (GetLord()) ? ((CUser*)GetLord())->m_sPKDescInfo.m_bPKMode : 1;
	m_SkillDesc.bSectionNum		= this->GetPrevSectionNum();
	m_SkillDesc.bSkillKind		= bSkillKind;
	m_SkillDesc.bSkillLevel		= bSkillLevel;
	m_SkillDesc.bTargetType		= OBJECT_TYPE_TILE;
	m_SkillDesc.dwOwnIndex		= GetID();
	m_SkillDesc.dwSkillKeepTime	= g_pEffectLayer->GetEffectInfo(bSkillKind)->Value[bSkillLevel].nDuration;
	m_SkillDesc.dwTargetIndex	= 0;
	m_SkillDesc.pDungeonLayer	= GetCurDungeonLayer();
	m_SkillDesc.v2OwnObjectPos	= *GetCurPosition();
	m_SkillDesc.wTileIndex_X	= pTile->wIndex_X;
	m_SkillDesc.wTileIndex_Z	= pTile->wIndex_Z;
	m_SkillDesc.pMonsterMaster	= NULL;

	GetAttackSkillDamage(bSkillKind, bSkillLevel,&m_SkillDesc.wDamageMinMax[0], &m_SkillDesc.wDamageMinMax[1]);

	g_pEffectLayer->SendSkill(&m_SkillDesc);
}

BOOL CMonster::AttachSkill(BYTE bOwnType
						 , DWORD dwOwnIndex
						 , BYTE bSkillKind
						 , BYTE bSkillLevel
						 , WORD wClass
						 , DWORD& dwDestTime
						 , DWORD& dwAlphaDamage)
{
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	EffectDesc* pEffectDesc = GetEffectDesc(bSkillKind);
	int nInsertNum = -1;
	switch(bSkillKind)
	{// 스킬 실패 요인들
	case __SKILL_CONFUSE__:
	case __SKILL_PRESSURE__:
	case __SKILL_CALMDOWN__:
	case __SKILL_SOULETER__:
	case __SKILL_SLEEP__:
		
	case __SKILL_MINDCONTROL__:
	case __SKILL_MINDEXPLOSION__:
		
		// 보스몹은 특수 스킬이 안먹힘
		if (m_pBaseMonster->bRace == MONSTER_RANK_DRAGON)
			return FALSE;
		else if (m_pBaseMonster->bRace == MONSTER_RACE_BOSS)
			return FALSE;

		//// 가디언은 슬립, 배쉬만 걸리도록 수정 : 최덕석 2005.3.25
		// 가디언 이이면서 슬립이나 배쉬가 아니면
		if ((GetLord() || GetRace() == OBJECT_TYPE_GUARDIAN)
		&& bSkillKind != __SKILL_SLEEP__ && bSkillKind != __SKILL_PRESSURE__)
		{
			// 걸리지 않음
			return FALSE;
		}

		if (bSkillKind == __SKILL_MINDCONTROL__ || bSkillKind == __SKILL_MINDEXPLOSION__)
		{
			if (bOwnType == OBJECT_TYPE_PLAYER)
			{
				CUser* pOwnUser = g_pUserHash->GetData(dwOwnIndex);
				if (!pOwnUser)	return FALSE;
				nInsertNum = pOwnUser->GetSubordinateIndex();
				
				if (nInsertNum	< 0)
					return FALSE;				
			}

			if(GetEffectDesc(__SKILL_PRESSURE__) || GetEffectDesc(__SKILL_CONFUSE__) 
				|| GetEffectDesc(__SKILL_CALMDOWN__) || GetEffectDesc(__SKILL_SOULETER__)
				|| GetEffectDesc(__SKILL_SLEEP__))
			{
				return FALSE;
			}
		}
		break;
	}
		
	if (!pEffectDesc)
	{// 새로 만들어라.
		
		pEffectDesc = AllocEffectDesc(bSkillKind, bSkillLevel);
		
		SetStatusFromSkillStatusValue(
			  bSkillKind
			, bSkillLevel
			, wClass
			, pEffectDesc->fParamPlus
			);	
		switch(bSkillKind)
		{
		case __SKILL_CALMDOWN__:			// 타켓을 버리고 공격을 받을때까지 타켓을 잡지않는다.
			{
				
				if (GetEffectDesc(__SKILL_CONFUSE__))
					DetachSkill(GetEffectDesc(__SKILL_CONFUSE__));
				else
				{
					// : 최덕석 2005.1.14
					ClearTargetAI();
					SetStatus( UNIT_STATUS_NORMAL );
					m_dwTemp[ NPC_TEMP_FINDUSER ] = 1;
				}
			}
			break;
		case __SKILL_SOULETER__:
		case __SKILL_SLEEP__:				// 어 나 잠들었네.
			SetStatus( UNIT_STATUS_NORMAL );
			m_dwTemp[ NPC_TEMP_AIFLAG ] |= NPC_AIFLAG_SLEEP;
			break;
		
		case __SKILL_PRESSURE__:
			SetStatus( UNIT_STATUS_NORMAL );
			m_dwTemp[ NPC_TEMP_AIFLAG ] |= NPC_AIFLAG_DEEPSLEEP;
			break;

		case __SKILL_CONFUSE__:
			{
				SetStatus( UNIT_STATUS_NORMAL, TRUE );
				m_dwTemp[NPC_TEMP_TARGET] = 1;						// 몬스터를 때려라.ㅋ ㅋ 
			}
			break;
		case __SKILL_REDELEMENTAL__:
		case __SKILL_BLUEELEMENTAL__:
		case __SKILL_GREENELEMENTAL__:
			{
				pEffectDesc->fParamPlus[0] = FLOAT(m_dwTemp[NPC_TEMP_SLOT]);
				pEffectDesc->pOwnUser = g_pUserHash->GetData(dwOwnIndex);
			}break;
		
		case __SKILL_MINDCONTROL__:
		case __SKILL_MINDEXPLOSION__:
			{
				pEffectDesc->fParamPlus[0] = FLOAT(nInsertNum);
				pEffectDesc->pOwnUser = g_pUserHash->GetData(dwOwnIndex);
				if (pEffectDesc->pOwnUser)
				{
					SetStatus( UNIT_STATUS_NORMAL, TRUE );
					SetLord(pEffectDesc->pOwnUser);	// 주인을 세팅하고.
					m_dwTemp[NPC_TEMP_SLOT]		= (DWORD)pEffectDesc->fParamPlus[0];
					m_dwTemp[ NPC_TEMP_PEACE ]	= 1;						// 주인의 커맨드를 기다리기 위해서.
					((CUser*)GetLord())->m_pMonster[ m_dwTemp[NPC_TEMP_SLOT] ] = this;
					SetMonsterAI( 4 );										// 가디언의 AI로 바꾸어 준다.

					// 여기서 해당 유저에게 마인드 컨트롤이 됐다고 알려야한다.
					DSTC_SKILL_RESULT		Result;
					Result.bType			= 2;
					Result.wKind			= __SKILL_MINDCONTROL__;
					Result.dwSkillResult[0] = 1;						// 설정.
					Result.dwSkillResult[1]	= GetID();					// 해당 몬스터의 아이디.
					Result.dwSkillResult[2] = GetLord()->GetID();	// 주인의 인덱스.
					Result.dwSkillResult[3] = m_dwTemp[NPC_TEMP_SLOT];	// 집코드.

					NetSendToUser( ((CUser*)GetLord())->m_dwConnectionIndex
						, (char*)&Result
						, Result.GetPacketSize()
						, FLAG_SEND_NOT_ENCRYPTION );
				}
				else
				{
					FreeEffectDesc(pEffectDesc);
					return FALSE;
				}
				
			}break;
		case __SKILL_HIDING__:			// 은신.
			UpdateMonsterForAI( this );
			break;
			
		case __SKILL_PRAY__:
			{
				// 나에게 걸린 나쁜스킬을 없애라.
				POSITION_ pos = GetUsingStatusEffectList()->GetHeadPosition();
				while(pos)
				{
					EffectDesc* pEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
					if (pEffectDesc->pEffect->bCrime == CRIME_APPLY_ENEMY)
					{
						//프레이 확률 * { 프레이레벨 * 2 / (프레이 레벨 + 걸려있는 스킬레벨) }
						if (g_pEffectLayer->IsSuccessByFormula0(pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel))
						{
							DetachSkill( pEffectDesc );
							break;
						}
					}
				}				
			}
			break;
		case __SKILL_SUMMONACCELERATOR__:
			{
				// 먼저 메시지를 보내고 reset한다.
				// 여기서 해당 유저에게 마인드 컨트롤이 됐다고 알려야한다.
				DSTC_SKILL_RESULT	Result;
				Result.bType		= 2;
				Result.wKind		= __SKILL_SUMMONACCELERATOR__;
				Result.dwSkillResult[0] = GetID();						// 해제.
				Result.dwSkillResult[1]	= GetMaxHP();					// 해당 몬스터의 MAX HP
				Result.dwSkillResult[2]	= GetHP();					// 해당 몬스터의 HP
				
				GetCurDungeonLayer()->BroadCastSectionMsg( 
					(char*)&Result
					, Result.GetPacketSize()
					, GetPrevSectionNum() );
			}break;
		case __SKILL_AMFLEAFIRE__:
			{
				POSITION_ pos = GetUsingStatusEffectList()->GetHeadPosition();
				while(pos)
				{
					EffectDesc* pEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
					if (pEffectDesc->pEffect->bType == TYPE_DRIVE)
					{
						DWORD dwTime = pEffectDesc->dwDestTick - g_dwTickCount;
						pEffectDesc->dwDestTick += DWORD(dwTime * GetAlphaStat(USER_OVERDRIVE_DELAY) / 100.f);
						pEffectDesc->dwAmfleafireTime = dwTime;
					}
				}
			}break;
		case __SKILL_DETONATION__:
			{
				if (bOwnType == OBJECT_TYPE_PLAYER)
				{				
					CUser* pUser = g_pUserHash->GetData(dwOwnIndex);
					pEffectDesc->pOwnUser = pUser;
					if (pUser)
					{
						if( pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON) == __SKILL_ATTACK__ )
						{
							pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN] 
								= pUser->GetAttackDamageMin_L();
							pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MAX] 
								= pUser->GetAttackDamageMax_L();							
						}
						else if( pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON) == __SKILL_ATTACK__ )
						{
							/*pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN] 
								= pUser->m_pwAttackDamage_L[0]*(100+g_pEffectLayer->GetFormula16(__SKILL_DETONATION__, pEffectDesc->bSkillLevel, pUser->GetClass()))/100.;
							pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MAX] 
								= pUser->m_pwAttackDamage_L[1]*(100+g_pEffectLayer->GetFormula16(__SKILL_DETONATION__, pEffectDesc->bSkillLevel, pUser->GetClass()))/100.;*/
							pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN] 
								= pUser->GetAttackDamageMin_R();
							pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MAX] 
								= pUser->GetAttackDamageMax_R();
						}						
					}
				}
				else if (bOwnType == OBJECT_TYPE_MONSTER)
				{				
					CMonster* pMonster = g_pMonsterTable->GetMonster(dwOwnIndex);
					if (pMonster)
					{
						pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN] 
							= DWORD(pMonster->GetAttackDamageMin_L()
								*(100+g_pEffectLayer->GetFormula16(__SKILL_DETONATION__
										, bSkillLevel, pMonster->GetClass()))/100.f);
						pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MAX] 
							= DWORD(pMonster->GetAttackDamageMax_L()
								*(100+g_pEffectLayer->GetFormula16(__SKILL_DETONATION__
										, bSkillLevel, pMonster->GetClass()))/100.f);												
					}
				}
			}break;
		}			
	}	

	if (pEffect->bType == TYPE_ACTIVE)
	{
		dwDestTime = pEffect->Value[pEffectDesc->bSkillLevel].nDuration;
		DetachSkill(pEffectDesc);
	}
	else if (pEffect->bType == TYPE_DRIVE)
	{		
		pEffectDesc->dwDestTick = g_dwTickCount+dwDestTime;
	}
	else if (pEffect->bType == TYPE_TIMEZERO)
	{
		dwDestTime = pEffect->Value[pEffectDesc->bSkillLevel].nDuration;
		pEffectDesc->dwDestTick = g_dwTickCount+dwDestTime;
	}
	
	return TRUE;
}

void CMonster::DetachSkill(EffectDesc* pEffectDesc)
{
	DSTC_DUNGEON_STOPSTATUSEFFECT packet;
	packet.bTargetType		= 1; // 몬스터 스킬 멈추라는 플래그.
	packet.bSkillKind		= pEffectDesc->pEffect->bID;
	packet.dwTargetIndex	= GetID();
	packet.dwCurHP			= GetHP();
	
	GetCurDungeonLayer()->BroadCastSectionMsg( 
		  (char*)&packet
		, packet.GetPacketSize()
		, GetPrevSectionNum() );
	
	switch( pEffectDesc->pEffect->bID )
	{
	case __SKILL_CALMDOWN__:				// 타켓을 버리고 공격을 받을때까지 타켓을 잡지않는다.
		{
			SetStatus( UNIT_STATUS_NORMAL );
			m_dwTemp[ NPC_TEMP_FINDUSER ] = 0;
		}
		break;

	case __SKILL_SOULETER__:
	case __SKILL_SLEEP__:				// 어 나 잠들었네.
		SetStatus( UNIT_STATUS_NORMAL );
		m_dwTemp[ NPC_TEMP_AIFLAG ] &= ~NPC_AIFLAG_SLEEP;
		break;
	
	case __SKILL_PRESSURE__:
		SetStatus( UNIT_STATUS_NORMAL );
		m_dwTemp[ NPC_TEMP_AIFLAG ] &= ~NPC_AIFLAG_DEEPSLEEP;
		break;

	case __SKILL_CONFUSE__:
		{
			SetStatus( UNIT_STATUS_NORMAL, TRUE );
			m_dwTemp[NPC_TEMP_TARGET] = 0;
		}
		break;

	case __SKILL_MINDEXPLOSION__:
	case __SKILL_MINDCONTROL__:			 				// 마인드 콘트롤.
		{
			// 먼저 메시지를 보내고 reset한다.
			// 여기서 해당 유저에게 마인드 컨트롤이 됐다고 알려야한다.
			DSTC_SKILL_RESULT		Result;
			Result.bType			= 2;
			Result.wKind			= __SKILL_MINDCONTROL__;
			Result.dwSkillResult[0] = 0;						// 해제.
			Result.dwSkillResult[1]	= GetID();					// 해당 몬스터의 아이디.
			Result.dwSkillResult[2] = GetLord()->GetID();	// 주인의 인덱스.
			Result.dwSkillResult[3] = m_dwTemp[NPC_TEMP_SLOT];	// 집코드.
			
			//NetSendToUser( GetLord()->m_dwConnectionIndex, (char*)&Result, Result.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
			GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&Result, Result.GetPacketSize(), GetPrevSectionNum() );

			SetStatus( UNIT_STATUS_NORMAL, TRUE );
			m_dwTemp[ NPC_TEMP_PEACE ]	= 0;			// 주인의 커맨드를 기다리기 위해서.
			SetMonsterAI( m_pBaseMonster->bAI );
			((CUser*)GetLord())->m_pMonster[ m_dwTemp[NPC_TEMP_SLOT] ] = NULL;
			SetLord(NULL);
			m_dwTemp[NPC_TEMP_SLOT]		= 0xffffffff;
		}
		break;

	case __SKILL_REDELEMENTAL__:// 몬스터 삭제하기.	
	case __SKILL_BLUEELEMENTAL__:
	case __SKILL_GREENELEMENTAL__:
		{
//			if (GetTemp(NPC_TEMP_SLOT) < MAX_USER_GUARDIAN)
//				((CUser*)GetLord())->m_pMonster[GetTemp(NPC_TEMP_SLOT)] = NULL;
			
			SetStatus( UNIT_STATUS_DEAD );
		}
		break;
	case __SKILL_AMFLEAFIRE__:
		{
			POSITION_  pos = GetUsingStatusEffectList()->GetHeadPosition();
			while(pos)
			{
				EffectDesc* pEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
				if (pEffectDesc->dwAmfleafireTime) // 증가한 시간을 다시 빼줘야지.
				{
					DWORD dwTime = pEffectDesc->dwAmfleafireTime;
					pEffectDesc->dwDestTick -= DWORD(dwTime * GetAlphaStat(USER_OVERDRIVE_DELAY) / 100.F);
					pEffectDesc->dwAmfleafireTime = 0;
				}
			}			
		}break;

	case __SKILL_SUMMONACCELERATOR__:
		{
			// 먼저 메시지를 보내고 reset한다.
			// 여기서 해당 유저에게 마인드 컨트롤이 됐다고 알려야한다.
			// sung-han 2005-01-12 가디언 HP가 갱신 안되는 것 해결, 계산을 안하고 값을 날려서 문제였슴
			//--------------------------------------------------------
			ReSetStatusFromSkillStatusValue(pEffectDesc);	
			//--------------------------------------------------------
			DSTC_SKILL_RESULT		Result;
			Result.bType			= 2;
			Result.wKind			= __SKILL_SUMMONACCELERATOR__;
			Result.dwSkillResult[0] = GetID();						// 해제.
			Result.dwSkillResult[1]	= GetMaxHP();					// 해당 몬스터의 MAX HP
			Result.dwSkillResult[2]	= GetHP();					// 해당 몬스터의  HP
			
			GetCurDungeonLayer()->BroadCastSectionMsg( 
				(char*)&Result
				, Result.GetPacketSize()
				, GetPrevSectionNum() );
		}break;

	case __SKILL_DETONATION__: // 폭팔 시켜라.
		{
			//SkillToMonster(__SKILL_DETONATION_MONSTER__, pEffectDesc->bSkillLevel, this);
			// 주인있는놈이라면 에너지와 상관없이 폭팔한다.
			// 에너지가 0이면 폭팔 될 가능성이 생긴다.
#ifdef DEV_MODE
			if (GetLord() || !GetHP())
#else
			if ( (GetLord() || !GetHP())
					&& g_pEffectLayer->IsSuccessByFormula0(pEffectDesc->pEffect->bID, pEffectDesc->bSkillLevel))
#endif
			{				
				WORD wMinMax[2]={0,};
				wMinMax[0] = WORD(pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN]
					+GetAttackDamageMin_L()*(1+(GetLord() != NULL)));
				wMinMax[1] = WORD(pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MAX]
					+GetAttackDamageMax_L()*(1+(GetLord() != NULL)));

				if (GetLord())
					((CUser*)GetLord())->m_pMonster[GetTemp(NPC_TEMP_SLOT)] = NULL;

				SetLord(pEffectDesc->pOwnUser);
				
				SkillToMonster(__SKILL_DETONATION_MONSTER__
					, pEffectDesc->bSkillLevel
					, this
					, wMinMax
				);

				DSTC_DISAPPEAR_MON	DisAppearMon;
				DisAppearMon.dwMonsterIndex = GetID();
				GetCurDungeonLayer()->BroadCastSectionMsg( (char*)&DisAppearMon
					, DisAppearMon.GetPacketSize()
					, GetCurSection()->m_bSectionNum 
				);
				
				UpdateMonsterForAI( this );

				SetStatus(UNIT_STATUS_DEAD);				
			}
		}break;
	}

	FreeEffectDesc(pEffectDesc);	
}

void CMonster::SetStatusFromSkillStatusValue(BYTE bSkillKind
										, BYTE bSkillLevel
										, WORD wClass
										, float* pResetValue
										, BOOL bUserStatusSend)
{
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	if ((char*)bSkillLevel<0)	return;	
	
	int nCount = 0;
	
	for (int i = 0; i < 5; ++i )
	{// 상태파라미터 변화 스킬.
		float fResult = 0;
		USER_PROPERTY_CODE wStatusID = (USER_PROPERTY_CODE)pEffect->sStatusValue[i].wStatusID;
		BYTE bFormula = pEffect->sStatusValue[i].bFormula;
		if (!wStatusID) break;
		
		switch(wStatusID)
		{
		case USER_MAXHP_PERCENT:
			{
				switch(bFormula)
				{
				case 16:
					fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
					break;
				}
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
				UpdateMaxHP();
			}break;
		case USER_MAXHP:
			switch(bFormula)
			{
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateMaxHP();
			break;
		case USER_ATTACKRATE_PERCENT:
			switch(bFormula)
			{
			case 19:
				fResult = FLOAT(g_pEffectLayer->GetFormula19(bSkillKind, bSkillLevel, wClass));
				
				break;
			case 4:
				fResult = g_pEffectLayer->GetFormula4(bSkillKind, bSkillLevel, wClass, GetAttackSpeed());
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateAttackSpeed();
			break;
		case USER_FIRE_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel, m_wFireResist);
				
				break;
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateFireResist();
			break;
		case USER_ICE_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel, m_wIceResist);
				
				break;
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateIceResist();
			break;
		case USER_LIGHT_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel, m_wLightResist);
				
				break;
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID) + fResult);
			UpdateLightResist();
			break;
		case USER_POI_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel, m_wPoiResist);
				
				break;
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdatePoiResist();
			break;
		case USER_PHY_RESIST:
			switch(bFormula)
			{
			case 15:
				fResult = g_pEffectLayer->GetFormula15(bSkillKind, bSkillLevel, m_wPhyResist);
				
				break;
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				
				break;
			case 24:
				fResult = FLOAT(g_pEffectLayer->GetFormula24(bSkillKind, bSkillLevel, wClass));
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdatePhyResist();
			break;
		case USER_MAX_RESIST:
			switch(bFormula)
			{
			case 23:
				fResult = g_pEffectLayer->GetFormula23(bSkillLevel);
				
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			break;
		case USER_MOVESPEED_PERCENT:
			{
				switch(bFormula)
				{
				case 16:
					fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
					break;
				}
				SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
				UpdateMoveSpeed();
			}break;
		case USER_MOVESPEED:
			switch(bFormula)
			{
				
			case 17:
				fResult = g_pEffectLayer->GetFormula17(bSkillKind, bSkillLevel, wClass);
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateMoveSpeed();
			break;
		case USER_ATTACK_R:
		case USER_ATTACK_R_PERCENT: // ad 를 올려야 하는데 일단 오른손 잡이로 가정하자.
			switch(bFormula)
			{
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateAD();
			break;
		case USER_HP_PERCENT:
			{
				switch(bFormula)
				{
				case 16:
					fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
					SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
					break;
					
				}
				WORD wHP = (WORD)GetHP();
				SetHP(DWORD(wHP*(1+fResult/100.)));
			}break;
		case USER_HP:
			switch(bFormula)
			{
			case 16:
				//m_fPlusParam[wStatusID] = 0;
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				//m_fPlusParam[wStatusID] += fResult;
				break;
				
			}
			SetHP(DWORD(GetHP()+fResult));
			break;

		case USER_ATTACKACURACY_PERCENT:// 정확도 변수가 없군.
			switch(bFormula)
			{
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateAttackAcuracy();
			break;
		case USER_AVOID:
			switch(bFormula)
			{
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateAvoid();
			break;
		case USER_BLIND:
			switch(bFormula)
			{
			case 16:
				fResult = FLOAT(g_pEffectLayer->GetFormula16(bSkillKind, bSkillLevel, wClass));
				break;
			}
			SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)+fResult);
			UpdateSight();
			break;
		case USER_DETECTION:
			{// 은신하고 있다면 은신시간을 남은시간을 갱신시켜 은신을 풀리게 해준다.
			}break;

		}

		if (pEffect->bType == TYPE_DRIVE || pEffect->bType == TYPE_TIMEZERO )
		{
			if (pResetValue)
				pResetValue[nCount++] = fResult;// active종류는 다시 지워질 필요 없기 때문에. 넣을 필요가 없다.
		}
	}	
}

void CMonster::ReSetStatusFromSkillStatusValue(EffectDesc* pEffectDesc)
{
	const Effect* pEffect = pEffectDesc->pEffect;
	BYTE bSkillLevel = pEffectDesc->bSkillLevel;
	if ((char*)bSkillLevel<0)	return;
//	BYTE bSkillKind = pEffect->bID;
	int nCount = 0;
	
	for (int i = 0; i < 5; ++i )
	{// 상태파라미터 변화 스킬.
		float fResult = 0.f;
		USER_PROPERTY_CODE wStatusID = (USER_PROPERTY_CODE)pEffect->sStatusValue[i].wStatusID;
//		BYTE bFormula = pEffect->sStatusValue[i].bFormula;
		if (!wStatusID)	break;
		SetAlphaStat(wStatusID, GetAlphaStat(wStatusID)-pEffectDesc->fParamPlus[i]);
		
		switch(wStatusID)
		{
		case USER_MAXHP_PERCENT:
		case USER_MAXHP:
			UpdateMaxHP();
			break;
		case USER_ATTACKRATE_PERCENT:
			UpdateAttackAcuracy();
			break;
		case USER_FIRE_RESIST:
			UpdateFireResist();
			break;
		case USER_ICE_RESIST:
			UpdateIceResist();
			break;
		case USER_LIGHT_RESIST:
			UpdateLightResist();
			break;
		case USER_POI_RESIST:
			UpdatePoiResist();
			break;
		case USER_PHY_RESIST:
			UpdatePhyResist();
			break;
		case USER_MOVESPEED_PERCENT:
		case USER_MOVESPEED:
			UpdateMoveSpeed();
			break;
		case USER_ATTACK_R_PERCENT:
		case USER_ATTACK_R: // ad 를 올려야 하는데 일단 오른손 잡이로 가정하자.
			UpdateAD();
			break;
		case USER_HP_PERCENT:
			
			SetHP(DWORD(GetHP()/(1+pEffectDesc->fParamPlus[i]/100.)));
			
			break;
		case USER_HP:
			if (GetHP() < pEffectDesc->fParamPlus[i])
				SetHP(0);
			else
				SetHP(DWORD(GetHP()-pEffectDesc->fParamPlus[i]));
			
			break;

		case USER_ATTACKACURACY_PERCENT:// 정확도 변수가 없군.
			UpdateAttackAcuracy();
			break;
		case USER_AVOID:
			UpdateAvoid();
			break;
		case USER_BLIND:
			UpdateSight();
			break;
		}

		if (pEffect->bType == TYPE_DRIVE || pEffect->bType == TYPE_TIMEZERO )
			pEffectDesc->fParamPlus[nCount++] = fResult;// active종류는 다시 지워질 필요 없기 때문에. 넣을 필요가 없다.		
	}
}

void* CMonster::operator new(size_t size)
{
	CMonster* pNew = (CMonster*)g_pMonsterClassPool->Allocation();
	pNew->SetObjectType(OBJECT_TYPE_MONSTER);
	pNew->SetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON, __SKILL_ATTACK__);
	return pNew;
}

void CMonster::operator delete(void* pDeleteMonster)
{
	g_pMonsterClassPool->FreeAllocation(pDeleteMonster);	
}

void CMonster::CreateResource()
{
	NewUsingStatusEffectList();
	m_pUnitForAI = NULL;

	AI_FindObject[0] = AI_FindObjectNear;
	AI_FindObject[1] = AI_FindObjectLeastHP;
	AI_FindObject[2] = AI_FindObjectNearAndFewHP;
	AI_FindObject[3] = AI_FindObjectAttachDetectAndNearAndFewHP;
	AI_DefenseType[0] = AI_DefenseNormal;
	AI_DefenseType[1] = AI_DefenseRecovery;
	AI_DefenseType[2] = AI_Defense_ChangeTargetToFierceObject;
}

void CMonster::RemoveResource()
{
	RemoveAllSkill();
}

void CMonster::Initialize(BASEMONSTER* pBaseMonster, GUARDIAN_INFO* pGuardian, CUser* pLord, BYTE byZipCode, DWORD dwLevel, DWORD dwExp)
{
	
	SetLevel(dwLevel);
	SetBaseMonsterInfo(pBaseMonster);
	AddExp(dwExp);
	SetSP(GetMaxSP());
	SetHP(GetMaxHP());
	SetLord(pLord);
	SetStatus(UNIT_STATUS_NORMAL, FALSE, TRUE);\

	// 주인이 있을때의 경우.
	if (pLord)
	{
		SetTemp(NPC_TEMP_SLOT, byZipCode);
		SetTemp(NPC_TEMP_PEACE, 1);			// 처음에는 평화 상태.
		SetTemp(NPC_TEMP_CREATETYPE, 1);

		// 엘레멘탈의 경우
		if (!pGuardian)
			SetInfoIndex( 0xffff );		// 몬스터가 죽었을시에 리스폰 몬스터가 아니라는 것을 말한다. 0xffff	
	}
	
	// 가디언일 경우.
	if (pGuardian)
	{
		// 주인이 없으면
		if (!pLord)
		{
			// 수호 가디언으로
			SetTemp(NPC_TEMP_DUNGEONGUARDIAN, 1);
		}

		// 기본 정보 설정
		SetGuardianDBID(pGuardian->dwID);
		SetName(pGuardian->szName);
		SetStatPoint(pGuardian->wStatPoint);
		SetEgo(pGuardian->wEgo);
		SetStr(pGuardian->wStr);
		SetInt(pGuardian->wInt);
		SetDex(pGuardian->wDex);
		SetVit(pGuardian->wVit);
		UpdateAllStatus();

		// 수호 가디언이면
		if (GetTemp(NPC_TEMP_DUNGEONGUARDIAN))
		{			
			// HP, SP 를 만땅으로 회복
			SetSP(GetMaxSP());
			SetHP(GetMaxHP());
		}
		// 일반 가디언이면
		else
		{
			// 주인에게 가디언 설정
			pLord->SetGuardian(byZipCode, this);
			
			// HP가 바닥이면
			if (!pGuardian->dwHP)	
			{
				SetHP(1);
			}
			else	
			{
				SetHP(pGuardian->dwHP);
			}

			// SP 복구 (가디언은 SP 안쓰지만 , 내일을 위해)
			SetSP(pGuardian->dwMP);
		}

		for(int i = 0; i < sizeof(pGuardian->monsterSkill)/sizeof(pGuardian->monsterSkill[0]); ++i)
		{
			if (pGuardian->monsterSkill[i].bySkillKind && pGuardian->monsterSkill[i].bSkillLevel)
				SetSkillLevel(pGuardian->monsterSkill[i].bySkillKind, pGuardian->monsterSkill[i].bSkillLevel);
		}
		
		// 가디언의 능력치를 맞추고 클라이언트에 알림
		SendAllStatus();
	}
}

BOOL CMonster::IsAlliance(const CUnit* pUnit)
{
	if (!pUnit)
		return FALSE;

	switch(pUnit->GetObjectType())
	{
	case OBJECT_TYPE_PLAYER:
		return IsAlliance((CUser*)pUnit);
	case OBJECT_TYPE_MONSTER:
		return IsAlliance((CMonster*)pUnit);
	}

	return FALSE;
}

BOOL CMonster::IsAlliance(const CMonster* pMonster)
{
	//때릴 수 있게.
	CDungeonLayer *pLayer = this->GetCurDungeonLayer();

	if( pLayer // 몬스터가 딴애 때릴 수 있다.: hwoarang 050218
	&&	pLayer->m_pTeamMatch)
	{
		return !pLayer->m_pTeamMatch->OnAttack( this->GetLord(), pMonster->GetLord() );
	}

	return (this == pMonster)
		|| (GetLord() == pMonster->GetLord() && pMonster->GetTemp(NPC_TEMP_TARGET) == 0)	// 같은 종족도 컨퓨즈 걸린 놈은 적으로 : 최덕석 2005.1.21
		|| (GetLord() 
			&& GetLord()->GetAttackMode() == ATTACK_MODE_DEFENCE 
			&& pMonster->IsGuardian() == GUARDIAN_TYPE_DUNGEON)
		|| (pMonster->GetLord()																// 수성 유저 소환물은 적으로 인식하지 않음 : 최덕석 2005.2.28
			&& pMonster->GetLord()->GetAttackMode() == ATTACK_MODE_DEFENCE)
		|| (pMonster->GetLord() && pMonster->GetLord() == this)
		|| (GetLord() && GetLord() == pMonster);

}

BOOL CMonster::IsAlliance(const CUser* pUser)
{
	CDungeonLayer *pLayer = this->GetCurDungeonLayer();

	if( pLayer // 몬스터가 딴애 때릴 수 있다.: hwoarang 050218
	&&	pLayer->m_pTeamMatch)
	{
		return !pLayer->m_pTeamMatch->OnAttack( this->GetLord(), pUser );
	}

	return (GetLord() 
			&& GetLord()->IsAlliance((CUnit*)pUser)) 
		|| (IsGuardian() == GUARDIAN_TYPE_DUNGEON 
			&& pUser->GetAttackMode() == ATTACK_MODE_DEFENCE);
}

BOOL CMonster::IsNormalMonster() const
{
	return m_dwTemp[ NPC_TEMP_CREATETYPE ] == 0 && GetRace() == OBJECT_TYPE_MONSTER;
}

BOOL CMonster::IsElemental(VOID) const 
{
	return GetRace() == OBJECT_TYPE_MONSTER && m_dwTemp[ NPC_TEMP_CREATETYPE ] && GetLord();	
}

BOOL CMonster::IsMindControll(VOID) const
{
	return IsNormalMonster() && (GetEffectDesc(__SKILL_MINDCONTROL__) || GetEffectDesc(__SKILL_MINDEXPLOSION__));	
}

BOOL CMonster::IsGuardian(VOID) const
{
	if (GetRace() == OBJECT_TYPE_GUARDIAN)
	{
		if (GetTemp(NPC_TEMP_DUNGEONGUARDIAN))
			return GUARDIAN_TYPE_DUNGEON;	// 수호가디언.
		else
			return GUARDIAN_TYPE_NORMAL;	// 주인있는 가디언
	}

	return FALSE;
}


void CMonster::SetHP(DWORD dwHP, const CUnit* pAttackUser)
{
	DWORD dwPrevHP = GetHP();
	m_dwHP = (min(dwHP, GetMaxHP()));

#ifdef JAPAN_LOCALIZING
	if(!m_dwHP)
	{
		if(GetRace() == OBJECT_TYPE_GUARDIAN)
		{
			CUser *pLord = (CUser *)GetLord();
			if(pLord)
			{
				CItem* pGuardianItem = &pLord->m_pInv_Guardian[GetTemp(NPC_TEMP_SLOT)];
				// 가디언의 수명 페널티를 적용
				SendExchangeGuardianLife(pLord, -pGuardianItem->GetBaseItem()->BaseItem_Guardian.wDyingPenalty);
				
			}
		}
	}
#endif

	if( pAttackUser && 
		pAttackUser->GetObjectType()==OBJECT_TYPE_MONSTER)
		pAttackUser = pAttackUser->GetLord();
		
	if( pAttackUser &&
		pAttackUser->GetObjectType()==OBJECT_TYPE_PLAYER &&
		pAttackUser->GetCurDungeon())
	{
		if (dwHP)
		{
			// 걸어 가고 있는데 누가 때렸을때..그놈 죽여~~		
			if (GetUnitStatus() == UNIT_STATUS_WALKING)
			{
				if( PathFind() )
				{
					SetStatusStop();
				}
				else 
				{
					SetStatus( UNIT_STATUS_NORMAL );
				}
			}
		}		
		
		if (IsGuardian() || IsElemental())
		{
			// 소환된 몬스터일경우데도 역시 경험치를 얻질 못한다.
			return;
		}

		DWORD dwExp = GetExpByMonster(this, dwPrevHP);

		if (dwExp)
		{
			::GiveMeExp((CUser*)pAttackUser, this, dwExp);
		}
	}
}

// 확률에 해당하는 아이템을 드롭시켜라.
int CMonster::GetDropItem(int nMagicItemRate, int iRate)
{
	int iItemID = 0;

	// 몬스터 아이템 주머니 부터 조사해랑
	for(int i = 0; i < 10; i++ )
	{
		if( iRate <= m_pBaseMonster->ItemRate[i].ItemRate * nMagicItemRate )
		{
			iItemID = m_pBaseMonster->ItemRate[i].ItemID;

			// 속주머니를 뒤져야 하나?
			if (iItemID > BASE_ITEMBOTARY_INDEX && iItemID < (BASE_ITEMBOTARY_INDEX+200))
			{
				iRate = rand();

				for(int j  = 0; j < 20; ++j)
				{
					if (iRate <= g_pBaseBotary[iItemID-BASE_ITEMBOTARY_INDEX].ItemRate[j].ItemRate)
					{
						return g_pBaseBotary[iItemID-BASE_ITEMBOTARY_INDEX].ItemRate[j].ItemID;					
					}
				}
			}

			break;
		}
	}
	
	return iItemID;
}

BOOL CMonster::IsNPC() const
{
	return m_pBaseMonster 
		&& (MONSTER_RACE_NPC == m_pBaseMonster->bRace);
}

CUnit* CMonster::GetUnitForAI() const
{
	return m_pUnitForAI;
}

void UpdateUserForAI( CUser* pUser )
{
	CSection*		pSection=0;
	CSection*		pLinkSection=0;
	CMonster*		pMonster=0;
	POSITION_		MonsterPos=0;
	CDungeonLayer*	pLayer=0;

	if( !pUser ) 
		return;

	else if (!pUser->GetCurDungeon())	
		return;

	pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) 
		return;

	pSection = pLayer->GetSection( pUser->GetPrevSectionNum() );
	if( !pSection ) 
		return;

	MonsterPos = pSection->m_pMonsterList->GetHeadPosition();
	
	while( MonsterPos )
	{
		pMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
		pMonster->DisAppearUnit( pUser->GetID() );
	}
	
	for( int i=0; pSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = pLayer->GetSection(pSection->m_wLink[i]);
		if( !pLinkSection ) continue;

		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		while( MonsterPos )
		{
			pMonster = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
			pMonster->DisAppearUnit( pUser->GetID() );
		}
	}
}

void UpdateMonsterForAI( CMonster* pMonster )
{
	CSection*	pSection=0;
	CSection*	pLinkSection=0;
	CMonster*	pMon=0;
	POSITION_	MonsterPos=0;
	
	if( !pMonster ) return;

	pSection	= pMonster->GetCurSection();
	if (!pSection)	return;
	
	MonsterPos	= pSection->m_pMonsterList->GetHeadPosition();	
	while( MonsterPos )
	{
		pMon = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
		pMon->DisAppearUnit( pMonster->GetID() );
	}
	
	for( int i=0; pSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = pMonster->GetCurDungeonLayer()->GetSection( pSection->m_wLink[i] );
		if( !pLinkSection ) continue;

		MonsterPos = pLinkSection->m_pMonsterList->GetHeadPosition();
		while( MonsterPos )
		{
			pMon = (CMonster*)pSection->m_pMonsterList->GetNext( MonsterPos );
			pMon->DisAppearUnit( pMonster->GetID() );
		}
	}
}

void CMonster::SetSkillLevel(BYTE bySkillKind, BYTE bySkillLevel)
{
	int nIndex = 0;
	for(int i = (IsGuardian()? MAX_GUARDIAN_USE_SKILL: MAX_MONSTER_USE_SKILL)-1; i >= 0; --i)
	{
		// 이미 배웟다면 그곳에 들어가기.
		if (m_MonsterSkill[i].bySkillKind == bySkillKind)
		{
			nIndex = i;
			break;
		}
		
		// 들어 갈수 있는 자리 알아내기.
		if (m_MonsterSkill[i].bySkillKind == 0)
		{
			nIndex = i;
		}
	}

	m_MonsterSkill[nIndex].bySkillKind = bySkillKind;
	m_MonsterSkill[nIndex].bSkillLevel = bySkillLevel;

	m_dwTemp[NPC_TEMP_SKILLMANY] = 0;
	for(i = 0; i < (IsGuardian()? MAX_GUARDIAN_USE_SKILL: MAX_MONSTER_USE_SKILL); ++i)
	{
		if (m_MonsterSkill[i].bSkillLevel != 0)
			m_dwTemp[NPC_TEMP_SKILLMANY]++;
	}
}

BYTE CMonster::GetSkillLevel(const BYTE bySkillKind) const
{
	for(int i = 0; i < (IsGuardian()? MAX_GUARDIAN_USE_SKILL: MAX_MONSTER_USE_SKILL); ++i)
	{
		if (m_MonsterSkill[i].bySkillKind == bySkillKind)
		{		
			return m_MonsterSkill[i].bSkillLevel;
		}
	}
	
	return 0;
}

const MONSTER_SKILL* CMonster::GetMonsterSkillPtr() const 
{
	return m_MonsterSkill;
}

bool CMonster::OnDie()
{
	DeathTime( ::timeGetTime() );//죽은시간 기록
	return true;
}

void  CMonster::GetAttackSkillDamage(BYTE bySelectedSkill, BYTE bSkillLevel, WORD* pwAttackDamageMin, WORD* pwAttackDamageMax) const
{
	// 물리 공격 데미지를 스킬 데미지로 적용 (에효...)
	*pwAttackDamageMin = GetAttackDamageMin_L();
	*pwAttackDamageMax = GetAttackDamageMax_L();

	//// 가뎅이 스킬 데미지 적용 : 최덕석
	// 신 가디언이면 (투두 : 신가디언 체크 함수 만들기) 
	if(GetRace() == OBJECT_TYPE_GUARDIAN && GetLord() && GetBaseMonsterInfo()->dwID <= 5080)
	{
		Effect* pEffect = g_pEffectLayer->GetEffectInfo(bySelectedSkill);

		// 물리형 스킬이면
		if(pEffect->wProperty%10 == SKILL_PROPERTY_KIND_PHY)
		{
			// MD = AD * {(1+ (SL*10 / 477)}
			*pwAttackDamageMin = GetAttackDamageMin_L() * (1.f + (bSkillLevel*10.f / 477.f));
			*pwAttackDamageMax = GetAttackDamageMax_L() * (1.f + (bSkillLevel*10.f / 477.f));
		}
		// 중독형 스킬이면
		else if(pEffect->wProperty%10 == SKILL_PROPERTY_KIND_POI)
		{
			// MD = Base * {(STR/100 + 1 + (SL*10 / 400))}
			*pwAttackDamageMin = (float)pEffect->Value[bSkillLevel].nMin * (GetStr()/100.0f + (1.f + bSkillLevel * 10.f / 400.f));
			*pwAttackDamageMax = (float)pEffect->Value[bSkillLevel].nMax * (GetStr()/100.0f + (1.f + bSkillLevel * 10.f / 400.f));
		}
		// 외의 스킬이면
		else
		{
			// MD = {Base + baseAD + ((LAD * Level)/1.5} + (EGO*10)}*(1+((SL*10)/350))
			const BASEMONSTER *bm = GetBaseMonsterInfo();
			
			*pwAttackDamageMin = pEffect->Value[bSkillLevel].nMin + bm->dwBaseAD_Min + (bm->wLAD.wMin * GetLevel())/1.5f + (GetEgo() * 10) * (1.f + (bSkillLevel*10.0f)/350.f);
			*pwAttackDamageMax = pEffect->Value[bSkillLevel].nMax + bm->dwBaseAD_Max + (bm->wLAD.wMax * GetLevel())/1.5f + (GetEgo() * 10) * (1.f + (bSkillLevel*10.0f)/350.f);
		}
	}

}

void CMonster::GetAttackDamageByFormula1(float& fDamageMin, float& fDamageMax, BYTE bSkillKind) const
{
	// 물리 데미지
	//minDMG = [(STR / 125 + WeaponDMG.min) * (100 + STR) / 100] * ClassAD / 2 * (100 + 웨폰마스터리.Skill.AD + 스킬.ad) / 100 + ITEM_ATTR_ATTACK_MIN + ITEM_ATTR_LEVEL_PER_DMG_MIN * char_level
	float fSkill_Min	= 100.0f;
	float fSkill_Max	= 100.0f;
	
	if( GetSkillLevel(__SKILL_WEAPONMASTERY__) != 0 )
	{
		fSkill_Min	+= (float)g_pEffectLayer->GetEffectInfo( __SKILL_WEAPONMASTERY__ )->Value[ GetSkillLevel(__SKILL_WEAPONMASTERY__)-1 ].nMin;
		fSkill_Max	+= (float)g_pEffectLayer->GetEffectInfo( __SKILL_WEAPONMASTERY__ )->Value[ GetSkillLevel(__SKILL_WEAPONMASTERY__)-1 ].nMax;
	}	
	
	fSkill_Min += GetAlphaStat( USER_ATTACK_R );
	fSkill_Max += GetAlphaStat( USER_ATTACK_R );
	fSkill_Min += fSkill_Min*GetAlphaStat(USER_ATTACK_R_PERCENT)/100.f;
	fSkill_Max += fSkill_Max*GetAlphaStat(USER_ATTACK_R_PERCENT)/100.f;


	float fClassRace		= float(g_pBaseClassInfo[GetClass()].iAD + g_pBaseRaceInfo[GetRace()].iAD);
	float fWeaponDamage_Min = 0.f;
	float fWeaponDamage_Max = 0.f;


	if( bSkillKind != 0 )		// 물리 스킬 데미지를 적용한다.
	{
		
		fSkill_Min	+= g_pEffectLayer->GetFormula16(bSkillKind, BYTE(GetSkillLevel(bSkillKind)-1), GetClass());
		fSkill_Max	+= g_pEffectLayer->GetFormula19(bSkillKind, BYTE(GetSkillLevel(bSkillKind)-1), GetClass());
	}

	fDamageMin = fWeaponDamage_Min * fClassRace / 2.f 
		* fSkill_Min / 100.0f+GetItemAttr(ITEM_ATTR_ATTACK_MIN) 
		+ GetItemAttr(ITEM_ATTR_LEVEL_PER_DMG_MIN) * GetLevel()
		+(2*GetLevel())*(100+GetStr())/100.f;
	fDamageMax = fWeaponDamage_Max * fClassRace / 2.f 
		* fSkill_Max / 100.0f+GetItemAttr(ITEM_ATTR_ATTACK_MAX) 
		+ GetItemAttr(ITEM_ATTR_LEVEL_PER_DMG_MAX) * GetLevel()
		+(2*GetLevel())*(100+GetStr())/100.f;

	DWORD dwSkillPassiveValueMin = 0 ;//GetSkillPassiveValueMin(bSkillKind);

	fDamageMin += fDamageMin*dwSkillPassiveValueMin/100.f;
	fDamageMax += fDamageMax*dwSkillPassiveValueMin/100.f;

	if (fDamageMin > fDamageMax)
		fDamageMin = fDamageMax-1;	
}


// 일단 사용하지 않음 만약의 사태 대비 김영대 205.02.19
void CMonster::GetAttackDamageByFormula2(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, BYTE bSkillLevel) const
{
	// 마법 데미지	
	//Skill.MAX * (1 + 스킬.마스터리* efficiency /2550) * (Max_스킬 + Race.MD)/100 * ( 100 + 익스플로젼.스킬.MD) / 100
	//Skill.Min * (1 + 스킬.마스터리* efficiency /2550) * (BaseClassSkill.Max + Race.MD)/100 * ( 100 + 익스플로젼.스킬.MD) / 100  * ( 100 + ITEM_ATTR_SKILL_DMG (속성) ) / 100 + ITEM_ATTR_ATTACK(속성).min*(1000 + ITEM_WEAPON.Mana_Damage)/1000
	
	if (!bSkillLevel)	return;

	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	float fManaMastery  = 1.f;
	float fSkill_Min	= 0.0f;
	float fSkill_Max	= 0.0f;
	
	DWORD	dwSkillPassiveDamage = 0;
	int		nClassRace = g_pEffectLayer->GetBaseClassSkillMax(GetClass(), bSkillKind);	
	
	// 스킬데미지를 위한 패시브 처리들.
	//dwSkillPassiveDamage = GetSkillPassiveValueMin(bSkillKind);

	// 클래스별 패널티
	switch( pEffect->wProperty / 100)
	{	
		case CLASS_TYPE_WIZARD:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Magic;
			break;
		case CLASS_TYPE_WARRIOR:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Aura;
			break;
		case CLASS_TYPE_HUNTER:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Chakra;
			break;
		case CLASS_TYPE_SUMMONER:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Summon;
			break;
		case CLASS_TYPE_PRIEST:
			nClassRace += g_pBaseRaceInfo[GetRace()].iMax_Divine;
			break;
		default:break;
	}
	
	fSkill_Min	= pEffect->Value[ bSkillLevel-1 ].nMin * fManaMastery; 
	fSkill_Max	= pEffect->Value[ bSkillLevel-1 ].nMax * fManaMastery;

	//fSkill_Min += fSkill_Min*(GetAlphaStat(USER_SKILLATTACK_L_PERCENT))/100.f;
	//fSkill_Max += fSkill_Max*(GetAlphaStat(USER_SKILLATTACK_L_PERCENT))/100.f;
	fSkill_Min += fSkill_Min*(GetAlphaStat(USER_SKILLATTACK_R_PERCENT))/100.f;
	fSkill_Max += fSkill_Min*(GetAlphaStat(USER_SKILLATTACK_R_PERCENT))/100.f;
	
	fDamageMin = fSkill_Min * nClassRace / 100.f * ( 100 + dwSkillPassiveDamage) / 100.f;
	fDamageMax = fSkill_Max * nClassRace / 100.f * ( 100 + dwSkillPassiveDamage) / 100.f;

	
	WORD wAttackSkillDamage = 0;
		
	fDamageMin += fDamageMin*wAttackSkillDamage/1000.f;
	fDamageMax += fDamageMax*wAttackSkillDamage/1000.f;

	fDamageMin = fSkill_Min * nClassRace / 100.f * ( 100 + dwSkillPassiveDamage) / 100.f*(1000+wAttackSkillDamage+(4*GetLevel()))/1000.f;
	fDamageMax = fSkill_Max * nClassRace / 100.f * ( 100 + dwSkillPassiveDamage) / 100.f*(1000+wAttackSkillDamage+(4*GetLevel()))/1000.f;
	
}


void CMonster::GetAttackDamageByFormula20(float& fDamageMin, float& fDamageMax, BYTE bSkillKind) const
{
	// 차크라 라이프 익스플러전을 위한 데미지	
	//SP * Min / 100 *BaseClassSkill.Max / 100  ~ SP * Max / 100 * BaseClassSkill.Max / 100  사이의 데미지를 줌
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	float fSkill_Min	= 0.0f;
	float fSkill_Max	= 0.0f;
	BYTE bSkillLevel = GetSkillLevel(bSkillKind);
	if (!bSkillLevel)	return;

	fSkill_Min	= (float)pEffect->Value[ bSkillLevel-1 ].nMin; 
	fSkill_Max	= (float)pEffect->Value[ bSkillLevel-1 ].nMax;

	fSkill_Min += fSkill_Min*GetAlphaStat( USER_SKILLATTACK_R_PERCENT );
	fSkill_Max += fSkill_Min*GetAlphaStat( USER_SKILLATTACK_R_PERCENT );

	fDamageMin = fSkill_Min / 100.f*g_pEffectLayer->GetBaseClassSkillMax(GetClass(), bSkillKind)/100.f;
	fDamageMax = fSkill_Max / 100.f*g_pEffectLayer->GetBaseClassSkillMax(GetClass(), bSkillKind)/100.f;


	//fDamageMin = wSP * fSkill_Min / 100.f*g_pEffectLayer->GetBaseClassSkillMax(GetClass(), bSkillKind)/100.f;
	//fDamageMax = wSP * fSkill_Max / 100.f*g_pEffectLayer->GetBaseClassSkillMax(GetClass(), bSkillKind)/100.f;
}


