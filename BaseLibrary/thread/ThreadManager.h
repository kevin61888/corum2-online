#pragma once

#include <winsock2.h>
#include <windows.h>
#include <process.h>

#include "Lock.hpp"
#include "Thread.h"


typedef class CThreadManager*		LPCThreadManager;
class CThreadManager
{
private:
	enum { MAX_THREAD_NUM = 15 };

	CThread*		m_pThreads[MAX_THREAD_NUM];
	HANDLE			m_hThreads[MAX_THREAD_NUM];
	
	CBCSLock		m_ThreadLock;

	int				m_nThreadNum;
	int				m_bJoinStarted;


public:
	// constructor and destructor
	CThreadManager(void);
	virtual ~CThreadManager(void);


	bool			RegisterAndRun(CThread* pThread);		// 스레드를 등록한다. 최대 63개까지 등록할 수 있다.
	bool			JoinThread(bool bDelete = true);						// 모든 스레드를 종료시킨다.

	inline int		GetThreadNum(void);
	inline CThread*	GetThreadPtr(int nIndex);

	// 매니저에 등록하지 않고, 그냥 실행 시킨다.
	static HANDLE	Run(CThread* pThread);
	//static bool		Stop(CThread* pThread, DWORD dwTimeout = INFINITE);
};

int CThreadManager::GetThreadNum(void)
{
	return m_nThreadNum;
}

CThread* CThreadManager::GetThreadPtr(int nIndex)
{
	return m_pThreads[nIndex];
}
