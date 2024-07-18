/*********************************************************************************************************
 *
 *  Module Name:	timercache.h
 *
 *  Abstract:		Cache Manager by Timer (CMT). Serve for accelerating performance of data requestors.
 *
 *  Author:		Vyacheslav I. Levtchenko (mail-to: sl@eltrast.ru)
 *
 *  Revision History:	05.08.2003	started
 *
 *  Classes, methods and structures:
 *
 *  External:	HashKey, Time_Slice_Sleep, SetTimeOut, CheckOnTimeOut
 *  Internal:	TimerCache
 *
 *  TODO:
 *
 *********************************************************************************************************/

#ifndef __TIMERCACHE_H__
#define __TIMERCACHE_H__

/* Status object graph */

/* +---------------------LOOPBACK----------------------+ */
/* +-> FREE -> BUSY -> LIVE -> KILL +--------+-> ERAS -+ */
/*                                  +-> DEAD +           */

template <class Tkey, class Tvalue, class Thash = unsigned long>
class TimerCache
{
  typedef struct TimerCacheElem
   {
    /* redanted service information */
    long ref;
    long status; /* "FREE" || "BUSY" || "LIVE" || "KILL" || "DEAD" || "ERAS" */
    ulonglong lastus;

    /* usefull payload */
    Tkey  key;   /* for example key == char* */
    Thash hash;
    long sizeval;
    Tvalue* pval;
   } tce, *ptce;

  ulonglong CacheTimeOut;
  Thash MaxElem, *BubleBooster;
  long UseCounter;

  ptce Storage, TopStorage;
  HashKey<Tkey, Thash> HK;

  /* Private cache array methods */

  /* Lock reference on element */
  static inline long Lock (IN ptce& pElem)
   {
    return InterlockedIncrement (&pElem->ref);
   }

  /* Free reference on element */
  static inline long Release (IN ptce& pElem)
   {
    return InterlockedDecrement (&pElem->ref);
   }

  /* Synoname of Release */
  static inline long UnLock (IN ptce& pElem)
   {
    return Release (pElem);
   }

  /* Lock reference counter removed element */
  static inline long LockRemove (IN ptce& pElem)
   {
    return (InterlockedExchangeAdd (&pElem->ref, MINUS_NULL) + (long)MINUS_NULL);
   }

  /* Free reference counter removed element */
  static inline void ReleaseRemove (IN ptce& pElem)
   {
    InterlockedExchangeAdd (&pElem->ref, MINUS_MEDIAN);
   }

  /* Synoname of ReleaseRemove */
  static inline void UnLockRemove (IN ptce& pElem)
   {
    ReleaseRemove (pElem);
   }

  /* Change status of cache element from XXX -> (to) YYY */
  static inline long ChangeStatus (IN OUT ptce& pElem, IN long NewStatus, IN long PreviosStatus)
   {
    return (long)InterlockedCompareExchange ((PVOID*)&pElem->status, (PVOID)NewStatus, (PVOID)PreviosStatus);
   }

  /* Set cache element timer */
  inline void SetTimeOut (IN ptce& pElem)
   {
    ::SetTimeOut ((DWORD*)&pElem->lastus, (DWORD*)&CacheTimeOut);
   }

  /* Check cache element timer */
  static inline bool CheckOnTimeOut (IN ptce& pElem)
   {
    return ::CheckOnTimeOut ((DWORD*)&pElem->lastus);
   }

  /* Buble up */
  inline bool BubleUp (const Thash buble_pos)
   {
    if (buble_pos)
     {
      ulonglong* prev = (ulonglong*)(&BubleBooster[buble_pos - 1]);
      ulonglong Comperand = *prev;

      if (InterlockedRotate (prev, &Comperand) != Comperand)
       {
	tbrk ();
	return false;
       }

      return true;
     }

    return false;
   }

