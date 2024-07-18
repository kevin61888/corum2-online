//======================================================//
// Code by Jang.							2002.5.27	//
//======================================================//
#include "GameDefaultWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "CharSelectProcess.h"
#include "InputManager.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "LoginProcess.h"
#include "CorumResource.h"
#include "QuantityWnd.h"
#include "CodeFun.h"
#include "PacketEncrypt.h"

CGameDefaultWnd* CGameDefaultWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CGameDefaultWnd::CGameDefaultWnd()
{ 	
	m_bBtnChk[0]	= FALSE;
	m_bBtnChk[1]	= FALSE;
	m_bChar			= FALSE;
	m_bInit			= FALSE;
	m_bExit			= FALSE;
	m_byType		= 0;
	m_dwCurTime		= 0;
	m_dwPevTime		= 0;
}

CGameDefaultWnd::~CGameDefaultWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
void CGameDefaultWnd::SetChar(BOOL bChar)
{
	m_bChar = bChar;
}

BOOL CGameDefaultWnd::GetChar()
{
	return m_bChar;
}

BOOL CGameDefaultWnd::Init()
{
/*
	InsertData(SPR_OBJ_GAMEDEFAULT_WND, SPR_INTERFACE_OK2_WND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_GAMEDEFAULT_OK1, SPR_INTERFACE_OK1, 121, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GAMEDEFAULT_OK2, SPR_INTERFACE_OK2, 121, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GAMEDEFAULT_OK3, SPR_INTERFACE_OK3, 121, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GAMEDEFAULT_CANCEL1, SPR_INTERFACE_CANCEL1, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GAMEDEFAULT_CANCEL2, SPR_INTERFACE_CANCEL2, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_GAMEDEFAULT_CANCEL3, SPR_INTERFACE_CANCEL3, 189, 110, 1.0f, 1.0f, 1);
	
	InsertCheckInterface(121, 110, 185, 126, 3, CHECK_BTN);
	InsertCheckInterface(189, 110, 253, 126, 2, CHECK_BTN);
	*/
	m_bInit	= TRUE;
	
	return TRUE;
}

void CGameDefaultWnd::Remove()
{
	RemoveAllData();		
	m_bInit		= FALSE;
	m_bActive	= FALSE;
}

void CGameDefaultWnd::OpenWndEx(char* pszCaption, char* pszText, char* pszText2, char* pszText3, float fPosX, float fPosY, BYTE byCount)
{
	m_byType	= 1;

	SetPosObj(SPR_OBJ_GAMEDEFAULT_OK1, 121, 110);	
	SetPosObj(SPR_OBJ_GAMEDEFAULT_OK2, 121, 110);
	SetPosObj(SPR_OBJ_GAMEDEFAULT_OK3, 121, 110);

	SetRender(SPR_OBJ_GAMEDEFAULT_WND, TRUE);
	SetRender(SPR_OBJ_GAMEDEFAULT_OK1, TRUE);
	SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL1, TRUE);
	
	m_byCount	= byCount;
	
	SetPosX(fPosX);
	SetPosY(fPosY);

	m_rtRect[0].left	= (LONG)m_fPosX+25;
	m_rtRect[0].right	= (LONG)m_fPosX+120;
	m_rtRect[0].top		= (LONG)m_fPosZ+6;
	m_rtRect[0].bottom	= (LONG)m_fPosZ+18;

	for(int i = 1 ; i < byCount+1; i++)
	{
		m_rtRect[i].left	= (LONG)m_fPosX+25;
		m_rtRect[i].right	= (LONG)m_fPosX+255;
		m_rtRect[i].top		= (LONG)m_fPosZ+45+(i-1)*17;
		m_rtRect[i].bottom	= (LONG)m_fPosZ+57+(i-1)*17;
	}	
	
	lstrcpy(m_szInfo[0], pszCaption);
	lstrcpy(m_szInfo[1], pszText);	
	lstrcpy(m_szInfo[2], pszText2);
	lstrcpy(m_szInfo[3], pszText3);	
}

