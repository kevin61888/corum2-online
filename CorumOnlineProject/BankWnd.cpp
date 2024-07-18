//======================================================//
// Code by Jang.							2003.03.19	//
//======================================================//
#include "BankWnd.h"
#include "Interface.h"
#include "UserInterface.h"
#include "QuantityWnd.h"
#include "ChatWnd.h"
#include "GameControl.h"
#include "User.h"
#include "HashTable.h"
#include "ItemWnd.h"
#include "message.h"
#include "DungeonTable.h"
#include "InputManager.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "initgame.h"
#include "CodeFun.h"


CBankWnd* CBankWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CBankWnd::CBankWnd()
{
	m_byMoneyChk	= 0;	
	m_byBankChk		= 0;
}

CBankWnd::~CBankWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CBankWnd::Init()
{
	CInterface* pInterface = CInterface::GetInstance();

	m_byServer = pInterface->m_byServer;

	if(pInterface->m_byServer==__SERVER_TYPE_VILL__)
	{
		/*
		InsertData(SPR_OBJ_BANK_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
		InsertData(SPR_OBJ_BANK_WND2, SPR_INTERFACE_DF_WND2, 0, 48, 1.0f, 22.0f, 0);
		InsertData(SPR_OBJ_BANK_WND3, SPR_INTERFACE_BANK_WND3, 0, 400, 1.0f, 1.0f, 0);
		InsertData(SPR_OBJ_BANK_CHK1, SPR_INTERFACE_BANK_CHK1, 6, 24, 1.0f, 1.0f, 2);
		InsertData(SPR_OBJ_BANK_CHK2, SPR_INTERFACE_BANK_CHK2, 6, 24, 1.0f, 1.0f, 2);
		InsertData(SPR_OBJ_BANK_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 2);
		InsertData(SPR_OBJ_BANK_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 2);		
		InsertData(SPR_OBJ_BANK_BAR, SPR_INTERFACE_BANK_BAR, 0, 32, 1.0f, 1.0f, 0);
		InsertData(SPR_OBJ_SC_BG, SPR_INTERFACE_SC_BG, 245, 48, 1.0f, 176.5f, 2);
		InsertData(SPR_OBJ_BANK_SC, SPR_INTERFACE_SC, 243, 48, 1.0f, 1.0f, 3);											

		InsertData(SPR_OBJ_BANK_MONEY1, SPR_INTERFACE_MONEY1, 23, 405, 1.0f, 1.0f, 3);
		InsertData(SPR_OBJ_BANK_MONEY2, SPR_INTERFACE_MONEY2, 23, 405, 1.0f, 1.0f, 3);
		InsertData(SPR_OBJ_BANK_MONEY3, SPR_INTERFACE_MONEY3, 23, 405, 1.0f, 1.0f, 3);

		InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
		InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
		InsertCheckInterface(19, 29, 62, 41, 4, CHECK_BTN);
		InsertCheckInterface(75, 29, 119, 41, 5, CHECK_BTN);
		InsertCheckInterface(243, 48, 253, 400, 6, CHECK_SCROLL);
		InsertCheckInterface(4, 50, 242, 400, 7, CHECK_ITEM);
		InsertCheckInterface(23, 410, 232, 426, 8, CHECK_BTN);										
		*/
		SetScroll(__MAX_BANK_SC__, 48, 400);

		m_bInit = TRUE;
	}		

	m_pSprLarge	= g_pRenderer->CreateSpriteObject(GetFile("menu_4.tif", DATA_TYPE_UI), 144, 1, 34, 68, 0);
	m_pSprSmall	= g_pRenderer->CreateSpriteObject(GetFile("menu_4.tif", DATA_TYPE_UI), 190, 115, 34, 34, 0);
	
	return TRUE;
}

void CBankWnd::Remove()
{
	if(m_byServer==__SERVER_TYPE_VILL__)
	{
		RemoveAllData();		
		m_bActive	= FALSE;
		m_bInit		= FALSE;
	}

	if(m_pSprLarge)
		m_pSprLarge->Release();
	if(m_pSprSmall)
		m_pSprSmall->Release();
}

