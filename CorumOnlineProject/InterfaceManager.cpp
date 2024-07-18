//#################################################################################################
//	Class Name			: CInterfaceManager	
//	File Name			: InterfaceManager.cpp
//	Description			: Managing Interface
//						  - Check Mouse Click Point and Process Callback Function
//	Written by minjin. 2004. 11. 18.
//							for Refactoring Interface Codes...
//#################################################################################################

#include "stdafx.h"
#include "GameControl.h"
#include "InterfaceManager.h"

CInterfaceManager* CInterfaceManager::m_pThis = NULL;

CInterfaceManager::CInterfaceManager()
{
	InitInterfacePositionChecker();
}

CInterfaceManager::~CInterfaceManager()
{
	DestroyInstance();
}

CInterfaceManager* CInterfaceManager::GetInstance()
{
	if(!m_pThis)
	{
		m_pThis = new CInterfaceManager;
		return m_pThis;
	}
	
	return NULL;
}

void CInterfaceManager::DestroyInstance()
{
	if(m_pThis)
	{
		delete m_pThis;
		m_pThis = NULL;
	}
}

void CInterfaceManager::InitInterfacePositionChecker()
{
	// 콜백 함수는 지금 작업중..
	// 일단 리턴값으로 넣어 놓았습니다.. -_-;;

	for ( int nUICheckerCnt = 0; nUICheckerCnt < MAX_INTERFACE_CHECKER; nUICheckerCnt++ )
	{
		m_InterfaceChecker[nUICheckerCnt].lLeft					= NULL;
		m_InterfaceChecker[nUICheckerCnt].lRight				= NULL;
		m_InterfaceChecker[nUICheckerCnt].lTop					= NULL;
		m_InterfaceChecker[nUICheckerCnt].lBottom				= NULL;
		m_InterfaceChecker[nUICheckerCnt].pOvrProc				= NULL;
		m_InterfaceChecker[nUICheckerCnt].pLDnProc				= NULL;
		m_InterfaceChecker[nUICheckerCnt].pLUpProc				= NULL;
		m_InterfaceChecker[nUICheckerCnt].pRDnProc				= NULL;
		m_InterfaceChecker[nUICheckerCnt].pRUpProc				= NULL;
	}

	// Item Inventory Window Button
	m_InterfaceChecker[UI_BTN_ITEM].lLeft						= 83;
	m_InterfaceChecker[UI_BTN_ITEM].lTop						= 684;
	m_InterfaceChecker[UI_BTN_ITEM].lRight						= m_InterfaceChecker[UI_BTN_ITEM].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_BTN_ITEM].lBottom						= m_InterfaceChecker[UI_BTN_ITEM].lTop + _UI_HEIGHT_;
	m_InterfaceChecker[UI_BTN_ITEM].pOvrProc					= OnItemBtnOvrProc;
	m_InterfaceChecker[UI_BTN_ITEM].pLDnProc					= OnItemBtnLDnProc;

	// Character Information Window Button
	m_InterfaceChecker[UI_BTN_CHARACTER].lLeft					= 164;
	m_InterfaceChecker[UI_BTN_CHARACTER].lTop					= 684;
	m_InterfaceChecker[UI_BTN_CHARACTER].lRight					= m_InterfaceChecker[UI_BTN_CHARACTER].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_BTN_CHARACTER].lBottom				= m_InterfaceChecker[UI_BTN_CHARACTER].lTop + _UI_HEIGHT_;
	m_InterfaceChecker[UI_BTN_CHARACTER].pOvrProc				= OnCharacterBtnOvrProc;
	m_InterfaceChecker[UI_BTN_CHARACTER].pLDnProc				= OnCharacterBtnLDnProc;

	// Skill Information Window Button
	m_InterfaceChecker[UI_BTN_SKILL].lLeft						= 243;
	m_InterfaceChecker[UI_BTN_SKILL].lTop						= 684;
	m_InterfaceChecker[UI_BTN_SKILL].lRight						= m_InterfaceChecker[UI_BTN_SKILL].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_BTN_SKILL].lBottom					= m_InterfaceChecker[UI_BTN_SKILL].lTop + _UI_HEIGHT_;
	m_InterfaceChecker[UI_BTN_SKILL].pOvrProc					= OnSkillBtnOvrProc;
	m_InterfaceChecker[UI_BTN_SKILL].pLDnProc					= OnSkillBtnLDnProc;

	// Group Window Button
	m_InterfaceChecker[UI_BTN_GROUP].lLeft						= 747;
	m_InterfaceChecker[UI_BTN_GROUP].lTop						= 684;
	m_InterfaceChecker[UI_BTN_GROUP].lRight						= m_InterfaceChecker[UI_BTN_GROUP].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_BTN_GROUP].lBottom					= m_InterfaceChecker[UI_BTN_GROUP].lTop + _UI_HEIGHT_;
	m_InterfaceChecker[UI_BTN_GROUP].pOvrProc					= OnGroupBtnOvrProc;
	m_InterfaceChecker[UI_BTN_GROUP].pLDnProc					= OnGroupBtnLDnProc;

	// Dungeon Info (Lair Dungeon) Window Button
	m_InterfaceChecker[UI_BTN_LAIR].lLeft						= 827;
	m_InterfaceChecker[UI_BTN_LAIR].lTop						= 684;
	m_InterfaceChecker[UI_BTN_LAIR].lRight						= m_InterfaceChecker[UI_BTN_LAIR].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_BTN_LAIR].lBottom						= m_InterfaceChecker[UI_BTN_LAIR].lTop + _UI_HEIGHT_;
	m_InterfaceChecker[UI_BTN_LAIR].pOvrProc					= OnLairBtnOvrProc;
	m_InterfaceChecker[UI_BTN_LAIR].pLDnProc					= OnLairBtnLDnProc;

	// Chatting Window ( Screen Bottom ) Button
	m_InterfaceChecker[UI_BTN_CHAT].lLeft						= 907;
	m_InterfaceChecker[UI_BTN_CHAT].lTop						= 684;
	m_InterfaceChecker[UI_BTN_CHAT].lRight						= m_InterfaceChecker[UI_BTN_CHAT].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_BTN_CHAT].lBottom						= m_InterfaceChecker[UI_BTN_CHAT].lTop + _UI_HEIGHT_;
	m_InterfaceChecker[UI_BTN_CHAT].pOvrProc					= OnChatBtnOvrProc;
	m_InterfaceChecker[UI_BTN_CHAT].pLDnProc					= OnChatBtnLDnProc;

	// Game Option Window Button
	m_InterfaceChecker[UI_BTN_OPTION].lLeft						= 659;
	m_InterfaceChecker[UI_BTN_OPTION].lTop						= 731;
	m_InterfaceChecker[UI_BTN_OPTION].lRight					= m_InterfaceChecker[UI_BTN_OPTION].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_BTN_OPTION].lBottom					= m_InterfaceChecker[UI_BTN_OPTION].lTop + 32;
	m_InterfaceChecker[UI_BTN_OPTION].pOvrProc					= OnOptionBtnOvrProc;
	m_InterfaceChecker[UI_BTN_OPTION].pLDnProc					= OnOptionBtnLDnProc;

	// L Hand Selected ( Left Bottom )
	m_InterfaceChecker[UI_L_HAND].lLeft							= 132;
	m_InterfaceChecker[UI_L_HAND].lTop							= 733;
	m_InterfaceChecker[UI_L_HAND].lRight						= m_InterfaceChecker[UI_L_HAND].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_L_HAND].lBottom						= m_InterfaceChecker[UI_L_HAND].lTop + 32;
	m_InterfaceChecker[UI_L_HAND].pOvrProc						= OnLHandAttackOvrProc;
	m_InterfaceChecker[UI_L_HAND].pLDnProc						= OnLHandAttackLDnProc;

	// R Hand Selected ( Left Bottom )
	m_InterfaceChecker[UI_R_HAND].lLeft							= 245;
	m_InterfaceChecker[UI_R_HAND].lTop							= 733;
	m_InterfaceChecker[UI_R_HAND].lRight						= m_InterfaceChecker[UI_R_HAND].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_R_HAND].lBottom						= m_InterfaceChecker[UI_R_HAND].lTop + 32;
	m_InterfaceChecker[UI_R_HAND].pOvrProc						= OnRHandAttackOvrProc;
	m_InterfaceChecker[UI_R_HAND].pLDnProc						= OnRHandAttackLDnProc;


	
	// L SKILL

	// R SKILL



	// PK Mode On / Off Button
	m_InterfaceChecker[UI_BTN_PK_MODE].lLeft					= 333;
	m_InterfaceChecker[UI_BTN_PK_MODE].lTop						= 731;
	m_InterfaceChecker[UI_BTN_PK_MODE].lRight					= m_InterfaceChecker[UI_BTN_PK_MODE].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_BTN_PK_MODE].lBottom					= m_InterfaceChecker[UI_BTN_PK_MODE].lTop + 32;
	m_InterfaceChecker[UI_BTN_PK_MODE].pOvrProc					= OnPKBtnOvrProc;
	m_InterfaceChecker[UI_BTN_PK_MODE].pLDnProc					= OnPKBtnLDnProc;

	// Belt ( Right Bottom )
	m_InterfaceChecker[UI_BELT].lLeft							= 727;
	m_InterfaceChecker[UI_BELT].lTop							= 733;
	m_InterfaceChecker[UI_BELT].lRight							= 1024;
	m_InterfaceChecker[UI_BELT].lBottom							= 768;
	m_InterfaceChecker[UI_BELT].pOvrProc						= OnBeltInvOvrProc;
	m_InterfaceChecker[UI_BELT].pLDnProc						= OnBeltInvLDnProc;

	// Guardian Inventory Window ( Left Top )
	m_InterfaceChecker[UI_GUARDIAN].lLeft						= 93;
	m_InterfaceChecker[UI_GUARDIAN].lTop						= 56;
	m_InterfaceChecker[UI_GUARDIAN].lRight						= m_InterfaceChecker[UI_GUARDIAN].lLeft + _UI_WIDTH_;
	m_InterfaceChecker[UI_GUARDIAN].lBottom						= 88;
	m_InterfaceChecker[UI_GUARDIAN].pOvrProc					= OnGuardianInvOvrProc;
	m_InterfaceChecker[UI_GUARDIAN].pLDnProc					= OnGuardianInvLDnProc;

	// Trade Button
	m_InterfaceChecker[UI_BTN_TRADE].lLeft						= 0;
	m_InterfaceChecker[UI_BTN_TRADE].lTop						= 661;
	m_InterfaceChecker[UI_BTN_TRADE].lRight						= 46;
	m_InterfaceChecker[UI_BTN_TRADE].lBottom					= 677;
	m_InterfaceChecker[UI_BTN_TRADE].pOvrProc					= OnTradeBtnOvrProc;
	m_InterfaceChecker[UI_BTN_TRADE].pLDnProc					= OnTradeBtnLDnProc;

	// Rest Button
	m_InterfaceChecker[UI_BTN_REST].lLeft						= 0;
	m_InterfaceChecker[UI_BTN_REST].lTop						= 679;
	m_InterfaceChecker[UI_BTN_REST].lRight						= 64;
	m_InterfaceChecker[UI_BTN_REST].lBottom						= 695;
	m_InterfaceChecker[UI_BTN_REST].pOvrProc					= OnRestBtnOvrProc;
	m_InterfaceChecker[UI_BTN_REST].pLDnProc					= OnRestBtnLDnProc;

	// Player Shop Button
	m_InterfaceChecker[UI_BTN_SHOP].lLeft						= 977;
	m_InterfaceChecker[UI_BTN_SHOP].lTop						= 661;
	m_InterfaceChecker[UI_BTN_SHOP].lRight						= 1024;
	m_InterfaceChecker[UI_BTN_SHOP].lBottom						= 677;
	m_InterfaceChecker[UI_BTN_SHOP].pOvrProc					= OnShopBtnOvrProc;
	m_InterfaceChecker[UI_BTN_SHOP].pLDnProc					= OnShopBtnLDnProc;

	// Match Button
	m_InterfaceChecker[UI_BTN_MATCH].lLeft						= 977;
	m_InterfaceChecker[UI_BTN_MATCH].lTop						= 679;
	m_InterfaceChecker[UI_BTN_MATCH].lRight						= 1024;
	m_InterfaceChecker[UI_BTN_MATCH].lBottom					= 695;
	m_InterfaceChecker[UI_BTN_MATCH].pOvrProc					= OnMatchBtnOvrProc;
	m_InterfaceChecker[UI_BTN_MATCH].pLDnProc					= OnMatchBtnLDnProc;

	// RIVAL GUILD WAR Button
	m_InterfaceChecker[UI_BTN_RIVAL_GUILD_WAR].lLeft			= 977;
	m_InterfaceChecker[UI_BTN_RIVAL_GUILD_WAR].lTop				= 634;
	m_InterfaceChecker[UI_BTN_RIVAL_GUILD_WAR].lRight			= 1024;
	m_InterfaceChecker[UI_BTN_RIVAL_GUILD_WAR].lBottom			= 659;
	m_InterfaceChecker[UI_BTN_RIVAL_GUILD_WAR].pOvrProc			= OnRivalGuildWarBtnOvrProc;
	m_InterfaceChecker[UI_BTN_RIVAL_GUILD_WAR].pLDnProc			= OnRivalGuildWarBtnLDnProc;

}

