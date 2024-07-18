// LoginWnd.h: interface for the CLoginWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGINWND_H__3EBE59C3_6548_40F0_81DE_EFB986754AE2__INCLUDED_)
#define AFX_LOGINWND_H__3EBE59C3_6548_40F0_81DE_EFB986754AE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"Menu.h"

class CLoginWnd   : public CMenu
{
public:
	// Singleton Patten //
private:
	static CLoginWnd* c_pThis;

public:
	static CLoginWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CLoginWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	

	int		CheckInterface();
	CLoginWnd();
	virtual ~CLoginWnd();

};

#endif // !defined(AFX_LOGINWND_H__3EBE59C3_6548_40F0_81DE_EFB986754AE2__INCLUDED_)
