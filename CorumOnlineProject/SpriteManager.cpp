#include "stdafx.h"
#include "SpriteManager.h"
#include "InitGame.h"
#include "CodeFun.h"

CSpriteManager::CSpriteManager(DWORD dwNum)
{
	memset(this, 0, sizeof(CSpriteManager));
		
	m_dwMaxBucketNum	= dwNum;	
	m_pSpritePool		= CreateStaticMemoryPool();
	
	InitializeStaticMemoryPool(m_pSpritePool, sizeof(V2_SPRITE), MAX_SPRITE_NUM_PER_SCREEN, MAX_SPRITE_NUM_PER_SCREEN);

	m_pSpriteList		= new COnlyList(MAX_SPRITE_NUM_PER_SCREEN);	
	m_ppInfoTable		= NULL;
	m_ppInfoTable		= new V2_SPRITE*[ dwNum ];
	memset(m_ppInfoTable, 0, sizeof(V2_SPRITE*)*dwNum);
	m_dwNum				= 0;
	
}

V2_SPRITE*	CSpriteManager::CreateSprite(DWORD dwSprID, float x, float y, bool bShow, int nOrder, BYTE bAlpha)
{	
	V2_SPRITE* pSpr = (V2_SPRITE*)LALAlloc(m_pSpritePool);

	if(pSpr==NULL)
		MessageBox(g_hMainWnd, "SpritePool = NULL", "CorumOnline", MB_OK);

	memset(pSpr, 0, sizeof(V2_SPRITE));

	pSpr->dwSprID		= dwSprID;
	pSpr->bRenderFlag	= bShow;
	pSpr->nZOrder		= nOrder;
	pSpr->v2Scaling.x	= 1.0;
	pSpr->v2Scaling.y	= 1.0;
	pSpr->fRot			= 0.0f;

	DWORD dwAlpha = 0xffffffff;
	memcpy((BYTE*)(&dwAlpha)+3, &bAlpha, 1);
	pSpr->dwAlpha = dwAlpha;
	
//	char* pTemp = g_Res.GetResFileNameOnly(dwSprID);/

	char szDir[ _MAX_PATH ] = {0,};
	GetCurrentDirectory(_MAX_PATH, szDir);
	
	pSpr->pSpr = g_pRenderer->CreateSpriteObject(FILE(dwSprID), 0);
	
	if(!pSpr->pSpr)
	{
		char szInfo[0xff] = {0,};
		wsprintf(szInfo, "%d", dwSprID);
		MessageBox(g_hMainWnd, szInfo, "CorumOnlineProject", MB_OK);
		return NULL;
	}
		
	IMAGE_HEADER desc;
	pSpr->pSpr->GetImageHeader(&desc, 0);
		
	pSpr->dwWidth	= desc.dwWidth;
	pSpr->dwHeight	= desc.dwHeight;
	pSpr->vPos.x	= x;
	pSpr->vPos.y	= y;
	pSpr->vRelativePos.x=x;	// 윈도우가 움직일때 그에 속한 버튼들이 움직이기 위한 상대적인 위치값.
	pSpr->vRelativePos.y=y;	
	if(!AddSprite(pSpr))
	{
		char szInfo[0xff] = {0,};
		LALFree(m_pSpritePool, pSpr);
		wsprintf(szInfo, "AddSprtite Fail = %d", dwSprID);
		MessageBox(g_hMainWnd, szInfo, "CorumOnline", MB_OK);
		return NULL;
	}
	return pSpr;
}

