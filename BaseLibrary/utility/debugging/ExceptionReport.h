#pragma once

#define USE_MINIDUMP		1
#define USE_DBGHELPS		0
#define PRINT_SYSTEMINFO	1

#include <dbghelp.h>
#include <cctype>
#include "DLLModule.h"
#include "DebugUtils.h"
#if PRINT_SYSTEMINFO == 1
#include "SystemInfo.h"
#endif

#if USE_DBGHELPS == 0
#pragma comment(lib, "dbghelp.lib")
#endif

class CDBGFuncClass : public CDLLModule
{
public :
	CDBGFuncClass(void);
	virtual ~CDBGFuncClass(void);

	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, MiniDumpWriteDump, (HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION, CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION))
	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, SymEnumSymbols, (HANDLE, ULONG64, PCSTR, PSYM_ENUMERATESYMBOLS_CALLBACK, PVOID))
	DECLARE_DLL_FUNCTION(ULONG,  WINAPI, SymSetContext, (HANDLE, PIMAGEHLP_STACK_FRAME, PIMAGEHLP_CONTEXT))
	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, SymFromAddr, (HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO))
	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, StackWalk, (DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID, PREAD_PROCESS_MEMORY_ROUTINE, PFUNCTION_TABLE_ACCESS_ROUTINE, PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE))
//	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, StackWalk64, (DWORD, HANDLE, HANDLE, LPSTACKFRAME64, PVOID, PREAD_PROCESS_MEMORY_ROUTINE64, PFUNCTION_TABLE_ACCESS_ROUTINE64, PGET_MODULE_BASE_ROUTINE64, PTRANSLATE_ADDRESS_ROUTINE64))
 	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, SymGetLineFromAddr, (HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE))		// ImageHlp
	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, SymGetLineFromAddr64, (HANDLE, DWORD64, PDWORD, PIMAGEHLP_LINE64))
	DECLARE_DLL_FUNCTION(PVOID,  WINAPI, SymFunctionTableAccess, (HANDLE, DWORD))								// ImageHlp
//	DECLARE_DLL_FUNCTION(PVOID,  WINAPI, SymFunctionTableAccess64, (HANDLE, DWORD64))
	DECLARE_DLL_FUNCTION(DWORD64,WINAPI, SymGetModuleBase64, (HANDLE, DWORD64))
	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, SymInitialize, (HANDLE, PSTR, BOOL))
	DECLARE_DLL_FUNCTION(DWORD,  WINAPI, SymSetOptions, (DWORD))
	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, SymCleanup, (HANDLE))
	DECLARE_DLL_FUNCTION(DWORD,  WINAPI, SymGetModuleBase, (HANDLE, DWORD))
//	DECLARE_DLL_FUNCTION(DWORD64,WINAPI, SymGetModuleBase64, (HANDLE, DWORD64))
	DECLARE_DLL_FUNCTION(BOOL,   WINAPI, SymGetTypeInfo, (HANDLE, DWORD64, ULONG, IMAGEHLP_SYMBOL_TYPE_INFO, PVOID))

public:
//	BEGIN_DLL_INIT()
//	INIT_DLL_FUNCTION(BOOL,   WINAPI, MiniDumpWriteDump, (HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION, CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION))
//	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymEnumSymbols, (HANDLE, ULONG64, PCSTR, PSYM_ENUMERATESYMBOLS_CALLBACK, PVOID))
//	INIT_DLL_FUNCTION(ULONG,  WINAPI, SymSetContext, (HANDLE, PIMAGEHLP_STACK_FRAME, PIMAGEHLP_CONTEXT))
//	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymFromAddr, (HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO))
//	INIT_DLL_FUNCTION(BOOL,   WINAPI, StackWalk, (DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID, PREAD_PROCESS_MEMORY_ROUTINE, PFUNCTION_TABLE_ACCESS_ROUTINE, PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE))
////	INIT_DLL_FUNCTION(BOOL,   WINAPI, StackWalk64, (DWORD, HANDLE, HANDLE, LPSTACKFRAME64, PVOID, PREAD_PROCESS_MEMORY_ROUTINE64, PFUNCTION_TABLE_ACCESS_ROUTINE64, PGET_MODULE_BASE_ROUTINE64, PTRANSLATE_ADDRESS_ROUTINE64))
// 	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymGetLineFromAddr, (HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE))		// ImageHlp
//	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymGetLineFromAddr64, (HANDLE, DWORD64, PDWORD, PIMAGEHLP_LINE64))
//	INIT_DLL_FUNCTION(PVOID,  WINAPI, SymFunctionTableAccess, (HANDLE, DWORD))
////	INIT_DLL_FUNCTION(PVOID,  WINAPI, SymFunctionTableAccess64, (HANDLE, DWORD64))
//	INIT_DLL_FUNCTION(DWORD64,WINAPI, SymGetModuleBase64, (HANDLE, DWORD64))
//	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymInitialize, (HANDLE, PSTR, BOOL))
//	INIT_DLL_FUNCTION(DWORD,  WINAPI, SymSetOptions, (DWORD))
//	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymCleanup, (HANDLE))
//	INIT_DLL_FUNCTION(DWORD,  WINAPI, SymGetModuleBase, (HANDLE, DWORD))
////	INIT_DLL_FUNCTION(DWORD64,WINAPI, SymGetModuleBase64, (HANDLE, DWORD64))
//	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymGetTypeInfo, (HANDLE, DWORD64, ULONG, IMAGEHLP_SYMBOL_TYPE_INFO, PVOID))
//	END_DLL_INIT()

	BOOL		Init(LPCTSTR szDll);
};

