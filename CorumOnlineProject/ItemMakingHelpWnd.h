#if !defined(AFX_ITEMMAKINGHELPWND_H__256E8D91_8C6B_4853_8AC1_6B703EBF5613__INCLUDED_)
#define AFX_ITEMMAKINGHELPWND_H__256E8D91_8C6B_4853_8AC1_6B703EBF5613__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "Menu.h"


#define SPR_OBJ_NOTICE_WND1			0
#define SPR_OBJ_NOTICE_WND2			1
#define SPR_OBJ_NOTICE_WND3			2
#define SPR_OBJ_NOTICE_CLOSE1		3
#define SPR_OBJ_NOTICE_CLOSE2		4
#define SPR_OBJ_HELP_NEXT1			5
#define SPR_OBJ_HELP_NEXT2			6
#define SPR_OBJ_HELP_NEXT3			7


class CItemMakingHelpWnd : public CMenu  
{

public:

	CItemMakingHelpWnd();
	~CItemMakingHelpWnd();

private:
	static CItemMakingHelpWnd* c_pThis;

public:
	static CItemMakingHelpWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemMakingHelpWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	char	m_szHelpMessage[1024];
	BOOL	m_bBtn;
	BYTE	m_bypage;
	
	void	RenderTextEx	();
	void	SetButtonStatus	(DWORD dwSprID, DWORD dwStatus);
	
	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init			();	
	void	RenderText		();	
	void	Remove			();
	void	SetOrder		();	
	void	SetActive		(BOOL bActive = TRUE);	
	int		CheckInterface	();	

};

#endif 
