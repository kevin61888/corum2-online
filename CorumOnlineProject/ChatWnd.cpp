//======================================================//
// Code by Jang.							2002.1.8	//
//======================================================//
#include "ChatWnd.h"
#include "GameControl.h"
#include "ChatListWnd.h"
#include "DungeonProcess.h"
#include "UserInterface.h"
#include "Interface.h"
#include "QuantityWnd.h"
#include "Interface.h"
#include "Chat.h"
#include "message.h"
#include "ComboBox.h"
#include "InputManager.h"
#include "IMEEdit.h"
#include "ChatBackground.h"
#include "NetworkClient.h"
#include "Script.h"
#include "InitGame.h"
#include "CorumResource.h"
#include "CodeFun.h"


CChatWnd* CChatWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CChatWnd::CChatWnd()
{		
	m_byClipType	= 0;
	m_bChkBtn[0]	= FALSE;
	m_byChatMode	= CHAT_MODE_CHAT;
	m_byChatType	= CHAT_TYPE_ALL;	
	m_bChatList		= FALSE;
	m_bChkBtn[0]	= FALSE;
	m_bChkBtn[1]	= FALSE;
	m_bChkBtn[2]	= FALSE;
	m_rtPos.left	= 370;
	m_rtPos.right	= 700;
	m_rtPos.top		= 757;
	m_rtPos.bottom	= 771;
	
	if(GetGameStatus()==UPDATE_GAME_PLAY)
	{
		RECT rt;
		rt.left = 443;	rt.right = 573;	rt.top = 737;	rt.bottom = 751;
		g_pInputManager->InitializeInput(INPUT_BUFFER_6, FONT_SS3D_GAME, rt, 0xffffffff, GetStartOrder()+2);
		
	}		
	
	memset(m_szName, 0, sizeof(m_szName));
}

CChatWnd::~CChatWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CChatWnd::Init()
{
	
	InsertData(SPR_OBJ_CHAT_WND, SPR_INTERFACE_CHAT_WND, 257, 704, 1.0f, 1.0f, 0, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_CHAT1, SPR_INTERFACE_CHAT_CHAT1, 375, 735, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_CHAT2, SPR_INTERFACE_CHAT_CHAT2, 375, 735, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_MESSAGE1, SPR_INTERFACE_CHAT_MESSAGE1, 375, 735, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_MESSAGE2, SPR_INTERFACE_CHAT_MESSAGE2, 375, 735, 1.0f, 1.0f, 1, FALSE, FALSE);		
	InsertData(SPR_OBJ_CHAT_ALL1, SPR_INTERFACE_CHATLIST_ALL1, 398, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_ALL2, SPR_INTERFACE_CHATLIST_ALL2, 398, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_ALL3, SPR_INTERFACE_CHATLIST_ALL3, 398, 715, 1.0f, 1.0f, 1, FALSE, FALSE);	
	InsertData(SPR_OBJ_CHAT_PARTY1, SPR_INTERFACE_CHATLIST_PARTY1, 444, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_PARTY2, SPR_INTERFACE_CHATLIST_PARTY2, 444, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_PARTY3, SPR_INTERFACE_CHATLIST_PARTY3, 444, 715, 1.0f, 1.0f, 1, FALSE, FALSE);	
	InsertData(SPR_OBJ_CHAT_GUILD1, SPR_INTERFACE_CHATLIST_GUILD1, 490, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_GUILD2, SPR_INTERFACE_CHATLIST_GUILD2, 490, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_GUILD3, SPR_INTERFACE_CHATLIST_GUILD3, 490, 715, 1.0f, 1.0f, 1, FALSE, FALSE);	
	InsertData(SPR_OBJ_CHAT_WHISPER1, SPR_INTERFACE_CHATLIST_WHISPER1, 536, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_WHISPER2, SPR_INTERFACE_CHATLIST_WHISPER2, 536, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_WHISPER3, SPR_INTERFACE_CHATLIST_WHISPER3, 536, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_FRIEND1, SPR_INTERFACE_CHATLIST_FRIEND1, 582, 715, .0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_FRIEND2, SPR_INTERFACE_CHATLIST_FRIEND2, 582, 715, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_FRIEND3, SPR_INTERFACE_CHATLIST_FRIEND3, 582, 715, 1.0f, 1.0f, 1, FALSE, FALSE);		
	InsertData(SPR_OBJ_CHAT_LIST1, SPR_INTERFACE_CHAT_LIST1, 586, 735, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_LIST2, SPR_INTERFACE_CHAT_LIST2, 586, 735, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_LIST3, SPR_INTERFACE_CHAT_LIST3, 586, 735, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_CLOSE1, SPR_INTERFACE_CLOSE1, 670, 709, 1.0f, 1.0f, 1, FALSE, FALSE);
	InsertData(SPR_OBJ_CHAT_CLOSE2, SPR_INTERFACE_CLOSE2, 670, 709, 1.0f, 1.0f, 1, FALSE, FALSE);

	InsertCheckInterface(338, 706, 685, 767, 1, CHECK_NONE);
	InsertCheckInterface(375, 735, 439, 750, 2, CHECK_BTN);
	InsertCheckInterface(398, 715, 442, 731, 3, CHECK_BTN);
	InsertCheckInterface(444, 715, 486, 731, 4, CHECK_BTN);
	InsertCheckInterface(490, 715, 534, 731, 5, CHECK_BTN);
	InsertCheckInterface(536, 715, 580, 731, 6, CHECK_BTN);
	InsertCheckInterface(582, 715, 626, 731, 7, CHECK_BTN);
	InsertCheckInterface(586, 735, 650, 751, 8, CHECK_BTN);
	InsertCheckInterface(670, 709, 683, 722, 9, CHECK_CLOSE);
	InsertCheckInterface(441, 735, 573, 751, 10, CHECK_BTN);	
	
	int nOrder = GetStartOrder();		
		
	m_bInit = TRUE;

	RECT rt;
	rt.left = 443;	rt.right = 573;	rt.top = 737;	rt.bottom = 751;
	g_pInputManager->InitializeInput(INPUT_BUFFER_6, FONT_SS3D_GAME, rt, 0xffffffff, nOrder+2);

	return TRUE;
}

