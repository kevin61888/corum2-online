#include "stdafx.h"
#include "LinkedList.h"


extern STMPOOL_HANDLE g_pNodePool;


CVoidList::CVoidList()
{
    m_nCount	= 0;
    m_bFreeData = 1;
    m_pNodeHead = m_pNodeTail = NULL;
}


CVoidList::~CVoidList()
{
    RemoveAll();
}


int CVoidList::GetCount() const
{
	return m_nCount;
}


int CVoidList::IsEmpty() const
{
	return m_nCount == 0;
}


void* CVoidList::GetHead() const
{
	if (m_pNodeHead == NULL)
		return NULL;
	
	return m_pNodeHead->data;
}


void* CVoidList::GetTail() const
{ 
	if (m_pNodeTail == NULL)
		return NULL;
	
	return m_pNodeTail->data;
}


void CVoidList::RemoveAll()
{
    CNode* pNode;
    CNode* pNext;

    pNode = m_pNodeHead;

    while(pNode != NULL)
    {
        pNext = pNode->pNext;
        FreeNode(pNode);
        pNode = pNext;
    }

    m_nCount = 0;
    m_pNodeHead = m_pNodeTail = NULL;
}


POSITION_ CVoidList::AddHead(void* newElement)
{
    CNode* pNewNode = NewNode(NULL, m_pNodeHead);

    pNewNode->data = newElement;

    if (m_pNodeHead != NULL)
        m_pNodeHead->pPrev = pNewNode;
    else
        m_pNodeTail = pNewNode;

    m_pNodeHead = pNewNode;
	
    return (POSITION_) pNewNode;
}


POSITION_ CVoidList::AddTail(void* newElement)
{
    CNode* pNewNode = NewNode(m_pNodeTail, NULL);

	if(pNewNode == NULL)
		return NULL;

    pNewNode->data = newElement;

    if (m_pNodeTail != NULL)
        m_pNodeTail->pNext = pNewNode;
    else
        m_pNodeHead = pNewNode;

    m_pNodeTail = pNewNode;

    return (POSITION_) pNewNode;
}


void CVoidList::AddHead(CVoidList* pNewList)
{
    if (pNewList == NULL) return;

    POSITION_ pos = pNewList->GetTailPosition();

    while (pos != NULL)
	{
        AddHead(pNewList->GetPrev(pos));
	}
}


void CVoidList::AddTail(CVoidList* pNewList)
{
    if (pNewList == NULL) return;

    POSITION_ pos = pNewList->GetHeadPosition();

    while (pos != NULL)
	{
        AddTail(pNewList->GetNext(pos));
	}
}


void* CVoidList::RemoveHead()
{
    if (m_pNodeHead == NULL) 
		return NULL;    

    CNode* pOldNode = m_pNodeHead;

    if (pOldNode == NULL) return NULL;
    void* returnValue = pOldNode->data;

    m_pNodeHead = pOldNode->pNext;

    if (m_pNodeHead != NULL)
        m_pNodeHead->pPrev = NULL;
    else
        m_pNodeTail = NULL;

    FreeNode(pOldNode);
    return returnValue;
}


void* CVoidList::RemoveTail()
{
    if (m_pNodeTail == NULL) return NULL;    

    CNode* pOldNode = m_pNodeTail;
    if (pOldNode == NULL) return NULL;
    void* returnValue = pOldNode->data;

    m_pNodeTail = pOldNode->pPrev;
    if (m_pNodeTail != NULL)
        m_pNodeTail->pNext = NULL;
    else
        m_pNodeHead = NULL;
    FreeNode(pOldNode);
    
	return returnValue;
}


POSITION_ CVoidList::InsertBefore(POSITION_ position, void* newElement)
{
    if (position == NULL)
        return AddHead(newElement); 

    CNode* pOldNode = (CNode*) position;
    CNode* pNewNode = NewNode(pOldNode->pPrev, pOldNode);

    pNewNode->data = newElement;

    if (pOldNode->pPrev != NULL)
    {
        pOldNode->pPrev->pNext = pNewNode;
    }
    else
    {
        if (pOldNode != m_pNodeHead) return NULL;
        m_pNodeHead = pNewNode;
    }

    pOldNode->pPrev = pNewNode;
    return (POSITION_) pNewNode;
}


