//==============================================================//
// Code by Jang.									2003.08.01	//
//==============================================================//
#include "Trade.h"
#include "ItemTradeShopWnd.h"
#include "ItemWnd.h"
#include "Interface.h"
#include "UserInterface.h"
#include "Message.h"
#include "ItemTradeWnd.h"
#include "User.h"
#include "../CommonServer/CommonHeader.h"
#include "QuantityWnd.h"
#include "DungeonProcess.h"
#include "GameControl.h"
#include "HashTable.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "BankWnd.h"
#include "StoreWnd.h"
#include "CodeFun.h"
#include "ItemNative.h"


//==============================================================//
// 아이템 교환시 교환버튼을 선택했을 경우 처리.					//
//==============================================================//
void CmdTradeOk( char* pMsg, DWORD dwLen )
{
	// 자기 자신 Ok를 했을 경우 //
	DSTC_TRADE_OK*		pPacket				= (DSTC_TRADE_OK*)pMsg;
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	char				szInfo[0xff]		= {0,};

	if(pPacket->byType==1)
	{
		// 확인만 되었을 경우 //
		// "%s 님에 응답을 기다리고 있습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE647].szMessage, pPacket->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
		pItemTradeShopWnd->m_bOkChk = TRUE;
		pItemTradeShopWnd->SetOk();
	}
	else if(pPacket->byType==3)
	{
		// 확인만 되었을 경우 //
		// "%s 님이 아이템 교환을 확인하셨습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE648].szMessage, pPacket->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);		
	}
	else if(pPacket->byType==2)
	{
		// 교환 활성화 //
		// "교환버튼이 활성화 되었습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE649].szMessage); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);

		// 버튼 변경 //
		pItemTradeShopWnd->m_bTradeChk = TRUE;
		pItemTradeShopWnd->SetTrade();
		pItemTradeShopWnd->m_bOkChk = TRUE;
		pItemTradeShopWnd->SetOk();
	}
}


//==============================================================//
// 아이템 교환 처리.											//
//==============================================================//
void CmdTradeData( char* pMsg, DWORD dwLen )
{
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	DSTC_TRADE_DATA*	pPacket				= (DSTC_TRADE_DATA*)pMsg;
	char				szInfo[0xff]		= {0,};
	
	g_pMainPlayer->m_dwMoney = pPacket->dwMoeny;

	memset(&g_pMainPlayer->m_pItemTrade, 0, sizeof(CItem)*MAX_TRADE);
	memset(&pItemTradeShopWnd->m_pTradeItem, 0, sizeof(CItem)*MAX_TRADE);	

	if( IsUltra() )
	{
		ITEM_TYPE eType;
		ITEM_NATIVE_INFO ItemNativeInfoDest;
		
		for(int i = 0; i < pPacket->btItemCount; i++)
		{
			eType = GetItemType( pPacket->Nodes[i].cItem.GetID() );

			if( eType == ITEM_TYPE_LARGE)
			{
				ItemNativeInfoDest.eSlotID		= ITEM_NATIVE_INV_LARGE;
				ItemNativeInfoDest.bySlotIndex	= pPacket->Nodes[i].index;
				
				SetSlotItemNative( &pPacket->Nodes[i].cItem, &ItemNativeInfoDest);
			}
			else if( eType == ITEM_TYPE_SMALL )
			{
				ItemNativeInfoDest.eSlotID		= ITEM_NATIVE_INV_SMALL;
				ItemNativeInfoDest.bySlotIndex	= pPacket->Nodes[i].index;
				
				SetSlotItemNative( &pPacket->Nodes[i].cItem, &ItemNativeInfoDest);
			}
		}
	}
	else
	{
		// modified by deepdark.
		// 아이템 이동 //
		for(int i = 0; i < pPacket->btItemCount; i++)
		{
			int nValue = pPacket->Nodes[i].cItem.GetID()/ITEM_DISTRIBUTE;

			if(nValue <= 20)
			{
				for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
				{
					if(g_pMainPlayer->m_pInv_Large[j].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pInv_Large[j], &pPacket->Nodes[i].cItem, sizeof(CItem));
						break;
					}
				}
			}
		}

		// 소비 아이템 //
		for(i = 0; i < pPacket->btItemCount; i++)
		{
			int nValue = pPacket->Nodes[i].cItem.GetID()/ITEM_DISTRIBUTE;

			if(GetType((WORD)nValue)==ITEM_SMALL)
			{
				Trade_SmallItem(&pPacket->Nodes[i].cItem);
			}
		}
	}

	if(pItemWnd->GetActive()==TRUE)	
	{
		pItemWnd->SetLine(TRUE, pItemWnd->m_byItemChk);
	}

	// ItemTradeShopWnd //
	pItemTradeShopWnd->m_bTradeOkChk = FALSE;
	pItemTradeShopWnd->SetActive(FALSE);

	// "아이템 교환이 성공하였습니다."
	lstrcpy(szInfo, g_Message[ETC_MESSAGE650].szMessage); 
	DisplayMessageAdd(szInfo, 0xFFFFFF00);
}


