#pragma once

#include "../stdafx.h"


class BlockManager
{
private:
	PVOID				m_pvMemoryBlock;
	int					m_iSingleBlockSize;
	int					m_iLineSize;
	DWORD				m_dwMaxBlock_X;
	DWORD				m_dwMaxBlock_Y;

public:
	bool				Initialize( DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, DWORD dwSingleBlockSize, PVOID pvMemoryBlock);
	bool				Initialize( BlockManager* pBlockManager);
	int					GetOffset( DWORD dwX, DWORD dwY)		{ return ((int)( dwY * m_iLineSize + dwX * m_iSingleBlockSize)); }
	PVOID				GetAddress ( DWORD dwX, DWORD dwY)		
	{ return ((PVOID)((int)m_pvMemoryBlock + (int)( dwY * m_iLineSize + dwX * m_iSingleBlockSize))); }
	//{ return ((PVOID)((int)m_pvMemoryBlock + (int)( dwX * m_iLineSize + dwY * m_iSingleBlockSize))); }
	PVOID				GetMemoryBlockHead ()					{ return m_pvMemoryBlock;	}
	DWORD				GetMaxBlock_X()							{ return m_dwMaxBlock_X;	}
	DWORD				GetMaxBlock_Y()							{ return m_dwMaxBlock_Y;	}
	int					GetSingleBlockSize()					{ return m_iSingleBlockSize;}
	int					GetLineSize()							{ return m_iLineSize;		}

	BlockManager();
	~BlockManager();
};