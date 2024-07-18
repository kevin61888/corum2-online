#include "BlockManager.h"

BlockManager::BlockManager()
{}

BlockManager::~BlockManager()
{}

bool BlockManager::Initialize( DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, DWORD dwSingleBlockSize, PVOID pvMemoryBlock)
{
	m_pvMemoryBlock		= pvMemoryBlock;
	m_dwMaxBlock_X		= dwMaxBlock_X;
	m_dwMaxBlock_Y		= dwMaxBlock_Y;
	m_iSingleBlockSize	= (int)dwSingleBlockSize;
	m_iLineSize			= (int)m_dwMaxBlock_X * m_iSingleBlockSize;

	return TRUE;
}

bool BlockManager::Initialize( BlockManager* pBlockManager)
{
	m_pvMemoryBlock		= pBlockManager->GetMemoryBlockHead();
	m_dwMaxBlock_X		= pBlockManager->GetMaxBlock_X();
	m_dwMaxBlock_Y		= pBlockManager->GetMaxBlock_Y();
	m_iSingleBlockSize	= pBlockManager->GetSingleBlockSize();
	m_iLineSize			= pBlockManager->GetLineSize();

	return TRUE;
}