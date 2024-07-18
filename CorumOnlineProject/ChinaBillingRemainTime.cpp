#include "stdafx.h"
#include "ChinaBillingRemainTime.h"
#include "message.h"
#include "InitGame.h"
#include "ErrorWnd.h"
#include "GameDefaultWnd.h"
#include "WorldWnd.h"
#include "LoginProcess.h"
#include "InputManager.h"
#include "Interface.h"

CHINA_REMAINED_TIME g_ChinaBillingRemainedTime;



void SetChinaBillingRemainedTime(DWORD dwType, DWORD dwTime)
{
	g_ChinaBillingRemainedTime.bReceived = TRUE;

	g_ChinaBillingRemainedTime.dwRegisteredTime = timeGetTime();
	g_ChinaBillingRemainedTime.dwTimeType = dwType;
	
	switch(dwType)
	{
	case china_billing_time_type_none		:
		g_ChinaBillingRemainedTime.dwExpireDate = -1;
		g_ChinaBillingRemainedTime.dwRemainedTime = (DWORD)-1;
		break;

	case china_billing_time_type_second		:
		g_ChinaBillingRemainedTime.dwExpireDate = -1;
		g_ChinaBillingRemainedTime.dwRemainedTime = dwTime;
		break;

	case china_billing_time_type_timestamp	:
		g_ChinaBillingRemainedTime.dwExpireDate = dwTime;
		g_ChinaBillingRemainedTime.dwRemainedTime = (DWORD)-1;
		break;
	}
}

DWORD GetChinaBillingRemainedTime()
{
	if(!g_ChinaBillingRemainedTime.IsTimeSet()) return (DWORD)-1;

	switch(g_ChinaBillingRemainedTime.dwTimeType) 
	{
	case china_billing_time_type_none:
		return (DWORD)-1;
		break;

	case china_billing_time_type_second:
		{
			// 지난 시간을 구한다. 단위는 초!
			DWORD dwPastTime = (timeGetTime() - g_ChinaBillingRemainedTime.dwRegisteredTime)/1000; 
			return (g_ChinaBillingRemainedTime.dwRemainedTime - dwPastTime < 0) ? 
				0 : g_ChinaBillingRemainedTime.dwRemainedTime - dwPastTime;
		}
		break;

	case china_billing_time_type_timestamp:
		{
			time_t CurrentDate; // 현재일.
			time(&CurrentDate);

			double time = difftime(g_ChinaBillingRemainedTime.dwExpireDate, CurrentDate);
			return (time < 0) ? 0 : (DWORD)time;
		}
		break;
	}
	return 0;
}

LPCSTR GetChinaBillingRemainedTime(BOOL bRemainedTime)
{
	static char szRemainTimeMsg[0xff];

	if(g_ChinaBillingRemainedTime.IsTimeSet())	return NULL;

	switch(g_ChinaBillingRemainedTime.dwTimeType) 
	{
	case china_billing_time_type_none:
		lstrcpy(szRemainTimeMsg, g_Message[ETC_MESSAGE1191].szMessage); // "귀하는 무료 고객이십니다."
		break;

	case china_billing_time_type_second:
		{
			// 지난 시간을 구한다. 단위는 초!
		//	DWORD dwPastTime = (timeGetTime() - g_ChinaBillingRemainedTime.dwRegisteredTime)/1000; 
			DWORD dwTime = GetChinaBillingRemainedTime();

			// 무조건 남은 시간 출력.
			DWORD dwDay		= dwTime / (60*60*24)	; dwTime -= (60*60*24) * dwDay;
			DWORD dwHour	= dwTime / (60*60)		; dwTime -= (60*60) * dwHour;
			DWORD dwMin		= dwTime / (60)			; dwTime -= 60 * dwMin;

			wsprintf(szRemainTimeMsg, g_Message[ETC_MESSAGE1192].szMessage, dwDay, dwHour, dwMin, dwTime); // "%u일 %u시간 %u분 %u초"
		}
		break;

	case china_billing_time_type_timestamp:
		{
			// 기간제라 하더라도 남은 시간 출력.
			if(bRemainedTime)
			{
				time_t CurrentDate; // 현재일.
				time(&CurrentDate);

				DWORD dwTime = GetChinaBillingRemainedTime();

				DWORD dwDay		= dwTime / (60*60*24)	; dwTime -= (60*60*24) * dwDay;
				DWORD dwHour	= dwTime / (60*60)		; dwTime -= (60*60) * dwHour;
				DWORD dwMin		= dwTime / (60)			; dwTime -= 60 * dwMin;

				wsprintf(szRemainTimeMsg, g_Message[ETC_MESSAGE1192].szMessage, dwDay, dwHour, dwMin, dwTime); // "%u일 %u시간 %u분 %u초"
			}
			// 결제 기간이 언제까지인 지 출력.
			else
			{
				tm* pExpireDate = NULL;
				char* szAMPM = g_Message[ETC_MESSAGE1193].szMessage; // "오전";
				
				pExpireDate = localtime(&g_ChinaBillingRemainedTime.dwExpireDate);
				if(pExpireDate->tm_hour > 12)
				{
					pExpireDate->tm_hour -= 12;
					lstrcpy(szAMPM, g_Message[ETC_MESSAGE1194].szMessage); // "오후"
				}

				if(pExpireDate->tm_hour == 0)
					pExpireDate->tm_hour = 12;

				wsprintf(szRemainTimeMsg, g_Message[ETC_MESSAGE1195].szMessage, // "%u년 %u월 %u일 %s %u시 %u분 %u초"
					pExpireDate->tm_year, 
					pExpireDate->tm_mon, 
					pExpireDate->tm_mday,
					szAMPM,
					pExpireDate->tm_hour,
					pExpireDate->tm_min,
					pExpireDate->tm_sec);
			}			
		}
		break;
	}

	return szRemainTimeMsg;
}


