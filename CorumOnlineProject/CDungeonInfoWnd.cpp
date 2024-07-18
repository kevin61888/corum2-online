#include "stdafx.h"
#include "CDungeonManagerWnd.h"
#include "CDungeonInfoWnd.h"
#include "GameControl.h"
#include "User.h"
#include "DSMiniMap.h"
#include "dungeontable.h"
#include "message.h"
#include "HashTable.h"
#include "GlobalSoundHandles.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "Interface.h"


#define	MAX_DISPLAY	10


CDungeonInfoWnd* CDungeonInfoWnd::c_pThis = NULL;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDungeonInfoWnd::CDungeonInfoWnd()
{	
	m_bPointer				= FALSE;
	m_bDownChk				= FALSE;					
	m_bTabIndex				= 0;
	m_bCurPage				= 0;
	m_pDisplayHeadPosition	= NULL;
	m_pSelectDungeon		= NULL;
	m_pPageList				= new CVoidList;
}

CDungeonInfoWnd::~CDungeonInfoWnd()
{
	if (m_pPageList)
	{
		m_pPageList->RemoveAll();
		delete m_pPageList;
	}
	m_pPageList = NULL;
}

BOOL CDungeonInfoWnd::Init()
{
	/*
	InsertData(SPR_OBJ_DUNGEONINFOEX_WND1, SPR_INTERFACE_DUNGEONINFOEX_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_DUNGEONINFOEX_WND2, SPR_INTERFACE_DUNGEONINFOEX_WND2, 0, 32, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_DUNGEONINFOEX_WND3, SPR_INTERFACE_DUNGEONINFOEX_WND3, 0, 48, 1.0f, 25.0f, 0);
	InsertData(SPR_OBJ_DUNGEONINFOEX_WND4, SPR_INTERFACE_DUNGEONINFOEX_WND4, 0, 248, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_DUNGEONINFO_CLOSE1, SPR_INTERFACE_CLOSE1, 406, 4, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_DUNGEONINFO_CLOSE2, SPR_INTERFACE_CLOSE2, 406, 4, 1.0f, 1.0f, 2);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_STATUS1, SPR_INTERFACE_DUNGEONINFOEX_STATUS1, 258, 3, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_STATUS2, SPR_INTERFACE_DUNGEONINFOEX_STATUS2, 258, 3, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_STATUS3, SPR_INTERFACE_DUNGEONINFOEX_STATUS3, 258, 3, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_INFORMATION1, SPR_INTERFACE_DUNGEONINFOEX_INFORMATION1, 305, 3, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_INFORMATION2, SPR_INTERFACE_DUNGEONINFOEX_INFORMATION2, 305, 3, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_INFORMATION3, SPR_INTERFACE_DUNGEONINFOEX_INFORMATION3, 305, 3, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PRODUCTION1, SPR_INTERFACE_DUNGEONINFOEX_PRODUCTION1, 352, 3, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PRODUCTION2, SPR_INTERFACE_DUNGEONINFOEX_PRODUCTION2, 352, 3, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PRODUCTION3, SPR_INTERFACE_DUNGEONINFOEX_PRODUCTION3, 352, 3, 1.0f, 1.0f, 1);	
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT1, SPR_INTERFACE_DUNGEONINFOEX_MANAGEMENT1, 234, 230, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT2, SPR_INTERFACE_DUNGEONINFOEX_MANAGEMENT2, 234, 230, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT3, SPR_INTERFACE_DUNGEONINFOEX_MANAGEMENT3, 234, 230, 1.0f, 1.0f, 1);		
	InsertData(SPR_OBJ_DUNGEONINFOEX_SELECTBAR, SPR_INTERFACE_DUNGEONINFOEX_SELECTBAR, 2, 49, 208.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PAGENEXT1, SPR_INTERFACE_PAGENEXT1, 382, 230, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PAGENEXT2, SPR_INTERFACE_PAGENEXT2, 382, 230, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PAGENEXT3, SPR_INTERFACE_PAGENEXT3, 382, 230, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PAGEPRE1, SPR_INTERFACE_PAGEPRE1, 300, 230, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PAGEPRE2, SPR_INTERFACE_PAGEPRE2, 300, 230, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_DUNGEONINFOEX_PAGEPRE3, SPR_INTERFACE_PAGEPRE3, 300, 230, 1.0f, 1.0f, 1);		
	InsertData(SPR_OBJ_DUNGEONINFOEX_PAGEBOX, SPR_INTERFACE_PAGEBOX, 317, 228, 1.0f, 1.0f, 1);
		
	InsertCheckInterface(403, 4, 415, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 403, 20, 9, CHECK_MOVEWND);
	InsertCheckInterface(258, 3, 302, 19, 3, CHECK_BTN);	
	InsertCheckInterface(305, 3, 349, 19, 4, CHECK_BTN);
	InsertCheckInterface(352, 3, 396, 19, 5, CHECK_BTN);		
	InsertCheckInterface(234, 230, 300, 246, 6, CHECK_BTN);
	InsertCheckInterface(300, 230, 316, 246, 7, CHECK_BTN);
	InsertCheckInterface(382, 230, 398, 246, 8, CHECK_BTN);
	*/
	m_bInit = TRUE;	
	
	return TRUE;
}

void CDungeonInfoWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
	
}
void CDungeonInfoWnd::ResetResource()
{
	m_bCurPage				= 0;
	m_pDisplayHeadPosition	= NULL;
	m_pSelectDungeon		= NULL;
	m_pPageList->RemoveAll();
}
void CDungeonInfoWnd::SetOrder()
{
}

void CDungeonInfoWnd::RenderText()
{			
	char szInfo[0xff] = {0,};
	int nLeft = 0, nRight = 0;

	switch(m_bTabIndex)	
	{
	case 0:
		{
//#if IS_JAPAN_LOCALIZING()
			nLeft	= (int)m_fPosX+7;
//#else
//			nLeft	= (int)m_fPosX+31;
//#endif
			nRight	= nLeft+100;
			
			// "던전이름"		
			RenderFont(g_Message[ETC_MESSAGE463].szMessage, nLeft, nRight, (int)m_fPosZ+34, (int)m_fPosZ+46, GetStartOrder()+2);

//#if IS_JAPAN_LOCALIZING()
			nLeft	= (int)m_fPosX+116;
//#else
//			nLeft	= (int)m_fPosX+140;
//#endif
			nRight	= nLeft+100;
						
			// "관리자"
			RenderFont(g_Message[ETC_MESSAGE162].szMessage
				, nLeft, nRight, (int)m_fPosZ+34, (int)m_fPosZ+46, GetStartOrder()+2);
									
			// "생산물"			
			RenderFont(g_Message[ETC_MESSAGE476].szMessage, (int)m_fPosX+228, (int)m_fPosX+328, (int)m_fPosZ+34, (int)m_fPosZ+46, GetStartOrder()+2);

			// "오픈시간"
			RenderFont(g_Message[ETC_MESSAGE464].szMessage
				, m_fPosX+358, m_fPosX+458, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);			

			// "던전상태"			
//			RenderFont(g_Message[ETC_MESSAGE465].szMessage
//				, m_fPosX+296, m_fPosX+396, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);

			// "운영방침"
//			RenderFont(g_Message[ETC_MESSAGE466].szMessage
//				, m_fPosX+348, m_fPosX+448, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);			

			int nLine = 0;
			
			POSITION_ pos = m_pPageList->GetAt(m_pDisplayHeadPosition);

			while(pos)
			{
				DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);				
				
				RenderFont(pDungeon->m_szDungeonName
					, m_fPosX+7, m_fPosX+107, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);
				RenderFont(pDungeon->m_szOwner
					, m_fPosX+116, m_fPosX+216, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);

				CBaseItem* pItem = g_pItemTableHash->GetData( pDungeon->m_wProduction );

				memset(szInfo, 0, sizeof(szInfo));

				// MSG_ID : 399 ; None
				RenderFont((!pItem) ? g_Message[ETC_MESSAGE399].szMessage : pItem->szItemName_Kor, 
					m_fPosX+228, m_fPosX+350, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);

								
				if(pDungeon->IsConquer()) // modified by deepdark!
				{
					int nHour = 0;
					int nMinute = 0;
					int nSec = 0;

					if (pDungeon->m_bSiege)
					{						
						nHour	= pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_HOUR);
						nMinute = pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_MIN);
						nSec	= pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeEndTick, g_dwCurTick, TIME_SEC);				
					}
					else
					{
						nHour	= pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeStartTick, g_dwCurTick, TIME_HOUR);
						nMinute = pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeStartTick, g_dwCurTick, TIME_MIN);
						nSec	= pDungeon->GetRemainTime(pDungeon->m_dwRemainSiegeStartTick, g_dwCurTick, TIME_SEC);
					}

					wsprintf(szInfo, "%02d:%02d:%02d", nHour, nMinute, nSec);
				}
				else
				{
					lstrcpy(szInfo, "--:--:--");
				}

				RenderFont(szInfo
					, m_fPosX+358, m_fPosX+500, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);								

				// "오픈", "평화"
