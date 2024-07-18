#include "soundcontrol.h"

void CSoundControl::Release()
{
	// 이펙트 헨들을 날린다.
	POSITION_	pos;
	POSITION_	PrevPos;

	pos = m_SoundEffectList.GetHeadPosition();
	while(pos)
	{
		PrevPos = pos;
		ISoundEffect * pSoundEffect = (ISoundEffect *)m_SoundEffectList.GetNext(pos);
		if (pSoundEffect)
		{
			pSoundEffect->Stop();
			m_SoundEffectList.RemoveAt(PrevPos);
			pSoundEffect = NULL;
		}
	}

	pos = m_SoundFileList.GetHeadPosition();
	while(pos)
	{
		PrevPos = pos;
		SOUND_FILE_HANDLE pSoundFile = m_SoundFileList.GetNext(pos);
		if (pSoundFile )
		{			
			m_SoundFileList.RemoveAt(PrevPos);
			pSoundFile = NULL;
		}
	}
}
// -----------------------------------------------------
// Name			: CSoundControl::GetSoundEffect(const char * szSoundFileName, bool bLoop)
// Desc			: 파일 헨들을 얻은후 이펙트 헨들을 넘긴다.
// Purpose		: 파일 이펙트 핸들을 얻는다.
// return		:
// Date/Code By : 040406 / seven
// -----------------------------------------------------
ISoundEffect * CSoundControl::GetSoundEffect(const char * szSoundFileName, VECTOR3 * pPos, BOOL bLoop)
{
	// 사운드 파일 헨들을 얻는다.
	SOUND_FILE_HANDLE pSoundFileHandle = new SOUND_FILE_HANDLE;
	if (NULL == pSoundFileHandle)
	{	
		return NULL;
	}

	::GetSoundEffectFile(&pSoundFileHandle, (char *)(const char*)szSoundFileName);
	if (0xcdcdcdcd == (unsigned int)pSoundFileHandle ||
		NULL == pSoundFileHandle)
	{
		return NULL;
	}
	m_SoundFileList.AddHead( pSoundFileHandle );

	// 사운드 이펙트 헨들을 얻는다.

	if (m_SoundEffectList.GetCount() >= DEFAULT_SOUND_MAX_ASYNC_COUNT-1)
	{ // 동시 발할수 있는 이펙트 헨들의 개수가 현재 게임 코딩에 8개로 제한되어있는관계로
	  // 7개 이상이 차면 알아서 지워준다. 숫자 칠은 define으로 뺀다.
	  // 루프된 이펙트 헨들은 놔둔다.

		POSITION_	pos;
		POSITION_	PrevPos;

		pos = m_SoundEffectList.GetHeadPosition();
		while(pos)
		{
			PrevPos = pos;
			ISoundEffect * pSoundEffect = (ISoundEffect *)m_SoundEffectList.GetNext(pos);
			if (pSoundEffect)
			{
				if ( !pSoundEffect->IsLoop() && !pSoundEffect->IsAlive())
				{ // 루프도 아니고 발생음이 종료한상태다. (자연스러운 릴리즈를 위한 뺑끼)
					m_SoundEffectList.RemoveAt(PrevPos);
					pSoundEffect = NULL;
				}
			}
		}		
	}

	ISoundEffect * pSoundEffect;
	::GetSoundEffect(&pSoundEffect, pSoundFileHandle, *pPos);
	if ( 0xcccccccc == (unsigned int)pSoundEffect || 
		 NULL == pSoundEffect)
	{
		return NULL;
	}
	m_SoundEffectList.AddHead( pSoundEffect );
	
	return pSoundEffect;
}

