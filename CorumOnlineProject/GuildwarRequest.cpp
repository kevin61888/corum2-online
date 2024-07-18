// GuildWarRequest.cpp: implementation of the CGuildWarRequest class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuildWarRequest.h"
#include "GameControl.h"
#include "User.h"
#include "UserInterface.h"
#include "Interface.h"
#include "message.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "CodeFun.h"
#include "RivalGuildWar.h"
#include "Chat.h"


CGuildWarRequest* CGuildWarRequest::c_pThis = NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuildWarRequest::CGuildWarRequest()
{

}

CGuildWarRequest::~CGuildWarRequest()
{

}


BOOL CGuildWarRequest::Init()
{
	m_byDropDownStatus = GRDDS_NONE;
	m_nGameForm = 0;
	m_nGameTime = 0;
	m_nGameStaff = 0;
//	m_bReRequestEnable = FALSE;
	m_byBtnRequest = 0;
	m_byBtnReRequest = 0;
	m_byBtnCancel = 0;

	ZeroMemory(&m_RankingData, sizeof(m_RankingData));


	m_bInit = TRUE;

//	wsprintf(m_szForm[0], "All");
	wsprintf(m_szForm[0], g_Message[ETC_MESSAGE1349].szMessage);//킬링필드

	wsprintf(m_szTime[0], "10%s", g_Message[ETC_MESSAGE1361].szMessage);//분
	wsprintf(m_szTime[1], "15%s", g_Message[ETC_MESSAGE1361].szMessage);//분
	wsprintf(m_szTime[2], "18%s", g_Message[ETC_MESSAGE1361].szMessage);//분
	wsprintf(m_szTime[3], "20%s", g_Message[ETC_MESSAGE1361].szMessage);//분

	wsprintf(m_szStaff[0], "6");
	wsprintf(m_szStaff[1], "9");
	wsprintf(m_szStaff[2], "12");
	wsprintf(m_szStaff[3], "15");
	wsprintf(m_szStaff[4], "20");

	return TRUE;
}
		
