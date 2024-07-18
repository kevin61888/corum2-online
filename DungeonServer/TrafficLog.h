#ifndef __TRAFFIC_LOG_H__
#define __TRAFFIC_LOG_H__

#include "StdAfx.h"


BOOL SendToUserWithLog(DWORD dwConnectionIndex, char* msg, DWORD length, DWORD flag);


#ifndef TRAFFIC_LOG
#define NetSendToUser(index, msg, len, flag) g_pNet->SendToUser(index, msg, len, flag)
#else
#define NetSendToUser(index, msg, len, flag) SendToUserWithLog(index, msg, len, flag)
#endif

#endif // __TRAFFIC_LOG_H__