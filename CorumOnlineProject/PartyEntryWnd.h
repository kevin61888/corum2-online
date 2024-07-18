//======================================================//
// Code by Jang.							2004.6.8	//
// Obejct : Party Entry Window.							//
//======================================================//
#ifndef		__PARTYENTRY_WND_INC__
#define		__PARTYENTRY_WND_INC__

#include	"Menu.h"

#define		PARTYENTRY_INPUT			0
#define		PARTYENTRY_RESULT			1

#define		SPR_OBJ_PARTYENTRY_WND1		0
#define		SPR_OBJ_PARTYENTRY_WND2		1
#define		SPR_OBJ_PARTYENTRY_WND3		2
#define		SPR_OBJ_PARTYENTRY_CLOSE1	3
#define		SPR_OBJ_PARTYENTRY_CLOSE2	4
#define		SPR_OBJ_PARTYENTRY_OK1		5
#define		SPR_OBJ_PARTYENTRY_OK2		6
#define		SPR_OBJ_PARTYENTRY_OK3		7
#define		SPR_OBJ_PARTYENTRY_CANCEL1	8
#define		SPR_OBJ_PARTYENTRY_CANCEL2	9
#define		SPR_OBJ_PARTYENTRY_CANCEL3	10
/*#define		SPR_OBJ_PARTYENTRY_FIRST1	11
#define		SPR_OBJ_PARTYENTRY_FIRST2	12
#define		SPR_OBJ_PARTYENTRY_FIRST3	13*/

#define		MAX_PARTYENTRY_TEXT			32

class CPartyEntryWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;	

	BOOL	m_bBtn[2];

	char	m_szInfo[0xff];

	RECT	m_rtPos;

	BYTE	m_byEntryType;
	
public:

	// Singleton Patten //
private:
	static CPartyEntryWnd* c_pThis;

public:
	static CPartyEntryWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CPartyEntryWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();	

	CPartyEntryWnd();
	~CPartyEntryWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


