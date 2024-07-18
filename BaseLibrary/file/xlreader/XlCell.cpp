#include "../../StdAfx.h"
#include "XlCell.h"

#include <BaseLibrary/Utility/Debugging/DebugUtils.h>
#define new New


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CXlCell::CXlCell(void) : m_nType(CXlCell::CellType_None), m_nRow(0), m_nCol(0), m_nXFIndex(0), 
m_dwValue(0), m_ValueType(CXlCell::CellValueType_None)
{
	m_Value.nValue	= 0;
}

CXlCell::~CXlCell(void)
{
}

const DWORD		BITMASK_100MULTIPLIED	= 0x00000001;
const DWORD		BITMASK_INTEGER			= 0x00000002;
const DWORD		BITMASK_ENCODEDVALUE	= 0xFFFFFFFC;
const int		ENCODEDBIT_LENGTH		= 2;

void CXlCell::SetValue(void)
{
	if ( BITMASK_INTEGER == ( BITMASK_INTEGER & m_dwValue ) ) {
		DWORD dwRealValue	= m_dwValue >> ENCODEDBIT_LENGTH;
		if ( BITMASK_100MULTIPLIED == ( BITMASK_100MULTIPLIED & m_dwValue ) ) {
			float* pValue	= reinterpret_cast<float*>(&dwRealValue);
			m_Value.fValue	= *pValue / 100.0f;
			m_ValueType		= CellValueType_100MultipliedInt;
		} else {
			int* pnValue	= reinterpret_cast<int*>(&dwRealValue);
			m_Value.nValue	= *pnValue;
			m_ValueType		= CellValueType_Int;
		}
	} else {
		DWORD64	dlValue	= m_dwValue;
		dlValue	<<= 32;
		if ( BITMASK_100MULTIPLIED == ( BITMASK_100MULTIPLIED & m_dwValue ) ) {
			double* pValue	= reinterpret_cast<double*>(&dlValue);
			m_Value.fValue	= static_cast<float>(*pValue) / 100.0f;
			m_ValueType		= CellValueType_100MultipliedFloat;
		} else {
			double* pValue	= reinterpret_cast<double*>(&dlValue);
			m_Value.dValue	= *pValue;
			m_ValueType		= CellValueType_Float;
		}
	}
}
