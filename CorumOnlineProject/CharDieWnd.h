//======================================================//
// Code by Jang.							2004.01.17	//
// Obejct : CharDie Window.								//
//======================================================//
#ifndef		__CHARDIE_WINDOW_INC__
#define		__CHARDIE_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_CHARDIE_WND		0
#define SPR_OBJ_CHARDIE_OK1		1
#define SPR_OBJ_CHARDIE_OK2		2
#define SPR_OBJ_CHARDIE_OK3		3
	
class CCharDieWnd : public CMenu
{
public:	
			
	RECT	m_rtPos;

	BOOL	m_bBtnChk;

public:

	// Singleton Patten //	
private:
	static CCharDieWnd* c_pThis;

public:
	static CCharDieWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CCharDieWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();		

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();
	
	CCharDieWnd();
	~CCharDieWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//