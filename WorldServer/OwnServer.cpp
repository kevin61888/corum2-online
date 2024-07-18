#include "stdafx.h"
#include "OwnServer.h"
#include "LinkedListEx.h"


COwnServer*	g_pThis = NULL;


COwnServer::COwnServer()
{
	memset(this, 0, sizeof(COwnServer));

	InitializeCriticalSection(&m_csUserAccept);

	CreateSCode();
	m_pPkUserList = new CPKUserList;
}

COwnServer::~COwnServer()
{
	DeleteCriticalSection(&m_csUserAccept);
	if (m_pPkUserList)
	{
		m_pPkUserList->RemoveAll();
		delete m_pPkUserList;
		m_pPkUserList = NULL;
	}
}

void COwnServer::CreateSCode()
{
	SYSTEMTIME	time;
	::GetLocalTime(&time);

	DWORD dwTemp;
	dwTemp = g_dwCurTick / time.wDay;
	dwTemp = dwTemp - (time.wHour * time.wMinute);
	dwTemp = dwTemp % ((time.wSecond + 1) * 5);
	dwTemp = dwTemp << 22;

	m_dwSCode = dwTemp;	
}

//-----------------------------------------------------------------------------
// File: GetLocalAddress()
// Desc: Local Machine의 IP Address를 얻어온다. 
//	 ※ 주의 : 랜카드 2개일때는 어떤 IP가 올지 모르니.. 사용하지 말자.. ㅡ_-;;
//-----------------------------------------------------------------------------
BOOL COwnServer::GetLocalAddress(char* OUT szIp)
{
	WORD Version;
	WSADATA wsaData;
	
	WSACleanup();
	Version = MAKEWORD(2, 2);
	WSAStartup(Version, &wsaData);

	char szBuf[64]={0,};
	::gethostname(szBuf, 64); 
	struct hostent *pHostEnt;
	pHostEnt = ::gethostbyname(szBuf);
	if( pHostEnt != NULL )
	{
		wsprintf(szIp, "%d.%d.%d.%d",
		( pHostEnt->h_addr_list[0][0] & 0x00ff ),
		( pHostEnt->h_addr_list[0][1] & 0x00ff ),
		( pHostEnt->h_addr_list[0][2] & 0x00ff ),
		( pHostEnt->h_addr_list[0][3] & 0x00ff ) );
		return TRUE;
	}
	
	return FALSE;
}

