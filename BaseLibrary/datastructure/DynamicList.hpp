#pragma once

#include <BaseLibrary/Thread/Lock.hpp>
#include <BaseLibrary/Utility/Macro/DebugMacros.h>		// #pragma todo() 를 사용하기 위해 정의
#include <BaseLibrary/Utility/Macro/CommonMacros.h>		// SAFE_DELETE() 등이 정의


typedef class CDynamicNode*	LPCDynamicNode;
class CDynamicNode
{
private:
	LPCDynamicNode		m_pPrev;
	LPCDynamicNode		m_pNext;

public:
	// constructor and destructor
	CDynamicNode(void) : m_pPrev(NULL), m_pNext(NULL)			{}
	virtual ~CDynamicNode(void)									{ m_pPrev	= NULL; m_pNext	= NULL; }

	// list 관리를 위한 methods
	inline LPCDynamicNode	GetPrev(void)						{ return m_pPrev;					}
	inline void				SetPrev(LPCDynamicNode pPrev)		{ if( NULL != m_pPrev ) m_pPrev	= pPrev;				}
	inline LPCDynamicNode	GetNext(void)						{ return m_pNext;					}
	inline void				SetNext(LPCDynamicNode pNext)		{ if( NULL != m_pNext ) m_pNext	= pNext;				}
};


// double linked list
// TypePtr 은 반드시 CDynamicNode 의 derived class 여야만 한다.
template <typename TypePtr> class CDynamicList
{
private:
	CBCSLock		m_Lock;

	TypePtr			m_pHead;
	LONG			m_lCount;

	inline void		SetHead(TypePtr Node);


public:
	// constructor and destructor
	CDynamicList(void);
	~CDynamicList(void);


	// count methods
	inline LONG		GetCount(void);
	inline void		IncreaseCount(void);
	inline void		DecreaseCount(void);

	// interfaces
	void			Terminate(void);
	void			DynamicInsert(TypePtr Node);
	void			DynamicRemove(TypePtr Node);
	TypePtr			DynamicFind(TypePtr Node);
	TypePtr			DynamicPopHead(void);

	inline TypePtr	GetHead(void);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
template<typename TypePtr> CDynamicList<TypePtr>::CDynamicList(void) : m_pHead(NULL), m_lCount(0)
{
}

template<typename TypePtr> CDynamicList<TypePtr>::~CDynamicList(void)
{
	Terminate();
}

template<typename TypePtr> TypePtr CDynamicList<TypePtr>::GetHead(void)
{
	return m_pHead;
}

template<typename TypePtr> void CDynamicList<TypePtr>::SetHead(TypePtr Node)
{
	m_pHead	= Node;
}

// pending number access methods
template<typename TypePtr> LONG CDynamicList<TypePtr>::GetCount(void)
{
	return m_lCount;
}

template<typename TypePtr> void CDynamicList<TypePtr>::IncreaseCount(void)
{
	::InterlockedIncrement(&m_lCount);
}

template<typename TypePtr> void CDynamicList<TypePtr>::DecreaseCount(void)
{
	::InterlockedDecrement(&m_lCount);
}


template<typename TypePtr> void CDynamicList<TypePtr>::Terminate(void)
{
	int	nNumberOfNode	= 0;
	while ( TypePtr pHead = DynamicPopHead() ) {
		INFLOG2(g_Log, "socket object list에서 socket object(0x%08x<0x%08x>)를 팝하고 릴리즈...", pHead, pHead->GetSocket());
		pHead->Release();

		++nNumberOfNode;
	}

	if ( nNumberOfNode )
		INFLOG1(g_Log, "CDynamicList - 연결된 리스트 %d 개를 종료", nNumberOfNode);
}

template<typename TypePtr> void CDynamicList<TypePtr>::DynamicInsert(TypePtr Node)
{
	// insert 할 node 를 맨 앞에 추가한다.
	CBCSLock::Syncronize sync(m_Lock);

	Node->SetPrev(NULL);

	if ( GetHead() )
		GetHead()->SetPrev(Node);

	Node->SetNext(GetHead());
	SetHead(Node);

	IncreaseCount();
}

template<typename TypePtr> void CDynamicList<TypePtr>::DynamicRemove(TypePtr Node)
{
	CBCSLock::Syncronize sync(m_Lock);

	//if ( GetHead() == Node )
	//	SetHead( (TypePtr)Node->GetNext() );
	//else
	if( Node->GetPrev() )
		Node->GetPrev()->SetNext(Node->GetNext());
	else
		SetHead( (TypePtr)Node->GetNext() );

	if ( Node->GetNext() )
		Node->GetNext()->SetPrev(Node->GetPrev());

	// node 의 연결을 끊는다.
	Node->SetPrev(NULL);
	Node->SetNext(NULL);

	DecreaseCount();
}

template<typename TypePtr> TypePtr CDynamicList<TypePtr>::DynamicFind(TypePtr Node)
{
	CBCSLock::Syncronize sync(m_Lock);

	TypePtr Curr = GetHead();
	while ( Curr ) {
		if ( Curr == Node )
			return Curr;

		Curr	= (TypePtr)Curr->GetNext();
	}

	return NULL;
}

template<typename TypePtr> TypePtr CDynamicList<TypePtr>::DynamicPopHead(void)
{
	CBCSLock::Syncronize sync(m_Lock);

	TypePtr pPrevHead	= GetHead();
	if ( NULL == pPrevHead )
		return NULL;

	TypePtr pNextHead	= (TypePtr)GetHead()->GetNext();
	SetHead(pNextHead);

	if ( pNextHead ) {
		pNextHead->SetPrev(NULL);
		pPrevHead->SetNext(NULL);
	}

	DecreaseCount();

	return pPrevHead;
}
