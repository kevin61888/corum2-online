#pragma once


#include "Windows.h"
#include "AbyssHash.h"


template<class TData>
struct Node64 
{
	__int64			 i64Key;
	TData*			 pData;
	ListNode<TData>* pListPos;
	Node64<TData>*	 pPrev;
	Node64<TData>*	 pNext;
};


template<class Td>
class CAbyssHash64
{
	DWORD			m_dwMaxBucketNum;
	DWORD			m_dwCount;
	Node64<Td>**	m_pBucket;
	ListNode<Td>*	m_pHead;			 
	ListNode<Td>*	m_pTail;			 
	STMPOOL_HANDLE	m_pNodePool;		
	STMPOOL_HANDLE	m_pListNodePool;	// 리스트 노드풀.

public:

	DWORD			GetMaxBucketNum()					{ return m_dwMaxBucketNum; }
	DWORD			GetCount()							{ return m_dwCount; }			 
	Node64<Td>*		GetBucketHead( DWORD dwKey )		{ return m_pBucket[dwKey%m_dwMaxBucketNum]; }
	
	ListNode<Td>*	GetHead()						{ return m_pHead; }
	ListNode<Td>*	GetTail()						{ return m_pTail; }

public:

	CAbyssHash64();		 
	~CAbyssHash64();

	void			InitializeHashTable( DWORD dwMaxBucket, DWORD dwMaxNode );	
	
	void			Destroy( BOOL bDeleteData = FALSE );
	void			Delete( Td* pdata, __int64 i64Key, BOOL bDeleteData = FALSE );	

	ListNode<Td>*	AddList(Td* pData);								 
	void			RemoveAtList( ListNode<Td>* pListNode );		 
	
	Td*				GetData( __int64 i64Key );

	BOOL			bSameKey( __int64 i64Key );
	__int64			Insert( Td* pdata, __int64 i64Key );	 

	virtual	void	DeleteData( Td* pData );		 
};


template<class Td>
CAbyssHash64<Td>::CAbyssHash64()
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
CAbyssHash64<Td>::~CAbyssHash64()
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
void CAbyssHash64<Td>::InitializeHashTable( DWORD dwMaxBucket, DWORD dwMaxNode )
{
	m_dwMaxBucketNum = dwMaxBucket;
	
	m_pBucket = new Node64<Td>*[(DWORD)m_dwMaxBucketNum];
	memset( m_pBucket, 0, sizeof(Td*) * (DWORD)m_dwMaxBucketNum );
	
	m_pNodePool		= CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pNodePool, sizeof( Node64<Td> ), (DWORD)dwMaxNode, (DWORD)dwMaxNode);

	m_pListNodePool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pListNodePool, sizeof( ListNode<Td> ), (DWORD)dwMaxNode, (DWORD)dwMaxNode);
}


template<class Td>
void CAbyssHash64<Td>::DeleteData( Td* pData )
{
	if( pData )
		delete (Td*)pData;
}


template<class Td>
ListNode<Td>*	CAbyssHash64<Td>::AddList(Td* pData)
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
void CAbyssHash64<Td>::RemoveAtList( ListNode<Td>* pListNode )
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
void	CAbyssHash64<Td>::Destroy( BOOL bDeleteData )
{

	Node64<Td>*	pDel;
	Node64<Td>*	pnode;

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
	
	// 리스트 삭제.
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
Td*	CAbyssHash64<Td>::GetData( __int64 i64Key )
{
	__int64 i64Index = i64Key%m_dwMaxBucketNum;
	Node64<Td>* pnode = m_pBucket[i64Index];

	while( pnode )
	{
		if( pnode->i64Key == i64Key )
			return pnode->pData;
		
		pnode = pnode->pNext;
	}

	return NULL;
}

template<class Td>	
BOOL CAbyssHash64<Td>::bSameKey( __int64 i64Key )		
{									
	__int64 i64Index = i64Key%m_dwMaxBucketNum;
	Node<Td>*	pCurNode	= m_pBucket[i64Index];
	
	while( pCurNode )
	{
		if( pCurNode->dwKey == dwKey )	return TRUE;

		pCurNode = pCurNode->pNext;
	}

	return FALSE;
}


template<class Td>
__int64 CAbyssHash64<Td>::Insert( Td* pdata, __int64 i64Key )
{
	__int64 i64Index = i64Key%m_dwMaxBucketNum;

	Node64<Td>* pnode = (Node64<Td>*)LALAlloc(m_pNodePool);
	
	if( !pnode )
	{
		__asm int 3
		return -1;		// 더이상 할당할수 없으면 에러리턴.
	}

	pnode->i64Key		= i64Key;
	pnode->pData		= pdata;

	if( !m_pBucket[i64Index] )
	{
		pnode->pPrev	= NULL;
		pnode->pNext	= NULL;
		m_pBucket[i64Index] = pnode;
	}
	else
	{
		Node64<Td>*	pCurNode	= m_pBucket[i64Index];
		Node64<Td>*	pPrevNode	= NULL;
		while( pCurNode )
		{
			if( pCurNode->i64Key == i64Key )	// 같은 키값이 들어오면 넣지 않는다.
			{
				LALFree( m_pNodePool, (void*)pnode );
				pnode = NULL;
				return -1;
			}

			pPrevNode	= pCurNode;
			pCurNode	= pCurNode->pNext;
		}
		pCurNode = pPrevNode->pNext = pnode;
		pCurNode->pPrev = pPrevNode;
		pCurNode->pNext	= NULL;
	}
	
	pnode->pListPos = AddList( pdata );		//List에 추가 

	m_dwCount++;
	
	return i64Index;
}	


template<class Td>	
void CAbyssHash64<Td>::Delete( Td* pdata, __int64 i64Key, BOOL bDeleteData)
{
	__int64 i64Index = i64Key % m_dwMaxBucketNum;
	
	Node64<Td>*	pDel;
	Node64<Td>*	pnode = m_pBucket[i64Index];
	
	while( pnode )
	{
		if( pnode->pData == pdata )
		{
			pDel = pnode;

			if( pnode->pPrev == NULL )	// Head
				m_pBucket[i64Index] = pnode->pNext;
			else
				pnode->pPrev->pNext = pnode->pNext;

			if( pnode->pNext )
				pnode->pNext->pPrev = pnode->pPrev;

			if( bDeleteData ) 
			{
				DeleteData(pDel->pData);
			}

			// List에서 삭제 
			RemoveAtList( pDel->pListPos );	
			
			LALFree( m_pNodePool, (void*)pDel );

			break;
		}

		pnode = pnode->pNext;
	}

	m_dwCount--;
}
