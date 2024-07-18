/*********************************************************************************************************
 *
 *  Module Name:	nbmap.h
 *
 *  Abstract:		Not Bloked Thread Safe Tree Storage (NBTSTS). It works without blocking on any 
 *			number processor platform. Multimap methods implemented also (Start() and Next()
 *			methods by hash).
 *
 *  Author:		Vyacheslav I. Levtchenko (mail-to: sl@eltrast.ru)
 *
 *  Revision History:	05.08.2003 started
 *
 *  Classes, methods and structures:
 *
 *  External:	HashKey, Time_Slice_Sleep
 *  Internal:	ThreadSafeMap, MapPos(EnumPos)
 *
 *  TODO:
 *
 *********************************************************************************************************/

#ifndef __NBMAP_H__
#define __NBMAP_H__

/* Map initial configuration */
#define LEVEL_LENGTH	4
#define HASH_LENGTH	(sizeof (Thash) << 3)
#define MAX_LEVELS	((HASH_LENGTH / LEVEL_LENGTH) + 1)

/* Used on map enumerating */
typedef struct EnumPos
{
  void* Map;	      /* back trace map */
  unsigned long Elem; /* back trace element */
} ep, *pep;

typedef struct MapPos
{
  void* Map;
  unsigned long Elem; /* Current locked element */
  unsigned long Cnt;  /* Enum counter */
  pep p;

   MapPos () : Map(NULL), Elem(0), Cnt(0), p(NULL) {}
  ~MapPos () { if (p) { delete [] p, p = NULL; } }
   operator = (const MapPos& old) { Map = old.Map; Elem = old.Elem; }

public:
  bool EnumInit (const long Levels)
   {
    if (!p)
     {
      p = new ep [Levels];
      if (!p) { brk ();	return false; }
     }

    memset (p, 0, sizeof (ep) * Levels);
    Cnt = 0;
    return true;
   }
} mp, *pmp;

/* Status object graph */

/* +---------------------LOOPBACK----------------------+ */
/* +-> FREE -> BUSY -> LIVE -> KILL +---->---+-> ERAS -+ */
/*                                  +-> DEAD +           */

template <class Tkey, class Tvalue, class Thash = unsigned long>
class ThreadSafeMap
{
  typedef struct MapElem
   {
    /* redanted service information */
    long  ref;
    long  status; /* "FREE" || "BUSY" || "LIVE" || "KILL" || "DEAD" || "ERAS" */

    /* usefull payload */
    Tkey  key;
    Thash hash;
    Tvalue*        pval;
    ThreadSafeMap* pmap; /* Collision map */
   } me, *pme;

  Thash MaxElem;
  long  UseCounter;

  pme Storage, TopStorage;
  HashKey<Tkey, Thash> HK;

  typedef struct HashPart
   {
    Thash mask;
    unsigned char off;
   } hp, *php;

  typedef struct MapArch
   {
    hp HP [MAX_LEVELS];
    unsigned char Levels; /* 0-based. Levels = (real Levels - 1) */
   } ma, *pma;

  pma pMA;
  unsigned char Level;

  /* Private map methods */

  /* Lock reference on element */
  static inline long Lock (IN pme& pElem)
   {
    return InterlockedIncrement(&pElem->ref);
   }

  /* Free reference on element */
  static inline long Release (IN pme& pElem)
   {
    return InterlockedDecrement(&pElem->ref);
   }

  /* Synoname of Release */
  static inline long UnLock (IN pme& pElem)
   {
    return Release (pElem);
   }

  /* Lock reference counter removed element */
  static inline long LockRemove (IN pme& pElem)
   {
    return (InterlockedExchangeAdd (&pElem->ref, MINUS_NULL) + (long)MINUS_NULL);
   }

  /* Free reference counter removed element */
  static inline void ReleaseRemove (IN pme& pElem)
   {
    InterlockedExchangeAdd (&pElem->ref, MINUS_MEDIAN);
   }

  /* Synoname of ReleaseRemove */
  static inline void UnLockRemove (IN pme& pElem)
   {
    ReleaseRemove (pElem);
   }

  /* Change status of map element from XXX -> (to) YYY */
  inline long ChangeStatus (IN OUT pme& pElem, IN long NewStatus, IN long PreviosStatus)
   {
    return (long)InterlockedCompareExchange ((PVOID*)&pElem->status, (PVOID)NewStatus, (PVOID)PreviosStatus);
   }

