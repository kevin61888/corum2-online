//======================================================//./
// Code by Jang.							2002.12.23	//
//======================================================//
#include "PlayerShopWnd.h"
#include "GameControl.h"
#include "HashTable.h"
#include "ItemWnd.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "DungeonTable.h"
#include "StoreWnd.h"
#include "ItemTradeShopWnd.h"
#include "Chat.h"
#include "QuantityWnd.h"
#include "InputManager.h"
#include "ChatBackground.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "Filter.h"
#include "CodeFun.h"
#include "ChatWnd.h"


CPlayerShopWnd* CPlayerShopWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CPlayerShopWnd::CPlayerShopWnd()
{	
	m_bInputMode	= 1;
	m_byMode		= 0;
	m_bChkMouse		= 0;
}

CPlayerShopWnd::~CPlayerShopWnd()
{
	
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CPlayerShopWnd::Init()
{
/*
	InsertData(SPR_OBJ_PLAYER_SHOP_WND, SPR_INTERFACE_PLAYER_SHOP_WND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(BUTTON_OBJ_PLAYERSHOP_EXIT1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_PLAYERSHOP_EXIT2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_PLAYERSHOP_OPEN1, BUTTON_INTERFACE_PLAYERSHOP_OPEN1, 121, 218, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_PLAYERSHOP_OPEN2, BUTTON_INTERFACE_PLAYERSHOP_OPEN2, 121, 218, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_PLAYERSHOP_OPEN3, BUTTON_INTERFACE_PLAYERSHOP_OPEN3, 121, 218, 1.0f, 1.0f, 1);	
	InsertData(BUTTON_OBJ_PLAYERSHOP_CLOSE1, BUTTON_INTERFACE_PLAYERSHOP_CLOSE1, 121, 218, 1.0f, 1.0f, 1);	
	InsertData(BUTTON_OBJ_PLAYERSHOP_CLOSE2, BUTTON_INTERFACE_PLAYERSHOP_CLOSE2, 121, 218, 1.0f, 1.0f, 1);	
	InsertData(BUTTON_OBJ_PLAYERSHOP_CLOSE3, BUTTON_INTERFACE_PLAYERSHOP_CLOSE3, 121, 218, 1.0f, 1.0f, 1);		
	InsertData(BUTTON_OBJ_PLAYERSHOPWND_CANCEL1, BUTTON_INTERFACE_PLAYERSHOPWND_CANCEL1, 186, 218, 1.0f, 1.0f, 1);	
	InsertData(BUTTON_OBJ_PLAYERSHOPWND_CANCEL2, BUTTON_INTERFACE_PLAYERSHOPWND_CANCEL2, 186, 218, 1.0f, 1.0f, 1);	
	InsertData(BUTTON_OBJ_PLAYERSHOPWND_CANCEL3, BUTTON_INTERFACE_PLAYERSHOPWND_CANCEL3, 186, 218, 1.0f, 1.0f, 1);		

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(8, 68, 248, 222, 4, CHECK_ITEM);
	InsertCheckInterface(121, 218, 185, 234, 5, CHECK_BTN);
	InsertCheckInterface(188, 218, 252, 234, 6, CHECK_BTN);
	InsertCheckInterface(0, 25, 252, 65, 7, CHECK_BTN);
	*/
	m_bInit	= TRUE;
	
	for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
	{
		m_sPlayerShopInfo.m_dwCustomSellPrice[i]	= 0;
		memset(&m_sPlayerShopInfo.cItem, 0, sizeof(CItem));
	}

	memset(m_sPlayerShopInfo.szTitle, 0, sizeof(m_sPlayerShopInfo.szTitle));

	m_byMode = 0;
	
	OnMove();
	return TRUE;
}


void CPlayerShopWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}


void CPlayerShopWnd::SetOrder()
{	
	g_pInputManager->SetInputOrder(INPUT_BUFFER_2, GetStartOrder()+2);

#ifdef _USE_IME

	if(g_pMainPlayer!=m_pOwnerPlayerShop)
		return;	
	if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_SHOP)
		return;

	if(GetActive() && g_pGVDungeon->bChatMode)
	{
		POINT ptIME;
		ptIME.x = (LONG)m_fPosX+65;
		ptIME.y = (LONG)m_fPosZ+29;	
		GET_IMEEDIT()->ActivateIME(ptIME, sizeof(m_sPlayerShopInfo.szTitle) - 1 - 12, GetStartOrder()+6);
	}
	
