#include "stdafx.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "user.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "DUNGEON_DATA_EX.h"
#include "Dungeon.h"
#include "OwnServer.h"
#include "Map.h"
#include "DungeonLayer.h"
#include "Section.h"
#include "MonsterTable.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "EventDungeonManager.h"


float __stdcall CalcDistance(const VECTOR2* const pv2Start, const VECTOR2* const pv2End)
{
	VECTOR3 v3Start = { pv2Start->x, 0, pv2Start->y };
	VECTOR3 v3End	= { pv2End->x, 0, pv2End->y };

	return CalcDistance(&v3Start, &v3End);
}

void __stdcall VECTOR3_SUB_VECTOR3(VECTOR2* const pv2Result
								 , const VECTOR2* const pv2Arg1
								 , const VECTOR2* const pv2Arg2)
{
	VECTOR3 v3Result	= {0, 0, 0};
	VECTOR3 v3Arg1		= {pv2Arg1->x, 0, pv2Arg1->y};
	VECTOR3 v3Arg2		= {pv2Arg2->x, 0, pv2Arg2->y};

	VECTOR3_SUB_VECTOR3(&v3Result, &v3Arg1, &v3Arg2);
	
	pv2Result->x = v3Result.x;
	pv2Result->y = v3Result.z;
}

void __stdcall Normalize (VECTOR2* OUT pv2Result,VECTOR2* IN pv2Arg1)
{
	VECTOR3 v3Result	= {0, 0, 0};
	VECTOR3 v3Arg1		= {pv2Arg1->x, 0, pv2Arg1->y};

	Normalize(&v3Result, &v3Arg1);
	pv2Result->x = v3Result.x;
	pv2Result->y = v3Result.z;
}

int	SkillResult_Unit_Unit(CUnit* pUser
						  , CUnit* pMonster
						  , BYTE bSkillKind
						  , BYTE bSkillLevel
						  , DWORD& dwTime
						  , WORD wAttackDamage[2]
						  , LPSKILL_RESULT pResult)
{
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	BOOL bSuccess = FALSE;
	BYTE bType = 2;

	if( pUser->GetEffectDesc(__SKILL_HIDING__) )
	{
		pUser->DetachSkill( pUser->GetEffectDesc( __SKILL_HIDING__ ));
	}
		
	switch(pEffect->bSuccessFormula)
	{
	case 0:
		{
			bSuccess = g_pEffectLayer->IsSuccessByFormula0(bSkillKind, bSkillLevel);
		}break;
	case 1:
		{
			bSuccess = g_pEffectLayer->IsSuccessByFormula1(pUser->GetAttackAcuracy()
						, pMonster->GetAvoid()
						, pMonster->GetBlockRate()
						, pUser->GetLevel()
						, pMonster->GetLevel()); 
		}break;
	case 2:
		{
			bSuccess = g_pEffectLayer->IsSuccessByFormula2(bSkillKind
						, bSkillLevel
						, pUser->GetLevel()
						, pMonster->GetLevel());
		}break;
	case 3:
		{
			bSuccess = g_pEffectLayer->IsSuccessByFormula1(pUser->GetAttackAcuracy()
						, pMonster->GetAvoid()
						, pMonster->GetBlockRate()
						, pUser->GetLevel()
						, pMonster->GetLevel()); 
		}break;
	case 4:
		{
			bSuccess = g_pEffectLayer->IsSuccessByFormula0(bSkillKind, bSkillLevel);

			float fPlusDP = 0;
			float fPlusBL = 0;
			if (bSuccess)
			{
				fPlusDP = g_pEffectLayer->GetFormula3(bSkillKind
							, bSkillLevel
							, pMonster->GetClass());
				fPlusBL = g_pEffectLayer->GetFormula3(bSkillKind
							, bSkillLevel
							, pMonster->GetClass());
			}

			bSuccess = g_pEffectLayer->IsSuccessByFormula1(pUser->GetAttackAcuracy()
				, WORD(pMonster->GetAvoid()+pMonster->GetAvoid()*fPlusDP)
				, WORD(pMonster->GetBlockRate()+pMonster->GetBlockRate()*fPlusBL)
				, pUser->GetLevel(), pMonster->GetLevel()); 
			
		}break;
	case 5:
		{
			bSuccess = g_pEffectLayer->IsSuccessByFormula5(bSkillKind
						, bSkillLevel
						, BYTE(pUser->GetSkillLevel(__SKILL_SUMMONMASTERY__)-1)
						, pUser->GetLevel()
						, pMonster->GetLevel());
		}break;
	case 6:
		{
			bSuccess = g_pEffectLayer->IsSuccessByFormula6(bSkillKind, bSkillLevel);
		}break;
	}

	if (bSuccess > 0)
	{
		// 기존에 사용중이던 스킬을 없애라.
		if( pMonster->GetEffectDesc(bSkillKind) )
		{
			pMonster->DetachSkill(pMonster->GetEffectDesc(bSkillKind));
		}
		
		DWORD dwAlphaDamage = 0;

		if (pMonster->AttachSkill(BYTE(pUser->GetObjectType())
				, pUser->GetID()
				, bSkillKind
				, bSkillLevel
				, pUser->GetClass()
				, dwTime
				, dwAlphaDamage))
		{	
			pResult->byType = 4;// 일단 성공	
			
			pResult->nDamage = rand()%( wAttackDamage[1] - wAttackDamage[0] + 1 );
			pResult->nDamage += wAttackDamage[0];

			if (pMonster->GetEffectDesc(bSkillKind))
			{
				EffectDesc* pEffectDesc = pMonster->GetEffectDesc(bSkillKind);
				pResult->nDamage += pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN]; // 풀데미지 
					//pMonster->GetEffectDesc(bSkillKind)->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN]; // 풀데미지
			}

			// 데토네이션이면서 주인있ㄴ놈은 데미지 입히지 말고 폭팔 걸어라.
			if (bSkillKind == __SKILL_DETONATION__ && pMonster->GetLord())
			{
				pResult->nDamage  = 0;
			}

			pResult->nDamage = pMonster->GetReduceDamageForObject(pResult->nDamage, pUser);
			
			// 그린엘리멘탈 슬립버그 임시로 막음 , 2005.02.21 김영대 
			if( bSkillKind != __SKILL_SLEEP__ && pResult->nDamage )
			{
				if (pMonster->GetEffectDesc(__SKILL_SLEEP__))
					pMonster->DetachSkill( pMonster->GetEffectDesc(__SKILL_SLEEP__ ));
				if ( pMonster->GetEffectDesc(__SKILL_SOULETER__) )
					pMonster->DetachSkill(pMonster->GetEffectDesc(__SKILL_SOULETER__));
				if ( pMonster->GetEffectDesc(__SKILL_CALMDOWN__) )
					pMonster->DetachSkill(pMonster->GetEffectDesc(__SKILL_CALMDOWN__));
			
			}

			pResult->nDamage = GetPropertyRealDamageByResist(BYTE(pEffect->wProperty%10)
								, pResult->nDamage
								, BYTE(pUser->GetObjectType())
								, pUser->GetID()
								, BYTE(pMonster->GetObjectType())
								, pMonster->GetID());
			
			if (bSkillKind == 	__SKILL_POISONING__	|| bSkillKind == __SKILL_POSIONCLOUD__)
			{
				EffectDesc* pEffectDesc = pMonster->GetEffectDesc(bSkillKind);

				pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN] = pResult->nDamage;
				pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_OWNER_TYPE] = pUser->GetObjectType();
				pEffectDesc->dwTemp[EFFECT_DESC_TEMP_POISON_OWNER_INDEX] = pUser->GetID();
			} 
			else if(bSkillKind == __SKILL_CALMDOWN__ || bSkillKind == __SKILL_HEAL__)
			{
				// 몹의 Agro에 영향을 주지 않는 Skill들은 여기로 와야함 : 최덕석 2005.1.14
			}
			else
			{
				if (pMonster->GetHP() < DWORD(pResult->nDamage)) 
				{
					pMonster->SetHP(0, pUser);
				}
				else
				{
					pMonster->SetHP(pMonster->GetHP() -pResult->nDamage, pUser); 
					pMonster->ChangeTargetObject(pUser, pResult->nDamage);
				}
			}

			return 1;
		}
		else
		{
			bType		= 2;
			bSuccess	= FALSE;
		}
	}
	else if (bSuccess  < 0)
	{
		bType		= 1;
		bSuccess	= FALSE;
	}
	
	pResult->byType = bType;	//miss
	pResult->nDamage = 0;

	return bSuccess;
}


