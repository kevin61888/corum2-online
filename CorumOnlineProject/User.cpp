#include "InitGame.h"
#include "DungeonProcess.h"
#include "User.h"
#include "GameControl.h"
#include "hashtable.h"
#include "userinterface.h"
#include "CorumResource.h"
#include "message.h"
#include "objectmanager.h"
#include "dungeon_data_ex.h"
#include "Chat.h"
#include "PlayerShopWnd.h"
#include "CharDieWnd.h"
#include "EffectProcess.h"
#include "NetworkClient.h"
#include "Map.h"
#include "Effect.h"
#include "DamageNumber.h"
#include "ChrInfo.h"
#include "GameControl.h"
#include "CodeFun.h"
#include "MinboboWork.h"
#include "ItemNativeManager.h"
#include "ItemMove.h"
#include "GroupWnd.h"
#include "RivalGuildWar.h"

CMainUser::CMainUser()
{
	memset(this, 0, sizeof(CMainUser));
	
	m_bMoveType					= UNIT_STATUS_WALKING;
	m_bInEventDungeon			= FALSE;
}

CMainUser::~CMainUser()
{
	if (m_pItemNativeManager)
	{
		delete m_pItemNativeManager;
		m_pItemNativeManager = NULL;
	}
}


void CMainUser::RegistItemNativeInfo()
{
	m_pItemNativeManager	= new CItemNativeManager;
	
	m_pItemNativeManager->Initialize(::GetItemNativePlayerBufferCount(), ITEM_NATIVE_PLAYER_START);
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_EQUIP, m_pEquip, sizeof(m_pEquip));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_INV_LARGE, m_pInv_Large, sizeof(m_pInv_Large));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_INV_SMALL, m_pInv_Small, sizeof(m_pInv_Small));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_INV_GUARDIAN, m_pInv_Guardian, sizeof(m_pInv_Guardian));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_BELT, m_pBelt, sizeof(m_pBelt));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_BANK_LARGE, m_pBank_Large, sizeof(m_pBank_Large));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_BANK_SMALL, m_pBank_Small, sizeof(m_pBank_Small));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_TRADE, m_pItemTrade, sizeof(m_pItemTrade));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_UPGRADE, m_pUpgradeItem, sizeof(m_pUpgradeItem));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_MAKING, m_pMakingItem, sizeof(m_pMakingItem));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_PLAYERSHOP, m_sPlayerShop.cItem, sizeof(m_sPlayerShop.cItem));
	m_pItemNativeManager->RegistItemNativeInfo(ITEM_NATIVE_GUARDIAN, &m_GuardianItem, sizeof(CItem)); 
}


DWORD CMainUser::GetItemTotalSize(ITEM_NATIVE eItemNative) const
{
	return m_pItemNativeManager->GetItemTotalSize(BYTE(eItemNative));
}


void CMainUser::RemoveItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex)
{
	m_pItemNativeManager->RemoveItem(BYTE(eItemNative), bySlotIndex);
}


void CMainUser::ConvertItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex)
{
	m_pItemNativeManager->ConvertItem(m_pItemNativeManager->GetItem(BYTE(eItemNative), bySlotIndex));
}


const CItem* CMainUser::GetItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex) const
{
	return m_pItemNativeManager->GetItem(BYTE(eItemNative), bySlotIndex);
}


void CMainUser::SetItem(ITEM_NATIVE eItemNative, BYTE bySlotIndex, const CItem* pItem)
{
	m_pItemNativeManager->SetItem(BYTE(eItemNative), bySlotIndex, pItem);
}


WORD CMainUser::GetAllSumWeight()
{
	DWORD	dwAllSumWeight	= 1000 + (m_dwLevel*15) + m_dwStr*2; 
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

//				DWORD dwWeight = GetSumWeight();
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


WORD CMainUser::GetEquipItemSumWeight()
{
	DWORD dwWeight = 0;

	for(int i = 0; i < MAX_EQUIP_POOL; i++)
	{
		if(m_pEquip[i].GetID())
		{
			CBaseItem* lpItem = g_pItemTableHash->GetData(m_pEquip[i].GetID());

			if(lpItem)
				dwWeight += lpItem->GetWeight();
		}
	}

	return (WORD)dwWeight;
}


WORD CMainUser::GetInvLargeItemSumWeight()
{
	DWORD dwWeight = 0;

	for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
	{
		if(m_pInv_Large[i].GetID())
		{
			CBaseItem* lpItem = g_pItemTableHash->GetData(m_pInv_Large[i].GetID());

			if(lpItem)
				dwWeight += lpItem->GetWeight();
		}
	}

	return (WORD)dwWeight;
}


WORD CMainUser::GetLargeSumWeight()
{
	DWORD dwWeight = 0;
	dwWeight = GetEquipItemSumWeight() + GetInvLargeItemSumWeight();
/*  가방 아이템 재 기획으로 인해 사라짐 
// 2005.01.18 김영대
	if(m_pEquip[EQUIP_TYPE_BAG].GetID())
	{
		BYTE bType			= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.bType;
		WORD wProbability	= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.wProbability;

		switch(bType)
		{
		case ITEM_BAG_TYPE_LEATHER_BAG:
			{
				int nTemp = dwWeight - (wProbability/2);
				
				if(nTemp < 0)
					dwWeight = 0;
				else
					dwWeight = dwWeight - (wProbability/2);	
			}
			break;
		}		
	}
*/
	return (WORD)dwWeight;	
}


WORD CMainUser::GetSmallItemSumWeight()
{
	DWORD dwWeight = 0;

	for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(m_pInv_Small[i].GetID())
		{
			CBaseItem* lpItem = g_pItemTableHash->GetData(m_pInv_Small[i].GetID());

			if(lpItem)
			{
				DWORD dwSmallWeight = lpItem->GetWeight() * m_pInv_Small[i].GetQuantity();
				
				dwWeight += dwSmallWeight;
			}
		}
	}
	
	for(i = 0; i < MAX_BELT_POOL; i++)
	{
		if(m_pBelt[i].GetID())
		{
			CBaseItem* lpItem = g_pItemTableHash->GetData(m_pBelt[i].GetID());

			if(lpItem)
			{
				DWORD dwSmallWeight = lpItem->GetWeight() * m_pBelt[i].GetQuantity();
				
				dwWeight += dwSmallWeight;
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
				int nTemp = dwWeight - wProbability;
				
				if(nTemp < 0)
					dwWeight = 0;
				else
					dwWeight = dwWeight - wProbability;	
			}
			break;
		}		
	}

	return (WORD)dwWeight;
}

WORD CMainUser::GetSumWeight()
{
	DWORD dwWeight = 0;
	dwWeight = GetEquipItemSumWeight() + GetInvLargeItemSumWeight() + GetSmallItemSumWeight();	

	return (WORD)dwWeight;
}


