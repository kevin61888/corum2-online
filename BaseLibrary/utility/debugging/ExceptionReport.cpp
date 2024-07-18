/* ==========================================================================
  File : ExceptionReport.h
  Vers.: 0.9
  Plat.: Windows 98 or above
  Desc.: ExceptionHandle routine (report)
  Req. : DbgHelp v5.1 or above

  Adapted from MSDN March 2002 - Under the Hood
========================================================================== */

// #define _WIN32_WINDOWS	0x0410		// Windows 98
// #define _WIN32_WINNT		0x0500		// Windows 2000

#define USE_DBGHELPS	0

#include "../../StdAfx.h"
#include "ExceptionReport.h"

#if USE_DBGHELPS == 1
#include "DBGHELPS.H"
#endif

CExceptionReport				g_CExceptionReport;

CDBGFuncClass*					CExceptionReport::m_pDbgHelp;
LPCERUserFunc					CExceptionReport::m_lpUserFunc;

TCHAR							CExceptionReport::m_szLogPrefixName[MAX_PATH];
TCHAR							CExceptionReport::m_szModuleName[MAX_PATH];

LPTOP_LEVEL_EXCEPTION_FILTER	CExceptionReport::m_OldFilter;
HANDLE							CExceptionReport::m_hReportFile;
HANDLE							CExceptionReport::m_hProcess;
BOOL							CExceptionReport::m_bHasSymbol;
BOOL							CExceptionReport::m_bFullMemory;


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CDBGFuncClass::CDBGFuncClass(void)
{
}

CDBGFuncClass::~CDBGFuncClass(void)
{
}

// initiate
BOOL CDBGFuncClass::Init(LPCTSTR szDll)
{
	if (m_hDLL)
		::FreeLibrary(m_hDLL);

	m_hDLL = ::LoadLibrary(szDll);
	BOOL	bOk = FALSE;

	INIT_DLL_FUNCTION(BOOL,   WINAPI, MiniDumpWriteDump, (HANDLE, DWORD, HANDLE, MINIDUMP_TYPE, CONST PMINIDUMP_EXCEPTION_INFORMATION, CONST PMINIDUMP_USER_STREAM_INFORMATION, CONST PMINIDUMP_CALLBACK_INFORMATION))

	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymEnumSymbols, (HANDLE, ULONG64, PCSTR, PSYM_ENUMERATESYMBOLS_CALLBACK, PVOID))
	INIT_DLL_FUNCTION(ULONG,  WINAPI, SymSetContext, (HANDLE, PIMAGEHLP_STACK_FRAME, PIMAGEHLP_CONTEXT))
	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymFromAddr, (HANDLE, DWORD64, PDWORD64, PSYMBOL_INFO))
	INIT_DLL_FUNCTION(BOOL,   WINAPI, StackWalk, (DWORD, HANDLE, HANDLE, LPSTACKFRAME, PVOID, PREAD_PROCESS_MEMORY_ROUTINE, PFUNCTION_TABLE_ACCESS_ROUTINE, PGET_MODULE_BASE_ROUTINE, PTRANSLATE_ADDRESS_ROUTINE))
	//INIT_DLL_FUNCTION(BOOL,   WINAPI, StackWalk64, (DWORD, HANDLE, HANDLE, LPSTACKFRAME64, PVOID, PREAD_PROCESS_MEMORY_ROUTINE64, PFUNCTION_TABLE_ACCESS_ROUTINE64, PGET_MODULE_BASE_ROUTINE64, PTRANSLATE_ADDRESS_ROUTINE64))
 	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymGetLineFromAddr, (HANDLE, DWORD, PDWORD, PIMAGEHLP_LINE))		// ImageHlp
	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymGetLineFromAddr64, (HANDLE, DWORD64, PDWORD, PIMAGEHLP_LINE64))
	INIT_DLL_FUNCTION(PVOID,  WINAPI, SymFunctionTableAccess, (HANDLE, DWORD))
	//INIT_DLL_FUNCTION(PVOID,  WINAPI, SymFunctionTableAccess64, (HANDLE, DWORD64))
	INIT_DLL_FUNCTION(DWORD64,WINAPI, SymGetModuleBase64, (HANDLE, DWORD64))
	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymInitialize, (HANDLE, PSTR, BOOL))
	INIT_DLL_FUNCTION(DWORD,  WINAPI, SymSetOptions, (DWORD))
	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymCleanup, (HANDLE))
	INIT_DLL_FUNCTION(DWORD,  WINAPI, SymGetModuleBase, (HANDLE, DWORD))
	//INIT_DLL_FUNCTION(DWORD64,WINAPI, SymGetModuleBase64, (HANDLE, DWORD64))
	INIT_DLL_FUNCTION(BOOL,   WINAPI, SymGetTypeInfo, (HANDLE, DWORD64, ULONG, IMAGEHLP_SYMBOL_TYPE_INFO, PVOID))

	return bOk;
} 

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructor and destructor
CExceptionReport::CExceptionReport(void)
{
	m_pDbgHelp	= new CDBGFuncClass;
	if (NULL == m_pDbgHelp)
		assert(false && "DBGFuncClass 생성 실패");

	m_bHasSymbol = FALSE;
	SetProgramName();
	SetUserFunc(NULL);

	m_bFullMemory = FALSE;

#if USE_DBGHELPS == 1
	DBGHELP_DllMain(GetModuleHandle(NULL), DLL_PROCESS_ATTACH, NULL);
#else
	if (TRUE == m_pDbgHelp->Init("DBGHELP.DLL"))
#endif
		// Install the unhandled exception filter function
		m_OldFilter = SetUnhandledExceptionFilter(UnhandledExceptionFilter);

	m_hProcess = GetCurrentProcess();
}

