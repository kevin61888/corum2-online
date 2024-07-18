#if !defined(AFX_UNIT_H__ED6E2260_EFD1_499C_9E63_48B3A2D19D4A__INCLUDED_)
#define AFX_UNIT_H__ED6E2260_EFD1_499C_9E63_48B3A2D19D4A__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "Effect.h"
#include "../CommonServer/ItemManagerDefine.h"


class CVoidList;
class CDungeon;
struct MAP_TILE;


enum SELECT_ATTACK_TYPE
{
	SELECT_ATTACK_TYPE_LBUTTON	=	0,
	SELECT_ATTACK_TYPE_RBUTTON	=	1,
};


enum UNIT_CLASS_ID
{
	UNIT_CLASS_ID_USER				= 1,
	UNIT_CLASS_ID_MONSTER			= 2,
	UNIT_CLASS_ID_TRADER_MONSTER	= 3,
};


enum UNIT_STATUS
{
	UNIT_STATUS_NONE					= 0,
	UNIT_STATUS_NORMAL					= 10,
	UNIT_STATUS_WALKING					= 11,
	UNIT_STATUS_RUNNING					= 12,
	UNIT_STATUS_ATTACKING				= 13,
	UNIT_STATUS_DAMAGING				= 14,
	UNIT_STATUS_SKILL					= 15,
	UNIT_STATUS_CASTING					= 16,			
	UNIT_STATUS_WAITING					= 17,			
	UNIT_STATUS_DEAD					= 18,
	UNIT_STATUS_READY					= 19,
	UNIT_STATUS_PORTAL_MOVING			= 20,
	UNIT_STATUS_PLAYER_SHOP				= 21,
	UNIT_STATUS_PLAYER_REST				= 22,
	UNIT_STATUS_DELAYTIME				= 23,
	UNIT_STATUS_WORLD					= 128,
	UNIT_STATUS_RIDE_AIRPLANE			= 130,
	UNIT_STATUS_RIDE_AIRPLANE_DOWN		= 131,
	UNIT_STATUS_AIRPLANE_DOWNING		= 134,
	UNIT_STATUS_AIRPLANE_FLOATING		= 135,
	UNIT_STATUS_AWAIT_DUNGEON_RESPONSE	= 136,	//던전에 조인하기 위해 던전(월드)서버의 응답을 기다리는중 
};


enum USER_PROPERTY_CODE
{	
	USER_MAXHP					= 1,
	USER_MAXMP					= 2,
	USER_HONOR					= 3,
	USER_EGO					= 4,
	USER_STR					= 5,
	USER_INT					= 6,
	USER_DEX					= 7,
	USER_VIT					= 8,
	USER_LUCK					= 9,
	USER_ATTACK_R				= 10,
	USER_ATTACK_L				= 11,
	USER_AA						= 12,
	USER_AVOID					= 13,
	USER_BLOCKRATE				= 14,
	USER_FIRE_RESIST			= 15,
	USER_ICE_RESIST				= 16,
	USER_LIGHT_RESIST			= 17,
	USER_POI_RESIST				= 18,
	USER_PHY_RESIST				= 19,
	USER_MOVESPEED				= 20,
	USER_EXP					= 21,
	USER_HP						= 22,
	USER_MP						= 23,
	USER_CASTINGTIME			= 24,
	USER_COOLTIME				= 25,
	USER_ATTACKRATE_PERCENT		= 26,
	USER_ATTACKACURACY_PERCENT	= 27,
	USER_OVERDRIVE_DELAY		= 28,
	USER_HEALHPSPEED			= 29,
	USER_HEALMPSPEED			= 30,
	USER_POISONING				= 31,
	USER_DEC_SKILLLEVEL			= 32,
	USER_SKILLATTACK_R_PERCENT	= 33,
	USER_SKILLATTACK_L_PERCENT	= 34,
	USER_CTTIME					= 35,
	USER_ALL_RESIST				= 36,
	USER_MAX_RESIST				= 37,
	USER_MOVESPEED_PERCENT		= 38,
	USER_HEALHPSPEED_PERCENT	= 39,
	USER_PHY_RESIST_PERCENT		= 40,
	USER_MP_PERCENT				= 41,
	USER_MAXHP_PERCENT			= 42,
	USER_ATTACK_R_PERCENT		= 43,
	USER_BLOCKRATE_PERCENT		= 44,
	USER_AVOID_PERCENT			= 45,
	USER_HP_PERCENT				= 46,
	USER_POI_RESIST_PERCENT		= 47,
	USER_ITEM_ATTACK			= 48,
	USER_ITEM_DEFENSE			= 49,
	USER_BLIND					= 50,
	USER_MAXMP_PERCENT			= 51,
	USER_ATTACK_L_PERCENT		= 52,
	USER_DETECTION				= 53,
	USER_MAX_STATUS
};


