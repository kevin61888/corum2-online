#include "MsgProc_for_yak.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "MonsterTable.h"
#include "Monster.h"
#include "DungeonTable.h"
#include "OwnServer.h"
#include "recvmsg.h"
#include "GameSystemNative.h"
#include "EventDungeonManager.h"


void InitializePacketProcYak()
{
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_SKILL ]							= CmdSkill;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_PKMODE ]					= CmdPKMode;		
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_DUNGEON_CHANGE_LRSKILLKIND]		= CmdDungeonSkill_ChangeLR;	
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_FINISH_MAPLOADING ]				= CmdFinishMapLoading;		
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ACCENTRANCE_ACQUISITION]			= CmdAccEntrance_Acquisition;		
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_PRODUCT_ACQUISITION]				= CmdProduct_Acquisition;		
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ACCEXP_ACQUISITION]				= CmdAccExp_Acquisition;		
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_ENTRANCE_EDIT]					= CmdEntrance_Edit;			
	PacketProc[ UPDATE_GAME_PLAY ][ Protocol_CTS::CMD_USER_DIE_OK]						= CmdUserDieOK;		
}


void CmdDungeonSkill_ChangeLR( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength)
{
	CTDS_DUNGEON_CHANGE_LRSKILLKIND* pPacket = (CTDS_DUNGEON_CHANGE_LRSKILLKIND*)pMsg;
	CUser*		pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if( !pUser ) return;
	

	pUser->SetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON
		, pPacket->bySelectSkillKind[SELECT_ATTACK_TYPE_LBUTTON]);
	pUser->SetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON
		, pPacket->bySelectSkillKind[SELECT_ATTACK_TYPE_RBUTTON]);
	
	if (pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON) == 0)
		pUser->SetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON
		, __SKILL_NONE_SELECT__);
	
	DSTC_USER_STATUS	UserStatus;
	WORD wMinMax[2] = {0, 0};
	pUser->GetAttackDamage(pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)
		, pUser->GetSkillLevel(pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON))
		, BYTE(pUser->GetCurrentWeaponID()/ITEM_DISTRIBUTE)
		, &wMinMax[0], &wMinMax[1], (BYTE)SELECT_ATTACK_TYPE_LBUTTON);
	
	pUser->SetAttackDamage_L(wMinMax[0], wMinMax[1]);

	pUser->GetAttackDamage(pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)
		, pUser->GetSkillLevel(pUser->GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON))
		, BYTE(pUser->GetCurrentWeaponID()/ITEM_DISTRIBUTE)
		, &wMinMax[0], &wMinMax[1], (BYTE)SELECT_ATTACK_TYPE_RBUTTON);

	pUser->SetAttackDamage_R(wMinMax[0], wMinMax[1]);

	// Send changed AttackDamage
	UserStatus.bStatusMany = 2;
	UserStatus.pStatus[0].dwCode	= USER_ATTACK_L;
	UserStatus.pStatus[0].dwMin		= pUser->GetAttackDamageMin_L();
	UserStatus.pStatus[0].dwMax		= pUser->GetAttackDamageMax_L();

	UserStatus.pStatus[1].dwCode	= USER_ATTACK_R;
	UserStatus.pStatus[1].dwMin		= pUser->GetAttackDamageMin_R();
	UserStatus.pStatus[1].dwMax		= pUser->GetAttackDamageMax_R();
	
	NetSendToUser( dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );
}


void CmdPKMode( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_PKMODE*	pPacket = (CTDS_DUNGEON_PKMODE*)pMsg;
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
		
	if(!pUser)																				return;
	if(!pUser->GetCurDungeon())																return;
	if(pUser->GetCurLayerIndex() == INVALID_DUNGEON_LAYER)									return;
	if(pUser->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType() == DUNGEON_TYPE_EVENT)	return;
	
	// pk 모드를 원하는군.
	if (pPacket->bPKMode) 
	{
		// 만약 GM이 PK모드 제한을 풀어 놓왔다면..
		if( !pUser->GetCurDungeon()->GetTemp(DUNGEON_TEMP_PKMODE))
		{
			if (pUser->IsPKZone())
			{
				pUser->SetPKMode(true);
			}
			else 
			if (pUser->GetCurLayerIndex() && pUser->m_sMatchDescInfo.m_bMatching && 
				pUser->GetCurDungeonLayer()->m_bStartMatch)	
			{
				// 대전 레이어 에서 1:1대전이 시작되었다.
				pUser->SetPKMode(pPacket->bPKMode);
			}
		}
		else
		{
			pUser->SetPKMode(pPacket->bPKMode);
		}	
	}
	else
	{
		if (pUser->GetCurLayerIndex() && pUser->m_sMatchDescInfo.m_bMatching )
		{
			// 대전중인데 평화 모드로 할려고 한다. 	
			// 대전모드에서 실수로 평화 모드로 바꿀경우 초보 유저들의 
			// 문제가 발생 할 수 있다는 내용이 있어서 고침. 
			// 기획과도 상의 됨 
			// by seven
			// 무조건 평화모드는 안된다.
			return;		
		}

		pUser->SetPKMode(pPacket->bPKMode);
	}
}

