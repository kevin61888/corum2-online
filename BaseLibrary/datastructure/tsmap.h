/*********************************************************************************************************
 *
 *  Module Name:	tsmap.h
 *
 *  Abstract:		Partialy Locked Thread Safe Map Storage (PLTSMS). It works without blocking linked
 *			lists in map array on uniprocessor platform. It works with blocking linked lists 
 *			in map array (in classic style) on multiprocessors platform. Non block linked 
 *			lists operate mode gets overall perfomance on workstation platform. Multimap 
 *			methods implemented also (Start() and Next() methods by hash).
 *
 *  Author:		Vyacheslav I. Levtchenko (mail-to: sl@eltrast.ru)
 *
 *  Revision History:	05.08.2003	started
 *
 *  Classes, methods and structures:
 *
 *  External:	HashKey, Time_Slice_Sleep, MLocker
 *  Internal:	ThreadSafeMap, MapPos
 *
 *  TODO:
 *
 *********************************************************************************************************/

#ifndef __TSMAP_H__
#define __TSMAP_H__

typedef struct MapPos
{
  plh pListEntry;
  plh pListHead;
  unsigned long MapElem;
  MapPos () : pListEntry(NULL), pListHead(NULL), MapElem(0) {}
} mp, *pmp;

template <class Tkey, class Tvalue, class Thash = unsigned long>
class ThreadSafeMap
{
  typedef struct MapElem
   {
    list_head ListHead;
    long UseCounter;
    MLocker LK;

    void Init (byte ProcNum)
     {
      INIT_LIST_HEAD (&ListHead);
      LK.Init (ProcNum);
      UseCounter = 0;
     }

    ~MapElem() {}
   } me, *pme;

  typedef struct ListElem
   {
    /* redanted service information */
    list_head ListEntry;
    long  ref;
    long  status; /* "LIVE" || "KILL" || "DEAD" */

    /* usefull payload */
    Tkey  key;
    Thash hash;
   } le, *ple;

  Thash MaxElem;
  long  UseCounter;

  pme Storage;
  HashKey<Tkey, Thash> HK;

  unsigned char ProcNum;

  /* Private map methods */

  /* Lock reference on element */
  static inline void Lock (IN plh& pListEntry)
   {
    InterlockedIncrement(&((ple)pListEntry)->ref);
   }

  /* Free reference on element */
  static inline void Release (IN plh& pListEntry)
   {
    InterlockedDecrement(&((ple)pListEntry)->ref);
   }

  /* Synoname of Release */
  static inline void UnLock (IN plh& pListEntry)
   {
    Release (pListEntry);
   }

  inline bool Remove (IN plh& pListEntry, IN plh pListHead)
   {
    /* List head removing protection */
    if (pListEntry == pListHead)
     {
      brk ();
      return false;
     }

    list_del (pListEntry);

#if defined (DEBUG)
    pListEntry->next = pListEntry->prev = NULL;
#endif

    delete pListEntry;

    InterlockedDecrement (&UseCounter);
    return true;
   }

  /* Search list entry by key & lock it */
  bool SearchByKey (OUT plh& pListEntry, IN plh pListHead, IN Tkey& key)
   {
    LOCK_THREAD_SWITCHING ();

    if (list_empty (pListHead))
     {
      UNLOCK_THREAD_SWITCHING ();
      return false;
     }

    plh pListEntryNext;
    list_for_each_safe (pListEntry, pListEntryNext, pListHead)
     {
      if (!pListEntry)
       {
	/* Element removed, very bad break point !!! */
	brk ();
	pListEntry = pListHead;

	UNLOCK_THREAD_SWITCHING ();
	return false;
       }

      if (LIVE_SIGNATURE != ((ple)pListEntry)->status)
       {
	/* Element dead */
	brk ();

	if (DEAD_SIGNATURE == ((ple)pListEntry)->status
	 && ((ple)pListEntry)->ref < 0)
	 {
	  /* Element marked as dead and element readies for removing */
	  brk ();
	  Remove (pListEntry, pListHead);
	 }

	continue;
       }

      if (((ple)pListEntry)->key == key)
       {
	Lock (pListEntry);

	UNLOCK_THREAD_SWITCHING ();
	return true;
       }
     }

    UNLOCK_THREAD_SWITCHING ();
    return false;
   }

