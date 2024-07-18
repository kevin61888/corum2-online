//=======================================================================
// CSoundEffectBasketCase
#include "GlobalSoundHandles.h"
#include "GameMenuWnd.h"
#include <crtdbg.h>
#include "Define.h"
#include "User.h"
#include "InitGame.h"
#include "GameControl.h"

GLOBAL_SOUND_LIB_HANDLES	g_SoundLibHandles;	
CSoundEffectBasketCase		g_SoundEffectBasketCase;

IMilesSoundLib*				g_pSoundLib				= NULL;
char*						g_pszSoundErrorString	= NULL;

VECTOR3						g_v3InterfaceSoundPos	= {0, };

#ifndef __GLOBALSOUNDHANDLES_H__
BOOL						g_bInitSoundEnalbe = TRUE;	// 로그인시 쾅하는 소리 제거
#endif

//===================================
// Purpose : 초기화 
//===================================
CSoundEffectBasketCase::CSoundEffectBasketCase()
{
	memset(this, 0, sizeof(*this));
	Init();
}
//===================================
// Purpose : 마무리
//===================================
CSoundEffectBasketCase::~CSoundEffectBasketCase()
{
	Release();
}
//===================================
// Purpose : 변수 초기화
//===================================
void CSoundEffectBasketCase::Init()
{
	if ( !m_pSoundEffectList )
	{
		m_pSoundEffectList = new CSoundEffectList;
	}
	ForceAllClean();
}
//===================================
// Purpose : 리소스 해제
//===================================
void CSoundEffectBasketCase::Release()
{
	ForceAllClean();
	delete m_pSoundEffectList;
	m_pSoundEffectList = NULL;
}
//===================================
// Purpose : 강제적으로 할당된 리소스 해제
//===================================
void CSoundEffectBasketCase::ForceAllClean()
{
	if (g_pSoundLib == NULL)
		return;

	POSITION_	pos;
	POSITION_	PrevPos;

	pos = m_pSoundEffectList->GetHeadPosition();
	while(pos)
	{
		PrevPos = pos;
		ISoundEffect * pSoundEffect = (ISoundEffect *)m_pSoundEffectList->GetNext(pos);
		if (pSoundEffect)
		{
			pSoundEffect->Stop();
			m_pSoundEffectList->RemoveAt(PrevPos);
			pSoundEffect = NULL;
		}
	}
}
//===================================
// Purpose : 죽어있는 리소스만 해제 
//===================================
void CSoundEffectBasketCase::UpdateToClean()
{
	if (g_pSoundLib == NULL)
		return;

	POSITION_	pos;
	POSITION_	PrevPos;

	pos = m_pSoundEffectList->GetHeadPosition();

	while(pos)
	{
		PrevPos = pos;

		ISoundEffect * pSoundEffect = (ISoundEffect *)m_pSoundEffectList->GetNext(pos);
		if (pSoundEffect && !pSoundEffect->IsAlive())
		{				
			m_pSoundEffectList->RemoveAt(PrevPos);
		}
	}
}
//===================================
// Purpose : 리소스 추가 
//===================================
void CSoundEffectBasketCase::Add(ISoundEffect * pSoundEffect )
{
	if (pSoundEffect == NULL)
		return;

	int iCount = m_pSoundEffectList->GetCount();
	if (iCount % 3 == 0)
	{
		UpdateToClean();
	}

	m_pSoundEffectList->AddTail((void*) pSoundEffect);
}
//===================================
// Purpose : 파일 헨들을 얻어온다. 
// Output : phOutFileHandle 로 받아옴 
//===================================
void GetSoundEffectFile(SOUND_FILE_HANDLE * phOutFileHandle, char * pszFileName)
{
	if ( !g_pSoundLib ) 
		return;

	if (*phOutFileHandle)
	{
		return;
	}

	SOUND_ERROR_CODE	Result;
	Result = g_pSoundLib->CreateSoundEffectFile(phOutFileHandle, pszFileName); 
	if (Result != SOUND_ERROR_NOERROR) 
	{ 
#ifdef DEVELOP_MODE
		char szBuf[255];
		g_pSoundLib->GetErrorString( &g_pszSoundErrorString, Result); 
		wsprintf(szBuf, "Sound : %s(%s)\n", pszFileName, g_pszSoundErrorString );
		OutputDebugString(g_pszSoundErrorString); 
#endif
		return; 
	} 
}
//===================================
// Purpose : 사운드 이펙트 헨들을 얻는다.
//===================================
void GetSoundEffect(ISoundEffect ** ppOutEffect, SOUND_FILE_HANDLE & rhInFileHandle, VECTOR3 & v3Pos)
{
	if ( !g_pSoundLib ) 
		return;

	float fDis = 0;

	if (g_pMainPlayer)
	{

		fDis = CalcDistance(&g_pMainPlayer->m_v3CurPos, &v3Pos);

		if ( fDis >= SOUND_LISTEN_SCOPE_DISTANCE && !(g_v3InterfaceSoundPos == v3Pos) )
		{
			*ppOutEffect = NULL;
			return;
		}
	}

	SOUND_ERROR_CODE	Result;

	if (!rhInFileHandle)
		return;

	Result = g_pSoundLib->CreateSoundEffect(ppOutEffect, rhInFileHandle);
	if (Result != SOUND_ERROR_NOERROR)
	{
		g_pSoundLib->GetErrorString( &g_pszSoundErrorString, Result );
		OutputDebugString( g_pszSoundErrorString );
	}
}
//===================================
// Purpose : 이펙트 사운드 핸들을 이용하여 소리를 출력
//===================================
void PlaySoundEffect(ISoundEffect * pSoundEffect, VECTOR3 * pV3Pos, bool bLoop)
{	
	if ( !g_pSoundLib ) 
		return;

	// sung-han 2005-03-14 로그인시 쾅하는 소리 제거, 사운드 on, off 기능
	if(!g_bInitSoundEnalbe)
		return;

	static VECTOR3	v3Pos = {0.f,};
	CGameMenuWnd * pGameMenuWnd = CGameMenuWnd::GetInstance();

	if (pSoundEffect && pGameMenuWnd->m_bEffectFlag)
	{
		pSoundEffect->SetMaxMinDistance(DEFAULT_SOUND_MAX_DISTANCE, DEFAULT_SOUND_MIN_DISTANCE);

		// sung-han 2005-03-14 거리에 따른 사운드 볼륨 조절 ----------------------------
		float fDis = 0;
		float fEffectVolume = g_fEffectVolume;
		if(g_pMainPlayer && !(*pV3Pos == v3Pos) )
		{
			fDis = CalcDistance(&g_pMainPlayer->m_v3CurPos, pV3Pos);
			fEffectVolume = g_fEffectVolume - (fDis / SOUND_LISTEN_SCOPE_DISTANCE)*g_fEffectVolume;
		}
		if(fEffectVolume < 0 || fEffectVolume > 1)
		{
			fEffectVolume = g_fEffectVolume;
		}
		//------------------------------------------------------------------------------

		
		pSoundEffect->SetVolume(fEffectVolume);
		if ( *pV3Pos == v3Pos )
			pSoundEffect->SetRelativePosition(&v3Pos);
		else
			pSoundEffect->SetPosition(pV3Pos);
		
		pSoundEffect->Play(bLoop);
	}
}
//===================================
// Purpose : 
//===================================
void PlaySoundEffectMaxMin(ISoundEffect * pSoundEffect, VECTOR3 * pV3Pos, bool bLoop, int iMaxDistance, int iMinDistance)
{	
	if ( !g_pSoundLib ) 
		return;

	CGameMenuWnd * pGameMenuWnd = CGameMenuWnd::GetInstance();

	if (pSoundEffect && pGameMenuWnd->m_bEffectFlag)
	{
		pSoundEffect->SetMaxMinDistance((float)iMaxDistance, (float)iMinDistance);
		pSoundEffect->SetPosition(pV3Pos);
		pSoundEffect->Play(bLoop);
	}
}
//===================================
// Purpose : 
//===================================
void PlayBGM(const char * szBGMName)
{
	if (!g_pSoundLib)
		return;

	if (__strcmp((char*)szBGMName, "\0") == 0)
	{
		return;
	}

	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();

	if (NULL == pGameMenuWnd)
	{
		return;
	}
	
	lstrcpy(pGameMenuWnd->m_szLast_BGM_Name, szBGMName);	// 최근 음악 파일 네임을 저장해 둔다.
	
	if (pGameMenuWnd ->m_bBGMFlag == FALSE)
		return;

	StopBGM();	// 일단 재생되고 있는 음악을 중지.

	g_pSoundLib->PlayBGM(szBGMName);

	if (g_fBGMVolume > 1.0f)
		g_fBGMVolume = 1.0f;
	else if( g_fBGMVolume < 0.0f)
		g_fBGMVolume = 0.0f;
		
	SetBGMVolume(g_fBGMVolume);		
}
//===================================
// Purpose : 
//===================================
void SetBGMVolume(float fBGMvolume)
{
	if (g_pSoundLib && fBGMvolume > 0)
		g_pSoundLib->SetBGMVolume(fBGMvolume);
}
//===================================
// Purpose : 
//===================================
void StopBGM()
{
	if (!g_pSoundLib)
		return;

	g_pSoundLib->StopBGM();
}
//===================================
// Purpose : 사운드 해제후 리소스가 제대로 다 해제되었나 검사한다.
//===================================
void Check_Sound_Resource()
{
	int nSoundEffectCount;
	int	nSoundFileCount;

	nSoundEffectCount = g_pSoundLib->GetAllocatedSoundEffectCount();
	nSoundFileCount	= g_pSoundLib->GetAllocatedSoundFileCount();

	if (0 != nSoundEffectCount)
		__asm nop;
	if (0 != nSoundFileCount)
	{
		g_pSoundLib->Run();

	}


}
//===================================
// Purpose : 사운드 해제 
//===================================
void Release_Sound_Resource()
{
	if (NULL == g_pSoundLib)
		return;

	g_SoundLibHandles.Release();
	g_SoundLibHandles.Init();
	
	Check_Sound_Resource();
}