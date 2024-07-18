#include "SearchModule_Astar.h"
//#include "OutputManager.h"
//#include "MapManager.h"

Sw*		g_pSw = NULL;

Sw::Sw()
{
	Initialize_Default();
}

Sw::~Sw()
{}

void Sw::Initialize_Default()
{
	DWORD		dwi;

	PFN_CASE_DIRECTION pfnCaseDirection_Temp[]	= 	{	
													Sw::Direction_00_Func,
													Sw::Direction_02_Func,
													Sw::Direction_04_Func,
													Sw::Direction_06_Func,
													Sw::Direction_08_Func,
													Sw::Direction_10_Func,
													Sw::Direction_12_Func,
													Sw::Direction_14_Func,
													Sw::Direction_16_Func,
													Sw::Direction_18_Func,
													Sw::Direction_20_Func,
													Sw::Direction_22_Func,
													Sw::Direction_24_Func,
													Sw::Direction_26_Func,
													Sw::Direction_28_Func,
													Sw::Direction_30_Func,
													};
	//		m_pfnCaseDirection
	for( dwi = 0; dwi < MAX_32_DIRECTION_NUM; dwi += CASE_4 )
	{
		m_pfnCaseDirection[ dwi]		= pfnCaseDirection_Temp[ dwi / 2];
		m_pfnCaseDirection[ dwi + 1]	= Sw::Direction_01_Func;
		m_pfnCaseDirection[ dwi + 2]	= pfnCaseDirection_Temp[ dwi / 2 + 1];
		m_pfnCaseDirection[ dwi + 3]	= Sw::Direction_03_Func;
	}
}

bool Sw::Initialize()
{
	if( !m_bSetSearchRange || !m_bSetMapInfo )
		return FALSE;

	m_DirectionManager.Initialize( m_VirtualBlockManager.GetBlockManager(), &m_BlockManager_Map);

	//	시작 블럭값 세트.
	m_pVirtualBlock_Start = (VirtualBlock*) m_BlockManager_Virtual.GetAddress( m_xySearchRange.dwX , m_xySearchRange.dwY);
	m_pVirtualBlock_Start->pDirection_32_Info	= m_DirectionManager.GetDirection_32_Info_Start();
//	m_pVirtualBlock_Start->fCostFromStart		= 0.0f;
	m_pVirtualBlock_Start->pVirtualBlock_Origin = m_pVirtualBlock_Start;
	m_pVirtualBlock_Start->pVirtualBlock_Parent = m_pVirtualBlock_Start;

	return TRUE;
}

bool Sw::SetSearchRange( DWORD dwMax_X_SearchRange, DWORD dwMax_Y_SearchRange)
{
	DWORD				dwMax_X;
	DWORD				dwMax_Y;

	if( dwMax_X_SearchRange < MIN_SEARCH_RANGE_X || dwMax_Y_SearchRange < MIN_SEARCH_RANGE_Y || 
		dwMax_X_SearchRange > MAX_SEARCH_RANGE_X || dwMax_Y_SearchRange > MAX_SEARCH_RANGE_Y )
		return FALSE;

	m_xySearchRange.dwX = dwMax_X_SearchRange;
	m_xySearchRange.dwY = dwMax_Y_SearchRange;

	dwMax_X	= m_xySearchRange.dwX * 2 + 1;
	dwMax_Y = m_xySearchRange.dwY * 2 + 1;

	m_VirtualBlockManager.SetVirtualBlockSize	( dwMax_X, dwMax_Y);
	m_BlockManager_Virtual.Initialize			( m_VirtualBlockManager.GetBlockManager() );

	m_BlockLineManager.SetTableSize				( dwMax_X, dwMax_Y);
	m_PriorityQue.Initialize					( dwMax_X * dwMax_Y);

	m_bSetSearchRange = TRUE;
	Initialize();

	return TRUE;
}

