//======================================================//
// Code by Jang.							2003.03.10	//
//======================================================//
#include "StoreWnd.h"
#include "GameControl.h"
#include "Define.h"
#include "DefResource.h"
#include "HashTable.h"
#include "UserInterface.h"
#include "Interface.h"
#include "ItemWnd.h"
#include "message.h"
#include "QuantityWnd.h"
#include "SkillWnd.h"
#include "BankWnd.h"
#include "InitGame.h"
#include "DungeonProcess.h"
#include "CodeFun.h"

CStoreWnd* CStoreWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CStoreWnd::CStoreWnd()
{ 	
	m_byRenderIndex		= 255;		
	m_wSuppliesCount	= 0;
	m_wWeaponCount		= 0;
	m_wArmorCount		= 0;	
	m_wSuppliesId		= 0;	
	m_byChk				= 1;
	m_dwMapId			= 0;	
	m_dwMonsterIndex	= 0;
	m_bSupplies			= FALSE;	

	for(int i = 0; i < 2; i++)
		m_bBtnChk[i] = FALSE;	
	for(i = 0; i < 200; i++)
		m_wItemId[i] = 0;	
}

CStoreWnd::~CStoreWnd()
{

}


//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CStoreWnd::Init()
{
	int nIndex = 0;
	
	CInterface* pInterface = CInterface::GetInstance();
	
	m_byServer = pInterface->m_byServer;
	
	if(pInterface->m_byServer==__SERVER_TYPE_VILL__)
	{
	/*
		InsertData(SPR_OBJ_STORE_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
		InsertData(SPR_OBJ_STORE_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 21.0f, 0);
		InsertData(SPR_OBJ_STORE_WND3, SPR_INTERFACE_DF_WND3, 0, 368, 1.0f, 1.0f, 0);
		InsertData(SPR_OBJ_STORE_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
		InsertData(SPR_OBJ_STORE_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);														
		InsertData(SPR_OBJ_STOREWND_PAGE, SPR_INTERFACE_PAGEBOX, 96, 357, 1.0f, 1.0f, 1);		
		InsertData(SPR_OBJ_STOREWND_PREV1, SPR_INTERFACE_PAGEPRE1, 78, 359, 1.0f, 1.0f, 1);
		InsertData(SPR_OBJ_STOREWND_PREV2, SPR_INTERFACE_PAGEPRE2, 78, 359, 1.0f, 1.0f, 1);
		InsertData(SPR_OBJ_STOREWND_PREV3, SPR_INTERFACE_PAGEPRE3, 78, 359, 1.0f, 1.0f, 1);
		InsertData(SPR_OBJ_STOREWND_NEXT1, SPR_INTERFACE_PAGENEXT1, 162, 359, 1.0f, 1.0f, 1);
		InsertData(SPR_OBJ_STOREWND_NEXT2, SPR_INTERFACE_PAGENEXT2, 162, 359, 1.0f, 1.0f, 1);
		InsertData(SPR_OBJ_STOREWND_NEXT3, SPR_INTERFACE_PAGENEXT3, 162, 359, 1.0f, 1.0f, 1);			
		
		InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
		InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
		InsertCheckInterface(78, 359, 94, 375, 4, CHECK_BTN);
		InsertCheckInterface(162, 359, 178, 375, 5, CHECK_BTN);
		InsertCheckInterface(7, 37, 251, 316, 6, CHECK_ITEM);
	*/
		ListNode<ITEM_STORE>*	lpItemStoreNode;
		LP_ITEM_STORE			lpItemStore;
		LP_ITEM_RESOURCE_EX		pItemResource;
			
		lpItemStoreNode = g_pItemStoreHash->GetHead();

		while(lpItemStoreNode)
		{	
			lpItemStore	= lpItemStoreNode->pData;						

			if(lpItemStore)
			{
				if(lpItemStore->wMapId == 0 || lpItemStore->wMapId == m_dwMapId)
				{
					pItemResource = g_pItemResourceHash->GetData(lpItemStore->wItemId);

					if(pItemResource)
					{						
						m_wItemId[nIndex] = lpItemStore->wItemId;					

						if(lpItemStore->byType==STORE_TYPE_SUSPZO)
						{
							m_wSuppliesCount++;
						}
						else
						{
							if(lpItemStore->byType==STORE_TYPE_WEAPON)
							{
								m_wWeaponCount++;
							}
							else if(lpItemStore->byType==STORE_TYPE_ARMOR)
							{
								m_wArmorCount++;							
							}
						}
						
						nIndex++;
					}
				}
			}

			lpItemStoreNode = lpItemStoreNode->pNext;
		}		
	}
	
	m_bInit	= TRUE;

	return TRUE;
}


void CStoreWnd::Remove()
{	
	if(m_byServer==__SERVER_TYPE_VILL__)	
		RemoveAllData();

	for(int i = 0; i < 200; i++)
		m_wItemId[i] = 0;

	m_wSuppliesCount	= 0;
	m_wWeaponCount		= 0;
	m_wArmorCount		= 0;
	m_bActive			= FALSE;
	m_bInit				= FALSE;
}

void CStoreWnd::SetOrder()
{
	
}

