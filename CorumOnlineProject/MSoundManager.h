// MSoundManager.h: interface for the MSoundManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSOUNDMANAGER_H__6CD50D69_6736_4921_A117_7F63088E919F__INCLUDED_)
#define AFX_MSOUNDMANAGER_H__6CD50D69_6736_4921_A117_7F63088E919F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundLib.h"
#include "MSoundControl.h"



class MSoundManager  
{
public:

	BOOL		Init(DWORD dwMaxSoundControlCount,  DWORD dwMaxSoundFileHandleCount, DWORD dwMaxSoundEffectCount, BOOL bDebugMode);
	// 초기화.
	// 최대 컨트롤 갯수.
	// 최대 파일핸들 갯수,
	// 최대 이펙트 갯수,
	// 디버그모드.

	BOOL		CreateSoundControl( MSoundControl** ppSoundControl ,DWORD dwMaxSoundHandle);
	// 컨트롤을 생성한다.

	
	
	void		Delete();

	
	
	MSoundManager();
	~MSoundManager();

protected:

	BOOL		ReleaseSoundLib();
	BOOL		InitSoundLib();
	
	// 상태정보.
	BOOL				m_bDebugMode;
	IMilesSoundLib*		m_pSoundLib;

	// 컨트롤들.
	DWORD				m_dwMaxSoundControlCount;


	// 파일 핸들 관리.
	DWORD				m_dwMaxSoundFileHandleCount;

	// 이펙트 관리.
	DWORD				m_dwMaxSoundEffectCount;

};

MSoundManager*			g_pSoundManager		=	0;

MSoundControl*			g_pInterfaceSound	=	0;
MSoundControl*			g_pPCSound			=	0;

#endif // !defined(AFX_MSOUNDMANAGER_H__6CD50D69_6736_4921_A117_7F63088E919F__INCLUDED_)