  /* Search list entry by hash & lock it */
  bool SearchByHash (OUT plh& pListEntry, IN plh pListHead, IN Thash& hash)
   {
    LOCK_THREAD_SWITCHING ();

    if (list_empty (pListHead))
     {
      UNLOCK_THREAD_SWITCHING ();
      return false;
     }

    plh pListEntryNext;
    list_for_each_safe (pListEntry, pListEntryNext, pListHead)
     {
      if (!pListEntry)
       {
	/* Element removed, very bad break point !!! */
	brk ();
	pListEntry = pListHead;

	UNLOCK_THREAD_SWITCHING ();
	return false;
       }

      if (LIVE_SIGNATURE != ((ple)pListEntry)->status)
       {
	/* Element dead */
	brk ();

	if (DEAD_SIGNATURE == ((ple)pListEntry)->status
	 && ((ple)pListEntry)->ref < 0)
	 {
	  /* Element marked as dead and element readies for removing */
	  brk ();
	  Remove (pListEntry, pListHead);
	 }

	continue;
       }

      if (((ple)pListEntry)->hash == hash)
       {
	Lock (pListEntry);

	UNLOCK_THREAD_SWITCHING ();
	return true;
       }
     }

    UNLOCK_THREAD_SWITCHING ();
    return false;
   }

public:
  ThreadSafeMap (const Thash NumElem = 32, const byte ProcessorsNumber = 1)
   {
    MaxElem = NumElem;
    Storage = new me [MaxElem];

    if (!Storage) { brk(); return; }

    ProcNum = ProcessorsNumber;

    pme p = Storage;

    for (Thash i = 0; i < MaxElem; i++, p++) p->Init (ProcNum);

    UseCounter = 0;
   }

  /* Doesn't thread safe method */
  bool IsEmpty (void)
   {
    if (!UseCounter)
      return true;
    else
      return false;
   }

  /* Get statistic about map using */
  inline long GetStat (void)
   {
    return UseCounter;
   }

  /* Get statistic about using map element */
  inline long GetStat (Thash MapElem)
   {
    return Storage[MapElem % MaxElem].UseCounter;
   }

  /* Look for element in map by ListEntry */
  static inline Tvalue* Lookup (IN mp& pos)
   {
    return (Tvalue*)(((ple)pos.pListEntry) + 1);
   }

  /* Get hash by key */
  inline Thash Hash (IN const Tkey& key)
   {
    return HK.Hash (key);
   }

  /* Insert element in map & if successfull than lock element */
  bool SetAt (IN OUT mp& pos, IN Tkey& key, IN Thash& hash, IN Tvalue& val)
   {
    pos.pListEntry = (plh) new byte [sizeof (le) + sizeof (Tvalue)];

    if (!pos.pListEntry) { brk (); return false; }

    ((ple)pos.pListEntry)->key  = key;
    ((ple)pos.pListEntry)->hash = hash;
    memcpy (((ple)pos.pListEntry) + 1, &val, sizeof (Tvalue));

    ((ple)pos.pListEntry)->ref  = 1; /* Lock element */
    ((ple)pos.pListEntry)->status = LIVE_SIGNATURE;

    pos.MapElem = hash % MaxElem;
    pos.pListHead = &Storage[pos.MapElem].ListHead;

    Storage[pos.MapElem].LK.Lock (); /* Lock linked list */

    InterlockedIncrement (&UseCounter);
    InterlockedIncrement (&Storage[pos.MapElem].UseCounter);

    PUSH_LOCK_THREAD_SWITCHING ();

    list_add (pos.pListEntry, pos.pListHead);

    POP_UNLOCK_THREAD_SWITCHING ();

    return true;
   }

