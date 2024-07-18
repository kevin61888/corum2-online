//==========================================================//
// Code by Jang.								2003.02.04	//
//==========================================================//
#include "QuantityWnd.h"
#include "GameControl.h"
#include "StoreWnd.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "ItemTradeShopWnd.h"
#include "PlayerShopWnd.h"
#include "InputManager.h"
#include "DUNGEON_DATA_EX.h"
#include "InitGame.h"
#include "DungeonProcess.h"
#include "NetworkClient.h"
#include "CharSelectProcess.h"
#include "LoginProcess.h"
#include "CodeFun.h"
#include "ItemNative.h"



CQuantityWnd* CQuantityWnd::c_pThis = NULL;


//==========================================================//
// Construction/Destruction.								//
//==========================================================//
CQuantityWnd::CQuantityWnd()
{	
	m_bySkill		= 0xff;
	m_byOrder		= 0;	
	m_byDrawIndex	= 0;	
	m_byChatLine	= 0;
	m_byZipCode		= 0;
	m_byQuantityAct = 0;
	m_bQuantityType	= __QUANTITY_MONEY__ - 1;	
	m_bReq			= FALSE;			
	m_bBtn[0]		= FALSE;
	m_bBtn[1]		= FALSE;
	m_fItemX		= 0;
	m_fItemY		= 0;
	m_fItemZ		= 0;
	m_bIsOkButton	= 0;

	m_rtPos[0].left		= (LONG)m_fPosX+5;
	m_rtPos[0].right	= (LONG)m_fPosX+128;
	m_rtPos[0].top		= (LONG)m_fPosZ+33;
	m_rtPos[0].bottom	= (LONG)m_fPosZ+45;
	
	m_rtPos[1].left		= (LONG)m_fPosX+5;
	m_rtPos[1].right	= (LONG)m_fPosX+128;
	m_rtPos[1].top		= (LONG)m_fPosZ+50;
	m_rtPos[1].bottom	= (LONG)m_fPosZ+62;


	m_rtPos[2].left		= (LONG)m_fPosX+5;
	m_rtPos[2].right	= (LONG)m_fPosX+128;
	m_rtPos[2].top		= (LONG)m_fPosZ+67;
	m_rtPos[2].bottom	= (LONG)m_fPosZ+79;
} 


CQuantityWnd::~CQuantityWnd()
{

}


//==========================================================//
// Class Member Function.									//
//==========================================================//
BOOL CQuantityWnd::Init()
{
/*
	InsertData(SPR_OBJ_QUANTITY_WND, SPR_INTERFACE_QUANTITY_WND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_QUANTITY_CLOSE1, SPR_INTERFACE_CLOSE1, 114, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_QUANTITY_CLOSE2, SPR_INTERFACE_CLOSE2, 114, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NAME_TRUE1, SPR_INTERFACE_OK1, 0, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NAME_TRUE2, SPR_INTERFACE_OK2, 0, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NAME_TRUE3, SPR_INTERFACE_OK3, 0, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NAME_FALSE1, SPR_INTERFACE_CANCEL1, 64, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NAME_FALSE2, SPR_INTERFACE_CANCEL2, 64, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NAME_FALSE3, SPR_INTERFACE_CANCEL3, 64, 110, 1.0f, 1.0f, 1);		
	
	InsertCheckInterface(114, 4, 126, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 114, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(0, 110, 63, 125, 4, CHECK_BTN);
	InsertCheckInterface(64, 110, 127, 125, 5, CHECK_BTN);
*/
	m_rtPos[0].left		= (LONG)m_fPosX+5;
	m_rtPos[0].right	= (LONG)m_fPosX+128;
	m_rtPos[0].top		= (LONG)m_fPosZ+33;
	m_rtPos[0].bottom	= (LONG)m_fPosZ+45;
	
	m_rtPos[1].left		= (LONG)m_fPosX+5;
	m_rtPos[1].right	= (LONG)m_fPosX+128;
	m_rtPos[1].top		= (LONG)m_fPosZ+50;
	m_rtPos[1].bottom	= (LONG)m_fPosZ+62;


	m_rtPos[2].left		= (LONG)m_fPosX+5;
	m_rtPos[2].right	= (LONG)m_fPosX+128;
	m_rtPos[2].top		= (LONG)m_fPosZ+67;
	m_rtPos[2].bottom	= (LONG)m_fPosZ+79;

	m_bInit	= TRUE;

	return TRUE;
}

void CQuantityWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CQuantityWnd::OpenWnd(BYTE bQuantityType)
{
	m_bQuantityType = bQuantityType;
	SetActive(TRUE);

	SetOrder();
}

void CQuantityWnd::SetOrder()
{	
	if(m_bQuantityType== __QUANTITY_JUMIN_NUMBER__ )
		g_pInputManager->SetInputOrder(INPUT_BUFFER_CHARACTER_JUMIN, GetStartOrder()+2);
	else
		g_pInputManager->SetInputOrder(INPUT_BUFFER_5, GetStartOrder()+2);
}

