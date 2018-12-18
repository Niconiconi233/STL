#include<deque>

template<typename T,typename Sequence=std::deque<T> >
class queue {
	friend bool operator==(const queue& x, const queue& y);
	friend bool operator<(const queue& x, const queue& y);
public:



protected:
	Sequence c;

public:
	bool empty() { return c.empty(); }
	size_type size() { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back()const { return c.back(); }
	void push(value_type& x) { c.push_back(x); }
	void pop() { c.pop_front(); }
};

template<typename T, typename Sequence>
bool operator==(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
	return x.c == y.c;
}

template<typename T, typename Sequence>
bool operator<(const queue<T, Sequence>& x, const queue<T, Sequence>& y) {
	return x.c < y.c;
}