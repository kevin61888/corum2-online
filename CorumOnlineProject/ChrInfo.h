// Chr의 Effect Frame, 효과음등의 정보를 가지고 있는 구조체 정의.

#pragma once
#include "Windows.h"

#define _MAX_FRAMESET	10
#define	_MAX_SOUND		10
#define _CHRINFO_FRAME0	0
#define _CHRINFO_FRAME1	1
#define _CHRINFO_FRAME2	2
#define _CHRINFO_FRAME3	3
#define _CHRINFO_FRAME4	4
#define _CHRINFO_FRAME5	5
#define _CHRINFO_FRAME6	6
#define _CHRINFO_FRAME7	7
#define _CHRINFO_FRAME8	8
#define _CHRINFO_FRAME9	9

struct SSOUNDINFO
{
	DWORD dwFrame;
	DWORD dwSoundNum;
};

struct ChrInfo
{
	
	BYTE	bEffectFrame[_MAX_FRAMESET];
	SSOUNDINFO	sSoundInfo[_MAX_SOUND];
	
	ChrInfo()
	{
		memset( this, 0, sizeof( ChrInfo ) );
	}		
};

struct ChrInfoLayer
{
	DWORD		m_dwMotionMany;
	DWORD		m_dwAniMany;
	DWORD		m_dwMaxNum;
	ChrInfo**	m_pChrInfo;
	void	Init(DWORD dwMaxNum, DWORD dwMotionMany, DWORD dwAniMany);
	
	void	Remove();
	DWORD GetFrameInfo(DWORD dwKind, DWORD dwAniIndex, DWORD dwMotionIndex, DWORD dwFrame);
	SSOUNDINFO* GetSoundInfo(DWORD dwKind, DWORD dwAniIndex, DWORD dwMotionIndex);
	ChrInfo*	GetChrInfo(DWORD dwKind);
	
};