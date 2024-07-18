//======================================================//
// Code by Jang.							2002.12.23	//
// Obejct : Char Window.								//
//======================================================//
#ifndef		__NPCDUNGEONINFO_INC__
#define		__NPCDUNGEONINFO_INC__

#include	"Menu.h"

#define SPR_OBJ_NPCDUNGEONINFO_WND1			0
#define SPR_OBJ_NPCDUNGEONINFO_WND2			1
#define SPR_OBJ_NPCDUNGEONINFO_WND3			2
#define SPR_OBJ_NPCDUNGEONINFO_CLOSE1		3
#define SPR_OBJ_NPCDUNGEONINFO_CLOSE2		4

class CNpcDungeonInfoWnd : public CMenu
{
public:

	RECT	m_rtRect;
	char	m_szInfo[0xff];
		
	BYTE	m_bType;						

public:

	// Singleton Patten //
private:
	static CNpcDungeonInfoWnd* c_pThis;

public:
	static CNpcDungeonInfoWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CNpcDungeonInfoWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();	
	
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// 체크 관련 함수 //
	void	SetEffect();
	
	CNpcDungeonInfoWnd();
	virtual ~CNpcDungeonInfoWnd();
};
#endif
//======================================================//
// End.													//
//======================================================//