void CQuantityWnd::RenderText()
{
	switch(m_bQuantityType)
	{
	case __QUANTITY_MONEY__:
		{						
			// MSG_ID : 360 ; 떨어뜨릴 돈에 액수
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE360].szMessage);		
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);
			
			// MSG_ID : 361 ; 를 입력해 주십시오.
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE361].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 362 ; Karz : 
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE362].szMessage);		
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_TRADEMONEY__:
		{
			// "교환하실 돈에 액수"
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE630].szMessage); 
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 361 ; 를 입력해 주십시오.
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE361].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 362 ; Karz : 
			lstrcpy(m_szInfo, g_Message[ETC_MESSAGE362].szMessage);		
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_THROW__:
		{			
			// MSG_ID : 363 ; 떨어뜨릴 수량을 입
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE363].szMessage);	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 364 ; 력해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE364].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);		
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_BUY__:
		{
			// MSG_ID : 366 ; 구입할 수량을 입력
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE366].szMessage);	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 367 ; 해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE367].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);				
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_PLAYERSHOPSELLCNT__:
		{
			// MSG_ID : 366 ; 구입할 수량을 입력
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE366].szMessage);	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 367 ; 해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE367].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);				
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_STORE__:
		{
			// MSG_ID : 368 ; 판매할 수량을 입력
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE368].szMessage);	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);
		
			// MSG_ID : 367 ; 해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE367].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
					
			// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);				
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_BANKINSERT__:
		{		
			// MSG_ID : 369 ; 창고에 저장할 수량
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE369].szMessage);	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 370 ; 을 입력해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE370].szMessage);	
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);				
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_FROMBANKTOINV__:
		{
			wsprintf(m_szInfo, "인벤에 저장할 수량");	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 370 ; 을 입력해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE370].szMessage);	
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);				
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_BNAKMONEYINSERT__:
		{
			// MSG_ID : 371 ; 입금할 금액을 입
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE371].szMessage);	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 364 ; 력해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE364].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 362 ; Karz : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE362].szMessage);				
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_BNAKMONEYPOP__:
		{
			// MSG_ID : 372 ; 출금할 금액을 입
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE372].szMessage);	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 364 ; 력해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE364].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 362 ; Karz : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE362].szMessage);			
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_DUNGEONMANAGEINSERT__:
		{
			// MSG_ID : 373 ; 설치할 마법진의 수량
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE373].szMessage);	
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// MSG_ID : 370 ; 을 입력해 주십시오.
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE370].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);					
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_TRADE__:
		{
			// "교환할 수량을 입력"
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE631].szMessage); 
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// "해 주십시오."
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE367].szMessage); 
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 수량 :
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);					 
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_PLAYERSHOP__:
		{
			// "가격을 입력해"
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE632].szMessage); 
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// "주십시오."
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE633].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// "가격 :"				// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE634].szMessage);	
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_PLAYERSHOPCNT__:
		{
			// "수량을 입력해"
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE635].szMessage); 
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// "주십시오."
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE633].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 수량 : 
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE365].szMessage);					
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_DUNGEON__:
		{
			// "입장료을 입력해"
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE813].szMessage); 
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);

			// "주십시오."
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE633].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// MSG_ID : 365 ; 가격 :
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE634].szMessage);					 
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	case __QUANTITY_JUMIN_NUMBER__:
		{						
			// "주민등록번호 뒷번호"
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE852].szMessage);		
			RenderFont(m_szInfo, m_rtPos[0].left, m_rtPos[0].right, m_rtPos[0].top, m_rtPos[0].bottom, GetStartOrder()+1);
			
			// "7자리를 입력해 주십"
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE853].szMessage);		
			RenderFont(m_szInfo, m_rtPos[1].left, m_rtPos[1].right, m_rtPos[1].top, m_rtPos[1].bottom, GetStartOrder()+1);
		
			// "시오."
			wsprintf(m_szInfo, "%s", g_Message[ETC_MESSAGE854].szMessage);					
			RenderFont(m_szInfo, m_rtPos[2].left, m_rtPos[2].right, m_rtPos[2].top, m_rtPos[2].bottom, GetStartOrder()+1);
		}
		break;
	}
	
	char		szPrice[0xff]={0,};
	NUMBERFMT	nFmt = { 0, 0, 3, ".", ",", 1 };

	if(!IsEmptyString(m_szMoney))
	{
		::GetNumberFormat(NULL, NULL, m_szMoney, &nFmt, szPrice, 15);
	}

	m_rtPos[4].left		= (LONG)m_fPosX+120-lstrlen(szPrice)*6;
	m_rtPos[4].right	= (LONG)m_fPosX+130;
	m_rtPos[4].top		= (LONG)m_fPosZ+84;
	m_rtPos[4].bottom	= (LONG)m_fPosZ+96;
	
	if(GetGameStatus()==UPDATE_GAME_CHAR_SELECT)
	{
		if(m_bQuantityType!=__QUANTITY_JUMIN_NUMBER__)
		{			
			if(g_pInputManager->IsDrawCursor())
			{
				lstrcat(szPrice, "|");
			}
		}
	}	

	RenderFont(szPrice, m_rtPos[4].left, m_rtPos[4].right, m_rtPos[4].top, m_rtPos[4].bottom, GetStartOrder()+1);	
}

void CQuantityWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;

#ifdef _USE_IME
	GET_IMEEDIT()->SetInputBlock(bActive);
#endif
	
	if(bActive == FALSE)
	{
		if(m_byQuantityAct == 0 && m_bQuantityType != __QUANTITY_JUMIN_NUMBER__)
		{
			if(!m_bReq)
			{
				if(g_pMainPlayer && g_pMainPlayer->m_MouseItem.GetID()!=0)
				{
					int nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;

					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;

					if(	m_bQuantityType==__QUANTITY_PLAYERSHOP__	||
						m_bQuantityType==__QUANTITY_PLAYERSHOPCNT__ ||
						m_bQuantityType==__QUANTITY_STORE__)
					{
						// 인벤토리에 넣기 //
						if(GetType((WORD)nValue)==ITEM_SMALL)
						{
							InvSmallInsert(&ItemPickup, &g_pMainPlayer->m_MouseItem);
						}
						else
						{
							InvLargeInsert(&ItemPickup);
						}

						m_bReq = TRUE;
					}
				}
			}			
		}
		
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			if(m_bQuantityType==__QUANTITY_MONEY__)
			{
				g_ItemMoveManager.Initialize();
			}
		}

		m_byQuantityAct = 0;		
		m_byDrawIndex	= 0;
		m_bSoundChk		= FALSE;
		
		memset(m_szMoney, 0, sizeof(m_szMoney));		
		::KillTimer(g_hMainWnd, 1);
				
		ShowSpriteAll();
	
		if(m_bQuantityType == __QUANTITY_JUMIN_NUMBER__)
			g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_JUMIN);			
		else
			g_pInputManager->GetInputBuffer(INPUT_BUFFER_5);							
		
		//if ( GetGameStatus() != UPDATE_GAME_LOGIN)   // 로그인시 꽝하는 사운드 이펙트 안나오게 안한다. 김영대 2003.02.22
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);				
	}
	else
	{		
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_NAME_TRUE1, TRUE);
		SetRender(SPR_OBJ_NAME_FALSE1, TRUE);
		SetRender(SPR_OBJ_QUANTITY_WND, TRUE);
		SetRender(SPR_OBJ_QUANTITY_CLOSE1, TRUE);						

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}


int CQuantityWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_NAME_TRUE1, TRUE);
		SetRender(SPR_OBJ_NAME_TRUE2, FALSE);
		SetRender(SPR_OBJ_NAME_TRUE3, FALSE);
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_NAME_FALSE1, TRUE);
		SetRender(SPR_OBJ_NAME_FALSE2, FALSE);
		SetRender(SPR_OBJ_NAME_FALSE3, FALSE);
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
			{
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					g_ItemMoveManager.Initialize();
				}
				else
				{
					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;
						
					// Process //
					if(m_bQuantityType==__QUANTITY_STORE__)
					{								
						int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
						BOOL	bChk = FALSE;
						
						if(nVal==50)
						{
//							LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

							BYTE byMaxCnt = 0;

							if(lpItemTable)
								byMaxCnt = lpItemTable->GetMaxQuantity();

							for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
								else
								{								
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
										{
											ItemPickup.bInv		= 14;
											ItemPickup.bZipCode	= i;
											bChk				= TRUE;
											break;
										}
									}															
								}
							}
							
							if(bChk==FALSE)
								ItemPickup.bInv	= 4;
							
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							m_bReq = TRUE;
						}
					}			
					else if(m_bQuantityType==__QUANTITY_THROW__)
					{				
						int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
						BOOL	bChk = FALSE;

//						if(nVal==50)
						{
//							LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

							BYTE byMaxCnt = 0;

							if(lpItemTable)
								byMaxCnt = lpItemTable->GetMaxQuantity();

							for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
								else
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
										{
											ItemPickup.bInv		= 14;
											ItemPickup.bZipCode	= i;
											bChk				= TRUE;
											break;
										}
									}
								}
							}
							
							if(bChk==FALSE)
								ItemPickup.bInv	= 4;										
							
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							m_bReq = TRUE;
						}
					}
					else if(m_bQuantityType==__QUANTITY_BANKINSERT__)
					{
						int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
						BOOL	bChk = FALSE;

						if(nVal==50)
						{
//							LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

							BYTE byMaxCnt = 0;

							if(lpItemTable)
								byMaxCnt = lpItemTable->GetMaxQuantity();

							for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
								else
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
										{
											ItemPickup.bInv		= 14;
											ItemPickup.bZipCode	= i;
											bChk				= TRUE;
											break;
										}
									}															
								}
							}
							
							if(bChk==FALSE)
								ItemPickup.bInv	= 4;
							
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							m_bReq = TRUE;
						}
					}
					else if(m_bQuantityType==__QUANTITY_FROMBANKTOINV__)
					{
						
					}
					else if(m_bQuantityType==__QUANTITY_DUNGEONMANAGEINSERT__)
					{
						int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;				
						BOOL	bChk = FALSE;

//						if(nVal==50)
						{
//							LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

							BYTE byMaxCnt = 0;

							if(lpItemTable)
								byMaxCnt = lpItemTable->GetMaxQuantity();

							for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
								else
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
										{
											ItemPickup.bInv		= 14;
											ItemPickup.bZipCode	= i;
											bChk				= TRUE;
											break;
										}
									}															
								}
							}
							
							if(bChk==FALSE)
								ItemPickup.bInv	= 4;
							
							g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
							m_bReq = TRUE;
						}
					}
				}

				SetActive(FALSE);							
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_QUANTITY_CLOSE1, FALSE);
				SetRender(SPR_OBJ_QUANTITY_CLOSE2, TRUE);				
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
			// 확인.
			if(eMouseCheck==MOUSE_UP)
			{
				// 확인.								
				CTDS_ITEM_PICKUP ItemPickup;	
				ItemPickup.bSectionNum	= 1;
				ItemPickup.i64ItemID	= 0;

				BOOL	bChk	= FALSE;
				__int64	nMoney	= _atoi64(m_szMoney);

				// Process //
				if(m_bQuantityType==__QUANTITY_MONEY__)
				{
// 넷클루 요청으로 Karz 땅에 떨어뜨지지 못하게 2005.02.05 김영대 
#if  IS_JAPAN_LOCALIZING()
					DisplayMessageAdd(g_Message[ETC_MESSAGE1372].szMessage, 0xffff0000);
#else
					if(nMoney>0)
					{
						ItemPickup.bInv		= 13;
						ItemPickup.dwMoney	= (DWORD)nMoney;
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TILE,0,1,1,(DWORD)nMoney);
						}
					}
