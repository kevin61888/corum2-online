#include "stdafx.h"
#include "ItemMakingWnd.h"
#include "ItemMakingSubWnd.h"
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
#include "ItemMakingHelpWnd.h"


CItemMakingWnd* CItemMakingWnd::c_pThis = NULL;


CItemMakingWnd::CItemMakingWnd()
{

}


CItemMakingWnd::~CItemMakingWnd()
{

}


BOOL CItemMakingWnd::Init()
{
	memset(m_pMakingItem,0,sizeof(m_pMakingItem));
	m_byProcess = ITEM_MAKING_PROCESS_DEFAULT;

/*
	InsertData(SPR_OBJ_ITEMMAKINGWND, SPR_INTERFACE_ITEMUPGRADEWND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_ITEMMAKING_HELPWND, SPR_INTERFACE_ITEMUPGRADE_HELPWND, 0, 128, 1.0f, 1.0f, 0);

	InsertData(BUTTON_OBJ_ITEMMAKING_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMMAKING_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	

	InsertData(BUTTON_OBJ_ITEMMAKING1, BUTTON_INTERFACE_TRADE1, 189, 110, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMMAKING2, BUTTON_INTERFACE_TRADE2, 189, 110, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMMAKING3, BUTTON_INTERFACE_TRADE3, 189, 110, 1.0f, 1.0f, 1);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(12, 33, 44, 97, 4, CHECK_ITEM);
	InsertCheckInterface(142, 33, 253, 97, 5, CHECK_ITEM);
	InsertCheckInterface(189, 110, 253, 126, 6, CHECK_BTN);
*/
	m_bInit	= TRUE;
	
	return TRUE;
}

void CItemMakingWnd::RenderText()
{	
	if(m_byProcess == ITEM_MAKING_PROCESS_DEFAULT)
	{		
		// "제조시킬 아이템을 올려놓으세요."
		RenderFont(g_Message[ETC_MESSAGE1227].szMessage, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);	
	}
	else if(m_byProcess == ITEM_MAKING_PROCESS_RESULT_WAITING)
	{		
		// "제조 하고 있는 중입니다."
		RenderFont(g_Message[ETC_MESSAGE1228].szMessage, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);			
	}
	else if(m_byProcess == ITEM_MAKING_PROCESS_RESULT_SUCCESS)
	{
		// "제조가 성공적으로 이루어 졌습니다."
		RenderFont(g_Message[ETC_MESSAGE1229].szMessage, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);
	}
	else if(m_byProcess == ITEM_MAKING_PROCESS_RESULT_FAIL)
	{
		// "제조를 실패 하였습니다."
		RenderFont(g_Message[ETC_MESSAGE1230].szMessage, m_fPosX+10, m_fPosX+250, m_fPosZ+139, m_fPosZ+151, GetStartOrder()+2);
	}

	RenderItem();
	ItemInfoRender();
}

void CItemMakingWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
		
	if(bActive==FALSE)
	{
		ShowSpriteAll();
		
		for(int i = 0; i < MAX_ITEM_MAKING; i++)
		{
			if(m_pMakingItem[i].GetID() != 0)
			{
				// 하나라도 있으면 제조캔슬 패킷을 보내준다.
				CTDS_MAKING_CANCEL pPacket;
				pPacket.dwUserIndex = g_pMainPlayer->m_dwUserIndex;
				g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);				
				break;
			}
		}
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
		
		CItemMakingSubWnd*	pItemMakingSubWnd	= CItemMakingSubWnd::GetInstance();
		CItemMakingHelpWnd*	pItemMakingHelpWnd	= CItemMakingHelpWnd::GetInstance(); 

		if(pItemMakingSubWnd && pItemMakingHelpWnd)
		{
			pItemMakingSubWnd->SetActive(FALSE);
			pItemMakingHelpWnd->SetActive(FALSE);
		}		
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_ITEMMAKINGWND, bActive);
		SetRender(SPR_OBJ_ITEMMAKING_HELPWND, bActive);
		SetRender(BUTTON_OBJ_ITEMMAKING_CLOSE1, bActive);
		SetRender(BUTTON_OBJ_ITEMMAKING1, bActive);

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}


void CItemMakingWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}


void CItemMakingWnd::SetOrder()
{

}


int CItemMakingWnd::CheckInterface()
{
	m_bMouseIcon	= FALSE;
	int nRt			= GetChk();

	if (m_byProcess == ITEM_MAKING_PROCESS_RESULT_WAITING)
		return FALSE;

	if(m_bBtn && nRt != 6)
	{
		SetRender(BUTTON_OBJ_ITEMMAKING1, TRUE);
		SetRender(BUTTON_OBJ_ITEMMAKING2, FALSE);
		SetRender(BUTTON_OBJ_ITEMMAKING3, FALSE);
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
				SetRender(BUTTON_OBJ_ITEMMAKING_CLOSE1, FALSE);
				SetRender(BUTTON_OBJ_ITEMMAKING_CLOSE2, TRUE);
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
					if(g_pMainPlayer->m_pMakingItem[0].GetID() != 0)
					{
						WORD wID = g_pMainPlayer->m_pMakingItem[0].GetID();
						g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_MAKING,0,wID,0,0);
						byChk = __MOUSE_POINTER_ITEM__;
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;
					}
				}
				else
				{
					if(m_pMakingItem[0].GetID()!=0)
					{
						if(g_pMainPlayer->m_MouseItem.GetID()==0)
						{
							// 아이템 제조창에서 마우스로 //
							CTDS_ITEM_PICKUP ItemPickup;	
							ItemPickup.bSectionNum	= 1;
							ItemPickup.i64ItemID	= 0;
							SetItemPacket(&ItemPickup, 81, 0, 0, 0, 0);
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						}

						byChk = __MOUSE_POINTER_ITEM__;
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;
					}	
				}							
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_pMainPlayer->m_pMakingItem[0].GetID()!=0)
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
					if(m_pMakingItem[0].GetID()!=0)
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
	case 5: // 스몰 아이템 
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_DOWN)
			{
				BYTE byZipCode = BYTE((g_Mouse.MousePos.x - (m_fPosX+142))/37);

				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_pMainPlayer->m_pMakingItem[byZipCode+1].GetID() != 0)
					{
						WORD wID = g_pMainPlayer->m_pMakingItem[byZipCode+1].GetID();
						g_ItemMoveManager.SetLButtonDownItem(ITEM_NATIVE_MAKING,byZipCode+1,wID,0,0);
						byChk = __MOUSE_POINTER_ITEM__;					
					}
					else
					{	
						byChk = __MOUSE_POINTER_DEFAULT__;
					}					
				}
				else
				{
					if(m_pMakingItem[byZipCode+1].GetID()!=0)
					{
						byChk = __MOUSE_POINTER_ITEM__;					

						if(g_pMainPlayer->m_MouseItem.GetID()==0)
						{
							// 아이템 제조창에서 마우스로 //
							CTDS_ITEM_PICKUP ItemPickup;	
							ItemPickup.bSectionNum	= 1;
							ItemPickup.i64ItemID	= 0;
							SetItemPacket(&ItemPickup, 80, byZipCode+1, 0, 0, 0);
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						}					
					}
					else
					{
						byChk = __MOUSE_POINTER_DEFAULT__;					
					}				
				}					
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				BYTE byZipCode = BYTE((g_Mouse.MousePos.x - (m_fPosX+142))/37);

				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					if(g_pMainPlayer->m_pMakingItem[byZipCode+1].GetID()!=0)
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
					if(m_pMakingItem[byZipCode+1].GetID()!=0)
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
	case 6:	// 제조 버튼 
		{
			if(eMouseCheck==MOUSE_UP)
			{
				SetRender(BUTTON_OBJ_ITEMMAKING1, TRUE);	// MOUSE_UP
				SetRender(BUTTON_OBJ_ITEMMAKING2, FALSE);	// MOUSE_OVER
				SetRender(BUTTON_OBJ_ITEMMAKING3, FALSE);	// MOUSE_DOWN

				// 아이템을 알맞게 넣었는가?
				BYTE bLargeCheck	= 0;
				BYTE bCheck[2]		= {0,};
				
				if(m_pMakingItem[0].GetID() == 0)
				{
					bLargeCheck = 1;
				}

				for(int i = 1; i < 4; ++i)
				{
					int nValue = m_pMakingItem[i].GetID() / ITEM_DISTRIBUTE;
					if(nValue>=ITEM_MATERIALST_INDEX && nValue<=ITEM_MATERIALEND_INDEX)
					{
						bCheck[0] += 1;
					}
					else if(m_pMakingItem[i].GetID() == __ITEM_MAKING_INDEX__)
					{
						bCheck[1] += 1;
					}
				}
				
				// 아이템 제조 조건이 모두 만족하면 
				if(bLargeCheck && bCheck[0] == 2 && bCheck[1])
				{
					m_byProcess = ITEM_MAKING_PROCESS_RESULT_WAITING;
					CItemMakingSubWnd* pItemMakingSubWnd = CItemMakingSubWnd::GetInstance();
					
					if(pItemMakingSubWnd)
					{
						pItemMakingSubWnd->m_byItemMakingProcess = 0;
						pItemMakingSubWnd->SetActive(TRUE);
					}						
				}
				else 
				{
					char szInfo[0xff] = {0,};
					wsprintf(szInfo,g_Message[ETC_MESSAGE1212].szMessage);	// "제조 조건이 맞지 않습니다."
					DisplayMessageAdd(szInfo, 0xffffff00);
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(BUTTON_OBJ_ITEMMAKING1, FALSE);	// MOUSE_UP
				SetRender(BUTTON_OBJ_ITEMMAKING2, TRUE);	// MOUSE_OVER
				SetRender(BUTTON_OBJ_ITEMMAKING3, FALSE);	// MOUSE_DOWN
				m_bBtn = TRUE;
			}
			else if(eMouseCheck == MOUSE_DOWN)
			{
				SetRender(BUTTON_OBJ_ITEMMAKING1, FALSE);	// MOUSE_UP
				SetRender(BUTTON_OBJ_ITEMMAKING2, FALSE);	// MOUSE_OVER
				SetRender(BUTTON_OBJ_ITEMMAKING3, TRUE);	// MOUSE_DOWN
				m_bBtn = TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(BUTTON_OBJ_ITEMMAKING_CLOSE1, TRUE);
		SetRender(BUTTON_OBJ_ITEMMAKING_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return FALSE;
}


void CItemMakingWnd::RenderItem()
{
	int nOrder = GetStartOrder();
		
	VECTOR2	vPos;
	
	if(g_ItemMoveManager.GetNewItemMoveMode())
	{
		if(g_pMainPlayer->m_pMakingItem[0].GetID()!=0)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(g_pMainPlayer->m_pMakingItem[0].GetID());

			if(lpItemResourceEx)
			{
				vPos.x	= (float)m_fPosX+12;
				vPos.y	= (float)m_fPosZ+33;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
					, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}
		}

		for(int i = 0; i < 3; i++)
		{
			if(g_pMainPlayer->m_pMakingItem[i+1].GetID()!=0)
			{
				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
					g_pItemResourceHash->GetData(g_pMainPlayer->m_pMakingItem[i+1].GetID());

				if(lpItemResourceEx)
				{
					vPos.x	= (float)m_fPosX+142+i*37;
					vPos.y	= (float)m_fPosZ+33;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
						, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}
			}
		}
	}
	else
	{
		if(m_pMakingItem[0].GetID()!=0)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(m_pMakingItem[0].GetID());

			if(lpItemResourceEx)
			{
				vPos.x	= (float)m_fPosX+12;
				vPos.y	= (float)m_fPosZ+33;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
					, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}
		}

		for(int i = 0; i < 3; i++)
		{
			if(m_pMakingItem[i+1].GetID()!=0)
			{
				LP_ITEM_RESOURCE_EX lpItemResourceEx = 
					g_pItemResourceHash->GetData(m_pMakingItem[i+1].GetID());

				if(lpItemResourceEx)
				{
					vPos.x	= (float)m_fPosX+142+i*37;
					vPos.y	= (float)m_fPosZ+33;
					g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
						, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
				}
			}
		}	
	}	
}


void CItemMakingWnd::ItemInfoRender()
{			
	if(g_Mouse.bLDown==TRUE)
		return;

	if(g_ItemMoveManager.GetNewItemMoveMode())
	{
		if( g_Mouse.MousePos.x>m_fPosX+12 &&
			g_Mouse.MousePos.x<m_fPosX+34 &&
			g_Mouse.MousePos.y>m_fPosZ+33 &&
			g_Mouse.MousePos.y<m_fPosZ+97)
		{					
			if(CUserInterface::GetInstance()->InterfaceCollision(ITEMMAKING_WND
				, (int)m_fPosX+12, (int)m_fPosX+34, (int)m_fPosZ+33, (int)m_fPosZ+97)==FALSE)
				return;
			else if(g_pMainPlayer->m_pMakingItem[0].m_wItemID==0)
				return;
			
			CInterface::GetInstance()->ItemInfoRender(&g_pMainPlayer->m_pMakingItem[0], FALSE);
		}
		else if(g_Mouse.MousePos.x>m_fPosX+142 &&
				g_Mouse.MousePos.x<m_fPosX+248 &&
				g_Mouse.MousePos.y>m_fPosZ+33 &&
				g_Mouse.MousePos.y<m_fPosZ+97)
		{
			int nIndex	= (g_Mouse.MousePos.x-(int)m_fPosX-142)/37;		

			if(CUserInterface::GetInstance()->InterfaceCollision(ITEMMAKING_WND
				, (int)m_fPosX+142+nIndex*37, (int)m_fPosX+174+nIndex*37, (int)m_fPosZ+33, (int)m_fPosZ+97)==FALSE)
				return;
			else if(g_pMainPlayer->m_pMakingItem[nIndex+1].m_wItemID==0)
				return;
			
			CInterface::GetInstance()->ItemInfoRender(&g_pMainPlayer->m_pMakingItem[nIndex+1], FALSE);		
		}
		else
		{
			return;
		}	
	}
	else
	{
		if( g_Mouse.MousePos.x>m_fPosX+12 &&
			g_Mouse.MousePos.x<m_fPosX+34 &&
			g_Mouse.MousePos.y>m_fPosZ+33 &&
			g_Mouse.MousePos.y<m_fPosZ+97)
		{					
			if(CUserInterface::GetInstance()->InterfaceCollision(ITEMMAKING_WND
				, (int)m_fPosX+12, (int)m_fPosX+34, (int)m_fPosZ+33, (int)m_fPosZ+97)==FALSE)
				return;
			else if(m_pMakingItem[0].m_wItemID==0)
				return;
			
			CInterface::GetInstance()->ItemInfoRender(&m_pMakingItem[0], FALSE);
		}
		else if(g_Mouse.MousePos.x>m_fPosX+142 &&
				g_Mouse.MousePos.x<m_fPosX+248 &&
				g_Mouse.MousePos.y>m_fPosZ+33 &&
				g_Mouse.MousePos.y<m_fPosZ+97)
		{
			int nIndex	= (g_Mouse.MousePos.x-(int)m_fPosX-142)/37;		

			if(CUserInterface::GetInstance()->InterfaceCollision(ITEMMAKING_WND
				, (int)m_fPosX+142+nIndex*37, (int)m_fPosX+174+nIndex*37, (int)m_fPosZ+33, (int)m_fPosZ+97)==FALSE)
				return;
			else if(m_pMakingItem[nIndex+1].m_wItemID==0)
				return;
			
			CInterface::GetInstance()->ItemInfoRender(&m_pMakingItem[nIndex+1], FALSE);		
		}
		else
		{
			return;
		}
	}	
}


void CItemMakingWnd::SetButtonStatus(DWORD dwSprID, DWORD dwStatus)
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