void CStoreWnd::RenderText()
{					
	if(	g_Mouse.MousePos.x>=m_fPosX+7 &&
		g_Mouse.MousePos.x<m_fPosX+251 &&
		g_Mouse.MousePos.y>=m_fPosZ+37 &&
		g_Mouse.MousePos.y<m_fPosZ+316)
	{
		int nPosX		= (int)(g_Mouse.MousePos.x-(m_fPosX+7));
		int nPosY		= (int)(g_Mouse.MousePos.y-(m_fPosZ+37));
		int nIndex		= 0;
		int nCoPosX		= (int)m_fPosX+7;
		int nCoPosX2	= (int)m_fPosX+7;
		int nCoPosY		= (int)m_fPosZ+37;
		int nCoPosY2	= (int)m_fPosZ+37;

		if((nPosX/34)>=7)
		{			
			m_bRender = FALSE;
			m_byRenderIndex	= 255;
		}
		else
		{
			if(m_byStoreType==STORE_TYPE_SUSPZO)
			{					
				nIndex	= (nPosX/34)+((nPosY/34)*7);

				nCoPosX2 += 34;
				nCoPosY2 += 34;

				if(nIndex!=0)
				{
					nCoPosX		= (int)m_fPosX+7+(nIndex%7)*34;
					nCoPosX2	= (int)m_fPosX+7+(nIndex%7+1)*34;
					nCoPosY		= (int)m_fPosZ+37+(nIndex/7)*34;
					nCoPosY2	= (int)m_fPosZ+37+(nIndex/7+1)*34;
				}		
			}
			else
			{
				nIndex	= (nPosX/34)+((nPosY/68)*7);

				nCoPosX2 += 34;
				nCoPosY2 += 68;

				if(nIndex!=0)
				{
					nCoPosX		= (int)m_fPosX+7+(nIndex%7)*34;
					nCoPosX2	= (int)m_fPosX+7+(nIndex%7+1)*34;
					nCoPosY		= (int)m_fPosZ+37+(nIndex/7)*68;
					nCoPosY2	= (int)m_fPosZ+37+(nIndex/7+1)*68;
				}		
			}

			if(nIndex<m_byPageItemCount)
			{			
				if(CUserInterface::GetInstance()->InterfaceCollision(STORE_WND, nCoPosX, nCoPosX2, nCoPosY, nCoPosY2)==TRUE)
				{					
					m_bRender = TRUE;
					m_byRenderIndex	= (BYTE)nIndex;							
				}
				else
				{					
					m_bRender = FALSE;
					m_byRenderIndex	= 255;
				}
			}
			else
			{				
				m_bRender		= FALSE;
				m_byRenderIndex	= 255;
			}
		}
	}

	if(m_byServer==__SERVER_TYPE_VILL__)
	{				
		RenderFont(g_Message[ETC_MESSAGE383].szMessage, m_fPosX+25, m_fPosX+200, m_fPosZ+6, m_fPosZ+18, GetStartOrder()+1);
		
		// MSG_ID : 384 ; Page %d/%d
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE384].szMessage, m_byCurPage+1, m_byMaxPage);	
		RenderFont(m_szInfo, m_fPosX+102, m_fPosX+160, m_fPosZ+362, m_fPosZ+374, GetStartOrder()+2);
	
		if(m_bRender)
		{
			if(!g_Mouse.bLDown)
			{
				int		nMaxLen	= 0;
				BYTE	byIndex = 0;

				// 이름 //
				if(!IsEmptyString(m_sStoreInfo[m_byRenderIndex].szItemName))
				{			
					int nLen = lstrlen(m_sStoreInfo[m_byRenderIndex].szItemName);

					DWORD dwColor = 0xffff0000;

					CBaseItem* lpItemTable = g_pItemTableHash->GetData(m_sStoreInfo[m_byRenderIndex].wItemId);

					if(lpItemTable)
					{
						if(lpItemTable->GetPrice() <= g_pMainPlayer->m_dwMoney)
							dwColor = m_sStoreInfo[m_byRenderIndex].dwNameColor;
					}

					nMaxLen = nLen;

					RenderFont(m_sStoreInfo[m_byRenderIndex].szItemName, g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7, g_Mouse.MousePos.y - 7 + byIndex*18, g_Mouse.MousePos.y + 7 + byIndex*18, __ORDER_ITEM_DESC__+1, dwColor);					
					byIndex++;
				}

				// 옵션 //
				for(int i = 0; i < m_sStoreInfo[m_byRenderIndex].byOptionCnt; i++)
				{
					int nLen = lstrlen(m_sStoreInfo[m_byRenderIndex].szOption[i]);

					if(nMaxLen<nLen)
						nMaxLen = nLen;

					RenderFont(m_sStoreInfo[m_byRenderIndex].szOption[i], g_Mouse.MousePos.x, g_Mouse.MousePos.x+nLen*7, g_Mouse.MousePos.y - 7 + byIndex*18, g_Mouse.MousePos.y + 7 + byIndex*18, __ORDER_ITEM_DESC__+1, m_sStoreInfo[m_byRenderIndex].dwColor[i]);
					byIndex++;
				}

				// 가격 //
				if(!IsEmptyString(m_sStoreInfo[m_byRenderIndex].szPriceName))
				{
					int nLen = lstrlen(m_sStoreInfo[m_byRenderIndex].szPriceName);

					if(nMaxLen<nLen)
						nMaxLen = nLen;
					
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(m_sStoreInfo[m_byRenderIndex].wItemId);

					DWORD dwColor = 0xffff0000;

					if(lpItemTable)
					{
						if(lpItemTable->GetPrice()<=g_pMainPlayer->m_dwMoney)
							dwColor = 0xffffffff;
					}

					RenderFont(	m_sStoreInfo[m_byRenderIndex].szPriceName,g_Mouse.MousePos.x,g_Mouse.MousePos.x+nLen*7,g_Mouse.MousePos.y - 7 + byIndex*18,g_Mouse.MousePos.y + 7 + byIndex*18,__ORDER_ITEM_DESC__+1,dwColor);					
				}

				CInterface::GetInstance()->RenderInfoBox((float)(g_Mouse.MousePos.x-5), (float)(g_Mouse.MousePos.y-17), (float)(nMaxLen*7+10), (float)(byIndex*18+27), __ORDER_ITEM_DESC__);			
			}
		}		
	}

	RenderItem();
	RenderLine();
}

