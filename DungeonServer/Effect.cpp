/////////////////////////////////////
//
//		 yakolla
//
/////////////////////////////////////
#include "stdafx.h"
#include "effect.h"
#include "OwnServer.h"
#include "Monster.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "MonsterTable.h"
#include "MonsterManager.h"
#include "DungeonLayer.h"

int Effect::GetMaxMastery(DWORD dwUserLevel, BYTE bSkillLevel, WORD wClass)
{
	/*
	int nMaxMastery = 0;
	switch ((wProperty/100))
	{
	case CLASS_TYPE_WARRIOR:
		nMaxMastery = Value[bSkillLevel].nMax*g_pBaseClassInfo[wClass].iMax_Aura/100.;
		break;
	case CLASS_TYPE_PRIEST:
		nMaxMastery = Value[bSkillLevel].nMax*g_pBaseClassInfo[wClass].iMax_Divine/100.;
		break;
	case CLASS_TYPE_SUMMONER:
		nMaxMastery = Value[bSkillLevel].nMax*g_pBaseClassInfo[wClass].iMax_Summon/100.;
		break;
	case CLASS_TYPE_HUNTER:
		nMaxMastery = Value[bSkillLevel].nMax*g_pBaseClassInfo[wClass].iMax_Chakra/100.;
		break;
	case CLASS_TYPE_WIZARD:
		nMaxMastery = Value[bSkillLevel].nMax*g_pBaseClassInfo[wClass].iMax_Magic/100.;
		break;
	}
	return nMaxMastery;
	*/
	return bSkillLevel;
}

void EffectLayer::Init()
{
	memset(m_Effect, 0, sizeof(m_Effect));
		
	LoadScript();
}

void EffectLayer::LoadScript()
{
	// 스킬 데이터를 로드한다.
	char szFile[ _MAX_PATH ]={0,};
	wsprintf(szFile, "%s\\skill.iti",  g_pThis->GetServerDataPath());
	if (Load(szFile) == 0) MessageBox(0, "skill.iti Error.", 0, 0);

	/*for(int i = 0; i < MAX_KIND_OF_CLASS_EX; ++i)
	{// 리소스를 미리 메모리에 읽어 둔다.
		Node<Effect>	*pNode = m_pEffectHash[i]->GetHead();
		while(pNode)
		{
			

			pNode = pNode->pNext;
		}
		
	}*/
}

DWORD EffectLayer::Load(char *szFileName)
{
	BASESKILL baseskill[MAX_SKILL_COUNT];	

	int count = g_pDb->OpenRecord( "SELECT [ID], [ResourceID], [StatusResourceID], [Property], [Type], [ableclass], [Target], [Target_Type], [Range], [Matery], [CastingTime], [CoolTime], [StateEffectLocation], [JointEffectCount], [Formula], [SuccessFormula], [SoundNum], [StatusID], [Crime], [Efficiency], [Value] FROM [Skill_Effect]", &baseskill, MAX_SKILL_COUNT, GAME_DB );

	for(int i = 0; i < count; ++i)
	{
		memcpy(&m_Effect[baseskill[i].bID], &baseskill[i], sizeof(BASESKILL));		
	}

	return TRUE;
}

EffectLayer::~EffectLayer()
{

}

Effect* EffectLayer::GetEffectInfo(BYTE bSkillID)
{	
	return &m_Effect[bSkillID];
}

int EffectLayer::GetEffectStatusPostion(BYTE bID)
{
	return m_Effect[bID].GetEffectPosition();
}

int Effect::GetEffectPosition()
{
	return dwEffectPosition;
}

