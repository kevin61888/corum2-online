//==================================================//
// Code by Jang.									//
//==================================================//
#include "InterfaceSpr.h"

void CInterfaceSprHash::DeleteData(void* pData)
{
	LP_SPRITE_DATA pSprData = (LP_SPRITE_DATA)pData;

	if(!pSprData)
		return;			

	pSprData->pSpr->Release();

	LALFree(g_pInterfaceSprPool, pSprData);
	pSprData = NULL;
}
//==================================================//
// End.												//
//==================================================//