enum BasicType  // Stolen from CVCONST.H in the DIA 2.0 SDK
{
    btNoType = 0,
    btVoid = 1,
    btChar = 2,
    btWChar = 3,
    btInt = 6,
    btUInt = 7,
    btFloat = 8,
    btBCD = 9,
    btBool = 10,
    btLong = 13,
    btULong = 14,
    btCurrency = 25,
    btDate = 26,
    btVariant = 27,
    btComplex = 28,
    btBit = 29,
    btBSTR = 30,
    btHresult = 31
};

typedef LONG (WINAPI *PCERUserFunc)(HANDLE hFile);
typedef PCERUserFunc LPCERUserFunc;

class CExceptionReport
{
public:
	// constructor and destructor
	CExceptionReport(void);
	~CExceptionReport(void);

	static void			SetProgramName(PTSTR pszProgamName = NULL);
	static void			SetExceptionOption( BOOL bFullMemoey ){	m_bFullMemory=bFullMemoey;	}
	static void			SetUserFunc(LPCERUserFunc lpUserFunc);
	
	static LONG WINAPI	UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo);
	static LONG WINAPI	UnhandledExceptionFilterTwo(PEXCEPTION_POINTERS pExceptionInfo);

    static int __cdecl	Printf(const TCHAR * format, ...);

#if PRINT_SYSTEMINFO == 1
	CSystemInfo							m_SystemInfo;
#endif

private:

	static void			WriteBasicInfo(PEXCEPTION_RECORD pExceptionRecord);
	static void			WriteExceptionReport(PEXCEPTION_POINTERS pExceptionInfo);
    static void			WriteStackDetails(PCONTEXT pContext, BOOL bWriteVariables);
    static void			WriteRegistersInfo(PCONTEXT pContext);
	static void			WriteMemoryDump(PCONTEXT pContext);

	static void			StoreCoreDump(void);
	static void			Dump(DWORD64 pData, DWORD dwSize, BOOL bAlign);

    static LPTSTR		GetExceptionString(DWORD dwCode);
    static BOOL			GetLogicalAddress( PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset);
    static BOOL			CALLBACK EnumerateSymbolsCallback(PSYMBOL_INFO,ULONG, PVOID);
    static BOOL			FormatSymbolValue(PSYMBOL_INFO, STACKFRAME *, char * pszBuffer, unsigned cbBuffer);
    static char*		DumpTypeIndex(char *, DWORD64, DWORD, unsigned, DWORD , BOOL &);
    static char*		FormatOutputValue(char * pszCurrBuffer, BasicType basicType, DWORD64 length, PVOID pAddress);
    static BasicType	GetBasicType(DWORD typeIndex, DWORD64 modBase);

	static inline void	Hex64ToStr(char *szDest, DWORD64 hex);

	//

	static CDBGFuncClass*				m_pDbgHelp;
	static LPCERUserFunc				m_lpUserFunc;

	static LPTOP_LEVEL_EXCEPTION_FILTER	m_OldFilter;

	static TCHAR						m_szLogPrefixName[MAX_PATH];
	static TCHAR						m_szModuleName[MAX_PATH];

	static HANDLE						m_hReportFile;
	static HANDLE						m_hProcess;
	static BOOL							m_bHasSymbol;

	static BOOL							m_bFullMemory;

	enum { BYTES_PER_LINE = 16, SHIFT_NUM = 4 };
	inline int IsPrint(int n) { return (isprint(n) || ((n) >= 0xA1) && ((n) <= 0xFE)); }
};

inline void CExceptionReport::SetProgramName(PTSTR pszProgamName) {
	DbgUtils::SetProgramName(m_szLogPrefixName, MAX_PATH, NULL);
}

extern CExceptionReport g_CExceptionReport;
