#pragma once

// ---------------------------------------------------------------------------------------
// 소스 레벨

#ifdef USE_IMMEDIATE_LOG_FLUSH
#define	BASELOG_FLUSH()		g_BaseLog_Flush();
#else
#define BASELOG_FLUSH()		(void*)0
#endif

#define _SOURCE_LEVEL_FATAL		0	// Only output errors
#define _SOURCE_LEVEL_ERROR		1	// Normal release mode
#define _SOURCE_LEVEL_WARN		2	// Normal beta season mode
#define _SOURCE_LEVEL_INFO		3	// Output mostly
#define _SOURCE_LEVEL_DEBUG		4	// Output fully

#ifndef _SRCFILE_LEVEL		// 로그 레벨이 지정되어 있는지 확인
	#ifdef _DEBUG
		#define _SRCFILE_LEVEL		_SOURCE_LEVEL_DEBUG
	#else
		#define _SRCFILE_LEVEL		_SOURCE_LEVEL_ERROR
	#endif
#endif

#if (_SRCFILE_LEVEL >= _SOURCE_LEVEL_FATAL)
	#define USE_BASELOG_FATAL
#endif
#if (_SRCFILE_LEVEL >= _SOURCE_LEVEL_ERROR)
	#define USE_BASELOG_ERROR
#endif
#if (_SRCFILE_LEVEL >= _SOURCE_LEVEL_WARN)
	#define USE_BASELOG_WARN
#endif
#if (_SRCFILE_LEVEL >= _SOURCE_LEVEL_INFO)
	#define USE_BASELOG_INFO
#endif
#if (_SRCFILE_LEVEL == _SOURCE_LEVEL_DEBUG)
	#define USE_BASELOG_DEBUG
#endif

#ifdef	USE_BASELOG_FATAL
	#define FATALERRORLOG(x)	x
	#define FATALERRORLOG0(szString)															g_BaseLog_DetailLog(szString); BASELOG_FLUSH();
	#define FATALERRORLOG1(szFormat, arg1)													g_BaseLog_DetailLog(szFormat, arg1); BASELOG_FLUSH();
	#define FATALERRORLOG2(szFormat, arg1, arg2)												g_BaseLog_DetailLog(szFormat, arg1, arg2); BASELOG_FLUSH();
	#define FATALERRORLOG3(szFormat, arg1, arg2, arg3)										g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3); BASELOG_FLUSH();
	#define FATALERRORLOG4(szFormat, arg1, arg2, arg3, arg4)									g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4); BASELOG_FLUSH();
	#define FATALERRORLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)							g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5); BASELOG_FLUSH();
	#define FATALERRORLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6); BASELOG_FLUSH();
	#define FATALERRORLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7); BASELOG_FLUSH();
	#define FATALERRORLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); BASELOG_FLUSH();
	#define FATALERRORLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); BASELOG_FLUSH();
#else
	#define FATALERRORLOG(x)	(void*)0
	#define FATALERRORLOG0(szString)															(void*)0
	#define FATALERRORLOG1(szFormat, arg1)													(void*)0
	#define FATALERRORLOG2(szFormat, arg1, arg2)												(void*)0
	#define FATALERRORLOG3(szFormat, arg1, arg2, arg3)										(void*)0
	#define FATALERRORLOG4(szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define FATALERRORLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define FATALERRORLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define FATALERRORLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define FATALERRORLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define FATALERRORLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif

#ifdef	USE_BASELOG_ERROR
	#define ERRORLOG(x)	x
	#define ERRORLOG0(szString)															g_BaseLog_DetailLog(szString); BASELOG_FLUSH();
	#define ERRORLOG1(szFormat, arg1)														g_BaseLog_DetailLog(szFormat, arg1); BASELOG_FLUSH();
	#define ERRORLOG2(szFormat, arg1, arg2)												g_BaseLog_DetailLog(szFormat, arg1, arg2); BASELOG_FLUSH();
	#define ERRORLOG3(szFormat, arg1, arg2, arg3)											g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3); BASELOG_FLUSH();
	#define ERRORLOG4(szFormat, arg1, arg2, arg3, arg4)									g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4); BASELOG_FLUSH();
	#define ERRORLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)								g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5); BASELOG_FLUSH();
	#define ERRORLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6); BASELOG_FLUSH();
	#define ERRORLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)					g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7); BASELOG_FLUSH();
	#define ERRORLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); BASELOG_FLUSH();
	#define ERRORLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)		g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); BASELOG_FLUSH();
#else
	#define ERRORLOG(x)	(void*)0
	#define ERRORLOG0(szString)															(void*)0
	#define ERRORLOG1(szFormat, arg1)														(void*)0
	#define ERRORLOG2(szFormat, arg1, arg2)												(void*)0
	#define ERRORLOG3(szFormat, arg1, arg2, arg3)											(void*)0
	#define ERRORLOG4(szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define ERRORLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)								(void*)0
	#define ERRORLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define ERRORLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)					(void*)0
	#define ERRORLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define ERRORLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)		(void*)0
#endif

