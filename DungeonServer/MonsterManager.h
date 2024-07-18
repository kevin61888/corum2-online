#pragma once
#include "../CommonServer/CommonHeader.h"
#include "../CommonServer/AbyssHash64.h"
//#include "AbyssHash64.h"


#define BASE_ITEMBOTARY_INDEX	4200
#define	MAX_ITEMBOTARY			500
#define ITI_TYPE_BASEMONSTER	2000
#define MAX_MONSTER_USE_SKILL	10
#define MAX_GUARDIAN_USE_SKILL	5


enum MONSTER_RACE
{
	MONSTER_RACE_NPC = 0,
	MONSTER_RACE_GUARDIAN = 12,
	MONSTER_RACE_ELEMENTAL = 6,
	MONSTER_RACE_AMERITAT = 9,
};

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


#pragma pack( push, 1 )
struct MONSTER_SKILL
{
	BYTE	bySkillKind;
	BYTE	bSkillLevel;
};

struct MONSTER_AITYPE
{
	BYTE	bMoveType;
	BYTE	bSearchType;
	BYTE	bAttackType;
	BYTE	bDefenseType;
};

struct MONSTER_AI
{
	BYTE				bSetTargetType;
	BYTE				bFirstAttack;
	BYTE				bTargetChangeType;
	BYTE				bCongestionMode;
	DWORD				dwMonsterIDForCongestionMode;	//폭주모드를 발동시킬 대장 몬스터의 아이디
	float				fRunningRate;					//달리기 속도
	WORD				wCongestionTime;				//폭주모드 유지 시간 	
	float				fMinHPRateForCongestionMode;	//폭주모드가 On될 HP Percentage (예)0.2이면 몬스터 HP가 20% 남았을때 폭주모드 발동한다.
};

//몬스터 AI 타겟 설정 조건 DEFINE
#define AI_TARGET_TYPE_NEAR				0	//가장 가까운 놈을 타겟으로  
#define AI_TARGET_TYPE_LEAST_HP			1	//수치상으로 가장 적은 HP 유저를 타겟으로 
#define AI_TARGET_TYPE_NEAR_AND_FEW_HP	2	//가까우면서 적은 HP놈을 타겟으로 (구버젼 공식이라 정확히 잘 모르겠음)
#define AI_TARGET_TYPE_SUMMON_OBJECT	3	//가디언이나 소환물을을 타겟으로 
#define AI_TARGET_TYPE_SAME_KIND		4	//타겟이 없을시 시야에 같은 id의 몬스터가 타겟으로 삼은 적을 타겟으로 잡는다.

//몬스터 선공조건 DEFINE
#define AI_FIRST_ATTACK_NONE			0	//선공하지 않는다. 공격받으면 공격한다. 
#define AI_FIRST_ATTACK_ENABLE			1	//선공한다 


//몬스터 타겟 변경 조건 
#define AI_CHANGE_TARGET_TYPE_NONE				0	//한놈만 죽어라 공격한다. 
#define AI_CHANGE_TARGET_TYPE_PERCENTAGE_DEMAGE	1	//몬스터 HP의 xx% 이상의 데미지를 입힌놈으로 타겟을 바꾼다.
#define AI_CHANGE_TARGET_TYPE_ABSOLUTELY_CHANGE 2	//공격성공여부에 상관없이 무조건 최근 공격받은놈으로 타겟을 바꾼다. 
#define AI_CHANGE_TARGET_TYPE_SAME_KIND			3	//내 타겟이 있더라도 시야에 같은 id의 몬스터가 타겟을 변경하면 변경된 적을 다시 타겟으로 설정한다. 


struct MONSTER_ITEM_RATE
{
	WORD	ItemID;
	WORD	ItemRate;
};

struct MIN_MAX
{
	WORD	wMin;
	WORD	wMax;
};

// 몬스터가 줄수 있는 아이템의 주머니를 더 늘려주기 위한 것이다.(속주머니)
struct BASEITEM_BOTARY
{
	DWORD				dwID;
	MONSTER_ITEM_RATE	ItemRate[20];	
};