void CGuildWarRequest::RenderText()
{
	const int nFixTextPosX1 = 15;
	const int nFixTextY1 = 35;
	const int nFixTextPosY1 = 54;
	const int nFixTextPosY2 = 50;

	const int nDropDownTextPosX1 = 90;

	int i;

	int nSize = lstrlen(g_Message[ETC_MESSAGE1343].szMessage);//길드전 신청
	RenderFont(g_Message[ETC_MESSAGE1343].szMessage, m_fPosX+20, m_fPosX+20+nSize*7, m_fPosZ+5, m_fPosZ+19, GetStartOrder()+1);//길드전 신청

	nSize = lstrlen(g_Message[ETC_MESSAGE1339].szMessage);//경기방식
	RenderFont(g_Message[ETC_MESSAGE1339].szMessage, m_fPosX+nFixTextPosX1, m_fPosX+nFixTextPosX1+nSize*7, m_fPosZ+nFixTextY1, m_fPosZ+nFixTextY1+14, GetStartOrder()+1);//경기방식
	nSize = lstrlen(g_Message[ETC_MESSAGE1340].szMessage);
	RenderFont(g_Message[ETC_MESSAGE1340].szMessage, m_fPosX+nFixTextPosX1, m_fPosX+nFixTextPosX1+nSize*7, m_fPosZ+nFixTextY1+nFixTextPosY2*1, m_fPosZ+nFixTextY1+nFixTextPosY2*1+14, GetStartOrder()+1);//경기시간
	nSize = lstrlen(g_Message[ETC_MESSAGE1362].szMessage);
	RenderFont(g_Message[ETC_MESSAGE1362].szMessage, m_fPosX+nFixTextPosX1, m_fPosX+nFixTextPosX1+nSize*7, m_fPosZ+nFixTextY1+nFixTextPosY2*2, m_fPosZ+nFixTextY1+nFixTextPosY2*2+14, GetStartOrder()+1);//경기인원

	if(m_nGameForm > -1)
	{
		nSize = lstrlen(m_szForm[m_nGameForm]);
		RenderFont(m_szForm[m_nGameForm], m_fPosX+nDropDownTextPosX1, m_fPosX+nDropDownTextPosX1+nSize*7, m_fPosZ+nFixTextPosY1, m_fPosZ+nFixTextPosY1+14, GetStartOrder()+2);
	}

	if(m_nGameTime > -1)
	{
		nSize = lstrlen(m_szTime[m_nGameTime]);
		RenderFont(m_szTime[m_nGameTime], m_fPosX+nDropDownTextPosX1, m_fPosX+nDropDownTextPosX1+nSize*7, m_fPosZ+nFixTextPosY1+nFixTextPosY2, m_fPosZ+nFixTextPosY1+nFixTextPosY2+14, GetStartOrder()+2);
	}

	if(m_nGameStaff > -1)
	{
		nSize = lstrlen(m_szStaff[m_nGameStaff]);
		RenderFont(m_szStaff[m_nGameStaff], m_fPosX+nDropDownTextPosX1, m_fPosX+nDropDownTextPosX1+nSize*7, m_fPosZ+nFixTextPosY1+nFixTextPosY2*2, m_fPosZ+nFixTextPosY1+nFixTextPosY2*2+14, GetStartOrder()+2);
	}

	if(m_byDropDownStatus == GRDDS_FORM)
	{
		for(i=0; i<1; i++)
		{
			nSize = lstrlen(m_szForm[i]);
			RenderFont(m_szForm[i], m_fPosX+nDropDownTextPosX1, m_fPosX+nDropDownTextPosX1+nSize*7, m_fPosZ+nFixTextPosY1+16+i*15, m_fPosZ+nFixTextPosY1+16+i*15+14, GetStartOrder()+4);
		}
	}
	else if(m_byDropDownStatus == GRDDS_TIME)
	{
		for(i=0; i<4; i++)
		{
			nSize = lstrlen(m_szTime[i]);
			RenderFont(m_szTime[i], m_fPosX+nDropDownTextPosX1, m_fPosX+nDropDownTextPosX1+nSize*7, m_fPosZ+nFixTextPosY1+nFixTextPosY2+16+i*15, m_fPosZ+nFixTextPosY1+nFixTextPosY2+16+i*15+14, GetStartOrder()+4);
		}
	}
	else if(m_byDropDownStatus == GRDDS_STAFF)
	{
		for(i=0; i<5; i++)
		{
			nSize = lstrlen(m_szStaff[i]);
			RenderFont(m_szStaff[i], m_fPosX+nDropDownTextPosX1, m_fPosX+nDropDownTextPosX1+nSize*7, m_fPosZ+nFixTextPosY1+nFixTextPosY2*2+16+i*15, m_fPosZ+nFixTextPosY1+nFixTextPosY2*2+16+i*15+14, GetStartOrder()+4);
		}
	}
/*
	char szTmp[100];

	int nPosX = 10;
	int nPosZ = 170;
	int nPosZadd = 15;

	ZeroMemory(szTmp, sizeof(szTmp));
	wsprintf(szTmp, g_Message[ETC_MESSAGE1363].szMessage);//신청 길드 정보
	nSize = lstrlen(szTmp);
	RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+nSize*7, m_fPosZ+nPosZ, m_fPosZ+nPosZ+14, GetStartOrder()+1);

	ZeroMemory(szTmp, sizeof(szTmp));
	wsprintf(szTmp, "%s(GM:%s)", m_RankingData.szGuildName, m_RankingData.szGuildMaster);
	nSize = lstrlen(szTmp);
	RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+nSize*7, m_fPosZ+nPosZ+nPosZadd, m_fPosZ+nPosZ+nPosZadd+14, GetStartOrder()+1);

	ZeroMemory(szTmp, sizeof(szTmp));
	wsprintf(szTmp, g_Message[ETC_MESSAGE1364].szMessage, m_RankingData.iGuildTotalCount);//길드원 : %d명
	nSize = lstrlen(szTmp);
	RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+nSize*7, m_fPosZ+nPosZ+nPosZadd*2, m_fPosZ+nPosZ+nPosZadd*2+14, GetStartOrder()+1);

	ZeroMemory(szTmp, sizeof(szTmp));
	wsprintf(szTmp, g_Message[ETC_MESSAGE1365].szMessage, 1, m_RankingData.WeekPoint[0]+m_RankingData.WeekPoint[1], m_RankingData.iRank[0], 
		m_RankingData.WeekPoint[0], m_RankingData.WeekPoint[1]);//%d주간 점수 %d점 %d위 (%d/%d)
	nSize = lstrlen(szTmp);
	RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+nSize*7, m_fPosZ+nPosZ+nPosZadd*4, m_fPosZ+nPosZ+nPosZadd*4+14, GetStartOrder()+1);

	ZeroMemory(szTmp, sizeof(szTmp));
	wsprintf(szTmp, g_Message[ETC_MESSAGE1365].szMessage, 5, m_RankingData.MonthPoint[0]+m_RankingData.MonthPoint[1], m_RankingData.iRank[1], 
		m_RankingData.MonthPoint[0], m_RankingData.MonthPoint[1]);//%d주간 점수 %d점 %d위 (%d/%d)
	nSize = lstrlen(szTmp);
	RenderFont(szTmp, m_fPosX+nPosX, m_fPosX+nPosX+nSize*7, m_fPosZ+nPosZ+nPosZadd*5, m_fPosZ+nPosZ+nPosZadd*5+14, GetStartOrder()+1);
	*/
}