void CGameDefaultWnd::OpenWnd(char* pszCaption, char* pszText, char* pszText2, char* pszText3, float fPosX, float fPosY, BYTE byCount, BOOL bSound)
{
	m_byType	= 0;

	SetRender(SPR_OBJ_GAMEDEFAULT_WND, TRUE);
	SetRender(SPR_OBJ_GAMEDEFAULT_OK1, TRUE);
	
	m_byCount	= byCount;
	
	SetPosX(fPosX);
	SetPosY(fPosY);

	m_rtRect[0].left	= (LONG)m_fPosX+25;
	m_rtRect[0].right	= (LONG)m_fPosX+120;
	m_rtRect[0].top		= (LONG)m_fPosZ+6;
	m_rtRect[0].bottom	= (LONG)m_fPosZ+18;

	for(int i = 1 ; i < byCount+1; i++)
	{
		m_rtRect[i].left	= (LONG)m_fPosX+25;
		m_rtRect[i].right	= (LONG)m_fPosX+255;
		m_rtRect[i].top		= (LONG)m_fPosZ+45+(i-1)*17;
		m_rtRect[i].bottom	= (LONG)m_fPosZ+57+(i-1)*17;
	}	
	
	lstrcpy(m_szInfo[0], pszCaption);
	lstrcpy(m_szInfo[1], pszText);	
	lstrcpy(m_szInfo[2], pszText2);
	lstrcpy(m_szInfo[3], pszText3);
	
	SetPosObj(SPR_OBJ_GAMEDEFAULT_OK1, 189, 110);	
	SetPosObj(SPR_OBJ_GAMEDEFAULT_OK2, 189, 110);
	SetPosObj(SPR_OBJ_GAMEDEFAULT_OK3, 189, 110);	

	if(bSound)
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
}

void CGameDefaultWnd::SetOrder()
{
}

void CGameDefaultWnd::RenderText()
{	
	if(!IsEmptyString(m_szInfo[0]))
		RenderFont(m_szInfo[0], m_fPosX+25, m_fPosX+120, m_fPosZ+6, m_fPosZ+48, GetStartOrder()+2);		

	for(int i = 1; i < m_byCount+1; i++)
	{
		if(!IsEmptyString(m_szInfo[i]))
			RenderFont(m_szInfo[i], m_fPosX+25, m_fPosX+255, m_fPosZ+45+(i-1)*17, m_fPosZ+57+(i-1)*17, GetStartOrder()+2);
	}
			
	if(m_bExit)
	{
		m_dwCurTime	= timeGetTime();

		if((m_dwCurTime-m_dwPevTime)>=10000)
			g_bIsRunning = FALSE;
	}
}

