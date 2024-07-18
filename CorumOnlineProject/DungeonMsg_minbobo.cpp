#include "DungeonMsg_minbobo.h"
#include "InitGame.h"
#include "WorldProcess.h"
#include "DSMiniMap.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "UserInterface.h"
#include "message.h"
#include "DungeonTable.h"
#include "StoreWnd.h"
#include "ErrorWnd.h"
#include "Chat.h"
#include "CodeFun.h"
#include "Guild.h"
#include "Party.h"
#include "Trade.h"
#include "Shop.h"
#include "Messenger.h"
#include "BankWnd.h"
#include "CharWnd.h"
#include "GuardianWnd.h"
#include "CorumResource.h"
#include "HelpWnd.h"
#include "ItemWnd.h"
#include "SkillWnd.h"
#include "WorldMsg_minbobo.h"
#include "GameMenuWnd.h"
#include "DungeonMsg.h"
#include "MinboboWork.h"
#include "Filter.h"
#include "ItemNative.h"
#include "RivalGuildWar.h"


void InitPacketProcDungeonMinbobo()
{
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PORTAL_JOIN_USER]			= CmdPortalJoinUser;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PORTAL_MOVE_OUTSIDE]		= CmdPortalMoveOutside;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PORTAL_MOVE_OUTSIDE_FAIL]	= CmdPortalMoveOutsideFail;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_WEAPON_HAND]				= CmdWeaponHand;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_ARMOR_ITEM]					= CmdArmorItem;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_HELMET_ITEM]				= CmdHelmetItem;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUILD_RITEM]				= CmdGuildItem;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PARTY_PORTALFAIL]			= CmdPartyPotalFail;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PARTY_PORTALMOVE]			= CmdPartyPotalMove;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PORTAL_DISAPPEAR]			= CmdPortalDisappear;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PARTY_ARRAY]				= CmdPartyArray;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_SHOP_FAIL]					= CmdShopFail;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYERSHOP_ITEM]			= CmdPlayerShopItem;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYERSHOPITEM_RETURN_INV]	= CmdPlayerShopItem_ReturnInv;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYERSHOP_SELL]			= CmdPlayerShopSell;					
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYERSHOP_SOLDOUT]			= CmdPlayerShopSoldOut;					
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_USER_PORTAL]				= CmdUserPortal;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_SHOP_ITEM]					= CmdShopItem;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYER_REST]				= CmdPlayerRest;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUILD_RANK]					= CmdGuildUserRank;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PARTY_BOARD]				= CmdPartyBoard;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUILD_WAR_MESSAGE]			= CmdGuildWarMessage;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_USER_SPEED]					= CmdUserSpeed;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_OVERDRIVE_CHK]				= CmdOverDrive;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUILD_INFO]					= CmdGuildInfoRefresh;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_NOTICE]				= CmdDungeonNotice;			
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_REQUEST]				= CmdTradeRequest;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_RESULT]				= CmdTradeResult;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_SKILL_INFO]					= CmdSkillInfo;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_STATUS_INFO]				= CmdStatusInfo;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_BANK_INFO]					= CmdBankInfo;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PARTY]						= CmdParty;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_CANCEL]				= CmdTradeCancel;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_CANCEL_RETURN_INV]	= CmdTradeCancelReturnInv;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_OK]					= CmdTradeOk;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_ITEM]					= CmdTradeItem;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_TRADE]				= CmdTradeTrade;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_RETURN]				= CmdTradeRetrun;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_DATA]					= CmdTradeData;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYERSHOP_OPEN]			= CmdPlayerShopOpen;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYERSHOP_CLOSE]			= CmdPlayerShopClose;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PLAYERSHOP_SHOWALL]			= CmdPlayerShopShowAll;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUILD_FAIL]					= CmdGuildFail;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_UPGRADE_CHK]				= CmdUpgradeChk;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_TRADE_CANCELFAIL]			= CmdTradeCancelFail;	
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_STATUS_INIT]				= CmdStatusInit;		
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_SKILL_INIT]					= CmdSkillInit;							
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PARTY_MAGICARRAYFAIL]		= CmdPartyMagicArrayFail;							
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_COPY_ITEM_INSERTED]			= CmdCopyItemInserted;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_DUNGEON_CHAT]				= CmdDungeonChat;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUILD_FLAG]					= CmdGuildFlag;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_GUILDUSER_FLAG]				= CmdGuildUserFlag;
	PacketProc[UPDATE_GAME_PLAY][Protocol_DSTC::CMD_PARTY_CONFIG]				= CmdPartyConfig;
	// 파티 매칭 : 최덕석 2005.1.24
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PMATCH_REGISTER_RESULT]	= CmdPMatchRegisterResult;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PMATCH_ESCAPE_RESULT]		= CmdPMatchEscapeResult;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PMATCH_REQUEST]			= CmdPMatchRequest;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PMATCH_CHANGE_RESULT]		= CmdPMatchChangeResult;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_FRIEND]				= CmdPartyFriend;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GO_TO_WORLD]				= CmdGoToWorld;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GO_TO_WORLD_FAIL]			= CmdGoToWorldFail;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_COMMAND_DGFAIL]			= CmdDGFail;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_COMMAND_DGWHISPER]		= CmdDGWhisper;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_COMMAND_MESSAGE]			= CmdDGMessage;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_WSDGCHAT_USER]			= CmdDGChatUser;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_WSGUILD_CREATE]			= CmdGuildCreate;		
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_DGGUILD_USERINFO]			= CmdDgGuildUserInfo;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_DGGUILD_USER]				= CmdDgGuildUser;				
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_USERREQUESTWS]		= CmdPartyUserRequest;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_USERREQUEST_FAIL]	= CmdPartyUserRequestFail;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDUSER_INFORT]			= CmdGuildUserInfoRt;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_PORTALFAIL]			= CmdServerPartyPortalFail;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_REQUESTRT]			= CmdGuildRequest;		
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_USERREQUEST_FAIL]	= CmdGuildUserRequestFail;					
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_DATA_REQUESTRT]		= CmdGuildDataRequest;					
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_COUNT]				= CmdGuildCount;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_EXP]				= CmdGuildExp;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_LEVEL]				= CmdGuildLevel;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_RANK_FAIL]				= CmdGuildRankFail;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_USER_MASTER]				= CmdGuildUserMaster;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_USER_GUILDMASTERRT]		= CmdGuildUserGuildMaster;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_LIST]				= CmdGuildList;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_FAIL]				= CmdGuildError;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDWAR_MASTER]			= CmdGuildWarMaster;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDWAR_ERROR]			= CmdGuildWarError;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDWAR_JOIN]			= CmdGuildWarJoin;			
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDWAR_POINT]			= CmdGuildWarPoint;			
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDWAR_EXITRT]			= CmdGuildWarExit;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDWAR_RANKRT]			= CmdGuildWarRank;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_COMMAND_DGFAIL]			= CmdDGFail;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_COMMAND_DGWHISPER]		= CmdDGWhisper;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_COMMAND_MESSAGE]			= CmdDGMessage;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_WSDGCHAT_USER]			= CmdDGChatUser;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_WSGUILD_CREATE]			= CmdGuildCreate;		
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_DGGUILD_USERINFO]			= CmdDgGuildUserInfo;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_DGGUILD_USER]				= CmdDgGuildUser;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDJOINRECV_MESSAGE]	= CmdGuildJoinMessage;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_JOINRESULT]			= CmdGuildJoinResult;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_JOININFO]			= CmdGuildJoinInfo;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_RANKRT]				= CmdGuildRankRt;		
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_USER_DELETE]				= CmdUserDelete;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILD_DELETERT]			= CmdGuildDelete;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_JOINRT]				= CmdPartyJoin;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_JOINID]				= CmdPartyJoinId;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_LOGOUT]				= CmdPartyLogout;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_LEVEL]				= CmdPartyLevel;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_INFORT]				= CmdPartyInfo;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_LEADER]				= CmdPartyLeader;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_HP]					= CmdPartyHP;		
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_PORTAL]				= CmdPartyPotal;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_PORTALUSED]			= CmdPartyPotalUsed;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_PARTY_PORTALMOVE]			= CmdPartyPotalWorldMove;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_MESSENGER_USER]			= CmdMessengerUser;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_MESSENGER_USERINFO]		= CmdMessengerUserInfo;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_MESSENGER_USERFAIL]		= CmdMessengerUserFail;
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_MESSENGER_USERADDRT]		= CmdMessengerUserAdd;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_CHARACTER_SEARCHRT]		= CmdCharacterSearch;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_MESSENGER_FAIL]			= CmdMessengerFail;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_MESSENGER_SUCCESS]		= CmdMessengerSuccess;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_GUILDUSER_LEVEL]			= CmdGuildUserLevel;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_FRIENDUSER_INFORT]		= CmdFriendInfo;	
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_FRIENDUSER_DELETERT]		= CmdFriendDelete;		
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_MESSENGER_STATUSRT]		= CmdFriendStaus;	
	// sung-han 2005-02-05 길드전 관련
	PacketProc[UPDATE_GAME_WORLD][Protocol_World::CMD_TEAM_MATCH]				= Cmd_WSTC_RivalGuildWar;
}


