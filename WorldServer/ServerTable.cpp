#include "stdafx.h"
#include "ServerTable.h"


CServerTable* g_pServerTable = NULL;


CServerTable::CServerTable(DWORD dwNum)
{
	m_dwMaxBucketNum = dwNum;

	m_ppInfoTable = NULL;
	m_pLoginAgent = NULL; 
	m_ppInfoTable = new SERVER_DATA*[ dwNum ];		
	memset(m_ppInfoTable, 0, sizeof(SERVER_DATA*)*dwNum);

	m_dwNum = 0;
}

CServerTable::~CServerTable()
{	
	RemoveServerTable();

	if (m_ppInfoTable)
	{
		delete [] m_ppInfoTable;
		m_ppInfoTable = NULL;
	}
}

SERVER_DATA* CServerTable::AllocNewServer(DWORD dwID)
{
	SERVER_DATA* pServer = new SERVER_DATA;
	memset(pServer, 0, sizeof(SERVER_DATA));

	pServer->dwID = dwID;

	return pServer;
}

BOOL CServerTable::Add(SERVER_DATA* pServer)
{
	if(!pServer) return FALSE;

	if(pServer->bServerType >= MAX_SERVER_TYPE)	
	{
		Log(LOG_IMPORTANT, "Invalid ServerType Entered at CServerTable::Add(SERVER_DATA* pServer)! (pServer->bServerType = %d)", pServer->bServerType);
		return FALSE;
	}		

	pServer->ListPos = m_ServerList[ pServer->bServerType ].AddTail(pServer);	

	DWORD dwIndex = pServer->dwID % m_dwMaxBucketNum;		
	m_dwNum++;

	// m_ppInfoTable[dwIndex] 에 내용이 없으면 괄호 안으로.
	if (!m_ppInfoTable[dwIndex])	
	{
		m_ppInfoTable[dwIndex] = pServer;
		pServer->pNextData = NULL;
		pServer->pPrevData = NULL;
		return TRUE;
	}

	SERVER_DATA* cur = m_ppInfoTable[dwIndex];
	SERVER_DATA* prv = NULL;
	while (cur)
	{
		prv = cur;
		cur = cur->pNextData;
	}
	
	cur = prv->pNextData = pServer;
	cur->pPrevData = prv;
	cur->pNextData = NULL;

	return TRUE;
}

SERVER_DATA* CServerTable::GetServerInfo(DWORD dwID)
{
	DWORD dwIndex = dwID % m_dwMaxBucketNum;

	SERVER_DATA* cur = m_ppInfoTable[dwIndex];

	while (cur)
	{
		if (cur->dwID == dwID)
		{
			return cur;
		}
		cur = cur->pNextData;
	}

	return NULL;
}

void CServerTable::Remove(DWORD dwConnectionIndex)
{
	SERVER_DATA* pInfo = (SERVER_DATA*)g_pNet->GetServerInfo(dwConnectionIndex);

	DWORD dwIndex = pInfo->dwID%m_dwMaxBucketNum;

	if(pInfo->bServerType >= MAX_SERVER_TYPE)	
	{
		Log(LOG_IMPORTANT, "Invalid ServerType Entered at CServerTable::Remove(DWORD dwConnectionIndex)! (pInfo->bServerType = %d)", pInfo->bServerType);
		return;
	}
	
	m_ServerList[ pInfo->bServerType-1 ].RemoveAt( pInfo->ListPos );

	SERVER_DATA* cur = m_ppInfoTable[dwIndex];
	SERVER_DATA* next = NULL;
	SERVER_DATA* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrevData;
		next = cur->pNextData;

		if (cur->dwID == pInfo->dwID)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNextData = next;
			
			if (next)
				next->pPrevData = prv;

			if(cur)	
				delete cur;
						
			cur = NULL;
			m_dwNum--;

			return;
		}
		cur = cur->pNextData;
	}
}