void CChatWnd::SetName(char* pszName, BOOL bChk)
{
	__lstrcpyn(m_szName, pszName, MAX_CHARACTER_NAME_REAL_LENGTH);
	
	if(bChk)		
		g_pInputManager->SetInput(INPUT_BUFFER_6, m_szName);		
}

void CChatWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CChatWnd::SetOrder()
{	
	g_pInputManager->SetInputOrder(INPUT_BUFFER_6, GetStartOrder()+2);
}

/*
void CChatWnd::RenderText()
{	
	int nLeft = 0;

	if(m_byRenderMode>=3 && m_byRenderMode<=7)
	{		
#if IS_JAPAN_LOCALIZING()

		switch(m_byRenderMode)
		{
		case 3:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 450;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE487].szMessage); // "모두와 대화합니다."
				}
				else
				{
					nLeft	= 430;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE488].szMessage); // "모두에게 메세지를 보냅니다."
				}					
			}
			break;
		case 4:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 420;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE489].szMessage); // "파티원과 대화합니다.");
				}
				else
				{
					nLeft	= 410;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE490].szMessage); // "파티원에게 메세지를 보냅니다.");
				}
			}
			break;
		case 5:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 425;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE491].szMessage); // "접속한 길드원과 대화합니다.");
				}
				else
				{
					nLeft	= 410;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE492].szMessage); // "접속한 길드원에게 메세지를 보냅니다.");
				}
			}
			break;
		case 6:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 435;
					__lstrcpyn(m_szInfo, g_Message[ETC_MESSAGE493].szMessage, lstrlen(g_Message[ETC_MESSAGE493].szMessage));// "귓말을 보낼 대상을 클릭, ID를 입력하세요.");
				}
				else
				{
					nLeft	= 420;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE264].szMessage);//"귓말을 보낼 대상을 클릭, ID를 입력하세요.";
				}
			}
			break;
		case 7:
			{
				nLeft	= 450;
				lstrcpy(m_szInfo, g_Message[ETC_MESSAGE494].szMessage); //"예정중입니다.");				
			}
			break;
		}
#else		
		switch(m_byRenderMode)
		{
		case 3:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 455;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE487].szMessage); // "모두와 대화합니다."
				}
				else
				{
					nLeft	= 425;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE488].szMessage); // "모두에게 메세지를 보냅니다."
				}					
			}
			break;
		case 4:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 447;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE489].szMessage);
				}
				else
				{
					nLeft	= 415;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE490].szMessage); // "파티원에게 메세지를 보냅니다.");
				}
			}
			break;
		case 5:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 425;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE491].szMessage); // "접속한 길드원과 대화합니다.");
				}
				else
				{
					nLeft	= 395;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE492].szMessage); // "접속한 길드원에게 메세지를 보냅니다.");
				}
			}
			break;
		case 6:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 385;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE493].szMessage);// "귓말을 보낼 대상을 클릭, ID를 입력하세요.");
				}
				else
				{
					nLeft	= 385;
					lstrcpy(m_szInfo, g_Message[ETC_MESSAGE493].szMessage);//"귓말을 보낼 대상을 클릭, ID를 입력하세요.");
				}
			}
			break;
		case 7:
			{
				nLeft	= 470;
				lstrcpy(m_szInfo, g_Message[ETC_MESSAGE494].szMessage); //"예정중입니다.");				
			}
			break;
		}
#endif		
		RenderFont(m_szInfo, nLeft, 573, 737, 751, GetStartOrder()+2);
	}
}
 */

