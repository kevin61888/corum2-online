//==========================================================//
// Code by Jang.								2003.04.08	//
// Object : Guild Ex.										//
//==========================================================//
#ifndef		__GUILDEX_INC__
#define		__GUILDEX_INC__

#include	"Menu.h"

#define SPR_OBJ_GUILDEX_WND			0
#define SPR_OBJ_GUILDEX_CLOSE1		1
#define SPR_OBJ_GUILDEX_CLOSE2		2
#define SPR_OBJ_GUILDEX_OK1			3
#define SPR_OBJ_GUILDEX_OK2			4
#define SPR_OBJ_GUILDEX_OK3			5
#define SPR_OBJ_GUILDEX_CANCEL1		6
#define SPR_OBJ_GUILDEX_CANCEL2		7
#define SPR_OBJ_GUILDEX_CANCEL3		8

class CGuildExWnd : public CMenu
{
public:

	BYTE	m_byGuildType;	
			
	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;
	
	BYTE	m_byChk;
	
	BOOL	m_bBtn[2];

public:

	// Singleton Patten //
private:
	static CGuildExWnd* c_pThis;

public:
	static CGuildExWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildExWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
	
	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();

	// 체크 관련 함수 //
	void	GuildMsg();

	CGuildExWnd();
	virtual ~CGuildExWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//