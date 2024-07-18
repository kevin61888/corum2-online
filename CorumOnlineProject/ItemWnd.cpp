//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "ItemWnd.h"
#include "GameControl.h"
#include "../CommonServer/CommonHeader.h"
#include "HashTable.h"
#include "../CommonServer/AbyssHash.h"
#include "DefResource.h"
#include "Define.h"
#include "UserInterface.h"
#include "Interface.h"
#include "QuantityWnd.h"
#include "ChatWnd.h"
#include "StoreWnd.h"
#include "GuildExWnd.h"
#include "GuildRankWnd.h"
#include "BankWnd.h"
#include "GuildEditWnd.h"
#include "ItemTradeShopWnd.h"
#include "message.h"
#include "DungeonTable.h"
#include "InputManager.h"
#include "Chat.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"


CItemWnd* CItemWnd::c_pThis = NULL;


//======================================================//
// Construction/Destrution.								//
//======================================================//
CItemWnd::CItemWnd()
{	
	m_byOrder		= 0;
	m_byItemChk		= 0;
	m_bSoundChk		= FALSE;
	m_fPosX			= 128;
	m_fPosZ			= 128;
	m_fPrevPosX		= 128;
	m_fPrevPosZ		= 128;
	m_bItemKeyChk	= FALSE;
	m_bMouseIcon	= FALSE;
	m_bDownChk		= FALSE;
	m_bActive		= FALSE;
	m_bClick		= FALSE;
	m_bUp			= FALSE;
	m_bDown			= FALSE;	
	m_bUpChk		= FALSE;
	m_bClose		= FALSE;	
	m_dwCurTime		= 0;
	m_dwPrvTime		= 0;
	m_pLineSpr		= NULL;
	m_pBoxSpr		= NULL;
	m_pCureentSpr	= NULL;
	m_sItemChkType.nItemIndex		= -1;
	m_sItemChkType.byItemZipCode	= 0;
}

CItemWnd::~CItemWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CItemWnd::Init()
{
/*
	InsertData(SPR_OBJ_ITEMWINDOW, SPR_INTERFACE_ITEMWINDOW, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_ITEMWINDOW2, SPR_INTERFACE_ITEMWINDOW2, 0, 512, 1.0f, 1.0f, 0);
	
	InsertData(SPR_OBJ_ITEM_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEM_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEM_CHK1, SPR_INTERFACE_ITEM_CHK1, 6, 251, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEM_CHK2, SPR_INTERFACE_ITEM_CHK2, 6, 251, 1.0f, 1.0f, 1);

#if IS_JAPAN_LOCALIZING()

	InsertData(SPR_OBJ_MONEY_BUTTON1, SPR_INTERFACE_MONEY1, 23, 492, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_MONEY_BUTTON2, SPR_INTERFACE_MONEY2, 23, 492, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_MONEY_BUTTON3, SPR_INTERFACE_MONEY3, 23, 492, 1.0f, 1.0f, 1);

#else

	InsertData(SPR_OBJ_MONEY_BUTTON1, SPR_INTERFACE_MONEY1, 23, 491, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_MONEY_BUTTON2, SPR_INTERFACE_MONEY2, 23, 491, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_MONEY_BUTTON3, SPR_INTERFACE_MONEY3, 23, 491, 1.0f, 1.0f, 1);

#endif
	
	InsertData(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, SPR_INTERFACE_ITEMWND_GUARDIAN_OFF1, 1, 594, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMWND_GUARDIAN_OFF2, SPR_INTERFACE_ITEMWND_GUARDIAN_OFF2, 1, 594, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMWND_GUARDIAN_ON1, SPR_INTERFACE_ITEMWND_GUARDIAN_ON1, 1, 594, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_ITEMWND_GUARDIAN_ON2, SPR_INTERFACE_ITEMWND_GUARDIAN_ON2, 1, 594, 1.0f, 1.0f, 2);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(21, 254, 63, 268, 4, CHECK_BTN);
	InsertCheckInterface(76, 254, 118, 268, 5, CHECK_BTN);
	InsertCheckInterface(6, 278, 244, 482, 6, CHECK_ITEM);
	InsertCheckInterface(24, 492, 231, 505, 7, CHECK_BTN);
	InsertCheckInterface(6, 44, 244, 248, 8, CHECK_ITEM);
	InsertCheckInterface(2, 528, 34, 592, 9, CHECK_BTN);
	InsertCheckInterface(1, 594, 35, 607, 10, CHECK_BTN);
	*/

	m_pLineSpr			= g_pRenderer->CreateSpriteObject(GetFile("menu_4.tif", DATA_TYPE_UI), 1, 96, 2, 2, 0);
	m_pBoxSpr			= g_pRenderer->CreateSpriteObject(GetFile("speaking_box.tif", DATA_TYPE_UI), 0);	
	m_pCureentSpr		= g_pRenderer->CreateSpriteObject(GetFile("menu_4.tif", DATA_TYPE_UI), 1, 1, 36, 68, 0);	
	m_bInit				= TRUE;
	m_bGuarInterface	= FALSE;

	return TRUE;
}

