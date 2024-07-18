//
// File: CServerINIFile.h
// Desc: INI file class
//
// Auth: Standil (cpascal@nownuri.net)
//

#pragma once

#include <tchar.h>

typedef class CServerINIFile*		LPCServerINIFile;
class CServerINIFile {
public:
	CServerINIFile();
	CServerINIFile( LPCSTR szFilename, UINT nOpenFlags = modeRead );
	~CServerINIFile();

	enum OpenFlags {
		modeRead	= (int) 0x00000,
		modeWrite	= (int) 0x00080,
		modeCreate	= (int) 0x01000,
		modeAll		= (int) 0x08000,
	};

	BOOL Open( LPCSTR szFilename, UINT nOpenFlags = modeRead );
	BOOL ReadLine( const TCHAR *szSection );
	void Close( void );

	// Compatiable version
	BOOL ReadData( const TCHAR *szField, double &dNumber );
	BOOL ReadData( const TCHAR *szField, float &fNumber );

	BOOL ReadData( const TCHAR *szField, unsigned long &lNumber );

	BOOL ReadData( const TCHAR *szField, int &iNumber );
	BOOL ReadData( const TCHAR *szField, unsigned int &iNumber );
	BOOL ReadData( const TCHAR *szField, unsigned short &iNumber );
	BOOL ReadData( const TCHAR *szField, short &iNumber);
	BOOL ReadData( const TCHAR *szField, unsigned char &iNumber );
	BOOL ReadData( const TCHAR *szField, char &iNumber );

	BOOL ReadData( const TCHAR *szField, __int64 &i64Number );
	BOOL ReadData( const TCHAR *szField, DWORD64 &i64Number );

	BOOL ReadString( const TCHAR *szField, TCHAR *szString, DWORD dwSize );

	// Non-compatiable version
	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, double &dNumber );
	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, float &fNumber );

	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, unsigned long &lNumber );

	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, int &iNumber );
	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, unsigned int &iNumber );
	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, unsigned short &iNumber );
	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, short &iNumber);
	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, unsigned char &iNumber );
	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, char &iNumber );

	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, __int64 &i64Number );
	BOOL ReadData( const TCHAR *szSection, const TCHAR *szField, DWORD64 &i64Number );

	BOOL ReadString( const TCHAR *szSection, const TCHAR *szField, TCHAR *szString, DWORD dwSize );

private:
	TCHAR m_szFilename[ MAX_PATH ];
	TCHAR m_szSection[ MAX_PATH ];

	UINT m_uFlag;

private :
	enum {
		INIFILE_BUFFER_LEN	= 4096
	};

	BOOL Init( void );

	TCHAR m_szLocal[ INIFILE_BUFFER_LEN ];
};
