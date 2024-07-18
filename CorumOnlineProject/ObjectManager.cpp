#include "stdafx.h"
#include "ObjectManager.h"
#include "InitGame.h"
#include "GameControl.h"
#include "CodeFun.h"

CObjectManager::CObjectManager(DWORD dwNum)
{
	memset(this, 0, sizeof(CObjectManager));
	m_dwMaxBucketNum = dwNum;
		
	m_pObjPool = CreateStaticMemoryPool();
	InitializeStaticMemoryPool(m_pObjPool, sizeof(OBJECT_HANDLE_DESC), MAX_OBJECT_CONTAINER_NUM, MAX_OBJECT_CONTAINER_NUM);


	m_ppInfoTable = NULL;
	m_ppInfoTable = new OBJECT_HANDLE_DESC*[ dwNum ];		
	memset(m_ppInfoTable, 0, sizeof(OBJECT_HANDLE_DESC*)*dwNum);

	m_dwNum = 0;
}

OBJECT_HANDLE_DESC*	CObjectManager::CreateObject(DWORD dwObjID, float x, float y, float z, BYTE bActionFlag, bool bShow, DWORD dwFlag)
{
	//OBJECT_HANDLE_DESC* pObj = (OBJECT_HANDLE_DESC*)m_pObjPool->Alloc();

	OBJECT_HANDLE_DESC* pObj = (OBJECT_HANDLE_DESC*)LALAlloc(m_pObjPool);
	pObj->dwObjID = dwObjID;
	
	//pObj->pDesc = (LPObjectDesc)g_pObjDescPool->Alloc();
	pObj->pDesc = AllocObjDesc();
	memset(pObj->pDesc, 0, sizeof(OBJECT_DESC));
	pObj->pDesc->bActionFlag = bActionFlag;

	pObj->pHandle = CreateHandleObject(FILE(dwObjID), GXPlayerPROC, pObj->pDesc, dwFlag);

	if(!pObj->pHandle)
		return NULL;


	VECTOR3 v3Pos;
	v3Pos.x = x;
	v3Pos.y = y;
	v3Pos.z = z;
	GXSetPosition(pObj->pHandle, &v3Pos, FALSE);
		
	if(!AddObject(pObj))
	{
		//g_pObjDescPool->Free((char*)pObj->pDesc);
		//m_pObjPool->Free((char*)pObj);
		FreeObjDesc( pObj->pDesc);
		pObj->pDesc = NULL;
		LALFree(m_pObjPool, pObj);
		return NULL;
	}

	return pObj;
}




BOOL CObjectManager::AddObject(OBJECT_HANDLE_DESC* pObj)
{
	if(!pObj)	return FALSE;

	DWORD dwIndex = pObj->dwObjID % m_dwMaxBucketNum;		
	m_dwNum++;

	if (!m_ppInfoTable[dwIndex])	
	{
		m_ppInfoTable[dwIndex] = pObj;
		pObj->pNext = NULL;
		pObj->pPrev = NULL;
		return TRUE;
	}

	OBJECT_HANDLE_DESC* cur = m_ppInfoTable[dwIndex];
	OBJECT_HANDLE_DESC* prv = NULL;
	while (cur)
	{
		prv = cur;
		cur = cur->pNext;
	}
	
	cur = prv->pNext = pObj;
	cur->pPrev = prv;
	cur->pNext = NULL;

	return TRUE;
}


OBJECT_HANDLE_DESC* CObjectManager::GetObject(DWORD dwObjID)
{
	DWORD dwIndex = dwObjID%m_dwMaxBucketNum;

	OBJECT_HANDLE_DESC* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->dwObjID == dwObjID)
		{
			return cur;
		}
		cur = cur->pNext;
	}
	return NULL;
}


void CObjectManager::RemoveObject(DWORD dwObjID)
{

	DWORD dwIndex = dwObjID%m_dwMaxBucketNum;

	OBJECT_HANDLE_DESC* cur = m_ppInfoTable[dwIndex];
	OBJECT_HANDLE_DESC* next = NULL;
	OBJECT_HANDLE_DESC* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrev;
		next = cur->pNext;
		if (cur->dwObjID == dwObjID)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNext = next;
			
			if (next)
				next->pPrev = prv;

			if(cur)	
			{
				DeleteHandleObject(cur->pHandle);
				FreeObjDesc( cur->pDesc);
				cur->pDesc = NULL;
				LALFree(m_pObjPool, cur);
			}
						
			cur = NULL;
			m_dwNum--;
			return;

		}
		cur = cur->pNext;
	}
	return;

}


void CObjectManager::RemoveObject(OBJECT_HANDLE_DESC* pInfo)
{
	if(!pInfo)	return;
	DWORD dwIndex = pInfo->dwObjID%m_dwMaxBucketNum;

	OBJECT_HANDLE_DESC* cur = m_ppInfoTable[dwIndex];
	OBJECT_HANDLE_DESC* next = NULL;
	OBJECT_HANDLE_DESC* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrev;
		next = cur->pNext;
		if (cur->dwObjID == pInfo->dwObjID)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNext = next;
			
			if (next)
				next->pPrev = prv;

			if(cur)	
			{
				DeleteHandleObject(cur->pHandle);
				//g_pObjDescPool->Free((char*)cur->pDesc);
				FreeObjDesc( cur->pDesc);
				cur->pDesc = NULL;
				//m_pObjPool->Free((char*)cur);
				LALFree(m_pObjPool, cur);
			}
						
			cur = NULL;
			m_dwNum--;
			return;

		}
		cur = cur->pNext;
	}
	return;
}



void CObjectManager::RemoveAllObject()
{
	OBJECT_HANDLE_DESC* cur = NULL;
	OBJECT_HANDLE_DESC* next = NULL;
	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];
		while (cur)
		{
			next = cur->pNext;
			DeleteHandleObject(cur->pHandle);
			FreeObjDesc( cur->pDesc);
			cur->pDesc = NULL;
			LALFree(m_pObjPool, cur);
			
			cur = next;
		}
		m_ppInfoTable[i] = NULL;
	}
}

char* CObjectManager::GetFile(DWORD dwObjID)
{
	return g_Res.GetResFileNameOnly(dwObjID);
}

CObjectManager::~CObjectManager()
{
	RemoveAllObject();

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}

	if(m_pObjPool)
	{
		ReleaseStaticMemoryPool(m_pObjPool);
		m_pObjPool = NULL;
	}
}
