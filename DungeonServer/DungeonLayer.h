#pragma once


#include "GameSystem.h"

#include "BaseTeamMatch.h"// : hwoarang 050202


class CDungeon;
class CMap;
class CSection;
class Sw;
struct LAYER_FORMATION;
struct GUARDIAN_INFO;
class CVoidList;
class CPcList;
class CMonsterList;
class CCreateItemList;
class CActiveSectionList;
class CUser;
class CMonster;


#pragma pack(push, 1)
struct Monster_Spot	
{
	WORD	wMonsterID;
	WORD	wPosX;
	WORD	wPosZ;
};

struct Monster_Info
{
	BOOL			bLive;				// 살아 있는가.
	DWORD			dwCreateTick;		// 죽었을떄 생성 지연시간.
	DWORD			dwKillTime;			// 죽었을때의 시각.	
	float			fAttackSight;		// 공격을위해 유저에게 다가가야하는 거리.
	float			fBound;
	Monster_Spot	Spot;				// 생성되는 좌표와 종류.
};

struct Move_Spot						// 케릭터가 특정 장소로 이동시 다른곳으로 이동시긴다. 위층 아래층..
{
	WORD	wStartSpotX;
	WORD	wStartSpotZ;
	RECT	rcMoveSpot;
	int		wDestLayerID;
	int		wDestLayerMoveSpotNum;		// 대상 레이어의 어느 곳으로 이동할것인가.
};
#pragma pack(pop)


class CDungeonLayer
{

	WORD					m_wTotalSectionNum;							// 맵에 있는 총 Section의 갯수 
	WORD					m_wTotalMonsterInfoNum;						// 몬스터 정보의 총개수.
	CMap*					m_pMap;										// 맵
	CDungeon*				m_pParent;									// 부모 던젼 포인터 
	CSection*				m_pSection[MAX_SECTION_NUM_PER_LAYER];		// 맵 Section 포인터 

public:	

	LAYER_FORMATION*		m_pLayerFormation;
	CActiveSectionList*		m_pActiveSectionList;			// 현재 섹션이 Active된 상태인가를 나타내는 링크리스트 
	CMonsterList*			m_pMonsterList;				
	CMonsterList*			m_pResponseMonsterList;		
	CPcList*				m_pPcList;						// PC List
	CCreateItemList*		m_pCreateItemList;
	CVoidList*				m_pDeadMonsterList;				// 죽은 몬스터들			
	LPITEM_HASH				m_pItemHash;					// ItemHash
	BYTE					m_bLayer;						// 현재 층수 
	WORD					m_wTotalMonster;				// 소환된 몬스터를 제외한 몬스터들의 카운트.
	WORD					m_wCurMonster;
	WORD					m_wTotalUserCount;
	WORD					m_wStartSpotNum;
	RECT*					m_pStartSpot;
	WORD					m_wMoveSpotNum;
	Move_Spot*				m_pMoveSpot;
	CDungeonLayer*			m_pPrvInfo;			
	CDungeonLayer*			m_pNextInfo;	
	Sw*						m_pSw;
	DWORD					m_dwLastFrameSec;
	DWORD					m_dwLastOwnerFrameSec;

	CBaseTeamMatch*			m_pTeamMatch;

	//PVP 매치 플래그
	BOOL					m_bMatching;					// 대전 : TRUE,  비대전 : FALSE 
	DWORD					m_dwMatchBeginTime;				// 대전 시작 틱 
	DWORD					m_dwMatchEndTime;				// 대전 종료 틱 ( m_dwMatchBeginTime + m_dwMatchTick )

	BOOL					m_bReadyMatchNum;				// 대전 준비된 유저수 
	BOOL					m_bStartMatch;					// 대전 시작
	
