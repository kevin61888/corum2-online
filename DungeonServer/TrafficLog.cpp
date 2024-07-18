#include "stdafx.h"
#include "TrafficLog.h"


struct TRAFFIC_NODE
{
	DWORD	dwCount;
	unsigned __int64 ulTraffic;
};


TRAFFIC_NODE g_TrafficNode[256];

void TrafficLog()
{
	FILE* pf;

	pf = fopen("TrafficLog.txt", "wt");

	for(int i = 0; i < 256; i++)
	{
		if(g_TrafficNode[i].dwCount)
		{
			char szMsg[255]={0,};

			wsprintf(szMsg, "Header: %3u, Count: %10u, Traffic: %20I64u Bytes\n", 
				i, g_TrafficNode[i].dwCount, g_TrafficNode[i].ulTraffic);

			fwrite(szMsg, strlen(szMsg), 1, pf);			
		}
	}

	fclose(pf);
}

BOOL SendToUserWithLog(DWORD dwConnectionIndex,char* msg,DWORD length,DWORD flag)
{
	static BOOL bFirst = TRUE;
	static DWORD dwTickPre = 0;

	if(bFirst)
	{
		memset(g_TrafficNode, 0, sizeof(TRAFFIC_NODE)*256);
		dwTickPre = timeGetTime();
		bFirst = FALSE;
	}
	else
	{
		DWORD dwTick = timeGetTime();
		if(dwTick - dwTickPre > 60*1000*5) // 5분마다 갱신!!
		{
			dwTickPre = dwTick;
			TrafficLog();
		}
	}

	if(BYTE(msg[0]) == UPDATE_GAME_PLAY)
	{
		BYTE	bHeader = BYTE(msg[1]);

		g_TrafficNode[bHeader].dwCount++;
		g_TrafficNode[bHeader].ulTraffic += length;
	}	

	return g_pNet->SendToUser(dwConnectionIndex, msg, length, flag);
}