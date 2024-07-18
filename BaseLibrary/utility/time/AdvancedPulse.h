#pragma once

#include <winsock2.h>
#include <windows.h>

#include <map>

// definition used in CAdvancedPulse
#define DEFAULT_TPP		100
typedef  void (*Operation)(void);

typedef DWORD	OPERATIONIDTYPE;

class CAdvancedPulse
{
private:
	// 펄스의 타이머 정보
	struct PULSE_TIMER {
		OPERATIONIDTYPE	m_nOperationID;	// 오퍼레이션의 종류
		DWORD			m_dwPulse;
		DWORD			m_dwMaxPulse;	// 오퍼레이션을 실행하는 펄스 수
		Operation		m_pOperation;	// 오퍼레이션의 실행 프로시져
	};

	typedef struct PULSE_TIMER									*LPPULSE_TIMER;
	typedef std::map<OPERATIONIDTYPE, LPPULSE_TIMER>			PulseTimerMap;
	typedef std::map<OPERATIONIDTYPE, LPPULSE_TIMER>::iterator	PulseTimerMapItr;

	PulseTimerMap	m_PulseTimerList;

	DWORD			m_dwPulse;
	DWORD			m_dwLastTick;
	DWORD			m_dwCustomTick;
	DWORD			m_dwTicksPerPulse;
	DWORD			m_dwHeavyTrafficCount;

public:
	// constructor and destructor
	CAdvancedPulse(void);
	~CAdvancedPulse(void);

	void			Reset(void);
	inline DWORD	GetCurrentPulse(void) const;

	inline DWORD	GetLastTick(void) const;
	inline DWORD	GetExtraTick(void) const;
	inline bool		GetTPPOverTwoTime(void) const;
	DWORD			GetLeftTick(void);

	void			SetTicksPerPulse(DWORD dwTicksPerPulse);
	DWORD			CheckSleep(void);

	bool			InstallTimer(OPERATIONIDTYPE nOperation, DWORD dwPulse, Operation pOperation);
	bool			UninstallTimer(OPERATIONIDTYPE nOperation);

	void			Operate(void);
};

DWORD CAdvancedPulse::GetCurrentPulse(void) const
{
	return m_dwPulse;
}

DWORD CAdvancedPulse::GetLastTick(void) const
{
	return m_dwLastTick;
}
