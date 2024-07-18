//==================================================//
// Code by Jang.									//
// Object : Interface.								//
//==================================================//
#pragma once

#include "Define.h"
#include "Struct.h"
#include "InterfaceSpr.h"
#include "InterfaceSprDef.h"


#define MAX_SPRITE_DATA		250
#define MAX_CHECK_INTERFACE	150


#define MOUSE_TYPE_NONE		0
#define MOUSE_TYPE_DEFAULT	1
#define MOUSE_TYPE_BTN		2
#define MOUSE_TYPE_ITEM		3
#define MOUSE_TYPE_ATTACK	4
#define MOUSE_TYPE_NPC		5
#define MOUSE_TYPE_GUARDIAN	6


enum CHECKTYPE
{
	CHECK_NONE		= 0,	
	CHECK_CLOSE		= 1,
	CHECK_BTN		= 2,
	CHECK_PUSHBTN	= 3,
	CHECK_SCROLL	= 4,
	CHECK_MOVEWND	= 5,
	CHECK_ITEM		= 6,
	CHECK_ATTACK	= 7,
	CHECK_NPC		= 8,
	CHECK_GUARDIAN	= 9
};

enum MOUSECHECK
{
	MOUSE_NONE		= 0,
	MOUSE_OVER		= 1,
	MOUSE_DOWN		= 2,
	MOUSE_UP		= 3,
};


typedef struct sSprite_Render_Data
{
	DWORD	dwId;
	DWORD	dwSprId;
	DWORD	dwAlpha;

	BOOL	bPos;
	BOOL	bOrder;
	BOOL	bRender;

	float	fPosX;
	float	fPosY;
	float	fRot;

	int		nOrder;
					
	VECTOR2	v2Scaling;


}SSPR_RENDER_DATA, *LP_SPR_RENDER_DATA;

class CMenu
{
public:
	 
	float	m_fPosX;					// 게임 메뉴 윈도우 좌표.
	float	m_fPosZ;					// 게임 메뉴 윈도우 좌표.
	float	m_fPrevPosX;				// 이전 게임 메뉴 윈도우 좌표.
	float	m_fPrevPosZ;				// 이전 게임 메뉴 윈도우 좌표.
	float	m_fMaxSizeWidth;			// 게임 메뉴의 윈도우 사이즈.
	float	m_fMaxSizeHeight;			// 게임 메뉴의 윈도우 사이즈.
	
	BOOL	m_bScCheckInterface;
	
	BOOL	m_bPointer;
	BOOL	m_bSoundChk;
	BOOL	m_bActive;
	BOOL	m_bClick;
	BOOL	m_bUp;
	BOOL	m_bDown;
	BOOL	m_bUpChk;
	BOOL	m_bClose;
	
	BOOL	m_bScChk[2];
	
	BOOL	m_bMouseIcon;
	BOOL	m_bDownChk;
	BOOL	m_bInit;
	BOOL	m_bFirst;
	BOOL	m_bScroll;
	BOOL	m_bMoveChk;
	BYTE	m_byScrollChk;	
	
	BYTE	m_byType;
	BYTE	m_byOrder;		
	BYTE	m_bySpriteDataCnt;
	BYTE	m_byChkInterfaceCnt;
	
	float	m_fSCTemp;
		
	BOOL	m_bValueScrollChk;

	int		m_nCurScrollPos;
	int		m_nScrollPos;
	float	m_fScrollPos;		

	char	m_szMenu[0xff];

	BYTE	m_byMaxScroll;
	float	m_fStartSize;
	float	m_fEndSize;
	float	m_fScrollSize;

	SSPR_RENDER_DATA	m_sSprite_Data[MAX_SPRITE_DATA];
	RECT				m_rt[MAX_CHECK_INTERFACE];	
	CHECKTYPE			m_rtType[MAX_CHECK_INTERFACE];
	BYTE				m_byValue[MAX_CHECK_INTERFACE];
	
	int		m_nIndex;

	int	m_nOrder[MAX_CHECK_INTERFACE];

public:

	void	InitSpriteData();
	void	InitCheckInterface();
	void	RemoveAllData();	
	void	RemoveData(DWORD dwSprId);	
	void	ShowSpriteAll();
	
