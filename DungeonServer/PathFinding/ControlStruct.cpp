#include "ControlStruct.h"



// ----------------------------------------------------------
//			CLinker
// ----------------------------------------------------------
CLinker::LinkItem* CLinker::GetLinkItemMemory()
{
	LinkItem* pTempLinkItem;

	if( !m_pLinkItemPoolHead)
		return NULL;

	pTempLinkItem			= m_pLinkItemPoolHead;
	m_pLinkItemPoolHead		= m_pLinkItemPoolHead->pNext;
	pTempLinkItem->pNext	= NULL;

	return pTempLinkItem;
}

CLinker::LinkItem* CLinker::GetLinkItemMemory(PVOID pVaddr)
{
	LinkItem* pTempLinkItem;

	if( !m_pLinkItemPoolHead)
		return NULL;

	pTempLinkItem			= m_pLinkItemPoolHead;
	m_pLinkItemPoolHead		= m_pLinkItemPoolHead->pNext;
	pTempLinkItem->pNext	= NULL;
	pTempLinkItem->pVaddr	= pVaddr;

	return pTempLinkItem;
}

BOOL CLinker::FreeLinkItemMemory(LinkItem* pLinkItem)
{
	memset(pLinkItem, 0, sizeof(LinkItem));

	pLinkItem->pNext	= m_pLinkItemPoolHead;
	m_pLinkItemPoolHead = pLinkItem;
	
	return TRUE;
}

VOID CLinker::Initialize(DWORD dwMaxItemNum)
{
	if( m_pLinkItemPool)
		delete [] m_pLinkItemPool;
	memset(this, 0, sizeof(CLinker));

	m_dwMaxItemNum		= dwMaxItemNum;
	m_dwCurrentItemNum	= dwMaxItemNum;
	
	m_pLinkItemPool		= new LinkItem[dwMaxItemNum];

	memset(m_pLinkItemPool, 0, dwMaxItemNum * sizeof(LinkItem));

	for( DWORD dwi = 0; dwi < dwMaxItemNum - 1; dwi++)
		m_pLinkItemPool[dwi].pNext = m_pLinkItemPool + dwi + 1;
	
	m_pLinkItemPool[dwMaxItemNum - 1].pNext = NULL;
	m_pLinkItemPoolHead = m_pLinkItemPool;
}

PVOID CLinker::AddToLink_Head(PVOID pVaddr)
{
	LinkItem* pLinkItem = GetLinkItemMemory(pVaddr);	//		new

	if( !pLinkItem)
	{
//		printf("\n( [ Linker_Object ] AddToLink_Head Fail : Address is %x )\n",pVaddr);
		return pLinkItem;
	}

	pLinkItem->pVMemoryCheckAddr = (PVOID)pLinkItem;
	
	if(m_pLinkItemHead)
	{
		pLinkItem->pNext = m_pLinkItemHead;
		m_pLinkItemHead	 = pLinkItem;
	}
	else
	{
		m_pLinkItemHead = m_pLinkItemTail = pLinkItem;
	}

	return pLinkItem;
}


PVOID CLinker::AddToLink_Tail(PVOID pVaddr)
{
	LinkItem* pLinkItem = GetLinkItemMemory(pVaddr);	//		new

	if( !pLinkItem)
	{
//		printf("\n( [ Linker_Object ] AddToLink_Tail Fail : Address is %x )\n",pVaddr);
		return pLinkItem;
	}

	pLinkItem->pVMemoryCheckAddr = (PVOID)pLinkItem;

	if(m_pLinkItemHead )
	{
		m_pLinkItemTail->pNext = pLinkItem;
		pLinkItem->pPrev = m_pLinkItemTail;
		m_pLinkItemTail = pLinkItem;
	}
	else
	{
		m_pLinkItemHead = m_pLinkItemTail = pLinkItem;
	}

//	printf("( [ Linker_Object ] AddToLink_Tail : Address is %x )\n",pVaddr);

	return pLinkItem;
}