//=============================
// 무게 퍼센테이지를 리턴한다.
//=============================
FLOAT CMainUser::GetAverageWeight(WORD wAppendWeight)
{
	float fPerWeight = 0;

	if(m_pEquip[EQUIP_TYPE_BAG].GetID())
	{
		BYTE bType			= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.bType;
//		WORD wProbability	= m_pEquip[EQUIP_TYPE_BAG].m_Item_Bag.wProbability; 
		
		switch(bType)
		{
			// 가방 아이템이 3종류로 변경 2005.01.18 김영대
			case ITEM_BAG_TYPE_DECREASE_SMALL_BAG : // 스몰 아이템의 무게를 %d% 만큼 감소시켜주는 가방				
				{//((Sum_Large Item Weight)+(   (Sum_Small Item Weight)-%d   )/Total_Chr Weight)*100					
					int SmallSumWeight = GetSmallItemSumWeight();// - wProbability;
					
					if(SmallSumWeight < 0)	SmallSumWeight = 0;
					int SumWeight = SmallSumWeight + GetEquipItemSumWeight() + GetInvLargeItemSumWeight();

					fPerWeight = (float)SumWeight / (float)GetAllSumWeight() * 100.f;
				}
				break;
			case ITEM_BAG_TYPE_INCREASE_TOTAL_BAG ://	캐릭터의 총 수용 무게를 %d 만큼 상승 시켜주는 가방				
				{
					int AllSumWeight = GetAllSumWeight() ;//+ wProbability;
					fPerWeight = (float)GetSumWeight() / (float)AllSumWeight * 100.f;
				}
				break;
			case ITEM_BAG_TYPE_INCREASE_TOTALPER_BAG ://	캐릭터의 총 수용 무게를 %d%% 만큼 상승 시켜주는 가방				
				{
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
	
	return fPerWeight;
}


void CMainUser::GetCheckUpgrade()
{
	BYTE byUpgrade[5] = {0, 0, 0, 0, 0};

	for(int i = 0; i < MAX_EQUIP_POOL; i++)
	{
		if(i==EQUIP_TYPE_RHAND1 ||i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND1 || i==EQUIP_TYPE_LHAND2)
		{
			if(m_bCurrnetHand==1)
			{
				if(i==EQUIP_TYPE_RHAND1 || i==EQUIP_TYPE_LHAND1)
				{
					int nValue = m_pEquip[i].GetID()/ITEM_DISTRIBUTE;
									
					if((nValue>=2 && nValue<=5) || nValue==7)
					{
						if(CheckItem(&m_pEquip[i]))
						{
							if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
								byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]+=2;
						}
					}
					else
					{
						if(CheckItem(&m_pEquip[i]))
						{
							if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
								byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]++;
						}						
					}					
				}
			}
			else if(m_bCurrnetHand==2)
			{
				if(i==EQUIP_TYPE_RHAND2 || i==EQUIP_TYPE_LHAND2)
				{
					int nValue = m_pEquip[i].GetID()/ITEM_DISTRIBUTE;
									
					if((nValue>=2 && nValue<=5) || nValue==7)
					{
						if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
							byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]+=2;
					}
					else
					{
						if(CheckItem(&m_pEquip[i]))
						{
							if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)
								byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]++;
						}					
					}
				}										
			}
		}
		else if(i==EQUIP_TYPE_HELMET	||
				i==EQUIP_TYPE_RGLOVE	||
				i==EQUIP_TYPE_ARMOR		||
				i==EQUIP_TYPE_BELT		||
				i==EQUIP_TYPE_LGLOVE	||
				i==EQUIP_TYPE_BOOTS)
		{
			if(m_pEquip[i].m_bSlot.uUsedSlot>=6 && m_pEquip[i].m_bSlot.uUsedSlot<=10)	
			{
				if(CheckItem(&m_pEquip[i]))
				{
					byUpgrade[m_pEquip[i].m_bSlot.uUsedSlot-6]++;
				}
			}				
		}
	}

	for(i = 0; i < 5; i++)
	{
		if(byUpgrade[i]==8)
		{
			m_byUpgrade = BYTE(i+6);
			
			SetEffectIndexAll(this, i+1);	

			return;
		}
	}
	
	SetEffectIndexAll(this, 0xffffffff);
	
	m_byUpgrade = 0;
}


BOOL CMainUser::IsAlliance(CUser* pUser)
{
	// 이벤트 던젼일경우 
	if(m_bInEventDungeon)
	{
		if(m_bInEvnetDungeonFirstTimeOut == TRUE && this != pUser)
		{
			return FALSE;
		}		
		else if(m_bInEvnetDungeonFirstTimeOut == FALSE)
		{
			return	(this == pUser) ||
					(m_dwPartyId && m_dwPartyId == pUser->m_dwPartyId) ||
					(m_dwGuildId && m_dwGuildId == pUser->m_dwGuildId);						
		}
	}

	if (m_bMatching == TRUE && this != pUser)
	{
		return FALSE;
	}
	else
	{
		if( this != pUser// 자신은 아니고
		&&	g_pThisDungeon->IsStadium() )//길드전 맵이고 : hwoarang 050202
		{
			if( G_W_F_PLAYER & this->m_dwGuildWarFlag  //Player 서로 Player 플래그이고
			&&	G_W_F_PLAYER & pUser->m_dwGuildWarFlag )
			{
				return (this->m_team_index == pUser->m_team_index );
			}
		}

		return	(this == pUser) ||
				(m_dwPartyId && m_dwPartyId == pUser->m_dwPartyId) ||
				(m_dwGuildId && m_dwGuildId == pUser->m_dwGuildId);
	}
	return TRUE;
}


BOOL CMainUser::IsAlliance(CMonster* pMonster)
{
	BOOL bAlliance =	(pMonster->m_dwLordIndex == m_dwUserIndex) 
					||	(m_bAttackMode == ATTACK_MODE_DEFENSE 
					&& pMonster->m_dwMonsterKind == OBJECT_TYPE_GUARDIAN 
					&& !pMonster->m_dwLordIndex);	// 방어자일 경우에 수호가디언을 공격하지 못해.
	if (!bAlliance)
	{
		CUser* pUser = g_pUserHash->GetData(pMonster->m_dwLordIndex);
		if (pUser)
		{
			bAlliance = IsAlliance(pUser);
		}
	}

	return bAlliance;
}

BOOL CMainUser::IsGMUser() const
{
	return BOOL(USER_TYPE_GM == m_bUserType);
}


BOOL CMainUser::IsAttack(CUser* pUser, BOOL bAuto)
{
	// 대만은 유저의 무게 제한이 포화상태이면  
	// 공격을 못하게 한다.
#if IS_TAIWAN_LOCALIZING()
	if (GetAverageWeight() >= 100.f)
	{
		return FALSE;
	}
#endif

	BOOL bCheck = FALSE;
	if(!( g_pMainPlayer->GetSkillKind(0) == __SKILL_ATTACK__ && (g_Mouse.bLDown == true || bAuto) ))
	{
		return FALSE;
	}
	else if( g_dwCurTick - m_dwTemp[ USER_TEMP_LASTATTACKTICK ]	< m_wAttackSpeed )
	{
		return FALSE;
	}
	else if (!pUser || IsAlliance(pUser))
	{
		return FALSE;
	}

	if(g_pThisDungeon->IsStadium() )//길드전 맵이고 : hwoarang 050202
	{
		bCheck = TRUE;
	}

//-> 사라져야함
	ListNode<SGUILD_WAR>* lpGuildWarPos = g_pGuildWarHash->GetHead();

	while(lpGuildWarPos)
	{
		LP_GUILD_WAR lpGuildWar = lpGuildWarPos->pData;

		if(lpGuildWar)
		{
			if(lpGuildWar->dwGuildId==pUser->m_dwGuildId)
			{
				bCheck = TRUE;
				break;
			}
		}
		lpGuildWarPos = lpGuildWarPos->pNext;
	}
//<- 
	// PK 모드가 아니라면 그냥 이동.
	if( !bCheck && !CUserInterface::GetInstance()->m_nPK )
	{
		return FALSE;	
	}
	// 죽은놈 클릭하면 무시.
	else if (pUser->GetStatus() == UNIT_STATUS_DEAD)
	{
		return FALSE;	
	}
	else if( CalcDistance( &m_v3CurPos, &pUser->m_v3CurPos ) > 250.0f )
	{
		if (bAuto)
		{
			m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
			m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0; 
		}
		return FALSE;
	}	
	else if( GetStatus() == UNIT_STATUS_NORMAL || GetStatus() == UNIT_STATUS_ATTACKING )
	{
		return TRUE;
	}

	return FALSE;
}


