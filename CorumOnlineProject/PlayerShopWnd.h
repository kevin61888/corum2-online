// PlayerShopWnd.h: interface for the PlayerShopWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYERSHOPWND_H__4AFFCE66_C8BB_4BE6_9AC9_D74F7F3E816E__INCLUDED_)
#define AFX_PLAYERSHOPWND_H__4AFFCE66_C8BB_4BE6_9AC9_D74F7F3E816E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include	"Menu.h"
#include "struct.h"
class CUser;

#define SPR_OBJ_PLAYER_SHOP_WND				0
#define BUTTON_OBJ_PLAYERSHOP_EXIT1			1
#define BUTTON_OBJ_PLAYERSHOP_EXIT2			2
#define BUTTON_OBJ_PLAYERSHOP_OPEN1			3
#define BUTTON_OBJ_PLAYERSHOP_OPEN2			4
#define BUTTON_OBJ_PLAYERSHOP_OPEN3			5
#define BUTTON_OBJ_PLAYERSHOP_CLOSE1		6
#define BUTTON_OBJ_PLAYERSHOP_CLOSE2		7
#define BUTTON_OBJ_PLAYERSHOP_CLOSE3		8
#define BUTTON_OBJ_PLAYERSHOPWND_CANCEL1	9
#define BUTTON_OBJ_PLAYERSHOPWND_CANCEL2	10
#define BUTTON_OBJ_PLAYERSHOPWND_CANCEL3	11

class CPlayerShopWnd   : public CMenu
{
	BYTE	m_bInputMode;		// 0 : 노점상 제목, 1 : 설명란

	char	m_szInfo[0xff];
	RECT	m_rtPos;

public:

	BYTE	m_byMode;
	DWORD	m_dwMoney;	

	BYTE	m_bChkMouse;
	BYTE	m_bZipCode;

	PLAYERSHOPINFO	m_sPlayerShopInfo;
	CUser*			m_pOwnerPlayerShop;	// 누가 이창을 엑티브 시켰냐..	
	
public:
	
	CPlayerShopWnd();
	~CPlayerShopWnd();

private:
	static CPlayerShopWnd* c_pThis;

public:
	static CPlayerShopWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CPlayerShopWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
		
	int		CheckInterface();

	void	PlayerShopMode(BYTE bMode);	// bMode = 0 : close, 1 : open	
	void	ItemInfoRender();	
	void	RenderItemPlayerShop();	

	BYTE	GetInputMode();
	void	SetInputMode(BYTE bInputMode, BOOL bClear = TRUE);
	void	PlayerShopStart();
	void	SetInputModeChk();

private:
	
	void	SetButtonStatus(DWORD dwSprID, DWORD dwStatus);	
	void	OnMove();	
};

#endif // !defined(AFX_PLAYERSHOPWND_H__4AFFCE66_C8BB_4BE6_9AC9_D74F7F3E816E__INCLUDED_)
