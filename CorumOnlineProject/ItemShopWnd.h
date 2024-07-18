//======================================================//
// Code by Jang.							2003.02.03	//
// Obejct : Char Window.								//
//======================================================//
#ifndef		__ITEMSHOPWND_INC__
#define		__ITEMSHOPWND_INC__

#include "Menu.h"

#define SPR_OBJ_ITEMSHOPWND				0
#define SPR_OBJ_ITEMSHOPWND_CLOSE1		1
#define SPR_OBJ_ITEMSHOPWND_CLOSE2		2
#define BUTTON_OBJ_MAKE1				3
#define BUTTON_OBJ_MAKE2				4
#define BUTTON_OBJ_MAKE3				5
#define BUTTON_OBJ_STR1					6
#define BUTTON_OBJ_STR2					7
#define BUTTON_OBJ_STR3					8
#define BUTTON_OBJ_TRADE1				9
#define BUTTON_OBJ_TRADE2				10
#define BUTTON_OBJ_TRADE3				11
#define BUTTON_OBJ_INFO1				12
#define BUTTON_OBJ_INFO2				13
#define BUTTON_OBJ_INFO3				14
#define SPR_OBJ_BANK_BTN1				15
#define SPR_OBJ_BANK_BTN2				16
#define SPR_OBJ_BANK_BTN3				17

class CItemShopWnd : public CMenu
{
public:

	BOOL	m_bActiveChk;

	char	m_szInfo[0xff];
	RECT	m_rtPos;

	BYTE	m_byServer;
	BYTE	m_byTypeRt;

	DWORD	m_dwId[18];
	

	BYTE	m_byChatLine;
	SDGCHAT	m_sDgChat[5];

public:

	// Singleton Patten //
private:
	static CItemShopWnd* c_pThis;

public:
	static CItemShopWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemShopWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //		
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();		
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// 체크 관련 함수 //	
	void	SetPosSpr();	

	CItemShopWnd();
	virtual ~CItemShopWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//