/*********************************************************************************************************
 *
 *  Module Name:	list_ex.h
 *
 *  Abstract:		Linux lists extension.
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

#ifndef __LIST_EX_H__
#define __LIST_EX_H__

#ifndef _LVM_H_INCLUDE
#  define _LVM_H_INCLUDE
#endif

#ifdef __cplusplus
extern "C" {

#  define new newx
#endif // __cplusplus

#ifdef _WIN32
/* Visual C++ uses the keyword "__inline" rather than "__inline__" */
#  define __inline__ __inline
#endif // _WIN32

#include <list.h>

typedef struct list_head list_head, *plh;

/*
 * list_body - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 */
#define list_body(ptr, type) (type)(ptr + 1)

__inline__ void clear_list(plh proot)
{
  plh hlh, next;

  if (list_empty(proot))
    return;

  list_for_each_safe(hlh, next, proot)
   {
    list_del(hlh);
    delete hlh;
   }
}

#include "debug.h"

__inline__ bool DelLE (plh pLH)
{
  if (!pLH) { brk (); return false; }

  list_del(pLH);
  delete pLH;
  return true;
}

#ifdef __cplusplus
#ifdef new
#  undef new
#endif // new
}
#endif // __cplusplus

#endif // __LIST_EX_H__
