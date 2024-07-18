#pragma once
#include "windows.h"

#include <string>
using namespace std;

#include "Threads.h"
using namespace Loki;

const size_t MAX_LOG_SIZE	= 2048;//로그기록 최대 길이

enum eOUPUT_TYPE
{
	OUTPUT_NOT_SET			=	0x00000000,
	OUTPUT_JUST_FILE		=	0x00000001,
	OUTPUT_JUST_TRACE		=	0x00000002,
	OUTPUT_JUST_CONSOLE		=	0x00000004,
	OUTPUT_FILE_AND_TRACE	=	OUTPUT_JUST_FILE | OUTPUT_JUST_TRACE,
	OUTPUT_FILE_AND_CONSOLE	=	OUTPUT_JUST_FILE | OUTPUT_JUST_CONSOLE,
	OUTPUT_ALL				=	OUTPUT_JUST_FILE | OUTPUT_JUST_TRACE | OUTPUT_JUST_CONSOLE,	
};

enum eLOG_LEVEL
{
	LOG_LV0		= 0,
	LOG_LV1		= 1,
	LOG_LV2		= 2,
	LOG_LV3		= 3,
	LOG_LV4		= 4,
	LOG_LV5		= 5,
	LOG_LV6		= 6,
	LOG_LV7		= 7,
	LOG_LV8		= 8,
	LOG_LV9		= 9,
};

class CDebugLog
	:	public ObjectLevelLockable< CDebugLog >
{
public:
	CDebugLog();
	virtual ~CDebugLog();
	
public:
	bool Init(const eOUPUT_TYPE OutputMode, const string &strFolderName = "./CustomerService", const string &strFileName = "Log.txt");
	void Log(const eLOG_LEVEL LogLv, const char *szLogMsg, ...);//기록부

protected:
	void Clear();
	void SetWriteFile();//기록될 파일 설정// 반드시 호출 전에 SetTime을 호출 하세요 SetTime 뒤에 오게 하세요
	
	void OutFile(const char *szLogMsg);
	void OutTrace(const char *szLogMsg);	
	void OutConsole(const char *szLogMsg);
	
	bool OpenConsole();
	void CloseConsole();

protected:
	typedef struct t_LogDate
	{
		t_LogDate(const __int32 iYear = 0, const __int32 iMonth = 0, const __int32 iDay = 0)
		{
			m_iYear = iYear;
			m_iMonth = iMonth;
			m_iDay = iDay;
		}
		__int32 m_iYear;
		__int32 m_iMonth;
		__int32 m_iDay;
	}LOG_DATE, *LP_LOG_DATE;

protected:
	DWORD	m_dwModeFlag;
	
	string	m_strFileName;
	string	m_strFolderName;

	LOG_DATE m_PrevLogDate;

	FILE*	m_pLogFile;//파일 핸들
	HANDLE	m_hConsole;//콘솔 핸들.

protected:
	CDebugLog operator=(const CDebugLog &old);//대입연산자 사용 불가.
	CDebugLog(const CDebugLog &old);//복사생성자 사용 불가.
};

/*-------------------------------------------------------------------
Example>
void Garbage()
{
	CDebugLog Dbg;
	Dbg.Init(OUTPUT_FILE_AND_TRACE ,"log", "Error2.txt");
	
	Dbg.Log(LOG_LV1,"여기서 무었을 %d을 호출 했지", 100);
	.
	.
	.
	Dbg.Log(LOG_LV1,"들어오면 안되는곳");
	.
	.
	.
}
*/