enum STATUS_POINT_KIND
{
	STATUS_POINT_KIND_EGO = 0,
	STATUS_POINT_KIND_STR = 1,
	STATUS_POINT_KIND_INT = 2,
	STATUS_POINT_KIND_DEX = 3,
	STATUS_POINT_KIND_VIT = 4, 
};


enum ATTACK_MODE
{
	ATTACK_MODE_NONE	= 0,
	ATTACK_MODE_OFFENSE	= 1,		// 공격자
	ATTACK_MODE_DEFENCE	= 2,		// 방어자
};

enum ENUM_SEND_MODE
{
	SEND_MODE_BROADCAST_SECTION				= 1,
	SEND_MODE_BROADCAST_SECTION_EXCEPT_LINK = 2,
	SEND_MODE_BROADCAST_DUNGEONLAYER		= 3,
	SEND_MODE_BROADCAST_EXCEPT_SENDER		= 4,
	SEND_MODE_ONE_PLAYER					= 5
};


class CUnit  
{
	DWORD				m_dwIndex;							// 고유 아이디
	WORD				m_wClass;							// 클래스.
	WORD				m_wRace;							// 종족
	WORD				m_wGrade;
	DWORD				m_dwLevel;
	DWORD				m_dwEgo;				
	DWORD				m_dwStr;				
	DWORD				m_dwInt;				
	DWORD				m_dwDex;				
	DWORD				m_dwVit;				
	WORD				m_wStatPoint;	
	BYTE				m_byLayerIndex;						// 현재 있는 던전 층
	UNIT_STATUS			m_eUnitStatus;
	BYTE				m_byPrevSectionNum;					// 이전 섹션 번호 	
	CVoidList*			m_pUsingStatusEffectList;			// 현재 적용받고 있는 상태이펙트 리스트
	VECTOR2				m_v2CurPos;							// 현재 좌표 
	VECTOR2				m_v2Direction;						// 바라보고 있는 방향.
	DWORD				m_dwLastMoveTick;					// 마지막으로 이동했던 시각.
	CDungeon*			m_pCurDungeon;						// 현재 몬스터가 있는던젼.
	CDungeonLayer*		m_pCurDungeonLayer;
	EffectDesc*			m_pEffectDesc[MAX_SKILL];			// 현재 적용중인 이펙트를 가지고 있는다.
	float				m_fPlusParam[USER_MAX_STATUS];		// 상승치를 가지고 있는 배열
	WORD				m_pwAttackDamage_L[2];
	WORD				m_pwAttackDamage_R[2];
	BYTE				m_bySelectedSkill[2];
	ATTACK_MODE			m_eAttackMode;
	GAME_OBJECT_TYPE	m_eObjectType;
	CUnit*				m_pLordUnit;
	MAP_TILE*			m_pTileCur;							// 케릭터가 현재 서있는 타일.
	short				m_pwItemAttr[MAX_ITEM_KIND2_CODE];	// 아이템 작용으로 상승되는 케릭터 속성값을 저항하는 템프 변수.
	CUnit*				m_pStaffUnit;						// 부리는 몬스터, 아메리타트 용 : 최덕석 2005.1.26

protected:

	DWORD				m_dwMaxHP;
	DWORD				m_dwMaxMP;
	float				m_fMoveSpeed;
	WORD				m_wAttackAcuracy;					// 공격율
	WORD				m_wAvoid;							// 회피율
	WORD				m_wBlockRate;						// 블록율
	short				m_wFireResist;
	short				m_wIceResist;
	short				m_wLightResist;
	short				m_wPoiResist;
	short				m_wPhyResist;
	WORD				m_wAttackSpeed;						// 공속
	DWORD				m_dwHP;
	DWORD				m_dwSP;
	DWORD				m_dwExp;
	DWORD				m_dwDeathTime; // 사망 시간 설정 : hwoarang 050202

public:

	CUnit();
	virtual ~CUnit();

	virtual const char*				Name() const {return "";}//이름 받기 : hwoarang 050202

