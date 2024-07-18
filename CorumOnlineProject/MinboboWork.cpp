#include "MinboboWork.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "message.h"
#include "CodeFun.h"
#include "CorumResource.h"
#include "DungeonMsg.h"
#include "EffectProcess.h"
#include "DungeonTable.h"
#include "User.h"
#include "UserInterface.h"
#include "DSMiniMap.h"
#include "GameMenuWnd.h"
#include "StoreWnd.h"
#include "Interface.h"
#include "ErrorWnd.h"


void CreateMainPlayerByChangeLayer()
{
	LIGHT_DESC	shadowlight;

	shadowlight.dwDiffuse	= 0xffffffff;
	shadowlight.v3Point.x	= g_pMainPlayer->m_v3CurPos.x + 500.0f;
	shadowlight.v3Point.y	= 800.0f;
	shadowlight.v3Point.z	= g_pMainPlayer->m_v3CurPos.z - 600.0f;
	shadowlight.fFov		= PI/2.0f;
	shadowlight.v3Up.x		= 0.0f;
	shadowlight.v3Up.y		= 1.0f;
	shadowlight.v3Up.z		= 0.0f;
	shadowlight.fRs			= 2000.0f;
	shadowlight.v3To		= g_pMainPlayer->m_v3CurPos;	
	
	g_pMainPlayer->m_hShadowLightHandle = g_pExecutive->CreateGXLight(&shadowlight
		, NULL, NULL, 0, NULL, GXLIGHT_TYPE_ENABLE_SHADOW |	GXLIGHT_TYPE_DISABLE_LIGHT_COLOR);

	g_pExecutive->GXLEnableDynamicLight(g_pMainPlayer->m_hShadowLightHandle);
	
	VECTOR3	v3Offset;
	v3Offset.x = 500.0f;
	v3Offset.y = 1000.0f;
	v3Offset.z = -300.0f;

	g_pExecutive->GXOAttachLight(g_pMainPlayer->m_hPlayer.pHandle
		,"Bip01",g_pMainPlayer->m_hShadowLightHandle,&v3Offset,ATTACH_TYPE_LINK);
}

void ReleaseMainPlayerByChangeLayer()
{
	// 반투명 되있을지도 모르니 정상상태로.
	g_pMainPlayer->UserObjectAlpha(255);
	
	if(g_pMainPlayer->m_hShadowLightHandle)
	{
		g_pExecutive->GXODetachLight(g_pMainPlayer->m_hPlayer.pHandle, g_pMainPlayer->m_hShadowLightHandle);
		g_pExecutive->DeleteGXLight(g_pMainPlayer->m_hShadowLightHandle);
		g_pMainPlayer->m_hShadowLightHandle = NULL;
	}
}

void ToggleUserShadow(CUser* pUser, BYTE bEnable)
{
	CGameMenuWnd* pGameMenuWnd = CGameMenuWnd::GetInstance();

	if(bEnable)
	{
		g_pExecutive->EnableSendShadow(pUser->m_hPlayer.pHandle);
		g_pExecutive->EnableSendShadow(pUser->m_hPlayerHead.pHandle);

		if(pUser->m_hPlayerLHand.pHandle)
			g_pExecutive->EnableSendShadow(pUser->m_hPlayerLHand.pHandle);

		if(pUser->m_hPlayerRHand.pHandle)
			g_pExecutive->EnableSendShadow(pUser->m_hPlayerRHand.pHandle);

		if(!pGameMenuWnd->m_bShadowFlag)	//Mod 그림자 사용시 
			ShowObject(pUser->m_hShadowHandle);			
	}
	else
	{
		g_pExecutive->DisableSendShadow(pUser->m_hPlayer.pHandle);
		g_pExecutive->DisableSendShadow(pUser->m_hPlayerHead.pHandle);

		if(pUser->m_hPlayerLHand.pHandle)
			g_pExecutive->DisableSendShadow(pUser->m_hPlayerLHand.pHandle);

		if(pUser->m_hPlayerRHand.pHandle)
			g_pExecutive->DisableSendShadow(pUser->m_hPlayerRHand.pHandle);

		if(!pGameMenuWnd->m_bShadowFlag)	//Mod 그림자 사용시 	
			HideObject(pUser->m_hShadowHandle);
	}
}


void PortalEffectFunc(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	if(bFrameFlag == FRAME_FLAG_FINISHED )
	{
		CUser*	pUser = (CUser*)pData->pInfo;
		if( !pUser ) return;
		RemovePlayer( pUser );

		pData->ObjectFunc = NULL;		
	}
}


