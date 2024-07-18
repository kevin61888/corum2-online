#include "stdafx.h"
#include "TraderMonster.h"
#include "ClassMemoryPool.h"
#include "ItemNativeManager.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "GameSystemNative.h"


CTraderMonster::CTraderMonster()
{
}


CTraderMonster::~CTraderMonster()
{
	RemoveResource();
}


void CTraderMonster::RemoveResource()
{

	if (m_pItemNativeManager)
	{
		delete m_pItemNativeManager;
		m_pItemNativeManager = NULL;
	}
}


void CTraderMonster::CreateResource()
{	
	CMonster::CreateResource();
	
	m_pItemNativeManager = new CItemNativeManager;
	m_pItemNativeManager->Initialize(::GetItemNativeTraderMonsterCount(), ITEM_NATIVE_TRADERMONSTER_START);
	
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_TRADERMONSTER_INV_LARGE
							, m_Inv_Large
							, sizeof(CItem)
							, MAX_TRADERMONSTER_INV_LARGE
							, ITEM_NATIVE_TYPE_REAL);
	
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_TRADERMONSTER_INV_SMALL
							, m_Inv_Small
							, sizeof(CItem)
							, MAX_TRADERMONSTER_INV_SMALL
							, ITEM_NATIVE_TYPE_REAL);	

	CreateStoreItem();	
}


void CTraderMonster::CreateStoreItem()
{
	BYTE byStoreType	= (BYTE)g_NPCTable[GetClass()].dwType;
	WORD wDungeonID		= GetCurDungeonID();
	
	ListNode<ITEM_STORENATIVE>* lpItemStoreNativeNode	= NULL;
	LP_ITEM_STORENATIVE			lpItemStoreNative		= NULL;
	BYTE bySlotIndex									= 0;

	lpItemStoreNativeNode = g_pItemStoreNativeHash->GetHead();

	while(lpItemStoreNativeNode)
	{
		lpItemStoreNative = lpItemStoreNativeNode->pData;
		
		if(NULL == lpItemStoreNative)
		{
			lpItemStoreNativeNode = lpItemStoreNativeNode->pNext;
			continue;
		}

		if(wDungeonID != lpItemStoreNative->wMapId)
		{
			lpItemStoreNativeNode = lpItemStoreNativeNode->pNext;
			continue;
		}
		
		CBaseItem* pBaseItem = g_pBaseItemHash->GetData(lpItemStoreNative->wItemId);

		if(NULL == pBaseItem)
		{
			lpItemStoreNativeNode = lpItemStoreNativeNode->pNext;
			continue;
		}

		if(byStoreType == STORE_TYPE_WEAPON)
		{
			if( GetItemKind(pBaseItem->wID) == ITEM_KIND_WEAPON )
			{
				CItem cItem;
				memset(&cItem,0,sizeof(CItem));

				if( CreateItem(&cItem, lpItemStoreNative->wItemId,1 ) )
				{
					SetItem(ITEM_NATIVE_TRADERMONSTER_INV_LARGE, bySlotIndex, &cItem);
					bySlotIndex += 1;
				}
			}
		}
		else if(byStoreType == STORE_TYPE_ARMOR)
		{
			if( GetItemKind(pBaseItem->wID) == ITEM_KIND_AROMR )
			{
				CItem cItem;
				memset(&cItem,0,sizeof(CItem));

				if( CreateItem(&cItem, lpItemStoreNative->wItemId,1 ) )
				{
					SetItem(ITEM_NATIVE_TRADERMONSTER_INV_LARGE, bySlotIndex, &cItem);
					bySlotIndex += 1;
				}
			}
		}
		else if(byStoreType == STORE_TYPE_SUSPZO)
		{
			if( GetItemType(pBaseItem->wID) == ITEM_TYPE_SMALL )
			{
				CItem cItem;
				memset(&cItem,0,sizeof(CItem));
				if( CreateItem(&cItem, lpItemStoreNative->wItemId,pBaseItem->GetMaxQuantity() ) )
				{
					SetItem(ITEM_NATIVE_TRADERMONSTER_INV_SMALL, bySlotIndex, &cItem);
					bySlotIndex += 1;
				}
			}	
		}											
						
		lpItemStoreNativeNode = lpItemStoreNativeNode->pNext;
	}	
}


void* CTraderMonster::operator new(size_t size)
{
	CTraderMonster* pNew = (CTraderMonster*)g_pTraderMonsterClassPool->Allocation();
	pNew->SetObjectType(OBJECT_TYPE_TRADER_MONSTER);
	pNew->SetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON, __SKILL_ATTACK__);
	return pNew;
}


void CTraderMonster::operator delete(void* pDeleteMonster)
{
	g_pTraderMonsterClassPool->FreeAllocation(pDeleteMonster);
}


const CItem* CTraderMonster::GetItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex) const
{
	assert(ITEM_NATIVE_TRADERMONSTER_START < eItemNative && eItemNative < ITEM_NATIVE_TRADERMONSTER_END);

	return (CItem*)m_pItemNativeManager->GetItem((BYTE)eItemNative, bySlotIndex);
}


void CTraderMonster::SetItem(enum ITEM_NATIVE eItemNative, BYTE bySlotIndex, const CItem* pItem)
{
	assert(ITEM_NATIVE_TRADERMONSTER_START < eItemNative && eItemNative < ITEM_NATIVE_TRADERMONSTER_END);
	
	m_pItemNativeManager->SetItem((BYTE)eItemNative, bySlotIndex, pItem);
}