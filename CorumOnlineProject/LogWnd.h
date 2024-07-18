// IfxLog2.h: interface for the IfxLog2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFXLOG2_H__E2662C55_E046_11D3_9B19_00A024DB7C85__INCLUDED_)
#define AFX_IFXLOG2_H__E2662C55_E046_11D3_9B19_00A024DB7C85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// [IfxLogWnd2 Handling Document at 20000214 by iskim]
//
// 로그아웃을 사용하려면 다음을 참조하십시오
// 로그아웃의 유형은 세가지입니다.
//
// 1) 일반로그 : 일반적인 목적으로 사용됩니다.
// 2) 경고로그 : 경고성 목적으로 사용됩니다.
// 3) 에러로그 : 에러발생시 사용됩니다.
//
// 로그 2.0을 사용하신다면 다음과 같은 방법으로 사용하십시오.
// A) LOGOUT:일반로그
//		Ex1: LOGOUT(_T("printf의 형식으로 포맷팅하십시오."));
//		Ex2: LOGOUT(_T("%04d : 일반로그입니다."), 100);
//
// B) LOGWAR:경고로그
//		Ex1: LOGWAR(_T("%04d : 경고로그입니다."), 100);
//
// C) LOGERR:에러로그
//		Ex1: LOGERR(_T("%04d : 에러로그입니다."), 100);
//
// 자세한 로그아웃을 위한 덤프를 지원합니다.
// Prepare Ex)
//		TCHAR pDump[2048];
//		::lstrcpy(pDump, _T("이것은 덤프데이터입니다."), 10);
//
// A) DUMPOUT:일반로그
//		Ex1: DUMPOUT(pDump)(_T("%04d : 일반로그입니다."), 100);
//
// B) DUMPWAR:경고로그
//		Ex1: DUMPWAR(pDump)(_T("%04d : 경고로그입니다."), 100);
//
// C) DUMPERR:일반로그
//		Ex1: DUMPERR(pDump)(_T("%04d : 에러로그입니다."), 100);
//
// 더욱 자세한 로그아웃을 지원합니다.
//		GetLastError()의 메시지는 덤프됩니다.
//
// A) LOGOUT_LASTERROR : GetLastError()를 포맷팅합니다.
//		Ex1:
//		try {
//			... some error
//		} catch (...) {
//			LOGOUT_LASTERROR;
//		}
//
//
#include "tchar.h"

static  LPCTSTR    g_logOut                = _T("로그 2.0");
static  LPCTSTR    g_logOutwindowClassName = _T("ISKIM Frameworks LogWnd2");
static  LPCTSTR    g_AppName               = _T("Abyss21^^");

// iType
#define LIT_ADVANCE	0
#define LIT_WARNING	1
#define LIT_ERROR	2

// Buffer Limitation
#define _MAX_FROM		1024
#define _MAX_WHERE		1024
#define _MAX_MESSAGE	2048
#define _MAX_DUMP		2048

typedef struct _LOGITEM {
	int		iType;
	TCHAR	pszFrom[_MAX_FROM];
	TCHAR	pszWhere[_MAX_WHERE];
	TCHAR	pszMessage[_MAX_MESSAGE];
	TCHAR	pszDump[_MAX_DUMP];
} LOGITEM, FAR* LPLOGITEM;

inline void _LogOut (int nType, LPCTSTR pFrom, LPCTSTR pWhere, LPCTSTR pMessage, LPCTSTR pDump)
{
    COPYDATASTRUCT cd; 
    HWND hWnd = ::FindWindow (g_logOutwindowClassName, g_logOut); 
    if (hWnd)
    {  
		LOGITEM li;
		::memset(&li, NULL, sizeof(LOGITEM));

		li.iType = nType;
		if (pFrom)
		{
#ifdef _UNICODE
			::lstrcpynW(li.pszFrom, pFrom, _MAX_FROM);
#else
			::lstrcpyn(li.pszFrom, pFrom, _MAX_FROM);
#endif
		}

		if (pWhere)
		{
#ifdef _UNICODE
			::lstrcpynW(li.pszWhere, pWhere, _MAX_WHERE);
#else
			::lstrcpyn(li.pszWhere, pWhere, _MAX_WHERE);
#endif
		}

		if (pMessage)
		{
#ifdef _UNICODE
			::lstrcpynW(li.pszMessage, pMessage, _MAX_MESSAGE);
#else
			::lstrcpyn(li.pszMessage, pMessage, _MAX_MESSAGE);
#endif
		}

		if (pDump)
		{
#ifdef _UNICODE
			::lstrcpynW(li.pszDump, pDump, _MAX_DUMP);
#else
			::lstrcpyn(li.pszDump, pDump, _MAX_DUMP);
#endif
		}

#ifdef _UNICODE
        cd.dwData = 0xFEFF;
#else
		cd.dwData = 0;
#endif
        cd.cbData = sizeof(LOGITEM);
        cd.lpData = (void *)&li;
        ::SendMessage (hWnd, WM_COPYDATA, 0, (LPARAM)&cd);
    } 
}

