#include "stdafx.h"
#include "Messenger.h"
#include "WorldUserTable.h"
#include "DungeonTable.h"


void CmdMessengerUserAdd(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_MESSENGER_USERADD* pPacket = (CTWS_MESSENGER_USERADD*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_MESSENGER_USERFAIL	pFailSendPacket;

	CWorldUser* pMessengerUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

	if(pMessengerUser)
	{
		LP_MESSENGER_TABLE lpMessengerTable = g_pMessengerTableHash->GetData(pMessengerUser->m_dwUserIndex);

		if(!lpMessengerTable)
		{
			// 실패 메세지 //
			memset(pFailSendPacket.szName, 0, sizeof(pFailSendPacket.szName));
			__lstrcpyn(pFailSendPacket.szName, pMessengerUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pFailSendPacket.byFail	= 1;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailSendPacket, pFailSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);			
			return;
		}
		
		
		if(lpMessengerTable->pMessengerUserHash->GetCount()==MAX_MESSENGER_USER)
		{
			// 실패 메세지 //
			memset(pFailSendPacket.szName, 0, sizeof(pFailSendPacket.szName));
			__lstrcpyn(pFailSendPacket.szName, pMessengerUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pFailSendPacket.byFail	= 1;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailSendPacket, pFailSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}

		LP_MESSENGER_TABLE lpMessengerUserTable = g_pMessengerTableHash->GetData(pUser->m_dwUserIndex);

		if(!lpMessengerUserTable)
		{
			// 실패 메세지 //
			memset(pFailSendPacket.szName, 0, sizeof(pFailSendPacket.szName));
			__lstrcpyn(pFailSendPacket.szName, pMessengerUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pFailSendPacket.byFail	= 2;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailSendPacket, pFailSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}		

		if(lpMessengerUserTable->pMessengerUserHash->GetCount()==MAX_MESSENGER_USER)
		{
			// 실패 메세지 //
			memset(pFailSendPacket.szName, 0, sizeof(pFailSendPacket.szName));
			__lstrcpyn(pFailSendPacket.szName, pMessengerUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pFailSendPacket.byFail	= 2;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailSendPacket, pFailSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}		
		
		LP_MESSENGER_USER lpChkUser		= lpMessengerTable->pMessengerUserHash->GetData(pUser->m_dwUserIndex);
		LP_MESSENGER_USER lpChkUserDst	= lpMessengerUserTable->pMessengerUserHash->GetData(pMessengerUser->m_dwUserIndex);

		if(lpChkUser && lpChkUserDst)
		{
			// 실패 메세지 //
			memset(pFailSendPacket.szName, 0, sizeof(pFailSendPacket.szName));
			__lstrcpyn(pFailSendPacket.szName, pMessengerUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pFailSendPacket.byFail	= 3;
			g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailSendPacket, pFailSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			return;
		}
		
		// 적용 //
		WSTC_MESSENGER_USERADD pSendPacket;
		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
		pSendPacket.dwUserIndex	= pUser->m_dwUserIndex;
		g_pNet->SendToUser(pMessengerUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
	else
	{
		// 실패 메세지 //
		memset(pFailSendPacket.szName, 0, sizeof(pFailSendPacket.szName));
		pFailSendPacket.byFail	= 0;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pFailSendPacket, pFailSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
}

void CmdMessengerAnswer(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_MESSENGER_ANSWER* pPacket = (CTWS_MESSENGER_ANSWER*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	if(pPacket->byType==0)
	{
		// 거부 했을 경우 //

		CWorldUser* pMessengerUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

		if(pMessengerUser)
		{
			WSTC_MESSENGER_FAIL pMessengerPacket;
			memset(pMessengerPacket.szName, 0, sizeof(pMessengerPacket.szName));
			__lstrcpyn(pMessengerPacket.szName, pPacket->szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			g_pNet->SendToUser(pMessengerUser->m_dwConnectionIndex, (char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}
	}
	else
	{
		// 승인 했을 경우 //
		CWorldUser* pMessengerUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

		if(pMessengerUser)
		{
			// DB 업데이트 //
			LP_MESSENGER_TABLE lpMessengerTable		= g_pMessengerTableHash->GetData(pUser->m_dwUserIndex);
			LP_MESSENGER_TABLE lpMessengerSubTable	= g_pMessengerTableHash->GetData(pMessengerUser->m_dwUserIndex);

			if(lpMessengerTable && lpMessengerSubTable)
			{
				char	szQuery[0xff]={0,};

				LP_MESSENGER_USER lpMessengerUser = lpMessengerTable->pMessengerUserHash->GetData(pMessengerUser->m_dwUserIndex);

				if(!lpMessengerUser)
				{
					LP_MESSENGER_USER lpMessengerUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);

					if(!lpMessengerUser)
						return;

					memset(lpMessengerUser, 0, sizeof(MESSENGER_USER));
					lpMessengerUser->byType			= 0; 
					lpMessengerUser->dwUserIndex	= pMessengerUser->m_dwUserIndex;				
					lpMessengerUser->wClass			= pMessengerUser->m_wClass;
					__lstrcpyn(lpMessengerUser->szCharName, pMessengerUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);				
					lpMessengerTable->pMessengerUserHash->Insert(lpMessengerUser, lpMessengerUser->dwUserIndex);				
										
					// DB Insert.
					wsprintf(szQuery, "Insert into Messenger(Char_Index, Dst_Char_Index, Name, Type, Class) VALUES(%u, %u, '%s', %u, %u)", pUser->m_dwUserIndex, lpMessengerUser->dwUserIndex, lpMessengerUser->szCharName, lpMessengerUser->byType, lpMessengerUser->wClass);
					g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pUser, (BYTE)GAME_DB);				

					if(lpMessengerTable->pMessengerUserHash->GetCount()==1)
					{
						wsprintf(szQuery, "Update CharacterInfo Set Messenger = 1 Where Char_Index = %u", pUser->m_dwUserIndex);
						g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pUser, (BYTE)GAME_DB);
					}				
				}
				else
				{
				}								

				LP_MESSENGER_USER lpMessengerUserDst = lpMessengerSubTable->pMessengerUserHash->GetData(pUser->m_dwUserIndex);

				if(!lpMessengerUserDst)
				{
					LP_MESSENGER_USER lpMessengerSubUser = (LP_MESSENGER_USER)LALAlloc(g_pMessengerUserPool);

					if(!lpMessengerSubUser)
						return;

					memset(lpMessengerSubUser, 0, sizeof(MESSENGER_USER));
					lpMessengerSubUser->byType		= 0; 
					lpMessengerSubUser->dwUserIndex	= pUser->m_dwUserIndex;
					lpMessengerSubUser->wClass		= pUser->m_wClass;							
					__lstrcpyn(lpMessengerSubUser->szCharName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);				
					lpMessengerSubTable->pMessengerUserHash->Insert(lpMessengerSubUser, lpMessengerSubUser->dwUserIndex);

					// DB Insert.				
					wsprintf(szQuery, "Insert into Messenger(Char_Index, Dst_Char_Index, Name, Type, Class) VALUES(%u, %u, '%s', %u, %u)", pMessengerUser->m_dwUserIndex, lpMessengerSubUser->dwUserIndex, lpMessengerSubUser->szCharName, lpMessengerSubUser->byType, lpMessengerSubUser->wClass);
					g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pUser, (BYTE)GAME_DB);				

					if(lpMessengerSubTable->pMessengerUserHash->GetCount()==1)
					{
						wsprintf(szQuery, "Update CharacterInfo Set Messenger = 1 Where Char_Index = %u", pMessengerUser->m_dwUserIndex);
						g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pMessengerUser, (BYTE)GAME_DB);
					}
				}
				else
				{
				}				
				
				// 메세지 보내기 //
				WSTC_MESSENGER_SUCCESS pSendPacket;
				memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
				__lstrcpyn(pSendPacket.szName, pMessengerUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
				pSendPacket.byType		= 0;
				pSendPacket.dwUserIndex	= pMessengerUser->m_dwUserIndex;								
				pSendPacket.wClass		= pMessengerUser->m_wClass;
				g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);

				memset(pSendPacket.szName, 0, sizeof(pSendPacket.szName));
				__lstrcpyn(pSendPacket.szName, pUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
				pSendPacket.byType		= 0;
				pSendPacket.dwUserIndex	= pUser->m_dwUserIndex;	
				pSendPacket.wClass		= pUser->m_wClass;
				g_pNet->SendToUser(pMessengerUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}		
		}
	}
}

void CmdMessengerDelete(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_FRIENDUSER_DELETE* pPacket = (CTWS_FRIENDUSER_DELETE*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	WSTC_MESSENGER_USERINFO	pOnlinePacket;

	LP_MESSENGER_TABLE lpMessengerTable	= g_pMessengerTableHash->GetData(pUser->m_dwUserIndex);

	if(lpMessengerTable)
	{
		char		szQuery[0xff]	= {0,};
		DBBINDING*	pBinding		= 0;		
		int			nColumn			= 1;	
		int			col				= 0;
		DWORD		dwParamOffset	= 0;
		BYTE		byIndex			= 0;

		LP_MESSENGER_USER lpMessengerUser = lpMessengerTable->pMessengerUserHash->GetData(pPacket->dwUserIndex);

		if(!lpMessengerUser)
			return;

		lpMessengerTable->pMessengerUserHash->Delete(lpMessengerUser, lpMessengerUser->dwUserIndex, TRUE);
		memset(lpMessengerUser, 0, sizeof(MESSENGER_USER));
		lpMessengerUser = NULL;

		MESSENGER_USER sMessengerUser[MAX_MESSENGER_USER];
		memset(sMessengerUser, 0, sizeof(MESSENGER_USER)*MAX_MESSENGER_USER);

		byIndex			= 0;
		nColumn			= 1;	
		col				= 0;
		dwParamOffset	= 0;

		ListNode<MESSENGER_USER>* lpMessengerUserPos = lpMessengerTable->pMessengerUserHash->GetHead();

		while(lpMessengerUserPos)
		{
			LP_MESSENGER_USER lpMessengerUser = lpMessengerUserPos->pData;

			if(lpMessengerUser)
			{
				sMessengerUser[byIndex].byType		= lpMessengerUser->byType;
				sMessengerUser[byIndex].dwUserIndex	= lpMessengerUser->dwUserIndex;						
				sMessengerUser[byIndex].wClass		= lpMessengerUser->wClass;
				__lstrcpyn(sMessengerUser[byIndex].szCharName, lpMessengerUser->szCharName, MAX_CHARACTER_NAME_REAL_LENGTH);
				byIndex++;
			}
			lpMessengerUserPos = lpMessengerUserPos->pNext;
		}

		// DB Delete //
		pBinding = g_pDb->THCreateParamInfo((WORD)nColumn);
		
		if(!pBinding)
		{
			Log( LOG_IMPORTANT, "Fail to Create Param Info!" );
			return;
		}

		pBinding[col].obValue	= dwParamOffset;
		pBinding[col].cbMaxLen	= sizeof(sMessengerUser);
		pBinding[col++].wType	= DBTYPE_BYTES;					
		dwParamOffset			+= sizeof(sMessengerUser);		

		sprintf(szQuery, "Delete From Messenger Where Char_Index = %u And Dst_Char_Index = %u", pUser->m_dwUserIndex, pPacket->dwUserIndex);
		g_pDb->THExecuteSQLByParam(szQuery, pBinding, &sMessengerUser, sizeof(sMessengerUser), (BYTE)nColumn, FALSE, NULL, pUser, (BYTE)GAME_DB); // BY DEEPDARK 왜 리턴값을 받겠다는 거냐!!~

		if(lpMessengerTable->pMessengerUserHash->GetCount()==0)
		{
			wsprintf(szQuery, "Update CharacterInfo Set Messenger = 0 Where Char_Index = %u", pUser->m_dwUserIndex);
			g_pDb->THExecuteSQL(szQuery, FALSE, NULL, pUser, (BYTE)GAME_DB);
		}

		CWorldUser* pOnLineUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);

		if(pOnLineUser)
		{
			LP_MESSENGER_TABLE lpMessengerSubTable = g_pMessengerTableHash->GetData(pOnLineUser->m_dwUserIndex);
						
			if(lpMessengerSubTable)
			{
				LP_MESSENGER_USER lpMessengerSubUser = lpMessengerSubTable->pMessengerUserHash->GetData(pUser->m_dwUserIndex);

				if(lpMessengerSubUser)
				{
					pOnlinePacket.byType		= 1;
					pOnlinePacket.dwUserIndex	= pUser->m_dwUserIndex;
					g_pNet->SendToUser(pOnLineUser->m_dwConnectionIndex, (char*)&pOnlinePacket, pOnlinePacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
				}
			}			
		}

		WSTS_FRIENDUSER_DELETE pSendPacket;
		pSendPacket.dwUserIndex	= pPacket->dwUserIndex;
		g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
	}
}

void CmdMessengerInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_FRIENDUSER_INFO* pPacket = (CTWS_FRIENDUSER_INFO*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}

	CWorldUser* pOnLineUser = g_pUserTable->GetUserInfo(pPacket->dwUserIndex);		

	if(pOnLineUser)
	{
		LP_MESSENGER_TABLE lpMessengerSubTable = g_pMessengerTableHash->GetData(pOnLineUser->m_dwUserIndex);
					
		if(lpMessengerSubTable)
		{
			LP_MESSENGER_USER lpMessengerSubUser = lpMessengerSubTable->pMessengerUserHash->GetData(pUser->m_dwUserIndex);

			if(lpMessengerSubUser)
			{				
				DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo((WORD)pOnLineUser->m_dwDungeonID);

				if(!pDungeon)	
				{
					WSTC_FRIENDUSER_INFO pSendPacket;
					memset(pSendPacket.szCharName, 0, sizeof(pSendPacket.szCharName));
					__lstrcpyn(pSendPacket.szCharName, pOnLineUser->m_szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
					pSendPacket.dwUserIndex		= pOnLineUser->m_dwUserIndex;
					pSendPacket.wDungeonID		= 0;
					pSendPacket.byLayerIndex	= 0;
					g_pNet->SendToUser(pUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
				else
				{
					WSTDS_FRIENDUSER_INFO pGuildInfo;							
					pGuildInfo.dwUserIndex		= pUser->m_dwUserIndex;
					pGuildInfo.dwUserIndexRv	= pOnLineUser->m_dwUserIndex;												
					g_pNet->SendToServer(pDungeon->m_pServer->dwConnectionIndex, (char*)&pGuildInfo, pGuildInfo.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
					return;
				}
			}
		}			
	}
}

void CmdMessengerStatus(CWorldUser* pUser, char* pMsg, DWORD dwLength)
{
	CTWS_MESSENGER_STATUS* pPacket = (CTWS_MESSENGER_STATUS*)pMsg;
	if(pPacket->GetPacketSize() != dwLength)
	{
		IllegalSizePacketProc(pPacket->bGameStatus, pPacket->bHeader, dwLength);
		return;
	}
	
	LP_MESSENGER_TABLE lpMessengerTable = g_pMessengerTableHash->GetData(pUser->m_dwUserIndex);
					
	if(lpMessengerTable)
	{
		pUser->m_byMessengerStatus = pPacket->byType;

		ListNode<MESSENGER_USER>* lpMessengerUserPos = lpMessengerTable->pMessengerUserHash->GetHead();

		while(lpMessengerUserPos)
		{
			LP_MESSENGER_USER lpMessengerUser = lpMessengerUserPos->pData;

			if(!lpMessengerUser)
			{
				lpMessengerUserPos = lpMessengerUserPos->pNext;
				continue;
			}

			CWorldUser* pOnLineUser = g_pUserTable->GetUserInfo(lpMessengerUser->dwUserIndex);

			if(!pOnLineUser)
			{
				lpMessengerUserPos = lpMessengerUserPos->pNext;
				continue;
			}

			LP_MESSENGER_TABLE lpMessengerSubTable = g_pMessengerTableHash->GetData(pOnLineUser->m_dwUserIndex);
					
			if(lpMessengerSubTable)
			{
				WSTC_MESSENGER_STATUS pSendPacket;
				pSendPacket.dwUserIndex	= pPacket->dwUserIndex;
				pSendPacket.byType		= pPacket->byType;
				g_pNet->SendToUser(pOnLineUser->m_dwConnectionIndex, (char*)&pSendPacket, pSendPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
			}
			lpMessengerUserPos = lpMessengerUserPos->pNext;
		}
	}			
}
//======================================================//
// End.													//
//======================================================//
