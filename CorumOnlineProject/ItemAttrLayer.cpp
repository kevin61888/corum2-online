#include "StdAfx.h"
#include "ItemAttrLayer.h"
#include "../CommonServer/CommonHeader.h"
#include "struct.h"
#include "InitGame.h"


DWORD CItemAttrLayer::GetVirtualCode(WORD wRealOptionCode,WORD wRealOptionValue)
{
	//(real_code - 32) * 20 + int((real_value-1)/25) + 32
	if(wRealOptionCode < MAX_ITEM_KIND1_CODE)
	{
		return wRealOptionCode;
	}

	return (wRealOptionCode - MAX_ITEM_KIND1_CODE) * 20 + (wRealOptionValue - 1) / 25 + MAX_ITEM_KIND1_CODE;
}

DWORD CItemAttrLayer::GetValueListValue(WORD wRealOptionValue)
{
	// real_value mod 25
	DWORD dwValueListValue = wRealOptionValue % 25;

	if(!dwValueListValue)
	{
		dwValueListValue = 25;
	}
	
	return dwValueListValue;
}

DWORD	CItemAttrLayer::GetValueListID(WORD	wVirtualOptionCode, WORD wValueListValue)
{
	//(type - 1) * 25 + vitual_value
	return (m_ItemAttrDefine[wVirtualOptionCode].bValueType-1)*25+wValueListValue;
}

short	CItemAttrLayer::GetVirtualValue(WORD wRealOptionCode,WORD wRealOptionValue)
{
	if(wRealOptionCode < MAX_ITEM_KIND1_CODE)
	{
		return wRealOptionValue;
	}

	DWORD dwVirtualCode		= GetVirtualCode(wRealOptionCode, wRealOptionValue );
	DWORD dwValueListValue	= GetValueListValue(wRealOptionValue);
	DWORD dwValueListID		= GetValueListID((WORD)dwVirtualCode, (WORD)dwValueListValue);
	
	return m_ItemAttrValueList[dwValueListID].wMin;
	
}


char*	CItemAttrLayer::GetItemDescription(WORD wItemID, WORD wDescNum)
{
//	LP_ITEM_TABLE pItemInfo = g_pItemTableHash->GetData(wItemID);
	CBaseItem* pItemInfo = g_pItemTableHash->GetData(wItemID);
	
//	switch(pItemInfo->nItemType)
	switch(pItemInfo->dwCode_ID)
	{
	case ITEM_KIND_CONSUMABLE:
		{
			LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemID);

			switch(lpItemOption->byDisplayFlag[wDescNum])
			{
			case '0':
				wsprintf(m_szTemp, "%s", lpItemOption->szOption[wDescNum]);
				break;
			case '1':// %d분 동안
				wsprintf(m_szTemp, lpItemOption->szOption[wDescNum], pItemInfo->BaseItem_Consumable.wLast_Time);
				break;
			case '2':// %분 동안 %d
				wsprintf(m_szTemp
					, lpItemOption->szOption[wDescNum]
					, pItemInfo->BaseItem_Consumable.wLast_Time
					, GetVirtualValue(pItemInfo->BaseItem_Consumable.sItem_Attr[0].wAttr_Code, pItemInfo->BaseItem_Consumable.sItem_Attr[0].wAttr_Value));
				break;
			case '3':// opition value %d
				wsprintf(m_szTemp
					, lpItemOption->szOption[wDescNum]
					, GetVirtualValue(pItemInfo->BaseItem_Consumable.sItem_Attr[0].wAttr_Code, pItemInfo->BaseItem_Consumable.sItem_Attr[0].wAttr_Value));
				break;
			case '4':// opition value %d
				wsprintf(m_szTemp
					, lpItemOption->szOption[wDescNum]
					, GetVirtualValue(pItemInfo->BaseItem_Consumable.sItem_Attr[1].wAttr_Code, pItemInfo->BaseItem_Consumable.sItem_Attr[1].wAttr_Value));
				break;
			case '5':// opition value %d
				wsprintf(m_szTemp
					, lpItemOption->szOption[wDescNum]
					, GetVirtualValue(pItemInfo->BaseItem_Consumable.sItem_Attr[2].wAttr_Code, pItemInfo->BaseItem_Consumable.sItem_Attr[2].wAttr_Value));
				break;
			case '6':// opition value %d
				wsprintf(m_szTemp
					, lpItemOption->szOption[wDescNum]
					, GetVirtualValue(pItemInfo->BaseItem_Consumable.sItem_Attr[3].wAttr_Code, pItemInfo->BaseItem_Consumable.sItem_Attr[3].wAttr_Value));
				break;
			case '7':// opition value %d
				wsprintf(m_szTemp
					, lpItemOption->szOption[wDescNum]
					, GetVirtualValue(pItemInfo->BaseItem_Consumable.sItem_Attr[4].wAttr_Code, pItemInfo->BaseItem_Consumable.sItem_Attr[4].wAttr_Value));
				break;			
			}
			
		}break;
	default:
		m_szTemp[0] = NULL;
	}
	
	return m_szTemp;
}


