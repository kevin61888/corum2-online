 //======================================================//
// Code by Jang.							2002.1.8	//
// Obejct : Chat Window.								//
//======================================================//
#ifndef		__CHAT_WINDOW_INC__
#define		__CHAT_WINDOW_INC__

#include	"Menu.h"

class CScript;
class CComboBox;

#define	SPR_OBJ_CHAT_WND				0
#define	SPR_OBJ_CHAT_CHAT1				1
#define	SPR_OBJ_CHAT_CHAT2				2
#define	SPR_OBJ_CHAT_MESSAGE1			3
#define	SPR_OBJ_CHAT_MESSAGE2			4
#define	SPR_OBJ_CHAT_ALL1				5
#define	SPR_OBJ_CHAT_ALL2				6
#define	SPR_OBJ_CHAT_ALL3				7
#define	SPR_OBJ_CHAT_PARTY1				8
#define	SPR_OBJ_CHAT_PARTY2				9
#define	SPR_OBJ_CHAT_PARTY3				10
#define	SPR_OBJ_CHAT_GUILD1				11
#define	SPR_OBJ_CHAT_GUILD2				12
#define	SPR_OBJ_CHAT_GUILD3				13
#define	SPR_OBJ_CHAT_WHISPER1			14
#define	SPR_OBJ_CHAT_WHISPER2			15
#define	SPR_OBJ_CHAT_WHISPER3			16
#define	SPR_OBJ_CHAT_FRIEND1			17
#define	SPR_OBJ_CHAT_FRIEND2			18
#define	SPR_OBJ_CHAT_FRIEND3			19
#define	SPR_OBJ_CHAT_LIST1				20
#define	SPR_OBJ_CHAT_LIST2				21
#define	SPR_OBJ_CHAT_LIST3				22
#define	SPR_OBJ_CHAT_CLOSE1				23
#define	SPR_OBJ_CHAT_CLOSE2				24

class CChatWnd : public CMenu
{
private:
	
public:	
		
	//CComboBox*			m_pComboBox;
	
	BYTE	m_byChatChk;
	BYTE	m_byChatMode;
	BYTE	m_byChatType;
	BYTE	m_byRenderMode;
	
	BOOL	m_bChatList;	
	BOOL	m_bChkBtn[3];

	RECT	m_rtPos;
	char	m_szInfo[0xff];

	char	m_szClip[0xff];

	BYTE	m_byClipType;
	

	char	m_szName[MAX_CHARACTER_NAME_LENGTH];
	
public:

	// Singleton Patten //
private:
	static CChatWnd* c_pThis;

public:
	static CChatWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CChatWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();	
	void	Remove();	
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);	
	int		CheckInterface();

	// 체크 관련 함수 //	
	void	ChatDefault(char* pszText);
	void	ChatParty(char* pszText);
	void	ChatGuild(char* pszText);
	void	ChatFriend(char* pszText);
	void	ChatWhisper(char* pszText, CScript* pScript);
	void	SetName(char* pszName, BOOL bChk = FALSE);

	//void	AddToComboBox(char* szID); // BY deepdark.

	//int		GetComboBoxChk();

	CChatWnd();
	~CChatWnd();
};

#endif
//======================================================//
// End.													//
//======================================================//