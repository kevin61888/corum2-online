//======================================================//
// Code by Jang.							2004.3.11	//
//======================================================//
#include "RankWnd.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "DSMiniMap.h"
#include "CodeFun.h"


CRankWnd* CRankWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CRankWnd::CRankWnd()
{
}

CRankWnd::~CRankWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CRankWnd::Init()
{	
/*
	InsertData(SPR_OBJ_RANK_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_RANK_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_RANK_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_RANK_CLOSE1, SPR_INTERFACE_CLOSE1, 242, 4, 1.0f, 1.0f, 1);
	InsertData(SPR_OBJ_RANK_CLOSE2, SPR_INTERFACE_CLOSE2, 242, 4, 1.0f, 1.0f, 1);	

	InsertCheckInterface(242, 4, 256, 16, 2, CHECK_CLOSE);
	InsertCheckInterface(0, 0, 242, 20, 3, CHECK_MOVEWND);
	*/
	m_bInit	= TRUE;

	return TRUE;
}

void CRankWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CRankWnd::SetOrder()
{
}

void CRankWnd::RenderText()
{
	char szInfo[0xff] = {0, };

	lstrcpy(szInfo, g_Message[ETC_MESSAGE917].szMessage); // "랭킹"
	
	int nSize = lstrlen(szInfo);		
	RenderFont(szInfo, m_fPosX+22, m_fPosX+22+nSize*7, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);

	lstrcpy(szInfo, g_Message[ETC_MESSAGE918].szMessage); // "순위"

	nSize = lstrlen(szInfo);
	RenderFont(szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

	lstrcpy(szInfo, g_Message[ETC_MESSAGE919].szMessage); // "길드명"

	nSize = lstrlen(szInfo);
	RenderFont(szInfo, m_fPosX+75, m_fPosX+75+nSize*7, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

	lstrcpy(szInfo, g_Message[ETC_MESSAGE920].szMessage); // "길드 레더 점수"

	nSize = lstrlen(szInfo);
	RenderFont(szInfo, m_fPosX+155, m_fPosX+155+nSize*7, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);	

	BYTE byIndex = 0;

	for(int i = 0; i < 3; i++)
	{
		if(m_dwGuildId[i]!=0)
		{
			wsprintf(szInfo, g_Message[ETC_MESSAGE921].szMessage, i+1); // "%d 위"

			nSize = lstrlen(szInfo);
			RenderFont(szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+70+i*30, m_fPosZ+84+i*30, GetStartOrder()+1);			

			lstrcpy(szInfo, m_szGuildName[i]);

			nSize = lstrlen(szInfo);
			RenderFont(szInfo, m_fPosX+75, m_fPosX+75+nSize*7, m_fPosZ+70+i*30, m_fPosZ+84+i*30, GetStartOrder()+1);

			wsprintf(szInfo, "%u", m_dwPoint[i]);

			nSize = lstrlen(szInfo);
			RenderFont(szInfo, m_fPosX+180, m_fPosX+180+nSize*7, m_fPosZ+70+i*30, m_fPosZ+84+i*30, GetStartOrder()+1);			
			byIndex++;
		}
	}
	
	if(g_pMainPlayer->m_dwGuildId!=0)
	{
		wsprintf(szInfo, g_Message[ETC_MESSAGE921].szMessage, m_dwRank); // "%d 위"

		nSize = lstrlen(szInfo);
		RenderFont(szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+70+byIndex*30, m_fPosZ+84+byIndex*30, GetStartOrder()+1);		

		lstrcpy(szInfo, m_szGuildName[3]);

		nSize = lstrlen(szInfo);
		RenderFont(szInfo, m_fPosX+75, m_fPosX+75+nSize*7, m_fPosZ+70+byIndex*30, m_fPosZ+84+byIndex*30, GetStartOrder()+1);

		wsprintf(szInfo, "%u", m_dwPoint[3]);

		nSize = lstrlen(szInfo);
		RenderFont(szInfo, m_fPosX+180, m_fPosX+180+nSize*7, m_fPosZ+70+byIndex*30, m_fPosZ+84+byIndex*30, GetStartOrder()+1);		
	}
	else
	{
		lstrcpy(szInfo, g_Message[ETC_MESSAGE67].szMessage); // "가입된 길드가 없습니다."

		nSize = lstrlen(szInfo);
		RenderFont(szInfo, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+70+byIndex*30, m_fPosZ+84+byIndex*30, GetStartOrder()+1);
	}	
}

void CRankWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_RANK_WND1, TRUE);
		SetRender(SPR_OBJ_RANK_WND2, TRUE);
		SetRender(SPR_OBJ_RANK_WND3, TRUE);
		SetRender(SPR_OBJ_RANK_CLOSE1, TRUE);		

		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int CRankWnd::CheckInterface()
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
				SetRender(SPR_OBJ_RANK_CLOSE1, FALSE);
				SetRender(SPR_OBJ_RANK_CLOSE2, TRUE);
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
		SetRender(SPR_OBJ_RANK_CLOSE1, TRUE);
		SetRender(SPR_OBJ_RANK_CLOSE2, FALSE);		
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
