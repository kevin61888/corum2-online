#include "stdafx.h"
#include "Map.h"
#include "Linkedlist.h"
#include "DungeonLayer.h"
#include "Dungeon.h"
#include "DUNGEON_DATA_EX.h"
#include "OwnServer.h"
#include "Section.h"

CMap::CMap()
{
	memset(this, 0, sizeof(CMap));	
}

CMap::~CMap()
{

}


//-----------------------------------------------------------------------------
// 맵 파일로 부터 맵(타일)을 생성한다. ※ 주의 : 일반 던전서버일 경우에만 실행되어야 한다. 초보자 던전서버일 경우 리 Load해놓은 Data를 Copy하여 초기화 한다. 
//-----------------------------------------------------------------------------
void CMap::Create(  DWORD dwID, CDungeonLayer* pParent)
{
	m_pCPList		= new CVoidList;
	m_pParentLayer	= pParent;

	ReadMap(dwID);

	if (!m_pParentLayer->GetParent()->GetDungeonDataEx()->IsConquer())
	{
		m_bCPRemainCount = 0;
	}
}


//-----------------------------------------------------------------------------
// 맵을 해제 한다. ※ 주의 : 일반 던전서버일 경우에만 실행되어야 한다. 초보자 던전서버일 경우 메모리풀로 부터 메모리를 해제한다. 
//-----------------------------------------------------------------------------
void CMap::Destroy()
{
	delete [] m_pTile;
	m_pTile = NULL;

	if (m_pCPList)
	{
		POSITION_ pos = m_pCPList->GetHeadPosition();

		while(pos)
		{
			POSITION_ del  = pos;
			delete (CP_DESC*)m_pCPList->GetNext(pos);
			m_pCPList->RemoveAt(del);
		}

		delete m_pCPList;
	}

	m_pCPList = NULL;
}


