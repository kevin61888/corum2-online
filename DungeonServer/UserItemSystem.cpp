#include "stdafx.h"
#include "User.h"
#include "Dungeon.h"
#include "DungeonLayer.h"
#include "ClassMemoryPool.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "DUNGEON_DATA_EX.h"
#include "Map.h"
#include "Monster.h"
#include "Section.h"
#include "OwnServer.h"
#include "DBProc.h"
#include "MonsterManager.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "recvmsg.h"
#include "ItemNativeManager.h"
#include "MsgProc.h"
#include "MsgProc_for_jjw6263.h"
#include "EventDungeonManager.h"
#include "ItemAttrLayer.h"


// 현재 사용중인 시간제 아이템에 대한 처리
void CUser::UsingItemListProcess()
{
	if (!GetCurDungeon())	return;
	
	// 시간제 아이템 체크
	if( g_dwTickCount - m_dwTemp[ USER_TEMP_LASTITEMLISTCHECKTICK ]	< 1000 * 60 )
		return;
	
	DWORD dwLastItemListChectTick = m_dwTemp[ USER_TEMP_LASTITEMLISTCHECKTICK ];
	dwLastItemListChectTick;

	m_dwTemp[ USER_TEMP_LASTITEMLISTCHECKTICK ] = g_dwTickCount;

	BOOL		bDetach = FALSE;
	POSITION_	pos		= m_pUsingItemList->GetHeadPosition();
	POSITION_	del		= NULL;

	while(pos)
	{
		del		= pos;
		bDetach = FALSE;

		CItem* pItem = (CItem*)m_pUsingItemList->GetNext(pos);
		if (NULL == pItem)
		{
			m_pUsingItemList->RemoveAt(del);
			continue;
		}

		CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());
		if (NULL == pBaseItem)
		{
			m_pUsingItemList->RemoveAt(del);
			continue;
		}
		
		if (__ITEM_INSURANCE_INDEX__ == pBaseItem->wID )
		{
			// 보험 아이템은 사용되어지고 있는 아이템 리스트에 추가되어지지만 시간제한은 없다.
			continue;
		}

		DWORD dwCode_ID = GetItemKind(pItem->GetID());
		
		switch(dwCode_ID)
		{
		case ITEM_KIND_CONSUMABLE:
			{
				if( ( 0 <  pBaseItem->GetLastTime() )
				&& 	 IsOverDueDay(pItem->m_Item_Consumable.dwEndDate) )
				{
					bDetach = TRUE;
				}				
			}break;
		case ITEM_KIND_GUARDIAN:
			{
				if (GUARDIAN_ITEM_TYPE_EGG == pBaseItem->BaseItem_Guardian.bType)
				{
					if (IsGrowGuardianItem(pItem))
					{					
						if (g_dwTickCount > m_GuardianEggGrowDesc[0].dwLastTick	&& (g_dwTickCount - m_GuardianEggGrowDesc[0].dwLastTick) >= 1000)
						{
							pItem->SetGuardianEggCompleteTime(pItem->m_Item_Guardian.dwCompleteTime+1);							
							m_GuardianEggGrowDesc[0].dwLastTick = g_dwTickCount;
							SendToUpdate_GuardianItem(m_dwConnectionIndex, pItem);
						}					
					}
					else
					{
						bDetach = TRUE;
					}
				}
				else
				{
					if (IsOverDueDay(pItem->m_Item_Guardian.dwEndLife))
					{
						UnSummonGuardian(0);
					}
				}
			}break;
			default: break;
		}

		if (bDetach)
		{
			DetachItem(del);
		}
	}
}

// 사용하고 있는 아이템 리스트에 넣을수 있어?
BOOL CUser::IsAvailabilityAttachItem(WORD wItemID)
{
	POSITION_ pos = m_pUsingItemList->GetHeadPosition();
	
	while(pos)
	{
		CItem*	pItem	= (CItem*)m_pUsingItemList->GetNext(pos);
		WORD	wID		= pItem->GetID();

		if(wItemID == wID)
		{
			return FALSE;
		}
	}

	return TRUE;
}


// 유효기간있는 아이템 초기화 해준다. 조인할때 한번만 해준다.
void CUser::InitUsingItem()
{
	BOOL bUsingItem = FALSE;

	for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		bUsingItem = FALSE;

		if(	m_pInv_Small[i].GetID()/ITEM_DISTRIBUTE >= ITEM_CONSUMABLEST_INDEX &&
			m_pInv_Small[i].GetID()/ITEM_DISTRIBUTE <= ITEM_CONSUMABLEEND_INDEX)
		{
			// 한번이라도 사용했다.
			bUsingItem = m_pInv_Small[i].m_Item_Consumable.bInvIndex;
		}
		else if(m_pInv_Small[i].GetID()==__ITEM_INSURANCE_INDEX__)
		{
			// 아이템 보험 한번이라도 사용했다.
			bUsingItem = m_pInv_Small[i].m_Item_Zodiac.bInvIndex;
		}
		
		if (bUsingItem)
		{
			AttachItem(&m_pInv_Small[i]);
		}
	}
}


// 시간이 있는 아이템 추가.
void CUser::AttachItem(CItem* pItem)
{	
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());
		
	if(!pBaseItem)	return;	
	m_pUsingItemList->AddTail(pItem);

	DWORD dwCode_ID = GetItemKind(pItem->GetID());

	if(dwCode_ID == ITEM_KIND_CONSUMABLE)
	{
		for(int i = 0; i < MAX_BASEITEM_CONSUMABLE_ITEMATTR; ++i)
		{
			ExchangeConsumableItemRefreshUserAttr(pItem->GetID()
				, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code
				, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value
				, TRUE);
		}
	}
	else if (dwCode_ID == ITEM_KIND_GUARDIAN)
	{
		m_GuardianEggGrowDesc[0].dwLastTick = g_dwTickCount;
		m_GuardianEggGrowDesc[0].pos		= m_pUsingItemList->GetTailPosition();
	}	
}


// 시간이 있는 아이템 삭제.
void CUser::DetachItem(POSITION_ pos)
{
	if(NULL == pos){return;}
	
	CItem*		pItem		= (CItem*)m_pUsingItemList->GetAt(pos);

	if(NULL == pItem){return;}
	
	CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItem->GetID());
		
	m_pUsingItemList->RemoveAt(pos);

	DWORD dwCode_ID = GetItemKind(pItem->GetID());

	if(dwCode_ID == ITEM_KIND_CONSUMABLE)
	{
		DSTC_ITEM_PICKUP PickupItem;
		PickupItem.bInv			= 74;
		PickupItem.bZipCode		= BYTE(pItem->m_Item_Consumable.bInvIndex - 1);
		PickupItem.bEquipCode	= 0;

		memset(&m_pInv_Small[PickupItem.bZipCode], 0, sizeof(CItem));
		memset(&PickupItem.Item, 0, sizeof(CItem));

		NetSendToUser( m_dwConnectionIndex, (char*)&PickupItem, PickupItem.GetPacketSize()
			, FLAG_SEND_NOT_ENCRYPTION );

		for(int i = 0; i < MAX_BASEITEM_CONSUMABLE_ITEMATTR; ++i)
		{
			ExchangeConsumableItemRefreshUserAttr
			(
				pItem->GetID()
				, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code
				, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value
				, FALSE
			);
		}	
	}
	else if(dwCode_ID == ITEM_KIND_GUARDIAN)// : 050108 hwoarang 
	{
		BYTE byZipCode = 0;
		m_GuardianEggGrowDesc[byZipCode].dwLastTick = 0;
		m_GuardianEggGrowDesc[byZipCode].pos		= NULL;
	}
}


// 시간이 있는 아이템 재계산
void CUser::UpdateUsingItem(CItem* pItem)
{
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());
		
	if(!pBaseItem)	return;

	DWORD dwCode_ID = GetItemKind(pItem->GetID());

	if(dwCode_ID == ITEM_KIND_CONSUMABLE)
	{
		for(int i = 0; i < MAX_BASEITEM_CONSUMABLE_ITEMATTR; ++i)
		{
			ExchangeConsumableItemRefreshUserAttr
			(
				pItem->GetID()
				, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Code
				, pBaseItem->BaseItem_Consumable.sItem_Attr[i].wAttr_Value
				, TRUE
			);
		}
	}	
}


