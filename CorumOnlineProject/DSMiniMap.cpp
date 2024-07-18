//======================================================//
// Code by Jang.							2002.12.30	//
//======================================================//
#include "DSMiniMap.h"
#include "User.h"
#include "Map.h"
#include "GameControl.h"
#include "DungeonTable.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "CodeFun.h"

CMiniMapWnd* CMiniMapWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CMiniMapWnd::CMiniMapWnd()
{
	m_wMapId		= 0;
	m_byMapLayer	= 0;
	m_dwLayerId		= 0;
	m_fRot			= 0.0f;
	m_bTemp			= FALSE;
	m_bInit			= FALSE;
	
	for(int i = 0; i < MAX_PARTY; i++)
		m_bPartyPtr[i] = 0;
	
}

CMiniMapWnd::~CMiniMapWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CMiniMapWnd::Init()
{
//	int nOrder = GetStartOrder();

	float x = 144 * (g_pMainPlayer->m_v3CurPos.x/g_pMap->m_fMiniMapSize);
	float y = 144 - (144*(g_pMainPlayer->m_v3CurPos.z/g_pMap->m_fMiniMapSize));

/*
	InsertData(SPR_OBJ_PARTY_RED, SPR_INTERFACE_PARTY_RED, 0, 0, 1.0f, 1.0f, 5);
	InsertData(SPR_OBJ_PARTY_BLUE, SPR_INTERFACE_PARTY_BLUE, 0, 0, 1.0f, 1.0f, 5);
	InsertData(SPR_OBJ_PARTY_WHITE, SPR_INTERFACE_PARTY_WHITE,  0, 0, 1.0f, 1.0f, 5);
	InsertData(SPR_OBJ_PARTY_GREEN, SPR_INTERFACE_PARTY_GREEN, 0, 0, 1.0f, 1.0f, 5);
	InsertData(SPR_OBJ_PARTY_BORA, SPR_INTERFACE_PARTY_BORA, 0, 0, 1.0f, 1.0f, 5);
	InsertData(SPR_OBJ_PARTY_YELLOW, SPR_INTERFACE_PARTY_YELLOW, 0, 0, 1.0f, 1.0f, 5);
	InsertData(SPR_OBJ_PARTY_ORANGE, SPR_INTERFACE_PARTY_ORANGE, 0, 0, 1.0f, 1.0f, 5);
	InsertData(SPR_OBJ_PARTY_PINK, SPR_INTERFACE_PARTY_PINK, 0, 0, 1.0f, 1.0f, 5);
	InsertData(SPR_OBJ_MINIMAP, SPR_INTERFACE_MINIMAP, 0, 0, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DSMAP_USER, SPR_INTERFACE_DSMAP_USER, x-3, 31+y, 1.0f, 1.0f, 4);
	
	InsertCheckInterface(0, 0, 159, 33, 2, CHECK_MOVEWND);
*/
	InsertData(SPR_OBJ_DSMAP_USER, SPR_INTERFACE_DSMAP_USER, x-3, 31+y, 1.0f, 1.0f, 4);

	SetRender(SPR_OBJ_MINIMAP, TRUE);
	SetRender(SPR_OBJ_DSMAP_USER, TRUE);

	

	m_bInit = TRUE;

	return TRUE;
}

DWORD CMiniMapWnd::GetPartyResource(BYTE byIndex)
{
	switch(byIndex)
	{
	case 0:
		return SPR_OBJ_PARTY_PINK;
	case 1:
		return SPR_OBJ_PARTY_BLUE;
	case 2:
		return SPR_OBJ_PARTY_WHITE;
	case 3:
		return SPR_OBJ_PARTY_GREEN;
	case 4:
		return SPR_OBJ_PARTY_BORA;
	case 5:
		return SPR_OBJ_PARTY_YELLOW;
	case 6:
		return SPR_OBJ_PARTY_ORANGE;
	case 7:
		return SPR_OBJ_PARTY_RED;		
	}		
	return 0;
}

void CMiniMapWnd::SetPartyPtr(DWORD dwResource)
{
	switch(dwResource)
	{
	case SPR_OBJ_PARTY_RED:
		m_bPartyPtr[0] = 0;
		break;
	case SPR_OBJ_PARTY_BLUE:
		m_bPartyPtr[1] = 0;
		break;
	case SPR_OBJ_PARTY_WHITE:
		m_bPartyPtr[2] = 0;
		break;
	case SPR_OBJ_PARTY_GREEN:
		m_bPartyPtr[3] = 0;
		break;
	case SPR_OBJ_PARTY_BORA:
		m_bPartyPtr[4] = 0;
		break;
	case SPR_OBJ_PARTY_YELLOW:
		m_bPartyPtr[5] = 0;
		break;
	case SPR_OBJ_PARTY_ORANGE:
		m_bPartyPtr[6] = 0;
		break;
	case SPR_OBJ_PARTY_PINK:
		m_bPartyPtr[7] = 0;
		break;
	}
}

