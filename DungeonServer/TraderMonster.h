// TraderMonster.h: interface for the CTraderMonster class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADERMONSTER_H__6D8EF2C5_C51E_4F4E_B5DC_FC390FB69D01__INCLUDED_)
#define AFX_TRADERMONSTER_H__6D8EF2C5_C51E_4F4E_B5DC_FC390FB69D01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Monster.h"
#include "../CommonServer/CommonHeader.h"
class CClassMemoryPool;
class CItemNativeManager;


#define MAX_TRADERMONSTER_INV_SMALL (7*9*4)
#define MAX_TRADERMONSTER_INV_LARGE (7*4*7)


#define	STORE_TYPE_SUSPZO				1
#define	STORE_TYPE_WEAPON				2
#define	STORE_TYPE_ARMOR				3


class CTraderMonster  : public CMonster
{
	CItemNativeManager* m_pItemNativeManager;

	CItem m_Inv_Small[MAX_TRADERMONSTER_INV_SMALL];
	CItem m_Inv_Large[MAX_TRADERMONSTER_INV_LARGE];

public:
	
	static void* operator new(size_t size);
	static void operator delete(void* pDeleteMonster);

	virtual void	RemoveResource();
	virtual void	CreateResource();
	void			CreateStoreItem();

	const CItem*	GetItem(enum ITEM_NATIVE eItemNative, BYTE bySlotIndex) const;
	void			SetItem(enum ITEM_NATIVE eItemNative, BYTE bySlotIndex, const CItem* pItem);
	
	CTraderMonster();
	virtual ~CTraderMonster();

};


extern CClassMemoryPool* g_pTraderMonsterClassPool;


#endif 