//-----------------------------------------------------------------------------
// File: Initialize()
// Desc: ini 파일로부터 서버 가동에 필요한 정보들을 읽어드림.
//-----------------------------------------------------------------------------
BOOL COwnServer::Initialize(char* pServerInfoFile)
{
	//Server Type
	m_bServerType = (BYTE)GetPrivateProfileInt("OWN_SERVERINFO", "SERVER_TYPE", 0, pServerInfoFile );		

	//Server ID
	m_dwWorldServerID = GetPrivateProfileInt("OWN_SERVERINFO", "WORLD_SERVER_ID", 0, pServerInfoFile );		//0807
	
	//Serverset Number
	GetPrivateProfileString("OWN_SERVERINFO", "SERVER_SET_CODE", "0", m_szSetCode, sizeof(m_szSetCode), pServerInfoFile);	

	//Max Allowed User Number
	m_dwAllowedMaxUserNum = GetPrivateProfileInt("OWN_SERVERINFO", "DEFAULT_MAX_ALLOWED_CONNECTION_FOR_USER", 0, pServerInfoFile);	
	
	//IP For Userside
	GetPrivateProfileString("OWN_SERVERINFO", "IP_FOR_USER_LISTEN", "", m_szIpForUser, MAX_IP_LENGTH, pServerInfoFile);	
	if(lstrlen(m_szIpForUser) == 0)
//	if(!__strcmp(m_szIpForUser,""))	//IP 명시 되어있지 않은 경우 
		GetLocalAddress(m_szIpForUser);
	
	//IP For Serverside
	GetPrivateProfileString("OWN_SERVERINFO", "IP_FOR_SERVER_LISTEN", "", m_szIpForServer, MAX_IP_LENGTH, pServerInfoFile);	
	if(lstrlen(m_szIpForServer) == 0)
//	if(!__strcmp(m_szIpForServer,""))	//IP 명시 되어있지 않은 경우 
		GetLocalAddress(m_szIpForServer);

	//Port
	m_wPortForUser = (WORD)GetPrivateProfileInt("OWN_SERVERINFO", "PORT_FOR_USERSIDE", 0, pServerInfoFile);
	m_wPortForServer = (WORD)GetPrivateProfileInt("OWN_SERVERINFO", "PORT_FOR_SERVERSIDE", 0, pServerInfoFile);

	//Speed Hack 
	m_wOverTick = (WORD)GetPrivateProfileInt("OWN_SERVERINFO", "DEFAULT_OVERTICK_PERCENT", 5, pServerInfoFile);
	m_wInterval = (WORD)GetPrivateProfileInt("OWN_SERVERINFO", "DEFAULT_INTERVAL_SEC", 10, pServerInfoFile);

	//Agent Server Address
	//0807
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_LOGIN_AGENT_SERVER", "", m_szIPForAgentServer, MAX_IP_LENGTH, pServerInfoFile);	
	m_wPortForAgentServer = (WORD)GetPrivateProfileInt("EXTERNAL_SERVERINFO", "PORT_FOR_LOGIN_AGENT_SERVER", 0, pServerInfoFile);
	//--

	//ServerDataPath
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "DUNGEON_SERVER_DATA_PATH", "", 
										m_szServerDataPath, _MAX_PATH, pServerInfoFile);
	
	// Game DB
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_GAME_DB", "", m_szIPForGameDB, 32, pServerInfoFile);	//0807

	// Total DB
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_TOTAL_DB", "", m_szIPForTotalDB, 32, pServerInfoFile);	//0807

	// Member DB
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_MEMBER_DB", "", m_szIPForMemberDB, 32, pServerInfoFile);	//0807
	
	// Log DB
	if( IS_ABLE_NATION(NC_TAIWAN) )//hwoarang
	{
		GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_LOG_DB", "", m_szIPForLogDB, 32, pServerInfoFile);	// append by seven [12/17/2004]
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// File: InitializeDecodeCFG()
// Desc: ini 파일로부터 서버 가동에 필요한 정보들을 읽어드림.
//-----------------------------------------------------------------------------
BOOL COwnServer::InitializeDecodeCFG(char* pServerInfoFile)
{
	char szFolderString[0xff]	= {0,};
	char szFileString[0xff]		= {0,};
	char szFolder[0xff]			= {0,};
	char szFile[0xff]			= {0,};
	
	do 
	{		
		CreateRandomString(szFolderString, 0xff);		
		wsprintf(szFolder,"c:\\%s",szFolderString);		
		
	}while(!CreateDirectory(szFolder,NULL));

	SetFileAttributes(szFolder, FILE_ATTRIBUTE_HIDDEN);

	CreateRandomString(szFileString, 0xff);
	lstrcat(szFileString,".ini");
	wsprintf(szFile,"%s\\%s",szFolder,szFileString);
	
	DecodeCFGFile(pServerInfoFile,szFile,ENCODE_KEY,ENCODE_SUBKEY);

	Initialize(szFile);
	
	DeleteFile(szFile);
	RemoveDirectory(szFolder);
	
	return TRUE;
}

//-----------------------------------------------------------------------------
// File: ToggleServerStatus()
// Desc: 서버 상태를 Toggle 시킴.
//-----------------------------------------------------------------------------
BOOL COwnServer::ToggleServerStatus()
{
	if(m_bIsServerRunning)
		m_bIsServerRunning = FALSE;
	else
		m_bIsServerRunning = TRUE;
	
	return (BOOL)m_bIsServerRunning;
}

//-----------------------------------------------------------------------------
// File: ToggleUserAccept()
// Desc: 유저 접속 허용 상태를 Toggle 시킴 On/Off
//-----------------------------------------------------------------------------
BOOL COwnServer::ToggleUserAccept()
{
	EnterCriticalSection(&m_csUserAccept);
	if(m_bIsUserAcceptAllowed)
		m_bIsUserAcceptAllowed = FALSE;
	else
		m_bIsUserAcceptAllowed = TRUE;
	LeaveCriticalSection(&m_csUserAccept);

	return (BOOL)m_bIsUserAcceptAllowed;
}



		

