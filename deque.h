#pragma once
#include"alloc.h"
#include"construct.h"

template<typename T,typename Ref,typename Ptr,size_t buf>
struct __deque_iterator {
	typedef __deque_iterator<T, T&, T*, buf>			iterator;
	typedef __deque_iterator<T, const T&, const T*, buf>	const_iterator;
	static size_t buffer_size() { return __deque_buf_size(buf, sizeof(T)); }

	typedef random_access_iterator_tag iterator_category;
	typedef T								value_type;
	typedef ptrdiff_t						difference_type;
	typedef Ptr								pointer;
	typedef Ref								reference;
	typedef size_t							size_type;
	typedef T**								map_pointer;

	typedef __deque_iterator self;

	T* cur;
	T* first;
	T* last;
	map_pointer node;

	__deque_iterator(int x) :cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}

	reference operator*() const { return *cur; }
	pointer operator->() const { return &(operator*()); }

	difference_type operator-(const self& x) const {
		return difference_type(buffer_size())*(node - x->node - 1) + (cur - first) + (x->last - x->cur);
	}

	self&  operator++() {
		++cur;
		if (cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	self operator++(int) {
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--() {
		--cur;
		if (cur == first) {
			set_node(node - 1);
			cur = last;
		}
		return *this;
	}

	self operator--(int) {
		self tmp = *this;
		--*this;
		return tmp;
	}

	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
			cur += n;
		else {
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size()) : -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n) const {
		self tmp = *this;
		return tmp += n;
	}

	self& operator-(difference_type n) {
		return *this += -n;
	}

	self operator-(difference_type n) const {
		self tmp = *this;
		tmp -= n;
		return tmp;
	}

	reference operator[](difference_type n) {
		return *(*this + n);
	}

	bool operator==(const self& x) const { return cur == x->cur; }
	bool operator!= (const self& x) const { return !(*this == x); }
	bool operator<(const self& x) const {
		return (node == x->node) ? (cur < x.cur) : (node < x.node);
	}

	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}
};