void CUser::ExchangeConsumableItemRefreshUserAttr(WORD wItemID, WORD wAttrCode, short nValue, BYTE byCreate)
{
//	DWORD dwVirtualCode		= g_pItemAttrLayer->GetVirtualCode(wAttrCode, nValue); // : 050110 hwoarang 
//	short shVirtualValue	= g_pItemAttrLayer->GetVirtualValue(wAttrCode, nValue);	

	DWORD dwVirtualCode		= wAttrCode;
	short shVirtualValue	= nValue;	

	if (byCreate)
		SetItemAttr((ITEM_ATTR_CODE)dwVirtualCode, shVirtualValue, true);
	else
		SetItemAttr((ITEM_ATTR_CODE)dwVirtualCode, shVirtualValue, false);
	
	DSTC_USER_STATUS UserStatus;
	UserStatus.bStatusMany	= 0;

	switch(wAttrCode)
	{
	case ITEM_ATTR_MOVE_SPEED:
		UpdateMoveSpeed();
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= (DWORD)m_fMoveSpeed;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_MOVESPEED;
		break;
	case ITEM_ATTR_ATTACK_SPEED_POINT:
		UpdateAttackSpeed();
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= m_wAttackSpeed;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_ATTACKRATE_PERCENT;
		break;
	case ITEM_ATTR_AA_POINT:
		UpdateAttackAcuracy();
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= m_wAttackAcuracy;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_ATTACKACURACY_PERCENT;
		break;
	case ITEM_ATTR_AVOID_POINT:
		UpdateAvoid();
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= m_wAvoid;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_AVOID;
		break;
	case ITEM_ATTR_EGO:		
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetNewestEgo();
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_EGO;		
		break;
	case ITEM_ATTR_STR:		
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetNewestStr();
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_STR;		
		break;
	case ITEM_ATTR_INT:		
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetNewestInt();
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_INT;
		UpdateMaxSP();
		UpdateHealSPSec();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MAXMP;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetMaxSP();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HEALMPSPEED;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealMPSec;
		UpdateFireResist();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_FIRE_RESIST;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wFireResist;
		UpdateIceResist();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ICE_RESIST;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wIceResist;
		UpdateLightResist();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_LIGHT_RESIST;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wLightResist;
		UpdatePoiResist();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_POI_RESIST;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wPoiResist;
		break;
	case ITEM_ATTR_DEX:		
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetNewestDex();
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_DEX;
		UpdateAvoid();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_AVOID;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAvoid;
		UpdateAttackAcuracy();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACKACURACY_PERCENT;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_wAttackAcuracy;
		break;
	case ITEM_ATTR_VIT:		
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetNewestVit();
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_VIT;
		UpdateMaxHP();
		UpdateHealHPSec();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_MAXHP;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= GetMaxHP();
		UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_HEALHPSPEED;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwMin	= m_dwHealHPSec;
		break;
	case ITEM_ATTR_HP_PERCENT:
		UpdateHealHPSec();
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= m_dwHealHPSec;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_HEALHPSPEED;
		break;
	case ITEM_ATTR_SP_PERCENT:
		UpdateHealSPSec();
		UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= m_dwHealMPSec;
		UserStatus.pStatus[UserStatus.bStatusMany++].dwCode	= USER_HEALMPSPEED;
		break;
	}

	// 스킬레벨을 갱신해줘라.
	// sung-han 2005-01-21 스킬 레벨이 바로 올라가지 않는 부분 수정, VirtualCode 에서 RealCode로 바꿈.
	DWORD dwRealCode = g_pItemAttrLayer->GetVirtualCode( wAttrCode, nValue );
	if (g_pItemAttrLayer->m_ItemAttrDefine[dwRealCode].wFormula == 5)
	{
		SendAllUserSkillLevel();
	}

	WORD wMinMax[2] = {0, 0};
	GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)
		, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON))
		, BYTE(GetCurrentWeaponID()/ITEM_DISTRIBUTE)
		, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_LBUTTON);
	
	SetAttackDamage_L(wMinMax[0], wMinMax[1]);
	
	GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)
		, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON))
		, BYTE(GetCurrentWeaponID()/ITEM_DISTRIBUTE)
		, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);
	
	SetAttackDamage_R(wMinMax[0], wMinMax[1]);

	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_R;
	UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_R();
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_R();
	UserStatus.pStatus[UserStatus.bStatusMany].dwCode	= USER_ATTACK_L;
	UserStatus.pStatus[UserStatus.bStatusMany].dwMin	= GetAttackDamageMin_L();
	UserStatus.pStatus[UserStatus.bStatusMany++].dwMax	= GetAttackDamageMax_L();
	
	if(UserStatus.bStatusMany)
	{
		NetSendToUser( m_dwConnectionIndex, (char*)&UserStatus, UserStatus.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	
		if (GetCurDungeon())
		{		
			CDungeonLayer* pLayer = GetCurDungeonLayer();
			if (!pLayer)	return;

			// 컨슈머블 아이템 사용이펙트를 표시해주자.
			DSTC_ORIGINATIONITEMWITHEFFECTKIND packet;
			packet.dwUserIndex	= GetID();
			packet.bCreate		= byCreate;
			packet.bOriginationItemWithEffectKind[packet.bCount++] = 0;

			pLayer->BroadCastSectionMsg( (char*)&packet, packet.GetPacketSize(), GetPrevSectionNum() );
		}
	}	
}


