/////////////////////////////////////
//
//
//		yakolla
//		skill.iti 파일을 던전 데이타폴더에 넣어 둔다.
/////////////////////////////////////
#pragma once
#include "..\\4DyuchiGRX_common\\math.inl"
#define     MAX_SKILL					151
#define MAX_SKILL_COUNT					200		// 버퍼를 위해서.
#define MAX_SKILL_LEVEL				50
#define ITI_TYPE_BASESKILL			3000
#define MAX_KIND_OF_CLASS_EX		MAX_KIND_OF_CLASS + 2	// 클레스에 속하지 않는 이펙트가 필요 해서.
#define MAX_STATUS_SKILLVALUE		5		// 상태스킬에 의한 변화할수 있는값.
#define PROPERTY_AURA	100
#define PROPERTY_DIVINE	200
#define PROPERTY_SUMMON	300
#define PROPERTY_CHAKRA	400
#define PROPERTY_MAGIC	500

#define TARGET_MONSTER		1		// 스킬쏠때 클릭해야 하는 대상(몬스터)
#define TARGET_TILE			2		// 스킬쏠때 클릭해야 하는 대상(타일)
#define TARGET_PLAYER		4		// 스킬쏠때 클릭해야 하는 대상(플레이어)
#define TARGET_ALL			7		// 스킬쏠때 클릭해야 하는 대상(모든)
#define TARGET_PKPLAYER		8		// 스킬쏠때 클릭해야 하는 대상(PK모드일때 PLAYER)

#define TARGETTYPE_NONE		0
#define TARGETTYPE_TILE		1
#define TARGETTYPE_ENEMY_MONSTER  2	// 대상이 몬스터
#define TARGETTYPE_FRIEND_MONSTER  4	// 대상이 몬스터
#define	TARGETTYPE_ENEMY_PLAYER	8		// 대상이 플레이어
#define	TARGETTYPE_FRIEND_PLAYER	16		// 대상이 플레이어

#define TARGETTYPE_OBJECT	100		// 개체
#define	TARGETTYPE_LINE		110		// 직선
#define	TARGETTYPE_CIRCLE	120		// 원안에 있는 놈들
#define	TARGETTYPE_X		130		// X 모양에 있는놈들
#define TARGETTYPE_BOX		 140	// 박스
#define TARGETTYPE_MULTICOLLITION 150	// 3중 충돌
#define TARGETTYPE_TIMEZEROTILE 160	// 타일에다가 지뢰를 박는다. 시간제한.
#define TARGETTYPE_SUMMONTILE	170 // 타일에다가 소환을 한다.
#define	TARGETTYPE_ARC		180		// 부채꼴
#define	TYPE_ACTIVE		1
#define	TYPE_DRIVE		2
#define	TYPE_PASSIVE		3
#define TYPE_TIMEZERO	4
#define	NONE			0
#define	SKILL_PROPERTY_KIND_FIRE		1	// 스킬 속성 불
#define	SKILL_PROPERTY_KIND_ICE			2	// 스킬 속성 얼음
#define	SKILL_PROPERTY_KIND_EARTH		3	// 스킬 속성 불1/2 물리1/2
#define	SKILL_PROPERTY_KIND_LIGHT		4	// 스킬 속성 전기
#define	SKILL_PROPERTY_KIND_PHY			5	// 스킬 속성 물리
#define SKILL_PROPERTY_KIND_POI			6	// 스킬 속성 독
#define CRIME_APPLY_ENEMY		1	// 스킬사용될때 적용되는 대상이 적군에게
#define CRIME_APPLY_FRIENDLY	2	// 스킬사용될때 적용되는 대상이 아군에게
#define CRIME_APPLY_ENEMY_FRIENDLY_MONSTER 3 // 스킬사용될때 적용되는 공격스킬이지만 나가 소환한 몬스터에게만 사용할수 있게 한다.
											// 물론 PK일때는 전부다 적용시킬수 있다.

// 마법 계열
#define	__SKILL_MANAMASTERY__			1
#define	__SKILL_FIREMISSILE__			2
#define	__SKILL_FIREBLAZE__				3
#define	__SKILL_MAGMAWALL__				4
#define	__SKILL_ICEMISSILE__			5
#define	__SKILL_ICEWAVE__				6
#define	__SKILL_ICEWALL__				7
	
