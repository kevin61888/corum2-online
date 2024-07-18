#include "MemoryBlock.h"


MemoryBlock::MemoryBlock()
{
	m_pvMemoryBlock	= NULL;
}

MemoryBlock::~MemoryBlock()
{
	if ( m_pvMemoryBlock)
		delete [] m_pvMemoryBlock;
	m_pvMemoryBlock = NULL;
}

bool MemoryBlock::CreateMemoryBlock( DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, DWORD dwSingleBlockSize)
{
	if( m_pvMemoryBlock)
		delete [] m_pvMemoryBlock;

	m_pvMemoryBlock = new PVOID [dwMaxBlock_X * dwMaxBlock_Y * dwSingleBlockSize];
	
	if( !m_pvMemoryBlock)
		return FALSE;
	
	ClearMemory();
	m_BlockManager.Initialize( dwMaxBlock_X, dwMaxBlock_Y, dwSingleBlockSize, m_pvMemoryBlock);

	return TRUE;
}

bool MemoryBlock::FreeMemoryBlock()
{
	if( !m_pvMemoryBlock)
		return FALSE;

	delete [] m_pvMemoryBlock;
	m_pvMemoryBlock = NULL;

	return TRUE;
}

bool MemoryBlock::ClearMemory()
{
	memset( m_pvMemoryBlock, 0, m_BlockManager.GetMaxBlock_Y() * m_BlockManager.GetLineSize()); 

	return TRUE;
}
