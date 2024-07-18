#ifndef __LICENSING_PROC_H__
#define __LICENSING_PROC_H__

#pragma once

#define MAX_LICENSING_PACKET_TYPE				10

#define LICENSING_SERVER_IP						"222.234.223.195"
#define LICENSING_SERVER_PORT					12210

#define LICENSING_HEARTBEAT_SEND_PERIOD			60*1000		// 60 Sec.
#define LICENSING_CONNECT_PERIOD				10*1000		// 10 Sec.
#define LICENSING_SEND_INFO_PERIOD				10*60*1000	// 10 MIN

#define LICENSING_HEARTBEAT_TIMER_INDEX			13
#define LICENSING_CONNECT_TIMER_INDEX			14
#define LICENSING_SEND_INFO_INDEX				15


void InitializeProcFunctionForLicensing();
BOOL ConnectToLicensingServer();
void DisconnectLicensingServer();

// INetWork registered event call back procedure.
void __stdcall FailToConnectLicensingServer(void* pExt);								// process connection failed
void __stdcall SuccessToConnectLicensingServer(DWORD dwConnectionIndex, void* pExt);	// process connection established
void __stdcall OnDisconnectLicensingServer();											// process session closing with Licensing server
void __stdcall OnRecvFromLicensingServerTCP(char* pMsg, DWORD dwLength);				// process received packet.
void __stdcall OnLicensingHeartbeat(DWORD dwEventIndex);								// timer event callback for heartbeat.
void __stdcall OnLicensingTryConnect(DWORD dwEventIndex);								// timer event callback for trying connect to Licensing server.
void __stdcall OnLicensingSendInfo(DWORD dwEventIndex);									// timer event callback for sending infomations.

// msg proc for recv.
void OnLicensingHeartbeat(char* pMsg, DWORD dwLength);
void OnLicensingAuthenticationResult(char* pMsg, DWORD dwLength);
void OnLicensingCommand(char* pMsg, DWORD dwLength);

// send functions..
void SendToLicensingServer(char* szMsg, DWORD dwLen, BOOL bTimer = TRUE);

void SendLicensingHeartbeat();
void SendLicensingAuthentication();
void SendLicensingCurrentUser(DWORD dwCCU);
void SendLicensingBillingInformation(char* pData, DWORD dwSize);


void KickAllUser();


#endif // __LICENSING_PROC_H__