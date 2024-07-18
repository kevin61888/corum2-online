//======================================================//
// Code by Jang.							2004.3.11	//
// Obejct : Rank Npc Window.							//
//======================================================//
#ifndef		__RANK_WINDOW_INC__
#define		__RANK_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_RANK_WND1		0
#define SPR_OBJ_RANK_WND2		1
#define SPR_OBJ_RANK_WND3		2
#define SPR_OBJ_RANK_CLOSE1		3
#define SPR_OBJ_RANK_CLOSE2		4

class CRankWnd : public CMenu
{
public:	

	DWORD		m_dwGuildId[4];
	DWORD		m_dwPoint[4];
	char		m_szGuildName[4][MAX_GUILD_NAME_LENGTH];	
	
	DWORD		m_dwRank;
	
public:

	// Singleton Patten //
private:
	static CRankWnd* c_pThis;

public:
	static CRankWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CRankWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();	

	CRankWnd();
	~CRankWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