void CUser::InitUserItemAll()
{
#ifndef EQUIP_CHK 
	int						i, j, iLoop;
	DWORD					dwSetID;
	USER_SET_ITEM_INFO*		pUserSetItem;
	LPSET_ITEM_INFO			pSetItemInfo;
	CBaseItem*				pBaseItem;
	BOOL					bChk;
	
	// 속성을 모두 초기화하고.
	memset( m_pwItemAttr, 0, sizeof(m_pwItemAttr) );	
	
	// 초기화 하고.
	g_pUserSetItemHash->Destroy( TRUE );	

	// 일단 세트 아이템 정보를 만든다.
	for( i=0; i<MAX_EQUIP; i++ )
	{				
		switch( i )
		{
		case EQUIP_TYPE_RHAND1:
		case EQUIP_TYPE_LHAND1:	
		case EQUIP_TYPE_RHAND2:
		case EQUIP_TYPE_LHAND2:
		case EQUIP_TYPE_HELMET:
		case EQUIP_TYPE_RGLOVE:			
		case EQUIP_TYPE_MAIL:						
		case EQUIP_TYPE_AMULE2:					
		case EQUIP_TYPE_BELT:				
		case EQUIP_TYPE_LGLOVE:			
		case EQUIP_TYPE_RRING1:			
		case EQUIP_TYPE_RRING2:			
		case EQUIP_TYPE_RRING3:			
		case EQUIP_TYPE_BOOTS:					
			{										
				if(m_bCurrentHand==1)
				{
					if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
						break;
				}
				else if(m_bCurrentHand==2)
				{
					if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
						break;
				}		

				pBaseItem = g_pBaseItemHash->GetData( m_pEquip[i].m_wItemID );
				if( !pBaseItem ) continue;

				dwSetID = pBaseItem->GetSetID(); 
				if( dwSetID == 0 || dwSetID == 32767 ) continue;

				pUserSetItem = g_pUserSetItemHash->GetData( dwSetID );
				if( pUserSetItem )		// 만약 등록된 아이디라면.
				{
					bChk = FALSE;
					for( j=0; j<8; j++ )
					{
						if( pUserSetItem->pwSetItemID[j] == m_pEquip[i].m_wItemID )
						{
							bChk = TRUE;
							break;
						}
					}

					if( !bChk ) // 삽입한다.
					{
						pUserSetItem->pwSetItemID[pUserSetItem->wCnt] = m_pEquip[i].m_wItemID;
						pUserSetItem->wCnt++;
					}
				}
				else					// 아니라면.^^
				{
					// 새로 만들어 삽입한다.
					pUserSetItem = new USER_SET_ITEM_INFO;
					memset( pUserSetItem, 0, sizeof( USER_SET_ITEM_INFO ) );

					pUserSetItem->dwSetID = dwSetID;
					pUserSetItem->pwSetItemID[ pUserSetItem->wCnt ] = m_pEquip[i].m_wItemID;
					pUserSetItem->wCnt++;

					g_pUserSetItemHash->Insert( pUserSetItem, pUserSetItem->dwSetID );
				}

				// 풀셋인지를 체크한다.
				pSetItemInfo = g_pSetItemInfoHash->GetData( pUserSetItem->dwSetID );
				if( pSetItemInfo->bFullSet == (BYTE)pUserSetItem->wCnt ) 
					pUserSetItem->isFullSet = TRUE;
			}
			break;

		default: 
			continue;
		}
	}
	
	for( i = 0; i < MAX_EQUIP; i++ )
	{
		if(m_bCurrentHand==1)
		{
			if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
				continue;
		}
		else if(m_bCurrentHand==2)
		{
			if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
				continue;
		}

		InitUserItem( &m_pEquip[i] );
	}
	
	POSITION_ pos = m_pUsingItemList->GetHeadPosition();
	while(pos)
	{
		CItem* pItem = (CItem*)m_pUsingItemList->GetNext(pos);
		UpdateUsingItem(pItem);
	}
	
	// 세트아이템 추가 옵션과 풀옵 을 적용한다.
	ListNode<USER_SET_ITEM_INFO>* pNode = NULL;
	pNode = g_pUserSetItemHash->GetHead();

	while( pNode )
	{
		pUserSetItem = pNode->pData;
		pSetItemInfo = g_pSetItemInfoHash->GetData( pUserSetItem->dwSetID );
		
		// 세트아이템 추가 옵션.
		iLoop = pUserSetItem->wCnt - 1;
		if( pUserSetItem->wCnt > 5 ) iLoop = 5;

		for( i=0; i<iLoop; i++ )
		{
			if( pSetItemInfo->pBonusOption[i][0].wKind != 0 ) 
				SetItemAttr(pSetItemInfo->pBonusOption[i][0].wKind, pSetItemInfo->pBonusOption[i][0].wValue);
			
			if( pSetItemInfo->pBonusOption[i][1].wKind != 0 ) 
				SetItemAttr(pSetItemInfo->pBonusOption[i][1].wKind , pSetItemInfo->pBonusOption[i][1].wValue);			
		}
		
		// 풀옵.
		if( pUserSetItem->isFullSet )
		{
			for( i=0; i<8; i++ )
			{
				if( pSetItemInfo->pFullOption[i].wKind == 0 ) 
					continue;
				SetItemAttr(pSetItemInfo->pFullOption[i].wKind , pSetItemInfo->pFullOption[i].wValue);
				
			}
		}

		pNode = pNode->pNext;
	}
#else

	int						i, j, iLoop;
	DWORD					dwSetID;
	USER_SET_ITEM_INFO*		pUserSetItem;
	LPSET_ITEM_INFO			pSetItemInfo;
	CBaseItem*				pBaseItem;
	BOOL					bChk;
		
	InitializeItemAttr();
	memset(m_byItemChk, 0, sizeof(m_byItemChk));	

	g_pUserSetItemHash->Destroy(TRUE);

	for(i = 0; i < MAX_EQUIP_POOL; i++)
	{			
		bChk = FALSE;

		if(m_bCurrentHand==1)
		{
			if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
				continue;
		}
		else if(m_bCurrentHand==2)
		{
			if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
				continue;
		}	

		if(	i==EQUIP_TYPE_AMULE1 || i==EQUIP_TYPE_AMULE3 ||
			i==EQUIP_TYPE_LRING1 || i==EQUIP_TYPE_LRING2 || i==EQUIP_TYPE_LRING3)
			continue;

		if(i==EQUIP_TYPE_RIDE)
			continue;
		
		if(m_pEquip[i].GetID()==0)
			continue;

		if(!bChk)
		{
			// 세트 아이템 체크 //
			pBaseItem = g_pBaseItemHash->GetData( m_pEquip[i].m_wItemID );
			
			if(!pBaseItem)
				continue;

			dwSetID = pBaseItem->GetSetID(); 
			
			if(!(dwSetID==0 || dwSetID==32767))
				continue;

			// 아이템 체크 //
			if(CheckItem(&m_pEquip[i])==TRUE)
			{
				m_byItemChk[i] = 1;
				InitUserItem(&m_pEquip[i]);			
			}
		}		
	}

	i = 0;
	
	for(;;)
	{
		if(i==MAX_EQUIP_POOL)
			break;
		
		if(m_bCurrentHand==1)
		{
			if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
			{
				i++;
				continue;
			}
		}
		else if(m_bCurrentHand==2)
		{
			if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
			{
				i++;
				continue;
			}
		}

		if(i==EQUIP_TYPE_RIDE)
		{
			i++;
			continue;
		}

		if(	i==EQUIP_TYPE_AMULE1 || i==EQUIP_TYPE_AMULE3 ||
			i==EQUIP_TYPE_LRING1 || i==EQUIP_TYPE_LRING2 || i==EQUIP_TYPE_LRING3)
		{
			i++;
			continue;
		}

		if(m_pEquip[i].GetID()==0)
		{
			i++;
			continue;
		}			
		
		if(m_byItemChk[i]!=1)
		{
			if(CheckItem(&m_pEquip[i])==TRUE)
			{		
				pBaseItem = g_pBaseItemHash->GetData(m_pEquip[i].m_wItemID);

				if(!pBaseItem)
				{
					i++;
					continue;
				}

				dwSetID = pBaseItem->GetSetID(); 
				
				if(dwSetID==0 || dwSetID==32767)
				{
					InitUserItem(&m_pEquip[i]);
					m_byItemChk[i] = 1;
					i = 0;					
					continue;
				}

				pUserSetItem = g_pUserSetItemHash->GetData( dwSetID );

				if(pUserSetItem)		// 만약 등록된 아이디라면.
				{
					bChk = FALSE;

					for(j = 0; j < 8; j++)
					{
						if(pUserSetItem->pwSetItemID[j]==m_pEquip[i].m_wItemID)
						{
							bChk = TRUE;
							break;
						}
					}

					if(!bChk) // 삽입한다.
					{
						pUserSetItem->pwSetItemID[pUserSetItem->wCnt] = m_pEquip[i].m_wItemID;
						pUserSetItem->wCnt++;
					}
				}
				else					// 아니라면.^^
				{
					// 새로 만들어 삽입한다.
					pUserSetItem = new USER_SET_ITEM_INFO;
					memset( pUserSetItem, 0, sizeof( USER_SET_ITEM_INFO ) );

					pUserSetItem->dwSetID = dwSetID;
					pUserSetItem->pwSetItemID[ pUserSetItem->wCnt ] = m_pEquip[i].m_wItemID;
					pUserSetItem->wCnt++;

					g_pUserSetItemHash->Insert( pUserSetItem, pUserSetItem->dwSetID );
				}

				// 풀셋인지를 체크한다.
				pSetItemInfo = g_pSetItemInfoHash->GetData( pUserSetItem->dwSetID );
				if( pSetItemInfo && ( pSetItemInfo->bFullSet == (BYTE)pUserSetItem->wCnt ) )
					pUserSetItem->isFullSet = TRUE;
				
				InitUserItem(&m_pEquip[i]);

				m_byItemChk[i] = 1;

				i = 0;
				continue;
			}
			else
				m_byItemChk[i] = 2;

			// 세트 아이템 체크 //
			pBaseItem = g_pBaseItemHash->GetData( m_pEquip[i].m_wItemID );

			if(!pBaseItem)
			{
				i++;
				continue;
			}
		}
		
		if(i<MAX_EQUIP_POOL)
			i++;
		else
			break;
	}
	
	for(i = 0; i < MAX_EQUIP_POOL; i++)
	{
		if(m_bCurrentHand==1)
		{
			if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
				continue;
		}
		else if(m_bCurrentHand==2)
		{
			if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
				continue;
		}	
		
		if(i==EQUIP_TYPE_RIDE)
			continue;

		if(	i==EQUIP_TYPE_AMULE1 || i==EQUIP_TYPE_AMULE3 ||
			i==EQUIP_TYPE_LRING1 || i==EQUIP_TYPE_LRING2 || i==EQUIP_TYPE_LRING3)
			continue;

		InitUserBonusItem(&m_pEquip[i]);
	}	

	// 세트아이템 추가 옵션과 풀옵 을 적용한다.
	ListNode<USER_SET_ITEM_INFO>* pNode	= g_pUserSetItemHash->GetHead();
	
	while(pNode)
	{
		pUserSetItem = pNode->pData;
		pSetItemInfo = g_pSetItemInfoHash->GetData( pUserSetItem->dwSetID );
		
		if(!pSetItemInfo)
		{
			pNode = pNode->pNext;
			continue;
		}

		// 세트아이템 추가 옵션.
		iLoop = pUserSetItem->wCnt - 1;
		if( pUserSetItem->wCnt > 5 ) iLoop = 5;		

		for( i=0; i<iLoop; i++ )
		{
			if( pSetItemInfo->pBonusOption[i][0].wKind != 0 ) 
				SetItemAttr((ITEM_ATTR_CODE)pSetItemInfo->pBonusOption[i][0].wKind, pSetItemInfo->pBonusOption[i][0].wValue);
			
			if( pSetItemInfo->pBonusOption[i][1].wKind != 0 )
				SetItemAttr((ITEM_ATTR_CODE)pSetItemInfo->pBonusOption[i][1].wKind , pSetItemInfo->pBonusOption[i][1].wValue);
		}
		
		// 풀옵.
		if( pUserSetItem->isFullSet )
		{
			for(i = 0; i < 8; i++)
			{
				if(pSetItemInfo->pFullOption[i].wKind==0) 
					continue;

				SetItemAttr((ITEM_ATTR_CODE)pSetItemInfo->pFullOption[i].wKind , pSetItemInfo->pFullOption[i].wValue);				
			}
		}
		pNode = pNode->pNext;
	}

	// 아이템 몰 아이템 적용 //
	POSITION_ pos = m_pUsingItemList->GetHeadPosition();
	while(pos)
	{
		CItem* pItem = (CItem*)m_pUsingItemList->GetNext(pos);
		UpdateUsingItem(pItem);
	}

	BYTE byUpgrade[5] = {0, 0, 0, 0, 0};

	for(i = 0; i < MAX_EQUIP_POOL; i++)
	{
		if(i==EQUIP_TYPE_RHAND1 ||i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND1 || i==EQUIP_TYPE_LHAND2)
		{
			if(m_bCurrentHand==1)
			{				
				if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
				{
					int nValue = m_pEquip[i].GetID()/ITEM_DISTRIBUTE;
									
					if((nValue>=2 && nValue<=5) || nValue==7)
					{
						if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
						{
							if(CheckItem(&m_pEquip[i]))
								byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]+=2;
						}
					}
					else
					{
						if(CheckItem(&m_pEquip[i]))
						{
							if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
								byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]+=1;					
						}
					}
				}
			}
			else if(m_bCurrentHand==2)
			{
				if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
				{
					int nValue = m_pEquip[i].GetID()/ITEM_DISTRIBUTE;
									
					if((nValue>=2 && nValue<=5) || nValue==7)
					{
						if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
						{
							if(CheckItem(&m_pEquip[i]))
								byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]+=2;
						}
					}
					else
					{
						if(CheckItem(&m_pEquip[i]))
						{
							if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
								byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]+=1;					
						}
					}
				}
			}
		}
		else if(i==EQUIP_TYPE_HELMET || i==EQUIP_TYPE_RGLOVE || i==EQUIP_TYPE_MAIL || i==EQUIP_TYPE_BELT || i==EQUIP_TYPE_LGLOVE || i==EQUIP_TYPE_BOOTS)
		{
			if(CheckItem(&m_pEquip[i]))
			{
				if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
					byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]++;
			}
		}
	}

	BYTE byUpgradeValue[25] = {10, 5, 5, 8, 0, 15, 7, 7, 10, 1, 25, 10, 10, 14, 2, 40, 15, 15, 20, 3, 60, 25, 25, 30, 4};
	
	if(!GetCurDungeon())
		return;
		
	CDungeonLayer*	pLayer = GetCurDungeonLayer();

	if(!pLayer)
		return;
	
	for(i = 0; i < 5; i++)
	{
		if(byUpgrade[i]==8)
		{					
			SetItemAttr(ITEM_ATTR_ALLSTATUS, byUpgradeValue[i*5]);
			SetItemAttr(ITEM_ATTR_HP_PERCENT, byUpgradeValue[i*5+1]);
			SetItemAttr(ITEM_ATTR_SP_PERCENT, byUpgradeValue[i*5+2]);
			SetItemAttr(ITEM_ATTR_AVOID_PERSENT, byUpgradeValue[i*5+3]);
			SetItemAttr(ITEM_ATTR_ALLSKILL, byUpgradeValue[i*5+4]);

			if(m_byUpgrade!=i+6)
			{
				m_byUpgrade = BYTE(i+6);

				DSTC_UPGRADE_CHK pUpgradePacket;
				pUpgradePacket.byUpgrade	= m_byUpgrade;
				pUpgradePacket.dwUserIndex	= GetID();		
				pLayer->BroadCastSectionMsg((char*)&pUpgradePacket, pUpgradePacket.GetPacketSize(), GetPrevSectionNum());
			}
			return;
		}
	}
	
	if(m_byUpgrade!=0)
	{
		m_byUpgrade = 0;

		DSTC_UPGRADE_CHK pUpgradePacket;
		pUpgradePacket.byUpgrade	= m_byUpgrade;
		pUpgradePacket.dwUserIndex	= GetID();		
		pLayer->BroadCastSectionMsg((char*)&pUpgradePacket, pUpgradePacket.GetPacketSize(), GetPrevSectionNum());
	}
