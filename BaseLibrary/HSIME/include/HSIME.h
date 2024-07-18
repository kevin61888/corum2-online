// HSIME.h: interface for the CHSIME class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HSIME_H__17714882_2106_46F5_9F6E_40C38C80E4FF__INCLUDED_)
#define AFX_HSIME_H__17714882_2106_46F5_9F6E_40C38C80E4FF__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <imm.h>
#pragma comment(lib,"imm32.lib")

#pragma warning(disable:4786)

#include <string>
#include <map>
#include <hash_map>
#include <vector>

using namespace std;
using namespace stdext;

namespace HSIME
{
	typedef struct CARETDATA
	{
		__int32 iCaretPos;
		__int32 iSelectStart;
		__int32 iSelectEnd;
		__int32 iImeCursor;
		__int32 iImeDeltaStart;
		CARETDATA()
		{
			iCaretPos = 0;
			iSelectStart = 0;
			iSelectEnd = 0;
			iImeCursor = 0;
			iImeDeltaStart = 0;
		}
	}t_CaretData,*LPCARETDATA;

	class CCandidateData
	{
	public:
		CCandidateData(const int iIndex, const string &str)
		{
			m_iIndex  = iIndex;
			m_strCadidate = str;
		};
		~CCandidateData(){};

		__int32 GetIndex() const {return m_iIndex;}
		const string& GetString() const {return m_strCadidate;}
	
	private:
		__int32 m_iIndex;
		string	m_strCadidate;
	};

	class CHSIME  
	{
	public:
		enum eKeyState
		{
			KEY_STATE_DOWN	= 1,
			KEY_STATE_UP	= 2,
		};

		typedef vector<CCandidateData> CANDI_BUF;
		typedef CANDI_BUF::iterator CANDI_ITOR;
		typedef CANDI_BUF::const_iterator CANDI_CONST_ITOR;

		typedef __int32 (CALLBACK *LPKBFUNC)(const eKeyState KeyState);//키보드가 눌러졌을때 호출 되는 함수
		
		typedef hash_map<__int32, LPKBFUNC> KB_HASH;
		typedef KB_HASH::iterator KBIter;

		enum eLANGUAGE_TYPE
		{
			LT_DEFAULT = 0,
			LT_TRADITIONAL_CHINESE,	
			LT_JAPANESE,
			LT_KOREAN,
			LT_SIMPLIFIED_CHINESE
		}; 
	public:
		CHSIME();
		~CHSIME();
	
	public:
		const bool Init(const HWND hParentWnd, const HINSTANCE hInstance, const WNDPROC lpProc);

		const bool SetFocus();//EditWnd로 포커스를 셋팅. 포커스가 없으면 문자 입력 불가능.

		const bool IsShowCandidate() const {return m_bIsShowCandidate;}//윈도우가 보여주는 기본창을 볼 수 있게 할 것인가?
		const bool IsSymbolMode() const {return m_bIsSymbolMode;}//입력기 상태중 심볼 입력 모드
		const bool IsFullShape() const {return m_bIsFullShape;}//전자, 반자 모드
		const bool IsOnlyNumeric() const {return m_bIsNumeric;}//숫자 입력 모드인가? 
		const bool IsMultiLine() const {return m_bIsMultiLine;}//엔터 입력을 받는가?(멀티라인을 쓰는가)
		const bool IsNowComp() const {return m_bIsComp;}//지금 조합중인가?

	public:
		const string& GetIMEName() const { return m_strIMEName; }//현재 입력기의 이름을 리턴
		const CARETDATA& GetCaretPos();
		const eLANGUAGE_TYPE GetLanguageType() const{ return m_eLanguageType; }//언어타입을 리턴. eLANGUAGE_TYPE를 참조

		DWORD GetLimitLength() const {return m_dwLimitLength;}
		DWORD GetMaxLineCount() const {return m_dwMaxLineCount;}
		
	public://문자열 관련
		const string& GetResultStr();
		const string& GetCompStr()  const { return m_strCompStr; }//조합의 결과물을 리턴
		const string& GetCompReadStr() const { return m_strCompReadStr; }//조합중인 상태의 문자를 리턴

//		void GetCadidateData(CANDI_BUF &vtOut){	vtOut = m_vtCandlist; }//Candidate를 리턴
		
		const CANDI_BUF& GetCadidateData() const {return m_vtCandlist;}//Candidate를 리턴

		LPCANDIDATELIST	GetCandiPoint();//Cadndidate 리스트가 있을때 m_lpCL 을 리턴

		void SetString(const char* szText);//강제로 내용을 셋팅. IsOnlyNumeric인데 숫자가 아닌 문자를 넣으면 ""로 셋팅 됨

	public://모드 관련
		void SetShowCandidate(const bool bIsShow) { m_bIsShowCandidate = bIsShow; }
		void SetEnableIME(const bool bIsEnable);
		void SetEnableIME(const bool bIsEnable, HWND hWnd, HIMC &hIMC);//해당 윈도우의 IME 켜기 끄기
		void SetNextIME();//다른 입력기로 전환.
		void SetNativeIME();//자국어 입력기로 전환.
		void SetEnglishIME(const bool bIsForce);//영어 입력기로 전환
		
		void SetDisableEnglish();//영어입력 강제로 셋팅 되었을 경우 풀어줌

