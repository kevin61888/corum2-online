#pragma once

#include <map>

#include <BaseLibrary/Utility/ETC/CRC32.h>


typedef class CCommandParser*		LPCCommandParser;
class CCommandParser
{
private:
	enum { COMMANDMAP_NUMBER	= 256 };
	typedef std::map<DWORD, int>		CommandMapType;
	typedef CommandMapType::iterator	CommandMapItr;

	CommandMapType	m_CommandMap[CCommandParser::COMMANDMAP_NUMBER];
	CCRC32			m_CRC32;


	DWORD			ToDWORD( LPSTR lpszCommand );
	CommandMapType*	GetCommandMap( LPSTR lpszCommand );


public:
// constructor and destructor
	CCommandParser(void);
	~CCommandParser(void);

	bool	InsertCommand( int nID, LPSTR lpszCommand );
	bool	RemoveCommand( int nID, LPSTR lpszCommand );

	int		ParseCommand( LPSTR lpszCommand, int *nCommand );
	int		Find( LPSTR lpszCommand );
};
