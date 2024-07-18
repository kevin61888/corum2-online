// CDungeonManagerWnd.cpp: implementation of the CDungeonManagerWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "CDungeonManagerWnd.h"
#include "GameControl.h"
#include "User.h"
#include "QuantityWnd.h"
#include "dungeon_data_ex.h"
#include "hashtable.h"
#include "../CommonServer/CommonHeader.h"
#include "Chat.h"
#include "UserInterface.h"
#include "QuantityWnd.h"
#include "Interface.h"
#include "DungeonProcess.h"
#include "InputManager.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "message.h"
#include "CodeFun.h"
#include "ItemNative.h"

CDungeonManagerWnd* CDungeonManagerWnd::c_pThis = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDungeonManagerWnd::CDungeonManagerWnd()
{		
	m_pDungeon	= NULL;		
}

CDungeonManagerWnd::~CDungeonManagerWnd()
{
}

BOOL CDungeonManagerWnd::Init()
{
/*
	InsertData(SPR_OBJ_DUNGEONMANAGER_WND, SPR_INTERFACE_DUNGEONMANAGER_WND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_DUNGEONMANAGER_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_DUNGEONMANAGER_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION1_1, BUTTON_INTERFACE_ACQUISITION1, 205, 79, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION1_2, BUTTON_INTERFACE_ACQUISITION2, 205, 79, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION1_3, BUTTON_INTERFACE_ACQUISITION3, 205, 79, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION2_1, BUTTON_INTERFACE_ACQUISITION1, 205, 131, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION2_2, BUTTON_INTERFACE_ACQUISITION2, 205, 131, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION2_3, BUTTON_INTERFACE_ACQUISITION3, 205, 131, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION3_1, BUTTON_INTERFACE_ACQUISITION1, 205, 44, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION3_2, BUTTON_INTERFACE_ACQUISITION2, 205, 44, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ACQUISITION3_3, BUTTON_INTERFACE_ACQUISITION3, 205, 44, 1.0f, 1.0f, 1);	
	
	InsertData(SPR_OBJ_DUNGEONMANAGER_MOF1, SPR_INTERFACE_DUNGEONMANAGER_MOF1, 205, 114, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_DUNGEONMANAGER_MOF2, SPR_INTERFACE_DUNGEONMANAGER_MOF2, 205, 114, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_DUNGEONMANAGER_MOF3, SPR_INTERFACE_DUNGEONMANAGER_MOF3, 205, 114, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);	
	InsertCheckInterface(205, 44, 249, 60, 9, CHECK_BTN);
	InsertCheckInterface(205, 79, 249, 94, 7, CHECK_BTN);
	InsertCheckInterface(205, 114, 249, 130, 12, CHECK_BTN);
	InsertCheckInterface(205, 131, 249, 147, 8, CHECK_BTN);
	InsertCheckInterface(136, 173, 168, 250, 10, CHECK_ITEM);
	InsertCheckInterface(11, 173, 43, 230, 11, CHECK_ITEM);	
	*/
					
	m_bInit = TRUE;

	return TRUE;
}

void CDungeonManagerWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;	
	m_bInit		= FALSE;
}

void CDungeonManagerWnd::SetOrder()
{
}

