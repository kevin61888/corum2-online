#ifndef __OWN_SERVER_H__
#define __OWN_SERVER_H__

#pragma once

#include "GlobalDefine.h"
#include "ItemManager.h"

#define DEBUG_MODE

//현재 서버정보 클래스 
class COwnServer
{

	BYTE				m_bServerType;								//Server Type
	DWORD				m_dwServerID;								//Server ID
	DWORD				m_dwServerSetNum;							//ServerSet Number	
	DWORD				m_dwDevServerSetNum;
	char				m_szSetCode[10];								//ServerSet Code
	DWORD				m_dwAllowedMaxUserNum;						//이서버가 허용하는 유저 접속수 
	BOOL				m_bIsServerRunning;							//현재 서버가 가동중인가 아닌가 여부 
	BOOL				m_bIsUserAcceptAllowed;						//유저 접속 허용 여부 

	char				m_szIpForUser[ MAX_IP_LENGTH ];				//User Listen용 IP
	char				m_szIpForServer[ MAX_IP_LENGTH ];			//Server Listen용 IP
	char				m_szServerDataPath[ _MAX_PATH ];			//Dungeon Server Data Path
	WORD				m_wPortForUser;								//Port for User
	WORD				m_wPortForServer;							//Port for Server
	
	char				m_szIpForLogDB[32];
	char				m_szIpForMemberDB[32];
	char				m_szIpForTotalDB[32];
	char				m_szIpForGameDB[32];

	DWORD				m_dwSCode;									//Secret Code ㅡ_-;;
	DWORD				m_dwSerialCode;								//유저의 World서버 연결을 위한 SerialCode

// CHINA_BILLING
	char				m_szIPForBillingServer[ MAX_IP_LENGTH ];
	WORD				m_wPortForBillingServer;
	BOOL				m_bBillingServerConnected;
	BOOL				m_bBillingServerClosing;
	BOOL				m_bIsTryingReconnectBillingServer;
	DWORD				m_dwBillingServerConnectionIndex;

// USE_LICENSING_SERVER
	BOOL				m_bLicensingServerConnected;
	BOOL				m_bLicensingServerClosing;
	BOOL				m_bIsTryingReconnectLicensingServer;
	DWORD				m_dwLicensingServerConnectionIndex;
	BOOL				m_bAuthenticated;
	
	CRITICAL_SECTION	m_csUserAccept;
	ITEM_SERIAL			m_Serial;

	void				CreateSCode();
public:

	HANDLE				m_hKeyEvent[10];							//KeyEvent;	

	BYTE				m_bUserListening;							//User Listen 했는지의 여부 
	BYTE				m_bServerListening;							//Server Listen 했는지의 여부 
	
	COwnServer();
	~COwnServer();

