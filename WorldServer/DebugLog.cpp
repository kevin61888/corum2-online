// Debug.cpp: implementation of the CDebugLog class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Debuglog.h"
#include <assert.h>
#include <direct.h>
#include <Time.h>

#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_FCLOSE(p)			{ if(p) { ::fclose(p); (p)=NULL; } }

CDebugLog::CDebugLog()
{
	m_pLogFile = NULL;
	m_hConsole	= INVALID_HANDLE_VALUE;
	Clear();
}

CDebugLog::~CDebugLog()
{
	Clear();
	CloseConsole();
}

bool CDebugLog::Init(const eOUPUT_TYPE OutputMode, const string &strFolderName, const string &strFileName)
{
	Lock lock(*this);

	switch(OutputMode)
	{
	case OUTPUT_NOT_SET:
		{
			assert(NULL && "CDebugLog Init Error(OUTPUT_NOT_SET)");
			break;
		}//OUTPUT_NOT_SET:
	default:
		{	
			m_dwModeFlag = OutputMode;
			break;
		}
	}//switch(OutputMode)

	if(OUTPUT_JUST_CONSOLE  & m_dwModeFlag)
	{
		OpenConsole();
	}

	m_strFileName = strFileName;
	m_strFolderName = strFolderName;
	return true;
}

void CDebugLog::Clear()
{
	Lock lock(*this);
	m_dwModeFlag = OUTPUT_NOT_SET;
	
	m_strFileName = "";

	SAFE_FCLOSE(m_pLogFile);//파일 클리어
}

void CDebugLog::SetWriteFile()//파일 관리부
{
	Lock lock(*this);

	time_t now_time = {0,};
	::time( &now_time );   // Get time in seconds
	struct tm *new_local_time = NULL;
	new_local_time = localtime( &now_time );
	
	LOG_DATE NewDate((new_local_time->tm_year+1900)%100, new_local_time->tm_mon+1, new_local_time->tm_mday);

	if(!m_pLogFile
	|| memcmp( &NewDate, &m_PrevLogDate, sizeof(LOG_DATE) ) )
	{
		m_PrevLogDate = NewDate;
		
		::mkdir( m_strFolderName.c_str() );
				
		char szFileName[MAX_PATH] = {0,};

		::sprintf(szFileName,"%s/%02d_%02d_%02d %s", 
			m_strFolderName.c_str(),
			m_PrevLogDate.m_iYear,
			m_PrevLogDate.m_iMonth,
			m_PrevLogDate.m_iDay,
			m_strFileName.c_str() );
		
		SAFE_FCLOSE( m_pLogFile );//파일 닫고		
		
		m_pLogFile = ::fopen(szFileName,"at");//파일 오픈
		
		if(!m_pLogFile)
		{
			assert(NULL && "SetWriteFile() Error" );//열리지 않았으면 assert
		}
	}
}

void CDebugLog::Log(const eLOG_LEVEL LogLv, const char *szLogMsg, ...)
{
	Lock lock(*this);

	char szLog[MAX_LOG_SIZE] = {0,};
	va_list vargs;
	va_start(vargs,szLogMsg);
	::vsprintf(szLog, szLogMsg, vargs);
	va_end(vargs);

	char szTime[MAX_PATH] = {0,};
	char szResultMsg[MAX_LOG_SIZE+20];

	::sprintf(szResultMsg, "<Lv%d %s> %s",LogLv, ::_strtime(szTime), szLog);

	if(OUTPUT_JUST_FILE & m_dwModeFlag )
	{
		OutFile(szResultMsg);
	}
	if(OUTPUT_JUST_TRACE & m_dwModeFlag )
	{
		OutTrace(szResultMsg);
	}
	if(OUTPUT_JUST_CONSOLE & m_dwModeFlag )
	{
		OutConsole(szResultMsg);
	}
}

void CDebugLog::OutFile(const char* szLogMsg)
{
	Lock lock(*this);
	
	SetWriteFile();
	
	if(m_pLogFile)
	{
		::fprintf(m_pLogFile,"%s\n",szLogMsg);
		::fflush(m_pLogFile);
	}
}

void CDebugLog::OutTrace(const char* szLogMsg)
{
	Lock lock(*this);
	::OutputDebugString(szLogMsg);
	::OutputDebugString("\n");
}

void CDebugLog::OutConsole(const char* szLogMsg)
{
	Lock lock(*this);
	DWORD dwBytes = 0;
	::WriteFile(m_hConsole, szLogMsg, static_cast<DWORD>(::strlen(szLogMsg)), &dwBytes, NULL);
	::WriteFile(m_hConsole, "\n", static_cast<DWORD>(::strlen("\n")), &dwBytes, NULL);
}

bool CDebugLog::OpenConsole()
{
	Lock lock(*this);
	if( OUTPUT_JUST_CONSOLE & m_dwModeFlag )
	{
		if(!::AllocConsole()){return false;}
		m_hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
		if(INVALID_HANDLE_VALUE == m_hConsole) 
		{
			::FreeConsole();
			return false;
		}
		return true;
	}
	return false;
}

void CDebugLog::CloseConsole()
{
	Lock lock(*this);
	if(( OUTPUT_JUST_CONSOLE & m_dwModeFlag )
	&& ( INVALID_HANDLE_VALUE != m_hConsole) )
	{
		::FreeConsole();
		::CloseHandle(m_hConsole);
		m_hConsole	= INVALID_HANDLE_VALUE;
	}
}