#endif
				}
				else if(m_bQuantityType==__QUANTITY_TRADEMONEY__)
				{
					if(nMoney>0)
					{
						CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();
						
						if(!pItemTradeShopWnd->m_bOkChk)
						{
							pItemTradeShopWnd->m_dwMoney = (DWORD)nMoney;
						}
						else
						{
							// "교환 확인을 하셨습니다. Karz를 교환 하실수 없습니다."
							DisplayMessageAdd(g_Message[ETC_MESSAGE1255].szMessage, 0xffff0000);
							SetActive(FALSE);
							return 0;
						}
					}
				}
				else if(m_bQuantityType==__QUANTITY_PLAYERSHOP__)
				{
					if(nMoney>0)
					{
						int nValue = 0;

						if(g_ItemMoveManager.GetNewItemMoveMode())
							nValue = g_ItemMoveManager.GetMouseItemID() / ITEM_DISTRIBUTE;
						else
							nValue = g_pMainPlayer->m_MouseItem.GetID() / ITEM_DISTRIBUTE;
						
						if(nValue<=20)
						{
							ItemPickup.bInv		= 57;
							ItemPickup.bZipCode	= m_byZipCode;
							ItemPickup.dwMoney	= (DWORD)nMoney;
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

							if(g_ItemMoveManager.GetNewItemMoveMode())
								g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_PLAYERSHOP,m_byZipCode,1,1,(DWORD)nMoney);

							m_bReq = TRUE;
						}
						else
						{	
							RECT rt;
							rt.left		= (LONG)m_fPosX+124-7;
							rt.right	= (LONG)m_fPosX+124;
							rt.top		= (LONG)m_fPosZ+83;
							rt.bottom	= (LONG)m_fPosZ+95;
							int nOrder	= GetStartOrder()+2;

							OpenWnd(__QUANTITY_PLAYERSHOPCNT__);

							g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
							g_pInputManager->SetFocusInput(INPUT_BUFFER_5);
							
							CInterface::GetInstance()->SetWnd(QUANTITY_WND);

							memset(m_szMoney, 0, sizeof(m_szMoney));
							m_byDrawIndex	= 0;						
		
							// 코드, 가격 //
							CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
							pPlayerShopWnd->m_dwMoney = (DWORD)nMoney;						
							return CInterface::GetInstance()->GetSetWnd();
						}
					}
				}
				else if(m_bQuantityType==__QUANTITY_PLAYERSHOPSELLCNT__)
				{
					if(nMoney>0)
					{										
						CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

						ItemPickup.bInv			= 73;				
						ItemPickup.bZipCode		= pPlayerShopWnd->m_bZipCode;					
						ItemPickup.dwMoney		= pPlayerShopWnd->m_pOwnerPlayerShop->m_dwUserIndex;
						ItemPickup.dwId			= CStoreWnd::GetInstance()->m_wSuppliesId;
						ItemPickup.dwShop		= pPlayerShopWnd->m_sPlayerShopInfo.m_dwCustomSellPrice[pPlayerShopWnd->m_bZipCode];
						ItemPickup.bZipCode2	= (BYTE)nMoney;
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						m_bReq = TRUE;

						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL
								, m_byZipCode
								, 1
								, (BYTE)nMoney
								, pPlayerShopWnd->m_sPlayerShopInfo.m_dwCustomSellPrice[pPlayerShopWnd->m_bZipCode]
								, CStoreWnd::GetInstance()->m_wSuppliesId);
						}
					}
				}
				else if(m_bQuantityType==__QUANTITY_PLAYERSHOPCNT__)
				{
					if(nMoney>0)
					{
						int nValue = 0;

						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							nValue = g_ItemMoveManager.GetMouseItemID() / ITEM_DISTRIBUTE;
						}
						else
						{
							nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;
						}

						if(nValue>20)
						{	
							CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

							ItemPickup.bInv		= 59;
							ItemPickup.dwMoney	= (DWORD)nMoney;
							ItemPickup.bZipCode	= m_byZipCode;
							ItemPickup.dwId		= pPlayerShopWnd->m_dwMoney;
							g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

							if(g_ItemMoveManager.GetNewItemMoveMode())
							{
								g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_PLAYERSHOP
									,m_byZipCode,1,(BYTE)nMoney,pPlayerShopWnd->m_dwMoney);
							}

							m_bReq = TRUE;
						}
					}
				}
				else if(m_bQuantityType==__QUANTITY_BNAKMONEYPOP__)
				{
					if(nMoney>0)
					{
						ItemPickup.bInv		= 33;
						ItemPickup.dwMoney	= (DWORD)nMoney;
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_KARZ,0xff,1,1,(DWORD)nMoney,0);
						}
					}
				}
				else if(m_bQuantityType==__QUANTITY_BNAKMONEYINSERT__)
				{
					if(nMoney>0)
					{
						ItemPickup.bInv		= 32;
						ItemPickup.dwMoney	= (DWORD)nMoney;
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_BANK_KARZ,0xff,1,1,(DWORD)nMoney,0);
						}
					}
				}
				else if(m_bQuantityType==__QUANTITY_THROW__)
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TILE,m_byZipCode,1,(BYTE)nMoney,0);
						m_bReq = TRUE;
					}
					else
					{
						if(nMoney==0)
						{
//							LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

							BYTE byMaxCnt = 0;

							if(lpItemTable)
								byMaxCnt = lpItemTable->GetMaxQuantity();

							for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
								else
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
										{
											ItemPickup.bInv		= 14;
											ItemPickup.bZipCode	= i;
											bChk				= TRUE;
											break;
										}
									}
								}
							}
							
							if(bChk==FALSE)
								ItemPickup.bInv	= 4;										
						}
						else
						{
							if(g_pMainPlayer->GetStatus()==UNIT_STATUS_DEAD)
							{
								SetActive(FALSE);
								return CInterface::GetInstance()->GetSetWnd();
							}					

							ItemPickup.fItemX	= m_fItemX;
							ItemPickup.fItemY	= m_fItemY;
							ItemPickup.fItemZ	= m_fItemZ;
							ItemPickup.bInv		= 28;
							ItemPickup.dwMoney	= (DWORD)nMoney;
						}

						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						m_bReq = TRUE;
					}
				}
				else if(m_bQuantityType==__QUANTITY_STORE__)
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADERMONSTER_INV_SMALL,m_byZipCode,1,(BYTE)nMoney);
						m_bReq = TRUE;
					}
					else
					{
						if(nMoney==0)
						{
//							LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

							BYTE byMaxCnt = 0;

							if(lpItemTable)
								byMaxCnt = lpItemTable->GetMaxQuantity();

							for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
								else
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
										{
											ItemPickup.bInv		= 14;
											ItemPickup.bZipCode	= i;
											bChk				= TRUE;
											break;
										}
									}															
								}
							}
							
							if(bChk==FALSE)
								ItemPickup.bInv	= 4;										
						}
						else
						{
							ItemPickup.bInv		= 18;
							ItemPickup.bZipCode	= 0;
							ItemPickup.dwMoney	= (DWORD)nMoney;
						}								
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						m_bReq = TRUE;
					}					
				}
				else if(m_bQuantityType==__QUANTITY_BUY__)
				{
					CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();
					
					if(nMoney>0)
					{
						ItemPickup.bInv			= 19;
						ItemPickup.dwMoney		= (DWORD)nMoney;
						ItemPickup.dwId			= pStoreWnd->m_wSuppliesId;
						ItemPickup.bZipCode		= m_byZipCode;
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL,m_byZipCode,1,(BYTE)nMoney);				
						}
					}				
				}
				else if(m_bQuantityType==__QUANTITY_BANKINSERT__)
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_BANK_SMALL,m_byZipCode,1,(BYTE)nMoney);
						m_bReq = TRUE;
					}
					else
					{
						if(nMoney==0)
						{
//							LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

							BYTE byMaxCnt = 0;

							if(lpItemTable)
								byMaxCnt = lpItemTable->GetMaxQuantity();

							for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
								else
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
										{
											ItemPickup.bInv		= 14;
											ItemPickup.bZipCode	= i;
											bChk				= TRUE;
											break;
										}
									}
								}
							}
							
							if(bChk==FALSE)
								ItemPickup.bInv	= 4;										
						}
						else
						{
							ItemPickup.bInv		= 25;
							ItemPickup.dwMoney	= (DWORD)nMoney;
							ItemPickup.bZipCode	= m_byZipCode;
						}	
						
						g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						m_bReq = TRUE;
					}					
				}
				else if(m_bQuantityType==__QUANTITY_FROMBANKTOINV__)
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL,m_byZipCode,1,(BYTE)nMoney);
					}
				}
				else if(m_bQuantityType==__QUANTITY_DUNGEONMANAGEINSERT__)
				{
					if(nMoney==0)
					{
//						LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
						CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

						BYTE byMaxCnt = 0;

						if(lpItemTable)
							byMaxCnt = lpItemTable->GetMaxQuantity();

						for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
							{
								ItemPickup.bInv		= 14;
								ItemPickup.bZipCode	= i;
								bChk				= TRUE;
								break;
							}
							else
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
								{
									// 아이템에서 있는 칸을 찾는다 //
									if(g_pMainPlayer->m_pInv_Small[i].m_Item_Zodiac.bQuantity<byMaxCnt)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= i;
										bChk				= TRUE;
										break;
									}
								}
							}
						}
						
						if(bChk==FALSE)
							ItemPickup.bInv	= 4;										
					}
					else
					{
						ItemPickup.dwMoney	= (DWORD)nMoney;					
						SetItemPacket(&ItemPickup, 63, 0, 0, 0, 0);					
					}

					g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
					m_bReq = TRUE;
				}			
				else if(m_bQuantityType==__QUANTITY_TRADE__)
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADE,m_byZipCode,1,(BYTE)nMoney);
						m_bReq = TRUE;
					}
					else
					{
						if(nMoney==0)
						{
//							LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
							CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

							BYTE byMaxCnt = 0;

							if(lpItemTable)
								byMaxCnt = lpItemTable->GetMaxQuantity();

							for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
							{
								if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
								else
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
									{
										// 아이템에서 있는 칸을 찾는다 //
										if(g_pMainPlayer->m_pInv_Small[i].m_Item_Zodiac.bQuantity<byMaxCnt)
										{
											ItemPickup.bInv		= 14;
											ItemPickup.bZipCode	= i;
											bChk				= TRUE;
											break;
										}
									}
								}
							}
							
							if(bChk==FALSE)
								ItemPickup.bInv	= 4;										
						}
						else
						{
							ItemPickup.dwMoney	= (DWORD)nMoney;					
							SetItemPacket(&ItemPickup, 47, m_byZipCode, 0, 0, 0);					
						}
						g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
						m_bReq = TRUE;
					}
				}
				else if(m_bQuantityType==__QUANTITY_DUNGEON__)
				{
					m_byQuantityAct = 1;
					if (nMoney != g_pThisDungeon->m_dwEntrance)
					{
						CTDS_ENTRANCE_EDIT packet;
						packet.wDungeonID = (WORD)g_pThisDungeon->m_dwID;
						packet.dwMoney	 = DWORD(nMoney);

						g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
					}					
				}
				
				if(nMoney!=0)
					m_byQuantityAct = 1;

				// 주민등록뒷자리 입력 모드일때 7자 미만이 입력되었으면 창을 닫지 말자 
				if(	m_bQuantityType == __QUANTITY_JUMIN_NUMBER__ &&
					g_pInputManager->GetInputBufferLength(INPUT_BUFFER_CHARACTER_JUMIN) < 7)
				{
					return 0;
				}

				m_bIsOkButton = 1;
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{							
				SetRender(SPR_OBJ_NAME_TRUE1, FALSE);
				SetRender(SPR_OBJ_NAME_TRUE2, FALSE);
				SetRender(SPR_OBJ_NAME_TRUE3, TRUE);	
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_NAME_TRUE1, FALSE);
				SetRender(SPR_OBJ_NAME_TRUE2, TRUE);
				SetRender(SPR_OBJ_NAME_TRUE3, FALSE);	
				m_bBtn[0] = TRUE;					
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 5:	
		{
			// 취소
			if(eMouseCheck==MOUSE_UP)
			{				
				memset(m_szMoney, 0, sizeof(m_szMoney));
				m_byDrawIndex	= 0;
				::KillTimer(g_hMainWnd, 1);
				
				DWORD dwBufferIndex;
				if(m_bQuantityType == __QUANTITY_JUMIN_NUMBER__)
					dwBufferIndex = INPUT_BUFFER_CHARACTER_JUMIN;
				else
					dwBufferIndex = INPUT_BUFFER_5;

				g_pInputManager->GetInputBuffer(dwBufferIndex);

				if(m_bQuantityType == __QUANTITY_JUMIN_NUMBER__)
				{
					m_bIsOkButton = 0;					
					SetActive(FALSE);								
				}
				else
				{
					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.Initialize();
					}
					else
					{
						CTDS_ITEM_PICKUP ItemPickup;
						ItemPickup.bSectionNum	= 1;
						ItemPickup.i64ItemID	= 0;

						int		nVal = g_pMainPlayer->m_MouseItem.m_wItemID/ITEM_DISTRIBUTE;
						BOOL	bChk = FALSE;

						if(m_bQuantityType==__QUANTITY_STORE__)
						{								
							if(GetType((WORD)nVal)==ITEM_SMALL)
							{
//								LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
								CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

								BYTE byMaxCnt = 0;

								if(lpItemTable)
									byMaxCnt = lpItemTable->GetMaxQuantity();

								for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= i;
										bChk				= TRUE;
										break;
									}
									else
									{
										if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
										{
											// 아이템에서 있는 칸을 찾는다 //
											if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
											{
												ItemPickup.bInv		= 14;
												ItemPickup.bZipCode	= i;
												bChk				= TRUE;
												break;
											}
										}															
									}
								}
								
								if(bChk==FALSE)
									ItemPickup.bInv	= 4;
								
								g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								m_bReq = TRUE;
							}
						}
						else if(m_bQuantityType==__QUANTITY_THROW__)
						{
							if(GetType((WORD)nVal)==ITEM_SMALL)
							{
//								LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
								CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

								BYTE byMaxCnt = 0;

								if(lpItemTable)
									byMaxCnt = lpItemTable->GetMaxQuantity();

								for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= i;
										bChk				= TRUE;
										break;
									}
									else
									{
										if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
										{
											// 아이템에서 있는 칸을 찾는다 //
											if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
											{
												ItemPickup.bInv		= 14;
												ItemPickup.bZipCode	= i;
												bChk				= TRUE;
												break;
											}
										}
									}
								}
								
								if(bChk==FALSE)
									ItemPickup.bInv	= 4;										
								
								g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								m_bReq = TRUE;
							}
						}			
						else if(m_bQuantityType==__QUANTITY_BANKINSERT__)
						{
							if(GetType((WORD)nVal)==ITEM_SMALL)
							{
//								LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
								CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

								BYTE byMaxCnt = 0;

								if(lpItemTable)
									byMaxCnt = lpItemTable->GetMaxQuantity();

								for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= i;
										bChk				= TRUE;
										break;
									}
									else
									{
										if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
										{
											// 아이템에서 있는 칸을 찾는다 //
											if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
											{
												ItemPickup.bInv		= 14;
												ItemPickup.bZipCode	= i;
												bChk				= TRUE;
												break;
											}
										}															
									}
								}
								
								if(bChk==FALSE)
									ItemPickup.bInv	= 4;
								
								g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								m_bReq = TRUE;
							}
						}
						else if(m_bQuantityType==__QUANTITY_FROMBANKTOINV__)
						{

						}
						else if(m_bQuantityType==__QUANTITY_DUNGEONMANAGEINSERT__)
						{
							if(nVal==ITEM_DIVIDE_ID_ZODIAC)
							{
//								LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
								CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

								BYTE byMaxCnt = 0;

								if(lpItemTable)
									byMaxCnt = lpItemTable->GetMaxQuantity();

								for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
									{
										ItemPickup.bInv		= 14;
										ItemPickup.bZipCode	= i;
										bChk				= TRUE;
										break;
									}
									else
									{
										if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
										{
											// 아이템에서 있는 칸을 찾는다 //
											if(g_pMainPlayer->m_pInv_Small[i].m_Item_Zodiac.bQuantity<byMaxCnt)
											{
												ItemPickup.bInv		= 14;
												ItemPickup.bZipCode	= i;
												bChk				= TRUE;
												break;
											}
										}															
									}
								}
								
								if(bChk==FALSE)
									ItemPickup.bInv	= 4;
								
								g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
								m_bReq = TRUE;
							}
						}
					}
					
					m_bIsOkButton = 0;			
					SetActive(FALSE);			
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_NAME_FALSE1, FALSE);
				SetRender(SPR_OBJ_NAME_FALSE2, FALSE);
				SetRender(SPR_OBJ_NAME_FALSE3, TRUE);						
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_NAME_FALSE1, FALSE);
				SetRender(SPR_OBJ_NAME_FALSE2, TRUE);
				SetRender(SPR_OBJ_NAME_FALSE3, FALSE);	
				m_bBtn[1] = TRUE;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;	
	}	
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_QUANTITY_CLOSE1, TRUE);
		SetRender(SPR_OBJ_QUANTITY_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{
		if(m_bQuantityType == __QUANTITY_JUMIN_NUMBER__)
		{
			g_pInputManager->GetInputRect(INPUT_BUFFER_CHARACTER_JUMIN).top		= (LONG)m_fPosZ+93;
			g_pInputManager->GetInputRect(INPUT_BUFFER_CHARACTER_JUMIN).bottom	= (LONG)m_fPosZ+105;
			g_pInputManager->GetInputRect(INPUT_BUFFER_CHARACTER_JUMIN).left	= (LONG)m_fPosX+20;
			g_pInputManager->GetInputRect(INPUT_BUFFER_CHARACTER_JUMIN).right	= (LONG)m_fPosX+70;
		}
		else
		{
			g_pInputManager->GetInputRect(INPUT_BUFFER_5).top		= (LONG)m_fPosZ+83;
			g_pInputManager->GetInputRect(INPUT_BUFFER_5).bottom	= (LONG)m_fPosZ+95;
			g_pInputManager->GetInputRect(INPUT_BUFFER_5).left		= (LONG)m_fPosX+124-(m_byDrawIndex*7)-10;
			g_pInputManager->GetInputRect(INPUT_BUFFER_5).right		= (LONG)m_fPosX+124;
		}

		m_rtPos[0].left		= (LONG)m_fPosX+5;
		m_rtPos[0].right	= (LONG)m_fPosX+128;
		m_rtPos[0].top		= (LONG)m_fPosZ+33;
		m_rtPos[0].bottom	= (LONG)m_fPosZ+45;
		
		m_rtPos[1].left		= (LONG)m_fPosX+5;
		m_rtPos[1].right	= (LONG)m_fPosX+128;
		m_rtPos[1].top		= (LONG)m_fPosZ+50;
		m_rtPos[1].bottom	= (LONG)m_fPosZ+62;
	
		m_rtPos[2].left		= (LONG)m_fPosX+5;
		m_rtPos[2].right	= (LONG)m_fPosX+128;
		m_rtPos[2].top		= (LONG)m_fPosZ+67;
		m_rtPos[2].bottom	= (LONG)m_fPosZ+79;

		return CInterface::GetInstance()->GetSetWnd();
	}

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}