void CChatWnd::RenderText()
{	
	int nLeft = 0;
	DWORD dwMsgId = 0;

	if(m_byRenderMode>=3 && m_byRenderMode<=7)
	{		
#if IS_JAPAN_LOCALIZING()

		switch(m_byRenderMode)
		{
		case 3:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 450;
					dwMsgId = ETC_MESSAGE487; // // "모두와 대화합니다."
				}
				else
				{
					nLeft	= 430;
					dwMsgId = ETC_MESSAGE488;// "모두에게 메세지를 보냅니다."
				}					
			}
			break;
		case 4:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 420;
					dwMsgId = ETC_MESSAGE489;// "파티원과 대화합니다.");
				}
				else
				{
					nLeft	= 410;
					dwMsgId = ETC_MESSAGE490;// "파티원에게 메세지를 보냅니다.");
				}
			}
			break;
		case 5:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 425;
					dwMsgId = ETC_MESSAGE491;// "접속한 길드원과 대화합니다.");
				}
				else
				{
					nLeft	= 410;
					dwMsgId = ETC_MESSAGE492;// "접속한 길드원에게 메세지를 보냅니다.");
				}
			}
			break;
		case 6:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 435;
					dwMsgId = ETC_MESSAGE493;// "귓말을 보낼 대상을 클릭, ID를 입력하세요.");
				}
				else
				{
					nLeft	= 420;
					dwMsgId = ETC_MESSAGE264;//"귓말을 보낼 대상을 클릭, ID를 입력하세요.";
				}
			}
			break;
		case 7:
			{
				nLeft	= 450;
				dwMsgId = ETC_MESSAGE494;//"예정중입니다.");	
			}
			break;
		}
#else		
		switch(m_byRenderMode)
		{
		case 3:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 455; 
					dwMsgId = ETC_MESSAGE487;  // "모두와 대화합니다."
				}
				else
				{
					nLeft	= 425;
					dwMsgId = ETC_MESSAGE488; // "모두에게 메세지를 보냅니다."
				}					
			}
			break;
		case 4:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 447;
					dwMsgId = ETC_MESSAGE489;
				}
				else
				{
					nLeft	= 415;
					dwMsgId = ETC_MESSAGE490; // "파티원에게 메세지를 보냅니다."
				}
			}
			break;
		case 5:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 425;
					dwMsgId = ETC_MESSAGE491; // "접속한 길드원과 대화합니다."
				}
				else
				{
					nLeft	= 395;
					dwMsgId = ETC_MESSAGE492; // "접속한 길드원에게 메세지를 보냅니다."
				}
			}
			break;
		case 6:
			{
				if(m_byChatMode==CHAT_MODE_CHAT)
				{
					nLeft	= 385;
					dwMsgId = ETC_MESSAGE493; // "귓말을 보낼 대상을 클릭, ID를 입력하세요."
				}
				else
				{
					nLeft	= 385;
					dwMsgId = ETC_MESSAGE493; //"귓말을 보낼 대상을 클릭, ID를 입력하세요."
				}
			}
			break;
		case 7:
			{
				nLeft	= 470;
				dwMsgId = ETC_MESSAGE494; //"예정중입니다."
			}
			break;
		}
#endif		
		RenderFont(g_Message[dwMsgId].szMessage, nLeft, 573, 737, 751, GetStartOrder()+2);
	}
}

void CChatWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	   
	if(bActive==FALSE)
	{
		if (g_pInputManager->GetCurFocusInputID()==INPUT_BUFFER_6)
		{
			g_pInputManager->ClearInput(INPUT_BUFFER_6);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_19);
		}
		
		m_bSoundChk	= FALSE;
		ShowSpriteAll();

		IMESetEdit(0, TRUE, TRUE);

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		if(m_byChatMode==CHAT_MODE_MESSAGE)
			SetRender(SPR_OBJ_CHAT_MESSAGE1, TRUE);
		else
			SetRender(SPR_OBJ_CHAT_CHAT1, TRUE);

		SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
		SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
		SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
		SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
		SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
		SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
		SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
		SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
		SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
		SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
		SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
		SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);
		SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
		SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
		SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);		
		SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
		SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
		SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
		SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
		SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);
		SetRender(SPR_OBJ_CHAT_LIST1, TRUE);
//		SetRender(SPR_OBJ_CHAT_CLOSE1, TRUE);

		switch(m_byChatType)
		{
		case CHAT_TYPE_ALL:
			SetRender(SPR_OBJ_CHAT_ALL3, TRUE);
			SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
			break;
		case CHAT_TYPE_PARTY:
			SetRender(SPR_OBJ_CHAT_PARTY3, TRUE);
			SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
			break;
		case CHAT_TYPE_GUILD:
			SetRender(SPR_OBJ_CHAT_GUILD3, TRUE);
			SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
			break;
		case CHAT_TYPE_WHISPER:
			SetRender(SPR_OBJ_CHAT_WHISPER3, TRUE);
			SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
			break;
		case CHAT_TYPE_FRIEND:
			SetRender(SPR_OBJ_CHAT_FRIEND3, TRUE);
			SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
			break;
		}
		SetRender(SPR_OBJ_CHAT_WND, TRUE);
		g_nChatModeExt = 1;
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);

		// User Name //
		if(!IsEmptyString(m_szName))
		{
			g_pInputManager->ClearInput(INPUT_BUFFER_6);
			g_pInputManager->InsertCharacter(INPUT_BUFFER_6, m_szName, lstrlen(m_szName));
		}
	}
}

int CChatWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk(0, TRUE);

	if(m_bChkBtn[2]==TRUE && nRt!=8)
	{
		SetRender(SPR_OBJ_CHAT_LIST1, TRUE);
		SetRender(SPR_OBJ_CHAT_LIST2, FALSE);
		SetRender(SPR_OBJ_CHAT_LIST3, FALSE);
		m_bChkBtn[2] = FALSE;
	}				

	if(m_bChkBtn[0]==TRUE && nRt!=2)
	{
		if(m_byChatMode==CHAT_MODE_MESSAGE)
		{
			if(g_Mouse.bLDown)
			{
				SetRender(SPR_OBJ_CHAT_MESSAGE1, FALSE);
				SetRender(SPR_OBJ_CHAT_MESSAGE2, TRUE);
			}
			else
			{
				SetRender(SPR_OBJ_CHAT_MESSAGE1, TRUE);
				SetRender(SPR_OBJ_CHAT_MESSAGE2, FALSE);
			}
			SetRender(SPR_OBJ_CHAT_CHAT1, FALSE);
			SetRender(SPR_OBJ_CHAT_CHAT2, FALSE);
		}
		else
		{
			if(g_Mouse.bLDown)
			{
				SetRender(SPR_OBJ_CHAT_CHAT1, FALSE);
				SetRender(SPR_OBJ_CHAT_CHAT2, TRUE);
			}
			else
			{
				SetRender(SPR_OBJ_CHAT_CHAT1, TRUE);
				SetRender(SPR_OBJ_CHAT_CHAT2, FALSE);
			}
			SetRender(SPR_OBJ_CHAT_MESSAGE1, FALSE);
			SetRender(SPR_OBJ_CHAT_MESSAGE2, FALSE);			
		}
		m_bChkBtn[0] = FALSE;
	}


	if(m_bChkBtn[1]==TRUE && !(nRt>=3 && nRt<=7))
	{
		SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
		SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
		SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
		SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
		SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
		SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
		SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
		SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
		SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
		SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
		SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
		SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
		SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
		SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
		SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);

		SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
		SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
		SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
		SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
		SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);

		switch(m_byChatType)
		{
		case CHAT_TYPE_ALL:
			SetRender(SPR_OBJ_CHAT_ALL3, TRUE);
			SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
			break;
		case CHAT_TYPE_PARTY:
			SetRender(SPR_OBJ_CHAT_PARTY3, TRUE);
			SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
			break;
		case CHAT_TYPE_GUILD:
			SetRender(SPR_OBJ_CHAT_GUILD3, TRUE);
			SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
			break;
		case CHAT_TYPE_WHISPER:
			SetRender(SPR_OBJ_CHAT_WHISPER3, TRUE);
			SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
			break;
		case CHAT_TYPE_FRIEND:
			SetRender(SPR_OBJ_CHAT_FRIEND3, TRUE);
			SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
			break;
		}
		m_bChkBtn[1] = FALSE;
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
			if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byChatMode==CHAT_MODE_MESSAGE)
				{
					m_byChatMode = CHAT_MODE_CHAT;
					SetRender(SPR_OBJ_CHAT_CHAT1, FALSE);
					SetRender(SPR_OBJ_CHAT_CHAT2, TRUE);
					SetRender(SPR_OBJ_CHAT_MESSAGE1, FALSE);
					SetRender(SPR_OBJ_CHAT_MESSAGE2, FALSE);
				}
				else if(m_byChatMode==CHAT_MODE_CHAT)
				{
					m_byChatMode = CHAT_MODE_MESSAGE;
					SetRender(SPR_OBJ_CHAT_CHAT1, FALSE);
					SetRender(SPR_OBJ_CHAT_CHAT2, FALSE);
					SetRender(SPR_OBJ_CHAT_MESSAGE1, FALSE);
					SetRender(SPR_OBJ_CHAT_MESSAGE2, TRUE);
				}
				m_bChkBtn[0]	= TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byChatMode==CHAT_MODE_MESSAGE)
				{
					SetRender(SPR_OBJ_CHAT_CHAT1, FALSE);
					SetRender(SPR_OBJ_CHAT_CHAT2, FALSE);
					SetRender(SPR_OBJ_CHAT_MESSAGE1, FALSE);
					SetRender(SPR_OBJ_CHAT_MESSAGE2, TRUE);
				}
				else
				{
					SetRender(SPR_OBJ_CHAT_CHAT1, FALSE);
					SetRender(SPR_OBJ_CHAT_CHAT2, TRUE);
					SetRender(SPR_OBJ_CHAT_MESSAGE1, FALSE);
					SetRender(SPR_OBJ_CHAT_MESSAGE2, FALSE);
				}
				m_bChkBtn[0] = TRUE;
			}		
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_byChatType = CHAT_TYPE_ALL;
			
				SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
				SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
				SetRender(SPR_OBJ_CHAT_ALL3, TRUE);
				SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
				SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
				SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
				SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);
				SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);						
								
				m_bChkBtn[1]	= TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byChatType!=CHAT_TYPE_ALL)
				{	
					SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL2, TRUE);
					SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
					SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHAT_ALL3, TRUE);
						SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHAT_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHAT_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHAT_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHAT_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
						break;
					}		
					m_bChkBtn[1] = TRUE;
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 4:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChkBtn[1]	= TRUE;
				m_byChatType	= CHAT_TYPE_PARTY;
				SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
				SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
				SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY3, TRUE);
				SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
				SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
				SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);
				SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byChatType!=CHAT_TYPE_PARTY)
				{
					SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
					SetRender(SPR_OBJ_CHAT_PARTY2, TRUE);
					SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHAT_ALL3, TRUE);
						SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHAT_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHAT_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHAT_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHAT_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
						break;
					}
					m_bChkBtn[1] = TRUE;
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_DOWN)
			{
				m_bChkBtn[1] = TRUE;
				m_byChatType = CHAT_TYPE_GUILD;
				SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
				SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
				SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
				SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD3, TRUE);
				SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
				SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);
				SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);					
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byChatType!=CHAT_TYPE_GUILD)
				{
					SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
					SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
					SetRender(SPR_OBJ_CHAT_GUILD2, TRUE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHAT_ALL3, TRUE);
						SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHAT_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHAT_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHAT_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHAT_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
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
				m_byChatType = CHAT_TYPE_WHISPER;
				m_bChkBtn[1] = TRUE;
				SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
				SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
				SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
				SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
				SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER3, TRUE);
				SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
				SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);									
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byChatType!=CHAT_TYPE_WHISPER)
				{
					SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
					SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
					SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, TRUE);
					SetRender(SPR_OBJ_CHAT_WHISPER2, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHAT_ALL3, TRUE);
						SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHAT_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHAT_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHAT_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHAT_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
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
				m_byChatType = CHAT_TYPE_FRIEND;
				m_bChkBtn[1] = TRUE;
				SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
				SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
				SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
				SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
				SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
				SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
				SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
				SetRender(SPR_OBJ_CHAT_FRIEND3, TRUE);
				SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);
				SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
				SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);					
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byChatType!=CHAT_TYPE_FRIEND)
				{
					SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL2, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL3, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY2, FALSE);
					SetRender(SPR_OBJ_CHAT_PARTY3, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD2, FALSE);
					SetRender(SPR_OBJ_CHAT_GUILD3, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND2, FALSE);
					SetRender(SPR_OBJ_CHAT_FRIEND3, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER2, FALSE);
					SetRender(SPR_OBJ_CHAT_WHISPER3, FALSE);
					SetRender(SPR_OBJ_CHAT_ALL1, TRUE);
					SetRender(SPR_OBJ_CHAT_PARTY1, TRUE);
					SetRender(SPR_OBJ_CHAT_GUILD1, TRUE);
					SetRender(SPR_OBJ_CHAT_FRIEND2, TRUE);
					SetRender(SPR_OBJ_CHAT_WHISPER1, TRUE);

					switch(m_byChatType)
					{
					case CHAT_TYPE_ALL:
						SetRender(SPR_OBJ_CHAT_ALL3, TRUE);
						SetRender(SPR_OBJ_CHAT_ALL1, FALSE);
						break;
					case CHAT_TYPE_PARTY:
						SetRender(SPR_OBJ_CHAT_PARTY3, TRUE);
						SetRender(SPR_OBJ_CHAT_PARTY1, FALSE);
						break;
					case CHAT_TYPE_GUILD:
						SetRender(SPR_OBJ_CHAT_GUILD3, TRUE);
						SetRender(SPR_OBJ_CHAT_GUILD1, FALSE);
						break;
					case CHAT_TYPE_WHISPER:
						SetRender(SPR_OBJ_CHAT_WHISPER3, TRUE);
						SetRender(SPR_OBJ_CHAT_WHISPER1, FALSE);
						break;
					case CHAT_TYPE_FRIEND:
						SetRender(SPR_OBJ_CHAT_FRIEND3, TRUE);
						SetRender(SPR_OBJ_CHAT_FRIEND1, FALSE);
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
			if(eMouseCheck==MOUSE_UP)
			{
				SetRender(SPR_OBJ_CHAT_LIST1, FALSE);
				SetRender(SPR_OBJ_CHAT_LIST2, TRUE);
				SetRender(SPR_OBJ_CHAT_LIST3, FALSE);
				m_bChkBtn[2] = TRUE;
			
				// 채팅 리스트 윈도우 //
				CChatListWnd* pChatListWnd = CChatListWnd::GetInstance();

				if(pChatListWnd->GetActive())
					pChatListWnd->SetActive(FALSE);
				else
					pChatListWnd->SetActive();
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_CHAT_LIST1, FALSE);
				SetRender(SPR_OBJ_CHAT_LIST2, FALSE);
				SetRender(SPR_OBJ_CHAT_LIST3, TRUE);
				m_bChkBtn[2] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_CHAT_LIST1, FALSE);
				SetRender(SPR_OBJ_CHAT_LIST2, TRUE);
				SetRender(SPR_OBJ_CHAT_LIST3, FALSE);
				m_bChkBtn[2] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 9:
		{
/*			if(eMouseCheck==MOUSE_UP)
			{
				SetActive(FALSE);
				ChatOff();
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_CHAT_CLOSE1, FALSE);
				SetRender(SPR_OBJ_CHAT_CLOSE2, TRUE);
			}			
			SetMouseCheck(nRt, eMouseCheck);*/
		}
		break;
	case 10:
		{
			if(eMouseCheck==MOUSE_UP)
			{	
#ifdef _USE_IME
				
				if(GET_IMEEDIT()->GetEditIndex()!=2)
				{
					POINT pChatPoint;
					pChatPoint.x	= 445;
					pChatPoint.y	= 737;
	
					GET_IMEEDIT()->SetEditIndex(2);				
					GET_IMEEDIT()->SetImeEditString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_6));	
					GET_IMEEDIT()->ActivateIME(pChatPoint, __MAX_IDBUUFER_SIZE__);
				}				