int CInterfaceManager::StupidInterfaceChecker()
{	
	for ( int nUICheckerCnt = 0; nUICheckerCnt < MAX_INTERFACE_CHECKER; nUICheckerCnt++ )
	{
		if (g_Mouse.MousePos.x >= m_InterfaceChecker[nUICheckerCnt].lLeft	&&
			g_Mouse.MousePos.x <= m_InterfaceChecker[nUICheckerCnt].lRight	&&
			g_Mouse.MousePos.y >= m_InterfaceChecker[nUICheckerCnt].lTop	&&
			g_Mouse.MousePos.y <= m_InterfaceChecker[nUICheckerCnt].lBottom)
		{
			return nUICheckerCnt;
			// 개선되면.. 자신에 해당하는 스프라이트 체인저와, 뻥션을 호출해줘야 한다..
		}
	}

	return UI_NOT_FOUND;
}

int CInterfaceManager::StupidInterfaceAreaCheck()
{
	#ifndef VER_RIVAL_GUILD_WAR
		const int RGW = 661;
	#else
		const int RGW = 631;
	#endif


	if( (g_Mouse.MousePos.x>=0   && g_Mouse.MousePos.x<=340  && g_Mouse.MousePos.y>=661 && g_Mouse.MousePos.y<=768)	||
		(g_Mouse.MousePos.x>=341 && g_Mouse.MousePos.x<=370  && g_Mouse.MousePos.y>=723 && g_Mouse.MousePos.y<=768)	||
		(g_Mouse.MousePos.x>=653 && g_Mouse.MousePos.x<=682  && g_Mouse.MousePos.y>=723 && g_Mouse.MousePos.y<=768)	||
		(g_Mouse.MousePos.x>=683 && g_Mouse.MousePos.x<=1024 && g_Mouse.MousePos.y>=RGW && g_Mouse.MousePos.y<=768) ||	//kjk
		(g_Mouse.MousePos.x>=93  && g_Mouse.MousePos.x<=125  && g_Mouse.MousePos.y>=76 && g_Mouse.MousePos.y<=108 ) )
	{
		return StupidInterfaceChecker();
	}

	return UI_NOT_APPLIED_AREA;
}