  /* CleanUp element */
  inline bool RemoveDead (IN OUT pme pElem)
   {
    if (!pElem->pval) /* BUSY && ERAS may be */
     {
      pElem->ref  = 0;
      pElem->hash = NULL;
      pElem->key  = NULL;
      pElem->status = FREE_SIGNATURE;
      return false;
     }

    delete pElem->pval, pElem->pval = NULL;
    pElem->ref  = 0;
    pElem->hash = NULL;
    pElem->key  = NULL;
    pElem->status = FREE_SIGNATURE;

    InterlockedDecrement (&UseCounter);
    return true;
   }

  /* ERAS -> FREE. Enable only in ERASE status. If successfull than setup FREE status */
  inline bool Erase (IN OUT pme& pElem)
   {
    Tvalue* locp = pElem->pval;
    Tvalue* prev = (Tvalue*)InterlockedCompareExchange ((PVOID*)&pElem->pval, (PVOID)NULL, (PVOID)locp);

    if (locp
     && locp == prev)
     {
      pElem->hash = NULL;
      pElem->key  = NULL;

      InterlockedDecrement (&UseCounter);
      if (locp) delete locp, locp = NULL; /* delete pval */

      /* Set FREE status */
      ChangeStatus (pElem, FREE_SIGNATURE, ERAS_SIGNATURE);
      return true;
     }
    else
     {
      /* Element corupted, very bad break point !!! */
      brk ();
      /* status don't touch */
      return false;
     }
   }

  /* DEAD -> ERAS -> FREE. Return true if erase dead element */
  inline bool EraseDead (IN OUT pme& pElem)
   {
    /* Change status on ERASE */
    long status = ChangeStatus (pElem, ERAS_SIGNATURE, DEAD_SIGNATURE);

    if (DEAD_SIGNATURE == status)
     {
      /* Setup ERASE status successfull */
      if ((unsigned long)pElem->ref == MINUS_NULL)
       {
	/* Reference counter locked successfull */
	/* Begin termination dead element of map */
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
  inline bool EraseKilled (IN OUT pme& pElem)
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
  /* Return false if pElem biger of TopStorage */
  bool Remove (IN pme pElem)
   {
    if (pElem >= TopStorage) { brk (); return false; }

    /* If LIVE status passed, than entry to killing status */
    long status = ChangeStatus (pElem, KILL_SIGNATURE, LIVE_SIGNATURE);

    /* Release element */
    Release (pElem); // == 0

    /* Map element status control */
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
	brk ();
       }
      else
       {
	/* Element status corrupted, very bad break point !!! */
	brk ();
       }

      return false;
     } /* end LIVE_SIGNATURE != status */

    /* set KILL status successfull */

    /* Try to lock reference counter for using */
    if ((unsigned long)LockRemove (pElem) == MINUS_NULL) /* Reference counter locked successfull */
     {
      /* Begin termination element of map */
      if (EraseKilled (pElem))
       {
	/* Unlock reference counter from unchanged state */
	ReleaseRemove (pElem);
	return true;
       }
     }

    /* Map element's in using, try to wait releasing */
    for (unsigned long retry = SPINLOCK_COUNTER; retry > 0; retry--)
     {
      if ((unsigned long)pElem->ref == MINUS_NULL)
        break;

      Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
     }

    if (!retry)
     {
      brk ();

      /* Mark map element for removing */
      status = ChangeStatus (pElem, DEAD_SIGNATURE, KILL_SIGNATURE);
      return false;
     }

    /* Begin termination element of map */
    if (EraseKilled (pElem))
     {
      /* Unlock reference counter from unchanged state */
      ReleaseRemove (pElem);
      return true;
     }

    return false;
   }

  /* Get element index in map by key hash */
  inline Thash GetElem (const Thash& hash)
   {
    if (!pMA) { brk (); return 0; }

    php pHP = &pMA->HP[Level];
    return (hash >> pHP->off) & pHP->mask;
   }

  inline unsigned char NumBits (Thash val)
   {
    unsigned char cnt = 0;
    while (val & 1) { val >>= 1; cnt++; }
    return cnt;
   }

  inline Thash GetMask (unsigned char size)
   {
    Thash mask = 0;
    while (size--) { mask |= 1 << size; }
    return mask;
   }

  inline bool MapInit (void)
   {
    MaxElem = 1 << NumBits (pMA->HP[Level].mask);

    /* Use MaxElem */
    Storage = new me [MaxElem];

    if (!Storage) { brk(); return false; }

    memset (Storage, 0, sizeof (me) * MaxElem);

    pme p = Storage;

    /* Initialize map elements */
    for (Thash i = 0; i < MaxElem; i++, p++)
      p->status = FREE_SIGNATURE;

    TopStorage = Storage + MaxElem;
    return true;
   }

public:
  /* Root map initilise */
  ThreadSafeMap (const Thash NumElem = 32, const byte ProcessorsNumber = 1) : Storage(NULL), TopStorage(NULL), UseCounter(0), MaxElem(0), Level(0)
   {
    if (!NumElem) { brk (); return; }

    pMA = new ma;
    if (!pMA) { brk (); return; }

    pMA->Levels = 0;

    unsigned char size = NumBits (NumElem - 1);
    pMA->HP[pMA->Levels].off  = 0;
    pMA->HP[pMA->Levels].mask = GetMask (size);

    /* Generate LevelSizes and LevelMask */
    unsigned char last = (unsigned char)HASH_LENGTH - size;

    while (last)
     {
      if (++(pMA->Levels) >= (unsigned char)(MAX_LEVELS - 1))
       {
	brk ();
	pMA->Levels = (unsigned char)(MAX_LEVELS - 1);
	return;
       }

      pMA->HP[pMA->Levels].off = pMA->HP[pMA->Levels - 1].off + size; /* previos level size plus previos offset */
      size = last > LEVEL_LENGTH ? LEVEL_LENGTH : last;
      pMA->HP[pMA->Levels].mask = GetMask (size);
      last -= size;
     }

    MapInit ();
   }

  /* Sub map initilise */
  ThreadSafeMap (const unsigned char Lev, const pma MA) : Storage(NULL), TopStorage(NULL), UseCounter(0), MaxElem(0), Level(1)
   {
    if (!MA || !Lev || Lev > MA->Levels) { brk (); return; }

    Level = Lev;
    pMA   = MA;

    MapInit ();
   }

  /* Doesn't thread safe method */
  inline bool IsEmpty (void)
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
    if (!Storage || !MaxElem) { brk (); return 0; }

    return Storage[MapElem % MaxElem].ref;
   }

