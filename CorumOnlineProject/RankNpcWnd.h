//======================================================//
// Code by Jang.							2004.3.10	//
// Obejct : Rank Npc Window.							//
//======================================================//
#ifndef		__RANKNPC_WINDOW_INC__
#define		__RANKNPC_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_RANKNPC_WND1		0
#define SPR_OBJ_RANKNPC_WND2		1
#define SPR_OBJ_RANKNPC_WND3		2
#define SPR_OBJ_RANKNPC_CLOSE1		3
#define SPR_OBJ_RANKNPC_CLOSE2		4
#define SPR_OBJ_RANKNPC_RANK1		5
#define SPR_OBJ_RANKNPC_RANK2		6
#define SPR_OBJ_RANKNPC_RANK3		7

class CRankNpcWnd : public CMenu
{
public:
	
	BYTE		m_byLine;
	
	char		m_szRenderText[20][0xff];

	DWORD		m_dwGuildId[4];
	DWORD		m_dwPoint[4];
	char		m_szGuildName[4][MAX_GUILD_NAME_LENGTH];

	BOOL		m_bBtn;
	
	DWORD		m_dwRank;

public:

	// Singleton Patten //
private:
	static CRankNpcWnd* c_pThis;

public:
	static CRankNpcWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CRankNpcWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();
	
	void	SetText();

	CRankNpcWnd();
	~CRankNpcWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


