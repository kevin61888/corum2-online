/*
//////////////////////////////////////////////////////////////////////////////////////

	CIMEEdit Class.

		- 다국어 입력 지원을 위한 IME 관련 메쏘드를 지원.
		- Written by 코룸 사업부 최인호, deepdark (deepdark@esofnet.com, deepdark@devpia.com)
		- Append  by 코룸 사업부 유재영, seven (y2jinc@esofnet.com, y2jinc@hitel.net)

	현재의 버전은 한국어, 일본어를 지원. 중국어 등의 경우 추가적인 코드가 필요할 수 있음.
	차후, 랭귀지 정보를 생성자에서 받아 처리하도록 수정하는 것이 좋을 것으로 생각됨.

    중국/대만 버전은 윈도우에서 제공하는 IME 캔디 데이터 리스트 윈도우를 사용한다. 
	(부분적으로 문제가 되는 부분이 발생하여 그렇게 처리하였음.)

	다국어 처리 주의 사항.
	- DBCS/MBCS의 차이를 명확히 할 것(일본어의 경우 1바이트 문자, 2바이트 문자 모두 존재)
	- 각 언어별 특성을 이해할 것. ConversionMode로 모든 상태를 표현할 수 없다.
	  ConversionMode를 신뢰하지 마라.
	
	Modify List(date / writer)

///////////////////////////////////////////////////////////////////////////////////////
*/


#ifndef __IMEEDIT_H__2d57894b_2168_4268_9a16_1860a13e2f3d__
#define __IMEEDIT_H__2d57894b_2168_4268_9a16_1860a13e2f3d__

#pragma once

#include <imm.h>
#include "Define.h"
#ifdef SS3D_TEST
#include "./../SS3D_0719/4dyuchigx/4DyuchiGRX_common/IExecutive.h"	
#else
#include "./../4DyuchiGRX_common/IExecutive.h"	
#endif

#pragma comment(lib, "Imm32.lib")

///////////////////////////////////////////////////////////////////////////////////////

// 이 세개의 상수는 전체적으로 가능한 텍스트 길이에 따라 수정한다.
// 이 상수에 의해 차지하는 메모리가 크므로 반드시 알맞은 크기를 찾는다.
#define MAX_RESULT						64	// RESIZE BY INPUT BUFFER LENGTH OF APP.
#define MAX_CLAUSE_TEXT					256 // MAX 256 BYTE!
#define MAX_CLAUSE						64	//

#define EXTRA_STRING_SIZE				10	// Result Composition String * EXTRA_STRING_SIZE

// Clause Attribute Code.
#define CLAUSE_ERROR					-2	// 에러!
#define CLAUSE_ATTR_NONE				-1	// 변환완료된 문자열
#define CLAUSE_NOLINE					0	// 변환 전의 문자열
#define CLAUSE_THICKLINE				1	// 현재 변환중인 문자열
#define CLAUSE_THINLINE					2	// 현재 변환 대기 중인 문자열

// 24BIT RGB 값.
#define BACK_COLOR						RGB(0x00, 0x00, 0x00)
#define TEXT_COLOR						RGB(0xff, 0xff, 0xff)
#define COMP_COLOR						RGB(0x50, 0x50, 0x80)
#define LINE_COLOR						RGB(0x08, 0x08, 0x08)
#define CURSOR_COLOR					RGB(0xff, 0xff, 0xff)

#define WM_IME_SPRITECHANGE				(WM_USER + 1221)

#define MAX_CANDIDATE_LIST				4
#define DEFAULT_CAND_NUM_PER_PAGE		9
#define IDT_IME							1221

#define CANDIDATE_BACK_COLOR			RGB(0xc0, 0xc0, 0xc0)
#define CANDIDATE_TEXT_COLOR			RGB(0x50, 0x50, 0x80)
#define CANDIDATE_SELECTED_BACK_COLOR	RGB(0x00, 0x00, 0x80)
#define CANDIDATE_SELECTED_TEXT_COLOR	RGB(0xff, 0xff, 0xff)