	DWORD					m_dwMatchTick;					// 대전시간 (특정한 틱)
	DWORD					m_dw30SecBeforeEndMatchTick;	// 대전 끝나기 30초전
	DWORD					m_dwEndMatchJudgmentTick;		// 대전 종료후 5초간 이러저러한 일들 을 한다.
	DWORD					m_dw10SecEndMatchLayerTick;		// 대전이 종료되고 10초후에 대전 종료 플레그를 세팅한다. 

	

public:
	void			Create(DWORD dwID, BYTE bLayer, CDungeon* pParent);
	void			Destroy();	
	void			InitMatch();
	BOOL			InsertUser( CUser* pUser, const VECTOR2* pv2Start );
	BOOL			InsertUserSection( CUser* pUser, WORD wSectionNum );
	BOOL			InsertUserSection( CUser* pUser );					
	BOOL			RemoveUserSection( CUser* pUser );					
	BOOL			RemoveUser( CUser* pUser );							
	BOOL			InsertMonster( CMonster* pMonster, const VECTOR2* v2Start  );
	BOOL			InsertMonsterSection( CMonster* pMonster, WORD wSectionNum );
	BOOL			RemoveMonsterSection( CMonster* pMonster );
	BOOL			RemoveMonster( CMonster* pMonster );	
	BOOL			InsertItem( ITEM_SERVER* pItem );
	BOOL			RemoveItem( ITEM_SERVER* pItem );
	void			Process();											
	void			InitList();	
	DWORD			BroadCastMsg( char* pMsg, DWORD wLength ) const;
	DWORD			BroadCastMsgExceptSender( char* pMsg, DWORD wLength, const CUser* pSender ) const;
	DWORD			BroadCastSectionMsg( char* pMsg, DWORD wLength, const CUser* pSender ) const;
	DWORD			BroadCastSectionMsg( char* pMsg, DWORD wLength, BYTE bSectionNum ) const;
	DWORD			BroadCastSectionMsgExceptLink( char* pMsg, DWORD wLength, BYTE bSectionNum ) const;				
	DWORD			BroadCastSectionMsgExceptLink( char* pMsg, DWORD wLength, CUser* pSender ) const;
	void			IntersectSection( BYTE bPrevSectionNum, BYTE bCurSectionNum, BYTE* pIntersectSection );		
	void			BroadCastIntersectSectionInfo( BYTE	bPrevSectionNum, BYTE bCurSectionNum, CUser* pUser );	
	void			SendSectionInfo( BYTE bSectionNum, CUser* pUser , BYTE bPortal = 0 );
	CSection*		GetSection(int nSectionNum) const;					
	BOOL			AddActiveSection(int nSectionNum);					
	BOOL			AddActiveSection(CSection* pSection);				
	BOOL			RemoveActiveSection(int nSectionNum);				
	BOOL			RemoveActiveSection(CSection* pSection);			
	BOOL			MoveSection(CUser* pUser , int nFrom, int nTo);		
	BOOL			MoveSection(CMonster* pNpc , int nFrom, int nTo);	
	double			GetDistance(VECTOR2* vFrom, VECTOR2* vTo);			
	BOOL			SetSection(int nSectionNum, CSection* pSection);
	void			SetTotalSectionNum(WORD wTotalSectionNum)	{	m_wTotalSectionNum = wTotalSectionNum;	}
	CMap*			GetMap()	{	return m_pMap;	}
	BYTE			GetDungeonType();	
	DWORD			GetUserCount();
	void			CreateMonsterAll();	
	void			CreateMonster( CMonster* pMonster );
	void			CallGuardian( CUser* pUser, DWORD dwZipCode, GUARDIAN_INFO* pGuardian );
	void			ResetMonster(BOOL bCreateMonster = 1);
	void			ReviveMonster(CMonster* pMonster, DWORD dwFlag = 1);
	CMonster*		CallDungeonGuardian( GUARDIAN_INFO* pGuardian );
	CMonster*		CallMonster( CUser* pUser, DWORD dwZipCode, DWORD dwBaseMonsterNum , const VECTOR2* const v3Start);
	CMonster*		CallMonsterByGM( CUser* pUser, DWORD dwZipCode, DWORD dwBaseMonsterNum, DWORD dwItem );
	void			InitMoveSpot( DWORD dwMoveSpotNum );
	CDungeon*		GetParent()	{	return m_pParent;	}		
	VECTOR2			GetAroundEmptyTile(WORD wIndex_X, WORD wIndex_Z);		
	void			AttachMagicArrayToParty(CUser* pUser, DWORD dwItemID);	
	BOOL			GetStartTile(WORD wStartSpotID, VECTOR2* const pv2Start);

	//팀매치 관련, (아래 TeamMatch 붙은 함수 전부 : hwoarang 050202
	bool			CreateTeamMatch(const FLAG_TYPE dwMatchType);//팀매치 생성
	bool			IsTeamMatchRun() const ;
	bool			IsTeamMatchEnd() const;//팀매치 끝났냐?

	bool			IsTeamMatchPlayer(CUser* pUser) const;

	bool			PrepareTeamMatch(const FLAG_TYPE match_type, const FLAG_TYPE match_level,  const COUNT_TYPE player_count, const COUNT_TYPE team_count, const GUILD_WAR_TIME match_time, const bool bIsAutoMatch);

	bool			OnDie(CUser* pCaster, CUser* pTarget);//사망시

	bool			CanMove();//이동시 가능 불가능

private:

	void			__ProcessTeamMatch();
	void			__ProcessMatch();
	void			__ProcessSiege();
	void			__ProcessUpdateItemOwner();
	void			__ProcessDeleteItem();
	void			__ProcessCreateItem();
	void			__ProcessRespawnMonster();
	void			__ProcessUpdateMonster();
	void			__ProcessDeadMonsterClear();
};
