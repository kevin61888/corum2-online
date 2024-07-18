//
//	Game Notice message display window.
//
//				2003/11/13
//				Trust Pak


#ifndef __GAME_NOTICE_MESSAGE_WINDOW__
#define	__GAME_NOTICE_MESSAGE_WINDOW__

#define SPR_OBJ_NOTICE_WND1			0
#define SPR_OBJ_NOTICE_WND2			1
#define SPR_OBJ_NOTICE_WND3			2
#define SPR_OBJ_NOTICE_CLOSE1		3
#define SPR_OBJ_NOTICE_CLOSE2		4

//------------------------------------------------------------------------====
class CGameNoticeMessageWnd : public CMenu
{
public:
	
	// Singleton Patten //
private:
	static CGameNoticeMessageWnd* c_pThis;

public:
	static CGameNoticeMessageWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGameNoticeMessageWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	BOOL	Init();		

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	void	SetPosition(float fX, float fZ);

	void	RenderTextEx();
	int		CheckInterface();	

	CGameNoticeMessageWnd();
	~CGameNoticeMessageWnd();

public:
	float	m_fWndWidth;
	float	m_fWndHeight;
};

//----------------------------------------------------------------------------


#endif