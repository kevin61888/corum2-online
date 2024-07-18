#include "../../StdAfx.h"
#include "AdvancedPulse.h"

// constructor and destructor
CAdvancedPulse::CAdvancedPulse(void) : m_dwPulse(0), m_dwCustomTick(0xFFFFFFFF), m_dwLastTick(0xFFFFFFFF), m_dwHeavyTrafficCount(0)
{
	m_PulseTimerList.clear();
	SetTicksPerPulse(DEFAULT_TPP);
//	BASEDEBUGLOG("this:0x%08x.", this);
}

CAdvancedPulse::~CAdvancedPulse(void)
{
//	BASEDEBUGLOG("this:0x%08x.", this);	
	if( m_PulseTimerList.size() != 0 ) {
		PulseTimerMapItr itr;
	
		for( itr=m_PulseTimerList.begin(); itr != m_PulseTimerList.end();) {
			delete itr->second;
			itr	= m_PulseTimerList.erase( itr );
		}
	}
}

void CAdvancedPulse::Reset(void)
{
	m_dwLastTick	= 0xFFFFFFFF;
	m_dwCustomTick	= 0xFFFFFFFF;
	m_dwPulse		= 0;
}

void CAdvancedPulse::SetTicksPerPulse(DWORD dwTicksPerPulse)
{
	m_dwTicksPerPulse = dwTicksPerPulse;
}

DWORD CAdvancedPulse::GetLeftTick(void)
{
	DWORD dwCurrentTick = timeGetTime();

	if( dwCurrentTick <= (m_dwCustomTick + m_dwTicksPerPulse) )
		return (m_dwCustomTick + m_dwTicksPerPulse - dwCurrentTick);
	return 0;
}

DWORD CAdvancedPulse::CheckSleep(void)
{
	if( m_dwCustomTick == 0xFFFFFFFF ) {
		m_dwCustomTick = timeGetTime();
		return ++m_dwPulse;
	}
	m_dwCustomTick += m_dwTicksPerPulse;
	m_dwLastTick	= timeGetTime();

	if( m_dwLastTick <= m_dwCustomTick ) {
		// 흐른시간이 TPP보다 작은 경우 Sleep
		m_dwHeavyTrafficCount = 0;

		DWORD dwSleepTime = m_dwCustomTick - m_dwLastTick;
		if( dwSleepTime <= (m_dwTicksPerPulse + 10) ) {
			Sleep( dwSleepTime );
		} else {
			// timeGetTime() Overflow : 49.7d
			m_dwCustomTick = m_dwLastTick;
		}
	} else if( m_dwLastTick <= (m_dwCustomTick + m_dwTicksPerPulse) ) {
		// m_dwTicksPerPulse < dwElapsedTick <= m_dwTicksPerPulse * 2
		// BASEINFOLOG1( "TPP < 흐른시간 < TPP * 2, 트래픽이 %d 입니다.", m_dwHeavyTrafficCount );
	} else {
		WARNLOG1( "Time Overflow : 흐른 시간이TPP 의 두배보다 크거나, 연속적인 트래픽이 %d 입니다.", m_dwHeavyTrafficCount );
		if( (++m_dwHeavyTrafficCount) > 5 )
			m_dwCustomTick = m_dwLastTick;
	}
	return ++m_dwPulse;
}

bool CAdvancedPulse::InstallTimer(OPERATIONIDTYPE nOperation, DWORD dwPulse, Operation pOperation)
{
	LPPULSE_TIMER newPulseTimer		= new PULSE_TIMER;

//	ASSERT( newPulseTimer != NULL );
	newPulseTimer->m_nOperationID	= nOperation;
	newPulseTimer->m_dwMaxPulse		= dwPulse;
	newPulseTimer->m_dwPulse		= 0;
	newPulseTimer->m_pOperation		= pOperation;

	std::pair< PulseTimerMapItr, bool > PulseTimerMapPr;

	PulseTimerMapPr = m_PulseTimerList.insert( PulseTimerMap::value_type(nOperation, newPulseTimer) );
	if( PulseTimerMapPr.second == false ) {
		delete newPulseTimer;
		return false;
	}
	return true;
}

bool CAdvancedPulse::UninstallTimer(OPERATIONIDTYPE nOperation)
{
	PulseTimerMapItr itr;
	
	if( m_PulseTimerList.end() == (itr = m_PulseTimerList.find( nOperation )) )
		return false;
//	ASSERT( lpPulseTimer != NULL );
	delete itr->second;
	m_PulseTimerList.erase( itr );
	return true;
}

void CAdvancedPulse::Operate(void)
{
	for( PulseTimerMapItr itr=m_PulseTimerList.begin(); itr != m_PulseTimerList.end(); ++itr ) {
		LPPULSE_TIMER lpPulseTimer = itr->second;
//		ASSERT( lpPulseTimer != NULL );
		lpPulseTimer->m_dwPulse++;
		if( lpPulseTimer->m_dwMaxPulse <= lpPulseTimer->m_dwPulse ) {
			lpPulseTimer->m_dwPulse = 0;
			if( lpPulseTimer->m_pOperation ) {
				lpPulseTimer->m_pOperation();
			}
		}
	}
}
