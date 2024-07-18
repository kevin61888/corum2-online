#pragma once
//======================================================//
// Code by Jang.							2004.1.16	//
// Obejct : World Window.								//
//======================================================//
#ifndef		__WORLD_WINDOW_INC__
#define		__WORLD_WINDOW_INC__

#include	"Menu.h"


#define SPR_OBJ_WORLD_WND1		0
#define SPR_OBJ_WORLD_WND2		1
#define SPR_OBJ_WORLD_WND3		2
#define SPR_OBJ_WORLD_CLOSE1	3
#define SPR_OBJ_WORLD_CLOSE2	4

class CWorldWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;		
	BOOL	m_bPointer;
	
	BYTE	m_bInfoType;
	char	szMessageLine[7][255];

public:

	// Singleton Patten //
private:
	static CWorldWnd* c_pThis;

public:
	static CWorldWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CWorldWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();		

	CWorldWnd();
	~CWorldWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


