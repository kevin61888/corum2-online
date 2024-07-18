#pragma once


#include "GameSystem.h"

float	__stdcall CalcDistance(const VECTOR2* const pv2Start, const VECTOR2* const pv2End);
void	__stdcall VECTOR3_SUB_VECTOR3(VECTOR2* const pv2Result, const VECTOR2* const pv2Arg1, const VECTOR2* const pv2Arg2);
void	__stdcall Normalize (VECTOR2* OUT pv2Result,VECTOR2* IN pv2Arg1);
BOOL	EpsilonVector(const VECTOR2* const v2Vec1, const VECTOR2* const v2Vec2, float fEpsilon );


int		SkillResult_Unit_Unit(CUnit* pUser, CUnit* pMonster, BYTE bSkillKind, BYTE bSkillLevel, DWORD& dwTime, WORD wAttackDamage[2], LPSKILL_RESULT pResult);
int		SkillResult_System_Mon(DWORD dwMonsterMasterIndex, CMonster* pTargetMonster, BYTE bSkillKind, BYTE bSkillLevel, DWORD& dwTime, WORD wMinMax[2], LPSKILL_RESULT pResult);
int		SkillResult_System_User(CUser* pTargetUser, BYTE bSkillKind, BYTE bSkillLevel, DWORD& dwTime, WORD wMinMax[2], LPSKILL_RESULT pResult);


int		GetExp(BYTE bUserLevel, BYTE bMonsterLevel, DWORD dwExp);
DWORD	GetExpByMonster( CMonster* pMonster, DWORD dwPrevHP );	
void	GiveMeExp(CUser* pUser, CMonster* pMonster, DWORD dwExp);
int		InsertDamageTargetObject(PARAM_TARGETCOUNT* pParam, BYTE byCount, BYTE byObjectType, void* pObject);
int		IsCollitionByCircle(CDungeonLayer* pDungeonLayer, VECTOR2* v2Start, const VECTOR2* const v2Object, BYTE byRadius);
int		IsCollitionByBox(CDungeonLayer* pDungeonLayer, VECTOR2* v2Start, const VECTOR2* const v2Object, BYTE byRadius);
int		IsCollitionByArc(CDungeonLayer* pDungeonLayer, VECTOR2* v2Start, float fRad, const VECTOR2* const v2Object, BYTE byRadius);
int		IsCollitionByLine(CDungeonLayer* pDungeonLayer, VECTOR2* v2Start, float fRad, const VECTOR2* const v2Object, BYTE byRadius);
int		GetDamageCircle(PARAM_TARGETCOUNT* pParam);
time_t	GetDueDay(int nEndMin);
DWORD	GetLandTime(int nEndMin);
BOOL	IsOverDueDay(DWORD dwEndDate);
int		GetDamageObject(PARAM_TARGETCOUNT* pParam);
int		GetSummon(PARAM_TARGETCOUNT* pParam);
int		GetDamageBox(PARAM_TARGETCOUNT* pParam);
int		GetDamage8Line(PARAM_TARGETCOUNT* pParam);
int		GetDamageArc(PARAM_TARGETCOUNT* pParam);
int		GetDamageLine(PARAM_TARGETCOUNT* pParam);
VECTOR2	GetSkillStartPosition(BYTE bTargetType, DWORD dwTargetIndex, WORD wTileIndex_X, WORD wTileIndex_Z);
BOOL	IsLinkSection(CDungeonLayer* pDungeonLayer, CSection* pStartingPointSection, CSection* pInnerPointSection);
int		GetTargetCount(PARAM_TARGETCOUNT* pParam);
int		GetPropertyRealDamageByResist(BYTE bProperty, int iDamage,  BYTE bOffenseOjectType, DWORD dwOffenseIndex, BYTE bDefenseObjectType, DWORD dwDefenseIndex);
void	MakeAppearUser(DSTC_APPEAR* pAppear, CUser* pUser, BYTE bPortal);
BOOL	IsAbleUpgradeAriesZodiac(CItem* pItem);
BOOL	IsGrowGuardianItem(CItem* pGuardianItem);
void	SendExchangeGuardianLife(CUser* pMasterUser, int nMinute);
void	ExchangeGuardianLife(CItem* pGuardianItem, int nMinute);
void	SendSummonEggStatus(DWORD dwConnectionIndex, BOOL bSummon, const CItem* pGuardianItem);


int		IsSkillMonMon(BYTE bSkillKind, CMonster *pOwnMonster, CMonster* pTargetMonster);
int		IsSkillMonUser(BYTE bSkillKind, CMonster *pOwnMonster, CUser* pTargetUser);
int		IsSkillUserUser(BYTE bSkillKind, CUser *pOwnUser, CUser* pTargetUser, BYTE bPK);
int		IsSkillUserMon(BYTE bSkillKind, CUser *pOwnUser, CMonster* pTargetMonster);
int		IsSkillMon(BYTE bSkillKind, CMonster* pMonster);
int		IsSkillUser(BYTE bSkillKind, CUser* pUser);


void	SystemSkillToUser(CDungeonLayer* pLayer, BYTE bSkillKind, BYTE bSkillLevel, DWORD dwTime, const VECTOR2* const pvecStartPosition, WORD wMinMax[2], BYTE bOwnType, DWORD dwOwnIndex, CUser* pTargetUser);
void	SystemSkillToMonster(CDungeonLayer* pLayer, BYTE bSkillKind, BYTE bSkillLevel, DWORD dwTime, const VECTOR2* const pvecStartPosition, WORD wMinMax[2], BYTE bOwnType, DWORD dwOwnIndex, CMonster* pTargetMonster);


DWORD	GetGuardianItemLevel(DWORD dwExp);