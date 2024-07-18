#pragma once

class CItem;

BOOL ItemUsedZodiac(CItem* pItem, BYTE byZipcode, BYTE byType);
BOOL ItemUsedSupplies(CItem* pItem, BYTE byZipcode, BYTE byType);
BOOL ItemUsedConsumable(CItem* pItem, BYTE byZipcode, BYTE byType);
BOOL ItemUsedSpecial(CItem* pItem, BYTE byZipcode, BYTE byType);
BOOL ItemUsedZodianInsurance(CItem* pItem, BYTE byZipcode, BYTE byType);
BOOL ItemUsedPresentBox(CItem* pItem, BYTE byZipcode, BYTE byType);
BOOL ItemUsedMagicArray(CItem* pItem, BYTE byZipcode, BYTE byType);
BOOL IsSamePropertyConsumableItem(CItem* pSrc, CItem* pDst);