inline LPCTSTR LogFormat (LPCTSTR pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);

	static _TCHAR buffer[2048];
	wvsprintf(buffer, pFormat, args);

    va_end(args);

	return buffer;
}

inline void LogOut (int iType, void* pFilename, unsigned int lineno, LPCTSTR pszMessage, LPCTSTR pDump = NULL)
{
    TCHAR where[1024];
	wsprintf(where, _T("%d line in %s"), lineno, pFilename);
    _LogOut (iType, g_AppName , where, pszMessage, pDump);
}

inline DWORD LogOutLastError (void* pFilename, unsigned int lineno, LPCTSTR pszMessage)
{
   if (::GetLastError() == 0) 
        return 0;
   
    LPVOID pDump;
    DWORD  result;
    result = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                             NULL,
                             GetLastError(),
                             MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                             (LPTSTR)&pDump,
                             0,
                             NULL);
  
    ::LogOut(LIT_ADVANCE, pFilename, lineno, pszMessage, (LPCTSTR)pDump);
    
    if(result)
        ::LocalFree(pDump);
   
    return result;
}

class CLogSystem {
public:
	CLogSystem(int nType, void* pFrom, int nLineno, LPCTSTR pDump = NULL)
		: m_pFrom(pFrom), m_nLineno(nLineno), m_nType(nType), m_pDump(pDump), m_nLastError(0)
	{
	}
	CLogSystem(int nType, void* pFrom, int nLineno, int nLastError)
		: m_pFrom(pFrom), m_nLineno(nLineno), m_nType(nType), 
		m_pDump(NULL), m_nLastError(nLastError)
	{
	}

	inline void Trace(LPCTSTR pFormat, ...)
	{
		va_list args;
		va_start(args, pFormat);

		static _TCHAR buffer[2048];
		wvsprintf(buffer, pFormat, args);

		va_end(args);
		LogOut(m_nType, m_pFrom, m_nLineno, buffer, m_pDump);
	}
	inline void TraceLastError()
	{
		if (m_nLastError == 0)
			return;

		static _TCHAR buffer[2048];
		wsprintf(buffer, _T(" %d : the calling thread's last-error code value"), m_nLastError);

		LPVOID pDump;
		DWORD  result;
		result = ::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
								 NULL,
								 m_nLastError,
								 MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
								 (LPTSTR)&pDump,
								 0,
								 NULL);

		::LogOut(LIT_ADVANCE, m_pFrom, m_nLineno, buffer, (LPCTSTR)pDump);

		if(result)
			::LocalFree(pDump);
	}

protected:
	int m_nLastError;
	int m_nType;
	int m_nLineno;
	void* m_pFrom;
	LPCTSTR m_pDump;
};

// LOGOUT
#define LOGFMT	::LogFormat
#define LOGOUT	CLogSystem(LIT_ADVANCE, __FILE__, __LINE__).Trace
#define LOGWAR	CLogSystem(LIT_WARNING, __FILE__, __LINE__).Trace
#define LOGERR	CLogSystem(LIT_ERROR, __FILE__, __LINE__).Trace
#define DUMPOUT(dump)	CLogSystem(LIT_ADVANCE, __FILE__, __LINE__, dump).Trace
#define DUMPWAR(dump)	CLogSystem(LIT_WARNING, __FILE__, __LINE__, dump).Trace
#define DUMPERR(dump)	CLogSystem(LIT_ERROR, __FILE__, __LINE__, dump).Trace
// LOGOUT_LASTERROR
#define LOGOUT_LASTERROR	CLogSystem(LIT_ERROR, __FILE__, __LINE__, GetLastError()).TraceLastError()

#endif // !defined(AFX_IFXLOG2_H__E2662C55_E046_11D3_9B19_00A024DB7C85__INCLUDED_)
