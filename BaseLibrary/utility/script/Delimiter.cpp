#include "../../StdAfx.h"

#include "Delimiter.h"


// TODO: CDelimiter::Write 계열 제작
CDelimiter::CDelimiter(char* szLine, char* pszDelimiter) : m_pHeadPtr(szLine), m_pCurrPtr(szLine)
{
	::strncpy(m_szDelimiter, pszDelimiter, MAX_DELIMITER_NUM);
}

CDelimiter::~CDelimiter(void)
{
	Terminate();
}

void CDelimiter::Terminate(void)
{
}

BOOL CDelimiter::ReadData(bool& bNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

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

BOOL CDelimiter::ReadData(double& dNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	dNumber = atof(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(float& fNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	fNumber = (float) atof(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(unsigned long& fNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	fNumber = (unsigned long) atol(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(int& iNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = atoi(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(unsigned short& iNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = (unsigned short)atoi(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(short& iNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = (short)atoi(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(unsigned char& iNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = (unsigned char)atoi(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(char& iNumber)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	iNumber = (char)atoi(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(__int64& i64Number)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	i64Number = _atoi64(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadData(DWORD64& i64Number)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	i64Number = (DWORD)_atoi64(pszData);
	return TRUE;
}

BOOL CDelimiter::ReadString(char* szString, DWORD dwSize)
{
	CHAR *pszData = ::strtok(m_pCurrPtr, m_szDelimiter);

	if ( !pszData || !(*pszData) )
		return FALSE;

	strncpy(szString, pszData, dwSize);
	return TRUE;
}

BOOL CDelimiter::ReadString(char* szString, DWORD dwSize, OUT CHAR* pcToken)
{
	CHAR *pszData = ::strpbrk(m_pCurrPtr, m_szDelimiter);

	*pcToken	= 0;
	if ( !pszData || !(*pszData) ) {
		if ( !(m_pCurrPtr) )
			return FALSE;
	}

	int	nLen;
	if ( pszData == m_pCurrPtr ) {
		*pcToken	= *pszData;	// set token
		nLen	= 1;
	} else {
		if (pszData)
			nLen	= pszData - m_pCurrPtr;
		else
			nLen	= static_cast<int>(strlen(m_pCurrPtr));
	}

	strncpy(szString, m_pCurrPtr, nLen);
	szString[ nLen ]	= '\0';
	m_pCurrPtr	+= nLen;
	return TRUE;
}
