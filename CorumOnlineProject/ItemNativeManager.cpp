#include "stdafx.h"
#include "ItemNativeManager.h"
#include "../CommonServer/CommonHeader.h"


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
	m_byStartNativeIndex = byStartNativeIndex;

	m_pItemNativeDesc = new ITEM_NATIVE_DESC[byItemNativeInfoCount];
}

void CItemNativeManager::RegistItemNativeInfo(BYTE byNativeID, CItem* pItem, DWORD dwTotalSize)
{
	byNativeID = GetNativeID(byNativeID);

	assert(byNativeID < m_byItemNativeInfoCount);

	m_pItemNativeDesc[byNativeID].pItem = pItem;
	m_pItemNativeDesc[byNativeID].dwSize = dwTotalSize;
}

DWORD CItemNativeManager::GetItemTotalSize(BYTE byNativeID) const
{
	byNativeID = GetNativeID(byNativeID);

	assert(byNativeID < m_byItemNativeInfoCount);
	
	return m_pItemNativeDesc[byNativeID].dwSize;
}

void CItemNativeManager::RemoveItem(BYTE byNativeID, BYTE bySlotIndex)
{
	byNativeID = GetNativeID(byNativeID);
	
	CItem* pItem = GetItem(byNativeID, bySlotIndex);
	
	memset(pItem, 0, sizeof(CItem));
}

CItem* CItemNativeManager::GetItem(BYTE byNativeID, BYTE bySlotIndex)
{
	byNativeID = GetNativeID(byNativeID);

	assert(byNativeID < m_byItemNativeInfoCount);
	assert(bySlotIndex < GetItemTotalSize(byNativeID)/sizeof(CItem));

	return &m_pItemNativeDesc[byNativeID].pItem[bySlotIndex];
}

void CItemNativeManager::SetItem(BYTE byNativeID, BYTE bySlotIndex, const CItem* pItem)
{
	byNativeID = GetNativeID(byNativeID);

	memcpy(GetItem(byNativeID, bySlotIndex), pItem, sizeof(CItem));
}

void CItemNativeManager::ConvertItem(CItem* pItem)
{
	WORD wOldID = pItem->GetID();
	WORD wNewID = ( wOldID / 100 * 200 ) + wOldID % 100;
	pItem->SetID(wNewID);
}

BYTE CItemNativeManager::GetNativeID(BYTE byNativeID) const
{
	assert(m_byStartNativeIndex < byNativeID);

	return byNativeID - m_byStartNativeIndex;
}