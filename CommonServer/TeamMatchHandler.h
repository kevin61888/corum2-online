#pragma once
#include "../Commonserver/HS_LIST.h"
#include "MatcherResult.h"

template < typename T_ELEMENT >
class CTeamMatchHandler
{
public:
	typedef T_ELEMENT	T_PLAYER;
	typedef CHS_LIST< T_ELEMENT > ELEMENT_LIST;
public:
	CTeamMatchHandler(void){}
	virtual ~CTeamMatchHandler(void){}

	virtual bool Open() = 0;
	virtual void OnDestroy() = 0;

	virtual bool handle_insert() = 0;
	virtual bool handle_remove() = 0;

	virtual void OnInsert() = 0;
	virtual void OnRemove() = 0;//지우기전에 들어옴 return false; 일경우 지워지지 않음
	
	virtual bool IsReady( size_t minplayerCount ) CONST = 0;
	virtual bool OnMatchNotify(const CMatcherResult::NotifyResult& rNR ) = 0;

//	virtual bool OnMatchNotify(const T_PLAYER& rPlayer) = 0;

	virtual T_ELEMENT* Find(const T_ELEMENT& rElement)
	{
		T_ELEMENT* pElement = m_lstElement.Find(rElement);
		return pElement;
	}
	
	ELEMENT_LIST& List() {return m_lstElement;}

protected:
	ELEMENT_LIST	m_lstElement;
};