void CGuildWarRequest::Remove()
{
	RemoveAllData();
	m_bActive	= FALSE;
	m_bInit		= FALSE;
}

void CGuildWarRequest::SetOrder()
{
}

void CGuildWarRequest::SetActive(BOOL bActive)
{
	m_bActive = bActive;		
	
	if(bActive==FALSE)
	{
		m_bSoundChk	= FALSE;
		m_bReRequestEnable = FALSE;
		ShowSpriteAll();
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDCLOSE, g_v3InterfaceSoundPos, FALSE);
	}
	else
	{
		CInterface::GetInstance()->SetWnd((BYTE)m_nIndex);

		SetRender(SPR_OBJ_GUILDWAR_REQUEST_WINDOWS1, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_REQUEST_WINDOWS2, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_REQUEST_WINDOWS3, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_REQUEST_CLOSE1, TRUE);

		SetRenderStatus();

		m_byDropDownStatus = GRDDS_NONE;
//		m_nGameForm = 0;
//		m_nGameTime = 0;
//		m_nGameStaff = 0;
//		m_bReRequestEnable = FALSE;
				
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_WNDOPEN, g_v3InterfaceSoundPos, FALSE);
	}
}

int	CGuildWarRequest::CheckInterface()
{
	m_bMouseIcon = FALSE;

	int nRt = GetChk();

	if(m_byDropDownStatus)
	{
		if( nRt != 0 && nRt != 1 )
		{
			int nTempRt = 0;
			
			nTempRt = GetChk(0, FALSE, m_byDropDownStatus);
			if(nTempRt != 0 && nTempRt != 1)
			{
				nRt = nTempRt;
			}
		}
	}

	SetRenderStatus();

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
				SetRender(SPR_OBJ_GUILDWAR_REQUEST_CLOSE1, FALSE);
				SetRender(SPR_OBJ_GUILDWAR_REQUEST_CLOSE2, TRUE);
			}						
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;
	case 3:			
		{
			SetMouseCheck(nRt, eMouseCheck);
		}
		break;	
	case 4://경기방식 드롭다운 버튼
		{
			if(eMouseCheck==MOUSE_UP)
			{
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byDropDownStatus == GRDDS_FORM)
					m_byDropDownStatus = GRDDS_NONE;
				else
					m_byDropDownStatus = GRDDS_FORM;

				SetRenderStatus();
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
			}
		}
		break;
	case 5://경기시간 드롭다운 버튼
		{
			if(eMouseCheck==MOUSE_UP)
			{
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byDropDownStatus == GRDDS_TIME)
					m_byDropDownStatus = GRDDS_NONE;
				else
					m_byDropDownStatus = GRDDS_TIME;

				SetRenderStatus();
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
			}
		}
		break;
	case 6://경기인원 드롭다운 버튼
		{
			if(eMouseCheck==MOUSE_UP)
			{
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_byDropDownStatus == GRDDS_STAFF)
					m_byDropDownStatus = GRDDS_NONE;
				else
					m_byDropDownStatus = GRDDS_STAFF;

				SetRenderStatus();
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
			}
		}
		break;
	case 7://신청 버튼
		{
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_nGameForm > -1 && m_nGameTime > -1 && m_nGameStaff > -1)
				{
					if(m_bReRequestEnable)
					{
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY1, FALSE);
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY2, TRUE);
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY3, FALSE);
						TEAM_MATCH_PACKET TeamMatch;
						lstrcpy(TeamMatch.Body.GuildWarRequest.szCasterName, g_pMainPlayer->m_szName );
						lstrcpy(TeamMatch.Body.GuildWarRequest.szTargetName, m_szRequestName);
						TeamMatch.Body.GuildWarRequest.Sequence = GWRS_APPLY;
						TeamMatch.Body.GuildWarRequest.MatchType = GetFormConvert(m_nGameForm);
						TeamMatch.Body.GuildWarRequest.MatchTime = GetWarTime(m_nGameTime);
						TeamMatch.Body.GuildWarRequest.MatchNumber = GetWarNumber(m_nGameStaff);
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_REQUEST);
					}
					else
					{
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST1, FALSE);
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST2, TRUE);
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST3, FALSE);
						TEAM_MATCH_PACKET TeamMatch;
						
						lstrcpy(TeamMatch.Body.GuildWarRequest.szCasterName, g_pMainPlayer->m_szName);
						lstrcpy(TeamMatch.Body.GuildWarRequest.szTargetName, m_szRequestName);
						TeamMatch.Body.GuildWarRequest.Sequence = GWRS_REQUEST;
						TeamMatch.Body.GuildWarRequest.MatchType = GetFormConvert(m_nGameForm);
						TeamMatch.Body.GuildWarRequest.MatchTime =	GetWarTime(m_nGameTime);
						TeamMatch.Body.GuildWarRequest.MatchNumber = GetWarNumber(m_nGameStaff);
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_REQUEST);

						if(m_dwTargetGuildId > 0)
						{
							LP_GUILD_DATA lpGuildData = g_pGuildInfoHash->GetData(m_dwTargetGuildId);

							if(lpGuildData)
							{
								int nSize = lstrlen(lpGuildData->szGuildName);
								char szTemp[255] = {0,};
								wsprintf(szTemp, g_Message[ETC_MESSAGE1388].szMessage, lpGuildData->szGuildName );// "'%s'길드의 수락을 기다리고 있습니다."
								DisplayMessageAdd(szTemp, TEXT_COLOR_YELLOW); 
							}								
						}
					}
					SetActive(FALSE);
					// 서버로 신청 내용을 날려준다.
				}
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_bReRequestEnable)
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY2, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY3, TRUE);
				}
				else
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST2, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST3, TRUE);
				}
				m_byBtnRequest = 1;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_bReRequestEnable)
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY2, TRUE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY3, FALSE);
				}
				else
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST2, TRUE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST3, FALSE);
				}
				m_byBtnRequest = 2;
			}
		}
		break;
	case 8://수정신청 버튼
		{
			if( m_bReRequestEnable )
			{
				if(eMouseCheck==MOUSE_UP)
				{
					if(m_nGameForm > -1 && m_nGameTime > -1 && m_nGameStaff > -1)
					{
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST1, FALSE);
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST2, TRUE);
						SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST3, FALSE);
						TEAM_MATCH_PACKET TeamMatch;
						lstrcpy(TeamMatch.Body.GuildWarRequest.szCasterName, g_pMainPlayer->m_szName );
						lstrcpy(TeamMatch.Body.GuildWarRequest.szTargetName, m_szRequestName);
						
						TeamMatch.Body.GuildWarRequest.Sequence = GWRS_REREQUEST;
						TeamMatch.Body.GuildWarRequest.MatchType = GetFormConvert(m_nGameForm);
						TeamMatch.Body.GuildWarRequest.MatchTime = GetWarTime(m_nGameTime);
						TeamMatch.Body.GuildWarRequest.MatchNumber = GetWarNumber(m_nGameStaff);
						Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_REQUEST);
						// 서버로 수정신청 내용을 날려준다.
