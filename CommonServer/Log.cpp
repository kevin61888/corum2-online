#include "stdafx.h"
#include "Log.h"


FILE*				g_fpLog					= NULL;
CRITICAL_SECTION	g_criLog;
HANDLE				g_hIn					= NULL;
HANDLE				g_hOut					= NULL;
DWORD				g_dwScreenBufferLastRow = 0;
int					g_ConsoleLogLevel		= 2;
int					g_FileLogLevel			= 7;


void DECLSPECIFIER InitLog( void )
{
	InitializeCriticalSectionAndSpinCount(&g_criLog, 1000);

	g_hIn  = GetStdHandle(STD_INPUT_HANDLE);
	g_hOut = GetStdHandle(STD_OUTPUT_HANDLE);

	// To prevent mouse input buffer processing.
	DWORD dwMode = 0;
	GetConsoleMode(g_hIn, &dwMode);
	dwMode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(g_hIn, dwMode);
}

void DECLSPECIFIER FreeLog( void )
{
	DeleteCriticalSection(&g_criLog);

	if( g_fpLog )
	{
		fclose( g_fpLog );
	}
}

void DECLSPECIFIER SetConsoleSize(DWORD col,DWORD row)
{
	SMALL_RECT srect;
	COORD	dwSize;
	
	CONSOLE_SCREEN_BUFFER_INFO info;

	dwSize.X = (short) col;
	dwSize.Y = (short) row;
	
	SetConsoleScreenBufferSize(g_hOut,dwSize);
	GetConsoleScreenBufferInfo(g_hOut,&info);

	g_dwScreenBufferLastRow = row-1;
	srect.Top = 0;
	srect.Left = 0;
	srect.Right = SHORT(info.dwMaximumWindowSize.X-1);
	srect.Bottom = SHORT(info.dwMaximumWindowSize.Y-1);
	SetConsoleWindowInfo(g_hOut,TRUE, &srect);
}

void DECLSPECIFIER GetConsoleKeyInput(PINPUT_RECORD pInput, LPDWORD pResult)
{
	ReadConsoleInput(g_hIn,pInput,1,pResult);
}

void DECLSPECIFIER Log( int type, char *logmsg, ... )
{
	va_list vargs;
	struct tm *now;
	time_t nowTime;

	int year, mon, day;
	static int log_year = 0, log_mon = 0, log_day = 0;
	int hour, min, sec;

	char LogIdentifier[NUM_OF_LOG_TYPE] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G' };
	char buf[(MAX_LOG_LENGTH*10)+1]={0,};
	static char szLogFileName[80+1]={0,};

	// Argument Processing
	va_start( vargs, logmsg );

	// Get nowtime
	time( &nowTime );
	now = localtime(&nowTime);

	// Make it usable.
	year = now->tm_year + 1900;
	mon  = now->tm_mon + 1;
	day  = now->tm_mday;
	hour = now->tm_hour;
	min  = now->tm_min;
	sec  = now->tm_sec;

	// Lock...
	EnterCriticalSection(&g_criLog);

	if( log_year && ( (log_year != year) || (log_mon != mon) || (log_day != day) ) )
	{
		// Close fpLog
		fclose( g_fpLog );
		g_fpLog = NULL;

		// Clear log_year
		log_year = 0;
	}

	if( log_year == 0 || !g_fpLog )
	{
		// Set log_year, log_mon, log_day.
		log_year = year;
		log_mon = mon;
		log_day = day;

		wsprintf( szLogFileName, ".\\%d-%02d-%02d.log", year, mon, day );

		g_fpLog = fopen( szLogFileName, "a" );

		if( !g_fpLog )
		{
			// Notify ERROR
			wsprintf( buf, "FATAL ERROR at Log() :: Can't open LogFile('%s')", szLogFileName );
			WriteText( buf );
			goto lb_Exit;
		}
	}

	// Write Log Type
	buf[0] = LogIdentifier[type];
	buf[1] = ' ';

	// Write Log rised time.
	wsprintf( buf+2, "<%02d:%02d:%02d> ", hour, min, sec );

	// Write Log's Body.
	if( strlen( logmsg ) > (MAX_LOG_LENGTH-2-11) )
	{
		// Self-calling.
		Log( LOG_FATAL, "Map() Too long string - This log will be lost" );
		va_end( vargs );
		goto lb_Exit;
	}

	vsprintf( buf+2+11, logmsg, (vargs) );

	// Now Log it.
	// To Screen
	switch(type)
	{
	case LOG_JUST_DISPLAY:
		SetConsoleTextAttribute(g_hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE ); 
		break;
	case LOG_JUST_DISPLAY1:
		SetConsoleTextAttribute(g_hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_BLUE ); 
		break;
	default:
		SetConsoleTextAttribute(g_hOut, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | BACKGROUND_INTENSITY); 
	}

	WriteText( buf );		
	
	// To File
	if( g_fpLog && (LOG_JUST_DISPLAY > type) )
	{
		lstrcat( buf, "\n" );
		fputs( buf, g_fpLog );
		// this can make server to slow.
		fflush( g_fpLog );
	}

lb_Exit:
	LeaveCriticalSection(&g_criLog);
	
	// Finish Func
	va_end( vargs );
	return;
}	

void DECLSPECIFIER WriteText(char* msg, bool type )
{
	DWORD lenout;

	WriteConsole(g_hOut, msg, lstrlen(msg), &lenout,0);
	if( type == true )	WriteConsole(g_hOut, "\n", 1, &lenout,0);
	
	return;
}