void CGameDefaultWnd::SetActive(BOOL bActive)
{
	m_bActive = bActive;
	
	if(bActive==FALSE)
	{	
		if(m_bInit==TRUE)
		{
			m_bExit	= FALSE;
			m_bSoundChk	= FALSE;
			ShowSpriteAll();
			//if ( GetGameStatus() != UPDATE_GAME_LOGIN)   // 로그인시 꽝하는 사운드 이펙트 안나오게 안한다. 김영대 2003.02.22
			_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);

			// 삭제 할때 //
			if(GetChar())
			{
#if IS_KOREA_LOCALIZING()

				if(g_NetMarble.bIsTryCorrectLogin)	//넷마블
				{
					CTWS_DELETE_CHARACTER packet;
					memset(packet.szCharacterName, 0, sizeof(packet.szCharacterName));
					packet.bChrNum = (BYTE)g_pGVLogin->dwCurCharIndex;

					int nLen = lstrlen(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName);
	
					if(nLen)
					{
						__lstrcpyn(packet.szCharacterName, g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName, MAX_CHARACTER_NAME_REAL_LENGTH);
						memset(packet.szJumin, 0, sizeof(packet.szJumin));

						//	by minjin. 2004. 10. 25.
						SendMsgWithBlowfish((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
						//g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);

					}	
					else
					{
						SetChar(FALSE);
					}

				}
				else
				{	//코룸 - 주민번호 체크 
					CQuantityWnd* pQuantityWnd = CQuantityWnd::GetInstance();
					pQuantityWnd->OpenWnd(__QUANTITY_JUMIN_NUMBER__);
					
					RECT rt;
					rt.left	 = (LONG)pQuantityWnd->m_fPosX+20;
					rt.right = (LONG)rt.left + 50;
					rt.top	 = (LONG)pQuantityWnd->m_fPosZ+93;
					rt.bottom  = (LONG)pQuantityWnd->m_fPosZ+105;
					int nOrder = __ORDER_INTERFACE_START__ + pQuantityWnd->m_byOrder*10+2;						
					g_pInputManager->InitializeInput(INPUT_BUFFER_CHARACTER_JUMIN, FONT_SS3D_GAME, rt, 0xffffffff, nOrder, TRUE);
					g_pInputManager->SetFocusInput(INPUT_BUFFER_CHARACTER_JUMIN);
					g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_INPUT_JUMIN_NUM;
					SetChar(FALSE);
				}
#else
				CTWS_DELETE_CHARACTER packet;	memset(packet.szCharacterName, 0, sizeof(packet.szCharacterName));
				packet.bChrNum = g_pGVLogin->dwCurCharIndex;
				
				int nLen = lstrlen(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName);

				if(nLen)
				{
					__lstrcpyn(packet.szCharacterName, g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName, MAX_CHARACTER_NAME_REAL_LENGTH);
					memset(packet.szJumin, 0, sizeof(packet.szJumin));

					//	by minjin. 2004. 10. 25.					
					SendMsgWithBlowfish((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
					//g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);				

					SetChar(FALSE);
				}
				else
				{
					SetChar(FALSE);
				}
#endif
			}	
		}
	}			
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}	
}

int CGameDefaultWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(nRt!=2 && m_bBtnChk[0]==TRUE)
	{
		if(m_byType==0)
		{
			SetRender(SPR_OBJ_GAMEDEFAULT_OK1, TRUE);
			SetRender(SPR_OBJ_GAMEDEFAULT_OK3, FALSE);
			SetRender(SPR_OBJ_GAMEDEFAULT_OK2, FALSE);
		}
		else
		{
			SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL1, TRUE);
			SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL2, FALSE);
			SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL3, FALSE);
		}
		m_bBtnChk[0] = FALSE;
	}

	if(nRt!=3 && m_bBtnChk[1]==TRUE)
	{
		SetRender(SPR_OBJ_GAMEDEFAULT_OK1, TRUE);
		SetRender(SPR_OBJ_GAMEDEFAULT_OK3, FALSE);
		SetRender(SPR_OBJ_GAMEDEFAULT_OK2, FALSE);
		m_bBtnChk[1] = FALSE;
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
				if(m_byType==0)
				{
					SetRender(SPR_OBJ_GAMEDEFAULT_OK1, TRUE/*FALSE*/);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK2, FALSE/*TRUE*/);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK3, FALSE);				
					SetActive(FALSE);
					
					LoginInterface(TRUE);
				}
				else if(m_byType == 167)
				{
					SetRender(SPR_OBJ_GAMEDEFAULT_OK1, TRUE/*FALSE*/);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK2, FALSE/*TRUE*/);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK3, FALSE);
					
					SetActive(FALSE);
					m_bBtnChk[0] = TRUE;

					g_pNet->DisconnectServer(SERVER_INDEX_AGENT);
					
					//다시 ServerSelect 모드로 
					SPR(SPR_LOGIN_SERVERSELECT_WINDOW)->ShowSprite(TRUE);
					SPR(SPR_LOGIN_LOGIN_WINDOW)->ShowSprite(FALSE);

					SPR(BUTTON_OK)->ShowSprite(FALSE);
					SPR(BUTTON_OK_DOWN)->ShowSprite(FALSE);
					SPR(BUTTON_OK_OVER)->ShowSprite(FALSE);

					SPR(BUTTON_CANCEL)->ShowSprite(FALSE);
					SPR(BUTTON_CANCEL_DOWN)->ShowSprite(FALSE);
					SPR(BUTTON_CANCEL_OVER)->ShowSprite(FALSE);
					SPR(SPR_LOGIN_EXIT1)->ShowSprite(FALSE);								

					g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
					g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
					g_pInputManager->SetFocusInput(INPUT_BUFFER_0);

					RECT rt;
