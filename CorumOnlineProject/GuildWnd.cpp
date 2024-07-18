//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "GuildWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "GuildExWnd.h"

//2005.02.04 김영대 *****
#include "InputManager.h"
#include "DungeonProcess.h"
#include "ChatWnd.h"
//************************

CGuildWnd* CGuildWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGuildWnd::CGuildWnd()
{
 	m_bBtn[0]	= FALSE;
	m_bBtn[1]	= FALSE;
}

CGuildWnd::~CGuildWnd()
{
	
}


//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CGuildWnd::Init()
{	
/*
	InsertData(SPR_OBJ_GUILD_CREATE_WND, SPR_INTERFACE_OKWND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GUILD_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GUILD_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILD_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILD_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILD_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILD_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_GUILD_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 2);	
	
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	InsertCheckInterface(121, 110, 185, 126, 4, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 5, CHECK_BTN);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CGuildWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildWnd::SetOrder()
{

}

void CGuildWnd::RenderText()
{
	if(m_bGuildType==__GCTYPE_GUILD__)
	{
		// MSG_ID : 285 ; 길드 
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE285].szMessage);	
	}
	else
	{
		// MSG_ID : 286 ; 클랜 
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE286].szMessage);	
	}

	RenderFont(m_szInfo, m_fPosX+25, m_fPosX+55, m_fPosZ+6, m_fPosZ+20, GetStartOrder()+1);			

	if(m_bGuildType==__GCTYPE_GUILD__)
	{
		// MSG_ID : 310 ; 길드를 생성 하시겠습니까?
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE310].szMessage);	
	}
	else	
	{
		// MSG_ID : 311 ; 클랜를 생성 하시겠습니까?
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE311].szMessage);	
	}

	RenderFont(m_szInfo, m_fPosX+30, m_fPosX+200, m_fPosZ+46, m_fPosZ+60, GetStartOrder()+1);
}

void CGuildWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		m_bSoundChk	= FALSE;
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILD_CREATE_WND, TRUE);
		SetRender(SPR_OBJ_GUILD_OK1, TRUE);
		SetRender(SPR_OBJ_GUILD_CANCEL1, TRUE);		
		SetRender(SPR_OBJ_GUILD_CLOSE1, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CGuildWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtn[0] && nRt!=4)
	{
		SetRender(SPR_OBJ_GUILD_OK1, TRUE);
		SetRender(SPR_OBJ_GUILD_OK2, FALSE);
		SetRender(SPR_OBJ_GUILD_OK3, FALSE);								
		m_bBtn[0] = FALSE;
	}

	if(m_bBtn[1] && nRt!=5)
	{
		SetRender(SPR_OBJ_GUILD_CANCEL1, TRUE);
		SetRender(SPR_OBJ_GUILD_CANCEL2, FALSE);
		SetRender(SPR_OBJ_GUILD_CANCEL3, FALSE);
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
				SetActive(FALSE);				
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_GUILD_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILD_CLOSE2, TRUE);
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
			// 길드 생성 아이템 사용 2005.01.25 김영대
			switch(eMouseCheck )
			{
				case MOUSE_UP :
				{
					// 아이템 확인 //
					
					BYTE				bSrcIndex = 0;
					BOOL				bItemChk = FALSE;
					CTDS_ITEM_PICKUP	pPacket;
					pPacket.bInv		= 30;
					pPacket.bZipCode2	= m_bGuildType;

					switch(m_bGuildType)
					{
						case __GCTYPE_GUILD__:
						{
							if(g_pMainPlayer->m_pInv_Small[m_bZipCode].m_wItemID==__ITEM_GUILD_INDEX__)
							{
								pPacket.bZipCode = m_bZipCode;
								bSrcIndex = m_bZipCode;
								bItemChk = TRUE;
							}						
							else
							{
								for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==__ITEM_GUILD_INDEX__)
									{
										pPacket.bZipCode	= i;
										bSrcIndex			= i;
										bItemChk			= TRUE;
									}							
								}
							}
						}
						break;
						case __GCTYPE_CLEN__:
						{
							if(g_pMainPlayer->m_pInv_Small[m_bZipCode].m_wItemID==__ITEM_CLEN_INDEX__)
							{
								pPacket.bZipCode = m_bZipCode;
								bSrcIndex = m_bZipCode;	
								bItemChk = TRUE;
							}
							else
							{
								for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
								{
									if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==__ITEM_CLEN_INDEX__)
									{
										pPacket.bZipCode	= i;
										bSrcIndex			= i;
										bItemChk			= TRUE;
									}							
								}
							}
						}
						break;
					}
					
					if(bItemChk==TRUE)
					{
						//SetActive(FALSE);				
						//InputGuildName(m_bGuildType);
						g_pNet->SendMsg( (char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE );
						
						if(g_ItemMoveManager.GetNewItemMoveMode())
						{
							g_ItemUsedManager.SendItemUsedNativePacket(ITEM_NATIVE_INV_SMALL,bSrcIndex,0xff);
						}					
					}
					else
					{
						if(m_bGuildType==__GCTYPE_GUILD__)
						{
							// MSG_ID : 312 ; 아이템이 없습니다. 길드를 생성할수가 없습니다.
							DisplayMessageAdd(g_Message[ETC_MESSAGE312].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
						}
						else
						{
							// MSG_ID : 313 ; 아이템이 없습니다. 클랜를 생성할수가 없습니다.
							DisplayMessageAdd(g_Message[ETC_MESSAGE313].szMessage, 0xff00ffff, TRUE, DEF_CHAT_TYPE_GUILD);	
						}
						
						_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
					}	
					
					
					SetActive(FALSE);				
				}
				break;
				case MOUSE_DOWN:
				{				
					SetRender(SPR_OBJ_GUILD_OK1, FALSE);
					SetRender(SPR_OBJ_GUILD_OK2, FALSE);
					SetRender(SPR_OBJ_GUILD_OK3, TRUE);
					m_bBtn[0]	= TRUE;
				}
				break;
				case MOUSE_OVER :
				{
					SetRender(SPR_OBJ_GUILD_OK1, FALSE);
					SetRender(SPR_OBJ_GUILD_OK2, TRUE);
					SetRender(SPR_OBJ_GUILD_OK3, FALSE);				
					m_bBtn[0]	= TRUE;
				}
				break;
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
				SetRender(SPR_OBJ_GUILD_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILD_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GUILD_CANCEL3, TRUE);
				m_bBtn[1]	= TRUE;
			}
			else if(eMouseCheck==MOUSE_UP)
			{
				SetRender(SPR_OBJ_GUILD_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GUILD_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GUILD_CANCEL3, FALSE);
				m_bBtn[1]	= TRUE;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_GUILD_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILD_CLOSE2, FALSE);		
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

void CGuildWnd::InputGuildName(BYTE	byGuildType)
{
//챗팅 모드 
	CGuildExWnd*		pGuildExWnd			= CGuildExWnd::GetInstance();		
	CChatWnd*			pChatWnd			= CChatWnd::GetInstance();

	pGuildExWnd->SetActive(TRUE);
	pGuildExWnd->m_byChk = 1;
	pGuildExWnd->m_byGuildType = byGuildType;			

	// 입력 모드 //
	RECT rt;
	rt.left		= (LONG)pGuildExWnd->m_fPosX+135;
	rt.right	= (LONG)pGuildExWnd->m_fPosX+200;
	rt.top		= (LONG)pGuildExWnd->m_fPosZ+70;
	rt.bottom	= (LONG)pGuildExWnd->m_fPosZ+82;
	int nOrder	= __ORDER_INTERFACE_START__ + pGuildExWnd->m_byOrder*10 + 2;

	if(g_pGVDungeon->bChatMode==TRUE)
	{
		g_pInputManager->ClearInput(INPUT_BUFFER_19);
		g_pInputManager->SetFocusInput(INPUT_BUFFER_0);
		
		pChatWnd->SetActive(FALSE);				
	}

	g_pInputManager->InitializeInput(INPUT_BUFFER_7, FONT_SS3D_GAME, rt, 0xffffffff, nOrder);
	
	ChatModeOn(INPUT_BUFFER_7, FALSE);

#ifdef	_USE_IME 
	
	POINT pChatPoint;
	pChatPoint.x	= long(pGuildExWnd->m_fPosX+135);
	pChatPoint.y	= long(pGuildExWnd->m_fPosZ+70);
	GET_IMEEDIT()->SetEditIndex(1);
//			GET_IMEEDIT()->ClearImeEditString();			
	GET_IMEEDIT()->ActivateIME(pChatPoint, MAX_GUILD_NAME_REAL_LENGTH, pGuildExWnd->GetStartOrder()+2);			
#endif      

}

void CGuildWnd::SendGuildItemUse(BYTE btGuildType)
{
	CTDS_ITEM_PICKUP	pPacket;
	pPacket.bInv		= 30;
	pPacket.bZipCode2	= m_bGuildType;
	switch(m_bGuildType)
	{
		case __GCTYPE_GUILD__:
		{
			if(g_pMainPlayer->m_pInv_Small[m_bZipCode].m_wItemID==__ITEM_GUILD_INDEX__)
			{
				pPacket.bZipCode = m_bZipCode;
			}						
			else
			{
				for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==__ITEM_GUILD_INDEX__)
					{
						pPacket.bZipCode	= i;
					}							
				}
			}
		}
		break;
		case __GCTYPE_CLEN__:
		{
			if(g_pMainPlayer->m_pInv_Small[m_bZipCode].m_wItemID==__ITEM_CLEN_INDEX__)
			{
				pPacket.bZipCode = m_bZipCode;
			}
			else
			{
				for(BYTE i = 0; i < MAX_INV_SMALL_POOL; i++)
				{
					if(g_pMainPlayer->m_pInv_Small[i].m_wItemID==__ITEM_CLEN_INDEX__)
					{
						pPacket.bZipCode	= i;
					}							
				}
			}
		}
		break;
	}
	g_pNet->SendMsg( (char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE );

}
