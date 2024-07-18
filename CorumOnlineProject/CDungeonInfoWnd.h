#pragma once

#include	"Menu.h"

#define SPR_OBJ_DUNGEONINFOEX_WND1					0
#define SPR_OBJ_DUNGEONINFOEX_WND2					1
#define SPR_OBJ_DUNGEONINFOEX_WND3					2
#define SPR_OBJ_DUNGEONINFOEX_WND4					3
#define SPR_OBJ_DUNGEONINFO_CLOSE1					4
#define SPR_OBJ_DUNGEONINFO_CLOSE2					5
#define BUTTON_OBJ_DUNGEONINFOEX_STATUS1			6
#define BUTTON_OBJ_DUNGEONINFOEX_STATUS2			7
#define BUTTON_OBJ_DUNGEONINFOEX_STATUS3			8
#define BUTTON_OBJ_DUNGEONINFOEX_INFORMATION1		9
#define BUTTON_OBJ_DUNGEONINFOEX_INFORMATION2		10	
#define BUTTON_OBJ_DUNGEONINFOEX_INFORMATION3		11
#define BUTTON_OBJ_DUNGEONINFOEX_PRODUCTION1		12
#define BUTTON_OBJ_DUNGEONINFOEX_PRODUCTION2		13
#define BUTTON_OBJ_DUNGEONINFOEX_PRODUCTION3		14
#define BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT1		15
#define BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT2		16
#define BUTTON_OBJ_DUNGEONINFOEX_MANAGEMENT3		17
#define BUTTON_OBJ_DUNGEONINFOEX_PAGEPRE1			18
#define BUTTON_OBJ_DUNGEONINFOEX_PAGEPRE2			19
#define BUTTON_OBJ_DUNGEONINFOEX_PAGEPRE3			20
#define BUTTON_OBJ_DUNGEONINFOEX_PAGENEXT1			21
#define BUTTON_OBJ_DUNGEONINFOEX_PAGENEXT2			22
#define BUTTON_OBJ_DUNGEONINFOEX_PAGENEXT3			23
#define SPR_OBJ_DUNGEONINFOEX_SELECTBAR				24
#define SPR_OBJ_DUNGEONINFOEX_PAGEBOX				25

class CVoidList;
class DUNGEON_DATA_EX;

class CDungeonInfoWnd : public CMenu  
{
public:
		
	BOOL				m_bScrollClick;					// 스크롤 버튼 아이디값이 저장.
	BYTE				m_bTabIndex;		

	char				m_szInfo[TEXT_RENDER_BUFFER];
	RECT				m_rtPos;

	DWORD				m_dwOwnDungeonCount;			// 자기 던전 개수.

	CVoidList*			m_pPageList;					// 각 페이지의 머리를 기억한 리스트
	void*				m_pDisplayHeadPosition;
	DUNGEON_DATA_EX*	m_pSelectDungeon;
	
	BYTE				m_bCurPage;
	BOOL				m_bPointer;

public:
		
private:
	static CDungeonInfoWnd* c_pThis;

public:
	static CDungeonInfoWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CDungeonInfoWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	void	SelectDungeon(int nIndex);	
	void	InsertDungeonInfo(DUNGEON_DATA_EX* pDungeon);

	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// 체크 관련 함수 //	
	void	ResetResource();	

public:
	
	CDungeonInfoWnd();
	~CDungeonInfoWnd();

private:
	void	ShowManageButton(BOOL bShow);
	void	SetButtonStatus(DWORD dwSprID, DWORD dwStatus);
	void	OnMove();
	void	SetTab(BYTE bTabIndex);
	void	NextPage();
	void	PrevPage();
};
