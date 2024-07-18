//==============================================================//
// Code by Jang.									2004.06.22	//
//==============================================================//
#include "CodeFun.h"
#include "Define.h"
#include "stdafx.h"
#include "GameControl.h"
#include "InitGame.h"
#include "DSMiniMap.h"
#include "DungeonProcess.h"
#include "Map.h"
#include "DungeonTable.h"
#include "StringFunc.h"
#include "../CommonServer/CommonHeader.h"
//#include "ItemManagerDefine.h"


BYTE g_byPointerType = __MOUSE_POINTER_DEFAULT__;


//==============================================================//
// 아이템 이름 설정.											//
//==============================================================//
void ItemDataName(char* pszItemName, WORD wItemId, int nIndex)
{
	LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(wItemId);	
				
	if(lpItemResource)
	{	
		if(lpItemResource->byResourceType==__ITEM_RESOURCETYPE_MOD__)
			wsprintf(pszItemName, "%s_%03d.mod", lpItemResource->szModFileName, nIndex);
		else if(lpItemResource->byResourceType==__ITEM_RESOURCETYPE_CHR__)
			wsprintf(pszItemName, "%s_%03d.chr", lpItemResource->szModFileName, nIndex);		
	}
}


//==============================================================//
// 아이템 생성, 캐릭터 Attach.									//
//==============================================================//
void ItemAttach(WORD wItemId, GXOBJECT_HANDLE* hHandle, LPObjectDesc pDesc, GXOBJECT_HANDLE* hAttachHandle, BYTE byType
				, int nIndex, BYTE byDir, char* szItemInfo)
{
	LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(wItemId);

	pDesc = NULL;

	GXOBJECT_HANDLE hItemHandle			= NULL;
	GXOBJECT_HANDLE hAttachItemHandle	= NULL;
	
	if(hAttachHandle)
		hAttachItemHandle = *(hAttachHandle);

	char szInfo[0xff] = {0, };

	if(byType==ITEM_ATTACH_TYPE_FLAG)
	{		
		wsprintf(szInfo, szItemInfo);	

		pDesc		= AllocObjDesc();
		hItemHandle	= CreateHandleObject(szInfo, GXPlayerPROC, pDesc, 0);
		g_pExecutive->GXOAttach(hItemHandle, hAttachItemHandle, "Bip01 Spine3");	

		if(hHandle)
			*(hHandle) = hItemHandle;

		return;
	}

	if(!lpItemResource)
		return;		
	
	ItemDataName(szInfo, wItemId, nIndex);

	DWORD dwDir = (byDir) ? DATA_TYPE_CHARACTER : DATA_TYPE_ITEM;
/*
	DWORD dwDir = DATA_TYPE_ITEM;
	
	if(byDir)
		dwDir = DATA_TYPE_CHARACTER;*/

	if(lpItemResource->byAnimation==1)
	{			
		pDesc		= AllocObjDesc();
		hItemHandle	= CreateHandleObject(GetFile(szInfo, BYTE(dwDir)), GXPlayerPROC, pDesc, 0);
	}
	else
	{
		hItemHandle = CreateHandleObject(GetFile(szInfo, BYTE(dwDir)), NULL, NULL, 0);
	}

	switch(byType)
	{
	case ITEM_ATTACH_TYPE_LHAND:	g_pExecutive->GXOAttach(hItemHandle, hAttachItemHandle, "Bip01 L Hand");	break;
	case ITEM_ATTACH_TYPE_RHAND:	g_pExecutive->GXOAttach(hItemHandle, hAttachItemHandle, "Bip01 R Hand");	break;
	case ITEM_ATTACH_TYPE_HEAD:		g_pExecutive->GXOAttach(hItemHandle, hAttachItemHandle, "Bip01 Head");		break;		
	}

	if(hHandle)
		*(hHandle) = hItemHandle;
}


//==============================================================//
// 아이템 삭제.													//
//==============================================================//
void ItemDeleteObject(GXOBJECT_HANDLE* hHandle, GXOBJECT_HANDLE* hAttachHandle)
{
	GXOBJECT_HANDLE hDeleteHandle		= NULL;
	GXOBJECT_HANDLE	hDeleteAttachHandle	= NULL;

	if(hHandle)
		hDeleteHandle = *(hHandle);

	if(hAttachHandle)
		hDeleteAttachHandle	= *(hAttachHandle);
	
	if(hDeleteHandle && hDeleteAttachHandle)
		g_pExecutive->GXODetach(hDeleteAttachHandle, hDeleteHandle);

	if(hDeleteHandle)
	{		
		DeleteHandleObject(hDeleteHandle);
		hDeleteHandle = NULL;
	}

	if(hHandle)
		*(hHandle) = hDeleteHandle;

	if(hAttachHandle)
		*(hAttachHandle) = hDeleteAttachHandle;
}