void CItemWnd::Remove()
{
	RemoveAllData();

	if(m_pLineSpr)
		m_pLineSpr->Release();
	if(m_pBoxSpr)
		m_pBoxSpr->Release();
	if(m_pCureentSpr)
		m_pCureentSpr->Release();
	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CItemWnd::RenderText()//: 050111 hwoarang
{	
	int nPos	= 0;
	int nSize	= 0;
	
	// 돈 //
	char szInfo[0xff] = {0,};
	wsprintf(szInfo, "%u", g_pMainPlayer->m_dwMoney);

	NUMBERFMT nFmt = { 0, 0, 3, ".", ",", 1 };

	::GetNumberFormat(NULL, NULL, szInfo, &nFmt, m_szInfo, 15);	
	nSize = lstrlen(m_szInfo);
	
	if(!IsEmptyString(m_szInfo))
		RenderFont(m_szInfo, m_fPosX+187-nSize*6, m_fPosX+256, m_fPosZ+495+nPos, m_fPosZ+507+nPos, GetStartOrder()+4);

	// 무게		
	DWORD	dwColor			= 0xffffffff;
//	WORD	wSumWeight		= g_pMainPlayer->GetSumWeight();
	WORD	wAllSumWeight	= g_pMainPlayer->GetAllSumWeight();
	WORD	wLargeSumWeight = g_pMainPlayer->GetLargeSumWeight();
	WORD	wSmallSumWeight = g_pMainPlayer->GetSmallItemSumWeight();
//	float	fPerWeight		= ((float)wSumWeight/(float)wAllSumWeight)*100;
	float	fPerWeight		= g_pMainPlayer->GetAverageWeight();

#if IS_TAIWAN_LOCALIZING()
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1000].szMessage, wSmallSumWeight + wLargeSumWeight, wAllSumWeight, (WORD)fPerWeight);//무게 : (S  + L) %u / %u (%u%%)
#else
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1000].szMessage, wSmallSumWeight, wLargeSumWeight, wAllSumWeight, (WORD)fPerWeight);//무게 : S %u + L %u / %u (%u%%)
#endif 

	if((WORD)fPerWeight >= WEIGTH_100PER_OVER)
		dwColor = 0xffff0000;	
	else if((WORD)fPerWeight >= WEIGTH_80PER_OVER )//색깔 80 초과부터로 : 050103 hwoarang
		dwColor = 0xffffff00;
			
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+260-nSize*7, m_fPosX+260, m_fPosZ+30, m_fPosZ+42, GetStartOrder()+4, dwColor);

	// 수량 //
	if(m_byItemChk == 1)
	{
		for(int i = 0;  i < MAX_INV_SMALL_POOL; i++)	
		{			
			if(g_pMainPlayer->m_pInv_Small[i].m_wItemID!=0)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_INV_SMALL &&
						i == g_ItemMoveManager.GetNativeSrcIndex())
					{
						continue;
					}
				}

				const int iQuantity =   g_pMainPlayer->m_pInv_Small[i].GetQuantity();
	 
				if(1 < iQuantity)
				{
					// 좌측 상단 좌표 //
					wsprintf(m_szInfo, "%d", iQuantity);

					int nSize	= lstrlen(m_szInfo);
					int nPosX	= (int)(m_fPosX+6)+(i%7)*34;
					int nPosY	= (int)(m_fPosZ+278)+(i/7)*34;

					if(!IsEmptyString(m_szInfo))
						RenderFont(m_szInfo, nPosX+30-(nSize+1)*6, nPosX+32, nPosY+20, nPosY+34, GetStartOrder()+4);
				}
			}
		}
	}	
	
	RenderItem();
	RenderItemInfo();	
	RenderLine();
	RenderCurrentHand();
}


void CItemWnd::RenderItem()
{
	RenderItemEquip();
	RenderItemGuardian();

	if(m_byItemChk==0)
		RenderItemLarge();
	else
		RenderItemSmall();
}


void CItemWnd::RenderItemBelt()
{	
	VECTOR2	vPos;						

	for(int i= 0; i < MAX_BELT_POOL; i++)
	{
		if(g_pMainPlayer->m_pBelt[i].m_wItemID!=0)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_BELT &&	i == g_ItemMoveManager.GetNativeSrcIndex())
				{
					continue;
				}
			}

			LP_ITEM_RESOURCE_EX lpItemResourceEx =
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pBelt[i].m_wItemID);

			if(lpItemResourceEx)
			{
				int nPosX = (int)727+(i*37);
				int nPosY = (int)733;

				vPos.x	= (float)nPosX;
				vPos.y	= (float)nPosY;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_USER_BLET__, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}			
		}
	}
}

void CItemWnd::RenderItemGuardian()
{
	int nOrder = __ORDER_INTERFACE_START__ + m_byOrder*10;
		
	VECTOR2	vPos;						
	
	for(int i= 0; i < MAX_INV_GUARDIAN_POOL; i++)
	{
		if(g_pMainPlayer->m_pInv_Guardian[i].m_wItemID!=0)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_INV_GUARDIAN &&
					i == g_ItemMoveManager.GetNativeSrcIndex())
				{
					continue;
				}
			}

			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pInv_Guardian[i].m_wItemID);

			if(lpItemResourceEx)
			{
				int nPosX = (int)m_fPosX+2+(55*i);
				int nPosY = (int)m_fPosZ+528;

				vPos.x	= (float)nPosX;
				vPos.y	= (float)nPosY;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}
		}
	}
}


void CItemWnd::RenderItemSmall()
{
	int nOrder = __ORDER_INTERFACE_START__ + m_byOrder*10;
		
	VECTOR2	vPos;					
				
	for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
	{
		if(g_pMainPlayer->m_pInv_Small[i].m_wItemID!=0)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_INV_SMALL &&
					i == g_ItemMoveManager.GetNativeSrcIndex())
				{
					continue;
				}
			}

			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pInv_Small[i].m_wItemID);

			if(lpItemResourceEx)
			{
				int nPosX = (int)(m_fPosX+6)+(i%7)*34;
				int nPosY = (int)(m_fPosZ+278)+(i/7)*34;

				vPos.x	= (float)nPosX;
				vPos.y	= (float)nPosY;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}			
		}
	}
}