V2_SPRITE*	CSpriteManager::CreateSprite(DWORD dwSprID, float x, float y, int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha)
{	
	V2_SPRITE*		pSpr	= (V2_SPRITE*)LALAlloc(m_pSpritePool);
	DWORD			dwAlpha = 0xffffffff;
	IMAGE_HEADER	desc;
		
	if(pSpr==NULL)
		MessageBox(g_hMainWnd, "SpritePool = NULL", "CorumOnline", MB_OK);
		
	memset(pSpr, 0, sizeof(V2_SPRITE));

	pSpr->dwSprID		= dwSprID;
	pSpr->bRenderFlag	= bShow;
	pSpr->nZOrder		= nOrder;
	pSpr->v2Scaling.x	= 1.0;
	pSpr->v2Scaling.y	= 1.0;
	pSpr->fRot			= 0.0f;
	pSpr->vRelativePos.x=x;	// 윈도우가 움직일때 그에 속한 버튼들이 움직이기 위한 상대적인 위치값.
	pSpr->vRelativePos.y=y;
	memcpy((BYTE*)(&dwAlpha)+3, &bAlpha, 1);
	pSpr->dwAlpha = dwAlpha;

//	char*	pTemp = g_Res.GetResFileNameOnly(dwSprID);
	char	szDir[ _MAX_PATH ] = {0,};
	GetCurrentDirectory(_MAX_PATH, szDir);
	
	char* pp = FILE(dwSprID);
	
	if(!__strcmp(pp,"psteam.tga"))
		asm_int3();
	
	pSpr->pSpr = g_pRenderer->CreateSpriteObject(FILE(dwSprID), nStartX, nStartY, nWidth, nHeight, 0);

	if(!pSpr->pSpr)
	{
		char szInfo[0xff] = {0,};
		wsprintf(szInfo, "%d", dwSprID);
		MessageBox(g_hMainWnd, szInfo, "CorumOnlineProject", MB_OK);		
		return NULL;
	}			
	pSpr->pSpr->GetImageHeader(&desc, 0);
			
	pSpr->dwWidth	= nWidth;
	pSpr->dwHeight	= nHeight;
	pSpr->vPos.x	= x;
	pSpr->vPos.y	= y;
		
	if(!AddSprite(pSpr))
	{		
		LALFree(m_pSpritePool, pSpr);

		char szInfo[0xff] = {0,};
		wsprintf(szInfo, "AddSprtite Fail = %d", dwSprID);
		MessageBox(g_hMainWnd, szInfo, "CorumOnlineProject", MB_OK);		
		return NULL;
	}
	return pSpr;
}


V2_SPRITE*	CSpriteManager::CreateSpriteFile(DWORD dwSprID, char* szFilePath, float x, float y, bool bShow, int nOrder, BYTE bAlpha)
{	
	V2_SPRITE*		pSpr	= (V2_SPRITE*)LALAlloc(m_pSpritePool);
	DWORD			dwAlpha = 0xffffffff;
	IMAGE_HEADER	desc;

	if(pSpr==NULL)
		MessageBox(g_hMainWnd, "SpritePool = NULL", "CorumOnline", MB_OK);

	memset(pSpr, 0, sizeof(V2_SPRITE));
	
	pSpr->dwSprID		= dwSprID;
	pSpr->bRenderFlag	= bShow;
	pSpr->nZOrder		= nOrder;
	pSpr->v2Scaling.x	= 1.0;
	pSpr->v2Scaling.y	= 1.0;
	pSpr->fRot			= 0.0f;
	
	memcpy((BYTE*)(&dwAlpha)+3, &bAlpha, 1);
	pSpr->dwAlpha		= dwAlpha;		
	pSpr->pSpr			= g_pRenderer->CreateSpriteObject(szFilePath, 0);
	
	if(!pSpr->pSpr)
	{
		char szInfo[0xff] = {0,};
		wsprintf(szInfo, "%d", dwSprID);
		MessageBox(g_hMainWnd, szInfo, "CorumOnlineProject", MB_OK);		
		return NULL;
	}	
	pSpr->pSpr->GetImageHeader(&desc, 0);

	pSpr->dwWidth	= desc.dwWidth;
	pSpr->dwHeight	= desc.dwHeight;
	pSpr->vPos.x	= x;
	pSpr->vPos.y	= y;
		
	if(!AddSprite(pSpr))
	{		
		LALFree(m_pSpritePool, pSpr);

		char szInfo[0xff] = {0,};
		wsprintf(szInfo, "AddSprite Fail = %d", dwSprID);
		MessageBox(g_hMainWnd, szInfo, "CorumOnlineProject", MB_OK);		
		return NULL;
	}
	return pSpr;
}

