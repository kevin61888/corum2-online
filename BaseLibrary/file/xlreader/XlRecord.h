#pragma once

#include <windows.h>
#include <vector>


// constants
const WORD	BOOKTYPE_BIFF8			= 0x0600;
const WORD	BOOKTYPE_BIFF7			= 0x0500;
const WORD	BOOKTYPE_WORKBOOKGLOBAL	= 0x0005;
const WORD	BOOKTYPE_WORKSHEET		= 0x0010;

const WORD	RECORDTYPE_BOF			= 0x0809;
const WORD	RECORDTYPE_EOF			= 0x000a;
const WORD	RECORDTYPE_BOUNDSHEET	= 0x0085;
const WORD	RECORDTYPE_DIMENSION	= 0x0200;
const WORD	RECORDTYPE_ROW			= 0x0208;
const WORD	RECORDTYPE_FILEPASS		= 0x002f;
const WORD	RECORDTYPE_NOTE			= 0x001c;
const WORD	RECORDTYPE_TXO			= 0x01b6;
const WORD	RECORDTYPE_RK			= 0x007e;
const WORD	RECORDTYPE_RK2			= 0x027e;
const WORD	RECORDTYPE_MULRK		= 0x00bd;
const WORD	RECORDTYPE_INDEX		= 0x020b;
const WORD	RECORDTYPE_SST			= 0x00fc;
const WORD	RECORDTYPE_EXTSST		= 0x00ff;
const WORD	RECORDTYPE_CONTINUE		= 0x003c;
const WORD	RECORDTYPE_LABEL		= 0x0204;
const WORD	RECORDTYPE_LABELSST		= 0x00fd;
const WORD	RECORDTYPE_NUMBER		= 0x0203;
const WORD	RECORDTYPE_NAME			= 0x0018;
const WORD	RECORDTYPE_ARRAY		= 0x0221;
const WORD	RECORDTYPE_STRING		= 0x0207;
const WORD	RECORDTYPE_FORMULA		= 0x0406;
const WORD	RECORDTYPE_FORMULA2		= 0x0006;
const WORD	RECORDTYPE_FORMAT		= 0x041e;
const WORD	RECORDTYPE_XF			= 0x00e0;
const WORD	RECORDTYPE_BOOLERR		= 0x0205;
const WORD	RECORDTYPE_UNKNOWN		= 0xffff;
const WORD	RECORDTYPE_NINETEENFOUR	= 0x0022;
const WORD	RECORDTYPE_BLANK		= 0x0201;


// CXlRecord
typedef class CXlRecord*		LPCXlRecord;
class CXlRecord
{
public:
	// constructor and destructor
	CXlRecord(void);
	CXlRecord(LPCXlRecord pRecord);
	virtual ~CXlRecord(void);

	WORD	m_wType;
	WORD	m_wLength;
	int		m_nPosition;
	CHAR*	m_pData;
};


// CXlBOFRecord
typedef class CXlBOFRecord*		LPCXlBOFRecord;
class CXlBOFRecord : public CXlRecord
{
public:
	// constructor and destructor
	CXlBOFRecord(void);
	CXlBOFRecord(LPCXlRecord pRecord);
	virtual ~CXlBOFRecord(void);

	WORD	m_wVersion;
	WORD	m_wStreamType;


	BOOL	IsWorkSheet(void)	{ return (m_wStreamType == BOOKTYPE_WORKSHEET);	}
	BOOL	IsBIFF7(void)		{ return (m_wVersion == BOOKTYPE_BIFF7);		}
	BOOL	IsBIFF8(void)		{ return (m_wVersion == BOOKTYPE_BIFF8);		}
};


// CXlFormatRecord
typedef class CXlFormatRecord*	LPCXlFormatRecord;
class CXlFormatRecord : public CXlRecord
{
private:
	BOOL		m_bDate;
	BOOL		m_bNumber;

	void		SetDataNumberFlag(CHAR* pString);

public:
	// constructor and destructor
	CXlFormatRecord(LPCXlRecord pRecord, WORD wBIFF);
	virtual ~CXlFormatRecord(void);
};


// CXFRecord
typedef class CXFRecord*	LPCXFRecord;
class CXFRecord : public CXlRecord
{
public:
	// constructor and destructor
	CXFRecord(void);
	CXFRecord(LPCXlRecord pRecord);
	virtual ~CXFRecord(void);

	BOOL		m_bDate;
	BOOL		m_bNumber;

	int			m_nFormatRecord;
	CHAR*		m_pDateFormat;
	CHAR*		m_pNumberFormat;

	void		SetDataNumberFlag(CHAR* pString);
};


typedef std::vector<LPCXlRecord>	ContinueRecordVector;


typedef class CXlSSTRecord*		LPCXlSSTRecord;
class CXlSSTRecord : public CXlRecord
{
private:
	int			GetContinuedString(CHAR* pSource, CHAR** ppDest, int nDestPos, int nContBreakIndex, BOOL bASCII, int nCharsLeft);
	int			GetChars(CHAR* pSource, OUT CHAR** ppDest, int nPos, BOOL bASCII, int nNumChars);
	void		ReadUniqueStrings(CHAR* pData, int nDataLen, int nOffset);


public:
	// constructor and destructor
	CXlSSTRecord(LPCXlRecord pRecord, ContinueRecordVector& RecordVector);
	virtual ~CXlSSTRecord(void);


	int			m_nTotalStrings;		// the total number of strings in this table
	int			m_nUniqueStrings;		// the number of unique strings

	typedef std::vector<int>	ContinueBreakVector;
	typedef std::vector<CHAR*>	UniqueStringVector;

	ContinueBreakVector	m_ContinueBreaks;
	UniqueStringVector	m_UniqueStrings;
};


// CBoundsheetRecord
typedef class CXlBoundsheetRecord*	LPCXlBoundsheetRecord;
class CXlBoundsheetRecord : public CXlRecord
{
public:
	// constructor and destructor
	CXlBoundsheetRecord(LPCXlRecord pRecord, WORD wBIFF);
	~CXlBoundsheetRecord(void);


	int			m_nOffset;
	BYTE		m_byTypeFlag;
	BYTE		m_byVisibilityFlag;
	int			m_nLength;

	CHAR*		m_pName;
};


// CXlNameRecord
typedef class CXlNameRecord*		LPCXlNameRecord;
class CXlNameRecord : public CXlRecord
{
public:
	// constructor and destructor
	CXlNameRecord(LPCXlRecord pRecord);
	~CXlNameRecord(void);


	CHAR*		m_pName;
};


// CXlMulRK
typedef class CXlMulRK*		LPCXlMulRK;
class CXlMulRK : public CXlRecord
{
public:
	// constructor and destructor
	CXlMulRK(void);
	CXlMulRK(LPCXlRecord pRecord);
	virtual ~CXlMulRK(void);


	int		m_nRow;
	int		m_nFirst;
	int		m_nLast;
	int		m_nNumRKs;
	DWORD*	m_pRKFloats;
	int*	m_pXFIndices;
};