// Item Inventory Button
void CInterfaceManager::OnItemBtnOvrProc()
{

}

void CInterfaceManager::OnItemBtnLDnProc()
{

}

// Character Information Button
void CInterfaceManager::OnCharacterBtnOvrProc()
{

}

void CInterfaceManager::OnCharacterBtnLDnProc()
{

}

// Skill Window Button
void CInterfaceManager::OnSkillBtnOvrProc()
{

}

void CInterfaceManager::OnSkillBtnLDnProc()
{

}

// Group Window Button
void CInterfaceManager::OnGroupBtnOvrProc()
{

}

void CInterfaceManager::OnGroupBtnLDnProc()
{

}

// Dungeon Info (Lair Dungeon) Window Button
void CInterfaceManager::OnLairBtnOvrProc()
{

}

void CInterfaceManager::OnLairBtnLDnProc()
{

}

// Chatting Window ( Screen Bottom ) Button
void CInterfaceManager::OnChatBtnOvrProc()
{

}

void CInterfaceManager::OnChatBtnLDnProc()
{

}

// Game Option Window Button
void CInterfaceManager::OnOptionBtnOvrProc()
{

}

void CInterfaceManager::OnOptionBtnLDnProc()
{

}

// L Hand Selected ( Left Bottom )
void CInterfaceManager::OnLHandAttackOvrProc()
{

}

