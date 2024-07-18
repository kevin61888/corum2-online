#pragma once

#include "../stdafx.h"
#include "MemoryBlock.h"
#include "ControlStruct.h"
#include "DirectionManager.h"


struct VirtualBlock
{
	//	S	Data
	Direction_32_Info*	pDirection_32_Info;		//	가장 주의해야하는 포인터. DirectionManager 이외의 사용을 금한다.

	//	R	Data
	DWORD				dwOffset_Map;			//	이 가상블럭에 대응하는 실제 맵블럭위치의 옵셋.
	DWORD				dwX_This;				//	이 가상블럭의 X 좌표값. 
	DWORD				dwY_This;				//	이 가상블럭의 Y 좌표값.

	//	R/W	Data
	DWORD				dwTarget;				//	목표 확인용.
	VirtualBlock*		pVirtualBlock_Origin;	//	직선이 유지되는 가장 상위의 블럭 포인터.
	VirtualBlock*		pVirtualBlock_Parent;	//	한단계 상위 블럭 포인터.
	float				fCostFromStart;			//	스타트블럭 위치로부터 현제블럭 위치까지 거쳐온 총 거리값.
	DWORD				dwCostTotal;			//	dwCostFromStart에 휴리스틱 Cost가 추가된 값.
};

class VirtualBlockManager : public MemoryBlock
{
private:
	VirtualBlock*		m_pVirtualBlock;
	BlockManager		m_BlockManager_Map;
	DWORD				m_dwCenter_X;
	DWORD				m_dwCenter_Y;
	
	bool				m_bSetVirtualBlockSize;
	bool				m_bSetOffset_Map;

private:
	bool				CreateVirtualBlock			();

public:
	bool				CreateVirtualBlock			( DWORD dwMaxBlock_X, DWORD dwMaxBlock_Y, BlockManager* pBlockManager_Map );	//	X , Y 값의 최대범의를 인자로 받아, 가상블럭을 구성할 메모리를 할당한다.
	bool				SetVirtualBlockSize			( DWORD dwMaxBlock_X, DWORD dwMaxBlcok_Y );
	bool				SetOffset_Map				( BlockManager* pBlockManager_Map);


	DWORD				GetCenter_X					()		{ return m_dwCenter_X; }			//	가상블럭의 중점 X값 리턴.
	DWORD				GetCenter_Y					()		{ return m_dwCenter_Y; }			//	가상블럭의 중점 Y값 리턴.

	VirtualBlockManager();
	~VirtualBlockManager();
};

