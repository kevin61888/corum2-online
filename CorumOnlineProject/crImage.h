#pragma once
#include "crbasemenu.h"

class crImage : public crBaseMenu
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

	/**
	*
	* \param iIndex 
	* \param pSpr 
	* \return 
	*/
	virtual	BOOL	SetSprite( IDISpriteObject* pSpr );	
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	virtual	BOOL	Event( INT iX, INT iY );
	virtual	BOOL	EventOver( INT iX, INT iY );
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	crImage();
	virtual ~crImage();
	///////////////////////////////////////////////////////////////////////////////////

	__inline	void	SetScale( float fScale )
	{
		m_fScale = fScale;
	}

private:
    IDISpriteObject*	m_pSpr;	// 버튼의 이미지.
	float				m_fScale;
};