void CBankWnd::SetOrder()
{	
}

void CBankWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
		
	if(m_byServer==__SERVER_TYPE_VILL__)
	{
		if(bActive==FALSE)
		{
			ShowSpriteAll();			
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
		}
		else
		{
			CInterface::GetInstance()->SetWnd(m_nIndex);

			SetRender(SPR_OBJ_BANK_WND1, TRUE);
			SetRender(SPR_OBJ_BANK_WND2, TRUE);
			SetRender(SPR_OBJ_BANK_WND3, TRUE);
			SetRender(SPR_OBJ_BANK_CLOSE1, TRUE);
			SetRender(SPR_OBJ_BANK_BAR, TRUE);
			SetRender(SPR_OBJ_SC_BG, TRUE);
			SetRender(SPR_OBJ_BANK_SC, TRUE);
			SetRender(SPR_OBJ_BANK_MONEY1, TRUE);
			SetRender(SPR_OBJ_BANK_CHK1, FALSE);
			SetRender(SPR_OBJ_BANK_CHK2, FALSE);

// Begin - minjin 2004. 09. 10.
#if IS_JAPAN_LOCALIZING()
// Added Codes for Initializing Bank Window
// 우선.. SetActive() 에 있는 코드들을 가져다가 박아놨다..
// 필요없는 코드들도 있을텐데.. -_-;;
// 확인해보고 나중에 반드시 손봐야 함!!
			if(m_byBankChk==0)			
			{
				SetRender(SPR_OBJ_BANK_CHK1, TRUE);
				SetRender(SPR_OBJ_BANK_CHK2, FALSE);				
				SetPosObjY(SPR_OBJ_BANK_SC, 48);				
				m_byBankChk		= 0;
//				m_szSCTemp		= 0;
				m_nCurScrollPos	= 0;
				m_nScrollPos	= __MAX_BANK_SC__;		
			}
			else
			{
				SetRender(SPR_OBJ_BANK_CHK1, FALSE);
				SetRender(SPR_OBJ_BANK_CHK2, TRUE);				
				SetPosObjY(SPR_OBJ_BANK_SC, 48);
				m_byBankChk		= 0;
//				m_szSCTemp		= 0;
				m_nCurScrollPos	= 0;
				m_nScrollPos	= __MAX_BANK_SC__;
				m_byBankChk		= 1;
			}
#else
// Existed Codes
			if(m_byBankChk==0)			
				SetRender(SPR_OBJ_BANK_CHK1, TRUE);	
			else
				SetRender(SPR_OBJ_BANK_CHK2, TRUE);	
#endif
// End - minjin
			
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
		}
	}
}

int CBankWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(nRt!=8 && m_byMoneyChk!=0)
	{
		SetRender(SPR_OBJ_BANK_MONEY1, TRUE);
		SetRender(SPR_OBJ_BANK_MONEY2, FALSE);
		SetRender(SPR_OBJ_BANK_MONEY3, FALSE);
		m_byMoneyChk = 0;
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
				SetRender(SPR_OBJ_BANK_CLOSE1, FALSE);
				SetRender(SPR_OBJ_BANK_CLOSE2, TRUE);
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
				SetRender(SPR_OBJ_BANK_CHK1, TRUE);
				SetRender(SPR_OBJ_BANK_CHK2, FALSE);				
				SetPosObjY(SPR_OBJ_BANK_SC, 48);				
				m_byBankChk		= 0;
				m_fSCTemp		= 0;
				m_nCurScrollPos	= 0;
				m_nScrollPos	= __MAX_BANK_SC__;				
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_BANK_CHK1, FALSE);
				SetRender(SPR_OBJ_BANK_CHK2, TRUE);				
				SetPosObjY(SPR_OBJ_BANK_SC, 48);
				m_byBankChk		= 0;
				m_fSCTemp		= 0;
				m_nCurScrollPos	= 0;
				m_nScrollPos	= __MAX_BANK_SC__;
				m_byBankChk		= 1;
			}			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(GetCheckScroll())
						SetPosObjY(SPR_OBJ_BANK_SC, m_fScrollPos-m_fPosZ);
				}
				else
				{
					if(g_pMainPlayer->m_MouseItem.m_wItemID==0)
					{
						if(GetCheckScroll())
							SetPosObjY(SPR_OBJ_BANK_SC, m_fScrollPos-m_fPosZ);
					}
				}				
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 7:
		{					
			BOOL bItemExist	= FALSE;
			BYTE byType = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{
				CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();

				if(!pQuantityWnd->GetActive())
				{
					if(m_byBankChk==0)
					{
						if(g_pMainPlayer->m_MouseItem.m_wItemID==0)
						{						
							int nItemChk	= m_nCurScrollPos*7;
							int nPosX		= (g_Mouse.MousePos.x-(int)m_fPosX-4)/34;
							int nPosY		= (g_Mouse.MousePos.y-(int)m_fPosZ-50)/68;
							int nIndex		= (nItemChk+nPosX)+(nPosY*7);

							if(g_pMainPlayer->m_pBank_Large[nIndex].m_wItemID!=0)
							{
								CTDS_ITEM_PICKUP ItemPickup;							

								if(nIndex<MAX_BANK_LARGE_POOL)
								{
									int nVal = g_pMainPlayer->m_pBank_Large[nIndex].m_wItemID/ITEM_DISTRIBUTE;
									WORD wID = g_pMainPlayer->m_pBank_Large[nIndex].m_wItemID;

									if(nVal<=20)
									{									
										ItemPickup.bSectionNum	= 1;
										ItemPickup.i64ItemID	= 0;
										ItemPickup.bInv			= 26;
										ItemPickup.bZipCode		= nIndex;
										g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE );
										
										if(g_ItemMoveManager.GetNewItemMoveMode())	
											g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_BANK_LARGE,nIndex,wID,0);										
									}
								}
							}
						}
					}
					else if(m_byBankChk==1)
					{
						if(g_pMainPlayer->m_MouseItem.m_wItemID==0)
						{											
							int nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-4)/34;
							int nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-50)/34;
							int nIndex	= nPosX+(nPosY*7);

							if(g_pMainPlayer->m_pBank_Small[nIndex].m_wItemID!=0)
							{							
								CTDS_ITEM_PICKUP ItemPickup;							

								if(nIndex<MAX_BANK_SMALL_POOL)
								{							
									WORD wID	= g_pMainPlayer->m_pBank_Small[nIndex].m_wItemID; 
								
									if(g_pMainPlayer->m_pBank_Small[nIndex].GetType()==ITEM_SMALL)
									{
										ItemPickup.bSectionNum	= 1;
										ItemPickup.i64ItemID	= 0;
										ItemPickup.bInv			= 27;
										ItemPickup.bZipCode		= nIndex;
										g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
										
										if(g_ItemMoveManager.GetNewItemMoveMode())	
											g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_BANK_SMALL,nIndex,wID,0);
									}
								}
							}
						}
					}
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				int		nPosX		= 0;
				int		nPosY		= 0;
				int		nIndex		= 0;				

				if(m_byBankChk==0)
				{
					int nItemChk	= m_nCurScrollPos*7;
					nPosX			= (g_Mouse.MousePos.x-(int)m_fPosX-4)/34;
					nPosY			= (g_Mouse.MousePos.y-(int)m_fPosZ-50)/68;
					nIndex			= (nItemChk+nPosX)+(nPosY*7);
					bItemExist		= (g_pMainPlayer->m_pBank_Large[nIndex].m_wItemID!=0);
				}
				else if(m_byBankChk==1)
				{
					nPosX		= (g_Mouse.MousePos.x-(int)m_fPosX-4)/34;
					nPosY		= (g_Mouse.MousePos.y-(int)m_fPosZ-50)/34;
					nIndex		= nPosX+(nPosY*7);
					bItemExist	= (g_pMainPlayer->m_pBank_Small[nIndex].m_wItemID!=0);
				}

				if(bItemExist==0)
				{
					if(!g_Mouse.bLDown)
						byType = __MOUSE_POINTER_DEFAULT__;
				}
			}
			else if(eMouseCheck==MOUSE_UP)
			{
				int		nPosX		= 0;
				int		nPosY		= 0;
				int		nIndex		= 0;				

				if(m_byBankChk==0)
				{
					int nItemChk	= m_nCurScrollPos*7;
					nPosX			= (g_Mouse.MousePos.x-(int)m_fPosX-4)/34;
					nPosY			= (g_Mouse.MousePos.y-(int)m_fPosZ-50)/68;
					nIndex			= (nItemChk+nPosX)+(nPosY*7);
					bItemExist		= (g_pMainPlayer->m_pBank_Large[nIndex].m_wItemID!=0);
				}
				else if(m_byBankChk==1)
				{
					nPosX		= (g_Mouse.MousePos.x-(int)m_fPosX-4)/34;
					nPosY		= (g_Mouse.MousePos.y-(int)m_fPosZ-50)/34;
					nIndex		= nPosX+(nPosY*7);
					bItemExist	= (g_pMainPlayer->m_pBank_Small[nIndex].m_wItemID!=0);
				}
			}		
			
			SetMouseCheck(nRt, eMouseCheck, byType);
		}
		break;
	case 8:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				CInterface*		pInterface		= CInterface::GetInstance();
				CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
				CChatWnd *		pChatWnd		= CChatWnd::GetInstance();
				
				SetRender(SPR_OBJ_BANK_MONEY1, FALSE);
				SetRender(SPR_OBJ_BANK_MONEY2, TRUE);
				SetRender(SPR_OBJ_BANK_MONEY3, FALSE);
				m_byMoneyChk = 1;				

				if(g_pGVDungeon->bChatMode)
				{
					g_pInputManager->SetFocusInput(INPUT_BUFFER_19);

					if(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19))
					{
						if(!IsEmptyString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)))
						{						
							memset(pChatWnd->m_szClip, 0, sizeof(pChatWnd->m_szClip));
							__lstrcpyn(pChatWnd->m_szClip, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)
								, lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)));
							pChatWnd->m_byClipType = 1;
						}
					}
					
					ChatModeOff(INPUT_BUFFER_19);
				}
				
				if(pQuantityWnd->GetActive()==FALSE)
				{
					pQuantityWnd->OpenWnd(__QUANTITY_BNAKMONEYPOP__);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_BANK_KARZ,0xff,0,0,0);
					}
				}
				else
				{
					pQuantityWnd->SetActive(FALSE);
				}
				
				pInterface->SetWnd(QUANTITY_WND);
				
				if(pQuantityWnd->GetActive()==TRUE)
				{						
					// 채팅모드이면 초기화.
					int nOrder	= GetStartOrder()+2;

					RECT rt;
					rt.left		= (int)pQuantityWnd->m_fPosX+124-7;
					rt.right	= (int)pQuantityWnd->m_fPosX+124;
					rt.top		= (int)pQuantityWnd->m_fPosZ+83;
					rt.bottom	= (int)pQuantityWnd->m_fPosZ+95;																					
					g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
					g_pInputManager->SetFocusInput(INPUT_BUFFER_5);
				}
				else
				{
					memset(pQuantityWnd->m_szMoney, 0, sizeof(pQuantityWnd->m_szMoney));
					pQuantityWnd->m_byDrawIndex =	0;	
					g_pInputManager->SetFocusInput(0);
					g_pInputManager->GetInputBuffer(INPUT_BUFFER_5);						
				}
				
				m_byMoneyChk = 1;								
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{		
				SetRender(SPR_OBJ_BANK_MONEY1, FALSE);
				SetRender(SPR_OBJ_BANK_MONEY2, FALSE);
				SetRender(SPR_OBJ_BANK_MONEY3, TRUE);			
				m_byMoneyChk = 2;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_BANK_MONEY1, FALSE);
				SetRender(SPR_OBJ_BANK_MONEY2, TRUE);
				SetRender(SPR_OBJ_BANK_MONEY3, FALSE);						
				m_byMoneyChk = 1;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
		
	if(MouseScrollChk(nRt, 6))	
	{
		SetPosObjY(SPR_OBJ_BANK_SC, m_fScrollPos-m_fPosZ);		
		return CInterface::GetInstance()->GetSetWnd();
	}
	
	if(MouseUp())
	{		
		SetRender(SPR_OBJ_BANK_CLOSE1, TRUE);
		SetRender(SPR_OBJ_BANK_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}


void CBankWnd::RenderText()
{
	int	nSize			= 0;
	int nPos			= 0;
	char szInfo[0xff]	= {0,};			
	NUMBERFMT nFmt		= { 0, 0, 3, ".", ",", 1 };

	if(m_byServer==__SERVER_TYPE_VILL__)
	{	
		RenderBank();
							
		RenderFont(g_Message[ETC_MESSAGE0].szMessage, m_fPosX+20, m_fPosX+50, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+2);
		
		if(m_byBankChk==1)
		{	
			for(int i = 0;  i < MAX_BANK_SMALL_POOL; i++)	
			{
				if(g_pMainPlayer->m_pBank_Small[i].m_wItemID!=0)
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						if(g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_BANK_SMALL &&
							i == g_ItemMoveManager.GetNativeSrcIndex())
						{
							continue;
						}
					}

					if(g_pMainPlayer->m_pBank_Small[i].GetQuantity() <= 1 )
						continue;

					wsprintf(szInfo, "%d", g_pMainPlayer->m_pBank_Small[i].GetQuantity());

					// 좌측 상단 좌표 //
					int nPosX	= (int)(m_fPosX+4)+(i%7)*34;
					int nPosY	= (int)(m_fPosZ+50)+(i/7)*34;
					int nSize	= lstrlen(szInfo);
					
					RenderFont(szInfo, nPosX+30-(nSize+1)*6, nPosX+30, nPosY+20, nPosY+32, GetStartOrder()+3);
				}
			}
		}		
		
		char szMoneyInfo[0xFF] = {0,};
		wsprintf(szInfo, "%u", g_pMainPlayer->m_dwBankMoney);
		
		::GetNumberFormat(NULL, NULL, szInfo, &nFmt, szMoneyInfo, 15);
		
		nSize = lstrlen(szMoneyInfo);

		if(m_byMoneyChk==2)
			nPos = 1;
		
		RenderFont(szMoneyInfo, m_fPosX+187-nSize*7, m_fPosX+256, m_fPosZ+408+nPos, m_fPosZ+422+nPos, GetStartOrder()+4);
		
		RenderItemInfo();
	}
}