void CItemWnd::RenderItemLarge()
{
	int nOrder = __ORDER_INTERFACE_START__ + m_byOrder*10;
		
	VECTOR2	vPos;
	
	for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
	{
		if(g_pMainPlayer->m_pInv_Large[i].m_wItemID!=0)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_INV_LARGE &&
					i == g_ItemMoveManager.GetNativeSrcIndex())
				{
					continue;
				}
			}
						
			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pInv_Large[i].m_wItemID);

			if(lpItemResourceEx)
			{
				int nPosX = (int)(m_fPosX+6)+(i%7)*34;
				int nPosY = (int)(m_fPosZ+279)+(i/7)*68;

				vPos.x	= (float)nPosX;
				vPos.y	= (float)nPosY;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}
		}		
	}
}

void CItemWnd::RenderItemEquip()
{
	int nOrder = __ORDER_INTERFACE_START__ + m_byOrder*10;
		
	VECTOR2	vPos;					

	for(int i= 0; i < MAX_EQUIP_POOL; i++)
	{
		if(g_pMainPlayer->m_pEquip[i].m_wItemID!=0)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_EQUIP &&
					i == g_ItemMoveManager.GetNativeSrcIndex())
				{
					continue;
				}
			}

			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pEquip[i].m_wItemID);

			if(lpItemResourceEx)
			{
				int nPosX = 6+(i%7)*34;
				int nPosY = 44+(i/7)*68;

				vPos.x	= m_fPosX+nPosX;
				vPos.y	= m_fPosZ+nPosY;

				DWORD alpha = 0xffffffff;
				BYTE bAlpha = 128;
				memcpy(((BYTE*)&alpha)+3, &bAlpha, 1);
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}						
		}
	}

	if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID != 0)		
	{
		int nVal = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID / ITEM_DISTRIBUTE;

		if((nVal>=2 && nVal<=5) || nVal==7)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND1].m_wItemID);

			if(lpItemResourceEx)
			{
				int nPosX = 6+(EQUIP_TYPE_LHAND1%7)*34;
				int nPosY = 44+(EQUIP_TYPE_LHAND1/7)*68;

				vPos.x	= m_fPosX+nPosX;
				vPos.y	= m_fPosZ+nPosY;
				
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}						
		}
	}

	if(g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID!=0)
	{
		int nVal = g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID/ITEM_DISTRIBUTE;

		if((nVal>=2 && nVal<=5) || nVal==7)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pEquip[EQUIP_TYPE_RHAND2].m_wItemID);

			if(lpItemResourceEx)
			{
				int nPosX = 6+(EQUIP_TYPE_LHAND2%7)*34;
				int nPosY = 44+(EQUIP_TYPE_LHAND2/7)*68;

				vPos.x	= m_fPosX+nPosX;
				vPos.y	= m_fPosZ+nPosY;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}
		}
	}
}

void CItemWnd::RenderItemMouse()
{
	VECTOR2	vPos;

	if( g_ItemMoveManager.GetNewItemMoveMode() )
	{
		WORD wMouseItemID = g_ItemMoveManager.GetMouseItemID();

		if(wMouseItemID)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = g_pItemResourceHash->GetData(wMouseItemID);

			if(lpItemResourceEx)
			{
				if(CQuantityWnd::GetInstance()->GetActive()==FALSE)
				{
					if(CQuantityWnd::GetInstance()->m_bReq==FALSE)
					{
						int nValue = wMouseItemID / ITEM_DISTRIBUTE;

						if(nValue<=20)
						{
							vPos.x	= (float)(g_Mouse.MousePos.x - 16);
							vPos.y	= (float)(g_Mouse.MousePos.y - 32);
						}
						else
						{
							vPos.x	= (float)(g_Mouse.MousePos.x - 16);
							vPos.y	= (float)(g_Mouse.MousePos.y - 16);
						}
						
						g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_MOUSE_POINTER__, RENDER_TYPE_DISABLE_TEX_FILTERING);
						CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEMCLICK__);
					}				
				}			
			}					
		}
	}
	else
	{
		CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();
				
		if(pStoreWnd->m_bSupplies==TRUE)
		{
			if(pStoreWnd->m_wSuppliesId!=0)
			{
				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
					g_pItemResourceHash->GetData(pStoreWnd->m_wSuppliesId);

				if(lpItemResourceEx)
				{
					int nValue = pStoreWnd->m_wSuppliesId/ITEM_DISTRIBUTE;

					if(nValue<=20)
					{
						vPos.x	= (float)(g_Mouse.MousePos.x - 16);
						vPos.y	= (float)(g_Mouse.MousePos.y - 32);
					}
					else
					{
						vPos.x	= (float)(g_Mouse.MousePos.x - 16);
						vPos.y	= (float)(g_Mouse.MousePos.y - 16);
					}
					
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_MOUSE_POINTER__, RENDER_TYPE_DISABLE_TEX_FILTERING);
					CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEMCLICK__);
				}			
			}
		}
		else if(g_pMainPlayer->m_MouseItem.m_wItemID!=0)
		{					
			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

			if(lpItemResourceEx)
			{
				if(CQuantityWnd::GetInstance()->GetActive()==FALSE)
				{
					if(CQuantityWnd::GetInstance()->m_bReq==FALSE)
					{
						int nValue = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;

						if(nValue<=20)
						{
							vPos.x	= (float)(g_Mouse.MousePos.x - 16);
							vPos.y	= (float)(g_Mouse.MousePos.y - 32);
						}
						else
						{
							vPos.x	= (float)(g_Mouse.MousePos.x - 16);
							vPos.y	= (float)(g_Mouse.MousePos.y - 16);
						}
						
						g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, __ORDER_MOUSE_POINTER__, RENDER_TYPE_DISABLE_TEX_FILTERING);
						CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEMCLICK__);
					}				
				}			
			}					
		}
	}
}

