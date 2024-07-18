#pragma once
#include "Linkedlist.h"
#include "User.h"


#define TILE_SIZE				(float)125.0f
#define MAX_EVENT_DUNGEON_KIND	16

struct DB_CHANGE_DUNGEON;
class CMonster;
class DUNGEON_DATA_EX;
class CDungeonLayer;


#pragma pack(push, 1)
struct EVENT_DUNGEON_PROPERTY
{
	BYTE		byDungeonType;					// 이벤트 던젼의 종류.
	BYTE		byMaxEnterNo;					// 최대 입장객 수.
	BYTE		byStartKillMonsterCount;		// 수련던젼에서 처음 입장했을때 다음레이어로 갈수 있는 몬스터 죽인수 
	BYTE		byFlagParty;					// 파티 전용 던젼인가?
	BYTE		byFlagItemDrop;					// 아이템을 드롭하는 던젼인가?
	BYTE		byFlagFloorDown;				// 아랫층으로 내려가면서 진행하는 던젼인가?
	BYTE		byFlagBattleRoyal;				// 배틀로열 형식 던젼인가?
	BYTE		byFlagMosterKillDown;			// 몬스터를 죽이는 순서대로 내려가는 던젼인가?(수련던전)
	BYTE		byFlagExpStyle;					// 경험치를 획득하는 스타일. 
	BYTE		byMonsterExpRatio;				// 몬스터가 경험치 주는 배율.
	BYTE		byBossExpRatio;					// 보스 몬스터 경험치 주는 배율.
	BYTE		byFailedExpRatio;				// 실패시 빼앗길 경험치의 비율
	DWORD		dwBonusMoney;					// 성공시 획득할 카르츠의 양. 
	BYTE		byBonusExpRatio;				// 성공시 보너스로 획득하는 경험치의 양
	DWORD		dwSpecialConditionStart;		// 특수한 던젼의 층 이동 시작 조건.
	DWORD		dwSpecialCondition;				// 특수한 던젼의 층 이동시 변경되는 조건.	
};
#pragma pack(pop)


class CDungeon
{

protected:	

	BYTE					m_bDungeonKind;									// Dungeon Type (General or Novice)	
	char					m_szDungeonTitle[ MAX_DUNGEON_TITLE_LENGTH ];	// 던전 타이틀
	WORD					m_wID;											// 일반 던전일 경우 던전 고유번호 
	WORD					m_wDungeonUserCount;							// 현재 던전내의 총 유저수 
	DWORD					m_dwEventClearTick;								// 이벤트 던젼 클리어 된 시각.
	DWORD					m_dwEventFirstCloseTick;						// 1차 이벤트 던젼 클리어 된 시각. 
	DWORD					m_dwTemp[DUNGEON_TEMP_MAX];
	POSITION_				m_DungeonListPos;								// DungeonTable에서 DungeonList의 빠른삭제를 위한 DungeonList Node Pointer
	EVENT_DUNGEON_PROPERTY* m_pEventDungeonProperty;						// 이벤트 던젼일 경우 이벤트 던젼의 속성을 담게 된다.
	DB_CHANGE_DUNGEON*		m_pChangeDungeon;								// 가디언을 죽인놈의 정보를 공성 끝날때까지 기억시켜 놓아라.
	CMonster*				m_pDungeonGuardian;
	DUNGEON_DATA_EX*		m_pInfo;										// 던전인포
	CDungeon*				m_pPrevDungeon;
	CDungeon*				m_pNextDungeon;
	CDungeonLayer*			m_pDungeonLayer[ MAX_LAYER_PER_DUNGEON ];
	PARTYUSER_HASH*			m_pPartyUserHash;
	GUILDUSER_HASH*			m_pGuildUserHash;
	CVoidList*				m_pDungeonUserList;
	CVoidList*				m_pDefenceParty;								// 던전 방어 할 각 파티아이디
	CVoidList*				m_pRemovePCList;								// 월드로 나갈 유저 리스트 	

public:

	DWORD							GetTemp(DUNGEON_TEMP eTemp_Kind);
	void							SetTemp(DUNGEON_TEMP eTemp_Kind, DWORD dwValue);
	DWORD							GetEventClearTick() const;
	DWORD							GetEventFirstCloseTick() const;
	void							SetEventFirstCloseTick(DWORD dwEventFirstCloseTick);
	void							SetEventClearTick(DWORD dwEventClearTick);
	DUNGEON_DATA_EX*				GetDungeonDataEx() const;
const EVENT_DUNGEON_PROPERTY* const	GetEventDungeonProperty() const;
	void							SetEventDungeonProperty(EVENT_DUNGEON_PROPERTY* const pEventDungeonProperty);
	PARTYUSER_HASH*					GetPartyUserHash() const;
	GUILDUSER_HASH*					GetGuildUserHash() const;
	void							SetDungeonGuardian(CMonster* const pMonster);
	const CMonster*					GetDungeonGuardian();
	CVoidList*						GetDungeonUserList() const;
	CDungeon*						GetPrevDungeon() const;
	CDungeon*						GetNextDungeon() const;
	void							SetPrevDungeon(CDungeon* const pDungeon);
	void							SetNextDungeon(CDungeon* const pDungeon);
	CDungeonLayer*					GetDungeonLayer(const BYTE byLayerIndex) const;
	CVoidList*						GetRemovePCList()	const;
	BOOL							Build();	
	void							SetDungeonTitle(char* szTitle);
	WORD							GetID()	const						{	return m_wID;	}
	void							Destroy();
	void							SetDungeonID(const WORD wID)		{	m_wID = wID;	}
	void							SetDungeonKind(const BYTE bKind)	{	m_bDungeonKind = bKind;		}
	char*							GetDungeonName();
	char*							GetDungeonTitle()					{	return m_szDungeonTitle;	}
	BYTE							GetDungeonKind()					{	return m_bDungeonKind;		}
	DWORD							GetDungeonTotalUserCount()			{   return m_wDungeonUserCount; }	
	WORD							GetTotalLayer() const;
	WORD							GetDungeonLayerID(BYTE bLayer) const;
	BOOL							SetDungeonInfo(DUNGEON_DATA_EX* pInfo, BYTE bDungeonKind);
	DUNGEON_JOIN_FAIL				JoinUser(CUser* pUser, BYTE bLayerIndex = 0, VECTOR2 *pv2SpecifiedPos = NULL);
	void							ChangeLayerUser( CUser* pUser, BYTE byCurLayerIndex, int nDestLayerIndex, BYTE byDestLayerSpot ) const;
	void							ChangeLayerUser( CUser* pUser, BYTE byCurLayerIndex, BYTE byMoveSpotNum ) const;
	void							ChangeLayerMonster( CMonster* pMonster, int nDestLayerIndex, VECTOR2& v2Dest) const;
	void							InsertUser(CUser* pUser);
	void							RemoveUser(CUser* pUser);
	void							BroadCast(char* pMsg, DWORD dwLength);
	void							BroadCastInCircle(CDungeonLayer* pDungeonLayer, VECTOR2* v2Center, BYTE byRadiusTileCount, char* pMsg, DWORD dwLength);
	void							SendJoinSectionInfo( CUser* pUser , BYTE bPortal = 0 ) const;
	void							SendSiegeInfo(CUser* pUser) const;				
	void							SetDungeonListPos(POSITION_ Pos)	{	m_DungeonListPos = Pos;	}
	POSITION_						GetDungeonListPos();
	void							GotoLobbyUser(CUser* pUser) const;
	BOOL							CreateDungeonGuardian();
	BOOL							RemoveDungeonGuardian();
	void							DistinctionAttackMode();					
	void							SetDungeonOwner(DWORD dwUserIndex, DWORD dwGuildID, DWORD dwPartyID, char* pLoginID, const char* const pCharacterName);	
	void							RestartSiege();								
	BOOL							PermissionJoinDungeon(CUser* pUser);	
	void							StartSiege();							
	void							EndSiege();								
	BYTE							GetAttackMode(CUser* pUser) const;		
	void							InsertDefensePartyList(DWORD dwPartyID);
	void							RemoveAllDefencePartyList();			
	void							RemoveUserForInsidePortal( CUser* pUser ) const;		
	BOOL							SendPortalMsg( CUser* pUser, BYTE bDestLayerID, VECTOR2 *pv2Pos, BYTE bChangeDungeon ) const;
	DWORD							JoinUserForPortal( CUser* pUser, BYTE bLayerIndex);
	void							ReviveAllMonster();							
	void							RemoveAllUserMonster( CUser* pUser );
	int								GetEventDungeonType();									
	int								GetEventDungeonLevel();									
	void							ReportUserEnterEventDungeonByPortal(WORD wDungeonID);	
	void							ResetMonster(BOOL bCreateMonster = TRUE);				
	void							BanAllUser();											
	void							AddAllAccumulationExp() const;							
	void							SubAllAccumulationExp();								
	void							PlusAccumulationExp(LONG byPercent) const;				
	void							MinusAccumulationExp(BYTE byPercent);					
	void							EventFirstTimeUpProcess();								
	void							EventClearProcess();									
	void							AllUserPKMode(BOOL bIsPKMode = 1);						
	void							EventTimeUpProcess();									
	
	CDungeon();
	~CDungeon();
};

