#pragma once


#include "stdafx.h"
#include "../4DyuchiGXGFunc/global.h"


struct CNode
{
    CNode*  pNext;
    CNode*  pPrev;
    void*    data;
};


typedef void* POSITION_;


class CVoidList
{
public:

    CVoidList();

	int				GetCount()	const;
    int				IsEmpty()	const;
	void*			GetHead()	const; 
    void*			GetTail()	const;
    void*			RemoveHead();
    void*			RemoveTail();
    POSITION_		AddHead(void* newElement);
    POSITION_		AddTail(void* newElement);
    void			AddHead(CVoidList* pNewList);
    void			AddTail(CVoidList* pNewList);
    void			RemoveAll();
	POSITION_		GetHeadPosition() const	{ return (POSITION_) m_pNodeHead; }
    POSITION_		GetTailPosition() const { return (POSITION_) m_pNodeTail; }
    void*			SwapData(POSITION_& rPosition1,POSITION_& rPosition2);
    void*			GetNext(POSITION_& rPosition) const; 
    void*			GetPrev(POSITION_& rPosition) const; 
    void*			GetAt(POSITION_ position) const;
	void			SetAt(POSITION_ pos, void* newElement);                
    void			RemoveAt(POSITION_ position);
    POSITION_		InsertBefore(POSITION_ position, void* newElement);
    POSITION_		InsertAfter(POSITION_ position, void* newElement);
    POSITION_		Find(void* searchValue, POSITION_ startAfter = NULL) const;
    POSITION_		FindIndex(int nIndex) const;

	virtual void	FreeNode(CNode*);

protected:

    CNode*			m_pNodeHead;
    CNode*			m_pNodeTail;
    int				m_nCount;

	CNode*			NewNode(CNode*, CNode*);

protected:

	virtual void    FreeData(void* pData);

public:

    BOOL	m_bFreeData;
    ~CVoidList();
};