#endif
}

void CUser::InitUserItem( CItem* pItem )
{
#ifndef EQUIP_CHK

	if( !pItem->m_wItemID ) return;				// 아이템을 장착하고 있지 않다면 끝.
	
	int						i, iItemKind, iLoop;
	USER_SET_ITEM_INFO*		pUserSetItem;
	LPSET_ITEM_INFO			pSetItemInfo;
	CBaseItem*				pBaseItem;
	
	pBaseItem = g_pBaseItemHash->GetData( pItem->m_wItemID );
	if(!pBaseItem)
		return;
			
	// 옵션의 처리이다.
	for( i=0; i<MAX_ITEM_ATTR; i++ )
		SetItemAttr(pItem->wAttr[i].uCode, pItem->wAttr[i].uValue);	
	
	// 여기서부터 세트 아이템 적용.

	DWORD dwSetID = pBaseItem->GetSetID();
	if( dwSetID !=0 && dwSetID != 32767 )		// SetItem ?
	{
		pSetItemInfo = g_pSetItemInfoHash->GetData( dwSetID );
		pUserSetItem = g_pUserSetItemHash->GetData( dwSetID );

		// 옵션 적용시작.
		if( pUserSetItem->wCnt >= 2 )
		{
			// 아이템 각각의 세트 추가옵션.
			iLoop = pUserSetItem->wCnt - 1;
			if( pUserSetItem->wCnt > 4 ) iLoop = 4;

			if( iItemKind >=10 && iItemKind <= 17 )		// Armor..
			{
				for( i=0; i<iLoop; i++ )
				{
					if( pBaseItem->BaseItem_Armor.pPartOption[i].wKind == 0 ) 
						continue;
					SetItemAttr(pBaseItem->BaseItem_Armor.pPartOption[i].wKind
						, pBaseItem->BaseItem_Armor.pPartOption[i].wValue);				
				}
			}
			else										// Weapon..
			{
				for( i=0; i<iLoop; i++ )
				{
					if( pBaseItem->BaseItem_Weapon.pPartOption[i].wKind == 0 ) 
						continue;
					SetItemAttr(pBaseItem->BaseItem_Weapon.pPartOption[i].wKind
						, pBaseItem->BaseItem_Weapon.pPartOption[i].wValue);
				}
			}
		}
	}

#else 

	if( !pItem->m_wItemID ) return;				// 아이템을 장착하고 있지 않다면 끝.
	
	int						i;	
	CBaseItem*				pBaseItem;	
	pBaseItem = g_pBaseItemHash->GetData( pItem->m_wItemID );
	if(!pBaseItem)
		return;
		
	// 옵션의 처리이다.
	for( i=0; i<MAX_ITEM_ATTR; i++ )
		SetItemAttr((ITEM_ATTR_CODE)pItem->wAttr[i].uCode, pItem->wAttr[i].uValue);
#endif
}
 
void CUser::InitUserBonusItem(const CItem* pItem )
{
	if(!pItem->m_wItemID)
		return;
			
	CBaseItem*			pBaseItem;
	USER_SET_ITEM_INFO*	pUserSetItem;
	LPSET_ITEM_INFO		pSetItemInfo;
	int					i, iLoop, iItemKind;
	
	pBaseItem = g_pBaseItemHash->GetData( pItem->m_wItemID );

	if(!pBaseItem)
		return;

	iItemKind = pItem->m_wItemID / ITEM_DISTRIBUTE;

	DWORD dwSetID = pBaseItem->GetSetID();
	if( dwSetID !=0 && dwSetID != 32767 )		// SetItem ?
	{
		pSetItemInfo = g_pSetItemInfoHash->GetData( dwSetID );
		pUserSetItem = g_pUserSetItemHash->GetData( dwSetID );

		if(!pSetItemInfo || !pUserSetItem)
			return;

		// 옵션 적용시작.
		if( pUserSetItem->wCnt >= 2 )
		{
			// 아이템 각각의 세트 추가옵션.
			iLoop = pUserSetItem->wCnt - 1;
			if( pUserSetItem->wCnt > 4 ) iLoop = 4;

			if( iItemKind >= 10 && iItemKind <= 17 )		
			{
				// Armor..
				for( i = 0; i < iLoop; i++ )
				{
					if( pBaseItem->BaseItem_Armor.pPartOption[i].wKind == 0 ) 
						continue;
					SetItemAttr((ITEM_ATTR_CODE)pBaseItem->BaseItem_Armor.pPartOption[i].wKind
						, pBaseItem->BaseItem_Armor.pPartOption[i].wValue);					
				}
			}
			else										
			{
				// Weapon..
				for( i=0; i<iLoop; i++ )
				{
					if( pBaseItem->BaseItem_Weapon.pPartOption[i].wKind == 0 ) 
						continue;
					SetItemAttr((ITEM_ATTR_CODE)pBaseItem->BaseItem_Weapon.pPartOption[i].wKind
						, pBaseItem->BaseItem_Weapon.pPartOption[i].wValue);					
				}
			}
		}
	}
}


