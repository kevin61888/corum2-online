//======================================================//
// Code by Jang.							2003.05.06	//
// Object : Party.										//
//======================================================//
#include "stdafx.h"
#include "PartyHash.h"
#include "WorldUser.h"

STMPOOL_HANDLE g_pPartyTablePool = NULL;
STMPOOL_HANDLE g_pPartyBoardTablePool = NULL;

CPartyTableHash*		g_pPartyTableHash = NULL;
CPartyBoardTableHash*	g_pPartyBoardTableHash = NULL;

void CPartyTableHash::DeleteData(void* pData)
{
	LP_PARTY_TABLE pPartyTable = (LP_PARTY_TABLE)pData;

	if(pPartyTable)
	{
		if(pPartyTable->pPartyUserList)
		{
			POSITION_	pos	= pPartyTable->pPartyUserList->GetHeadPosition();			

			while(pos)
			{			
				CWorldUser* pUser = (CWorldUser*)pPartyTable->pPartyUserList->GetNext(pos);

				if(pUser)
					pUser->m_byLeader = 0;
			}
			pPartyTable->pPartyUserList->RemoveAll();
			delete pPartyTable->pPartyUserList;
			pPartyTable->pPartyUserList = NULL;
		}
		pPartyTable->dwPartyId	= 0;
		pPartyTable->bCount		= 0;
		LALFree(g_pPartyTablePool, pPartyTable);	
	}
}

void CPartyBoardTableHash::DeleteData(void* pData)
{
	LP_PARTYBOARD_TABLE pPartyBoardTable = (LP_PARTYBOARD_TABLE)pData;

	if(pPartyBoardTable)
	{		
		LALFree(g_pPartyBoardTablePool, pPartyBoardTable);	
		pPartyBoardTable = NULL;
	}
}

//======================================================//
// End.													//
//======================================================//