  /* Insert element in map & if successfull than lock element */
  inline bool SetAt (IN OUT mp& pos, IN Tkey& key, IN Tvalue& val)
   {
    Thash hash;
    return SetAt (pos, key, hash = HK.Hash (key), val);
   }

  /* Look for element in map by key & if successfull search than lock element */
  bool LookupByKey (IN OUT mp& pos, IN Tkey& key, OUT Tvalue*& pValue)
   {
    pos.MapElem   = HK.Hash (key) % MaxElem;
    pos.pListHead = &Storage[pos.MapElem].ListHead;

    Storage[pos.MapElem].LK.Lock (); /* Lock linked list */

    if (!SearchByKey (pos.pListEntry, pos.pListHead, key))
     {
      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return false;
     }

    pValue = Lookup (pos);
    return true;
   }

  /* Look for element in map by keys hash & if successfull search than lock element */
  bool LookupByKeyHash (IN OUT mp& pos, IN Tkey& key, OUT Tvalue*& pValue)
   {
    Thash hash    = HK.Hash (key);
    pos.MapElem   = hash % MaxElem;
    pos.pListHead = &Storage[pos.MapElem].ListHead;

    Storage[pos.MapElem].LK.Lock (); /* Lock linked list */

    if (!SearchByHash (pos.pListEntry, pos.pListHead, hash))
     {
      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return false;
     }

    pValue = Lookup (pos);
    return true;
   }

  /* Look for element in map by hash & if successfull search than lock element */
  bool LookupByHash (IN OUT mp& pos, IN Thash& hash, OUT Tvalue*& pValue)
   {
    pos.MapElem   = hash % MaxElem;
    pos.pListHead = &Storage[pos.MapElem].ListHead;

    Storage[pos.MapElem].LK.Lock (); /* Lock linked list */

    if (!SearchByHash (pos.pListEntry, pos.pListHead, hash))
     {
      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return false;
     }

    pValue = Lookup (pos);
    return true;
   }

  /* Unlock position in map */
  inline void Release (IN mp& pos)
   {
    Release (pos.pListEntry);

    Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
   }

  /* Synoname of Release */
  inline void UnLock (IN mp& pos)
   {
    Release (pos);
   }

  /* Remove element from map by ListEntry and always unlock element */
  /* return false if list entry destroyed */
  bool Remove (IN mp& pos)
   {
    LOCK_THREAD_SWITCHING ();

    if (!pos.pListEntry)
     {
      /* Element removed, very bad break point !!! */
      brk ();
      UNLOCK_THREAD_SWITCHING ();

      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return false;
     }

    if (pos.pListEntry == pos.pListHead)
     {
      /* Try to remove head of list, very bad break point !!! */
      brk ();
      UNLOCK_THREAD_SWITCHING ();

      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return false;
     }

    /* If LIVE status passed, than entry to killing status */
    long status = (long)InterlockedCompareExchange ((PVOID*)&((ple)pos.pListEntry)->status, (PVOID)KILL_SIGNATURE, (PVOID)LIVE_SIGNATURE);

    /* Release element */
    Release (pos.pListEntry);

    /* List element status control */
    if (LIVE_SIGNATURE != status)
     {
      if (KILL_SIGNATURE == status)
       {
	/* Already in killing mode - release and go out */
	tbrk ();
	InterlockedDecrement (&((ple)pos.pListEntry)->ref);
       }
      else
      if (DEAD_SIGNATURE == status)
       {
	/* Element marked as dead */
	if (((ple)pos.pListEntry)->ref < 0)
	 {
	  /* Element ready for removing */
	  brk ();
	  Remove (pos.pListEntry, pos.pListHead);
	  InterlockedDecrement (&Storage[pos.MapElem].UseCounter);

	  UNLOCK_THREAD_SWITCHING ();
	  Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
	  return true;
	 }
	else
	 {
	  /* Element doesn't ready for removing */
	  brk ();
	 }
       }
      else
       {
	/* List status corrupted, very bad break point !!! */
	brk ()
       }

      UNLOCK_THREAD_SWITCHING ();

      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return false;
     }
    
    /* Try to lock reference counter for using */
    long ref = InterlockedDecrement (&((ple)pos.pListEntry)->ref);

    if (ref < 0)
     {
      /* Reference counter locked successfull */
      Remove (pos.pListEntry, pos.pListHead);
      InterlockedDecrement (&Storage[pos.MapElem].UseCounter);

      UNLOCK_THREAD_SWITCHING ();

      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return true;
     }

    /* List element's in using, try to wait releasing */
    for (unsigned long retry = SPINLOCK_COUNTER; retry > 0; retry--)
     {
      if (((ple)pos.pListEntry)->ref < 0)
        break;

      UNLOCK_THREAD_SWITCHING ();

      Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);

      LOCK_THREAD_SWITCHING ();
     }

