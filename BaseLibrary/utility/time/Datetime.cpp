/* ==========================================================================
  File : DateTime.cpp
  Vers.: 1.0
  Plat.: Unix & Windows
  Desc.: DateTime class (for Server-application)
  Auth.: DongHo Byun (cpascal@nownuri.net)
========================================================================== */

#include "../../StdAfx.h"
#include "DateTime.h"

CDateTime::CDateTime()
{
	UpdateTime();
}

CDateTime::~CDateTime()
{
}

time_t CDateTime::GetTime(void)
{
	return m_tTime;
}

void CDateTime::UpdateTime(void)
{
	m_tTime = time(0);
#ifdef _WIN32
	memcpy( &m_tm, localtime( &m_tTime ), sizeof( struct tm ) );
#else
	localtime_r( &m_tTime, &m_tm );
#endif
	m_bChanged = true;
}

void CDateTime::SetHoliday(int iYear, int iMon, int iDay)
{
	m_HolidaySet.insert( (iYear << 15) + (iMon << 7) + iDay );
}

int CDateTime::GetCurrentYear(void)
{
	return ((int) m_tm.tm_year) + 1900;
}

int CDateTime::GetCurrentMonth(void)
{
	return (int) m_tm.tm_mon + 1;
}

int CDateTime::GetCurrentDay(void)
{
	return (int) m_tm.tm_mday;
}

int CDateTime::GetCurrentHour(void)
{
	return (int) m_tm.tm_hour;
}

int CDateTime::GetCurrentMinute(void)
{
	return (int) m_tm.tm_min;
}

bool CDateTime::IsHoliday(void)
{
	if( m_bChanged == false )
		return m_bResult;
	if( (m_tm.tm_wday == 0) || (m_tm.tm_wday == 6) ) {
		m_bResult = true;
		return m_bResult;
	}
	if( m_HolidaySet.find( (GetCurrentYear() << 15) + (GetCurrentMonth() << 7) + GetCurrentDay() ) == m_HolidaySet.end() )
		m_bResult = false;
	else
		m_bResult = true;
	return m_bResult;
}

