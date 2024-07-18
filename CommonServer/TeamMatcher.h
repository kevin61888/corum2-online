#pragma once
#include "../Commonserver/HS_HASH.h"
#include "MatcherResult.h"

template< typename T_CONDITION, typename T_MATCH_HANDLER = CMatchHandler<> >
class CTeamMatcher
{
public:
	CTeamMatcher(void){}
	virtual ~CTeamMatcher(void){}
	typedef typename T_MATCH_HANDLER	T_HANDLER;


	typedef typename T_MATCH_HANDLER::T_PLAYER T_PLAYER;

	typedef T_MATCH_HANDLER* T_HANDLER_FOR_HASH;
	typedef CHS_HASH< T_CONDITION , T_HANDLER_FOR_HASH > HANDLER_HASH;

	typedef typename HANDLER_HASH::iterator HANDLER_HASH_ITOR;

	typedef struct tagReadyElement
	{
		tagReadyElement(const T_CONDITION &rCond, T_MATCH_HANDLER *pHandler)
		{
			m_Cond = rCond;
			m_pHandler = pHandler;
		}

		T_CONDITION		m_Cond;
		T_MATCH_HANDLER *m_pHandler;
	}READY_ELEMENT;

	typedef vector< READY_ELEMENT > HANDLER_ARRAY;

public:

	bool Insert(const T_CONDITION &rCond, const T_PLAYER &rPlayer)
	{	//return은 ** 이고
		//받을때 **로 받으면 되는데.
		T_HANDLER_FOR_HASH	*pHandler = m_hash.Find( rCond );
		if( pHandler )
		{
			return common_insert( *pHandler, rPlayer);
		} // if( *pHandler )
		else
		{
			T_MATCH_HANDLER *pNewHandler = MakeHandler();
			if(pNewHandler)
			{
				if( pNewHandler->Open() )
				{
					const bool bRet = m_hash.Insert(rCond, pNewHandler);
					
					if(bRet)
					{
						return  common_insert( pNewHandler, rPlayer);
					}
					else
					{
						assert( NULL  && "Insert Handler Failed");
					}
				} // if( pNewHandler->Open() )
				else
				{
					DestroyHandler( pNewHandler );
					return false;
				}
			} // if(pNewHandler)
			else
			{
				assert( NULL  && "Can't Create Handler pNewHandler");
				return false;
			}
		}
		return false;
	}
	
	bool Remove(const T_CONDITION &rCond, const T_PLAYER &rPlayer)
	{
		T_HANDLER_FOR_HASH *pHandler = m_hash.Find(rCond);

		if( pHandler)
		{
			if( (*pHandler)->handle_remove() )
			{
				const bool bRet = (*pHandler)->List().Erase(rPlayer);
				if(bRet)
				{
					(*pHandler)->OnRemove();
				}
				return bRet;
			} // if( (*pHandler)->OnRemove() )

			return false;
		} // if( pHandler)
		else
		{
//			assert( NULL  && "CMatch::Remove() There is No Handler");
		}
		return false;
	}
	
	T_PLAYER* Find(const T_CONDITION &rCond, const T_PLAYER &rPlayer)
	{
		T_MATCH_HANDLER *pHandler = m_hash.Find(rCond);

		if( pHandler)
		{
			T_PLAYER *pPlayer = pHandler->Find(rPlayer);

			if(NULL != pPlayer)
			{
				return pPlayer;
			}			
		}
		return NULL;
	}

	bool Modify(const T_CONDITION &rCond, const T_PLAYER &rPlayer)
	{
		return true;
	}

	virtual bool HandleEvent() = 0;
	
protected:
	virtual T_MATCH_HANDLER* MakeHandler()
	{
		T_MATCH_HANDLER *pHandler = new T_MATCH_HANDLER;
		return pHandler;
	}

	virtual void DestroyHandler(T_MATCH_HANDLER *&prHandler)
	{
		prHandler->OnDestroy();
		SAFE_DELETE(prHandler);
	}

	bool common_insert(T_MATCH_HANDLER *pHandler, const T_PLAYER &rPlayer)
	{
		if( pHandler->handle_insert() )
		{
			if( !pHandler->List().Find( rPlayer ) )//찾아보고 없으면 넣어라
			{

				pHandler->List().PushBack( rPlayer );
				pHandler->OnInsert();
				return true;
			} // if( !pHandler->List().Find( rPlayer ) )//찾아보고 없으면 넣어라
			else
			{
				cout << "!!! Aleady Inserted Element"<<endl;
				return false;
			}
		} // if( pHandler->handle_insert() )
		return false;
	}

protected:
	HANDLER_HASH m_hash;
};
