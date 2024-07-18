// HSIME.cpp: implementation of the CHSIME class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "HSIME.h"
#include <cassert>
//#include <cassert>

//언어 코드
#define LID_TRADITIONAL_CHINESE	0x0404
#define LID_JAPANESE			0x0411
#define LID_KOREAN				0x0412
#define LID_SIMPLIFIED_CHINESE	0x0804

#define SAFE_DELETE(p)			{ if(p) { delete (p);		(p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);		(p)=NULL; } }
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release();	(p)=NULL; } }
#define SAFE_FCLOSE(p)			{ if(p) { ::fclose(p);		(p)=NULL; } }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

class CSafeIMEContext
{
public:
	CSafeIMEContext(const HWND hWnd, HIMC &hIMC)
		:	m_hWnd(hWnd), m_rhIMC(hIMC)
	{
		m_rhIMC = ::ImmGetContext(m_hWnd);
	}
	~CSafeIMEContext()
	{
		::ImmReleaseContext(m_hWnd, m_rhIMC);
	}

	__inline	CSafeIMEContext&	operator=( const CSafeIMEContext& /*rhs*/ )
	{
		assert( !"Error!" );
        return *this;        
	}

private:
	HIMC &m_rhIMC;
	const HWND &m_hWnd;
};

using namespace HSIME;

CHSIME::CHSIME()
{
	m_hEWnd = NULL;
	m_pParentProc = NULL;
	m_lpCL = NULL;
	m_hImcSave = NULL;

	SetShowCandidate(false);

	SetIMEName("");
	SetLanguageType();

	m_bIsSymbolMode = false;
	m_bIsFullShape	= false;
	m_bIsComp		= false;
	m_bIsForceEnglishIME = false;
	SetMultiLine( false, 100);

	m_strCompStr = "";
	m_strCompReadStr = "";
	Clear();
}

CHSIME::~CHSIME()
{
	m_hImcSave = NULL;
	Clear(); 
}

void CHSIME::Clear()
{
	FreeCandList();
	ClearCadndidateBuf();
	
	m_mapKBHash.clear();

	if( GetEditHWND() )
	{
		::DestroyWindow(m_hEWnd);
		m_hEWnd = NULL;
	}
	FreeCandList();
}

void CHSIME::ClearCadndidateBuf()
{
	m_vtCandlist.clear();
}

const bool CHSIME::Init(const HWND hParentWnd, const HINSTANCE hInstance, const WNDPROC lpProc)
{
	Clear();

//	m_hEWnd = ::CreateWindow( "edit", NULL, 
				//			WS_CHILD | ES_AUTOHSCROLL| ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN,
				//			0, 0, 500, 400, hParentWnd, NULL, hInstance, NULL );
	m_hEWnd =			::CreateWindowEx(WS_EX_TOPMOST,"edit",NULL,WS_CHILD | ES_AUTOHSCROLL| ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN,
					0, 0, 500, 400, hParentWnd,NULL, hInstance, NULL);

#ifdef STRICT
	m_pParentProc = ( WNDPROC )::SetWindowLong( GetEditHWND(), GWL_WNDPROC, ( LONG )lpProc );
#else
	m_pParentProc = ( FARPROC )::SetWindowLong( GetEditHWND(), GWL_WNDPROC, ( LONG )lpProc );
#endif
	SetLimitLength(MAX_PATH,true);
	return true;
}

const bool CHSIME::SetFocus()
{
	const HWND hNowWnd = ::GetFocus();
	
	if(GetEditHWND() == hNowWnd)
	{
		return true;
	}

	const HWND hFocusWnd = ::SetFocus(GetEditHWND());
	
	return ((hFocusWnd == GetEditHWND())?true:false);
}

void CHSIME::SetOnlyNumeric( const bool bIsNumeric, const bool bDoClear)
{
	if(bDoClear)
	{
		SetString("");
	}
	
	m_bIsNumeric = bIsNumeric;
	
	const DWORD dwBefore = ::GetWindowLong(GetEditHWND(), GWL_STYLE);
	
	if(bIsNumeric)
	{
		::SetWindowLong(GetEditHWND(),GWL_STYLE, dwBefore | ES_NUMBER);
	}
	else
	{
		::SetWindowLong(GetEditHWND(),GWL_STYLE, dwBefore & ~ES_NUMBER);
	}
}