  /* CleanUp element */
  inline bool RemoveDead (IN OUT ptce pElem)
   {
    if (!pElem->pval) /* BUSY && ERAS may be */
     {
      pElem->ref     = 0;
      pElem->lastus  = 0;
      pElem->sizeval = 0;
      pElem->hash = NULL;
      pElem->key  = NULL;
      pElem->status = FREE_SIGNATURE;
      return false;
     }

    delete [] pElem->pval, pElem->pval = NULL;
    pElem->ref     = 0;
    pElem->lastus  = 0;
    pElem->sizeval = 0;
    pElem->hash = NULL;
    pElem->key  = NULL;
    pElem->status = FREE_SIGNATURE;

    InterlockedDecrement (&UseCounter);
    return true;
   }

  /* ERAS -> FREE. Enable only in ERASE status. If successfull than setup FREE status */
  inline bool Erase (IN OUT ptce& pElem)
   {
    Tvalue *locp = pElem->pval;
    Tvalue *prev = (Tvalue*)InterlockedCompareExchange ((PVOID*)&pElem->pval, (PVOID)NULL, (PVOID)locp);

    if (locp
     && locp == prev)
     {
      pElem->hash = NULL;
      pElem->key  = NULL;

      InterlockedDecrement (&UseCounter);
      if (locp) delete [] locp, locp = NULL;

      /* Set FREE status */
      ChangeStatus (pElem, FREE_SIGNATURE, ERAS_SIGNATURE);
      return true;
     }
    else
     {
      /* Cache corupted, very bad break point !!! */
      brk ();
      /* status don't touch */
      return false;
     }
   }

  /* DEAD -> ERAS -> FREE. Return true if erase dead element */
  bool EraseDead (IN OUT ptce& pElem)
   {
    /* Change status on ERASE */
    long status = ChangeStatus (pElem, ERAS_SIGNATURE, DEAD_SIGNATURE);

    if (DEAD_SIGNATURE == status)
     {
      /* Setup ERASE status successfull */
      if (pElem->ref == MINUS_NULL)
       {
	/* Reference counter locked successfull */
	/* Begin termination dead element of cache */
	return Erase (pElem);
       }
      else
       {
	/* Element doesn't ready for removing */
	/* Try to restore status */
	ChangeStatus (pElem, DEAD_SIGNATURE, ERAS_SIGNATURE);
	return false;
       }
     } /* end ERAS_SIGNATURE */
    else
     {
      /* ERASE status doesn't setuped */
      /* Already doesn't dead %-)) may be already erased %-)) */
      return false;
     }
   }

  /* KILL -> ERAS -> FREE. Return true if erase element */
  bool EraseKilled (IN OUT ptce& pElem)
   {
    /* Change status on ERASE */
    long status = ChangeStatus (pElem, ERAS_SIGNATURE, KILL_SIGNATURE);

    if (KILL_SIGNATURE == status)
     {
      /* Setup ERASE status successfull */
      return Erase (pElem);
     }
    else
     {
      /* ERASE status doesn't setuped */
      /* Already doesn't in killing mode %-)) may be already erased %-)) */
      brk (); /* lock counter succefully locked, but nobody do it... :-(( */
      return false;
     }
   }

