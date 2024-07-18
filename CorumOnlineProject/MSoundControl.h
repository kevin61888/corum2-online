// MSoundControl.h: interface for the MSoundControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSOUNDCONTROL_H__71FF33A4_39ED_4387_8154_78935C7E9A41__INCLUDED_)
#define AFX_MSOUNDCONTROL_H__71FF33A4_39ED_4387_8154_78935C7E9A41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SoundLib.h"

class MSoundManager;

class MSoundControl  
{
public:

	DWORD Run();
	// 리턴값은 지금 플레이 되고 있는 사운드 이펙트 갯수.

	DWORD			Release();
	// 컨트롤 해제.



	// 사운드 메니져가 건드릴 이벤트핸들러. 직접 건드리지 말도록.
	BOOL			OnCreate( MSoundManager* pManager, DWORD dwMaxSoundHandleCount);
	// 사운드 메니쳐에서 첨 생성되고 호출되는 초기화 함수.

	MSoundControl();
	~MSoundControl();

protected:
	void Delete();

	// 글로벌 메니져.
	MSoundManager*			m_pManager;

	// 사운드 핸들들.
	DWORD					m_dwSoundHandleCount;						// 사운드 핸들 카운트. 버퍼 크기.
	SOUND_FILE_HANDLE*		m_pSoundFileHandle;							// 핸들 버퍼.

	// 사운드 이펙트.
	DWORD					m_dwMaxSoundEffectCount;					// 최대 사운드 이펙트 갯수. 버퍼 크기.
	DWORD					m_dwSoundEffectCount;						// 현재 잡혀있는 사운드 이펙트 갯수.
	ISoundEffect**			m_ppSoundEffect;							// 사운드 이펙트 버퍼.

};

#endif // !defined(AFX_MSOUNDCONTROL_H__71FF33A4_39ED_4387_8154_78935C7E9A41__INCLUDED_)
