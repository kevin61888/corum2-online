//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "CharMoveEditWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "InputManager.h"
#include "CharMoveRtWnd.h"
#include "CodeFun.h"
#include "PacketEncrypt.h"

CCharMoveEditWnd* CCharMoveEditWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CCharMoveEditWnd::CCharMoveEditWnd()
{ 	
	m_bBtn			= FALSE;		
	m_bInputMode	= FALSE;
	m_bNameChk		= FALSE;
	m_byMoveFail	= 0;
	m_byInputIndex	= 0xff;

	for(int i = 0; i < 4; i++)
	{
		m_sCharMoveInfoEdit[i].sCharMoveInfo.byNum		= 0xff;
		m_sCharMoveInfoEdit[i].sCharMoveInfo.byResult	= 0xff;
		memset(&m_sCharMoveInfoEdit[i].sCharMoveInfo.szName, 0, sizeof(m_sCharMoveInfoEdit[i].sCharMoveInfo.szName));
		memset(&m_sCharMoveInfoEdit[i].szName, 0, sizeof(m_sCharMoveInfoEdit[i].szName));
	}
}

CCharMoveEditWnd::~CCharMoveEditWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CCharMoveEditWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_CHARMOVEEDIT_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHARMOVEEDIT_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_CHARMOVEEDIT_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);			
	InsertData(SPR_OBJ_CHARMOVEEDIT_OK1, SPR_INTERFACE_OK1, 189, 238, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVEEDIT_OK2, SPR_INTERFACE_OK2, 189, 238, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVEEDIT_OK3, SPR_INTERFACE_OK3, 189, 238, 1.0f, 1.0f, 2);	
		
	InsertCheckInterface(189, 238, 254, 254, 2, CHECK_BTN);	

	InsertCheckInterface(15, 80, 254, 94, 3, CHECK_BTN);	
	InsertCheckInterface(15, 100, 254, 114, 4, CHECK_BTN);	
	InsertCheckInterface(15, 120, 254, 134, 5, CHECK_BTN);	
	InsertCheckInterface(15, 140, 254, 154, 6, CHECK_BTN);	
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CCharMoveEditWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CCharMoveEditWnd::SetOrder()
{
}

void CCharMoveEditWnd::RenderText()
{	
	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1094].szMessage); // "알림 - 캐릭터 이전"
				
	int nSize = lstrlen(m_szInfo);
		
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);

	wsprintf(m_szInfo, g_Message[ETC_MESSAGE1095].szMessage); // "캐릭터의 이름을 수정해 주세요."
				
	nSize = lstrlen(m_szInfo);

	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

	BYTE byInde = 0;

	for(int i = 0; i < 4; i++)
	{
		DWORD dwColor = 0xffffffff;

		if(m_sCharMoveInfoEdit[i].sCharMoveInfo.byResult!=1)
			continue;
		
		wsprintf(m_szInfo, "%d. %s : ", i+1, m_sCharMoveInfoEdit[i].sCharMoveInfo.szName);

		if(m_bInputMode	&& m_byInputIndex==i)
		{
			if(!IsEmptyString(g_pInputManager->GetInputBuffer(1)))
				lstrcat(m_szInfo, g_pInputManager->GetInputBuffer(1));			

			if(g_pInputManager->IsDrawCursor())
				lstrcat(m_szInfo, "|");
		}
		else
		{
			if(!IsEmptyString(m_sCharMoveInfoEdit[i].szName))
				lstrcat(m_szInfo, m_sCharMoveInfoEdit[i].szName);
		}		

		nSize = lstrlen(m_szInfo);
		
		if( (g_Mouse.MousePos.x>=m_fPosX+15 && g_Mouse.MousePos.x<=m_fPosX+15+nSize*7) &&
			(g_Mouse.MousePos.y>=m_fPosZ+80+i*20 && g_Mouse.MousePos.y<=m_fPosZ+94+i*20) )
			dwColor = 0xfffff000;

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+80+i*20, m_fPosZ+94+i*20, GetStartOrder()+1, dwColor);

		byInde += 1;
	}

	byInde += 2;

	if(m_bNameChk)
	{
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE1096].szMessage); // "캐릭명이 중복되었거나, 공백이 있어서"

		nSize = lstrlen(m_szInfo);
		
		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+100+byInde*20, m_fPosZ+114+byInde*20, GetStartOrder()+1);

		byInde += 1;

		wsprintf(m_szInfo, g_Message[ETC_MESSAGE1097].szMessage); // "이전에 실패하였습니다. 확인 후 재이전을"

		nSize = lstrlen(m_szInfo);		

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+100+byInde*20, m_fPosZ+114+byInde*20, GetStartOrder()+1);

		byInde += 1;

		wsprintf(m_szInfo, g_Message[ETC_MESSAGE1098].szMessage); // "해주시기 바랍니다."

		nSize = lstrlen(m_szInfo);
		
		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+100+byInde*20, m_fPosZ+114+byInde*20, GetStartOrder()+1);
	}
}