#endif
}

void CPlayerShopWnd::RenderText()
{
	int nLen	= 0;

	memset(m_szInfo, 0, sizeof(m_szInfo));
	
	for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
	{
		if(m_pOwnerPlayerShop == g_pMainPlayer)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_PLAYERSHOP &&
					i == g_ItemMoveManager.GetNativeSrcIndex())
				{
					continue;
				}
			}

			int nValue	= g_pMainPlayer->m_sPlayerShop.cItem[i].GetID()/ITEM_DISTRIBUTE;
			
			if(GetType((WORD)nValue)==ITEM_SMALL)
			{
				// 수량 //
				wsprintf(m_szInfo, "%u", g_pMainPlayer->m_sPlayerShop.cItem[i].GetQuantity());
				nLen = lstrlen(m_szInfo);
			}
			else
			{
				continue;
			}
		}
		else
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_PLAYERSHOP &&
					i == g_ItemMoveManager.GetNativeSrcIndex())
				{
					continue;
				}
			}

			int nValue	= m_sPlayerShopInfo.cItem[i].GetID()/ITEM_DISTRIBUTE;
			
			if(GetType((WORD)nValue)==ITEM_SMALL)
			{
				// 수량 //
				wsprintf(m_szInfo, "%u", m_sPlayerShopInfo.cItem[i].GetQuantity());
				nLen = lstrlen(m_szInfo);
			}
			else
			{
				continue;
			}
		}
		
		// 좌측 상단 좌표 //
		int nPosX = (int)(m_fPosX+8)+(i%5)*52;
		int nPosY = (int)(m_fPosZ+68)+(i/5)*71;

		if(!IsEmptyString(m_szInfo))
			RenderFont(m_szInfo, nPosX+30-nLen*7, nPosX+32, nPosY+20, nPosY+34, GetStartOrder()+4);
	}

	if(m_pOwnerPlayerShop!=g_pMainPlayer)
	{			
		int nPosX = (int)m_fPosX+65;
		int nPosY = (int)m_fPosZ+29;

		nLen = lstrlen(m_sPlayerShopInfo.szTitle);
		
		if(!IsEmptyString(m_sPlayerShopInfo.szTitle))
			RenderFont(m_sPlayerShopInfo.szTitle, nPosX, nPosX+nLen*7, nPosY, nPosY+14, GetStartOrder()+4);
	}
	
	ItemInfoRender();
	RenderItemPlayerShop();
}


void CPlayerShopWnd::ItemInfoRender()
{	
	CItem*	pItem;			
	int		nIndex	= 0;
	int		nPosX	= 0;
	int		nPosY	= 0;
	DWORD	dwPrice	= 0;
	
	if(g_Mouse.bLDown==TRUE)
		return;

	if(	g_Mouse.MousePos.x>=m_fPosX+8	&&
		g_Mouse.MousePos.x<=m_fPosX+250 &&
		g_Mouse.MousePos.y>=m_fPosZ+68	&&
		g_Mouse.MousePos.y<=m_fPosZ+203 )
	{
		nPosX	= (int)g_Mouse.MousePos.x-(int)m_fPosX-8;
		nPosY	= (int)g_Mouse.MousePos.y-(int)m_fPosZ-68;
		nIndex	= (nPosX/52)+(nPosY/71)*5;

		if((nPosX%52<=32) && (nPosY%71<=64))
		{		
			if(g_pMainPlayer == m_pOwnerPlayerShop)
			{
				pItem	= &g_pMainPlayer->m_sPlayerShop.cItem[nIndex];
				dwPrice	= g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[nIndex];
			}
			else
			{
				pItem	= &m_sPlayerShopInfo.cItem[nIndex];
				dwPrice	= m_sPlayerShopInfo.m_dwCustomSellPrice[nIndex];
			}

			if(!pItem)
				return;
			
			if(pItem->GetID() == 0)
				return;

			if(pItem->GetType()==ITEM_LARGE)
			{
							
				if(nIndex>=MAX_PLAYER_SHOP_INV)					
					return;
				else if(CUserInterface::GetInstance()->InterfaceCollision(PLAYERSHOP_WND
					, (int)m_fPosX+(nIndex%5)*52+8
					, (int)m_fPosX+((nIndex%5)+1)*52+8
					, (int)m_fPosZ+(nIndex/5)*71+68
					, (int)m_fPosZ+((nIndex/5)+1)*71+68)==FALSE)
					return;
				else if(pItem->GetID()==0)
					return;									
			}
			else
			{
				if(!((nPosX%52<=32) && (nPosY%71<=32)))
					return;
				else if(nIndex>=MAX_PLAYER_SHOP_INV)
					return;
				else if(CUserInterface::GetInstance()->InterfaceCollision(PLAYERSHOP_WND
					, (int)m_fPosX+(nIndex%5)*52+8
					, (int)m_fPosX+((nIndex%5)+1)*52+8
					, (int)m_fPosZ+(nIndex/5)*71+68
					, (int)m_fPosZ+(nIndex/5)*71+32+68)==FALSE)
					return;
				else if(pItem->GetID()==0)
					return;					
			}

			CInterface::GetInstance()->ItemInfoRender(pItem, FALSE, TRUE, dwPrice);
		}	
	}	
}


