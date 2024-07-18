#ifndef __SERVERFUNCTION_H__
#define __SERVERFUNCTION_H__


#pragma once


#define UM_LISTEN_FOR_USERSIDE		WM_USER + 800
#define UM_LISTEN_FOR_SERVERSIDE	WM_USER + 801
#define UM_SERVER_RUNNING			WM_USER + 802


#define SCREEN_TEXT_SIZE_X			100
#define SCREEN_TEXT_SIZE_Y			100


#define BILLING_PROCESS_CHECK_TICK	60000		// 1Ка  
#define BILLING_CHECK_USER			300000		// 5Ка 


class CUser;
struct BILLING_INFO;


#pragma pack(push,1)
struct ServerRecord
{
	DWORD dwWorldMapID;
	char  szIpForUser[ MAX_IP_LENGTH ];
	WORD  wPortForUser;
	char  szIpForServer[ MAX_IP_LENGTH ];
	WORD  wPortForServer;
};
#pragma pack(pop)


extern void	DisplayDBMessage(char* szMsg);
extern void	DisplayDBReport(char* szMsg);
extern void	DisplayBillDBMessage(char* szMsg);
extern void	DisplayDBBillReport(char* szMsg);
extern void	InitNetwork();
extern void	ListenForUserside();
extern void	ListenForServerside();
extern void	InitOleDBThread();
extern bool	ConnectToDBServer();
extern BOOL	QueryAllServer();
extern BOOL	QueryBaseClassInfo();
extern BOOL	InitItemData();
extern void	CreateItem(CItem* pItem, BYTE byCount);
extern void	PrevUserProcess( DWORD dwPropID );
extern BOOL	SetBillingOrder(BILLING_INFO* pBillInfo);
extern time_t	GetDueDay(int nEndMin);
extern int		GetFreeTime(DBTIMESTAMP* pRecentLogoutTime, int nRemainedDBFreeTime);
extern void	SendToUserLoginFailPacket( CUser* pUser, BYTE bResult, DWORD dwExData );
extern void	TerminateServer();
extern void __stdcall GetDBResult(DWORD dwEventIndex);
extern void __stdcall GetBillingDBResult(DWORD dwEventIndex);
extern void __stdcall GetLogDBResult(DWORD dwEventIndex);
extern void __stdcall ShutDownServer(DWORD dwVal);
extern void __stdcall BillingCheck(DWORD dwEventIndex);
extern void __stdcall StartServerWithUserSide(DWORD dwEventIndex);
extern void __stdcall StartServerWithServerSide(DWORD dwEventIndex);
extern void __stdcall ShowAllServerStatus(DWORD dwEventIndex);
extern void __stdcall ShowCurUserCount(DWORD dwEventIndex);
extern void __stdcall IncreaseUserLimit(DWORD dwEventIndex);
extern void __stdcall DecreaseUserLimit(DWORD dwEventIndex);
extern void __stdcall LogOutAllUser(DWORD dwEventIndex);


#endif 