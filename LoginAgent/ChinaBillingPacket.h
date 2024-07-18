#ifndef __CHINA_BILLING_PACKET_H__
#define __CHINA_BILLING_PACKET_H__

#pragma once

#include "GlobalDefine.h"
#include "WinSock.h"


// 중국	빌링 서버와의 패킷 프로토콜을 정의한다.

#define MAX_BILLING_PACKET_TYPE			10

enum CHINA_BILLING_USER_DATA
{
	china_billing_info_none				= 0,	
	china_billing_info_approval			= 1,
	china_billing_info_limit			= 2,
};

struct SCHINA_BILLING_USER_DATA
{
	CHINA_BILLING_USER_DATA eChinaBillingUserData;
	DWORD					dwChinaBillingPoint;
};

// msg type
enum CHINA_BILLING_PACKET_CODE
{
	china_billing_packet_login			= 0x01,
	china_billing_packet_login_result	= 0x02,
	china_billing_packet_logout			= 0x03,
	china_billing_packet_kickout		= 0x04,
	china_billing_packet_server_clear	= 0x05,

	china_billing_packet_dummy			= 0x06,
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

// billing time type code from billing server
enum CHINA_BILLING_TIME_TYPE
{
	china_billing_time_type_none				= 0x00,
	china_billing_time_type_second				= 0x01,
	china_billing_time_type_timestamp			= 0x02,
};


#pragma pack(push, 1)
struct Proto_China_Billing_Login
{
	BYTE	byPacketType;
	BYTE	btGameWorld;
	DWORD	dwUserIndex;
	char	szUserID[MAX_ID_LENGTH];
	DWORD	dwIP;

	Proto_China_Billing_Login()
	{
		ZeroMemory(this, GetPacketSize());
		byPacketType = china_billing_packet_login;
	}

	WORD	GetPacketSize()				{ return sizeof(Proto_China_Billing_Login);		}
	void	SetServerSetID(BYTE btSet)	{ btGameWorld = btSet;							}
	void	SetUserIndex(DWORD dwIndex)	{ dwUserIndex = htonl(dwIndex);					}
	void	SetIP(DWORD dwUserIP)		{ dwIP = htonl(dwUserIP);						}
	void	SetUserID(LPCSTR szID)		{ lstrcpyn(szUserID, szID, MAX_ID_LENGTH);		}
};

struct Proto_China_Billing_Login_Result
{
	BYTE	byPacketType;
	BYTE	btGameWorld;
	DWORD	dwUserIndex;
	char	szUserID[MAX_ID_LENGTH];
	BYTE	btResult;
	BYTE	btTime_Type;
	DWORD	dwTime;

	Proto_China_Billing_Login_Result()
	{
		ZeroMemory(this, GetPacketSize());
		byPacketType = china_billing_packet_login_result;
	}

	WORD	GetPacketSize()		{ return sizeof(Proto_China_Billing_Login_Result);		}
	BYTE	GetServerSetID()	{ return btGameWorld;									}
	DWORD	GetUserIndex()		{ return ntohl(dwUserIndex);							}
	LPCSTR	GetUserName()		{ return szUserID;										}
	BYTE	GetLoginResult()	{ return btResult;										}
	BYTE	GetTimeType()		{ return btTime_Type;									}
	DWORD	GetTime()			{ return ntohl(dwTime);									}
};

struct Proto_China_Billing_Logout
{
	BYTE	byPacketType;
	BYTE	btGameWorld;
	DWORD	dwUserIndex;
	char	szUserID[MAX_ID_LENGTH];
	DWORD	dwIP;

	Proto_China_Billing_Logout()
	{
		ZeroMemory(this, GetPacketSize());
		byPacketType = china_billing_packet_logout;
	}

	WORD	GetPacketSize()				{ return sizeof(Proto_China_Billing_Logout);	}
	void	SetServerSetID(BYTE btSet)	{ btGameWorld = btSet;							}
	void	SetUserIndex(DWORD dwIndex)	{ dwUserIndex = htonl(dwIndex);					}
	void	SetIP(DWORD dwUserIP)		{ dwIP = htonl(dwUserIP);						}
	void	SetUserID(LPCSTR szID)		{ lstrcpyn(szUserID, szID, MAX_ID_LENGTH);		}
};

struct Proto_China_Billing_Kickout
{
	BYTE	byPacketType;
	BYTE	btGameWorld;
	DWORD	dwUserIndex;
	char	szUserID[MAX_ID_LENGTH];
	BYTE	btKickCode;

	Proto_China_Billing_Kickout()
	{
		ZeroMemory(this, GetPacketSize());
		byPacketType = china_billing_packet_kickout;
	}

	WORD	GetPacketSize()				{ return sizeof(Proto_China_Billing_Kickout);	}
	BYTE	GetServerSetID()			{ return btGameWorld;							}
	DWORD	GetUserIndex()				{ return ntohl(dwUserIndex);					}
	LPCSTR	GetUserName()				{ return szUserID;								}
	BYTE	GetKickCode()				{ return btKickCode;							}
};

struct Proto_China_Billing_Server_Clear
{
	BYTE	byPacketType;
	BYTE	btGameWorld;

	Proto_China_Billing_Server_Clear()
	{
		ZeroMemory(this, GetPacketSize());
		byPacketType = china_billing_packet_server_clear;
	}

	WORD	GetPacketSize()				{ return sizeof(Proto_China_Billing_Server_Clear); }
	void	SetServerSetID(BYTE btSet)	{ btGameWorld = btSet;							}
};

struct Proto_China_Billing_Dummy
{
	BYTE	byPacketType;

	Proto_China_Billing_Dummy()
	{
		byPacketType = china_billing_packet_dummy;	
	}

	WORD GetPacketSize()				{ return sizeof(Proto_China_Billing_Dummy);		}
};

#pragma pack(pop)



#endif // __CHINA_BILLING_PACKET_H__


