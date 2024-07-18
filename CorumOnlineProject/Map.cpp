#include "Map.h"
#include "GameControl.h"
#include "message.h"
#include "Dungeon_Data_ex.h"
#include "InitGame.h"

CMap::CMap( DWORD dwWidth, DWORD dwHeight, float fTileSize )
{
	memset(this, 0, sizeof(CMap));
	m_pTile = new MAP_TILE[ dwWidth * dwHeight ];	

	//memset( m_pTile, 0xff, sizeof(MAP_TILE) * (dwWidth * dwHeight) );
	memset( m_pTile, 0x00, sizeof(MAP_TILE) * (dwWidth * dwHeight) );

	m_dwMapXTileMany	= dwWidth;
	m_dwMapZTileMany	= dwHeight;
	m_fTileSize			= fTileSize;
	m_fMapXLength		= (float)( m_dwMapXTileMany * m_fTileSize );
	m_fMapZLength		= (float)( m_dwMapZTileMany * m_fTileSize );
	m_pCPList			= new CVoidList;
	
	if(m_fMapXLength>m_fMapZLength)
		m_fMiniMapSize = m_fMapXLength;
	else
		m_fMiniMapSize = m_fMapZLength;
}

CMap::~CMap()
{
	if( m_pTile )
	{
		delete [] m_pTile;
		m_pTile = NULL;
	}

	if (m_pCPList)
	{
		POSITION_ pos = m_pCPList->GetHeadPosition();
		while(pos)
		{
			POSITION_ del  = pos;
			
			CP_DESC* pCPDesc =  (CP_DESC*)m_pCPList->GetNext(pos);
			g_pSprManager->RemoveSprite(pCPDesc->pSpr_Cp_GuageBar);
			g_pSprManager->RemoveSprite(pCPDesc->pSpr_Cp_GuageBoard);
			g_pSprManager->RemoveSprite(pCPDesc->pSpr_Minimap_Dot);
			delete pCPDesc;
			m_pCPList->RemoveAt(del);
		}

		delete m_pCPList;
	}

	m_pCPList = NULL;
}

MAP_TILE* CMap::GetMap(DWORD dwX, DWORD dwZ)
{
	if( dwX >= m_dwMapXTileMany || dwZ >= m_dwMapZTileMany )	
		return NULL;

	return ( m_pTile + (m_dwMapXTileMany * dwZ + dwX) );
}

BOOL CMap::SetMap(DWORD dwX, DWORD dwZ, MAP_TILE* pTile)
{
	if (NULL == pTile)
	{
		return FALSE;
	}

	if(dwX >= m_dwMapXTileMany || dwZ >= m_dwMapZTileMany)	
		return FALSE;
	
	memcpy( m_pTile + (m_dwMapXTileMany * dwZ + dwX), pTile, sizeof(MAP_TILE));  
	return TRUE;
}

BOOL CMap::SetTileOccupied( DWORD dwX, DWORD dwZ, BYTE attr, LPVOID pType )
{
	if (NULL == g_pThisDungeon)
	{
		return FALSE;;
	}

	if(dwX >= m_dwMapXTileMany || dwZ >= m_dwMapZTileMany) return FALSE;
	
	MAP_TILE* pTile = ( m_pTile + ( m_dwMapXTileMany * dwZ + dwX  ) );
	if( pType == (LPVOID)g_pMainPlayer )
	{
//		MessageBox(g_hMainWnd, g_Message[ETC_MESSAGE340].szMessage, g_Message[ETC_MESSAGE341].szMessage, 0);		// MSG_ID : 340 ; SetTileOccupied 메이플레이어와 똑같네.\n에러 발견시 개발자에게 알려주십시오.., 341 ; ErrNum : 006
		__asm nop;		//qq
	}

	if( pTile->wAttr.uOccupied == TILE_OCCUPIED_DONT_MOVE ) return FALSE;
	if( pTile->pData == pType )	goto lb_set;
	if( pTile->pData == NULL )	goto lb_set;
	
	return FALSE;

lb_set:
	// 세팅
	if( attr == TILE_EMPTY )
	{
		pTile->pData = NULL;
		pTile->wAttr.uOccupied = TILE_EMPTY;
	}
	else 
	{
		pTile->pData = pType;
		if( g_pThisDungeon->GetDungeonType() == DUNGEON_TYPE_VILLAGE && g_pMainPlayer->m_bCurLayer == 0 )
			return TRUE;
		
		pTile->wAttr.uOccupied = attr;
	}

	return TRUE;
}

MAP_TILE* CMap::GetTile(float fx, float fz)
{
	if(fx < 0 || fz < 0)	return NULL;

	DWORD x = (DWORD)( fx / m_fTileSize );
	DWORD z = (DWORD)( fz / m_fTileSize );
	
	return GetMap(x, z);
}

