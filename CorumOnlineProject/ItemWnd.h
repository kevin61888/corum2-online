//======================================================//
// Code by Jang.							2002.12.23	//
// Obejct : Item Window.								//
//======================================================//
#ifndef		__ITEM_WINDOW_INC__
#define		__ITEM_WINDOW_INC__

#include	"Menu.h"
#include	"User.h"

#define SPR_OBJ_ITEMWINDOW						0
#define SPR_OBJ_ITEMWINDOW2						1
#define SPR_OBJ_ITEM_CLOSE1						2
#define SPR_OBJ_ITEM_CLOSE2						3
#define SPR_OBJ_ITEM_CHK1						4
#define SPR_OBJ_ITEM_CHK2						5
#define SPR_OBJ_MONEY_BUTTON1					6
#define SPR_OBJ_MONEY_BUTTON2					7
#define SPR_OBJ_MONEY_BUTTON3					8
#define SPR_OBJ_ITEMWND_GUARDIAN_OFF1			9
#define SPR_OBJ_ITEMWND_GUARDIAN_OFF2			10
#define SPR_OBJ_ITEMWND_GUARDIAN_ON1			11
#define SPR_OBJ_ITEMWND_GUARDIAN_ON2			12


class CItemWnd : public CMenu
{
public:
	
	ITEM_CHKTYPE	m_sItemChkType;

	RECT			m_rtPos;
	char			m_szInfo[0xff];
		
	BYTE			m_byItemChk;	// 0이면 Large,1이면 Small
	
	BOOL			m_bItemKeyChk;
	
	BOOL			m_bBtn;

	// 체크 관련 변수 //				
	WORD			m_wItemId[__MAX_ITEM_ICON__];

	BOOL			m_bGuarInterface;
	
	DWORD			m_dwCurTime;
	DWORD			m_dwPrvTime;
	
	IDISpriteObject*	m_pLineSpr;
	IDISpriteObject*	m_pBoxSpr;
	IDISpriteObject*	m_pCureentSpr;

public:

	// Singleton Patten //
private:
	static CItemWnd* c_pThis;

public:
	static CItemWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();	

	void	RenderText();	
	void	Remove();		
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	
	
	int		CheckInterface();
		
	void	SetLine(BOOL bActive, BYTE bLineType);		
	void	SetPosSommon();
	
	void	RenderItem();
	void	RenderBelt();	
	void	RenderItemMouse();
	void	RenderItemBelt();
	void	RenderItemLarge();
	void	RenderItemSmall();
	void	RenderItemEquip();
	void	RenderItemGuardian();;
	void	RenderItemInfo();
	void	RenderLine();
	void	RenderCurrentHand();

	// 체크 관련 함수 //
	BOOL	GetMouseIcon();

	void	GetMouseIcon(BOOL bMouseIcon);	
	
	CItemWnd();
	virtual ~CItemWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//