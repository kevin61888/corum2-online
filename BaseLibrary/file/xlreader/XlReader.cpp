#include "../../StdAfx.h"
#include "XlReader.h"

//#include <BaseLibrary/Utility/Debugging/DebugUtils.h>
//#define new New


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CXlReader::CXlReader(void) : m_by1904(0), /*m_nDataPos(0), */m_pXlSST(NULL)
{
}

CXlReader::~CXlReader(void)
{
	Terminate();
}

BOOL CXlReader::Load(const CHAR* szFileName)
{
	if ( FALSE == m_File.Open(szFileName) ) {
		return FALSE;
	} else {
		if ( FALSE == SeekBOF() )
			return FALSE;

		ParseSheets();

		CreateSheets();

		return TRUE;
	}
}

BOOL CXlReader::SeekBOF(void)
{
	WORD	wOPCode;
	int		nReadLen;
	while ( ( nReadLen	= m_File.Read(&wOPCode, sizeof(wOPCode)) ) >= sizeof(wOPCode)) {
		if ( RECORDTYPE_BOF == wOPCode )
			return TRUE;
	}

	// TODO : 에러 로그 처리

	return FALSE;
}

void CXlReader::ParseSheets(void)
{
	int	nOffset	= sizeof(WORD);
	m_File.Seek(-nOffset, CLowFile::seekCurrent);
	LPCXlBOFRecord	pBOFRecord	= GetBOFRecord();
	LPCXlRecord		pRecord		= NULL;

	BOOL	bLoop	= TRUE;
	while ( bLoop ) {
		pRecord = GetRecordFromFile();
		switch ( pRecord->m_wType ) {
		case RECORDTYPE_SST:
			m_pXlSST	= ProcessSST(pRecord);
			break;
		case RECORDTYPE_FILEPASS:
			// throw new BiffException(BiffException.passwordProtected);
			break;
		case RECORDTYPE_TXO:
			break;
		case RECORDTYPE_NAME:
			{
				CXlNameRecord pRecord = new CXlNameRecord(pRecord);
			} break;
		case RECORDTYPE_ROW:
			break;
		case RECORDTYPE_NINETEENFOUR:
			m_by1904	= pRecord->m_pData[0]? 1: 0;
			break;
		case RECORDTYPE_FORMAT:
			m_XlRecords.push_back(new CXlFormatRecord(pRecord, pBOFRecord->m_wVersion));
			break;
		case RECORDTYPE_XF:
			m_XlRecords.push_back(new CXFRecord(pRecord));
			break;
		case RECORDTYPE_BOUNDSHEET:
			m_XlBoundSheets.push_back(new CXlBoundsheetRecord(pRecord, pBOFRecord->m_wVersion));
			break;
		case RECORDTYPE_EXTSST:
			break;
		case RECORDTYPE_CONTINUE:
			break;;
		case RECORDTYPE_EOF:
			bLoop	= FALSE;
			break;
		default:
			break;
		}

		SAFE_DELETE(pRecord);

	}

	SAFE_DELETE(pBOFRecord);
}

void CXlReader::CreateSheets(void)
{
	LPCXlRecord		pRecord = GetRecordFromFile();
	if ( RECORDTYPE_BOF != pRecord->m_wType ) {
		// TODO : 에러 로그 처리
		return;
	}
	LPCXlBOFRecord	pBOFRecord	= new CXlBOFRecord(pRecord);
	SAFE_DELETE(pRecord);

	while ( pBOFRecord && pBOFRecord->IsWorkSheet() ) {
		if ( ( FALSE == pBOFRecord->IsBIFF7() ) && ( FALSE == pBOFRecord->IsBIFF8() ) ) {
			// TODO : 에러 로그 처리
	        return;
		}

		LPCXlSheet pSheetImpl = new CXlSheet;

		int	nBOFs	= 1;
		while ( nBOFs >= 1 ) {
			pRecord = GetRecordFromFile();

			// use this form for quick performance
			WORD wType	= pRecord->m_wType;
			switch ( wType ) {
			case RECORDTYPE_EOF:
				--nBOFs;
				break;
			case RECORDTYPE_BOF:
				++nBOFs;
				break;
			case RECORDTYPE_XF:
				break;
			case RECORDTYPE_DIMENSION:
				pSheetImpl->InitiateCells(pRecord);
				break;
			case RECORDTYPE_LABELSST:
				pSheetImpl->SetLabelSST(pRecord, m_pXlSST);
				break;
			case RECORDTYPE_RK:
			case RECORDTYPE_RK2:
				pSheetImpl->SetCellRK(pRecord);
				break;
			case RECORDTYPE_MULRK:
				pSheetImpl->SetCellMulRK(pRecord);
				break;
			case RECORDTYPE_ROW:
				break;
			case RECORDTYPE_NUMBER:
				pSheetImpl->SetNumber(pRecord);
				break;
			case RECORDTYPE_BOOLERR:
				break;
			case RECORDTYPE_CONTINUE:
				break;
			case RECORDTYPE_FORMULA:
			case RECORDTYPE_FORMULA2:
				pSheetImpl->SetFormula(pRecord);
				break;
			case RECORDTYPE_LABEL:
				break;
			case RECORDTYPE_NAME:
				pSheetImpl->SetName(pRecord);
				break;
			default:
				break;
			}

			SAFE_DELETE(pRecord);
		}
		// set the name on the sheet
		LPCXlBoundsheetRecord pBoundsheetRecord = m_XlBoundSheets[m_XlSheets.size()];
		pSheetImpl->SetSheetName(pBoundsheetRecord->m_pName);
		m_XlSheets.push_back(pSheetImpl);

		// The next record will normally be a BOF or empty padding until the end of the block is reached.
		// In exceptionally unlucky cases, the last EOF  will coincide with a block division, so we have to
		// check there is more data to retrieve. Thanks to liamg for spotting this
		SAFE_DELETE(pBOFRecord);

		if (pRecord	= GetRecordFromFile()) {
			if ( RECORDTYPE_BOF == pRecord->m_wType ) {
				pBOFRecord = new CXlBOFRecord(pRecord);
			}

			SAFE_DELETE(pRecord);
		}
	}
	SAFE_DELETE(pBOFRecord);
}

