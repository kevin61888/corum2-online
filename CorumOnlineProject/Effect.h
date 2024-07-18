/////////////////////////////////////
//
//
//		yakolla
//
/////////////////////////////////////
#pragma once
#include "ObjectDefine.h"
#include "Define.h"
#include "SoundLib.h"
#include "GameDefine.h"
extern DWORD	g_dwCurTick;
#define MAX_SKILL_LEVEL				50
#define ITI_TYPE_BASESKILL			3000
#define MAX_KIND_OF_CLASS_EX		MAX_KIND_OF_CLASS + 2	// 클레스에 속하지 않는 이펙트가 필요 해서.
#define MAX_STATUS_SKILLVALUE		5		// 상태스킬에 의한 변화할수 있는값.

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
#define	TYPE_PASSIVE	3
#define TYPE_TIMEZERO	4

#define	NONE			0
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
#define	__SKILL_RAGE__					96
#define	__SKILL_REDELEMENTAL__			97
#define __SKILL_SOULBLASTER				98
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
#define __SKILL_FORTIFICATIONMASTERY__	144
#define __SKILL_AMPLIFICATIONMASTERY__	145
#define __SKILL_POISONMASTERY__			146
#define __SKILL_EXTREMESPEAR__			147
#define __SKILL_ATTACK__				151
#define __SKILL_NONE_SELECT__			255

#define __CHR_EFFECT_NONE__				(__SKILL_ATTACK__+1)
#define	__CHR_EFFECT_GUARDIAN_DYING__	(__CHR_EFFECT_NONE__+1)

#define SKILL_TEMP_EFFECT_TIME_FINISH	0		// 이펙트가 진행중일때 끝낼수 있는 플래그.
#define SKILL_TEMP_EFFECT_TIME_5SEC		1		// 이펙트가 끝나기 5초전에 소리 내라는 플래그.
#define	SKILL_TEMP_EFFECT_TIME_30SEC	2		// 이펙트가 끝나기 30초전에 소리 내라는 플래그.
#define SKILL_TEMP_CREATECOUNT			3		// 똑같은 이펙트가 여러개 만들어질때 만들어진 순서.
#define SKILL_TEMP_HOAMINGRANDOM		4		// 유도 계산에서 사용하는 랜덤중인지 플래그.
#define SKILL_TEMP_HOAMINGFRAMECOUNT	5		// 유도 계산에서의 진행 카운트
#define SKILL_TEMP_HOAMINGARROW			6		// 유도 계산에서의 방향플래그.
#define SKILL_TEMP_HOAMINGDRUNKENCOUNT	7		// 유도 게산에서 드렁큰모드로 전환된 카운트
#define SKILL_TEMP_EFFECT_TIME_1SEC		8		// 1초마다 처리하기 위한
#define SKILL_TEMP_EFFECT_ALPHA			9		// POTION에서 쓰일 알파 초기상태값.
#define	SKILL_TEMP_POSONING				10
#define	SKILL_TEMP_CAMERA_FRAMECOUNT	11		// 카메라 흔들린 프레임
#define	SKILL_TEMP_AMFLEAFIRETIME		12		// 엠플리 적용시켰을때의 오버드라이브의 남은 시간을 저장하여 엠플리가 사라졋을때 기준삼아 다시 시간을 깍아준다.
class CUser;
class CMonster;
class CEffectHash;
interface ISoundEffect;
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
	BYTE			bID;		// 아이디.
	DWORD			dwResourceID;
	DWORD			dwStatusResourceID;
	WORD			wProperty; // none : 0, 마법 : 500, Aura : 100, Divine, : 200, Summon : 300, Chakra : 400
	BYTE			bType; // none : 0, Active : 1, Drive : 2, Passive : 3
	BYTE			bAbleClass;	// 스킬을 사용할수 있는 클래스
	char			szName[50];
	char			szDescription[255];	// 스킬에 대한 간단한 설명.
	char			szDescription2[2][64];		
	BYTE			bSkillTarget; // 0 = none, 1 = 땅, 2 = 몬스터(적군), 4 = 몬스터(아군), 8 = 유저(적군), 16 = 유저(아군), 32 = 적군에게는 pk모드일때만 사용가능한가?
	BYTE			bSkillType; // none : 0, player : 1, monster : 2, object : 100, 2 직선 : 110, 원 : 120, X범위 : 130, 직교 : 140, 3중충돌 : 150, 타일셋 : 160
	DWORD			dwRange;
	DWORD			dwMinMastery;	// 최소 마스터리
	DWORD			dwCastingTime;
	DWORD			dwCoolTime;
	DWORD			dwEffectPosition; // none : 0, 머리위 : 1, 몸통 : 2, 바닥 : 4
	BYTE			bJointEffect;
	BYTE			bFormula;		// 공식 
	BYTE			bSuccessFormula;	// 스킬 성공률에 관한 공식
	WORD			wSoundNum[8];	// 사운드 넘버들.
	STATUSVALUE		sStatusValue[MAX_STATUS_SKILLVALUE];
	BYTE			bCrime;			// 적용대상이 적이면 1, 아군이면 2
	BYTE			bEfficiency;
	
	VALUE			Value[MAX_SKILL_LEVEL+1];
	
	
} *LPBASESKILL;
#pragma pack( pop )
//<-- 스크립트 구조들
struct Effect : public BASESKILL
{
	
