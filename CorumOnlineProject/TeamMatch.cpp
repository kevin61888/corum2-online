#include "stdafx.h"
#include "InitGame.h"
#include "NetworkClient.h"
#include "GroupWnd.h"
#include "GuildWarFinalSettingWnd.h"
#include "GuildWarStatusWnd.h"

void Send_CMD_TEAM_MATCH_INSERT(TEAM_MATCH_PACKET TeamMatch, WORD wType)
{
	TEAM_MATCH_PACKET TMPacket;

	TMPacket.wType = wType;

	switch(wType)
	{
	case CTWS_TM_INSERT:
	case CTWS_TM_REMOVE:
		{
			TMPacket.Body.Condtion = TeamMatch.Body.Condtion;
		}
		break;

	case CTWS_TM_LADDER_LIST:
	case CTWS_TM_GUILD_WAR_LIST:
		{
			TMPacket.Body.PageNum.iPageNumber = TeamMatch.Body.PageNum.iPageNumber;
		}
		break;

	case CTWS_TM_GUILD_WAR_REQUEST:
		{
			TMPacket.Body.GuildWarRequest = TeamMatch.Body.GuildWarRequest;
		}
		break;

	case CTWS_TM_GUILD_WAR_OBSERVE:
		{
			TMPacket.Body.ObserveIndex.iObserveIndex = TeamMatch.Body.ObserveIndex.iObserveIndex;
		}
		break;
	}

	CGroupWnd*	pGroupWnd = CGroupWnd::GetInstance();
	pGroupWnd->m_bGuildWarPacketSendEnable = TRUE;//나중에 빼자(서버하고 연결이 되면)
	
	g_pNet->SendMsg((char*)&TMPacket, TMPacket.GetPacketSize(), SERVER_INDEX_WORLD);
}


void Recv_CMD_TEAM_MATCH(char* pMsg, DWORD dwLen)
{
	TEAM_MATCH_PACKET* pPacket = (TEAM_MATCH_PACKET*)pMsg;
	CGroupWnd*	pGroupWnd = CGroupWnd::GetInstance();
	pGroupWnd->m_bGuildWarPacketSendEnable = TRUE;

//	pGroupWnd->m_nGuildWarRankListMaxPage	//값을 넣어야 한다.
//	pGroupWnd->m_nGuildWarRankListTotalCount
//	pGroupWnd->m_nGuildWarListMaxPage
//	pGroupWnd->m_nGuildWarListTotalCount
//	pGroupWnd->m_bGuildWarMatchEnable //결과에 따라서 값을 셋팅
/*	if(pPacket->wType == CTWS_TM_LADDER_LIST)
	{
		memcpy(pGroupWnd->m_LadderList, pPacket->Body.LadderList, sizeof(pPacket->Body.LadderList));
	}
	else if(pPacket->wType == CTWS_TM_GUILD_WAR_LIST)
	{
		memcpy(pGroupWnd->m_GuildWarList, pPacket->Body.GuildWarList, sizeof(pPacket->Body.LadderList));
	}*/
}

void CmdGuildMatchBegin( char* pMsg, DWORD dwLen )
{
//	CGuildWarFinalSettingWnd::GetInstance()->SetActive( TRUE );
}

void CmdGuildMatchBeginSetting( char* pMsg, DWORD dwLen )
{
	CGuildWarFinalSettingWnd*	pWnd = CGuildWarFinalSettingWnd::GetInstance();

	if( pWnd->GetActive() )
		pWnd->ProcessPacket( (GUILD_MATCHBEGIN_SETTING*)pMsg );
}

void CmdGuildMatchStatus( char* pMsg, DWORD dwLen )
{
	CGuildWarStatusWnd*	pWnd = CGuildWarStatusWnd::GetInstance();

	if( pWnd->GetActive() )
		pWnd->ProcessPacket( (GUILD_MATCH_STATUS*)pMsg );
}

void CmdGuildMatchStatusRespon( char* pMsg, DWORD dwLen )
{
	CGuildWarStatusWnd*	pWnd = CGuildWarStatusWnd::GetInstance();

	if( pWnd->GetActive() )
		pWnd->ProcessPacket( (GUILD_MATCH_STATUS_RESULT*)pMsg );
}
