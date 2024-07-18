#pragma once

// File  : BufferFormatter.hpp
// Author: Byun DongHo	(cpascal@nownuri.net>
// Desc. : Buffer Formatter (memory / packet)

#include <iostream>

#include <string>

typedef class CBufferFormatter*		LPCBufferFormatter;

class CBufferFormatter
{
public:
	// constructor and destructor
	inline CBufferFormatter( void );
	inline CBufferFormatter( LPVOID lpBuffer );
	inline ~CBufferFormatter( void );

public:
	inline void Attach( LPVOID lpBuffer );
	inline void Reset( void );

	inline int		Size( void );
	template <typename BufferType> inline BufferType SkipT( int nSize );			// cppis : 사용의 편의성을 위해서 템플릿으로 수정
	template <typename BufferType> inline BufferType GetBufferT( void )			{ return reinterpret_cast<BufferType>(m_pBuffer);		}
	template <typename BufferType> inline BufferType GetCurrentPtrT( void )		{ return reinterpret_cast<BufferType>(m_pCurrentPtr);	}

	inline 	LPBYTE	Skip( int nSize );
	inline 	LPBYTE	GetBuffer( void )		{ return reinterpret_cast<LPBYTE>(m_pBuffer);		}
	inline	LPBYTE	GetCurrentPtr( void )	{ return reinterpret_cast<LPBYTE>(m_pCurrentPtr);	}

public:
	// Put into Buffer
	inline CBufferFormatter& operator << (CHAR chValue);
	inline CBufferFormatter& operator << (BYTE byValue);
	inline CBufferFormatter& operator << (short sValue);
	inline CBufferFormatter& operator << (WORD wValue);
	inline CBufferFormatter& operator << (int nValue);
	inline CBufferFormatter& operator << (DWORD wValue);
	inline CBufferFormatter& operator << (__int64 i64Value);
	inline CBufferFormatter& operator << (DWORD64 dw64Value);
	//inline CBufferFormatter& operator << (CHAR *szString);
	//inline CBufferFormatter& operator << (const CHAR *szString);
	//inline CBufferFormatter& operator << (std::string &strString);
	inline void PutCHAR( CHAR chValue );
	inline void PutBYTE( BYTE byValue );
	inline void PutSHORT( short sValue );
	inline void PutWORD( WORD wValue );
	inline void PutINT( int nValue );
	inline void PutDWORD( DWORD dwValue );
	inline void PutINT64( __int64 i64Value );
	inline void PutDWORD64( DWORD64 dw64Value );
	//inline void PutSTRING( CHAR *szString );
	inline void PutSTRING( const CHAR *szString );
	inline void PutSTRING( const CHAR *szString, const int nSize );
	inline void PutSTRING2( const CHAR *szString, const WORD nSize );
	//inline void PutSTRING( std::string &strString );
	//inline void PutSTRING( CHAR *szString, int nMaxLen );
	//inline int	PutSTRING( const CHAR *szString, int nMaxLen );
	//inline int	PutSTRING( std::string &strString, int nMaxLen );
	inline void PutBINARY( LPVOID szBuffer, int nLen );

	// Get from Buffer
	inline CBufferFormatter& operator >> (CHAR& chValue);
	inline CBufferFormatter& operator >> (BYTE& byValue);
	inline CBufferFormatter& operator >> (short& sValue);
	inline CBufferFormatter& operator >> (WORD& wValue);
	inline CBufferFormatter& operator >> (int& nValue);
	inline CBufferFormatter& operator >> (DWORD& dwValue);
	inline CBufferFormatter& operator >> (__int64& i64Value);
	inline CBufferFormatter& operator >> (DWORD64& dw64Value);
	//inline CBufferFormatter& operator >> (std::string &strString);
	// 가능하면 GetString() 사용 희망, 버퍼오버플로우 가능성
	//inline CBufferFormatter& operator >> (CHAR *szString);

	inline CHAR		GetCHAR( void );
	inline BYTE		GetBYTE( void );
	inline short	GetSHORT( void );
	inline WORD		GetWORD( void );
	inline int		GetINT( void );
	inline DWORD	GetDWORD( void );
	inline __int64	GetINT64( void );
	inline DWORD64	GetDWORD64( void );
	//inline int		GetSTRING( std::string &strString, int nMaxLen );
	//inline int		GetSTRING( CHAR *szString, int nMaxLen );
	inline int		GetBINARY( LPVOID szBuffer, int nLen );

private:
	BYTE *m_pBuffer;
	BYTE *m_pCurrentPtr;
};

