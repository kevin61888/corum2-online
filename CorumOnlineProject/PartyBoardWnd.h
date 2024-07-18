//======================================================//
// Code by Jang.							2004.6.8	//
// Obejct : Party Board Window.							//
//======================================================//
#ifndef		__PARTYBOARD_WND_INC__
#define		__PARTYBOARD_WND_INC__

#include	"Menu.h"

#define		SPR_OBJ_PARTYBOARD_WND1			0
#define		SPR_OBJ_PARTYBOARD_WND2			1
#define		SPR_OBJ_PARTYBOARD_WND3			2
#define		SPR_OBJ_PARTYBOARD_CLOSE1		3
#define		SPR_OBJ_PARTYBOARD_CLOSE2		4
#define		SPR_OBJ_PARTYBOARD_PARTYENTRY1	5
#define		SPR_OBJ_PARTYBOARD_PARTYENTRY2	6
#define		SPR_OBJ_PARTYBOARD_PARTYENTRY3	7
#define		SPR_OBJ_PARTYBOARD_PARTYSEA1	8
#define		SPR_OBJ_PARTYBOARD_PARTYSEA2	9
#define		SPR_OBJ_PARTYBOARD_PARTYSEA3	10
#define		SPR_OBJ_PARTYBOARD_PARTYHELP1	11
#define		SPR_OBJ_PARTYBOARD_PARTYHELP2	12
#define		SPR_OBJ_PARTYBOARD_PARTYHELP3	13

class CPartyBoardWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;	

	BOOL	m_bBtn[3];

	char	m_szInfo[0xff];

	RECT	m_rtPos;
	
	char	m_szText[5][0xff];

	int		m_nSize[5];

	BYTE	m_byLine;

	DWORD	m_dwTime;

	BYTE	m_byErrChk;

public:

	// Singleton Patten //
private:
	static CPartyBoardWnd* c_pThis;

public:
	static CPartyBoardWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CPartyBoardWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();	

	void	SetText(char* pszText);

	CPartyBoardWnd();
	~CPartyBoardWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


