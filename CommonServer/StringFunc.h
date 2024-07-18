#ifndef __STRING_FUNC_H__
#define __STRING_FUNC_H__


#pragma once


#include "CommonServer.h"


// 다국어 지원을 위해 인라인 문자열 비교함수를 제공한다


inline int DECLSPECIFIER __strcmp(const char* szString1, const char* szString2)
{
	return (CompareString(LOCALE_USER_DEFAULT, 0, szString1, lstrlen(szString1), szString2, lstrlen(szString2)) - 2);
}

inline int DECLSPECIFIER __stricmp(const char* szString1, const char* szString2)
{
	return (CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE|NORM_IGNOREKANATYPE|NORM_IGNOREWIDTH, szString1, lstrlen(szString1), szString2, lstrlen(szString2)) - 2);
}

inline int DECLSPECIFIER __strncmp(const char* szString1, const char* szString2, int nLength)
{
	return (CompareString(LOCALE_USER_DEFAULT, 0, szString1, nLength, szString2, nLength) - 2);
}

inline int DECLSPECIFIER __strnicmp(const char* szString1, const char* szString2, int nLength)
{
	return (CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE|NORM_IGNOREKANATYPE|NORM_IGNOREWIDTH, szString1, nLength, szString2, nLength) - 2);
}

inline LPSTR DECLSPECIFIER __lstrcpyn(const LPSTR lpString1, const LPSTR lpString2, int iMaxLength)
{
	return lstrcpyn(lpString1, lpString2, iMaxLength+1);
}

inline bool DECLSPECIFIER IsEmptyString(const char* szString)
{
	return (lstrlen(szString) == 0);
}


#endif 