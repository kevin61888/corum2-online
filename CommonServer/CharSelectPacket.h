#pragma once


#include "LoginPacket.h"
#include "ProtocolDefinition.h"	
#include "Item.h"


#pragma pack(push,1)


//-----------------------------------------------------------------------------
// 선택한 캐릭터를 날려준다. 
//-----------------------------------------------------------------------------
struct CTWS_CHARACTER_SELECT : public CHARSELECT_PACKET_HEADER
{
	BYTE	bCharacterIndex;	//4개 배열중 배열 Index
	
	CTWS_CHARACTER_SELECT()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CHARACTER_SELECT;	}
	DWORD	GetPacketSize()	{	return (DWORD)sizeof(CTWS_CHARACTER_SELECT);	}
};


//-----------------------------------------------------------------------------
// 새로운 캐릭터를 생성한다. 
//-----------------------------------------------------------------------------
struct CTWS_CREATE_NEW_CHARACTER : public CHARSELECT_PACKET_HEADER
{
	BYTE	bChrNum;		//DB에서의 1,2,3,4 자리 
	BYTE	bClass;			//클래스
	WORD	wHead;			//몸 번호 
	WORD	wStartMapID;	//처음 시작 맵 번호 
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];

	CTWS_CREATE_NEW_CHARACTER()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CREATE_NEW_CHARACTER;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_CREATE_NEW_CHARACTER);	}
};


//-----------------------------------------------------------------------------
// 캐릭터 생성 결과 성공 패킷 
//-----------------------------------------------------------------------------
struct WSTC_CREATE_CHARACTER_SUCCESS : public CHARSELECT_PACKET_HEADER
{
	DWORD	dwStr;
	DWORD	dwVit;
	DWORD	dwDex;
	DWORD	dwInt;
	DWORD	dwEgo;
	
	WSTC_CREATE_CHARACTER_SUCCESS()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CREATE_CHARACTER_SUCCESS;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CREATE_CHARACTER_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// 캐릭터를 삭제한다. 
//-----------------------------------------------------------------------------
struct CTWS_DELETE_CHARACTER : public CHARSELECT_PACKET_HEADER
{
	BYTE	bChrNum;
	char	szCharacterName[MAX_CHARACTER_NAME_LENGTH];
	char	szJumin[7];

	CTWS_DELETE_CHARACTER()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_DELETE_CHARACTER;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_DELETE_CHARACTER);	}
};


//-----------------------------------------------------------------------------
// 캐릭터 생성 결과 실패 패킷 
//-----------------------------------------------------------------------------
struct WSTC_CREATE_CHARACTER_FAIL : public CHARSELECT_PACKET_HEADER
{
	int		nResultValue;

	WSTC_CREATE_CHARACTER_FAIL()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CREATE_CHARACTER_FAIL;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_CREATE_CHARACTER_FAIL);	}
};


//-----------------------------------------------------------------------------
// 유저가 캐릭터를 선택하면 월드에서 필요한 선택한 캐릭터의 정보를 보내준다. 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_USER_INFO : public CHARSELECT_PACKET_HEADER
{ // 여기에 필드를 추가할경우 가장아래에 추가해야만 한다. 심각한문제가 발생할지도 모른다. 길드 탈퇴 버그 수정 김영대 2005.03.24
	DWORD		dwUserIndex;
	DWORD		dwRecentOutMap;
	DWORD		dwRecentOutPos;
	CItem		Equip[MAX_EQUIP_POOL];
	char		szGuildName[MAX_CHARACTER_NAME_LENGTH];
	char		szNickName[MAX_CHARACTER_NAME_LENGTH];
	BYTE		byType;
	BYTE		byRank;
	DWORD		dwLev;
	WORD		wClass;
	WORD		wHead;	
	DWORD		dwGuildId;
	DWORD		dwMoney;
	BYTE		bySkillKey[16];
	BYTE		byPosKey[16];
	short		wCriminalTime;
	BYTE		byHelpTypeA;
	BYTE		byHelpTypeB;
	BYTE		byHelpInterface;
	BYTE		byCurrentHand;
	BYTE		byMessenger;	
	DBTIMESTAMP	sGuildJoinTime;
	DWORD		dwPrevGuildId;	
	float		fStartPos_X;		//WorldMap에서의 시작위치 
	float		fStartPos_Z;		//WorldMap에서의 시작위치 
	BYTE		dwGuildWarFlag;		// 0 : 기본. 1 : 길드전 중, 2 : 관전
	
	WSTC_WORLD_USER_INFO()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_WORLD_USER_INFO;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_WORLD_USER_INFO);	}
};


//-----------------------------------------------------------------------------
// 캐릭터 선택 실패 
//-----------------------------------------------------------------------------
struct ASTC_CHARACTER_SELECT_FAIL : public CHARSELECT_PACKET_HEADER
{
	BYTE	bErrorCode;

	ASTC_CHARACTER_SELECT_FAIL()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CHARACTER_SELECT_FAIL;	}
	DWORD	GetPacketSize()	{	return (DWORD)sizeof(ASTC_CHARACTER_SELECT_FAIL);	}
};


//-----------------------------------------------------------------------------
// WorldServer로 접속하라  
//-----------------------------------------------------------------------------
struct ASTC_CONNECT_WORLD_SERVER : public CHARSELECT_PACKET_HEADER
{
	DWORD	dwIp;
	WORD	wPort;
	DWORD	dwPropID;
	DWORD	dwChar_Index;
	DWORD	dwSerialCode;
	BYTE	bEventFlag;