#endif				
				// 포커스 //
				if(g_pInputManager->GetInputBufferLength(INPUT_BUFFER_6)==0)	
					InputChatName();
				else
					InputChatName(TRUE);

				g_nChatModeExt = 1;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}	
		
	if(MouseUp())
	{
//		SetRender(SPR_OBJ_CHAT_CLOSE1, TRUE);
//		SetRender(SPR_OBJ_CHAT_CLOSE2, FALSE);		
	}

	if(MouseChk(nRt))
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}

void CChatWnd::ChatDefault(char* pszText)
{
	CTDS_DUNGEON_CHAT pChatPacket;
	pChatPacket.bChatType		= 1;
	pChatPacket.bChatStrLength	= (BYTE)lstrlen(pszText);
	lstrcpy(pChatPacket.szChatStr, pszText);
	g_pNet->SendMsg((char*)&pChatPacket, pChatPacket.GetPacketSize(), SERVER_INDEX_ZONE);	
}

void CChatWnd::ChatParty(char* pszText)
{
	CTWS_DGCHAT_USER pChatPacket;
	memset(pChatPacket.szMessage, 0, sizeof(pChatPacket.szMessage));
	memset(pChatPacket.szDstId, 0, sizeof(pChatPacket.szDstId));
	memset(pChatPacket.szId, 0, sizeof(pChatPacket.szId));

	if(g_pMainPlayer->m_dwPartyId!=0)
	{		
		pChatPacket.nIndex = 3;				
		__lstrcpyn(pChatPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		lstrcpy(pChatPacket.szMessage, pszText);				
		g_pNet->SendMsg((char*)&pChatPacket, pChatPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}
	else
	{
		ErrMessage(g_Message[ETC_MESSAGE69].szMessage, 0xffffff00, DEF_CHAT_TYPE_PARTY);		

		char*	szTemp;
				
		szTemp	= g_Message[ETC_MESSAGE69].szMessage;	
		
		ChatListAdd(szTemp, 0xffffff00, DEF_CHAT_TYPE_PARTY);
	}
}

void CChatWnd::ChatFriend(char* pszText)
{
	CTWS_DGCHAT_USER pChatPacket;

	if(g_pMessengerOnLineHash->GetCount()>0)
	{
		pChatPacket.nIndex = 4;
		memset(pChatPacket.szId, 0, sizeof(pChatPacket.szId));
		memset(pChatPacket.szDstId, 0, sizeof(pChatPacket.szDstId));
		memset(pChatPacket.szMessage, 0, sizeof(pChatPacket.szMessage));
		__lstrcpyn(pChatPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		lstrcpy(pChatPacket.szMessage, pszText);
		g_pNet->SendMsg((char*)&pChatPacket, pChatPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}
	else
	{
		char*	szTemp=0;
		char	szInfo[0xff] = {0,};
//		int		nIndex	= 0;
		
		lstrcpy(szInfo, g_Message[ETC_MESSAGE250].szMessage); // "온라인 중인 친구가 없습니다."
		ErrMessage(szInfo, 0xff92D5FF, DEF_CHAT_TYPE_FRIEND);
	}
}

void CChatWnd::ChatGuild(char* pszText)
{
	CTWS_DGCHAT_USER pChatPacket;

	if(g_pMainPlayer->m_dwGuildId!=0 && g_pGuildUserHash->GetCount()!=0)
	{
		pChatPacket.nIndex = 2;
		memset(pChatPacket.szId, 0, sizeof(pChatPacket.szId));
		memset(pChatPacket.szDstId, 0, sizeof(pChatPacket.szDstId));
		memset(pChatPacket.szMessage, 0, sizeof(pChatPacket.szMessage));
		__lstrcpyn(pChatPacket.szId, g_pMainPlayer->m_szName, MAX_CHARACTER_NAME_REAL_LENGTH);
		lstrcpy(pChatPacket.szMessage, pszText);
		g_pNet->SendMsg((char*)&pChatPacket, pChatPacket.GetPacketSize(), SERVER_INDEX_WORLD);
	}
	else
	{
		char*	szTemp;				
		int		nIndex	= 0;

		if(g_pMainPlayer->m_dwGuildId==0)
		{
			ErrMessage(g_Message[ETC_MESSAGE67].szMessage, 0xff00ffff, DEF_CHAT_TYPE_GUILD);
			nIndex = ETC_MESSAGE67;
		}
		else if(g_pGuildUserHash->GetCount()==0)
		{
			ErrMessage(g_Message[ETC_MESSAGE68].szMessage, 0xff00ffff, DEF_CHAT_TYPE_GUILD);
			nIndex = ETC_MESSAGE68;
		}

		szTemp	= g_Message[nIndex].szMessage;	
				
		ChatListAdd(szTemp, 0xff00ffff, DEF_CHAT_TYPE_GUILD);
	}
}

void CChatWnd::ChatWhisper(char* pszText, CScript* pScript)
{
	// 귓말 잘못 보내는 버그 수정 
	// 2005.01.30 김영대 
	// strcmp --> stricmp 로 변경 
	if(__stricmp(g_pMainPlayer->m_szName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_6))==0)
	{
		ErrMessage(g_Message[ETC_MESSAGE64].szMessage, 0xFFFFB894, DEF_CHAT_TYPE_WHISPER);
	}
	else
	{	
		char	szInfo[0xff] = {0,};
		char	szText[0xff] = {0,};																										
		char	szText2[0xff] = {0,};
		char*	szToken = NULL;
		char	szSeps[]	= "',?";

		// Id 검색 //
		memset(szText, 0, sizeof(szText));
		memset(szText2, 0, sizeof(szText2));

		__lstrcpyn(szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_6), MAX_CHARACTER_NAME_REAL_LENGTH);
		__lstrcpyn(szText2, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)));

		szToken = strtok(szText, szSeps);

		if(szToken)
		{													
			// 귓말 잘못 보내는 버그 수정 
			// 2005.01.30 김영대 
			// strcmp --> stricmp 로 변경 
			if(__stricmp(szToken, g_pInputManager->GetInputBuffer(INPUT_BUFFER_6))!=0)
			{
				ErrMessage(g_Message[ETC_MESSAGE57].szMessage, 0xffff9a63, DEF_CHAT_TYPE_WHISPER);

				g_pInputManager->ClearInput(INPUT_BUFFER_19);
				g_pInputManager->SetFocusInput(INPUT_BUFFER_6);
				
				return;
			}
		}
		else
		{
			if(!IsEmptyString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_6)))
			{
				ErrMessage(g_Message[ETC_MESSAGE57].szMessage, 0xffff9a63, DEF_CHAT_TYPE_WHISPER);
				g_pInputManager->ClearInput(INPUT_BUFFER_6);
				g_pInputManager->SetFocusInput(INPUT_BUFFER_19);
			}
			else
			{
				ErrMessage(g_Message[ETC_MESSAGE66].szMessage, 0xffff9a63, DEF_CHAT_TYPE_WHISPER);
				g_pInputManager->ClearInput(INPUT_BUFFER_6);
				g_pInputManager->SetFocusInput(INPUT_BUFFER_19);
				
			}
			return;
		}

		WSTC_DS_CHAT packet;		
		memset(packet.cScript.szId, 0, sizeof(packet.cScript.szId));
		memset(packet.cScript.szMessage, 0, sizeof(packet.cScript.szMessage));
		memset(packet.cScript.szText, 0, sizeof(packet.cScript.szText));

		szToken = strtok(szText2, szSeps);
		memset(&packet.cScript, 0, sizeof(packet.cScript));

		if(szToken)
		{
			// 귓말 잘못 보내는 버그 수정 
			// 2005.01.30 김영대 
			// strcmp --> stricmp 로 변경 
			if(__stricmp(szToken, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19))==0)
			{
				__lstrcpyn(szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)));
				__lstrcpyn(packet.cScript.szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)));
			}
			else
			{
				memset(szText, 0, sizeof(szText));

				lstrcat(szText, szToken);
				lstrcat(szText, " ");

				while(szToken)
				{
					szToken = strtok(NULL, szSeps);

					if(szToken)
					{
						lstrcat(szText, szToken);
						lstrcat(szText, " ");
					}												
				}
				__lstrcpyn(packet.cScript.szText, szText, sizeof(szText));
			}													
		}													
		packet.cScript.enCommand = COMMAND_WHISPER;
		__lstrcpyn(pScript->m_szName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_6), MAX_CHARACTER_NAME_REAL_LENGTH);
		__lstrcpyn(packet.cScript.szId, g_pInputManager->GetInputBuffer(INPUT_BUFFER_6), MAX_ID_REAL_LENGTH);
		__lstrcpyn(packet.cScript.szMessage, g_pInputManager->GetInputBuffer(INPUT_BUFFER_19), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_19)));
		g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD);
		
		wsprintf(szInfo, "%s : %s", g_pMainPlayer->m_szName, packet.cScript.szMessage);
		
		DisplayMessageAdd(szInfo, 0xffff9a63, TRUE, DEF_CHAT_TYPE_WHISPER);		
	}	
}
//======================================================//
// End.													//
//======================================================//