  /* LIVE -> KILL (-> ERAS -> FREE) || LIVE -> KILL -> DEAD */
  /* Set status FREE or DEAD, begin with LIVE status going over KILL and ERAS */
  /* Return false if pElem biger of Top Storage */
  bool Remove (IN ptce pElem)
   {
    if (pElem >= TopStorage) { brk (); return false; }

    /* If LIVE status passed, than entry to killing status */
    long status = ChangeStatus (pElem, KILL_SIGNATURE, LIVE_SIGNATURE);

    /* Release element */
    Release (pElem); // == 0

    /* Cache element status control */
    if (LIVE_SIGNATURE != status)
     {
      if (KILL_SIGNATURE == status)
       {
	/* Already in killing mode */
        tbrk ();
       }
      else
      if (DEAD_SIGNATURE == status)
       {
	/* Element marked as dead */
	brk ();

	if (EraseDead (pElem))
	 {
	  /* Unlock reference counter from unchanged state */
	  ReleaseRemove (pElem);
	  return true;
	 }
       } /* end DEAD_SIGNATURE */
      else
      if (ERAS_SIGNATURE == status)
       {
	/* Already in erasing mode */
	brk ();
       }
      else
      if (BUSY_SIGNATURE == status)
       {
	/* strange mode try to remove... :-(( */
	brk ();
       }
      else
      if (FREE_SIGNATURE == status)
       {
	/* Final cleanup %-)) */
	tbrk ();
       }
      else
       {
	/* Element status corrupted, very bad break point !!! */
	brk ()
       }

      return false;
     } /* end LIVE_SIGNATURE != status */

    /* set KILL status successfull */

    /* Try to lock reference counter for using */
    if ((unsigned long)LockRemove (pElem) == MINUS_NULL) /* Reference counter locked successfull */
     {
      /* Begin termination element of cache */
      if (EraseKilled (pElem))
       {
	/* Unlock reference counter from unchanged state */
	ReleaseRemove (pElem);
	return true;
       }
     }

    /* Cache element's in using, try to wait releasing */
    for (unsigned long retry = SPINLOCK_COUNTER; retry > 0; retry--)
     {
      if ((unsigned long)pElem->ref == MINUS_NULL)
        break;

      Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
     }

    if (!retry)
     {
      brk ();

      /* Mark cache element for removing */
      status = ChangeStatus (pElem, DEAD_SIGNATURE, KILL_SIGNATURE);
      return false;
     }

    /* Begin termination element of cache */
    if (EraseKilled (pElem))
     {
      /* Unlock reference counter from unchanged state */
      ReleaseRemove (pElem);
      return true;
     }

    brk ();
    return false;
   }

  /* Search array element by key & lock it */
  bool SearchByKey (IN OUT Thash& pos, OUT ptce& pElem, IN Tkey& key)
   {
    if (!Storage || !MaxElem) { brk (); return false; }

    Thash i;
    /* Move to ahead of cache array */
    for (i = 0, pos = BubleBooster[i], pElem = &Storage [pos]; i < MaxElem; pos = BubleBooster[++i], pElem = &Storage [pos])
     {
      if (LIVE_SIGNATURE != pElem->status)
       {
	if (EraseDead (pElem))
	 {
	  /* Unlock reference counter from unchanged state */
	  ReleaseRemove (pElem);
	  continue;
	 }
       }

      /* Lock element */
      if (Lock (pElem) <= 0)
       {
	/* Locked counter detected */
	UnLock (pElem);
	continue;
       }

      if (pElem->key == key)
       {
	BubleUp (i);
	SetTimeOut (pElem);
	return true;
       }
      else
      if (CheckOnTimeOut (pElem)
       && ChangeStatus (pElem, DEAD_SIGNATURE, LIVE_SIGNATURE) == LIVE_SIGNATURE)
       {
	/* Lock reference counter to unchanged state */
	LockRemove (pElem);

	UnLock (pElem);

	if (EraseDead (pElem))
	 {
	  /* Unlock reference counter from unchanged state */
	  ReleaseRemove (pElem);
	 }

	continue;
       } /* end else key */

      UnLock (pElem);
     }

    return false;
   }

