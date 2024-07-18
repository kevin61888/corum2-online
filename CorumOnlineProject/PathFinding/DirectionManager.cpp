#include "DirectionManager.h"

//		Note :	시작점은 1시, 그리고 시계방향으로 증가.

DirectionManager::DirectionManager()
{
	memset( this, 0, sizeof( DirectionManager));
	Initialize_Default();
}

DirectionManager::~DirectionManager()
{}

void DirectionManager::Initialize_Default()
{
	DWORD				dwi;
	DWORD				dwj;
	DWORD				dwbDiag;
	DWORD				dwDirection;
	Direction_32_Info**	ppDirection_32_Info;

	//		Set  m_Direction_8_Info.

	//		dwDirection_This
	//		fCost_This
	for( dwi = 0; dwi < MAX_8_DIRECTION_NUM; dwi++)
	{
		m_Direction_8_Info[dwi].dwDirection_This	= dwi;
		m_Direction_8_Info[dwi].fCost_This			= ( (dwi % 2) == 0 ) ? COST_DIRECTION_DIAG : COST_DIRECTION ;
	}
	
	//	메모리 카피는 나중에 Initialize(...) 에서 한다.
	//	memcpy( m_Direction_8_Info + MAX_8_DIRECTION_NUM, m_Direction_8_Info, MAX_8_DIRECTION_NUM * sizeof( Direction_8_Info));
	//	memcpy( m_Direction_8_Info + MAX_8_DIRECTION_NUM * 2, m_Direction_8_Info, MAX_8_DIRECTION_NUM * sizeof( Direction_8_Info));

	//		Set  m_Direction_32_Info.

	//		pDirection_32_Info
	//		dwDirection_32_This
	//		pDirection_8_Info_Begin
	//		dwRepeatCounter
	for( dwi = 0; dwi < MAX_32_DIRECTION_NUM; dwi++)
	{
		dwDirection	= dwi / NUM_PER_DIRECTION;
		dwbDiag		= dwDirection % 2;

		m_Direction_32_Info[ dwi].ppDirection_32_Info		= *(m_pDirection_32_Info + dwi);		//	- qudtls
		m_Direction_32_Info[ dwi].dwDirection_32_This		= dwi;	
		m_Direction_32_Info[ dwi].pDirection_8_Info_Begin	= m_Direction_8_Info + MAX_8_DIRECTION_NUM + dwDirection + dwbDiag - 2;
		m_Direction_32_Info[ dwi].dwRepeatCounter			= 5 - dwbDiag * 2;
	}
	//		Set  Extra m_Direction_32_Info.  This Direction - 32.
	m_Direction_32_Info[ MAX_32_DIRECTION_NUM].ppDirection_32_Info		= m_pDirection_32_Info_Start;
	m_Direction_32_Info[ MAX_32_DIRECTION_NUM].dwDirection_32_This		= 33;
	m_Direction_32_Info[ MAX_32_DIRECTION_NUM].pDirection_8_Info_Begin	= m_Direction_8_Info + MAX_8_DIRECTION_NUM;
	m_Direction_32_Info[ MAX_32_DIRECTION_NUM].dwRepeatCounter			= MAX_8_DIRECTION_NUM;


	//		Set  m_pDirection_32_Info.  This is Direction Transition Table.
	for( dwi = 0; dwi < MAX_32_DIRECTION_NUM; dwi += NUM_PER_DIRECTION)
	{
		dwbDiag	= ( dwi / NUM_PER_DIRECTION ) % 2;
		for( dwj = 0; dwj < NUM_PER_DIRECTION; dwj++)
		{
			ppDirection_32_Info = *(m_pDirection_32_Info + dwi + dwj) - dwbDiag;
			ppDirection_32_Info[ 1] = m_Direction_32_Info + (((MAX_32_DIRECTION_NUM + dwi) - NUM_PER_DIRECTION) % MAX_32_DIRECTION_NUM) + (( dwj == 2 ) ?  3 : 2);
			ppDirection_32_Info[ 2] = m_Direction_32_Info + (( dwj == 3 ) ? ( dwi + 1 ) :  ( dwi + dwj));
			ppDirection_32_Info[ 3] = m_Direction_32_Info + ((dwi + NUM_PER_DIRECTION) % MAX_32_DIRECTION_NUM) + (( dwj == 0 ) ?  3 : 0);
			
			if( dwbDiag == 0 )
			{
				ppDirection_32_Info[ 0] = m_Direction_32_Info + ((MAX_32_DIRECTION_NUM + dwi - (NUM_PER_DIRECTION * 2)) % MAX_32_DIRECTION_NUM + 3);
				ppDirection_32_Info[ 4] = m_Direction_32_Info + ((dwi + (NUM_PER_DIRECTION * 2)) % MAX_32_DIRECTION_NUM + 3); 
			}
		}
	}
	//		Set  Extra - m_pDirection_32_Info_Start. This is For First Block.
	for( dwi = 0; dwi < MAX_8_DIRECTION_NUM; dwi++ )
		m_pDirection_32_Info_Start[ dwi] = m_Direction_32_Info + ( dwi * 4 + 3 );		// <-
}

