#pragma once


#include "PathFinding\\BlockLineManager.h"
#include "Unit.h"
#include "Linkedlist.h"
#include "MonsterManager.h"


#define GUARDIAN_TYPE_NORMAL		1		// 유저가 소환한
#define GUARDIAN_TYPE_DUNGEON		2		// 던전을 수호하기 위한.

#define MONSTER_CLASS_BEAST			1
#define MONSTER_CLASS_UNDEAD		2
#define MONSTER_CLASS_DEMON			3
#define MONSTER_CLASS_PLANT			4
#define MONSTER_CLASS_ELEMENTAL		5
#define MONSTER_RACE_BOSS			7
#define	MONSTER_RANK_DRAGON			9

#define NPC_AIFLAG_SLEEP			1
#define NPC_AIFLAG_DEEPSLEEP		2

#define MAX_AI_SEARCH_TYPE_NUM	4				// AI가 오브젝트를 찾는 방식의 갯수
#define	MAX_AI_DEFENSE_TYPE_NUM	5				// AI가 데미지 먹은후 행동 하는 펑션


#define AI_KIND_MOVETYPE			1		// 움직이는 동안에 AI
#define	AI_KIND_SEARCHTYPE			2		// 검색하는 AI
#define	AI_KIND_ATTACKTYPE			3		// 공격할때의 AI
#define	AI_KIND_DEFENSETYPE			4		// 방어할때의 AI
#define MAX_ASTRT_CURVE				20

struct MAP_TILE;
class CSection;
struct BASEMONSTER;
class CItem;
class CClassMemoryPool;

enum MONSTER_TEMP
{
	NPC_TEMP_DAMAGE				=0	,	// 몬스터가 마지막으로 받은 데미지이다. AI에서 사용한다.
	NPC_TEMP_DAMAGETICK			=1	,	// 마지막으로 기억한 데미지의 틱.
	NPC_TEMP_FINDUSER			=2	,	// 유저를 찾을까? 말까?
	NPC_TEMP_TARGET				=3	,	// 나의 타켓은 누구인가.? 유저? 몬스터? 둘다? 
	NPC_TEMP_AIFLAG				=4	,	// 이 플래그가 0일떄만 AI가 가능하다.
	NPC_TEMP_CREATETYPE			=5	,	// 이것이 0이면 스폰되는 몬스터 이것이 1이면 스폰되는 몬스터가 아니다.
	NPC_TEMP_SKILLMANY			=6	,	// 이것이 0이면 스킬을 쓸수 없는넘이다.
	NPC_TEMP_ITEM				=7	,	// 만약 이것이 0이 아니라면 이 몬스터가 죽었을시 이 아이템이 떨어진다.
	NPC_TEMP_CONGESTION_MODE	=8	,	// 폭주모드; 만약 이것이 0이 아니라면 폭주모드가 발동한 시점의 Tick이다. 

	NPC_TEMP_MOVE				=10	,	// 움직임의 방법 플래그.
	NPC_TEMP_AIKIND				=11	,	// AI의 대상 몬스터냐 유저냐.
	NPC_TEMP_SLOT				=12	,	// 가디언이라면 유저의 몇번쨰 슬롯인가.
	NPC_TEMP_PEACE				=13,
	NPC_TEMP_DUNGEONGUARDIAN	=14	,	// 만약 내가 가디언이라면 주인이 있는 가디언이냐 던젼 가디언이냐.
	NPC_TEMP_LASTATTACKTICK		=15,	// 마지막으로 공격한 틱
	NPC_TEMP_LASTSKILLTICK		=16,	// 마지막으로 스킬사용한 틱
	NPC_TEMP_LASTSTATUSTICK		=17,	// 마지막으로 상태 변한 틱
	NPC_TEMP_KILLTICK			=18,	// 죽었을때 틱.
	MAX_NPC_TEMP	
};
enum	AI_DEFENSE_ENUM
{
	AI_DEFENSE_TEMP_DAMAGE = 0,
};

// 길찾기 결과를 가지고 있을 구조체이다. Client와 다르다.
struct A_STAR
{
	DWORD	dwCurveNum;							// 길찾기 중에서 방향이 달라지는 지점의 개수.
	DWORD	dwCurCurve;							// 현재까지 진행된 커브.

	BlockLocation	pLocation[MAX_ASTRT_CURVE];	// 커브할곳의 위치값을 가진 배열.
};

struct AI_DEFENSE_PARAM
{
	BYTE bObjectType;
	const void* pObject;
	DWORD dwTemp[10];
};

typedef struct GUARDIAN_CREATION_INFO
{
	CUser*		pUser;			// 부모가될 유저의 정보.
	CDungeon*	pDungeon;		// 둘중에 하나는 NULL.
	BYTE		bZipCode;		// 유저의 가디언 배열에서의 인덱스.
	BOOL		bUserGuardian;
	CItem*		pItem;
} * LPGUARDIAN_CREATION_INFO;


