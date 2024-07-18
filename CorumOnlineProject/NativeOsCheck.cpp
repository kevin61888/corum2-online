#include "stdafx.h"
#include "IMEEdit.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef WINBASEAPI HMODULE (WINAPI *LPLOADLIBRAYFUNC)(LPCSTR );
typedef WINBASEAPI BOOL (WINAPI *LPFREELIBRARYFUNC)(HMODULE );
typedef WINBASEAPI BOOL (WINAPI *LPENUMRESOURCELANGUAGESFUNC)(HMODULE , LPCSTR , LPCSTR , ENUMRESLANGPROC , LONG ); 

LPLOADLIBRAYFUNC g_pFuncLoadLibrary = 0;
LPFREELIBRARYFUNC g_pFuncFreeLibrary = 0;
LPENUMRESOURCELANGUAGESFUNC g_pFuncEnumResourceLanguage = 0;

LONG g_lLangID = 0;

// kernel32.dll
BYTE g_btKernel32DLL[] =
{ 107-15, 101-14,114-13,110-12,101-11,108-10,0x33-9, 0x32-8, 46-7,100-6,108-5,108-4,0};

BOOL CALLBACK EnumResLangProc(HANDLE hModule, LPCSTR lpszType, 
		LPCSTR lpszName, WORD wIDLanguage, LONG lParam)
{
	if(!lpszName) return FALSE;

	g_lLangID = wIDLanguage;

	return TRUE;
}

BOOL SetLoadLibrayFunctionPointer()
{
	BYTE btKernel32DLL2[0xFF] = {0,};
	BYTE btLoadLibrary2[0xFF] = {0,}; 
	BYTE btFreeLibrary2[0xFF] = {0,}; 
	BYTE btEnumResourceLanguages2[0xff] = {0,}; 

	// LoadLibrary
	BYTE btLoadLibrary[] =
	{76-20,111-19,97-18,100-17,76-16,105-15,98-14,114-13,97-12,114-11,121-10, 0x41-9, 0};

	// FreeLibrary
	BYTE btFreeLibrary[] =
	{ 0x46-20, 0x72-19, 0x65-18, 0x65-17, 76-16,105-15,98-14,114-13,97-12,114-11,121-10, 0};

	// EnumResourceLanguages
	BYTE btEnumResourceLanguages[] =
	{0x45-25, 0x6e-24, 0x75-23, 0x6d-22, 0x52-21, 0x65-20, 0x73-19, 0x6f-18, 0x75-17, 0x72-16, 0x63-15, 
	0x65-14, 0x4c-13, 0x61-12, 0x6e-11, 0x67-10, 0x75-9, 0x61-8, 0x67-7, 0x65-6, 0x73-5, 0x41-4, 0};

	for(int i = 0; i < sizeof(g_btKernel32DLL); i++)
	{
		if(g_btKernel32DLL[i] == 0) break;
		btKernel32DLL2[i] = BYTE(g_btKernel32DLL[i] + (15 - i));
	}

	for(i = 0; i < sizeof(btLoadLibrary); i++)
	{
		if(btLoadLibrary[i] == 0) break;
		btLoadLibrary2[i] = BYTE(btLoadLibrary[i] + (20 - i));
	}

	for(i = 0; i < sizeof(btFreeLibrary); i++)
	{
		if(btFreeLibrary[i] == 0) break;
		btFreeLibrary2[i] = BYTE(btFreeLibrary[i] + (20 - i));
	}

	for(i = 0; i < sizeof(btEnumResourceLanguages); i++)
	{
		if(btEnumResourceLanguages[i] == 0) break;
		btEnumResourceLanguages2[i] = BYTE(btEnumResourceLanguages[i] + (25 - i));
	}

	HMODULE hLib = GetModuleHandle((LPCSTR)btKernel32DLL2); // "Kernel32.dll"

	g_pFuncLoadLibrary = (LPLOADLIBRAYFUNC)GetProcAddress(hLib, (LPCSTR)btLoadLibrary2); // "LoadLibraryA" 
	g_pFuncEnumResourceLanguage=(LPENUMRESOURCELANGUAGESFUNC)GetProcAddress(hLib, (LPCSTR)btEnumResourceLanguages2); // "EnumResourceLanguagesA"
	g_pFuncFreeLibrary = (LPFREELIBRARYFUNC)GetProcAddress(hLib, (LPCSTR)btFreeLibrary2); // "FreeLibrary"

	if(!g_pFuncLoadLibrary) return FALSE;
	if(!g_pFuncFreeLibrary) return FALSE;
	if(!g_pFuncEnumResourceLanguage) return FALSE;
	return TRUE;
}

BOOL IsProperNativeOs()
{
#if IS_KOREA_LOCALIZING()
	UINT uCodePage	= CodePage_Korean;
	LONG lLangID	= LanguageCode_Korean;
#elif IS_JAPAN_LOCALIZING()
	UINT uCodePage	= CodePage_Japanese;
	LONG lLangID	= LanguageCode_Japanese;
#elif IS_CHINA_LOCALIZING()
	UINT uCodePage	= CodePage_Chinese_Simplified;
	LONG lLangID	= LanguageCode_Chinese_Prc;
#elif IS_TAIWAN_LOCALIZING()
	UINT uCodePage	= CodePage_Chinese_Traditional;
	LONG lLangID	= LanguageCode_Chinese_Taiwan;
#else
	UINT uCodePage	= CodePage_USA_English;
	LONG lLangID	= 0;
#endif
	
	if(g_pFuncLoadLibrary == NULL) return FALSE;
	if(g_pFuncFreeLibrary == NULL) return FALSE;
	if(g_pFuncEnumResourceLanguage == NULL) return FALSE;

	BYTE btKernel32DLL2[0xFF] = {0,};

	for(int i = 0; i < sizeof(g_btKernel32DLL); i++)
	{
		if(g_btKernel32DLL[i] == 0) break;
		btKernel32DLL2[i] = BYTE(g_btKernel32DLL[i] + (15 - i));
	}

	HMODULE hLib = (*g_pFuncLoadLibrary)((LPCSTR)btKernel32DLL2); // "Kernel32.dll"
	if(hLib == NULL) return FALSE;

	(*g_pFuncEnumResourceLanguage)(hLib, RT_VERSION, MAKEINTRESOURCE(1), (ENUMRESLANGPROC)EnumResLangProc, NULL);

	(*g_pFuncFreeLibrary)(hLib);

	if(g_lLangID != lLangID) return FALSE;

//--------------------------------------------
	if(GetACP() != uCodePage) return FALSE;

	char szCodePage[0xff];
	itoa(uCodePage, szCodePage, 10);

	char szLCData[0xff];
	LCID localeid = GetSystemDefaultLCID();
	GetLocaleInfo(localeid, LOCALE_IDEFAULTANSICODEPAGE, szLCData, 0xff);
	
	if(__strcmp(szCodePage, szLCData) != 0) return FALSE;

	return TRUE;
}