void CBankWnd::RenderItemInfo()
{
	CItem*	sItem;
	int		nItemChk	= 0;
	int		nPosX		= 0;
	int		nPosY		= 0;
	int		nIndex		= 0;

	if( g_Mouse.MousePos.x>m_fPosX+4 &&
		g_Mouse.MousePos.x<m_fPosX+242 &&
		g_Mouse.MousePos.y>m_fPosZ+50 &&
		g_Mouse.MousePos.y<m_fPosZ+400)
	{
		if(m_byBankChk==0)
		{
			nItemChk	= m_nCurScrollPos*7;
			nPosX		= (g_Mouse.MousePos.x-(int)m_fPosX-4)/34;
			nPosY		= (g_Mouse.MousePos.y-(int)m_fPosZ-50)/68;
			nIndex		= (nItemChk+nPosX)+(nPosY*7);

			if(nIndex>=MAX_BANK_LARGE_POOL)
				return;
			else if(CUserInterface::GetInstance()->InterfaceCollision(BANK_WND
				, (int)(m_fPosX+4+nPosX*34)
				, (int)(m_fPosX+4+(nPosX+1)*34)
				, (int)(m_fPosZ+50+nPosY*68)
				, (int)(m_fPosZ+50+(nPosY+1)*68))==FALSE)
				return;				
			else if(g_pMainPlayer->m_pBank_Large[nIndex].m_wItemID==0)
				return;

			sItem = &g_pMainPlayer->m_pBank_Large[nIndex];

			if(!sItem)
				return;
		}
		else
		{
			nPosX	= (g_Mouse.MousePos.x-(int)m_fPosX-4)/34;
			nPosY	= (g_Mouse.MousePos.y-(int)m_fPosZ-50)/34;
			nIndex	= nPosX+(nPosY*7);

			if(nIndex>=MAX_BANK_SMALL_POOL)			
				return;
			else if(CUserInterface::GetInstance()->InterfaceCollision(BANK_WND
				, (int)(m_fPosX+4+nPosX*34)
				, (int)(m_fPosX+4+(nPosX+1)*34)
				, (int)(m_fPosZ+50+nPosY*34)
				, (int)(m_fPosZ+50+(nPosY+1)*34))==FALSE)			
				return;			
			else if(g_pMainPlayer->m_pBank_Small[nIndex].m_wItemID==0)			
				return;
			
			sItem = &g_pMainPlayer->m_pBank_Small[nIndex];

			if(!sItem)
				return;
		}

		CInterface::GetInstance()->ItemInfoRender(sItem, FALSE);
	}
}

