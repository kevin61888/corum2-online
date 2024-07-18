#include "stdafx.h"
#include "InterfaceInstance.h"
#include "Interface.h"
#include "Menu.h"
#include "ItemWnd.h"
#include "CharWnd.h"
#include "SkillWnd.h"
#include "GameMenuWnd.h"
#include "DSMiniMap.h"
#include "CommandWnd.h"
#include "ChatWnd.h"
#include "ChatListWnd.h"
#include "UserInterface.h"
#include "ItemShopWnd.h"
#include "NpcDungeonInfoWnd.h"
#include "QuantityWnd.h"
#include "ExitWnd.h"
#include "StoreWnd.h"
#include "cdungeonmanagerwnd.h"
#include "cdungeoninfownd.h"
#include "BankWnd.h"
#include "GuildWnd.h"
#include "GuildExWnd.h"
#include "GuildJoinWnd.h"
#include "GuildRankWnd.h"
#include "GuildEditWnd.h"
#include "PartyJoinWnd.h"
#include "DefaultWnd.h"
#include "HelpWnd.h"
#include "ErrorWnd.h"
#include "ItemTradeWnd.h"
#include "ItemTradeShopWnd.h"
#include "Interface.h"
#include "OkWnd.h"
#include "GroupWnd.h"
#include "playershopwnd.h"
#include "PartyPortalWnd.h"
#include "RequestMatchWnd.h"
#include "DungeonSiegeInfoWnd.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "ItemUpgradeHelpWnd.h"
#include "EventDungeonWnd.h"
#include "NoticeWnd.h"
#include "PartyRequestWnd.h"
#include "InitItemWnd.h"
#include "GuildRequestWnd.h"
#include "MessengerWnd.h"
#include "GuildDeleteWnd.h"
#include "GameDefaultWnd.h"
#include "GameNoticeWnd.h"
#include "CharMoveWnd.h"
#include "MessengerDeleteWnd.h"
#include "GuildMasterWnd.h"
#include "WorldWnd.h"
#include "CharDieWnd.h"
#include "GuildPortalWnd.h"
#include "GuildWarWnd.h"
#include "RankNpcWnd.h"
#include "RankWnd.h"
#include "EventNPCWnd.h"
#include "EventNPCSubWnd.h"
#include "CharMoveRtWnd.h"
#include "CharMoveEditWnd.h"
#include "PartyBoardWnd.h"
#include "PartyEntryWnd.h"
#include "PartyHelpWnd.h"
#include "PartyBoardInfoWnd.h"
#include "BlockWnd.h"
#include "ItemMakingWnd.h"
#include "ItemMakingSubWnd.h"
#include "ItemMakingHelpWnd.h"
#include "TopDungeonHelpWnd.h"
#include "GuardianWnd.h"
#include "PartyMatchingWnd.h"
#include "GuildWarRequest.h"
#include "GuildWarInvite.h"
#include "GuildWarFinalSettingWnd.h"
#include "GuildWarStatusWnd.h"
#include "LoginWnd.h"
#include "Develop.h"
#include <time.h>

