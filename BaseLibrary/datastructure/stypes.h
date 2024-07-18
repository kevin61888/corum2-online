/*********************************************************************************************************
 *
 *  Module Name:	stypes.h
 *
 *  Abstract:		Standarting associative types and definitions.
 *
 *  Author:		Vyacheslav I. Levtchenko (mail-to: sl@eltrast.ru)
 *
 *  Revision History:	05.08.2003	started
 *
 *  Classes, methods and structures:
 *
 *  TODO:
 *
 *********************************************************************************************************/

#ifndef __STYPES_H__
#define __STYPES_H__

typedef unsigned char		byte;
typedef unsigned short		word;
typedef unsigned int		dword;

typedef volatile bool		vbool;
typedef volatile unsigned short	vword;
typedef volatile unsigned long	vdword;
typedef volatile long		vint, vlong, vlng;

#if defined (__GNUC__)

typedef unsigned long long	ddword;
typedef unsigned long long	ulonglong;
typedef long long		sddword;
typedef long long		longlong;

#elif defined (_MSC_VER)

typedef unsigned __int64	ddword;
typedef unsigned __int64	ulonglong;
typedef __int64			sddword;
typedef __int64			longlong;

#endif

#if defined(VXD) || defined(_NTDDK_)

#  ifndef IN
#    define IN
#  endif

#  ifndef OUT
#    define OUT
#  endif

#  ifndef OPTIONAL
#    define OPTIONAL
#  endif

typedef unsigned char		BOOLEAN;
typedef int			BOOL;
typedef unsigned char		BYTE;
typedef unsigned short		WORD;
typedef unsigned long		DWORD;
typedef unsigned char*		PBYTE;
typedef unsigned short*		PWORD;
typedef unsigned long*		PDWORD;
typedef void*			LPVOID;

#ifdef __cplusplus
/* delete piece of heap */
#  define del(heap) if (heap) delete heap, heap = NULL; else brk ();

/* delete array of piece of heap */
#  define destroy(array) if (array) delete [] array, array = NULL; else brk ();
#endif

#endif

#include "debug.h"

#undef  _U
#define _U1(a,b) a##b
#define _U(c)	 _U1(L,c)

#endif // __STYPES_H__
