#include "stdafx.h"
#include "NetworkClient.h"
#include "ClientTrafficLog.h"


#ifdef __TRAFFIC_LOG

#pragma pack(push, 1)
struct TRAFFIC_TYPE_STRUCT
{
	BYTE	bGameStatus;
	BYTE	bHeader;
};

union TRAFFIC_TYPE
{
	TRAFFIC_TYPE_STRUCT TrafficType;
	unsigned short		usTrafficType;
};

struct TRAFFIC_NODE
{
	TRAFFIC_TYPE		TrafficType;
	WORD				wdCount;
	unsigned int		nTraffic;
};

#pragma pack(pop)

TRAFFIC_NODE g_SendTrafficNode[0xffff];
TRAFFIC_NODE g_RecvTrafficNode[0xffff];


static void WriteSendTrafficLog()
{
	FILE* pf=0;
	DWORD dwTotalCount=0;
	unsigned __int64 ulTotalTraffic=0;

	pf = fopen("SendTrafficLog.txt", "wt");

	for(int i = 0; i < 0xffff; i++)
	{
		if(g_SendTrafficNode[i].wdCount)
		{
			char szMsg[0xff]={0,};
			TRAFFIC_TYPE Type;
			Type.usTrafficType = (unsigned short)i;

			wsprintf(szMsg, "Status: %3u, Header: %3u, Count: %10u, Traffic: %20I64u Bytes\n", 
				Type.TrafficType.bGameStatus, Type.TrafficType.bHeader, g_SendTrafficNode[i].wdCount, g_SendTrafficNode[i].nTraffic);

			dwTotalCount += g_SendTrafficNode[i].wdCount;
			ulTotalTraffic += g_SendTrafficNode[i].nTraffic;

			fwrite(szMsg, lstrlen(szMsg), 1, pf);			
		}
	}

	char szLine[] = "----------------------------------------------------------------------------------";
	fwrite(szLine, lstrlen(szLine), 1, pf);

	char szTemp[0xff]={0,};
	wsprintf(szTemp, "Total %10u Packets, %20I64u Bytes Sent to Server!!", dwTotalCount, ulTotalTraffic);
	fwrite(szTemp, lstrlen(szTemp), 1, pf);

	fclose(pf);

	// init traffic info array!
	memset(g_SendTrafficNode, 0, sizeof(g_SendTrafficNode));
}

static void WriteRecvTrafficLog()
{
	FILE* pf=0;
	DWORD dwTotalCount=0;
	unsigned __int64 ulTotalTraffic=0;

	pf = fopen("RecvTrafficLog.txt", "wt");

	for(int i = 0; i < 0xffff; i++)
	{
		if(g_RecvTrafficNode[i].wdCount)
		{
			char szMsg[0xff]={0,};
			TRAFFIC_TYPE Type;
			Type.usTrafficType = (unsigned short)i;

			wsprintf(szMsg, "Status: %3u, Header: %3u, Count: %10u, Traffic: %20I64u Bytes\n", 
				Type.TrafficType.bGameStatus, Type.TrafficType.bHeader, g_RecvTrafficNode[i].wdCount, g_RecvTrafficNode[i].nTraffic);

			dwTotalCount += g_RecvTrafficNode[i].wdCount;
			ulTotalTraffic += g_RecvTrafficNode[i].nTraffic;

			fwrite(szMsg, lstrlen(szMsg), 1, pf);			
		}
	}

	char szLine[] = "----------------------------------------------------------------------------------";
	fwrite(szLine, lstrlen(szLine), 1, pf);

	char szTemp[0xff]={0,};
	wsprintf(szTemp, "Total %10u Packets, %20I64u Bytes Received from Server!!", dwTotalCount, ulTotalTraffic);
	fwrite(szTemp, lstrlen(szTemp), 1, pf);

	fclose(pf);

	// init traffic info array!
	memset(g_RecvTrafficNode, 0, sizeof(g_RecvTrafficNode));
}

void SendMsgLog(char* msg, DWORD length)
{
	static BOOL bFirst = TRUE;
	static DWORD dwTickPre = 0;

	if(bFirst)
	{
		memset(g_SendTrafficNode, 0, sizeof(g_SendTrafficNode));
		dwTickPre = timeGetTime();
		bFirst = FALSE;
	}
	else
	{
		DWORD dwTick = timeGetTime();
		if(dwTick - dwTickPre > 60*1000) // 1분마다 갱신!!
		{
			dwTickPre = dwTick;
			WriteSendTrafficLog();
		}
	}

	// get packet type!
	TRAFFIC_TYPE Type;
	Type.TrafficType.bGameStatus = msg[0];
	Type.TrafficType.bHeader	 = msg[1];

	// increase packet count & length
	g_SendTrafficNode[Type.usTrafficType].wdCount++;
	g_SendTrafficNode[Type.usTrafficType].nTraffic += length;
}

void RecvMsgLog(char* msg, DWORD length)
{
	static BOOL bFirst = TRUE;
	static DWORD dwTickPre = 0;

	if(bFirst)
	{
		memset(g_RecvTrafficNode, 0, sizeof(g_RecvTrafficNode));
		dwTickPre = timeGetTime();
		bFirst = FALSE;
	}
	else
	{
		DWORD dwTick = timeGetTime();
		if(dwTick - dwTickPre > 60*1000) // 1분마다 갱신!!
		{
			dwTickPre = dwTick;
			WriteRecvTrafficLog();
		}
	}

	// get packet type!
	TRAFFIC_TYPE Type;
	Type.TrafficType.bGameStatus = msg[0];
	Type.TrafficType.bHeader	 = msg[1];

	// increase packet count & length
	g_RecvTrafficNode[Type.usTrafficType].wdCount++;
	g_RecvTrafficNode[Type.usTrafficType].nTraffic += length;
}

#endif // __TRAFFIC_LOG