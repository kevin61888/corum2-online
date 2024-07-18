#pragma once

#include "ObjectDefine.h"

//----------------------------------------------------------------------------------
// Damage Number....
#define MAX_DAMAGE_NUMBER		10

typedef struct Damage_Number
{
	OBJECT_HANDLE_DESC	m_hDamageNumber[4][10];
	OBJECT_HANDLE_DESC	m_hDamageChar[2];				// 0 - Block, 1 - Miss
	BYTE				m_pbUsedDamageNumber[10];
} DAMAGE_NUMBER, *LPDAMAGE_NUMBER;

struct DAMAGE_NUMBER_OBJECT_POOL
{
	DAMAGE_NUMBER		pDamageNumberPool[MAX_DAMAGE_NUMBER];
	DAMAGE_NUMBER		*pDamageNumberIndex[MAX_DAMAGE_NUMBER];
	DWORD				dwCurNumberIndex;
};

extern DAMAGE_NUMBER_OBJECT_POOL	g_pDamageNumberPool[2];			// Monster And User..

void					InitDamageNumber();
void					ReleaseDamageNumber();

DAMAGE_NUMBER*			AllocDamageNumber( DAMAGE_NUMBER_OBJECT_POOL* pPool );
void					FreeDamageNumber( DAMAGE_NUMBER_OBJECT_POOL* pPool, DAMAGE_NUMBER* pNumber );
void					ClearDamageNumber( DAMAGE_NUMBER* pNumber );

//----------------------------------------------------------------------------------
