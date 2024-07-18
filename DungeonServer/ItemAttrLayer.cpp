#include "StdAfx.h"

#include "../CommonServer/CommonHeader.h"
#include "ItemAttrLayer.h"
#include "GameSystem.h"

//jaewon
CItemAttrLayer::CItemAttrLayer()
{
	InitSkillLevelMapping();
}

void CItemAttrLayer::QueryItemAttr()
{
	DBRECEIVEDATA*			pData				= NULL;
	ITEM_ATTR_VALUE_LIST*	pItemAttrValueList	= NULL;
	ITEM_ATTR_DEFINE*		pItemAttrDefine		= NULL;	

	memset(m_ItemAttrDefine, 0, sizeof(m_ItemAttrDefine));
	pData = g_pDb->OpenRecordEx("Select code, formula, success_formula, skill_id, value_type from item_attr_define"
		, MAX_ATTR_DEFINE, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB);
	
	for( DWORD i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pItemAttrDefine = (ITEM_ATTR_DEFINE*)(pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ));
		memcpy(&m_ItemAttrDefine[pItemAttrDefine->dwID], pItemAttrDefine, pData->Query.select.dwRowSize );		
	}

	g_pDb->ReleaseRecordset(pData);

	memset(m_ItemAttrValueList, 0, sizeof(m_ItemAttrValueList));
	pData = g_pDb->OpenRecordEx("Select id, type, min, max, pbt from item_attr_value_list"
		, MAX_ATTR_VALUE_LIST, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB);
	
	for( i = 0; i < pData->Query.select.dwRowCount; i++ )
	{
		pItemAttrValueList = 
			(ITEM_ATTR_VALUE_LIST*)(pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ));
		memcpy(&m_ItemAttrValueList[pItemAttrValueList->dwID], pItemAttrValueList, pData->Query.select.dwRowSize );		
	}

	g_pDb->ReleaseRecordset(pData);
}


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


DWORD CItemAttrLayer::GetValueListID(WORD wVirtualOptionCode, WORD wValueListValue)
{
	if (wVirtualOptionCode >= MAX_ATTR_DEFINE) return 0;

	//(type - 1) * 25 + vitual_value
	return (m_ItemAttrDefine[wVirtualOptionCode].bValueType - 1) * 25 + wValueListValue;
}


short CItemAttrLayer::GetVirtualValue(WORD wRealOptionCode,WORD wRealOptionValue)
{
	if(wRealOptionCode < MAX_ITEM_KIND1_CODE)
	{
		return wRealOptionValue;
	}

	DWORD dwVirtualCode		= GetVirtualCode(wRealOptionCode, wRealOptionValue );
	DWORD dwValueListValue	= GetValueListValue(wRealOptionValue);
	DWORD dwValueListID		= GetValueListID((WORD)dwVirtualCode, (WORD)dwValueListValue);
	
	if (dwValueListID >= MAX_ATTR_VALUE_LIST) return 0;
	
	return m_ItemAttrValueList[dwValueListID].wMin;
}

