// Unit.cpp: implementation of the CUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Unit.h"
#include "Linkedlist.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "Map.h"
#include "DUNGEON_DATA_EX.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "ItemAttrLayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUnit::CUnit()
{
	m_dwDeathTime = 0;
}

CUnit::~CUnit()
{

}

void CUnit::RemoveAllDetachSkill()
{
	if (m_pUsingStatusEffectList)
	{
		POSITION_ pos = m_pUsingStatusEffectList->GetHeadPosition();
		while(pos)
		{
			EffectDesc* pEffectDesc = (EffectDesc*)m_pUsingStatusEffectList->GetNext(pos);
			DetachSkill(pEffectDesc);
		}
	}
}

void CUnit::RemoveAllSkill()
{
	if (m_pUsingStatusEffectList)
	{
		RemoveAllDetachSkill();
		delete m_pUsingStatusEffectList;
	}
	m_pUsingStatusEffectList = NULL;
}

DWORD CUnit::GetID() const
{
	return m_dwIndex;
}

void CUnit::SetID(DWORD dwID)
{
	m_dwIndex = dwID;
}

void CUnit::SetObjectType(GAME_OBJECT_TYPE eObjectType)
{
	m_eObjectType = eObjectType;
}

GAME_OBJECT_TYPE CUnit::GetObjectType() const
{
	return m_eObjectType;
}

CDungeon* CUnit::GetCurDungeon() const
{
	return m_pCurDungeon;
}

CDungeonLayer* CUnit::GetCurDungeonLayer() const
{
	if(m_pCurDungeon == NULL)
		return NULL;

	return m_pCurDungeon->GetDungeonLayer(GetCurLayerIndex());
}

WORD CUnit::GetCurDungeonID() const
{
	assert(m_pCurDungeon);
	
	return m_pCurDungeon->GetID();
}

WORD CUnit::GetClass() const
{
	return m_wClass;
}

WORD CUnit::GetRace() const
{
	return m_wRace;
}

WORD CUnit::GetGrade() const
{
	return m_wGrade;
}

DWORD CUnit::GetHP() const
{
	return m_dwHP;
}

DWORD CUnit::GetSP() const
{
	return m_dwSP;
}

DWORD CUnit::GetExp() const
{
	return m_dwExp;
}

DWORD CUnit::GetLevel() const
{
	return m_dwLevel;
}

DWORD CUnit::GetMaxHP() const
{
	return m_dwMaxHP;
}

DWORD CUnit::GetMaxSP() const
{
	return m_dwMaxMP;
}

DWORD CUnit::GetEgo() const
{
	return m_dwEgo;
}

DWORD CUnit::GetStr() const
{
	return m_dwStr;
}

DWORD CUnit::GetInt() const
{
	return m_dwInt;
}

DWORD CUnit::GetDex() const
{
	return m_dwDex;
}

DWORD CUnit::GetVit() const
{
	return m_dwVit;
}

void CUnit::SetLayerIndex(BYTE byLayerIndex)
{
	m_byLayerIndex = byLayerIndex;
}

void CUnit::SetCurPosition(const VECTOR2* pv2CurPos)
{
	if (!GetCurDungeonLayer())
		return;
	
	CMap*		pMap	= GetCurDungeonLayer()->GetMap();
	MAP_TILE*	pTile	= pMap->GetTile( pv2CurPos->x, pv2CurPos->y );

	// 만약 이동이 가능한 다일이 아니라면.
	if(!pTile || pTile->wAttr.uSection == 0)
	{
		return;
	}

	// 유저이면
	if(GetObjectType() == OBJECT_TYPE_PLAYER)
	{
		CDungeonLayer *pDungeonLayer = GetCurDungeonLayer();
		
		CBaseTeamMatch *pTeamMatch = pDungeonLayer->m_pTeamMatch;
		if( pTeamMatch )
		{
			const FLAG_TYPE target_flag = pTeamMatch->GetTeamMatchFlag(this);
			// 관전자면
			if( (target_flag &	G_W_F_OBSERVER )
			||	(target_flag == G_W_F_NONE ) )
			{
				m_v2CurPos = *pv2CurPos;
				SetCurMapTile(pMap->GetTile( GetCurPosition()->x, GetCurPosition()->y));
				return;	
			} // if( target_flag & G_W_F_OBSERVER )
		}
	}

	if (GetCurMapTile())
	{
		// 타일이 바뀌었는지 체크한다.
		if( ( GetCurMapTile()->wIndex_X != pTile->wIndex_X ) || ( GetCurMapTile()->wIndex_Z != pTile->wIndex_Z ) )
		{
			pMap->SetTileOccupied( GetCurMapTile()->wIndex_X
				, GetCurMapTile()->wIndex_Z
				, TILE_EMPTY
				, this );
		}
	}
	
	switch (GetObjectType())
	{
	case OBJECT_TYPE_PLAYER:
		{
			pMap->SetTileOccupied( pTile->wIndex_X, pTile->wIndex_Z, TILE_OCCUPIED_PC, this );
		}
		break;
	case OBJECT_TYPE_MONSTER:
		{
			pMap->SetTileOccupied( pTile->wIndex_X, pTile->wIndex_Z, TILE_OCCUPIED_MONSTER, this );
		}
		break;
	}

	m_v2CurPos = *pv2CurPos;
	SetCurMapTile(pMap->GetTile( GetCurPosition()->x, GetCurPosition()->y));
}

