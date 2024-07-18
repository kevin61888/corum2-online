#pragma once
#include "../CommonServer/CommonHeader.h"
#include "Linkedlist.h"
#include "GameSystem.h"
class CMonster;
class CMonsterList;
class CPcList;

#pragma pack(push,1)

struct Section_Link_Info
{
	WORD	wSectionNum;
	WORD	x1, y1, x2, y2;

	WORD	pwSectionLinkNum[ MAX_SECTION_LINK_NUM ];

	Section_Link_Info()
	{
		memset( this, 0, sizeof( Section_Link_Info ) );
	}
};


class CSection
{
public:

	BYTE			m_bSectionNum;		// 1번부터 ZeroBase 아님 
	WORD			x1, y1, x2, y2;
	WORD			m_wLink[ MAX_SECTION_LINK_NUM ];
	CPcList*		m_pPcList;
	CMonsterList*	m_pMonsterList;
	LPITEM_HASH		m_pItemHash;
	POSITION_		m_pActiveListPos;	// ActiveSectionList 리스트에서의 Node의 위치 
	
public:

	CMonster*		FindMonster(DWORD dwBaseMonsterID);		
	
	CSection();
	~CSection();
};

#pragma pack(pop)