CExceptionReport::~CExceptionReport(void)
{
	SetUnhandledExceptionFilter(m_OldFilter);
#if USE_DBGHELPS == 1
	DBGHELP_DllMain(GetModuleHandle(NULL), DLL_PROCESS_DETACH, NULL);
#endif

	SAFE_DELETE(m_pDbgHelp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Set application-specific function
void CExceptionReport::SetUserFunc(LPCERUserFunc lpUserFunc)
{
	m_lpUserFunc = lpUserFunc;
}

LONG WINAPI CExceptionReport::UnhandledExceptionFilterTwo(PEXCEPTION_POINTERS pExceptionInfo)
{
	if (m_hReportFile) {
		Printf("Exception occured AGAIN! EXITING NOW!\r\n");
		CloseHandle(m_hReportFile);
		m_hReportFile = NULL;
	}
	if (m_OldFilter)
		return m_OldFilter(pExceptionInfo);
	else
		return EXCEPTION_CONTINUE_SEARCH;
}

// This is called when unhandled exception occurs
LONG WINAPI CExceptionReport::UnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	TCHAR szLogFileName[ MAX_PATH ];

    SetUnhandledExceptionFilter(UnhandledExceptionFilterTwo);

	_tcscpy(szLogFileName, m_szLogPrefixName);

    SYSTEMTIME stSystemTime;
	GetLocalTime(&stSystemTime);

#if USE_MINIDUMP == 1
	TCHAR szDumpFileName[ MAX_PATH ];

	_tcscpy(szDumpFileName, m_szLogPrefixName);
	wsprintf(szDumpFileName + strlen(szDumpFileName), " %04d-%02d-%02d %02d,%02d,%02d.DMP", 
		stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay,
		stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	HANDLE hDumpFile = CreateFile(szDumpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 
								  FILE_ATTRIBUTE_NORMAL, 0);

	if (m_pDbgHelp->MiniDumpWriteDump && INVALID_HANDLE_VALUE != hDumpFile) {
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;


		// 2004.04.27. SSuReBBa modifid.
		if( m_bFullMemory )
			m_pDbgHelp->MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithFullMemory , &ExInfo, NULL, NULL);
		else
			m_pDbgHelp->MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithDataSegs , &ExInfo, NULL, NULL);

		//2004.3.22. jaewon modified.
//		m_pDbgHelp->MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithFullMemory , &ExInfo, NULL, NULL);
//		MiniDumpWithDataSegs
//		MiniDumpWithFullMemory

		CloseHandle(hDumpFile);
	}
#endif

	wsprintf(szLogFileName + strlen(szLogFileName), " %04d-%02d-%02d %02d,%02d,%02d.TXT", 
			 stSystemTime.wYear, stSystemTime.wMonth, stSystemTime.wDay,
			 stSystemTime.wHour, stSystemTime.wMinute, stSystemTime.wSecond);

	m_hReportFile = CreateFile(szLogFileName, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, 0);

	if (INVALID_HANDLE_VALUE != m_hReportFile) {
		SetFilePointer(m_hReportFile, 0, 0, FILE_END);
		WriteExceptionReport(pExceptionInfo);
		CloseHandle(m_hReportFile);
		m_hReportFile = NULL;
	}

	m_hReportFile = CreateFile(szLogFileName, GENERIC_WRITE, 0, 
		0, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, 0);

	if (m_lpUserFunc && INVALID_HANDLE_VALUE != m_hReportFile) {
		SetFilePointer(m_hReportFile, 0, 0, FILE_END);
		Printf(_T("------------------------------------------------------------------------------\r\n"));
	    Printf(_T("    Application-specific log\r\n"));
		Printf(_T("------------------------------------------------------------------------------\r\n"));
		Printf(_T("\r\n"));
		m_lpUserFunc(m_hReportFile);
		Printf(_T("\r\n"));
		Printf(_T("\r\n"));
		Printf(_T("==============================================================================\r\n"));
		CloseHandle(m_hReportFile);
		m_hReportFile = NULL;
	}

//	if (m_OldFilter)
//		return m_OldFilter(pExceptionInfo);
//	else
//		return EXCEPTION_CONTINUE_SEARCH;
	return EXCEPTION_EXECUTE_HANDLER;
}

