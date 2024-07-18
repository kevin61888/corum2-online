//======================================================//
// Code by Jang.							2002.5.27	//
// Obejct : Help Window.								//
//======================================================//
#ifndef		__HELP_WINDOW_INC__
#define		__HELP_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_HELP_WND1			0
#define SPR_OBJ_HELP_WND2			1
#define SPR_OBJ_HELP_WND3			2
#define SPR_OBJ_HELP_CLOSE1			3
#define SPR_OBJ_HELP_CLOSE2			4
#define SPR_OBJ_HELP_NEXT1			5
#define SPR_OBJ_HELP_NEXT2			6
#define SPR_OBJ_HELP_NEXT3			7
#define SPR_OBJ_HELP_HP				8
#define SPR_OBJ_HELP_SP				9
#define SPR_OBJ_HELP_COOL			10
#define SPR_OBJ_HELP_EXP			11
#define SPR_OBJ_HELP_ITEM			12
#define SPR_OBJ_HELP_CHAR			13
#define SPR_OBJ_HELP_SKILL			14
#define SPR_OBJ_HELP_GROUP			15
#define SPR_OBJ_HELP_DUNGEON		16
#define SPR_OBJ_HELP_CHAT			17
#define SPR_OBJ_HELP_SHOP			18
#define SPR_OBJ_HELP_MATCH			19
#define SPR_OBJ_HELP_REST			20
#define SPR_OBJ_HELP_TRADE			21
#define SPR_OBJ_HELP_PK				22
#define SPR_OBJ_HELP_OPTION			23
#define SPR_OBJ_HELP_SLOT			24
#define SPR_OBJ_HELP_SUPPLISE		25
#define SPR_OBJ_HELP_INTERFACE1		26
#define SPR_OBJ_HELP_UP1			27
#define SPR_OBJ_HELP_UP2			28
#define SPR_OBJ_HELP_INTERFACE2		29
#define SPR_OBJ_HELP_INTERFACE3		30

class CHelpWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;

	// 체크 관련 변수 //				
	BOOL	m_bPointer;

	BYTE	m_byType;
	
	BOOL	m_bBtnChk[2];

	BYTE	m_bySubType;

public:

	// Singleton Patten //
private:
	static CHelpWnd* c_pThis;

public:
	static CHelpWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CHelpWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	void	RenderTextHelp();
	void	SetClear();

	CHelpWnd();
	~CHelpWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


