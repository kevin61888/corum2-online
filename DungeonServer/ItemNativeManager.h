#if !defined(AFX_ITEMNATIVEMANAGER_H__4FFCEA94_D33C_400B_BE35_9F196FC43E15__INCLUDED_)
#define AFX_ITEMNATIVEMANAGER_H__4FFCEA94_D33C_400B_BE35_9F196FC43E15__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CItem;


enum ITEM_NATIVE_TYPE
{
	ITEM_NATIVE_TYPE_REAL		= 1,
	ITEM_NATIVE_TYPE_VIRTUAL	= 2,		
};


class CItemNativeManager  
{
	struct ITEM_NATIVE_DESC
	{
		ITEM_NATIVE_TYPE eItemNativeType;
		BYTE byMaxCount;
		DWORD dwASize;
		void* pItem;
	};
	
	ITEM_NATIVE_DESC* m_pItemNativeDesc;
	BYTE m_byItemNativeInfoCount;
	BYTE m_byStartNativeIndex;

	BYTE GetNativeID(BYTE byNativeID) const;

public:

	CItemNativeManager();
	virtual ~CItemNativeManager();

	void			Initialize(BYTE byItemNativeInfoCount, BYTE byStartNativeIndex);
	void			RegistItemNativeInfo(BYTE byNativeID, void* pItem, DWORD dwASize, BYTE byMaxCount, ITEM_NATIVE_TYPE eItemNativeType);
	DWORD			GetItemTotalSize(BYTE byNativeID) const;
	void			RemoveItem(BYTE byNativeID, BYTE bySlotIndex);
	void*			GetItem(BYTE byNativeID, BYTE bySlotIndex) const;
	void			SetItem(BYTE byNativeID, BYTE bySlotIndex, const void* pItem);
	ITEM_NATIVE_TYPE	GetType(BYTE byNativeID) const;
};


#endif 