int	SkillResult_System_Mon(DWORD dwMonsterMasterIndex, CMonster* pTargetMonster, BYTE bSkillKind, BYTE bSkillLevel
						   , DWORD& dwTime, WORD wMinMax[2], LPSKILL_RESULT pResult)
{
	DWORD dwAlphaDamage = 0;
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);

	if( (bSkillKind == __SKILL_MINDCONTROL__ || bSkillKind == __SKILL_MINDEXPLOSION__) &&
		(pTargetMonster->GetLord() || pTargetMonster->GetRace() == OBJECT_TYPE_GUARDIAN))
	{
		goto lbl_fail;
	}
	
	// 기존에 사용중이던 스킬을 없애라.
	if( pTargetMonster->GetEffectDesc(bSkillKind))
	{
		pTargetMonster->DetachSkill(pTargetMonster->GetEffectDesc(bSkillKind));
	}

	if ((bSkillKind == __SKILL_MINDCONTROL__ || bSkillKind == __SKILL_MINDEXPLOSION__) 
		? pTargetMonster->AttachSkill(OBJECT_TYPE_PLAYER
			, dwMonsterMasterIndex
			, bSkillKind
			, bSkillLevel
			, WORD(pEffect->wProperty/100)
			, dwTime
			, dwAlphaDamage)
		: pTargetMonster->AttachSkill(OBJECT_TYPE_SKILL
			, 0
			, bSkillKind
			, bSkillLevel
			, WORD(pEffect->wProperty/100)
			, dwTime
			, dwAlphaDamage))
	{
		pResult->byType = 4;// 일단 성공
		
		pResult->nDamage = rand()%( wMinMax[1] - wMinMax[0] + 1 );
		pResult->nDamage += wMinMax[0]; // 풀데미지

		
		if( pResult->nDamage)
		{
			if ( pTargetMonster->GetEffectDesc(__SKILL_SLEEP__ ) )
				pTargetMonster->DetachSkill( pTargetMonster->GetEffectDesc(__SKILL_SLEEP__ ) );
			if ( pTargetMonster->GetEffectDesc(__SKILL_SOULETER__) )
				pTargetMonster->DetachSkill(pTargetMonster->GetEffectDesc(__SKILL_SOULETER__));
			if ( pTargetMonster->GetEffectDesc(__SKILL_CALMDOWN__) )
				pTargetMonster->DetachSkill(pTargetMonster->GetEffectDesc(__SKILL_CALMDOWN__));
		}
	
		pResult->nDamage = GetPropertyRealDamageByResist(
							BYTE(pEffect->wProperty%10)
							, pResult->nDamage
							, OBJECT_TYPE_SKILL
							, 0
							, OBJECT_TYPE_MONSTER
							, pTargetMonster->GetID());

		if (bSkillKind == 	__SKILL_POISONING__
			|| bSkillKind == __SKILL_POSIONCLOUD__)
		{
			pTargetMonster->GetEffectDesc(bSkillKind)->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN] = pResult->nDamage;
			pTargetMonster->GetEffectDesc(bSkillKind)->dwTemp[EFFECT_DESC_TEMP_POISON_OWNER_TYPE] = OBJECT_TYPE_SKILL;
			pTargetMonster->GetEffectDesc(bSkillKind)->dwTemp[EFFECT_DESC_TEMP_POISON_OWNER_INDEX] = 0;
		}
		else
		{
			if (pTargetMonster->GetHP() < DWORD(pResult->nDamage)) pTargetMonster->SetHP(0);
			else pTargetMonster->SetHP(pTargetMonster->GetHP() -pResult->nDamage); 
		}
		return 1;
	}
	else
	{
		pResult->byType = 2;// 일단 성공
		goto lbl_fail;
	}
	

lbl_fail:
	return 0;
}

int	SkillResult_System_User(CUser* pTargetUser, BYTE bSkillKind, BYTE bSkillLevel, DWORD& dwTime, WORD wMinMax[2], LPSKILL_RESULT pResult)
{
	// 기존에 사용중이던 스킬을 없애라.
	if( pTargetUser->GetEffectDesc(bSkillKind))
		pTargetUser->DetachSkill(pTargetUser->GetEffectDesc(bSkillKind));
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);	
	
	DWORD dwAlphaDamage  = 0;
	pResult->byType  = 4;// 일단 성공
	pTargetUser->AttachSkill(OBJECT_TYPE_SKILL, 0, bSkillKind, bSkillLevel, WORD(pEffect->wProperty/100), dwTime, dwAlphaDamage);
						
	pResult->nDamage = rand()%( wMinMax[1] - wMinMax[0] + 1 );
	pResult->nDamage += wMinMax[0]+dwAlphaDamage; // 풀데미지
	
	pResult->nDamage = GetPropertyRealDamageByResist(BYTE(pEffect->wProperty%10)
						, pResult->nDamage
						, OBJECT_TYPE_SKILL
						, 0
						, OBJECT_TYPE_PLAYER
						, pTargetUser->GetID());
	
	if (bSkillKind == 	__SKILL_POISONING__
		|| bSkillKind == __SKILL_POSIONCLOUD__)
	{
		pTargetUser->GetEffectDesc(bSkillKind)->dwTemp[EFFECT_DESC_TEMP_POISON_DAMAGE_MIN] = pResult->nDamage;
		pTargetUser->GetEffectDesc(bSkillKind)->dwTemp[EFFECT_DESC_TEMP_POISON_OWNER_TYPE] = OBJECT_TYPE_SKILL;
		pTargetUser->GetEffectDesc(bSkillKind)->dwTemp[EFFECT_DESC_TEMP_POISON_OWNER_INDEX] = 0;
	}
	else
	{
		pTargetUser->SetDamage(0, pResult->nDamage);
	}
		
	return 1;	
}

//=======================================================================================================
int	GetExp(BYTE bUserLevel, BYTE bMonsterLevel, DWORD dwExp)
{
	int x = bMonsterLevel - bUserLevel;
	int nExp = 0;

	// 몬스터와 유저와의 레벨갭에 의한 경험치 획득치.
	if (x < -40)
	{
		nExp = (int)(dwExp * 0.1f);
	}
	else if(x < -25)
	{
		nExp = (int)(dwExp * 0.45f);
	}
	else if(x < -10)
	{
		nExp = (int)(dwExp * 0.75f);
	}
	else if(x < 10)
	{
		nExp = (int)(dwExp * 1.0f);
	}
	else if(x < 25)
	{
		nExp = (int)(dwExp * 1.45f);
	}
	else if(x < 40)
	{
		nExp = (int)(dwExp * 1.75f);
	}
	else
	{
		nExp = (int)(dwExp * 2.0f);
	}

#ifdef JAPAN_LOCALIZING
	if (x < -40)
	{
		nExp = (int)(dwExp * 0.1f);
	}
	else if(x < -25)
	{
		nExp = (int)(dwExp * 0.45f);
	}
	else if(x < -10)
	{
		nExp = (int)(dwExp * 0.75f);
	}
	else if(x < 10)
	{
		nExp = (int)(dwExp * 1.0f);
	}
	else if(x < 25)
	{
		nExp = (int)(dwExp * 0.75f);
	}
	else if(x < 40)
	{
		nExp = (int)(dwExp * 0.45f);
	}
	else
	{
		nExp = (int)(dwExp * 0.1f);
	}
#endif

	// 유저 패널티
	if (bUserLevel < 76)
	{
		nExp -= nExp*0;
	}
	else if(bUserLevel < 126)
	{
		nExp -= (int)(nExp*0.1f);
	}
	else if(bUserLevel < 176)
	{
		nExp -= (int)(nExp*0.3f);
	}
	else if(bUserLevel < 226)
	{
		nExp -= (int)(nExp*0.5f);
	}
	else
	{
		nExp -= (int)(nExp*0.75f);
	}
	
	if( nExp <= 0 ) nExp = 1;

	return nExp;
}


// 몬스터의 깍인 HP로 경험치를 계산하여 준다.
DWORD GetExpByMonster( CMonster* pMonster, DWORD dwPrevHP )
{
	if( !pMonster )
		return 0;
	if (!dwPrevHP)	
		return 0;
	if( pMonster->GetBaseMonsterInfo()->dwExp == 0 )
		return 0;

	DWORD dwHPCur, dwHPPrev, dwResult;

	// 20퍼센드의 경험치양과 HP를 구한다.
	DWORD dwHpPersent  = (DWORD)( (float)pMonster->GetMaxHP() * 0.20f );
	DWORD dwExpPersent = (DWORD)( (float)pMonster->GetBaseMonsterInfo()->dwExp * 0.20f );
	
	// HP의 위치를 구한다.
	if( pMonster->GetHP() != 0 )
		dwHPCur		= ( pMonster->GetHP() - 1 ) / dwHpPersent;	// 단계가 0이 안나오게 하기위해서 +1 
	else
		dwHPCur		= 0;

	dwHPPrev	= ( dwPrevHP - 1 ) / dwHpPersent;

	dwResult = dwHPPrev - dwHPCur;
	
	return dwResult * dwExpPersent;
}

