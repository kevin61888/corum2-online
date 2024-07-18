// DUNGEON_DATA_EX.cpp: implementation of the DUNGEON_DATA_EX class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DUNGEON_DATA_EX.h"
#include "GameDefine.h"
#include "GameControl.h"
#include "User.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DUNGEON_DATA_EX::DUNGEON_DATA_EX()
{
	Initialize();
}

DUNGEON_DATA_EX::~DUNGEON_DATA_EX()
{

}
void	DUNGEON_DATA_EX::Initialize()
{
	memset((DUNGEON_DATA*)this, 0, sizeof(DUNGEON_DATA));
	m_dwRemainSiegeStartTick = 0;
	m_dwRemainSiegeEndTick = 0;
	m_dwRemainLevelStartTick = 0;
	

	bReceivedInfo = 0;
	vPos.x  = 0;vPos.y  = 0;vPos.z = 0;
	pSprMark = NULL;
	bDivision = 0;

	pListPos = NULL;
	pReceivedDungeonPos = NULL;	
	pNext = NULL;
	pPrev = NULL;
	m_hOwnDungeon = NULL;
	m_hSiegingDungeon = NULL;
	m_hGroupDungeon = NULL;

	m_hEventDungeonModel	= NULL;
	m_hEventDungeonEffect	= NULL;
	
	m_byEventDisplayState	= EVENT_DUNGEON_DISPLAY_NONE;
	m_bEventUsed			= FALSE;	

	m_dwEventOpenReadyTime	= 0;	
	m_dwEndOpenReadyTime	= 0;
	m_dwEventStartTick		= 0;
	m_dwEventEndTick		= 0;

	
#if IS_JAPAN_LOCALIZING()
	memset(m_szSchoolName, 0, sizeof(m_szSchoolName));
#endif

}

VOID DUNGEON_DATA_EX::SetDungeonType(DWORD dwID)
{//: hwoarang 050202 
	m_byDungeonType = DungeonIDtoType(dwID);
}

INT	 DUNGEON_DATA_EX::GetDungeonType(VOID)
{
	return m_byDungeonType;
}

WORD	DUNGEON_DATA_EX::GetLevel()
{	// 던전 레벨
	return WORD(m_wUpgradeLevel+m_wInitLevel);
}

WORD	DUNGEON_DATA_EX::GetUpgradeTime()
{	// 던전 업그레이드 시간
	return WORD((1+(sqrt(static_cast<float>(m_wInitLevel)))+m_wUpgradeLevel/2)*1000*60*60);
}

void DUNGEON_DATA_EX::SetOperationMode(BYTE bOperationMode)
{
	m_bOperationType = bOperationMode;

}

void DUNGEON_DATA_EX::StopOperationTime()
{
	m_dwStopStartTick = g_dwCurTick;
//	m_dwStopLevelTick = m_dwRemainLevelStartTick-g_dwCurTick;
//	m_dwStopGradeTick = m_dwRemainGradeCraeteTick-g_dwCurTick;
}

void DUNGEON_DATA_EX::ResumeOperationTime()
{
//	m_dwRemainLevelStartTick=m_dwStopLevelTick+g_dwCurTick;
//	m_dwRemainGradeCraeteTick=m_dwStopGradeTick+g_dwCurTick;
}

int	DUNGEON_DATA_EX::GetIdleTime()
{	// 공성전 할때까지의 휴식기간.
	//return (m_wUpgradeLevel/2+12)*1000*60*60;
	return 60*1000*60*3;
}

void DUNGEON_DATA_EX::SetSiegeStartDestTime(DWORD dwTick)
{
	// 공성전 될 시간 셋팅한다.
	m_bSiege = FALSE;
	m_dwRemainSiegeStartTick = g_dwCurTick+dwTick;
}

void DUNGEON_DATA_EX::SetSiegeEndDestTime(DWORD dwTick)
{
	m_bSiege = TRUE;
	m_dwRemainSiegeEndTick = g_dwCurTick+dwTick;
}	