void CServerTable::Remove(SERVER_DATA* pInfo)
{
	if(pInfo->bServerType >= MAX_SERVER_TYPE)		
	{
		Log(LOG_IMPORTANT, "Invalid ServerType Entered at CServerTable::Remove(SERVER_DATA* pInfo)! (pInfo->bServerType = %d)", pInfo->bServerType);
		return;
	}
	
	m_ServerList[ pInfo->bServerType ].RemoveAt( pInfo->ListPos );	

	DWORD dwIndex = pInfo->dwID%m_dwMaxBucketNum;

	SERVER_DATA* cur = m_ppInfoTable[dwIndex];
	SERVER_DATA* next = NULL;
	SERVER_DATA* prv = NULL;
	
	while (cur)
	{
		prv = cur->pPrevData;
		next = cur->pNextData;

		if (cur->dwID == pInfo->dwID)
		{
			if (!prv)	// if head
				m_ppInfoTable[dwIndex] = next;	
			else 
				prv->pNextData = next;
			
			if (next)
				next->pPrevData = prv;

			if(cur)	
				delete cur;
						
			cur = NULL;
			m_dwNum--;

			return;
		}

		cur = cur->pNextData;
	}
}

void CServerTable::RemoveServerTable()
{
	for(int k=0; k<MAX_SERVER_TYPE; k++)
		m_ServerList[ k ].RemoveAll();
		
	SERVER_DATA* cur = NULL;
	SERVER_DATA* next = NULL;

	for (DWORD i=0; i<m_dwMaxBucketNum; i++)
	{
		cur = m_ppInfoTable[i];
		while (cur)
		{
			next = cur->pNextData;

			delete cur;
			cur = next;
		}
		m_ppInfoTable[i] = NULL;
	}
}

