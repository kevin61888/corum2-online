#include "stdafx.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DungeonTable.h"
#include "Dungeon.h"
#include "OwnServer.h"
#include "DungeonLayer.h"
#include "DUNGEON_DATA_EX.h"
#include "ServerFunction.h"
#include "AcceptTable.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "recvmsg.h"
#include "DBProc.h"
#include "RecordSet.h"
#include "GameSystemNative.h"


void CmdRequestUserAcceptForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL* pPacket = (DSWDS_REQUEST_USER_ACCEPT_FOR_PORTAL*)pMsg;

	ACCEPT_USER* pUser = (ACCEPT_USER*)0xffffffff;

	while(pUser)
	{
		pUser = g_pAcceptTable->GetAcceptInfo(pPacket->dwUserIndex);

		if(pUser)
		{
			g_pAcceptTable->Remove(pUser);
			Log(LOG_IMPORTANT, "Request UserIndex is already accepted! (CmdRequestUserAcceptForPortal Function!)");
		}
		else 
		{
			goto lb_Continue;	
		}
	}

lb_Continue:
	
	// 접속 허용 명단에 추가 
	pUser = g_pAcceptTable->AllocNewAccept(pPacket->wDungeonID, pPacket->dwUserIndex);
	pUser->bConnectToGameRoom = pPacket->bConnectionToGameRoom;

	if(!g_pAcceptTable->Add(pUser))
	{
		Log(LOG_IMPORTANT, "Adding User to AcceptTable is Failed! (CmdRequestUserAcceptForPortal Function!)");

		DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL FailPacket;
		FailPacket.wFromDungeonID = pPacket->wFromDungeonID;
		FailPacket.dwUserIndex = pPacket->dwUserIndex;
		FailPacket.bErrorCode = 3;				//Accept Table에 Add 실패 
		g_pNet->SendToServer(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		return;
	}
	
	// 다시 던전서버에게 접속할 Address와 함께 알림 
	DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL packet;	
	packet.wFromDungeonID = pPacket->wFromDungeonID;
	packet.dwUserIndex = pPacket->dwUserIndex;
#if defined TAIWAN_LOCALIZING
	// 대만은 내부아이피로 유저리슨하기 때문에 외부 IP를 유저리슨 IP로 본다.
	packet.dwConnectIP = inet_addr(g_pThis->GetIPForExtern());
#else
	packet.dwConnectIP = inet_addr(g_pThis->GetIPForUser());
#endif
	packet.wPort = g_pThis->GetPortForUser();

	// Append by seven / [12/9/2004]
	// GM 명령 관련하여 던전이동시
	// 타던전 던전 포탈시 위치를 모를경우가 있다.
	// 이를 위해서 이동할 던전 위치를 알아서 보내준다.
	if (255 != pPacket->bLayerIndex)
	{
		if (Portal_Type_GM == pPacket->bPortalType)
		{
			// 다시말해서 특정던전의 0번 스폿으로 갈놈이다.
			CDungeon * pDungeonDest = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
			if (pDungeonDest )
			{
				CDungeonLayer * pLayer = pDungeonDest->GetDungeonLayer(pPacket->bLayerIndex);
				if (pLayer)
				{
					pLayer->GetStartTile(0, &packet.v2Pos);
				}
			}
		}
		else
		{
			// 일반 / NPC 포탈 이동이다.
			// 포탈 정보가 제대로 들어간 상태일 것이다. 위치부분을 알고있기 때문에 
			// 아무것도 넣지 않아도 된다. 
		}
	}
	else
	{
		// pUser의 위치로 갈놈이다.
		CUser * pUser = g_pUserHash->GetData(pPacket->dwTargetUserIndex);
		if (!pUser)
		{
			// 대상 유저가 없다. 
			// 이런 경우는 흔치 않겠지만 있을수 있다.
			// bErrorCode를 추가하자
			DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL FailPacket;
			FailPacket.wFromDungeonID = pPacket->wFromDungeonID;
			FailPacket.dwUserIndex	  = pPacket->dwUserIndex;
			FailPacket.bErrorCode	  = 3;				//Accept Table에 Add 실패 
			g_pNet->SendToServer(dwConnectionIndex, (char*)&FailPacket, FailPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);
		}

		packet.bLayerIndex = pUser->GetCurLayerIndex();

		packet.v2Pos.x = pUser->GetCurPosition()->x;
		packet.v2Pos.y = pUser->GetCurPosition()->y;
	}
	
	g_pNet->SendToServer(dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );	
}


void CmdUserAcceptAllowedForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL* pPacket = (DSWDS_USER_ACCEPT_ALLOWED_FOR_PORTAL*)pMsg;

	CUser* pUser = g_pUserHash->GetData( pPacket->dwUserIndex );

	if(pUser)
	{
		if(pUser->pBufForPortal)
		{			
			PORTAL_BUCKET* pPortal = (PORTAL_BUCKET*)(pUser->pBufForPortal);
			pPortal->dwConnectIP = pPacket->dwConnectIP;
			pPortal->wPort = pPacket->wPort;

			// 포탈 이동할 위치 지정 
			if (0 == pPortal->wPortalItemID &&
				0.f == pPortal->v2Pos.x && 0.f == pPortal->v2Pos.y)
			{ 
				// 일반포탈이 아니다. 
				pPortal->v2Pos = pPacket->v2Pos;	// append by seven / [12/9/2004]
			}


			if (255 == pPortal->bLayerNo)
			{
				// 나중에 값을 받기위해서 층을 255로 셋해두었으니 
				// 제대로된 층수를 입력해준다.
				pPortal->bLayerNo = pPacket->bLayerIndex;				
			}

			pUser->ChangeGodMode(GODMODE_STATUS_MAPLOADING);

			// 유저 아이템 저장이 끝난후에 최종적으로 포탈 이동하라는 패킷을 날리자. 
			UpdateUserDBThread(pUser);	
			// sung-han 2005-02-28 포탈들어가는 이펙트 안나오는것 수정-----------------------------
			// 같은 포트(서버)만 처리돼어 있던것을 다른 포트도 적용돼게 수정
			CDungeon * pDungeonDest = g_pDungeonTable->GetDungeonInfo(pUser->GetCurDungeonID());
			pDungeonDest->RemoveUserForInsidePortal(pUser);
			// ------------------------------------------------------------------------------------

			Log( LOG_JUST_FILEWRITE, "UpdateUserDB Name : %s, Level : %u, Exp : %u", pUser->m_szName, pUser->GetLevel(), pUser->GetExp());
		}
		else
		{			
			SendPortalFailMessage(pUser, 0, 4, 0);						
			Log(LOG_IMPORTANT, "Portal info is not found at CmdUserAcceptAllowedForPortal");
		}
	}
}


void CmdUserAcceptFailedForPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL* pPacket = (DSWDS_USER_ACCEPT_FAILED_FOR_PORTAL*)pMsg;

	CUser* pUser = g_pUserHash->GetData( pPacket->dwUserIndex );

	if(pUser)
	{	
		// 길드전 또는 PVP중일때 에러코드가 254로 날아오면, 이전던전의 월드좌표로 이동시켜 준다.
		//
		if( pPacket->bErrorCode == 254 )
		{
			WORD	wRecentJoinDungeonID = pUser->m_wRecentJoinDungeonID;
			
			if( wRecentJoinDungeonID > 2000 && wRecentJoinDungeonID < 3000 )
			{
				pUser->m_bDestWorldSpot = (wRecentJoinDungeonID % 2) == 0;
                pUser->m_wRecentJoinDungeonID -= pUser->m_bDestWorldSpot;                
			}

			// Remark
			pUser->GoToWorld( pUser->m_bDestWorldSpot, GOTOWORLD_STATUS_GOTOWORLD_OUTSTATUS_GUILDWAR/*USER_TEMP_GOTOWORLD_OUTSTAUS_GUILDWAR*/ );
		}
		else
		{
			pUser->m_bPortalMoving = 0;

			if(pUser->pBufForPortal)
			{
				PORTAL_BUCKET* pPortal = (PORTAL_BUCKET*)(pUser->pBufForPortal);
				SendPortalFailMessage(pUser, pPortal->wToDungeonID, pPacket->bErrorCode, pPortal->wPortalItemID );
				
				LALFree(g_pPortalBufferPool, pUser->pBufForPortal);
				pUser->pBufForPortal = NULL;
			}
			else
			{
				SendPortalFailMessage(pUser, 0, 4, 0);
				Log(LOG_IMPORTANT, "Portal info is not found at CmdUserAcceptFailedForPortal");
			}
		}
	}
}

void CmdUserPortal(DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)//: hwoarang 050129
{
	WSTDS_USER_PORTAL* pPacket = (WSTDS_USER_PORTAL*)pMsg;

	CUser * pPortalUser		= (CUser *)g_pUserHash->GetData(pPacket->dwUserIndex);
	CUser * pTargetToUser	= (CUser *)g_pUserHash->GetData(pPacket->dwTargetToUserIndex); 

	DSTC_USER_PORTAL pSendPacket;
	
	DUNGEON_MOVE_INFO &rSendDMI = pSendPacket.DMI;

	if (!pPortalUser)
	{
		return;
	}

	pPortalUser->m_ePortalType = Portal_Type_GM;	// GM 에 의한 포탈이다.

	switch( pPacket->byGMCommand )
	{
	case GM_Command_Get_Recall_Someone:
		{
			rSendDMI = pPacket->DMI;	// 이동할 던전 정보

			NetSendToUser(pPortalUser->m_dwConnectionIndex, 
				(char*)&pSendPacket, pSendPacket.GetPacketSize(),
				FLAG_SEND_NOT_ENCRYPTION);
		}
		break;
	case GM_Command_Get_Move_Someone:
		{		
			rSendDMI = pPacket->DMI;	// 이동할 던전 정보

			if (pTargetToUser)
			{
				// 같은 포트의 던전 포탈이다
				rSendDMI.byLayerIndex = pTargetToUser->m_bInLastLayer;	// 층 
				rSendDMI.wX = (WORD)pTargetToUser->GetCurPosition()->x;				// 위치 
				rSendDMI.wY = (WORD)pTargetToUser->GetCurPosition()->y;
			}
			else
			{
//				rSendDMI.byLayerIndex = pPacket->byLayerIndex;	// 층 
				rSendDMI.wX = 0;									// 위치를 모른다. 
				rSendDMI.wY = 0;
				pSendPacket.dwTargetUserIndex = pPacket->dwTargetToUserIndex;
			}

			NetSendToUser(pPortalUser->m_dwConnectionIndex, 
				(char*)&pSendPacket, pSendPacket.GetPacketSize(),
				FLAG_SEND_NOT_ENCRYPTION);
		}

		break;
	case GM_Command_Dungeon_Portal:
		{
			rSendDMI = pPacket->DMI;	// 이동할 던전 정보

//			rSendDMI.wDungeonID		= pPacket->wDungeonID;	// 이동할 던전
		
//			rSendDMI.byLayerIndex = pPacket->byLayerIndex;	// 층 
//			rSendDMI.wX = (WORD)pPacket->wX;	// 위치 
//			rSendDMI.wY = (WORD)pPacket->wY;

			NetSendToUser(pPortalUser->m_dwConnectionIndex, 
				(char*)&pSendPacket, pSendPacket.GetPacketSize(),
				FLAG_SEND_NOT_ENCRYPTION);
		}
		break;
	}
}