void CHSIME::SetMultiLine(const bool bIsMultiLine, const DWORD dwMaxCaretPerLine, const DWORD dwMaxLineConut, const bool bDoClear)
{
	m_bIsMultiLine = bIsMultiLine;

	if(IsMultiLine())
	{
		SetLimitLength( ((dwMaxCaretPerLine+2) * dwMaxLineConut) -2, bDoClear);//+2 는 캐니지리턴 값, -2는 마지막줄의 캐니지리턴값
		
		m_dwMaxCaretPerLine = dwMaxCaretPerLine;
		m_dwMaxLineCount = dwMaxLineConut;

		::SendMessage(GetEditHWND(), EM_LINELENGTH,  m_dwLimitLength, NULL);
	}
}

void CHSIME::SetLimitLength(const DWORD dwLen, const bool bDoClear)
{
	if(bDoClear)
	{
		SetString("");
	}
	
	m_dwLimitLength = dwLen;

	::SendMessage(GetEditHWND(), EM_SETLIMITTEXT,  m_dwLimitLength, NULL);
}

const CARETDATA& CHSIME::GetCaretPos()
{
	CARETDATA TempCD = m_CaretData;

	::SendMessage(GetEditHWND(), EM_GETSEL,  (WPARAM)&m_CaretData.iSelectStart, (LPARAM)&m_CaretData.iSelectEnd);

	if(m_CaretData.iSelectStart != TempCD.iSelectStart)
	{
		m_CaretData.iCaretPos = m_CaretData.iSelectStart;
	}

	if(m_CaretData.iSelectEnd != TempCD.iSelectEnd)
	{
		m_CaretData.iCaretPos = m_CaretData.iSelectEnd;
	}
	return m_CaretData;
}

void CHSIME::SetCaretPos(const bool bIsHome)//캐럿을 맨앞 또는 맨뒤로 설정합니다.
{
	if(bIsHome)//맨앞
	{
		::SendMessage(GetEditHWND(), WM_KEYDOWN, VK_HOME, NULL);
	}
	else
	{
		::SendMessage(GetEditHWND(), WM_KEYDOWN, VK_END, NULL);
	}
}

void CHSIME::SetCaretPos(const __int32 iStartPos, const __int32 iEndPos)//캐럿의 영역 설정 iRange 가 0 이 아니면 영역 설정.
{
	::SendMessage(GetEditHWND(), EM_SETSEL,  (WPARAM)iStartPos, (LPARAM)iEndPos);
	
	m_CaretData.iCaretPos		= iEndPos;
	m_CaretData.iSelectStart	= iEndPos;
	m_CaretData.iSelectEnd		= iStartPos;
}

