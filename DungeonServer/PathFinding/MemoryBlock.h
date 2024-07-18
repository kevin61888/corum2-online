#pragma once

#include "../stdafx.h"
#include "BlockManager.h"

class MemoryBlock
{
protected:
	PVOID				m_pvMemoryBlock;
	BlockManager		m_BlockManager;

public:
	bool				CreateMemoryBlock		( DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, DWORD dwSingleBlockSize);
	BlockManager*		GetBlockManager			()		{ return &m_BlockManager;	}
	PVOID				GetMemoryBlockHead		()		{ return m_pvMemoryBlock;	}
	bool				ClearMemory				();
	bool				FreeMemoryBlock			();

	MemoryBlock();
	~MemoryBlock();
};