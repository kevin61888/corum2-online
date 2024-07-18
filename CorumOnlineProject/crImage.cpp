#include ".\crimage.h"
#include <cassert>
#include "GameDefine.h"
#include "GameControl.h"

extern I4DyuchiGXRenderer*			g_pRenderer;

crImage::crImage()
: m_pSpr(NULL),
m_fScale( 1.0f )
{
}

crImage::~crImage()
{
}

///////////////////////////////////////////////////////////////////////////////////
BOOL	crImage::Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder )
{	
	crBaseMenu*	pMenu;

	BOOL	bVal = TRUE;

	if( IsVisible() )
	{
		VECTOR2	vPos;
		VECTOR2	vScale;

		vPos.x = iX + GetRect().left;
		vPos.y = iY + GetRect().top;
		vScale.x = m_fScale,
		vScale.y = m_fScale;

		g_pRenderer->RenderSprite(
			m_pSpr, 
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

INT		crImage::ProcessMessgae()
{
	return TRUE;
}

INT		crImage::Destroy()
{
    if( m_pSpr )
		m_pSpr->Release();

	return TRUE;
}

BOOL	crImage::Initialize(
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

BOOL	crImage::SetSprite( IDISpriteObject* pSpr )
{
	if( m_pSpr )
		m_pSpr->Release();

	m_pSpr = pSpr;
	return TRUE;
}

BOOL	crImage::Event( INT iX, INT iY )
{
	if( m_fnEvent )
		m_fnEvent( this );

	return TRUE;
}

BOOL	crImage::EventOver( INT iX, INT iY )
{
	if( m_fnEventOver )
		m_fnEventOver( this );

	return TRUE;
}