//						int nSize = lstrlen(m_RankingData.szGuildName);
//						char szTemp[255] = {0,};
//						wsprintf(szTemp, g_Message[ETC_MESSAGE1388].szMessage, m_RankingData.szGuildName);// "'%s'길드의 수락을 기다리고 있습니다."
//						DisplayMessageAdd(szTemp, TEXT_COLOR_YELLOW); 
					}
				}
				else if(eMouseCheck==MOUSE_DOWN)
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST2, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST3, TRUE);
					m_byBtnReRequest = 1;
				}
				else if(eMouseCheck==MOUSE_OVER)
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST2, TRUE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST3, FALSE);
					m_byBtnReRequest = 2;
				}
			}
		}
		break;
	case 9://취소 버튼
		{
			if(eMouseCheck==MOUSE_UP)
			{
				if(m_bReRequestEnable)
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT2, TRUE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT3, FALSE);

					TEAM_MATCH_PACKET TeamMatch;
					lstrcpy(TeamMatch.Body.GuildWarRequest.szCasterName, g_pMainPlayer->m_szName );
					lstrcpy(TeamMatch.Body.GuildWarRequest.szTargetName, m_szRequestName);
					TeamMatch.Body.GuildWarRequest.Sequence = GWRS_REJECT;
					TeamMatch.Body.GuildWarRequest.MatchType = GetFormConvert(m_nGameForm);
					TeamMatch.Body.GuildWarRequest.MatchTime = GetWarTime(m_nGameTime);
					TeamMatch.Body.GuildWarRequest.MatchNumber = GetWarNumber(m_nGameStaff);
					Cmd_CTWS_GuildWarPacket(TeamMatch, CTWS_TM_GUILD_WAR_REQUEST);
				}
				else
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL2, TRUE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL3, FALSE);
				}

				SetActive(FALSE);
			}
			else if(eMouseCheck==MOUSE_DOWN)
			{
				if(m_bReRequestEnable)
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT2, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT3, TRUE);
				}
				else
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL2, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL3, TRUE);
				}
				m_byBtnCancel = 1;
			}
			else if(eMouseCheck==MOUSE_OVER)
			{
				if(m_bReRequestEnable)
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT2, TRUE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT3, FALSE);
				}
				else
				{
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL1, FALSE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL2, TRUE);
					SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL3, FALSE);
				}
				m_byBtnCancel = 2;
			}
		}
		break;	
	// 각 라인을 선택했을때(10개씩 예약)
	case GWR_DROPDOWN_SELECT_FORM_START:
