//======================================================//
// Code by Jang.							2002.05.27	//
// Obejct : Default Window.								//
//======================================================//
#ifndef		__DEFAULT_WINDOW_INC__
#define		__DEFAULT_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_DEFAULT_WND			0
#define SPR_OBJ_DEFAULT_CLOSE1		1
#define SPR_OBJ_DEFAULT_CLOSE2		2
#define SPR_OBJ_DEFAULT_OK1			3
#define SPR_OBJ_DEFAULT_OK2			4
#define SPR_OBJ_DEFAULT_OK3			5
#define SPR_OBJ_DEFAULT_CANCEL1		6
#define SPR_OBJ_DEFAULT_CANCEL2		7
#define SPR_OBJ_DEFAULT_CANCEL3		8

class CDefaultWnd : public CMenu
{
public:
			
	RECT	m_rtRect;
	char	m_szInfo[TEXT_RENDER_BUFFER];

	BOOL	m_bMoveChk;	
	
public:

	// Singleton Patten //
private:
	static CDefaultWnd* c_pThis;

public:
	static CDefaultWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CDefaultWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
	
	void	RenderText();	
	void	Remove();	
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);		
	int		CheckInterface();

	// 체크 관련 함수 //
		
	CDefaultWnd();
	~CDefaultWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//