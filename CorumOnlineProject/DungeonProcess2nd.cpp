#include "stdafx.h"
#include "Define.h"
#include "InitGame.h"
#include "GameControl.h"
#include "DungeonProcess.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "GameControl.h"
#include "HashTable.h"
#include "EffectCommand.h"
#include "EffectProcess.h"
#include "ObjectManager.h"
#include "Monster.h"
#include "Npc.h"
#include "DungeonTable.h"
#include "WorldProcess.h"
#include "Interface.h"
#include "UserInterface.h"
#include "Script.h"
#include "Filter.h"
#include "ItemWnd.h"
#include "CharWnd.h"
#include "GameMenuWnd.h"
#include "SkillWnd.h"
#include "ChatWnd.h"
#include "CommandWnd.h"
#include "Struct.h"
#include "SkillWnd.h"
#include "ItemShopWnd.h"
#include "DSMiniMap.h"
#include "QuantityWnd.h"
#include "ExitWnd.h"
#include "StoreWnd.h"
#include "cdungeonmanagerwnd.h"
#include "cdungeoninfownd.h"
#include "BankWnd.h"
#include "GuildWnd.h"
#include "GuildExWnd.h"
#include "ChatWnd.h"
#include "GuildJoinWnd.h"
#include "GuildRankWnd.h"
#include "GMTool.h"
#include "GuildEditWnd.h"
#include "PartyJoinWnd.h"
#include "ComboBox.h"
#include "NoticeWnd.h"
#include "HelpWnd.h"
#include "ErrorWnd.h"
#include "ItemTradeWnd.h"
#include "ItemTradeShopWnd.h"
#include "message.h"
#include "OkWnd.h"
#include "GroupWnd.h"
#include "npcdungeoninfownd.h"
#include "playershopwnd.h"
#include "PartyPortalWnd.h"
#include "RequestMatchWnd.h"
#include "CodeFun.h"
#include "Party.h"
#include "Trade.h"
#include "Guild.h"
#include "Chat.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "DungeonSiegeInfoWnd.h"
#include "GameNoticeWnd.h"
#include "InitItemWnd.h"
#include "IMEEdit.h"
#include "Messenger.h"
#include "Shop.h"
#include "InputManager.h"
#include "WorldWnd.h"
#include "CharDieWnd.h"
#include "ChatBackground.h"
#include "PathFinding\SearchModule_Astar.h"
#include "NetworkClient.h"
#include "Map.h"
#include "Effect.h"
#include "DamageNumber.h"
#include "ChrInfo.h"
#include "RankWnd.h"
#include "RankNpcWnd.h"
#include "EventNPCWnd.h"
#include "EventNPCSubWnd.h"
#include "special_effect.h"
#include "PartyEntryWnd.h"
#include "PartyBoardWnd.h"
#include "ItemUsed.h"
#include "ItemMakingWnd.h"
#include "EventDungeonProcess.h"
#include "TopDungeonHelpWnd.h"
#include "MinboboWork.h"
#include "ItemNative.h"
#include "BaloonChat.h"				
#include "TokenCommand.h"			
#include "RivalGuildWar.h"	
#include "DungeonProcess2nd.h"
#include "GuildWarRequest.h"
#include "GuildWarFinalSettingWnd.h"
#include "GuildWarStatusWnd.h"


extern BOOL g_bPlayerShop;


void CheckAndProcForGroupWnd()
{
	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();

	if ( pGroupWnd->m_bPartyMove == TRUE )
	{
		pGroupWnd->SetStatus( GROUP_PARTY_CREATEADD );
	}
	if ( pGroupWnd->m_bGuildMove == TRUE )
	{
		pGroupWnd->SetStatus( GROUP_GUILD_DEFAULT );
	}
	if ( pGroupWnd->m_bMessengerMove == TRUE )
	{
		pGroupWnd->SetStatus( GROUP_FRIEND_DEFAULT );
	}
	if ( pGroupWnd->m_bGuildWarMove == TRUE )
	{
		pGroupWnd->SetStatus( GROUP_GUILDWAR_DEFAULT );
	}
}


void PickingObjectTypeMonster(LPObjectDesc pObjDesc)
{
	CMonster*		pMonster		= NULL;
	CUserInterface* pUserInterface	= CUserInterface::GetInstance();

	CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ATTACKCLICK__);

	pMonster = (CMonster*)pObjDesc->pInfo;
	
	if( g_bLCtrl )
	{
		// 만약 다른 몬스터라면 공격하라는 명령을 보낸다.
		CTDS_GUARDIAN_COMMAND_ATTACK	Attack;
		Attack.bAttackType				= 0;	// Monster...
		Attack.dwTargetIndex			= pMonster->m_dwMonsterIndex;

		if( FillSelectedMonster( Attack.pdwGuardianIndex, Attack.pdwMonsterIndex ) != 0 )
		{
			BOOL bAttack = TRUE;

			if( !g_pMainPlayer->IsAlliance(pMonster) )
			{
				if( g_pThisDungeon->IsStadium() )
				{
					bAttack = TRUE;
				}
				else
				{
					if(	pMonster->m_dwLordIndex 
					&&	!pUserInterface->m_nPK  )
					{
						bAttack = FALSE;
					}
				}

			}
			else
			{
				bAttack = FALSE;
			}

			if (bAttack)
			{
				g_pNet->SendMsg( (char*)&Attack, Attack.GetPacketSize(), SERVER_INDEX_ZONE );
			}
		}

		// 강제로 l버튼 플래그를 false한다. MouseEventDungeon에 진입못하게 하기 위해서.
		g_Mouse.bLDown = false;
	}
	
