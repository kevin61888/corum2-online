#pragma once

#include	"Menu.h"

#define	MAX_ITEM_UPGRADE	4
#define	ITEM_UPGRADE_SUCCESS_TYPE_AD	1	// 공격력 성공
#define	ITEM_UPGRADE_SUCCESS_TYPE_DP	2	// 방어력 성공
#define	ITEM_UPGRADE_SUCCESS_TYPE_OP	4	// 옵션 성공
#define	ITEM_UPGRADE_SUCCESS_TYPE_SL	8	// 슬롯증가 성공
#define	ITEM_UPGRADE_WAITING			127	// 업그레이드 결과 기다리는중
#define	ITEM_UPGRADE_FAIL				128	// 실패
#define ITEM_UPGRADE_FAIL_INSURANCE		129	// 실패 하지만 아이템 보험 처리 

#define SPR_OBJ_ITEMUPGRADEWND			0
#define SPR_OBJ_ITEMUPGRADE_HELPWND		1
#define BUTTON_OBJ_ITEMUPGRADE_CLOSE1	2
#define BUTTON_OBJ_ITEMUPGRADE_CLOSE2	3
#define BUTTON_OBJ_ITEMUPGRADE1			4
#define BUTTON_OBJ_ITEMUPGRADE2			5
#define BUTTON_OBJ_ITEMUPGRADE3			6


class CItemUpgradeWnd : public CMenu
{	
public:
		
	CItem	m_pUpgradeItem[MAX_ITEM_UPGRADE];

	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;
			
	BOOL	m_bWaitingUpgrade;	// 업글을 기다리는중인가?
	char	m_szUpgradeDesc[16][TEXT_RENDER_BUFFER];
	
	BOOL	m_bBtn;
	
public:
	
private:
	static CItemUpgradeWnd* c_pThis;

public:
	static CItemUpgradeWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CItemUpgradeWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
		
	void	RenderText();	
	void	Remove();
	void	SetOrder();	
	void	SetActive(BOOL bActive = TRUE);
	
	int		CheckInterface();

	// 체크 관련 함수 //	
	void	RenderItem();

	void	ItemInfoRender();	
	
public:

	BOOL	IsAbleUpgradeAriesZodiac(CItem* pItem);

	CItemUpgradeWnd();
	~CItemUpgradeWnd();

private:
	

	void	OnMove();
	void	SetButtonStatus(DWORD dwSprID, DWORD dwStatus);// dwStatus = 0(nomal) : 1(mouseover) : 2(mousedown)	
};