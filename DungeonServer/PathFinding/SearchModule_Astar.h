#pragma once
#include "../stdafx.h"
//#include "ControlStruct.h"
#include "PriorityQue.h"
#include "BlockManager.h"
#include "DirectionManager.h"
#include "MemoryBlock.h"
#include "BlockLineManager.h"

const DWORD	OBSTACLE_BIT_MASK				=	0xF0;
const DWORD	MAX_BLOCK_LOCATION_NUM_RESULT	=	100;

const DWORD	MIN_SEARCH_RANGE_X				=	3;
const DWORD MIN_SEARCH_RANGE_Y				=	3;
const DWORD	MAX_SEARCH_RANGE_X				=	80;
const DWORD MAX_SEARCH_RANGE_Y				=	80;

struct Direct_Access
{
	DWORD*		pdwAddress_Map;
};

struct XY
{
	DWORD		dwX;
	DWORD		dwY;
};

class Sw;
typedef DWORD (Sw::*PFN_CASE_DIRECTION)( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );

class Sw
{
private:
	PriorityQue			m_PriorityQue;
	DirectionManager	m_DirectionManager;
	VirtualBlockManager m_VirtualBlockManager;
	BlockLineManager	m_BlockLineManager;

	PFN_CASE_DIRECTION	m_pfnCaseDirection[MAX_32_DIRECTION_NUM];

	XY					m_xySearchRange;
	BlockManager		m_BlockManager_Virtual;
	BlockManager		m_BlockManager_Map;

	VirtualBlock*		m_pVirtualBlock_Start;

	BlockLocation		m_BlockLocation[ MAX_BLOCK_LOCATION_NUM_RESULT];

	bool				m_bSetSearchRange;
	bool				m_bSetMapInfo;

private:
	void				Initialize_Default	();
	bool				Initialize			();
	DWORD				Direction_00_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_01_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_02_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_03_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_04_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_06_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_08_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_10_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_12_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_14_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_16_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_18_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_20_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_22_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_24_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_26_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_28_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
	DWORD				Direction_30_Func	( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin );
public:
	bool				Initialize			( DWORD dwMax_X_SearchRange, DWORD dwMax_Y_SearchRange, DWORD dwMax_X_Map, DWORD dwMax_Y_Map, DWORD dwSingleBlockSize_Map, PVOID pvMap);
	bool				SetSearchRange		( DWORD dwMax_X_SearchRange, DWORD dwMax_Y_SearchRange);
	bool				SetMapInfo			( DWORD dwMax_X, DWORD dwMax_Y, DWORD dwSingleBlockSize, PVOID pvMap);
	DWORD				FindShortestWay		( DWORD dwStart_X, DWORD dwStart_Y, DWORD dwTarget_X, DWORD dwTarget_Y, BlockLocation** ppBlockLocation_Out);

	Sw();
	~Sw();
};

extern Sw* g_pSw;