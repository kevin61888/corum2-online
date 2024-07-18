//
// File: INIFile.cpp
// Desc: INI file class
//

#include "../../StdAfx.h"
#include "INIFile.h"
#include <vector>
#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <float.h>
#if _MSC_VER > 1200
#include <fstream>
#include <ios>

using namespace std;
#else
#include <fstream.h>
#endif

const CHAR CommentIndicators[]	= ";#";
const CHAR EqualIndicators[]	= "=:"; 
const CHAR WhiteSpace[]			= " \t\n\r";

static int CompareNoCase( STLSTRING str1, STLSTRING str2 )
{
	if( str1 == "" || str2 == "" )
		return -1;
#ifdef WIN32
	return stricmp(str1.c_str(), str2.c_str());	
#else
	return strcasecmp(str1.c_str(), str2.c_str());
#endif
}

static void Trim( STLSTRING& szStr )
{
	STLSTRING szTrimChars = WhiteSpace;
	
	szTrimChars += EqualIndicators;
	int nPos, rPos;

	// trim left
	nPos = szStr.find_first_not_of(szTrimChars);

	if ( nPos > 0 )
		szStr.erase(0, nPos);

	// trim right and return
	nPos = szStr.find_last_not_of(szTrimChars);
	rPos = szStr.find_last_of(szTrimChars);

	if ( rPos > nPos && rPos > -1)
		szStr.erase(rPos, szStr.size()-rPos);
}

static STLSTRING GetNextWord( STLSTRING& CommandLine )
{
	int nPos = CommandLine.find_first_of(EqualIndicators);
	STLSTRING sWord = STLSTRING("");

	if ( nPos > -1 ) {
		sWord = CommandLine.substr(0, nPos);
		CommandLine.erase(0, nPos+1);
	} else {
		sWord = CommandLine;
		CommandLine = STLSTRING("");
	}
	Trim( sWord );
	return sWord;
}

static int WriteLn( fstream& stream, char* fmt, ... )
{
	char buf[MAX_BUFFER_LEN];
	int nLength;
	STLSTRING szMsg;

	memset(buf, 0, MAX_BUFFER_LEN);
	va_list args;
	va_start (args, fmt);
	nLength = vsnprintf(buf, MAX_BUFFER_LEN, fmt, args);
	va_end (args);

	if ( buf[nLength] != '\n' && buf[nLength] != '\r' )
		buf[nLength++] = '\n';
	stream.write(buf, nLength);

	return nLength;
}

CINIFile::CINIFile( STLSTRING szFileName )
{
	m_bDirty		= FALSE;
	m_szFileName	= szFileName;
	m_Flags			= (AUTOCREATE_SECTIONS | AUTOCREATE_KEYS);
	m_Sections.push_back( (new SectionType) );
	Load( m_szFileName );
}

CINIFile::CINIFile()
{
	Clear();
	m_Flags			= (AUTOCREATE_SECTIONS | AUTOCREATE_KEYS);
	m_Sections.push_back( (new SectionType) );
}

CINIFile::~CINIFile()
{
	if ( m_bDirty )
		Save();
	int nSectionCount = m_Sections.size();
	for( int i=0; i < nSectionCount; i++ ) {
		int nKeyCount = m_Sections[i]->Keys.size();
		for( int j=0; j < nKeyCount; j++ ) {
			if( m_Sections[i]->Keys[j] != NULL )
				delete( m_Sections[i]->Keys[j] );
		}
		m_Sections[i]->Keys.clear();
		if( m_Sections[i] != NULL )
			delete( m_Sections[i] );
	}
	m_Sections.clear();
}

void CINIFile::Clear()
{
	m_bDirty		= FALSE;
	m_szFileName	= STLSTRING("");
	m_Sections.clear();
}

void CINIFile::SetFileName( STLSTRING szFileName )
{
	if( m_szFileName.size() != 0 && CompareNoCase( szFileName, m_szFileName ) != 0 )
		m_bDirty = TRUE;
	m_szFileName = szFileName;
}

