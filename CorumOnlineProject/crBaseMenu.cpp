#include ".\crbasemenu.h"
#include <cassert>
#include "GameControl.h"

extern MouseState			g_Mouse;

crBaseMenu::crBaseMenu()
: m_dwBackgroundColor(0xffffffff),
m_fnEvent(NULL),
m_fnEventOver(NULL)
{
	SetEnable( TRUE );
	SetVisible( TRUE );
	SetEraseBackground( TRUE );
	m_SStatus.m_bDynamicAllocated = TRUE;
}

crBaseMenu::~crBaseMenu()
{
}

BOOL	crBaseMenu::Update( crBaseMenu*	pParentMenu, INT	iX, INT iY )
{
	crBaseMenu*	pMenu;

	BOOL	bVal = FALSE;

	if( IsVisible() )
	{		
		// 차일드 플리즈~~~
		pMenu = GetChild();

		if( pMenu )
			bVal = pMenu->Update( this, iX + GetRect().left, iY + GetRect().top );

		if( bVal == TRUE )
			return bVal;
	}
	
	pMenu = GetNext();

	if( pMenu )
		bVal = pMenu->Update( pParentMenu, iX, iY );

	if( bVal == TRUE )
		return bVal;

	if( IsVisible() )
	{
		return MouseInRect( iX, iY );
	}

	return FALSE;
}

void	crBaseMenu::ValidateState()
{
	// 차일드 플리즈~~~
	crBaseMenu*	pMenu = GetChild();

	if( pMenu )
		pMenu->ValidateState();

	pMenu = GetNext();

	if( pMenu )
		pMenu->ValidateState();

	SetMouseSelected( GetMouseSelect() );
	SetMouseSelect( FALSE );
    SetMouseOver( FALSE );
}

BOOL	crBaseMenu::Initialize(
								 INT		iX,
								 INT		iY,
								 INT		iWidth,
								 INT		iHeight,
								 LPCTSTR	lpMenuName,
								 DWORD	dwBackgroundColor,
								 const crBaseMenu_Status&	SStatus
						   )
{
	assert( lpMenuName );

	RECT	rcBound = { iX, iY, iX + iWidth, iY + iHeight };
	SetRect( rcBound );
	
	SetBaseMenuName( lpMenuName );
	SetBackgroundColor( dwBackgroundColor );

	m_SStatus = SStatus;

	return TRUE;
}

BOOL	crBaseMenu::MouseInRect( INT iX, INT iY )
{
	RECT	rcMouse = { GetRect().left + iX, GetRect().top + iY, GetRect().right + iX, GetRect().bottom + iY };

	SetMouseOver( ::PtInRect( &rcMouse, g_Mouse.MousePos ) );

	if( GetMouseOver() && g_Mouse.bLDown == true )
	{
		SetMouseSelect( TRUE );
	}

	if( GetMouseOver() )
	{
		if( GetMouseSelected() && FALSE == GetMouseSelect() )
			return Event( iX, iY );

		return EventOver( iX, iY );
	}
	return FALSE;
}

BOOL	crBaseMenu::Event( INT iX, INT iY )
{
	if( m_fnEvent )
		m_fnEvent( this );
	return TRUE;
}

BOOL	crBaseMenu::EventOver( INT iX, INT iY )
{
	if( m_fnEventOver )
		m_fnEventOver( this );
	return TRUE;
}