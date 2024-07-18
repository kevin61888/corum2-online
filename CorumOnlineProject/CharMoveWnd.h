//==========================================================//
// Code by Jang.								2004.05.24	//
//==========================================================//
#ifndef		__CHARMOVE__WND__
#define		__CHARMOVE__WND__

#include	"Menu.h"

#define	SPR_OBJ_CHARMOVE_WND		0
#define	SPR_OBJ_CHARMOVE_CLOSE1		1
#define	SPR_OBJ_CHARMOVE_CLOSE2		2
#define	SPR_OBJ_CHARMOVE_OK1		3
#define	SPR_OBJ_CHARMOVE_OK2		4
#define	SPR_OBJ_CHARMOVE_OK3		5
#define	SPR_OBJ_CHARMOVE_CANCEL1	6
#define	SPR_OBJ_CHARMOVE_CANCEL2	7
#define	SPR_OBJ_CHARMOVE_CANCEL3	8

class CCharMoveWnd : public CMenu
{
public:
			
	char	m_szInfo[3][0xff];
	BYTE	m_byCnt;

	RECT	m_rtPos;

	BYTE	m_byType;

	BOOL	m_bPackChk;

	BOOL	m_bBtn[2];
	
public:
	
	// Singleton Patten //
private:
	static CCharMoveWnd* c_pThis;

public:
	static CCharMoveWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CCharMoveWnd; return c_pThis; }
	static void				DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	void	SetText(char* pszText1, char* pszText2, char* pszText3, BYTE byCnt);

	int		CheckInterface();

	// 체크 관련 함수 //		

	CCharMoveWnd();
	~CCharMoveWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//
