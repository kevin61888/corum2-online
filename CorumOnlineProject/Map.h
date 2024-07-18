//----------------------------------------------------------------------------------------------//
// Map Class
//----------------------------------------------------------------------------------------------//
#include "StdAfx.h"
#include "GameDefine.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_Common/typedef.h"
#else
#include "./../4DyuchiGRX_Common/typedef.h"
#endif
#pragma once

#define	TILE_SIZE		125.0f
#define TILE_SIZE_WORLD	80.0f
#define	TILE_HEIGHT	125.0f
#define	TILE_WIDTH	125.0f
//#define GRID_WIDTH		1500.0f		
//#define GRID_HEIGHT		1500.0f		

#define GRID_WIDTH		1250.0f		
#define GRID_HEIGHT		1250.0f		


#define TILE_X(X)	(float)( X * TILE_WIDTH + (TILE_WIDTH * 0.5f ) )
#define TILE_Z(Z)	(float)( Z * TILE_HEIGHT + (TILE_HEIGHT * 0.5f ) )

#define WORLDTILE_X(X)	(float)( X * TILE_SIZE_WORLD + (TILE_SIZE_WORLD * 0.5f ) )
#define WORLDTILE_Z(Z)	(float)( Z * TILE_SIZE_WORLD + (TILE_SIZE_WORLD * 0.5f ) )




#pragma pack( push, 1 )

#define MAX_SECTION_NUM_PER_LAYER		150
#define MAX_SECTION_LINK_NUM			20		//한섹션당 링크되는 섹션의 최대 갯수


class EffectDesc;
class CVoidList;
struct V2_SPRITE;
struct TILE_ATTR
{
	WORD uAttr		: 4;		//  0 ~ 15
	WORD uOccupied	: 4;		//	0 ~ 15
	WORD uSection	: 8;		//  0 ~ 255
};

struct MAP_TILE
{
	float	x;
	float	z;
	WORD	wIndex_X;	//배열 Index값 
	WORD	wIndex_Z;
	
//	void*	pOccupiedObj;	//점유하고 있는 Object Pointer
	void*	pData;

	TILE_ATTR wAttr;
};

struct Section_Link_Info
{
	WORD	wSectionNum;
	WORD	x1, y1, x2, y2;

	WORD	pwSectionLinkNum[ MAX_SECTION_LINK_NUM ];

	Section_Link_Info()
	{
		memset( this, 0, sizeof( Section_Link_Info ) );
	}
};
struct Move_Spot			// 케릭터가 특정 장소로 이동시 다른곳으로 이동시긴다. 위층 아래층..
{
	WORD	wStartSpotX;
	WORD	wStartSpotZ;
	RECT	rcMoveSpot;
	int		wDestLayerID;
	int		wDestLayerMoveSpotNum;		// 대상 레이어의 어느 곳으로 이동할것인가.
};
struct CP_DESC
{
	BYTE		bID;
	BOOL		bDestroy;
	WORD		wProperty;			// 중화되었을때 나타나는 현상번호.
	DWORD		dwStartTime;		// 유저가 cp에 서있기 시작한 시간.
	MAP_TILE*	pTile;
	EffectDesc* pEffectDesc;
	V2_SPRITE*	pSpr_Cp_GuageBar;
	V2_SPRITE*	pSpr_Cp_GuageBoard;
	V2_SPRITE*	pSpr_Minimap_Dot;
};
class CMap
{
	
protected:
	char				m_szMapName[ MAX_MAP_NAME_LENGTH ];		

public:
	float				m_fMapXLength;
	float				m_fMapZLength;
	DWORD				m_dwMapXTileMany;
	DWORD				m_dwMapZTileMany;

	float				m_fTileSize;
	
	float				m_fMiniMapSize;
	
	WORD				m_wTotalSectionMany;
	Section_Link_Info	m_Section[MAX_SECTION_NUM_PER_LAYER];
	
	MAP_TILE*			m_pTile;				
	CVoidList*			m_pCPList;	// cp 포인트 리스트
	
	BYTE				m_bCPRemainCount;		// 없애야 할 cp 갯수.
	BOOL				SetTileOccupied( DWORD dwX, DWORD dwZ, BYTE attr, LPVOID pType );

	MAP_TILE*			GetMap(DWORD dwX, DWORD dwZ);
	BOOL				SetMap(DWORD dwX, DWORD dwZ, MAP_TILE* pTile);
	MAP_TILE*			GetTile(float fx, float fz);
	
	CMap(DWORD dwWidth, DWORD dwHeight, float fTileSize);
	~CMap();

};


#pragma pack (pop)

extern CMap* g_pMap;