void CStoreWnd::RenderLine()
{	
	VECTOR2	vPos;				

	if(m_byStoreType==STORE_TYPE_SUSPZO)
	{
		for(int i = 0; i < 63; i++)
		{
			vPos.x	= m_fPosX+5+(i%7)*34;
			vPos.y	= m_fPosZ+34+(i/7)*34;
			g_pRenderer->RenderSprite(CBankWnd::GetInstance()->m_pSprSmall, NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+1, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}

	}
	else
	{
		for(int i = 0; i < 28; i++)
		{
			vPos.x	= m_fPosX+5+(i%7)*34;
			vPos.y	= m_fPosZ+34+(i/7)*68;
			g_pRenderer->RenderSprite(CBankWnd::GetInstance()->m_pSprLarge, NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+1, RENDER_TYPE_DISABLE_TEX_FILTERING);
		}
	}
}	

void CStoreWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(m_byServer==__SERVER_TYPE_VILL__)
	{
		if(bActive==FALSE)
		{
			ShowSpriteAll();
			
			m_bRender = FALSE;
			m_byRenderIndex = 255;
			
			CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

			if(pQuantityWnd->GetActive())
				pQuantityWnd->SetActive(FALSE);

			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
		}
		else
		{
			CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

			SetRender(SPR_OBJ_STORE_WND1, TRUE);
			SetRender(SPR_OBJ_STORE_WND2, TRUE);
			SetRender(SPR_OBJ_STORE_WND3, TRUE);			
			SetRender(SPR_OBJ_STORE_CLOSE1, TRUE);
			SetRender(SPR_OBJ_STOREWND_PAGE, TRUE);					
			SetRender(SPR_OBJ_STOREWND_PREV1, TRUE);
			SetRender(SPR_OBJ_STOREWND_NEXT1, TRUE);						

			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
		}
	}
}

int CStoreWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();
			
	if(nRt!=4)
	{
		if(m_bBtnChk[0]==TRUE)
		{
			m_bBtnChk[0] = FALSE;
			SetRender(SPR_OBJ_STOREWND_PREV1, TRUE);
			SetRender(SPR_OBJ_STOREWND_PREV2, FALSE);
			SetRender(SPR_OBJ_STOREWND_PREV3, FALSE);
		}		
	}	
	
	if(nRt!=5)
	{
		if(m_bBtnChk[1]==TRUE)
		{
			m_bBtnChk[1] = FALSE;
			SetRender(SPR_OBJ_STOREWND_NEXT1, TRUE);
			SetRender(SPR_OBJ_STOREWND_NEXT2, FALSE);
			SetRender(SPR_OBJ_STOREWND_NEXT3, FALSE);
		}
	}

	if(nRt!=6)
	{
		if(m_bRender)
		{
			m_bRender		= FALSE;
			m_byRenderIndex	= 255;
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
				SetActive(FALSE);	
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_STORE_CLOSE1, FALSE);
				SetRender(SPR_OBJ_STORE_CLOSE2, TRUE);
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
			if(eMouseCheck==MOUSE_UP)
			{
				SetPrevNext(__STORE_PAGE_PREV__);										
				SetRender(SPR_OBJ_STOREWND_PREV1, FALSE);
				SetRender(SPR_OBJ_STOREWND_PREV2, TRUE);
				SetRender(SPR_OBJ_STOREWND_PREV3, FALSE);
				m_bBtnChk[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				m_bBtnChk[0]	= TRUE;
				SetRender(SPR_OBJ_STOREWND_PREV1, FALSE);
				SetRender(SPR_OBJ_STOREWND_PREV2, FALSE);
				SetRender(SPR_OBJ_STOREWND_PREV3, TRUE);
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_STOREWND_PREV1, FALSE);
				SetRender(SPR_OBJ_STOREWND_PREV2, TRUE);
				SetRender(SPR_OBJ_STOREWND_PREV3, FALSE);
				m_bBtnChk[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetPrevNext(__STORE_PAGE_NEXT__);						
				SetRender(SPR_OBJ_STOREWND_NEXT1, FALSE);
				SetRender(SPR_OBJ_STOREWND_NEXT2, TRUE);
				SetRender(SPR_OBJ_STOREWND_NEXT3, FALSE);
				m_bBtnChk[1]	= TRUE;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{							
				SetRender(SPR_OBJ_STOREWND_NEXT1, FALSE);
				SetRender(SPR_OBJ_STOREWND_NEXT2, FALSE);
				SetRender(SPR_OBJ_STOREWND_NEXT3, TRUE);
				m_bBtnChk[1]	= TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_STOREWND_NEXT1, FALSE);
				SetRender(SPR_OBJ_STOREWND_NEXT2, TRUE);
				SetRender(SPR_OBJ_STOREWND_NEXT3, FALSE);
				m_bBtnChk[1] = TRUE;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:	
		{					
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byStoreType==STORE_TYPE_SUSPZO)
				{
					// Small Item
					CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

					if(!pQuantityWnd->GetActive())
					{
						int nPosX	= (int)(g_Mouse.MousePos.x-(m_fPosX+7));
						int nPosY	= (int)(g_Mouse.MousePos.y-(m_fPosZ+37));

						if((nPosX/34)<7)
						{
							int nIndex	= (nPosX/34)+((nPosY/34)*7);

							if(nIndex<m_byPageItemCount)
							{
								if(m_bSupplies==FALSE)
								{
									if(g_ItemMoveManager.GetNewItemMoveMode())
									{
										if(g_ItemMoveManager.GetNativeSrc() == 0xff)
										{										
											CBaseItem* pItemTable = g_pItemTableHash->GetData(m_sStoreInfo[nIndex].wItemId);
											
											if(pItemTable)
											{
												nIndex = (m_byCurPage*MAX_ONEPAGE_SMALL) + nIndex;
												g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_TRADERMONSTER_INV_SMALL
													,(BYTE)nIndex
													,m_sStoreInfo[nIndex].wItemId
													,0
													,m_dwMonsterIndex);
												
												m_wSuppliesId	= m_sStoreInfo[nIndex].wItemId;
												m_bSupplies		= TRUE;
												g_pMainPlayer->m_nItemSelect = 1;
											}
										}
									}
									else
									{									
										if(g_pMainPlayer->m_MouseItem.m_wItemID==0)
										{
											CBaseItem* pItemTable = g_pItemTableHash->GetData(m_sStoreInfo[nIndex].wItemId);							

											if(pItemTable)
											{
												m_wSuppliesId	= m_sStoreInfo[nIndex].wItemId;
												m_bSupplies		= TRUE;
												g_pMainPlayer->m_nItemSelect = 1;
											}								
										}
									}
								}
							}
							
							m_bRender		= FALSE;
							m_byRenderIndex	= m_byPageItemCount;											
						}
					}
					else
					{
						m_bRender		= FALSE;
						m_byRenderIndex	= m_byPageItemCount;
					}															
				}
				else
				{
					// LargeItem
					int nPosX	= (int)(g_Mouse.MousePos.x-(m_fPosX+7));
					int nPosY	= (int)(g_Mouse.MousePos.y-(m_fPosZ+37));
					int nIndex	= (nPosX/34)+((nPosY/68)*7);

					if(nIndex<m_byPageItemCount)
					{
						if(m_bSupplies==FALSE)
						{
							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								int nRealIndex = (m_byCurPage*MAX_ONEPAGE_LARGE) + nIndex;
								g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_TRADERMONSTER_INV_LARGE
												,(BYTE)nRealIndex
												,m_sStoreInfo[nIndex].wItemId
												,0
												,m_dwMonsterIndex);
								m_wSuppliesId	= m_sStoreInfo[nIndex].wItemId;							
								m_bSupplies		= TRUE;
							}
							else
							{
								if(g_pMainPlayer->m_MouseItem.m_wItemID==0)
								{							
									m_wSuppliesId	= m_sStoreInfo[nIndex].wItemId;							
									m_bSupplies		= TRUE;
								}
							}							
						}
					}
					
					m_bRender		= FALSE;
					m_byRenderIndex	= m_byPageItemCount;					
				}
			}												
			else if(eMouseCheck==MOUSE_OVER)
			{			
				if(m_byStoreType==STORE_TYPE_SUSPZO)
				{
					int nPosX	= (int)(g_Mouse.MousePos.x-(m_fPosX+7));
					int nPosY	= (int)(g_Mouse.MousePos.y-(m_fPosZ+37));
					
					int nIndex	= (nPosX/34)+((nPosY/34)*7);
					
					if((nPosX/34)>=7)
					{
						if(!CInterface::GetInstance()->GetIconChk())
							CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);

						m_bRender		= FALSE;
						m_byRenderIndex	= 255;					
					}
					else
					{
						// 화면 출력 데이터 삽입 //
						if(nIndex<m_byPageItemCount)
						{	
							BOOL bChk = TRUE;

							if(m_byStoreType==STORE_TYPE_SUSPZO)
							{
								if(nIndex>=m_wSuppliesCount)
								{								
									m_bRender = FALSE;
									m_byRenderIndex	= 255;
									bChk = FALSE;
								}
							}
							else if(m_byStoreType==STORE_TYPE_WEAPON)
							{
								if(nIndex>=m_wWeaponCount)
								{								
									m_bRender = FALSE;
									m_byRenderIndex	= 255;
									bChk = FALSE;
								}
							}
							else if(m_byStoreType==STORE_TYPE_ARMOR)
							{
								if(nIndex>=m_wArmorCount)
								{								
									m_bRender = FALSE;
									m_byRenderIndex	= 255;
									bChk = FALSE;
								}
							}

							if(bChk)
							{
								if(!CInterface::GetInstance()->GetIconChk())
									CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEM__);
														
								if(CUserInterface::GetInstance()->InterfaceCollision(STORE_WND, (int)(m_fPosX+7+(nIndex%7)*34), (int)(m_fPosX+7+(nIndex%7+1)*34), (int)(m_fPosZ+37+(nIndex/7)*34), (int)(m_fPosZ+37+(nIndex/7+1)*34))==TRUE)
								{								
									m_bRender = TRUE;
									m_byRenderIndex	= (BYTE)nIndex;
								}
								else
								{								
									m_bRender = FALSE;
									m_byRenderIndex	= 255;
								}
							}					
						}	
						else
						{
							if(!CInterface::GetInstance()->GetIconChk())
								CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);

							m_bRender		= FALSE;
							m_byRenderIndex	= 255;						
						}
					}
				}
				else
				{
					// 화면 출력 데이터 삽입 //
					int nPosX	= (int)(g_Mouse.MousePos.x-(m_fPosX+7));
					int nPosY	= (int)(g_Mouse.MousePos.y-(m_fPosZ+37));
					int nIndex	= (nPosX/34)+((nPosY/68)*7);
					
					if(nIndex<m_byPageItemCount)
					{					
						//if(m_byRenderIndex!=nIndex)
						{						
							if(!CInterface::GetInstance()->GetIconChk())
								CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_ITEM__);
							
							int nCoPosX		= (int)m_fPosX+7; 
							int nCoPosX2	= (int)m_fPosX+7+34;
							int nCoPosY		= (int)m_fPosZ+37;
							int nCoPosY2	= (int)m_fPosZ+37+68;

							if(nIndex!=0)
							{
								nCoPosX		= (int)m_fPosX+7+(nIndex%7)*34;
								nCoPosX2	= (int)m_fPosX+7+(nIndex%7+1)*34;
								nCoPosY		= (int)m_fPosZ+37+(nIndex/7)*68;
								nCoPosY2	= (int)m_fPosZ+37+(nIndex/7+1)*68;
							}												

							if(CUserInterface::GetInstance()->InterfaceCollision(STORE_WND, nCoPosX, nCoPosX2, nCoPosY, nCoPosY2)==TRUE)
							{							
								m_bRender = TRUE;
								m_byRenderIndex	= (BYTE)nIndex;							
							}
							else
							{							
								m_bRender = FALSE;
								m_byRenderIndex	= 255;							
							}
						}
					}	
					else
					{
						if(!CInterface::GetInstance()->GetIconChk())
							CUserInterface::GetInstance()->SetMousePointerType(__MOUSE_POINTER_DEFAULT__);

						m_bRender		= FALSE;
						m_byRenderIndex	= 255;
					}
				}
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}	
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_STORE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_STORE_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

