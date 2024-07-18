#include "stdafx.h"
#include "MessengerHash.h"

//======================================================//
// Code by Jang.							2003.11.0	//
// Object : Guild.										//
//======================================================//

CMessengerTableHash*	g_pMessengerTableHash = NULL;
STMPOOL_HANDLE			g_pMessengerTablePool = NULL;
STMPOOL_HANDLE			g_pMessengerUserPool = NULL;

void CMessengerUserHash::DeleteData(void* pData)
{
	LP_MESSENGER_USER lpMessengerUser = (LP_MESSENGER_USER)pData;

	if(lpMessengerUser)
	{
		LALFree(g_pMessengerUserPool, lpMessengerUser);	
		lpMessengerUser = NULL;
	}
}

void CMessengerTableHash::DeleteData(void* pData)
{
	LP_MESSENGER_TABLE lpMessengerTable = (LP_MESSENGER_TABLE)pData;

	if(lpMessengerTable)
	{
		if(lpMessengerTable->pMessengerUserHash)
		{
			lpMessengerTable->pMessengerUserHash->Destroy(TRUE);
			delete lpMessengerTable->pMessengerUserHash;
			lpMessengerTable->pMessengerUserHash = NULL;
		}
		LALFree(g_pMessengerTablePool, lpMessengerTable);	
		memset(lpMessengerTable, 0, sizeof(MESSENGER_TABLE));
		lpMessengerTable = NULL;
	}
}
//======================================================//
// End.													//
//======================================================//