#include "../../StdAfx.h"

#include "ScriptFile.h"


// constructor and destructor
CScriptFile::CScriptFile(const char* pszDelimiter) : m_hFile(INVALID_HANDLE_VALUE), m_dwColumn(0), m_dwColumnCount(0)
{
	m_ColumnNames.reserve(DEFAULT_COLUMN_NUM);
	strncpy(m_szDelimiter, pszDelimiter, MAX_DELIMITER_NUM);

	m_szLine[0]			= '\0';
	m_szBackupLine[0]	= '\0';
}

CScriptFile::~CScriptFile(void)
{
	Terminate();
}

// GENERIC_READ  OPEN_EXISTING
BOOL CScriptFile::Open(LPCSTR szFilename, BOOL bHeadLine, DWORD dwDesiredAccess, DWORD dwCreationDisposition)
{
	m_hFile	= ::CreateFile(szFilename, dwDesiredAccess, FILE_SHARE_READ, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);
	if ( INVALID_HANDLE_VALUE == m_hFile )
		return FALSE;

	if ( TRUE == bHeadLine ) {
		ReadLine();

		for ( m_dwColumnCount=0; ; ++m_dwColumnCount ) {
			char* szColumnName = new char[MAX_COLUMN_LENGHT];
			if ( NULL == szColumnName )
				return FALSE; 

			if ( false == ReadString(szColumnName, MAX_COLUMN_LENGHT - 1) )  {
				delete szColumnName;
				break;
			}

			size_t nLength = strlen(szColumnName);
			while ( nLength-- > 0 ) {
				if ( szColumnName[nLength] == '\r' || szColumnName[nLength] == '\n' )
					szColumnName[nLength] = 0;
			}

			szColumnName[MAX_COLUMN_LENGHT - 1] = 0;
			m_ColumnNames.push_back(szColumnName);
		}
	}
	return TRUE;
}

void CScriptFile::Terminate(void)
{
	ColumnArray::size_type nTotalSize = m_ColumnNames.size();
	for ( ColumnArray::size_type nCount=0; nCount<nTotalSize; ++nCount ) { 
		SAFE_DELETE_ARRAY(m_ColumnNames[nCount]);
	}
	m_ColumnNames.clear();

	SAFE_CLOSEHANDLE(m_hFile);
}

int CScriptFile::ReadLine(void)
{
	int nReadLen	= ReadString();
	if ( nReadLen ) {
		if ( (m_szLine[0]) && (m_szLine[0] != ';') ) {
			m_dwColumn = 0;
			memcpy(m_szBackupLine, m_szLine, MAX_LINE_LENGTH);
			return nReadLen;
		}
	}
	return 0;
}

int CScriptFile::ReadString(void)
{
	TCHAR*	pBufferPtr	= m_szLine;
	int		nCount		= 0;

	DWORD	dwReadBytes	= 0;
	int		nCharacter	= 0;
	while ( ReadFile(m_hFile, (LPVOID)&nCharacter, 1, &dwReadBytes, NULL) && dwReadBytes ) {
		if ( nCharacter == EOF ) {
			if ( pBufferPtr == m_szLine )
				return 0;
			else
				break;
		}
		if ( (*pBufferPtr++ = (TCHAR)nCharacter) == _T('\n') )
			break;

		++nCount;
		dwReadBytes	= 0;
	}

	if ( pBufferPtr == m_szLine )
		return 0;

	if ( EOF == nCharacter )
		*pBufferPtr = _T('\0');					// 파일의 끝
	else
		*(pBufferPtr - 2) = _T('\0');			// \n 을 없애기 전에 \r을 없앤다.

	return nCount;
}

BOOL CScriptFile::ReadData(bool& bNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;
		
	bNumber = false;
	if ( atoi(pszData) == 1 )
		bNumber = true;

	char szString[2];
	strncpy(szString, pszData, 1);
	if ( NULL == ::strcmp(szString, "O") )
		bNumber = true;

	return TRUE;
}

