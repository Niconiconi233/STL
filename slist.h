#pragma once
#include"vcruntime.h"
#include<iostream>
#include"iterator_traits.h"
#include"alloc.h"
#include"construct.h"

struct __slist_node_base {
	__slist_node_base* next;
};

template<typename T>
struct __slist_node :public __slist_node_base {
	T data;
};

inline __slist_node_base* __slist_make_link(__slist_node_base* prenode, __slist_node_base* newnode) {
	newnode->next = prenode->next;
	prenode->next = newnode;
	return newnode;
}

inline size_t __slist_size(__slist_node_base* node) {
	size_t size = 0;
	for (; node != 0; node = node->next)
		++size;
	return size;
}

struct __slist_iterator_base {
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef forward_iterator_tag iterator_category;

	__slist_node_base* node;
	__slist_iterator_base(__slist_node_base* x) :node(x) {}
	void incr() { node = node->next; }//moveforward
	bool operator==(const __slist_iterator_base& lhs)const {
		return node == lhs.node;
	}
	bool operator!=(const __slist_iterator_base& lhs) const {
		return node != lhs.node;
	}
};

template<typename T,typename Ref,typename Ptr>
struct __slist_iterator :public __slist_iterator_base {
	typedef __slist_iterator<T, T&, T*>				iterator;
	typedef __slist_iterator<T, const T&, const T*>	const_iterator;
	typedef __slist_iterator<T, Ref, Ptr>				self;

	typedef T	value_type;
	typedef Ref	reference;
	typedef Ptr	pointer;
	typedef __slist_node<T>	list_node;

	__slist_iterator(list_node* x) :__slist_iterator_base(x) {}
	__slist_iterator() :__slist_iterator_base(0) {}
	__slist_iterator(const iterator& x) :__slist_iterator_base(x.node) {}

	reference operator*()const { return ((list_node*)node)->data; }
	pointer operator->()const { return &(operator*()); }
	self& operator++() {
		incr();
		return *this;
	}

	self operator++(int) {
		__slist_iterator tmp = *this;
		incr();
		return tmp;
	}

};

template<typename T,typename Alloc=alloc>
class slist {
public:
	typedef T	value_type;
	typedef value_type* pointer;
	typedef value_type&	reference;
	typedef const value_type* const_pointer;
	typedef const value_type& const_reference;
	typedef size_t	size_type;
	typedef ptrdiff_t difference_type;

	typedef __slist_iterator<T, T&, T*>	iterator;
	typedef __slist_iterator<T, const T&, const T*> const_iterator;

private:
	typedef __slist_node<T> list_node;
	typedef __slist_node_base list_base_node;
	typedef __slist_iterator_base iterator_base;
	typedef simple_alloc<list_node, Alloc> list_node_allocator;

	list_node* create_node(const value_type& x) {
		list_node* node = list_node_allocator::allocate();
		construct(&node->data, x);
		node->next = nullptr;
		//std::cout << node << std::endl;
		return node;
	}

	void destory_node(list_node* ptr) {
		destroy(&ptr->data);
		list_node_allocator::deallocate(ptr);
	}

private:
	list_base_node head;
public:
	slist() { head.next = nullptr; }
	~slist() { clear(); }

	iterator begin() { return iterator((list_node*)head.next); }
	iterator end() { return iterator(0); }
	size_type size()const { return __slist_size(head.next); }
	bool empty()const { return head.next == nullptr; }

	void swap(slist& l) {
		list_base_node* tmp = head.next;
		head.next = l.head.next;
		l.head.next = tmp;
	}

	reference front() {
		return  (((list_node*)head.next)->data);
	}

	void push_front(const value_type& x) {
		__slist_make_link(&head, create_node(x));
	}

	void pop_front() {
		list_node* tmp = (list_node*)head.next;
		head.next = tmp->next;
		destroy(tmp);
	}

	iterator insert(const iterator& position, const value_type& x) {
		__slist_make_link(position, create_node(x));
	}

	void clear() {
		list_node* cur = (list_node*)head.next;
		while (cur) {
			//std::cout << cur << std::endl;
			list_node* tmp = (list_node*)cur->next;
			destory_node(cur);
			cur = tmp;
		}
	}
};
