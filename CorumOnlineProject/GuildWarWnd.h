//==========================================================//
// Code by Jang.								2004.02.16	//
//==========================================================//
#ifndef		__GUILDWAR__WND__
#define		__GUILDWAR__WND__

#include	"Menu.h"

#define SPR_OBJ_GUILDWAR_WND		0
#define SPR_OBJ_GUILDWAR_CLOSE1		1
#define SPR_OBJ_GUILDWAR_CLOSE2		2
#define SPR_OBJ_GUILDWAR_OK1		3
#define SPR_OBJ_GUILDWAR_OK2		4
#define SPR_OBJ_GUILDWAR_OK3		5
#define SPR_OBJ_GUILDWAR_CANCEL1	6
#define SPR_OBJ_GUILDWAR_CANCEL2	7
#define SPR_OBJ_GUILDWAR_CANCEL3	8

class CGuildWarWnd : public CMenu
{
public:
			
	char	m_szInfo[0xff];
	RECT	m_rtPos;

	DWORD	m_dwGuildId;	
	DWORD	m_dwUserIndex;

	char	m_szGuildName[MAX_GUILD_NAME_LENGTH];

	BOOL	m_bChk;
	BYTE	m_byType;
	
	BOOL	m_bBtn[2];
	
public:
	
	// Singleton Patten //
private:
	static CGuildWarWnd* c_pThis;

public:
	static CGuildWarWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildWarWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();

	// 체크 관련 함수 //		

	CGuildWarWnd();
	~CGuildWarWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//
