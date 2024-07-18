#ifndef __REPORT_CMLISTENER_H__
#define __REPORT_CMLISTENER_H__


#pragma once


#include "CommonHeader.h"


struct MACHINENAME
{
	char	szBuffer[MAX_COMPUTERNAME_LENGTH+1];
	DWORD	dwSize;
	WORD	wPort;
	BYTE	bServerType;
	char	szIpForUser[16];	

	MACHINENAME()
	{
		memset(szBuffer,0,sizeof(szBuffer));
		dwSize		= MAX_COMPUTERNAME_LENGTH+1;
		wPort		= 0; 
		bServerType = 0;
		memset(szIpForUser,0,sizeof(szIpForUser));
	}
};


void DECLSPECIFIER ReportToCMListener(DWORD dwType, MACHINENAME* pMachineName);


#endif 