PVOID CLinker::AddToLink_Prev(PVOID pVaddr)
{
	LinkItem* pLinkItem, *pPrev;

	if( !m_pLinkItemHead)
		return AddToLink_Head(pVaddr);


	pLinkItem = GetLinkItemMemory(pVaddr);	//		new
	
	if( !pLinkItem)
	{
//		printf("\n( [ Linker_Object ] AddToLink_Prev : Address is %x )\n",pVaddr);
		return pLinkItem;
	}

	pLinkItem->pVMemoryCheckAddr = (PVOID)pLinkItem;

	pPrev = m_pCurrentLinkItem->pPrev;
	
	if( pPrev)
	{
		pPrev->pNext = pLinkItem;
		pLinkItem->pPrev = pPrev;
	}
	else
		m_pLinkItemHead = pLinkItem;
	
	pLinkItem->pNext = m_pCurrentLinkItem;
	m_pCurrentLinkItem->pPrev = pLinkItem;
	
	return pLinkItem;
}

PVOID CLinker::AddToLink_Next(PVOID pVaddr)
{
	LinkItem* pLinkItem, *pNext;

	if( !m_pLinkItemHead)
		return AddToLink_Head(pVaddr);


	pLinkItem = GetLinkItemMemory(pVaddr);	//		new
	
	if( !pLinkItem)
	{
//		printf("\n( [ Linker_Object ] AddToLink_Prev : Address is %x )\n",pVaddr);
		return pLinkItem;
	}
	pLinkItem->pVMemoryCheckAddr = (PVOID)pLinkItem;


	pNext = m_pCurrentLinkItem->pNext;
	
	if( pNext)
	{
		pNext->pPrev = pLinkItem;
		pLinkItem->pNext = pNext;
	}
	else
		m_pLinkItemTail = pLinkItem;
	
	m_pCurrentLinkItem->pNext = pLinkItem;
	pLinkItem->pPrev = m_pCurrentLinkItem;
	
	return pLinkItem;
}

PVOID CLinker::GetItem(PVOID pVItemHandle)
{
	LinkItem* pLinkItem = (LinkItem*)pVItemHandle;

	if( pVItemHandle != pLinkItem->pVMemoryCheckAddr )
		return NULL;

	return pLinkItem->pVaddr;
}

PVOID CLinker::RemoveFromLink(PVOID pVItemHandle)
{
	PVOID	pVItemaddr;
	LinkItem* pLinkItem = (LinkItem*)pVItemHandle;

	if( pVItemHandle != pLinkItem->pVMemoryCheckAddr )
		return NULL;

	LinkItem* pPrev = pLinkItem->pPrev;
	LinkItem* pNext = pLinkItem->pNext;

	if( pPrev)
		pPrev->pNext = pNext;
	else
	{
		m_pLinkItemHead = pNext;
	}

	if( pNext)
		pNext->pPrev = pPrev;
	else
	{
		m_pLinkItemTail = pPrev;
	}

	pVItemaddr = pLinkItem->pVaddr;
	FreeLinkItemMemory( pLinkItem);		//		delete
	
	return pVItemaddr;
}

VOID CLinker::RemoveAllLink()
{
	memset(m_pLinkItemPool, 0, m_dwMaxItemNum * sizeof(LinkItem));

	for( DWORD dwi = 0; dwi < m_dwMaxItemNum - 1; dwi++)
		m_pLinkItemPool[dwi].pNext = m_pLinkItemPool + dwi + 1;
	
	m_pLinkItemPool[m_dwMaxItemNum - 1].pNext = NULL;
	m_pLinkItemPoolHead = m_pLinkItemPool;

	m_pLinkItemHead = m_pLinkItemTail = NULL;
}


PVOID CLinker::SetUserPointerToHeadItem()
{
	if( !m_pLinkItemHead )
		return NULL;

	m_pCurrentLinkItem = m_pLinkItemHead;
	*m_ppVUserPointerToItem = m_pCurrentLinkItem->pVaddr;
	return m_pCurrentLinkItem;
}

PVOID CLinker::SetUserPointerToTailItem()
{
	m_pCurrentLinkItem = m_pLinkItemTail;
	*m_ppVUserPointerToItem = m_pCurrentLinkItem->pVaddr;
	return m_pCurrentLinkItem;
}

PVOID CLinker::SetUserPointerToNext()
{
//	if( !m_pCurrentLinkItem->pNext)
//		return FALSE;

	m_pCurrentLinkItem = m_pCurrentLinkItem->pNext;

	if( !m_pCurrentLinkItem)
	{
		*m_ppVUserPointerToItem = NULL;
		return NULL;
	}

	*m_ppVUserPointerToItem = m_pCurrentLinkItem->pVaddr;
	return m_pCurrentLinkItem;
}

PVOID CLinker::SetUserPointerToPrev()
{
	if(m_pCurrentLinkItem->pPrev)
		return NULL;

	m_pCurrentLinkItem = m_pCurrentLinkItem->pPrev;
	*m_ppVUserPointerToItem = m_pCurrentLinkItem->pVaddr;
	return m_pCurrentLinkItem;
}

