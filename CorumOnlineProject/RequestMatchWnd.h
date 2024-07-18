//======================================================//
// Code by Jang.							2003.06.12	// 
// Obejct : Item Trade Window.							//
//======================================================//
#ifndef		__REQUESTMATCHWND__
#define		__REQUESTMATCHWND__

#include	"Menu.h"

#define SPR_OBJ_REQUESTMATCH_WND		0
#define SPR_OBJ_REQUESTMATCH_CLOSE1		1
#define SPR_OBJ_REQUESTMATCH_CLOSE2		2
#define SPR_OBJ_REQUESTMATCH_OK1		3
#define SPR_OBJ_REQUESTMATCH_OK2		4
#define SPR_OBJ_REQUESTMATCH_OK3		5
#define SPR_OBJ_REQUESTMATCH_CANCEL1	6
#define SPR_OBJ_REQUESTMATCH_CANCEL2	7
#define SPR_OBJ_REQUESTMATCH_CANCEL3	8

class CRequestMatchWnd : public CMenu
{
public:
	enum { REQUEST_END_TICK = 10000 };		// 대전 신청 대기 시간 10초간 

	BOOL	m_bMoveChk;	

	char	m_szName[MAX_CHARACTER_NAME_LENGTH];

	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;		
	
	BOOL	m_bBtnChk[2];
	DWORD	m_dwRequestMatchUserIndex;
	
	DWORD	m_dwRequestMatchEndTick;	// 신청을 받고 대답해야할 시간
										// 넘어가면 자동으로 거절한것으로 
										// 알고 윈도우를 닫는다.
	
public:
	
private:
	static CRequestMatchWnd* c_pThis;

public:
	static CRequestMatchWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CRequestMatchWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
		
	int		CheckInterface();

	// 체크 관련 함수 //	

	CRequestMatchWnd();
	~CRequestMatchWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//