const VECTOR2* CUnit::GetCurPosition() const
{
	return &m_v2CurPos;
}

MAP_TILE* CUnit::GetCurMapTile() const
{
	return m_pTileCur;
}

WORD CUnit::GetAttackAcuracy() const
{
	return m_wAttackAcuracy;
}

WORD CUnit::GetAvoid() const
{
	return m_wAvoid;
}

WORD CUnit::GetBlockRate() const
{
	return m_wBlockRate;
}

short CUnit::GetFireResist() const
{
	return m_wFireResist;
}

short CUnit::GetIceResist() const
{
	return m_wIceResist;
}

short CUnit::GetLightResist() const
{
	return m_wLightResist;
}

short CUnit::GetPoiResist() const
{
	return m_wPoiResist;
}

short CUnit::GetPhyResist() const
{
	return m_wPhyResist;
}

float CUnit::GetMoveSpeed() const
{
	return m_fMoveSpeed;
}

const WORD* CUnit::GetPtrOfAttackDamageL() const
{
	return m_pwAttackDamage_L;
}

const WORD* CUnit::GetPtrOfAttackDamageR() const
{
	return m_pwAttackDamage_R;
}

WORD CUnit::GetAttackSpeed() const
{
	return m_wAttackSpeed;
}

WORD CUnit::GetCurDungeonLayerID()
{
	assert(GetCurDungeon());

	return (WORD)GetCurDungeon()->GetDungeonDataEx()
		->m_dwLayerFormation[GetCurLayerIndex()];
}

BYTE CUnit::GetCurLayerIndex() const
{
	return m_byLayerIndex;
}

BOOL CUnit::IsCurDungeonSiege()
{
	assert(GetCurDungeon());

	return GetCurDungeon()->GetDungeonDataEx()->m_bSiege;
}

BOOL CUnit::IsCurDungeonVillage()
{
	assert(GetCurDungeon());

	return GetCurDungeon()->GetDungeonDataEx()->IsVillage();
}

UNIT_STATUS CUnit::GetUnitStatus() const
{
	return m_eUnitStatus;
}

BYTE CUnit::GetPrevSectionNum() const
{
	return m_byPrevSectionNum;
}

CVoidList* CUnit::GetUsingStatusEffectList() const
{
	return m_pUsingStatusEffectList;
}

void CUnit::NewUsingStatusEffectList()
{
	if (!m_pUsingStatusEffectList)
		m_pUsingStatusEffectList = new CVoidList;
}

EffectDesc* CUnit::GetEffectDesc(BYTE bySkillKind) const
{
	assert(bySkillKind<__SKILL_ATTACK__);
	
	return m_pEffectDesc[bySkillKind];
}

const VECTOR2* CUnit::GetCurDirection() const
{
	return &m_v2Direction;
}

void CUnit::SetPrevSectionNum(BYTE bySectionNum)
{
	m_byPrevSectionNum = bySectionNum;
}

void CUnit::SetCurDungeon(CDungeon* pDungeon)
{
	m_pCurDungeon = pDungeon;
}

void CUnit::SetCurDungeonLayer(CDungeonLayer* pDungeonLayer)
{
	m_pCurDungeonLayer = pDungeonLayer;	
}

short CUnit::GetAllResist() const
{
	return 0;
}

void CUnit::SetRace(WORD wRace)
{
	m_wRace = wRace;
}

void CUnit::SetLevel(DWORD dwLevel)
{
	m_dwLevel = dwLevel;
}

void CUnit::SetSP(DWORD dwSP)
{
	m_dwSP = min(dwSP, GetMaxSP());
}

