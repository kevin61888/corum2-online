//====================================================================================================//
//
// $Header: /cvsroot/DEVELOP_V1/CorumOnlineProject/crtList.h,v 1.2 2005/02/04 02:41:49 hwoarang Exp $
//
//====================================================================================================//

//====================================================================================================//
#pragma once
//====================================================================================================//

//====================================================================================================//
/**
* \ingroup XEngineCore
*
* \version 0.1
*
* \date 2004-07-08
*
* \author iihito
* 
* \par Remarks :
* 
* \todo 
*
* \bug 
*
*/
template< typename T >
class crtList
{
public:
	//======================================================================//
	T*	m_pMe;
	T*	m_pPrev;
	T*	m_pNext;
	//======================================================================//

public:
	//======================================================================//
	/**
	*
	* \return 
	*/
	T*	GetPrev() const;
	//======================================================================//

	//======================================================================//
	/**
	*
	* \return 
	*/
	T*	GetNext() const;
	//======================================================================//

	//======================================================================//
	/**
	*
	* \return 
	*/
	bool	IsBegin() const;
	//======================================================================//

	//======================================================================//
	/**
	*
	* \return 
	*/
	bool	IsEnd() const;
	//======================================================================//

	//======================================================================//
	/**
	*
	* \return 
	*/
	T*	GetBegin();
	//======================================================================//

	//======================================================================//
	/**
	*
	* \return 
	*/
	T*	GetEnd();
	//======================================================================//

	//======================================================================//
	/**
	*
	* \param ptr 
	* \return 
	*/
	T*	PushFront( T* const ptr );
	//======================================================================//

	//======================================================================//
	/**
	*
	* \param ptr 
	* \return 
	*/
	T*	PushBack( T* const ptr );
	//======================================================================//

	//======================================================================//
	/**
	*
	*/
	void	PopFront();	
	//======================================================================//

	//======================================================================//
	/**
	*
	*/
	void	PopBack();
	//======================================================================//

	//======================================================================//
	/**
	*
	*/
	void	Pop();
	//======================================================================//

	//======================================================================//
	/**
	*
	*/
	void	SwapFront();
	//======================================================================//

	//======================================================================//
	/**
	*
	*/
	void	SwapBack();
	//======================================================================//

	//======================================================================//
	/**
	*
	* \return 
	*/
	bool	NullCheck_SwapFront();
	//======================================================================//

	//======================================================================//
	/**
	*
	* \return 
	*/
	bool	NullCheck_SwapBack();
	//======================================================================//

	//======================================================================//
	/**
	*
	* \return 
	*/
	crtList();
	//======================================================================//

	//======================================================================//
	/**
	*
	* \param pT 
	*/
	static	void	BubbleSort( T* pT );	
	//======================================================================//

	//======================================================================//
	/**
	*
	* \param pT 
	*/
	static	void	QuickSort( T* pT );
	//======================================================================//

	//======================================================================//
	/**
	*
	* \param pBegin 
	* \param pEnd 
	* \param pT 
	* \return 
	*/
	static	bool	Find( T* pBegin, T* pEnd, T* pT );
	//======================================================================//

	//======================================================================//
	/**
	*
	* \param pBegin 
	* \param pT 
	* \return 
	*/
	static	bool	Find( T* pBegin, T* pT );
	//======================================================================//
};
//====================================================================================================//

//======================================================================//
/**
*
* \param pT 
* \param Compare 
*/
template<typename T, typename __Compare>	void	BubbleSort( T* pT, __Compare	Compare );
//======================================================================//

//====================================================================================================//
//////////////////////////////////////////////////////////////////////
//
// $Header: /cvsroot/DEVELOP_V1/CorumOnlineProject/crtList.h,v 1.2 2005/02/04 02:41:49 hwoarang Exp $
//
// crtList.h: interface for the crtList class.
//
//////////////////////////////////////////////////////////////////////

#pragma warning( disable :4311 )
#pragma warning( disable :4312 )

//////////////////////////////////////////////////////////////////////
template<typename T> T*	crtList<T>::GetPrev(void) const
{
	return m_pPrev;
}

template<typename T> T*	crtList<T>::GetNext(void) const
{
	return m_pNext;
}

template<typename T> bool	crtList<T>::IsBegin(void) const
{
	return m_pPrev == NULL;
}

template<typename T> bool	crtList<T>::IsEnd(void) const
{
	return m_pNext == NULL;
}

template<typename T> T*	crtList<T>::GetBegin(void)
{
	T*	tmp = m_pPrev;

	while( tmp != NULL )
		tmp = tmp->m_pPrev;

	return tmp;
}

