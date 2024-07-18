//======================================================//
// Code by Jang.							2003.05.09	//
//======================================================//
#include "CharMoveRTWnd.h"
#include "SpriteManager.h"
#include "GameControl.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "Chat.h"
#include "NetworkClient.h"
#include "InitGame.h"
#include "CharMoveEditWnd.h"
#include "CodeFun.h"

CCharMoveRtWnd* CCharMoveRtWnd::c_pThis = NULL;

//======================================================//
// Construction/Destrution.								//
//======================================================//
CCharMoveRtWnd::CCharMoveRtWnd()
{ 	
	m_bBtn			= FALSE;
	m_byTotalCnt	= 0;
	m_byMoveSuccess	= 0;
	m_byMoveFail	= 0;
	m_bySize		= 0;

	for(int i = 0; i < 4; i++)
	{
		m_sCharMoveInfo[i].byNum	= 0xff;
		m_sCharMoveInfo[i].byResult	= 0xff;
		memset(&m_sCharMoveInfo[i].szName, 0, sizeof(m_sCharMoveInfo[i].szName));
	}
}

CCharMoveRtWnd::~CCharMoveRtWnd()
{
}
//======================================================//
// Class Member Function.								//
//======================================================//
BOOL CCharMoveRtWnd::Init()
{	
	/*
	InsertData(SPR_OBJ_CHARMOVERT_WND1, SPR_INTERFACE_DF_WND1, 0, 0, 1.0f, 1.0f, 0);
	InsertData(SPR_OBJ_CHARMOVERT_WND2, SPR_INTERFACE_DF_WND2, 0, 32, 1.0f, 13.0f, 0);
	InsertData(SPR_OBJ_CHARMOVERT_WND3, SPR_INTERFACE_DF_WND3, 0, 240, 1.0f, 1.0f, 0);			
	InsertData(SPR_OBJ_CHARMOVERT_OK1, SPR_INTERFACE_OK1, 189, 238, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVERT_OK2, SPR_INTERFACE_OK2, 189, 238, 1.0f, 1.0f, 2);
	InsertData(SPR_OBJ_CHARMOVERT_OK3, SPR_INTERFACE_OK3, 189, 238, 1.0f, 1.0f, 2);	
		
	InsertCheckInterface(189, 238, 254, 254, 2, CHECK_BTN, 2);	
*/
	m_bInit	= TRUE;

	return TRUE;
}

void CCharMoveRtWnd::Remove()
{
	RemoveAllData();	
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CCharMoveRtWnd::SetOrder()
{
}

void CCharMoveRtWnd::RenderText()
{
	BYTE byIndex = 0;

	lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1094].szMessage); // "알림 - 캐릭터 이전"
				
	int nSize = lstrlen(m_szInfo);
	
	RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);

	if(m_byTotalCnt==m_byMoveSuccess)
	{
		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1099].szMessage); // "캐릭터 이전이 성공적으로 완료되었습니다."

		int nSize = lstrlen(m_szInfo);

		RenderFont(m_szInfo, m_fPosX+10, m_fPosX+10+nSize*7, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

		byIndex += 2;
	}
	else
	{
		wsprintf(m_szInfo, g_Message[ETC_MESSAGE1100].szMessage, m_byMoveFail); // "캐릭터명의 중복으로 %d개의 캐릭터가"

		int nSize = lstrlen(m_szInfo);

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40, m_fPosZ+54, GetStartOrder()+1);

		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1101].szMessage); // "이전에 실패 하였습니다. 확인 후 다시"

		nSize = lstrlen(m_szInfo);

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+60, m_fPosZ+74, GetStartOrder()+1);

		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1102].szMessage); // "이전을 해주십시요."

		nSize = lstrlen(m_szInfo);

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+80, m_fPosZ+94, GetStartOrder()+1);
						
		byIndex += 4;
	}

	BOOL bChk = FALSE;
		
	// 캐릭터 이전 정보 출력 //		
	for(int i = 0; i < 4; i++)
	{		
		if(m_sCharMoveInfo[i].byNum!=0xff)
		{	
			if(m_sCharMoveInfo[i].byResult==0)				// 성공.
			{
				//"%d. %s 캐릭터 이전완료."
				wsprintf(m_szInfo, g_Message[ETC_MESSAGE1103].szMessage, i+1, m_sCharMoveInfo[i].szName);
				
				int nSize = lstrlen(m_szInfo);
				
				RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40+byIndex*20, m_fPosZ+54+byIndex*20, GetStartOrder()+1);
			}
			else if(m_sCharMoveInfo[i].byResult==1)			// 이름 중복.
			{
				//"%d. %s 캐릭터 이름 중복."
				wsprintf(m_szInfo, g_Message[ETC_MESSAGE1104].szMessage, i+1, m_sCharMoveInfo[i].szName);
				
				int nSize = lstrlen(m_szInfo);

				RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40+byIndex*20, m_fPosZ+54+byIndex*20, GetStartOrder()+1);
			}
			else if(m_sCharMoveInfo[i].byResult==2)			// 자리 중복.
			{
				//"%d. %s 캐릭터 자리 중복."
				wsprintf(m_szInfo, g_Message[ETC_MESSAGE1105].szMessage, i+1, m_sCharMoveInfo[i].szName);
				
				int nSize = lstrlen(m_szInfo);

				RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40+byIndex*20, m_fPosZ+54+byIndex*20, GetStartOrder()+1);

				bChk = TRUE;
			}
			else if(m_sCharMoveInfo[i].byResult==3)			// 업데이트 실패.
			{
				//"%d. %s 캐릭터 업데이트 실패."
				wsprintf(m_szInfo, g_Message[ETC_MESSAGE1106].szMessage, i+1, m_sCharMoveInfo[i].szName);
				
				int nSize = lstrlen(m_szInfo);

				RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40+byIndex*20, m_fPosZ+54+byIndex*20, GetStartOrder()+1);

				bChk = TRUE;
			}
			byIndex++;
		}
	}

	if(bChk)
	{
		byIndex++;

		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1107].szMessage); // "캐릭터 위치가 중복되었거나 정보 업데이"
				
		int nSize = lstrlen(m_szInfo);

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40+byIndex*20, m_fPosZ+54+byIndex*20, GetStartOrder()+1);

		byIndex++;

		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1108].szMessage); // "트에 실패하였습니다. 운영팀에 문의해 주"
				
		nSize = lstrlen(m_szInfo);

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40+byIndex*20, m_fPosZ+54+byIndex*20, GetStartOrder()+1);

		byIndex++;

		lstrcpy(m_szInfo, g_Message[ETC_MESSAGE1109].szMessage); // "시기 바랍니다."
				
		nSize = lstrlen(m_szInfo);

		RenderFont(m_szInfo, m_fPosX+15, m_fPosX+15+nSize*7, m_fPosZ+40+byIndex*20, m_fPosZ+54+byIndex*20, GetStartOrder()+1);
	}
}

