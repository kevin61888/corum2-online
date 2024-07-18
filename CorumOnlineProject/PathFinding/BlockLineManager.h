#pragma once

#include "../stdafx.h"
#include "MemoryBlock.h"

const DWORD	CASE_4					=	4;
const DWORD	MAX_STACK_SIZE			=	500000;
const DWORD MAX_BLOCK_NUM			=	10000;

struct BlockLocation
{
	DWORD		dwBlock_X;
	DWORD		dwBlock_Y;
};

struct BlockLineInfo
{
	float				fLineCost;
	DWORD				dwMaxBlock;
	int*				piBlockOffset_Map;
};

class BlockLineManager : public MemoryBlock
{
private:
	BlockManager		m_BlockManager_Map;

	BlockLineInfo*		m_pBlockLineInfo;
	int*				m_piBlockOffset_Map;

	bool				m_bSetTableSize;
	bool				m_bSetMapInfo;

private:
	DWORD				GetBlockLine( DWORD dwBlock_X, DWORD dwBlock_Y, BlockLocation* pBlockLocation);
	DWORD				GetComparedBlock( BlockLocation* pBlockLocation_Out, BlockLocation* pBlockLocation_Src, DWORD dwBlockNum_Src, BlockLocation* pBlockLocation_Des, DWORD dwBlockNum_Des);
	bool				CreateBlockLineTable();

public:
	bool				CreateBlockLineTable(DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, BlockManager* pBlockManager);
	bool				SetTableSize(DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y);
	bool				SetMapInfo(BlockManager* pBlockManager);
	BlockLineInfo*		GetBlockLineInfo( DWORD dwBlock_X, DWORD dwBlock_Y);

	BlockLineManager();
	~BlockLineManager();
};