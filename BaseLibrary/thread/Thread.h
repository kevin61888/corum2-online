#pragma once

#include <winsock2.h>
#include <windows.h>


class CThread
{
protected:
	HANDLE	m_hThread;


public:
	// constructor and destructor
	CThread(void) : m_hThread(INVALID_HANDLE_VALUE)	{}
	virtual ~CThread(void)							{}


	// interface
	virtual unsigned int Run(void) = 0;		// 실제 실행 되는 루프를 넣는다.
	virtual BOOL End(void) = 0;				// 루프가 끝날 수 있는 루틴을 넣는다.


	// implementation
	typedef unsigned int(__stdcall *LPThreadFunc)(void*);
	static inline unsigned int __stdcall ThreadFunc(void* pArg);

	inline void		SetHandle(HANDLE hHandle);
	inline HANDLE	GetHandle(void);
};

inline unsigned int __stdcall CThread::ThreadFunc(void* pArg)
{
	return static_cast<CThread*>(pArg)->Run();
}

void CThread::SetHandle(HANDLE hHandle)
{
	m_hThread = hHandle;
}

HANDLE CThread::GetHandle(void)
{
	return m_hThread;
}