// Write informations to Report file. Called by UnhandledExceptionFilter
void CExceptionReport::WriteExceptionReport(PEXCEPTION_POINTERS pExceptionInfo)
{
    // Start out with a banner
    Printf(_T("==============================================================================\r\n\r\n"));
	WriteBasicInfo(pExceptionInfo->ExceptionRecord);
	WriteRegistersInfo(pExceptionInfo->ContextRecord);

	if (!m_pDbgHelp->SymInitialize)
		return;

//	if (m_pDbgHelp->SymSetOptions)
//		m_pDbgHelp->SymSetOptions(SYMOPT_DEFERRED_LOADS);
    // Initialize DbgHelp
    if (!m_pDbgHelp->SymInitialize(GetCurrentProcess(), 0, TRUE))
        return;
    CONTEXT trashableContext = *(pExceptionInfo->ContextRecord);
    WriteStackDetails(&trashableContext, FALSE);
#ifdef _M_IX86  // X86 Only!
	if (m_bHasSymbol) {
		trashableContext = *(pExceptionInfo->ContextRecord);
		WriteStackDetails(&trashableContext, TRUE);
//		Printf(_T("------------------------------------------------------------------------------\r\n"));
//	    Printf(_T("    Global Variables\r\n"));
//		Printf(_T("------------------------------------------------------------------------------\r\n"));
//		Printf(_T("\r\n"));
//		if (m_pDbgHelp->SymEnumSymbols)
//		    m_pDbgHelp->SymEnumSymbols(GetCurrentProcess(), (DWORD64) GetModuleHandle(m_szModuleName), 0, EnumerateSymbolsCallback, 0);
	}
#endif      // X86 Only!
	if (m_pDbgHelp->SymCleanup)
		m_pDbgHelp->SymCleanup(GetCurrentProcess());

	Printf(_T("\r\n"));
    Printf(_T("\r\n"));

	WriteMemoryDump(pExceptionInfo->ContextRecord);

	if (m_lpUserFunc) {
		Printf(_T("------------------------------------------------------------------------------\r\n"));
	    Printf(_T("    Application-specific log\r\n"));
		Printf(_T("------------------------------------------------------------------------------\r\n"));
		Printf(_T("\r\n"));

		m_lpUserFunc(m_hReportFile);
		Printf(_T("\r\n"));
		Printf(_T("\r\n"));
	}

	if (!m_lpUserFunc)
		Printf(_T("==============================================================================\r\n"));
}

// Write basic informations(user, computer name, error type)
void CExceptionReport::WriteBasicInfo(PEXCEPTION_RECORD pExceptionRecord)
{
	TCHAR szFileName[MAX_PATH] = _T(""), szUserName[MAX_PATH] = _T(""), szComputerName[MAX_PATH] = _T("");
	DWORD dwUserLen = MAX_PATH, dwComputerLen = MAX_PATH;

	SYSTEMTIME stSystemTime;
	GetLocalTime(&stSystemTime);

    GetModuleFileName((HMODULE) NULL, szFileName, MAX_PATH);
	GetUserName(szUserName, &dwUserLen);
	GetComputerName(szComputerName, &dwComputerLen);

	Printf(_T("------------------------------------------------------------------------------\r\n"));
    Printf(_T("    Basic Information\r\n"));
	Printf(_T("------------------------------------------------------------------------------\r\n"));
	Printf(_T("\r\n"));

	Printf(_T("Program Name : %s\r\n"), m_szLogPrefixName);
	Printf(_T("EXE          : %s\r\n"), szFileName);
	Printf(_T("User         : %s\r\n"), szUserName);
	Printf(_T("Computer     : %s\r\n"), szComputerName);
	Printf(_T("\r\n"));

#if PRINT_SYSTEMINFO == 1
	Printf(_T("OS           : %s %s\r\n"), g_CExceptionReport.m_SystemInfo.GetOSTypeName(), g_CExceptionReport.m_SystemInfo.GetOSVersionName() );
	Printf(_T("Processor    : %s %s\r\n"), g_CExceptionReport.m_SystemInfo.GetCPUIDVendorName(), g_CExceptionReport.m_SystemInfo.GetCPUIDProductName() );
	Printf(_T("Disk         : %s\r\n"), g_CExceptionReport.m_SystemInfo.GetDiskSpaceInfo() );
	Printf(_T("\r\n"));
#endif

	// Print information about the type of fault and where the fault occured
    DWORD section, offset;
    GetLogicalAddress( pExceptionRecord->ExceptionAddress, m_szModuleName, sizeof(m_szModuleName), section, offset);
	Printf(_T("Program      : %s\r\n"), m_szModuleName);
    Printf(_T("Exception    : %08X (%s)\r\n"), pExceptionRecord->ExceptionCode, GetExceptionString(pExceptionRecord->ExceptionCode));
    Printf(_T("Fault Address: %08X %02X:%08X\r\n"), pExceptionRecord->ExceptionAddress, section, offset);
    Printf(_T("\r\n"));
	Printf(_T("\r\n"));

	// TODO: System Information!
}