void CmdCharacterSearch( char* pMsg, DWORD dwLen )
{
	WSTC_SEARCH* pPacket = (WSTC_SEARCH*)pMsg;

	char szInfo[0xff] = {0,};

	if(pPacket->byMsg==0)
	{
		if(pPacket->byType==0)	
		{
			// "%s는 World User Hash 존재."
			wsprintf(szInfo, g_Message[ETC_MESSAGE265].szMessage, pPacket->szName); 
		}
		else
		{
			// "%s는 Dugeon Id %u에 User Hash 존재."
			wsprintf(szInfo, g_Message[ETC_MESSAGE266].szMessage, pPacket->szName, pPacket->wDungeonID); 
		}

		DisplayMessageAdd(szInfo, 0xffff0000);
	}
}

void CmdUpgradeChk( char* pMsg, DWORD dwLen )
{
	DSTC_UPGRADE_CHK* pPacket = (DSTC_UPGRADE_CHK*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	
	if(pUser)
	{
		if(pUser->m_dwUserIndex==g_pMainPlayer->m_dwUserIndex)
			return;

		pUser->m_byUpgrade = pPacket->byUpgrade;

		if(pPacket->byUpgrade==0)
		{
			SetEffectIndexAll(pUser, 0xffffffff);
		}
		else
		{
			SetEffectIndexAll(pUser, pPacket->byUpgrade-6+1);
		}
	}
}

void CmdUserSpeed( char* pMsg, DWORD dwLen )
{
	DSTC_USER_SPEED* pPacket = (DSTC_USER_SPEED*)pMsg;

	if(g_pMainPlayer->m_dwUserIndex == pPacket->dwUserIndex)
	{
		g_pMainPlayer->m_fMoveSpeed = pPacket->fSpeed;
	}
	else
	{
		CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

		if(pUser)
		{
			pUser->m_fMoveSpeed = pPacket->fSpeed;
		}
	}
}


void CmdOverDrive( char* pMsg, DWORD dwLen )
{
	DSTC_OVERDRIVE_CHK* pPacket = (DSTC_OVERDRIVE_CHK*)pMsg;

	if(pPacket->byExc)
		memcpy(&g_pMainPlayer->m_pInv_Small[pPacket->byZipCode], &pPacket->cItem, sizeof(CItem));
}


void CmdPlayerRest( char* pMsg, DWORD dwLen )
{
	DSTC_PLAYER_REST* pPacket = (DSTC_PLAYER_REST*)pMsg;

	if(pPacket->dwUserIndex==g_pMainPlayer->m_dwUserIndex)
	{
		if(pPacket->byType==1)
		{
			g_pMainPlayer->SetActionNext(39, 40, ACTION_LOOP);
			g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc	= NULL;
			g_pMainPlayer->SetStatus(UNIT_STATUS_PLAYER_REST);
			SendStopPacket();						
		}
		else
		{
			g_pMainPlayer->SetAction(41, 0, ACTION_ONCE);
			g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc	= PlayerShopProc;	
		}
	}
	else
	{
		CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

		if(!pUser)
			return;
		
		if(pPacket->byType==1)
		{
			pUser->SetActionNext(39, 40, ACTION_LOOP);
			pUser->SetStatus(UNIT_STATUS_PLAYER_REST);
			pUser->m_hPlayer.pDesc->ObjectFunc	= NULL;
		}
		else
		{
			pUser->SetAction(41, 0, ACTION_ONCE);
			pUser->m_hPlayer.pDesc->ObjectFunc	= PlayerShopProc;
		}
	}	
}

void CmdUserPortal( char* pMsg, DWORD dwLen )
{
	// 이 메시지 자체가 GM_command 관련 메시지다 
	DSTC_USER_PORTAL* pPacket = (DSTC_USER_PORTAL*)pMsg;

	DUNGEON_MOVE_INFO &rSendDMI = pPacket->DMI;

	WORD wToDungeonID = rSendDMI.wDungeonID;	
				
	SendPacketRequestDungeonInfo(wToDungeonID);

	g_pMainPlayer->SetAction(36);
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
	
	SendStopPacket();

	CTDS_PORTAL_MOVE packet;	
	packet.wPortalItemID    = 0;
	packet.wToDungeonID		= rSendDMI.wDungeonID;					
	packet.bPortalType		= Portal_Type_GM;
	packet.bLayerNo			= rSendDMI.byLayerIndex;
	packet.v2Pos.x			= (float)rSendDMI.wX ;
	packet.v2Pos.y			= (float)rSendDMI.wY ;
	packet.dwTargetUserIndex = pPacket->dwTargetUserIndex;

	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
	g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
}


void CmdSkillInit( char* pMsg, DWORD dwLen )
{
	DSTC_SKILL_INIT* pPacket = (DSTC_SKILL_INIT*)pMsg;
	
	for(int i = 0; i < MAX_SKILL; i++)
		g_pMainPlayer->sSkillTable[i].bSKillLevel	= pPacket->bySkill[i];

	g_pMainPlayer->SetSkillChangeLR(__SKILL_ATTACK__, 0);
	g_pMainPlayer->SetSkillChangeLR(__SKILL_NONE_SELECT__, 1);
		
	g_pMainPlayer->m_wPointSkill	= pPacket->wPoint;

	CSkillWnd* pSkillWnd = CSkillWnd::GetInstance();
	
	if(pSkillWnd->GetActive())
		pSkillWnd->SetActive();

	// "초기화에 성공하였습니다."
	DisplayMessageAdd(g_Message[ETC_MESSAGE157].szMessage, 0xffff0000); 
}


void CmdStatusInit( char* pMsg, DWORD dwLen )
{
	DSTC_STATUS_INIT* pPacket = (DSTC_STATUS_INIT*)pMsg;
	
	g_pMainPlayer->m_wPoint	= pPacket->wPoint;

	// "초기화에 성공하였습니다."
	DisplayMessageAdd(g_Message[ETC_MESSAGE157].szMessage, 0xffff0000); 
}


void CmdWeaponHand(char* pMsg, DWORD dwLen)
{
	DSTC_WEAPON_HAND* pPacket = (DSTC_WEAPON_HAND*)pMsg;

	CUser*	pUser	= g_pUserHash->GetData(pPacket->dwUserIndex);
	
	if( !pUser ) 
	{
		RequestDungeonInfo( 1, pPacket->dwUserIndex );
		return;
	}

	if(pUser->m_dwUserIndex==g_pMainPlayer->m_dwUserIndex)
	{
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
		SendStopPacket();
		
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0;
	}

	pUser->m_wHandR = (WORD)pPacket->dwHand;
	pUser->m_wHandL = (WORD)pPacket->dwHand2;
	
	if(pPacket->dwHand==0)
	{
		SetLHandChange(pUser, pPacket->dwHand2);
		SetRHandChange(pUser, pPacket->dwHand);		
	}
	else
	{
		CBaseItem* pItemTable = g_pItemTableHash->GetData(pPacket->dwHand);	

		if(pItemTable)
		{		
			if(pItemTable->BaseItem_Weapon.bModCount==2)
				SetLHandChange(pUser, pPacket->dwHand);
			else		
				SetLHandChange(pUser, pPacket->dwHand2);

			SetRHandChange(pUser, pPacket->dwHand);
		}
	}
}


void CmdBankInfo(char* pMsg, DWORD dwLen)
{
	DSTC_BANK_INFO* pPacket		= (DSTC_BANK_INFO*)pMsg;

	g_pMainPlayer->m_dwBankMoney	= pPacket->dwMoney;

	memset(g_pMainPlayer->m_pBank_Large, 0, sizeof(g_pMainPlayer->m_pBank_Large));
	memset(g_pMainPlayer->m_pBank_Small, 0, sizeof(g_pMainPlayer->m_pBank_Small));

	ITEM_NODE_FOR_MSG* pNode = pPacket->Items;
	
	for(int i = 0; i < pPacket->bSmallItemCount; i++)
	{
		memcpy(&g_pMainPlayer->m_pBank_Small[pNode[i].index], &pNode[i].cItem, sizeof(CItem));
	}

	pNode = &pPacket->Items[pPacket->bSmallItemCount];
	
	for(i = 0; i < pPacket->bLargeItemCount; i++)
	{
		memcpy(&g_pMainPlayer->m_pBank_Large[pNode[i].index], &pNode[i].cItem, sizeof(CItem));
	}	
}


void CmdHelmetItem(char* pMsg, DWORD dwLen)
{
	DSTC_HELMET_ITEM*	pPacket = (DSTC_HELMET_ITEM*)pMsg;
	CUser*				pUser	= g_pUserHash->GetData(pPacket->dwUserIndex);	
	
	if(!pUser)
	{
		RequestDungeonInfo( 1, pPacket->dwUserIndex );
		return;
	}

	if(pUser->m_hPlayerHelmet.pHandle)
	{
		ItemDeleteObject(&pUser->m_hPlayerHelmet.pHandle, &pUser->m_hPlayer.pHandle);		
	}

	if(pPacket->dwHelmet!=0)
	{
		ItemAttach((WORD)pPacket->dwHelmet
			, &pUser->m_hPlayerHelmet.pHandle
			, NULL
			, &pUser->m_hPlayer.pHandle
			, ITEM_ATTACH_TYPE_HEAD);		
	}
}


void CmdArmorItem(char* pMsg, DWORD dwLen)
{
	DSTC_ARMOR_ITEM*	pPacket = (DSTC_ARMOR_ITEM*)pMsg;	
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if( !pUser )
	{
		RequestDungeonInfo( 1, pPacket->dwUserIndex );
		return;
	}

	if(pUser->m_dwUserIndex==g_pMainPlayer->m_dwUserIndex)
	{
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
		g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
		SendStopPacket();
		
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
		g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0;
	}	

	VECTOR3	vPos = {0.f,};
	if(pUser->m_hPlayer.pHandle)
	{
		g_pExecutive->GXOGetPosition(pUser->m_hPlayer.pHandle, &vPos);
		
		if(pUser->m_hPlayerRHand.pHandle)
			g_pExecutive->GXODetach(pUser->m_hPlayer.pHandle, pUser->m_hPlayerRHand.pHandle);
		if(pUser->m_hPlayerLHand.pHandle)
			g_pExecutive->GXODetach(pUser->m_hPlayer.pHandle, pUser->m_hPlayerLHand.pHandle);
		if(pUser->m_hPlayerHead.pHandle)
			g_pExecutive->GXODetach(pUser->m_hPlayer.pHandle, pUser->m_hPlayerHead.pHandle);
		if(pUser->m_hPlayerHelmet.pHandle)
			g_pExecutive->GXODetach(pUser->m_hPlayer.pHandle, pUser->m_hPlayerHelmet.pHandle);
 
		DeleteObj(&pUser->m_hPlayer.pHandle);
		
		if(pPacket->dwArmor == 0)
		{
			pUser->m_hPlayer.pHandle = CreateHandleObject( GetFile(RESTYPE_BASE_BODY, pUser->m_wClass)
			, GXPlayerPROC
			, pUser->m_hPlayer.pDesc
			, NULL);
		}
		else
		{
			LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(pPacket->dwArmor);

			if(lpItemResource)
			{
				if(lpItemResource->wModCount>0)
				{
					char szInfo[0xff] = {0,};

					ItemDataName(szInfo, (WORD)pPacket->dwArmor, pUser->m_wClass-1);
					pUser->m_hPlayer.pHandle = CreateHandleObject(	GetFile(szInfo, DATA_TYPE_CHARACTER)
						, GXPlayerPROC
						, pUser->m_hPlayer.pDesc
						, NULL);
				}
			}
		}

		if(pUser->m_hPlayerRHand.pHandle)		
			g_pExecutive->GXOAttach(pUser->m_hPlayerRHand.pHandle, pUser->m_hPlayer.pHandle, "Bip01 R Hand");
		if(pUser->m_hPlayerLHand.pHandle)
			g_pExecutive->GXOAttach(pUser->m_hPlayerLHand.pHandle, pUser->m_hPlayer.pHandle, "Bip01 L Hand");		
		if(pUser->m_hPlayerHead.pHandle)
			g_pExecutive->GXOAttach(pUser->m_hPlayerHead.pHandle, pUser->m_hPlayer.pHandle, "Bip01 Head");
		if(pUser->m_hPlayerHelmet.pHandle)
			g_pExecutive->GXOAttach(pUser->m_hPlayerHelmet.pHandle, pUser->m_hPlayer.pHandle, "Bip01 Head");			

		GXSetPosition(pUser->m_hPlayer.pHandle, &vPos, TRUE, TRUE);				
	}
}


void CmdStatusInfo(char* pMsg, DWORD dwLen)
{
	DSTC_STATUS_INFO* pPacket = (DSTC_STATUS_INFO*)pMsg;
	if(pPacket->bStatusInfo==TRUE)
	{
		CCharWnd* pCharWnd = CCharWnd::GetInstance();
		CGuardianWnd* pGuardianWnd = CGuardianWnd::GetInstance();
		
		switch(pPacket->byObjectType)
		{
		case OBJECT_TYPE_PLAYER:
			{			
				switch(pPacket->byStatusType)
				{
				case 0:
					g_pMainPlayer->m_dwEgo += 1;
					break;
				case 1:
					g_pMainPlayer->m_dwStr += 1;	
					break;
				case 2:
					g_pMainPlayer->m_dwInt += 1;	
					break;
				case 3:
					g_pMainPlayer->m_dwDex += 1;	
					break;
				case 4:
					g_pMainPlayer->m_dwVit += 1;
					break;
				}
				g_pMainPlayer->m_wPoint -= 1;
				
				if(g_pMainPlayer->m_wPoint==0)
					pCharWnd->HideAllStatButton();
			}break;
		
		case OBJECT_TYPE_MONSTER:
			{
				CMonster* pGuardian = pCharWnd->GetMyGuardian();
				if (!pGuardian)
					return;

				switch(pPacket->byStatusType)
				{
				case 0:
					pGuardian->m_wEgo += 1;
					break;
				case 1:
					pGuardian->m_wStr += 1;	
					break;
				case 2:
					pGuardian->m_wInt += 1;	
					break;
				case 3:
					pGuardian->m_wDex += 1;	
					break;
				case 4:
					pGuardian->m_wVit += 1;
					break;
				}
				pGuardian->m_wStatPoint -= 1;
				// 가디언 인터페이스 윈도우..
				if(pGuardian->m_wStatPoint==0)
				{
					pGuardianWnd->HideAllStatButton();
					//pCharWnd->HideAllStatButton();
				}
			}break;
		}
	}
	
	if(g_pMainPlayer->m_byHelp[1]<11)
	{
		CHelpWnd* pHelpWnd = CHelpWnd::GetInstance();		
		pHelpWnd->m_byType			= 11;
		g_pMainPlayer->m_byHelp[1]	= 11;
		pHelpWnd->SetClear();
		pHelpWnd->SetActive();

		WSTC_HELP_INFO pHelpInfoPacket;
		pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
		pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
		pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
		g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}
}


void CmdSkillInfo(char* pMsg, DWORD dwLen)
{
	DSTC_SKILL_INFO*	pPacket		= (DSTC_SKILL_INFO*)pMsg;
	CSkillWnd*			pSkillWnd	= CSkillWnd::GetInstance();
	
	if(pPacket->bSkillInfo==TRUE)
	{		
		g_pMainPlayer->m_wPointSkill -= 1;		
	}

	if(g_pMainPlayer->m_byHelp[1]==11)
	{
		CHelpWnd* pHelpWnd = CHelpWnd::GetInstance();		
		pHelpWnd->m_byType			= 12;
		g_pMainPlayer->m_byHelp[1]	= 12;
		pHelpWnd->SetClear();
		pHelpWnd->SetActive();

		WSTC_HELP_INFO pHelpInfoPacket;
		pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
		pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
		pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
		g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}

	if(pSkillWnd->GetActive()==TRUE)
	{
		pSkillWnd->SetSkillUpBtn();
		pSkillWnd->SetSkillButtonPos();
	}
}


void CmdCopyItemInserted( char* pMsg, DWORD dwLen )
{
	DSTC_COPY_ITEM_INSERTED *pPacket = (DSTC_COPY_ITEM_INSERTED*)pMsg;
	
	CBaseItem* lpItem = g_pItemTableHash->GetData(pPacket->dwCopyItemID);

	if(lpItem)
	{	
		char szInfo[0xff];
		// "%s 아이템은 복사된 아이템으로 삽입될 수 없습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE991].szMessage, lpItem->szItemName_Kor);	
		DisplayMessageAdd(szInfo, 0xFFFF2CFF);											
	}	
}

void CmdDungeonNotice( char* pMsg, DWORD dwLen )
{
	// 던젼 서버로부터 공지 메시지를 받았다.	
	CmdNotice(pMsg, dwLen);
}


void CmdPortalJoinUser( char* pMsg, DWORD dwLen )
{
	DSTC_PORTAL_JOIN_USER* pCopy = new DSTC_PORTAL_JOIN_USER;
	memcpy(pCopy, pMsg, dwLen);
	
	g_bRButton	= FALSE;	
	g_pMainPlayer->SetAction(MOTION_TYPE_PORTAL_DISAPPEAR, 0, ACTION_ONCE);
	g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
	g_pMainPlayer->m_hPlayer.pDesc->bSkip	= 0;
	g_pMainPlayer->m_bCurLayer				= pCopy->bCurLayerIndex;
	ToggleUserShadow(g_pMainPlayer, FALSE);

	// 포탈의 제어를 포탈 Effect가 끝나는 시점에 동작하게 한다. new는 추후 포탈 4가지 구현후 통합된 풀로 바꾸자
	ShowObject(g_pMainPlayer->m_hSelfPortalEffect.pHandle);
	GXSetPosition(g_pMainPlayer->m_hSelfPortalEffect.pHandle, &g_pMainPlayer->m_v3CurPos, FALSE, TRUE);
	
	g_pMainPlayer->m_hSelfPortalEffect.pDesc->pInfo			= (void*)g_pMainPlayer;
	g_pMainPlayer->m_hSelfPortalEffect.pDesc->dwTempBuf[0]	= (DWORD)pCopy;

	::SetAction(g_pMainPlayer->m_hSelfPortalEffect.pHandle, 1, 0, ACTION_LOOP);

	g_pMainPlayer->m_hSelfPortalEffect.pDesc->ObjectFunc	= PortalEffectFuncInsideMainPlayer;
	g_pMainPlayer->m_hSelfPortalEffect.pDesc->bSkip			= 0;
	
	// 월드서버에 최근 던전 정보 갱신 
	CTWS_REFRESH_RECENT_DUNGEON packet;
	packet.wDungeonID = pCopy->wDungeonID;
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD );	
}


