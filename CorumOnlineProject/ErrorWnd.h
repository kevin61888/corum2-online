#pragma once
//======================================================//
// Code by Jang.							2003.05.29	//
// Obejct : Error Window.								//
//======================================================//
#ifndef		__ERROR_WINDOW_INC__
#define		__ERROR_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_ERROR_WND		0
#define SPR_OBJ_ERROR_OK1		1
#define SPR_OBJ_ERROR_OK2		2
#define SPR_OBJ_ERROR_OK3		3

class CErrorWnd : public CMenu
{
public:
		
	BOOL	m_bMessageChk;	
	BOOL	m_bExit;
		
	RECT	m_rtPos;

	BOOL	m_bBtnChk;

	char	m_szInfo[3][128];

	BYTE	m_byCount;
	
	BOOL	m_bPointer;

	BYTE	m_byType;

	BYTE	m_byTime;

	DWORD	m_dwCurTime;
	DWORD	m_dwPevTime;

public:

	// Singleton Patten //
private:
	static CErrorWnd* c_pThis;

public:
	static CErrorWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CErrorWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();		

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();	

	void	SetError(char* pszText, char* pszText2, char* pszText3, BYTE byCount);

	CErrorWnd();
	~CErrorWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//