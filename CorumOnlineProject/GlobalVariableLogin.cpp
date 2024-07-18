#include "GlobalVariableLogin.h"
#include "GameControl.h"
#include "LoginProcess.h"

DWORD GlobalVariableLogin::GetTotalSetNumber()
{
	return ServerSetInfo.dwTotalSetNum;
}

DWORD GlobalVariableLogin::GetSelectedSet()
{
	return ServerSetInfo.dwCurMouseOverSet;
}

SERVER_SET_INFO* GlobalVariableLogin::GetSetInfo(DWORD dwIndex)
{
	return &(ServerSetInfo.pServerInfo[dwIndex]);
}

SERVER_SET_INFO* GlobalVariableLogin::GetSelectedSetInfo()
{
	return GetSetInfo(GetSelectedSet());
}

void GlobalVariableLogin::SetCurSet(DWORD dwIndex)
{
	ServerSetInfo.dwCurMouseOverSet = dwIndex;
}

#if IS_CHINA_LOCALIZING()
BOOL GlobalVariableLogin::LoadConnectionInfo()
{
	int		iRegionID = -1;
	LPCSTR* ppArgv = (LPCSTR*) __argv;
	int		nArgc = __argc;	

	for(int i = 1; i < nArgc; i++)
	{
		if(	(_strnicmp(ppArgv[i], "-region:", 8) == 0) ||
			(_strnicmp(ppArgv[i], "/region:", 8) == 0) )
		{
			iRegionID = atoi(ppArgv[i] + lstrlen("/region:"));
			break;
		}
	}

	// set default region if failed.
	if(iRegionID == -1) iRegionID = 1;

	// load ini file information...
	char szFile[ 0xff ] = {0,};
	wsprintf(szFile, "%s\\Connect.ini", g_szBasePath);

	char szAppName[0xff]= {0,};
	wsprintf(szAppName, "REGION_%02d", iRegionID);

	g_pGVLogin->ServerSetInfo.dwTotalSetNum = GetPrivateProfileInt(szAppName, "totalsetnum", 0, szFile);
	if(!g_pGVLogin->ServerSetInfo.dwTotalSetNum)
		return FALSE;

	if(!g_pGVLogin->ServerSetInfo.pServerInfo)
		g_pGVLogin->ServerSetInfo.pServerInfo = new SERVER_SET_INFO[ g_pGVLogin->ServerSetInfo.dwTotalSetNum ];

	if(!g_pGVLogin->ServerSetInfo.pServerInfo) return FALSE;
	
	char szTemp[ 0xff ] = {0,};
	for(i=0; i<g_pGVLogin->ServerSetInfo.dwTotalSetNum; i++)
	{
		wsprintf(szTemp, "set_%02d_name", i+1 );
		GetPrivateProfileString(szAppName,  szTemp, "", 
			g_pGVLogin->ServerSetInfo.pServerInfo[i].szSetName, MAX_SERVER_SET_NAME_LENGTH, szFile);
		wsprintf(szTemp, "set_%02d_address", i+1 );
		GetPrivateProfileString(szAppName,  szTemp, "211.174.52.109", 
			g_pGVLogin->ServerSetInfo.pServerInfo[i].szIp, MAX_IP_LENGTH, szFile);
	}
	
	g_pGVLogin->ServerSetInfo.dwCurMouseOverSet = 0xffffffff;	

	return TRUE;
}
#else
BOOL GlobalVariableLogin::LoadConnectionInfo()
{
	char szFile[ 0xff ] = {0,};
	wsprintf(szFile, "%s\\Connect.ini", g_szBasePath);	 	
	g_pGVLogin->ServerSetInfo.dwTotalSetNum = GetPrivateProfileInt("SERVER_SET_INFO", "totalsetnum", 0, szFile);
	if(!g_pGVLogin->ServerSetInfo.dwTotalSetNum)
		return FALSE;

	if(!g_pGVLogin->ServerSetInfo.pServerInfo)
		g_pGVLogin->ServerSetInfo.pServerInfo = new SERVER_SET_INFO[ g_pGVLogin->ServerSetInfo.dwTotalSetNum ];

	if(!g_pGVLogin->ServerSetInfo.pServerInfo) return FALSE;
	
	char szTemp[ 0xff ] = {0,};

	for(DWORD i=0; i<g_pGVLogin->ServerSetInfo.dwTotalSetNum; i++)
	{
		wsprintf(szTemp, "set_%02d_name", i+1 );
		GetPrivateProfileString("SERVER_SET_INFO",  szTemp, "", 
			g_pGVLogin->ServerSetInfo.pServerInfo[i].szSetName, MAX_SERVER_SET_NAME_LENGTH, szFile);
		wsprintf(szTemp, "set_%02d_address", i+1 );
		GetPrivateProfileString("SERVER_SET_INFO",  szTemp, "211.174.52.109", 
			g_pGVLogin->ServerSetInfo.pServerInfo[i].szIp, MAX_IP_LENGTH, szFile);
	}
	
	g_pGVLogin->ServerSetInfo.dwCurMouseOverSet = 0xffffffff;
	
	return TRUE;
}
#endif

void GlobalVariableLogin::ReleaseConnectionInfo()
{
	if(g_pGVLogin->ServerSetInfo.pServerInfo)
	{
		delete [] g_pGVLogin->ServerSetInfo.pServerInfo;
		g_pGVLogin->ServerSetInfo.pServerInfo = NULL;
	}	
}