void GiveMeExp(CUser* pUser, CMonster* pMonster, DWORD dwExp)
{
	if (!pUser->GetCurDungeon())	
		return;
	
	// 파티를 맺지 않았다면 
	if (!pUser->m_dwPartyId)
	{		
		dwExp = (DWORD)GetExp((BYTE)pUser->GetLevel(), (BYTE)pMonster->GetLevel(), dwExp);
		
		if( Event_GiveMeExp(pUser, dwExp) )
		{
			return;	
		}

		pUser->AddExp(dwExp);		
	}
	else
	{
		if( Event_GiveMeExp(pUser, dwExp) )
		{
			return;	
		}

		// 파티 경험치
		CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
		
		if( !pLayer ) return;
		
		CUser* pPartyUser[MAX_PARTY_USER] = { NULL, };
		DWORD dwPartyID		= pUser->m_dwPartyId;
		DWORD dwUserCount	= 0;
		DWORD dwTotalLevel	= 0;
		bool pClass[CLASS_TYPE_MAX] = {false, };

		// 같은 층의 모든 파티원을 구한다
		Node<CUser>* pnode = pUser->GetCurDungeon()->GetPartyUserHash()->GetBucketHead(dwPartyID);

		while( pnode )
		{
			if (pnode->pData->GetCurDungeon() && pnode->pData->GetUnitStatus() != UNIT_STATUS_DEAD)
			{
				CDungeonLayer* pTempLayer = pnode->pData->GetCurDungeonLayer();
				if( pTempLayer == pLayer && pnode->pData->m_dwPartyId == dwPartyID)
				{
					pPartyUser[dwUserCount] = pnode->pData;

					// 모든 클래스 참가, 추가 경험치 : 최덕석 2005.1.25
					pClass[pPartyUser[dwUserCount]->GetClass()] = true;
					
					dwTotalLevel += pnode->pData->GetLevel();
					dwUserCount++;
				}
			}
			
			pnode = pnode->pNext;
		}

		// 파티이나 파티원을 못찾을 경우는 기본으로 먹음
		if(dwUserCount == 0)
		{
			dwExp = (DWORD)GetExp((BYTE)pUser->GetLevel(), (BYTE)pMonster->GetLevel(), dwExp);

			if( Event_GiveMeExp(pUser, dwExp) )
			{
				return;	
			}

			pUser->AddExp(dwExp);
			
			return;
		}

		// 공통분배면
		if(pUser->m_PartyConfig.exp == PARTY_CONFIG_EXP_SHARE)
		{
			// 파티 평균랩을 기준으로 획득 경험치를 산출
			dwExp = (DWORD)GetExp(dwTotalLevel/dwUserCount, (BYTE)pMonster->GetLevel(), dwExp);
		}
		// 다른(레벨, 보너스) 분배면
		else
		{
			// 타격자 기준으로 경험치 산출
			dwExp = (DWORD)GetExp(pUser->GetLevel(), (BYTE)pMonster->GetLevel(), dwExp);
		}
		
		// 시너지스틱 트레이너를 사용중이면
		if (pUser->m_pUseMagicArrayDesc[ITEM_SYNERGISTIC_TRAINING])
		{
			//최종 몬스터 EXP = 원래 몬스터 EXP * (100 + point_1) / 100
			CBaseItem* pBaseItem =  pUser->m_pUseMagicArrayDesc[ITEM_SYNERGISTIC_TRAINING]->pItem;
			if (pBaseItem)
			{
				dwExp = DWORD(dwExp * ((100 + pBaseItem->BaseItem_MagicArray.wPoint1) / 100.f));
			}
		}

		///////////////////
		// 파티 경험치 변경 : 최덕석 2005.1.25
		float fResExp = 0.f;
		
		// 파티 추가 경험치
		fResExp += 0.09f * (dwUserCount - 1);

		// 모든 클래스 참가 추가 경험치
		for(int iClass=1;iClass<CLASS_TYPE_MAX;iClass++)
		{
			// 클래스가 하나라도 없다면
			if(pClass[iClass] == false)
				break;
		}
		if(iClass == CLASS_TYPE_MAX)
		{
			fResExp += 0.1f;
		}

		// 풀파티 추가 경험치
		if(dwUserCount == MAX_PARTY_USER)
		{
			fResExp += 0.2f;
		}

		for(DWORD i = 0; i < dwUserCount; ++i)
		{
			DWORD dwPersonalExp = 0;
			
			switch(pUser->m_PartyConfig.exp)
			{
			case PARTY_CONFIG_EXP_LEVEL:	// 레벨별 분배
				dwPersonalExp = ((float)dwExp * (1.f + fResExp)) * ((float)pPartyUser[i]->GetLevel()/(float)dwTotalLevel);
				break;
			case PARTY_CONFIG_EXP_SHARE:	// 균등 분배
				dwPersonalExp = ((float)dwExp * (1.f + fResExp)) / dwUserCount;
				break;
			case PARTY_CONFIG_EXP_KILLER:	// 보너스만 분배
				// 공격한 대상이면
				if(pUser == pPartyUser[i])
				{
					dwPersonalExp = (float)dwExp + ((float)dwExp * fResExp) / dwUserCount;
				}
				// 파티원이면
				else
				{
					dwPersonalExp = ((float)dwExp * fResExp) / dwUserCount;
				}
				break;
			}

			pPartyUser[i]->AddExp(dwPersonalExp);
		}
	}
	
	// 대상이 길드원이면
	if (pUser->m_dwGuildId)
	{
		// 길드에도 경험치를 부여
		DSTWS_DUNGEON_GROUPEXP packet;
		packet.bType = 2;
		packet.dwOwnUserIndex	= pUser->GetID();
		packet.dwExp			= dwExp;
		packet.dwMonsterLevel = pMonster->GetLevel();
		g_pNet->SendToServer(WSINDEX, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}


int InsertDamageTargetObject(PARAM_TARGETCOUNT* pParam, BYTE byCount, BYTE byObjectType, void* pObject)
{
	CMap*		pMap = pParam->pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( pParam->vecStart->x, pParam->vecStart->y );
	if (!pStartingPointTile)	return 0;

	CSection* pStartingPointSection = pParam->pDungeonLayer->GetSection(pStartingPointTile->wAttr.uSection);
	if (!pStartingPointSection)	return 0;

	BOOL bAdd = true;
	BOOL bInSection = false;
	
	for(int k = 0; k < byCount; ++k)
	{
		// 이미 추가 된놈은 또 추가 되면 안되기에..
		if (pParam->pTargetInfo[k].pUnit == pObject )
		{
			bAdd = false;
			break;
		}
	}

	BYTE bPrevSectionNum = 0xff;
	if (byObjectType == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = (CMonster*)pObject ;
		if (!pMonster->GetHP()
			|| pMonster->GetUnitStatus() == UNIT_STATUS_DEAD)	return 0;
		bPrevSectionNum = pMonster->GetPrevSectionNum();
	}
	else if (byObjectType == OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = (CUser*)pObject ;
		if (pUser->GetCurLayerIndex() == INVALID_DUNGEON_LAYER)	return 0;
		bPrevSectionNum = pUser->GetPrevSectionNum();
	}

	bInSection = 
		IsLinkSection(pParam->pDungeonLayer, pStartingPointSection, pParam->pDungeonLayer->GetSection(bPrevSectionNum));
	
	if (bAdd && bInSection)
	{
		// 리스트인덱스에 추가하라.
		pParam->pTargetInfo[byCount].pUnit = (CUnit*)pObject;
		pParam->pTargetInfo[byCount].dwTargetType = byObjectType;
		return 1;
	}

	return 0;
	
}


int IsCollitionByCircle(CDungeonLayer* pDungeonLayer, VECTOR2* v2Start, const VECTOR2* const v2Object, BYTE byRadius)
{
	CMap*		pMap = pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( v2Start->x, v2Start->y );
	if (!pStartingPointTile)	return 0;

	MAP_TILE* pObjectPointTile = pMap->GetTile( v2Object->x, v2Object->y);
	if (!pObjectPointTile)	return 0;

	// 원이므로 한방향만 조사해도 상관없어.
	float fRadian = (float)atan2(v2Object->y-v2Start->y, v2Object->x-v2Start->x);
	fRadian = (float)fabs(fabs(fRadian)-abs(long(fRadian/1.57f))*1.57f);

	pObjectPointTile = pMap->GetTile( v2Object->x, v2Object->y);
	if (!pObjectPointTile)	return 0;

	if (fabs(cosf(fRadian)*byRadius) >= fabs(cosf(fRadian)*(pObjectPointTile->wIndex_X-pStartingPointTile->wIndex_X)))
	{
		if (fabs(sinf(fRadian)*byRadius) >= fabs(sinf(fRadian)*(pObjectPointTile->wIndex_Z-pStartingPointTile->wIndex_Z)))
		{
			return 1;
		}
	}
	
	return 0;
}


int IsCollitionByBox(CDungeonLayer* pDungeonLayer, VECTOR2* v2Start, const VECTOR2* const v2Object, BYTE byRadius)
{
	CMap*		pMap = pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( v2Start->x, v2Start->y );
	if (!pStartingPointTile)	return 0;

	MAP_TILE* pObjectPointTile = pMap->GetTile( v2Object->x, v2Object->y);
	if (!pObjectPointTile)	return 0;

	// 박스의 가로세로를 계산하면 된다.
	float fRadian = (float)atan2(pObjectPointTile->z-pStartingPointTile->z, pObjectPointTile->x-pStartingPointTile->x);
	
	pObjectPointTile = pMap->GetTile( v2Object->x, v2Object->y);
	if (!pObjectPointTile)	return 0;

	if (byRadius >= fabs(cosf(fRadian)*(pObjectPointTile->wIndex_X-pStartingPointTile->wIndex_X)))
	{
		if (byRadius >= fabs(sinf(fRadian)*(pObjectPointTile->wIndex_Z-pStartingPointTile->wIndex_Z)))
		{
			return 1;
		}
	}

	return 0;
}

int IsCollitionByArc(CDungeonLayer* pDungeonLayer, VECTOR2* v2Start, float fRad, const VECTOR2* const v2Object, BYTE byRadius)
{
	CMap*		pMap = pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( v2Start->x, v2Start->y );

	if (!pStartingPointTile)	return 0;

	MAP_TILE* pObjectPointTile = pMap->GetTile( v2Object->x, v2Object->y);
	if (!pObjectPointTile)		return 0;

	// 원이므로 한방향만 조사해도 상관없어.
	float fRadian = (float)atan2((float)pObjectPointTile->wIndex_Z-pStartingPointTile->wIndex_Z, (float)pObjectPointTile->wIndex_X-pStartingPointTile->wIndex_X);
	
	if (fabs(cosf(fRadian)*byRadius) >= fabs(cosf(fRadian)*(pObjectPointTile->wIndex_X-pStartingPointTile->wIndex_X)))
	{
		if (fabs(sinf(fRadian)*byRadius) >= fabs(sinf(fRadian)*(pObjectPointTile->wIndex_Z-pStartingPointTile->wIndex_Z)))
		{
			if (fRad+.7859 > fRadian && fRad-.7859 < fRadian)
				return 1;
		}
	}

	return 0;
}

int IsCollitionByLine(CDungeonLayer* pDungeonLayer, VECTOR2* v2Start, float fRad, const VECTOR2* const v2Object, BYTE byRadius)
{
	CMap*		pMap = pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( v2Start->x, v2Start->y );

	if (!pStartingPointTile)	return 0;

	MAP_TILE* pObjectPointTile = pMap->GetTile( v2Object->x, v2Object->y);

	if (!pObjectPointTile)		return 0;
	
	// 나가는 방향
	float fRadian = (float)atan2(pObjectPointTile->z-pStartingPointTile->z, pObjectPointTile->x-pStartingPointTile->x); 

	if (fabs(cosf(fRad)*byRadius) >= fabs(cosf(fRadian)*(pObjectPointTile->wIndex_X-pStartingPointTile->wIndex_X))
		&& fRad == fRadian)
	{
		if (fabs(sinf(fRad)*byRadius) >= fabs(sinf(fRadian)*(pObjectPointTile->wIndex_Z-pStartingPointTile->wIndex_Z)))
		{
			return 1;
		}
	}

	return 0;
}

int GetDamageCircle(PARAM_TARGETCOUNT* pParam)
{
	// 원안에 있는놈들은 모두 리스트에 넣어라.
	CMap*		pMap = pParam->pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( pParam->vecStart->x, pParam->vecStart->y );

	if (!pStartingPointTile)	return 0;

	CSection* pStartingPointSection = pParam->pDungeonLayer->GetSection(pStartingPointTile->wAttr.uSection);

	if (!pStartingPointSection)	return 0;

	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pParam->bSkillKind);
	int nMax = pEffect->Value[pParam->bSkillLevel].nCompass; // 가로 최대 길이
	
	int nCount = 0;
	
	// 색션안에 있는놈들
	POSITION_ pos = pStartingPointSection->m_pMonsterList->GetHeadPosition();
	CMonster* pMonster = NULL;
		
	while(pos)
	{
		pMonster = (CMonster*)pStartingPointSection->m_pMonsterList->GetNext(pos);

		if (IsCollitionByCircle(pParam->pDungeonLayer, pParam->vecStart, pMonster->GetCurPosition(), BYTE(nMax)))
		{
			nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);

			if (nCount >= MAX_SKILL_TARGET_COUNT)
				return MAX_SKILL_TARGET_COUNT;			
		}
	}

	pos = pStartingPointSection->m_pPcList->GetHeadPosition();
	CUser* pUser = NULL;
		
	while(pos)
	{
		pUser = (CUser*)pStartingPointSection->m_pPcList->GetNext(pos);

		if (IsCollitionByCircle(pParam->pDungeonLayer, pParam->vecStart, pUser->GetCurPosition(), BYTE(nMax)))
		{
			nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);

			if (nCount >= MAX_SKILL_TARGET_COUNT)
				return MAX_SKILL_TARGET_COUNT;	
		}
	}

	// 링크된 섹션안에 있는놈들
	CSection* pLinkSection = NULL;

	for( int i = 0; pStartingPointSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = pParam->pDungeonLayer->GetSection(pStartingPointSection->m_wLink[i]);
		
		if( !pLinkSection )
		{
			continue;
		}

		pos = pLinkSection->m_pMonsterList->GetHeadPosition();

		while(pos)
		{
			pMonster = (CMonster*)pLinkSection->m_pMonsterList->GetNext(pos);
			
			if (IsCollitionByCircle(pParam->pDungeonLayer, pParam->vecStart, pMonster->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;
			}
		}

		pos = pLinkSection->m_pPcList->GetHeadPosition();

		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			
			if (IsCollitionByCircle(pParam->pDungeonLayer, pParam->vecStart, pUser->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;
			}
		}
	}

	return nCount;
}