//==============================================================//
// 아이템 삭제.													//
//==============================================================//
void ObjDeleteDesc(LPObjectDesc* pDesc)
{
	LPObjectDesc pDeleteDesc = NULL;
		
	if(pDesc)
		pDeleteDesc = *(pDesc);

	if(pDeleteDesc)
	{
		FreeObjDesc(pDeleteDesc);
		pDeleteDesc = NULL;

		if(pDesc)
			*(pDesc) = pDeleteDesc;
	}
}


//==============================================================//
// GXOBJECT_HANDLE 삭제.										//
//==============================================================//
void DeleteObj(GXOBJECT_HANDLE* hDeleteHandle)
{
	GXOBJECT_HANDLE hHandle = NULL;

	if(hDeleteHandle)
		hHandle = *(hDeleteHandle);

	if(hHandle)
	{
		DeleteHandleObject(hHandle);
		hHandle = NULL;
	}

	if(hDeleteHandle)
		*(hDeleteHandle) = hHandle;
}


//==============================================================//
// 이펙트 인덱스 설정.											//
//==============================================================//
void SetEffectIndex(GXOBJECT_HANDLE hHandle, DWORD dwEffectIndex)
{
	if(hHandle)
		g_pExecutive->GXOSetEffectIndex(hHandle, dwEffectIndex);
}


//==============================================================//
// 마우스 포인터 설정.											//
//==============================================================//
void SetMousePointerType(BYTE byPointerType)
{
	g_byPointerType = byPointerType;

	if(!g_hCursor[ byPointerType ] || byPointerType >= __MAX_MOUSE_POINTER__)	
	{
		asm_int3();
		return;
	}

	if(GetCursor() != g_hCursor[ byPointerType ])
		SetCursor( g_hCursor[ byPointerType ] );
}


//==============================================================//
// 문자열 일정 사이즈로 만들는 함수.							//
//==============================================================//
void SetStrFn(char* pszText, char** pArrayText, BYTE* byLen, int nIndex, int nSize, int nMaxSize)
{
	char* szTemp	= 0;	
	szTemp			= pszText;

	while(nSize > 0)
	{
		if(nSize > nMaxSize)
		{
			for(;;)
			{
				if(nSize > nMaxSize)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+nMaxSize, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev);

					__lstrcpyn(pArrayText[nIndex], szTemp, nSize);
					byLen[nIndex] = (BYTE)nSize;

					szTemp = szPrev;
					nIndex++;
					nSize = lstrlen(szTemp);
				}
				else
				{
					// 구한 문자열을 채팅 라인에 쓰고, 길이를 기록한다.
					lstrcpy(pArrayText[nIndex], szTemp);
					byLen[nIndex] = (BYTE)lstrlen(szTemp);
					nIndex++;								
					nSize = 0;
					break;
				}
			}
		}
		else
		{	
			memset(pArrayText[nIndex], 0, nMaxSize);
			__lstrcpyn(pArrayText[nIndex], szTemp, lstrlen(szTemp));	
			byLen[nIndex] = (BYTE)lstrlen(pArrayText[nIndex]);
			nIndex++;
			nSize = 0;
		}		
	}
}


