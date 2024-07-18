#include "../../StdAfx.h"
#include "XlSheet.h"
#include "XlRecord.h"

//#include <BaseLibrary/Utility/Debugging/DebugUtils.h>
//#define new New


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CXlSheet::CXlSheet(void) : m_nRowMaxNum(0), m_nColMaxNum(0)
{
}

CXlSheet::~CXlSheet(void)
{
	TerminateHeadline();

	SAFE_DELETE_ARRAY(m_pCells);
}

const int	DIMENSIONSIZE_LESSTHAN_BIFF8VERSION				= 10;
const int	ROWNUMOFFSET_INDIMENSION_LESSTHAN_BIFF8VERSION	= 2;
const int	COLNUMOFFSET_INDIMENSION_LESSTHAN_BIFF8VERSION	= 6;
const int	ROWNUMOFFSET_INDIMENSION_HIGHTHAN_BIFF8VERSION	= 4;
const int	COLNUMOFFSET_INDIMENSION_HIGHTHAN_BIFF8VERSION	= 10;
BOOL CXlSheet::InitiateCells(LPCXlRecord pRecord)
{
	if ( DIMENSIONSIZE_LESSTHAN_BIFF8VERSION == pRecord->m_wLength ) {
		WORD	wRows = *reinterpret_cast<WORD*>(pRecord->m_pData + ROWNUMOFFSET_INDIMENSION_LESSTHAN_BIFF8VERSION);
		WORD	wCols = *reinterpret_cast<WORD*>(pRecord->m_pData + COLNUMOFFSET_INDIMENSION_LESSTHAN_BIFF8VERSION);
		SetRowMaxNum(static_cast<int>(wRows));
		SetColMaxNum(static_cast<int>(wCols));
	} else {
		int		nRows = *reinterpret_cast<int*>(pRecord->m_pData  + ROWNUMOFFSET_INDIMENSION_HIGHTHAN_BIFF8VERSION);
		WORD	wCols = *reinterpret_cast<WORD*>(pRecord->m_pData + COLNUMOFFSET_INDIMENSION_HIGHTHAN_BIFF8VERSION);
		SetRowMaxNum(static_cast<int>(nRows));
		SetColMaxNum(static_cast<int>(wCols));
	}

	if ( m_pCells	= new CXlCell[ GetRowMaxNum() * GetColMaxNum() ] )
		return TRUE;
	else
		return FALSE;
}

const int	ROWINDEXOFFSET_IN_RK	= 0;
const int	COLINDEXOFFSET_IN_RK	= 2;
const int	XFINDEXOFFSET_IN_RK		= 4;
const int	RKVALUEOFFSET_IN_RK		= 6;
void CXlSheet::SetCellRK(LPCXlRecord pRecord)
{
	WORD	wRow		= *reinterpret_cast<WORD*>(pRecord->m_pData + ROWINDEXOFFSET_IN_RK);
	WORD	wCol		= *reinterpret_cast<WORD*>(pRecord->m_pData + COLINDEXOFFSET_IN_RK);
	WORD	wXFIndex	= *reinterpret_cast<WORD*>(pRecord->m_pData + XFINDEXOFFSET_IN_RK);

	LPCXlCell	pCell	= GetCell(static_cast<int>(wRow), static_cast<int>(wCol));
	pCell->m_nType		= CXlCell::CellType_Number;
	pCell->m_nRow		= static_cast<int>(wRow);
	pCell->m_nCol		= static_cast<int>(wCol);
	pCell->m_nXFIndex	= static_cast<int>(wXFIndex);
	pCell->m_dwValue	=  *reinterpret_cast<LPDWORD>(pRecord->m_pData + RKVALUEOFFSET_IN_RK);
	pCell->SetValue();
}

void CXlSheet::SetCellMulRK(LPCXlRecord pRecord)
{
	CXlMulRK	MulRK(pRecord);

	for ( int i=0; i<MulRK.m_nNumRKs; ++i ) {
		LPCXlCell pCell	= GetCell(MulRK.m_nRow, MulRK.m_nFirst + i);
		pCell->m_nType		= CXlCell::CellType_Number;
		pCell->m_nRow		= MulRK.m_nRow;
		pCell->m_nCol		= MulRK.m_nFirst + i;
		pCell->m_nXFIndex	= MulRK.m_pXFIndices[i];
		pCell->m_dwValue	= MulRK.m_pRKFloats[i];
		pCell->SetValue();
	}
}