//==============================================================//
// 교환중에 인벤토리에 소비아이템 삽입.							//
//==============================================================//
void Trade_SmallItem(CItem* pItem)
{
//	int		nValue		= pItem->GetID()/ITEM_DISTRIBUTE;
	BYTE	byPackCnt	= pItem->GetQuantity();
		
	for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(g_pMainPlayer->m_pInv_Small[i].GetID()==0)
		{
			memcpy(&g_pMainPlayer->m_pInv_Small[i], pItem, sizeof(CItem));
			g_pMainPlayer->m_pInv_Small[i].SetQuantity(byPackCnt);
			break;
		}
		else if(g_pMainPlayer->m_pInv_Small[i].GetID()==pItem->GetID())
		{
//			LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Small[i].m_wItemID);
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Small[i].m_wItemID);

//			int nValue			= g_pMainPlayer->m_pInv_Small[i].m_wItemID/ITEM_DISTRIBUTE;
			int nMaxQuantity	= 1;

			nMaxQuantity = lpItemTable->GetMaxQuantity();				
			
			BYTE byCnt = BYTE(nMaxQuantity - g_pMainPlayer->m_pInv_Small[i].GetQuantity());

			if(byCnt>=byPackCnt)
			{
				BYTE bQuantity = g_pMainPlayer->m_pInv_Small[i].GetQuantity();
				g_pMainPlayer->m_pInv_Small[i].SetQuantity(BYTE(bQuantity+byPackCnt));
				break;
			}
			else if(byCnt!=0)
			{
				BYTE bQuantity = g_pMainPlayer->m_pInv_Small[i].GetQuantity();
				g_pMainPlayer->m_pInv_Small[i].SetQuantity(BYTE(bQuantity+byCnt));
				byPackCnt = byPackCnt - byCnt;
			}
		}
	}
}


//==============================================================//
// 교환 실패 메세지.											//
//==============================================================//
void CmdTradeRetrun( char* pMsg, DWORD dwLen )
{
	DSTC_TRADE_RETURN*	pPacket			= (DSTC_TRADE_RETURN*)pMsg;
	char				szInfo[0xff]	= {0,};

	switch(pPacket->byType)
	{	
	case 0:
		// "교환할 아이템이 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE651].szMessage); 
		break;
	case 1:
		// "Karz가 초과하였습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE652].szMessage); 
		break;
	case 2:
		// "%s 님에 Karz가 초과하였습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE653].szMessage, pPacket->szCharacterName); 
		break;
	case 3:
		// "장비 아이템에 빈 공간이 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE654].szMessage); 
		break;
	case 4:
		// "%s 님에 장비 아이템에 빈 공간이 없습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE655].szMessage, pPacket->szCharacterName); 
		break;
	case 5:
		// "소비 아이템에 빈 공간이 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE656].szMessage); 
		break;
	case 6:
		// "%s 님에 소비 아이템에 빈 공간이 없습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE657].szMessage, pPacket->szCharacterName); 
		break;
	case 7:
		// "DB에 업데이트에 문제가 생겼습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE658].szMessage); 
		break;
	case 8:
		//"동일한 번호가 존재하는 복사아이템일 가능성이 있어 교환하실 수가 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE993].szMessage);
		break;
	}
	
	DisplayMessageAdd(szInfo, 0xFFFFFF00);
}


