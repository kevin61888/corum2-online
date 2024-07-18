//
//	Event NPC sub Window
//
//				2003/11/13
//				Trust Pak


#ifndef __EVENT_NPC_SUB_WINDOW__
#define	__EVENT_NPC_SUB_WINDOW__


#define MAX_MOVE_DUNGEON_INFO	32

#define EVENTNPC_SUB_TYPE_OK_SELECT			0 
#define EVENTNPC_SUB_TYPE_OK_CANNOT_SELECT	1
#define EVENTNPC_SUB_TYPE_CANCEL			2
#define	EVENTNPC_LEVEL_HIGH					3
#define EVENTNPC_LEVEL_LOW					4

#define SPR_EVENT_NPC_SUB_WND1			0
#define SPR_EVENT_NPC_SUB_WND2			1
#define SPR_EVENT_NPC_SUB_WND3			2
#define SPR_EVENT_NPC_SUB_CLOSE1		3
#define SPR_EVENT_NPC_SUB_CLOSE2		4



//------------------------------------------------------------------------====
struct MOVE_DUNGEON_INFO
{
	DWORD		dwID;
	char		m_szDungeonName[MAX_DUNGEON_NAME_LENGTH];
	int			iLevelMin;
	int			iLevelMax;
};


//------------------------------------------------------------------------====
class CEventNPCSubWnd : public CMenu
{
public:
	float				m_fWndWidth;
	float				m_fWndHeight;

	char				m_szRenderText[16][256];	
	MOVE_DUNGEON_INFO	m_aMoveDungeonInfo[MAX_MOVE_DUNGEON_INFO];

	int					m_iMoveDungeonInfoNum;

	int					m_iSelectedIndex;	

	int					m_iTextLineNumber;
	int					m_iMessageType;		// OK_SELECT, OK_CANNOT_SELECT, CANCEL


	//MOVE_DUNGEON_INFO m_MoveDungeonInfo[]


public:
	
	// Singleton Patten //
private:
	static CEventNPCSubWnd* c_pThis;

public:
	static CEventNPCSubWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CEventNPCSubWnd; return c_pThis; }
	static void				DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	BOOL	Init();		

	void	RenderText();
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	
	void	SetPosition(float fX, float fZ);	
	int		CheckInterface();	

	void	SetText(int iType);		// OK_SELECT, OK_CONNOT_SELECT, CANCEL

	void	QueryCanMoveDungeon(void);

	void	CheckTextSelectedIndex(void);
	void	PortalProcess(void);

	CEventNPCSubWnd();
	~CEventNPCSubWnd();
};

//----------------------------------------------------------------------------


#endif