void CmdDungeonSkillCasting( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_DUNGEON_CASTING*	pPacket = (CTDS_DUNGEON_CASTING*)pMsg;
	
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);		
	if(!pUser) return;

	CDungeonLayer*pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer ) return;

	BYTE	bSkillKind = pUser->GetSelectedSkill((SELECT_ATTACK_TYPE)pPacket->bSkillKindLR);
	char	bSkillLevel = char(pUser->GetSkillLevel(bSkillKind)-1);
	if(bSkillLevel<0)	return;
	
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);
	
	if (g_dwTickCount - pUser->m_dwStartSkillTick[bSkillKind] < pEffect->dwCoolTime)
	{
		// 쿨포인트 계산하기.
		int nCool = INT(pUser->m_fCurCoolPoint*1000-pEffect->dwCoolTime);
	
		if(nCool<0)
		{
			bSkillKind = 0;
			goto lbl_fail;
		}
				
		pUser->m_fCurCoolPoint = max(nCool/1000.f, 0.1F);		
	}

	if (pEffect->bID != __SKILL_AURARECHARGE__ 
		&& (pEffect->bType == TYPE_ACTIVE || pEffect->bType == TYPE_TIMEZERO))// 드라이브는 클라이언트에서 마우스업할때 쏘기 때문에 서버에서 캐스팅 타임 끝난후에 보내줄수가 없기 때문에 이런 편법을..
		pUser->SetStatus(UNIT_STATUS_CASTING);
	
	pUser->m_sCastingInfo.bSkillKind = bSkillKind;
	pUser->m_sCastingInfo.bOwnType = pPacket->bOwnType;
	pUser->m_sCastingInfo.bPK = pUser->m_sPKDescInfo.m_bPKMode;
	pUser->m_sCastingInfo.bTargetType = pPacket->bTargetType;
	pUser->m_sCastingInfo.dwTargetIndex = pPacket->dwTargetIndex;
	pUser->m_sCastingInfo.wTileIndex_X = pPacket->wTileIndex_X;
	pUser->m_sCastingInfo.wTileIndex_Z = pPacket->wTileIndex_Z;
	
	switch(pPacket->bSkillKindLR)
	{
	case  SELECT_ATTACK_TYPE_LBUTTON:
		if (bSkillKind == __SKILL_LIFEEXPLOSION__)
		{
			float fMin;
			float fMax;
			
			pUser->GetAttackDamageByFormula20(fMin, fMax, bSkillKind
				, pPacket->bSkillKindLR, (WORD)pUser->GetSP());
			pUser->m_sCastingInfo.wAttackDamage[0] = WORD(fMin);
			pUser->m_sCastingInfo.wAttackDamage[1] = WORD(fMax);
			pUser->GetAttackingDamage(pUser->m_sCastingInfo.wAttackDamage[0],
				pUser->m_sCastingInfo.wAttackDamage[1]
				, &pUser->m_sCastingInfo.wAttackDamage[0]
				, &pUser->m_sCastingInfo.wAttackDamage[1],SELECT_ATTACK_TYPE_LBUTTON);
		}
		else
		{
			pUser->GetAttackingDamage(pUser->GetAttackDamageMin_L()
				, pUser->GetAttackDamageMax_L(), &pUser->m_sCastingInfo.wAttackDamage[0]
				, &pUser->m_sCastingInfo.wAttackDamage[1], SELECT_ATTACK_TYPE_LBUTTON);
		}
		
		break;
	case SELECT_ATTACK_TYPE_RBUTTON:
		if (bSkillKind == __SKILL_LIFEEXPLOSION__)
		{
			float fMin;
			float fMax;
			
			pUser->GetAttackDamageByFormula20(fMin, fMax, bSkillKind
				, pPacket->bSkillKindLR, (WORD)pUser->GetSP());
			
			pUser->m_sCastingInfo.wAttackDamage[0] = WORD(fMin);
			pUser->m_sCastingInfo.wAttackDamage[1] = WORD(fMax);
			pUser->GetAttackingDamage(pUser->m_sCastingInfo.wAttackDamage[0]
				, pUser->m_sCastingInfo.wAttackDamage[1], &pUser->m_sCastingInfo.wAttackDamage[0]
				, &pUser->m_sCastingInfo.wAttackDamage[1], SELECT_ATTACK_TYPE_RBUTTON);
		}
		else
		{
			pUser->GetAttackingDamage(pUser->GetAttackDamageMin_R(), pUser->GetAttackDamageMax_R()
				, &pUser->m_sCastingInfo.wAttackDamage[0]
				, &pUser->m_sCastingInfo.wAttackDamage[1], SELECT_ATTACK_TYPE_RBUTTON);
			
		}
		
		break;
	}
	
	pUser->m_dwTemp[USER_TEMP_CASTINGSTARTTICK] = g_dwTickCount;
	pUser->m_dwTemp[USER_TEMP_CASTINGDESTTICK] = DWORD(max(g_dwTickCount, 
		g_dwTickCount+pEffect->dwCastingTime
		+pEffect->dwCastingTime*pUser->GetAlphaStat(USER_CASTINGTIME)
		+(pEffect->dwCastingTime*pUser->m_fPlusParamByMagicFieldArray[USER_CASTINGTIME]/100.F)));
