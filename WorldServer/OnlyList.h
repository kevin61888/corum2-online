#ifndef __ONLY_LIST_H__
#define __ONLY_LIST_H__


#pragma once


#include "../4DyuchiGXGFunc/global.h"
#include "LinkedListEx.h"


class COnlyList : public CVoidList
{
	STMPOOL_HANDLE	m_pNodePool;

public:
	
	CNode*	NewNode(CNode* pPrev, CNode* pNext);
	void	FreeNode(CNode* pNode);

	COnlyList(DWORD dwMaxNodePoolNum);
	~COnlyList();
};


#endif 