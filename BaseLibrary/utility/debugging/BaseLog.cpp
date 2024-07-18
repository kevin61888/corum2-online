#include "../../StdAfx.h"
#include "BaseLog.h"

#ifndef MAX_BASELOG_BUFFER
#define MAX_BASELOG_BUFFER			(1024 * 8)
#endif

#define DetailLogType(pFormat, pRoutine, pFileName, lLineNumber) \
					  PrintTimeEx(m_LogType[eLogType]); \
					  PrintArguments(pFormat); \
					  PrintSourceInfo(pRoutine, pFileName, lLineNumber); \
					  ProcessLogging(eLogType)

void g_BaseLog_SimpleLog( const char* pFormat, ... )
{
	static CHAR m_Buffer[MAX_BASELOG_BUFFER];
	DWORD m_dwBufferUsed = 0;
	va_list	Arguments;

	va_start( Arguments, pFormat );
	_vsntprintf( m_Buffer, MAX_BASELOG_BUFFER - m_dwBufferUsed, pFormat, Arguments );
	va_end( Arguments );
	OutputDebugString( m_Buffer );
}

void g_BaseLog_ErrorBox( const char* pFormat, ... )
{
	static CHAR m_Buffer[MAX_BASELOG_BUFFER];
	DWORD m_dwBufferUsed = 0;
	va_list	Arguments;

	va_start( Arguments, pFormat );
	_vsntprintf( m_Buffer, MAX_BASELOG_BUFFER - m_dwBufferUsed, pFormat, Arguments );
	va_end( Arguments );
	MessageBox( NULL, m_Buffer, "Error", MB_OK );
}

void g_BaseLog_DetailLog(const char* pFormat, ...)
{
	static CHAR m_Buffer[MAX_BASELOG_BUFFER];
	DWORD m_dwBufferUsed = 0;

	m_dwBufferUsed = ::_snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, "[Tm-%08d][Ex-", ::timeGetTime());

	va_list	Arguments;
	va_start(Arguments, pFormat);
	m_dwBufferUsed += ::_vsntprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, pFormat, Arguments);
	va_end(Arguments);

	m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, "]\r\n\0");
//	m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, "][FN-%s][LN-%d]\r\n\0", pFileName, nLine);

	OutputDebugString(m_Buffer);
}

#define BYTES_PER_LINE		16
#define SHIFT_NUM			4

void g_BaseLog_Dump( DWORD64 dw64Offset, DWORD dwSize, BOOL bAlign )
{
	static CHAR m_Buffer[MAX_BASELOG_BUFFER];
	DWORD m_dwBufferUsed = 0;
	DWORD dwLoc, dwILoc, dwX;
	LPBYTE pOut = (LPBYTE) dw64Offset;

	if (bAlign == TRUE)
		pOut = (LPBYTE) ((dw64Offset >> SHIFT_NUM) << SHIFT_NUM);
	for (dwLoc = 0; dwLoc < dwSize; dwLoc += 16, pOut += BYTES_PER_LINE) {
		LPBYTE pLine = pOut;

		m_dwBufferUsed = _snprintf(m_Buffer, MAX_BASELOG_BUFFER - m_dwBufferUsed, "%08lX: ", (DWORD64) pOut);
		for (dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++) {
			if (dwX == (BYTES_PER_LINE / 2))
				m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, " ");
			if (dwILoc++ >= dwSize) {
				m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, "?? ");
			} else {
				m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, "%02X ", *(pLine++));
			}
		}
		pLine = pOut;
		m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, " ");
		for (dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++) {
			if (dwILoc++ >= dwSize) {
				m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, " ");
			} else {
				m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, "%c", isprint(*pLine) ? *pLine : '.');
				pLine++;
			}
		}
		m_dwBufferUsed += _snprintf(m_Buffer + m_dwBufferUsed, MAX_BASELOG_BUFFER - m_dwBufferUsed, "\r\n");
		OutputDebugString(m_Buffer);
	}
}

void g_BaseLog_Flush(void)
{
}
