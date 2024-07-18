#include "stdafx.h"
#include "OwnServer.h"
#include "../CommonServer/CommonHeader.h"
#include "DUNGEON_DATA_EX.h"
#include "Linkedlist.h"
#include "DungeonTable.h"
#include "Dungeon.h"
#include "GameSystem.h"


#define CONSOLE_TITLE	"Dungeon Server - [CorumOnline]"
#define CONSOLE_TITLE2	"Village Server - [CorumOnline]"


COwnServer::COwnServer()
{
	memset(this, 0, sizeof(COwnServer));
	InitializeCriticalSection(&m_csUserAccept);

	m_pSerial		= new ITEM_SERIAL;
	m_pDungeonInfo	= new DUNGEON_DATA_EX[MAX_DUNGEON_PER_SERVER];
}

COwnServer::~COwnServer()
{
	if (m_pPKUserCheckList)
	{
		m_pPKUserCheckList->RemoveAll();
		delete m_pPKUserCheckList;
		m_pPKUserCheckList  = NULL;
	}

	if (m_pSerial)
	{
		delete m_pSerial;
		m_pSerial = NULL;
	}

	if (m_pDungeonInfo)
	{
		delete [] m_pDungeonInfo;
		m_pDungeonInfo = NULL;
	}

	DeleteCriticalSection(&m_csUserAccept);	
}

//-----------------------------------------------------------------------------
// Local Machine의 IP Address를 얻어온다. ※ 주의 : 랜카드 2개일때는 어떤 IP가 올지 모르니.. 사용하지 말자.. ㅡ_-;;
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