bool CHSIME::IsCorrectString(const string& strSrc)
{
	string::size_type len = strSrc.length();
	if( IsOnlyNumeric() && len )
	{
		int iTemp = ::atoi(strSrc.c_str());
		if(!iTemp)
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	return true;
}

void CHSIME::GetWindowText(string &str)
{
//	str.reserve(m_dwLimitLength+2);//이전 상태가 현재 예약하려는 크기보다 크다고 해도 벡터내부에서는 다르게 동작한다.
	static vector<char> vt(MAX_PATH,NULL);//MAX_PATH 의 크기를 가지는 벡터

	vt.reserve(m_dwLimitLength+2);

	::GetWindowText(GetEditHWND(), &vt[0], m_dwLimitLength+1 );

	str = (const char*)&vt[0];
}

bool CHSIME::IsCanInputAtMultiLine(const string& strSrc, const int nVirtKey)
{
	if(!IsMultiLine()){return true;}//멀티라인이 아니면 리턴

	DWORD dwLineCount = 0;
//	string::size_type size = strSrc.length();

//	DWORD dwNotUsedCaret = 0;//라인당 사용하지 않은 캐럿

	string strTemp(strSrc);//버퍼 할당
//	const DWORD dwSrcSize = strSrc.size();

#pragma warning( disable : 4127 )
	while(true)
#pragma warning( default : 4127 )
	{
		string::size_type idx = strTemp.find( "\r\n" );
		if(string::npos != idx)//캐니지리턴을 찾았다
		{
			++dwLineCount;//찾았으니 일단 한줄 먹고
			string strHead = strTemp.substr(0, idx);// \n\r 의 길이가 2 이기 때문에 +1을 해야함

			dwLineCount += strHead.length() / m_dwMaxCaretPerLine;

//			if(strHead.length() % m_dwMaxCaretPerLine)
//			{
//				dwNotUsedCaret += m_dwMaxCaretPerLine - (strHead.length() % m_dwMaxCaretPerLine);
//				++dwLineCount;
//			}

			if(dwLineCount >= m_dwMaxLineCount)
			{
				return false;
			}
			if( (dwLineCount+1) == m_dwMaxLineCount && VK_RETURN == nVirtKey)//한줄더 더해기한상태에서 엔터키 들어오면
			{
				return false;
			}

			string strTail = strTemp.substr(idx+2);// \n\r 의 길이가 2 이기 때문에
			if(!strTail.empty())//비어있지 않다
			{
				 strTemp = strTail;
			}
			else
			{
				break;
			}
		}
		else //캐니지리턴을 찾지 못했다
		{
			dwLineCount += strTemp.length() / m_dwMaxCaretPerLine;
			
			if(dwLineCount >= m_dwMaxLineCount)
			{
				return false;
			}
			if( (dwLineCount+1) == m_dwMaxLineCount && VK_RETURN == nVirtKey)
			{
				return false;
			}

			if( strTemp.length() % m_dwMaxCaretPerLine)
			{
				++dwLineCount; 
			}
			break;
		}
	}
	return true;
}

void CHSIME::SetString(const char* szText)
{
	if(IsCorrectString(szText))
	{
		::SetWindowText(GetEditHWND(), szText);
	}
	else
	{
		::SetWindowText(GetEditHWND(), "");
	}

	SetCaretPos(false);//캐럿을 맨 끝으로
}

const string& CHSIME::GetResultStr()
{
	GetWindowText(m_strResult);
	
	if( !IsCorrectString(m_strResult) )
	{
		SetString("");
		m_strResult = "";
	}

	if( IsOnlyNumeric())
	{
		const double dbNum = ::atof(m_strResult.c_str());
		char szNumeric[MAX_PATH] = {0,};
		::sprintf(szNumeric,"%.0f",dbNum);
		m_strResult = szNumeric;
	}

	if(m_bIsForceEnglishIME)
	{
		if(CorrectAlphaNumeric(m_strResult))
		{
//			::SetWindowText(GetEditHWND(),m_strResult.c_str());//이거하면 comp 끝나고 앞에 글자가 지워짐
		}
	}

	return m_strResult;
}

void CHSIME::SetEnableIME(const bool bIsEnable)
{
	if(bIsEnable)
	{
		if(m_hImcSave)
		{
			::ImmAssociateContext(GetEditHWND(), m_hImcSave);
			m_hImcSave = NULL;
		}
	}
	else
	{
		HIMC hIMC = NULL;
		
		CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
		
		if(!hIMC){return;}

		m_hImcSave = hIMC;//끄기전 IME 핸들을 저장한다 
		::ImmAssociateContext(GetEditHWND(), NULL);//IME 핸들 없엠
	}
}

void CHSIME::SetNextIME()
{
	::ActivateKeyboardLayout(( HKL )HKL_NEXT, 0 );
}

void CHSIME::SetNativeIME()//자국어 입력기로 전환.
{
	if(GetLanguageType() != LT_JAPANESE)
	{
		HIMC hIMC = NULL;
		CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
		if(!hIMC){return;}

		::ImmSetConversionStatus(hIMC, IME_CMODE_NATIVE, IME_SMODE_AUTOMATIC);
	}
}

void CHSIME::SetEnglishIME(const bool bIsForce)
{
	if(GetLanguageType() != LT_JAPANESE)
	{
		HIMC hIMC = NULL;
		CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
		if(!hIMC){return;}
		
		m_bIsForceEnglishIME = bIsForce;
		::ImmSetConversionStatus(hIMC, IME_CMODE_ALPHANUMERIC, IME_SMODE_AUTOMATIC);
	}
}

void CHSIME::SetDisableEnglish()
{
	m_bIsForceEnglishIME = false;
}

void CHSIME::SetShapeIME()
{
	switch(GetLanguageType())
	{
	case LT_SIMPLIFIED_CHINESE:
		{
			::ImmSimulateHotKey( GetEditHWND(), IME_CHOTKEY_SHAPE_TOGGLE );
		}break;

	case LT_TRADITIONAL_CHINESE:
		{
			::ImmSimulateHotKey( GetEditHWND(), IME_THOTKEY_SHAPE_TOGGLE );
		}break;
	default:
		{
			SetConversionStatus( GetEditHWND(), !IsFullShape(), IME_CMODE_FULLSHAPE);
		}break;
	}	
}

void CHSIME::SetSymbolIME()
{
	switch(GetLanguageType())
	{
	case LT_SIMPLIFIED_CHINESE:
		{
			::ImmSimulateHotKey( GetEditHWND(), IME_CHOTKEY_SYMBOL_TOGGLE );
		}break;

	case LT_TRADITIONAL_CHINESE:
		{
			::ImmSimulateHotKey( GetEditHWND(), IME_THOTKEY_SYMBOL_TOGGLE );
		}break;
	default:
		{
		}break;
	}
}

void CHSIME::SetLanguageType()
{
	const HKL hKL = ::GetKeyboardLayout(0);

	switch (LOWORD(hKL))
	{
	case LID_TRADITIONAL_CHINESE:	{m_eLanguageType = LT_TRADITIONAL_CHINESE;}break;
	case LID_JAPANESE:				{m_eLanguageType = LT_JAPANESE;}break;
	case LID_KOREAN:				{m_eLanguageType = LT_KOREAN;}break;
	case LID_SIMPLIFIED_CHINESE:	{m_eLanguageType = LT_SIMPLIFIED_CHINESE;}break;
	default:						{m_eLanguageType = LT_DEFAULT;}break;
	}
}

void CHSIME::SetConversionStatus( const HWND& /*hWnd*/, const bool bIsOn, const DWORD dwNewConvMode, const DWORD dwNewSentMode)
{
	DWORD dwConversion = 0, dwSentence = 0;

	HIMC hIMC = NULL;
	CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
	if(!hIMC){return;}
	
	::ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );
	
	if(dwNewConvMode || dwNewSentMode)
	{
		if(bIsOn)
		{
			::ImmSetConversionStatus( hIMC, dwConversion | dwNewConvMode, dwSentence | dwNewSentMode );
		}
		else
		{
			::ImmSetConversionStatus( hIMC, dwConversion & ~dwNewConvMode, dwSentence & ~dwNewSentMode );
		}
	}

	::ImmGetConversionStatus( hIMC, &dwConversion, &dwSentence );

	m_bIsFullShape	= (( dwConversion & IME_CMODE_FULLSHAPE )? true : false);
	m_bIsSymbolMode	= (( dwConversion & IME_CMODE_SYMBOL	)? true : false);
}

