#if !defined(AFX_ITEMNATIVEMANAGER_H__98178D55_8328_4D5E_837E_5CCCA4ABE04B__INCLUDED_)
#define AFX_ITEMNATIVEMANAGER_H__98178D55_8328_4D5E_837E_5CCCA4ABE04B__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


class CItem;


class CItemNativeManager  
{
	struct ITEM_NATIVE_DESC
	{
		DWORD dwSize;
		CItem* pItem;
	};
	
	ITEM_NATIVE_DESC* m_pItemNativeDesc;
	BYTE m_byItemNativeInfoCount;
	BYTE m_byStartNativeIndex;

	BYTE GetNativeID(BYTE byNativeID) const;

public:

	CItemNativeManager();
	virtual ~CItemNativeManager();

	void			Initialize(BYTE byItemNativeInfoCount, BYTE byStartNativeIndex);
	void			RegistItemNativeInfo(BYTE byNativeID, CItem* pItem, DWORD dwTotalSize);
	DWORD			GetItemTotalSize(BYTE byNativeID) const;
	void			RemoveItem(BYTE byNativeID, BYTE bySlotIndex);
	CItem*			GetItem(BYTE byNativeID, BYTE bySlotIndex);
	void			SetItem(BYTE byNativeID, BYTE bySlotIndex, const CItem* pItem);
	void			ConvertItem(CItem* pItem);
};

#endif // !defined(AFX_ITEMNATIVEMANAGER_H__98178D55_8328_4D5E_837E_5CCCA4ABE04B__INCLUDED_)
