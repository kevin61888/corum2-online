#ifndef __RECV_MSG_H__
#define __RECV_MSG_H__

#pragma once


struct DBRECEIVEDATA;
class CUser;


void __stdcall OnAcceptUser(DWORD dwConnectionIndex);
void __stdcall OnAcceptServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectServer(DWORD dwConnectionIndex);
void __stdcall OnDisconnectUser(DWORD dwConnectionIndex);
void __stdcall OnRecvFromServerTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);
void __stdcall OnRecvFromUserTCP(DWORD dwConnectionIndex,char* pMsg,DWORD dwLength);

void ReceivedFromDB(DBRECEIVEDATA* pResult);
void ReceivedFromDBBill(DBRECEIVEDATA* pResult);
void InitializePacketProc();
void LogoutMonitoringTool(CUser* pUser);
void ReceivedFromDBLog(DBRECEIVEDATA* pResult);


#endif 