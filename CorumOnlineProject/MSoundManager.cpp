// MSoundManager.cpp: implementation of the MSoundManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MSoundManager.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MSoundManager::MSoundManager()
{

}

MSoundManager::~MSoundManager()
{

}

BOOL MSoundManager::Init(DWORD dwMaxSoundControlCount,  DWORD dwMaxSoundFileHandleCount, DWORD dwMaxSoundEffectCount, BOOL bDebugMode)
{
	BOOL	bResult;

	m_dwMaxSoundControlCount	=	dwMaxSoundControlCount;

	// 핸들 카운트.
	m_dwMaxSoundFileHandleCount		=	dwMaxSoundFileHandleCount;

	// 이펙트 카운트.
	m_dwMaxSoundEffectCount			=	dwMaxSoundEffectCount;

	// 디버그?
	m_bDebugMode	=	bDebugMode;
	
	// 사운드 라이브러리 초기화.
	bResult	=	InitSoundLib();
	if( bResult == FALSE)
	{
		// redist 디렉토리가 안맞던가, 사운드카드가 없거나 해서 실패했다.
		// 소리 안나야지 뭐.
		return	FALSE;
	}



	return	TRUE;
}

BOOL MSoundManager::InitSoundLib()
{
	BOOL	bResult;

	IMilesSoundLib*			pSoundLib		=	0;

	bResult	=	CreateSoundLib( &pSoundLib);
	if( bResult == FALSE)
		_asm int 3;

	SOUND_ERROR_CODE	result	=	pSoundLib->Init( m_dwMaxSoundFileHandleCount, m_dwMaxSoundEffectCount, "redist", FALSE, FALSE);

	if( result != SOUND_ERROR_NOERROR)
	{
		// 에러코드가 리턴되었다.
		
		// 사운드 라이브러리 해제시키고 리턴.
		pSoundLib->Delete();
		return	FALSE;
	}

	m_pSoundLib		=	pSoundLib;
	

	return	TRUE;
}

BOOL MSoundManager::ReleaseSoundLib()
{
	m_pSoundLib->Delete();
	
	return	TRUE;
}

void MSoundManager::Delete()
{

	if( m_pSoundLib)
	{
		ReleaseSoundLib();
	}



}

BOOL MSoundManager::CreateSoundControl(MSoundControl** ppSoundControl, DWORD dwMaxSoundHandle)
{
	MSoundControl*		pControl	=	new	MSoundControl;

	BOOL	bResult;

	bResult	=	pControl->OnCreate( this, dwMaxSoundHandle);
	if( bResult == FALSE)
	{
		pControl->Release();
		return	FALSE;
	}

	*ppSoundControl	=	pControl;

	return	TRUE;


}