// 장착창에 아이템을 장착 했을시.
BOOL CUser::SetEquipItem( USER_STATUS_ARRAY* pUserStatus, BYTE bEquipNum )
{	
	// 아이템의 종류와 장착장의 블럭이 적당한곳인지를 체크한다.
	InitUserItemAll();
	
	switch( bEquipNum )
	{
	case EQUIP_TYPE_RHAND1:			
	case EQUIP_TYPE_RHAND2:			
		{				
			WORD wMinMax[2] = {0, 0};
			GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON)
				, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_LBUTTON))
				, BYTE(GetCurrentWeaponID()/ITEM_DISTRIBUTE)
				, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_LBUTTON);
			
			SetAttackDamage_L(wMinMax[0], wMinMax[1]);
			
			GetAttackDamage(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON)
				, GetSkillLevel(GetSelectedSkill(SELECT_ATTACK_TYPE_RBUTTON))
				, BYTE(GetCurrentWeaponID()/ITEM_DISTRIBUTE)
				, &wMinMax[0], &wMinMax[1], SELECT_ATTACK_TYPE_RBUTTON);
			
			SetAttackDamage_R(wMinMax[0], wMinMax[1]);

			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwCode	= USER_ATTACK_L;
			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwMin	= GetAttackDamageMin_L();
			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwMax	= GetAttackDamageMax_L();
			pUserStatus->dwUsedSlot++;

			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwCode	= USER_ATTACK_R;
			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwMin	= GetAttackDamageMin_R();
			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwMax	= GetAttackDamageMax_R();
			pUserStatus->dwUsedSlot++;

			goto lb_success;
		}
		break;
		
	case EQUIP_TYPE_RIDE:	return FALSE;
	case EQUIP_TYPE_BAG:	return FALSE;
	case EQUIP_TYPE_AMULE1:	return FALSE;
	case EQUIP_TYPE_AMULE3:	return FALSE;
	case EQUIP_TYPE_LRING1:	return FALSE;
	case EQUIP_TYPE_LRING2:	return FALSE;
	case EQUIP_TYPE_LRING3:	return FALSE;
			
	case EQUIP_TYPE_LHAND1:			
	case EQUIP_TYPE_LHAND2:
		{
			UpdatePhyResist();
			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwCode	= USER_PHY_RESIST;
			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwMin	= m_wPhyResist;
			pUserStatus->dwUsedSlot++;
		}
		break;
	
	case EQUIP_TYPE_HELMET:
	case EQUIP_TYPE_RGLOVE:			
	case EQUIP_TYPE_MAIL:				
	case EQUIP_TYPE_AMULE2:				
	case EQUIP_TYPE_BELT:				
	case EQUIP_TYPE_LGLOVE:			
	case EQUIP_TYPE_RRING1:			
	case EQUIP_TYPE_RRING2:
	case EQUIP_TYPE_RRING3:
	case EQUIP_TYPE_BOOTS:				
		{
			UpdatePhyResist();
			UpdateAvoid();
			UpdateBlockRate();
			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwCode	= USER_PHY_RESIST;
			pUserStatus->pUserStatus[ pUserStatus->dwUsedSlot ].dwMin	= m_wPhyResist;
			pUserStatus->dwUsedSlot++;
			goto lb_success;
		}
		break;
	}
	
	return FALSE;

lb_success:
	return TRUE;
}


//=======================================
// 착용할수 있는 아이템인지 
//=======================================
BOOL CUser::CheckItem(CItem* pItem) const
{
	if (!pItem)	return FALSE;

#ifndef	EQUIP_CHK 	
	return TRUE;	
#else	
	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(pItem->GetID());
	
	if(!pBaseItem)
		return FALSE;
	
	BOOL bClass = FALSE;
	
	BYTE byClassId = pBaseItem->GetClass();
	
	if(byClassId&(BYTE)pow(2, GetClass()-1))
	{
		bClass = TRUE;
	}

#ifdef __USE_STAT_LIMIT	
	if(bClass)
	{
		WORD wItemId = pItem->GetID();
		
		if(ITEM_TYPE_LARGE == GetItemType(wItemId))
		{
			DWORD dwItemKind =  GetItemKind(wItemId);
			switch ( dwItemKind )
			{
				case ITEM_KIND_WEAPON :
				{
					if(GetCheckStr() >= pBaseItem->BaseItem_Weapon.wMin_Str)
					{					
						if(GetLevel() >= pBaseItem->BaseItem_Weapon.wMin_Level)
							return TRUE;
					}
				}
				break;
				case ITEM_KIND_AROMR :
				{
					if(GetCheckStr()>=pBaseItem->BaseItem_Armor.wMin_Str)							
					{					
						if(GetLevel()>=pBaseItem->BaseItem_Armor.wMin_Level)
							return TRUE;
					}
				}
				break;
				case ITEM_KIND_BAG : //  가방아이템 능력치 제한 체크안함 김영대 2005.02.22
				{
					return TRUE;
				}
			}
		}
	}	
	return FALSE;
#endif 
	return bClass;
#endif	
}

DWORD CUser::GetCheckStr() const
{
	DWORD					dwStr			= 0;
	int						i				= 0;
	int						j				= 0;
	int						iLoop			= 0;
	int						iItemKind		= 0;
	CBaseItem*				pBaseItem		= NULL;	
	USER_SET_ITEM_INFO*		pUserSetItem	= NULL;
	LPSET_ITEM_INFO			pSetItemInfo	= NULL;

	ListNode<USER_SET_ITEM_INFO>* pNode	= g_pUserSetItemHash->GetHead();
	
	while(pNode)
	{
		pUserSetItem = pNode->pData;
		pSetItemInfo = g_pSetItemInfoHash->GetData( pUserSetItem->dwSetID );
				
		iLoop = pUserSetItem->wCnt - 1;
		
		if(pUserSetItem->wCnt > 5)
			iLoop = 5;

		for( i = 0; i < iLoop; i++ )
		{
			if( pSetItemInfo->pBonusOption[i][0].wKind==ITEM_ATTR_STR ||
				pSetItemInfo->pBonusOption[i][0].wKind==ITEM_ATTR_ALLSTATUS) 
			{
				dwStr += pSetItemInfo->pBonusOption[i][0].wValue;
			}
			if( pSetItemInfo->pBonusOption[i][1].wKind==ITEM_ATTR_STR ||
				pSetItemInfo->pBonusOption[i][1].wKind==ITEM_ATTR_ALLSTATUS) 
			{
				dwStr += pSetItemInfo->pBonusOption[i][1].wValue;			
			}
		}
		
		if(pUserSetItem->isFullSet)
		{
			for(i = 0; i < 8; i++)
			{
				if(pSetItemInfo->pFullOption[i].wKind==0) 
					continue;

				if(	pSetItemInfo->pFullOption[i].wKind==ITEM_ATTR_STR ||
					pSetItemInfo->pFullOption[i].wKind==ITEM_ATTR_ALLSTATUS) 
				{
					dwStr += pSetItemInfo->pFullOption[i].wValue;				
				}
			}
		}

		pNode = pNode->pNext;
	}		
	
	for(i = 0; i < MAX_EQUIP_POOL; i++)
	{
		if(m_bCurrentHand==1)
		{
			if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
				continue;
		}
		else if(m_bCurrentHand==2)
		{
			if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
				continue;
		}			

		if(	i==EQUIP_TYPE_AMULE1 || i==EQUIP_TYPE_AMULE3 || i==EQUIP_TYPE_LRING1 ||
			i==EQUIP_TYPE_LRING2 || i==EQUIP_TYPE_LRING3)
			continue;

		if(i==EQUIP_TYPE_RIDE)
			continue;

		if(!m_pEquip[i].m_wItemID)
			continue;					
		
		pBaseItem = g_pBaseItemHash->GetData(m_pEquip[i].m_wItemID);

		if(!pBaseItem)
			continue;

		iItemKind = m_pEquip[i].m_wItemID / ITEM_DISTRIBUTE;

		DWORD dwSetID = pBaseItem->GetSetID();
		
		if( dwSetID !=0 && dwSetID != 32767 )		// SetItem ?
		{
			pSetItemInfo = g_pSetItemInfoHash->GetData( dwSetID );
			pUserSetItem = g_pUserSetItemHash->GetData( dwSetID );

			if(!pSetItemInfo || !pUserSetItem)
				continue;

			// 옵션 적용시작.
			if( pUserSetItem->wCnt >= 2 )
			{
				// 아이템 각각의 세트 추가옵션.
				iLoop = pUserSetItem->wCnt - 1;
				if( pUserSetItem->wCnt > 4 ) iLoop = 4;

				if( iItemKind >= 10 && iItemKind <= 17 )		
				{
					// Armor..
					for( j = 0; j < iLoop; j++ )
					{
						if(	pBaseItem->BaseItem_Armor.pPartOption[j].wKind==ITEM_ATTR_STR ||
							pBaseItem->BaseItem_Armor.pPartOption[j].wKind==ITEM_ATTR_ALLSTATUS) 
						{
							dwStr += pBaseItem->BaseItem_Armor.pPartOption[j].wValue;
						}
					}
				}
				else										
				{
					// Weapon..
					for( j = 0; j < iLoop; j++ )
					{
						if(	pBaseItem->BaseItem_Weapon.pPartOption[j].wKind==ITEM_ATTR_STR ||
							pBaseItem->BaseItem_Weapon.pPartOption[j].wKind==ITEM_ATTR_ALLSTATUS) 
						{
							dwStr += pBaseItem->BaseItem_Weapon.pPartOption[j].wValue;												
						}
					}
				}
			}
		}
	}

	return GetNewestStr()+dwStr;
}