time_t GetDueDay(int nEndMin)
{
	struct tm when;
	time_t now;

	time(&now);
	when = *localtime(&now);
	when.tm_min += nEndMin;
	now = mktime(&when);
	
	return now;	
}


DWORD GetLandTime(int nEndMin)
{
	return nEndMin * 60 * 1000;
}


BOOL IsOverDueDay(DWORD dwEndDate)
{
	time_t now;
	time(&now);
	
	return (dwEndDate < (DWORD)now);
}


int GetDamageObject(PARAM_TARGETCOUNT* pParam)
{
	int nCount = 0;

	if (pParam->bTargetType == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = g_pMonsterTable->GetMonster(pParam->dwTargetIndex);
		if (pMonster)
			nCount = InsertDamageTargetObject(pParam, 0, pParam->bTargetType, pMonster);		
	}
	else if (pParam->bTargetType == OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = g_pUserHash->GetData(pParam->dwTargetIndex);
		if (pUser)
		{
			if (pUser->GetCurLayerIndex() == INVALID_DUNGEON_LAYER)	return 0;
			nCount = InsertDamageTargetObject(pParam, 0, pParam->bTargetType, pUser);
		}		
	}
	
	return nCount;
}


// 몬스터를 소환한다.
int	GetSummon(PARAM_TARGETCOUNT* pParam)
{
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pParam->bSkillKind);
	int nCount		= 0;
	CUser* pUser	= pParam->pMaster;
	int nInsertNum	= -1;// 몬스터 내부하로 만들 자리가 있나?

	if ((nInsertNum = pUser->GetSubordinateIndex())<0) 
	{ 
		// 집어 넣을곳이 없구나.
		return 0;
	}

	// 한마리씩만 소환가능하다. 나중에 또 여러마리 소환가능할지도 모르니깐 4
	for(int i = 0; i < MAX_USER_GUARDIAN; ++i)
	{
		if (pUser->m_pMonster[i])
		{			
			if (pEffect->bID == __SKILL_REDELEMENTAL__ && 
				pUser->m_pMonster[i]->GetEffectDesc(pEffect->bID))
			{
				pUser->m_pMonster[i]->DetachSkill(pUser->m_pMonster[i]->GetEffectDesc(pEffect->bID));
				break;
			}
			else if (pEffect->bID == __SKILL_BLUEELEMENTAL__ && 
				pUser->m_pMonster[i]->GetEffectDesc(pEffect->bID))
			{
				pUser->m_pMonster[i]->DetachSkill(pUser->m_pMonster[i]->GetEffectDesc(pEffect->bID));
				break;
			}
			else if (pEffect->bID == __SKILL_GREENELEMENTAL__ && 
				pUser->m_pMonster[i]->GetEffectDesc(pEffect->bID))
			{
				pUser->m_pMonster[i]->DetachSkill(pUser->m_pMonster[i]->GetEffectDesc(pEffect->bID));
				break;
			}
		}
	}

	pParam->pTargetInfo[nCount].pUnit = 
		pParam->pDungeonLayer->CallMonster(pUser
											, nInsertNum
											, pEffect->Value[pParam->bSkillLevel].nMax
											, pUser->GetCurPosition());

	if (!pParam->pTargetInfo[nCount].pUnit)
		return 0;

	pParam->pTargetInfo[nCount].dwTargetType = OBJECT_TYPE_MONSTER;
	nCount = 1;

	return nCount;
}