	void				(*SkillFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// 스킬 펑션.
	void				(*MonsterDamageFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// 몬스터 맞는 펑션.
	void				(*MonsterKillFunc)(GXOBJECT_HANDLE, OBJECT_DESC*, DWORD, BYTE);		// 몬스터 죽는 펑션.
	void				(*MonsterKillWithAction)(GXOBJECT_HANDLE);		// 몬스터 죽는 죽기전에 액션 펑션.
	void				(*MonsterSKillDamageWithAction)(GXOBJECT_HANDLE);		// 몬스터 스킬 데미지 액션 펑션.
	
public:
	int					GetEffectPosition()				{	return dwEffectPosition;	}
	DWORD				GetClassType()					{	return wProperty/100;		}
	BOOL				IsRangeSkill()					{	return bSkillType/10 > 10;	}
	int					GetMaxMastery(BYTE bSkillLevel)
	{
	/*
		if (wProperty/100 == g_pMainPlayer->m_wClass)
			return (Value[bSkillLevel].nMax*(*((int*)&g_sBaseClassInfo[g_pMainPlayer->m_wClass-1].iMax_Aura+(wProperty/100-1)))/100.) + (g_pMainPlayer->m_dwLevel * 0.5);
		else
			return (Value[bSkillLevel].nMax*(*((int*)&g_sBaseClassInfo[g_pMainPlayer->m_wClass-1].iMax_Aura+(wProperty/100-1)))/100.);
			*/
		//return (Value[bSkillLevel].nMax*(*((int*)&g_sBaseClassInfo[g_pMainPlayer->m_wClass-1].iMax_Aura+(wProperty/100-1)))/100.);
		return bSkillLevel+1;
	}

};
class EffectDesc;
#define LIGHT_DESCEX_TEMP_SHOW 0		// SHOW상태인지 HIDE상태인지.
struct LIGHT_DESCEX
{
	GXLIGHT_HANDLE		m_handle;		// 데미지 입었을때 나타나는 라이트 핸들
	LIGHT_DESC			m_sLightDesc;			// 라이트 데스크
	void				(*LightFunc)(EffectDesc*);			// 라이트 펑션.깜빡거리기 위해서..	
	int					m_nDestTick;
	DWORD				m_dwTemp[10];
};
// 이펙트 표현을 위한 구조체.
class EffectDesc
{
public:
	EffectDesc(){};
	~EffectDesc(){};
	BYTE				bEffectInfoNum;		// 표현할려고 하는 이펙트의 정보 아이디.	
	BYTE				bJoint;				// 부가 이펙트 리소스 정보.
	WORD				wChrNum;			// chr 정보 참조 인덱스
	Effect*				pEffect;				// 이펙트 정보

	DWORD				dwCount;				// 이펙트 맞은놈의 수
	DWORD				dwOwnType;				// 스킬쓴놈의 타입
	DWORD				dwOwnIndex;				// 스킬쓴놈의 인덱스
	DWORD				dwFrameCount;			// 진행 카운트
	DWORD				dwFrameFinish;			// 멈춤 카운트
	DWORD				dwHashKey;
	
	float				f_Radian;				// 이펙트 진행 라디안.
	float				f_Radcount;
		
	DWORD				dwSkillLevel;
	DWORD				dwTemp[20];				
	DWORD				dwTargetIndex[20];		// 이펙트 맞은놈들의 인덱스
	BYTE				byTargetObjectType[20];	// 타겟속성. 몬스터인지 유저인지.
	void*				pUsingStatus;	
	BOOL				bOwn;					// 이펙트를 쏜게 자신이면 1 아니면 0 , 0xff = 이펙트 옵션오프 해도 적용받지 않는것들이다.
	VECTOR3				vecOwn;					// 스킬시직위치.
	VECTOR3				vecBasePosion;			// 땅을 클릭했을때를 위한 위치.

	LIGHT_DESCEX		m_sLightDescEx;
	OBJECT_HANDLE_DESC	hEffect;	// 이펙트의 오브젝트 핸들.
	ISoundEffect*		m_pSound[8];
	SOUND_FILE_HANDLE	m_hSoundFile[8];
	
	DWORD				GetRemainTime(DWORD dwCurTick)	
	{
		return (hEffect.pDesc->dwDestTime > dwCurTick) ?
			((hEffect.pDesc->dwDestTime - dwCurTick)/1000) : 0;
	}
};


struct EffectLayer
{
	Effect					m_Effect[MAX_SKILL_COUNT];
	
	BYTE					m_nEffectSize[MAX_SKILL_COUNT];
	
private:
	
	DWORD					Load(char* szFileName); // 파일 읽어.
	DWORD					ParsingSkillDescription(BYTE bSkillKind);
	BOOL					SearchString(BYTE bSkillKind, int nLen, int nIndex);
	DWORD					m_dwCount;

public:
	EffectDesc*				CreateMagicArray(BYTE bSkillKind, VECTOR3* vecStart, BOOL bOwn);	// 마법을 사용할때 마법원 그리기.
	void					SetSkillStatusUserUser(CUser* pTargetUser, BYTE bSkillKind);
	void					SendSkillStatusUserUser();
	BOOL					IsEffectUse(BYTE bSkillKind, VECTOR3* vecTarget, DWORD dwStartSkillTick);
	EffectDesc*				CreateGXObject(char *szFile, BOOL bOwn, WORD wChrNum);
	EffectDesc*				CreateStatusEffect(BYTE bSkillKind, BYTE bJoint, BOOL bOwn);
	void					SetSkillStatusUserMon(CMonster* pMonster, BYTE bSkillKind);
	void					SendSkillStatusUserMon();
	EffectDesc*				CreateEffect(BYTE bSkillKind, BYTE bJoint, BOOL bOwn);
	BOOL					IsEffectShow(EffectDesc* pEffectDesc);
	void					AttachLight(EffectDesc* pEffectDesc, BYTE bLightNum, void(*LightFunc)(EffectDesc*));
	void					DetachLight(EffectDesc* pEffectDesc);
		
	void					Init(BOOL bChk);			// 초기화
	void					LoadScript(BOOL bChk);		// 스크립트 파일을 읽어 와라.

	int						GetEffectStatusPostion(BYTE bID)		{	return m_Effect[bID].GetEffectPosition();			}
	BYTE					GetSkillMasteryKind(BYTE bSkillKind)	{	return BYTE(bSkillKind/30*30+__SKILL_MANAMASTERY__);}
	Effect*					GetEffectInfo(BYTE bSkillID)			{	return &m_Effect[bSkillID];							}
	
~EffectLayer(){};
};


