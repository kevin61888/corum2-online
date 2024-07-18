// CItemUpgradeWnd.cpp: implementation of the CItemUpgradeWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "ItemUpgrade.h"
#include "ItemUpgradeSubWnd.h"
#include "ItemUpgradeHelpWnd.h"
#include "GameControl.h"
#include "User.h"
#include "QuantityWnd.h"
#include "dungeon_data_ex.h"
#include "hashtable.h"
#include "../CommonServer/CommonHeader.h"
#include "Chat.h"
#include "UserInterface.h"
#include "Message.h"
#include "DungeonTable.h"
#include "Interface.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "ItemNative.h"


CItemUpgradeWnd* CItemUpgradeWnd::c_pThis = NULL;


CItemUpgradeWnd::CItemUpgradeWnd()
{		
	memset(m_pUpgradeItem, 0, sizeof(m_pUpgradeItem));
	m_bWaitingUpgrade = 0;		
}


CItemUpgradeWnd::~CItemUpgradeWnd()
{
	
}


BOOL CItemUpgradeWnd::Init()
{	
	memset(m_pUpgradeItem, 0, sizeof(m_pUpgradeItem));
	m_bWaitingUpgrade = 0;

	// "공격력 업그레이드 성공입니다..."
	__lstrcpyn(m_szUpgradeDesc[ITEM_UPGRADE_SUCCESS_TYPE_AD], g_Message[ETC_MESSAGE693].szMessage, TEXT_RENDER_BUFFER);
	
	// "방어력 업그레이드 성공입니다..."
	__lstrcpyn(m_szUpgradeDesc[ITEM_UPGRADE_SUCCESS_TYPE_DP], g_Message[ETC_MESSAGE212].szMessage, TEXT_RENDER_BUFFER);
	
	// "옵션 업그레이드 성공입니다..."
	__lstrcpyn(m_szUpgradeDesc[ITEM_UPGRADE_SUCCESS_TYPE_OP], g_Message[ETC_MESSAGE694].szMessage, TEXT_RENDER_BUFFER);
	
	// "슬롯이 증가되었습니다..."
	__lstrcpyn(m_szUpgradeDesc[ITEM_UPGRADE_SUCCESS_TYPE_SL], g_Message[ETC_MESSAGE213].szMessage, TEXT_RENDER_BUFFER);

	// "공격력, 옵션 업그레이드 성공입니다..."
	__lstrcpyn(m_szUpgradeDesc[ITEM_UPGRADE_SUCCESS_TYPE_AD+ITEM_UPGRADE_SUCCESS_TYPE_OP], g_Message[ETC_MESSAGE695].szMessage, TEXT_RENDER_BUFFER);

	// "방어력, 옵션 업그레이드 성공입니다..."
	__lstrcpyn(m_szUpgradeDesc[ITEM_UPGRADE_SUCCESS_TYPE_DP+ITEM_UPGRADE_SUCCESS_TYPE_OP], g_Message[ETC_MESSAGE214].szMessage, TEXT_RENDER_BUFFER);

	// "슬롯 증가, 옵션업그레이드 성공입니다..."
	__lstrcpyn(m_szUpgradeDesc[ITEM_UPGRADE_SUCCESS_TYPE_SL+ITEM_UPGRADE_SUCCESS_TYPE_OP], g_Message[ETC_MESSAGE215].szMessage, TEXT_RENDER_BUFFER);
	
	/*
	InsertData(SPR_OBJ_ITEMUPGRADEWND, SPR_INTERFACE_ITEMUPGRADEWND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_ITEMUPGRADE_HELPWND, SPR_INTERFACE_ITEMUPGRADE_HELPWND, 0, 128, 1.0f, 1.0f, 0);
	InsertData(BUTTON_OBJ_ITEMUPGRADE_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMUPGRADE_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	
	InsertData(BUTTON_OBJ_ITEMUPGRADE1, BUTTON_INTERFACE_ITEMUPGRADE1, 189, 110, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMUPGRADE2, BUTTON_INTERFACE_ITEMUPGRADE2, 189, 110, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMUPGRADE3, BUTTON_INTERFACE_ITEMUPGRADE3, 189, 110, 1.0f, 1.0f, 1);			
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(12, 33, 44, 97, 4, CHECK_ITEM);
	InsertCheckInterface(142, 33, 253, 97, 5, CHECK_ITEM);
	InsertCheckInterface(189, 110, 253, 126, 6, CHECK_BTN);		*/
	
	m_bInit	= TRUE;

	return TRUE;
}

void CItemUpgradeWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}


void CItemUpgradeWnd::SetOrder()
{

}


void CItemUpgradeWnd::RenderText()
{	
	char szInfo[0xff] = {0,};

	if(m_bWaitingUpgrade == ITEM_UPGRADE_FAIL)
	{
		lstrcpy(szInfo, g_Message[ETC_MESSAGE587].szMessage); // "아이템 업그레이드 실패입니다..."
		RenderFont(szInfo, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);
	}
	else if (m_bWaitingUpgrade == ITEM_UPGRADE_FAIL_INSURANCE)
	{
		lstrcpy(szInfo, g_Message[ETC_MESSAGE1172].szMessage); // "아이템 업그레이드 실패하여"
		RenderFont(szInfo, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);

		lstrcpy(szInfo, g_Message[ETC_MESSAGE1173].szMessage); // "보험처리 되었습니다."
		RenderFont(szInfo, m_fPosX+10, m_fPosX+250, m_fPosZ+156, m_fPosZ+168, GetStartOrder()+2);
	}
	else if (m_bWaitingUpgrade == 0)
	{
		lstrcpy(szInfo, g_Message[ETC_MESSAGE583].szMessage); // "무기와 재료( 조디악(백양, 처녀),"
		RenderFont(szInfo, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);

		lstrcpy(szInfo, g_Message[ETC_MESSAGE584].szMessage); // "엘릭서, 쥬얼 )를 올려놓으세요..."
		RenderFont(szInfo, m_fPosX+10, m_fPosX+250, m_fPosZ+156, m_fPosZ+168, GetStartOrder()+2);
	}
	else if (m_bWaitingUpgrade == ITEM_UPGRADE_WAITING)
	{
		lstrcpy(szInfo, g_Message[ETC_MESSAGE585].szMessage); // "아이템 업그레이드 중입니다..."
		RenderFont(szInfo, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);	
	}
	else
	{
		lstrcpy(szInfo, m_szUpgradeDesc[m_bWaitingUpgrade]); 
		RenderFont(szInfo, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);
	}

	RenderItem();
	ItemInfoRender();
}


void CItemUpgradeWnd::RenderItem()
{
	int nOrder = GetStartOrder();
		
	VECTOR2	vPos = {0.f,0.f};
	
	if(g_ItemMoveManager.GetNewItemMoveMode())
	{
		if(g_pMainPlayer->m_pUpgradeItem[0].GetID()!=0)
		{
			if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_UPGRADE &&
				0 == g_ItemMoveManager.GetNativeSrcIndex())
			{
				// ???
			}
			else
			{
				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pUpgradeItem[0].GetID());

				if(lpItemResourceEx)
				{
					vPos.x	= (float)m_fPosX+12;
					vPos.y	= (float)m_fPosZ+33;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}
			}			
		}

		for(int i = 0; i < 3; i++)
		{
			if(g_pMainPlayer->m_pUpgradeItem[i+1].GetID()!=0)
			{
				if(	g_ItemMoveManager.GetNativeSrc() == ITEM_NATIVE_UPGRADE &&
					(i+1) == g_ItemMoveManager.GetNativeSrcIndex() )
				{
					continue;
				}

				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
					g_pItemResourceHash->GetData(g_pMainPlayer->m_pUpgradeItem[i+1].GetID());

				if(lpItemResourceEx)
				{
					vPos.x	= (float)m_fPosX+142+i*37;
					vPos.y	= (float)m_fPosZ+33;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}
			}
		}
	}
	else
	{
		if(m_pUpgradeItem[0].GetID()!=0)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = g_pItemResourceHash->GetData(m_pUpgradeItem[0].GetID());
			if(lpItemResourceEx)
			{
				vPos.x	= (float)m_fPosX+12;
				vPos.y	= (float)m_fPosZ+33;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}
		}

		for(int i = 0; i < 3; i++)
		{
			if(m_pUpgradeItem[i+1].GetID()!=0)
			{
				LP_ITEM_RESOURCE_EX lpItemResourceEx = g_pItemResourceHash->GetData(m_pUpgradeItem[i+1].GetID());
				if(lpItemResourceEx)
				{
					vPos.x	= (float)m_fPosX+142+i*37;
					vPos.y	= (float)m_fPosZ+33;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}
			}
		}
	}	
}


