// TopDungeonHelpWnd.cpp: implementation of the CTopDungeonHelpWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "TopDungeonHelpWnd.h"
#include "Interface.h"
#include "InitGame.h"
#include "CodeFun.h"
#include "NetworkClient.h"


CTopDungeonHelpWnd* CTopDungeonHelpWnd::c_pThis = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTopDungeonHelpWnd::CTopDungeonHelpWnd()
{

}

CTopDungeonHelpWnd::~CTopDungeonHelpWnd()
{

}
BOOL CTopDungeonHelpWnd::Init()
{
	m_byWorldNum = 1;	
	int nSize = 0;
	for(int i = HELP_TEXT_PAGE1_START+1; i <= HELP_TEXT_PAGE1_END; i++)
	{
		LP_HELP_INFO lpHelpInfo = (LP_HELP_INFO)g_pHelpInfoHash->GetData(i);
		
		if(lpHelpInfo)
		{
			nSize = lstrlen(lpHelpInfo->szText);
			InsertCheckInterface(lpHelpInfo->wLeft, 
				lpHelpInfo->wTop, int(lpHelpInfo->wLeft+(float)nSize*6.5f), 
				lpHelpInfo->wTop+15, BYTE(HELP_TEXT_CHECK_COMMAND1+i-HELP_TEXT_PAGE1_START-1), CHECK_BTN);
		}		
	}
	
	m_bInit	= TRUE;

	m_sHelpTextBuffer[0][0].wStart = HELP_TEXT_PAGE1_START;
	m_sHelpTextBuffer[0][0].wEnd = HELP_TEXT_PAGE1_END;
	m_sHelpTextBuffer[1][0].wStart = HELP_TEXT_PAGE2_1_START;
	m_sHelpTextBuffer[1][0].wEnd = HELP_TEXT_PAGE2_1_END;
	m_sHelpTextBuffer[1][1].wStart = HELP_TEXT_PAGE2_2_START;
	m_sHelpTextBuffer[1][1].wEnd = HELP_TEXT_PAGE2_2_END;
	m_sHelpTextBuffer[1][2].wStart = HELP_TEXT_PAGE2_3_START;
	m_sHelpTextBuffer[1][2].wEnd = HELP_TEXT_PAGE2_3_END;
	m_sHelpTextBuffer[2][0].wStart = HELP_TEXT_PAGE3_1_START;
	m_sHelpTextBuffer[2][0].wEnd = HELP_TEXT_PAGE3_1_END;
	m_sHelpTextBuffer[3][0].wStart = HELP_TEXT_PAGE4_1_START;
	m_sHelpTextBuffer[3][0].wEnd = HELP_TEXT_PAGE4_1_END;
	m_sHelpTextBuffer[4][0].wStart = HELP_TEXT_PAGE5_1_START;
	m_sHelpTextBuffer[4][0].wEnd = HELP_TEXT_PAGE5_1_END;

	
	return TRUE;
}

void CTopDungeonHelpWnd::RenderText()
{
	char szInfo[0xff] = {0, };
	if( m_byWorldNum == 1)
	{
		wsprintf(szInfo, "NPC %s", g_NPCTable[43].szName);//노엘
	}
	else
	{
		wsprintf(szInfo, "NPC %s", g_NPCTable[48].szName);//소니아
	}
	int nSize = lstrlen(szInfo);

	RenderFont(szInfo, m_fPosX+22, m_fPosX+22+nSize*7, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);
	RenderTextEx();
}
void CTopDungeonHelpWnd::RenderTextEx()
{	
//	int		nSize	= 0;
//	BYTE	byIndex	= 0;
//	RECT	rtPos;

	for(int i = 0; i <= m_sHelpTextBuffer[m_sPagePosition.byCommand][m_sPagePosition.byPage].wEnd-m_sHelpTextBuffer[m_sPagePosition.byCommand][m_sPagePosition.byPage].wStart; i++)
	{
		LP_HELP_INFO lpHelpInfo = 
			(LP_HELP_INFO)g_pHelpInfoHash->GetData(m_sHelpTextBuffer[m_sPagePosition.byCommand][m_sPagePosition.byPage].wStart+i);
		
		if(lpHelpInfo)
		{
			if( !m_sPagePosition.byCommand )//첫 페이지
			{
				if (!m_sPagePosition.byPage && m_bySelect == HELP_TEXT_CHECK_COMMAND1+i-1)
				{
					InterfaceTextRender(WORD(m_sHelpTextBuffer[m_sPagePosition.byCommand][m_sPagePosition.byPage].wStart+i), 0xff7f7fff);
				}
				else
				{
					InterfaceTextRender(WORD(m_sHelpTextBuffer[m_sPagePosition.byCommand][m_sPagePosition.byPage].wStart+i), 0xffffffff);
				}
			}
			else//나머지
			{
				InterfaceTextRender(WORD(m_sHelpTextBuffer[m_sPagePosition.byCommand][m_sPagePosition.byPage].wStart+i), 0xffffffff);
			}
			
		}	
	}		
	
}

