#include "stdafx.h"
#include "OwnServer.h"
#include "./../CommonServer/Encode.h"
#include "./../CommonServer/Log.h"
#include "./../CommonServer/Item.h"



extern IOleDBThread* g_pDb;


COwnServer::COwnServer()
{
	memset(this, 0, sizeof(COwnServer));

	InitializeCriticalSection(&m_csUserAccept);

	CreateSCode();
	
	m_dwSerialCode = 0xffffffff;
}

COwnServer::~COwnServer()
{
	DeleteCriticalSection(&m_csUserAccept);
}

void COwnServer::CreateSCode()
{
	SYSTEMTIME	time;
	::GetLocalTime(&time);

	DWORD dwTemp;
	dwTemp = GetTickCount() / time.wDay;
	dwTemp = dwTemp - (time.wHour * time.wMinute);
	dwTemp = dwTemp % ((time.wSecond + 1) * 5);
	dwTemp = dwTemp << 22;

	m_dwSCode = dwTemp;	
}

DWORD	COwnServer::GetNewSerial(BYTE bCharNum)
{
	bCharNum;

	m_dwSerialCode--;
	return m_dwSerialCode;
	/*
	m_dwSerialCode += 10;

	DWORD dwTemp = (m_dwSerialCode + bCharNum) << 22;
	return dwTemp;
	*/
}

