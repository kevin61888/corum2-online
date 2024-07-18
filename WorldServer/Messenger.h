#ifndef __MESSENGER_H__
#define __MESSENGER_H__

#pragma once

#include "WorldUser.h"
#include "MessengerHash.h"

//======================================================//
// Code by Jang.										//
//======================================================//

void CmdMessengerUserAdd(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdMessengerAnswer(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdMessengerDelete(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdMessengerInfo(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdMessengerStatus(CWorldUser* pUser, char* pMsg, DWORD dwLength);


#endif // __MESSENGER_H__