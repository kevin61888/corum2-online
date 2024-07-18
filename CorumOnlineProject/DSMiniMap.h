//======================================================//
// Code by Jang.							2002.12.30	//
// Obejct : Mini Map Window.							//
//======================================================//
#ifndef		__MINI_MAP_WINDOW_INC__
#define		__MINI_MAP_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_PARTY_RED			0
#define SPR_OBJ_PARTY_BLUE			1
#define SPR_OBJ_PARTY_WHITE			2
#define SPR_OBJ_PARTY_GREEN			3
#define SPR_OBJ_PARTY_BORA			4
#define SPR_OBJ_PARTY_YELLOW		5
#define SPR_OBJ_PARTY_ORANGE		6
#define SPR_OBJ_PARTY_PINK			7
#define SPR_OBJ_MINIMAP				8
#define SPR_OBJ_DSMAP_USER			9
#define SPR_OBJ_MAP					10

class CMiniMapWnd : public CMenu
{
public:

	DWORD	m_dwLayerId;

	BOOL	m_bTemp;

	WORD	m_wMapId;
	BYTE	m_byMapLayer;
	RECT	m_rtRect;
	char	m_szInfo[TEXT_RENDER_BUFFER];
	
	float	m_fRot;
	
	BYTE	m_bPartyPtr[MAX_PARTY];
	
	
public:

	// Singleton Patten //
private:
	static CMiniMapWnd* c_pThis;

public:
	static CMiniMapWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CMiniMapWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();
	
	void	RenderText();	
	void	Remove();	
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);		
	int		CheckInterface();

	// 체크 관련 함수 //	
	BOOL	CreateMap();
	BOOL	ReleaseMap();

	void	SetMinimapPos();
	VECTOR2 GetRatioMinimap(float x, float z);
	
	void	SetRot(BOOL bInc);
	float	GetRot(BOOL bChk = FALSE);	

	BYTE	GetPartyPtr();
	DWORD	GetPartyResource(BYTE byIndex);
	void	SetPartyPtr(BYTE byIndex, BYTE byValue);
	void	SetPartyPtr(DWORD dwResource);
	BOOL	GetPartyPtr(DWORD dwResource);
	BYTE	GetPartyPtrRt(DWORD dwResource);

	CMiniMapWnd();
	~CMiniMapWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//