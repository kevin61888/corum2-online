#pragma once

#include <iostream>

/**
 * 링버퍼에서 memcpy 없이 데이터 끄집어 오기 위한 용도로 이용.
 * 현재로선 각 데이터형에 대한 사이즈에 제약이 있다.
 * short,WORD - 2bytes, int,DWORD - 4bytes, 
 */

typedef class CRingBufferReader*		LPCRingBufferReader;
class CRingBufferReader
{
public:
	inline CRingBufferReader( LPBYTE pBufferLinearStart, LPBYTE pBufferStart, LPBYTE pBufferLinearEnd, LPBYTE pBufferEnd );
	inline ~CRingBufferReader( void );

public:
	inline LPBYTE	Skip( size_t nSize = 1); // 내부 버퍼 포인터를 nSize 만큼 이동하며 반환은 현재 포인터를 반환함.
	inline size_t	GetLeftSizeToLinearEnd( void );

public:
	inline CHAR		GetCHAR( void );
	inline BYTE		GetBYTE( void );
	inline short	GetSHORT( void );
	inline WORD		GetWORD( void );
	inline INT		GetINT( void );
	inline DWORD	GetDWORD( void );
	inline INT64	GetINT64( void );
	inline DWORD64	GetDWORD64( void );
	inline void		GetSTRING( LPBYTE szBuffer );//String 형을 StrLenByte + StrData 로 정의한다!
	inline void		GetBINARY( LPBYTE szBuffer, size_t nLen );// 문자열등 1바이트의 연속된 형태의 데이터 추출시 활용.

	inline LPBYTE	GetCurrentPtr() { return m_pCurrentPtr; }

private:
	LPBYTE	m_pBufferLinearStart;	// 전체 링버퍼의 물리적 시작지점(Including)
	LPBYTE	m_pBufferStart;		// 읽기 시작할 지점(Including)
	LPBYTE	m_pBufferLinearEnd;		// 전체 링버퍼의 물리적 종료지점(Excluding)
	LPBYTE	m_pBufferEnd;			// 읽기 종료할 지점(Excluding)
	LPBYTE	m_pCurrentPtr;		// 링버퍼에서 현재 읽을 버퍼 포인터
};

CRingBufferReader::CRingBufferReader( LPBYTE pBufferLinearStart, LPBYTE pBufferStart, LPBYTE pBufferLinearEnd, LPBYTE pBufferEnd ) : m_pBufferLinearStart(pBufferLinearStart), m_pBufferStart(pBufferStart), m_pBufferLinearEnd(pBufferLinearEnd), m_pBufferEnd(pBufferEnd), m_pCurrentPtr(pBufferStart)
{
}

CRingBufferReader::~CRingBufferReader( void )
{
}

LPBYTE CRingBufferReader::Skip( size_t nSize )
{
	LPBYTE lpRet = m_pCurrentPtr;

	if( m_pCurrentPtr + nSize < m_pBufferLinearEnd )
		m_pCurrentPtr += nSize;
	else //m_pCurrentPtr + nSize == m_pBufferLinearEnd 인 경우 이쪽으로
		m_pCurrentPtr = m_pBufferLinearStart + nSize - GetLeftSizeToLinearEnd();
	
	return lpRet;
}

size_t CRingBufferReader::GetLeftSizeToLinearEnd( void )
{
	return m_pBufferLinearEnd - m_pCurrentPtr;
}


CHAR CRingBufferReader::GetCHAR(void)
{
	register CHAR chValue;

	chValue = *((CHAR *)m_pCurrentPtr);

	Skip(sizeof(CHAR));

	return chValue;
}

BYTE CRingBufferReader::GetBYTE(void)
{
	register BYTE byValue;

	byValue = *((BYTE *) m_pCurrentPtr);

	Skip(sizeof(BYTE));

	return byValue;
}

short CRingBufferReader::GetSHORT(void)
{
	short sValue;

	if( sizeof(short) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();

		sValue = *pTwo;
		sValue <<= 8;
		sValue += *pOne;
	}
	else
	{
		sValue = *((short *) m_pCurrentPtr);
		Skip( sizeof(short) );
	}

	return sValue;
}

