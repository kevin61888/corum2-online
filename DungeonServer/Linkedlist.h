#pragma once


#include "VoidList.h"


class CNpcList : public CVoidList
{
public:
    void FreeData(void* pData); 
};

class CMonsterList : public CVoidList
{
public:
	~CMonsterList();
    void FreeData(void* pData); 	
};

class CItemList : public CVoidList
{
public:
	void FreeData(void* pData);
};

class CPcList : public CVoidList
{
	void FreeData(void* pData);
	void FreeNode(CNode*);
};

class CActiveSectionList : public CVoidList
{
	void FreeData(void* pData);
	void FreeNode(CNode*);
};

class CDungeonList : public CVoidList
{
	void FreeData(void* pData);
	void FreeNode(CNode*);
};

class CCreateItemList : public CVoidList
{
	void FreeData(void* pData);
	void FreeNode(CNode*);
};