		void SetShapeIME();//전/반자 모드 전환
		void SetSymbolIME();//심볼 모드로 전환
		void SetCaretPos(const bool bIsHome);//캐럿을 맨앞 또는 맨뒤로 설정
		void SetCaretPos(const __int32 iStartPos, const __int32 iEndPos );//캐럿의 영역 설정, (iStartPos = 0 , iEndPos = -1 )--> 모든 영역선택 //캐럿은 iEndPos 로 설정
		void SetOnlyNumeric( const bool bIsNumeric, const bool bDoClear = true );//숫자 전용 모드로 셋팅
		void SetMultiLine(const bool bIsMultiLine, const DWORD dwMaxCaretPerLine= 1, const DWORD dwMaxLineConut = 1, const bool bDoClear = true);
		
		void SetLimitLength(const DWORD dwLen, const bool bDoClear = true);//입력받는 최대 길이 셋팅. 
		
	public://키보드 이벤트 바인딩
		bool FuncBind(const __int32 iIndex, const LPKBFUNC lpFunc);//함수 바인딩
		
	private://키보드 이벤트 함수 실행
		__int32 Excute(const __int32 iIndex, const eKeyState KeyState);

	public://Edit Wnd 프로시저에서 사용
		bool CALLBACK HandleMessage(HWND &hWnd, UINT &message, WPARAM &wParam, LPARAM &lParam);//윈도우 프로시저 에서 불려지는 콜백

#ifdef STRICT
		WNDPROC	GetParentProc(){return m_pParentProc;}
#else
		FARPROC	GetParentProc(){return m_pParentProc;}
#endif

	private:
		LPCANDIDATELIST m_lpCL;	//Candidate버퍼 포인터
		CANDI_BUF m_vtCandlist;
		KB_HASH m_mapKBHash;

		bool m_bIsShowCandidate;//Candidate윈도우를 보이게 하는 플래그 
		bool m_bIsSymbolMode;//심볼 모드인지 알아 보는 플래그
		bool m_bIsFullShape;//전/반자 플래그
		bool m_bIsNumeric;//숫자전용 모드 플래그
		bool m_bIsMultiLine;//엔터 입력을 받을 것인가?
		bool m_bIsComp;//현재 조합중인가?
		
		string m_strIMEName;//IME 의 이름.
		
		CARETDATA	m_CaretData;//셀렉트된 캐럿의 위치
		
		eLANGUAGE_TYPE m_eLanguageType;
	
	private://문자열
		string m_strCompStr;//조합중인 문자가 저장됨
		string m_strCompReadStr;//조합을 위해서 읽히고 있는 문자열
		string m_strResult;//결과물. 에디트 박스에 기록되는 최종 스트링
		DWORD m_dwLimitLength;//최대 입력 길이 생성시에 MAX_PATH로 셋팅
		DWORD m_dwMaxCaretPerLine;//멀티라인시 1줄당 최대 캐럿
		DWORD m_dwMaxLineCount;//멀티라인시 라인카운트

	private:
		bool m_bIsForceEnglishIME;
		HWND m_hEWnd;//입력을 받고있는 IME 윈도우의 핸들 //Init 시에 생성.
		HIMC m_hImcSave;//IME Enable/Disable 때문에 있는 IME 핸들 저장용변수

	private:
		vector<char> m_vecCandListBuf;

#ifdef STRICT
		WNDPROC	m_pParentProc;
#else
		FARPROC	m_pParentProc;
#endif

	private:
		void Clear();
		void ClearCadndidateBuf();//벡터 버퍼를 지움

		const HWND GetEditHWND() const {return m_hEWnd;}

		void SetIMEName(const char* szIMEName) { m_strIMEName = szIMEName; }//내부에서만 사용 
		void SetConversionStatus( const HWND &hWnd, const bool bIsOn = true, const DWORD dwNewConvMode = 0, const DWORD dwNewSentMode = 0);//입력기의 모드 전환 
		void SetLanguageType();//입력기의 언어타입 셋팅

		void AllocCandList( const DWORD dwSize );//
		void FreeCandList();

		bool SetIMECandiDateList(const HWND hWnd);//m_lpCL 에 값을 셋팅 한다.
//		bool SetIMEConversionList(const HWND hWnd);//m_lpCL 에 값을 셋팅 한다.

		bool IsCorrectString(const string& strSrc);
		bool IsCanInputAtMultiLine(const string& strSrc, const int nVirtKey);

		void GetWindowText(string &str);

		bool CorrectAlphaNumeric(string &str);
		
		LRESULT	CALLBACK OnWM_IME_SETCONTEXT(const HWND &hWnd, const UINT &message, WPARAM &wParam, LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_COMPOSITION(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);//조합중인 것.
		LRESULT	CALLBACK OnWM_IME_NOTIFY(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_INPUTLANGCHANGEREQUEST(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_INPUTLANGCHANGE(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_STARTCOMPOSITION(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_ENDCOMPOSITION(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_CONTROL(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_CHAR(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
		LRESULT	CALLBACK OnWM_IME_SELECT(const HWND &hWnd, const UINT &message, const WPARAM &wParam, const LPARAM &lParam);
	};
}
//할것. 입력기 숨기기.
//숨겨진 입력기의 글자리스트 뽑기.
//입력에 이상이 없게 하기. 
#endif // !defined(AFX_HSIME_H__17714882_2106_46F5_9F6E_40C38C80E4FF__INCLUDED_)
