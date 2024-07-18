#ifndef __LINKED_LIST_EX_H__
#define __LINKED_LIST_EX_H__

#pragma once

#include "LinkedList.h"


class CServerList : public CVoidList
{
public:
	void FreeNode(CNode* pNode);
};

class COwnDugneonList : public CVoidList
{
public:
	void FreeNode(CNode* pNode);
};

class CDugneonRefreshList : public CVoidList
{
public:
	void FreeNode(CNode* pNode);
};

class CDugneonRefreshUserList : public CVoidList
{
public:
	void FreeNode(CNode* pNode);
};

class CPKUserList : public CVoidList
{
public:
	void FreeNode(CNode* pNode);
};


class CVoidDontDeleteList : public CVoidList
{
public:
	void FreeNode(CNode* pNode);
};

#endif 