LPCANDIDATELIST CHSIME::GetCandiPoint()
{
	if( m_lpCL && m_lpCL->dwCount ) {return m_lpCL;}
	return NULL;
}

void CHSIME::FreeCandList()
{
//	SAFE_DELETE_ARRAY(m_lpCL);
	m_vecCandListBuf.clear();
}

void CHSIME::AllocCandList( const DWORD dwSize )
{
	FreeCandList();
	if(!dwSize)
	{
//		assert(dwSize && "AllocCandList dwSize 0");
		return;
	}
	
	m_vecCandListBuf.resize(dwSize);

	m_lpCL = reinterpret_cast<LPCANDIDATELIST>( &m_vecCandListBuf.at(0) );
}

bool CHSIME::SetIMECandiDateList(const HWND /*hWnd*/)//한국용
{
	HIMC hIMC = NULL;
	CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
	if(!hIMC){return false;}

	const DWORD dwSize = ::ImmGetCandidateList(hIMC,0x0,NULL,0);//Cand 리스트의 사이즈를 가져온다

	FreeCandList();

	AllocCandList(dwSize);//받은 만큼의 길이로 CandidateList의 버퍼를 생성.Address --> m_lpCL

	ClearCadndidateBuf();//스트링의 저장용 버퍼를 비움

	DWORD i= 0;
	__int32 iCount = 0;
	LPDWORD lpdwOffset = NULL;
	if( !m_lpCL ){goto __FAIL;}

	if(!::ImmGetCandidateList(hIMC,0x0,m_lpCL,dwSize)){	goto __FAIL;}

	lpdwOffset = &m_lpCL->dwOffset[0];

	if(GetLanguageType() == LT_JAPANESE
	|| GetLanguageType() == LT_SIMPLIFIED_CHINESE)
	{
		if(m_lpCL->dwPageSize)
		{
			m_lpCL->dwPageStart = m_lpCL->dwPageSize*(m_lpCL->dwSelection/m_lpCL->dwPageSize);
		}
	}
	
	lpdwOffset += m_lpCL->dwPageStart;

	for (i = m_lpCL->dwPageStart; (i < m_lpCL->dwCount) && (i < m_lpCL->dwPageStart + m_lpCL->dwPageSize) ; i++)
	{
		++iCount;
		string strTemp = (LPSTR)m_lpCL + *lpdwOffset++;
		CCandidateData CD(iCount,strTemp);
		m_vtCandlist.push_back(CD);
	}

	goto __SUCCESS;

__FAIL:
	{
//		FreeCandList();
		return false;
	}
__SUCCESS:
	{
//		FreeCandList();
		return true;
	}	
}

