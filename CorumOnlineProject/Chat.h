//==============================================================//
// Code by Jang.									2003.08.01	//
// Object : 채팅 관련 작업(길드, 파티, 귓속말, 친구)			//
//==============================================================//
#ifndef		__CHAT_INC__
#define		__CHAT_INC__

#include	"struct.h"

#define		DEF_CHAT_TYPE_ALLCHAT		0
#define		DEF_CHAT_TYPE_WHISPER		1
#define		DEF_CHAT_TYPE_GUILD			2
#define		DEF_CHAT_TYPE_PARTY			3
#define		DEF_CHAT_TYPE_FRIEND		4

extern SWSCHATMSG	g_sWordChatMsg[__MAX_WORD_QUEUE__];
extern	int			g_nScrollPos	;
extern	int			g_nWordStart	;
extern	int			g_nWordEnd		;
extern	int			g_nWrIndex		;
extern	int			g_nWordIndex	;
extern	int			g_nPGSelect		;
extern	int			g_nChatDst		;
extern	int			g_nChatModeExt	;
extern	int			g_nChatId		;
extern	int			g_nMsgInx		;
extern	int			g_nCurScrollPos	;
extern SMSG			g_sDgChatListMsg[__MAX_CHAT_LIST_QUEUE__];
extern int			g_nDgChatListIndex;
extern int			g_nDgChatListStart;
extern int			g_nDgChatListEnd;
extern int			g_nDGZoom;
//==============================================================//
// Function.													//
//==============================================================//
void	InitDungeonMessage();
void	ProcessMessgae();
int		MessageAdd(char* pszMessage, DWORD dwCol, BOOL bChkSystemMsg, int nSize = 0);
int		DisplayMessageAdd(char* pszMessage, DWORD dwCol, BOOL bChk = TRUE, BYTE byType = DEF_CHAT_TYPE_ALLCHAT);
void	CmdDGFail(char* pMsg, DWORD dwLen);
void	CmdDGWhisper(char* pMsg, DWORD dwLen);
void	CmdDGMessage(char* pMsg, DWORD dwLen);
void	CmdDGChatUser(char* pMsg, DWORD dwLen);

void	ChatListAdd(char* pszText, DWORD dwCol, BYTE byType, BOOL bChkSystemMsg = TRUE);
void	AddChat(char* pszText, DWORD dwColor, BYTE byType, int nSize = 0);
void	ChkMaxChaList(BYTE byType);
void	RemoveChat(POSITION_ pos, BYTE byType);

void	ChatModeOn(DWORD dwInputId, BOOL bClear = TRUE);
void	ChatModeOff(DWORD dwInputId, DWORD dwFocusId = 0, BOOL bChk = FALSE, BOOL bClear = TRUE);
void	IMESetEdit(DWORD dwIndex, BOOL bChk = TRUE, BOOL bBack = FALSE, BOOL bNative = FALSE);

void	WorldMessageAdd(char* pszMessage, DWORD dwChatColor);

// by deepdark
#ifdef _USE_IME
void	EnableChatInput(BOOL bClear = FALSE, DWORD dwIndex = 1); // INDEX가 0이면 조절안함.
#endif

#endif
//==============================================================//
// End.															//
//==============================================================//