char*	CItemAttrLayer::GetAttrOptionDescription(WORD	wRealOptionCode, WORD wRealOptionValue)
{
	DWORD dwVirtualCode		= 0;
	DWORD dwValueListValue	= 0;
	DWORD dwValueListID		= 0;
	
	// 기존의 아이템 옵션종류군.
	if( wRealOptionCode < MAX_ITEM_KIND1_CODE )
	{
		dwVirtualCode = wRealOptionCode;
	}
	else
	{
		// 아이템 옵션2종류군.
		dwVirtualCode		= GetVirtualCode(wRealOptionCode, wRealOptionValue );
		dwValueListValue	= GetValueListValue(wRealOptionValue);
		dwValueListID		= GetValueListID((WORD)dwVirtualCode, (WORD)dwValueListValue);
	}		
	
	switch(m_ItemAttrDefine[dwVirtualCode].bDisplayKind)
	{
		case 0:// item option value 를 출력하라.
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, wRealOptionValue);
			break;
		case 1:// 확장 옵션 테이블의 ValueList테이블의 min
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, m_ItemAttrValueList[dwValueListID].wMin);
			break;
		case 2:// 확장 옵션 테이블의 ValueList테이블의 max
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, m_ItemAttrValueList[dwValueListID].wMax);
			break;
		case 3:// 확장 옵션 테이블의 ValueList테이블의 pbt
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, m_ItemAttrValueList[dwValueListID].bPBT);
			break;
		case 4:// 확장 옵션 테이블의 ValueList테이블의 min, max
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, m_ItemAttrValueList[dwValueListID].wMin, m_ItemAttrValueList[dwValueListID].wMax);
			break;
		case 5:// 확장 옵션 테이블의 ValueList테이블의 pbt, min
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, m_ItemAttrValueList[dwValueListID].bPBT, m_ItemAttrValueList[dwValueListID].wMin);
			break;
		case 6:// 확장 옵션 테이블의 ValueList테이블의 MIN / 10 을 %붙여 출력
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin));
			break;
		case 7:// 확장 옵션 테이블의 ValueList테이블의 MIN / 11 을 %붙여 출력 
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin/11));
			break;
		case 8:// 확장 옵션 테이블의 ValueList테이블의 MIN / 12 을 %붙여 출력 
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin/12));
			break;
		case 9:// 확장 옵션 테이블의 ValueList테이블의 MIN / 13 을 %붙여 출력 
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin/13));
			break;
		case 10:// 확장 옵션 테이블의 ValueList테이블의 MIN / 14 을 %붙여 출력 
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin/14));
			break;
		case 11:// 확장 옵션 테이블의 ValueList테이블의 MIN / 15 을 %붙여 출력 
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin/15));
			break;
		case 12://MIN * 0.7, MIN * 1.3
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin*0.7), int(m_ItemAttrValueList[dwValueListID].wMin*1.3));
			break;
		case 13://MIN * 0.8, MIN * 1.2
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin*0.8), int(m_ItemAttrValueList[dwValueListID].wMin*1.2));
			break;
		case 14://MIN * 0.9, MIN * 1.1
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin*0.9), int(m_ItemAttrValueList[dwValueListID].wMin*1.1));
			break;
		case 15://MIN * 0.1, MIN * 1.9
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin*0.1), int(m_ItemAttrValueList[dwValueListID].wMin*1.9));
			break;
		case 16://MIN * 0.6, MIN * 1.4
			wsprintf(m_szTemp, m_ItemAttrDefine[dwVirtualCode].szDescription, int(m_ItemAttrValueList[dwValueListID].wMin*0.6), int(m_ItemAttrValueList[dwValueListID].wMin*1.4));
			break;
	}
	
	return m_szTemp;	
}