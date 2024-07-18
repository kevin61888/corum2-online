#pragma once


#include "oledb.h"
#include "ProtocolDefinition.h"	


#pragma pack(push,1)		


//-----------------------------------------------------------------------------
// 월드 서버에게 서버들 생존 정보를 내놓으라 요청한다.
//-----------------------------------------------------------------------------
struct ASTWS_REQUEST_SERVER_ALIVE
{
	WORD	wHeader;
	WORD	wInfoType;	// 정보 타입. 현재는 사용되지 않음.	

	ASTWS_REQUEST_SERVER_ALIVE()	{ wHeader = Protocol_Agent_Server::ACMD_REQUEST_SERVER_ALIVE; }
	DWORD	GetPacketSize()			{ return sizeof(ASTWS_REQUEST_SERVER_ALIVE); }	
};


struct WSTAS_REQUEST_CHINA_BILLING
{
	WORD	wHeader;
	
	DWORD	dwUserIndex;
	DWORD	dwPropID;

	WSTAS_REQUEST_CHINA_BILLING()	{ wHeader = Protocol_Agent_Server::ACMD_REQUEST_CHINA_BILLING; }
	DWORD	GetPacketSize()			{ return sizeof(WSTAS_REQUEST_CHINA_BILLING); }	
};


struct WSTAS_REQUEST_CHINA_BILLINGINFO
{
	WORD	wHeader;
	
	DWORD	dwUserIndex;
	DWORD	dwPropID;

	WSTAS_REQUEST_CHINA_BILLINGINFO()	{ wHeader = Protocol_Agent_Server::ACMD_REQUEST_CHINA_BILLINGINFO; }
	DWORD	GetPacketSize()				{ return sizeof(WSTAS_REQUEST_CHINA_BILLINGINFO); }	
};


//-----------------------------------------------------------------------------
// 월드 서버에게 공지 메세지를 보내라고 한다.
//-----------------------------------------------------------------------------
struct ASTWS_CMTOOL_NOTICE
{
	WORD	bHeader;	
	WORD	wMsgLen;
	char	szMsg[1024];
	
	ASTWS_CMTOOL_NOTICE()			{ bHeader = Protocol_Agent_Server::ACMD_CMTOOL_NOTICE; }
	DWORD	GetPacketSize()			{ return 4 + wMsgLen; }
};


//-----------------------------------------------------------------------------
// 길드에 가입되어
//-----------------------------------------------------------------------------
struct ASTWS_GUILD_CHARACTER_DELETE
{
	WORD	wHeader;	
	DWORD	dwGuildId;
	BYTE	byRank;
	DWORD	dwIndex;
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];

	ASTWS_GUILD_CHARACTER_DELETE()	{ wHeader = Protocol_Agent_Server::ACMD_GUILD_CHARACTER_DELETE; }
	DWORD	GetPacketSize()			{ return sizeof(ASTWS_GUILD_CHARACTER_DELETE); }	
};


//-----------------------------------------------------------------------------
// 과금 종료 시간이 10분 미만일때 보내주는 패킷 
//-----------------------------------------------------------------------------
struct ASTWS_BILLING_SMALL_TIME
{
	WORD wHeader;
	BYTE bBillingType;		//현재 과금 타입 
	DWORD dwChar_Index;		//CharacterIndex
	DWORD dwSecond;			//몇 초 남았나...

	ASTWS_BILLING_SMALL_TIME()	{ wHeader = Protocol_Agent_Server::ACMD_BILLING_SMALL_TIME;	}
	DWORD GetPacketSize()		{	return sizeof(ASTWS_BILLING_SMALL_TIME); }
};


//-----------------------------------------------------------------------------
// 과금 종료 시간이 10분 미만일때 보내주는 패킷 
//-----------------------------------------------------------------------------
struct ASTWS_CHANGE_BILLING_TYPE
{
	WORD wHeader;
	BYTE bBillingType;		//바뀐 과금 타입 
	DWORD dwChar_Index;		//CharacterIndex
	DWORD dwRemainTime;		//몇 초 남았나...
	DBTIMESTAMP eDay;		//종료 날짜 

	ASTWS_CHANGE_BILLING_TYPE()	{ wHeader = Protocol_Agent_Server::ACMD_CHANGE_BILLING_TYPE;	}
	DWORD GetPacketSize()		{	return sizeof(ASTWS_CHANGE_BILLING_TYPE); }
};