lbl_fail:
	DSTC_DUNGEON_CASTING	packet;
	packet.bSkillKind		= bSkillKind;
	packet.dwUserIndex		= pUser->GetID();
	packet.dwTargetIndex	= 0;	
	
	packet.bTargetType		= pPacket->bTargetType;
	if (pPacket->bTargetType==OBJECT_TYPE_MONSTER)
	{
		CMonster* pMonster =  g_pMonsterTable->GetMonster( pPacket->dwTargetIndex );
		if (pMonster)
		{
			packet.vecTarget = *pMonster->GetCurPosition();
			packet.dwTargetIndex = pMonster->GetID();
		}
		else goto lbl_tile;
				
	}
	else if (pPacket->bTargetType==OBJECT_TYPE_PLAYER)
	{
		CUser* pUser = g_pUserHash->GetData(pPacket->dwTargetIndex);
		if (pUser)
		{
			packet.dwTargetIndex = pUser->GetID();
			packet.vecTarget	 = *pUser->GetCurPosition();
		}
		else goto lbl_tile;
		
		
	}
	else if (pPacket->bTargetType == OBJECT_TYPE_TILE)
	{
lbl_tile:
		packet.vecTarget.x = pPacket->wTileIndex_X*TILE_SIZE;
		packet.vecTarget.y = pPacket->wTileIndex_Z*TILE_SIZE;
	}
	pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), pUser->GetPrevSectionNum() );
}


void CmdFinishMapLoading( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
//	CTDS_FINISH_MAPLOADING* pPacket = (CTDS_FINISH_MAPLOADING*)pMsg;
	CUser* pUser = (CUser*)g_pNet->GetUserInfo(dwConnectionIndex);
	if (!pUser)	return;
	
	pUser->ChangeGodMode(GODMODE_STATUS_EVENT_TIMER);
	if (!pUser->GetCurDungeon())	return;
	
	CDungeonLayer* pLayer = pUser->GetCurDungeonLayer();
	if( !pLayer )					return;
	
	if ( pUser->GetCurDungeonLayer()->m_bMatching == TRUE && pUser->GetCurDungeonLayer()->m_bLayer ) 
	{ 
		// 레이어에 유저 카운터를 올려준다.
		pUser->GetCurDungeonLayer()->m_bReadyMatchNum++;	

		DSTC_READY_MATCH_MAN2MAN	ReadyPacket;
		ReadyPacket.dwReadyUserIndex = pUser->GetID();
		pUser->GetCurDungeonLayer()->BroadCastMsg((char *)&ReadyPacket, ReadyPacket.GetPacketSize());

		if ( pUser->GetCurDungeonLayer()->m_bReadyMatchNum >= 2 )
		{
			// 카운트 시작해라!
			pUser->GetCurDungeonLayer()->m_dwMatchBeginTime = g_dwTickCount;
			pUser->GetCurDungeonLayer()->m_dwMatchEndTime = 
				pUser->GetCurDungeonLayer()->m_dwMatchBeginTime + 
				pUser->GetCurDungeonLayer()->m_dwMatchTick;
		}
	}

	Event_FinishMapLoading(pUser);	
}


