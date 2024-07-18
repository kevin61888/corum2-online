//
// File: CLowFile.cpp
// Desc: File class
//
// Auth: Standil (cpascal@nownuri.net)
//

#ifndef _CLOWFILE_H_
#define _CLOWFILE_H_

#include "OpenMode.h"

typedef class CLowFile*		LPCLowFile;
class CLowFile {
public:
	CLowFile();
	~CLowFile();

	enum SeekFlags {
		seekBegin	= 0,
		seekCurrent = 1,
		seekEnd		= 2
	};

	void	Close( void );											// 파일 닫기
	BOOL	Open( const LPCSTR szFilename, UINT nOpenFlags = modeRead );	// 파일 열기
	BOOL	OpenReadf( LPCTSTR szFilename, ... );
	BOOL	OpenWritef( LPCTSTR szFilename, ... );
	BOOL	OpenCreatef( LPCTSTR szFilename, ... );

	int		Size( void );											// 파일 길이 얻기
	int		GetFilePointer( void );									// 현재 위치 얻기
	int		Read( LPVOID lpBuffer, DWORD dwBytesToRead );			// 파일 읽기
	int		Write( LPVOID lpBuffer, DWORD dwBytesToRead );			// 파일 쓰기
	int		Seek( LONG dwBytesToMove , SeekFlags dwMoveMethod );	// 위치 이동
	BOOL	IsEOF( void );											// 끝인지 검사

	// 인자로 파일이름을 전달하는 인터페이스 함수들
	BOOL	Access( LPCSTR szFilename );		// 파일 존재여부 검사
	int		Size( LPCSTR szFilename );			// 파일 길이 반환(-1 = 없음)
	int		Read( LPCSTR szFilename, LPVOID lpBuffer, DWORD dwBytesToRead );
	int		Write( LPCSTR szFilename, LPVOID lpBuffer, DWORD dwBytesToRead );

private:
	HANDLE	m_hFile;
	TCHAR	m_szFilename[MAX_PATH];
};

#endif
