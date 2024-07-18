// GuardianWnd.h: interface for the CGuardianWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUARDIANWND_H__D35B341E_9801_48C1_AFB4_21E2724B15BB__INCLUDED_)
#define AFX_GUARDIANWND_H__D35B341E_9801_48C1_AFB4_21E2724B15BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include	"Menu.h"

#define SPR_OBJ_GUARDIAN_WINDOWS1		0
#define SPR_OBJ_GUARDIAN_WINDOWS2		1
#define SPR_OBJ_GUARDIAN_CLOSE1			2
#define SPR_OBJ_GUARDIAN_CLOSE2			3
#define BUTTON_OBJ_G_EGO_UP1			4
#define BUTTON_OBJ_G_EGO_UP2			5
#define BUTTON_OBJ_G_STR_UP1			6
#define BUTTON_OBJ_G_STR_UP2			7
#define BUTTON_OBJ_G_INT_UP1			8
#define BUTTON_OBJ_G_INT_UP2			9
#define BUTTON_OBJ_G_DEX_UP1			10
#define BUTTON_OBJ_G_DEX_UP2			11
#define BUTTON_OBJ_G_VIT_UP1			12
#define BUTTON_OBJ_G_VIT_UP2			13

/*#######################################################
	Guardian Information Window Column Precisions
#######################################################*/
#define GUARDIAN_INFOTEXT_CHAR_WIDTH					7		// 글자 넓이
#define GUARDIAN_INFOTEXT_TOP_XPOS_RIGHT				306		// 직업, 종족, 길드, 기본스탯 (HP, SP, LEVEL, EXP)
#define GUARDIAN_INFOTEXT_BASIC_STAT_XPOS_LEFT			93		// EGO, STR, INT, DEX, VIT
#define GUARDIAN_INFOTEXT_BASIC_STAT_XPOS_RIGHT			317		// EGO, STR, INT, DEX, VIT
#define GUARDIAN_INFOTEXT_RESIST_STAT_XPOS_LEFT			247		// 저항력 스탯 (Physical, Flame, Ice, Electric, Poison)
#define GUARDIAN_INFOTEXT_RESIST_STAT_XPOS_RIGHT		305		// 저항력 스탯 (Physical, Flame, Ice, Electric, Poison)
#define GUARDIAN_THIS_TEXT_LEFT(_LEFT_, _SIZE_)	_LEFT_ - _SIZE_ * GUARDIAN_INFOTEXT_CHAR_WIDTH

enum GUARDIAN_STATUS_POINT_KIND
{
	GUARDIAN_STATUS_POINT_KIND_EGO = 0,
	GUARDIAN_STATUS_POINT_KIND_STR = 1,
	GUARDIAN_STATUS_POINT_KIND_INT = 2,
	GUARDIAN_STATUS_POINT_KIND_DEX = 3,
	GUARDIAN_STATUS_POINT_KIND_VIT = 4, 
	GUARDIAN_STATUS_POINT_KIND_MAX,
};

class CMonster;
struct CTDS_CHAR_LEVELUP;

class CGuardianWnd : public CMenu  
{
public:
	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;		

	BOOL	m_bBtn[5];

private:
	static CGuardianWnd* c_pThis;

	void	GuardianDisplay();
	void	Send_GuardianStatPoint(GUARDIAN_STATUS_POINT_KIND enStatusPointKind);
	void	SendStatPoint(GAME_OBJECT_TYPE enObjectType, GUARDIAN_STATUS_POINT_KIND enStatusPointKind);
	void	ShowAllStatIncreseButton(BOOL bShow);
	void	OnStatButtonEvent(GUARDIAN_STATUS_POINT_KIND enStatusPointKind, MOUSECHECK enMouseCheck);
	void	SpriteStatButtonEvent(GUARDIAN_STATUS_POINT_KIND enStatusPointKind, MOUSECHECK enMouseCheck);
	BOOL	IsStatPoint();
public:
	static CGuardianWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuardianWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }

	BOOL	Init();
			
	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	int		CheckInterface();	
	void	HideAllStatButton();
	CMonster*	GetMyGuardian();

public:
	CGuardianWnd();
	virtual ~CGuardianWnd();

};

#endif // !defined(AFX_GUARDIANWND_H__D35B341E_9801_48C1_AFB4_21E2724B15BB__INCLUDED_)