#ifdef __USE_NEW_CONNECT_INI_CHINA
					for(int i=0; (DWORD)i<g_pGVLogin->GetTotalRegionNumber(); i++)
					{
						//Server Set Display Rect - 01 Line
						rt.left = 484;	rt.right = rt.left + 100;	rt.top = 492-1 + (i*19);	rt.bottom = rt.top	+ 14;
						g_pInputManager->InitializeInput(INPUT_BUFFER_LOGIN_SERVER_LIST+i, FONT_SS3D_LOGIN, rt, 0xffffffff, 3);
						g_pInputManager->InsertCharacter(INPUT_BUFFER_LOGIN_SERVER_LIST+i, 						
							g_pGVLogin->GetRegionInfo(i)->szRegionName, lstrlen(g_pGVLogin->GetRegionInfo(i)->szRegionName));						
					}
#else
					for(int i=0; (DWORD)i<g_pGVLogin->GetTotalSetNumber(); i++)
					{
						//Server Set Display Rect - 01 Line
						rt.left = 484;	rt.right = rt.left + 100;	rt.top = 492-1 + (i*19);	rt.bottom = rt.top	+ 14;
						g_pInputManager->InitializeInput(INPUT_BUFFER_LOGIN_SERVER_LIST+i, FONT_SS3D_LOGIN, rt, 0xffffffff, 3);
						g_pInputManager->InsertCharacter(INPUT_BUFFER_LOGIN_SERVER_LIST+i, 						
							g_pGVLogin->GetSetInfo(i)->szSetName, lstrlen(g_pGVLogin->GetSetInfo(i)->szSetName));						
					}
#endif
					g_pGVLogin->bIsLoginInputMode = 0; 
					g_pGVLogin->bIsLoging = FALSE;
				}
				SetActive(FALSE);
				m_bBtnChk[0] = TRUE;		
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{	
				if(m_byType==0)
				{
					SetRender(SPR_OBJ_GAMEDEFAULT_OK1, FALSE/*FALSE*/);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK2, FALSE/*TRUE*/);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK3, TRUE);		
				}
				SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL2, FALSE);
				SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL3, TRUE);											
				m_bBtnChk[0] = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{				
				if(m_byType==0)
				{
					SetRender(SPR_OBJ_GAMEDEFAULT_OK1, FALSE/*FALSE*/);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK2, TRUE/*TRUE*/);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK3, FALSE);							
				}
				SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL1, FALSE);
				SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL2, TRUE);
				SetRender(SPR_OBJ_GAMEDEFAULT_CANCEL3, FALSE);							
				m_bBtnChk[0] = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
		}			
		break;
	case 3:
		{						
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_byType==1)
				{
					SetRender(SPR_OBJ_GAMEDEFAULT_OK1, FALSE);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK2, TRUE);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK3, FALSE);			
					SetChar(TRUE);
					SetActive(FALSE);
					m_bBtnChk[1] = TRUE;					
				}	
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byType==1)
				{
					SetRender(SPR_OBJ_GAMEDEFAULT_OK1, FALSE);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK2, FALSE);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK3, TRUE);			
					m_bBtnChk[1] = TRUE;
				}
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_byType==1)
				{
					SetRender(SPR_OBJ_GAMEDEFAULT_OK1, FALSE);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK2, TRUE);
					SetRender(SPR_OBJ_GAMEDEFAULT_OK3, FALSE);
					m_bBtnChk[1] = TRUE;
				}
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
	}		
	
	MouseUp();

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