BOOL CStoreWnd::SetItemInfo(BYTE byType, WORD wItemId, BYTE byIndex)
{
	CBaseItem* lpItemTable = g_pItemTableHash->GetData(wItemId);

	char		szInfo[0xff] = {0,};
	char		szPrice[0xff] = {0,};
	NUMBERFMT	nFmt = { 0, 0, 3, ".", ",", 1 };		

	DWORD			dwItemSetID	= 0;
	LPSET_ITEM_INFO pSetItemInfo=0;

	if(lpItemTable)
	{		
		int	iInfoRow = 0;
		
		memset(m_sStoreInfo[byIndex].dwColor, 0xff, sizeof(DWORD)*10);

		m_sStoreInfo[byIndex].wItemId		= wItemId;
		m_sStoreInfo[byIndex].dwNameColor	= 0xffffffff;
		__lstrcpyn(m_sStoreInfo[byIndex].szItemName, lpItemTable->szItemName_Kor, sizeof(lpItemTable->szItemName_Kor));		

		// Karz, Option //
		if(byType == STORE_TYPE_WEAPON)
		{
			BOOL bClass = FALSE;

			if( lpItemTable->BaseItem_Weapon.bClassID != 0 )
			{				
				if(lpItemTable->BaseItem_Weapon.bClassID&(BYTE)( 1<<(g_pMainPlayer->m_wClass-1) ))
					bClass = TRUE;				

				// 이름의 컬러를 설정한다.
				if(bClass)
				{
					lstrcpy(m_sStoreInfo[byIndex].szOption[iInfoRow], g_Message[ETC_MESSAGE995].szMessage); // "클래스착용 가능"
					m_sStoreInfo[byIndex].dwColor[iInfoRow] = 0xffffffff;
				}
				else
				{
					lstrcpy(m_sStoreInfo[byIndex].szOption[iInfoRow], g_Message[ETC_MESSAGE996].szMessage); // "클래스착용 불가"
					m_sStoreInfo[byIndex].dwColor[iInfoRow] = TEXT_COLOR_RED;					
				}
				
				iInfoRow++;
			}											

			dwItemSetID	= lpItemTable->GetSetID();

			if(pSetItemInfo && dwItemSetID!=0 && dwItemSetID!=32767)
			{
				pSetItemInfo = g_pSetItemInfoHash->GetData(dwItemSetID);			
			}
			
			if( pSetItemInfo && dwItemSetID != 0 && dwItemSetID != 32767 )
			{
				lstrcpy(m_sStoreInfo[byIndex].szItemName, pSetItemInfo->szItemName_Kor );
			}
			
			/*lstrcat(m_sStoreInfo[byIndex].szItemName, "(" );
			lstrcat(m_sStoreInfo[byIndex].szItemName, lpItemTable->szItemName_Kor );			
			lstrcat(m_sStoreInfo[byIndex].szItemName, ")" );*/
					
			if(!bClass)
				m_sStoreInfo[byIndex].dwNameColor = 0xffff0000;
			else
			{
				if( lpItemTable->BaseItem_Armor.wSetID!=0)
					m_sStoreInfo[byIndex].dwNameColor = TEXT_COLOR_GREEN;
				else
					m_sStoreInfo[byIndex].dwNameColor = 0xffffffff;
			}

			wsprintf(m_sStoreInfo[byIndex].szOption[iInfoRow]
				, g_Message[ETC_MESSAGE858].szMessage
				, lpItemTable->BaseItem_Weapon.wMin_Damage, lpItemTable->BaseItem_Weapon.wMax_Damage );//"Damage %d ~ %d"
			iInfoRow++;
				
			wsprintf( m_sStoreInfo[byIndex].szOption[iInfoRow]
				, g_Message[ETC_MESSAGE997].szMessage
				,  lpItemTable->BaseItem_Weapon.bItemGrade ); // "Grade : %u"			
			iInfoRow++;

			wsprintf( m_sStoreInfo[byIndex].szOption[iInfoRow]
				, g_Message[ETC_MESSAGE998].szMessage
				, lpItemTable->BaseItem_Weapon.wMana_Damage); // "스킬공격력 : %u"
			iInfoRow++;

#ifdef __USE_STAT_LIMIT

			if(g_pMainPlayer->m_dwLevel<lpItemTable->BaseItem_Weapon.wMin_Level)
				m_sStoreInfo[byIndex].dwColor[iInfoRow]	= 0xffff0000;				
			else
				m_sStoreInfo[byIndex].dwColor[iInfoRow]	= 0xffffffff;

			wsprintf( m_sStoreInfo[byIndex].szOption[iInfoRow], "Level : %u", lpItemTable->BaseItem_Weapon.wMin_Level);
			iInfoRow++;

			if(g_pMainPlayer->m_dwStr<lpItemTable->BaseItem_Weapon.wMin_Str)
				m_sStoreInfo[byIndex].dwColor[iInfoRow]	= 0xffff0000;				
			else
				m_sStoreInfo[byIndex].dwColor[iInfoRow]	= 0xffffffff;

			wsprintf( m_sStoreInfo[byIndex].szOption[iInfoRow], "Str : %u", lpItemTable->BaseItem_Weapon.wMin_Str);
			iInfoRow++;

#endif

			wsprintf( m_sStoreInfo[byIndex].szOption[iInfoRow], "Slot : %u", lpItemTable->BaseItem_Weapon.wMax_Slot);
			iInfoRow++;
			
			wsprintf(szInfo, "%u", lpItemTable->BaseItem_Weapon.dwPrice);			

			::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szPrice, 15);
			wsprintf(m_sStoreInfo[byIndex].szPriceName, g_Message[ETC_MESSAGE870].szMessage, szPrice); // "%s Karz"
						
			wsprintf(m_sStoreInfo[byIndex].szOption[iInfoRow], g_Message[ETC_MESSAGE999].szMessage
				, lpItemTable->BaseItem_Weapon.wWeight); // "무게 : %u"
			iInfoRow++;

			m_sStoreInfo[byIndex].byOptionCnt = (BYTE)iInfoRow;
		}
		else if(byType==STORE_TYPE_ARMOR)
		{	
			BOOL bClass = FALSE;

			if( lpItemTable->BaseItem_Armor.bClassID != 0 )
			{
				if(lpItemTable->BaseItem_Armor.bClassID & ( 1<<(g_pMainPlayer->m_wClass-1) ))
					bClass = TRUE;
				

				// 이름의 컬러를 설정한다.
				if(bClass)
				{
					lstrcpy(m_sStoreInfo[byIndex].szOption[iInfoRow], g_Message[ETC_MESSAGE995].szMessage); // "클래스착용 가능"
					m_sStoreInfo[byIndex].dwColor[iInfoRow] = 0xffffffff;
				}
				else
				{
					lstrcpy(m_sStoreInfo[byIndex].szOption[iInfoRow], g_Message[ETC_MESSAGE996].szMessage); // "클래스착용 불가"
					m_sStoreInfo[byIndex].dwColor[iInfoRow] = TEXT_COLOR_RED;					
				}							
				iInfoRow++;
			}

			dwItemSetID	= lpItemTable->GetSetID();

			if( pSetItemInfo && dwItemSetID!=0 && dwItemSetID!=32767)
			{
				pSetItemInfo = g_pSetItemInfoHash->GetData(dwItemSetID);			
			}
			
			if( pSetItemInfo && dwItemSetID != 0 && dwItemSetID != 32767 )
			{
				lstrcpy(m_sStoreInfo[byIndex].szItemName, pSetItemInfo->szItemName_Kor );
			}
			
			/*lstrcat(m_sStoreInfo[byIndex].szItemName, "(" );
			lstrcat(m_sStoreInfo[byIndex].szItemName, lpItemTable->szItemName_Kor );			
			lstrcat(m_sStoreInfo[byIndex].szItemName, ")" );*/
					
			if(!bClass)
				m_sStoreInfo[byIndex].dwNameColor = 0xffff0000;
			else
			{
				if( lpItemTable->BaseItem_Armor.wSetID!=0)
					m_sStoreInfo[byIndex].dwNameColor = TEXT_COLOR_GREEN;
				else
					m_sStoreInfo[byIndex].dwNameColor = 0xffffffff;
			}

			wsprintf(m_sStoreInfo[byIndex].szOption[iInfoRow]
				, g_Message[ETC_MESSAGE104].szMessage, lpItemTable->BaseItem_Armor.wMin_Damage);	// "Defence %d"
			iInfoRow++;

			wsprintf(m_sStoreInfo[byIndex].szOption[iInfoRow]
				, g_Message[ETC_MESSAGE997].szMessage, lpItemTable->BaseItem_Armor.bItemGrade ); // "Grade : %u"
			iInfoRow++;

			wsprintf( m_sStoreInfo[byIndex].szOption[iInfoRow]
				, "Slot : %u", lpItemTable->BaseItem_Armor.wMax_Slot);
			iInfoRow++;

			wsprintf(m_sStoreInfo[byIndex].szOption[iInfoRow]
				, g_Message[ETC_MESSAGE999].szMessage, lpItemTable->BaseItem_Armor.wWeight); // "무게 : %u"
			iInfoRow++;

#ifdef __USE_STAT_LIMIT
			
			if(g_pMainPlayer->m_dwLevel<lpItemTable->BaseItem_Armor.wMin_Level)
				m_sStoreInfo[byIndex].dwColor[iInfoRow]	= 0xffff0000;				
			else
				m_sStoreInfo[byIndex].dwColor[iInfoRow]	= 0xffffffff;

			wsprintf( m_sStoreInfo[byIndex].szOption[iInfoRow], "Level : %u", lpItemTable->BaseItem_Armor.wMin_Level);
			iInfoRow++;

			if(g_pMainPlayer->m_dwStr<lpItemTable->BaseItem_Armor.wMin_Str)
				m_sStoreInfo[byIndex].dwColor[iInfoRow]	= 0xffff0000;				
			else
				m_sStoreInfo[byIndex].dwColor[iInfoRow]	= 0xffffffff;

			wsprintf( m_sStoreInfo[byIndex].szOption[iInfoRow], "Str : %u", lpItemTable->BaseItem_Armor.wMin_Str);
			iInfoRow++;

#endif
			wsprintf(szInfo, "%u", lpItemTable->BaseItem_Armor.dwPrice);			

			::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szPrice, 15);
			wsprintf(m_sStoreInfo[byIndex].szPriceName, g_Message[ETC_MESSAGE870].szMessage, szPrice); // "%s Karz"																	

			m_sStoreInfo[byIndex].byOptionCnt = (BYTE)iInfoRow;						
		}
		else if(byType == STORE_TYPE_BAG)
		{
			wsprintf(szInfo, "%u", lpItemTable->BaseItem_Bag.dwPrice);						

			// Option //
			LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);

			if(lpItemOption)
			{
				for(int i = 0; i < lpItemOption->byCount; i++)
					__lstrcpyn(m_sStoreInfo[byIndex].szOption[i], lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

				m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;
			}	

			::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szPrice, 15);
			wsprintf(m_sStoreInfo[byIndex].szPriceName, g_Message[ETC_MESSAGE870].szMessage, szPrice); // "%s Karz"
			
			m_sStoreInfo[byIndex].byOptionCnt++;
		}
		else if(byType == STORE_TYPE_SUSPZO)
		{
			m_sStoreInfo[byIndex].byOptionCnt = 0;

			switch(lpItemTable->dwCode_ID)
			{
				case ITEM_KIND_MAGICARRAY:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_MagicArray.dwPrice);						

						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);

						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;
						}						
					}
					break;
				case ITEM_KIND_SUPPLIES:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_Supplies.dwPrice);
						
						if(lpItemTable->BaseItem_Supplies.bType == 7)
						{
							wsprintf(m_sStoreInfo[byIndex].szOption[0], g_Message[ETC_MESSAGE1419].szMessage
							, lpItemTable->BaseItem_Supplies.wMin_Lev);
						}
						else if(lpItemTable->BaseItem_Supplies.bType==1 && lpItemTable->BaseItem_Supplies.bType==2)
						{
							wsprintf(m_sStoreInfo[byIndex].szOption[0], "%d%% / %d%%"
							, lpItemTable->BaseItem_Supplies.wMin, lpItemTable->BaseItem_Supplies.wMax);
						}
						else
						{
							wsprintf(m_sStoreInfo[byIndex].szOption[0]
							, "%d / %d", lpItemTable->BaseItem_Supplies.wMin, lpItemTable->BaseItem_Supplies.wMax);
						}
						m_sStoreInfo[byIndex].byOptionCnt = 1;
					}
					break;
				case ITEM_KIND_ZODIAC:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_Zodiac.dwPrice);						

						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);

						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;							
						}
					}
					break;
				case ITEM_KIND_SPECIAL:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_Special.dwPrice);

						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);

						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;							
						}
					}
					break;				
				case ITEM_KIND_MATERIALS:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_Materials.dwPrice);

						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);

						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;							
						}
					}
					break;
				case ITEM_KIND_MIXUPGRADE:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_MixUpgrade.dwPrice);

						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);

						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i], sizeof(lpItemOption->szOption[i]));

							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;							
						}
					}
					break;
				case ITEM_KIND_MAGICFIELDARRAY:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_MagicFieldArray.dwPrice);

						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);

						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i],sizeof(lpItemOption->szOption[i]));

							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;							
						}
					}
					break;
				case ITEM_KIND_UPGRADE:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_UpGrade.dwPrice);
						
						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);
						
						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i],sizeof(lpItemOption->szOption[i]));
							
							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;							
						}
					}
					break;
				case ITEM_KIND_LIQUID:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_LiQuid.dwPrice);
						
						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);
						
						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i],sizeof(lpItemOption->szOption[i]));
							
							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;							
						}
					}
					break;
				case ITEM_KIND_EDITION:
					{
						wsprintf(szInfo, "%u", lpItemTable->BaseItem_Edition.dwPrice);
						
						// Option //
						LP_ITEM_OPTION lpItemOption = (LP_ITEM_OPTION)g_pItemOptionHash->GetData(wItemId);
						
						if(lpItemOption)
						{
							for(int i = 0; i < lpItemOption->byCount; i++)
								__lstrcpyn(m_sStoreInfo[byIndex].szOption[i]
								, lpItemOption->szOption[i],sizeof(lpItemOption->szOption[i]));
							
							m_sStoreInfo[byIndex].byOptionCnt = lpItemOption->byCount;							
						}
					}
					break;
			}			
			
			::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szPrice, 15);
			wsprintf(m_sStoreInfo[byIndex].szPriceName, g_Message[ETC_MESSAGE870].szMessage, szPrice); // "%s Karz"
			
			wsprintf(m_sStoreInfo[byIndex].szOption[m_sStoreInfo[byIndex].byOptionCnt]
				, g_Message[ETC_MESSAGE999].szMessage, lpItemTable->GetWeight());	// "무게 : %u"		
			m_sStoreInfo[byIndex].byOptionCnt++;			
		}
		return TRUE;
	}	
	return FALSE;
}