void CTopDungeonHelpWnd::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CTopDungeonHelpWnd::SetOrder()
{
}

void CTopDungeonHelpWnd::SetActive(BOOL bActive)
{
	m_bActive	= bActive;
	
	if(bActive==FALSE)
	{		
		m_bSoundChk	= FALSE;	
		ShowSpriteAll();

		memset(&m_sPagePosition, 0, sizeof(m_sPagePosition));
		
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{		
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		memset(&m_sPagePosition, 0, sizeof(m_sPagePosition));

		if( m_byWorldNum == 1)
		{
			m_sHelpTextBuffer[0][0].wStart = HELP_TEXT_PAGE1_START;
			m_sHelpTextBuffer[0][0].wEnd = HELP_TEXT_PAGE1_END;
			m_sHelpTextBuffer[1][0].wStart = HELP_TEXT_PAGE2_1_START;
			m_sHelpTextBuffer[1][0].wEnd = HELP_TEXT_PAGE2_1_END;
			m_sHelpTextBuffer[1][1].wStart = HELP_TEXT_PAGE2_2_START;
			m_sHelpTextBuffer[1][1].wEnd = HELP_TEXT_PAGE2_2_END;
			m_sHelpTextBuffer[1][2].wStart = HELP_TEXT_PAGE2_3_START;
			m_sHelpTextBuffer[1][2].wEnd = HELP_TEXT_PAGE2_3_END;
			m_sHelpTextBuffer[2][0].wStart = HELP_TEXT_PAGE3_1_START;
			m_sHelpTextBuffer[2][0].wEnd = HELP_TEXT_PAGE3_1_END;
			m_sHelpTextBuffer[3][0].wStart = HELP_TEXT_PAGE4_1_START;
			m_sHelpTextBuffer[3][0].wEnd = HELP_TEXT_PAGE4_1_END;
			m_sHelpTextBuffer[4][0].wStart = HELP_TEXT_PAGE5_1_START;
			m_sHelpTextBuffer[4][0].wEnd = HELP_TEXT_PAGE5_1_END;
		}
		else if( m_byWorldNum == 2)
		{
			m_sHelpTextBuffer[0][0].wStart = HELP_TEXT_PAGE1_START_2WORLD;
			m_sHelpTextBuffer[0][0].wEnd = HELP_TEXT_PAGE1_END_2WORLD;
			m_sHelpTextBuffer[1][0].wStart = HELP_TEXT_PAGE2_1_START_2WORLD;
			m_sHelpTextBuffer[1][0].wEnd = HELP_TEXT_PAGE2_1_END_2WORLD;
			m_sHelpTextBuffer[1][1].wStart = HELP_TEXT_PAGE2_2_START_2WORLD;
			m_sHelpTextBuffer[1][1].wEnd = HELP_TEXT_PAGE2_2_END_2WORLD;
			m_sHelpTextBuffer[1][2].wStart = HELP_TEXT_PAGE2_3_START_2WORLD;
			m_sHelpTextBuffer[1][2].wEnd = HELP_TEXT_PAGE2_3_END_2WORLD;
			m_sHelpTextBuffer[2][0].wStart = HELP_TEXT_PAGE3_1_START_2WORLD;
			m_sHelpTextBuffer[2][0].wEnd = HELP_TEXT_PAGE3_1_END_2WORLD;
			m_sHelpTextBuffer[3][0].wStart = HELP_TEXT_PAGE4_1_START_2WORLD;
			m_sHelpTextBuffer[3][0].wEnd = HELP_TEXT_PAGE4_1_END_2WORLD;
			m_sHelpTextBuffer[4][0].wStart = HELP_TEXT_PAGE5_1_START_2WORLD;
			m_sHelpTextBuffer[4][0].wEnd = HELP_TEXT_PAGE5_1_END_2WORLD;
		}
		
		SetRender(SPR_OBJ_NOTICE_WND1, TRUE);
		SetRender(SPR_OBJ_NOTICE_WND2, TRUE);
		SetRender(SPR_OBJ_NOTICE_WND3, TRUE);
		SetRender(SPR_OBJ_NOTICE_CLOSE1, TRUE);	
		SetRender(BUTTON_INTERFACE_HELP_END1, TRUE);
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);		
	}
}