int	GetDamageBox(PARAM_TARGETCOUNT* pParam)
{
	CMap*		pMap = pParam->pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( pParam->vecDest->x, pParam->vecDest->y );

	if (!pStartingPointTile)	return 0;

	CSection* pStartingPointSection = pParam->pDungeonLayer->GetSection(pStartingPointTile->wAttr.uSection);

	if (!pStartingPointSection)	return 0;

	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pParam->bSkillKind);
	int nMax = pEffect->Value[pParam->bSkillLevel].nCompass/2; // 가로 최대 길이
	
	int nCount = 0;
	VECTOR2	v2Dir = {0.f, 0.f};
	VECTOR3_SUB_VECTOR3( &v2Dir, pParam->vecStart, pParam->vecDest );
	float fRad = (float)atan2(v2Dir.y, v2Dir.x); // 나가는 방향

	// 색션안에 있는놈들
	POSITION_ pos		= pStartingPointSection->m_pMonsterList->GetHeadPosition();
	CMonster* pMonster	= NULL;

	// 박스의 원점을 박스의 중간점으로 되도록 하기위해서.
	VECTOR2 vecStart = *pParam->vecStart;

	vecStart.x = vecStart.x+cosf(fRad)*TILE_SIZE*nMax;
	vecStart.y = vecStart.y+sinf(fRad)*TILE_SIZE*nMax;

	while(pos)
	{
		pMonster = (CMonster*)pStartingPointSection->m_pMonsterList->GetNext(pos);

		if (IsCollitionByBox(pParam->pDungeonLayer, &vecStart, pMonster->GetCurPosition(), BYTE(nMax)))
		{
			nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
			if (nCount >= MAX_SKILL_TARGET_COUNT)
				return MAX_SKILL_TARGET_COUNT;		
		}
	}

	pos			= pStartingPointSection->m_pPcList->GetHeadPosition();
	CUser* pUser= NULL;
		
	while(pos)
	{
		pUser = (CUser*)pStartingPointSection->m_pPcList->GetNext(pos);

		if (IsCollitionByBox(pParam->pDungeonLayer, &vecStart
			, pUser->GetCurPosition(), BYTE(nMax)))
		{
			nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);

			if (nCount >= MAX_SKILL_TARGET_COUNT)
				return MAX_SKILL_TARGET_COUNT;			
		}
	}

	// 링크된 섹션안에 있는놈들
	CSection* pLinkSection = NULL;

	for( int i=0; pStartingPointSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = pParam->pDungeonLayer->GetSection(pStartingPointSection->m_wLink[i]);

		if( !pLinkSection ) continue;

		pos = pLinkSection->m_pMonsterList->GetHeadPosition();

		while(pos)
		{
			pMonster = (CMonster*)pLinkSection->m_pMonsterList->GetNext(pos);
			
			if (IsCollitionByBox(pParam->pDungeonLayer, &vecStart, 
				pMonster->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;
			}
		}

		pos = pLinkSection->m_pPcList->GetHeadPosition();

		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			
			if (IsCollitionByBox(pParam->pDungeonLayer, &vecStart
				, pUser->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;
			}
		}
	}
	
	return nCount;
}

int	GetDamage8Line(PARAM_TARGETCOUNT* pParam)
{
	CMap*		pMap = pParam->pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( pParam->vecStart->x, pParam->vecStart->y );

	if (!pStartingPointTile)	return 0;

	CSection* pStartingPointSection = pParam->pDungeonLayer->GetSection(pStartingPointTile->wAttr.uSection);
	
	if (!pStartingPointSection)	return 0;

	// X 범위 계산.
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pParam->bSkillKind);
	
	int nMax = pEffect->Value[pParam->bSkillLevel].nCompass; // 가로 최대 길이
	
	VECTOR2 array[] = 
	{
		{1.0, -1.0},{1.0, 0.0},{1.0, 1.0},
		{0.0, 1.0},	{-1.0, 1.0},{-1.0, 0.0},
		{-1.0, -1.0},{0.0, -1.0},
		{0.0, 0.0}
	};
	
	int nCount = 0;

	for(int arrayIndex = 0; arrayIndex < 9; ++arrayIndex)
	{	
		float fRad = (float)atan2(array[arrayIndex].y*TILE_SIZE, array[arrayIndex].x*TILE_SIZE); // 나가는 방향

		// 색션안에 있는놈들
		POSITION_ pos		= pStartingPointSection->m_pMonsterList->GetHeadPosition();
		CMonster* pMonster	= NULL;
			
		while(pos)
		{
			pMonster = (CMonster*)pStartingPointSection->m_pMonsterList->GetNext(pos);

			if (IsCollitionByLine(pParam->pDungeonLayer, pParam->vecStart
				, fRad, pMonster->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;				
			}
		}

		pos			= pStartingPointSection->m_pPcList->GetHeadPosition();
		CUser* pUser= NULL;
			
		while ( pos )
		{
			pUser = (CUser*)pStartingPointSection->m_pPcList->GetNext(pos);

			if ( IsCollitionByLine( pParam->pDungeonLayer, pParam->vecStart, fRad, pUser->GetCurPosition(), BYTE(nMax)) )
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;			
			}
		}

		// 링크된 섹션안에 있는놈들
		CSection* pLinkSection = NULL;

		for( int i = 0; pStartingPointSection->m_wLink[i] != 0; i++ )
		{
			pLinkSection = pParam->pDungeonLayer->GetSection(pStartingPointSection->m_wLink[i]);
			
			if( !pLinkSection ) continue;

			pos = pLinkSection->m_pMonsterList->GetHeadPosition();

			while(pos)
			{
				pMonster = (CMonster*)pLinkSection->m_pMonsterList->GetNext(pos);
				
				if (IsCollitionByLine(pParam->pDungeonLayer, pParam->vecStart
					, fRad, pMonster->GetCurPosition(), BYTE(nMax)))
				{
					nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
					if (nCount >= MAX_SKILL_TARGET_COUNT)
						return MAX_SKILL_TARGET_COUNT;
				}
			}

			pos = pLinkSection->m_pPcList->GetHeadPosition();

			while(pos)
			{
				pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
				
				if (IsCollitionByLine(pParam->pDungeonLayer, pParam->vecStart
					, fRad, pUser->GetCurPosition(), BYTE(nMax)))
				{
					nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);
					if (nCount >= MAX_SKILL_TARGET_COUNT)
						return MAX_SKILL_TARGET_COUNT;
				}
			}
		}
	}
	
	return nCount;
}

int GetDamageLine(PARAM_TARGETCOUNT* pParam)
{
	CMap*		pMap = pParam->pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( pParam->vecDest->x, pParam->vecDest->y );

	if (!pStartingPointTile)	return 0;

	CSection* pStartingPointSection = pParam->pDungeonLayer->GetSection(pStartingPointTile->wAttr.uSection);

	if (!pStartingPointSection)	return 0;
		
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pParam->bSkillKind);
	int		nMax	= pEffect->Value[pParam->bSkillLevel].nCompass; // 가로 최대 길이	
	int		nCount	= 0;
	
	// 나가는 방향
	float fRad = (float)atan2(pParam->vecStart->y-pStartingPointTile->z, pParam->vecStart->x-pStartingPointTile->x); 

	// 색션안에 있는놈들
	POSITION_ pos		= pStartingPointSection->m_pMonsterList->GetHeadPosition();
	CMonster* pMonster	= NULL;
		
	while(pos)
	{
		pMonster = (CMonster*)pStartingPointSection->m_pMonsterList->GetNext(pos);

		if (IsCollitionByLine(pParam->pDungeonLayer, pParam->vecDest
			, fRad, pMonster->GetCurPosition(), BYTE(nMax)))
		{
			nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
			if (nCount >= MAX_SKILL_TARGET_COUNT)
				return MAX_SKILL_TARGET_COUNT;			
		}
	}

	pos				= pStartingPointSection->m_pPcList->GetHeadPosition();
	CUser* pUser	= NULL;
		
	while(pos)
	{
		pUser = (CUser*)pStartingPointSection->m_pPcList->GetNext(pos);

		if (IsCollitionByLine(pParam->pDungeonLayer, pParam->vecDest
			, fRad, pUser->GetCurPosition(), BYTE(nMax)))
		{
			nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);
			if (nCount >= MAX_SKILL_TARGET_COUNT)
				return MAX_SKILL_TARGET_COUNT;			
		}
	}

	// 링크된 섹션안에 있는놈들
	CSection* pLinkSection = NULL;
	
	for( int i=0; pStartingPointSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = pParam->pDungeonLayer->GetSection(pStartingPointSection->m_wLink[i]);
		if( !pLinkSection ) continue;

		pos = pLinkSection->m_pMonsterList->GetHeadPosition();

		while(pos)
		{
			pMonster = (CMonster*)pLinkSection->m_pMonsterList->GetNext(pos);
			
			if (IsCollitionByLine(pParam->pDungeonLayer, pParam->vecDest
				, fRad, pMonster->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;
			}
		}

		pos = pLinkSection->m_pPcList->GetHeadPosition();

		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			
			if (IsCollitionByLine(pParam->pDungeonLayer, pParam->vecDest
				, fRad, pUser->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;
			}
		}
	}

	return nCount;
}

