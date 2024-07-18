#include "stdafx.h"
#include "ItemNativeManager.h"


CItemNativeManager::CItemNativeManager()
{
	m_pItemNativeDesc = NULL;
	m_byItemNativeInfoCount = 0;
}


CItemNativeManager::~CItemNativeManager()
{
	if (m_pItemNativeDesc)
	{
		delete [] m_pItemNativeDesc;
		m_pItemNativeDesc = NULL;
	}
}


void CItemNativeManager::Initialize(BYTE byItemNativeInfoCount, BYTE byStartNativeIndex)
{
	assert(!m_pItemNativeDesc);
	
	m_byItemNativeInfoCount = byItemNativeInfoCount;
	m_byStartNativeIndex	= byStartNativeIndex;

	m_pItemNativeDesc = new ITEM_NATIVE_DESC[byItemNativeInfoCount];
}


void CItemNativeManager::RegistItemNativeInfo(BYTE byNativeID, void* pItem, DWORD dwASize, BYTE byMaxCount, ITEM_NATIVE_TYPE eItemNativeType)
{
	byNativeID = GetNativeID(byNativeID);

	assert(byNativeID < m_byItemNativeInfoCount);

	m_pItemNativeDesc[byNativeID].pItem				= pItem;
	m_pItemNativeDesc[byNativeID].dwASize			= dwASize;
	m_pItemNativeDesc[byNativeID].byMaxCount		= byMaxCount;
	m_pItemNativeDesc[byNativeID].eItemNativeType	= eItemNativeType;
}


DWORD CItemNativeManager::GetItemTotalSize(BYTE byNativeID) const
{
	byNativeID = GetNativeID(byNativeID);

	assert(byNativeID < m_byItemNativeInfoCount);
	
	return m_pItemNativeDesc[byNativeID].dwASize * m_pItemNativeDesc[byNativeID].byMaxCount;
}


void CItemNativeManager::RemoveItem(BYTE byNativeID, BYTE bySlotIndex)
{
	byNativeID = GetNativeID(byNativeID);
	
	void* pItem = GetItem(byNativeID, bySlotIndex);
	
	memset(pItem, 0, m_pItemNativeDesc[byNativeID].dwASize);
}


void* CItemNativeManager::GetItem(BYTE byNativeID, BYTE bySlotIndex) const
{
#ifdef _DEBUG
	BYTE OldbyNativeID = byNativeID;
#endif
	byNativeID = GetNativeID(byNativeID);

	assert(byNativeID < m_byItemNativeInfoCount);
	assert(bySlotIndex < GetItemTotalSize(OldbyNativeID) / m_pItemNativeDesc[byNativeID].dwASize);

	return (char*)m_pItemNativeDesc[byNativeID].pItem + bySlotIndex * m_pItemNativeDesc[byNativeID].dwASize;
}


void CItemNativeManager::SetItem(BYTE byNativeID, BYTE bySlotIndex, const void* pItem)
{
	BYTE OldbyNativeID = byNativeID;
	byNativeID = GetNativeID(byNativeID);
	void* pItemSrc = GetItem(OldbyNativeID, bySlotIndex);
	memcpy(pItemSrc, pItem, m_pItemNativeDesc[byNativeID].dwASize);
}


BYTE CItemNativeManager::GetNativeID(BYTE byNativeID) const
{
	assert(m_byStartNativeIndex < byNativeID);

	return BYTE(byNativeID - m_byStartNativeIndex);
}


ITEM_NATIVE_TYPE CItemNativeManager::GetType(BYTE byNativeID) const
{
	byNativeID = GetNativeID(byNativeID);

	return m_pItemNativeDesc[byNativeID].eItemNativeType;
}