void PortalAppearFuncUser(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	if(bFrameFlag == FRAME_FLAG_FINISHED )
	{
		CUser*	pUser = (CUser*)pData->pInfo;
		
		ToggleUserShadow(pUser, TRUE);
		pUser->SetAction(MOTION_TYPE_WARSTAND);
		pUser->SetStatus(UNIT_STATUS_NORMAL);
		pData->ObjectFunc = NULL;		

		g_bRButton	= FALSE;
		g_bBeltChk	= FALSE;
		g_bGuildChk = FALSE;
		g_bMagicChk	= FALSE;

		if(pUser == g_pMainPlayer)	
		{
			// 조인 했으면 무적 모드 풀기 위한 패킷을 날려라.
			CTDS_FINISH_MAPLOADING packet;
			g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
		}
	}	
}


void ChangeLayerForPortal(DSTC_PORTAL_JOIN_USER* pPacket)
{	
	CMiniMapWnd*	pMiniMapWnd		= CMiniMapWnd::GetInstance();
	CInterface*		pIntreface		= CInterface::GetInstance();
	DWORD			dwTotalProgress = 20;
	DWORD			dwCurProgress	= 0;

	// 월드서버에게 현재 로딩중이라는 정보를 알려준다. 
	SendLoadingStatus(TRUE);		

	g_ItemMoveManager.Initialize();
	g_helpLoadingSprite.Load(pPacket->wDungeonLayer);
	g_helpLoadingSprite.SetLoadingFlag(TRUE);
	
	memset(&g_pMainPlayer->m_GuardianItem, 0, sizeof(CItem));

	ShowLoading();	
			
	for(int i = ITEM_WND; i <= EXIT_WND; i++)
	{
		if(i!=MINIMAP_WND)
			pIntreface->m_pMenu[i]->SetActive(FALSE);
	}

	CUserInterface::GetInstance()->CloseGuardianDescriptionWnd();

	// 반투명 되있을지도 모르니 정상상태로.
	g_pMainPlayer->UserObjectAlpha(255);

	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);
	
	ReleaseMainPlayerByChangeLayer();

	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);	
	
	g_pExecutive->UnloadAllPreLoadedGXObject(UNLOAD_PRELOADED_RESOURCE_TYPE_ONLY_UNLOAD_ENABLED);
	
	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);

	g_pMainPlayer->RemoveResource();
	
	pMiniMapWnd->ReleaseMap();
	pMiniMapWnd->m_wMapId		= pPacket->wDungeonID;
	pMiniMapWnd->m_dwLayerId	= pPacket->wDungeonLayer;
	pMiniMapWnd->CreateMap();

	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);
	
	RemoveAllPlayer();

	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);

	RemoveAllMonster();

	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);
	
	RemoveAllItem();

	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);
	
	RemoveAllEffectDesc();

	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);
		
	InitializeEffect();
	
	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);

	PreLoadCurLayerMonster(pPacket->pdwMonsterKind);
	
	if(pPacket->wDungeonID != (WORD)-1)	
		SetProgressBar(dwTotalProgress, ++dwCurProgress);

	InitMap(pPacket->wDungeonLayer);	
	CreateMainPlayerByChangeLayer();	

	BYTE bMoveType = g_pMainPlayer->m_bMoveType;

	g_pThisDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	InitMainPlayer( (DSTC_CHANGE_LAYER*)pPacket );
	
	CMiniMapWnd::GetInstance()->m_byMapLayer = pPacket->bCurLayerIndex;

	SelectBGM( pPacket->wDungeonID, pPacket->bCurLayerIndex, pPacket->wDungeonLayer );
	
	if(g_pThisDungeon->IsVillage()==TRUE)
	{
		int nIndex = 0;

		CStoreWnd*	pStoreWnd	= CStoreWnd::GetInstance();
		CInterface* pInterface	= CInterface::GetInstance();

		pInterface->m_byServer		= __SERVER_TYPE_VILL__;
		pStoreWnd->m_byServer		= __SERVER_TYPE_VILL__;
		pStoreWnd->m_wSuppliesCount	= 0;
		pStoreWnd->m_wWeaponCount	= 0;
		pStoreWnd->m_wArmorCount	= 0;
		pStoreWnd->m_dwMapId		= pPacket->wDungeonID;

		ListNode<ITEM_STORE>*	lpItemStoreNode;
		LP_ITEM_STORE			lpItemStore;
		LP_ITEM_RESOURCE_EX		pItemResource;
			
		lpItemStoreNode = g_pItemStoreHash->GetHead();

		while(lpItemStoreNode)
		{	
			lpItemStore	= lpItemStoreNode->pData;						

			if(lpItemStore)
			{
				if(lpItemStore->wMapId==0 || lpItemStore->wMapId==pPacket->wDungeonID)
				{
					pItemResource = g_pItemResourceHash->GetData(lpItemStore->wItemId);

					if(pItemResource)
					{						
						pStoreWnd->m_wItemId[nIndex] = lpItemStore->wItemId;					

						if(lpItemStore->byType==STORE_TYPE_SUSPZO)
							pStoreWnd->m_wSuppliesCount++;
						else
						{
							if(lpItemStore->byType==STORE_TYPE_WEAPON)
								pStoreWnd->m_wWeaponCount++;
							else if(lpItemStore->byType==STORE_TYPE_ARMOR)
								pStoreWnd->m_wArmorCount++;							
						}

						nIndex++;
					}
				}
			}

			lpItemStoreNode = lpItemStoreNode->pNext;
		}		
	}

	g_pMainPlayer->m_bMoveType	= bMoveType;

	// 서버에서 이동한 던전에 나타나라고 메시지 날라올때까지 백화면이 보여라.
	SPR(SPR_LOADING_BACK)->ShowSprite(TRUE);

	for(i=0; i<20; i++)
	{
		SPR(SPR_LOADING_BLOCK_1 + i)->ShowSprite(TRUE);
	}

	SPR(SPR_LOADING_BLOCK_HEAD)->ShowSprite(TRUE);
	SPR(SPR_LOADING_BLOCK_HEAD)->vPos.x = (float)(394 + (9*20));	

	DungeonEnvironmentSetting(g_pThisDungeon);

	// 월드서버에게 현재 로딩중이라는 정보를 알려준다. 
	SendLoadingStatus(FALSE);		
}


