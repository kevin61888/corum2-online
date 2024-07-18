//======================================================//
//														//
//======================================================//
#pragma once
//#include "GXDefault.h"
#include "OnlyList.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGXGFunc/global.h"
#else
#include "./../4DyuchiGXGFunc/global.h"
#endif

interface IDISpriteObject;
#define	MAX_SPRITE_NUM_PER_SCREEN	2000

struct V2_SPRITE
{
	DWORD				dwSprID;

	IDISpriteObject*	pSpr;
	VECTOR2				vPos;
	VECTOR2				vRelativePos;

	VECTOR2				v2Scaling;
	DWORD				dwWidth;
	DWORD				dwHeight;
	DWORD				dwAlpha;
	float				fRot;
	POSITION_			ListPos;
	BYTE				bRenderFlag;		
	int					nZOrder;
	
	V2_SPRITE*			pPrev;
	V2_SPRITE*			pNext;

	BOOL PtInSpr(LONG x, LONG y)
	{		
		if(bRenderFlag && x >= vPos.x && x <= vPos.x + dwWidth*v2Scaling.x
			&& y >= vPos.y && y <= vPos.y + dwHeight*v2Scaling.y)
			return TRUE;
		else
			return FALSE;		
	};

	void ShowSprite(BOOL bShow);			

	void SetAlpha(BYTE bAlpha)
	{
		DWORD alpha = 0xffffffff;
		memcpy(((BYTE*)&alpha)+3, &bAlpha, 1);
		dwAlpha = alpha;
	}

	void SetScaling(float x, float y)	{ v2Scaling.x = x; v2Scaling.y = y;	}
};


class CSpriteManager
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;		
	V2_SPRITE**				m_ppInfoTable;

	STMPOOL_HANDLE			m_pSpritePool;
	
	V2_SPRITE*				m_pBackSprite;	

	char					m_szTempBuffer[ _MAX_PATH ];

	BOOL					AddSprite(V2_SPRITE* pSpr);
	
public:								  
	COnlyList*				m_pSpriteList;

	POSITION_				GetSpriteListHeadPos()	{	return m_pSpriteList->GetHeadPosition();	}
	V2_SPRITE*				GetNextSprite(POSITION_& rPosition)		{	return (V2_SPRITE*)m_pSpriteList->GetNext(rPosition);	}	

	V2_SPRITE*				CreateSprite(DWORD dwSprID, float x, float y, bool bShow = TRUE, 
												int nOrder = 0, BYTE bAlpha = 0xff);	
	
	V2_SPRITE*				CreateSpriteFile(DWORD dwSprID, char* szFilePath, float x, float y, 
												bool bShow = TRUE, int nOrder = 0, BYTE bAlpha = 0xff);	
	
	V2_SPRITE*				CreateSpriteFile(DWORD dwSprID, char* szFilePath, float x, float y, 
												int nStartX, int nStartY, int nWidth, int nHeight, bool bShow = TRUE, int nOrder = 0, BYTE bAlpha = 0xff);	
	
	V2_SPRITE*				CreateSprite(DWORD dwSprID, float x, float y, int nStartX, int nStartY, 
								int nWidth, int nHeight, bool bShow = TRUE, int nOrder = 0, BYTE bAlpha = 0xff);			

	V2_SPRITE*				CreateBackground(DWORD dwSprID, float x = 0, float y = 0);
	V2_SPRITE*				CreateBackground(char* szFilePath, float x = 0, float y = 0);
	V2_SPRITE*				GetSprite(DWORD dwSprID);
	
	STMPOOL_HANDLE			GetSpritePool()	{	return m_pSpritePool;	}		//021105
	
	void					RenderAllSprite();
	void					RenderBackground();
	void					RemoveSprite(DWORD dwSprID);
	void					RemoveSprite(V2_SPRITE* pInfo);
	void					RemoveBackground();
	void					RemoveAllSprite();
	void					SetAlpha(DWORD dwSprID, BYTE bAlpha);

	void					AddRenderList(V2_SPRITE* pSprite);
	void					RemoveRenderList(V2_SPRITE* pSprite);

	char*					GetFile(char* szFileName, BYTE bDataType);	
	char*					GetFile(DWORD dwSprID);
	DWORD					GetCount()	{	return m_dwNum;	}
	
	
	CSpriteManager(DWORD dwNum);
	~CSpriteManager();
};
//======================================================//
//														//
//======================================================//
extern CSpriteManager*	g_pSprManager;