void GXSetPosition(GXOBJECT_HANDLE gxh, VECTOR3* pv3Pos, BOOL bDoInterpolation, BOOL bChk)
{	
	if(!bChk || GetGameStatus()==UPDATE_GAME_WORLD)
	{
		g_pExecutive->GXOSetPosition(gxh, pv3Pos, bDoInterpolation);
	}
	else
	{
		if(!g_pMap)
			return;

		MAP_TILE* pTile = g_pMap->GetTile(pv3Pos->x, pv3Pos->z);

		if(!pTile)
		{
			if(g_pMainPlayer->m_bInEventDungeon)
			{
				g_pExecutive->GXOSetPosition(gxh, pv3Pos, bDoInterpolation);
				return;
			}

			BOOL bChk = FALSE;

			CMiniMapWnd*		pMiniMapWnd		= CMiniMapWnd::GetInstance();
			DUNGEON_DATA_EX*	pDungeonInfo	= NULL;
			
			if(g_pDungeonTable)
			{
				pDungeonInfo = g_pDungeonTable->GetDungeonInfo(pMiniMapWnd->m_wMapId);

				if(!IsEmptyString(pDungeonInfo->m_szDungeonName))
					bChk = TRUE;
			}

			if(!bChk)
				WriteErrorLogFile("Handle : 0x%x, PosX : %f, PosZ : %f\n", gxh, pv3Pos->x, pv3Pos->z);
			else
				WriteErrorLogFile("Handle : 0x%x, Dungeon : %s, PosX : %f, PosZ : %f\n", gxh, pDungeonInfo->m_szDungeonName, pv3Pos->x, pv3Pos->z);

			return;
		}
		
		g_pExecutive->GXOSetPosition(gxh, pv3Pos, bDoInterpolation);
	}	
}

int GetType(WORD wId)
{
	int nValue = wId;

	if(	(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)	||
		(nValue>=ITEM_ARMORST_INDEX	&& nValue<=ITEM_ARMOREND_INDEX)		||
		nValue==ITEM_GUARDIAN_INDEX || nValue==ITEM_RIDE_INDEX || nValue==ITEM_BAG_INDEX)
	{
		return ITEM_LARGE;	
	}
	else if(	(nValue>=ITEM_MATERIALST_INDEX		&& nValue<=ITEM_MATERIALEND_INDEX) 
			||	(nValue>=ITEM_SPECIALST_INDEX		&& nValue<=ITEM_SPECIALEND_INDEX) 
			||	(nValue>=ITEM_MIXUPGRADEST_INDEX	&& nValue<=ITEM_MIXUPGRADEEND_INDEX) 
			||	(nValue>=ITEM_CONSUMABLEST_INDEX	&& nValue<=ITEM_CONSUMABLEEND_INDEX)
			||	nValue==ITEM_SUPPLIES_INDEX			|| nValue==ITEM_ZODIAC_INDEX 
			||	nValue==ITEM_MAGICARRAY_INDEX		|| nValue==ITEM_MAGICFIELDARRAY_INDEX 
			||	nValue==ITEM_UPGRADE_INDEX			|| nValue==ITEM_LIQUID_INDEX || nValue==ITEM_EDITION_INDEX)
	{
		return ITEM_SMALL;
	}
	else
	{	
		return ITEM_NONE;
	}
}

void RenderFont(char* pszBuffer, float fLeft, float fRight, float fTop, float fBottom, int nOrder, DWORD dwColor)
{
	RenderFont(pszBuffer, (int)fLeft, (int)fRight, (int)fTop, (int)fBottom, nOrder, dwColor);
}

void RenderFont(char* pszBuffer, int nLeft, int nRight, int nTop, int nBottom, int nOrder, DWORD dwColor)
{
	if(nLeft==0 && nRight==0)
	{
//		asm_int3();
		return;
	}
	if(nTop==0 && nBottom==0)
	{
//		asm_int3();
		return;
	}

	int nSize = lstrlen(pszBuffer);

	if(!nSize)
	{
		//asm_int3();
		return;
	}

	RECT rt;
	SetRect(&rt, nLeft, nTop, nRight, nBottom);
	g_pGeometry->RenderFont(GetFont(), pszBuffer, nSize, &rt, dwColor, CHAR_CODE_TYPE_ASCII, nOrder, 0);					
}

void WriteErrorLogFile(const char* pErrMsg, ...)
{
	SYSTEMTIME	st;
	va_list		argptr;

	FILE*		pFile		= NULL; 
		
	int			nCnt		= 0; 
	char		szBuf[1024] = ""; 

	GetLocalTime(&st);

	wsprintf(szBuf, "%s\\%04d%02d%02d.log", g_szBasePath, st.wYear, st.wMonth, st.wDay);

	pFile = fopen(szBuf, "a+w");

	if(!pFile)
		return;	

	fprintf(pFile,"%02d-%02d-%02d %02d:%02d:%02d ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	
	va_start(argptr, pErrMsg); 
	nCnt = vfprintf(pFile, pErrMsg, argptr); 
	va_end(argptr); 

	fclose(pFile); 
}