void CPlayerShopWnd::RenderItemPlayerShop()
{
	int nOrder	= __ORDER_INTERFACE_START__ + m_byOrder*10;
	
	VECTOR2	vPos;					

	for(int i = 0; i < MAX_PLAYER_SHOP_INV; i++)
	{
		// 내꺼 
		if(m_pOwnerPlayerShop==g_pMainPlayer)
		{
			if(g_pMainPlayer->m_sPlayerShop.cItem[i].m_wItemID!=0)
			{	
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_PLAYERSHOP &&
						i == g_ItemMoveManager.GetNativeSrcIndex())
					{
						continue;
					}
				}

				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
					g_pItemResourceHash->GetData(g_pMainPlayer->m_sPlayerShop.cItem[i].m_wItemID);
						
				if(lpItemResourceEx)
				{
					int nPosX	= 8+(i%5)*52;
					int	nPosY	= 68+(i/5)*71;
								
					vPos.x	= m_fPosX+nPosX;
					vPos.y	= m_fPosZ+nPosY;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}				
			}						
		}
		else
		{
			// 다른 유저꺼 
			if(m_sPlayerShopInfo.cItem[i].m_wItemID!=0)
			{	
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_PLAYERSHOP &&
						i == g_ItemMoveManager.GetNativeSrcIndex())
					{
						continue;
					}
				}

				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
					g_pItemResourceHash->GetData(m_sPlayerShopInfo.cItem[i].m_wItemID);
						
				if(lpItemResourceEx)
				{
					int nPosX	= 8+(i%5)*52;
					int	nPosY	= 68+(i/5)*71;
								
					vPos.x	= m_fPosX+nPosX;
					vPos.y	= m_fPosZ+nPosY;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}
			}
		}
	}
}

void CPlayerShopWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive == FALSE)
	{	
		ShowSpriteAll();
		SetInputMode(0);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{				
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		// 자신이 열었구나..
		if (m_pOwnerPlayerShop == g_pMainPlayer)	
		{
			if(g_pMainPlayer->m_bPlayerShop)
				SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE1, TRUE);	
			else
				SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN1, TRUE);

			SetRender(BUTTON_OBJ_PLAYERSHOPWND_CANCEL1, TRUE);

			SetInputMode(m_bInputMode);
			
			CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

			if(pQuantityWnd->GetActive())
			{
				// "노점에 아이템 이동중에는 쓰기가 안됩니다."
				DisplayMessageAdd(g_Message[ETC_MESSAGE1041].szMessage, 0xFFFFC309);			
			}
			else
			{
				// 내껏만 관리는 할수 있으니께
				if(g_pMainPlayer==m_pOwnerPlayerShop)
				{
					if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PLAYER_SHOP)
						SetInputModeChk();					
				}
			}
		}
		else
		{
			SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN1, FALSE);
			SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN2, FALSE);
			SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN3, FALSE);
			SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE1, FALSE);
			SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE2, FALSE);
			SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE3, FALSE);
			SetInputMode(0, FALSE);
		}

		SetRender(SPR_OBJ_PLAYER_SHOP_WND, TRUE);
		
		RECT rt = {long(m_fPosX+65), long(m_fPosZ+29), long(m_fPosX+256), long(m_fPosZ+43)};
		g_pInputManager->InitializeInput(INPUT_BUFFER_2, FONT_SS3D_GAME, rt, 0xffffffff, GetStartOrder()+1);
		g_pInputManager->InsertCharacter(INPUT_BUFFER_2, m_sPlayerShopInfo.szTitle, lstrlen(m_sPlayerShopInfo.szTitle));						

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CPlayerShopWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);

	if(!m_bUpChk)
	{
		// 마우스 오버를 위한 초기화..
		if (g_pMainPlayer == m_pOwnerPlayerShop)
		{
			if (g_pMainPlayer->m_bPlayerShop)
				SetButtonStatus(BUTTON_OBJ_PLAYERSHOP_CLOSE1, 0);		
			else
				SetButtonStatus(BUTTON_OBJ_PLAYERSHOP_OPEN1, 0);		

			SetButtonStatus(BUTTON_OBJ_PLAYERSHOPWND_CANCEL1, 0);		
		}		
	}
			
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
				SetRender(BUTTON_OBJ_PLAYERSHOP_EXIT1, FALSE);
				SetRender(BUTTON_OBJ_PLAYERSHOP_EXIT2, TRUE);
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

			if(eMouseCheck==MOUSE_DOWN)
			{
				//=====================================================================================================
				// 내꺼 
				//=====================================================================================================
				if(g_pMainPlayer == m_pOwnerPlayerShop)
				{
					// 요 이프문 뉴아이템 무브에서는 바꿔야 한다. 지금은 기냥 냅둔다.
					if(g_pMainPlayer->m_MouseItem.GetID()==0)				
					{
						int nPosX	= g_Mouse.MousePos.x-(int)m_fPosX-8;
						int nPosY	= g_Mouse.MousePos.y-(int)m_fPosZ-68;

						if((nPosX%52<=32) && (nPosY%71<=64))
						{
							int nIndex = (nPosX/52)+(nPosY/71)*5;
														
							if(g_pMainPlayer->m_sPlayerShop.cItem[nIndex].GetID()!=0)
							{
								// 메세지 이동 //
								int nValue = g_pMainPlayer->m_sPlayerShop.cItem[nIndex].GetID()/ITEM_DISTRIBUTE;

								if(nIndex < MAX_PLAYER_SHOP_INV)
								{									
									if(GetType((WORD)nValue)==ITEM_LARGE)
									{
										CTDS_ITEM_PICKUP ItemPickup;
										ItemPickup.bSectionNum	= 1;
										ItemPickup.i64ItemID	= 0;
										ItemPickup.bInv			= 58;
										ItemPickup.bZipCode		= (BYTE)nIndex;
										ItemPickup.dwMoney		= g_pMainPlayer->m_sPlayerShop.m_dwCustomSellPrice[nIndex];
										g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

										if( g_ItemMoveManager.GetNewItemMoveMode() )
										{
											WORD wID = g_pMainPlayer->m_sPlayerShop.cItem[nIndex].m_wItemID;
											g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_PLAYERSHOP,(BYTE)nIndex,wID,0,0);
										}
									}
									else if(GetType((WORD)nValue)==ITEM_SMALL)
									{
										if((nPosX%52<=32) && (nPosY%71<=32))
										{
											// 스몰 아이템 이동 //
											CTDS_ITEM_PICKUP ItemPickup;
											ItemPickup.bSectionNum	= 1;
											ItemPickup.i64ItemID	= 0;
											ItemPickup.bInv			= 60;
											ItemPickup.bZipCode		= (BYTE)nIndex;
											g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);											

											if( g_ItemMoveManager.GetNewItemMoveMode() )
											{
												WORD wID = g_pMainPlayer->m_sPlayerShop.cItem[nIndex].m_wItemID;
												g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_PLAYERSHOP,(BYTE)nIndex,wID,0,0);
											}
										}										
										else
										{
											byChk = __MOUSE_POINTER_DEFAULTCLICK__;
										}
									}
									else
									{
										byChk = __MOUSE_POINTER_DEFAULTCLICK__;
									}
								}
								else
								{
									byChk = __MOUSE_POINTER_DEFAULTCLICK__;
								}
							}
							else
							{
								byChk = __MOUSE_POINTER_DEFAULTCLICK__;
							}
						}
						else
						{
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
						}
					}				
					else
					{
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
				}
				else if(g_pMainPlayer != m_pOwnerPlayerShop)
				{
					//=====================================================================================================
					// 다른 유저꺼 
					//=====================================================================================================
					int nPosX	= g_Mouse.MousePos.x-(int)m_fPosX-8;
					int nPosY	= g_Mouse.MousePos.y-(int)m_fPosZ-68;					
					int nIndex	= (nPosX/52)+(nPosY/71)*5;
					int nValue	= m_sPlayerShopInfo.cItem[nIndex].GetID()/ITEM_DISTRIBUTE;

					if(nIndex >= MAX_PLAYER_SHOP_INV)
					{
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
					else
					{	
						m_bZipCode = (BYTE)nIndex;

						if(m_sPlayerShopInfo.cItem[nIndex].GetID()!=0)
						{
							if((nPosX%52<=32) && (nPosY%71<=64))
							{											
								if(GetType((WORD)nValue)==ITEM_LARGE)
								{
									CStoreWnd::GetInstance()->m_wSuppliesId	= m_sPlayerShopInfo.cItem[nIndex].GetID();
									CStoreWnd::GetInstance()->m_bSupplies	= TRUE;								
									g_pMainPlayer->m_nItemSelect = 1;
									m_bChkMouse	= 1;
									
									if(g_ItemMoveManager.GetNewItemMoveMode())
									{
										g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_PLAYERSHOP
											, (BYTE)nIndex
											, m_sPlayerShopInfo.cItem[nIndex].GetID()
											, 0
											, m_pOwnerPlayerShop->m_dwUserIndex);
									}
								}
								else if(GetType((WORD)nValue)==ITEM_SMALL)
								{
									if((nPosX%52<=32) && (nPosY%71<=32))
									{
										CStoreWnd::GetInstance()->m_wSuppliesId	= m_sPlayerShopInfo.cItem[nIndex].GetID();
										CStoreWnd::GetInstance()->m_bSupplies	= TRUE;								
										g_pMainPlayer->m_nItemSelect = 1;
										m_bChkMouse	= 1;
										m_bZipCode	= (BYTE)nIndex;

										if(g_ItemMoveManager.GetNewItemMoveMode())
										{
											g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_PLAYERSHOP
												, (BYTE)nIndex
												, m_sPlayerShopInfo.cItem[nIndex].GetID()
												, 0
												, m_pOwnerPlayerShop->m_dwUserIndex);
										}
									}
								}
								else
								{
									byChk = __MOUSE_POINTER_DEFAULTCLICK__;									
								}
							}
							else
							{
								byChk = __MOUSE_POINTER_DEFAULTCLICK__;
							}
						}
						else
						{
							byChk = __MOUSE_POINTER_DEFAULTCLICK__;
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
				int nPosX	= g_Mouse.MousePos.x-(int)m_fPosX-8;
				int nPosY	= g_Mouse.MousePos.y-(int)m_fPosZ-68;

				if((nPosX%52<=32) && (nPosY%71<=64))
				{
					int nIndex = (nPosX/52)+(nPosY/71)*5;

					//=====================================================================================================
					// 내꺼 
					//=====================================================================================================
					if (g_pMainPlayer == m_pOwnerPlayerShop)
					{
						if(g_pMainPlayer->m_sPlayerShop.cItem[nIndex].GetID()!=0)
						{
							int nValue = g_pMainPlayer->m_sPlayerShop.cItem[nIndex].GetID()/ITEM_DISTRIBUTE;
														
							if(GetType((WORD)nValue)==ITEM_SMALL)
							{
								if(!((nPosX%52<=32) && (nPosY%71<=32)))									
									byChk = __MOUSE_POINTER_DEFAULT__;
							}
							else
							{
//								byChk = __MOUSE_POINTER_DEFAULT__;
							}
						}
						else
						{
							byChk = __MOUSE_POINTER_DEFAULT__;
						}
					}
					else
					{
						//=====================================================================================================
						// 다른 유저꺼 
						//=====================================================================================================
						if(m_sPlayerShopInfo.cItem[nIndex].GetID()!=0)
						{
							int nValue = m_sPlayerShopInfo.cItem[nIndex].GetID()/ITEM_DISTRIBUTE;
														
							if(GetType((WORD)nValue)==ITEM_SMALL)
							{
								if(!((nPosX%52<=32) && (nPosY%71<=32)))
								{
									byChk = __MOUSE_POINTER_DEFAULT__;									
								}
							}
						}
						else
						{
							byChk = __MOUSE_POINTER_DEFAULT__;
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
	case 5:
		{			
			// 노점상 개점 //
			if(eMouseCheck==MOUSE_DOWN)
			{
				if (g_pMainPlayer == m_pOwnerPlayerShop)
				{
#ifdef _USE_IME
					if(GET_IMEEDIT()->IsActivated())
					{
						g_pInputManager->ClearInput(INPUT_BUFFER_2);
						g_pInputManager->InsertCharacter(INPUT_BUFFER_2, (LPSTR)GET_IMEEDIT()->GetImeEditString()
							, lstrlen(GET_IMEEDIT()->GetImeEditString()));
						GET_IMEEDIT()->DeactivateIME();

#if IS_CHINA_LOCALIZING()
						if(CheckString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_2)))
						{
							g_pInputManager->ClearInput(INPUT_BUFFER_2);
						}
#endif
					}
#endif					
					if(m_byMode==0)
						SetButtonStatus(BUTTON_OBJ_PLAYERSHOP_OPEN1, 2);
					else
						SetButtonStatus(BUTTON_OBJ_PLAYERSHOP_CLOSE1, 2);					
				}				
			}
			else if(eMouseCheck==MOUSE_UP)
			{
				if (g_pMainPlayer==m_pOwnerPlayerShop)
				{
					if(m_byMode==0)
					{	
						if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PLAYER_SHOP)
						{
							CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

							if(pItemTradeShopWnd->GetActive()==FALSE)
							{
								if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PORTAL_MOVING
									&& g_pMainPlayer->GetStatus()!=UNIT_STATUS_DEAD)
								{
									CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

									if(pQuantityWnd->GetActive()==FALSE)
									{
										if(pQuantityWnd->m_bReq==FALSE)
										{
											if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_REST)
											{
												// "앉기중에는 사용할 수 없습니다."
												DisplayMessageAdd(g_Message[ETC_MESSAGE797].szMessage, 0xFFFF0000); 
											}
											else
											{
												PlayerShopMode(1);
											}
										}
									}
								}
								else
								{
									if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PORTAL_MOVING)
									{
										// "포탈중에는 노점을 열기를 할수 없습니다."
										DisplayMessageAdd(g_Message[ETC_MESSAGE770].szMessage, 0xFFFFC309); 
									}
									else if(g_pMainPlayer->GetStatus()==UNIT_STATUS_DEAD)
									{
										// "죽었에 때는 노점을 열기를 할수 없습니다."
										DisplayMessageAdd(g_Message[ETC_MESSAGE771].szMessage, 0xFFFFC309); 
									}
								}
							}
							else
							{
								// "교환중에는 노점을 열기를 할수 없습니다."
								DisplayMessageAdd(g_Message[ETC_MESSAGE772].szMessage, 0xFFFFC309); 
							}
						}
					}
					else
					{
						if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_SHOP)
						{
							PlayerShopMode(0);
						}
					}

					if(m_byMode==0)
						SetButtonStatus(BUTTON_OBJ_PLAYERSHOP_OPEN1, 1);
					else
						SetButtonStatus(BUTTON_OBJ_PLAYERSHOP_CLOSE1, 1);
				}				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(g_pMainPlayer==m_pOwnerPlayerShop)
				{				
					if(m_byMode==0)
						SetButtonStatus(BUTTON_OBJ_PLAYERSHOP_OPEN1, 1);
					else
						SetButtonStatus(BUTTON_OBJ_PLAYERSHOP_CLOSE1, 1);
				}
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:		// 창 닫아.
		{				
			if(eMouseCheck==MOUSE_UP)
			{	
				if(g_pMainPlayer==m_pOwnerPlayerShop)
					SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)		
			{	
				if(g_pMainPlayer==m_pOwnerPlayerShop)
					SetButtonStatus(BUTTON_OBJ_PLAYERSHOPWND_CANCEL1, 2);					
			}
			else if(eMouseCheck==MOUSE_OVER)		
			{					
				if(g_pMainPlayer == m_pOwnerPlayerShop)
					SetButtonStatus(BUTTON_OBJ_PLAYERSHOPWND_CANCEL1, 1);
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 7:		// 제목란
		{						
			if(eMouseCheck==MOUSE_DOWN)				
			{			
				if(g_pMainPlayer->m_MouseItem.GetID()==0)
				{
					CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

					if(pQuantityWnd->GetActive())
					{
						//"노점에 아이템 이동중에는 쓰기가 안됩니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE1041].szMessage, 0xFFFFC309);			
					}
					else
					{
						// 내껏만 관리는 할수 있으니께
						if(g_pMainPlayer == m_pOwnerPlayerShop)
						{
							if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PLAYER_SHOP)
							{
								SetInputModeChk();
							}
						}
					}				
				}				
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;		
	}		
	
	if(MouseUp())
	{
		SetRender(BUTTON_OBJ_PLAYERSHOP_EXIT1, TRUE);
		SetRender(BUTTON_OBJ_PLAYERSHOP_EXIT2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
		OnMove();
		return CInterface::GetInstance()->GetSetWnd();
	}
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

void CPlayerShopWnd::SetInputMode(BYTE bInputMode, BOOL bClear)
{
	g_pInputManager->GetInputBuffer(INPUT_BUFFER_2);
	
	if(!bInputMode)
	{
		g_pGVDungeon->bChatMode = FALSE;

#ifdef _USE_IME
		if(GET_IMEEDIT()->IsActivated())
		{
			//g_pInputManager->ClearInput(INPUT_BUFFER_2);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_2, 
											(LPSTR)GET_IMEEDIT()->GetImeEditString(),
											lstrlen(GET_IMEEDIT()->GetImeEditString()));

			GET_IMEEDIT()->DeactivateIME();
			//GET_IMEEDIT()->SetEditIndex(0);

#if IS_CHINA_LOCALIZING()
			if(CheckString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_2)))
			{
				g_pInputManager->ClearInput(INPUT_BUFFER_2);
			}
#endif
			if(IsChatEditBackgroundShowed()) 
				ShowChatEditBackground(FALSE);

			CChatWnd* pChatWnd = CChatWnd::GetInstance();
			
			if(pChatWnd->GetActive()==TRUE)
				pChatWnd->SetActive(FALSE);
		}
#endif
		
		if(g_pMainPlayer!=m_pOwnerPlayerShop && bClear)				
		{
			g_pInputManager->ClearInput(INPUT_BUFFER_2);					
			memset(m_sPlayerShopInfo.szTitle, 0, sizeof(m_sPlayerShopInfo.szTitle));
		}
		if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_PLAYER_SHOP)
		{
			if(bClear)
			{
				g_pInputManager->ClearInput(INPUT_BUFFER_2);
				memset(m_sPlayerShopInfo.szTitle, 0, sizeof(m_sPlayerShopInfo.szTitle));			
			}
			else
			{
//				lstrcpy(m_sPlayerShopInfo.szTitle, g_pInputManager->GetInputBuffer(INPUT_BUFFER_2));
			}
		}
		if(g_pMainPlayer==m_pOwnerPlayerShop && bClear)
		{
			g_pInputManager->ClearInput(INPUT_BUFFER_2);					
			memset(m_sPlayerShopInfo.szTitle, 0, sizeof(m_sPlayerShopInfo.szTitle));
		}
		
		ChatModeOff(INPUT_BUFFER_2, 0, FALSE, FALSE);
	}
	else
	{						
		IMESetEdit(0, FALSE);

		ChatModeOn(INPUT_BUFFER_2, FALSE);

#ifdef _USE_IME
		
		POINT ptIME;
		ptIME.x = (LONG)m_fPosX+65;
		ptIME.y = (LONG)m_fPosZ+29;
		int nOrder = __ORDER_INTERFACE_START__ + m_byOrder*10+6;
		//g_pInputManager->GetInputOrder(INPUT_BUFFER_2)+1

		GET_IMEEDIT()->ActivateIME(ptIME, sizeof(m_sPlayerShopInfo.szTitle)- 1 - 12, nOrder);
#endif		
		g_nChatModeExt = 1;
	}	
	
	m_bInputMode = bInputMode;
}

