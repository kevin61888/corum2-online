#pragma once

#include <vector>
#include <windows.h>

#include "./XlHeadLine/XlHeadLine.h"
#include "XlSheet.h"
#include "XlRecord.h"
#include "XlCell.h"

#include <vector>
typedef std::vector<LPCXlRecord>	XlRecordVector;


// CXlSheet
typedef class CXlSheet*		LPCXlSheet;
class CXlSheet
{
private:
	int					m_nRowMaxNum;		// the maximum number of rows
	int					m_nColMaxNum;		// the maximum number of columns

	CXlHeadLine			m_HeadLine;


	inline int			GetHeadLineRowNum(void)				{ return m_HeadLine.m_nHeadRow;		}
	inline int			GetHeadLineColNum(void)				{ return m_HeadLine.m_nHeadCol;		}


public:
	// constructor and destructor
	CXlSheet(void);
	virtual ~CXlSheet(void);

	enum { SheetNameLen	= 256	};
	CHAR				m_SheetName[CXlSheet::SheetNameLen];

	LPCXlCell			m_pCells;


	inline int			GetRowMaxNum(void)					{ return m_nRowMaxNum;				}
	inline int			GetColMaxNum(void)					{ return m_nColMaxNum;				}
	inline void			SetRowMaxNum(int nRowMaxNum)		{ m_nRowMaxNum	= nRowMaxNum;		}
	inline void			SetColMaxNum(int nColMaxNum)		{ m_nColMaxNum	= nColMaxNum;		}

	inline void			SetSheetName(const CHAR* pName);
	inline BOOL			CheckSheetName(const CHAR* pName);
	BOOL				InitiateCells(LPCXlRecord pRecord);
	void				SetCellRK(LPCXlRecord pRecord);
	void				SetCellMulRK(LPCXlRecord pRecord);
	void				SetNumber(LPCXlRecord pRecord);
	void				SetLabelSST(LPCXlRecord pRecord, LPCXlSSTRecord pSSTRecord);
	void				SetName(LPCXlRecord pRecord);
	void				SetFormula(LPCXlRecord pRecord);
	inline LPCXlCell	GetCell(int nRow, int nCol);
	inline LPCXlCell	GetValidCell(int nRow, int nCol);

	void				TerminateHeadline(void);
	BOOL				InitiateHeadline(void);

	//inline LPCXlCell	GetHeadRowCell(CHAR* szTitle);
	//inline LPCXlCell	GetHeadRowCell(CHAR* szTitle, CHAR* szSubTitle);
	//inline LPCXlCell	GetHeadColCell(CHAR* szTitle);
	//inline LPCXlCell	GetHeadColCell(CHAR* szTitle, CHAR* szSubTitle);
	inline int			GetDataRowNum(void);
	inline int			GetDataColNum(void);
	inline LPCXlCell	GetCellFromDataRow(const CHAR* szTitle, int nRow);
	inline LPCXlCell	GetCellFromDataRow(const CHAR* szTitle, const CHAR* szSubTitle, int nRow);
	LPCXlHeadNode		GetHeadRowOfCell(LPCXlCell pCell, int& rIndex);
};

void CXlSheet::SetSheetName(const CHAR* pName)
{
	::strncpy(m_SheetName, pName, CXlSheet::SheetNameLen);
	m_SheetName[strlen(pName)]	= '\0';
}

BOOL CXlSheet::CheckSheetName(const CHAR* pName)
{
	if ( 0 == ::strncmp(m_SheetName, pName, CXlSheet::SheetNameLen) )
		return TRUE;
	else
		return FALSE;
}

LPCXlCell CXlSheet::GetCell(int nRow, int nCol)
{
	return &m_pCells[ nRow*GetColMaxNum() + nCol ];
}

LPCXlCell CXlSheet::GetValidCell(int nRow, int nCol)
{
	if ( ( nRow < GetRowMaxNum() ) && ( nCol < GetColMaxNum() ) ) {
		LPCXlCell pCell	= &m_pCells[ nRow*GetColMaxNum() + nCol ];
		if ( CXlCell::CellType_None == pCell->m_nType )
			return NULL;
		else
			return pCell;
	} else
		return NULL;
}

//LPCXlCell CXlSheet::GetHeadRowCell(CHAR* szTitle)
//{
//	if ( LPCXlHeadNode pNode = m_HeadLine.GetHeadRow(szTitle) )
//		return pNode->m_pXlCell;
//	else
//		return NULL;
//}
//
//LPCXlCell CXlSheet::GetHeadRowCell(CHAR* szTitle, CHAR* szSubTitle)
//{
//	if ( LPCXlHeadNode pNode = m_HeadLine.GetHeadRow(szTitle, szSubTitle) )
//		return pNode->m_pXlCell;
//	else
//		return NULL;
//}
//
//LPCXlCell CXlSheet::GetHeadColCell(CHAR* szTitle)
//{
//	if ( LPCXlHeadNode pNode = m_HeadLine.GetHeadCol(szTitle) )
//		return pNode->m_pXlCell;
//	else
//		return NULL;
//}
//
//LPCXlCell CXlSheet::GetHeadColCell(CHAR* szTitle, CHAR* szSubTitle)
//{
//	if ( LPCXlHeadNode pNode = m_HeadLine.GetHeadCol(szTitle, szSubTitle) )
//		return pNode->m_pXlCell;
//	else
//		return NULL;
//}

// interface
int CXlSheet::GetDataRowNum(void)
{
	return ( GetRowMaxNum() - GetHeadLineRowNum() );
}

int CXlSheet::GetDataColNum(void)
{
	return ( GetColMaxNum() - GetHeadLineColNum() );
}

LPCXlCell CXlSheet::GetCellFromDataRow(const CHAR* szTitle, int nRow)
{
	nRow	+= m_HeadLine.m_nHeadRow;
	if ( ( nRow < m_HeadLine.m_nHeadRow ) || ( GetRowMaxNum() <= nRow ) )
		return NULL;

	if ( LPCXlHeadNode pNode = m_HeadLine.GetHeadCol(szTitle) ) {
		return GetValidCell(nRow, pNode->m_pXlCell->m_nCol);
	} else
		return NULL;
}

LPCXlCell CXlSheet::GetCellFromDataRow(const CHAR* szTitle, const CHAR* szSubTitle, int nRow)
{
	nRow	+= m_HeadLine.m_nHeadRow;
	if ( ( nRow < m_HeadLine.m_nHeadRow ) || ( GetRowMaxNum() <= nRow ) )
		return NULL;

	if ( LPCXlHeadNode pNode = m_HeadLine.GetHeadCol(szTitle, szSubTitle) ) {
		return GetValidCell(nRow, pNode->m_pXlCell->m_nCol);
	} else
		return NULL;
}
