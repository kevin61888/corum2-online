#include "stdafx.h"
#include "OnlyList.h"


COnlyList::COnlyList(DWORD dwMaxNodePoolNum)
{
	m_pNodePool = CreateStaticMemoryPool();
	
	if(!InitializeStaticMemoryPool(m_pNodePool, sizeof(CNode), dwMaxNodePoolNum, dwMaxNodePoolNum))
	{
		_asm int 3
	}
}


COnlyList::~COnlyList()
{
	RemoveAll();

	if(m_pNodePool)
	{
		ReleaseStaticMemoryPool(m_pNodePool);
		m_pNodePool = NULL;
	}
}

CNode* COnlyList::NewNode(CNode* pPrev, CNode* pNext)
{
    CNode* pNode;

    pNode = (CNode*)LALAlloc(m_pNodePool);
	if (pNode == NULL) 
		return NULL;

    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    pNode->data  = NULL;
    m_nCount++;
	
    if (m_nCount <= 0)
    {
        FreeNode(pNode);
        return NULL; 
    }

    return pNode;
}

void COnlyList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) 
		return;
    
	LALFree(m_pNodePool, pNode);

	m_nCount--;
}


