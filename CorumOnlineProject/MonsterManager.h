#pragma once

#include "../CommonServer/AbyssHash.h"

#define ITI_TYPE_BASEMONSTER	2000

#pragma pack(push, 1)

typedef struct __MONSTER_SKILL
{
	WORD	wSkill;
	BYTE	bSkillLevel;
} MONSTER_SKILL;

typedef struct __MONSTER_ITEM_RATE
{
	WORD	ItemID;
	WORD	ItemRate;
} MONSTER_ITEM_RATE;

typedef struct __BASEMONSTER
{
	DWORD			dwID;
	char			szMonsterName_Kor[50];
	char			szMonsterName_Eng[25];
	
	WORD			wBaseLevel;
	BYTE			bRank;
	BYTE			bType;
	BYTE			bSize;
	BYTE			bScale;
	BYTE			bCircle;

	DWORD			dwHP;
	DWORD			dwAA;
	DWORD			dwDP;
	BYTE			bBlock;
	DWORD			dwExp;
	DWORD			wAD;

	WORD			wA_Range;
	WORD			wAW;
	WORD			wMS;
	short			wFire_resist;
	short			wIce_resist;
	short			wPhy_resist;
	short			wLight_resit;
	short			wPoi_resist;

	MONSTER_SKILL	Skill[3];
	
	BYTE			bTQS;
	BYTE			bAI;
	BYTE			bSR;		// Search_Range
	BYTE			bSK;		// Space Keep....
	
	DWORD			dwMin_Money;
	DWORD			dwMax_Money;
	
	WORD			wNTR;		// Dont Put the Item Down
	MONSTER_ITEM_RATE	ItemRate[4];
	
	BYTE			wDMV;
	WORD			wMODNo;
	WORD			wANMNo;
	WORD			wSC_Shots;
	WORD			wSneutral;
	WORD			wStargeting;
	WORD			wSdamage1;
	WORD			wSdamage2;
	WORD			wSattack1;
	WORD			wSattack2;
	WORD			wSdeath1;
	WORD			wSdeath2;
	WORD			wSmove;

} BASEMONSTER, *LPBASEMONSTER;
#pragma pack( pop )

typedef CAbyssHash<BASEMONSTER>	BASEMONSTER_HASH, * LPBASEMONSTER_HASH;

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