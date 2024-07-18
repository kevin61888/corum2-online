#ifndef __OWN_SERVER_H__
#define __OWN_SERVER_H__

#pragma once

#include "GlobalDefine.h"
#include "LinkedListEx.h"

#define DEBUG_MODE

//현재 서버정보 클래스 
class COwnServer
{

	BYTE				m_bServerType;								//Server Type
	DWORD				m_dwWorldServerID;							//World Server ID	//0807
	DWORD				m_dwConnectionIndexForAgentServer;			//Agent Server ConnectionIndex //0807
	char				m_szSetCode[MAX_PATH];								//ServerSet Code
	DWORD				m_dwAllowedMaxUserNum;						//이서버가 허용하는 유저 접속수 
	BOOL				m_bIsServerRunning;							//현재 서버가 가동중인가 아닌가 여부 
	BOOL				m_bIsUserAcceptAllowed;						//유저 접속 허용 여부 

	char				m_szIpForUser[ MAX_IP_LENGTH ];				//User Listen용 IP
	char				m_szIpForServer[ MAX_IP_LENGTH ];			//Server Listen용 IP
	char				m_szIPForAgentServer[ MAX_IP_LENGTH ];		//Agent Server 접속용 IP	//0807
	char				m_szIPForLogDB[ 32 ];			//Log DB

	char				m_szIPForGameDB[32];
	char				m_szIPForTotalDB[32];
	char				m_szIPForMemberDB[32];


	char				m_szServerDataPath[ _MAX_PATH ];			//Dungeon Server Data Path
	WORD				m_wPortForUser;								//Port for User
	WORD				m_wPortForServer;							//Port for Server
	WORD				m_wPortForAgentServer;						//Port For AgentServer      //0807

	WORD				m_wOverTick;				//speed hack over time //add seven [7/23/2004]
	WORD				m_wInterval;				//speed hack over time //add seven [7/23/2004]

	DWORD				m_dwSCode;									//Secret Code ㅡ_-;;
	
	CRITICAL_SECTION	m_csUserAccept;

	
	void				CreateSCode();
public:
	CPKUserList*		m_pPkUserList;
	DWORD				m_GLOBAL_MAGIC_FIND_PBT;					// 이벤트로 인한 마법 떨구는 확률
	DWORD				m_dwGLOBAL_EXP_GAIN;
	DWORD				m_dwGLOBAL_PARTY_BONUS_EXP_GAIN;			// Party Bonus Exp Gain by minjin.	2004. 12. 07.
	HANDLE				m_hKeyEvent[15];							//KeyEvent;	
	BYTE				m_bAdultMode;
	
	BYTE				m_bUserListening;							//User Listen 했는지의 여부
	BYTE				m_bServerListening;							//Server Listen 했는지의 여부
	
	COwnServer();
	~COwnServer();

	BOOL	Initialize			(char* pServerInfoFile);
	BOOL	InitializeDecodeCFG	(char* pServerInfoFile);
	BOOL	GetLocalAddress(char* OUT szIp);	
	BOOL	ToggleServerStatus();	
	BOOL	ToggleUserAccept();
	BOOL	IsUserAcceptAllowed()	{	return m_bIsUserAcceptAllowed;	}
	
	
	BYTE	GetServerType()			{	return m_bServerType;		}
	BOOL	IsServerRunning()		{	return m_bIsServerRunning;	}
	DWORD	GetServerID()			{	return m_dwWorldServerID;		}	//0807
	DWORD	GetAgentConnectionIndex()	{	return m_dwConnectionIndexForAgentServer;	}	//0807
	char*	GetServerSetCode()		{	return m_szSetCode;			}
//	DWORD	GetServerSetNum()		{	return m_dwServerSetNum;	}
	DWORD	GetMaxUserAllowedNum()	{	return m_dwAllowedMaxUserNum;	}
	WORD	GetPortForUser()		{	return m_wPortForUser;		}
	WORD	GetPortForServer()		{	return m_wPortForServer;	}
	WORD	GetPortForAgentServer()	{	return m_wPortForAgentServer;	}	//0807
	WORD	GetNTOSOverTick()		{	return m_wOverTick; }	// [7/23/2004]
	WORD	GetNTOSInterval()		{	return m_wInterval; }	// [7/23/2004]
	char*	GetIPForUser()			{	return m_szIpForUser;		}
	char*	GetIPForServer()		{	return m_szIpForServer;		}
	char*   GetIPForAgentServer()	{	return m_szIPForAgentServer;	}	//0807
	char*	GetServerDataPath()		{	return m_szServerDataPath;	}

	char*	GetIPForGameDB()		{	return m_szIPForGameDB;		}
	char*	GetIPForTotalDB()		{	return m_szIPForTotalDB;	}
	char*	GetIPForMemberDB()		{	return m_szIPForMemberDB;	}
	char*	GetIPForLogDB()			{	return m_szIPForLogDB;		}

	
	void	SetServerStatus(BOOL bRunning)	{	m_bIsServerRunning = bRunning;	}
	void	SetAgentConnectionIndex(DWORD dwIndex)	{ m_dwConnectionIndexForAgentServer = dwIndex;	}	//0807

};


extern COwnServer*	g_pThis;
extern MACHINENAME g_MachineName;

#endif // __OWN_SERVER_H__