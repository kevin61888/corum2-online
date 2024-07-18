//
// File: CServerINIFile.h
// Desc: INI file class
//
// Auth: Standil (cpascal@nownuri.net)
//

#include "../../StdAFX.h"
#include "ServerINIFile.h"
#include <io.h>

// TODO - CServerINIFile::Write 계열 작성

CServerINIFile::CServerINIFile()
{
	Init();
}

CServerINIFile::~CServerINIFile()
{
}

CServerINIFile::CServerINIFile( LPCSTR szFilename, UINT nOpenFlags )
{
	Init();
	Open( szFilename, nOpenFlags );
}

BOOL CServerINIFile::Init( void )
{
	ZeroMemory( m_szFilename, MAX_PATH );
	ZeroMemory( m_szSection, MAX_PATH );
	m_uFlag = 0;
	return TRUE;
}

BOOL CServerINIFile::Open( LPCSTR szFilename, UINT nOpenFlags )
{
	if ( _access( szFilename, 0 ) == 0 ) {
		_tcscpy( m_szFilename, ".\\" );
		_tcscat( m_szFilename, szFilename );
	} else {
		_tcscpy( m_szFilename, szFilename );
	}
	m_uFlag = nOpenFlags;
	return TRUE;
}

void CServerINIFile::Close( void )
{
}

BOOL CServerINIFile::ReadLine( const TCHAR *szSection )
{
	if ( !*m_szFilename )
		return FALSE;
	_tcscpy( m_szSection, szSection );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, double &dNumber )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		dNumber = 0.0;
		return FALSE;
	}
	dNumber = _tstof( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, float &fNumber )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		fNumber = 0.0f;
		return FALSE;
	}
	fNumber = (float) _tstof( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, unsigned long &lNumber )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		lNumber = 0;
		return FALSE;
	}
	lNumber = _ttol( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, int &iNumber )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		iNumber = 0;
		return FALSE;
	}
	iNumber = _ttoi( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, unsigned int &iNumber )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		iNumber = 0;
		return FALSE;
	}
	iNumber = _ttoi( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, unsigned short &iNumber )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		iNumber = 0;
		return FALSE;
	}
	iNumber = _ttoi( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, short &iNumber)
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		iNumber = 0;
		return FALSE;
	}
	iNumber = _ttoi( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, unsigned char &iNumber )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		iNumber = 0;
		return FALSE;
	}
	iNumber = _ttoi( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, char &iNumber )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		iNumber = 0;
		return FALSE;
	}
	iNumber = _ttoi( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, __int64 &i64Number )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		i64Number = 0;
		return FALSE;
	}
	i64Number = _ttoi64( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szField, DWORD64 &i64Number )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), m_szLocal, INIFILE_BUFFER_LEN, m_szFilename );
	if ( !*m_szLocal ) {
		i64Number = 0;
		return FALSE;
	}
	i64Number = _ttoi64( m_szLocal );
	return TRUE;
}

BOOL CServerINIFile::ReadString( const TCHAR *szField, TCHAR *szString, DWORD dwSize )
{
	if ( !*m_szFilename || !*m_szSection )
		return FALSE;
	if ( !szField || !*szField )
		return FALSE;
	GetPrivateProfileString( m_szSection, szField, _T( "" ), szString, dwSize, m_szFilename );
	return TRUE;
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, double &dNumber )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, dNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, float &fNumber )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, fNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, unsigned long &lNumber )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, lNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, int &iNumber )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, iNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, unsigned int &iNumber )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, iNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, unsigned short &iNumber )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, iNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, short &iNumber)
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, iNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, unsigned char &iNumber )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, iNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, char &iNumber )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, iNumber );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, __int64 &i64Number )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, i64Number );
}

BOOL CServerINIFile::ReadData( const TCHAR *szSection, const TCHAR *szField, DWORD64 &i64Number )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadData( szField, i64Number );
}

BOOL CServerINIFile::ReadString( const TCHAR *szSection, const TCHAR *szField, TCHAR *szString, DWORD dwSize )
{
	if ( ReadLine( szSection ) == FALSE )
		return FALSE;
	return ReadString( szField, szString, dwSize );
}