// Show the registers
void CExceptionReport::WriteRegistersInfo(PCONTEXT pContext)
{
#ifdef _M_IX86  // X86 Only!
	Printf(_T("------------------------------------------------------------------------------\r\n"));
    Printf(_T("    x86 Registers\r\n"));
	Printf(_T("------------------------------------------------------------------------------\r\n"));
	Printf(_T("\r\n"));

    Printf(_T("EAX=%08X  EBX=%08X  ECX=%08X  EDX=%08X\r\n"), pContext->Eax, pContext->Ebx, pContext->Ecx, pContext->Edx);
	Printf(_T("ESI=%08X  EDI=%08X  EBP=%08X\r\n"), pContext->Esi, pContext->Edi, pContext->Ebp);
    Printf(_T("DS =%04X      ES=%04X       FS=%04X       GS:%04X\r\n"), pContext->SegDs, pContext->SegEs, pContext->SegFs, pContext->SegGs);
    Printf(_T("CS:EIP=%04X:%08X\r\n"), pContext->SegCs, pContext->Eip);
    Printf(_T("SS:ESP=%04X:%08X\r\n"), pContext->SegSs, pContext->Esp);
    Printf(_T("Flags=%08X\r\n"), pContext->EFlags);
    Printf(_T("\r\n"));
    Printf(_T("\r\n"));
#endif
}

// Show the Memory
void CExceptionReport::WriteMemoryDump(PCONTEXT pContext)
{
	Printf(_T("------------------------------------------------------------------------------\r\n"));
    Printf(_T("    Memory Dump\r\n"));
	Printf(_T("------------------------------------------------------------------------------\r\n"));
	Printf(_T("\r\n"));

	Printf(_T("Code: %d bytes starting at (EIP = %08lX)\r\n"), 16, pContext->Eip);
	Dump(pContext->Eip, 16, FALSE);
	Printf(_T("\r\n"));

	Printf(_T("Stack: %d bytes starting at (ESP = %08lX)\r\n"), 1024, pContext->Esp);
	Dump(pContext->Esp, 1024, TRUE);
	Printf(_T("\r\n"));
    Printf(_T("\r\n"));
}

// Given an exception code, returns a pointer to a static string with a description of the exception                                         
#define EXCEPTION(x) case EXCEPTION_##x: return _T(#x);

LPTSTR CExceptionReport::GetExceptionString(DWORD dwCode)
{
	switch (dwCode) {
		EXCEPTION(ACCESS_VIOLATION)
		EXCEPTION(DATATYPE_MISALIGNMENT)
		EXCEPTION(BREAKPOINT)
		EXCEPTION(SINGLE_STEP)
		EXCEPTION(ARRAY_BOUNDS_EXCEEDED)
		EXCEPTION(FLT_DENORMAL_OPERAND)
		EXCEPTION(FLT_DIVIDE_BY_ZERO)
		EXCEPTION(FLT_INEXACT_RESULT)
		EXCEPTION(FLT_INVALID_OPERATION)
		EXCEPTION(FLT_OVERFLOW)
		EXCEPTION(FLT_STACK_CHECK)
		EXCEPTION(FLT_UNDERFLOW)
		EXCEPTION(INT_DIVIDE_BY_ZERO)
		EXCEPTION(INT_OVERFLOW)
		EXCEPTION(PRIV_INSTRUCTION)
		EXCEPTION(IN_PAGE_ERROR)
		EXCEPTION(ILLEGAL_INSTRUCTION)
		EXCEPTION(NONCONTINUABLE_EXCEPTION)
		EXCEPTION(STACK_OVERFLOW)
		EXCEPTION(INVALID_DISPOSITION)
		EXCEPTION(GUARD_PAGE)
		EXCEPTION(INVALID_HANDLE)
	}

	// If not one of the "known" exceptions, try to get the string
	// from NTDLL.DLL's message table.

	static TCHAR szBuffer[512] = { 0 };

	FormatMessage(FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE, GetModuleHandle(_T("NTDLL.DLL")),
				  dwCode, 0, szBuffer, sizeof(szBuffer), 0);

	return szBuffer;
}

