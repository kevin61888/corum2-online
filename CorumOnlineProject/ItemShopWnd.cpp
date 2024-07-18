//======================================================//
// Code by Jang.							2003.02.04	//
//======================================================//
#include "ItemShopWnd.h"
#include "GameControl.h"
#include "NpcDungeonInfoWnd.h"
#include "Interface.h"
#include "UserInterface.h"
#include "DSMiniMap.h"
#include "DungeonTable.h"
#include "StoreWnd.h"
#include "BankWnd.h"
#include "ItemWnd.h"
#include "Interface.h"
#include "ItemUpgrade.h"
#include "ItemUpgradeHelpWnd.h"
#include "initgame.h"
#include "ItemTradeShopWnd.h"
#include "Chat.h"
#include "message.h"
#include "ItemMakingWnd.h"
#include "ItemMakingHelpWnd.h"
#include "CodeFun.h"

CItemShopWnd* CItemShopWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CItemShopWnd::CItemShopWnd()
{
	m_byTypeRt		= 0;	
	m_byChatLine	= 0;	
	m_bActiveChk	= FALSE;
}

CItemShopWnd::~CItemShopWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CItemShopWnd::Init()
{			
	for(int i = 0; i < 5; i++)
		memset(&m_sDgChat[i], 0, sizeof(__tag_dg_chat__));	

	DWORD dwId[18] =
	{
		BUTTON_OBJ_TRADE1, BUTTON_OBJ_TRADE2, BUTTON_OBJ_TRADE3, BUTTON_OBJ_MAKE1, BUTTON_OBJ_MAKE2, BUTTON_OBJ_MAKE3,
		BUTTON_OBJ_STR1, BUTTON_OBJ_STR2, BUTTON_OBJ_STR3, BUTTON_OBJ_INFO1, BUTTON_OBJ_INFO2, BUTTON_OBJ_INFO3,
		SPR_OBJ_BANK_BTN1, SPR_OBJ_BANK_BTN2, SPR_OBJ_BANK_BTN3, SPR_OBJ_ITEMSHOPWND, SPR_OBJ_ITEMSHOPWND_CLOSE1, SPR_OBJ_ITEMSHOPWND_CLOSE2, 
	};

	for(i = 0; i < 18; i++)
		m_dwId[i] = dwId[i];

/*
	InsertData(SPR_OBJ_ITEMSHOPWND, SPR_INTERFACE_ITEMSHOPWND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_ITEMSHOPWND_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ITEMSHOPWND_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	

	InsertData(BUTTON_OBJ_MAKE1, BUTTON_INTERFACE_MAKE1, 64, 98, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_MAKE2, BUTTON_INTERFACE_MAKE2, 64, 98, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_MAKE3, BUTTON_INTERFACE_MAKE3, 64, 98, 1.0f, 1.0f, 1);

	InsertData(BUTTON_OBJ_STR1, BUTTON_INTERFACE_STR1, 128, 98, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_STR2, BUTTON_INTERFACE_STR2, 128, 98, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_STR3, BUTTON_INTERFACE_STR3, 128, 98, 1.0f, 1.0f, 1);

	InsertData(BUTTON_OBJ_TRADE1, BUTTON_INTERFACE_TRADE1, 98, 192, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_TRADE2, BUTTON_INTERFACE_TRADE2, 98, 192, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_TRADE3, BUTTON_INTERFACE_TRADE3, 98, 192, 1.0f, 1.0f, 1);

	InsertData(BUTTON_OBJ_INFO1, BUTTON_INTERFACE_INFO1, 192, 98, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_INFO2, BUTTON_INTERFACE_INFO2, 192, 98, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_INFO3, BUTTON_INTERFACE_INFO3, 192, 98, 1.0f, 1.0f, 1);

	InsertData(SPR_OBJ_BANK_BTN1, SPR_INTERFACE_BANK_BTN1, 0, 98, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_BANK_BTN2, SPR_INTERFACE_BANK_BTN2, 0, 98, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_BANK_BTN3, SPR_INTERFACE_BANK_BTN3, 0, 98, 1.0f, 1.0f, 1);

	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(3, 99, 60, 113, 4, CHECK_BTN);
	InsertCheckInterface(67, 99, 124, 113, 5, CHECK_BTN);
	InsertCheckInterface(131, 99, 188, 113, 6, CHECK_BTN);
	InsertCheckInterface(195, 99, 252, 113, 7, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CItemShopWnd::Remove()
{	
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CItemShopWnd::SetOrder()
{
}

void CItemShopWnd::SetPosSpr()
{
	// NPC KAI 타입이 7번으로 변경 
	// 2005.01.29 김영대
	if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	|| 
		m_byTypeRt==__NPC_VILL_WEAPON__		||	
		m_byTypeRt==__NPC_VILL_ARMOR__		||
		m_byTypeRt==__NPC_VILL_KAI__)
	{
		SetPosObjX(BUTTON_OBJ_MAKE1, 64);
		SetPosObjY(BUTTON_OBJ_MAKE1, 98);
		SetPosObjX(BUTTON_OBJ_MAKE2, 64);
		SetPosObjY(BUTTON_OBJ_MAKE2, 98);
		SetPosObjX(BUTTON_OBJ_MAKE3, 64);
		SetPosObjY(BUTTON_OBJ_MAKE3, 98);
		SetPosObjX(BUTTON_OBJ_STR1, 128);
		SetPosObjY(BUTTON_OBJ_STR1, 98);
		SetPosObjX(BUTTON_OBJ_STR2, 128);
		SetPosObjY(BUTTON_OBJ_STR2, 98);
		SetPosObjX(BUTTON_OBJ_STR3, 128);
		SetPosObjY(BUTTON_OBJ_STR3, 98);
		SetPosObjX(BUTTON_OBJ_TRADE1, 0);
		SetPosObjY(BUTTON_OBJ_TRADE1, 98);
		SetPosObjX(BUTTON_OBJ_TRADE2, 0);
		SetPosObjY(BUTTON_OBJ_TRADE2, 98);
		SetPosObjX(BUTTON_OBJ_TRADE3, 0);
		SetPosObjY(BUTTON_OBJ_TRADE3, 98);
		SetPosObjX(BUTTON_OBJ_INFO1, 192);
		SetPosObjY(BUTTON_OBJ_INFO1, 98);		
		SetPosObjX(BUTTON_OBJ_INFO2, 192);
		SetPosObjY(BUTTON_OBJ_INFO2, 98);		
		SetPosObjX(BUTTON_OBJ_INFO3, 192);
		SetRender(BUTTON_OBJ_INFO3, 98);
	}
	else if(m_byTypeRt==__NPC_VILL_BANK__)
	{	
		SetPosObjX(BUTTON_OBJ_INFO1, 64);
		SetPosObjY(BUTTON_OBJ_INFO1, 98);		
		SetPosObjX(BUTTON_OBJ_INFO2, 64);
		SetPosObjY(BUTTON_OBJ_INFO2, 98);		
		SetPosObjX(BUTTON_OBJ_INFO3, 64);
		SetPosObjY(BUTTON_OBJ_INFO3, 98);
		SetPosObjX(SPR_OBJ_BANK_BTN1, 0);
		SetPosObjY(SPR_OBJ_BANK_BTN1, 98);		
		SetPosObjX(SPR_OBJ_BANK_BTN2, 0);
		SetPosObjY(SPR_OBJ_BANK_BTN2, 98);
		SetPosObjX(SPR_OBJ_BANK_BTN3, 0);
		SetPosObjY(SPR_OBJ_BANK_BTN3, 98);
	}
}

void CItemShopWnd::RenderText()
{			
	CMiniMapWnd*		pMiniMap		= CMiniMapWnd::GetInstance();
	DUNGEON_DATA_EX*	pDungeonInfo	= g_pDungeonTable->GetDungeonInfo(pMiniMap->m_wMapId);		

	if(!IsEmptyString(pDungeonInfo->m_szDungeonName))
	{
		// 던젼 이름 //		

#if IS_JAPAN_LOCALIZING()
		if(lstrlen(pDungeonInfo->m_szSchoolName))
		{
			RenderFont(pDungeonInfo->m_szSchoolName, m_fPosX+25, m_fPosX+100, m_fPosZ+5, m_fPosZ+17, GetStartOrder()+1);			
		}
		else
		{
			RenderFont(pDungeonInfo->m_szDungeonName, m_fPosX+25, m_fPosX+100, m_fPosZ+5, m_fPosZ+17, GetStartOrder()+1);			
		}
#else
		RenderFont(pDungeonInfo->m_szDungeonName, m_fPosX+25, m_fPosX+100, m_fPosZ+5, m_fPosZ+17, GetStartOrder()+1);

#endif
	}
	
	for(int i = 0; i < 5; i++)
	{
		if(m_sDgChat[i].nSize>0)
		{
			if(!IsEmptyString(m_sDgChat[i].szChat))
				RenderFont(m_sDgChat[i].szChat, m_fPosX+20, m_fPosX+246, m_fPosZ+42+i*15, m_fPosZ+54+i*15, GetStartOrder()+1);
		}
	}
}

void CItemShopWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive==FALSE)
	{
		m_bSoundChk	= FALSE;
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{		
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		if(m_byTypeRt==__NPC_VILL_BANK__)
		{							
			SetRender(SPR_OBJ_BANK_BTN1, TRUE);
			SetRender(BUTTON_OBJ_INFO1, TRUE);
		}
		else if(m_byTypeRt==__NPC_VILL_SUPPLIES__	||
				m_byTypeRt==__NPC_VILL_WEAPON__		|| 
				m_byTypeRt==__NPC_VILL_ARMOR__		||
				m_byTypeRt==__NPC_VILL_KAI__)								
		{
			SetRender(BUTTON_OBJ_MAKE1, TRUE);
			SetRender(BUTTON_OBJ_STR1, TRUE);
			SetRender(BUTTON_OBJ_TRADE1, TRUE);
			SetRender(BUTTON_OBJ_INFO1, TRUE);
		}						
		SetRender(SPR_OBJ_ITEMSHOPWND, TRUE);
		SetRender(SPR_OBJ_ITEMSHOPWND_CLOSE1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CItemShopWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	InitValue(nRt);

	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);

	switch(nRt)
	{	
	case 1:
		{						
			if(eMouseCheck==MOUSE_OVER)
			{
				for(int i = 0; i < 15; i++)
					SetRender(m_dwId[i], FALSE);

				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);
				}				
				else if(m_byTypeRt==__NPC_VILL_BANK__)
				{
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(SPR_OBJ_BANK_BTN1, TRUE);
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 2: // 닫기버튼 
		{						
			if(eMouseCheck==MOUSE_UP)
				SetActive(FALSE);
			else if(eMouseCheck==MOUSE_DOWN)
			{
				for(int i = 0; i < 15; i++)
					SetRender(m_dwId[i], FALSE);

				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{								
					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_TRADE2, TRUE);									
				}
				else if(m_byTypeRt==__NPC_VILL_BANK__)
				{				
					SetRender(SPR_OBJ_BANK_BTN1, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);															
				}
				SetRender(SPR_OBJ_ITEMSHOPWND_CLOSE1, FALSE);
				SetRender(SPR_OBJ_ITEMSHOPWND_CLOSE2, TRUE);				
			}			
			SetMouseCheck(nRt, eMouseCheck);						
		}
		break;
	case 3: // 창이동 
		{						
			if(eMouseCheck==MOUSE_UP)
			{
				for(int i = 0; i < 15; i++)
					SetRender(m_dwId[i], FALSE);

				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{								
					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_TRADE2, TRUE);									
				}
				else if(m_byTypeRt==__NPC_VILL_BANK__)
				{				
					SetRender(SPR_OBJ_BANK_BTN1, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);															
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4: // 제조버튼 
		{		
			BYTE byChk = 0;

			if(m_byTypeRt==__NPC_VILL_SIN__ || m_byTypeRt==__NPC_VILL_SU__)
			{
				if(eMouseCheck==MOUSE_UP || eMouseCheck==MOUSE_OVER)
				{
					byChk = __MOUSE_POINTER_BUTTON__;
				}
				else if(eMouseCheck==MOUSE_DOWN)
				{
					byChk = __MOUSE_POINTER_BUTTONCLICK__;
				}
			}

			if(eMouseCheck==MOUSE_UP)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					CItemMakingWnd* pItemMakingWnd			= CItemMakingWnd::GetInstance();
					CItemMakingHelpWnd* pItemMakingHelpWnd	= CItemMakingHelpWnd::GetInstance(); 
					CInterface*		pInterface				= CInterface::GetInstance();

					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);
					
					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_TRADE2, TRUE);

					if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
						m_byTypeRt==__NPC_VILL_WEAPON__		||
						m_byTypeRt==__NPC_VILL_ARMOR__		||
						m_byTypeRt==__NPC_VILL_KAI__)
					{
						CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

						if(pItemTradeShopWnd->GetActive())
						{
							//"교환중에는 다른 행동을 하실 수 없습니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE1006].szMessage, 0xFFFFFF00);							
						}
						else
						{
							CItemWnd* pItemWnd = CItemWnd::GetInstance();						
							pItemWnd->SetActive(TRUE);
							pItemMakingWnd->SetActive(TRUE);
							pItemMakingHelpWnd->SetActive(TRUE);

							// Order 재정의 //
							pInterface->SetWnd(ITEM_WND);
							pInterface->SetWnd(ITEMMAKING_WND);
							pInterface->SetWnd(ITEMMAKINGHELP_WND);
													
							SetActive(FALSE);
						}
					}					
				}
				else if(m_byTypeRt==__NPC_VILL_BANK__)
				{
					CItemTradeShopWnd*	pItemTradeShopWnd	= CItemTradeShopWnd::GetInstance();
					CInterface*			pInterface			= CInterface::GetInstance();						

					if(pItemTradeShopWnd->GetActive())
					{
						//"교환중에는 다른 행동을 하실 수 없습니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE1006].szMessage, 0xFFFFFF00);							
					}
					else
					{
						// 은행 프로세스 //
						for(int i = 0; i < 15; i++)
							SetRender(m_dwId[i], FALSE);

						SetRender(SPR_OBJ_BANK_BTN2, TRUE);
						SetRender(BUTTON_OBJ_INFO1, TRUE);
						
						CBankWnd* pBankWnd = CBankWnd::GetInstance();									
						
						// Order 재정의 //
						CItemWnd* pItemWnd = CItemWnd::GetInstance();									
						pItemWnd->SetActive(!pBankWnd->GetActive());
						pInterface->SetWnd(ITEM_WND);
						// Order 재정의 //
						pInterface->SetWnd(BANK_WND);
						pBankWnd->SetActive(!pBankWnd->GetActive());				
					}					

					SetActive(FALSE);					
				}

				m_bActiveChk = TRUE;
			}			
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);
					
					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_TRADE3, TRUE);
				}
				else if(m_byTypeRt==__NPC_VILL_BANK__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(SPR_OBJ_BANK_BTN3, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);				
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);
					
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);			
					SetRender(BUTTON_OBJ_TRADE2, TRUE);							
				}
				else if(m_byTypeRt==__NPC_VILL_BANK__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(SPR_OBJ_BANK_BTN2, TRUE);
					SetRender(BUTTON_OBJ_INFO1, TRUE);
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}		
		break;		
	case 5:	// 거래버튼
		{
			BYTE byChk = 0;

			if(m_byTypeRt==__NPC_VILL_SIN__ || m_byTypeRt==__NPC_VILL_SU__)
			{
				if(eMouseCheck==MOUSE_UP || eMouseCheck==MOUSE_OVER)
					byChk = __MOUSE_POINTER_BUTTON__;
				else if(eMouseCheck==MOUSE_DOWN)
					byChk = __MOUSE_POINTER_BUTTONCLICK__;
			}

			if(eMouseCheck==MOUSE_UP)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					CStoreWnd*		pStoreWnd		= CStoreWnd::GetInstance();
					CInterface*		pInterface		= CInterface::GetInstance();

					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_MAKE2, TRUE);

					if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
						m_byTypeRt==__NPC_VILL_WEAPON__		||
						m_byTypeRt==__NPC_VILL_ARMOR__		||
						m_byTypeRt==__NPC_VILL_KAI__)
					{
						CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

						if(pItemTradeShopWnd->GetActive())
						{
							//"교환중에는 다른 행동을 하실 수 없습니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE1006].szMessage, 0xFFFFFF00);							
						}
						else
						{
							if(pStoreWnd->GetActive()==FALSE)
							{
								// NPC KAI에게 무기를 상점을 물린다.
								// 2005.01.29 김영대
								switch(m_byTypeRt)
								{
									case __NPC_VILL_SUPPLIES__ :
										pStoreWnd->OpenStoreItem(STORE_TYPE_SUSPZO);
										break;
									case __NPC_VILL_WEAPON__ :
									case __NPC_VILL_KAI__ :
										pStoreWnd->OpenStoreItem(STORE_TYPE_WEAPON);
										break;
									case __NPC_VILL_ARMOR__ :
										pStoreWnd->OpenStoreItem(STORE_TYPE_ARMOR);
										break;
								}

							}

							CItemWnd* pItemWnd = CItemWnd::GetInstance();		
						
							pItemWnd->SetActive(!pStoreWnd->GetActive());
							pStoreWnd->SetActive(!pStoreWnd->GetActive());											

							// Order 재정의 //
							pInterface->SetWnd(STORE_WND);
							pInterface->SetWnd(ITEM_WND);
						}
						
						SetActive(FALSE);						
					}
					else if(m_byTypeRt==__NPC_VILL_BANK__)
					{
						for(int i = 0; i < 15; i++)
							SetRender(m_dwId[i], FALSE);

						SetRender(BUTTON_OBJ_INFO2, TRUE);
						SetRender(SPR_OBJ_BANK_BTN1, TRUE);

						// 프로세스 //
						CNpcDungeonInfoWnd*	pNpcDungeonInfoWnd	= CNpcDungeonInfoWnd::GetInstance();				
						CInterface*			pInterface			= CInterface::GetInstance();
						
						pNpcDungeonInfoWnd->SetActive(!pNpcDungeonInfoWnd->GetActive());
						pNpcDungeonInfoWnd->m_bType = m_byTypeRt;						

						if(pNpcDungeonInfoWnd->GetActive()!=FALSE)
						{
							pInterface->SetWnd(NPCDUNGEON_INFO_WND);				
						}
					}
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);
								
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_MAKE3, TRUE);
				}
				else if(m_byTypeRt==__NPC_VILL_BANK__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(BUTTON_OBJ_INFO3, TRUE);
					SetRender(SPR_OBJ_BANK_BTN1, TRUE);
				}				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);
					
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_MAKE2, TRUE);
				}
				else if(m_byTypeRt==__NPC_VILL_BANK__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(BUTTON_OBJ_INFO2, TRUE);
					SetRender(SPR_OBJ_BANK_BTN1, TRUE);
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}				
		break;
	case 6:	// 강화버튼 
		{
			BYTE byChk = 0;

			if(m_byTypeRt==__NPC_VILL_SIN__ || m_byTypeRt==__NPC_VILL_SU__)
			{
				if(eMouseCheck==MOUSE_UP || eMouseCheck==MOUSE_OVER)
					byChk = __MOUSE_POINTER_BUTTON__;
				else if(eMouseCheck==MOUSE_DOWN)
					byChk = __MOUSE_POINTER_BUTTONCLICK__;
			}

			if(eMouseCheck==MOUSE_UP)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_STR2, TRUE);

					if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
						m_byTypeRt==__NPC_VILL_WEAPON__		||
						m_byTypeRt==__NPC_VILL_ARMOR__		||
						m_byTypeRt==__NPC_VILL_KAI__)
					{
						CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

						if(pItemTradeShopWnd->GetActive())
						{
							//"교환중에는 다른 행동을 하실 수 없습니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE1006].szMessage, 0xFFFFFF00);							
						}
						else
						{
							CItemWnd*			pItemWnd			= CItemWnd::GetInstance();
							CItemUpgradeWnd*	pItemUpgradeWnd		= CItemUpgradeWnd::GetInstance();
							CItemUpgradeHelpWnd*pItemUpgradeHelpWnd = CItemUpgradeHelpWnd::GetInstance(); 
							
							if(pItemWnd->GetActive()==FALSE)
								pItemWnd->SetActive();								
							
							pItemUpgradeWnd->SetActive(!pItemUpgradeWnd->GetActive());
							pItemUpgradeHelpWnd->SetActive(!pItemUpgradeHelpWnd->GetActive());

							CInterface::GetInstance()->SetWnd(ITEMUPGRADE_HELP_WND);
							CInterface::GetInstance()->SetWnd(ITEMUPGRADE_WND);
							CInterface::GetInstance()->SetWnd(ITEM_WND);
							
							SetActive(FALSE);
						}
					}					
				}
				m_bActiveChk = TRUE;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);
					
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_MAKE1, TRUE);			
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_STR3, TRUE);
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{			
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);
					
					SetRender(BUTTON_OBJ_INFO1, TRUE);
					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_STR2, TRUE);
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}	
		break;
	case 7:	// 정보버튼 
		{
			BYTE byChk = 0;

			if(m_byTypeRt==__NPC_VILL_SIN__ || m_byTypeRt==__NPC_VILL_SU__)
			{
				if(eMouseCheck==MOUSE_UP || eMouseCheck==MOUSE_OVER)
					byChk = __MOUSE_POINTER_BUTTON__;
				else if(eMouseCheck==MOUSE_DOWN)
					byChk = __MOUSE_POINTER_BUTTONCLICK__;
			}

			if(eMouseCheck==MOUSE_UP)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{	
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_INFO2, TRUE);

					// 여기는 창을 출력 //			
					CNpcDungeonInfoWnd*	pNpcDungeonInfoWnd	= CNpcDungeonInfoWnd::GetInstance();				
					CInterface*			pInterface		= CInterface::GetInstance();					
					pNpcDungeonInfoWnd->SetActive(!pNpcDungeonInfoWnd->GetActive());
					pNpcDungeonInfoWnd->m_bType = m_byTypeRt;
					pInterface->SetWnd(NPCDUNGEON_INFO_WND);				
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_INFO3, TRUE);
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(	m_byTypeRt==__NPC_VILL_SUPPLIES__	||
					m_byTypeRt==__NPC_VILL_WEAPON__		||
					m_byTypeRt==__NPC_VILL_ARMOR__		||
					m_byTypeRt==__NPC_VILL_KAI__)
				{
					for(int i = 0; i < 15; i++)
						SetRender(m_dwId[i], FALSE);

					SetRender(BUTTON_OBJ_MAKE1, TRUE);
					SetRender(BUTTON_OBJ_TRADE1, TRUE);
					SetRender(BUTTON_OBJ_STR1, TRUE);
					SetRender(BUTTON_OBJ_INFO2, TRUE);
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;	
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_ITEMSHOPWND_CLOSE1, TRUE);
		SetRender(SPR_OBJ_ITEMSHOPWND_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();	
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();
	
	return 0;
}
//======================================================//
// End.													//
//======================================================//