#define	__SKILL_EARTHQUAKE__			9
#define	__SKILL_METEOR__				10
#define	__SKILL_LIGHTNING__				11
#define	__SKILL_AIREALCOAT__			12
#define	__SKILL_THUNDERSTORM__			13
#define	__SKILL_MANAEXPAND__			14
#define	__SKILL_SPEEDCASTING__			15
#define	__SKILL_EXPLOSION__				16
#define	__SKILL_MEDITETION__			17
#define	__SKILL_MULTICASTING__			18
#define	__SKILL_CONCENTRATION__			19
#define __SKILL_BLASTING__				20
#define __SKILL_FREEZING__				21
#define __SKILL_FLASHSHOCK__			22
#define __SKILL_DISPEL__				23
#define __SKILL_FIREMASTERY__			24
#define __SKILL_ICEMASTERY__			25
#define __SKILL_LIGHTMASTERY__			26
#define __SKILL_DRAGONICFIREBLAST__		28
#define __SKILL_DRAGONICVIBRATION__		29
// 오라 계열
#define	__SKILL_AURAMASTERY__			31
#define	__SKILL_AURARECHARGE__			32
#define	__SKILL_ZEAL__					33
#define	__SKILL_BASH__					34
#define __SKILL_LIGHTFLING				35
#define	__SKILL_RAGINGSWORD__			36
#define __SKILL_LIFEBALANCE__			37
#define	__SKILL_LIGHTNINGBREAK__		38
#define	__SKILL_SILENTBRANDYSHIP__		39
#define	__SKILL_ANTAGONIZE__			41
#define	__SKILL_AURABATTLER__			42
#define	__SKILL_LIFEFORCE__				43
#define	__SKILL_REGENERATION__			44
#define	__SKILL_WEAPONMASTERY__			45
#define	__SKILL_DEFENSEMASTERY__		46
#define __SKILL_SWORDMASTERY__			47
#define __SKILL_BLUNTWEAPON_FIGTER_MASTERY__	48
#define __SKILL_SPEARMASTERY__			49
#define __SKILL_INFULX__				50
#define __SKILL_AMPLIFY__				51
#define __SKILL_BERSERKER__				52

// 디바인 계열
#define	__SKILL_DIVINEMASTERY__			61
#define	__SKILL_HEAL__					62
#define	__SKILL_PRAY__					63
#define	__SKILL_BLESSING__				64
#define	__SKILL_WINDTALES__				65
#define	__SKILL_HOLDSHIELD__			66
#define	__SKILL_WINDFORCE__				67
#define	__SKILL_TREEAGION__				68
#define	__SKILL_AMFLEAFIRE__			69
#define	__SKILL_HOLYPRESSURE__			70
#define	__SKILL_APSOLUTMAGICBARRIER__	71
#define	__SKILL_SPELLBINDING__			72
#define	__SKILL_DETECT__				73
#define	__SKILL_AROSE__					74
#define	__SKILL_HOLYDIVINE__			75
#define __SKILL_ORDRISING__				77
#define __SKILL_SUITRISING__			78
#define __SKILL_ORDSLUM__				79
#define __SKILL_SUITSLUM__				80
#define __SKILL_REMEDY__				81
#define __SKILL_PROTECTSHIELD__			82
#define __SKILL_FRAIL__					83
#define __SKILL_DETECTION__				84
#define __SKILL_ROUNDRANGE__			85
#define __SKILL_BLUNTWEAPON_PREST_MASTERY__	86
#define __SKILL_HOLYBRAIN				87
#define __SKILL_INCREASING				88
#define	__SKILL_LANDINGSUPPORT__		89
// 서몬 계열
#define	__SKILL_SUMMONMASTERY__			91
#define	__SKILL_CALMDOWN__				92
#define	__SKILL_SLEEP__					93
#define	__SKILL_CONFUSE__				94
#define	__SKILL_MINDCONTROL__			95
#define	__SKILL_RAGE__					96		//kk
#define	__SKILL_REDELEMENTAL__			97
#define __SKILL_SOULBLASTER__			98
#define	__SKILL_MINDEXPLOSION__			99
#define	__SKILL_SOULETER__				100
#define	__SKILL_SUMMONACCELERATOR__		101
#define	__SKILL_DOMENATION__			102
#define	__SKILL_TAMINGMASTERY__			103
#define	__SKILL_BEYONDLIMIT__			104
#define __SKILL_BLUEELEMENTAL__			105
#define __SKILL_GREENELEMENTAL__		106
#define __SKILL_MIRACULOUSHEART__		107
#define __SKILL_ARCANUMSKIN__			108
#define __SKILL_FRENZY__				109
#define __SKILL_DETONATION__			110
#define __SKILL_TURNOVER__				111
#define __SKILL_DETONATION_MONSTER__	112
// 차크라 계열
#define	__SKILL_CHAKRAMASTERY__			121
#define	__SKILL_POISONING__				123
#define	__SKILL_CHAINATTACKER__			124
#define	__SKILL_IRONSKIN__				125
#define	__SKILL_SNIPING__				126
#define	__SKILL_SHADOWSQUARE__			127
#define	__SKILL_HIDING__				128
#define	__SKILL_SPELLTRAP__				129
#define	__SKILL_PRESSURE__				130
#define	__SKILL_SYLPHID__				131
#define	__SKILL_SHAUT__					132
#define	__SKILL_BERSERK__				133
#define	__SKILL_SPEEDOVER__				134
#define	__SKILL_DODGE__					135
#define	__SKILL_TRAINING__				136
#define	__SKILL_BLASTQUAKE__			137
#define	__SKILL_LIFEEXPLOSION__			138
#define __SKILL_DEATHBLOW__				139
#define __SKILL_VAMPIRE__				140
#define __SKILL_POSIONCLOUD__			141
#define __SKILL_DARK__					143
#define __SKILL_FORTIFICATIONMASTERY__	144
#define __SKILL_AMPLIFICATIONMASTERY__	145
#define __SKILL_POISONMASTERY__			146
#define __SKILL_EXTREMESPEAR__				147
#define __SKILL_ATTACK__				151
#define __SKILL_NONE_SELECT__			255

