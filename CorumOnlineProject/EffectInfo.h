// Struct Effect Info....
#pragma once


#define MAX_EFFECT_INFO			200
#define MAX_EFFECT_SOUND_NUM	200
#define MAX_PLAYER_SOUND_NUM	200	
//#define MAX_MONSTER_SOUND_NUM	6
#define MAX_GAMEPLAY_SOUND_NUM  200 

#define EFFECT_TYPE_DAMAGE			11
#define EFFECT_TYPE_STATUS			20		// 분기점..
#define EFFECT_TYPE_NORMAL			21		// 한번 나오고 끝나며, 목적오브켁트의 스폿을 따라다닌다.
#define EFFECT_TYPE_STATUS_TOP		1		
#define EFFECT_TYPE_STATUS_CENTER	2		
#define EFFECT_TYPE_STATUS_BOTTOM	4		

//--- 몬스터 사운 드 시작 번호 ---------//
#define MONSTER_SOUND_NEUTRAL		0
#define MONSTER_SOUND_TARGET		1
#define MONSTER_SOUND_GETHIT		2
#define MONSTER_SOUND_ATTACK		4
#define MONSTER_SOUND_DEATH			6
#define MONSTER_SOUND_MOVE			8


//--- Game Play 에서의 사운트 시작번호 -//
#define GAMEPLAY_HEABYSTONE_WALK	5
#define GAMEPLAY_HEABYSTONE_RUN		1
#define GAMEPLAY_AIRPLANE_LANDING	102
#define GAMEPLAY_AIRPLANE_TAKE_OFF	101

/*
// 이펙트의 정보를 가지고있는 구조체.
struct EffectInfo
{
	char	szEffectName[100];		// 이펙트 이름.
	float	fRange;					// 사정거리.

	BYTE	bType;					// 이펙트의 타입.

	BYTE	bStartSoundNum;			// 이펙트가 시작될때의 사운드.
	BYTE	bFinishSoundNum;		// 이펙트가 끝났을때의 사운드.
};

// 이펙트 표현을 위한 구조체.
struct EffectDesc
{
	DWORD	dwEffectInfoNum;		// 표현할려고 하는 이펙트의 정보 아이디.	
	
	BYTE	bTargetObjectType;		// 타겟 오브젝트의 타입.
	DWORD	dwTargetIndex;			// 타겟의 인덱스.
//////////////////////////////////////////////////////////////////////////////////
	//slowboat add
	float	f_Radcount;
	DWORD	dwFrameCount;
	DWORD	dwTemp[10];
///////////////////////////////////////////////////////////////////////////////

	OBJECT_HANDLE_DESC	hEffect;	// 이펙트의 오브젝트 핸들.
};
*/