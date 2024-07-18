#pragma once

#include <vector>

#include <BaseLibrary/File/XlReader/XlCell.h>


typedef class CXlHeadNode*		LPCXlHeadNode;
class CXlHeadNode
{
public:
	// constructor and destructor
	CXlHeadNode(void);
	CXlHeadNode(LPCXlCell pXlCell);
	virtual ~CXlHeadNode(void);

	enum { MAX_TITLE_LEN	= 128	};
	CHAR	m_Title[CXlHeadNode::MAX_TITLE_LEN];

	typedef std::vector<LPCXlHeadNode>	XlHeadNodeVector;
	XlHeadNodeVector		m_HeadNodes;

	LPCXlCell				m_pXlCell;
};