void CmdPortalMoveOutside( char* pMsg, DWORD dwLen )
{
	DSTC_PORTAL_MOVE_OUTSIDE* pPacket = (DSTC_PORTAL_MOVE_OUTSIDE*)pMsg;
	
	if(pPacket->dwConnectIP == 0)
	{
		SPR(SPR_LOADING_BACK)->ShowSprite(FALSE);

		//메세지 
		CErrorWnd* pError = CErrorWnd::GetInstance();

		// "포탈이동에 실패했습니다.!"
		pError->SetError(g_Message[ETC_MESSAGE526].szMessage,"","", 1); 
		pError->SetActive();
		g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);

		return;
	}

	//포탈의 제어를 포탈 Effect가 끝나는 시점에 동작하게 한다. new는 추후 포탈 4가지 구현후 통합된 풀로 바꾸자
	DSTC_PORTAL_MOVE_OUTSIDE* pCopy = new DSTC_PORTAL_MOVE_OUTSIDE;
	memcpy(pCopy, pPacket, dwLen);
	GXSetPosition(g_pMainPlayer->m_hSelfPortalEffect.pHandle, &g_pMainPlayer->m_v3CurPos, FALSE, TRUE);
	
	g_pMainPlayer->m_hSelfPortalEffect.pDesc->pInfo = (void*)g_pMainPlayer;
	g_pMainPlayer->m_hSelfPortalEffect.pDesc->dwTempBuf[0] = (DWORD)pCopy;
	::SetAction(g_pMainPlayer->m_hSelfPortalEffect.pHandle, 1, 0, ACTION_ONCE);
	ShowObject(g_pMainPlayer->m_hSelfPortalEffect.pHandle);
	g_pMainPlayer->m_hSelfPortalEffect.pDesc->bSkip = 0;

	g_pMainPlayer->SetAction(37,0,ACTION_ONCE);
	g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
	g_pMainPlayer->m_hSelfPortalEffect.pDesc->ObjectFunc = PortalEffectFuncMainPlayer;
	g_pMainPlayer->m_hPlayer.pDesc->bSkip = 0;
	ToggleUserShadow(g_pMainPlayer, FALSE);

	// 월드서버에 최근 던전 정보 갱신 
	CTWS_REFRESH_RECENT_DUNGEON packet;
	packet.wDungeonID = pPacket->wToDungeonID;
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD );

	_PlaySound(0, SOUND_TYPE_ITEM, SOUND_ITEM_TELE_GATE_OPEN, g_Camera.v3CameraPos, FALSE);
}