void CmdSkill( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_SKILL*			pPacket = (CTDS_SKILL*)pMsg;

	CUser*		pOwnUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if(!pOwnUser )
		return;
	
	else if( pOwnUser->GetUnitStatus() == UNIT_STATUS_DEAD)
		return;			
	
	else if (NULL == pOwnUser->GetCurDungeon())
		return;
	
#if defined TAIWAN_LOCALIZING
	if (pOwnUser->GetAverageWeight() >= WEIGTH_100PER_OVER)
	{
		pOwnUser->SendSkillCastingFail(SKILL_CASTING_FAIL_REASON_LOW);
		return;
	}
#endif
	
	SKILLDESC skillDesc;
	skillDesc.bOwnType			= OBJECT_TYPE_PLAYER;
	skillDesc.bPK				= pPacket->bPK;
	skillDesc.bSkillKind		= pOwnUser->m_sCastingInfo.bSkillKind;
	skillDesc.bSkillLevel		= BYTE(pOwnUser->GetSkillLevel(skillDesc.bSkillKind)-1);
	skillDesc.bTargetType		= pPacket->bTargetType;
	skillDesc.dwOwnIndex		= pOwnUser->GetID();
	skillDesc.dwSkillKeepTime	= pPacket->dwTime;
	skillDesc.dwTargetIndex		= pPacket->dwTargetIndex;
	skillDesc.pDungeonLayer		= pOwnUser->GetCurDungeonLayer();
	skillDesc.bSectionNum		= pOwnUser->GetPrevSectionNum();
	skillDesc.v2OwnObjectPos	= *pOwnUser->GetCurPosition();
	skillDesc.wTileIndex_X		= pPacket->wTileIndex_X;
	skillDesc.wTileIndex_Z		= pPacket->wTileIndex_Z;
	skillDesc.pMonsterMaster	= pOwnUser;
	skillDesc.wDamageMinMax[0]	= pOwnUser->m_sCastingInfo.wAttackDamage[0];
	skillDesc.wDamageMinMax[1]	= pOwnUser->m_sCastingInfo.wAttackDamage[1];
	g_pEffectLayer->SendSkill(&skillDesc);	
}


void CmdUserDieOK( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
//	CTDS_USER_DIE_OK* pPacket = (CTDS_USER_DIE_OK*)pMsg;	
	CUser* pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
	if (!pUser)	return;

	pUser->SetEventCompulsiveDisconnectUser();
}


void CmdEntrance_Edit( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_ENTRANCE_EDIT* pPacket = (CTDS_ENTRANCE_EDIT*)pMsg;
	
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (pDungeon)
	{
		CUser* pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
		if (!pUser)	return;

		if (pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser))
		{
			// 관리자군.
			if (pPacket->dwMoney <= pDungeon->GetDungeonDataEx()->GetMaxEntrancePay())
			{
				pDungeon->GetDungeonDataEx()->SetEntrance(pPacket->dwMoney);
			}			
		}
	}
}


// 던전에 쌓인 경험치를 먹어라.
void CmdAccExp_Acquisition( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_ACCEXP_ACQUISITION* pPacket = (CTDS_ACCEXP_ACQUISITION*)pMsg;
	
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (pDungeon)
	{
		CUser*		pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
		if (!pUser)	return;

		if (pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser))
		{
			// 관리자군.
			if (!pDungeon->GetDungeonDataEx()->m_dwAccExp)	return;
			
			pUser->AddExp(pDungeon->GetDungeonDataEx()->m_dwAccExp, FALSE);
			pDungeon->GetDungeonDataEx()->SetAccExp(0);			
		}
	}	
}