  /* Get hash by key */
  inline Thash Hash (IN const Tkey& key)
   {
    return HK.Hash (key);
   }

#define GO_DOWN_RET(METHOD)	\
  if (pElem->pmap)		\
    return pElem->pmap->METHOD;	\
  else				\
    return false;

  /* Search array element by key & lock it */
  inline bool SearchByKey (IN OUT mp& pos, IN Tkey& key, IN Thash& hash, OUT Tvalue*& pValue)
   {
    if (!Storage || !MaxElem) { brk (); return false; }

    pos.Map   = this;
    pos.Elem  = GetElem (hash);
    pme pElem = &Storage [pos.Elem];

    if (LIVE_SIGNATURE != pElem->status)
     {
      if (EraseDead (pElem))
       {
	brk ();
	/* Unlock reference counter from unchanged state */
	ReleaseRemove (pElem);
       }

      /* Element empty, go down */
      GO_DOWN_RET(SearchByKey (pos, key, hash, pValue));
     }

    /* Lock element */
    if (Lock (pElem) <= 0)
     {
      /* Locked counter detected */
      UnLock (pElem);
      GO_DOWN_RET(SearchByKey (pos, key, hash, pValue));
     }

    if (LIVE_SIGNATURE != pElem->status)
     {
      UnLock (pElem);
      GO_DOWN_RET(SearchByKey (pos, key, hash, pValue));
     }

    /* Element successfully locked */
    if (pElem->key == key)
     {
      pValue = pElem->pval;
      return true;
     }

    /* Key doesn't equial, go down */
    UnLock (pElem);
    GO_DOWN_RET(SearchByKey (pos, key, hash, pValue));
   }

