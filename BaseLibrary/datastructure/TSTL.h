/*********************************************************************************************************
 *
 *  Module Name:	TSTL.h
 *
 *  Abstract:		TSTL main include.
 *
 *  Author:		Vyacheslav I. Levtchenko (mail-to: sl@eltrast.ru). 
 *			Big thanks to Andrew Zabolotny <anpaza@mail.ru> for gas inline macroses.
 *
 *  Revision History:	05.08.2003	started
 *
 *  Classes, methods and structures:
 *
 *  TODO: Platform i486+. Timercache uses Pentium instruct set.
 *
 *********************************************************************************************************/

#ifndef __TSTL_H__
#define __TSTL_H__

/* Common defines */

#define BUBLE_BOOSTER

#define MANY		 100
#define SPINLOCK_COUNTER 500

#define MINUS_MEDIAN	 0x1000
#define MINUS_NULL	 ((unsigned long)(0 - MINUS_MEDIAN))

/* Make signature from chars */
#define SIGN(A, B, C, D) ( ((unsigned long)(D) << 24) | ((unsigned long)(C) << 16) | ((unsigned long)(B) << 8) | (unsigned long)(A) )

/* Objects Status Definition Signatures (OSDS) */
#define FREE_SIGNATURE	SIGN ('F','R','E','E')	/* I'm "FREE"	!!! */
#define BUSY_SIGNATURE	SIGN ('B','U','S','Y')	/* I'm "BUSY"	!!! */
#define LIVE_SIGNATURE	SIGN ('L','I','V','E')	/* I'm "LIVE"	!!! */
#define KILL_SIGNATURE	SIGN ('K','I','L','L')	/* I'm "KILLED"	!!! */
#define DEAD_SIGNATURE	SIGN ('D','E','A','D')	/* I'm "DEAD"	!!! */
#define ERAS_SIGNATURE	SIGN ('E','R','A','S')	/* I'm "ERASED"	!!! */

/* Enter to fast mutex */
#define LOCK_RESOURCE(Status) \
  while (FREE_SIGNATURE != ChangeStatus (Status, BUSY_SIGNATURE, FREE_SIGNATURE)) \
    Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);

/* Leave fast mutex */
#define UNLOCK_RESOURCE(Status) \
  if (BUSY_SIGNATURE != ChangeStatus (Status, FREE_SIGNATURE, BUSY_SIGNATURE)) brk ();

/* Inline macroses */
#if defined (_MSC_VER)

#  if defined (VXD)
#    define InterlockedIncrement	_InterlockedIncrement
#    define InterlockedDecrement	_InterlockedDecrement
#    define InterlockedExchangeAdd	_InterlockedExchangeAdd
#    define InterlockedCompareExchange	_InterlockedCompareExchange

#    if defined (__cplusplus)
extern "C" {
#    endif

long InterlockedIncrement   (long* Addend);
long InterlockedDecrement   (long* Addend);
long InterlockedExchangeAdd (long* Addend, long Value);

/* Stupid prototype definition by $MS - fucking shit !!! %-((( */
void* InterlockedCompareExchange (void** Destination, void* Exchange, void* Comperand);

#    if defined (__cplusplus)
};
#    endif

#    pragma intrinsic(_InterlockedIncrement)
#    pragma intrinsic(_InterlockedDecrement)
#    pragma intrinsic(_InterlockedExchangeAdd)
#    pragma intrinsic(_InterlockedCompareExchange)
#  endif

#  if defined (BUBLE_BOOSTER)
#    pragma warning(disable:4035) /* re-enable below */
static __inline unsigned __int64 InterlockedCompareExchange64 (IN unsigned __int64* Destination, IN unsigned __int64* Exchange, IN unsigned __int64* Comperand)
{
  __asm {
    mov   esi, Comperand ; Comperand
    mov   eax, [esi + 0]
    mov   edx, [esi + 4]

    mov   esi, Exchange  ; Exchange
    mov   ebx, [esi + 0]
    mov   ecx, [esi + 4]

    mov   esi, Destination ; Destination
    lock  cmpxchg8b [esi]
   }
}