#undef EXCEPTION

// Given a linear address, locates the module, section, and offset containing that address.                                                               
// Note: the szModule paramater buffer is an output buffer of length specified by the len parameter (in characters!)                                       
BOOL CExceptionReport::GetLogicalAddress(PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset)
{
    MEMORY_BASIC_INFORMATION mbi;

	if (!VirtualQuery(addr, &mbi, sizeof(mbi))) {
        return FALSE;
	}

    PVOID hMod = mbi.AllocationBase;
	if (NULL == hMod) {
		return FALSE;
	}

	if (!GetModuleFileName((HMODULE) hMod, szModule, len)) {
		return FALSE;
	}

    // Point to the DOS header in memory
    PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER) hMod;

    // From the DOS header, find the NT (PE) header
    PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(((DWORD64) hMod) + pDosHdr->e_lfanew);

    PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION(pNtHdr);

    DWORD64 rva = (DWORD64)addr - (DWORD64)hMod; // RVA is offset from module load address

    // Iterate through the section table, looking for the one that encompasses
    // the linear address.
    for (unsigned i = 0; i < pNtHdr->FileHeader.NumberOfSections; i++, pSection++) {
        DWORD sectionStart = pSection->VirtualAddress;
        DWORD sectionEnd = sectionStart + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

        // Is the address in this section???
        if ((rva >= sectionStart) && (rva <= sectionEnd)) {
            // Yes, address is in the section.  Calculate section and offset,
            // and store in the "section" & "offset" params, which were
            // passed by reference.
            section = i+1;
            offset = (DWORD) (rva - sectionStart);
            return TRUE;
        }
    }

    return FALSE;   // Should never get here!
}