void CItemWnd::RenderBelt()// : 050110 hwoarang
{	
	for(int i = 0;  i < MAX_BELT_POOL; i++)	
	{
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_BELT &&
				i == g_ItemMoveManager.GetNativeSrcIndex())
			{
				continue;
			}
		}
		const int iQuantity = g_pMainPlayer->m_pBelt[i].GetQuantity();
		if(1 < iQuantity)
		{
			wsprintf(m_szInfo, "%d", iQuantity );

			int nSize = lstrlen(m_szInfo);
			RenderFont(m_szInfo, 755+i*37-(nSize*7), 755+i*37, 733+19, 745+19, __ORDER_USER_BLET__+1);
		}
	}
}

void CItemWnd::RenderItemInfo()
{
	CItem*	pItem;	
	int		nIndex		= 0;
	int		nPosX		= 0;
	int		nPosY		= 0;
	BOOL	isEquip		= FALSE;	
	
	if(g_Mouse.bLDown==TRUE)
		return;	
	
	if(	g_Mouse.MousePos.x>m_fPosX+6	&&
		g_Mouse.MousePos.x<m_fPosX+244	&&
		g_Mouse.MousePos.y>m_fPosZ+278	&&
		g_Mouse.MousePos.y<m_fPosZ+482)	
	{
		if(m_byItemChk==1)
		{
			nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
			nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-278)/34;			
			nIndex	= nPosY*7+nPosX;						
			
			if(nIndex>=MAX_INV_SMALL_POOL)
				return;
			else if(CUserInterface::GetInstance()->InterfaceCollision(ITEM_WND
				, (int)m_fPosX+nPosX*34+6
				, (int)m_fPosX+(nPosX+1)*34+6
				, (int)m_fPosZ+nPosY*34+278
				, (int)m_fPosZ+(nPosY+1)*34+278)==FALSE)
				return;
			else if(g_pMainPlayer->m_pInv_Small[nIndex].m_wItemID==0)
				return;
			
			pItem = &g_pMainPlayer->m_pInv_Small[nIndex];

			if(!pItem)
				return;

			CInterface::GetInstance()->ItemInfoRender(pItem, isEquip);
		}
		else if(m_byItemChk==0)
		{	
			nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
			nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-278)/68;
			nIndex	= nPosY*7+nPosX;			
					
			if(nIndex>=MAX_INV_LARGE_POOL)
				return;
			else if(CUserInterface::GetInstance()->InterfaceCollision(ITEM_WND
				, (int)m_fPosX+nPosX*34+6
				, (int)m_fPosX+(nPosX+1)*34+6
				, (int)m_fPosZ+nPosY*68+278
				, (int)m_fPosZ+(nPosY+1)*68+278)==FALSE)
				return;
			else if(g_pMainPlayer->m_pInv_Large[nIndex].m_wItemID==0)
				return;
			
			pItem = &g_pMainPlayer->m_pInv_Large[nIndex];

			if(!pItem)
				return;

			CInterface::GetInstance()->ItemInfoRender(pItem, isEquip);
		}		
	}	
	else if(g_Mouse.MousePos.x>=m_fPosX+6	&&
			g_Mouse.MousePos.x<=m_fPosX+244 &&
			g_Mouse.MousePos.y>=m_fPosZ+44	&&
			g_Mouse.MousePos.y<=m_fPosZ+248)
	{
		isEquip = TRUE;
		
		nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
		nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-44)/68;
		nIndex	= nPosY*7+nPosX;
						
		if(nIndex>=MAX_EQUIP_POOL)
			return;
		else if(CUserInterface::GetInstance()->InterfaceCollision(ITEM_WND
			, (int)m_fPosX+nPosX*34+6
			, (int)m_fPosX+(nPosX+1)*34+6
			, (int)m_fPosZ+nPosY*68+44
			, (int)m_fPosZ+(nPosY+1)*68+44)==FALSE)
			return;
		else if(g_pMainPlayer->m_pEquip[nIndex].m_wItemID==0)
			return;

		pItem = &g_pMainPlayer->m_pEquip[nIndex];						

		if(!pItem)
			return;
				
		if( nIndex==EQUIP_TYPE_RHAND1	|| nIndex==EQUIP_TYPE_RHAND2	||
			nIndex==EQUIP_TYPE_LHAND1	|| nIndex==EQUIP_TYPE_LHAND2	||
			nIndex==EQUIP_TYPE_HELMET	|| nIndex==EQUIP_TYPE_RGLOVE	||
			nIndex==EQUIP_TYPE_ARMOR	|| nIndex==EQUIP_TYPE_BELT		||
			nIndex==EQUIP_TYPE_LGLOVE	|| nIndex==EQUIP_TYPE_BOOTS)
		{
			CInterface::GetInstance()->ItemInfoRender(pItem, isEquip, 0, 0, nIndex, g_pMainPlayer->m_byUpgrade);
		}
		else
			CInterface::GetInstance()->ItemInfoRender(pItem, isEquip, 0, 0, nIndex, 0);
	}
	else if(g_Mouse.MousePos.x>m_fPosX+2	&&
			g_Mouse.MousePos.x<m_fPosX+33	&&
			g_Mouse.MousePos.y>m_fPosZ+528	&&
			g_Mouse.MousePos.y<m_fPosZ+591)	
	{				
		if(CUserInterface::GetInstance()->InterfaceCollision(ITEM_WND
			, (int)m_fPosX+2
			, (int)m_fPosX+33
			, (int)m_fPosZ+nPosY*528
			, (int)m_fPosZ+591)==FALSE)
			return;
		else if(g_pMainPlayer->m_pInv_Guardian[0].m_wItemID==0)		
			return;

		pItem = &g_pMainPlayer->m_pInv_Guardian[0];
		
		if(!pItem)
			return;

		CInterface::GetInstance()->ItemInfoRender(pItem, isEquip);
	}	
}

