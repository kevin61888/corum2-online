#ifndef __WORLD_SERVER_TABLE_H__
#define __WORLD_SERVER_TABLE_H__

#pragma once

#include "GlobalDefine.h"

typedef struct WORLD_SERVER_DATA
{
	DWORD			dwWorldMapID;			//WorldMapID
	DWORD			dwConnectionIndex;	
	DWORD			dwServerStatus;	

	DWORD			dwIPForServer;
	DWORD			dwIPForUser;
	char			szIPForServer[ MAX_IP_LENGTH ];	//IP For Server
	WORD			wPortForServer;		
	WORD			wPortForUser;

} *LPWorldServerData;


class CWorldServerTable
{
	LPWorldServerData	m_pWorldServer[ MAX_WORLD_SERVER_NUM + 1 ];

	DWORD				m_dwCurServerCount;			//현재 월드 서버 카운트 

public:

	LPWorldServerData	GetServer(DWORD dwWorldServerID);
	LPWorldServerData	AllocNewServer(DWORD dwWorldMapID);
	BOOL				DeleteServer(DWORD dwWorldMapID);
	BOOL				SendToWorldServer(char* pPacket, DWORD dwLength, DWORD dwWorldServerID);
	void				ShowAllServerStatus();
	void				BroadCastWorldServer(char* pPacket, DWORD dwLength);


	CWorldServerTable();
	~CWorldServerTable();	
};


extern CWorldServerTable *g_pServerTable;

#endif // __WORLD_SERVER_TABLE_H__