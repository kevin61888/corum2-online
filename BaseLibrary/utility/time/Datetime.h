/* ==========================================================================
  File : DateTime.h
  Vers.: 1.0
  Plat.: Unix & Windows
  Desc.: DateTime class (for Server-application)
  Auth.: DongHo Byun (cpascal@nownuri.net)
========================================================================== */

#ifndef _CDATETIME_H_
#define _CDATETIME_H_

#include <set>
#include <time.h>

class CDateTime
{
public:
	CDateTime();
	~CDateTime();

	void UpdateTime(void);			// 현재시간 새로 얻기
	time_t GetTime(void);

	void SetHoliday(int, int, int);		// 휴일추가
	bool IsHoliday(void);			// 휴일인지 유무

	int GetCurrentYear(void);
	int GetCurrentMonth(void);
	int GetCurrentDay(void);
	int GetCurrentHour(void);
	int GetCurrentMinute(void);

private:
	time_t m_tTime;
	struct tm m_tm;
	std::set<int> m_HolidaySet;

	bool m_bResult;
	bool m_bChanged;
};

#endif