BOOL CUnit::IsAvoidSuccess(CUnit* pOffense) const    
{
	// 공격자AA*100 / ( 방어자DP + 공격자AA ) * 공격자 레벨 / 방어자 레벨

//	int iAvoidPercent = (int)( ( pOffense->GetAttackAcuracy() * 100 ) / ( GetAvoid() + pOffense->GetAttackAcuracy() ) * ((pOffense->GetLevel()+10) * 2) / ((GetLevel()+10) *2) );
	
	int iAvoidPercent = (int)(	( pOffense->GetAttackAcuracy() * 100 ) /
								( GetAvoid() + pOffense->GetAttackAcuracy() ) *
								( pow( (float)(pOffense->GetLevel()+10), 2) ) /
								( pow( (float)(GetLevel()+10), 2) ));	
	
	if( iAvoidPercent > 95 )
	{
		iAvoidPercent = 95;
	}

	int iRand = rand() % 100;
	
	return iAvoidPercent < iRand;
}

BOOL CUnit::IsBlockSuccess() const
{
	return  GetBlockRate() > (rand()%100);
}

void CUnit::SetExp(DWORD dwExp)
{
	m_dwExp = dwExp;
}

DWORD CUnit::GetLastMoveTick() const
{
	return m_dwLastMoveTick;
}

void CUnit::SetLastMoveTick(DWORD dwLastTick)
{
	m_dwLastMoveTick = dwLastTick;
}

void CUnit::SetEffectDesc(BYTE bySkillKind, EffectDesc* const pEffectDesc)
{
	assert(bySkillKind < __SKILL_ATTACK__);

	m_pEffectDesc[bySkillKind] = pEffectDesc;
}

float CUnit::GetAlphaStat(USER_PROPERTY_CODE eUserStat) const
{
	assert(eUserStat < USER_MAX_STATUS);
	
	return m_fPlusParam[eUserStat];
}

void CUnit::SetAlphaStat(USER_PROPERTY_CODE eUserStat, float fParamPlus)
{
	assert(eUserStat < USER_MAX_STATUS);
	
	m_fPlusParam[eUserStat] = fParamPlus;
}

void CUnit::SetAttackDamage_L(WORD wDamageMin, WORD wDamageMax)
{
	m_pwAttackDamage_L[0] = wDamageMin;
	m_pwAttackDamage_L[1] = wDamageMax;
}

void CUnit::SetAttackDamage_R(WORD wDamageMin, WORD wDamageMax)
{
	m_pwAttackDamage_R[0] = wDamageMin;
	m_pwAttackDamage_R[1] = wDamageMax;
}

WORD CUnit::GetAttackDamageMin_L() const
{
	return m_pwAttackDamage_L[0];
}

WORD CUnit::GetAttackDamageMax_L() const
{
	return m_pwAttackDamage_L[1];
}

WORD CUnit::GetAttackDamageMin_R() const
{
	return m_pwAttackDamage_R[0];
}

WORD CUnit::GetAttackDamageMax_R() const
{
	return m_pwAttackDamage_R[1];
}

void CUnit::GetAttackingDamage(WORD wAttackDamageMin
								  , WORD wAttackDamageMax
								  , WORD* pwAttackDamageMin
								  , WORD* pwAttackDamageMax
								  , BYTE byLR)
{
	*pwAttackDamageMin = wAttackDamageMin;
	*pwAttackDamageMax = wAttackDamageMax;
}

DWORD CUnit::GetAttackDamageRandomMinMax() const
{
	DWORD dwTmp;
	dwTmp = GetAttackDamageMax_L() - GetAttackDamageMin_L() + 1;
	DWORD iRand  = rand() % dwTmp;
	iRand  += GetAttackDamageMin_L();

	return iRand;
}

void CUnit::SetClass(WORD wClass)
{
	m_wClass = wClass;
}

void CUnit::SetGrade(WORD wGrade)
{
	m_wGrade = wGrade;
}

void CUnit::SetEgo(DWORD dwEgo)
{
	m_dwEgo = dwEgo;
}

void CUnit::SetDex(DWORD dwDex)
{
	m_dwDex = dwDex;
}

void CUnit::SetStr(DWORD dwStr)
{
	m_dwStr = dwStr;
}

void CUnit::SetInt(DWORD dwInt)
{
	m_dwInt = dwInt;
}

void CUnit::SetVit(DWORD dwVit)
{
	m_dwVit = dwVit;
}

