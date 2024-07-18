#include "../../StdAfx.h"
#include "SystemInfo.h"
#include <BaseLibrary/Utility/String/StringConvert.h>

// These are the bit flags that get set on calling cpuid
// with register eax set to 1
#define _MMX_FEATURE_BIT        0x00800000
#define _SSE_FEATURE_BIT        0x02000000
#define _SSE2_FEATURE_BIT       0x04000000

// This bit is set when cpuid is called with
// register set to 80000001h (only applicable to AMD)
#define _3DNOW_FEATURE_BIT      0x80000000

CSystemInfo::CSystemInfo( void )
{
	CheckOSType();
	CheckCPUID();
}

CSystemInfo::~CSystemInfo(void)
{
}

bool CSystemInfo::CheckCPUID(void)
{
    DWORD dwStandard = 0;
    DWORD dwFeature = 0;
    DWORD dwMax = 0;
    DWORD dwExt = 0;
    int feature = 0;
    int os_support = 0;
    union {
        char cBuf[12+1];
        struct {
            DWORD dw0;
            DWORD dw1;
            DWORD dw2;
        } s;
    } Ident;

	if(HaveCPUID() == false) {
		return false;
	}
    _asm {
        push ebx
        push ecx
        push edx

        // get the vendor string
        xor eax, eax
        cpuid
        mov dwMax, eax
        mov Ident.s.dw0, ebx
        mov Ident.s.dw1, edx
        mov Ident.s.dw2, ecx

        // get the Standard bits
        mov eax, 1
        cpuid
        mov dwStandard, eax
        mov dwFeature, edx

        // get AMD-specials
        mov eax, 80000000h
        cpuid
        cmp eax, 80000000h
        jc notamd
        mov eax, 80000001h
        cpuid
        mov dwExt, edx

notamd:
        pop ecx
        pop ebx
        pop edx
    }

    if (dwFeature & _MMX_FEATURE_BIT) {
        feature |= _CPU_FEATURE_MMX;
        if (CheckCPUID_OSSupport(_CPU_FEATURE_MMX))
            os_support |= _CPU_FEATURE_MMX;
    }
    if (dwExt & _3DNOW_FEATURE_BIT) {
        feature |= _CPU_FEATURE_3DNOW;
        if (CheckCPUID_OSSupport(_CPU_FEATURE_3DNOW))
            os_support |= _CPU_FEATURE_3DNOW;
    }
    if (dwFeature & _SSE_FEATURE_BIT) {
        feature |= _CPU_FEATURE_SSE;
        if (CheckCPUID_OSSupport(_CPU_FEATURE_SSE))
            os_support |= _CPU_FEATURE_SSE;
    }
    if (dwFeature & _SSE2_FEATURE_BIT) {
        feature |= _CPU_FEATURE_SSE2;
        if (CheckCPUID_OSSupport(_CPU_FEATURE_SSE2))
            os_support |= _CPU_FEATURE_SSE2;
    }

    m_CPUID_INFO.os_support = os_support;
	m_CPUID_INFO.feature = feature;
    m_CPUID_INFO.family = (dwStandard >> 8) & 0xF; // retrieve family
    if (m_CPUID_INFO.family == 15) {               // retrieve extended family
        m_CPUID_INFO.family |= (dwStandard >> 16) & 0xFF0;
    }
    m_CPUID_INFO.model = (dwStandard >> 4) & 0xF;  // retrieve model
    if (m_CPUID_INFO.model == 15) {                // retrieve extended model
        m_CPUID_INFO.model |= (dwStandard >> 12) & 0xF;
    }
    m_CPUID_INFO.stepping = (dwStandard) & 0xF;    // retrieve stepping

    Ident.cBuf[12] = 0;
	       if( strcmp( "GenuineIntel", Ident.cBuf ) == 0 ) {
		m_CPUID_INFO.VendorNo = CSystemInfo::Intel;
	} else if( strcmp( "AuthenticAMD", Ident.cBuf ) == 0 ) {
		m_CPUID_INFO.VendorNo = CSystemInfo::AMD;
	} else if( strcmp( "CyrixInstead", Ident.cBuf ) == 0 ) {
		m_CPUID_INFO.VendorNo = CSystemInfo::Cyrix;
	} else if( strcmp( "CentaurHauls", Ident.cBuf ) == 0 ) {
		m_CPUID_INFO.VendorNo = CSystemInfo::Centaur;
	} else {
		m_CPUID_INFO.VendorNo = CSystemInfo::Unknown;
	}
    strcpy(m_CPUID_INFO.v_name, Ident.cBuf);

//    map_mname(m_CPUID_INFO.family, m_CPUID_INFO.model, m_CPUID_INFO.v_name, m_CPUID_INFO.model_name);

    m_CPUID_INFO.checks = _CPU_FEATURE_MMX | _CPU_FEATURE_SSE | _CPU_FEATURE_SSE2 | _CPU_FEATURE_3DNOW;

	return true;
}

