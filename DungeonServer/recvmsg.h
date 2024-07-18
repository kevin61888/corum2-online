#pragma once


#include "..\\4DyuchiGXGFunc\\type.h"


struct	DBRECEIVEDATA;
class	CUser;


void __stdcall FailToConnectWorldServer(void* pExt);
void __stdcall SuccessToConnectWorldServer(DWORD dwConnectionIndex, void* pExt);
void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);
void __stdcall OnRecvFromServerTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall OnRecvFromUserTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);

extern void ReceivedFromDB(DBRECEIVEDATA* pResult);
extern void ReceivedFromDBLog(DBRECEIVEDATA* pResult);

extern void __DisconnectUserInTeamMatch(CUser* pUser);
extern void __DisconnectUserInMatch(CUser* pUser);
extern void __DisconnectUserInEventDungeon(CUser* pUser);
extern void __DisconnectUserDisAppear(CUser* pUser);
extern void __DisconnectUserItemReturnToInv(CUser* pUser);		
extern void __DisconnectUserUpdate(CUser* pUser, DWORD dwConnectionIndex);





