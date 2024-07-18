#include "stdafx.h"
#include "OnlyList.h"

COnlyList::COnlyList(DWORD dwMaxNodePoolNum)
{
	m_pNodePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pNodePool, sizeof(CNode), dwMaxNodePoolNum, dwMaxNodePoolNum);
}

COnlyList::~COnlyList()
{
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
        return NULL; // make sure we don't overflow
    }

    return pNode;
}

void COnlyList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) 
		return;
    //FreeData((void*)(pNode->data));
	LALFree(m_pNodePool, pNode);
	m_nCount--;
}


