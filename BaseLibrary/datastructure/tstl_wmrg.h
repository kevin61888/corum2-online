/*********************************************************************************************************
 *
 *  Module Name:	tstl_wmrg.h
 *
 *  Abstract:		Writer multiply reader guard.
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

#ifndef __TSTL_WMRG_H__
#define __TSTL_WMRG_H__

/* Writer multiply reader guard */
class WMRG
{
  long Readers, Status, ManyWriters;
  MLocker csResource, csWrite;

  /* Change status of cache element from XXX -> (to) YYY */
  static inline long ChangeStatus (IN OUT long& Status, IN long NewStatus, IN long PreviosStatus)
   {
    return (long)InterlockedCompareExchange ((PVOID*)&Status, (PVOID)NewStatus, (PVOID)PreviosStatus);
   }

public:
  /* Constructor */
  WMRG (const long ManyWriter = 0) : Readers(0), Status(FREE_SIGNATURE), csResource (MANY), csWrite (MANY)
   {
    ManyWriters = ManyWriter;
   }

  /* Destructor  */
  ~WMRG (void)
   {
    if (Readers) brk ();
   }

  /* Call this to gain shared read access */
  bool WaitToRead ()
   {
    LOCK_RESOURCE(Status);

    if (1 == InterlockedIncrement (&Readers))
      csResource.Lock ();

    UNLOCK_RESOURCE(Status);
    return true;
   }

  /* Call this when done accessing the resource */
  void ReadingDone ()
   {
    LOCK_RESOURCE(Status);

    if (!InterlockedDecrement (&Readers))
      csResource.UnLock ();

    UNLOCK_RESOURCE(Status);
   }

  /* Call this to gain exclusive write access */
  bool WaitToWrite ()
   {
    if (ManyWriters)
      csWrite.Lock ();

    csResource.Lock ();
    return true;
   }

  /* Call this when done accessing the resource */
  void WritingDone ()
   {
    csResource.UnLock ();

    if (ManyWriters)
      csWrite.UnLock ();
   }
};

#endif // __TSTL_WMRG_H__
