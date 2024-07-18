#include "GameSystemNative.h"
#include "GameSystem.h"
#include "GameSystem_for_yak.h"
#include "Monster.h"
#include "MonsterManager.h"
#include "DUNGEON_DATA_EX.h"
#include "Dungeon.h"
#include "OwnServer.h"
#include "Map.h"
#include "DungeonLayer.h"
#include "Section.h"
#include "MonsterTable.h"
#include "ItemMove.h"
#include "ItemNative.h"
#include "MsgProc_for_jjw6263.h"
#include "User.h"


// 몬스터 죽었을때 아이템 생성 
BOOL MakeCItemClass( CItem* pItem, WORD wBaseItemID, const BASEMONSTER* const pBaseMonster, WORD wOptionNum, CUser* pUser )
{
	int			iLevel = 0, iRand = 0, iSlot = 0;
	int			i = 0, j = 0, k = 0, iItemKind = 0, iOption = 0, iValue = 0;
	WORD		wProbability = 0;
	CBaseItem*	pBaseItem = g_pBaseItemHash->GetData( wBaseItemID );	

	if( !pItem ) return FALSE;

	switch(GetItemKind(wBaseItemID))
	{
	case ITEM_KIND_WEAPON:
		{
			pItem->m_Item_Weapon.wMin_Damage	=	pBaseItem->BaseItem_Weapon.wMin_Damage;
			pItem->m_Item_Weapon.wMax_Damage	=	pBaseItem->BaseItem_Weapon.wMax_Damage;
			pItem->m_Item_Weapon.wMana_Damage	=	pBaseItem->BaseItem_Weapon.wMana_Damage;
			
			iSlot = pBaseItem->BaseItem_Weapon.wMax_Slot - pBaseItem->BaseItem_Weapon.wMin_Slot;
			pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Weapon.wMin_Slot;
			pItem->m_bSlot.uMaxSlot += rand()%( iSlot + 1 );

			if( pBaseItem->BaseItem_Weapon.wSetID != 0 )	
			{
				k = 0;
				for( i = 0; i < 6; i++ )
				{
					if( pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wKind == 0 ) continue;

					iValue =	pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMax -
								pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
					
					if( iValue > 0 )
					{
						pItem->wAttr[k].uValue = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
						iRand = rand()%(iValue+1);
						pItem->wAttr[i].uValue += iRand;
					}
					else
					{
						pItem->wAttr[k].uValue = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
					}

					if( pItem->wAttr[i].uValue != 0 )
					{
						pItem->wAttr[k].uCode = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wKind;
						k++;
					}
				}

				return TRUE;
			}
		}
		break;
	case ITEM_KIND_AROMR:
		{
			pItem->m_Item_Armor.wDefense	=	pBaseItem->BaseItem_Armor.wMin_Damage;
		
			iSlot = pBaseItem->BaseItem_Armor.wMax_Slot - pBaseItem->BaseItem_Armor.wMin_Slot;
			pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Armor.wMin_Slot;
			pItem->m_bSlot.uMaxSlot += rand()%( iSlot + 1 );

			if( pBaseItem->BaseItem_Armor.wSetID != 0 )	
			{
				k = 0;
				for( i = 0; i < 6; i++ )
				{
					if( pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wKind == 0 ) continue;

					iValue =	pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMax -
								pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;
					if( iValue > 0 )
					{
						pItem->wAttr[k].uValue = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;
						iRand = rand()%(iValue+1);
						pItem->wAttr[i].uValue += iRand;
					}
					else
					{
						pItem->wAttr[k].uValue = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;
					}

					if( pItem->wAttr[k].uValue != 0 )
					{
						pItem->wAttr[k].uCode = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wKind;
						k++;
					}
				}
				
				return TRUE;
			}
		}
		break;
	case ITEM_KIND_BAG:
		{
			pItem->m_Item_Bag.bType	= pBaseItem->BaseItem_Bag.bType;
			wProbability = pBaseItem->BaseItem_Bag.WValue_Max - pBaseItem->BaseItem_Bag.wValue_Min;
			pItem->m_Item_Bag.wProbability = pBaseItem->BaseItem_Bag.wValue_Min;
			pItem->m_Item_Bag.wProbability += rand() % ( wProbability + 1 );
			return TRUE;
		}
		break;
	case ITEM_KIND_GUARDIAN:
		{
			pItem->m_Item_Guardian.bCreate			= GUARDIAN_CREATE_STATUS_NONE;
			pItem->m_Item_Guardian.dwID				= 0;
			pItem->m_Item_Guardian.dwCompleteTime	= 0;
			pItem->m_Item_Guardian.dwEndLife		= 0;
			return TRUE;	
		}
		break;
	case ITEM_KIND_RIDE:
		break;
	case ITEM_KIND_MONEY:
		{
			if( !pBaseMonster )	return FALSE;

			DWORD dwMoney = pBaseMonster->dwMax_Money - pBaseMonster->dwMin_Money;
			if( dwMoney != 0 )
			{
				dwMoney = rand()%dwMoney;
				dwMoney += pBaseMonster->dwMin_Money;
			}
			else
			{
				dwMoney = pBaseMonster->dwMin_Money;
			}

			// 몬스터.money * (100 + ITEM_ATTR_KARZ_GAIN ) / 100
			if (pUser)
				dwMoney *= (DWORD)((100+g_pThis->m_dwGLOBAL_KARZ_GAIN+pUser->GetItemAttr(ITEM_ATTR_KARZ_GAIN)) / 100.);
			
			pItem->m_Item_Money.dwMoney = dwMoney;
			return TRUE;
		}
		break;
	case ITEM_KIND_ZODIAC:
		{
			switch(wBaseItemID)
			{
			case __ITEM_PORTAL_VILL1__:
				pItem->m_Item_Zodiac.wMapId	= 1;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL2__:
				pItem->m_Item_Zodiac.wMapId	= 3;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL3__:
				pItem->m_Item_Zodiac.wMapId	= 4;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL4__:
				pItem->m_Item_Zodiac.wMapId	= 5;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL5__:
				pItem->m_Item_Zodiac.wMapId	= 6;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL6__: // 귀환 스크롤 추가 김영대 2004.02.14
				pItem->m_Item_Zodiac.wMapId	= 7;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			}
			
			pItem->SetQuantity(1);
			return TRUE;
		}
		break;
	case ITEM_KIND_MIXUPGRADE:
	case ITEM_KIND_SUPPLIES:
	case ITEM_KIND_SPECIAL:
	case ITEM_KIND_UPGRADE:
	case ITEM_KIND_LIQUID:
	case ITEM_KIND_EDITION:
	case ITEM_KIND_CONSUMABLE:
	case ITEM_KIND_MATERIALS:
		{
			pItem->SetQuantity(1);
			return TRUE;
		}		
	default:	return FALSE;
	}	
	
	// 옵션을 붙여준다.
	if( wOptionNum == 0 ) return TRUE;
	if( iItemKind >= ITEM_NUM_PER_OPTION ) return TRUE;
	
	for( i = 0; i < wOptionNum; i++ )
	{
		for( ;; )			// 현제 아이템 타입에 가능한 옵션을 뽑는다.
		{
			iOption = ( rand() % (MAX_ITEM_KIND1_CODE-1) ) + 1;		// 0은 안나오게 한다.
			if( iOption == ITEM_ATTR_ALLSTATUS ) continue;			// 일단 올스텟은 옵션에서 뺴자.

			if( g_pItemOptionTable[iItemKind].pbOption[iOption] )	// 해당 옵션을 붙일수 있는 아이템인지를 확인한다.
			{
				iLevel = pBaseMonster->wOptionLevel[i].wMax - pBaseMonster->wOptionLevel[i].wMin;

				if( iLevel == 0 )
				{
					iLevel = pBaseMonster->wOptionLevel[i].wMin;
				}
				else
				{
					iRand = rand()%(iLevel+1);
					iLevel = pBaseMonster->wOptionLevel[i].wMin + iRand;
				}
				
				if( iLevel == 0 || iLevel > 6 )
				{
					iLevel = 1;
					Log( LOG_NORMAL, "Option Item Level : 7 ^^ " );
				}
				
				if( g_pItemUpgradeTable[iLevel][iOption].pwItemMax[iItemKind] != 0 ) 
					break;
			}
		}
		
		// 아이템에 붙일 옵션을 찾았으면 이제 아이템을 검사하여 같은 옵션이 붙어 있는지를 검사한다.
		// 그리고 만약 같은 옵션이 있다면 한번더 올려준다.
		BOOL bSameOption;
		bSameOption = FALSE;
		
		for( j = 0; pItem->wAttr[j].uCode != 0 && j<MAX_ITEM_ATTR; j++ )
		{
			if( pItem->wAttr[j].uCode == iOption )
			{
				bSameOption = TRUE;
				break;
			}
		}

		if( j >= MAX_ITEM_ATTR ) continue;		// 더이상 옵션도 붙일수가 없는것이다.
		
		iValue = g_pItemUpgradeTable[iLevel][iOption].dwMax - g_pItemUpgradeTable[iLevel][iOption].dwMin;
		if( iValue == 0 )
		{
			iValue = g_pItemUpgradeTable[iLevel][iOption].dwMin;
		}
		else
		{
			iRand	= rand()%(iValue+1);
			iValue = g_pItemUpgradeTable[iLevel][iOption].dwMin + iRand;
		}
		
		// 마지막으로 최대값 교정.
		if( bSameOption )
			iValue = pItem->wAttr[j].uValue;

		if( iValue > g_pItemUpgradeTable[iLevel][iOption].pwItemMax[iItemKind] )
			iValue = g_pItemUpgradeTable[iLevel][iOption].pwItemMax[iItemKind];

		pItem->wAttr[j].uCode	= iOption;
		pItem->wAttr[j].uValue  = iValue;
	}

	return TRUE;
}


