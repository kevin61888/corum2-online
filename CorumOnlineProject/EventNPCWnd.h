//======================================================//
// Obejct : Event NPC conversation dialog.				//
//======================================================//
#ifndef		__EVENTDUNGEON_NPC_WND_INC__
#define		__EVENTDUNGEON_NPC_WND_INC__


/*
#define SPR_OBJ_EVENT_NPC_CREATE_WND		0
#define SPR_OBJ_EVENT_NPC_CLOSE1			1
#define SPR_OBJ_EVENT_NPC_CLOSE2			2
#define SPR_OBJ_EVENT_NPC_OK1				3
#define SPR_OBJ_EVENT_NPC_OK2				4
#define SPR_OBJ_EVENT_NPC_OK3				5
#define SPR_OBJ_EVENT_NPC_CANCEL1			6
#define SPR_OBJ_EVENT_NPC_CANCEL2			7
#define SPR_OBJ_EVENT_NPC_CANCEL3			8

#include	"Menu.h"


//----------------------------------------------------------------------------
class CEventNPCWnd : public CMenu
{
public:
	BOOL	m_bBtn[2];	

	char	m_szRenderText[16][256];
	int		m_iTextLineNumber;

	char	m_szInfo[0xff];
	RECT	m_rtPos;

public:
	CEventNPCWnd();
	~CEventNPCWnd();

	static CEventNPCWnd* GetInstance()
	{
		static CEventNPCWnd cOkWnd;
		return &cOkWnd;
	};

	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();
};*/

//----------------------------------------------------------------------------


/*#define SPR_OBJ_EVENT_NPC_WND			0
#define SPR_OBJ_EVENT_NPC_CLOSE1		1
#define SPR_OBJ_EVENT_NPC_CLOSE2		2
#define SPR_OBJ_EVENT_NPC_OK1			3
#define SPR_OBJ_EVENT_NPC_OK2			4
#define SPR_OBJ_EVENT_NPC_OK3			5
#define SPR_OBJ_EVENT_NPC_CANCEL1		6
#define SPR_OBJ_EVENT_NPC_CANCEL2		7
#define SPR_OBJ_EVENT_NPC_CANCEL3		8*/

#define SPR_OBJ_EVENT_NPC_WND1			0
#define SPR_OBJ_EVENT_NPC_WND2			1
#define SPR_OBJ_EVENT_NPC_WND3			2
#define SPR_OBJ_EVENT_NPC_CLOSE1		3
#define SPR_OBJ_EVENT_NPC_CLOSE2		4
#define SPR_OBJ_EVENT_NPC_OK1			5
#define SPR_OBJ_EVENT_NPC_OK2			6
#define SPR_OBJ_EVENT_NPC_OK3			7
#define SPR_OBJ_EVENT_NPC_CANCEL1		8
#define SPR_OBJ_EVENT_NPC_CANCEL2		9
#define SPR_OBJ_EVENT_NPC_CANCEL3		10

class CEventNPCWnd : public CMenu
{
public:
			
	char	m_szInfo[0xff];
	RECT	m_rtPos;	
	
	char	m_szRenderText[16][256];
	int		m_iTextLineNumber;

	BOOL	m_bChk;
	BYTE	m_byType;
	
	BOOL	m_bBtn[2];
	
public:
	
	// Singleton Patten //
private:
	static CEventNPCWnd* c_pThis;

public:
	static CEventNPCWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CEventNPCWnd; return c_pThis; }
	static void				DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();

	// 체크 관련 함수 //		

	CEventNPCWnd();
	~CEventNPCWnd();
};




#endif