//==============================================================//
// 교환 승인.													//
//==============================================================//
void CmdTradeTrade( char* pMsg, DWORD dwLen )
{
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	DSTC_TRADE_TRADE*	pPacket				= (DSTC_TRADE_TRADE*)pMsg;
	char				szInfo[0xff]		= {0,};
	
	if(pPacket->byType==0)
	{
		// 자기 자신이 교환을 승인했을 경우 //
		// "%s 님에 응답을 기다리고 있습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE647].szMessage, pPacket->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
		pItemTradeShopWnd->m_bTradeOneChk = TRUE;
		pItemTradeShopWnd->SetTradeOne();
	}
	else
	{
		// 상대방 교환을 승인했을 경우 //
		// "%s님이 교환을 승인 하셨습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE659].szMessage, pPacket->szCharacterName); 
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
}


//==============================================================//
// 상대편에 정보를 설정 하고 보여준다.							//
//==============================================================//
void CmdTradeItem( char* pMsg, DWORD dwLen )
{
	DSTC_TRADE_ITEM*	pPacket				= (DSTC_TRADE_ITEM*)pMsg;
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();

	// 상대편이 Ok를 했을 경우 //
	memset(&pItemTradeShopWnd->m_pTradeItem, 0, sizeof(pItemTradeShopWnd->m_pTradeItem));
	
	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		memcpy(&pItemTradeShopWnd->m_pTradeItem[pPacket->Nodes[i].index], &pPacket->Nodes[i].cItem, sizeof(CItem));
	}

	pItemTradeShopWnd->m_dwDstMoney = pPacket->dwMoney;	
}


//==============================================================//
// 아이템 교환 취소 처리.										//
//==============================================================//
void CmdTradeCancelFail( char* pMsg, DWORD dwLen )
{
	DSTC_TRADE_CANCELFAIL*	pPacket			= (DSTC_TRADE_CANCELFAIL*)pMsg;
	char					szInfo[0xff]	= {0,};

	if(pPacket->byType==0)
	{
		// "%s 님이 아이템 교환을 취소하였습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE661].szMessage, pPacket->szCharacterName); 
	}
	else if(pPacket->byType==1)
	{
		// "아이템 교환이 취소되었습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE660].szMessage); 
	}

	DisplayMessageAdd(szInfo, 0xFFFFFF00);
}