//	case GWR_DROPDOWN_SELECT_FORM_START+1:
	case GWR_DROPDOWN_SELECT_TIME_START:
	case GWR_DROPDOWN_SELECT_TIME_START+1:
	case GWR_DROPDOWN_SELECT_TIME_START+2:
	case GWR_DROPDOWN_SELECT_TIME_START+3:
	case GWR_DROPDOWN_SELECT_STAFF_START:
	case GWR_DROPDOWN_SELECT_STAFF_START+1:
	case GWR_DROPDOWN_SELECT_STAFF_START+2:
	case GWR_DROPDOWN_SELECT_STAFF_START+3:
	case GWR_DROPDOWN_SELECT_STAFF_START+4:
		{
			if(m_byDropDownStatus != GRDDS_NONE)
			{
				if(m_byDropDownStatus == GRDDS_FORM)
				{
					if(eMouseCheck==MOUSE_DOWN)
					{
						m_nGameForm = nRt - GWR_DROPDOWN_SELECT_FORM_START;
						m_byDropDownStatus = 0;
					}
					else if(eMouseCheck==MOUSE_OVER)
					{
					}
				}
				else if(m_byDropDownStatus == GRDDS_TIME)
				{
					if(eMouseCheck==MOUSE_DOWN)
					{
						m_nGameTime = nRt - GWR_DROPDOWN_SELECT_TIME_START;
						m_byDropDownStatus = 0;
					}
					else if(eMouseCheck==MOUSE_OVER)
					{
					}
				}
				else if(m_byDropDownStatus == GRDDS_STAFF)
				{
					if(eMouseCheck==MOUSE_DOWN)
					{
						m_nGameStaff = nRt - GWR_DROPDOWN_SELECT_STAFF_START;
						m_byDropDownStatus = 0;
					}
					else if(eMouseCheck==MOUSE_OVER)
					{
					}
				}
			}
		}
		break;
	}

	SetMouseCheck(nRt, eMouseCheck);

	if(MouseUp() && GetActive())
	{
		SetRender(SPR_OBJ_GUILDWAR_REQUEST_CLOSE1, TRUE);
		SetRender(SPR_OBJ_GUILDWAR_REQUEST_CLOSE2, FALSE);		
	}

	MouseChk(nRt);
	
	if(SetMoveWnd(nRt))	
		return CInterface::GetInstance()->GetSetWnd();
	
	if(IsReturn(nRt, eMouseCheck))
		return CInterface::GetInstance()->GetSetWnd();

	return 0;
}