bool Sw::SetMapInfo( DWORD dwMax_X, DWORD dwMax_Y, DWORD dwSingleBlockSize, PVOID pvMap)
{
	m_bSetMapInfo = m_BlockManager_Map.Initialize( dwMax_X, dwMax_Y, dwSingleBlockSize, pvMap);
	m_VirtualBlockManager.SetOffset_Map		( &m_BlockManager_Map);
	m_BlockLineManager.SetMapInfo			( &m_BlockManager_Map);
	Initialize();

	return m_bSetMapInfo;
}

bool Sw::Initialize( DWORD dwMax_X_SearchRange, DWORD dwMax_Y_SearchRange, DWORD dwMax_X_Map, DWORD dwMax_Y_Map, DWORD dwSingleBlockSize_Map, PVOID pvMap)
{
	DWORD				dwMax_X;
	DWORD				dwMax_Y;

	if( dwMax_X_SearchRange < MIN_SEARCH_RANGE_X || dwMax_Y_SearchRange < MIN_SEARCH_RANGE_Y || 
		dwMax_X_SearchRange > MAX_SEARCH_RANGE_X || dwMax_Y_SearchRange > MAX_SEARCH_RANGE_Y )
		return FALSE;

	if ( !m_BlockManager_Map.Initialize( dwMax_X_Map, dwMax_Y_Map, dwSingleBlockSize_Map, pvMap ) )
		return FALSE;

	m_xySearchRange.dwX = dwMax_X_SearchRange;
	m_xySearchRange.dwY = dwMax_Y_SearchRange;

	dwMax_X	= m_xySearchRange.dwX * 2 + 1;
	dwMax_Y = m_xySearchRange.dwY * 2 + 1;
	
	m_VirtualBlockManager.CreateVirtualBlock	( dwMax_X, dwMax_Y, &m_BlockManager_Map);
	m_BlockManager_Virtual.Initialize			( m_VirtualBlockManager.GetBlockManager());
	m_BlockLineManager.CreateBlockLineTable		( dwMax_X, dwMax_Y, &m_BlockManager_Map);
	m_PriorityQue.Initialize					( dwMax_X * dwMax_Y);

	m_bSetSearchRange = m_bSetMapInfo = TRUE;
	Initialize();

	/*
	DWORD wSX, wSY, wEX, wEY;
	BlockLocation *pBlock;
	for(wSX=1;wSX<=dwMax_X_Map;wSX++)
	{
		for(wSY=1;wSY<=dwMax_Y_Map;wSY++)
		{
			for(wEX=wSX+1;wEX <= min(wSX+5, dwMax_Y_Map);wEX++)
			{
				for(wEY=wSY+1;wEY <= min(wSY+5, dwMax_Y_Map);wEY++)
				{
					FindShortestWay(wSX, wSY, wEX, wEY, &pBlock);
				}
			}
		}
		Log( LOG_JUST_DISPLAY, "path... (%d/%d)", wSX, dwMax_X_Map);
	}
	*/

	return TRUE;
}