//==============================================================//
// 아이템 교환 취소 처리(아이템 처리).							//
//==============================================================//
void CmdTradeCancel(char* pMsg, DWORD dwLen)
{	
	DSTC_TRADE_CANCEL*	pPacket				= (DSTC_TRADE_CANCEL*)pMsg;
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
//	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();	
	BYTE				byBankItem			= 0;	
	char				szInfo[0xff]		= {0,};

	memset(pItemTradeShopWnd->m_szName, 0, sizeof(pItemTradeShopWnd->m_szName));
	pItemTradeShopWnd->m_dwMoney = 0;

	if(pPacket->byType==0)
	{
		// "아이템 교환이 취소되었습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE660].szMessage); 		
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	else if(pPacket->byType==1)
	{
		// "%s 님이 아이템 교환을 취소하였습니다."	
		wsprintf(szInfo, g_Message[ETC_MESSAGE661].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	else if(pPacket->byType==2)
	{
		// "%s 님이 게임을 종료하였습니다."	
		wsprintf(szInfo, g_Message[ETC_MESSAGE662].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	
	pItemTradeShopWnd->SetActive(FALSE);
	
	// ITEM MOVE... Modified by deepdark.
	for(int i = 0; i < pPacket->btItemCount; i++)
	{
		int nValue = pPacket->Nodes[i].cItem.GetID()/ITEM_DISTRIBUTE;
		
		// 스몰 아이템 //
		if(GetType((WORD)nValue)==ITEM_SMALL)				
		{
			Insert_SmallInvItem(&pPacket->Nodes[i].cItem, 0, pPacket->Nodes[i].cItem.GetQuantity());
		}
		else
		{
			// 큰 아이템 //
			if(pPacket->bInvType[i]==0)
			{
				for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
				{
					if(g_pMainPlayer->m_pInv_Large[j].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pInv_Large[j], &pPacket->Nodes[i].cItem, sizeof(CItem));
						break;
					}
				}
			}
			else
			{
				for(int j = 0; j < MAX_BANK_LARGE_POOL; j++)
				{
					if(g_pMainPlayer->m_pBank_Large[j].GetID()==0)
					{
						memcpy(&g_pMainPlayer->m_pBank_Large[j], &pPacket->Nodes[i].cItem, sizeof(CItem));
						byBankItem+=1;
						break;
					}
				}
			}
		}

		memset(&g_pMainPlayer->m_pItemTrade[pPacket->Nodes[i].index], 0, sizeof(CItem));
	}

	memset(pItemTradeShopWnd->m_pTradeItem, 0, sizeof(CItem)*MAX_TRADE);

	if(byBankItem > 0)
	{
		// "장비창에 공간이 부족합니다. %d개에 아이템을 창고로 이동합니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE1168].szMessage, byBankItem);
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
}


void CmdTradeCancelReturnInv(char* pMsg, DWORD dwLen)
{
	DSTC_TRADE_CANCEL_RETURN_INV* pPacket = (DSTC_TRADE_CANCEL_RETURN_INV*)pMsg;

	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
//	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	char				szInfo[0xff]		= {0,};

	memset(pItemTradeShopWnd->m_szName, 0, sizeof(pItemTradeShopWnd->m_szName));
	pItemTradeShopWnd->m_dwMoney = 0;

	if(pPacket->byType==0)
	{
		// "아이템 교환이 취소되었습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE660].szMessage); 		
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	else if(pPacket->byType==1)
	{
		// "%s 님이 아이템 교환을 취소하였습니다."	
		wsprintf(szInfo, g_Message[ETC_MESSAGE661].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	else if(pPacket->byType==2)
	{
		// "%s 님이 게임을 종료하였습니다."	
		wsprintf(szInfo, g_Message[ETC_MESSAGE662].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFFF00);
	}
	
	pItemTradeShopWnd->SetActive(FALSE);

	ITEM_NATIVE_INFO	ItemNativeInfo_Src;
	ITEM_NATIVE_INFO	ItemNativeInfo_Dest;
//	CBaseItem*			pBaseItemSrc		= NULL;
//	CBaseItem*			pBaseItemDest		= NULL;
//	const CItem*		pItemSrc			= NULL;
//	const CItem*		pItemDest			= NULL;
	
	ItemNativeInfo_Src.eSlotID		= ITEM_NATIVE_PLAYERSHOP;
	ItemNativeInfo_Src.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;
	
	ItemNativeInfo_Dest.dwOwnerID	= g_pMainPlayer->m_dwUserIndex;

	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		ItemNativeInfo_Src.bySlotIndex = pPacket->ItemNodeNative[i].index;				
		
		ItemNativeInfo_Dest.eSlotID		= pPacket->ItemNodeNative[i].eSlotID;
		ItemNativeInfo_Dest.bySlotIndex = pPacket->ItemNodeNative[i].bySlotIndex;
		
		AllLinkSrcToDestNative(&ItemNativeInfo_Src,&ItemNativeInfo_Dest);
	}

	memset(pItemTradeShopWnd->m_pTradeItem, 0, sizeof(CItem)*MAX_TRADE);
}


//==============================================================//
// 아이템 교환 신청 결과.										//
//==============================================================//
void CmdTradeResult(char* pMsg, DWORD dwLen)
{
	DSTC_TRADE_RESULT*	pPacket				= (DSTC_TRADE_RESULT*)pMsg;
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
	CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
	CStoreWnd*			pStoreWnd			= CStoreWnd::GetInstance();
	CBankWnd*			pBankWnd			= CBankWnd::GetInstance();

	char szInfo[0xff] = {0,};

	if(pPacket->byCode == 0)
	{
		// 교환창 //
		// MSG_ID : 107 ; %s 님이 아이템 교환을 승인하셨습니다.
		wsprintf(szInfo, g_Message[ETC_MESSAGE107].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFC309);

		// Open Windows //		
		pItemTradeShopWnd->SetActive(TRUE);
		pItemTradeShopWnd->m_bTradeOkChk = TRUE;

		memset(pItemTradeShopWnd->m_szName, 0, sizeof(pItemTradeShopWnd->m_szName));
		__lstrcpyn(pItemTradeShopWnd->m_szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);				
		
		if(!pItemWnd->GetActive())
			pItemWnd->SetActive(TRUE);				
		if(pStoreWnd->GetActive())
			pStoreWnd->SetActive(FALSE);		
		if(pBankWnd->GetActive())
			pBankWnd->SetActive(FALSE);
			
		return;
	}
	else if(pPacket->byCode == 1)
	{
		// MSG_ID : 108 ; %s 님이 아이템 교환을 거부하셨습니다.
		wsprintf(szInfo, g_Message[ETC_MESSAGE108].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFC309);

		return;
	}
}


//==============================================================//
// 아이템 교환 신청 받음(신청시 문제가 있으면 에러 메세지).		//
//==============================================================//
void CmdTradeRequest(char* pMsg, DWORD dwLen)
{
	DSTC_TRADE_REQUEST* pPacket				= (DSTC_TRADE_REQUEST*)pMsg;	
	CItemTradeWnd*		pItemTradeWnd		= CItemTradeWnd::GetInstance();
	CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();

	char szInfo[0xff] = {0,};	

	if(pPacket->byCode == 1)
	{
		// MSG_ID : 109 ; %s 님이 로그 아웃 하셨습니다.
		wsprintf(szInfo, g_Message[ETC_MESSAGE109].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFC309);
		return;
	}
	else if(pPacket->byCode == 2)
	{
		// MSG_ID : 110 ; %s 님이 이미 아이템 교환 요청을 받으셨습니다.
		wsprintf(szInfo, g_Message[ETC_MESSAGE110].szMessage, pPacket->szCharacterName);	
		DisplayMessageAdd(szInfo, 0xFFFFC309);
		return;
	}
	else if(pPacket->byCode == 3)
	{
		// "동시에 교환을 할수가 없습니다."
		lstrcpy(szInfo, g_Message[ETC_MESSAGE663].szMessage); 
		DisplayMessageAdd(szInfo, 0xFFFFC309);
		return;
	}
	
	if(pItemTradeWnd->GetActive() == TRUE || pItemTradeShopWnd->GetActive() == TRUE)
	{
		// 이미 트래이드 요청을 받았으면 실패 //
		CTDS_TRADE_REQUEST pSendPacket;
		memset(pSendPacket.szCharacterName, 0, sizeof(pSendPacket.szCharacterName));
		__lstrcpyn(pSendPacket.szCharacterName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);

		pSendPacket.dwUserIndex	= pPacket->dwUserIndex;
		pSendPacket.byCode		= 1;

		g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_ZONE);
		return;
	}

	memset(pItemTradeWnd->m_szName, 0, sizeof(pItemTradeWnd->m_szName));
	__lstrcpyn(pItemTradeWnd->m_szName, pPacket->szCharacterName, MAX_CHARACTER_NAME_REAL_LENGTH);
	pItemTradeWnd->m_dwUserIndex	= pPacket->dwUserIndex;
	pItemTradeWnd->SetActive(TRUE);

	// 메세지 //	
	// MSG_ID : 111 ; %s 님에게서 교환 요청이 들어왔습니다.
	wsprintf(szInfo, g_Message[ETC_MESSAGE111].szMessage, pPacket->szCharacterName);	
	DisplayMessageAdd(szInfo, 0xFFFFC309);
}


//==============================================================//
// End.															//
//==============================================================//