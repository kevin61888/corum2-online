/*********************************************************************************************************
 *
 *  Module Name:	pathstor.h
 *
 *  Abstract:		Simple File Name Path Cache Storage (SFNPCS). Serve for accelerating performance
 *			of heap manager on fixed blocks.
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

#ifndef __PATHSTOR_H__
#define __PATHSTOR_H__

template <long Tsize, class Tpath>
class PathStorage
{
#include <pshpack1.h>
  typedef struct PathElem
   {
    long ref;
    Tpath Path [Tsize];
   } pe, *ppe;
#include <poppack.h>

  long MaxElem;
  ppe Storage, TopStorage;

public:
  PathStorage (const long NumElem = 32)
   {
    MaxElem = NumElem;
    Storage = new pe [MaxElem];

    if (!Storage) { brk(); return; }

    TopStorage = Storage + MaxElem;

    ppe p = Storage;

    for (long i = 0; i < MaxElem; i++, p++)
      p->ref = 0;
   }

  ~PathStorage (void) { if (Storage) delete Storage; }

  Tpath* Get (const long Size = Tsize) /* Size is amount of symbols <Tpath> in path string */
   {
    if (!Storage || !MaxElem || !Size) { brk (); return NULL; }

    if (Size > Tsize)
     {
      brk ();
      return new Tpath [Size];
     }

    ppe p = Storage;

    /* Move to ahead of cache array */
    for (long i = 0, ref = 0; i < MaxElem && (1 != (ref = InterlockedIncrement(&p->ref))); InterlockedDecrement(&p->ref), i++, p++) {;}

    if (1 == ref)
      return p->Path;
    else
     {
      tbrk ();
      return new Tpath [Tsize];
     }
   }

  void Free (Tpath* Path)
   {
    if (!Storage || !MaxElem || !Path) { brk (); return; }

    ppe p = (ppe)((byte*)Path - sizeof (long));

    if (p < Storage || p > TopStorage)
     {
      tbrk ();
      delete Path;
     }
    else
      InterlockedDecrement(&p->ref);
   }
};

#endif
