//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "ItemTradeShopWnd.h"
#include "GameControl.h"
#include "User.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "ItemWnd.h"
#include "../CommonServer/AbyssHash.h"
#include "HashTable.h"
#include "ChatWnd.h"
#include "QuantityWnd.h"
#include "InputManager.h"
#include "DungeonTable.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"

CItemTradeShopWnd* CItemTradeShopWnd::c_pThis = NULL;

//======================================================//
// Construction/Desertion.								//
//======================================================//
CItemTradeShopWnd::CItemTradeShopWnd()
{
	m_dwMoney		= 0;			
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
	m_bBtn[2]		= FALSE;
	m_bBtn[3]		= FALSE;
	m_bOkChk		= FALSE;
	m_bTradeChk		= FALSE;
	m_bTradeOkChk	= FALSE;
	m_bTradeOneChk	= FALSE;

	memset(m_pTradeItem, NULL, sizeof(m_pTradeItem));
}

CItemTradeShopWnd::~CItemTradeShopWnd()
{
	
}


//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CItemTradeShopWnd::Init()
{
	/*
	InsertData(SPR_OBJ_ITEMTRADESHOP_WND1, SPR_INTERFACE_ITEMTRADESHOP_WND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_ITEMTRADESHOP_WND2, SPR_INTERFACE_ITEMTRADESHOP_WND, 256, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_ITEMTRADESHOP_CLOSE1, SPR_INTERFACE_CLOSE1, 498, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMTRADESHOP_CLOSE2, SPR_INTERFACE_CLOSE2, 498, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMTRADESHOP_TRADE1, SPR_INTERFACE_ITEMTRADESHOP_TRADE1, 224, 219, 1.0f, 1.0f, 4);
	InsertData(SPR_OBJ_ITEMTRADESHOP_TRADE2, SPR_INTERFACE_ITEMTRADESHOP_TRADE2, 224, 219, 1.0f, 1.0f, 4);
	InsertData(SPR_OBJ_ITEMTRADESHOP_TRADE3, SPR_INTERFACE_ITEMTRADESHOP_TRADE3, 224, 219, 1.0f, 1.0f, 4);
	InsertData(SPR_OBJ_ITEMTRADESHOP_TRADE4, SPR_INTERFACE_ITEMTRADESHOP_TRADE4, 224, 219, 1.0f, 1.0f, 4);
	InsertData(SPR_OBJ_ITEMTRADESHOP_OK1_1, SPR_INTERFACE_ITEMTRADESHOP_OK0, 96, 219, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADESHOP_OK1_2, SPR_INTERFACE_OK1, 96, 219, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADESHOP_OK1_3, SPR_INTERFACE_OK2, 96, 219, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADESHOP_OK1_4, SPR_INTERFACE_OK3, 96, 219, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMTRADESHOP_OK2_1, SPR_INTERFACE_ITEMTRADESHOP_OK0, 352, 219, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMTRADESHOP_OK2_2, SPR_INTERFACE_OK1, 352, 219, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMTRADESHOP_OK2_3, SPR_INTERFACE_OK2, 352, 219, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMTRADESHOP_OK2_4, SPR_INTERFACE_OK3, 352, 219, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMTRADESHOP_CANCEL1, SPR_INTERFACE_CANCEL1, 224, 219, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_ITEMTRADESHOP_CANCEL2, SPR_INTERFACE_CANCEL2, 224, 219, 1.0f, 1.0f, 3);
	InsertData(SPR_OBJ_ITEMTRADESHOP_CANCEL3, SPR_INTERFACE_CANCEL3, 224, 219, 1.0f, 1.0f, 3);	

	InsertCheckInterface(498, 4, 510, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 498, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(96, 219, 160, 235, 4, CHECK_BTN);
	InsertCheckInterface(224, 219, 288, 235, 5, CHECK_BTN);
	InsertCheckInterface(7, 52, 252, 181, 6, CHECK_ITEM);
	InsertCheckInterface(2, 189, 253, 201, 7, CHECK_BTN);
*/
	for(int i = 0; i < MAX_TRADE*2; i++)
	{
		if(i<MAX_TRADE)
			memset(&g_pMainPlayer->m_pItemTrade[i], 0, sizeof(CItem));					
		else
			memset(&m_pTradeItem[(i-MAX_TRADE)], 0, sizeof(CItem));
	}
	
	m_bInit	= TRUE;

	return TRUE;
}

