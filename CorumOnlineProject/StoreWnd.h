//======================================================//
// Code by Jang.							2003.03.10	//
// Obejct : Store Window.								//
//======================================================//
#ifndef		__STORE_WINDOW_INC__
#define		__STORE_WINDOW_INC__

#include "Menu.h"

#define SPR_OBJ_STORE_WND1			0
#define SPR_OBJ_STORE_WND2			1
#define SPR_OBJ_STORE_WND3			2
#define SPR_OBJ_STORE_CLOSE1		3
#define SPR_OBJ_STORE_CLOSE2		4
#define SPR_OBJ_STOREWND_PAGE		5
#define SPR_OBJ_STOREWND_PREV1		6
#define SPR_OBJ_STOREWND_PREV2		7
#define SPR_OBJ_STOREWND_PREV3		8
#define SPR_OBJ_STOREWND_NEXT1		9
#define SPR_OBJ_STOREWND_NEXT2		10
#define SPR_OBJ_STOREWND_NEXT3		11


#define MAX_ONEPAGE_SMALL (7*9)
#define MAX_ONEPAGE_LARGE (7*4)


class CStoreWnd : public CMenu
{
public:
		
	BYTE	m_byServer;
	BYTE	m_byChk;
	BYTE	m_byRenderIndex;
	
	WORD	m_wSuppliesCount;
	WORD	m_wWeaponCount;
	WORD	m_wArmorCount;
	WORD	m_wSuppliesId;
	WORD	m_wItemId[500];

	BOOL	m_bRender;					
	BOOL	m_bSupplies;		
	BOOL	m_bBtnChk[2];
	
	char	m_szInfo[TEXT_RENDER_BUFFER];					// Text 출력 버퍼.
	RECT	m_rtPos;										// Text 출력 좌표. 

	BYTE	m_byStoreType;									// 상점 타입.
	
	DWORD	m_dwMapId;
	DWORD	m_dwMonsterIndex;

private:		
	
	BYTE	m_byCurPage;									// 현재 페이지수.
	BYTE	m_byMaxPage;									// 최대 페이지수.
	BYTE	m_byPageItemCount;								// 한 페이지에 아이템수.
			
	SSTORE_INFO	m_sStoreInfo[STORE_MAX_ITEM_COUNT];

public:
	
	// Singleton Patten //
private:
	static CStoreWnd* c_pThis;

public:
	static CStoreWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CStoreWnd; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();	

	void	RenderText();	
	void	Remove();		
	void	SetOrder();	
	void	RenderLine();
	void	SetActive(BOOL bActive = TRUE);	
	
	int		CheckInterface();		
	
	// 체크 관련 함수 //
	void	RenderItem();
	
	void	OpenStoreItem(BYTE byType);
	void	SetPrevNext(BYTE bPNType);	

	BOOL	SetItemInfo(BYTE byType, WORD wItemId, BYTE byIndex);	
	
	CStoreWnd();
	virtual ~CStoreWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//