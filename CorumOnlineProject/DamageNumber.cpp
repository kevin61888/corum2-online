#include "DamageNumber.h"
#include "GameControl.h"
#include "GameDefine.h"
#include "InitGame.h"
#include "CodeFun.h"

DAMAGE_NUMBER_OBJECT_POOL	g_pDamageNumberPool[2];

//------------------------------------------------------------------------------------------------
// Damage Number..
void	InitDamageNumber()
{
	int j = 0, k = 0, l = 0;
	char	szBuf[1024] = {0,};

	memset( g_pDamageNumberPool, 0, sizeof( DAMAGE_NUMBER_OBJECT_POOL ) * 2 );
	
//	for( i=0; i<2; i++ )		// Monster And User
//	{
	for( j=0; j<MAX_DAMAGE_NUMBER; j++ )	
	{
		for( k=0; k<4; k++ )
		{
			for( l=0; l<10; l++ )
			{
				wsprintf( szBuf, "dfy%d.chr", l );
				g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageNumber[k][l].pDesc = AllocObjDesc();
				g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageNumber[k][l].pHandle = 
					CreateHandleObject( GetFile( szBuf, DATA_TYPE_DAMAGENUMBER ), GXPlayerPROC, g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageNumber[k][l].pDesc, GXOBJECT_CREATE_TYPE_EFFECT );						
				
				if( !g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageNumber[k][l].pHandle ) asm_int3();
				HideObject( g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageNumber[k][l].pHandle );
			}
		}

		g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageChar[0].pDesc = AllocObjDesc();
		g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageChar[1].pDesc = AllocObjDesc();
	
		g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageChar[0].pHandle = 
			CreateHandleObject( GetFile( "dfyblock.chr", DATA_TYPE_DAMAGENUMBER ), GXPlayerPROC, g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageChar[0].pDesc, GXOBJECT_CREATE_TYPE_EFFECT );
		HideObject( g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageChar[0].pHandle );

		g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageChar[1].pHandle = 
			CreateHandleObject( GetFile( "dfymiss.chr", DATA_TYPE_DAMAGENUMBER ), GXPlayerPROC, g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageChar[1].pDesc, GXOBJECT_CREATE_TYPE_EFFECT );
		HideObject( g_pDamageNumberPool[0].pDamageNumberPool[j].m_hDamageChar[1].pHandle );

		// 포인터 연결.
		g_pDamageNumberPool[0].pDamageNumberIndex[j] = &g_pDamageNumberPool[0].pDamageNumberPool[j];
	}

	for( j=0; j<MAX_DAMAGE_NUMBER; j++ )	
	{
		for( k=0; k<4; k++ )
		{
			for( l=0; l<10; l++ )
			{
				wsprintf( szBuf, "dfr%d.chr", l );
				g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageNumber[k][l].pDesc = AllocObjDesc();
				g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageNumber[k][l].pHandle = 
					CreateHandleObject( GetFile( szBuf, DATA_TYPE_DAMAGENUMBER ), GXPlayerPROC, g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageNumber[k][l].pDesc, GXOBJECT_CREATE_TYPE_EFFECT );						
				
				if( !g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageNumber[k][l].pHandle ) asm_int3();
				HideObject( g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageNumber[k][l].pHandle );
			}
		}

		g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageChar[0].pDesc = AllocObjDesc();
		g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageChar[1].pDesc = AllocObjDesc();
	
		g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageChar[0].pHandle = 
			CreateHandleObject( GetFile( "dfrblock.chr", DATA_TYPE_DAMAGENUMBER ), GXPlayerPROC, g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageChar[0].pDesc, GXOBJECT_CREATE_TYPE_EFFECT );
		HideObject( g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageChar[0].pHandle );

		g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageChar[1].pHandle = 
			CreateHandleObject( GetFile( "dfrmiss.chr", DATA_TYPE_DAMAGENUMBER ), GXPlayerPROC, g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageChar[1].pDesc, GXOBJECT_CREATE_TYPE_EFFECT );
		HideObject( g_pDamageNumberPool[1].pDamageNumberPool[j].m_hDamageChar[1].pHandle );

		// 포인터 연결.
		g_pDamageNumberPool[1].pDamageNumberIndex[j] = &g_pDamageNumberPool[1].pDamageNumberPool[j];
	}
//	}
}

void	ReleaseDamageNumber()
{
	int i, j, k, l;
	
	for( i=0; i<2; i++ )		// Monster And User
	{
		for( j=0; j<MAX_DAMAGE_NUMBER; j++ )	
		{
			for( k=0; k<4; k++ )
			{
				for( l=0; l<10; l++ )
				{
					DeleteHandleObject( g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageNumber[k][l].pHandle );
					g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageNumber[k][l].pHandle = NULL;

					FreeObjDesc( g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageNumber[k][l].pDesc );
				}
			}

			DeleteHandleObject( g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageChar[0].pHandle );
			g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageChar[0].pHandle = NULL;
			FreeObjDesc( g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageChar[0].pDesc );

			DeleteHandleObject( g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageChar[1].pHandle );
			g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageChar[0].pHandle = NULL;
			FreeObjDesc( g_pDamageNumberPool[i].pDamageNumberPool[j].m_hDamageChar[1].pDesc );
		}
	}
}

DAMAGE_NUMBER*	AllocDamageNumber( DAMAGE_NUMBER_OBJECT_POOL* pPool )
{
	if( !pPool ) return NULL;

	DAMAGE_NUMBER*	pResult = NULL;

	if( pPool->dwCurNumberIndex >= MAX_DAMAGE_NUMBER ) // 더이상 없다.
		return NULL;
	
	pResult = pPool->pDamageNumberIndex[ pPool->dwCurNumberIndex ];
	pPool->dwCurNumberIndex++;

	return pResult;
}

void	FreeDamageNumber( DAMAGE_NUMBER_OBJECT_POOL* pPool, DAMAGE_NUMBER* pNumber )
{
	if( !pPool ) return;
	if( !pNumber ) return;
	if( pPool->dwCurNumberIndex == 0 ) return;
	
	pPool->dwCurNumberIndex--;
	pPool->pDamageNumberIndex[ pPool->dwCurNumberIndex ] = pNumber;
}

void	ClearDamageNumber( DAMAGE_NUMBER* pNumber )
{
	if( !pNumber ) return;

	int i, j;

	for( i=0; i<4; i++ )
	{
		for( j=0; j<10; j++ )
		{
			pNumber->m_hDamageNumber[i][j].pDesc->ObjectFunc = NULL;
			g_pExecutive->SetAlphaFlag( pNumber->m_hDamageNumber[i][j].pHandle, 255 );
			HideObject( pNumber->m_hDamageNumber[i][j].pHandle );
		}
	}
	memset( pNumber->m_pbUsedDamageNumber, 0, sizeof( BYTE ) * 10 );

	pNumber->m_hDamageChar[0].pDesc->ObjectFunc	= NULL;
	g_pExecutive->SetAlphaFlag( pNumber->m_hDamageChar[0].pHandle, 255 );
	HideObject( pNumber->m_hDamageChar[0].pHandle );

	pNumber->m_hDamageChar[1].pDesc->ObjectFunc	= NULL;
	g_pExecutive->SetAlphaFlag( pNumber->m_hDamageChar[1].pHandle, 255 );
	HideObject( pNumber->m_hDamageChar[1].pHandle );
}