void CDungeonManagerWnd::RenderText()
{	
	if(m_pDungeon)
	{
		{
			// 던전 생산품
			WORD wItemID = m_pDungeon->m_wProduction;
			if (wItemID)
			{
				int nPosX = (LONG)m_fPosX+15;
//				int nPosY = (LONG)m_fPosZ+98;				
				int nPosY = (int)m_fPosZ+63;

//				LP_ITEM_TABLE	pItem = g_pItemTableHash->GetData( wItemID );
				CBaseItem*	pItem = g_pItemTableHash->GetData( wItemID );

				if (pItem)
				{
					if(m_pDungeon->m_wProduction != g_DungeonProductionItemMinMax[m_pDungeon->m_byProperty].wItemIDDefault)
						wsprintf(m_szInfo, "%s (%d%%)", pItem->szItemName_Kor, (m_pDungeon->m_byProductionCount+1)*25); 
					else
						wsprintf(m_szInfo, "%s (%d)", pItem->szItemName_Kor, m_pDungeon->m_byProductionCount); 
											
					RenderFont(m_szInfo, nPosX, nPosX+250, nPosY, nPosY+20, GetStartOrder()+3);
				}
			}
		}
		{
			// 던전 입장료
			int nPosX = (int)m_fPosX+71;
//			int nPosY = (int)m_fPosZ+116;
			int nPosY = (int)m_fPosZ+82;

			wsprintf(m_szInfo, "%d", m_pDungeon->m_dwEntrance);
			RenderFont(m_szInfo, nPosX, nPosX+100, nPosY, nPosY+20, GetStartOrder()+3);			
		}
		{
			// 던전 누적 입장료
			int nPosX = (int)m_fPosX+71;
//			int nPosY = (int)m_fPosZ+134;			
			int nPosY = (int)m_fPosZ+100;
			
			wsprintf(m_szInfo, "%d", m_pDungeon->m_dwAccEntrance);
			RenderFont(m_szInfo, nPosX, nPosX+100, nPosY, nPosY+20, GetStartOrder()+3);
		}
		{
			// 경험치
			int nPosX = (int)m_fPosX+71;
//			int nPosY = (int)m_fPosZ+46;
			int nPosY = (int)m_fPosZ+28;

			wsprintf(m_szInfo, "%d", m_pDungeon->m_dwAccExp);
			RenderFont(m_szInfo, nPosX, nPosX+100, nPosY, nPosY+20, GetStartOrder()+3);
		}
//		{
//			// 속성
//			int nPosX = (int)m_fPosX+71;
//			int nPosY = (int)m_fPosZ+28;
			
//			switch(m_pDungeon->m_byProperty)
//			{
//			case DUNGEON_PROPERTY_FIRE:
//				{
//					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1175].szMessage); // "(파이어)"
//				}break;
//			case DUNGEON_PROPERTY_ICE:
//				{
//					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1176].szMessage); // "(워터)"
//				}break;
//			case DUNGEON_PROPERTY_LIGHT:
//				{
//					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1177].szMessage); // "(라이트)"
//				}break;
//			default:
//				{
//					lstrcpy(m_szInfo, "(Unknown)"); // "(Unknown)"
//				}break;
//			}
//			RenderFont(m_szInfo, nPosX, nPosX+100, nPosY, nPosY+20, GetStartOrder()+3);
//		}
//		{
			// 상태
//			int nPosX = (int)m_fPosX+71;
//			int nPosY = (int)m_fPosZ+63;

//			switch(m_pDungeon->m_bOperationType)
//			{
//			case DUNGEON_OPERATIONTYPE_PRECOCIOUS:
//				wsprintf(m_szInfo, g_Message[ETC_MESSAGE1180].szMessage); // "숙성중"
//				break;
//			case DUNGEON_OPERATIONTYPE_PRODUCTION:
//				wsprintf(m_szInfo, g_Message[ETC_MESSAGE1181].szMessage); // "산출중"
//				break;
//			case DUNGEON_OPERATIONTYPE_CONVERSION:
//				wsprintf(m_szInfo, g_Message[ETC_MESSAGE1182].szMessage); // "변환중"
//				break;
//			}
//			RenderFont(m_szInfo, nPosX, nPosX+100, nPosY, nPosY+20, GetStartOrder()+3);						
//		}
	}
	
	RenderItem();
}

void CDungeonManagerWnd::RenderItem()
{
	int nOrder = GetStartOrder();
		
	VECTOR2	vPos;						
	
	if(m_pDungeon)
	{
		if(m_pDungeon->m_cGuardianItem.m_wItemID!=0)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(m_pDungeon->m_cGuardianItem.m_wItemID);

			if(lpItemResourceEx)
			{
				vPos.x	= (float)m_fPosX+136;
				vPos.y	= (float)m_fPosZ+175;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
					, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}
		}
		
		if(m_pDungeon->m_cMagicFieldArrayItem.m_wItemID!=0)
		{
			LP_ITEM_RESOURCE_EX lpItemResourceEx = 
				g_pItemResourceHash->GetData(m_pDungeon->m_cMagicFieldArrayItem.m_wItemID);

			if(lpItemResourceEx)
			{
				vPos.x	= (float)m_fPosX+11;
				vPos.y	= (float)m_fPosZ+175;
				g_pRenderer->RenderSprite(lpItemResourceEx->pSpr
					, NULL, 0.0f, &vPos, NULL, 0xffffffff, nOrder+3, RENDER_TYPE_DISABLE_TEX_FILTERING);
			}
		
			// 좌측 상단 좌표 //
			wsprintf(m_szInfo, "%u", m_pDungeon->m_cMagicFieldArrayItem.m_Item_MagicField_Array.bQuantity);

			int nLen = lstrlen(m_szInfo);

			RenderFont(m_szInfo, m_fPosX+43-nLen*7, m_fPosX+43, m_fPosZ+196, m_fPosZ+210, GetStartOrder()+5);
		}		
	}	
}

void CDungeonManagerWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	OnChangeTab(bActive);
	
	if(bActive==TRUE)
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
	else
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
		
}

int CDungeonManagerWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();	
	
	if(m_bBtnChk && nRt!=26)
	{
		m_bBtnChk = FALSE;
		SetRender(SPR_OBJ_DUNGEONMANAGER_MOF1, TRUE);
		SetRender(SPR_OBJ_DUNGEONMANAGER_MOF2, FALSE);
		SetRender(SPR_OBJ_DUNGEONMANAGER_MOF3, FALSE);
	}
	
	if(!m_bUpChk)
	{
		// 마우스 오버를 위한 초기화..		
		SetButtonStatus(BUTTON_OBJ_ACQUISITION1_1, 0);
		SetButtonStatus(BUTTON_OBJ_ACQUISITION2_1, 0);		
		SetButtonStatus(BUTTON_OBJ_ACQUISITION3_1, 0);		
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
				SetRender(SPR_OBJ_DUNGEONMANAGER_CLOSE1, FALSE);
				SetRender(SPR_OBJ_DUNGEONMANAGER_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4:
	case 5:
	case 6:		// togle button
		{						
			// mousedown
			if(eMouseCheck==MOUSE_DOWN)
				SetDungeonOperationType(nRt-4);	
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 7:
	case 8:		// 획득 버튼.	
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{	
				// mousedown
				SetButtonStatus(BUTTON_OBJ_ACQUISITION1_1+(nRt-7)*3, 2);
								
				if(nRt==7)
				{
					if (!m_pDungeon->m_bSiege)
					{
						// 아템 획득
						CTDS_PRODUCT_ACQUISITION packet;
						packet.wDungeonID = (WORD)m_pDungeon->m_dwID;
						g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
					}
					
				}
				else if(nRt==8)
				{
					// 입장료 획득
					if (m_pDungeon->m_dwAccEntrance)
					{
						CTDS_ACCENTRANCE_ACQUISITION packet;
						packet.wDungeonID = (WORD)m_pDungeon->m_dwID;						
						g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
					}					
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{	
				// mouseover
				SetButtonStatus(BUTTON_OBJ_ACQUISITION1_1+(nRt-7)*3, 1);				
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 9:		// 경험치
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				// mousedown
				SetButtonStatus(BUTTON_OBJ_ACQUISITION3_1, 2);
								
				if(nRt==9)
				{
					// 경험치 획득
					if(m_pDungeon->m_dwAccExp)
					{
						CTDS_ACCEXP_ACQUISITION packet;
						packet.wDungeonID = (WORD)m_pDungeon->m_dwID;						
						g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
					}
				}				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{	
				// mouseover
				SetButtonStatus(BUTTON_OBJ_ACQUISITION3_1, 1);
				
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;	
	case 10:
		{			
			BYTE byType = 0;

			if(m_pDungeon->m_cGuardianItem.GetID()==0)
				byType = __MOUSE_POINTER_DEFAULT__;			

			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_pDungeon->m_cGuardianItem.GetID()!=0)
				{
					// 던전관리창에서 마우스로 가디언아이템 이동 //
					if(!g_pThisDungeon->m_bSiege)
					{
						CTDS_ITEM_PICKUP ItemPickup;	
						ItemPickup.bSectionNum	= 1;
						ItemPickup.i64ItemID	= 0;
						SetItemPacket(&ItemPickup, 43, 0, 0, 0, 0);
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					}
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byType);
		}
		break;				
	case 11:
		{
			BYTE byType = 0;

			if(m_pDungeon->m_cMagicFieldArrayItem.GetID()==0)
				byType = __MOUSE_POINTER_DEFAULT__;
						
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(!m_pDungeon->m_bSiege &&
					m_pDungeon->m_cMagicFieldArrayItem.GetID()!=0 &&
					!g_pMainPlayer->m_MouseItem.GetID())
				{
					// 던전관리창에서 마우스로 가디언아이템 이동 //
					CTDS_ITEM_PICKUP ItemPickup;	
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;
					SetItemPacket(&ItemPickup, 64, 0, 0, 0, 0);
					g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);										
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byType);
		}
		break;		
	case 12:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				m_bBtnChk = TRUE;
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF1, FALSE);
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF2, TRUE);
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF3, FALSE);

				CQuantityWnd*	pQuantityWnd	= CQuantityWnd::GetInstance();
				CInterface*		pInterface		= CInterface::GetInstance();

				if(pQuantityWnd->GetActive()==FALSE)
					pQuantityWnd->OpenWnd(__QUANTITY_DUNGEON__);
				else
					pQuantityWnd->SetActive(FALSE);
				
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
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				m_bBtnChk = TRUE;
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF1, FALSE);
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF2, FALSE);
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF3, TRUE);				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				m_bBtnChk = TRUE;
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF1, FALSE);
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF2, TRUE);
				SetRender(SPR_OBJ_DUNGEONMANAGER_MOF3, FALSE);				
			}		

			SetMouseCheck(nRt, eMouseCheck);
		}			
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_DUNGEONMANAGER_CLOSE1, TRUE);
		SetRender(SPR_OBJ_DUNGEONMANAGER_CLOSE1, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

void CDungeonManagerWnd::OnChangeTab(int nIndex)
{
	m_nTabIndex	= nIndex;
//	m_pComboBox->SetActive(nIndex);
	
	bool bActive = 0;	// 일단 모두 안보이기.
	
	SetRender(SPR_OBJ_DUNGEONMANAGER_WND, bActive);
	SetRender(SPR_OBJ_DUNGEONMANAGER_CLOSE1, bActive);
	SetRender(SPR_OBJ_DUNGEONMANAGER_CLOSE2, bActive);
	

	SetRender(BUTTON_OBJ_ACQUISITION1_1, bActive);
	SetRender(BUTTON_OBJ_ACQUISITION1_2, bActive);
	SetRender(BUTTON_OBJ_ACQUISITION1_3, bActive);
	SetRender(BUTTON_OBJ_ACQUISITION2_1, bActive);
	SetRender(BUTTON_OBJ_ACQUISITION2_2, bActive);
	SetRender(BUTTON_OBJ_ACQUISITION2_3, bActive);
	SetRender(BUTTON_OBJ_ACQUISITION3_1, bActive);
	SetRender(BUTTON_OBJ_ACQUISITION3_2, bActive);
	SetRender(BUTTON_OBJ_ACQUISITION3_3, bActive);

	SetRender(SPR_OBJ_DUNGEONMANAGER_MOF1, bActive);
	SetRender(SPR_OBJ_DUNGEONMANAGER_MOF2, bActive);
	SetRender(SPR_OBJ_DUNGEONMANAGER_MOF3, bActive);			
	
	bActive = 1;
	
	if(nIndex)
	{
		// 보일때 무조건 보여줘야 하는것들.
		SetRender(SPR_OBJ_DUNGEONMANAGER_WND, bActive);
		SetRender(SPR_OBJ_DUNGEONMANAGER_CLOSE1, bActive);
		SetRender(BUTTON_OBJ_ACQUISITION1_1, bActive);
		SetRender(BUTTON_OBJ_ACQUISITION2_1, bActive);
		SetRender(BUTTON_OBJ_ACQUISITION3_1, bActive);		
		SetRender(SPR_OBJ_DUNGEONMANAGER_MOF1, bActive);								

		if (m_pDungeon)
			SetDungeonOperationType(m_pDungeon->m_bOperationType);
	}	
}

void CDungeonManagerWnd::SetButtonStatus(DWORD dwSprID, DWORD dwStatus)
{
	switch(dwStatus)
	{
	case 0:		//nomal
		{
			SetRender(dwSprID, TRUE);
			SetRender(dwSprID+1, FALSE);
			SetRender(dwSprID+2, FALSE);
		}
		break;
	case 1:		//mouseover
		{
			SetRender(dwSprID, FALSE);
			SetRender(dwSprID+1, TRUE);
			SetRender(dwSprID+2, FALSE);
		}
		break;
	case 2:		//mousedown
		{
			SetRender(dwSprID, FALSE);
			SetRender(dwSprID+1, FALSE);
			SetRender(dwSprID+2, TRUE);
		}
		break;
	}
}

CItem* CDungeonManagerWnd::GetGuardianItem()
{	
	return &m_pDungeon->m_cGuardianItem;
}

CItem* CDungeonManagerWnd::GetMagicArrayItem()
{
	return &m_pDungeon->m_cMagicFieldArrayItem;
}

void CDungeonManagerWnd::SetItem(CItem* pItem, BOOL bView)
{

}


void CDungeonManagerWnd::SetDungeonOperationType(BYTE bOperationType)
{			
	m_pDungeon->SetOperationMode(bOperationType);
}


void CDungeonManagerWnd::SetDungeon(DUNGEON_DATA_EX *pDungeon)
{
	m_pDungeon = pDungeon;		
}