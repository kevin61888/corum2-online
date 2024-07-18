//======================================================//
// Code by Jang.							2004.6.14	//
// Obejct : Party Board Info Window.					//
//======================================================//
#ifndef		__PARTYBOARDINFO_WND_INC__
#define		__PARTYBOARDINFO_WND_INC__

#include	"Menu.h"

#define		SPR_OBJ_PARTYBOARDINFO_WND1		0
#define		SPR_OBJ_PARTYBOARDINFO_WND2		1
#define		SPR_OBJ_PARTYBOARDINFO_WND3		2
#define		SPR_OBJ_PARTYBOARDINFO_WND4		3
#define		SPR_OBJ_PARTYBOARDINFO_CLOSE1	4
#define		SPR_OBJ_PARTYBOARDINFO_CLOSE2	5


class CPartyBoardInfoWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;	

	char	m_szInfo[0xff];

	RECT	m_rtPos;

public:

	// Singleton Patten //
private:
	static CPartyBoardInfoWnd* c_pThis;

public:
	static CPartyBoardInfoWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CPartyBoardInfoWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }

	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();		

	CPartyBoardInfoWnd();
	~CPartyBoardInfoWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