void CmdPortalMoveOutsideFail( char* pMsg, DWORD dwLen )
{
	DSTC_PORTAL_MOVE_OUTSIDE_FAIL* pPacket = (DSTC_PORTAL_MOVE_OUTSIDE_FAIL*)pMsg;
	char szBuf[0xff];
	
	switch(pPacket->bErrorCode)
	{
	case 16:
		{
			// "이벤트 던젼이 목적지인 경우, 포탈기능을 이용할 수 없습니다."
			lstrcpy(szBuf, g_Message[ETC_MESSAGE254].szMessage);
		}
		break;
		
	case 15:
		{
			//"이벤트 던젼에서는 포탈기능을 이용할 수 없습니다."
			lstrcpy(szBuf, g_Message[ETC_MESSAGE524].szMessage);
		}
		break;
	case 5:
		{
			// "포탈 이동 중에는 사용할 수 없습니다."
			lstrcpy(szBuf, g_Message[ETC_MESSAGE502].szMessage);
		}
		break;
	case 6:
		{
			// "같은 던전 같은 층에서 포탈을 사용할 수 없습니다."
			lstrcpy(szBuf, g_Message[ETC_MESSAGE621].szMessage);
		}
		break;
	case 7:
		{
			// "죽었을때는 이동할 수 없습니다."
			lstrcpy(szBuf, g_Message[ETC_MESSAGE527].szMessage);
		}
		break;
	case 8:
		{
			//"이동하려는 던전이 공성전이 진행중 입니다. 공성중에는 이동할 수 없습니다."
			lstrcpy(szBuf, g_Message[ETC_MESSAGE528].szMessage);
		}
		break;
	case 9:
		{
			// "조건이 맞지않아 입장할 수 없습니다."
			// add minjin. 2004. 10. 04. with yakolla
			lstrcpy(szBuf, g_Message[ETC_MESSAGE1240].szMessage);
		}
		break;
	case 17:
		{
			// "입장료가 부족합니다."
			lstrcpy(szBuf, g_Message[ETC_MESSAGE799].szMessage);					
		}
		break;
	case 18:
		{
			// "이벤트 던전에 입구가 사라졌을때는 이동할수 없습니다."
			lstrcpy(szBuf,g_Message[ETC_MESSAGE1118].szMessage);
		}
		break;
	case 19:
		{
			// "입장객이 꽉 찼습니다."
			lstrcpy(szBuf,g_Message[ETC_MESSAGE1119].szMessage);
		}
		break;
	case 20:
		{
			// "이벤트 던젼이 닫혀 있습니다."
			lstrcpy(szBuf,g_Message[ETC_MESSAGE680].szMessage);
		}
		break;
	default:
		{
			// "포탈 이동에 실패했습니다. (에러코드:%d)"
			wsprintf(szBuf, g_Message[ETC_MESSAGE529].szMessage, pPacket->bErrorCode);
		}
		break;
	}
	
	DisplayMessageAdd(szBuf, 0xFF21DBFF);
	
	g_pMainPlayer->SetAction(MOTION_TYPE_DUNGEONSTAND);
	g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
	g_bRButton	= FALSE;
	g_bBeltChk	= FALSE;
	g_bGuildChk = FALSE;
	g_bMagicChk	= FALSE;
}


