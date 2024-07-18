#ifndef		__ITEM_MOVE_INC__
#define		__ITEM_MOVE_INC__


#define		DEF_COPYITEM_TYPE_LARGE		1		// Large, Equip, Bank Large.
#define		DEF_COPYITEM_TYPE_SMALL		2		// Small, Belt, Bank Small.


class CItem;
class CUser;
class CBaseItem;
class CDungeonLayer;
struct DSTC_ITEM_PICKUP;
struct DSTC_ITEM_MOVE;
struct ITEM_NATIVE_INFO;
struct ITEM_NATIVE_INFOEX;


void		ConvertItem(CItem* pItem);
BYTE		Insert_PlayerShopItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
BYTE		Insert_SmallBeltItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bNew);
int 		Insert_SmallBankItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bChk = TRUE);
int			Insert_SmallInvItem(CUser* pUser, const CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bNew, BOOL bChk = TRUE);
void		Insert_TradeItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BYTE byNew);
void		Insert_MagicFieldArrayItem(CUser* pUser, CItem* pItem, BYTE byPackCnt);
void		Insert_UpgradeItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
void		Insert_MakingItem(CUser* pUser, CItem* pItem, BYTE bZipCode, BYTE byPackCnt);
BOOL		SetPositionPotalItem(CItem* pItem, BYTE byLayerIndex, WORD wMapID, WORD wPosX, WORD wPosZ);
void		Trade_Cancel(CUser* pUser);
BOOL		Trade_SmallItem(CItem* pInvItem, CItem* pItem);
int			Insert_SmallInvItem2(CItem* pInv_Small, CItem* pItem, BYTE bZipCode, BYTE byPackCnt, BOOL bChk);
void		SendToUpdate_GuardianItem(DWORD dwConnectionIndex, CItem* pGuardianItem);
void		PlayerShopLogAll(CUser* pUser);
void		PlayerShopLog(CUser* pUser, CItem* pItem, BYTE byInv, BYTE byIndex, DWORD dwPrice, DWORD dwCnt, BOOL bReturn);
void		PlayerShopPriceLog(CUser* pUser, CItem* pItem, BYTE byInv, BYTE byIndex, DWORD dwPrice, DWORD dwCnt, BOOL bReturn, DWORD dwOwnUserIndex, DWORD dwOwnPrice, BYTE byOwnZipcode, BYTE byOwnZipcode2);
BOOL		IsCopyItem(CUser* pCheckUser, CItem* pInsertItem, int nInvType);
void		SendCopyItemPacket(CUser* pUser, CItem* pInsertItem, int nInvType);
BOOL		SetIsCopyItem(CUser* pUser, DSTC_ITEM_PICKUP* pPickupItem, CItem* pItem, BYTE byInv, BYTE byType, BOOL bChk = FALSE);
BOOL		IsCopyItem(CUser* pUser, DSTC_ITEM_PICKUP* pPickupItem, CItem* pItem, BYTE byInv, BYTE byType, BOOL bChk = FALSE);
void		ItemHandMod(CUser*	pUser, DWORD dwEqCode1, DWORD dwEqCode2);
void		ItemArmorMod(CUser* pUser, DWORD dwEqCode);
void		ItemHelmetMod(CUser* pUser, DWORD dwEqCode);
BOOL		CheckItem(CItem* pItem, enum ITEM_TYPE eType);
int			GetItemIndex(CItem* pItem);
void		SetPortal(CItem* pItem);
void		CoypPickupMsg(DSTC_ITEM_PICKUP* pPickupItem, CItem* pItem, BYTE byInv, BYTE byZipCode);
BOOL		CheckShop(CUser* pUser);
BOOL		CheckStore(DWORD dwKey);
BOOL		CheckTrade(__int64 nKey);
void		CheckSpeed(CUser* pUser);
CBaseItem*	GetBaseItem(WORD wId);
void		CheckItemSlot(CUser* pUser);
BOOL		CheckEquip(BYTE byValue, BYTE byEquipCode);
void		PlayerShop_Cancel(CUser* pUser);
void		Making_Cancel(CUser* pUser);
void		UpGrade_Cancel(CUser* pUser);
void		GuardianSupplies_Cancel(CUser* pUser);


#endif