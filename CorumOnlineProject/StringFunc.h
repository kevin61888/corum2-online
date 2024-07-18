#ifndef __STRING_FUNC_H__
#define __STRING_FUNC_H__

#pragma once


// 다국어 지원을 위해 인라인 문자열 비교함수를 제공한다
// by deepdark.

inline int __strcmp(char* szString1, const char* szString2)
{
	return (CompareString(LOCALE_USER_DEFAULT, 0, szString1, lstrlen(szString1), szString2, lstrlen(szString2)) - 2);
}

inline int __stricmp(char* szString1, const char* szString2)
{
	return (CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE|NORM_IGNOREKANATYPE, szString1, lstrlen(szString1), szString2, lstrlen(szString2)) - 2);
}

inline int __strncmp(char* szString1, const char* szString2, int nLength)
{
	return (CompareString(LOCALE_USER_DEFAULT, 0, szString1, nLength, szString2, nLength) - 2);
}

inline int __strnicmp(char* szString1, const char* szString2, int nLength)
{
	return (CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE|NORM_IGNOREKANATYPE, szString1, nLength, szString2, nLength) - 2);
}

inline LPSTR __lstrcpyn(LPSTR lpString1, LPCSTR lpString2, int iMaxLength, BOOL bIgnoreTermination = FALSE)
{
	return (bIgnoreTermination) ? strncpy(lpString1, lpString2, iMaxLength) : lstrcpyn(lpString1, lpString2, iMaxLength+1);
}

inline bool IsEmptyString(char* szString)
{
	return (lstrlen(szString) == 0);
}

#endif // __STRING_FUNC_H__