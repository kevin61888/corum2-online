#pragma once

#include "crBaseMenu.h"

class crListBoxItemInterface
{
public:
	xstring	m_strItemText;
	DWORD	m_dwColor;
	BOOL	m_bSelected;
	BOOL	m_bLock;
    void*	m_pData;

	crListBoxItemInterface();
	crListBoxItemInterface( const xstring&	strItemText, DWORD	dwColor, void*	pData );
	~crListBoxItemInterface();
};