#define EFFECT_TYPE_STATUS_TOP		1		
#define EFFECT_TYPE_STATUS_CENTER	2		
#define EFFECT_TYPE_STATUS_BOTTOM	4		

class CUnit;
class CUser;
class CMonster;
class CDungeonLayer;
struct SKILL_TARGETINFO;
struct SKILL_RESULT;
struct DSTC_USER_STATUS;
//--> 스크립트 구조들
#pragma pack( push, 1 )

struct VALUE
{
	int		nMin;
	int		nMax;
	int		nMana;
	int		nCompass;
	int		nDuration;
	int		nProbability;
};

struct STATUSVALUE
{
	WORD			wStatusID;	// 상태마법을 썻을때 변화되는 필드 플래그.
	BYTE			bFormula;	// 식에 의한 값을 계산하기 위해서.
};

typedef struct BASESKILL
{
	BYTE			bID;				// 아이디.
	DWORD			dwResourceID;
	DWORD			dwStatusResourceID;
	WORD			wProperty;			// none : 0, 마법 : 500, Aura : 100, Divine, : 200, Summon : 300, Chakra : 400
	BYTE			bType;				// none : 0, Active : 1, Drive : 2, Passive : 3
	BYTE			bAbleClass;			// 스킬을 사용할수 있는 클래스
//	char			szName_Kor[50];
//	char			szName_Eng[25];
	BYTE			bSkillTarget;		// none : 0, monster : 1, 지역 : 2, player : 3
	BYTE			bSkillType;			// none : 0, player : 1, monster : object : 100, 2 직선 : 110, 원 : 120, X범위 : 130, 직교 : 140, 3중충돌 : 150, 타일셋 : 160
	DWORD			dwRange;
	DWORD			dwMinMastery;		// 최소 마스터리
	DWORD			dwCastingTime;
	DWORD			dwCoolTime;
	DWORD			dwEffectPosition;	// 31~29 bit none : 0, 머리위 : 1, 몸통 : 2, 바닥 : 4,  28~0 bit subkey
	BYTE			bJointEffect;
	BYTE			bFormula;			// 공식 
	BYTE			bSuccessFormula;	// 스킬 성공률에 관한 공식
	WORD			wSoundNum[8];		// 사운드 넘버들.
	STATUSVALUE		sStatusValue[MAX_STATUS_SKILLVALUE];	
	BYTE			bCrime;				// 적용대상이 적이면 1, 아군이면 2
	BYTE			bEfficiency;
//	char			szInfo[128];	// 스킬에 대한 간단한 설명.
	VALUE			Value[MAX_SKILL_LEVEL+1];	
} *LPBASESKILL;




