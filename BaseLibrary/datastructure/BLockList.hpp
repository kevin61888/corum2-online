#pragma once

#include "../StdAfx.h"

#include <list>

#include <BaseLibrary/Thread/Lock.hpp>
#include <BaseLibrary/Utility/Macro/DebugMacros.h>		// #pragma todo() 를 사용하기 위해 정의
#include <BaseLibrary/Utility/Macro/CommonMacros.h>		// SAFE_DELETE() 등이 정의


template <typename Type> class CBLockList
{
public:
    typedef std::list<Type>				TypeList;
	typedef typename TypeList::iterator	TypeListItr;
	typedef typename TypeList::reverse_iterator	TypeListRItr;


private:
	CBCSLock	m_Lock;
	TypeList	m_List;
	LONG		m_lCount;


public:
	// constructor and destructor
	CBLockList(void);
	~CBLockList(void);


	// count methods
	LONG		GetCount(void)			{ return m_lCount; }
	void		IncreaseCount(void)		{ ::InterlockedIncrement(&m_lCount); }
	void		DecreaseCount(void)		{ ::InterlockedDecrement(&m_lCount); }

	// interfaces
	void		Insert(Type Node);
	void		Remove(Type Node);
	Type		Find(Type Node);
	Type		PopHead(void);
	Type		Pop(Type Node);
	Type		GetTail(void);
	Type		GetHead(void);
	TypeListItr	Begin(void)				{ CBCSLock::Syncronize sync(m_Lock); return m_List.begin();		}
	TypeListItr	End(void)				{ CBCSLock::Syncronize sync(m_Lock); return m_List.end();		}
	TypeListItr	Erase(TypeListItr itr)	{ CBCSLock::Syncronize sync(m_Lock); return m_List.erase(itr);	}
	void		Clear(void)				{ CBCSLock::Syncronize sync(m_Lock); m_List.clear();			}

	bool		IsEmpty(void)			{ CBCSLock::Syncronize sync(m_Lock); return m_List.empty();		}

};

// constructor and destructor
template<typename Type> CBLockList<Type>::CBLockList(void) : m_lCount(0)
{
	m_List.clear();
}

template<typename Type> CBLockList<Type>::~CBLockList(void)
{
}

template<typename Type> void CBLockList<Type>::Insert(Type Node)
{
	// push back
	CBCSLock::Syncronize sync(m_Lock);

	m_List.push_back(Node);
	IncreaseCount();
}

template<typename Type> void CBLockList<Type>::Remove(Type Node)
{
	CBCSLock::Syncronize sync(m_Lock);

    for (TypeListItr itr = m_List.begin(); itr != m_List.end();) {
        if ((*itr) == Node) {
			itr = m_List.erase(itr);
			DecreaseCount();
			return;
        } else {
            ++itr;
        }
	}
}

template<typename Type> Type CBLockList<Type>::Find(Type Node)
{
	CBCSLock::Syncronize sync(m_Lock);

    for (TypeListItr itr = m_List.begin(); itr != m_List.end();) {
        if ((*itr) == Node) {
			return (*itr);
        } else {
            ++itr;
        }
	}

	return NULL;
}

template<typename Type> Type CBLockList<Type>::PopHead(void)
{
	Type PrevHead	= NULL;

	CBCSLock::Syncronize sync(m_Lock);

	if (false == m_List.empty()) {
		TypeListItr itr = m_List.begin();
		PrevHead	= (*itr);
		m_List.pop_front();
		DecreaseCount();
	}

	return PrevHead;
}

template<typename Type> Type CBLockList<Type>::Pop(Type Node)
{
	CBCSLock::Syncronize sync(m_Lock);

	if (false == m_List.empty()) {
		for (TypeListItr itr = m_List.begin(); itr != m_List.end();) {
			if ((*itr) == Node) {
				itr = m_List.erase(itr);
				DecreaseCount();
				return Node;
			} else {
				++itr;
			}
		}
	}

	return Node;
}

template<typename Type> Type CBLockList<Type>::GetTail( void )
{
	CBCSLock::Syncronize sync(m_Lock);

	if ( true == m_List.empty() )
		return NULL;
	TypeListRItr itr = m_List.rbegin();
	return (*itr);
}

template<typename Type> Type CBLockList<Type>::GetHead( void )
{
	CBCSLock::Syncronize sync(m_Lock);

	if ( true == m_List.empty() )
		return NULL;
	TypeListItr itr = m_List.begin();
	return (*itr);
}