void CUnit::SetCurDirection(const VECTOR2* v2Dir)
{
	m_v2Direction = *v2Dir;
}

void CUnit::SetUnitStatus(UNIT_STATUS eUnitStatus)
{
	m_eUnitStatus = eUnitStatus;
}

void CUnit::SetSelectedSkill(SELECT_ATTACK_TYPE eSelectAttackType
							 , BYTE bSkillKind)
{
	m_bySelectedSkill[eSelectAttackType] = bSkillKind;
}

BYTE CUnit::GetSelectedSkill(SELECT_ATTACK_TYPE eSelectAttackType) const
{
	return m_bySelectedSkill[eSelectAttackType];
}

void CUnit::SetAttackMode(ATTACK_MODE eAttackMode)
{
	m_eAttackMode = eAttackMode;
}

ATTACK_MODE CUnit::GetAttackMode() const
{
	return m_eAttackMode;
}

void CUnit::SetLord(CUnit* pUnit)
{
	m_pLordUnit = pUnit;
}

CUnit* CUnit::GetLord() const
{
	if (m_pLordUnit && m_pLordUnit->GetUnitStatus() == UNIT_STATUS_NONE)
		return NULL;
		
	return m_pLordUnit;
}

void CUnit::SetStaff(CUnit* pUnit)
{
	m_pStaffUnit = pUnit;
}

CUnit* CUnit::GetStaff() const
{	
	return m_pStaffUnit;
}

void CUnit::SetCurMapTile(MAP_TILE* pMapTile)
{
	m_pTileCur = pMapTile;
}

void CUnit::SetBad(BOOL bBad
				   , BYTE byCriminalKind
				   , int nValue
				   , BOOL bChk)
{
	
}

void CUnit::SetAttackTypeAndFrame( const CUnit* pUnit, DWORD dwType )
{

}

DWORD CUnit::GetReduceDamageForObject(DWORD dwDamage, CUnit* pAttackUnit)
{
	switch(pAttackUnit->GetObjectType())
	{
	case OBJECT_TYPE_PLAYER:
		if (GetObjectType() == OBJECT_TYPE_PLAYER)
			dwDamage /= 4;
		else if (GetLord() && GetObjectType() == OBJECT_TYPE_MONSTER )
			dwDamage /= 4;

		break;
	case OBJECT_TYPE_MONSTER:
		
		// 주인있는 몹이 유저를 때렷다.
		if (pAttackUnit->GetLord() && GetObjectType() == OBJECT_TYPE_PLAYER)
			dwDamage /= 4;
		// 주인없는 몹이 주인있는 몹을 때렷다.
		else if (!pAttackUnit->GetLord() && GetLord() && GetObjectType() == OBJECT_TYPE_MONSTER )
		{
// #ifndef 로 되있어서 국내랑 일본이랑 반대였음 ㅡ,.ㅡ;
// 이미 국내에 1/4 로 패치 되있어서 다시 돌릴수 없어서 국내 일본 둘다 1/4 로 처리 함.
// jaewon 050122
//#ifndef JAPAN_LOCALIZING		//	일본이 아닌경우에는 데미지를 1/4 로 줄이고, 일본은 그냥 100% 들어가게 한다.
			dwDamage /= 4;
//#endif
		}

		break;
	}
	
	return dwDamage;
}

BOOL CUnit::ChangeTargetObject(const CUnit* pAttackUser, int nAttackDemage)
{
	return FALSE;
}

void CUnit::GetAttackDamage(BYTE bySelectedSkill
			, BYTE bSkillLevel
			, BYTE bWeaponKind
			, WORD* pwAttackDamageMin
			, WORD* pwAttackDamageMax
			, BYTE bLR) const
{

	*pwAttackDamageMin = 0;
	*pwAttackDamageMax = 0;
}


DWORD CUnit::GetConnectionIndex() const
{
	if (GetLord())
		return GetLord()->GetConnectionIndex();
	
	return 0;
}

EffectDesc* CUnit::AllocEffectDesc(BYTE bySkillKind, BYTE bySkillLevel)
{
	assert(bySkillLevel < MAX_SKILL_LEVEL);
	
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bySkillKind);
	EffectDesc* pEffectDesc = new EffectDesc;
	memset(pEffectDesc, 0, sizeof(EffectDesc));
	
	pEffectDesc->pEffect = pEffect;
	pEffectDesc->pos = GetUsingStatusEffectList()->AddHead(pEffectDesc);
	pEffectDesc->dwLastUpdateTick	= g_dwTickCount;		// 스킬이 걸린 시점을 기억한다.		
	SetEffectDesc(bySkillKind, pEffectDesc);
	pEffectDesc->bSkillLevel = bySkillLevel;

