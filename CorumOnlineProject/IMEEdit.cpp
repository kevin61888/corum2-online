#include "stdafx.h"
#include "IMEEdit.h"

#include "Develop.h"
#include "Chat.h"
#include "Message.h"
#include "InitGame.h"
#include "system_info.h"

#include "Guild.h"
#include "SafeMemory.h"

typedef unsigned short int	WORD;


//------------------------------------------------------------------
// Name : SetCapital
// Desc : Caps Lock 키 ON / OFF switch 시키는 기능을 한다.
//------------------------------------------------------------------
void SetCapital(bool bState)
{
	BYTE bKeyState[256];

	GetKeyboardState((unsigned char *)&bKeyState);
	if ( (bState && !(bKeyState[VK_CAPITAL] & 1)) ||
		 (!bState && (bKeyState[VK_CAPITAL] & 1)) )
	{
		keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY |0, 0);

		keybd_event(VK_CAPITAL, 0, KEYEVENTF_EXTENDEDKEY |KEYEVENTF_KEYUP, 0);
	}
}

//------------------------------------------------------------------
// Name : SetCtrlSpaceHotKey
// Desc : Ctrl + Space 키를 누르도록 설정 
//------------------------------------------------------------------
void SetCtrlSpaceHotKey()
{
	keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY |0, 0);
	keybd_event(VK_SPACE, 0, KEYEVENTF_EXTENDEDKEY |0, 0);

	keybd_event(VK_CONTROL, 0, KEYEVENTF_EXTENDEDKEY |KEYEVENTF_KEYUP, 0);
	keybd_event(VK_SPACE, 0, KEYEVENTF_EXTENDEDKEY |KEYEVENTF_KEYUP, 0);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// static functions for texture creation.
static BOOL __Check16BitColor(BITMAP bmp)
{
	WORD cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 

	if		(cClrBits == 1)		cClrBits = 1; 
    else if (cClrBits <= 4)		cClrBits = 4; 
    else if (cClrBits <= 8)		cClrBits = 8; 
    else if (cClrBits <= 16)	cClrBits = 16; 
    else if (cClrBits <= 24)	cClrBits = 24; 
    else						cClrBits = 32; 

	return (cClrBits == 16);
}

static PBITMAPINFO __CreateBitmapInfoStructFor16Bit(HBITMAP hBmp)
{
	BITMAP		bmp;
	PBITMAPINFO pbmi;

	if(!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp)) return NULL;

	// check! - 16 bit color mode.
	if(!__Check16BitColor(bmp))
	{
		MessageBox(	NULL, 
					"This Application Supports Only 16 Bits High Color Mode.\r\n"
					"Set Color Mode Correctly, And Retry.", 
					"Color Setting Error", 
					MB_ICONERROR|MB_OK);
#ifdef DEVELOP_MODE
		_asm { int 3 }
#endif
	}

	pbmi = (PBITMAPINFO) new BYTE [sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<<16)]; 
	if(pbmi == NULL)
	{
		MessageBox(	NULL,
					"BitmapInfo Allocation Failed!",
					"Memory Failure", 
					MB_ICONERROR|MB_OK);

		_asm { int 3 }
	}

	SAFEMEMSET(pbmi, 0, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<<16));

	pbmi->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER); 
	pbmi->bmiHeader.biWidth			= bmp.bmWidth; 
	pbmi->bmiHeader.biHeight		= -bmp.bmHeight; // 이미지 반전을 막기위해 마이너스!
	pbmi->bmiHeader.biPlanes		= bmp.bmPlanes; 
	pbmi->bmiHeader.biBitCount		= bmp.bmBitsPixel; 
	pbmi->bmiHeader.biClrUsed		= (1<<16); 
	pbmi->bmiHeader.biCompression	= BI_RGB; 
	pbmi->bmiHeader.biSizeImage		= bmp.bmWidth * bmp.bmHeight * 2;
	pbmi->bmiHeader.biClrImportant	= 0;

	return pbmi;
}

