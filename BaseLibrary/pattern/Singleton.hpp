#pragma once

#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

#include <BaseLibrary/Thread/Lock.hpp>

// Method1: template class
#include <cassert>

template<typename Derived> class CSingleton
{
private:
	static Derived* ms_pSingleton;

protected:
	// constructor and destructor
	CSingleton(void);
	virtual ~CSingleton(void);

public:
	inline static bool IsInitialized() { return NULL != ms_pSingleton; }
	inline static Derived& GetInstance(void);
	inline static Derived* GetInstancePtr(void);
};

template<typename Derived> CSingleton<Derived>::CSingleton( void )
{
	assert(!ms_pSingleton && "Singleton 클래스가 이미 생성되어 있습니다.");
	size_t nOffset = (size_t)(Derived*) 1 - (size_t)(CSingleton<Derived>*)(Derived*) 1;
	ms_pSingleton = (Derived*)((size_t)this + nOffset);
}

template<typename Derived> CSingleton<Derived>::~CSingleton( void )
{
	assert(ms_pSingleton && "Singleton 클래스가 생성되지 않았습니다");
    ms_pSingleton = 0;
}

template<typename Derived> inline Derived& CSingleton<Derived>::GetInstance( void )
{
	assert( ms_pSingleton && "Singleton 클래스가 생성되지 않았습니다" );
    return ( *ms_pSingleton );
}

template<typename Derived> inline Derived* CSingleton<Derived>::GetInstancePtr( void )
{
	assert( ms_pSingleton && "Singleton 클래스가 생성되지 않았습니다" );
    return ( ms_pSingleton );
}

template<typename Derived> Derived* CSingleton<Derived>::ms_pSingleton = 0;

// Method2: MACRO
#define DECLARE_SINGLETON_CLASS( cmName )	\
private: \
  static cmName	s_Instance;	\
public: \
	static cmName*	GetInstance(void)		{ return &s_Instance; }	\
	static cmName*	GetInstancePtr(void)	{ return &s_Instance; }

#define DEFINE_SINGLETON_CLASS( cmName )	cmName cmName::s_Instance;

#endif















