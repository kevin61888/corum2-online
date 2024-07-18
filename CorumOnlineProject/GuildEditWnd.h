//==========================================================//
// Code by Jang.								2003.05.03	//
// Object : Guild Edit.										//
//==========================================================//
#ifndef		__GUILDEDIT_INC__
#define		__GUILDEDIT_INC__

#include	"Menu.h"

#define SPR_OBJ_GUILDEDIT_WND			0
#define SPR_OBJ_GUILDEDIT_BAR			1
#define SPR_OBJ_GUILDEDIT_CLOSE1		2
#define SPR_OBJ_GUILDEDIT_CLOSE2		3
#define SPR_OBJ_GUILDEDIT_OK1			4
#define SPR_OBJ_GUILDEDIT_OK2			5
#define SPR_OBJ_GUILDEDIT_OK3			6
#define SPR_OBJ_GUILDEDIT_CANCEL1		7
#define SPR_OBJ_GUILDEDIT_CANCEL2		8
#define SPR_OBJ_GUILDEDIT_CANCEL3		9
#define SPR_OBJ_GUILDEDIT_CHK			10

class CGuildEditWnd : public CMenu
{
public:

	BYTE	m_byGuildType;
	
	char	m_szInfo[TEXT_RENDER_BUFFER];
	RECT	m_rtPos;		

	char	m_szNickName[MAX_GUILD_NAME_LENGTH];
	char	m_szCharacterName[MAX_GUILD_NAME_LENGTH];

	BYTE	m_byRank;	
	BYTE	m_byUesrRank;
	
	DWORD	m_dwUserIndex;

	BOOL	m_bBtn[2];

public:

	// Singleton Patten //
private:
	static CGuildEditWnd* c_pThis;

public:
	static CGuildEditWnd*	GetInstance()		{ if(!c_pThis) c_pThis = new CGuildEditWnd; return c_pThis; }
	static void			DestroyInstance()	{ if(c_pThis) { delete c_pThis; c_pThis = NULL;} }


	// 공통으로 사용되는 것들은 재정의 한다 //
	BOOL	Init();

	void	RenderText();	
	void	Remove();
	void	SetOrder();
	void	SetActive(BOOL bActive = TRUE);

	int		CheckInterface();

	// 체크 관련 함수 //	
	void	SetData(char* pszName, char* pszNickName, BYTE bRank, DWORD dwUserIndex);
	void	SetRankChk(BYTE bRank = 0);	

	CGuildEditWnd();
	virtual ~CGuildEditWnd();
};

#endif
//==========================================================//
// End.														//
//==========================================================//