CBufferFormatter::CBufferFormatter( LPVOID lpBuffer )
{
	Attach( lpBuffer );
}

void CBufferFormatter::Attach( LPVOID lpBuffer )
{
	m_pBuffer = (LPBYTE) lpBuffer;
	Reset();
}

void CBufferFormatter::Reset( void )
{
	m_pCurrentPtr = m_pBuffer;
}

CBufferFormatter::CBufferFormatter( void )
{
}

CBufferFormatter::~CBufferFormatter( void )
{
}

int CBufferFormatter::Size( void )
{
	return (int)(m_pCurrentPtr - m_pBuffer);
}

LPBYTE CBufferFormatter::Skip( int nSize )
{
	LPBYTE lpRet = m_pCurrentPtr;

	m_pCurrentPtr += nSize;
	return lpRet;
}

/**
 *	nSize 만큼 버퍼의 사용가능 포인터 위치를 이동한다.
 *	그러나, 리턴되는 포인터의 위치는 이동이 이루어지기 이전의 것이다!!
 * 
 *	@param	nSize		버퍼 위치 포인터가 이동할 거리
 *	@return	BufferType	리턴되는 포인터의 위치를 BufferType 으로 캐스팅한 값
 */
template <typename BufferType> BufferType CBufferFormatter::SkipT( int nSize )
{
	LPBYTE lpRet = m_pCurrentPtr;

	m_pCurrentPtr += nSize;

	return reinterpret_cast<BufferType>(lpRet);
}

