#pragma once
//构造函数和析构函数
//二级函数必须在一级函数的前面
#include<new.h>
#include <algorithm>
#include"iterator_traits.h"
#include"type_traits.h"

template<typename T1,typename T2>
inline void construct(T1* p, const T2& value)
{
	new(p) T1(value);//在p指向的地址调用拷贝构造函数
}

template<typename T>
inline void destroy(T* pointer)
{
	pointer->~T();
}

template<typename ForwardIterator, typename T>
inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
{
	typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
	_destroy_aux(first, last, trivial_destructor());
}

template<typename ForwardIterator>//自定义类型
inline void _destroy_aux(ForwardIterator first, ForwardIterator last, __false_type)
{
	for (; first < last; ++first)
		destroy(&*first);
}

template<typename ForwardIterator>//基本类型
inline void _destroy_aux(ForwardIterator first, ForwardIterator last, __true_type)
{

}

template<typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
	_destroy(first, last, value_type(first));
}

inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}

//uninitialized_fill_n

template<typename ForwardIterator, typename Size, typename T, typename T1>
inline ForwardIterator _Uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*)
{
	typedef typename __type_traits<T1>::is_POD_type is_POD;
	return _Uninitialized_fill_n_aux(first, n, x, is_POD());
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator _Uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type)
{
	return std::fill_n(first, n, x);
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator _Uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type)
{
	ForwardIterator cur = first;
	for (; n > 0; --n, ++cur)
	{
		construct(&*cur, x);
	}
	return cur;

}

template<typename ForwardIterator,typename Size,typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
{
	return _Uninitialized_fill_n(first, n, x, value_type(first));
}

//uninitialized_copy

template<typename InputIterator, typename ForwardIterator, typename T>
inline ForwardIterator _Uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
	typedef typename __type_traits<T>::is_POD_type is_POD;
	return _Uninitialized_copy_aux(first, last, result, is_POD());
}

template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator _Uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type)
{
	return std::copy(first, last, result);
}

template<typename InputIterator, typename ForwardIterator>
inline ForwardIterator _Uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type)
{
	ForwardIterator cur = result;
	for (; first != last; ++first, ++cur)
	{
		construct(&*first, *result);
		return cur;
	}
}

template<typename InputIterator,typename ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,ForwardIterator result)
{
	return _Uninitialized_copy(first, last, result, value_type(first));
}



//uninitialized_fill

template<typename ForwardIterator, typename T, typename T1>
inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*)
{
	typedef typename __type_traits<T1>::is_POD is_POD;
	_uninitialized_fill_aux(first, last, x, is_POD());
}

template<typename ForwardIterator, typename T>
inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __true_type)
{
	fill(first, last, x);
}

template<typename ForwardIterator, typename T>
inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, __false_type)
{
	ForwardIterator cur = first;
	for (; cur != last; ++cur)
		construct(&*cur, x);
}

template<typename ForwardIterator,typename T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{
	return _uninitialized_fill(first, last, x);
}