//-----------------------------------------------------------------------------
// File: GetLocalAddress()
// Desc: Local Machine의 IP Address를 얻어온다. 
//	 ※ 주의 : 랜카드 2개일때는 어떤 IP가 올지 모르니.. 사용하지 말자.. ㅡ_-;;
//-----------------------------------------------------------------------------
BOOL COwnServer::GetLocalAddress(char* OUT szIp)
{
	WORD Version = MAKEWORD(2, 2);
	WSADATA wsaData;
	struct hostent *pHostEnt = NULL;
	char szBuf[64]={0,};

	WSACleanup();
	WSAStartup(Version, &wsaData);
	
	::gethostname(szBuf, 64); 
	
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

BOOL COwnServer::Initialize()
{
	switch( MY_NATION_CODE )
	{
	case NC_KOREA:
		{
			return Initialize(".\\Server.ini");
		}break;
	default:
		{
			return InitializeDecodeCFG(".\\Server.cfg");
		}break;
	}
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
	m_dwServerID = GetPrivateProfileInt("OWN_SERVERINFO", "IP_FOR_USER_LISTEN", 0, pServerInfoFile );		
	
	//Serverset Number
	GetPrivateProfileString("OWN_SERVERINFO", "SERVER_SET_CODE", "0", m_szSetCode, sizeof(m_szSetCode), pServerInfoFile);	
	m_dwServerSetNum	= GetPrivateProfileInt("OWN_SERVERINFO", "SERVER_SET_CODE", 0, pServerInfoFile);	
	m_dwDevServerSetNum = GetPrivateProfileInt("OWN_SERVERINFO", "DEVMODE_SERVER_SET", 100, pServerInfoFile);	

	//Max Allowed User Number
	m_dwAllowedMaxUserNum = GetPrivateProfileInt("OWN_SERVERINFO", "DEFAULT_MAX_ALLOWED_CONNECTION_FOR_USER", 0, pServerInfoFile);	
	
	//IP For Userside
	GetPrivateProfileString("OWN_SERVERINFO", "IP_FOR_USER_LISTEN", "", m_szIpForUser, MAX_IP_LENGTH, pServerInfoFile);	
	if(lstrlen(m_szIpForUser) == 0)	//IP 명시 되어있지 않은 경우 
		GetLocalAddress(m_szIpForUser);
	
	//IP For Serverside
	GetPrivateProfileString("OWN_SERVERINFO", "IP_FOR_SERVER_LISTEN", "", m_szIpForServer, MAX_IP_LENGTH, pServerInfoFile);	
	if(lstrlen(m_szIpForServer) == 0) //IP 명시 되어있지 않은 경우 
		GetLocalAddress(m_szIpForServer);

	//Port
	m_wPortForUser = (WORD)GetPrivateProfileInt("OWN_SERVERINFO", "PORT_FOR_USERSIDE", 0, pServerInfoFile);
	m_wPortForServer = (WORD)GetPrivateProfileInt("OWN_SERVERINFO", "PORT_FOR_SERVERSIDE", 0, pServerInfoFile);
	
#ifdef CHINA_BILLING
	GetPrivateProfileString("OWN_SERVERINFO", "IP_FOR_BILLING_SERVER", "192.168.0.186",m_szIPForBillingServer, _MAX_PATH, pServerInfoFile);
	m_wPortForBillingServer = GetPrivateProfileInt("OWN_SERVERINFO", "PORT_FOR_BILLING_SERVER", 14999, pServerInfoFile);

	Log(LOG_NORMAL, "BillingServer(IP: %s, Port: %d)", m_szIPForBillingServer, m_wPortForBillingServer);
#endif

	//ServerDataPath
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "WORLD_SERVER_DATA_PATH", "",m_szServerDataPath, _MAX_PATH, pServerInfoFile);

	// Game DB
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_GAME_DB", "", m_szIpForGameDB, 32, pServerInfoFile);
	
	// Syste DB
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_TOTAL_DB", "", m_szIpForTotalDB, 32, pServerInfoFile);
	
	// Member DB
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_MEMBER_DB", "", m_szIpForMemberDB, 32, pServerInfoFile);

	// Log DB 
	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_LOG_DB", "", m_szIpForLogDB, 32, pServerInfoFile);

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
// File: ToggleUserAccept()
// Desc: 유저 접속 허용 상태를 Toggle 시킴 On/Off
//-----------------------------------------------------------------------------
BOOL COwnServer::ToggleUserAccept()
{
	EnterCriticalSection(&m_csUserAccept);

	m_bIsUserAcceptAllowed = !m_bIsUserAcceptAllowed;

	LeaveCriticalSection(&m_csUserAccept);

	return (BOOL)m_bIsUserAcceptAllowed;
}		

//-----------------------------------------------------------------------------
// File: GetNewSerial()
// Desc: Item 생성 시에 새로운 Item Serial 번호를 생성한다. 
//	     Added by KBS 030115
//-----------------------------------------------------------------------------
ITEM_SERIAL	COwnServer::GetNewSerial()
{
	m_Serial.ItemSerialStruct.dwSerialNum++;
	return m_Serial;
}

BOOL COwnServer::ResetItemSerialNumber()
{
	char szQuery[ QUERY_STRING_SIZE ]={0,};
	int nRet = 0;

	wsprintf(szQuery, "Select ServerSerialID from ServerTable "
						"Where Port = %d",  m_wPortForUser);

	BYTE bSerialID;
	nRet = g_pDb->OpenRecord(szQuery, &bSerialID, 1, (BYTE)GAME_DB );

	if(nRet < 0)
	{
		Log(LOG_IMPORTANT, "Fail to Query ServerSerialID!(Port=%d)", m_wPortForUser);
		return FALSE;
	}

	wsprintf(szQuery, "CZP_RESET_ITEM_SERIAL %d",  m_wPortForUser );
	int nRestartCount = -1;
	nRet = g_pDb->OpenRecord(szQuery, &nRestartCount, 1, (BYTE)GAME_DB );

	if(nRet < 0)
	{
		Log(LOG_IMPORTANT, "Fail to Reset ItemSerialCount!(Port=%d)", m_wPortForUser);
		return FALSE;
	}

	ITEM_SERIAL_STRUCT &rISS = m_Serial.ItemSerialStruct;
	rISS.bServerSet = (BYTE)atoi(GetServerSetCode());
	rISS.bServerSerialID = bSerialID;
	rISS.wRestartCount = (WORD)nRestartCount;
	rISS.dwSerialNum = 0;
	
	Log(LOG_JUST_DISPLAY, "@ ItemSerialCount Reset Successfully! (RestartCount = %d)", nRestartCount);
	//--

	return TRUE;
}