template<typename T> T*	crtList<T>::GetEnd(void)
{
	T*	tmp = m_pNext;

	while( tmp != NULL )
		tmp = tmp->m_pNext;

	return tmp;
}

template<typename T> T*	crtList<T>::PushFront( T* const ptr )
{
	if( m_pPrev )
	{
		m_pPrev->m_pNext = ptr;
		ptr->m_pPrev = m_pPrev;
	}

	m_pPrev = ptr;
	ptr->m_pNext = m_pMe;

	return m_pPrev->m_pPrev;
}

template<typename T> void	crtList<T>::PopFront(void)
{
	if( m_pPrev )
	{
		m_pPrev->m_pNext = m_pNext;
		m_pPrev = NULL;
	}
}

template<typename T> T*	crtList<T>::PushBack( T* const ptr )
{
	if( m_pNext )
	{
		m_pNext->m_pPrev = ptr;
		ptr->m_pNext = m_pNext;
	}

	m_pNext = ptr;
	ptr->m_pPrev = m_pMe;

	return m_pNext->m_pNext;
}

template<typename T> void	crtList<T>::PopBack(void)
{
	if( m_pNext )
	{
		m_pNext->m_pPrev = m_pPrev;
		m_pNext = NULL;
	}
}

template<typename T> void	crtList<T>::Pop(void)
{
	if( m_pPrev )
	{
		m_pPrev->m_pNext = m_pNext;			
	}

	if( m_pNext )
	{
		m_pNext->m_pPrev = m_pPrev;		
	}

	m_pPrev = NULL;
	m_pNext = NULL;
}

template<typename T> void	crtList<T>::SwapFront(void)
{
	T* pT = m_pPrev;

	PopFront();
	PushBack( pT );
}

template<typename T> void	crtList<T>::SwapBack(void)
{
	T* pT = m_pNext;

	PopBack();
	PushFront( pT );
}

template<typename T> bool	crtList<T>::NullCheck_SwapFront(void)
{
	if( IS_NULL(m_pPrev) )
		return false;

	T* pT = m_pPrev;

	PopFront();
	PushBack( pT );
}

template<typename T> bool	crtList<T>::NullCheck_SwapBack(void)
{
	if( IS_NULL(m_pNext) )
		return false;

	T* pT = m_pNext;

	PopBack();
	PushFront( pT );
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
template< typename T > void	crtList<T>::BubbleSort( T* pT )
{
	T*	pCurrent = pT;
	T*	pNext = pCurrent->GetNext();

	while( pCurrent && pNext )
	{
		if( (*pCurrent < *pNext) == false )
		{
			pCurrent->SwapBack();

			// 윗쪽에 노드가 있다면 전파한다.
			if( pNext->GetPrev() )
			{
				pCurrent = pNext->GetPrev();
				pNext = pCurrent->GetNext();
			}
			else
				pNext = pCurrent->GetNext();			
		}
		else
		{
			pCurrent = pNext;
			pNext = pCurrent->GetNext();
		}
	}
}

template<typename T, typename __Compare>	void	BubbleSort( T* pT, __Compare	Compare )
{
	T*	pCurrent = pT;
	T*	pNext = pCurrent->GetNext();

	while( pCurrent && pNext )
	{
		if( Compare( pCurrent, pNext ) == false )
		{
			pCurrent->SwapBack();

			// 윗쪽에 노드가 있다면 전파한다.
			if( pNext->GetPrev() )
			{
				pCurrent = pNext->GetPrev();
				pNext = pCurrent->GetNext();
			}
			else
				pNext = pCurrent->GetNext();			
		}
		else
		{
			pCurrent = pNext;
			pNext = pCurrent->GetNext();
		}
	}
}

template<typename T> void	crtList<T>::QuickSort( T* pT )
{

}

template<typename T>	bool	crtList<T>::Find( T* pBegin, T* pEnd, T* pT )
{
	for( T* pIter = pBegin ; pIter != pEnd && pIter != NULL ; pIter = pIter->GetNext() )
	{
		if( pIter == pT )
			return true;
	}

	return false;
}

template<typename T>	bool	crtList<T>::Find( T* pBegin, T* pT )
{
	for( T* pIter = pBegin ; pIter != NULL ; pIter = pIter->GetNext() )
	{
		if( pIter == pT )
			return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
template<typename T>	crtList<T>::crtList(void) : m_pPrev(NULL), m_pNext(NULL)
{
	INT	iOffset = (INT)(T*)1 - (INT)(crtList<T>*)(T*)1;
	m_pMe = (T*)((INT)this + iOffset);
}

//////////////////////////////////////////////////////////////////////
//====================================================================================================//