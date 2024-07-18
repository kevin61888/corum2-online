#include "ItemUpgradeSubWnd.h"
#include "ItemUpgrade.h"
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


CItemUpgradeSubWnd* CItemUpgradeSubWnd::c_pThis = NULL;

CItemUpgradeSubWnd::CItemUpgradeSubWnd()
{
	memset(&m_rtPos,0,sizeof(m_rtPos));
	m_byItemUpgradeProcess	= 0;
	m_dwNpcID				= 0;
	m_bBtn					= FALSE;	
}


CItemUpgradeSubWnd::~CItemUpgradeSubWnd()
{

}


BOOL CItemUpgradeSubWnd::Init()
{
/*
	InsertData(SPR_OBJ_ITEM_UPGRADE_SUB_WND, SPR_INTERFACE_OKWND2, 0, 0, 1.0f, 1.0f, 0);
	InsertData(BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_ITEMUPGRADE_SUB_OK1, SPR_INTERFACE_OK1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(BUTTON_OBJ_ITEMUPGRADE_SUB_OK2, SPR_INTERFACE_OK2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(BUTTON_OBJ_ITEMUPGRADE_SUB_OK3, SPR_INTERFACE_OK3, 189, 110, 1.0f, 1.0f, 2);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(189, 110, 253, 126, 4, CHECK_BTN);
*/
	m_bInit	= TRUE;

	return TRUE;
}


void CItemUpgradeSubWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}


void CItemUpgradeSubWnd::RenderText()
{
	RenderFont(g_NPCTable[m_dwNpcID].szName, m_fPosX+25, m_fPosX+95, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

	if(m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_START)
	{
		// "자네 좀더 강한것을 찾아 나에게 왔나?"		
		RenderFont(g_Message[ETC_MESSAGE1032].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

		// "제대로 찾아왔구먼.."		
		RenderFont(g_Message[ETC_MESSAGE1033].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);
	}
	else if(m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_READY)
	{
		// "강력한 무기를 위한 준비물은"		
		RenderFont(g_Message[ETC_MESSAGE1034].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

		// "모두 가져왔나? 이리줘보게.."		
		RenderFont(g_Message[ETC_MESSAGE1035].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);	
	}
	else if	(m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_SUCCESS)
	{
		// "자 다되었네..."		
		RenderFont(g_Message[ETC_MESSAGE1036].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);
				
		// "최고장비의 탄생을 축하하자구.."		
		RenderFont(g_Message[ETC_MESSAGE1037].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);
	}
	else if	(m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_FAIL)
	{
		// "이런. 재료가 잘못되었었던 모양이야."		
		RenderFont(g_Message[ETC_MESSAGE1038].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

		// "무기가 망가져버렸네.."		
		RenderFont(g_Message[ETC_MESSAGE1039].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);
		
		// "하지만 내 실력을 의심하진 말라구.."		
		RenderFont(g_Message[ETC_MESSAGE1040].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+74, m_fPosZ+88, GetStartOrder()+1);
	}
	else if (m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_FAIL_INSURANCE)
	{
		// "재료가 안좋았는데"		
		RenderFont(g_Message[ETC_MESSAGE1138].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

		// "그나마 다행이군."		
		RenderFont(g_Message[ETC_MESSAGE1139].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+57, m_fPosZ+71, GetStartOrder()+1);
		
		// "그카드 덕분인가?"		
		RenderFont(g_Message[ETC_MESSAGE1140].szMessage, m_fPosX+15, m_fPosX+250, m_fPosZ+74, m_fPosZ+88, GetStartOrder()+1);		
	}
}

void CItemUpgradeSubWnd::SetOrder()
{
	
}

void CItemUpgradeSubWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_ITEM_UPGRADE_SUB_WND, bActive);
		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE1, bActive);
		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK1, bActive);
//		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK2, bActive);
//		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK3, bActive);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}


int CItemUpgradeSubWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;
	
	int nRt = GetChk();

	if(m_bBtn && nRt != 4)
	{
		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK1, TRUE);
		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK2, FALSE);
		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK3, FALSE);
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

		// CHECK_CLOSE
		case 2:
			{			
				if(eMouseCheck == MOUSE_UP)
				{
					SetActive(FALSE);
					CItemUpgradeWnd* pItemUpgrade = CItemUpgradeWnd::GetInstance();
					pItemUpgrade->m_bWaitingUpgrade = 0;
				}
				else if(eMouseCheck == MOUSE_DOWN)
				{
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE1, FALSE);
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE2, TRUE);
				}
				
				SetMouseCheck(nRt, eMouseCheck);
			}
			break;

		// CHECK_MOVEWND
		case 3:			
			{
				SetMouseCheck(nRt, eMouseCheck);
			}
			break;

		// CHECK_BTN
		case 4:
			{
				if(eMouseCheck == MOUSE_UP)
				{
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK1, TRUE);
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK2, FALSE);
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK3, FALSE);

					if(m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_START)
					{
						m_byItemUpgradeProcess++;
					}
					else if(m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_READY)
					{
						CTDS_ITEMUPGRADE packet;
						g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);						
					}
					else if(m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_SUCCESS	||
							m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_FAIL		||
							m_byItemUpgradeProcess == ITEM_UPGRADE_PROCESS_FAIL_INSURANCE)
					{
						m_byItemUpgradeProcess = 0;
						SetActive(FALSE);
					}
				}
				else if(eMouseCheck == MOUSE_OVER)
				{
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK1, FALSE);
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK2, TRUE);
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK3, FALSE);
					m_bBtn = TRUE;
				}
				else if(eMouseCheck == MOUSE_DOWN)
				{
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK1, FALSE);
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK2, FALSE);
					SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_OK3, TRUE);
					m_bBtn = TRUE;
				}
				
				SetMouseCheck(nRt, eMouseCheck);
			}
			break;
	}

	if(MouseUp())
	{
		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE1, TRUE);
		SetRender(BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE2, FALSE);		
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