static __inline unsigned __int64 InterlockedRotate (IN unsigned __int64* Destination, IN unsigned __int64* Comperand)
{
  __asm {
    mov   ecx, Comperand ; Comperand
    mov   eax, [ecx + 0]
    mov   edx, [ecx + 4]

    ; rotate low and high part
    mov   ebx, edx ; load low part
    mov   ecx, eax ; load high part

    mov   esi, Destination ; Destination
    lock  cmpxchg8b [esi]
   }
}

#    pragma warning(default:4035)
#  endif

static __inline void SpinLock (unsigned long* pSpinLock)
{
  __asm {
     mov  ecx, pSpinLock

spinlock:
     lock bts dword ptr [ecx], 0
     jnb  exit
spin:
     test dword ptr [ecx], 1
     jz   spinlock ; tray to lock spin

     repz nop  ; wait for next check
     jmp  spin ; goto to next check
exit:
   }
}

#  if defined (_NTDDK_) || (VXD)
#    define LOCK_THREAD_SWITCHING()	  { _asm cli }
#    define UNLOCK_THREAD_SWITCHING()	  { _asm sti }
#    define PUSH_LOCK_THREAD_SWITCHING()  { _asm pushfd _asm cli }
#    define POP_UNLOCK_THREAD_SWITCHING() { _asm popfd }
#  else
#    define LOCK_THREAD_SWITCHING()	  { ; }
#    define UNLOCK_THREAD_SWITCHING()	  { ; }
#    define PUSH_LOCK_THREAD_SWITCHING()  { ; }
#    define POP_UNLOCK_THREAD_SWITCHING() { ; }

static __inline void* InterlockedCompareExchange (void** Destination, void* Exchange, void* Comperand)
{
  return (void*)InterlockedCompareExchange ((long*)Destination, (long)Exchange, (long)Comperand);
}
#  endif

#elif defined (__GNUC__)

/* Big thanks to Andrew Zabolotny <anpaza@mail.ru> for that macroses. */

static inline long InterlockedIncrement (long* Addend)
{
  const long one = 1;
  long res;
  asm (	"	lock xadd %0,(%1)\n"
	"	incl	%0"
	: "=r" (res)
	: "r" (Addend), "0" (1)
	: "memory", "flags");
  return res;
}

static inline long InterlockedDecrement (long* Addend)
{
  long res;
  asm (	"	lock xadd %0,(%1)\n"
	"	decl	%0"
	: "=r" (res)
	: "r" (Addend), "0" (-1)
	: "memory", "flags");
  return res;
}

static inline long InterlockedExchangeAdd (long* Addend, long Value)
{
  long res;
  asm (	"	lock xadd %0,(%1)"
	: "=r" (res)
	: "r" (Addend), "0" (Value)
	: "memory", "flags");
  return res;
}

static inline long InterlockedCompareExchange (long* Destination, long Exchange, long Comperand)
{
  long res;
  asm (	"	lock cmpxchg %2,(%1)"
	: "=a" (res)
	: "r" (Destination), "r" (Exchange), "0" (Comperand)
	: "memory", "flags");
  return res;
}

static inline void* InterlockedCompareExchange (void** Destination, void* Exchange, void* Comperand)
{
  return (void*)InterlockedCompareExchange ((long*)Destination, (long)Exchange, (long)Comperand);
}

#  if defined (BUBLE_BOOSTER)
static inline unsigned long long InterlockedCompareExchange64 (unsigned long long *Destination,	unsigned long long *Exchange, unsigned long long *Comperand)
{
  unsigned long long retval;
  asm (	"	movl	0(%1), %%eax\n"
	"	movl	4(%1), %%edx\n"
	"	movl	0(%2), %%ebx\n"
	"	movl	4(%2), %%ecx\n"
	"	lock cmpxchg8b (%3)\n"
	: "=A" (retval)
	: "d" (Comperand), "c" (Exchange), "S" (Destination)
	: "ebx", "memory", "flags");
  return retval;
}

static inline unsigned long long InterlockedRotate (unsigned long long *Destination, unsigned long long *Comperand)
{
  unsigned long long retval;
  asm (	"	movl	0(%1), %%eax\n"
	"	movl	4(%1), %%edx\n"
	"	movl	%%edx, %%ebx\n"
	"	movl	%%eax, %%ecx\n"
	"	lock cmpxchg8b (%2)\n"
	: "=A" (retval)
	: "d" (Comperand), "S" (Destination)
	: "ebx", "ecx", "memory", "flags");
  return retval;
}
#  endif