#pragma pack( push, 1 )
struct UPDATE_GUARDIAN
{
	DWORD	dwLevel;
	DWORD	dwExp;
	DWORD	dwHP;
	DWORD	dwMP;
	WORD	wEgo;
	WORD	wStr;
	WORD	wInt;
	WORD	wDex;
	WORD	wVit;
	WORD	wStatPoint;
	MONSTER_SKILL monsterSkill[5];
	DWORD	dwID;

};
typedef struct GUARDIAN_INFO
{
	DWORD	dwID;
	char	szName[50];
	DWORD	dwKind;
	DWORD	dwLevel;
	DWORD	dwExp;
	DWORD	dwHP;
	DWORD	dwMP;
	BYTE	bOldGuardian;	// 구가뎐?
	WORD	wStatPoint;
	WORD	wEgo;
	WORD	wStr;
	WORD	wInt;
	WORD	wDex;
	WORD	wVit;
	MONSTER_SKILL monsterSkill[5];
} * LPGUARDIAN_INFO;
#pragma pack( pop )

class CMonster : public CUnit					// 가디언도 몬스터 클래스를 같이쓴다.
{
	WORD				m_wInfoIndex;			// 이몬스터가 생성된 정보를 가지고 있는 인포배열의 인덱스
	MAP_TILE*			m_pPrevTile;			//바로 이전 타일 포인터 
	VECTOR2				m_v2DestPos;			// 목적 좌표
	CSection*			m_pCurSection;			// 현재 몬스터가 있는섹션.
	POSITION_			m_pLayerListNode;		//방 유저 리스트에서 빠른 삭제를 위한 노드 포인터 
	POSITION_			m_pPrevSectionNode;		//섹션의 유저리스트에서의 빠른 삭제를 위한 노드 포인터 위치
	DWORD				m_dwSerial_NO;			// 가디언일떄 디비의 고유번호.
	CUnit*				m_pUnitForAI;			// AI의 대상이 되는 유닛.
	DWORD				m_dwAI;
	float				m_fSight;				// 몬스터의 시야(Cm).
	float				m_fAttackRange;			// 공격권..
	short				m_wAll_resist;
	char				m_szGuardianName[50];
	DWORD				m_dwTemp[MAX_NPC_TEMP];	// 0 - 9까지는 몬스터 가디언 공용. 10-19까지는 가디언만.
	A_STAR				m_PathFinder;
	BASEMONSTER*		m_pBaseMonster;			// 몬스터일떄의 베이스 정보.
	POSITION_			m_pResponListPos;
	SKILLDESC			m_SkillDesc;			// 스킬 쏠수 있는 패킷
	VECTOR2				m_v2TargetAIPos;		// 타겟의 위치
	VECTOR2				m_v2TargetAIDir;
	MONSTER_SKILL		m_MonsterSkill[MAX_MONSTER_USE_SKILL];
	BOOL				m_bSpeedUp;				// 몬스터가 두배속 모드인가? 가디언용 : 최덕석
	BOOL				m_bCongestionMode;		// 몬스터가 폭주모드 : 최덕석
	
protected:
	// AI
	void*				FindUser_Guardian();				// 가디언용.

	void				SearchUser();						// FindUser로 적당한 유저를 찾기위해 여기저기 돌아다닌다.
	void				SearchUser_Guardian();

	BOOL				PathFind(const VECTOR2* v2Dest = NULL );
	void				SetResist();
	void				SetAllResist();
	BOOL				IsAlliance(const CUser* pUser);
	BOOL				IsAlliance(const CMonster* pMonster);

	void				(CMonster::*AI)();
	void				AI0();
	void				AI1();
	void				AI2();
	void				AI3();
	void				AI4();		// For Guardian.....
	void				AI5();		// For Dungeon Guardian.....
	void				AI6();		// FOr Special Monster.....
	void				AI8();		// 보호자(유에린 류) : 최덕석 2005.2.7
	void				AI9();		// 아메리타트 : 최덕석 2005.1.26
	void				AI_New();

	// AI에 의한 수행 함수.
	void				SpeedUp(float fRate);
	void				SpeedNormal();
	void				Move(float fRunningRate = 1.0f);
	BOOL				AttackAfterMove();	//kk
	void				Attack();
	void				Damage();
	void				Delay();
	
	// AI에 의한 조건에 따른 유저 찾기 함수 펑션
	void*				(CMonster::*AI_FindObject[MAX_AI_SEARCH_TYPE_NUM])(BYTE bObjectType);
	void*				AI_FindObjectNear(BYTE bObjectType);	// 가장 근처에 있는 놈
	void*				AI_FindObjectNearAndFewHP(BYTE bObjectType);	// 근처에 있고 hp가 적은 놈
	void*				AI_FindObjectAttachDetectAndNearAndFewHP(BYTE bObjectType);// 디텍트 기능이 있는놈이다.
	void*				AI_FindObjectLeastHP(BYTE bObjectType);		//HP가 가장적은놈 
	