BOOL CINIFile::Load( STLSTRING szFileName )
{
	// We dont want to create a new file here.  If it doesn't exist, just
	// return FALSE and report the failure.
	fstream File( szFileName.c_str(), ios::in|ios::in );
	if ( File.is_open() ) {
		BOOL bDone = FALSE;
		BOOL bAutoKey = (m_Flags & AUTOCREATE_KEYS) == AUTOCREATE_KEYS;
		BOOL bAutoSec = (m_Flags & AUTOCREATE_SECTIONS) == AUTOCREATE_SECTIONS;
		
		STLSTRING szLine;
		STLSTRING szComment;
		char buffer[MAX_BUFFER_LEN]; 
		SectionType* pSection = GetSection("");

		// These need to be set, we'll restore the original values later.
		m_Flags |= AUTOCREATE_KEYS;
		m_Flags |= AUTOCREATE_SECTIONS;

		while ( !bDone ) {
			memset(buffer, 0, MAX_BUFFER_LEN);
			File.getline(buffer, MAX_BUFFER_LEN);

			szLine = buffer;
			Trim(szLine);

			bDone = ( File.eof() || File.bad() || File.fail() );

			if ( szLine.find_first_of(CommentIndicators) == 0 ) {
				szComment += "\n";
				szComment += szLine;
			} else if ( szLine.find_first_of('[') == 0 ) { // new section
				szLine.erase( 0, 1 );
				szLine.erase( szLine.find_last_of(']'), 1 );

				CreateSection(szLine, szComment);
				pSection = GetSection(szLine);
				szComment = STLSTRING("");
			} else if ( szLine.size() > 0 ) { // we have a key, add this key/value pair
				STLSTRING szKey = GetNextWord(szLine);
				STLSTRING szValue = szLine;

				if ( szKey.size() > 0 && szValue.size() > 0 ) {
					SetValue( szKey, szValue, szComment, pSection->szName );
					szComment = STLSTRING("");
				}
			}
		}

		// Restore the original flag values.
		if ( !bAutoKey )
			m_Flags &= ~AUTOCREATE_KEYS;

		if ( !bAutoSec )
			m_Flags &= ~AUTOCREATE_SECTIONS;
	} else {
		return FALSE;
	}
	File.close();
	
	m_bDirty = FALSE;
	return TRUE;
}

BOOL CINIFile::Save( void )
{
	if ( KeyCount() == 0 && SectionCount() == 0 )
		return FALSE; 
	if ( m_szFileName.size() == 0 )
		return FALSE;
	fstream File(m_szFileName.c_str(), ios::out|ios::trunc );
	if ( File.is_open() ) {
		SectionItr s_pos;
		KeyItr k_pos;
		SectionType *Section;
		KeyType *Key;

		for ( s_pos = m_Sections.begin(); s_pos != m_Sections.end(); s_pos++ ) {
			Section = (*s_pos);
			BOOL bWroteComment = FALSE;

			if ( Section->szComment.size() > 0 ) {
				bWroteComment = TRUE;
				WriteLn( File, "\n%s", CommentStr( Section->szComment).c_str() );
			}
			if ( Section->szName.size() > 0 ) {
				WriteLn( File, "%s[%s]", bWroteComment ? "" : "\n", Section->szName.c_str() );
			}
			for ( k_pos = Section->Keys.begin(); k_pos != Section->Keys.end(); k_pos++ ) {
				Key = (*k_pos);

				if ( Key->szKey.size() > 0 && Key->szValue.size() > 0 ) {
					WriteLn(File, "%s%s%s%s%c%s", 
						Key->szComment.size() > 0 ? "\n" : "",
						CommentStr( Key->szComment ).c_str(),
						Key->szComment.size() > 0 ? "\n" : "",
						Key->szKey.c_str(),
						EqualIndicators[0],
						Key->szValue.c_str());
				}
			}
		}
	} else {
		return FALSE;
	}
	m_bDirty = FALSE;
	File.flush();
	File.close();
	return TRUE;
}

