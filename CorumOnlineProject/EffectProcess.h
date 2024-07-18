#pragma once

//#include "../4DyuchiGRX_Common/typedef.h"
#include "ObjectDefine.h"
class CMonster;
class EffectDesc;
struct TARGETINFO;
class CUser;
class CMainUser;

void PlayerAttackFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void PlayerDamageFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void PlayerSkillFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void PlayerKillFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void MonsterSkillFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void MonsterMoveFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void MonsterAttackFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag);
void MonsterDamageFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void MonsterKillFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void MonsterDisappear( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );

void EffectOnceAndNormal( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectOnceAndHideFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectOnceAndRemoveFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectEventDungeonFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectGuardianKillFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectChainAttackerFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillUserAroundRemoveFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectPotionAroundRemoveFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillUserStatusTopFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillUserStatusCenterFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillUserStatusBottomFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillMonsterStatusTopFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillMonsterStatusCenterFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillMonsterStatusBottomFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectAlphaFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSnowFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillAroundFunc( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSkillCameraQuake(EffectDesc* pEffectDesc, DWORD dwCurFrame);

void MonsterKillWithAction( GXOBJECT_HANDLE handle);
void MonsterKillWithAction1( GXOBJECT_HANDLE handle);
void MonsterKillWithAction2( GXOBJECT_HANDLE handle);
void MonsterKillWithAction3( GXOBJECT_HANDLE handle);

void DragonicFireblast( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void FireBallWithEightDirection(DWORD dwPara);
void FireBallHeadFunctionBySlowboat( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void FireBallHeadFunctionBySlowboatEightDirection( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void SetHoamingDirection(VECTOR3* vTarget, VECTOR3* vMine, HANDLE hHandle, float fpower, DWORD dwRandom,EffectDesc*	pEffectDesc);
EffectDesc* SkillSubProcess_FireBall(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo, float fRad );
EffectDesc* SkillSubProcess_BlastQuake(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo );
EffectDesc* SkillSubProcess_General(BYTE bSkillKind, BYTE bSkillLevel, BYTE bJoint, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo );

EffectDesc* SkillSubProcess_IceWall(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo );
EffectDesc* SkillSubProcess_MagmaWall(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOwnType, DWORD dwOwnIndex, DWORD dwCount, VECTOR3 vecStart, TARGETINFO* pTargetInfo );
EffectDesc*	CreateEffectBall(BYTE bSkillKind, float fRadian, DWORD dwOwnType, DWORD dwOwnIndex);
void EffectWall( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectBlastQuake( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void EffectSubCommonProcess(EffectDesc* pEffectDesc);	// 각각 스킬펑션은 다르지만 공통 처리하는것이 있다.
void SkillDamageFunc1( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void SkillDamageFunc2( GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag );
void MonsterSKillDamageWithAction1( GXOBJECT_HANDLE handle);
void MonsterSKillDamageWithAction2( GXOBJECT_HANDLE handle);
void MonsterSKillDamageWithAction3( GXOBJECT_HANDLE handle);
void LightTimeOff(EffectDesc* pEffectDesc);
void LightTimeOnOff(EffectDesc* pEffectDesc);
BOOL SkillEvent(BYTE bLR);
void CreateSnowEffect();
DWORD	GetSkillStartActionFinishCount(BYTE bSkillKind, void* pObject, BYTE bObjectType);
void CameraQuake(VECTOR3& vecQuake);
int		IsSkillMon(BYTE bSkillKind, CMonster* pMonster);
int		IsSkillUser(BYTE bSkillKind, CUser* pUser);
int		IsSkillUserUser(BYTE bSkillKind, CMainUser *pOwnUser, CUser* pTargetUser, BYTE bPK);
int		IsSkillUserMon(BYTE bSkillKind, CMainUser *pOwnUser, CMonster* pTargetMonster);
