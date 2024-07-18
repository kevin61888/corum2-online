#pragma once


#pragma pack(push,1)

#include <map>
#include "User.h"

#define	MAX_ATTR_DEFINE		500
#define	MAX_ATTR_VALUE_LIST	500


struct ITEM_ATTR_DEFINE
{
	DWORD	dwID;
	WORD	wFormula;
	WORD	wSuccessFormula;
	BYTE	bSkillID;
	BYTE	bValueType;
};


struct ITEM_ATTR_VALUE_LIST
{
	DWORD	dwID;
	BYTE	bValueType;
	short	wMin;
	short	wMax;
	BYTE	bPBT;
};


using namespace std;
class CItemAttrLayer
{

public:
	CItemAttrLayer();//jaewon

	ITEM_ATTR_DEFINE		m_ItemAttrDefine[500];
	ITEM_ATTR_VALUE_LIST	m_ItemAttrValueList[500];

	void	QueryItemAttr();
	DWORD	GetVirtualCode(WORD	wRealOptionCode, WORD wRealOptionValue);
	DWORD	GetValueListValue(WORD wRealOptionValue);
	char*	GetAttrDescription(WORD	wRealOptionCode, WORD wRealOptionValue);
	DWORD	GetValueListID(WORD	wVirtualOptionCode, WORD wValueListValue);
	short	GetVirtualValue(WORD wRealOptionCode, WORD	wRealOptionValue);

	//jaewon 050114 start	
	BYTE	GetSkillLevelByBonusOption( const CUser * pUser, const BYTE bSkillKind) const;	
private:
	typedef map< const BYTE, const ITEM_ATTR_CODE >  SKILL_HASH;
	typedef SKILL_HASH::iterator SKILL_ITOR;
	typedef SKILL_HASH::const_iterator SKILL_C_ITOR;
	SKILL_HASH m_mapSkillLevel;
	void	InitSkillLevelMapping( void) ;
	//jaewon 050114 end 

};


#pragma pack(pop)