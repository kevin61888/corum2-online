#include "../../StdAfx.h"
#include <lmerr.h>
#include "DebugUtils.h"


DbgUtils::CConvertErrorToText	g_ConvertErrorToText;


// Set application-name for prefix of log filename
void DbgUtils::SetProgramName(PTSTR pszOutBuffer, int nBufferSize, PTSTR pszProgramName)
{
	if (0 == pszProgramName) {
		TCHAR szDrive[MAX_PATH], szDir[MAX_PATH], szFilename[MAX_PATH], szExt[MAX_PATH];

		// Figure out what the report file will be named, and store it away
	    GetModuleFileName(0, pszOutBuffer, nBufferSize);

		PTSTR pszDot = pszOutBuffer;
	    // Look for the '.' before the "EXE" extension. Replace '.' to '\0'
		if ((pszDot = _tcsrchr(pszDot, _T('.')))) {
			*pszDot = 0;
		}

		_tsplitpath(pszOutBuffer, szDrive, szDir, szFilename, szExt);
		_tcsncpy(pszOutBuffer, szFilename, nBufferSize);
	} else {
		_tcsncpy(pszOutBuffer, pszProgramName, nBufferSize);
	}
}

// constructor and destructor
DbgUtils::CConvertErrorToText::CConvertErrorToText(void) : m_hNetMsg(LoadLibraryEx(TEXT("netmsg.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE))
{
}

DbgUtils::CConvertErrorToText::~CConvertErrorToText(void)
{
	if (NULL != m_hNetMsg)
		FreeLibrary(m_hNetMsg);
}

DWORD DbgUtils::CConvertErrorToText::GetErrorTextBuffer(LPTSTR lpMessage, DWORD dwBufferLen, DWORD dwLastError)
{
	DWORD	dwFormatFlags = FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM;
	LPCVOID	pModule = 0;

	if (dwLastError >= NERR_BASE && dwLastError <= MAX_NERR && NULL != m_hNetMsg) {
		pModule = m_hNetMsg;
		dwFormatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
	}

	return FormatMessage(dwFormatFlags, pModule, dwLastError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), lpMessage, dwBufferLen, NULL);	
}