int GetDamageArc(PARAM_TARGETCOUNT* pParam)
{
	CMap*		pMap = pParam->pDungeonLayer->GetMap();
	MAP_TILE*	pStartingPointTile = pMap->GetTile( pParam->vecDest->x, pParam->vecDest->y );
	if (!pStartingPointTile)	return 0;

	CSection* pStartingPointSection = pParam->pDungeonLayer->GetSection(pStartingPointTile->wAttr.uSection);
	if (!pStartingPointSection)	return 0;
	
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pParam->bSkillKind);
	int nMax = pEffect->Value[pParam->bSkillLevel].nCompass; // 가로 최대 길이
	
	int nCount = 0;
	float fRad = (float)atan2((float)(int(pParam->vecStart->y / TILE_SIZE) - pStartingPointTile->wIndex_Z
		), (float)(int(pParam->vecStart->x / TILE_SIZE) - pStartingPointTile->wIndex_X)); // 나가는 방향

	// 색션안에 있는놈들
	POSITION_ pos = pStartingPointSection->m_pMonsterList->GetHeadPosition();
	CMonster* pMonster = 0;
		
	while(pos)
	{
		pMonster = (CMonster*)pStartingPointSection->m_pMonsterList->GetNext(pos);
		if (IsCollitionByArc(pParam->pDungeonLayer, pParam->vecDest
			, fRad, pMonster->GetCurPosition(), BYTE(nMax)))
		{
			nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
			if (nCount >= MAX_SKILL_TARGET_COUNT)
				return MAX_SKILL_TARGET_COUNT;
			
		}
	}

	pos = pStartingPointSection->m_pPcList->GetHeadPosition();
	CUser* pUser=0;
		
	while(pos)
	{
		pUser = (CUser*)pStartingPointSection->m_pPcList->GetNext(pos);
		if (IsCollitionByArc(pParam->pDungeonLayer, pParam->vecDest
			, fRad, pUser->GetCurPosition(), BYTE(nMax)))
		{
			nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);
			if (nCount >= MAX_SKILL_TARGET_COUNT)
				return MAX_SKILL_TARGET_COUNT;			
		}
	}

	// 링크된 섹션안에 있는놈들
	CSection* pLinkSection=0;

	for( int i=0; pStartingPointSection->m_wLink[i] != 0; i++ )
	{
		pLinkSection = pParam->pDungeonLayer->GetSection(pStartingPointSection->m_wLink[i]);

		if( !pLinkSection )
		{
			continue;
		}

		pos = pLinkSection->m_pMonsterList->GetHeadPosition();

		while(pos)
		{
			pMonster = (CMonster*)pLinkSection->m_pMonsterList->GetNext(pos);
			
			if (IsCollitionByArc(pParam->pDungeonLayer, pParam->vecDest
				, fRad, pMonster->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_MONSTER, pMonster);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;
			}
		}

		pos = pLinkSection->m_pPcList->GetHeadPosition();

		while(pos)
		{
			pUser = (CUser*)pLinkSection->m_pPcList->GetNext(pos);
			
			if (IsCollitionByArc(pParam->pDungeonLayer, pParam->vecDest
				, fRad, pUser->GetCurPosition(), BYTE(nMax)))
			{
				nCount += InsertDamageTargetObject(pParam, BYTE(nCount), OBJECT_TYPE_PLAYER, pUser);
				if (nCount >= MAX_SKILL_TARGET_COUNT)
					return MAX_SKILL_TARGET_COUNT;
			}
		}
	}

	return nCount;	
}


// 해당 타겟의 위치를 알아와 스킬 스타트 포지션에 값을 받아온다.
VECTOR2 GetSkillStartPosition(BYTE bTargetType, DWORD dwTargetIndex, WORD wTileIndex_X, WORD wTileIndex_Z)
{
	VECTOR2 vecStart = {0.f, 0.f};

	if (bTargetType & OBJECT_TYPE_PLAYER)
	{
		CUser* pTargetUser = g_pUserHash->GetData( dwTargetIndex );

		if (pTargetUser)
			vecStart = *pTargetUser->GetCurPosition();
		else
			goto lbl_tile;
	}
	else if (bTargetType & OBJECT_TYPE_MONSTER)
	{
		CMonster* pTargetMonster = g_pMonsterTable->GetMonster( dwTargetIndex );

		if (pTargetMonster)
			vecStart = *pTargetMonster->GetCurPosition();
		else
			goto lbl_tile;

	}
	else if (bTargetType == OBJECT_TYPE_SKILL)
	{
		vecStart.x = (dwTargetIndex>>16)*TILE_WIDTH+TILE_WIDTH/2;
		vecStart.y = (dwTargetIndex&0x0000ffff)*TILE_WIDTH+TILE_WIDTH/2;
	}
	else if (bTargetType & OBJECT_TYPE_TILE)
	{
lbl_tile:
		vecStart.x = wTileIndex_X * TILE_SIZE + TILE_SIZE / 2;
		vecStart.y = wTileIndex_Z * TILE_SIZE + TILE_SIZE / 2;		
	}

	return vecStart;
}

BOOL IsLinkSection(CDungeonLayer* pDungeonLayer, CSection* pStartingPointSection, CSection* pInnerPointSection)
{
	BOOL bInSection = false;
	if (pStartingPointSection == pInnerPointSection)
		return true;

	for( int i = 0; pStartingPointSection->m_wLink[i] != 0; i++ )
	{
		// 아무리 영역이 커도 링크색션까지만 검사해라.
		CSection* pLinkSection = pDungeonLayer->GetSection(pStartingPointSection->m_wLink[i]);
		if( !pLinkSection ) continue;

		if (pInnerPointSection == pLinkSection)
		{
			bInSection = true;
			break;
		}
	}

	return bInSection;						
}

// 스킬영역에 있는 놈들 모두 가져와라.
int GetTargetCount(PARAM_TARGETCOUNT* pParam)
{
	int nCount = 0;
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pParam->bSkillKind);

	switch(pEffect->bSkillType)	
	{
		case TARGETTYPE_OBJECT:			nCount = GetDamageObject(pParam);	break;
		case TARGETTYPE_LINE:			nCount = GetDamageLine(pParam);		break;
		case TARGETTYPE_ARC:			nCount = GetDamageArc(pParam);		break;
		case TARGETTYPE_CIRCLE:			nCount = GetDamageCircle(pParam);	break;
		case TARGETTYPE_X:				nCount = GetDamage8Line(pParam);	break;
		case TARGETTYPE_BOX:			nCount = GetDamageBox(pParam);		break;
		case TARGETTYPE_MULTICOLLITION:	nCount = GetDamageCircle(pParam);	break;
		case TARGETTYPE_SUMMONTILE:		nCount = GetSummon(pParam);			break;		
	}

	return nCount;
}

// 스킬데미지에  대한 저항력을 얻어와라.
int GetPropertyRealDamageByResist(BYTE bProperty, int iDamage, BYTE bOffenseOjectType, DWORD dwOffenseIndex
								  , BYTE bDefenseObjectType, DWORD dwDefenseIndex)
{	
	int nResist = 0;
	DWORD dwVit = 0;
	short wFireResist=0, wIceResist=0, wPhyResist=0, wLightResist=0, wPoiResist=0, wAllResist=0;

	if (bDefenseObjectType == OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = g_pUserHash->GetData(dwDefenseIndex);
		if (!pUser)	return 0;

		dwVit			= pUser->GetNewestVit()/5;
		wFireResist		= pUser->GetFireResist();
		wIceResist		= pUser->GetIceResist();
		wPhyResist		= pUser->GetPhyResist();
		wLightResist	= pUser->GetLightResist();
		wPoiResist		= pUser->GetPoiResist();
		wAllResist		= pUser->GetAllResist();		
	}
	else if (bDefenseObjectType == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = g_pMonsterTable->GetMonster(dwDefenseIndex);
		if (!pMonster)	return 0;

		wFireResist		= pMonster->GetFireResist();
		wIceResist		= pMonster->GetIceResist();
		wPhyResist		= pMonster->GetPhyResist();
		wLightResist	= pMonster->GetLightResist();
		wPoiResist		= pMonster->GetPoiResist();
		wAllResist		= pMonster->GetAllResist();
	}

	switch(bProperty)
	{
		case SKILL_PROPERTY_KIND_FIRE:// 불저항력만큼 풀데미지 감소
			//{ 화염데미지 * (100 - USER_FIRE_RESIST + 공격자.ITEM_ATTR_TARGET_FIRE_RESIST + 공격자.ITEM_ATTR_TARGET_ALL_RESIST) / 100 - ITEM_ATTR_FIRE_RESIST_POINT - ITEM_ATTR_ALL_RESIST_POINT } * (100 - ITEM_ATTR_FIRE_ABSORB ) / 100 + ITEM_ATTR_FIRE_ABSORB_POINT + ITEM_ATTR_ALL_ABSORB_POINT
			nResist = (int)(iDamage * (100 - wFireResist)* 0.01f);		
			break;

		case SKILL_PROPERTY_KIND_ICE:// 얼음저항력만큼 풀데미지 감소
			nResist = (int)(iDamage * (100 - wIceResist) * 0.01f);		
			break;

		case SKILL_PROPERTY_KIND_EARTH:// 물리 1/2, 불1/2
			nResist = (int)(iDamage * (100-wPhyResist) * 0.01f);		
			break;

		case SKILL_PROPERTY_KIND_LIGHT:// 전기
			nResist = (int)(iDamage * (100-wLightResist) * 0.01f);		
			break;

		case SKILL_PROPERTY_KIND_PHY:// 물리
			nResist = (int)(iDamage * (100-wPhyResist) * 0.01f);
			break;

		case SKILL_PROPERTY_KIND_POI:// 독
			nResist = (int)(iDamage * (100-wPoiResist) * 0.01f-dwVit);		
			break;

		default:		
			nResist = (int)(iDamage*(100-wAllResist)/100.);	// 다 받아라.
	}

	if (nResist < 0)
		nResist = 0;

	return nResist;
}


