//======================================================//
// Code by Jang.							2004.6.9	//
// Obejct : Party Help Window.							//
//======================================================//
#ifndef		__PARTYHELP_WND_INC__
#define		__PARTYHELP_WND_INC__

#include	"Menu.h"

#define		SPR_OBJ_PARTYHELP_WND1			0
#define		SPR_OBJ_PARTYHELP_WND2			1
#define		SPR_OBJ_PARTYHELP_WND3			2
#define		SPR_OBJ_PARTYHELP_CLOSE1		3
#define		SPR_OBJ_PARTYHELP_CLOSE2		4
#define		SPR_OBJ_PARTYHELP_HELP1			5
#define		SPR_OBJ_PARTYHELP_HELP2			6
#define		SPR_OBJ_PARTYHELP_HELP3			7

class CPartyHelpWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;	

	char	m_szInfo[0xff];

	RECT	m_rtPos;

public:

	// Singleton Patten //
private:
	static CPartyHelpWnd* c_pThis;

public:
	static CPartyHelpWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CPartyHelpWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();		

	CPartyHelpWnd();
	~CPartyHelpWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