void InitInterfaceInstances()
{
	#define CHECK_ALLOCATION(x) if(!x) __asm { int 3 };

#ifdef __RANDOM_MEMORY_ALLOCATION
	srand( (unsigned) time( NULL ) );	
	DWORD dwRandSize = rand() % 1024;
	char* pRandMemBlock = new char [dwRandSize*4];
#endif

	CHECK_ALLOCATION( CInterface::GetInstance()				);
	CHECK_ALLOCATION( CUserInterface::GetInstance()			);
	CHECK_ALLOCATION( CLoginWnd::GetInstance()				);
	CHECK_ALLOCATION( CCommandWnd::GetInstance()			);	
	CHECK_ALLOCATION( CItemWnd::GetInstance()				);
	CHECK_ALLOCATION( CCharWnd::GetInstance()				);
	CHECK_ALLOCATION( CSkillWnd::GetInstance()				);
	CHECK_ALLOCATION( CGameMenuWnd::GetInstance()			);		
	CHECK_ALLOCATION( CMiniMapWnd::GetInstance()			);
	CHECK_ALLOCATION( CChatWnd::GetInstance()				);		
	CHECK_ALLOCATION( CChatListWnd::GetInstance()			);
	CHECK_ALLOCATION( CItemShopWnd::GetInstance()			);
	CHECK_ALLOCATION( CNpcDungeonInfoWnd::GetInstance()		);
	CHECK_ALLOCATION( CQuantityWnd::GetInstance()			);
	CHECK_ALLOCATION( CExitWnd::GetInstance()				);
	CHECK_ALLOCATION( CStoreWnd::GetInstance()				);
	CHECK_ALLOCATION( CDungeonManagerWnd::GetInstance()		);
	CHECK_ALLOCATION( CDungeonInfoWnd::GetInstance()		);
	CHECK_ALLOCATION( CBankWnd::GetInstance()				);
	CHECK_ALLOCATION( CGuildWnd::GetInstance()				);
	CHECK_ALLOCATION( CGuildExWnd::GetInstance()			);
	CHECK_ALLOCATION( CGuildJoinWnd::GetInstance()			);
	CHECK_ALLOCATION( CGuildRankWnd::GetInstance()			);
	CHECK_ALLOCATION( CGuildEditWnd::GetInstance()			);
	CHECK_ALLOCATION( CPartyJoinWnd::GetInstance()			);
	CHECK_ALLOCATION( CDefaultWnd::GetInstance()			);
	CHECK_ALLOCATION( CHelpWnd::GetInstance()				);
	CHECK_ALLOCATION( CErrorWnd::GetInstance()				);
	CHECK_ALLOCATION( CItemTradeWnd::GetInstance()			);
	CHECK_ALLOCATION( CItemTradeShopWnd::GetInstance()		);
	CHECK_ALLOCATION( COkWnd::GetInstance()					);
	CHECK_ALLOCATION( CGroupWnd::GetInstance()				);
	CHECK_ALLOCATION( CPlayerShopWnd::GetInstance()			);
	CHECK_ALLOCATION( CPartyPortalWnd::GetInstance()		);
	CHECK_ALLOCATION( CRequestMatchWnd::GetInstance()		);
	CHECK_ALLOCATION( CDungeonSiegeInfoWnd::GetInstance()	);
	CHECK_ALLOCATION( CItemUpgradeWnd::GetInstance()		);
	CHECK_ALLOCATION( CEventDungeonWnd::GetInstance()		);
	CHECK_ALLOCATION( CNoticeWnd::GetInstance()				);
	CHECK_ALLOCATION( CPartyRequestWnd::GetInstance()		);
	CHECK_ALLOCATION( CInitItemWnd::GetInstance()			);
	CHECK_ALLOCATION( CGuildRequestWnd::GetInstance()		);
	CHECK_ALLOCATION( CMessengerWnd::GetInstance()			);
	CHECK_ALLOCATION( CGuildDeleteWnd::GetInstance()		);
	CHECK_ALLOCATION( CGameNoticeMessageWnd::GetInstance()	);
	CHECK_ALLOCATION( CMessengerDeleteWnd::GetInstance()	);
	CHECK_ALLOCATION( CGuildMasterWnd::GetInstance()		);
	CHECK_ALLOCATION( CWorldWnd::GetInstance()				);
	CHECK_ALLOCATION( CCharDieWnd::GetInstance()			);
	CHECK_ALLOCATION( CGuildPortalWnd::GetInstance()		);
	CHECK_ALLOCATION( CGuildWarWnd::GetInstance()			);
	CHECK_ALLOCATION( CRankNpcWnd::GetInstance()			);
	CHECK_ALLOCATION( CRankWnd::GetInstance()				);
	CHECK_ALLOCATION( CEventNPCWnd::GetInstance()			);
	CHECK_ALLOCATION( CEventNPCSubWnd::GetInstance()		);
	CHECK_ALLOCATION( CItemUpgradeSubWnd::GetInstance()		);
	CHECK_ALLOCATION( CItemUpgradeHelpWnd::GetInstance()	);
	CHECK_ALLOCATION( CCharMoveWnd::GetInstance()			);
	CHECK_ALLOCATION( CCharMoveRtWnd::GetInstance()			);
	CHECK_ALLOCATION( CCharMoveEditWnd::GetInstance()		);
	CHECK_ALLOCATION( CPartyBoardWnd::GetInstance()			);
	CHECK_ALLOCATION( CPartyEntryWnd::GetInstance()			);
	CHECK_ALLOCATION( CPartyHelpWnd::GetInstance()			);
	CHECK_ALLOCATION( CPartyBoardInfoWnd::GetInstance()		);
	CHECK_ALLOCATION( CBlockWnd::GetInstance()				);
	CHECK_ALLOCATION( CGameDefaultWnd::GetInstance()		);
	CHECK_ALLOCATION( CItemMakingWnd::GetInstance()			);
	CHECK_ALLOCATION( CItemMakingSubWnd::GetInstance()		);
	CHECK_ALLOCATION( CItemMakingHelpWnd::GetInstance()		);
	CHECK_ALLOCATION( CTopDungeonHelpWnd::GetInstance()		);

#ifdef __RANDOM_MEMORY_ALLOCATION
	delete [] pRandMemBlock;
#endif
}