void MakeAppearUser(DSTC_APPEAR* pAppear, CUser* pUser, BYTE bPortal)
{
	pAppear->dwUserIndex	= pUser->GetID();
	pAppear->v2CurPos		= *pUser->GetCurPosition();
	pAppear->wHead			= pUser->GetHeadModelNo();
	pAppear->wClass			= pUser->GetClass();
	pAppear->wArmor			= pUser->m_pEquip[EQUIP_TYPE_MAIL].m_wItemID;
	pAppear->wHelmet		= pUser->m_pEquip[EQUIP_TYPE_HELMET].m_wItemID;
	pAppear->v2Direction	= *pUser->GetCurDirection();
		
	if(pUser->m_bCurrentHand==1)
	{
		pAppear->wHandR		= pUser->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID;
		pAppear->wHandL		= pUser->m_pEquip[EQUIP_TYPE_LHAND1].m_wItemID;
	}
	else if(pUser->m_bCurrentHand==2)
	{
		pAppear->wHandR		= pUser->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID;
		pAppear->wHandL		= pUser->m_pEquip[EQUIP_TYPE_LHAND2].m_wItemID;
	}	
	
	pAppear->byGuildFlag	= pUser->m_byGuildFlag;
	pAppear->wCriminalTime	= pUser->m_wCriminalPropensity;
	pAppear->bAttackMode	= BYTE(pUser->GetAttackMode());
	pAppear->wSiegePKCount	= pUser->m_wSiegePKCount;
	//pAppear->bIsPortal			= bPortal;
	
	__lstrcpyn( pAppear->szName, (LPSTR)pUser->GetName(), MAX_CHARACTER_REAL_LENGTH );
	__lstrcpyn( pAppear->szNickName, pUser->m_szNickName, MAX_NICK_REAL_LENGTH  );
	pAppear->dwPartyID		= pUser->m_dwPartyId;
	pAppear->dwGuildId		= pUser->m_dwGuildId;
	pAppear->byUpgrade		= pUser->m_byUpgrade;

	CDungeonLayer *pLayer = pUser->GetCurDungeonLayer();//길드전 플래그 : hwoarang 050202 
	if( pLayer 
	&&	pLayer->m_pTeamMatch)
	{
		pAppear->dwGuildWarFlag	= pLayer->m_pTeamMatch->GetTeamMatchFlag( pUser );
		pAppear->team_index	= pLayer->m_pTeamMatch->GetTeamIndex( pUser );
	}
	else
	{
		pAppear->dwGuildWarFlag = G_W_F_NONE;
		pAppear->team_index = 0;
	}
}


BOOL IsAbleUpgradeAriesZodiac(CItem* pItem)
{
	return	pItem->GetID() == 8000 ||
			pItem->GetID() == 8201 ||
			pItem->GetID() == 8202 ||
			pItem->GetID() == 8203 ||
			pItem->GetID() == 8204 ||
			pItem->GetID() == 8205;
}


BOOL IsGrowGuardianItem(CItem* pGuardianItem)
{
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pGuardianItem->GetID());

	return (pGuardianItem->m_Item_Guardian.dwEndLife 
		&& !IsOverDueDay(pGuardianItem->m_Item_Guardian.dwEndLife)
		&& pGuardianItem->m_Item_Guardian.dwCompleteTime 
				< pBaseItem->BaseItem_Guardian.wCompleteTime);
}

void SendExchangeGuardianLife(CUser* pMasterUser, int nMinute)
{
	CItem* pGuardianItem = &pMasterUser->m_pInv_Guardian[0];
	::ExchangeGuardianLife(pGuardianItem, nMinute);
	::SendToUpdate_GuardianItem(pMasterUser->GetConnectionIndex(), pGuardianItem);
}

void ExchangeGuardianLife(CItem* pGuardianItem, int nMinute)
{
	struct tm when = *localtime((const long*)&pGuardianItem->m_Item_Guardian.dwEndLife);
	when.tm_min += nMinute;	// 가디언 죽거나, 레벨업에 시간 변동 : 최덕석 2005.1.10
	pGuardianItem->m_Item_Guardian.dwEndLife = mktime(&when);
}

void SendSummonEggStatus(DWORD dwConnectionIndex, BOOL bSummon, const CItem* pGuardianItem)
{
	DSTC_GURADIAN_EGG_SUMMON_STATUS packet;
	packet.byGuardianEggSummonStatus = (BYTE)bSummon;
	packet.byZipCode = 0;
	packet.i64ID = pGuardianItem->GetSerial()->i64Serial;
	
	NetSendToUser(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
}


int	IsSkillMon(BYTE bSkillKind, CMonster* pMonster)
{
	return (pMonster->GetHP() && pMonster->GetUnitStatus() != UNIT_STATUS_DEAD);
}


int	IsSkillUser(BYTE bSkillKind, CUser* pUser)
{
	switch(bSkillKind)// : 050108 hwoarang
	{
	case __SKILL_TREEAGION__:
		{
			if(UNIT_STATUS_DEAD == pUser->GetUnitStatus())
			{
				return 1;
			}
		}break;
	case __SKILL_SUMMONACCELERATOR__:
		{
			return 0;
		}break;
	default	:
		{
			if( pUser->GetHP()
			&&  (UNIT_STATUS_DEAD	!=  pUser->GetUnitStatus())	)
			{
				return 1;
			}
		}break;
	}
	return 0;//지우지 마세요;
}

BOOL IsValidUnit(const CUnit* pUnit)
{
	if (!pUnit)
		return FALSE;
	
	if(IsBadReadPtr(pUnit, sizeof(CUnit)))
		return FALSE;

	if (pUnit->GetUnitStatus() == UNIT_STATUS_NONE)
		return FALSE;

	return TRUE;
}

int	IsSkillMonMon(BYTE bSkillKind, CMonster *pOwnMonster, CMonster* pTargetMonster)
{
	if(bSkillKind >= MAX_SKILL) return 0;
	if (!IsValidUnit(pTargetMonster))		return 0;
	if (!IsValidUnit(pOwnMonster))			return 0;
	
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);

	if (pEffect->bCrime == CRIME_APPLY_ENEMY 
				&& pOwnMonster->IsAlliance((CUnit*)pTargetMonster))
	{
		return 0;
	}

	if (!IsSkillMon(bSkillKind, pTargetMonster))	
	{
		return 0;
	}

	if(pOwnMonster->GetLord() && pTargetMonster->GetLord())
	{
		CUser* pOwnLord		= (CUser*)pOwnMonster->GetLord();
		CUser* pTargetLord	= (CUser*)pTargetMonster->GetLord();

		BOOL bGuildWar = IsGuildWar( (CUser*)pOwnMonster->GetLord(), (CUser*)pTargetMonster->GetLord());
/*									pOwnMonster->GetCurDungeon()
									, pTargetLord->m_dwGuildId
									, pOwnLord->m_dwGuildId
									, pTargetLord->m_dwPartyId
									, pOwnLord->m_dwPartyId
									, GODMODE_STATUS_RELEASE);
*/
		if (	pEffect->bCrime == CRIME_APPLY_ENEMY 
				&& pOwnLord->IsAlliance((CUnit*)pTargetLord))
		{
			return 0;
		}

		if (pEffect->bCrime == CRIME_APPLY_ENEMY
					&& !( pOwnLord->m_sPKDescInfo.m_bPKMode || bGuildWar) )					
		{
			return 0;
		}
	}

	return 1;
}


int	IsSkillMonUser(BYTE bSkillKind, CMonster *pOwnMonster, CUser* pTargetUser)
{
	
	if(bSkillKind >= MAX_SKILL) return 0;
	if(!pTargetUser)			return 0;
	if(!pOwnMonster)			return 0;

	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	
	// 적에게 사용되는 스킬이며, 대상이 동맹이라면 무시.
	if (pEffect->bCrime == CRIME_APPLY_ENEMY && pOwnMonster->IsAlliance((CUnit*)pTargetUser))	
		return 0;

	// 주인이 존재하는 몬스터의 경우,
	if(pOwnMonster->GetLord())
	{
		CUser* pLord = (CUser*)pOwnMonster->GetLord();

		BOOL bGuildWar = IsGuildWar(pTargetUser, pLord);
/*									pTargetUser->GetCurDungeon()
									, pTargetUser->m_dwGuildId
									, pLord->m_dwGuildId
									, pTargetUser->m_dwPartyId
									, pLord->m_dwPartyId
									, pLord->m_enGodMode);
*/
		// 적에게 사용되는 스킬이며, PK 모드도 길드전 중도 아니며라면 무시.
		if(!( (pLord->m_sPKDescInfo.m_bPKMode || bGuildWar ) && pEffect->bCrime == CRIME_APPLY_ENEMY))
			return 0;		
	}

	if(!IsSkillUser(bSkillKind, pTargetUser))
		return 0;

	return 1;
}