	// ai에 의한 조건에 따른 데미지 먹은 후에 함수 펑션
	void				(CMonster::*AI_DefenseType[MAX_AI_DEFENSE_TYPE_NUM])(const AI_DEFENSE_PARAM* pParam);
	void				AI_DefenseNormal(const AI_DEFENSE_PARAM* pParam);						// 다른놈에게 맞아도 타겟변화는 일어 나지 않는다.
	void				AI_DefenseRecovery(const AI_DEFENSE_PARAM* pParam);					// 다른놈에게 맞을때 스킬 10~12번을 사용해 나의 상태를 복구시켜라.
	void				AI_Defense_ChangeTargetToFierceObject(const AI_DEFENSE_PARAM* pParam);	// 다른놈에게 맞을때 더 사나운놈한테 타겟을 바꺼로
	
public:

	static void* operator new(size_t size);
	static void operator delete(void* pDeleteMonster);

	virtual void	Update();
	virtual void	DetachSkill(EffectDesc* pEffectDesc);	// 상태 스킬을 띄어라.
	virtual void	RemoveResource();
	virtual void	CreateResource();
	virtual void	UpdateMaxHP();
	virtual void	UpdateMaxSP();
	virtual void	UpdateAttackSpeed();
	virtual void	UpdateAttackAcuracy();
	virtual void	UpdateBlockRate();
	virtual void	AddExp(int iExp, BOOL bAlphaExp = TRUE);
	virtual void	UpdateFireResist();				// Resist
	virtual void	UpdateIceResist();
	virtual void	UpdateLightResist();
	virtual void	UpdatePoiResist();
	virtual void	UpdatePhyResist();
	virtual void	UpdateAvoid();
	virtual void	UpdateAD();
	
	virtual int		GetExpDying();
	virtual void	ReSetStatusFromSkillStatusValue(EffectDesc* pEffectDesc);
	virtual void	SetStatusFromSkillStatusValue(BYTE bSkillKind
						, BYTE bSkillLevel
						, WORD wClass
						, float* pResetValue
						, BOOL bUserStatusSend = FALSE);
	virtual void	SetDamageOver(const CUnit* pUnit, DWORD dwDamage);
	virtual BOOL	IsAlliance(const CUnit* pUnit);
	virtual BOOL	AttachSkill(BYTE bOwnType
						, DWORD dwOwnIndex
						, BYTE bSkillKind
						, BYTE bSkillLevel
						, WORD wClass
						, DWORD& dwDestTime
						, DWORD& dwAlphaDamage);
	virtual WORD	GetOriginalAttackAcuracy() const;
	virtual WORD	GetOriginalAvoid() const;
	virtual void	UpdateMoveSpeed();
	virtual void	LevelUP();
	virtual BOOL	IsBadAction(CUnit* pDefenseUnit);
	virtual void	SetCurDungeonLayer(CDungeonLayer* const pDungeonLayer);
	virtual short	GetAllResist() const;
	virtual USER_PROPERTY_CODE		SetDamage(DWORD dwOffenseLevel, DWORD dwDamage );
	virtual void	SetHP(DWORD dwHP, const CUnit* pAttacker = NULL);
	virtual BOOL	ChangeTargetObject(const CUnit* pAttackUser, int nAttackDemage= -1);
	virtual const char*	GetName() const;
	virtual void	SendStopPacket(ENUM_SEND_MODE eSendMode) const;
	virtual void	UpdateAllStatus();
	virtual void	SendAllStatus();
	virtual BYTE	GetSkillLevel(const BYTE bySkillKind) const;
	
	virtual bool	OnDie();
	
	WORD				GetOriginalAD_Min() const;
	WORD				GetOriginalAD_Max() const;
	DWORD				GetOriginalMaxHP() const;
	void				UpdateSight();
	WORD				GetInfoIndex()					{ return m_wInfoIndex; }
	void				SetInfoIndex( WORD wInfoIndex ) { m_wInfoIndex = wInfoIndex; }
	void				SetStatusStop();										// 현재 몬스터의 상태를 정지로 만든다.
	BOOL				SetPathFindMove(BYTE bMonsterStatus);										// 길찾기 결과를 순차적으로 적용한다.
	BOOL				SetMonsterAI( DWORD dwAI );								// 몬스터의 AI 함수 포인터를 연결한다.
	void				SetStatus( BYTE	bStatus, BOOL bClearTarget = FALSE, BOOL bCompulsion = FALSE);								// 몬스터의 상태를 설정한다.
	void				ClearTargetAI();
	void				ScheduleMoveDestPos(const VECTOR2* pDestPos );
	void				SubExp( int iExp );
	void				DisAppearUnit( DWORD dwUnitID );						// 접속을 끊었거나 할때 인자로 들어온 인덱스 값이 몬스터가 잡고 있는 유저의 것이라면 NULL을 만든다.
	int					GetDropItem(int nMagicItemRate, int iRate);
	BYTE				GetAIType(BYTE bAIKind);