void CItemUpgradeWnd::ItemInfoRender()
{			
	if(g_Mouse.bLDown==TRUE)
		return;

	if(g_ItemMoveManager.GetNewItemMoveMode())
	{	
		if(	g_Mouse.MousePos.x>m_fPosX+12 && g_Mouse.MousePos.x<m_fPosX+34 &&
			g_Mouse.MousePos.y>m_fPosZ+33 && g_Mouse.MousePos.y<m_fPosZ+97)
		{					
			if(CUserInterface::GetInstance()->InterfaceCollision(ITEMUPGRADE_WND, (int)m_fPosX+12, (int)m_fPosX+34, (int)m_fPosZ+33, (int)m_fPosZ+97)==FALSE)
				return;
			if(g_pMainPlayer->m_pUpgradeItem[0].m_wItemID==0)
				return;
			
			CInterface::GetInstance()->ItemInfoRender(&g_pMainPlayer->m_pUpgradeItem[0], FALSE);
		}
		else if(g_Mouse.MousePos.x>m_fPosX+142 && g_Mouse.MousePos.x<m_fPosX+248 && g_Mouse.MousePos.y>m_fPosZ+33 && g_Mouse.MousePos.y<m_fPosZ+97)
		{
			int nIndex	= (g_Mouse.MousePos.x-(int)m_fPosX-142)/37;		

			if(CUserInterface::GetInstance()->InterfaceCollision(ITEMUPGRADE_WND, (int)m_fPosX+142+nIndex*37, (int)m_fPosX+174+nIndex*37, (int)m_fPosZ+33, (int)m_fPosZ+97)==FALSE)
				return;
			if(g_pMainPlayer->m_pUpgradeItem[nIndex+1].m_wItemID==0)
				return;
			
			CInterface::GetInstance()->ItemInfoRender(&g_pMainPlayer->m_pUpgradeItem[nIndex+1], FALSE);		
		}
	}
	else
	{
		if(	g_Mouse.MousePos.x>m_fPosX+12 && g_Mouse.MousePos.x<m_fPosX+34 &&
			g_Mouse.MousePos.y>m_fPosZ+33 && g_Mouse.MousePos.y<m_fPosZ+97)
		{					
			if(CUserInterface::GetInstance()->InterfaceCollision(ITEMUPGRADE_WND, (int)m_fPosX+12, (int)m_fPosX+34, (int)m_fPosZ+33, (int)m_fPosZ+97)==FALSE)
				return;
			if(m_pUpgradeItem[0].m_wItemID==0)
				return;
			
			CInterface::GetInstance()->ItemInfoRender(&m_pUpgradeItem[0], FALSE);
		}
		else if(g_Mouse.MousePos.x>m_fPosX+142 && g_Mouse.MousePos.x<m_fPosX+248 && g_Mouse.MousePos.y>m_fPosZ+33 && g_Mouse.MousePos.y<m_fPosZ+97)
		{
			int nIndex	= (g_Mouse.MousePos.x-(int)m_fPosX-142)/37;		

			if(CUserInterface::GetInstance()->InterfaceCollision(ITEMUPGRADE_WND, (int)m_fPosX+142+nIndex*37, (int)m_fPosX+174+nIndex*37, (int)m_fPosZ+33, (int)m_fPosZ+97)==FALSE)
				return;
			if(m_pUpgradeItem[nIndex+1].m_wItemID==0)
				return;
			
			CInterface::GetInstance()->ItemInfoRender(&m_pUpgradeItem[nIndex+1], FALSE);		
		}
	}
}


void CItemUpgradeWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
		
	if(bActive==FALSE)
	{
		ShowSpriteAll();
		
		for(int i = 0; i < 4; i++)
		{
			if(m_pUpgradeItem[i].GetID()!=0)
			{				
				CTDS_UPGRADE_FAIL pPacket;
				pPacket.dwUserIndex = g_pMainPlayer->m_dwUserIndex;
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);				
				break;
			}
		}
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
		
		CItemUpgradeSubWnd*		pItemUpgradeSubWnd	= CItemUpgradeSubWnd::GetInstance();
		CItemUpgradeHelpWnd*	pItemUpgradeHelpWnd = CItemUpgradeHelpWnd::GetInstance(); 

		if(pItemUpgradeSubWnd && pItemUpgradeHelpWnd)
		{
			pItemUpgradeSubWnd->SetActive(FALSE);
			pItemUpgradeHelpWnd->SetActive(FALSE);
		}		
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_ITEMUPGRADEWND, bActive);
		SetRender(SPR_OBJ_ITEMUPGRADE_HELPWND, bActive);
		SetRender(BUTTON_OBJ_ITEMUPGRADE_CLOSE1, bActive);
		SetRender(BUTTON_OBJ_ITEMUPGRADE1, bActive);

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}


int CItemUpgradeWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	// Waiting..
	if (m_bWaitingUpgrade==ITEM_UPGRADE_WAITING)
		return 0;
	
	if(m_bBtn && nRt != 6)
	{
		SetRender(BUTTON_OBJ_ITEMUPGRADE1, TRUE);
		SetRender(BUTTON_OBJ_ITEMUPGRADE2, FALSE);
		SetRender(BUTTON_OBJ_ITEMUPGRADE3, FALSE);
		m_bBtn = FALSE;
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
				SetRender(BUTTON_OBJ_ITEMUPGRADE_CLOSE1, FALSE);
				SetRender(BUTTON_OBJ_ITEMUPGRADE_CLOSE2, TRUE);
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
	case 4: // 라지 아이템
		{
			BYTE byChk = 0;
			
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_pMainPlayer->m_pUpgradeItem[0].GetID() != 0)
					{
						WORD wID = g_pMainPlayer->m_pUpgradeItem[0].GetID();
						g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_UPGRADE,0,wID,0,0);
						byChk = __MOUSE_POINTER_ITEM__;
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;
					}

					m_bWaitingUpgrade = 0;
				}
				else
				{
					if(m_pUpgradeItem[0].GetID()!=0)
					{
						if(g_pMainPlayer->m_MouseItem.GetID()==0)
						{
							// 아이템 업그레이드창에서 마우스로 //
							CTDS_ITEM_PICKUP ItemPickup;	
							ItemPickup.bSectionNum	= 1;
							ItemPickup.i64ItemID	= 0;
							SetItemPacket(&ItemPickup, 66, 0, 0, 0, 0);
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						}
						byChk = __MOUSE_POINTER_ITEM__;
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;
					}

					m_bWaitingUpgrade = 0;
				}				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_pMainPlayer->m_pUpgradeItem[0].GetID()!=0)
					{
						byChk = __MOUSE_POINTER_ITEM__;					
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;					
					}
				}
				else
				{
					if(m_pUpgradeItem[0].GetID()!=0)
					{
						byChk = __MOUSE_POINTER_ITEM__;					
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;					
					}
				}				
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;		
	case 5:
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{
				BYTE byZipCode = BYTE((g_Mouse.MousePos.x - (m_fPosX+142))/37);

				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_pMainPlayer->m_pUpgradeItem[byZipCode+1].GetID() != 0)
					{
						WORD wID = g_pMainPlayer->m_pUpgradeItem[byZipCode+1].GetID();
						g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_UPGRADE,byZipCode+1,wID,0,0);
						byChk = __MOUSE_POINTER_ITEM__;					
					}
					else
					{	
						byChk = __MOUSE_POINTER_DEFAULT__;
					}

					m_bWaitingUpgrade = 0;
				}
				else
				{
					if(m_pUpgradeItem[byZipCode+1].GetID()!=0)
					{
						byChk = __MOUSE_POINTER_ITEM__;					

						if(g_pMainPlayer->m_MouseItem.GetID()==0)
						{
							// 아이템 업그레이드창에서 마우스로 //
							CTDS_ITEM_PICKUP ItemPickup;	
							ItemPickup.bSectionNum	= 1;
							ItemPickup.i64ItemID	= 0;
							SetItemPacket(&ItemPickup, 68, byZipCode+1, 0, 0, 0);
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						}					
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;					
					}

					m_bWaitingUpgrade = 0;
				}				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				BYTE byZipCode = BYTE((g_Mouse.MousePos.x - (m_fPosX+142))/37);

				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_pMainPlayer->m_pUpgradeItem[byZipCode+1].GetID()!=0)
					{
						byChk = __MOUSE_POINTER_ITEM__;		
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;					
					}					
				}
				else
				{
					if(m_pUpgradeItem[byZipCode+1].GetID()!=0)
					{
						byChk = __MOUSE_POINTER_ITEM__;		
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;					
					}
				}				
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;	
	case 6:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				SetRender(BUTTON_OBJ_ITEMUPGRADE1, TRUE);
				SetRender(BUTTON_OBJ_ITEMUPGRADE2, FALSE);
				SetRender(BUTTON_OBJ_ITEMUPGRADE3, FALSE);

				// 아이템을 알맞게 넣었는가?
				BYTE bCheck[4] = {0, };	
				
				for(int i = 0; i < 4; ++i)
				{
					if(!m_pUpgradeItem[i].GetID())
					{
						continue;
					}
					if( m_pUpgradeItem[i].GetKind() == ITEM_KIND_WEAPON ||
						m_pUpgradeItem[i].GetKind() == ITEM_KIND_AROMR)
					{
						if(m_pUpgradeItem[i].m_bSlot.uMaxSlot > 0)
						{
							bCheck[0]++;
						}
						else
						{
							char szInfo[0xff] = {0,};
							wsprintf(szInfo,g_Message[ETC_MESSAGE1002].szMessage);	
							DisplayMessageAdd(szInfo, 0xffffff00);
						}
					}
					else if(m_pUpgradeItem[i].GetKind() == ITEM_KIND_UPGRADE)
					{
						bCheck[1]++;
					}
					else if (m_pUpgradeItem[i].GetKind() == ITEM_KIND_LIQUID)
					{
						bCheck[2]++;
					}
					else if (m_pUpgradeItem[i].GetKind() == ITEM_KIND_EDITION)
					{
						bCheck[3]++;
					}
				}

				if(bCheck[0] && bCheck[1] && bCheck[2])
				{
					// 1.(무기류와방어구)
					// 2.강화서 
					// 3.리퀴드
					// 이 세가지는 꼭 있어야 돼!!!
					m_bWaitingUpgrade = ITEM_UPGRADE_WAITING;
					CItemUpgradeSubWnd* pItemUpgradeSubWnd = CItemUpgradeSubWnd::GetInstance();
					
					if(pItemUpgradeSubWnd)
					{
						pItemUpgradeSubWnd->m_byItemUpgradeProcess = 0;
						pItemUpgradeSubWnd->SetActive(TRUE);
					}					
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(BUTTON_OBJ_ITEMUPGRADE1, FALSE);
				SetRender(BUTTON_OBJ_ITEMUPGRADE2, TRUE);
				SetRender(BUTTON_OBJ_ITEMUPGRADE3, FALSE);
				m_bBtn = TRUE;
			}
			else if(eMouseCheck == MOUSE_DOWN)
			{
				SetRender(BUTTON_OBJ_ITEMUPGRADE1, FALSE);
				SetRender(BUTTON_OBJ_ITEMUPGRADE2, FALSE);
				SetRender(BUTTON_OBJ_ITEMUPGRADE3, TRUE);
				m_bBtn = TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(BUTTON_OBJ_ITEMUPGRADE_CLOSE1, TRUE);
		SetRender(BUTTON_OBJ_ITEMUPGRADE_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}


void CItemUpgradeWnd::SetButtonStatus(DWORD dwSprID, DWORD dwStatus)
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


BOOL CItemUpgradeWnd::IsAbleUpgradeAriesZodiac(CItem* pItem)
{
	return	pItem->GetID() == 8000 ||
			pItem->GetID() == 8201 ||
			pItem->GetID() == 8202 ||
			pItem->GetID() == 8203 ||
			pItem->GetID() == 8204 ||
			pItem->GetID() == 8205;
}


void CItemUpgradeWnd::OnMove()
{

}



