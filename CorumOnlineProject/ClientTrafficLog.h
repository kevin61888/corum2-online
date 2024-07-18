#ifndef __CLIENT_TRAFFIC_LOG_H__
#define __CLIENT_TRAFFIC_LOG_H__

#pragma once

#include "Develop.h"

#ifdef __TRAFFIC_LOG

void SendMsgLog(char* msg, DWORD length);
void RecvMsgLog(char* msg, DWORD length);

#endif // __TRAFFIC_LOG


#endif // __CLIENT_TRAFFIC_LOG_H__