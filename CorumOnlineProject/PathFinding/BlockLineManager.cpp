#include "BlockLineManager.h"
#include "math.h"

BlockLineManager::BlockLineManager()
{
	memset( this, 0, sizeof(BlockLineManager));
}

BlockLineManager::~BlockLineManager()
{
	if( m_piBlockOffset_Map )
		delete m_piBlockOffset_Map;
}

DWORD BlockLineManager::GetBlockLine( DWORD dwBlock_X, DWORD dwBlock_Y, BlockLocation* pBlockLocation)
{
	DWORD		dwX=0;
	DWORD		dwY=0;

	DWORD		dwPixcelX=0;
	DWORD		dwPixcelX_Max=0;
	DWORD		dwBlockPixcelSize=0;
	float		fA=0.f;
	float		fValue_Y=0.f;
	DWORD		dwCrossNum=0;
	DWORD		dwOffset=0;

	if( dwBlock_X == 0 )
	{
		pBlockLocation[0].dwBlock_X = 0;
		pBlockLocation[0].dwBlock_Y = 0;
		return 1;
	}

	dwBlockPixcelSize	= 10;
	fA					= (float) dwBlock_Y / (float) dwBlock_X;
	dwPixcelX_Max		= dwBlockPixcelSize * dwBlock_X;
	dwCrossNum			= 0;

	for( dwPixcelX = (dwBlockPixcelSize / 2); dwPixcelX <= dwPixcelX_Max; dwPixcelX += dwBlockPixcelSize)
	{
		fValue_Y		= (float) dwPixcelX * fA / (float) dwBlockPixcelSize + 0.5f;
		dwX				= dwPixcelX / dwBlockPixcelSize;
		dwY				= (DWORD) fValue_Y;
		
		if( (float)dwY == fValue_Y )
			dwCrossNum++;

		dwOffset = dwX + dwY - dwCrossNum;
		pBlockLocation[ dwOffset].dwBlock_X = dwX;
		pBlockLocation[ dwOffset].dwBlock_Y = dwY - ((DWORD)((float)dwY / fValue_Y));
		pBlockLocation[ dwOffset + 1].dwBlock_X = dwX + 1;
		pBlockLocation[ dwOffset + 1].dwBlock_Y = dwY;
	}

	return (dwX + dwY + 2 - dwCrossNum);
}

DWORD BlockLineManager::GetComparedBlock( BlockLocation* pBlockLocation_Out, BlockLocation* pBlockLocation_Src, DWORD dwBlockNum_Src, BlockLocation* pBlockLocation_Des, DWORD dwBlockNum_Des)
{
	DWORD		dwi;
	DWORD		dwj;
	DWORD		dwBlockNum_Compared;

	dwBlockNum_Compared = 0;

	for( dwi = 0; dwi < dwBlockNum_Des; dwi++ )
	{
		for( dwj = 0; dwj < dwBlockNum_Src; dwj++ )
			if(	( pBlockLocation_Des[ dwi].dwBlock_X == pBlockLocation_Src[ dwj].dwBlock_X ) && ( pBlockLocation_Des[ dwi].dwBlock_Y == pBlockLocation_Src[ dwj].dwBlock_Y ) )
				break;

		if( dwj < dwBlockNum_Src )
			continue;

		pBlockLocation_Out[ dwBlockNum_Compared].dwBlock_X = pBlockLocation_Des[ dwi].dwBlock_X;
		pBlockLocation_Out[ dwBlockNum_Compared].dwBlock_Y = pBlockLocation_Des[ dwi].dwBlock_Y;
		dwBlockNum_Compared++;
	}

	return dwBlockNum_Compared;
}