// ------------------------------------------------------
// Description : KeyLayout 의 언어 부분(WORD) ( 언어 )
// ------------------------------------------------------
enum LANGUAGE_CODE
{
	LanguageCode_Chinese_Taiwan = 0x0404,
	LanguageCode_Japanese		= 0x0411,
	LanguageCode_Korean			= 0x0412,
	LanguageCode_Chinese_Prc	= 0x0804,
};

// ------------------------------------------------------
// Description : IME KeyLayout 식별 값(DWORD) ( IME 식별+언어 )
// ------------------------------------------------------
enum IME_INPUT_IDENTIFY
{
	Ime_Input_Identity_Korean					= 0xe0010412,	// (Korean default)  
	Ime_Input_Identity_Japanese_IME_Standard	= 0xe0010411,	// IME Standard 8.2 (Japan default)
	Ime_Input_Identity_Chinese_Intelligence_ABC = 0xe0040804,	// 지능 ABC
	Ime_Input_Identity_Chinese_Pinyin			= 0xe00e0804,	// Pinyin IME 3.	(china default)
	Ime_Input_Identity_Chinese_five_gaga		= 0xe0230804,	// 
	Ime_Input_Identity_Taiwan_New_Phonetic		= 0xe0080404,	// Phonetic			(Taiwan default)
};

// ------------------------------------------------------
// Description : 각국 코드 페이지 
// ------------------------------------------------------
enum NATIONAL_CODE_PAGE
{
	CodePage_Vietnamese				= 1258,
	CodePage_Baltic					= 1257,
	CodePage_Arabic					= 1256,
	CodePage_Hebrew					= 1255,
	CodePage_Turkish				= 1254,
	CodePage_Greek					= 1253,
	CodePage_Latin1					= 1252,
	CodePage_Cyrillic				= 1251,
	CodePage_European				= 1250,
	CodePage_Chinese_Traditional	= 950,	
	CodePage_Korean					= 949,
	CodePage_Chinese_Simplified		= 936,
	CodePage_Japanese				= 932,	
	CodePage_Thai					= 874,	
	CodePage_Multilingual			= 850,	
	CodePage_USA_English			= 437,	
};


#ifndef __ORDER_USERINTERFACE_START_
#define __ORDER_USERINTERFACE_START_	0
#endif

#define IMEOPTION_NORMAL				0x00
#define IMEOPTION_PASSWORD				0x01
#define IMEOPTION_SPACEENABLED			0x02
#define IMEOPTION_ERRTEXTOUT			0x04

///////////////////////////////////////////////////////////////////////////////////////
class CIMEEdit
{
	class CTextWithClause
	{
		// CLAUSE INFOS
		char	m_szResult[MAX_RESULT];						// result string!
		char	m_szClause[MAX_CLAUSE][MAX_CLAUSE_TEXT];	// composition clauses!
		char	m_Attribute[MAX_CLAUSE];					// attribute of composition clause
		int		m_iClauseCount;
		// TEXT INFOS.
		int		m_iCursorX;		// Cursor Position.
		DWORD	m_dwResultLen;	// By Byte!!

	public:
		CTextWithClause();

		int		GetCursor()			{ return m_iCursorX;	}
		LPCSTR	GetResultString()	{ return m_szResult;	}
		DWORD	GetResultLen()		{ return m_dwResultLen;	}
		int		GetClauseCount()	{ return m_iClauseCount;}		

		void	Clear();
		void	ClearComposition();
		void	ClearResult();
		void	SetResultString(LPCSTR szString);

		void	AppendResultString(LPCSTR szResult, DWORD dwLen);
		void	AppendClause(LPCSTR szAClause, char Attr);		

		int		GetLeftString(OUT char* szTarget);
		int		GetRightString(OUT char* szTarget);
		char	GetClauseString(int index, OUT char* szBuf);		

		void	OnKeyDownBack();
		void	OnKeyDownDelete();
		void	OnKeyDownLeft();
		void	OnKeyDownRight();
		void	OnKeyDownHome();
		void	OnKeyDownEnd();
	};

public:

	static CIMEEdit* GetInstance();
	static void DestroyInstance();

	~CIMEEdit();

