#include "stdafx.h"
#include "LinkedList.h"
#include "GameSystem.h"


void CPcList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
	LALFree( g_pNodePool, (void*)pNode );
    m_nCount--;
}

void CActiveSectionList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
	LALFree( g_pNodePool, (void*)pNode );
    m_nCount--;
}


void CDungeonList::FreeNode(CNode* pNode)
{
    if (pNode == NULL) return;
	LALFree( g_pNodePool, (void*)pNode );
    m_nCount--;
}

void CCreateItemList::FreeNode( CNode* pNode )
{
	if( pNode == NULL ) return;
	LALFree( g_pNodePool, (void*)pNode );
	m_nCount--;
}


CMonsterList::~CMonsterList()
{
	RemoveAll();
}

void CMonsterList::FreeData(void* pData) 
{
    if (pData == NULL) 
		return;
}

void CPcList::FreeData(void* pData)
{
    if (pData == NULL)
		return;	
}

void CActiveSectionList::FreeData(void* pData) 
{
    if (pData == NULL) 
		return;
}

void CDungeonList::FreeData(void* pData) 
{
    if (pData == NULL) 
		return;
}

void CCreateItemList::FreeData( void* pData )
{
	if( pData == NULL )
		return;
}