void CmdResponsePotalJoinUser( char* pMsg, DWORD dwLen )
{
	DSTC_RESPONSE_PORTAL_JOIN_USER* pPacket = (DSTC_RESPONSE_PORTAL_JOIN_USER*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if (!pUser)
	{
		RequestDungeonInfo( 1, pPacket->dwUserIndex );
		return;
	}

	VECTOR3 v3Start;
	VECTOR2_TO_VECTOR3(pPacket->vecStart, v3Start)

	if (pUser == g_pMainPlayer)
	{
		g_pMainPlayer->m_hSelfPortalEffect.pDesc->bSkip = 0;		
		
		SetUserPosition(g_pMainPlayer, &v3Start);
		SPR(SPR_LOADING_BACK)->ShowSprite(FALSE);
		
		for(int i=0; i<20; i++)
		{
			SPR(SPR_LOADING_BLOCK_1 + i)->ShowSprite(FALSE);
		}

		SPR(SPR_LOADING_BLOCK_HEAD)->ShowSprite(FALSE);		
	}
	
	ShowObject(pUser->m_hSelfPortalEffect.pHandle);
	::SetAction(pUser->m_hSelfPortalEffect.pHandle, 1, 0, ACTION_ONCE);
	GXSetPosition(pUser->m_hSelfPortalEffect.pHandle, &v3Start, FALSE, TRUE);
	
	pUser->m_hSelfPortalEffect.pDesc->pInfo			= (void*)pUser;
	pUser->SetAction(MOTION_TYPE_PORTAL_APPEAR, 0, ACTION_ONCE);
	pUser->m_hSelfPortalEffect.pDesc->ObjectFunc	= PortalAppearFuncUser;
	pUser->SetStatus(UNIT_STATUS_PORTAL_MOVING);
	pUser->m_hPlayer.pDesc->bSkip = FALSE;
}


void CmdGoToWorld( char* pMsg, DWORD dwLen )
{	
	WSTC_MOVE_TO_WORLD *pPacket = (WSTC_MOVE_TO_WORLD*)pMsg;
	
	g_ItemMoveManager.Initialize();
	g_helpLoadingSprite.Load(10000 + pPacket->bWorldMapID);
	g_helpLoadingSprite.SetLoadingFlag(TRUE);
	ShowLoading();
	
	// User Info Send //
	if(g_pMainPlayer)
	{
		CTWS_PARTYUSER_WORLDINFO pPartyPacket;
		pPartyPacket.byCurrentHand	= g_pMainPlayer->m_bCurrnetHand;
		pPartyPacket.wArmor			= g_pMainPlayer->m_pEquip[EQUIP_TYPE_ARMOR].GetID();		
		pPartyPacket.wHelmet		= g_pMainPlayer->m_pEquip[EQUIP_TYPE_HELMET].GetID();

		if(g_pMainPlayer->m_bCurrnetHand==1)
		{
			pPartyPacket.wHandL		= g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND1].GetID();
			pPartyPacket.wHandR		= g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].GetID();	
		}
		else
		{
			pPartyPacket.wHandL		= g_pMainPlayer->m_pEquip[EQUIP_TYPE_LHAND2].GetID();
			pPartyPacket.wHandR		= g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].GetID();	
		}

		g_pNet->SendMsg((char*)&pPartyPacket, pPartyPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}
	
	// 월드맵
	g_pMainPlayer->m_dwRecentOutMap = pPacket->bWorldMapID;
	// 월드맵에서의 주인공이 위치할 던전 번호 
	g_pMainPlayer->m_wContinueDungeonID = pPacket->wToDungeonID;
	g_pMainPlayer->m_bContinueWorld = 1;		

	VECTOR3 vWorldPos = {pPacket->fStartPos_X, 0.f, pPacket->fStartPos_Z};
	GXSetPosition(g_pMainPlayer->m_hPlayer.pHandle, &vWorldPos, FALSE, FALSE);
	CreateMainPlayerByChangeLayer();

	if(SetGameStatus(UPDATE_GAME_WORLD))
	{
		// 월드맵 이동에 성공했다는 Msg를 월드서버에 보냄 
		CTWS_GO_TO_WORLD_SUCCESS	packet;
		packet.bWorldMapId = pPacket->bWorldMapID;
		g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD );
	}		
}


