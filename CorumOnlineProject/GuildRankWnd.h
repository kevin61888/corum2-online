//==========================================================//
// Code by Jang.								2003.04.29	//
// Object : Guild Rank Setting Wnd.							//	
//==========================================================//
#ifndef		__GUILD_RANKWND_INC__
#define		__GUILD_RANKWND_INC__

#include "Menu.h"

#define SPR_OBJ_GUILDRANK_WND			0
#define SPR_OBJ_GUILDRANK_CLOSE1		1
#define SPR_OBJ_GUILDRANK_CLOSE2		2
#define SPR_OBJ_GUILDRANK_OK1			3
#define SPR_OBJ_GUILDRANK_OK2			4
#define SPR_OBJ_GUILDRANK_OK3			5
#define SPR_OBJ_GUILDRANK_CANCEL1		6
#define SPR_OBJ_GUILDRANK_CANCEL2		7
#define SPR_OBJ_GUILDRANK_CANCEL3		8

class CGuildRankWnd : public CMenu
{
public:
	
	char	m_szInfo[0xff];
	RECT	m_rtPos;	

	BOOL	m_bBtn[2];

public:
	
private:
	static CGuildRankWnd* c_pThis;

public:
	static CGuildRankWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildRankWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }

	
	BOOL	Init();
	
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	void	GuildMsg();

	int		CheckInterface();
		
	CGuildRankWnd();
	virtual ~CGuildRankWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//