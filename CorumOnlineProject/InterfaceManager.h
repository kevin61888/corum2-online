#include "windows.h"

#define MAX_INTERFACE_CHECKER	20

#define _UI_WIDTH_				32
#define _UI_HEIGHT_				16


class CInterfaceManager
{
private:
	static CInterfaceManager* m_pThis;

public:
	CInterfaceManager();
	virtual ~CInterfaceManager();

	static CInterfaceManager*	GetInstance();
	static void					DestroyInstance();

	enum EN_InterfaceManager
	{
		UI_NOT_APPLIED_AREA		= 0,
		UI_NOT_FOUND			= 1,
		UI_BTN_ITEM				= 2,
		UI_BTN_CHARACTER		= 3,
		UI_BTN_SKILL			= 4,
		UI_BTN_GROUP			= 5,
		UI_BTN_LAIR				= 6,
		UI_BTN_CHAT				= 7,
		UI_BTN_OPTION			= 8,
		UI_L_HAND				= 9,
		UI_R_HAND				= 10,
		UI_L_SKILL_SELECT		= 11,
		UI_R_SKILL_SELECT		= 12,
		UI_BTN_PK_MODE			= 13,
		UI_BELT					= 14,
		UI_GUARDIAN				= 15,
		UI_BTN_TRADE			= 16,
		UI_BTN_REST				= 17,
		UI_BTN_SHOP				= 18,
		UI_BTN_MATCH			= 19,
		UI_BTN_RIVAL_GUILD_WAR	= 20,
	};

	// Interface Checker
	struct __tag_Interface_Checker
	{

		LONG	lLeft;
		LONG	lRight;
		LONG	lTop;
		LONG	lBottom;

		VOID	(CInterfaceManager::*pOvrProc)();	// Mouse On Over
		VOID	(CInterfaceManager::*pLDnProc)();	// Mouse L Button Down
		VOID	(CInterfaceManager::*pLUpProc)();	// Mouse L Button Up
		VOID	(CInterfaceManager::*pRDnProc)();	// Mouse R Button Down
		VOID	(CInterfaceManager::*pRUpProc)();	// Mouse R Button Up

	} m_InterfaceChecker[MAX_INTERFACE_CHECKER];

	void InitInterfacePositionChecker();			// Initializing Interface Position Checker
	int  InterfaceCheckAndProc();					// Interface Checking and Processing

	int  StupidInterfaceChecker();
	int  StupidInterfaceAreaCheck();

	// On Over Proc
	void OnItemBtnOvrProc();						// Item Inventory Button
	void OnCharacterBtnOvrProc();					// Character Information Button
	void OnSkillBtnOvrProc();						// Skill Window Button
	void OnGroupBtnOvrProc();						// Group Window Button
	void OnLairBtnOvrProc();						// Dungeon Info (Lair Dungeon) Window Button
	void OnChatBtnOvrProc();						// Chatting Window ( Screen Bottom ) Button
	void OnOptionBtnOvrProc();						// Game Option Window Button
	void OnLHandAttackOvrProc();					// L Hand Selected ( Left Bottom )
	void OnRHandAttackOvrProc();					// R Hand Selected ( Left Bottom )
	void OnPKBtnOvrProc();							// PK Mode On / Off Button
	void OnBeltInvOvrProc();						// Belt ( Right Bottom )
	void OnGuardianInvOvrProc();					// Guardian Inventory Window ( Left Top )
	void OnTradeBtnOvrProc();						// Trade Button
	void OnRestBtnOvrProc();						// Rest Button
	void OnShopBtnOvrProc();						// Player Shop Button
	void OnMatchBtnOvrProc();						// Match Button
	void OnRivalGuildWarBtnOvrProc();				// RIVAL GUILD WAR Button

	// On L Button Down Proc
	void OnItemBtnLDnProc();						// Item Inventory Button
	void OnCharacterBtnLDnProc();					// Character Information Button
	void OnSkillBtnLDnProc();						// Skill Window Button
	void OnGroupBtnLDnProc();						// Group Window Button
	void OnLairBtnLDnProc();						// Dungeon Info (Lair Dungeon) Window Button
	void OnChatBtnLDnProc();						// Chatting Window ( Screen Bottom ) Button
	void OnOptionBtnLDnProc();						// Game Option Window Button
	void OnLHandAttackLDnProc();					// L Hand Selected ( Left Bottom )
	void OnRHandAttackLDnProc();					// R Hand Selected ( Left Bottom )
	void OnPKBtnLDnProc();							// PK Mode On / Off Button
	void OnBeltInvLDnProc();						// Belt ( Right Bottom )
	void OnGuardianInvLDnProc();					// Guardian Inventory Window ( Left Top )
	void OnTradeBtnLDnProc();						// Trade Button
	void OnRestBtnLDnProc();						// Rest Button
	void OnShopBtnLDnProc();						// Player Shop Button
	void OnMatchBtnLDnProc();						// Match Button
	void OnRivalGuildWarBtnLDnProc();				// RIVAL GUILD WAR Button


};