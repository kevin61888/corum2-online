#pragma once

#include <windows.h>
#include <vector>
#include <BaseLibrary/File/LowFile.h>

#include "XlSheet.h"
#include "XlRecord.h"


typedef class CXlReader*	LPCXlReader;
class CXlReader
{
private:
	CLowFile			m_File;
	BYTE				m_by1904;
	//int				m_nDataPos;
	LPCXlSSTRecord		m_pXlSST;

	typedef std::vector<LPCXlBoundsheetRecord>	XlBoundSheetVector;
	typedef std::vector<LPCXlSheet>				XlSheetVector;


	XlRecordVector		m_XlRecords;
	XlBoundSheetVector	m_XlBoundSheets;
	XlSheetVector		m_XlSheets;


	LPCXlBOFRecord		GetBOFRecord(void);
	LPCXlRecord			GetRecordFromFile(void);
	void				ClearContinueRecord(void);
	LPCXlSSTRecord		ProcessSST(LPCXlRecord pRecord);

	inline BOOL			Is1904(void);

	BOOL				SeekBOF(void);
	void				ParseSheets(void);
	void				CreateSheets(void);


public:
	// constructor and destructor
	CXlReader(void);
	virtual ~CXlReader(void);


	BOOL				Load(const CHAR* szFileName);
	void				Terminate(void);
	inline LPCXlSheet	GetSheet(const CHAR* szSheetName, BOOL bHeadline);
};

BOOL CXlReader::Is1904(void)
{
	return ( m_by1904? TRUE: FALSE );
}

LPCXlSheet CXlReader::GetSheet(const CHAR* szSheetName, BOOL bHeadline)
{
	// TODO : sheet를 읽으면서 헤드라인을 다시 정리할 것.

	for ( XlSheetVector::size_type i=0; i<m_XlSheets.size(); ++i ) {
		if ( TRUE == m_XlSheets[i]->CheckSheetName(szSheetName) ) {
			if ( bHeadline )
				m_XlSheets[i]->InitiateHeadline();		// headline initialize

			return m_XlSheets[i];
		}
	}

	return NULL;
}