//-----------------------------------------------------------------------------
// 월드 서버가 로그인 서버에게 서버의 생존 정보를 보내온다.
//-----------------------------------------------------------------------------
struct WSTAS_SERVER_ALIVE_INFO
{
	WORD	wHeader;
	DWORD	dwServerID;
	WORD	wServerType;			// 0 : 월드 서버, 1 : 던젼 서버.
	WORD	wConnectedUserCount;	// 접속되어 있는 유저의 수.
	
	DWORD	dwMagicItemDropRate;	// 마법 아이템 드롭률.
	DWORD	dwExpGainRate;			// 경험치 획득률.

	WSTAS_SERVER_ALIVE_INFO()		{ wHeader = Protocol_Agent_Server::ACMD_SERVER_ALIVE_INFO;	}
	DWORD	GetPacketSize()			{ return sizeof(WSTAS_SERVER_ALIVE_INFO); }
};


enum BILLING_INFO_PROCEDURE_TYPE 
{	
	Billing_Info_Procedure_Type_NONE		= 0,
	Billing_Info_Procedure_Type_Normal		= 1,
	Billing_Info_Procedure_Type_Level_Limit	= 2,
};

//-----------------------------------------------------------------------------
// Packet: ASTWS_CHINA_BILLING_INFO.
// Desc: 중국 상용화 관련(유저 빌링 정보를 월드서버로 보낸다).
//-----------------------------------------------------------------------------
struct ASTWS_CHINA_BILLING_INFO
{
	WORD	wHeader;

	DWORD	dwUserIndex;

	BYTE	byType;
	DWORD	dwPoint;	

	BYTE	byBillingProcedureType;		// 0: not procedure, 1 : normal, 2 : detected level limit
	
	ASTWS_CHINA_BILLING_INFO()	{	wHeader = Protocol_Agent_Server::ACMD_CHINA_BILLING_INFO;	}
	DWORD	GetPacketSize()		{	return sizeof(ASTWS_CHINA_BILLING_INFO);					}

	void	SetProcedureType(enum BILLING_INFO_PROCEDURE_TYPE eProcedureType)
	{
		byBillingProcedureType = (BYTE)eProcedureType;
	}

	BYTE	GetProcedureType() const
	{
		return byBillingProcedureType;
	}
};


//-----------------------------------------------------------------------------
// 월드서버가 Login Agent 서버에 Attach를 시도할때 쓰는 패킷 
//-----------------------------------------------------------------------------
struct WSTAS_SERVER_ATTACH
{
	WORD	wHeader;
	DWORD	dwWorldServerID;			
	
	WSTAS_SERVER_ATTACH()	{	wHeader = Protocol_Agent_Server::ACMD_SERVER_ATTACH;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_SERVER_ATTACH);	}
};


//-----------------------------------------------------------------------------
// 월드서버가 Agent 서버에게 해당유저의 접속이 실패했다고 알림 
//-----------------------------------------------------------------------------
struct WSTAS_USER_ACCEPT_ALLOWED
{
	WORD	wHeader;
	DWORD	dwPropID;			//Agent에서 유저를 뽑아오기위한 Key값 (MEMBER DB에 있는 Chr_Log_Info Index값)

	WSTAS_USER_ACCEPT_ALLOWED()	{	wHeader = Protocol_Agent_Server::ACMD_USER_ACCEPT_ALLOWED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_USER_ACCEPT_ALLOWED);	}
};


//-----------------------------------------------------------------------------
// 월드서버가 Agent 서버에게 해당유저의 접속이 실패했다고 알림 
//-----------------------------------------------------------------------------
struct WSTAS_USER_ACCEPT_FAILED
{
	WORD	wHeader;
	DWORD	dwPropID;		//Agent에서 유저 구분하기 위한 Key 값 
	BYTE	bErrorCode;

	WSTAS_USER_ACCEPT_FAILED()	{	wHeader = Protocol_Agent_Server::ACMD_USER_ACCEPT_FAILED;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_USER_ACCEPT_FAILED);	}
};


