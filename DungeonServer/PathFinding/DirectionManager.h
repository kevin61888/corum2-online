#pragma once
#include "../stdafx.h"
#include "BlockManager.h"


const DWORD	NUM_PER_DIRECTION			=	4;
const DWORD	MAX_8_DIRECTION_NUM			=	8;
const DWORD MAX_32_DIRECTION_NUM		=	32;
const DWORD MAX_SCAN_DIRECTION_NUM		=	5;
const float COST_DIRECTION				=	1.0f;
const float COST_DIRECTION_DIAG			=	1.41421f;

struct Direction_8_Info
{
	DWORD		dwDirection_This;
	float		fCost_This;
	int			iBlockOffset_Virtual;
	int			iBlockOffset_Map;
};

struct Direction_32_Info
{
	Direction_32_Info**	ppDirection_32_Info;
	DWORD				dwDirection_32_This;
	Direction_8_Info*	pDirection_8_Info_Begin;
	DWORD				dwRepeatCounter;
};

class DirectionManager
{
private:
	Direction_8_Info		m_Direction_8_Info			[ MAX_8_DIRECTION_NUM * 3];
	Direction_32_Info		m_Direction_32_Info			[ MAX_32_DIRECTION_NUM + 1];
	Direction_32_Info*		m_pDirection_32_Info		[ MAX_32_DIRECTION_NUM][ MAX_SCAN_DIRECTION_NUM];
	Direction_32_Info*		m_pDirection_32_Info_Start	[ MAX_8_DIRECTION_NUM];

private:
	void						Initialize_Default	();

public:
	bool						Initialize					( BlockManager* pBlockManager_Virtual, BlockManager* pBlockManager_Map);		//	멤버배열들을 초기화한다.
	Direction_32_Info*	GetDirection_32_Info_Start			();
	Direction_32_Info*	GetDirection_32_Info				( DWORD dwDirectionIndex );
	Direction_32_Info*	GetDirection_32_Info_New			( Direction_32_Info* pDirection_32_Info, DWORD	dwCounter);

	DirectionManager();
	~DirectionManager();
};



/*
struct Direction_Info3
{
	Direction_Info2*	pDirection_Info2;
	DWORD				dwPFunc;
};
*/