  /* Search array element by hash & lock it */
  inline bool SearchByHash (IN OUT mp& pos, IN Thash& hash, OUT Tvalue*& pValue)
   {
    if (!Storage || !MaxElem) { brk (); return false; }

    pos.Map   = this;
    pos.Elem  = GetElem (hash);
    pme pElem = &Storage [pos.Elem];

    if (LIVE_SIGNATURE != pElem->status)
     {
      if (EraseDead (pElem))
       {
	brk ();
	/* Unlock reference counter from unchanged state */
	ReleaseRemove (pElem);
       }

      /* Element empty, go down */
      GO_DOWN_RET(SearchByHash (pos, hash, pValue));
     }

    /* Lock element */
    if (Lock (pElem) <= 0)
     {
      /* Locked counter detected */
      UnLock (pElem);
      GO_DOWN_RET(SearchByHash (pos, hash, pValue));
     }

    if (LIVE_SIGNATURE != pElem->status)
     {
      UnLock (pElem);
      GO_DOWN_RET(SearchByHash (pos, hash, pValue));
     }

    /* Element successfully locked */
    if (pElem->hash == hash)
     {
      pValue = pElem->pval;
      return true;
     }

    /* Hash doesn't equial, go down */
    UnLock (pElem);
    GO_DOWN_RET(SearchByHash (pos, hash, pValue));
   }

  /* Insert element in map & if successfull than lock element */
  bool SetAt (IN OUT mp& pos, IN Tkey& key, IN Thash& hash, IN Tvalue& val)
   {
    if (!Storage || !MaxElem) { brk (); return false; }

    pos.Map   = this;
    pos.Elem  = GetElem (hash);
    pme pElem = &Storage [pos.Elem];

    long status = BUSY_SIGNATURE;

    for (long retry = SPINLOCK_COUNTER; retry > 0; retry--)
     {
      if (FREE_SIGNATURE != pElem->status)
       {
	if (EraseDead (pElem))
	 {
	  /* Try to lock free map element */
	  status = ChangeStatus (pElem, BUSY_SIGNATURE, FREE_SIGNATURE);

	  if (FREE_SIGNATURE != status)
	   {
	    brk (); /* Bingo!!! - concurent SetAt in progress!!! */

	    /* Unlock reference counter from unchanged state */
	    ReleaseRemove (pElem);

	    Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
	    continue;
	   }

	  brk ();

	  /* Lock element */
	  Lock (pElem);

	  /* Unlock reference counter from unchanged state */
	  ReleaseRemove (pElem);

	  break;
	 }
	else /* Collision detected */
	if (FREE_SIGNATURE != pElem->status)
	 {
	  if (Level == pMA->Levels)
	   {
	    brk ();
	    Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
	    continue;
	   }
	
	  if (pElem->pmap) /* Already used, go down */
	    return pElem->pmap->SetAt (pos, key, hash, val);

	  /* This is simple element, try replace on map */
	  ThreadSafeMap* Map = new ThreadSafeMap (Level + 1, pMA);
	  if (!Map)
	   {
	    brk ();
	    Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
	    continue;
	   }

	  if (!Map->SetAt (pos, key, hash, val))
	   {
	    brk ();
	    delete Map;
	    Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
	    continue;
	   }

	  /* Activate record */
	  status = (long)InterlockedCompareExchange ((PVOID*)&pElem->pmap, (PVOID)Map, (PVOID)NULL);

	  if (status)
	   {
	    brk (); /* Concurent map SetAt detected */
	    delete Map;

	    if (pElem->pmap) /* Already used, go down */
	      return pElem->pmap->SetAt (pos, key, hash, val);
	    else
	      {
	       brk ();
	       Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
	       continue;
	      } /* may be only with removing submap */
	   }
	  else
	    return true;
	 }
       } /* !FREE */

      /* Lock element */
      if (Lock (pElem) <= 0)
       {
	/* Locked counter detected */
	UnLock (pElem);
	Time_Slice_Sleep (SPINLOCK_SLEEP_TIME);
	continue;
       }

      /* Try to lock free map element */
      status = ChangeStatus (pElem, BUSY_SIGNATURE, FREE_SIGNATURE);

      if (FREE_SIGNATURE == status)
	break;

      UnLock (pElem);
     } /* end for retry */

    if (!retry)
     {
      if (pElem->pmap) /* while used, go down */
	return pElem->pmap->SetAt (pos, key, hash, val);
      else
	brk ();

      return false;
     }

    /* Allocate new map value */
    Tvalue* pval = new Tvalue;
    if (!pval)
     {
      brk ();

      ChangeStatus (pElem, FREE_SIGNATURE, BUSY_SIGNATURE);
      UnLock (pElem);

      return false;
     }

    pElem->key  = key;
    pElem->hash = hash;

    // memcpy (pElem->pval, &val, sizeof (Tvalue));
    *pval = val;

    /* Activate record */
    pElem->pval = pval;
    ChangeStatus (pElem, LIVE_SIGNATURE, BUSY_SIGNATURE);
    InterlockedIncrement (&UseCounter);

    return true;
   }