//-----------------------------------------------------------------------------
// 월드서버가 Agent 서버에게 해당유저가 World서버 로그인에 성공했다는 메세지를 날림 
//-----------------------------------------------------------------------------
struct WSTAS_USER_WORLD_LOGIN_SUCCESS
{
	WORD	wHeader;
	DWORD	dwPropID;			//User의 MemberDB PropID		
	DWORD	dwUserIndex;

	WSTAS_USER_WORLD_LOGIN_SUCCESS()	{	wHeader = Protocol_Agent_Server::ACMD_USER_WORLD_LOGIN_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_USER_WORLD_LOGIN_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// 월드서버에서 유저가 Disconnect 되었다는 메세지를 Agent 서버에 알려준다. 
//-----------------------------------------------------------------------------
struct WSTAS_WORLD_USER_DISCONNECT
{
	WORD	wHeader;
	DWORD	dwPropID;			//User의 MemberDB PropID
		
	WSTAS_WORLD_USER_DISCONNECT()	{	wHeader = Protocol_Agent_Server::ACMD_WORLD_USER_DISCONNECT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTAS_WORLD_USER_DISCONNECT);	}	
};


//-----------------------------------------------------------------------------
// Agent 서버가 World서버에게 해당 User를 강제 Logout 하라고 날림 
//-----------------------------------------------------------------------------
struct ASTWS_KICK_USER
{
	WORD	wHeader;
	DWORD	dwCharIndex;
	BYTE	btKickCode;

	ASTWS_KICK_USER()		{	wHeader = Protocol_Agent_Server::ACMD_KICK_USER;	btKickCode = 0; 	}
	DWORD	GetPacketSize()	{	return sizeof(ASTWS_KICK_USER);	}
};


//-----------------------------------------------------------------------------
// 월드서버가 Login Agent 서버에 Attach 결과 패킷 
//-----------------------------------------------------------------------------
struct ASTWS_SERVER_ATTACH_RESULT
{
	WORD	wHeader;
	BYTE	bErrorCode;		//0이면 Success, 아니면 Error		
	BYTE	bAdultMode;	
	ASTWS_SERVER_ATTACH_RESULT()	{	wHeader = Protocol_Agent_Server::ACMD_SERVER_ATTACH_RESULT;		}
	DWORD	GetPacketSize()	{	return sizeof(ASTWS_SERVER_ATTACH_RESULT);	}
};


//-----------------------------------------------------------------------------
// Agent서버가 World 서버에 유저를 추가하라는 Packet을 보냄 
//-----------------------------------------------------------------------------
struct BILLING_INFO
{
	BYTE		bPCRoom;				//PC방 접속 여부 
	BYTE		bUserBillType;			//개인 과금 타입 
	BYTE		bPCRoomBillType;		//PC방 과금 타입 	

	DWORD		dwTimePerUserMeterRate;		//개인 종량제 유저용 Time
	DWORD		dwTimeForPCRoomMeteRate;	//PC방 유저용 Time
	DWORD		dwTimeFor1HourFree;			//1시간 무료용 Time

	BYTE		bCurBillingOrderIndex;		//현재 BillingOrder Index (월드서버용)
	BYTE		BillingOrder[MAX_BILLING_ORDER_COUNT];	//결제 순위 
	
	DBTIMESTAMP	RecentLogout;			//최근 로그아웃 시간(계정기준)
	DBTIMESTAMP	UserFlatEndTime;		//개인 종량제 종료날짜 
	DBTIMESTAMP	PCFlatEndTime;			//PC방 종량제 종료날짜 
};


struct ASTWS_ADD_NEW_USER
{
	WORD		wHeader;
	DWORD		dwPropID;			//Agent에서 유저를 뽑아오기위한 Key값 (MEMBER DB에 있는 Chr_Log_Info Index값)
	DWORD		dwSerialCode;		//Agent의 시리얼 코드 
	DWORD		dwChar_Index;		//선택한 Character의 DB상의 Char_Index
	BYTE		bBillingType;		//결제 타입 
	DWORD		dwRemainTime;		//남은 시간 
	DBTIMESTAMP	BillingEday;		//결제 끝나는시각 

	BILLING_INFO	BillingInfo;//결제 정보 

	ASTWS_ADD_NEW_USER()	{	wHeader = Protocol_Agent_Server::ACMD_ADD_NEW_USER;	}
	DWORD	GetPacketSize()	{	return sizeof(ASTWS_ADD_NEW_USER);	}
};


#pragma pack(pop)

