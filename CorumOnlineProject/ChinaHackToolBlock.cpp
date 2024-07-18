#include "stdafx.h"
#include "ChinaHackToolBlock.h"
#include "Develop.h"
#include "InitGame.h"
#include "DFXForm.h"

bool g_bFreeCorumFound = false;

BOOL CALLBACK EnumWindowsProcForFreeCorum(HWND hwnd, LPARAM lParam)
{
	int i = 0;

	// Â÷ÈÄ ÇÊ¿äÇÏ¸é ¿ÜºÎ datafile·Î ´ëÃ¼ÇÒ ¼öµµ ÀÖ´Ù!!
	char* szCorum[4] = 
	{ 
		"·¬ÇÑÖ®¿ÆÂ¡°æ",		// Áß¹® 1.0
		"·¬ÇÑ¿ÆÂ¡Ãâ·Ñ°æ",	// Áß¹® 1.2
		"ÛãÊµÎ¡×ÌØó?÷ú",	// ÇÑ±Û 1.0
		"ÛãÊµñýÎ¡×Ì÷ú"		// ÇÑ±Û 1.2
	};

	char szCaption[0xff] = {0,};
	::GetWindowText(hwnd, szCaption, sizeof(szCaption));

	DWORD dwLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC );

	for(i = 0; i < 4; i++)
	{
		LPSTR szTemp = (LPSTR)szCaption;

		for(;;)
		{
			if(lstrlen(szTemp) == 0) break;

			if(szTemp[0] != ' ')
			{
				int iRet = CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, 
					szCorum[i], lstrlen(szCorum[i]), 
					szTemp, lstrlen(szCorum[i]));

				if(CSTR_EQUAL == iRet) 
				{
					g_bFreeCorumFound = true;
					return FALSE;
				}

				iRet = CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, 
					szCorum[i], lstrlen(szCorum[i]), 
					szTemp, lstrlen(szCorum[i]));

				if(CSTR_EQUAL == iRet) 
				{
					g_bFreeCorumFound = true;
					return FALSE;
				}

				iRet = CompareString(dwLCID, NORM_IGNORECASE, 
					szCorum[i], lstrlen(szCorum[i]), 
					szTemp, lstrlen(szCorum[i]));

				if(CSTR_EQUAL == iRet) 
				{
					g_bFreeCorumFound = true;
					return FALSE;
				}

				if(0 == _tcsncmp(szCorum[i], szTemp, lstrlen(szCorum[i])))
				{
					g_bFreeCorumFound = true;
					return FALSE;
				}
			}

			szTemp = CharNextExA(0, szTemp, 0);
		}	
	}

	return TRUE;
}

#include "TlHelp32.h"

DWORD EnumParentProcess()
{
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapShot == INVALID_HANDLE_VALUE) return 0;

	PROCESSENTRY32 ProcessEntry;
	BOOL bOk = 0;
	DWORD pid = GetCurrentProcessId();

	bOk = Process32First(hSnapShot, &ProcessEntry);
	while(bOk)
	{
		bOk = Process32Next(hSnapShot, &ProcessEntry);
		if(bOk && ProcessEntry.th32ProcessID == 0) continue;

		// ºÎ¸ð 
		if(ProcessEntry.th32ProcessID == pid)
		{
			CloseHandle(hSnapShot);
			return ProcessEntry.th32ParentProcessID;
		}
	}

	CloseHandle(hSnapShot);
	return 0;
}

BOOL EnumProcesses()
{
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;

	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnapShot == INVALID_HANDLE_VALUE) return FALSE;

	PROCESSENTRY32 ProcessEntry;
	BOOL bOk = 0;
//	DWORD pid = GetCurrentProcessId();

	bOk = Process32First(hSnapShot, &ProcessEntry);
	while(bOk)
	{
		bOk = Process32Next(hSnapShot, &ProcessEntry);
		if(bOk && ProcessEntry.th32ProcessID == 0) continue;

		if(bOk)
		{
			int iRet = CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, 
					ProcessEntry.szExeFile, lstrlen(ProcessEntry.szExeFile), 
					"FreeCorum", lstrlen("FreeCorum"));

			if(CSTR_EQUAL == iRet) 
			{
				CloseHandle(hSnapShot);
				return TRUE;
			}
		}	
	}

	CloseHandle(hSnapShot);
	return FALSE;
}


BOOL IsHackToolUsed()
{

#ifdef __RELEASE_BUILD

	// ÀÏ´Ü ºÎ¸ð ÇÁ·Î¼¼½º°¡ ¾ÆÁ÷ »ì¾Æ ÀÖ´Ù¸é Á×ÀÎ´Ù!!
	DWORD dwParentProcessID = EnumParentProcess();
	HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, dwParentProcessID); 
	if(hProcess != NULL)
	{
		DWORD dwExitCode;
		if(GetExitCodeProcess(hProcess, &dwExitCode))
		{
			if(dwExitCode == STILL_ACTIVE)
			{
				TerminateProcess(hProcess, 0);
			}
		}

		CloseHandle(hProcess);
	}

#endif // __RELEASE_BUILD



	// check all window caption.
	EnumWindows(EnumWindowsProcForFreeCorum, 0);

	if(g_bFreeCorumFound || EnumProcesses())
	{
		MessageBox(g_hMainWnd, "HackTool Found!\nNow, CorumOnline exits...", "HackTool Detection", MB_OK|MB_ICONWARNING|MB_TOPMOST);
		return TRUE;
	}

	return  FALSE;
}

BOOL CheckGlobalEventHandle()
{
	// Check GKO!!
	////////////////////////////////////////////////////////////////////////////
	SYSTEMTIME time;
	GetLocalTime(&time);
	char szTemp[0xff]={0,};
	wsprintf(szTemp, "Fuck%02dYou%02dHacker%02d", time.wYear, time.wMonth, time.wDay);

	DFXform xform;
	UINT nSize = 0;
	char szBuf[0xff]={0,};
	xform.XXEncode((LPBYTE)szTemp, lstrlen(szTemp), (LPBYTE)szBuf, &nSize);

	char szEventName[0xff] = {0,};
	wsprintf(szEventName, "Global\\%s", szBuf);

	/*char szDate[0xff] = {0,};
	_strdate(szDate);

	for(int i = 0; i < 0xff; i++)
	{
		if(szDate[i] == NULL) break;
		if(szDate[i] == '/') szDate[i] = '_';
	}

	char szEventName[0xff] = {0,};
	wsprintf(szEventName, "Global\\FuckFreeCorum%s", szDate);
	*/

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, szEventName);
	if(hEvent == NULL) return FALSE;

	SetEvent(hEvent);
	CloseHandle(hEvent);

	return TRUE;
}