V2_SPRITE*	CSpriteManager::CreateSpriteFile(DWORD dwSprID, char* szFilePath, float x, float y,  int nStartX, int nStartY, int nWidth, int nHeight, bool bShow, int nOrder, BYTE bAlpha)
{
	V2_SPRITE*		pSpr	= (V2_SPRITE*)LALAlloc(m_pSpritePool);
	DWORD			dwAlpha = 0xffffffff;
	IMAGE_HEADER	desc;

	if(pSpr==NULL)
		MessageBox(g_hMainWnd, "SpritePool = NULL", "CorumOnline", MB_OK);

	memset(pSpr, 0, sizeof(V2_SPRITE));
	
	pSpr->dwSprID		= dwSprID;
	pSpr->bRenderFlag	= bShow;
	pSpr->nZOrder		= nOrder;
	pSpr->v2Scaling.x	= 1.0;
	pSpr->v2Scaling.y	= 1.0;
	pSpr->fRot			= 0.0f;
	
	memcpy((BYTE*)(&dwAlpha)+3, &bAlpha, 1);
	pSpr->dwAlpha		= dwAlpha;		
	pSpr->pSpr			= g_pRenderer->CreateSpriteObject(szFilePath, nStartX, nStartY, nWidth, nHeight, 0);
	
	if(!pSpr->pSpr)
	{
		char szInfo[0xff] = {0,};
		wsprintf(szInfo, "%d", dwSprID);
		MessageBox(g_hMainWnd, szInfo, "CorumOnlineProject", MB_OK);
		return NULL;
	}	
	pSpr->pSpr->GetImageHeader(&desc, 0);

	pSpr->dwWidth	= nWidth;
	pSpr->dwHeight	= nHeight;
	pSpr->vPos.x	= x;
	pSpr->vPos.y	= y;
		
	if(!AddSprite(pSpr))
	{		
		LALFree(m_pSpritePool, pSpr);

		char szInfo[0xff] = {0,};
		wsprintf(szInfo, "AddSprite  = %d", dwSprID);
		MessageBox(g_hMainWnd, szInfo, "CorumOnlineProject", MB_OK);		
		return NULL;
	}
	return pSpr;
}

V2_SPRITE*	CSpriteManager::CreateBackground(DWORD dwSprID, float x, float y)
{	
	m_pBackSprite = (V2_SPRITE*)LALAlloc(m_pSpritePool);

	if(m_pBackSprite==NULL)		
		MessageBox(g_hMainWnd, "SpritePool = NULL", "CorumOnline", MB_OK);

	memset(m_pBackSprite, 0, sizeof(V2_SPRITE));

	m_pBackSprite->dwSprID = dwSprID;
	m_pBackSprite->bRenderFlag = 1;
	m_pBackSprite->dwAlpha = 0xffffffff;
	
	m_pBackSprite->pSpr = g_pRenderer->CreateSpriteObject(FILE(dwSprID), 0);
	if(!m_pBackSprite->pSpr)	return NULL;

	IMAGE_HEADER desc;
	m_pBackSprite->pSpr->GetImageHeader(&desc, 0);

	m_pBackSprite->dwWidth = desc.dwWidth;
	m_pBackSprite->dwHeight = desc.dwHeight;
	m_pBackSprite->vPos.x = x;
	m_pBackSprite->vPos.y = y;
	
	return m_pBackSprite;
}


