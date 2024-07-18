#include "../../StdAfx.h"
#include "PathString.h"

void AdvSplitPath( register TCHAR *szFullPath, TCHAR *szPath, TCHAR *szFilename )
{
	if( !szFullPath ) {
//		LOGF( "%s: [ERROR] Got NULL argument!", __FUNCDNAME__ );
		return;
	}
	register TCHAR *p;
	TCHAR *pSlash = NULL;
	int iLen;

	for( p = szFullPath; *p; p++ ) {
		if( *p == _T('/') || *p == _T('\\') )
			pSlash = p + 1;
	}

	if( pSlash ) {
		if( szPath ) {
			iLen = (int) (pSlash - szFullPath) / sizeof( TCHAR );
			_tcsncpy( szPath, szFullPath, iLen );
			*(szPath + iLen) = _T('\0');
		}
		szFullPath = pSlash;
	} else {
		if( szPath )
			*szPath = _T('\0');
	}

	if( szFilename ) {
		iLen = (int) (p - szFullPath) / sizeof( TCHAR );
		_tcsncpy( szFilename, szFullPath, iLen );
		*(szFilename+ iLen) = _T('\0');
	}
}

void AdvSplitPath( register TCHAR *szFullPath, TCHAR *szPath, TCHAR *szFilename, TCHAR *szExtName )
{
	if( !szFullPath ) {
//		LOGF( "%s: [ERROR] Got NULL argument!", __FUNCDNAME__ );
		return;
	}
	register TCHAR *p;
	TCHAR *pSlash = NULL;
	TCHAR *pDot = NULL;
	int iLen;

	for( p = szFullPath; *p; p++ ) {
		if( *p == _T('/') || *p == _T('\\') )
			pSlash = p + 1;
		else if( *p == _T('.') )
			pDot = p;
	}

	if( pSlash ) {
		if( szPath ) {
			iLen = (int) (pSlash - szFullPath) / sizeof( TCHAR );
			_tcsncpy( szPath, szFullPath, iLen );
			*(szPath + iLen) = _T('\0');
		}
		szFullPath = pSlash;
	} else {
		if( szPath )
			*szPath = _T('\0');
	}

	if( pDot && (pDot >= szFullPath) ) {
		if( szFilename ) {
			iLen = (int) (pDot - szFullPath) / sizeof( TCHAR );
			_tcsncpy( szFilename, szFullPath, iLen );
			*(szFilename+ iLen) = _T('\0');
		}
		if( szExtName ) {
			pDot++;
			iLen = (int) (p - pDot) / sizeof( TCHAR );
			_tcsncpy( szExtName, pDot, iLen );
			*(szExtName + iLen) = _T('\0');
		}
	} else {
		if( szFilename ) {
			iLen = (int) (p - szFullPath) / sizeof( TCHAR );
			_tcsncpy( szFilename, szFullPath, iLen );
			*(szFilename+ iLen) = _T('\0');
		}
		if( szExtName )
			*szExtName = _T('\0');
	}
}

int AdvGetAppPath( LPTSTR lpszPath, DWORD dwLength )
{
	HMODULE hModule = ::GetModuleHandle( NULL );		// Get handle of current module
	TCHAR szExeFileName[_MAX_PATH];
	CHAR szPath[_MAX_PATH];

	::GetModuleFileName( hModule, szExeFileName, _MAX_PATH );
	AdvSplitPath( szExeFileName, szPath, NULL );

	_tcsncpy( lpszPath, szPath, dwLength );
	if ( _tcslen( szPath ) > dwLength )
		return dwLength;
	return _tcslen( lpszPath );
}