void CmdGoToWorldFail( char* pMsg, DWORD dwLen )// : hwoarang 050202 
{
	WSTC_GO_TO_WORLD_FAIL *pPacket = (WSTC_GO_TO_WORLD_FAIL*)pMsg;

	CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();
	char szStr[0xff] = {0,};
	wsprintf(szStr, "(ErrorCode:%d)",pPacket->bErrorCode);

	switch(pPacket->bErrorCode )
	{
	case GO_TO_WORLD_ERROR_STADIUM:
		{
			pErrorWnd->m_bExit = FALSE;//경기장 이동에는 게임 종료 안한다
		}break;
	case GO_TO_WORLD_ERROR_DUNGEON_POS:
	case GO_TO_WORLD_ERROR_DUNGEON_NO:
	default:
		{
			pErrorWnd->m_bExit = TRUE;
		}break;
	}
	
	pErrorWnd->SetError(g_Message[ETC_MESSAGE530].szMessage, // "월드맵 이동에 실패했습니다."
		szStr, 
		g_Message[ETC_MESSAGE483].szMessage, // "운영팀에 문의하세요!"
		2);

	pErrorWnd->SetActive();
}

void CmdPortalDisappear( char* pMsg, DWORD dwLen )
{
	DSTC_PORTAL_DISAPPEAR* pPacket = (DSTC_PORTAL_DISAPPEAR*)pMsg;

	CUser*	pUser = g_pUserHash->GetData( pPacket->dwUserIndex );
	if( !pUser ) return;
	
	GXSetPosition(pUser->m_hSelfPortalEffect.pHandle, &pUser->m_v3CurPos, FALSE, TRUE);
	
	pUser->m_hSelfPortalEffect.pDesc->pInfo = (void*)pUser;

	ShowObject(pUser->m_hSelfPortalEffect.pHandle);
	SetAction(pUser->m_hSelfPortalEffect.pHandle, 1, 0, ACTION_ONCE);
	pUser->m_hSelfPortalEffect.pDesc->bSkip = 0;

	pUser->SetAction(MOTION_TYPE_PORTAL_DISAPPEAR, 0, ACTION_ONCE);
	pUser->m_hSelfPortalEffect.pDesc->ObjectFunc = PortalEffectFunc;
	pUser->m_hPlayer.pDesc->bSkip = 0;

	ToggleUserShadow(pUser, FALSE);
}