void CServerTable::BroadCast(char* pPacket, DWORD dwLength, BYTE bServerType)
{
	SERVER_DATA* pServer = NULL;

	if(bServerType == 0)
	{
		// LoginAgent를 제외하고 BroadCast  
		for(int i = SERVER_TYPE_DUNGEON; i < MAX_SERVER_TYPE; i++)	
		{
			POSITION_ pos = m_ServerList[i].GetHeadPosition();

			while(pos)
			{
				pServer = (SERVER_DATA*)m_ServerList[i].GetNext(pos);
				
				if (NULL == pServer)
				{
					Log(LOG_IMPORTANT, "pServer is NULL at CServerTable::BroadCast() method.");					
				}
				else
				{
					g_pNet->SendToServer(pServer->dwConnectionIndex, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
				}			
			}
		}
	}
	else
	{
		POSITION_ pos = m_ServerList[ bServerType ].GetHeadPosition();	

		while(pos)
		{
			pServer = (SERVER_DATA*)m_ServerList[ bServerType  ].GetNext(pos);	
			if(!pServer)
			{
				Log(LOG_IMPORTANT, "pServer is NULL!! in CServerTable::BroadCast()");
				continue;
			}

			g_pNet->SendToServer(pServer->dwConnectionIndex, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
		}
	}
}


void CServerTable::BroadCastAlivePacket()
{
	SERVER_DATA* pServer = NULL;

	// LoginAgent를 제외하고 BroadCast  
	for(int i = SERVER_TYPE_DUNGEON; i < MAX_SERVER_TYPE; i++)	
	{
		POSITION_ pos = m_ServerList[i].GetHeadPosition();

		while(pos)
		{
			pServer = (SERVER_DATA*)m_ServerList[i].GetNext(pos);
			
			if (NULL == pServer)
			{
				Log(LOG_IMPORTANT, "pServer is NULL at CServerTable::BroadCastAlivePacket() method.");					
			}
			else
			{				
				pServer->bSended = 1;
			}			
		}
	}
}

void CServerTable::CheckAlivePacket()
{
	SERVER_DATA* pServer = NULL;

	// LoginAgent를 제외하고 BroadCast
	for(int i = SERVER_TYPE_DUNGEON; i < MAX_SERVER_TYPE; i++)	  
	{
		POSITION_ pos = m_ServerList[i].GetHeadPosition();

		while(pos)
		{
			pServer = (SERVER_DATA*)m_ServerList[i].GetNext(pos);
			
			if (NULL == pServer)
			{
				Log(LOG_IMPORTANT, "pServer is NULL at CServerTable::BroadCastAlivePacket() method.");					
			}
			else
			{
				// 응답 메세지 못받은 서버..  Down 간주!!
				if(pServer->bSended && !pServer->bReceived)		
				{
					Log( LOG_NORMAL, "								   ");
					Log( LOG_NORMAL, "Emergency!!!!! Down Server!!!! : %d", pServer->wPort);
					Log( LOG_NORMAL, "								   ");
				}
			}
		}
	}
}


SERVER_DATA* CServerTable::SetLoginAgentServer(DWORD dwConnectionIndex)
{
	SERVER_DATA* pServer = new SERVER_DATA;
	memset(pServer, 0, sizeof(SERVER_DATA));

	pServer->dwID				= 0;		// LoginAgent의 경우 ID가 없으므로 0으로 셋팅하자 
	pServer->bServerType		= SERVER_TYPE_LOGINAGENT;
	pServer->dwServerStatus		= SERVER_STATUS_CONNECTED;
	pServer->dwConnectionIndex	= dwConnectionIndex;
	g_pNet->GetServerAddress(dwConnectionIndex, pServer->szIPForServer, &pServer->wPort);
	m_pLoginAgent				= pServer;
	
	Add(pServer);
	g_pNet->SetServerInfo(dwConnectionIndex, pServer);
		
	return pServer;
}


void CServerTable::SendAgent(char* pPacket, DWORD dwLength)
{
	if(!m_pLoginAgent)	
	{
		Log(LOG_IMPORTANT, "SendAgent Fail! LoginAgent is not attched!");
		return;
	}

	g_pNet->SendToServer(m_pLoginAgent->dwConnectionIndex, pPacket, dwLength, FLAG_SEND_NOT_ENCRYPTION);
}


void CServerTable::ShowAllServerStatus()
{
	Log( LOG_JUST_DISPLAY, "								    				              " );
	Log( LOG_JUST_DISPLAY, "------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "         World Server - Dungeon Server Connection Status          " );
	Log( LOG_JUST_DISPLAY, "------------------------------------------------------------------" );
	Log( LOG_JUST_DISPLAY, "_No____ Type ________ IP _________ ID _ Status _____ Warning _____" );
						 //"[ 1]  11111111122222 / 111111111222222 / 11111 /  (11)"		
	POSITION_		pos			= 0;
	SERVER_DATA*	pServer		= 0;
	char*			szType[4]	= {"AGENT", "WORLD", "DUNGEON", "VILLAGE"};	
	char*			szStatus[2] = {"X", "O"};

	int nCount = 0;

	for(int i = 0; i < MAX_SERVER_TYPE ; i++)
	{
		pos = m_ServerList[i].GetHeadPosition();

		while(pos)
		{
			nCount++;
			pServer = (SERVER_DATA*)m_ServerList[i].GetNext(pos);
			if(pServer->dwServerStatus == SERVER_STATUS_NOT_CONNECTED)
			{
				Log( LOG_JUST_DISPLAY, "[%2d] %7s / %15s / %3d /  (%s)   <<Disconnected!!>>"     
					, nCount , szType[ pServer->bServerType ]
					, pServer->szIPForServer , pServer->dwID, szStatus[ pServer->dwServerStatus ]); 	
			}
			else
			{
				Log( LOG_JUST_DISPLAY, "[%2d] %7s / %15s / %3d /  (%s)"      
					, nCount , szType[ pServer->bServerType ], pServer->szIPForServer
					, pServer->dwID, szStatus[ pServer->dwServerStatus ]); 
			}
		}
	}

	Log( LOG_JUST_DISPLAY, "																  " );
}