void CItemTradeShopWnd::Remove()
{
	RemoveAllData();			
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CItemTradeShopWnd::SetOrder()
{

}

void CItemTradeShopWnd::RenderText()
{	
	char szInfo[0xff] = {0,};
	
	lstrcpy(szInfo, g_Message[ETC_MESSAGE450].szMessage);
	RenderFont(szInfo, m_fPosX+221, m_fPosX+300, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);

	int nSize = lstrlen(g_pMainPlayer->m_szName);
	RenderFont(g_pMainPlayer->m_szName
		, m_fPosX+128-(nSize/2)*8, m_fPosX+128+(nSize/2)*8, m_fPosZ+33, m_fPosZ+47, GetStartOrder()+1);
	
	nSize = lstrlen(m_szName);
	RenderFont(m_szName
		, m_fPosX+384-(nSize/2)*8, m_fPosX+384+(nSize/2)*8, m_fPosZ+33, m_fPosZ+47, GetStartOrder()+1);
	
	for(int i = 0; i < MAX_TRADE; i++)
	{
		int nValue = g_pMainPlayer->m_pItemTrade[i].GetID()/ITEM_DISTRIBUTE;

		if(GetType(nValue)==ITEM_SMALL)
		{
			// 수량 //
			wsprintf(m_szInfo, "%u", g_pMainPlayer->m_pItemTrade[i].GetQuantity());

			int nLen = lstrlen(m_szInfo);
											
			// 좌측 상단 좌표 //
			int nPosX = (int)(m_fPosX+7)+(i%7)*35;
			int nPosY = (int)(m_fPosZ+52)+(i/7)*67;

			if(!IsEmptyString(m_szInfo))
				RenderFont(m_szInfo, nPosX+30-nLen*6, nPosX+32, nPosY+20, nPosY+34, GetStartOrder()+4);
		}
	}

	for(i = 0; i < MAX_TRADE; i++)
	{
		int nValue = m_pTradeItem[i].GetID()/ITEM_DISTRIBUTE;

		if(GetType(nValue)==ITEM_SMALL)
		{
			// 수량 //
			wsprintf(m_szInfo, "%u", m_pTradeItem[i].GetQuantity());

			int nLen = lstrlen(m_szInfo);
											
			// 좌측 상단 좌표 //
			int nPosX = (int)(m_fPosX+263)+(i%7)*35;
			int nPosY = (int)(m_fPosZ+52)+(i/7)*67;

			if(!IsEmptyString(m_szInfo))
				RenderFont(m_szInfo, nPosX+30-nLen*6, nPosX+32, nPosY+20, nPosY+34, GetStartOrder()+4);
		}
	}

	NUMBERFMT nFmt = { 0, 0, 3, ".", ",", 1 };

	if(m_dwMoney!=0)
	{
		wsprintf(szInfo, "%u", m_dwMoney);

		::GetNumberFormat(NULL, NULL, szInfo, &nFmt, m_szInfo, 15);

		int nLen = lstrlen(m_szInfo);
		RenderFont(m_szInfo, m_fPosX+220-nLen*6, m_fPosX+256, m_fPosZ+192, m_fPosZ+206, GetStartOrder()+4);		
	}

	if(m_dwDstMoney!=0)
	{
		wsprintf(szInfo, "%u", m_dwDstMoney);

		::GetNumberFormat(NULL, NULL, szInfo, &nFmt, m_szInfo, 15);

		int nLen = lstrlen(m_szInfo);
		RenderFont(m_szInfo, m_fPosX+476-nLen*6, m_fPosX+490, m_fPosZ+192, m_fPosZ+206, GetStartOrder()+4);		
	}

	RenderTrade();
	ItemInfoRender();
}

void CItemTradeShopWnd::SetTrade()
{
	SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE1, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE2, TRUE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE3, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE4, FALSE);

	SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_1, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_2, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_3, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_4, FALSE);

	SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL1, TRUE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL2, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL3, FALSE);

	SetPosObjX(SPR_OBJ_ITEMTRADESHOP_CANCEL1, 96);
	SetPosObjX(SPR_OBJ_ITEMTRADESHOP_CANCEL2, 96);
	SetPosObjX(SPR_OBJ_ITEMTRADESHOP_CANCEL3, 96);
}

