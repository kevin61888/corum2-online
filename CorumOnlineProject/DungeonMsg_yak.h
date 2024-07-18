#pragma once

#include	"struct.h"

void		InitPacketProcDungeonYak();


void		CmdUpdateGuardianItem( char* pMsg, DWORD dwLen );
void		CmdGuardianLevelUP( char* pMsg, DWORD dwLen );
void		CmdSkillStopStatusEffect( char* pMsg, DWORD dwLen );
void		CmdSkill( char* pMsg, DWORD dwLen );
void		CmdApearSkill( char* pMsg, DWORD dwLen );
void		CmdSkill_ChangeLR( char* pMsg, DWORD dwLen );
void		CmdSiegeInfo( char* pMsg, DWORD dwLen );
void		CmdDestroyCP( char* pMsg, DWORD dwLen );
void		CmdAttackMode( char* pMsg, DWORD dwLen );
void		CmdPKCount( char* pMsg, DWORD dwLen );
void		CmdFinishMapLoading( char* pMsg, DWORD dwLen );
void		CmdResponsePotalJoinUser( char* pMsg, DWORD dwLen );
void		CmdAccEntranceAcquisition( char* pMsg, DWORD dwLen );
void		CmdPKMode( char* pMsg, DWORD dwLen );
void		CmdBadAction( char* pMsg, DWORD dwLen );
void		CmdPlayerBadMode( char* pMsg, DWORD dwLen );
void		CmdUsedPotion( char* pMsg, DWORD dwLen );
void		CmdSkillCastingFail( char* pMsg, DWORD dwLen );
void		CmdSkillCasting( char* pMsg, DWORD dwLen );
void		CmdSkillResult( char* pMsg, DWORD dwLen );
void		CmdMoveMonster( char* pMsg, DWORD dwLen );
void		CmdMoveDungeon( char* pMsg, DWORD dwLen );
void		CmdStopDungeon( char* pMsg, DWORD dwLen );
void		CmdAppearDungeon( char* pMsg, DWORD dwLen );
void		CmdDisappearDungeon( char* pMsg, DWORD dwLen );
void		CmdAttack_User_Mon( char* pMsg, DWORD dwLen );
void		CmdAttack_User_User( char* pMsg, DWORD dwLen );
void		CmdAttack_Mon_Mon( char* pMsg, DWORD dwLen );
void		CmdMonsterCongestionMode( char* pMsg, DWORD dwLen );
void		CmdGuardianEggSummonStatus( char* pMsg, DWORD dwLen );
void		CmdAppearMonster( char* pMsg, DWORD dwLen );
void		CmdDisAppearMonster( char* pMsg, DWORD dwLen );
void		CmdKillMonster( char* pMsg, DWORD dwLen );
void		CmdStopMonster( char* pMsg, DWORD dwLen );
void		CmdAttack_Mon_User( char* pMsg, DWORD dwLen );
void		CmdUserStatus( char* pMsg, DWORD dwLen );
void		CmdUserSkillLevel( char* pMsg, DWORD dwLen );
void		CmdGuardianStatus( char* pMsg, DWORD dwLen );
void		CmdOriginationItemWithEffectKind(char* pMsg, DWORD dwLen);
void		CmdAppendixGuardianInfo(char* pMsg, DWORD dwLen);