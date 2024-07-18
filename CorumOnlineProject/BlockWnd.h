#pragma once
//======================================================//
// Code by Jang.							2004.1.16	//
// Obejct : World Window.								//
//======================================================//
#ifndef		__BLOCK_WINDOW_INC__
#define		__BLOCK_WINDOW_INC__

#include	"Menu.h"

#define MAX_BLOCK_MESSAGE_LINE_COUNT	7

#define SPR_OBJ_BLOCK_WND1		0
#define SPR_OBJ_BLOCK_WND2		1
#define SPR_OBJ_BLOCK_WND3		2
#define SPR_OBJ_BLOCK_CLOSE1	3
#define SPR_OBJ_BLOCK_CLOSE2	4

class CBlockWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;		
	
	BYTE	m_bInfoType;
	char	szMessageLine[MAX_BLOCK_MESSAGE_LINE_COUNT][255];

public:

	// Singleton Patten //
private:
	static CBlockWnd* c_pThis;

public:
	static CBlockWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CBlockWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();	

	CBlockWnd();
	~CBlockWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//


