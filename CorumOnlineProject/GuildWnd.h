//======================================================//
// Code by Jang.							2003.04.03	//
// Object : Guild Wnd.									//
//======================================================//
#ifndef		__GUILDWND_INC__
#define		__GUILDWND_INC__

#include	"Menu.h"

#define SPR_OBJ_GUILD_CREATE_WND		0
#define SPR_OBJ_GUILD_CLOSE1			1
#define SPR_OBJ_GUILD_CLOSE2			2
#define SPR_OBJ_GUILD_OK1				3
#define SPR_OBJ_GUILD_OK2				4
#define SPR_OBJ_GUILD_OK3				5
#define SPR_OBJ_GUILD_CANCEL1			6
#define SPR_OBJ_GUILD_CANCEL2			7
#define SPR_OBJ_GUILD_CANCEL3			8

class CGuildWnd : public CMenu
{
public:
	
	BYTE	m_bZipCode;
	BYTE	m_bGuildType;
		
	char	m_szInfo[0xff];
	RECT	m_rtPos;	
	
	BOOL	m_bBtn[2];

public:
	
	// Singleton Patten //
private:
	static CGuildWnd* c_pThis;

public:
	void SendGuildItemUse(BYTE btGuildType);
	void InputGuildName(BYTE	byGuildType);
	static CGuildWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();

	// 체크 관련 함수 //

	CGuildWnd();
	virtual ~CGuildWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//