BOOL CMainUser::IsAttack(CMonster* pMonster, BOOL bAuto)
{
	// 대만은 유저의 무게 제한이 포화상태이면  
	// 공격을 못하게 한다.
#if IS_TAIWAN_LOCALIZING()
	if (GetAverageWeight() >= 100.f)
	{
		//"포화무게한도에 도달하여 달리기가 안되며, 포션의 사용 딜레이가 증가합니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE986].szMessage, 0xffff0000);			

		return FALSE;
	}
#endif

	if(!( g_pMainPlayer->GetSkillKind(0) == __SKILL_ATTACK__  && (g_Mouse.bLDown == true || bAuto) ))
	{
		return FALSE;
	}
	else if( g_dwCurTick - m_dwTemp[ USER_TEMP_LASTATTACKTICK ] < m_wAttackSpeed )
	{
		return FALSE;
	}
	else if (!pMonster || IsAlliance(pMonster))
	{
		return FALSE;
	}
	
	BOOL bChk = FALSE;

	if(g_pThisDungeon->IsStadium() )//길드전 맵이고 : hwoarang 050202
	{
		bChk = TRUE;
	}

	if(pMonster->m_dwLordIndex)
	{								
		CUser* pUser = g_pUserHash->GetData(pMonster->m_dwLordIndex);

		if(pUser)
		{
			if(g_pGuildWarHash->GetData(pUser->m_dwGuildId))
				bChk = TRUE;									
		}
	}

	if(!bChk)
	{
		if (pMonster->m_dwLordIndex && !CUserInterface::GetInstance()->m_nPK)
			return FALSE;
	}	
		
	// 아메리타트는 거대해서 타격 범위가 넓다 : 최덕석 2005.2.14
	if( CalcDistance( &m_v3CurPos, &pMonster->m_v3CurPos ) > (pMonster->m_wModNo == 630 ? 700.0f : 250.0f) )
	{
		if (bAuto)
		{
			m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
			m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0; 
		}
		return FALSE;
	}
	else if( GetStatus() == UNIT_STATUS_NORMAL || GetStatus() == UNIT_STATUS_ATTACKING)
	{
		return TRUE;
	}

	return FALSE;	
}

BYTE CUser::GetStatus()
{
	return m_bStatus;
}


void CUser::SetStatus(BYTE bStatus, BOOL bLive)
{
	if(!bLive && m_bStatus==UNIT_STATUS_DEAD)
	{
		return;
	}

	m_bStatus	= bStatus;

	if(bStatus==UNIT_STATUS_DAMAGING)
	{
	}	
	if(bStatus == UNIT_STATUS_DEAD)
	{
		g_pMap->SetTileOccupied( m_pCurTile->wIndex_X, m_pCurTile->wIndex_Z, TILE_EMPTY, this );

		if (!m_bMatching && g_pMainPlayer == this && g_pThisDungeon->GetDungeonType() != DUNGEON_TYPE_EVENT)
		{
			CCharDieWnd::GetInstance()->SetActive();
		}
		
		if (this == g_pMainPlayer)
		{
			g_pMainPlayer->m_dwTemp[USER_TEMP_AUTO_TARGET_INDEX] = 0;
			g_pMainPlayer->m_dwTemp[USER_TEMP_LASTATTACKTICK] = g_dwCurTick;
		}
		else if( m_dwUserIndex == g_pMainPlayer->m_dwTemp[USER_TEMP_AUTO_TARGET_INDEX] )
		{
			g_pMainPlayer->m_dwTemp[USER_TEMP_AUTO_TARGET_INDEX] = 0;
			g_pMainPlayer->m_dwTemp[USER_TEMP_LASTATTACKTICK] = g_dwCurTick;
		}

		SetAction(MOTION_TYPE_DYING, 0, ACTION_ONCE);
		m_hPlayer.pDesc->ObjectFunc = PlayerKillFunc;		
	}
}


void CUser::CreateResource()
{
	m_pEffectList = new COnlyList(50);
	m_pUsingStatusEffectList = new COnlyList(50);
	m_dwTemp[USER_TEMP_100MILLI] = 0;	
	m_pSkillPacket = new CTDS_SKILL;
	memset(m_pSkillPacket, 0, sizeof(CTDS_SKILL));
}

void CUser::RemoveResource()
{
	DeleteMagicArray();
	memset(m_dwTemp, 0, sizeof(m_dwTemp));
	m_bBad = false;
	RemoveAllStatusEffect();
	if (m_pSkillPacket)
	{
		delete m_pSkillPacket;
		m_pSkillPacket = NULL;
	}

	if(m_pEffectList)
		delete m_pEffectList;
	m_pEffectList = NULL;

	if (m_pPKEffect)	
	{
		RemoveEffectDesc(m_pPKEffect);
		m_pPKEffect = NULL;
	}

	if (m_pOwnerEffect)
	{
		RemoveEffectDesc(m_pOwnerEffect);
		m_pOwnerEffect = NULL;
	}

	if(m_pUsingStatusEffectList)
		delete m_pUsingStatusEffectList;
	m_pUsingStatusEffectList = NULL;
//	g_pDigitalAudioSet->Stop(m_hCastingSoundHandle);
	
	if (m_pEffectAttackMode)
	{
		char szLog[0xff] = {0,};
		wsprintf(szLog, "Remove 0x%p\n", m_pEffectAttackMode);
		OutputDebugString(szLog);

		RemoveEffectDesc(m_pEffectAttackMode);
		m_pEffectAttackMode = NULL;
	}

	// seven
	if (m_hPlayerEmoticon.pHandle)
	{
		DeleteHandleObject(m_hPlayerEmoticon.pHandle);
		m_hPlayerEmoticon.pHandle = NULL;
		FreeObjDesc( m_hPlayerEmoticon.pDesc );
		m_hPlayerEmoticon.pDesc = NULL;
	}	
}

BOOL CUser::UsingStatusSkill(BYTE bSkillKind)
{
	return m_pEffectDesc[bSkillKind] != NULL;
}