	//Packet..
	void				SendMovePacket();
	void				SendRunningPacket();
	void				SendWalkingPacket();
	void				SendAttackPacket();
	BOOL				SendSkillPacket(BYTE bSkillArrayIndex, BOOL bSkillTick = TRUE);
	void				SkillToMonster(BYTE bSkillKind
							, BYTE bSkillLevel
							, const CMonster* pTargetMonster
							, const WORD* pwMinMax);// bSkillKind : 스킬 종류, bSkillLevel : 스킬 레벨, pTargetMonster : 스킬맞을놈
	void				SkillToUser(BYTE bSkillKind
							, BYTE bSkillLevel
							, const CUser* pTargetUser);// bSkillKind : 스킬 종류, bSkillLevel : 스킬 레벨, pTargetUser : 스킬맞을놈
	void				SkillToTile(BYTE bSkillKind
							, BYTE bSkillLevel
							, const MAP_TILE* pTile);// bSkillKind : 스킬 종류, bSkillLevel : 스킬 레벨, pTile : 스킬맞을놈
	
	
	BOOL				IsElemental(VOID) const;
	BOOL				IsMindControll(VOID) const; 
	BOOL				IsGuardian(VOID) const;
	BOOL				IsNormalMonster() const;
	BOOL				IsNPC() const;

	BOOL				IsCongestionMode() const;	
	void				CheckCongestionMode(BOOL bOnCheck);	//True이면 조건을 따져보고 폭주모드를 On 시키고 
															//False이면 조건을 따져봐서 폭주모드를 Off 시킨다.			
	BYTE				GetAI_FirstAttack() const;
	BYTE				GetAI_TargetChangeType() const;
	float				GetAI_MinHPRateForCongestionMode() const;
	float				GetAI_RunningRate() const;
	DWORD				GetAI_MonsterIDForCongestionMode() const;
	WORD				GetAI_CongestionTime() const;
	void				SetTargetAI(const CUnit* pUnit);
	void				RemoveCurDungeonLayer(BOOL bSendDisapper = FALSE);
	void				SetDestPosition(const VECTOR2* v2Dest);
	const VECTOR2*		GetDestPosition() const;
	CSection*			GetCurSection() const;
	void				RemoveCurSection();
	void				SetCurSection(CSection* pSection);
	DWORD				GetTemp(MONSTER_TEMP eMonster_Temp) const;
	void				SetTemp(MONSTER_TEMP eMonster_Temp, DWORD dwValue);
	void				AddResponList();
	void				RemoveResponList();
	void				SetBaseMonsterInfo(BASEMONSTER* pBaseMonster);
	const BASEMONSTER* 	GetBaseMonsterInfo() const;
	void				SetGuardianDBID(DWORD dwGuardianDBID);
	DWORD				GetGuardianDBID() const;
	void				SetName(char* pszName);
	CUnit*				GetUnitForAI() const;
	void				ReSearchPath();
	void				Initialize(
									BASEMONSTER* pBaseMonster
								  , GUARDIAN_INFO* pGuardian
								  , CUser* pLord
								  , BYTE byZipCode
								  , DWORD dwLevel
								  , DWORD dwExp
								  );
	void				SetSkillLevel(BYTE bySkillKind, BYTE bySkillLevel);
	const MONSTER_SKILL*		GetMonsterSkillPtr() const;
	void*				FindObject(BYTE bSearchType);			// 적당한 유저를 찾아 m_pUserForAI에 넣는다. AI1 AI2

	void  GetAttackSkillDamage(BYTE bySelectedSkill, BYTE bSkillLevel, WORD* pwAttackDamageMin, WORD* pwAttackDamageMax) const;
	void  GetAttackDamageByFormula1(float& fDamageMin, float& fDamageMax, BYTE bSkillKind) const;
	void  GetAttackDamageByFormula2(float& fDamageMin, float& fDamageMax, BYTE bSkillKind, BYTE bSkillLevel) const;
	void  GetAttackDamageByFormula20(float& fDamageMin, float& fDamageMax, BYTE bSkillKind) const;
	CMonster();
	~CMonster();

public:
	
	CMonster*			m_pPrevData;
	CMonster*			m_pNextData;
};
	

void UpdateUserForAI( CUser* pUser );
void UpdateMonsterForAI( CMonster* pMonster );