BYTE CPlayerShopWnd::GetInputMode()
{
	return m_bInputMode;
}

void CPlayerShopWnd::OnMove()
{
#ifdef _USE_IME
	GET_IMEEDIT()->SetEditPos((LONG)m_fPosX+65, (LONG)m_fPosZ+29);
#endif
	
	g_pInputManager->GetInputRect(INPUT_BUFFER_2).left		= (LONG)m_fPosX+65;
	g_pInputManager->GetInputRect(INPUT_BUFFER_2).right		= (LONG)m_fPosX+256;
	g_pInputManager->GetInputRect(INPUT_BUFFER_2).top		= (LONG)m_fPosZ+29;
	g_pInputManager->GetInputRect(INPUT_BUFFER_2).bottom	= (LONG)m_fPosZ+43;	
}

void CPlayerShopWnd::SetButtonStatus(DWORD dwSprID, DWORD dwStatus)
{
	switch(dwStatus)
	{
	case 0://nomal
		{
			SetRender(dwSprID, TRUE);
			SetRender(dwSprID+1, FALSE);
			SetRender(dwSprID+2, FALSE);
		}break;
	case 1://mouseover
		{
			SetRender(dwSprID, FALSE);
			SetRender(dwSprID+1, TRUE);
			SetRender(dwSprID+2, FALSE);
		}break;
	case 2://mousedown
		{
			SetRender(dwSprID, FALSE);
			SetRender(dwSprID+1, FALSE);
			SetRender(dwSprID+2, TRUE);
		}break;
	}
}