/*
bool CHSIME::SetIMEConversionList(const HWND hWnd)
{
	HIMC hIMC = NULL;
	CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
	if(!hIMC){return false;}

	const HKL hKL = ::GetKeyboardLayout(0);
	if( ::ImmIsIME( hKL ))
	{
	//-->??	const DWORD dwSize = ::ImmGetCandidateList(hIMC,0x0,NULL,0);//Cand 리스트의 사이즈를 가져온다					
		char szTemp[MAX_PATH] = {0,};
		AllocCandList(MAX_PATH);

//		DWORD dwBufSize = ::ImmGetConversionList( hKL, hIMC, szTemp, m_lpCL, MAX_PATH, GCL_CONVERSION );
		
		FreeCandList();

//		if(dwBufSize)
		{
			goto __SUCCESS;
		}
	}

	goto __FAIL;
__FAIL:
	{
		return false;
	}
__SUCCESS:
	{
		return true;
	}
}
*/

LRESULT CALLBACK CHSIME::OnWM_IME_NOTIFY(const HWND &hWnd, const UINT &/*message*/, const WPARAM &wParam, const LPARAM &/*lParam*/)
{//이미 처리할것이 있었다면 이후의 기본 처리는 하지않는다.
	//return true;//이후에 기본 프로시저를 무시하라는 의미
	//return false;//이후에 기본 프로시저를 호출 하라는 의미
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_NOTIFY [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif

	switch (wParam)
	{
	case IMN_CLOSESTATUSWINDOW://0x0001
		{
		}break;
	case IMN_OPENSTATUSWINDOW://0x0002
		{
		}break;
	case IMN_CLOSECANDIDATE:			//0x0004 //캔디데이트가 닫힌다.
		{
			ClearCadndidateBuf();
		}break;
	case IMN_CHANGECANDIDATE://0x0003
	case IMN_OPENCANDIDATE://0x0005
		{
			SetIMECandiDateList(hWnd);
#ifdef _DEBUG
{
		LPCANDIDATELIST lpC = GetCandiPoint();
		if(lpC)
		{
			char szDev[MAX_PATH] = {0,};
			::sprintf(szDev,"Count[%d] PageSize[%d] PageStart[%d] Selection[%d]\n",
				lpC->dwCount,lpC->dwPageSize,lpC->dwPageStart,lpC->dwSelection);
			::OutputDebugString((LPSTR)szDev);
		}
}
#endif
			if(!IsShowCandidate())//주음 입력법 뒤에 나오는 쉬프트 리스트 아이들
			{
				return true;
			}
		}break;
	case IMN_SETCONVERSIONMODE://0x0006 문자 변환 모드로 전환. IMN_SETOPENSTATUS 다음 들어옴
		{
			SetConversionStatus(hWnd);
		}break;
	case IMN_SETSENTENCEMODE://0x0007
		{
			//ImmGetConversionStatus
		}			
	case IMN_SETOPENSTATUS://0x0008 스테이터스 창이 열렸다.
		{
		}break;
	case IMN_SETCANDIDATEPOS:{}			//0x0009
	case IMN_SETCOMPOSITIONFONT:		//0x000A
		{
		}break;
	case IMN_SETCOMPOSITIONWINDOW:        //0x000B //컴포지션 윈도우가 뭔지 몰라도 -_-;.. 셋된대..
		{
			ClearCadndidateBuf();
		}break;
	case IMN_SETSTATUSWINDOWPOS:{}		//0x000C
	case IMN_GUIDELINE://0x000D
		{
		}break;
	case IMN_PRIVATE://이것은 고칠 수가 없다
		{
		}
	default:
		{
			return false;
		}
	}
    return false;
}

LRESULT CALLBACK CHSIME::OnWM_IME_COMPOSITION(const HWND &/*hWnd*/, const UINT &/*message*/, const WPARAM &/*wParam*/, const LPARAM &lParam)
{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_COMPOSITION [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
//	char szDebug[MAX_PATH] = {0,};
//	sprintf(szDebug,"OnWM_IME_COMPOSITION [%d] [%d] [%d]\n", lParam, m_strCompStr.size(),m_strCompReadStr.size());
//	OutputDebugString(szDebug);

//	SetIMEConversionList(hWnd);

	HIMC hIMC = NULL;
	CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
	if(!hIMC){return 0;}

	DWORD dwSize = 0;
	char szTemp[ MAX_PATH ]= {0,};

	if( (lParam & GCS_RESULTSTR)
	||	!lParam )//일본판의 경우 0으로 들어오면 Comp 를 끝낸다는 메세지다.
	{
		m_strCompStr = "";
		m_strCompReadStr = "";
		m_CaretData.iImeCursor = 0;
		goto __RETURN_FALSE;
	}
	if( lParam & GCS_COMPSTR )
	{
		if(GetLanguageType() == LT_KOREAN)
		{
			goto __RETURN_FALSE;
		}

		dwSize = ::ImmGetCompositionString( hIMC, GCS_COMPSTR, szTemp, sizeof( szTemp ));
		szTemp[ dwSize ] = '\0';

		m_strCompStr = szTemp;

		// Get composition attribute and cursor position in composition string
		if (lParam & GCS_COMPATTR)//조합중인 구성 요소를 받아 들임.
		{
			dwSize = ::ImmGetCompositionString(hIMC, GCS_COMPATTR, szTemp, sizeof(szTemp));
		}
		if (lParam & GCS_COMPCLAUSE)
		{
			DWORD adwTemp[MAX_PATH] = {0,};
			::ImmGetCompositionString(hIMC, GCS_COMPCLAUSE, adwTemp, sizeof(adwTemp));
		}

		if( lParam & GCS_CURSORPOS )
		{
			m_CaretData.iImeCursor = 0xffff & ::ImmGetCompositionString( hIMC, GCS_CURSORPOS, NULL, 0 );
		}
		if( lParam & GCS_DELTASTART )
		{
			m_CaretData.iImeDeltaStart = 0xffff & ::ImmGetCompositionString( hIMC, GCS_DELTASTART, NULL, 0 );
		}
	}
	if( lParam & GCS_COMPREADSTR )
	{
		dwSize = ::ImmGetCompositionString( hIMC, GCS_COMPREADSTR, szTemp, sizeof( szTemp ));
		szTemp[ dwSize ] = '\0';

		m_strCompReadStr = szTemp;
	}

	if(m_strCompReadStr == m_strCompStr)
	{
		m_strCompReadStr = "";
	}
	goto __RETURN_TRUE;

__RETURN_TRUE:
	{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_COMPOSITION Return TRUE\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
		return true;
	}
__RETURN_FALSE:
	{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_COMPOSITION Return FALSE\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
		return false;
	}
}

LRESULT	CALLBACK CHSIME::OnWM_IME_SETCONTEXT(const HWND &/*hWnd*/, const UINT &/*message*/, WPARAM &/*wParam*/, LPARAM &lParam)
{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_SETCONTEXT [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif

	if(!IsShowCandidate())
	{
		lParam &= (~ISC_SHOWUICANDIDATEWINDOW);
		lParam &= (~ISC_SHOWUICOMPOSITIONWINDOW);
		lParam &= (~ISC_SHOWUIGUIDELINE);
		lParam &= (~ISC_SHOWUIALL);
		return false;
	}
	return false;
}

LRESULT	CALLBACK CHSIME::OnWM_INPUTLANGCHANGEREQUEST(const HWND &/*hWnd*/, const UINT &/*message*/, const WPARAM &/*wParam*/, const LPARAM &/*lParam*/)
{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_INPUTLANGCHANGEREQUEST [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
	return false;
}

LRESULT	CALLBACK CHSIME::OnWM_INPUTLANGCHANGE(const HWND &hWnd, const UINT &/*message*/, const WPARAM &/*wParam*/, const LPARAM &/*lParam*/)
{//언어가 바뀐 정보를 받는다. 내부에서는 언어에 따른 폰트를 바꾸면 좋다.
	//wParam	Specifies the character set of the new locale. 
	//lParam	Input locale identifier. For more information, see Languages, Locales, and Keyboard Layouts. 
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_INPUTLANGCHANGE [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
	const HKL hKL = ::GetKeyboardLayout( 0 );
	if( ::ImmIsIME( hKL ))
	{
		HIMC hIMC = NULL;
		CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
		if(!hIMC){return false;}
		char szTemp[MAX_PATH] = {0,};
		::ImmEscape( hKL, hIMC, IME_ESC_IME_NAME, szTemp );//IME이름 가져오기
		
		SetIMEName(szTemp);//입력기 정보 셋팅
		
		SetConversionStatus(hWnd);

		SetLanguageType();
	}
	else
	{
		SetIMEName("");//이름 없음
	}
	return false;
}

LRESULT	CALLBACK CHSIME::OnWM_IME_STARTCOMPOSITION(const HWND &/*hWnd*/, const UINT &/*message*/, const WPARAM &/*wParam*/, const LPARAM &/*lParam*/)
{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_STARTCOMPOSITION [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
	m_bIsComp = true;

	if(!IsShowCandidate())//주음 입력법 조합중인 문자 나오지 않게
	{
		return true;
	}	
	return false;
}

LRESULT	CALLBACK CHSIME::OnWM_IME_ENDCOMPOSITION(const HWND &/*hWnd*/, const UINT &/*message*/, const WPARAM &/*wParam*/, const LPARAM &/*lParam*/)
{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_ENDCOMPOSITION [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
	m_bIsComp = false;
	return false;
}

LRESULT	CALLBACK CHSIME::OnWM_IME_CONTROL(const HWND &/*hWnd*/, const UINT &/*message*/, const WPARAM &/*wParam*/, const LPARAM &/*lParam*/)
{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_CONTROL [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
	return false;
}

LRESULT	CALLBACK CHSIME::OnWM_CHAR(const HWND &/*hWnd*/, const UINT &/*message*/, const WPARAM &wParam, const LPARAM &/*lParam*/)
{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_CHAR [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
	const int nVirtKey = ( int )wParam;	// virtual-key code 

	if(IsMultiLine() && nVirtKey != VK_BACK)
	{
		string str = "";
		GetWindowText(str);
		
		if( !IsCanInputAtMultiLine(str, nVirtKey) )
		{
			return true;
		}
	}

	bool bIsOk = false;
	if('0' <= nVirtKey && '9' >= nVirtKey ){bIsOk = true;}
	if('A' <= nVirtKey && 'Z' >= nVirtKey ){bIsOk = true;}
	if('a' <= nVirtKey && 'z' >= nVirtKey ){bIsOk = true;}
	if( VK_BACK == nVirtKey ){bIsOk = true; }
	if( VK_HOME == nVirtKey ){bIsOk = true; }
	if( VK_END  == nVirtKey ){bIsOk = true; }
	if( VK_INSERT  == nVirtKey ){bIsOk = true; }
	if( VK_DELETE  == nVirtKey ){bIsOk = true; }
	
	if(m_bIsForceEnglishIME && !bIsOk)
	{
		return true;
	}

	switch ( nVirtKey )//엔터나 탭 치면 삑삑 소리나는거 방지
	{
	case VK_RETURN: 
		{
			return (IsMultiLine()?false:true);
		}break;
	case VK_TAB: 
		{
			return true;
		}
	}
	return false;
}

LRESULT	CALLBACK CHSIME::OnWM_IME_SELECT(const HWND &/*hWnd*/, const UINT &/*message*/, const WPARAM &/*wParam*/, const LPARAM &/*lParam*/)
{
#ifdef _DEBUG
{
	char szDebug[MAX_PATH] = {0,};
	sprintf(szDebug,"WM_IME_SELECT [%08X][%08X]\n", wParam,lParam);
    OutputDebugString(szDebug);
}	
#endif
	return false;
}

bool CALLBACK CHSIME::HandleMessage(HWND &hWnd, UINT &message, WPARAM &wParam, LPARAM &lParam)
{
	switch ( message )//(WINVER >= 0x0400)
	{
	case WM_KEYDOWN:
		{
			Excute(wParam,KEY_STATE_DOWN);
		}break;
	case WM_KEYUP:
		{
			Excute(wParam,KEY_STATE_UP);
		}break;
	case WM_CHAR:
		{
			if(OnWM_CHAR( hWnd, message, wParam, lParam)){return true;}
		}break;
    case WM_IME_SETCONTEXT:
		{
			if(OnWM_IME_SETCONTEXT( hWnd, message, wParam, lParam)){return true;}
		}break;
	case WM_IME_NOTIFY:
		{
			if(OnWM_IME_NOTIFY( hWnd, message, wParam, lParam)){return true;}
		}break;
	case WM_IME_CONTROL:
		{
			if(OnWM_IME_CONTROL( hWnd, message, wParam, lParam)){return true;}
		}break;
	case WM_IME_COMPOSITIONFULL:
		{
		}break;
	case WM_IME_SELECT:
		{
			if(OnWM_IME_SELECT( hWnd, message, wParam, lParam)){return true;}
		}break;
	case WM_IME_CHAR:
		{
		}break;
//	case WM_IME_REQUEST:
//		{
//		}break;
	case WM_IME_KEYDOWN:
		{
		}break;
	case WM_IME_KEYUP:
		{
		}break;
	case WM_IME_COMPOSITION ://조합중인 문자의 정보를 받을 수 있다.
		{
			if(OnWM_IME_COMPOSITION( hWnd, message, wParam, lParam)){return true;}
		}break;
	case WM_INPUTLANGCHANGEREQUEST:
		{
			if(OnWM_INPUTLANGCHANGEREQUEST( hWnd, message, wParam, lParam)){return true;}
		}break;
	case WM_INPUTLANGCHANGE:
		{
			if(OnWM_INPUTLANGCHANGE(hWnd, message, wParam, lParam)){return true;}
		}break;
	case WM_IME_STARTCOMPOSITION://주음입력
		{
			if(OnWM_IME_STARTCOMPOSITION( hWnd, message, wParam, lParam)){return true;}
		}break;
	case WM_IME_ENDCOMPOSITION://== WM_IME_KEYLAST == WM_IME_COMPOSITION
		{
			if(OnWM_IME_ENDCOMPOSITION( hWnd, message, wParam, lParam)){return true;}
		}break;

	default:
		{
			return false;
		}break;
	}
	return false;
}

bool CHSIME::FuncBind(const __int32 iIndex, const LPKBFUNC lpFunc)
{
	const KBIter iter = m_mapKBHash.find(iIndex); 

	if( m_mapKBHash.end() != iter )
	{
//		assert(NULL && "Use Input Index Already ");
		return false;
	}

	m_mapKBHash.insert( KB_HASH::value_type(iIndex,lpFunc) );
	return true;
}

__int32 CHSIME::Excute(const __int32 iIndex, const eKeyState KeyState)
{
	const KBIter iter = m_mapKBHash.find(iIndex); 
	if( m_mapKBHash.end() == iter )
	{
//		assert(NULL && "KBExcute Index Is Failed");//주석을 풀면 등록 안된 키보드 누르면 크래쉬 납니다.
		return 0;
	}
	return (*iter->second)(KeyState);
}

bool CHSIME::CorrectAlphaNumeric(string &str)
{
	string::iterator itor = str.begin();

	bool bIsChange = false;
	while( str.end() != itor )
	{
		const char value = (*itor);
		if( 0x80 & value )
		{
			itor = str.erase(itor);//2글자 삭제
			if( str.end() != itor)
			{
				itor = str.erase(itor);
			}
			bIsChange = true;
			continue;
		}
		bool bIsOk = false;
		if('0' <= value && '9' >= value ){bIsOk = true;}
		if('A' <= value && 'Z' >= value ){bIsOk = true;}
		if('a' <= value && 'z' >= value ){bIsOk = true;}
		if(!bIsOk)
		{
			itor = str.erase(itor);
			bIsChange = true;
			continue;
		}
		++itor;
	}

	return bIsChange;
}

void CHSIME::SetEnableIME(const bool bIsEnable, HWND hWnd, HIMC &hIMC)
{
	if(bIsEnable)
	{
		if(hIMC)
		{
			::ImmAssociateContext(hWnd, hIMC);
			hIMC = NULL;
		}
	}
	else
	{
		HIMC hIMC = NULL;
		CSafeIMEContext SafeIC(GetEditHWND(), hIMC);
		if(!hIMC){return;}

		::ImmAssociateContext(hWnd, NULL);//IME 핸들 없엠
	}
}
