#define BYTES_PER_LINE		16

/**
 * 해당 데이터 메모리 값은 hex 코드로 덤프를 뜬다. 
 *
 * @param fileName		덤프 데이터 저장할 파일명
 * @param dw64Offset	덤프할 원본 데이터
 * @param dwSize		덤프할 원본 데이터 크기
 * @param bAlign		한줄로 정렬 여부(기본값 : FALSE)
 */
void HexaDump( LPSTR fileName, DWORD64 dw64Offset, DWORD dwSize, BOOL bAlign = FALSE)
{
	DWORD dwLoc, dwILoc, dwX;
	LPBYTE pOut = (LPBYTE) dw64Offset;
	TCHAR szOutput[BYTES_PER_LINE * 6];

	HFILE handle = _lopen( fileName, OF_WRITE);	
	if( handle == HFILE_ERROR) {
		handle = _lcreat( fileName, 0);
	}
	else _llseek( handle, 0, FILE_END);

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
		_lwrite( handle, szOutput, _tcslen( szOutput ) );
	}
	_stprintf( szOutput, _T( "\r\n" ) );//마지막에 버퍼를 줄바꿈으로 세팅한 뒤.

	//fprintf(stdout, "%d", _tcslen( szOutput ));//2 <-- 문자열 끝 표시 00 제외한 값 리턴.

	_lwrite( handle, szOutput, _tcslen( szOutput ) );//줄바꿈 버퍼 뿌려줌.
	_lclose( handle);
}