	BOOL	IsReturn(int nRt, MOUSECHECK eMouseCheck);

	void	SetCheckInterfacePos(int nStartX, int nStartY, int nEndX, int nEndY, BYTE byReturn);

	BOOL	InsertCheckInterface(int nStartX, int nStartY, int nEndX, int nEndY, BYTE byReturn, CHECKTYPE sCheckType = CHECK_NONE, BYTE byValue = 0, BYTE byOrder = 0);

	BOOL	InsertData(	DWORD	dwId,
						DWORD	dwSprId,
						float	fPosX,
						float	fPosY,
						float	fScalingX,
						float	fScalingY,
						int		nOrder,
						BOOL	bRender		= FALSE,
						BOOL	bPos		= TRUE,
						BOOL	bOrderChk	= TRUE,
						DWORD	dwAlpha		= 0xffffffff,
						float	fRot		= 0.0f				);
	BOOL	SetPosObj(DWORD dwId, float fPosX, float fPosY);	

	BOOL	SetPosObjX(DWORD dwId, int nPosX);
	BOOL	SetPosObjY(DWORD dwId, int nPosX);
	BOOL	SetPosObjX(DWORD dwId, float fPosX);
	BOOL	SetPosObjY(DWORD dwId, float fPosY);
	BOOL	SetRotObj(DWORD dwId, float fRot);
	
	BOOL	SetScalingObjX(DWORD dwId, float fScalingX);
	BOOL	SetScalingObjY(DWORD dwId, float fScalingY);
	BOOL	SetScalingObj(DWORD dwId, float fScalingX, float fScalingY);

	BOOL	SetMoveWnd(int nRt, int nValue = 0);

	void	InitValue(int nRt);
	BOOL	MouseUp();
	BOOL	MouseChk(int nRt);

	BOOL	GetRender(DWORD dwId);

	BOOL	GetCheckScroll();

	void	SetScroll(BYTE byMaxScroll, float fStartSize, float fEndSize);

	void	SetRender(DWORD dwId, BOOL bRender);
	void	SetMouseCheck(int nRt, MOUSECHECK eMouseCheck, BYTE byChk = 0, BOOL bEx = FALSE);
	
	MOUSECHECK	GetMouseCheck(int nRt);

public:
	
	BYTE	GetChk(int nValue = 0, BOOL bPos = FALSE, BYTE byOrder = 0);
	
	void	SetPos(float fPosX, float fPosZ);			// 게임 메뉴 좌표 설정.
	void	SetMaxSize(float fWidth, float fHeight);	// 게임 메뉴 좌표 설정.	
	void	SetOrderIndex(BYTE byOrder);
	void	SetClick(BOOL bClick);
	void	SetUp(BOOL bUp);
	void	GetMouseIcon(BOOL bMouseIcon);		
	virtual	void	Render();

	BOOL	GetActive();
	BOOL	GetClick();		
	BOOL	GetMouseIcon();
		
	WORD	GetStartOrder();

	int		GetOrderIndex();
	int		GetPosX();									// 게임 메뉴 좌표 설정.
	int		GetPosY();									// 게임 메뉴 좌표 설정.		

	void	SetPosX(float fPosX);
	void	SetPosY(float fPosY);

	LP_SPR_RENDER_DATA	GetData(DWORD dwId);

	virtual	void	CreateMenu(
		char*	pszName, 
		float	fWidth, 
		float	fHeight, 
		BYTE	byType, 
		int		nIndex, 
		BOOL	bActive = FALSE, 
		BOOL	bScroll = FALSE
		);
	
	BOOL	MouseScrollChk(int nRt, int nScrollNum);
	void	InterfaceTextRender(WORD wID, DWORD dwColor = 0xffffffff);

public:
		
	// 공통으로 사용되는 것들은 재정의 한다.	
	virtual BOOL	Init()							= 0;
	virtual void	RenderText()					= 0;
	virtual void	SetActive(BOOL bActive = TRUE)	= 0;
	virtual void	Remove()						= 0;
	virtual void	SetOrder()						= 0;
	virtual int		CheckInterface()				= 0;	
	
	CMenu();
	~CMenu();
//private:
//	void	InitSpriteData();
//	void	InitCheckInterface();

};
//==================================================//
// End.												//
//==================================================//