#include "UserHash.h"

/*
void CUserHash::Delete( Td* pdata, DWORD dwKey, BOOL bDeleteData )
{
	DWORD dwIndex = dwKey%m_dwMaxBucketNum;
	
	Node<Td>*	pDel;
	Node<Td>*	pnode	= m_pBucket[dwIndex];

	while( pnode )
	{
		if( pnode->pData == pdata )
		{
			pDel = pnode;

			if( pnode->pPrev == NULL )	// Head
				m_pBucket[dwIndex] = pnode->pNext;
			else pnode->pPrev->pNext = pnode->pNext;

			if( pnode->pNext )
				pnode->pNext->pPrev = pnode->pPrev;

			if( bDeleteData ) delete pDel->pData;

			LALFree( m_pNodePool, (void*)pDel );
			break;
		}

		pnode = pnode->pNext;
	}

	// 리스트에서도 검색해서 지워준다.
	pnode = m_pHead;
	while( pnode )
	{
		if( pnode->pData == pdata )
		{
			pDel = pnode;
			if( pnode->pPrev == NULL ) //head
				m_pHead = pnode->pNext;
			else 
				pnode->pPrev->pNext = pnode->pNext;

			if( pnode->pNext )
				pnode->pNext->pPrev = pnode->pPrev;
			else	// tail
				m_pTail = pnode->pPrev;

			LALFree( m_pListNodePool, (void*)pDel);
			break;
		}
		pnode = pnode->pNext;
	}

	m_dwNodeNum--;
}



void CUserHash::Destroy( BOOL bDeleteData )
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
					if( pDel->pData )
					{
						delete pDel->pData;
						pDel->pData = NULL;
					}
				}
				LALFree( m_pNodePool, (void*)pDel );
//				pDel = NULL;
			}
			m_pBucket[i] = NULL;
		}
	}
	
	//리스트 삭제.
	pnode = m_pHead;
	while( pnode )
	{
		pDel = pnode;
		pnode = pnode->pNext;

		LALFree( m_pListNodePool, (void*)pDel);
		pDel = NULL;
	}
	
	m_pHead = NULL;
	m_pTail = NULL;
	
	m_dwNodeNum = 0;	
}
*/