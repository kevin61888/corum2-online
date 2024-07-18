#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

class CSoundControlList : public CVoidList
{
public:
	void FreeData(void* pData)
	{
		if (pData)
		{
			((CSoundControl *)pData)->Release();
			pData = NULL;
		}
	}
};

class CSoundManager
{
public:
	CSoundManager();
	~CSoundManager();

	void	Init(DWORD dwMaxSoundControls);
	// 초기화

	void	DeleteAllSoundControl();
	// 메니저를 삭제할때 


	CSoundControl *			GetSoundControl();
	
	bool					ReleaseSoundControl(CSoundControl * pSoundControl);
	// 강제로 릴리즈할 필요가 있을때 사용하는 함수
	
private:
	DWORD					m_dwMaxSoundControls;
		
	CSoundControlList		m_SoundControlList;
	// 사운드 컨트롤 리스트
};

#endif
// _SOUNDMANAGER_H_
