//======================================================//
// Code by Jang.							2002.1.10	//
//======================================================//
#include "ChatListWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "CodeFun.h"

CChatListWnd* CChatListWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CChatListWnd::CChatListWnd()
{
	m_byIndex		= 0;		
	m_bySize		= 0;				
	m_byScrollChk	= 1;
	m_byMode		= 0;
	m_byChatType	= CHAT_TYPE_ALL;		
	m_bChkBtn[0]	= FALSE;
	m_bChkBtn[1]	= FALSE;
	m_bChkBtn[2]	= FALSE;
	m_bChkBtn[3]	= FALSE;
}

CChatListWnd::~CChatListWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CChatListWnd::Init()
{
/*
	InsertData(SPR_OBJ_CHATLIST_WND1, SPR_INTERFACE_CHATLIST_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHATLIST_WND2, SPR_INTERFACE_CHATLIST_WND2, 0, 32, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHATLIST_WND3, SPR_INTERFACE_CHATLIST_WND3, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHATLIST_CLOSE1, SPR_INTERFACE_CLOSE1, 406, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_CLOSE2, SPR_INTERFACE_CLOSE2, 406, 4, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_CHATLIST_UPDOWN1, SPR_INTERFACE_CHATLIST_UPDOWN1, 391, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_UPDOWN2, SPR_INTERFACE_CHATLIST_UPDOWN2, 391, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_ALL1, SPR_INTERFACE_CHATLIST_ALL1, 159, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_ALL2, SPR_INTERFACE_CHATLIST_ALL2, 159, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_ALL3, SPR_INTERFACE_CHATLIST_ALL3, 159, 3, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_CHATLIST_PARTY1, SPR_INTERFACE_CHATLIST_PARTY1, 205, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_PARTY2, SPR_INTERFACE_CHATLIST_PARTY2, 205, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_PARTY3, SPR_INTERFACE_CHATLIST_PARTY3, 205, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_GUILD1, SPR_INTERFACE_CHATLIST_GUILD1, 251, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_GUILD2, SPR_INTERFACE_CHATLIST_GUILD2, 251, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_GUILD3, SPR_INTERFACE_CHATLIST_GUILD3, 251, 3, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_CHATLIST_WHISPER1, SPR_INTERFACE_CHATLIST_WHISPER1, 297, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_WHISPER2, SPR_INTERFACE_CHATLIST_WHISPER2, 297, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_WHISPER3, SPR_INTERFACE_CHATLIST_WHISPER3, 297, 3, 1.0f, 1.0f, 1);		
	InsertData(SPR_OBJ_CHATLIST_FRIEND1, SPR_INTERFACE_CHATLIST_FRIEND1, 343, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_FRIEND2, SPR_INTERFACE_CHATLIST_FRIEND2, 343, 3, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_FRIEND3, SPR_INTERFACE_CHATLIST_FRIEND3, 343, 3, 1.0f, 1.0f, 1);	
	InsertData(SPR_OBJ_CHATLIST_SC, SPR_INTERFACE_SC, 407, 32, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_SC1, SPR_INTERFACE_CHATLIST_SC1, 406, 20, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_SC2, SPR_INTERFACE_CHATLIST_SC2, 406, 20, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_SC3, SPR_INTERFACE_CHATLIST_SC3, 406, 20, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_CHATLIST_SC4, SPR_INTERFACE_CHATLIST_SC4, 406, 20, 1.0f, 1.0f, 1);
			 
	InsertCheckInterface(406, 4, 418, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 158, 20, 3, CHECK_MOVEWND);	
	InsertCheckInterface(391, 4, 406, 16, 4, CHECK_BTN);
	InsertCheckInterface(159, 3, 203, 19, 5, CHECK_BTN);
	InsertCheckInterface(205, 3, 249, 19, 6, CHECK_BTN);
	InsertCheckInterface(251, 3, 295, 19, 7, CHECK_BTN);
	InsertCheckInterface(297, 3, 341, 19, 8, CHECK_BTN);
	InsertCheckInterface(343, 3, 387, 19, 9, CHECK_BTN);
	InsertCheckInterface(407, 32, 416, 122+m_bySize*64, 10, CHECK_SCROLL, 2);
	InsertCheckInterface(406, 20, 417, 30, 11, CHECK_BTN);
	InsertCheckInterface(406, (m_bySize*64+90)+32, 417, (m_bySize*64+90)+32+16, 12, CHECK_BTN, 1);
	*/
	InsertCheckInterface(407, 32, 416, 122+m_bySize*64, 10, CHECK_SCROLL, 2);
	InsertCheckInterface(406, (m_bySize*64+90)+32, 417, (m_bySize*64+90)+32+16, 12, CHECK_BTN, 1);
	m_bInit = TRUE;

	return TRUE;
}

void CChatListWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CChatListWnd::SetOrder()
{
}

void CChatListWnd::RenderText()
{	
	RenderFont(g_Message[ETC_MESSAGE486].szMessage, m_fPosX+25, m_fPosX+85, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);			

	if(m_byChatType==CHAT_TYPE_ALL)
	{
		POSITION_ pos = g_pAllChatList->GetHeadPosition();

		BYTE byIndex	= 0;
		BYTE byCurPos	= 0;

		while(pos)
		{
			LP_CHAT_MSG lpChatMsg = (LP_CHAT_MSG)g_pAllChatList->GetNext(pos);

			if(m_nCurScrollPos>byCurPos)
			{
				byCurPos++;
				continue;
			}
				
			if(byIndex < m_bySize * 4 + 5)
			{
				int nSize = lstrlen(lpChatMsg->szText);
				RenderFont(lpChatMsg->szText
					, m_fPosX+10
					, m_fPosX+(nSize*6)+10
					, m_fPosZ+32+8+byIndex*16
					, m_fPosZ+32+22+byIndex*16
					, GetStartOrder()+1
					, lpChatMsg->dwColor);
				byIndex++;
			}
		}
	}	
	else if(m_byChatType==CHAT_TYPE_PARTY)
	{
		POSITION_ pos = g_pPartyChatList->GetHeadPosition();

		BYTE byIndex	= 0;
		BYTE byCurPos	= 0;

		while(pos)
		{
			LP_CHAT_MSG lpChatMsg = (LP_CHAT_MSG)g_pPartyChatList->GetNext(pos);

			if(m_nCurScrollPos>byCurPos)
			{
				byCurPos++;
				continue;
			}
				
			if(byIndex<m_bySize*4+5)
			{
				int nSize = lstrlen(lpChatMsg->szText);
				RenderFont(lpChatMsg->szText, m_fPosX+10, m_fPosX+10+nSize*7, m_fPosZ+32+8+byIndex*16, m_fPosZ+32+22+byIndex*16, GetStartOrder()+1, lpChatMsg->dwColor);
				byIndex++;
			}
		}
	}
	else if(m_byChatType==CHAT_TYPE_GUILD)
	{
		POSITION_ pos = g_pGuildChatList->GetHeadPosition();

		BYTE byIndex	= 0;
		BYTE byCurPos	= 0;

		while(pos)
		{
			LP_CHAT_MSG lpChatMsg = (LP_CHAT_MSG)g_pGuildChatList->GetNext(pos);

			if(m_nCurScrollPos>byCurPos)
			{
				byCurPos++;
				continue;
			}
				
			if(byIndex<m_bySize*4+5)
			{
				int nSize = lstrlen(lpChatMsg->szText);
				RenderFont(lpChatMsg->szText, m_fPosX+10, m_fPosX+10+nSize*7, m_fPosZ+32+8+byIndex*16, m_fPosZ+32+22+byIndex*16, GetStartOrder()+1, lpChatMsg->dwColor);
				byIndex++;
			}
		}
	}
	else if(m_byChatType==CHAT_TYPE_WHISPER)
	{
		POSITION_ pos = g_pWhisperChatList->GetHeadPosition();

		BYTE byIndex	= 0;
		BYTE byCurPos	= 0;

		while(pos)
		{
			LP_CHAT_MSG lpChatMsg = (LP_CHAT_MSG)g_pWhisperChatList->GetNext(pos);

			if(m_nCurScrollPos>byCurPos)
			{
				byCurPos++;
				continue;
			}
				
			if(byIndex<m_bySize*4+5)
			{
				int nSize = lstrlen(lpChatMsg->szText);
				RenderFont(lpChatMsg->szText, m_fPosX+10, m_fPosX+10+nSize*7, m_fPosZ+32+8+byIndex*16, m_fPosZ+32+22+byIndex*16, GetStartOrder()+1, lpChatMsg->dwColor);
				byIndex++;
			}
		}
	}
	else if(m_byChatType==CHAT_TYPE_FRIEND)
	{
		POSITION_ pos = g_pFriendChatList->GetHeadPosition();

		BYTE byIndex	= 0;
		BYTE byCurPos	= 0;

		while(pos)
		{
			LP_CHAT_MSG lpChatMsg = (LP_CHAT_MSG)g_pFriendChatList->GetNext(pos);

			if(m_nCurScrollPos>byCurPos)
			{
				byCurPos++;
				continue;
			}
				
			if(byIndex<m_bySize*4+5)
			{
				int nSize = lstrlen(lpChatMsg->szText);
				RenderFont(lpChatMsg->szText, m_fPosX+10, m_fPosX+10+nSize*7, m_fPosZ+32+8+byIndex*16, m_fPosZ+32+22+byIndex*16, GetStartOrder()+1, lpChatMsg->dwColor);
				byIndex++;
			}
		}
	}
}