void CUser::AttachSkill(BYTE bSkillKind, BYTE bSkillLevel, DWORD dwEndTime)
{	
	EffectDesc*	pEffectDesc = m_pEffectDesc[bSkillKind];
	Effect*		pEffect = g_pEffectLayer->GetEffectInfo(bSkillKind);

	// 오버라이드 3개 이하까지는 중첩된 이펙트를 보여줘야 하기 때문에..
	// 기존 이펙트를 감추고 새로운 이펙트를 만들어 준다.

	if (!pEffectDesc)
	{
		// 새로 생성되긴 하지만 기존에 오버드라이브 이펙트가 보여지기는 하지만 중첩된 이미지로 교체시켜야 해야 한다.
		if (pEffect->bType == TYPE_DRIVE)
		{
			POSITION_ pos = m_pUsingStatusEffectList->GetHeadPosition();
			while(pos)
			{
				// 숨겨라.
				EffectDesc* pEffectDescTemp = (EffectDesc*)m_pUsingStatusEffectList->GetNext(pos);
				
				if (pEffectDescTemp->pEffect->bType == TYPE_DRIVE)
					g_pExecutive->DisableRender(pEffectDescTemp->hEffect.pHandle);
			}
		}
		
		pEffectDesc = g_pEffectLayer->CreateStatusEffect(bSkillKind, 0, g_pMainPlayer == this);
		pEffectDesc->pUsingStatus = m_pUsingStatusEffectList->AddTail(pEffectDesc);
		m_pEffectDesc[bSkillKind] = pEffectDesc;
		
		pEffectDesc->dwCount = 1;
		::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_LOOP );
		pEffectDesc->byTargetObjectType[0] = OBJECT_TYPE_PLAYER;
		pEffectDesc->dwTargetIndex[0] = m_dwUserIndex;
		pEffectDesc->dwSkillLevel	= bSkillLevel;

		switch(pEffectDesc->pEffect->GetEffectPosition())
		{
		case EFFECT_TYPE_STATUS_TOP:
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillUserStatusTopFunc;
			break;

		case EFFECT_TYPE_STATUS_CENTER:
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillUserStatusCenterFunc;
			break;

		case EFFECT_TYPE_STATUS_BOTTOM:
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillUserStatusBottomFunc;
			break;
		default:
			::SetAction( pEffectDesc->hEffect.pHandle, 1, 0, ACTION_ONCE_HIDE );
			pEffectDesc->hEffect.pDesc->ObjectFunc = EffectSkillUserStatusTopFunc;
		}
		
		pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_1SEC] = g_dwCurTick;
		if (pEffect->bType == TYPE_DRIVE)
		{
			++m_bOverDriveCount;
			::SetAction(pEffectDesc->hEffect.pHandle, (WORD)min(m_bOverDriveCount, 3), 0, ACTION_LOOP );
		}
	}	
	
	pEffectDesc->dwTemp[SKILL_TEMP_EFFECT_TIME_FINISH] = 0;	// 상태 마법 끝나기 위한 플래그.
	
	pEffectDesc->hEffect.pDesc->dwDestTime  = g_dwCurTick+dwEndTime;
	pEffectDesc->hEffect.pDesc->dwStartTick = g_dwCurTick;
}

void CUser::SetActionCasting(BYTE bSkillKind, VECTOR3 &vecTarget, BOOL bDirection)
{
	if (m_bStatus == UNIT_STATUS_DEAD)
		return;

	m_bSkillTmp = bSkillKind;
	
	SetAction(MOTION_TYPE_CHARGE, 0, ACTION_LOOP);
	SetStatus(UNIT_STATUS_CASTING);
	m_hPlayer.pDesc->ObjectFunc	= PlayerSkillFunc;
	
	// 유저 캐릭터를 스킬을 걸 방향으로 처다 보라.
	if (bDirection)
	{
		VECTOR3	v3DirMon;
		VECTOR3_SUB_VECTOR3( &v3DirMon, &vecTarget, &m_v3CurPos );
		g_pExecutive->GXOSetDirection( m_hPlayer.pHandle, &g_Camera.v3AxsiY, (float)(atan2(v3DirMon.z, v3DirMon.x) + DEG90 ) );
	}
	
	if (m_pEffectMagicArray == NULL)
		m_pEffectMagicArray = g_pEffectLayer->CreateMagicArray(bSkillKind, &m_v3CurPos, g_pMainPlayer == this);	
	
//	m_hCastingSoundHandle = _PlaySound(bSkillKind, SOUND_EFFECT_CASTING1,1);
}

void CUser::SetActionSkill(BYTE bSkillKind)
{
	switch(bSkillKind)
	{// 유저 액션
	
	case __SKILL_ZEAL__:
		SetAction( MOTION_TYPE_ATTACK1_1, 0, ACTION_ONCE );
		break;

	case __SKILL_BASH__:
		SetActionNext( MOTION_TYPE_BASH, MOTION_TYPE_WARSTAND, ACTION_LOOP, 0 );
		break;

	case __SKILL_RAGINGSWORD__:
		SetActionNext( MOTION_TYPE_RAGINGSWORD, MOTION_TYPE_WARSTAND, ACTION_LOOP, 0 );
		break;
	
	case __SKILL_LIGHTNINGBREAK__:
		SetActionNext( MOTION_TYPE_LIGHTNINGBREAK, MOTION_TYPE_WARSTAND, ACTION_LOOP, 0 );
		break;

	case __SKILL_BLASTQUAKE__:
		SetActionNext( MOTION_TYPE_BLASTQUAKE, MOTION_TYPE_WARSTAND, ACTION_LOOP, 0 );
		break;
	case __SKILL_CHAINATTACKER__:
		SetActionNext( MOTION_TYPE_CHAINATTACKER, MOTION_TYPE_WARSTAND, ACTION_LOOP, 0 );
		break;
	default:
		{			
			if (g_pEffectLayer->GetEffectInfo(bSkillKind)->bType == TYPE_DRIVE)
				SetActionNext( MOTION_TYPE_OVERDRIVE, MOTION_TYPE_WARSTAND, ACTION_LOOP, 0 );
			else
				SetActionNext( MOTION_TYPE_SKILL, MOTION_TYPE_WARSTAND, ACTION_LOOP, 0 );			
		}		
	}

	if (this == g_pMainPlayer)
		g_pMainPlayer->m_dwStartSkillTick[bSkillKind] = g_dwCurTick;
}

BOOL CMainUser::CheckItem(CItem* pItem)
{
#ifndef EQUIP_CHK	
	return TRUE;
#else
	// 체크 클래스 //
	CBaseItem* lpItemTable = g_pItemTableHash->GetData(pItem->GetID());
	
	if(!lpItemTable)
		return FALSE;
	
	BOOL bClass = FALSE;
	
	BYTE byClassId = (BYTE)lpItemTable->GetClass();
	
	if(byClassId & (BYTE)pow(2, m_wClass-1))
		bClass = TRUE;
	
	return bClass;	
#endif
	return TRUE;
}

//------------------------------------------------------------------------------
// CUser Code....
CUser::CUser()
{
	memset(this, 0, sizeof(CUser));
}

CUser::~CUser()
{	

}

void CUser::SetAction( WORD wAct, int nFrame /* = 0 */, BYTE bFlag /* = ACTION_LOOP  */)
{			
	GXOBJECT_HANDLE handle = m_hPlayer.pHandle ;
	LPObjectDesc	pDesc = (LPObjectDesc)g_pExecutive->GetData( handle );

	if(!pDesc)	
	{
		// MSG_ID : 278 ; Fail to ChangeAction!
		MessageBox(g_hMainWnd,g_Message[ETC_MESSAGE278].szMessage,NULL,NULL);	
		return;	
	}		
		
	DWORD			dwMotionIndex	= g_pExecutive->GXOGetCurrentMotionIndex( handle );
	DWORD			dwCurFrame = g_pExecutive->GXOGetCurrentFrame(handle);
	MOTION_DESC		MotionDesc;
	
	g_pExecutive->GXOGetMotionDesc( handle, &MotionDesc, dwMotionIndex, 0 );
	
	for(int i = (int)dwCurFrame; i < (int)MotionDesc.dwLastFrame; ++i)
	{		
		if( !pDesc->ObjectFunc ) break;

		pDesc->nCurFrame = i;		
		
		if (pDesc->nCurFrame == (int)MotionDesc.dwLastFrame)
		{
			if( pDesc->bActionFlag == ACTION_NEXT )
				(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_CHANGE_NEXT );
			else 
				(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_FINISHED );
		
		}
		else
		{
			(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_PLAYING );
		}
	}

	::SetAction( m_hPlayer.pHandle, wAct, nFrame, bFlag );
}

