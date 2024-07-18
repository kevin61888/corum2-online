#ifndef __ILLEGAL_PACKET_SIZE_LOG_H__
#define __ILLEGAL_PACKET_SIZE_LOG_H__


#pragma once


#include "CommonServer.h"
#include "Log.h"


inline void DECLSPECIFIER IllegalSizePacketProc(BYTE byStatus, BYTE byHeader, DWORD dwLen)
{
	Log(LOG_IMPORTANT, "Illegal Size Packet(%u/%u) Received..(%u) Bytes", byStatus, byHeader, dwLen);
}

inline void DECLSPECIFIER IllegalSizePacketProc(BYTE byHeader, DWORD dwLen)
{
	Log(LOG_IMPORTANT, "Illegal Size Packet(%u) Received..(%u) Bytes", byHeader, dwLen);
}

inline void DECLSPECIFIER IllegalSizePacketProc(WORD wdHeader, DWORD dwLen)
{
	Log(LOG_IMPORTANT, "Illegal Size Packet(%u) Received..(%u) Bytes", wdHeader, dwLen);
}

#endif 