void CmdLoginChinaBillRemainedTime( char* pMsg, DWORD dwLen )
{
	WSTC_CHINA_BILL_REMAIN_TIME* pPacket = (WSTC_CHINA_BILL_REMAIN_TIME*)pMsg;

	// 전역 변수에 남은 시간을 기록한다.... :)
	SetChinaBillingRemainedTime(pPacket->GetTimeType(), pPacket->GetTime());

	// 타이머 가동 시작!
	StartChinaBillingInformTimer();
}

void CmdWorldChinaBillKickCode(char* pMsg, DWORD dwLen)
{
	WSTC_CHINA_BILL_KICK_CODE* pPacket = (WSTC_CHINA_BILL_KICK_CODE*)pMsg;

	DWORD dwMsgID = 0;

	switch(pPacket->btKickCode)
	{
	case china_billing_kick_expired_personal_time	:	dwMsgID = ETC_MESSAGE1201;	break;
	case china_billing_kick_expired_pcroom_time		:	dwMsgID = ETC_MESSAGE1202;	break;
	case china_billing_kick_expired_personal_period	:	dwMsgID = ETC_MESSAGE1203;	break;
	case china_billing_kick_expired_pcroom_period	:	dwMsgID = ETC_MESSAGE1204;	break;
	case china_billing_kick_dupplicated_login		:	dwMsgID = ETC_MESSAGE1205;	break;
	case china_billing_kick_etc						:	dwMsgID = ETC_MESSAGE1206;	break;
	}

	// Kick에 대한 메시지 박스 띄운다. 
	CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();	
	
	pErrorWnd->SetError(g_Message[dwMsgID].szMessage, g_Message[ETC_MESSAGE594].szMessage , "", 2);				
	pErrorWnd->SetActive(TRUE);
	pErrorWnd->m_byType		= 1;
	pErrorWnd->m_byTime		= 5;
	pErrorWnd->m_dwCurTime	= timeGetTime();
	pErrorWnd->m_dwPevTime	= timeGetTime();

	CInterface::GetInstance()->SetWnd(ERROR_WND);
}

void ShowChinaBillingRemainTime()
{	
	if(!g_ChinaBillingRemainedTime.IsTimeSet()) return;

	// 화면에 메시지 뿌리기~~~ (월드에서 해야 한다!!)
	LPCSTR szRemainTimeMsg = GetChinaBillingRemainedTime(FALSE);

	if(NULL == szRemainTimeMsg) return;

	CWorldWnd* pWorldWnd = CWorldWnd::GetInstance();

	memset(pWorldWnd->szMessageLine, 0, sizeof(pWorldWnd->szMessageLine));

	switch(g_ChinaBillingRemainedTime.dwTimeType) 
	{
	case china_billing_time_type_none		:
		lstrcpy(pWorldWnd->szMessageLine[0], GetChinaBillingRemainedTime(TRUE));
		break;

	case china_billing_time_type_second		:
		lstrcpy(pWorldWnd->szMessageLine[0], g_Message[ETC_MESSAGE1196].szMessage); // "귀하의 게임 가능한 시간은"
		lstrcpy(pWorldWnd->szMessageLine[1], GetChinaBillingRemainedTime(TRUE));
		lstrcpy(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE1197].szMessage); // "남았습니다."
		break;

	case china_billing_time_type_timestamp	:
		lstrcpy(pWorldWnd->szMessageLine[0], g_Message[ETC_MESSAGE1198].szMessage); // "귀하의 결제 종료일은"
		lstrcpy(pWorldWnd->szMessageLine[1], GetChinaBillingRemainedTime(FALSE));
		lstrcpy(pWorldWnd->szMessageLine[2], g_Message[ETC_MESSAGE1199].szMessage); // "입니다."
		break;
	}

	pWorldWnd->m_bInfoType = 0;
	pWorldWnd->SetActive();	
}

