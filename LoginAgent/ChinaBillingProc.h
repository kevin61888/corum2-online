#ifndef __CHINA_BILLING_PROC_H__
#define __CHINA_BILLING_PROC_H__


#pragma once

#include "ChinaBillingPacket.h"

class CUser;

#define CHINA_BILLING_KEEP_ALIVE_SEND_PERIOD		30*1000 // 30 Sec.
#define CHINA_BILLING_CONNECT_PERIOD				10*1000	// 10 Sec.

#define CHINA_BILLING_KEEP_ALIVE_TIMER_INDEX		11
#define CHINA_BILLING_CONNECT_TIMER_INDEX			12

#define	CHINA_BILLING_FREE_LEVEL_LIMIT		10


// 
void InitializeProcFunction();
void ConnectToBillingServer();
void DisconnectBillingServer();

// INetWork registered event call back procedure.
void __stdcall FailToConnectBillingServer(void* pExt);								// process connection failed
void __stdcall SuccessToConnectBillingServer(DWORD dwConnectionIndex, void* pExt);	// process connection established
void __stdcall OnDisconnectBillingServer();											// process session closing with billing server
void __stdcall OnRecvFromBillingServerTCP(char* pMsg, DWORD dwLength);				// process received packet.
void __stdcall OnChinaBillingKeepAlive(DWORD dwEventIndex);							// timer event callback for keep alive.
void __stdcall OnChinaBillingTryConnect(DWORD dwEventIndex);						// timer event callback for trying connect to billing server.

// Login db proc for china billing
void QueryTyprLoginChinaBilling(DBRECEIVEDATA* pResult);


// msg proc for recv.
void OnBillingMsgLoginResult(char* pMsg, DWORD dwLength);
void OnBillingMsgKickOut(char* pMsg, DWORD dwLength);
void OnBillingMsgClear(char* pMsg, DWORD dwLength);

// send function.
void SendToChinaBillingServer(char* szMsg, DWORD dwLen, BOOL bTimer = TRUE);

void SendChinaBillingLogin(DWORD dwUserIndex, LPCSTR szUserName, DWORD dwIP, BOOL bTimer = TRUE);
void SendChinaBillingLogout(DWORD dwUserIndex, LPCSTR szUserName, DWORD dwIP);
void SendChinaBillingClear();
void SendChinaBillingDummy();
void SendChinaBillingLoginAll();
void SendChinaBillingInfo(CUser * pUser, DWORD dwUserIndex, enum BILLING_INFO_PROCEDURE_TYPE eProcType);


#endif // __CHINA_BILLING_PROC_H__