int	IsSkillUserUser(BYTE bSkillKind, CUser *pOwnUser, CUser* pTargetUser, BYTE bPK)
{
	if(bSkillKind >= MAX_SKILL) return 0;
	if(!pTargetUser)			return 0;
	if(!pOwnUser)				return 0;

	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);

#ifdef JAPAN_LOCALIZING	//	일본은 배쉬 스킬 대인공격 안됨!!
	if (pEffect->bID == __SKILL_BASH__)	// added by minjin. 2004. 10. 30.
		return 0;
#endif

	if (!IsSkillUser(bSkillKind, pTargetUser))
	{
		return 0;// 죽었을때 스킬 받을수 있는것은 오직 트래이지온뿐이다.
	}

	if (pEffect->bCrime == CRIME_APPLY_FRIENDLY &&
				!(pEffect->bSkillTarget & TARGETTYPE_ENEMY_PLAYER)
			&& (!pOwnUser->IsAlliance((CUnit*)pTargetUser) 
					&& pEffect->bSkillTarget & TARGETTYPE_FRIEND_PLAYER)
		)
	{
		return 0;//아군만 적용하는 마법이다.
	}

	if (pEffect->bCrime == CRIME_APPLY_ENEMY 
				&& (pOwnUser->IsAlliance((CUnit*)pTargetUser) 
			|| pTargetUser->m_enGodMode))
	{
		return 0;// 적공격하는 마법인데 우리편, 신모드인놈들 제외.
	}

	BOOL bGuildWar = IsGuildWar( pOwnUser, pTargetUser );

	if ( (pEffect->bCrime == CRIME_APPLY_ENEMY || 
			pEffect->bCrime == CRIME_APPLY_ENEMY_FRIENDLY_MONSTER)
		 && !(bPK || bGuildWar) )
	{
		return 0;//적공격하는 마법이라면 pk모드가 활성화 되있어야 한다.
	}
	
	return 1;
}


int	IsSkillUserMon(BYTE bSkillKind, CUser *pOwnUser, CMonster* pTargetMonster)
{
	if(bSkillKind >= MAX_SKILL) return 0;
	if(!pOwnUser)				return 0;
	if(!pTargetMonster)			return 0;

	if (!IsSkillMon(bSkillKind, pTargetMonster))
		return 0;

	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	if (pEffect->bCrime == CRIME_APPLY_ENEMY && pOwnUser->IsAlliance((CUnit*)pTargetMonster))
		return 0;// 적공격하는 마법인데 우리편은 빼야지.

	if(pTargetMonster->GetLord())
	{
		CUser* pLord = (CUser*)pTargetMonster->GetLord();

		BOOL bGuildWar = IsGuildWar(pOwnUser, pLord);
//									, pOwnUser->m_dwGuildId
///									, pLord->m_dwGuildId
//									, pOwnUser->m_dwPartyId
//									, pLord->m_dwPartyId
//									, GODMODE_STATUS_RELEASE);

		if(	!pOwnUser->IsAlliance((CUnit*)pTargetMonster) &&
			pEffect->bCrime == CRIME_APPLY_ENEMY &&
			!(pOwnUser->m_sPKDescInfo.m_bPKMode || bGuildWar) ) 
			return 0;
	}

	if (pEffect->bCrime == CRIME_APPLY_FRIENDLY &&
		!(pEffect->bSkillTarget & TARGETTYPE_ENEMY_MONSTER)
		&& (!pOwnUser->IsAlliance((CUnit*)pTargetMonster) 
		&& pEffect->bSkillTarget & TARGETTYPE_FRIEND_MONSTER) )
		return 0;
	
	return 1;
}

void SystemSkillToUser(CDungeonLayer* pLayer
					   , BYTE bSkillKind
					   , BYTE bSkillLevel
					   , DWORD dwTime
					   , const VECTOR2* const pvecStartPosition
					   , WORD wMinMax[2]
					   , BYTE bOwnType
					   , DWORD dwOwnIndex
					   , CUser* pTargetUser)
{
	SKILLDESC skillDesc;
	skillDesc.dwOwnIndex		= int(pvecStartPosition->x/TILE_WIDTH)<<16 | int(pvecStartPosition->y/TILE_HEIGHT);
	skillDesc.bOwnType			= OBJECT_TYPE_SKILL;
	skillDesc.bPK				= 0;
	skillDesc.bSectionNum		= pTargetUser->GetPrevSectionNum();
	skillDesc.bSkillKind		= bSkillKind;
	skillDesc.bSkillLevel		= bSkillLevel;
	skillDesc.bTargetType		= OBJECT_TYPE_PLAYER;
	skillDesc.dwSkillKeepTime	= dwTime;
	skillDesc.dwTargetIndex		= pTargetUser->GetID();
	skillDesc.pDungeonLayer		= pLayer;
	skillDesc.v2OwnObjectPos	= *pvecStartPosition;
	skillDesc.wDamageMinMax[0]	= WORD(wMinMax[0]/4);
	skillDesc.wDamageMinMax[1]	= WORD(wMinMax[1]/4);
	skillDesc.wTileIndex_X		= 0;
	skillDesc.wTileIndex_Z		= 0;
	skillDesc.pMonsterMaster	= NULL;
	
	if (bOwnType == OBJECT_TYPE_PLAYER)
	{
		skillDesc.pMonsterMaster = g_pUserHash->GetData(dwOwnIndex);
		if (skillDesc.pMonsterMaster)
		{
			skillDesc.bPK = skillDesc.pMonsterMaster->m_sPKDescInfo.m_bPKMode;
			skillDesc.dwOwnIndex = skillDesc.pMonsterMaster->GetID();			
		}
	}
	
	g_pEffectLayer->SendSkill(&skillDesc);
}

void SystemSkillToMonster(CDungeonLayer* pLayer
							, BYTE bSkillKind
							, BYTE bSkillLevel
							, DWORD dwTime
							, const VECTOR2* const pvecStartPosition
							, WORD wMinMax[2]
							, BYTE bOwnType
							, DWORD dwOwnIndex
							, CMonster* pTargetMonster)
{	
	SKILLDESC skillDesc;

	skillDesc.dwOwnIndex		= int(pvecStartPosition->x/TILE_WIDTH)<<16 | int(pvecStartPosition->y/TILE_HEIGHT);
	skillDesc.bOwnType			= OBJECT_TYPE_SKILL;
	skillDesc.bPK				= 0;
	skillDesc.bSectionNum		= pTargetMonster->GetPrevSectionNum();
	skillDesc.bSkillKind		= bSkillKind;
	skillDesc.bSkillLevel		= bSkillLevel;
	skillDesc.bTargetType		= OBJECT_TYPE_MONSTER;
	skillDesc.dwSkillKeepTime	= dwTime;
	skillDesc.dwTargetIndex		= pTargetMonster->GetID();
	skillDesc.pDungeonLayer		= pLayer;
	skillDesc.v2OwnObjectPos	= *pvecStartPosition;
	skillDesc.wDamageMinMax[0]	= wMinMax[0];
	skillDesc.wDamageMinMax[1]	= wMinMax[1];
	skillDesc.wTileIndex_X		= 0;
	skillDesc.wTileIndex_Z		= 0;
	skillDesc.pMonsterMaster	= NULL;
	
	if (bOwnType == OBJECT_TYPE_PLAYER)
	{
		skillDesc.pMonsterMaster = g_pUserHash->GetData(dwOwnIndex);
		if (skillDesc.pMonsterMaster)
		{
			skillDesc.bPK = skillDesc.pMonsterMaster->m_sPKDescInfo.m_bPKMode;
			skillDesc.dwOwnIndex = skillDesc.pMonsterMaster->GetID();
		}
	}

	g_pEffectLayer->SendSkill(&skillDesc);
}

DWORD GetGuardianItemLevel(DWORD dwExp)
{	
	DWORD dwLow = 1, dwHigh = 200;
	DWORD dwMidLevel	= (dwHigh-dwLow)/2;
	DWORD dwMidExp		= 0;
	DWORD dwPrevExp		= 0;
	int cnt = 0;

	while(dwMidLevel)
	{
		dwMidExp	= GetExpTableOfLevel(OBJECT_TYPE_MONSTER, dwMidLevel);
		dwPrevExp	= GetExpTableOfLevel(OBJECT_TYPE_MONSTER, dwMidLevel-1);

		
		if(dwMidExp == 0)
			dwMidExp = -1;
		if(dwPrevExp == 0)
			dwPrevExp = -1;

		if (dwMidLevel == 1 || dwMidLevel == MAX_LEVEL)
		{
			break;
		}
		else if (dwExp < dwMidExp)
		{
			dwHigh = dwMidLevel;
			if (dwPrevExp <= dwExp)
			{
				--dwMidLevel;
				break;
			}

			dwMidLevel -= (dwHigh-dwLow)/2;
		}
		else if (dwMidExp < dwExp)
		{
			dwLow = dwMidLevel;
			if (dwPrevExp >= dwExp)
			{
				++dwMidLevel;
				break;
			}

			dwMidLevel += (dwHigh-dwLow)/2;				
		}
		else
		{
			break;
		}

		// 안정치
		if(cnt++ > 200)
		{
			Log(LOG_IMPORTANT, "Couldn't calculate guardian level.");
			break;
		}
	}

	return dwMidLevel;
}