//				RenderFont(g_Message[(pDungeon->m_bSiege) ? ETC_MESSAGE467 : ETC_MESSAGE468].szMessage, 
//					m_fPosX+296, m_fPosX+396, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);
				
//				DWORD dwMessageId = 0;
//				switch(pDungeon->m_bOperationType)
//				{
//				case DUNGEON_OPERATIONTYPE_PRECOCIOUS:
//					dwMessageId = ETC_MESSAGE1180; // "숙성중"
//					break;
//				case DUNGEON_OPERATIONTYPE_PRODUCTION:
//					dwMessageId = ETC_MESSAGE1181; // "산출중"
//					break;
//				case DUNGEON_OPERATIONTYPE_CONVERSION:
//					dwMessageId = ETC_MESSAGE1182; // "변환중"
//					break;
//				}
				
//				RenderFont(g_Message[dwMessageId].szMessage
//					, m_fPosX+348, m_fPosX+448, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);
				
				if(++nLine == MAX_DISPLAY)
					break;
			}			
		}
		break;

	case 1:
		{
#if IS_JAPAN_LOCALIZING()
			nLeft	= (int)m_fPosX+7;
#else
			nLeft	= (int)m_fPosX+31;
#endif
			nRight	= nLeft+100;

			// "던전이름"
			RenderFont(g_Message[ETC_MESSAGE463].szMessage
				, nLeft, nRight, (int)m_fPosZ+34, (int)m_fPosZ+46, GetStartOrder()+2);

			// "레벨"
			RenderFont(g_Message[ETC_MESSAGE472].szMessage
				, m_fPosX+113, m_fPosX+213, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);

			// "남은시간"
			RenderFont(g_Message[ETC_MESSAGE473].szMessage
				, m_fPosX+145, m_fPosX+245, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);

			// "마법진"
			RenderFont(g_Message[ETC_MESSAGE474].szMessage
				, m_fPosX+229, m_fPosX+329, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);

			// "가디언"
			RenderFont(g_Message[ETC_MESSAGE263].szMessage
				, m_fPosX+317, m_fPosX+417, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);

			int nLine = 0;
			POSITION_ pos = m_pPageList->GetAt(m_pDisplayHeadPosition);

			while(pos)
			{
				DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);
												
				RenderFont(pDungeon->m_szDungeonName
					, m_fPosX+7, m_fPosX+107, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);
												
				wsprintf(szInfo, "%d", pDungeon->GetLevel());
				RenderFont(szInfo
					, m_fPosX+117, m_fPosX+217, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);				
				
				if(pDungeon->IsConquer())
				{
					int nHour;
					int nMinute;
					int nSec;
				
					if (pDungeon->m_bSiege)
					{
						nHour = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, pDungeon->m_dwStopStartTick, TIME_HOUR);
						nMinute = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, pDungeon->m_dwStopStartTick, TIME_MIN);
						nSec = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, pDungeon->m_dwStopStartTick, TIME_SEC);
					}
					else
					{
						nHour = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, g_dwCurTick, TIME_HOUR);
						nMinute = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, g_dwCurTick, TIME_MIN);
						nSec = pDungeon->GetRemainTime(pDungeon->m_dwRemainLevelStartTick, g_dwCurTick, TIME_SEC);
					}
					
					wsprintf(szInfo, "%02d:%02d:%02d", nHour, nMinute, nSec);				
				}
				else
				{
					wsprintf(szInfo, "--:--:--");
				}

				RenderFont(szInfo
					, m_fPosX+145, m_fPosX+245, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);
			