bool CSystemInfo::CheckCPUID_OSSupport( int feature )
{
#if _MSC_VER > 1200
    __try {
        switch (feature) {
        case _CPU_FEATURE_SSE:
            __asm {
                xorps xmm0, xmm0        // executing SSE instruction
            }
            break;
        case _CPU_FEATURE_SSE2:
            __asm {
                xorpd xmm0, xmm0        // executing SSE2 instruction
            }
            break;
        case _CPU_FEATURE_3DNOW:
            __asm {
                pfrcp mm0, mm0          // executing 3DNow! instruction
                emms
            }
            break;
        case _CPU_FEATURE_MMX:
            __asm {
                pxor mm0, mm0           // executing MMX instruction
                emms
            }
            break;
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION) {
            return false;
        }
        return false;
    }
    return true;
#else
#pragma note("MSVC++ 6.0에선 XORPS assembly 미지원")
	if( feature != _CPU_FEATURE_MMX )
		return false;
    int nCPUFeatures=0;
	try {
		__asm {
			; we must push/pop the registers << CPUID>>  writes to, as the
			; optimiser doesn't know about << CPUID>> , and so doesn't expect
			; these registers to change.
			push eax
			push ebx
			push ecx
			push edx

			; << CPUID>> 
			; eax=0,1,2 -> CPU info in eax,ebx,ecx,edx
			mov eax,1
			_emit 0x0f
			_emit 0xa2
			mov nCPUFeatures,edx

			pop edx
			pop ecx
			pop ebx
			pop eax
		}
    }
    catch(...) {
		return false;
	}
	return (nCPUFeatures & 0x00800000) != 0;
#endif
}