#ifdef DEV_MODE
	POSITION_ pos = GetUsingStatusEffectList()->GetHeadPosition();
	while(pos)
	{
		EffectDesc* pDummyEffectDesc = (EffectDesc*)GetUsingStatusEffectList()->GetNext(pos);
		if (pEffectDesc != pDummyEffectDesc && pDummyEffectDesc->pos == pEffectDesc->pos)
			__asm int 3
	}
#endif
	
	return pEffectDesc;
}

void CUnit::FreeEffectDesc(EffectDesc* pEffectDesc)
{
	ReSetStatusFromSkillStatusValue(pEffectDesc);
	SetEffectDesc(pEffectDesc->pEffect->bID, NULL);
	GetUsingStatusEffectList()->RemoveAt(pEffectDesc->pos);
	
	delete pEffectDesc;
}

void CUnit::AddStatusPoint(enum STATUS_POINT_KIND eStatus_Point_Kind)
{
	if(GetNotUseStatPoint()>0)
	{
		SetStatPoint(WORD(GetNotUseStatPoint()-1));

		switch(eStatus_Point_Kind)
		{
		case STATUS_POINT_KIND_EGO:			
			SetEgo(GetEgo()+1);
			
			break;
		case STATUS_POINT_KIND_STR:
			SetStr(GetStr()+1);
			UpdateAD();
			break;
		case STATUS_POINT_KIND_INT:
			SetInt(GetInt()+1);
			UpdateFireResist();
			UpdateIceResist();
			UpdateLightResist();
			UpdatePoiResist();
			UpdatePhyResist();
			break;
		case STATUS_POINT_KIND_DEX:
			SetDex(GetDex()+1);
			UpdateAttackAcuracy();
			UpdateAvoid();
			break;
		case STATUS_POINT_KIND_VIT: 
			SetVit(GetVit()+1);
			UpdateMaxHP();
			break;
		}

		DSTC_STATUS_INFO	pSendPacket;
		pSendPacket.bStatusInfo	 = TRUE;							
		pSendPacket.byStatusType = BYTE(eStatus_Point_Kind);
		pSendPacket.byObjectType = BYTE(GetObjectType());
			
		NetSendToUser(
			  GetConnectionIndex()
			, (char*)&pSendPacket
			, pSendPacket.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION
			);

		SendAllStatus();
	}
}

WORD CUnit::GetNotUseStatPoint() const
{
	return m_wStatPoint;
}

void CUnit::SetStatPoint(WORD wPoint)
{
	m_wStatPoint = wPoint;
}

void CUnit::SetItemAttr(ITEM_ATTR_CODE eRealCode, int nValue, const bool bIsPlus)// : 050111 hwoarang
{
	if(bIsPlus)
	{
		if (eRealCode < MAX_ITEM_KIND1_CODE)
			m_pwItemAttr[g_pItemAttrLayer->GetVirtualCode((WORD)eRealCode, (WORD)nValue)] += nValue;
		else if (eRealCode < MAX_ITEM_KIND2_CODE)
			m_pwItemAttr[g_pItemAttrLayer->GetVirtualCode((WORD)eRealCode, (WORD)nValue)] += g_pItemAttrLayer->GetVirtualValue(eRealCode, (WORD)nValue);
	}
	else
	{
		if (eRealCode < MAX_ITEM_KIND1_CODE)
			m_pwItemAttr[g_pItemAttrLayer->GetVirtualCode((WORD)eRealCode, (WORD)nValue)] -= nValue;
		else if (eRealCode < MAX_ITEM_KIND2_CODE)
			m_pwItemAttr[g_pItemAttrLayer->GetVirtualCode((WORD)eRealCode, (WORD)nValue)] -= g_pItemAttrLayer->GetVirtualValue(eRealCode, (WORD)nValue);
	}
}

short CUnit::GetItemAttr(ITEM_ATTR_CODE eVirtualCode) const
{
	return m_pwItemAttr[eVirtualCode];
}

void CUnit::InitializeItemAttr()
{
	memset(m_pwItemAttr, 0, sizeof(m_pwItemAttr));
}