  /* Insert element in map & if successfull than lock element */
  inline bool SetAt (IN OUT mp& pos, IN Tkey& key, IN Tvalue& val)
   {
    Thash hash;
    return SetAt (pos, key, hash = HK.Hash (key), val);
   }

  /* Look for element in map by key & if successfull search than lock element */
  inline bool LookupByKey (IN OUT mp& pos, IN Tkey& key, OUT Tvalue*& pValue)
   {
    Thash hash;
    return SearchByKey (pos, key, hash = HK.Hash (key), pValue);
   }

  /* Look for element in map by keys hash & if successfull search than lock element */
  inline bool LookupByKeyHash (IN OUT mp& pos, IN Tkey& key, OUT Tvalue*& pValue)
   {
    Thash hash;
    return SearchByHash (pos, hash = HK.Hash (key), pValue);
   }

  /* Look for element in map by hash & if successfull search than lock element */
  inline bool LookupByHash (IN OUT mp& pos, IN Thash& hash, OUT Tvalue*& pValue)
   {
    return SearchByHash (pos, hash, pValue);
   }

  /* Look for element in map by position */
  inline Tvalue* Lookup (IN mp& pos)
   {
    if (!pos.Map) { brk (); return NULL; }

    if (this != (ThreadSafeMap*)pos.Map)
      return ((ThreadSafeMap*)pos.Map)->Lookup (pos);

    if (!((ThreadSafeMap*)pos.Map)->Storage
      || pos.Elem >= ((ThreadSafeMap*)pos.Map)->MaxElem) { brk (); return NULL; }

    return ((ThreadSafeMap*)pos.Map)->Storage [pos.Elem].pval;
   }

  /* Unlock position in map */
  inline void Release (IN mp& pos)
   {
    if (!pos.Map) { brk (); return; }

    if (this != (ThreadSafeMap*)pos.Map)
     {
      ((ThreadSafeMap*)pos.Map)->Release (pos);
      return;
     }

    if (!((ThreadSafeMap*)pos.Map)->Storage
      || pos.Elem >= ((ThreadSafeMap*)pos.Map)->MaxElem) { brk (); return; }

    pme pElem = &((ThreadSafeMap*)pos.Map)->Storage [pos.Elem];
    Release (pElem);
   }

  /* Synoname of Release */
  inline void UnLock (IN mp& pos)
   {
    Release (pos);
   }

  /* Remove element from map and always unlock element */
  inline bool Remove (IN mp& pos)
   {
    if (!pos.Map) { brk (); return false; }

    if (this != (ThreadSafeMap*)pos.Map)
      return ((ThreadSafeMap*)pos.Map)->Remove (pos);

    if (!((ThreadSafeMap*)pos.Map)->Storage
      || pos.Elem >= ((ThreadSafeMap*)pos.Map)->MaxElem) { brk (); return false; }

    return Remove (&((ThreadSafeMap*)pos.Map)->Storage [pos.Elem]);
   }