//				LP_ITEM_TABLE pItem = g_pItemTableHash->GetData( pDungeon->m_cMagicFieldArrayItem.GetID() );
				CBaseItem* pItem = g_pItemTableHash->GetData( pDungeon->m_cMagicFieldArrayItem.GetID() );

				memset(szInfo, 0, sizeof(szInfo));

				RenderFont((!pItem) ? g_Message[ETC_MESSAGE399].szMessage : pItem->szItemName_Kor, 
					m_fPosX+229, m_fPosX+329, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);

				// 가디언! by deepdark.
//				LP_ITEM_TABLE pGuardianItem = g_pItemTableHash->GetData( pDungeon->m_cGuardianItem.GetID() );
				CBaseItem* pGuardianItem = g_pItemTableHash->GetData( pDungeon->m_cGuardianItem.GetID() );
				
				// // "수호가디언"
				RenderFont((!pGuardianItem) ? g_Message[ETC_MESSAGE475].szMessage : pGuardianItem->szItemName_Kor, 
					m_fPosX+317, m_fPosX+417, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);
				
				if(++nLine == MAX_DISPLAY)
					break;
			}
		}break;
	case 2:
		{
#if IS_JAPAN_LOCALIZING()
			nLeft	= (int)m_fPosX+7;
#else
			nLeft	= (int)m_fPosX+31;
#endif
			nRight	= nLeft+100;

			// "던전이름"
			RenderFont(g_Message[ETC_MESSAGE463].szMessage, nLeft, nRight, (int)m_fPosZ+34, (int)m_fPosZ+46, GetStartOrder()+2);
			
#if IS_JAPAN_LOCALIZING()
			nLeft	= (int)m_fPosX+116;
#else
			nLeft	= (int)m_fPosX+140;
#endif
			nRight	= nLeft+100;
			
			// "생산물"			
			RenderFont(g_Message[ETC_MESSAGE476].szMessage, nLeft, nRight, (int)m_fPosZ+34, (int)m_fPosZ+46, GetStartOrder()+2);

			// "남은시간"
			RenderFont(g_Message[ETC_MESSAGE473].szMessage, m_fPosX+228, m_fPosX+328, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);

			// "보관수"			
			RenderFont(g_Message[ETC_MESSAGE477].szMessage, m_fPosX+293, m_fPosX+393, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);

			// "입장수입"
			RenderFont(g_Message[ETC_MESSAGE478].szMessage, m_fPosX+348, m_fPosX+448, m_fPosZ+34, m_fPosZ+46, GetStartOrder()+2);

			int nLine = 0;
			POSITION_ pos = m_pPageList->GetAt(m_pDisplayHeadPosition);
			
			while(pos)
			{
				DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);
								
				RenderFont(pDungeon->m_szDungeonName, m_fPosX+7, m_fPosX+107, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);
							
//				LP_ITEM_TABLE pItem = g_pItemTableHash->GetData( pDungeon->m_wProduction );
				CBaseItem* pItem = g_pItemTableHash->GetData( pDungeon->m_wProduction );

				memset(szInfo, 0, sizeof(szInfo));

				// MSG_ID : 399 ; None
				RenderFont((!pItem) ? g_Message[ETC_MESSAGE399].szMessage : pItem->szItemName_Kor, 
					m_fPosX+116, m_fPosX+216, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);

				RenderFont("--:--:--", m_fPosX+228, m_fPosX+328, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);

				wsprintf(szInfo, "%u", pDungeon->m_dwAccEntrance);
				RenderFont(szInfo, m_fPosX+344, m_fPosX+444, m_fPosZ+52+nLine*17, m_fPosZ+64+nLine*17, GetStartOrder()+2);

				if(++nLine == MAX_DISPLAY)
					break;
			}
		}
		break;
	}	
	wsprintf(szInfo, "%d/%d", m_bCurPage+1, g_pDungeonTable->m_pReceivedDungeonList->GetCount()/MAX_DISPLAY+1);	
	RenderFont(szInfo, m_fPosX+340, m_fPosX+440, m_fPosZ+233, m_fPosZ+245, GetStartOrder()+3);
}

void CDungeonInfoWnd::SetActive(BOOL bActive)
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

		SetRender(SPR_OBJ_DUNGEONINFOEX_WND1, bActive);	
		SetRender(SPR_OBJ_DUNGEONINFOEX_WND2, bActive);	
		SetRender(SPR_OBJ_DUNGEONINFOEX_WND3, bActive);	
		SetRender(SPR_OBJ_DUNGEONINFOEX_WND4, bActive);				
