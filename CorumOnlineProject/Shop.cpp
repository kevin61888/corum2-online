//==============================================================//
// Code by Jang.									2003.08.01	//
//==============================================================//
#include "Shop.h"
#include "GameControl.h"
#include "Chat.h"
#include "PlayerShopWnd.h"
#include "Message.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "Filter.h"
#include "ItemNative.h"
#include "InputManager.h"


//==============================================================//
// 노점 오픈 실패 에러 메세지.									//
//==============================================================//
void CmdShopFail(char* pMsg, DWORD dwLen)
{
	DSTC_SHOP_FAIL* pPacket = (DSTC_SHOP_FAIL*)pMsg;
	
	CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

	if(pPacket->byType==1)
	{
		// "마을에서만 상점을 개설할 수 있습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE758].szMessage, 0xFFFFC309); 
	}		
	else if(pPacket->byType==2 || pPacket->byType==3 || pPacket->byType==4)
	{
		// "현재 위치에서 상점을 개설할 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE759].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==5)
	{
		// "인벤토리에 빈 공간이 부족하여 취소할 수 없습니다."	
		DisplayMessageAdd(g_Message[ETC_MESSAGE760].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==6)
	{
		// "판매할 아이템이 설정되지 않았습니다."		
		DisplayMessageAdd(g_Message[ETC_MESSAGE761].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==7)
	{
		// "상점이 취소되었습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE763].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==8)
	{
		// "상점이 취소되었습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE763].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==9)
	{
		// "아이템이 존재하지 않습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE764].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==10)
	{
		// "상점의 품목이 변경되었습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE765].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==11)
	{
		// "카르츠가 부족하여 구매할 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE766].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==12)
	{
		// "상점주인의 최대 카르츠 소유양이 초과되어 판매할 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE767].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==13)
	{
		// "인벤토리에 여유공간이 부족하여 구매할 수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE768].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==14)
	{
		// "교환중에는 노점을 열기를 할수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE772].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==15)
	{
		// "노점중에는 교환을 할수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE773].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==16)
	{
		// "대결중에는 교환을 할수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE774].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==17)
	{
		// "포탈 중에는 교환을 할수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE775].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==18)
	{
		// "앉기중에는 교환을 할수 없습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE797].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==19)
	{
		// "노점도중의 판매자의 가격변경으로 구매가 안됩니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1042].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==20)
	{
		// "판매가격이 설정되어있지 않습니다."
		DisplayMessageAdd(g_Message[ETC_MESSAGE1256].szMessage, 0xFFFFC309); 
	}
	else if(pPacket->byType==21)
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE1426].szMessage, 0xFFFFC309); //아이템을 가져올 수 없습니다.
	}
	else if(pPacket->byType==22)
	{
		DisplayMessageAdd(g_Message[ETC_MESSAGE1427].szMessage, 0xFFFFC309); //길드전매칭시에는 노점을 열수 없습니다.
	}
	else if(pPacket->byType==0)
	{
		__lstrcpyn(g_pMainPlayer->m_szPlayerShopTitle
			, g_pMainPlayer->m_sPlayerShop.szTitle, MAX_REAL_PLAYER_SHOP_TITLE);
		pPlayerShopWnd->PlayerShopStart();
	}
}


//==============================================================//
// 노점에서 아이템 팔렸을 경우 처리.							//
//==============================================================//
void CmdPlayerShopSell(char* pMsg, DWORD dwLen)
{
	DSTC_PLAYERSHOP_SELL* pPacket = (DSTC_PLAYERSHOP_SELL*)pMsg;

	if( IsUltra() )
	{
		if( pPacket->byBuyRealCnt > 0 )
		{
			BYTE byCnt = g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode].GetQuantity();
			g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode].SetQuantity(BYTE(byCnt - pPacket->byBuyRealCnt));
		}
		else
		{
			memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode], &pPacket->cItem, sizeof(CItem));

			if(g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode].GetID()==0)
				g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->byZipCode] = 0;
		}

		g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
	}
	else
	{
		memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode], &pPacket->cItem, sizeof(CItem));

		if(g_pMainPlayer->m_sPlayerShop.cItem[pPacket->byZipCode].GetID()==0)
			g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[pPacket->byZipCode] = 0;

		g_pMainPlayer->m_dwMoney = pPacket->dwMoney;
	}
}


//==============================================================//
// 노점에서 아이템이 매진되었을때 처리.							//
//==============================================================//
void CmdPlayerShopSoldOut(char* pMsg, DWORD dwLen)
{
//	DSTC_PLAYERSHOP_SOLDOUT* pPacket = (DSTC_PLAYERSHOP_SOLDOUT*)pMsg;

	g_byStatusMessenger	= 0;
						
	// 패킷 보내기 //			
	CTWS_MESSENGER_STATUS pSendPacket;
	pSendPacket.byType		= 0;
	pSendPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
	g_pNet->SendMsg((char*)&pSendPacket, pSendPacket.GetPacketSize(), SERVER_INDEX_WORLD);

	// Close //
	CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

	if(pPlayerShopWnd->GetActive()==TRUE)
	{
		pPlayerShopWnd->SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN1, TRUE);
		pPlayerShopWnd->SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE1, FALSE);
		pPlayerShopWnd->SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE2, FALSE);
		pPlayerShopWnd->SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE3, FALSE);
	}	

	pPlayerShopWnd->m_byMode = 0;
	g_pMainPlayer->SetPlayerShop(0);
}


//==============================================================//
// 노점 닫을때 처리.											//
//==============================================================//
void CmdPlayerShopItem(char* pMsg, DWORD dwLen)
{
	DSTC_PLAYERSHOP_ITEM* pPacket = (DSTC_PLAYERSHOP_ITEM*)pMsg;
	
	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		int index	= pPacket->ItemNode[i].index; // 실제 cItem의 인덱스.
		int nValue	= pPacket->ItemNode[i].cItem.GetID()/ITEM_DISTRIBUTE;		

		if(GetType((WORD)nValue)==ITEM_SMALL)
		{
			BYTE byCnt = Insert_SmallInvItem(&pPacket->ItemNode[i].cItem, 0, pPacket->ItemNode[i].cItem.GetQuantity());

			if(byCnt==0)
				memset(&g_pMainPlayer->m_sPlayerShop.cItem[index], 0, sizeof(CItem));
			else
				g_pMainPlayer->m_sPlayerShop.cItem[index].SetQuantity(byCnt);
		}
		else if(GetType((WORD)nValue)==ITEM_LARGE)
		{
			for(int j = 0; j < MAX_INV_LARGE_POOL; j++)
			{
				if(g_pMainPlayer->m_pInv_Large[j].GetID()==0)
				{
					memcpy(&g_pMainPlayer->m_pInv_Large[j], &pPacket->ItemNode[i].cItem, sizeof(CItem));
					memset(&g_pMainPlayer->m_sPlayerShop.cItem[index], 0, sizeof(CItem));
					break;
				}
			}
		}
	}
}