bool DirectionManager::Initialize( BlockManager* pBlockManager_Virtual, BlockManager* pBlockManager_Map)
{
	int			iLineSize;
	int			iSingleBlockSize;

	//		Set  m_Direction_8_Info.

	//		iBlockOffset_Map
	iLineSize			= pBlockManager_Virtual->GetLineSize();
	iSingleBlockSize	= pBlockManager_Virtual->GetSingleBlockSize();

	m_Direction_8_Info[0].iBlockOffset_Virtual = -iLineSize	+	iSingleBlockSize;
	m_Direction_8_Info[1].iBlockOffset_Virtual =					iSingleBlockSize;
	m_Direction_8_Info[2].iBlockOffset_Virtual =  iLineSize	+	iSingleBlockSize;
	m_Direction_8_Info[3].iBlockOffset_Virtual =  iLineSize;
	m_Direction_8_Info[4].iBlockOffset_Virtual =  iLineSize	-	iSingleBlockSize;
	m_Direction_8_Info[5].iBlockOffset_Virtual =				-	iSingleBlockSize;
	m_Direction_8_Info[6].iBlockOffset_Virtual = -iLineSize	-	iSingleBlockSize;
	m_Direction_8_Info[7].iBlockOffset_Virtual = -iLineSize;

	//		iBlockOffset_Virtual
	iLineSize			= pBlockManager_Map->GetLineSize();
	iSingleBlockSize	= pBlockManager_Map->GetSingleBlockSize();

	m_Direction_8_Info[0].iBlockOffset_Map = -iLineSize	+	iSingleBlockSize;
	m_Direction_8_Info[1].iBlockOffset_Map =					iSingleBlockSize;
	m_Direction_8_Info[2].iBlockOffset_Map =  iLineSize	+	iSingleBlockSize;
	m_Direction_8_Info[3].iBlockOffset_Map =  iLineSize;
	m_Direction_8_Info[4].iBlockOffset_Map =  iLineSize	-	iSingleBlockSize;
	m_Direction_8_Info[5].iBlockOffset_Map =				-	iSingleBlockSize;
	m_Direction_8_Info[6].iBlockOffset_Map = -iLineSize	-	iSingleBlockSize;
	m_Direction_8_Info[7].iBlockOffset_Map = -iLineSize;

	memcpy( m_Direction_8_Info + MAX_8_DIRECTION_NUM, m_Direction_8_Info, MAX_8_DIRECTION_NUM * sizeof( Direction_8_Info));
	memcpy( m_Direction_8_Info + MAX_8_DIRECTION_NUM * 2, m_Direction_8_Info, MAX_8_DIRECTION_NUM * sizeof( Direction_8_Info));

	return TRUE;
}

Direction_32_Info* DirectionManager::GetDirection_32_Info_Start()
{
	return ( m_Direction_32_Info + MAX_32_DIRECTION_NUM );
}

Direction_32_Info* DirectionManager::GetDirection_32_Info( DWORD dwDirectionIndex)
{
	return ( m_Direction_32_Info + dwDirectionIndex);
}

Direction_32_Info* DirectionManager::GetDirection_32_Info_New( Direction_32_Info* pDirection_32_Info, DWORD dwCounter)
{
	return ( *(pDirection_32_Info->ppDirection_32_Info + dwCounter));
}