// 상점에서 살때 아이템 생성 
BOOL CreateItem(CItem* pItem, WORD wItemID, BYTE byCount, BYTE byInStoreFlag, BYTE byUseItemOption)
{
	if(!pItem)	return FALSE;

	CBaseItem* pBaseItem = g_pBaseItemHash->GetData(wItemID);		
	if(!pBaseItem)	return FALSE;

	memset(pItem, 0, sizeof(CItem));
	pItem->m_wItemID = wItemID;
	pItem->m_Serial	 = *g_pThis->GetNewSerial();
	
	BOOL		bSuccess		= FALSE;
	int			iRand = 0, iSlot = 0;
	int			i = 0, k = 0, iValue = 0;
	WORD		wProbability = 0;

	switch(GetItemKind(pItem->GetID()))
	{	
	case ITEM_KIND_WEAPON: 
		{
			pItem->m_Item_Weapon.wMin_Damage	= pBaseItem->BaseItem_Weapon.wMin_Damage;
			pItem->m_Item_Weapon.wMax_Damage	= pBaseItem->BaseItem_Weapon.wMax_Damage;
			pItem->m_Item_Weapon.wMana_Damage	= pBaseItem->BaseItem_Weapon.wMana_Damage;

#if !defined TAIWAN_LOCALIZING
			iSlot = pBaseItem->BaseItem_Weapon.wMax_Slot - pBaseItem->BaseItem_Weapon.wMin_Slot;
			pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Weapon.wMin_Slot;
			pItem->m_bSlot.uMaxSlot += rand()%( iSlot + 1 );
#else
			// 대만이다. 
			if (Store_Flag_In_Store == byInStoreFlag)
			{	// 상점에서 Item 생성 
				// 맥스값을 그냥 가져온다.
				pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Weapon.wMax_Slot;
			}
			else
			{
				iSlot = pBaseItem->BaseItem_Weapon.wMax_Slot - pBaseItem->BaseItem_Weapon.wMin_Slot;
				pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Weapon.wMin_Slot;
				pItem->m_bSlot.uMaxSlot += rand()%( iSlot + 1 );
			}
#endif
			if(byUseItemOption)
			{
				if( pBaseItem->BaseItem_Weapon.wSetID != 0 )	
				{
					k = 0;
					for( i = 0; i < 6; i++ )
					{
						if( pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wKind == 0 ) continue;

						iValue =	pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMax -
									pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
						
						if( iValue > 0 )
						{
							pItem->wAttr[k].uValue = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
							iRand = rand()%(iValue+1);
							pItem->wAttr[i].uValue += iRand;
						}
						else
						{
							pItem->wAttr[k].uValue = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
						}

						if( pItem->wAttr[i].uValue != 0 )
						{
							pItem->wAttr[k].uCode = pBaseItem->BaseItem_Weapon.pBaseSetItemOption[i].wKind;
							k++;
						}
					}				
				}
			}			

			bSuccess = TRUE;
		}
		break;
	case ITEM_KIND_AROMR:
		{
			pItem->m_Item_Armor.wDefense	= pBaseItem->BaseItem_Armor.wMin_Damage;
						
#if !defined TAIWAN_LOCALIZING			
			iSlot = pBaseItem->BaseItem_Armor.wMax_Slot - pBaseItem->BaseItem_Armor.wMin_Slot;
			pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Armor.wMin_Slot;
			pItem->m_bSlot.uMaxSlot += rand()%( iSlot + 1 );
#else
			if (Store_Flag_In_Store == byInStoreFlag)
			{
				// 상점에서 Item 생성 
				pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Armor.wMax_Slot;
			}
			else
			{
				iSlot = pBaseItem->BaseItem_Armor.wMax_Slot - pBaseItem->BaseItem_Armor.wMin_Slot;
				pItem->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Armor.wMin_Slot;
				pItem->m_bSlot.uMaxSlot += rand()%( iSlot + 1 );
			}
#endif
			if(byUseItemOption)
			{
				if( pBaseItem->BaseItem_Armor.wSetID != 0 )	
				{
					k = 0;
					for( i = 0; i < 6; i++ )
					{
						if( pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wKind == 0 ) continue;

						iValue =	pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMax -
									pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;
						if( iValue > 0 )
						{
							pItem->wAttr[k].uValue = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;
							iRand = rand()%(iValue+1);
							pItem->wAttr[i].uValue += iRand;
						}
						else
						{
							pItem->wAttr[k].uValue = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wMin;
						}

						if( pItem->wAttr[k].uValue != 0 )
						{
							pItem->wAttr[k].uCode = pBaseItem->BaseItem_Armor.pBaseSetItemOption[i].wKind;
							k++;
						}
					}					
				}
			}

			bSuccess = TRUE;
		}
		break;
	case ITEM_KIND_BAG:
		{
			pItem->m_Item_Bag.bType	= pBaseItem->BaseItem_Bag.bType;
			wProbability = pBaseItem->BaseItem_Bag.WValue_Max - pBaseItem->BaseItem_Bag.wValue_Min;
			pItem->m_Item_Bag.wProbability = pBaseItem->BaseItem_Bag.wValue_Min;
			pItem->m_Item_Bag.wProbability += rand() % ( wProbability + 1 );	
			bSuccess = TRUE;
		}
		break;
	case ITEM_KIND_GUARDIAN:	
		{
			pItem->m_Item_Guardian.bCreate			= GUARDIAN_CREATE_STATUS_NONE;
			pItem->m_Item_Guardian.dwID				= 0;
			pItem->m_Item_Guardian.dwCompleteTime	= 0;
			pItem->m_Item_Guardian.dwEndLife		= 0;
			bSuccess = TRUE;
		}
		break;
	case ITEM_KIND_ZODIAC:
		{
			switch(wItemID)
			{
			case __ITEM_PORTAL_VILL1__:
				pItem->m_Item_Zodiac.wMapId	= 1;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL2__:
				pItem->m_Item_Zodiac.wMapId	= 3;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL3__:
				pItem->m_Item_Zodiac.wMapId	= 4;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL4__:
				pItem->m_Item_Zodiac.wMapId	= 5;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL5__:
				pItem->m_Item_Zodiac.wMapId	= 6;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			case __ITEM_PORTAL_VILL6__: // 귀환 스크롤 추가 김영대 2004.02.14
				pItem->m_Item_Zodiac.wMapId	= 7;
				pItem->m_Item_Zodiac.bType	= 1;
				break;
			}
			
			pItem->SetQuantity(byCount);
			bSuccess = TRUE;
		}
		break;		
	case ITEM_KIND_RIDE:
		break;
	case ITEM_KIND_MAGICARRAY:
	case ITEM_KIND_MIXUPGRADE:
	case ITEM_KIND_MAGICFIELDARRAY:
	case ITEM_KIND_SPECIAL:
	case ITEM_KIND_MATERIALS:
	case ITEM_KIND_SUPPLIES:
	case ITEM_KIND_UPGRADE:
	case ITEM_KIND_LIQUID:
	case ITEM_KIND_EDITION:
	case ITEM_KIND_CONSUMABLE:
		{	
			pItem->SetQuantity(byCount);
			bSuccess = TRUE;
		}
		break;
	case ITEM_KIND_LAND:
		break;
	default:	return FALSE;		
	}

	return bSuccess;	
}


