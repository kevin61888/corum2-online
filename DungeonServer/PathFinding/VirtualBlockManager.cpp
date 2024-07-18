#include "VirtualBlockManager.h"


VirtualBlockManager::VirtualBlockManager()
{
	memset( this, 0, sizeof(VirtualBlockManager));
}

VirtualBlockManager::~VirtualBlockManager()
{}

bool VirtualBlockManager::CreateVirtualBlock()
{
	DWORD			dwX;
	DWORD			dwY;
	DWORD			dwMax_X;
	DWORD			dwMax_Y;

	VirtualBlock*	pVirtualBlock;

	if( !m_bSetVirtualBlockSize || !m_bSetOffset_Map )
		return FALSE;

	CreateMemoryBlock( m_BlockManager.GetMaxBlock_X(), m_BlockManager.GetMaxBlock_Y(), m_BlockManager.GetSingleBlockSize());
	m_pVirtualBlock = (VirtualBlock*) m_BlockManager.GetMemoryBlockHead();

	dwMax_X			= m_BlockManager.GetMaxBlock_X();
	dwMax_Y			= m_BlockManager.GetMaxBlock_Y();
	
	//		가상블럭의  맵옵셋 세트.
	//					X 값 세트.
	//					Y 값 세트.
	for( dwX = 0; dwX < dwMax_X; dwX++)
		for( dwY = 0; dwY < dwMax_Y; dwY++)
		{
			pVirtualBlock = (VirtualBlock*)m_BlockManager.GetAddress( dwX, dwY);
			pVirtualBlock->dwOffset_Map = m_BlockManager_Map.GetOffset( dwX, dwY);
			pVirtualBlock->dwX_This		= dwX;
			pVirtualBlock->dwY_This		= dwY;
		}

	//		가장자리에 위치한 가상블럭의 맵옵셋이 NULL을 가리키도록 세팅하는 루프.
	for( dwX = 0; dwX < dwMax_X; dwX++)
	{
		((VirtualBlock*)m_BlockManager.GetAddress( dwX, 0))->dwOffset_Map			= NULL;
		((VirtualBlock*)m_BlockManager.GetAddress( dwX, dwMax_Y - 1))->dwOffset_Map	= NULL;
	}
	for( dwY = 0; dwY < dwMax_Y; dwY++)
	{
		((VirtualBlock*)m_BlockManager.GetAddress( 0, dwY))->dwOffset_Map			= NULL;
		((VirtualBlock*)m_BlockManager.GetAddress( dwMax_X - 1, dwY))->dwOffset_Map	= NULL;
	}

	return TRUE;
}

bool VirtualBlockManager::CreateVirtualBlock( DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, BlockManager* pBlockManager_Map)
{
	m_bSetVirtualBlockSize	= m_BlockManager.Initialize( dwMaxBlock_X, dwMaxBlock_Y, sizeof( VirtualBlock), NULL);
	m_bSetOffset_Map		= m_BlockManager_Map.Initialize( pBlockManager_Map->GetMaxBlock_X(), pBlockManager_Map->GetMaxBlock_Y(), pBlockManager_Map->GetSingleBlockSize(), NULL);
	CreateVirtualBlock();
	return TRUE;
}

bool VirtualBlockManager::SetVirtualBlockSize( DWORD dwMaxBlock_X, DWORD dwMaxBlcok_Y )
{
	m_bSetVirtualBlockSize = m_BlockManager.Initialize( dwMaxBlock_X, dwMaxBlcok_Y, sizeof( VirtualBlock), NULL);
	CreateVirtualBlock();
	return m_bSetVirtualBlockSize;
}

bool VirtualBlockManager::SetOffset_Map( BlockManager* pBlockManager_Map)
{
	m_bSetOffset_Map = m_BlockManager_Map.Initialize( pBlockManager_Map->GetMaxBlock_X(), pBlockManager_Map->GetMaxBlock_Y(), pBlockManager_Map->GetSingleBlockSize(), pBlockManager_Map->GetMemoryBlockHead());
	CreateVirtualBlock();
	return m_bSetOffset_Map;
}