POSITION_ CVoidList::InsertAfter(POSITION_ position, void* newElement)
{
    if (position == NULL)
        return AddTail(newElement); 

    CNode* pOldNode = (CNode*) position;
    CNode* pNewNode = NewNode(pOldNode, pOldNode->pNext);

    pNewNode->data = newElement;

    if (pOldNode->pNext != NULL)
    {
        pOldNode->pNext->pPrev = pNewNode;
    }
    else
    {
        if (pOldNode != m_pNodeTail) return NULL;
        m_pNodeTail = pNewNode;
    }

    pOldNode->pNext = pNewNode;
    return (POSITION_) pNewNode;
}


void CVoidList::RemoveAt(POSITION_ position)
{
    if (position == NULL) return;
	
	if(m_nCount == 0)
	{
		Log(LOG_IMPORTANT, "Current list position is bottom! but RemoveAt function called!");	
		return;
	}

    CNode* pOldNode = (CNode*) position;

//	if( NULL == pOldNode->pPrev )
//	{
//		--m_nCount;
//	}

    if (pOldNode == m_pNodeHead)
	{
		m_pNodeHead = pOldNode->pNext;
    }
	else
    {
		pOldNode->pPrev->pNext = pOldNode->pNext;
    }
	
	if (pOldNode == m_pNodeTail)
    {
		m_pNodeTail = pOldNode->pPrev; 
    }
	else
    {
		pOldNode->pNext->pPrev = pOldNode->pPrev;
	}

    FreeNode(pOldNode);
}


POSITION_ CVoidList::FindIndex(int nIndex) const
{
    if (nIndex < 0) return NULL;

    if (nIndex >= m_nCount)
        return NULL;        

    register CNode* pNode = m_pNodeHead;

    while (nIndex--)
        pNode = pNode->pNext;

    return (POSITION_) pNode;
}


POSITION_ CVoidList::Find(void* searchValue, POSITION_ startAfter) const
{
    register CNode* pNode = (CNode*) startAfter;

    if (pNode == NULL)
        pNode = m_pNodeHead;        
    else
        pNode = pNode->pNext;       

    for (; pNode != NULL; pNode = pNode->pNext)
		if (pNode->data == searchValue)
			return (POSITION_) pNode;
    return NULL;
}



CNode* CVoidList::NewNode(CNode* pPrev, CNode* pNext)
{ 
    CNode* pNode;

    pNode = (CNode*)LALAlloc( g_pNodePool );
	if (pNode == NULL) return NULL;

    pNode->pPrev = pPrev;
    pNode->pNext = pNext;
    pNode->data  = NULL;
    ++m_nCount;
	
    if (m_nCount <= 0)
    {
        FreeNode(pNode);
        return NULL; 
    }

    return pNode;
}


void CVoidList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;

    FreeData((void*)(pNode->data));

	LALFree( g_pNodePool, (void*)pNode );

    --m_nCount;
}


void CVoidList::FreeData(void* pData)
{
	if (pData == NULL)
		return;
}


void* CVoidList::SwapData(POSITION_& rPosition1,POSITION_& rPosition2)
{
    void* pData;
    CNode* pNode1, *pNode2;
    pNode1 = (CNode*) rPosition1;
    pNode2 = (CNode*) rPosition2;

    if (pNode1 == NULL || pNode2 == NULL)
		return NULL;

    pData = pNode1->data;
    pNode1->data = pNode2->data;
    pNode2->data = pData;

    return pData; 
}


void* CVoidList::GetNext(POSITION_& rPosition) const 
{ 
	CNode* pNode = (CNode*) rPosition;

    if (pNode == NULL) return NULL;

    rPosition = (POSITION_) pNode->pNext;

    return pNode->data;
}


void* CVoidList::GetPrev(POSITION_& rPosition) const 
{
	CNode* pNode = (CNode*) rPosition;

	if (pNode == NULL) return NULL;

	rPosition = (POSITION_) pNode->pPrev;

    return pNode->data;
}

void* CVoidList::GetAt(POSITION_ position) const
{ 
	CNode* pNode = (CNode*) position;

    if (pNode == NULL) return NULL;

    return pNode->data;
}


void CVoidList::SetAt(POSITION_ pos, void* newElement)
{
	CNode* pNode = (CNode*) pos;

    if (pNode == NULL) return;

    pNode->data = newElement;
}
