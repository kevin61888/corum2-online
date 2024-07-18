#pragma once

#include "XlHeadNode.h"


class CXlHeadLine
{
public:
	// constructor and destructor
	CXlHeadLine(void);
	virtual ~CXlHeadLine(void);


	typedef std::vector<LPCXlHeadNode>	XlHeadNodeVector;
	XlHeadNodeVector	m_HeadRow;
	XlHeadNodeVector	m_HeadCol;

	int				m_nHeadRow;
	int				m_nHeadCol;


	void			Terminate(void);

	LPCXlHeadNode	GetHeadRow(const CHAR* szTitle);
	LPCXlHeadNode	GetHeadRow(const CHAR* szTitle, const CHAR* szSubTitle);
	LPCXlHeadNode	GetHeadCol(const CHAR* szTitle);
	LPCXlHeadNode	GetHeadCol(const CHAR* szTitle, const CHAR* szSubTitle);
};
