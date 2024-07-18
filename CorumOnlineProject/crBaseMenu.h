#pragma once

#include <Windows.h>
#include <tchar.h>
#include "crtList.h"
#include "crtNode.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_common/IRenderer.h"
#else
#include "./../4DyuchiGRX_common/IRenderer.h"
#endif

#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

typedef	VOID	(*crMenuEventFunction)( VOID* pData );

#ifdef UNICODE
typedef	wstring	xstring;
#else
typedef	string	xstring;
#endif

struct crBaseMenu_Status
{
	DWORD	m_bVisible : 1;
	DWORD	m_bEnable : 1;
	DWORD	m_bEraseBackground : 1;
	DWORD	m_bDynamicAllocated : 1;
	DWORD	m_bExistClose : 1;
	DWORD	m_bMouseSelected : 1;	// 이전 프레임에 선택되었었는가?
	DWORD	m_bMouseOver : 1;		// 현재 마우스 오버의 상태
	DWORD	m_bMouseSelect : 1;		// 현재 선택의 상태
	DWORD	m_Reserved : 24;

	crBaseMenu_Status()
		: m_bVisible(TRUE),
        m_bEnable(TRUE),
        m_bEraseBackground(TRUE),
		m_bDynamicAllocated(TRUE),
		m_bExistClose(FALSE),
		m_bMouseSelected(FALSE),
        m_bMouseOver(FALSE),
		m_bMouseSelect(FALSE)
	{
	}
};

struct crSpriteRenderInfo
{
	FLOAT	m_fRot;
	VECTOR2	m_vScale;
	DWORD	m_dwColor;
	INT		m_iWidth;
	INT		m_iHeight;
};

#define crBaseMenuMsg_Enable	(1)

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
 * \date 2005-01-31
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
class crBaseMenu : public crtNode< crBaseMenu >
{
public:
	///////////////////////////////////////////////////////////////////////////////////
	typedef list< crBaseMenu* >						crBaseMenu_List;
	typedef list< crBaseMenu* >::iterator			crBaseMenu_List_Iter;
	typedef list< crBaseMenu* >::const_iterator		crBaseMenu_List_Const_Iter;
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	crBaseMenu();
	virtual ~crBaseMenu();
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \param pParentMenu 
	 * \param iX 
	 * \param iY 
	 * \return 
	 */
	virtual	BOOL	Render( crBaseMenu*	pParentMenu, INT	iX, INT iY, DWORD dwZOrder ) = 0;
	/**
	 *
	 * \return 
	 */
	virtual	INT		ProcessMessgae() = 0;
	
	/**
	 *
	 * \return 
	 */
	virtual	INT		Destroy() = 0;

	/**
	 *
	 * \return 
	 */
	virtual	BOOL	Update( crBaseMenu*	pParentMenu, INT	iX, INT iY );

	/**
	 *
	 */
	virtual	void	ValidateState();

	/**
	 *
	 * \return 
	 */
	virtual	BOOL	Event( INT iX, INT iY );

	/**
	*
	* \return 
	*/
	virtual	BOOL	EventOver( INT iX, INT iY );

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
		INT					iX,
		INT					iY,
		INT					iWidth,
		INT					iHeight,
		LPCTSTR				lpMenuName,
		DWORD				dwBackgroundColor,
        const crBaseMenu_Status&	SStatus
		);
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	virtual	BOOL	MouseInRect( INT iX, INT iY );
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \param iX 
	 * \param iY 
	 */
	__inline	void	SetPos( INT iX, INT iY );
	/**
	 *
	 * \param dwColor 
	 */
	__inline	void	SetBackgroundColor( DWORD dwColor );
	/**
	 *
	 * \return 
	 */
	__inline	DWORD	GetBackgroundColor() const;
	/**
	 *
	 * \param rcBound 
	 */
	__inline	void	SetRect( const RECT& rcBound );
	/**
	 *
	 * \return 
	 */
	__inline	const RECT& GetRect() const;
	/**
	 *
	 * \return 
	 */
	__inline	INT	GetWidth() const;
	/**
	 *
	 * \return 
	 */
	__inline	INT	GetHeight() const;
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	/**
	 *
	 * \return 
	 */
	__inline	BOOL	IsVisible() const;
	/**
	 *
	 * \param bVisible 
	 */
	__inline	void	SetVisible( BOOL bVisible );
	/**
	 *
	 * \return 
	 */
	__inline	BOOL	IsEnable() const;
	/**
	 *
	 * \param bEnable 
	 */
	__inline	void	SetEnable( BOOL bEnable );
	/**
	 * 배경을 칠할것인지를 설정한다.
	 * \param bEnable 
	 */
	__inline	void	SetEraseBackground( BOOL bEnable );
	/**
	 * 배경을 칠할것인지에 대한 플래그를 리턴받는다.
	 * \return 
	 */
	__inline	BOOL	GetEraseBackground();
	/**
	 *
	 * \param bEnable 
	 */
	__inline	void	SetMouseSelected( BOOL bEnable )
	{
		m_SStatus.m_bMouseSelected = bEnable;
	}
	/**
	 *
	 * \return 
	 */
	__inline	BOOL	GetMouseSelected() const
	{
		return m_SStatus.m_bMouseSelected;
	}
	/**
	*
	* \param bEnable 
	*/
	__inline	void	SetMouseSelect( BOOL bEnable )
	{
		m_SStatus.m_bMouseSelect = bEnable;
	}
	/**
	*
	* \return 
	*/
	__inline	BOOL	GetMouseSelect() const
	{
		return m_SStatus.m_bMouseSelect;
	}
	/**
	*
	* \param bEnable 
	*/
	__inline	void	SetMouseOver( BOOL bEnable )
	{
		m_SStatus.m_bMouseOver = bEnable;
	}
	/**
	*
	* \return 
	*/
	__inline	BOOL	GetMouseOver() const
	{
		return m_SStatus.m_bMouseOver;
	}
	/**
	 * 메뉴의 이름을 설정한다.
	 * \param lpName 
	 */
	__inline	void	SetBaseMenuName( LPCTSTR lpName );
	/**
	 * 메뉴의 이름을 리턴한다.
	 * \return 
	 */
	__inline	LPCTSTR	GetBaseMenuName();
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////
	/**
	*
	* \param eventFunction 
	*/
	__inline	void	SetEventFunction( crMenuEventFunction eventFunction )
	{
		m_fnEvent = eventFunction;
	}
	/**
	*
	* \param eventFunction 
	*/
	__inline	void	SetEventOverFunction( crMenuEventFunction eventFunction )
	{
		m_fnEventOver = eventFunction;
	}
	///////////////////////////////////////////////////////////////////////////////
protected:
	crMenuEventFunction		m_fnEvent;
	crMenuEventFunction		m_fnEventOver;

private:
	///////////////////////////////////////////////////////////////////////////////////
	crBaseMenu_Status		m_SStatus;				// 상태
	DWORD					m_dwBackgroundColor;	// 배경 색
	RECT					m_rcBound;				// 객체의 영역
	xstring					m_strMenuName;			// 메뉴의 이름	
	
	///////////////////////////////////////////////////////////////////////////////////
};

#include "crBaseMenu.inl"