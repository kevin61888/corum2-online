//==========================================================//
// Code by Jang.								2002.12.17	//
// Object : User Interface.									//
//==========================================================//
#ifndef		__USER_INTERFACE__
#define		__USER_INTERFACE__

#include	"Menu.h"
#include	"GlobalSoundHandles.h"
#include "GuildWarStatusWnd.h"

class CMonster;
class CUser;

#define SPR_OBJ_UI_LEFT							0
#define SPR_OBJ_UI_RIGHT						1
#define SPR_OBJ_EN2								2
#define SPR_OBJ_EN1								3
#define SPR_OBJ_EXP								4
#define SPR_OBJ_MANA2							5
#define SPR_OBJ_MANA1							6
#define SPR_OBJ_CAST							7
#define BUTTON_OBJ_ITEM1						8
#define BUTTON_OBJ_CHR1							9
#define BUTTON_OBJ_SKILL1						10
#define BUTTON_OBJ_GUILD1						11
#define BUTTON_OBJ_PARTY1						12
#define BUTTON_OBJ_CHAT1						13
#define BUTTON_OBJ_ITEM2						14
#define BUTTON_OBJ_CHR2							15
#define BUTTON_OBJ_SKILL2						16
#define BUTTON_OBJ_GUILD2						17
#define BUTTON_OBJ_PARTY2						18
#define BUTTON_OBJ_CHAT2						19
#define BUTTON_OBJ_SYSTEM1						20
#define BUTTON_OBJ_SYSTEM2						21
#define BUTTON_OBJ_PKON1						22
#define BUTTON_OBJ_PKON2						23
#define BUTTON_OBJ_PKOFF1						24
#define BUTTON_OBJ_PKOFF2						25
#define SPR_OBJ_MONSTERHP_BAR1					26
#define SPR_OBJ_MONSTERHP_BAR2					27
#define SPR_OBJ_MONSTER_GAGE1					28
#define SPR_OBJ_MONSTER_GAGE2					29
#define SPR_OBJ_USERHP_BAR1						30
#define SPR_OBJ_USERHP_BAR2						31
#define SPR_OBJ_USER_GAGE1						32
#define SPR_OBJ_USER_GAGE2						33
#define SPR_OBJ_GUARDIAN_WND1					34
#define SPR_OBJ_GUARDIAN_EN						35
#define SPR_OBJ_UI_TRADE1						36
#define SPR_OBJ_UI_TRADE2						37
#define SPR_OBJ_UI_TRADE3						38
#define SPR_OBJ_UI_SHOP1						39
#define SPR_OBJ_UI_SHOP2						40
#define SPR_OBJ_UI_SHOP3						41
#define SPR_OBJ_UI_REST1						42
#define SPR_OBJ_UI_REST2						43
#define SPR_OBJ_UI_REST3						44
#define SPR_OBJ_UI_MATCH1						45
#define SPR_OBJ_UI_MATCH2						46
#define SPR_OBJ_UI_MATCH3						47
#define SPR_OBJ_UI_QUICKSLOT_INABLE1			48
#define SPR_OBJ_UI_QUICKSLOT_INABLE2			49
#define SPR_OBJ_UI_QUICKSLOT_INABLE3			50
#define SPR_OBJ_UI_QUICKSLOT_INABLE4			51
#define SPR_OBJ_UI_QUICKSLOT_INABLE5			52
#define SPR_OBJ_UI_QUICKSLOT_INABLE6			53
#define SPR_OBJ_UI_QUICKSLOT_INABLE7			54
#define SPR_OBJ_UI_QUICKSLOT_INABLE8			55
#define SPR_OBJ_UI_GUARDIAN_STATUS				56

class CUserInterface : public CMenu
{
public:
	
	OBJECT_HANDLE_DESC		m_pUserClickHandle;
	
	OBJECT_HANDLE_DESC		m_pUserMouseHandle[3];

	BOOL	m_bCommand;
	BOOL	m_bParty;

	BOOL	m_bGuardian;

	BYTE	m_bSkillRender;
		
	BOOL	m_bPointerChk;	
	BOOL	m_bSmall;

	BOOL	m_bMonster;

	char	m_szMosterName[50];

	char	m_szGuardianName[50];
	char	m_szMatchUserName[50];

	float	m_fMonMaxHp;
	float	m_fMonHp;

	float	m_fUserMaxHp;
	float	m_fUserHp;

	DWORD	m_dwMonCurTime;
	DWORD	m_dwMonPrevTime;
	DWORD	m_dwUserCurTime;
	DWORD	m_dwUserPrevTime;
	
	float	m_fCIPosX;
	float	m_fCIPosY;

	BYTE	m_byOrderCheck;

	// UI 출력 관련 변수 //	
	float	m_fEnIndex[2];
	float	m_fEnSpeed[2];	
	float	m_fEnIncDec;
	float	m_fEnDecSpeed;
	float	m_fEnIncSpeed;

	float	m_fManaIndex[2];
	float	m_fManaSpeed[2];
	float	m_fManaIncDec;
	float	m_fManaDecSpeed;
	float	m_fManaIncSpeed;

	float	m_fExpIndex;
	float	m_fExpSpeed;	
				
	BOOL	m_bEnDec;
	BOOL	m_bEnInc;	
	BOOL	m_bEn[2];
	BOOL	m_bEnDef;

	BOOL	m_bManaDec;
	BOOL	m_bManaInc;	
	BOOL	m_bMana[2];
	BOOL	m_bManaDef;

	BOOL	m_bExp;	
	BOOL	m_bExpIncDef;

