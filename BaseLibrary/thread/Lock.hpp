#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <winsock2.h>
#include <windows.h>


// 다중 CPU환경에서는 Cache-Line에 대한 고려를 해야만 할 것.
#define CACHE_ALIGN						32
#define CACHE_PAD(Name, BytesSoFar)		BYTE Name[CACHE_ALIGN - ((BytesSoFar) % CACHE_ALIGN)]


template<class DerivedLockClass> class CBLock
{
public:
	class Syncronize
	{
	public:
		inline Syncronize(DerivedLockClass* pLockClass) : Lock(*pLockClass)	{ Lock.Lock();		}
		inline Syncronize(DerivedLockClass& LockClass) : Lock(LockClass)	{ Lock.Lock();		}
		inline ~Syncronize(void)											{ Lock.Unlock();	}
	protected:
		DerivedLockClass& Lock;
	};
};


// 이 Lock클래스들은 더이상 상속을 받아서는 안 된다.

// ----------------------------------------------------------------------------
// DummyLock ------------------------------------------------------------------

class CDummyLock : public CBLock<CDummyLock>
{
public:
	inline void Lock(void)			{}
	inline void Unlock(void)		{}
	inline bool IsVaildLock(void)	{ return true; }
};


// ----------------------------------------------------------------------------
// SpinLock -------------------------------------------------------------------

class CSpinLock : public CBLock<CSpinLock>
{
public:
	inline CSpinLock(void) : m_bLocked(FALSE) {}
	inline ~CSpinLock(void)			{}
	inline void Lock(void)			{ while(TRUE == InterlockedExchange( reinterpret_cast<LONG *> (&m_bLocked), TRUE )) { Sleep(0); } }
	inline void Unlock(void)		{ InterlockedExchange(&m_bLocked, FALSE); }
	inline bool IsVaildLock(void)	{ return true; }
private:
	LONG m_bLocked;
};



// ----------------------------------------------------------------------------
// Critical Section -----------------------------------------------------------

class CBCSLock : public CBLock<CBCSLock>
{
private:
	CRITICAL_SECTION	m_CSLock;
public:
	inline CBCSLock(void)			{ InitializeCriticalSection(&m_CSLock);	}
	inline ~CBCSLock(void)			{ DeleteCriticalSection(&m_CSLock);		}
	inline void Lock(void)			{ EnterCriticalSection(&m_CSLock);		}
	inline void Unlock(void)		{ LeaveCriticalSection(&m_CSLock);		}
	inline bool IsVaildLock(void)	{ return true;							}
};


// ----------------------------------------------------------------------------
// Mutex Lock -----------------------------------------------------------------

class CMutex : public CBLock<CMutex>
{
private:
	HANDLE	m_hMutex;
public:
	inline CMutex(void) : m_hMutex(CreateMutex(NULL, FALSE, NULL)) {}
	inline ~CMutex(void)			{ if (0 != m_hMutex) { CloseHandle(m_hMutex); }		}
	inline DWORD Lock(void)			{ return WaitForSingleObject(m_hMutex, INFINITE);	}
	inline DWORD Unlock(void)		{ return ReleaseMutex(m_hMutex);					}
	inline bool IsVaildLock(void)	{ return (NULL != m_hMutex);						}
};



class CNamedMutex : public CBLock<CNamedMutex>
{
private:
	HANDLE m_hMutex;
public:
	inline CNamedMutex(const char* szMutexName, BOOL bInitialOwner) : m_hMutex(CreateMutex(NULL, bInitialOwner, szMutexName)) {}
	inline ~CNamedMutex(void)		{ if (0 != m_hMutex) { CloseHandle(m_hMutex); }		}
	inline DWORD Lock(void)			{ return WaitForSingleObject(m_hMutex, INFINITE);	}
	inline DWORD Unlock(void)		{ return ReleaseMutex(m_hMutex);					}
	inline bool IsVaildLock(void)	{ return (NULL != m_hMutex);						}
};


/*
class CFairRWLock : private CBCSLock
{
private:
  Semaphore     access_lock;     // used as a one-at-a-time release valve
  CBCSLock       read_barrier;    // used to block/wakeup readers
  unsigned int  is_write_lock;   // nonzero if locked for writing
  unsigned int  writer_count;    // # of writers waiting for or holding the lock
  unsigned int  reader_count;    // # of readers holding the lock
  unsigned int  readers_waiting; // # of readers waiting for the lock

public:
  ReadLock()
      : access_lock(1), is_write_lock(0), 
        writers_waiting(0), reader_count(0), readers_waiting(0)
  {
    
  }

  void ReadLock() 
  {
    Mutex::Lock();

    // if there is at least one writer using the lock or waiting for it,
    // we need to wait for access
    if (writer_count > 0)) 
    {
      if (readers_waiting++ == 0) // if we're the first reader in line
        Mutex::Unlock();
        access_lock.Wait();       // get the access lock
        Mutex::Lock();
        if (readers_waiting > 1)  // then if there are other readers
          read_barrier.Open();    // let them go
      }
      else 
      {
        Mutex::Unlock();
        read_barrier.Wait();      // otherwise wait until someone lets us go
        Mutex::Lock();
      }
      readers_waiting--;
    }
    reader_count++;
    Mutex::Unlock();
  }

  void WriteLock() 
  {
    Mutex::Lock();
    writer_count++;       // one more writer
    Mutex::Unlock();
    access_lock.Wait();   // wait until the access lock is available
    Mutex::Lock();
    is_write_lock = 1;    // lock is a write lock
    read_barrier.Close(); // give readers something to wait for
    Mutex::Unlock();
  }

  void Unlock() 
  {
    Mutex::Lock();
    if (is_write_lock)
    {          // if this is a write lock
      is_write_lock = 0;          // let it go
      writer_count--;             // one less writer
      access_lock.Post();         // now let someone else have a chance
    }
    else if (--reader_count == 0)
    {
        // if we're the last reader
        access_lock.Post();         // release the access lock
    }
    Mutex::Unlock();
  }
};
*/

#endif
