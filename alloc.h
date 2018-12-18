#pragma once
#include"_default_alloc_template.h"
#include "_malloc_alloc_template.h"
//// in preprocess
#ifdef __STL_THREADS
# include <stl_threads.h> // stl 的线程，只是对linux或windows线程的一个封装
# define __NODE_ALLOCATOR_THREADS true
# ifdef __STL_SGI_THREADS
#   define __NODE_ALLOCATOR_LOCK if (threads && __us_rsthread_malloc) \
                { _S_node_allocator_lock._M_acquire_lock(); }  // 获取锁
#   define __NODE_ALLOCATOR_UNLOCK if (threads && __us_rsthread_malloc) \
                { _S_node_allocator_lock._M_release_lock(); }  // 释放锁
# else /* !__STL_SGI_THREADS */
#   define __NODE_ALLOCATOR_LOCK \
        { if (threads) _S_node_allocator_lock._M_acquire_lock(); }
#   define __NODE_ALLOCATOR_UNLOCK \
        { if (threads) _S_node_allocator_lock._M_release_lock(); }
# endif
#else /* !__STL_THREADS */
#   define __NODE_ALLOCATOR_LOCK
#   define __NODE_ALLOCATOR_UNLOCK
#   define __NODE_ALLOCATOR_THREADS false
#endif

# ifdef __STL_THREADS
static _STL_mutex_lock _S_node_allocator_lock; // 互斥锁变量
# endif
class _Lock {
public:
	_Lock() { __NODE_ALLOCATOR_LOCK; }
	~_Lock() { __NODE_ALLOCATOR_UNLOCK; }
};

//#define _USE_MALLOC

#ifdef _USE_MALLOC

typedef _malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc;
#else

typedef _default_alloc_template<__NODE_ALLOCATOR_THREADS, 0> alloc;

#endif

template<typename T,typename Alloc>
class simple_alloc {
public:
	static T* allocate(size_t n)
	{
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate(void)
	{
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* p, size_t n)
	{
		if (0 != n)
			Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T* p)
	{
		Alloc::deallocate(p, sizeof(T));
	}
};