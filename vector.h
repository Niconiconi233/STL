#pragma once
#include"alloc.h"
#include"construct.h"
#include <algorithm>

template<typename T, typename Alloc = alloc >
class vector {
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type* iterator;
	typedef value_type& reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

protected:
	typedef simple_alloc<value_type, Alloc> data_allocator;

	iterator start;
	iterator finish;
	iterator end_of_stroage;

	void insert_aux(iterator position, const T& x);//动态改变数组大小
	void deallocate() {
		if (start)
			data_allocator::deallocate(start, end_of_stroage - start);
	}
	void fill_initialize(size_type n, const T& x) {//生成的数组大小为指定大小
		start = allocate_and_fill(n, x);
		finish = start + n;
		end_of_stroage = finish;
	}

public:
	iterator begin() const { return start; }
	iterator end() const { return finish; }
	size_type size() const { return size_type(end() - begin()); }
	size_type capacity() const { return size_type(end_of_stroage - begin()); }//
	reference operator[](size_type n) { return *(begin() + n); }
	void insert(iterator position, size_type n, const T& x);

	void insert(size_type position, const T& x)
	{
		insert(start + position, 1, x);
	}

	
	//constructor
	vector() :start(0), finish(0), end_of_stroage(0) {}
	vector(size_type n, const T& x) { fill_initialize(n, x); }
	vector(int n, const T& x) { fill_initialize(n, x); }
	vector(long n, const T& x) { fill_initialize(n, x); }
	explicit vector(size_type n) { fill_initialize(n, T()); }

	~vector() { destroy(start, finish);
				deallocate();
	}

	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }
	void push_back(const T& x) {
		if (finish != end_of_stroage)
		{
			construct(finish, x);
			++finish;
		}
		else
			insert_aux(end(), x);
	}

	void pop_back() {
		--finish;
		destroy(finish);
	}

	iterator erase(iterator first, iterator last)
	{
		iterator i = std::copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}

	iterator erase(iterator position)
	{
		if (position + 1 != end())
		{
			std::copy(position + 1, finish, position);//move value from position+1 to finish to position
			--finish;
			destroy(finish);
			return position;
		}
	}

	void resize(size_type new_size, const T& x)
	{
		if (new_size < size())
			erase(begin() + new_size, end());
		else
			insert(end(), new_size - size(), x);//
	}

	void resize(size_type new_size) { resize(new_size, T()); }
	void clear() { erase(begin(), end()); }//

protected:
	iterator allocate_and_fill(size_type n, const T& x)
	{
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}



};

template<typename T, typename Alloc /*= alloc */>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
{
	if (n != 0)
	{
		if (size_type(end_of_stroage - finish) >= n)//剩余空间足够
		{
			T x_copy = x;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n)
			{
				uninitialized_copy(finish - n, finish, finish);
				finish += n;
				std::copy_backward(position, old_finish - n, old_finish);
				std::fill(position, position + n, x_copy);
			}
			else
			{
				uninitialized_fill_n(finish, n - elems_after, x_copy);
				finish += n;
				uninitialized_copy(position, old_finish, finish);
				finish += elems_after;
				std::fill(position, old_finish, x_copy);
			}
		}
		else
		{
			const size_type old_size = size();
			const size_type len = old_size + std::max(old_size, n);

			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try{
				new_finish = uninitialized_copy(start,position,new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (...)
			{
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			destroy(start, finish);
			deallocate();
			start = new_start;//迭代器失效
			finish = new_finish;
			end_of_stroage = new_start + len;
		}
	}
}

template<typename T, typename Alloc /*= alloc */>
void vector<T, Alloc>::insert_aux(iterator position, const T& x)
{
	if (finish != end_of_stroage) {
		construct(finish, *(finish - 1));
		++finish;
		T copy = x;
		std::copy_backward(position, finish - 2, finish - 1);
		*position = copy;
	}
	else
	{
		const size_type old_size = size();
		const size_type len = old_size != 0 ? 2 * old_size : 1;//分配两倍的大小
		iterator new_start = data_allocator::allocate(len);
		iterator new_finish = new_start;
		try
		{
			new_finish = uninitialized_copy(start, position, new_start);
			construct(new_finish, x);
			++new_finish;
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (...)
		{
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, len);
			throw;
		}
		destroy(begin(), end());
		deallocate();

		start = new_start;//迭代器失效
		finish = new_finish;
		end_of_stroage = new_start + len;
	}
}