inline size_t __deque_buf_size(size_t n, size_t sz)
{
	return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template<typename T, typename Alloc = alloc,size_t bufsiz = 0>
class deque {
public:
	typedef T		    value_type;
	typedef T*			pointer;
	typedef size_t		size_type;
	typedef T&			reference;


public:
	typedef __deque_iterator<value_type, reference, pointer, bufsiz> iterator;

protected:
	typedef pointer* map_pointer;
	typedef simple_alloc<value_type, Alloc> data_allocator;
	typedef simple_alloc<pointer, Alloc> map_allocator;

	iterator start;
	iterator finish;
	map_pointer map;
	size_type map_size;

public:
	deque(int n, const value_type& value) :start(0), finish(0), map(0), map_size(0) { fill_initialize(n, value); }
	iterator begin() { return start; }
	iterator end() { return finish; }

	reference front() { return *start; }
	reference back() {
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}
	size_type size() { return finish - start; }
	size_type max_size() { return size_type(-1); }
	bool empty() const { return finish == start; }

	void push_back(const value_type& x) {
		//有两个空间以上
		if (finish->cur != finish->last - 1) {
			construct(finish->cur, x);
			++finish->cur;
		}
		else
			push_back_aux(x);
	}

	void push_front(const value_type& x) {
		if (start->cur != start->first) {
			construct(start->cur - 1, x);
			--start->cur;
		}
		else
			push_front_aux(x);
	}

	void pop_back() {
		if (finish->cur != finish->first) {
			--finish->cur;
			destroy(finish->cur);
		}
		else {
			pop_back_aux();
		}
	}

	void pop_front() {
		if (start->cur != start->last - 1)
		{
			destroy(start->cur);
			++start->cur;
		}
		else {
			pop_front_aux();
		}
	}

	iterator erase(iterator pos) {
		iterator next = pos;
		++next;
		ptrdiff_t index = pos - start;
		if (index < (size() >> 1)) {//如果清除点前面的元素少 就移动前面的元素 
			//将一个源范围中的元素值分配到目标范围，循环访问元素的源序列并将它们分配在一个向后方向的新位置。
			copy_backward(start, pos, next);//first到pos-1之间的元素移动到next之前
			pop_front();
		}
		else {
			copy(next, finish, pos);
			pop_back();
		}

		return start + index;
	}

	iterator erase(iterator first, iterator last) {
		if (first == start && last == finish) {
			clear();
			return finish;
		}
		else {
			ptrdiff_t n = last - first;
			ptrdiff_t elems_before = first - start;
			if (elems_before < (size() - n) / 2) {
				copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer cur = start->node; cur < new_start.node; ++cur) {
					data_allocator::deallocate(*cur, iterator->buffer_size());
				}
				start = new_start;
			}
			else {
				copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);

				for (map_pointer cur = new_finish->node; cur < finish.node; ++cur)
					data_allocator::deallocate(*cur, iterator->buffer_size());
				finish = new_finish;
			}
			return start + elems_before;
		}
	}

	void clear() {
		for (map_pointer node = start->node + 1; node < finish.node; ++node) {
			destroy(*node, *node + iterator->buffer_size());
			data_allocator::deallocate(*node, iterator->buffer_size());
		}
		if (start->node != finish->node) {
			destroy(start->cur, start->last);
			destroy(finish->first, finish->cur);
			data_allocator::deallocate(finish->first, iterator->buffer_size());
		}
		else {
			destroy(start->cur, finish->cur);
			finish = start;
		}
	}

	iterator insert(iterator position, const value_type& x) {
		if (position->cur == start->cur) {
			push_front(x);
			return start;
		}
		else if (position->cur == finish->cur) {
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
		{
			return insert_aux(position, x);
		}
	}

private:
	void fill_initialize(size_type n, const T& value)
	{
		create_map_and_nodes(n);
		map_pointer cur;
		try
		{
			for (cur = start->node; cur < finish.node; ++cur)
				uninitialized_fill(cur, *cur + iterator->buffer_size(), value);
			uninitialized_fill(finish->first, finish->cur, value);
		}
		catch (...)
		{

		}
	}

	void create_map_and_nodes(size_type n) {
		//需要节点数 元素数/buff_size
		size_type num_nodes = n / iterator->buffer_size() + 1;
		//map节点数最少为8 否则为节点数+2
		map_size = max(/*initial_map_size()*/ 8, num_nodes + 2);
		//分配空间
		map = map_allocator::allocate(map_size);
		//头迭代器
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		//为迭代器
		map_pointer nfinish = nstart + num_nodes - 1;

		map_pointer cur;
		try
		{
			for (cur = nstart; cur < nfinish; ++cur)
				*cur = allocate_node();//创建map
		}
		catch (...)
		{

		}
		start->set_node(nstart);
		finish->set_node(nfinish);
		start->cur = start->first;
		finish->cur = finish->first + n % iterator->buffer_size();
	}

	void push_back_aux(const value_type& x) {
		value_type x_copy = x;
		reserve_map_at_back();
		//初始化新节点
		*(finish->node + 1) = allocate_node();
		try
		{
			construct(finish->cur, x_copy);
			finish->set_node(finish.node + 1);//修改尾迭代器
			finish->cur = finish->first;
		}
		catch (...)
		{
			//deallocate_node(*(finish->node + 1));
		}
	}

	void push_front_aux(const value_type& x) {
		value_type x_copy = x;
		reserve_map_at_front();
		*(start->node + 1) = allocate_node();
		try
		{
			start->set_node(start->node - 1);
			start->cur = start->last - 1;
			construct(start->cur, x_copy);

		}
		catch (...)
		{
			start->set_node(start->node + 1);
			start->cur = start->first;
			deallocate_node(*start->node - 1);
			throw;
		}
	}

	void reserve_map_at_back(size_type nodes_to_add = 1) {
		//后面的空间是否足够 从0开始
		if (nodes_to_add + 1 > map_size - (finish->node - map)) {
			reallocate_map(nodes_to_add, false);
		}
	}

	void reserve_map_at_front(size_type nodes_to_add = 1) {
		if (nodes_to_add > start.node - map)
			reallocate_map(nodes_to_add, true);
	}

	void reallocate_map(size_type nodes_to_add, bool add_at_front) {
		size_type old_num_nodes = finish->node - start->node + 1;//+1?
		size_type new_num_nodes = old_num_nodes + nodes_to_add;
		map_pointer new_nestart;
		//如果空间还够
		if (map_size > 2 * new_num_nodes) {
			//计算新的start
			new_nestart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			if (new_nestart < start.node)
				copy(start->node, finish->node + 1, new_nestart);
			else
				copy_backward(start->node, finish->node + 1, new_nestart + old_num_nodes);
		}
		else {
			//计算新的大小
			size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
			map_pointer new_map = map_allocator::allocate(new_map_size);
			new_nestart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
			copy(start->node, finish->node + 1, new_nestart);
			map_allocator::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;
		}
		start->set_node(new_nestart);
		finish->set_node(new_nestart + old_num_nodes - 1);
	}

	void pop_back_aux() {
		deallocate_node(finish->first);
		finish->set_node(finish->node - 1);
		finish->cur = finish->last - 1;
		destroy(finish->cur);
	}

	void pop_front_aux() {
		deallocate_node(start->first);
		start->set_node(start->node + 1);
		start->cur = start->first;
	}

	iterator insert_aux(iterator position, const value_type& x) {
		ptrdiff_t	index = position - start;
		value_type x_copy = x;
		if (index < size() / 2) {
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			position = start + index;
			iterator pos1 = position;
			++pos1;
			copy(front2, pos1, front1);
		}
		else {
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			position = start + index;
			copy_backward(position, back2, back1);
		}
		*position = x_copy;
		return position;
	}
	map_pointer allocate_node() {
		return data_allocator::allocate(iterator->buffer_size());
	}

	void deallocate_node(T* ptr) {
		data_allocator::deallocate(ptr, iterator->buffer_size());
	}
};