const int	ROWINDEXOFFSET_IN_NUMBER	= 0;
const int	COLINDEXOFFSET_IN_NUMBER	= 2;
const int	XFINDEXOFFSET_IN_NUMBER		= 4;
const int	VALUEOFFSET_IN_NUMBER		= 6;
void CXlSheet::SetNumber(LPCXlRecord pRecord)
{
	WORD wRow				= *reinterpret_cast<WORD*>(pRecord->m_pData + ROWINDEXOFFSET_IN_NUMBER);
	WORD wCol				= *reinterpret_cast<WORD*>(pRecord->m_pData + COLINDEXOFFSET_IN_NUMBER);
	WORD wXFIndex			= *reinterpret_cast<WORD*>(pRecord->m_pData + XFINDEXOFFSET_IN_NUMBER);

	LPCXlCell pCell	= GetCell(static_cast<int>(wRow), static_cast<int>(wCol));
	pCell->m_nType			= CXlCell::CellType_Number;
	pCell->m_nRow			= static_cast<int>(wRow);
	pCell->m_nCol			= static_cast<int>(wCol);
	pCell->m_nXFIndex		= static_cast<int>(wXFIndex);
	pCell->m_Value.dValue	= *reinterpret_cast<double*>(pRecord->m_pData + VALUEOFFSET_IN_NUMBER);
	pCell->m_ValueType		= CXlCell::CellValueType_Double;
}

const int	ROWINDEXOFFSET_IN_LABELSST	= 0;
const int	COLINDEXOFFSET_IN_LABELSST	= 2;
const int	XFINDEXOFFSET_IN_LABELSST	= 4;
const int	SSTRECORDINDEX_IN_LABELSST	= 6;
void CXlSheet::SetLabelSST(LPCXlRecord pRecord, LPCXlSSTRecord pSSTRecord)
{
	WORD wRow				=  *reinterpret_cast<WORD*>(pRecord->m_pData + ROWINDEXOFFSET_IN_LABELSST);
	WORD wCol				=  *reinterpret_cast<WORD*>(pRecord->m_pData + COLINDEXOFFSET_IN_LABELSST);
	int	 nIndex				=  *reinterpret_cast<int*>(pRecord->m_pData + SSTRECORDINDEX_IN_LABELSST);

	LPCXlCell	pCell		= GetCell(static_cast<int>(wRow), static_cast<int>(wCol));
	pCell->m_nType			= CXlCell::CellType_Label;
	pCell->m_nRow			= static_cast<int>(wRow);
	pCell->m_nCol			= static_cast<int>(wCol);
	pCell->m_Value.pValue	= pSSTRecord->m_UniqueStrings[nIndex];
}

const int	OPTIONFLAGOFFSET_IN_NAME	= 0;
const int	LENGTHOFFSET_IN_NAME		= 3;
const int	NAMEOFFSET_IN_NAME			= 15;
void CXlSheet::SetName(LPCXlRecord pRecord)
{
	int	nLength = static_cast<int>(pRecord->m_pData[LENGTHOFFSET_IN_NAME]);
	::strncpy(m_SheetName, pRecord->m_pData + NAMEOFFSET_IN_NAME, nLength);
}

const int	ROWINDEXOFFSET_IN_FORMULA			= 0;
const int	COLINDEXOFFSET_IN_FORMULA			= 2;
const int	XFINDEXOFFSET_IN_FORMULA			= 4;
const int	RESULTOFFSET_IN_FORMULA				= 6;		// formula 의 결과 값의 offset
const int	CHECKVALUEOFFSET_IN_FORMULARESULT	= 6;		// formula 의 결과 값 중 체크 값의 offset.
const int	OPTIONOFFSET_IN_FORMULA				= 14;
const int	DATAOFFSET_IN_FORMULA				= 20;


const int	FORMULARESULT_STRING				= 0x00;
const int	FORMULARESULT_BOOLEAN				= 0x01;
const int	FORMULARESULT_ERROR					= 0x02;
const int	FORMULARESULT_EMPTYCELL				= 0x03;