// 아이템 타일에 떨굴때 
BOOL AppearItem(CUser* pUser, const CItem* pItem, BYTE byCount, DWORD dwMoney, BOOL bRandom, float fX, float fY, float fZ, DWORD dwOwnerLimitSec, CUser* pItemOwnerUser )
{
	DSTC_APPEAR_ITEM	AppearItem;	
	CDungeonLayer*		pLayer;

	pLayer = pUser->GetCurDungeonLayer();

	if(!pLayer)
		return FALSE;

	if(pItem)
	{
		if(GetItemType(pItem->GetID())!=ITEM_TYPE_MONEY)
		{
			if(!pItem)
				return FALSE;
		}	
	}

	memset(&AppearItem.Item, 0, sizeof(CItem));

	int	iRandX = 0;
	int	iRandZ = 0;

	ITEM_TYPE eType = (pItem) ? GetItemType(pItem->GetID()) : ITEM_TYPE_MONEY;

	if(eType==ITEM_TYPE_MONEY)
	{
		AppearItem.Item.m_Serial				=	*g_pThis->GetNewSerial();
		AppearItem.Item.m_wItemID				=	__ITEM_KARZ__;
		AppearItem.Item.m_bSlot.uMaxSlot		=	0;
		AppearItem.Item.m_bSlot.uUsedSlot		=	0;
		AppearItem.bType						=	1;
		AppearItem.Item.m_Item_Money.dwMoney	=	dwMoney;
		AppearItem.v2ItemPos					=	*pUser->GetCurPosition();
	}
	else if(eType==ITEM_TYPE_LARGE)
	{
		memcpy(&AppearItem.Item, pItem, sizeof(CItem));
		AppearItem.Item.m_Serial	= pItem->m_Serial;
		AppearItem.bType			= 1;
		AppearItem.v2ItemPos		= *pUser->GetCurPosition();
	}
	else if(eType==ITEM_TYPE_SMALL)
	{
		memcpy(&AppearItem.Item, pItem, sizeof(CItem));		
		AppearItem.Item.m_Serial	= pItem->m_Serial;		
		AppearItem.bType			= 1;
		AppearItem.v2ItemPos		= *pUser->GetCurPosition();
		AppearItem.Item.SetQuantity(byCount);
	}

	if(bRandom)
	{
		// 위치를 적당히 바꿔준다.
		if(((iRandX=rand()%125+1)%2)>0)
			iRandX *= -1;
		if(((iRandZ=rand()%125+1)%2)>0)
			iRandZ *= -1;

		AppearItem.v2ItemPos.x	+=  (float)iRandX;
		AppearItem.v2ItemPos.y	+=	(float)iRandZ;
	}	
	else
	{	
		AppearItem.v2ItemPos.x	 = fX;
		AppearItem.v2ItemPos.y	 = fZ;

		if(AppearItem.v2ItemPos.x>pUser->GetCurPosition()->x)
		{
			if((pUser->GetCurPosition()->x+200)<AppearItem.v2ItemPos.x)
				AppearItem.v2ItemPos.x = pUser->GetCurPosition()->x+200;			
		}
		else if(AppearItem.v2ItemPos.x<pUser->GetCurPosition()->x)
		{
			if((pUser->GetCurPosition()->x-200)>AppearItem.v2ItemPos.x)
				AppearItem.v2ItemPos.x = pUser->GetCurPosition()->x-200;
		}

		if(AppearItem.v2ItemPos.y>pUser->GetCurPosition()->y)
		{
			if((pUser->GetCurPosition()->y+200)<AppearItem.v2ItemPos.y)
				AppearItem.v2ItemPos.y = pUser->GetCurPosition()->y+200;			
		}
		else if(AppearItem.v2ItemPos.y<pUser->GetCurPosition()->y)
		{
			if((pUser->GetCurPosition()->y-200)>AppearItem.v2ItemPos.y)
				AppearItem.v2ItemPos.y = pUser->GetCurPosition()->y-200;
		}
	}
	
	if (AppearItem.v2ItemPos.x < 0)
		AppearItem.v2ItemPos.x = 0;
	if (AppearItem.v2ItemPos.y < 0)
		AppearItem.v2ItemPos.y = 0;

	if (AppearItem.v2ItemPos.x > pLayer->GetMap()->m_dwTileNumWidth*TILE_SIZE)
		AppearItem.v2ItemPos.x = pLayer->GetMap()->m_dwTileNumWidth*TILE_SIZE-1;
	if (AppearItem.v2ItemPos.y > pLayer->GetMap()->m_dwTileNumHeight*TILE_SIZE)
		AppearItem.v2ItemPos.y = pLayer->GetMap()->m_dwTileNumHeight*TILE_SIZE-1;

	ITEM_SERVER* pSItem = (ITEM_SERVER*)LALAlloc(g_pItemPool);

	if(!pSItem)
		return FALSE;

	memcpy(&pSItem->Item, &AppearItem.Item, sizeof(CItem));
	pSItem->v2ItemPos			= AppearItem.v2ItemPos;
	pSItem->dwCurFramePerSec	= 0;						

	MAP_TILE* pTile = pLayer->GetMap()->GetTile(pSItem->v2ItemPos.x, pSItem->v2ItemPos.y);	
	if(!pTile)	return FALSE;
									
	pSItem->dwSectionNum = pTile->wAttr.uSection;	

	if(pSItem->dwSectionNum == 0)
	{
		LALFree( g_pItemPool, (void*)pSItem );
		return FALSE;
	}

	DWORD dwItemOwnerIndex = pUser->GetID();
	
	if( pItemOwnerUser != NULL)
		dwItemOwnerIndex = pItemOwnerUser->GetID();


	MakeItemStruct(pSItem, &pSItem->Item, &pSItem->v2ItemPos, pSItem->dwSectionNum, dwItemOwnerIndex , 0, 0, 0, dwOwnerLimitSec);

	if(pLayer->InsertItem(pSItem)==false)
	{
		LALFree( g_pItemPool, (void*)pSItem );
		return FALSE;
	}

	pLayer->BroadCastSectionMsg((char*)&AppearItem, AppearItem.GetPacketSize(), (BYTE)pSItem->dwSectionNum);

	return TRUE;
}

// 아이템 업그레이드 다운플로우 현상 버그 수정 //hwoarang
const int NORMAL_LIQUD_VALUE	= 20;
const int SUPER_LIQUD_VALUE		= 40;

