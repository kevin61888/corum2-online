//======================================================//
// Code by Jang.							2003.04.03	//
// Object : Guild Join Wnd.								//
//======================================================//
#ifndef		__GUILDJOINWND_INC__
#define		__GUILDJOINWND_INC__

#include	"Menu.h"

#define SPR_OBJ_GUILDJOIN_CREATEWND		0
#define SPR_OBJ_GUILDJOIN_CLOSE1		1
#define SPR_OBJ_GUILDJOIN_CLOSE2		2
#define SPR_OBJ_GUILDJOIN_OK1			3
#define SPR_OBJ_GUILDJOIN_OK2			4
#define SPR_OBJ_GUILDJOIN_OK3			5
#define SPR_OBJ_GUILDJOIN_CANCEL1		6
#define SPR_OBJ_GUILDJOIN_CANCEL2		7
#define SPR_OBJ_GUILDJOIN_CANCEL3		8

class CGuildJoinWnd : public CMenu
{
public:

	char	m_szGuildName[MAX_GUILD_NAME_LENGTH];
	char	m_szName[MAX_CHARACTER_NAME_LENGTH];

	BYTE	m_bGuildType;
				
	char	m_szInfo[0xff];
	RECT	m_rtPos;		

	DWORD	m_dwUserIndex;
	
	BOOL	m_bMsgChk;	
	
	BOOL	m_bBtn[2];

public:
	
	// Singleton Patten //
private:
	static CGuildJoinWnd* c_pThis;

public:
	static CGuildJoinWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildJoinWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();

	// 체크 관련 함수 //		

	CGuildJoinWnd();
	virtual ~CGuildJoinWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//
