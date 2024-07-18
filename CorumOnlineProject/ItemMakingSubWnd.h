#if !defined(AFX_ITEMMAKINGSUBWND_H__D5989DDC_967D_452A_81BE_D9C586BA15A0__INCLUDED_)
#define AFX_ITEMMAKINGSUBWND_H__D5989DDC_967D_452A_81BE_D9C586BA15A0__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#define ITEM_MAKING_PROCESS_START			0
#define ITEM_MAKING_PROCESS_READY			1
#define ITEM_MAKING_PROCESS_SUCCESS			2
#define ITEM_MAKING_PROCESS_FAIL			3


#define SPR_OBJ_ITEM_MAKING_SUB_WND			0
#define BUTTON_OBJ_ITEMMAKING_SUB_CLOSE1	1
#define BUTTON_OBJ_ITEMMAKING_SUB_CLOSE2	2
#define BUTTON_OBJ_ITEMMAKING_SUB_OK1		3
#define BUTTON_OBJ_ITEMMAKING_SUB_OK2		4
#define BUTTON_OBJ_ITEMMAKING_SUB_OK3		5


#include "Menu.h"


class CItemMakingSubWnd : public CMenu  
{

public:

	BYTE	m_byItemMakingProcess;
	RECT	m_rtPos;
	DWORD	m_dwNpcID;
	BOOL	m_bBtn;	

public:

	CItemMakingSubWnd();
	virtual ~CItemMakingSubWnd();

private:
	static CItemMakingSubWnd* c_pThis;

public:
	static CItemMakingSubWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemMakingSubWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }

	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();	
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	
	int		CheckInterface();

};


#endif