//		SetRender(BUTTON_OBJ_DUNGEONINFOEX_STATUS1, bActive);	
//		SetRender(BUTTON_OBJ_DUNGEONINFOEX_INFORMATION1, bActive);	
//		SetRender(BUTTON_OBJ_DUNGEONINFOEX_PRODUCTION1, bActive);	
		SetRender(BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT1, bActive);	
		SetRender(SPR_OBJ_DUNGEONINFOEX_SELECTBAR, bActive);	
		SetRender(BUTTON_OBJ_DUNGEONINFOEX_PAGENEXT1, bActive);	
		SetRender(BUTTON_OBJ_DUNGEONINFOEX_PAGEPRE1, bActive);	
		SetRender(SPR_OBJ_DUNGEONINFOEX_PAGEBOX, bActive);	
		SetRender(SPR_OBJ_DUNGEONINFO_CLOSE1, bActive);	
		
		m_dwOwnDungeonCount = 1;				
		SelectDungeon(-1);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}	
}

int CDungeonInfoWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(!m_bUpChk)
	{
		// 마우스 오버를 위한 초기화..		
//		for(int i = 0; i < 6; ++i)
//		{
//			if (i == m_bTabIndex)
//				SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+i*3, 2);
//			else
//				SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+i*3, 0);
//		}
		if (m_pSelectDungeon)
		{
			if (m_pSelectDungeon == g_pThisDungeon)
				ShowManageButton(m_pSelectDungeon->IsDungeonOwner(g_pMainPlayer));
			else
				ShowManageButton(FALSE);
		}
		else
		{
			ShowManageButton(FALSE);
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
			{
				SetActive(FALSE);	
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_DUNGEONINFO_CLOSE1, FALSE);
				SetRender(SPR_OBJ_DUNGEONINFO_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 9:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
//	case 3:
//	case 4:
//	case 5:
//		{			
//			if(eMouseCheck==MOUSE_DOWN)
//			{
//				SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(m_bTabIndex)*3, 0);
//				SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3, 2);
//				SetTab(BYTE(nRt-3));
//			}
//			else if(eMouseCheck==MOUSE_OVER)
//			{			
//				if(m_bTabIndex != nRt-3)
//					SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3, 1);				
//			}
//			SetMouseCheck(nRt, eMouseCheck);
//		}
//		break;
	case 6://관리
	case 7://전페이지
	case 8://다음페이지
		{		
			BYTE byChk = 0;
			
			if(eMouseCheck==MOUSE_UP)
			{
				if(nRt==6)
				{
					if(GetRender(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3))
						SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3, 1);
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else
				{
					SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3, 1);
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(nRt==6)
				{
					if(GetRender(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3))
						SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3, 2);
					else
						byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
				else
				{
					SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3, 2);
				}

				if(nRt==8)		// next
				{
					NextPage();
				}
				else if(nRt==7)	// prev
				{					
					PrevPage();
				}
				else if(nRt==6)	// 관리
				{
					if (m_pSelectDungeon && m_pSelectDungeon->IsDungeonOwner(g_pMainPlayer) && g_pThisDungeon == m_pSelectDungeon)
					{
						CDungeonManagerWnd::GetInstance()->SetDungeon(m_pSelectDungeon);
						CDungeonManagerWnd::GetInstance()->SetActive();
					}
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(nRt==6)
				{
					if(GetRender(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3))
						SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3, 1);						
					else
						byChk = __MOUSE_POINTER_DEFAULT__;
				}
				else				
				{
					SetButtonStatus(BUTTON_OBJ_DUNGEONINFOEX_STATUS1+(nRt-3)*3, 1);		
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;	
	}
		
	if(MouseUp())
	{
		SetRender(SPR_OBJ_DUNGEONINFO_CLOSE1, TRUE);
		SetRender(SPR_OBJ_DUNGEONINFO_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(nRt>=1 && eMouseCheck==MOUSE_DOWN)
	{
		if(m_fPosZ+49 < g_Mouse.MousePos.y && g_Mouse.MousePos.y < m_fPosZ+219)
		{
			int nIndex = int((g_Mouse.MousePos.y-m_fPosZ-49)/17);
			SelectDungeon(nIndex);
		}		
	}
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))	
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}


void CDungeonInfoWnd::InsertDungeonInfo(DUNGEON_DATA_EX* pDungeon)
{
	if (g_pDungeonTable->m_pReceivedDungeonList->GetCount() % 10 == 1)
		m_pPageList->AddTail(g_pDungeonTable->m_pReceivedDungeonList->GetTailPosition());
	
	if (!m_pDisplayHeadPosition)
		m_pDisplayHeadPosition = m_pPageList->GetHeadPosition();
}

void CDungeonInfoWnd::SetButtonStatus(DWORD dwSprID, DWORD dwStatus)
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
	case 1:			//mouseover
		{
			SetRender(dwSprID, FALSE);
			SetRender(dwSprID+1, TRUE);
			SetRender(dwSprID+2, FALSE);			
		}
		break;
	case 2:			//mousedown
		{
			SetRender(dwSprID, FALSE);
			SetRender(dwSprID+1, FALSE);
			SetRender(dwSprID+2, TRUE);			
		}
		break;
	}
}

void CDungeonInfoWnd::OnMove()
{	
}

void CDungeonInfoWnd::SetTab(BYTE bTabIndex)
{
	m_bTabIndex = bTabIndex;
	switch(bTabIndex)
	{
	case 0:// 상태보기
		{
		}break;
	case 1:// 정보보기
		{
		}break;
	case 2:// 생산보기
		{
		}break;
	}
}

void CDungeonInfoWnd::NextPage()
{
	POSITION_  pos = m_pDisplayHeadPosition;	
	m_pPageList->GetNext(pos);
	if (!pos)		return;
	
	m_pDisplayHeadPosition = pos;
	m_bCurPage++;
	SelectDungeon(-1);
}

void CDungeonInfoWnd::PrevPage()
{
	POSITION_  pos = m_pDisplayHeadPosition;	
	m_pPageList->GetPrev(pos);
	if (!pos)		return;

	m_pDisplayHeadPosition = pos;
	m_bCurPage--;
	SelectDungeon(-1);
}
void CDungeonInfoWnd::ShowManageButton(BOOL bShow)
{
	SetRender(BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT1, bShow);
	SetRender(BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT2, FALSE);
	SetRender(BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT3, FALSE);
}
void CDungeonInfoWnd::SelectDungeon(int nIndex)
{	
	int nLine = 0;
	m_pSelectDungeon = NULL;

	POSITION_ pos = m_pPageList->GetAt(m_pDisplayHeadPosition);
	while(pos)
	{
		DUNGEON_DATA_EX* pDungeon = (DUNGEON_DATA_EX*)g_pDungeonTable->m_pReceivedDungeonList->GetNext(pos);
		if (nLine == nIndex)
		{
			m_pSelectDungeon = pDungeon;
			
			break;
		}
		if (++nLine == MAX_DISPLAY)	break;
	}

	if (!m_pSelectDungeon)
	{// 선택된게 없다면.
		nIndex = -1;
	}

	//SetPosObjX(SPR_OBJ_DUNGEONINFOEX_SELECTBAR, (float)(49+nLine*17));
	SetPosObjY(SPR_OBJ_DUNGEONINFOEX_SELECTBAR, float(49+nLine*17));
		
	if (nIndex < 0)
	{
		SetRender(SPR_OBJ_DUNGEONINFOEX_SELECTBAR, FALSE);	
		ShowManageButton(FALSE);

		CDungeonManagerWnd* pDungeonManagerWnd = CDungeonManagerWnd::GetInstance();
		
		if (pDungeonManagerWnd->m_pDungeon && pDungeonManagerWnd->m_pDungeon == m_pSelectDungeon)
		{
			pDungeonManagerWnd->SetDungeon(NULL);
			pDungeonManagerWnd->SetActive(FALSE);
		}
		m_pSelectDungeon = NULL;
	}
	else
	{
		SetRender(SPR_OBJ_DUNGEONINFOEX_SELECTBAR, TRUE);

		if(m_pSelectDungeon)
		{
			if (m_pSelectDungeon == g_pThisDungeon)
				ShowManageButton(m_pSelectDungeon->IsDungeonOwner(g_pMainPlayer));
			else
				ShowManageButton(FALSE);
		}
	}
}