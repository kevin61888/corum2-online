#pragma once
#include "crbasemenu.h"
#include <cassert>

struct SSpriteInfo
{
	BOOL	bErase;
	DWORD	dwWidth;
	DWORD	dwHeight;

	SSpriteInfo()
		: bErase( FALSE ),
		dwHeight(0),
		dwWidth(0)
	{
	}

	SSpriteInfo( BOOL bVal )
		: bErase( bVal ),
		dwHeight(0),
		dwWidth(0)
	{
	}
};

class crImageList :
	public crBaseMenu
{
public:
	///////////////////////////////////////////////////////////////////////////////////
	/**
	*
	* \param pParentMenu 
	* \param iX 
	* \param iY 
	* \return 
	*/
	virtual	BOOL	Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder );
	virtual	BOOL	RenderOnly( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder );

	/**
	*
	* \return 
	*/
	virtual	INT		ProcessMessgae();

	/**
	*
	* \return 
	*/
	virtual	INT		Destroy();

	/**
	*
	* \param iX 
	* \param iY 
	* \param iWidth 
	* \param iHeight 
	* \param lpMenuName 
	* \param dwBackgroundColor 
	* \param bVisible 
	* \param bEnable 
	* \param bDynamicAllocated 
	* \return 
	*/
	virtual	BOOL	Initialize(
		INT		iX,
		INT		iY,
		INT		iWidth,
		INT		iHeight,
		LPCTSTR	lpMenuName,
		DWORD	dwBackgroundColor,
		const crBaseMenu_Status&	SStatus
		);

	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	virtual	BOOL	Event( INT iX, INT iY );
	virtual	BOOL	EventOver( INT iX, INT iY );
	///////////////////////////////////////////////////////////////////////////////////
	
	BOOL	SetSprite( DWORD dwIndex, IDISpriteObject* pSpr, DWORD dwWidth, DWORD dwHeight, BOOL bErase = TRUE );
	__inline	VOID	SetCurIndex( DWORD dwIndex )
	{
		assert( m_vecSpr.size() > dwIndex );
		m_dwCurIndex = dwIndex;
	}
	__inline	VOID	SetScale( float fScale )
	{
		m_fScale = fScale;
	}

	crImageList();
	virtual ~crImageList();

	enum crImageList_ImageAlign
	{
		ILIA_LEFT_TOP = 0,
		ILIA_CENTER,
		ILIA_MAX
	};
	__inline	void	SetImageAlign( crImageList_ImageAlign eType )
	{
		assert( eType < ILIA_MAX );
		m_eImageAlign = eType;
	}
private:
	vector< pair< IDISpriteObject*, SSpriteInfo > >	m_vecSpr;	// ¿ÃπÃ¡ˆ.
	DWORD						m_dwCurIndex;
	float						m_fScale;
	crImageList_ImageAlign		m_eImageAlign;
};