typedef struct BASEMONSTER
{
	DWORD				dwID;
	char				szMonsterName_Kor[50];
	char				szMonsterName_Eng[25];
	
	WORD				wBaseLevel;
	BYTE				bRace;					// 
					
	BYTE				bClass;					// 종족
	BYTE				bSize;
	BYTE				bScale;					// 스케일
	DWORD				dwBaseHP;
	WORD				wBaseMP;
	DWORD				dwBaseAA;
	WORD				wBaseDP;
	DWORD				dwBaseAD_Min;
	DWORD				dwBaseAD_Max;
	WORD				wBaseAW;
	WORD				wBaseMS;
	BYTE				bBaseBR;
	BYTE				bLHP;
	BYTE				bLMP;
	BYTE				bLAA;
	BYTE				bLDP;
	MIN_MAX				wLAD;
	BYTE				bLMS;
	BYTE				bLBR;
	BYTE				bLRF;
	BYTE				bLRI;
	BYTE				bLRL;
	BYTE				bLRPo;
	BYTE				bLRPh;
	DWORD				m_dwEgo;				
	DWORD				m_dwStr;				
	DWORD				m_dwInt;				
	DWORD				m_dwDex;				
	DWORD				m_dwVit;				

	DWORD				dwExp;

	BYTE				bAD_type;
	WORD				wA_Range;
	WORD				wAW;
	WORD				wMS;
	short				wFire_resist;
	short				wIce_resist;
	short				wPhy_resist;
	short				wLight_resit;
	short				wPoi_resist;
	short				wAll_resist;

	WORD				wNonSkillRate;
	WORD				wSkillRate;			// 스킬 쏠지 말지를 검사하는 주기
	MONSTER_SKILL		sSkill[MAX_MONSTER_USE_SKILL];
	
	BYTE				bTQS;
	
	WORD				wResponseSpeed;	// 상태 변화 반응 속도
	BYTE				bAI;
	//------------   For Monster AI   ---------------------------
	MONSTER_AI			AI;
	//------------------------------------------------------------
		
	MONSTER_AITYPE		sAiType;
	BYTE				bSR;			// Search_Range
	
	DWORD				dwMin_Money;
	DWORD				dwMax_Money;
	MIN_MAX				wItemCount;		// 몇개의 아이템을 떨굴지를 결정한다.
	
	
	MONSTER_ITEM_RATE	ItemRate[10];
	WORD				wMoneyRate;	
	WORD				wNTR;			// Dont Put the Item Down

	WORD				wOption_0;
	WORD				wOptionRate[2];
	MIN_MAX				wOptionLevel[2];

	BYTE				wDMV;
	WORD				wMODNo;
	WORD				wANMNo;
	WORD				wSC_Shots;
	WORD				wSneutral;
	WORD				wStargeting;
	WORD				wSdamage1;
	WORD				wSdamage2;
	WORD				wSattack1;
	WORD				wSattack2;
	WORD				wSdeath1;
	WORD				wSdeath2;
	WORD				wSmove1;
	WORD				wSmove2;

} *LPBASEMONSTER;
#pragma pack( pop )


typedef CAbyssHash<BASEMONSTER>			BASEMONSTER_HASH,		* LPBASEMONSTER_HASH;

class CMonsterManager
{
private:
	DWORD					m_dwMonsterNum;

public:
	LPBASEMONSTER_HASH		m_pBaseMonsterHash;
	
	DWORD	GetMonsterNum()		{ return m_dwMonsterNum; }
	
//	BOOL	Parse_BaseMonster( char* szFileName );
	BOOL	Save_BaseMonster( char* szFileName );
	BOOL	Load_BaseMonster( char* szFileName );

	LPBASEMONSTER	GetMonster( DWORD dwItemID );
	

public:
	void	Destroy();
	CMonsterManager();
	~CMonsterManager();
};


extern LPBASEMONSTER_HASH			g_pBaseMonsterHash;

