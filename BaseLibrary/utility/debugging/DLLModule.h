#pragma once

class CDLLModule
{
protected:
	HMODULE		m_hDLL;


public:
	// constructor and destructor
	CDLLModule(void) : m_hDLL(NULL)
	{
	}
	virtual ~CDLLModule(void)	// free the library
	{
		if (m_hDLL)
			::FreeLibrary(m_hDLL);
	}
	//////////////////////////////////////////////////////////////////
	// See if dll been loaded, returning true dose not mean that all functions of the dll is valid.
	BOOL			IsLoaded(void)
	{
		return (m_hDLL != NULL);
	}

	/////////////////////////////////////////////////////////
	//	pure virtual, must implemented in derived class
	//	used macros to generate the implementation
	virtual BOOL	Init(LPCTSTR szDll) = 0;
};

//////////////////////////////////////////////////////////////////////
// macros to implement the Init function
#define DECLARE_DLL_FUNCTION(ret, cc, func, params)		ret	(cc *func)params;

//#define BEGIN_DLL_INIT() \
//		BOOL Init(LPCTSTR szDll) \
//		{ \
//			if (m_hDLL) \
//				::FreeLibrary(m_hDLL); \
//			m_hDLL = ::LoadLibrary(szDll); \
//			BOOL	bOk = FALSE;

#define INIT_DLL_TWIN_FUNCTION(ret, cc, func, params, origin) \
		if (m_hDLL) { \
			func = (ret(cc*)params)GetProcAddress(m_hDLL, origin); \
		} else \
			func = NULL; \
		if (func) \
			bOk = TRUE;

//#define END_DLL_INIT() \
//		return bOk; \
//	} 

#define INIT_DLL_FUNCTION(ret, cc, func, params) \
		if (m_hDLL) { \
			func = (ret(cc*)params)GetProcAddress(m_hDLL, #func); \
		} else \
			func = NULL; \
		if (func) \
			bOk = TRUE;
