#ifndef __PARTY_H__
#define __PARTY_H__

#pragma once

#include "WorldUser.h"
#include "PartyHash.h"

extern DWORD g_dwPartyId;

void CmdPartyUserRequestFail(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdPartyUserRequest(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdPartyInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdPartySet(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdPartyLogOutUser(CWorldUser* pUser ,char* pMsg,DWORD dwLength);
void CmdPartyLogoutCl(CWorldUser* pUser ,char* pMsg,DWORD dwLength);
void CmdPartyApprove(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdPartyJoin(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdPartyEntry(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdPartyInfoRequest(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdPartyBoardDelete(CWorldUser* pUser, char* pMsg, DWORD dwLength);
void CmdPartyUserWorldInfo(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdPartyConfig(CWorldUser* pUser, char* pMsg,DWORD dwLength);
void CmdPartyFriend(CWorldUser* pUser, char* pMsg,DWORD dwLength);

void AddPartyFriend(DWORD dwUserIndex, DWORD dwFriendIndex);

#endif // __PARTY_H__