//jaewon 050114
void CItemAttrLayer::InitSkillLevelMapping( void)
{
	m_mapSkillLevel.insert( make_pair(__SKILL_MEDITETION__,ITEM_ATTR_SKILL_LEVEL_SKILL_MEDITETION ) );	
	m_mapSkillLevel.insert( make_pair(__SKILL_MEDITETION__,ITEM_ATTR_SKILL_LEVEL_SKILL_MEDITETION) );
	m_mapSkillLevel.insert( make_pair(__SKILL_FIREMISSILE__,ITEM_ATTR_SKILL_LEVEL_SKILL_FIREMISSILE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_FIREBLAZE__,ITEM_ATTR_SKILL_LEVEL_SKILL_FIREBLAZE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_MAGMAWALL__,ITEM_ATTR_SKILL_LEVEL_SKILL_MAGMAWALL) );
	m_mapSkillLevel.insert( make_pair(__SKILL_ICEMISSILE__,ITEM_ATTR_SKILL_LEVEL_SKILL_ICEMISSILE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_ICEWAVE__,ITEM_ATTR_SKILL_LEVEL_SKILL_ICEWAVE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_ICEWALL__,ITEM_ATTR_SKILL_LEVEL_SKILL_ICEWALL) );
	m_mapSkillLevel.insert( make_pair(__SKILL_LIGHTNING__,ITEM_ATTR_SKILL_LEVEL_SKILL_LIGHTNING) );
	m_mapSkillLevel.insert( make_pair(__SKILL_AIREALCOAT__,ITEM_ATTR_SKILL_LEVEL_SKILL_AIREALCOAT) );
	m_mapSkillLevel.insert( make_pair(__SKILL_THUNDERSTORM__,ITEM_ATTR_SKILL_LEVEL_SKILL_THUNDERSTORM) );
	m_mapSkillLevel.insert( make_pair(__SKILL_LANDINGSUPPORT__,ITEM_ATTR_SKILL_LEVEL_SKILL_LANDINGSUPPORT) );
	m_mapSkillLevel.insert( make_pair(__SKILL_EARTHQUAKE__,ITEM_ATTR_SKILL_LEVEL_SKILL_EARTHQUAKE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_METEOR__,ITEM_ATTR_SKILL_LEVEL_SKILL_METEOR) );
	m_mapSkillLevel.insert( make_pair(__SKILL_MANAEXPAND__,ITEM_ATTR_SKILL_LEVEL_SKILL_MANAEXPAND) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SPEEDCASTING__,ITEM_ATTR_SKILL_LEVEL_SKILL_SPEEDCASTING) );
	m_mapSkillLevel.insert( make_pair(__SKILL_EXPLOSION__,ITEM_ATTR_SKILL_LEVEL_SKILL_EXPLOSION) );
	m_mapSkillLevel.insert( make_pair(__SKILL_CONCENTRATION__,ITEM_ATTR_SKILL_LEVEL_SKILL_CONCENTRATION) );
	m_mapSkillLevel.insert( make_pair(__SKILL_REGENERATION__,ITEM_ATTR_SKILL_LEVEL_SKILL_REGENERATION) );
	m_mapSkillLevel.insert( make_pair(__SKILL_WEAPONMASTERY__,ITEM_ATTR_SKILL_LEVEL_SKILL_WEAPONMASTERY) );
	m_mapSkillLevel.insert( make_pair(__SKILL_DEFENSEMASTERY__,ITEM_ATTR_SKILL_LEVEL_SKILL_DEFENSEMASTERY) );
	m_mapSkillLevel.insert( make_pair(__SKILL_AURARECHARGE__,ITEM_ATTR_SKILL_LEVEL_SKILL_AURARECHARGE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_ZEAL__,ITEM_ATTR_SKILL_LEVEL_SKILL_ZEAL) );
	m_mapSkillLevel.insert( make_pair(__SKILL_BASH__,ITEM_ATTR_SKILL_LEVEL_SKILL_BASH) );
	m_mapSkillLevel.insert( make_pair(__SKILL_LIGHTNINGBREAK__,ITEM_ATTR_SKILL_LEVEL_SKILL_LIGHTNINGBREAK) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SILENTBRANDYSHIP__,ITEM_ATTR_SKILL_LEVEL_SKILL_SILENTBRANDYSHIP) );
	m_mapSkillLevel.insert( make_pair(__SKILL_ANTAGONIZE__,ITEM_ATTR_SKILL_LEVEL_SKILL_ANTAGONIZE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_AURABATTLER__,ITEM_ATTR_SKILL_LEVEL_SKILL_AURABATTLER) );
	m_mapSkillLevel.insert( make_pair(__SKILL_LIFEFORCE__,ITEM_ATTR_SKILL_LEVEL_SKILL_LIFEFORCE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_PRAY__,ITEM_ATTR_SKILL_LEVEL_SKILL_PRAY) );
	m_mapSkillLevel.insert( make_pair(__SKILL_BLESSING__,ITEM_ATTR_SKILL_LEVEL_SKILL_BLESSING) );
	m_mapSkillLevel.insert( make_pair(__SKILL_WINDTALES__,ITEM_ATTR_SKILL_LEVEL_SKILL_WINDTALES) );
	m_mapSkillLevel.insert( make_pair(__SKILL_HOLDSHIELD__,ITEM_ATTR_SKILL_LEVEL_SKILL_HOLDSHIELD) );
	m_mapSkillLevel.insert( make_pair(__SKILL_WINDFORCE__,ITEM_ATTR_SKILL_LEVEL_SKILL_WINDFORCE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_AMFLEAFIRE__,ITEM_ATTR_SKILL_LEVEL_SKILL_AMFLEAFIRE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_HOLYPRESSURE__,ITEM_ATTR_SKILL_LEVEL_SKILL_HOLYPRESSURE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_APSOLUTMAGICBARRIER__,ITEM_ATTR_SKILL_LEVEL_SKILL_APSOLUTMAGICBARRIER) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SPELLBINDING__,ITEM_ATTR_SKILL_LEVEL_SKILL_SPELLBINDING) );
	m_mapSkillLevel.insert( make_pair(__SKILL_DOMENATION__,ITEM_ATTR_SKILL_LEVEL_SKILL_DOMENATION) );
	m_mapSkillLevel.insert( make_pair(__SKILL_MIRACULOUSHEART__,ITEM_ATTR_SKILL_LEVEL_SKILL_MIRACULOUSHEART) );
	m_mapSkillLevel.insert( make_pair(__SKILL_ARCANUMSKIN__,ITEM_ATTR_SKILL_LEVEL_SKILL_ARCANUMSKIN) );
	m_mapSkillLevel.insert( make_pair(__SKILL_FRENZY__,ITEM_ATTR_SKILL_LEVEL_SKILL_FRENZY) );
	m_mapSkillLevel.insert( make_pair(__SKILL_CALMDOWN__,ITEM_ATTR_SKILL_LEVEL_SKILL_CALMDOWN) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SLEEP__,ITEM_ATTR_SKILL_LEVEL_SKILL_SLEEP) );
	m_mapSkillLevel.insert( make_pair(__SKILL_CONFUSE__,ITEM_ATTR_SKILL_LEVEL_SKILL_CONFUSE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SOULBLASTER__,ITEM_ATTR_SKILL_LEVEL_SKILL_SOULBLASTER) );
	m_mapSkillLevel.insert( make_pair(__SKILL_MINDCONTROL__,ITEM_ATTR_SKILL_LEVEL_SKILL_MINDCONTROL) );
	m_mapSkillLevel.insert( make_pair(__SKILL_RAGE__,ITEM_ATTR_SKILL_LEVEL_SKILL_RAGE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_REDELEMENTAL__,ITEM_ATTR_SKILL_LEVEL_SKILL_REDELEMENTAL) );
	m_mapSkillLevel.insert( make_pair(__SKILL_BLUEELEMENTAL__,ITEM_ATTR_SKILL_LEVEL_SKILL_BLUEELEMENTAL) );
	m_mapSkillLevel.insert( make_pair(__SKILL_GREENELEMENTAL__,ITEM_ATTR_SKILL_LEVEL_SKILL_GREENELEMENTAL) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SPEEDOVER__,ITEM_ATTR_SKILL_LEVEL_SKILL_SPEEDOVER) );
	m_mapSkillLevel.insert( make_pair(__SKILL_DODGE__,ITEM_ATTR_SKILL_LEVEL_SKILL_DODGE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_POISONING__,ITEM_ATTR_SKILL_LEVEL_SKILL_POISONING) );
	m_mapSkillLevel.insert( make_pair(__SKILL_CHAINATTACKER__,ITEM_ATTR_SKILL_LEVEL_SKILL_CHAINATTACKER) );
	m_mapSkillLevel.insert( make_pair(__SKILL_BLASTQUAKE__,ITEM_ATTR_SKILL_LEVEL_SKILL_BLASTQUAKE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_IRONSKIN__,ITEM_ATTR_SKILL_LEVEL_SKILL_IRONSKIN) );
	m_mapSkillLevel.insert( make_pair(__SKILL_HIDING__,ITEM_ATTR_SKILL_LEVEL_SKILL_HIDING) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SPELLTRAP__,ITEM_ATTR_SKILL_LEVEL_SKILL_SPELLTRAP) );
	m_mapSkillLevel.insert( make_pair(__SKILL_PRESSURE__,ITEM_ATTR_SKILL_LEVEL_SKILL_PRESSURE) );
	m_mapSkillLevel.insert( make_pair(__SKILL_LIFEEXPLOSION__,ITEM_ATTR_SKILL_LEVEL_SKILL_LIFEEXPLOSION) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SYLPHID__,ITEM_ATTR_SKILL_LEVEL_SKILL_SYLPHID) );
	m_mapSkillLevel.insert( make_pair(__SKILL_SHAUT__,ITEM_ATTR_SKILL_LEVEL_SKILL_SHAUT) );
	m_mapSkillLevel.insert( make_pair(__SKILL_BERSERK__,ITEM_ATTR_SKILL_LEVEL_SKILL_BERSERK) );
	
	
}

//jaewon 050114
BYTE CItemAttrLayer::GetSkillLevelByBonusOption( const CUser * pUser, const BYTE bSkillKind ) const
{
	SKILL_C_ITOR Itor =  m_mapSkillLevel.find(bSkillKind);

	if( m_mapSkillLevel.end() != Itor)
	{
		const ITEM_ATTR_CODE eAttrCode = (*Itor).second;
		if( NULL != pUser )
		{
			return pUser->GetItemAttr( eAttrCode );
		}
	}

	return 0;
}