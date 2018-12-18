#pragma once
#include<vector>
#include<algorithm>
template<typename T,typename Sequence = std::vector<T>, typename Compare = std::less<typename Sequence::value_type> >
class Priority_queue {
public:
	typedef typename Sequence::value_type   value_type;
	typedef typename Sequence::size_type	size_type;
	typedef typename Sequence::reference	reference;
	typedef typename Sequence::const_reference	const_reference;

protected:
	Sequence c;
	Compare comp;
public:
	Priority_queue() :c() {}
	explicit Priority_queue(const Compare& x) :comp(x), c() {}
	template<typename InputIterator>
	Priority_queue(InputIterator first, InputIterator last, const Compare& x) : c(first, last), comp(x) { std::make_heap(c.begin(), c.end(), comp); }
	template<typename InputIterator>
	Priority_queue(InputIterator first, InputIterator last) : c(first, last) { std::make_heap(c.begin(), c.end(), comp); }
	bool empty() { return c.empty(); }
	size_type size() const { return c.size(); }
	const_reference top() { return c.front(); }
	void push(const value_type& x) {
		c.push_back(x);
		std::push_heap(c.begin(), c.end(),comp);
	}

	void pop() {
		std::pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
	}
};
