//
// File: CLowFile.cpp
// Desc: File class
//
// Auth: Standil (cpascal@nownuri.net)
//

#include "../StdAfx.h"
#include "LowFile.h"
#include "FileUtility.h"

CLowFile::CLowFile() : m_hFile( INVALID_HANDLE_VALUE )
{
	memset( m_szFilename, 0, sizeof( m_szFilename ) );
}

CLowFile::~CLowFile()
{
	Close();
}

int CLowFile::Size( void )
{
	return ::GetFileSize( m_hFile, NULL );
}

int CLowFile::Size( LPCSTR szFilename )
{
	DWORD dwFilesize;

	if( FALSE == Open( szFilename, modeRead ) )
		return -1;
	dwFilesize = Size();
	Close();
	return dwFilesize;
}

BOOL CLowFile::Open( const LPCSTR szFilename, UINT nOpenFlags )
{
	DWORD dwAccessMode = GENERIC_READ;
	DWORD dwCreation = OPEN_EXISTING;

	Close();
	switch( nOpenFlags ) {
	case modeRead :
		dwAccessMode = GENERIC_READ;
		dwCreation = OPEN_EXISTING;
		break;
	case modeWrite :
		dwAccessMode = GENERIC_WRITE;
		dwCreation = OPEN_ALWAYS;
		break;
	case modeCreate :
		dwAccessMode = GENERIC_WRITE;
		dwCreation = CREATE_ALWAYS;
		break;
	case modeAll :
		dwAccessMode = GENERIC_READ | GENERIC_WRITE;
		dwCreation = OPEN_ALWAYS;
		break;
	}
	strncpy( m_szFilename, szFilename, MAX_PATH );
	if( (m_hFile = AdvCreateFile( m_szFilename, dwAccessMode, 0, NULL, dwCreation, FILE_ATTRIBUTE_NORMAL, NULL )) == INVALID_HANDLE_VALUE )
		return FALSE;
	return TRUE;
}

BOOL CLowFile::Access( LPCSTR szFilename )
{
	HANDLE hAccessFile;

	if( (hAccessFile = AdvCreateFile( szFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL )) == INVALID_HANDLE_VALUE )
		return FALSE;
	CloseHandle( hAccessFile );
	return TRUE;
}

BOOL CLowFile::OpenReadf( LPCTSTR szFilename, ... )
{
	va_list	vaList;

	va_start( vaList, szFilename );
	_vsntprintf( m_szFilename, sizeof( m_szFilename ), szFilename, vaList );
	va_end( vaList );
	return Open( m_szFilename, modeRead );
}

BOOL CLowFile::OpenWritef( LPCTSTR szFilename, ... )
{
	va_list	vaList;

	va_start( vaList, szFilename );
	_vsntprintf( m_szFilename, sizeof( m_szFilename ), szFilename, vaList );
	va_end( vaList );
	return Open( m_szFilename, modeWrite );
}

BOOL CLowFile::OpenCreatef( LPCTSTR szFilename, ... )
{
	va_list	vaList;

	va_start( vaList, szFilename );
	_vsntprintf( m_szFilename, sizeof( m_szFilename ), szFilename, vaList );
	va_end( vaList );
	return Open( m_szFilename, modeCreate );
}

void CLowFile::Close( void )
{
	if( m_hFile != INVALID_HANDLE_VALUE ) {
		CloseHandle( m_hFile );
		m_hFile = INVALID_HANDLE_VALUE;
	}
}

int CLowFile::Read( LPVOID lpBuffer, DWORD dwBytesToRead )
{
	DWORD dwRead;

	if( ReadFile( m_hFile, lpBuffer, dwBytesToRead, &dwRead, NULL ) == FALSE )
		return -1;
	return (int) dwRead;
}

int CLowFile::Read( LPCSTR szFilename, LPVOID lpBuffer, DWORD dwBytesToRead )
{
	DWORD dwFilesize, dwRead;

	if( FALSE == Open( szFilename, modeRead ) )
		return -1;
	dwFilesize = Size();
	if( dwFilesize < dwBytesToRead )
		dwBytesToRead = dwFilesize;
	dwRead = Read( lpBuffer, dwBytesToRead );
	Close();
	return dwRead;
}

int CLowFile::Write( LPVOID lpBuffer, DWORD dwBytesToWrite )
{
	DWORD dwWrite;

	if( WriteFile( m_hFile, lpBuffer, dwBytesToWrite, &dwWrite, NULL ) == FALSE )
		return -1;
	return (int) dwWrite;
}

int CLowFile::Write( LPCSTR szFilename, LPVOID lpBuffer, DWORD dwBytesToWrite )
{
	DWORD dwWrite;

	if( FALSE == Open( szFilename, modeWrite ) )
		return -1;
	dwWrite = Write( lpBuffer, dwBytesToWrite );
	Close();
	return dwWrite;
}

int CLowFile::Seek( LONG dwBytesToMove, SeekFlags sfMoveMethod )
{
   return SetFilePointer( m_hFile, dwBytesToMove, NULL, sfMoveMethod );
}

int CLowFile::GetFilePointer( void )
{
	return SetFilePointer( m_hFile, 0, NULL, FILE_CURRENT );
}

BOOL CLowFile::IsEOF( void )
{	
	return ( GetFilePointer() >= Size() );
}
