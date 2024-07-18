#include ".\crlistboxiteminterface.h"

crListBoxItemInterface::crListBoxItemInterface()
: m_dwColor(0xffffffff),
m_bSelected(FALSE),
m_pData(NULL),
m_bLock(FALSE)
{
}

crListBoxItemInterface::crListBoxItemInterface( const xstring&	strItemText, DWORD	dwColor, void*	pData )
: m_dwColor(dwColor),
m_bSelected(FALSE),
m_pData(pData),
m_strItemText(strItemText),
m_bLock(FALSE)
{
}

crListBoxItemInterface::~crListBoxItemInterface()
{
}
