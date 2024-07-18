#include "WorldMsg_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "DungeonTable.h"
#include "InitGame.h"
#include "GameControl.h"
#include "message.h"
#include "Effect.h"
#include "CorumResource.h"
#include "CDungeonInfoWnd.h"
#include "ObjectManager.h"
#include "Chat.h"
#include "DungeonSiegeInfoWnd.h"
#include "Map.h"
#include "EffectProcess.h"
#include "CodeFun.h"

void CmdReplyDungeonInfo( char* pMsg, DWORD dwLen )
{
	WSTC_REPLY_DUNGEON_INFO* pPacket = (WSTC_REPLY_DUNGEON_INFO*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	
	if (!pDungeon)	return;
	pDungeon->m_dwOwnerGuildNum = pPacket->dwOwnerGuildNum;
	pDungeon->SetOperationMode(pPacket->bOperationType);

	pDungeon->m_wGroupID = pPacket->wGroupID;
	pDungeon->m_bSiege = pPacket->bSiege;
	pDungeon->m_dwOwnerGuildNum = pPacket->dwOwnerGuildNum;
	pDungeon->m_wProduction = pPacket->wProduction;
	pDungeon->m_byProductionCount = pPacket->byProductionCount;

	pDungeon->m_wAreaLevel_h = pPacket->wAreaLevel_h;
	pDungeon->m_wInitLevel = pPacket->wInitLevel;
	pDungeon->m_wUpgradeLevel = pPacket->wUpgradeLevel;
	pDungeon->m_wLayerCount = pPacket->wLayerCount;
	pDungeon->SetOwner(pPacket->dwOwnerIndex, pPacket->dwOwnerGuildNum, pPacket->szOwner, pPacket->szManager);
	memcpy(&pDungeon->m_cMagicFieldArrayItem, &pPacket->cMagicArrayItem, sizeof(CItem));
	memcpy(&pDungeon->m_cGuardianItem, &pPacket->m_cGuardianItem, sizeof(CItem));
	pDungeon->m_byDefenseCount = pPacket->byDefenseCount;
	pDungeon->m_byProperty = pPacket->byProperty;
	pDungeon->m_dwEntrance = pPacket->dwEntrance;
	pDungeon->m_dwAccEntrance = pPacket->dwAccEntrance;
	pDungeon->m_dwAccExp	 = pPacket->dwAccExp;
	pDungeon->m_byEntererMaxLevel = pPacket->byEntererMaxLevel;
	pDungeon->m_byEntererMinLevel = pPacket->byEntererMinLevel;
	lstrcpy(pDungeon->m_szManager, pPacket->szManager);
	
#if IS_JAPAN_LOCALIZING()
	if(lstrlen(pPacket->szSchoolName))
		__lstrcpyn(pDungeon->m_szSchoolName, pPacket->szSchoolName, sizeof(pDungeon->m_szSchoolName));
	else
		memset(pDungeon->m_szSchoolName, 0, sizeof(pDungeon->m_szSchoolName));
#endif
	
	memcpy(pDungeon->m_szMemo, pPacket->szMemo, pPacket->bMemoLength);
 	
	pDungeon->m_dwRemainSiegeStartTick	= pPacket->dwRemainSiegeStartTick + g_dwCurTick;	//공성 시작시간 
	pDungeon->m_dwRemainLevelStartTick	= pPacket->dwRemainLevelStartTick+ g_dwCurTick;
	pDungeon->m_dwRemainSiegeEndTick	= pPacket->dwRemainSiegeEndTick + g_dwCurTick;

	pDungeon->bReceivedInfo = 1;
	pDungeon->pReceivedDungeonPos = g_pDungeonTable->m_pReceivedDungeonList->AddTail( pDungeon );

	if (pDungeon->IsConquer())
	{
		if (pDungeon->m_bSiege)
			pDungeon->StopOperationTime();		
	}
	
	// 내가 들어와있는 던전에 해당하는 정보만 셋팅하라.	
	g_pDungeonTable->AddListDungeonGroup(pDungeon, pPacket->wGroupID);
	CDungeonInfoWnd::GetInstance()->InsertDungeonInfo(pDungeon);		
}

void CmdRefreshEventDungeon_StartSiege(char* pMsg, DWORD dwLen)
{
	// 공성 시작 했으니깐 공성 끝날 시간 셋팅.
	WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE* pPacket = (WSTC_REFRESHEVENT_DUNGEON_STARTSIEGE*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
		
	pDungeon->SetSiegeEndDestTime(pPacket->dwSiegeEndTick);
	pDungeon->StopOperationTime();
	
	// 월드에 있을때만 보여줘
	if (!g_pThisDungeon) 
	{
		if(!pDungeon->m_hSiegingDungeon)
		{
			pDungeon->m_hSiegingDungeon = 
				CreateHandleObject( g_pObjManager->GetFile((DWORD)DUNGEON_SIEGING), NULL, NULL, GXOBJECT_CREATE_TYPE_EFFECT|GXOBJECT_CREATE_TYPE_DEFAULT_PROC );//KUK
		}
		
		GXSetPosition(pDungeon->m_hSiegingDungeon, &pDungeon->vPos, FALSE);
		ShowObject(pDungeon->m_hSiegingDungeon);
	}
	
	if (pDungeon->IsConquer())
	{		
		if (g_pThisDungeon == pDungeon)
		{
			// 현재 내가 접속한 던전정보와 일치하군.			
			DisplayMessageAdd(g_Message[ETC_MESSAGE432].szMessage, 0xFF21DBFF);	// MSG_ID : 432 ; 공성전 시작.
			CDungeonSiegeInfoWnd::GetInstance()->SetActive(TRUE);

			POSITION_ pos = g_pMap->m_pCPList->GetHeadPosition();

			while(pos)
			{
				CP_DESC* pCPDesc = (CP_DESC*)g_pMap->m_pCPList->GetNext(pos);
					
				pCPDesc->pSpr_Minimap_Dot->ShowSprite(TRUE);
				pCPDesc->pSpr_Cp_GuageBar->ShowSprite(FALSE);
				pCPDesc->pSpr_Cp_GuageBoard->ShowSprite(FALSE);
				pCPDesc->dwStartTime = 0;
				HideObject(pCPDesc->pEffectDesc->hEffect.pHandle);
								
				pCPDesc->pEffectDesc->hEffect.pDesc->ObjectFunc = NULL;				
			}

			g_pMap->m_bCPRemainCount = BYTE(g_pMap->m_pCPList->GetCount());
			
		}
	}
}

void CmdRefreshEventDungeon_EndSiege(char* pMsg, DWORD dwLen)
{
	// 공성 끝났으니깐 다시 공성 시작할 시간을 셋팅.
	WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE* pPacket = (WSTC_REFRESHEVENT_DUNGEON_ENDSIEGE*)pMsg;
	DUNGEON_DATA_EX*	pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	
	pDungeon->SetSiegeStartDestTime(pPacket->dwSiegeStartTick);
	pDungeon->ResumeOperationTime();
	if (pDungeon->m_hSiegingDungeon)
	{
		DeleteHandleObject( pDungeon->m_hSiegingDungeon );
		pDungeon->m_hSiegingDungeon = NULL;
	}
	

	if (g_pThisDungeon == pDungeon)
	{
		// 현재 내가 접속한 던전정보와 일치하군.				
		DisplayMessageAdd(g_Message[ETC_MESSAGE433].szMessage, 0xFF21DBFF);	// MSG_ID : 433 ; 공성전 끝.
		CDungeonSiegeInfoWnd::GetInstance()->SetActive(FALSE);
		
		POSITION_ pos = g_pMap->m_pCPList->GetHeadPosition();
		while(pos)
		{
			CP_DESC* pCPDesc = (CP_DESC*)g_pMap->m_pCPList->GetNext(pos);
				
			pCPDesc->pSpr_Minimap_Dot->ShowSprite(FALSE);
			pCPDesc->pSpr_Cp_GuageBar->ShowSprite(FALSE);
			pCPDesc->pSpr_Cp_GuageBoard->ShowSprite(FALSE);
			pCPDesc->dwStartTime = 0;
			HideObject(pCPDesc->pEffectDesc->hEffect.pHandle);
							
			pCPDesc->pEffectDesc->hEffect.pDesc->ObjectFunc = NULL;
		}

		if (pPacket->bDefenseSuccess)
		{
			ListNode<CUser>*	pNode;
			pNode = g_pUserHash->GetHead();

			while( pNode )
			{			
				if( pNode->pData->m_bAttackMode == ATTACK_MODE_DEFENSE)
				{
					EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_DUNGEON_GUARD_SUCCESS), g_pMainPlayer == pNode->pData, __CHR_EFFECT_NONE__);
					
					pEffectDesc->dwTargetIndex[0] = pNode->pData->m_dwUserIndex;
					pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_PLAYER;
					::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
					pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillUserAroundRemoveFunc;
				}

				pNode = pNode->pNext;
			}
		}
		
	}
	else if (!g_pThisDungeon)
	{
		// 월드에 있을때 이펙트 보여주기.
		EffectDesc* pEffectDesc = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_DUNGEONDEFENSESUCCESS_WORLD), 1, __CHR_EFFECT_NONE__);
		
		pEffectDesc->dwTargetIndex[0] = pDungeon->m_dwID;
		pEffectDesc->byTargetObjectType[0]	= OBJECT_TYPE_SUMMON;
		::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE );
		pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillUserAroundRemoveFunc;
	}
}