bool BlockLineManager::CreateBlockLineTable()
{
	DWORD		dwi;
	DWORD		dwX;
	DWORD		dwY;
	DWORD		dwMax_X;
	DWORD		dwMax_Y;

	DWORD		dwBlockNum_Src;
	DWORD		dwBlockNum_Des;
	DWORD		dwBlockNum_Compared;
	DWORD		dwPosition_BlockLocation_Stack;
	int			iGap_XY;
	
	BlockLineInfo*		pBlockLineInfo;
	int*				piBlockOffset_Map;

	BlockLocation*		Src;
	BlockLocation*		Des;
	BlockLocation*		BlockLocation_Stack;

	int					iLineSize;
	int					iSingleBlockSize;

	if( !m_bSetTableSize || !m_bSetMapInfo )
		return FALSE;
	
	Src = (BlockLocation*) new BYTE [ sizeof( BlockLocation) * ( MAX_BLOCK_NUM * 2 + MAX_STACK_SIZE )];
	if( !Src )
		return FALSE;
	Des					= Src + MAX_BLOCK_NUM;
	BlockLocation_Stack	= Des + MAX_BLOCK_NUM;

	CreateMemoryBlock( dwMax_X = m_BlockManager.GetMaxBlock_X(), dwMax_Y = m_BlockManager.GetMaxBlock_Y(), m_BlockManager.GetSingleBlockSize());
	m_pBlockLineInfo = (BlockLineInfo*) m_pvMemoryBlock;

	if( m_piBlockOffset_Map)
		delete [] m_piBlockOffset_Map;

	iGap_XY = dwMax_X - dwMax_Y;

	if( iGap_XY < 0 )
	{
		dwMax_X = dwMax_Y;
		dwMax_Y = m_BlockManager.GetMaxBlock_X();
	}

	dwPosition_BlockLocation_Stack = 0;

	for( dwY = 0; dwY < dwMax_Y; dwY++ )
	{
		for( dwX = dwY; dwX < dwMax_X; dwX++ )
		{
			dwBlockNum_Src							= GetBlockLine( dwX, dwY, Src);

			dwBlockNum_Des							= GetBlockLine( dwX + 1, dwY, Des);
			dwBlockNum_Compared						= GetComparedBlock( BlockLocation_Stack + dwPosition_BlockLocation_Stack, Src, dwBlockNum_Src, Des, dwBlockNum_Des );
       
			pBlockLineInfo							= (BlockLineInfo*) m_BlockManager.GetAddress( dwX, dwY);
			pBlockLineInfo->piBlockOffset_Map		= (int*) dwPosition_BlockLocation_Stack;
			pBlockLineInfo->dwMaxBlock				= dwBlockNum_Compared;
			pBlockLineInfo->fLineCost				= (float) sqrt( (double)((dwX+1) * (dwX+1) + dwY * dwY ));
			dwPosition_BlockLocation_Stack			+= dwBlockNum_Compared;

			dwBlockNum_Des							= GetBlockLine( dwX + 1, dwY + 1, Des);
			dwBlockNum_Compared						= GetComparedBlock( BlockLocation_Stack + dwPosition_BlockLocation_Stack, Src, dwBlockNum_Src, Des, dwBlockNum_Des );

			pBlockLineInfo++;
			pBlockLineInfo->piBlockOffset_Map		= (int*) dwPosition_BlockLocation_Stack;
			pBlockLineInfo->dwMaxBlock				= dwBlockNum_Compared;
			pBlockLineInfo->fLineCost				= (float) sqrt( (double)((dwX+1) * (dwX+1) + (dwY+1) * (dwY+1) ));
			dwPosition_BlockLocation_Stack			+= dwBlockNum_Compared;

			if( dwY != dwX)//좌상 to 우하 접어서 똑같이 만들려고 이짓 합니다. 
			{
				memcpy( m_BlockManager.GetAddress( dwY, dwX) , (pBlockLineInfo - 1), sizeof(BlockLineInfo) * 2);
			}
//		debug
			if( MAX_STACK_SIZE < dwPosition_BlockLocation_Stack )
				_asm int 3;
		}
	}

	//		메모리를 할당하고, 스택에 있던 데이터를 변환하여 이곳으로 옮긴다.
	m_piBlockOffset_Map = new int[ dwPosition_BlockLocation_Stack * CASE_4];
	iLineSize			=	m_BlockManager_Map.GetLineSize();
	iSingleBlockSize	=	m_BlockManager_Map.GetSingleBlockSize();

	//		방향 포멧 8방향 : [0] = 1_2,   [1] = 2_3,   [2] = 3_4,   [3] = 4_5.
	for( dwi = 0; dwi < dwPosition_BlockLocation_Stack; dwi++ )
	{
		dwX = BlockLocation_Stack[ dwi].dwBlock_X;
		dwY = BlockLocation_Stack[ dwi].dwBlock_Y;

		piBlockOffset_Map		= m_piBlockOffset_Map + dwi * CASE_4;
		piBlockOffset_Map[ 0]	= iLineSize * dwY + iSingleBlockSize * dwX;
		piBlockOffset_Map[ 1]	= iLineSize * dwX + iSingleBlockSize * dwY; 
		piBlockOffset_Map[ 2]	= iLineSize * dwX - iSingleBlockSize * dwY;
		piBlockOffset_Map[ 3]	= iLineSize * dwY - iSingleBlockSize * dwX;
	}

	//		멤버에 임시로 가지고 잇던 인덱스값을 실제 주소값으로 변환하는 루프.
	dwMax_X = m_BlockManager.GetMaxBlock_X();
	dwMax_Y = m_BlockManager.GetMaxBlock_Y();

	pBlockLineInfo = m_pBlockLineInfo;
	dwMax_X = dwMax_X * dwMax_Y * 2;

	for( dwX = 0; dwX < dwMax_X; dwX++)
	{
		pBlockLineInfo->piBlockOffset_Map = m_piBlockOffset_Map + ((int)(pBlockLineInfo->piBlockOffset_Map)) * CASE_4;
		pBlockLineInfo++;
	}
	
	delete [] Src;

	return TRUE;
}

bool BlockLineManager::CreateBlockLineTable(DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, BlockManager* pBlockManager)
{
	m_bSetTableSize = m_BlockManager.Initialize( dwMaxBlock_X, dwMaxBlock_Y, sizeof(BlockLineInfo) * 2, NULL);
	m_bSetMapInfo	= m_BlockManager_Map.Initialize(pBlockManager->GetMaxBlock_X(), pBlockManager->GetMaxBlock_Y(), pBlockManager->GetSingleBlockSize(), pBlockManager->GetMemoryBlockHead());
	CreateBlockLineTable();
	return TRUE;
}

bool BlockLineManager::SetTableSize(DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y)
{
	m_bSetTableSize = m_BlockManager.Initialize( dwMaxBlock_X, dwMaxBlock_Y, sizeof(BlockLineInfo), NULL);
	CreateBlockLineTable();
	return m_bSetTableSize;
}

bool BlockLineManager::SetMapInfo(BlockManager* pBlockManager)
{
	m_bSetMapInfo = m_BlockManager_Map.Initialize(pBlockManager->GetMaxBlock_X(), pBlockManager->GetMaxBlock_Y(), pBlockManager->GetSingleBlockSize(), pBlockManager->GetMemoryBlockHead());
	CreateBlockLineTable();
	return m_bSetMapInfo;
}

BlockLineInfo* BlockLineManager::GetBlockLineInfo( DWORD dwBlock_X, DWORD dwBlock_Y)
{
	BlockLineInfo*	pBlockLineInfo;
	pBlockLineInfo = (BlockLineInfo*) m_BlockManager.GetAddress( dwBlock_X, dwBlock_Y);
	return 	pBlockLineInfo;
}