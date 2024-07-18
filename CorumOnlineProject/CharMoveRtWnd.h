//======================================================//
// Code by Jang.							2002.5.27	//
// Obejct : Character Move Result Window.				//
//======================================================//
#ifndef		__CHARMOVERT_WND_INC__
#define		__CHARMOVERT_WND_INC__

#include	"Menu.h"

#define SPR_OBJ_CHARMOVERT_WND1		0
#define SPR_OBJ_CHARMOVERT_WND2		1
#define SPR_OBJ_CHARMOVERT_WND3		2
#define	SPR_OBJ_CHARMOVERT_OK1		3
#define	SPR_OBJ_CHARMOVERT_OK2		4
#define	SPR_OBJ_CHARMOVERT_OK3		5

class CCharMoveRtWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;

	// 체크 관련 변수 //				
	BOOL	m_bPointer;

	BOOL	m_bBtn;

	BYTE	m_byTotalCnt;
	BYTE	m_byMoveSuccess;
	BYTE	m_byMoveFail;

	char	m_szInfo[0xff];

	RECT	m_rtPos;

	BYTE	m_bySize;

	CHARMOVEINFO	m_sCharMoveInfo[4];

public:

	// Singleton Patten //
private:
	static CCharMoveRtWnd* c_pThis;

public:
	static CCharMoveRtWnd*	GetInstance()	{ if(!c_pThis) c_pThis = new CCharMoveRtWnd; return c_pThis; }
	static void				DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();	

	void	SetCharacter(char* szName, BYTE byNum, BYTE byResult);

	CCharMoveRtWnd();
	~CCharMoveRtWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