//<-- 스크립트 구조들
struct Effect : public BASESKILL
{
public:
	
	int GetEffectPosition();
	int	GetMaxMastery(DWORD dwUserLevel, BYTE bSkillLevel, WORD wClass);	
};

enum
{
	EFFECT_DESC_TEMP_POISON_DAMAGE_MIN		=	0,	// 데미지 사용한다.
	EFFECT_DESC_TEMP_POISON_DAMAGE_MAX		=	1,	// 데미지 사용한다.
	EFFECT_DESC_TEMP_POISON_OWNER_TYPE		=	2,	// 중독에서 사용한다.
	EFFECT_DESC_TEMP_POISON_OWNER_INDEX		=	3,	// 중독에서 사용한다.
	EFFECT_DESC_TEMP_MAX 
};

struct EffectDesc
{
	float		fParamPlus[5];					// 상태변화값.
	DWORD		dwDestTick;						// 끝날 시간.
	DWORD		dwLastUpdateTick;				// 마지막 으로 상태를 저용한 시간( 중독등의 스킬 적용시 사용 )
	DWORD		dwAmfleafireTime;				// 엠플리파이어 영향을 받아 시간이 늘어난시간을 계산.
	Effect*		pEffect;						// 이펙트 정보
	void*		pos;							// 리스트에 연결할때의 포지션.
	CUser*		pOwnUser;						// 스킬 쏜놈.
	DWORD		dwTemp[EFFECT_DESC_TEMP_MAX];	// 여러가지 용도로 사용할 공간
	BYTE		bSkillLevel;					// 스킬 레벨	
};

struct SKILLDESC
{
	BYTE			bSkillKind;					// 스킬 아이디
	char			bSkillLevel;				// 스킬 레벨 : zero base
	DWORD			dwSkillKeepTime;			// 스킬 지속시간 (단, 오버드라이브만 사용가능하다)
	BYTE			bOwnType;					// 스킬을 쏘는 객체 타입
	DWORD			dwOwnIndex;					// 스킬을 쏘는 객체 인덱스
	BYTE			bTargetType;				// 스킬을 당하는 객체 타입
	DWORD			dwTargetIndex;				// 스킬을 당하는 객체 인덱스
	WORD			wTileIndex_X;				// 스킬 발동할 위치
	WORD			wTileIndex_Z;				// 스킬 발동할 위치
	VECTOR2			v2OwnObjectPos;				// 스킬 쏘는 객체의 위치
	BOOL			bPK;						// PK 모드인가.
	WORD			wDamageMinMax[2];			// 스킬 데미지 최소 최대값을 기록
	CDungeonLayer*	pDungeonLayer;				// 쏘려고 하는 던전 층포인터
	BYTE			bSectionNum;				// 쏘려고 하는 섹션 넘버.
	CUser*			pMonsterMaster;				// 몬스터 소환시 사용되어진다.소환할 몬스터의 주인이 될 놈의 포인터를 넣어 준다.
};

struct EffectLayer
{	
	Effect			m_Effect[MAX_SKILL_COUNT];	
	
private:
	
