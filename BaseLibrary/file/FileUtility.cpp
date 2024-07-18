#include "../StdAfx.h"
#include "FileUtility.h"
#include <BaseLibrary/Utility/String/PathString.h>

HANDLE AdvCreateFile( LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile )
{
	TCHAR szPath[MAX_PATH] = "";

	if( (dwCreationDisposition == CREATE_NEW) || (dwCreationDisposition == CREATE_ALWAYS) || (dwCreationDisposition == OPEN_ALWAYS) ) {
		AdvSplitPath( (TCHAR *) lpFileName, szPath, (TCHAR *) NULL );
		if( *szPath )
			CreateDirectory( szPath, NULL );
	}
	return CreateFile( lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
}