int ItemUpgrade(CUser* pUser,CItem* pItem[4],BYTE* pbZipCode)
{
	BOOL		bSuccess			= 0;
	CBaseItem*	pBaseItem			= g_pBaseItemHash->GetData( pItem[ITEM_UPGRADE_WEAPON]->GetID() );
	CBaseItem*	pUpGradeBaseItem	= g_pBaseItemHash->GetData( pItem[ITEM_UPGRADE_UPGRADE]->GetID() );
	CBaseItem*	pLiQuidBaseItem		= g_pBaseItemHash->GetData( pItem[ITEM_UPGRADE_LIQUID]->GetID() );
	CBaseItem*  pEditionBaseItem	= NULL;

	if(pItem[ITEM_UPGRDAE_EDITION])
		if(pItem[ITEM_UPGRDAE_EDITION]->GetID() != 0)
			pEditionBaseItem		= g_pBaseItemHash->GetData( pItem[ITEM_UPGRDAE_EDITION]->GetID() );

	if(!pBaseItem)
		return FALSE;

	float	fRate				= 0;
	WORD	wfluent				= 0;
	WORD	wprobability_minus	= 0;
	WORD	wprobability_plus	= 0;

	// wfluent
	// 업그레이드할 아이템의 맥스 슬롯이 ItemLiQuid의 1_Slot 보다 크거나 같으면 
	if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot >= pLiQuidBaseItem->BaseItem_LiQuid.w1_slot)
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_a;
	// 업그레이드할 아이템의 맥스 슬롯이 ItemLiQuid의 2_Slot 보다 크거나 같으면 
	else if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot >= pLiQuidBaseItem->BaseItem_LiQuid.w2_slot)
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_b;
	// 해당되는게 없으면 
	else
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_c;
	
	// wprobability_minus
	wprobability_minus = pUpGradeBaseItem->BaseItem_UpGrade.wProbability_minus;

	// wprobability_plus
	// 0보다 크면 확률 가산제 에디션 아이템 이다.
	if( pEditionBaseItem )
		if( pEditionBaseItem->BaseItem_Edition.wProbability_plus > 0)
			wprobability_plus = pEditionBaseItem->BaseItem_Edition.wProbability_plus;

	// 확률을 구하자 
	// EX) (5 * fluent_a,b,c * (100 - probability_minus) / 100) + probaaility_plus
	
	// fRate
	fRate = (float) ( ( ( 5. * wfluent * (100 - wprobability_minus) ) / 100 ) + wprobability_plus);

	const int nRandomRate = rand() % 100;

	if(fRate < nRandomRate)
	{
		// 실패 했을때의 처리 
		// 유저의 인벤 스몰에 보험아이템이 있는지 검사를 한다.
		// 조디악카드(천칭) 아이템 업그레이드 실패에 대해서 소정의 보험처리가 된다.
		// 실패시 아이템 소실방지. 그러나 업그레이드 한단계 감소 및 옵션 증발.
		// 업그레이드 +6 이후부터는 적용불가함

		if( pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot < 7 )
		{
			// 업그레이드 +6 이전 이므로 보험아이템이 있는지 검사한다.
			BOOL bInsuranceChk = FALSE;

			POSITION_ pos = pUser->m_pUsingItemList->GetHeadPosition();
			POSITION_ del = NULL;

			while(pos)
			{
				del						= pos;
				CItem*		pItem		= (CItem*)pUser->m_pUsingItemList->GetNext(pos);
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItem->GetID());

				if (!pBaseItem)
				{
					pUser->m_pUsingItemList->RemoveAt(del);
					continue;
				}

				// 보험 아이템 이면 
				if (pItem->GetID() == __ITEM_INSURANCE_INDEX__)
				{
					bInsuranceChk	= TRUE;
					break;
				}
			}
			
			// 보헝 처리 해야한다.!!!
			if( bInsuranceChk )
			{
				// 업그레이드 한단계 감소및
				switch (GetItemKind(pItem[ITEM_UPGRADE_WEAPON]->GetID()))
				{
					case ITEM_KIND_WEAPON:
					{
						int Attack = 0;
						
						//리퀴드와 반응시 상승 수치 
						if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
							Attack = pUpGradeBaseItem->BaseItem_UpGrade.wR1_plus;
						//슈퍼리퀴드와 반응시 상승 수치 
						else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
							Attack = pUpGradeBaseItem->BaseItem_UpGrade.wR2_plus;

						// 업그레이드 +1 이상이면
						if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
						{
							if(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage - Attack > 0)
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage -= Attack;
							else
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage = 0;

							if(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage - Attack > 0)
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage -= Attack;
							else
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage = 0;	
						}
						else 
						{
							pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage = pBaseItem->BaseItem_Weapon.wMin_Damage;
							pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage = pBaseItem->BaseItem_Weapon.wMax_Damage;
						}
						
						// 스킬 공격력 추가 
						if( !pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
						{
							if(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage  >= NORMAL_LIQUD_VALUE)
							{
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage -= NORMAL_LIQUD_VALUE;
							}
							else
							{
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage = 0;
							}
						}
						else if( pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
						{
							if(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage >= SUPER_LIQUD_VALUE)
							{
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage -= SUPER_LIQUD_VALUE;
							}
							else
							{
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage = 0;
							}
						}
						
						if(pBaseItem->BaseItem_Weapon.wMax_Slot > pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot)
							pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot	+= 1;

						if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
							pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot	-= 1;
					}
					break;

					case ITEM_KIND_AROMR:
					{
						int Defense = 0;
						
						//리퀴드와 반응시 상승 수치 
						if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
							Defense = pUpGradeBaseItem->BaseItem_UpGrade.wR1_plus;	
						//슈퍼리퀴드와 반응시 상승 수치 
						else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
							Defense = pUpGradeBaseItem->BaseItem_UpGrade.wR2_plus;

						// 업그레이드 +1 이상이면
						if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
						{
							if(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense - Defense > 0)
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense -= Defense;
							else
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense = 0;
						}
						else
						{
							pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense = pBaseItem->BaseItem_Armor.wMin_Damage;
						}
	
						if(pBaseItem->BaseItem_Armor.wMax_Slot > pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot)
							pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot	+= 1;

						if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
							pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot	-= 1;
					}
				}

				// 옵션 증발 
				for( int i = 0; i < MAX_ITEM_ATTR; i++ )
				{
					pItem[ITEM_UPGRADE_WEAPON]->wAttr[i].uCode	= 0;
					pItem[ITEM_UPGRADE_WEAPON]->wAttr[i].uValue	= 0; 
				}

				CItem* pItem = (CItem*)pUser->m_pUsingItemList->GetAt(del);
				pUser->m_pUsingItemList->RemoveAt(del);
				*pbZipCode = BYTE(pItem->m_Item_Zodiac.bInvIndex - 1);

				if( IsUltra() )
				{
					ITEM_NATIVE_INFO ItemNativeInfo;
					ItemNativeInfo.eSlotID		= ITEM_NATIVE_INV_SMALL;
					ItemNativeInfo.bySlotIndex	= *pbZipCode;
					ItemNativeInfo.dwOwnerID	= pUser->GetID();
					
					RemoveItemNative(&ItemNativeInfo);
				}
				else
				{
					memset(&pUser->m_pInv_Small[*pbZipCode],0,sizeof(CItem));
				}
				
				bSuccess = -1;
			}
			else
			{
				// 보험 아이템도 없는 쒜리들.. 아이템 뽀개버려 
				bSuccess = FALSE;
			}
		}
		else
		{
			// 보험 아이템 생각 안한다. 그냥 무조건 실패다 
			bSuccess = FALSE;
		}		
	}
	else 
	{
		// 성공시
		switch (GetItemKind(pItem[ITEM_UPGRADE_WEAPON]->GetID()))
		{
			case ITEM_KIND_WEAPON:
			{
				int Attack = 0;

				//리퀴드와 반응시 상승 수치 
				if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
					Attack = pUpGradeBaseItem->BaseItem_UpGrade.wR1_plus;
				//슈퍼리퀴드와 반응시 상승 수치 
				else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
					Attack = pUpGradeBaseItem->BaseItem_UpGrade.wR2_plus;
				
				pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage	+= Attack;
				pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage	+= Attack;					
				pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot			-= 1;
				pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot			+= 1;

				// 스킬 공격력 추가 
				if( !pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
				{
					pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage	+= NORMAL_LIQUD_VALUE;
				}
				else
				{
					pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage	+= SUPER_LIQUD_VALUE;
				}
				
				// "공격력 업그레이드 성공입니다..."
				bSuccess = ITEM_UPGRADE_SUCCESS_TYPE_AD;
			}
			break;
			
			case ITEM_KIND_AROMR:
			{
				int Defense = 0;
				
				//리퀴드와 반응시 상승 수치 
				if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
					Defense = pUpGradeBaseItem->BaseItem_UpGrade.wR1_plus;	
				//슈퍼리퀴드와 반응시 상승 수치 
				else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
					Defense = pUpGradeBaseItem->BaseItem_UpGrade.wR2_plus;
							
				pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense	+= Defense;				
				pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot		-= 1;
				pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot		+= 1;

				// "방어력 업그레이드 성공입니다..."
				bSuccess = ITEM_UPGRADE_SUCCESS_TYPE_DP;
			}
			break;			
		}
	
		// 에디션 아이템이 확률가산제가 아니고 옵션 추가제 이면 옵션을 추가한다. 
		if( pEditionBaseItem )
		{
			if( pEditionBaseItem->BaseItem_Edition.wProbability_plus == 0 )
			{
				// 옵션 추가제 아이템의 리얼코드 
				WORD wRealOptionCode	= pEditionBaseItem->BaseItem_Edition.wOption;
				// 업글아이템의 리얼코드 
				WORD wRealOptionCodeWA	= 0;
				// 업글아이템의 리얼밸류 
				WORD wValue				= 0;
				WORD wLiQuid_plus		= 0;	

				// 이제 아이템을 검사하여 같은 옵션이 붙어 있는지를 검사한다.
				// 그리고 만약 같은 옵션이 있다면 한번더 올려준다.
				for( int j = 0; pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode != 0 && j < MAX_ITEM_ATTR; j++ )
				{
					wRealOptionCodeWA = pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode;
					if( wRealOptionCode == wRealOptionCodeWA )
					{
						break;
					}
				}
				
				// 같은옵션붙은게 없군...그럼 옵션 들어갈 자리 찾아내.
				if( j >= MAX_ITEM_ATTR )
				{
					for( j = 0; pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode != 0 && j < MAX_ITEM_ATTR; j++ )
					{
						if( pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode == 0 )
						{
							break;
						}
					}
				}

				// 마지막으로 최대값 교정.
				wValue = pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uValue;
				
				//리퀴드와 반응시 상승 수치 
				if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
				{
					wLiQuid_plus	=	pEditionBaseItem->BaseItem_Edition.wLiquid_a_max -
										pEditionBaseItem->BaseItem_Edition.wLiquid_a_min;
					wValue			+=  pEditionBaseItem->BaseItem_Edition.wLiquid_a_min;
					wValue			+=  rand() % (wLiQuid_plus + 1);
				}
				//슈퍼리퀴드와 반응시 상승 수치 
				else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
				{
					wLiQuid_plus	=	pEditionBaseItem->BaseItem_Edition.wLiquid_b_max -
										pEditionBaseItem->BaseItem_Edition.wLiquid_b_min;
					wValue			+=  pEditionBaseItem->BaseItem_Edition.wLiquid_b_min;
					wValue			+=  rand() % (wLiQuid_plus + 1); 
				}				

				pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode	= wRealOptionCode;
				pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uValue	= wValue;
				
				if(bSuccess == ITEM_UPGRADE_SUCCESS_TYPE_AD)
				{
					// "공격력, 옵션 업그레이드 성공입니다..."
					bSuccess = ITEM_UPGRADE_SUCCESS_TYPE_AD + ITEM_UPGRADE_SUCCESS_TYPE_OP;
				}
				else if(bSuccess == ITEM_UPGRADE_SUCCESS_TYPE_DP)
				{
					// "방어력, 옵션 업그레이드 성공입니다..."
					bSuccess = ITEM_UPGRADE_SUCCESS_TYPE_DP + ITEM_UPGRADE_SUCCESS_TYPE_OP;
				}
			}
		}
	}

	return bSuccess;
}


//###########################################################
// 해외 업그레이드 함수 
//###########################################################
int ItemUpgrade_Localizing(CUser* pUser,CItem* pItem[4],BYTE* pbZipCode)
{	
	BOOL		bSuccess			= 0;
	CBaseItem*	pBaseItem			= g_pBaseItemHash->GetData( pItem[ITEM_UPGRADE_WEAPON]->GetID() );
	CBaseItem*	pUpGradeBaseItem	= g_pBaseItemHash->GetData( pItem[ITEM_UPGRADE_UPGRADE]->GetID() );
	CBaseItem*	pLiQuidBaseItem		= g_pBaseItemHash->GetData( pItem[ITEM_UPGRADE_LIQUID]->GetID() );
	CBaseItem*  pEditionBaseItem	= NULL;

	if(pItem[ITEM_UPGRDAE_EDITION])
		if(pItem[ITEM_UPGRDAE_EDITION]->GetID() != 0)
			pEditionBaseItem		= g_pBaseItemHash->GetData( pItem[ITEM_UPGRDAE_EDITION]->GetID() );

	if(!pBaseItem)
		return FALSE;

	// 확률을 구하자 
	// EX) 5 * fluent_a,b,c * (100 - probability_minus) / 100 + probaaility_plus

	float	fRate				= 0;
	WORD	wfluent				= 0;
	WORD	wprobability_minus	= 0;
	WORD	wprobability_plus	= 0;

#if !defined TAIWAN_LOCALIZING
	// wfluent
	// 업그레이드할 아이템의 맥스 슬롯이 ItemLiQuid의 1_Slot 보다 크거나 같으면 
	if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot >= pLiQuidBaseItem->BaseItem_LiQuid.w1_slot)
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_a;
	// 업그레이드할 아이템의 맥스 슬롯이 ItemLiQuid의 2_Slot 보다 크거나 같으면 
	else if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot >= pLiQuidBaseItem->BaseItem_LiQuid.w2_slot)
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_b;
	// 해당되는게 없으면 
	else
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_c;
#else
	BYTE byRemainSlot = BYTE(10 - pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot);

	if(byRemainSlot >= pLiQuidBaseItem->BaseItem_LiQuid.w1_slot)
	{
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_a;
	}
	else if(byRemainSlot >= pLiQuidBaseItem->BaseItem_LiQuid.w2_slot)
	{
		// 업그레이드할 아이템의 남아있는 슬롯이 ItemLiQuid의 2_Slot 보다 크거나 같으면 
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_b;
	}
	else
	{
		// 해당되는게 없으면 
		wfluent = pLiQuidBaseItem->BaseItem_LiQuid.wfluent_c;
	}