	// Activatation/Deactivation!
	void	ActivateIME(POINT ptEdit, int iMaxLenInByte = 64, 
						int zOrder = __ORDER_USERINTERFACE_START_, DWORD dwOption = IMEOPTION_ERRTEXTOUT); 
	void	ActivateIME(int x, int y, int iMaxLenInByte = 64, 
						int zOrder = __ORDER_USERINTERFACE_START_, DWORD dwOption = IMEOPTION_ERRTEXTOUT); 	
	void	DeactivateIME(BOOL bClearEditString = TRUE);

	// Edit Activation Related Methods.
	BOOL	IsActivated();

	// change language input mode
	void	SetNativeLanguageMode(BOOL bSet = TRUE);

	// Main Message Loop Pre-Processing Function!
	BOOL	ProcessIMEMessage(UINT msg, WPARAM wparam, LPARAM lparam);
	
	// Initialization Method.
	void	InitializeIME(I4DyuchiGXExecutive* pIExecutive, HWND hWnd);

	// IME Edit String Related Methods.
	LPCSTR	GetImeEditString();
	void	SetImeEditString(LPCSTR szString);
	void	ClearImeEditString();

	// Edit String Texture related Methods.
	void	CreateSprite(HDC dc, int nTextWidth, int nTextHeight, int nCandWidth, int nCandHeight);	
	BOOL	RenderSprite(int zOrder = __ORDER_USERINTERFACE_START_);
	void	DeleteSprite();
	
	// Other Configuration Methods..
	void	SetEditFonts(LONG lFontSize = -12, BYTE btCharSet = DEFAULT_CHARSET);
	void	SetStatusWndPos(int x = -10000, int y = -10000);		// IME Status Wnd의 위치 지정
	void	SetEditPos(int x, int y);
	POINT	GetEditPos();
	void	SetMaxTextLen(int nByte = 64);							// 최대 입력 길이 지정(byte)
	int		GetMaxTextLen();
	void	SetOrder(int iZOrder = __ORDER_USERINTERFACE_START_);
	int		GetOrder();

	void	SetEditIndex(DWORD dwIndex = 0);
	DWORD	GetEditIndex();

	void	SetOption(DWORD dwOption);								// 현재 옵션을 통째로 세팅.
	DWORD	GetOption();											// 현재 옵션 가져옴.
	void	SetPasswordMode(BOOL bSet = TRUE);						// 패스워드 모드로 세팅
	void	SetErrTextOut(BOOL bTextOut = TRUE);					// 에러 시 화면에 에러 출력여부 세팅
	void	EnableSpaceKey(BOOL bEnable = TRUE);					// 공백 입력 가능하도록 세팅
	void	SetInputBlock(BOOL bBlock = TRUE);						// 키입력 처리를 할 것인가 말것인가!
	void	SetKeyLayout(DWORD dwKeyLayout);						// dwKeyLayout 의 키레이아웃을 설정한다.

protected:
	CIMEEdit();

	void	Activate();
	void	Deactivate(BOOL bClearEditString);

	// 지정된 DC의 RECT에 출력한다.
	void	DrawToTextSprite();
	void	DrawTextSprite(HDC dc, RECT rc);
	void	DrawCompositionText(HDC dc, RECT rc, LPCSTR szText, COLORREF text, COLORREF back, int Attr);

	void	DrawToCandidateSprite();
	void	DrawCandidateSprite(HDC dc, RECT rc);

private:
	BOOL	AcquireIME();
	void	ReleaseIME();
	
	void	EnableIME(bool bEnable = TRUE);

	void	ClearCompStr();
	BOOL	CheckIMEProperty();
	BOOL	SetCompositionFont(LPLOGFONT lf);

	void	NotifyIMESpriteChange();
	
	BOOL	GetResultString();
	BOOL	GetCompString(LONG flag);
	int		GetLeftString(OUT char* target);
	int		GetRightString(OUT char* target);

	void	SaveLastConversionMode();
	void	LoadLastConversionMode();