int CTopDungeonHelpWnd::CheckInterface()
{
	m_bMouseIcon = FALSE;
	
	int nRt = GetChk();

	SetRender(SPR_OBJ_HELP_OK1, FALSE);
	SetRender(SPR_OBJ_HELP_OK2, FALSE);
	SetRender(SPR_OBJ_HELP_OK3, FALSE);
	SetRender(SPR_OBJ_HELP_CANCEL1, FALSE);
	SetRender(SPR_OBJ_HELP_CANCEL2, FALSE);
	SetRender(SPR_OBJ_HELP_CANCEL3, FALSE);
	SetRender(SPR_OBJ_HELP_ONLY_OK1, FALSE);
	SetRender(SPR_OBJ_HELP_ONLY_OK2, FALSE);
	SetRender(SPR_OBJ_HELP_ONLY_OK3, FALSE);

	if( m_sPagePosition.byCommand == 1 && (m_sPagePosition.byPage == 0 || m_sPagePosition.byPage == 1) ||
		m_sPagePosition.byCommand == 2)
	{
		SetRender(SPR_OBJ_HELP_OK1, TRUE);
		SetRender(SPR_OBJ_HELP_CANCEL1, TRUE);
	}
	else if( m_sPagePosition.byCommand == 1 && m_sPagePosition.byPage == 2 ||
		m_sPagePosition.byCommand == 3 || m_sPagePosition.byCommand == 4)
	{
		SetRender(SPR_OBJ_HELP_ONLY_OK1, TRUE);
	}

	InitValue(nRt);
	
	MOUSECHECK eMouseCheck = GetMouseCheck(nRt);
	m_bySelect = (BYTE)nRt;
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
/*	case 4:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				SetRender(SPR_OBJ_HELP_NEXT1, TRUE);
				SetRender(SPR_OBJ_HELP_NEXT2, FALSE);
				SetRender(SPR_OBJ_HELP_NEXT3, FALSE);	
				m_sPagePosition.byPage++;
				switch(m_sPagePosition.byCommand)
				{
				case 0:
					if(m_sPagePosition.byPage > 3)
						m_sPagePosition.byPage = 0;
					break;
				case 1:
					if(m_sPagePosition.byPage > 2)
						m_sPagePosition.byPage = 0;
					break;
				case 2:
					if(m_sPagePosition.byPage > 1)
						m_sPagePosition.byPage = 0;
					break;
				case 3:
					if(m_sPagePosition.byPage > 1)
						m_sPagePosition.byPage = 0;
					break;
				}
				
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				SetRender(SPR_OBJ_HELP_NEXT1, FALSE);
				SetRender(SPR_OBJ_HELP_NEXT2, TRUE);
				SetRender(SPR_OBJ_HELP_NEXT3, FALSE);
				m_bBtn = TRUE;
			}
			else if(eMouseCheck == MOUSE_DOWN)
			{
				SetRender(SPR_OBJ_HELP_NEXT1, FALSE);
				SetRender(SPR_OBJ_HELP_NEXT2, FALSE);
				SetRender(SPR_OBJ_HELP_NEXT3, TRUE);
				m_bBtn = TRUE;
			}
			
			SetMouseCheck(nRt, eMouseCheck);
		}break;
*/
	case 5://확인버튼(오른쪽에 취소버튼 있을때)
		{
			if(eMouseCheck==MOUSE_DOWN)//마우스 눌렀을때 모양으로 버튼만 바꿔준다.
			{
				if( m_sPagePosition.byCommand == 1 && (m_sPagePosition.byPage == 0 || m_sPagePosition.byPage == 1) ||
					m_sPagePosition.byCommand == 2)
				{
					SetRender(SPR_OBJ_HELP_OK1, FALSE);
					SetRender(SPR_OBJ_HELP_OK2, FALSE);
					SetRender(SPR_OBJ_HELP_OK3, TRUE);
				}
			}
			else if(eMouseCheck==MOUSE_UP)//처리 프로세스
			{
				switch( m_sPagePosition.byCommand )
				{
				case 1:
					{
						if( m_sPagePosition.byPage == 0 || m_sPagePosition.byPage == 1 )
						{
							m_sPagePosition.byPage++;
						}

						if( m_sPagePosition.byPage == 2 )
						{
							SetRender(SPR_OBJ_HELP_OK1, FALSE);
							SetRender(SPR_OBJ_HELP_OK2, FALSE);
							SetRender(SPR_OBJ_HELP_OK3, FALSE);
							SetRender(SPR_OBJ_HELP_CANCEL1, FALSE);
							SetRender(SPR_OBJ_HELP_ONLY_OK1, TRUE);
						}
					}
					break;
				case 2:
					{
						if( m_byWorldNum == 1 )
						{
							CTDS_PORTAL_MOVE packet;	
							packet.wPortalItemID    = 0;
							packet.wToDungeonID		= 3028;					
							packet.bLayerNo			= 0;
							packet.v2Pos.x			= (float)1800;
							packet.v2Pos.y			= (float)2600;
							g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
							g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
						}
						else if( m_byWorldNum == 2 )
						{
							CTDS_PORTAL_MOVE packet;	
							packet.wPortalItemID    = 0;
							packet.wToDungeonID		= 3027;					
							packet.bLayerNo			= 0;
							packet.v2Pos.x			= (float)1900;
							packet.v2Pos.y			= (float)2600;
							g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_ZONE);
							g_pMainPlayer->SetStatus(UNIT_STATUS_PORTAL_MOVING);
						}
						
						// 포탈이 들어가야 된다., 돈도 빼야 한다.. 50000
						SetActive(FALSE);
					}
					break;
				}
			}
			else if(eMouseCheck==MOUSE_OVER)//마우스 가져다 댄 모양으로 버튼만 바꿔준다.
			{
				if( m_sPagePosition.byCommand == 1 && (m_sPagePosition.byPage == 0 || m_sPagePosition.byPage == 1) ||
					m_sPagePosition.byCommand == 2)
				{
					SetRender(SPR_OBJ_HELP_OK1, FALSE);
					SetRender(SPR_OBJ_HELP_OK2, TRUE);
				}
			}
		}
		break;

	case 6://취소버튼(오른쪽에 확인버튼 있을때), 확인버튼
		{
			if( m_sPagePosition.byCommand )
			{
				if(eMouseCheck==MOUSE_DOWN)//마우스 눌렀을때 모양으로 버튼만 바꿔준다.
				{
					if( m_sPagePosition.byCommand == 1 && (m_sPagePosition.byPage == 0 || m_sPagePosition.byPage == 1) ||
					m_sPagePosition.byCommand == 2)
					{
						SetRender(SPR_OBJ_HELP_CANCEL1, FALSE);
						SetRender(SPR_OBJ_HELP_CANCEL2, FALSE);
						SetRender(SPR_OBJ_HELP_CANCEL3, TRUE);
					}
					else
					{
						SetRender(SPR_OBJ_HELP_ONLY_OK1, FALSE);
						SetRender(SPR_OBJ_HELP_ONLY_OK2, FALSE);
						SetRender(SPR_OBJ_HELP_ONLY_OK3, TRUE);
					}
				}
				else if(eMouseCheck==MOUSE_UP)//처리 프로세스
				{
					SetActive(FALSE);
				}
				else if(eMouseCheck==MOUSE_OVER)//마우스 가져다 댄 모양으로 버튼만 바꿔준다.
				{
					if( m_sPagePosition.byCommand == 1 && (m_sPagePosition.byPage == 0 || m_sPagePosition.byPage == 1) ||
						m_sPagePosition.byCommand == 2)
					{
						SetRender(SPR_OBJ_HELP_CANCEL1, FALSE);
						SetRender(SPR_OBJ_HELP_CANCEL2, TRUE);
					}
					else
					{
						SetRender(SPR_OBJ_HELP_ONLY_OK1, FALSE);
						SetRender(SPR_OBJ_HELP_ONLY_OK2, TRUE);
					}
				}
			}
		}
		break;

	case HELP_TEXT_CHECK_COMMAND1:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				m_sPagePosition.byCommand = 1;
				SetRender(SPR_OBJ_HELP_OK1, TRUE);
				SetRender(SPR_OBJ_HELP_CANCEL1, TRUE);
			}
		}
		break;
	case HELP_TEXT_CHECK_COMMAND2:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				m_sPagePosition.byCommand = 2;
				SetRender(SPR_OBJ_HELP_OK1, TRUE);
				SetRender(SPR_OBJ_HELP_CANCEL1, TRUE);
			}
		}
		break;
	case HELP_TEXT_CHECK_COMMAND3:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				m_sPagePosition.byCommand = 3;
				SetRender(SPR_OBJ_HELP_ONLY_OK1, TRUE);
			}
		}
		break;
	case HELP_TEXT_CHECK_COMMAND4:
		{
			if(eMouseCheck==MOUSE_UP)
			{
				m_sPagePosition.byCommand = 4;
				SetRender(SPR_OBJ_HELP_ONLY_OK1, TRUE);
			}
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
		return 1;
	
	if(IsReturn(nRt, eMouseCheck))
		return 1;		
	
	return 0;
}