PVOID CLinker::GetHeadItem()
{
	return m_pLinkItemHead->pVaddr;
}

PVOID CLinker::GetTailItem()
{
	return m_pLinkItemTail->pVaddr;
}


CLinker::CLinker()
{
	memset(this, 0, sizeof(CLinker));
}

CLinker::~CLinker()
{
	if(m_pLinkItemPool)
	{
		RemoveAllLink();
		delete [] m_pLinkItemPool;
		m_pLinkItemPool = NULL;
	}
}




// ----------------------------------------------------------
//			CHash_String
// ----------------------------------------------------------
CHash_String::HashItem*	CHash_String::GetHashItemMemory()
{
	HashItem*	pTempHashItem;
	if( !m_pHashItemPoolHead)
		return NULL;

	
	pTempHashItem		= m_pHashItemPoolHead;
	m_pHashItemPoolHead = m_pHashItemPoolHead->pNext;

//	memset(pTempHashItem, 0, sizeof(HashItem));		//	메모리를 요구할때는 .
	pTempHashItem->pNext= NULL;		//	 지워준다.

	return pTempHashItem;
}

CHash_String::HashItem*	CHash_String::GetHashItemMemory(char* pszText, PVOID pVaddr)
{
	HashItem*	pTempHashItem;
	if( !m_pHashItemPoolHead)
		return NULL;
	
	pTempHashItem		= m_pHashItemPoolHead;
	m_pHashItemPoolHead = m_pHashItemPoolHead->pNext;

	pTempHashItem->pszText	= pszText;
	pTempHashItem->pVaddr	= pVaddr;
	pTempHashItem->pNext	= NULL;

	return pTempHashItem;
}


BOOL CHash_String::FreeHashItemMemory(CHash_String::HashItem* pHashItem)
{
	memset(pHashItem, 0, sizeof(HashItem));		//	다 지우고..

	pHashItem->pNext	= m_pHashItemPoolHead;
	m_pHashItemPoolHead = pHashItem;

	return TRUE;
}


VOID CHash_String::Initialize(DWORD dwMaxBucketNum, DWORD dwMaxItemNum, DWORD dwKeyLevel)
{
	memset( this, 0, sizeof(CHash_String));

	m_dwMaxBucketNum	= dwMaxBucketNum;
	m_dwMaxItemNum		= dwMaxItemNum;
	m_dwKeyLevel		= dwKeyLevel;

	m_ppHashItemBucket	= new HashItem*[dwMaxBucketNum];
	m_pHashItemPool		= new HashItem[dwMaxItemNum];

	memset(m_ppHashItemBucket, 0, dwMaxBucketNum * sizeof(DWORD));
	memset(m_pHashItemPool, 0, dwMaxItemNum * sizeof(HashItem));

	for( DWORD dwi= 0; dwi < dwMaxItemNum - 1; dwi++)
		m_pHashItemPool[dwi].pNext = m_pHashItemPool + dwi + 1;

	m_pHashItemPool[dwMaxItemNum - 1].pNext = NULL;
	m_pHashItemPoolHead = m_pHashItemPool;
}


DWORD CHash_String::GetKey(char* pszText)
{
	DWORD	dwKey = 0;
	for( DWORD i = 0; i < m_dwKeyLevel; i++)
		dwKey += (DWORD)pszText[i];
	dwKey %= m_dwMaxBucketNum;
	return dwKey;
}


BOOL CHash_String::Identify(char* pszSrc, char* pszDes)
{
	if( !pszSrc)
	{
//		printf("error Item deleted .. check it\n\n");
		return FALSE;
	}

	for( DWORD i = 0; (pszSrc[i] == pszDes[i]); i++)
		if( pszSrc[i] == 0 /* || pszSrc[i] == 32 */ ) 
		{
//			printf("\n( [ Hash_String   ] Compare Result : Identical )\n");
			return TRUE;
		}

//	printf("\n( [ Hash_String   ] Compare Result : Not Identical )\n");
	return FALSE;
}