void CBankWnd::RenderLineBank()
{	
	VECTOR2	vPos;

	if(CInterface::GetInstance()->m_byServer==__SERVER_TYPE_VILL__)
	{
		if(m_byBankChk==0)
		{		
			for(int i = 0; i < 7*5; i++)
			{
				vPos.x		= m_fPosX+4+34*(i%7);
				vPos.y		= m_fPosZ+48+68*(i/7);
				g_pRenderer->RenderSprite(m_pSprLarge
					, NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+2, RENDER_TYPE_DISABLE_TEX_FILTERING);								
			}					
		}
		else
		{
			for(int i = 0; i < MAX_BANK_SMALL_POOL; i++)
			{
				vPos.x		= m_fPosX+4+34*(i%7);
				vPos.y		= m_fPosZ+48+34*(i/7);
				g_pRenderer->RenderSprite(m_pSprSmall
					, NULL, 0.0f, &vPos, NULL, 0xffffffff, GetStartOrder()+2, RENDER_TYPE_DISABLE_TEX_FILTERING);				
			}
		}
	}
}

void CBankWnd::RenderBank()
{
	int nIndex	= 0;
	int nOrder	= __ORDER_INTERFACE_START__ + m_byOrder*10;
		
	VECTOR2	vPos;						
	
	if(CInterface::GetInstance()->m_byServer==__SERVER_TYPE_VILL__)
	{
		RenderLineBank();

		if(m_byBankChk==0)
		{
			for(int i = m_nCurScrollPos * 7; i < (m_nCurScrollPos+5) * 7; i++)
			{
				if(g_pMainPlayer->m_pBank_Large[i].m_wItemID!=0)
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_BANK_LARGE &&
							i == g_ItemMoveManager.GetNativeSrcIndex())
						{
							nIndex++;
							continue;
						}
					}

					LP_ITEM_RESOURCE_EX lpItemResourceEx = 
						g_pItemResourceHash->GetData(g_pMainPlayer->m_pBank_Large[i].m_wItemID);
						
					if(lpItemResourceEx)
					{
						int nPosX = 4+(nIndex%7)*34;
						int nPosY = 50+(nIndex/7)*68;
									
						vPos.x	= m_fPosX+nPosX;
						vPos.y	= m_fPosZ+nPosY;								
						g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
							, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);					
					}										
				}
				
				nIndex++;
			}
		}
		else
		{
			for(int i = 0; i < MAX_BANK_SMALL_POOL; i++)
			{				
				if(g_pMainPlayer->m_pBank_Small[i].m_wItemID!=0)				
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_BANK_SMALL &&
							i == g_ItemMoveManager.GetNativeSrcIndex())
						{
							continue;
						}
					}

					LP_ITEM_RESOURCE_EX lpItemResourceEx = 
						g_pItemResourceHash->GetData(g_pMainPlayer->m_pBank_Small[i].m_wItemID);
						
					if(lpItemResourceEx)
					{
						int nPosX = 4+(i%7)*34;
						int nPosY = 50+(i/7)*34;
									
						vPos.x	= m_fPosX+nPosX;
						vPos.y	= m_fPosZ+nPosY;						
						g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
							, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+2, RENDER_TYPE_DISABLE_TEX_FILTERING);						
					}										
				}
				nIndex++;
			}
		}		
	}
}