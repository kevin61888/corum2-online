//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "InitItemWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"

CInitItemWnd* CInitItemWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CInitItemWnd::CInitItemWnd()
{ 	
	m_byIndex	= 0;
	m_byType	= 0;
	m_bBtn[0]	= FALSE;
	m_bBtn[1]	= FALSE;
}

CInitItemWnd::~CInitItemWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CInitItemWnd::Init()
{	
/*
	InsertData(SPR_OBJ_INITITEM_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_INITITEM_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_INITITEM_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_INITITEM_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_INITITEM_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_INITITEM_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_INITITEM_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_INITITEM_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_INITITEM_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CInitItemWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CInitItemWnd::SetOrder()
{
}

void CInitItemWnd::RenderText()
{
	if(m_byType==1)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE158].szMessage); // "스텟 포인트"
	else if(m_byType==2)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE159].szMessage); // "스킬 포인트"

	int nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+25+nSize*7, m_fPosZ+6, m_fPosZ+18, GetStartOrder()+1);	
				
	if(m_byType==1)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE160].szMessage); // "스텟 포인트를 초기화하시겠습니까?"
	else if(m_byType==2)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE161].szMessage); // "스킬 포인트를 초기화하시겠습니까?"
	
	nSize = lstrlen(m_szInfo);
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+52, GetStartOrder()+1);
}

void CInitItemWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_INITITEM_WND, TRUE);
		SetRender(SPR_OBJ_INITITEM_CLOSE1, TRUE);
		SetRender(SPR_OBJ_INITITEM_OK1, TRUE);
		SetRender(SPR_OBJ_INITITEM_CANCEL1, TRUE);
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}


int CInitItemWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_INITITEM_OK1, TRUE);
		SetRender(SPR_OBJ_INITITEM_OK2, FALSE);
		SetRender(SPR_OBJ_INITITEM_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_INITITEM_CANCEL1, TRUE);
		SetRender(SPR_OBJ_INITITEM_CANCEL2, FALSE);
		SetRender(SPR_OBJ_INITITEM_CANCEL3, FALSE);
		m_bBtn[1] = FALSE;
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
				SetRender(SPR_OBJ_INITITEM_CLOSE1, FALSE);
				SetRender(SPR_OBJ_INITITEM_CLOSE2, TRUE);
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
		{			
			if(eMouseCheck==MOUSE_UP)
			{				
				WORD wId = g_pMainPlayer->m_pInv_Small[m_byIndex].GetID();
															
				if(	wId==__ITEM_STATUS_INT__ ||
					(wId>=__ITEM_STATUS_INTSTART__ && wId<=__ITEM_STATUS_INTEND__) ||
					wId==__ITEM_SKILL_INT__ ||
					(wId>=__ITEM_SKILL_INTSTART__ && wId<=__ITEM_SKILL_INTEND__))
				{
					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bInv			= 69;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;
					ItemPickup.bZipCode		= m_byIndex;
					g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL,m_byIndex,0xff);
					}
				}
				else
				{
					// "아이템이 없습니다"
					DisplayMessageAdd(g_Message[ETC_MESSAGE144].szMessage, 0xffFF0000); 
				}
							
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_INITITEM_OK1, FALSE);
				SetRender(SPR_OBJ_INITITEM_OK2, FALSE);
				SetRender(SPR_OBJ_INITITEM_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{								
				SetRender(SPR_OBJ_INITITEM_OK1, FALSE);
				SetRender(SPR_OBJ_INITITEM_OK2, TRUE);
				SetRender(SPR_OBJ_INITITEM_OK3, FALSE);				
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{					
				SetRender(SPR_OBJ_INITITEM_CANCEL1, FALSE);
				SetRender(SPR_OBJ_INITITEM_CANCEL2, FALSE);
				SetRender(SPR_OBJ_INITITEM_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_INITITEM_CANCEL1, FALSE);
				SetRender(SPR_OBJ_INITITEM_CANCEL2, TRUE);
				SetRender(SPR_OBJ_INITITEM_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_INITITEM_CLOSE1, TRUE);
		SetRender(SPR_OBJ_INITITEM_CLOSE2, FALSE);		
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