void CItemWnd::SetOrder()
{

}


void CItemWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;

	if(bActive==FALSE)
	{		
		ShowSpriteAll();

		m_bSoundChk		= FALSE;		
		m_bItemKeyChk	= FALSE;		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_ITEMWINDOW, TRUE);
		SetRender(SPR_OBJ_ITEMWINDOW2, TRUE);
		SetRender(SPR_OBJ_ITEM_CLOSE1, TRUE);										
		SetRender(SPR_OBJ_MONEY_BUTTON1, TRUE);

		if(m_byItemChk==0)		
			SetRender(SPR_OBJ_ITEM_CHK1, TRUE);
		else if(m_byItemChk==1)
			SetRender(SPR_OBJ_ITEM_CHK2, TRUE);
						
		// 장착 창에 모든것을 보여준다.			
		SetPosSommon();				
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

BOOL CItemWnd::GetMouseIcon()
{
	return m_bMouseIcon;
}

void CItemWnd::GetMouseIcon(BOOL bMouseIcon)
{
	m_bMouseIcon = bMouseIcon;
}

int CItemWnd::CheckInterface()
{
	m_sItemChkType.nItemIndex		= -1;
	m_sItemChkType.byItemZipCode	= 0;
	
	m_bMouseIcon = FALSE;

	int nRt = GetChk();
	
	if(m_bGuarInterface==TRUE)
	{
		if(nRt!=10)
		{		
			if(CUserInterface::GetInstance()->m_bGuardian==FALSE)
			{
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON1, FALSE);
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON2, FALSE);
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, TRUE);
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF2, FALSE);
			}
			else
			{
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, FALSE);
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF2, FALSE);
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON1, TRUE);
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON2, FALSE);
			}
			m_bGuarInterface = FALSE;
		}
	}

	if(m_bBtn && nRt!=7)
	{		
		SetRender(SPR_OBJ_MONEY_BUTTON1, TRUE);
		SetRender(SPR_OBJ_MONEY_BUTTON2, FALSE);
		SetRender(SPR_OBJ_MONEY_BUTTON3, FALSE);
		m_bBtn  = TRUE;
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
				SetActive(FALSE);				
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_ITEM_CLOSE1, FALSE);
				SetRender(SPR_OBJ_ITEM_CLOSE2, TRUE);
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
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_ITEM_CHK1, TRUE);
				SetRender(SPR_OBJ_ITEM_CHK2, FALSE);
				m_byItemChk	= 0;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_ITEM_CHK1, FALSE);
				SetRender(SPR_OBJ_ITEM_CHK2, TRUE);			
				m_byItemChk	= 1;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:	// InvLarge,Inv_Small
		{			
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{				
				CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();			

				if(!pQuantityWnd->GetActive())
				{
					CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

					if(!pItemTradeShopWnd->m_bOkChk)
					{
						CTDS_ITEM_PICKUP ItemPickup;

						CGuildExWnd*	pGuildExWnd		= CGuildExWnd::GetInstance();
						CGuildRankWnd*	pGuildRankWnd	= CGuildRankWnd::GetInstance();
						CGuildEditWnd*	pGuildEditWnd	= CGuildEditWnd::GetInstance();
						CStoreWnd*		pStoreWnd		= CStoreWnd::GetInstance();
					
						if(!pStoreWnd->m_bSupplies)
						{							
							if(	pGuildExWnd->GetActive()==FALSE &&
								pGuildRankWnd->GetActive()==FALSE &&
								pGuildEditWnd->GetActive()==FALSE)
							{
								if(g_pMainPlayer->m_MouseItem.m_wItemID==0)
								{
									// Large Item
									if(m_byItemChk==0)
									{			
										int nPosX = (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
										int nPosY = (g_Mouse.MousePos.y-(int)m_fPosZ-278)/68;
										int nItemIndex = nPosY*7+nPosX;
										
										if(nItemIndex<=20 && g_pMainPlayer->m_MouseItem.m_wItemID==0)
										{
											if(g_pMainPlayer->m_pInv_Large[nItemIndex].m_wItemID!=0)
											{			
												ItemPickup.bSectionNum	= 1;
												ItemPickup.i64ItemID	= 0;
												ItemPickup.bInv			= 7;
												ItemPickup.bZipCode		= nItemIndex;
												
												g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );

												if(g_ItemMoveManager.GetNewItemMoveMode())
												{
													WORD wID = g_pMainPlayer->m_pInv_Large[nItemIndex].m_wItemID;
													g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_INV_LARGE,nItemIndex,wID,0);
												}												
											}
											else
												byChk = __MOUSE_POINTER_DEFAULTCLICK__;
										}
									}
									// Small Item
									else if(m_byItemChk==1)
									{						
										if(CStoreWnd::GetInstance()->m_bSupplies==FALSE)
										{
											int nPosX = (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
											int nPosY = (g_Mouse.MousePos.y-(int)m_fPosZ-278)/34;
											int nItemIndex = nPosY*7+nPosX;

											if(g_pMainPlayer->m_pInv_Small[nItemIndex].m_wItemID!=0)
											{					
												ItemPickup.bSectionNum	= 1;
												ItemPickup.i64ItemID	= 0;
												ItemPickup.bInv			= 15;
												ItemPickup.bZipCode		= nItemIndex;

												g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );								

												if(g_ItemMoveManager.GetNewItemMoveMode())
												{
													WORD wID = g_pMainPlayer->m_pInv_Small[nItemIndex].m_wItemID;
													g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_INV_SMALL,nItemIndex,wID,0);
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
						}
					}
				}				
			}	
			else if(eMouseCheck==MOUSE_OVER)
			{						
				if(m_byItemChk==0)
				{
					int nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
					int nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-278)/68;
					int nIndex	= nPosY*7+nPosX;
									
					if(g_pMainPlayer->m_pInv_Large[nIndex].m_wItemID!=0)
					{					
						m_sItemChkType.nItemIndex		= __ITEM_LARGE__;				
						m_sItemChkType.byItemZipCode	= nIndex;
						byChk = __MOUSE_POINTER_ITEM__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else if(m_byItemChk==1)
				{
					int nPosX = (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
					int nPosY = (g_Mouse.MousePos.y-(int)m_fPosZ-278)/34;
					int nIndex = nPosY*7+nPosX;

					if(g_pMainPlayer->m_pInv_Small[nIndex].m_wItemID!=0)
					{
						m_sItemChkType.nItemIndex		= __ITEM_SMALL__;
						m_sItemChkType.byItemZipCode	= nIndex;
						byChk = __MOUSE_POINTER_ITEM__;
					}
					else
						byChk = __MOUSE_POINTER_DEFAULT__;						
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}				
		break;
	case 7:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
				CGuildExWnd*	pGuildExWnd		= CGuildExWnd::GetInstance();			
				CGuildRankWnd*	pGuildRankWnd	= CGuildRankWnd::GetInstance();
				CGuildEditWnd*	pGuildEditWnd	= CGuildEditWnd::GetInstance();

				if(	pGuildExWnd->GetActive()==FALSE &&
					pGuildRankWnd->GetActive()==FALSE &&
					pGuildEditWnd->GetActive()==FALSE)
				{					
					CChatWnd* pChatWnd = CChatWnd::GetInstance();

					if(g_pGVDungeon->bChatMode)
					{						
						g_pInputManager->SetFocusInput(INPUT_BUFFER_19);
					
						if(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19))
						{
							if(!IsEmptyString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)))
							{						
								memset(pChatWnd->m_szClip, 0, sizeof(pChatWnd->m_szClip));
								__lstrcpyn(pChatWnd->m_szClip
									, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)
									, lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)));
								pChatWnd->m_byClipType = 1;
							}
						}	
						
						ChatModeOff(INPUT_BUFFER_19);						
					}
					
					if(pChatWnd->GetActive()==TRUE)
					{
						pChatWnd->SetActive(FALSE);
					}
															
					CBankWnd* pBankWnd = CBankWnd::GetInstance();

					if(pBankWnd->GetActive()==FALSE)
					{
						CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

						if(pItemTradeShopWnd->GetActive()==TRUE)
						{
							// "교환하실 경우에는 Karz를 바닥에 버릴수 없습니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE591].szMessage, 0xFFFFC309); 
							
							SetRender(SPR_OBJ_MONEY_BUTTON1, FALSE);
							SetRender(SPR_OBJ_MONEY_BUTTON2, TRUE);
							SetRender(SPR_OBJ_MONEY_BUTTON3, FALSE);																
							CUserInterface::GetInstance()->SetPointer(__MOUSE_POINTER_BUTTON__);							

							m_bClick	= TRUE;
							m_bDown		= TRUE;
							m_bUpChk	= TRUE;

							return 1;
						}
						
						CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

						if(pQuantityWnd->GetActive()==FALSE)
						{
							pQuantityWnd->OpenWnd(__QUANTITY_MONEY__);

							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_INV_KARZ,0,0,0,0);	
							}
						}
						else
						{
							pQuantityWnd->SetActive(FALSE);						
						}
					}
					else					
					{
						CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

						if(pQuantityWnd->GetActive()==FALSE)
						{
							pQuantityWnd->OpenWnd(__QUANTITY_BNAKMONEYINSERT__);

							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_INV_KARZ,0,0,0,0);	
							}
						}
						else
						{
							pQuantityWnd->SetActive(FALSE);						
						}
					}				
					
					if(pGuildExWnd->GetActive()==TRUE)
					{
						pGuildExWnd->SetActive(FALSE);
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);						
					}

					if(pQuantityWnd->GetActive()==TRUE)
					{
						// 채팅모드이면 초기화.							
						RECT rt;
						rt.left		= (int)pQuantityWnd->m_fPosX+124-7;
						rt.right	= (int)pQuantityWnd->m_fPosX+124;
						rt.top		= (int)pQuantityWnd->m_fPosZ+83;
						rt.bottom	= (int)pQuantityWnd->m_fPosZ+95;					
						g_pInputManager->InitializeInput(INPUT_BUFFER_5
							, FONT_SS3D_GAME, rt, 0xffffffff, pQuantityWnd->GetStartOrder()+2);
						g_pInputManager->SetFocusInput(INPUT_BUFFER_5);						
					}
					else
					{
						memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
						pQuantityWnd->m_byDrawIndex =	0;					
						::KillTimer(g_hMainWnd, 1);

						g_pInputManager->GetInputBuffer(INPUT_BUFFER_5);
					}
					
					SetRender(SPR_OBJ_MONEY_BUTTON1, FALSE);
					SetRender(SPR_OBJ_MONEY_BUTTON2, TRUE);
					SetRender(SPR_OBJ_MONEY_BUTTON3, FALSE);						
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_MONEY_BUTTON1, FALSE);
				SetRender(SPR_OBJ_MONEY_BUTTON2, FALSE);
				SetRender(SPR_OBJ_MONEY_BUTTON3, TRUE);
				m_bBtn = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_MONEY_BUTTON1, FALSE);
				SetRender(SPR_OBJ_MONEY_BUTTON2, TRUE);
				SetRender(SPR_OBJ_MONEY_BUTTON3, FALSE);								
				m_bBtn = TRUE;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 8: // Equip
		{			
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{
				CQuantityWnd*		pQuantityWnd		= CQuantityWnd::GetInstance();
				CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();						

				if(!pQuantityWnd->GetActive() && !pItemTradeShopWnd->GetActive())
				{
					CGuildExWnd*	pGuildExWnd		= CGuildExWnd::GetInstance();
					CGuildRankWnd*	pGuildRankWnd	= CGuildRankWnd::GetInstance();
					CGuildEditWnd*	pGuildEditWnd	= CGuildEditWnd::GetInstance();
					CStoreWnd*		pStoreWnd		= CStoreWnd::GetInstance();
					
					if(!pStoreWnd->m_bSupplies)
					{
						if(!pGuildExWnd->GetActive() && !pGuildRankWnd->GetActive() && !pGuildEditWnd->GetActive())
						{
							CTDS_ITEM_PICKUP ItemPickup;
							
							int nPosX = (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
							int nPosY = (g_Mouse.MousePos.y-(int)m_fPosZ-44)/68;
							int nItemIndex = nPosY*7+nPosX;
								
							if(g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID!=0)
							{				
								if(g_pMainPlayer->m_MouseItem.GetID()==0)
								{
									ItemPickup.bSectionNum	= 1;
									ItemPickup.i64ItemID	= 0;
									ItemPickup.bInv			= 11;
									ItemPickup.bEquipCode	= nItemIndex;								 
									g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );

									if(g_ItemMoveManager.GetNewItemMoveMode())
									{
										WORD wID = g_pMainPlayer->m_pEquip[nItemIndex].m_wItemID;
										g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_EQUIP,nItemIndex,wID,0);
									}
								}						
							}
							else
							{
								BYTE bType = 0;

								if(nItemIndex==EQUIP_TYPE_LHAND1)
									bType = EQUIP_TYPE_RHAND1;
								else if(nItemIndex==EQUIP_TYPE_LHAND2)
									bType = EQUIP_TYPE_RHAND2;

								if(nItemIndex==EQUIP_TYPE_LHAND1 || nItemIndex==EQUIP_TYPE_LHAND2)
								{
									if(g_pMainPlayer->m_pEquip[bType].m_wItemID!=0)
									{
										int nVal = g_pMainPlayer->m_pEquip[bType].m_wItemID/ITEM_DISTRIBUTE;

										// 투핸드 
										if((nVal>=2 && nVal<=5) || nVal==7)						
										{
											ItemPickup.bSectionNum	= 1;
											ItemPickup.i64ItemID	= 0;
											ItemPickup.bInv			= 11;
											ItemPickup.bEquipCode	= bType;
											g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );

											if(g_ItemMoveManager.GetNewItemMoveMode())
											{
												WORD wID = g_pMainPlayer->m_pEquip[bType].m_wItemID;
												g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_EQUIP,bType,wID,0);
											}
										}							
									}
									else
									{
										byChk = __MOUSE_POINTER_DEFAULT__;																
									}
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
				else
				{
					byChk = __MOUSE_POINTER_DEFAULT__;
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				int nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-6)/34;
				int nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-44)/68;
				int nIndex	= nPosY*7+nPosX;
				
				if(g_pMainPlayer->m_pEquip[nIndex].m_wItemID!=0)
				{				
					m_sItemChkType.nItemIndex		= __ITEM_EQUIP__;
					m_sItemChkType.byItemZipCode	= nIndex;					
				}
				else
					byChk = __MOUSE_POINTER_DEFAULT__;
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}				
		break;	
	case 9:	// Guardian	
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();
					
					if(!pStoreWnd->m_bSupplies)
					{
						if(CUserInterface::GetInstance()->m_bGuardian==FALSE)
						{
							WORD wID = g_pMainPlayer->m_pInv_Guardian[0].m_wItemID;
							g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_INV_GUARDIAN,0,wID,0);
						}
						else
						{
							// MSG_ID : 315 ; 가디언 아이템을 이동할 수가 없습니다.
							DisplayMessageAdd(g_Message[ETC_MESSAGE315].szMessage, 0xFFFFC309);
						}
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
					}
				}
				else
				{
					CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();
					
					if(!pStoreWnd->m_bSupplies)
					{					
						if(	g_pMainPlayer->m_pInv_Guardian[0].m_wItemID!=0 &&
							g_pMainPlayer->m_MouseItem.GetID()==0)
						{			
							if(CUserInterface::GetInstance()->m_bGuardian==FALSE)
							{
								CTDS_ITEM_PICKUP ItemPickup;
								ItemPickup.bSectionNum	= 1;
								ItemPickup.i64ItemID	= 0;
								ItemPickup.bInv			= 39;
								ItemPickup.bZipCode		= 0;
								g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );							
							}
							else
							{
								// MSG_ID : 315 ; 가디언 아이템을 이동할 수가 없습니다.
								DisplayMessageAdd(g_Message[ETC_MESSAGE315].szMessage, 0xFFFFC309);		
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
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(g_pMainPlayer->m_pInv_Guardian[0].m_wItemID!=0)
				{
					byChk = __MOUSE_POINTER_ITEM__;
				}
				else
				{
					byChk = __MOUSE_POINTER_DEFAULT__;
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}				
		break;
	case 10:	// Call Guardian
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{							
				CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();
					
				if(!pStoreWnd->m_bSupplies)
				{					
					if(g_pMainPlayer->m_pInv_Guardian[0].m_wItemID!=0)
					{					
						if(g_pMainPlayer->GetStatus()!=UNIT_STATUS_DEAD)
						{
//							LP_ITEM_TABLE pItemInfo = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Guardian[0].GetID());
							CBaseItem* pItemInfo = g_pItemTableHash->GetData(g_pMainPlayer->m_pInv_Guardian[0].GetID());

							if (GUARDIAN_ITEM_TYPE_ADULT == pItemInfo->BaseItem_Guardian.bType)
							{
								if(CUserInterface::GetInstance()->m_bGuardian==FALSE)
								{				
									// ON //
									CTDS_DUNGEON_CALLGARDIAN CallGuardinan;
									CallGuardinan.bZipCode = 0;
									g_pNet->SendMsg( (char*)&CallGuardinan, CallGuardinan.GetPacketSize()
										, SERVER_INDEX_ZONE );
								}
								else
								{
									// OFF //
									CTDS_GUARDIAN_COMMAND Command;
									Command.bZipCode = 0;
									Command.bCommand = 1;
									g_pNet->SendMsg( (char*)&Command, Command.GetPacketSize(), SERVER_INDEX_ZONE );				
								}
							}
							else if (GUARDIAN_ITEM_TYPE_EGG == pItemInfo->BaseItem_Guardian.bType)
							{
								CTDS_DUNGEON_CALLGARDIAN CallGuardinan;
								CallGuardinan.bZipCode = 0;
								g_pNet->SendMsg( (char*)&CallGuardinan, CallGuardinan.GetPacketSize(), SERVER_INDEX_ZONE );
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
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_bGuarInterface==FALSE)
				{			
					if(CUserInterface::GetInstance()->m_bGuardian==FALSE)
					{
						SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON1, FALSE);
						SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON2, FALSE);
						SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, FALSE);
						SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF2, TRUE);
					}
					else
					{
						SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, FALSE);
						SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF2, FALSE);
						SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON1, FALSE);
						SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON2, TRUE);
					}
					m_bGuarInterface = TRUE;
				}

				if(g_pMainPlayer->m_pInv_Guardian[0].m_wItemID==0)
				{
					byChk = __MOUSE_POINTER_DEFAULT__;
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}		
		break;	
	}	
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_ITEM_CLOSE1, TRUE);
		SetRender(SPR_OBJ_ITEM_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))		
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