static void __SetLockedRectWithBitmapData(LPWORD lpSrc, LOCKED_RECT& LockRect, int iWidth, int iHeight)
{
	LPWORD lpDes = NULL;

	for(int i = 0; i < iHeight; i++)
	{
		lpDes = LPWORD((LPBYTE)LockRect.pBits + LockRect.Pitch*i);

		for(int j = 0; j < iWidth; j++)
		{
			*lpDes++ = 
				lpSrc[iWidth * i + j] ? 
				lpSrc[iWidth * i + j] | 0x8000 : 0;
		}		
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
CIMEEdit* CIMEEdit::s_pCIMEEditInstance = NULL;

CIMEEdit* CIMEEdit::GetInstance()
{
	if(!s_pCIMEEditInstance)
	{
		s_pCIMEEditInstance = new CIMEEdit;
	}

	return s_pCIMEEditInstance;
}

void CIMEEdit::DestroyInstance()
{
	if(s_pCIMEEditInstance)
	{
		delete s_pCIMEEditInstance;
		s_pCIMEEditInstance = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
CIMEEdit::CTextWithClause::CTextWithClause()
{ 
	Clear(); 
}

void CIMEEdit::CTextWithClause::Clear()
{
	m_iClauseCount	= 0;	
	m_iCursorX		= 0;
	m_dwResultLen	= 0;

	SAFEMEMSET(m_szResult, 0, MAX_RESULT);
	SAFEMEMSET(m_szClause, 0, MAX_CLAUSE * MAX_CLAUSE_TEXT);
	SAFEMEMSET(m_Attribute, -1, MAX_CLAUSE);
}

void CIMEEdit::CTextWithClause::ClearComposition()
{
	SAFEMEMSET(m_szClause, 0, MAX_CLAUSE * MAX_CLAUSE_TEXT);
	SAFEMEMSET(m_Attribute, -1, MAX_CLAUSE);
	m_iClauseCount = 0;
}

void CIMEEdit::CTextWithClause::ClearResult()
{
	SAFEMEMSET(m_szResult, 0, MAX_RESULT);
	m_dwResultLen = 0;
}

void CIMEEdit::CTextWithClause::SetResultString(LPCSTR szString)
{ 	
	lstrcpy(m_szResult, szString); 
	
	m_iCursorX = lstrlen(szString);
	m_dwResultLen = m_iCursorX;
}

void CIMEEdit::CTextWithClause::AppendClause(LPCSTR szAClause, char Attr) 
{
	if(m_iClauseCount == MAX_CLAUSE) return;
	
	lstrcpy(m_szClause[m_iClauseCount], szAClause);	
	m_Attribute[m_iClauseCount++] = Attr;
}

char CIMEEdit::CTextWithClause::GetClauseString(int index, OUT char* szBuf)
{
	if(index >= m_iClauseCount) return CLAUSE_ERROR;
	
	lstrcpy(szBuf, m_szClause[index]);
	
	return m_Attribute[index];
}

// character.. process.
int CIMEEdit::CTextWithClause::GetLeftString(char* szTarget)		// 커서를 중심으로 왼쪽 문자들을 얻는다
{
	if(m_iCursorX == 0) return 0;
	
#ifdef DEVELOP_MODE
	if(m_iCursorX < 0) _asm { int 3 }
#endif

	__lstrcpyn(szTarget, m_szResult, m_iCursorX);
	
	return m_iCursorX;
}

int CIMEEdit::CTextWithClause::GetRightString(char* szTarget)		// 커서를 중심으로 오른쪽 문자들을 얻는다.
{
	if((DWORD)m_iCursorX == m_dwResultLen) return 0;

	__lstrcpyn(szTarget, m_szResult + m_iCursorX, m_dwResultLen - m_iCursorX);	

	return (m_dwResultLen - m_iCursorX);
}

void CIMEEdit::CTextWithClause::AppendResultString(LPCSTR szResult, DWORD dwLen)
{
	// CURSOR 오른쪽의 문자열을 얻어온다.
	char szRightStr[MAX_RESULT] = {0,};
	int iRightLen = GetRightString(szRightStr);

	if(IsGuildCreate())
	{
		int nSize = lstrlen(szResult);

		for(int i = 0; i < nSize; i++)
		{
			if(szResult[i]==32)
			{
				memset((void*)&szResult[i], 0, 1);
				dwLen--;
				break;
			}
		}
	}

	// 새로 받은 Result 문자열 복사!
	SAFEMEMCPY(m_szResult+m_iCursorX, szResult, dwLen);		

	m_iCursorX += dwLen;
	
	if(iRightLen)
	{
		SAFEMEMCPY(m_szResult+m_iCursorX, szRightStr, iRightLen);			
	}

	m_dwResultLen += dwLen;

	m_szResult[m_dwResultLen] = 0;
}

void CIMEEdit::CTextWithClause::OnKeyDownBack()
{
#ifdef DEVELOP_MODE
	if(m_iCursorX < 0) _asm {int 3}
#endif 

	if(m_dwResultLen == 0) return;

	char szRightStr[MAX_RESULT] = {0,};
	int iRightLen = GetRightString(szRightStr);

	LPSTR szPrev = CharPrevExA(0, m_szResult, &m_szResult[m_iCursorX], 0);

	int iLen = lstrlen(&m_szResult[m_iCursorX]) - lstrlen(szPrev);

	m_iCursorX += iLen;
	m_dwResultLen += iLen; 

#ifdef DEVELOP_MODE
	if(m_iCursorX < 0) _asm {int 3}
	if(m_dwResultLen < 0) _asm {int 3}
#endif 

	if(iRightLen)
	{
		__lstrcpyn(m_szResult + m_iCursorX, szRightStr, iRightLen);		
	}
	
	m_szResult[m_dwResultLen] = 0;
}

void CIMEEdit::CTextWithClause::OnKeyDownDelete()
{
	if((DWORD)m_iCursorX == m_dwResultLen) return;

	LPSTR szNext = CharNextExA(0, &m_szResult[m_iCursorX], 0);
	int iLen = lstrlen(&m_szResult[m_iCursorX]) - lstrlen(szNext);

	__lstrcpyn(m_szResult + m_iCursorX, szNext, lstrlen(szNext));

	m_dwResultLen -= iLen;

#ifdef DEVELOP_MODE
	if(m_dwResultLen < 0) _asm {int 3}
#endif 

	m_szResult[m_dwResultLen] = 0;
}

void CIMEEdit::CTextWithClause::OnKeyDownLeft()
{
	if(m_iCursorX == 0) return;
	
	LPSTR szPrev = CharPrevExA(0, m_szResult, &m_szResult[m_iCursorX], 0);

	int iLen = lstrlen(&m_szResult[m_iCursorX]) - lstrlen(szPrev);

	m_iCursorX += iLen;

#ifdef DEVELOP_MODE
	if(m_iCursorX < 0) _asm { int 3}
#endif
}

void CIMEEdit::CTextWithClause::OnKeyDownRight()
{
	if((DWORD)m_iCursorX == m_dwResultLen) return;

	LPSTR szNext = CharNextExA(0, &m_szResult[m_iCursorX], 0);
	int iLen = lstrlen(&m_szResult[m_iCursorX]) - lstrlen(szNext);

	m_iCursorX += iLen;
}

void CIMEEdit::CTextWithClause::OnKeyDownHome()
{
	m_iCursorX = 0;
}

void CIMEEdit::CTextWithClause::OnKeyDownEnd()
{
	m_iCursorX = m_dwResultLen;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CIMEEdit::CIMEEdit()
{
	m_hKeyLayout		= NULL;
	m_iTextCharset		= 0;
    m_property			= 0;
	m_nMaxTextLen		= MAX_RESULT-1;
	m_hIMC				= NULL;
	m_hWnd				= NULL;
	m_bActivated		= FALSE;
	m_bCandidateSet		= FALSE;
	m_bOpenStatus		= FALSE;
	m_bDrawCursor		= TRUE;
	m_bInputBlock		= FALSE;
	m_dwOption			= IMEOPTION_NORMAL;
	m_dwConversionMode	= 0;
	m_dwConversionModeOfActivating = 0;
	m_dwSentenceMode	= 0;
	m_zOrder			= 0;
	m_dwEditIndex		= 0;

	m_pIExecutive		= NULL;
	m_hFont				= NULL;
	m_hThinPen			= NULL;
	m_hThickPen			= NULL;

	m_lpTextBits		= NULL;
	m_pbiText			= NULL;
	m_hTextBitmap		= NULL;
	m_hTextMemDC		= NULL;
	m_pTextSprite		= NULL;
	m_TextSpritePos.x	= 0.0f;
	m_TextSpritePos.y	= 0.0f;
	m_TextSpriteScale.x = 1.0f;
	m_TextSpriteScale.y = 1.0f;	

	m_pCandidateLists	= NULL;
	m_lpCandidateBits	= NULL;
	m_pbiCandidate		= NULL;
	m_hCandidateBitmap	= NULL;
	m_hCandidateMemDC	= NULL;
	m_pCandidateSprite	= NULL;
	m_CandidateSpritePos.x	= 0.0f;
	m_CandidateSpritePos.y	= 0.0f;
	m_CandidateSpriteScale.x = 1.0f;
	m_CandidateSpriteScale.y = 1.0f;
}

CIMEEdit::~CIMEEdit()
{
	DeleteSprite();
}

void CIMEEdit::ActivateIME(POINT ptEdit, int iMaxLenInByte, int zOrder, DWORD dwOption)
{
	m_dwOption = dwOption;
	SetEditPos(ptEdit.x, ptEdit.y);
	SetMaxTextLen(iMaxLenInByte);
	SetOrder(zOrder);

	Activate();
}

void CIMEEdit::ActivateIME(int x, int y, int iMaxLenInByte, int zOrder, DWORD dwOption)
{
	m_dwOption = dwOption;
	SetEditPos(x, y);
	SetMaxTextLen(iMaxLenInByte);
	SetOrder(zOrder);

	Activate();
}

void CIMEEdit::DeactivateIME(BOOL bClearEditString)
{
	Deactivate(bClearEditString);
}

void CIMEEdit::Activate()
{
	if(m_bActivated) return;

	static bool bCreated = false;

	SetCapital(FALSE);

	if(!bCreated) 
	{
		if(m_pIExecutive)
		{
			HDC	hdc = GetDC(m_hWnd); 

			// 512*32 static texture!!
			int iTextSpriteWidth = 
				(LanguageCode_Japanese == (WORD)m_hKeyLayout) ? 1024 : 512;
			int iCandSpriteWidth = 
				(LanguageCode_Japanese == (WORD)m_hKeyLayout) ? 1024 : 256;

			CreateSprite(hdc, iTextSpriteWidth, 32, iCandSpriteWidth, 256);

			bCreated = true;

			ReleaseDC(m_hWnd, hdc);
		}
	}

	ClearCompStr();
	SetTimer(m_hWnd, IDT_IME, 500, NULL);
	NotifyIMESpriteChange();
	
	m_bActivated = TRUE;
	
	LoadLastConversionMode();		// 최근의 입력모드로 세팅한다.
}

void CIMEEdit::Deactivate(BOOL bClearEditString)
{
	if(!m_bActivated) return;

	// 최신 최신 상태를 저장하기전에 영문 모드로 바꾼다. 
	// 그렇지 않으면 윈도우의 정보 표시와 
	// 실제 오브젝트의 표시의 내용이 상이하게 된다.
	// 영문에서 종료 하고 영문바뀌어서 사용하면 문제가 되지 않겠지만
	// 중문에서 종료 하고 영문바뀌어서 사용하면 문제가 된다.
	SaveLastConversionMode();		// 최종 입력모드를 저장한다.
	SetNativeLanguageMode(FALSE);	// 강제로 영문모드로 변환한다.		
	ClearCompStr();
	if(bClearEditString) ClearImeEditString();
	KillTimer(m_hWnd, IDT_IME);
	m_bDrawCursor = TRUE;
	NotifyIMESpriteChange();

	if (IsChineseLanguage())
	{
		// Pinyin 으로 IME를 변경하고 
		// 영문 입력가능하도록 ConversionStatus를 설정한다.
		SetKeyLayout(Ime_Input_Identity_Chinese_Pinyin);	
		ImmSetConversionStatus(m_hIMC, 0, this->m_dwSentenceMode);
	}

	m_bActivated = FALSE;
}

BOOL CIMEEdit::IsActivated()
{
	return m_bActivated;
}

void CIMEEdit::SetEditIndex(DWORD dwIndex)
{
	m_dwEditIndex = dwIndex;
}

DWORD CIMEEdit::GetEditIndex()
{
	return m_dwEditIndex;
}

void CIMEEdit::SetMaxTextLen(int nByte)
{
#ifdef DEVELOP_MODE
	if(MAX_RESULT < nByte) _asm int 3;
#endif

	m_nMaxTextLen = nByte+1;
}

int CIMEEdit::GetMaxTextLen() 
{ 
	return m_nMaxTextLen;	
}

void CIMEEdit::SetEditPos(int x, int y)
{
	m_TextSpritePos.x	= (float)x;
	m_TextSpritePos.y	= (float)y;

	if(IsActivated())
		NotifyIMESpriteChange();
}

POINT CIMEEdit::GetEditPos()
{
	POINT pos;
	pos.x = LONG(m_TextSpritePos.x);
	pos.y = LONG(m_TextSpritePos.y);

	return pos;
}

void CIMEEdit::SetOrder(int iZOrder)
{
	m_zOrder = iZOrder;
}

int CIMEEdit::GetOrder() 
{ 
	return m_zOrder; 
}

void CIMEEdit::InitializeIME(I4DyuchiGXExecutive* pIExecutive, HWND hwnd)
{
#ifdef DEVELOP_MODE
	if(pIExecutive == NULL) _asm int 3;
	if(hwnd == NULL) _asm int 3;
#endif

	m_pIExecutive	= pIExecutive;
	m_hWnd			= hwnd;

	m_hKeyLayout	= GetKeyboardLayout(0);	// 0 means Current thread
	m_hKeyLayoutOfActivating = m_hKeyLayout;
	m_iTextCharset	= GetTextCharset(GetDC(hwnd));
	m_property		= ImmGetProperty(m_hKeyLayout, IGP_PROPERTY);
	m_bOpenStatus	= ImmGetOpenStatus(m_hIMC);
	
	//////////////////////////////////////////////////////////////////////////
	// IME 초기화 각국 에 따라 정해진 IME를 설정해진다. 
	// 멤버 함수로 이동할것이다.
	//char szKeyLayout[0xff];

	// DEFAULT IME 설정 
	switch ((WORD)m_hKeyLayout )
	{
		case LanguageCode_Chinese_Taiwan:
			SetKeyLayout(Ime_Input_Identity_Taiwan_New_Phonetic);
		break;

/*
		case LanguageCode_Chinese_Prc:
			SetKeyLayout(Ime_Input_Identity_Chinese_Pinyin);
		break;*/


		default: 
		break;
	}
	// =========================
	

	switch((WORD)m_hKeyLayout)
	{
	case LanguageCode_Japanese:
		m_dwConversionMode				= IME_CMODE_FULLSHAPE|IME_CMODE_ROMAN|IME_CMODE_NATIVE;
		m_dwConversionModeOfActivating	= IME_CMODE_FULLSHAPE|IME_CMODE_ROMAN|IME_CMODE_NATIVE;
		m_dwSentenceMode				= IME_SMODE_PHRASEPREDICT;
		break;
		
	case LanguageCode_Chinese_Prc:	
		m_dwConversionMode	= 0;
		m_dwConversionModeOfActivating = 0;
		m_dwSentenceMode	= 0;
		break;

	case LanguageCode_Chinese_Taiwan:
	//case LanguageCode_Chinese_Prc:
	default:
		m_dwConversionMode	= 0;
		m_dwConversionModeOfActivating = 0;
		m_dwSentenceMode	= 0;
	}

	// seven-start
	// 캐럿의 위치를 위해
	::SendMessage(hwnd, WM_SETFOCUS, 0, 0);
	// seven-end
}

void CIMEEdit::SaveLastConversionMode()
{
	if(AcquireIME())
	{
		m_hKeyLayoutOfActivating = GetKeyboardLayout(0);	
		ImmGetConversionStatus(m_hIMC, &m_dwConversionMode, &m_dwSentenceMode);
		m_dwConversionModeOfActivating = m_dwConversionMode;
		ReleaseIME();
	}
}

void CIMEEdit::LoadLastConversionMode()
{
	if(AcquireIME())
	{
		switch((WORD)m_hKeyLayout)
		{
		case LanguageCode_Japanese:
			if(m_bOpenStatus != ImmGetOpenStatus(m_hIMC))
			{
				ImmSimulateHotKey(m_hWnd, IME_JHOTKEY_CLOSE_OPEN);
			}	
			break;

		case LanguageCode_Chinese_Prc:
			break;
		default:
			break;
		}


		if (!IsJapaneseLanguage())
		{
			// IME 사용중의 conversionmode로 돌린다.
			SetKeyLayout((DWORD)m_hKeyLayoutOfActivating);

			ImmSetConversionStatus(m_hIMC, m_dwConversionModeOfActivating, m_dwSentenceMode);
		}

		ReleaseIME();
	}
}

BOOL CIMEEdit::IsTaiwanLanguage()
{ 
	return bool(LanguageCode_Chinese_Taiwan == (WORD)m_hKeyLayout);
}

BOOL CIMEEdit::IsChineseLanguage()
{ 
	return bool(LanguageCode_Chinese_Prc == (WORD)m_hKeyLayout);
}

BOOL CIMEEdit::IsJapaneseLanguage()
{
	return bool(LanguageCode_Japanese == (WORD)m_hKeyLayout);
}

void CIMEEdit::SetNativeLanguageMode(BOOL bSet)
{
	if(AcquireIME())
	{		
		switch((WORD)m_hKeyLayout)
		{
		case LanguageCode_Japanese:
			ImmSetConversionStatus(m_hIMC, m_dwConversionMode, m_dwSentenceMode);
			m_bOpenStatus = ImmGetOpenStatus(m_hIMC);

			if(	(bSet && !m_bOpenStatus) || // 영문모드에 ALT+~을 보내 DirectInput Mode로 세팅한다.
				(!bSet && m_bOpenStatus) )	// DirectInput Mode의 경우, ALT+~를 보내서 영문 모드로 세팅한다.
			{
				ImmSimulateHotKey(m_hWnd, IME_JHOTKEY_CLOSE_OPEN);
			}
			break;		
		
		case LanguageCode_Chinese_Prc:
			ImmSetConversionStatus(m_hIMC, (bSet) ? IME_CMODE_NATIVE : 0, m_dwSentenceMode);
/*			m_bOpenStatus = ImmGetOpenStatus(m_hIMC);

			if(	(bSet && !m_bOpenStatus) || // 영문모드에 ALT+~을 보내 DirectInput Mode로 세팅한다.
				(!bSet && m_bOpenStatus) )	// DirectInput Mode의 경우, ALT+~를 보내서 영문 모드로 세팅한다.
			{
				if (IsActivated())
				{
					if (What_Kind_Of_Platform_Win32_WinNT == WhatKindOfPlatform())
					{ 
						// 이부분 명확치 않다. 테스트후 제수정을 가할 수 있는 부분이다.
						// window NT 시리즈 에서만 해라.
						ImmSimulateHotKey(m_hWnd, IME_CHOTKEY_IME_NONIME_TOGGLE);
					}
				}
			}
*/
			break;		
		case LanguageCode_Chinese_Taiwan:
		case LanguageCode_Korean:	
			ImmSetConversionStatus(m_hIMC, (bSet) ? IME_CMODE_NATIVE : 0, m_dwSentenceMode);
			break;
			
		default:
			ImmSetConversionStatus(m_hIMC, (bSet) ? IME_CMODE_NATIVE : 0, m_dwSentenceMode);

			break;
		}

		ReleaseIME();
	}
}

LPCSTR CIMEEdit::GetImeEditString()
{
	return m_TextWithClause.GetResultString();
}

void CIMEEdit::ClearImeEditString()
{
	m_TextWithClause.Clear();
	NotifyIMESpriteChange();
}

void CIMEEdit::SetImeEditString(LPCSTR szString)
{
	m_TextWithClause.SetResultString(szString);
	NotifyIMESpriteChange();
}

void CIMEEdit::SetStatusWndPos(int x, int y)
{
	if(AcquireIME()) 
	{
		POINT pt;

		pt.x = x;
		pt.y = y;
		
#ifdef _DEBUG
		if(!ImmSetStatusWindowPos(m_hIMC, &pt))
			OutputDebugString("ImmSetStatusWindowPos Failed!!!");
#else
		ImmSetStatusWindowPos(m_hIMC, &pt);
#endif
		ReleaseIME();
	}
}

void CIMEEdit::ClearCompStr()
{
	if(AcquireIME())
	{
		// Composition string을 무조건! 초기화시킨다.
		::ImmNotifyIME(m_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
		ReleaseIME();
	}
}

void CIMEEdit::SetOption(DWORD dwOption)
{
	m_dwOption = dwOption;
}

DWORD CIMEEdit::GetOption()
{
	return m_dwOption;
}

void CIMEEdit::EnableSpaceKey(BOOL bEnable)
{
	if(bEnable)
		m_dwOption |= IMEOPTION_SPACEENABLED;
	else
		m_dwOption &= ~IMEOPTION_SPACEENABLED;
}

void CIMEEdit::SetPasswordMode(BOOL bSet)
{
	if(bSet)
		m_dwOption |= IMEOPTION_PASSWORD;
	else
		m_dwOption &= ~IMEOPTION_PASSWORD;
}

void CIMEEdit::SetErrTextOut(BOOL bTextOut)
{
	if(bTextOut)
		m_dwOption |= IMEOPTION_ERRTEXTOUT;
	else
		m_dwOption &= ~IMEOPTION_ERRTEXTOUT;
}

void CIMEEdit::SetInputBlock(BOOL bBlock)
{
	m_bInputBlock = bBlock;
}

//-------------------
// Name : SetKeyLayout
// Desc : dwKeyLayout 의 키레이아웃을 설정한다.
// [6/4/2004]
//-------------------
void CIMEEdit::SetKeyLayout(DWORD dwKeyLayout)
{
	char szKeyLayout[0xFF] = {0,};
	wsprintf(szKeyLayout, "%08X", (DWORD)dwKeyLayout);
	LoadKeyboardLayout(szKeyLayout, KLF_ACTIVATE);
}					


BOOL CIMEEdit::AcquireIME() 
{
	if(m_hWnd == NULL) return FALSE;

	return (BOOL)(m_hIMC = ImmGetContext(m_hWnd));
}

void CIMEEdit::ReleaseIME()
{
	ImmReleaseContext(m_hWnd, m_hIMC);
}

// IME 활성 or 비활성
void CIMEEdit::EnableIME(bool bEnable)
{ 
	if ( AcquireIME() )
	{
		ImmSetOpenStatus(m_hIMC, bEnable);

		ReleaseIME();
	}
}

// character.. process.
int CIMEEdit::GetLeftString(char* target)		// 커서를 중심으로 왼쪽 문자들을 얻는다
{
	return m_TextWithClause.GetLeftString(target);
}

int CIMEEdit::GetRightString(char* target)		// 커서를 중심으로 오른쪽 문자들을 얻는다.
{
	return m_TextWithClause.GetRightString(target);
}

void CIMEEdit::SetEditFonts(LONG lFontSize, BYTE btCharSet) 
{ 
	LOGFONT lf; 

	if(m_hFont)
	{
		DeleteObject(m_hFont);
	}

	if(btCharSet == DEFAULT_CHARSET)
	{
		btCharSet = (BYTE)m_iTextCharset;
	}

	lf.lfHeight			= lFontSize; 
	lf.lfWidth			= 0; 
	lf.lfEscapement		= 0; 
	lf.lfOrientation	= 0; 
	lf.lfWeight			= FW_BOLD; 
	lf.lfItalic			= 0; 
	lf.lfUnderline		= 0; 
	lf.lfStrikeOut		= 0; 
	lf.lfCharSet		= btCharSet; 
	lf.lfOutPrecision	= 0;//OUT_DEFAULT_PRECIS; 
	lf.lfClipPrecision	= 0;//CLIP_DEFAULT_PRECIS; 
	lf.lfQuality		= PROOF_QUALITY;//DEFAULT_QUALITY; 
	lf.lfPitchAndFamily	= 0;//DEFAULT_PITCH|FF_DONTCARE;
	
	switch((WORD)m_hKeyLayout)
	{
	case LanguageCode_Japanese:	// japanness
		lstrcpy(lf.lfFaceName, "MS PGothic"); 
		break;	
	case LanguageCode_Korean:	// korean
		lstrcpy(lf.lfFaceName, "굴림체");
		break;
	case LanguageCode_Chinese_Taiwan:
		lstrcpy(lf.lfFaceName, "Mingliu");		
		break;
	case LanguageCode_Chinese_Prc:
		lstrcpy(lf.lfFaceName, "Mingliu");
		break;
	default:
		lstrcpy(lf.lfFaceName, "Courier New");
	}
	
	// Composition Font Set!
	SetCompositionFont(&lf);	
	
	// CreateFont!
	m_hFont = CreateFontIndirect(&lf); 
} 

BOOL CIMEEdit::SetCompositionFont(LPLOGFONT lf)
{
	BOOL bRet = FALSE;

	if(AcquireIME()) 
	{
		bRet = ImmSetCompositionFont(m_hIMC, lf);
		ReleaseIME();
	} 

	return bRet;
}

BOOL CIMEEdit::CheckIMEProperty() 
{
	if(	LanguageCode_Chinese_Taiwan == (WORD)m_hKeyLayout ||
		LanguageCode_Chinese_Prc == (WORD)m_hKeyLayout )
	{
		return TRUE;
	}

	// Property가 IME_PROP_SPECIAL_UI가 아니고, IME_PROP_AT_CARET 여야...
	// 즉, 일반적인 UI 타입이고, conversion window는 캐럿에 존재한다.

	return (!(m_property & IME_PROP_SPECIAL_UI) && (m_property & IME_PROP_AT_CARET));
}

BOOL CIMEEdit::CheckAttrInput(char* attr, int iLen)
{
	for(int i = 0; i < iLen; i++)
	{
		if(*(attr+i) != ATTR_INPUT) return FALSE;
	}

	return TRUE;
}

void CIMEEdit::SetCompClauses(char* strComp, char* szAttr, LPDWORD pClause, int iClauseCount)
{
	char szClause[MAX_CLAUSE_TEXT] = {0,};
	char* pClauseStr = NULL;
	int nLen = 0;

	for(int i = 0; i < iClauseCount; i++)
	{
		if(i == (iClauseCount - 1))
		{
			pClauseStr = strComp + pClause[i];
			lstrcpy(szClause, pClauseStr);
		}
		else
		{
			nLen = pClause[i+1] - pClause[i];
			pClauseStr = strComp + pClause[i];
			
			SAFEMEMCPY(szClause, pClauseStr, nLen);
			
			szClause[nLen] = 0;	
		}

		m_TextWithClause.AppendClause(szClause, szAttr[pClause[i]]);
	}
}

BOOL CIMEEdit::GetCompString(LONG flag)
{
    DWORD		dwLen = 0;       
	DWORD		dwLenAttr = 0;
	DWORD		dwLenClause = 0;
	BOOL		bNotConverted = TRUE;

    char szString[MAX_CLAUSE_TEXT] = {0,};  
    char szAttr[MAX_CLAUSE_TEXT] = {0,};     
	DWORD pClause[MAX_CLAUSE] = {0,}; 

	if(!AcquireIME()) return FALSE;

	m_TextWithClause.ClearComposition();

    if((dwLen = ImmGetCompositionString(m_hIMC, GCS_COMPSTR, NULL, 0)) > 0) 
	{
#ifdef DEVELOP_MODE
		if(dwLen > MAX_CLAUSE_TEXT) _asm int 3;
#endif
		ImmGetCompositionString(m_hIMC, GCS_COMPSTR, szString, dwLen);
		szString[dwLen] = 0;
		
		// attr!!
		if(flag & GCS_COMPATTR) 
		{
			if((dwLenAttr = ImmGetCompositionString(m_hIMC, GCS_COMPATTR, NULL, 0)) > 0) 
			{
#ifdef DEVELOP_MODE
				if(dwLenAttr > MAX_CLAUSE_TEXT) _asm int 3;
#endif
				ImmGetCompositionString(m_hIMC, GCS_COMPATTR, szAttr, dwLenAttr);
				szAttr[dwLenAttr] = 0;

				bNotConverted = CheckAttrInput(szAttr, dwLenAttr);
			}
		} 

		// Length By Byte.
		DWORD dwMaxCompStrLen = m_nMaxTextLen - m_TextWithClause.GetResultLen();

		// 일본어 변환을 위한 여유분을 준다!
		if(LanguageCode_Japanese == (WORD)m_hKeyLayout)
		{
			if(dwMaxCompStrLen > 1) dwMaxCompStrLen += 10; 
		}

		if(bNotConverted && (dwLen > dwMaxCompStrLen))
		{
			szString[dwMaxCompStrLen] = 0;

			// Composition string을 초기화시킨다.
			ImmNotifyIME(m_hIMC, NI_COMPOSITIONSTR, CPS_CANCEL, 0);

			// 붙일 Composition string이 존재하면 붙인다.
			if(dwMaxCompStrLen > 0)
			{
				ImmSetCompositionString(m_hIMC, SCS_SETSTR, szString, dwMaxCompStrLen, NULL, 0);	
			}
			
			if(m_dwOption & IMEOPTION_ERRTEXTOUT) 
				DisplayMessageAdd(g_Message[ETC_MESSAGE790].szMessage, 0xffff0000);

			OutputDebugString("IME: Reached MaxTextLen");
		}
		else
		{
			// clause!!
			if(flag & GCS_COMPCLAUSE) 
			{
				if((dwLenClause = ImmGetCompositionString(m_hIMC, GCS_COMPCLAUSE, NULL, 0)) > 0)
				{
#ifdef DEVELOP_MODE
					if(dwLenClause > MAX_CLAUSE_TEXT) _asm int 3;
#endif
					ImmGetCompositionString(m_hIMC, GCS_COMPCLAUSE, pClause, dwLenClause);
					
					
					if (!IsChineseLanguage())	// modify [8/16/2004]
					{
						SetCompClauses(szString, szAttr, pClause, (dwLenClause/sizeof(DWORD)));
					}
				}
			}
			else // 한국어의 경우 clause 정보 없다! 모두 하나의 clause로 볼 수 있다.
			{
				m_TextWithClause.AppendClause(szString, szAttr[0]);
			}
		}		
	}	

	ReleaseIME();

	return TRUE;
}

BOOL CIMEEdit::GetResultString()
{
	char szResult[MAX_RESULT*EXTRA_STRING_SIZE] = {0,};
	DWORD dwLen = 0; 

 	if(AcquireIME()) 
	{
		if((dwLen = ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, NULL, 0)) > 0) 
		{
			if(dwLen > MAX_RESULT*EXTRA_STRING_SIZE) 
			{
				MessageBox(	NULL, "Too Long Result String!!", "GetResultString()", MB_ICONERROR|MB_OK);
				_asm int 3;
			}

			ImmGetCompositionString(m_hIMC, GCS_RESULTSTR, szResult, dwLen);

			if(dwLen > MAX_RESULT) szResult[MAX_RESULT-1] = 0;
			else 	szResult[dwLen] = 0;
		}

		if((dwLen+m_TextWithClause.GetResultLen()) >= DWORD(m_nMaxTextLen)) 
		{
			if(m_dwOption & IMEOPTION_ERRTEXTOUT) 
				DisplayMessageAdd(g_Message[ETC_MESSAGE790].szMessage, 0xffff0000);

			OutputDebugString("IME: Reached MaxTextLen");
		}
		else
		{
			if (!IsChineseLanguage())	// modify [8/16/2004]
			{
				// 지능 ABC는 이거하면 결과값이 2번 나온다.
				m_TextWithClause.AppendResultString(szResult, dwLen);
			}
		}

		ReleaseIME();
	}

	return TRUE;
}

inline void CIMEEdit::NotifyIMESpriteChange()
{
	PostMessage(m_hWnd, WM_IME_SPRITECHANGE, 0, 0);
}

inline void CIMEEdit::OnNotifyIMESpriteChange()
{
	DrawToTextSprite();
	DrawToCandidateSprite();
}

/////////////////////////////////////////////////////////////////////////////
// CIMEEdit message handlers

BOOL CIMEEdit::ProcessIMEMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(!m_hWnd) return FALSE;
	
	//--------------
	// test code
//	{ 
//		char szText[0xff]={0,};
//		wsprintf(szText, "message : %04X, wParam : %04X, lParam : %04X\n", msg, wparam, lparam);
//		OutputDebugString(szText);
//	} 
	//--------------

	switch(msg) 
	{ 
	case WM_INPUTLANGCHANGE:
		OnInputLangChange(wparam,lparam);
		NotifyIMESpriteChange();
		return FALSE;

	case WM_IME_STARTCOMPOSITION:			// 글자 조합의 시작. WM_IME_COMPOSITION 메시지를 받을 준비를 한다. 
		if(IsActivated())
		{	
			OnIMEStartComposition(wparam,lparam);
			NotifyIMESpriteChange();

			if (IsChineseLanguage() || IsTaiwanLanguage())
			{ 
				// 해당 IME 에게 메시지를 처리 할 수 있도록 FALSE 를 리턴한다. 
				return FALSE;
			}
		}
	
		return TRUE;  
		
	case WM_IME_ENDCOMPOSITION:				// 글자 조합의 끝. 조합된 문자열 처리를 끝낸다 		
		if(IsActivated())
		{
			OnIMEEndComposition(wparam,lparam);
			NotifyIMESpriteChange();
			if (IsChineseLanguage() || IsTaiwanLanguage())
			{ 
				// 해당 IME 에게 메시지를 처리 할 수 있도록 FALSE 를 리턴한다. 
				return FALSE;
			}
		}
		
		return TRUE;   

	case WM_IME_COMPOSITION:				// 현재 조합중인 문자열이나 완성된 문자열을 얻어서 화면 출력 
		if(IsActivated())
		{	
			OnIMEComposition(wparam,lparam);
			NotifyIMESpriteChange();
			
			if (IsChineseLanguage())
			{ 
				return FALSE;
			}	
		}

		return TRUE;

	case WM_IME_SETCONTEXT:					// 글자 조합 윈도우와 한자 변환 윈도우를 표시하지 않게 바꿈
		OnIMESetContext(wparam,lparam);
		return TRUE;    

	case WM_IME_NOTIFY:						// 기본 캔디데이트 윈도우 사용 시 처리 안함.
		if(!IsActivated()) return TRUE;		// 비활성화시 무시하도록!
		return OnIMENotify(wparam,lparam);

	case WM_IME_COMPOSITIONFULL:
		OnIMECompositionFull(wparam,lparam);
		return TRUE;   
		
	case WM_TIMER:
		if(IDT_IME != wparam) return FALSE;
		m_bDrawCursor = !m_bDrawCursor;
		NotifyIMESpriteChange();
		return TRUE;

	// seven-start
	case WM_CLOSE:		
		PostMessage(m_hWnd, WM_KILLFOCUS, 0, 0);	 // 이부분 안넣어주고 입력중에 종료하면 문제 발생.	
		return FALSE;

	case WM_SETFOCUS: // 캐럿은 포커스와 일생을 같이한다.
		{ 
			// Set Caret Position 
			CreateCaret(m_hWnd, NULL, 8, 8);	// 케럿은 보일 필요가 없기때문에 문자 너비, 높이를 임의로 정했다.
			HideCaret(m_hWnd);
		} 
		return TRUE;

	case WM_KILLFOCUS:
		DestroyCaret();
		return TRUE;
	// seven-end

	case WM_CHAR:
		{
			if(!IsActivated()) return FALSE;	// activated 아닌 상태에선 ime에서 처리 안함!

			BOOL bRet = OnChar(wparam);
			NotifyIMESpriteChange();
			return bRet;
			break;
		}
	case WM_KEYDOWN:
		{
			if(!IsActivated()) return FALSE; // activated 아닌 상태에선 ime에서 처리 안함!				

			BOOL bRet =	OnKeyDown(wparam);
			if(bRet) NotifyIMESpriteChange();
			return bRet;
			break;
		}
	case WM_IME_SPRITECHANGE:
		if(IsActivated()) OnNotifyIMESpriteChange();
		return TRUE;
    }

	return FALSE;
}

BOOL CIMEEdit::OnKeyDown(WPARAM nChar)
{
	switch(nChar)
	{
	case VK_DELETE:	m_TextWithClause.OnKeyDownDelete();	break;
	case VK_BACK:	m_TextWithClause.OnKeyDownBack();	break;
#ifndef KOREA_MODE
	case VK_LEFT:	m_TextWithClause.OnKeyDownLeft();	break;
	case VK_RIGHT:	m_TextWithClause.OnKeyDownRight();	break;
#endif
	case VK_HOME:	m_TextWithClause.OnKeyDownHome();	break;
	case VK_END:	m_TextWithClause.OnKeyDownEnd();	break;
	case VK_SHIFT:
		{
			if (Ime_Input_Identity_Chinese_Intelligence_ABC == (DWORD)m_hKeyLayout)
			{ // 지능 ABC 에서 영/중 변환시 문제가 되서 따로 처리한다.
				m_dwConversionMode = !m_dwConversionMode;
				ImmSetConversionStatus(m_hIMC, m_dwConversionMode, m_dwSentenceMode);
			}
			else
			{
				return FALSE;
			}
		}
		break;

	default: return FALSE;
	}

	return TRUE;
}

BOOL CIMEEdit::OnChar(UINT nChar)
{
	if(m_bInputBlock) return FALSE;

	// prevent ctrl key!
	SHORT nState = GetKeyState(VK_CONTROL);
	if(nState & 0x8000) return FALSE;

	// don't process backspace, esc, tab, return key!
	if(nChar == VK_BACK)	return FALSE;
	if(nChar == VK_ESCAPE)	return FALSE;
	if(nChar == VK_TAB)		return FALSE;
	if(nChar == VK_RETURN)	return FALSE;

	if(IsGuildCreate() && (nChar == VK_SPACE))	// Guild 생성중에 공백문자.
		return FALSE;
	
	// check space key enabled..
	if((nChar == VK_SPACE) && (m_dwOption & IMEOPTION_SPACEENABLED == FALSE)) 
		return TRUE;

	char str[2];
	str[0] = nChar & 0xff;
	str[1] = 0;

	int iLen = lstrlen(str); 

#ifdef DEVELOP_MODE
	if(m_nMaxTextLen < 0) _asm { int 3 }
#endif

	if((iLen+m_TextWithClause.GetResultLen()) >= DWORD(m_nMaxTextLen)) 
	{
		if(m_dwOption & IMEOPTION_ERRTEXTOUT) 
			DisplayMessageAdd(g_Message[ETC_MESSAGE790].szMessage, 0xffff0000);

		OutputDebugString("IME: Reached MaxTextLen");
	}
	else
	{
		m_TextWithClause.AppendResultString(str, iLen);
	}	
	
	return TRUE;
}

LONG CIMEEdit::OnInputLangChange(WPARAM dwCommand, LPARAM dwData)
{
	if ((WORD)m_hKeyLayout != (WORD)dwData)
	{ // IME 언어를 변경 했을때만 Text를 초기화 한다.
		m_TextWithClause.Clear();
	}

	m_TextWithClause.ClearComposition();

	m_hKeyLayout = (HKL)dwData;
	m_iTextCharset = dwCommand;

	SetEditFonts(-12, DEFAULT_CHARSET);

	switch((WORD)m_hKeyLayout)
	{
	case LanguageCode_Japanese:
		m_dwConversionMode	= IME_CMODE_FULLSHAPE|IME_CMODE_ROMAN|IME_CMODE_NATIVE;
		m_dwSentenceMode	= IME_SMODE_PHRASEPREDICT;
		break;	
	case LanguageCode_Korean:
		m_dwConversionMode	= 0;
		m_dwSentenceMode	= 0;
		break;
	case LanguageCode_Chinese_Prc:
	case LanguageCode_Chinese_Taiwan:
	default:
		m_dwConversionMode	= 0;
		m_dwSentenceMode	= 0;
	}

	SetNativeLanguageMode(FALSE);

	return FALSE;
}


LONG CIMEEdit::OnIMESetContext(WPARAM dwCommand, LPARAM dwData)
{
	dwData &= ~ISC_SHOWUIALL;

	return DefWindowProc(m_hWnd, WM_IME_SETCONTEXT, dwCommand, dwData);	
}

LONG CIMEEdit::OnIMEStartComposition(WPARAM dwCommand, LPARAM dwData)
{
	char szText[256] = {0,};
	COMPOSITIONFORM CompForm;
	POINT			pt = GetEditPos();
	
	CompForm.dwStyle = CFS_POINT;

	HDC dc = GetDC(m_hWnd);

 	HFONT hOldFont = (HFONT)SelectObject(dc, m_hFont);


	SIZE size;

	if (m_TextWithClause.GetCursor())
	{
		lstrcpy(szText, m_TextWithClause.GetResultString());
		GetTextExtentPoint32(dc, szText, m_TextWithClause.GetCursor(), &size);
	}
	else
	{
		GetTextExtentPoint32(dc, "|", 1, &size);
	}

	ReleaseDC(m_hWnd, dc);

	pt.x = pt.x + size.cx-28;
	pt.y = pt.y - (size.cy);
	CompForm.ptCurrentPos = pt;

	m_CandidateSpritePos.x = (float)pt.x;
	m_CandidateSpritePos.y = (float)pt.y - size.cy*2;

	ImmSetCompositionWindow(m_hIMC, &CompForm);

	SelectObject(dc, hOldFont);		

	return false;
}

LONG CIMEEdit::OnIMEComposition(WPARAM dwCommand, LPARAM dwData)
{
	if(CheckIMEProperty()) 
	{ 
		// !IME_PROP_SPECIAL_UI && IME_PROP_AT_CARET
		if(dwData & GCS_RESULTSTR)
		{
			GetResultString();				// Result String
		}
		else if(dwData & GCS_COMPSTR)
		{
			GetCompString(dwData);			// Composition string
		}
	}

	return 0;
}

LONG CIMEEdit::OnIMEEndComposition(WPARAM dwCommand, LPARAM dwData)
{
	if(CheckIMEProperty()) 
	{ 
		m_TextWithClause.ClearComposition();
		return TRUE;
	} 

	return FALSE;
}

LONG CIMEEdit::OnIMECompositionFull(WPARAM dwCommand, LPARAM dwData)
{
	// Make sure the size for drawing the composition string.
	// Application should draw the composition string correctly.
	return 0;
}

LONG CIMEEdit::OnIMEControl(WPARAM dwCommand, LPARAM dwData)
{
	// This message is not received by the application window.
	// But don't pass it to DefWindowProc().
	return 0;
}

LONG CIMEEdit::OnIMENotify(WPARAM dwCommand, LPARAM dwData)
{
	// This class does not handle all notification message.
	// So we pass those notification messages which are not hanlded
	// by this application to the DefWindowProc.

	// 중국과 대만 버전은 윈도우에서 제공하는 캔디데이터를 사용한다. 
	if (IsChineseLanguage() || IsTaiwanLanguage()) 
	{
//		if( What_Kind_Of_Platform_Win32_Window == WhatKindOfPlatform() ) 
//		{
//			return FALSE;
//		}
//		else if (Ime_Input_Identity_Chinese_Intelligence_ABC == (DWORD)m_hKeyLayout)
//		{
//			return FALSE;
//		}

		return FALSE;
	}



    switch(dwCommand) 
	{
    case IMN_OPENCANDIDATE:
		return OnNotifyOpenCandidate(dwData);

    case IMN_CLOSECANDIDATE:
		return OnNotifyCloseCandidate(dwData);

    case IMN_CHANGECANDIDATE:
		return OnNotifyChangeCandidate(dwData);

 //   case IMN_SETOPENSTATUS:
//		return OnNotifySetOpenStatus();
    }
	
    return FALSE;
}

BOOL CIMEEdit::GetCandidateList(LONG lParam)
{
    if(AcquireIME())
	{
		// 기존의 캔디데이트에 대한 자료가 남아 있다면 제거한다
		if(m_pCandidateLists) delete [] m_pCandidateLists;

		// 몇번째의 캔디데이트인지 확인한다.
		for(DWORD dwIndex = 0; dwIndex < MAX_CANDIDATE_LIST; dwIndex++) 
		{
			if(lParam & (1 << dwIndex)) break;
		}

 		// candidate list의 size를 알아온다.
		DWORD dwBufLen = ImmGetCandidateList(m_hIMC, dwIndex, m_pCandidateLists, 0);

		// candidate list를 저장할 버퍼를 생성한다.
		m_pCandidateLists = (LPCANDIDATELIST) new char[dwBufLen];
		if(!m_pCandidateLists) _asm int 3;
		
		// candidate list 값을 받아온다.
		ImmGetCandidateList(m_hIMC, dwIndex, m_pCandidateLists, dwBufLen);

		// candidate를 dc에 그려, 텍스쳐에 복사한다.
		DrawToCandidateSprite();

		// candidate window가 있음을 세팅!
		m_bCandidateSet = TRUE;

		ReleaseIME();

		return TRUE;
	}

	return FALSE;
}

// IMN_OPENCANDIDATE:
BOOL CIMEEdit::OnNotifyOpenCandidate(LONG lParam)
{
	return GetCandidateList(lParam);
}

// IMN_CLOSECANDIDATE:
BOOL CIMEEdit::OnNotifyCloseCandidate(LONG lParam)
{
	// candidate window가 없음을 세팅!
	m_bCandidateSet = FALSE;

	return TRUE;
}

// IMN_CHANGECANDIDATE:
BOOL CIMEEdit::OnNotifyChangeCandidate(LONG lParam)
{
	return GetCandidateList(lParam);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// for composition string out.

BOOL CIMEEdit::RenderSprite(int zOrder)
{
	if(m_pIExecutive == NULL) return FALSE;
	if(m_pTextSprite == NULL) return FALSE;
	if(!IsActivated()) return FALSE;

	if(zOrder == __ORDER_USERINTERFACE_START_)
		zOrder = m_zOrder;

	m_pIExecutive->GetGeometry()->RenderSprite(	m_pTextSprite, 
												0, 
												NULL, 
												&m_TextSpritePos, 
												&m_TextSpriteRect,
												0xffffffff,
												zOrder,
												RENDER_TYPE_DISABLE_TEX_FILTERING|RENDER_TYPE_SPRITE_OPASITY);

	if(m_bCandidateSet)
	{
		m_pIExecutive->GetGeometry()->RenderSprite(	m_pCandidateSprite, 
													0, 
													NULL, 
													&m_CandidateSpritePos, 
													&m_CandidateSpriteRect,
													0xffffffff,
													66666,
													RENDER_TYPE_DISABLE_TEX_FILTERING|RENDER_TYPE_SPRITE_OPASITY);
	}

	return TRUE;
}

void CIMEEdit::DeleteSprite()
{
	if(m_hFont)					{DeleteObject(m_hFont);					m_hFont		= NULL;}
	if(m_hThinPen)				{DeleteObject(m_hThinPen);				m_hThinPen	= NULL;}
	if(m_hThickPen)				{DeleteObject(m_hThickPen);				m_hThickPen = NULL;}
	
	if(m_lpTextBits)			{delete [] m_lpTextBits;				m_lpTextBits	= NULL;}
	if(m_pbiText)				{delete [] m_pbiText;					m_pbiText		= NULL;}
	if(m_hTextBitmap)			{DeleteObject(m_hTextBitmap);			m_hTextBitmap	= NULL;}
	if(m_hTextMemDC)			{DeleteDC(m_hTextMemDC);				m_hTextMemDC	= NULL;}
	if(m_pTextSprite)			{m_pTextSprite->Release();				m_pTextSprite	= NULL;}

	if(m_lpCandidateBits)		{delete [] m_lpCandidateBits;			m_lpCandidateBits	= NULL;}
	if(m_pbiCandidate)			{delete [] m_pbiCandidate;				m_pbiCandidate		= NULL;}
	if(m_hCandidateBitmap)		{DeleteObject(m_hCandidateBitmap);		m_hCandidateBitmap	= NULL;}
	if(m_hCandidateMemDC)		{DeleteDC(m_hCandidateMemDC);			m_hCandidateMemDC	= NULL;}
	if(m_pCandidateSprite)		{m_pCandidateSprite->Release();			m_pCandidateSprite  = NULL;}
}

void CIMEEdit::CreateSprite(HDC dc, int nTextWidth, int nTextHeight, int nCandWidth, int nCandHeight)
{
	// FONT 및 PEN 생성.
	SetEditFonts(-12, DEFAULT_CHARSET);

	m_hThinPen = CreatePen(PS_COSMETIC | PS_SOLID, 1, LINE_COLOR); 
	m_hThickPen = CreatePen(PS_COSMETIC | PS_SOLID, 2, LINE_COLOR); 

	// 출력 스프라이트 영역 지정!
	m_TextSpriteRect.left	= 0;
	m_TextSpriteRect.right	= nTextWidth;
	m_TextSpriteRect.top	= 0;
	m_TextSpriteRect.bottom	= nTextHeight;

	m_CandidateSpriteRect.left	= 0;
	m_CandidateSpriteRect.right	= nCandWidth;
	m_CandidateSpriteRect.top	= 0;
	m_CandidateSpriteRect.bottom= nCandHeight;	
	
	// 메모리 DC 생성.
	m_hTextMemDC = CreateCompatibleDC(dc);
	m_hCandidateMemDC = CreateCompatibleDC(dc);

	// DC로부터 일정 크기의 비트맵 생성.
	m_hTextBitmap = CreateCompatibleBitmap(dc, nTextWidth, nTextHeight);
	m_hCandidateBitmap = CreateCompatibleBitmap(dc, nCandWidth, nCandHeight);

	if(m_hTextBitmap == NULL || m_hCandidateBitmap == NULL)
	{
		MessageBox(	NULL,
					"Compatible Bitmap Creation Failed!",
					"Bitmap Error",
					MB_ICONERROR|MB_OK);

		_asm { int 3 }		
	}

	// 메모리 DC에 비트맵 선택.
	SelectObject(m_hTextMemDC, m_hTextBitmap);
	SelectObject(m_hCandidateMemDC, m_hCandidateBitmap);

	// 비트맵 정보 구조체를 생성한다.
	m_pbiText = __CreateBitmapInfoStructFor16Bit(m_hTextBitmap);
	m_pbiCandidate = __CreateBitmapInfoStructFor16Bit(m_hCandidateBitmap);
	
	m_lpTextBits = new BYTE [m_pbiText->bmiHeader.biSizeImage];
    if(!m_lpTextBits) 
	{
		MessageBox(	NULL,
					"Text Image Buffer Allocation Failed!",
					"Memory Failure", 
					MB_ICONERROR|MB_OK);

		_asm { int 3 }
	}

	m_lpCandidateBits = new BYTE [m_pbiCandidate->bmiHeader.biSizeImage];
    if(!m_lpCandidateBits) 
	{
		MessageBox(	NULL,
					"Candidate Image Buffer Allocation Failed!",
					"Memory Failure", 
					MB_ICONERROR|MB_OK);

		_asm { int 3 }
	}

	////////////////////////////////////////////////////////////////
	if(m_pIExecutive == NULL) _asm int 3; 
	I4DyuchiGXRenderer* pRenderer =	m_pIExecutive->GetRenderer();

	m_pTextSprite = pRenderer->CreateEmptySpriteObject(nTextWidth, nTextHeight, TEXTURE_FORMAT_A1R5G5B5, 0);
	m_pCandidateSprite = pRenderer->CreateEmptySpriteObject(nCandWidth, nCandHeight, TEXTURE_FORMAT_A1R5G5B5, 0);
}

void CIMEEdit::DrawToTextSprite()
{
	// mem DC에 일단 그린다.
	DrawTextSprite(m_hTextMemDC, m_TextSpriteRect);

	// 컬러 정보를 얻어온다!!
    if(!GetDIBits(m_hTextMemDC, m_hTextBitmap, 0, 
		(WORD) -m_pbiText->bmiHeader.biHeight, m_lpTextBits, m_pbiText, DIB_RGB_COLORS)) return;

	LOCKED_RECT LockedRect;
	LockedRect.Pitch = 0;
	LockedRect.pBits = 0;

	BOOL bLock = m_pTextSprite->LockRect(&LockedRect, &m_TextSpriteRect, TEXTURE_FORMAT_A1R5G5B5);
	if(bLock == FALSE) _asm int 3;

	__SetLockedRectWithBitmapData((LPWORD)m_lpTextBits, LockedRect, 
		m_TextSpriteRect.right - m_TextSpriteRect.left, 
		m_TextSpriteRect.bottom - m_TextSpriteRect.top);

	m_pTextSprite->UnlockRect();
}

void CIMEEdit::DrawToCandidateSprite()
{
	// 세팅된 캔디테이트가 없으면 나가버린다.
	if(!m_bCandidateSet) return ;

	// mem DC에 일단 그린다.
	DrawCandidateSprite(m_hCandidateMemDC, m_CandidateSpriteRect);

	// 컬러 정보를 얻어온다!!
    if(!GetDIBits(m_hCandidateMemDC, m_hCandidateBitmap, 0, 
		(WORD) -m_pbiCandidate->bmiHeader.biHeight, m_lpCandidateBits, m_pbiCandidate, DIB_RGB_COLORS)) return;

	LOCKED_RECT LockedRect;
	LockedRect.Pitch = 0;
	LockedRect.pBits = 0;

	BOOL bLock = m_pCandidateSprite->LockRect(&LockedRect, &m_CandidateSpriteRect, TEXTURE_FORMAT_A1R5G5B5);
	if(bLock == FALSE) _asm int 3;

	__SetLockedRectWithBitmapData((LPWORD)m_lpCandidateBits, LockedRect, 
		m_CandidateSpriteRect.right - m_CandidateSpriteRect.left, 
		m_CandidateSpriteRect.bottom - m_CandidateSpriteRect.top);

	m_pCandidateSprite->UnlockRect();
}

void CIMEEdit::DrawCompositionText(HDC dc, RECT rc, LPCSTR szText, COLORREF text, COLORREF back, int Attr)
{
	DWORD dwBackColor = SetBkColor(dc, back);
	ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, 0);
	SetBkMode(dc, TRANSPARENT);

	DWORD dwTxtColor = SetTextColor(dc, text);

	// comp str text draw!
	DrawText(dc, szText, lstrlen(szText), &rc, DT_LEFT|DT_NOPREFIX);

	if(LanguageCode_Korean == (WORD)m_hKeyLayout)
	{
		SIZE size;	
		GetTextExtentPoint32(dc, "9| ", lstrlen("9| "), &size);
		size.cx += 2;

		m_CandidateSpritePos.x = m_TextSpritePos.x + float(rc.left);
		if(m_CandidateSpritePos.x > size.cx)
			m_CandidateSpritePos.x -= size.cx;
	}

	// clause indicator underline draw!
	if(Attr > CLAUSE_NOLINE)
	{
		HPEN hPen = (Attr == CLAUSE_THICKLINE) ? m_hThickPen : m_hThinPen;
		HPEN hPenOld = (HPEN)SelectObject(dc, hPen); 

		// CandidateWindow 좌표 세팅을 위해,
		// 현재 위치한 Clause의 Upper-Left x좌표를 기억시킨다.
		if(Attr == CLAUSE_THICKLINE)
		{
			SIZE size;	
			GetTextExtentPoint32(dc, "9| ", lstrlen("9| "), &size);
			size.cx += 2;

			m_CandidateSpritePos.x = m_TextSpritePos.x + float(rc.left);
			if(m_CandidateSpritePos.x > size.cx)
				m_CandidateSpritePos.x -= size.cx;
		}

		POINT OldPt;
		MoveToEx(dc, rc.left+2, rc.bottom + 1, &OldPt);
		LineTo(dc, rc.right-2, rc.bottom + 1);

		SelectObject(dc, hPenOld); 
	}

	SetTextColor(dc, dwTxtColor);
	SetBkColor(dc, dwBackColor);
}

void CIMEEdit::DrawTextSprite(HDC dc, RECT rc)
{
	SIZE size;
	char szTotalText[MAX_RESULT] = {0,};
	char szClause[MAX_CLAUSE_TEXT] = {0,};
	char szTemp[MAX_RESULT] = {0,};
	char szPassword[MAX_RESULT] = {0,};

	// 일단 폰트를 설정한다.
	HFONT hOldFont = (HFONT)SelectObject(dc, m_hFont);

	// 배경색을 설정한다.
	DWORD dwBackColor = SetBkColor(dc, BACK_COLOR);
	ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, 0);
	SetBkMode(dc, TRANSPARENT);

	// Result Text의 색을 설정한다.
	SetTextColor(dc, TEXT_COLOR);

	// 패스워드 모드인 경우, "****"로 변경하여 화면에 출력한다.
	if(m_dwOption & IMEOPTION_PASSWORD)
	{
		// Result String 출력!
		if(lstrlen(m_TextWithClause.GetResultString()))
		{
			SAFEMEMSET(szPassword, '*', m_TextWithClause.GetResultLen());
			szPassword[m_TextWithClause.GetResultLen()] = 0;

			DrawText(dc, szPassword, lstrlen(szPassword), &rc, DT_LEFT|DT_NOPREFIX);

			// get extent point of result string
			GetTextExtentPoint32(dc, szPassword, lstrlen(szPassword), &size);

			// 현재 캐럿 위치까지의 문자열의 Extent를 구해서 rc를 변경한다.
			if(m_TextWithClause.GetCursor())
			{	
				int iLeftLen = GetLeftString(szTemp);
				SAFEMEMSET(szTemp, '*', iLeftLen);
				GetTextExtentPoint32(dc, szTemp, iLeftLen, &size);

				rc.left += size.cx;
			}
		}

		// 현재 캐럿 위치에 composition 중인 텍스트 출력!
		if(m_TextWithClause.GetClauseCount())
		{
			for(int i = 0; i < m_TextWithClause.GetClauseCount(); i++)
			{
				// clause와 해당 clause의 영역을 얻어온다.
				char Attr = m_TextWithClause.GetClauseString(i, szClause);
				SAFEMEMSET(szClause, '*', lstrlen(szClause));
				GetTextExtentPoint32(dc, szClause, lstrlen(szClause), &size);
				
				RECT rcComp;
				rcComp.left		= rc.left;
				rcComp.right	= rc.left + size.cx;
				rcComp.top		= rc.top;
				rcComp.bottom	= rc.top + size.cy;

				// seven-start
				// 캐럿위치를 셋팅 
				if(size.cy != 0) // 아무것도 없이 2번 틀어 와서리. 
				{
					// MingLiu 에 관련하여 위치를 세팅함.
					// 일단 다른 폰트에 관해 테스트 해본결과 높이 값의 변동사항이 있음.

					int x, y;
					x = y = 0;
					
					if ((WORD)m_hKeyLayout == LanguageCode_Chinese_Taiwan)
					{
						x = int(m_TextSpritePos.x + rcComp.left - 8);
						y = int(m_TextSpritePos.y - (size.cy*5-1));
					}
					else
					{
						x = int(m_TextSpritePos.x + rcComp.left - 8);
						y = int(m_TextSpritePos.y - (size.cy*2));
					}

					SetCaretPos(x, y);
				}
				// seven-end

				// clause 출력!
				DrawCompositionText(dc, rcComp, szClause, TEXT_COLOR, COMP_COLOR, Attr);

				// 다음 clause를 위한 영역 재설정!
				rc.left += size.cx;
			}
		}	

		// Caret 출력!!!!
		if(m_bDrawCursor)
		{
			SetTextColor(dc, CURSOR_COLOR);
			rc.left -= 2;
			DrawText(dc, "|", lstrlen("|"), &rc, DT_LEFT|DT_NOPREFIX);
		}
	}
	else
	{
		// Result String 출력!
		if(lstrlen(m_TextWithClause.GetResultString()))
		{
			// draw result string
			DrawText(	dc, 
						m_TextWithClause.GetResultString(), 
						lstrlen(m_TextWithClause.GetResultString()), 
						&rc, 
						DT_LEFT|DT_NOPREFIX);
			
			// get extent point of result string
			GetTextExtentPoint32(dc,
						m_TextWithClause.GetResultString(), 
						lstrlen(m_TextWithClause.GetResultString()), 
						&size);

			// 현재 캐럿 위치까지의 문자열의 Extent를 구해서 rc를 변경한다.
			if(m_TextWithClause.GetCursor())
			{	
				int iLeftLen = GetLeftString(szTemp);
				GetTextExtentPoint32(dc, szTemp, iLeftLen, &size);

				rc.left += size.cx;
			}
		}

		// 현재 캐럿 위치에 composition 중인 텍스트 출력!
		if(m_TextWithClause.GetClauseCount())
		{
			for(int i = 0; i < m_TextWithClause.GetClauseCount(); i++)
			{
				// clause와 해당 clause의 영역을 얻어온다.
				char Attr = m_TextWithClause.GetClauseString(i, szClause);
				GetTextExtentPoint32(dc, szClause, lstrlen(szClause), &size);
				
				RECT rcComp;
				rcComp.left		= rc.left;
				rcComp.right	= rc.left + size.cx;
				rcComp.top		= rc.top;
				rcComp.bottom	= rc.top + size.cy;
				
				// seven-start
				// 캐럿위치를 셋팅 
				if (size.cy != 0) // 아무것도 없이 2번 틀어 와서리. 
				{
					// MingLiu 에 관련하여 위치를 세팅함.
					// 일단 다른 폰트에 관해 테스트 해본결과 높이 값의 변동사항이 있음.

					int x, y;		x = y = 0;

					if ((WORD)m_hKeyLayout == IsTaiwanLanguage())
					{
						x = int(m_TextSpritePos.x + rcComp.left - 8);
						y = int(m_TextSpritePos.y - (size.cy*6-1));
					}
					else
					{
						x = int(m_TextSpritePos.x + rcComp.left - 8);
						y = int(m_TextSpritePos.y - (size.cy*2));
					}

					SetCaretPos(x, y);
				}
				// seven-end

				// clause 출력!
				DrawCompositionText(dc, rcComp, szClause, TEXT_COLOR, COMP_COLOR, Attr);

				// 다음 clause를 위한 영역 재설정!
				rc.left += size.cx;
			}
		}	

		// Caret 출력!!!!
		if(m_bDrawCursor)
		{
			SetTextColor(dc, CURSOR_COLOR);
			rc.left -= 2;
			DrawText(dc, "|", lstrlen("|"), &rc, DT_LEFT|DT_NOPREFIX);
		}
	}

	SelectObject(dc, hOldFont);
}

// dc에 candidate window를 그린다. 
// 더불어, candidate의 높이를 구해, y 좌표를 세팅한다.
void CIMEEdit::DrawCandidateSprite(HDC dc, RECT rc)
{
	char szBuf[MAX_CLAUSE_TEXT] = {0,};

	// 일단 폰트를 설정한다.
	HFONT hOldFont = (HFONT)SelectObject(dc, m_hFont);

	// 현재 리스트의 한 페이지 당 캔디데이트수 구하기.
	DWORD dwNumPerPage = (m_pCandidateLists->dwPageSize) ? 
						m_pCandidateLists->dwPageSize : DEFAULT_CAND_NUM_PER_PAGE;

	// 시작 인덱스 구하기
	DWORD dwStartIndex = (m_pCandidateLists->dwSelection == 0) ? 
		0 : dwNumPerPage * ((m_pCandidateLists->dwSelection) / dwNumPerPage);

	// 마지막 인덱스 구하기.
	DWORD dwEndIndex = dwStartIndex + dwNumPerPage;
	dwEndIndex = (dwEndIndex >= m_pCandidateLists->dwCount) ?
				 m_pCandidateLists->dwCount : dwEndIndex;

	// get height of font
	SIZE size;	
	GetTextExtentPoint32(dc, "A", lstrlen("A"), &size);

	int iHeight = size.cy+1;
	int iWidth = 0;

	// get max width of candidates.
	for(DWORD dwIndex = dwStartIndex; dwIndex < dwEndIndex; dwIndex++)
	{
		LPSTR lpStr = (LPSTR) m_pCandidateLists + m_pCandidateLists->dwOffset[dwIndex];

		if (__strcmp(lpStr, "") != 0)
			GetTextExtentPoint32(dc, lpStr, lstrlen(lpStr), &size);
		iWidth = __max(iWidth, size.cx);		
	}
		
	// clear background.
	DWORD dwBackColor = SetBkColor(dc, BACK_COLOR);
	ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rc, NULL, 0, 0);
	SetBkMode(dc, TRANSPARENT);

	RECT Realrc; // 실제 그림이 그려질 영역!! 우리는 이 영역을 미리 계산할 수 있다.

	Realrc.left		= 0;
	Realrc.right	= iWidth+50;
	Realrc.top		= 0;
	Realrc.bottom	= (dwNumPerPage + 1) * iHeight;

	// Candidate Window Y Position Set!
	m_CandidateSpritePos.y = m_TextSpritePos.y - Realrc.bottom - 1;
	if(m_CandidateSpritePos.y < 0)
	{
		m_CandidateSpritePos.y = m_TextSpritePos.y + iHeight;
	}

	// Draw Background of real rect
	dwBackColor = SetBkColor(dc, CANDIDATE_BACK_COLOR);
	ExtTextOut(dc, 0, 0, ETO_OPAQUE, &Realrc, NULL, 0, 0);

	// Draw Selection
	RECT rcSel = Realrc;
	rcSel.top = rcSel.top + (m_pCandidateLists->dwSelection - dwStartIndex) * iHeight;
	rcSel.bottom = rcSel.top + iHeight;

	SetBkColor(dc, CANDIDATE_SELECTED_BACK_COLOR);
	ExtTextOut(dc, 0, rcSel.top, ETO_OPAQUE, &rcSel, NULL, 0, 0);

	SetBkMode(dc, TRANSPARENT);
	DWORD dwTextColor = GetTextColor(dc); 

	// Draw Candidate Strings.
	for(int y = 0; dwStartIndex < dwEndIndex; dwStartIndex++, y++)
	{
		LPSTR lpStr = (LPSTR) m_pCandidateLists + m_pCandidateLists->dwOffset[dwStartIndex];
		
		COLORREF Color = (dwStartIndex == m_pCandidateLists->dwSelection) ? 
					CANDIDATE_SELECTED_TEXT_COLOR : CANDIDATE_TEXT_COLOR;

		SetTextColor(dc, Color);

		wsprintf(szBuf, "%u| %s", y+1, lpStr);
		TextOut(dc, 2, y * iHeight, szBuf, lstrlen(szBuf));
	}
	
	SetTextColor(dc, CANDIDATE_TEXT_COLOR);

	wsprintf(szBuf, "%u/%u", m_pCandidateLists->dwSelection+1, m_pCandidateLists->dwCount);
	GetTextExtentPoint32(dc, szBuf, lstrlen(szBuf), &size);
	TextOut(dc, Realrc.right - size.cx - 2, dwNumPerPage * iHeight, szBuf, lstrlen(szBuf));

	SetTextColor(dc, dwTextColor);
	SetBkColor(dc, dwBackColor);	

	SelectObject(dc, hOldFont);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// eof