void CCharMoveRtWnd::SetCharacter(char* szName, BYTE byNum, BYTE byResult)
{
	for(int i = 0; i < 4; i++)
	{
		if(m_sCharMoveInfo[i].byNum!=0xff)
		{
			if(__strcmp(m_sCharMoveInfo[i].szName, szName)==0)
			{
				m_sCharMoveInfo[i].byNum	= byNum;
				m_sCharMoveInfo[i].byResult	= byResult;
				return;
			}
		}		
	}

	for(i = 0; i < 4; i++)
	{
		if(m_sCharMoveInfo[i].byNum==0xff)
		{
			__lstrcpyn(m_sCharMoveInfo[i].szName, szName, MAX_CHARACTER_NAME_REAL_LENGTH);
			m_sCharMoveInfo[i].byNum	= byNum;
			m_sCharMoveInfo[i].byResult	= byResult;
			return;
		}		
	}
}

void CCharMoveRtWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;

	if(bActive==FALSE)
	{
		ShowSpriteAll();
				
		if(m_byTotalCnt==m_byMoveSuccess && m_byTotalCnt!=0)
			g_bCharMoveChk = FALSE;
		else
		{
			if(m_byTotalCnt!=0 && m_byMoveFail!=0)
			{
				// 이름 변환 작업 //
				CCharMoveEditWnd* pCharMoveEditWnd = CCharMoveEditWnd::GetInstance();
				
				for(int i = 0; i < 4; i++)
				{
					if(m_sCharMoveInfo[i].byResult==1)
						pCharMoveEditWnd->SetCharacter(m_sCharMoveInfo[i].szName, m_sCharMoveInfo[i].byNum, m_sCharMoveInfo[i].byResult);
				}
				pCharMoveEditWnd->SetActive();
			}			
		}
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		BYTE	byIndex = 0;
		BOOL	bChk	= FALSE;

		for(int i = 0; i < 4; i++)
		{
			if(m_sCharMoveInfo[i].byResult!=0)
				byIndex +=1;
			if(bChk && (m_sCharMoveInfo[i].byResult==2 || m_sCharMoveInfo[i].byResult==3))
			{
				byIndex += 4;
				bChk  = TRUE;
			}
		}
		
		if(byIndex>=6)
		{
			BYTE byCnt = 3;
			
			if(byIndex-5<=3)
				byCnt = BYTE(byIndex-5);
			else
				byCnt = 3;

			m_bySize = BYTE(16*byCnt);

			SetScalingObjY(SPR_OBJ_CHARMOVERT_WND2, 13.0f+byCnt);			
			SetPosY(float(768/2-(48+(13+byCnt)*16)/2));
			SetPosObjY(SPR_OBJ_CHARMOVERT_WND3, float(240+m_bySize));
			SetPosObjY(SPR_OBJ_CHARMOVERT_OK1, float(238+m_bySize));
			SetPosObjY(SPR_OBJ_CHARMOVERT_OK2, float(238+m_bySize));
			SetPosObjY(SPR_OBJ_CHARMOVERT_OK3, float(238+m_bySize));			
		}

		SetRender(SPR_OBJ_CHARMOVERT_WND1, TRUE);		
		SetRender(SPR_OBJ_CHARMOVERT_WND2, TRUE);
		SetRender(SPR_OBJ_CHARMOVERT_WND3, TRUE);
		SetRender(SPR_OBJ_CHARMOVERT_OK1, TRUE);						
	}
}

int CCharMoveRtWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk(m_bySize);

	if(m_bBtn && nRt!=2)
	{
		SetRender(SPR_OBJ_CHARMOVERT_OK1, TRUE);
		SetRender(SPR_OBJ_CHARMOVERT_OK2, FALSE);
		SetRender(SPR_OBJ_CHARMOVERT_OK3, FALSE);								
		m_bBtn = FALSE;
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
				m_bBtn	= TRUE;
				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_CHARMOVERT_OK1, FALSE);
				SetRender(SPR_OBJ_CHARMOVERT_OK2, FALSE);
				SetRender(SPR_OBJ_CHARMOVERT_OK3, TRUE);
				m_bBtn = TRUE;
			}			
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_CHARMOVERT_OK1, FALSE);
				SetRender(SPR_OBJ_CHARMOVERT_OK2, TRUE);
				SetRender(SPR_OBJ_CHARMOVERT_OK3, FALSE);
				m_bBtn = TRUE;
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