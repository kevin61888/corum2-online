// class CMonster
#pragma once
#include "ObjectDefine.h"
//#include "DamageNumber.h"
//#include "Map.h"
#include "SoundControl.h"
#include "SoundLib.h"
#include "MonsterManager.h"

// Proto Type
class CUser;

#define		MONSTER_TEMP_PUSHING					1		// 몬스터 밀림처리가 끝났다는 플래그.
#define		MONSTER_TEMP_FRAMECOUNT					2		// 밀림처리를 위한 프레임기반 증가.
#define		MONSTER_TEMP_PUSHPOWER					3		// 밀릴때의 파워를 저장하는 
#define 	MONSTER_TEMP_PUSHPOWER1					4		// 밀릴때의 파워를 저장하는 
#define		MONSTER_TEMP_KILLERINDEX				5		//	몬스터 죽인 유저인덱스
#define		MONSTER_TEMP_SKILL_POISON_RESIST		6		// 독 저항력
#define		MONSTER_TEMP_TARGET_TYPE				11		// 타겟의 타입. 몬스터? 유저?
#define		MONSTER_TEMP_TARGET_INDEX				12		// 타켓의 인덱스번호.
#define		MONSTER_TEMP_SELECTED					13		// 명령을 위한 선택 플래그
#define		MONSTER_TEMP_DYINGFRAME					14		// 강제 죽는처리를 위해 시간체크를 해라.
#define		MONSTER_TEMP_FINISHDYINGFRAME			15		// 강제 죽는처리를 위해 끝나는 시간체크를 해라.

#define		MONSTER_TEMP_MONMON_DAMAGE_TYPE			26		// 몬스터끼리 싸울때 쓰이는 데미지 타입
#define		MONSTER_TEMP_MONMON_DAMAGE				27		// 몬스터끼리 싸울때 쓰이는 데미지 값
#define		MONSTER_TEMP_DAMAGE_TYPE				28		// 미스인지 블록인지 성공인지
#define		MONSTER_TEMP_DAMAGE						29		// 데미지값								

#define		MONSTER_MAXHP			1
#define		MONSTER_MAXMP			2
#define		MONSTER_HP				3
#define		MONSTER_MP				4
#define		MONSTER_EXP				5

#define		MAX_GUARDIAN_USE_SKILL	5

enum GUARDIAN_CREATE_STATUS
{
	GUARDIAN_CREATE_STATUS_NONE = 0,		// 초기 상태
	GUARDIAN_CREATE_STATUS_ALREADYCALL = 1,	// 이미 콜한놈이구만.
	GUARDIAN_CREATE_STATUS_CREATING = 3,	// 생성중이야.
};

enum GUARDIAN_ITEM_TYPE
{
	GUARDIAN_ITEM_TYPE_EGG = 1,				// 초란
	GUARDIAN_ITEM_TYPE_ADULT = 2,			// 성충
	GUARDIAN_ITEM_TYPE_DIE = 3,				// 썩은알
	GUARDIAN_ITEM_TYPE_BROKEN = 4,			// 깨진알
	GUARDIAN_ITEM_TYPE_SOUL = 5,			// 영혼
};
//#pragma pack( push, 1 )


class CMonster			// 가디언을 포함하는 클래스이다.
{
private:
	BYTE				m_bStatus;						// Monster Status..

public:
	char				m_szName[50];
	DWORD				m_dwMonsterIndex;				// Monster Index..
	BYTE				m_bClass;				
	DWORD				m_dwMonsterKind;				// Monster? or Guardian?

	WORD				m_wCurSectionNum;				// 서버의 PrevSectionNum과 같은 변수.
	
	VECTOR3				m_v3CurPos;						// Current Position..
	VECTOR3				m_v3DestPos;
	VECTOR3				m_v3Direction;					// Current Direction..

	OBJECT_HANDLE_DESC	m_hMonster;						// 20

	EffectDesc*			m_pEffectTarget;						// 자신의 소유 몬스터를 표현할떄 쓴다.
	EffectDesc*			m_pEffectSelect;						// 자신의 소유 몬스터를 선택할떄 쓴다.
	
	GXOBJECT_HANDLE		m_hShadowHandle;
	
