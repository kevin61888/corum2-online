#ifndef __LICENSING_PROTOCOL_H__
#define __LICENSING_PROTOCOL_H__


#pragma once


enum Proto_Licensing
{
	Proto_Licensing_Heartbeat			= 0, // LoginAgent에서 1분간 보내는 패킷이 없는 경우, 송신. 라이센싱 서버는 재송신.
	Proto_Licensing_Authenticaion		= 1, // 인증받은 로그인서버인지 확인 요청.
	Proto_Licensing_AuthenticaionResult = 2, // 인증 결과를 응답.
	Proto_Licensing_CurrentUser			= 3, // 동접 숫자를 알림.
	Proto_Licensing_BillingInformation	= 4, // 빌링 관련 정보 송신.
	Proto_Licensing_Command				= 5, // 라이센싱 서버의 명령 전송.
};


enum Licensing_Command
{
	Command_Send_Heartbeat				= 0, // Heartbeat를 날리라고 명령.
	Command_Send_CurrentUser			= 1, // 현재 유저 정보를 날리라고 명령.
	Command_Send_BillingInformation		= 2, // 빌링 정보를 날리라고 명령.
	Command_Suicide						= 3, // 죽으라고 명령.
	Command_DropAllUsers				= 4, // 모든 유저의 연결을 끊어 버리라고 명령.	
};


struct Proto_Licensing_Packet_Heartbeat
{
	BYTE	byPacketType;

	Proto_Licensing_Packet_Heartbeat()				{ byPacketType = Proto_Licensing_Heartbeat;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_Heartbeat);	}
};

struct Proto_Licensing_Packet_Authenticaion
{
	BYTE	byPacketType;

	Proto_Licensing_Packet_Authenticaion()			{ byPacketType = Proto_Licensing_Authenticaion;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_Authenticaion);	}
};

struct Proto_Licensing_Packet_AuthenticaionResult
{
	BYTE	byPacketType;
	BYTE	byResult;	// 1 if succeeded!

	Proto_Licensing_Packet_AuthenticaionResult()	{ byPacketType = Proto_Licensing_AuthenticaionResult; byResult = 0;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_AuthenticaionResult);	}
};

struct Proto_Licensing_Packet_CurrentUser
{
	BYTE	byPacketType;
	DWORD	dwCurrentUserCount;

	Proto_Licensing_Packet_CurrentUser()			{ byPacketType = Proto_Licensing_CurrentUser; dwCurrentUserCount = 0;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_CurrentUser);	}
};

struct Proto_Licensing_Packet_BillingInformation
{
	BYTE	byPacketType;
	BYTE	BillingInformation[2048];

	Proto_Licensing_Packet_BillingInformation()		{ byPacketType = Proto_Licensing_BillingInformation; memset(&BillingInformation, 0, sizeof(BillingInformation));	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_BillingInformation);	}
};

struct Proto_Licensing_Packet_Command
{
	BYTE	byPacketType;
	BYTE	byCommand;

	Proto_Licensing_Packet_Command()				{ byPacketType = Proto_Licensing_Command; byCommand = 0;	}
	WORD	GetPacketSize()							{ return sizeof(Proto_Licensing_Packet_Command);	}
};


#endif 