WORD CRingBufferReader::GetWORD(void)
{
	WORD wValue;

	if( sizeof(WORD) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();

		wValue = *pTwo;
		wValue <<= 8;
		wValue += *pOne;
	}
	else
	{
		wValue = *((WORD *) m_pCurrentPtr);
		Skip( sizeof(WORD) );
	}
	return wValue;
}

INT CRingBufferReader::GetINT(void)
{
	INT nValue;

	if( sizeof(INT) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();
		LPBYTE pThree	= Skip();
		LPBYTE pFour	= Skip();

		nValue = *pFour;
		nValue <<= 8;
		nValue += *pThree;
		nValue <<= 8;
		nValue += *pTwo;
		nValue <<= 8;
		nValue += *pOne;
	}
	else
	{
		nValue = *((INT *) m_pCurrentPtr);
		Skip( sizeof(INT) );
	}

	return nValue;
}

DWORD CRingBufferReader::GetDWORD(void)
{
	DWORD dwValue;

	if( sizeof(DWORD) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();
		LPBYTE pThree	= Skip();
		LPBYTE pFour	= Skip();

		dwValue = *pFour;
		dwValue <<= 8;
		dwValue += *pThree;
		dwValue <<= 8;
		dwValue += *pTwo;
		dwValue <<= 8;
		dwValue += *pOne;
	}
	else
	{
		dwValue = *((DWORD *) m_pCurrentPtr);
		Skip( sizeof(DWORD) );
	}

	return dwValue;
}

INT64 CRingBufferReader::GetINT64(void)
{
	INT64 i64Value;

	if( sizeof(INT64) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();
		LPBYTE pThree	= Skip();
		LPBYTE pFour	= Skip();
		LPBYTE pFive	= Skip();
		LPBYTE pSix		= Skip();
		LPBYTE pSeven	= Skip();
		LPBYTE pEight	= Skip();

		i64Value = *pEight;
		i64Value <<= 8;
		i64Value += *pSeven;
		i64Value <<= 8;
		i64Value += *pSix;
		i64Value <<= 8;
		i64Value += *pFive;
		i64Value <<= 8;
		i64Value += *pFour;
		i64Value <<= 8;
		i64Value += *pThree;
		i64Value <<= 8;
		i64Value += *pTwo;
		i64Value <<= 8;
		i64Value += *pOne;
	}
	else
	{
		i64Value = *((INT64 *) m_pCurrentPtr);
		Skip( sizeof(INT64) );
	}

	return i64Value;
}

DWORD64 CRingBufferReader::GetDWORD64(void)
{
	DWORD64 dw64Value;

	if( sizeof(DWORD64) > GetLeftSizeToLinearEnd() )
	{
		LPBYTE pOne		= Skip();
		LPBYTE pTwo		= Skip();
		LPBYTE pThree	= Skip();
		LPBYTE pFour	= Skip();
		LPBYTE pFive	= Skip();
		LPBYTE pSix		= Skip();
		LPBYTE pSeven	= Skip();
		LPBYTE pEight	= Skip();

		dw64Value = *pEight;
		dw64Value <<= 8;
		dw64Value += *pSeven;
		dw64Value <<= 8;
		dw64Value += *pSix;
		dw64Value <<= 8;
		dw64Value += *pFive;
		dw64Value <<= 8;
		dw64Value += *pFour;
		dw64Value <<= 8;
		dw64Value += *pThree;
		dw64Value <<= 8;
		dw64Value += *pTwo;
		dw64Value <<= 8;
		dw64Value += *pOne;
	}
	else
	{
		dw64Value = *((DWORD64 *) m_pCurrentPtr);
		Skip( sizeof(DWORD64) );
	}

	return dw64Value;
}

void CRingBufferReader::GetSTRING( LPBYTE szBuffer )
{
	BYTE byStrLen = GetBYTE();
	GetBINARY(szBuffer, byStrLen);
}

void CRingBufferReader::GetBINARY( LPBYTE szBuffer, size_t len )
{
	size_t leftSize = GetLeftSizeToLinearEnd();
	if( len > leftSize )
	{
		memcpy( szBuffer, m_pCurrentPtr, leftSize );
		memcpy( szBuffer + leftSize, m_pBufferLinearStart, len - leftSize );
	}
	else
	{
		memcpy( szBuffer, m_pCurrentPtr, len );
	}

	Skip( len );
}

