#ifndef __SECTION_H__
#define __SECTION_H__


#pragma once


#include "LinkedListEx.h"
#include "GlobalDefine.h"


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

	WORD	m_wSectionNum;		//1น๘บฮลอ ZeroBase พฦดิ 
	WORD	x1, y1, x2, y2;
	WORD	m_wLink[ MAX_SECTION_LINK_NUM ];
};
#pragma pack(pop)


#endif 