void CUser::SetActionNext( WORD wAct, WORD wNextAct, BYTE bNextFlag /* = ACTION_LOOP */, int nNextFrame /* = 0  */)
{
	GXOBJECT_HANDLE handle = m_hPlayer.pHandle ;
	LPObjectDesc	pDesc = (LPObjectDesc)g_pExecutive->GetData( handle );
	
	DWORD			dwCurFrame = g_pExecutive->GXOGetCurrentFrame(handle);
	DWORD			dwMotionIndex	= g_pExecutive->GXOGetCurrentMotionIndex( handle );
	MOTION_DESC		MotionDesc;
	
	g_pExecutive->GXOGetMotionDesc( handle, &MotionDesc, dwMotionIndex, 0 );
	dwCurFrame = pDesc->nCurFrame; 

	for(int i = (int)dwCurFrame; i < (int)MotionDesc.dwLastFrame; ++i)
	{	
		if( !pDesc->ObjectFunc ) break;

		pDesc->nCurFrame = i;
		
		if (pDesc->nCurFrame == (int)MotionDesc.dwLastFrame)
		{
			if( pDesc->bActionFlag == ACTION_NEXT )
				(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_CHANGE_NEXT );
			else 
				(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_FINISHED );
		}
		else 
		{
			(*pDesc->ObjectFunc)( handle, pDesc, pDesc->nCurFrame, FRAME_FLAG_PLAYING );
		}
	}

	::SetActionNext( m_hPlayer.pHandle, wAct, wNextAct, bNextFlag, nNextFrame );
}


void CUser::ShowMoveStartEffect()
{
	if(m_bStatus != UNIT_STATUS_RUNNING ) return;

	::ShowObject( m_hMoveStartEffect.pHandle );
	::SetAction( m_hMoveStartEffect.pHandle, 1, 0, ACTION_ONCE );
	m_hMoveStartEffect.pDesc->ObjectFunc = EffectOnceAndHideFunc;
	GXSetPosition( m_hMoveStartEffect.pHandle, &m_v3CurPos, FALSE );
}

void CUser::ShowMoveStopEffect()
{
	if(m_bStatus != UNIT_STATUS_RUNNING ) return;

	::ShowObject( m_hMoveStopEffect.pHandle );
	::SetAction( m_hMoveStopEffect.pHandle, 1, 0, ACTION_ONCE );
	m_hMoveStopEffect.pDesc->ObjectFunc = EffectOnceAndHideFunc;
	GXSetPosition( m_hMoveStopEffect.pHandle, &m_v3CurPos, FALSE );	
}

void CUser::RemoveAllStatusEffect()
{
	if (m_pEffectList)
		m_pEffectList->RemoveAll();

	if (m_pUsingStatusEffectList)
	{
		POSITION_ pos = m_pUsingStatusEffectList->GetHeadPosition();
	
		while( pos )
		{
			POSITION_ delPos = pos;
			EffectDesc* pEffectDesc = (EffectDesc*)m_pUsingStatusEffectList->GetNext(pos);
			
			RemoveEffectDesc(pEffectDesc);
			m_pUsingStatusEffectList->RemoveAt(delPos);
		}
		memset(m_pEffectDesc, 0, sizeof(m_pEffectDesc));
		m_bOverDriveCount = 0;		
	}	
}

void CUser::SetDamageIndex( DWORD dwDamage )
{
	LPDAMAGE_NUMBER	pDamage;
	pDamage = AllocDamageNumber( &g_pDamageNumberPool[0] );
	if( !pDamage ) return;
	
	ClearDamageNumber( pDamage );

	int i, iStart = 0;//, iCnt = 0;
	VECTOR3	v3Pos;

	int	 iDamage[6] = {0,};
	char szDamage[6] = {0,};
	char szTmp[2] = {0,};
//	BOOL bFirstZero = FALSE;
//	if (dwDamage > 9999)
//		dwDamage = 9999;

	wsprintf( szDamage, "%05d", dwDamage );
	iStart = 4;

	for( i=0; i<5; i++ )
	{
		if( szDamage[i] != '0' ) 
		{
			iStart = i;
			break;
		}
	}
	
	MATRIX4 mtY;
	VECTOR3	v3CameraRad;
	int		iStartSpot = (5 - iStart)*23/2;

	g_pGeometry->GetCameraAngleRad( &v3CameraRad, 0 );
	SetRotationYMatrix( &mtY, (v3CameraRad.y)-3.141592f );
	
	szTmp[1] = '\0';
	for( i=iStart; i<5; i++ ) 
	{
		szTmp[0] = szDamage[i];
		iDamage[i] = atoi( szTmp );
		
		v3Pos.z	 = 0;
		v3Pos.y  = 0;
		v3Pos.x  = (float)( (5-i)*23 - iStartSpot );
		TransformVector3_VPTR1( &v3Pos, &mtY, 1 );
		v3Pos.x += m_v3CurPos.x;
		v3Pos.y += m_v3CurPos.y;
		v3Pos.z += m_v3CurPos.z;

		ShowObject( pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pHandle );
		::SetAction( pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pHandle, 1, 0, ACTION_ONCE );
		GXSetPosition( pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pHandle, &v3Pos, FALSE );
		pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pDesc->ObjectFunc = DamageNumberFunc;
		
		pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pDesc->bType = OBJECT_TYPE_DAMAGE_USER;
		pDamage->m_hDamageNumber[ pDamage->m_pbUsedDamageNumber[iDamage[i]] ][iDamage[i]].pDesc->pInfo = pDamage;
		
		pDamage->m_pbUsedDamageNumber[ iDamage[i] ]++;
	}
}

void CUser::SetDamageChar( DWORD dwChar )
{
	LPDAMAGE_NUMBER	pDamage;
	pDamage = AllocDamageNumber( &g_pDamageNumberPool[0] );
	if( !pDamage ) return;
	
	ClearDamageNumber( pDamage );
	
	MATRIX4 mtY;
	VECTOR3	v3CameraRad, v3Pos;

	g_pGeometry->GetCameraAngleRad( &v3CameraRad, 0 );
	SetRotationYMatrix( &mtY, (v3CameraRad.y)-3.141592f );
	
	v3Pos.z	 = 0;
	v3Pos.y  = 0;
	v3Pos.x  = 0;//( (5-i)*23 - iStartSpot );
	TransformVector3_VPTR1( &v3Pos, &mtY, 1 );
	v3Pos.x += m_v3CurPos.x;
	v3Pos.y += m_v3CurPos.y;
	v3Pos.z += m_v3CurPos.z;

	ShowObject( pDamage->m_hDamageChar[dwChar].pHandle );
	::SetAction( pDamage->m_hDamageChar[dwChar].pHandle, 1, 0, ACTION_ONCE );
	GXSetPosition( pDamage->m_hDamageChar[dwChar].pHandle, &v3Pos, FALSE );
	pDamage->m_hDamageChar[dwChar].pDesc->ObjectFunc = DamageNumberFunc;
	pDamage->m_hDamageChar[dwChar].pDesc->bType = OBJECT_TYPE_DAMAGE_USER;	
	pDamage->m_hDamageChar[dwChar].pDesc->pInfo = pDamage;
}