LPCSTR GetChinaBillingRemainedTimeString()
{
	static char szMsg[0xff];

	if(!g_ChinaBillingRemainedTime.IsTimeSet())
	{
		lstrcpy(szMsg, "No Billing Info.");
	}
	else
	{
		if(g_ChinaBillingRemainedTime.dwTimeType == china_billing_time_type_none) 
		{
			lstrcpy(szMsg, GetChinaBillingRemainedTime(TRUE));
		}
		else
		{
			// "귀하의 게임 가능한 시간은 %s 남았습니다."
			wsprintf(szMsg, g_Message[ETC_MESSAGE1200].szMessage, GetChinaBillingRemainedTime(TRUE));
		}
	}

	return szMsg;
}

void OnChinaBillingLoginFail(BYTE btResult)
{
	CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();

	g_pGVLogin->bIsLoging = FALSE;

	btResult &= 0x0f;

	switch(btResult)
	{
	case china_billing_login_success		: // 이 코드는 성공이 아니다!!! 다른 용도로 돌려쓴다~ :)
		// "빌링 서버에 접속하지 못했습니다."
		// "잠시 후 다시 시도해 주세요."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1183].szMessage, 
			g_Message[ETC_MESSAGE339].szMessage, "", 402, 459, 2, TRUE);									
		break;

	case china_billing_login_no_user		:
		// "귀하의 빌링 정보를 찾지 못했습니다.";
		// 운영팀에 문의하세요."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1184].szMessage, 
			g_Message[ETC_MESSAGE483].szMessage, "", 402, 459, 2, TRUE);	
		break;

	case china_billing_login_over_capacity	:
		// "빌링 서버의 최대 처리량을 넘었습니다.";
		// "잠시 후 다시 시도해 주세요."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1185].szMessage, 
			g_Message[ETC_MESSAGE339].szMessage, "", 402, 459, 2, TRUE);	
		break;

	case china_billing_login_timeout		:
		// "빌링 서버에서 타임아웃 되었습니다.";
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1186].szMessage, "", "", 402, 459, 1, TRUE);	
		break;

	case china_billing_login_canceled		:
		// "빌링 서버에 의해 취소되었습니다.";
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1187].szMessage, "", "", 402, 459, 1, TRUE);	
		break;

	case china_billing_login_etc			:
		// "기타 이유로 빌링 서버에 의해 거부되었습니다.";
		// "운영팀에 문의하세요."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1188].szMessage, 
			g_Message[ETC_MESSAGE483].szMessage, "", 402, 459, 2, TRUE);	
		break;

	case china_billing_login_time_expired	:
		// "귀하의 결제가 만료되었습니다.";
		// "다시 결제해 주시기 바랍니다."
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, 
			g_Message[ETC_MESSAGE1189].szMessage, 
			g_Message[ETC_MESSAGE1190].szMessage, "", 402, 459, 2, TRUE);	
		break;
	}

	pGameDefaultWnd->SetActive();
	LoginInterface(FALSE);

	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_ID);
	g_pInputManager->ClearInput(INPUT_BUFFER_LOGIN_PASSWORD);
	g_pInputManager->SetFocusInput(INPUT_BUFFER_LOGIN_ID);
}

void StartChinaBillingInformTimer()
{
	SetTimer(g_hMainWnd, IDT_CHINA_BILLING_INFORM, CHINA_BILLING_INFORM_TIMER_PERIOD, OnChinaBillRemainTimeInform);
}

void StopChinaBillingInformTimer()
{
	KillTimer(g_hMainWnd, IDT_CHINA_BILLING_INFORM);
}

void CALLBACK OnChinaBillRemainTimeInform(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	static bool bOneHour = false;
	static bool bFiveMin = false;

	// 현재 남은 시간을 가져온다.
	DWORD dwRemainTicks = GetChinaBillingRemainedTime();

	// 일단 분단위로 계산한다.
	DWORD dwRemainMin = dwRemainTicks / (1000*60);

	// 60분이 남는지.
	if(!bOneHour && dwRemainMin == 60)
	{
		bOneHour = true;

		ShowChinaBillingRemainTimeInform(60);
	}

	// 5분이 남는지..
	else if(!bFiveMin && dwRemainMin == 5)
	{
		bFiveMin = true;

		// 더이상 필요없으니 타이머도 죽이자!
		StopChinaBillingInformTimer();

		ShowChinaBillingRemainTimeInform(5);
	}	
}

void ShowChinaBillingRemainTimeInform(DWORD dwMin)
{
	CErrorWnd* pErrorWnd = CErrorWnd::GetInstance();

	if(dwMin == 60)
	{
		pErrorWnd->SetError(g_Message[ETC_MESSAGE1207].szMessage, "" , "", 1);
	}
	else if(dwMin == 5)
	{
		pErrorWnd->SetError(g_Message[ETC_MESSAGE1208].szMessage, "", "", 1);
	}
	else 
	{
		return;
	}

	pErrorWnd->SetActive(TRUE);
	pErrorWnd->m_byType		= 77;
	
//	pErrorWnd->m_byTime		= 5;
//	pErrorWnd->m_dwCurTime	= timeGetTime();
//	pErrorWnd->m_dwPevTime	= timeGetTime();

	CInterface::GetInstance()->SetWnd(ERROR_WND);
}
