//==================================================//
// Code by Jang.						2002.10.15	//
// Object : Ini ÆÄÀÏ.								//
//==================================================//
#ifndef		_INI_INC_
#define		_INI_INC_

#include "stdafx.h"

extern BOOL		WritePPS(LPCTSTR lpAppName, LPCTSTR lpString, LPCTSTR lpFileName, BOOL bForceReadOnly = TRUE);
extern BOOL		WritePPSting(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString, LPCTSTR lpFileName, BOOL bForceReadOnly = TRUE);
extern DWORD	GetPPS(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
extern UINT		GetPPI(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault, LPCTSTR lpFileName);

#endif
//==================================================//
// End.												//
//==================================================//