  /* Search array element by hash & lock it */
  bool SearchByHash (IN OUT Thash& pos, OUT ptce& pElem, IN Thash& hash)
   {
    if (!Storage || !MaxElem) { brk (); return false; }

    Thash i;
    /* Move to ahead of cache array */
    for (i = 0, pos = BubleBooster[i], pElem = &Storage [pos]; i < MaxElem; pos = BubleBooster[++i], pElem = &Storage [pos])
     {
      if (LIVE_SIGNATURE != pElem->status)
       {
	if (EraseDead (pElem))
	 {
	  /* Unlock reference counter from unchanged state */
	  ReleaseRemove (pElem);
	  continue;
	 }
       }

      /* Lock element */
      if (Lock (pElem) <= 0)
       {
	/* Locked counter detected */
	UnLock (pElem);
	continue;
       }

      if (pElem->hash == hash)
       {
	BubleUp (i);
	SetTimeOut (pElem);
	return true;
       }
      else
      if (CheckOnTimeOut (pElem)
       && ChangeStatus (pElem, DEAD_SIGNATURE, LIVE_SIGNATURE) == LIVE_SIGNATURE)
       {
	/* Lock reference counter to unchanged state */
	LockRemove (pElem);

	UnLock (pElem);

	if (EraseDead (pElem))
	 {
	  /* Unlock reference counter from unchanged state */
	  ReleaseRemove (pElem);
	 }

	continue;
       } /* end else hash */ 

      UnLock (pElem);
     } /* end for pos */

    return false;
   }

public:
  TimerCache (const ulonglong TimeOut, const Thash NumElem = 32) : CacheTimeOut(0), Storage(NULL), TopStorage(NULL), UseCounter(0), MaxElem(0)
   {
    Storage = new tce [NumElem];
    if (!Storage)
     {
      brk ();
      return;
     }

    BubleBooster = new Thash [NumElem];
    if (!BubleBooster)
     {
      brk ();
      delete Storage, Storage = NULL;
      return;
     }

    memset (Storage,      0, sizeof (tce)   * NumElem);
    memset (BubleBooster, 0, sizeof (Thash) * NumElem);

    ptce p = Storage;

    for (Thash i = 0; i < NumElem; i++, p++)
     {
      p->status = FREE_SIGNATURE;
      BubleBooster [i] = i;
     }

    TopStorage = Storage + NumElem;
    CacheTimeOut = TimeOut;
    MaxElem  = NumElem;
   }

  /* Doesn't thread safe method */
  inline bool IsEmpty (void)
   {
    if (!UseCounter)
      return true;
    else
      return false;
   }

  /* Get statistic about cache using */
  inline long GetStat (void)
   {
    return UseCounter;
   }

  /* Get hash by key */
  inline Thash Hash (IN const Tkey& key)
   {
    return HK.Hash (key);
   }