DWORD CUser::GetCheckDex()
{
	DWORD					dwDex		= 0;
	int						i			= 0;
	int						j			= 0;
	int						iLoop		= 0;
	int						iItemKind	= 0;
	CBaseItem*				pBaseItem;	
	USER_SET_ITEM_INFO*		pUserSetItem;
	LPSET_ITEM_INFO			pSetItemInfo;

	ListNode<USER_SET_ITEM_INFO>* pNode	= g_pUserSetItemHash->GetHead();
	
	while(pNode)
	{
		pUserSetItem = pNode->pData;
		pSetItemInfo = g_pSetItemInfoHash->GetData( pUserSetItem->dwSetID );
				
		iLoop = pUserSetItem->wCnt - 1;
		
		if(pUserSetItem->wCnt > 5)
			iLoop = 5;

		for( i=0; i<iLoop; i++ )
		{
			if(pSetItemInfo->pBonusOption[i][0].wKind==ITEM_ATTR_DEX || pSetItemInfo->pBonusOption[i][0].wKind==ITEM_ATTR_ALLSTATUS) 
				dwDex += pSetItemInfo->pBonusOption[i][0].wValue;
			if(pSetItemInfo->pBonusOption[i][1].wKind==ITEM_ATTR_DEX || pSetItemInfo->pBonusOption[i][1].wKind==ITEM_ATTR_ALLSTATUS) 
				dwDex += pSetItemInfo->pBonusOption[i][1].wValue;			
		}
		
		if(pUserSetItem->isFullSet)
		{
			for(i = 0; i < 8; i++)
			{
				if(pSetItemInfo->pFullOption[i].wKind==0) 
					continue;

				if(pSetItemInfo->pFullOption[i].wKind==ITEM_ATTR_DEX || pSetItemInfo->pFullOption[i].wKind==ITEM_ATTR_ALLSTATUS) 
					dwDex += pSetItemInfo->pFullOption[i].wValue;				
			}
		}
		pNode = pNode->pNext;
	}		
	
	for(i = 0; i < MAX_EQUIP_POOL; i++)
	{
		if(m_bCurrentHand==1)
		{
			if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
				continue;
		}
		else if(m_bCurrentHand==2)
		{
			if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
				continue;
		}	
		
		if(	i==EQUIP_TYPE_AMULE1 || i==EQUIP_TYPE_AMULE3 ||
			i==EQUIP_TYPE_LRING1 || i==EQUIP_TYPE_LRING2 || i==EQUIP_TYPE_LRING3)
			continue;

		if(i==EQUIP_TYPE_RIDE)
			continue;

		if(!m_pEquip[i].m_wItemID)
			continue;					
		
		pBaseItem = g_pBaseItemHash->GetData(m_pEquip[i].m_wItemID);

		if(!pBaseItem)
			continue;

		iItemKind = m_pEquip[i].m_wItemID / ITEM_DISTRIBUTE;

		DWORD dwSetID = pBaseItem->GetSetID();
		
		if( dwSetID !=0 && dwSetID != 32767 )		// SetItem ?
		{
			pSetItemInfo = g_pSetItemInfoHash->GetData( dwSetID );
			pUserSetItem = g_pUserSetItemHash->GetData( dwSetID );

			if(!pSetItemInfo || !pUserSetItem)
				continue;

			// 옵션 적용시작.
			if( pUserSetItem->wCnt >= 2 )
			{
				// 아이템 각각의 세트 추가옵션.
				iLoop = pUserSetItem->wCnt - 1;
				if( pUserSetItem->wCnt > 4 ) iLoop = 4;

				if( iItemKind >=10 && iItemKind <= 17 )		// Armor..
				{
					for( j=0; j<iLoop; j++ )
					{
						if(pBaseItem->BaseItem_Armor.pPartOption[j].wKind==ITEM_ATTR_DEX || pBaseItem->BaseItem_Armor.pPartOption[j].wKind==ITEM_ATTR_ALLSTATUS) 
							dwDex += pBaseItem->BaseItem_Armor.pPartOption[j].wValue;
					}
				}
				else										// Weapon..
				{
					for( j=0; j<iLoop; j++ )
					{
						if(pBaseItem->BaseItem_Weapon.pPartOption[j].wKind==ITEM_ATTR_DEX || pBaseItem->BaseItem_Weapon.pPartOption[j].wKind==ITEM_ATTR_ALLSTATUS) 
							dwDex += pBaseItem->BaseItem_Weapon.pPartOption[j].wValue;												
					}
				}
			}
		}
	}

	return GetNewestDex()+dwDex;
}

WORD CUser::GetCurrentWeaponID() const
{	
	if(m_bCurrentHand==1)
	{
		if(CheckItem((CItem*)&m_pEquip[EQUIP_TYPE_RHAND1])==TRUE)
			return m_pEquip[EQUIP_TYPE_RHAND1].GetID();
	}
	else if(m_bCurrentHand==2)
	{
		if(CheckItem((CItem*)&m_pEquip[EQUIP_TYPE_RHAND2])==TRUE)
			return m_pEquip[EQUIP_TYPE_RHAND2].GetID();
	}
	
	return 0;
}

void CUser::RemoveEquipItem( USER_STATUS* pUserStatus, BYTE bEquipNum )
{
	// 장착장의 bEquipNum에 아이템이 있는지를 검사한다.
	if( m_pEquip[bEquipNum].m_wItemID == 0 )
		return;
}

DWORD CUser::GetItemTotalSize(ITEM_NATIVE eItemNative) const
{
	return m_pItemNativeManager->GetItemTotalSize((BYTE)eItemNative);
}


ITEM_NATIVE_TYPE CUser::GetItemNativeType(BYTE byNativeID) const
{
	return m_pItemNativeManager->GetType(byNativeID);
}


const CItem* CUser::GetItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex) const
{
	assert(m_pItemNativeManager->GetType((BYTE)eItemNative) == ITEM_NATIVE_TYPE_REAL);

	return (CItem*)m_pItemNativeManager->GetItem((BYTE)eItemNative, bySlotIndex);
}


const ITEM_NATIVE_INFO* CUser::GetItemLink(ITEM_NATIVE eItemNative, BYTE bySlotIndex)
{
	assert(m_pItemNativeManager->GetType((BYTE)eItemNative) == ITEM_NATIVE_TYPE_VIRTUAL);

	return (ITEM_NATIVE_INFO*)m_pItemNativeManager->GetItem((BYTE)eItemNative, bySlotIndex);
}


void CUser::SetItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex, const CItem* pItem)
{
	assert(m_pItemNativeManager->GetType((BYTE)eItemNative) == ITEM_NATIVE_TYPE_REAL);

	m_pItemNativeManager->SetItem((BYTE)eItemNative, bySlotIndex, pItem);
}


void CUser::SetItemLink(ITEM_NATIVE eFromItemNative, BYTE byFromSlotIndex, const ITEM_NATIVE_INFO* pToItemNativeLinkInfo)
{
	assert(m_pItemNativeManager->GetType((BYTE)eFromItemNative) == ITEM_NATIVE_TYPE_VIRTUAL);

	m_pItemNativeManager->SetItem((BYTE)eFromItemNative, byFromSlotIndex, pToItemNativeLinkInfo);

	m_byItemNativeLinkCount += 1;
	m_CureItemNativeLink = eFromItemNative;
}


void CUser::RemoveItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex)
{
	m_pItemNativeManager->RemoveItem((BYTE)eItemNative, bySlotIndex);
}


void CUser::RemoveItemLink(ITEM_NATIVE eItemNative, BYTE bySlotIndex)
{
	assert(m_pItemNativeManager->GetType((BYTE)eItemNative) == ITEM_NATIVE_TYPE_VIRTUAL);

	m_pItemNativeManager->RemoveItem((BYTE)eItemNative, bySlotIndex);

	m_byItemNativeLinkCount -= 1;

	assert(m_byItemNativeLinkCount >= 0);

	if(!m_byItemNativeLinkCount)
		m_CureItemNativeLink = ITEM_NATIVE_PLAYER_START;
}


BOOL CUser::IsCorrectItemNativeLink(ITEM_NATIVE eItemNative)
{
	if(m_CureItemNativeLink == ITEM_NATIVE_PLAYER_START)
	{
		// 현재 유저가 링크 한개도 안시킨거다.
		return TRUE;
	}
	else if(m_CureItemNativeLink == eItemNative)
	{
		// 현재 유저가 링크한 창에 또 놓은거다.잘했다.
		return TRUE;
	}
	else if(m_CureItemNativeLink != ITEM_NATIVE_PLAYER_START &&
			m_CureItemNativeLink != eItemNative)
	{
		// 한놈만 패라!!!
		return FALSE;
	}

	return FALSE;
}