BOOL CScriptFile::ReadData(double& dNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	dNumber = atof(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(float& fNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	fNumber = (float) atof(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(unsigned long& fNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	fNumber = (unsigned long) atol(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(int& iNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = atoi(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(unsigned short& iNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = (unsigned short)atoi(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(short& iNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = (short)atoi(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(unsigned char& iNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = static_cast<unsigned char>(*pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(char& iNumber)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = (char)atoi(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(__int64& i64Number)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	i64Number = _atoi64(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadData(DWORD64& i64Number)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	i64Number = (DWORD)_atoi64(pszData);
	return TRUE;
}

BOOL CScriptFile::ReadString(char* szString, DWORD dwSize)
{
	CHAR *pszData = ::strtok((m_dwColumn++) ? NULL : m_szLine, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	strncpy(szString, pszData, dwSize);
	return TRUE;
}

BOOL CScriptFile::ReadData(const char* szField, double& dNumber)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField) ) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(dNumber);
}

BOOL CScriptFile::ReadData(const char* szField, float& fNumber)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(fNumber);
}

BOOL CScriptFile::ReadData(const char* szField, unsigned long& fNumber)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(fNumber);
}

BOOL CScriptFile::ReadData(const char* szField, int& iNumber)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(iNumber);
}

BOOL CScriptFile::ReadData(const char* szField, unsigned short& iNumber)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(iNumber);
}

BOOL CScriptFile::ReadData(const char* szField, short& iNumber)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(iNumber);
}

BOOL CScriptFile::ReadData(const char* szField, unsigned char& iNumber)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(iNumber);
}

BOOL CScriptFile::ReadData(const char* szField, char& iNumber)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(iNumber);
}

BOOL CScriptFile::ReadData(const char* szField, __int64& i64Number)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(i64Number);
}

BOOL CScriptFile::ReadData(const char* szField, DWORD64& i64Number)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadData(i64Number);
}

BOOL CScriptFile::ReadString(const char* szField, char *szString, DWORD dwSize)
{
	int nSearch;

	if ( ((nSearch = FindColumn(szField)) == -1) )
		return FALSE;

	GotoColumn(nSearch);
	return ReadString(szString, dwSize);
}

void CScriptFile::WriteData(const char* pFormat, ...)
{
	DWORD dwWritten	= 0;

	va_list	Arguments;
	va_start(Arguments, pFormat);
	//dwWritten += _vsntprintf(m_szLine + dwWritten, MAX_LINE_LENGTH - dwWritten, pFormat, Arguments);
	dwWritten += _vsntprintf(m_szLine + dwWritten, MAX_LINE_LENGTH - dwWritten, pFormat, Arguments);
	va_end(Arguments);

	//dwWritten += _snprintf(m_szLine + dwWritten, MAX_LINE_LENGTH - dwWritten, "\r\n\0");

	DWORD	dwWriteBytes	= 0;
	WriteFile(m_hFile, m_szLine, dwWritten, &dwWriteBytes, NULL);
}

BOOL CScriptFile::GotoColumn(int nColumn)
{
	CHAR* pszPos = 0;

	if ( nColumn < 0 || (DWORD)nColumn > m_dwColumnCount )
		return FALSE;

	if ( (m_dwColumn == (DWORD) nColumn) )
		return TRUE;

	if ( nColumn == 0 ) {
		m_dwColumn = 0;
		strtok(m_szLine, m_szDelimiter);
	} else {
		for ( ;m_dwColumn < m_dwColumnCount && nColumn != m_dwColumn; ++m_dwColumn )
			pszPos = strtok(0, m_szDelimiter);

		if ( m_dwColumn == m_dwColumnCount ) {
			memcpy(m_szLine, m_szBackupLine, MAX_LINE_LENGTH);
			pszPos = strtok(m_szLine, m_szDelimiter);
			for ( m_dwColumn = 1; m_dwColumn < (DWORD)nColumn && nColumn != m_dwColumn; ++m_dwColumn ) {
				pszPos = strtok(NULL, m_szDelimiter);
			}
		}
	}

	return TRUE;
}

int CScriptFile::FindColumn(const char* szField)
{	
	for ( ColumnArray::size_type nCount = 0; nCount < m_ColumnNames.size(); ++nCount ) {
		const char* szColumnName = m_ColumnNames[nCount];
		if ( 0 == strcmp(szField, szColumnName) ) {
            return static_cast<int>(nCount);
		}
	}

	return -1;
}