#endif
	
	// wprobability_minus
	wprobability_minus = pUpGradeBaseItem->BaseItem_UpGrade.wProbability_minus;

	// wprobability_plus
	// 0보다 크면 확률 가산제 에디션 아이템 이다.
	if( pEditionBaseItem )
		if( pEditionBaseItem->BaseItem_Edition.wProbability_plus > 0)
			wprobability_plus = pEditionBaseItem->BaseItem_Edition.wProbability_plus;
	
	// fRate
	fRate = (float) ( ( ( 5. * wfluent * (100 - wprobability_minus) ) / 100 ) + wprobability_plus);

	int nRandomRate = rand() % 100;

	if(fRate < nRandomRate)
	{
		// 실패 했을때의 처리 
		// 유저의 인벤 스몰에 보험아이템이 있는지 검사를 한다.
		// 조디악카드(천칭) 아이템 업그레이드 실패에 대해서 소정의 보험처리가 된다.
		// 실패시 아이템 소실방지. 그러나 업그레이드 한단계 감소 및 옵션 증발.
		// 업그레이드 +9 이후부터는 적용불가함

		if( pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot < 9 )
		{
			// 업그레이드 +9 이전 이므로 보험아이템이 있는지 검사한다.
			BOOL bInsuranceChk = FALSE;

			POSITION_ pos = pUser->m_pUsingItemList->GetHeadPosition();
			POSITION_ del = NULL;

			while(pos)
			{
				del						= pos;
				CItem*		pItem		= (CItem*)pUser->m_pUsingItemList->GetNext(pos);
				CBaseItem*	pBaseItem	= g_pBaseItemHash->GetData(pItem->GetID());

				if (!pBaseItem)
				{
					pUser->m_pUsingItemList->RemoveAt(del);
					continue;
				}

				// 보험 아이템 이면 
				if (pItem->GetID() == __ITEM_INSURANCE_INDEX__)
				{
					bInsuranceChk	= TRUE;
					break;
				}
			}
			
			// 보헝 처리 해야한다.!!!
			if( bInsuranceChk )
			{
				// 2구간
				if( pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot >= 7 &&
					pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot < 9 )
				{				
					// 업그레이드 한단계 감소및
					switch (GetItemKind(pItem[ITEM_UPGRADE_WEAPON]->GetID()))
					{
						case ITEM_KIND_WEAPON:
						{
							int Attack = 0;
							
							//리퀴드와 반응시 상승 수치 
							if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
								Attack = pUpGradeBaseItem->BaseItem_UpGrade.wR1_plus;
							//슈퍼리퀴드와 반응시 상승 수치 
							else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
								Attack = pUpGradeBaseItem->BaseItem_UpGrade.wR2_plus;

							// 업그레이드 +1 이상이면
							if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
							{
								if(int(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage - Attack) > 0)
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage -= Attack;
								else
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage = 0;

								if(int(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage - Attack) > 0)
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage -= Attack;
								else
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage = 0;	
							}
							else 
							{
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage = pBaseItem->BaseItem_Weapon.wMin_Damage;
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage = pBaseItem->BaseItem_Weapon.wMax_Damage;
							}
							
							// 스킬 공격력 감소 
							if( !pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
							{
								if(int(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage)  >= NORMAL_LIQUD_VALUE)
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage -= NORMAL_LIQUD_VALUE;
								}
								else
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage = 0;
								}
							}
							else if( pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
							{
								if(int(pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage) >= SUPER_LIQUD_VALUE)
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage -= SUPER_LIQUD_VALUE;
								}
								else
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage = 0;
								}
							}

#if defined TAIWAN_LOCALIZING
							// 일반주문서와 반응시
							if( !pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
							{	
								if(pBaseItem->BaseItem_Weapon.wMax_Slot > pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot)
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot	+= 1;
								if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot	-= 1;
							}
							// 티미드주문서와 반응시 
							else if( pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
							{
								if(	int(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot + 2) >
									pBaseItem->BaseItem_Weapon.wMax_Slot )
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Weapon.wMax_Slot;
								}
								else
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot += 2;
								}

								if( int(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot - 2) < 0 )
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot = 0;	
								}
								else
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot -= 2;
								}
							}
#else						
							if(pBaseItem->BaseItem_Weapon.wMax_Slot > pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot)
								pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot	+= 1;
							if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
								pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot	-= 1;
#endif
						}
						break;

						case ITEM_KIND_AROMR:
						{
							int Defense = 0;
							
							// 리퀴드와 반응시 상승 수치 
							if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
								Defense = pUpGradeBaseItem->BaseItem_UpGrade.wR1_plus;	
							// 슈퍼리퀴드와 반응시 상승 수치 
							else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
								Defense = pUpGradeBaseItem->BaseItem_UpGrade.wR2_plus;

							// 업그레이드 +1 이상이면
							if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
							{
								if( pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense > Defense )
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense -= Defense;
								else
									pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense = 0;
							}
							else
							{
								pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense = pBaseItem->BaseItem_Armor.wMin_Damage;
							}
		
#if defined TAIWAN_LOCALIZING
							// 일반주문서와 반응시
							if( !pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
							{	
								if(pBaseItem->BaseItem_Armor.wMax_Slot > pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot)
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot	+= 1;
								if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot	-= 1;
							}
							// 티미드주문서와 반응시 
							else if( pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
							{
								if(	int(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot + 2) >
									pBaseItem->BaseItem_Armor.wMax_Slot )
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot = pBaseItem->BaseItem_Armor.wMax_Slot;
								}
								else
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot += 2;
								}

								if( int(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot - 2) < 0 )
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot = 0;	
								}
								else
								{
									pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot -= 2;
								}
							}
#else						
							if(pBaseItem->BaseItem_Weapon.wMax_Slot > pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot)
								pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot	+= 1;
							if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot > 0)
								pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot	-= 1;