// 던전 아이템 획득해라.
void CmdProduct_Acquisition( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{
	CTDS_PRODUCT_ACQUISITION* pPacket = (CTDS_PRODUCT_ACQUISITION*)pMsg;
	
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (pDungeon)
	{
		CUser* pUser = (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
		if (!pUser)	return;

		if (pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser)	&& !pDungeon->GetDungeonDataEx()->m_bSiege)
		{
			// 관리자군.
			// 원소 속성석이거나 가디언 성체가 완성되었을때만 획득가능하지.
			if ((g_DungeonProductionItemMinMax[pDungeon->GetDungeonDataEx()->m_byProperty].wItemIDDefault
				== pDungeon->GetDungeonDataEx()->m_wProduction && pDungeon->GetDungeonDataEx()->m_byProductionCount == 1)
				|| (g_DungeonProductionItemMinMax[pDungeon->GetDungeonDataEx()->m_byProperty].wItemIDDefault
				!= pDungeon->GetDungeonDataEx()->m_wProduction && pDungeon->GetDungeonDataEx()->m_byProductionCount == 3))
			{				
				CItem sItem;
				CreateItem(&sItem, pDungeon->GetDungeonDataEx()->m_wProduction, 1);
				// 던전 산출물 버그 : 김영대 050218
				int nPackCount = Insert_SmallInvItem(pUser, &sItem, 0, sItem.GetQuantity(), FALSE);
				
				// Item Log
				SetItemCount( sItem.m_wItemID, pDungeon->GetID(), sItem.GetQuantity() );
				
				DSTWS_REFRESH_PRODUCT_ACQUISITION ServerPacket;
				ServerPacket.wDungeonID = WORD(pDungeon->GetDungeonDataEx()->m_dwID);
				
				// 디폴트 생산품으로 돌려놓기위해서..
				ServerPacket.wProduction = g_DungeonProductionItemMinMax[pDungeon->GetDungeonDataEx()->m_byProperty].wItemIDDefault;
				
				// 먹은수.
				g_pNet->SendToServer(WSINDEX, (char*)&ServerPacket, ServerPacket.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION );

				pDungeon->GetDungeonDataEx()->m_wProduction			= 
					g_DungeonProductionItemMinMax[pDungeon->GetDungeonDataEx()->m_byProperty].wItemIDDefault;
				pDungeon->GetDungeonDataEx()->m_byProductionCount	= 0;
				pDungeon->GetDungeonDataEx()->SetOperationMode(DUNGEON_OPERATIONTYPE_PRECOCIOUS);
				
				DSTC_ITEM_PICKUP	packet;
				packet.bInv		= 14;
				packet.bZipCode	= 0;
				memcpy(&packet.Item, &sItem, sizeof(CItem));
				NetSendToUser(pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize(), FLAG_SEND_NOT_ENCRYPTION);		
			}			
		}
	}	
}


void CmdAccEntrance_Acquisition( DWORD dwConnectionIndex, char* pMsg, DWORD dwLength )
{		
	CTDS_ACCENTRANCE_ACQUISITION* pPacket = (CTDS_ACCENTRANCE_ACQUISITION*)pMsg;
	CDungeon* pDungeon = g_pDungeonTable->GetDungeonInfo(pPacket->wDungeonID);
	if (pDungeon)
	{
		CUser*		pUser	= (CUser*)g_pNet->GetUserInfo( dwConnectionIndex );
		if (!pUser)	return;

		if (pDungeon->GetDungeonDataEx()->IsDungeonOwner(pUser))
		{
			// 던전의 주인이거나, 길드마스터일때 건드릴수 있다.
			if (!pDungeon->GetDungeonDataEx()->m_dwAccEntrance)	return;
			
			int nPlusMoney = 
				pDungeon->GetDungeonDataEx()->SetAccEntrance(-INT(pDungeon->GetDungeonDataEx()->m_dwAccEntrance));
			
			if (pUser->m_dwMoney<=__MAX_MONEY__ -nPlusMoney)
			{
				pUser->m_dwMoney += -nPlusMoney;
				pUser->m_dwMoney = min(pUser->m_dwMoney, __MAX_MONEY__);
			}
			else
			{
				pUser->m_dwMoney = __MAX_MONEY__;
			}
			
			DSTC_ACCENTRANCE_ACQUISITION packet;
			packet.dwMoney = pUser->m_dwMoney;
			NetSendToUser( pUser->m_dwConnectionIndex, (char*)&packet, packet.GetPacketSize()
				, FLAG_SEND_NOT_ENCRYPTION );			
		}
	}
}