#if IS_KOREA_LOCALIZING() 
	// 일본어판의 경우 마우스질 해도 채팅 창 안없앤다..
	CChatWnd* pChatWnd = CChatWnd::GetInstance();

	if(pChatWnd->GetActive())
	{					
		if(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19))
		{
			if(!IsEmptyString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)))
			{
				// //
				memset(pChatWnd->m_szClip, 0, sizeof(pChatWnd->m_szClip));
				__lstrcpyn(pChatWnd->m_szClip, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)
					, lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)));
				pChatWnd->m_byClipType = 1;
				pChatWnd->SetActive(FALSE);
				
				ChatModeOff(INPUT_BUFFER_19, TRUE);															
				IMESetEdit(0);
			}
			else
			{
				if(g_pGVDungeon->bChatMode==TRUE)
				{
					pChatWnd->SetActive(FALSE);
					ChatModeOff(INPUT_BUFFER_19);
					IMESetEdit(0);
				}
			}
		}
	}
	
	CheckAndProcForGroupWnd();
#endif 
}


void PickingObjectTypePlayer(LPObjectDesc pObjDesc)
{
	CUser*			pUser			= (CUser*)pObjDesc->pInfo;
	CChatWnd*		pChatWnd		= CChatWnd::GetInstance();
	CGroupWnd*		pGroupWnd		= CGroupWnd::GetInstance();
	CUserInterface* pUserInterface	= CUserInterface::GetInstance();
	
	if ( pUser && !g_pMainPlayer->IsAlliance(pUser) && g_pThisDungeon && g_pThisDungeon->GetDungeonType() != DUNGEON_TYPE_VILLAGE )
	{
		// 때릴놈
		CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ATTACK__);
	}

	pChatWnd->SetName(pUser->m_szName, pChatWnd->GetActive());

	if(pChatWnd->GetActive() == TRUE)
	{
		if(pChatWnd->m_byChatType == CHAT_TYPE_WHISPER)
		{
			g_pInputManager->ClearInput(INPUT_BUFFER_19);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
			
			g_pInputManager->ClearInput(INPUT_BUFFER_6);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_6, pUser->m_szName, lstrlen(pUser->m_szName));
			g_pInputManager->GetInputBuffer(INPUT_BUFFER_19);
			
#ifdef _USE_IME	
			if(GET_IMEEDIT()->GetEditIndex() != 2)
			{
				POINT pChatPoint;
				pChatPoint.x	= 445;
				pChatPoint.y	= 737;

				GET_IMEEDIT()->SetEditIndex(2);				
				GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_6));	
				GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_IDBUUFER_SIZE__);
			}			
			else
			{
				GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_6));	
			}
