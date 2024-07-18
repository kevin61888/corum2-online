#ifndef		__FORBID_WORD_HEADER__
#define		__FORBID_WORD_HEADER__

#include <list>

static char	g_szDefaultFilename[] = "sfilter.dat";
class CFORBIDWORD
{
public:
	CFORBIDWORD();
	virtual ~CFORBIDWORD();
	static CFORBIDWORD*	m_gpForbidWord;

public:
	void	LoadForbidWord( char* szFilename = NULL );
	void	UnloadForbidWord();
	bool	IsForbidWord( char* szString );

protected:
	bool						m_bCheck;
	std::list< std::string >	m_lSlander;

};	typedef CFORBIDWORD*	LPCFORBIDWORD;


inline LPCFORBIDWORD FORBIDWORD()
{
	if( CFORBIDWORD::m_gpForbidWord == NULL )
		CFORBIDWORD::m_gpForbidWord = new CFORBIDWORD;
	return CFORBIDWORD::m_gpForbidWord;
}
inline void DESTORYFOBIDWORD()
{
	if( CFORBIDWORD::m_gpForbidWord != NULL )
		delete CFORBIDWORD::m_gpForbidWord;
	CFORBIDWORD::m_gpForbidWord = NULL;
}

#endif//	__FORBID_WORD_HEADER__