BOOL DUNGEON_DATA_EX::StartSiegeWarTime()
{ // 공성전 시작해도 되는가?
	if (m_bSiege == FALSE)	// 공성전 아닐때.
	{
		if (IsStartWarTime())
		{
			SetSiegeStartDestTime(GetIdleTime());
			
			return TRUE;
		}
	}
	else	// 공성전일땐 제한 시간있으므로.
	{
		if (IsEndWarTime())
			//SetSiegeEndDestTime(m_wBattleTimeLimit*1000*60);
			SetSiegeEndDestTime(60*1000*60);
	}
	
	return FALSE;
}
	
BOOL DUNGEON_DATA_EX::LevelUpgrade()
{
	// 던전 레벨 업그레이드하기.
	if (IsLevelUpgradeTime())
	{
		m_wUpgradeLevel = (WORD)min(m_wUpgradeLevel+1, 15);
		//dwRemainLevelStartTick = timeGetTime()+GetUpgradeTime()*60*60*1000;
		m_dwRemainLevelStartTick = g_dwCurTick+GetUpgradeTime()*1000*60;
		return TRUE;
	}

	return FALSE;
}
	
DWORD DUNGEON_DATA_EX::GetEntrancePay()
{
	return m_dwEntrance;
}

DWORD DUNGEON_DATA_EX::GetMaxEntrancePay()
{
	return DWORD(sqrt(static_cast<float>(GetLevel()*10))*30 + GetLevel()*10);
}

int DUNGEON_DATA_EX::GetRemainTime(DWORD dwDestTime, DWORD dwCurTick, DWORD dwFlag)
{
	DWORD dwSec = 0;

	if (dwDestTime >= dwCurTick)
	{
		dwSec = (dwDestTime - dwCurTick) / 1000;
	}

	int nHour, nMinute, nSec;	
	nHour	= dwSec / 3600;
	nMinute = ((dwSec % 3600) / 60) % 60;
	nSec	= (dwSec - (3600 * nHour + 60 * nMinute)) % 60;

	switch(dwFlag)
	{
		case TIME_HOUR:
			return nHour;
			
		case TIME_MIN:
			return nMinute;
			
		case TIME_SEC:
			return nSec;
	}

	return -1;
}

BOOL DUNGEON_DATA_EX::IsConquer()
{
	return (m_dwID >= 4000)  && (m_dwID < 5000);
}

BOOL DUNGEON_DATA_EX::IsKaien()
{
	return m_dwID == 10;
}


BOOL DUNGEON_DATA_EX::IsStartWarTime()
{
	return int(m_dwRemainSiegeStartTick - g_dwCurTick) <= 0;
}
BOOL DUNGEON_DATA_EX::IsEndWarTime()
{
	return int(m_dwRemainSiegeEndTick - g_dwCurTick) <= 0;
}
BOOL	DUNGEON_DATA_EX::IsLevelUpgradeTime()
{
	return int(m_dwRemainLevelStartTick - g_dwCurTick) <= 0;
}
DWORD DUNGEON_DATA_EX::GetCPTime()
{
	return GetLevel()/10+10*1000;
}
void DUNGEON_DATA_EX::SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerName, char* pszGuildName)
{
	m_dwOwnerIndex = dwOwnerIndex;
	m_dwOwnerGuildNum = dwGuildID;
	__lstrcpyn(m_szOwner, pOwnerName, MAX_NICK_REAL_NAME);
	__lstrcpyn(m_szManager, pszGuildName, MAX_GUILD_NAME_REAL_LENGTH);
}

BOOL DUNGEON_DATA_EX::IsDungeonOwner(CUser* pUser)
{
	if (g_pMainPlayer == pUser)
		return (m_dwOwnerIndex == pUser->m_dwUserIndex || (m_dwOwnerGuildNum && pUser->m_dwGuildId == m_dwOwnerGuildNum && g_pMainPlayer->m_byRank == 1));
	else
		return (m_dwOwnerIndex == pUser->m_dwUserIndex) || (m_dwOwnerGuildNum && pUser->m_dwGuildId == m_dwOwnerGuildNum);
	
	return 0;
}