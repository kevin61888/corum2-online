//======================================================//
// Code by Jang.							2002.12.23	//
// Obejct : Char Window.								//
//======================================================//
#ifndef		__GAMEMENU_WINDOW_INC__
#define		__GAMEMENU_WINDOW_INC__

#include	"Menu.h"

#define SPR_OBJ_OPTIONWND				0
#define	SPR_OBJ_OPTION_SHADOW			1
#define SPR_OBJ_OPTION_BGM				2
#define SPR_OBJ_OPTION_TEXT				3
#define SPR_OBJ_OPTION_EFFECT			4
#define SPR_OBJ_OPTION_EFCHK			5
#define SPR_OBJ_SYSTEM_CLOSE1			6
#define SPR_OBJ_SYSTEM_CLOSE2			7
#define BUTTON_OBJ_EXIT1				8
#define BUTTON_OBJ_EXIT2				9
#define BUTTON_OBJ_EXIT3				10
#define SPR_OBJ_OPTION_BGMBOL			11
#define SPR_OBJ_OPTION_EFFECTBOL		12
#define SPR_OBJ_OPTION_SYSTEMMSG		13
#define SPR_OBJ_OPTION_TEXTSPEED		14
#define SPR_OBJ_OPTION_TEXTLINE			15
#define SPR_OBJ_OPTION_NOTICE			16
#define SPR_OBJ_OPTION_EMOTICON			17
#define SPR_OBJ_OPTION_WHISPER			18
#define SPR_OBJ_OPTION_GUILD			19
#define SPR_OBJ_OPTION_PARTY			20
#define SPR_OBJ_OPTION_FRIEND			21

class CGameMenuWnd : public CMenu
{
public:

	BOOL	m_bMoveChk;	

	BYTE	m_byEffectIndex;			// Effect Index.

	BOOL	m_bBtn;		
	BOOL	m_bExitDown;		

	BOOL	m_bTextFlag;				// 옵션 플레그 관련.
	BOOL	m_bBGMFlag;					// 옵션 플레그 관련.	
	BOOL	m_bShadowFlag;				// 옵션 플레그 관련.
	BOOL	m_bEffectFlag;				// 옵션 플레그 관련.
	BOOL	m_bBGMRepeat;				// 옵션 플레그 관련.
	BOOL	m_bSystemMsgFlag;			// 옵션 플레그 관련.
	// sung-han 2005-03-30------------------------------------------------------------------------------------
	BYTE	m_byChatRemainTime;			// 옵션 플레그, 노멀채팅 표시 사라지는 시간(1:5초, 2:10초, 3:15초) 
	BOOL	m_bChatKindParty;			// 옵션 플레그, 노멀채팅창에 파티채팅 표시여부
	BOOL	m_bChatKindGuild;			// 옵션 플레그, 노멀채팅창에 길드채팅 표시여부
	BOOL	m_bChatKindWhisper;			// 옵션 플레그, 노멀채팅창에 귓말채팅 표시여부
	BOOL	m_bChatKindFriend;			// 옵션 플레그, 노멀채팅창에 친구채팅 표시여부
	BOOL	m_bChatEmoticon;			// 옵션 플레그, 채팅시에 자동 이모티콘의 반응 여부
	BYTE	m_byKindNotich;				// 옵션 플레그, 공지 메세지 표시방법
	// --------------------------------------------------------------------------------------------------------
	DWORD	m_dwBGMId;					// BG 아이디.
	char	m_szLast_BGM_Name[255];		// 최근(현재 실행한) 음악 BackGround Name // add seven 040408

public:

	// Singleton Patten //
private:
	static CGameMenuWnd* c_pThis;

public:
	static CGameMenuWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGameMenuWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);
	void	OpenWnd();
	
	int		CheckInterface();

	// 체크 관련 함수 //		
	void	SetEffect();

	CGameMenuWnd();
	~CGameMenuWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//