CXlBOFRecord* CXlReader::GetBOFRecord(void)
{
	if ( m_File.IsEOF() )
		return NULL;

	LPCXlBOFRecord	pBOFRecord = new CXlBOFRecord;

	int		nReadLen;
	if ( ( nReadLen	= m_File.Read(&pBOFRecord->m_wType, sizeof(pBOFRecord->m_wType)) ) < sizeof(pBOFRecord->m_wType)) {
		// TODO : 에러 로그 처리
		return NULL;
	}
	if ( ( nReadLen	= m_File.Read(&pBOFRecord->m_wLength, sizeof(pBOFRecord->m_wLength)) ) < sizeof(pBOFRecord->m_wLength)) {
		// TODO : 에러 로그 처리
		return NULL;
	}

	pBOFRecord->m_pData	= new CHAR[pBOFRecord->m_wLength];
	if ( ( nReadLen	= m_File.Read(pBOFRecord->m_pData, pBOFRecord->m_wLength) ) < pBOFRecord->m_wLength) {
		// TODO : 에러 로그 처리
		SAFE_DELETE(pBOFRecord);		// SAFE_DELETE_ARRAY(pBOFRecord->m_pData);
		return NULL;
	}

	pBOFRecord->m_wVersion		= *reinterpret_cast<WORD*>(pBOFRecord->m_pData);
	pBOFRecord->m_wStreamType	= *reinterpret_cast<WORD*>(pBOFRecord->m_pData + sizeof(pBOFRecord->m_wVersion));

	//*//
	int	nDataPos	= m_File.Seek(0, CLowFile::seekCurrent);
	pBOFRecord->m_nPosition	+= ( nDataPos + sizeof(pBOFRecord->m_wType) + sizeof(pBOFRecord->m_wLength) );
	/*/
	pBOFRecord->m_nPosition	+= ( m_nDataPos + sizeof(pBOFRecord->m_wType) + sizeof(pBOFRecord->m_wLength) );
	m_nDataPos				= ( pBOFRecord->m_nPosition + pBOFRecord->m_wLength );
	//*/

	return pBOFRecord;
}

CXlRecord* CXlReader::GetRecordFromFile(void)
{
	if ( m_File.IsEOF() )
		return NULL;

	LPCXlRecord	pRecord		= new CXlRecord;
	m_File.Read(&pRecord->m_wType, sizeof(pRecord->m_wType));
	m_File.Read(&pRecord->m_wLength, sizeof(pRecord->m_wLength));

	//*/
	int	nDataPos	= m_File.Seek(0, CLowFile::seekCurrent);
	pRecord->m_nPosition	= ( nDataPos + sizeof(pRecord->m_wType) + sizeof(pRecord->m_wLength) );
	/*/
	pRecord->m_nPosition	= ( m_nDataPos + sizeof(pRecord->m_wType) + sizeof(pRecord->m_wLength) );
	//*/
	pRecord->m_pData		= new CHAR[pRecord->m_wLength];
	if ( pRecord->m_wLength != m_File.Read(pRecord->m_pData, pRecord->m_wLength) ) {
		SAFE_DELETE(pRecord);
		return NULL;
	}

	//m_nDataPos				= ( pRecord->m_nPosition + pRecord->m_wLength );

	return pRecord;
}

LPCXlSSTRecord CXlReader::ProcessSST(LPCXlRecord pRecord)
{
	ContinueRecordVector	ContinueRecords;

	LPCXlRecord	pContinueRecord = GetRecordFromFile();
	while ( RECORDTYPE_CONTINUE == pContinueRecord->m_wType ) {
		ContinueRecords.push_back(pContinueRecord);
		pContinueRecord	= GetRecordFromFile();
	}
	SAFE_DELETE(pContinueRecord);

	LPCXlSSTRecord pSST = new CXlSSTRecord(pRecord, ContinueRecords);

	while ( ContinueRecords.size() ) {
		LPCXlRecord	pContinueRecord	= ContinueRecords[0];
		ContinueRecords.erase(ContinueRecords.begin());
		SAFE_DELETE(pContinueRecord);
	}

	return pSST;
}

void CXlReader::Terminate(void)
{
	for ( XlRecordVector::size_type nRecordIndex=0; nRecordIndex<m_XlRecords.size(); ++nRecordIndex )
		SAFE_DELETE(m_XlRecords[nRecordIndex]);
	m_XlRecords.clear();

	for ( XlBoundSheetVector::size_type nBSIndex=0; nBSIndex<m_XlBoundSheets.size(); ++nBSIndex )
		SAFE_DELETE(m_XlBoundSheets[nBSIndex]);
	m_XlBoundSheets.clear();

	for ( XlSheetVector::size_type nSheetIndex=0; nSheetIndex<m_XlSheets.size(); ++nSheetIndex )
		SAFE_DELETE(m_XlSheets[nSheetIndex]);
	m_XlSheets.clear();

	SAFE_DELETE(m_pXlSST);
}
