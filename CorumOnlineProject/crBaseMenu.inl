__inline	void	crBaseMenu::SetPos( INT iX, INT iY )
{
	INT	iWidth = GetWidth();
	INT	iHeight = GetHeight();

	m_rcBound.left = iX;
	m_rcBound.top = iY;

	m_rcBound.bottom = iY + iHeight;
	m_rcBound.right = iX + iWidth;
}
/**
*
* \param dwColor 
*/
__inline	void	crBaseMenu::SetBackgroundColor( DWORD dwColor )
{
	m_dwBackgroundColor = dwColor;
}
/**
*
* \return 
*/
__inline	DWORD	crBaseMenu::GetBackgroundColor() const
{
	return m_dwBackgroundColor;
}
/**
*
* \param rcBound 
*/
__inline	void	crBaseMenu::SetRect( const RECT& rcBound )
{
	m_rcBound = rcBound;
}
/**
*
* \return 
*/
__inline	const RECT& crBaseMenu::GetRect() const
{
	return m_rcBound;
}
/**
*
* \return 
*/
__inline	INT	crBaseMenu::GetWidth() const
{
	return m_rcBound.right - m_rcBound.left;
}
/**
*
* \return 
*/
__inline	INT	crBaseMenu::GetHeight() const
{
	return m_rcBound.bottom - m_rcBound.top;
}
/**
*
* \return 
*/
__inline	BOOL	crBaseMenu::IsVisible() const
{
	return m_SStatus.m_bVisible;
}
/**
*
* \param bVisible 
*/
__inline	void	crBaseMenu::SetVisible( BOOL bVisible )
{
	m_SStatus.m_bVisible = bVisible;
}
/**
*
* \return 
*/
__inline	BOOL	crBaseMenu::IsEnable() const
{
	return m_SStatus.m_bEnable;
}
/**
*
* \param bEnable 
*/
__inline	void	crBaseMenu::SetEnable( BOOL bEnable )
{
	m_SStatus.m_bEnable = bEnable;
}
/**
*
* \param bEnable 
*/
__inline	void	crBaseMenu::SetEraseBackground( BOOL bEnable )
{
	m_SStatus.m_bEraseBackground = bEnable;
}
/**
*
* \return 
*/
__inline	BOOL	crBaseMenu::GetEraseBackground()
{
	return m_SStatus.m_bEraseBackground;
}
/**
*
* \param lpName 
*/
__inline	void	crBaseMenu::SetBaseMenuName( LPCTSTR lpName )
{
	m_strMenuName = lpName;
}
/**
*
* \return 
*/
__inline	LPCTSTR	crBaseMenu::GetBaseMenuName()
{
	if( m_strMenuName.empty() == true )
		return NULL;

	return m_strMenuName.c_str();
}