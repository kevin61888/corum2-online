// MSoundControl.cpp: implementation of the MSoundControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSoundControl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MSoundControl::MSoundControl()
{

}

MSoundControl::~MSoundControl()
{

}

BOOL MSoundControl::OnCreate(MSoundManager *pManager, DWORD dwMaxSoundHandleCount)
{
	// 디벨로퍼 모드 디파인
	if( pManager == 0 || dwMaxSoundHandleCount == 0 || dwMaxSoundHandleCount >= 0xbbbbbbbb)
		_asm int 3;



	// 메니져.
	m_pManager		=	pManager;

	// 사운드 핸들들.
	m_dwSoundHandleCount		=	dwMaxSoundHandleCount;									// 사운드 핸들 카운트. 버퍼 크기.
	m_pSoundFileHandle			=	new	SOUND_FILE_HANDLE[m_dwSoundHandleCount];			// 핸들 버퍼.
	memset( m_pSoundFileHandle, 0, sizeof(SOUND_FILE_HANDLE)*m_dwSoundHandleCount);

	// 사운드 이펙트.
	m_dwMaxSoundEffectCount		=	10;					// 최대 사운드 이펙트 갯수. 버퍼 크기. 임시로 10이다. 나중엔 링크드리스트로 갯수 제한 없이....
	m_dwSoundEffectCount		=	0;						// 현재 잡혀있는 사운드 이펙트 갯수.
	m_ppSoundEffect				=	new ISoundEffect*[m_dwMaxSoundEffectCount];							// 사운드 이펙트 버퍼.
	
	return TRUE;
}

DWORD MSoundControl::Release()
{
	return	0;
}

void MSoundControl::Delete()
{
	
}

DWORD MSoundControl::Run()
{
	return	0;
}
