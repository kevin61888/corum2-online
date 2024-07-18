#ifndef __SERVER_TABLE_H__
#define __SERVER_TABLE_H__

#pragma once


#include "LinkedListEx.h"

struct SERVER_DATA
{
	BYTE			bServerType;		
	DWORD			dwID;							//Server ID(Port번호)
	DWORD			dwConnectionIndex;	
	DWORD			dwServerStatus;	
	POSITION_		ListPos;						//LinkedList에서의 노드 위치값 
	DWORD			dwIPForUser;
	char			szIPForServer[MAX_IP_LENGTH];	//IP For Server
	WORD			wPort;							//포트
	BYTE			bSended;						//Server Alive Check Packet 보냈는지의 Flag
	BYTE			bReceived;						//Server Alive Check Packet 응답을 받았는지의 Flag
	SERVER_DATA*	pNextData;
	SERVER_DATA*	pPrevData;
};


class CServerTable
{
	DWORD					m_dwMaxBucketNum;	
	DWORD					m_dwNum;		
	SERVER_DATA**			m_ppInfoTable;
	SERVER_DATA*			m_pLoginAgent;		
	CServerList				m_ServerList[MAX_SERVER_TYPE];	// ServerType - 1의 인덱스가 해당 ServerType의 리스트를 가르킴 
		
	void					RemoveServerTable();
		
public:								  
	
	SERVER_DATA*			AllocNewServer(DWORD dwID);
	SERVER_DATA*			GetServerInfo(DWORD dwId);
	SERVER_DATA*			SetLoginAgentServer(DWORD dwConnectionIndex);
	SERVER_DATA*			GetLoginAgentServer()		{	return m_pLoginAgent;	}
	BOOL					Add(SERVER_DATA* pServer);
	void					Remove(DWORD dwConnectionIndex);
	void					Remove(SERVER_DATA* pInfo);
	void					BroadCast(char* pPacket, DWORD dwLength, BYTE bServerType = 0);		// bServerType을 지정해줄 경우 그 서버종류에만 Broadcast
	void					SendAgent(char* pPacket, DWORD dwLength);	
	DWORD					GetCount()	{	return m_dwNum;	}
	void					ShowAllServerStatus();	
	void					BroadCastAlivePacket();
	void					CheckAlivePacket();

	CServerTable(DWORD dwNum);
	~CServerTable();
};


extern CServerTable* g_pServerTable;


#endif 