#include "stdafx.h"
#include "LinkedlistEx.h"




void CServerList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
    delete pNode;

    m_nCount--;
}

void CDugneonRefreshList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
    delete pNode;

    m_nCount--;
}
void COwnDugneonList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
    delete pNode;

    m_nCount--;
}


void CDugneonRefreshUserList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
    delete pNode;

    m_nCount--;
}

void CPKUserList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
    delete pNode;

    m_nCount--;
}


void CVoidDontDeleteList::FreeNode(CNode* pNode)
{
	if (NULL == pNode)
	{
		return;
	}	

	delete pNode;
	--m_nCount;
}