#endif
						}
					}

					// 옵션 증발 
					for( int i = 0; i < MAX_ITEM_ATTR; i++ )
					{
						pItem[ITEM_UPGRADE_WEAPON]->wAttr[i].uCode	= 0;
						pItem[ITEM_UPGRADE_WEAPON]->wAttr[i].uValue	= 0; 
					}

					CItem* pItem = (CItem*)pUser->m_pUsingItemList->GetAt(del);
					pUser->m_pUsingItemList->RemoveAt(del);
					*pbZipCode = BYTE(pItem->m_Item_Zodiac.bInvIndex - 1);
					memset(&pUser->m_pInv_Small[*pbZipCode],0,sizeof(CItem));
					
					bSuccess = -1;
				}
				// 1구간 
				else if(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot < 7 )
				{
					CItem* pItem = (CItem*)pUser->m_pUsingItemList->GetAt(del);
					pUser->m_pUsingItemList->RemoveAt(del);
					*pbZipCode = BYTE(pItem->m_Item_Zodiac.bInvIndex - 1);
					memset(&pUser->m_pInv_Small[*pbZipCode],0,sizeof(CItem));
					
					bSuccess = -1;
				}
			}
			// 보험 아이템도 없는 쒜리들.. 아이템 뽀개버려 
			else
			{
				bSuccess = FALSE;
			}
		}
		// 보험 아이템 생각 안한다. 그냥 무조건 실패다 
		else
		{
			bSuccess = FALSE;
		}		
	}
	else 
	{
		// 성공시
		switch (GetItemKind(pItem[ITEM_UPGRADE_WEAPON]->GetID()))
		{
			case ITEM_KIND_WEAPON:
			{
				int Attack = 0;

				//리퀴드와 반응시 상승 수치 
				if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
					Attack = pUpGradeBaseItem->BaseItem_UpGrade.wR1_plus;
				//슈퍼리퀴드와 반응시 상승 수치 
				else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
					Attack = pUpGradeBaseItem->BaseItem_UpGrade.wR2_plus;
				
				pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMin_Damage	+= Attack;
				pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMax_Damage	+= Attack;	
				
#if defined TAIWAN_LOCALIZING
				// 일반주문서와 반응시
				if( !pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
				{
					pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot		-= 1;
					pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot		+= 1;
				}
				// 티미드 주문서와 반응시
				else if( pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
				{
					if( int(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot - 2) < 0 )
					{
						pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot = 0;
					}
					else
					{
						pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot -= 2;
					}

					if( int(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot + 2) > 
						pBaseItem->BaseItem_Weapon.wMax_Slot )
					{
						pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot = pBaseItem->BaseItem_Weapon.wMax_Slot;
					}
					else
					{
						pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot += 2;
					}
				}
#else
				pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot			-= 1;
				pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot			+= 1;
#endif
				// 스킬 공격력 추가 
				if( !pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
				{
					pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage	+= NORMAL_LIQUD_VALUE;
				}
				else if( pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
				{
					pItem[ITEM_UPGRADE_WEAPON]->m_Item_Weapon.wMana_Damage	+= SUPER_LIQUD_VALUE;
				}
				
				// "공격력 업그레이드 성공입니다..."
				bSuccess = ITEM_UPGRADE_SUCCESS_TYPE_AD;
			}
			break;
			
			case ITEM_KIND_AROMR:
			{
				int Defense = 0;
				
				//리퀴드와 반응시 상승 수치 
				if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
					Defense = pUpGradeBaseItem->BaseItem_UpGrade.wR1_plus;	
				//슈퍼리퀴드와 반응시 상승 수치 
				else// if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
					Defense = pUpGradeBaseItem->BaseItem_UpGrade.wR2_plus;
							
				pItem[ITEM_UPGRADE_WEAPON]->m_Item_Armor.wDefense	+= Defense;	
				
#if defined TAIWAN_LOCALIZING
				// 일반주문서와 반응시
				if( !pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
				{
					pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot		-= 1;
					pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot		+= 1;
				}
				// 티미드 주문서와 반응시
				else if( pUpGradeBaseItem->BaseItem_UpGrade.bclassfication )
				{
					if( int(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot - 2) < 0 )
					{
						pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot = 0;
					}
					else
					{
						pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot -= 2;
					}

					if( int(pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot + 2) > 
						pBaseItem->BaseItem_Armor.wMax_Slot )
					{
						pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot = pBaseItem->BaseItem_Armor.wMax_Slot;
					}
					else
					{
						pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot += 2;
					}									
				}
#else
				pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uMaxSlot			-= 1;
				pItem[ITEM_UPGRADE_WEAPON]->m_bSlot.uUsedSlot			+= 1;
#endif

				// "방어력 업그레이드 성공입니다..."
				bSuccess = ITEM_UPGRADE_SUCCESS_TYPE_DP;
			}
			break;			
		}
	
		// 에디션 아이템이 확률가산제가 아니고 옵션 추가제 이면 옵션을 추가한다. 
		if( pEditionBaseItem )
		{
			if( pEditionBaseItem->BaseItem_Edition.wProbability_plus == 0 )
			{
				// 옵션 추가제 아이템의 리얼코드 
				WORD wRealOptionCode	= pEditionBaseItem->BaseItem_Edition.wOption;
				// 업글아이템의 리얼코드 
				WORD wRealOptionCodeWA	= 0;
				// 업글아이템의 리얼밸류 
				WORD wValue				= 0;
				WORD wLiQuid_plus		= 0;	

				// 이제 아이템을 검사하여 같은 옵션이 붙어 있는지를 검사한다.
				// 그리고 만약 같은 옵션이 있다면 한번더 올려준다.
				for( int j = 0; pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode != 0 && j < MAX_ITEM_ATTR; j++ )
				{
					wRealOptionCodeWA = pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode;
					if( wRealOptionCode == wRealOptionCodeWA )
					{
						break;
					}
				}
				
				// 같은옵션붙은게 없군...그럼 옵션 들어갈 자리 찾아내.
				if( j >= MAX_ITEM_ATTR )
				{
					for( j = 0; pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode != 0 && j < MAX_ITEM_ATTR; j++ )
					{
						if( pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode == 0 )
						{
							break;
						}
					}
				}

				// 마지막으로 최대값 교정.
				wValue = pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uValue;
				
				//리퀴드와 반응시 상승 수치
				if( !pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
				{
					wLiQuid_plus	=	pEditionBaseItem->BaseItem_Edition.wLiquid_a_max -
										pEditionBaseItem->BaseItem_Edition.wLiquid_a_min;
					wValue			+=  pEditionBaseItem->BaseItem_Edition.wLiquid_a_min;
					wValue			+=  rand() % (wLiQuid_plus + 1);
				}
				//슈퍼리퀴드와 반응시 상승 수치 
				else if( pLiQuidBaseItem->BaseItem_LiQuid.bclassfication )
				{
					wLiQuid_plus	=	pEditionBaseItem->BaseItem_Edition.wLiquid_b_max -
										pEditionBaseItem->BaseItem_Edition.wLiquid_b_min;
					wValue			+=  pEditionBaseItem->BaseItem_Edition.wLiquid_b_min;
					wValue			+=  rand() % (wLiQuid_plus + 1); 
				}				

				pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uCode	= wRealOptionCode;
				pItem[ITEM_UPGRADE_WEAPON]->wAttr[j].uValue	= wValue;
				
				if(bSuccess == ITEM_UPGRADE_SUCCESS_TYPE_AD)
				{
					// "공격력, 옵션 업그레이드 성공입니다..."
					bSuccess = ITEM_UPGRADE_SUCCESS_TYPE_AD + ITEM_UPGRADE_SUCCESS_TYPE_OP;
				}
				else if(bSuccess == ITEM_UPGRADE_SUCCESS_TYPE_DP)
				{
					// "방어력, 옵션 업그레이드 성공입니다..."
					bSuccess = ITEM_UPGRADE_SUCCESS_TYPE_DP + ITEM_UPGRADE_SUCCESS_TYPE_OP;
				}
			}
		}
	}

	return bSuccess;
}


// 아이템 제조 
BOOL ItemMaking(CUser* pUser,CItem* pItem[4])
{
	CBaseItem*	pBaseItemMaterial1	= g_pBaseItemHash->GetData( pItem[ITEM_MAKING_MATERIALS1]->GetID() );
	CBaseItem*	pBaseItemMaterial2	= g_pBaseItemHash->GetData( pItem[ITEM_MAKING_MATERIALS2]->GetID() );

	if(!pBaseItemMaterial1 || !pBaseItemMaterial2)
		return FALSE;

	LPITEM_MAKING_INFO pItemMakingInfo1 = g_pItemMakingInfoHash->GetData(pBaseItemMaterial1->BaseItem_Materials.byOrder);
	LPITEM_MAKING_INFO pItemMakingInfo2 = g_pItemMakingInfoHash->GetData(pBaseItemMaterial2->BaseItem_Materials.byOrder);	

	if(pItemMakingInfo1->wComplete != pItemMakingInfo2->wComplete)
		return FALSE;

	int nItemMakingProbability = 0;

#ifdef JAPAN_LOCALIZING	
	nItemMakingProbability = 10;
#else
	nItemMakingProbability = 50;
#endif

	int nRandomRate = rand() % 100;

	// 제조 확률 거꾸로 먹는 버그 수정 : 김영대 05.03.10
	if(nRandomRate >= (100 - nItemMakingProbability))
	{
		// 성공
		CBaseItem*	pBaseItemResult = g_pBaseItemHash->GetData( pItemMakingInfo1->wComplete );
		if(!pBaseItemResult)
			return FALSE;

		memset(pItem[ITEM_MAKING_RESULT],0,sizeof(CItem));
		pItem[ITEM_MAKING_RESULT]->m_Serial				= *g_pThis->GetNewSerial();
		pItem[ITEM_MAKING_RESULT]->m_wItemID			= pBaseItemResult->wID;
		pItem[ITEM_MAKING_RESULT]->m_bSlot.uMaxSlot		= 0;
		pItem[ITEM_MAKING_RESULT]->m_bSlot.uUsedSlot	= 0;

		DWORD dwCode_ID = GetItemKind(pBaseItemResult->wID);
		
		switch (dwCode_ID )
		{

		case ITEM_KIND_WEAPON :
			{
				int	iRand	= 0;
				int	iSlot	= 0;
				int	i		= 0;
				int k       = 0;
				int iValue	= 0;

				pItem[ITEM_MAKING_RESULT]->m_Item_Weapon.wMin_Damage	=	pBaseItemResult->BaseItem_Weapon.wMin_Damage;
				pItem[ITEM_MAKING_RESULT]->m_Item_Weapon.wMax_Damage	=	pBaseItemResult->BaseItem_Weapon.wMax_Damage;
				pItem[ITEM_MAKING_RESULT]->m_Item_Weapon.wMana_Damage	=	pBaseItemResult->BaseItem_Weapon.wMana_Damage;
				
				iSlot = pBaseItemResult->BaseItem_Weapon.wMax_Slot - pBaseItemResult->BaseItem_Weapon.wMin_Slot;
				pItem[ITEM_MAKING_RESULT]->m_bSlot.uMaxSlot = pBaseItemResult->BaseItem_Weapon.wMin_Slot;
				pItem[ITEM_MAKING_RESULT]->m_bSlot.uMaxSlot += rand()%( iSlot + 1 );

				if( pBaseItemResult->BaseItem_Weapon.wSetID != 0 )	
				{// 유니크 아이템 아니면 세트 아이템이라는 소리.
					for( i = 0; i < 6; i++ )
					{
						if( pBaseItemResult->BaseItem_Weapon.pBaseSetItemOption[i].wKind == 0 )
						{
							continue;
						}

						iValue =	pBaseItemResult->BaseItem_Weapon.pBaseSetItemOption[i].wMax -
									pBaseItemResult->BaseItem_Weapon.pBaseSetItemOption[i].wMin;

						if( iValue > 0 )
						{
							pItem[ITEM_MAKING_RESULT]->wAttr[k].uValue = 
								pBaseItemResult->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
							iRand = rand() % (iValue + 1);
							pItem[ITEM_MAKING_RESULT]->wAttr[i].uValue += iRand;
						}
						else
						{
							pItem[ITEM_MAKING_RESULT]->wAttr[k].uValue = 
								pBaseItemResult->BaseItem_Weapon.pBaseSetItemOption[i].wMin;
						}

						if( pItem[ITEM_MAKING_RESULT]->wAttr[i].uValue != 0 )
						{
							pItem[ITEM_MAKING_RESULT]->wAttr[k].uCode = 
								pBaseItemResult->BaseItem_Weapon.pBaseSetItemOption[i].wKind;
							k++;
						}
					}
					return TRUE;
				}
			}
			break;

		case ITEM_KIND_AROMR:
			{
				int			iRand = 0, iSlot = 0;
				int			i = 0, k = 0, iValue = 0;
				WORD		wProbability = 0;

				pItem[ITEM_MAKING_RESULT]->m_Item_Armor.wDefense	= pBaseItemResult->BaseItem_Armor.wMin_Damage;
							
				iSlot = pBaseItemResult->BaseItem_Armor.wMax_Slot - pBaseItemResult->BaseItem_Armor.wMin_Slot;
				pItem[ITEM_MAKING_RESULT]->m_bSlot.uMaxSlot = pBaseItemResult->BaseItem_Armor.wMin_Slot;
				pItem[ITEM_MAKING_RESULT]->m_bSlot.uMaxSlot += rand()%( iSlot + 1 );

				if( pBaseItemResult->BaseItem_Armor.wSetID != 0 )	
				{
					k = 0;
					for( i = 0; i < 6; i++ )
					{
						if( pBaseItemResult->BaseItem_Armor.pBaseSetItemOption[i].wKind == 0 ) continue;

						iValue =	pBaseItemResult->BaseItem_Armor.pBaseSetItemOption[i].wMax -
									pBaseItemResult->BaseItem_Armor.pBaseSetItemOption[i].wMin;
						if( iValue > 0 )
						{
							pItem[ITEM_MAKING_RESULT]->wAttr[k].uValue = pBaseItemResult->BaseItem_Armor.pBaseSetItemOption[i].wMin;
							iRand = rand()%(iValue+1);
							pItem[ITEM_MAKING_RESULT]->wAttr[i].uValue += iRand;
						}
						else
						{
							pItem[ITEM_MAKING_RESULT]->wAttr[k].uValue = pBaseItemResult->BaseItem_Armor.pBaseSetItemOption[i].wMin;
						}

						if( pItem[ITEM_MAKING_RESULT]->wAttr[k].uValue != 0 )
						{
							pItem[ITEM_MAKING_RESULT]->wAttr[k].uCode = pBaseItemResult->BaseItem_Armor.pBaseSetItemOption[i].wKind;
							k++;
						}
					}					
				}

				return TRUE;
			}
			break;
		case ITEM_KIND_RIDE :
			{
				pItem[ITEM_MAKING_RESULT]->m_Item_RideObject.fWorldPos_x = 0;
				pItem[ITEM_MAKING_RESULT]->m_Item_RideObject.fWorldPos_z = 0;
				return TRUE;
			}
			break;
		case ITEM_KIND_GUARDIAN :
			{
				pItem[ITEM_MAKING_RESULT]->m_Item_Guardian.bCreate	= GUARDIAN_CREATE_STATUS_NONE;
				pItem[ITEM_MAKING_RESULT]->m_Item_Guardian.dwID		= 0;

				return TRUE;
			}
			break;
		case ITEM_KIND_BAG:
			{
				pItem[ITEM_MAKING_RESULT]->m_Item_Bag.bType =  pBaseItemResult->BaseItem_Bag.bType;
				WORD wProbability = pBaseItemResult->BaseItem_Bag.WValue_Max - pBaseItemResult->BaseItem_Bag.wValue_Min;
				pItem[ITEM_MAKING_RESULT]->m_Item_Bag.wProbability = pBaseItemResult->BaseItem_Bag.wValue_Min;
				pItem[ITEM_MAKING_RESULT]->m_Item_Bag.wProbability += rand() % ( wProbability + 1 );
				return TRUE;
			}
			break;
		default :
			{
			}
		}
	
	}
	else 
	{
		// 실패 
		return FALSE;
	}	

	return FALSE;
}


void MakeItemStruct( ITEM_SERVER* pMakeItem, CItem* pItem, VECTOR2* pv2Pos, DWORD dwSection, DWORD dwOwnerIndex
					, DWORD dwPartyIndex, DWORD dwRemoveFrame, DWORD dwOwnerFrame, DWORD dwOwnerLimitSec  /* 재원 050329*/ )
{
	if( !pMakeItem )	return;
	if( !pItem )		return;
	if( !pv2Pos )		return;
	
	memcpy( &pMakeItem->Item, pItem, sizeof( CItem ) );
	memcpy( &pMakeItem->v2ItemPos, pv2Pos, sizeof( VECTOR2 ) );
	pMakeItem->dwSectionNum			= dwSection;
	pMakeItem->dwOwnerIndex			= dwOwnerIndex;
	pMakeItem->dwPartyIndex			= dwPartyIndex;
	pMakeItem->dwCurFramePerSec		= dwRemoveFrame;
	pMakeItem->dwOwnerFramePerSec	= dwOwnerFrame;
	pMakeItem->dwOwnerLimitSec    = dwOwnerLimitSec ; //재원 050329
}


void CreateItemByMonster( CMonster* pMonster, CUser* pUser )
{
	if( !pMonster )	return;
	if( !pUser )	return;

	int		i = 0, iRandX = 0, iRandZ = 0, iRandItem = 0, iItemRate = 0, iItemKind = 0;
	WORD	wOptionNum = 0;

	if( pMonster->GetTemp(NPC_TEMP_ITEM) != 0 )	
	{	
		CreateItemByGM( pUser, pMonster->GetTemp(NPC_TEMP_ITEM));
		return;
	}

	if(DUNGEON_TYPE_EVENT == pMonster->GetCurDungeon()->GetDungeonDataEx()->GetDungeonType())
	{	
		if(NULL == pMonster->GetCurDungeon()->GetEventDungeonProperty())
		{
			return;
		}
		
		if(!pMonster->GetCurDungeon()->GetEventDungeonProperty()->byFlagItemDrop)
		{
			return;
		}
	}
	
	iItemRate = rand();	
	iRandItem = pMonster->GetDropItem(
		(int)((100 + g_pThis->m_GLOBAL_MAGIC_FIND_PBT + pUser->GetItemAttr(ITEM_ATTR_MAGIC_FIND_PBT)) / 100.), iItemRate);
	
	if(!iRandItem)
	{
		if( iItemRate <= pMonster->GetBaseMonsterInfo()->wMoneyRate )			
			iRandItem = __ITEM_KARZ__;
		else
			return;			
	}	
	
	iItemKind = iRandItem / ITEM_DISTRIBUTE;	
	
	iItemRate = rand();
	
	if( iItemRate <= pMonster->GetBaseMonsterInfo()->wOption_0 )
	{
		wOptionNum = 0;
	}
	else
	{
		for( i = 0; i < 2; i++ )
		{
			if( iItemRate <= pMonster->GetBaseMonsterInfo()->wOptionRate[i] )
			{
				wOptionNum = i + 1;
				break;
			}
		}
	}

	ITEM_CREATE	AppearItem;
	memset(&AppearItem.Item, NULL, sizeof(CItem));
	AppearItem.Item.m_Serial	= *g_pThis->GetNewSerial();
	AppearItem.Item.m_wItemID	= iRandItem;
	AppearItem.v2ItemPos		= *pMonster->GetCurPosition();
	
	if( ( (iRandX = rand()%125+1) % 2 ) > 0 ) iRandX *= -1;
	if( ( (iRandZ = rand()%125+1) % 2 ) > 0 ) iRandZ *= -1;

	AppearItem.v2ItemPos.x		+=  (float)iRandX;
	AppearItem.v2ItemPos.y		+=	(float)iRandZ;

	MAP_TILE* pTile	= pMonster->GetCurDungeonLayer()->GetMap()->GetTile( AppearItem.v2ItemPos.x, AppearItem.v2ItemPos.y );
	if( !pTile )						return;
	if( pTile->wAttr.uSection == 0 )	return;
	
	AppearItem.dwSectionNum	= pTile->wAttr.uSection;
		
	if(!MakeCItemClass( &AppearItem.Item, iRandItem, pMonster->GetBaseMonsterInfo(), wOptionNum, pUser)) 
	{
		return;
	}

	ITEM_CREATE* pItem = (ITEM_CREATE*)LALAlloc( g_pCreateItemPool );
	if( !pItem ) return;

	memcpy( pItem, &AppearItem, sizeof( ITEM_CREATE ) );
	pItem->dwCreateTick	= g_dwTickCount;
	pItem->dwOwnerIndex	= pUser->GetID();
	
	// 파티 사냥의 드롭 아이템 처리 : 최덕석 2005.1.26
	// 아이템 분배 조건이 '습득자 우선', '랜덤'의 경우(카르츠는 무조건 집어짐)
	if(pUser->m_dwPartyId
		&& (GetItemType(pItem->Item.GetID())==ITEM_TYPE_MONEY ||
			pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_RANDOM ||
			pUser->m_PartyConfig.item == PARTY_CONFIG_ITEM_FIRST))
	{
		// 아이템을 파티 소유로 설정
		pItem->dwPartyIndex = pUser->m_dwPartyId;
	} else
	{
		pItem->dwPartyIndex = 0;
	}
	
	pItem->DelPos		= pMonster->GetCurDungeonLayer()->m_pCreateItemList->AddTail( pItem );
	

	if( !pItem->DelPos )
	{
		Log( LOG_JUST_DISPLAY, "Create Item Monster DelPos NULL" );
	}
}


void CreateItemByGM( CUser* pUser, DWORD dwBaseItemID )
{
	ITEM_CREATE	AppearItem;
	int			iRandX = 0, iRandZ = 0, iRandItem = 0, iItemRate = 0, iItemKind = 0;
	WORD		wOptionNum = 0;

	// GM에 의해 아이템 생성할때
	iItemRate = rand();	
	iRandItem = dwBaseItemID;
	iItemKind = iRandItem / ITEM_DISTRIBUTE;	
	iItemRate = rand();
	wOptionNum = 2;

	memset(&AppearItem.Item, NULL, sizeof(CItem));
	AppearItem.Item.m_Serial	= *g_pThis->GetNewSerial();
	AppearItem.Item.m_wItemID	= iRandItem;
	AppearItem.v2ItemPos		= *pUser->GetCurPosition();
	
	if( ( (iRandX = rand()%125+1) % 2 ) > 0 ) iRandX *= -1;
	if( ( (iRandZ = rand()%125+1) % 2 ) > 0 ) iRandZ *= -1;

	AppearItem.v2ItemPos.x	+=  (float)iRandX;
	AppearItem.v2ItemPos.y	+=	(float)iRandZ;

	MAP_TILE* pTile	= pUser->GetCurDungeonLayer()->GetMap()->GetTile( AppearItem.v2ItemPos.x, AppearItem.v2ItemPos.y );
	if( !pTile )						return;
	if( pTile->wAttr.uSection == 0 )	return;
	
	AppearItem.dwSectionNum	= pTile->wAttr.uSection;	
	
	if(!MakeCItemClass( &AppearItem.Item, iRandItem, NULL, 0, NULL))
	{
		return;
	}

	ITEM_CREATE* pItem = (ITEM_CREATE*)LALAlloc( g_pCreateItemPool );
	if( !pItem ) return;

	memcpy( pItem, &AppearItem, sizeof( ITEM_CREATE ) );
	pItem->dwCreateTick	= g_dwTickCount;
	pItem->dwOwnerIndex	= pUser->GetID();
	pItem->DelPos		= pUser->GetCurDungeonLayer()->m_pCreateItemList->AddTail( pItem );
	
	
	// 아이템 로그 추가 김영대 2005.02.23
	SetItemLog(&pItem->Item, ITEM_LOG_GM_CREATITEM , pUser->GetCurDungeonLayer()->GetParent()->GetID(),
				pUser->m_szName, NULL );

	if( !pItem->DelPos )
	{
		Log( LOG_JUST_DISPLAY, "Create Item Monster DelPos NULL" );
	}
}


BOOL SetItemCount( WORD wBaseItemID, WORD wDungeonID, int iValue )
{
	return 0;
	if(!g_bEnableItemLog)	return FALSE;

	if( wBaseItemID == 0 ) 
		return 0;

	// 로그를 남기지 말아야할 아이템인지 확인한다.
	if( g_pInvalidLogItemHash->bSameKey( wBaseItemID ) )
		return 0;
	
	char szSQL[255]={0,};
	wsprintf( szSQL, "CZP_ITEM_COUNT %d, %d, %d, %d", wBaseItemID, g_pThis->GetServerSetNum(), wDungeonID, iValue );	
	g_pLogDb->THExecuteSQL( szSQL, FALSE, 0, 0, LOG_DB );

	return TRUE;
}


BOOL SetItemLog( CItem* pItem, BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller )
{
	if(!g_bEnableItemLog)	return FALSE;

	if(!pItem)
		return FALSE;
	
	if( pItem->m_wItemID == 0 ) 
		return 0;

	if( pItem->m_wItemID == __ITEM_KARZ__ )	//카르츠는 여기서 거르자 
		return 0;			

	// 로그를 남기지 말아야할 아이템인지 확인한다.
	if( g_pInvalidLogItemHash->bSameKey( pItem->m_wItemID ) ) 
		return 0;

	ITEM_LOG	Log;
	
	memset( &Log, 0, sizeof( ITEM_LOG ) );

	Log.bLogCode	= bLogCode;
	Log.bServerSet	= (BYTE)g_pThis->GetServerSetNum();
	Log.dwDungeonID	= wDungeonID;
	memcpy( &Log.Serial, &pItem->m_Serial, sizeof( ITEM_SERIAL ) );
	memcpy( &Log.Item, pItem, sizeof( CItem ) );
	__lstrcpyn( Log.szOwnerName, szOwner, MAX_CHARACTER_NAME_LENGTH );
	if( szSeller )
		__lstrcpyn( Log.szSellerName, szSeller, MAX_CHARACTER_NAME_LENGTH );
	
	char		szQuery[1024]={0,};
	int			nColumn			= 7;	
	int			col				= 0;
	DWORD		dwParamOffset	= 0;
	DBBINDING*	pBinding		= g_pLogDb->THCreateParamInfo(nColumn);	

	if(!pBinding)
	{
		::Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetItemLog Function()!");
		return 0;
	}

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Serial);
	pBinding[col++].wType	= DBTYPE_UI8;		
	dwParamOffset			+= sizeof(Log.Serial);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bLogCode);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(Log.bLogCode);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bServerSet);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(Log.bServerSet);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.dwDungeonID);
	pBinding[col++].wType	= DBTYPE_UI4;		
	dwParamOffset			+= sizeof(Log.dwDungeonID);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Item);
	pBinding[col++].wType	= DBTYPE_BYTES;		
	dwParamOffset			+= sizeof(Log.Item);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szOwnerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szOwnerName);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szSellerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szSellerName);

