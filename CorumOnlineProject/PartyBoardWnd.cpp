//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "PartyBoardWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "PartyEntryWnd.h"
#include "PartyHelpWnd.h"
#include "CodeFun.h"

CPartyBoardWnd* CPartyBoardWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CPartyBoardWnd::CPartyBoardWnd()
{ 		
	m_bBtn[0]	= FALSE;
	m_bBtn[1]	= FALSE;
	m_bBtn[2]	= FALSE;
	m_byLine	= 0;
	m_dwTime	= 0;
	m_byErrChk	= 0;
}

CPartyBoardWnd::~CPartyBoardWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CPartyBoardWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_PARTYBOARD_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_PARTYBOARD_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 6.0f, 0);
	InsertData(SPR_OBJ_PARTYBOARD_WND3, SPR_INTERFACE_DF_WND3, 0, 128, 1.0f, 1.0f, 0);	
	InsertData(SPR_OBJ_PARTYBOARD_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYBOARD_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYENTRY1, SPR_INTERFACE_PARTYBOARD_ENTRY1, 25, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYENTRY2, SPR_INTERFACE_PARTYBOARD_ENTRY2, 25, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYENTRY3, SPR_INTERFACE_PARTYBOARD_ENTRY3, 25, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYSEA1, SPR_INTERFACE_PARTYBOARD_SEA1, 99, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYSEA2, SPR_INTERFACE_PARTYBOARD_SEA2, 99, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYSEA3, SPR_INTERFACE_PARTYBOARD_SEA3, 99, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYHELP1, SPR_INTERFACE_PARTYBOARD_HELP1, 173, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYHELP2, SPR_INTERFACE_PARTYBOARD_HELP2, 173, 115, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_PARTYBOARD_PARTYHELP3, SPR_INTERFACE_PARTYBOARD_HELP3, 173, 115, 1.0f, 1.0f, 2);
		
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(25, 115, 85, 131, 4, CHECK_BTN);
	InsertCheckInterface(99, 115, 159, 131, 5, CHECK_BTN);
	InsertCheckInterface(173, 115, 233, 131, 6, CHECK_BTN);
		*/
	m_bInit	= TRUE;

	return TRUE;
}

void CPartyBoardWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CPartyBoardWnd::SetOrder()
{
}

void CPartyBoardWnd::RenderText()
{
	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1147].szMessage); // "파티정보"
	
	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+85, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);
	
	for(int i = 0; i < m_byLine; i++)
		RenderFont(m_szText[i], m_fPosX+20, m_fPosX+20+m_nSize[i]*7, m_fPosZ+40+i*20, m_fPosZ+54+i*20, GetStartOrder()+1);		
}

void CPartyBoardWnd::SetText(char* pszText)
{
	char*	szTemp	= pszText;	
	int nSize = lstrlen(pszText);

	m_byLine = 0;

	for(int i = 0; i < 5; i++)	
	{
		m_nSize[i]	= 0;
		memset(m_szText[i], 0, sizeof(m_szText[i]));
	}
	
	while(nSize>0)
	{
		if(nSize>34)
		{
			for(;;)
			{
				if(nSize > 38)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+34, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev); 

					__lstrcpyn(m_szText[m_byLine], szTemp, nSize);
					m_nSize[m_byLine] = nSize;

					szTemp = szPrev; 
					m_byLine++;
					nSize = lstrlen(szTemp);
				}
				else
				{
					// 구한 문자열을 채팅 라인에 쓰고, 길이를 기록한다.
					lstrcpy(m_szText[m_byLine], szTemp);
					m_nSize[m_byLine] = lstrlen(szTemp);
					m_byLine++;								
					nSize = 0;
					break;
				}
			}
		}
		else
		{
			lstrcpy(m_szText[m_byLine], szTemp);
			m_nSize[m_byLine] = lstrlen(m_szText[m_byLine]);
			m_byLine++;
			nSize = 0;							
		}		
	}		
}
void CPartyBoardWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{	
		m_byErrChk	= 0;
		m_bSoundChk	= FALSE;
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_PARTYBOARD_WND1, TRUE);
		SetRender(SPR_OBJ_PARTYBOARD_WND2, TRUE);
		SetRender(SPR_OBJ_PARTYBOARD_WND3, TRUE);
		SetRender(SPR_OBJ_PARTYBOARD_CLOSE1, TRUE);		
		SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY1, TRUE);		
		SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA1, TRUE);		
		SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CPartyBoardWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY1, TRUE);
		SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY2, FALSE);
		SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY3, FALSE);
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA1, TRUE);
		SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA2, FALSE);
		SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA3, FALSE);
		m_bBtn[1] = FALSE;
	}

	if(m_bBtn[2] && nRt!=6)
	{
		SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP1, TRUE);
		SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP2, FALSE);
		SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP3, FALSE);
		m_bBtn[2] = FALSE;
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
				SetRender(SPR_OBJ_PARTYBOARD_CLOSE1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_CLOSE2, TRUE);
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
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY2, TRUE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY3, FALSE);
				m_bBtn[0] = TRUE;

				if(g_pMainPlayer->m_dwPartyId!=0)
				{
					CPartyEntryWnd* pPartyEntryWnd	= CPartyEntryWnd::GetInstance();
					pPartyEntryWnd->m_byEntryType	= PARTYENTRY_INPUT;
					pPartyEntryWnd->SetActive();
				}
				else
					DisplayMessageAdd(g_Message[ETC_MESSAGE69].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "참가하고 있는 파티가 없습니다."
				
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY2, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY2, TRUE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYENTRY3, FALSE);
				m_bBtn[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA2, TRUE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA3, FALSE);
				m_bBtn[1] = TRUE;
				
				DWORD dwNextTime = timeGetTime();

				// 30초 동안은 보낼수 없다 //
				if((dwNextTime-m_dwTime) >= 1000*30)
				{
					m_dwTime = dwNextTime;

					CTWS_PARTY_INFOREQUEST pPacket;
					pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);

					SetActive(FALSE);
				}
				else
				{
					if(m_byErrChk==0)
					{
						DisplayMessageAdd(g_Message[ETC_MESSAGE1148].szMessage, 0xffffff00, TRUE, DEF_CHAT_TYPE_PARTY); // "파티정보 갱신중입니다."
						m_byErrChk = 1;
					}
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{	
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA2, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA2, TRUE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYSEA3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP2, TRUE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP3, FALSE);
				m_bBtn[2] = TRUE;

				CPartyHelpWnd* pPartyHelpWnd = CPartyHelpWnd::GetInstance();
				pPartyHelpWnd->SetActive();
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{	
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP2, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP3, TRUE);
				m_bBtn[2] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{					
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP1, FALSE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP2, TRUE);
				SetRender(SPR_OBJ_PARTYBOARD_PARTYHELP3, FALSE);
				m_bBtn[2] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
					
	if(MouseUp())
	{
		SetRender(SPR_OBJ_PARTYBOARD_CLOSE1, TRUE);
		SetRender(SPR_OBJ_PARTYBOARD_CLOSE2, FALSE);		
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