BOOL COwnServer::LoadConfigFile(char* pServerConfigInfoFile)
{
	m_wMAX_USER_CONNECTION = (WORD)GetPrivateProfileInt("PUBLIC_VARIABLE", "MAX_USER_CONNECTION", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_USER = (WORD)GetPrivateProfileInt("PUBLIC_VARIABLE", "HASH_SIZE_USER", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_MONSTER = (WORD)GetPrivateProfileInt("PUBLIC_VARIABLE", "POOL_SIZE_MONSTER", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_TRADER_MONSTER = (WORD)GetPrivateProfileInt("PUBLIC_VARIABLE", "POOL_SIZE_TRADER_MONSTER", 100, pServerConfigInfoFile );
	m_wPOOL_SIZE_USER = (WORD)GetPrivateProfileInt("PUBLIC_VARIABLE", "POOL_SIZE_USER", 0, pServerConfigInfoFile );
	m_dwMaxServerNum = GetPrivateProfileInt("PUBLIC_VARIABLE", "MAX_SERVER_NUM", 0, pServerConfigInfoFile );
	
	m_wPOOL_SIZE_ITEMTRADE				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_ITEMTRADE", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_ITEMSTORE				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_ITEMSTORE", MAX_ITEM_STORE_ENTITY, pServerConfigInfoFile );
	m_wPOOL_SIZE_ITEMSHOP				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_ITEMSHOP", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_ITEMTILE				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_ITEMTILE", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_GUILDLIST				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_GUILDLIST", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_SHOPLOG				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_SHOPLOG", 0, pServerConfigInfoFile );
	m_dwPOOL_SIZE_NODE					= GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_NODE", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_ITEM					= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_ITEM", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_CREATEITEM				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_CREATEITEM", 0, pServerConfigInfoFile );
	m_wPOOL_SIZE_PORTAL					= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "POOL_SIZE_PORTAL", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_SETITEMINFO			= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_SETITEMINFO", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_MAKEITEMINFO			= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_MAKEITEMINFO", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_BASEITEM				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_BASEITEM", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_ITEMTRADE				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_ITEMTRADE", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_ITEMSHOP				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_ITEMSHOP", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_SHOPLOG				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_SHOPLOG", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_GUILDLIST				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_GUILDLIST", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_ITEMTILE				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_ITEMTILE", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_ITEMSTORE				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_ITEMSTORE", MAX_ITEM_STORE_ENTITY, pServerConfigInfoFile );
	m_wHASH_SIZE_BASEMONSTER			= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_BASEMONSTER", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_SETITEM				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_SETITEM", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_INVALIDLOG				= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_INVALIDLOG", 0, pServerConfigInfoFile );
	m_wHASH_SIZE_STORENATIVE			= (WORD)GetPrivateProfileInt("PRIVATE_VARIABLE", "HASH_SIZE_STORENATIVE", 0, pServerConfigInfoFile );
	m_dwMainMsgQueMaxBufferSize			= GetPrivateProfileInt("PRIVATE_VARIABLE", "MAIN_MSGQUE_MAX_BUFFER_SIZE", 0, pServerConfigInfoFile );
	m_dwServerBufferSizePerConnection	= GetPrivateProfileInt("PRIVATE_VARIABLE", "SERVER_BUFFER_SIZE_PER_CONNECTION", 0, pServerConfigInfoFile );
	m_dwServerMaxTransferSize			= GetPrivateProfileInt("PRIVATE_VARIABLE", "SERVER_MAX_TRANSFER_SIZE", 0, pServerConfigInfoFile );
	m_dwUserBufferSizePerConnection		= GetPrivateProfileInt("PRIVATE_VARIABLE", "USER_BUFFER_SIZE_PER_CONNECTION", 0, pServerConfigInfoFile );
	m_dwUserMaxTransferSize				= GetPrivateProfileInt("PRIVATE_VARIABLE", "USER_MAX_TRANSFER_SIZE", 0, pServerConfigInfoFile );
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// ini 파일로부터 서버 가동에 필요한 정보들을 읽어드림.
//-----------------------------------------------------------------------------
BOOL COwnServer::Initialize(char* pServerInfoFile, char* pServerConfigInfoFile)
{
	m_bServerType = (BYTE)GetPrivateProfileInt("OWN_SERVERINFO", "SERVER_TYPE", 0, pServerInfoFile );		

	m_dwServerID = GetPrivateProfileInt("OWN_SERVERINFO", "SERVER_ID", 0, pServerInfoFile );		
	m_dwDevServerSetNum = GetPrivateProfileInt("OWN_SERVERINFO", "DEVMODE_SERVER_SET", 100, pServerInfoFile);	

#if defined TAIWAN_LOCALIZING
	m_dwServerSetNum = GetPrivateProfileInt("OWN_SERVERINFO", "SERVER_SET_CODE", 0, pServerInfoFile);	
#else
	m_dwServerSetNum = GetPrivateProfileInt("OWN_SERVERINFO", "SERVER_SET", 0, pServerInfoFile);	
#endif

	m_dwAllowedMaxUserNum = GetPrivateProfileInt("OWN_SERVERINFO", "DEFAULT_MAX_ALLOWED_CONNECTION_FOR_USER", 0, pServerInfoFile);	
	
	GetPrivateProfileString("OWN_SERVERINFO", "IP_FOR_USER_LISTEN", "", m_szIpForUser, MAX_IP_LENGTH, pServerInfoFile);	
	if(lstrlen(m_szIpForUser) == 0)
		GetLocalAddress(m_szIpForUser);
	
#ifdef TAIWAN_LOCALIZING
	GetPrivateProfileString("OWN_SERVERINFO", "IP_FOR_EXTERN", "", m_szIpForExtern, MAX_IP_LENGTH, pServerInfoFile);	
	if(lstrlen(m_szIpForExtern) == 0)
	{
		MessageBox(NULL, "Check Extern IP in server.ini!", "Alert!", MB_OK);
		return FALSE;
	}
#endif
	
	GetPrivateProfileString("OWN_SERVERINFO", "IP_FOR_SERVER_LISTEN", "", m_szIpForServer, MAX_IP_LENGTH, pServerInfoFile);	
	if(lstrlen(m_szIpForServer) == 0)
		GetLocalAddress(m_szIpForServer);

	GetPrivateProfileString("EXTERNAL_SERVERINFO", "IP_FOR_WORLD_SERVER", "", m_szIpForWorldServer, MAX_IP_LENGTH, pServerInfoFile);	
	
	m_wPortForUser			= (WORD)GetPrivateProfileInt("OWN_SERVERINFO", "PORT_FOR_USERSIDE", 0, pServerInfoFile);
	m_wPortForServer		= (WORD)GetPrivateProfileInt("OWN_SERVERINFO", "PORT_FOR_SERVERSIDE", 0, pServerInfoFile);
	m_wPortForWorldServer	= (WORD)GetPrivateProfileInt("EXTERNAL_SERVERINFO", "PORT_FOR_WORLD_SERVER", 0, pServerInfoFile);

	GetPrivateProfileString("EXTERNAL_SERVERINFO", "DUNGEON_SERVER_DATA_PATH", "", m_szServerDataPath, _MAX_PATH, pServerInfoFile);

	if(m_bServerType == SERVER_TYPE_OCCUPIED_DUNGEON || m_bServerType == SERVER_TYPE_NON_OCCUPIED_DUNGEON || m_bServerType == SERVER_TYPE_PATHWAY_DUNGEON )
	{
		m_wLogColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE;
		SetConsoleTitle(CONSOLE_TITLE);
		Log(LOG_NORMAL, "########### CorumZero Online - Dungeon Server Started! ###########");
	}
	else if(m_bServerType == SERVER_TYPE_VILLAGE)
	{
		m_wLogColor = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_GREEN;
		SetConsoleTitle(CONSOLE_TITLE2);
		Log(LOG_NORMAL, "########### CorumZero Online - Village Server Started! ###########");
	}

	m_pPKUserCheckList	= new CVoidList;
	LoadConfigFile(pServerConfigInfoFile);

	return TRUE;
}


//-----------------------------------------------------------------------------
// ini 파일로부터 서버 가동에 필요한 정보들을 읽어드림.
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
	Initialize(szFile, ".\\DungeonConfig.ini");
	
	DeleteFile(szFile);
	RemoveDirectory(szFolder);
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// SYSTEM DB로 부터 현재 이서버가 가지고 있어야 할 던전들의 구성을 받아온다.
//-----------------------------------------------------------------------------
BOOL COwnServer::RequestDungeonInfo()
{
	char szQuery[ 0xff ]={0,};
	wsprintf(szQuery, "CZP_REQUEST_DUNGEON_INFO_DG %d",  m_wPortForUser);
	
	DUNGEON_DATA DungeonInfo[MAX_DUNGEON_PER_SERVER];
	int nRet = g_pDb->OpenRecord(szQuery, &DungeonInfo[0], MAX_DUNGEON_PER_SERVER, GAME_DB );

	if(nRet < 0)
	{
		Log(LOG_IMPORTANT, "Fail to Query DungeonInfo!(Port=%d)", m_wPortForUser);
		return FALSE;
	}
	
	for(int i = 0; i < nRet; ++i)
	{
		memcpy((DUNGEON_DATA*)&m_pDungeonInfo[i], &DungeonInfo[i], sizeof(DUNGEON_DATA));
	}

	m_dwTotalDungeonNum = (DWORD)nRet;

	WORD wOldID = 0;
	WORD wNewID = 0;

	for(i = 0; i < nRet; ++i)
	{
		wOldID = m_pDungeonInfo[i].m_cMagicFieldArrayItem.GetID();
		if(wOldID != 0)
		{
			wNewID = WORD(( wOldID / 100 * 200 ) + wOldID % 100);
			m_pDungeonInfo[i].m_cMagicFieldArrayItem.SetID(wNewID);
		}		
		
		wOldID = m_pDungeonInfo[i].m_cGuardianItem.GetID();
		if(wOldID != 0)
		{
			wNewID = WORD(( wOldID / 100 * 200 ) + wOldID % 100);
			m_pDungeonInfo[i].m_cGuardianItem.SetID(wNewID);
		}		
	}
	
	Log(LOG_JUST_DISPLAY, "@ DungeonInfo Query Successfully!");

	return TRUE;
}
		

//-----------------------------------------------------------------------------
// 유저 접속 허용 상태를 Toggle 시킴 On/Off
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


//-----------------------------------------------------------------------------
// 이서버가 일반 던전서버일 경우 지정된 던전을 생성
//-----------------------------------------------------------------------------
BOOL COwnServer::PreLoadDungeon()
{	
	for(DWORD i=0; i<m_dwTotalDungeonNum; i++)
	{
		CDungeon* pDungeon = g_pDungeonTable->CreateNewDungeon( (WORD)m_pDungeonInfo[ i ].m_dwID );
		
		pDungeon->SetDungeonInfo(&m_pDungeonInfo[ i ], m_bServerType);
		pDungeon->Build();	
		
		g_pDungeonTable->Add(pDungeon);
		
		Log(LOG_JUST_DISPLAY, "@ Dungeon(%s) Create Successfully!", m_pDungeonInfo[ i ].m_szDungeonName);	
	}
	
	return TRUE;
}


//-----------------------------------------------------------------------------
// Item 생성 시에 새로운 Item Serial 번호를 생성한다. 
//-----------------------------------------------------------------------------
ITEM_SERIAL* COwnServer::GetNewSerial()
{
	m_pSerial->ItemSerialStruct.dwSerialNum++;
	return m_pSerial;
}


BOOL COwnServer::ResetItemSerialNumber()
{
	char szQuery[ 0xff ] = {0,};
	int nRet = 0;

	wsprintf(szQuery, "Select ServerSerialID from ServerTable Where Port = %d", m_wPortForUser);
	
	BYTE bSerialID = 0;
	nRet = g_pDb->OpenRecord(szQuery, &bSerialID, 1, GAME_DB );

	if(nRet < 0)
	{
		Log(LOG_IMPORTANT, "Fail to Query ServerSerialID!(Port=%d)", m_wPortForUser);
		return FALSE;
	}

	wsprintf(szQuery, "CZP_RESET_ITEM_SERIAL %d",  m_wPortForUser );
	int nRestartCount = -1;
	nRet = g_pDb->OpenRecord(szQuery, &nRestartCount, 1, GAME_DB );

	if(nRet < 0)
	{
		Log(LOG_IMPORTANT, "Fail to Reset ItemSerialCount!(Port=%d)", m_wPortForUser);
		return FALSE;
	}

	m_pSerial->ItemSerialStruct.bServerSet		= (BYTE)m_dwServerSetNum;
	m_pSerial->ItemSerialStruct.bServerSerialID	= bSerialID;
	m_pSerial->ItemSerialStruct.wRestartCount	= (WORD)nRestartCount;
	m_pSerial->ItemSerialStruct.dwSerialNum		= 0;
	
	Log(LOG_JUST_DISPLAY, "@ ItemSerialCount Reset Successfully! (RestartCount = %d)", nRestartCount);

	return TRUE;
}
