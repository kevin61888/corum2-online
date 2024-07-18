#include "DebugLog.h"

CDebugLog::CDebugLog()
{
	m_fpLogFile = NULL;
}

CDebugLog::~CDebugLog()
{
	if( m_fpLogFile )
	{
		fclose( m_fpLogFile );
		m_fpLogFile = NULL;
	}
}

BOOL CDebugLog::InitDubugLog( char* szFileName )
{
	if( !szFileName ) return FALSE;
	
	Save();

	m_fpLogFile = fopen( szFileName, "wt" );
	if( !m_fpLogFile ) return FALSE;

	return TRUE;
}

BOOL CDebugLog::Log( char* szLog )
{
	if( !m_fpLogFile ) return FALSE;

	fprintf( m_fpLogFile, szLog );
	fprintf( m_fpLogFile, "\r\n" );

	return TRUE;
}

BOOL CDebugLog::Save()
{
	if( !m_fpLogFile ) return FALSE;

	fclose( m_fpLogFile );
	
	return TRUE;
}