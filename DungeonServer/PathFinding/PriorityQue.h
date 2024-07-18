#pragma once
#include "../stdafx.h"
#include "ControlStruct.h"
#include "VirtualBlockManager.h"

class PriorityQue
{
private:
	CLinker			m_Linker;
public:
	void			Initialize	( DWORD dwMaxItemNum);
	void			Clear		();
	void			Push		( VirtualBlock* pVirtualBlock);
	VirtualBlock*	Pop			();

	PriorityQue();
	~PriorityQue();
};