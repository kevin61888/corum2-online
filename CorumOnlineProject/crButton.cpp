#include ".\crbutton.h"
#include <cassert>
#include "GameDefine.h"
#include "GameControl.h"

extern I4DyuchiGXRenderer*			g_pRenderer;

crButton::crButton()
{
	m_pSpr[0] = NULL,
		m_pSpr[1] = NULL,
		m_pSpr[2] = NULL;
}

crButton::~crButton()
{
}


BOOL	crButton::Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder )
{
	crBaseMenu*	pMenu;

	BOOL	bVal = TRUE;

	if( IsVisible() )
	{
        VECTOR2	vPos;
		VECTOR2	vScale;

		vPos.x = iX + GetRect().left;
		vPos.y = iY + GetRect().top;
		vScale.x = 1.0f;
		vScale.y = 1.0f;

		INT	iIndex;

		if( TRUE == GetMouseSelect() )
			iIndex = 2;
		else if( GetMouseOver() )
			iIndex = 1;
		else
			iIndex  = 0;

		g_pRenderer->RenderSprite(
		m_pSpr[iIndex], 
		&vScale,
		0.0f, 
		&vPos, 
		NULL, 
		0xffffffff, 
		dwZOrder, 
		0
		);

		pMenu = GetChild();
		
		if( pMenu )
			bVal &= pMenu->Render( this, iX + GetRect().left, iY + GetRect().top, dwZOrder+1 );
	}

	pMenu = GetNext();

	if( pMenu )
		bVal &= pMenu->Render( pParentMenu, iX, iY, dwZOrder );

	return bVal;
}

INT		crButton::ProcessMessgae()
{
	return -1;
}

BOOL	crButton::Initialize(
						   INT		iX,
						   INT		iY,
						   INT		iWidth,
						   INT		iHeight,
						   LPCTSTR	lpMenuName,
						   DWORD	dwBackgroundColor,
						   const crBaseMenu_Status&	SStatus
						   )
{
	return crBaseMenu::Initialize( 
		iX,
		iY,
		iWidth,
		iHeight,
		lpMenuName,
		dwBackgroundColor,
		SStatus
		);
}

INT		crButton::Destroy()
{
	for( INT iter = 0 ; iter < 3 ; ++iter )
	{
		if( m_pSpr[iter] )
		{
			m_pSpr[iter]->Release();
			m_pSpr[iter] = NULL;
		}
	}
	return TRUE;
}

BOOL	crButton::SetSprite( INT iIndex, IDISpriteObject* pSpr )
{
	assert( iIndex >= 0 && iIndex < 3 );

	if( m_pSpr[iIndex] )
		m_pSpr[iIndex]->Release();

	m_pSpr[iIndex] = pSpr;

	return TRUE;
}

BOOL	crButton::Event( INT iX, INT iY )
{
	if( m_fnEvent )
		m_fnEvent( this );

	return TRUE;
}

BOOL	crButton::EventOver( INT iX, INT iY )
{
	if( m_fnEventOver )
		m_fnEventOver( this );

	return TRUE;
}