  /* Insert element in cache & if successfull than lock element */
  bool SetAt (IN OUT Thash& pos, IN Tkey& key, IN Thash& hash, IN Tvalue*& pval, long sizeval)
   {
    if (!Storage || !MaxElem) { brk (); return false; }

    ptce pElem;

    if (SearchByHash (pos = 0, pElem, hash)
     && LIVE_SIGNATURE == pElem->status)
      return true;

    long status = BUSY_SIGNATURE;

    for (long retry = 3; retry > 0; retry--)
     {
      pElem = Storage;

      /* Move to ahead of cache array */
      for (pos = 0; pos < MaxElem; pos++, pElem++)
       {
	if (LIVE_SIGNATURE == pElem->status
	 && CheckOnTimeOut (pElem)
	 && ChangeStatus (pElem, DEAD_SIGNATURE, LIVE_SIGNATURE) == LIVE_SIGNATURE)
	 {
	  /* Lock reference counter to unchanged state */
	  LockRemove (pElem);
	 }

	if (FREE_SIGNATURE != pElem->status)
	 {
	  if (EraseDead (pElem))
	   {
	    /* Try to lock free cache element */
	    status = ChangeStatus (pElem, BUSY_SIGNATURE, FREE_SIGNATURE);

	    if (FREE_SIGNATURE != status)
	     {
	      brk (); /* Bingo!!! - concurent SetAt in progress!!! */
	      ReleaseRemove (pElem);

	      Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
	      continue;
	     }

	    /* Lock element */
	    Lock (pElem);

	    /* Unlock reference counter from unchanged state */
	    ReleaseRemove (pElem);

	    break;
	   }
	 }

	/* Lock element */
	if (Lock (pElem) <= 0)
	 {
	  /* Locked counter detected */
	  UnLock (pElem);
	  continue;
	 }

	/* Try to lock free cache element */
	status = ChangeStatus (pElem, BUSY_SIGNATURE, FREE_SIGNATURE);

	if (FREE_SIGNATURE == status)
	  break;

	UnLock (pElem);
       } /* end for pos */

      /* Initial FREE status */
      if (FREE_SIGNATURE == status)
	break;
     } /* end for retry */

    if (!retry)
     {
      brk ();

      /* Try to remove bottom element */
      pos = BubleBooster[MaxElem - 1];
      pElem = &Storage [pos];

      /* Lock element */
      if (Lock (pElem) <= 0)
       {
	/* Locked counter detected */
	UnLock (pElem);
	return false;
       }

      /* Remove last element */
      if (!Remove (pElem))
	return false;

      /* Lock element */
      if (Lock (pElem) <= 0)
       {
	/* Locked counter detected */
	UnLock (pElem);
	return false;
       }

      /* Try to lock free cache element */
      if (FREE_SIGNATURE != ChangeStatus (pElem, BUSY_SIGNATURE, FREE_SIGNATURE))
       {
	UnLock (pElem);
	return false;
       }

      BubleUp (MaxElem - 1);
     }

    /* Allocate new cache element */
    if (!(pElem->pval = new Tvalue [sizeval]))
     {
      brk ();

      ChangeStatus (pElem, FREE_SIGNATURE, BUSY_SIGNATURE);
      UnLock (pElem);

      return false;
     }

    pElem->key  = key;
    pElem->hash = hash;
    pElem->sizeval = sizeval;

    memcpy (pElem->pval, pval, sizeof (Tvalue) * sizeval);
    SetTimeOut (pElem);

    InterlockedIncrement (&UseCounter);

    /* Activate record */
    ChangeStatus (pElem, LIVE_SIGNATURE, BUSY_SIGNATURE);
    return true;
   }

  /* Insert element in cache & if successfull than lock element */
  inline bool SetAt (IN OUT Thash& pos, IN Tkey& key, IN Tvalue*& pValue, long sizeval)
   {
    Thash hash;
    return SetAt (pos, key, hash = HK.Hash (key), pValue, sizeval);
   }

  /* Look for element in cache by key & if successfull search than lock element */
  inline bool LookupByKey (IN OUT Thash& pos, IN Tkey& key, OUT Tvalue*& pValue, long& sizeval)
   {
    ptce pElem;

    if (!SearchByKey (pos = 0, pElem, key))
      return false;

    pValue  = pElem->pval;
    sizeval = pElem->sizeval;
    return true;
   }

  /* Look for element in cache by keys hash & if successfull search than lock element */
  inline bool LookupByKeyHash (IN OUT Thash& pos, IN Tkey& key, OUT Tvalue*& pValue, long& sizeval)
   {
    ptce pElem;
    Thash hash = Hash (key);

    if (!SearchByHash (pos = 0, pElem, hash))
      return false;

    pValue  = pElem->pval;
    sizeval = pElem->sizeval;
    return true;
   }

  /* Look for element in cache by hash & if successfull search than lock element */
  inline bool LookupByHash (IN OUT Thash& pos, IN Thash& hash, OUT Tvalue*& pValue, long& sizeval)
   {
    ptce pElem;

    if (!SearchByHash (pos = 0, pElem, hash))
      return false;

    pValue  = pElem->pval;
    sizeval = pElem->sizeval;
    return true;
   }

  /* Look for element in cache by pointer on pval */
  inline Tvalue* Lookup (IN Thash& pos)
   {
    if (!Storage || pos >= MaxElem) { brk (); return NULL; }

    return Storage [pos].pval;
   }

