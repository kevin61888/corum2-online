//======================================================//
// Code by Jang.							2002.02.27	//
// Obejct : Char Window.								//
//======================================================//
#ifndef		__EXIT_WINDOW_INC__
#define		__EXIT_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_EXIT_WND1				0
#define SPR_OBJ_EXIT_WND2				1
#define SPR_OBJ_EXIT_WND3				2
#define BUTTON_OBJ_GAME_EXIT1			3
#define BUTTON_OBJ_GAME_EXIT2			4
#define BUTTON_OBJ_GAME_EXIT3			5
#define BUTTON_OBJ_CHAR_SELECT1			6
#define BUTTON_OBJ_CHAR_SELECT2			7
#define BUTTON_OBJ_CHAR_SELECT3			8
#define BUTTON_OBJ_GAME_RETURN1			9
#define BUTTON_OBJ_GAME_RETURN2			10
#define BUTTON_OBJ_GAME_RETURN3			11
#define SPR_OBJ_EXIT_CLOSE1				12
#define SPR_OBJ_EXIT_CLOSE2				13
#define BUTTON_OBJ_GAME_WORLD1			14
#define BUTTON_OBJ_GAME_WORLD2			15
#define BUTTON_OBJ_GAME_WORLD3			16

class CExitWnd : public CMenu
{
public:
	
	BOOL	m_bChkBtn;
			
	BOOL	m_bGameReturn;
	BOOL	m_bCharExit;
	BOOL	m_bGameExit;
	BOOL	m_bDownChk;
	BOOL	m_bWorldChk;
	
	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;
	
	BOOL	bOtherBtn;

	BOOL	m_bPointer;
	
public:

	// Singleton Patten //
private:
	static CExitWnd* c_pThis;

public:
	static CExitWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CExitWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();

	// 체크 관련 함수 //	

	CExitWnd();
	~CExitWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//