	// 유저, 몬스터 둘다 가지고 있어야 할 함수들.
	virtual void					Update() = 0;
	virtual void					DetachSkill(EffectDesc* pEffectDesc) = 0;
	virtual void					RemoveResource() = 0;
	virtual void					CreateResource() = 0;
	virtual void					UpdateMaxHP() = 0;
	virtual void					UpdateMaxSP() = 0;	
	virtual void					UpdateAttackSpeed() = 0;
	virtual void					UpdateAttackAcuracy() = 0;
	virtual void					UpdateBlockRate() = 0;
	virtual void					AddExp(int iExp, BOOL bAlphaExp = TRUE) = 0;
	virtual void					UpdateFireResist() = 0;		
	virtual void					UpdateIceResist() = 0;
	virtual void					UpdateLightResist() = 0;
	virtual void					UpdatePoiResist() = 0;
	virtual void					UpdatePhyResist() = 0;
	virtual void					UpdateAvoid() = 0;
	virtual void					UpdateAD() = 0;
	virtual int						GetExpDying() = 0;
	virtual void					ReSetStatusFromSkillStatusValue(EffectDesc* pEffectDesc) = 0;
	virtual void					SetStatusFromSkillStatusValue(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, float* pResetValue, BOOL bUserStatusSend = FALSE) = 0;
	virtual void					SetDamageOver(const CUnit* pUnit, DWORD dwDamage) = 0;
	virtual BOOL					IsAlliance(const CUnit* pUnit) = 0;
	virtual BOOL					AttachSkill(BYTE bOwnType, DWORD dwOwnIndex, BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, DWORD& dwDestTime, DWORD& dwAlphaDamage) = 0;
	virtual WORD					GetOriginalAttackAcuracy() const = 0;
	virtual WORD					GetOriginalAvoid() const = 0;
	virtual void					UpdateMoveSpeed() = 0;
	virtual void					LevelUP() = 0;
	virtual BOOL					IsBadAction(CUnit* pDefenseUnit) = 0;
	virtual short					GetAllResist() const = 0;
	virtual USER_PROPERTY_CODE		SetDamage(DWORD dwOffenseLevel, DWORD dwDamage ) = 0;
	virtual void					SetHP(DWORD dwHP, const CUnit* pAttacker = NULL) = 0;
	virtual const char*				GetName() const = 0;
	virtual void					SendStopPacket(ENUM_SEND_MODE eSendMode) const = 0;
	virtual void					UpdateAllStatus() = 0;
	virtual void					SendAllStatus() = 0;
	virtual BYTE					GetSkillLevel(const BYTE bySkillKind) const = 0;

	// 유저 몬스터 합치면서 유저만 사용하긴 하지만. 합칠때 유용하라고 이런방식을 썻다. 디폴트로 가지고 있는 함수, 오버로딩 해서 쓰면 된다.
	virtual void					SetBad(BOOL bBad, BYTE byCriminalKind, int nValue, BOOL bChk = FALSE);	
	virtual void					SetAttackTypeAndFrame(const CUnit* pUnit, DWORD dwType);
	virtual void					GetAttackingDamage(WORD wAttackDamageMin, WORD wAttackDamageMax, WORD* pwAttackDamageMin, WORD* pwAttackDamageMax, BYTE byLR);
	virtual void					SetCurDungeonLayer(CDungeonLayer* const pDungeonLayer);
	virtual BOOL					ChangeTargetObject(const CUnit* pAttackUser, int nAttackDemage= -1);
	virtual void					GetAttackDamage(BYTE bySelectedSkill, BYTE bSkillLevel, BYTE bWeaponKind, WORD* pwAttackDamageMin, WORD* pwAttackDamageMax, BYTE bLR) const;
	virtual DWORD					GetConnectionIndex() const;
	
	virtual bool					OnDie() = 0;//사망 처리: hwoarang 050202 

