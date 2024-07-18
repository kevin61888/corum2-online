#pragma once

#include "Menu.h"
#include "crBaseMenu.h"

/**
 * \ingroup CorumOnlineProject
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-02-02
 *
 * \author 전병렬
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 
 *
 * \bug 
 *
 */
class crStaticText : public crBaseMenu
{
public:
	enum	crStaticText_TextAlign
	{
		STTA_LEFT = 0,
		STTA_CENTER,
		STTA_RIGHT,
		STTA_MAX
	};	//	텍스트의 정렬방식, 기본 레프트

	// 스트링 포맷
	// 텍스트 셋

	// 영역,랙트

	// 렌더링
	// 이벤트 처리
	//

	/////////////////
	// 기본
	// 네임
	// ID
	// 
	/**
	 *
	 * \param eAlign 
	 */
	virtual	VOID	SetAlign( crStaticText_TextAlign	eAlign );

	/**
	 *
	 * \return 
	 */
	virtual	crStaticText_TextAlign	GetAlign() const;

	/**
	 *
	 * \param lpText 
	 * \return 
	 */
	virtual	LPCTSTR	SetText( LPCTSTR	lpText );

	/**
	 *
	 * \return 
	 */
	virtual	LPCTSTR	GetText();

	/**
	 *
	 * \param szFormat 
	 * \param ... 
	 * \return 
	 */
	virtual	LPCTSTR	Format( LPCTSTR	szFormat, ... );

	/**
	 *
	 * \param dwColor 
	 * \return 
	 */
	virtual	DWORD	SetTextColor( DWORD dwColor );
	/**
	 *
	 * \return 
	 */
	virtual	DWORD	GetTextColor() const;
	
	////////////////////////////////////////////////////////////////////
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
	 * \param SStatus 
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
	//////////////////////////////////////////////////////////////////////

	crStaticText();
	virtual ~crStaticText();

private:
	/**
	 *
	 */
	void	CalcTextRect();

	crStaticText_TextAlign	m_eTextAlignType;	//	텍스트의 정렬방식, 기본 레프트
	DWORD					m_dwTextColor;	
	xstring					m_strData;    
	RECT					m_rcTextSize;
};
