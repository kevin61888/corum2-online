#include "stdafx.h"
#include "ClassMemoryPool.h"
#include "LinkedList.h"


CClassMemoryPool::CClassMemoryPool()
{
	m_wMaxElement		= 0;
	m_pFreeMemoryList	= new CVoidList;
}


CClassMemoryPool::~CClassMemoryPool()
{
	m_pFreeMemoryList->RemoveAll();
	
	delete m_pFreeMemoryList;
	m_pFreeMemoryList = NULL;

	if (m_pMemoryPool)
	{
		delete [] m_pMemoryPool;
		m_pMemoryPool = NULL;
	}
}


DWORD CClassMemoryPool::CreateMemoryPool(WORD wASize, WORD wMaxElement)
{
	m_wMaxElement	= wMaxElement;
	m_wASize		= wASize;
	m_pMemoryPool	= new char[wMaxElement * wASize];
	
	memset(m_pMemoryPool, 0, sizeof(wMaxElement*wASize));

	for(WORD w = 0; w < wMaxElement; ++w)
	{
		FreeAllocation(m_pMemoryPool + w * wASize);
	}

	return 0;
}

void* CClassMemoryPool::Allocation()
{	
	void* pNew = m_pFreeMemoryList->GetHead();
	m_pFreeMemoryList->RemoveHead();
	
	return pNew;
}

void CClassMemoryPool::FreeAllocation(void* pAddr)
{	
	m_pFreeMemoryList->AddTail(pAddr);
	memset(pAddr, 0, m_wASize);	
}