#include "PriorityQue.h"

PriorityQue::PriorityQue()
{}

PriorityQue::~PriorityQue()
{}

void PriorityQue::Initialize( DWORD dwMaxItem)
{
	m_Linker.Initialize( dwMaxItem);
}

void PriorityQue::Clear()
{
	m_Linker.RemoveAllLink();
}

void PriorityQue::Push( VirtualBlock* pVirtualBlock_In)
{
	VirtualBlock*	pVirtualBlock;
	float			fCost_In;
	PVOID			pvLinkItemHandle;

	fCost_In = pVirtualBlock_In->fCostFromStart;

	m_Linker.SetUserPointerToItem( (PVOID*) &pVirtualBlock);
	pvLinkItemHandle = m_Linker.SetUserPointerToHeadItem();

	while( pvLinkItemHandle)
	{
		if( pVirtualBlock->fCostFromStart > fCost_In )
		{
			m_Linker.AddToLink_Prev( pVirtualBlock_In);
			return;
		}
		pvLinkItemHandle = m_Linker.SetUserPointerToNext();
	}
	m_Linker.AddToLink_Tail( pVirtualBlock_In);
}

VirtualBlock* PriorityQue::Pop()
{
	VirtualBlock*	pVirtualBlock;
	PVOID			pvLinkItemHandle;
	
	m_Linker.SetUserPointerToItem( (PVOID*) &pVirtualBlock);
	pvLinkItemHandle = m_Linker.SetUserPointerToHeadItem();

	if( !pvLinkItemHandle)
		return NULL;

	m_Linker.RemoveFromLink( pvLinkItemHandle);

	return pVirtualBlock;
}

