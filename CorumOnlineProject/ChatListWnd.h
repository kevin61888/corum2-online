//======================================================//
// Code by Jang.							2002.1.10	//
// Obejct : Chat Window.								//
//======================================================//
#ifndef		__CHATLIST_WINDOW_INC__
#define		__CHATLIST_WINDOW_INC__

#include	"Menu.h"

#define	SPR_OBJ_CHATLIST_WND1			0
#define	SPR_OBJ_CHATLIST_WND2			1
#define	SPR_OBJ_CHATLIST_WND3			2
#define	SPR_OBJ_CHATLIST_CLOSE1			3
#define	SPR_OBJ_CHATLIST_CLOSE2			4
#define	SPR_OBJ_CHATLIST_UPDOWN1		5
#define	SPR_OBJ_CHATLIST_UPDOWN2		6
#define	SPR_OBJ_CHATLIST_ALL1			7
#define	SPR_OBJ_CHATLIST_ALL2			8
#define	SPR_OBJ_CHATLIST_ALL3			9
#define	SPR_OBJ_CHATLIST_PARTY1			10
#define	SPR_OBJ_CHATLIST_PARTY2			11
#define	SPR_OBJ_CHATLIST_PARTY3			12
#define	SPR_OBJ_CHATLIST_GUILD1			13
#define	SPR_OBJ_CHATLIST_GUILD2			14
#define	SPR_OBJ_CHATLIST_GUILD3			15
#define	SPR_OBJ_CHATLIST_WHISPER1		16
#define	SPR_OBJ_CHATLIST_WHISPER2		17
#define	SPR_OBJ_CHATLIST_WHISPER3		18
#define	SPR_OBJ_CHATLIST_FRIEND1		19
#define	SPR_OBJ_CHATLIST_FRIEND2		20
#define	SPR_OBJ_CHATLIST_FRIEND3		21
#define	SPR_OBJ_CHATLIST_SC				22
#define	SPR_OBJ_CHATLIST_SC1			23
#define	SPR_OBJ_CHATLIST_SC2			24
#define	SPR_OBJ_CHATLIST_SC3			25
#define	SPR_OBJ_CHATLIST_SC4			26

class CChatListWnd : public CMenu
{
public:
		
	BYTE	m_byIndex;							
					
	RECT	m_rtPos;
	SMSG	m_sMsg[__MAX_MSG_CNT__];
	
	BYTE	m_bySize;
	BYTE	m_byMode;
	BYTE	m_byChatType;
	BYTE	m_byScrollChk;				

	BOOL	m_bChkBtn[4];	

public:
	
private:
	static CChatListWnd* c_pThis;

public:
	static CChatListWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CChatListWnd; return c_pThis; }
	static void				DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	BOOL	Init();	

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();	

	void	SetScroll();	
	
	CChatListWnd();
	~CChatListWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//