BOOL CHash_String::Add(char* pszText, PVOID pVaddr)
{
	DWORD dwKey = GetKey(pszText);
	HashItem* pHashItem = (HashItem*)(((DWORD)(m_ppHashItemBucket + dwKey)) - ((DWORD)(&(((HashItem*)0)->pNext))));
	while( pHashItem->pNext)
	{
		pHashItem = pHashItem->pNext;
		if( Identify( pHashItem->pszText, pszText))
		{
//			printf("\n( [ Hash_String   ] Insert String : Fail , %s )\n",pszText);		//////////////
			return 0;
		}
	}

//	printf("\n( [ Hash_String   ] Insert String : Success , %s )\n", pszText);
	return BOOL(pHashItem->pNext = GetHashItemMemory(pszText, pVaddr));		//		new
}

PVOID CHash_String::Remove(char* pszText)
{
	DWORD dwKey = GetKey(pszText);
	HashItem* pHashItem = (HashItem*)(((DWORD)(m_ppHashItemBucket + dwKey)) - ((DWORD)(&(((HashItem*)0)->pNext))));
	HashItem* pOldHashItem = pHashItem;
	PVOID pVaddr;

	pHashItem = pHashItem->pNext;
	while( pHashItem )	//KUK
	{
		if( Identify( pHashItem->pszText, pszText))
		{
			pOldHashItem->pNext = pHashItem->pNext;
//			printf("\n( [ Hash_String   ] remove String : Success , %s )\n",pHashItem->pszText);
			pVaddr = pHashItem->pVaddr;
			FreeHashItemMemory( pHashItem);		//		delete
			return pVaddr;
		}
		pOldHashItem	= pHashItem;
		pHashItem		= pHashItem->pNext;
	}

//	printf("\n( [ Hash_String   ] Remove String : Fail , %s , No Item)\n",pszText);	/////////////////
	return 0;
}

VOID CHash_String::RemoveAll()
{
	HashItem *pHashItem, *pTempHashItem;

	for (DWORD dwi = 0; dwi < m_dwMaxBucketNum; dwi++)
		if(	m_ppHashItemBucket[dwi])
		{
			pHashItem = m_ppHashItemBucket[dwi];
			do
			{
				pTempHashItem = pHashItem;
				pHashItem = pHashItem->pNext;
				FreeHashItemMemory( pTempHashItem);		//		delete
			} while(pHashItem);
			m_ppHashItemBucket[dwi] = NULL;
		}
}

PVOID CHash_String::Search(char* pszText)
{
	DWORD dwKey = GetKey(pszText);
	HashItem* pHashItem = (HashItem*)(((DWORD)(m_ppHashItemBucket + dwKey)) - ((DWORD)(&(((HashItem*)0)->pNext))));
	pHashItem = pHashItem->pNext;
	while( pHashItem )	//KUK
	{
		if( Identify( pHashItem->pszText, pszText))
			return pHashItem->pVaddr;
		pHashItem = pHashItem->pNext;
	}
	return 0;
}

CHash_String::CHash_String()
{
	memset( this, 0, sizeof(CHash_String));
}

CHash_String::~CHash_String()
{
	RemoveAll();
       
	if(m_ppHashItemBucket)
		delete [] m_ppHashItemBucket;
	if(m_pHashItemPool)
		delete [] m_pHashItemPool;
}



// ----------------------------------------------------------
//			CHash_Value
// ----------------------------------------------------------
CHash_Value::HashItem*	CHash_Value::GetHashItemMemory()
{
	HashItem*	pTempHashItem;
	if( !m_pHashItemPoolHead)
		return NULL;
	
	pTempHashItem = m_pHashItemPoolHead;
	m_pHashItemPoolHead = m_pHashItemPoolHead->pNext;
	pTempHashItem->pNext= NULL;		//	다 지워준다.

	return pTempHashItem;
}

CHash_Value::HashItem*	CHash_Value::GetHashItemMemory(DWORD dwValue, PVOID pVaddr)
{
	HashItem*	pTempHashItem;
	if( !m_pHashItemPoolHead)
		return NULL;
	
	pTempHashItem = m_pHashItemPoolHead;
	m_pHashItemPoolHead = m_pHashItemPoolHead->pNext;

	pTempHashItem->pNext	= NULL;
	pTempHashItem->dwValue	= dwValue;
	pTempHashItem->pVaddr	= pVaddr;

	return pTempHashItem;
}

BOOL CHash_Value::FreeHashItemMemory(CHash_Value::HashItem* pHashItem)
{
	memset(pHashItem, 0, sizeof(HashItem));		//	다 지우고..

	pHashItem->pNext = m_pHashItemPoolHead;      
	m_pHashItemPoolHead = pHashItem;

	return TRUE;
}

