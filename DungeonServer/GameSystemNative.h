#pragma once


#include "StdAfx.h"


class	CUser;
class	CMonster;
class	CItem;
struct	BASEMONSTER;
struct  VECTOR2;
struct  ITEM_SERVER;


enum STORE_FLAG
{
	Store_Flag_Out_Store	= 0,					
	Store_Flag_In_Store		= 1,
};


int		ItemUpgrade(CUser* pUser,CItem* pItem[4],BYTE* pbZipCode);
int		ItemUpgrade_Localizing(CUser* pUser,CItem* pItem[4],BYTE* pbZipCode);
BOOL	ItemMaking(CUser* pUser,CItem* pItem[4]);
void	ItemShopRemove(CUser* pUser);


BOOL	SetItemCount( WORD wBaseItemID, WORD wDungeonID, int iValue );
BOOL	SetItemLog( CItem* pItem, BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller );
BOOL	SetItemLogKarz( BYTE bLogCode, WORD wDungeonID, char* szOwner, char* szSeller,DWORD dwMoney,CItem* pItem = NULL );


BOOL	CreateItem(CItem* pItem, WORD wItemID, BYTE byCount, BYTE byInStoreFlag = Store_Flag_Out_Store, BYTE byUseItemOption = FALSE);
BOOL	MakeCItemClass( CItem* pItem, WORD wBaseItemID, const BASEMONSTER* const pBaseMonster, WORD wOptionNum, CUser* pUser );
BOOL	AppearItem(CUser* pUser, const CItem* pItem, BYTE byCount, DWORD dwMoney,  BOOL bRandom = TRUE, float fX = 0, float fY = 0, float fZ = 0, DWORD dwOwnerLimitSec = 10, CUser* pItemOwerUser = NULL);
void	MakeItemStruct( ITEM_SERVER* pMakeItem, CItem*	pItem, VECTOR2*	pv2Pos, DWORD dwSection, DWORD dwOwnerIndex, DWORD dwPartyIndex, DWORD dwRemoveFrame, DWORD dwOwnerFrame, DWORD dwOwnerLimitSec = 10 /* Àç¿ø 050329*/ );
void	CreateItemByMonster( CMonster* pMonster, CUser* pUser );
void	CreateItemByGM( CUser* pUser, DWORD dwBaseItemID );
