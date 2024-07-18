//======================================================//
// Code by Jang.							2003.03.19	// 
// Obejct : Bank Window.								//
//======================================================//
#ifndef		__BANK_INC__
#define		__BANK_INC__

#include	"Menu.h"

#define SPR_OBJ_BANK_WND1		0
#define SPR_OBJ_BANK_WND2		1
#define SPR_OBJ_BANK_WND3		2
#define SPR_OBJ_BANK_CHK1		3
#define SPR_OBJ_BANK_CHK2		4
#define SPR_OBJ_BANK_CLOSE1		5
#define SPR_OBJ_BANK_CLOSE2		6
#define SPR_OBJ_BANK_BAR		7
#define SPR_OBJ_SC_BG			8
#define SPR_OBJ_BANK_SC			9
#define SPR_OBJ_BANK_MONEY1		10
#define SPR_OBJ_BANK_MONEY2		11
#define SPR_OBJ_BANK_MONEY3		12

class CBankWnd : public CMenu
{
public:

	BYTE	m_byServer;
	BYTE	m_byBankChk;
	BYTE	m_byMoneyChk;	
		
	BOOL	m_bMoveChk;			
		
	IDISpriteObject*	m_pSprLarge;
	IDISpriteObject*	m_pSprSmall;

public:

private:
	static CBankWnd* c_pThis;

public:
	static CBankWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CBankWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }
	
	BOOL	Init();
			
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();		
	
	void	RenderText();
	void	RenderLineBank();
	void	RenderBank();		
	void	RenderItemInfo();
	
	CBankWnd();
	~CBankWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//