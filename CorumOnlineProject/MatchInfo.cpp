#include "stdafx.h"
#include "matchinfo.h"
#include "CodeFun.h"

CMatchInfo::CMatchInfo()
{
	memset(m_szInfo, 0, sizeof(m_szInfo));
	
	Init();
}

CMatchInfo::~CMatchInfo()
{
}

void CMatchInfo::Init()
{			
	m_bDisplayMatchInfoFlag	= 0;
	m_dwFirstTick			= 0;
	m_dwCurTick				= 0;
	m_dwMatchTimeTick		= 0;
	m_dwRemainedTick		= 0;

	// set sprite position
	m_MatchInfoBackBoardSpr.vPos.x = 0;
	m_MatchInfoBackBoardSpr.vPos.y = 40;
	m_MatchInfoBackBoardSpr.v2Scaling.x = 80;
	m_MatchInfoBackBoardSpr.v2Scaling.y = 25;
}

void CMatchInfo::Release()
{
	if (GetBackBoardSprite())
	{
		m_MatchInfoBackBoardSpr.pSpr->Release();
		m_MatchInfoBackBoardSpr.pSpr = NULL; 
	}
}

void CMatchInfo::InitMatchInfoString()
{
	int i; 
	for (i = 0; i < MATCH_INFO_STRING_MAX_COUNT; i++)
	{
		memset(m_szInfo[i], 0, sizeof(m_szInfo[i]));
	}
}

// Get

BOOL CMatchInfo::IsMatching() const 
{
	return (GetElapseTimeTick() > 0 );
}

BOOL CMatchInfo::IsDisplayMatch() const
{
	return m_bDisplayMatchInfoFlag;
}

DWORD CMatchInfo::GetElapseTimeTick() const
{
	if (m_dwCurTick && m_dwFirstTick)
		return m_dwCurTick - m_dwFirstTick;
	else
		return 0;
}

DWORD CMatchInfo::GetRemainSec() const
{
	int dwRemainTimeTick = DWORD(m_dwMatchTimeTick - GetElapseTimeTick())/1000;	// 초단위로 
	return (0 != dwRemainTimeTick) ? dwRemainTimeTick : 0;
}

IDISpriteObject* CMatchInfo::GetBackBoardSprite() const
{
	return m_MatchInfoBackBoardSpr.pSpr;
}

// Set

void CMatchInfo::SetDisplayMatchFlag(BOOL bDisplayFlag)
{
	m_bDisplayMatchInfoFlag = bDisplayFlag;

	if (FALSE == bDisplayFlag)
		Init();
}

void CMatchInfo::SetMatchTimeTick(DWORD dwMatchTimeTick)
{
	m_dwMatchTimeTick = dwMatchTimeTick;
}

void CMatchInfo::SetCurTick(DWORD dwCurTick)
{
	if ( !m_dwFirstTick )
	{
		m_dwFirstTick = dwCurTick;
	}

	m_dwCurTick = dwCurTick;
}

char *	CMatchInfo::GetInfoString(MATCH_INFO_STRING eMatchInfo) const
{
	return (char *)&m_szInfo[(int)eMatchInfo][0];
}

void	CMatchInfo::SetInfoString(MATCH_INFO_STRING eMatchInfo, const char * szMatchInfo)
{
	if (szMatchInfo)
		__lstrcpyn(m_szInfo[(int)eMatchInfo], szMatchInfo, lstrlen(szMatchInfo));
}


void	CMatchInfo::SetBackBoardSprite(IDISpriteObject* pSpr)
{	
	m_MatchInfoBackBoardSpr.pSpr = pSpr;
}

void	CMatchInfo::RenderInfo()
{	
	int x = 0;
	int y = 40;
	m_MatchInfoBackBoardSpr.vPos.x = float(x);
	m_MatchInfoBackBoardSpr.vPos.y = float(y);

	// BackGround
	if (m_MatchInfoBackBoardSpr.pSpr)
	{
		g_pRenderer->RenderSprite(m_MatchInfoBackBoardSpr.pSpr, &m_MatchInfoBackBoardSpr.v2Scaling, 0.0, &m_MatchInfoBackBoardSpr.vPos, NULL, 0xffffffff, 5, 0);
	}

	// [남은대전시간]		
	RenderFont(m_szInfo[MATCH_INFO_STRING_TIME], x+5, x+250, y+10, y+40, 2000);
	RenderFont(m_szInfo[MATCH_INFO_STRING_RAMAINED_TIME], x+5, x+250, y+30, y+60, 2000);
}


