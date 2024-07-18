//======================================================//
// Code by Jang.							2002.12.23	//
//======================================================//
#include "NoticeWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "HashTable.h"
#include "GlobalSoundHandles.h"
#include "CodeFun.h"



CNoticeWnd* CNoticeWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CNoticeWnd::CNoticeWnd()
{ 		
}

CNoticeWnd::~CNoticeWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CNoticeWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_NOTICE_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_NOTICE_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_NOTICE_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_NOTICE_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_NOTICE_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	*/
	m_bInit	= TRUE;

	memset(m_szNoticeMessage, 0, 1024);

	return TRUE;
}

void CNoticeWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CNoticeWnd::SetOrder()
{
}

void CNoticeWnd::RenderText()
{	
	char szInfo[0xff] = {0,};

	lstrcpy(szInfo, g_Message[ETC_MESSAGE415].szMessage); // "공지"

	int nSize = lstrlen(szInfo);
	RenderFont(szInfo, m_fPosX+22, m_fPosX+22+nSize*6, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);
	
	RenderTextEx();
}


void CNoticeWnd::SetActive(BOOL bActive)
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
		CInterface::GetInstance()->SetWnd(m_nIndex);

		SetRender(SPR_OBJ_NOTICE_WND1, TRUE);
		SetRender(SPR_OBJ_NOTICE_WND2, TRUE);
		SetRender(SPR_OBJ_NOTICE_WND3, TRUE);
		SetRender(SPR_OBJ_NOTICE_CLOSE1, TRUE);				

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CNoticeWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

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
				SetRender(SPR_OBJ_NOTICE_CLOSE1, FALSE);
				SetRender(SPR_OBJ_NOTICE_CLOSE2, TRUE);
			}
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	}		
	
	if(MouseUp())
	{
		SetRender(SPR_OBJ_NOTICE_CLOSE1, TRUE);
		SetRender(SPR_OBJ_NOTICE_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();		

	return 0;
}

void CNoticeWnd::RenderTextEx()
{	
	for(int i = 120; i <= 130; i++)
	{
		InterfaceTextRender(i);
		
	}	
	/*스피드 핵 사용이 감지되었습니다. 이
	정보는 서버의 DB에 저장되며, 고객감
	동팀의 정밀분석을 통해 스피드핵 사
	용여부를 최종 판단하게 됩니다. 스피
	드핵 사용이 확인되면 해당 계정에 운
	영원칙에 따라 제재가 가해지며, 개별
	적으로 통보가 가게 됩니다. 스피드핵
	을 사용하지 않았는데 일시적인 렉이
	나 PC이상으로 감지된 경우에는 데이
	터분석을 통해 자동확인되므로 안심하
	셔도 됩니다.*/
}

