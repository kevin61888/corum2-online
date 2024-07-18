#include ".\crstatictext.h"
#include "CodeFun.h"

crStaticText::crStaticText()
: m_eTextAlignType(STTA_LEFT),
m_dwTextColor(0xffffffff)
{
}

crStaticText::~crStaticText()
{
}

void	crStaticText::CalcTextRect()
{
	// 사이즈 체에크~~~~~

	// 여기서만 통용된다. 굴림체 12폰트......

	INT	iWidth = m_strData.length() * 7;
	INT	iHeight = 12;
	m_rcTextSize.top = ((GetHeight()-iHeight)>>1);
	m_rcTextSize.bottom = (m_rcTextSize.top + iHeight);

	switch( GetAlign() )
	{
	case STTA_LEFT:
		// 왼쪽에 딱붙임.
		m_rcTextSize.left = 0;
		m_rcTextSize.right = iWidth;
		break;

	case STTA_CENTER:
		// 중간으로...
		m_rcTextSize.left = (GetWidth()-iWidth)>>1;
		m_rcTextSize.right = m_rcTextSize.left+iWidth;
		break;

	case STTA_RIGHT:
		// 끝을 오른쪽에 딱 붙임.
		m_rcTextSize.right = GetWidth();
		m_rcTextSize.left = m_rcTextSize.right - iWidth;
		break;
	}	
}

VOID	crStaticText::SetAlign( crStaticText_TextAlign	eAlign )
{
	assert( eAlign < STTA_MAX );

	if( eAlign == m_eTextAlignType )
		return;

	m_eTextAlignType = eAlign;

	CalcTextRect();
}

crStaticText::crStaticText_TextAlign	crStaticText::GetAlign() const
{
	return m_eTextAlignType;
}

LPCTSTR	crStaticText::SetText( LPCTSTR	lpText )
{
	assert( lpText );
	m_strData = lpText;

	if( m_strData.empty() == true )
		return NULL;   	

	CalcTextRect();

	return m_strData.c_str();
}

LPCTSTR	crStaticText::GetText()
{
	return m_strData.c_str();
}

LPCTSTR	crStaticText::Format( LPCTSTR	szFormat, ... )
{
	va_list args;
	va_start( args, szFormat );

	static TCHAR strBuffer[0xfff];
	wvsprintf( strBuffer, szFormat, args );

	va_end( args );
	SetText( strBuffer );

	return GetText();
}

BOOL	crStaticText::Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder )
{	
	crBaseMenu*	pMenu;

	BOOL	bVal = TRUE;

	if( IsVisible() )
	{
		RenderFont( 
			const_cast< char* >(GetText()), 
			GetRect().left+m_rcTextSize.left+iX, 
			GetRect().right+m_rcTextSize.right+iX, 
			GetRect().top+m_rcTextSize.top+iY, 
			GetRect().bottom+m_rcTextSize.bottom+iY, 
			dwZOrder, 
			m_dwTextColor 
			);
		
		pMenu = GetChild();
		
		if( pMenu && IsVisible() )
			bVal &= pMenu->Render( this, iX + GetRect().left, iY + GetRect().top, dwZOrder+1 );
	}

	pMenu = GetNext();

	if( pMenu )
		bVal &= pMenu->Render( pParentMenu, iX, iY, dwZOrder );

	return bVal;
}

INT		crStaticText::ProcessMessgae()
{
	return -1;
}

BOOL	crStaticText::Initialize(
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

INT		crStaticText::Destroy()
{
	return 0;
}

/**
*
* \param dwColor 
* \return 
*/
DWORD	crStaticText::SetTextColor( DWORD dwColor )
{
	m_dwTextColor = dwColor;
	return m_dwTextColor;
}

/**
*
* \return 
*/
DWORD	crStaticText::GetTextColor() const
{
	return m_dwTextColor;
}