BOOL CINIFile::SetKeyComment( STLSTRING szKey, STLSTRING szComment, STLSTRING szSection )
{
	KeyItr k_pos;
	SectionType* pSection;

	if ( (pSection = GetSection( szSection )) == NULL )
		return FALSE;
	for ( k_pos = pSection->Keys.begin(); k_pos != pSection->Keys.end(); k_pos++)  {
		if ( CompareNoCase( (*k_pos)->szKey, szKey ) == 0 ) {
			(*k_pos)->szComment = szComment;
			m_bDirty = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CINIFile::SetSectionComment( STLSTRING szSection, STLSTRING szComment )
{
	SectionItr s_pos;

	for ( s_pos = m_Sections.begin(); s_pos != m_Sections.end(); s_pos++ ) {
		if ( CompareNoCase( (*s_pos)->szName, szSection ) == 0 ) {
		    (*s_pos)->szComment = szComment;
			m_bDirty = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CINIFile::SetValue( STLSTRING szKey, STLSTRING szValue, STLSTRING szComment, STLSTRING szSection )
{
	KeyType* pKey = GetKey( szKey, szSection );
	SectionType* pSection = GetSection( szSection );

	if ( pSection == NULL ) {
		if ( !(m_Flags & AUTOCREATE_SECTIONS) || !CreateSection(szSection,""))
			return FALSE;
		pSection = GetSection(szSection);
	}

	if ( pSection == NULL )
		return FALSE;

	if ( pKey == NULL && szValue.size() > 0 && (m_Flags & AUTOCREATE_KEYS) ) {
		pKey = new KeyType;
		pKey->szKey = szKey;
		pKey->szValue = szValue;
		pKey->szComment = szComment;
		m_bDirty = TRUE;
		pSection->Keys.push_back( pKey );
		return TRUE;
	}
	if ( pKey != NULL ) {
		pKey->szValue = szValue;
		pKey->szComment = szComment;
		m_bDirty = TRUE;
		return TRUE;
	}
	return FALSE;
}

BOOL CINIFile::SetFloat( STLSTRING szKey, float fValue, STLSTRING szComment, STLSTRING szSection )
{
	char szStr[64];

	snprintf( szStr, 64, "%f", fValue );
	return SetValue( szKey, szStr, szComment, szSection );
}

BOOL CINIFile::SetInt(STLSTRING szKey, int nValue, STLSTRING szComment, STLSTRING szSection)
{
	char szStr[64];

	snprintf( szStr, 64, "%d", nValue );
	return SetValue( szKey, szStr, szComment, szSection );
}

BOOL CINIFile::SetBOOL(STLSTRING szKey, BOOL bValue, STLSTRING szComment, STLSTRING szSection)
{
	STLSTRING szValue = bValue ?  "TRUE" : "FALSE";

	return SetValue( szKey, szValue, szComment, szSection );
}

STLSTRING CINIFile::GetValue( STLSTRING szKey, STLSTRING szSection ) 
{
	KeyType* pKey = GetKey( szKey, szSection );

	return (pKey == NULL) ? STLSTRING("") : pKey->szValue;
}

STLSTRING CINIFile::GetString( STLSTRING szKey, STLSTRING szSection )
{
	return GetValue( szKey, szSection );
}

float CINIFile::GetFloat(STLSTRING szKey, STLSTRING szSection)
{
	STLSTRING szValue = GetValue(szKey, szSection);
	if ( szValue.size() == 0 )
		return 0.0;
	return (float)atof( szValue.c_str() );
}

int	CINIFile::GetInt( STLSTRING szKey, STLSTRING szSection )
{
	STLSTRING szValue = GetValue(szKey, szSection);

	if ( szValue.size() == 0 )
		return 0;

	return atoi( szValue.c_str() );
}

BOOL CINIFile::GetBOOL( STLSTRING szKey, STLSTRING szSection )
{
	BOOL bValue = FALSE;
	STLSTRING szValue = GetValue(szKey, szSection);

	if ( szValue.find("1") == 0 ||
		CompareNoCase( szValue, "TRUE" ) == 0 ||
		CompareNoCase( szValue, "yes" ) == 0 )
		bValue = TRUE;
	return bValue;
}

BOOL CINIFile::DeleteSection(STLSTRING szSection)
{
	SectionItr s_pos;

	for ( s_pos = m_Sections.begin(); s_pos != m_Sections.end(); s_pos++ ) {
		if ( CompareNoCase( (*s_pos)->szName, szSection ) == 0 ) {
			for ( int i=0; i < (int) (*s_pos)->Keys.size(); i++ ) {
				if( (*s_pos)->Keys[i] != NULL ) {
					delete( (*s_pos)->Keys[i] );
				}
			}
			m_Sections.erase( s_pos );
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CINIFile::DeleteKey(STLSTRING szKey, STLSTRING szFromSection)
{
	KeyItr k_pos;
	SectionType* pSection;

	if ( (pSection = GetSection(szFromSection)) == NULL )
		return FALSE;
	for ( k_pos = pSection->Keys.begin(); k_pos != pSection->Keys.end(); k_pos++ ) {
		if ( CompareNoCase( (*k_pos)->szKey, szKey ) == 0 ) {
			if( (*k_pos) != NULL )
				delete( *k_pos );
			pSection->Keys.erase( k_pos );
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CINIFile::CreateKey(STLSTRING szKey, STLSTRING szValue, STLSTRING szComment, STLSTRING szSection)
{
	BOOL bAutoKey = (m_Flags & AUTOCREATE_KEYS) == AUTOCREATE_KEYS;
	BOOL bReturn  = FALSE;
	m_Flags |= AUTOCREATE_KEYS;
	bReturn = SetValue(szKey, szValue, szComment, szSection);
	if ( !bAutoKey )
		m_Flags &= ~AUTOCREATE_KEYS;
	return bReturn;
}

BOOL CINIFile::CreateSection(STLSTRING szSection, STLSTRING szComment)
{
	SectionType* pSection = GetSection(szSection);
	if ( pSection )
		return FALSE;
	pSection = new SectionType;
	pSection->szName = szSection;
	pSection->szComment = szComment;
	m_Sections.push_back( pSection );
	m_bDirty = TRUE;
	return TRUE;
}

BOOL CINIFile::CreateSection( STLSTRING szSection, STLSTRING szComment, KeyList Keys )
{
	if ( !CreateSection(szSection, szComment) )
		return FALSE;
	SectionType* pSection = GetSection(szSection);

	if ( !pSection )
		return FALSE;
	KeyItr k_pos;

	pSection->szName = szSection;
	for ( k_pos = Keys.begin(); k_pos != Keys.end(); k_pos++ ) {
		KeyType* pKey = new KeyType;
		pKey->szComment = (*k_pos)->szComment;
		pKey->szKey = (*k_pos)->szKey;
		pKey->szValue = (*k_pos)->szValue;

		pSection->Keys.push_back( pKey );
	}
	m_Sections.push_back( pSection );
	m_bDirty = TRUE;

	return TRUE;
}

int CINIFile::SectionCount() 
{ 
	return m_Sections.size(); 
}

int CINIFile::KeyCount()
{
	int nCounter = 0;
	SectionItr s_pos;

	for ( s_pos = m_Sections.begin(); s_pos != m_Sections.end(); s_pos++ )
		nCounter += (*s_pos)->Keys.size();
	return nCounter;
}

CINIFile::KeyType* CINIFile::GetKey(STLSTRING szKey, STLSTRING szSection)
{
	KeyItr k_pos;
	SectionType* pSection;

	if ( (pSection = GetSection(szSection)) == NULL )
		return NULL;
	for ( k_pos = pSection->Keys.begin(); k_pos != pSection->Keys.end(); k_pos++ ) {
		if ( CompareNoCase( (*k_pos)->szKey, szKey ) == 0 )
			return (*k_pos);
	}
	return NULL;
}

CINIFile::SectionType* CINIFile::GetSection(STLSTRING szSection)
{
	SectionItr s_pos;

	for ( s_pos = m_Sections.begin(); s_pos != m_Sections.end(); s_pos++ ) {
		if ( CompareNoCase( (*s_pos)->szName, szSection ) == 0 ) 
			return (*s_pos);
	}
	return NULL;
}

STLSTRING CINIFile::CommentStr( STLSTRING szComment )
{
	STLSTRING szNewStr = STLSTRING("");

	Trim( szComment );
    if ( szComment.size() == 0 )
       return szComment;
	if ( szComment.find_first_of(CommentIndicators) != 0 ) {
		szNewStr = CommentIndicators[0];
		szNewStr += " ";
	}
	szNewStr += szComment;
	return szNewStr;
}