	BOOL	m_bMatchUserHPShowFlag;			// pvp 중인지 나타낸다
	BOOL	m_bMatchUserHPShowEnable;		// pvp 중에 상대유저 HP바를 나타내는 플래그이다.

	int		m_nChatChk;
	int		m_nPK;		
		
	char	m_szCharacterName[MAX_CHARACTER_NAME_LENGTH];

	BYTE	m_byPointerType;
	
	int		m_nMousePos[__MAX_MOUSE_POINTER__*2];
	
	BOOL	m_bUserInterface;

	DWORD	m_nPrevTick;
	DWORD	m_nCurrentTick;
	BOOL	m_bTimeChk;
	
	WORD	m_wMaxSkillLevel;				// 최대 스킬 레벨 //
	WORD	m_wMaxMasteryLevel;				// 최대 마스터리 스킬 레벨 //

	BOOL	m_bMoveChr;
		
	BYTE	m_byMoveType;

	BOOL	m_bUIChk[4];

	BOOL	m_bTrade;
	BOOL	m_bMatch;		// 대결 요청자가 Match 버튼을 클릭한 상태 유무
	BOOL	m_bRGW;			// 라이벌 길드전 버튼 클릭 유무
	
	WORD	m_dwMagicArray;

	IDISpriteObject*		m_pSprPlayerShop[2];

	CMonster*				m_pMonster;
	CUser *					m_pUser;
	CUser *					m_pMatchUser;
	unsigned __int64		m_i64ActiveGuardianItemID;
public:
	virtual	void	CreateMenu(
		char*	pszName, 
		float	fWidth, 
		float	fHeight, 
		BYTE	byType, 
		int		nIndex, 
		BOOL	bActive = FALSE, 
		BOOL	bScroll = FALSE
		);

	// Singleton Patten //
private:
	static CUserInterface* c_pThis;

public:
	static CUserInterface*	GetInstance()		{ if(!c_pThis) c_pThis = new CUserInterface; return c_pThis; }
	static void		DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통 함수 //
	BOOL	Init();	

	void	InsertUIData();
	void	SetRenderDefaultUI();

	void	Render();
	void	RenderText();	
	void	Remove();	
	void	SetOrder();		
	void	SetActive(BOOL bActive = FALSE);		
	int		CheckInterface();
	
	int		GetChk();
	int		GetChkOrderIndex();

	void	RenderGuardianItem();

	// 던젼에 들어가면 보여지는 UI 함수 // 

	void	DengeonEnStart();
	void	DengeonExpStart();
	void	DengeonManaStart();	
	
	// Hp 관련 함수 //

	void	DengeonHpDec();
	void	DengeonHpInc();
	void	DengeonHpDef();		

	void	SetDengeonHpDec();
	void	SetDengeonHpInc();
	void	SetDengeonHpDef();
	
	// Exp 관련 함수 //

	void	DengeonExpDefInc();	
	void	SetDengeonExpDefInc();	

	// Mana 관련 함수 //

	void	DengeonManaDec();
	void	DengeonManaInc();
	void	DengeonManaDef();		

	void	SetDengeonManaDec();
	void	SetDengeonManaInc();
	void	SetDengeonManaDef();
	
	// Casting 관련 함수 //
	void	DengeonCastingDef();	
	void	SetCool();

	// QuickSlot 관련 함수
	void	EnableQuickSlot(BOOL bEnAble);	
	
	void	SetDengeonHp(WORD wHp);
	void	SetDengeonMp(WORD wMp);

	void	SetMousePointerPos(float fPosX, float fPosZ);
	void	SetMousePointerType(BYTE byPointerType);
	
	void	SetPointerChk(BOOL bChk);
	BOOL	GetPointerChk();
		
	void	OpenMonsterBar(char* pszName, DWORD dwHp, DWORD dwMaxHp);
	void	OpenUserBar(char *pszName, DWORD dwHP, DWORD dwMaxHP);
	void	CloseUserBar();
	void	RenderMonsterBar();
	void	RenderUserBar();
	void	SetPKMode(BOOL bPK);
	void	SetPointer(BYTE byPointerType);

	BOOL	InterfaceCollision(int nIndex, int nPosX1, int nPosX2, int nPosY1, int nPosY2);
	BOOL	PosCollision(int nPosX1, int nPosX2, int nPosY1, int nPosY2);
		
	void	PartyUserName();
	void	OpenGuardianDescriptionWnd(INT64 i64ID
									, char* pszName
									, DWORD dwGauge
									, DWORD dwMaxGauge);

	void	CloseGuardianDescriptionWnd();
	void	SetGauge(DWORD dwGauge, DWORD dwMaxGauge);

	// added by minjin for refactoring
	int BeltProcForInterface();
	int GuardianProcForInterface();
	int PlayerShopProcForInterface();
	int RestProcForInterface();
	int MatchProcForInterface();
	int ChattingProcForInterface();
	int DungeonInfoProcForInterface();

	CUserInterface();
	virtual ~CUserInterface();

	VOID	ClearPVPFlag()
	{
		m_bPVP_Ready = FALSE;
		m_dwPVP_Ready_Count = 0;
	}

	VOID	IncPVPCount()
	{
		++m_dwPVP_Ready_Count;

		if( m_dwPVP_Ready_Count == 2 )
			m_bPVP_Ready = TRUE;
	}

	BOOL	IsAllUserPVPReady()
	{
		return m_bPVP_Ready;
	}
private:
	CGuildWarStatusWnd*	m_pStatusWnd;
	BOOL				m_bPVP_Ready;	// PVP시에 로딩 완료가 되었는지의 여부
	DWORD				m_dwPVP_Ready_Count;
};

#endif
//==========================================================//
// End.														//
//==========================================================//
