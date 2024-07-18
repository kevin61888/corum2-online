
#pragma once
#include "../../stdafx.h"
#include <time.h>

#define BYTES_PER_LINE		16

/**
 * 해당 데이터 메모리 값은 hex 코드로 덤프를 뜬다. 
 *
 * @param fileName		덤프 데이터 저장할 파일명
 * @param dw64Offset	덤프할 원본 데이터
 * @param dwSize		덤프할 원본 데이터 크기
 * @param bAlign		    한줄로 정렬 여부(기본값 : FALSE)
 */
static void HexaDump( LPSTR fileName, DWORD64 dw64Offset, DWORD dwSize, BOOL bAlign = FALSE, LPSTR AddInfo = NULL )
{
	DWORD dwLoc, dwILoc, dwX;
	LPBYTE pOut = (LPBYTE) dw64Offset;
	TCHAR szOutput[BYTES_PER_LINE * 6];

	HFILE handle = _lopen( fileName, OF_WRITE);	
	if( handle == HFILE_ERROR) {
		handle = _lcreat( fileName, 0);
	}
	else _llseek( handle, 0, FILE_END);

	//////////////////////////////////////////////////////////////////////////////////////////////
	TCHAR sztemp[11] = {0,};
	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );
	sprintf( sztemp, "%02d:%02d:%02d  ", currTM->tm_hour, currTM->tm_min, currTM->tm_sec );
	_lwrite( handle, sztemp, (UINT)10 );

	if( AddInfo )
		_lwrite( handle, AddInfo, (UINT)(_tcslen( AddInfo )) );
	_lwrite( handle, "\r\n", (UINT)2 );
	/////////////////////////////////////////////////////////////////////////////////////////////

	if( bAlign == TRUE )
		pOut = (LPBYTE) ((dw64Offset >> 4) << 4);

	for( dwLoc = 0; dwLoc < dwSize; dwLoc += 16, pOut += BYTES_PER_LINE ) {
		LPBYTE pLine = pOut;

		_stprintf( szOutput, _T( "%08lX: " ), (DWORD64) pOut );
		for( dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++ ) {
			if( dwX == (BYTES_PER_LINE / 2) )
				_stprintf( szOutput + _tcslen( szOutput ), _T( " " ) );
			if( dwILoc++ >= dwSize ) {
				_stprintf( szOutput + _tcslen( szOutput ), _T( "?? " ) );
			} else {
				_stprintf( szOutput + _tcslen( szOutput ), _T( "%02X " ), *(pLine++) );
			}
		}
		pLine = pOut;
		_stprintf( szOutput + _tcslen( szOutput ), _T( " " ) );
		for( dwX = 0, dwILoc = dwLoc; dwX < BYTES_PER_LINE; dwX++ ) {
			if( dwILoc++ >= dwSize ) {
				_stprintf( szOutput + _tcslen( szOutput ), _T( " " ) );
			} else {
				_stprintf( szOutput + _tcslen( szOutput ), _T( "%c" ), isprint( *pLine ) ? *pLine : '.');
				pLine++;
			}
		}
		//fprintf(stdout, "%d", _tcslen( szOutput ));//76

		_stprintf( szOutput + _tcslen( szOutput ), _T( "\r\n" ) );//글자 뒤에 줄바꿈 문자 붙임.(0x0d0a00).<-자동으로 00 붙어서 문자열 끝 표시. _tcslen(szOutput) 은 현재 버퍼에 들어있는 문자열크기!

		//fprintf( stdout, "%s", szOutput );
		_lwrite( handle, szOutput, (UINT)(_tcslen( szOutput )) );
	}
	_stprintf( szOutput, _T( "\r\n" ) );//마지막에 버퍼를 줄바꿈으로 세팅한 뒤.

	//fprintf(stdout, "%d", _tcslen( szOutput ));//2 <-- 문자열 끝 표시 00 제외한 값 리턴.

	_lwrite( handle, szOutput, (UINT)(_tcslen( szOutput )) );//줄바꿈 버퍼 뿌려줌.
	_lclose( handle);
}


/**
 * 해당 데이터의 스트링을 파일로 출력한다.
 *
 * @param szFilename 스트링 데이터 저장할 파일명
 * @param szMsg	    스트링 원본 데이터
 * @param  ...				va_list
  */
static void StringDump( char* szFilename, char* szMsg, ... )
{
	char szOutBuff[256] = {0,};
	va_list	v1;
	va_start( v1, szMsg );
	vsprintf( szOutBuff, szMsg, v1 );
	va_end(v1);

	//FILE* fp;
	//fp = fopen( szFilename, "a" );
	HFILE handle = _lopen( szFilename, OF_WRITE);	
	if( handle == HFILE_ERROR) {
		handle = _lcreat( szFilename, 0);
	}
	else _llseek( handle, 0, FILE_END);

	////////////////////////////////////////////////////////////////////////	
	TCHAR sztemp[23] = {0,};

	time_t tval;
	time( &tval );
	struct tm *currTM = localtime( &tval );

	sprintf( sztemp, "[%04d/%02d/%02d %02d:%02d:%02d] ",
			currTM->tm_year+1900, currTM->tm_mon+1, currTM->tm_mday,
			currTM->tm_hour, currTM->tm_min, currTM->tm_sec );
	_lwrite( handle, sztemp, (UINT)22 );

	_lwrite( handle, szOutBuff, (UINT)(strlen( szOutBuff )) );

	////////////////////////////////////////////////////////////////////////

	sprintf( sztemp, "\r\n" );
	_lwrite( handle, sztemp, (UINT)2 );

	//fprintf( fp, "%s\n", szOutBuff );
	//fflush( fp );
	//fclose( fp );

	_lclose( handle);
}
