#include "chrinfo.h"

void ChrInfoLayer::Init(DWORD dwMaxNum, DWORD dwMotionMany, DWORD dwAniMany)
{
	m_pChrInfo = new ChrInfo*[dwMaxNum];

	for(DWORD i = 0; i < dwMaxNum; ++i)
		m_pChrInfo[i] = NULL;
	
	m_dwMotionMany	= dwMotionMany;
	m_dwAniMany		= dwAniMany;
	m_dwMaxNum		= dwMaxNum;
}	

void ChrInfoLayer::Remove()
{
	for(DWORD i = 0; i < m_dwMaxNum; ++i)
	{
		if(m_pChrInfo[i])
			delete [] m_pChrInfo[i];

		m_pChrInfo[i] = NULL;
	}

	if(m_pChrInfo)
		delete [] m_pChrInfo;
	
	m_pChrInfo = NULL;
}

DWORD ChrInfoLayer::GetFrameInfo(DWORD dwKind, DWORD dwAniIndex, DWORD dwMotionIndex, DWORD dwFrame)
{
	return (m_pChrInfo[dwKind]) ?
		m_pChrInfo[dwKind][dwAniIndex*m_dwMotionMany+dwMotionIndex-1].bEffectFrame[dwFrame] : 0;
}

ChrInfo* ChrInfoLayer::GetChrInfo(DWORD dwKind)
{
	return m_pChrInfo[dwKind];
}

SSOUNDINFO*	ChrInfoLayer::GetSoundInfo(DWORD dwKind, DWORD dwAniIndex, DWORD dwMotionIndex)
{
	return (m_pChrInfo[dwKind]) ?
		m_pChrInfo[dwKind][dwAniIndex*m_dwMotionMany+dwMotionIndex-1].sSoundInfo : 0;
}