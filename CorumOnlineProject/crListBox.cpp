#include ".\crlistbox.h"
#include "crButton.h"
#include "CodeFun.h"
#include "GameControl.h"

extern MouseState			g_Mouse;

crListBox::crListBox()
:/*
 m_pBtnPrev(NULL),
m_pBtnNext(NULL),*/

m_dwOnceVisbleLineCount(0),
m_dwLineStep(0),
m_dwVisiblePage(0)
{
	m_dwColor[0] = m_dwColor[1] = m_dwColor[2] = 0xffffffff;
}

crListBox::~crListBox()
{
}

BOOL	crListBox::Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder )
{
	crBaseMenu*	pMenu;

	BOOL	bVal = TRUE;

	if( IsVisible() )
	{
		if( GetPage() * m_dwOnceVisbleLineCount > m_vecListItem.size() )
			PrevPage();
		
		// ÁÖ·ç¸¤ ·»´õ¾î¸µ...
		ListItemList_Const_Iter	iter_begin = m_vecListItem.begin() + GetPage() * m_dwOnceVisbleLineCount;
		ListItemList_Const_Iter	iter_end = m_vecListItem.end();
		
		DWORD	dwCount = 0;
		
		while( iter_begin != iter_end && dwCount < m_dwOnceVisbleLineCount )
		{
			RenderFont( 
				const_cast< char* >((*iter_begin).m_strItemText.c_str()), 
				GetRect().left + iX, 
				GetRect().right + iX,
				(dwCount * m_dwLineStep) + GetRect().top + iY,
				((dwCount+1) * m_dwLineStep) + GetRect().top + iY,
				dwZOrder,
				(*iter_begin).m_dwColor
				);            
			
			++iter_begin;
			++dwCount;
		}
		
		pMenu = GetChild();
		
		if( pMenu && IsVisible() )
			bVal &= pMenu->Render( this, iX + GetRect().left, iY + GetRect().top, dwZOrder+1 );
	}	

	pMenu = GetNext();

	if( pMenu )
		bVal &= pMenu->Render( pParentMenu, iX, iY, dwZOrder );

	return bVal;
}

void	crListBox::ValidateState()
{
	crBaseMenu::ValidateState();

	ListItemList_Iter	iter_begin = m_vecListItem.begin();
	ListItemList_Const_Iter	iter_end = m_vecListItem.end();

	while( iter_begin != iter_end )
	{
		if( (*iter_begin).m_bLock )
		{
            (*iter_begin).m_dwColor = m_dwColor[3];
		}
		else if( (*iter_begin).m_bSelected )
			(*iter_begin).m_dwColor = m_dwColor[2];
		else
			(*iter_begin).m_dwColor = m_dwColor[0];

		++iter_begin;
	}
}

BOOL	crListBox::Event( INT iX, INT iY )
{
	if( GetPage() * m_dwOnceVisbleLineCount > m_vecListItem.size() )
		PrevPage();

	ListItemList_Iter	iter_begin = m_vecListItem.begin() + GetPage() * m_dwOnceVisbleLineCount;
	ListItemList_Const_Iter	iter_end = m_vecListItem.end();

	DWORD	dwCount = 0;
	RECT	rcBound;

	while( iter_begin != iter_end && dwCount < m_dwOnceVisbleLineCount )
	{
//		if( (*iter_begin).m_bLock == FALSE )
		{
			::SetRect( 
				&rcBound, 
				GetRect().left + iX, 
				GetRect().top + iY + (m_dwLineStep*dwCount), 
				GetRect().right + iX, 
				GetRect().top + iY + (m_dwLineStep*(dwCount+1)) 
				);

			if( TRUE == ::PtInRect( &rcBound, g_Mouse.MousePos ) )
			{
				(*iter_begin).m_bSelected = 1 - (*iter_begin).m_bSelected;

				if( (*iter_begin).m_bSelected )
					(*iter_begin).m_dwColor = m_dwColor[2];			
				else
					(*iter_begin).m_dwColor = m_dwColor[0];

				if( m_fnEvent )
				{
					m_iterLastSelected = iter_begin;
					m_fnEvent( this );
				}
				break;
			}
		}
		
		++iter_begin;
		++dwCount;
	}

	return TRUE;
}