void CPlayerShopWnd::PlayerShopStart()
{
	SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN1, FALSE);
	SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN2, FALSE);
	SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN3, FALSE);
	SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE1, TRUE);
	g_pMainPlayer->SetPlayerShop(1);		
}

void CPlayerShopWnd::PlayerShopMode(BYTE bMode)
{
	if (bMode)
	{
		// Open //
		g_pMainPlayer->m_hPlayer.pDesc->ObjectFunc = NULL;
		SendStopPacket();			
		
		CTDS_PLAYERSHOP_OPEN packet;
		memset(packet.szTitle, 0, sizeof(packet.szTitle));
		memset(g_pMainPlayer->m_sPlayerShop.szTitle, 0, sizeof(g_pMainPlayer->m_sPlayerShop.szTitle));
		memset(m_sPlayerShopInfo.szTitle, 0, sizeof(m_sPlayerShopInfo.szTitle));

		lstrcpy(packet.szTitle, g_pInputManager->GetInputBuffer(INPUT_BUFFER_2));						
		lstrcpy(g_pMainPlayer->m_sPlayerShop.szTitle, g_pInputManager->GetInputBuffer(INPUT_BUFFER_2));		
		lstrcpy(m_sPlayerShopInfo.szTitle, g_pInputManager->GetInputBuffer(INPUT_BUFFER_2));				
		
		g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);

		SetInputMode(0, FALSE);		
	}
	else
	{
		g_byStatusMessenger	= 0;
						
		// 패킷 보내기 //			
		CTWS_MESSENGER_STATUS pPacket;
		pPacket.byType		= 0;
		pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
		g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);

		// Close //
		SetRender(BUTTON_OBJ_PLAYERSHOP_OPEN1, TRUE);
		SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE1, FALSE);
		SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE2, FALSE);
		SetRender(BUTTON_OBJ_PLAYERSHOP_CLOSE3, FALSE);

		g_pMainPlayer->SetPlayerShop(0);
				
		CTDS_PLAYERSHOP_CLOSE packet;
		g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
		m_byMode = bMode;		
	}	
}


void CPlayerShopWnd::SetInputModeChk()
{
	if(GetInputMode()==1)
		SetInputMode(0, FALSE);
	else
		SetInputMode(1);
}