#endif
			InputChatName(TRUE);
		}
	}
	
	if ( !pUser->IsSameUser(g_pMainPlayer) )
	{
		if(pGroupWnd->m_bPartyMove == TRUE)					
		{
			if(g_pMainPlayer->m_dwPartyId == 0)
			{
				CTWS_PARTY_JOIN pPacket;
				__lstrcpyn(pPacket.szName, pUser->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);			
				pPacket.bErrCode	= 0;
				pPacket.dwUserIndex	= pUser->m_dwUserIndex;
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				
				char szInfo[0xff] = {0,};
				// %s 님에게 파티 메세지 보냈습니다.
				wsprintf(szInfo, g_Message[ETC_MESSAGE387].szMessage, pUser->m_szName);	
				DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
			}
			else
			{
				if(pGroupWnd->m_bPartyMsg==FALSE)
				{
					CTWS_PARTY_JOIN pPacket;
					__lstrcpyn(pPacket.szName, pUser->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);			
					pPacket.bErrCode	= 0;
					pPacket.dwUserIndex	= pUser->m_dwUserIndex;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
					char szInfo[0xff] = {0,};
					// %s 님에게 파티 메세지 보냈습니다.
					wsprintf(szInfo, g_Message[ETC_MESSAGE387].szMessage, pUser->m_szName);	
					DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
				}
				else
				{
					BOOL bChk = FALSE;

					ListNode<PARTY_USER>* pNode = g_pPartyUserHash->GetHead();

					while(pNode)
					{
						LP_PARTY_USER pPartyUser = pNode->pData;
						
						if(pPartyUser->byLeader==1)
						{
							CTWS_PARTY_USERREQUEST pPartyPacket;
							pPartyPacket.dwUserIndex	= pUser->m_dwUserIndex;
							pPartyPacket.dwReqUserIndex	= g_pMainPlayer->m_dwUserIndex;
							pPartyPacket.dwPartyId		= g_pMainPlayer->m_dwPartyId;

							g_pNet->SendMsg((char*)&pPartyPacket, pPartyPacket.GetPacketSize(), SERVER_INDEX_WORLD);

							bChk = TRUE;
							break;
						}

						pNode = pNode->pNext;
					}

					char szInfo[0xff] = {0,};
					
					if(bChk)
					{
						// "파티 리더에게 %s님에 가입요청메세지를 보냅니다."
						wsprintf(szInfo, g_Message[ETC_MESSAGE723].szMessage, pUser->m_szName); 
					}
					else
					{
						// "파티 리더에게 %s님에 가입요청메세지를 보내지 못했습니다."
						wsprintf(szInfo, g_Message[ETC_MESSAGE724].szMessage, pUser->m_szName); 
					}
					
					DisplayMessageAdd(szInfo, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY);
				}
			}							
		}
		else if(pGroupWnd->m_bGuildMove == TRUE)
		{
			char szInfo[0xff] = {0,};

			if(pGroupWnd->m_bGuildMsg)
			{							
				CGuildRankWnd*	pGuildRankWnd	= CGuildRankWnd::GetInstance();
				CGuildExWnd*	pGuildExWnd		= CGuildExWnd::GetInstance();

				if(pGuildRankWnd->GetActive() == FALSE && pGuildExWnd->GetActive() == FALSE)
				{
					LP_GROUP_INFO lpGroupInfo = (LP_GROUP_INFO)g_pGroupInfoHash->GetData(g_pGuildInfo.wLevel);

					if(lpGroupInfo)
					{
						if(g_pGuildInfo.byCount < lpGroupInfo->wMemeber[0])
						{										
							// User Search //
							CTWS_GUILDJOIN_MESSAGE	pGuildJoin;
							memset(pGuildJoin.szGuildName, 0, sizeof(pGuildJoin.szGuildName));
							memset(pGuildJoin.szCharacterName, 0, sizeof(pGuildJoin.szCharacterName));
							pGuildJoin.dwUserIndex	= pUser->m_dwUserIndex;
							pGuildJoin.bType		= g_pMainPlayer->m_byType;
							__lstrcpyn(pGuildJoin.szGuildName, g_pMainPlayer->m_szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
							__lstrcpyn(pGuildJoin.szCharacterName, pUser->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
							g_pNet->SendMsg((char*)&pGuildJoin, pGuildJoin.GetPacketSize(), SERVER_INDEX_WORLD);
					
							if(g_pMainPlayer->m_byType==__GCTYPE_GUILD__)
							{
								// MSG_ID : 388 ; %s 님에게 길드 가입 메세지를 보냈습니다.
								wsprintf(szInfo, g_Message[ETC_MESSAGE388].szMessage, pUser->m_szName);	
							}
							else
							{
								// MSG_ID : 389 ; %s 님에게 클랜 가입 메세지를 보냈습니다.
								wsprintf(szInfo, g_Message[ETC_MESSAGE389].szMessage, pUser->m_szName);	
							}

							DisplayMessageAdd(szInfo, 0xFFFFFF00, TRUE, DEF_CHAT_TYPE_GUILD);										
						}
						else
						{
							// MSG_ID : 390 ; 더이상 가입을 받을수 없습니다.
							lstrcpy(szInfo, g_Message[ETC_MESSAGE390].szMessage);		
							DisplayMessageAdd(szInfo, 0xFFFFFF00, TRUE, DEF_CHAT_TYPE_GUILD);
							_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
						}
					}								
				}
				else
				{
					if(pGuildExWnd->m_byGuildType==__GCTYPE_GUILD__)
					{
						// MSG_ID : 391 ; 길드를 생성중입니다.
						lstrcpy(szInfo, g_Message[ETC_MESSAGE391].szMessage);	
					}
					else
					{
						// MSG_ID : 392 ; 클랜를 생성중입니다.	
						lstrcpy(szInfo, g_Message[ETC_MESSAGE392].szMessage);	
					}

					DisplayMessageAdd(szInfo, 0xFFFFFF00, TRUE, DEF_CHAT_TYPE_GUILD);
				}
			}
			else
			{		
				if(pUser->m_dwGuildId == 0)
				{
					LP_GROUP_INFO lpGroupInfo = (LP_GROUP_INFO)g_pGroupInfoHash->GetData(g_pGuildInfo.wLevel);

					if(lpGroupInfo)
					{
						if(g_pGuildInfo.byCount<lpGroupInfo->wMemeber[0])
						{
							CTWS_GUILD_REQUEST	pRequestGuild;
							pRequestGuild.dwGuildId				= g_pMainPlayer->m_dwGuildId;
							pRequestGuild.dwUserIndex			= pUser->m_dwUserIndex;
							pRequestGuild.dwRequestUserIndex	= g_pMainPlayer->m_dwUserIndex;
							g_pNet->SendMsg((char*)&pRequestGuild, pRequestGuild.GetPacketSize(), SERVER_INDEX_WORLD);
						}
						else
						{
							// MSG_ID : 390 ; 더이상 가입을 받을수 없습니다.
							lstrcpy(szInfo, g_Message[ETC_MESSAGE390].szMessage);		
							DisplayMessageAdd(szInfo, 0xFFFFFF00, TRUE, DEF_CHAT_TYPE_GUILD);
							_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
						}
					}
				}
				else
				{
					// "다른 길드에 가입되어 있습니다."
					lstrcpy(szInfo, g_Message[ETC_MESSAGE142].szMessage); 
					DisplayMessageAdd(szInfo, 0xFFFFFF00, TRUE, DEF_CHAT_TYPE_GUILD);
				}
			}								
			pGroupWnd->SetStatus(GROUP_GUILD_DEFAULT);		
		}
		else if(pGroupWnd->m_bMessengerMove==TRUE)
		{
			// 친구 등록 //
			if(g_pMessengerOnLineHash->GetCount()+g_pMessengerOffLineHash->GetCount()<MAX_MESSENGER_USER)
			{				
				LP_MESSENGER_USER lpMessengerUser = g_pMessengerOnLineHash->GetData(pUser->m_dwUserIndex);
				
				if(lpMessengerUser)	
				{
					// "이미 등록되어 있습니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE251].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND); 
				}
				else
				{
					CTWS_MESSENGER_USERADD	pMessengerPacket;							
					memset(pMessengerPacket.szCharacterName, 0, sizeof(pMessengerPacket.szCharacterName));
					pMessengerPacket.dwUserIndex	= pUser->m_dwUserIndex;
					__lstrcpyn(pMessengerPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					g_pNet->SendMsg((char*)&pMessengerPacket, pMessengerPacket.GetPacketSize(), SERVER_INDEX_WORLD);
					
					char szInfo[0xff] = {0,};
					wsprintf(szInfo, g_Message[ETC_MESSAGE830].szMessage, pUser->m_szName); // "%s 님에게 친구 등록 메세지를 보냈습니다."
					DisplayMessageAdd(szInfo, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);		// "이미 등록되어 있습니다."
				}														
			}
			else
			{
				// "더 이상 친구를 등록할 수 없습니다."					
				DisplayMessageAdd(g_Message[ETC_MESSAGE165].szMessage, 0xff92D5FF, TRUE, DEF_CHAT_TYPE_FRIEND);	
			}
		}
		else if(pGroupWnd->m_bGuildWarMove == TRUE)
		{
			// 길드전 신청 //
			BYTE bDisplayMessage = 0xff;
			
			__try
			{
				LP_GUILD_WAR lpGuildWar = g_pGuildWarHash->GetData(pUser->m_dwGuildId);
				BOOL bIsLiveGuildMaster = FALSE;

				if(g_pMainPlayer->m_dwGuildId == 0)
				{
					bDisplayMessage = 0; __leave;
				}
				if(pUser->m_dwGuildId == 0)
				{
					bDisplayMessage = 1; __leave;
				}
				if(g_pGuildWarHash->GetCount() >= MAX_GUILDWAR)
				{
					bDisplayMessage = 2; __leave;
				}
				if(lpGuildWar)
				{
					bDisplayMessage = 3; __leave;
				}
				if(g_pGuildInfo.wLevel < 3)
				{
					bDisplayMessage = 4; __leave;
				}
				if(g_pGuildUserHash->GetCount() < 1)
				{
					bDisplayMessage = 5; __leave;
				}
				if(g_pMainPlayer->m_dwGuildId == pUser->m_dwGuildId)
				{
					bDisplayMessage = 6; __leave;
				}

				if(g_pMainPlayer->m_byRank == 1)
					bIsLiveGuildMaster = TRUE;								
				else
				{
					ListNode<GUILD_USER>* lpGuildUserPos = g_pGuildUserHash->GetHead();

					while(lpGuildUserPos)
					{
						LP_GUILD_USER lpGuildUser = lpGuildUserPos->pData;

						if(lpGuildUser)
						{
							if(lpGuildUser->byRank == 1)
							{
								bIsLiveGuildMaster = TRUE;
								break;
							}
						}

						lpGuildUserPos = lpGuildUserPos->pNext;
					}
				}

				if(FALSE == bIsLiveGuildMaster)
				{
					bDisplayMessage = 7; __leave;
				}

				if(g_pMainPlayer->m_byRank != 1)
				{
					CTWS_GUILDWAR_REQUEST pPacket;
					pPacket.dwGuildId		= g_pMainPlayer->m_dwGuildId;
					pPacket.dwDstGuildId	= pUser->m_dwGuildId;
					pPacket.dwUserIndex		= g_pMainPlayer->m_dwUserIndex;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}	
				else
				{
 					CTWS_GUILDWAR_REQUESTRT pPacket;				
					pPacket.dwGuildId		= g_pMainPlayer->m_dwGuildId;
					pPacket.dwDstGuildId	= pUser->m_dwGuildId;
					pPacket.dwUserIndex		= g_pMainPlayer->m_dwUserIndex;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}
			}
			__finally
			{
				DWORD dwMessageID = 0;

				switch(bDisplayMessage)
				{
					case 0: // "가입된 길드가 없습니다."
						dwMessageID = ETC_MESSAGE67;	break;
					case 1: // "길드에 가입되어 있지않습니다."
						dwMessageID = ETC_MESSAGE877;	break;
					case 2: // "하나의 길드는 최대 8개의 다른 길드와 동시에 길드전이 가능합니다."
						dwMessageID = ETC_MESSAGE876;	break;
					case 3: // "현재 길드전이 진행되고 있습니다."
						dwMessageID = ETC_MESSAGE875;	break;
					case 4: // "길드 레벨 3이상이어야 길드전이 가능합니다."
						dwMessageID = ETC_MESSAGE874;	break;
					case 5: // "길드마스터를 제외한 길드원 최소 1명 이상 존재해야 합니다."
						dwMessageID = ETC_MESSAGE873;	break;
					case 6: // "같은 길드원끼리 길드전을 신청할 수 없습니다."
						dwMessageID = ETC_MESSAGE894;	break;
					case 7: // "길드 마스터가 게임상에 접속해 있지 않아 길드전을 시작할 수 없습니다."
						dwMessageID = ETC_MESSAGE872;	break;					
					default:							break;
				}

				if(bDisplayMessage != 0xff)
					DisplayMessageAdd(g_Message[dwMessageID].szMessage, 0xffffa2ff, TRUE, DEF_CHAT_TYPE_GUILD);
			}			
		}
		else if(pUserInterface->m_bTrade == TRUE)
		{
			CTDS_TRADE_REQUEST pPacket;
			memset(pPacket.szCharacterName, 0, sizeof(pPacket.szCharacterName));
			__lstrcpyn(pPacket.szCharacterName, pUser->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			pPacket.dwUserIndex	= pUser->m_dwUserIndex;
			pPacket.byCode		= 0;
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);												

			char szInfo[0xff] = {0,};
			// MSG_ID : 386 ; %s님에게 교환을 신청하였습니다.
			wsprintf(szInfo, g_Message[ETC_MESSAGE386].szMessage, pUser->m_szName);	
			DisplayMessageAdd(szInfo, 0xFFFFC309);						
		}
		else if(pUserInterface->m_bMatch == TRUE)
		{
			if (g_pThisDungeon->GetDungeonType() != DUNGEON_TYPE_VILLAGE)
			{
				// "대결은 마을에서만 신청 가능합니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE498].szMessage, 0xFFFF0000); 
				return;
			}

			if(g_pMainPlayer->GetStatus() == UNIT_STATUS_PLAYER_SHOP)
			{
				// "노점 중에는 대결을 할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE847].szMessage, 0xFFFF0000); 
				return;
			}

			if(g_pMainPlayer->GetStatus() == UNIT_STATUS_PLAYER_REST)
			{
				// "앉기 중에는 대결을 할 수 없습니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE846].szMessage, 0xFFFF0000); 
				return;
			}

			g_pMainPlayer->SetStatus(UNIT_STATUS_NORMAL);
			g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
			SendStopPacket();
			
			g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
			g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0;
			
			CTDS_REQUEST_MATCH_MAN2MAN	packet;
			packet.dwTargetUserIndex = pUser->m_dwUserIndex;					

			g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
			
			char szInfo[0xff] = {0,};
			// "%s 님에게 대결을 신청하였습니다."
			wsprintf(szInfo, g_Message[ETC_MESSAGE499].szMessage, pUser->m_szName); 
			DisplayMessageAdd(szInfo, 0xFFFFC309);

			pUserInterface->m_bMatch	= FALSE;
		}
		else if(pGroupWnd->m_bGuildWarChallenge)// sung-han 2005-01-30 길드전 관련
		{
			if(g_pMainPlayer->m_dwGuildId == pUser->m_dwGuildId)
			{
				DisplayMessageAdd(g_Message[ETC_MESSAGE1401].szMessage, TEXT_COLOR_YELLOW);// "같은 길드원에게 신청할 수 없습니다."
			}
			else if(pUser->m_dwGuildId == 0)
			{
				char szTemp[255] = {0,};
				wsprintf(szTemp, g_Message[ETC_MESSAGE1387].szMessage, pUser->m_szName);// "'%s'님은 소속된 길드가 없습니다."
				DisplayMessageAdd(szTemp, TEXT_COLOR_YELLOW);
			}
			else
			{
				CGuildWarRequest* pGuildWarRequest = CGuildWarRequest::GetInstance();
				TEAM_MATCH_PACKET TeamMatch;
				lstrcpy(TeamMatch.Body.GuildWarRequest.szCasterName, g_pMainPlayer->m_szName );
				lstrcpy(TeamMatch.Body.GuildWarRequest.szTargetName, pUser->m_szName);
				lstrcpy(pGuildWarRequest->m_szRequestName, pUser->m_szName);
				pGuildWarRequest->m_dwTargetGuildId = pUser->m_dwGuildId;
				TeamMatch.Body.GuildWarRequest.Sequence = GWRS_NONE;
				pGroupWnd->m_bGuildWarChallenge = FALSE;
				Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_REQUEST);
			}
		}
	}				

	if (pUser->m_bPlayerShop)
	{
		CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();

		if(g_pMainPlayer->GetStatus() == UNIT_STATUS_PLAYER_SHOP)	
		{
			// "노점 개설시에는 다른 사람에 노점에 대한 정보를 볼수 없습니다."								
			DisplayMessageAdd(g_Message[ETC_MESSAGE1005].szMessage, 0xFFFFC309); 
		}
		else if(pItemTradeShopWnd->GetActive())
		{
			//"교환중에는 다른 행동을 하실 수 없습니다."			
			DisplayMessageAdd(g_Message[ETC_MESSAGE1006].szMessage, 0xFFFF2CFF); 
		}
		else
		{
			// 노점상하는군?
			CTDS_PLAYERSHOP_SHOWALL packet;
			packet.dwUserIndex = pUser->m_dwUserIndex;
			g_pNet->SendMsg( (char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE );
			g_bPlayerShop = TRUE;
		}					
	}
	
	if( g_bLCtrl )
	{

		BOOL bChk = FALSE;

		ListNode<SGUILD_WAR>* lpGuildWarPos = g_pGuildWarHash->GetHead();

		while(lpGuildWarPos)
		{
			LP_GUILD_WAR lpGuildWar = lpGuildWarPos->pData;

			if(lpGuildWar)
			{
				if(lpGuildWar->dwGuildId == pUser->m_dwGuildId)
				{
					bChk = TRUE;
					break;
				}
			}

			lpGuildWarPos = lpGuildWarPos->pNext;
		}
			
		// 같은편이 아니어야 때릴수 있엉..
		if(g_pThisDungeon->IsStadium() )//길드전 맵이고 : hwoarang 050202
		{
			if (g_pMainPlayer->IsAlliance(pUser))		return;
		}
		else
		{
			if(!bChk && !g_pThisDungeon->IsVillage())					
			{
				if( !CUserInterface::GetInstance()->m_nPK )	return;
				if (g_pMainPlayer->IsAlliance(pUser))		return;
			}
		}
												
		// 만약 다른 몬스터라면 공격하라는 명령을 보낸다.
		CTDS_GUARDIAN_COMMAND_ATTACK	Attack;
		Attack.bAttackType				= 1;	// Monster...
		Attack.dwTargetIndex			= pUser->m_dwUserIndex;
		if( FillSelectedMonster( Attack.pdwGuardianIndex, Attack.pdwMonsterIndex ) != 0 )
		{
			g_pNet->SendMsg( (char*)&Attack, Attack.GetPacketSize(), SERVER_INDEX_ZONE );
		}
		
		// 강제로 l버튼 플래그를 false한다. MouseEventDungeon에 진입못하게 하기 위해서.
		g_Mouse.bLDown = false;
	}				

	CheckAndProcForGroupWnd();
}


void PickingObjectTypeTrader_Monster(LPObjectDesc pObjDesc)
{
	CUserInterface* pUserInterface	= CUserInterface::GetInstance();
	CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_NPCCLICK__);

	char*	szTemp			= NULL;	
	char	szInfo[0xff]	= {0,};	
	int		nSize			= 0;
				
	VECTOR3			v3Npc;
	CItemShopWnd*	pItemShopWnd	= CItemShopWnd::GetInstance();					
	CMonster*		pNpcMonster		= (CMonster*)pObjDesc->pInfo;				

	// Npc의 위치를 판단하여 위치가 너무 멀면 그냘 이동만한다.
	g_pExecutive->GXOGetPosition( g_hHandle, &v3Npc );
	g_fDist = CalcDistance( &g_pMainPlayer->m_v3CurPos, &v3Npc );

	if(g_fDist >= 650)
		goto lb_move;

#if IS_JAPAN_LOCALIZING()
	// 레드1일때 마을 npc와 이야기 할수 없어.
	else if (g_pMainPlayer->m_wCriminalTime <= CRIMINAL_LEVEL_1)
	{
		// "당신의 성향지수가 나빠 대화를 거절하였습니다..."
		DisplayMessageAdd(g_Message[ETC_MESSAGE898].szMessage, 0xFFFF0000);
		return;
	}
#endif	
	
	if(pItemShopWnd->GetActive()==FALSE)
	{											
		if(pItemShopWnd->GetActive()==FALSE)
		{
			switch(g_NPCTable[pNpcMonster->m_bClass].dwType)
			{
			case __NPC_VILL_GUILD__:
				{
					CRankWnd*		pRankWnd	= CRankWnd::GetInstance();
					CRankNpcWnd*	pRankNpcWnd = CRankNpcWnd::GetInstance();

					if(pRankWnd->GetActive()==FALSE && pRankNpcWnd->GetActive()==FALSE)
					{
						CTWS_GUILDWAR_RANK pPacket;
						pPacket.dwGuildId = g_pMainPlayer->m_dwGuildId;
						g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);							
					}
				}
				break;
			case __NPC_VILL_EVENT__:
				{
					// 이벤트 던젼으로 이동시켜주는 NPC에게 말을 걸었음.							
					CEventNPCWnd*		pEventNpcWnd	= CEventNPCWnd::GetInstance();
					CEventNPCSubWnd*	pEventNpcSubWnd	= CEventNPCSubWnd::GetInstance();

					if (FALSE == pEventNpcSubWnd->GetActive())
					{
						pEventNpcWnd->SetActive(TRUE);
						//pEventNpcSubWnd->SetActive(TRUE);
					}
				}
				break;
			case __NPC_PARTY_BOARD__:
				{
					CPartyBoardWnd* pPartyBoardWnd	= CPartyBoardWnd::GetInstance();							

					int nNum = rand()%3;

					if(IsEmptyString(g_NPCTable[pNpcMonster->m_bClass].szMsg[nNum]))
						nNum = 0;

					wsprintf(szInfo, "%s : %s", g_NPCTable[pNpcMonster->m_bClass].szName, g_NPCTable[pNpcMonster->m_bClass].szMsg[nNum]);

					pPartyBoardWnd->SetText(szInfo);
					pPartyBoardWnd->SetActive();
				}
				break;
			case __NPC_WORLD_MOVE1__:
				{
					CTopDungeonHelpWnd::GetInstance()->m_byWorldNum = 1;
					CTopDungeonHelpWnd::GetInstance()->SetActive();
				}
				break;
			case __NPC_WORLD_MOVE2__:
				{
					CTopDungeonHelpWnd::GetInstance()->m_byWorldNum = 2;
					CTopDungeonHelpWnd::GetInstance()->SetActive();
				}
				break;
			default:
				{
					pItemShopWnd->m_byTypeRt =  (BYTE)g_NPCTable[pNpcMonster->m_bClass].dwType;
					pItemShopWnd->SetPosSpr();
					pItemShopWnd->SetActive(TRUE);
					
					CStoreWnd* pStoreWnd		= CStoreWnd::GetInstance();
					pStoreWnd->m_dwMonsterIndex = pNpcMonster->m_dwMonsterIndex;
					
					CItemUpgradeSubWnd* pItemUpgradeSubWnd = CItemUpgradeSubWnd::GetInstance();
					pItemUpgradeSubWnd->m_dwNpcID = g_NPCTable[pNpcMonster->m_bClass].dwID;
				}							
			}						
		}	
		
		if(	g_NPCTable[pNpcMonster->m_bClass].dwType != __NPC_VILL_GUILD__ &&
			g_NPCTable[pNpcMonster->m_bClass].dwType != __NPC_PARTY_BOARD__)
		{
			pItemShopWnd->m_byChatLine	= 0;	

			int nNum = rand()%3;

			if(IsEmptyString(g_NPCTable[pNpcMonster->m_bClass].szMsg[nNum]))
				nNum = 0;

			wsprintf(szInfo,"%s : %s", g_NPCTable[pNpcMonster->m_bClass].szName, g_NPCTable[pNpcMonster->m_bClass].szMsg[nNum]);
			szTemp	= szInfo;	
			nSize	= lstrlen(szInfo);

			for(int i = 0; i < 5; i++)
			{
				pItemShopWnd->m_sDgChat[i].nSize = 0;						
			}

			while(nSize>0)
			{
				if(nSize>30)
				{
					for(;;)
					{
						if(nSize > 34)
						{
							char* szPrev = CharPrevExA(0, szTemp, szTemp+34, 0);
							nSize = lstrlen(szTemp) - lstrlen(szPrev); 

							__lstrcpyn(pItemShopWnd->m_sDgChat[pItemShopWnd->m_byChatLine].szChat, szTemp, nSize);
							pItemShopWnd->m_sDgChat[pItemShopWnd->m_byChatLine].nSize = nSize;

							szTemp = szPrev; 
							pItemShopWnd->m_byChatLine++;
							nSize = lstrlen(szTemp);
						}
						else
						{
							// 구한 문자열을 채팅 라인에 쓰고, 길이를 기록한다.
							lstrcpy(pItemShopWnd->m_sDgChat[pItemShopWnd->m_byChatLine].szChat, szTemp);
							pItemShopWnd->m_sDgChat[pItemShopWnd->m_byChatLine].nSize = lstrlen(szTemp);
							pItemShopWnd->m_byChatLine++;								
							nSize = 0;
							break;
						}
					}
				}
				else
				{
					lstrcpy(pItemShopWnd->m_sDgChat[pItemShopWnd->m_byChatLine].szChat, szTemp);
					pItemShopWnd->m_sDgChat[pItemShopWnd->m_byChatLine].nSize = lstrlen(pItemShopWnd->m_sDgChat[pItemShopWnd->m_byChatLine].szChat);
					pItemShopWnd->m_byChatLine++;
					nSize = 0;							
				}		
			}										
								
			if(	pItemShopWnd->m_byType==__NPC_VILL_SUPPLIES__	||
				pItemShopWnd->m_byType==__NPC_VILL_WEAPON__		||
				pItemShopWnd->m_byType==__NPC_VILL_ARMOR__)
			{
				if(g_pMainPlayer->m_byHelp[0] < 5)
				{
					CHelpWnd* pHelpWnd = CHelpWnd::GetInstance();
					pHelpWnd->m_byType = 5;							
					pHelpWnd->SetClear();
					pHelpWnd->SetActive();
					g_pMainPlayer->m_byHelp[0] = 5;
				
					WSTC_HELP_INFO pHelpInfoPacket;
					pHelpInfoPacket.bHelpInfo[0] = g_pMainPlayer->m_byHelp[0];
					pHelpInfoPacket.bHelpInfo[1] = g_pMainPlayer->m_byHelp[1];
					pHelpInfoPacket.bHelpInfo[2] = g_pMainPlayer->m_byHelp[2];
					g_pNet->SendMsg((char*)&pHelpInfoPacket, pHelpInfoPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}	
			}		
		}												
	}

	pUserInterface->m_bMatch = FALSE;	
	
	CheckAndProcForGroupWnd();
	return;

lb_move:
	DungeonMove();
}


void PickingObjectTypeItem(LPObjectDesc pObjDesc)
{
	CUserInterface*		pUserInterface		= CUserInterface::GetInstance();
	CQuantityWnd*		pQuantityWnd		= CQuantityWnd::GetInstance();
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	
	if(pQuantityWnd->GetActive())
		return;

	if(	g_pMainPlayer->GetStatus()!=UNIT_STATUS_NORMAL &&
		g_pMainPlayer->GetStatus()!=UNIT_STATUS_WALKING &&
		g_pMainPlayer->GetStatus()!=UNIT_STATUS_RUNNING)
		return;				

	if(pItemTradeShopWnd->GetActive()==TRUE)
		return;

	ITEM* pItem = (ITEM*)pObjDesc->pInfo;

	if(!pItem)
		return;

	g_fDist = CalcDistance( &g_pMainPlayer->m_v3CurPos, &pItem->v3ItemPos );
	
	if( g_fDist >= 185.0f ) 
	{
		// 여기서 이동후에 시행할 아이템 번호를 입력한다.
		g_pMainPlayer->m_i64PickupItem	= pItem->Item.m_Serial.i64Serial;
		goto lb_move;
	}

	// 대만은 무게 포화상태가 되면 아이템을 집을수 없게 막는다.
#if IS_TAIWAN_LOCALIZING()
	if ((g_pMainPlayer->GetAverageWeight()) >= 100.f )
	{
		//"포화무게한도에 도달하여 달리기가 안되며, 포션의 사용 딜레이가 증가합니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE986].szMessage, 0xffff0000);			
		return;
	}
#endif				
	if(g_ItemMoveManager.GetNewItemMoveMode())
	{
		g_ItemMoveManager.TileToInvPickupItem(pItem);
	}
	else
	{
		SendPickupItem(pItem, CItemWnd::GetInstance()->GetActive());
	}				

	if(g_pMainPlayer->m_dwItemInfoType == 2)
	{
		if(pItem->Item.GetSerial()->i64Serial == g_pMainPlayer->m_InfoItem.GetSerial()->i64Serial)
		{
			CUserInterface* pUserInterface = CUserInterface::GetInstance();
			pUserInterface->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);
			memset(&g_pMainPlayer->m_InfoItem, 0, sizeof( CItem ) );
			g_pMainPlayer->m_dwItemInfoType = 0;						
		}
	}
	
	pUserInterface->m_bMatch = FALSE;	
	
	CheckAndProcForGroupWnd();
	return;
	
lb_move:
	DungeonMove();
}