BOOL	crListBox::EventOver( INT iX, INT iY )
{
	if( GetPage() * m_dwOnceVisbleLineCount > m_vecListItem.size() )
		PrevPage();

	ListItemList_Iter	iter_begin = m_vecListItem.begin() + GetPage() * m_dwOnceVisbleLineCount;
	ListItemList_Const_Iter	iter_end = m_vecListItem.end();

	DWORD	dwCount = 0;
	RECT	rcBound;

	while( iter_begin != iter_end && dwCount < m_dwOnceVisbleLineCount )
	{
		if( (*iter_begin).m_bLock == FALSE )
		{
			::SetRect( 
				&rcBound, 
				GetRect().left + iX, 
				GetRect().top + iY + (m_dwLineStep*dwCount), 
				GetRect().right + iX, 
				GetRect().top + iY + (m_dwLineStep*(dwCount+1)) 
				);

			if( TRUE == ::PtInRect( &rcBound, g_Mouse.MousePos ) )
			{
				(*iter_begin).m_dwColor = m_dwColor[1];

				if( m_fnEventOver ) 
					m_fnEventOver( NULL );
				break;
			}
		}		

		++iter_begin;
		++dwCount;
	}

	return TRUE;
}
/**
*
* \return 
*/
INT		crListBox::ProcessMessgae()
{
	return -1;
}

INT		crListBox::Destroy()
{
	return 0;
}

void	crListBox::UpdateList()
{
}

BOOL	crListBox::SetPage( DWORD dwPage )
{
	m_dwVisiblePage = dwPage;
    return TRUE;
}

BOOL	crListBox::NextPage()
{
	if( (m_dwVisiblePage+1) * m_dwOnceVisbleLineCount >= m_vecListItem.size() )
		return FALSE;

	++m_dwVisiblePage;
	return TRUE;
}

BOOL	crListBox::PrevPage()
{
	if( m_dwVisiblePage == 0 )
		return FALSE;

	--m_dwVisiblePage;
	return TRUE;
}

BOOL	crListBox::DeleteAllItems()
{
	m_dwVisiblePage = 0;
    m_vecListItem.clear();

	return TRUE;
}

BOOL	crListBox::AddItem( LPCTSTR lpText, void* pData )
{
	m_vecListItem.push_back( ListItem( xstring( lpText ), m_dwColor[0], pData ) );
	return TRUE;
}

BOOL	crListBox::InsertItem( INT iIndex, LPCTSTR lpText, void* pData )
{
	m_vecListItem.insert( m_vecListItem.begin() + iIndex, ListItem( xstring( lpText ), m_dwColor[0], pData ) );
	return TRUE;
}

BOOL	crListBox::AddItemFromFormat( void* pData, LPCTSTR lpText, ... )
{
	va_list args;
	va_start( args, lpText );

	static TCHAR strBuffer[0xfff];
	wvsprintf( strBuffer, lpText, args );

	va_end( args );

	m_vecListItem.push_back( ListItem( xstring( strBuffer ), m_dwColor[0], pData ) );
	
	return TRUE;
}

BOOL	crListBox::InsertItemFromFormat( INT iIndex, void* pData, LPCTSTR lpText, ... )
{	
	va_list args;
	va_start( args, lpText );

	static TCHAR strBuffer[0xfff];
	wvsprintf( strBuffer, lpText, args );

	va_end( args );

	if( m_vecListItem.empty() == true )
        m_vecListItem.push_back( ListItem( xstring( strBuffer ), m_dwColor[0], pData ) );
	else
		m_vecListItem.insert( m_vecListItem.begin() + iIndex,  ListItem( xstring( strBuffer ), m_dwColor[0], pData ) );

	return TRUE;
}