  /* Free reference on element */
  inline long Release (IN Thash& pos)
   {
    if (!Storage || pos >= MaxElem) { brk (); return false; }

    ptce pElem = &Storage [pos];
    return Release (pElem);
   }

  /* Synoname of Release */
  inline long UnLock (IN Thash& pos)
   {
    return Release (pos);
   }

  /* Remove element from cache & unlock element if successfull */
  inline bool Remove (IN Thash& pos)
   {
    if (!Storage || pos >= MaxElem) { brk (); return false; }

    return Remove (&Storage [pos]);
   }

  /* Remove element from cache on cleanup */
  inline bool RemoveDead (IN Thash& pos)
   {
    if (!Storage || pos >= MaxElem) { brk (); return false; }

    return RemoveDead (&Storage [pos]);
   }

  /* Remove element from cache by key */
  inline bool RemoveByKey (IN Tkey& key)
   {
    long sizeval;
    Thash pos = 0;
    Tvalue* pValue;

    if (!LookupByKey (pos, key, pValue, sizeval))
      return false;

    return Remove (pos);
   }

  /* Remove element from cache by keys hash */
  inline bool RemoveByKeyHash (IN Tkey& key)
   {
    long sizeval;
    Tvalue* pValue;
    Thash pos = 0;

    if (!LookupByKeyHash (pos, key, pValue, sizeval))
      return false;

    return Remove (pos);
   }

  /* Remove element from cache by hash */
  inline bool RemoveByHash (IN Thash& hash)
   {
    long sizeval;
    Thash pos = 0;
    Tvalue* pValue;

    if (!LookupByHash (pos, hash, pValue, sizeval))
      return false;

    return Remove (pos);
   }

  /* Next caches enumerating & if failure than unlock last element */
  inline bool Start (IN OUT Thash& pos, OUT Tkey& key, OUT Thash& hash, OUT Tvalue*& pValue)
   {
    ptce pElem = &Storage [pos];

    /* Move to ahead of cache array */
    for (; pos < MaxElem; pos++, pElem++)
     {
      if (LIVE_SIGNATURE != pElem->status)
       {
	if (EraseDead (pElem))
	 {
	  /* Unlock reference counter from unchanged state */
	  ReleaseRemove (pElem);
	  continue;
	 }
       }

      /* Lock element */
      if (Lock (pElem) <= 0)
       {
	/* Locked counter detected */
	UnLock (pElem);
	continue;
       }

      key    = pElem->key;
      hash   = pElem->hash;
      pValue = pElem->pval;

      return true;
     }

    return false;
   }

  /* Begin caches enumerating & if successfull than lock element */
  inline bool Next (IN OUT Thash& pos, OUT Tkey& key, OUT Thash& hash, OUT Tvalue*& pValue)
   {
    ptce pElem = &Storage [pos++];
    UnLock (pElem);

    return Start (pos, key, hash, pValue);
   }

  /* Doesn't thread safe method, it called from destructor */
  inline void RemoveAll (void)
   {
    /* Move to ahead of cache array */
    for (ptce pElem = Storage; pElem < TopStorage; pElem++)
     {
      if (FREE_SIGNATURE == pElem->status)
	continue;

      if (!pElem->pval) /* BUSY && ERAS may be */
	continue;

      delete [] pElem->pval, pElem->pval = NULL;
      InterlockedDecrement (&UseCounter);
     }
   }

  ~TimerCache (void)
   {
    RemoveAll ();

    if (Storage) delete [] Storage, Storage = TopStorage = NULL;
    if (BubleBooster) delete BubleBooster,  BubleBooster = NULL;
   }
};

template <class Tcache>
static bool InitTC (Tcache*& pC, int Elem, ulonglong CacheTimeOut)
{
  pC = new Tcache (CacheTimeOut, Elem);

  if (!pC)
    return false;
  else
    return true;
};

#endif