DWORD CUser::GetItemNativeLinkCount()
{
	DWORD dwVirtualLinkItemNativeHashCount = m_pVirtualLinkItemNativeHash->GetCount();
	return dwVirtualLinkItemNativeHashCount;
}


ITEM_NATIVE CUser::GetCurItemNativeLink()
{
	return m_CureItemNativeLink;
}


void CUser::SetPlayerShopCustomSellPrice(DWORD dwCustomSellPrice,BYTE bySlotIndex)
{
	assert(dwCustomSellPrice > 0);
	assert(MAX_PLAYER_SHOP_INV > bySlotIndex);
	m_sPlayerShopLink.dwCustomSellPrice[bySlotIndex] = dwCustomSellPrice;
}


DWORD CUser::GetPlayerShopCustomSellPrice(BYTE bySlotIndex)
{
	assert(MAX_PLAYER_SHOP_INV > bySlotIndex);

	DWORD dwCustomSellPrice = m_sPlayerShopLink.dwCustomSellPrice[bySlotIndex];

	return dwCustomSellPrice;
}


void CUser::RemovePlayerShopCustonSellPrice(DWORD dwCustomSellPrice,BYTE bySlotIndex)
{
	assert(dwCustomSellPrice == 0);
	assert(MAX_PLAYER_SHOP_INV > bySlotIndex);
	m_sPlayerShopLink.dwCustomSellPrice[bySlotIndex] = dwCustomSellPrice;
}


void CUser::RemovePlayerShopTitle()
{
	memset( m_sPlayerShopLink.szTitle, 0, sizeof(m_sPlayerShopLink.szTitle) );
}


void CUser::SetPlayerShopMode(BYTE bPlayerShopMode)
{
	m_sPlayerShopLink.m_bPlayerShopMode = bPlayerShopMode;
}


void CUser::SetPlayerShopTitle(LPSTR lpTitle)
{
	lstrcpy(m_sPlayerShopLink.szTitle, lpTitle);
}


BOOL CUser::IsUserVirtualItemNativeLink(const CItem* pItemNative)
{
	CItem cItem;
	memcpy(&cItem,pItemNative,sizeof(CItem));

	DWORD wKind = GetItemKind(cItem.GetID());

	if(wKind == ITEM_KIND_CONSUMABLE)
	{
		// 마일리지 아이템이면 현재 사용중인거인지 검사해야해..
		if( cItem.m_Item_Consumable.bInvIndex >= 1 )
		{
			return FALSE;
		}
	}

	if(cItem.GetID() == __ITEM_INSURANCE_INDEX__)
	{
		// 아이템 보험 적용 되어있어?
		if( cItem.m_Item_Zodiac.bInvIndex >= 1 )
		{
			return FALSE;
		}
	}

	if( cItem.IsOverlapItem() )
	{
		// Small아템중 겹쳐지는거.. 
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			return FALSE;
		}
		else
		{
			if( pItem->GetQuantity() == 0 )
			{
				// 수량 전부다 링크시켰잖아.
				return TRUE;
			}
		}
	}
	else
	{
		// 안겹쳐지는 딸랑 수량이 하나인 아템..
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CUser::IsUserVirtualItemNativeLink(const CItem* pItemNative,BYTE byLinkCnt)
{
	CItem cItem;
	memcpy(&cItem,pItemNative,sizeof(CItem));

	if( cItem.IsOverlapItem() )
	{
		// Small아템중 겹쳐지는거.. 
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			// 링크 시킬수 있다.
			return TRUE;
		}
		else
		{
			if( pItem->GetQuantity() >= byLinkCnt )
			{
				// 링크 시킬수 있다.
				return TRUE;
			}
			else
			{
				// 안돼!
				return FALSE;
			}
		}
	}
	else
	{
		// 안겹쳐지는 딸랑 수량이 하나인 아템..
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			// 링크 시킬수 있다.
			return TRUE;
		}
		else
		{
			// 뭐여 씨바 
			return FALSE;
		}
	}

	return FALSE;
}


//=============================================================================== 
// Inv Window => Link Window
//=============================================================================== 
BOOL CUser::SetUserVirtualItemNativeLink(const CItem* pItemNative,BYTE byLinkCnt)
{
	CItem cItem;
	memcpy(&cItem,pItemNative,sizeof(CItem));

	if( cItem.IsOverlapItem() )
	{
		// Small 아템중 겹쳐지는거.. 
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			// 링크 시킬수 있다.
			pItem = (CItem*)LALAlloc(g_pVirtualLinkItemNativePool);		

			if(pItem == NULL)
				return FALSE;

			memcpy(pItem,pItemNative,sizeof(CItem));
			pItem->SetQuantity( BYTE(pItem->GetQuantity() - byLinkCnt) );
			m_pVirtualLinkItemNativeHash->Insert(pItem, pItem->m_Serial.i64Serial);
			
			return TRUE;
		}
		else
		{
			if( pItem->GetQuantity() >= byLinkCnt )
			{
				// 링크 시킬수 있다.
				pItem->SetQuantity(BYTE(pItem->GetQuantity() - byLinkCnt));
				return TRUE;
			}
			else
			{
				// 안돼!
				return FALSE;
			}
		}
	}
	else
	{
		// 안겹쳐지는 딸랑 수량이 하나인 아템..
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			// 링크 시킬수 있다.
			pItem = (CItem*)LALAlloc(g_pVirtualLinkItemNativePool);		

			if(pItem == NULL)
				return FALSE;

			memcpy(pItem,pItemNative,sizeof(CItem));
			m_pVirtualLinkItemNativeHash->Insert(pItem, pItem->m_Serial.i64Serial);
			return TRUE;
		}
	}

	return FALSE;
}


//=============================================================================== 
// Link Window => Inv Window
//=============================================================================== 
BOOL CUser::SetUserVirtualItemNativeLinkReturn(const CItem* pItemNative,BYTE byLinkCnt,SET_LINKTOINV_TYPE eType)
{
	CItem cItem;
	memcpy(&cItem,pItemNative,sizeof(CItem));

	if( cItem.IsOverlapItem() )
	{
		// Small 아템중 겹쳐지는거..
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			// 어라.링크해쉬에 아이템이 없잖아.
			return FALSE;
		}
		else
		{
			if(cItem.GetQuantity() < pItem->GetQuantity() + byLinkCnt)
			{
				// 리턴 시킬수 없다.
				return FALSE;
			}
			else
			{
				if(eType == SET_LINK_TO_INV_EMPTY_SLOT)
				{
					if( cItem.GetQuantity() - byLinkCnt <= pItem->GetQuantity() )
					{
						// 지워버려 
						m_pVirtualLinkItemNativeHash->Delete(pItem, pItem->m_Serial.i64Serial);
						LALFree(g_pVirtualLinkItemNativePool, pItem);
						pItem = NULL;
					}
				}
				else if(eType == SET_LINK_TO_INV_NATIVE_SLOT)
				{
					pItem->SetQuantity(BYTE(pItem->GetQuantity() + byLinkCnt));
				
					if(cItem.GetQuantity() <= pItem->GetQuantity())
					{
						// 지워버려
						m_pVirtualLinkItemNativeHash->Delete(pItem, pItem->m_Serial.i64Serial);
						LALFree(g_pVirtualLinkItemNativePool, pItem);
						pItem = NULL;
					}
				}				

				return TRUE;
			}
		}
	}
	else
	{
		// 안겹쳐지는 딸랑 수량이 하나인 아템..
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			// 어라.링크해쉬에 아이템이 없잖아.
			return FALSE;
		}
		else
		{
			// 지워버려 
			m_pVirtualLinkItemNativeHash->Delete(pItem, pItem->m_Serial.i64Serial);
			LALFree(g_pVirtualLinkItemNativePool, pItem);
			pItem = NULL;
			return TRUE;
		}
	}
}