// Put into Buffer
CBufferFormatter& CBufferFormatter::operator << (CHAR chValue)
{
	*((CHAR *)m_pCurrentPtr) = chValue;
	m_pCurrentPtr++;
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (BYTE byValue)
{
	*((BYTE *) m_pCurrentPtr) = byValue;
	m_pCurrentPtr++;
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (short sValue)
{
	*((short *) m_pCurrentPtr) = sValue;
	m_pCurrentPtr += sizeof( short );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (WORD wValue)
{
	*((WORD *) m_pCurrentPtr) = wValue;
	m_pCurrentPtr += sizeof( WORD );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (int nValue)
{
	*((int *) m_pCurrentPtr) = nValue;
	m_pCurrentPtr += sizeof( int );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (DWORD dwValue)
{
	*((DWORD *) m_pCurrentPtr) = dwValue;
	m_pCurrentPtr += sizeof( DWORD );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (__int64 i64Value)
{
	*((__int64 *) m_pCurrentPtr) = i64Value;
	m_pCurrentPtr += sizeof( __int64 );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (DWORD64 dw64Value)
{
	*((DWORD64 *) m_pCurrentPtr) = dw64Value;
	m_pCurrentPtr += sizeof( DWORD64 );
	return *this;
}

/*CBufferFormatter& CBufferFormatter::operator << (CHAR *szString)
{
	while ( *szString ) 
		*((CHAR *)m_pCurrentPtr++) = *(szString++);
	*((CHAR *)m_pCurrentPtr++) = '\0';
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (const CHAR *szString)
{
	while ( *szString ) 
		*((CHAR *)m_pCurrentPtr++) = *((CHAR *)szString++);
	*((CHAR *)m_pCurrentPtr++) = '\0';
	return *this;
}

CBufferFormatter& CBufferFormatter::operator << (std::string &strString)
{
	memcpy( (LPVOID) m_pCurrentPtr, (LPVOID) strString.c_str(), strString.size() + 1 );
	m_pCurrentPtr += (strString.size() + 1);
	return *this;
}*/

void CBufferFormatter::PutCHAR( CHAR chValue )
{
	*((CHAR *)m_pCurrentPtr) = chValue;
	m_pCurrentPtr++;
}

void CBufferFormatter::PutBYTE( BYTE byValue )
{
	*((BYTE *) m_pCurrentPtr) = byValue;
	m_pCurrentPtr++;
}

void CBufferFormatter::PutSHORT( short sValue )
{
	*((short *) m_pCurrentPtr) = sValue;
	m_pCurrentPtr += sizeof( short );
}

void CBufferFormatter::PutWORD( WORD wValue )
{
	*((WORD *) m_pCurrentPtr) = wValue;
	m_pCurrentPtr += sizeof( WORD );
}

void CBufferFormatter::PutINT( int nValue )
{
	*((int *) m_pCurrentPtr) = nValue;
	m_pCurrentPtr += sizeof( int );
}

void CBufferFormatter::PutDWORD( DWORD dwValue )
{
	*((DWORD *) m_pCurrentPtr) = dwValue;
	m_pCurrentPtr += sizeof( DWORD );
}

void CBufferFormatter::PutINT64( __int64 i64Value )
{
	*((__int64 *) m_pCurrentPtr) = i64Value;
	m_pCurrentPtr += sizeof( __int64 );
}

void CBufferFormatter::PutDWORD64( DWORD64 dw64Value )
{
	*((DWORD64 *) m_pCurrentPtr) = dw64Value;
	m_pCurrentPtr += sizeof( DWORD64 );
}

//String 형을 StrLenByte + StrData 로 정의한다!
void CBufferFormatter::PutSTRING( const CHAR *szString )
{
	PutSTRING(szString, (int)(strlen(szString)));
}

void CBufferFormatter::PutSTRING( const CHAR *szString, int nSize )
{
	PutBYTE((BYTE)nSize);
	PutBINARY((LPVOID)szString, nSize);
}

void CBufferFormatter::PutSTRING2( const CHAR *szString, const WORD nSize )
{
	PutWORD(nSize);
	PutBINARY((LPVOID)szString, nSize);
}

/*
void CBufferFormatter::PutSTRING( CHAR *szString )
{
	while ( *szString ) 
		*((CHAR *)m_pCurrentPtr++) = *(szString++);
	*((CHAR *)m_pCurrentPtr++) = '\0';
}

void CBufferFormatter::PutSTRING( const CHAR *szString )
{
	while ( *szString ) 
		*((CHAR *)m_pCurrentPtr++) = *((CHAR *)szString++);
	*((CHAR *)m_pCurrentPtr++) = '\0';
}

void CBufferFormatter::PutSTRING( std::string &strString )
{
	memcpy( (LPVOID) m_pCurrentPtr, (LPVOID) strString.c_str(), strString.size() + 1 );
	m_pCurrentPtr += (strString.size() + 1);
}

void CBufferFormatter::PutSTRING( CHAR *szString, int nMaxLen )
{
	while ( (nMaxLen-- > 0) && *szString )
		*((CHAR *)m_pCurrentPtr++) = *(szString++);
	if ( nMaxLen >= 0 )
		*((CHAR *)m_pCurrentPtr++) = '\0';
}

int CBufferFormatter::PutSTRING( const CHAR *szString, int nMaxLen )
{
	BYTE *pPtr	= m_pCurrentPtr;
	while ( (nMaxLen-- > 0) && *szString )
		*((CHAR *)m_pCurrentPtr++) = *(szString++);
	if ( nMaxLen >= 0 ) {
		*((CHAR *)m_pCurrentPtr++) = '\0';
		return (int)(m_pCurrentPtr - pPtr - 1);	// remove 1 byte of pending '\0' from return value
	} else
		return (int)(m_pCurrentPtr - pPtr);
}

int CBufferFormatter::PutSTRING( std::string &strString, int nMaxLen )
{
	if ( (int)strString.size() >= nMaxLen ) {
		memcpy( (LPVOID) m_pCurrentPtr, (LPVOID) strString.c_str(), nMaxLen );
		m_pCurrentPtr += nMaxLen;
	} else {
		memcpy( (LPVOID) m_pCurrentPtr, (LPVOID) strString.c_str(), strString.size() + 1 );
		m_pCurrentPtr += (strString.size() + 1);
	}

	return (int)strString.size();
}*/

void CBufferFormatter::PutBINARY( LPVOID szBuffer, int nLen )
{
	memcpy( (LPVOID) m_pCurrentPtr, szBuffer, nLen );
	m_pCurrentPtr += nLen;
}

// Get from Buffer
CBufferFormatter& CBufferFormatter::operator >> (CHAR& chValue)
{
	chValue = *((CHAR *)m_pCurrentPtr);
	m_pCurrentPtr++;
	return *this;
}

CBufferFormatter& CBufferFormatter::operator >> (BYTE& byValue)
{
	byValue = *((BYTE *) m_pCurrentPtr);
	m_pCurrentPtr++;
	return *this;
}

CBufferFormatter& CBufferFormatter::operator >> (short& sValue)
{
	sValue = *((short *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( short );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator >> (WORD& wValue)
{
	wValue = *((WORD *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( WORD );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator >> (int& nValue)
{
	nValue = *((int *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( int );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator >> (DWORD& dwValue)
{
	dwValue = *((DWORD *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( DWORD );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator >> (__int64& i64Value)
{
	i64Value = *((__int64 *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( __int64 );
	return *this;
}

CBufferFormatter& CBufferFormatter::operator >> (DWORD64& dw64Value)
{
	dw64Value = *((DWORD64 *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( DWORD64 );
	return *this;
}
/*
CBufferFormatter& CBufferFormatter::operator >> (CHAR *szString)
{
	while ( *m_pCurrentPtr ) 
		*((CHAR *)szString++) = *((CHAR *)m_pCurrentPtr++);
	*((CHAR *)szString++) = *((CHAR *)m_pCurrentPtr++);		// \0 복사
	return *this;
}

CBufferFormatter& CBufferFormatter::operator >> (std::string &strString)
{
	strString.assign( (char *) m_pCurrentPtr );
	m_pCurrentPtr += (strlen( (const char *) m_pCurrentPtr ) + 1);
	return *this;
}
*/
CHAR CBufferFormatter::GetCHAR(void)
{
	register CHAR chValue;

	chValue = *((CHAR *)m_pCurrentPtr);
	m_pCurrentPtr++;
	return chValue;
}

BYTE CBufferFormatter::GetBYTE(void)
{
	register BYTE byValue;

	byValue = *((BYTE *) m_pCurrentPtr);
	m_pCurrentPtr++;
	return byValue;
}

short CBufferFormatter::GetSHORT(void)
{
	register short sValue;

	sValue = *((short *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( short );
	return sValue;
}

WORD CBufferFormatter::GetWORD(void)
{
	WORD wValue;

	wValue = *((WORD *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( WORD );
	return wValue;
}

int CBufferFormatter::GetINT(void)
{
	int nValue;

	nValue = *((int *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( int );
	return nValue;
}

DWORD CBufferFormatter::GetDWORD(void)
{
	DWORD dwValue;

	dwValue = *((DWORD *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( DWORD );
	return dwValue;
}

__int64 CBufferFormatter::GetINT64(void)
{
	__int64 i64Value;

	i64Value = *((__int64 *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( __int64 );
	return i64Value;
}

DWORD64 CBufferFormatter::GetDWORD64(void)
{
	DWORD64 dw64Value;

	dw64Value = *((DWORD64 *) m_pCurrentPtr);
	m_pCurrentPtr += sizeof( DWORD64 );
	return dw64Value;
}
/*
int CBufferFormatter::GetSTRING( std::string &strString, int nMaxLen )
{
	strString.assign( (char *) m_pCurrentPtr, 0, nMaxLen );
	if ( (int)strString.size() < nMaxLen )
		m_pCurrentPtr += strString.size() + 1;
	else
		m_pCurrentPtr += nMaxLen;
	return (int)strString.size();
}

int CBufferFormatter::GetSTRING( CHAR *szString, int nMaxLen )
{
	CHAR *pPtr = szString;

	while ( (nMaxLen-- > 0) && *m_pCurrentPtr )
		*(pPtr++) = *((CHAR *)m_pCurrentPtr++);
	if ( nMaxLen >= 0 ) {
		*(pPtr) = '\0';
		m_pCurrentPtr++;
	}
	return (int)(pPtr - szString);
}
*/
int CBufferFormatter::GetBINARY( LPVOID szBuffer, int nLen )
{
	memcpy( szBuffer, (LPVOID) m_pCurrentPtr, nLen );
	m_pCurrentPtr += nLen;
	return nLen;
}