void CStoreWnd::OpenStoreItem(BYTE byType)
{
	m_byStoreType	= byType;
	m_byCurPage		= 0;

	int nNum	= 0;		

	for(int i = 0; i < STORE_MAX_ITEM_COUNT; i++)
		memset(&m_sStoreInfo[i], 0, sizeof(m_sStoreInfo[i]));

	if(m_byStoreType == STORE_TYPE_WEAPON)
	{
		if(m_wWeaponCount%STORE_MAX_ITEM_COUNT2==0)
			m_byMaxPage = BYTE(m_wWeaponCount/STORE_MAX_ITEM_COUNT2);
		else
			m_byMaxPage = BYTE((m_wWeaponCount/STORE_MAX_ITEM_COUNT2)+1);
		
		if(m_wWeaponCount>=STORE_MAX_ITEM_COUNT2)
			m_byPageItemCount = STORE_MAX_ITEM_COUNT2;
		else
			m_byPageItemCount = (BYTE)m_wWeaponCount;			

		ListNode<ITEM_STORE>*	lpItemStoreNode;
		LP_ITEM_STORE			lpItemStore;
			
		lpItemStoreNode = g_pItemStoreHash->GetHead();

		while(lpItemStoreNode)
		{	
			lpItemStore	= lpItemStoreNode->pData;						

			if(lpItemStore)
			{
				if(lpItemStore->wMapId == 0 || lpItemStore->wMapId == m_dwMapId)
				{
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(lpItemStore->wItemId);

					if(lpItemTable)
					{
						if(lpItemTable->dwCode_ID == ITEM_KIND_WEAPON)
						{
							SetItemInfo(STORE_TYPE_WEAPON, lpItemStore->wItemId, (BYTE)nNum);
							nNum++;

							if(nNum==m_byPageItemCount)
								break;
						}
					}
				}
			}
			
			lpItemStoreNode = lpItemStoreNode->pNext;
		}
	}
	else if(m_byStoreType == STORE_TYPE_ARMOR)
	{
		if(m_wArmorCount % STORE_MAX_ITEM_COUNT2 == 0)
			m_byMaxPage = BYTE(m_wArmorCount/STORE_MAX_ITEM_COUNT2);
		else
			m_byMaxPage = BYTE((m_wArmorCount/STORE_MAX_ITEM_COUNT2)+1);
		
		if(m_wArmorCount>=STORE_MAX_ITEM_COUNT2)
			m_byPageItemCount = STORE_MAX_ITEM_COUNT2;
		else
			m_byPageItemCount = (BYTE)m_wArmorCount;		

		ListNode<ITEM_STORE>*	lpItemStoreNode;
		LP_ITEM_STORE			lpItemStore;
			
		lpItemStoreNode = g_pItemStoreHash->GetHead();

		while(lpItemStoreNode)
		{	
			lpItemStore	= lpItemStoreNode->pData;						

			if(lpItemStore->wMapId == 0 || lpItemStore->wMapId == m_dwMapId)
			{
				CBaseItem* lpItemTable = g_pItemTableHash->GetData(lpItemStore->wItemId);

				if(lpItemTable->dwCode_ID == ITEM_KIND_BAG)
				{
					SetItemInfo(STORE_TYPE_BAG, lpItemStore->wItemId, (BYTE)nNum);
					nNum++;	
					
					if(nNum == m_byPageItemCount)
						break;
				}
				else if(lpItemTable->dwCode_ID == ITEM_KIND_AROMR)
				{
					SetItemInfo(STORE_TYPE_ARMOR, lpItemStore->wItemId, (BYTE)nNum);
					nNum++;	
					
					if(nNum == m_byPageItemCount)
						break;
				}								
			}
			
			lpItemStoreNode = lpItemStoreNode->pNext;
		}
	}
	else if(m_byStoreType == STORE_TYPE_SUSPZO)
	{
		if(m_wSuppliesCount%STORE_MAX_ITEM_COUNT==0)
			m_byMaxPage = BYTE(m_wSuppliesCount/STORE_MAX_ITEM_COUNT);
		else
			m_byMaxPage = BYTE((m_wSuppliesCount/STORE_MAX_ITEM_COUNT)+1);
		
		if(m_wSuppliesCount>=STORE_MAX_ITEM_COUNT)
			m_byPageItemCount = STORE_MAX_ITEM_COUNT;
		else
			m_byPageItemCount = (BYTE)m_wSuppliesCount;			

		ListNode<ITEM_STORE>*	lpItemStoreNode;
		LP_ITEM_STORE			lpItemStore;		
			
		lpItemStoreNode = g_pItemStoreHash->GetHead();

		while(lpItemStoreNode)
		{	
			lpItemStore	= lpItemStoreNode->pData;						

			if(lpItemStore)
			{
				if(lpItemStore->wMapId==0 || lpItemStore->wMapId==m_dwMapId)
				{
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(lpItemStore->wItemId);

					if(lpItemTable)
					{
						if(GetType(WORD(lpItemStore->wItemId/ITEM_DISTRIBUTE))==ITEM_SMALL)
						{
							SetItemInfo(STORE_TYPE_SUSPZO, lpItemStore->wItemId, (BYTE)nNum);
							nNum++;
							
							if(nNum==m_byPageItemCount)
								break;
						}						
					}				
				}
			}
			
			lpItemStoreNode = lpItemStoreNode->pNext;
		}				
	}
}