//===================================================================================
// 링크된 아이템이 다른 유저에게 팔렸다.
//===================================================================================
BOOL CUser::SetUserVirtualItemNativeLinkSell(const CItem* pItemNative,BYTE bySellCnt)
{
	CItem cItem;
	memcpy(&cItem,pItemNative,sizeof(CItem));

	if( cItem.IsOverlapItem() )
	{
		// Small 아템중 겹쳐지는거..
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
		if(NULL == pItem)
		{
			// 어라.링크해쉬에 아이템이 없잖아.
			return FALSE;
		}
		else
		{
			if(cItem.GetQuantity() < pItem->GetQuantity() + bySellCnt)
			{
				// 어라 팔릴리가 없는뎅 
				return FALSE;
			}
			else
			{
				if( cItem.GetQuantity() - bySellCnt <= pItem->GetQuantity() )
				{
					// 링크한 수량 다 팔린거야.
					// 지워버려 
					m_pVirtualLinkItemNativeHash->Delete(pItem, pItem->m_Serial.i64Serial);
					LALFree(g_pVirtualLinkItemNativePool, pItem);
					pItem = NULL;
				}

				return TRUE;
			}		
		}
	}
	else
	{
		// 안겹쳐지는 딸랑 수량이 하나인 아템..
		CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);

		if(NULL == pItem)
		{
			// 어라.링크해쉬에 아이템이 없잖아.
			return FALSE;
		}
		else
		{
			// 지워버려 
			m_pVirtualLinkItemNativeHash->Delete(pItem, pItem->m_Serial.i64Serial);
			LALFree(g_pVirtualLinkItemNativePool, pItem);
			pItem = NULL;
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CUser::RemoveUserVirtualItemNativeLink(const CItem* pItemNative)
{
	CItem cItem;
	memcpy(&cItem,pItemNative,sizeof(CItem));

	CItem* pItem = m_pVirtualLinkItemNativeHash->GetData(cItem.m_Serial.i64Serial);
	if(pItem)
	{
		m_pVirtualLinkItemNativeHash->Delete(pItem, pItem->m_Serial.i64Serial);
		LALFree(g_pVirtualLinkItemNativePool, pItem);
		pItem = NULL;
		return TRUE;
	}
	
	return FALSE;
}


void CUser::RemoveAllVirtualItemNativeLink()
{
	ListNode<CItem>* pVirtualLinkPos = m_pVirtualLinkItemNativeHash->GetHead();

	while(pVirtualLinkPos)
	{
		CItem* pItem = pVirtualLinkPos->pData;

		if(pItem)
		{
			m_pVirtualLinkItemNativeHash->Delete(pItem, pItem->m_Serial.i64Serial);
			LALFree(g_pVirtualLinkItemNativePool, pItem);
			pItem = NULL;
		}

		pVirtualLinkPos = pVirtualLinkPos->pNext;
	}
}


void CUser::ConvertItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex)
{
	::ConvertItem((CItem*)m_pItemNativeManager->GetItem((BYTE)eItemNative, bySlotIndex));
}


//====================================
// 무게 백분율  
//====================================
WORD CUser::GetAverageWeight()
{
	float fPerWeight = 0;

	if(m_pEquip[EQUIP_TYPE_BAG].GetID())
	{
		BYTE bType			= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.bType;
		WORD wProbability	= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.wProbability; 
		
		switch(bType)
		{
			case ITEM_BAG_TYPE_DECREASE_SMALL_BAG : // 스몰 아이템의 무게를 %d% 만큼 감소시켜주는 가방				
				{//((Sum_Large Item Weight)+(   (Sum_Small Item Weight)-%d   )/Total_Chr Weight)*100					
					int SmallSumWeight = GetSmallItemSumWeight();
					
					if(SmallSumWeight < 0)	SmallSumWeight = 0;
					
					int SumWeight = SmallSumWeight + GetEquipItemSumWeight() + GetInvLargeItemSumWeight();

					fPerWeight = (float)SumWeight / (float)GetAllSumWeight() * 100.f;
				}
				break;
			case ITEM_BAG_TYPE_INCREASE_TOTAL_BAG ://	캐릭터의 총 수용 무게를 %d% 만큼 상승 시켜주는 가방				
				{//((Sum_Large Item Weight)+(Sum_Small Item Weight))/((Total_Chr Weight)+%d)*100					
					int AllSumWeight = GetAllSumWeight() ;
					fPerWeight = (float)GetSumWeight() / (float)AllSumWeight * 100.f;
				}
				break;
			case ITEM_BAG_TYPE_INCREASE_TOTALPER_BAG ://	캐릭터의 총 수용 무게를 %d%% 만큼 상승 시켜주는 가방				
				{//"((Sum_Large Item Weight)+(Sum_Small Item Weight))/((Total_Chr Weight)+(Total_Chr Weight)*(%d%%))*100"					
					/*
					int AllSumWeight = GetAllSumWeight();
					float fPer = (float)wProbability / (float)AllSumWeight * 100.f;
					AllSumWeight = AllSumWeight + (int)fPer;
					fPerWeight = (float)GetSumWeight() / (float)AllSumWeight * 100.f;
					*/

					DWORD dwWeight = GetSumWeight();
					int AllSumWeight = GetAllSumWeight();
					fPerWeight = (float)dwWeight / (float)AllSumWeight * 100.f;					

				}
				break;
		}
	}
	else
	{
		fPerWeight = (float)GetSumWeight() / (float)GetAllSumWeight() * 100.f;
	}
	
	return (WORD)fPerWeight;
}


//====================================
// 내 인벤에 들고 있는 아이템의 총무게  
//====================================
WORD CUser::GetSumWeight()
{
	return WORD(GetEquipItemSumWeight() + GetInvLargeItemSumWeight() + GetSmallItemSumWeight());
}


//====================================
// 유저가 들수있는 아이템의 총무게 
//====================================
WORD CUser::GetAllSumWeight()
{
	DWORD dwAllSumWeight  = (1000 + (GetLevel() * 15) + GetNewestStr() * 2);
	WORD	wAllSumWeight	= (WORD)dwAllSumWeight;
	if(m_pEquip[EQUIP_TYPE_BAG].GetID())
	{
		BYTE bType			= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.bType;
		WORD wProbability	= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.wProbability;

		switch(bType)
		{
		case ITEM_BAG_TYPE_INCREASE_TOTAL_BAG://	캐릭터의 총 수용 무게를 %d% 만큼 상승 시켜주는 가방				
			{//((Sum_Large Item Weight)+(Sum_Small Item Weight))/((Total_Chr Weight)+%d)*100					
				int nTemp = wAllSumWeight + wProbability;
				if(nTemp < 0)
					wAllSumWeight = 0;
				else
					wAllSumWeight = wAllSumWeight + wProbability;	
			}
			break;
		case ITEM_BAG_TYPE_INCREASE_TOTALPER_BAG ://	캐릭터의 총 수용 무게를 %d%% 만큼 상승 시켜주는 가방				
			{//"((Sum_Large Item Weight)+(Sum_Small Item Weight))/((Total_Chr Weight)+(Total_Chr Weight)*(%d%%))*100"					

				DWORD dwWeight = GetSumWeight();
				float fProper = wProbability ;

				fProper = (float)(fProper/ 100);

				//float fPer = ( (float)dwWeight )/((dwAllSumWeight) +  ((dwAllSumWeight)*(fProper))  )*100.f ;					
				float fPer = (dwAllSumWeight)*(fProper);
				dwAllSumWeight = dwAllSumWeight + (DWORD)fPer;
				wAllSumWeight = dwAllSumWeight;
			}
			break;

		}		
	}

	return wAllSumWeight;
}


//==============================================
// 내 인벤에 들고 있는 장착 아이템의 총무게  
//==============================================
WORD CUser::GetEquipItemSumWeight()
{
	WORD wWeight = 0;

	for(int i = 0; i < MAX_EQUIP_POOL; i++)
	{
		if(m_pEquip[i].GetID())
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(m_pEquip[i].m_wItemID);
			if(pBaseItem)
			{
				wWeight += pBaseItem->GetWeight();
			}
		}					
	}

	return wWeight;
}


//==============================================
// 내 인벤에 들고 있는 인벤 라쥐 아이템의 총무게  
//==============================================
WORD CUser::GetInvLargeItemSumWeight()
{
	WORD wWeight = 0;

	for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
	{
		if(m_pInv_Large[i].GetID())
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(m_pInv_Large[i].m_wItemID);
			if(pBaseItem)
			{
				wWeight += pBaseItem->GetWeight();
			}
		}
	}

	return wWeight;
}


//==============================================
// 내 인벤,벨트에 들고 있는 스몰 아이템의 총무계  
//==============================================
WORD CUser::GetSmallItemSumWeight()
{
	WORD wWeight = 0;

	for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(m_pInv_Small[i].GetID())
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(m_pInv_Small[i].m_wItemID);
			if(pBaseItem)				
			{
				wWeight += (pBaseItem->GetWeight() * m_pInv_Small[i].GetQuantity());
			}
		}
	}

	for(i = 0; i < MAX_BELT_POOL; i++)
	{
		if(m_pBelt[i].GetID())
		{
			CBaseItem* pBaseItem = g_pBaseItemHash->GetData(m_pBelt[i].m_wItemID);
			if(pBaseItem)				
			{
				wWeight += (pBaseItem->GetWeight() * m_pBelt[i].GetQuantity());
			}
		}
	}

	if(m_pEquip[EQUIP_TYPE_BAG].GetID())
	{
		BYTE bType			= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.bType;
		WORD wProbability	= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.wProbability;

		switch(bType)
		{
		case ITEM_BAG_TYPE_DECREASE_SMALL_BAG:
			{
				int nTemp = wWeight - wProbability;
				
				if(nTemp < 0)
					wWeight = 0;
				else
					wWeight = wWeight - wProbability;	
			}
			break;
		}		
	}

	return wWeight;
}