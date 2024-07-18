//======================================================//
// Code by Jang.										//
// Obejct : Notice Window.								//
//======================================================//
#ifndef		__NOTICE_WINDOW_INC__
#define		__NOTICE_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_NOTICE_WND1			0
#define SPR_OBJ_NOTICE_WND2			1
#define SPR_OBJ_NOTICE_WND3			2
#define SPR_OBJ_NOTICE_CLOSE1		3
#define SPR_OBJ_NOTICE_CLOSE2		4


class CNoticeWnd : public CMenu
{
public:

	// Singleton Patten //
private:
	static CNoticeWnd* c_pThis;

public:
	static CNoticeWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CNoticeWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();		

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	void	RenderTextEx();
	int		CheckInterface();	

	char	m_szNoticeMessage[1024];

	CNoticeWnd();
	~CNoticeWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//