//-----------------------------------------------------------------------------
// 속성 파일을 읽어드린다. 
//-----------------------------------------------------------------------------
BOOL CMap::ReadMap(DWORD dwID)
{
	//여기서 파일 및 섹션 읽어 들이는 작업 ...
	HANDLE	hFile = NULL;
	DWORD dwRead = 0,	dwTotalReadByte=0; 
	BOOL bResult = 0;

	char szFile[ _MAX_PATH ]={0,};
	memset(szFile, 0, sizeof(szFile));
	wsprintf(szFile, "%s\\%d.ttb", g_pThis->GetServerDataPath(), dwID);

#ifdef JAPAN_LOCALIZING		//	added by minjin. for Necessary.. 2004. 10. 31.
	Log (LOG_NORMAL, "Load Map File : %s", szFile );
#endif
	
	hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		Log(LOG_IMPORTANT,"Fail to Open TTB File!(%d.ttb)", dwID );
		return FALSE;
	}
	
	DWORD dwTileSize=0, dwObjNum=0;
			
	//Read XTileMany				
	bResult = ReadFile(hFile, &m_dwTileNumWidth, sizeof(m_dwTileNumWidth), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(m_dwTileNumWidth))	__asm int 3
		
	//Read ZTileMany				
	bResult = ReadFile(hFile, &m_dwTileNumHeight, sizeof(m_dwTileNumHeight), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(m_dwTileNumHeight))	__asm int 3

	//Read TileSize
	bResult = ReadFile(hFile, &dwTileSize, sizeof(dwTileSize), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(dwTileSize))	__asm int 3
		
	//Read ObjectNum
	bResult = ReadFile(hFile, &dwObjNum, sizeof(dwObjNum), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(dwObjNum))	__asm int 3
	

	//Tile Memory Allocation
	m_pTile = new MAP_TILE[ m_dwTileNumWidth * m_dwTileNumHeight ];
	memset( m_pTile, 0x0, sizeof(MAP_TILE) * (m_dwTileNumWidth * m_dwTileNumHeight) );
	
	//Read TileAttribute
	MAP_TILE*	pTile=0;
	for(DWORD i=0; i< (m_dwTileNumWidth * m_dwTileNumHeight); i++)
	{
		pTile = m_pTile + i;

		bResult = ReadFile(hFile, &pTile->wAttr, 2, &dwRead, NULL);
		if(!bResult || dwRead != 2)	__asm int 3
		
		//여기서 타일(pTile) X, Y Index값및 기타 정보 채워넣어라!
		if( pTile->wAttr.uAttr == 1 )	//벽 
		{	
			pTile->wAttr.uOccupied = TILE_OCCUPIED_DONT_MOVE;
		}
		else if( pTile->wAttr.uAttr == 9 )	//cp 위치
		{
			// 점령 던전만 필요 한데 지금은 전부다 처리 하게 되있다..
			CP_DESC* pData = new CP_DESC;
			memset(pData, 0, sizeof(CP_DESC));
			m_pCPList->AddTail(pData);
			pData->bID = BYTE(m_pCPList->GetCount());
			pData->pTile = pTile;
		}
		else if (pTile->wAttr.uAttr == 10)
		{
			m_pGuardianTile = pTile;
		}
	}

	m_bCPRemainCount = BYTE(m_pCPList->GetCount());

	//Read SectionMany, 총 섹션 갯수 								
	WORD	wTotalSectionNum=0;
	bResult = ReadFile(hFile, &wTotalSectionNum, sizeof(wTotalSectionNum), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(wTotalSectionNum))	__asm int 3

	if(m_pParentLayer)
		m_pParentLayer->SetTotalSectionNum(wTotalSectionNum);
	Section_Link_Info sSectionLinkInfo;
	
	//Read Section Information
	for(i=0; i<wTotalSectionNum; i++)
	{
		dwTotalReadByte = 0;

		CSection* pSection = new CSection;

		//Read SectionMany								
		bResult = ReadFile(hFile, &sSectionLinkInfo, sizeof(Section_Link_Info), &dwRead, NULL);
		// 저장할때 section갯수는 byte로 해야할것을...쩝.
		pSection->m_bSectionNum = (BYTE)sSectionLinkInfo.wSectionNum;
		memcpy(pSection->m_wLink, sSectionLinkInfo.pwSectionLinkNum, sizeof(pSection->m_wLink));
		memcpy(&pSection->x1, &sSectionLinkInfo.x1, sizeof(pSection->x1)*4);
		
		if(!bResult || dwRead != sizeof(Section_Link_Info))	__asm int 3
		
		if(m_pParentLayer)
			m_pParentLayer->SetSection(i+1, pSection);
	}
	
	// 이부분 이제 지워야하지만 아래에 영향을 끼쳐 일단 놔둔다.
	WORD	wTotalMonsertInfoNum;
	bResult = ReadFile(hFile, &wTotalMonsertInfoNum, sizeof(wTotalMonsertInfoNum), &dwRead, NULL);
	if(!bResult || dwRead != sizeof(wTotalMonsertInfoNum))	__asm int 3
	
	if( wTotalMonsertInfoNum != 0 )
	{
		char* pInfo;
		pInfo = new char[ sizeof(Monster_Spot) * wTotalMonsertInfoNum ];
		
		bResult = ReadFile(hFile, pInfo, sizeof(Monster_Spot) * wTotalMonsertInfoNum, &dwRead, NULL);
		if(!bResult || dwRead != (sizeof(Monster_Spot) * wTotalMonsertInfoNum) )	__asm int 3
		
		delete[] pInfo;
	}
		
	//읽어드린 타일을 편집한다.
	DWORD x, z, s;
 	for( z = 0; z < m_dwTileNumHeight; z++ )
	{
		for( x = 0; x < m_dwTileNumWidth; x++ )
		{
	       	m_pTile[m_dwTileNumWidth * z + x].wIndex_X = (WORD)x;
			m_pTile[m_dwTileNumWidth * z + x].wIndex_Z = (WORD)z;
			m_pTile[m_dwTileNumWidth * z + x].x = x * TILE_WIDTH;
			m_pTile[m_dwTileNumWidth * z + x].z = z * TILE_HEIGHT;

			for( s = 1; s <= wTotalSectionNum; s++ )
			{
				if( m_pTile[m_dwTileNumWidth * z + x].wIndex_X	>= m_pParentLayer->GetSection(s)->x1 &&
					m_pTile[m_dwTileNumWidth * z + x].wIndex_X	<= m_pParentLayer->GetSection(s)->x2 &&
					m_pTile[m_dwTileNumWidth * z + x].wIndex_Z	>= m_pParentLayer->GetSection(s)->y1 &&
					m_pTile[m_dwTileNumWidth * z + x].wIndex_Z	<= m_pParentLayer->GetSection(s)->y2 )
				{
					m_pTile[m_dwTileNumWidth * z + x].wAttr.uSection = m_pParentLayer->GetSection(s)->m_bSectionNum;
					break;
				}
			}
		}
	}
		
	// Move Spot 읽어오기 
	// ttb로 부터 MoveSpot 갯수 읽어오기 
	WORD	wSpotNum;
	WORD	wID;
	bResult = ReadFile(hFile, &wSpotNum, sizeof(WORD), &dwRead, NULL);
	if(!bResult || dwRead > sizeof(WORD)*MAX_MOVE_SPOT_NUM)	__asm int 3

	// MoveSpot 생성 
	m_pParentLayer->m_pMoveSpot = new Move_Spot[ wSpotNum ];	
	memset(m_pParentLayer->m_pMoveSpot, 0, sizeof(Move_Spot)*wSpotNum);
	m_pParentLayer->m_wMoveSpotNum = wSpotNum;
	
	char szQuery[ 0xff ]={0,};
	wsprintf(szQuery, "Select * from SpotInfo where DungeonID = %d", m_pParentLayer->GetParent()->GetID());
	SPOT_VALUE	SpotInfo;
	
	int nRet = g_pDb->OpenRecord(szQuery, &SpotInfo, 1, GAME_DB);
	if(nRet < 0)	__asm int 3

	for(i=0; i<wSpotNum; i++)
	{
		bResult = ReadFile(hFile, &wID, sizeof(WORD), &dwRead, NULL);	

		if(!bResult || wID > MAX_MOVE_SPOT_NUM || wID == 0)	__asm int 3	
		
		bResult = ReadFile(hFile, &m_pParentLayer->m_pMoveSpot[wID-1].rcMoveSpot, sizeof(RECT), &dwRead, NULL);	
		if(!bResult)	__asm int 3

		m_pParentLayer->m_pMoveSpot[i].wDestLayerID = SpotInfo.spot[m_pParentLayer->m_bLayer].wDestLayerID[i];
		m_pParentLayer->m_pMoveSpot[i].wDestLayerMoveSpotNum = SpotInfo.spot[m_pParentLayer->m_bLayer].wDestSpotID[i];
	}
	
	// Start Spot 읽어오기 
	// ttb로 부터 StartSpot 갯수 읽어오기 
	WORD wStartSpotNum;
	bResult = ReadFile(hFile, &wStartSpotNum, sizeof(WORD), &dwRead, NULL);
	if(!bResult || dwRead > sizeof(WORD)*MAX_START_SPOT_NUM)	__asm int 3

	if( 0 < wStartSpotNum )
	{
		//StartSpot 생성 
		m_pParentLayer->m_pStartSpot = new RECT[ wStartSpotNum ];	memset(m_pParentLayer->m_pStartSpot, 0, sizeof(RECT));
		m_pParentLayer->m_wStartSpotNum = wStartSpotNum;

		for(i=0; i<wStartSpotNum; i++)
		{
			bResult = ReadFile(hFile, &wID, sizeof(WORD), &dwRead, NULL);	
			if(!bResult || wID > MAX_START_SPOT_NUM || wID == 0)	__asm int 3
			
			bResult = ReadFile(hFile, &m_pParentLayer->m_pStartSpot[wID-1], sizeof(RECT), &dwRead, NULL);	
			if(!bResult)	__asm int 3
		} // for(i=0; i<wStartSpotNum; i++)
	} // if( 0 < wStartSpotNum )
	else
	{
		m_pParentLayer->m_pStartSpot = NULL;
		m_pParentLayer->m_wStartSpotNum = 0;
	}
	
	CloseHandle(hFile);

	return TRUE;
}

