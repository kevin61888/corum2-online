#include "../../StdAfx.h"
#include "XlRecord.h"

#include <string>

#include "XlReader.h"

#include <BaseLibrary/Utility/Macro/BaseMath.h>

//#include <BaseLibrary/Utility/Debugging/DebugUtils.h>
//#define new New


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXlRecord
// constructor and destructor
CXlRecord::CXlRecord(void) : m_wType(0), m_wLength(0), m_nPosition(0), m_pData(NULL)
{
}

CXlRecord::CXlRecord(LPCXlRecord pRecord) : m_wType(pRecord->m_wType), m_wLength(pRecord->m_wLength), m_nPosition(pRecord->m_nPosition)
{
	m_pData	= new CHAR[m_wLength];
	::memcpy(m_pData, pRecord->m_pData, m_wLength);
}

CXlRecord::~CXlRecord(void)
{
	SAFE_DELETE_ARRAY(m_pData);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXlBOFRecord
// constructor and destructor
CXlBOFRecord::CXlBOFRecord(void) : m_wVersion(0), m_wStreamType(0)
{
}

CXlBOFRecord::CXlBOFRecord(LPCXlRecord pRecord) : CXlRecord(pRecord), 
m_wVersion(*reinterpret_cast<WORD*>(pRecord->m_pData)), m_wStreamType(*reinterpret_cast<WORD*>(pRecord->m_pData + sizeof(m_wVersion)))
{
}

CXlBOFRecord::~CXlBOFRecord(void)
{
}


const int	BIFF8_SHIFT_VALUE	= 5;
const int	BIFF7_SHIFT_VALUE	= 3;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// the date strings to look for
const CHAR*	DateType[]		= { "dd", "mm", "yy", "hh", "ss", "m/", "/d", NULL	};
const CHAR*	NumberType[]	= { "#", "0", NULL	};


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXlFormatRecord
// constructor and destructor
CXlFormatRecord::CXlFormatRecord(LPCXlRecord pRecord, WORD wBIFF) : CXlRecord(pRecord), m_bDate(FALSE), m_bNumber(FALSE)
{
	int		nNumChars	= m_pData[2];
	WORD	wIndexCode	= *reinterpret_cast<WORD*>(m_pData);

	if ( wBIFF == BOOKTYPE_BIFF8 ) {
		SetDataNumberFlag(m_pData + BIFF8_SHIFT_VALUE);
	} else {
		SetDataNumberFlag(m_pData + BIFF7_SHIFT_VALUE);
	}
}

CXlFormatRecord::~CXlFormatRecord(void)
{
	DWORD dwError	= ::GetLastError();
}

void CXlFormatRecord::SetDataNumberFlag(CHAR* pString)
{
	std::basic_string<char>::size_type	NPos	= -1;
	std::string	FormatString(pString);

	int nIndex	= 0;
	while ( DateType[nIndex] ) {
		if ( NPos != FormatString.find_first_of(DateType[nIndex], 0) ) {
			m_bDate	= TRUE;
		}
		++nIndex;
	}
	// See if this is number format - look for the # or 0 characters
	if ( FALSE == m_bDate ) {
		nIndex	= 0;
		while ( NumberType[nIndex] ) {
			if ( NPos != FormatString.find_first_of(NumberType[nIndex], 0) ) {
				m_bNumber	= TRUE;
			}
			++nIndex;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXFRecord

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The list of built in date format values
const int	DateFormatIndex[]	= { 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x2d, 0x2e, 0x2f, 0x00 };
// The list of java date format equivalents
const CHAR* DateFormats[]		= { "M/d/yy", "d-MMM-yy", "d-MMM", "MMM-yy", "h:mm a", "h:mm:ss a", "H:mm", "H:mm:ss", "M/d/yy H:mm",
								"mm:ss", "H:mm:ss", "mm:ss.S", NULL };

 // The list of build in number format values
const int	NumberFormatIndex[]	= { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x30, 0x00	};

// The list of java number format equivalents
const CHAR*	NumberFormats[]		= { "0", "0.00", "#,##0", "#,##0.00", "$#,##0;($#,##0)", "$#,##0;($#,##0)", "$#,##0.00;($#,##0.00)",
									"$#,##0.00;($#,##0.00)", "0%", "0.00%", "0.00E00", "#,##0;(#,##0)", "#,##0;(#,##0)",
									"#,##0.00;(#,##0.00)", "#,##0.00;(#,##0.00)", "#,##0;(#,##0)", "$#,##0;($#,##0)", "#,##0.00;(#,##0.00)",
									"$#,##0.00;($#,##0.00)",  "##0.0E0", NULL };

// constructor and destructor
CXFRecord::CXFRecord(void) : m_bDate(FALSE), m_bNumber(FALSE), m_nFormatRecord(0), m_pDateFormat(NULL), m_pNumberFormat(NULL)
{
}

CXFRecord::CXFRecord(LPCXlRecord pRecord) : CXlRecord(pRecord), 
m_bDate(FALSE), m_bNumber(FALSE), m_nFormatRecord(0), m_pDateFormat(NULL), m_pNumberFormat(NULL)
{
	m_nFormatRecord	= *reinterpret_cast<WORD*>(m_pData + 2);

	int nIndex;
	// compare against the date formats
	for ( nIndex = 0; DateFormatIndex[nIndex]; ++nIndex ) {
		if ( m_nFormatRecord == DateFormatIndex[nIndex] ) {
			m_bDate			= TRUE;
			m_pDateFormat	= const_cast<CHAR*>(DateFormats[nIndex]);
			break;
		}
	}

	// compare against the number formats
	for ( nIndex = 0; NumberFormatIndex[nIndex]; ++nIndex ) {
		if ( m_nFormatRecord == NumberFormatIndex[nIndex] ) {
			m_bNumber		= TRUE;
			m_pNumberFormat	= const_cast<CHAR*>(NumberFormats[nIndex]);
			break;
		}
	}
}

CXFRecord::~CXFRecord(void)
{
}

void CXFRecord::SetDataNumberFlag(CHAR* pString)
{
	std::basic_string<char>::size_type	NPos	= -1;
	std::string	FormatString(pString);

	int nIndex	= 0;
	while ( DateType[nIndex] ) {
		if ( NPos != FormatString.find_first_of(DateType[nIndex], 0) ) {
			m_bDate	= TRUE;
		}
		++nIndex;
	}
	// see if this is number format - look for the # or 0 characters
	if ( FALSE == m_bDate ) {
		nIndex	= 0;
		while ( DateType[nIndex] ) {
			if ( NPos != FormatString.find_first_of(NumberType[nIndex], 0) ) {
				m_bNumber	= TRUE;
			}
			++nIndex;
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CXlSSTRecord::CXlSSTRecord(LPCXlRecord pRecord, ContinueRecordVector& RecordVector)
{
	m_ContinueBreaks.clear();
	m_UniqueStrings.clear();

	// concatenate everything into one big bugger of a byte array
	int nTotalRecordLength = 0;

	for ( ContinueRecordVector::size_type i=0; i<RecordVector.size(); ++i ) {
		LPCXlRecord	pRecord	= RecordVector[i];
		nTotalRecordLength	+= pRecord->m_wLength;
	}
	nTotalRecordLength	+= pRecord->m_wLength;

	CHAR* pData = new CHAR[nTotalRecordLength];

	// first the original data gets put in
	int nPos = 0;
	::memcpy(pData, pRecord->m_pData, pRecord->m_wLength);
	nPos += pRecord->m_wLength;

	// now copy in everything else.
	for ( i=0; i < RecordVector.size(); i++ ) {
		LPCXlRecord	pRecordInVector	= RecordVector[i];
		::memcpy(pData + nPos, pRecordInVector->m_pData, pRecordInVector->m_wLength);
		m_ContinueBreaks.push_back(nPos);
		nPos += pRecordInVector->m_wLength;
	}

	// get total strings and unique strings
	m_nTotalStrings		= *reinterpret_cast<int*>(pData);
	m_nUniqueStrings	= *reinterpret_cast<int*>(pData + sizeof(m_nTotalStrings));

	ReadUniqueStrings(pData, nTotalRecordLength, 2*sizeof(int));

	SAFE_DELETE_ARRAY(pData);
}

CXlSSTRecord::~CXlSSTRecord(void)
{
	while ( m_ContinueBreaks.size() ) {
		m_ContinueBreaks.erase(m_ContinueBreaks.begin());
	}

	while ( m_UniqueStrings.size() ) {
		CHAR*	pUniqueString	= m_UniqueStrings[0];
		m_UniqueStrings.erase(m_UniqueStrings.begin());
		SAFE_DELETE_ARRAY(pUniqueString);
	}
}

void CXlSSTRecord::ReadUniqueStrings(CHAR* pData, int nDataLen, int nOffset)
{
	int	nPos				= nOffset;

	int	nFormattingRuns		= 0, 
		nExtendedRunLength	= 0;


	for ( int i=0; i<m_nUniqueStrings; ++i ) {
		// read in the number of characters
		int		nNumChars		= *reinterpret_cast<WORD*>(pData + nPos);
		nPos					+= sizeof(WORD);
		BYTE	byOptionFlag	= pData[nPos];
		++nPos;

		BOOL	bExtendedString	= ( 0 != ( byOptionFlag & 0x04 ) );	// see if it is an extended string
		BOOL	bRichString		= ( 0 != ( byOptionFlag & 0x08 ) );	// see if string contains formatting information

		if ( bRichString ) {		// read in the crun 
			nFormattingRuns		= *reinterpret_cast<WORD*>(pData + nPos);
			nPos += sizeof(WORD);
		}
		if ( bExtendedString ) {	// read in cchExtRst
			nExtendedRunLength	= *reinterpret_cast<int*>(pData + nPos);
			nPos += sizeof(int);
		}

		// see if string is ASCII (compressed) or unicode
		BOOL	bASCIIEncoding = ( 0 == (byOptionFlag & 0x01) );

		BOOL	bHolder	= bASCIIEncoding;
		CHAR*	pHolder	= NULL;
		nPos += GetChars(pData, &pHolder, nPos, bHolder, nNumChars);
		bASCIIEncoding	= bHolder;

		if ( bASCIIEncoding ) {
			m_UniqueStrings.push_back(pHolder);			//s = new String(pHolder);
		} else {
			char	HoldBuffer[MAX_PATH];
			int		nStringLen = WideCharToMultiByte(CP_ACP, 0, reinterpret_cast<LPCWSTR>(pHolder), nNumChars, 
													 HoldBuffer, MAX_PATH, NULL, NULL);
			HoldBuffer[nStringLen]	= '\0';

			SAFE_DELETE_ARRAY(pHolder);
			pHolder	= new CHAR[nStringLen + 1];
			::memcpy(pHolder, HoldBuffer, nStringLen + 1);

			m_UniqueStrings.push_back(pHolder);
		}

		// for rich strings, skip over the formatting runs
		if ( bRichString )
			nPos += ( 4 * nFormattingRuns );

		// for extended strings, skip over the extended string pData
		if ( bExtendedString )
			nPos += nExtendedRunLength;

		if ( nPos > nDataLen ) {
			// TODO : 에러 로그 처리 및 에러 처리 - "position exceeds record length"
			//Assert.assert(false, "position exceeds record length");
			return;
		}
	}
}

int CXlSSTRecord::GetChars(CHAR* pSource, OUT CHAR** ppDest, int nPos, BOOL bASCII, int nNumChars)
{
	BOOL	bSpansBreak = FALSE;

	int		nDestLen;
	if ( bASCII )
		nDestLen = nNumChars;
	else
		nDestLen = nNumChars * 2;

	*ppDest = new CHAR[nDestLen + 1];

	ContinueBreakVector::size_type	i	= 0;
	while ( ( i < m_ContinueBreaks.size() ) &&  ( bSpansBreak == FALSE ) ) {
		bSpansBreak = nPos < m_ContinueBreaks[i] && ( nPos + nDestLen > m_ContinueBreaks[i] );

		if ( !bSpansBreak )
			++i;
	}

	// if it doesn't span a break simply do an array copy into the destination array and finish
	if ( FALSE == bSpansBreak ) {
		::memcpy(*ppDest, pSource + nPos, nDestLen);
		(*ppDest)[nDestLen]	= '\0';
		return nDestLen;
	}

	// copy the portion before the break nPos into the array
	int nBreakPos = m_ContinueBreaks[i];
	::memcpy(*ppDest, pSource + nPos, nBreakPos - nPos);

	int nBytesRead = nBreakPos - nPos;
	int nCharsRead;
	if ( bASCII )
		nCharsRead = nBytesRead;
	else
		nCharsRead = nBytesRead / 2;

	//String tmp = new String(pDest);
	nBytesRead += GetContinuedString(pSource, ppDest, nBytesRead, static_cast<int>(i), bASCII, nNumChars - nCharsRead);

	//tmp = new String(pDest);
	return nBytesRead;
}

int CXlSSTRecord::GetContinuedString(CHAR* pSource, OUT CHAR** ppDest, int nDestPos, int nContBreakIndex, BOOL bASCII, int nCharsLeft)
{
	int nBreakPos	= m_ContinueBreaks[nContBreakIndex];
	int nBytesRead	= 0;

	while ( nCharsLeft > 0 ) {
		if ( static_cast<ContinueBreakVector::size_type>(nContBreakIndex) >= m_ContinueBreaks.size() ) {
			// TODO : 에러 로그 처리 - "continuation break index"
			return -1;
		}

		if ( bASCII && ( 0 == pSource[nBreakPos] ) ) {
			// the string is consistently bASCII throughout
			int nLength = nContBreakIndex == (m_ContinueBreaks.size() - 1)? nCharsLeft: MIN(nCharsLeft, m_ContinueBreaks[nContBreakIndex+1]-nBreakPos-1);

			::memcpy(*ppDest + nDestPos, pSource + nBreakPos + 1, nLength);

			nDestPos	+= nLength;
			nBytesRead	+= (nLength + 1);
			nCharsLeft	-= nLength;
			bASCII		= TRUE;

		} else if ( !bASCII && ( 0 != pSource[nBreakPos] ) ) {
			// the string is Unicode throughout
			int nLength = (nContBreakIndex == m_ContinueBreaks.size() - 1)? nCharsLeft*2: MIN(nCharsLeft*2, m_ContinueBreaks[nContBreakIndex+1]-nBreakPos-1);

			// it looks like the string continues as Unicode too.  That's handy
			::memcpy(*ppDest + nDestPos, pSource + nBreakPos + 1, nLength);

			nDestPos	+= nLength;
			nBytesRead	+= nLength + 1;
			nCharsLeft	-= nLength / 2;
			bASCII		= FALSE;

		} else if ( !bASCII && ( 0 == pSource[nBreakPos] ) ) {
			// bummer - the string starts off as Unicode, but after the continuation it is in straightforward ASCII encoding
			int nChars = (nContBreakIndex == m_ContinueBreaks.size() - 1)? nCharsLeft: MIN(nCharsLeft, m_ContinueBreaks[nContBreakIndex + 1]-nBreakPos-1);

			for (int j = 0 ; j < nChars; j++) {
				(*ppDest)[nDestPos] = pSource[nBreakPos + j + 1];
				nDestPos += 2;
			}

			nBytesRead	+= nChars + 1 ;
			nCharsLeft	-= nChars;
			bASCII		= FALSE;

		} else {
			// double bummer - the string starts off as ASCII, but after the continuation it is in Unicode. 
			// this impacts the allocated array

			// reallocate what we have of the byte array so that it is all Unicode
			CHAR*	pOldBytes = *ppDest;
			*ppDest = new CHAR[nDestPos * 2 + nCharsLeft * 2];
			for ( int j=0; j<nDestPos; ++j ) {
				*ppDest[j * 2] = pOldBytes[j];
			}
			nDestPos	= nDestPos * 2;

			SAFE_DELETE_ARRAY(pOldBytes);

			int nLength = nContBreakIndex == (m_ContinueBreaks.size() - 1)? nCharsLeft*2: MIN(nCharsLeft*2, m_ContinueBreaks[nContBreakIndex+1]-nBreakPos-1);

			::memcpy(*ppDest + nDestPos, pSource + nBreakPos + 1, nLength);

			nDestPos	+= nLength;
			nBytesRead	+= ( nLength + 1 );
			nCharsLeft	-= ( nLength / 2 );
			bASCII		= FALSE;
		}    
		nContBreakIndex++;
	}

	return nBytesRead;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CXlBoundsheetRecord::CXlBoundsheetRecord(LPCXlRecord pRecord, WORD wBIFF) : CXlRecord(pRecord)
{
	m_nOffset			= *reinterpret_cast<int*>(pRecord->m_pData);
	m_byTypeFlag		= pRecord->m_pData[4];
	m_byVisibilityFlag	= pRecord->m_pData[5];
	m_nLength			= pRecord->m_pData[6];

	if ( BOOKTYPE_BIFF8 == wBIFF ) {
		if ( 0 == pRecord->m_pData[7] ) {
			// standard ASCII encoding
			m_pName = new CHAR[m_nLength + 1];
			::memcpy(m_pName, pRecord->m_pData + 8, m_nLength);
			m_pName[m_nLength]	= '\0';
		} else {
			// little endian Unicode encoding
			char	HoldBuffer[MAX_PATH];
			int		nStringLen = WideCharToMultiByte(CP_ACP, 0, reinterpret_cast<LPCWSTR>(pRecord->m_pData + 8), m_nLength, 
														HoldBuffer, MAX_PATH, NULL, NULL);
			HoldBuffer[nStringLen]	= '\0';
			m_pName	= new CHAR[nStringLen + 1];
			::strncpy(m_pName, HoldBuffer, nStringLen + 1);
		}
	} else {
		m_pName = new CHAR[m_nLength];
		::memcpy(m_pName, pRecord->m_pData + 7, m_nLength);
	}
}

CXlBoundsheetRecord::~CXlBoundsheetRecord(void)
{
	SAFE_DELETE_ARRAY(m_pName);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CXlNameRecord
CXlNameRecord::CXlNameRecord(LPCXlRecord pRecord) : CXlRecord(pRecord)
{
}

CXlNameRecord::~CXlNameRecord(void)
{
	SAFE_DELETE_ARRAY(m_pName);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CXlMulRK::CXlMulRK(void) : m_nRow(0), m_nFirst(0), m_nLast(0), m_nNumRKs(0), m_pRKFloats(NULL), m_pXFIndices(NULL)
{
}

const int	ROWINDEXOFFSET_IN_MULRK				= 0;
const int	FIRSTCOLUMNINDEXOFFSET_IN_MULRK		= 2;
const int	XFRKSTRUCTUREOFFSET_IN_MULRK		= 4;
const int	XFRKSTRUCTUREINDEXOFFSET_IN_MULRK	= 2;	// offset of index in the XF/RK list
const int	XFRKSTRUCTURESIZE_IN_MULRK			= 6;	// size of XF/RK list node

CXlMulRK::CXlMulRK(LPCXlRecord pRecord) : CXlRecord(pRecord), m_nRow(0), m_nFirst(0), m_nLast(0), m_nNumRKs(0), m_pRKFloats(NULL), m_pXFIndices(NULL)
{
	WORD	wRow	= *reinterpret_cast<WORD*>(pRecord->m_pData + ROWINDEXOFFSET_IN_MULRK);
	WORD	wFirst	= *reinterpret_cast<WORD*>(pRecord->m_pData + FIRSTCOLUMNINDEXOFFSET_IN_MULRK);
	WORD	wLast	= *reinterpret_cast<WORD*>(pRecord->m_pData + pRecord->m_wLength - 2);

	m_nRow			= static_cast<int>(wRow);
	m_nFirst		= static_cast<int>(wFirst);
	m_nLast			= static_cast<int>(wLast);
	m_nNumRKs		= m_nLast - m_nFirst + 1;

	int	nOffset		= XFRKSTRUCTUREOFFSET_IN_MULRK;
	m_pXFIndices	= new int[m_nNumRKs];
	m_pRKFloats		= new DWORD[m_nNumRKs];
	for ( int i=0; i<m_nNumRKs; ++i ) {
		WORD	wXFIndex	= *reinterpret_cast<WORD*>(pRecord->m_pData + nOffset);
		m_pXFIndices[i]		= static_cast<int>(wXFIndex);
		DWORD	dwValue		= *reinterpret_cast<LPDWORD>(pRecord->m_pData + nOffset + XFRKSTRUCTUREINDEXOFFSET_IN_MULRK);
		m_pRKFloats[i]		= dwValue;
		nOffset	+= XFRKSTRUCTURESIZE_IN_MULRK;
	}
}

CXlMulRK::~CXlMulRK(void)
{
	SAFE_DELETE_ARRAY(m_pXFIndices);
	SAFE_DELETE_ARRAY(m_pRKFloats);
}