const WORD	FORMULARESULT_CHECKVALUE			= 0xFFFF;
void CXlSheet::SetFormula(LPCXlRecord pRecord)
{
	WORD wRow			= *reinterpret_cast<WORD*>(pRecord->m_pData + ROWINDEXOFFSET_IN_FORMULA);
	WORD wCol			= *reinterpret_cast<WORD*>(pRecord->m_pData + COLINDEXOFFSET_IN_FORMULA);
	WORD wXFIndex		= *reinterpret_cast<WORD*>(pRecord->m_pData + XFINDEXOFFSET_IN_FORMULA);
	WORD wCheckValue	= *reinterpret_cast<WORD*>(pRecord->m_pData + RESULTOFFSET_IN_FORMULA + CHECKVALUEOFFSET_IN_FORMULARESULT);

	if ( ( pRecord->m_pData[RESULTOFFSET_IN_FORMULA] == FORMULARESULT_STRING ) && 
		 ( FORMULARESULT_CHECKVALUE == wCheckValue ) ) {
		 // TODO : string 처리
	} else if ( ( pRecord->m_pData[RESULTOFFSET_IN_FORMULA] == FORMULARESULT_BOOLEAN ) && 
				( FORMULARESULT_CHECKVALUE == wCheckValue ) ) {
		 // TODO : boolean 처리
	} else if ( ( pRecord->m_pData[RESULTOFFSET_IN_FORMULA] == FORMULARESULT_ERROR ) && 
				( FORMULARESULT_CHECKVALUE == wCheckValue ) ) {
		 // TODO : 에러 처리
	} else {
		// it is most assuredly a number
		LPCXlCell	pCell		= GetCell(static_cast<int>(wRow), static_cast<int>(wCol));
		pCell->m_nType			= CXlCell::CellType_Number;
		pCell->m_nRow			= static_cast<int>(wRow);
		pCell->m_nCol			= static_cast<int>(wCol);
		pCell->m_Value.dValue	= *reinterpret_cast<double*>(pRecord->m_pData + RESULTOFFSET_IN_FORMULA);
		pCell->m_ValueType		= CXlCell::CellValueType_Double;
	}
}

void CXlSheet::TerminateHeadline(void)
{
	m_HeadLine.Terminate();
}

const int	DEFAULT_HEADLINE_COLUMN_INDEX	= 0;
BOOL CXlSheet::InitiateHeadline(void)
{
	TerminateHeadline();

	// 헤드로의 수 와 헤드칼럼의 길이 구하기
	for ( m_HeadLine.m_nHeadRow=1; m_HeadLine.m_nHeadRow<GetRowMaxNum(); ++m_HeadLine.m_nHeadRow )
		if ( LPCXlCell pCell = GetValidCell(m_HeadLine.m_nHeadRow, 0) )
			break;
	for ( m_HeadLine.m_nHeadCol=1; m_HeadLine.m_nHeadCol<GetColMaxNum(); ++m_HeadLine.m_nHeadCol )
		if ( LPCXlCell pCell = GetValidCell(0, m_HeadLine.m_nHeadCol) )
			break;

	// HeadCol 과 HeadRow 설정
	for ( int nHeadCol=m_HeadLine.m_nHeadCol; nHeadCol<GetColMaxNum(); ) {
		LPCXlHeadNode pHeadNode	= new CXlHeadNode(GetValidCell(0, nHeadCol));
		if ( m_HeadLine.m_nHeadCol > 1 ) {
			do {
				if ( LPCXlCell pDownCell = GetValidCell(1, nHeadCol++) )
					pHeadNode->m_HeadNodes.push_back( new CXlHeadNode(pDownCell) );
				else
					break;
			} while ( NULL == GetValidCell(0, nHeadCol) );
		} else
			nHeadCol++;

		m_HeadLine.m_HeadCol.push_back(pHeadNode);
	}

	for ( int nHeadRow=m_HeadLine.m_nHeadRow; nHeadRow<GetRowMaxNum(); ) {
		LPCXlHeadNode pHeadNode	= new CXlHeadNode(GetValidCell(nHeadRow, 0));
		if ( m_HeadLine.m_nHeadRow > 1 ) {
			do {
				if ( LPCXlCell pRightCell = GetValidCell(nHeadRow++, 1) )
					pHeadNode->m_HeadNodes.push_back( new CXlHeadNode(pRightCell) );
				else
					break;
			} while ( NULL == GetValidCell(nHeadRow, 0) );
		} else
			nHeadRow++;

		m_HeadLine.m_HeadRow.push_back(pHeadNode);
	}

	return TRUE;
}

LPCXlHeadNode CXlSheet::GetHeadRowOfCell(LPCXlCell pCell, int& rIndex)
{
	for ( unsigned int nHeadRow=0; nHeadRow<m_HeadLine.m_HeadRow.size(); ++nHeadRow ) {
		rIndex			= 0;
		if ( m_HeadLine.m_HeadRow[nHeadRow]->m_HeadNodes.size() > 0 ) {
			for ( unsigned int nSubRow=0; nSubRow<m_HeadLine.m_HeadRow[nHeadRow]->m_HeadNodes.size(); ++nSubRow ) {
				if ( m_HeadLine.m_HeadRow[nHeadRow]->m_HeadNodes[nSubRow]->m_pXlCell->m_nRow == pCell->m_nRow ) {
					return m_HeadLine.m_HeadRow[nHeadRow];
				}
				++rIndex;
			}
		}
	}

	return NULL;
}