void CUser::SetSkillFailAction(ENUM_SKILL_CASTING_FAIL_REASON eFailReason)
{
	if (GetStatus() == UNIT_STATUS_DEAD)
		return;

	if (g_pMainPlayer == this)
	{
		switch(eFailReason)
		{
		case SKILL_CASTING_FAIL_REASON_GET_SLAPPED:
			{
				DisplayMessageAdd(g_Message[ETC_MESSAGE1076].szMessage, 0xFFFFC309);	
			}break;
		case SKILL_CASTING_FAIL_REASON_LOW:
			{
				// MSG_ID : 454 ; 대상이 없거나 마나가 부족합니다.
				DisplayMessageAdd(g_Message[ETC_MESSAGE454].szMessage, 0xFFFFC309);	
			}			
			break;
		case SKILL_CASTING_FAIL_REASON_LACK_ZODIAC_ITEM:
			{
				//"황소카드가 부족합니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE1043].szMessage, 0xFFFFC309); 
			}			
			break;
		case SKILL_CASTING_FAIL_REASON_LACK_SP:
			{
				// "SP가 부족합니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE148].szMessage, 0xFFFFC309); 
			}
			break;
		case SKILL_CASTING_FAIL_REASON_OVER_RANGE:
			{
				DisplayMessageAdd(g_Message[ETC_MESSAGE147].szMessage, 0xFFFFC309);
			}
			break;
		case SKILL_CASTING_FAIL_NONE_REASEON:
			break;
		}
	}

	SetStatus(UNIT_STATUS_NORMAL);
	m_hPlayer.pDesc->ObjectFunc = NULL;

	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_Camera.v3CameraPos, FALSE);
	SetActionNext( MOTION_TYPE_SKILL, MOTION_TYPE_WARSTAND, ACTION_LOOP, 0 );

	DeleteMagicArray();
}
void CUser::DeleteMagicArray()
{
	if (m_pEffectMagicArray)
	{
		m_pEffectMagicArray->dwFrameCount = 0;
		MOTION_DESC		MotionDesc;
		/*BOOL bMotion	= */g_pExecutive->GXOGetMotionDesc(m_pEffectMagicArray->hEffect.pHandle, &MotionDesc, 1,0);
		m_pEffectMagicArray->dwFrameFinish = MotionDesc.dwLastFrame/4;
		m_pEffectMagicArray->hEffect.pDesc->ObjectFunc = EffectAlphaFunc;

		//RemoveEffectDesc((EffectDesc*)m_hEffectMagicArray.pDesc->pInfo);
		m_pEffectMagicArray = NULL;
	}
}

void CUser::SetSiegePKCount(int nCount)
{
	m_wSiegePKCount = WORD(nCount);
	if (m_wSiegePKCount)
	{
		if (!m_pPKEffect)	
		{
			m_pPKEffect = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_PK_COUNT), g_pMainPlayer == this, __CHR_EFFECT_NONE__);
			
			m_pPKEffect->byTargetObjectType[0] = OBJECT_TYPE_PLAYER;
			m_pPKEffect->dwTargetIndex[0] = m_dwUserIndex;
			
		}
		::SetAction(m_pPKEffect->hEffect.pHandle, (WORD)min(m_wSiegePKCount, 20), 0, ACTION_LOOP);
		m_pPKEffect->hEffect.pDesc->ObjectFunc = EffectSkillUserStatusTopFunc;
	}
	else
	{
		if (m_pPKEffect)	
		{
			RemoveEffectDesc(m_pPKEffect);
			m_pPKEffect = NULL;
		}
	}
}

void CUser::DungeonOwnerEffect()
{
	if (g_pThisDungeon->IsDungeonOwner(this))
	{
		if (!m_pOwnerEffect)
		{
			m_pOwnerEffect = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_DUNGEON_OWNER), 1, __CHR_EFFECT_NONE__);
			m_pOwnerEffect->byTargetObjectType[0] = OBJECT_TYPE_PLAYER;
			m_pOwnerEffect->dwTargetIndex[0] = m_dwUserIndex;
			
			::SetAction(m_pOwnerEffect->hEffect.pHandle, 1, 0, ACTION_LOOP);
			m_pOwnerEffect->hEffect.pDesc->ObjectFunc = EffectSkillUserStatusTopFunc;
		}
	}
	else
	{
		if (m_pOwnerEffect)
		{
			RemoveEffectDesc(m_pOwnerEffect);
			
		}
	}
}

void CUser::SetAttackMode(BYTE bAttackMode)
{
	if (m_pEffectAttackMode)
	{
		RemoveEffectDesc(m_pEffectAttackMode);
		m_pEffectAttackMode = NULL;
	}
	
	if (bAttackMode == ATTACK_MODE_DEFENSE)
	{
		// 방어자일때
		if (!m_pEffectAttackMode)
			m_pEffectAttackMode = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_DEFENCE_TEAM), g_pMainPlayer == this, __CHR_EFFECT_NONE__);
	}
	else if (bAttackMode == ATTACK_MODE_OFFENSE)
	{
		// 공격자일때의 이펙트
		if (!m_pEffectAttackMode)
			m_pEffectAttackMode = g_pEffectLayer->CreateGXObject(g_pObjManager->GetFile(EFFECT_OFFENSE_TEAM), g_pMainPlayer == this, __CHR_EFFECT_NONE__);
	}	
		
	if (m_pEffectAttackMode)
	{
		m_pEffectAttackMode->byTargetObjectType[0] = OBJECT_TYPE_PLAYER;
		m_pEffectAttackMode->dwTargetIndex[0] = m_dwUserIndex;
		m_pEffectAttackMode->hEffect.pDesc->ObjectFunc = EffectSkillUserStatusTopFunc;
	}

	m_bAttackMode = bAttackMode;	
}

