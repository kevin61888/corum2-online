#if !defined(AFX_CLASSMEMORYPOOL_H__B32FD358_8D1A_4A58_9D7D_68A10F812C98__INCLUDED_)
#define AFX_CLASSMEMORYPOOL_H__B32FD358_8D1A_4A58_9D7D_68A10F812C98__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CVoidList;


class CClassMemoryPool
{
	WORD		m_wMaxElement;
	WORD		m_wASize;
	CVoidList*	m_pFreeMemoryList;
	char*		m_pMemoryPool;

public:

	CClassMemoryPool();
	virtual ~CClassMemoryPool();

	DWORD	CreateMemoryPool(WORD wASize, WORD wMaxElement);
	void*	Allocation();
	void	FreeAllocation(void* pAddr);
};


#endif 