//-----------------------------------------------------------------------------
// 지정해준 인덱스의 타일을 셋팅함. 
//-----------------------------------------------------------------------------
BOOL CMap::SetMap(DWORD dwX, DWORD dwZ, MAP_TILE* pTile)
{
	if(dwX >= m_dwTileNumWidth || dwZ >= m_dwTileNumHeight)	
		return FALSE;
	
	memcpy( m_pTile + (m_dwTileNumWidth * dwZ + dwX), pTile, sizeof(MAP_TILE));  
	return TRUE;
}

BOOL CMap::SetTileOccupied( DWORD dwX, DWORD dwZ, BYTE attr, LPVOID pType )
{
	if(dwX >= m_dwTileNumWidth || dwZ >= m_dwTileNumHeight)
	{	
		return FALSE;
	}
	
	if( m_pTile[ m_dwTileNumWidth * dwZ + dwX ].wAttr.uOccupied == TILE_OCCUPIED_DONT_MOVE ) 
	{
		return FALSE;
	}

	if( m_pTile[ m_dwTileNumWidth * dwZ + dwX ].pData == pType ) goto lb_set;
	if( m_pTile[ m_dwTileNumWidth * dwZ + dwX ].pData == NULL ) goto lb_set; 

	return FALSE;

lb_set:

	// 세팅
	if( attr == TILE_EMPTY )
	{
		m_pTile[ m_dwTileNumWidth * dwZ + dwX ].pData = NULL;
		m_pTile[ m_dwTileNumWidth * dwZ + dwX ].wAttr.uOccupied = TILE_EMPTY;
	}
	else 
	{
		m_pTile[ m_dwTileNumWidth * dwZ + dwX ].pData = pType;
		if( m_pParentLayer->GetParent()->GetDungeonDataEx()->GetDungeonType() == DUNGEON_TYPE_VILLAGE && m_pParentLayer->m_bLayer == 0 )
		{
			return TRUE;
		}

		m_pTile[ m_dwTileNumWidth * dwZ + dwX ].wAttr.uOccupied = attr;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
// 타일 인덱스 좌표로 타일 포인터를 구함 
//-----------------------------------------------------------------------------
MAP_TILE*	CMap::GetMap(DWORD dwX, DWORD dwZ)
{
	if( dwX >= m_dwTileNumWidth || dwZ >= m_dwTileNumHeight )	
		return NULL;

	return ( m_pTile + (m_dwTileNumWidth * dwZ + dwX) );
}

//-----------------------------------------------------------------------------
// 좌표로 타일 포인터를 구함. 
//-----------------------------------------------------------------------------
MAP_TILE* CMap::GetTile(float fx, float fz)
{
	if(fx < 0 || fz < 0)	return NULL;

	DWORD x = (DWORD)( fx / TILE_WIDTH );
	DWORD z = (DWORD)( fz / TILE_HEIGHT );
	
	return GetMap(x, z);
}