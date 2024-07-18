//==========================================================//
// Code by Jang.								2003.11.10	//
//==========================================================//
#ifndef		__MESSENGER__WND__
#define		__MESSENGER__WND__

#include	"Menu.h"

#define SPR_OBJ_MESSENGER_WND			0
#define SPR_OBJ_MESSENGER_CLOSE1		1
#define SPR_OBJ_MESSENGER_CLOSE2		2
#define SPR_OBJ_MESSENGER_OK1			3
#define SPR_OBJ_MESSENGER_OK2			4
#define SPR_OBJ_MESSENGER_OK3			5
#define SPR_OBJ_MESSENGER_CANCEL1		6
#define SPR_OBJ_MESSENGER_CANCEL2		7
#define SPR_OBJ_MESSENGER_CANCEL3		8

class CMessengerWnd : public CMenu
{
public:
			
	char	m_szInfo[0xff];
	RECT	m_rtPos;

	DWORD	m_dwUserIndex;	

	char	m_szName[MAX_CHARACTER_NAME_LENGTH];	

	BOOL	m_bChk;

	BOOL	m_bBtn[2];
	
public:

	
	// Singleton Patten //
private:
	static CMessengerWnd* c_pThis;

public:
	static CMessengerWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CMessengerWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();

	// 체크 관련 함수 //	

	CMessengerWnd();
	~CMessengerWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//