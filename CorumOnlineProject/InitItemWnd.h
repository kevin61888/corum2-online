//==========================================================//
// Code by Jang.								2003.05.09	//
//==========================================================//
#ifndef		__INITITEM__WND__
#define		__INITITEM__WND__

#include	"Menu.h"

#define SPR_OBJ_INITITEM_WND		0
#define SPR_OBJ_INITITEM_CLOSE1		1
#define SPR_OBJ_INITITEM_CLOSE2		2
#define SPR_OBJ_INITITEM_OK1		3
#define SPR_OBJ_INITITEM_OK2		4
#define SPR_OBJ_INITITEM_OK3		5
#define SPR_OBJ_INITITEM_CANCEL1	6
#define SPR_OBJ_INITITEM_CANCEL2	7
#define SPR_OBJ_INITITEM_CANCEL3	8

class CInitItemWnd : public CMenu
{
public:
			
	char	m_szInfo[0xff];
	RECT	m_rtPos;
	
	BYTE	m_byIndex;
	BYTE	m_byType;

	char	m_szName[MAX_CHARACTER_NAME_LENGTH];
	char	m_szRequestName[MAX_CHARACTER_NAME_LENGTH];

	BOOL	m_bBtn[2];
	
public:

	// Singleton Patten //
private:
	static CInitItemWnd* c_pThis;

public:
	static CInitItemWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CInitItemWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();

	// 체크 관련 함수 //
	CInitItemWnd();
	~CInitItemWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//