//
// Walks the stack, and writes the results to the report file 
//
void CExceptionReport::WriteStackDetails(PCONTEXT pContext, BOOL bWriteVariables)  // TRUE if local/params should be output
{
	Printf(_T("------------------------------------------------------------------------------\r\n"));
	Printf(_T("    Call Stack (%s)\r\n"), bWriteVariables ? "Detail" : "Short");
	Printf(_T("------------------------------------------------------------------------------\r\n"));
	Printf(_T("\r\n"));
    Printf(_T("Address   Frame     Function                        SourceFile\r\n"));
    DWORD dwMachineType = 0;
    // Could use SymSetOptions here to add the SYMOPT_DEFERRED_LOADS flag
    STACKFRAME sf;

   memset(&sf, 0, sizeof(sf));

#ifdef _M_IX86
    // Initialize the STACKFRAME structure for the first call.  This is only
    // necessary for Intel CPUs, and isn't mentioned in the documentation.
    sf.AddrPC.Offset       = pContext->Eip;
    sf.AddrPC.Mode         = AddrModeFlat;
    sf.AddrStack.Offset    = pContext->Esp;
    sf.AddrStack.Mode      = AddrModeFlat;
    sf.AddrFrame.Offset    = pContext->Ebp;
    sf.AddrFrame.Mode      = AddrModeFlat;

    dwMachineType = IMAGE_FILE_MACHINE_I386;
#endif
    while(1) {
        // Get the next stack frame
		if (!m_pDbgHelp->StackWalk)
			break;
        if (!m_pDbgHelp->StackWalk(dwMachineType, m_hProcess, GetCurrentThread(), &sf, pContext, 0, m_pDbgHelp->SymFunctionTableAccess, m_pDbgHelp->SymGetModuleBase, 0))
            break;
        if (sf.AddrFrame.Offset == 0)  // Basic sanity check to make sure
            break;                      // the frame is OK.  Bail if not.
        Printf(_T("%08X  %08X  "), sf.AddrPC.Offset, sf.AddrFrame.Offset);

        // Get the name of the function for this stack frame entry
        BYTE symbolBuffer[ sizeof(SYMBOL_INFO) + 2048 ];
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuffer;
        pSymbol->SizeOfStruct = sizeof(symbolBuffer);
        pSymbol->MaxNameLen = 2048;
                        
        DWORD64 symDisplacement = 0;    // Displacement of the input address,
                                        // relative to the start of the symbol
        if (m_pDbgHelp->SymFromAddr && m_pDbgHelp->SymFromAddr(m_hProcess, sf.AddrPC.Offset, &symDisplacement, pSymbol)) {
            TCHAR szModule[MAX_PATH] = _T("");
			_stprintf(szModule, _T("%-s+0x08I64X"), pSymbol->Name, symDisplacement);
			Printf(_T("%-30s"), szModule);
        } else {
            // No symbol found.  Print out the logical address instead.
            TCHAR szModule[MAX_PATH] = _T("");
            DWORD section = 0, offset = 0;

            GetLogicalAddress((PVOID) (DWORD64) sf.AddrPC.Offset, szModule, sizeof(szModule), section, offset);
            Printf(_T("%04X:%08X                   %s"), section, offset, szModule);
        }

        // Get the source line for this stack frame entry
        IMAGEHLP_LINE lineInfo = { sizeof(IMAGEHLP_LINE) };
        DWORD dwLineDisplacement;
        if (m_pDbgHelp->SymGetLineFromAddr && (m_pDbgHelp->SymGetLineFromAddr(m_hProcess, sf.AddrPC.Offset, &dwLineDisplacement, &lineInfo))) {
            Printf(_T("  %s line %u"),lineInfo.FileName,lineInfo.LineNumber); 
			if (m_bHasSymbol == FALSE)
				m_bHasSymbol = TRUE;
        }
        Printf(_T("\r\n"));

        // Write out the variables, if desired
        if (bWriteVariables) {
            // Use SymSetContext to get just the locals/params for this frame
            IMAGEHLP_STACK_FRAME imagehlpStackFrame;

            imagehlpStackFrame.InstructionOffset = sf.AddrPC.Offset;
			if (m_pDbgHelp->SymSetContext)
				m_pDbgHelp->SymSetContext(m_hProcess, &imagehlpStackFrame, 0);

            // Enumerate the locals/parameters
			if (m_pDbgHelp->SymEnumSymbols)
	            m_pDbgHelp->SymEnumSymbols(m_hProcess, 0, 0, EnumerateSymbolsCallback, &sf);

            Printf(_T("\r\n"));
        }
    }
	Printf(_T("\r\n"));
	Printf(_T("\r\n"));
}

//
// The function invoked by SymEnumSymbols
//
BOOL CALLBACK CExceptionReport::EnumerateSymbolsCallback(PSYMBOL_INFO  pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
    char szBuffer[2048];

    __try {
        if (FormatSymbolValue(pSymInfo, (STACKFRAME *) UserContext, szBuffer, sizeof(szBuffer)))
            Printf(_T("\t%s\r\n"), szBuffer);
    }
    __except(1) {
        Printf(_T("punting on symbol %s\r\n"), pSymInfo->Name);
    }
    return TRUE;
}