void CItemWnd::SetPosSommon()
{
	for(int i = 0; i < 1; i++)	
	{
		if(g_pMainPlayer->m_pInv_Guardian[i].m_wItemID==0)
			SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, TRUE);
		else
		{	
			if(CUserInterface::GetInstance()->m_bGuardian==TRUE)
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_ON1, TRUE);
			else
				SetRender(SPR_OBJ_ITEMWND_GUARDIAN_OFF1, TRUE);
		}			
	}	
}


void CItemWnd::SetLine(BOOL bActive, BYTE bLineType)
{
	if(bLineType==0)
	{
		if(bActive==TRUE)
		{
			SetRender(SPR_OBJ_ITEM_CHK1, TRUE);
			SetRender(SPR_OBJ_ITEM_CHK2, FALSE);
		}
		else
		{
			SetRender(SPR_OBJ_ITEM_CHK1, FALSE);
			SetRender(SPR_OBJ_ITEM_CHK2, FALSE);			
		}
	}
	else
	{
		if(bActive==TRUE)
		{	
			SetRender(SPR_OBJ_ITEM_CHK1, FALSE);
			SetRender(SPR_OBJ_ITEM_CHK2, TRUE);			
		}
		else
		{
			SetRender(SPR_OBJ_ITEM_CHK1, FALSE);
			SetRender(SPR_OBJ_ITEM_CHK2, FALSE);			
		}
	}
}