VOID CHash_Value::Initialize(DWORD dwMaxBucketNum, DWORD dwMaxItemNum)
{
	memset( this, 0, sizeof(CHash_Value));

	m_dwMaxBucketNum	= dwMaxBucketNum;
	m_dwMaxItemNum		= dwMaxItemNum;

	m_ppHashItemBucket  = new HashItem*[dwMaxBucketNum];
	m_pHashItemPool		= new HashItem[dwMaxItemNum];

	memset(m_ppHashItemBucket, 0, dwMaxBucketNum * sizeof(DWORD));
	memset(m_pHashItemPool, 0, dwMaxItemNum * sizeof(HashItem));

	for( DWORD dwi = 0; dwi < dwMaxItemNum - 1; dwi++)
		m_pHashItemPool[dwi].pNext = m_pHashItemPool + dwi + 1;	

	m_pHashItemPool[dwMaxItemNum - 1].pNext = NULL;
	m_pHashItemPoolHead = m_pHashItemPool;
}

inline DWORD CHash_Value::GetKey(DWORD dwValue)
{
	return (dwValue %= m_dwMaxBucketNum);
}

PVOID CHash_Value::Search(DWORD dwValue)
{
	DWORD dwKey = GetKey( dwValue);	
	HashItem* pHashItem = (HashItem*)(((DWORD)(m_ppHashItemBucket + dwKey)) - ((DWORD)(&(((HashItem*)0)->pNext))));
	pHashItem = pHashItem->pNext;
	while( pHashItem )	//KUK
	{
		if( pHashItem->dwValue == dwValue)
			return pHashItem->pVaddr;
		pHashItem = pHashItem->pNext;
	}
	return 0;
}

BOOL CHash_Value::Add(DWORD dwValue, PVOID pVaddr)
{
	DWORD dwKey = GetKey(dwValue);
	HashItem* pHashItem = (HashItem*)(((DWORD)(m_ppHashItemBucket + dwKey)) - ((DWORD)(&(((HashItem*)0)->pNext))));
	

	while( pHashItem->pNext)
	{
		if( !(pHashItem->pNext->pVaddr))
		{
//			printf("error Item deleted .. check it\n\n");
			goto Lb_s;
		}

		pHashItem = pHashItem->pNext;
		if( pHashItem->dwValue == dwValue)
		{
//			printf("\n( [ Hash_Value    ] Insert Value : Fail , %d\n", dwValue);
			return 0;
		}
	}
//	printf("\n( [ Hash_Value    ] Insert Value : Success , %d \n", dwValue);
Lb_s:
	return BOOL(pHashItem->pNext = GetHashItemMemory(dwValue, pVaddr));		//		new
}

PVOID CHash_Value::Remove(DWORD dwValue)
{
	DWORD dwKey = GetKey(dwValue);
	HashItem* pHashItem = (HashItem*)(((DWORD)(m_ppHashItemBucket + dwKey)) - ((DWORD)(&(((HashItem*)0)->pNext))));
	HashItem* pOldHashItem = pHashItem;
	PVOID pVaddr;

	pHashItem = pHashItem->pNext;
	while( pHashItem )	//KUK
	{
		if( pHashItem->dwValue == dwValue)
		{
			pOldHashItem->pNext = pHashItem->pNext;
			pVaddr = pHashItem->pVaddr;
			FreeHashItemMemory( pHashItem);		//		delete
			return pVaddr;
		}
		pOldHashItem	= pHashItem;
		pHashItem		= pHashItem->pNext;
	}
	return 0;
}

VOID CHash_Value::RemoveAll()
{
	HashItem *pHashItem, *pTempHashItem;

	for (DWORD dwi = 0; dwi < m_dwMaxBucketNum; dwi++)
		if(	m_ppHashItemBucket[dwi])
		{
			pHashItem = m_ppHashItemBucket[dwi];
			do
			{
				pTempHashItem = pHashItem;
				pHashItem = pHashItem->pNext;
				FreeHashItemMemory( pTempHashItem);		//		delete
			} while(pHashItem);
			m_ppHashItemBucket[dwi] = NULL;
		}
}

CHash_Value::CHash_Value()
{
	memset( this, 0, sizeof(CHash_Value));
}

CHash_Value::~CHash_Value()
{
	RemoveAll();

	if(m_ppHashItemBucket)
		delete [] m_ppHashItemBucket;
	if(m_pHashItemPool)
		delete [] m_pHashItemPool;
}
// ----------------------------------------------------------