    if (!retry)
     {
      brk ();

      /* Mark list element for removing */
      status = (long)InterlockedCompareExchange ((PVOID*)&((ple)pos.pListEntry)->status, (PVOID)DEAD_SIGNATURE, (PVOID)KILL_SIGNATURE);

      InterlockedDecrement (&Storage[pos.MapElem].UseCounter);

      UNLOCK_THREAD_SWITCHING ();

      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return true;
     }

    brk ();

    if (!pos.pListEntry->next || !pos.pListEntry->prev)
     {
      /* List element removed, vary bad break point !!! */
      brk ();
      UNLOCK_THREAD_SWITCHING ();

      Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
      return true;
     }

    Remove (pos.pListEntry, pos.pListHead);
    InterlockedDecrement (&Storage[pos.MapElem].UseCounter);

    UNLOCK_THREAD_SWITCHING ();

    Storage[pos.MapElem].LK.UnLock (); /* Unlock linked list */
    return true;
   }

  inline bool RemoveDead (IN mp& pos)
   {
    return Remove (pos);
   }

  /* Remove element from map by key */
  inline bool RemoveByKey (IN Tkey& key)
   {
    mp pos;
    Tvalue* pValue;

    if (!LookupByKey (pos, key, pValue))
      return false;

    return Remove (pos);
   }

  /* Remove element from map by keys hash */
  inline bool RemoveByKeyHash (IN Tkey& key)
   {
    mp pos;
    Tvalue* pValue;

    if (!LookupByKeyHash (pos, key, pValue))
      return false;

    return Remove (pos);
   }

  /* Remove element from map by hash */
  inline bool RemoveByHash (IN Thash& hash)
   {
    mp pos;
    Tvalue* pValue;

    if (!LookupByHash (pos, hash, pValue))
      return false;

    return Remove (pos);
   }

  /* Begin maps enumerating & if successfull than lock element */
  bool Start (IN OUT mp& pos, OUT Tkey& key, OUT Thash& hash, OUT Tvalue*& pValue)
   {
    /* Move to ahead of maps array */
    for (pos.MapElem = 0; pos.MapElem < MaxElem; Storage[pos.MapElem].LK.UnLock (), pos.MapElem++)
     {
      Storage[pos.MapElem].LK.Lock (); /* Lock linked list */

      LOCK_THREAD_SWITCHING ();
      pos.pListHead = &Storage[pos.MapElem].ListHead;

      if (list_empty (pos.pListHead))
       {
	UNLOCK_THREAD_SWITCHING ();
	continue;
       }
      
      pos.pListEntry = pos.pListHead->next;

      if (!pos.pListEntry)
       {
	/* List element removed, very bad break point !!! */
	brk ();
	UNLOCK_THREAD_SWITCHING ();
	continue;
       }

      /* List element status control */
      while (pos.pListEntry != pos.pListHead
	  && LIVE_SIGNATURE != ((ple)pos.pListEntry)->status)
       {
	/* Element dead */
	brk ();

	if (DEAD_SIGNATURE == ((ple)pos.pListEntry)->status
	 && ((ple)pos.pListEntry)->ref < 0)
	 {
	  /* Element marked as dead and element readies for removing */
	  brk ();

	  /* Save pointer on next entry */
	  plh pListEntryNext = pos.pListEntry->next;
	  Remove (pos.pListEntry, pos.pListHead);

	  /* Set pointer on next entry */
	  pos.pListEntry = pListEntryNext;
	 }
	else
	 {
	  /* Set pointer on next entry */
	  pos.pListEntry = pos.pListEntry->next;
	 }
       } /* End while dead elements */

      if (pos.pListEntry == pos.pListHead)
       {
	/* End of list reached */
	UNLOCK_THREAD_SWITCHING ();
	/* Go to next list */
	continue; 
       }

      Lock (pos.pListEntry);
      UNLOCK_THREAD_SWITCHING ();

      key  = ((ple)pos.pListEntry)->key;
      hash = ((ple)pos.pListEntry)->hash;
      pValue = Lookup (pos);
      return true;
     } /* End for MapElem */

    return false;
   }

  /* Next maps enumerating & if failure than unlock last element */
  bool Next (IN OUT mp& pos, OUT Tkey& key, OUT Thash& hash, OUT Tvalue*& pValue)
   {
    do
     {
      LOCK_THREAD_SWITCHING ();

      if (pos.pListEntry == pos.pListEntry->next)
       {
	/* List looped, very bad break point !!! */
	brk ();
	pos.pListEntry->next = pos.pListHead;
       }

      /* Move to ahead of linked list */
      plh pListEntryPrev = pos.pListEntry;
      pos.pListEntry = pos.pListEntry->next;

      if (pListEntryPrev != pos.pListHead)
	Release (pListEntryPrev);

      if (!pos.pListEntry)
       {
	/* List element removed, very bad break point !!! */
	brk ();
	pos.pListEntry = pos.pListHead;
       }

      /* List element status control */
      while (pos.pListEntry != pos.pListHead
	  && LIVE_SIGNATURE != ((ple)pos.pListEntry)->status)
       {
	/* Element dead */
	brk ();

	if (DEAD_SIGNATURE == ((ple)pos.pListEntry)->status
	 && ((ple)pos.pListEntry)->ref < 0)
	 {
	  /* Element marked as dead and element readies for removing */
	  brk ();

	  /* Save pointer on next entry */
	  plh pListEntryNext = pos.pListEntry->next;
	  Remove (pos.pListEntry, pos.pListHead);

	  /* Set pointer on next entry */
	  pos.pListEntry = pListEntryNext;
	 }
	else
	 {
	  /* Set pointer on next entry */
	  pos.pListEntry = pos.pListEntry->next;
	 }
       } /* End while dead elements */

      if (pos.pListEntry != pos.pListHead)
       {	
	Lock (pos.pListEntry);
	UNLOCK_THREAD_SWITCHING ();

	key  = ((ple)pos.pListEntry)->key;
	hash = ((ple)pos.pListEntry)->hash;
	pValue = Lookup (pos);
	return true;
       }

      UNLOCK_THREAD_SWITCHING ();

      /* Go to next list */

      /* Move to ahead of maps array */
      for (Storage[pos.MapElem].LK.UnLock (); ++pos.MapElem < MaxElem; Storage[pos.MapElem].LK.UnLock ())
       {
	Storage[pos.MapElem].LK.Lock (); /* Lock linked list */

	pos.pListHead = &Storage[pos.MapElem].ListHead;

	LOCK_THREAD_SWITCHING ();

	if (!list_empty (pos.pListHead))
 	 {
	  pos.pListEntry = pos.pListHead;
	  break;
	 }

	UNLOCK_THREAD_SWITCHING ();
       }
     }
    while (pos.MapElem < MaxElem);

    UNLOCK_THREAD_SWITCHING ();
    return false;
   }

  /* Begin maps enumerating by hash (AKA multimap) & if successfull than lock element */
  inline bool Start (IN OUT mp& pos, IN Thash& hash, OUT Tvalue*& pValue)
   {
    /* First time use standart method */
    return LookupByHash (pos, hash, pValue);
   }

  /* Next maps enumerating by hash (AKA multimap) & if failure than unlock last element */
  bool Next (IN OUT mp& pos, OUT Thash& hash, OUT Tvalue*& pValue)
   {
    LOCK_THREAD_SWITCHING ();

    if (pos.pListEntry == pos.pListEntry->next)
     {
      /* List looped, very bad break point !!! */
      brk ();
      pos.pListEntry->next = pos.pListHead;
     }

    /* Move to ahead of linked list */
    plh pListEntryPrev = pos.pListEntry;
    pos.pListEntry = pos.pListEntry->next;

    if (pListEntryPrev != pos.pListHead)
      Release (pListEntryPrev);

    if (!pos.pListEntry)
     {
      /* List element removed, very bad break point !!! */
      brk ();
      pos.pListEntry = pos.pListHead;
     }

    /* List element status control */
    while (pos.pListEntry != pos.pListHead
        && (LIVE_SIGNATURE != ((ple)pos.pListEntry)->status
	 || hash != ((ple)pos.pListEntry)->hash))
     {
      if (DEAD_SIGNATURE == ((ple)pos.pListEntry)->status
       && ((ple)pos.pListEntry)->ref < 0)
       {
	/* Element marked as dead and element readies for removing */
	brk ();

	/* Save pointer on next entry */
	plh pListEntryNext = pos.pListEntry->next;
	Remove (pos.pListEntry, pos.pListHead);

	/* Set pointer on next entry */
	pos.pListEntry = pListEntryNext;
       }
      else
       {
	/* Set pointer on next entry */
	pos.pListEntry = pos.pListEntry->next;
       }
     } /* End while dead elements */

    if (pos.pListEntry != pos.pListHead)
     {	
      /* Bingo !!! Founded equial hash - time to change hash function !!! */
      brk ();

      Lock (pos.pListEntry);
      UNLOCK_THREAD_SWITCHING ();

//      key  = ((ple)pos.pListEntry)->key;
//      hash = ((ple)pos.pListEntry)->hash;
      pValue = Lookup (pos);
      return true;
     }
    
    UNLOCK_THREAD_SWITCHING ();

    Storage[pos.MapElem].LK.UnLock ();
    return false;
   }

  /* Doesn't thread safe method, it called from destructor */
  inline void RemoveAll (void)
   {
    mp pos;

    /* Move to ahead of maps array */
    for (pos.MapElem = 0; pos.MapElem < MaxElem; pos.MapElem++)
     {
      pos.pListHead = &Storage[pos.MapElem].ListHead;

      if (list_empty (pos.pListHead))
	continue;

      plh pListEntryNext;
      list_for_each_safe (pos.pListEntry, pListEntryNext, pos.pListHead)
       {
	brk ();
	Remove (pos.pListEntry, pos.pListHead);

	InterlockedDecrement (&Storage[pos.MapElem].UseCounter);
	InterlockedDecrement (&UseCounter);
       }
     }
   }

  ~ThreadSafeMap (void)
   {
    RemoveAll ();

    if (Storage) delete [] Storage, Storage = NULL;
   }
};

template <class Tmap>
static bool InitMap (Tmap*& pM, int Elem, byte& ProcessorsNumber)
{
  pM = new Tmap (Elem, ProcessorsNumber);

  if (!pM)
    return false;
  else
    return true;
};

#endif
