//======================================================//
// Code by Jang.							2004.02.12	//
// Object : Guild Portal Window.						//
//======================================================//
#ifndef		__GUILDPORTAL_WINDOW_INC__
#define		__GUILDPORTAL_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_GUILDPORTAL_WND			0
#define SPR_OBJ_GUILDPORTAL_CLOSE1		1
#define SPR_OBJ_GUILDPORTAL_CLOSE2		2
#define SPR_OBJ_GUILDPORTAL_OK1			3
#define SPR_OBJ_GUILDPORTAL_OK2			4
#define SPR_OBJ_GUILDPORTAL_OK3			5
#define SPR_OBJ_GUILDPORTAL_CANCEL1		6
#define SPR_OBJ_GUILDPORTAL_CANCEL2		7
#define SPR_OBJ_GUILDPORTAL_CANCEL3		8

class CGuildPortalWnd : public CMenu
{
public:
			
	BOOL	m_bActiveChk;				

	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;

	char	m_szName[MAX_CHARACTER_NAME_LENGTH];

	DWORD	m_dwUserIndex;
	
	BYTE	m_byResult;
	
	BYTE	m_byIndex;

	BYTE	m_byType;

	BOOL	m_bBtn[2];

public:
	
private:
	static CGuildPortalWnd* c_pThis;

public:
	static CGuildPortalWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildPortalWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }

	
	BOOL	Init();
	
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	
	int		CheckInterface();		
	
	CGuildPortalWnd();
	~CGuildPortalWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//