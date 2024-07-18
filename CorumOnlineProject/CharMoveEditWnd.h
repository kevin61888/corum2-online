//======================================================//
// Code by Jang.							2004.6.03	//
// Obejct : Character Move Edit Window.					//
//======================================================//
#ifndef		__CHARMOVEEDIT_WND_INC__
#define		__CHARMOVEEDIT_WND_INC__

#include	"Menu.h"

#define SPR_OBJ_CHARMOVEEDIT_WND1		0
#define SPR_OBJ_CHARMOVEEDIT_WND2		1
#define SPR_OBJ_CHARMOVEEDIT_WND3		2
#define	SPR_OBJ_CHARMOVEEDIT_OK1		3
#define	SPR_OBJ_CHARMOVEEDIT_OK2		4
#define	SPR_OBJ_CHARMOVEEDIT_OK3		5

class CCharMoveEditWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;

	// 체크 관련 변수 //				
	BOOL	m_bPointer;

	BOOL	m_bBtn;

	BOOL	m_bNameChk;
	
	BYTE	m_byMoveFail;

	char	m_szInfo[0xff];

	RECT	m_rtPos;	
	
	BOOL	m_bInputMode;
	BYTE	m_byInputIndex;

	CHARMOVEINFOEIDT	m_sCharMoveInfoEdit[4];

public:

	// Singleton Patten //
private:
	static CCharMoveEditWnd* c_pThis;

public:
	static CCharMoveEditWnd*	GetInstance()	{ if(!c_pThis) c_pThis = new CCharMoveEditWnd; return c_pThis; }
	static void				DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();	

	void	SetCharacter(char* szName, BYTE byNum, BYTE byResult);

	CCharMoveEditWnd();
	~CCharMoveEditWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