void CMiniMapWnd::SetPartyPtr(BYTE byIndex, BYTE byValue)
{
	m_bPartyPtr[byIndex] = byValue;
}

BYTE CMiniMapWnd::GetPartyPtrRt(DWORD dwResource)
{
	for(BYTE i = 0; i < MAX_PARTY; i++)
	{
		if(GetPartyResource(i)==dwResource)
			return i;	
	}
	return MAX_PARTY;
}

BOOL CMiniMapWnd::GetPartyPtr(DWORD dwResource)
{
	for(BYTE i = 0; i < MAX_PARTY; i++)
	{
		if(m_bPartyPtr[i]==1)
		{
			if(GetPartyResource(i)==dwResource)
				return TRUE;
		}		
	}
	return FALSE;
}

BYTE CMiniMapWnd::GetPartyPtr()
{
	for(BYTE i = 0; i < MAX_PARTY; i++)
	{
		if(m_bPartyPtr[i]==0)
			return i;
	}
	return MAX_PARTY;
}

void CMiniMapWnd::Remove()
{
	RemoveAllData();
	
	m_bInit = FALSE;	
	m_bActive	= FALSE;
}

void CMiniMapWnd::SetOrder()
{
	int nOrder = GetStartOrder();

	POSITION_ pos = g_pMap->m_pCPList->GetHeadPosition();

	while(pos)
	{
		CP_DESC* pCPDesc = (CP_DESC*)g_pMap->m_pCPList->GetNext(pos);
		pCPDesc->pSpr_Minimap_Dot->nZOrder = nOrder+3;
	}		
}

void CMiniMapWnd::RenderText()
{
	if(m_bTemp)
	{
//		int nOrder = GetStartOrder();

		DUNGEON_DATA_EX* pDungeonInfo = g_pDungeonTable->GetDungeonInfo(m_wMapId);
				
		if(!IsEmptyString(pDungeonInfo->m_szDungeonName))
		{			
			// 던젼 이름 //
#if IS_JAPAN_LOCALIZING()
			// modified by minjin. 2004. 10. 26.
			// adjust Z-Order '4' to '6'
			if(lstrlen(pDungeonInfo->m_szSchoolName))
				RenderFont(pDungeonInfo->m_szSchoolName, m_fPosX+15, m_fPosX+120, m_fPosZ+9, m_fPosZ+23, GetStartOrder()+6);
			else
				RenderFont(pDungeonInfo->m_szDungeonName, m_fPosX+15, m_fPosX + 100, m_fPosZ+9, m_fPosZ+23, GetStartOrder()+6);
#else
			// 미니 멥에 이름 제대로 나오지 않던 버그 수정 
			// 2005.01.30 김영대 
			RenderFont(pDungeonInfo->m_szDungeonName, m_fPosX+15, m_fPosX+150, m_fPosZ+9, m_fPosZ+23, GetStartOrder()+6);			
#endif
		}		
		
		if(pDungeonInfo->m_dwID/1000!=0)
		{
			// 던젼 층수 //
			if(m_byMapLayer==0)
			{
				lstrcpy(m_szInfo, g_Message[ETC_MESSAGE16].szMessage);
			}
			else
			{

// Begin - minjin 2004. 08. 27.
// Debugging for Dungeon Layer (between Ground Dungeon and Underground Dungeon)
//
// 우선, 배열 첨자 에러가 우려되는 관계로, 일본에만 적용했고,
// 1236번 메시지로 "지상 %d층" 이라는 메시지를 추가.. cdb 추출하셔야 합니다.
// 모든 나라에 적용되고 나면, else 이하구문 지우겠습니다
// - 주 : 현재 던전 구분에 지상 / 지하가 없는 관계로, 던젼아이디로 그냥 비교해버렸습니다.. -_-;;

#if IS_JAPAN_LOCALIZING()
				if ( pDungeonInfo->m_dwID == 3027 ) // 탑 던전인 경우.. (지상이다)
				{
					wsprintf(m_szInfo, g_Message[ETC_MESSAGE1237].szMessage, m_byMapLayer);	// MSG_ID : 1237 ; 지상%d층
				}
				else
				{
					wsprintf(m_szInfo, g_Message[ETC_MESSAGE17].szMessage, m_byMapLayer);	// MSG_ID : 17 ; 지하 %d층
				}
#else
				wsprintf(m_szInfo, g_Message[ETC_MESSAGE17].szMessage, m_byMapLayer);	// MSG_ID : 17 ; 지하 %d층
#endif

// End - minjin

			}

			RenderFont(m_szInfo, m_fPosX+15, m_fPosX+120, m_fPosZ+23, m_fPosZ+37, GetStartOrder()+6);
		}
	}
}

void CMiniMapWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;
	
	if(bActive==TRUE)
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);		
}

int CMiniMapWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);

	switch(nRt)
	{
	case 1:
		{			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 2:
		{			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
		
	MouseUp();

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
		SetMinimapPos();

		if(m_dwLayerId!=0)
		{		
			SetPosObjX(SPR_OBJ_MAP, 8);
			SetPosObjY(SPR_OBJ_MAP, 42);
		}
		return CInterface::GetInstance()->GetSetWnd();
	}

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		
	
	return 0;
}

float CMiniMapWnd::GetRot(BOOL bChk)
{
	if(bChk)
		return m_fRot/90.0f;

	return m_fRot;
}

void CMiniMapWnd::SetRot(BOOL bInc)
{
	if(bInc)
	{
		m_fRot += 1.0f;

		if(m_fRot>=90.0f)
			m_fRot = 0;
	}
	else
	{
		m_fRot -= 1.0f;

		if(m_fRot<0)
			m_fRot = 89.0f;	
	}	
}

BOOL CMiniMapWnd::CreateMap()
{
//	int nOrder = GetStartOrder();	

	if(m_dwLayerId!=0)
	{
		char szMapModelFile[0xff] = {0,};
		char szFileNameTIF[0xff] = {0,};

		m_fRot	= 0.0f;
					
		wsprintf(szFileNameTIF, "%d.tif", m_dwLayerId);
//		wsprintf(szMapModelFile, "%s", GetFile(szFileNameTIF, DATA_TYPE_MAP));
		lstrcpy(szMapModelFile, GetFile(szFileNameTIF, DATA_TYPE_MAP));
			
		LP_SPRITE_DATA	lpSpriteData	= (LP_SPRITE_DATA)LALAlloc(g_pInterfaceSprPool);	
		memset(lpSpriteData, 0, sizeof(SSPRITE_DATA));
		lpSpriteData->dwId				= SPR_INTERFACE_MAP;
		lpSpriteData->pSpr				= g_pRenderer->CreateSpriteObject(GetFile(szFileNameTIF, DATA_TYPE_MAP), 0);
		g_pInterfaceSprHash->Insert(lpSpriteData, SPR_INTERFACE_MAP);
				
		InsertData(SPR_OBJ_MAP, SPR_INTERFACE_MAP, 8, 42, 1.0f, 1.0f, 1);

		SetRender(SPR_OBJ_MAP, TRUE);		
		SetOrder();

		return TRUE;
	}
	return FALSE;				
}

BOOL CMiniMapWnd::ReleaseMap()
{	
	if(m_dwLayerId!=0)
	{									
		m_dwLayerId	 = 0;

		LP_SPRITE_DATA lpSpriteData = g_pInterfaceSprHash->GetData(SPR_INTERFACE_MAP);

		if(lpSpriteData)
			g_pInterfaceSprHash->Delete(lpSpriteData, SPR_INTERFACE_MAP, TRUE);
			
		RemoveData(SPR_OBJ_MAP);

		return TRUE;
	}
	return FALSE;
}

VECTOR2 CMiniMapWnd::GetRatioMinimap(float x, float z)
{
	VECTOR2 vec2 = 
	{
		144 * (x/g_pMap->m_fMiniMapSize),
		144 - (144*(z/g_pMap->m_fMiniMapSize))
	};

	return vec2;
}
void CMiniMapWnd::SetMinimapPos()
{
	// 사이즈 변경 //
	VECTOR2 vec2 = GetRatioMinimap(g_pMainPlayer->m_v3CurPos.x, g_pMainPlayer->m_v3CurPos.z);	

	SetPosObjX(SPR_OBJ_DSMAP_USER, vec2.x+8-11);
	SetPosObjY(SPR_OBJ_DSMAP_USER, vec2.y+42-11);
	
	POSITION_ pos = g_pMap->m_pCPList->GetHeadPosition();	
	while(pos)
	{
		CP_DESC* pCPDesc = (CP_DESC*)g_pMap->m_pCPList->GetNext(pos);
		VECTOR2 vec2 = GetRatioMinimap(pCPDesc->pTile->x, pCPDesc->pTile->z);
		pCPDesc->pSpr_Minimap_Dot->vPos.x = m_fPosX+vec2.x+8;
		pCPDesc->pSpr_Minimap_Dot->vPos.y = m_fPosZ+vec2.y+42;
	}
}
//======================================================//
// End.													//
//======================================================//