void PortalEffectFuncInsideMainPlayer(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	if(bFrameFlag == FRAME_FLAG_FINISHED )
	{
		PostMessage(g_hMainWnd, UM_PORTAL_CHANGE_LAYER, (WPARAM)pData->dwTempBuf[0], NULL );
		
		pData->dwTempBuf[0] = 0;
		pData->ObjectFunc = NULL;
	}
}


void PortalEffectFuncMainPlayer(GXOBJECT_HANDLE handle, LPObjectDesc pData, DWORD dwCurFrame, BYTE bFrameFlag)
{
	if(bFrameFlag == FRAME_FLAG_FINISHED )
	{
		_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_TELEPORT, g_Camera.v3CameraPos, FALSE);
		
		CUser*	pUser = (CUser*)pData->pInfo;

		DSTC_PORTAL_MOVE_OUTSIDE* pPacket = (DSTC_PORTAL_MOVE_OUTSIDE*)pData->dwTempBuf[0];
		
		ShowLoading();

		char szIP[16];
		ChangeAddressType(pPacket->dwConnectIP, szIP);
		DWORD dwConnectionIndex = g_pNet->ConnectToServer(szIP, pPacket->wPort);

		if(!dwConnectionIndex)
		{
			SPR(SPR_LOADING_BACK)->ShowSprite(FALSE);
			//메세지 
			CErrorWnd* pError = CErrorWnd::GetInstance();
			pError->SetError(g_Message[ETC_MESSAGE526].szMessage,"","", 1); // "포탈이동에 실패했습니다.!"
			pError->SetActive();
			pUser->SetStatus(UNIT_STATUS_NORMAL);
			
			delete pPacket;
			pData->dwTempBuf[0] = 0;
			pData->ObjectFunc = NULL;
			HideObject(handle);

			return;
		}
		
		g_pNet->DisconnectServer( SERVER_INDEX_ZONE );
		g_dwConnectIndex[ SERVER_INDEX_ZONE ] = dwConnectionIndex;
		g_pGVDungeon->bMainUserPotalJoin = TRUE;
		
		CTDS_DUNGEON_JOIN join;
		memset(join.szChrName, 0, sizeof(join.szChrName));	
		
		join.dwPartyID					= g_pMainPlayer->m_dwPartyId;
		join.dwUserIndex				= g_pMainPlayer->m_dwUserIndex;
		join.wDungeonID					= pPacket->wToDungeonID;		
		join.bIsPortalJoin				= 1;
		join.bLayerNum					= pPacket->bLayerNo;
		join.v2Pos						= pPacket->v2Pos;

		if(g_pThisDungeon != NULL)
		{
			g_pMainPlayer->wPrevDungeonID	= join.wPrevDungeonID				= g_pThisDungeon->m_dwID;			
		}
		
		__lstrcpyn( join.szChrName, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH + 1);
				
		g_pNet->SendMsg( (char*)&join, join.GetPacketSize(), SERVER_INDEX_ZONE );	

		delete pPacket;
		pData->dwTempBuf[0] = 0;
		pData->ObjectFunc = NULL;
		HideObject(handle);		

		LP_PARTY_USER sPartyNode = g_pPartyUserHash->GetData( pUser->m_dwUserIndex );

		if(sPartyNode)
		{
			CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();

			VECTOR2 vec = pMiniMapWnd->GetRatioMinimap(pUser->m_v3CurPos.x, pUser->m_v3CurPos.z);
			pMiniMapWnd->SetPosObjX(sPartyNode->dwResourceId, vec.x+8);
			pMiniMapWnd->SetPosObjY(sPartyNode->dwResourceId, vec.y+42);
		}
	}
}

void ReleaseDungeonEffect()
{
	POSITION_ pos = g_pDungeonTable->m_pReceivedDungeonList->GetHeadPosition();

	while(pos)
	{
		DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);
		g_pDungeonTable->RemoveDungeonEffect(pDungeon);
	}
}