void CGuildWarRequest::SetRenderStatus()
{
	for(int i=BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST1; i<=BUTTON_OBJ_GUILDWAR_REQUEST_APPLY3; i++)
	{
		SetRender(i, FALSE);
	}

	SetRender(DROPDOWN_OBJ_FORM_LEFT, TRUE);
	SetRender(DROPDOWN_OBJ_FORM_MIDDLE, TRUE);
	if(m_byDropDownStatus == 1)
	{
		SetRender(DROPDOWN_OBJ_FORM_RIGHT2, TRUE);
		SetRender(DROPDOWN_OBJ_FORM_BOARD, TRUE);
	}
	else
		SetRender(DROPDOWN_OBJ_FORM_RIGHT1, TRUE);

	SetRender(DROPDOWN_OBJ_TIME_LEFT, TRUE);
	SetRender(DROPDOWN_OBJ_TIME_MIDDLE, TRUE);
	if(m_byDropDownStatus == 2)
	{
		SetRender(DROPDOWN_OBJ_TIME_RIGHT2, TRUE);
		SetRender(DROPDOWN_OBJ_TIME_BOARD, TRUE);
	}
	else
		SetRender(DROPDOWN_OBJ_TIME_RIGHT1, TRUE);

	SetRender(DROPDOWN_OBJ_STAFF_LEFT, TRUE);
	SetRender(DROPDOWN_OBJ_STAFF_MIDDLE, TRUE);
	if(m_byDropDownStatus == 3)
	{
		SetRender(DROPDOWN_OBJ_STAFF_RIGHT2, TRUE);
		SetRender(DROPDOWN_OBJ_STAFF_BOARD, TRUE);
	}
	else
		SetRender(DROPDOWN_OBJ_STAFF_RIGHT1, TRUE);

	if(m_bReRequestEnable)
	{
		if(m_byBtnRequest == 1)//눌렸을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY3, TRUE);
		}
		else if(m_byBtnRequest == 2)//오버랩 됐을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY2, TRUE);
		}
		else
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_APPLY1, TRUE);
		}


		if(m_byBtnReRequest == 1)//눌렸을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST3, TRUE);
		}
		else if(m_byBtnReRequest == 2)//오버랩 됐을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST2, TRUE);
		}
		else
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REREQUEST1, TRUE);
		}

		if(m_byBtnCancel == 1)//눌렸을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT3, TRUE);
		}
		else if(m_byBtnCancel == 2)//오버랩 됐을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT2, TRUE);
		}
		else
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REJECT1, TRUE);
		}
	}
	else
	{
		if(m_byBtnRequest == 1)//눌렸을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST3, TRUE);
		}
		else if(m_byBtnRequest == 2)//오버랩 됐을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST2, TRUE);
		}
		else
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_REQUEST1, TRUE);
		}

		if(m_byBtnCancel == 1)//눌렸을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL3, TRUE);
		}
		else if(m_byBtnCancel == 2)//오버랩 됐을때
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL2, TRUE);
		}
		else
		{
			SetRender(BUTTON_OBJ_GUILDWAR_REQUEST_CANCEL1, TRUE);
		}
	}
	m_byBtnRequest = 0;
	m_byBtnReRequest = 0;
	m_byBtnCancel = 0;
}

void CGuildWarRequest::SetRankData(GUILD_LADDER_DATA LadderData)
{
	memcpy(&m_RankingData, &LadderData, sizeof(LadderData));
}
