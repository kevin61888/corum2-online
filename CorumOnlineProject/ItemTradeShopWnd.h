//======================================================//
// Code by Jang.							2003.06.16	// 
// Obejct : Item Trade Shop Window.						//
//======================================================//
#ifndef		__ITEMTRADESHOP_INC__
#define		__ITEMTRADESHOP_INC__

#include	"Menu.h"

#define SPR_OBJ_ITEMTRADESHOP_WND1			0
#define SPR_OBJ_ITEMTRADESHOP_WND2			1
#define SPR_OBJ_ITEMTRADESHOP_CLOSE1		2
#define SPR_OBJ_ITEMTRADESHOP_CLOSE2		3
#define SPR_OBJ_ITEMTRADESHOP_TRADE1		4
#define SPR_OBJ_ITEMTRADESHOP_TRADE2		5
#define SPR_OBJ_ITEMTRADESHOP_TRADE3		6
#define SPR_OBJ_ITEMTRADESHOP_TRADE4		7
#define SPR_OBJ_ITEMTRADESHOP_OK1_1			8
#define SPR_OBJ_ITEMTRADESHOP_OK1_2			9
#define SPR_OBJ_ITEMTRADESHOP_OK1_3			10
#define SPR_OBJ_ITEMTRADESHOP_OK1_4			11
#define SPR_OBJ_ITEMTRADESHOP_OK2_1			12
#define SPR_OBJ_ITEMTRADESHOP_OK2_2			13
#define SPR_OBJ_ITEMTRADESHOP_OK2_3			14
#define SPR_OBJ_ITEMTRADESHOP_OK2_4			15
#define SPR_OBJ_ITEMTRADESHOP_CANCEL1		16
#define SPR_OBJ_ITEMTRADESHOP_CANCEL2		17
#define SPR_OBJ_ITEMTRADESHOP_CANCEL3		18

class CItemTradeShopWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;
	BOOL	m_bBtn[4];

	DWORD	m_dwMoney;
	
	char	m_szName[MAX_CHARACTER_NAME_LENGTH];
	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;

	CItem	m_pTradeItem[MAX_TRADE];

	DWORD	m_dwDstMoney;
	
	BOOL	m_bOkChk;
	BOOL	m_bTradeChk;
	BOOL	m_bTradeOneChk;
	BOOL	m_bTradeOkChk;

public:
	
private:
	static CItemTradeShopWnd* c_pThis;

public:
	static CItemTradeShopWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemTradeShopWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
		
	int		CheckInterface();		

	// 체크 관련 함수 //	
	void	SetOk();
	void	SetTrade();
	void	SetTradeOne();
	void	RenderTrade();	
	void	ItemInfoRender();

	CItemTradeShopWnd();
	~CItemTradeShopWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//
