#pragma once

#include "Menu.h"
#include "crDialog.h"

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
class crProxyMenuToDialog : public CMenu
{
public:
	crBaseMenu*	m_pDlg;

	crProxyMenuToDialog();
	virtual ~crProxyMenuToDialog();

	// 공통으로 사용되는 것들은 재정의 한다.	
	/**
	 *
	 * \return 
	 */
	virtual BOOL	Init();

	/**
	 *
	 */
	virtual void	RenderText();

	/**
	 *
	 * \param bActive 
	 */
	virtual void	SetActive(BOOL bActive = TRUE);

	/**
	 *
	 */
	virtual void	Remove();

	/**
	 *
	 */
	virtual void	SetOrder();

	/**
	 *
	 * \return 
	 */
	virtual int		CheckInterface();

	/**
	 *
	 * \param pszName 
	 * \param fWidth 
	 * \param fHeight 
	 * \param byType 
	 * \param nIndex 
	 * \param bActive 
	 * \param bScroll 
	 */
	virtual	void	CreateMenu(
		char*	pszName, 
		float	fWidth, 
		float	fHeight, 
		BYTE	byType, 
		int		nIndex, 
		BOOL	bActive = FALSE, 
		BOOL	bScroll = FALSE
		);

	__inline	void	SetProxyInit()
	{
		m_bProxyInit = TRUE;
	}
	__inline	BOOL	GetProxyInit() const
	{
		return m_bProxyInit;
	}

private:
	BOOL	m_bProxyInit;
};
