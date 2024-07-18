#pragma once

#include <BaseLibrary/DataStructure/BLockList.hpp>
#include <BaseLibrary/Thread/Lock.hpp>
#include <stdio.h>
#include <BaseLibrary/StdAfx.h>

// 
// 이 메모리 풀을 사용하기 위해서는 클래스 내에 다음과 같이 멤버 정의가 추가되어야 한다.
// - in the head file...
// DECLARE_MEMORY_POOL( ClassType )
// 
// - in the source file...
// DEFINE_MEMORY_POOL( ClassType, NumberOfPoolingUnit );


// :::::::::: 아래는 옛날것 - 위의 매크로 사용법 참조 ::::::::::::::::
//
// 이 메모리 풀을 사용하기 위해서는 클래스 내에 다음과 같이 멤버 정의가 추가되어야 한다.
// - in the head file...
// class Type {
// public:
//		// memory pool relate members
//		Type*			m_pNextMemPtr;
//		static int		ms_nPoolIndex;	// CMemoryPoolFactory 내의 인덱스
//}
// - in the source file...
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// static member
//CMemoryPool<Type>	Type::ms_MemPool("Type", NumberOfPoolingUnit);

#define DECLARE_MEMORY_POOL( cmName ) \
public: \
	static CMemoryPool<##cmName>	ms_MemPool; \
	inline static void* operator new(size_t size) { return ms_MemPool.Pop(); } \
	inline static void* operator new(size_t size, char *pszFilename, int nLine) { return ms_MemPool.Pop(); } \
	inline static void operator delete(void *ptr, char *pszFilename, int nLine) { ms_MemPool.Push((##cmName *) ptr); } \
	inline static void operator delete(void *ptr, size_t size) { ms_MemPool.Push((##cmName *) ptr); } \
	inline static void operator delete(void *ptr) { ms_MemPool.Push((##cmName *) ptr); }

#define DEFINE_MEMORY_POOL( cmName, nPoolNumber ) \
CMemoryPool<cmName>	cmName::ms_MemPool(#cmName, nPoolNumber);


template <typename Type> class CMemoryPool
{
private:
	class CMemoryPoolUnit {
	public:
		CMemoryPoolUnit*	m_pNextMemUnitPtr;
		CMemoryPoolUnit*	m_pHeaderPtr;
		// constructor and destructor
		CMemoryPoolUnit(void) : m_pNextMemUnitPtr(NULL), m_pHeaderPtr(NULL) {}
		~CMemoryPoolUnit(void) {}
	};
	typedef typename class CMemoryPoolUnit*	LPCMemoryPoolUnit;

	enum { LEAST_UNITSIZE	= 12 };
	enum { MAX_POOLNAME_LEN	= 32 };
	char		m_PoolName[CMemoryPool::MAX_POOLNAME_LEN];
	LONG		m_lPoolUnitNum;

	typedef CBLockList<void*>		PoolList;
	PoolList			m_PoolList;

	LPCMemoryPoolUnit	m_pHeadOfList;
	LONG				m_lBasicPoolUnitNum;

	CBCSLock			m_Lock;


	Type*		AllocateMemory(LONG lPoolUnitNum);


public:
	// constructor and destructor
	CMemoryPool(char* szPoolName, LONG lPoolUnitNum);
	~CMemoryPool(void);


	void		Initialize(char* szPoolName, LONG lPoolUnitNum);
	void		Terminate(void);

	Type*		Pop(void);
	void		Push(Type* pPoolUnit);
	char*		GetPoolName(void);
	LONG		GetPoolUnitNumber(void);
};

// constructor and destructor
template<typename Type> CMemoryPool<Type>::CMemoryPool(char* szPoolName, LONG lPoolUnitNum) : m_lPoolUnitNum(0), m_lBasicPoolUnitNum(lPoolUnitNum)
{
	if ( CMemoryPool::LEAST_UNITSIZE > sizeof(Type) ) {
		BASEDEBUGLOG3("create of '%s' pool error - requested pool unit size(%d) is less than least unit size(%d)",
					  m_PoolName, sizeof(Type), CMemoryPool::LEAST_UNITSIZE);
		assert(false && "requested pool unit size is less than least unit size");
	}

	Initialize(szPoolName, lPoolUnitNum);
}

template<typename Type> CMemoryPool<Type>::~CMemoryPool(void)
{
#pragma todo("terminate 시에 풀에 있지 않은 객체들을 풀에 넣으면서 디스트럭터 처리하는 부분을 확인할 것.")
	Terminate();
}

template<typename Type> Type* CMemoryPool<Type>::AllocateMemory(LONG lPoolUnitNum)
{
	void* pRawMemory	= ::operator new (lPoolUnitNum * sizeof(Type));
	if (NULL == pRawMemory) {
		BASEDEBUGLOG3("create of '%s' pool error - requested pool unit number:%d, pool unit size:%d",
					  m_PoolName, lPoolUnitNum, sizeof(Type));
		return NULL;
	}
	Type* pType	= static_cast<Type*>(pRawMemory);

	// initiate array pointer
	for ( int i=0; i<(lPoolUnitNum-1); ++i ) {
		( (LPCMemoryPoolUnit)&pType[i] )->m_pNextMemUnitPtr	=  ((LPCMemoryPoolUnit)&pType[i+1] );
	}
	((LPCMemoryPoolUnit)&pType[lPoolUnitNum-1])->m_pNextMemUnitPtr	= NULL;

	CBCSLock::Syncronize sync(m_Lock);

	m_PoolList.Insert(pRawMemory);

	::InterlockedExchangeAdd(&m_lPoolUnitNum, lPoolUnitNum);

	return reinterpret_cast<Type*>(pType);
}

template<typename Type> void CMemoryPool<Type>::Initialize(char* szPoolName, LONG lPoolUnitNum)
{
	if (0 == lPoolUnitNum) {
		BASEDEBUGLOG1("initialization of '%s' pool error - parameter of pool unit number is 0", m_PoolName);
		return;
	}

	if (0 > ::_snprintf(m_PoolName, CMemoryPool::MAX_POOLNAME_LEN, "%s", szPoolName))	// set pool name
		m_PoolName[CMemoryPool::MAX_POOLNAME_LEN - 1]	= '\0';

	m_pHeadOfList	= reinterpret_cast<LPCMemoryPoolUnit>(AllocateMemory(lPoolUnitNum));
}

template<typename Type> void CMemoryPool<Type>::Terminate(void)
{
	BASEDEBUGLOG3("termination of '%s' memory pool - current pool unit number:%d, basic unit number:%d", 
				  m_PoolName, m_lPoolUnitNum, m_lBasicPoolUnitNum);

	while ( void* pMemory = m_PoolList.PopHead() ) {
		BASEDEBUGLOG2("termination of '%s' memory pool - head ptr:0x%08x", m_PoolName, pMemory);
		delete pMemory;
	}
}

template<typename Type> Type* CMemoryPool<Type>::Pop(void)
{
	CBCSLock::Syncronize sync(m_Lock);

	LPCMemoryPoolUnit pPoolUnit	= m_pHeadOfList;
	if (pPoolUnit) {
		m_pHeadOfList	= pPoolUnit->m_pNextMemUnitPtr;
	} else {
		BASEDEBUGLOG3("operate new of '%s' pool error - head ptr:0x%08x, current pool unit number:%d",
					  m_PoolName, m_pHeadOfList, m_lPoolUnitNum);

		m_pHeadOfList	= reinterpret_cast<LPCMemoryPoolUnit>(AllocateMemory(m_lBasicPoolUnitNum));
		pPoolUnit	= m_pHeadOfList;
		if (NULL == pPoolUnit) {
			BASEDEBUGLOG3("operate new of '%s' pool error - head ptr:0x%08x, current pool unit number:%d",
						  m_PoolName, m_pHeadOfList, m_lPoolUnitNum);
			return NULL;
		}
		m_pHeadOfList	= pPoolUnit->m_pNextMemUnitPtr;
	}
	::InterlockedDecrement(&m_lPoolUnitNum);

	return reinterpret_cast<Type*>(pPoolUnit);
}

template<typename Type> void CMemoryPool<Type>::Push(Type* pPoolUnit)
{
	CBCSLock::Syncronize sync(m_Lock);

	((LPCMemoryPoolUnit)pPoolUnit)->m_pNextMemUnitPtr	= m_pHeadOfList;
	m_pHeadOfList				= (LPCMemoryPoolUnit)pPoolUnit;

	::InterlockedIncrement(&m_lPoolUnitNum);
}

template<typename Type> char* CMemoryPool<Type>::GetPoolName(void)
{
	return (char*)m_PoolName;
}

template<typename Type> LONG CMemoryPool<Type>::GetPoolUnitNumber(void)
{
	return m_lPoolUnitNum;
}
