#include "../../../StdAfx.h"
#include "XlHeadLine.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CXlHeadLine::CXlHeadLine(void) : m_nHeadRow(0), m_nHeadCol(0)
{
}

CXlHeadLine::~CXlHeadLine(void)
{
}


void CXlHeadLine::Terminate(void)
{
	for ( XlHeadNodeVector::size_type nRowIndex=0; nRowIndex<m_HeadRow.size(); ++nRowIndex )
		SAFE_DELETE(m_HeadRow[nRowIndex]);
	m_HeadRow.clear();
	for ( XlHeadNodeVector::size_type nColIndex=0; nColIndex<m_HeadCol.size(); ++nColIndex )
		SAFE_DELETE(m_HeadCol[nColIndex]);
	m_HeadCol.clear();

	m_nHeadRow	= 0;
	m_nHeadCol	= 0;
}

LPCXlHeadNode CXlHeadLine::GetHeadRow(const CHAR* szTitle)
{
	for ( XlHeadNodeVector::size_type Title=0; Title<m_HeadRow.size(); ++Title ) {
		if ( 0 == strncmp(m_HeadRow[Title]->m_Title, szTitle, CXlHeadNode::MAX_TITLE_LEN) )
			return m_HeadRow[Title];
	}
	return NULL;
}

LPCXlHeadNode CXlHeadLine::GetHeadRow(const CHAR* szTitle, const CHAR* szSubTitle)
{
	for ( XlHeadNodeVector::size_type Title=0; Title<m_HeadRow.size(); ++Title ) {
		if ( 0 == strncmp(m_HeadRow[Title]->m_Title, szTitle, CXlHeadNode::MAX_TITLE_LEN) ) {
			for ( XlHeadNodeVector::size_type SubIndex=0; SubIndex<m_HeadRow[Title]->m_HeadNodes.size(); ++SubIndex ) {
				if ( 0 == strncmp(m_HeadRow[Title]->m_HeadNodes[SubIndex]->m_Title, szSubTitle, CXlHeadNode::MAX_TITLE_LEN) )
					return m_HeadRow[Title]->m_HeadNodes[SubIndex];
			}
			return NULL;
		}
	}
	return NULL;
}

LPCXlHeadNode CXlHeadLine::GetHeadCol(const CHAR* szTitle)
{
	for ( XlHeadNodeVector::size_type Title=0; Title<m_HeadCol.size(); ++Title ) {
		if ( 0 == strncmp(m_HeadCol[Title]->m_Title, szTitle, CXlHeadNode::MAX_TITLE_LEN) )
			return m_HeadCol[Title];
	}
	return NULL;
}

LPCXlHeadNode CXlHeadLine::GetHeadCol(const CHAR* szTitle, const CHAR* szSubTitle)
{
	for ( XlHeadNodeVector::size_type Title=0; Title<m_HeadCol.size(); ++Title ) {
		if ( 0 == strncmp(m_HeadCol[Title]->m_Title, szTitle, CXlHeadNode::MAX_TITLE_LEN) ) {
			for ( XlHeadNodeVector::size_type SubIndex=0; SubIndex<m_HeadCol[Title]->m_HeadNodes.size(); ++SubIndex ) {
				if ( 0 == strncmp(m_HeadCol[Title]->m_HeadNodes[SubIndex]->m_Title, szSubTitle, CXlHeadNode::MAX_TITLE_LEN) )
					return m_HeadCol[Title]->m_HeadNodes[SubIndex];
			}
			return NULL;
		}
	}
	return NULL;
}
