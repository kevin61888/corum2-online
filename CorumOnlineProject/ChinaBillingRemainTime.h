#ifndef __CHINA_BILLING_REMAINED_TIME_H__
#define __CHINA_BILLING_REMAINED_TIME_H__

#pragma once

// billing time type code from billing server
enum CHINA_BILLING_TIME_TYPE
{
	china_billing_time_type_none				= 0x00,
	china_billing_time_type_second				= 0x01,
	china_billing_time_type_timestamp			= 0x02,
};

// login result code from billing server
enum CHINA_BILLING_LOGIN_CODE
{
	china_billing_login_success			= 0x00,
	china_billing_login_unexpected1		= 0x01,
	china_billing_login_unexpected2		= 0x02,

	china_billing_login_no_user			= 0x03,
	china_billing_login_over_capacity	= 0x04,
	china_billing_login_timeout			= 0x05,
	china_billing_login_canceled		= 0x06,
	china_billing_login_etc				= 0x07,

	china_billing_login_time_expired	= 0x0f,
};

// kick out code from billing server
enum CHINA_BILLING_KICK_CODE
{
	china_billing_kick_expired_personal_time	= 0x01,
	china_billing_kick_expired_pcroom_time		= 0x02,
	china_billing_kick_expired_personal_period	= 0x03,
	china_billing_kick_expired_pcroom_period	= 0x04,
	china_billing_kick_dupplicated_login		= 0x05,
	china_billing_kick_etc						= 0x06,
};

// 타이머 ID
#define IDT_CHINA_BILLING_INFORM			1218
#define CHINA_BILLING_INFORM_TIMER_PERIOD	60*1000 // 1 MIN
	

struct CHINA_REMAINED_TIME
{
	BOOL	bReceived;
	DWORD	dwTimeType;
	DWORD	dwRegisteredTime;	// 최초 남은 시간이 기록된 시간.
	DWORD	dwRemainedTime;	
	time_t	dwExpireDate;

	CHINA_REMAINED_TIME()
	{
		ZeroMemory(this, sizeof(CHINA_REMAINED_TIME));
	}
	
	BOOL	IsTimeSet()	{ return bReceived;	}
};



void	CmdLoginChinaBillRemainedTime( char* pMsg, DWORD dwLen );	// 결제 정보 알려주는 메시지 처리.
void	CmdWorldChinaBillKickCode(char* pMsg, DWORD dwLen);			// 월드용 Kick 사유 통지 처리.

void	OnChinaBillingLoginFail(BYTE btResult);						// 중국 빌링과 관련된 로그인 실패 시 처리.

DWORD	GetChinaBillingRemainedTime();								// 남은 사용 가능 시간을 ms 단위로 제공.
void	ShowChinaBillingRemainTime();								// 월드 처음 조인 시, 결제 정보 메시지 박스 띄워줌.
LPCSTR	GetChinaBillingRemainedTimeString();						// 잔여 플레이 가능 시간을 초단위로 알려줌.
void	SetChinaBillingRemainedTime(DWORD dwType, DWORD dwTime);	// 남은 시간을 전역 구조체에 저장!

// 남은 시간(1시간/5분) 알려주기 위한 타이머.
void	StartChinaBillingInformTimer();
void	StopChinaBillingInformTimer();
void	CALLBACK OnChinaBillRemainTimeInform(HWND hWnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime); // timer function!
void	ShowChinaBillingRemainTimeInform(DWORD dwMin);




#endif // __CHINA_BILLING_REMAINED_TIME_H__