void CUser::SetPlayerShop(BOOL bPlayerShop)
{
	m_bPlayerShop = (BYTE)bPlayerShop;
	
	if(m_dwUserIndex == g_pMainPlayer->m_dwUserIndex && m_bPlayerShop)
	{
		g_byStatusMessenger	= 2;
						
		// 패킷 보내기 //			
		CTWS_MESSENGER_STATUS pPacket;
		pPacket.byType		= 2;
		pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
		g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);

		CPlayerShopWnd* pPalayerShopWnd = CPlayerShopWnd::GetInstance();
		pPalayerShopWnd->m_byMode = 1;

		// sung-han 2005-02-15 노점을 개설할 경우 길드전매칭 취소를 보낸다--------------------------------
		CGroupWnd*	pGroupWnd = CGroupWnd::GetInstance();
		if(pGroupWnd->m_bGuildWarMatchEnable)
		{
			pGroupWnd->m_bGuildWarMatchEnable = FALSE;
			pGroupWnd->m_bGuildWarPacketSendEnable = FALSE;
			TEAM_MATCH_PACKET TeamMatch;
			TeamMatch.Body.Condtion.iMatchType = M_T_NONE;
			TeamMatch.Body.Condtion.iMatchGuildNumber = 0;
			TeamMatch.Body.Condtion.iMatchGuildType = T_T_NONE;
			Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_REMOVE);
		}
		//--------------------------------------------------------------------------------------------------
	}

	if(m_bPlayerShop)
	{
		m_hPlayer.pDesc->ObjectFunc	= NULL;

		SendStopPacket();
		SetStatus(UNIT_STATUS_PLAYER_SHOP);		
		SetActionNext(39, 40, ACTION_LOOP);		
		
		::ShowObject(m_hPlayerShop.pHandle);
		::SetAction(m_hPlayerShop.pHandle, 1, 0, ACTION_ONCE);
		GXSetPosition(m_hPlayerShop.pHandle, &m_v3CurPos, FALSE);
		g_pExecutive->GXOSetDirection(m_hPlayerShop.pHandle
			, &g_Camera.v3AxsiY
			, (float)(atan2(m_v3Direction.z, m_v3Direction.x) + DEG90 ) );
	}
	else
	{
		::SetAction(m_hPlayerShop.pHandle, 2, 0, ACTION_ONCE);
		m_hPlayerShop.pDesc->ObjectFunc	= ShopProc;

		SetAction(41, 0, ACTION_ONCE);
		m_hPlayer.pDesc->ObjectFunc	= PlayerShopProc;

		if(g_pMainPlayer == this)		
			memset(g_pMainPlayer->m_sPlayerShop.szTitle, 0, sizeof(g_pMainPlayer->m_sPlayerShop.szTitle));
	}
}


void CUser::UserObjectAlpha(BYTE bAlpha)
{	
	g_pExecutive->SetAlphaFlag(m_hPlayer.pHandle, bAlpha );
	g_pExecutive->SetAlphaFlag(m_hPlayerHead.pHandle, bAlpha );
	if (m_hPlayerLHand.pHandle)	g_pExecutive->SetAlphaFlag(m_hPlayerLHand.pHandle , bAlpha );
	if (m_hPlayerRHand.pHandle)	g_pExecutive->SetAlphaFlag(m_hPlayerRHand.pHandle , bAlpha );	
}


void CUser::UserObjectEnable(BYTE bEnable)
{
	if (bEnable)
	{
		UserObjectAlpha(255);

		g_pExecutive->EnableRender(m_hPlayer.pHandle);
		g_pExecutive->EnableRender(m_hPlayerHead.pHandle);
		if (m_hPlayerLHand.pHandle)	g_pExecutive->EnableRender(m_hPlayerLHand.pHandle);
		if (m_hPlayerRHand.pHandle)	g_pExecutive->EnableRender(m_hPlayerRHand.pHandle);
	}
	else
	{
		g_pExecutive->DisableRender(m_hPlayer.pHandle);
		g_pExecutive->DisableRender(m_hPlayerHead.pHandle);
		if (m_hPlayerLHand.pHandle)	g_pExecutive->DisableRender(m_hPlayerLHand.pHandle);
		if (m_hPlayerRHand.pHandle)	g_pExecutive->DisableRender(m_hPlayerRHand.pHandle);
	}

	ToggleUserShadow(this, bEnable);	
}

void CUser::SetMatchUser(DWORD	dwMatchUserIndex)
{
	m_dwMatchUserIndex = dwMatchUserIndex;
}

BOOL CUser::IsSameUser( CUser* pUser )		// added by minjin.
{	
	return ( m_dwUserIndex == pUser->m_dwUserIndex ) ? TRUE : FALSE;
}

BYTE CMainUser::GetSkillLevel(BYTE bSkillKind)
{
	unsigned char bSkillLevel = (char)sSkillTable[bSkillKind].bSKillLevel;
	
	return BYTE(max(bSkillLevel, 0));
}

void CMainUser::SetPacketSkillUser(CUser *pTargetUser, BYTE bSkillKindLR)
{
	m_pSkillPacket->bStatus = UPDATE_GAME_PLAY;
	m_pSkillPacket->bHeader = Protocol_CTS::CMD_SKILL;
	m_pSkillPacket->bSkillKind		= bSkillKindLR;
	m_pSkillPacket->dwTargetIndex		= pTargetUser->m_dwUserIndex;
	m_pSkillPacket->wTileIndex_X		= WORD(pTargetUser->m_v3CurPos.x / TILE_SIZE);
	m_pSkillPacket->wTileIndex_Z		= WORD(pTargetUser->m_v3CurPos.z / TILE_SIZE);
	m_pSkillPacket->bOwnType = OBJECT_TYPE_PLAYER;
	m_pSkillPacket->bTargetType = OBJECT_TYPE_PLAYER;
	m_pSkillPacket->bPK = CUserInterface::GetInstance()->m_nPK == __PK_MODE__;
	m_pSkillPacket->dwTime = g_dwCurTick;

	m_bSkillTmp = GetSkillKind(bSkillKindLR);

	SendCasting();			
}

void CMainUser::SetPacketSkillMonster(CMonster* pTargetMonster, BYTE bSkillKindLR)
{
	m_pSkillPacket->bStatus = UPDATE_GAME_PLAY;
	m_pSkillPacket->bHeader = Protocol_CTS::CMD_SKILL;
	m_pSkillPacket->bSkillKind		= bSkillKindLR;
	m_pSkillPacket->dwTargetIndex		= pTargetMonster->m_dwMonsterIndex;
	m_pSkillPacket->wTileIndex_X		= WORD(pTargetMonster->m_v3CurPos.x / TILE_SIZE);
	m_pSkillPacket->wTileIndex_Z		= WORD(pTargetMonster->m_v3CurPos.z / TILE_SIZE);
	m_pSkillPacket->bOwnType = OBJECT_TYPE_PLAYER;
	m_pSkillPacket->bTargetType = OBJECT_TYPE_MONSTER;
	m_pSkillPacket->bPK = CUserInterface::GetInstance()->m_nPK == __PK_MODE__;
	m_pSkillPacket->dwTime = g_dwCurTick;

	m_bSkillTmp = GetSkillKind(bSkillKindLR);

	SendCasting();	
}

void CMainUser::SetPacketSkillTile(WORD wTileIndexX, WORD wTileIndexZ,  BYTE bSkillKindLR)
{
	m_pSkillPacket->bStatus = UPDATE_GAME_PLAY;
	m_pSkillPacket->bHeader = Protocol_CTS::CMD_SKILL;
	m_pSkillPacket->bSkillKind		= bSkillKindLR;
	m_pSkillPacket->dwTargetIndex		= 0;
	m_pSkillPacket->wTileIndex_X		= wTileIndexX;
	m_pSkillPacket->wTileIndex_Z		= wTileIndexZ;
	m_pSkillPacket->bOwnType = OBJECT_TYPE_PLAYER;
	m_pSkillPacket->bTargetType = OBJECT_TYPE_TILE;
	m_pSkillPacket->bPK = CUserInterface::GetInstance()->m_nPK == __PK_MODE__;
	m_pSkillPacket->dwTime = g_dwCurTick;

	m_bSkillTmp = GetSkillKind(bSkillKindLR);

	SendCasting();	
}

void CMainUser::SetSkillChangeLR(BYTE bySkillKind, BYTE byLR)
{
	// 손에든  스킬을 바꾼다. 	
	if(byLR==0)
	{
		if(m_bySkill[1]==bySkillKind)
			m_bySkill[1] = __SKILL_NONE_SELECT__;
	}
	else if(byLR==1)
	{
		if(m_bySkill[0]==bySkillKind)
			m_bySkill[0] = __SKILL_NONE_SELECT__;
	}

	m_bySkill[byLR]	= bySkillKind;

	CTDS_DUNGEON_CHANGE_LRSKILLKIND packet;
	packet.bySelectSkillKind[1] = GetSkillKind(1);
	packet.bySelectSkillKind[0] = GetSkillKind(0);
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
}