	// for ime Message 
	void	OnNotifyIMESpriteChange();
	BOOL	OnChar(UINT nChar);
	BOOL	OnKeyDown(WPARAM nChar);
	LONG	OnIMESetContext(WPARAM dwCommand, LPARAM dwData);
	LONG	OnIMEStartComposition(WPARAM dwCommand, LPARAM dwData);
	LONG	OnIMEComposition(WPARAM dwCommand, LPARAM dwData);
	LONG	OnIMEEndComposition(WPARAM dwCommand, LPARAM dwData);
	LONG	OnIMECompositionFull(WPARAM dwCommand, LPARAM dwData);
	LONG	OnIMEControl(WPARAM dwCommand, LPARAM dwData);
	LONG	OnIMENotify(WPARAM dwCommand, LPARAM dwData);
	LONG	OnInputLangChange(WPARAM dwCommand, LPARAM dwData);

	// for candidate window, OnNotify 시 처리...
	BOOL	OnNotifyOpenCandidate(LONG lParam);		// IMN_OPENCANDIDATE
	BOOL	OnNotifyCloseCandidate(LONG lParam);	// IMN_CLOSECANDIDATE
	BOOL	OnNotifyChangeCandidate(LONG lParam);	// IMN_CHANGECANDIDATE
//	BOOL	OnNotifySetOpenStatus(LONG lParam);		// IMN_SETOPENSTATUS

	BOOL	GetCandidateList(LONG lParam);
	BOOL	CheckAttrInput(char* attr, int iLen);
	void	SetCompClauses(char* strComp, char* szAttr, LPDWORD pClause, int iClauseCount);

	BOOL	IsChineseLanguage();
	BOOL	IsTaiwanLanguage();
	BOOL	IsJapaneseLanguage();

	HIMC	m_hIMC;
	HWND	m_hWnd; 
	HKL		m_hKeyLayout;				// Keyboard Layout(Locale related)
	HKL		m_hKeyLayoutOfActivating;	// Activating of Keyboard Layout(Locale related)
	int		m_iTextCharset; // character set code.
	DWORD	m_property;		// IME Property.
	int		m_nMaxTextLen;
	int		m_zOrder;
	DWORD	m_dwEditIndex;
	BOOL	m_bActivated;
	BOOL	m_bCandidateSet;	
	BOOL	m_bOpenStatus;
	BOOL	m_bDrawCursor;
	BOOL	m_bInputBlock;
	DWORD	m_dwOption;
	DWORD	m_dwConversionMode;
	DWORD	m_dwKeyLayoutOfActivating;
	DWORD	m_dwConversionModeOfActivating;	// Activating of ConversionMode(native/english)를 기억한다.
	DWORD	m_dwSentenceMode;

	CTextWithClause		m_TextWithClause;
	I4DyuchiGXExecutive* m_pIExecutive;
	HFONT				m_hFont;
	HPEN				m_hThinPen;
	HPEN				m_hThickPen;

	// FOR Composition Text
	HDC					m_hTextMemDC;
	HBITMAP				m_hTextBitmap;
	PBITMAPINFO			m_pbiText;
	LPBYTE				m_lpTextBits;
	IDISpriteObject*	m_pTextSprite;
	RECT				m_TextSpriteRect;
	VECTOR2				m_TextSpritePos;
	VECTOR2				m_TextSpriteScale;

	// for Candidate Wnd
	HDC					m_hCandidateMemDC;
	HBITMAP				m_hCandidateBitmap;
	PBITMAPINFO			m_pbiCandidate;
	LPBYTE				m_lpCandidateBits;
	IDISpriteObject*	m_pCandidateSprite;
	RECT				m_CandidateSpriteRect;
	VECTOR2				m_CandidateSpritePos;
	VECTOR2				m_CandidateSpriteScale;

	LPCANDIDATELIST		m_pCandidateLists;

	static CIMEEdit*	s_pCIMEEditInstance;
};

#define GET_IMEEDIT() CIMEEdit::GetInstance()
#define DESTROY_IMEEDIT() CIMEEdit::DestroyInstance()

#endif // __IMEEDIT_H__2d57894b_2168_4268_9a16_1860a13e2f3d__