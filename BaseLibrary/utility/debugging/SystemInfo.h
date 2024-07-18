#pragma once

#include <stdlib.h>

#define _CPU_FEATURE_MMX    0x0001
#define _CPU_FEATURE_SSE    0x0002
#define _CPU_FEATURE_SSE2   0x0004
#define _CPU_FEATURE_3DNOW  0x0008

#define _MAX_VNAME_LEN  13
#define _MAX_MNAME_LEN  30

enum {
	_OS_WIN30			= 0x00000001,
	_OS_WIN95			= 0x00000002,
	_OS_WIN95OSR2		= 0x00000004,
	_OS_WIN98			= 0x00000008,
	_OS_WIN98SE			= 0x00000010,
	_OS_WINME			= 0x00000020,
	_OS_WINME_NEWER		= 0x00000040,
	_OS_WIN9XME			= 0x0000007E,

	_OS_WINNT3			= 0x00000100,
	_OS_WINNT4			= 0x00000200,
	_OS_WIN2000			= 0x00000400,
	_OS_WINXP			= 0x00000800,
	_OS_WIN2003			= 0x00001000,
	_OS_WIN2003_NEWER	= 0x00002000,
	_OS_WIN2KXP			= 0x00003F00,

	_OS_UNIX			= 0x00FFF000,
	_OS_MAC				= 0x07000000,
	_OS_DOS				= 0x70000000,

	_OS_UNKNOWN			= 0x80000000
};

class CSystemInfo
{
public:
	CSystemInfo( void );
	~CSystemInfo( void );

private:
	typedef enum { Intel, AMD, Cyrix, Centaur, Unknown } CPUID_Vendor;

	typedef struct strCPUID_INFO {
		CPUID_Vendor VendorNo;				// vendor no
	    char v_name[_MAX_VNAME_LEN];        // vendor name
	    char model_name[_MAX_MNAME_LEN];    // name of model			e.g. Intel Pentium-Pro
		int family;                         // family of the processor  e.g. 6 = Pentium-Pro architecture
		int model;                          // model of processor		e.g. 1 = Pentium-Pro for family = 6
		int stepping;                       // processor revision number
		int feature;                        // processor feature (same as return value from _cpuid)
		int os_support;                     // does OS Support the feature?
		int checks;                         // mask of checked bits in feature and os_support fields
	} CPUID_INFO;
	TCHAR m_szUserName[_MAX_PATH];
	TCHAR m_szComputerName[_MAX_PATH];
	TCHAR m_szVersionName[_MAX_PATH];

private:
	// CPUID
	bool CheckCPUID( void );
	bool HaveCPUID( void );
	bool CheckCPUID_OSSupport( int feature );

	bool CheckOSType( void );

	CPUID_INFO		m_CPUID_INFO;
	OSVERSIONINFOEX	m_OSVERSIONINFO;

public:
	LPCTSTR GetCPUIDVendorName( void );
	LPCTSTR GetCPUIDProductName( void );

	LPCTSTR GetComputerName( void );
	LPCTSTR GetUserName( void );

	DWORD	GetOSType( void );				// 위에 enum으로 선언된 OS type 반환
	LPCTSTR GetOSTypeName( void );
	DWORD	GetOSVersion( void );
	LPCTSTR	GetOSVersionName( void );

	LPCTSTR GetDiskSpaceInfo( void );

	BOOL IsMMX( void )		{ return (m_CPUID_INFO.os_support & _CPU_FEATURE_MMX);	}
	BOOL Is3DNow( void )	{ return (m_CPUID_INFO.os_support & _CPU_FEATURE_3DNOW);	}
	BOOL IsSSE( void )		{ return (m_CPUID_INFO.os_support & _CPU_FEATURE_SSE);	}
	BOOL IsSSE2( void )		{ return (m_CPUID_INFO.os_support & _CPU_FEATURE_SSE2);	}

};