DWORD Sw::FindShortestWay ( DWORD dwStart_X, DWORD dwStart_Y, DWORD dwTarget_X, DWORD dwTarget_Y, BlockLocation** ppBlockLocation_Out)
{
//	MapInfo*			pMap;	
	
	DWORD				dwCounter;
	DWORD				dwCounter_Max;

	VirtualBlock*		pVirtualBlock_Parent;
	VirtualBlock*		pVirtualBlock_Child;

	Direction_32_Info*	pDirection_32_Info;
	Direction_32_Info*	pDirection_32_Info_New;
	Direction_8_Info*	pDirection_8_Info;

	DWORD				dwMapAdr_Start;
	DWORD				dwMapAdr_Origin;

	VirtualBlock*		pVirtualBlock_Target;

	DWORD				dwStart_X_Map; 
	DWORD				dwStart_Y_Map; 

	BlockLocation*		pBlockLocation;
	DWORD				dwCounter_Result;

	DWORD*				pdwMapAdr_Target;
//	DWORD				dwMapInfo_Target;

	// for optimization
	register int a, b;
	register int iA, iB;

//	Check Range.
	if( (int)dwStart_X - (int)dwTarget_X > (int)m_xySearchRange.dwX || (int)dwStart_X - (int)dwTarget_X < -(int)m_xySearchRange.dwX )
		return 0;
	if( (int)dwStart_Y - (int)dwTarget_Y > (int)m_xySearchRange.dwY || (int)dwStart_Y - (int)dwTarget_Y < -(int)m_xySearchRange.dwY )
		return 0;

	dwCounter_Result = 0;

	dwCounter		= 0;
	dwCounter_Max	= m_BlockManager_Virtual.GetMaxBlock_X() * m_BlockManager_Virtual.GetMaxBlock_Y();
	pVirtualBlock_Parent = (VirtualBlock*) m_BlockManager_Virtual.GetAddress( 0,0);


#define O_CMD_1\
	pVirtualBlock_Parent->fCostFromStart = 1000.0f;\
	pVirtualBlock_Parent++
	
	iA = dwCounter_Max/8;
	iB = dwCounter_Max%8;

	for(a=0;a<iA;a++)
	{
		O_CMD_1;O_CMD_1;O_CMD_1;O_CMD_1;
		O_CMD_1;O_CMD_1;O_CMD_1;O_CMD_1;
	}
	for(b=0;b<iB;b++)
	{
		O_CMD_1;
	}
/*
	for ( ; dwCounter <  dwCounter_Max; dwCounter++ ,pVirtualBlock_Parent++ )
		pVirtualBlock_Parent->fCostFromStart = 1000.0f;
*/	

//	Main ---------------------------

	dwStart_X_Map	= dwStart_X - m_xySearchRange.dwX;
	dwStart_Y_Map	= dwStart_Y - m_xySearchRange.dwY;
	dwMapAdr_Start	= (DWORD) m_BlockManager_Map.GetAddress( dwStart_X_Map, dwStart_Y_Map);
	
	pVirtualBlock_Target = (VirtualBlock*) m_BlockManager_Virtual.GetAddress( (dwTarget_X - dwStart_X_Map), (dwTarget_Y - dwStart_Y_Map) );

	if( pVirtualBlock_Target->dwOffset_Map == NULL )	//	030709 rain
		return 0;

//	Target Exception
	pdwMapAdr_Target = (DWORD*)(dwMapAdr_Start + pVirtualBlock_Target->dwOffset_Map);
	if( (*pdwMapAdr_Target) & OBSTACLE_BIT_MASK )
	{
		//	Set Round Target
		pDirection_32_Info	= m_pVirtualBlock_Start->pDirection_32_Info;
		pDirection_8_Info	= pDirection_32_Info->pDirection_8_Info_Begin;
		dwCounter			= 0;	
		dwCounter_Max		= pDirection_32_Info->dwRepeatCounter;

 		for(; dwCounter < dwCounter_Max; dwCounter++, pDirection_8_Info++ )
		{
			pVirtualBlock_Child = (VirtualBlock*)((int)pVirtualBlock_Target + pDirection_8_Info->iBlockOffset_Virtual);
			pVirtualBlock_Child->dwTarget = 1;
		}
	}
	else
	{
		pdwMapAdr_Target = NULL;
		pVirtualBlock_Target->dwTarget = 1;
	}


//
//	g_pOm->SetWayMarkObject( m_xySearchRange.dwX, m_xySearchRange.dwY,1);


	m_pVirtualBlock_Start->fCostFromStart = 0.0f;
	m_PriorityQue.Push( m_pVirtualBlock_Start );
  
	for( ;; )	//KUK
	{
		pVirtualBlock_Parent = m_PriorityQue.Pop();
		if( !pVirtualBlock_Parent )
			break;
//		g_pOm->SetWayMarkObject( pVirtualBlock_Parent->dwX_This, pVirtualBlock_Parent->dwY_This, 1);
/*
		if( pVirtualBlock_Parent == pVirtualBlock_Target )
		{
			while( pVirtualBlock_Parent != m_pVirtualBlock_Start )
			{
				g_pOm->SetWayMarkObject( dwStart_X_Map + pVirtualBlock_Parent->dwX_This, dwStart_Y_Map + pVirtualBlock_Parent->dwY_This, 3);
				pVirtualBlock_Parent = pVirtualBlock_Parent->pVirtualBlock_Origin;
			} 
			g_pOm->SetWayMarkObject( dwStart_X_Map + pVirtualBlock_Parent->dwX_This, dwStart_Y_Map + pVirtualBlock_Parent->dwY_This, 3);
			goto Lb_End;
		}
*/

		// end of finding : duk9 2005.1.4
		if( pVirtualBlock_Parent->dwTarget )
		{
			pBlockLocation = m_BlockLocation + ( MAX_BLOCK_LOCATION_NUM_RESULT);

//			pVirtualBlock_Parent = pVirtualBlock_Parent->pVirtualBlock_Origin;		//	<--
			while( pVirtualBlock_Parent != m_pVirtualBlock_Start )
			{
				pBlockLocation--;
				pBlockLocation->dwBlock_X = dwStart_X_Map + pVirtualBlock_Parent->dwX_This;
				pBlockLocation->dwBlock_Y = dwStart_Y_Map + pVirtualBlock_Parent->dwY_This;
				pVirtualBlock_Parent = pVirtualBlock_Parent->pVirtualBlock_Origin;
				dwCounter_Result++;
			}

			*ppBlockLocation_Out = pBlockLocation;

			goto Lb_End;
		}
		
		dwMapAdr_Origin		= dwMapAdr_Start + pVirtualBlock_Parent->pVirtualBlock_Origin->dwOffset_Map;

		pDirection_32_Info	= pVirtualBlock_Parent->pDirection_32_Info;
		pDirection_8_Info	= pDirection_32_Info->pDirection_8_Info_Begin;

		dwCounter			= 0;		
		dwCounter_Max		= pDirection_32_Info->dwRepeatCounter;

 		for(; dwCounter < dwCounter_Max; dwCounter++, pDirection_8_Info++ )
		{
			pVirtualBlock_Child = (VirtualBlock*)((int)pVirtualBlock_Parent + pDirection_8_Info->iBlockOffset_Virtual);
//	Debug	pMap	=	(MapInfo*) (dwMapAdr_Start + pVirtualBlock_Child->dwOffset_Map);
			if( !pVirtualBlock_Child->dwOffset_Map )
				continue;
			if( (*((DWORD*)(dwMapAdr_Start + pVirtualBlock_Child->dwOffset_Map))) & OBSTACLE_BIT_MASK )
				continue;
//			g_pOm->SetWayMarkObject( pVirtualBlock_Child->dwX_This, pVirtualBlock_Child->dwY_This, 2);
			pDirection_32_Info_New = m_DirectionManager.GetDirection_32_Info_New( pDirection_32_Info, dwCounter);
			if ( (this->*(m_pfnCaseDirection[pDirection_32_Info_New->dwDirection_32_This]))( pVirtualBlock_Parent, pVirtualBlock_Child, pDirection_8_Info, dwMapAdr_Origin) )
				pVirtualBlock_Child->pDirection_32_Info = pDirection_32_Info_New;
		}
	}
//
Lb_End:
	m_PriorityQue.Clear();

	//	Target Exception
	if( pdwMapAdr_Target )
	{
		//	Set Round Target
		pDirection_32_Info	= m_pVirtualBlock_Start->pDirection_32_Info;
		pDirection_8_Info	= pDirection_32_Info->pDirection_8_Info_Begin;
		dwCounter			= 0;		
		dwCounter_Max		= pDirection_32_Info->dwRepeatCounter;

#define O_CMD_3\
	pVirtualBlock_Child = (VirtualBlock*)((int)pVirtualBlock_Target + pDirection_8_Info->iBlockOffset_Virtual);\
	pVirtualBlock_Child->dwTarget = 0;\
	pDirection_8_Info++

		iA = dwCounter_Max/8;
		iB = dwCounter_Max%8;

		for(a=0;a<iA;a++)
		{
			O_CMD_3;O_CMD_3;O_CMD_3;O_CMD_3;
			O_CMD_3;O_CMD_3;O_CMD_3;O_CMD_3;
		}
		for(b=0;b<iB;b++)
		{
			O_CMD_3;
		}
/*
 		for(; dwCounter < dwCounter_Max; dwCounter++, pDirection_8_Info++ )
		{
			pVirtualBlock_Child = (VirtualBlock*)((int)pVirtualBlock_Target + pDirection_8_Info->iBlockOffset_Virtual);
			pVirtualBlock_Child->dwTarget = 0;
		}
*/
	}
	else
		pVirtualBlock_Target->dwTarget = 0;


	return dwCounter_Result;
}

