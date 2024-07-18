#include "stdafx.h"
#include "UniqueProcess.h"


HANDLE g_hGlobalEvent = NULL;


int DECLSPECIFIER CreateGlobalKernelObject(DWORD dwServerID)
{
	char szObjectName[0xff]={0,};
	wsprintf(szObjectName, "Global\\CorumServer_%u", dwServerID);

	HANDLE g_hGlobalEvent = CreateEvent(NULL, FALSE, FALSE, szObjectName);
	
	if(!g_hGlobalEvent) return -1;						// 실패
	if(ERROR_ALREADY_EXISTS == GetLastError()) return 0;// 중복
	return 1;											// 성공
}


void DECLSPECIFIER DeleteGlobalKernelObject()
{
	if(g_hGlobalEvent)
	{
		CloseHandle(g_hGlobalEvent);
		g_hGlobalEvent = NULL;
	}
}