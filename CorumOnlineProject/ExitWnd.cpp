//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "ExitWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "CommandWnd.h"
#include "InitGame.h"
#include "message.h"
#include "NetworkClient.h"
#include "DSMiniMap.h"
#include "chat.h"
#include "CodeFun.h"
#include "PacketEncrypt.h"


CExitWnd* CExitWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CExitWnd::CExitWnd()
{ 
	m_bPointer		= FALSE;
	m_bInit			= FALSE;			
	bOtherBtn		= FALSE;
	m_bChkBtn		= FALSE;
	m_bGameExit		= FALSE;
	m_bCharExit		= FALSE;
	m_bGameReturn	= FALSE;
}

CExitWnd::~CExitWnd()
{
	
}


//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CExitWnd::Init()
{
	InsertData(SPR_OBJ_EXIT_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_EXIT_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 5.0f, 0);
	InsertData(SPR_OBJ_EXIT_WND3, SPR_INTERFACE_DF_WND3, 0, 110, 1.0f, 1.0f, 0);
	InsertData(BUTTON_OBJ_GAME_EXIT1, BUTTON_INTERFACE_GAME_EXIT1, 23, 35, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_GAME_EXIT2, BUTTON_INTERFACE_GAME_EXIT2, 23, 35, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_GAME_EXIT3, BUTTON_INTERFACE_GAME_EXIT3, 23, 35, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_CHAR_SELECT1, BUTTON_INTERFACE_CHAR_SELECT1, 23, 55, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_CHAR_SELECT2, BUTTON_INTERFACE_CHAR_SELECT2, 23, 55, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_CHAR_SELECT3, BUTTON_INTERFACE_CHAR_SELECT3, 23, 55, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_GAME_RETURN1, BUTTON_INTERFACE_GAME_RETURN1, 23, 75, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_GAME_RETURN2, BUTTON_INTERFACE_GAME_RETURN2, 23, 75, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_GAME_RETURN3, BUTTON_INTERFACE_GAME_RETURN3, 23, 75, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_GAME_WORLD1, BUTTON_INTERFACE_GAME_WORLD1, 23, 95, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_GAME_WORLD2, BUTTON_INTERFACE_GAME_WORLD2, 23, 95, 1.0f, 1.0f, 1);
	InsertData(BUTTON_OBJ_GAME_WORLD3, BUTTON_INTERFACE_GAME_WORLD3, 23, 95, 1.0f, 1.0f, 1);

	InsertData(SPR_OBJ_EXIT_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_EXIT_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);
		
	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);	
	InsertCheckInterface(25, 35, 232, 48, 4, CHECK_BTN);
	InsertCheckInterface(25, 55, 232, 67, 5, CHECK_BTN);	
	InsertCheckInterface(25, 75, 232, 88, 6, CHECK_BTN);
	InsertCheckInterface(25, 95, 232, 108, 7, CHECK_BTN);

	m_bInit = TRUE;
	
	return TRUE;
}

void CExitWnd::Remove()
{
	RemoveAllData();
	m_bInit		= FALSE;
	m_bActive	= FALSE;
}

void CExitWnd::SetOrder()
{

}

void CExitWnd::RenderText()
{
	RenderFont(g_Message[ETC_MESSAGE155].szMessage, m_fPosX+25, m_fPosX+55, m_fPosZ+6, m_fPosZ+48, GetStartOrder()+1);	
}

void CExitWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;

	if(bActive==FALSE)
	{
		if(m_bInit==TRUE)
		{		
			m_bSoundChk	= FALSE;
			ShowSpriteAll();
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
		}
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_EXIT_WND1, TRUE);
		SetRender(SPR_OBJ_EXIT_WND2, TRUE);
		SetRender(SPR_OBJ_EXIT_WND3, TRUE);
		SetRender(BUTTON_OBJ_GAME_EXIT1, TRUE);			
		SetRender(BUTTON_OBJ_CHAR_SELECT1, TRUE);						
		SetRender(BUTTON_OBJ_GAME_RETURN1, TRUE);							
		SetRender(SPR_OBJ_EXIT_CLOSE1, TRUE);
		
		if(GetGameStatus()==UPDATE_GAME_PLAY)
			SetRender(BUTTON_OBJ_GAME_WORLD1, TRUE);
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CExitWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();
	
	if(nRt!=4 && m_bGameExit)
	{
		SetRender(BUTTON_OBJ_GAME_EXIT1, TRUE);
		SetRender(BUTTON_OBJ_GAME_EXIT2, FALSE);
		SetRender(BUTTON_OBJ_GAME_EXIT3, FALSE);
		m_bGameExit = FALSE;
	}

	if(nRt!=5 && m_bCharExit)
	{
		SetRender(BUTTON_OBJ_CHAR_SELECT1, TRUE);
		SetRender(BUTTON_OBJ_CHAR_SELECT2, FALSE);
		SetRender(BUTTON_OBJ_CHAR_SELECT3, FALSE);
		m_bCharExit = FALSE;
	}

	if(nRt!=6 && m_bGameReturn)
	{
		SetRender(BUTTON_OBJ_GAME_RETURN1, TRUE);
		SetRender(BUTTON_OBJ_GAME_RETURN2, FALSE);
		SetRender(BUTTON_OBJ_GAME_RETURN3, FALSE);
		m_bGameReturn = FALSE;
	}

	if(nRt!=7 && m_bWorldChk)
	{
		if(GetGameStatus()==UPDATE_GAME_PLAY)
		{
			SetRender(BUTTON_OBJ_GAME_WORLD1, TRUE);
			SetRender(BUTTON_OBJ_GAME_WORLD2, FALSE);
			SetRender(BUTTON_OBJ_GAME_WORLD3, FALSE);
			m_bWorldChk = FALSE;
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
				SetActive(FALSE);				
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_EXIT_CLOSE1, FALSE);
				SetRender(SPR_OBJ_EXIT_CLOSE2, TRUE);
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
				// 게임종료 
				SetActive(FALSE);
				g_bIsRunning = FALSE;	
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(BUTTON_OBJ_GAME_EXIT1, FALSE);
				SetRender(BUTTON_OBJ_GAME_EXIT2, FALSE);
				SetRender(BUTTON_OBJ_GAME_EXIT3, TRUE);
				m_bGameExit = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(BUTTON_OBJ_GAME_EXIT1, FALSE);
				SetRender(BUTTON_OBJ_GAME_EXIT2, TRUE);
				SetRender(BUTTON_OBJ_GAME_EXIT3, FALSE);
				m_bGameExit = TRUE;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 5:
		{			
			if(eMouseCheck==MOUSE_UP)
			{
				// 서버화면으로 
				CInterface*		pInterface	= CInterface::GetInstance();
				CCommandWnd*	pCommandWnd = CCommandWnd::GetInstance();
				pCommandWnd->m_bMessageChk	= FALSE;

				CMiniMapWnd* pMiniMapWnd = CMiniMapWnd::GetInstance();
				pMiniMapWnd->ReleaseMap();
				
				for(int i = 0; i < CUR_INTERFACE; i++)
				{
					if(pInterface->m_pMenu[i]->GetActive()==TRUE)
						pInterface->m_pMenu[i]->SetActive(FALSE);
				}						

				if(GetGameStatus()== UPDATE_GAME_WORLD)
				{
					g_pNet->DisconnectServer(SERVER_INDEX_WORLD);
#ifdef __PACKET_ENCRYPTION
					// 게임 재시작.. 키를 다시 리셋한다.
					// modified by minjin. 2004. 11. 05.
					ResetEncryptionKey();
#endif
					::PostMessage(g_hMainWnd, UM_GAME_RESTART, 0, 0);					
				}
				else if(GetGameStatus()== UPDATE_GAME_PLAY)
				{
					g_pNet->DisconnectServer(SERVER_INDEX_WORLD);
#ifdef __PACKET_ENCRYPTION
					// 게임 재시작.. 키를 다시 리셋한다.
					// modified by minjin. 2004. 11. 05.
					ResetEncryptionKey();
#endif
					::PostMessage(g_hMainWnd, UM_GAME_RESTART, 0, 0);
					return 0;
				}				
								
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(BUTTON_OBJ_CHAR_SELECT1, FALSE);
				SetRender(BUTTON_OBJ_CHAR_SELECT2, FALSE);
				SetRender(BUTTON_OBJ_CHAR_SELECT3, TRUE);
				m_bCharExit = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(BUTTON_OBJ_CHAR_SELECT1, FALSE);
				SetRender(BUTTON_OBJ_CHAR_SELECT2, TRUE);
				SetRender(BUTTON_OBJ_CHAR_SELECT3, FALSE);
				m_bCharExit = TRUE;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 6:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(BUTTON_OBJ_GAME_RETURN1, FALSE);
				SetRender(BUTTON_OBJ_GAME_RETURN2, FALSE);
				SetRender(BUTTON_OBJ_GAME_RETURN3, TRUE);			
				m_bGameReturn	= TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(BUTTON_OBJ_GAME_RETURN1, FALSE);
				SetRender(BUTTON_OBJ_GAME_RETURN2, TRUE);
				SetRender(BUTTON_OBJ_GAME_RETURN3, FALSE);			
				m_bGameReturn	= TRUE;
			}

			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 7:
		{
			BYTE byChk = 0;

			if(eMouseCheck==MOUSE_UP)
			{
				if(GetGameStatus()==UPDATE_GAME_PLAY)
				{
					if (g_pMainPlayer->IsMatching() )
					{
						//"대전중 월드로 바로 나갈 수 없습니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE1065].szMessage, 0xFFFF0000);
						return 0;
					}
					else if( g_pThisDungeon->IsStadium() )
					{
						//"대전중 월드로 바로 나갈 수 없습니다."
						if( g_pMainPlayer->m_dwGuildWarFlag == G_W_F_OBSERVER )
						{
							// 옵저버는 나가게 해준다.
							GUILD_MATCH_CANCEL_OBSERVER	Packet;
							Packet.wDungeonID = g_pMainPlayer->m_wDungeonID;
							g_pNet->SendMsg( (char*)&Packet, Packet.GetPacketSize(), SERVER_INDEX_WORLD);
						}
						else
							DisplayMessageAdd(g_Message[ETC_MESSAGE1402].szMessage, 0xFFFF0000); // "길드전 중에는 월드맵으로 이동할 수 없습니다."

						return 0;
					}
					else if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PLAYER_SHOP)
					{
						// "노점중에는 월드 바로 나갈 수 없습니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE1121].szMessage, 0xFFFF0000); 
					}
					else if(g_pMainPlayer->GetStatus()==UNIT_STATUS_PORTAL_MOVING)
					{
						// "포탈중에는 월드 바로 나갈 수 없습니다."
						DisplayMessageAdd(g_Message[ETC_MESSAGE1122].szMessage, 0xFFFF0000); 
					}
					else
					{
						CTDS_WORLD_MOVE pPacket;
						pPacket.dwUserIndex	= g_pMainPlayer->m_dwUserIndex;
						g_pNet->SendMsg( (char*)&pPacket, pPacket.GetPacketSize(), SERVER_INDEX_ZONE);
					}

					SetActive(FALSE);
				}				
				else
				{
					byChk = __MOUSE_POINTER_DEFAULT__;
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(GetGameStatus()==UPDATE_GAME_PLAY)
				{
					SetRender(BUTTON_OBJ_GAME_WORLD1, FALSE);
					SetRender(BUTTON_OBJ_GAME_WORLD2, FALSE);
					SetRender(BUTTON_OBJ_GAME_WORLD3, TRUE);			
					m_bWorldChk	= TRUE;
				}				
				else
				{
					byChk = __MOUSE_POINTER_DEFAULTCLICK__;
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(GetGameStatus()==UPDATE_GAME_PLAY)
				{
					SetRender(BUTTON_OBJ_GAME_WORLD1, FALSE);
					SetRender(BUTTON_OBJ_GAME_WORLD2, TRUE);
					SetRender(BUTTON_OBJ_GAME_WORLD3, FALSE);			
					m_bWorldChk	= TRUE;
				}			
				else
				{
					byChk = __MOUSE_POINTER_DEFAULT__;
				}
			}

			SetMouseCheck(nRt, eMouseCheck, byChk);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_EXIT_CLOSE1, TRUE);
		SetRender(SPR_OBJ_EXIT_CLOSE2, FALSE);		
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