void CQuantityWnd::QuantityMsg()
{	
	CTDS_ITEM_PICKUP ItemPickup;
	ItemPickup.bSectionNum	= 1;
	ItemPickup.i64ItemID	= 0;

	BOOL	bChk	= FALSE;
	int		nMoney	= atoi(m_szMoney);

	if(nMoney > 0)
	{
		if(m_bQuantityType == __QUANTITY_DUNGEONMANAGEINSERT__)
		{
			if(nMoney == 0)
			{
				for(int i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
//					LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

					BYTE byMaxCnt = 0;

					if(lpItemTable)
						byMaxCnt = lpItemTable->GetMaxQuantity();

					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
					{
						ItemPickup.bInv		= 14;
						ItemPickup.bZipCode	= (BYTE)i;
						bChk				= TRUE;
						break;
					}
					else
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
						{
							// 아이템에서 있는 칸을 찾는다 //
							if(g_pMainPlayer->m_pInv_Small[i].m_Item_Zodiac.bQuantity<byMaxCnt)
							{
								ItemPickup.bInv		= 14;
								ItemPickup.bZipCode	= (BYTE)i;
								bChk				= TRUE;
								break;
							}
						}
					}
				}
				
				if(bChk==FALSE)
					ItemPickup.bInv	= 4;										
			}
			else
			{
				ItemPickup.dwMoney	= (DWORD)nMoney;					
				SetItemPacket(&ItemPickup, 63, 0, 0, 0, 0);					
			}

			g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
			m_bReq = TRUE;
		}
		else if(m_bQuantityType==__QUANTITY_MONEY__)
		{
// 넷클루 요청으로 Karz 땅에 떨어뜨지지 못하게 2005.02.05 김영대 
#if IS_JAPAN_LOCALIZING()
			DisplayMessageAdd(g_Message[ETC_MESSAGE1372].szMessage, 0xffff0000);
#else
			ItemPickup.bInv		= 13;
			ItemPickup.dwMoney	= nMoney;
			g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
#endif;
			
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TILE,0,1,1,nMoney);
			}
		}
		else if(m_bQuantityType==__QUANTITY_PLAYERSHOP__)
		{
			if(nMoney>0)
			{
				int nValue = 0;

				if(g_ItemMoveManager.GetNewItemMoveMode())
					nValue = g_ItemMoveManager.GetMouseItemID() / ITEM_DISTRIBUTE;
				else
					nValue = g_pMainPlayer->m_MouseItem.GetID() / ITEM_DISTRIBUTE;
				
				if(nValue<=20)
				{
					ItemPickup.bInv		= 57;
					ItemPickup.bZipCode	= m_byZipCode;
					ItemPickup.dwMoney	= nMoney;
					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_PLAYERSHOP,m_byZipCode,1,1,nMoney);

					m_bReq = TRUE;
				}
				else
				{	
					RECT rt;
					rt.left		= (LONG)m_fPosX+124-7;
					rt.right	= (LONG)m_fPosX+124;
					rt.top		= (LONG)m_fPosZ+83;
					rt.bottom	= (LONG)m_fPosZ+95;
					int nOrder	= GetStartOrder()+2;

					OpenWnd(__QUANTITY_PLAYERSHOPCNT__);

					g_pInputManager->InitializeInput(INPUT_BUFFER_5, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
					g_pInputManager->SetFocusInput(INPUT_BUFFER_5);

					CInterface::GetInstance()->SetWnd(QUANTITY_WND);

					memset(m_szMoney, 0, sizeof(m_szMoney));
					m_byDrawIndex	= 0;

					// 코드, 가격 //
					CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();
					pPlayerShopWnd->m_dwMoney = nMoney;
					return;
				}
				
				if(nMoney!=0)
				{
					m_byQuantityAct = 1;
				}
			}
		}
		else if(m_bQuantityType==__QUANTITY_PLAYERSHOPCNT__)
		{
			if(nMoney>0)
			{
				int nValue = 0;

				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					nValue = g_ItemMoveManager.GetMouseItemID() / ITEM_DISTRIBUTE;
				}
				else
				{
					nValue = g_pMainPlayer->m_MouseItem.GetID()/ITEM_DISTRIBUTE;
				}

				if(nValue>20)
				{	
					CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

					ItemPickup.bInv		= 59;
					ItemPickup.dwMoney	= nMoney;
					ItemPickup.dwId		= pPlayerShopWnd->m_dwMoney;
					ItemPickup.bZipCode	= m_byZipCode;
					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

					if(g_ItemMoveManager.GetNewItemMoveMode())
					{
						g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_PLAYERSHOP
							,m_byZipCode,1,(BYTE)nMoney,pPlayerShopWnd->m_dwMoney);
					}

					m_bReq = TRUE;
				}
				
				if(nMoney!=0)
				{
					m_byQuantityAct = 1;
				}
			}
		}
		else if(m_bQuantityType==__QUANTITY_PLAYERSHOPSELLCNT__)
		{
			if(nMoney>0)
			{										
				CPlayerShopWnd* pPlayerShopWnd = CPlayerShopWnd::GetInstance();

				ItemPickup.bInv			= 73;				
				ItemPickup.bZipCode		= pPlayerShopWnd->m_bZipCode;					
				ItemPickup.dwMoney		= pPlayerShopWnd->m_pOwnerPlayerShop->m_dwUserIndex;
				ItemPickup.dwId			= CStoreWnd::GetInstance()->m_wSuppliesId;
				ItemPickup.bZipCode2	= (BYTE)nMoney;
				ItemPickup.dwShop		= pPlayerShopWnd->m_sPlayerShopInfo.m_dwCustomSellPrice[pPlayerShopWnd->m_bZipCode];
				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				m_bReq = TRUE;

				if(nMoney!=0)
				{
					m_byQuantityAct = 1;
				}	
				
				if(g_ItemMoveManager.GetNewItemMoveMode())
				{
					g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL
						, m_byZipCode
						, 1
						, (BYTE)nMoney
						, pPlayerShopWnd->m_sPlayerShopInfo.m_dwCustomSellPrice[pPlayerShopWnd->m_bZipCode]
						, CStoreWnd::GetInstance()->m_wSuppliesId);
				}
			}
		}
		else if(m_bQuantityType==__QUANTITY_BNAKMONEYPOP__)
		{
			ItemPickup.bInv		= 33;
			ItemPickup.dwMoney	= nMoney;
			g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_KARZ,0xff,1,1,nMoney,0);
			}
		}
		else if(m_bQuantityType==__QUANTITY_TRADEMONEY__)
		{
			if(nMoney>0)
			{
				CItemTradeShopWnd* pItemTradeShopWnd = CItemTradeShopWnd::GetInstance();

				if(!pItemTradeShopWnd->m_bOkChk)
				{
					pItemTradeShopWnd->m_dwMoney = nMoney;

					if(nMoney!=0)
					{
						m_byQuantityAct = 1;
					}
				}
				else
				{
					// "교환 확인을 하셨습니다. Karz를 교환 하실수 없습니다."
					DisplayMessageAdd(g_Message[ETC_MESSAGE1255].szMessage, 0xffff0000);
					SetActive(FALSE);
					return;
				}				
			}
		}
		else if(m_bQuantityType==__QUANTITY_BNAKMONEYINSERT__)
		{
			ItemPickup.bInv		= 32;
			ItemPickup.dwMoney	= nMoney;
			g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);

			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_BANK_KARZ,0xff,1,1,nMoney,0);
			}
		}
		else if(m_bQuantityType==__QUANTITY_THROW__)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TILE,m_byZipCode,1,(BYTE)nMoney,0);
				m_bReq = TRUE;
			}
			else
			{			
				if(nMoney==0)
				{
//					LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

					BYTE byMaxCnt = 0;

					if(lpItemTable)
						byMaxCnt = lpItemTable->GetMaxQuantity();

					for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
						{
							ItemPickup.bInv		= 14;
							ItemPickup.bZipCode	= i;
							bChk				= TRUE;
							break;
						}
						else
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
							{
								// 아이템에서 있는 칸을 찾는다 //
								if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
							}
						}
					}
					
					if(bChk==FALSE)
					{
						ItemPickup.bInv	= 4;										
					}
				}
				else
				{
					if(g_pMainPlayer->GetStatus()==UNIT_STATUS_DEAD)
					{
						SetActive(FALSE);
						return;
					}					
					ItemPickup.fItemX	= m_fItemX;
					ItemPickup.fItemY	= m_fItemY;
					ItemPickup.fItemZ	= m_fItemZ;
					ItemPickup.bInv		= 28;
					ItemPickup.dwMoney	= (DWORD)nMoney;
				}

				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				m_bReq = TRUE;
			}
		}		
		else if(m_bQuantityType==__QUANTITY_BUY__)
		{
			CStoreWnd* pStoreWnd = CStoreWnd::GetInstance();
	
			ItemPickup.bInv			= 19;
			ItemPickup.dwMoney		= nMoney;
			ItemPickup.dwId			= pStoreWnd->m_wSuppliesId;
			ItemPickup.bZipCode		= m_byZipCode;
			g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
			
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL,m_byZipCode,1,(BYTE)nMoney);				
			}
		}
		else if(m_bQuantityType==__QUANTITY_STORE__)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADERMONSTER_INV_SMALL,m_byZipCode,1,(BYTE)nMoney);
				m_bReq = TRUE;
			}
			else
			{			
				if(nMoney==0)
				{
//					LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

					BYTE byMaxCnt = 0;

					if(lpItemTable)
						byMaxCnt = lpItemTable->GetMaxQuantity();

					for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
						{
							ItemPickup.bInv		= 14;
							ItemPickup.bZipCode	= i;
							bChk				= TRUE;
							break;
						}
						else
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
							{
								// 아이템에서 있는 칸을 찾는다 //
								if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
							}															
						}
					}
					
					if(bChk==FALSE)
						ItemPickup.bInv	= 4;										
				}
				else
				{
					ItemPickup.bInv		= 18;
					ItemPickup.bZipCode	= 0;
					ItemPickup.dwMoney	= nMoney;
				}
				
				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				m_bReq = TRUE;
			}
		}
		else if(m_bQuantityType==__QUANTITY_BANKINSERT__)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_BANK_SMALL,m_byZipCode,1,(BYTE)nMoney);
				m_bReq = TRUE;
			}
			else
			{
				if(nMoney==0)
				{
//					LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

					BYTE byMaxCnt = 0;

					if(lpItemTable)
						byMaxCnt = lpItemTable->GetMaxQuantity();

					for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
						{
							ItemPickup.bInv		= 14;
							ItemPickup.bZipCode	= i;
							bChk				= TRUE;
							break;
						}
						else
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
							{
								// 아이템에서 있는 칸을 찾는다 //
								if(g_pMainPlayer->m_pInv_Small[i].m_Item_Supplies.bQuantity<byMaxCnt)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
							}
						}
					}
					
					if(bChk==FALSE)
						ItemPickup.bInv	= 4;										
				}
				else
				{
					ItemPickup.bInv		= 25;
					ItemPickup.dwMoney	= nMoney;
					ItemPickup.bZipCode	= m_byZipCode;
				}
				
				g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				m_bReq = TRUE;	
			}			
		}
		else if(m_bQuantityType==__QUANTITY_FROMBANKTOINV__)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_INV_SMALL,m_byZipCode,1,(BYTE)nMoney);
			}
		}
		else if(m_bQuantityType==__QUANTITY_TRADE__)
		{
			if(g_ItemMoveManager.GetNewItemMoveMode())
			{
				g_ItemMoveManager.SetLButtonUpItem(ITEM_NATIVE_TRADE,m_byZipCode,1,(BYTE)nMoney);
				m_bReq = TRUE;
			}
			else
			{			
				if(nMoney==0)
				{
//					LP_ITEM_TABLE lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);
					CBaseItem* lpItemTable = g_pItemTableHash->GetData(g_pMainPlayer->m_MouseItem.m_wItemID);

					BYTE byMaxCnt = 0;

					if(lpItemTable)
						byMaxCnt = lpItemTable->GetMaxQuantity();

					for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
					{
						if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==0)
						{
							ItemPickup.bInv		= 14;
							ItemPickup.bZipCode	= i;
							bChk				= TRUE;
							break;
						}
						else
						{
							if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==g_pMainPlayer->m_MouseItem.m_wItemID)
							{
								// 아이템에서 있는 칸을 찾는다 //
								if(g_pMainPlayer->m_pInv_Small[i].m_Item_Zodiac.bQuantity<byMaxCnt)
								{
									ItemPickup.bInv		= 14;
									ItemPickup.bZipCode	= i;
									bChk				= TRUE;
									break;
								}
							}
						}
					}
					
					if(bChk==FALSE)
					{
						ItemPickup.bInv	= 4;										
					}
				}
				else
				{
					ItemPickup.dwMoney	= nMoney;					
					SetItemPacket(&ItemPickup, 47, m_byZipCode, 0, 0, 0);					
				}

				g_pNet->SendMsg((char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				m_bReq = TRUE;
			}
		}					
		else if(m_bQuantityType==__QUANTITY_DUNGEON__)
		{
			m_byQuantityAct = 1;
			if (DWORD(nMoney) != g_pThisDungeon->m_dwEntrance)
			{
				CTDS_ENTRANCE_EDIT packet;
				packet.wDungeonID = (WORD)g_pThisDungeon->m_dwID;
				packet.dwMoney	 = nMoney;

				g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
			}
		}
	}
	else if(nMoney <= 0)
	{
		if(g_ItemMoveManager.GetNewItemMoveMode())
		{
			g_ItemMoveManager.Initialize();

			CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
			pQuantityWnd->m_bReq = FALSE;
		}
	}

	memset(m_szMoney, 0, sizeof(m_szMoney));
	m_byDrawIndex	= 0;
	SetActive(FALSE);

	if(g_ItemMoveManager.GetNewItemMoveMode())
		g_ItemMoveManager.Initialize();
}
//==========================================================//
// End.														//
//==========================================================//