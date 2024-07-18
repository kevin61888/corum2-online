#pragma once

#include	"Menu.h"

#define SPR_OBJ_DUNGEONMANAGER_WND							0
#define SPR_OBJ_DUNGEONMANAGER_CLOSE1						1
#define SPR_OBJ_DUNGEONMANAGER_CLOSE2						2
#define BUTTON_OBJ_ACQUISITION1_1							9
#define BUTTON_OBJ_ACQUISITION1_2							10
#define BUTTON_OBJ_ACQUISITION1_3							11
#define BUTTON_OBJ_ACQUISITION2_1							12
#define BUTTON_OBJ_ACQUISITION2_2							13
#define BUTTON_OBJ_ACQUISITION2_3							14
#define BUTTON_OBJ_ACQUISITION3_1							15
#define BUTTON_OBJ_ACQUISITION3_2							16
#define BUTTON_OBJ_ACQUISITION3_3							17
#define SPR_OBJ_DUNGEONMANAGER_MOF1							18
#define SPR_OBJ_DUNGEONMANAGER_MOF2							19
#define SPR_OBJ_DUNGEONMANAGER_MOF3							20

class CItem;
class DUNGEON_DATA_EX;

class CDungeonManagerWnd : public CMenu
{
public:

	BOOL				m_bBtnChk;

	DUNGEON_DATA_EX*	m_pDungeon;
		
	BOOL	m_bDownChk;
			
	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;
	int		m_nTabIndex;		
	
public:
	
private:
	static CDungeonManagerWnd* c_pThis;

public:
	static CDungeonManagerWnd*	GetInstance()	{ if(!c_pThis) c_pThis = new CDungeonManagerWnd; return c_pThis; }
	static void			DestroyInstance()		{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// 체크 관련 함수 //	
	
public:	
	
	CItem*	GetMagicArrayItem();
	CItem*	GetGuardianItem();

	void	RenderItem();
	void	SetDungeon(DUNGEON_DATA_EX* pDungeon);
	void	SetItem(CItem* pItem, BOOL bView);
	void	OnChangeTab(int nIndex);
	void	SetDungeonOperationType(BYTE bOperationType);

	CDungeonManagerWnd();
	~CDungeonManagerWnd();

private:
	
	void	SetButtonStatus(DWORD dwSprID, DWORD dwStatus);// dwStatus = 0(nomal) : 1(mouseover) : 2(mousedown)
};