V2_SPRITE*	CSpriteManager::CreateBackground(char* szFilePath, float x, float y)
{
	//m_pBackSprite = (V2_SPRITE*)m_pSpritePool->Alloc();
	LALAlloc(m_pSpritePool);
	memset(m_pBackSprite, 0, sizeof(V2_SPRITE));

	m_pBackSprite->dwSprID = 0;			//배경 스프라이트 
	
	m_pBackSprite->pSpr = g_pRenderer->CreateSpriteObject(szFilePath, 0);
	if(!m_pBackSprite->pSpr)	return NULL;

	IMAGE_HEADER desc;
	m_pBackSprite->pSpr->GetImageHeader(&desc, 0);

	m_pBackSprite->dwWidth = desc.dwWidth;
	m_pBackSprite->dwHeight = desc.dwHeight;
	m_pBackSprite->vPos.x = x;
	m_pBackSprite->vPos.y = y;
	
	return m_pBackSprite;
}



BOOL CSpriteManager::AddSprite(V2_SPRITE* pSpr)
{
	if(!pSpr)	return FALSE;	
		
	if(pSpr->bRenderFlag)
		pSpr->ListPos = m_pSpriteList->AddTail(pSpr);
	else
		pSpr->ListPos = NULL;
	
	DWORD dwIndex = pSpr->dwSprID % m_dwMaxBucketNum;		
	m_dwNum++;

	if (!m_ppInfoTable[dwIndex])	
	{
		m_ppInfoTable[dwIndex] = pSpr;
		pSpr->pNext = NULL;
		pSpr->pPrev = NULL;
		return TRUE;
	}

	V2_SPRITE* cur = m_ppInfoTable[dwIndex];
	V2_SPRITE* prv = NULL;
	while (cur)
	{
		prv = cur;
		cur = cur->pNext;
	}
	
	cur = prv->pNext = pSpr;
	cur->pPrev = prv;
	cur->pNext = NULL;

	return TRUE;
}


V2_SPRITE* CSpriteManager::GetSprite(DWORD dwSprID)
{
	DWORD dwIndex = dwSprID%m_dwMaxBucketNum;

	V2_SPRITE* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->dwSprID == dwSprID)
		{
			return cur;
		}
		cur = cur->pNext;
	}
	return NULL;
}


void CSpriteManager::RemoveSprite(DWORD dwSprID)
{

	DWORD dwIndex = dwSprID%m_dwMaxBucketNum;

	V2_SPRITE* cur = m_ppInfoTable[dwIndex];
	V2_SPRITE* next = NULL;
	V2_SPRITE* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrev;
		next = cur->pNext;
		if (cur->dwSprID == dwSprID)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNext = next;
			
			if (next)
				next->pPrev = prv;

			if(cur)	
			{
				if(cur->ListPos)
				{
					m_pSpriteList->RemoveAt(cur->ListPos);
					cur->ListPos = NULL;
				}

				cur->pSpr->Release();
				LALFree(m_pSpritePool, cur);
			}
						
			cur = NULL;
			m_dwNum--;
			return;

		}
		cur = cur->pNext;
	}
	return;

}


void CSpriteManager::RemoveSprite(V2_SPRITE* pInfo)
{
	if(!pInfo)	return;
	DWORD dwIndex = pInfo->dwSprID%m_dwMaxBucketNum;

	if(pInfo->ListPos)
	{
		m_pSpriteList->RemoveAt(pInfo->ListPos);
		pInfo->ListPos = NULL;
	}
	
	V2_SPRITE* cur = m_ppInfoTable[dwIndex];
	V2_SPRITE* next = NULL;
	V2_SPRITE* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrev;
		next = cur->pNext;
		if (cur->dwSprID == pInfo->dwSprID)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNext = next;
			
			if (next)
				next->pPrev = prv;

			if(cur)	
			{
				cur->pSpr->Release();
				LALFree(m_pSpritePool, cur);
			}
						
			cur = NULL;
			m_dwNum--;
			return;

		}
		cur = cur->pNext;
	}
	return;
}

