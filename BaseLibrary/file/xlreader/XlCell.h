#pragma once

#include <windows.h>


typedef class CXlCell*		LPCXlCell;
class CXlCell
{
public:
	// constructor and destructor
	CXlCell(void);
	virtual ~CXlCell(void);

	enum { CellType_None, CellType_Label	= 0x2, CellType_Number	= 0x3, CellType_Date	= 0x4, 
		   CellType_Boolean	= 0x5, CellType_Error	= 0x6	};
	enum { CellValueType_None, CellValueType_Int, CellValueType_100MultipliedInt, CellValueType_Float, CellValueType_100MultipliedFloat, CellValueType_Double, CellValueType_String	};

	int		m_nType;
	int		m_nRow;
	int		m_nCol;
	int		m_nXFIndex;

	int		m_ValueType;
	DWORD	m_dwValue;
	union {
		int		nValue;
		float	fValue;
		double	dValue;
		CHAR*	pValue;
	} m_Value;

	void		SetValue(void);
};