void DungeonMove()
{
	if( g_pThisDungeon->IsStadium() // : hwoarang 050222 
	&&	(	TRUE == CGuildWarFinalSettingWnd::GetInstance()->GetActive()
		||	TRUE == CGuildWarStatusWnd::GetInstance()->IsPrepareTime() ) )
	{
		//경기장이고 프리페어 타임이거나 경기장 소환 인터페이스 떠있으면 무브 못함
		return;
	}
	else if( (g_pMainPlayer->m_bMatching == TRUE && !CUserInterface::GetInstance()->IsAllUserPVPReady()) ) 
	{
		return;
	}
	CUserInterface* pUserInterface	= CUserInterface::GetInstance();

	// sung-han 2005-02-13 길드전 관련
	CGroupWnd* pGroupWnd = CGroupWnd::GetInstance();
	if(pGroupWnd->m_bGuildWarChallenge)
	{
		pGroupWnd->m_bGuildWarChallenge = FALSE;
	}

	pUserInterface->m_bMatch = FALSE;
	pUserInterface->m_bTrade = FALSE;
	
	g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_TYPE ]	= 0;
	g_pMainPlayer->m_dwTemp[ USER_TEMP_AUTO_TARGET_INDEX ]	= 0;

	if( g_pMainPlayer->GetStatus() == UNIT_STATUS_NORMAL || 
		g_pMainPlayer->GetStatus() == UNIT_STATUS_WALKING ||
		g_pMainPlayer->GetStatus() == UNIT_STATUS_RUNNING )	
	{
		// 움직일수 있는 상태라면.
		GXSetPosition(CUserInterface::GetInstance()->m_pUserClickHandle.pHandle, &g_Mouse.v3Mouse, FALSE);
		SetAction(CUserInterface::GetInstance()->m_pUserClickHandle.pHandle, 1, ACTION_ONCE);
		ShowObject(CUserInterface::GetInstance()->m_pUserClickHandle.pHandle);

		if( PathFindDungeon() )
		{
			#ifdef GM_TOOL
				if(g_pMainPlayer->IsGMUser() && g_pGVGm->bQuickMove)
				{
					g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
					g_pMainPlayer->m_pDestTile = g_pMap->GetMap( g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_X,
													   g_PathFinder.pLocation[g_PathFinder.dwCurCurve].dwBlock_Y );

					CTDS_GM_QUICK_MOVE	packet;
					VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3CurPos, packet.v2MoveStartPos)
					VECTOR3_TO_VECTOR2(g_pMainPlayer->m_v3Direction, packet.v2MoveDirection)
					packet.wDestX				= g_pMainPlayer->m_pDestTile->wIndex_X;
					packet.wDestZ				= g_pMainPlayer->m_pDestTile->wIndex_Z;
					g_pNet->SendMsg( (char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE );
					
				}
				else
					SetPathFindMove();
			#else
				SetPathFindMove();
			#endif
		}
	}
	
	CheckAndProcForGroupWnd();
}