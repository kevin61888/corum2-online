#pragma once
//==========================================================//
// Code by Jang.								2003.02.21	//
//==========================================================//
#ifndef		__QUANTITY__WND__
#define		__QUANTITY__WND__

#include	"Menu.h"

#define SPR_OBJ_QUANTITY_WND		0
#define SPR_OBJ_QUANTITY_CLOSE1		1
#define SPR_OBJ_QUANTITY_CLOSE2		2
#define SPR_OBJ_NAME_TRUE1			3
#define SPR_OBJ_NAME_TRUE2			4
#define SPR_OBJ_NAME_TRUE3			5
#define SPR_OBJ_NAME_FALSE1			6
#define SPR_OBJ_NAME_FALSE2			7
#define SPR_OBJ_NAME_FALSE3			8

class CQuantityWnd : public CMenu
{
public:
	
	RECT	m_rtPos[4];
	char	m_szInfo[0xff];
	char	m_szMoney[32];

	BYTE	m_bQuantityType;
	BYTE	m_byDrawIndex;	
	BYTE	m_bySkill;
	BYTE	m_byOrder;
	BYTE	m_byChatLine;
	BYTE	m_byZipCode;
	BYTE	m_bIsOkButton;
	
	BOOL	m_bReq;				
	BOOL	m_bBtn[2];

	// 체크 관련 변수 //	
	
	BYTE	m_byQuantityAct;

	float	m_fItemX;
	float	m_fItemY;
	float	m_fItemZ;
	
public:

	
	// Singleton Patten //
private:
	static CQuantityWnd* c_pThis;

public:
	static CQuantityWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CQuantityWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //	
	BOOL	Init();
	BOOL	IsOk()	{	return m_bIsOkButton;	}
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);	
	
	int		CheckInterface();

	// 체크 관련 함수 //	
	void	OpenWnd(BYTE bQuantityType);	
	void	QuantityMsg();
	
	CQuantityWnd();
	~CQuantityWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//