#include "stdafx.h"
#include "ReportCMListener.h"


void DECLSPECIFIER ReportToCMListener(DWORD dwType, MACHINENAME* pMachineName)
{
	HWND hWnd = NULL;
	hWnd = ::FindWindow(NULL,"CMListener");
	
	if(hWnd)
	{
		COPYDATASTRUCT cds;
		cds.dwData = dwType;
		cds.cbData = sizeof(MACHINENAME);
		cds.lpData = pMachineName;
		::SendMessage(hWnd,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds);
	}
}