void CStoreWnd::SetPrevNext(BYTE bPNType)
{
	if(bPNType==__STORE_PAGE_PREV__)
	{
		if(m_byCurPage==0)
			return;

		m_byCurPage -= 1;
	}
	else
	{
		if(m_byCurPage==m_byMaxPage-1)
			return;

		m_byCurPage += 1;
	}

	if(m_byStoreType==STORE_TYPE_WEAPON)
	{
		if(m_wWeaponCount/STORE_MAX_ITEM_COUNT2>m_byCurPage)
			m_byPageItemCount = STORE_MAX_ITEM_COUNT2;
		else
			m_byPageItemCount = BYTE(m_wWeaponCount%STORE_MAX_ITEM_COUNT2);
				
		int nIndex		= m_wSuppliesCount + m_wWeaponCount + m_wArmorCount;
		int nNum		= 0;
		int nPageCount	= 0;

		for(int i = 0; i < nIndex; i++)
		{
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(m_wItemId[i]);

			if(lpItemTable->dwCode_ID==ITEM_KIND_WEAPON)
			{
				if(nPageCount>=STORE_MAX_ITEM_COUNT2*m_byCurPage)
				{
					SetItemInfo(STORE_TYPE_WEAPON, m_wItemId[i], (BYTE)nNum);
					nNum++;

					if(nNum==m_byPageItemCount)
						break;
				}
				nPageCount++;
			}
		}
	}
	else if(m_byStoreType==STORE_TYPE_ARMOR)
	{
		if(m_wArmorCount/STORE_MAX_ITEM_COUNT2>m_byCurPage)
			m_byPageItemCount = STORE_MAX_ITEM_COUNT2;
		else
			m_byPageItemCount = BYTE(m_wArmorCount%STORE_MAX_ITEM_COUNT2);
	
		int nIndex		= m_wSuppliesCount + m_wWeaponCount + m_wArmorCount;
		int nNum		= 0;
		int nPageCount	= 0;

		for(int i = 0; i < nIndex; i++)
		{
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(m_wItemId[i]);

			if(lpItemTable)
			{
				if(lpItemTable->dwCode_ID==ITEM_KIND_AROMR)
				{
					if(nPageCount>=STORE_MAX_ITEM_COUNT2*m_byCurPage)
					{
						SetItemInfo(STORE_TYPE_ARMOR, m_wItemId[i], (BYTE)nNum);
						nNum++;

						if(nNum==m_byPageItemCount)
							break;
					}
					nPageCount++;
				}
				else if(lpItemTable->dwCode_ID==ITEM_KIND_BAG)
				{
					if(nPageCount>=STORE_MAX_ITEM_COUNT2*m_byCurPage)
					{
						SetItemInfo(STORE_TYPE_BAG, m_wItemId[i], (BYTE)nNum);
						nNum++;

						if(nNum==m_byPageItemCount)
							break;
					}
					nPageCount++;
				}
			}
		}		
	}
	else if(m_byStoreType==STORE_TYPE_SUSPZO)
	{
		if(m_wSuppliesCount/STORE_MAX_ITEM_COUNT2>m_byCurPage)
			m_byPageItemCount = STORE_MAX_ITEM_COUNT2;
		else
			m_byPageItemCount = BYTE(m_wSuppliesCount%STORE_MAX_ITEM_COUNT2);
		
		int nIndex		= m_wSuppliesCount + m_wWeaponCount + m_wArmorCount;
		int nNum		= 0;		
		int nPageCount	= 0;

		for(int i = 0; i < nIndex; i++)
		{
			CBaseItem* lpItemTable = g_pItemTableHash->GetData(m_wItemId[i]);

			if(lpItemTable)
			{
				if( lpItemTable->dwCode_ID==ITEM_KIND_SUPPLIES ||
					lpItemTable->dwCode_ID==ITEM_KIND_ZODIAC ||
					lpItemTable->dwCode_ID==ITEM_KIND_SPECIAL)
				{
					if(nPageCount>=STORE_MAX_ITEM_COUNT2*m_byCurPage)
					{
						SetItemInfo(STORE_TYPE_SUSPZO, m_wItemId[i], (BYTE)nNum);
						nNum++;
						
						if(nNum==m_byPageItemCount)
							break;
					}
					nPageCount++;
				}
			}
		}
	}
}

void CStoreWnd::RenderItem()
{
	int nOrder	= __ORDER_INTERFACE_START__ + m_byOrder*10;
	
	VECTOR2	vPos;					

	for(int i = 0; i < m_byPageItemCount; i++)
	{
		if(m_sStoreInfo[i].wItemId!=0)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = g_pItemResourceHash->GetData(m_sStoreInfo[i].wItemId);
						
			if(lpItemResourceEx)
			{
				if(m_byStoreType==STORE_TYPE_SUSPZO)
				{
					vPos.x	= m_fPosX + 6 + (i%7)*34;
					vPos.y	= m_fPosZ + 36 + (i/7)*34;					
				}			
				else
				{
					vPos.x	= m_fPosX + 6 + (i%7)*34;
					vPos.y	= m_fPosZ + 36 + (i/7)*68;
				}
				
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}			
		}							
	}
}