#ifdef DEV_MODE
	Log.bServerSet = (BYTE)g_pThis->GetDevModeServerSetNum();
#endif

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	wsprintf( szQuery, "Insert into ItemLog%d_%d%02d%02d (Serial, LogCode, ServerSet, DungeonID, Item, OwnerName, SellerName) values( ?, ?, ?, ?, ?, ?, ? )",
						g_pThis->GetServerSetNum(), CurTime.wYear, CurTime.wMonth, CurTime.wDay);

	g_pLogDb->THExecuteSQLByParam( szQuery, pBinding, &Log.Serial, sizeof(Log), nColumn, FALSE, 0, NULL, LOG_DB);
	
	return TRUE;
}


BOOL SetItemLogKarz( BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller, DWORD dwMoney,CItem* pItem )
{ 
	if(!g_bEnableItemLog)
		return FALSE;

	if(dwMoney == 0)
		return FALSE;
	
	KARZ_LOG Log;	
	memset(&Log,0,sizeof(KARZ_LOG));

	Log.bLogCode	= bLogCode;
	Log.bServerSet	= (BYTE)g_pThis->GetServerSetNum();
	Log.dwDungeonID	= wDungeonID;
	Log.dwMoney		= dwMoney;
	if(pItem != NULL)
	{
		memcpy( &Log.Serial, &pItem->m_Serial, sizeof( ITEM_SERIAL ) );
		memcpy( &Log.Item, pItem, sizeof( CItem ) );
	}	
	__lstrcpyn( Log.szOwnerName, szOwner, MAX_CHARACTER_NAME_LENGTH );
	if( szSeller )
		__lstrcpyn( Log.szSellerName, szSeller, MAX_CHARACTER_NAME_LENGTH );

	char		szQuery[1024]	={0,};
	int			nColumn			= 8;	
	int			col				= 0;
	DWORD		dwParamOffset	= 0;
	DBBINDING*	pBinding		= g_pLogDb->THCreateParamInfo(nColumn);	

	if(!pBinding)
	{
		::Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetItemLog Function()!");
		return 0;
	}

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Serial);
	pBinding[col++].wType	= DBTYPE_UI8;		
	dwParamOffset			+= sizeof(Log.Serial);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bLogCode);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(Log.bLogCode);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.bServerSet);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(Log.bServerSet);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.dwDungeonID);
	pBinding[col++].wType	= DBTYPE_UI4;		
	dwParamOffset			+= sizeof(Log.dwDungeonID);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.Item);
	pBinding[col++].wType	= DBTYPE_BYTES;		
	dwParamOffset			+= sizeof(Log.Item);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.dwMoney);
	pBinding[col++].wType	= DBTYPE_UI4;		
	dwParamOffset			+= sizeof(Log.dwMoney);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szOwnerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szOwnerName);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(Log.szSellerName);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(Log.szSellerName);
	
