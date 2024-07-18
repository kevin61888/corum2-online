//
// File: CCRC32.cpp
// Desc: CRC32 class
//
// Auth: Standil (cpascal@nownuri.net)
// Adapted from CodeGuru
//

#ifndef _CCRC32_H_
#define _CCRC32_H_

#define USE_DYNAMIC_CRC32			0

class CCRC32
{
public:
	CCRC32();
	~CCRC32();

	BOOL PutData( DWORD &dwCRC32, LPVOID lpData, DWORD dwSize ) const;
	BOOL PutData( DWORD &dwCRC32, LPCTSTR szString ) const;
	BOOL PutData( DWORD &dwCRC32, BYTE byData ) const;
	void InitValue( DWORD &dwCRC32 ) const;

//	DWORD *m_pdwCRC32Table;

protected:
//	void Init( void );
//	void Free( void );
	inline void CalcCrc32( const BYTE byte, DWORD &dwCrc32 ) const;
	static DWORD s_ArrayCRC32[256];
};

#endif /* _CCRC32_H_ */
