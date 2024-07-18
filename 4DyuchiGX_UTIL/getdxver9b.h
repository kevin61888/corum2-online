#ifndef	_GetDXVer_h_
#define	_GetDXVer_h_

#include <windows.h>


//-----------------------------------------------------------------------------
// Name: GetDXVersion()
// Desc: This function returns the DirectX version.
// Arguments: 
//      pdwDirectXVersion - This can be NULL.  If non-NULL, the return value is:
//              0x00000000 = No DirectX installed
//              0x00010000 = DirectX 1.0 installed
//              0x00020000 = DirectX 2.0 installed
//              0x00030000 = DirectX 3.0 installed
//              0x00030001 = DirectX 3.0a installed
//              0x00050000 = DirectX 5.0 installed
//              0x00060000 = DirectX 6.0 installed
//              0x00060100 = DirectX 6.1 installed
//              0x00060101 = DirectX 6.1a installed
//              0x00070000 = DirectX 7.0 installed
//              0x00070001 = DirectX 7.0a installed
//              0x00080000 = DirectX 8.0 installed
//              0x00080100 = DirectX 8.1 installed
//              0x00080101 = DirectX 8.1a installed
//              0x00080102 = DirectX 8.1b installed
//              0x00080200 = DirectX 8.2 installed
//              0x00090000 = DirectX 9.0 installed
//              0x00090002 = DirectX 9.0b installed
//      strDirectXVersion - Destination string to receive a string name of the DirectX Version.  Can be NULL.
//      cchDirectXVersion - Size of destination buffer in characters.  Length should be at least 10 chars.
// Returns: S_OK if the function succeeds.  
//          E_FAIL if the DirectX version info couldn't be determined.
//
// Please note that this code is intended as a general guideline. Your
// app will probably be able to simply query for functionality (via
// QueryInterface) for one or two components.
//
// Also please ensure your app will run on future releases of DirectX.
// For example:
//     "if( dwDirectXVersion != 0x00080100 ) return false;" is VERY BAD. 
//     "if( dwDirectXVersion < 0x00080100 ) return false;" is MUCH BETTER.
//-----------------------------------------------------------------------------


HRESULT GetDXVersion( DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion );

//extern DWORD GetDXVersion();

#endif