// Given a SYMBOL_INFO representing a particular variable, displays its contents.
// If it's a user defined type, display the members and their values.
BOOL CExceptionReport::FormatSymbolValue(PSYMBOL_INFO pSym, STACKFRAME * sf, char * pszBuffer, unsigned cbBuffer)
{
    char * pszCurrBuffer = pszBuffer;

    // Indicate if the variable is a local or parameter
    if (pSym->Flags & IMAGEHLP_SYMBOL_INFO_PARAMETER)
        pszCurrBuffer += sprintf(pszCurrBuffer, "Parameter ");
    else if (pSym->Flags & IMAGEHLP_SYMBOL_INFO_LOCAL)
        pszCurrBuffer += sprintf(pszCurrBuffer, "Local ");

    // If it's a function, don't do anything.
    if (pSym->Tag == 5)   // SymTagFunction from CVCONST.H from the DIA SDK
        return FALSE;

    // Emit the variable name
    pszCurrBuffer += sprintf(pszCurrBuffer, "\'%s\'", pSym->Name);

    DWORD  pVariable = 0;    // Will point to the variable's data in memory

    if (pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGRELATIVE) {
        // if (pSym->Register == 8)   // EBP is the value 8 (in DBGHELP 5.1)
        {                               //  This may change!!!
            pVariable = (DWORD) sf->AddrFrame.Offset;
			// TODO: CHECK HERE
            pVariable += (DWORD) pSym->Address;
        }
        // else
        //  return FALSE;
    }
    else if (pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER) {
        return FALSE;   // Don't try to report register variable
    } else {
        pVariable = (DWORD) pSym->Address;   // It must be a global variable
    }

    // Determine if the variable is a user defined type (UDT).  IF so, bHandled
    // will return TRUE.
    BOOL bHandled;
    pszCurrBuffer = DumpTypeIndex(pszCurrBuffer,pSym->ModBase, pSym->TypeIndex, 0, pVariable, bHandled);
    if (!bHandled) {
        // The symbol wasn't a UDT, so do basic, stupid formatting of the
        // variable.  Based on the size, we're assuming it's a char, WORD, or
        // DWORD.
        BasicType basicType = GetBasicType(pSym->TypeIndex, pSym->ModBase);
        pszCurrBuffer = (CHAR *) FormatOutputValue( pszCurrBuffer, basicType, pSym->Size, reinterpret_cast<PVOID> (pVariable) );
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// If it's a user defined type (UDT), recurse through its members until we're
// at fundamental types.  When he hit fundamental types, return
// bHandled = FALSE, so that FormatSymbolValue() will format them.
//////////////////////////////////////////////////////////////////////////////
char *CExceptionReport::DumpTypeIndex(char *pszCurrBuffer, DWORD64 modBase, DWORD dwTypeIndex, unsigned nestingLevel, DWORD  offset, BOOL & bHandled)
{
    bHandled = FALSE;

	if (!m_pDbgHelp->SymGetTypeInfo)
		return NULL;

    // Get the name of the symbol.  This will either be a Type name (if a UDT), or the structure member name.
    WCHAR * pwszTypeName;
    if (m_pDbgHelp->SymGetTypeInfo(m_hProcess, modBase, dwTypeIndex, TI_GET_SYMNAME, &pwszTypeName)) {
        pszCurrBuffer += sprintf(pszCurrBuffer, " %ls", pwszTypeName);
        LocalFree(pwszTypeName);
    }

    // Determine how many children this type has.
    DWORD dwChildrenCount = 0;
    m_pDbgHelp->SymGetTypeInfo(m_hProcess, modBase, dwTypeIndex, TI_GET_CHILDRENCOUNT, &dwChildrenCount);

    if (!dwChildrenCount)     // If no children, we're done
        return pszCurrBuffer;

    // Prepare to get an array of "TypeIds", representing each of the children.
    // SymGetTypeInfo(TI_FINDCHILDREN) expects more memory than just a
    // TI_FINDCHILDREN_PARAMS struct has.  Use derivation to accomplish this.
    struct FINDCHILDREN : TI_FINDCHILDREN_PARAMS {
        ULONG   MoreChildIds[1024];
        FINDCHILDREN(){Count = sizeof(MoreChildIds) / sizeof(MoreChildIds[0]);}
    } children;

    children.Count = dwChildrenCount;
    children.Start= 0;
    // Get the array of TypeIds, one for each child type
    if (!m_pDbgHelp->SymGetTypeInfo(m_hProcess, modBase, dwTypeIndex, TI_FINDCHILDREN, &children)) {
        return pszCurrBuffer;
    }

    // Append a line feed
    pszCurrBuffer += sprintf(pszCurrBuffer, "\r\n");

    // Iterate through each of the children
    for (unsigned i = 0; i < dwChildrenCount; i++) {
        // Add appropriate indentation level (since this routine is recursive)
        for (unsigned j = 0; j <= nestingLevel+1; j++)
            pszCurrBuffer += sprintf(pszCurrBuffer, "\t");

        // Recurse for each of the child types
        BOOL bHandled2;
        pszCurrBuffer = DumpTypeIndex(pszCurrBuffer, modBase, children.ChildId[i], nestingLevel+1, offset, bHandled2);

        // If the child wasn't a UDT, format it appropriately
        if (!bHandled2) {
            // Get the offset of the child member, relative to its parent
            DWORD dwMemberOffset;
            m_pDbgHelp->SymGetTypeInfo(m_hProcess, modBase, children.ChildId[i], TI_GET_OFFSET, &dwMemberOffset);

            // Get the real "TypeId" of the child.  We need this for the SymGetTypeInfo(TI_GET_TYPEID) call below.
            DWORD typeId;
            m_pDbgHelp->SymGetTypeInfo(m_hProcess, modBase, children.ChildId[i], TI_GET_TYPEID, &typeId);

            // Get the size of the child member
            ULONG64 length;
            m_pDbgHelp->SymGetTypeInfo(m_hProcess, modBase, typeId, TI_GET_LENGTH,&length);

            // Calculate the address of the member
            DWORD  dwFinalOffset = offset + dwMemberOffset;

            BasicType basicType = GetBasicType(children.ChildId[i], modBase);

            pszCurrBuffer = (CHAR *) FormatOutputValue(pszCurrBuffer, basicType, length, (PVOID)dwFinalOffset); 

            pszCurrBuffer += sprintf(pszCurrBuffer, "\r\n");
        }
    }

    bHandled = TRUE;
    return pszCurrBuffer;
}

char *CExceptionReport::FormatOutputValue(char *pszCurrBuffer, BasicType basicType, DWORD64 length, PVOID pAddress)
{
    // Format appropriately (assuming it's a 1, 2, or 4 bytes (!!!)
    if (length == 1)
        pszCurrBuffer += sprintf(pszCurrBuffer, " = %X", *(PBYTE)pAddress);
    else if (length == 2)
        pszCurrBuffer += sprintf(pszCurrBuffer, " = %X", *(PWORD)pAddress);
    else if (length == 4) {
        if (basicType == btFloat) {
            pszCurrBuffer += sprintf(pszCurrBuffer," = %f", *(PFLOAT)pAddress);
        } else if (basicType == btChar) {
            if (!IsBadStringPtr(*(PSTR*)pAddress, 32)) {
                pszCurrBuffer += sprintf(pszCurrBuffer, " = \"%.31s\"", *(PDWORD)pAddress);
			} else
                pszCurrBuffer += sprintf(pszCurrBuffer, " = %X", *(PDWORD)pAddress);
        } else
            pszCurrBuffer += sprintf(pszCurrBuffer," = %X", *(PDWORD)pAddress);
    } else if (length == 8) {
        if (basicType == btFloat) {
            pszCurrBuffer += sprintf(pszCurrBuffer, " = %lf", *(double *)pAddress);
		} else {
            pszCurrBuffer += sprintf( pszCurrBuffer, " = %08I64X",
                                        *(DWORD64*)pAddress );
		}
    }
    return pszCurrBuffer;
}

BasicType CExceptionReport::GetBasicType(DWORD typeIndex, DWORD64 modBase)
{
    BasicType basicType;

	if (!m_pDbgHelp->SymGetTypeInfo)
		return btNoType;
    if (m_pDbgHelp->SymGetTypeInfo(m_hProcess, modBase, typeIndex, TI_GET_BASETYPE, &basicType)) {
        return basicType;
    }
    // Get the real "TypeId" of the child.  We need this for the
    // SymGetTypeInfo(TI_GET_TYPEID) call below.
    DWORD typeId;
    if (m_pDbgHelp->SymGetTypeInfo(m_hProcess,modBase, typeIndex, TI_GET_TYPEID, &typeId)) {
        if (m_pDbgHelp->SymGetTypeInfo(m_hProcess, modBase, typeId, TI_GET_BASETYPE, &basicType)) {
            return basicType;
        }
    }
    return btNoType;
}

// Memory Dump function
void CExceptionReport::Dump(DWORD64 dw64Offset, DWORD dwSize, BOOL bAlign)
{
	DWORD dwLoc, dwILoc, dwX;
	LPBYTE pOut = (LPBYTE) dw64Offset;

	if (bAlign == TRUE)
		pOut = (LPBYTE) ((dw64Offset >> SHIFT_NUM) << SHIFT_NUM);
	for (dwLoc = 0; dwLoc < dwSize; dwLoc += 16, pOut += BYTES_PER_LINE) {
		LPBYTE pLine = pOut;

		Printf(_T("%08lX: "), (DWORD64) pOut);
		for (dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++) {
			if (dwX == (BYTES_PER_LINE / 2))
				Printf(_T(" "));
			if (dwILoc++ > dwSize) {
				Printf(_T("?? "));
			} else {
				Printf(_T("%02X "), *(pLine++));
			}
		}
		pLine = pOut;
		Printf(" ");
		for (dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++) {
			if (dwILoc++ > dwSize) {
				Printf(_T(" "));
			} else {
				Printf(_T("%c"), isprint(*pLine) ? *pLine : '.');
				pLine++;
			}
		}
		Printf("\r\n");
	}
}

// Helper function that writes to the report file, and allows the user to use printf style formating                                                     
int __cdecl CExceptionReport::Printf(const TCHAR * format, ...)
{
    TCHAR szBuff[4096];
    int retValue;
    DWORD cbWritten;
    va_list argptr;

    va_start(argptr, format);
    retValue = vsprintf(szBuff, format, argptr);
    va_end(argptr);
    WriteFile( m_hReportFile, szBuff, retValue * sizeof(TCHAR), &cbWritten, 0 );
    return retValue;
}
