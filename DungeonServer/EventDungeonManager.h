#pragma once


class	CUser;
class	CMonster;
class	CDungeon;
class	CDungeonLayer;
struct	EVENT_DUNGEON_PROPERTY;


BOOL QueryEventDungeonProperties();
BYTE GetMyPartyUser(CUser* pUser, CUser** pPartyUser, WORD* pPartyKillMonTotalCount, BOOL bIsMyselfExclusion = FALSE);

void Event_SurvivalDisplay(CUser* pUser);
void Event_TraniningDisplay(CUser** pCommonUser, CUser* pUser, BYTE byUserCount, WORD wPartyKillMonTotalCount, const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty, BOOL bIsParty);


void Event_SurvivalClear(CDungeon* pDungeon);
void Event_TraniningClear(CUser** pCommonUser, CUser* pUser, BYTE byUserCount, const EVENT_DUNGEON_PROPERTY* pEventDungeonProperty, BOOL bIsParty);


void Event_DungeonJoinUser( CUser* pUser, CDungeon* pDungeon );
BOOL Event_IsChangeLayer(CUser* pUser);
void Event_InsertDungeonLayerUser(CUser* pUser, CDungeonLayer* pLayer);
void Event_RemoveDungeonLayerUser(CUser* pUser, CDungeonLayer* pLayer);
void Event_MonsterKillByUser( CUser* pUser, CMonster* pMonster );
void Event_MonsterKillByMonster(CMonster* pAttackMon, CMonster* pDefenseMon);
BOOL Event_GiveMeExp(CUser* pUser, DWORD dwExp);
void Event_SetPartyID(CUser* pUser);
void Event_DisconnectUser(CUser* pUser);
void Event_FinishMapLoading(CUser* pUser);
BOOL Event_IsClearEvent(CUser* pUser);