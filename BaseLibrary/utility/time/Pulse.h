#pragma once

#include <map>
#include <BaseLibrary/Pattern/Singleton.hpp>
#include <BaseLibrary/Pattern/FastDelegate.h>

// definition used in CPulse
#define DEFAULT_TPP		100

using namespace fastdelegate;

class CPulse : public CSingleton<CPulse>
{
private:
	// 펄스의 타이머 정보
	struct PULSE_TIMER {
		int				m_nOperationID;	// 오퍼레이션의 종류
		DWORD			m_dwPulse;
		DWORD			m_dwMaxPulse;	// 오퍼레이션을 실행하는 펄스 수
		FastDelegate0*	m_pDelegate;
	};

	typedef struct PULSE_TIMER							*LPPULSE_TIMER;
	typedef std::map<DWORD, LPPULSE_TIMER>				PulseTimerMap;
	typedef std::map<DWORD, LPPULSE_TIMER>::iterator	PulseTimerMapItr;

	PulseTimerMap	m_PulseTimerList;

	DWORD			m_dwPulse;
	DWORD			m_dwLastTick;
	DWORD			m_dwExtraTick;
	DWORD			m_dwTicksPerPulse;
	DWORD			m_dwHeavyTrafficCount;

	bool			m_TPPOverTwoTime;

public:
	// constructor and destructor
	CPulse(void);
	virtual ~CPulse(void);

	inline DWORD	GetCurrentPulse(void) const;
	inline DWORD	GetLastTick(void) const;
	inline DWORD	GetExtraTick(void) const;
	inline bool		GetTPPOverTwoTime(void) const;
	DWORD			GetLeftTick(void);

	DWORD			CheckSleep(void);
	void			SetTicksPerPulse(DWORD dwTicksPerPulse);

	template <class X> bool InstallTimer( int nOperation, DWORD dwPulse, X* pClass, void (X::* pOperation)() )
	{
		LPPULSE_TIMER newPulseTimer		= new PULSE_TIMER;

	//	ASSERT( newPulseTimer != NULL );
		newPulseTimer->m_nOperationID	= nOperation;
		newPulseTimer->m_dwMaxPulse		= dwPulse;
		newPulseTimer->m_dwPulse		= 0;
		newPulseTimer->m_pDelegate		= new FastDelegate0(pClass, pOperation);

		std::pair< PulseTimerMapItr, bool > PulseTimerMapPr;

		PulseTimerMapPr = m_PulseTimerList.insert( PulseTimerMap::value_type(nOperation, newPulseTimer) );
		if ( PulseTimerMapPr.second == false ) {
			delete newPulseTimer;
			return false;
		}
		return true;
	}
	bool			UninstallTimer(int nOperation);

	void			Operate(void);
};

DWORD CPulse::GetCurrentPulse(void) const
{
	return m_dwPulse;
}

DWORD CPulse::GetLastTick(void) const
{
	return m_dwLastTick;
}

DWORD CPulse::GetExtraTick(void) const
{
	return m_dwExtraTick;
}

bool CPulse::GetTPPOverTwoTime(void) const
{
	return m_TPPOverTwoTime;
}