//	Direction_Function List
//	Direction_32_Info : 1, 5, 9, 13, 17, 21, 25, 29
DWORD Sw::Direction_01_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	float fCost_New;

	fCost_New = pVirtualBlock_Parent->fCostFromStart + pDirection_8_Info->fCost_This;
	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 3, 7, 11, 15, 19, 23, 27, 31
DWORD Sw::Direction_03_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	float fCost_New;

	fCost_New = pVirtualBlock_Parent->fCostFromStart + pDirection_8_Info->fCost_This;
	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Parent;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 0
DWORD Sw::Direction_00_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Parent->dwX_This - pVirtualBlock_Origin->dwX_This;
	dwY_Gap					= pVirtualBlock_Origin->dwY_This - pVirtualBlock_Parent->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap) + 1;

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin - piBlockOffset_Map[ 2]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 2
DWORD Sw::Direction_02_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Parent->dwX_This - pVirtualBlock_Origin->dwX_This;
	dwY_Gap					= pVirtualBlock_Origin->dwY_This - pVirtualBlock_Parent->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap) + 1;

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin - piBlockOffset_Map[ 3]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 4
DWORD Sw::Direction_04_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Parent->dwX_This - pVirtualBlock_Origin->dwX_This;
	dwY_Gap					= pVirtualBlock_Origin->dwY_This - pVirtualBlock_Parent->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap);

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin - piBlockOffset_Map[ 3]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 6		
DWORD Sw::Direction_06_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Parent->dwX_This - pVirtualBlock_Origin->dwX_This;
	dwY_Gap					= pVirtualBlock_Parent->dwY_This - pVirtualBlock_Origin->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap);

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin + piBlockOffset_Map[ 0]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 8
DWORD Sw::Direction_08_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Parent->dwX_This - pVirtualBlock_Origin->dwX_This;
	dwY_Gap					= pVirtualBlock_Parent->dwY_This - pVirtualBlock_Origin->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap) + 1;

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;


	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin + piBlockOffset_Map[ 0]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 10