	BOOL	Initialize();
	BOOL	Initialize				(char* pServerInfoFile);
	BOOL	InitializeDecodeCFG		(char* pServerInfoFile);
	BOOL	GetLocalAddress			(char* OUT szIp);	
	BOOL	ToggleUserAccept		();
	BOOL	IsUserAcceptAllowed()	{	return m_bIsUserAcceptAllowed;	}

// CHINA_BILLING
	char*	GetIPForBillingServer()							{	return m_szIPForBillingServer; }
	WORD	GetPortForBillingServer()						{	return m_wPortForBillingServer; }
	DWORD	GetBillingServerConnectionIndex()				{	return m_dwBillingServerConnectionIndex; }	
	void	SetBillingServerConnectionIndex(DWORD dwIndex)	{	m_dwBillingServerConnectionIndex = dwIndex; }
	BOOL	IsBillingServerConnected()						{	return m_bBillingServerConnected; }
	void	SetBillingServerConnected(BOOL bConnected)		{	m_bBillingServerConnected = bConnected; }
	void	SetBillingServerClosing(BOOL bClosing)			{	m_bBillingServerClosing = bClosing;	}
	BOOL	IsBillingServerConnectionClosing()				{	return m_bBillingServerClosing;	}
	BOOL	IsTryToConnectBillingServerContinuous()			{	return m_bIsTryingReconnectBillingServer; }
	void	SetTryToConnectBillingServer(BOOL bTry)			{	m_bIsTryingReconnectBillingServer = bTry; }
	
// USE_LICENSING_SERVER
	DWORD	GetLicensingServerConnectionIndex()				{	return m_dwLicensingServerConnectionIndex; }	
	void	SetLicensingServerConnectionIndex(DWORD dwIndex){	m_dwLicensingServerConnectionIndex = dwIndex; }
	BOOL	IsLicensingServerConnected()					{	return m_bLicensingServerConnected; }
	void	SetLicensingServerConnected(BOOL bConnected)	{	m_bLicensingServerConnected = bConnected; }
	void	SetLicensingServerClosing(BOOL bClosing)		{	m_bLicensingServerClosing = bClosing;	}
	BOOL	IsLicensingServerConnectionClosing()			{	return m_bLicensingServerClosing;	}
	BOOL	IsTryToConnectLicensingServerContinuous()		{	return m_bIsTryingReconnectLicensingServer; }
	void	SetTryToConnectLicensingServer(BOOL bTry)		{	m_bIsTryingReconnectLicensingServer = bTry; }
	BOOL	IsAuthenticated()								{	return m_bAuthenticated;	}
	void	SetAuthenticated(BOOL bSet)						{	m_bAuthenticated = bSet;	}

	
	
	BYTE	GetServerType()			{	return m_bServerType;		}
	BOOL	IsServerRunning()		{	return m_bIsServerRunning;	}
	DWORD	GetServerID()			{	return m_dwServerID;		}
	DWORD	GetServerSetNum()		{	return m_dwServerSetNum;	}
	DWORD	GetDevModeServerSetNum(){	return m_dwDevServerSetNum;	}
	char*	GetServerSetCode()		{	return m_szSetCode;			}
//	DWORD	GetServerSetNum()		{	return m_dwServerSetNum;	}
	DWORD	GetMaxUserAllowedNum()	{	return m_dwAllowedMaxUserNum;	}
	WORD	GetPortForUser()		{	return m_wPortForUser;		}
	WORD	GetPortForServer()		{	return m_wPortForServer;	}
	char*	GetIPForUser()			{	return m_szIpForUser;		}
	char*	GetIPForServer()		{	return m_szIpForServer;		}
	char*	GetServerDataPath()		{	return m_szServerDataPath;	}

	char*	GetIPForLogDB()			{	return m_szIpForLogDB;		}
	char*	GetIPForMemberDB()		{	return m_szIpForMemberDB;	}
	char*	GetIPForTotalDB()		{	return m_szIpForTotalDB;	}
	char*	GetIPForGameDB()		{	return m_szIpForGameDB;		}
	
	DWORD	GetNewSerial(BYTE bCharNum);	//For WorldServer

	void	SetMaxUserAllowedNum(DWORD dwMaxAllowed)	{	m_dwAllowedMaxUserNum = dwMaxAllowed;	}

		
	BOOL		ResetItemSerialNumber();	
	ITEM_SERIAL	GetNewSerial();				//For Item Create	

};


//struct MACHINENAME
//{
//	char	szBuffer[MAX_COMPUTERNAME_LENGTH+1];
//	DWORD	dwSize;
//	WORD	wPort;
//	BYTE	bServerType;
//	char	szIpForUser[MAX_IP_LENGTH];	
//
//	MACHINENAME()
//	{
//		memset(szBuffer,0,sizeof(szBuffer));
//		dwSize		= MAX_COMPUTERNAME_LENGTH+1;
//		wPort		= 0; 
//		bServerType = 0;
//		memset(szIpForUser,0,sizeof(szIpForUser));
//	}
//};

extern COwnServer*	g_pThis;
extern MACHINENAME g_MachineName;

#endif // __OWN_SERVER_H__
