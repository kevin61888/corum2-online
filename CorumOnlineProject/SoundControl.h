#ifndef _SOUNDCONTROL_H_
#define _SOUNDCONTROL_H_

#include "GlobalSoundHandles.h"
#include "SoundLib.h"
#include "VoidList.h"

class SoundEffectList : public CVoidList
{
public:
	void FreeData(void* pData)
	{
		if (pData)
		{
			((ISoundEffect *)pData)->Release();
			pData = NULL;
		}
	}
};


class SoundFileList : public CVoidList
{
public:
	void FreeData(void* pData)
	{
		if (pData)
		{
			SAFE_REL_SOUND_FILE((SOUND_FILE_HANDLE)pData);
		}
	}
};

// -----------------------------------------------------
// Name			: CSoundControl
// Desc			: 사운드 이펙트와 파일 헨들을 관리한다.
// Purpose		:
// Date/Code By : 040406 / Seven
// -----------------------------------------------------
class CSoundControl
{
public:
	CSoundControl() 
	{
	}

	~CSoundControl()
	{
		Release();
	}

// void Init(DWORD dwSoundEffectHandles, DWORD dwSoundFileHandles);
// 현재는 이팩트, 파일 사운드 개수를 제한하지 않는다.

	ISoundEffect * GetSoundEffect(const char * szSoundFileName, VECTOR3 * pPos, BOOL bLoop);
	// 사운드 이펙트를 얻는다.
	// 파일 이름(경로 포함)과 사운드가 루프 될건지 설정한다.
	// 내부에서 파일 핸들을 연다.

	void Release();
	// 모든 이팩트와 파일 사운드를 삭제한다.

private:
//	DWORD			m_dwSoundEffectHandles;
//	DWORD			m_dwMaxSoundFileHandles;

	SoundEffectList	m_SoundEffectList;
	SoundFileList	m_SoundFileList;
};

#endif
// _SOUNDCONTROL_H_