DWORD Sw::Direction_10_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Parent->dwX_This - pVirtualBlock_Origin->dwX_This;
	dwY_Gap					= pVirtualBlock_Parent->dwY_This - pVirtualBlock_Origin->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap) + 1;

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;


	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin + piBlockOffset_Map[ 1]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 12
DWORD Sw::Direction_12_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Parent->dwX_This - pVirtualBlock_Origin->dwX_This;
	dwY_Gap					= pVirtualBlock_Parent->dwY_This - pVirtualBlock_Origin->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap);

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin + piBlockOffset_Map[ 1]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 14
DWORD Sw::Direction_14_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Origin->dwX_This - pVirtualBlock_Parent->dwX_This;
	dwY_Gap					= pVirtualBlock_Parent->dwY_This - pVirtualBlock_Origin->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap);

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin + piBlockOffset_Map[ 2]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 16
DWORD Sw::Direction_16_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Origin->dwX_This - pVirtualBlock_Parent->dwX_This;
	dwY_Gap					= pVirtualBlock_Parent->dwY_This - pVirtualBlock_Origin->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap) + 1;

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;



	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin + piBlockOffset_Map[ 2]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 18
DWORD Sw::Direction_18_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Origin->dwX_This - pVirtualBlock_Parent->dwX_This;
	dwY_Gap					= pVirtualBlock_Parent->dwY_This - pVirtualBlock_Origin->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap) + 1;

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;


	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin + piBlockOffset_Map[ 3]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 20