	DWORD				m_wModNo;						// 임시.
	float				m_fSpeed;
	BYTE				m_byBaseScale;
	DWORD				m_dwHP;
	DWORD				m_dwMaxHP;
	DWORD				m_dwExp;
	DWORD				m_dwLevel;
	WORD				m_wStatPoint;
	WORD				m_wEgo;
	WORD				m_wStr;
	WORD				m_wInt;
	WORD				m_wDex;
	WORD				m_wVit;
	WORD				m_wAttackAcuracy;		// 공격율
	WORD				m_wAvoid;				// 회피율
	short				m_wFireResist;
	short				m_wIceResist;
	short				m_wLightResist;
	short				m_wPoiResist;
	short				m_wPhyResist;
	WORD				m_pwAttackDamage_L[2];
	MAP_TILE*			m_pCurTile;	
	// slowboat add ///////////////////////
	DWORD				m_dwTemp[30];
	///////////////////////////////////////
	//=======> yakolla
	BYTE				m_bOverDriveCount;
	BYTE				m_bZipCode;
	DWORD				m_dwLordIndex;
	
	COnlyList*			m_pEffectList;				// 이펙트가 를 쏘기 전에 이펙트가 온것을 저장할 큐
	COnlyList*			m_pUsingStatusEffectList;	// 현재 적용받고 있는 상태이펙트 리스트
	EffectDesc*			m_pEffectDesc[MAX_SKILL_COUNT];
	VECTOR3				m_vecTemp;		// 초기 위치
	float				m_radian;		// 튀길 각도
	float				m_fDir;			// 회전할때 쓰일 변수.
	//<======== yakolla

	// 가디언 선택된 스킬 : 최덕석 2005.3.3
	MONSTER_SKILL		m_Skill[MAX_GUARDIAN_USE_SKILL];
	BYTE				m_bSelectedSkill;

	////////////////////////////////////////////////////////////
	// sound_new test
//#if !defined(SOUND_NEW)	
	// 임시 사운드.
	SOUND_FILE_HANDLE	m_hSoundFile[10];
	ISoundEffect *		m_pSound[10];
	CSoundControl *		m_pSoundControl;	
		

//#endif
	//==

	WORD				wNeu;
	WORD				wDamage[2];
	WORD				wAttack[2];
	WORD				wDeath[2];
	WORD				wMove[2];
	POSITION_			m_pDyingList;		// 제한시간이 지나도 죽지 않는다면 억지로 죽여야 하기 위한 리스트에 추가하라.ㅡ.ㅡ
	
	BYTE				m_bSkillTmp;
	BYTE				m_bCongestionMode;	// 폭주모드 깃발
private:
	void				RemoveAllStatusEffect();
	
public:
	BOOL				UsingStatusSkill(BYTE bSkillKind);
	void				AttachSkill(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwEndTime);
	void				RemoveResource();
	void				CreateResource();
	void				SetStatus(BYTE bStatus, void (*ObjectFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE));
	BYTE				GetStatus();
	void				SetDamageIndex( DWORD dwDamage );
	void				SetDamageChar( DWORD dwChar );
	void				SetAction( WORD wAct, int nFrame = 0, BYTE bFlag = ACTION_LOOP );
	void				SetActionNext( WORD wAct, WORD wNextAct, BYTE bNextFlag = ACTION_LOOP, int nNextFrame = 0 );
	void				(*WithActionFunc)(GXOBJECT_HANDLE handle);
	void				InitDie(CUser* pKiller, BOOL bOwn, DWORD dwPower, DWORD dwStartKillFrame);
	void				ReleaseSoundHandle();

	void				ModelScaleByLevel();
	BOOL				IsNormalMonster();
	void				SetActionSkill(BYTE bSkillKind);
	

	// 가디언 스킬 선택 : 최덕석 2005.3.3
	BYTE GetSkillLevel(const BYTE bySkillKind) const;
	void SetSkillLevel(const BYTE bySkillKind, const BYTE bySkillLevel);
	BYTE GetSelectedSkill() const;
	void SetSelectedSkill(BYTE bSkill);

	CMonster();
	~CMonster();
};

//#pragma pack(pop)