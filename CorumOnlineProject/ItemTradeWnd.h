//======================================================//
// Code by Jang.							2003.06.12	// 
// Obejct : Item Trade Window.							//
//======================================================//
#ifndef		__ITEMTRADE_INC__
#define		__ITEMTRADE_INC__

#include	"Menu.h"

#define SPR_OBJ_ITEMTRADE_WND			0
#define SPR_OBJ_ITEMTRADE_CLOSE1		1
#define SPR_OBJ_ITEMTRADE_CLOSE2		2
#define SPR_OBJ_ITEMTRADE_OK1			3
#define SPR_OBJ_ITEMTRADE_OK2			4
#define SPR_OBJ_ITEMTRADE_OK3			5
#define SPR_OBJ_ITEMTRADE_CANCEL1		6
#define SPR_OBJ_ITEMTRADE_CANCEL2		7
#define SPR_OBJ_ITEMTRADE_CANCEL3		8

class CItemTradeWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;
		
	DWORD	m_dwUserIndex;
	char	m_szName[MAX_CHARACTER_NAME_LENGTH];

	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;		
	
	BOOL	m_bBtnChk[2];
	
public:
	
private:
	static CItemTradeWnd* c_pThis;

public:
	static CItemTradeWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemTradeWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();		
	void	SetActive(BOOL bActive = TRUE);
		
	int		CheckInterface();		

	// 체크 관련 함수 //	

	CItemTradeWnd();
	~CItemTradeWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//