bool CSystemInfo::HaveCPUID(void)
{
    __try {
        _asm {
            xor eax, eax
            cpuid
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
    return true;
}

LPCTSTR CSystemInfo::GetCPUIDVendorName(void)
{
	switch( m_CPUID_INFO.VendorNo ) {
	case CSystemInfo::Intel   :
		return "Intel";
	case CSystemInfo::AMD     :
		return "AMD";
	case CSystemInfo::Cyrix   :
		return "Cyrix";
	case CSystemInfo::Centaur :
		return "Centaur";
	case CSystemInfo::Unknown :
		return "Unknown";
	}
	return "Unknown";
}

LPCTSTR CSystemInfo::GetCPUIDProductName(void)
{
	switch( m_CPUID_INFO.VendorNo ) {
	case CSystemInfo::AMD     :
		switch( m_CPUID_INFO.family ) {
        case 4: // Am486/AM5x86
			return "Am486";
        case 5: // K6
            switch( m_CPUID_INFO.model ) {
            case 0:
            case 1:
            case 2:
            case 3:
				return "K5";
            case 6:
            case 7:
				return "K6";
            case 8:
				return "K6-2";
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
				return "K6-3";
            }
            break;
        case 6: // Athlon
            // No model numbers are currently defined
			return "Athlon";
        }
		break;
	case CSystemInfo::Intel   :
        switch( m_CPUID_INFO.family ) { // extract family code
        case 4:
            switch( m_CPUID_INFO.model ) { // extract model code
            case 0:
            case 1:
				return "486DX";
            case 2:
				return "486SX";
            case 3:
				return "486DX2";
            case 4:
				return "486SL";
            case 5:
				return "486SX2";
            case 7:
				return "486DX2E";
            case 8:
				return "486DX4";
            }
            break;
        case 5:
            switch( m_CPUID_INFO.model ) { // extract model code
            case 1:
            case 2:
            case 3:
				return "Pentium";
            case 4:
				return "Pentium-MMX";
            }
            break;
        case 6:
            switch( m_CPUID_INFO.model ) { // extract model code
            case 1:
				return "Pentium-Pro";
            case 3:
            case 5:
				return "Pentium-II"; // actual differentiation depends on cache settings
            case 6:
				return "Celeron";
            case 7:
            case 8:
            case 10:
				return "Pentium-III"; // actual differentiation depends on cache settings
            }
            break;
        case 15 | (0x00 << 4): // family 15, extended family 0x00
            switch ( m_CPUID_INFO.model ) {
            case 0:
            case 1:
            case 2:
				return "Pentium-4";
            }
            break;
        }
	case CSystemInfo::Cyrix   :
		return "Cyrix";
	case CSystemInfo::Centaur :
		return "Centaur";
	case CSystemInfo::Unknown :
		return "Unknown";
	}
	return "Unknown";
}

LPCTSTR CSystemInfo::GetDiskSpaceInfo(void)
{
	static CHAR szBuffer[1024] = "";
	CHAR szSmallBuffer[64];
	DWORD dwDriveType;
	ULARGE_INTEGER uliFreeBytesAvailable, uliTotalNumberOfBytes, uliTotalNumberOfFreeBytes;

	DWORD dwDrives = GetLogicalDrives();
	DWORD dwDriveBit = 0x00000001;
	for( int i=0; i < 26; i++ ) {
		if( dwDrives & dwDriveBit ) {
			sprintf( szSmallBuffer, "%c:\\", 'A' + i );
			dwDriveType = GetDriveType( szSmallBuffer );
			if( (dwDriveType == DRIVE_FIXED) || (dwDriveType == DRIVE_REMOTE) ) {
				if( !GetDiskFreeSpaceEx( szSmallBuffer, &uliFreeBytesAvailable, &uliTotalNumberOfBytes, &uliTotalNumberOfFreeBytes ) )
					continue;
				CHAR szSize1[16], szSize2[16];

				AdvI64cvt1024( uliFreeBytesAvailable.QuadPart, szSize1 );
				AdvI64cvt1024( uliTotalNumberOfBytes.QuadPart, szSize2 );
				sprintf( szBuffer + strlen( szBuffer ), "%c: (FREE: %s/ TOTAL: %s), ", 'A' + i, szSize1, szSize2 );
			} else {
				sprintf( szBuffer + strlen( szBuffer ), "%c:, ", 'A' + i );
			}
		}
		dwDriveBit <<= 1;
	}
	return szBuffer;
}

bool CSystemInfo::CheckOSType(void)
{
	DWORD dwLen;

	dwLen = _MAX_PATH;
	::GetUserName( m_szUserName, &dwLen );
	dwLen = _MAX_PATH;
	::GetComputerName( m_szComputerName, &dwLen);
	m_OSVERSIONINFO.dwOSVersionInfoSize = sizeof( OSVERSIONINFOEX );
	return ( GetVersionEx( (LPOSVERSIONINFO) &m_OSVERSIONINFO ) == TRUE );
}

DWORD CSystemInfo::GetOSType( void )
{
	switch( m_OSVERSIONINFO.dwPlatformId ) {
	case VER_PLATFORM_WIN32s		:
		return _OS_WIN30;
	case VER_PLATFORM_WIN32_WINDOWS	:
		if( m_OSVERSIONINFO.dwMajorVersion == 4 ) {
			if( m_OSVERSIONINFO.dwMinorVersion == 0 ) {
				if( m_OSVERSIONINFO.szCSDVersion[1] == 'C' || m_OSVERSIONINFO.szCSDVersion[1] == 'B' )
					return _OS_WIN95OSR2;
				return _OS_WIN95;
			} else if( m_OSVERSIONINFO.dwMinorVersion == 10 ) {
				if( m_OSVERSIONINFO.szCSDVersion[1] == 'A' )
					return _OS_WIN98SE;
				return _OS_WIN98;
			}
			else if( m_OSVERSIONINFO.dwMinorVersion == 80 )
				return _OS_WINME;
			else
				return _OS_WINME_NEWER;
		}
		return _OS_WINME_NEWER;
	case VER_PLATFORM_WIN32_NT		:
		if ( m_OSVERSIONINFO.dwMajorVersion < 4 )
			return _OS_WINNT3;
		if ( m_OSVERSIONINFO.dwMajorVersion == 4 )
			return _OS_WINNT4;
		if ( m_OSVERSIONINFO.dwMajorVersion == 5 ) {
			if ( m_OSVERSIONINFO.dwMinorVersion == 0 )
				return _OS_WIN2000;
			else if ( m_OSVERSIONINFO.dwMinorVersion == 1 )
				return _OS_WINXP;
			else if ( m_OSVERSIONINFO.dwMinorVersion == 2 )
				return _OS_WIN2003;
		}
		return _OS_WIN2003_NEWER;
	}
	return _OS_UNKNOWN;
}

LPCTSTR CSystemInfo::GetOSTypeName( void )
{
	switch( GetOSType() ) {
	case _OS_WIN30			: return _T( "Windows 3.x" );
	case _OS_WIN95			: return _T( "Windows 95" );
	case _OS_WIN95OSR2		: return _T( "Windows 95 OSR2" );
	case _OS_WIN98			: return _T( "Windows 98" );
	case _OS_WIN98SE		: return _T( "Windows 98 SE" );
	case _OS_WINME			: return _T( "Windows ME" );
	case _OS_WINME_NEWER	: return _T( "Windows ME or newer" );
	case _OS_WINNT3			: return _T( "Windows NT4" );
	case _OS_WINNT4			: return _T(" Windows NT5" );
	case _OS_WIN2000		: return _T( "Windows 2000" );
	case _OS_WINXP			: return _T( "Windows XP" );
	case _OS_WIN2003		: return _T( "Windows 2003" );
	case _OS_WIN2003_NEWER	: return _T( "Windows 2003 or newer" );
	}
	return _T( "Unknown" );
}

DWORD CSystemInfo::GetOSVersion( void )
{
	return ( m_OSVERSIONINFO.dwPlatformId << 16 | m_OSVERSIONINFO.dwMajorVersion << 8 | m_OSVERSIONINFO.dwMinorVersion );
}

LPCTSTR CSystemInfo::GetOSVersionName( void )
{
	m_szVersionName[0] = _T( '\0' );
	if( m_OSVERSIONINFO.wServicePackMajor > 0 )
		_stprintf( m_szVersionName + _tcslen( m_szVersionName ), _T( "SP%d " ), m_OSVERSIONINFO.wServicePackMajor );
	_stprintf( m_szVersionName + _tcslen( m_szVersionName ), _T( "%d.%d " ), m_OSVERSIONINFO.dwMajorVersion, m_OSVERSIONINFO.dwMinorVersion );
	_stprintf( m_szVersionName + _tcslen( m_szVersionName ), _T( "(Build %d)" ), m_OSVERSIONINFO.dwBuildNumber );
	return m_szVersionName;
}

LPCTSTR CSystemInfo::GetComputerName( void )
{
	return m_szComputerName;
}

LPCTSTR CSystemInfo::GetUserName( void )
{
	return m_szUserName;
}

