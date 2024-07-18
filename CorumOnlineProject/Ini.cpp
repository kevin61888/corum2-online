//==================================================//
// Code by Jang.						2002.10.15	//
// Modified by deepdark for readonly ini file...    //
//==================================================//
#include "Ini.h"
//==================================================//
// Extern Function.									//
//==================================================//
BOOL WritePPSection(LPCTSTR lpAppName, LPCTSTR lpString, LPCTSTR lpFileName, BOOL bForceReadOnly)
{
	// ini화일에 해당 키에 대한 정보를 기록합니다.
	// 섹션과 키가 없으면 자동적으로 추가합니다. 
	// "키=값"의 형식으로 되어있는 문자열 데이타 아닌때 사용.
	// 파라미터 : 섹션명, 문자열, ini 화일명(full-path로 지정해야함) 

	if(!WritePrivateProfileSection(lpAppName, lpString, lpFileName))
	{
		if(bForceReadOnly && (ERROR_ACCESS_DENIED == GetLastError()))
		{
			SetFileAttributes(lpFileName, FILE_ATTRIBUTE_NORMAL);
			return WritePrivateProfileSection(lpAppName, lpString, lpFileName);
		}

		return FALSE;
	}

	return TRUE;
}

BOOL WritePPSting(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName, BOOL bForceReadOnly)
{
	// ini화일에 해당 키에 대한 정보를 기록합니다.
	// 섹션과 키가 없으면 자동적으로 추가합니다. 
	// "키=값"의 형식으로 되어있는 문자열 데이타.
	// 파라미터 : 섹션명, 키이름, 키값, ini 화일명(full-path로 지정해야함) 
	if(!WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName))
	{
		if(bForceReadOnly && (ERROR_ACCESS_DENIED == GetLastError()))
		{
			SetFileAttributes(lpFileName, FILE_ATTRIBUTE_NORMAL);
			return WritePrivateProfileString(lpAppName, lpKeyName, lpString, lpFileName);
		}

		return FALSE;
	}

	return TRUE;
}

DWORD GetPPS(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, 
				LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName)
{
	// ini화일로부터 문자열 타입의 키값을 읽어옵니다.
	// 파라미터 : 섹션명, 키 이름, 키를 발견하지 못했을 경우 반환되는 값,
	// 값을 담을 버퍼, 값을 담을 버퍼의 최대길이, ini 화일명(full-path로 지정해야함)
	return GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
}

UINT GetPPI(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName)
{
	// ini화일로부터 정수 타입의 키값을 읽어옵니다. 
	// 섹션명, 키 이름, 만일 키가 발견되지 않으면 반환되는 값,
	// ini 화일명(full-path로 지정해야함) 
	return GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, lpFileName);
}
//==================================================//
// End.												//
//==================================================//