void DestroyInterfaceInstances()
{
	CInterface::DestroyInstance();
	CLoginWnd::DestroyInstance();

	CCommandWnd::DestroyInstance();
	CUserInterface::DestroyInstance();
	CItemWnd::DestroyInstance();
	CCharWnd::DestroyInstance();
	CSkillWnd::DestroyInstance();
	CGameMenuWnd::DestroyInstance();		
	CMiniMapWnd::DestroyInstance();
	CChatWnd::DestroyInstance();		
	CChatListWnd::DestroyInstance();
	CItemShopWnd::DestroyInstance();
	CNpcDungeonInfoWnd::DestroyInstance();
	CQuantityWnd::DestroyInstance();
	CExitWnd::DestroyInstance();
	CStoreWnd::DestroyInstance();
	CDungeonManagerWnd::DestroyInstance();
	CDungeonInfoWnd::DestroyInstance();
	CBankWnd::DestroyInstance();
	CGuildWnd::DestroyInstance();
	CGuildExWnd::DestroyInstance();
	CGuildJoinWnd::DestroyInstance();
	CGuildRankWnd::DestroyInstance();
	CGuildEditWnd::DestroyInstance();
	CPartyJoinWnd::DestroyInstance();
	CDefaultWnd::DestroyInstance();
	CHelpWnd::DestroyInstance();
	CErrorWnd::DestroyInstance();
	CItemTradeWnd::DestroyInstance();
	CItemTradeShopWnd::DestroyInstance();
	COkWnd::DestroyInstance();
	CGroupWnd::DestroyInstance();
	CPlayerShopWnd::DestroyInstance();
	CPartyPortalWnd::DestroyInstance();
	CRequestMatchWnd::DestroyInstance();
	CDungeonSiegeInfoWnd::DestroyInstance();
	CItemUpgradeWnd::DestroyInstance();
	CEventDungeonWnd::DestroyInstance();
	CNoticeWnd::DestroyInstance();
	CPartyRequestWnd::DestroyInstance();
	CInitItemWnd::DestroyInstance();
	CGuildRequestWnd::DestroyInstance();
	CMessengerWnd::DestroyInstance();
	CGuildDeleteWnd::DestroyInstance();
	CGameNoticeMessageWnd::DestroyInstance();
	CMessengerDeleteWnd::DestroyInstance();
	CGuildMasterWnd::DestroyInstance();
	CWorldWnd::DestroyInstance();
	CCharDieWnd::DestroyInstance();
	CGuildPortalWnd::DestroyInstance();
	CGuildWarWnd::DestroyInstance();
	CRankNpcWnd::DestroyInstance();
	CRankWnd::DestroyInstance();
	CEventNPCWnd::DestroyInstance();
	CEventNPCSubWnd::DestroyInstance();
	CItemUpgradeSubWnd::DestroyInstance();
	CItemUpgradeHelpWnd::DestroyInstance();
	CCharMoveWnd::DestroyInstance();
	CCharMoveRtWnd::DestroyInstance();
	CCharMoveEditWnd::DestroyInstance();
	CPartyBoardWnd::DestroyInstance();
	CPartyEntryWnd::DestroyInstance();
	CPartyHelpWnd::DestroyInstance();
	CPartyBoardInfoWnd::DestroyInstance();
	CBlockWnd::DestroyInstance();
	CGameDefaultWnd::DestroyInstance();
	CItemMakingWnd::DestroyInstance();
	CItemMakingSubWnd::DestroyInstance();
	CItemMakingHelpWnd::DestroyInstance();
	CTopDungeonHelpWnd::DestroyInstance();
	CGuardianWnd::DestroyInstance();
	CPartyMatchingWnd::DestroyInstance();
	CGuildWarRequest::DestroyInstance();
	CGuildWarInvite::DestroyInstance();
	CGuildWarFinalSettingWnd::DestroyInstance();
	CGuildWarStatusWnd::DestroyInstance();
}

















