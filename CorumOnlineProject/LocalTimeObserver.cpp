
#include "LocalTimeObserver.h"


LocalTimeObserver::LocalTimeObserver()
{
	Init();
}

LocalTimeObserver::~LocalTimeObserver()
{
}

int	LocalTimeObserver::Init()
{
	ZeroMemory( this, sizeof(LocalTimeObserver));
	m_BaseInterval	= 4000;
	m_Interval		= 1000;
	return 0;
}

int	LocalTimeObserver::UnInit()
{
	//	...
	return 0;
}

int LocalTimeObserver::StartBaseTime()
{
	__asm
	{
		call		dword ptr[GetTickCount]
		mov			ebx, dword ptr[this]
		mov			ecx, eax

		rdtsc
		mov			dword ptr[ebx + 4],	edx			//	high
		mov			dword ptr[ebx + 8], eax			//	low

		cmp			ecx, edx
		jbe			lb_invalid_tick					//	(CR_high < tick) is valid.
		mov			dword ptr[ebx + 0xC], ecx		//	tick
	}

	return 0;

lb_invalid_tick:
	return -1;
}

int LocalTimeObserver::StopBaseTime()
{
	DWORD	base;
	__asm
	{
		call		dword ptr[GetTickCount]
		mov			ebx, dword ptr[this]
		mov			ecx, dword ptr[ebx + 0xC]		//	_tick
		mov			esi, eax						//	tick
		sub			eax, ecx						//	[ tick ]
		cmp			eax, dword ptr[ebx + 0x14]		//	check base interval
		jbe			lb_not_enough_tick
		mov			ecx, eax
		mov			dword ptr[ebx + 0xC], esi

		mov			esi, dword ptr[ebx + 4]			//	_high
		mov			edi, dword ptr[ebx + 8]			//	_low
		rdtsc
		mov			dword ptr[ebx + 4],	edx			//	high
		mov			dword ptr[ebx + 8], eax			//	low
		sub			eax, edi
		sbb			edx, esi						//	[ high,low ]

		div			ecx
		mov			dword ptr[ebp - 8], eax			//	base

		//	sub info
		mov			dword ptr[ebx + 0x14], ecx		//	base interval
	}

	m_Base = (float) base;
	return 0;

lb_not_enough_tick:
	return -1;
}

int LocalTimeObserver::Checkup(int* pOverTick)
{
	DWORD	base;
	float	overtick;

	__asm
	{
		call		dword ptr[GetTickCount]
		mov			ebx, dword ptr[this]
		mov			ecx, dword ptr[ebx + 0xC]		//	_tick
		mov			esi, eax						//	tick
		sub			eax, ecx						//	[ tick ]
		cmp			eax, dword ptr[ebx + 0x18]		//	check interval
		jbe			lb_not_enough_tick
		mov			ecx, eax
		mov			dword ptr[ebx + 0xC], esi

		mov			esi, dword ptr[ebx + 4]			//	_high
		mov			edi, dword ptr[ebx + 8]			//	_low
		rdtsc
		mov			dword ptr[ebx + 4],	edx			//	high
		mov			dword ptr[ebx + 8], eax			//	low
		sub			eax, edi
		sbb			edx, esi						//	[ high,low ]
		div			ecx
		mov			dword ptr[ebp - 8], eax

		//	sub info
	}

	overtick = (m_Base / (float)base) * 100.0f;
	*pOverTick = (int)overtick - 100;

	return 0;

lb_not_enough_tick:
	return -1;
}


void InitializeClietSpeedHackChecker()
{
	LocalTimeObserver* pObserver = LocalTimeObserver::GetInstance();

	pObserver->StartBaseTime();

	Sleep(4100);

	pObserver->StopBaseTime();
}

BOOL IsSpeedHackClient()
{
	int	overtick = 0;

	if(LocalTimeObserver::GetInstance()->Checkup( &overtick) < 0)
		return FALSE;

	// overtick이 MAX_OVERTICKS 이상이면 hack으로 판별..
	if(overtick > MAX_OVERTICKS) return TRUE;

	return FALSE;
}