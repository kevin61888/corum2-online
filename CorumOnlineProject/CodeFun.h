//==============================================================//
// Code by Jang.									2004.06.22	//
// Object : 필요한 함수 모음.									//
//==============================================================//
#pragma once

#define ITEM_ATTACH_TYPE_NONE	0
#define ITEM_ATTACH_TYPE_HEAD	1
#define ITEM_ATTACH_TYPE_LHAND	2
#define ITEM_ATTACH_TYPE_RHAND	3
#define ITEM_ATTACH_TYPE_FLAG	4

#include "Define.h"
#include "struct.h"

extern BYTE		g_byPointerType;

//==============================================================//
// Function.													//
//==============================================================//
extern void		ItemDataName(char* pszItemName, WORD wItemId, int nIndex = 0);
extern void		ItemAttach(WORD wItemId, GXOBJECT_HANDLE* hHandle, LPObjectDesc pDesc, GXOBJECT_HANDLE* hAttachHandle, BYTE byType = ITEM_ATTACH_TYPE_NONE, int nIndex = 0, BYTE byDir = 0, char* szItemInfo = NULL);
extern void		ItemDeleteObject(GXOBJECT_HANDLE* hHandle, GXOBJECT_HANDLE* hAttachHandle = NULL);
extern void		ObjDeleteDesc(LPObjectDesc* pDesc);
extern void		DeleteObj(GXOBJECT_HANDLE* hHandle);
extern void		SetEffectIndex(GXOBJECT_HANDLE hHandle, DWORD dwEffectIndex = 0xffffffff);
extern void		SetMousePointerType(BYTE byPointerType);
extern void		SetStrFn(char* pszText, char** pArrayText, BYTE* byLen, int nIndex, int nSize, int nMaxSize);
extern void		GXSetPosition(GXOBJECT_HANDLE gxh,VECTOR3* pv3Pos, BOOL bDoInterpolation, BOOL bChk = FALSE);
extern int		GetType(WORD wId);

extern void		WriteErrorLogFile(const char* pErrMsg, ...);

// 짱나라고 일부러.. 꼭 캐스팅해야 에러 안날껄?? - deepdark.
extern void		RenderFont(char* pszBuffer, int nLeft, int nRight, int nTop, int nBottom, int nOrder, DWORD dwColor = 0xffffffff);
extern void		RenderFont(char* pszBuffer, float fLeft, float fRight, float fTop, float fBottom, int nOrder, DWORD dwColor = 0xffffffff);
//==============================================================//
// End.															//
//==============================================================//