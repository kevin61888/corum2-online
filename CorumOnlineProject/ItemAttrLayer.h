#pragma once


#include "../CommonServer/CommonHeader.h"


#define	MAX_ATTR_DEFINE		500
#define	MAX_ATTR_VALUE_LIST	500


#pragma pack(push,1)


struct ITEM_ATTR_DEFINE
{
	DWORD	dwID;
	WORD	wFormula;
	WORD	wSuccessFormula;
	BYTE	bSkillID;
	BYTE	bValueType;
	char	szDescription[100];
	BYTE	bDisplayKind;
};


struct ITEM_ATTR_VALUE_LIST
{
	DWORD	dwID;
	BYTE	bValueType;
	short	wMin;
	short	wMax;
	BYTE	bPBT;
};


class CItemAttrLayer
{
private:

	char	m_szTemp[0xff];

public:

	ITEM_ATTR_DEFINE		m_ItemAttrDefine[MAX_ATTR_DEFINE];
	ITEM_ATTR_VALUE_LIST	m_ItemAttrValueList[MAX_ATTR_VALUE_LIST];

	void	QueryItemAttr				();
	DWORD	GetVirtualCode				(WORD wRealOptionCode, WORD wRealOptionValue);
	DWORD	GetValueListValue			(WORD wRealOptionValue);
	char*	GetAttrDescription			(WORD wRealOptionCode, WORD wRealOptionValue);
	DWORD	GetValueListID				(WORD wVirtualOptionCode, WORD wValueListValue);
	short	GetVirtualValue				(WORD wRealOptionCode, WORD	wRealOptionValue);
	char*	GetItemDescription			(WORD wItemID, WORD wDescNum);
	char*	GetAttrOptionDescription	(WORD wRealOptionCode, WORD wRealOptionValue);
};


#pragma pack(pop)