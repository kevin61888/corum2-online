#ifndef _CWORDSEARCHSIMPLE_H_
#define _CWORDSEARCHSIMPLE_H_

class CWordSearchSimple
{
public:
	CWordSearchSimple( void );
	~CWordSearchSimple( void );

	enum {
		HUGE_NUMBER = 32768
	};

protected:
	CHAR		m_szCurseWords[HUGE_NUMBER];
	BOOL		m_bInit;

protected:
	LPSTR		GetField( LPCSTR szBuffer, int delim, LPSTR szField );
	void		FilterChar( LPCSTR szSource, LPSTR szDest, LPCSTR szChars );

public:
	BOOL		AddWord( LPCTSTR szWord );

	TCHAR *		ReplaceString( LPTSTR szString );
	BOOL		JustSearchString( LPCTSTR szString );
};

#endif /* _CWORDSEARCHSIMPLE_H_ */
