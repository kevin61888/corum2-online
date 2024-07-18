#pragma once


#include "Windows.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGXGFunc/global.h"
#else
#include "./../4DyuchiGXGFunc/global.h"
#endif


template<class TT>
struct ListNode
{
	TT*			  pData;
	ListNode<TT>* pPrev;
	ListNode<TT>* pNext;
};

template<class TData>
struct Node 
{
	DWORD			 dwKey;
	TData*			 pData;
	ListNode<TData>* pListPos;	
	Node<TData>*	 pPrev;
	Node<TData>*	 pNext;
};


template<class Td>
class CAbyssHash
{
protected:
	DWORD			m_dwMaxBucketNum;
	DWORD			m_dwCount;
	Node<Td>**		m_pBucket;
	ListNode<Td>*	m_pHead;			
	ListNode<Td>*	m_pTail;			
	STMPOOL_HANDLE	m_pNodePool;		
	STMPOOL_HANDLE	m_pListNodePool;	
	BOOL			m_bSameKeyCheck;	
public:
	DWORD		GetMaxBucketNum()				{ return m_dwMaxBucketNum; }
	DWORD		GetCount()						{ return m_dwCount; }			
	DWORD		GetBuckeCount(DWORD dwKey);
	Node<Td>*	GetBucketHead( DWORD dwKey )		{ return m_pBucket[dwKey%m_dwMaxBucketNum]; }
	
	ListNode<Td>*	GetHead()						{ return m_pHead; }
	ListNode<Td>*	GetTail()						{ return m_pTail; }

public:

	CAbyssHash();		
	virtual ~CAbyssHash();

	void	InitializeHashTable( DWORD dwMaxBucket, DWORD dwMaxNode, BOOL bSameKeyCheck = TRUE );
	
	void	Destroy( BOOL bDeleteData = FALSE );
	void	Delete( Td* pdata, DWORD dwKey, BOOL bDeleteData = FALSE );	

	ListNode<Td>*	AddList(Td* pData);								
	void			RemoveAtList( ListNode<Td>* pListNode );	
	
	virtual Td*		GetData( DWORD dwKey );

	BOOL	bSameKey( DWORD dwKey )	;			
	DWORD	Insert( Td* pdata, DWORD dwKey );	

	virtual	void DeleteData( void* pData );		
};

template<class Td>
CAbyssHash<Td>::CAbyssHash()
{
	m_dwMaxBucketNum	= 0;
	m_dwCount			= 0;
	m_pBucket			= NULL;
	m_pHead				= NULL;
	m_pTail				= NULL;
	m_pNodePool			= NULL;	
	m_pListNodePool		= NULL;
}

template<class Td>
CAbyssHash<Td>::~CAbyssHash()
{
	if( m_pBucket )
	{
		delete[] m_pBucket;
		m_pBucket = NULL;
	}

	if( m_pNodePool )
	{
		ReleaseStaticMemoryPool(m_pNodePool);
		m_pNodePool = NULL;
	}

	if( m_pListNodePool )
	{
		ReleaseStaticMemoryPool(m_pListNodePool);
		m_pListNodePool = NULL;
	}
}

template<class Td>
void CAbyssHash<Td>::InitializeHashTable( DWORD dwMaxBucket, DWORD dwMaxNode, BOOL bSameKeyCheck )
{
	m_dwMaxBucketNum = dwMaxBucket;
	m_bSameKeyCheck=bSameKeyCheck;
	m_pBucket = new Node<Td>*[m_dwMaxBucketNum];
	memset( m_pBucket, 0, sizeof(Td*) * m_dwMaxBucketNum );

	m_pNodePool		= CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pNodePool, sizeof( Node<Td> ), dwMaxNode, dwMaxNode);

	m_pListNodePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pListNodePool, sizeof( ListNode<Td> ), dwMaxNode, dwMaxNode);
}

template<class Td>
void CAbyssHash<Td>::DeleteData( void* pData )
{
	if( pData )
		delete (Td*)pData;
}

template<class Td>
ListNode<Td>*	CAbyssHash<Td>::AddList(Td* pData)
{
	ListNode<Td>* pListNode = (ListNode<Td>*)LALAlloc(m_pListNodePool);
	pListNode->pData	= pData;

	if( !m_pHead )
	{
		m_pTail			= pListNode;
		m_pTail->pPrev	= NULL;
		m_pTail->pNext	= NULL;
		m_pHead = m_pTail;
	}
	else
	{
		m_pTail->pNext		= pListNode;
		pListNode->pPrev	= m_pTail;
		pListNode->pNext	= NULL;
		m_pTail				= pListNode;
	}

	return pListNode;
}