void CmdDungeonChat( char* pMsg, DWORD dwLen )
{
	DSTC_DUNGEON_CHAT *pPacket = (DSTC_DUNGEON_CHAT*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	if(!pUser)	
	{
		RequestDungeonInfo( 1, pPacket->dwUserIndex );
		return;
	}

	int		nSize	= 0;
	char*	szTemp = NULL;
	char	szChat[0xff] = {0,};
	char	szBuf[0xff] = {0,};
	
	memset(szChat, 0, sizeof(szChat));
	memcpy(szChat, pPacket->szChatStr, pPacket->bChatStrLength);

	// 채팅 메시지에 금지단어나 욕설이 있는 지 체크한다.
	if(ConvString(szChat)) return;

#if IS_CHINA_LOCALIZING()
	if(NoConvString(szChat)) return;
#endif

	wsprintf(szBuf, "[%s] %s", pUser->m_szName, szChat);	
	DisplayMessageAdd(szBuf, 0xFFFFFFFF);
	
	for(int i = 0; i < 3; i++)
		memset(&pUser->m_sDgChat[i], 0, sizeof(__tag_dg_chat__));

	pUser->m_byChatLine = 0;
	szTemp = szChat;
	
	nSize = lstrlen(szChat);

	while(nSize>0)
	{
		// sung-han 2005-03-30 말풍선 한줄당 길이 조정 22->26
		if(nSize>26)
		{
			for(;;)
			{
				if(nSize > 26)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+26, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev);

					__lstrcpyn(pUser->m_sDgChat[pUser->m_byChatLine].szChat, szTemp, nSize);
					pUser->m_sDgChat[pUser->m_byChatLine].nSize = nSize;

					szTemp = szPrev;
					pUser->m_byChatLine++;
					nSize = lstrlen(szTemp);
				}
				else
				{
					// 구한 문자열을 채팅 라인에 쓰고, 길이를 기록한다.
					lstrcpy(pUser->m_sDgChat[pUser->m_byChatLine].szChat, szTemp);
					pUser->m_sDgChat[pUser->m_byChatLine].nSize = lstrlen(szTemp);
					pUser->m_byChatLine++;								
					nSize = 0;
					break;
				}
			}
		}
		else
		{
			lstrcpy(pUser->m_sDgChat[pUser->m_byChatLine].szChat, szTemp);
			pUser->m_sDgChat[pUser->m_byChatLine].nSize = lstrlen(pUser->m_sDgChat[pUser->m_byChatLine].szChat);
			pUser->m_byChatLine++;
			nSize = 0;
		}		
	}	

	pUser->m_pBaloonChat.v2Scaling.x = 75;

	switch(pUser->m_byChatLine)
	{
	case 1:
		pUser->m_pBaloonChat.v2Scaling.y = 12;
		break;
	case 2:
		pUser->m_pBaloonChat.v2Scaling.y = 18;
		break;
	case 3:
		pUser->m_pBaloonChat.v2Scaling.y = 24;
		break;
	}

	pUser->m_dwChatTick = g_dwCurTick;
}