	ASTC_CONNECT_WORLD_SERVER()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::ACMD_CONNECT_WORLD_SERVER;	}
	DWORD	GetPacketSize()	{	return (DWORD)sizeof(ASTC_CONNECT_WORLD_SERVER);	}
};


//-----------------------------------------------------------------------------
// World에 로그인 
//-----------------------------------------------------------------------------
struct CTWS_WORLD_LOGIN : public CHARSELECT_PACKET_HEADER
{
	DWORD	dwPropID;
	DWORD	dwChar_Index;
	DWORD	dwSerialCode;
	BYTE	bEventFlag;

	CTWS_WORLD_LOGIN()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_WORLD_LOGIN;		}
	DWORD	GetPacketSize()	{	return sizeof(CTWS_WORLD_LOGIN);	}
};


//-----------------------------------------------------------------------------
// World에 로그인 실패 
//-----------------------------------------------------------------------------
struct WSTC_WORLD_LOGIN_FAIL : public CHARSELECT_PACKET_HEADER
{
	BYTE	bErrorCode;

	WSTC_WORLD_LOGIN_FAIL()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_WORLD_LOGIN_FAIL;		}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_WORLD_LOGIN_FAIL);	}
};


//-----------------------------------------------------------------------------
// 캐릭터 삭제 결과 패킷 
//-----------------------------------------------------------------------------
struct WSTC_DELETE_CHARACTER_RESULT : public CHARSELECT_PACKET_HEADER
{
	BYTE	bSuccess;		// 성공이면 TRUE, 실패면 FALSE
	int		nResultValue;
	
	WSTC_DELETE_CHARACTER_RESULT()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_DELETE_CHARACTER_RESULT;	}
	DWORD	GetPacketSize()	{	return sizeof(WSTC_DELETE_CHARACTER_RESULT);	}
};


//-----------------------------------------------------------------------------
// Item몰 처리할때의 성공, 실패 정보 패킷 
//-----------------------------------------------------------------------------
struct ASTC_ITEMMALL_INFO : public CHARSELECT_PACKET_HEADER
{
	BYTE	bSuccess;
	BYTE	bItemCount;
	BYTE	bItemAllCount;	
	BYTE	byItemMallType;

	ASTC_ITEMMALL_INFO()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::ACMD_ITEMMALL_INFO;	}
	DWORD	GetPacketSize()	{	return (DWORD)sizeof(ASTC_ITEMMALL_INFO);	}
};


//-----------------------------------------------------------------------------
// Client에게 경고중이라는 
//-----------------------------------------------------------------------------
struct WSTC_WARNING_TITLE : public CHARSELECT_PACKET_HEADER
{
	BYTE	bLen;
	char	szReason[0xff];	

	WSTC_WARNING_TITLE()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_WARNING_TITLE; memset(szReason, 0, 0xff);}
	DWORD	GetPacketSize()	{	return 3 + bLen;	}
};


//-----------------------------------------------------------------------------
// 캐릭터 이전 관련.
//-----------------------------------------------------------------------------
struct ASTC_CHR_MOVECHK : public CHARSELECT_PACKET_HEADER
{
	BYTE	byCnt;
	BYTE	byType;
		
	ASTC_CHR_MOVECHK()				{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CHR_MOVE_CHK;		}
	DWORD	GetPacketSize()			{ return sizeof(ASTC_CHR_MOVECHK); }
};


//-----------------------------------------------------------------------------
// 캐릭터 이전 관련.
//-----------------------------------------------------------------------------
struct CTAS_CHR_MOVECHK : public CHARSELECT_PACKET_HEADER
{
	BYTE	byType;
		
	CTAS_CHR_MOVECHK()				{ bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_CHR_MOVE_CHKRT;	}
	DWORD	GetPacketSize()			{ return sizeof(CTAS_CHR_MOVECHK); }
};


//-----------------------------------------------------------------------------
// 캐릭터 이동 실패 패킷 
//-----------------------------------------------------------------------------
struct ASTC_MOVE_CHARACTER_FAIL : public CHARSELECT_PACKET_HEADER
{
	BYTE	byType;
	BYTE	byNum;
	char	szName[MAX_CHARACTER_NAME_LENGTH];

	ASTC_MOVE_CHARACTER_FAIL()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_MOVE_CHARACTER_FAIL;	}
	DWORD	GetPacketSize()		{	return sizeof(ASTC_MOVE_CHARACTER_FAIL);	}
};


//-----------------------------------------------------------------------------
// 캐릭터 이동 실패 패킷 
//-----------------------------------------------------------------------------
struct ASTC_MOVE_CHARACTER_SUCCESS : public CHARSELECT_PACKET_HEADER
{
	BYTE				byCharNum;
	BYTE				byNum;
	RS_CharacterLogInfo	Character;

	ASTC_MOVE_CHARACTER_SUCCESS()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_MOVE_CHARACTER_SUCCESS;	}
	DWORD	GetPacketSize()			{	return sizeof(ASTC_MOVE_CHARACTER_SUCCESS);	}
};


//-----------------------------------------------------------------------------
// 캐릭터 재전송.
//-----------------------------------------------------------------------------
struct CTAS_MOVE_CHARACTER_RESEND : public CHARSELECT_PACKET_HEADER
{
	BYTE	byType[4];
	BYTE	byNum[4];
	char	szName[4][MAX_CHARACTER_NAME_LENGTH];

	CTAS_MOVE_CHARACTER_RESEND()	{	bGameStatus = UPDATE_GAME_CHAR_SELECT;	bHeader = Protocol_CharSelect::CMD_MOVE_CHARACTER_RESEND;	}
	DWORD	GetPacketSize()			{	return sizeof(CTAS_MOVE_CHARACTER_RESEND);	}
};


#pragma pack(pop)