void CmdPlayerShopItem_ReturnInv(char* pMsg, DWORD dwLen)
{
	DSTC_PLAYERSHOPITEM_RETURN_INV* pPacket = (DSTC_PLAYERSHOPITEM_RETURN_INV*)pMsg;

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
}


//==============================================================//
// 노점 디비 처리후 받는 패킷.									//
//==============================================================//
void CmdShopItem(char* pMsg, DWORD dwLen)
{
	DSTC_SHOP_ITEM* pPacket = (DSTC_SHOP_ITEM*)pMsg;
	
	memset(&g_pMainPlayer->m_sPlayerShop.cItem, 0, sizeof(g_pMainPlayer->m_sPlayerShop.cItem));

	for(int i = 0; i < pPacket->bItemCount; i++)
		memcpy(&g_pMainPlayer->m_sPlayerShop.cItem[pPacket->Nodes[i].index], &pPacket->Nodes[i].cItem, sizeof(CItem));
}


void CmdPlayerShopOpen( char* pMsg, DWORD dwLen )
{
	DSTC_PLAYERSHOP_OPEN* pPacket = (DSTC_PLAYERSHOP_OPEN*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	
	if(!pUser)
		return;
	
	__lstrcpyn(pUser->m_szPlayerShopTitle, pPacket->szTitle, MAX_REAL_PLAYER_SHOP_TITLE);
	pUser->SetPlayerShop(1);
}


void CmdPlayerShopClose( char* pMsg, DWORD dwLen )
{
	DSTC_PLAYERSHOP_CLOSE* pPacket = (DSTC_PLAYERSHOP_CLOSE*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);
	
	CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

	if(pPlayerShopWnd->m_pOwnerPlayerShop && pPlayerShopWnd->GetActive())
	{
		if(	pPlayerShopWnd->m_pOwnerPlayerShop->m_dwUserIndex == pPacket->dwUserIndex &&
			g_pMainPlayer->m_dwUserIndex != pPacket->dwUserIndex)
		{
			// "판매자의 상점이 폐쇄되었습니다."
			DisplayMessageAdd(g_Message[ETC_MESSAGE1010].szMessage, 0xFFFFC309); 
			pPlayerShopWnd->SetActive(FALSE);
		}
	}
	
	if(!pUser)
		return;

	if(pUser->m_dwUserIndex == g_pMainPlayer->m_dwUserIndex)
	{
		pPlayerShopWnd->SetInputMode(0);
		return;
	}
			
	pUser->SetPlayerShop(0);
}


void CmdPlayerShopShowAll( char* pMsg, DWORD dwLen )
{
	DSTC_PLAYERSHOP_SHOWALL* pPacket = (DSTC_PLAYERSHOP_SHOWALL*)pMsg;
	
	CUser* pUser = g_pUserHash->GetData(pPacket->dwUserIndex);

	if(!pUser)
		return;

	CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
	
	// 초기화.
	memset(&pPlayerShopWnd->m_sPlayerShopInfo, 0, sizeof(pPlayerShopWnd->m_sPlayerShopInfo));
	
	// title 복사!!
	__lstrcpyn(pPlayerShopWnd->m_sPlayerShopInfo.szTitle, pPacket->szTitle, MAX_PLAYER_SHOP_TITLE + 1);

	// item 정보 복사!!
	for(int i = 0; i < pPacket->bItemCount; i++)
	{
		pPlayerShopWnd->m_sPlayerShopInfo.m_dwCustomSellPrice[pPacket->ItemNode[i].index] = pPacket->ItemNode[i].m_dwCustomSellPrice;
		memcpy(&pPlayerShopWnd->m_sPlayerShopInfo.cItem[pPacket->ItemNode[i].index], &pPacket->ItemNode[i].cItem, sizeof(CItem));
	}
			
	pPlayerShopWnd->m_pOwnerPlayerShop = pUser;
	pPlayerShopWnd->SetActive();
}

//==============================================================//
// End.															//
//==============================================================//