void CSpriteManager::RemoveBackground()
{
	if(m_pBackSprite)
	{
		m_pBackSprite->pSpr->Release();
		LALFree(m_pSpritePool, m_pBackSprite);
		m_pBackSprite = NULL;
	}
}


void CSpriteManager::RemoveAllSprite()
{
	m_pSpriteList->RemoveAll();
		
	V2_SPRITE* cur = NULL;
	V2_SPRITE* next = NULL;
	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];
		while (cur)
		{
			next = cur->pNext;
			cur->pSpr->Release();
			LALFree(m_pSpritePool, cur);
			cur = next;
		}
		m_ppInfoTable[i] = NULL;
	}
}

char* CSpriteManager::GetFile(DWORD dwSprID)
{
	return g_Res.GetResFileNameOnly(dwSprID);
}

void CSpriteManager::RenderAllSprite()
{
	POSITION_ pos = m_pSpriteList->GetHeadPosition();

	while(pos)
	{
		V2_SPRITE* pSpr = (V2_SPRITE*)m_pSpriteList->GetNext(pos);
		
		if(pSpr->v2Scaling.x==1.0f && pSpr->v2Scaling.y==1.0f)		
		{
			BOOL bChk = g_pRenderer->RenderSprite(pSpr->pSpr, NULL, pSpr->fRot, &pSpr->vPos, NULL, pSpr->dwAlpha, pSpr->nZOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);		

			if(!bChk)
				asm_int3();
		}
		else
		{
			g_pRenderer->RenderSprite(pSpr->pSpr, &pSpr->v2Scaling, pSpr->fRot, &pSpr->vPos, NULL, pSpr->dwAlpha, pSpr->nZOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);		
		}
	}
}


void CSpriteManager::RenderBackground()
{
	if(m_pBackSprite)
		g_pRenderer->RenderSprite(m_pBackSprite->pSpr, NULL, 0.0, &m_pBackSprite->vPos,NULL, 0xffffffff, m_pBackSprite->nZOrder, RENDER_TYPE_DISABLE_TEX_FILTERING);
}

void CSpriteManager::SetAlpha(DWORD dwSprID, BYTE bAlpha)
{
	DWORD dwAlpha = 0xffffffff;
	memcpy((BYTE*)(&dwAlpha)+3, &bAlpha, 1);
	GetSprite(dwSprID)->dwAlpha = dwAlpha;	
}


void CSpriteManager::AddRenderList(V2_SPRITE* pSprite)
{
	if(!pSprite)
	{
		MessageBox(g_hMainWnd, "Fail to add render list!", "Error", MB_OK);
		return;
	}

	pSprite->ListPos = m_pSpriteList->AddTail(pSprite);
}

void CSpriteManager::RemoveRenderList(V2_SPRITE* pSprite)
{
	if(!pSprite)
	{
		MessageBox(g_hMainWnd, "Fail to remove render list!", "Error", MB_OK);
		return;
	}
	
	m_pSpriteList->RemoveAt(pSprite->ListPos);
	pSprite->ListPos = NULL;
}




CSpriteManager::~CSpriteManager()
{
	RemoveBackground();
	RemoveAllSprite();

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}

	if(m_pSpritePool)
	{
		ReleaseStaticMemoryPool(m_pSpritePool);
		m_pSpritePool = NULL;
	}

	if(m_pSpriteList)
		delete m_pSpriteList;

}


void V2_SPRITE::ShowSprite(BOOL bShow)
{ 
	bRenderFlag = (BYTE)bShow; 

	if(bRenderFlag)
	{
		if(!ListPos)
			g_pSprManager->AddRenderList(this);
	}
	else
	{
		if(ListPos)
			g_pSprManager->RemoveRenderList(this);
	}
}	