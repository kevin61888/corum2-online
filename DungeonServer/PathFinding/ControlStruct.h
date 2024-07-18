#pragma once
#include "../stdafx.h"

// ----------------------------------------------------------
class CLinker
{
private:

	struct LinkItem
	{
		PVOID		pVaddr;
		LinkItem*	pPrev;
		LinkItem*	pNext;
		PVOID		pVMemoryCheckAddr;
	};

	DWORD			m_dwMaxItemNum;
	DWORD			m_dwCurrentItemNum;

	LinkItem*		m_pLinkItemHead;
	LinkItem*		m_pLinkItemTail;

	LinkItem*		m_pLinkItemPoolHead;
	LinkItem*		m_pLinkItemPool;

//	User Setting for Refference
	DWORD			m_dwDataOffset;
	DWORD			m_dwDataSize;

	LinkItem*		m_pCurrentLinkItem;
	PVOID*			m_ppVUserPointerToItem;

	LinkItem*		GetLinkItemMemory();
	LinkItem*		GetLinkItemMemory(PVOID pVaddr);
	BOOL			FreeLinkItemMemory(LinkItem* pLinkItem);

public:

	void		Initialize(DWORD dwMaxItemNum);

	PVOID		AddToLink_Head(PVOID pVaddr);
	PVOID		AddToLink_Tail(PVOID pVaddr);
	PVOID		AddToLink_Prev(PVOID pVaddr);
	PVOID		AddToLink_Next(PVOID pVaddr);

	PVOID		GetItem(PVOID pVItemHandle);
	PVOID		RemoveFromLink(PVOID pVItemHandle);

	void		RemoveAllLink();

	PVOID		GetHeadItem();
	PVOID		GetTailItem();

	PVOID		SetUserPointerToHeadItem();
	PVOID		SetUserPointerToTailItem();
	PVOID		SetUserPointerToNext();
	PVOID		SetUserPointerToPrev();

	void		SetUserPointerToItem(PVOID* ppVaddr)	{ m_ppVUserPointerToItem = ppVaddr; }

	CLinker();
	~CLinker();
};

// ----------------------------------------------------------

class CHash_String
{
	struct HashItem				//	HashItem For String
	{
		PVOID		pVaddr;		//	Stored Address
		char*		pszText;	//	Identifier
		HashItem*	pNext;		//	Next Item
//		HashItem*	pPoolNext;		Pool Link
	};

private:

	DWORD		m_dwMaxBucketNum;
	DWORD		m_dwMaxItemNum;
	DWORD		m_dwKeyLevel;
	HashItem**	m_ppHashItemBucket;

//	DWORD		m_dwCurrentNum;	
	HashItem*	m_pHashItemPoolHead;
//	HashItem*	m_pItemTail;
	HashItem*	m_pHashItemPool;


	HashItem*	GetHashItemMemory();
	HashItem*	GetHashItemMemory(char* pszText, PVOID pVaddr);
	BOOL		FreeHashItemMemory(HashItem* pHashItem);

	DWORD		GetKey(char* pszText);
	BOOL		Identify(char* pszSrc, char* pszDes);

public:

	VOID		Initialize(DWORD dwMaxBucketNum, DWORD dwMaxItemNum, DWORD dwKeyLevel);
	BOOL		Add(char* pszText, PVOID pVaddr);
	PVOID		Remove(char* pszText);
	VOID		RemoveAll();
	
	PVOID		Search(char* pszText);

	CHash_String();
	~CHash_String();
};

// ----------------------------------------------------------

class CHash_Value
{

	struct HashItem				//	HashItem For Value
	{
		PVOID		pVaddr;		
		DWORD		dwValue;
		HashItem*	pNext;
	};

private:

	DWORD		m_dwMaxBucketNum;
	DWORD		m_dwMaxItemNum;
	HashItem**	m_ppHashItemBucket;

	HashItem*	m_pHashItemPoolHead;
	HashItem*	m_pHashItemPool;

	HashItem*	GetHashItemMemory();
	HashItem*	GetHashItemMemory(DWORD dwValue, PVOID pVaddr);
	BOOL		FreeHashItemMemory(HashItem* pHashItem);

	DWORD		GetKey(DWORD dwValue);

public:

	VOID			Initialize(DWORD dwMaxBucketNum, DWORD dwMaxItemNum);
	BOOL			Add(DWORD dwValue, PVOID pVaddr);
	PVOID			Remove(DWORD dwValue);
	VOID			RemoveAll();
	
	PVOID			Search(DWORD dwValue);

	CHash_Value();
	~CHash_Value();
};


