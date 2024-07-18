#ifndef __LOCAL_TIME_OBSERVER_H__
#define __LOCAL_TIME_OBSERVER_H__


#pragma once


#include "stdafx.h"


/*
#include "RainStatus.h"
enum TIME_OBSERVER_STATUS
{
	TS_NONE				=	0,
	TS_INIT				=	SET_BIT( 0),
	TS_BASE_COMPLETE	=	SET_BIT( 1)
};
*/


class LocalTimeObserver
{
	DWORD			m_dummy;
	DWORD			m_CR_high;
	DWORD			m_CR_low;
	DWORD			m_Tick;
	float			m_Base;
	DWORD			m_BaseInterval;
	DWORD			m_Interval;

protected:
	LocalTimeObserver();

public:

	~LocalTimeObserver();

	static LocalTimeObserver* GetInstance()
	{
		static LocalTimeObserver cLocalTimeObserver;
		return &cLocalTimeObserver;
	}

	int	Init();
	int	UnInit();

	int	SetBaseInterval( DWORD Interval)		{ m_BaseInterval = Interval;		return 0;	}
	int	GetBaseInterval( DWORD*	pInterval)		{ *pInterval	 = m_BaseInterval;	return 0;	}

	int	GetInterval( DWORD*	pInterval)			{ *pInterval = m_Interval;		return 0;	}
	int	SetInterval( DWORD Interval)			{ m_Interval = Interval;		return 0;	}

	int GetBase( float* pBase)					{ *pBase = m_Base;				return 0;	}

	int StartBaseTime();
	int StopBaseTime();

	int Checkup(int* pOverTick);
};


//////////////////////////////////////////////////////////////////////


#define MAX_OVERTICKS	8	// PERCENT ( %)


void InitializeClietSpeedHackChecker();
BOOL IsSpeedHackClient();


#endif // __LOCAL_TIME_OBSERVER_H__