#include "../../../StdAfx.h"
#include "XlHeadNode.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CXlHeadNode::CXlHeadNode(void) : m_pXlCell(NULL)
{
	m_Title[0]	= '\0';
	m_HeadNodes.clear();
}

CXlHeadNode::CXlHeadNode(LPCXlCell pXlCell) : m_pXlCell(pXlCell)
{
	if ( CXlCell::CellType_Number == pXlCell->m_ValueType )
		::_snprintf(m_Title, CXlHeadNode::MAX_TITLE_LEN, "%d", m_pXlCell->m_Value.dValue);
	else
		::strncpy(m_Title, m_pXlCell->m_Value.pValue, CXlHeadNode::MAX_TITLE_LEN);
}

CXlHeadNode::~CXlHeadNode(void)
{
	for ( XlHeadNodeVector::size_type nIndex=0; nIndex<m_HeadNodes.size(); ++nIndex )
		SAFE_DELETE(m_HeadNodes[nIndex]);
	m_HeadNodes.clear();
}
