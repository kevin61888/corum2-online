#ifndef _DEBUG_UTILS_H_
#define _DEBUG_UTILS_H_

#include <winsock2.h>
#include <windows.h>
#include <tchar.h>

namespace DbgUtils
{
	class   CConvertErrorToText;
	void    SetProgramName(PTSTR pszOutBuffer, int nBufferSize, PTSTR pszProgamName = NULL);	
};


class DbgUtils::CConvertErrorToText
{
protected:

	HMODULE		m_hNetMsg;

public:
	// constructor and destructor
	CConvertErrorToText();
	virtual ~CConvertErrorToText();

    inline static CConvertErrorToText& Instance() { static CConvertErrorToText Instance; return Instance; }
	DWORD GetErrorTextBuffer(LPTSTR lpMessage, DWORD dwBufferLen, DWORD dwLastError);
};

#ifdef _DEBUG
#define New		new( _CLIENT_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the allocations to be of _CLIENT_BLOCK type
#else
#define New		new
#endif // _DEBUG

#endif