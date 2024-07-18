#pragma once

#include "define.h"

#define MAX_INPUT_BUFFER_NUM		21

enum	INPUT_BUFFER_ENUM
{
	INPUT_BUFFER_0 = 0,					// 사용하면 안됨.
	INPUT_BUFFER_CHARACTER_NAME1 = 1,
	INPUT_BUFFER_1 = 1,	
	INPUT_BUFFER_WORLD_CHAT	= 1,
	INPUT_BUFFER_LOGIN_ID = 1,
	INPUT_BUFFER_CHARACTER_NAME2 = 2,
	INPUT_BUFFER_2 = 2,
	INPUT_BUFFER_LOGIN_PASSWORD = 2,
	INPUT_BUFFER_CHARACTER_RACE1 = 3,
	INPUT_BUFFER_3 = 3,
	INPUT_BUFFER_LOGIN_SERVER_LIST = 3,
	INPUT_BUFFER_CHARACTER_CLASS1 = 4,
	INPUT_BUFFER_4 = 4,
	INPUT_BUFFER_CHARACTER_EXP = 5,
	INPUT_BUFFER_5 = 5,
	INPUT_BUFFER_CHARACTER_LEVEL = 6,
	INPUT_BUFFER_6 = 6,
	INPUT_BUFFER_WORLD_CHAT_GUILD_PARTY_USERNAME = 6,
	INPUT_BUFFER_CHARACTER_STR = 7,
	INPUT_BUFFER_7 = 7,
	INPUT_BUFFER_WORLD_DUNGEON_NAME	= 7,
	INPUT_BUFFER_CHARACTER_VIT = 8,
	INPUT_BUFFER_8 = 8,
	INPUT_BUFFER_WORLD_DUNGEON_SIEGE_DEFENSECOUNT = 8,
	INPUT_BUFFER_CHARACTER_DEX = 9,
	INPUT_BUFFER_9 = 9,
	INPUT_BUFFER_WORLD_DUNGEON_SIEGE_TIME	=	9,
	INPUT_BUFFER_CHARACTER_INT = 10,
	INPUT_BUFFER_10 = 10,
	INPUT_BUFFER_WORLD_DUNGEON_LEVELUP_TIME	= 10,
	INPUT_BUFFER_CHARACTER_EGO = 11,
	INPUT_BUFFER_11 = 11,
	INPUT_BUFFER_WORLD_DUNGEON_MANAGER	=	11,
	INPUT_BUFFER_WORLD_DUNGEON_JOIN_FAIL	=	11,
	INPUT_BUFFER_12 = 12,
	INPUT_BUFFER_WORLD_DUNGEON_MEMO	= 12,
	INPUT_BUFFER_13 = 13,
	INPUT_BUFFER_WORLD_DUNGEON_TYPE = 13,
	INPUT_BUFFER_CHARACTER_RACE2 = 14,
	INPUT_BUFFER_14 = 14,
	INPUT_BUFFER_WORLD_DUNGEON_LEVELTYPE = 14,
	INPUT_BUFFER_CHARACTER_CLASS2 = 15,
	INPUT_BUFFER_15 = 15,
	INPUT_BUFFER_WORLD_DUNGEON_LEVEL = 15,
	INPUT_BUFFER_16 = 16,
	INPUT_BUFFER_WORLD_DUNGEON_LAYER_COUNT = 16,
	INPUT_BUFFER_CHARACTER_JUMIN = 16,
	INPUT_BUFFER_17 = 17,
	INPUT_BUFFER_WORLD_DUNGEON_GUILD = 17,
	INPUT_BUFFER_18 = 18,
	INPUT_BUFFER_WORLD_DUNGEON_MANAGER2 = 18,
	INPUT_BUFFER_19 = 19,
	INPUT_BUFFER_WORLD_DUNGEON_ITEM	= 19,
	INPUT_BUFFER_20 = 20,
	INPUT_BUFFER_PARTY_ENTRY = 20,
};

struct INPUT_BUFFER
{
	DWORD	dwColor;	

	BYTE	bFontIndex;			//Font 배열 번호 
	RECT	Pos;				//화면상에 Display될 좌표 
	DWORD	dwInputIndex;		//현재 InputBuffer Index
	BYTE	bLine;				//현재 라인
	BYTE	bPassword;			//패스워드로 쓰일경우면 *로 렌더 
	char	szBuffer[ MAX_INPUT_BUFFER_SIZE ];	//Buffer

	int		nOrder;

	char*	GetCurPos()	{	return &szBuffer[ dwInputIndex ];	}

	BOOL PtInRect(LONG x, LONG y)
	{
		if(x >= Pos.left && x <= Pos.right && y >= Pos.top && y <= Pos.bottom)
			return TRUE;
		else
			return FALSE;
	};
};

class CInputManager
{
	BOOL			m_bDrawCursor;					// 수량 윈도우의 커서 깜빡임을 위해!! by deepdark.
	DWORD			m_dwCurFocusInputID;			// 현재 캐럿의 아이디
	INPUT_BUFFER	m_InputBuffer[MAX_INPUT_BUFFER_NUM];
	
	

public:
	CInputManager();
	~CInputManager();

	void			Initialize();
	void			InitializeInput(DWORD dwInputID, BYTE bFontID, RECT& rt, DWORD dwColor, int nOrder, BYTE bPassword = FALSE, BOOL bChk = TRUE);
	DWORD			GetCurFocusInputID();					// 현재 인풋버퍼 아이디 리턴
	DWORD			GetInputBufferLength(DWORD dwInputID);	//	해당 인풋버퍼에 채워진 문자수
	void			InsertCharacter(DWORD dwInputID, char* szBuf, DWORD dwLen);			// 현재 인풋버퍼 마지막에 길이 만큼 카피 한다.
	
	void			SetCharacter(DWORD dwInputID, DWORD dwLen);

	void			RenderInputBuffer(DWORD dwInputID);
	void			RenderInputBufferAll();				// 인풋버퍼에 있는 내용을 출력하라.
	void			BlinkInputCaret();					// 현재 캐럿을 깜빡거리기.
	void			SetFocusInput(DWORD dwInputID);								// 현재 인풋버퍼를 정한다.
	void			ClearInput(DWORD dwInputID);								// 인풋 버퍼를 지운다.
	void			SetInputColor(DWORD dwInputID, DWORD dwColor);				// 인풋 버퍼의 색을 정한다.
	void			RenderConvertToSpecialCharacter(DWORD dwInputID, char c);	// 특정한 문자로 버퍼길이 만큼 채우기
	char*			GetInputBuffer(DWORD dwInputID, BOOL bChk = FALSE);			// 캐럿을 없앤 순수 문자열을 얻어온다.
	BOOL			PtInRect(DWORD dwInputID, long x, long y);
	void			BackSpaceEvent();
	int				GetInputOrder(DWORD dwInputID);
	void			SetInputOrder(DWORD dwInputID, int nOrder);
	RECT&			GetInputRect(DWORD dwInputID);
	BOOL			IsDrawCursor(); // added by deepdark

	void			SetInput(DWORD dwInputID, char* pszBuffer);
	
};