	// 유저, 몬스터에 상관없이 공통 처리를 하는 함수들.
	float							GetMoveSpeed() const;
	void							SetMoveSpeed(const float fMoveSpeed ){m_fMoveSpeed = fMoveSpeed;}
	DWORD							GetID() const;
	void							SetID(DWORD dwID);
	void							SetObjectType(GAME_OBJECT_TYPE eObjectType);
	GAME_OBJECT_TYPE				GetObjectType() const;
	CDungeon*						GetCurDungeon() const;
	void							SetCurDungeon(CDungeon* pDungeon);
	CDungeonLayer*					GetCurDungeonLayer() const;	
	WORD							GetCurDungeonID() const;
	WORD							GetClass() const;
	void							SetClass(WORD wClass);
	WORD							GetRace() const;
	void							SetRace(WORD wRace);
	WORD							GetGrade() const;
	void							SetGrade(WORD wGrade);
	DWORD							GetHP() const;
	DWORD							GetSP() const;
	DWORD							GetMaxHP() const;
	DWORD							GetMaxSP() const;
	DWORD							GetExp() const;
	void							SetExp(DWORD dwExp);
	DWORD							GetLevel() const;
	void							SetLevel(DWORD dwLevel);
	DWORD							GetEgo() const;
	void							SetEgo(DWORD dwEgo);
	DWORD							GetStr() const;
	void							SetStr(DWORD dwStr);
	DWORD							GetInt() const;
	void							SetInt(DWORD dwInt);
	DWORD							GetDex() const;
	void							SetDex(DWORD dwDex);
	DWORD							GetVit() const; 
	void							SetVit(DWORD dwVit);
	WORD							GetAttackAcuracy() const;
	WORD							GetAvoid() const;
	WORD							GetBlockRate() const;
	short							GetFireResist() const;
	short							GetIceResist() const;
	short							GetLightResist() const;
	short							GetPoiResist() const;
	short							GetPhyResist() const;
	WORD							GetNotUseStatPoint() const;
	void							SetStatPoint(WORD wPoint);	
	BYTE							GetCurLayerIndex() const;
	void							SetLayerIndex(BYTE byLayerIndex);
	const WORD*						GetPtrOfAttackDamageL() const;
	const WORD*						GetPtrOfAttackDamageR() const;
	WORD							GetAttackSpeed() const;
	void							SetCurPosition(const VECTOR2* pv2CurPos);
	const VECTOR2*					GetCurPosition() const;
	MAP_TILE*						GetCurMapTile() const;
	void							SetCurMapTile(MAP_TILE* pMapTile);
	WORD							GetCurDungeonLayerID();
	BOOL							IsCurDungeonSiege();
	BOOL							IsCurDungeonVillage();
	UNIT_STATUS						GetUnitStatus() const;
	void							SetUnitStatus(UNIT_STATUS eUnitStatus);
	BYTE							GetPrevSectionNum() const;
	void							SetPrevSectionNum(BYTE bySectionNum);
	CVoidList*						GetUsingStatusEffectList() const;
	void							NewUsingStatusEffectList();
	EffectDesc*						GetEffectDesc(BYTE bySkillKind) const;
	void							SetEffectDesc(BYTE bySkillKind, EffectDesc* pEffectDesc);
	const VECTOR2*					GetCurDirection() const;
	void							SetCurDirection(const VECTOR2* v2Dir);
	void							SetSP(DWORD dwSP);
	BOOL							IsAvoidSuccess(CUnit* pOffense) const;
	BOOL							IsBlockSuccess() const;
	DWORD							GetLastMoveTick() const;
	void							SetLastMoveTick(DWORD dwLastTick);
	float							GetAlphaStat(USER_PROPERTY_CODE eUserStat) const;
	void							SetAlphaStat(USER_PROPERTY_CODE eUserStat, float fParamPlus);
	void							SetAttackDamage_L(WORD wDamageMin, WORD wDamageMax);
	WORD							GetAttackDamageMin_L() const;
	WORD							GetAttackDamageMax_L() const;
	void							SetAttackDamage_R(WORD wDamageMin, WORD wDamageMax);
	void							GetAttackDamage_R(WORD* pwDamageMin, WORD* pwDamageMax);
	WORD							GetAttackDamageMin_R() const;
	WORD							GetAttackDamageMax_R() const;
	DWORD							GetAttackDamageRandomMinMax() const;
	void							SetSelectedSkill(SELECT_ATTACK_TYPE eSelectAttackType, BYTE bSkillKind);
	BYTE							GetSelectedSkill(SELECT_ATTACK_TYPE eSelectAttackType) const;
	void							SetAttackMode(ATTACK_MODE eAttackMode);
	ATTACK_MODE						GetAttackMode() const;
	void							SetLord(CUnit* pUnit);
	CUnit*							GetLord() const;
	void							SetStaff(CUnit* pUnit);
	CUnit*							GetStaff() const;
	DWORD							GetReduceDamageForObject(DWORD dwDamage, CUnit* pAttackUnit);
	EffectDesc*						AllocEffectDesc(BYTE bySkillKind, BYTE bySkillLevel);
	void							FreeEffectDesc(EffectDesc* pEffectDesc);
	void							AddStatusPoint(enum STATUS_POINT_KIND eStatus_Point_Kind);
	void							RemoveAllDetachSkill();
	char							GetSkillLevelByItem(BYTE bySkillKind) const;
	void							SetItemAttr(enum ITEM_ATTR_CODE eRealCode, int nValue, const bool bIsPlus = true);// : 050111 hwoarang
	short							GetItemAttr(ITEM_ATTR_CODE eVirtualCode) const;
	void							InitializeItemAttr();
	
	DWORD	DeathTime() const		{return m_dwDeathTime;}//: hwoarang 050202
	void	DeathTime(const DWORD dwDeathTime) {m_dwDeathTime = dwDeathTime;}//: hwoarang 050202


protected:
	
	void							RemoveAllSkill();

};



#endif 
