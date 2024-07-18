#ifndef		__EVENTDUNGEON_WINDOW_INC__
#define		__EVENTDUNGEON_WINDOW_INC__


#include	"Menu.h"


#define EVENT_ENTER									1
#define EVENT_FAILED								2
#define EVENT_DUNGEON_SURVIVAL_FIRST_TIMEOUT		3		// 생존던젼 1차 클리어.


#define SPR_OBJ_EVENTDUNGEON_WND1		0
#define SPR_OBJ_EVENTDUNGEON_WND2		1
#define SPR_OBJ_EVENTDUNGEON_WND3		2
#define SPR_OBJ_EVENT_CLOSE1			3
#define SPR_OBJ_EVENT_CLOSE2			4


class CEventDungeonWnd : public CMenu
{
public:
			
	RECT	m_rtPos;

	BOOL	m_bBtnChk;

	char	m_szInfo[0xff];
	
public:

	// Singleton Patten //
private:
	static CEventDungeonWnd* c_pThis;

public:
	static CEventDungeonWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CEventDungeonWnd; return c_pThis; }
	static void					DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();		

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();
	
	int		m_iMessageType;
	BYTE	m_bDungeonMonsterExpRatio;

	CEventDungeonWnd();
	~CEventDungeonWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//