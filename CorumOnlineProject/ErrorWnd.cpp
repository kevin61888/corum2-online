//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "ErrorWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "CodeFun.h"


CErrorWnd* CErrorWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CErrorWnd::CErrorWnd()
{ 
	m_byCount	= 0;	
	m_bExit		= FALSE;
	m_bPointer	= FALSE;
	m_dwCurTime	= 0;
	m_dwPevTime	= 0;
	m_fPosX		= 400;
	m_fPosZ		= 320;	
}

CErrorWnd::~CErrorWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CErrorWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_ERROR_WND, SPR_INTERFACE_OK2_WND, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_ERROR_OK1, SPR_INTERFACE_OK1, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ERROR_OK2, SPR_INTERFACE_OK2, 189, 110, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_ERROR_OK3, SPR_INTERFACE_OK3, 189, 110, 1.0f, 1.0f, 1);
		
	InsertCheckInterface(189, 110, 253, 126, 2, CHECK_BTN);	
*/
	m_bInit		= TRUE;
	m_byType	= 0;
	m_byTime	= 5;

	return TRUE;
}

void CErrorWnd::Remove()
{	
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CErrorWnd::SetOrder()
{
}

void CErrorWnd::RenderText()
{	
	RenderFont(g_Message[ETC_MESSAGE154].szMessage, m_fPosX+25, m_fPosX+65, m_fPosZ+6, m_fPosZ+18, GetStartOrder()+1);	
	
	// 메세지 //
	for(int i = 0; i < m_byCount; i++)
		RenderFont(m_szInfo[i], m_fPosX+25, m_fPosX+255, m_fPosZ+45+(i*18), m_fPosZ+57+(i*18), GetStartOrder()+1);
		
	if(m_byType==1)
	{
		m_dwCurTime	= timeGetTime();

		if((m_dwCurTime-m_dwPevTime)>=1000)
		{
			m_dwCurTime	= timeGetTime();
			m_dwPevTime	= timeGetTime();
			m_byTime--;
			
			if(m_byTime==0)
				g_bIsRunning = FALSE;		
			else
				wsprintf(m_szInfo[1], g_Message[ETC_MESSAGE552].szMessage, m_byTime);//"%u초 후에 종료하겠습니다."			
		}
	}
}

void CErrorWnd::SetError(char* pszText, char* pszText2, char* pszText3, BYTE byCount)
{
	m_byCount = byCount;

	if(byCount>0)
		lstrcpy(m_szInfo[0], pszText);
	if(byCount>1)
		lstrcpy(m_szInfo[1], pszText2);
	if(byCount>2)
		lstrcpy(m_szInfo[2], pszText3);
}

void CErrorWnd::SetActive(BOOL bActive)
{
	if(m_byType==1)
		g_bIsRunning = FALSE;
		
	m_bActive	= bActive;
	m_byType	= 0;
	
	if(bActive==FALSE)
	{
		m_bSoundChk = FALSE;
		ShowSpriteAll();		
		//if ( GetGameStatus() != UPDATE_GAME_LOGIN)   // 로그인시 꽝하는 사운드 이펙트 안나오게 안한다. 김영대 2003.02.22
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_ERROR_WND, TRUE);
		SetRender(SPR_OBJ_ERROR_OK1, TRUE);
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CErrorWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_bBtnChk==TRUE && nRt!=3)
	{
		SetRender(SPR_OBJ_ERROR_OK1, TRUE);
		SetRender(SPR_OBJ_ERROR_OK2, FALSE);
		SetRender(SPR_OBJ_ERROR_OK3, FALSE);
		m_bBtnChk = FALSE;
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

				if(m_bExit)
					g_bIsRunning	= FALSE;	//게임종료 
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_ERROR_OK1, FALSE);
				SetRender(SPR_OBJ_ERROR_OK2, FALSE);
				SetRender(SPR_OBJ_ERROR_OK3, TRUE);
				m_bBtnChk = TRUE;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_ERROR_OK1, FALSE);
				SetRender(SPR_OBJ_ERROR_OK2, TRUE);
				SetRender(SPR_OBJ_ERROR_OK3, FALSE);
				m_bBtnChk = TRUE;
			}
			SetMouseCheck(nRt, eMouseCheck);
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