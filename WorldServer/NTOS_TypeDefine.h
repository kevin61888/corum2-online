#ifndef __NETWORK_TIME_OBSERVER_SERVER_TYPEDEFINE_H__
#define __NETWORK_TIME_OBSERVER_SERVER_TYPEDEFINE_H__


#pragma once


#include "stdafx.h"


#define NTOS_INIT		0x1
#define NTOS_RUN		0x2


struct NTOS_INFO
{
	BOOL		bStatus;
	DWORD		dwSync;
	DWORD		dwSyncPrev;
	DWORD		dwTime;
};

#endif // __NETWORK_TIME_OBSERVER_SERVER_TYPEDEFINE_H__