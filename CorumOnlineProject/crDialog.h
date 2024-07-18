#pragma once

#include "crBaseMenu.h"
#include "crButton.h"
#include "crStaticText.h"

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
 * \author Àüº´·Ä
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
class crDialog : public crBaseMenu
{
public:
	crDialog();
	virtual ~crDialog();

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
	///////////////////////////////////////////////////////////////////////////////////

private:
    crButton		m_btnClose;
	crStaticText	m_textName;

	crSpriteRenderInfo	m_SSpriteRenderInfo[3];
	IDISpriteObject*	m_pSpr[3];
};
