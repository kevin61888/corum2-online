//==========================================================//
// Code by Jang.								2003.04.08	//
// Object : Guild Ex.										//
//==========================================================//
#include "GuildExWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "Define.h"
#include "DungeonProcess.h"
#include "UserInterface.h"
#include "Interface.h"
#include "Filter.h"
#include "message.h"
#include "Chat.h"
#include "InputManager.h"
#include "IMEEdit.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "ItemNative.h"


CGuildExWnd* CGuildExWnd::c_pThis = NULL;

//==========================================================//
// Construction/Destruction.								//
//==========================================================//
CGuildExWnd::CGuildExWnd()
{		
	m_byChk	 = 0;
}

CGuildExWnd::~CGuildExWnd()
{
}
//==========================================================//
// Class Member Function.									//
//==========================================================//
BOOL CGuildExWnd::Init()
{	
/*
	InsertData(SPR_OBJ_GUILDEX_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILDEX_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDEX_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILDEX_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEX_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEX_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEX_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEX_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILDEX_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildExWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildExWnd::SetOrder()
{
	g_pInputManager->SetInputOrder(INPUT_BUFFER_7, GetStartOrder()+2);

#ifdef	_USE_IME 
		
	if(GetActive())
		GET_IMEEDIT()->SetOrder(GetStartOrder()+2);

#endif
}

void CGuildExWnd::RenderText()
{	
	if(m_byGuildType==__GCTYPE_GUILD__)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE285].szMessage);	// MSG_ID : 285 ; 길드
	else if(m_byGuildType==__GCTYPE_CLEN__)	
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE286].szMessage);	// MSG_ID : 286 ; 클랜

	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+55, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);

	if(m_byGuildType==__GCTYPE_GUILD__)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE287].szMessage);		// MSG_ID : 287 ; 길드 이름을 입력하세요.
	else if(m_byGuildType==__GCTYPE_CLEN__)
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE288].szMessage);		// MSG_ID : 298	; 클랜 이름을 입력하세요.

	RenderFont(m_szInfo, m_fPosX+30, m_fPosX+200, m_fPosZ+46, m_fPosZ+60, GetStartOrder()+1);
}

void CGuildExWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();		
		ChatModeOff(INPUT_BUFFER_7);

		if(m_byChk==1)
		{						
			CTDS_ITEM_PICKUP ItemPickup;
			ItemPickup.bSectionNum	= 1;
			ItemPickup.i64ItemID	= 0;
			SetItemPacket(&ItemPickup, 70, m_byGuildType, 0, 0, 0);
			g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
		}
		m_byChk = 0;

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);

		IMESetEdit(0);
	}	
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDEX_WND, TRUE);
		SetRender(SPR_OBJ_GUILDEX_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDEX_CANCEL1, TRUE);		
		SetRender(SPR_OBJ_GUILDEX_CLOSE1, TRUE);
			
		RECT rt;
		rt.left = (LONG)m_fPosX+135;	rt.right = (LONG)m_fPosX+256;
		rt.top = (LONG)m_fPosZ+70;		rt.bottom = (LONG)m_fPosZ+84;
		g_pInputManager->InitializeInput(INPUT_BUFFER_7, FONT_SS3D_GAME, rt, 0xffffffff, GetStartOrder()+2);
		g_pInputManager->SetInputOrder(INPUT_BUFFER_7, GetStartOrder()+2);
		g_nChatModeExt = 1;
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildExWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILDEX_OK1, TRUE);
		SetRender(SPR_OBJ_GUILDEX_OK2, FALSE);
		SetRender(SPR_OBJ_GUILDEX_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILDEX_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILDEX_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILDEX_CANCEL3, FALSE);
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
				if(m_byChk==1)
				{
					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;
					SetItemPacket(&ItemPickup, 70, m_byGuildType, 0, 0, 0);
					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				}
				m_byChk = 0;
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUILDEX_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDEX_CLOSE2, TRUE);
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
				char	szTemp[256] = {0,};
				char*	pToken=0;
				char	szSeps[] = "',?";
				char	szText[256] = {0,};

#ifdef _USE_IME

				g_pInputManager->ClearInput(INPUT_BUFFER_7);
				g_pInputManager->InsertCharacter(INPUT_BUFFER_7,(LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
				
#endif
				if(IsEmptyString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_7)))
				{
					DisplayMessageAdd(g_Message[ETC_MESSAGE206].szMessage, 0xFFFFB894, TRUE, DEF_CHAT_TYPE_GUILD); // "이름을 입력하세요."
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
					
					g_pInputManager->ClearInput(INPUT_BUFFER_7);								
					return CInterface::GetInstance()->GetSetWnd();
				}

				__lstrcpyn(szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_7), lstrlen(g_pInputManager->GetInputBuffer(INPUT_BUFFER_7))+1);

				pToken = strtok(szText, szSeps);

				if(pToken)
				{
					if(__strcmp(pToken, g_pInputManager->GetInputBuffer(INPUT_BUFFER_7))!=0)
					{
						DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894, TRUE, DEF_CHAT_TYPE_GUILD);	// MSG_ID : 57 ; 다음 문자를 사용할수가 없습니다(', ?).
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
						
						g_pInputManager->ClearInput(INPUT_BUFFER_7);									
						return CInterface::GetInstance()->GetSetWnd();
					}						
				}
				else
				{
					DisplayMessageAdd(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894, TRUE, DEF_CHAT_TYPE_GUILD);	// MSG_ID : 57 ; 다음 문자를 사용할수가 없습니다(', ?).										
					g_pInputManager->ClearInput(INPUT_BUFFER_7);								
					return CInterface::GetInstance()->GetSetWnd();
				}

				__lstrcpyn(szTemp, g_pInputManager->GetInputBuffer(INPUT_BUFFER_7), MAX_GUILD_NAME_REAL_LENGTH);

				pToken = strtok(szTemp, " ");

				if(pToken==NULL)
				{
					char szInfo[0xff] = {0,};

					if(m_byGuildType==__GCTYPE_GUILD__)
						wsprintf(szInfo, g_Message[ETC_MESSAGE289].szMessage, g_pMainPlayer->m_szName);	// MSG_ID : 289 ; %s님에 길드에 이름이 없습니다
					else
						wsprintf(szInfo, g_Message[ETC_MESSAGE290].szMessage, g_pMainPlayer->m_szName);	// MSG_ID : 290 ; %s님에 클랜에 이름이 없습니다

					DisplayMessageAdd(szInfo, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);
					_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
				}
				else
				{
					CTWS_GUILD_CREATE pPacket;
					memset(pPacket.szGuildName, 0, sizeof(pPacket.szGuildName));
					

					int nRt = ConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_7));

					if(nRt==1)
					{																		
						DisplayMessageAdd(g_Message[ETC_MESSAGE58].szMessage, 0xFFFF0000, TRUE, DEF_CHAT_TYPE_GUILD);	// MSG_ID : 58 ; 비속어나 욕설은 사용할 수 없습니다.
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
						
						g_pInputManager->ClearInput(INPUT_BUFFER_7);
						return CInterface::GetInstance()->GetSetWnd();
					}

					nRt = NoConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_7));
			
					if(nRt==1)
					{				
						DisplayMessageAdd(g_Message[ETC_MESSAGE59].szMessage, 0xFFFF0000, TRUE, DEF_CHAT_TYPE_GUILD);	// MSG_ID : 59 ; 금지 단어입니다. 사용할 수 없습니다.
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
						
						g_pInputManager->ClearInput(INPUT_BUFFER_7);
						return CInterface::GetInstance()->GetSetWnd();									
					}

					pPacket.bType	= m_byGuildType;
					pPacket.dwLev	= g_pMainPlayer->m_dwLevel;
					pPacket.wClass	= g_pMainPlayer->m_wClass;
					__lstrcpyn(pPacket.szGuildName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_7), MAX_GUILD_NAME_REAL_LENGTH);
					g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
				}		
				m_byChk = 0;
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDEX_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDEX_OK2, FALSE);
				SetRender(SPR_OBJ_GUILDEX_OK3, TRUE);
				m_bBtn[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_GUILDEX_OK1, FALSE);
				SetRender(SPR_OBJ_GUILDEX_OK2, TRUE);
				SetRender(SPR_OBJ_GUILDEX_OK3, FALSE);				
				m_bBtn[0] = TRUE;				
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{ // 길드 생성아이템을 이용하여 길드 생성시 취소 버튼 눌렀을경우
		  // 김영대 2005.01.24
			if(eMouseCheck==MOUSE_UP)
			{					
				if(m_byChk==1)
				{
					CTDS_ITEM_PICKUP ItemPickup;
					ItemPickup.bSectionNum	= 1;
					ItemPickup.i64ItemID	= 0;
					SetItemPacket(&ItemPickup, 70, m_byGuildType, 0, 0, 0);
					g_pNet->SendMsg( (char*)&ItemPickup, ItemPickup.GetPacketSize(), SERVER_INDEX_ZONE);
				}
				m_byChk		= 0;				
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{				
				SetRender(SPR_OBJ_GUILDEX_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDEX_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILDEX_CANCEL3, TRUE);
				m_bBtn[1] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				SetRender(SPR_OBJ_GUILDEX_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILDEX_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILDEX_CANCEL3, FALSE);
				m_bBtn[1] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILDEX_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDEX_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
	{	
		RECT rt;
		rt.left = (LONG)m_fPosX+135;	rt.right = (LONG)m_fPosX+256;
		rt.top = (LONG)m_fPosZ+70;		rt.bottom = (LONG)m_fPosZ+84;
		g_pInputManager->GetInputRect(INPUT_BUFFER_7) = rt;

#ifdef	_USE_IME 	

		GET_IMEEDIT()->SetEditPos(int(m_fPosX+135), int(m_fPosZ+70));			

#endif
		return CInterface::GetInstance()->GetSetWnd();
	}

	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}
			
void CGuildExWnd::GuildMsg()
{
	char	szInfo[0xff]={0,};
	char	szTemp[256]={0,};
	char	szText[256]={0,};
	char*	pToken=0;
	char	szSeps[]	= "',?";

#ifdef _USE_IME	// by deepdark.
	g_pInputManager->ClearInput(INPUT_BUFFER_7);
	g_pInputManager->InsertCharacter(INPUT_BUFFER_7, (LPSTR)GET_IMEEDIT()->GetImeEditString(), lstrlen(GET_IMEEDIT()->GetImeEditString()));
	
#endif	
							
	__lstrcpyn(szTemp, g_pInputManager->GetInputBuffer(INPUT_BUFFER_7), MAX_GUILD_NAME_REAL_LENGTH);

	pToken = strtok(szTemp, " ");

	if(pToken==NULL)						
	{					
		if(m_byGuildType==__GCTYPE_GUILD__)
			wsprintf(szInfo, g_Message[ETC_MESSAGE55].szMessage, g_pMainPlayer->m_szName); // MSG_ID : 55 ; %s 님의 길드에 이름이 없습니다.
		else 
			wsprintf(szInfo, g_Message[ETC_MESSAGE56].szMessage, g_pMainPlayer->m_szName); // MSG_ID : 56 ; %s 님의 클랜에 이름이 없습니다.
		
		ErrMessage(szInfo, 0xFFFFFF00, DEF_CHAT_TYPE_GUILD);
	}							
	else
	{
		__lstrcpyn(szText, g_pInputManager->GetInputBuffer(INPUT_BUFFER_7), MAX_GUILD_NAME_REAL_LENGTH);							

		pToken = strtok(szText, szSeps);

		if(pToken)
		{
			if(__strcmp(pToken, g_pInputManager->GetInputBuffer(INPUT_BUFFER_7))!=0)
			{
				ErrMessage(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894, DEF_CHAT_TYPE_GUILD);
				g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
				//SetFocusInput(INPUT_BUFFER_7, 1, 0, 0, 0, 0, 0);							
				return;
			}						
		}
		else
		{
			ErrMessage(g_Message[ETC_MESSAGE57].szMessage, 0xFFFFB894, DEF_CHAT_TYPE_GUILD);
			g_pInputManager->ClearInput(INPUT_BUFFER_7);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_7);
			
			return;
		}

		CTWS_GUILD_CREATE pPacket;
		
		if(IsEmptyString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_7)))
		{
			DisplayMessageAdd(g_Message[ETC_MESSAGE206].szMessage, 0xFFFFB894, TRUE, DEF_CHAT_TYPE_GUILD); // "이름을 입력하세요."
			ErrMessage(g_Message[ETC_MESSAGE58].szMessage, 0xFFFFFF00, DEF_CHAT_TYPE_GUILD);
			g_pInputManager->ClearInput(INPUT_BUFFER_7);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_7);
			return;
		}

		int nRt = ConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_7));

		if(nRt==1)
		{	
			ErrMessage(g_Message[ETC_MESSAGE58].szMessage, 0xFFFFFF00, DEF_CHAT_TYPE_GUILD);
			g_pInputManager->ClearInput(INPUT_BUFFER_7);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_7);
			return;
		}

		nRt = NoConvString(g_pInputManager->GetInputBuffer(INPUT_BUFFER_7));

		if(nRt==1)
		{	
			ErrMessage(g_Message[ETC_MESSAGE59].szMessage, 0xFFFF0000); // "금지 단어입니다. 사용할 수 없습니다."
			g_pInputManager->ClearInput(INPUT_BUFFER_7);
			g_pInputManager->SetFocusInput(INPUT_BUFFER_7);
			return;
		}
		memset(pPacket.szGuildName, 0, sizeof(pPacket.szGuildName));
		pPacket.bType	= m_byGuildType;
		pPacket.dwLev	= g_pMainPlayer->m_dwLevel;
		pPacket.wClass	= g_pMainPlayer->m_wClass;
		__lstrcpyn(pPacket.szGuildName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_7), MAX_GUILD_NAME_REAL_LENGTH);
		g_pNet->SendMsg((char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_WORLD);
		m_byChk = 0;
		SetActive(FALSE);					
	}
}
//==========================================================//
// End.														//
//==========================================================//