static inline void SpinLock (unsigned long* pSpinLock)
{
  asm (	"1:	lock bts $0, (%0)\n"
	"	jnb	3f\n"
	"2:	testb	$1, (%0)\n"
	"	jz	1b\n"
	"	.byte	0xf3\n"	/* repz */
	"	nop\n"
	"	jmp	2b\n"
	"3:\n"
	:
	: "c" (pSpinLock)
	: "memory", "flags");
}

#  if defined (__KERNEL__)
#    define LOCK_THREAD_SWITCHING()       { asm ("cli"); }
#    define UNLOCK_THREAD_SWITCHING()     { asm ("sti"); }
#    define PUSH_LOCK_THREAD_SWITCHING()  { asm ("pushfl\ncli"); }
#    define POP_UNLOCK_THREAD_SWITCHING() { asm ("popfl"); }
#  else
#    define LOCK_THREAD_SWITCHING()       { ; }
#    define UNLOCK_THREAD_SWITCHING()     { ; }
#    define PUSH_LOCK_THREAD_SWITCHING()  { ; }
#    define POP_UNLOCK_THREAD_SWITCHING() { ; }
#  endif

#else
#  error "Must be port for that compiler!!!"
#endif

#include "stypes.h"

/* Selective includes */
#if defined (VXD)

#  define ONE_SECOND	1250000 /* 1250000 tick per second */
#  define SPINLOCK_SLEEP_TIME (1000 / SPINLOCK_COUNTER)

#  include "9xkrnl_mutex.h"

#elif defined (_NTDDK_)

#  define ONE_SECOND		10000000	/* 10000000 tick per second */
#  define HZ(x)			(ONE_SECOND / (x))
#  define RELATIVE_HZ_TIMEOUT(x)	(-(HZ(x)))
#  define SPINLOCK_SLEEP_TIME (2 * RELATIVE_HZ_TIMEOUT(SPINLOCK_COUNTER))

#  define Time_Slice_Sleep(time)\
{ LARGE_INTEGER TimeOut;	\
  TimeOut.QuadPart = time;	\
  KeDelayExecutionThread (KernelMode, TRUE, &TimeOut); }

#  if defined (USE_SPINLOCK)
#    include "spinlock.h"
#  else
#    include "ntkrnl_mutex.h"
#  endif

#elif defined (WIN32)

#  define ONE_SECOND	1000 /* 1000 tick per second */
#  define SPINLOCK_SLEEP_TIME (1000 / SPINLOCK_COUNTER)
#  define Time_Slice_Sleep(time) { Sleep(time); }

#  if defined (USE_SPINLOCK)
#    include "spinlock.h"
#  else
#    include "win32_mutex.h"
#  endif

#elif defined (__GNUC__)

#  define ONE_SECOND	1000 /* 1000 tick per second */
#  define SPINLOCK_SLEEP_TIME (1000 / SPINLOCK_COUNTER)
#  define Time_Slice_Sleep(time) { msleep(time); }

#  if defined (__DJGPP__)

#    if defined (USE_SPINLOCK)
#      include "spinlock.h"
#    else
#      include "djgpp_mutex.h"
#    endif

#  elif defined (__linux__) && (__KERNEL__)

#    if defined (USE_SPINLOCK)
#      include "spinlock.h"
#    else
#      include "lnkrnl_mutex.h"
#    endif

#  elif defined (__FreeBSD__) && (__KERNEL__)

#    if defined (USE_SPINLOCK)
#      include "spinlock.h"
#    else
#      include "bsdkrnl_mutex.h"
#    endif

#  else

#    if defined (USE_SPINLOCK)
#      include "spinlock.h"
#    else
#      include "pthread_mutex.h"
#    endif

#  endif

#else
#  error "Undefied target system!!!"
#endif

/* Common includes */
#include "hash.h"

#if defined (PART_LOCKED_MAP)
#  include "list_ex.h"
#  include "tsmap.h"
#else
#  include "nbmap.h"
#endif

#include "timercache.h"
#include "pathstor.h"
#include "pipe.h"

/* WMRG.h By Jeffrey Richter - sucks!!! */
#include "tstl_wmrg.h"

#endif