void CItemWnd::RenderLine()
{
	VECTOR2	vPos;
		
	if(m_byItemChk==0)
	{		
		for(int i = 0; i < MAX_INV_LARGE_POOL; i++)
		{
			vPos.x		= m_fPosX+5+(i%7)*34;
			vPos.y		= m_fPosZ+276+(i/7)*68;
			g_pRenderer->RenderSprite(CBankWnd::GetInstance()->m_pSprLarge, NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+1, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
	else
	{
		for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
		{
			vPos.x		= m_fPosX+5+(i%7)*34;
			vPos.y		= m_fPosZ+276+(i/7)*34;
			g_pRenderer->RenderSprite(CBankWnd::GetInstance()->m_pSprSmall, NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+1, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}									
	}	
}

void CItemWnd::RenderCurrentHand()
{
	VECTOR2 vScaling;
	VECTOR2	vPos;

	vScaling.x	= 1.0f;
	vScaling.y	= 1.0f;

	vPos.x		= m_fPosX+4+(g_pMainPlayer->m_bCurrnetHand-1)*34;
	vPos.y		= m_fPosZ+42;
	g_pRenderer->RenderSprite(m_pCureentSpr, &vScaling, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+2, RENDER_TYPE_DISABLE_TEX_FILTERING);

	vPos.x		= m_fPosX+174+(g_pMainPlayer->m_bCurrnetHand-1)*34;
	vPos.y		= m_fPosZ+42;
	g_pRenderer->RenderSprite(m_pCureentSpr, &vScaling, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
}
//======================================================//
// End.													//
//======================================================//