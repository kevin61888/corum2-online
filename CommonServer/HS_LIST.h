#pragma once

#include <list>
#include <string>
using namespace std;

template < typename T_ELEMENT , typename T_INFO = void*>
class CHS_LIST
	: public list< T_ELEMENT >
{
public:
	CHS_LIST(void){}
	virtual ~CHS_LIST(void){}

public:
	T_ELEMENT* Find(const T_ELEMENT& T_Element)
	{
		iterator Itor = begin();

		while( end() != Itor )
		{
			if( (*Itor) == T_Element )
			{
				return &(*Itor);
			}
			++Itor;
		}
		return NULL;
	}

	void PushBack( const T_ELEMENT& T_Element )
	{
		push_back(T_Element);
	}

	bool Erase( const T_ELEMENT& T_Element )
	{
		iterator Itor = begin();
		
		while( end() != Itor )
		{
			if( (*Itor) == T_Element )
			{
				this->erase(Itor);
				return true;
			}
			++Itor;
		}
		return false;
	}

public:
	T_INFO& Info(){return m_Info;}
	void Info(const T_INFO &T_rInfo){m_Info = T_rInfo;}

private:
	T_INFO m_Info;
};
