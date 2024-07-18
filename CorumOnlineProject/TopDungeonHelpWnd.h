// TopDungeonHelpWnd.h: interface for the CTopDungeonHelpWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOPDUNGEONHELPWND_H__028E5707_CB82_4808_BA8B_6A3BFD6736CD__INCLUDED_)
#define AFX_TOPDUNGEONHELPWND_H__028E5707_CB82_4808_BA8B_6A3BFD6736CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Menu.h"

#define SPR_OBJ_NOTICE_WND1			0
#define SPR_OBJ_NOTICE_WND2			1
#define SPR_OBJ_NOTICE_WND3			2
#define SPR_OBJ_NOTICE_CLOSE1		3
#define SPR_OBJ_NOTICE_CLOSE2		4
#define SPR_OBJ_HELP_NEXT1			5
#define SPR_OBJ_HELP_NEXT2			6
#define SPR_OBJ_HELP_NEXT3			7
#define SPR_OBJ_HELP_END1			8
#define SPR_OBJ_HELP_END2			9
#define SPR_OBJ_HELP_END3			10
#define SPR_OBJ_HELP_OK1			11
#define SPR_OBJ_HELP_OK2			12
#define SPR_OBJ_HELP_OK3			13
#define SPR_OBJ_HELP_CANCEL1		14
#define SPR_OBJ_HELP_CANCEL2		15
#define SPR_OBJ_HELP_CANCEL3		16
#define SPR_OBJ_HELP_ONLY_OK1		17
#define SPR_OBJ_HELP_ONLY_OK2		18
#define SPR_OBJ_HELP_ONLY_OK3		19

#define	HELP_TEXT_PAGE1_START		462
#define	HELP_TEXT_PAGE1_END			466
#define HELP_TEXT_PAGE2_1_START		467
#define HELP_TEXT_PAGE2_1_END		468
#define HELP_TEXT_PAGE2_2_START		477
#define HELP_TEXT_PAGE2_2_END		478
#define HELP_TEXT_PAGE2_3_START		479
#define HELP_TEXT_PAGE2_3_END		481
#define HELP_TEXT_PAGE3_1_START		469
#define HELP_TEXT_PAGE3_1_END		471
#define HELP_TEXT_PAGE4_1_START		472
#define HELP_TEXT_PAGE4_1_END		473
#define HELP_TEXT_PAGE5_1_START		474
#define HELP_TEXT_PAGE5_1_END		476

#define	HELP_TEXT_PAGE1_START_2WORLD		482
#define	HELP_TEXT_PAGE1_END_2WORLD			486
#define HELP_TEXT_PAGE2_1_START_2WORLD		487
#define HELP_TEXT_PAGE2_1_END_2WORLD		488
#define HELP_TEXT_PAGE2_2_START_2WORLD		496
#define HELP_TEXT_PAGE2_2_END_2WORLD		497
#define HELP_TEXT_PAGE2_3_START_2WORLD		498
#define HELP_TEXT_PAGE2_3_END_2WORLD		499
#define HELP_TEXT_PAGE3_1_START_2WORLD		489
#define HELP_TEXT_PAGE3_1_END_2WORLD		491
#define HELP_TEXT_PAGE4_1_START_2WORLD		492
#define HELP_TEXT_PAGE4_1_END_2WORLD		492
#define HELP_TEXT_PAGE5_1_START_2WORLD		493
#define HELP_TEXT_PAGE5_1_END_2WORLD		495

#define HELP_TEXT_CHECK_COMMAND1	10
#define HELP_TEXT_CHECK_COMMAND2	11
#define HELP_TEXT_CHECK_COMMAND3	12
#define HELP_TEXT_CHECK_COMMAND4	13

#define MAX_HELP_TEXT_COMMAND		5
#define MAX_HELP_TEXT_PAGE			4
struct PAGE_POSITION
{
	BYTE byCommand;
	BYTE byPage;
};

struct HELP_TEXT_START_END
{
	WORD wStart;
	WORD wEnd;
};
class CTopDungeonHelpWnd   : public CMenu
{
	BYTE	m_bySelect;
	BOOL	m_bBtn;
	PAGE_POSITION	m_sPagePosition;
	HELP_TEXT_START_END	m_sHelpTextBuffer[MAX_HELP_TEXT_COMMAND][MAX_HELP_TEXT_PAGE];
public:
	BYTE	m_byWorldNum;	//1:1월드맵(초기값), 2:2월드맵

	// Singleton Patten //
private:
	static CTopDungeonHelpWnd* c_pThis;

public:
	static CTopDungeonHelpWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CTopDungeonHelpWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();	

	void	RenderText();	
	void	Remove();		
	void	SetOrder();	
	void	RenderTextEx();
	void	SetActive(BOOL bActive = TRUE );
	
	int		CheckInterface();	

	CTopDungeonHelpWnd();
	virtual ~CTopDungeonHelpWnd();

};

#endif // !defined(AFX_TOPDUNGEONHELPWND_H__028E5707_CB82_4808_BA8B_6A3BFD6736CD__INCLUDED_)