DWORD Sw::Direction_20_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Origin->dwX_This - pVirtualBlock_Parent->dwX_This;
	dwY_Gap					= pVirtualBlock_Parent->dwY_This - pVirtualBlock_Origin->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap);

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin + piBlockOffset_Map[ 3]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 22
DWORD Sw::Direction_22_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Origin->dwX_This - pVirtualBlock_Parent->dwX_This;
	dwY_Gap					= pVirtualBlock_Origin->dwY_This - pVirtualBlock_Parent->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap);

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin - piBlockOffset_Map[ 0]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 24
DWORD Sw::Direction_24_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Origin->dwX_This - pVirtualBlock_Parent->dwX_This;
	dwY_Gap					= pVirtualBlock_Origin->dwY_This - pVirtualBlock_Parent->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap) + 1;

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;


	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin - piBlockOffset_Map[ 0]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart		= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 26
DWORD Sw::Direction_26_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Origin->dwX_This - pVirtualBlock_Parent->dwX_This;
	dwY_Gap					= pVirtualBlock_Origin->dwY_This - pVirtualBlock_Parent->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap) + 1;

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin - piBlockOffset_Map[ 1]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 28
DWORD Sw::Direction_28_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Origin->dwX_This - pVirtualBlock_Parent->dwX_This;
	dwY_Gap					= pVirtualBlock_Origin->dwY_This - pVirtualBlock_Parent->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap);

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin - piBlockOffset_Map[ 1]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart		= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
//	Direction_Function List
//	Direction_32_Info : 30
DWORD Sw::Direction_30_Func ( VirtualBlock* pVirtualBlock_Parent, VirtualBlock* pVirtualBlock_Child, Direction_8_Info* pDirection_8_Info, DWORD dwMapAdr_Origin )
{
	DWORD				dwX_Gap;
	DWORD				dwY_Gap;
	VirtualBlock*		pVirtualBlock_Origin;
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;
	int*				piBlockOffset_MaX_Map;
	float				fCost_New;

	pVirtualBlock_Origin	= pVirtualBlock_Parent->pVirtualBlock_Origin;
	dwX_Gap					= pVirtualBlock_Parent->dwX_This - pVirtualBlock_Origin->dwX_This;
	dwY_Gap					= pVirtualBlock_Origin->dwY_This - pVirtualBlock_Parent->dwY_This;
	pBlockLineInfo			= m_BlockLineManager.GetBlockLineInfo( dwX_Gap, dwY_Gap);

	fCost_New				= pBlockLineInfo->fLineCost;
	piBlockOffset_Map		= pBlockLineInfo->piBlockOffset_Map;
	piBlockOffset_MaX_Map	= piBlockOffset_Map + pBlockLineInfo->dwMaxBlock * CASE_4;

	for(; piBlockOffset_Map < piBlockOffset_MaX_Map; piBlockOffset_Map += CASE_4 )
		if( (*((DWORD*)(dwMapAdr_Origin - piBlockOffset_Map[ 2]))) & OBSTACLE_BIT_MASK )
		{
			fCost_New				= pDirection_8_Info->fCost_This;
			pVirtualBlock_Origin	= pVirtualBlock_Parent;
			goto Lb_Loop_Exit;
		}

Lb_Loop_Exit:

	fCost_New += pVirtualBlock_Origin->fCostFromStart;

	if( fCost_New >= pVirtualBlock_Child->fCostFromStart )
		return 0;

	pVirtualBlock_Child->fCostFromStart			= fCost_New;
	pVirtualBlock_Child->pVirtualBlock_Parent	= pVirtualBlock_Parent;
	pVirtualBlock_Child->pVirtualBlock_Origin	= pVirtualBlock_Origin;

	m_PriorityQue.Push( pVirtualBlock_Child);

	return 1;
}
