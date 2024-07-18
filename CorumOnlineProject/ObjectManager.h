#pragma once
//#include "GXDefault.h"
#include "ObjectDefine.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGXGFunc/global.h"
#else
#include "./../4DyuchiGXGFunc/global.h"
#endif

#define MAX_OBJECT_CONTAINER_NUM	200

class CObjectManager
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;		
	OBJECT_HANDLE_DESC**	m_ppInfoTable;

	STMPOOL_HANDLE			m_pObjPool;
	
	I4DyuchiGXExecutive*	m_pExecutive;
	
	BOOL					AddObject(OBJECT_HANDLE_DESC* pObj);
	
public:								  
	
	OBJECT_HANDLE_DESC*		CreateObject(DWORD dwObjID, float x, float y, float z, BYTE bActionFlag = ACTION_LOOP, bool bShow = true, DWORD dwFlag = 0);
	OBJECT_HANDLE_DESC*		GetObject(DWORD dwObjID);

	
	void					RemoveObject(DWORD dwObjID);
	void					RemoveObject(OBJECT_HANDLE_DESC* pInfo);
	void					RemoveAllObject();

	char*					GetFile(DWORD dwObjID);
	
	WORD					GetCount()	{	return (WORD)m_dwNum;	}
	
	
	CObjectManager(DWORD dwNum);
	~CObjectManager();
};

extern CObjectManager* g_pObjManager;