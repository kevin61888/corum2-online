#ifndef __SAFE_MEMORY_H__
#define __SAFE_MEMORY_H__

#pragma once

#include <crtdbg.h>

#ifdef __SAFE_MEMORY
#	define SAFEMEMCPY(des, src, size) __SafeMemCpy(des, src, size, __FILE__, __LINE__)
#	define SAFEMEMSET(des, val, size) __SafeMemSet(des, val, size, __FILE__, __LINE__)	
#else
#	define SAFEMEMCPY(des, src, size) memcpy(des, src, size)
#	define SAFEMEMSET(des, val, size) memset(des, val, size)	
#endif
	

inline void* __SafeMemCpy(void* pDes, const void* pSrc, size_t size, char* szSource, int iLine)
{
	if(IsBadWritePtr(pDes, size) || IsBadReadPtr(pSrc, size))
	{
		char szMessage[0xff]={0,};
		wsprintf(szMessage, "memcpy in %s file %u line", szSource, iLine);
		MessageBox(NULL, szMessage, "Memory Fault", MB_ICONERROR|MB_OK);
		__asm int 3
	}

	return memcpy(pDes, pSrc, size);
}

inline void* __SafeMemSet(void* pDes, int iVal, size_t size, char* szSource, int iLine)
{
	if(IsBadWritePtr(pDes, size))
	{
		char szMessage[0xff]={0,};
		wsprintf(szMessage, "memset in %s file %u line", szSource, iLine);
		MessageBox(NULL, szMessage, "Memory Fault", MB_ICONERROR|MB_OK);
		__asm int 3
	}
	
	return memset(pDes, iVal, size);
}


#endif // __SAFE_MEMORY_H__