#pragma once
#include<iostream>
#if 0
#include<new>
#define _THROW_BAD_ALLOC throw bad_alloc
#elif !defined(_THROW_BAD_ALLOC)
#include<iostream>
#define _THROW_BAD_ALLOC std::cerr<<"cout of memory"<<std::endl;
#endif

template<int inst>
class _malloc_alloc_template {
private:
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*, size_t);
	static void (*_malloc_alloc_oom_handler)();

public:
	static void* allocate(size_t n)
	{
		std::cout << "using _malloc_alloc_template" << std::endl;
		void* result = malloc(n);
		if (0 == result)//malloc not success
			result = oom_malloc(n);//handler other fun
		return result;
	}

	static void deallocate(void* p, size_t/*n*/)
	{
		std::cout << "using _malloc_alloc_template deallocate" << std::endl;
		free(p);
	}
	static void *reallocate(void* p, size_t/*old_size*/,size_t new_size)
	{
		void* result = realloc(p, new_size);
		if (0 == result)
			result = oom_realloc(p, new_size);
		return result;
	}

	static void (*set_malloc_handler(void (*f)()))()//pointer fun
	{
		void (* old)() = _malloc_alloc_oom_handler;
		_malloc_alloc_oom_handler = f;
		return(old);
	}

};

template<int inst>
void* _malloc_alloc_template<inst>::oom_realloc(void* p, size_t n)
{
	void(*my_malloc_handler)();
	void* result;
	for (;;)
	{
		my_malloc_handler = _malloc_alloc_oom_handler;
		if (0 == my_malloc_handler) { _THROW_BAD_ALLOC; }//cerr
		(*my_malloc_handler)();//handler fun free(buf)
		result = realloc(p, n);//try to malloc
		if (result)
			return result;

	}
}

template<int inst>
void* _malloc_alloc_template<inst>::oom_malloc(size_t n)
{
	void(*my_malloc_handler)();
	void* result;
	for (;;)
	{
		my_malloc_handler = _malloc_alloc_oom_handler;
		if (0 == my_malloc_handler){_THROW_BAD_ALLOC;}
		(*my_malloc_handler)();
		result = malloc(n);
		if (result)
			return result;
	}
}

template<int inst>
void(* _malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = 0;

typedef _malloc_alloc_template<0> malloc_alloc;

