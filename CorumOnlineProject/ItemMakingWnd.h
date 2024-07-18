#if !defined(AFX_ITEMMAKINGWND_H__7BD66B70_BA1D_4FA6_AD02_ABEC93A93800__INCLUDED_)
#define AFX_ITEMMAKINGWND_H__7BD66B70_BA1D_4FA6_AD02_ABEC93A93800__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "Menu.h"


#define MAX_ITEM_MAKING 4


#define ITEM_MAKING_PROCESS_DEFAULT			0
#define ITEM_MAKING_PROCESS_RESULT_WAITING	1
#define ITEM_MAKING_PROCESS_RESULT_SUCCESS	2
#define ITEM_MAKING_PROCESS_RESULT_FAIL		3


#define SPR_OBJ_ITEMMAKINGWND			0
#define SPR_OBJ_ITEMMAKING_HELPWND		1
#define BUTTON_OBJ_ITEMMAKING_CLOSE1	2
#define BUTTON_OBJ_ITEMMAKING_CLOSE2	3
#define BUTTON_OBJ_ITEMMAKING1			4	// MOUSE_UP
#define BUTTON_OBJ_ITEMMAKING2			5	// MOUSE_OVER
#define BUTTON_OBJ_ITEMMAKING3			6	// MOUSE_DOWN


class CItemMakingWnd : public CMenu  
{

public:

	CItem	m_pMakingItem[MAX_ITEM_MAKING];
	RECT	m_rtPos;
	BYTE	m_byProcess;
	BOOL	m_bBtn;

public:

	CItemMakingWnd();
	~CItemMakingWnd();

public:

private:
	static CItemMakingWnd* c_pThis;

public:
	static CItemMakingWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemMakingWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다.	
	BOOL	Init();
	void	RenderText();
	void	SetActive(BOOL bActive = TRUE);
	void	Remove();
	void	SetOrder();
	int		CheckInterface();

public:

	void	RenderItem();
	void	ItemInfoRender();
	void	SetButtonStatus(DWORD dwSprID, DWORD dwStatus);
};


#endif 