  /* Remove element from map on cleanup */
  inline bool RemoveDead (IN mp& pos)
   {
    if (!pos.Map) { brk (); return false; }

    if (this != (ThreadSafeMap*)pos.Map)
      return ((ThreadSafeMap*)pos.Map)->RemoveDead (pos);

    if (!((ThreadSafeMap*)pos.Map)->Storage
      || pos.Elem >= ((ThreadSafeMap*)pos.Map)->MaxElem) { brk (); return false; }

    return RemoveDead (&((ThreadSafeMap*)pos.Map)->Storage [pos.Elem]);
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

#define GO_DOWN_CON(METHOD)	\
  if (pElem->pmap)		\
   {/* Store current location */\
    pos.p[Level].Map  = pos.Map;\
    pos.p[Level].Elem = pos.Elem;\
    if (pElem->pmap->METHOD)	\
      return true;		\
    else			\
      continue;			\
   }				\
  else				\
    continue;

  /* Check and lock LIVE element else go down in to lower map or  */
  inline bool ProcessElem (IN OUT mp& pos, OUT Tkey& key, OUT Thash& hash, OUT Tvalue*& pValue)
   {
    if (!Storage) { brk (); return false; }

    pme pElem;

    /* Look for LIVE element or not empty low map */
    for (; pos.Elem < MaxElem; pos.Cnt++, pos.Elem++)
     {
      pElem = &Storage [pos.Elem];

      if (LIVE_SIGNATURE != pElem->status)
       {
	if (EraseDead (pElem))
	 {
	  /* Unlock reference counter from unchanged state */
	  ReleaseRemove (pElem);
	 }

	GO_DOWN_CON(Start (pos, key, hash, pValue));
       }

      /* Lock element */
      if (Lock (pElem) <= 0)
       {
	/* Locked counter detected */
	UnLock (pElem);
	GO_DOWN_CON(Start (pos, key, hash, pValue));
       }

      if (LIVE_SIGNATURE != pElem->status)
       {
	UnLock (pElem);
	GO_DOWN_CON(Start (pos, key, hash, pValue));
       }

      key    = pElem->key;
      hash   = pElem->hash;
      pValue = pElem->pval;
      return true;
     } /* End while pos.Elem  <= MaxElem */

    if (!Level)
     {
      brk ();
      return false;
     }

    /* End map, go to new up horizontal element */
    pos.Cnt++;
    pos.Map  =   pos.p[Level - 1].Map;
    pos.Elem = ++pos.p[Level - 1].Elem;
    return ((ThreadSafeMap*)pos.Map)->ProcessElem (pos, key, hash, pValue);
   }

  /* Next maps enumerating & if failure than unlock last element */
  inline bool Next (IN OUT mp& pos, OUT Tkey& key, OUT Thash& hash, OUT Tvalue*& pValue)
   {
    if (!pos.Map) { brk (); return false; }

    /* Go to current map */
    if (this != pos.Map)
      return ((ThreadSafeMap*)pos.Map)->Next (pos, key, hash, pValue);

    /* Process previos element */
    pme pElem = &Storage [pos.Elem];
    UnLock (pElem);

    /* Process previos elements map */
    if (pElem->pmap)
     {/* Store current location */
      pos.p[Level].Map  = pos.Map;
      pos.p[Level].Elem = pos.Elem;

      /* If succefully than begin scaning low map */
      if (pElem->pmap->Start (pos, key, hash, pValue))
	return true;
     }

    /* Go to new horizontal element */
    pos.Cnt++, pos.Elem++;
    return ProcessElem (pos, key, hash, pValue);
   }

  /* Begin maps enumerating & if successfull than lock element */
  inline bool Start (IN OUT mp& pos, OUT Tkey& key, OUT Thash& hash, OUT Tvalue*& pValue)
   {
    if (!pMA) { brk (); return false; }

    if (!Level)
      if (!pos.EnumInit (pMA->Levels))
	return false;

    pos.Map  = this;
    pos.Elem = 0;
    return ProcessElem (pos, key, hash, pValue);
   }

  inline ThreadSafeMap* GetNextMap (OUT Thash& hash)
   {
    if (!Storage) { brk (); return NULL; }

    return Storage [GetElem (hash)].pmap;
   }

  /* Next maps enumerating by hash (AKA multimap) & if failure than unlock last element */
  inline bool Next (IN OUT mp& pos, IN Thash& hash, OUT Tvalue*& pValue)
   {
    if (!pos.Map) { brk (); return false; }

    ((ThreadSafeMap*)pos.Map)->UnLock (pos);

    /* Go on next map */
    ThreadSafeMap* NextMap = ((ThreadSafeMap*)pos.Map)->GetNextMap (hash);

    if (NextMap)
      return NextMap->SearchByHash (pos, hash, pValue);
    else
      return false;
   }

  /* Begin maps enumerating by hash (AKA multimap) & if successfull than lock element */
  inline bool Start (IN OUT mp& pos, IN Thash& hash, OUT Tvalue*& pValue)
   {
    return SearchByHash (pos, hash, pValue);
   }

  /* Doesn't thread safe method, it called from destructor */
  inline void RemoveAll (void)
   {
    /* Move to ahead of map array */
    for (pme pElem = Storage; pElem < TopStorage; pElem++)
     {
      if (pElem->pmap)
	pElem->pmap->RemoveAll ();

      if (FREE_SIGNATURE == pElem->status)
	continue;

      if (!pElem->pval) /* BUSY && ERAS may be */
	continue;

      delete pElem->pval, pElem->pval = NULL;
      InterlockedDecrement (&UseCounter);
     }
   }

  ~ThreadSafeMap (void)
   {
    RemoveAll ();

    if (Storage) delete [] Storage, Storage = NULL;
    if (!Level && pMA) delete pMA, pMA = NULL;
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
