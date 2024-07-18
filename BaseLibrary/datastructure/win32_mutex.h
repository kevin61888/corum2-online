/*********************************************************************************************************
 *
 *  Module Name:	win32_mutex.h
 *
 *  Abstract:		Win32 mutexes.
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

#ifndef __WIN32_MUTEX_H__
#define __WIN32_MUTEX_H__

/* Map Locker - MLocker */
class MLocker
{
protected:
  CRITICAL_SECTION m_CriticalSection;

public:
  static byte ProcNum;

  void Init (byte ProcessorsNumber = 1) /* ProcessorsNumber for compability */
   {
    ProcNum = ProcessorsNumber;

    InitializeCriticalSection (&m_CriticalSection);
   }

  MLocker (byte ProcessorsNumber = 1)
   {
    Init (ProcessorsNumber);
   }

  ~MLocker()
   {
    DeleteCriticalSection (&m_CriticalSection);
   }

  void Lock  (void)
   {
    EnterCriticalSection (&m_CriticalSection);
   }

  void UnLock(void)
   {
    LeaveCriticalSection (&m_CriticalSection);
   }
};

/* Pipe Locker - PLocker */
/* ClientNum < 2 - no locking */
class PLocker
{
  CRITICAL_SECTION m_CriticalSection;
  dword ClientNum;

public:
  void Init (dword ClientsNumber, byte ProcessorsNumber = 1) /* ProcessorsNumber for compability */
   {
    ClientNum = ClientsNumber;

    if (ClientNum < 2)
      return;

    InitializeCriticalSection (&m_CriticalSection);
   }

  PLocker (dword ClientsNumber, byte ProcessorsNumber = 1)
   {
    Init (ClientsNumber, ProcessorsNumber);
   }

  ~PLocker ()
   {
    if (ClientNum < 2)
      return;

    DeleteCriticalSection (&m_CriticalSection);
   }

  void Lock (void)
   {
    if (ClientNum < 2)
      return;

    EnterCriticalSection (&m_CriticalSection);
   }

  void UnLock (void)
   {
    if (ClientNum < 2)
      return;

    LeaveCriticalSection (&m_CriticalSection);
   }
};

#endif // __WIN32_MUTEX_H__