void CItemTradeShopWnd::SetOk()
{
	SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_1, TRUE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_2, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_3, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_4, FALSE);
}

void CItemTradeShopWnd::SetTradeOne()
{
	SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE1, TRUE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE2, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE3, FALSE);
	SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE4, FALSE);
}

void CItemTradeShopWnd::ItemInfoRender()
{	
	CItem*	pItem;				
	int		nIndex		= 0;
	int		nPosX		= 0;
	int		nPosY		= 0;
		
	if(g_Mouse.bLDown==TRUE)	
		return;
	
	if(	g_Mouse.MousePos.x>m_fPosX+7 &&	g_Mouse.MousePos.x<m_fPosX+252 &&
		g_Mouse.MousePos.y>m_fPosZ+52 && g_Mouse.MousePos.y<m_fPosZ+186)	
	{
		nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-7)/35;
		nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-52)/67;
		nIndex	= nPosY*7+nPosX;
		
		pItem = &g_pMainPlayer->m_pItemTrade[nIndex];

		if(!pItem)
			return;
		
		if(pItem->GetID() == 0)
			return;

		if(pItem->GetType()==ITEM_LARGE)
		{
			if(nIndex>=MAX_INV_LARGE_POOL)
				return;
			else if(CUserInterface::GetInstance()->InterfaceCollision(ITEMTRADESHOP_WND
				, (int)(m_fPosX+nPosX*35+7), (int)(m_fPosX+(nPosX+1)*35+7)
				, (int)(m_fPosZ+nPosY*67+52), (int)(m_fPosZ+(nPosY+1)*67+52))==FALSE)
				return;
			else if(g_pMainPlayer->m_pItemTrade[nIndex].m_wItemID==0)
				return;		
		}
		else
		{
			if((m_fPosZ+(nPosY+1)*35+52)<g_Mouse.MousePos.y)
				return;
			else if(nIndex>=MAX_INV_SMALL_POOL)
				return;
			else if(CUserInterface::GetInstance()->InterfaceCollision(ITEMTRADESHOP_WND
				, (int)(m_fPosX+nPosX*35+7), (int)(m_fPosX+(nPosX+1)*35+7)
				, (int)(m_fPosZ+nPosY*35+52), (int)(m_fPosZ+(nPosY+1)*35+52))==FALSE)
				return;
			else if(g_pMainPlayer->m_pItemTrade[nIndex].m_wItemID==0)
				return;						
		}
		
		CInterface::GetInstance()->ItemInfoRender(pItem, FALSE);
	}
		
	if(	g_Mouse.MousePos.x>m_fPosX+263 && g_Mouse.MousePos.x<m_fPosX+508 &&
		g_Mouse.MousePos.y>m_fPosZ+52 && g_Mouse.MousePos.y<m_fPosZ+186)	
	{
		nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-263)/35;
		nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-52)/67;
		nIndex	= nPosY*7+nPosX;
		
		pItem = &m_pTradeItem[nIndex];

		if(!pItem)
			return;
		
		if(pItem->GetID() == 0)
			return;
					
		if(pItem->GetType()==ITEM_LARGE)
		{
			if(nIndex>=MAX_INV_LARGE_POOL)
				return;
			else if(CUserInterface::GetInstance()->InterfaceCollision(ITEMTRADESHOP_WND
				, (int)(m_fPosX+nPosX*35+263), (int)(m_fPosX+(nPosX+1)*35+263)
				, (int)(m_fPosZ+nPosY*67+52), (int)(m_fPosZ+(nPosY+1)*67+52))==FALSE)
				return;
			else if(m_pTradeItem[nIndex].m_wItemID==0)
				return;			
		}
		else
		{
			if((m_fPosZ+(nPosY+1)*35+52)<g_Mouse.MousePos.y)
				return;
			else if(nIndex>=MAX_INV_SMALL_POOL)
				return;
			else if(CUserInterface::GetInstance()->InterfaceCollision(ITEMTRADESHOP_WND
				, (int)(m_fPosX+nPosX*35+263), (int)(m_fPosX+(nPosX+1)*35+263)
				, (int)(m_fPosZ+nPosY*35+52), (int)(m_fPosZ+(nPosY+1)*35+52))==FALSE)
				return;
			else if(m_pTradeItem[nIndex].m_wItemID==0)
				return;
		}

		CInterface::GetInstance()->ItemInfoRender(pItem, FALSE);
	}	
}

void CItemTradeShopWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive==FALSE)
	{
		m_bSoundChk		= FALSE;
		m_bOkChk		= FALSE;
		m_bTradeChk		= FALSE;
		m_bTradeOneChk	= FALSE;
		
		SetPosObjX(SPR_OBJ_ITEMTRADESHOP_CANCEL1, 224);
		SetPosObjX(SPR_OBJ_ITEMTRADESHOP_CANCEL2, 224);
		SetPosObjX(SPR_OBJ_ITEMTRADESHOP_CANCEL3, 224);

		ShowSpriteAll();
		
		m_dwMoney = 0;

		if(m_bTradeOkChk == TRUE)
		{
			// Item Trade 취소 //
			CTDS_TRADE_CALCEL pPacket;
			pPacket.bType = 1;
			g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);		
		}

		m_bTradeOkChk	= FALSE;
		m_dwMoney		= 0;
		m_dwDstMoney	= 0;
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_ITEMTRADESHOP_WND1, TRUE);
		SetRender(SPR_OBJ_ITEMTRADESHOP_WND2, TRUE);
		SetRender(SPR_OBJ_ITEMTRADESHOP_CLOSE1, TRUE);	
		SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_2, TRUE);
		SetRender(SPR_OBJ_ITEMTRADESHOP_OK2_1, TRUE);
		SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CItemTradeShopWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(nRt!=5)
	{
		if(m_bTradeChk==FALSE)
		{
			if(m_bBtn[1]==TRUE)
			{
				SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL1, TRUE);
				SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL2, FALSE);
				SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL3, FALSE);
				m_bBtn[1] = FALSE;
			}
		}		
		else
		{
			if(m_bTradeOneChk==FALSE)
			{
				if(m_bBtn[3]==TRUE)
				{
					SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE2, TRUE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE3, FALSE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE4, FALSE);
					m_bBtn[3] = FALSE;
				}
			}			
		}		
	}
	
	if(nRt!=4)
	{
		if(m_bTradeChk==FALSE)
		{
			if(m_bOkChk==FALSE)
			{
				if(m_bBtn[0]==TRUE)
				{			
					SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_2, TRUE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_3, FALSE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_4, FALSE);
					m_bBtn[0] = FALSE;
				}		
			}		
		}
		else
		{
			if(m_bBtn[2]==TRUE)
			{
				SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL1, TRUE);
				SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL2, FALSE);
				SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL3, FALSE);
				m_bBtn[2] = FALSE;
			}
		}
	}

	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);
	
	switch(nRt)
	{	
	case 1:
		{			
			SetMouseCheck(nRt, eMouseCheck);						
		}
		break;
	case 2:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_ITEMTRADESHOP_CLOSE1, FALSE);
				SetRender(SPR_OBJ_ITEMTRADESHOP_CLOSE2, TRUE);
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(g_ItemMoveManager.GetMouseItemID())
					return TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4:
		{	
			BYTE byChk = 0;

			if(m_bTradeChk==FALSE)
			{
				if(m_bOkChk==FALSE)				
				{
					if(eMouseCheck==MOUSE_OVER || eMouseCheck==MOUSE_UP)
						byChk = __MOUSE_POINTER_BUTTON__;
					else if(eMouseCheck==MOUSE_DOWN)
						byChk = __MOUSE_POINTER_BUTTONCLICK__;
				}
				else
				{
					if(eMouseCheck==MOUSE_OVER || eMouseCheck==MOUSE_UP)
						byChk = __MOUSE_POINTER_DEFAULT__;
					else if(eMouseCheck==MOUSE_DOWN)
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
			}
			else	
			{
				if(eMouseCheck==MOUSE_OVER || eMouseCheck==MOUSE_UP)
					byChk = __MOUSE_POINTER_DEFAULT__;
				else if(eMouseCheck==MOUSE_DOWN)
					byChk = __MOUSE_POINTER_BUTTON__;
			}
			
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_bTradeChk==FALSE)
				{
					if(m_bOkChk==FALSE)
					{
						// 트레이드할 모든 준비가 끝났다. 
						CTDS_TRADE_OK pPacket;
						pPacket.dwMoney = m_dwMoney;
						pPacket.bType	= 0;

						g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);

						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_2, FALSE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_3, TRUE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_4, FALSE);
					}
				}
				else
				{					
					SetActive(FALSE);					
					m_bBtn[2]	= TRUE;				
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_bTradeChk==FALSE)
				{
					if(m_bOkChk==FALSE)
					{					
						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_2, FALSE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_3, FALSE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_4, TRUE);
						m_bBtn[0] = TRUE;				
					}									
				}
				else
				{					
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL1, FALSE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL2, FALSE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL3, TRUE);
					m_bBtn[2]	= TRUE;
				}								
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_bTradeChk==FALSE)
				{
					if(m_bOkChk==FALSE)
					{
						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_2, FALSE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_3, TRUE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_OK1_4, FALSE);												
						m_bBtn[0]	= TRUE;
					}					
				}
				else
				{
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL1, FALSE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL2, TRUE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL3, FALSE);
					m_bBtn[2]	= TRUE;
				}				
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	case 5:
		{
			BYTE byChk = 0;

			if(m_bTradeChk==FALSE)
			{
				if(m_bOkChk==FALSE)				
				{
					if(eMouseCheck==MOUSE_OVER || eMouseCheck==MOUSE_UP)
						byChk = __MOUSE_POINTER_BUTTON__;
					else if(eMouseCheck==MOUSE_DOWN)
						byChk = __MOUSE_POINTER_BUTTONCLICK__;
				}
				else
				{
					if(eMouseCheck==MOUSE_OVER || eMouseCheck==MOUSE_UP)
						byChk = __MOUSE_POINTER_DEFAULT__;
					else if(eMouseCheck==MOUSE_DOWN)
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
			}
			else	
			{
				if(eMouseCheck==MOUSE_OVER || eMouseCheck==MOUSE_UP)
					byChk = __MOUSE_POINTER_DEFAULT__;
				else if(eMouseCheck==MOUSE_DOWN)
					byChk = __MOUSE_POINTER_BUTTON__;
			}
			
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_bTradeChk==FALSE)				
				{
					// Item Trade 취소 //
					SetActive(FALSE);
				}
				else
				{
					if(m_bTradeOneChk==FALSE)
					{
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE2, FALSE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE3, TRUE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE4, FALSE);

						m_bBtn[3]		= TRUE;					

						// Trade 메세지 //						
						CTDS_TRADE_TRADE pItemTrade;
						memset(pItemTrade.szCharacterName, 0, sizeof(pItemTrade.szCharacterName));
						__lstrcpyn(pItemTrade.szCharacterName, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
						
						g_pNet->SendMsg((char*)&pItemTrade, pItemTrade.GetPacketSize(), SERVER_INDEX_ZONE);
					}					
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_bTradeChk==FALSE)
				{					
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL1, FALSE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL2, FALSE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL3, TRUE);
					m_bBtn[1]	= TRUE;
				}
				else
				{
					if(m_bTradeOneChk==FALSE)
					{						
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE2, FALSE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE3, FALSE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE4, TRUE);
						m_bBtn[3] = TRUE;
					}					
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_bTradeChk==FALSE)
				{					
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL1, FALSE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL2, TRUE);
					SetRender(SPR_OBJ_ITEMTRADESHOP_CANCEL3, FALSE);
					m_bBtn[1]	= TRUE;
				}
				else
				{
					if(m_bTradeOneChk==FALSE)
					{						
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE2, FALSE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE3, TRUE);
						SetRender(SPR_OBJ_ITEMTRADESHOP_TRADE4, FALSE);
						m_bBtn[3] = TRUE;
					}					
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	case 6:
		{
			BYTE byChk = 0;			
			
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_bOkChk==FALSE)
				{
					int nPosX	= (int)(g_Mouse.MousePos.x-m_fPosX-5)/35;
					int nPosY	= (int)(g_Mouse.MousePos.y-m_fPosZ-52)/67;
					int nIndex	= nPosY*7+nPosX;

					if(nIndex>=MAX_TRADE)
					{
						return 0;
					}
					
					int nValue = g_pMainPlayer->m_pItemTrade[nIndex].GetID()/ITEM_DISTRIBUTE;

					if(g_pMainPlayer->m_MouseItem.GetID()==0)
					{
						if(g_pMainPlayer->m_pItemTrade[nIndex].GetID()==0)
						{
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;							
						}
						else
						{
							if(	(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)	||
								(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)		||
								nValue==ITEM_GUARDIAN_INDEX || nValue==ITEM_RIDE_INDEX)
							{
								byChk = __MOUSE_POINTER_ITEMCLICK__;
								
								CTDS_ITEM_PICKUP	ItemPickup;
								ItemPickup.bInv			= 48;
								ItemPickup.bSectionNum	= 1;
								ItemPickup.i64ItemID	= 0;
								ItemPickup.bZipCode		= nIndex;				
								g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

								if(g_ItemMoveManager.GetNewItemMoveMode())
								{
									WORD wID = g_pMainPlayer->m_pItemTrade[nIndex].GetID();
									g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_TRADE,nIndex,wID,0,0);
								}
							}
							else
							{
								// 위치 확인 //
								if(	g_Mouse.MousePos.x>=m_fPosX+5+nPosX*35				&&
									g_Mouse.MousePos.x<=nPosX+m_fPosX+5+(nPosX+1)*35	&&
									g_Mouse.MousePos.y>=m_fPosZ+52+nPosY*67				&&
									g_Mouse.MousePos.y<=m_fPosZ+52+nPosY*67+35)
								{
									byChk = __MOUSE_POINTER_ITEMCLICK__;									

									// 아이템 이동 //
									CTDS_ITEM_PICKUP	ItemPickup;
									ItemPickup.bInv			= 49;
									ItemPickup.bSectionNum	= 1;
									ItemPickup.i64ItemID	= 0;
									ItemPickup.bZipCode		= nIndex;				
									g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

									if(g_ItemMoveManager.GetNewItemMoveMode())
									{
										WORD wID = g_pMainPlayer->m_pItemTrade[nIndex].GetID();
										g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_TRADE,nIndex,wID,0,0);
									}
								}
								else
								{
									byChk = __MOUSE_POINTER_DEFAULTCLICK__;
								}
							}						
						}
					}
				}
				else
				{
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;				
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_bOkChk==FALSE)
				{
					int nPosX	= (int)(g_Mouse.MousePos.x-m_fPosX-5)/35;
					int nPosY	= (int)(g_Mouse.MousePos.y-m_fPosZ-52)/67;
					int nIndex	= nPosY*7+nPosX;

					if(nIndex>=MAX_TRADE)
					{
						return 0;
					}
					
					int nValue = g_pMainPlayer->m_pItemTrade[nIndex].GetID()/ITEM_DISTRIBUTE;

					if(g_pMainPlayer->m_pItemTrade[nIndex].GetID()==0)
					{
						byChk = __MOUSE_POINTER_DEFAULT__;						
					}
					else
					{
						if(	(nValue>=ITEM_WEAPONST_INDEX && nValue<=ITEM_WEAPONEND_INDEX)	||
							(nValue>=ITEM_ARMORST_INDEX && nValue<=ITEM_ARMOREND_INDEX)		||
							nValue==ITEM_GUARDIAN_INDEX || nValue==ITEM_RIDE_INDEX)
						{
							byChk = __MOUSE_POINTER_ITEM__;
						}
						else
						{
							// 위치 확인 //
							if(	g_Mouse.MousePos.x>=m_fPosX+5+nPosX*35 &&
								g_Mouse.MousePos.x<=nPosX+m_fPosX+5+(nPosX+1)*35 &&
								g_Mouse.MousePos.y>=m_fPosZ+52+nPosY*67 &&
								g_Mouse.MousePos.y<=m_fPosZ+52+nPosY*67+35)
							{
								byChk = __MOUSE_POINTER_ITEM__;
							}
							else
							{
								byChk = __MOUSE_POINTER_DEFAULT__;
							}
						}
					}
				}
				else
				{
					byChk = __MOUSE_POINTER_DEFAULT__;					
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	case 7:
		{
			BYTE byChk = 0;
			
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_bOkChk==FALSE)
				{
					CChatWnd*		pChatWnd = CChatWnd::GetInstance();
					CQuantityWnd*	pQuantityWnd = CQuantityWnd::GetInstance();

					if(g_pGVDungeon->bChatMode)
						ChatModeOff(INPUT_BUFFER_19);

					if(pChatWnd->GetActive()==TRUE)
						pChatWnd->SetActive(FALSE);					
			
					g_pInputManager->ClearInput(INPUT_BUFFER_19);
					
					// Quantity Wnd //					
					if(pQuantityWnd->GetActive()==FALSE)
					{
						pQuantityWnd->OpenWnd(__QUANTITY_TRADEMONEY__);
					}
					else
					{
						pQuantityWnd->SetActive(FALSE);					
					}

					RECT rt;
					rt.left		= (LONG)pQuantityWnd->m_fPosX+124-7;
					rt.right	= (LONG)pQuantityWnd->m_fPosX+124;
					rt.top		= (LONG)pQuantityWnd->m_fPosZ+83;
					rt.bottom	= (LONG)pQuantityWnd->m_fPosZ+95;
					int nOrder	= pQuantityWnd->GetStartOrder()+2;						

					g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
					g_pInputManager->SetFocusInput(INPUT_BUFFER_5);

					CInterface::GetInstance()->SetWnd(QUANTITY_WND);

					byChk = __MOUSE_POINTER_BUTTON__;
				}
				else
				{
					byChk = __MOUSE_POINTER_DEFAULT__;
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_bOkChk==FALSE)
					byChk = __MOUSE_POINTER_BUTTONCLICK__;					
				else
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;					
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_bOkChk==FALSE)
					byChk = __MOUSE_POINTER_BUTTON__;
				else
					byChk = __MOUSE_POINTER_DEFAULT__;					
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_ITEMTRADESHOP_CLOSE1, TRUE);
		SetRender(SPR_OBJ_ITEMTRADESHOP_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{		
		return CInterface::GetInstance()->GetSetWnd();
	}			

	if(IsReturn(nRt, eMouseCheck))
	{
		return CInterface::GetInstance()->GetSetWnd();		
	}

	return 0;
}