#ifdef DEV_MODE
	Log.bServerSet = 100;
#endif

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	wsprintf( szQuery, "Insert into KarzLog%d_%d%02d%02d (Serial, LogCode, ServerSet, DungeonID, Item, Money, OwnerName, SellerName) values( ?, ?, ?, ?, ?, ?, ?, ? )",
						g_pThis->GetServerSetNum(), CurTime.wYear, CurTime.wMonth, CurTime.wDay);

	g_pLogDb->THExecuteSQLByParam( szQuery, pBinding, &Log.Serial, sizeof(Log), nColumn, FALSE, 0, NULL, LOG_DB);
	
	return TRUE;
}


void ItemShopRemove(CUser* pUser)
{	
	ITEM_SHOP* pItemShop = g_pItemShopHash->GetData(pUser->GetID());

	BOOL bChk = FALSE;
	
	if(pItemShop)
	{		
		// Inv Insert			
		for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
		{
			if(pUser->m_sPlayerShop.cItem[i].GetID()!=0)
			{
				if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_SMALL)
				{
					int nCnt = Insert_SmallInvItem(pUser
						, &pUser->m_sPlayerShop.cItem[i]
						, 0
						, pUser->m_sPlayerShop.cItem[i].GetQuantity()
						, FALSE);

					if(nCnt==0)
					{
						bChk = TRUE;
						memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
					}
					else
					{
						bChk = TRUE;
						pUser->m_sPlayerShop.cItem[i].SetQuantity((BYTE)nCnt);
					}
				}
				else if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_LARGE)
				{
					for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
					{
						if(pUser->m_pInv_Large[j].GetID()==0)
						{
							bChk = TRUE;
							memcpy(&pUser->m_pInv_Large[j], &pUser->m_sPlayerShop.cItem[i], sizeof(CItem));
							memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
							break;
						}
					}
				}
			}						
		}

		if(g_pThis->GetServerType() == SERVER_TYPE_VILLAGE)
		{
			for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
			{
				if(pUser->m_sPlayerShop.cItem[i].GetID()!=0)
				{
					if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_SMALL)
					{
						int nCnt = Insert_SmallBankItem(pUser
							, &pUser->m_sPlayerShop.cItem[i]
							, 0
							, pUser->m_sPlayerShop.cItem[i].GetQuantity()
							, FALSE);					

						if(nCnt==0)
						{
							memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
							bChk = TRUE;
						}
						else
						{
							pUser->m_sPlayerShop.cItem[i].SetQuantity((BYTE)nCnt);
							bChk = TRUE;
						}
					}
					else if(GetItemType(pUser->m_sPlayerShop.cItem[i].GetID())==ITEM_TYPE_LARGE)
					{
						for(int j = 0; j < MAX_BANK_LARGE_POOL; j++)
						{
							if(pUser->m_pBank_Large[j].GetID()==0)
							{
								bChk = TRUE;
								memcpy(&pUser->m_pBank_Large[j], &pUser->m_sPlayerShop.cItem[i], sizeof(CItem));
								memset(&pUser->m_sPlayerShop.cItem[i], 0, sizeof(CItem));
								break;
							}
						}
					}
				}	
				
				pUser->m_sPlayerShop.dwCustomSellPrice[i] = 0;
			}			
		}
		
		// 디비 쿼리 
		if(bChk)
			PlayerShopLogAll(pUser);

		if(pItemShop->byType==1)
		{
			SHOP_TILE* pNewShopTile = g_pShopTileHash->GetData(pUser->GetID());

			if(pNewShopTile)
			{
				g_pShopTileHash->Delete(pNewShopTile, pUser->GetID(), TRUE);
				memset(pNewShopTile, 0, sizeof(SHOP_TILE));
				pNewShopTile = NULL;
			}			
		}

		g_pItemShopHash->Delete(pItemShop, pUser->GetID(), TRUE);
		memset(pItemShop, 0, sizeof(ITEM_SHOP));
		pItemShop = NULL;
	}
}