char CUnit::GetSkillLevelByItem(BYTE bySkillKind) const
{
	char bItemSkillLevel = 0;
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bySkillKind);
	
	switch(pEffect->wProperty/100)
	{
	case CLASS_TYPE_WARRIOR:
		{
			
			const BYTE bAlphastat = static_cast<BYTE>(GetAlphaStat(USER_DEC_SKILLLEVEL) );
			bItemSkillLevel = (BYTE)(	GetItemAttr(ITEM_ATTR_ALLSKILL) +
										GetItemAttr(ITEM_ATTR_AURASKILL) +
										bAlphastat -
										GetItemAttr(ITEM_ATTR_SKILL_LEV_AURA_ALL));
			switch(pEffect->bType)
			{
			case TYPE_TIMEZERO:
			case TYPE_ACTIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_AURA_ACTIVE);
				break;
			case TYPE_DRIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_AURA_OVERDRIVE);
				break;
			case TYPE_PASSIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_AURA_PACIVE);
				break;						
			}					
		}
		break;

	case CLASS_TYPE_PRIEST:
		{
			bItemSkillLevel = (BYTE)(GetItemAttr(ITEM_ATTR_ALLSKILL)
				+GetItemAttr(ITEM_ATTR_DEVINESKILL)
				+GetAlphaStat(USER_DEC_SKILLLEVEL)
				-GetItemAttr(ITEM_ATTR_SKILL_LEV_DIVINE_ALL));

			switch(pEffect->bType)
			{
			case TYPE_TIMEZERO:
			case TYPE_ACTIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_DIVINE_ACTIVE);
				break;
			case TYPE_DRIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_DIVINE_OVERDRIVE);
				break;
			case TYPE_PASSIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_DIVINE_PACIVE);
				break;						
			}					
		}
		break;

	case CLASS_TYPE_SUMMONER:
		{
			
			bItemSkillLevel = (BYTE)(GetItemAttr(ITEM_ATTR_ALLSKILL)
				+GetItemAttr(ITEM_ATTR_SUMMONSKILL)
				+GetAlphaStat(USER_DEC_SKILLLEVEL)
				-GetItemAttr(ITEM_ATTR_SKILL_LEV_SUMMON_ALL));
			switch(pEffect->bType)
			{
			case TYPE_TIMEZERO:
			case TYPE_ACTIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_SUMMON_ACTIVE);
				break;
			case TYPE_DRIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_SUMMON_OVERDRIVE);
				break;
			case TYPE_PASSIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_SUMMON_PACIVE);
				break;
				
			}					
		}
		break;

	case CLASS_TYPE_HUNTER:
		{
			bItemSkillLevel = (BYTE)(GetItemAttr(ITEM_ATTR_ALLSKILL)
				+GetItemAttr(ITEM_ATTR_CHAKRASKILL)
				+GetAlphaStat(USER_DEC_SKILLLEVEL)
				-GetItemAttr(ITEM_ATTR_SKILL_LEV_CHAKRA_ALL));
			switch(pEffect->bType)
			{
			case TYPE_TIMEZERO:
			case TYPE_ACTIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_CHAKRA_ACTIVE);
				break;
			case TYPE_DRIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_CHAKRA_OVERDRIVE);
				break;
			case TYPE_PASSIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_CHAKRA_PACIVE);
				break;
				
			}
			
		}
		break;

	case CLASS_TYPE_WIZARD:
		{
			bItemSkillLevel = (BYTE)(GetItemAttr(ITEM_ATTR_ALLSKILL)
				+GetItemAttr(ITEM_ATTR_MANASKILL)
				+GetAlphaStat(USER_DEC_SKILLLEVEL)
				-GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_ALL));
			switch(pEffect->bType)
			{
			case TYPE_TIMEZERO:
			case TYPE_ACTIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_ACTIVE);
				switch (pEffect->wProperty%10)
				{
				case SKILL_PROPERTY_KIND_FIRE:
					bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_FIRE);
					break;
				case SKILL_PROPERTY_KIND_ICE:
					bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_ICE);
					break;
				case SKILL_PROPERTY_KIND_EARTH:
					bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_EARTH);
					break;
				case SKILL_PROPERTY_KIND_LIGHT:
					bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_LIGHT);
					break;
				case SKILL_PROPERTY_KIND_PHY:
					bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_ACTIVE_EARTH);
					break;
				}
				break;
			case TYPE_DRIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_OVERDRIVE);
				break;
			case TYPE_PASSIVE:
				bItemSkillLevel += GetItemAttr(ITEM_ATTR_SKILL_LEV_MANA_PACIVE);
				break;
				
			}						
		}
		break;
	}	

	return bItemSkillLevel;
}