void EffectLayer::SendSkillStatusUserUser(CUser *pSendUser,CUser* pTargetUser, BYTE bSkillKind, BYTE bSkillLevel, DWORD dwEndTime)
{
	DSTC_APEAR_SKILL	packet;

	packet.vecStart			= *pTargetUser->GetCurPosition();
	packet.bTargetType		= OBJECT_TYPE_PLAYER;
	packet.dwTargetIndex	= pTargetUser->GetID();
	packet.bSkillKind		= bSkillKind;
	packet.bSkillLevel		= bSkillLevel;
	packet.dwDestTime		= dwEndTime;

	NetSendToUser( pSendUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
}

void EffectLayer::SendSkillStatusUserMon(CUser* pSendUser, CMonster *pMonster, BYTE bSkillKind, BYTE bSkillLevel, DWORD dwEndTime)
{
	DSTC_APEAR_SKILL	packet;

	packet.vecStart			= *pMonster->GetCurPosition();
	packet.bTargetType		= OBJECT_TYPE_MONSTER;
	packet.dwTargetIndex	= pMonster->GetID();
	packet.bSkillKind		= bSkillKind;
	packet.bSkillLevel		= bSkillLevel;
	packet.dwDestTime		= dwEndTime;

	NetSendToUser( pSendUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
}

float EffectLayer::GetFormula17(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass)
{//스킬.min/100 * BaseClassSkill.Max / 100
	return GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMin/100.f * GetBaseClassSkillMax(wClass, bSkillKind)/100.f;
}

float EffectLayer::GetFormula18(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass)
{//스킬.sp * (100 - 스킬.min) / 100 * BaseClassSkill.Max / 100
	Effect* pEffect = GetEffectInfo(bSkillKind);
	return pEffect->Value[bSkillLevel].nMana*(100-pEffect->Value[bSkillLevel].nMin)/100.f*GetBaseClassSkillMax(wClass, bSkillKind)/100.f;
}

float EffectLayer::GetFormula3(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass)
{
	//최종 파라미터 상승분 = 순수 캐릭터 파라미터 * 스킬에서 올려주는% / 100 * BaseClassSkill.Max / 100
	int nMin = GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMin;
	int nBaseClassSkillMax = GetBaseClassSkillMax(wClass, bSkillKind);
	
	return (nMin/100.f)*nBaseClassSkillMax/100.f;
}

float EffectLayer::GetFormula4(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, float fOriginalParam)
{
	//최종 파라미터 상승분 = 순수 캐릭터 파라미터 * 스킬에서 올려주는% / 100 * BaseClassSkill.Max / 100
	int nMax = GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMax;
	int nBaseClassSkillMax = GetBaseClassSkillMax(wClass, bSkillKind);
	
	return (float)(fOriginalParam*(nMax/100.)*nBaseClassSkillMax/100.);
}

float EffectLayer::GetFormula5(BYTE bSkillKind, BYTE bSkillLevel, float fOriginalParam)
{
	int nMax = GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMax;
	return (float)(fOriginalParam*(nMax/100.));
}

int EffectLayer::GetFormula14(BYTE bSkillKind, BYTE bSkillLevel, BYTE bMasterySkillKind, BYTE bMasterySkillLevel)
{
	int nMasteryMin = GetEffectInfo(bMasterySkillKind)->Value[bMasterySkillLevel].nMin;
	int nMin = GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMin;
	return nMasteryMin+nMin;
}

float EffectLayer::GetFormula15(BYTE bSkillKind, BYTE bSkillLevel, float fOriginalParam)
{
	int nMin = GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMin;
	return fOriginalParam+nMin;
}

int EffectLayer::GetFormula16(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass)
{
	//스킬.min * BaseClassSkill.Max / 100
	Effect* pEffect = GetEffectInfo(bSkillKind);
	return (int)(pEffect->Value[bSkillLevel].nMin*GetBaseClassSkillMax(wClass, bSkillKind)/100.f);
}

int EffectLayer::GetFormula19(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass)
{
	//스킬.max * BaseClassSkill.Max / 100
	return (int)(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMax*GetBaseClassSkillMax(wClass, bSkillKind)/100.f);
}

int EffectLayer::GetFormula24(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass)
{
	//스킬.max * BaseClassSkill.Max / 100
	return (int)(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nCompass*GetBaseClassSkillMax(wClass, bSkillKind)/100.f);
}

int EffectLayer::GetFormula25(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass)
{
	//스킬.dur * BaseClassSkill.Max / 100
	Effect* pEffect = GetEffectInfo(bSkillKind);
	return (int)(pEffect->Value[bSkillLevel].nDuration*GetBaseClassSkillMax(wClass, bSkillKind)/100.f);
}

float EffectLayer::GetFormula23(BYTE bSkillLevel)
{
	return (float)(min((bSkillLevel+1)/2+1, 20));
}

float EffectLayer::GetFormula8(BYTE bSkillKind, BYTE bSkillLevel, float fOriginaParam)
{	
	int nMin = GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMin;
	return (float)(fOriginaParam*nMin/100.);
}

int EffectLayer::GetFormula9(BYTE bSkillKind, BYTE bSkillLevel, BYTE bOwnLevel, BYTE bTargetLevel, BYTE bSummonMasteryLevel)
{
	//스킬의성공확률 * (시전자 레벨 + 서몬마스터리/30) / 당하는놈 레벨
	int nMin = GetEffectInfo(bSkillKind)->Value[bSkillLevel].nMin;
	return nMin * (bOwnLevel+bSummonMasteryLevel/30/bTargetLevel);
}

BOOL EffectLayer::IsSuccessByFormula0(BYTE bSkillKind, BYTE bSkillLevel)
{
#ifdef DEV_MODE
	return 1;
#endif
	int nPercent = rand() % 100;
	return nPercent < min(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nProbability, 100);
}

BOOL EffectLayer::IsSuccessByFormula6(BYTE bSkillKind, BYTE bSkillLevel)
{
	int nPercent = rand() % 100;
	return nPercent < min(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nCompass, 100);
}

BOOL EffectLayer::IsSuccessByFormula1(WORD wAA, WORD wDP, WORD wBL, DWORD dwOffenseLevel, DWORD dwDefenceLevel)
{
	BOOL bSuccess = FALSE;

	// 공격자AA / ( 방어자DP + 공격자AA ) * 공격자 레벨 / 방어자 레벨
	int iAvoidPercent = ( (wAA * 100 ) / ( wDP + wAA )
					* dwOffenseLevel / dwDefenceLevel );
	
	if( iAvoidPercent > 95 ) iAvoidPercent = 95;

	int iRand = rand()%100;
	
	bSuccess = 	iRand < iAvoidPercent;
	if (!bSuccess)
	{		
		if( wBL > iRand )
		{	
			//block
			bSuccess = -1;			
		}
	}
	return bSuccess;
}

BOOL EffectLayer::IsSuccessByFormula2(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOffenseLevel, DWORD dwDefenceLevel)
{
	//최종성공확률 = 스킬 테이블 성공확률 * 시전자 레벨  / (시전자 레벨 + 당하는놈 레벨)
	int iRand = rand()%100;
	return (DWORD)iRand < min(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nProbability*dwOffenseLevel/(dwOffenseLevel+dwDefenceLevel), 100);
}

BOOL EffectLayer::IsSuccessByFormula5(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSummonMasterySkillLevel, DWORD dwOffenseLevel, DWORD dwDefenceLevel)
{
	//스킬의성공확률 * (시전자 레벨 + 서몬마스터리/30) / 당하는놈 레벨
	int iRand = rand()%100;
	int nSummonMasteryValue = 0;
	if ((char)bSummonMasterySkillLevel > -1)
		nSummonMasteryValue  = GetEffectInfo(__SKILL_SUMMONMASTERY__)->Value[bSummonMasterySkillLevel].nMin;

	return (DWORD)iRand < min(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nProbability*(dwOffenseLevel+nSummonMasteryValue/30)/dwDefenceLevel, 100);
}

BOOL EffectLayer::IsSuccess(BYTE bSkillKind, BYTE bSkillLevel, EffectDesc* pEffectDesc_Pray)
{		
	if (GetEffectInfo(bSkillKind)->bCrime == CRIME_APPLY_ENEMY && pEffectDesc_Pray )
	{
		//프레이 확률 * { 프레이레벨 * 2 / (프레이 레벨 + 걸려있는 스킬레벨) }
		int nRand = pEffectDesc_Pray->pEffect->Value[pEffectDesc_Pray->bSkillLevel].nProbability*(pEffectDesc_Pray->bSkillLevel*2/(pEffectDesc_Pray->bSkillLevel+bSkillLevel));
		return nRand > (rand() % 100);
	}
	else
	{
		int nPercent = rand() % 100;
		return nPercent < min(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nProbability, 100);
	}		
}

BOOL EffectLayer::IsSuccessByFormula13(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSenderLevel, BYTE bRecverLevel)
{
	int nPercent = rand() % 100;
	return nPercent < min(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nProbability*bSenderLevel/(bSenderLevel, bRecverLevel), 100);
}

BOOL EffectLayer::IsSuccessByFormula9(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSummonMasteryLevel, BYTE bSenderLevel, BYTE bRecverLevel)
{
	//스킬의성공확률 * (시전자 레벨 + 서몬마스터리/30) / 당하는놈 레벨
	int nSummonMastery = 1;
	if (bSummonMasteryLevel)
		nSummonMastery = GetEffectInfo(__SKILL_SUMMONMASTERY__)->Value[bSummonMasteryLevel-1].nMin;

	int nPercent = rand() % 100;
	return nPercent < min(GetEffectInfo(bSkillKind)->Value[bSkillLevel].nProbability*(bSenderLevel+nSummonMastery/30.)/bRecverLevel, 100);
}

BYTE EffectLayer::GetSkillMasteryKind(BYTE bSkillKind)
{
	return BYTE(bSkillKind/30*30+__SKILL_MANAMASTERY__);
}

int EffectLayer::GetBaseClassSkillMax(WORD wClass, BYTE bSkillKind)
{
	int nBaseClassSkillMax = 0;
	switch(GetEffectInfo(bSkillKind)->wProperty/100)
	{
	case CLASS_TYPE_WARRIOR:
		nBaseClassSkillMax = g_pBaseClassInfo[wClass].iMax_Aura;
		break;
	case CLASS_TYPE_PRIEST:
		nBaseClassSkillMax = g_pBaseClassInfo[wClass].iMax_Divine;
		break;
	case CLASS_TYPE_SUMMONER:
		nBaseClassSkillMax = g_pBaseClassInfo[wClass].iMax_Summon;
		break;
	case CLASS_TYPE_HUNTER:
		nBaseClassSkillMax = g_pBaseClassInfo[wClass].iMax_Chakra;
		break;
	case CLASS_TYPE_WIZARD:
		nBaseClassSkillMax = g_pBaseClassInfo[wClass].iMax_Magic;
		break;
	}
	return nBaseClassSkillMax;
}

int EffectLayer::GetUsedSPSkill(CUser* pOwnUser, SKILLDESC* pSkillDesc)
{
	int nUseMana = 0;
	Effect* pEffect = GetEffectInfo(pSkillDesc->bSkillKind);
	
	//>>>> user의 mp깍아 주기.
	if (pEffect->bType == TYPE_ACTIVE || pEffect->bType == TYPE_TIMEZERO )
	{
		if (pEffect->bID == __SKILL_AURARECHARGE__)
		{
			pSkillDesc->dwSkillKeepTime = g_dwTickCount-pOwnUser->m_dwTemp[USER_TEMP_CASTINGSTARTTICK];
			nUseMana = (int)(pEffect->Value[pSkillDesc->bSkillLevel].nMin*(pSkillDesc->dwSkillKeepTime/1000.));			
		}
		else
		{
			if (pEffect->bID == __SKILL_LIFEEXPLOSION__)
				nUseMana = -(INT)pOwnUser->GetSP();	// 전부다 사용해야 한다.
			else
				nUseMana = -pEffect->Value[pSkillDesc->bSkillLevel].nMana;
		}
	}
	else if (pEffect->bType == TYPE_DRIVE)
	{
		pSkillDesc->dwSkillKeepTime = g_dwTickCount - pOwnUser->m_dwTemp[USER_TEMP_CASTINGSTARTTICK];
		nUseMana = (int)(((pOwnUser->GetMaxSP()*0.1)*(pSkillDesc->dwSkillKeepTime/1000.)));
		
		nUseMana = min((int)pOwnUser->GetSP(), nUseMana);
		pSkillDesc->dwSkillKeepTime = 
			(DWORD)(nUseMana/pEffect->Value[pSkillDesc->bSkillLevel].nMana*pOwnUser->GetODC()*1000);

		if ((INT)pOwnUser->GetSP() <= nUseMana)
			nUseMana = -(INT)pOwnUser->GetSP();
		else
			nUseMana = -nUseMana;		
	}
	return nUseMana;
}

BOOL EffectLayer::MessyProcessForSystem(CUser* pOwnUser, SKILLDESC* pSkillDesc)
{
	if (!pOwnUser)
		return FALSE;
	
	// 쏘는 주체는 아이템이나 시피지만 그 사건이 캐릭터로 부터 발생된것이라면.. 유저가 쏜것처럼 바꿔준다.	
	pOwnUser = pSkillDesc->pMonsterMaster;
	if (!IsUnitStatusReadySkill(pSkillDesc->bSkillKind, pOwnUser))
		return FALSE;
	
	pSkillDesc->bOwnType = BYTE(pOwnUser->GetObjectType());
	pSkillDesc->dwOwnIndex = pOwnUser->GetID();		

	return TRUE;
}


BOOL EffectLayer::MessyProcessForUser(CUser* pOwnUser, SKILLDESC* pSkillDesc)
{
	if (!IsUnitStatusReadySkill(pSkillDesc->bSkillKind, pOwnUser))
		return FALSE;
		
	assert(pSkillDesc->bSkillLevel < MAX_SKILL_LEVEL);

	if (pSkillDesc->bSkillLevel < 0)
	{
		pOwnUser->SendSkillCastingFail(SKILL_CASTING_FAIL_REASON_LOW);
		return FALSE;
	}

	if (pOwnUser->m_enGodMode)		
	{
		pOwnUser->SendSkillCastingFail(SKILL_CASTING_FAIL_REASON_LOW);
		return FALSE;
	}

	int nMana = GetUsedSPSkill(pOwnUser, pSkillDesc);		
	if (nMana < 0 && (INT)pOwnUser->GetSP() < -nMana)
	{
		pOwnUser->SendSkillCastingFail(SKILL_CASTING_FAIL_REASON_LACK_SP);
		return FALSE;
	}

	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pSkillDesc->bSkillKind);
	if (pEffect->bType == TYPE_DRIVE && !pOwnUser->OverDriveChk())	
	{
		pOwnUser->SendSkillCastingFail(SKILL_CASTING_FAIL_REASON_LACK_ZODIAC_ITEM);
		return FALSE;
	}

	pOwnUser->m_dwStartSkillTick[pSkillDesc->bSkillKind] = g_dwTickCount;
	pOwnUser->SetSP(pOwnUser->GetSP()+nMana);

	// 보정도 할겸 보내주자. ㅡ.ㅡ
	DSTC_USER_STATUS UserStatus;	
	UserStatus.bStatusMany								= 0;
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MP;
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= pOwnUser->GetSP();
	
	NetSendToUser( pOwnUser->m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize()
		, FLAG_SEND_NOT_ENCRYPTION);

	return TRUE;
}


BOOL EffectLayer::IsRange(SKILLDESC* pSkillDesc, VECTOR2* pV2Start)
{
	Effect* pEffect = GetEffectInfo(pSkillDesc->bSkillKind);
	
	float fDist = CalcDistance( pV2Start, &pSkillDesc->v2OwnObjectPos );

	if( pEffect->dwRange && fDist > pEffect->dwRange )
	{
		if (pSkillDesc->bOwnType == OBJECT_TYPE_PLAYER)
		{
			CUser*	pOwnUser = g_pUserHash->GetData(pSkillDesc->dwOwnIndex);
			pOwnUser->SendSkillCastingFail(SKILL_CASTING_FAIL_REASON_OVER_RANGE);
		}
		return FALSE;
	}

	return TRUE;
}

BOOL EffectLayer::IsUnitStatusReadySkill(BYTE bySkillKind, const CUnit* pUnit)
{
	if (!pUnit)
		return FALSE;
	else if (bySkillKind != __SKILL_TREEAGION__ && pUnit->GetUnitStatus() == UNIT_STATUS_DEAD)
		return FALSE;			
	else if (NULL == pUnit->GetCurDungeon())
		return FALSE;
	else if (pUnit->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() == DUNGEON_TYPE_VILLAGE 
				&& !pUnit->GetCurLayerIndex())
		return FALSE;

	return TRUE;
}

BYTE EffectLayer::GetAppliedTargetCount(CDungeonLayer* pDungeonLayer
					, SKILLDESC* pSkillDesc
					, SKILL_TARGETINFO* pTargetInfo
					, VECTOR2* pV2Start)
{
	PARAM_TARGETCOUNT param_target;
		
	// 영역에 들어있는놈들 타일정보로 가져오기.

	param_target.bSkillKind		= pSkillDesc->bSkillKind;
	param_target.bSkillLevel	= pSkillDesc->bSkillLevel;
	param_target.pDungeonLayer	= pDungeonLayer;
	param_target.pMaster		= pSkillDesc->pMonsterMaster;
	param_target.pTargetInfo	= pTargetInfo;
	param_target.vecDest		= &pSkillDesc->v2OwnObjectPos;
	param_target.vecStart		= pV2Start;
	param_target.dwTargetIndex	= pSkillDesc->dwTargetIndex;
	param_target.bTargetType	= pSkillDesc->bTargetType;
	
	return (BYTE)GetTargetCount(&param_target);
}

void EffectLayer::RevisionStartPositon(SKILLDESC* pSkillDesc, VECTOR2* pV2Start)
{
	pV2Start->x = 0.f; pV2Start->y = 0.f;
	Effect* pEffect = GetEffectInfo(pSkillDesc->bSkillKind);
	
	if (pEffect->dwRange == 0)// 자기 위치에서 나가라.
	{
		pSkillDesc->v2OwnObjectPos = GetSkillStartPosition(
											pSkillDesc->bOwnType
											, pSkillDesc->dwOwnIndex
											, pSkillDesc->wTileIndex_X
											, pSkillDesc->wTileIndex_Z
											);

		pSkillDesc->bTargetType = pSkillDesc->bOwnType;
		pSkillDesc->dwTargetIndex = pSkillDesc->dwOwnIndex;

		// 마우스 찍은곳
		switch (pEffect->bSkillType)
		{
		case TARGETTYPE_ARC:
		case TARGETTYPE_LINE:
		
			pV2Start->x = TILE_SIZE*pSkillDesc->wTileIndex_X;
			pV2Start->y = TILE_SIZE*pSkillDesc->wTileIndex_Z;
			break;
		default:
			*pV2Start = pSkillDesc->v2OwnObjectPos;
		}		
	}
	else
	{
		*pV2Start =	GetSkillStartPosition(pSkillDesc->bTargetType
						, pSkillDesc->dwTargetIndex
						, pSkillDesc->wTileIndex_X
						, pSkillDesc->wTileIndex_Z
						);
	}
}

CUnit* EffectLayer::GetValiidUnit(SKILLDESC* pSkillDesc)
{
	CUnit*	pOffenseUnit = NULL;
	
	CDungeonLayer* pDungeonLayer = pSkillDesc->pDungeonLayer;
	if( !pDungeonLayer ) 
		return NULL;
	
	if (pSkillDesc->bOwnType == OBJECT_TYPE_PLAYER)
	{
		pOffenseUnit = g_pUserHash->GetData(pSkillDesc->dwOwnIndex);
		if (!MessyProcessForUser((CUser*)pOffenseUnit, pSkillDesc))
			return NULL;
	}
	else if (pSkillDesc->bOwnType == OBJECT_TYPE_MONSTER)
	{
		pOffenseUnit	= g_pMonsterTable->GetMonsterABS(pSkillDesc->dwOwnIndex);
		if(!pOffenseUnit)
			return NULL;
	}
	else if (pSkillDesc->bOwnType == OBJECT_TYPE_SKILL)
	{
		if (pSkillDesc->pMonsterMaster)
		{
			// 쏘는 주체는 아이템이나 시피지만 그 사건이 캐릭터로 부터 발생된것이라면.. 유저가 쏜것처럼 바꿔준다.	
			pOffenseUnit = pSkillDesc->pMonsterMaster;
			if (!MessyProcessForSystem((CUser*)pOffenseUnit, pSkillDesc))
				return NULL;
		}
	}

	return pOffenseUnit;
}

BOOL EffectLayer::IsSkillSuccess(SKILLDESC* pSkillDesc, CUnit* pOffense, CUnit* pDefense, SKILL_RESULT* pSkill_result)
{
	BOOL bSkillSuccess = FALSE;

	if (OBJECT_TYPE_MONSTER == pDefense->GetObjectType())
	{
		CMonster* pMonster = (CMonster*)pDefense;
		if (pSkillDesc->bOwnType == OBJECT_TYPE_PLAYER)
		{
			if (!IsSkillUserMon(pSkillDesc->bSkillKind, (CUser*)pOffense, pMonster))
				return FALSE;
			
			bSkillSuccess = SkillResult_Unit_Unit(pOffense
								, pMonster
								, pSkillDesc->bSkillKind
								, pSkillDesc->bSkillLevel
								, pSkillDesc->dwSkillKeepTime
								, pSkillDesc->wDamageMinMax
								, pSkill_result
							);

			//만약 이 몬스터가 비선공 몬스터이고 타겟없이 방황하고 있는넘이면 타겟을 셋팅해준다. 
			if(	pMonster->GetAI_FirstAttack() == AI_FIRST_ATTACK_NONE 
				&& !pMonster->GetUnitForAI())
			{
				pMonster->ChangeTargetObject(pOffense);
			}
			
			//무조건 타겟 변경해야 하는 몬스터이면 공격성공여부에 상관없이 타겟을 바꿔준다. 
			if( pMonster->GetAI_TargetChangeType() == AI_CHANGE_TARGET_TYPE_ABSOLUTELY_CHANGE)
			{
				pMonster->ChangeTargetObject(pOffense);
			}										
		}
		else if (pSkillDesc->bOwnType == OBJECT_TYPE_MONSTER)
		{
			if (!IsSkillMonMon(pSkillDesc->bSkillKind, (CMonster*)pOffense, pMonster))
				return FALSE;
			
			bSkillSuccess = SkillResult_Unit_Unit(pOffense
								, pMonster
								, pSkillDesc->bSkillKind
								, pSkillDesc->bSkillLevel
								, pSkillDesc->dwSkillKeepTime
								, pSkillDesc->wDamageMinMax
								, pSkill_result
							);

			// 데토네이션을 몬스터가 사용해 자폭한것은 맞았을때의 ai반응을 제외시켜라.
			// 힐 맞은 것도 안친다 : 최덕석 2005.2.14
			if (__SKILL_DETONATION_MONSTER__ != pSkillDesc->bSkillKind 
			&&  __SKILL_HEAL__ != pSkillDesc->bSkillKind)
			{
				//만약 이 몬스터가 비선공 몬스터이고 타겟없이 방황하고 있는넘이면 타겟을 셋팅해준다. 
				if(	pMonster->GetAI_FirstAttack() == AI_FIRST_ATTACK_NONE 
					&& !pMonster->GetUnitForAI() )
				{
					pMonster->ChangeTargetObject(pOffense);
				}
				
				//무조건 타겟 변경해야 하는 몬스터이면 공격성공여부에 상관없이 타겟을 바꿔준다. 
				if( pMonster->GetAI_TargetChangeType() == AI_CHANGE_TARGET_TYPE_ABSOLUTELY_CHANGE)
				{
					pMonster->ChangeTargetObject(pOffense);
				}
			}
		}
		else if (pSkillDesc->bOwnType == OBJECT_TYPE_SKILL)
		{
			if (!IsSkillMon(pSkillDesc->bSkillKind, pMonster))
				return FALSE;
			
			if (pSkillDesc->pMonsterMaster)
			{
				bSkillSuccess = SkillResult_System_Mon(
									pSkillDesc->pMonsterMaster->GetID()
									, pMonster
									, pSkillDesc->bSkillKind
									, pSkillDesc->bSkillLevel
									, pSkillDesc->dwSkillKeepTime
									, pSkillDesc->wDamageMinMax
									, pSkill_result
								);
			}
			else
			{
				bSkillSuccess = SkillResult_System_Mon(0
									, pMonster
									, pSkillDesc->bSkillKind
									, pSkillDesc->bSkillLevel
									, pSkillDesc->dwSkillKeepTime
									, pSkillDesc->wDamageMinMax
									, pSkill_result
								);
			}
		}
	}
	// 플레이어만 적용되는 스킬이다.
	else if (OBJECT_TYPE_PLAYER == pDefense->GetObjectType())
	{					
		CUser*	pTargetUser	= (CUser*)pDefense;

		if (pSkillDesc->bOwnType == OBJECT_TYPE_PLAYER)
		{
			
			if (!IsSkillUserUser(pSkillDesc->bSkillKind
					, (CUser*)pOffense
					, pTargetUser
					, (BYTE)pSkillDesc->bPK))
			{
				return FALSE;
			}
			
								
			bSkillSuccess = SkillResult_Unit_Unit(pOffense
								, pTargetUser
								, pSkillDesc->bSkillKind
								, pSkillDesc->bSkillLevel
								, pSkillDesc->dwSkillKeepTime
								, pSkillDesc->wDamageMinMax
								, pSkill_result
							);					
		}
		else if (pSkillDesc->bOwnType == OBJECT_TYPE_MONSTER)
		{
			
			if (!IsSkillMonUser(pSkillDesc->bSkillKind, (CMonster*)pOffense, pTargetUser))
				return FALSE;
			
			bSkillSuccess = SkillResult_Unit_Unit(pOffense
								, pTargetUser
								, pSkillDesc->bSkillKind
								, pSkillDesc->bSkillLevel
								, pSkillDesc->dwSkillKeepTime
								, pSkillDesc->wDamageMinMax
								, pSkill_result
							);
		}
		else if (pSkillDesc->bOwnType == OBJECT_TYPE_SKILL)
		{
			if (!IsSkillUser(pSkillDesc->bSkillKind, pTargetUser))	
				return FALSE;
								
			bSkillSuccess = SkillResult_System_User(pTargetUser
								, pSkillDesc->bSkillKind
								, pSkillDesc->bSkillLevel
								, pSkillDesc->dwSkillKeepTime
								, pSkillDesc->wDamageMinMax
								, pSkill_result
							);
		}
	}
	return bSkillSuccess;
}

extern void DieOnGuildWar(CDungeonLayer *pLayer, CUser* pDieUser);

void EffectLayer::AfterSkillSuccessProcess(SKILLDESC* pSkillDesc, CUnit* pOffense, CUnit* pDefense, DSTC_USER_STATUS* pUserStatus)
{
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(pSkillDesc->bSkillKind);

	if (pDefense->GetObjectType() == OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster = (CMonster*)pDefense;
		
		switch (pSkillDesc->bSkillKind)
		{
			// 스킬 예외 처리.					
		case __SKILL_BASH__:
			{
				// 엘리멘탈, 가디언도 베쉬 영향 받도록 : 최덕석 2005.3.7
				WORD wMinMax[2] = {0, 0};
				if (g_pEffectLayer->IsSuccessByFormula0(
										pSkillDesc->bSkillKind
										, pSkillDesc->bSkillLevel))
				{
					SystemSkillToMonster(pSkillDesc->pDungeonLayer
						, __SKILL_PRESSURE__, pSkillDesc->bSkillLevel
						, 0, &pSkillDesc->v2OwnObjectPos
						, wMinMax, pSkillDesc->bOwnType, pSkillDesc->dwOwnIndex
						, pMonster);
				}
			}
			break;
		case __SKILL_VAMPIRE__:
			{
				int nPlusHP = pEffect->Value[pSkillDesc->bSkillLevel].nMax;
				if (pSkillDesc->bOwnType == OBJECT_TYPE_PLAYER)
				{
					pOffense->SetHP((WORD)(pOffense->GetHP()+nPlusHP));
					if (nPlusHP)
					{
						pUserStatus->bStatusMany	= 0;
						pUserStatus->pStatus[pUserStatus->bStatusMany].dwCode	= USER_HP;
						pUserStatus->pStatus[pUserStatus->bStatusMany++].dwMin	= pOffense->GetHP();
						NetSendToUser( pOffense->GetConnectionIndex()
							, (char*)pUserStatus
							, pUserStatus->GetPacketSize()
							, FLAG_SEND_NOT_ENCRYPTION 
							);
					}
				}
				else if (pSkillDesc->bOwnType == OBJECT_TYPE_MONSTER)
				{
					pOffense->SetHP(pOffense->GetHP()+nPlusHP);
				}
				else if (pSkillDesc->bOwnType	== OBJECT_TYPE_SKILL)
				{
					if (pSkillDesc->pMonsterMaster)
					{
						pSkillDesc->pMonsterMaster->SetHP(
							(WORD)(pSkillDesc->pMonsterMaster->GetHP()+nPlusHP));
						if (nPlusHP)
						{
							pUserStatus->bStatusMany	= 0;
							pUserStatus->pStatus[pUserStatus->bStatusMany].dwCode	= USER_HP;
							pUserStatus->pStatus[pUserStatus->bStatusMany++].dwMin	= pSkillDesc->pMonsterMaster->GetHP();
							NetSendToUser( pSkillDesc->pMonsterMaster->m_dwConnectionIndex
								, (char*)pUserStatus
								, pUserStatus->GetPacketSize()
								, FLAG_SEND_NOT_ENCRYPTION 
								);
						}
					}
				}
			}
			break;
							
		}

		if(!pMonster->GetHP())	// 몬스터는 죽었다.
		{						
			if(pSkillDesc->bOwnType == OBJECT_TYPE_PLAYER)
				MonsterKillByUser( (CUser*)pOffense, pMonster );
			else if (pSkillDesc->bOwnType == OBJECT_TYPE_MONSTER)
				MonsterKillByMonster( (CMonster*)pOffense, pMonster );
			else
				pMonster->SetStatus( UNIT_STATUS_DEAD );
			
			SetBadAction(pOffense, pMonster);
			UpdateMonsterForAI( pMonster );
		}
	}
	if (pDefense->GetObjectType() == OBJECT_TYPE_PLAYER)
	{
		CUser* pTargetUser = (CUser*)pDefense;
		
		switch (pSkillDesc->bSkillKind)
		{
		case __SKILL_VAMPIRE__:
			{
				int nPlusHP = pEffect->Value[pSkillDesc->bSkillLevel].nMax;
				if (pSkillDesc->bOwnType == OBJECT_TYPE_PLAYER)
				{
					pOffense->SetHP((WORD)(pOffense->GetHP()+nPlusHP));
					if (nPlusHP)
					{
						pUserStatus->bStatusMany	= 0;
						pUserStatus->pStatus[pUserStatus->bStatusMany].dwCode	= USER_HP;
						pUserStatus->pStatus[pUserStatus->bStatusMany++].dwMin	= pOffense->GetHP();

						NetSendToUser( pOffense->GetConnectionIndex()
							, (char*)pUserStatus
							, pUserStatus->GetPacketSize()
							, FLAG_SEND_NOT_ENCRYPTION 
							);
					}
				}
				else if (pSkillDesc->bOwnType == OBJECT_TYPE_MONSTER)
				{
					pOffense->SetHP(pOffense->GetHP()+nPlusHP);
				}
				else if (pSkillDesc->bOwnType	== OBJECT_TYPE_SKILL)
				{
					if (pSkillDesc->pMonsterMaster)
					{
						pSkillDesc->pMonsterMaster->SetHP((WORD)(pSkillDesc->pMonsterMaster->GetHP()+nPlusHP));
						if (nPlusHP)
						{
							pUserStatus->bStatusMany	= 0;
							pUserStatus->pStatus[pUserStatus->bStatusMany].dwCode	= USER_HP;
							pUserStatus->pStatus[pUserStatus->bStatusMany++].dwMin	= pSkillDesc->pMonsterMaster->GetHP();
							
							NetSendToUser( pSkillDesc->pMonsterMaster->m_dwConnectionIndex
								, (char*)pUserStatus
								, pUserStatus->GetPacketSize()
								, FLAG_SEND_NOT_ENCRYPTION 
							);
						}
					}
				}

			}break;					
		}

		if (!pTargetUser->GetHP())
		{
			if(pOffense && pTargetUser)
			{
				if (pOffense->GetLord())
					((CUser*)pOffense->GetLord())->GuildMemberKill(pTargetUser);
				else 
					((CUser*)pOffense)->GuildMemberKill(pTargetUser);
				// 죽었구만.
				
				CDungeon	*pDungeon = pOffense->GetCurDungeon();
				CDungeonLayer *pLayer = pOffense->GetCurDungeonLayer();

				if(	pLayer//레이어가 있고
				&&	!pLayer->OnDie( (CUser*)pOffense, (CUser*)pDefense) )//길드전 사망처리가 죽이면 안된다고 한다. : hwoarang 050202
				{
					if(	pDungeon
					&&	pDungeon->GetDungeonDataEx()->IsStadium() //경기장이고
					&&	pLayer->m_pTeamMatch )//팀매치 매니저가 있으면
					{
						DieOnGuildWar( pLayer, (CUser*)pDefense);
						return;
					}

				} 
				
				pTargetUser->SetStatus( UNIT_STATUS_DEAD );
				pTargetUser->SendItemSkill(ITEM_SKILL_DIE, OBJECT_TYPE_PLAYER, pOffense);
			}
		}
	}
}

void EffectLayer::SendSkill(SKILLDESC* pSkillDesc)
{
	//>>> goto를 이용하기 위해선 여기에 변수를 선언해야 한다.
	CUnit*	pOffenseUnit = GetValiidUnit(pSkillDesc);
	if (pSkillDesc->bOwnType != OBJECT_TYPE_SKILL && !pOffenseUnit)
		return;
		
	VECTOR2 vecStart= {0.f, 0.f};
	RevisionStartPositon(pSkillDesc, &vecStart);
	if (!IsRange(pSkillDesc, &vecStart))
		return;

	DSTC_SKILL			packet;	
	DSTC_USER_STATUS	UserStatus;	

	SKILL_TARGETINFO	pTargetInfo[MAX_SKILL_TARGET_COUNT];
	memset(pTargetInfo, 0, sizeof(pTargetInfo)); // 일단 이넘이 의심가서 memset 시켰다.

	SKILL_RESULT		skill_result;
	memset(&skill_result, 0, sizeof(skill_result));

	int nCount = GetAppliedTargetCount(pSkillDesc->pDungeonLayer, pSkillDesc, pTargetInfo, &vecStart);
		
	for(int i = 0; i < min(nCount, MAX_SKILL_TARGET_COUNT); ++i)
	{
		if (IsSkillSuccess(pSkillDesc, pOffenseUnit, pTargetInfo[i].pUnit, &skill_result))
		{
			AfterSkillSuccessProcess(pSkillDesc, pOffenseUnit, pTargetInfo[i].pUnit, &UserStatus);
			if (skill_result.byType == 4 && skill_result.nDamage > 0)
				SetBadAction(pOffenseUnit, pTargetInfo[i].pUnit);
			
			packet.sTargetInfo[packet.bTargetCount].bTargetType		= BYTE(pTargetInfo[i].pUnit->GetObjectType());
			packet.sTargetInfo[packet.bTargetCount].dwTargetIndex	= pTargetInfo[i].pUnit->GetID();
			packet.sTargetInfo[packet.bTargetCount].dwCurHP			= pTargetInfo[i].pUnit->GetHP();
			packet.sTargetInfo[packet.bTargetCount].dwDamage		= skill_result.nDamage;
			packet.sTargetInfo[packet.bTargetCount++].bType			= skill_result.byType;
		}
	}
	
	packet.dwDestTime	= pSkillDesc->dwSkillKeepTime;
	packet.bSkillKind	= pSkillDesc->bSkillKind;
	packet.bSkillLevel	= pSkillDesc->bSkillLevel;
	packet.vecStart		= vecStart;
	packet.dwOwnIndex	= pSkillDesc->dwOwnIndex;
	packet.bOwnType		= pSkillDesc->bOwnType;
	pSkillDesc->pDungeonLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), pSkillDesc->bSectionNum );
}