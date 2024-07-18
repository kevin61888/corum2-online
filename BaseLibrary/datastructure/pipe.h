/*********************************************************************************************************
 *
 *  Module Name:	pipe.h
 *
 *  Abstract:		Thread safe FIFO stack (one direct pipe).
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

#ifndef __PIPE_H__
#define __PIPE_H__

#define DEBUG_PIPE

#if defined DEBUG_PIPE
#define PUT_CHECK_STOP(Val, Limit) \
  if ((Val) > Limit) { brk (); WRL.UnLock (); return false; }
#define GET_CHECK_STOP(Val, Limit) \
  if ((Val) > Limit) { brk (); return false; }
#else
#define PUT_CHECK_STOP(Val, Limit) {/* nothing */;}
#define GET_CHECK_STOP(Val, Limit) {/* nothing */;}
#endif

#define ADD_CHECK(Val, Inc, Limit) Val += Inc; if (Val == Limit) Val = 0;

/* Put		- stores   data (thread safe).
 * Get		- retrives data (thread safe).
 * FreeSize	- returns the size of the free space.
 * UsedSize	- returns the size of the used space.
 */
template <class Tvalue, class Tsize = unsigned long>
class Pipe
{
  volatile Tsize Size;	/* cyclo buffer size */
  volatile Tsize Head;	/* head offset */
  volatile Tsize Tail;	/* tail offset */
  PLocker  WRL;		/* Head protection on multi writers */

  Tvalue* Storage;	/* cyclo buffer */

public:
  Pipe (const Tsize MaxSize, const unsigned char ProcessorsNumber = 1) : WRL (MANY, ProcessorsNumber)
   {
    Size = MaxSize;
    Storage = new Tvalue [Size];

    if (!Storage) { brk(); return; }

    Tail = Head = 0;
    memset (Storage, 0, Size * sizeof (Tvalue));
   }

  ~Pipe ()
   {
    if (!Storage)
      return;
    else
      delete [] Storage, Storage = NULL;
   }

  /* Returns the size of the free space. */
  Tsize FreeSize (void)
   {
    if (!Storage)
      return 0;

    Tsize lTail = Tail, lHead = Head; /* Tail and Head may be changed */

    /* check empty space */
    if (lTail > lHead)
      return lTail - lHead;
    else
      return Size  - lHead + lTail;
   }

  /* Returns the size of the used space. */
  Tsize UsedSize (void)
   {
    if (!Storage)
      return 0;

    Tsize lTail = Tail, lHead = Head; /* Tail and Head may be changed */

    /* get the size of the used space */
    if (lHead > lTail)
      return lHead - lTail;
    else
    if (lHead == lTail)
      return 0;
    else
      return Size - lTail + lHead;
   }

  /* Stores Tvalues.
     Block   - pointer to Tvalues
     Counter - Tvalues counter
     Return true if Tvalues put into pipe.
   */
  bool Put (Tvalue* Block, Tsize Counter)
   {
    if (!Storage || !Block)
      return false;

    if (!Counter)
      return true;

    WRL.Lock ();
    Tsize FreeSize, PartSize, lTail = Tail; /* Tail may be changed */

    /* check if the free space is available */
    if (Head < lTail) /* :DDDD_H........T_DDDD: */
     {
      FreeSize = lTail - 1 - Head;
      PartSize = FreeSize;
     }
    else /* (lTail <= Head) :......T_DDDDD_H......: */
     {
      PartSize = Size - Head;
      FreeSize = PartSize + lTail - 1;
     }

    PUT_CHECK_STOP(FreeSize, Size);
    PUT_CHECK_STOP(PartSize, Size);

    if (FreeSize < Counter)
     {
      brk ();
      WRL.UnLock ();
      return false;
     }

    Tsize pos = 0;

    if (Counter > PartSize) /* do cust */
     {
      PUT_CHECK_STOP(Head + PartSize, Size);
      memcpy (&Storage[Head], &Block[pos], PartSize * sizeof (Tvalue));
      ADD_CHECK(Head, PartSize, Size);
      Counter -= PartSize;
      pos    += PartSize;
     }

    if (Counter)
     {
      PUT_CHECK_STOP(Head + Counter, Size);
      memcpy (&Storage[Head], &Block[pos], Counter   * sizeof (Tvalue));
      ADD_CHECK(Head, Counter, Size);
     }

    WRL.UnLock ();
    return true;
   }

  /* Retrives Tvalues.
     Block   - pointer to Tvalues
     Counter - Tvalues counter
     Return true if Tvalues get from pipe.
   */
  bool Get (Tvalue* Block, Tsize Counter)
   {
    if (!Storage || !Block)
      return false;

    if (!Counter)
      return true;

    Tsize UsedSize, PartSize, lHead = Head; /* Head may be changed */

    /* gets the size of the used space */
    if (lHead < Tail) /* :DDDD_H........T_DDDD: */
     {
      PartSize = Size - Tail;
      UsedSize = PartSize + lHead;
     }
    else /* (Tail <= lHead) :......T_DDDDD_H......: */
     {
      UsedSize = lHead - Tail;
      PartSize = UsedSize;
     }

    GET_CHECK_STOP(UsedSize, Size);
    GET_CHECK_STOP(PartSize, Size);

    if (UsedSize < Counter)
      return false;

    Tsize pos = 0;

    if (Counter > PartSize) /* do cust */
     {
      GET_CHECK_STOP(Tail + PartSize, Size);
      memcpy (&Block[pos], &Storage[Tail], PartSize * sizeof (Tvalue));
      ADD_CHECK(Tail, PartSize, Size);
      Counter -= PartSize;
      pos    += PartSize;
     }

    if (Counter)
     {
      GET_CHECK_STOP(Tail + Counter, Size);
      memcpy (&Block[pos], &Storage[Tail], Counter   * sizeof (Tvalue));
      ADD_CHECK(Tail, Counter, Size);
     }

    return true;
   }
};

template <class Tpipe>
static bool InitPipe (Tpipe*& pP, long Size, byte& ProcessorsNumber)
{
  pP = new Tpipe (Size, ProcessorsNumber);

  if (!pP)
    return false;
  else
    return true;
};

#endif
