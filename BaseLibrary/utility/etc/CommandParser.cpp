#include "../../StdAfx.h"
#include "CommandParser.h"


// constructor and destructor
CCommandParser::CCommandParser(void)
{
}

CCommandParser::~CCommandParser(void)
{
	// cppis : 명령의 삭제를 위해서 추가.
	for ( int i=0; i<CCommandParser::COMMANDMAP_NUMBER; ++i ) {
		for ( CommandMapItr itr=m_CommandMap[i].begin(); itr!=m_CommandMap[i].end(); ) {
			itr	= m_CommandMap[i].erase(itr);
		}
	}
}

DWORD CCommandParser::ToDWORD( LPSTR lpszCommand )
{
	DWORD dwRet;

	m_CRC32.InitValue( dwRet );
	while ( *lpszCommand )
		m_CRC32.PutData( dwRet, tolower( *lpszCommand++ ) );
	return dwRet;
}

CCommandParser::CommandMapType *CCommandParser::GetCommandMap( LPSTR lpszCommand )
{
	return &m_CommandMap[ tolower( *(BYTE *) lpszCommand ) ];
}

bool CCommandParser::InsertCommand( int nID, LPSTR lpszCommand )
{
	std::pair<CommandMapItr, bool> CommandMapPr;

	if ( lpszCommand == NULL || *lpszCommand == '\0' )
		return false;
	DWORD dwCRC32 = ToDWORD( lpszCommand );
	CommandMapPr = GetCommandMap( lpszCommand )->insert( CommandMapType::value_type( dwCRC32, nID ) );
	if ( CommandMapPr.second == false )
		return false;
	return true;
}

bool CCommandParser::RemoveCommand( int nID, LPSTR lpszCommand )
{
	if ( lpszCommand == NULL || *lpszCommand == '\0' )
		return false;
	DWORD dwCRC32 = ToDWORD( lpszCommand );
	CommandMapItr itr=GetCommandMap( lpszCommand )->find( dwCRC32 );
	if ( itr == GetCommandMap( lpszCommand )->end() )
		return false;
	GetCommandMap( lpszCommand )->erase( itr );
	return true;
}

int	CCommandParser::ParseCommand( LPSTR lpszCommand, int *nCommand )
{
	CHAR szCommand[256];
	int i;

	if ( lpszCommand == NULL || *lpszCommand == '\0' )
		return 0;
	for ( i=0; i < 256; ) {
		szCommand[i] = *lpszCommand;
		if ( *lpszCommand == '\0' )
			break;
		if ( *lpszCommand == ' ' || *lpszCommand == '\t' ) {
			szCommand[i] = '\0';
			while ( *lpszCommand == ' ' || *lpszCommand == '\t' ) {
				lpszCommand++;
				i++;
			}
			break;
		}
		i++;
		lpszCommand++;
	}
	*nCommand = Find( szCommand );
	if ( *nCommand == -1 )
		return 0;
	return i;
}

int	CCommandParser::Find( LPSTR lpszCommand )
{
	if( lpszCommand == NULL || *lpszCommand == '\0' )
		return -1;
	DWORD dwCRC32 = ToDWORD( lpszCommand );
	CommandMapItr itr = GetCommandMap( lpszCommand )->find( dwCRC32 );
	if ( itr == m_CommandMap[*(BYTE *) lpszCommand].end() )
		return -1;
	return itr->second;
}
