#if !defined(AFX_ITEMUPGRADESUBWND_H__3E6579BA_96D5_40A1_8944_EB224352D825__INCLUDED_)
#define AFX_ITEMUPGRADESUBWND_H__3E6579BA_96D5_40A1_8944_EB224352D825__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#define ITEM_UPGRADE_PROCESS_START			0
#define ITEM_UPGRADE_PROCESS_READY			1
#define ITEM_UPGRADE_PROCESS_SUCCESS		2
#define ITEM_UPGRADE_PROCESS_FAIL			3
#define ITEM_UPGRADE_PROCESS_FAIL_INSURANCE	4


#define SPR_OBJ_ITEM_UPGRADE_SUB_WND		0
#define BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE1	1
#define BUTTON_OBJ_ITEMUPGRADE_SUB_CLOSE2	2
#define BUTTON_OBJ_ITEMUPGRADE_SUB_OK1		3
#define BUTTON_OBJ_ITEMUPGRADE_SUB_OK2		4
#define BUTTON_OBJ_ITEMUPGRADE_SUB_OK3		5


#include "Menu.h"


class CItemUpgradeSubWnd : public CMenu  
{

public:

	BYTE	m_byItemUpgradeProcess;
	RECT	m_rtPos;
	DWORD	m_dwNpcID;
	BOOL	m_bBtn;		

public:

	CItemUpgradeSubWnd();
	~CItemUpgradeSubWnd();

private:
	static CItemUpgradeSubWnd* c_pThis;

public:
	static CItemUpgradeSubWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemUpgradeSubWnd; return c_pThis; }
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