void CItemTradeShopWnd::RenderTrade()
{
	int nOrder	= __ORDER_INTERFACE_START__ + m_byOrder*10;
		
	VECTOR2	vPos;					
		
	for(int i = 0; i < MAX_TRADE*2; i++)
	{
		if(i < MAX_TRADE)
		{
			if(g_pMainPlayer->m_pItemTrade[i].m_wItemID!=0)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_TRADE &&
						i == g_ItemMoveManager.GetNativeSrcIndex())
					{
						continue;
					}
				}

				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
					g_pItemResourceHash->GetData(g_pMainPlayer->m_pItemTrade[i].m_wItemID);

				if(lpItemResourceEx)
				{
					int nPosX	= 7+(i%7)*35;
					int	nPosY	= 52+(i/7)*67;

					vPos.x	= m_fPosX+nPosX;
					vPos.y	= m_fPosZ+nPosY;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}
			}
		}
		else
		{
			if(m_pTradeItem[i-MAX_TRADE].m_wItemID!=0)
			{				
				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
					g_pItemResourceHash->GetData(m_pTradeItem[i-MAX_TRADE].m_wItemID);

				if(lpItemResourceEx)
				{
					int nPosX	= 263+((i-MAX_TRADE)%7)*35;
					int	nPosY	= 52+((i-MAX_TRADE)/7)*67;

					vPos.x	= m_fPosX+nPosX;
					vPos.y	= m_fPosZ+nPosY;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}
			}
		}
	}								
}
//======================================================//
// End.													//
//======================================================//	
