#pragma once

#include "../StdAfx.h"

#include <list>

#include <BaseLibrary/Utility/Macro/DebugMacros.h>		// #pragma todo() 를 사용하기 위해 정의
#include <BaseLibrary/Utility/Macro/CommonMacros.h>		// SAFE_DELETE() 등이 정의


template <typename Type> class CList
{
public:
    typedef std::list<Type>				TypeList;
	typedef typename TypeList::iterator	TypeListItr;


private:
	TypeList	m_List;
	LONG		m_lCount;


public:
	// constructor and destructor
	CList(void);
	~CList(void);


	// count methods
	LONG		GetCount(void)			{ return m_lCount;		}
	void		IncreaseCount(void)		{ ::InterlockedIncrement(&m_lCount); }
	void		DecreaseCount(void)		{ ::InterlockedDecrement(&m_lCount); }

	// interfaces
	void		Insert(Type Node);
	void		Remove(Type Node);
	Type		Find(Type Node);
	Type		PopHead(void);
	Type		Pop(Type Node);
	TypeListItr	Begin(void)				{ return m_List.begin();	}
	TypeListItr	End(void)				{ return m_List.end();		}
	TypeListItr	Erase(TypeListItr itr)	{ return m_List.erase(itr); }
	void		Clear(void)				{ m_List.clear();			}

	bool		IsEmpty(void)			{ return m_List.empty();	}

};

// constructor and destructor
template<typename Type> CList<Type>::CList(void) : m_lCount(0)
{
	m_List.clear();
}

template<typename Type> CList<Type>::~CList(void)
{
}

template<typename Type> void CList<Type>::Insert(Type Node)
{
	m_List.push_back(Node);
	IncreaseCount();
}

template<typename Type> void CList<Type>::Remove(Type Node)
{
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

template<typename Type> Type CList<Type>::Find(Type Node)
{
    for (TypeListItr itr = m_List.begin(); itr != m_List.end();) {
        if ((*itr) == Node) {
			return (*itr);
        } else {
            ++itr;
        }
	}

	return NULL;
}

template<typename Type> Type CList<Type>::PopHead(void)
{
	Type PrevHead	= NULL;
	if (false == m_List.empty()) {
		TypeListItr itr = m_List.begin();
		PrevHead	= (*itr);
		m_List.pop_front();
		DecreaseCount();
	}

	return PrevHead;
}

template<typename Type> Type CList<Type>::Pop(Type Node)
{
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