void CChatListWnd::SetActive(BOOL bActive)
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
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
		SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
		SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
		SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);
		SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);		
		SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);	

		switch(m_byChatType)
		{
		case CHAT_TYPE_ALL:
			SetRender(SPR_OBJ_CHATLIST_ALL3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
			break;
		case CHAT_TYPE_PARTY:
			SetRender(SPR_OBJ_CHATLIST_PARTY3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
			break;
		case CHAT_TYPE_GUILD:
			SetRender(SPR_OBJ_CHATLIST_GUILD3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
			break;
		case CHAT_TYPE_WHISPER:
			SetRender(SPR_OBJ_CHATLIST_WHISPER3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
			break;
		case CHAT_TYPE_FRIEND:
			SetRender(SPR_OBJ_CHATLIST_FRIEND3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
			break;
		}			 
		SetRender(SPR_OBJ_CHATLIST_SC1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_SC3, TRUE);
		SetRender(SPR_OBJ_CHATLIST_SC, TRUE);
		SetRender(SPR_OBJ_CHATLIST_WND1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_WND2, TRUE);
		SetRender(SPR_OBJ_CHATLIST_WND3, TRUE);	
		SetRender(SPR_OBJ_CHATLIST_CLOSE1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_UPDOWN1, TRUE);
		
		SetScalingObj(SPR_OBJ_CHATLIST_WND2, 1.0f, 11.0f+m_bySize*8.0f);
		SetPosObjY(SPR_OBJ_CHATLIST_WND3, float(32+88+m_bySize*64));
		SetPosObjY(SPR_OBJ_CHATLIST_SC3, float((m_bySize*64+90)+32+2));
		SetPosObj(SPR_OBJ_CHATLIST_SC4, 406, float((m_bySize*64+90)+32+2));
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CChatListWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk(m_bySize*64);
	
	if(m_bChkBtn[0]==TRUE && nRt!=4)
	{
		m_bChkBtn[0] = FALSE;
		SetRender(SPR_OBJ_CHATLIST_UPDOWN1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_UPDOWN2, FALSE);
	}

	if(m_bChkBtn[1]==TRUE && !(nRt>=5 && nRt<=9))
	{
		SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
		SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
		SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
		SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
		SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
		SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
		SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);

		SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);

		switch(m_byChatType)
		{
		case CHAT_TYPE_ALL:
			SetRender(SPR_OBJ_CHATLIST_ALL3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
			break;
		case CHAT_TYPE_PARTY:
			SetRender(SPR_OBJ_CHATLIST_PARTY3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
			break;
		case CHAT_TYPE_GUILD:
			SetRender(SPR_OBJ_CHATLIST_GUILD3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
			break;
		case CHAT_TYPE_WHISPER:
			SetRender(SPR_OBJ_CHATLIST_WHISPER3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
			break;
		case CHAT_TYPE_FRIEND:
			SetRender(SPR_OBJ_CHATLIST_FRIEND3, TRUE);
			SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
			break;
		}
		m_bChkBtn[1] = FALSE;
	}
	
	if(m_bChkBtn[2]==TRUE && nRt!=11)
	{
		SetRender(SPR_OBJ_CHATLIST_SC1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_SC2, FALSE);
		m_bChkBtn[2] = FALSE;
	}
	
	if(m_bChkBtn[3]==TRUE && nRt!=12)
	{
		SetRender(SPR_OBJ_CHATLIST_SC3, TRUE);
		SetRender(SPR_OBJ_CHATLIST_SC4, FALSE);
		m_bChkBtn[3] = FALSE;
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
				SetRender(SPR_OBJ_CHATLIST_CLOSE1, FALSE);
				SetRender(SPR_OBJ_CHATLIST_CLOSE2, TRUE);
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
				m_bChkBtn[0]	= TRUE;

				SetRender(SPR_OBJ_CHATLIST_UPDOWN1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_UPDOWN2, FALSE);
								
				// 크기 변경 //
				if(m_byMode==0)
				{
					if(m_bySize<3)
						m_bySize++;
					else
					{
						m_byMode = 1;
						m_bySize--;
					}
				}
				else
				{
					if(m_bySize>0)
						m_bySize--;
					else
					{
						m_byMode = 0;
						m_bySize++;
					}
				}
				
				SetPos(m_fPosX, m_fPosZ);
				SetActive();

				// 재배치 //
				int nCount = 0;
				
				switch(m_byChatType)
				{
				case CHAT_TYPE_ALL:
					nCount = g_pAllChatList->GetCount();
					break;
				case CHAT_TYPE_PARTY:
					nCount = g_pPartyChatList->GetCount();
					break;
				case CHAT_TYPE_GUILD:
					nCount = g_pGuildChatList->GetCount();
					break;
				case CHAT_TYPE_WHISPER:
					nCount = g_pWhisperChatList->GetCount();
					break;
				case CHAT_TYPE_FRIEND:
					nCount = g_pFriendChatList->GetCount();
					break;
				}

				if(nCount>m_bySize*4+5)
				{
					// 번호 //					
					int		nIndex	= nCount-(m_bySize*4+5)+1;						
			
					m_nCurScrollPos	= nIndex-1;
					m_fSCTemp	= float((90+m_bySize*64)+32-16);
					SetPosObjY(SPR_OBJ_CHATLIST_SC, float((90+m_bySize*64)+32-16));
				}
				else
				{
					m_nCurScrollPos  = 0;
					m_fSCTemp = 32;
					SetPosObjY(SPR_OBJ_CHATLIST_SC, 32);
				}					
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChkBtn[0]	= TRUE;
				SetRender(SPR_OBJ_CHATLIST_UPDOWN1, FALSE);
				SetRender(SPR_OBJ_CHATLIST_UPDOWN2, TRUE);
			}		
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{						
			if(eMouseCheck==MOUSE_DOWN)
			{			
				m_byChatType	= CHAT_TYPE_ALL;
				m_bChkBtn[1]	= TRUE;
							
				int nCount = g_pAllChatList->GetCount();

				if(nCount>m_bySize*4+5)
				{
					// 번호 //					
					int		nIndex	= nCount-(m_bySize*4+5)+1;						
				
					m_nCurScrollPos	= nIndex-1;
					m_fSCTemp		= float((90+m_bySize*64)+32-16);
					SetPosObjY(SPR_OBJ_CHATLIST_SC, float((90+m_bySize*64)+32-16));
				}
				else
				{
					m_nCurScrollPos  = 0;
					m_fSCTemp		 = 32;
					SetPosObjY(SPR_OBJ_CHATLIST_SC, 32);
				}

				SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
				SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_ALL3, TRUE);
				SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);										
			}
			else if(eMouseCheck==MOUSE_OVER)
			{								
				if(m_byChatType!=CHAT_TYPE_ALL)
				{	
					SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL2, TRUE);
					SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHATLIST_ALL3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHATLIST_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHATLIST_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHATLIST_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHATLIST_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
						break;
					}		
					m_bChkBtn[1] = TRUE;					
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:		
		{						
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byChatType	= CHAT_TYPE_PARTY;
				m_bChkBtn[1]	= TRUE;

				int nCount = g_pPartyChatList->GetCount();

				if(nCount>m_bySize*4+5)
				{
					// 번호 //					
					int		nIndex	= nCount-(m_bySize*4+5)+1;						
				
					m_nCurScrollPos	= nIndex-1;
					m_fSCTemp		= float((90+m_bySize*64)+32-16);
					SetPosObjY(SPR_OBJ_CHATLIST_SC, float((90+m_bySize*64)+32-16));
				}
				else
				{
					m_nCurScrollPos  = 0;
					m_fSCTemp = 32;
					SetPosObjY(SPR_OBJ_CHATLIST_SC, 32);
				}
				
				SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY3, TRUE);
				SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);
			}
			if(eMouseCheck==MOUSE_OVER)
			{			
				if(m_byChatType!=CHAT_TYPE_PARTY)
				{
					SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_PARTY2, TRUE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHATLIST_ALL3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHATLIST_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHATLIST_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHATLIST_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHATLIST_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
						break;
					}
					m_bChkBtn[1] = TRUE;					
				}			
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 7:
		{						
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byChatType = CHAT_TYPE_GUILD;
				m_bChkBtn[1] = TRUE;

				int nCount = g_pGuildChatList->GetCount();

				if(nCount>m_bySize*4+5)
				{
					// 번호 //					
					int		nIndex	= nCount-(m_bySize*4+5)+1;						
				
					m_nCurScrollPos	= nIndex-1;
					m_fSCTemp		= float((90+m_bySize*64)+32-16);
					SetPosObjY(SPR_OBJ_CHATLIST_SC, float((90+m_bySize*64)+32-16));
				}
				else
				{
					m_nCurScrollPos  = 0;
					m_fSCTemp		= 32;
					SetPosObjY(SPR_OBJ_CHATLIST_SC, 32);
				}

				SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD3, TRUE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);																	
			}
			else if(eMouseCheck==MOUSE_OVER)
			{			
				if(m_byChatType!=CHAT_TYPE_GUILD)
				{
					SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_GUILD2, TRUE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHATLIST_ALL3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHATLIST_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHATLIST_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHATLIST_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHATLIST_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
						break;
					}
					m_bChkBtn[1] = TRUE;					
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}				
		break;
	case 8:
		{						
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byChatType = CHAT_TYPE_WHISPER;
				m_bChkBtn[1] = TRUE;

				int nCount = g_pWhisperChatList->GetCount();

				if(nCount>m_bySize*4+5)
				{
					// 번호 //					
					int		nIndex	= nCount-(m_bySize*4+5)+1;						
				
					m_nCurScrollPos	= nIndex-1;
					m_fSCTemp	= float((90+m_bySize*64)+32-16);
					SetPosObjY(SPR_OBJ_CHATLIST_SC, float((90+m_bySize*64)+32-16));
				}
				else
				{
					m_nCurScrollPos  = 0;
					m_fSCTemp		= 32;
					SetPosObjY(SPR_OBJ_CHATLIST_SC, 32);
				}
				
				SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER3, TRUE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
			}
			else if(eMouseCheck==MOUSE_OVER)
			{			
				if(m_byChatType!=CHAT_TYPE_WHISPER)
				{
					SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER2, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHATLIST_ALL3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHATLIST_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHATLIST_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHATLIST_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHATLIST_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
						break;
					}
					m_bChkBtn[1] = TRUE;					
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}		
		break;
	case 9:
		{						
			if(eMouseCheck==MOUSE_DOWN)
			{			
				m_byChatType = CHAT_TYPE_FRIEND;
				m_bChkBtn[1] = TRUE;

				int nCount = g_pFriendChatList->GetCount();

				if(nCount>m_bySize*4+5)
				{
					// 번호 //					
					int		nIndex	= nCount-(m_bySize*4+5)+1;						
				
					m_nCurScrollPos	= nIndex-1;
					m_fSCTemp		= float((90+m_bySize*64)+32-16);
					SetPosObjY(SPR_OBJ_CHATLIST_SC, float((90+m_bySize*64)+32-16));
				}
				else
				{
					m_nCurScrollPos = 0;
					m_fSCTemp		= 32;
					SetPosObjY(SPR_OBJ_CHATLIST_SC, 32);
				}
				
				SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_FRIEND3, TRUE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);
			}		
			else if(eMouseCheck==MOUSE_OVER)
			{			
				if(m_byChatType!=CHAT_TYPE_FRIEND)
				{
					SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHATLIST_ALL1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_PARTY1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_GUILD1, TRUE);
					SetRender(SPR_OBJ_CHATLIST_FRIEND2, TRUE);
					SetRender(SPR_OBJ_CHATLIST_WHISPER1, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHATLIST_ALL3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHATLIST_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHATLIST_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHATLIST_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHATLIST_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHATLIST_FRIEND1, FALSE);
						break;
					}
					m_bChkBtn[1] = TRUE;					
				}
			}	
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 10:
		{						
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byScrollChk==1 || m_byScrollChk==2)
				{
					int nCount = 0;

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						nCount = g_pAllChatList->GetCount();
						break;
					case CHAT_TYPE_PARTY:
						nCount = g_pPartyChatList->GetCount();
						break;
					case CHAT_TYPE_GUILD:
						nCount = g_pGuildChatList->GetCount();
						break;
					case CHAT_TYPE_WHISPER:
						nCount = g_pWhisperChatList->GetCount();
						break;
					case CHAT_TYPE_FRIEND:
						nCount = g_pFriendChatList->GetCount();
						break;
					}

					if(nCount>m_bySize*4+5)
					{
						// 번호 //
						float	fPos		= g_Mouse.MousePos.y-(m_fPosZ+32);						
						int		nIndex		= nCount-(m_bySize*4+5)+1;						
						float	fCurPos		= (float)(90+m_bySize*64)/(float)nIndex;
						float	fCurIndex	= (fPos/fCurPos);

						if(fCurIndex==nIndex)
							m_nCurScrollPos = nIndex-1;
						else
							m_nCurScrollPos = (int)fCurIndex;
					}

					float fMousePos = (float)g_Mouse.MousePos.y;

					if(fMousePos<m_fPosZ+32)
						fMousePos = m_fPosZ+32;
					else if(fMousePos>=m_fPosZ+106+m_bySize*64)
						fMousePos = m_fPosZ+106+m_bySize*64;

					m_fSCTemp = fMousePos - m_fPosZ;

					SetPosObjY(SPR_OBJ_CHATLIST_SC, fMousePos - m_fPosZ);					
					m_byScrollChk	= 2;					
				}			
			}	
			SetMouseCheck(nRt, eMouseCheck, TRUE);
		}
		break;
	case 11:
		{						
			if(eMouseCheck==MOUSE_UP)
			{
				// 재배치 //
				int nCount = 0;
				
				switch(m_byChatType)
				{
				case CHAT_TYPE_ALL:
					nCount = g_pAllChatList->GetCount();
					break;
				case CHAT_TYPE_PARTY:
					nCount = g_pPartyChatList->GetCount();
					break;
				case CHAT_TYPE_GUILD:
					nCount = g_pGuildChatList->GetCount();
					break;
				case CHAT_TYPE_WHISPER:
					nCount = g_pWhisperChatList->GetCount();
					break;
				case CHAT_TYPE_FRIEND:
					nCount = g_pFriendChatList->GetCount();
					break;
				}

				if(nCount>m_bySize*4+5)
				{
					int		nIndex	= nCount-(m_bySize*4+5);
					float	fCurPos = (float)(90+m_bySize*64-16)/(float)nIndex;					
					
					if(m_nCurScrollPos>0)
					{
						m_nCurScrollPos	-=	1;
						m_fSCTemp		=	32+(fCurPos*m_nCurScrollPos);
						SetPosObjY(SPR_OBJ_CHATLIST_SC, 32+(fCurPos*m_nCurScrollPos));
					}
					else
					{
						m_fSCTemp =	32;
						SetPosObjY(SPR_OBJ_CHATLIST_SC, 32);
					}
				}
				SetRender(SPR_OBJ_CHATLIST_SC1, TRUE);
				SetRender(SPR_OBJ_CHATLIST_SC2, FALSE);
				m_bChkBtn[2] = FALSE;
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{											
				SetRender(SPR_OBJ_CHATLIST_SC1, FALSE);
				SetRender(SPR_OBJ_CHATLIST_SC2, TRUE);
				m_bChkBtn[2] = TRUE;				
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 12:
		{						
			if(eMouseCheck==MOUSE_UP)
			{	
				// 재배치 //
				int nCount = 0;
				
				switch(m_byChatType)
				{
				case CHAT_TYPE_ALL:
					nCount = g_pAllChatList->GetCount();
					break;
				case CHAT_TYPE_PARTY:
					nCount = g_pPartyChatList->GetCount();
					break;
				case CHAT_TYPE_GUILD:
					nCount = g_pGuildChatList->GetCount();
					break;
				case CHAT_TYPE_WHISPER:
					nCount = g_pWhisperChatList->GetCount();
					break;
				case CHAT_TYPE_FRIEND:
					nCount = g_pFriendChatList->GetCount();
					break;
				}

				if(nCount>m_bySize*4+5)
				{
					int		nIndex	= nCount-(m_bySize*4+5);
					float	fCurPos = (float)(90+m_bySize*64-16)/(float)nIndex;					
					
					if(m_nCurScrollPos<nIndex)
					{						
						m_nCurScrollPos	+=	1;
						m_fSCTemp	=	32+(fCurPos*m_nCurScrollPos);
						SetPosObjY(SPR_OBJ_CHATLIST_SC, 32+(fCurPos*m_nCurScrollPos));
					}
					else
					{
						m_fSCTemp =	float((32+(90+m_bySize*64-16)));
						SetPosObjY(SPR_OBJ_CHATLIST_SC, float(32+(90+m_bySize*64-16)));
					}
				}
				SetRender(SPR_OBJ_CHATLIST_SC3, TRUE);
				SetRender(SPR_OBJ_CHATLIST_SC4, FALSE);
				m_bChkBtn[3] = FALSE;
			}
			else if(g_Mouse.bLDown && !m_bUpChk)
			{							
				SetRender(SPR_OBJ_CHATLIST_SC3, FALSE);
				SetRender(SPR_OBJ_CHATLIST_SC4, TRUE);
				m_bChkBtn[3] = TRUE;				
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}				
	
	if(m_bScChk[0] && nRt!=4)
	{
		if(g_Mouse.bLDown)
		{			
			if(m_byScrollChk==1 || m_byScrollChk==2)
			{
				int nCount = 0;

				switch(m_byChatType)
				{
				case CHAT_TYPE_ALL:
					nCount = g_pAllChatList->GetCount();
					break;
				case CHAT_TYPE_PARTY:
					nCount = g_pPartyChatList->GetCount();
					break;
				case CHAT_TYPE_GUILD:
					nCount = g_pGuildChatList->GetCount();
					break;
				case CHAT_TYPE_WHISPER:
					nCount = g_pWhisperChatList->GetCount();
					break;
				case CHAT_TYPE_FRIEND:
					nCount = g_pFriendChatList->GetCount();
					break;
				}

				if(nCount>m_bySize*4+5)
				{
					// 번호 //
					int		nIndex		= nCount-(m_bySize*4+5)+1;
					float	fPos		= g_Mouse.MousePos.y-(m_fPosZ+32);

					if(fPos>(90+m_bySize*64))
						fPos = (float)(90+m_bySize*64);

					float	fCurPos		= (float)(90+m_bySize*64)/(float)nIndex;
					float	fCurIndex	= (fPos/fCurPos);

					if(fCurIndex==nIndex)
						m_nCurScrollPos = nIndex-1;
					else
						m_nCurScrollPos = (int)fCurIndex;
				}

				float fMousePos = (float)g_Mouse.MousePos.y;

				if(fMousePos<m_fPosZ+32)
					fMousePos = m_fPosZ+32;
				else if(fMousePos>=m_fPosZ+106+m_bySize*64)
					fMousePos = m_fPosZ+106+m_bySize*64;

				SetPosObjY(SPR_OBJ_CHATLIST_SC, fMousePos - m_fPosZ);

				m_fSCTemp = fMousePos - m_fPosZ;				
			}
			m_byScrollChk = 2;
		}
		m_bClick	= TRUE;
		m_bDown		= TRUE;
		m_bUpChk	= TRUE;
		return CInterface::GetInstance()->GetSetWnd();
	}

	if(MouseUp())
	{
		SetRender(SPR_OBJ_CHATLIST_CLOSE1, TRUE);
		SetRender(SPR_OBJ_CHATLIST_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt, m_bySize*64))	
		return CInterface::GetInstance()->GetSetWnd();

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

void CChatListWnd::SetScroll() 
{
	// 재배치 //
	int nCount = 0;
	
	switch(m_byChatType)
	{
	case CHAT_TYPE_ALL:
		nCount = g_pAllChatList->GetCount();
		break;
	case CHAT_TYPE_PARTY:
		nCount = g_pPartyChatList->GetCount();
		break;
	case CHAT_TYPE_GUILD:
		nCount = g_pGuildChatList->GetCount();
		break;
	case CHAT_TYPE_WHISPER:
		nCount = g_pWhisperChatList->GetCount();
		break;
	case CHAT_TYPE_FRIEND:
		nCount = g_pFriendChatList->GetCount();
		break;
	}

	if(nCount>m_bySize*4+5)
	{
		// 번호 //					
		int		nIndex	= nCount-(m_bySize*4+5)+1;						
	
		m_nCurScrollPos	= nIndex-1;
		m_fSCTemp		= float((90+m_bySize*64)+32-16);
		SetPosObjY(SPR_OBJ_CHATLIST_SC, float((90+m_bySize*64)+32-16));
	}
	else
	{
		m_nCurScrollPos		= 0;
		m_fSCTemp			= 32;
		SetPosObjY(SPR_OBJ_CHATLIST_SC, 32);
	}
}
//======================================================//
// End.													//
//======================================================//
