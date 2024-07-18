//======================================================//
// Code by Jang.							2002.05.27	//
// Obejct : Default Window.								//
//======================================================//
#ifndef		__GAMEDEFAULT_WINDOW_INC__
#define		__GAMEDEFAULT_WINDOW_INC__

#include	"Menu.h"

#define	SPR_OBJ_GAMEDEFAULT_WND			0
#define	SPR_OBJ_GAMEDEFAULT_OK1			1
#define	SPR_OBJ_GAMEDEFAULT_OK2			2
#define	SPR_OBJ_GAMEDEFAULT_OK3			3
#define	SPR_OBJ_GAMEDEFAULT_CANCEL1		4
#define	SPR_OBJ_GAMEDEFAULT_CANCEL2		5
#define	SPR_OBJ_GAMEDEFAULT_CANCEL3		6

class CGameDefaultWnd : public CMenu
{
public:
	
	BOOL	m_bChar;
	BOOL	m_bBtnChk[2];	
	BYTE	m_byType;
	BYTE	m_byCount;
			
	DWORD	m_dwCurTime;	
	DWORD	m_dwPevTime;

	BOOL	m_bExit;
	RECT	m_rtRect[4];
	char	m_szInfo[4][TEXT_RENDER_BUFFER];

public:

	// Singleton Patten //
private:
	static CGameDefaultWnd* c_pThis;

public:
	static CGameDefaultWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGameDefaultWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
	
	
	void	RenderText();	
	void	Remove();	
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);		
	void	OpenWnd(char* pszCaption, char* pszText, char* pszText2, char* pszText3, float fPosX, float fPosY, BYTE byCount, BOOL bSound = FALSE);
	void	OpenWndEx(char* pszCaption, char* pszText, char* pszText2, char* pszText3, float fPosX, float fPosY, BYTE byCount);
		
	int		CheckInterface();

	void	SetChar(BOOL bChar);
	BOOL	GetChar();

	// 체크 관련 함수 //	

	CGameDefaultWnd();
	~CGameDefaultWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//