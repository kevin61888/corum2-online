#include "stdafx.h"
#include "WorldServerTable.h"


CWorldServerTable *g_pServerTable = NULL;


CWorldServerTable::CWorldServerTable()
{
	memset(this, 0, sizeof(CWorldServerTable));
}

CWorldServerTable::~CWorldServerTable()
{
	for(int i = 0; i < MAX_WORLD_SERVER_NUM + 1; i++)
	{
		if(m_pWorldServer[i])
		{
			delete m_pWorldServer[i];
			m_pWorldServer[i] = NULL;
		}
	}
}

LPWorldServerData CWorldServerTable::AllocNewServer(DWORD dwWorldMapID)
{
	if(m_pWorldServer[ dwWorldMapID ])
		return NULL;

	m_pWorldServer[ dwWorldMapID ] = new WORLD_SERVER_DATA;	
	memset(m_pWorldServer[ dwWorldMapID ], 0, sizeof(WORLD_SERVER_DATA));

	m_pWorldServer[ dwWorldMapID ]->dwWorldMapID = dwWorldMapID;
	m_dwCurServerCount++;
	return m_pWorldServer[ dwWorldMapID ];
}

BOOL CWorldServerTable::DeleteServer(DWORD dwWorldMapID)
{
	if(dwWorldMapID < 1 || dwWorldMapID > MAX_WORLD_SERVER_NUM)
		return FALSE;

	if(!m_pWorldServer[ dwWorldMapID ])
		return FALSE;

	delete m_pWorldServer[ dwWorldMapID ];
	m_pWorldServer[ dwWorldMapID ] = NULL;
	m_dwCurServerCount--;
	return TRUE;
}


LPWorldServerData CWorldServerTable::GetServer(DWORD dwWorldServerID)
{
	if(dwWorldServerID < 1 || dwWorldServerID > MAX_WORLD_SERVER_NUM)
		return NULL;

	return m_pWorldServer[ dwWorldServerID ];
}

void CWorldServerTable::BroadCastWorldServer(char* pPacket, DWORD dwLength)
{
	for(int i=1; i<MAX_WORLD_SERVER_NUM; i++)
	{
		if(m_pWorldServer[ i ])
		{
			g_pNet->SendToServer(m_pWorldServer[ i ]->dwConnectionIndex, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
		}
	}
}

BOOL CWorldServerTable::SendToWorldServer(char* pPacket, DWORD dwLength, DWORD dwWorldServerID)
{
	if(dwWorldServerID < 1 || dwWorldServerID > MAX_WORLD_SERVER_NUM)
		return FALSE;

	if(!m_pWorldServer[ dwWorldServerID ] )
		return FALSE;

	g_pNet->SendToServer(m_pWorldServer[ dwWorldServerID ]->dwConnectionIndex, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
	
	return TRUE;
}

void CWorldServerTable::ShowAllServerStatus()
{
	Log( LOG_JUST_DISPLAY, "								    				              " );
	Log( LOG_JUST_DISPLAY, "------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "          Agent Server - WorldServer Connection Status            " );
	Log( LOG_JUST_DISPLAY, "------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "_No____ Type ________ IP _________ ID _ Status _____ Warning _____" );
	
	WORLD_SERVER_DATA*	pServer;
	
	char* szStatus[2] = {"X", "O"};

	int nCount = 0;

	for(int i = 1; i < MAX_WORLD_SERVER_NUM + 1; i++)
	{
		nCount++;

		if(!m_pWorldServer[i])	continue;

		pServer = m_pWorldServer[i];
		if(pServer->dwServerStatus == SERVER_STATUS_NOT_CONNECTED)
			Log( LOG_JUST_DISPLAY, "[%2d] %7s / %15s / %3d /  (%s)   <<Disconnected!!>>"      
				, nCount , "WORLD", pServer->szIPForServer , pServer->dwWorldMapID, szStatus[ pServer->dwServerStatus ]); 	
		else
			Log( LOG_JUST_DISPLAY, "[%2d] %7s / %15s / %3d /  (%s)"      
				, nCount , "WORLD", pServer->szIPForServer , pServer->dwWorldMapID, szStatus[ pServer->dwServerStatus ]); 	
	}

	Log( LOG_JUST_DISPLAY, "																  " );
}




