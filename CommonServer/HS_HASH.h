#pragma once
#include <cassert>
#include <MAP>
using namespace std;

template< typename T_FIRST , typename T_SECOND>
class CHS_HASH
	: public map< T_FIRST , T_SECOND >
{
public:
	CHS_HASH(){}
	virtual ~CHS_HASH(){}

public:
	virtual bool Insert(const T_FIRST &First, const T_SECOND &Second)
	{
		T_SECOND *pSecond = Find( First );

		if(!pSecond)
		{
			insert( make_pair( First, Second ) );
			pSecond = Find( First );
			if(pSecond)
			{
				return true;
			} // if(pSecond)
			else
			{
				assert(NULL && "CHS_HASH Insert Fail");
				return false;
			}
		}
		else
		{
			assert(NULL && "CHS_HASH Element Already Insert");
			return false;
		}
	}

	virtual bool Remove(const T_FIRST First)
	{
		iterator Itor = find(First);

		if( end() != Itor )
		{
			erase( Itor );
			return true;
		}

		return false;
	}
		
	virtual T_SECOND* Find(const T_FIRST &First)
	{
		iterator Itor = find(First);

		if( end() != Itor )
		{
			return &((*Itor).second);
		}
		return NULL;
	}
};