void CInterfaceManager::OnLHandAttackLDnProc()
{

}

// R Hand Selected ( Left Bottom )
void CInterfaceManager::OnRHandAttackOvrProc()
{

}

void CInterfaceManager::OnRHandAttackLDnProc()
{

}

// PK Mode On / Off Button
void CInterfaceManager::OnPKBtnOvrProc()
{

}

void CInterfaceManager::OnPKBtnLDnProc()
{

}

// Belt ( Right Bottom )
void CInterfaceManager::OnBeltInvOvrProc()
{

}

void CInterfaceManager::OnBeltInvLDnProc()
{

}

// Guardian Inventory Window ( Left Top )
void CInterfaceManager::OnGuardianInvOvrProc()
{

}

void CInterfaceManager::OnGuardianInvLDnProc()
{

}

// Trade Button
void CInterfaceManager::OnTradeBtnOvrProc()
{

}

void CInterfaceManager::OnTradeBtnLDnProc()
{

}

// Rest Button
void CInterfaceManager::OnRestBtnOvrProc()
{

}

void CInterfaceManager::OnRestBtnLDnProc()
{

}

// Player Shop Button
void CInterfaceManager::OnShopBtnOvrProc()
{

}

void CInterfaceManager::OnShopBtnLDnProc()
{

}

// Match Button
void CInterfaceManager::OnMatchBtnOvrProc()
{

}

void CInterfaceManager::OnMatchBtnLDnProc()
{

}

// RIVAL GUILD WAR Button
void CInterfaceManager::OnRivalGuildWarBtnOvrProc()
{
	
}

void CInterfaceManager::OnRivalGuildWarBtnLDnProc()
{
	
}