BYTE CMainUser::GetSkillKind(BYTE byLR)
{
	return m_bySkill[byLR];
}

float CMainUser::GetODC()
{
	return (float)max((m_dwEgo*2 / m_dwLevel) * (100 + 5 * m_wClassRank) / 100., 0.5);
}

void CMainUser::SendCasting()
{	
	CTDS_DUNGEON_CASTING packet;
	packet.bSkillKindLR		= m_pSkillPacket->bSkillKind;
	packet.dwTargetIndex	= m_pSkillPacket->dwTargetIndex;
	packet.wTileIndex_X		= m_pSkillPacket->wTileIndex_X;
	packet.wTileIndex_Z		= m_pSkillPacket->wTileIndex_Z;
	packet.bTargetType		= m_pSkillPacket->bTargetType;
	packet.bOwnType			= m_pSkillPacket->bOwnType;
		
	g_pNet->SendMsg( (char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE );

	m_dwCastingTick = 0;	
	SetStatus(UNIT_STATUS_CASTING);	// 캐스팅은 끝났으나 아직 스킬 메시지가 날라오질 않았따.	
}

void CMainUser::SendSkill()
{
	if (IsSkilling())
	{
		g_pNet->SendMsg( (char*)m_pSkillPacket, m_pSkillPacket->GetPacketSize(), SERVER_INDEX_ZONE );
		m_pSkillPacket->bStatus = 0;
	}
}

BOOL CMainUser::IsSkilling()
{
	Effect* pEffect = g_pEffectLayer->GetEffectInfo(m_bSkillTmp);
	return m_pSkillPacket->bStatus && (pEffect->bType == TYPE_DRIVE || pEffect->bID == __SKILL_AURARECHARGE__);
}

void CMainUser::SetActionDummy()
{
	// 멍청하게 있을때 동작 바꿔주기.
	int nMotionIndex = g_pExecutive->GXOGetCurrentMotionIndex(m_hPlayer.pHandle);
	
	switch (nMotionIndex%50)
	{
	case MOTION_TYPE_WARSTAND:
	case MOTION_TYPE_DUNGEONSTAND:
	case MOTION_TYPE_STAND1:
	case MOTION_TYPE_STAND2:
		m_dwTemp[USER_TEMP_IDLETIME]++;
		break;
	}	
	
	if (m_dwTemp[USER_TEMP_IDLETIME] == 2000)
	{
		BYTE bStandMode = nMotionIndex%50 == MOTION_TYPE_WARSTAND;
		
		if (bStandMode)
			SetActionNext(MOTION_TYPE_WARSTANDSTAND, MOTION_TYPE_DUNGEONSTAND);
		else
		{
			switch (rand()%2)
			{
			case 0:
				SetActionNext(MOTION_TYPE_STAND1, MOTION_TYPE_DUNGEONSTAND);
				break;
			case 1:
				SetActionNext(MOTION_TYPE_STAND2, MOTION_TYPE_DUNGEONSTAND);
				break;
			}
		}		
	}	
}


// 현재 유저가 대전중인가 알아내는 함수 
// (대전 처음부터 끝까지 통틀어 대전 중으로 본다)
BOOL CMainUser::IsMatching() const
{
	// 유저가 마을 대전장소에서 대전중이다. 
	return ((g_pThisDungeon && g_pThisDungeon->GetDungeonType() == DUNGEON_TYPE_VILLAGE) &&
			(m_bMatching || (m_bCurLayer >= MATCH_DUNGEON_LAYER_TOP && m_bCurLayer < MATCH_DUNGEON_LAYER_BOTTOM)));
}


void CMainUser::GetAttackDamage_L(WORD& wAttackDamageMin, WORD& wAttackDamageMax)
{
	if (m_wClass == CLASS_TYPE_HUNTER)
	{
		// 레인저는 mp 양에 따라 데미지가 변한다.
		if (m_pwAttackDamage_L[0])
		{
			if (GetSkillKind(0) == __SKILL_ATTACK__)
			{
				wAttackDamageMin = WORD(m_pwAttackDamage_L[0] + m_wMP/30);
				wAttackDamageMax = WORD(m_pwAttackDamage_L[1] + m_wMP/30);
			}
			else
			{
				wAttackDamageMin = m_pwAttackDamage_L[0];
				wAttackDamageMax = m_pwAttackDamage_L[1];
			}
		}
		else
		{
			wAttackDamageMin = 0;
			wAttackDamageMax = 0;
		}
	}
	else
	{
		wAttackDamageMin = m_pwAttackDamage_L[0];
		wAttackDamageMax = m_pwAttackDamage_L[1];
	}
}


void CMainUser::GetAttackDamage_R(WORD& wAttackDamageMin, WORD& wAttackDamageMax)
{
	if (m_wClass == CLASS_TYPE_HUNTER)
	{
		// 레인저는 mp 양에 따라 데미지가 변한다.
		if (m_pwAttackDamage_R[0])
		{
			if (GetSkillKind(1) == __SKILL_ATTACK__)
			{
				wAttackDamageMin = WORD(m_pwAttackDamage_R[0] + m_wMP/30);
				wAttackDamageMax = WORD(m_pwAttackDamage_R[1] + m_wMP/30);
			}
			else
			{
				wAttackDamageMin = m_pwAttackDamage_R[0];
				wAttackDamageMax = m_pwAttackDamage_R[1];
			}
		}
		else
		{
			wAttackDamageMin = 0;
			wAttackDamageMax = 0;
		}		
	}
	else
	{
		wAttackDamageMin = m_pwAttackDamage_R[0];
		wAttackDamageMax = m_pwAttackDamage_R[1];
	}
}

short CMainUser::GetPhyResist()
{
	if (m_wClass == CLASS_TYPE_HUNTER)
	{
		// 레인저는 mp 양에 따라 데미지가 변한다.
		return short(min(m_wPhyResist + max(int(m_wMP/20 - (73+22*m_dwLevel)/35), 0), 75));
	}
	else
	{
		return m_wPhyResist;
	}
}


void CMainUser::WeightProcess(BOOL bChk, WORD wWeight)
{
	WORD	wSumWeight		= GetSumWeight();
	WORD	wAllSumWeight	= GetAllSumWeight();
	float	fPerWeight		= ((float)wSumWeight/(float)wAllSumWeight)*100;
		
	if((WORD)fPerWeight >= WEIGTH_100PER_OVER)
	{
		m_bMoveType = UNIT_STATUS_WALKING;

		if((!bChk) || (bChk && wSumWeight>wWeight))
		{
			// "포화무게한도에 도달하여 달리기가 안되며, 포션의 사용 딜레이가 증가합니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE986].szMessage, 0xffff0000);					
		}		
	}
	else if((WORD)fPerWeight >= WEIGTH_80PER_OVER)	
	{
		if((!bChk) || (bChk && wSumWeight>wWeight))
		{
			// "무게한도를 초과하여 이동속도가 감소하고 hp의 회복력이 정지됩니다."		
			DisplayMessageAdd(g_Message[ETC_MESSAGE990].szMessage, 0xffff0000);
		}
	}
}