template<class Td>
void CAbyssHash<Td>::RemoveAtList( ListNode<Td>* pListNode )
{
	if(m_pHead == pListNode)
		m_pHead = pListNode->pNext;
	else
		pListNode->pPrev->pNext = pListNode->pNext;

	if(m_pTail == pListNode)
		m_pTail = pListNode->pPrev;
	else
		pListNode->pNext->pPrev = pListNode->pPrev;


	LALFree( m_pListNodePool, (void*)pListNode);
}


template<class Td>
void	CAbyssHash<Td>::Destroy( BOOL bDeleteData )
{
	Node<Td>*	pDel;
	Node<Td>*	pnode;

	for( DWORD i=0; i<m_dwMaxBucketNum; i++ )
	{
		if( m_pBucket[i] )
		{
			pnode = m_pBucket[i];
			while( pnode )
			{
				pDel = pnode;
				pnode = pnode->pNext;
				
				if( bDeleteData )
				{
					DeleteData(pDel->pData);	
				}
				LALFree( m_pNodePool, (void*)pDel );
			}
			m_pBucket[i] = NULL;
		}
	}
	
	ListNode<Td>* pListNode;
	pListNode = m_pHead;
	while(pListNode)
	{
		LALFree( m_pListNodePool, pListNode );
		pListNode = pListNode->pNext;
	}
	
	m_pHead = NULL;
	m_pTail = NULL;
	
	m_dwCount = 0;
}

template<class Td>
Td*	CAbyssHash<Td>::GetData( DWORD dwKey )
{
	DWORD dwIndex = dwKey%m_dwMaxBucketNum;
	Node<Td>* pnode = m_pBucket[dwIndex];

	while( pnode )
	{
		if( pnode->dwKey == dwKey )
			return pnode->pData;
		
		pnode = pnode->pNext;
	}

	return NULL;
}


template<class Td>
DWORD CAbyssHash<Td>::GetBuckeCount(DWORD dwKey)
{
	DWORD dwIndex = dwKey%m_dwMaxBucketNum;
	Node<Td>* pnode = m_pBucket[dwIndex];
	DWORD dwCount = 0;

	while( pnode )
	{
		dwCount++;
		pnode = pnode->pNext;		
	}

	return dwCount;
}


template<class Td>	
BOOL CAbyssHash<Td>::bSameKey( DWORD dwKey )		
{									
	DWORD dwIndex = dwKey%m_dwMaxBucketNum;
	Node<Td>*	pCurNode	= m_pBucket[dwIndex];
	
	while( pCurNode )
	{
		if( pCurNode->dwKey == dwKey )	return TRUE;

		pCurNode = pCurNode->pNext;
	}

	return FALSE;
}

template<class Td>
DWORD CAbyssHash<Td>::Insert( Td* pdata, DWORD dwKey )
{
	DWORD dwIndex = dwKey%m_dwMaxBucketNum;

	Node<Td>* pnode = (Node<Td>*)LALAlloc(m_pNodePool);
	
	if( !pnode )
	{
		return DWORD(-1);		
	}

	pnode->dwKey		= dwKey;
	pnode->pData		= pdata;

	if( !m_pBucket[dwIndex] )
	{
		pnode->pPrev	= NULL;
		pnode->pNext	= NULL;
		m_pBucket[dwIndex] = pnode;
	}
	else
	{
		Node<Td>*	pCurNode	= m_pBucket[dwIndex];
		Node<Td>*	pPrevNode	= NULL;
		
		while( pCurNode )
		{
			if (m_bSameKeyCheck)
			{
				if( pCurNode->dwKey == dwKey )	
				{
					LALFree( m_pNodePool, (void*)pnode );
					pnode = NULL;
					return DWORD(-1);
				}
			}

			pPrevNode	= pCurNode;
			pCurNode	= pCurNode->pNext;
		}

		pCurNode = pPrevNode->pNext = pnode;
		pCurNode->pPrev = pPrevNode;
		pCurNode->pNext	= NULL;
	}
	
	pnode->pListPos = AddList( pdata );	

	m_dwCount++;
	
	return dwIndex;
}	

template<class Td>	
void CAbyssHash<Td>::Delete( Td* pdata, DWORD dwKey, BOOL bDeleteData)
{
	DWORD dwIndex = dwKey%m_dwMaxBucketNum;
	
	Node<Td>*	pDel;
	Node<Td>*	pnode	= m_pBucket[dwIndex];
	
	while( pnode )
	{
		if( pnode->pData == pdata )
		{
			pDel = pnode;

			if( pnode->pPrev == NULL )	
				m_pBucket[dwIndex] = pnode->pNext;
			else pnode->pPrev->pNext = pnode->pNext;

			if( pnode->pNext )
				pnode->pNext->pPrev = pnode->pPrev;

			if( bDeleteData ) 
				DeleteData(pDel->pData);
				
			RemoveAtList( pDel->pListPos );	
			
			LALFree( m_pNodePool, (void*)pDel );
			m_dwCount--;
			break;
		}

		pnode = pnode->pNext;
	}	
}

