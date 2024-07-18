#pragma once

#include "../StdAfx.h"

#include <map>


template <typename Key, typename Type> class CMap
{
public:
    typedef std::map<Key, Type>				TypeMap;
	typedef typename TypeMap::iterator		TypeMapItr;
	typedef std::pair< TypeMapItr, bool >	TypeMapPair;


private:
	TypeMap		m_Map;
	LONG		m_lCount;


public:
	// constructor and destructor
	CMap(void);
	~CMap(void);


	// pending number access methods
	LONG		GetCount(void)			{ return m_lCount;						}
	void		IncreaseCount(void)		{ ::InterlockedIncrement(&m_lCount);	}
	void		DecreaseCount(void)		{ ::InterlockedDecrement(&m_lCount);	}

	// methods
	bool		Insert(Key KeyValue, Type Node);
	bool		Remove(Key KeyValue);
	Type		Find(Key KeyValue);
	Type		PopHead(void);
	Type		Pop(Key KeyValue);
	TypeMapItr	Begin(void)				{ return m_Map.begin();					}
	TypeMapItr	End(void)				{ return m_Map.end();					}
	TypeMapItr	Erase(TypeMapItr itr)	{ return m_Map.erase(itr);				}
	void		Clear(void)				{ m_Map.clear();						}

	bool		IsEmpty(void)			{ return m_Map.empty();					}

};

// constructor and destructor
template<typename Key, typename Type> CMap<Key, Type>::CMap(void) : m_lCount(0)
{
	m_Map.clear();
}

template<typename Key, typename Type> CMap<Key, Type>::~CMap(void)
{
	for ( TypeMapItr Itr = m_Map.begin(); Itr != m_Map.end(); ) {
		Itr = m_Map.erase(Itr);
	}
}

template<typename Key, typename Type> bool CMap<Key, Type>::Insert(Key KeyValue, Type Node)
{
	TypeMapPair TypePair	= m_Map.insert( TypeMap::value_type(KeyValue, Node) );
	if ( false == TypePair.second )
		return false;

	IncreaseCount();

	return true;
}

template<typename Key, typename Type> bool CMap<Key, Type>::Remove(Key KeyValue)
{
	TypeMapItr Itr;
	if ( m_Map.end() == ( Itr = m_Map.find(KeyValue) ) )
		return false;

	Type Node = Itr->second;
	m_Map.erase(Itr);

	DecreaseCount();

	return true;
}

template<typename Key, typename Type> Type CMap<Key, Type>::Find(Key KeyValue)
{
	TypeMapItr Itr;
	if ( m_Map.end() == ( Itr = m_Map.find(KeyValue) ) )
		return NULL;
	else
		return Itr->second;
}

template<typename Key, typename Type> Type CMap<Key, Type>::PopHead(void)
{
	TypeMapItr Itr;
	if ( m_Map.end() != ( Itr = m_Map.begin() ) ) {
		Type Node	= Itr->second;
		Itr = m_Map.erase(Itr);
		DecreaseCount();
		return Node;
	}

	return NULL;
}

template<typename Key, typename Type> Type CMap<Key, Type>::Pop(Key KeyValue)
{
	TypeMapItr Itr;
	if ( m_Map.end() != ( Itr = m_Map.find(KeyValue) ) ) {
		Type Node	= Itr->second;
		Itr = m_Map.erase(Itr);
		DecreaseCount();
		return Node;
	}

	return NULL;
}