	DWORD			Load(char* szFileName); // 파일 읽어.
	BOOL			MessyProcessForUser(CUser* pOwnUser, SKILLDESC* pSkillDesc);
	BOOL			MessyProcessForSystem(CUser* pOwnUser, SKILLDESC* pSkillDesc);
	BYTE			GetAppliedTargetCount(CDungeonLayer* pDungeonLayer, SKILLDESC* pSkillDesc, SKILL_TARGETINFO* pTargetInfo, VECTOR2* pV2Start);
	void			RevisionStartPositon(SKILLDESC* pSkillDesc, VECTOR2* pV2Start);
	BOOL			IsRange(SKILLDESC* pSkillDesc, VECTOR2* pV2Start);
	CUnit*			GetValiidUnit(SKILLDESC* pSkillDesc);
	BOOL			IsSkillSuccess(SKILLDESC* pSkillDesc, CUnit* pOffense, CUnit* pDefense, SKILL_RESULT* pSkill_result);
	void			AfterSkillSuccessProcess(SKILLDESC* pSkillDesc, CUnit* pOffense, CUnit* pDefense, DSTC_USER_STATUS*	pUserStatus);
public:
	int				GetBaseClassSkillMax(WORD wClass, BYTE bSkillKind);
	BYTE			GetSkillMasteryKind(BYTE bSkillKind);	// 해당종류의 마스터리 아이디를 얻어온다.
	BOOL			IsSuccessByFormula0(BYTE bSkillKind, BYTE bSkillLevel);
	BOOL			IsSuccessByFormula1(WORD wAA, WORD wDP,  WORD wBL, DWORD dwOffenseLevel, DWORD dwDefenceLevel);
	BOOL			IsSuccessByFormula2(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwOffenseLevel, DWORD dwDefenceLevel);
	BOOL			IsSuccessByFormula13(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSenderLevel, BYTE bRecverLevel);
	BOOL			IsSuccessByFormula9(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSummonMasteryLevel, BYTE bSenderLevel, BYTE bRecverLevel);
	BOOL			IsSuccessByFormula5(BYTE bSkillKind, BYTE bSkillLevel, BYTE bSummonMasterySkillLevel, DWORD dwOffenseLevel, DWORD dwDefenceLevel);
	BOOL			IsSuccessByFormula6(BYTE bSkillKind, BYTE bSkillLevel);
	BOOL			IsSuccess(BYTE bSkillKind, BYTE bSkillLevel, EffectDesc* pEffectDesc_Pray);
	float			GetFormula8(BYTE bSkillKind, BYTE bSkillLevel, float fOriginaParam);
	int				GetFormula9(BYTE bSkillKind, BYTE bSkillLevel, BYTE bOwnLevel, BYTE bTargetLevel, BYTE bSummonMasteryLevel);
	int				GetFormula16(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	int				GetFormula19(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	float			GetFormula15(BYTE bSkillKind, BYTE bSkillLevel, float fOriginalParam);
	int				GetFormula14(BYTE bSkillKind, BYTE bSkillLevel, BYTE bMasterySkillKind, BYTE bMasterySkillLevel);
	float			GetFormula3(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	float			GetFormula4(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass, float fOriginalParam);
	float			GetFormula5(BYTE bSkillKind, BYTE bSkillLevel, float fOriginalParam);
	float			GetFormula17(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	float			GetFormula18(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	float			GetFormula23(BYTE bSkillLevel);
	int				GetFormula24(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	int				GetFormula25(BYTE bSkillKind, BYTE bSkillLevel, WORD wClass);
	void			SendSkillStatusUserMon(CUser* pSendUser, CMonster *pMonster, BYTE bSkillKind, BYTE bSkillLevel,DWORD dwEndTime); // 상태스킬을 만들어서 클라이언트에게 보내줘라.
	void			SendSkillStatusUserUser(CUser *pSendUser, CUser *pTargetUser, BYTE bSkillKind, BYTE bSkillLevel,DWORD dwEndTime);// 상태스킬을 만들어서 클라이언트에게 보내줘라.
		
	void			Init();			// 초기화
	Effect*			GetEffectInfo(BYTE bSkillKind);
	void			LoadScript();		// 스크립트 파일을 읽어 와라.
	int				GetEffectStatusPostion(BYTE bID);
	void			SendSkill(SKILLDESC* pSkillDesc);
	int				GetUsedSPSkill(CUser* pOwnUser, SKILLDESC* pSkillDesc);
	BOOL			IsUnitStatusReadySkill(BYTE bySkillKind, const CUnit* pUnit);

	~EffectLayer();
};

struct SKILL_COMMAND
{
	BYTE	bSkillKind;
	BYTE	bOwnType;
	BYTE	bTargetType;	// OBJECT_TYPE_PLAYER, OBJECT_TYPE_MONSTER, OBJECT_TYPE_TILE
	BYTE	bPK;			// pk 모드인지..
	DWORD	dwTargetIndex;
	DWORD	dwTime;			// 적용되어질 타임.	
};


#pragma pack( pop )
extern EffectLayer*	g_pEffectLayer;