void CmdRefreshEventDungeon_Change_Owner_Guild( char* pMsg, DWORD dwLen )
{
	WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD* pPacket = (WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER_GUILD*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(pDungeon)
	{
		pDungeon->SetOwner(pDungeon->m_dwOwnerIndex, pPacket->dwGuildId, pDungeon->m_szOwner, pPacket->szGuildName);
	}
}
void CmdRefreshEventDungeon_AccEntrance_Acquisition( char* pMsg, DWORD dwLen )
{
	WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION* pPacket = (WSTC_REFRESHEVENT_DUNGEON_ACCENTRANCE_ACQUISITION*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(pDungeon)
	{
		pDungeon->m_dwAccEntrance = pPacket->dwMoney;
	}
}
void CmdRefreshEventDungeon_OperationMode( char* pMsg, DWORD dwLen )
{
	WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE* pPacket = (WSTC_REFRESHEVENT_DUNGEON_OPERATIONMODE*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(pDungeon)
	{
		pDungeon->SetOperationMode(pPacket->byOperationMode);
	}
}
void CmdRefreshEventDungeon_Entrance_Edit( char* pMsg, DWORD dwLen )
{
	WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT* pPacket = (WSTC_REFRESHEVENT_DUNGEON_ENTRANCE_EDIT*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(pDungeon)
	{
		pDungeon->m_dwEntrance = pPacket->dwMoney;
	}
}

void CmdRefreshEventDungeon_AccExp_Acquisition( char* pMsg, DWORD dwLen )
{
	WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION* pPacket = (WSTC_REFRESHEVENT_DUNGEON_ACCEXP_ACQUISITION*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(pDungeon)
	{
		pDungeon->m_dwAccExp = pPacket->dwAccExp;
	}
}

// 던전 가디언 바꼇다.
void CmdRefreshEventDungeon_GuardianItem(char* pMsg, DWORD dwLen)
{
	WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM* pPacket = (WSTC_REFRESHEVENT_DUNGEON_GUARDIANITEM*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(pDungeon)
	{
		memcpy(&pDungeon->m_cGuardianItem, &pPacket->cGuardianItem, sizeof(CItem));
	}
}

// 던전 마법진 바꼇다.
void CmdRefreshEventDungeon_MagicFieldArrayItem(char* pMsg, DWORD dwLen)
{
	WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY* pPacket = (WSTC_REFRESHEVENT_DUNGEON_MAGICFIELDARRAY*)pMsg;
	DUNGEON_DATA_EX* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if(pDungeon)
	{
		memcpy(&pDungeon->m_cMagicFieldArrayItem, &pPacket->cMagicFieldArrayItem, sizeof(CItem));
	}
}


// 던전 주인 바꼇다.
void CmdRefreshEventDungeon_Change_Owner(char* pMsg, DWORD dwLen)
{
	WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER* pPacket = (WSTC_REFRESHEVENT_DUNGEON_CHANGE_OWNER*)pMsg;
	DUNGEON_DATA_EX*	pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	ListNode<CUser>* pUserNode =  g_pUserHash->GetHead();

	while (pUserNode)
	{
		if (pUserNode->pData->m_pOwnerEffect)
		{
			RemoveEffectDesc(pUserNode->pData->m_pOwnerEffect);
			pUserNode->pData->m_pOwnerEffect = NULL;
			break;
		}
		pUserNode = pUserNode->pNext;
	}
	
	pDungeon->SetOwner(pPacket->dwUserIndex, pPacket->dwGuildId, pPacket->szOwner, pPacket->szGuildName);
		
	if (g_pThisDungeon)
	{		
		if (g_pThisDungeon == pDungeon)
		{
			// 던준 주인 이팩트 주기.
			CUser* pOwner = g_pUserHash->GetData(pPacket->dwUserIndex);
			if (pOwner)
			{
				pOwner->DungeonOwnerEffect();
			}
		}
	}
}


void CmdRefreshEventDungeon_LevelUp(char* pMsg, DWORD dwLen)
{
	// 레벨업 될 시간.
	WSTC_REFRESHEVENT_DUNGEON_LEVELUP* pPacket = (WSTC_REFRESHEVENT_DUNGEON_LEVELUP*)pMsg;
	DUNGEON_DATA_EX*	pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	
	pDungeon->m_dwRemainLevelStartTick = pPacket->dwLevelUpDestTick+g_dwCurTick;

#ifdef DEVELOP_MODE
	DisplayMessageAdd(g_Message[ETC_MESSAGE435].szMessage, 0xFF21DBFF);	// MSG_ID : 435 ; 던전 레벨업.
#endif
}


void CmdRefreshEventDungeon_CreateProduction(char* pMsg, DWORD dwLen)
{
	// 아이템 만들어질 시간.
	WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION* pPacket = (WSTC_REFRESHEVENT_DUNGEON_CREATEPRODUCTION*)pMsg;
	DUNGEON_DATA_EX*	pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	pDungeon->m_byProductionCount = pPacket->byProductionCount;
	pDungeon->m_wProduction = pPacket->wProduction;
}


void CmdRefreshDefenseCount(char* pMsg, DWORD dwLen)
{
	WSTC_REFRESHEVENT_DEFENSECOUNT* pPacket = (WSTC_REFRESHEVENT_DEFENSECOUNT*)pMsg;
	DUNGEON_DATA_EX*	pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);

	pDungeon->m_byDefenseCount = pPacket->byDefenseCount;
}