#include ".\crimagelist.h"
#include "GameDefine.h"
#include "GameControl.h"

extern I4DyuchiGXRenderer*			g_pRenderer;

crImageList::crImageList()
: m_dwCurIndex(0),
m_eImageAlign(ILIA_LEFT_TOP)
{
}

crImageList::~crImageList()
{
}

///////////////////////////////////////////////////////////////////////////////////
BOOL	crImageList::Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder )
{
	crBaseMenu*	pMenu;

	BOOL	bVal = TRUE;

	if( IsVisible() || m_vecSpr.empty() == true )
	{
		VECTOR2	vPos;
		VECTOR2	vScale;

		vPos.x = iX + GetRect().left;
		vPos.y = iY + GetRect().top;
		vScale.x = m_fScale;
		vScale.y = m_fScale;
			
		if( m_eImageAlign == ILIA_CENTER )
		{
			vPos.x += (GetWidth()>>1) - (m_vecSpr[m_dwCurIndex].second.dwWidth>>1)*m_fScale;
			vPos.y += (GetHeight()>>1) - (m_vecSpr[m_dwCurIndex].second.dwHeight>>1)*m_fScale;
		}

		g_pRenderer->RenderSprite(
			m_vecSpr[m_dwCurIndex].first, 
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

BOOL	crImageList::RenderOnly( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder )
{
	if( IsVisible() || m_vecSpr.empty() == true )
	{
		VECTOR2	vPos;
		VECTOR2	vScale;

		vPos.x = iX + GetRect().left;
		vPos.y = iY + GetRect().top;
		vScale.x = m_fScale;
		vScale.y = m_fScale;
			
		if( m_eImageAlign == ILIA_CENTER )
		{
			vPos.x += (GetWidth()>>1) - (m_vecSpr[m_dwCurIndex].second.dwWidth>>1)*m_fScale;
			vPos.y += (GetHeight()>>1) - (m_vecSpr[m_dwCurIndex].second.dwHeight>>1)*m_fScale;
		}

		g_pRenderer->RenderSprite(
			m_vecSpr[m_dwCurIndex].first, 
			&vScale,
			0.0f, 
			&vPos, 
			NULL, 
			0xffffffff, 
			dwZOrder, 
			0
			);
	}	
	return TRUE;
}

INT		crImageList::ProcessMessgae()
{
	return TRUE;
}

INT		crImageList::Destroy()
{
//	if( m_pSpr )
//		m_pSpr->Release();

	return TRUE;
}

BOOL	crImageList::Initialize(
							INT		iX,
							INT		iY,
							INT		iWidth,
							INT		iHeight,
							LPCTSTR	lpMenuName,
							DWORD	dwBackgroundColor,
							const crBaseMenu_Status&	SStatus
							)
{
	m_dwCurIndex = 0;
	m_fScale = 1.0f;

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

/*
BOOL	crImageList::SetSprite( IDISpriteObject* pSpr )
{
	if( m_pSpr )
		m_pSpr->Release();

	m_pSpr = pSpr;
	return TRUE;
}*/


BOOL	crImageList::Event( INT iX, INT iY )
{
	if( m_fnEvent )
		m_fnEvent( this );

	return TRUE;
}

BOOL	crImageList::EventOver( INT iX, INT iY )
{
	if( m_fnEventOver )
		m_fnEventOver( this );

	return TRUE;
}

BOOL	crImageList::SetSprite( DWORD dwIndex, IDISpriteObject* pSpr, DWORD dwWidth, DWORD dwHeight, BOOL bErase )
{
	if( m_vecSpr.size() < dwIndex + 1 )
	{
		vector< pair< IDISpriteObject*, SSpriteInfo > >	tmp;
		
		tmp.reserve( dwIndex + 1 );
		tmp.resize( dwIndex + 1 );
		
		::memcpy( &tmp[0], &m_vecSpr[0], sizeof(pair< IDISpriteObject*, SSpriteInfo >)*m_vecSpr.size() );
		
		tmp.swap( m_vecSpr );
	}
	
	pair< IDISpriteObject*, SSpriteInfo >&	rPair = m_vecSpr[dwIndex];
	
	if( rPair.first && rPair.second.bErase == TRUE )
		rPair.first->Release();
	
	rPair.first = pSpr;
	rPair.second.bErase = bErase;
	rPair.second.dwWidth = dwWidth;
	rPair.second.dwHeight = dwHeight;
	return TRUE;
}