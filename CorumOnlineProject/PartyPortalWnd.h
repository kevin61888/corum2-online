//======================================================//
// Code by Jang.							2002.07.30	//
// Object : Party Portal Window.						//
//======================================================//
#ifndef		__PARTYPORTAL_WINDOW_INC__
#define		__PARTYPORTAL_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_PARTYPORTAL_WND			0
#define SPR_OBJ_PARTYPORTAL_CLOSE1		1
#define SPR_OBJ_PARTYPORTAL_CLOSE2		2
#define SPR_OBJ_PARTYPORTAL_OK1			3
#define SPR_OBJ_PARTYPORTAL_OK2			5
#define SPR_OBJ_PARTYPORTAL_OK3			6
#define SPR_OBJ_PARTYPORTAL_CANCEL1		7
#define SPR_OBJ_PARTYPORTAL_CANCEL2		8
#define SPR_OBJ_PARTYPORTAL_CANCEL3		9

class CPartyPortalWnd : public CMenu
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
	static CPartyPortalWnd* c_pThis;

public:
	static CPartyPortalWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CPartyPortalWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }

	
	BOOL	Init();
	
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	void	OpenWnd();
		
	int		CheckInterface();		
	
	CPartyPortalWnd();
	~CPartyPortalWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//