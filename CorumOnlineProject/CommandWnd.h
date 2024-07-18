//======================================================//
// Code by Jang.							2002.1.7	//
// Obejct : Command Window.								//
//======================================================//
#ifndef		__COMMAND_WINDOW_INC__
#define		__COMMAND_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_COMMAND_WND1		0
#define SPR_OBJ_COMMAND_WND2		1
#define SPR_OBJ_COMMAND_WND3		2
#define SPR_OBJ_COMMAND_CLOSE1		3
#define SPR_OBJ_COMMAND_CLOSE2		4
#define SPR_OBJ_COMMAND_SC			5
#define SPR_OBJ_DFCMD				6

class CCommandWnd : public CMenu
{
public:

	BYTE	m_byIndex;
	BYTE	m_byMaxIndex;
	BYTE	m_byScrollChk;
	
	BOOL	m_bMessageChk;
		
	// 체크 관련 변수 //
	RECT	m_rtPos;
	SMSG	m_sMsg[__MAX_MSG_CNT__];

public:

	// Singleton Patten //
private:
	static CCommandWnd* c_pThis;

public:
	static CCommandWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CCommandWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();

	BOOL	MessageAdd(char* pszMessage, DWORD dwCol);

	void	SetSize(int nCount);

	CCommandWnd();
	~CCommandWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