#ifdef	USE_BASELOG_WARN
	#define WARNLOG(x)	x
	#define WARNLOG0(szString)															g_BaseLog_DetailLog(szString); BASELOG_FLUSH();
	#define WARNLOG1(szFormat, arg1)													g_BaseLog_DetailLog(szFormat, arg1); BASELOG_FLUSH();
	#define WARNLOG2(szFormat, arg1, arg2)												g_BaseLog_DetailLog(szFormat, arg1, arg2); BASELOG_FLUSH();
	#define WARNLOG3(szFormat, arg1, arg2, arg3)										g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3); BASELOG_FLUSH();
	#define WARNLOG4(szFormat, arg1, arg2, arg3, arg4)									g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4); BASELOG_FLUSH();
	#define WARNLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)							g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5); BASELOG_FLUSH();
	#define WARNLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6); BASELOG_FLUSH();
	#define WARNLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7); BASELOG_FLUSH();
	#define WARNLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); BASELOG_FLUSH();
	#define WARNLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); BASELOG_FLUSH();
#else
	#define WARNLOG(x)	(void*)0
	#define WARNLOG0(szString)															(void*)0
	#define WARNLOG1(szFormat, arg1)													(void*)0
	#define WARNLOG2(szFormat, arg1, arg2)												(void*)0
	#define WARNLOG3(szFormat, arg1, arg2, arg3)										(void*)0
	#define WARNLOG4(szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define WARNLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define WARNLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define WARNLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define WARNLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define WARNLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif

#ifdef	USE_BASELOG_INFO
	#define INFOLOG(x)	x
	#define INFOLOG0(szString)															g_BaseLog_DetailLog(szString)
	#define INFOLOG1(szFormat, arg1)													g_BaseLog_DetailLog(szFormat, arg1); BASELOG_FLUSH();
	#define INFOLOG2(szFormat, arg1, arg2)												g_BaseLog_DetailLog(szFormat, arg1, arg2); BASELOG_FLUSH();
	#define INFOLOG3(szFormat, arg1, arg2, arg3)										g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3); BASELOG_FLUSH();
	#define INFOLOG4(szFormat, arg1, arg2, arg3, arg4)									g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4); BASELOG_FLUSH();
	#define INFOLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)							g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5); BASELOG_FLUSH();
	#define INFOLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6); BASELOG_FLUSH();
	#define INFOLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7); BASELOG_FLUSH();
	#define INFOLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); BASELOG_FLUSH();
	#define INFOLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); BASELOG_FLUSH();
#else
	#define INFOLOG(x)	(void*)0
	#define INFOLOG0(szString)															(void*)0
	#define INFOLOG1(szFormat, arg1)													(void*)0
	#define INFOLOG2(szFormat, arg1, arg2)												(void*)0
	#define INFOLOG3(szFormat, arg1, arg2, arg3)										(void*)0
	#define INFOLOG4(szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define INFOLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define INFOLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define INFOLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define INFOLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define INFOLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


#ifdef	USE_BASELOG_DEBUG
	#define DEBUGLOG(x)	x
	#define DEBUGLOG0(szString)															g_BaseLog_DetailLog(szString)
	#define DEBUGLOG1(szFormat, arg1)													g_BaseLog_DetailLog(szFormat, arg1); BASELOG_FLUSH();
	#define DEBUGLOG2(szFormat, arg1, arg2)												g_BaseLog_DetailLog(szFormat, arg1, arg2); BASELOG_FLUSH();
	#define DEBUGLOG3(szFormat, arg1, arg2, arg3)										g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3); BASELOG_FLUSH();
	#define DEBUGLOG4(szFormat, arg1, arg2, arg3, arg4)									g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4); BASELOG_FLUSH();
	#define DEBUGLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)							g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5); BASELOG_FLUSH();
	#define DEBUGLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6); BASELOG_FLUSH();
	#define DEBUGLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7); BASELOG_FLUSH();
	#define DEBUGLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8); BASELOG_FLUSH();
	#define DEBUGLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	g_BaseLog_DetailLog(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9); BASELOG_FLUSH();
#else
	#define DEBUGLOG(x)	(void*)0
	#define DEBUGLOG0(szString)															(void*)0
	#define DEBUGLOG1(szFormat, arg1)													(void*)0
	#define DEBUGLOG2(szFormat, arg1, arg2)												(void*)0
	#define DEBUGLOG3(szFormat, arg1, arg2, arg3)										(void*)0
	#define DEBUGLOG4(szFormat, arg1, arg2, arg3, arg4)									(void*)0
	#define DEBUGLOG5(szFormat, arg1, arg2, arg3, arg4, arg5)							(void*)0
	#define DEBUGLOG6(szFormat, arg1, arg2, arg3, arg4, arg5, arg6)						(void*)0
	#define DEBUGLOG7(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7)				(void*)0
	#define DEBUGLOG8(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8)			(void*)0
	#define DEBUGLOG9(szFormat, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9)	(void*)0
#endif


#define BASEDUMP(dw64Offset, dwSize, bAlign)	g_BaseLog_Dump(dw64Offset, dwSize, bAlign)
