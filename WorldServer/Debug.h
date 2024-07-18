#ifndef __DEBUG_H__
#define __DEBUG_H__


#pragma once

#include "GlobalDefine.h"

#define MODE_FOR_DEBUG	100

struct Protocol_Debug
{
	enum 
	{
		CMD_LOGIN					= 0
	};
};


#pragma pack(push,1)

//-----------------------------------------------------------------------------
// Packet: DEBUG_LOGIN
// Desc: 개발자용 LoginPacket
//-----------------------------------------------------------------------------
struct DEBUG_LOGIN
{
	BYTE	bGameStatus;
	BYTE	bHeader;
	char	szID[ MAX_ID_LENGTH ];
	char	szPassword[ MAX_PASSWORD_LENGTH ];


	DEBUG_LOGIN()	{	bGameStatus = MODE_FOR_DEBUG;	bHeader = Protocol_Debug::CMD_LOGIN;	}
	DWORD	GetPacketSize()	{	return sizeof(DEBUG_LOGIN);	}
};

#pragma pack(pop)


#endif // __DEBUG_H__