/*

namespace Threading { 

///////////////////////////////////////////////////////////////////////////
//	class MultiThreaded
///////////////////////////////////////////////////////////////////////////

	//	Defines the locking mechanisim for single threaded model
	//	which is actually empty
	template<typename T>
	class MultiThreaded {
	public : 
		typedef T VolatileType;

	//private:
		//CBCSLock	m_Lock;

	protected : 
		inline explicit MultiThreaded() { }
		inline ~MultiThreaded() { }

	private : 
		inline explicit MultiThreaded(MultiThreaded const&) {}
		inline MultiThreaded& operator=(MultiThreaded const&) { return *this; }
	};	//	end of class MultiThreaded

};	//	end of namespace Threading

namespace Traits {

///////////////////////////////////////////////////////////////////////////
//	class TypeTraits
///////////////////////////////////////////////////////////////////////////

	//	defines useful typetraits for types
	template<typename T>
	class TypeTraits {
	public : 
		typedef T Type;
		typedef T& RefType;
		typedef T* PointerType;

	protected : 
		inline explicit TypeTraits() {}
		inline ~TypeTraits() {}

	private : 
		inline explicit TypeTraits(TypeTraits const&) {}
		inline TypeTraits& operator=(TypeTraits const&) { return *this; }
	};	//	end of class TypeTraits


};	//	end of namespace Traits



///////////////////////////////////////////////////////////////////////////
//	class NoCreation
///////////////////////////////////////////////////////////////////////////

	//	disables creation of object
	template<typename T>
	class NoCreation {
	protected : 
		inline explicit NoCreation() {}
		inline ~NoCreation() {}

		//	creation/destruction/storage issues
		inline static T* CreateInstance() { throw std::logic_error("NoCreation::CreateInstance called"); }
		inline static void DestroyInstance(T*) { throw std::logic_error("NoCreation::DestroyInstance called"); }

	private : 
		inline explicit NoCreation(NoCreation const&) {}
		inline NoCreation& operator=(NoCreation const&) { return *this; }
	};	//	end of class NoCreation

///////////////////////////////////////////////////////////////////////////
//	class CreateUsingNew
///////////////////////////////////////////////////////////////////////////

	//	creates the object with a normal new invocation
	template<typename T>
	class CreateUsingNew {
	protected : 
		inline explicit CreateUsingNew() {}
		inline ~CreateUsingNew() {}

		//	creation/destruction/storage issues
		inline static T* CreateInstance() { return new T(); }
		inline static void DestroyInstance(T* t) { delete t; }

	private : 
		inline explicit CreateUsingNew(CreateUsingNew const&) {}
		inline CreateUsingNew& operator=(CreateUsingNew const&) { return *this; }
	};	//	end of class CreateUsingNew

///////////////////////////////////////////////////////////////////////////
//	class CreateUsingStatic
///////////////////////////////////////////////////////////////////////////

	//	creates the object with a static memory allocation
	//	but not static lifespan
	template<typename T>
	class CreateUsingStatic {
	protected : 
		inline explicit CreateUsingStatic() {}
		inline ~CreateUsingStatic() {}

		//	creation/destruction/storage issues
		inline static T* CreateInstance() {
			new(t_) T();
			return reinterpret_cast<T*>(t_);
		}
		inline static void DestroyInstance(T* t) { t->~T(); }

	private : 
		static unsigned char t_[];

	private : 
		inline explicit CreateUsingStatic(CreateUsingStatic const&) {}
		inline CreateUsingStatic& operator=(CreateUsingStatic const&) { return *this; }
	};	//	end of class CreateUsingStatic

	template<typename T>
	unsigned char CreateUsingStatic<T>::t_[sizeof(T)];

///////////////////////////////////////////////////////////////////////////
//	class DefaultLifetime
///////////////////////////////////////////////////////////////////////////

	//	dictates a default/normal life time of LIFO destruction
	//	uses atexit
	//	dead reference throws an exception
	template<typename T>
	class DefaultLifetime {
	protected : 
		inline explicit DefaultLifetime() {}
		inline ~DefaultLifetime() {}

		//	dead reference issues
		inline static void OnDeadReference() { throw std::logic_error("Dead Reference Detected"); }
		inline static void ScheduleForDestruction(void (*pFun)()) { std::atexit(pFun); }

	private : 
		inline explicit DefaultLifetime(DefaultLifetime const&) {}
		inline DefaultLifetime& operator=(DefaultLifetime const&) { return *this; }
	};	//	end of class DefaultLifetime

///////////////////////////////////////////////////////////////////////////
//	class PhoenixSingleton
///////////////////////////////////////////////////////////////////////////

	//	allows recurring singleton
	template<typename T>
	class PhoenixSingleton {
	protected : 
		inline explicit PhoenixSingleton() {}
		inline ~PhoenixSingleton() {}

		//	dead reference issues
		inline static void OnDeadReference() {}
		inline static void ScheduleForDestruction(void (*pFun)()) { std::atexit(pFun); }

	private : 
		inline explicit PhoenixSingleton(PhoenixSingleton const&) {}
		inline PhoenixSingleton& operator=(PhoenixSingleton const&) { return *this; }
	};	//	end of class PhoenixSingleton

///////////////////////////////////////////////////////////////////////////
//	class PhoenixSingleton
///////////////////////////////////////////////////////////////////////////

	//	no destruction calls, unlimited lifetime
	template<typename T>
	class NoDestruction {
	protected : 
		inline explicit NoDestruction() {}
		inline ~NoDestruction() {}

		//	dead reference issues
		inline static void OnDeadReference() {}
		inline static void ScheduleForDestruction(void (*)()) {}

	private : 
		inline explicit NoDestruction(NoDestruction const&) {}
		inline NoDestruction& operator=(NoDestruction const&) { return *this; }
	};	//	end of class NoDestruction

///////////////////////////////////////////////////////////////////////////
//	class CSingleton
///////////////////////////////////////////////////////////////////////////

	//	Gives a class a singleton property
	//	protected access of ctor and dtor allows creation of subclass of singleton
	//	to acheieve the effect of YourSingleton::Instance()
	//	extends the typetraits just to take advantage of the basic typedefs
	template<typename T, typename CreationPolicy = CreateUsingNew<T>, template <typename> class LifetimePolicy = DefaultLifetime, template <typename> class ThreadingModel = Threading::MultiThreaded>
	class CSingleton : public Traits::TypeTraits<typename ThreadingModel<T>::VolatileType>, public CreationPolicy, public LifetimePolicy<T>, public ThreadingModel<T> {
	public : 
		//	grabs the singleton's instance, if any
		//	otherwise creates an instance
		static PointerType Instance();

		//static PointerType InstancePtr();
		//	performs cleanup on the singleton
		static void Destroy();
		//	resets the singleton instance with a new user created instance
		static void Reset(PointerType, void (*pFun)(T*));

	protected : 
		inline explicit CSingleton() { CSingleton::instance_ = static_cast<PointerType>(this); CSingleton::destroyed_ = false; CSingleton::pFun_ = 0; }
		inline ~CSingleton() { CSingleton::instance_ = 0; CSingleton::destroyed_ = true; CSingleton::pFun_ = 0; }

	private : 
		typedef void (*UserSuppliedDestroy)(T*);

	private : 
		static PointerType instance_;
		static bool destroyed_;
		static UserSuppliedDestroy pFun_;

	private : 
		inline explicit CSingleton(CSingleton const&) {}
		inline CSingleton& operator=(CSingleton const&) { return *this; }
	};	//	end of class CSingleton

	template<typename T, typename CreationPolicy, template <typename> class LifetimePolicy, template <typename> class ThreadingModel>
	typename CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::PointerType CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::Instance() {
		if ( CSingleton::instance_ == 0 ) {
			CBCSLock	m_Lock;
			CBCSLock::Syncronize sync(m_Lock);
			if ( CSingleton::instance_ == 0 ) {
				CSingleton::pFun_ = 0;
				if ( CSingleton::destroyed_ ) {
					OnDeadReference();
					CSingleton::destroyed_ = false;
				}
				CSingleton::instance_ = CreateInstance();
				try {
					ScheduleForDestruction(CSingleton::Destroy);
				} catch(...) {
					DestroyInstance(CSingleton::instance_);
				}
			}
		}
		return CSingleton::instance_;
	}

//	template<typename T, typename CreationPolicy, template <typename> class LifetimePolicy, template <typename> class ThreadingModel>
		typename CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::PointerType CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::InstancePtr() {
		if ( CSingleton::instance_ == 0 ) {
			CBCSLock	m_Lock;
			CBCSLock::Syncronize sync(m_Lock);
			if ( CSingleton::instance_ == 0 ) {
				CSingleton::pFun_ = 0;
				if ( CSingleton::destroyed_ ) {
					OnDeadReference();
					CSingleton::destroyed_ = false;
				}
				CSingleton::instance_ = CreateInstance();
				try {
					ScheduleForDestruction(CSingleton::Destroy);
				} catch(...) {
					DestroyInstance(CSingleton::instance_);
				}
			}
		}
		return CSingleton::instance_;
//	}

	template<typename T, typename CreationPolicy, template <typename> class LifetimePolicy, template <typename> class ThreadingModel>
	void CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::Destroy() {
		if ( CSingleton::instance_ != 0 ) {
			CBCSLock	m_Lock;
			CBCSLock::Syncronize sync(m_Lock);
			if ( CSingleton::instance_ != 0 ) {
				if (  CSingleton::pFun_ != 0 ) {
					CSingleton::pFun_(CSingleton::instance_);
				} else {
					DestroyInstance(CSingleton::instance_);
				}
				CSingleton::instance_ = 0;
				CSingleton::destroyed_ = true;
			}
		}
	}

	template<typename T, typename CreationPolicy, template <typename> class LifetimePolicy, template <typename> class ThreadingModel>
	void CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::Reset(typename CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::PointerType p, UserSuppliedDestroy pFun) {
		CBCSLock	m_Lock;
		CBCSLock::Syncronize sync(m_Lock);
		if ( CSingleton::instance_ != 0 ) {
			//DestroyInstance(CSingleton::instance_);
		} else if ( p != 0 ) {
			ScheduleForDestruction(CSingleton::Destroy);
		}
		CSingleton::instance_ = p;
		CSingleton::pFun_ = pFun;
	}

	template<typename T, typename CreationPolicy, template <typename> class LifetimePolicy, template <typename> class ThreadingModel>
	typename CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::PointerType CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::instance_ = 0;
	template<typename T, typename CreationPolicy, template <typename> class LifetimePolicy, template <typename> class ThreadingModel>
	bool CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::destroyed_ = false;
	template<typename T, typename CreationPolicy, template <typename> class LifetimePolicy, template <typename> class ThreadingModel>
	typename CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::UserSuppliedDestroy CSingleton<T, CreationPolicy, LifetimePolicy, ThreadingModel>::pFun_ = 0;



#endif*/