void CCharMoveEditWnd::SetCharacter(char* szName, BYTE byNum, BYTE byResult)
{
	for(int i = 0; i < 4; i++)
	{
		if(m_sCharMoveInfoEdit[i].sCharMoveInfo.byNum!=0xff)
		{
			if(__strcmp(m_sCharMoveInfoEdit[i].sCharMoveInfo.szName, szName)==0)
			{
				m_sCharMoveInfoEdit[i].sCharMoveInfo.byNum		= byNum;
				m_sCharMoveInfoEdit[i].sCharMoveInfo.byResult	= byResult;
				return;
			}
		}		
	}

	for(i = 0; i < 4; i++)
	{
		if(m_sCharMoveInfoEdit[i].sCharMoveInfo.byNum==0xff)
		{
			__lstrcpyn(m_sCharMoveInfoEdit[i].sCharMoveInfo.szName, szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			m_sCharMoveInfoEdit[i].sCharMoveInfo.byNum		= byNum;
			m_sCharMoveInfoEdit[i].sCharMoveInfo.byResult	= byResult;			
			return;
		}		
	}			
}

void CCharMoveEditWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;
	m_bNameChk	= FALSE;

	if(bActive==FALSE)
	{
		// 캐릭터 이름 재전송 //
		if(m_bInputMode)
		{
			__lstrcpyn(m_sCharMoveInfoEdit[m_byInputIndex].szName, g_pInputManager->GetInputBuffer(1), MAX_CHARACTER_NAME_REAL_LENGTH);

			m_bInputMode	= FALSE;
			m_byInputIndex	= 0xff;
			g_pInputManager->SetFocusInput(0);
			g_pInputManager->ClearInput(1);
		}		

		BOOL bChk = FALSE;

		for(int i = 0; i < 4; i++)
		{
			if(m_sCharMoveInfoEdit[i].sCharMoveInfo.byResult==1)
			{	
				if(IsEmptyString(m_sCharMoveInfoEdit[i].szName))
				{
					bChk = TRUE;
					break;
				}
				else if(__strcmp(m_sCharMoveInfoEdit[i].szName, m_sCharMoveInfoEdit[i].sCharMoveInfo.szName)==0)
				{
					bChk = TRUE;
					break;
				}
			}
		}

		if(bChk)
		{
			m_bNameChk	= TRUE;
			m_bActive	= TRUE;
			return;
		}
		else
		{
			CTAS_MOVE_CHARACTER_RESEND pReSendPacket;

			BYTE byCnt = 0;

			for(int i = 0; i < 4; i++)
			{
				pReSendPacket.byNum[i]	= m_sCharMoveInfoEdit[i].sCharMoveInfo.byNum;
				
				if(m_sCharMoveInfoEdit[i].sCharMoveInfo.byResult==1)
				{
					pReSendPacket.byType[i]	= 1;
					byCnt += 1;
				}
				else
					pReSendPacket.byType[i]	= 0xff;

				memset(pReSendPacket.szName[i], 0, sizeof(pReSendPacket.szName[i]));
				__lstrcpyn(pReSendPacket.szName[i], m_sCharMoveInfoEdit[i].szName, MAX_CHARACTER_NAME_REAL_LENGTH);

				CCharMoveRtWnd* pCharMoveRtWnd = CCharMoveRtWnd::GetInstance();

				for(int j = 0; j < 4; j++)
				{
					if(__strcmp(pCharMoveRtWnd->m_sCharMoveInfo[j].szName, m_sCharMoveInfoEdit[i].sCharMoveInfo.szName)==0)
					{
						pCharMoveRtWnd->m_sCharMoveInfo[j].byNum	= 0xff;
						pCharMoveRtWnd->m_sCharMoveInfo[j].byResult	= 0;
						memset(pCharMoveRtWnd->m_sCharMoveInfo[j].szName, 0, sizeof(pCharMoveRtWnd->m_sCharMoveInfo[j].szName));
					}
				}
			}

			// by minjin. 2004. 10. 25.
//			SendMsgWithBlowfish((char*)&pReSendPacket, pReSendPacket.GetPacketSize(), SERVER_INDEX_AGENT);
//			g_pNet->SendMsg((char*)&pReSendPacket, pReSendPacket.GetPacketSize(), SERVER_INDEX_AGENT);
			
			if(byCnt>g_byCharMoveFail)
				return;
			
			g_byCharMoveFail -= byCnt;

			for(i = 0; i < 4; i++)
			{
				memset(&m_sCharMoveInfoEdit[i], 0, sizeof(m_sCharMoveInfoEdit[i]));
				m_sCharMoveInfoEdit[i].sCharMoveInfo.byNum	= 0xff;
			}
		}
		ShowSpriteAll();		
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);
		
		SetRender(SPR_OBJ_CHARMOVEEDIT_WND1, TRUE);		
		SetRender(SPR_OBJ_CHARMOVEEDIT_WND2, TRUE);
		SetRender(SPR_OBJ_CHARMOVEEDIT_WND3, TRUE);
		SetRender(SPR_OBJ_CHARMOVEEDIT_OK1, TRUE);						
	}
}

int CCharMoveEditWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn && nRt!=2)
	{
		SetRender(SPR_OBJ_CHARMOVEEDIT_OK1, TRUE);
		SetRender(SPR_OBJ_CHARMOVEEDIT_OK2, FALSE);
		SetRender(SPR_OBJ_CHARMOVEEDIT_OK3, FALSE);								
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
				m_bBtn	= TRUE;
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_CHARMOVEEDIT_OK1, FALSE);
				SetRender(SPR_OBJ_CHARMOVEEDIT_OK2, FALSE);
				SetRender(SPR_OBJ_CHARMOVEEDIT_OK3, TRUE);
				m_bBtn = TRUE;
			}			
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_CHARMOVEEDIT_OK1, FALSE);
				SetRender(SPR_OBJ_CHARMOVEEDIT_OK2, TRUE);
				SetRender(SPR_OBJ_CHARMOVEEDIT_OK3, FALSE);
				m_bBtn = TRUE;
			}		
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;	
	case 3:
	case 4:
	case 5:
	case 6:
		{
			BYTE byChk = 0;

			if(m_sCharMoveInfoEdit[nRt-3].sCharMoveInfo.byResult!=1)
			{
				if(eMouseCheck==MOUSE_UP)
					byChk = __MOUSE_POINTER_DEFAULT__;				
				else if(eMouseCheck==MOUSE_DOWN)
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				else if(eMouseCheck==MOUSE_OVER)
					byChk = __MOUSE_POINTER_DEFAULT__;			
			}
			else
			{
				if(eMouseCheck==MOUSE_UP)
				{											
					if(m_bInputMode && m_byInputIndex!=nRt-3)
					{						
						__lstrcpyn(m_sCharMoveInfoEdit[m_byInputIndex].szName, g_pInputManager->GetInputBuffer(1), MAX_CHARACTER_NAME_REAL_LENGTH);
						
						m_byInputIndex	= BYTE(nRt-3);
						g_pInputManager->SetFocusInput(1);

						if(!IsEmptyString(g_pInputManager->GetInputBuffer(1)))
							g_pInputManager->ClearInput(1);

						if(!IsEmptyString(m_sCharMoveInfoEdit[nRt-3].szName))
						{
							int nLen = lstrlen(m_sCharMoveInfoEdit[nRt-3].szName);

							g_pInputManager->ClearInput(1);
							g_pInputManager->InsertCharacter(1, m_sCharMoveInfoEdit[nRt-3].szName, nLen);
						}
					}
					else if(!m_bInputMode)
					{
						m_bInputMode	= TRUE;
						m_byInputIndex	= BYTE(nRt-3);
						g_pInputManager->SetFocusInput(1);

						if(!IsEmptyString(m_sCharMoveInfoEdit[nRt-3].szName))
						{
							int nLen = lstrlen(m_sCharMoveInfoEdit[nRt-3].szName);

							g_pInputManager->ClearInput(1);
							g_pInputManager->InsertCharacter(1, m_sCharMoveInfoEdit[nRt-3].szName, nLen);
						}
					}
					else if(m_bInputMode && m_byInputIndex==nRt-3)
					{						
						__lstrcpyn(m_sCharMoveInfoEdit[nRt-3].szName, g_pInputManager->GetInputBuffer(1), MAX_CHARACTER_NAME_REAL_LENGTH);

						m_bInputMode	= FALSE;
						m_byInputIndex	= 0xff;
						g_pInputManager->SetFocusInput(0);
						g_pInputManager->ClearInput(1);
					}					
				}
			}
			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;		
	}			
	
	MouseUp();
	
	MouseChk(nRt);	

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}
//======================================================//
// End.													//
//======================================================//